/******************************************************************************************
Keetch_Byram_SI.c

created 12/28/2004
Karyn Tabor

last updated: 06/23/2009

Calculates the Keetch-Byram Drought Index (KBDI) which considers the topmost layers of soil such that their field capacity is 200 mm of available water. The index estimates how much effective rainfall is needed to saturate this depth of soil at any time. Moisture is lost from the soil only by evaporation due to temperature effects. The first 5 mm of rainfall is lost to the canopy. There is a problem if any of the rainfall is lost as surface run-off.

based on equations from Alexander (1990)
Alexander, M.E. 1990. Computer Calulation of the Keetch-Byram Drought Index-Programmers Beware! Fire Management Today. 51:23-25.

KBDI values range from 0-203 mm

KBDI assumes flat terrain, and thus uniform drying. 

Command line arguments
<previous day's temperature> <annual average rainfall> <last rainfall> <daysince last rainfall> <output Byram-Keetch> <output drought index> <total pixels>

Inputs:
todays max temperature file is LST in degrees K*10 (signed 16 bit) - SFT_MOD07_YYYYJJJ_Day_AMZ_int1.bin
todays rainfall in mm (signed 16 bit) - ppt_3b42rt_YYYYJJJ_sum_AMZ.bin
Annual average rainfall mm	(signed 16 bit) - cru_aveann_ppt_mm_sin.bin
Previous Day's Byram-Keetch Index [mm] output values are mm (signed 16 bit) - BKDI_YYYYJJJ_win1.bin
total pixels = rows*cols

output is:
KBDI [mm] output values are mm (signed 16 bit) - KBDI_YYYYJJJ_win1.bin
The second ouput for debugging, Delta_KBDI is commented out. Umcomment to get output file: Delta_KBDI change in DBKI (signed 16 bit) - DKB_YYYYJJJ_win1.bin 


******************************************************************************************/

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


