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
string* getFileList(char* DirectoryName);
int getFileNumber(char* DirectoryName);
void histNormalize(unsigned char* img);
void accumulativeGraph(unsigned char* img,int *resultimg);
void showgraph(int* img);
void showgraph(unsigned char* img);
void writeFeature(char* filename,int* feature,int featureNum);
void readFeature(char* filename,int* feature,int featureNum);
void readsFeature(char* filename,double* e,int* p,int* index,double* threshold);
