#include "mysql.h"

MYSQL mysql;


/************************************************************
 *function:查看是tableb表中 conlum 是否存在values
 return values:错误-1 存在1 不存在0
 ************************************************************/
int check_str(char* table,char* conlum,char* values)
{
	char str[LENGTH];
	int ret,row;
	MYSQL_RES* res;

	bzero(str,0);
	strcpy(str,"select * from ");
	strcpy(str + strlen(str),table);
	strcpy(str + strlen(str)," where ");
	strcpy(str + strlen(str),conlum);
	strcpy(str + strlen(str)," = '");
	strcpy(str + strlen(str),values);
	strcpy(str + strlen(str),"'");

	printf("str = %s\n",str);////////////////////
	pthread_mutex_lock(&mysql_mutex);
	ret = mysql_query(&mysql,str);
	if(ret != 0)
	{
		printf("error in :%s %d\n",__FILE__,__LINE__);
		pthread_mutex_unlock(&mysql_mutex);
		return -1;
	}

	res = mysql_store_result(&mysql);
	pthread_mutex_unlock(&mysql_mutex);
	row = mysql_num_rows(res);
	printf("row = %d\n",row);//////////////////
	if(row == 0)
		return 0;
	else 
		return 1;
}

/******************************************************
 *function:获取最大编号
 ******************************************************/
int get_maxnum(char* table,char* depart)
{
	char str[LENGTH];
	int ret,row,field,num = 0;
	MYSQL_RES* res;
	MYSQL_ROW r;

	bzero(str,0);
	strcpy(str,"select * from ");
	strcpy(str + strlen(str),table);
	strcpy(str + strlen(str)," where position = '");
	strcpy(str + strlen(str),depart);
	strcpy(str + strlen(str),"'");

	pthread_mutex_lock(&mysql_mutex);
	ret = mysql_query(&mysql,str);
	if(ret != 0)
	{
		printf("error in :%s %d\n",__FILE__,__LINE__);
		pthread_mutex_unlock(&mysql_mutex);
		return -1;
	}

	res = mysql_store_result(&mysql);
	pthread_mutex_unlock(&mysql_mutex);
	if(res == NULL)
	{
		printf("error in :%s %d\n",__FILE__,__LINE__);
		return -1;
	}
	
	row = mysql_num_rows(res);
	field = mysql_num_fields(res);
	for(int i=0;i<row;i++)
	{
		r = mysql_fetch_row(res);
		if(num < atoi(r[0]))
			num = atoi(r[0]);
	}
	return num;
}

/******************************************************
 *function:获取最大编号
 ******************************************************/
int get_max_num(char* table)
{
	char str[LENGTH];
	int ret,row,field,num = 0;
	MYSQL_RES* res;
	MYSQL_ROW r;

	bzero(str,0);
	strcpy(str,"select * from ");
	strcpy(str + strlen(str),table);

	pthread_mutex_lock(&mysql_mutex);
	ret = mysql_query(&mysql,str);
	if(ret != 0)
	{
		printf("error in :%s %d\n",__FILE__,__LINE__);
		pthread_mutex_unlock(&mysql_mutex);
		return -1;
	}

	res = mysql_store_result(&mysql);
	pthread_mutex_unlock(&mysql_mutex);
	if(res == NULL)
	{
		printf("error in :%s %d\n",__FILE__,__LINE__);
		return -1;
	}
	
	row = mysql_num_rows(res);
	field = mysql_num_fields(res);
	for(int i=0;i<row;i++)
	{
		r = mysql_fetch_row(res);
		if(num < atoi(r[0]))
			num = atoi(r[0]);
	}
	return num;
}
/**************************************************************
 *function:插入员工信息
 **************************************************************/
