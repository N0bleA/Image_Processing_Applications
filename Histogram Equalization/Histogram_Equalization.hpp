#include "opencv2/opencv.hpp"

using namespace cv;

Mat histeq(Mat image)
{

	int x = image.rows;
	int y = image.cols;
	int color_level = 256;
	int histogram[256] = { 0 };																										// Initialize all pixels count to the zero!
	int equalized[256];
	double size = x*y;
	float probablity = 0.0;

	for (int j = 0; j < x; j++)
	{
		for (int k = 0; k < y; k++)
		{
			histogram[(int)image.at<uchar>(j, k)]++;          												// Count how many times a pixel is used in this image(8 bit images have value of betwenn 0-255)

		}
	}

	for (int n = 0; n < color_level; n++)
	{
		probablity += histogram[n] / size;                                          // Calculate PMF (probability mass function) for each pixel
		equalized[n] = round(probablity * (color_level - 1));												//Our next step involves calculation of CDF (cumulative distributive function).
	}

	Mat equalized_result(x, y, CV_8UC1);

	for (int t = 0; t < x; t++)
	{
		for (int r = 0; r < y; r++)
		{
			equalized_result.at<uchar>(t, r) = equalized[(int)image.at<uchar>(t, r)]; // Apply new histogram values to the image and get output image.
		}
	}
	Mat outImage;
	outImage = equalized_result;

	return outImage;
}
