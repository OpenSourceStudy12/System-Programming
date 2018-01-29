#ifndef _PACKAGE_H
#define _PACKAGE_H

#include "commen.h"

#define ACCOUNT    11   //账户名长度
#define PASSWARD   7    //密码长度
#define NAME       21   //名字长度
#define TEL        12   //联系方式长度
#define ADDR       100  //联系地址长度
#define ID         19   //证件号码长度
#define SEX        5    //性别
#define DEPART     15   //部门

#define C_NAME    15  //菜类
#define NMAE      30  //菜名


//数据包的头
typedef struct 
{
	char version[30];   //软件版本
	char protocol[20];  //协议版本
	short type;         //数据类型
    short opt;          //数据操作
	int length;         //数据长度
	char data[0];       //结构体尾地址
}pack_head_t;

//员工信息
typedef struct
{
	int num;
	char account[ACCOUNT];//账户名称
	char passward[PASSWARD];  //账户密码
	char name[NAME];      //名字
	int age;              //年龄
	char sex[SEX];        //性别
	char tel[TEL];        //联系方式
	char address[ADDR];   //联系地址
	char id[ID];          //证件号码       
	float salary;         //薪水
	char depart[DEPART];  //职位
}regist_t;

//登录数据
typedef struct
{
	int flag;                  //标志位
	char account[ACCOUNT];     //账户名称
	char passward[PASSWARD];   //账户密码
}login_t;


//原材料数据
typedef struct
{
	int num;           //材料编号
	char name[NAME];   //材料名称
	float repertory;   //数量
	float price;       //材料价格(/kg)
}marterial_t;


//菜品数据
typedef struct
{
	int num;
	char class_name[C_NAME];  //菜类
	char name[NAME];          //菜名
	float price;              //价格
}dishes_t;

//消费信息
typedef struct
{
	int num;                   //消费编号
	int table_num;             //桌号
	int dish_num;              //菜式数量
	char state[20];            //消费状态
	float price;               //消费额
	char submit_time[20];      //菜式提交时间
	char pay_time[20];         //菜式结帐时间
	char date[20];             //日期
	char state1[15];           //状态
}consum_t;

//点菜信息
typedef struct
{
	int num;                  //菜式编号
	char class_name[C_NAME];  //菜类
	char name[NAME];          //菜名
	float price;              //单价
	int dish_num;             //数量
	char s_time[20];          //菜式准备时间
	char e_time[20];          //菜式完成时间
	char state[20];           //状态
}order_t;

//厨房菜式
typedef struct
{
	int con_num;    //消费遍号
	int table_num;  //桌号
	int flag;		//标志位
	order_t order;  //订单
}chef_t;

//会员信息
typedef struct 
{
	int num;             //编号
	char name[NAME];     //名字
	char sex[5];         //性别
	int age;             //年龄
	char tel[TEL];       //电话
	char id[ID];         //证件号
	char passward[PASSWARD]; //密码
	float integral;      //积分
}member_t;

//查重数据
typedef struct
{
	int flag;
	char str[50];
}check_t;

//心跳包
typedef struct
{
	char str[30];
}heart_t;


//泛型结构体
typedef struct list list_t;


void* package(pack_head_t head,void* data);
int package_send(int sockfd,short type,int opt,int length,void* data);
int package_list_send(int socketfd,short type,int opt,int length,int num,list_t* list_head);
int packagesend(int socketfd,short type,int opt,int len,void* data,int length,int num,list_t* list_head);

#endif
