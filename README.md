# sobel-harness-instructional

This directory contains the code harnesses for doing three different implementation of a Sobel edge detection
filter. The three implementations are:

* CPU only in C++, with your added OpenMP parallelism
* GPU in CUDA
* GPU in C++ with OpenMP Device Offload for running a kernel on the GPU

# Build instructions - general

This assignment requires use of the Nvidia compilers as follows:

    module load PrgEnv-nvidia
    export CC=cc
    export CXX=CC 

Then, once your environment is set up, then:

    mkdir build  
    cd build  
    cmake ../  -Wno-dev
    make

It is OK to do builds on the login node once you have set up the environment above.  
All code should be executed on a GPU node.

# Comments about all the codes

In all three cases -- CPU only, GPU, and OpenMP device offload -- the programs will all read raw bytes
of an image from a hard-coded location and of a hard-coded sizes. You are free to modify the source
of data and image size if you like.

The image provided with the distribution, located in the data subdirectory, is of a sufficient size , 7112x5146, to
cause the CPU and GPU to do a non-trivial amount of work. More information about these data are located below.

# Adding your code to sobel_cpu.cpp

Start here because you want the algorithm to work properly first on a CPU and in serial.

You will need to add code inside two routines:

* do_sobel_filtering() is where you will iterate over all input (i,j) pixels and then invoke the sobel_filtered_pixel() function, and then assign the computed value to the (i,j) location in the output.
* sobel_filtered_pixel(), is where you will perform the Sobel stencil computation, convolving the 3x3 x- and y- filter weights with the 3x3 pixel window surrounding the (i,j) location in the input

For OpenMP parallelism in this code, focus your attention on the do_sobel_filtering() function, and
implement loop parallelism over the one or two for loops that iterate over the input image.

# Testing and verifying your computations

For all programs, when you run the code, it reads from a hard-coded data file, and writes to a hard-coded output file, also located in the data subdirectory. Each of the 3 different codes writes to a different named file, e.g., processed-raw-int8-4x-cpu.dat, processed-raw-int8-4x-gpu.dat, etc.

You can verify your result by displaying the image with the python script provided. For example, from the sobel-harness-instructional directory,

    module load python
    python scripts/imshow.py data/zebra-gray-int8-4 7112 5146  # display the source image
    python scripts/imshow.py data/processed-raw-int8-4x-gpu.dat 7112 5146  # display result from your code

This will display the results of the "correct results" of the sobel filter applied to the default input dataset, zebra-gray-int8-4x. 

Note: if you're running this script from Perlmutter, please be sure that you:
* ssh -Y user@perlmutter-p1.nersc.gov when you connect so that X connections are tunneled through ssh, and the image will actually display remotely, and
* do a module load python    otherwise you will be accessing an outdated version of python.

# Adding your code to the sobel_gpu.cu

Next, head over to sobel_gpu.cu to work on the CUDA implementation of your sobel filter.

Here, you will need to add code to sobel_filtered_pixel() and sobel_kernel_gpu(). The sobel_filtered_pixel() code on the GPU might be nearly identical to your CPU code.

For the sobel_kernel_gpu() function, you will need to think like a CUDA coder and look at CUDA variables --  blockIdx, blockDim, threadIdx, blockDim and gridDim --  to compute the index/stride to use in striding through the source 
array, calling the sobel_filtered_pixel() function at each location to do the work.

You will probably use a processing motif here very similar to what we did in Lab#2, vector addition in CUDA.

# Adding code to the sobel_cpu_omp_offload.cpp

Here, you will need to add your code to the sobel_filtered_pixel() and do_sobel_filtering() functions.

Your sobel_filtered_pixel() code is likely identical to that for your sobel_cpu.cpp implementation.

Inside the do_sobel_filtering() function, you will need to add the code that iterates over every (i,j)
location of input and calls the sobel_filtered_pixel() method.

You will also need to add the following line of code:

 #pragma omp target teams parallel for  

around those loops to iterate over very (i,j) location of input. You may also wish to consider additional clauses that might be appropriate here to increase parallelism if you are using nested loops.

You will also need to add one more item to the line reading #pragma omp target data (...). As written, that line of code maps the input data and parameters to the device, but it does not have a map() statement to pull the data back from the device. You need to add that one more item to this line in order to have the results come back from the GPU. 

For additional reading, slide #22 on this URL may be helpful: https://www.nersc.gov/assets/Uploads/OpenMPTrainingShortAdvanced.pdf


# Information about data files

Zebra file dimensions 
* Original: 3556 2573
* 4x Augmented: 7112 5146

* zebra-gray-int8.dat - raw 8-bit grayscale pixel values from the Zebra_July_2008-1.jpg image
* zebra-gray-int8-4x.dat - raw 8-bit grayscale pixel values from the Zebra_July_2008-1.jpg image but 
augmented 2x in each direction

Source file:  Zebra_July_2008-1.jpg, obtained from Wikimedia commons, https://commons.wikimedia.org/wiki/File:Zebra_July_2008-1.jpg

# python display script

imshow.py - a python script to display the raw 8-bit pixel values in grayscale. 

Usage:  

    python imshow.py filename-of-raw-8bit-bytes int-cols-width int-rows-height


# eof