int employee_insert(regist_t* reg)
{
	char str[LENGTH];
	char length;
	int ret,num;


	//获取编号
	num = get_maxnum("employee",reg->depart);
	if(num < 0)
		return -1;
	else if(num == 0)
	{
		if(!strcmp(reg->depart,"服务员"))
			num = WAITER * 1000;
		if(!strcmp(reg->depart,"厨师"))
			num = CHEF * 1000;
	
	}
	else
		num += 1;
/********************设置mysql命令***********************/

	strcpy(str,"insert into ");
	strcpy(str + strlen(str),"employee values(");
	sprintf(str + strlen(str),"%d",num);
	strcpy(str + strlen(str),",'");
	strcpy(str + strlen(str),reg->account);
	strcpy(str + strlen(str),"','");
	strcpy(str + strlen(str),reg->passward);
	strcpy(str + strlen(str),"','");
	strcpy(str + strlen(str),reg->name);
	strcpy(str + strlen(str),"',");
	sprintf(str + strlen(str),"%d",reg->age);
	strcpy(str + strlen(str),",'");
	strcpy(str + strlen(str),reg->sex);
	strcpy(str + strlen(str),"','");
	strcpy(str + strlen(str),reg->tel);
	strcpy(str + strlen(str),"','");
	strcpy(str + strlen(str),reg->address);
	strcpy(str + strlen(str),"','");
	strcpy(str + strlen(str),reg->id);
	strcpy(str + strlen(str),"',");
	sprintf(str + strlen(str),"%.1f",reg->salary);
	strcpy(str + strlen(str),",'");
	strcpy(str + strlen(str),reg->depart);
	strcpy(str + strlen(str),"')");

	printf("str = %s\n",str);
	//插入命令
	pthread_mutex_lock(&mysql_mutex);
	ret = mysql_query(&mysql,str);
	pthread_mutex_unlock(&mysql_mutex);
	return ret;
}


/****************************************************************
 *function:读取员工信息
 ****************************************************************/
list_t* employee_look(char* table,int* num)
{
	int ret,row,field;
 	list_t* head = NULL;
	regist_t employee;
	char str[LENGTH];
	MYSQL_RES* res;
	MYSQL_ROW r;

	strcpy(str,"select * from ");
	strcpy(str + strlen(str),table);
	strcat(str," where position != '管理员'");

	printf("str = %s\n",str);/////////////////////////////////////
	pthread_mutex_lock(&mysql_mutex);
	ret = mysql_query(&mysql,str);
	if(ret != 0)
	{
		printf("error in : %s %d\n",__FILE__,__LINE__);
		*num = 0;
		pthread_mutex_unlock(&mysql_mutex);
		return NULL;
	}

	res = mysql_store_result(&mysql);
	pthread_mutex_unlock(&mysql_mutex);

	row = mysql_num_rows(res);
	field = mysql_num_fields(res);
	for(int i=0;i<row;i++)
	{
		r = mysql_fetch_row(res);
		for(int j=0;j<field;j++)
		{
			switch(j)
			{
				case 0:
					employee.num = atoi(r[j]);
					break;
				case 1:
					strcpy(employee.account,r[j]);
					break;
				case 2:
					strcpy(employee.passward,r[j]);
					break;
				case 3:
					strcpy(employee.name,r[j]);	
					break;
				case 4:
					employee.age = atoi(r[j]);	
					break;
				case 5:
					strcpy(employee.sex,r[j]);	
					break;
				case 6:
					strcpy(employee.tel,r[j]);	
					break;
				case 7:
					strcpy(employee.address,r[j]);	
					break;
				case 8:
					strcpy(employee.id,r[j]);	
					break;
				case 9:
					employee.salary = atof(r[j]);	
					break;
				case 10:
					strcpy(employee.depart,r[j]);	
					break;
				default:
					break;
			}
		}
		head = list_create(head,&employee,sizeof(employee));
	}
	*num = row;
	return head;
}

/****************************************************************
 *function:读取会员信息
 ****************************************************************/
