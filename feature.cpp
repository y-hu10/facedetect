#include "feature.h"

int calRectFeature(int* img,int x1,int y1,int x2,int y2)
{
	if(x1 == 0&&y1 == 0)
		return *(img+wid*x2+y2);
	if(y1 == 0)
		return *(img+wid*x2+y2)-*(img+wid*(x1-1)+y2);
	if(x1 == 0)
		return *(img+wid*x2+y2)-*(img+wid*x2+y1-1);
	else
		return *(img+wid*x2+y2)-*(img+wid*x2+y1-1)-*(img+wid*(x1-1)+y2)+*(img+wid*(x1-1)+y1-1);

}

int calRectFeature1(int* img,int x1,int y1,int x2,int y2)
{
	int det = floor((x2-x1)/2);
	return calRectFeature(img,x1,y1,x1+det,y2)-calRectFeature(img,x1+det+1,y1,x2,y2);
}

int calRectFeature2(int* img,int x1,int y1,int x2,int y2)
{
	int det = floor((y2-y1)/2);
	return -calRectFeature(img,x1,y1,x2,y1+det)+calRectFeature(img,x1,y1+det+1,x2,y2);
}

int calRectFeature3(int* img,int x1,int y1,int x2,int y2)
{
	int det = (y2-y1+1)/3;
	return -calRectFeature(img,x1,y1,x2,y1+det-1)+calRectFeature(img,x1,y1+det,x2,y1+det*2-1)-calRectFeature(img,x1,y1+det*2,x2,y2);
}

int calRectFeature4(int* img,int x1,int y1,int x2,int y2)
{
	int det1 = floor((x2-x1)/2);
	int det2 = floor((y2-y1)/2);
	return -calRectFeature(img,x1,y1,x1+det1,y1+det2)+calRectFeature(img,x1,y1+det2+1,x1+det1,y2)
		+calRectFeature(img,x1+det1+1,y1,x2,y1+det2)-calRectFeature(img,x1+det1+1,y1+det2+1,x2,y2);
}

void produceFeature(int* img,int* feature)
{
	 int num = 0;
	 for(int i = 0;i < len;i++)
		 for(int j = 0;j < wid;j++)
		 {
			 for(int m = i+1;m < len;m++)
				 for(int n = j+1;n < wid;n++)
				 {
					 if((m-i)%2==1)
					 {
						*(feature+num) = calRectFeature1(img,i,j,m,n);
						num++;
					 }
					 if((n-j)%2==1)
					 {
						*(feature+num) = calRectFeature2(img,i,j,m,n);
						num++;
					 }
					 if((n-j)%3==2)
					 {
						*(feature+num) = calRectFeature3(img,i,j,m,n);
						num++;
					 }
					 if((m-i)%2==1&&(n-j)%2==1)
					 {
						*(feature+num) = calRectFeature4(img,i,j,m,n);
						num++;
					 }
				 }
		 }
}

int featureList()
{
	ofstream of("featureList.txt");
	if(!of)
	{
		cout << "error writing file"<<endl;
		return -1;
	}

	int sum = 0;
	for(int i = 0;i < len;i++)
		for(int j = 0;j < wid;j++)
		{
			for(int m = i+1;m < len;m++)
				for(int n = j+1;n < wid;n++)
				{
					if((m-i)%2==1)
					{
						of<<i<<" "<<j<<" "<<m<<" "<<n<<" "<<"feature 1"<<endl;
						sum++;
					}
					if((n-j)%2==1)
					{
						of<<i<<" "<<j<<" "<<m<<" "<<n<<" "<<"feature 2"<<endl;
						sum++;
					}
					if((n-j)%3==2)
					{
						of<<i<<" "<<j<<" "<<m<<" "<<n<<" "<<"feature 3"<<endl;
						sum++;
					}
					if((m-i)%2==1&&(n-j)%2==1)
					{
						of<<i<<" "<<j<<" "<<m<<" "<<n<<" "<<"feature 4"<<endl;
						sum++;
					}
				}
		}

	of.close();
	return sum;
}

