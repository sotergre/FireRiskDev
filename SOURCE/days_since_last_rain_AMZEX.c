#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <math.h>


/***************************************************************************
 days_since_last_rain_AMZEX.c

 Karyn Tabor
02/08/2008

 Calculate the number of days since the last rainfall event. The threshold
 for the last rainfall event (in mm) is entered on the command line.

 compile with: gcc -o days_since_last_rain_AMZEX days_since_last_rain_AMZEX.c

 Arguments: year jday thr

    year is the four digit year
    jday is the three digit julian day
    thr is the rainfall threshold in mm (must be an integer)
	Previous Day's since last last file (DSLR) in 8 bit binary.
    Daily rainfall summations in mm file type is 16 bit signed binary.

 Inputs:

    Previous Day's since last last file (DSLR) in 8 bit binary.
    Daily rainfall summations in mm file type is 16 bit signed binary.


    If the Previous Day's DSLR cannot be opened a
    warning will be given and the days since last rainfall will
    be set to zero for all pixels.


 Output:
    DSLR file in 8 bit binary.	The units are number of days since
    the rainfall last exceeded the threshold (thr).

    Command line arguments:
    <previous Day's DSLR file> <current day rainfall summation> <output DSLR> <threshold mm> <nrows> <ncols>

***************************************************************************/


int main( int argc, char **argv )
{

   /* year, jday, thr from command line */
   char prevdslr_fn[101];
   char dslr_fn[101];
   char rainsum_fn[101];
   signed short int thr;
   unsigned int npixels, nrows, ncols;

    /* pointer to rainfall sum file */
   signed short int *rainsum,*rainsum_ptr;

   /* pointer to days since rainfall file for previous Julian day */
   unsigned char *prevdslr,*prevdslr_ptr;

   /* pointer to days since rainfall file for current Julian day */
   unsigned char *dslr,*dslr_ptr;

    /* pixel loop counter */
   unsigned int pixel;

   /* bytes read/written check */
   signed int nbytes;

   /* file descriptors for rain, previous, current days since rain files */
   signed int rainsum_fd,prevdslr_fd,dslr_fd;



     /* read command line arguments */
   sprintf(prevdslr_fn,"%s",argv[1]);
   sprintf(rainsum_fn,"%s",argv[2]);
   sprintf(dslr_fn,"%s",argv[3]);
   thr=(signed short int)atoi(argv[4]);
   nrows=(signed short int)atoi(argv[5]);
   ncols=(signed short int)atoi(argv[6]);

    npixels = nrows*ncols;
     /* allocate memory for rainfall and read the file */

   rainsum=(signed short int *)calloc(npixels,sizeof(signed short int));
   if( rainsum==NULL )
   {
      printf("Error; couldn't allocate memory for %s\n",rainsum_fn);
      exit(1);
   }

   rainsum_fd=open(rainsum_fn,O_RDONLY);
   if( rainsum_fd==-1 )
   {
      printf("Error: couldn't open %s for read\n",rainsum_fn);
      exit(1);
   }

   nbytes=read(rainsum_fd,(signed short int *)rainsum,
          npixels*sizeof(signed short int));
   if( nbytes!=npixels*sizeof(signed short int) )
   {
      printf("Error: only %d of %d bytes read from %s\n",nbytes,
         npixels*sizeof(signed short int),rainsum_fn);
      exit(1);
   }
   close(rainsum_fd);


   /* allocate memory for days since last rain for previous Julian
      day and read the file */


   prevdslr=(unsigned char *)calloc(npixels,sizeof(unsigned char));
   if( prevdslr==NULL )
   {
      printf("Error; couldn't allocate memory for %s\n",prevdslr_fn);
      exit(1);
   }

   prevdslr_fd=open(prevdslr_fn,O_RDONLY);
   if( prevdslr_fd==-1 )
   {
      printf("Error: couldn't open %s for read\n",prevdslr_fn);

      /* Note that calloc has already initialized the memory to zero */
      printf("Setting days since last rain o zero\n");

   }
   else
   {
      nbytes=read(prevdslr_fd,(unsigned char *)prevdslr,
             npixels*sizeof(unsigned char));
      if( nbytes!=npixels*sizeof(unsigned char) )
      {
         printf("Error: only %d of %d bytes read from %s\n",nbytes,
            npixels*sizeof(unsigned char),prevdslr_fn);
         exit(1);
      }
      close(prevdslr_fd);
   }


   /* allocate memory for days since last rain for current Julian day */

   dslr=(unsigned char *)calloc(npixels,sizeof(unsigned char));
   if( dslr==NULL )
   {
      printf("Error; couldn't allocate memory for %s\n",dslr_fn);
      exit(1);
   }


   /* pixels loop */

   rainsum_ptr=rainsum;
   prevdslr_ptr=prevdslr;
   dslr_ptr=dslr;

   for( pixel=0; pixel<npixels; pixel++ )
   {
      /* If the rainfall for the current day exceeds the threshold
         then set days since rain to zero */

      if( *rainsum_ptr>thr )
      {
         *dslr_ptr = 0;
      }

      /* Otherwise set days since rain to one plus the previous days
         since rain */

      else
      {
         *dslr_ptr = *prevdslr_ptr + 1;
         if(*dslr_ptr>200){
            *dslr_ptr=200;}
      }


      rainsum_ptr++;
      prevdslr_ptr++;
      dslr_ptr++;
   }


   /* Free memory allocated for inputs */

   free(rainsum);
   free(prevdslr);


   /* Write output file and free memory */

   dslr_fd=open(dslr_fn,O_RDWR | O_CREAT);
   if( dslr_fd==-1 )
   {
      printf("Error: couldn't open %s for write\n",dslr_fn);
      exit(1);
   }

   nbytes=write(dslr_fd,(unsigned char *)dslr,
          npixels*sizeof(unsigned char));
   if( nbytes!=npixels*sizeof(unsigned char) )
   {
      printf("Error: only %d of %d bytes written to %s\n",nbytes,
         npixels*sizeof(unsigned char),dslr_fn);
      exit(1);
   }
   fchmod(dslr_fd,0666);

   close(dslr_fd);
   free(dslr);

   return 0;

}
