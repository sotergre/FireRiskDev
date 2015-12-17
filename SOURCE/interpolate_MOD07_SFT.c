#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <math.h>
#include <malloc.h>
#include <sys/uio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/**************************************************************************
 interpolate_MOD07_SFT.c
 Jen Small
 ed Karyn Tabor
 last update 03/25/2008

 Distance-weighted interpolation for 16 bit unsigned binary files.

 This program uses distance-weighted interpolation to fill in flagged
 values in a 16 bit unsigned binary image. A missing value
 is defined as a flagged value occurring underneath the input mask.

 When a missing value is found, a search for nonflagged neighboring
 pixels is done, with 8 closest pixels (in terms of Euclidean distance)
 searched first. The distance-weighted average of the four highest values of the eight closest
 pixels replaces the missing value. Search is done in 8 directions
 N,S,E,W,NE,SE,NW,SW for a maximum of maxinc_8dir increments.
 For a missing value at row=r, col=c, the search
 locations are:

      (r-x,c) (N)
      (r+x,c) (S)
      (r,c+x) (E)
      (r,c-x) (W)
      (r-x,c+x) (NE)
      (r-x,c-x) (NW)
      (r+x,c+x) (SE)
      (r+x,c-x) (SW)

Once a pixel is found in a given direction that direction will not be searched again.


If 8 pixels can not be found within a distance of maxinc_8dir pixels from the orignial missing pixel, the serach then begins in 16 directions.
For a missing value at row=r, col=c, the search locations are:

      (r-x,c) (N)
      (r+x,c) (S)
      (r,c+x) (E)
      (r,c-x) (W)
      (r-x,c+x) (NE)
      (r-x,c-x) (NW)
      (r+x,c+x) (SE)
      (r+x,c-x) (SW)
      (r-x, c+ceil(x/2)) (NNE)
      (r-ceil(x/2), c+x) (ENE)
      (r-x, c-ceil(x/2)) (NNW)
      (r-ceil(x/2), c-x) (WNW)
      (r+x, c+ceil(x/2)) (SSE)
      (r+ceil(x/2), c+x) (ESE)
      (r+x, c-ceil(x/2)) (SSW)
      (r+ceil(x/2), c-x) (WSW)


If 8 pixels can not be found within a istance of maxinc_16dir pixels from the orignial missing pixel, the serach then begins in 32 directions.  If 8 pixels can not be found within maxinc_32dir pixels the output remains flagged and the output distance file is aslo flagged with a value of -1.
For a missing value at row=r, col=c, the search locations are:

      (r-x,c) (N)
      (r+x,c) (S)
      (r,c+x) (E)
      (r,c-x) (W)
      (r-x,c+x) (NE)
      (r-x,c-x) (NW)
      (r+x,c+x) (SE)
      (r+x,c-x) (SW)
      (r-x,c+ceil(x/2)) (NNE)
      (r-ceil(x/2),c+x) (ENE)
      (r-x,c-ceil(x/2)) (NNW)
      (r-ceil(x/2),c-x) (WNW)
      (r+x,c+ceil(x/2)) (SSE)
      (r+ceil(x/2),c+x) (ESE)
      (r+x,c-ceil(x/2)) (SSW)
      (r+ceil(x/2),c-x) (WSW)
      (r-x,c+ceil(x/4)) (NNNE)
      (r-x,c+ceil(3*x/4)) (ENNE)
      (r-ceil(x/4),c+x) (EENE)
      (r-ceil(3*x/4),c+x) (NENE)
	  (r-x,c-ceil(x/4)) (NNNW)
	  (r-x,c-ceil(3*x/4)) (WNNW)
	  (r-ceil(x/4),c-x) (WWNW)
	  (r-ceil(3*x/4),c-x) (NWNW)
	  (r+c,r+ceil(x/4)) (SSSE)
	  (r+c,r+ceil(3*x/4)) (ESSE)
	  (r+ceil(x/4),c+x) (EESE)
	  (r+ceil(3*x/4),c+x) (SESE)
	  (r+x,c-ceil(x/4)) (SSSW)
	  (r+x,c-ceil(3*x/4)) (WSSW)
	  (r+ceil(x/4),c-x) (WWSW)
	  (r+ceil(3*x/4),c-x) (SWSW)


 Once 8 pixels are found, the four minimum pixel values are ignored to eliminate cloud edge errors and the interpolation is preformed on the 4 maximum values.

 Inputs:

	One 8 bit unsigned raw binary mask, with pixels to interpolate
        nonzero and pixels that should be flagged in the output (for
        example, water pixels) set to zero.

	One raw binary float image to be interpolated.

	The mask and input image must be coregistered and have the
        same number of rows and columns.


 Output:

	One interpolated raw binary 16 bit unsigned image.
	One raw binary 16 bit unsigned image containing the mean
	distance for interpolation for interpolated pixels (rounded
 	to the nearest integer) and zero for uninterpolated pixels.


 Compiles with:
 "gcc -g -o interpolate_32dir_16U_8pix interpolate_32dir_16U_8pix.c -lm"

 Must have functions allocate_binary.c, read_binary.c, and write_binary.c
 in the same directory for compiling.


 Command line arguments:

	<maskfn> <infn> <outfn> <distfn> <ncols> <nrows> <flagval> <maxinc_8dir> <maxinc_16dir> <maxinc_32dir>

 where:
	<maskfn> = filename for the land mask file
	<infn> = filename for the input file (to be interpolated)
 	<outfn> = filename for the output (interpolated) file
       <distfn> = filename for the mean distance file
	<ncols> = number of columns in input file
	<nrows> = number of rows in input file
	<flagval> = flag value in the input file
       <maxinc_8dir> = the maximum number of pixels from the flag pixel to search in 8 directions
	<maxinc_16dir> = the maximum number of pixels from the flag pixel to search in 16 directions
	<maxinc_32dir> = the maximum number of pixels from the flag pixel to search in 32 directions

 Filenames can have no more than 100 characters.

