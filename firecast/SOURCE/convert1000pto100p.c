/******************************************************************************************
convert1000pto100p.c

last update 2/14/2008

converts a percent files at 1000% to 100% and converts from signed 16bit to unsigned 8bit

RH min =20 max =100

Command line arguments

<infile> <outfile> <total pixels> <min val> <max val>

input is signed 16bit file and output is unsigned 8bit file.  All negative numbers are set to 0
output=(input-min val)*255/(max val)
set no data flag as min val

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
int i, lag, total_pixels, minval, maxval;
signed short int *input_16, *input_16_ptr;
unsigned char *output_8, *output_8_ptr;
char input_16_filename[101], output_8_filename[101];

total_pixels=atoi(argv[3]);
minval=atoi(argv[4]);
maxval=atoi(argv[5]);


 /** Check args, if incorrect specify usage **/

   if( argc!=6 )
   {
      printf("Usage:\n\n");
      printf("<input sint file> <output uchar file> <total pixels> <min val> <max val>\n");
      printf("See source code file for command line argument values\n");
      return 1;
   }

/*************************************************************
Memory Allocation
*************************************************************/
allocate_binary(total_pixels, "sint", (void *)(&input_16));
allocate_binary(total_pixels, "uchar", (void *)(&output_8));

/***********************************************************
Read input files
************************************************************/

sprintf(input_16_filename, argv[1]);
if(read_binary(total_pixels, input_16_filename, "sint", (void *)(&input_16))==-1)
{
	printf("\n%s is missing!\n\n", input_16_filename);
	exit(0);
}


/****************************************************************
Looping through images
*****************************************************************/

input_16_ptr = input_16;
output_8_ptr = output_8;

for(i=0; i<total_pixels;i++)
    {
		/*check for data flags*/
		if(*input_16_ptr <= 0)
			{
			*output_8_ptr = minval;
			}
		else if(*input_16_ptr >= maxval*10)
			{
			*output_8_ptr = 255;
			}
		else
			{
			/*convert to 100% from 1000% and scale*/
			*output_8_ptr = (unsigned char)((*input_16_ptr/10-minval)*255/maxval);
			}

	input_16_ptr++;
	output_8_ptr++;

	}




 /***********************************************************************
Free memory allocated for inputs
************************************************************************/
free(input_16);

/*************************************************************************
Write images and free memory
*************************************************************************/

sprintf(output_8_filename, argv[2]);
write_binary(total_pixels, output_8_filename, "uchar", (void *)(&output_8));
free(output_8);
}

