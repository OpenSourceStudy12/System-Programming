#include "server_func.h"


int epfd; //epoll文件描述符

/****************************************
 *function:设置文件描述符属性为非阻塞
 ****************************************/
void setNonblock(int fd)
{
	int flag;
	fcntl(fd,F_GETFL);
	flag |= O_NONBLOCK;
	fcntl(fd,F_SETFL,flag);
	return;
}

/************************************
 *function:关闭所有客户端套接字
 ************************************/
void client_close(list_t* head)
{
	list_t *tmp;
	if(head == NULL)
		return;
	
	tmp = head;
	while(tmp != NULL)
	{
		if(tmp->data != NULL)
			close(((client_t*)(tmp->data))->clientfd);
		tmp = tmp->next;
	}
	list_destroy(head);
}

/*************************************************
 *function:关闭某一客户端套接字
 *************************************************/ 
list_t* client_close_one(list_t * head,int clientfd)
{
	client_t *tmp = list_compare(head,&clientfd,clientfd_compare);
   	if(tmp == NULL)	
		return head;

	close(tmp->clientfd);
	head = list_node_del(head,&clientfd,clientfd_compare);
	return head;
}


/************************************************
 *function:比较套接字
 ************************************************/
int clientfd_compare(void* data,void* ptr)
{
	if(data == NULL || ptr == NULL)
		return -1;
	if(((client_t*)ptr)->clientfd == *(int*)data)
		return 0;
	else
		return -1;
}
/********************************************
 *function:判断操作类型，将函数添加到线程池
 ********************************************/
void communication(int clientfd)
{
	int ret,type,opt,length,port;
	char buff[200],ip[20];
	void* data;
	struct epoll_event ev;
	pack_head_t head;
	struct sockaddr_in clientaddr;
	int clientaddrlen;
	void*(*pfunc)(void*);    //定义函数指针
	/*
	   getpeername(clientfd,(struct sockaddr*)&clientaddr,&clientaddrlen);
	   port = ntohs(clientaddr.sin_port);
	   inet_ntop(AF_INET,&clientaddr.sin_addr.s_addr,ip,sizeof(ip));
	 */
	printf("------------------------\n");///////////////////////////////////
	pthread_mutex_lock(&socketfd_mutex);
	ret = read(clientfd,&head,sizeof(pack_head_t));
	pthread_mutex_unlock(&socketfd_mutex);
	if(ret < 0)
	{
		perror("read");
		printf("error in:%s %d\n",__FILE__,__LINE__);
		return;
	}
	else if(ret == 0)
	{
		printf("客户端断开连接\n");

		ev.data.fd = clientfd;
		ev.events = EPOLLIN | EPOLLET;
		epoll_ctl(epfd,EPOLL_CTL_DEL,clientfd,&ev);
		return;
	}

	//解包(包头)
	type = head.type;//数据类型
	opt = head.opt;  //数据操作

	printf("type = %d,opt = %d\n",type,opt);////////////////////////////////////

	//判断进行何种操作
	switch(type)
	{
		/*		case CHECK:
				switch(opt)
				{
				case INT:
				break;
				case STRING:
				break;
				case FLOAT:
				break;
				default:
				break;
				}
				break;*/
		case MEMBER:
			switch(opt)
			{
				case CHECK:
					pfunc = check;
					break;
				case ADD:
					pfunc = member_add;
					break;
				case DEL:
					pfunc = member_del;//删除会员
					break;
				case SHOW:
					pfunc = member_show;//会员查看
					break;
				case MODIFY:
					break;
				case SAVE:
					break;
				case PAY:
					pfunc = member_pay;//以会员方式结帐
					break;
				default:
					break;
			}
			break;
		case WAITER:
		case CHEF:
		case ADMIN:
			switch(opt)
			{
				case ADD:
					pfunc = employee_add;//添加有员工
					break;
				case LOGIN:
					pfunc = employee_login;//员工登录验证
					break;
				case DEL:
					pfunc = employee_del;
					break;
				case SHOW:
					pfunc = employee_show;//查看员工
					break;
				case L_K:
					pfunc = dish_state;//手持终端查看菜式状态
					break;
				case MODIFY:
					break;
				case SAVE:
					break;
				case CHECK:
					pfunc = check;
					break;
				default:
					break;
			}
			break;
		case FOOD:
			switch(opt)
			{
				case ADD:
					pfunc = dishes_add;//添加菜式
					break;
				case DEL:
					pfunc = dish_del;
					break;
				case SHOW:
					pfunc = dish_look;//菜式查看
					break;
				case MODIFY:
					break;
				case SAVE:
					break;
				default:
					break;
			}
			break;
		case MARTERIAL:
			switch(opt)
			{
				case ADD:
					pfunc = marterial_add;
					break;
				case DEL:
					break;
				case SUB:
					pfunc = marterial_sub;
					break;
				case SHOW:
					pfunc = marterial_look;
					break;
				case MODIFY:
					break;
				case SAVE:
					break;
				default:
					break;
			}
			break;
		case CONSUME:
			switch(opt)
			{
				case CHECK:
					break;
				case ADD:
					pfunc = tablenum_check;//查看对应桌号上是否存在客人
					break;
				case DEL:
					pfunc = consum_del;//删除历史订单
					break;
				case SHOW:
					pfunc = consum_look;//消费信息查看
					break;
				case MODIFY:
					break;
				case SAVE:
					pfunc = consum_submit;//提交订单
					break;
				case PAY:
					pfunc = consum_pay;//结帐
					break;
				default:
					break;
			}
			break;
		case ORDER:
			switch(opt)
			{
				case ADD:
					break;
				case DEL:
					break;
				case SHOW:
					pfunc = dish_show;
					break;
				case MODIFY:
					pfunc = dish_modify_chef;//厨房准备菜式
					break;
				case SAVE:
					pfunc = dish_finsh_chef;//厨房完成菜式
					break;
				case L_K:
					pfunc = dish_l_k;//厨房查看菜式
					break;
				case MODI:
					break;
				default:
					break;
			}
			break;
		case HEART:
			pfunc = heart_send;//发送心跳包
			break;
		default:
			return;
	}

	func_add(pfunc,&clientfd,sizeof(clientfd));
}

