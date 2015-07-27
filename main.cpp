#include <opencv2/opencv.hpp>  
#include <iostream>  
#include <string>
#include <fstream>
#include <time.h>
#include "readfile.h"
#include "feature.h"
using namespace std;  
using namespace cv;  
#define trainFacePath "train\\face\\"
#define trainNotFacePath "train\\non-face\\"
#define ColorLevel 256


int main()
{
	int faceNumber = getFileNumber(trainFacePath);
	int nonFaceNumber = getFileNumber(trainNotFacePath);
	int featureNum = featureList();
	clock_t start,finish;
	start = clock();

	int* feature_face = new int[faceNumber*featureNum];
	readFeature("face",feature_face,featureNum);

	int* feature_nonface = new int[nonFaceNumber*featureNum];
	readFeature("non-face",feature_nonface,featureNum);

	int* f_p = new int[200];
	double* f_e = new double[200];
	double* f_threshold = new double[200];
	int* f_featureIndex = new int[200];

	readsFeature("feature.txt",f_e,f_p,f_featureIndex,f_threshold);
	int sum = 0;
	for(int i = 0;i < faceNumber;i++)
		if(classify(feature_face+i*featureNum,f_e,f_threshold,f_p,f_featureIndex))
			sum++;
	cout << sum << endl;
	for(int i = 0;i < nonFaceNumber;i++)
		if(!classify(feature_nonface+i*featureNum,f_e,f_threshold,f_p,f_featureIndex))
			sum++;
	cout << sum<<endl;
/*
	int* label = new int[faceNumber+nonFaceNumber];
	double* weight = new double[faceNumber+nonFaceNumber];
	initWeight(weight,label,faceNumber,nonFaceNumber);

	double bestThreshold,bestE = -1,threshold,e;
	int p,bestP,featureIndex = 0;
	int* sfeatureMatrix = new int[(faceNumber+nonFaceNumber)*featureNum];
	int* order = new int[(faceNumber+nonFaceNumber)*featureNum];
	int* f_featureIndex = new int[200];
	int* f_e = new int[200];
	double* f_threshold = new double[200];
	int* f_p = new int[200];
	sortedFeatureMatrix(sfeatureMatrix,order,feature_face,feature_nonface,featureNum,faceNumber,nonFaceNumber);*/
/*
	ofstream of("feature.txt");
	for(int loop = 0;loop < 200;loop++)
	{	
		bestE = -1;bestThreshold = -1;bestP = 0;
		for(int i = 0;i < featureNum;i++)
		{
			e = weightError(sfeatureMatrix+i*(faceNumber+nonFaceNumber),order+i*(faceNumber+nonFaceNumber),label,weight,&p,&threshold,faceNumber+nonFaceNumber);
			if(bestE == -1|| e < bestE)
			{
				int sig = 0;
				for(int j = 0;j < loop;j++)
					if(i == f_featureIndex[j])
						sig = 1;
				if(sig == 1)
					continue;
				bestThreshold = threshold;
				bestP = p;
				bestE = e;
				featureIndex = i;
			}
		}
		f_e[loop] = bestE;
		f_threshold[loop] = bestThreshold;
		f_p[loop] = bestP;
		f_featureIndex[loop] = featureIndex;
		cout<<bestE<<" "<<bestThreshold<<" "<<bestP<<" "<<featureIndex<<endl;
		of<<bestE<<" "<<bestThreshold<<" "<<bestP<<" "<<featureIndex<<endl;
		for(int i = 0;i < 200;i++)
			cout << *(feature_face+i*featureNum+featureIndex) << " ";
		cout<<endl;
		for(int i = 0;i < 200;i++)
			cout << *(feature_nonface+i*featureNum+featureIndex) << " ";
		updateWeight(sfeatureMatrix+featureIndex*(faceNumber+nonFaceNumber),order+featureIndex*(faceNumber+nonFaceNumber),label,weight,faceNumber+nonFaceNumber,bestThreshold,bestP,bestE);
	}
	of.close();*/
	

	finish = clock();
	cout << finish-start <<" "<<(double)(finish-start)/CLOCKS_PER_SEC<<endl;
}


/*produce and write feature to file
	int fileNumber = getFileNumber(trainFacePath);
	string *s;
	s = getFileName(trainFacePath);
	for(int k = 0;k < fileNumber;k++)
	{
		if(k%100==0)
			cout << k<< endl;
		unsigned char img[19][19];
		int accimg[19][19];
		int* feature = new int[featureNum];
   		readFile((char*)((trainFacePath+s[k]).c_str()),(unsigned char*)img);
		histNormalize((unsigned char*)img);
		//showgraph((unsigned char*)img);
		accumulativeGraph((unsigned char*)img,(int*)accimg);
		produceFeature((int*)accimg,feature);
		writeFeature("non-face",feature,featureNum);
	}*/

/*
		for(int j = 0;j < faceNumber+nonFaceNumber;j++)
		{
			int temp;
			if(j%100 == 0)
				cin>>temp;
			cout<< *(feature+j)<<" ";
		}*/

/*test feautureSort
	int feature[9] = {7,2,3,4,1,5,9,8,0};
	int order[9] = {1,2,3,4,5,6,7,8,9};
	featureSort(feature,order,0,8);

	for(int i = 0;i < 9;i++)
		cout << feature[i] << " ";
	cout<<endl;
	for(int i = 0;i < 9;i++)
		cout << order[i] << " ";
	cout<<endl;*/


/*test feature calculation --- one image's feature
	ofstream of("feature.txt");
	if(!of)
		cout<<"error opening file"<<endl;
		for(int i = 0;i < featureNum;i++)
			of << *(feature+i) << " ";

		of.close();*/

/*test feature calculation
		cout << calRectFeature((int*)accimg,0,0,1,1)<<endl;
		cout << calRectFeature1((int*)accimg,0,0,3,3)<<endl;
		cout << calRectFeature2((int*)accimg,0,0,3,3)<<endl;
		cout << calRectFeature3((int*)accimg,0,0,1,2)<<endl;
		cout << calRectFeature4((int*)accimg,0,0,3,3)<<endl;*/

/*test accumulative graph calculation
		accumulativeGraph((unsigned char*)img,(int*)accimg);
		cout<<endl;
		showgraph((int*)accimg);*/

/*show figure
		Mat graph(Size(len,wid), CV_8U, img);
		cv::namedWindow("foobar1");
		cv::imshow("foobar1", graph);
		cv::waitKey(0);
*/
