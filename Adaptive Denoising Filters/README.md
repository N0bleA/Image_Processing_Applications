Adaptive filters aim to preserve information such as edges and object boundaries whilst
removing noise from the image. They utilize various local information i.e., local intensity mean,
local intensity variance, prediction of image noise stregnth, etc. to decide on the filtering type or
strength for a particular image pixel. In this assignment, you are supposed to implement an adaptive
local noise reduction filter with the following expression:

f̂(x, y) = g(x, y) −(σ^2η − σ^2L).( g(x,y) − μL)

where g(x, y) is the corrupted image, σ η2 and σ L2 are variances of the noise and local intensity, and
μL is the mean of local intensity, respectively. The function you are going to write for your
homework will take noise standard deviation (σ) as input, which is the square root of variance.

This is an implementation of adaptive noise cancellation function. You can prepare your own images and test it!
The codes are written in C++ on MSVS 2015! 
You need to have OpenCV libraries and configure your compiler for dependencies.!
