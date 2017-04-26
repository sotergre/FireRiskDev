#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <math.h>



/***************************************************************************
 HDF_QC.c

Karyn Tabor
2/12/2007


 Generates QC files from QA bit-fields in the MOD11B1 V4 and MOD07L2 V4 product
____________________________________________________________________________

 Arguments: <inputfile> <outputfile> <product> <startbit> <nrows> <ncols>

 	input file: YYYYJJJ.MOD11_QC_DAY.bin
		YYYY: is the 4 digit year
		JJJ: is the 3 digit julian day
    product is either MOD11 or MOD07
	startbit: Is the bit number for desired QC bit field (see below)

		nrows: number of rows

		ncolc: number of columns
____________________________________________________________________________

 INPUT: unsigned 8 byte binary file, MOD11 QC Channels:

 input file: GRID_MODIS_Grid_Daily_1km_LST_QC_Day.bin

____________________________________________________________________________

OUTPUT: 8 byte unsigned binary file of desired QC channel

	starbit and values for MOD11B1 QC_Day:

			0) Mandatory QA Flags
					0 = LST produced, good quality, not necessary to examine more detailed QA
					1 = LST produced, other quality, recommend examination of more detailed QA
					2 = LST not produced due to cloud effects
					3 = LST not produced primarily due to reasons other than cloud

			2) Data Quality Flags
			filename: MOD11_Data_qual.bin
					0 = Good Data Quality
					1 = Other Quality Data
					2 = TBD
					3 = TBD

			4) Emis Error Flag
			filename: MOD1_Emis_err.bin
					0 = Average Emissivity Error <=0.01
					1 = Average Emissivity Error <=0.02
					2 = Average Emissivity Error <=0.04
					3 = Average Emissivity Error  >0.04

			6) LST Error Flag
			filename: MOD11_LST_err.bin
					0 = Average LST Error <= 1K
					1 = Average LST Error <= 2K
					2 = Average LST Error <= 3K
					3 = Average LST Error  > 3K

starbit and values for MOD07L2 Quality_Assurance:

			0) Retrived Temperature Profile QA
					0 = Not Useful
					1 = Useful

			1) Retrived Temperature Profile Confidence
					0 = Lowest Confidence
					1 = Intermediate Confidence
					2 = High Confidence
					3 = Highest Confidence

			4) Retrived Moisture Profile QA
					0 = Not Useful
					1 = Useful

			5) Retrived Moisture Profile Confidence
					0 = Lowest Confidence
					1 = Intermediate Confidence
					2 = High Confidence
					3 = Highest Confidence


____________________________________________________________________________

	 compile
	 gcc -g -o MOD11_QC MOD11_QC.c -lm

	includes:
	allocate_binary.c
	write_binary.c
	read_binary.c
	general_header.h

***************************************************************************/

