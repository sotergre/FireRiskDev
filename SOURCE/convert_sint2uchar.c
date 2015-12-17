/******************************************************************************************
convert_sint2uchar.c

last update 2/12/2008

scales a ppt sum and distance to min and max values
Here we use 0 to 50 for daily sums
for distance min=0 max = 500

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
int i, lag, total_pixels, minval, maxval;
signed short int *input, *input_ptr;
unsigned char *output, *output_ptr;
char input_filename[101], output_filename[101];

total_pixels=atoi(argv[3]);
minval=atoi(argv[4]);
maxval=atoi(argv[5]);

 /** Check args, if incorrect specify usage **/

   if( argc!=6 )
   {
      printf("Usage:\n\n");
      printf("<input file> <output file> <total pixels> <min val> <maxval>\n");
      printf("See source code file for command line argument values\n");
      return 1;
   }

/*************************************************************
Memory Allocation
*************************************************************/
allocate_binary(total_pixels, "sint", (void *)(&input));
allocate_binary(total_pixels, "uchar", (void *)(&output));

/***********************************************************
Read input files
************************************************************/

sprintf(input_filename, argv[1]);
if(read_binary(total_pixels, input_filename, "sint", (void *)(&input))==-1)
{
	printf("\n%s is missing!\n\n", input_filename);
	exit(0);
}


/****************************************************************
Looping through images
*****************************************************************/
input_ptr = input;
output_ptr = output;

for(i=0; i<total_pixels;i++)
    {
		/*check for data flags*/
		if(*input_ptr <= 0)
			{
			*output_ptr = minval;
			}
		else if(*input_ptr >= maxval)
			{
			*output_ptr = 255;
			}
		else
			{
			*output_ptr = (unsigned char)((*input_ptr-minval)*255/maxval);
			}

	input_ptr++;
	output_ptr++;

	}




 /***********************************************************************
Free memory allocated for inputs
************************************************************************/
free(input);

/*************************************************************************
Write images and free memory
*************************************************************************/

sprintf(output_filename, argv[2]);
write_binary(total_pixels, output_filename, "uchar", (void *)(&output));
free(output);
}

