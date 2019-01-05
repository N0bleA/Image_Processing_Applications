#include <iostream>
#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;
using cv::Mat;

Mat interpNN(Mat image, double scale);
Mat Bilinear(Mat image, double scale);

int main()
{
	Mat myImage1 = imread("Image_Directory!!!!!!!!!!!", CV_32F);									/*Read Images from the directory with different format*/
	Mat myImage = imread("Image_Directory!!!!!!!!!!!", CV_8UC1);									// 8bit and 32bit image format

	imshow("My Original Image", myImage1);																				/*Show input properties*/
	cout << "Original_Width : " << myImage.cols << endl;
	cout << "Original_Height: " << myImage.rows << endl;
	cout << "Original_Channels: " << myImage.channels() << endl;									//Show image's channel number 1 or 3!!!

	Mat scaledImage = interpNN(myImage1, 0.8);												  					/*Function Call for NN and show the output image*/
	imshow("My Scaled Image", scaledImage);

	Mat scaledImage1 = Bilinear(myImage1, 0.8);																		/*Function Call for Bilinear and show the output image*/
	imshow("My Scaled Ima45ge", scaledImage1);

	imwrite("Output images to the given directory", scaledImage1);								/*After functions call writes the new image*/

	waitKey(0);
	return(0);
}
Mat interpNN(Mat image, double scale)
{

	int New_size_row = image.rows*scale;																			    /*The size of the scaled image Row and Coloumn*/
	int New_size_coloumn = image.cols*scale;
	int Original_row = 0;
	int Original_coloumn = 0;

	Mat Test_image(New_size_row, New_size_coloumn, CV_8UC1);											/* Create new Matrices for output image,8UC3 type for color, 8UC1 for grey scale*/
	Mat Test_1_image(New_size_row, New_size_coloumn, CV_8UC3);

	char Channel = image.channels();																				      /* To use proper matrices, we need to find image channel*/

	if (Channel == 3)
	{
		for (int y = 0; y < New_size_coloumn; y++)
		{
			Original_coloumn = round(y / scale);

			for (int x = 0; x <New_size_row; x++)
			{
				Original_row = round(x / scale);
				/*There dimensional Image thats why we use "Vec3b"*/
				Test_1_image.at<Vec3b>(x, y)[0] = image.at<Vec3b>(Original_row, Original_coloumn)[0];
				Test_1_image.at<Vec3b>(x, y)[1] = image.at<Vec3b>(Original_row, Original_coloumn)[1];							/* Fill the pixel where we found the coordinates by Y/Scale */
				Test_1_image.at<Vec3b>(x, y)[2] = image.at<Vec3b>(Original_row, Original_coloumn)[2];
			}
		}
		image = Test_1_image;																												/* Our final image assingn by Test_1_image*/
	}

	if (Channel == 1)
	{
		for (int y = 0; y < New_size_coloumn; y++)
		{
			Original_coloumn = round(y / scale);

			for (int x = 0; x < New_size_row; x++)
			{

				Original_row = round(x / scale);
				Test_image.at<uchar>(x, y) = image.at<uchar>(Original_row, Original_coloumn);										/* Grey Scaled Image so we use <uchar>*/
			}
		}
		image = Test_image;
	}
																																								/* Show output image property*/
	cout << "Scaled_Width_Nearest:" << image.cols << endl;
	cout << "Scaled_Height_Nearest:" << image.rows << endl;
	cout << "Scaled_Channels_Nearest:" << image.channels() << endl;

	return image;
}

/* The codes are the same for NN and Bilinear only the algorithm can differ. I use same steps for each function so that there is no need to explain the all code.*/


Mat Bilinear(Mat image, double scale)                // For Detailed Approach please check out Bilinear Interpolation Algorithm!!
{
	int New_size_row = image.rows*scale;
	int New_size_coloumn = image.cols*scale;
	Mat Test_image(New_size_row, New_size_coloumn, CV_8UC1);
	Mat Test_1_image(New_size_row, New_size_coloumn, CV_8UC3);
	float C1, C2, C3, C4;
	int x, y;
	float D1, D2;
	char Channel = image.channels();

	float x_ratio = ((float)(image.rows - 1)) / New_size_row;
	float y_ratio = ((float)(image.cols - 1)) / New_size_coloumn;

	if (Channel == 3)
	{
		for (int a = 0; a < 3; a++)
		{
		for (int i = 0; i < New_size_row; i++)
			{
				for (int j = 0; j < New_size_coloumn; j++)
				{

					x = (int)(x_ratio * i);
					y = (int)(y_ratio * j);

					D1 = (x_ratio * i) - x;
					D2 = (y_ratio * j) - y;

					C1 = image.at<Vec3b>(x, y)[a];
					C2 = image.at<Vec3b>(x + 1, y)[a];
					C3 = image.at<Vec3b>(x, y + 1)[a];
					C4 = image.at<Vec3b>(x + 1, y + 1)[a];
					Test_1_image.at<Vec3b>(i, j)[a] = (C1* (1 - D1) * (1 - D2) + C2 * (D1) * (1 - D2) + C3 * (D2) * (1 - D1) + C4 * (D1 * D2));
				}
			}
		}
		image = Test_1_image;
	}

	if (Channel == 1)
	{
		for (int i = 0; i < New_size_row; i++)
		{
			for (int j = 0; j < New_size_coloumn; j++)
			{

				x = (int)(x_ratio * i);
				y = (int)(y_ratio * j);
				D1 = (x_ratio * i) - x;
				D2 = (y_ratio * j) - y;
				C1 = image.at<uchar>(x, y);
				C2 = image.at<uchar>(x + 1, y);
				C3 = image.at<uchar>(x, y + 1);
				C4 = image.at<uchar>(x + 1, y + 1);
				Test_image.at<uchar>(i, j) = (C1* (1 - D1) * (1 - D2) + C2 * (D1) * (1 - D2) + C3 * (D2) * (1 - D1) + C4 * (D1 * D2));
			}
		}
		image = Test_image;
	}
	cout << "Scaled_Width_Near:" << image.cols << endl;
	cout << "Scaled_Height_Nearest:" << image.rows << endl;
	cout << "Scaled_Channels_Nearest:" << image.channels() << endl;
	return image;
}
