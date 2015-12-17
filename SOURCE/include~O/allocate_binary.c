#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <math.h>
#include <malloc/malloc.h>
#include <sys/uio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAX_NUM_STRINGS 500

/***********************************************************************
 allocate_binary.c
 last update 8/28/2002

 C function, allocates memory for a binary image.


 prototype:

   int allocate_binary(int numpx, char *dtype, void **imptr)


 arguments:

   numpx = number of pixels (rows*cols) to allocate for the image
   dtype = the datatype for the image. One of the following:
                double = double precision floating point
                float = single precision floating point
		int = signed integer
		uint = unsigned integer
		sint = signed short integer
		usint = unsigned short integer
		char = signed very short integer
		uchar = unsigned very short integer
   imptr = address of pointer to datatype at which memory will
	   be allocated. Must be cast to type (void *).


 return value:

   returns 0 if memory allocation was successful, -1 otherwise

***********************************************************************/

int allocate_binary(int numpx, char *dtype, void **imptr)
{
   int numbytes;

   /* allocate according to dtype */

   if( strcmp(dtype,"double")==0 )
   {
      *imptr=(double *)calloc(numpx,sizeof(double));
      numbytes=numpx*sizeof(double);
   }
   else if( strcmp(dtype,"float")==0 )
   {
      *imptr=(float *)calloc(numpx,sizeof(float));
      numbytes=numpx*sizeof(float);
   }
   else if( strcmp(dtype,"int")==0 )
   {
      *imptr=(signed int *)calloc(numpx,sizeof(signed int));
      numbytes=numpx*sizeof(signed int);
   }
   else if( strcmp(dtype,"uint")==0 )
   {
      *imptr=(unsigned int *)calloc(numpx,sizeof(unsigned int));
      numbytes=numpx*sizeof(unsigned int);
   }
   else if( strcmp(dtype,"sint")==0 )
   {
      *imptr=(signed short int *)calloc(numpx,sizeof(signed short int));
      numbytes=numpx*sizeof(signed short int);
   }
   else if( strcmp(dtype,"usint")==0 )
   {
      *imptr=(unsigned short int *)calloc(numpx,sizeof(unsigned short int));
      numbytes=numpx*sizeof(unsigned short int);
   }
   else if( strcmp(dtype,"char")==0 )
   {
      *imptr=(signed char *)calloc(numpx,sizeof(signed char));
      numbytes=numpx*sizeof(signed char);
   }
   else if( strcmp(dtype,"uchar")==0 )
   {
      *imptr=(unsigned char *)calloc(numpx,sizeof(unsigned char));
      numbytes=numpx*sizeof(unsigned char);
   }
   else
   {
      printf("The datatype passed to allocate_binary.c is invalid.\n");
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
      return -1;
   }


   /* Check that memory was allocated */

   if( *imptr==NULL )
   {
      printf("Memory allocation failed!\n");
      printf("Exiting...\n");
      return -1;
   }
   printf("Allocated %d bytes of memory\n",numbytes);
   return 0;

}
