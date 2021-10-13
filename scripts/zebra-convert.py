#!/usr/bin/env python
# coding: utf-8
#
# (C) 2021, E. Wes Bethel
# Load an image file, convert to grayscale, write out grayscale image as raw bytes,
# create a 2x2 tiled version, an augmented data set, display it, and write it out
# to disk as raw bytes.

#
# Future work:
# Get rid of hard-coded input file name, use a command line argument (was doing dev in jupyter)
# Provide for additional processing options


import sys 
import numpy as np
import matplotlib.pyplot as plt
from PIL import Image

#print ("number of arguments is ", len(sys.argv))
#print ('Argument List:', str(sys.argv))

#fname = sys.argv[1]
fname = "Zebra_July_2008-1.jpg"

# load the image file 
image = Image.open(fname)

# uncomment to display the image just loaded
#imgplot=plt.imshow(image)
#plt.show()

# now convert the input image to grayscale
gray_image = image.convert('L')


gray_image_array = np.asarray(gray_image)
print("Zebra dimensions are", np.shape(gray_image_array)), " rows,cols"
# uncomment to  display the new grayscale image
#plt.imshow(gray_image_array, cmap='gray', vmin=0, vmax=255)
#plt.show()

# write out the grayscale image
gray_image_array.astype('int8').tofile("zebra-gray-int8")



# print out the size of the grayscle image
np.shape(gray_image_array)



# now, create an augmented version that is a 2x2 tiling of the original image
augmented_array = np.concatenate( (gray_image_array, gray_image_array) , axis=1 )
augmented_array = np.concatenate( (augmented_array, augmented_array) , axis=0 )

# display the augmented image, should be a 2x2 tiling of the input
plt.imshow(augmented_array, cmap='gray', vmin=0, vmax=255)
plt.show()


# and write to disk the augmented image
augmented_array.astype('int8').tofile("zebra-gray-int8-4x")

# and display the dimensions of the augmented image
print ("Dimensions of augmented array", np.shape(augmented_array))


# EOF
