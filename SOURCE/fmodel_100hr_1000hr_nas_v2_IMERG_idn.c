/******************************************************************************************
fmodel_100hr_1000hr_nas_v2_IMERG.c
last update 7/1/2015

Calculates litter moisture content beginning with previous day's initial litter moisture content

Command line arguments

<year> <jday> <prevyear> <prejday> <image size in total pixels> <data dir> <lag>

rain files are rain duration in hours*10 (signed 16-bit)
humidity file is 1000% relative humidity
temperature file is LST in degrees K*10
previous day's moisture content in % (unsigned 8bit)
output number of days since litter moisture value (unsigned 8bit)
total pixels = rows*cols

total pixels = rows*cols
lag = 100 or 1000

output is:
moisture content is average moisture content (unsigned 8bit)
number of days since last moisture value

******************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <math.h>

#define initialmoisture 30 /*m_init = initial moisture content*/
#define delta_t 24 /*the time period of moisture change under question in hours*/

main(int argc, char *argv[])
{
int i, lag, total_pixels, year, prevyear,jday,prevjday;

signed short int *humid, *temp, *humid_ptr, *temp_ptr,  *m_exchange, *m_equilibrium,  *m_exchange_ptr, *m_equilibrium_ptr, *count_prev_lm, *count_prev_lm_ptr, *count_lm, *count_lm_ptr,  *prev_lm, *prev_lm_ptr, *m_content, *m_content_ptr;
signed short int *rain, *rain_ptr;
char filename[100];
char content_filename[101];
char data_dir[101];

float equilibrium(signed short int, signed short int);
float exchange(signed short int, signed short int, signed short int, int);


/** Check args, if incorrect specify usage **/

if( argc!=8 )
{
printf("Usage:\n\n");
printf("<year> <jday> <prevyear> <prevjday>/  \n");
printf(" <image size in total pixels> <data dir> <lag> \n\n");
printf("See source code file for command line argument values\n");
return 1;
}


/***********************************************************
Read input arguments
************************************************************/

year=atoi(argv[1]);
jday=atoi(argv[2]);
prevyear=atoi(argv[3]);
prevjday=atoi(argv[4]);
total_pixels=atoi(argv[5]);
sprintf(data_dir, "%s" , argv[6]);
lag=atoi(argv[7]);
printf("lag is %d\n", lag);

/*************************************************************
Memory Allocation
*************************************************************/
allocate_binary(total_pixels, "sint", (signed short int *)(&rain));
allocate_binary(total_pixels, "sint", (signed short int *)(&humid));
allocate_binary(total_pixels, "sint", (signed short int *)(&temp));
allocate_binary(total_pixels, "sint", (signed short int *)(&prev_lm));
allocate_binary(total_pixels, "sint", (signed short int  *)(&count_prev_lm));
allocate_binary(total_pixels, "sint", (signed short int  *)(&count_lm));
allocate_binary(total_pixels, "sint", (signed short int *)(&m_equilibrium));
allocate_binary(total_pixels, "sint", (signed short int *)(&m_exchange));
allocate_binary(total_pixels, "sint", (signed short int *)(&m_content));

/***********************************************************
Read input files
************************************************************/

sprintf(filename, "%s/IDN/IMERG/%d/daysum/ppt_IMERG30m_early_%d%.3d_dur_IDN.bin" , data_dir, prevyear, prevyear,prevjday);
if(read_binary_fmodel(total_pixels, filename, "sint", (signed short int *)(&rain))==-1)
{
printf("\n%s is missing!\n\n", filename);
exit(0);
}


sprintf(filename, "%s/IDN/MODOUT/RH/%d/RH_MOD07_%d%.3d_Day_IDN_interp.bin" , data_dir, year, year,jday);
if(read_binary_fmodel(total_pixels, filename, "sint", (signed short int *)(&humid))==-1)
{
printf("\n%s is missing!\n\n", filename);
exit(0);
}

sprintf(filename, "%s/IDN/MODOUT/SFT/%d/SFT_MOD07_%d%.3d_Day_IDN_interp.bin" , data_dir, year, year,jday);
if(read_binary_fmodel(total_pixels, filename, "sint", (signed short int *)(&temp))==-1)
{
printf("\n%s is missing!\n\n", filename);
exit(0);
}

sprintf(filename, "%s/IDN/MODOUT/LM/%d/HR%.4d/LM%.4dh_%d%.3d_Day_IDN_last.bin" , data_dir, prevyear, lag, lag, prevyear,prevjday);
if(read_binary_fmodel(total_pixels, filename, "sint", (signed short int *)(&prev_lm)) ==-1)
{
printf("Previous Day's Moisture - %s does not exist\n", filename);
printf("Initiating Litter Moisture with %d\n",initialmoisture);

for(i=0; i<total_pixels;i++)
{
  *(prev_lm + i)= initialmoisture;
 }
}

sprintf(filename, "%s/IDN/MODOUT/LM/%d/HR%.4d/LM%.4dh_%d%.3d_Day_IDN_count.bin" , data_dir, prevyear, lag,lag, prevyear,prevjday);
if(read_binary_fmodel(total_pixels, filename, "sint", (signed short int *)(&count_prev_lm)) ==-1)
{
printf("Count for Previous Day's Moisture - %s does not exist\n", filename);
}


/****************************************************************
Looping through images
*****************************************************************/

temp_ptr = temp;
rain_ptr = rain;
humid_ptr = humid;
prev_lm_ptr = prev_lm;
count_lm_ptr = count_lm;
count_prev_lm_ptr = count_prev_lm;
m_content_ptr = m_content;
m_exchange_ptr = m_exchange;
m_equilibrium_ptr = m_equilibrium;


for(i=0; i<total_pixels;i++)
{
/*check for data flags*/
if(*temp_ptr ==0)
	{
	*m_content_ptr = 0;
	*m_equilibrium_ptr = 0;
	*m_exchange_ptr = 0;
	*prev_lm_ptr = 0;
	*count_lm_ptr=-2;
	}
else if (*temp_ptr ==-1 || *humid_ptr == -1 )/*if nodatavalue*/
	{
	*m_content_ptr = -1;
	*m_equilibrium_ptr = -1;
	*m_exchange_ptr = -1;
	/*update count for how many days since previous valid LM value*/
	*count_lm_ptr=*count_prev_lm_ptr+1;
	}
else
	{
	/*calculate moisture equilibrium */
	*m_equilibrium_ptr = (signed short int)(10*(equilibrium(*humid_ptr, *temp_ptr)));
	/*calculate moisture exchange*/
	*m_exchange_ptr = (signed short int)(100*(exchange(*rain_ptr, *prev_lm_ptr, *m_equilibrium_ptr, lag)));
	/*Add moisture content to previous day's moisture content*/
	*m_content_ptr=  (signed short int)(*prev_lm_ptr + *m_exchange_ptr/100);

	/*So moisture content doesn't go above 100%*/
	if(*m_content_ptr >= 100)
		{
		*m_content_ptr=100;
		}
	else if(*m_content_ptr <= 0)
		{
		*m_content_ptr=0;
		}
	/*update prev_lm file with new values*/
	*prev_lm_ptr = *m_content_ptr;
	}


rain_ptr++;
humid_ptr++;
temp_ptr++;
prev_lm_ptr++;
count_lm_ptr = count_lm;
count_prev_lm_ptr = count_prev_lm;
m_content_ptr++;
m_exchange_ptr++;
m_equilibrium_ptr++;

}




/***********************************************************************
Free memory allocated for inputs
************************************************************************/
free(rain);
free(humid);
free(temp);
free(count_prev_lm);
free(m_exchange);
free(m_equilibrium);

/*************************************************************************
Write images and free memory
*************************************************************************/

sprintf(filename, "%s/IDN/MODOUT/LM/%d/HR%.4d/LM%.4dh_%d%.3d_Day_IDN.bin" , data_dir, year, lag, lag, year,jday);
write_binary(total_pixels, filename, "sint", (signed short int *)(&m_content));

sprintf(filename, "%s/IDN/MODOUT/LM/%d/HR%.4d/LM%.4dh_%d%.3d_Day_IDN_last.bin" , data_dir, year, lag, lag, year,jday);
write_binary(total_pixels, filename, "sint", (signed short int *)(&prev_lm));

sprintf(filename, "%s/IDN/MODOUT/LM/%d/HR%.4d/LM%.4dh_%d%.3d_Day_IDN_count.bin", data_dir, year, lag, lag, year,jday);
write_binary(total_pixels, filename, "sint", (signed short int *)(&count_lm));

free(m_content);
free(prev_lm);
free(count_lm);

}