/************************************************
 *function:发送心跳包
 ************************************************/
void* heart_send(void* argv)
{
	int socketfd = *(int*)argv,ret;
	heart_t heart;
	
	read(socketfd,&heart,sizeof(heart));//接收心跳包
	printf("heart = %s\n",heart.str);//////////////////////
	strcpy(heart.str,"this is ok!");

	ret = package_send(socketfd,0,0,sizeof(heart),&heart);//发送心跳包
	if(ret < 0)
	{
		printf("send fail\n");
	}
}
/*****************************************************************
 *function:帐号查重
 *****************************************************************/
void* check(void* argv)
{
	int socketfd = *(int*)argv;
	int ret,res = NO;
	check_t check;

	//获取帐号
	ret = read(socketfd,&check,sizeof(check));
	if(ret < 0)
	{
		perror("read");
		printf("error in:%s %d\n",__FILE__,__LINE__);
		res = YES; 
	}
	else if(ret == 0)
	{
		printf("与服务器断开连接\n");
		return NULL;
	}

	printf("---------------------------------\n");
	//帐号查重
	if(check.flag == WAITER || check.flag == CHEF)
		ret = check_str("employee","account",check.str);
	else if(check.flag == MEMBER)
		ret = check_str("member","tel",check.str);
	if(ret < 0)
	{
		printf("error in :%s %d\n",__FILE__,__LINE__);
		res = YES;
	}
	else if(ret == 1)
		res = YES;

	printf("flag = %d\n",check.flag);///////////////////////
	printf("res = %d\n",res);//////////////////////////
	package_send(socketfd,0,res,0,NULL);
}

/********************************************
 *function:员工添加
 ********************************************/
static void* employee_add(void* argv)
{
	int socketfd = *(int*)argv;
	int ret,res;
	char buff[LENGTH];
	pack_head_t head;
	login_t login;
	regist_t regist;

	/*************接收注册员工信息*************/
	for(;;)
	{
		ret = read(socketfd,&regist,sizeof(regist));
		if(ret < 0)
		{
			perror("read");
			printf("error in:%s %d\n",__FILE__,__LINE__);
			continue;
		}
		else if(ret == 0)
		{
			printf("客户端断开连接\n");
			return NULL;
		}
		break;
	}
	ret = employee_insert(&regist);
	if(ret == 0)
		res = OK;
	else
		res = FAIL;

	for(;;)
	{
		ret = package_send(socketfd,0,res,0,NULL);
		if(ret < 0)
			continue;
		return NULL;
	}
}

/***********************************************
 *function:查看员工信息
 ***********************************************/
void* employee_show(void* argv)
{
	int socketfd = *(int*)argv;
	int ret,n;
	list_t* list;

	list = employee_look("employee",&n);
	package_list_send(socketfd,0,0,sizeof(regist_t),n,list);
	list_destroy(list);
}

/*****************************************************
 *function:删除员工
 *****************************************************/
void* employee_del(void* argv)
{
	int socketfd = *(int*)argv;
	int ret,flag = OK,num;
	char str[LENGTH];

	ret = read(socketfd,&num,sizeof(num));
	if(ret < 0)
	{
		printf("error in : %s %d\n",__FILE__,__LINE__);
		flag = FAIL;
	}
	else if(ret == 0)
		printf("客户端断开连接\n");

	strcpy(str,"delete from employee where num = ");
	sprintf(str + strlen(str),"%d",num);

	pthread_mutex_lock(&mysql_mutex);
	ret = mysql_query(&mysql,str);
	pthread_mutex_unlock(&mysql_mutex);

	if(ret != 0)
	{
		printf("error in : %s %d\n",__FILE__,__LINE__);
		flag = FAIL;
	}

	package_send(socketfd,0,flag,0,NULL);
}

/**********************************************************************
 *function:会员办理
 **********************************************************************/
