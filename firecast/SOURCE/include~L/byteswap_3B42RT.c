#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <math.h>


/***************************************************************************
 byteswap_3B42RT.c

 Jen Small 9/17/2003

 Byteswap the TRMM 3B42RT data.

 Arguments: signed int *fd, char *big_end, char *little_end, 
   unsigned int *npix

 fd is a pointer to the file descriptor for the open file to be swapped.
 big_end is a pointer to allocated memory for the file in big endian 
   byte order.
 little end is a pointer to allocated memory for the file in little
   endian byte order (swapped).
 npix is the number of pixels (rows*columns) in the input file.

 Reads the file with file descriptor fd into big_end buffer, byteswaps
   to little_end buffer, and writes swapped file to a temporary file 
   'tmpfile'.

 Returns -1 if the swap failed, 0 otherwise.

***************************************************************************/

int byteswap_3B42RT(signed int *fd, char *big_end, char *little_end, 
   unsigned int *npix)
{

   /* the number of bytes in each swap buffer */
   unsigned int nbytes; 

   /* pixel loop counter */
   unsigned int pix;

   /* number of bytes read from input file */
   signed int bytes_read;

   /* first and second byte of each 2 byte signed short integer */
   char byte1,byte2;

   /* pointers to big and little endian buffers */
   char *big_ptr,*little_ptr;



   /* number of bytes in each swap buffer is 2 (=sizeof(signed short int))
      times the number of pixels */
   nbytes = (*npix)*2;


   /* read input file into big endian buffer */

   bytes_read = read(*fd,(char *)big_end,nbytes);
   if( bytes_read!=nbytes )
   {
      printf("Error: only %d of %d bytes read to swap buffer\n",
         bytes_read,nbytes);
      return -1;
   }


   /* loop through buffer and swap bytes */

   big_ptr= big_end;
   little_ptr= little_end;

   for( pix=0; pix<(*npix); pix++ )
   {

      byte1 = *big_ptr;
      byte2 = *(big_ptr+1);

      *little_ptr = byte2;
      *(little_ptr+1) = byte1;

      big_ptr+=2;
      little_ptr+=2;
   }


   /* write swapped data to file 'tmpfile' */
   
   if( write_binary(nbytes,"tmpfile","char",(void *)(&little_end))==-1 )
   {
      printf("Error: couldn't write swapped data to tmpfile\n");
      return -1;
   }


   return 0;

}  
