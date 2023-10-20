cmake_minimum_required(VERSION 3.14 FATAL_ERROR)
project(sobel_harness LANGUAGES CXX CUDA)

# 19 oct 2023: we are assuming the use of the NVHPC compiler on Perlmutter
# the compilation flags baked in below reflect that assumption
# set up your environment on Perlmutter with:
# module load PrgEnv-nvidia

set(CMAKE_BUILD_TYPE "Release")

add_executable(sobel_cpu sobel_cpu.cpp)
target_compile_options(sobel_cpu PRIVATE -fopenmp -fast)
target_link_options(sobel_cpu PRIVATE -fopenmp)

add_executable(sobel_cpu_omp_offload sobel_cpu_omp_offload.cpp)
target_compile_options(sobel_cpu_omp_offload PRIVATE -fast -mp=gpu -Minfo=mp,accel )
target_link_options(sobel_cpu_omp_offload PRIVATE -fast -mp=gpu -Minfo=mp,accel)

add_executable(sobel_gpu sobel_gpu.cu)
# generate CUDA code for an actual compute capability 7.0 device (Volta)
# set_property(TARGET sobel_gpu PROPERTY CUDA_ARCHITECTURES 70-real) 
