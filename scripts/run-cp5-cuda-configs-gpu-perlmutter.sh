#!/bin/bash -l
#SBATCH -A m3930
#SBATCH -C gpu
#SBATCH -q shared
#SBATCH -t 0:10:00
#SBATCH -n 1
#SBATCH -c 32
#SBATCH --gpus-per-task=1

#
# Optional command line argument: the name of the executable to run
#
# This script will execute a program, default name of "./sobel_gpu", over a range of 
# parameter values while running NVIDIA's ncu utility to collect various GPU metrics.
#
# See https://docs.nvidia.com/nsight-compute/NsightComputeCli/index.html#command-line-options
#

# first, "reset" the GPU to work around a known potential error
dcgmi profile --pause

# check for the optional command line argument: the name of an executable
if [ -z "$1" ] # no argument supplied
  then
   export EXE="./sobel_gpu"
else
   export EXE=$1
fi

# metrics
#gpu__time_duration.avg
#smsp__cycles_active.avg.pct_of_peak_sustained_elapsed
#dram__throughput.avg.pct_of_peak_sustained_elapsed

# iterate over threads/block and nblocks values
for N in 32 64 128 256 512 1024  # loop over nthreads per block
   do
      for B in 1 4 16 64 256 1024 4096 # loop over nblocks
         do
            echo "Working on config N=$N, B=$B"

            echo "ncu --set basic  --metrics smsp__cycles_active.avg.pct_of_peak_sustained_elapsed,dram__throughput.avg.pct_of_peak_sustained_elapsed,gpu__time_duration.avg --replay-mode kernel  --launch-count 1  $EXE -N $N -B $B"
            ncu --set basic  --metrics smsp__cycles_active.avg.pct_of_peak_sustained_elapsed,dram__throughput.avg.pct_of_peak_sustained_elapsed,gpu__time_duration.avg --replay-mode kernel  --launch-count 1  $EXE -N $N -B $B


      done # loop over nblocks
done # loop over nthreads per block

# EOF
