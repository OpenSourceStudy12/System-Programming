#ifndef _FILE_H
#define _FILE_H

#include<iostream>
#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>

using namespace std;

class File
{
protected:
	int fd;
	string filename;
	int flag;
public:
	File(const string &,int);
	~File();
	bool open();
	int read(char*,int);
	int write(char*,int);
	void close();

};


#endif
