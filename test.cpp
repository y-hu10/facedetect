#include "test.h"

void getFigureSize(char* filename,int* length,int*width)
{
	Mat image;
	image = imread(filename,CV_LOAD_IMAGE_GRAYSCALE);
	*length = image.rows;
	*width = image.cols;
}

void readFigure(char* filename,int* length,int* width,char* img)
{
	Mat image;
	image = imread(filename,CV_LOAD_IMAGE_GRAYSCALE);
	for(int i = 0;i < *length;i++)
		for(int j = 0;j < *width;j++)
			*(img+i*(*width)+j) = *(image.data+i*(*width)+j);
}


void figureFeature(char* img,double scale,int length,int width)
{
	int windowLen = scale*(double)len;
	int windowWid = scale*(double)wid;
	int windowMove = scale*4.0;

	unsigned char* smallImg = new unsigned char[windowLen*windowWid];
	int* accimg = new int[windowWid*windowLen];

	int ff_x1[200],ff_y1[200],ff_x2[200],ff_y2[200],f_kind[200];
	ifstream iff("finalFeature.txt");
	for(int i = 0;i < 200;i++)
	{
		iff>>ff_x1[i]>>ff_y1[i]>>ff_x2[i]>>ff_y2[i]>>f_kind[i];
		ff_x1[i] = ff_x1[i]*scale;
		ff_y1[i] = ff_y1[i]*scale;
		ff_x2[i] = ff_x2[i]*scale;
		ff_y2[i] = ff_y2[i]*scale;
	}
	iff.close();

	int* f_p = new int[200];
	double* f_e = new double[200];
	double* f_threshold = new double[200];
	int* f_featureIndex = new int[200];
	readsFeature("feature.txt",f_e,f_p,f_featureIndex,f_threshold);

	int face = 0;
	for(int i = 0;i < length;i+=windowMove)
		for(int j = 0;j < width;j+=windowMove)
		{
			if(i+windowLen>length || j+windowWid>width)
				continue;
			for(int m = 0;m < windowLen;m++)
				for(int n = 0;n < windowWid;n++)
				{	
					char tmp = *(img+(i+m)*width+n+j);
					*(smallImg+m*windowWid+n) = (unsigned char)tmp;
				}

			Mat graph(Size(windowWid,windowLen), CV_8U, smallImg);
			cv::namedWindow("foobar1");
			cv::imshow("foobar1", graph);
			cv::waitKey(0);

			accumulativeGraph((unsigned char*)smallImg,accimg,windowLen,windowWid);
			
/*
			for(int i = 0;i< 10;i++)
			{
				for(int j = 0;j < 10;j++)
					cout << (int)*(smallImg+i*windowWid+j)<<" ";
				cout<<endl;
			}
			for(int i = 0;i< 10;i++)
			{
				for(int j = 0;j < 10;j++)
					cout << (int)*(accimg+i*windowWid+j)<<" ";
				cout<<endl;
			}*/

			int tfeature = 0;
			double sum1 = 0,sum2 = 0;
			for(int i = 0;i < 200;i++)
			{ 
				if(f_kind[i] == 1)
					tfeature = calRectFeature1(accimg,ff_x1[i],ff_y1[i],ff_x2[i],ff_y2[i],windowWid,windowLen);
				else if(f_kind[i] == 2)
					tfeature = calRectFeature2(accimg,ff_x1[i],ff_y1[i],ff_x2[i],ff_y2[i],windowWid,windowLen);
				else if(f_kind[i] == 3)
					tfeature = calRectFeature3(accimg,ff_x1[i],ff_y1[i],ff_x2[i],ff_y2[i],windowWid,windowLen);
				else if(f_kind[i] == 4)
					tfeature = calRectFeature4(accimg,ff_x1[i],ff_y1[i],ff_x2[i],ff_y2[i],windowWid,windowLen);

				tfeature = tfeature/scale;
				double beta = *(f_e+i)/(1.0-*(f_e+i));
				double at = log(1.0/beta);
				if((*(f_p+i)*tfeature)<*(f_p+i)*(*(f_threshold+i)))
					sum1 += at;
				sum2 += at;
			}
			if(sum1 > sum2/2)
			{
				cout<<"face"<<endl;
				face++;
			}
			else
				cout<<"not face"<<endl;
		}
}

void accumulativeGraph(unsigned char* img,int* resultimg,int length,int width)
{
	for(int i = 0;i < length;i++)
		for(int j = 0;j < width;j++)
		{
			if(i == 0&&j == 0)
				*(resultimg) = *(img);
			else if(i == 0)
				*(resultimg+j) = *(img+j)+*(resultimg+j-1);
			else if(j == 0)
				*(resultimg+i*width) = *(img+i*width) + *(resultimg+(i-1)*width);
			else
				*(resultimg+i*width+j) = *(img+i*width+j)+*(resultimg+(i-1)*width+j)+*(resultimg+i*width+j-1)-*(resultimg+(i-1)*width+j-1);
		}
}