void* member_add(void* argv)
{
	int socketfd = *(int*)argv;
	int ret,res = OK;
	char str[LENGTH];
	pack_head_t head;
	member_t member;

	/*************接收会员信息*************/
	ret = read(socketfd,&member,sizeof(member));
	if(ret < 0)
	{
		perror("read");
		printf("error in:%s %d\n",__FILE__,__LINE__);
		res = FAIL;
	}
	else if(ret == 0)
	{
		printf("客户端断开连接\n");
		return NULL;
	}

	/************************获取编号**********************/
	member.num = get_max_num("member");
	if(member.num < 0)
		res = FAIL;
	else if(member.num == 0)
		member.num = MEMBER * 1000;
	else
		member.num += 1;
	/******************设置mysql命令*******************/
	bzero(str,sizeof(str));
	strcat(str,"insert into member values(");
	sprintf(str + strlen(str),"%d",member.num);
	strcat(str,",'");
	strcat(str,member.name);
	strcat(str,"','");
	strcat(str,member.sex);
	strcat(str,"',");
	sprintf(str + strlen(str),"%d",member.age);
	strcat(str,",'");
	strcat(str,member.tel);
	strcat(str,"','");
	strcat(str,member.id);
	strcat(str,"','");
	strcat(str,member.passward);
	strcat(str,"',");
	sprintf(str + strlen(str),"%.1f",member.integral);
	strcat(str,")");

	printf("str = %s\n",str);///////////////////////////////////
	pthread_mutex_lock(&mysql_mutex);
	ret = mysql_query(&mysql,str);
	pthread_mutex_unlock(&mysql_mutex);
	if(ret != 0)
		res = FAIL;

	package_send(socketfd,0,res,0,NULL);
}


/***********************************************
 *function:查看会员信息
 ***********************************************/
void* member_show(void* argv)
{
	int socketfd = *(int*)argv;
	int ret,n;
	list_t* list;

	list = member_look("member",&n);
	package_list_send(socketfd,0,0,sizeof(member_t),n,list);
	list_destroy(list);
}


/*****************************************************
 *function:删除会员
 *****************************************************/
void* member_del(void* argv)
{
	int socketfd = *(int*)argv;
	int ret,flag = OK,num;
	char str[LENGTH];

	ret = read(socketfd,&num,sizeof(num));
	if(ret < 0)
	{
		printf("error in : %s %d\n",__FILE__,__LINE__);
		flag = FAIL;
	}
	else if(ret == 0)
		printf("客户端断开连接\n");

	strcpy(str,"delete from member where num = ");
	sprintf(str + strlen(str),"%d",num);

	pthread_mutex_lock(&mysql_mutex);
	ret = mysql_query(&mysql,str);
	pthread_mutex_unlock(&mysql_mutex);

	if(ret != 0)
	{
		printf("error in : %s %d\n",__FILE__,__LINE__);
		flag = FAIL;
	}

	package_send(socketfd,0,flag,0,NULL);
}

/***************************************************
 *function:员工登录验证
 ***************************************************/
void* employee_login(void* argv)
{
	int socketfd = *(int*)argv;
	int ret,res = OK;
	login_t data;

	for(;;)
	{
		ret = read(socketfd,&data,sizeof(data));
		if(ret < 0)
		{
			perror("read");
			printf("error in:%s %d\n",__FILE__,__LINE__);
			usleep(100*1000);
			continue;
		}
		else if(ret == 0)
		{
			printf("客户端断开连接\n");
			return NULL;
		}

		if(data.flag == WAITER)
			ret = employee_eroll("employee",data.account,data.passward,"服务员",WAITER);
		else if(data.flag == CHEF)
			ret = employee_eroll("employee",data.account,data.passward,"厨师",CHEF);
		else if(data.flag == ADMIN)
			ret = employee_eroll("employee",data.account,data.passward,"管理员",ADMIN);
		else if(data.flag == MEMBER)
			ret = employee_eroll("member",data.account,data.passward,NULL,MEMBER);

		if(ret < 0)
			res = FAIL;
		break;
	}	
	
	if(data.flag == MEMBER && res == OK)
	{
		res = get_integral("member","tel",data.account);	
		package_send(socketfd,0,res,0,NULL);
	}
	else if(data.flag == MEMBER && res == FAIL)
		package_send(socketfd,0,-1,0,NULL);
	else
		package_send(socketfd,0,res,0,NULL);
}

/***************************************
 *function:吧台添加菜式
 ***************************************/
void* dishes_add(void* argv)
{
	int socketfd = *(int*)argv;
	int ret,n,flag = OK;
	dishes_t dish;

	//	pthread_mutex_lock(&socketfd_mutex);
	ret = read(socketfd,&n,sizeof(int));
	//	pthread_mutex_unlock(&socketfd_mutex);
	if(ret < 0)
		flag = FAIL;
	else if(ret == 0)
	{
		printf("吧台客户端断开连接\n");
		return NULL;
	}
	//	printf("n = %d\n",n);////////////////////////////////
	for(int i=0;i < n;i++)
	{
		//		pthread_mutex_lock(&socketfd_mutex);
		ret = read(socketfd,&dish,sizeof(dish));
		//		pthread_mutex_unlock(&socketfd_mutex);
		if(ret < 0)
		{
			flag = FAIL;
			break;
		}
		else if(ret == 0)
		{
			printf("吧台客户端断开连接\n");
			return NULL;		
		}

		ret = dishes_insert(dish.class_name,dish.name,dish.price);
		if(ret < 0)
		{
			flag = FAIL;
			break;
		}
	}
	//	pthread_mutex_lock(&socketfd_mutex);
	package_send(socketfd,0,flag,0,NULL);
	//	pthread_mutex_unlock(&socketfd_mutex);
}

