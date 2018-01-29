#ifndef __PACKAGE_H
#define __PACKAGE_H

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>

#define CLI_LOG_REQ	10
#define CLI_LOG_OK	11
#define CLI_LOG_FL	12
#define CLI_REG_REQ	20
#define CLI_REG_OK	21
#define CLI_REG_FL 	22

typedef struct 
{
	char version[20];	//软件版本
	char protocol[20];	//版本协议
	int type;			//消息类型
	int length;			//消息长度
	char data[0];		//尾指针
}package_t;

typedef struct 
{
	char account[20];
	char passwd[20];
}login_t;

typedef struct 
{
	char account[20];
	char passwd[20];
	char name[20];
	char tel[20];
	int age;
}regist_t;

void *packageHead(package_t pack, void *data);
void *package(char *version, char *protocol, int type, int length, void *data);
int packageSend(int sockfd, char *version, char *protocol, int type, int length, void *data);
int packageHeadSend(int sockfd, package_t pack, void *data);

#endif
