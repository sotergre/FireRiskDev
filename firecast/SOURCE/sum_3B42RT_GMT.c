#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <math.h>

/***************************************************************************
 sum_3B42RT_GMT.c

 Jen Small
 9/11/2003
 ed Karyn Tabor
 11/02/2007

 Sum rainfall duration three-hourly TRMM 3B42RT precip for a specified compositing period and
 calculates rainfall duration.

 Arguments: year jday1 jday2 swapflag

	year = four digit year for first day of compositing period
	jday1 = Julian day (1-365) for first day of compositing period
        jday2 = Julian day (1-365) for last day of compositing period
        swapflag = 0 if on a big-endian machine (Sun, SGI)
	           1 if on a little-endian machine (PC, DEC)
 Inputs: three-hourly TRMM rainfall files from the first to the last
         day of the compositing period, with filenames

	3B42RT.yyyymmddhh.bin

	yyyy is the four digit year
 	mm is the two digit month (01-12)
	dd is the two digit day in the month (01-31)
	hh is the two digit hour: one of 00,03,06,09,12,15,18,21

 Note: if a three-hourly rainfall file is missing, all pixels for
    that three-hourly period are treated as flagged.

 Output: total TRMM rainfall for the compositing period (mm), with filename

	ppt_3b42rt_yyyyd1-d2_sum.bin

	yyyy is the four digit year for the first day of the composite
        d1 is the first Julian day of the composite (leading zeroes
           are included - three digits)
        d2 is the last Julian day of the composite (leading zeroes are
	   included - three digits)

	In the case of a daily file only (year1=year2, month1=month2 and
	   day1=day2) the filename is

	ppt_3b42rt_yyyyddd_sum.bin

	where yyyy is the four digit year and ddd is the three digit
	   Julian day.

 The output file is a 16 bit signed binary file containing the sum
 of all three-hourly rainfall totals for the compositing period.
 Flagged three-hourly values are filled with time-weighted
 linear interpolation. A pixel is considered to be flagged if the
 3 hourly value is negative. If all eight of the three
 hourly values are negative then the sum is flagged with value -1.
 Units for the output are total mm rainfall for the compsiting period.
 Upper left pixel of file is centered at 0.125E longitude, 49.875N
 latitude. Pixel resolution is 0.25 degrees latitude x 0.25 degrees
 longitude. Output file size is 400 rows (50N - 50S), 1440 columns
 (-180E - 180E).

***************************************************************************/

