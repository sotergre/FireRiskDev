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
 dw_interp_MOD07_TP.c
 Jen Small
 last update 2/19/2002
 Karyn Tabor
 3/7/2008

 Distance-weighted interpolation.

 This program uses distance-weighted interpolation to fill in flagged
 values in a floating point (32 bit) binary image. A missing value
 is defined as a flagged value occurring within the input mask.

 When a missing value is found, a search for nonflagged neighboring
 pixels is done, with closest pixels (in terms of Euclidean distance)
 searched first. The distance-weighted average of the four closest
 pixels replaces the missing value.

 Inputs:

	One 8 bit unsigned raw binary mask, with pixels to interpolate
        nonzero and pixels that should be flagged in the output (for
        example, water pixels) set to zero.

	One raw binary signed int image  AT
        to be interpolated.

	One raw binary signed int image DPT
        to be interpolated.

	The mask and input image must be coregistered and have the
        same number of rows and columns.


 Output:

	One interpolated raw binary floating point AT image.
	One interpolated raw binary floating point DPT image.
	One interpolated raw binary floating point DIS image.


 Compiles with "make -f Make.dw_interp"


 Command line arguments:

	<maskfn> <inatpfn> <indptfn> <outatpfn> <outdptfn> <outdisfn> <ncols> <nrows> <flagval>

 where:
	<maskfn> = filename for the land mask file
	<inatpfn> = filename for the AT input file (to be interpolated)
	<indptfn> = filename for the DPT input file (to be interpolated)
 	<outatpfn> = filename for the output AT (interpolated) file
 	<outdptfn> = filename for the output DPT (interpolated) file
 	<outdisfn> = filename for the output interpolation distance file
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

   char maskfn[101],inatpfn[101],indptfn[101], outatpfn[101],outdptfn[101],outdisfn[101];
   int ncols,nrows,total_pixels;
   int flag;

   /* The calculated fill value using distance weighted averaging */

   float dwavg_atp, dwavg_dpt;


   /* The four nearest neighbor values used to get the dw average */

   float atpvals[4],dptvals[4];


   /* pointers to input and output files */

   unsigned char *mask;
   short signed int *inatpfile, *indptfile, *outatpfile,*outdptfile,*outdisfile;


   /* Distance from missing value to a searched pixel; max distance
      between any two pixels in the input image */

   float distance,max_distance;


   /* distances from four closest unflagged pixels to flagged pixel,
      sum of the four distances */

   float filldist[4],dsum;


   /* weight to apply to each value in the distance weighted average */

   float weight[4];


   /* row, column loop counters */

   register int r,c;


   /* count pixels found so far in search */

   int pixfound=0;


   /* offsets from row and column value of missing pixel to row and column
      value of search pixel */

   int offset1,offset2,offset[8];


   /* loop counter */

   int i;


   /* sum of row and column increments */

   int sum;



   /********************************************************************
    read command line arguments
   ********************************************************************/

   printf("Running distance-weighted interpolation\n\n");

   sprintf(maskfn,argv[1]);
   printf("The mask file is %s\n",maskfn);

   sprintf(inatpfn,argv[2]);
   printf("The input file is %s\n",inatpfn);

   sprintf(indptfn,argv[3]);
   printf("The input file is %s\n",indptfn);

   sprintf(outatpfn,argv[4]);
   printf("The output file is %s\n",outatpfn);

   sprintf(outdptfn,argv[5]);
   printf("The output file is %s\n",outdptfn);

   sprintf(outdisfn,argv[6]);
   printf("The output file is %s\n",outdisfn);

   ncols=atoi(argv[7]);
   nrows=atoi(argv[8]);
   printf("Input image has %d columns and %d rows\n",ncols,nrows);
   total_pixels= ncols*nrows;
   max_distance= sqrt((ncols*ncols)+(nrows*nrows));

   flag=atoi(argv[9]);
   printf("The flag value is %d\n",flag);


   /**********************************************************************
    Memory allocation
   **********************************************************************/

   allocate_binary(total_pixels,"uchar",(void *)(&mask));
   allocate_binary(total_pixels,"sint",(void *)(&inatpfile));
   allocate_binary(total_pixels,"sint",(void *)(&indptfile));
   allocate_binary(total_pixels,"sint",(void *)(&outatpfile));
   allocate_binary(total_pixels,"sint",(void *)(&outdptfile));
   allocate_binary(total_pixels,"sint",(void *)(&outdisfile));



   /**********************************************************************
    Read mask and input file
   **********************************************************************/

	if(read_binary(total_pixels,maskfn,"uchar",(void *)(&mask))==-1)
	{	
		printf("\n%s is missing!\n\n", maskfn);
		exit(0);
	}
	if(read_binary(total_pixels,inatpfn,"sint",(void *)(&inatpfile))==-1)
	{
		printf("\n%s is missing!\n\n", inatpfn);
		exit(0);
	}
	if(read_binary(total_pixels,indptfn,"sint",(void *)(&indptfile))==-1)
	{
		printf("\n%s is missing!\n\n", inatpfn);
		exit(0);
	}



   /**********************************************************************
    Pixels loop
   **********************************************************************/

   for(r=0; r<nrows; r++)
   {

      for(c=0; c<ncols; c++)
      {


         /*******************************************************************
          flag pixels not under the mask in the output file
         *******************************************************************/

         if( *(mask+(r*ncols)+c)==0 )
         {
            *(outatpfile+(r*ncols)+c)= flag;
            *(outdptfile+(r*ncols)+c)= flag;
         }

         /*******************************************************************
          If input has a missing value under the mask then interpolate
         *******************************************************************/

         else if( *(inatpfile+(r*ncols)+c) <= 0)
         {

            pixfound=0;
            distance=1;
            sum=1;
            offset1=0;
            offset2=1;

            while( pixfound<4 && distance<max_distance )
            {
               while( offset2>=0 )
               {

                  /******************************************************
                   Euclidean distance from missing value pixel
                   to a fill pixel at these row,col offsets.
                  ******************************************************/

                  distance= sqrt((offset1*offset1)+(offset2*offset2));


                  /******************************************************
                   For each distance there are up to eight pixels to
                   search, depending on whether (*) both offsets
                   are nonzero and whether (**) offsets are not equal:

              		(row+offset1,col+offset2)
			(row-offset1,col-offset2)
			(row-offset1,col+offset2) (*)
			(row+offset1,col-offset2) (*)
                        (row+offset2,col+offset1) (**)
			(row-offset2,col-offset1) (**)
			(row-offset2,col+offset1) (*,**)
 			(row+offset2,col-offset1) (*,**)
                  ******************************************************/

                  offset[0]= ((r+offset1)*ncols) + (c+offset2);
                  if( offset[0]<0 || offset[0]>=total_pixels )
                     offset[0]= -1;

                  offset[1]= ((r-offset1)*ncols) + (c-offset2);
                  if( offset[1]<0 || offset[1]>=total_pixels )
                     offset[1]= -1;


                  if( offset1>0 && offset2>0 )
                  {
                     offset[2]= ((r-offset1)*ncols) + (c+offset2);
                     if( offset[2]<0 || offset[2]>=total_pixels )
                        offset[2]= -1;

                     offset[3]= ((r+offset1)*ncols) + (c-offset2);
                     if( offset[3]<0 || offset[3]>=total_pixels )
                        offset[3]= -1;

                     if( offset1!=offset2 )
                     {
                        offset[4]= ((r+offset2)*ncols) + (c+offset1);
                        if( offset[4]<0 || offset[4]>=total_pixels )
                           offset[4]= -1;

                        offset[5]= ((r-offset2)*ncols) + (c-offset1);
                        if( offset[5]<0 || offset[5]>=total_pixels )
                           offset[5]= -1;

                        offset[6]= ((r-offset2)*ncols) + (c+offset1);
                        if( offset[6]<0 || offset[6]>=total_pixels )
                           offset[6]= -1;

                        offset[7]= ((r+offset2)*ncols) + (c-offset1);
                        if( offset[7]<0 || offset[7]>=total_pixels )
                           offset[7]= -1;
                     }
                     else
                     {
                        offset[4]= -1;
                        offset[5]= -1;
                        offset[6]= -1;
                        offset[7]= -1;
                     }
                  }
                  else
                  {
                     offset[2]= -1;
                     offset[3]= -1;

                     offset[4]= ((r+offset2)*ncols) + (c+offset1);
                     if( offset[4]<0 || offset[4]>=total_pixels )
                        offset[4]= -1;

                     offset[5]= ((r-offset2)*ncols) + (c-offset1);
                     if( offset[5]<0 || offset[5]>=total_pixels )
                        offset[5]= -1;

                     offset[6]= -1;
                     offset[7]= -1;
                  }


                  for( i=0; i<8; i++ )
                  {
                     if( offset[i]!=-1 && pixfound<4 )
                     {
                        if( *(inatpfile+offset[i])>0 &&
                            *(mask+offset[i])!=0 )
                        {
                           atpvals[pixfound]= *(inatpfile+offset[i]);
                           dptvals[pixfound]= *(indptfile+offset[i]);
                           filldist[pixfound]= distance;
                           pixfound++;
                        }
                     }
                  }

                  offset1++;
                  offset2= sum-offset1;
               }

               sum++;
               offset1=(int)floor(sum/2.0);
               offset2= sum-offset1;
               distance= sqrt((offset1*offset1)+(offset2*offset2));

            }


            /* Compute distance weighted average */

            dsum= filldist[0]+filldist[1]+filldist[2]+filldist[3];

            dwavg_atp=0.0;
            dwavg_dpt=0.0;
            for( i=0; i<4; i++ )
            {
               weight[i]= (1.0 - (filldist[i]/dsum))/3.0;
               dwavg_atp = dwavg_atp + (weight[i]*atpvals[i]);
               dwavg_dpt = dwavg_dpt + (weight[i]*dptvals[i]);
            }

              *(outatpfile+(r*ncols)+c) = (short signed int)dwavg_atp;
		*(outdptfile+(r*ncols)+c) = (short signed int)dwavg_dpt;
		*(outdisfile+(r*ncols)+c) = 10*(signed short int)(dsum/4);
         }

         /******************************************************************
          Copy unflagged pixels under the mask from input to output
         ******************************************************************/

         else
         {
            *(outatpfile+(r*ncols)+c) = (short signed int)*(inatpfile+(r*ncols)+c);
            *(outdptfile+(r*ncols)+c) = (short signed int)*(indptfile+(r*ncols)+c);
            *(outdisfile+(r*ncols)+c) = 0;
         }

      } /* close "c" loop (increment column counter) */

   } /* close "r" loop (increment row counter) */



   /************************************************************************
    Free memory allocated to inputs
   ************************************************************************/

   free(mask);
   free(inatpfile);
   free(indptfile);

   /************************************************************************
    write output file and free memory
   ************************************************************************/

   write_binary(total_pixels,outatpfn,"sint",(void *)(&outatpfile));
   free(outatpfile);

   write_binary(total_pixels,outdptfn,"sint",(void *)(&outdptfile));
   free(outdptfile);

   write_binary(total_pixels,outdisfn,"sint",(void *)(&outdisfile));
   free(outdisfile);



}