/*********************************************************
 *function:吧台菜式查看
 *********************************************************/
void* dish_look(void* argv)
{
	int socketfd = *(int*)argv;
	int ret,flag = OK,num;
	char select;
	char class[20];
	list_t* head;

	ret = read(socketfd,&select,sizeof(select));
	if(ret < 0)
		flag = FAIL;
	else if(ret == 0)
	{
		printf("客户端断开连接\n");
		return NULL;
	}
	switch(select)
	{
		case CLOD:
			strcpy(class,"凉  菜");
			break;
		case VEGETABLE:
			strcpy(class,"素  菜");
			break;
		case MEATE:
			strcpy(class,"荤  菜");
			break;
		case DRY:
			strcpy(class,"干  锅");
			break;
		case SPECIAL:
			strcpy(class,"招牌菜");
			break;
		case PLATE:
			strcpy(class,"铁  板");
			break;
		case SOUP:
			strcpy(class,"汤  类");
			break;
		case DRINK:
			strcpy(class,"酒  水");
			break;
		case 'a':
			strcpy(class,"NULL");
			break;
		default:
			strcpy(class,"NULL");
			break;
	}
	head = dish_search("dishes","class",class,&num);
	package_list_send(socketfd,0,OK,sizeof(dishes_t),num,head);
	list_destroy(head);
}

/**************************************************
 *function:吧台删除菜式
 **************************************************/
void* dish_del(void* argv)
{
	int socketfd = *(int*)argv;
	int ret,flag = OK,num;
	char str[LENGTH];

	ret = read(socketfd,&num,sizeof(num));
	if(ret < 0)
	{
		printf("error in : %s %d\n",__FILE__,__LINE__);
		flag = FAIL;
	}
	else if(ret == 0)
		printf("客户端断开连接\n");

	strcpy(str,"delete from dishes where num = ");
	sprintf(str + strlen(str),"%d",num);

	pthread_mutex_lock(&mysql_mutex);
	ret = mysql_query(&mysql,str);
	pthread_mutex_unlock(&mysql_mutex);

	if(ret != 0)
	{
		printf("error in : %s %d\n",__FILE__,__LINE__);
		flag = FAIL;
	}
	package_send(socketfd,0,flag,0,NULL);
}

void* tablenum_check(void* argv)
{
	int socketfd = *(int*)argv;
	int ret,table_num,opt;

	ret = read(socketfd,&table_num,sizeof(table_num));
	if(ret < 0)
	{
		printf("error in : %s %d\n",__FILE__,__LINE__);
		return NULL;
	}
	else if(ret == 0)
	{
		printf("与客户端断开连接!\n");
		printf("%s %d\n",__FILE__,__LINE__);
		return NULL;
	}

	ret = check_tablenum("consum",table_num);
	if(ret == 0)
		opt = NO;
	else
		opt = YES;

	printf("opt =%d\n",opt);
	package_send(socketfd,0,opt,0,NULL);
}

/**********************************************
 *function:提交订单
 **********************************************/
