#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <math.h>

/***************************************************************************
 dursum_3B42PP_GLOBAL_V7.c

Karyn Tabor
11/2013

 calulates rainfall Sum and duration for three-hourly TRMM 3B42 post processed precip for a specified compositing period and
 GMT.  This program also subsets and reproject rainfall files.

 Arguments: year startjday endjday swapflag

	year = four digit year of compositing period
	startjday = Julian day (1-365) for first day of compositing period
	endjday = Julian day (1-365) for last day of compositing period
	outdir = string for output directory

3B42 post processed data must first be converted from HDF to binary with select_vNdsd

    !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	*.precipitation files generated from TRMM 3b42 HDF files using Read_HDF (executble select_vNdsd)
	Batch all files in the directory with the following shell script:

	#! c:/cygwin/bin/bash

	ls *.HDF > hdflist.txt

	for filename in `cat hdflist.txt`
	do
	echo "1			" > batch.ini
	echo "${filename}	" >> batch.ini
	echo "1			" >> batch.ini
	echo "1			" >> batch.ini
	echo "0			" >> batch.ini
	echo "0			" >> batch.ini
	echo "0			" >> batch.ini

	./select_vNsds.exe -b batch.ini

	done



 Inputs: three-hourly TRMM rainfall files from the first to the last
         day of the compositing period, with filenames

    3B42.yyyymmdd.hh.7.precipitation or 3B42.yymmdd.h.6A.precipitation
	yyyy is the four digit digit year
 	mm is the two digit month (01-12)
	dd is the two digit day in the month (01-31)
	hh is the hour: one of 00,03,06,09,12,15,18,21

 Note: if a three-hourly rainfall file is missing, all pixels for
    that three-hourly period are treated as flagged.

 Output: total TRMM rainfall for the compositing period (mm), with filename

	ppt_3b42pp_yyyyd1-d2_GLOBAL.bin

	And rainfall duration for the compositing period (hours), with filename

	ppt_3b42pp_yyyyd1-d2_GLOBAL.bin


	yyyy is the four digit year for the first day of the composite
        d1 is the first Julian day of the composite (leading zeroes
           are included - three digits)
        d2 is the last Julian day of the composite (leading zeroes are
	   included - three digits)

	In the case of a daily file only the filename is

	for 24 hour sums:
	ppt_3b42pp_yyyyddd_sum_GLOBAL.bin
	ppt_3b42pp_yyyyddd_dur_GLOBAL.bin


	where yyyy is the four digit year and ddd is the three digit
	   Julian day.

 The output file is a 16 bit signed binary file containing the sum
 of all three-hourly rainfall totals for the compositing period and
 a 16 bit signed binary file of rainfall duration.
 Flagged three-hourly values are filled with time-weighted
 linear interpolation. A pixel is considered to be flagged if the
 3 hourly value is negative. If all eight of the three
 hourly values are negative then the sum is flagged with value -1.
 Units for the output are total mm rainfall for the compsiting period.


***************************************************************************/

