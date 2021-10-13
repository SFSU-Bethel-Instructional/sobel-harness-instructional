cmake_minimum_required(VERSION 3.14 FATAL_ERROR)
project(sobel_harness LANGUAGES CXX CUDA)

set(CMAKE_BUILD_TYPE "Release")

add_executable(sobel_cpu sobel_cpu.cpp)
target_compile_options(sobel_cpu PRIVATE -fopenmp -march=native)
target_link_options(sobel_cpu PRIVATE -fopenmp)

add_executable(sobel_cpu_omp_offload sobel_cpu_omp_offload.cpp)
target_compile_options(sobel_cpu_omp_offload PRIVATE -Ofast -fopenmp -fopenmp-targets=nvptx64-nvidia-cuda)
target_link_options(sobel_cpu_omp_offload PRIVATE -Ofast -fopenmp -fopenmp-targets=nvptx64-nvidia-cuda )

add_executable(sobel_gpu sobel_gpu.cu)
# generate CUDA code for an actual compute capability 7.0 device (Volta)
set_property(TARGET sobel_gpu PROPERTY CUDA_ARCHITECTURES 70-real) 
