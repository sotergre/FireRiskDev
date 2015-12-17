/******************************************************************************************
firerisk_index_nas_v2.c

last update 9/3/2013

Averages the 100 and 1000 litter moisture classes to generate fire risk index.

Command line arguments

<infile LM0100> <infile LM1000> <outfile firerisk> <total pixels>

input is signed 16bit file and output is signed 16 byte file.  

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
int i, lag, total_pixels;
signed short int *input_lm0100, *input_lm0100_ptr, *input_lm1000, *input_lm1000_ptr, *output, *output_ptr;
char input_lm0100_filename[101], input_lm1000_filename[101], output_filename[101];

total_pixels=atoi(argv[4]);

 /** Check args, if incorrect specify usage **/

   if( argc!=5 )
   {
      printf("Usage:\n\n");
      printf("<infile LM0100> <infile LM1000> <outfile firerisk> <total pixels>\n");
      printf("See source code file for command line argument values\n");
      return 1;
   }

/*************************************************************
Memory Allocation
*************************************************************/
allocate_binary(total_pixels, "sint", (void *)(&input_lm0100));
allocate_binary(total_pixels, "sint", (void *)(&input_lm1000));
allocate_binary(total_pixels, "sint", (void *)(&output));

/***********************************************************
Read input files
************************************************************/

sprintf(input_lm0100_filename, argv[1]);
if(read_binary(total_pixels, input_lm0100_filename, "sint", (void *)(&input_lm0100))==-1)
{
	printf("\n%s is missing!\n\n", input_lm0100_filename);
	exit(0);
}

sprintf(input_lm1000_filename, argv[2]);
if(read_binary(total_pixels, input_lm1000_filename, "sint", (void *)(&input_lm1000))==-1)
{
	printf("\n%s is missing!\n\n", input_lm1000_filename);
	exit(0);
}


/****************************************************************
Looping through images
*****************************************************************/
input_lm0100_ptr = input_lm0100;
input_lm1000_ptr = input_lm1000;
output_ptr = output;

for(i=0; i<total_pixels;i++)
    {
		/*check for data flags*/
		if(*input_lm0100_ptr+*input_lm1000_ptr == 0)
			{
			*output_ptr = 0;
			}
		else   {
			*output_ptr = (signed short int)((*input_lm0100_ptr+*input_lm1000_ptr)/2);
			}

	input_lm0100_ptr++;
	input_lm1000_ptr++;
	output_ptr++;

	}




 /***********************************************************************
Free memory allocated for inputs
************************************************************************/
free(input_lm0100);
free(input_lm1000);
/*************************************************************************
Write images and free memory
*************************************************************************/

sprintf(output_filename, argv[3]);
write_binary(total_pixels, output_filename, "sint", (void *)(&output));
free(output);
}

