/*
rescale_dslr.c

last update 2/12/2008

scales days since last rain fall to max number of days indicated

ie: max = 60

Command line arguments

<infile> <outfile> <total pixels> <max val>

input is unsigned 8bit file and output is unsigned 8bit file.  All negative numbers are set to 0
output=input*255/(max val)
set no data flag as min val

******************************************************************************************/

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
unsigned char *input, *input_ptr;
unsigned char *rescale, *rescale_ptr;
char input_filename[101], rescale_filename[101];

total_pixels=atoi(argv[3]);
maxval=atoi(argv[4]);


 /** Check args, if incorrect specify usage **/

   if( argc!=5 )
   {
      printf("Usage:\n\n");
      printf("<input file> <output rescale file> <total pixels> <max val>\n");
      printf("See source code file for command line argument values\n");
      return 1;
   }

/*************************************************************
Memory Allocation
*************************************************************/
allocate_binary(total_pixels, "uchar", (void *)(&input));
allocate_binary(total_pixels, "uchar", (void *)(&rescale));

/***********************************************************
Read input files
************************************************************/

sprintf(input_filename, argv[1]);
if(read_binary(total_pixels, input_filename, "uchar", (void *)(&input))==-1)
{
	printf("\n%s is missing!\n\n", input_filename);
	exit(0);
}


/****************************************************************
Looping through images
*****************************************************************/

input_ptr = input;
rescale_ptr = rescale;

for(i=0; i<total_pixels;i++)
    {
		/*check for data flags*/
		if(*input_ptr <= 0)
			{
			*rescale_ptr = 0;
			}
		else if(*input_ptr >= maxval)
			{
			*rescale_ptr = 255;
			}
		else
			{
			/*scale*/
			*rescale_ptr = (unsigned char)((*input_ptr)*255/maxval);
			}

	input_ptr++;
	rescale_ptr++;

	}




 /***********************************************************************
Free memory allocated for inputs
************************************************************************/
free(input);

/*************************************************************************
Write images and free memory
*************************************************************************/

sprintf(rescale_filename, argv[2]);
write_binary(total_pixels, rescale_filename, "uchar", (void *)(&rescale));
free(rescale);
}


