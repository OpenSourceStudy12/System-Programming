#ifndef _MYSQL_H
#define _MYSQL_H

#include <mysql/mysql.h>
#include "commen.h"
#include "pool.h"

/*
typedef struct
{
	int num1;
	int num2;
}num_t;
*/

extern MYSQL mysql;


int get_maxnum(char*,char*);
int check_str(char* table,char* conlum,char* values);
int get_max_num(char* table);
int employee_insert(regist_t* reg);
list_t* employee_look(char* table,int* num);
list_t* member_look(char* table,int* num);
int employee_eroll(char*,char*,char*,char*,int);
int dishes_insert(char* class_name,char* name,float price);
list_t* dish_search(char* table,char* conlum,char* name,int* num);
list_t* marterial_search(char*,char*,char*,int*);
int check_tablenum(char* table,int num);
int get_num(char*);
list_t* get_consum(char* str,int* num);
list_t* get_dish(char* str,int* num,int n);
list_t* get_dish_waiter(char* str,int* num);
list_t* look_consum(char* str,int* num);
int get_integral(char* table,char* conlum,char* account);


#endif
