#ifndef _CHEF_FUNC_H
#define _CHEF_FUNC_H

#include "commen.h"
#include "chef_client.h"


/***************定义客户端操作类型*******************/
#define ENTER        '1'   //登录
#define EXIT         '0'   //返回

#define PREPARE      '1'  //准备菜式
#define FINSH        '2'  //完成菜式

/*
typedef struct
{
	char name[NMAE];
	float num;
}mar_t;
*/

void head_menu();
void main_menu();
void man_menu();
void main_fun(int);
void man_fun(int);
int login(int);
void dish_prepare(int);
void dish_finsh(int);
void marter_menu();
void chef_look(void* data);
int check(void* table,void* dish,void* data);
int dish_finsh_check(void* num,void* data);
list_t* look_dish(int socketfd,int flag);
void sig_handle(int argc);



#endif