**************************************************************************/



int main(int argc, char *argv[])
{


/***********************************************************************
    Command line arguments

***********************************************************************/

   char maskfn[101],infn[101],outfn[101],distfn[101];
   int ncols,nrows,total_pixels;
   signed short int flag;
   int maxinc_8dir;
   int maxinc_16dir;
   int maxinc_32dir;


   /* The calculated fill value using distance weighted averaging */

   float dwavg;
   float frac_part;
   float tempvalue;
   float tempindex;

   /* The 32 directions: 0(N),1(S),2(E),3(W),4(NE),5(NW),6(SE),7(SW),8(NNE),9(ENE),10(NNW),11(WNW),12(SSE),13(ESE),14(SSW),15(WSW),16(NNNE),17(ENNE),18(EENE),19(NENE),20(NNNW),21(WNNW),22(WWNW),23(NWNW),24(SSSE),25(ESSE),26(EESE),27(SESE),28(SSSW),29(WSSW),30(WWSW),31(SWSW)*/
   float directions[32][2]={{-1,0},{1,0},{0,1},{0,-1},{-1,1},{-1,-1},{1,1},{1,-1},{-1,0.5},{-0.5,1},{-1,-0.5},{-0.5,-1},{1,0.5},{0.5,1},{1,-0.5},{0.5,-1},{-1,0.25},{-1,0.75},{-0.25,1},{-0.75,1},{-1,-0.25},{-1,-0.75},{-0.25,-1},{-0.75,-1},{1,0.25},{1,0.75},{0.25,1},{0.75,1},{1,-0.25},{1,-0.75},{0.25,-1},{0.75,-1}};


   /* If foundflag[i]=1 then a pixel was found in direction i */
   int foundflag[32];


   /* distances from missing pixel to search pixel in each direction */
   float distances[32];

   /* offset in pixels from UL corner of image to search pixel in each direction */
   int offsets[32];


   /* The values and the distances from the missing pixel for each of the four fill pixels
      used in the interpolation */
   float fillvals[8],filldist[8];

   /*array of found pixel values and locations from found flag array*/
   float maxarray[8][2];


   /* the sum of the fill distances */
   float dsum;


   /* weight to apply to each fill value in the distance weighted average */
   float weight[8];


   /* pointers to input and output files */
   unsigned char *mask;
   float *infile;
   signed short int *outfile, *distfile;


   /* row, column loop counters */

   register int r,c;


   /* count pixels found so far in search */

   int pixfound=0;

   /*number of pixels (up to 8) to use in distance weighted averages.  For example the (numpix) maximum values of the found pixels are average.*/
   int numpix = 4;


   /* increment at which to search ('x' in the outline above) */
   int increment;

   /* loop counter */
   int countflag=0;
   int i, j, k;
   float threshold;

   /* row and column location of the search pixel */
   int searchrow,searchcol;



   /** Check args, if incorrect specify usage **/

   if( argc!=11 )
   {
      printf("Usage:\n\n");
      printf("interpolate_binary_16U maskfn infn outfn distfn ncols ");
      printf("nrows flagval maxinc_8dir maxinc_16dir maxinc_32dir\n\n");
      printf("See source code file for command line argument values\n");
      return 1;
   }



   /********************************************************************
    read command line arguments
   ********************************************************************/

   printf("Running distance-weighted interpolation\n\n");

   sprintf(maskfn,argv[1]);
   printf("The mask file is %s\n",maskfn);

   sprintf(infn,argv[2]);
   printf("The input file is %s\n",infn);

   sprintf(outfn,argv[3]);
   printf("The output file is %s\n",outfn);

   sprintf(distfn,argv[4]);
   printf("The mean distance file is %s\n",distfn);


   ncols=atoi(argv[5]);
   nrows=atoi(argv[6]);
   printf("Input image has %d columns and %d rows\n",ncols,nrows);
   total_pixels= ncols*nrows;
   threshold=total_pixels*0.99;
   
   flag=atoi(argv[7]);
   printf("The new flag value is %d\n",flag);

   maxinc_8dir=atoi(argv[8]);
   printf("Searching a maximum of %d pixels in 8 directions\n",maxinc_8dir);

   maxinc_16dir=atoi(argv[9]);
   printf("Searching a maximum of %d pixels in 16 directions\n",maxinc_16dir);

   maxinc_32dir=atoi(argv[10]);
   printf("Searching a maximum of %d pixels in 32 directions\n",maxinc_32dir);

   /**********************************************************************
    Memory allocation
   **********************************************************************/

   allocate_binary(total_pixels,"uchar",(void *)(&mask));
   allocate_binary(total_pixels,"float",(void *)(&infile));
   allocate_binary(total_pixels,"sint",(void *)(&outfile));
   allocate_binary(total_pixels,"sint",(void *)(&distfile));


   /**********************************************************************
    Read mask and input file
   **********************************************************************/

   read_binary(total_pixels,maskfn,"uchar",(void *)(&mask));
   read_binary(total_pixels,infn,"float",(void *)(&infile));

  /*******************************************************************
   change 0 pixels to flag values
   *******************************************************************/

  for(r=0; r<nrows; r++)
   {
	for(c=0; c<ncols; c++)
      {

    	if( *(infile+(r*ncols)+c) <= 0)
         {
            *(infile+(r*ncols)+c)= flag;
		countflag++;
         }


	}
   }
/*exit program if there is less then 99% if the image to interpolate from*/
	if (countflag >= threshold)
	{	
	printf("Less the 99 percent of image is nodata. Exiting....\n");
	exit(0);
	}

   /**********************************************************************
    Pixels loop
   **********************************************************************/

   for(r=0; r<nrows; r++)
   {
	for(c= 0; c<ncols; c++)
      {
	
         /*******************************************************************
          flag pixels not under the mask in the output file
         *******************************************************************/

    	if( *(mask+(r*ncols)+c)==0)
         {
            *(outfile+(r*ncols)+c)= flag;
         }


         /*******************************************************************
          If input has a missing value under the mask then interpolate
          or if the cloud mask is not "confident clear"

         *******************************************************************/

         else if( *(infile+(r*ncols)+c)==flag)
         {
            pixfound=0;
            increment=1;
            for (k=0; k<32; k++)
			{
               distances[k]=0;
               offsets[k]=0;
               foundflag[k]=0;
            }
            for( k=0; k<8; k++ )
            {
               fillvals[k]=0;
               filldist[k]=0;
            }

            while( pixfound<8 && increment<=maxinc_8dir )
            {

               /**********************************************************************
                For each increment x there are up to eight pixels to
                search:

                (r-x,c) (N)
                (r+x,c) (S)
                (r,c+x) (E)
                (r,c-x) (W)
                (r-x,c+x) (NE)
                (r-x,c-x) (NW)
                (r+x,c+x) (SE)
                (r+x,c-x) (SW)

                A pixel (row,col) will not be searched if:
                   row<0 or row>nrows
                   col<0 or col>ncols
                   a pixel has already been found in that direction (foundflag[DIR]=1)
               ***********************************************************************/


               for( k=0; k<8; k++ )
               {
                  /* Search this direction if a pixel was not already found here and if
                    less than four pixels have been found */
                 	if( foundflag[k]!=1 && pixfound<8 )
                	{
                 	    /* Calculate row and column for the search pixel and check for
                  	      out of image bounds */

                  	   searchrow = r + increment*directions[k][0];
                  	   searchcol = c + increment*directions[k][1];

                   	  	if( searchrow>=0 && searchcol>=0 && searchrow<nrows && searchcol<ncols )
                   	  	{
                    	 /* Calculate offset in pixels from the UL corner of the image */
                     	 offsets[k] = searchrow*ncols + searchcol;

                      	 /* Calculate distance (Euclidean) from the missing pixel */
                       	 distances[k] = sqrt( pow((r-searchrow),2.0) + pow((c-searchcol),2.0) );

                         /* Check values at each offset and populate the fill values and fill
                          distances array with unflagged values and their distances */
			    		 	if( *(infile+offsets[k])!=flag)
                	   	   	{
                         	 fillvals[pixfound]= *(infile+offsets[k]);
                          	 filldist[pixfound] = distances[k];
                          	 foundflag[k]=1;
                          	 pixfound++;
                	     	}
                     	}
					}
              	} /*close for k loop*/

               increment++;
            } /*close while loop*/


            /* If 8 pixels can not be found within the maxincrement1 start searching in 16 directions*/
            while( pixfound<8 && increment<=maxinc_16dir )
            {
 	          /**********************************************************************
	          For each increment x there are up to sixteen pixels to
	          search:

	         (r-x,c) (N)
	         (r+x,c) (S)
	         (r,c+x) (E)
	         (r,c-x) (W)
	         (r-x,c+x) (NE)
	         (r-x,c-x) (NW)
	         (r+x,c+x) (SE)
	         (r+x,c-x) (SW)
	         (r-x, c+ceil(x/2)) (NNE)
			 (r-ceil(x/2), c+x) (ENE)
			 (r-x, c-ceil(x/2)) (NNW)
			 (r-ceil(x/2), c-x) (WNW)
			 (r+x, c+ceil(x/2)) (SSE)
			 (r+ceil(x/2), c+x) (ESE)
			 (r+x, c-ceil(x/2)) (SSW)
			 (r+ceil(x/2), c-x) (WSW)

             A pixel (row,col) will not be searched if:
	         row<0 or row>nrows
	         col<0 or col>ncols
	         a pixel has already been found in that direction (foundflag[DIR]=1)
	         ***********************************************************************/


			for( k=0; k<16; k++ )
			   {
			    /* Search this direction if a pixel was not already found here and if less than four pixels have been found */
			     if( foundflag[k]!=1 && pixfound<8 )
			       {
			        /* Calculate row and column for the search pixel and check for out of image bounds */
           			searchrow = r + ceil(increment*directions[k][0]);
               	    searchcol = c + ceil(increment*directions[k][1]);

               	  	if( searchrow>=0 && searchcol>=0 && searchrow<nrows && searchcol<ncols )
                 	  	{
                      	 /* Calculate offset in pixels from the UL corner of the image */
                       	 offsets[k] = searchrow*ncols + searchcol;

                       	 /* Calculate distance (Euclidean) from the missing pixel */
                       	 distances[k] = sqrt( pow((r-searchrow),2.0) + pow((c-searchcol),2.0) );

                          /* Check values at each offset and populate the fill values and fill
                            distances array with unflagged values and their distances */
  			    		 	if( *(infile+offsets[k])!=flag)
                   	   	   	{
                           	 fillvals[pixfound]= *(infile+offsets[k]);
                           	 filldist[pixfound] = distances[k];
                           	 foundflag[k]=1;
                           	 pixfound++;
                   	     	}
                       	}
   					}
               	} /*close for k loop*/

               increment++;

            } /*close while loop*/

 			/* If 8 pixels can not be found within the maxincrement2 start searching in 32 directions until maxincrement is readched then keep the pixel flagged*/
            while( pixfound<8 && increment <= maxinc_32dir)
            {
 	          /**********************************************************************
	          For each increment x there are up to sixteen pixels to
	          search:

 		     (r-x,c) (N)
 		     (r+x,c) (S)
 		     (r,c+x) (E)
 		     (r,c-x) (W)
 		     (r-x,c+x) (NE)
 		     (r-x,c-x) (NW)
 		     (r+x,c+x) (SE)
 		     (r+x,c-x) (SW)
 		     (r-x,c+ceil(x/2)) (NNE)
 		     (r-ceil(x/2),c+x) (ENE)
 		     (r-x,c-ceil(x/2)) (NNW)
 		     (r-ceil(x/2),c-x) (WNW)
 		     (r+x,c+ceil(x/2)) (SSE)
 		     (r+ceil(x/2),c+x) (ESE)
 		     (r+x,c-ceil(x/2)) (SSW)
 		     (r+ceil(x/2),c-x) (WSW)
 		     (r-x,c+ceil(x/4)) (NNNE)
 		     (r-x,c+ceil(3*x/4)) (ENNE)
 		     (r-ceil(x/4),c+x) (EENE)
 		     (r-ceil(3*x/4),c+x) (NENE)
			  (r-x,c-ceil(x/4)) (NNNW)
			  (r-x,c-ceil(3*x/4)) (WNNW)
			  (r-ceil(x/4),c-x) (WWNW)
			  (r-ceil(3*x/4),c-x) (NWNW)
			  (r+c,r+ceil(x/4)) (SSSE)
			  (r+c,r+ceil(3*x/4)) (ESSE)
			  (r+ceil(x/4),c+x) (EESE)
			  (r+ceil(3*x/4),c+x) (SESE)
			  (r+x,c-ceil(x/4)) (SSSW)
			  (r+x,c-ceil(3*x/4)) (WSSW)
			  (r+ceil(x/4),c-x) (WWSW)
			  (r+ceil(3*x/4),c-x) (SWSW)


             A pixel (row,col) will not be searched if:
	         row<0 or row>nrows
	         col<0 or col>ncols
	         a pixel has already been found in that direction (foundflag[DIR]=1)
	         ***********************************************************************/


			for( k=0; k<32; k++ )
			   {
			    /* Search this direction if a pixel was not already found here and if less than four pixels have been found */
			     if( foundflag[k]!=1 && pixfound<8 )
			       {
			        /* Calculate row and column for the search pixel and check for out of image bounds */
           			searchrow = r + ceil(increment*directions[k][0]);
               	    searchcol = c + ceil(increment*directions[k][1]);

               	  	if( searchrow>=0 && searchcol>=0 && searchrow<nrows && searchcol<ncols )
                 	  	{
                      	 /* Calculate offset in pixels from the UL corner of the image */
                       	 offsets[k] = searchrow*ncols + searchcol;

                       	 /* Calculate distance (Euclidean) from the missing pixel */
                       	 distances[k] = sqrt( pow((r-searchrow),2.0) + pow((c-searchcol),2.0) );

                          /* Check values at each offset and populate the fill values and fill
                            distances array with unflagged values and their distances */
  			    		 	if( *(infile+offsets[k])!=flag)
                   	   	   	{
                           	 fillvals[pixfound]= *(infile+offsets[k]);
                           	 filldist[pixfound] = distances[k];
                           	 foundflag[k]=1;
                           	 pixfound++;
                   	     	}
                       	}
   					}
               	} /*close for k loop*/

               increment++;

            } /*close while loop*/

            /* If no pixels cannot be found within the max_inc32 write a flag value to the outputfile and a -1 values to the distance file */
            if( increment >= maxinc_32dir && pixfound == 0)
            {
				*(outfile+(r*ncols)+c) = -1;
				*(distfile+(r*ncols)+c)=maxinc_32dir;
             }

            else if( pixfound==1 ) /*only one pixel is found*/
	           		{
	           		  dwavg = fillvals[0];
	           		  dsum = filldist[0];
           			}
           	else if (pixfound > 1 && pixfound < numpix) /*less then "numpix" pixels are found*/
           	{
			dsum=0.0;
				for( i=0; i<pixfound; i++ )
				   {
				    dsum += filldist[i];
				   }
				dwavg=0.0;
				for( i=0; i<pixfound; i++ )
				   {
				    weight[i]= (1.0 - (filldist[i]/dsum))/(float)(pixfound-1);
				    dwavg += (weight[i]*fillvals[i]);
              	   }
	            	frac_part = dwavg - (int)(dwavg);
	            	if( frac_part>=0.5 )
	        	   	{
	           	   *(outfile+(r*ncols)+c) = 10*(unsigned short int)dwavg + 1;
	        	   	}
	        	   	else
	            	{
	            	   *(outfile+(r*ncols)+c) = 10*(unsigned short int)dwavg;
	            	}


	            	/* Calculate mean distance and round to nearest integer */

	            	frac_part = dsum/(float)numpix - (int)(dsum/(float)numpix);

	            	if( frac_part>=0.5 )
	            	{
	            	   *(distfile+(r*ncols)+c)=(signed short int)(10*(dsum/(float)pixfound)+1);
	            	}
	            	else
	            	{
	            	   *(distfile+(r*ncols)+c) = (signed short int)(10*(dsum/(float)pixfound));
            	}
		}

   			else /*if between numpix and 8 pixels have been found, use maximum numpix of 8 values.*/
           	{
				for (i=0; i<8;i++)
				{
				maxarray[i][0]=fillvals[i];
				maxarray[i][1]=filldist[i];
    		    }

    		     for (i=0;i<numpix;i++) /*looping through desired number of maximas*/
				 {
					for (j=i+1;j<8;j++) /*looping through pixel array*/
					{
						if (maxarray[j][0]>maxarray[i][0])
						{
						tempvalue=maxarray[i][0];
						maxarray[i][0]=maxarray[j][0];
						maxarray[j][0]=tempvalue;

						tempindex=maxarray[i][1];
						maxarray[i][1]=maxarray[j][1];
						maxarray[j][1]=tempindex;
						}
					}/*closes pixel loop*/

				 }/*closes maxima loop*/
	       	dsum=0.0;
	        for( i=0; i<numpix; i++ )
	          	{
	            dsum += maxarray[i][1];
	           	}
	       	dwavg=0.0;
	       	for( i=0; i<numpix; i++ )
	         	{
	           	weight[i]= (1.0 - (maxarray[i][1]/dsum))/(float)(numpix-1);
	           	dwavg += (weight[i]*maxarray[i][0]);
				}


		/*distances*/
    	/* Round distance weighted average to nearest integer */
    	  	frac_part = dwavg - (int)(dwavg);
           	if( frac_part>=0.5 )
        	   	{
           	   *(outfile+(r*ncols)+c) = 10*(unsigned short int)(dwavg + 1);
        	   	}
        	   	else
    	        	{
            	   *(outfile+(r*ncols)+c) = (unsigned short int)(dwavg*10);
    	        	}


       	/* Calculate mean distance and round to nearest integer */
	       	frac_part = dsum/(float)numpix - (int)(dsum/(float)numpix);
           	if( frac_part>=0.5 )
            	{
            	   *(distfile+(r*ncols)+c)=10*(signed short int)(dsum/((float)numpix+1));
            	}
            	else
    	        	{
            	   *(distfile+(r*ncols)+c) = 10*(signed short int)(dsum/(float)numpix);
    	        	}

    		} /*closes if else maxinc*/

		/*check for pixels not interpolated and mark in distance file*/
			if (*(outfile+(r*ncols)+c) <= 0)
			{
	 	   	*(outfile+(r*ncols)+c) = -1;
			*(distfile+(r*ncols)+c)= maxinc_32dir;
			}


		} /*close "else if( *(infile+(r*ncols)+c)==flag )" search for flagged pixels*/

	     	/**********************************************************
        	 Copy unflagged pixels under the mask from input to output

         	 *********************************************************/
		else
    	 {
    	*(outfile+(r*ncols)+c) = (signed short int)(*(infile+(r*ncols)+c)*10);
    	 }

      } /* close "c" loop (increment column counter) */

   } /* close "r" loop (increment row counter) */


   /***************************************************************
    Free memory allocated to inputs
    ***************************************************************/

   free(mask);
   free(infile);


   /***************************************************************
    write output files and free memory
    ***************************************************************/

   write_binary(total_pixels,outfn,"sint",(void *)(&outfile));
   free(outfile);

   write_binary(total_pixels,distfn,"sint",(void *)(&distfile));
   free(distfile);


   return 0;
}
