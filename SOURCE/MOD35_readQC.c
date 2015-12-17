
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <math.h>T


/***************************************************************************
 MOD35_readQC.c

Karyn Tabor
3/3/2008


 Generates binary files from Cloud Mask bit-fields in the MOD35v005 product
 Binaries can be produced using the hdf2bin executable found at http://www.hdfeos.org/software/
____________________________________________________________________________

 Arguments: <inputfile> <outputfile> <bit-field> <npixels>

 	    input file: Name of input binary
		output file: Name of output binary
 		bit-field: code for desired bit-field of the chosen SDS
 		see MOD35 user guide for more information http://modis-atmos.gsfc.nasa.gov/MOD35_L2/format.html

		For Cloud Mask Channel bit field codes are:
			Profile #1
				 1) Cloud Mask Flag
				 2) Unobstructed FOV Quality Flag
				 3) Day/Night Path
				 4) Sunglint Path
				 5) Snow/Ice Background Path
			   	 6) Land or Water Flag
			Profile#2
			 	 7) Non-cloud obstruction Flag (heavy aerosol)
				 8) Thin Cirrus Detected (Near_IR)
				 9) Shadow Flag
			 	10) Thin Cirrus Detected (IR)
				11) Adjacent Cloud Detected
				12) Cloud Flag (IR threshold)
				13) High Cloud Flag (CO2 test)
				14) High Cloud Flag (6.7 micron test)
			Profile#3
				15) High Cloud Flag (1.38 micron Test)
				16) High Cloud Flag (3.7-12 micron Test)
				17) Cloud Flag (IR Temperature Difference Test)
				18) Cloud Flag (3.7-11 micron Test)
				19) Cloud Flag (Visible Reflectance Test)
				20) Cloud Flag (Visible Reflectance Ratio Test)
				21) Cloud Flag NDVI (Final Confidence Confirmation Test)
				22) Cloud Flag (- Night 7.3-11 micron Test)

		npixels: number of rows * number of columns
____________________________________________________________________________

 INPUT: signed 8 bit binary file, MOD35 QC Channels:

 ____________________________________________________________________________

OUTPUT: 8 bit unsigned binary file

			1) Cloud Mask Flag
			filename: MOD35_CM.bin
					1 = Not Determined
					2 = Determined

			2) Unobstructed FOV Quality Flag
			filename: MOD35_FOV_qual.bin
					1 = Cloudy
					2 = Uncertain
					3 = Probably Clear
					4 = Confident Clear

			3) Day or Night Path
			filename: MOD35_DNpath.bin
					1 = Night
					2 = Day

			4) Sunglint Path
			filename: MOD35_sgpath.bin
					1 = Yes
					2 = No

			5) Snow/Ice Background Path
			filename: MOD35_SIpath.bin
					1 = Yes
					2 = No

			6) Land or Water Path
			filename:  MOD35_land.bin
					1 = Water
					2 = Coastal
					3 = Desert
					4 = Land

			8+) For Bits 8 - 24
					1 = Yes
					2 = No
				 8) Non-Cloud Obstruction Flag
				 9) Thin Cirrus Detected (Solar Test)
				10) Shadow Flag
				11) Thin Cirrus Detected (Infrared Test)
				12) Adjacent Cloud Detected (within surrounding 1 km pixels)
				13) Cloud Flag (IR Threshold Test)
				14) High Cloud Flag (CO2 Test)
				15) High Cloud Flag (6.7 micron Test)
				
____________________________________________________________________________

	 compile
	 gcc -g -o MOD35_cloudmask MOD35_cloudmask.c

	includes:
	allocate_binary.c
	write_binary.c
	read_binary.c

***************************************************************************/

