# data file dimensions
#
# source : Zebra_July_2008-1.jpg, Wikimedia commons
# converted to grayscale using the zebra-convert.py script

zebra-gray-int8: 3556 2573  [cols, rows], uint8 (bytes)

zebra-gray-int8-4x: 7112 5146 [cols, rows], uint8 (bytes)

correct_results_4x.dat: 7112 5146 [cols, rows], uint8 (bytes) -- this is what your output should look like after doing the sobel filter computation on the zebra-gray-int8-4x dataset.
