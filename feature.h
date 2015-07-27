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


//Rect feature
int calRectFeature(int* img,int x1,int y1,int x2,int y2);
int calRectFeature1(int* img,int x1,int y1,int x2,int y2);
int calRectFeature2(int* img,int x1,int y1,int x2,int y2);
int calRectFeature3(int* img,int x1,int y1,int x2,int y2);
int calRectFeature4(int* img,int x1,int y1,int x2,int y2);
void produceFeature(int* img,int* feature);
int featureList();

void featureSort(int* feature,int* order,int l,int h);

void sortedFeatureMatrix(int* feature,int* order,int* feature_face,int* feature_nonface
						 ,int featureNum,int faceNumber,int nonFaceNumber);
void swap(int* a,int* b);
void initWeight(double* weight,int* label,int faceNumber,int nonFaceNumber);
double weightError(int* feature,int* order,int* label,double* weight,int* p,double* threshold,int n);
void updateWeight(int* feature,int* order,int* label,double* weight,int n,double threhold,int p,double e);
bool classify(int* feature,double* e,double* threshold,int* p,int* featureIndex);
