#include<stdio.h>
#include<string.h>

//从文件读取一行数据 ：c库函数 fgets 读取成功返回传入的指针，失败或读到文件尾返回NULL

int getcnf(char* s,char* p)
{
	char line[200];
	char key[20];
	FILE* fp;

	fp = fopen("data.cnf","r");

	if(fp == NULL)
		return -1;

	while(!feof(fp))//feof 用于判断是否到文件尾
	{
		fgets(line,sizeof(line),fp);
		sscanf(line,"%s",key);

		if(key[0]=='#')
			continue;
		if(!strcmp(key,p))
		{
		//	sscanf(line,"%*s%*s%s",s);
			sscanf(line,"%*s%*c%*c%s",s);
		//	sscanf(line,"%[^=]",s);
			fclose(fp);
			return 0;
		}
	}
	fclose(fp);
	return -1;
}



int main()
{
	char account[15];
	char passwd[10];
	int ret;

	ret = getcnf(account,"account");
	if(ret < 0)
		return -1;
	ret = getcnf(passwd,"passwd");
	if(ret < 0)
		return -1;

	printf("account = %s,passwd  = %s\n",account,passwd);
	return 0;
}
