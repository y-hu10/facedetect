#include <opencv2/opencv.hpp>  
#include <iostream>  
#include <string>
#include <fstream>
#include <time.h>
#include "readfile.h"
#include "feature.h"
#include "test.h"
using namespace std;  
using namespace cv;  

void readFileProduceFeature(string filePath,int featureNum,char* destFileName);
void adaboost(int*sfeatureMatrix,int*label,double*weight,int*order,int sampleNum,int featureNum,
			  int* f_featureIndex,double*f_e,int* f_p,double* f_threshold);
void writeTrainResult(int* f_featureIndex,double*f_e,int* f_p,double* f_threshold);