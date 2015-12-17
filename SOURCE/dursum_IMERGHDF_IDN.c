#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <math.h>

/***************************************************************************
dursum_IMERGHDF_IDN.c

Karyn Tabor
7/24/2015

Calulates rainfall sum and duration for 30-minute IMERG real time precip for a specified compositing period.
This adjusts for rainfall duration.

 The input files have already been subsetted and reporjected with GDAL (or GRADS?).
 Arguments: year startjday endjday timeperiod nrow ncols outputdirectory

	year = four digit year of compositing period
	jday = Julian day (1-365) for first day of compositing period
    	timeperiod = options for splitting day into 24 hr, first 15 hours and last 9 hours.
    		"day" for day sum
    		"p1" for first 15 hrs of that day
    		"p2" for the remaining 9 hours of that day
	outputdirectory = string for output directory

Input data type is 32-bit float
units are mm/hour

*******************************************
 Inputs: 30-minute IMERG rainfall files from the first to the last
         day of the compositing period, with filenames

    IMERG30m_early.IDN.YYYYMMDD.mmmm.bin
	YYYY is the four digit digit year
 	MM is the two digit month (01-12)
	DD is the two digit day in the month (01-31)
	mmmm is the minute at the start of the 30-minute period: one of 0000, 0030, 0060, 0090, 0120, 0150, ... 1410

 Note: if a three-hourly rainfall file is missing, all pixels for
    that three-hourly period are treated as flagged.

 Output: total IMERG rainfall for the compositing period (mm), with filename

	ppt_IMERG30m_early_yyyyd1-d2_sum_IDN.bin

	And rainfall duration for the compositing period (hours), with filename

	ppt_IMERG30m_early_yyyyd1-d2_dur_IDN.bin


	yyyy is the four digit year for the first day of the composite
        d1 is the first Julian day of the composite (leading zeroes
           are included - three digits)
        d2 is the last Julian day of the composite (leading zeroes are
	   included - three digits)

	In the case of a daily file only the filename is

	for 24 hour sums:
	ppt_IMERG30m_early_yyyyddd_sum_IDN.bin
	ppt_IMERG30m_early_yyyyddd_dur_IDN.bin

	for time period 1: noon previous day to 3 am current day

	ppt_IMERG30m_early_yyyyddd_sum_IDN_tp1.bin
	ppt_IMERG30m_early_yyyyddd_dur_IDN_tp1.bin

	for time period 2: 3 am current day to noon current day

	ppt_IMERG30m_yyyyddd_sum_IDN_tp2.bin
	ppt_IMERG30m_yyyyddd_dur_IDN_tp2.bin

	where yyyy is the four digit year and ddd is the three digit
	   Julian day.

 The output file is a 16 bit signed binary file containing the sum
 of all three-hourly rainfall totals for the compositing period and
 an 16 bit signed binary file of rainfall duration.

 Flagged three-hourly values are filled with time-weighted
 linear interpolation. A pixel is considered to be flagged if the
 3 hourly value is negative. If all eight of the three
 hourly values are negative then the sum is flagged with value -1.
 Units for the output are total mm rainfall for the compsiting period.

***************************************************************************/

