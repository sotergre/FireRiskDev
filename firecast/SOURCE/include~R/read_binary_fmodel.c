#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <math.h>
#include <malloc.h>
#include <sys/uio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAX_NUM_STRINGS 500

/***********************************************************************
 read_binary_fmodel.c
 last update 10/9/2001

 Read a binary image. Returns the number of bytes read.

 Arguments:
	<numpx> <fname> <dtype> <imptr>

 where
	<numpx> = number of pixels (rows*cols) in the image
        <fname> = the filename for the image.
	<dtype> = the datatype for the image. One of the following:
                double = double precision floating point
		float = single precision floating point
		int = signed integer
		uint = unsigned integer
		sint = signed short integer
		usint = unsigned short integer
		char = signed very short integer
		uchar = unsigned very short integer
	<imptr> = address of pointer to the memory allocated for the image.

 Note that sizes in bytes for each data type are machine dependent.

***********************************************************************/

int read_binary_fmodel(int numpx, char *fname, char *dtype, void **imptr)
{
   int bytes_read;
   int im_fd;
   int numbytes;

   /* Open file with read only permission. */

   im_fd=open(fname,O_RDONLY);
   if( im_fd==-1 )
   {
      printf("\nFile does not exist\n\n",fname);
	   return -1;
   }


	else
	{
   /* Read according to datatype */

   if( strcmp(dtype,"double")==0 )
   {
      numbytes=numpx*sizeof(double);
      bytes_read=read(im_fd,(double *)(*imptr),numbytes);
   }
   else if( strcmp(dtype,"float")==0 )
   {
      numbytes=numpx*sizeof(float);
      bytes_read=read(im_fd,(float *)(*imptr),numbytes);
   }
   else if( strcmp(dtype,"int")==0 )
   {
      numbytes=numpx*sizeof(signed int);
      bytes_read=read(im_fd,(signed int *)(*imptr),numbytes);
   }
   else if( strcmp(dtype,"uint")==0 )
   {
      numbytes=numpx*sizeof(unsigned int);
      bytes_read=read(im_fd,(unsigned int *)(*imptr),numbytes);
   }
   else if( strcmp(dtype,"sint")==0 )
   {
      numbytes=numpx*sizeof(signed short int);
      bytes_read=read(im_fd,(signed short int *)(*imptr),numbytes);
   }
   else if( strcmp(dtype,"usint")==0 )
   {
      numbytes=numpx*sizeof(unsigned short int);
      bytes_read=read(im_fd,(unsigned short int *)(*imptr),numbytes);
   }
   else if( strcmp(dtype,"char")==0 )
   {
      numbytes=numpx*sizeof(signed char);
      bytes_read=read(im_fd,(signed char *)(*imptr),numbytes);
   }
   else if( strcmp(dtype,"uchar")==0 )
   {
      numbytes=numpx*sizeof(unsigned char);
      bytes_read=read(im_fd,(unsigned char *)(*imptr),numbytes);
   }
   else
   {
      printf("The datatype passed to read_binary.c is invalid.\n");
      printf("Valid data types are:\n");
      printf("\tdouble = double precision floating point\n");
      printf("\tfloat = single precision floating point\n");
      printf("\tint = signed integer\n");
      printf("\tuint = unsigned integer\n");
      printf("\tsint = signed short integer\n");
      printf("\tusint = unsigned short integer\n");
      printf("\tchar = signed very short integer\n");
      printf("\tuchar = unsigned very short integer\n");
      printf("Exiting...\n");
      exit(1);
   }


   /* Close file */

   close(im_fd);


   /* Check on bytes read */

   if( bytes_read!=numbytes )
   {
      printf("Only %d of %d bytes read from %s.\n",bytes_read,numbytes,fname);
      printf("Check file is of type %s and number of pixels is %d.\n",
              dtype,numpx);
      printf("Exiting...\n");
      exit(1);
   }
   printf("Read %d bytes from %s.\n",bytes_read,fname);


   /* Return number of bytes read. */

  return bytes_read;
	}
}