list_t* member_look(char* table,int* num)
{
	int ret,row,field;
 	list_t* head = NULL;
	member_t member;
	char str[LENGTH];
	MYSQL_RES* res;
	MYSQL_ROW r;

	strcpy(str,"select * from ");
	strcpy(str + strlen(str),table);

//	printf("str = %s\n",str);/////////////////////////////////////
	pthread_mutex_lock(&mysql_mutex);
	ret = mysql_query(&mysql,str);
	if(ret != 0)
	{
		printf("error in : %s %d\n",__FILE__,__LINE__);
		*num = 0;
		pthread_mutex_unlock(&mysql_mutex);
		return NULL;
	}

	res = mysql_store_result(&mysql);
	pthread_mutex_unlock(&mysql_mutex);

	row = mysql_num_rows(res);
	field = mysql_num_fields(res);
	for(int i=0;i<row;i++)
	{
		r = mysql_fetch_row(res);
		for(int j=0;j<field;j++)
		{
			switch(j)
			{
				case 0:
					member.num = atoi(r[j]);
					break;
				case 1:
					strcpy(member.name,r[j]);
					break;
				case 2:
					strcpy(member.sex,r[j]);
					break;
				case 3:
					member.age = atoi(r[j]);	
					break;
				case 4:
					strcpy(member.tel,r[j]);	
					break;
				case 5:
					strcpy(member.id,r[j]);	
					break;
				case 6:
					strcpy(member.passward,r[j]);	
					break;
				case 7:
					member.integral = atof(r[j]);	
					break;
				default:
					break;
			}
		}
		head = list_create(head,&member,sizeof(member));
	}
	*num = row;
	return head;
}

/************************************************************
 *function:员工登录验证
 ************************************************************/
int employee_eroll(char* table,char* account,char* pass,
		char* depart,int flag)
{
	int ret,row,field;
	char str[LENGTH];
	MYSQL_RES* res;
	MYSQL_ROW r;

	strcpy(str,"select * from ");
	strcpy(str + strlen(str),table);
	if(flag == MEMBER)
		strcpy(str + strlen(str)," where tel = '");
	else
		strcpy(str + strlen(str)," where account = '");
	strcpy(str + strlen(str),account);
	strcpy(str + strlen(str),"' and passward = '");
	strcpy(str + strlen(str),pass);
	strcpy(str + strlen(str),"'");
	if(flag != MEMBER)
	{
		strcpy(str + strlen(str)," and position = '");
		strcpy(str + strlen(str),depart);
		strcpy(str + strlen(str),"'");
	}

//	printf("str = %s\n",str);/////////////////////////////////
	pthread_mutex_lock(&mysql_mutex);
	ret = mysql_query(&mysql,str);
	if(ret != 0)
	{
		printf("error in : %s %d\n",__FILE__,__LINE__);
		pthread_mutex_unlock(&mysql_mutex);
		return -1;
	}

	res = mysql_store_result(&mysql);
	pthread_mutex_unlock(&mysql_mutex);
	if(res == NULL)
	{
		printf("error in : %s %d\n",__FILE__,__LINE__);
		return -1;
	}
	row = mysql_num_rows(res);
	field = mysql_num_fields(res);
	for(int i=0;i<row;i++)
	{
		r = mysql_fetch_row(res);
		for(int j=0;j<field;j++)
			return 0;
	}
	return -1;
}

/*******************************************************
 *function:插入菜品
 *******************************************************/
int dishes_insert(char* class_name,char* name,float price)
{
	char str[LENGTH];
	int num,ret;

	num = get_max_num("dishes");
	if(num < 0)
		return -1;
	else if(num == 0)
		num = FOOD * 1000;
	else
		num +=1;

	/*************设置mysql命令***********/
	strcpy(str,"insert into dishes values(");
	sprintf(str + strlen(str),"%d",num);
	strcpy(str + strlen(str),",'");
	strcpy(str + strlen(str),class_name);
	strcpy(str + strlen(str),"','");
	strcpy(str + strlen(str),name);
	strcpy(str + strlen(str),"',");
	sprintf(str + strlen(str),"%.1f",price);
	strcpy(str + strlen(str),")");

//	printf("str = %s\n",str);//////////////////////////////////

	pthread_mutex_lock(&mysql_mutex);
	ret = mysql_query(&mysql,str);
	pthread_mutex_unlock(&mysql_mutex);
	if(ret != 0)
	{
		printf("error in : %s %d\n",__FILE__,__LINE__);
		return -1;
	}
	return 0;
}

/************************************************
 *function:读取菜式信息
 ************************************************/
