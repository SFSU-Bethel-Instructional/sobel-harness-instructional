#!/bin/bash 

#
# Oct 2021, w. bethel
# use these modules to set things up to do compilation/running on Cori GPU nodes
#

module purge
module load cmake
module load cgpu
module load PrgEnv-llvm/13_rc3

export CXX=clang++
export LIBOMPTARGET_INFO=4

# EOF