main( int argc, char **argv )
{

   /* command line arguments */
   /*year startjday endjday timeperiod nrow ncols outputdirectory*/
   int year,jday1,jday2, nrows, ncols;
   char timeperiod[5];
   char outputdir[100];

     /* julian day loop counter */
   unsigned short int jday, kday,year_2d;

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

	int timeperiodarray[48][2]={{0,0},{30,0},{60,0},{90,0},{120,0},{150,0},{180,0},{210,0},{240,0},{270,0},{300,0},{330,0},{360,0},{390,0},{420,0},{450,0},{480,0},{510,0},{540,0},{570,0},{600,0},{630,0},{660,0},{690,0},{720,0},{750,0},{780,0},{810,0},{840,0},{870,0},{900,0},{930,0},{960,0},{990,0},{1020,0},{1050,0},{1080,0},{1110,0},{1140,0},{1170,0},{1200,0},{1230,0},{1260,0},{1290,0},{1320,0},{1350,0},{1380,0},{1410,0}};

   /* day of month, month for current Julian day */
   unsigned short int dom,month,numdays;


    int npixels;

   /* pointer to a single three hourly rainfall array */
   float *rainin,*rainin_ptr;

   float *rain, *rain_ptr;


   /* pointer to rainfall duration */
   signed short int *dur, *dur_ptr;

   /* pointer to rainfall sum */
   signed short int *sum,*sum_ptr;


   /* filename for a 30-min rainfall file */
   char rain_fn[101];

   /* pixel loop counter */
   unsigned int pixel, r, c, step, timestep, numsteps;

   /* bytes read/written check */
   signed int nbytes;

   /* file descriptor for a single 30-min rainfall file */
   signed int rain_fd;

   /* filename for the sum */
   char sum_fn[101];

	/* filename for the duration */
   char dur_fn[101];

   /* time periods loop counter */
   unsigned int tp;

   /* pointer to rainfall sum cast to float - all calculations are done
      as floats and then rounded at the end of the compositing period */
   float *float_sum,*float_sum_ptr;

   /* pointer to gap count (for interpolation) */
   signed short int *gapcount,*gapcount_ptr;

   /* pointer to previous unflagged value (for interpolation) */
   float *lastval,*lastval_ptr;

   /* counter for interpolation - step through gapcount pixels */
   unsigned int i;

   /* weights on previous, current value in interpolation */
   float wtprev,wtcurr;

	int flag = 0;


   /** read command line arguments **/
 /*Arguments: year startjday endjday timeperiod nrow ncols outputdirectory*/
   year=(unsigned short int)atoi(argv[1]);
   jday1=(unsigned short int)atoi(argv[2]);
   jday2=(unsigned short int)atoi(argv[3]);
   sprintf(timeperiod, "%s", (argv[4]));
   nrows=atoi(argv[5]);
   ncols=atoi(argv[6]);
   printf("Summing IMERG for year %d, Julian days %d to %d\n",year,jday1,jday2);
   sprintf(outputdir, "%s", (argv[7]));

  /*calculate the total number of pixels for the input data based on the number of rows and columnns*/
  npixels=nrows*ncols;
printf("npixels are %d\n",npixels);

 if( jday1==jday2 )
 	{
		if (strcmp(timeperiod,"day")==0)
		{
  	    	sprintf(sum_fn,"%sppt_IMERG30m_early_%d%.3d_sum_IDN.bin",outputdir,year,jday1);
  	    	sprintf(dur_fn,"%sppt_IMERG30m_early_%d%.3d_dur_IDN.bin",outputdir,year,jday1);		}
		else
		{
		sprintf(sum_fn,"%sppt_IMERG30m_early_%d%.3d_sum_IDN_%s.bin",outputdir,year,jday1,timeperiod);
  	    sprintf(dur_fn,"%sppt_IMERG30m_early_%d%.3d_dur_IDN_%s.bin",outputdir,year,jday1,timeperiod);
		}
  	}
 else
	{
		if (strcmp(timeperiod,"day")==0)
		{
		sprintf(sum_fn,"%sppt_IMERG30m_early_%d%.3d-%.3d_sum_IDN.bin",outputdir,year,jday1,jday2);
		sprintf(dur_fn,"%sppt_IMERG30m_early_%d%.3d-%.3d_dur_IDN.bin",outputdir,year,jday1,jday2);		}
		else
		{
		sprintf(sum_fn,"%sppt_3b42_%d%.3d-%.3d_sum_IDN_%s.bin",outputdir,year,jday1,jday2, timeperiod);
		sprintf(dur_fn,"%sppt_IMERG30m_early_%d%.3d-%.3d_dur_IDN_%s.bin",outputdir,year,jday1, jday2,timeperiod);
		}
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


   if( allocate_binary(npixels,"float",(void *)(&rainin))==-1 )
   {
      printf("Error: couldn't allocate memory for rainfall file\n");
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

   if( allocate_binary(npixels,"sint",(void *)(&dur))==-1 )
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
      *lastval_ptr = -1.0;

      lastval_ptr++;
   }



/*determine timeperiod*/
if (strcmp(timeperiod,"day")==0)
{
	/*24 of the previous day*/;
	timestep=0;
	numsteps=48;
}
else if (strcmp(timeperiod,"p1")==0)
{
	timestep=0;
	numsteps=30;
}

else
{
	/*timeperiod is p2*/
	timestep=30;
	numsteps=18;
}


for (jday=jday1; jday <= jday2; jday++)
{
	for(step=0; step<numsteps; step++)
	{
	year=atoi(argv[1]);
    /*kday=jday+timeperiodarray[timestep][1];*/
    kday=jday;
	/*printf("timestep is %d and hour is %d\t",timestep,timeperiodarray[timestep][0]);
	printf("kday is %d\n",kday);*/

	/*testing for leap year*/
	if (year==2000 || year == 2004 || year == 2004 || year == 2008 || year == 2012 || year == 2016 || year == 2020 || year == 2024)
	{
	numdays=366;


	month = leap_month_of_jday[kday-1];
	dom = leap_day_of_month[kday-1];

	}

	else
	{
	numdays=365;
	month = month_of_jday[kday-1];
	dom = day_of_month[kday-1];

	}
   /* open the file for this half-hourly time period */
    /*year_2d=year%100;*/
    sprintf(rain_fn,"IMERG30m_early.IDN.%d%.2d%.2d.%.4d.bin",year,month,dom,timeperiodarray[timestep][0]);
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
			 /*set all values in the array to 0*/
			 for( pixel=0; pixel<npixels; pixel++ )
			  {
			   *(rain+pixel) = 0;
              }
		/* read directly to float buffer */

               nbytes=read(rain_fd,(float *)rainin,
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
         rainin_ptr=rainin;
         dur_ptr=dur;


         for( pixel=0; pixel<npixels; pixel++ )
         {

            if( *rain_ptr>=0 )
            {
               /* this pixel is not flagged; add it's value to the sum while converting units from mm/hour*10 to mm*/
	           *rain_ptr=*rainin_ptr/2.0;
               *float_sum_ptr +=*rain_ptr;
               if(*rain_ptr>0)
               {
		/*if there was any rainfall assume that it rained for the full 30 minutes. duration is hours*10)*/
               *dur_ptr=(signed short int)(*dur_ptr+5);
		   		}


               /* if there was a gap fill it */

               if( *gapcount_ptr>0 )
               {

                  if( *lastval_ptr==-1.0 )
                  {
                     /* If there is no previous unflagged value (ie the
                        first three-hourly period was flagged) then
                        add the current value to the sum gapcount times */

                     for( i=1; i<=(*gapcount_ptr); i++ )
                     {
                        *float_sum_ptr +=(float)*rain_ptr;
			             if(*rain_ptr>0)
			               {
	                        *dur_ptr=(signed short int)(*dur_ptr+5);
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
					    if(*rain_ptr>0)
               				{
                            *dur_ptr=(signed short int)(*dur_ptr+5*wtcurr);
							}
							if(*lastval_ptr>0)
							{
							*dur_ptr=(signed short int)(*dur_ptr+5*wtprev);
							}
                     }

                  }

               }


               /* set the previous unflagged value to the current
                  rain value; reset the gapcount to zero */

               *lastval_ptr = *rain_ptr;
               *gapcount_ptr = 0;

            }
            else if( jday==jday2 && tp==47 )
            {
               /* if the last three-hourly time step of the
                  compositing period is flagged... */
               if( *lastval_ptr==-1.0 )
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
                      *dur_ptr=(signed short int)(*dur_ptr+5);
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
            rainin_ptr++;
            float_sum_ptr++;
            lastval_ptr++;
            dur_ptr++;


         } /* end pixel loop */

       /*increment loops*/
	 timestep++;
	 	if(timestep>47)
	 	{
	 	timestep=0;
	 	}
	} /*end numsteps loop*/

} /*end for jday loop*/

   /* free memory for input and buffers */
   free(rain);
   free(rainin);
   free(gapcount);
   free(lastval);

   /* convert units and round sum */

   sum_ptr=sum;
   float_sum_ptr=float_sum;

   for( pixel=0; pixel<npixels; pixel++ )
   {


     /* round to nearest integer */

      if( ((*float_sum_ptr) - (short signed int)(*float_sum_ptr))>=0.5 )
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


   free(float_sum);

   /* write total rainfall for compositing period */

   if( write_binary(nrows*ncols,sum_fn,"sint",(void *)(&sum))==-1 )
   {
      printf("Error: couldn't write sum file %s\n",sum_fn);
      exit(1);
   }

   if( write_binary(nrows*ncols,dur_fn,"sint",(void *)(&dur))==-1 )
   {
      printf("Error: couldn't write dur file %s\n",dur_fn);
      exit(1);
   }


   free(sum);
   free(dur);

   return 0;

}
