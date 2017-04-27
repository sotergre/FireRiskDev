/******************************************************************************************
fmodel_10hr.c

last update 11/08/2006

Calculates litter moisture content beginning with previous day's initial litter moisture content

Command line arguments

<rain period 1 > <rain period 2> <humidity infile> <temperature infile> <previous day's moisture content> <output moisture content> <image size in total pixels>

rain files are rain duration in hours (unsigned 8bit)
humidity file is 1000% relative humidity
temperature file is LST in degrees K*10
previous day's moisture content in % (unsigned 8bit)
total pixels = rows*cols



nodatavalue is -1 (cloud) and 0 (missing data/swath)

output is:
moisture content is average moisture content % (unsigned 8bit)


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

main(int argc, char *argv[])
{
int i, total_pixels;
int delta_t;
unsigned char *rain_1, *rain_2,*rain_1_ptr, *rain_2_ptr, *prev_lm, *prev_lm_ptr,*m_content, *m_content_ptr, *m_content_temp, *m_content_temp_ptr;
signed short int *humid, *temp, *humid_ptr, *temp_ptr, *m_exchange, *m_equilibrium, *m_exchange_ptr, *m_equilibrium_ptr;
char filename[100];
char content_filename[101];

float equilibrium(signed short int, signed short int);
float exchange(unsigned char, unsigned char, signed short int, int);


total_pixels=atoi(argv[7]);

 printf("lag is 10\n");
 /** Check args, if incorrect specify usage **/

   if( argc!=8 )
   {
      printf("Usage:\n\n");
      printf("<rain period 1 infile> <rain period 2 infile> <humidity infile> <temperature infile>/  \n");
      printf("<previous day's liter moisture> <output moisture content> <image size in total pixels>\n\n");
      printf("See source code file for command line argument values\n");
      return 1;
   }

/*************************************************************
Memory Allocation
*************************************************************/
allocate_binary(total_pixels, "uchar", (unsigned char *)(&rain_1));
allocate_binary(total_pixels, "uchar", (unsigned char  *)(&rain_2));
allocate_binary(total_pixels, "sint", (signed short int *)(&humid));
allocate_binary(total_pixels, "sint", (signed short int *)(&temp));
allocate_binary(total_pixels, "uchar", (unsigned char  *)(&prev_lm));
allocate_binary(total_pixels, "sint", (signed short int *)(&m_equilibrium));
allocate_binary(total_pixels, "sint", (signed short int *)(&m_exchange));
allocate_binary(total_pixels, "uchar", (unsigned char  *)(&m_content));
allocate_binary(total_pixels, "uchar", (unsigned char *)(&m_content_temp));


/***********************************************************
Read input files
************************************************************/

sprintf(filename, argv[1]);
if(read_binary_fmodel(total_pixels, filename, "uchar", (unsigned char *)(&rain_1))==-1)
{
	printf("\n%s is missing!\n\n", filename);
	exit(0);
}

sprintf(filename, argv[2]);
if(read_binary_fmodel(total_pixels, filename, "uchar", (unsigned char *)(&rain_2))==-1)
{
	printf("\n%s is missing!\n\n", filename);
	exit(0);
}


sprintf(filename, argv[3]);
if(read_binary_fmodel(total_pixels, filename, "sint", (signed short int *)(&humid))==-1)
 {
	printf("\n%s is missing!\n\n", filename);
	exit(0);
 }

sprintf(filename, argv[4]);
if(read_binary_fmodel(total_pixels, filename, "sint", (signed short int *)(&temp))==-1)
 {
	printf("\n%s is missing!\n\n", filename);
	exit(0);
 }


 sprintf(filename, argv[5]);
 if(read_binary_fmodel(total_pixels, filename, "uchar", (unsigned char *)(&prev_lm)) ==-1)
 	{    
	printf("Previous Day's Moisture - %s does not exist\n", filename);
      printf("Initiating Litter Moisture with %d\n",initialmoisture);

	  for(i=0; i<total_pixels;i++)
	  	{
		  *(prev_lm + i)= initialmoisture;
		 }
  	}


/****************************************************************
Looping through images
*****************************************************************/

temp_ptr = temp;
rain_1_ptr = rain_1;
rain_2_ptr = rain_2;
humid_ptr = humid;
prev_lm_ptr = prev_lm;
m_content_ptr = m_content;
m_content_temp_ptr = m_content_temp;
m_exchange_ptr = m_exchange;
m_equilibrium_ptr = m_equilibrium;


for(i=0; i<total_pixels;i++)
    {
		/*check for data flags*/
		if(*temp_ptr <= 0)/*if water*/
			{
			*m_content_ptr = 0;
			*m_equilibrium_ptr = 0;
			*m_exchange_ptr = 0;
			}
		else
			{
			/*calculate moisture equilibrium */
			*m_equilibrium_ptr = (signed short int)(10*(equilibrium(*humid_ptr, *temp_ptr)));
			/*calculate moisture exchange for first 16 hours*/
			delta_t=15;
			*m_exchange_ptr = (signed short int)(10*(exchange(*rain_1_ptr, *prev_lm_ptr, *m_equilibrium_ptr, delta_t)));
			/*calculate moisture content after 16 hours*/
			*m_content_temp_ptr=  (unsigned char)(*prev_lm_ptr + *m_exchange_ptr/10);
			/*So moisture content doesn't go above 100%*/
			if(*m_content_temp_ptr >= 100)
				{
			  	*m_content_temp_ptr=100;
				}
			
			/*calculate moisture exchange for next 8 hours*/
			delta_t=9;
			*m_exchange_ptr = (signed short int)(100*(exchange(*rain_2_ptr, *m_content_temp_ptr, *m_equilibrium_ptr, delta_t)));
			*m_content_ptr=  (unsigned char)(*m_content_temp_ptr + *m_exchange_ptr/100);

			/*So moisture content doesn't go above 100%*/
			if(*m_content_ptr >= 100)
				{
			  	*m_content_ptr=100;
				}
			
		}

  rain_1_ptr++;
  rain_2_ptr++;
  humid_ptr++;
  temp_ptr++;
  prev_lm_ptr++;
  m_content_temp_ptr++;
  m_content_ptr++;
  m_exchange_ptr++;
  m_equilibrium_ptr++;

}




 /***********************************************************************
Free memory allocated for inputs
************************************************************************/
free(rain_1);
free(rain_2);
free(humid);
free(temp);
free(prev_lm);
free(m_content_temp);

/*************************************************************************
Write images and free memory
*************************************************************************/

sprintf(filename, argv[6]);
write_binary(total_pixels, filename, "uchar", (unsigned char *)(&m_content));
free(m_content);
free(m_equilibrium);
free(m_exchange);


}



/*******************Moisture exchange by fuels**********************/
float exchange(unsigned char up_rain, unsigned char m_init, signed short int m_eq, int hrs)
{
float m_b, m_exchange;
/*m_b, the boundary condition, is the time period average moisture content at the fuel surface*/

/*m_exchange is the rate of moisture uptake by fuels*/

	m_b = ((hrs-up_rain)*m_eq/10.0+up_rain*(2.7*up_rain+76))/hrs;
	if (hrs==15)
		m_exchange = (m_b - m_init)*(1-1.1*(exp(-hrs/10.0)));
	else
		m_exchange = (m_b - m_init)*(1-0.87*(exp(-hrs/10.0)));
	/*env parameter 1.023 = (1.1+1.1+.87)/3 based on Forcasting the 10-hour Timelag Fuel Moisture, Fosberg (1977) */

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














