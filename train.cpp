#include "train.h"

void readFileProduceFeature(string filePath,int featureNum,char* destFileName) //read image file, produce feature from image and write feature to file
{
	int fileNumber = getFileNumber((char*)filePath.c_str());
	string *s;
	s = getFileList((char*)filePath.c_str());
	unsigned char img[19][19];
	int accimg[19][19];
	int* feature = new int[featureNum];
	for(int k = 0;k < fileNumber;k++)
	{
		if(k%100==0)
			cout << k<< endl;
		readFile((char*)((filePath+s[k]).c_str()),(unsigned char*)img);
		histNormalize((unsigned char*)img);       //Normalized image
		//showgraph((unsigned char*)img);
		accumulativeGraph((unsigned char*)img,(int*)accimg);
		produceFeature((int*)accimg,feature);
		writeFeature(destFileName,feature,featureNum);
		

/*
		Mat graph(Size(len,wid), CV_8U, img);
		cv::namedWindow("foobar1");
		cv::imshow("foobar1", graph);
		cv::waitKey(0);*/
	}
}

void adaboost(int*sfeatureMatrix,int*label,double*weight,int*order,int sampleNum,int featureNum,
			  int* f_featureIndex,double*f_e,int* f_p,double* f_threshold)
{
	int p,bestP,featureIndex = 0;
	double bestThreshold,bestE = -1,threshold,e;

	ofstream of("feature.txt");
	for(int loop = 0;loop < 200;loop++)
	{	
		bestE = -1;bestThreshold = -1;bestP = 0;
		for(int i = 0;i < featureNum;i++)
		{
			e = weightError(sfeatureMatrix+i*(sampleNum),order+i*(sampleNum)
				,label,weight,&p,&threshold,sampleNum); //caculate best error for a certain feature
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
		updateWeight(sfeatureMatrix+featureIndex*sampleNum, //update the weight of samples
			order+featureIndex*sampleNum,label,weight,sampleNum,bestThreshold,bestP,bestE);
	}
	of.close();
}

void writeTrainResult(int* f_featureIndex,double*f_e,int* f_p,double* f_threshold)
{
	int f_x1[48627],f_y1[48627],f_x2[48627],f_y2[48627],kind[48627];
	int x1,y1,x2,y2,tkind;
	char temp[2000];
	ifstream iff("featureList.txt");
	if(!iff)
		cout<<"error"<<endl;
	for(int i = 0;i < 48627;i++)
	{
		iff>>x1>>y1>>x2>>y2>>temp>>tkind;
		f_x1[i] = x1;
		f_y1[i] = y1;
		f_x2[i] = x2;
		f_y2[i] = y2;
		kind[i] = tkind;
	}
	iff.close();

	int ff_x1[200],ff_y1[200],ff_x2[200],ff_y2[200],f_kind[200];
	ofstream off("finalFeature.txt");
	for(int i = 0;i < 200;i++)
	{
		ff_x1[i] = f_x1[f_featureIndex[i]];
		ff_y1[i] = f_y1[f_featureIndex[i]];
		ff_x2[i] = f_x2[f_featureIndex[i]];
		ff_y2[i] = f_y2[f_featureIndex[i]];
		f_kind[i] = kind[f_featureIndex[i]];
		off<<ff_x1[i]<<" "<<ff_y1[i]<<" "<<ff_x2[i]<<" "<<ff_y2[i]<<" "<<f_kind[i]<<endl;
	}
	off.close();
}