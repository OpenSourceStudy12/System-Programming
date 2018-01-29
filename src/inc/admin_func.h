#ifndef _ADMIN_FUNC_H
#define _ADMIN_FUNC_H

#include "commen.h"
#include "admin_menu.h"
#include "admin_client.h"


int admin_login(int);
int member_add(int);
void member_show(int);
void member_mod(int);
list_t* employee_info(int socketfd);
void employee_add(int);
void emlpoyee_show(int);
void employee_mod(int);
void member_del(int socketfd);
void employee_del(int);
void salary_mod();
void food_add();
list_t* food_info(int socketfd,char select);
void food_show(int);
void food_mod();
void food_del(int);
void material_add();
void material_show(int);
void material_mod();
void consum_show(int);
void consum_pay(int);
//void hisconsum_look();
void hisconsum_del(int);
void dish_look(void*);
void marterial_look(void* data);
void consum_look(void* argv);
void look_dish(void* data);
int consum_compare(void* num,void* data);
void employee_num(void* data);
void member_num(void* data);
int employee_num_check(void* data,void* agrv);
int member_num_check(void* data,void* argv);
int dish_num_check(void* data,void* argv);
void employee_printf(regist_t* ptr);
void member_printf(member_t* ptr);
void sig_handle(int argc);

#endif