void* consum_submit(void* argv)
{
	int socketfd = *(int*)argv;
	int ret,num,opt = OK;
	char str[LENGTH];
	consum_t consum;
	order_t order;
	pack_head_t head;

	ret = read(socketfd,&consum,sizeof(consum));
	if(ret < 0)
	{
		printf("error in : %s %d\n",__FILE__,__LINE__);
		opt = FAIL;
	}
	else if(ret == 0)
	{
		printf("与客户端断开连接!\n");
		printf("%s %d\n",__FILE__,__LINE__);
		return NULL;
	}
	/***********获取编号**********/
	consum.num = get_max_num("consum");
	if(consum.num < 0)
	{
		printf("error in : %s %d\n",__FILE__,__LINE__);
		opt = FAIL;
	}
	if(consum.num == 0)
		consum.num = CONSUME*1000;
	else
		consum.num += 1;

	sprintf(str,"%s%d%s%d%s%d%s%s%s%f%s%s%s%s%s%s%s%s%s","insert into consum values(",consum.num,",",consum.table_num,",",consum.dish_num,",'",consum.state,"',",consum.price,",'",consum.submit_time,"','",consum.pay_time,"','",consum.date,"','",consum.state1,"')");

	printf("str = %s\n",str);////////////////////////////////////
	/*********************插入消费信息***********************/
	pthread_mutex_lock(&mysql_mutex);
	ret = mysql_query(&mysql,str);
	pthread_mutex_unlock(&mysql_mutex);
	if(ret != 0)
	{
		printf("error in : %s %d\n",__FILE__,__LINE__);
		opt = FAIL;
	}

	/*****************************插入菜式信息****************************/
	//	read(socketfd,&head,sizeof(head));
	printf("dish_num = %d\n",consum.dish_num);///////////////////////////
	//读取点菜数e
	//	pthread_mutex_lock(&socketfd_mutex);
	ret = read(socketfd,&num,sizeof(num));
	//	pthread_mutex_unlock(&socketfd_mutex);
	if(ret < 0)
	{
		printf("error in : %s %d\n",__FILE__,__LINE__);
		opt = FAIL;
	}
	else if(ret == 0)
	{
		printf("与客户端断开连接!\n");
		printf("%s %d\n",__FILE__,__LINE__);
		opt = FAIL;
	}

	for(int i=0;i<num;i++)
	{
		//		pthread_mutex_lock(&socketfd_mutex);
		ret = read(socketfd,&order,sizeof(order));
		//		pthread_mutex_unlock(&socketfd_mutex);
		if(ret < 0)
		{
			printf("error in : %s %d\n",__FILE__,__LINE__);
			opt = FAIL;
			break;
		}
		else if(ret == 0)
		{
			printf("与客户端断开连接!\n");
			printf("%s %d\n",__FILE__,__LINE__);
			opt = FAIL;
		}

		sprintf(str,"%s%d%s%s%s%s%s%f%s%s%s%s%s%d%s%d%s%s%s","insert into consum_dish values(",order.num,",'",order.class_name,"','",order.name,"',",order.price,",'",order.s_time,"','",order.e_time,"',",consum.num,",",order.dish_num,",'",order.state,"')");

		printf("str = %s\n",str);////////////////////////////////////

		pthread_mutex_lock(&mysql_mutex);
		ret = mysql_query(&mysql,str);
		pthread_mutex_unlock(&mysql_mutex);
		if(ret != 0)
		{
			printf("error in : %s %d\n",__FILE__,__LINE__);
			opt = FAIL;
			break;
		}
	}

	/*********************反馈结果*********************/
	package_send(socketfd,0,opt,0,NULL);
}

/*************************************************
 *function:查看菜式状态
 *************************************************/
void* dish_state(void* argv)
{
	int socketfd = *(int*)argv;
	int ret,num,flag = OK;
	char str[LENGTH];
	list_t *list;
	consum_t consum;

	ret = read(socketfd,&num,sizeof(num));
	if(ret < 0)
	{
		flag = FAIL;
		printf("error in : %s %d\n",__FILE__,__LINE__);
	}
	else if(ret == 0)
	{
		printf("手持客户端断开连接\n");
		return NULL;
	}

	strcpy(str,"select * from consum where table_num = ");
	sprintf(str + strlen(str),"%d",num);
	strcat(str," and (state != '完成' or state1 = '未结帐')");

	printf("str = %s\n",str);////////////////////
	
	list = get_consum(str,&num);
	if(list == NULL)
	{
		flag = FAIL;
		package_send(socketfd,0,flag,0,NULL);
		return NULL;
	}

	memcpy(&consum,list->data,sizeof(consum));
	list_destroy(list);
	list = NULL;

	strcpy(str,"select * from consum_dish where con_num = ");
	sprintf(str + strlen(str),"%d",consum.num);

	printf("str = %s\n",str);////////////////////
	list = get_dish_waiter(str,&num);
	if(list == NULL)
		flag = FAIL;

	packagesend(socketfd,0,flag,sizeof(consum),&consum,sizeof(order_t),consum.dish_num,list);
	list_destroy(list);
}

/*********************************************
 *function:添加原材料
 *********************************************/
void* marterial_add(void* argv)
{
	int socketfd = *(int*)argv;
	int ret,n,flag = OK;
	marterial_t marterial;
	char str[LENGTH];

	//	pthread_mutex_lock(&socketfd_mutex);
	ret = read(socketfd,&n,sizeof(int));
	//	pthread_mutex_unlock(&socketfd_mutex);
	if(ret < 0)
	{
		flag = FAIL;
		printf("error in : %s %d\n",__FILE__,__LINE__);
	}
	else if(ret == 0)
	{
		printf("吧台客户端断开连接\n");
		return NULL;
	}
	//	printf("n = %d\n",n);////////////////////////////////
	for(int i=0;i < n;i++)
	{
		//		pthread_mutex_lock(&socketfd_mutex);
		ret = read(socketfd,&marterial,sizeof(marterial));
		//		pthread_mutex_unlock(&socketfd_mutex);
		if(ret < 0)
		{
			flag = FAIL;
			printf("error in : %s %d\n",__FILE__,__LINE__);
			break;
		}
		else if(ret == 0)
		{
			printf("吧台客户端断开连接\n");
			return NULL;		
		}
		/*******************设置mysql命令*********************/
		ret = check_str("marterial","name",marterial.name);
		if(ret < 0)
		{
			flag = FAIL;
			printf("error in : %s %d\n",__FILE__,__LINE__);
		}
		else if(ret == 0)//该材料未添加
		{
			//获取编号
			marterial.num = get_max_num("marterial");
			if(marterial.num < 0)
			{
				flag = FAIL;
				printf("error in : %s %d\n",__FILE__,__LINE__);
				break;
			}
			else if(marterial.num == 0)
				marterial.num = MARTERIAL * 1000;
			else 
				marterial.num += 1;

			sprintf(str,"%s%d%s%s%s%f%s%f%s","insert into marterial values(",marterial.num,",'",marterial.name,"',",marterial.repertory,",",marterial.price,")");
		}
		else//材料已存在
		{
			sprintf(str,"%s%f%s%f%s%s%s","update marterial set repertory = repertory+",marterial.repertory,", price = ",marterial.price," where name = '",marterial.name,"'");
		}

		//插入命令
		pthread_mutex_lock(&mysql_mutex);
		pthread_mutex_unlock(&mysql_mutex);
		ret = mysql_query(&mysql,str);
		if(ret != 0)
		{
			flag = FAIL;
			printf("error in : %s %d\n",__FILE__,__LINE__);
			break;
		}
	}
	//反馈结果
	//	printf("opt = %d\n",flag);
	//	pthread_mutex_lock(&socketfd_mutex);
	package_send(socketfd,0,flag,0,NULL);
	//	pthread_mutex_unlock(&socketfd_mutex);
}

