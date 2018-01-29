#ifndef _SERVER_FUNC_H
#define _SERVER_FUNC_H

#include <fcntl.h>
#include "pool.h"
#include "mysql.h"
#include "commen.h"
#include "package.h"
#include "server.h"


extern int epfd;
void setNonblock(int fd);
void client_close(list_t* head);
list_t* client_close_one(list_t * head,int clientfd);
int clientfd_compare(void* data,void* ptr);
void* heart_send(void* argv);
void communication(int clientfd);
void* check(void* argv);
void* member_add(void* argv);
void* member_show(void* argv);
void* member_del(void* argv);
static void* employee_add(void* argv);
void* employee_login(void* argv);
void* employee_show(void* argv);
void* employee_del(void* argv);
void* dishes_add(void* argv);
void* dish_look(void* argv);
void* dish_del(void* argv);
void* dish_state(void* argv);
void* tablenum_check(void* argv);
void* consum_submit(void* argv);
void* marterial_add(void* argv);
void* marterial_look(void* argv);
void* dish_l_k(void* argv);
void* marterial_sub(void* argv);
void* dish_modify_chef(void* argv);
void* dish_finsh_chef(void* argv);
void* consum_look(void* argv);
void* consum_del(void* argv);
void* dish_show(void* argv);
void* consum_pay(void* argv);
void* member_pay(void* argv);

#endif