main( int argc, char **argv )
{

   /* command line arguments */
   unsigned short int year,jday1,jday2,swapflag;

   /* julian day loop counter */
   unsigned short int jday;

   /* Day of month for each Julian day */
   unsigned short int day_of_month[365] =
   {
      1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,
      26,27,28,29,30,31,
      1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,
      26,27,28,
      1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,
      26,27,28,29,30,31,
      1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,
      26,27,28,29,30,
      1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,
      26,27,28,29,30,31,
      1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,
      26,27,28,29,30,
      1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,
      26,27,28,29,30,31,
      1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,
      26,27,28,29,30,31,
      1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,
      26,27,28,29,30,
      1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,
      26,27,28,29,30,31,
      1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,
      26,27,28,29,30,
      1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,
      26,27,28,29,30,31
   };

	/* Day of month for each Julian day in a leap year*/
	unsigned short int leap_day_of_month[366] =
	{
		1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,
		26,27,28,29,30,31,
		1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,
		26,27,28,29,
		1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,
		26,27,28,29,30,31,
		1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,
		26,27,28,29,30,
		1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,
		26,27,28,29,30,31,
		1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,
		26,27,28,29,30,
		1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,
		26,27,28,29,30,31,
		1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,
		26,27,28,29,30,31,
		1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,
		26,27,28,29,30,
		1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,
		26,27,28,29,30,31,
		1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,
		26,27,28,29,30,
		1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,
		26,27,28,29,30,31
	};

   /* month for each Julian day */
   unsigned short int month_of_jday[365] =
   {
      1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
      2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
      3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,
      4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,
      5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,
      6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,
      7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
      8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
      9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,
      10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,
      10,10,10,10,10,10,10,10,10,10,
      11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,
      11,11,11,11,11,11,11,11,11,
      12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,
      12,12,12,12,12,12,12,12,12,12
   };
	/* month for each Julian day in a leap year*/
	unsigned short int leap_month_of_jday[366] =
	{
		1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
		2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
		3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,
		4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,
		5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,
		6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,
		7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
		8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
		9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,
		10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,
		10,10,10,10,10,10,10,10,10,10,
		11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,
		11,11,11,11,11,11,11,11,11,
		12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,
		12,12,12,12,12,12,12,12,12,12
	};

   /* day of month, month for current Julian day */
   unsigned short int dom,month, numdays;

   /* number of bytes to skip (header=2880 bytes;
      60N-50N=40*1440*2=115200 bytes) */
   unsigned int skip_bytes = 118080;

   /* number of pixels (rows*cols=400*1440) in 3B42RT precip layer;
      taking only pixels between 50N and 50S */
   unsigned int npixels = 576000;

   /* pointer to a single three hourly rainfall array */
   signed short int *rain,*rain_ptr;

   /* pointer to rainfall sum */
   signed short int *sum,*sum_ptr;
   signed short int *sumfin,*sumfin_ptr;

   /* filename for a 3 hourly rainfall file */
   char rain_fn[101];

   /* pixel loop counter */
   unsigned int pixel, r, c;

   /* bytes read/written check */
   signed int nbytes;

   /* file descriptor for a single 3 hourly rainfall file */
   signed int rain_fd;

   /* filename for the sum */
   char sum_fn[101];

   /* time periods loop counter */
   unsigned int tp;

   /* pointer to rainfall sum cast to float - all calculations are done
      as floats and then rounded at the end of the compositing period */
   float *float_sum,*float_sum_ptr;

   /* pointer to gap count (for interpolation) */
   unsigned short int *gapcount,*gapcount_ptr;

   /* pointer to previous unflagged value (for interpolation) */
   signed short int *lastval,*lastval_ptr;

   /* counter for interpolation - step through gapcount pixels */
   unsigned int i;

   /* weights on previous, current value in interpolation */
   float wtprev,wtcurr;

   /*** byte swap variables used to swap to little endian ***/

   /* first and second bytes of the 2-byte signed integer */
   char byte1,byte2;

   /* buffer for data in big endian byte order */
   char *big_endian,*big_endian_ptr;

   /* buffer for data swapped to little endian byte order */
   char *little_endian,*little_endian_ptr;

   /* file descriptor for tmpfile to hold swapped (little endian) data */
   signed int tmp_fd;


   /** read command line arguments **/

   year=(unsigned short int)atoi(argv[1]);
   jday1=(unsigned short int)atoi(argv[2]);
   jday2=(unsigned short int)atoi(argv[3]);
   printf("Summing TRMM 3B42RT for year %d, Julian days %d to %d\n",
      year,jday1,jday2);

   swapflag = (unsigned short int)atoi(argv[4]);
   if( swapflag==0 )
   {
      printf("You are working on a big-endian machine (Sun, SGI)\n");
   }
   else if( swapflag==1 )
   {
      printf("You are working on a  little-endian machine (PC, DEC)\n");

      /* Allocate memory for buffers to hold swapped/unswapped data */

      if( allocate_binary(npixels*2,"char",(void *)(&big_endian))==-1 )
      {
         printf("Error: couldn't allocate memory for big endian buffer\n");
         exit(1);
      }

      if( allocate_binary(npixels*2,"char",(void *)(&little_endian))==-1 )
      {
         printf("Error: couldn't allocate memory for little endian buffer\n");
         exit(1);
      }
   }
   else
   {
      printf("Error: swapflag must be either 0 for big-endian (Sun, SGI)\n");
      printf("or 1 for little-endian (PC, DEC)\n");
      printf("Exiting...\n");
      exit(1);
   }

if( jday1==jday2 )
 	{
      sprintf(sum_fn,"ppt_3b42rt_%d%.3d_sum.bin",year,jday1);
  	}
   else
	{
       sprintf(sum_fn,"ppt_3b42rt_%d%.3d-%.3d_sum.bin",year,jday1,jday2);
       }

	printf("Output file will be %s\n", sum_fn);

   /* allocate memory for the sum, gapcount, and last unflagged
      value arrays and for a single three hourly rainfall array */

   if( allocate_binary(npixels,"float",(void *)(&float_sum))==-1 )
   {
      printf("Error: couldn't allocate memory for sum\n");
      exit(1);
   }

   if( allocate_binary(npixels,"sint",(void *)(&gapcount))==-1 )
   {
      printf("Error: couldn't allocate memory for gap count\n");
      exit(1);
   }

   if( allocate_binary(npixels,"sint",(void *)(&lastval))==-1 )
   {
      printf("Error: couldn't allocate memory for last unflagged value array\n");
      exit(1);
   }


   if( allocate_binary(npixels,"sint",(void *)(&rain))==-1 )
   {
      printf("Error: couldn't allocate memory for rainfall file\n");
      exit(1);
   }


   /* initialize last unflagged value array to flag value
      (note- gapcount and sum are initialized to zero
       by calloc)
   */

   lastval_ptr=lastval;

   for( pixel=0; pixel<npixels; pixel++ )
   {
      *lastval_ptr = -1;

      lastval_ptr++;
   }


   /* Julian days loop */

   for( jday=jday1; jday<=jday2; jday++ )
   {

      /* look up month and day for Julian day; add leading zeroes
         for TRMM filename */

	/*testing for leap year*/
	if (year==2000 || year == 2004 || year == 2004 || year == 2008 || year == 2012 || year == 2016 || year == 2020)
	{
		month = leap_month_of_jday[jday-1];
		dom = leap_day_of_month[jday-1];
		numdays=366;
	}
	else
	{
		month = month_of_jday[jday-1];
		dom = day_of_month[jday-1];
		numdays=365;
	}
  
      month = month_of_jday[jday-1];   
      dom = day_of_month[jday-1];
   
      /* Time periods loop - read in data for each time period */

      for( tp=0; tp<=21; tp+=3 )
      {

         /* open the file for this three-hourly time period */

         sprintf(rain_fn,"3B42RT.%d%.2d%.2d%.2d.bin",year,month,
            dom,tp);
         rain_fd=open(rain_fn,O_RDONLY);
         if( rain_fd==-1 )
         {

            /* if the file wasn't opened write flags (=-1) to all pixels */

            printf("Warning: couldn't open %s for read\n",rain_fn);
            printf("All pixels for this time step will be flagged\n");

            for( pixel=0; pixel<npixels; pixel++ )
            {
               *(rain+pixel) = -1;
            }
         }

         else
         {

            /* position file pointer to start at skip_bytes bytes */

            nbytes=lseek(rain_fd,skip_bytes,SEEK_SET);
            if( nbytes!=skip_bytes )
            {
               printf("Error: couldn't set file pointer to 50N for %s\n",
                  rain_fn);
            }



            /* if the swapflag is set to 1 then swap to little-endian byte
               order upon read, otherwise read directly */

            if( swapflag==1 )
            {

               /* call byteswap function */

               if( byteswap_3B42RT(&rain_fd,big_endian,little_endian,
                   &npixels)==-1 )
               {
                  printf("Error: couldn't byteswap %s\n",rain_fn);
                  exit(1);
               }


               /* read swapped data to 16S buffer */

               if( read_binary(npixels,"tmpfile","sint",(void *)(&rain))==-1 )
               {
                  printf("Error: read data from swapped file\n");
                  exit(1);
               }
            }
            else
            {
               /* read directly to 16S buffer */

               nbytes=read(rain_fd,(signed short int *)rain,
                  npixels*sizeof(signed short int));
               if( nbytes!=npixels*sizeof(signed short int) )
               {
                  printf("Error: only %d of %d bytes read from %s\n",
                    nbytes,npixels*sizeof(signed short int),rain_fn);
                  exit(1);
               }
            }


            /* close this three-hourly file */

            close(rain_fd);
         }


         /* pixels loop */

         lastval_ptr=lastval;
         gapcount_ptr=gapcount;
         float_sum_ptr=float_sum;
         rain_ptr=rain;

         for( pixel=0; pixel<npixels; pixel++ )
         {

            if( *rain_ptr>=0 )
            {
               /* this pixel is not flagged; add it's value to the sum */

               *float_sum_ptr += (float)(*rain_ptr);
           

               /* if there was a gap fill it */

               if( *gapcount_ptr>0 )
               {

                  if( *lastval_ptr==-1 )
                  {
                     /* If there is no previous unflagged value (ie the
                        first three-hourly period was flagged) then
                        add the current value to the sum gapcount times */

                     for( i=1; i<=(*gapcount_ptr); i++ )
                     {
                        *float_sum_ptr += (float)(*rain_ptr);
			            
                     }
                  }
                  else
                  {
                     /* Time-weighted linear interpolation over the
                        gap using the current value and the
                        previous unflagged value */

                     for( i=1; i<=(*gapcount_ptr); i++ )
                     {
                        wtprev = (float)(*gapcount_ptr-i+1)/
                                 (float)(*gapcount_ptr+1);

                        wtcurr = (float)(i)/(float)(*gapcount_ptr+1);

                        *float_sum_ptr += (float)(*lastval_ptr)*wtprev +
                                          (float)(*rain_ptr)*wtcurr;
					
                     }

                  }

               }


               /* set the previous unflagged value to the current
                  rain value; reset the gapcount to zero */

               *lastval_ptr = *rain_ptr;
               *gapcount_ptr = 0;

            }
            else if( jday==jday2 && tp==21 )
            {
               /* if the last three-hourly time step of the
                  compositing period is flagged... */

               if( *lastval_ptr==-1 )
               {
                  /* all time periods were flagged - flag the sum */

                  *float_sum_ptr = -1.0;
               }
               else
               {
                  /* add the previous unflagged value to the sum
                     gapcount times */

                  for( i=1; i<=(*gapcount_ptr); i++ )
                  {
                     *float_sum_ptr += (float)(*lastval_ptr);
                  }
               }
            }
            else
            {
               /* flagged and not last time period -
                  increment the gap counter */

               *gapcount_ptr += 1;
            }


            /* increment all pointers */

            gapcount_ptr++;
            rain_ptr++;
            float_sum_ptr++;
            lastval_ptr++;

         } /* end pixel loop */


         /* close the file for this three-hourly time period */

         close(rain_fd);


      } /* end three-hourly time period loop */


   } /* end Julian day loop */


   /* free memory for input and buffers */

   free(rain);
   free(gapcount);
   free(lastval);

   if( swapflag==1 )
   {
      free(big_endian);
      free(little_endian);
   }


   /* allocate memory to hold sum rounded to nearest integer */

   if( allocate_binary(npixels,"sint",(void *)(&sum))==-1 )
   {
      printf("Error: couldn't allocate memory for rounded sum\n");
      exit(1);
   }


   /* convert units and round sum */

   sum_ptr=sum;
   float_sum_ptr=float_sum;

   for( pixel=0; pixel<npixels; pixel++ )
   {

      /* Divide by 100 to convert 0.01 mm/hr to mm/hr;
         multiply by 3 to convert three-hourly mm/hr to total mm */

      *float_sum_ptr = (3.0*(*float_sum_ptr))/100.0;


      /* round to nearest integer */

      if( ((*float_sum_ptr) - (int)(*float_sum_ptr))>=0.5 )
      {
         *sum_ptr = (signed short int)(*float_sum_ptr)+1;
      }
      else
      {
         *sum_ptr = (signed short int)(*float_sum_ptr);
      }

      sum_ptr++;
      float_sum_ptr++;

   }




/*allocate memory for final files which are -180 to 180*/

  if( allocate_binary(npixels,"sint",(void *)(&sumfin))==-1 )
   {
      printf("Error: couldn't allocate memory for rainfall file\n");
      exit(1);
   }

   for(r=0; r<400; r++)
   {
	for(c= 0; c<720; c++)
      	{
     	*(sumfin+(r*1440)+c)= (signed short int)(*(sum+(r*1440)+c+720));
      	}
	for(c=720;c<1440;c++)
    	{
	*(sumfin+(r*1440)+c)= (signed short int)(*(sum+(r*1440)+c-720));
	}

   }


/* free memory for sum */

   free(float_sum);
   free(sum);

   /* write total rainfall for compositing period */

   if( write_binary(npixels,sum_fn,"sint",(void *)(&sumfin))==-1 )
   {
      printf("Error: couldn't write sum file %s\n",sum_fn);
      exit(1);
   }

   free(sumfin);

   return 0;

}