list_t* dish_search(char* table,char* conlum,char* name,int* num)
{
	int ret,row,field;
 	list_t* head = NULL;
	dishes_t dish;
	char str[LENGTH];
	MYSQL_RES* res;
	MYSQL_ROW r;

	strcpy(str,"select * from ");
	strcpy(str + strlen(str),table);

	if(strcmp(name,"NULL"))
	{
		strcpy(str + strlen(str)," where ");
		strcpy(str + strlen(str),conlum);
		strcpy(str + strlen(str)," = '");
		strcpy(str + strlen(str),name);
		strcpy(str + strlen(str),"'");
	}

	pthread_mutex_lock(&mysql_mutex);
	ret = mysql_query(&mysql,str);
	if(ret != 0)
	{
		printf("error in : %s %d\n",__FILE__,__LINE__);
		*num = 0;
		pthread_mutex_unlock(&mysql_mutex);
		return NULL;
	}

	res = mysql_store_result(&mysql);
	pthread_mutex_unlock(&mysql_mutex);

	row = mysql_num_rows(res);
	field = mysql_num_fields(res);
	for(int i=0;i<row;i++)
	{
		r = mysql_fetch_row(res);
		for(int j=0;j<field;j++)
		{
			switch(j)
			{
				case 0:
					dish.num = atoi(r[j]);
					break;
				case 1:
					strcpy(dish.class_name,r[j]);
					break;
				case 2:
					strcpy(dish.name,r[j]);
					break;
				case 3:
					dish.price = atof(r[j]);
					break;
				default:
					break;
			}
		}
		head = list_create(head,&dish,sizeof(dish));
	}
	*num = row;
	return head;
}

/**********************************************
 *function:读取原材料
 **********************************************/
list_t* marterial_search(char* table,char* conlum,char* name,int* num)
{
	int ret,row,field;
 	list_t* head = NULL;
	marterial_t marterial;
	char str[LENGTH];
	MYSQL_RES* res;
	MYSQL_ROW r;

	strcpy(str,"select * from ");
	strcpy(str + strlen(str),table);
	strcpy(str + strlen(str)," where ");
	strcpy(str + strlen(str),conlum);
	strcpy(str + strlen(str)," = '");
	strcpy(str + strlen(str),name);
	strcpy(str + strlen(str),"'");

//	printf("str = %s\n",str);/////////////////////////////////////
	pthread_mutex_lock(&mysql_mutex);
	ret = mysql_query(&mysql,str);
	if(ret != 0)
	{
		printf("error in : %s %d\n",__FILE__,__LINE__);
		*num = 0;
		pthread_mutex_unlock(&mysql_mutex);
		return NULL;
	}

	res = mysql_store_result(&mysql);
	pthread_mutex_unlock(&mysql_mutex);

	row = mysql_num_rows(res);
	field = mysql_num_fields(res);
	for(int i=0;i<row;i++)
	{
		r = mysql_fetch_row(res);
		for(int j=0;j<field;j++)
		{
			switch(j)
			{
				case 0:
					marterial.num = atoi(r[j]);
					break;
				case 1:
					strcpy(marterial.name,r[j]);
					break;
				case 2:
					marterial.repertory = atof(r[j]);
					break;
				case 3:
					marterial.price = atof(r[j]);
					break;
				default:
					break;
			}
		}
		head = list_create(head,&marterial,sizeof(marterial));
	}
	*num = row;
	return head;
}
/*********************************************
 *function:查看对应桌号上是有客人订餐
 return values: 0没有  1有
 *********************************************/
int check_tablenum(char* table,int num)
{
	char str[LENGTH];
	int ret,row;
	MYSQL_RES* res;

	bzero(str,0);
	strcpy(str,"select * from ");
	strcpy(str + strlen(str),table);
	strcpy(str + strlen(str)," where table_num = ");
	sprintf(str + strlen(str),"%d",num);
	strcpy(str + strlen(str)," and (state1 = '未结帐' or state != '完成')");

	printf("str = %s\n",str);///////////////////////////////

	pthread_mutex_lock(&mysql_mutex);
	ret = mysql_query(&mysql,str);
	if(ret != 0)
	{
		printf("error in :%s %d\n",__FILE__,__LINE__);
		pthread_mutex_unlock(&mysql_mutex);
		return -1;
	}

	res = mysql_store_result(&mysql);
	pthread_mutex_unlock(&mysql_mutex);
	row = mysql_num_rows(res);
	if(row == 0)
		return 0;
	else 
		return 1;
}

/**************************************
 *function:获取纪录数量
 **************************************/
