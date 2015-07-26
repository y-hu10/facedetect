#include <opencv2/opencv.hpp>  
#include <iostream>  
#include <string>
#include <fstream>
#include "readfile.h"
using namespace std;  
using namespace cv;  
#define trainFacePath "train\\face\\"
#define trainNotFacePath "train\\non-face\\"
#define ColorLevel 256


int main()
{
	int fileNumber = getFileNumber(trainFacePath);
	string *s;
	s = getFileName(trainFacePath);
	
	for(int k = 0;k < fileNumber;k++)
	{
		unsigned char img[19][19];
   		readFile((char*)((trainFacePath+s[k]).c_str()),(unsigned char*)img);
		histNormalize((unsigned char*)img);

		Mat graph(Size(len,wid), CV_8U, img);
		cv::namedWindow("foobar1");
		cv::imshow("foobar1", graph);
		cv::waitKey(0);

	}
}