/********************************************
 *function:查看原材料
 ********************************************/
void* marterial_look(void* argv)
{
	int socketfd = *(int*)argv;
	int ret,flag = OK,num;
	char select;
	char name[20];
	list_t* head;

	//	printf("111111111111111111111111");//////////////////////////////
	//	pthread_mutex_lock(&socketfd_mutex);
	ret = read(socketfd,&select,sizeof(select));
	//	pthread_mutex_unlock(&socketfd_mutex);
	if(ret < 0)
		flag = FAIL;
	else if(ret == 0)
	{
		printf("客户端断开连接\n");
		return NULL;
	}
	//	printf("saelect = %c\n",select);//////////////////////////////
	switch(select)
	{
		case FLAVOUR:
			strcpy(name,"调料");
			break;
		case FLOUR:
			strcpy(name,"面");
			break;
		case RICE:
			strcpy(name,"米");
			break;
		case DRINKS:
			strcpy(name,"酒水");
			break;
		case POTATO:
			strcpy(name,"土豆");
			break;
		case GREEN:
			strcpy(name,"青菜");
			break;
		case CUKE:
			strcpy(name,"黄瓜");
			break;
		case TOMATO:
			strcpy(name,"西红柿");
			break;
		case LETTUCE:
			strcpy(name,"莴笋");
			break;
		case PEPPER:
			strcpy(name,"青椒");
			break;
		case AGARIC:
			strcpy(name,"木耳");
			break;
		case BEAN_SURD:
			strcpy(name,"豆腐");
			break;
		case EGG:
			strcpy(name,"鸡蛋");
			break;
		case GARLIC:
			strcpy(name,"蒜苔");
			break;
		case LONG_BEAN:
			strcpy(name,"豆角");
			break;
		case EGGPLANT:
			strcpy(name,"茄子");
			break;
		case CHICKEN:
			strcpy(name,"鸡肉");
			break;
		case MEAT:
			strcpy(name,"猪肉");
			break;
		case FISH:
			strcpy(name,"鱼肉");
			break;
		case INTES:
			strcpy(name,"肥肠");
			break;
		default:
			strcpy(name,"NULL");
			break;
	}
	//	printf("菜类：%s\n",class);	//////////////////////////////
	head = marterial_search("marterial","name",name,&num);

	//	pthread_mutex_lock(&socketfd_mutex);
	package_list_send(socketfd,0,OK,sizeof(marterial_t),num,head);
	//	pthread_mutex_unlock(&socketfd_mutex);

	//	printf("num = %d\n",num);/////////////////////////////////
	list_destroy(head);
}

/*********************************************
 *function:厨房查看菜式
 *********************************************/
void* dish_l_k(void* argv)
{
	int socketfd = *(int*)argv;
	int ret,flag,num;
	pack_head_t head;
	char str[LENGTH];
	list_t *list,*tmp,*list_head;
	
	ret = read(socketfd,&flag,sizeof(flag));
	if(ret < 0)
	{	
		printf("read error in : %s %d\n",__FILE__,__LINE__);
		return NULL;
	}
	else if(ret == 0)
	{
		printf("客户端断开连接\n");
		printf("%s %d\n",__FILE__,__LINE__);
		return NULL;
	}

	bzero(str,sizeof(str));
	strcat(str,"select * from consum where state != '完成'");

	list = get_consum(str,&num);//获取消费信息

	ret = package_send(socketfd,0,0,sizeof(num),&num);//发送消费数量
	if(ret < 0)
	{
		printf("send error in %s %d\n",__FILE__,__LINE__);
		list_destroy(list);
		return NULL;
	}

	tmp = list;
	for(int i=0;i<num;i++)
	{
		int n;

		bzero(str,sizeof(str));
		strcpy(str,"select * from consum_dish where con_num = ");
		sprintf(str + strlen(str),"%d",((consum_t*)(tmp->data))->num);
	//	if(flag == 0)
	//		strcat(str," and state = '等待中'");
	//	else if(flag == 1)
		strcat(str," and state != '完成'");
		list_head = get_dish(str,&n,((consum_t*)(tmp->data))->table_num);
		ret = package_list_send(socketfd,0,0,sizeof(chef_t),n,list_head);//发送菜式信息
		if(ret < 0)
		{
			printf("send error in %s %d\n",__FILE__,__LINE__);
			list_destroy(list_head);
			list_destroy(list);
			return NULL;
		}
		list_destroy(list_head);
		list_head = NULL;
		tmp = tmp->next;
	}
	list_destroy(list);
}


