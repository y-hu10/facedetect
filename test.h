#include <opencv2/opencv.hpp>  
#include <iostream>  
#include <string>
#include <fstream>
#include <time.h>
#include "readfile.h"
#include "feature.h"
using namespace std;  
using namespace cv;  

void readFigure(char* filename,int* length,int* width,char* img);
void getFigureSize(char* filename,int* length,int*width);
void figureFeature(char* img,double scale,int length,int width);
void accumulativeGraph(unsigned char* img,int* resultimg,int length,int width);