int get_num(char* str)
{
	int ret,row,num = 0;
	MYSQL_RES* res;

	pthread_mutex_lock(&mysql_mutex);
	ret = mysql_query(&mysql,str);
	if(ret != 0)
	{
		printf("error in : %s %d\n",__FILE__,__LINE__);
		pthread_mutex_unlock(&mysql_mutex);
		return num;
	}

	res = mysql_store_result(&mysql);
	pthread_mutex_unlock(&mysql_mutex);
	row = mysql_num_rows(res);
	num = row;
	return num;
}

/***********************************************
 *function:读消费信息
 **********************************************/
list_t* get_consum(char* str,int* num)
{
	list_t* list = NULL;
	consum_t consum;
	int ret,row,field;
	MYSQL_RES* res;
	MYSQL_ROW r;

	pthread_mutex_lock(&mysql_mutex);
	ret = mysql_query(&mysql,str);
	if(ret != 0)
	{
		printf("error in : %s %d\n",__FILE__,__LINE__);
		*num = 0;
		pthread_mutex_unlock(&mysql_mutex);
		return NULL;
	}

	res = mysql_store_result(&mysql);
	pthread_mutex_unlock(&mysql_mutex);

	row = mysql_num_rows(res);
	*num = row;
	field = mysql_num_fields(res);
	for(int i=0;i<row;i++)
	{
		r = mysql_fetch_row(res);
		for(int j=0;j<field;j++)
		{
			switch(j)
			{
				case 0:
					consum.num = atoi(r[0]);
					break;
				case 1:
					consum.table_num = atoi(r[j]);
					break;
				case 2:
					consum.dish_num = atoi(r[j]);
					break;
				case 3:
					strcpy(consum.state,r[j]);
					break;
				case 4:
					consum.price = atof(r[j]);
					break;
				case 5:
					strcpy(consum.submit_time,r[j]);
					break;
				case 6:
					strcpy(consum.pay_time,r[j]);
					break;
				case 7:
					strcpy(consum.date,r[j]);
					break;
				case 8:
					strcpy(consum.state1,r[j]);
					break;
				default:
					break;
			}
		}
		list = list_create(list,&consum,sizeof(consum));
	}
	return list;
}

/****************************************************
 *function:获取对应消费编号的菜式信息
 ****************************************************/
list_t* get_dish(char* str,int* num,int n)
{
	int ret,row,field;
 	list_t* head = NULL;
	chef_t chef;
	MYSQL_RES* res;
	MYSQL_ROW r;

	pthread_mutex_lock(&mysql_mutex);
	ret = mysql_query(&mysql,str);
	if(ret != 0)
	{
		printf("error in : %s %d\n",__FILE__,__LINE__);
		*num = 0;
		pthread_mutex_unlock(&mysql_mutex);
		return NULL;
	}

	res = mysql_store_result(&mysql);
	pthread_mutex_unlock(&mysql_mutex);

	row = mysql_num_rows(res);
	field = mysql_num_fields(res);
	*num = row;

	for(int i=0;i<row;i++)
	{
		r = mysql_fetch_row(res);
		for(int j=0;j<field;j++)
		{
			chef.table_num = n;
			switch(j)
			{
				case 0:
					chef.order.num = atoi(r[j]);
					break;
				case 1:
					strcpy(chef.order.class_name,r[j]);
					break;
				case 2:
					strcpy(chef.order.name,r[j]);
					break;
				case 3:
					chef.order.price = atoi(r[j]);
					break;
				case 4:
					strcpy(chef.order.s_time,r[j]);
					break;
				case 5:
					strcpy(chef.order.e_time,r[j]);
					break;
				case 6:
					chef.con_num = atoi(r[j]);
					break;
				case 7:
					chef.order.dish_num = atoi(r[j]);
					break;
				case 8:
					strcpy(chef.order.state,r[j]);
					break;
				default:
					break;
			}
		}
		head = list_create(head,&chef,sizeof(chef));
	}
	return head;
}

/****************************************************
 *function:获取对应消费编号的菜式信息
 ****************************************************/