void* marterial_sub(void* argv)
{
	int socketfd = *(int*)argv;
	int ret,n = 0,flag = OK;
	marterial_t mar;
	char str[LENGTH];

	ret = read(socketfd,&n,sizeof(n));
	if(ret < 0)
	{	
		printf("read error in : %s %d\n",__FILE__,__LINE__);
		flag = FAIL;
	}
	else if(ret == 0)
	{
		printf("客户端断开连接\n");
		printf("%s %d\n",__FILE__,__LINE__);
		flag = FAIL;
	}

	printf("n = %d\n",n);/////////////////////////
	for(int i=0;i<n;i++)
	{
		ret = read(socketfd,&mar,sizeof(mar));
		if(ret < 0)
		{	
			printf("read error in : %s %d\n",__FILE__,__LINE__);
			flag = FAIL;
		}
		else if(ret == 0)
		{
			printf("客户端断开连接\n");
			printf("%s %d\n",__FILE__,__LINE__);
			flag = FAIL;
		}

		bzero(str,sizeof(str));
		strcat(str,"update marterial set repertory = repertory-");
		sprintf(str + strlen(str),"%f",mar.repertory);
		strcat(str," where name = '");
		strcat(str,mar.name);
		strcat(str,"'");

		pthread_mutex_lock(&mysql_mutex);
		ret = mysql_query(&mysql,str);
		pthread_mutex_unlock(&mysql_mutex);
		if(ret != 0)
		{
			printf("error in %s %d\n",__FILE__,__LINE__);
			flag = FAIL;
			break;
		}	
	}
	pthread_mutex_lock(&socketfd_mutex);
	package_send(socketfd,0,flag,0,NULL);
	pthread_mutex_unlock(&socketfd_mutex);
}

void* dish_modify_chef(void* argv)
{
	int socketfd = *(int*)argv;
	int ret,flag = OK;
	chef_t chef;
	char str[LENGTH];

	ret = read(socketfd,&chef,sizeof(chef));
	if(ret < 0)
	{	
		printf("read error in : %s %d\n",__FILE__,__LINE__);
		flag = FAIL;
	}
	else if(ret == 0)
	{
		printf("客户端断开连接\n");
		printf("%s %d\n",__FILE__,__LINE__);
		flag = FAIL;
	}

	bzero(str,sizeof(str));
	strcat(str,"update consum_dish set state = '");
	strcat(str,chef.order.state);
	if(chef.flag == MODIFY)
	{
		strcat(str,"',start_time = '");
		strcat(str,chef.order.s_time);
	}
	else if(chef.flag == SAVE)
	{
		strcat(str,"',end_time = '");
		strcat(str,chef.order.e_time);
	}
	strcat(str,"' where con_num = ");
	sprintf(str + strlen(str),"%d",chef.con_num);
	strcat(str," and name = '");
	strcat(str,chef.order.name);
	strcat(str,"'");

	printf("str = %s\n",str);////////////////////////////

	pthread_mutex_lock(&mysql_mutex);
	ret = mysql_query(&mysql,str);
	pthread_mutex_unlock(&mysql_mutex);
	if(ret != 0)
	{
		flag = FAIL;
		printf("error in : %s %d\n",__FILE__,__LINE__);
	}

	if(chef.flag == MODIFY)
	{
		strcpy(str,"update consum set state = '准备中'");
		strcpy(str + strlen(str)," where num = ");
		sprintf(str + strlen(str),"%d",chef.con_num);

		pthread_mutex_lock(&mysql_mutex);
		ret = mysql_query(&mysql,str);
		pthread_mutex_unlock(&mysql_mutex);
		if(ret != 0)
		{
			flag = FAIL;
			printf("error in : %s %d\n",__FILE__,__LINE__);
		}
	}

	package_send(socketfd,0,flag,0,NULL);
}


void* dish_finsh_chef(void* argv)
{
	int socketfd = *(int*)argv;
	int ret,flag = OK;
	char str[LENGTH];
	int con_num;

	ret = read(socketfd,&con_num,sizeof(con_num));
	if(ret < 0)
	{	
		printf("read error in : %s %d\n",__FILE__,__LINE__);
	}
	else if(ret == 0)
	{
		printf("客户端断开连接\n");
		printf("%s %d\n",__FILE__,__LINE__);
	}

	strcpy(str,"update consum set state = '完成' where num = ");
	sprintf(str + strlen(str),"%d",con_num);

	printf("str = %s\n",str);//////////////////////////////////
	pthread_mutex_lock(&mysql_mutex);
	ret = mysql_query(&mysql,str);
	if(ret != 0)
	{
		printf("error in : %s %d\n",__FILE__,__LINE__);
		flag = FAIL;
	}
	pthread_mutex_unlock(&mysql_mutex);

	package_send(socketfd,0,flag,0,NULL);
}

