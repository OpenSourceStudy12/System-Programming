#ifndef _ADMIN_MENU_H
#define _ADMIN_MENU_H

#include "commen.h"
#include "admin_func.h"

/***********定义操作类型*************/
#define ENTER            '1'  //登录
#define EXIT             '0'  //退出

#define MEM              '1'  //会员
#define EMPLOY           '2'  //员工
#define DISH             '3'  //菜式
#define MATER            '4'  //材料
#define CONS             '5'  //消费

#define INCREASE         '1'  //添加
#define LOOK             '2'  //查看
#define MOD              '3'  //修改
#define DELETE           '4'  //删除

#define SALARY           '5'  //薪资

#define PAY_BILL         '1'  //结帐
//#define HISCONSUM_LOOK   '3'  //历史消费清单查看
#define HISCONSUM_DEL    '3'  //删除消费清单

void head_menu();
void main_menu();
void man_menu();
void member_menu();
void employee_menu();
void food_menu();
void material_menu();
void consum_menu();
void dish_menu();
void marter_menu();
void main_func(int);
void man_func(int);
void member_func(int);
void employee_func(int);
void food_func(int);
void material_func(int);
void consum_func(int);


#endif
