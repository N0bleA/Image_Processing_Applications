#include "opencv2/opencv.hpp"
#include <math.h>
using namespace cv;


Mat denoise(Mat image, double noiseStdDev, int windowSize)
{
	// The input "image" depth is 8UC1 but due to some calculation image depth is changed for some specific operation.!
	// f̂(x, y) = g(x, y) −(σ^2η − σ^2L).( g(x,y) − μL) the implementation of formula is given below!!!!!!!!!!!!!!!!

	Mat outImage(image.size(), CV_8UC1);                                          // For output image
	Mat CorruptImage_minus_LocalMeanIntensity(image.size(), CV_32FC1);
	Mat LocalMean(image.size(), CV_8UC1);                                         //local mean of each window in the image
	Mat LocalMeanSquare(image.size(), CV_32FC1);
	Mat Local_Intensity(image.size(), CV_32FC1);
	Mat Local_Mean_32BitFloat(image.size(), CV_32FC1);
	Mat Variance_over_LocalIntensity(image.size(), CV_8UC1);
	Mat Outimage_32FC1(image.size(), CV_32FC1);
	Mat corrupted_image(image.size(), CV_32FC1);
	Mat variable_4(image.size(), CV_32FC1);                                       // (σ^2η − σ^2L).( g(x,y) − μL)
	Mat variable_10(image.size(), CV_32FC1);																			// Image depth conversion from (8UC1 to 32FC1) and called variable_10

	int border = (windowSize - 1) / 2;                                            // The border value is calculated according to the Window(Kernel_Size)

	double a, b;
	double mean;
	mean = 0;

	int new_row, new_col;
	new_row = image.rows;
	new_col = image.cols;

	image.convertTo(corrupted_image, CV_32FC1);                                   // Input image depth is converted to the 32bit Float with single channel!(g(x,y)-------->image)

	for (int x = 0; x < new_col; x++)
	{
		for (int y = 0; y < new_row; y++)
		{
			for (int kernel_x = 0; kernel_x < windowSize; kernel_x++)
			{
				a = x + kernel_x - border;

				for (int kernel_y = 0; kernel_y < windowSize; kernel_y++)
				{
					b = y + kernel_y - border;																						// Alternatively Blur function can be used but the standart iplementation of the mean process!!!

					if (a >= 0 && b >= 0 && a < new_col && b < new_row)
					{
						mean += image.at<uchar>(b, a) / pow(windowSize, 2);
					}
				}
			}
			LocalMean.at<uchar>(y, x) = mean;
			mean = 0;
		}
	}

	LocalMean.convertTo(Local_Mean_32BitFloat, CV_32FC1);                         //LocalMean of the image converted to the 32bit single channel image due to variable convention!!!
	CorruptImage_minus_LocalMeanIntensity = corrupted_image - Local_Mean_32BitFloat;													// The (g(x, y) − μL) part of the formulation!!!!!!!!

	blur(corrupted_image.mul(corrupted_image), LocalMeanSquare, Size(windowSize, windowSize), Point(-1, -1), BORDER_DEFAULT); //Blurs an image using the normalized box filter.

	sqrt(LocalMeanSquare - Local_Mean_32BitFloat.mul(Local_Mean_32BitFloat), Local_Intensity); // To find local Local_Intensity use the formula!!

	Mat Noise(image.size(), CV_32FC1, Scalar(noiseStdDev));                       // Create noisy image with noiseStdDev------>(σ η)
	Mat Noise_2;																																	// Take the second power of the noisy image for formula
	pow(Noise, 2, Noise_2);																												//(σ^2 η)

	Mat Variance(image.size(), CV_32FC1);
	pow(Local_Intensity, 2, Variance);                                            // Variance is equal to  second power of the Local_Intensity

	Variance_over_LocalIntensity = Noise_2.mul(1 / Variance);

	CorruptImage_minus_LocalMeanIntensity.convertTo(variable_10, CV_32FC1);       // Type conversion from (8UC1 to 32FC1) and called variable_10

  variable_4 = Variance_over_LocalIntensity.mul(variable_10);                   // (σ^2η − σ^2L).( g(x,y) − μL) this is the multiplication called variable_4

	Outimage_32FC1 = corrupted_image - variable_4;                                //  g(x, y) −(σ^2η − σ^2L).( g(x,y) − μL) this is the subtraction!

	Outimage_32FC1.convertTo(outImage, CV_8UC1);																	// Output image with 8UC1

	return outImage;
}
