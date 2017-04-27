#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <math.h>
#include <malloc.h>
#include <sys/uio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/**************************************************************************
 interpolate_SFT_temporal_nas_32day.c
 Karyn Tabor
 last update 10/16/2015

 temporal interpolation for 32-bit float binary files.

 This program uses temporal interpolation of the past 32 days to fill in flagged
 values in a 32-bit float binary image. A missing value
 is defined as a flagged value occurring underneath the input mask.

 When a missing value is found, the value is filled with the
 average value of 32 previous daily images.


 Inputs:

	One 8 bit unsigned raw binary mask, with pixels to interpolate
        nonzero and pixels that should be flagged in the output (for
        example, water pixels) set to zero.

	One raw binary float image to be interpolated.

	The mask and input image must be coregistered and have the
        same number of rows and columns.


 Output:

	One interpolated raw binary 16 bit unsigned image.
	One raw binary 8 byte image containing the number of previous
	values used for the average. ) values are ocean andflag values
	are pixels were no previous days were found to have data to fill
	the current image.


 Compiles with:
 "gcc -g -o interpolate_SFT_temporal_nas interpolate_SFT_temporal_nas.c -lm"

 Must have functions allocate_binary.c, read_binary.c, and write_binary.c
 in the same directory for compiling.


 Command line arguments:

	<maskfn> <year> <jday> <ncols> <nrows> <flagval>
 where:
	<maskfn> = filename for the land mask file
	<indir> = Path to input directory
	<outdir> = Path to output directory
	<year> = 4 digit year
	<jday> = julian day
	<ncols> = number of columns in input file
	<nrows> = number of rows in input file
	<flagval> = flag value in the input file

 Filenames can have no more than 100 characters.

**************************************************************************/



