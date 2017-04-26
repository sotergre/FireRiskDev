/******************************************************************
  
 h5_readdyn.c  - Read dataset using dynamic arrays

******************************************************************/
 
#include "hdf5.h"

#define FILE        "dyn.h5"
#define DATASETNAME "IntArray" 

int
main (void)
{
    hid_t       file, dataset;         /* handles */
    hid_t       datatype, dataspace;   
    H5T_class_t class;                 /* data type class */
    H5T_order_t order;                 /* data order */
    size_t      size;                  /*
				        * size of the data element	       
				        * stored in file
				        */
    hsize_t     dims_out[2];           /* dataset dimensions */      
    herr_t      status;                             

    int          **data_out; 
    int          rows, cols;

    int          i, j, status_n, rank;


    /*  Open the file and the dataset  */
    file = H5Fopen(FILE, H5F_ACC_RDONLY, H5P_DEFAULT);
    dataset = H5Dopen(file, DATASETNAME);

    /* Get datatype and dataspace handles and then query
       dataset class, order, size, rank and dimensions  */
    datatype  = H5Dget_type(dataset);     /* datatype handle */ 
    class     = H5Tget_class(datatype);
    if (class == H5T_INTEGER) printf("Data set has INTEGER type \n");
    order     = H5Tget_order(datatype);
    if (order == H5T_ORDER_LE) printf("Little endian order \n");

    size  = H5Tget_size(datatype);
    printf(" Data size is %d \n", size);

    dataspace = H5Dget_space(dataset);    /* dataspace handle */
    rank      = H5Sget_simple_extent_ndims(dataspace);
    status_n  = H5Sget_simple_extent_dims(dataspace, dims_out, NULL);
    printf("rank %d, dimensions %lu x %lu \n", rank,
	   (unsigned long)(dims_out[0]), (unsigned long)(dims_out[1]));

    rows = dims_out[0];
    cols = dims_out[1];

/*************************** BEGIN  *******************************/

/* Allocate memory for new integer array[row][col]. First
   allocate the memory for the top-level array (rows).  Make
   sure you use the sizeof a *pointer* to your data type. */

    data_out = (int**) malloc(rows*sizeof(int*));

/* Allocate a contiguous chunk of memory for the array data values.  
   Use the sizeof the data type. */

    data_out[0] = (int*)malloc( cols*rows*sizeof(int) );

/* Set the pointers in the top-level (row) array to the
   correct memory locations in the data value chunk. */

    for (i=1; i < rows; i++) data_out[i] = data_out[0]+i*cols;

/************************* END *************************************/

    /* Initialize data_out array to 0 */
    for (j = 0; j < dims_out[0]; j++) {
        for (i = 0; i < dims_out[1]; i++) {
                data_out[j][i] = 0;
        }
    }

    status = H5Dread(dataset, H5T_NATIVE_INT, H5S_ALL, H5S_ALL,
		     H5P_DEFAULT, &data_out[0][0]);
    for (j = 0; j < dims_out[0]; j++) {
	for (i = 0; i < dims_out[1]; i++) printf("%d ", data_out[j][i]);
	printf("\n");
    }

    /* Close/release resources  */
    H5Tclose(datatype);
    H5Dclose(dataset);
    H5Sclose(dataspace);
    H5Fclose(file);

    free(data_out[0]);
    free(data_out);

    return 0;
}     