main( int argc, char **argv )
{

   /* command line arguments */
   char CM_in_fn[101];
   char CM_out_fn[101];
   int bitfield;
   int npixels;
	int test;
   int change_bit[6]= {0,1,3,4,5,6};

   int bit;

   /* pointer the CM channel */
   unsigned char *CM_in;

   /* pointer to output image */
   unsigned char *CM_out;


   /* pixel loop counter */
   int pixel, curr_bit, next_bit, next_next_bit, maskbit;

   /** read command line arguments **/

   sprintf(CM_in_fn,argv[1]);
   sprintf(CM_out_fn,argv[2]);
   bitfield=(int)atoi(argv[3]);
   npixels=atoi(argv[4]);

/*****************************************************************************/
/*Allocating memory for input buffers*/



   /* allocate memory for the input CM file */
   allocate_binary(npixels,"char",(void *)(&CM_in));

  /* allocate memory for the output CM file */
   allocate_binary(npixels,"uchar",(void *)(&CM_out));

    if(read_binary(npixels,CM_in_fn,"char",(void *)(&CM_in))==-1)
  	 {
      printf("Error: couldn't read input file %s\n",CM_in_fn);
      exit(1);
  	 }

	/*determine starting bit*/
	if (bitfield<7)
	{
	bit=change_bit[bitfield-1];
		

   	/*mask bit is the cloud mask field determined/not determined*/
	maskbit=pow(2,0);
	curr_bit=pow(2,bit);
	next_bit=pow(2,bit+1);


	/* pixels loop */
	for( pixel=0; pixel<npixels; pixel++ )
	{
	   if ( bit == 1 || bit == 6)
    		{
			if((*(CM_in+pixel)&maskbit)) /*check byte 1 to see if cloud mask is determined*/
			{

		 		if((*(CM_in+pixel)&curr_bit) && (*(CM_in+pixel)&next_bit) )
		 			{
		 			*(CM_out+pixel) = 4;
		 			}
				else if ((*(CM_in+pixel)&curr_bit) && !(*(CM_in+pixel)&next_bit))
				 		{
				 		*(CM_out+pixel) = 3;
				 		}
		 		else if(!(*(CM_in+pixel)&curr_bit)&& (*(CM_in+pixel)&next_bit))
		 			{
		 				*(CM_out+pixel) = 2;
		 			}

		 			else if (!(*(CM_in+pixel)&curr_bit)&& !(*(CM_in+pixel)&next_bit))
				 		{
				 		*(CM_out+pixel) = 1;
				 		}
			 	}/*end if &naskbit*/
			 else
			 	{
			 	*(CM_out+pixel) = 0;
			 	}

		 } /*end if bit is 1 or 6...*/

		 else
		 {
			if((*(CM_in+pixel)&maskbit))/*check byte 1 to see if cloud mask is determined*/
		 	{
		 		if( (*(CM_in+pixel)&curr_bit) )
		 			{
		 			*(CM_out+pixel) = 2;
		 			}
				else
					{
		 			*(CM_out+pixel) = 1;
		 			}
			}
		 	else
			 {
			 *(CM_out+pixel) = 0;
		 	 }

		 } /*end else bit 1 or 6...*/

	 } /* end pixel loop */
	}/*if bit<7)*/
	
	else
	{
	bit=bitfield-7;
  	/*mask bit is the cloud mask field determined/not determined*/
	maskbit=pow(2,0);
	curr_bit=pow(2,bit);
	next_bit=pow(2,bit+1);
	/* pixels loop */
		for( pixel=0; pixel<npixels; pixel++ )
		{	
			if((*(CM_in+pixel)&maskbit))/*check byte 1 to see if cloud mask is determined*/
	 		{
	 			if( (*(CM_in+pixel)&curr_bit) )
	 				{
		 			*(CM_out+pixel) = 2;
		 			}
				else
					{
	 				*(CM_out+pixel) = 1;
		 			}
			}
	 		else
		 	{
			 *(CM_out+pixel) = 0;
	 		 }
 		} /* end pixel loop */
	}/*end else*/


    /* free memory for input*/
    free(CM_in);

 /* write output file */

   if( write_binary(npixels,CM_out_fn,"uchar",(void *)(&CM_out))==-1 )
   {
      printf("Error: couldn't write value mask file %s\n",CM_out_fn);
      exit(1);
   }

   /* free memory for */

   free(CM_out);

   return 0;

}
