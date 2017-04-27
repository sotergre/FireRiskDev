/******************************************************************************************
rhumid_at_dpt.c

Calculates relative humidity from dew point temperature and temperature

last update 09/20/2005

Input: binary signed 16 bit dewpoint (K)*10 and binary signed 16 bit LST temperature (K)*10

Output: binary signed 16 bit relative humidity 1000%

Command line arguments

<interpolated dewpoint file> <interpolated temperature infile> <output relative humidity file> <image size in total pixels>


******************************************************************************************/


#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <math.h>
#include <malloc.h>
#include <sys/uio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAX_NUM_STRINGS 500


main(int argc, char *argv[])
{
FILE *infile;
char DPT_filename[101], LST_filename[101], RH_filename[101];
int i, total_pixels;
signed short int *rhumid, *temp, *dpt;
signed short int *rhumid_ptr, *temp_ptr,*dpt_ptr;
float rh_dpt, rh_temp;

float relative_humidity(signed short int, signed short int);


 /** Check args, if incorrect specify usage **/

   if( argc!=5 )
   {
      printf("Usage:\n\n");
      printf("<interpolated dewpoint file> <interpolated temperature infile>/  \n");
      printf("<output relative humidity file> <image size in total pixels> \n\n");
      printf("See source code file for command line argument values\n");
      return 1;
   }


total_pixels=atoi(argv[4]);
printf("total number of pixels is %d\n", total_pixels);


/***********************Reading in data**************************/
/*rhumid is the 1000% realtive humidity*/
/*temp is the air temperature in degrees Kelvin*10*/
/*dpt is dew point is in degrees Kelvin*10*/

/*************************************************************
Memory Allocation
*************************************************************/
allocate_binary(total_pixels, "sint", (signed short int *)(&temp));
allocate_binary(total_pixels, "sint", (signed short int *)(&dpt));
allocate_binary(total_pixels, "sint", (signed short int *)(&rhumid));

/***********************************************************
Read input files
************************************************************/

sprintf(DPT_filename, argv[1]);
if(read_binary(total_pixels, DPT_filename, "sint", (void *)(&dpt))==-1)
{
	printf("\n%s is missing!\n\n", DPT_filename);
	exit(0);
}


sprintf(LST_filename, argv[2]);
if(read_binary(total_pixels, LST_filename, "sint", (void *)(&temp))==-1)
{
	printf("\n%s is missing!\n\n", LST_filename);
	exit(0);
}


/****************************************************************
Looping through images
*****************************************************************/

temp_ptr = temp;
dpt_ptr = dpt;
rhumid_ptr = rhumid;

for(i=0; i<total_pixels;i++)
    {
    *rhumid_ptr = (signed short int)(relative_humidity(*dpt_ptr,*temp_ptr));

  		dpt_ptr++;
  		temp_ptr++;
  		rhumid_ptr++;
  	}




 /***********************************************************************
Free memory allocated for inputs
************************************************************************/
free(dpt);
free(temp);

/*************************************************************************
Write images and free memory
*************************************************************************/
sprintf(RH_filename, argv[3]);
write_binary(total_pixels, RH_filename, "sint", (signed short int *)(&rhumid));
free(rhumid);
}

/*******************calculating relative humidity**********************/
float relative_humidity(signed short int  rh_dpt, signed short int  rh_temp)
{

float rel_humid, vp, sat_vp;

/*vp is the vapour pressure calculated from temperature*/
/*sat_vp is the saturated vapour pressure from dewpoint*/

/*check for data flags*/
if(rh_temp < 0 )
	{
	rel_humid=(float)rh_temp; /*data flag in output file will be the same as input file*/
	}
else
	{
	/*calculate vapor pressure*/
	vp = 0.611*exp(17.27*((rh_temp/10.0)-273)/((rh_temp/10.0)-36));
	/*calculate saturated vapor pressure*/
	sat_vp = 0.611*exp(17.27*((rh_dpt/10.0)-273)/((rh_dpt/10.0)-36));

		if (sat_vp == 0)
			{
			rel_humid =0.0;
			}
		else
			{
			rel_humid=(sat_vp/vp)*1000;
			}
	}

return rel_humid;
}

