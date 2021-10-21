//
// (C) 2021, E. Wes Bethel
// sobel_cpu_omp_offload.cpp
// usage:
//      sobel_cpu_omp_offload [no args, all is hard coded]
//

#include <iostream>
#include <vector>
#include <chrono>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include <omp.h>

// easy-to-find and change variables for the input.
// specify the name of a file containing data to be read in as bytes, along with 
// dimensions [columns, rows]

// this is the original laughing zebra image
//static char input_fname[] = "../data/zebra-gray-int8";
//static int data_dims[2] = {3556, 2573}; // width=ncols, height=nrows
//char output_fname[] = "../data/processed-raw-int8-cpu.dat";

// this one is a 4x augmentation of the laughing zebra
static char input_fname[] = "../data/zebra-gray-int8-4x";
static int data_dims[2] = {7112, 5146}; // width=ncols, height=nrows
char output_fname[] = "../data/processed-raw-int8-4x-cpu.dat";


// see https://en.wikipedia.org/wiki/Sobel_operator

//
// sobel_filtered_pixel(): perform the sobel filtering at a given i,j location
//
// input: float *s - the source data
// input: int i,j - the location of the pixel in the source data where we want to center our sobel convolution
// input: int nrows, ncols: the dimensions of the input and output image buffers
// input: float *gx, gy:  arrays of length 9 each, these are logically 3x3 arrays of sobel filter weights
//
// this routine computes Gx=gx*s centered at (i,j), Gy=gy*s centered at (i,j),
// and returns G = sqrt(Gx^2 + Gy^2)

// see https://en.wikipedia.org/wiki/Sobel_operator
//
float
sobel_filtered_pixel(float *s, int i, int j , int ncols, int nrows, float *gx, float *gy)
{

   float t=0.0;

   // ADD CODE HERE: add your code here for computing the sobel stencil computation at location (i,j)
   // of input s, returning a float

   return t;
}

//
//  do_sobel_filtering() will iterate over all input image pixels and invoke the
//  sobel_filtered_pixel() function at each (i,j) location of input to compute the
//  sobel filtered output pixel at location (i,j) in output.
//
// input: float *s - the source data, size=rows*cols
// input: int i,j - the location of the pixel in the source data where we want to center our sobel convolution
// input: int nrows, ncols: the dimensions of the input and output image buffers
// input: float *gx, gy:  arrays of length 9 each, these are logically 3x3 arrays of sobel filter weights
// output: float *d - the buffer for the output, size=rows*cols.
//
void
do_sobel_filtering(float *in, float *out, int ncols, int nrows)
{
   float Gx[] = {1.0, 0.0, -1.0, 2.0, 0.0, -2.0, 1.0, 0.0, -1.0};
   float Gy[] = {1.0, 2.0, 1.0, 0.0, 0.0, 0.0, -1.0, -2.0, -1.0};

   off_t out_indx = 0;
   int width, height, nvals;

   width=ncols;
   height=nrows;
   nvals=width*height;

// define the data mapping from the host to the device
// some of the data we only need to send: in, Gx, Gy, width, height
// some of the data we only need to retrieve: out

// ADD CODE HERE: you will need to add one more item to this line to map the "out" data array such that 
// it is returned from the the device after the computation is complete. everything else here is input.
#pragma omp target data map(to:in[0:nvals]) map(to:width) map(to:height) map(to:Gx[0:9]) map(to:Gy[0:9]) 
   {

   // ADD CODE HERE: insert your code here that iterates over every (i,j) of input,  makes a call
   // to sobel_filtered_pixel, and assigns the resulting value at location (i,j) in the output.
   
   // don't forget to include a  #pragma omp target teams parallel for around those loop(s).
   // You may also wish to consider additional clauses that might be appropriate here to increase parallelism 
   // if you are using nested loops.

   } // pragma omp target data
}


int
main (int ac, char *av[])
{
   // filenames, etc, hard coded at the top of the file
   // load input data
//    char input_fname[]="../data/zebra-gray-raw-int8.dat";
//   int data_dims[2] = {3556, 2573};
//   char output_fname[] = "../data/processed-raw-int8-cpu.dat";

   off_t nvalues = data_dims[0]*data_dims[1];
   unsigned char *in_data_bytes = (unsigned char *)malloc(sizeof(unsigned char)*nvalues);

   FILE *f = fopen(input_fname,"r");
   if (f == NULL)
   {
      printf(" Error opening the input file: %s \n", input_fname);
      return 1;
   }
   if (fread((void *)in_data_bytes, sizeof(unsigned char), nvalues, f) != nvalues*sizeof(unsigned char))
   {
      printf("Error reading input file. \n");
      fclose(f);
      return 1;
   }
   else
      printf(" Read data from the file %s \n", input_fname);
   fclose(f);

#define ONE_OVER_255 0.003921568627451

   // now convert from byte, in range 0..255, to float, in range 0..1
   float *in_data_floats = (float *)malloc(sizeof(float)*nvalues);
   for (off_t i=0; i<nvalues; i++)
      in_data_floats[i] = (float)in_data_bytes[i] * ONE_OVER_255;

   // now, create a buffer for output
   float *out_data_floats = (float *)malloc(sizeof(float)*nvalues);

   // do the processing =======================
   std::chrono::time_point<std::chrono::high_resolution_clock> start_time = std::chrono::high_resolution_clock::now();

   do_sobel_filtering(in_data_floats, out_data_floats, data_dims[0], data_dims[1]);

   std::chrono::time_point<std::chrono::high_resolution_clock> end_time = std::chrono::high_resolution_clock::now();

   std::chrono::duration<double> elapsed = end_time - start_time;
   std::cout << " Elapsed time is : " << elapsed.count() << " " << std::endl;

   // write output after converting from floats in range 0..1 to bytes in range 0..255
   unsigned char *out_data_bytes = in_data_bytes;  // just reuse the buffer from before
   for (off_t i=0; i<nvalues; i++)
      out_data_bytes[i] = (unsigned char)(out_data_floats[i] * 255.0);

   f = fopen(output_fname,"w");

   if (fwrite((void *)out_data_bytes, sizeof(unsigned char), nvalues, f) != nvalues*sizeof(unsigned char))
   {
      printf("Error writing output file. \n");
      fclose(f);
      return 1;
   }
   else
      printf(" Wrote the output file %s \n", output_fname);
   fclose(f);
}

// eof
