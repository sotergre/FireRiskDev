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

Calculates an monthly average of precipitation over several years

 Arguments: startyear endyear month rootdir

	startyear = is the four digits of the first year
	endyear = is the four digits of the last year
	month = three letter date (jan, feb, mar, jun, jul, aug, sep, oct, nov, dec
	rootdir = string for output directory


 Inputs: monthly sums TRMM rainfall files with filenames

       ppt_3b42pp_yyyymmm_sum_GLOBAL.bin
	yyyy is the four digits of the first year
	mmm is the three letter date (jan, feb, mar, jun, jul, aug, sep, oct, nov, dec

 Note: if a three-hourly rainfall file is missing, all pixels for
    that three-hourly period are treated as flagged.

 Output: average TRMM rainfall for the timeperiod with file name...

	       ppt_3b42pp_yyyyYYYmmm_ave_GLOBAL.bin



	yyyy is the four digits of the first year
	YYYY is the four digits of the final year
	mmm is the three letter date (jan, feb, mar, jun, jul, aug, sep, oct, nov, dec


 The output file is a 16 bit signed binary file containing the average
 of all monthly rainfall totals for the timeperiod period.


***************************************************************************/

main( int argc, char **argv )
{

/* command line arguments */
unsigned short int startyear,endyear;
char month[5],rootdir[100];

/* number of pixels (rows*cols=400*1440) in 3B42 precip layer taking only pixels between 50N and 50S */
unsigned int npixels = 576000;
int ncols=400;
int nrows=1440;

/* pointer to a single month sum rainfall array */
signed short int *rain,*rain_ptr;
float *sum,*sum_ptr;
signed short int *ave,*ave_ptr;


/* filename for input and output files*/
char rain_fn[101], ave_fn[101];

/* pixel loop counter */
unsigned int i;

/* time periods loop counter */
unsigned int year, numyears;

 

    /*******************************************************************************/


/** read command line arguments **/
startyear=(unsigned short int)atoi(argv[1]);
endyear=(unsigned short int)atoi(argv[2]);
sprintf(month, "%s", (argv[3]));
printf("Summing TRMM 3B42 for year %d to %d, month %s\n",startyear,endyear,month);
sprintf(rootdir, "%s", (argv[4]));

sprintf(ave_fn,"%sAVE/ppt_3b42pp_%d-%d_%s_ave_GLOBAL.bin",rootdir,startyear,endyear,month);
numyears=endyear-startyear+1;
printf("num years is %d\n",numyears);
   /* allocate memory */


   if( allocate_binary(npixels,"sint",(void *)(&rain))==-1 )
   {
      printf("Error: couldn't allocate memory for rainfall file\n");
      exit(1);
   }

  if( allocate_binary(npixels,"float",(void *)(&sum))==-1 )
   {
      printf("Error: couldn't allocate memory for sum file\n");
      exit(1);
   }

  if( allocate_binary(npixels,"sint",(void *)(&ave))==-1 )
   {
      printf("Error: couldn't allocate memory for ave file\n");
      exit(1);
   }

/*start looping through year*/
	for(year=startyear; year<=endyear; year++)
	{
		/* open the file for this month */
		sprintf(rain_fn, "%s%d/MONTH/ppt_3b42pp_%d%s_sum_GLOBAL.bin",rootdir,year,year,month); 

	 	if(read_binary(npixels, rain_fn, "sint", (void *)(&rain)) ==-1)
 		{	
	      printf("monthly rain does not exit\n");
	      exit (0);
		}
		
		rain_ptr = rain;
		sum_ptr = sum;
			for(i=0; i<npixels;i++)
		   	 {
			*sum_ptr = *sum_ptr + *rain_ptr;
			rain_ptr++;
			sum_ptr++;
			} /*end pixel loop*/

		/*reset rain array to 0's_*/
		 for( i=0; i<npixels; i++ ){
	           *(rain+i) = 0;}

	} /*end year loop*/

   /* free memory for input and buffers */

   free(rain);


/*calculate average*/
	sum_ptr = sum;
	ave_ptr = ave;
		for(i=0; i<npixels;i++)
	   	 {
		*ave_ptr = (int)(*sum_ptr/numyears);
		ave_ptr++;
		sum_ptr++;
		} /*end pixel loop*/

free(sum);
 

   /* write total rainfall for compositing period */

  if( write_binary(npixels,ave_fn,"sint",(void *)(&ave))==-1 )
   {
      printf("Error: couldn't write sum file %s\n",ave_fn);
      exit(1);
   }


   free(ave);

   return 0;

}
