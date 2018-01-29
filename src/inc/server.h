#ifndef _SERVER_H
#define _SERVER_H

#include "pool.h"
#include "server_func.h"

//时间
/*typedef struct
{
	int hour;
	int min;
	int sec;
}time_t;
*/

//客户端数据
typedef struct
{
	int clientfd;//台套接字
	char ip[20]; //ip
	int port;    //端口
//	time_t time; //保存客户端上次发消息的时间
}client_t;




#endif
