#include "opencv2/opencv.hpp"

using namespace cv;

int countCells(Mat image)
{

	Mat outimage;
	Mat qwe;
	Mat kernel;
	int kernel_size = 3;
	int erosion_size = 13;
	int dilation_size = 2;


	kernel = Mat::ones(kernel_size, kernel_size, CV_8U) / (int)(kernel_size*kernel_size); //  The kernel 3*3 windows with initialized with 1s.


	GaussianBlur(image, outimage, Size(13, 13),0);																//GaussianBlur used for noise cancellation and better results!

	threshold(outimage, qwe, 25, 255, THRESH_BINARY + THRESH_OTSU);               //threshold the input image with OTSU's tecnique.

	Mat element1 = getStructuringElement(MORPH_RECT,															//Structural Element for Dilation Process
		Size(2 * dilation_size + 1, 2 * dilation_size + 1),
		Point(dilation_size, dilation_size));

	dilate(qwe, qwe, element1);																										//Dilate Function call for filling the gaps between pixels

	Mat element = getStructuringElement(MORPH_ELLIPSE,														//Structural Element for Erotion Process
		Size(2 * erosion_size + 1, 2 * erosion_size + 1),
		Point(erosion_size, erosion_size));

	erode(qwe, qwe, element);																											//Erode Function call for thinning the gaps between pixels


	Mat conn, stats, centroids, drawimage;

	connectedComponentsWithStats(qwe, conn, stats, centroids);										// Found the connectedComponents and count them

	int noCells = stats.size().height - 1;

	cvtColor(qwe, drawimage, CV_GRAY2RGB);																				//Conver Greyscale image to the RGB image! For dot visibility(RED DOTS)!

	for (int i = 0; i < noCells; i++)
	{

		circle(drawimage, Point(centroids.at<double>(i + 1, 0), centroids.at<double>(i + 1, 1)), 3, Scalar(0, 0, 255), -1, CV_AA); // Draw red circle center of the each connectedComponents

	}

	return noCells;                                                               // Return number of connectedComponents
}
