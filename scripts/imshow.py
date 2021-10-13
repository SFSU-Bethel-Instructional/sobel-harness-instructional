#
# (C) 2021, E. Wes Bethel


# args:
# 1. name of raw data file to display (2D image, 8bit pixels)
# 2. xsize of data file (# cols)
# 3. ysize of data file (# rows)
#
import sys
import numpy as np
import matplotlib.pyplot as plt


# read data
#print ("Reading data from the file: ", sys.argv[1])
srcImg = np.fromfile(sys.argv[1],dtype='uint8')

# read dims
cols = int(sys.argv[2])
rows = int(sys.argv[3])

#print(" Reshaping to size cols = ", cols, " rows = ", rows)


# Reshape data from a 1D thing into a 2D image of the requested dimensions
srcImg=srcImg.reshape([rows, cols])

#print(" after reshape, array shapw is" , np.shape(srcImg))
plt.imshow(srcImg, cmap="gray", vmin=0, vmax=255)
plt.show()

# EOF

