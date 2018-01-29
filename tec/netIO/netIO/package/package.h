#ifndef __PACKAGE_H
#define __PACKAGE_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>

#define CLI_LOG_REQ 10
#define CLI_LOG_OK  11
#define CLI_LOG_FL	12
#define CLI_REG_REQ	20
#define CLI_REG_OK	21
#define CLI_REG_FL  22

typedef struct 
{
	char version[20]; 	//软件版本
	char protocol[20]; 	//软件协议
	short type;			//操作类型
	int	length; 		//数据长度
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
	char sex;
	int age;
}regist_t;

void *package(char *version, char *protocol, short type, int length, void *data);
void *packageHead(package_t head, void *data);
#endif
