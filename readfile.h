#include <opencv2/opencv.hpp>
#include <iostream>  
#include <string>
#include <fstream>
#include "dirent.h"
using namespace std;  
using namespace cv;

#define wid 19
#define len 19
#define ColorLevel 256


void readFile(char* filename,unsigned char* img);
string* getFileName(char* DirectoryName);
int getFileNumber(char* DirectoryName);
void histNormalize(unsigned char* img);