main( int argc, char **argv )
{

   /* command line arguments */
   char QC_in_fn[101];
   char QC_out_fn[101];
   char product[6];
   int startbit;
   int nrows, ncols;

   int npixels, bit;
   /* pointer the QC channel */

   float *MOD11_QC_in;
   unsigned char *QC_in_char;

   /* pointer to output image */
   unsigned char *QC_out;


   /* pixel loop counter */
   int pixel, curr_bit, next_bit;

   /* bytes read/written check */
   signed int nbytes;
   char *fcvt();



   /** read command line arguments **/

   sprintf(QC_in_fn,argv[1]);
   sprintf(QC_out_fn,argv[2]);
   sprintf(product,argv[3]);
   startbit=(int)atoi(argv[4]);
   nrows=(int)atoi(argv[5]);
   ncols=(int)atoi(argv[6]);

 /*calculating number of pixels*/
  npixels=nrows*ncols;


/*****************************************************************************/
/*Allocating memory for input buffers*/

 /* allocate memory for the input QC file */
   allocate_binary(npixels,"float",(void *)(&MOD11_QC_in));
   allocate_binary(npixels,"uchar",(void *)(&QC_in_char));

  /* allocate memory for the output QC file */
   allocate_binary(npixels,"uchar",(void *)(&QC_out));

    if (strcmp(product,"MOD11")==0)
    {
   	 if(read_binary(npixels,QC_in_fn,"float",(void *)(&MOD11_QC_in))==-1)
  		 {
   	   printf("Error: couldn't read input file %s\n",QC_in_fn);
   	   exit(1);
  		 }


		 for( pixel=0; pixel<npixels; pixel++ )
		 {
		 *(QC_in_char+pixel) = (char)*(MOD11_QC_in+pixel);
 		}
	free(MOD11_QC_in);
	}
	else if (strcmp(product,"MOD07")==0)
	{
		    if(read_binary(npixels,QC_in_fn,"char",(void *)(&QC_in_char))==-1)
		  	 {
		      printf("Error: couldn't read input file %s\n",QC_in_fn);
		      exit(1);
		  	 }
	}
 else
  	{
	printf("enter valid product.  Either MOD11 or MOD07\nEXITING PROGRAM\n");
	exit(0);
  	}

    curr_bit=pow(2,startbit);
    next_bit=pow(2,startbit+1);

if (strcmp(product,"MOD11")==0)
{

	printf("MOD11\n");
  /* pixels loop */
  for( pixel=0; pixel<npixels; pixel++ )
	 {
		 	if((*(QC_in_char+pixel)&curr_bit) && (*(QC_in_char+pixel)&next_bit))
		 		{
		 		*(QC_out+pixel) = 3;
		 		}
			else if ((*(QC_in_char+pixel)&curr_bit) && !(*(QC_in_char+pixel)&next_bit))
			 		{
			 		*(QC_out+pixel) = 1;
			 		}
		 	else if(!(*(QC_in_char+pixel)&curr_bit)&& (*(QC_in_char+pixel)&next_bit))
		 		{
		 			*(QC_out+pixel) = 2;
		 		}

		 		else if (!(*(QC_in_char+pixel)&curr_bit)&& !(*(QC_in_char+pixel)&next_bit))
			 		{
			 		*(QC_out+pixel) = 0;
			 		}

	 } /* end pixel loop */

}

else if (strcmp(product,"MOD07")==0)
{

	printf("MOD07\n");
	  /* pixels loop */
	  for( pixel=0; pixel<npixels; pixel++ )
		 {
			 if (startbit == 0 || startbit==4)
				 {
					 if (pixel==0)
					 {printf("startbit is %d\n", startbit);}
					 if((*(QC_in_char+pixel)&curr_bit) )
					 	{
					 	*(QC_out+pixel) = 1;
					 	}
					 else
					 	{
					 	*(QC_out+pixel) = 0;
					  	}
				 }
				 else
				 {

				 	if((*(QC_in_char+pixel)&curr_bit) && (*(QC_in_char+pixel)&next_bit))
				 		{
				 		*(QC_out+pixel) = 3;
				 		}
					else if ((*(QC_in_char+pixel)&curr_bit) && !(*(QC_in_char+pixel)&next_bit))
					 		{
					 		*(QC_out+pixel) = 1;
					 		}
				 	else if(!(*(QC_in_char+pixel)&curr_bit)&& (*(QC_in_char+pixel)&next_bit))
					 		{
				 			*(QC_out+pixel) = 2;
					 		}

			 		else if (!(*(QC_in_char+pixel)&curr_bit)&& !(*(QC_in_char+pixel)&next_bit))
					 		{
					 		*(QC_out+pixel) = 0;
					 		}
				} /*end if starbit loop*/
		 } /* end pixel loop */

}


    /* free memory for input*/
    free(QC_in_char);

 /* write output file */

   if( write_binary(npixels,QC_out_fn,"uchar",(void *)(&QC_out))==-1 )
   {
      printf("Error: couldn't write value mask file %s\n",QC_out_fn);
      exit(1);
   }

   /* free memory for */

   free(QC_out);

   return 0;

}