/*******************Moisture exchange by fuels**********************/
float exchange(signed short int up_rain,  signed short int m_init, signed short int m_eq, int lag)
{
float m_b, m_exchange;
/*m_b, the boundary condition, is the time period average moisture content at the fuel surface*/

/*m_exchange is the rate of moisture uptake by fuels*/
if (lag == 100)
{
m_b = ((delta_t-0.1*up_rain)*m_eq/10.0+0.1*up_rain*(0.5*0.1*up_rain+41))/delta_t;
/*boundary condition is based on The National Fire-Danger Rating System: basic equations, Cohen & Deeming*/
m_exchange = (m_b - m_init)*(1-.87*(exp(-0.24)));
/*env paramter 0.7547 based on Moisture Content Calculations for 100-hour Timelag Fuel in Fire Danger Rating, Fosberg (1971) */
}
else if (lag == 1000)
{
m_b = ((delta_t-0.1*up_rain)*m_eq/10.0+0.1*up_rain*(2.7*0.1*up_rain+76))/delta_t;
m_exchange = (m_b - m_init)*(1-.82*(exp(-0.024)));
/*env parameter 0.82 based on Moisture Content Calculations for 1000-Hour Timelag Fuels, Fosberg (1981) */
}
else
{
printf("Invlaid Lag time\n...exiting\n");
exit(0);
}

return m_exchange;
}

/*********************Rates of moisture exchange*******************/
float equilibrium(signed short int ex_humid, signed short int ex_temp)
{
float m_eq, temp_F;
/*convert kelvin to Fahrenheit*/
temp_F=(ex_temp/10.0 - 273.15)*9/5+32;

/*Calculating, m_eq, the equilibrium moisture content*/

if (ex_humid <= 100)
m_eq = 0.03229 + 0.281073*ex_humid/10.0 - 0.000578*(ex_humid/10.0)*temp_F;

else if (ex_humid <= 500)
{
m_eq = 2.22749 + 0.160107*(ex_humid/10.0) - 0.014784*temp_F;
}
else
m_eq = 21.0606 + .005565*pow(ex_humid/10.0,2.0) - 0.00035*ex_humid/10.0*temp_F - 0.483199*ex_humid/10.0;

return m_eq;

}