void featureSort(int* feature,int* order,int m,int n)
{
	if(n-m < 1)return;
	
	int key = *(feature+n),p,j;
	j = m-1;
	p = m;
	for(int i = m;i <= n-1;i++)
	{
		if(*(feature+p) <= key)
		{
			j++;
			swap(feature+p,feature+j);
			swap(order+p,order+j);
		}
		p++;
	}
	j++;
	swap(feature+j,feature+n);
	swap(order+j,order+n);
	featureSort(feature,order,m,j-1);
	featureSort(feature,order,j+1,n);
}

void swap(int *a,int *b)
{
	int temp = *a;
	*a = *b;
	*b = temp;
}

double weightError(int* feature,int* order,int* label,double* weight,int* p,double* threshold,int n)
{
	double bestError = -1,bestThreshold,error1,error2,error;
	int bestP;

	double Tplus = 0,Tminus = 0;
	double Splus = 0,Sminus = 0;
	int index;

	for(int i = 0;i < n;i++)
	{
		index = *(order+i);
		if(*(label+index) == 1)
			Tplus = Tplus + *(weight+index);
		else
			Tminus = Tminus + *(weight+index);
	}
	for(int i = 0;i < n;i++)
	{
		index = *(order+i);
		if(*(label+index) == 1)
			Splus = Splus + *(weight+index);
		else
			Sminus = Sminus + *(weight+index);
		error1 = Splus + (Tminus - Sminus);// sub is not face
		error2 = Sminus + (Tplus - Splus);// sub is face

		error = error1<error2?error1:error2;
		if(bestError == -1 || error < bestError)
		{
			bestError = error;
			bestThreshold = *(feature+i);
			bestP = (error1<error2)?-1:1;
		}
	}
	*p = bestP;
	*threshold = bestThreshold;
	return bestError;
}

void sortedFeatureMatrix(int* feature,int* order,int* feature_face,int* feature_nonface
						 ,int featureNum,int faceNumber,int nonFaceNumber)
{
	for(int i = 0;i < featureNum;i++)
	{
		for(int j = 0;j < faceNumber+nonFaceNumber;j++)
			*(order+i*(faceNumber+nonFaceNumber)+j) = j;
		for(int j = 0;j < faceNumber;j++)
			*(feature+i*(faceNumber+nonFaceNumber)+j) = *(feature_face+j*featureNum+i);
		for(int j = 0;j < nonFaceNumber;j++)
			*(feature+i*(faceNumber+nonFaceNumber)+j+faceNumber) = *(feature_nonface+j*(featureNum)+i);
		featureSort(feature+i*(faceNumber+nonFaceNumber),order+i*(faceNumber+nonFaceNumber),0,faceNumber+nonFaceNumber-1);
	}
}

void initWeight(double* weight,int* label,int faceNumber,int nonFaceNumber)
{
	for(int i = 0;i < faceNumber;i++)
	{
		*(label+i) = 1;
		*(weight+i) = 1.0/(2.0*faceNumber);
	}
	for(int i = faceNumber;i < faceNumber+nonFaceNumber;i++)
	{
		*(label+i) = 0;
		*(weight+i) = 1.0/(2.0*nonFaceNumber);
	}
}

void updateWeight(int* feature,int* order,int* label,double* weight,int n,double threhold,int p,double e)
{
	double beta = e/(1.0-e),ei;
	int index;
	int sum = 0;
	for(int i = 0;i < n;i++)
	{
/*
		if(p*(*(feature+i))<threhold*p == *(label+i))
			ei = 0;
		else
			ei = 1.0;*/
		index = *(order+i);
		ei = p*(*(feature+i))<threhold*p == *(label+index);
		sum += ei;
		*(weight+index) = *(weight+index)*pow(beta,ei);
	}
	double totalweight = 0;
	for(int i = 0;i < n;i++)
		totalweight = totalweight + *(weight+i);
	for(int i = 0;i < n;i++)
		*(weight+i) = *(weight+i)/totalweight;
}

bool classify(int* feature,double* e,double* threshold,int* p,int* featureIndex)
{
	double at,beta,sum1 = 0,sum2 = 0;
	for(int i = 0;i < 200;i++)
	{
		beta = *(e+i)/(1.0-*(e+i));
		at = log(1.0/beta);
		sum1 += at*(*(p+i)*(*(feature+*(featureIndex+i)))<*(p+i)*(*(threshold+i)));
		sum2 += at;
	}
	if(sum1 > sum2/2)
		return true;
	else
		return false;
}