int main(int argc, char *argv[])
{


/***********************************************************************
    Command line arguments

***********************************************************************/

   char mask_fn[101],infile_fn[101], outfile_fn[101], count_fn[101], in_dir[101], out_dir[101];
   int year, jday, prevday, ncols,nrows,total_pixels;
   signed short int flag;

   /*stores dates of previous 8 days*/
   int date_array[32][2];


   /* pointers to input and output files */
   unsigned char *mask, *count;
   float *infile, *tempin, *sum;
   signed short int *outfile;


   /* loop counter */
   int countflag=0;
   int i, j, k;
   float threshold;


   /** Check args, if incorrect specify usage **/

   if( argc!=9 )
   {
      printf("Usage:\n\n");
      printf("interpolate_SFT_temporal_nas maskfn in_dir out_dir year ");
      printf("jday ncols nrows flagval\n\n");
      printf("See source code file for command line argument values\n");
      return 1;
   }



   /********************************************************************
    read command line arguments
   ********************************************************************/

   printf("Running distance-weighted interpolation\n\n");

   sprintf(mask_fn,argv[1]);
   printf("The mask file is %s\n",mask_fn);
   sprintf(in_dir,argv[2]);
   sprintf(out_dir,argv[3]);
   year=atoi(argv[4]);
   jday=atoi(argv[5]);
   ncols=atoi(argv[6]);
   nrows=atoi(argv[7]);
   printf("Input image has %d columns and %d rows\n",ncols,nrows);
   total_pixels= ncols*nrows;

   threshold=total_pixels*0.99;

   flag=atoi(argv[8]);
   printf("The new flag value is %d\n",flag);


   /**********************************************************************
    Memory allocation
   **********************************************************************/

   allocate_binary(total_pixels,"uchar",(void *)(&mask));
   allocate_binary(total_pixels,"float",(void *)(&infile));
   allocate_binary(total_pixels,"float",(void *)(&sum));
   allocate_binary(total_pixels,"uchar",(void *)(&count));
   allocate_binary(total_pixels,"float",(void *)(&tempin));
   allocate_binary(total_pixels,"sint",(void *)(&outfile));



   /**********************************************************************
    Read mask and input file
   **********************************************************************/

   read_binary(total_pixels,mask_fn,"uchar",(void *)(&mask));

   sprintf(infile_fn,"%s%d/SFT_MOD07_%d%.3d_Day_IDN.bin", in_dir, year, year,jday);
   read_binary(total_pixels,infile_fn,"float",(void *)(&infile));

   /*count file name*/
   sprintf(count_fn,"%s%d/SFT_MOD07_%d%.3d_Day_IDN_interp_count.bin", out_dir, year, year,jday);
   /*outfile name*/
   sprintf(outfile_fn,"%s%d/SFT_MOD07_%d%.3d_Day_IDN_interp.bin", out_dir, year, year,jday);

  /*******************************************************************
   change 0 pixels to flag values
   *******************************************************************/

  for(i=0; i<total_pixels; i++)
   {
	  	/*flagging bad values*/
	   	if( *(infile+i) <= 0)
	         {
	            *(infile+i)= flag;
		     countflag++;
	         }

	/*initiatilizing count sum arrays at 0*/
	*(count+i)=0;
	*(sum+i)=0;
   }
/*exit program if there is less then 99% if the image to interpolate from*/
	if (countflag >= threshold)
	{
	printf("Less the 99 percent of image is nodata. Exiting....\n");
	exit(0);
	}

   /*******************
   create array for julian dates of previous week
   *********************/
prevday=jday;
printf("jday is %d\n", prevday);
    for(i=0; i<32; i++)
   {
	prevday=prevday-1;
	   if (prevday==0)
	   {
		   year=year-1;
	   		if(year== 2000 || year== 2004 || year== 2008 || year== 2012 || year== 2016 || year== 2020 || year== 2024 || year== 2028 || year== 2032 || year== 2036 )
	   		{
				prevday=366;

			}
			else
			{
				prevday=365;
			}
	   }
	   date_array[i][0]=year;
	   date_array[i][1]=prevday;

   }

/*testing*/

/*for (i=0; i<8;i++)
{
	for(j=0;j<2;j++)
	{
	printf("%d\t", date_array[i][j]);
	}
printf("\n");
}*/

/**************************/
   /**********************************************************************
       Create 32-day average from previous week
      **********************************************************************/

   for(i=0; i<32; i++)
   {
   sprintf(infile_fn,"%s%d/SFT_MOD07_%d%.3d_Day_IDN.bin", in_dir, date_array[i][0], date_array[i][0],date_array[i][1]);
   read_binary(total_pixels,infile_fn,"float",(void *)(&tempin));

   		/*pixel loop to sum values*/
   		for(j=0; j<total_pixels; j++)
  		 {
			 if( *(infile+j) == flag && *(mask+j) == 1 )
			 {
				if (*(tempin+j) >0 )
				{
				 *(sum+j)=*(sum+j)+*(tempin+j);
				 *(count+j)=*(count+j)+1;
				}
			 }

  		 }

   memset(tempin, 0, total_pixels);
   }

  free(tempin);

   /**********************************************************************
    Pixels loop
   **********************************************************************/

   for(i=0; i<total_pixels; i++)
   {
	if(*(count+i)>0)
	{
	/*printf("sum %f\tcount %d\n",*(sum+i), *(count+i));*/
	}

	 if (  *(mask+i) == 0)
	   {
		 *(outfile+i)=0;
	   }
	 else if( *(infile+i) == flag )
	  {
		 if( *(count+i) == 0)
		 {
		 *(outfile+i)=-1;
		 }
		 else
		 {
			*(outfile+i)=(int)((*(sum+i))/(*(count+i))*10);

		 }
	  }
	  else
	  {
		  *(outfile+i)=(int)(*(infile+i)*10);

	  }

   } /* close "i" loop (increment row counter) */

/*for(i=0; i<total_pixels; i++)
   {
printf("%d\t",*(outfile+i));
printf("%d\n",*(count+i));}*/
   /***************************************************************
    Free memory allocated to inputs
    ***************************************************************/

   free(mask);
   free(infile);
   free(sum);


   /***************************************************************
    write output files and free memory
    ***************************************************************/

   write_binary(total_pixels,outfile_fn,"sint",(void *)(&outfile));
   free(outfile);

   write_binary(total_pixels,count_fn,"uchar",(void *)(&count));
   free(count);


   return 0;
}
