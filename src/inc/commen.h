#ifndef _COMMEN_H
#define _COMMEN_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <signal.h>
#include <time.h>
#include "package.h"

/**********定义数据类型和操作************/
#define MEMBER      1     //会员
#define WAITER      2     //服务员
#define CHEF        3     //厨师
#define ADMIN       4     //管理员
//#define EMPLOYEE    2      //员工
#define FOOD        5     //菜式
#define MARTERIAL   6     //原材料
#define CONSUME     7     //消费
#define ORDER       8     //订单
#define CHECK       9     //查重
#define HEART       0     //心跳包

#define ADD         100    //增加
#define SUB         101    //减少
#define DEL         102    //删除
#define SHOW        103    //查看
#define MODIFY      104    //修改
#define SAVE        105    //保存
#define LOGIN       106    //登录
#define INT         107    //整型
#define STRING      108    //字符串型
#define FLOAT       109    //小数
#define L_K         110    //查看菜式
#define MODI        111    //厨房修改菜式        
#define PRE         112    //菜式准备
#define PAY         113    //结帐


#define OK          10    //成功
#define FAIL        11    //失败
#define YES         12    //重复
#define NO          13    //不重


#define LENGTH       1024   //命令和缓冲的大小

/**************************定义菜品种类**************************/
#define CLOD         '1'     //凉菜
#define VEGETABLE    '2'     //素菜
#define MEATE        '3'     //荤菜
#define DRY          '4'     //干锅
#define SPECIAL      '5'     //招牌菜
#define PLATE        '6'     //铁板
#define SOUP         '7'     //汤类
#define DRINK        '8'     //酒水

/*****************************定义原料***************************/
#define 	FLAVOUR     '1'      //调料
#define     FLOUR       '2'      //面
#define     RICE        '3'      //米
#define     DRINKS      '4'      //酒水
#define     POTATO      '5'      //土豆
#define     GREEN       '6'      //青菜
#define     CUKE        '7'      //黄瓜         
#define     TOMATO      '8'      //西红柿
#define     LETTUCE     '9'      //莴笋
#define     PEPPER      'a'      //青椒
#define     AGARIC      'b'      //木耳
#define     BEAN_SURD   'C'      //豆腐
#define     EGG         'd'      //鸡蛋
#define     GARLIC      'e'      //蒜苔
#define     LONG_BEAN   'f'      //豆角
#define     EGGPLANT    'g'      //茄子
#define     CHICKEN     'h'      //鸡肉
#define     MEAT        'i'      //猪肉
#define     FISH        'J'      //鱼肉
#define     INTES       'K'      //肥肠


//泛型结构体
typedef struct list
{
	void* data;
	struct list *next;
}list_t;


int getcnf(char*,char*);
void position(int x,int y,int z);
void set_pass(char* pass);
list_t* list_create(list_t* head,void* data,int size);
list_t* list_show(list_t* head,void(*func)(void*));
void* list_compare(list_t* head,void* data,int(*compare)(void*,void*));
void* list_comp(list_t* head,void* data1,void* data2,int(*compare)(void*,void*,void*));
list_t* list_insert(list_t* head,void* data,int size);
list_t* list_node_del(list_t* head,void* data,int(*func)(void*,void*));
void list_destroy(list_t* head);
void count_time(char* date,char* times);
void hour_min_sec(int *hour,int *min,int *sec);
int hide_passward(char *passwd);




#endif