main(int argc, char *argv[])
{

int i;
int total_pixels;

/*input files*/
signed short int *lastrain, *lastrain_ptr, *prevtemp, *prevtemp_ptr, *ave_ar, *ave_ar_ptr, *prevKBDI, *prevKBDI_ptr;

/*output*/
signed short int *Delta_KBDI, *Delta_KBDI_ptr, *KBDI, *KBDI_ptr;

/*input files*/
char rainsum_filename[101], temp_filename[101], ave_ar_filename[101], prevKB_filename[101];
/*output files*/
char KB_filename[101], Delta_KBDI_filename[101];

float KBDI_flt, delta_KBDI_flt;

/*initialize Keetch-Byram index at abritrary value*/
float initKBDI = 0.0;

float D_KBDI(signed short int, signed short int, signed short int, signed short int);


total_pixels=atoi(argv[7]);


 /** Check args, if incorrect specify usage **/

  if( argc!=8 )
   {
      printf("Usage:\n\n");
      printf("<previous day's temperature> <last rainfall> <Annual average rainfall>/  \n");
      printf("<Previous Day's Keetch-ByramIndex>/ \n");
	  printf("<Keetch-Byram Drought Index > <Delta_KBDI > <total pixels> \n\n");
      printf("See source code file for command line argument values\n");
      return 1;
   }
/**********!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!************/
printf("total pixels is %d\n",total_pixels);
/*************************************************************
Memory Allocation
*************************************************************/
allocate_binary(total_pixels, "sint", (void *)(&lastrain));
allocate_binary(total_pixels, "sint", (void *)(&prevtemp));
allocate_binary(total_pixels, "sint", (void *)(&ave_ar));
allocate_binary(total_pixels, "sint", (void *)(&prevKBDI));
allocate_binary(total_pixels, "sint", (void *)(&KBDI));
allocate_binary(total_pixels, "sint", (void *)(&Delta_KBDI));



/***********************************************************
Read input files
************************************************************/


 sprintf(temp_filename, argv[1]);
 if(read_binary(total_pixels, temp_filename, "sint", (void *)(&prevtemp)) ==-1)
 	{
      printf("Previous day's temperature does not exit\n");
      exit (0);
  	}

sprintf(ave_ar_filename, argv[2]);
if(read_binary(total_pixels, ave_ar_filename, "sint", (void *)(&ave_ar))==-1)
{
	printf("\n%s is missing!\n\n", ave_ar_filename);
	exit(0);
}

sprintf(rainsum_filename, argv[3]);
if(read_binary(total_pixels, rainsum_filename, "sint", (void *)(&lastrain))==-1)
 {
	printf("\n%s is missing!\n\n", rainsum_filename);
	exit(0);
 }



sprintf(prevKB_filename, argv[4]);
if(read_binary(total_pixels, prevKB_filename, "sint", (void *)(&prevKBDI))==-1)
 {
	printf("\n%s is missing!\n\n", prevKB_filename);
	*prevKBDI= initKBDI;
 }

/****************************************************************
Looping through images
*****************************************************************/

prevtemp_ptr = prevtemp;
ave_ar_ptr = ave_ar;
lastrain_ptr = lastrain;
prevKBDI_ptr = prevKBDI;
KBDI_ptr = KBDI;
Delta_KBDI_ptr = Delta_KBDI;


for(i=0; i<total_pixels;i++)
    {
		/*check for data flags*/
		if(*prevtemp_ptr <0)
			{
			*KBDI_ptr = *prevtemp_ptr;
			*Delta_KBDI_ptr = *prevtemp_ptr;
			}
		else
			{

			/*calculate change in Keetch-Bryam*/
			delta_KBDI_flt = D_KBDI(*lastrain_ptr, *prevKBDI_ptr, *prevtemp_ptr, *ave_ar_ptr);

			/*calculate Bryam-Keetch*/
			 KBDI_flt = *prevKBDI_ptr+delta_KBDI_flt;

				if( KBDI_flt < 0.0)
					 KBDI_flt=0.0;
				else if ( KBDI_flt > 203.2)
					 KBDI_flt=203.2;
				
			*KBDI_ptr = (signed short int)KBDI_flt;
			*Delta_KBDI_ptr=(signed short int)delta_KBDI_flt;

			}

  prevtemp_ptr++;
  ave_ar_ptr++;
  lastrain_ptr++;
  prevKBDI_ptr++;
  KBDI_ptr++;
  Delta_KBDI_ptr++;

}




 /***********************************************************************
Free memory allocated for inputs
************************************************************************/
free(lastrain);
free(ave_ar);
free(prevtemp);
free(prevKBDI);

/*************************************************************************
Write images and free memory
*************************************************************************/

sprintf(KB_filename, argv[5]);
write_binary(total_pixels, KB_filename, "sint", (void *)(&KBDI));
free(KBDI);

sprintf(Delta_KBDI_filename, argv[6]);
/*write_binary(total_pixels, Delta_KBDI_filename, "sint", (void *)(&Delta_KBDI));*/
free(Delta_KBDI);


}



/*******************Moisture exchange by fuels**********************/
float D_KBDI(signed short int lastrain_in, signed short int KBDI_in, signed short int prevtemp_in, signed short int ave_ar_in)
{
float delta_KBDI_exp;
float prevtempC;

prevtempC =  prevtemp_in/10-273.0;

if(	lastrain_in > 0)
	{
	delta_KBDI_exp=0;
	if (lastrain_in >=5)
	delta_KBDI_exp=(5-lastrain_in);
	}
else
{
	/*delta_KBDI_exp=(800-KBDI_in)*(0.968*exp(0.0486*(prevtempC*9/5+32))-8.3)*0.001/(1+10.88*exp(-0.0441*ave_ar_in/25.4))*0.254;*/
	delta_KBDI_exp=0.001*(203.2-KBDI_in)*(0.968*exp(0.0875*prevtempC+1.5552)-8.3)/(1+10.88*exp(-0.001736*ave_ar_in));
}

return delta_KBDI_exp;
}









