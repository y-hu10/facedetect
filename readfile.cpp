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