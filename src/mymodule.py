from skimage import io, morphology, data, color
import numpy as np
import matplotlib.pyplot as plt

img = io.imread('Threshold Map.bmp', as_grey=True)
img[img == 255] = 1
io.imshow(img)
img = 1 - img
skeleton = morphology.skeletonize(img)
skeleton = 1 - skeleton

skeleton[skeleton == 1] = 255
io.imsave('skeleton.bmp', skeleton)
