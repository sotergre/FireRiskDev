/******************************************************************************************
rhumid_at_dpt_nas_plevels.c

Calculates relative humidity from dew point temperature and temperature. Has to read the surface pressure file first to know which pressure level for the DPT and AT to use.

last update =6/12/2013

Input: 32 bit float dewpoint, temperature, and surface pressure files

Output: binary signed 16 bit relative humidity 1000%

Command line arguments

<julian day> <year> <image size in total pixels>


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
char SP_filename[101], DPT_filename[101], LST_filename[101], RH_filename[101];
char outdir[101];
int jday, year, i,j, k, total_pixels, surfaceP_int, min;
float *surfaceP, *temp, *dpt, *temp_tmp, *dpt_tmp;
float *temp_ptr,*dpt_ptr;
float *rhumid,*rhumid_ptr;
int *plevelarray, *plevelarray_ptr;
float rh_dpt, rh_temp;

float relative_humidity(float, float);


 /** Check args, if incorrect specify usage **/

   if( argc!=5 )
   {
      printf("Usage:\n\n");
      printf(" <julian day> <year> <output directory> <image size in total pixels> \n\n");
      printf("See source code file for command line argument values\n");
      return 1;
   }

jday=atoi(argv[1]);
year=atoi(argv[2]);
sprintf(outdir,argv[3]);
total_pixels=atoi(argv[4]);
printf("total number of pixels is %d\n", total_pixels);

/*sureface pressure related to MODIS atmospheric profile level*/
int plevel[18][2]={ {4,30,}, {5,50,}, {6,70,}, {7,100,}, {8,150,}, {9,200,}, {10,250,}, {11,300,}, {12,400,}, {13,500,}, {14,620,}, {15,700,}, {16,780,}, {17,850,}, {18,920,}, {19,950,}, {20,1000,}, {21,2000} };

/***********************Reading in data**************************/
/*rhumid is the 1000% realtive humidity*/
/*temp is the air temperature in degrees Kelvin*10*/
/*dpt is dew point is in degrees Kelvin*10*/

/*************************************************************
Memory Allocation
*************************************************************/
allocate_binary(total_pixels, "float", (void *)(&surfaceP));
allocate_binary(total_pixels, "int", (void *)(&plevelarray));
allocate_binary(total_pixels, "float", (float *)(&temp));
allocate_binary(total_pixels, "float", (float *)(&dpt));
allocate_binary(total_pixels, "float", (float *)(&rhumid));

/***********************************************************
Read input surface pressure file
************************************************************/

sprintf(SP_filename, "%sSF_Pressure/%d/SP_MOD07_%d%.3d_Day_AMZEX.bin", outdir, year, year, jday);
if(read_binary(total_pixels, SP_filename, "float", (void *)(&surfaceP))==-1)
{
	printf("\n%s is missing!\n\n", DPT_filename);
	exit(0);
}


/*convert Surface P array from float to int*/
for (i=0; i<total_pixels; i++)
{
	if(surfaceP[i] > 0.0)
	{	
		surfaceP_int=(int)surfaceP[i];
		k=1;
		for (j=0; j<17; j++)
		{
			if(surfaceP_int >= plevel[j][1] && surfaceP_int < plevel[k][1])
			{
			plevelarray[i]=	plevel[j][0];
			}
		k++;
		}


	}
	else{
	plevelarray[i]=0;
	}

/*initiate rhumid array with zeros*/
rhumid[i]=0;

}/*end for i loop*/

/*find minimum value in plevelarray*/
min=21;
for (i=0; i<total_pixels; i++)
{
	if(plevelarray[i]>0 && plevelarray[i] < min)
	{
	min=plevelarray[i];
	}

}

/*the minimum plevel is 4 but that is equal to the index 0 in plevel array*/
min=min-4;

free(surfaceP);

/****************************************************************
Looping through plevels
****************************************************************/

for (i=min; i<17; i++)
{
	sprintf(DPT_filename, "%sTP_PROFILE/%d/DPT_%d_MOD07_%d%.3d_Day_AMZEX.bin", outdir, year, plevel[i][0],year, jday);

	if(read_binary(total_pixels, DPT_filename, "float", (void *)(&dpt))==-1)
	{
		printf("\n%s is missing!\n\n", DPT_filename);
		exit(0);
	}


	sprintf(LST_filename,  "%sTP_PROFILE/%d/AT_%d_MOD07_%d%.3d_Day_AMZEX.bin", outdir, year, plevel[i][0],year, jday);
	if(read_binary(total_pixels, LST_filename, "float", (void *)(&temp))==-1)
	{
		printf("\n%s is missing!\n\n", LST_filename);
		exit(0);
	}



	/****************************************************************
	Looping through images	
	*****************************************************************/

	plevelarray_ptr = plevelarray;
	temp_ptr = temp;
	dpt_ptr = dpt;
	rhumid_ptr = rhumid;

	for(j=0; j<total_pixels;j++)
	{
		if(*plevelarray_ptr == plevel[i][0])
			{
				if(*dpt_ptr > 0 && *temp_ptr > 0)
				{
				/*printf("%f\t%f\n", *dpt_ptr, *temp_ptr);*/
				*rhumid_ptr = relative_humidity(*dpt_ptr,*temp_ptr);
				/*printf("%f\t",*rhumid_ptr);*/
				}
			}
	plevelarray_ptr++;
  	dpt_ptr++;
  	temp_ptr++;
  	rhumid_ptr++;

	}/*end j loop*/

/*clear arrays to read in new data*/
memset(temp, 0, total_pixels);
memset(dpt, 0, total_pixels);
}/*end for i plevel loop*/

 /***********************************************************************
Free memory allocated for inputs
************************************************************************/
free(dpt);
free(temp);
free(plevelarray);

/*************************************************************************
Write images and free memory
*************************************************************************/
sprintf(RH_filename,  "%sRH/%d/RH_MOD07_%d%.3d_Day_AMZEX.bin", outdir, year,year, jday);
write_binary(total_pixels, RH_filename, "float", (float *)(&rhumid));
free(rhumid);
}

/*******************calculating relative humidity**********************/
float relative_humidity(float  rh_dpt, float  rh_temp)
{

float rel_humid, vp, sat_vp;

/*vp is the vapour pressure calculated from temperature*/
/*sat_vp is the saturated vapour pressure from dewpoint*/
/*printf("%f\t%f\n", rh_dpt, rh_temp);*/
/*check for data flags*/
if(rh_temp < 0 || rh_dpt < 0)
	{
	rel_humid=-1; /*data flag in output file will be the same as input file*/
	}
else
	{
	/*calculate vapor pressure*/
	vp = 0.611*exp(17.27*((rh_temp)-273)/((rh_temp)-36));
	/*calculate saturated vapor pressure*/
	sat_vp = 0.611*exp(17.27*((rh_dpt)-273)/((rh_dpt)-36));

		if (sat_vp == 0)
			{
			rel_humid =0.0;
			}
		else
			{
			rel_humid=(sat_vp/vp)*100;
			}
	}
/*printf("%f\t", rel_humid);*/
return rel_humid;
}

