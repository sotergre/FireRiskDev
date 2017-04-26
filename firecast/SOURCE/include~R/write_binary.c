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
 write_binary.c
 last update 10/16/2001

 Write a binary image.
 Returns the number of bytes written.

 Arguments:
	<numpx> <fname> <dtype> <imptr>

 where
	<numpx> = number of pixels (rows*cols) in the image
        <fname> = name of the file to write the image to. Should
                  not already be open!
	<dtype> = the datatype for the image. One of the following:
                double = double precision floating point
		float = single precision floating point
		int = signed integer
		uint = unsigned integer
		sint = signed short integer
		usint = unsigned short integer
		char = signed very short integer
		uchar = unsigned very short integer
	<imptr> = address of pointer to the image

 Note that sizes in bytes for each data type are machine dependent.

***********************************************************************/

int write_binary(int numpx, char *fname, char *dtype, void **imptr)
{
   int bytes_written;
   int im_fd;
   int numbytes;


   /* Open file with read/write permission. If the file already
      exists it will be truncated to zero bytes and overwritten, 
      otherwise it will be created. */

   im_fd=open(fname,O_RDWR | O_CREAT | O_TRUNC, 0666);
   if( im_fd==-1 )
   {
      printf("Could not open %s for write.\n",fname);
      printf("Make sure you have write permission for the directory.\n");
      printf("Exiting...\n");
      exit(1);
   }


   /* Write according to datatype */

   if( strcmp(dtype,"double")==0 )
   {
      numbytes=numpx*sizeof(double);
      bytes_written=write(im_fd,(double *)(*imptr),numbytes);
   }
   else if( strcmp(dtype,"float")==0 )
   {
      numbytes=numpx*sizeof(float);
      bytes_written=write(im_fd,(float *)(*imptr),numbytes);
   }
   else if( strcmp(dtype,"int")==0 )
   {
      numbytes=numpx*sizeof(signed int);
      bytes_written=write(im_fd,(signed int *)(*imptr),numbytes);
   }
   else if( strcmp(dtype,"uint")==0 )
   {
      numbytes=numpx*sizeof(unsigned int);
      bytes_written=write(im_fd,(unsigned int *)(*imptr),numbytes);
   }
   else if( strcmp(dtype,"sint")==0 )
   {
      numbytes=numpx*sizeof(signed short int);
      bytes_written=write(im_fd,(signed short int *)(*imptr),numbytes);
   }
   else if( strcmp(dtype,"usint")==0 )
   {
      numbytes=numpx*sizeof(unsigned short int);
      bytes_written=write(im_fd,(unsigned short int *)(*imptr),numbytes);
   }
   else if( strcmp(dtype,"char")==0 )
   {
      numbytes=numpx*sizeof(signed char);
      bytes_written=write(im_fd,(signed char *)(*imptr),numbytes);
   }
   else if( strcmp(dtype,"uchar")==0 )
   {
      numbytes=numpx*sizeof(unsigned char);
      bytes_written=write(im_fd,(unsigned char *)(*imptr),numbytes);
   }
   else
   {
      printf("The datatype passed to write_binary.c is invalid.\n");
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
 
  
   /* Check on bytes written */

   if( bytes_written!=numbytes )
   {
      printf("Only %d of %d bytes written to %s.\n",bytes_written,numbytes,
              fname);
      printf("Check that the file should be of type %s\n",dtype); 
      printf("and the number of pixels should be %d.\n",numpx);
      printf("Exiting...\n");
      exit(1);
   }
   printf("Wrote %d bytes to %s.\n",bytes_written,fname);


   /* Return bytes written. */

   return bytes_written;

}
