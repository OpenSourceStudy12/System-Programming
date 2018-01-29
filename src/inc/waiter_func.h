#ifndef _WAITER_FUNC_H
#define _WAITER_FUNC_H

#include "commen.h"
#include "waiter_client.h"


/***************定义客户端操作类型*******************/
//#define REGISTER     '1'   //注册
#define ENTER        '1'   //登录
#define EXIT         '0'   //退出

#define BOOK         '1'   //点菜
#define DISH_ADD     '2'   //加菜(提交后的菜品)
#define DISH_LOOK    '3'   //所点菜品查看
#define STATE_LOOK   '4'   //菜品状态查看
#define DISH_SUB     '5'   //减菜
#define SUBMIT       '6'   //提交菜品


#define TABLE_NUM    50     //餐桌数量

void head_menu();
void main_menu();
void man_menu();
void dish_menu();
void main_fun(int);
void man_fun(int);
//void regist(int);
int login(int);
void order_dish(int);
void dish_add(int);
void dish_sub(int);
void dish_look();
void state_look(int);
void dish_submit(int);
//int tablenumcheck(void* num,void* data);
int dishnum_check(void* num,void* data);
void dish_show(void* data);
void dish_printf(void* data);
int order_num_compare(void* data,void* ptr);
void order_state(void* argv);
void sig_handle(int argc);



#endif
