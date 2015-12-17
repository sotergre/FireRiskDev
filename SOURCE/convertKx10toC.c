/******************************************************************************************
convertKx10toC.c

last update 2/27/2008

converts a temperature file of degrees Kelvin *10 to degrees Celcius
and scales to min and max values

Command line arguments

<infile> <outfile> <total pixels> <min val> <max val>

input is signed 16bit file and output is unsigned 8bit file.  All negative numbers are set to min val
min and max vals are celcius

******************************************************************************************/


#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <math.h>

main(int argc, char *argv[])
{
int i, lag, total_pixels, minval, maxval,maxval_K;
signed short int *input_K, *input_K_ptr;
unsigned char *output_C, *output_C_ptr;
char input_K_filename[101], output_C_filename[101];
float degreesC;

total_pixels=atoi(argv[3]);
minval=atoi(argv[4]);
maxval=atoi(argv[5]);


 /** Check args, if incorrect specify usage **/

   if( argc!=6 )
   {
      printf("Usage:\n\n");
      printf("<input Kelvin file> <output Celcius file> <total pixels> <min val> <maxval>\n");
      printf("See source code file for command line argument values\n");
      return 1;
   }

/*************************************************************
Memory Allocation
*************************************************************/
allocate_binary(total_pixels, "sint", (void *)(&input_K));
allocate_binary(total_pixels, "uchar", (void *)(&output_C));

/***********************************************************
Read input files
************************************************************/

sprintf(input_K_filename, argv[1]);
if(read_binary(total_pixels, input_K_filename, "sint", (void *)(&input_K))==-1)
{
	printf("\n%s is missing!\n\n", input_K_filename);
	exit(0);
}


/****************************************************************
Looping through images
*****************************************************************/
maxval_K=(maxval+273)*10;
input_K_ptr = input_K;
output_C_ptr = output_C;

for(i=0; i<total_pixels;i++)
    {
		/*check for data flags*/
		if(*input_K_ptr <= 0)
			{
			*output_C_ptr = minval;
			}
		else if(*input_K_ptr >= maxval_K)
			{
			*output_C_ptr = 255;
			}
		else
			{
			/*convert to degrees Celcius*/
			degreesC=(*input_K_ptr)/10-273;
			*output_C_ptr = (unsigned char)((degreesC-minval)*255/maxval);
			}

	input_K_ptr++;
	output_C_ptr++;

	}




 /***********************************************************************
Free memory allocated for inputs
************************************************************************/
free(input_K);

/*************************************************************************
Write images and free memory
*************************************************************************/

sprintf(output_C_filename, argv[2]);
write_binary(total_pixels, output_C_filename, "uchar", (void *)(&output_C));
free(output_C);
}