/*****************************************
 *function:消费信息查看
 *****************************************/
void* consum_look(void* argv)
{
	int socketfd = *(int*)argv;
	int ret,n;
	list_t* list;
	char str[LENGTH];
	int flag;

	ret = read(socketfd,&flag,sizeof(flag));
	if(flag == -1)//结帐时
		strcpy(str,"select * from consum where state1 = '未结帐'");
	else if(flag == 1)//历史订单
		strcpy(str,"select * from consum where state1 = '结帐'");
	else if(flag == 0)//查看所有订单	
		strcpy(str,"select * from consum");
	list = look_consum(str,&n);

	printf("num = %d\n",n);/////////////////////////////
	package_list_send(socketfd,0,0,sizeof(consum_t),n,list);
	list_destroy(list);
	
}

/*****************************************
 *function:点菜信息查看
 *****************************************/
void* dish_show(void* argv)
{
	int socketfd = *(int*)argv;
	int ret,n,num;
	list_t* list;
	char str[LENGTH];

	ret = read(socketfd,&num,sizeof(num));
	if(ret < 0)
	{	
		printf("read error in : %s %d\n",__FILE__,__LINE__);
	}
	else if(ret == 0)
	{
		printf("客户端断开连接\n");
		printf("%s %d\n",__FILE__,__LINE__);
	}

	bzero(str,sizeof(str));
	strcat(str,"select * from consum_dish ");
	strcat(str + strlen(str)," where con_num = ");
	sprintf(str + strlen(str),"%d",num);

	list = get_dish(str,&n,0);

	package_list_send(socketfd,0,0,sizeof(chef_t),n,list);
	list_destroy(list);
	
}

/***************************************
 *function:结帐
 ****************************************/
void* consum_pay(void* argv)
{
	int socketfd = *(int*)argv;
	int ret,flag = OK;
	consum_t consum;
	char str[LENGTH];

	ret = read(socketfd,&consum,sizeof(consum));
	if(ret < 0)
	{
		printf("error in : %s %d\n",__FILE__,__LINE__);
		flag = FAIL;
	}
	if(ret == 0)
	{
		printf("客户端断开连接\n");
		return NULL;
	}

	strcpy(str,"update consum set state1 = '结帐',pay_time = '");
	strcat(str,consum.pay_time);
	strcat(str,"' where num = ");
	sprintf(str + strlen(str),"%d",consum.num);

	printf("str = %s\n",str);////////////////////////////////
	pthread_mutex_unlock(&mysql_mutex);
	ret = mysql_query(&mysql,str);
	pthread_mutex_unlock(&mysql_mutex);

	if(ret != 0)
	{
		printf("error in : %s %d\n",__FILE__,__LINE__);
		flag = FAIL;
	}

	package_send(socketfd,0,flag,0,NULL);

}

/****************************************************
 *function:删除历史订单
 ****************************************************/
void* consum_del(void* argv)
{
	int socketfd = *(int*)argv;
	int ret,num,flag = OK;
	pack_head_t head;
	char str[LENGTH];

	ret = read(socketfd,&num,sizeof(num));
	if(ret < 0)
	{
		printf("error in : %s %d\n",__FILE__,__LINE__);
		flag = FAIL;
	}
	if(ret == 0)
	{
		printf("客户端断开连接\n");
		return NULL;
	}

	strcpy(str,"delete from consum where num = ");
	sprintf(str + strlen(str),"%d",num);

	pthread_mutex_lock(&mysql_mutex);
	ret = mysql_query(&mysql,str);
	pthread_mutex_unlock(&mysql_mutex);

	if(ret != 0)
	{
		printf("error in : %s %d\n",__FILE__,__LINE__);
		flag = FAIL;
	}

	package_send(socketfd,0,flag,0,NULL);

}

/**********************************************
 *function:以会员方式结帐（送积分）
 **********************************************/
void* member_pay(void* argv)
{
	int socketfd = *(int*)argv;
	int ret,flag = OK,integral;
	member_t member;
	char str[LENGTH];

	ret = read(socketfd,&member,sizeof(member));
	if(ret < 0)
	{
		printf("error in : %s %d\n",__FILE__,__LINE__);
		flag = FAIL;
	}
	if(ret == 0)
	{
		printf("客户端断开连接\n");
		return NULL;
	}

	strcpy(str,"update member set integral = integral+");
	sprintf(str + strlen(str),"%.1f",member.integral/10);
	strcat(str," where tel = '");
	strcat(str,member.tel);
	strcat(str,"'");

	pthread_mutex_lock(&mysql_mutex);
	ret = mysql_query(&mysql,str);
	pthread_mutex_unlock(&mysql_mutex);

	if(ret != 0)
	{
		printf("error in : %s %d\n",__FILE__,__LINE__);
		flag = FAIL;
	}

	package_send(socketfd,0,flag,0,NULL);
}