main( int argc, char **argv )
{

   /* command line arguments */
   unsigned short int year,jday1,jday2;
   char outputdir[100];

     /* julian day loop counter */
   unsigned short int jday, kday,year_2d;
char year_2s[3];

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
   unsigned short int dom,month,numdays;

   /* number of pixels (rows*cols=400*1440) in 3B42 precip layer;
      taking only pixels between 50N and 50S */
   unsigned int npixels = 576000;
   int ncols=400;
   int nrows=1440;

   /* pointer to a single three hourly rainfall array */
   float *rain,*rain_ptr;


   /* pointer to rainfall sum */
   signed short int *sum,*sum_ptr;
   signed short int *sumfin,*sumfin_ptr;

/* code for TRMM 3B42 version.  Either "7" or "7A"*/
   char version[3];

   /* filename for a 3 hourly rainfall file */
   char rain_fn[101];

   /* pixel loop counter */
   unsigned int pixel, r, c, step, timestep, numsteps;

   /* bytes read/written check */
   signed int nbytes;

   /* file descriptor for a single 3 hourly rainfall file */
   signed int rain_fd;

   /* filename for the sum */
   char sum_fn[101];

	/* filename for the duration */
   char dur_fn[101];

   /* time periods loop counter */
   unsigned int timeperiod;

   /* pointer to rainfall sum cast to float - all calculations are done
      as floats and then rounded at the end of the compositing period */
   float *float_sum,*float_sum_ptr;

   /* pointer to gap count (for interpolation) */
   unsigned short int *gapcount,*gapcount_ptr;

   /* pointer to previous unflagged value (for interpolation) */
   float *lastval,*lastval_ptr;

   /* counter for interpolation - step through gapcount pixels */
   unsigned int i;

   /* weights on previous, current value in interpolation */
   float wtprev,wtcurr;

	int flag = 0;

    /*******************************************************************************/


   /** read command line arguments **/

   year=(unsigned short int)atoi(argv[1]);
   jday1=(unsigned short int)atoi(argv[2]);
   jday2=(unsigned short int)atoi(argv[3]);
   printf("Summing TRMM 3B42 for year %d, Julian days %d to %d\n",year,jday1,jday2);
   sprintf(outputdir, "%s", (argv[4]));

if(year<2000 || year>2010){
	sprintf(version,"7");}
	else{
	sprintf(version,"7A");}

 if( jday1==jday2 )
 	{
  	    sprintf(sum_fn,"%sppt_3b42pp_%d%.3d_sum_GLOBAL.bin",outputdir,year,jday1);
  	}
 else
	{
		sprintf(sum_fn,"%sppt_3b42pp_%d%.3d-%.3d_sum_GLOBAL.bin",outputdir,year,jday1,jday2);
    }



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

   if( allocate_binary(npixels,"float",(void *)(&lastval))==-1 )
   {
      printf("Error: couldn't allocate memory for last unflagged value array\n");
      exit(1);
   }


   if( allocate_binary(npixels,"float",(void *)(&rain))==-1 )
   {
      printf("Error: couldn't allocate memory for rainfall file\n");
      exit(1);
   }

  if( allocate_binary(npixels,"sint",(void *)(&sum))==-1 )
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

year=atoi(argv[1]);
year_2d=year%100;
for (jday=jday1; jday <= jday2; jday++)
{
	/*testing for leap year*/
	if (year==2000 || year == 2004 || year == 2004 || year == 2008 || year == 2012 || year == 2016 || year == 2020)
	{
		numdays=366;
		month = leap_month_of_jday[jday-1];
		dom = leap_day_of_month[jday-1];
	}
	else
	{
		numdays=365;
		month = month_of_jday[jday-1];
		dom = day_of_month[jday-1];
	}

	/*check version for last 3 months of 2010*/
	if(year == 2010 && jday>273){
	sprintf(version,"7");}
	
	/*start looping through timeperiods*/
	for(timeperiod=0; timeperiod<=21; timeperiod+=3)
	{
		/* open the file for this three-hourly time period */

   		 sprintf(rain_fn,"3B42.%d%.2d%.2d.%.2d.%s.precipitation",year,month,dom,timeperiod,version);
   		 printf("Rain file name %s \n",rain_fn);
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
		/* read directly to 16S buffer */
   	         nbytes=read(rain_fd,(float *)rain,
                npixels*sizeof(float));
   	            if( nbytes!=npixels*sizeof(float) )
   	            {
   	               printf("Error: only %d of %d bytes read from %s\n",
                    nbytes,npixels*sizeof(float),rain_fn);
   	               exit(1);
   	            }
          }


          /* close this three-hourly file */
          close(rain_fd);
         /**/

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
	               *float_sum_ptr +=*rain_ptr;
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
               				        *float_sum_ptr +=*rain_ptr;
									}
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

               		         *float_sum_ptr += (*lastval_ptr)*wtprev +
                            (*rain_ptr)*wtcurr;
               		      }

               	   }


               /* set the previous unflagged value to the current
                  rain value; reset the gapcount to zero */

               *lastval_ptr = *rain_ptr;
               *gapcount_ptr = 0;
			}/*end if  *rain_ptr>=0*/
           	 else if( jday==jday2 && timeperiod==21 )
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
                     *float_sum_ptr += *lastval_ptr;
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


         } /*end pixel loop*/

 	} /*end for timeperiod loop*/

} /*end for jday loop*/

   /* free memory for input and buffers */

   free(rain);
   free(gapcount);
   free(lastval);


   /* convert units and round sum */

   sum_ptr=sum;
   float_sum_ptr=float_sum;

   for( pixel=0; pixel<npixels; pixel++ )
   {

      /* multiply by 3 to convert three-hourly mm/hr to total mm */
      /*fixed units. Input is actually mm/day*/

	if( *float_sum_ptr==-1 )
	      {
		*sum_ptr =-1;
		}
	else
		{
      		*float_sum_ptr = 3.0*(*float_sum_ptr);

	      /* round to nearest integer */

	      if( ((*float_sum_ptr) - (int)(*float_sum_ptr))>=0.5 )
		      {
		         *sum_ptr = (signed short int)(*float_sum_ptr)+1;
      			}
      		else
      			{
         		*sum_ptr = (signed short int)(*float_sum_ptr);
      			}
		}/*else *float_sum_ptr) !=-1*/
      sum_ptr++;
      float_sum_ptr++;

   }

/*allocate memory for final files which are -180 to 180*/

  if( allocate_binary(npixels,"sint",(void *)(&sumfin))==-1 )
   {
      printf("Error: couldn't allocate memory for rainfall file\n");
      exit(1);
   }

 /*fixed this. Original rainfall data has 400 cols and 1440 rows*/
   for(r=0; r<nrows; r++)
   {
	for(c= 0; c<ncols; c++)
      	{
			*(sumfin+(ncols-(c+1))*nrows+r)=*(sum+(r*ncols)+c);
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
