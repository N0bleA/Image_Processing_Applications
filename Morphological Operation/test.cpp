#include "Morphological_Count.hpp"
#include <string>

// number of cells in each image:
// 01 - 64
// 02 - 52
// 03 - 93
// 04 - 73
// 05 - 52
// 06 - 77
// 07 - 62
// 08 - 60
// 09 - 52
// 10 - 36

void main()
{
	for (int indImage = 1; indImage <= 10; indImage++)
	{
		// pad file name with zeros
		std::string fileName = std::to_string(indImage);
		while(fileName.size() < 2)
			fileName = "0" + fileName;

		// read as a grayscale image
		Mat image = imread(fileName + ".png", IMREAD_GRAYSCALE);

		// count the number of cells and print them
		std::cout << countCells(image) << std::endl;
	}
}
