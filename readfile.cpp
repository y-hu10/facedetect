#include "readfile.h"

void readFile(char* filename,unsigned char* img)
{
	ifstream infile;

	infile.open(filename,ios::binary);    //use binary to read file
	if(!infile)
		cout<<"error"<<endl;
	char buff;

	int k = 0;
	while(infile.read(&buff,sizeof(char)))
	{
		if(k >= 3)
		{
			*(img+k-3) = (char)((int)buff>0?(int)buff:128-(int)buff);
			k++;
		}
		else if(buff==(char)10)
			k++;	
	}
	infile.close();
	
/*  error:getLine can't read char SUB(ascii 23)
	while(getline(infile,buff))
	{
		if(k == 3)
		{
			char *char_buff = (char *)buff.c_str();
			for(int i = 0;i < 19;i++)
				for(int j = 0;j < 19;j++)
				{
					int temp = (int)*(char_buff+i*19+j);
					*(img+wid*i+j) = (char)(temp>0?temp:128-temp);
				}
		}
		k++;
	}*/
}


int getFileNumber(char* DirectoryName)
{
	int filenumber = 0;
	DIR *dir;
	struct dirent *ent;
	if ((dir = opendir (DirectoryName)) != NULL) {
		/* print all the files and directories within directory */
		while ((ent = readdir (dir)) != NULL) {
			if(strncmp(ent->d_name, ".", 1) == 0)
				continue;
			filenumber++;
			//	printf ("%s\n", ent->d_name);
		}
		closedir (dir);
	} else {
		/* could not open directory */
		perror ("");
		return NULL;
	}
	return filenumber;
}


string* getFileName(char* DirectoryName)
{
	DIR *dir;
	struct dirent *ent;
	int filenumber = getFileNumber(DirectoryName);
	string *fileName = new string[filenumber];
	int ind = 0;
	if ((dir = opendir (DirectoryName)) != NULL) {
		/* print all the files and directories within directory */
		while ((ent = readdir (dir)) != NULL) {
			if(strncmp(ent->d_name, ".", 1) == 0)
				continue;
			fileName[ind] = ent->d_name;
			ind++;
		}
		closedir (dir);
	} else {
		/* could not open directory */
		perror ("");
		return NULL;
	}
	return fileName;
}

void histNormalize(unsigned char*img)
{
	int fre[ColorLevel];
	memset(fre,0,ColorLevel*sizeof(int));
	for(int i = 0;i < len;i++)
		for(int j = 0;j < wid;j++)
			fre[*(img+i*wid+j)] = fre[*(img+i*wid+j)] + 1;
	for(int i = 1;i < ColorLevel;i++)
		fre[i] = fre[i-1] + fre[i];
	for(int i = 0;i < ColorLevel;i++)
		fre[i] = floor((double)fre[i]/(double)len/(double)wid*(double)ColorLevel);
	for(int i = 0;i < len;i++)
		for(int j = 0;j < wid;j++)
			*(img+i*wid+j) = fre[*(img+i*wid+j)];
}

void accumulativeGraph(unsigned char* img,int* resultimg)
{
	for(int i = 0;i < len;i++)
		for(int j = 0;j < wid;j++)
		{
			if(i == 0&&j == 0)
				*(resultimg) = *(img);
			else if(i == 0)
				*(resultimg+j) = *(img+j)+*(resultimg+j-1);
			else if(j == 0)
				*(resultimg+i*wid) = *(img+i*wid) + *(resultimg+(i-1)*wid);
			else
				*(resultimg+i*wid+j) = *(img+i*wid+j)+*(resultimg+(i-1)*wid+j)+*(resultimg+i*wid+j-1)-*(resultimg+(i-1)*wid+j-1);
		}
}

void showgraph(int* img)
{
	for(int i = 0;i < len;i++)
	{
		for(int j = 0;j < wid;j++)
		{
			cout << (int)*(img+i*wid+j) << " ";
		}
		cout << endl;
	}
}

void showgraph(unsigned char* img)
{
	for(int i = 0;i < len;i++)
	{
		for(int j = 0;j < wid;j++)
		{
			cout << (int)*(img+i*wid+j) << " ";
		}
		cout << endl;
	}
}

void writeFeature(char* filename,int* feature,int feautureNum)
{
	ofstream of(filename,ios::binary|ios::app);
	if(!of)
		cout<<"error"<<endl;
	for(int i = 0;i < feautureNum;i++)
		of.write((char*)(feature+i),sizeof(int));
	of.close();
}

void readFeature(char* filename,int* feature,int featureNum)
{
	ifstream iff(filename,ios::binary);
	if(!iff)
	{
		cout<<"error"<<endl;
		return;
	}
	int buff;


	int num = 0;
	while(iff.read((char*)&buff,sizeof(int)))
	{
		*(feature+num*featureNum) = buff;
		for(int i = 1;i < featureNum;i++)
		{
			iff.read((char*)&buff,sizeof(int));
			*(feature+num*featureNum+i) = buff;
		}
		num++;
	}

}

void readsFeature(char* filename,double* e,int* p,int* index,double* threshold)
{
	int t_p,t_index;
	double t_threshold,t_e;
	ifstream iff(filename);
	for(int i = 0;i < 200;i++)
	{
		iff>>t_e>>t_threshold>>t_p>>t_index;
		*(e+i) = t_e;
		*(threshold+i) = t_threshold;
		*(p+i) = t_p;
		*(index+i) = t_index;
	}
}