list_t* get_dish_waiter(char* str,int* num)
{
	int ret,row,field;
 	list_t* head = NULL;
	order_t order;
	MYSQL_RES* res;
	MYSQL_ROW r;

	pthread_mutex_lock(&mysql_mutex);
	ret = mysql_query(&mysql,str);
	if(ret != 0)
	{
		printf("error in : %s %d\n",__FILE__,__LINE__);
		*num = 0;
		pthread_mutex_unlock(&mysql_mutex);
		return NULL;
	}

	res = mysql_store_result(&mysql);
	pthread_mutex_unlock(&mysql_mutex);

	row = mysql_num_rows(res);
	field = mysql_num_fields(res);
	*num = row;

	for(int i=0;i<row;i++)
	{
		r = mysql_fetch_row(res);
		for(int j=0;j<field;j++)
		{
			switch(j)
			{
				case 0:
					order.num = atoi(r[j]);
					break;
				case 1:
					strcpy(order.class_name,r[j]);
					break;
				case 2:
					strcpy(order.name,r[j]);
					break;
				case 3:
					order.price = atoi(r[j]);
					break;
				case 4:
					strcpy(order.s_time,r[j]);
					break;
				case 5:
					strcpy(order.e_time,r[j]);
					break;
			//	case 6:
			//		chef.con_num = atoi(r[j]);
			//		break;
				case 7:
					order.dish_num = atoi(r[j]);
					break;
				case 8:
					strcpy(order.state,r[j]);
					break;
				default:
					break;
			}
		}
		head = list_create(head,&order,sizeof(order));
	}
	return head;
}

/************************************************
 *function:查看消费信息
 ************************************************/
list_t* look_consum(char* str,int* num)
{
	int ret,row,field;
 	list_t* head = NULL;
	consum_t consum;
	MYSQL_RES* res;
	MYSQL_ROW r;

	printf("str = %s\n",str);///////////////////////////////
	pthread_mutex_lock(&mysql_mutex);
	ret = mysql_query(&mysql,str);
	if(ret != 0)
	{
		printf("error in : %s %d\n",__FILE__,__LINE__);
		*num = 0;
		pthread_mutex_unlock(&mysql_mutex);
		return NULL;
	}

	res = mysql_store_result(&mysql);
	pthread_mutex_unlock(&mysql_mutex);

	row = mysql_num_rows(res);
	field = mysql_num_fields(res);
	*num = row;

	for(int i=0;i<row;i++)
	{
		r = mysql_fetch_row(res);
		for(int j=0;j<field;j++)
		{
			switch(j)
			{
				case 0:
					consum.num = atoi(r[j]);
					break;
				case 1:
					consum.table_num = atoi(r[j]);
					break;
				case 2:
					consum.dish_num = atoi(r[j]);
					break;
				case 3:
					 strcpy(consum.state,r[j]);
					break;
				case 4:
					consum.price = atoi(r[j]);
					break;
				case 5:
					strcpy(consum.submit_time,r[j]);
					break;
				case 6:
					strcpy(consum.pay_time,r[j]);
					break;
				case 7:
					strcpy(consum.date,r[j]);
					break;
				case 8:
					strcpy(consum.state1,r[j]);
					break;
				default:
					break;
			}
		}
		head = list_create(head,&consum,sizeof(consum));
	}
	return head;
}
 
/*********************************************************
 *function:获取会员的积分信息
 ********************************************************/
int get_integral(char* table,char* conlum,char* account)
{
	char str[LENGTH];
	int ret;
	int row,field;
	MYSQL_RES* res;
	MYSQL_ROW r;

	strcpy(str,"select * from ");
	strcat(str,table);
	strcat(str," where ");
	strcat(str,conlum);
	strcat(str," = '");
	strcat(str,account);
	strcat(str,"'");

	printf("str = %s\n",str);////////////////////////////
	pthread_mutex_lock(&mysql_mutex);
	ret = mysql_query(&mysql,str);
	if(ret != 0)
	{
		printf("error in : %s %d\n",__FILE__,__LINE__);
		pthread_mutex_unlock(&mysql_mutex);
		return -1;
	}

	res = mysql_store_result(&mysql);
	pthread_mutex_unlock(&mysql_mutex);

	row = mysql_num_rows(res);
	field = mysql_num_fields(res);

	for(int i=0;i<row;i++)
	{
		r = mysql_fetch_row(res);
		for(int j=0;j<field;j++)
		{
			if(j == 7)
				return (atoi(r[j]));
		}
	}
	return -1;
}

