#include "commen.h"

/*******************************************
 *function:从配置文件中读取数据
 *******************************************/
int getcnf(char* p,char* data)
{
	FILE* fp;
	char buff[200],key[20];

	fp = fopen("net.conf","r");
	if(fp == NULL)
		return -1;

	while(!feof(fp))//判断文件是否到末尾
	{
		fgets(buff,sizeof(buff),fp);
		sscanf(buff,"%s",key);
		if(!strcmp(key,p))
		{
			sscanf(buff,"%*s%*s%s",data);
			break;
		}
	}
	return 0;
}

/********************************************
 *function:从指定位置输出(相对位置)
 ********************************************/
void position(int x,int y,int z)
{
	if(x < 0 || y < 0 || z < 0)
		return;

	for(int i=0;i<x;i++)
	{
		printf("\n");
	}

	for(int i=0;i<y;i++)
	{
		printf("\t");
	}

	for(int i=0;i<z;i++)
	{
		printf(" ");
	}
}


/**************************************************
 *function:确保两次输入的密码匹配
 **************************************************/
void set_pass(char* pass)
{
	char pass1[10],pass2[10];
	for(;;)
	{
		//第一次
		position(1,5,0);
		printf("密    码：");
		scanf("%s",pass1);
		//hide_passward(pass1);
		while(getchar() != '\n');
		if(strlen(pass1) >= PASSWARD)
		{
			position(1,0,0);
			printf("密码长度不符");
			continue;
		}
		//第二次
		for(;;)
		{
			position(1,5,0);
			printf("输入密码：");
			scanf("%s",pass2);
			//hide_passward(pass2);
			while(getchar() != '\n');
			if(strlen(pass2) >= PASSWARD)
			{
				position(1,0,0);
				printf("密码长度不符");
				continue;
			}
			else 
				break;
		}
		//比较两次密码是否一致
		if(!strcmp(pass1,pass2))
			break;
		else
		{
			position(1,0,0);
			printf("两次输入的密码不匹配");
		}
	}
	strcpy(pass,pass1);
}

/**********************************************
 *function:获取系统时间
 **********************************************/
void count_time(char* date,char* times)
{
	int sec,min,hour,day,mon,year;
	time_t rawtime;
	struct tm* timeinfo;

	time(&rawtime);
	timeinfo = localtime(&rawtime);
	sec = timeinfo->tm_sec;
	min = timeinfo->tm_min;
	hour = timeinfo->tm_hour;
	day = timeinfo->tm_mday;
	mon = timeinfo->tm_mon;
	year = timeinfo->tm_year+1900;//获取的系统函数年份 = year-1900
	if(date != NULL)
	{
		bzero(date,strlen(date));
		//保存日期
		sprintf(date,"%d",year);
		strcpy(date + strlen(date),"-");
		if(mon<10)
		{
			strcpy(date + strlen(date),"0");
		}
		sprintf(date + strlen(date),"%d",mon);
		strcpy(date + strlen(date),"-");
		if(day<10)
		{
			strcpy(date + strlen(date),"0");
		}
		sprintf(date + strlen(date),"%d",day);
	}

	if(times != NULL)
	{
		bzero(times,strlen(times));
		//保存时间
		if(hour < 10)
		{
			sprintf(times,"0");
		}
		sprintf(times + strlen(times),"%d",hour);
		strcpy(times + strlen(times),":");
		if(min<10)
		{
			strcpy(times + strlen(times),"0");
		}
		sprintf(times + strlen(times),"%d",min);
		strcpy(times + strlen(times),":");

		if(sec<10)
		{
			strcpy(times + strlen(times),"0");
		}
		sprintf(times + strlen(times),"%d",sec);
	}
}

/****************************************************
 *function:获取系统时间
 ***************************************************/
void hour_min_sec(int *hour,int *min,int *sec)
{
	time_t rawtime;
	struct tm* timeinfo;

	time(&rawtime);
	timeinfo = localtime(&rawtime);
	*sec = timeinfo->tm_sec;
	*min = timeinfo->tm_min;
	*hour = timeinfo->tm_hour;
}


/**********************************************************
 *function:建立泛型链表(尾查法)
 **********************************************************/
list_t* list_create(list_t* head,void* data,int size)
{
	list_t* tmp = malloc(sizeof(list_t)),*temp = head;
	if(tmp == NULL)
		return NULL;

	tmp->data = malloc(size);
	if(tmp->data == NULL)
	{
		free(tmp);
		return NULL;
	}

	memcpy(tmp->data,data,size);
	tmp->next = NULL;

	if(temp == NULL)
	{
		head = tmp;
		return head;
	}

	while(temp->next != NULL)
		temp = temp->next;

	temp->next = tmp;
	return head;
}

/******************************************************
 *function:链表前插
 ******************************************************/
list_t* list_insert(list_t* head,void* data,int size)
{
	list_t* tmp = malloc(size);
	assert(tmp != NULL);

	memcpy(tmp,data,size);
	tmp->next = head;
	head = tmp;
}
/***************************************************
 *function:查看链表
 func是函数指针
 **************************************************/
list_t* list_show(list_t* head,void(*func)(void*))
{
	list_t* tmp;
	if(head == NULL)
		return NULL;

	while(head != NULL)
	{
		tmp = head;
		if(head->data != NULL)
			func(head->data);
		head = head->next;
	}
	return tmp;
}

/********************************************************
 *function:通过函数指针比较链表中是否存在数据
 return values:不存在返回NULL，存在返回数据地址
 ********************************************************/
void* list_compare(list_t* head,void* data,int(*compare)(void*,void*))
{
	if(head == NULL || data == NULL)
		return NULL;

	while(head != NULL)
	{
		if(head->data != NULL)
		{
			if(!compare(data,head->data))
			{
				return head->data;
			}
		}
		head = head->next;
	}
	return NULL;
}

/********************************************************
 *function:通过函数指针比较链表中是否存在数据
 return values:不存在返回NULL，存在返回数据地址
 ********************************************************/
void* list_comp(list_t* head,void* data1,void* data2,int(*compare)(void*,void*,void*))
{
	if(head == NULL || data1 == NULL || data2 == NULL)
		return NULL;

	while(head != NULL)
	{
		if(head->data != NULL)
		{
			if(!compare(data1,data2,head->data))
			{
				return head->data;
			}
		}
		head = head->next;
	}
	return NULL;
}

/**************************************************
 *functiuon:删除链表接节点
 **************************************************/
list_t* list_node_del(list_t* head,void* data,int(*func)(void*,void*))
{
	if(head == NULL  || data == NULL)
		return head;

	void* tmp = list_compare(head,data,func);
	list_t *temp,*tm;

	if(head->data == tmp)
	{
		temp = head;
		head = head->next;
		free(temp->data);
		free(temp);
	}
	else
	{
		temp = head;
		while(temp->next != NULL)
		{
			if(temp->next->data == tmp)
			{
				tm = temp->next;
				temp->next = tm->next;
				free(tm->data);
				free(tm);
				break;
			}
			temp = temp->next;
		}
	}
	return head;
}

/**************************************************
 *function:销毁链表
 **************************************************/
void list_destroy(list_t* head)
{
	list_t* tmp = head;
	if(tmp == NULL)
		return;

	while(head != NULL)
	{
		tmp = head;
		head = head->next;
		if(tmp->data != NULL)
			free(tmp->data);
		free(tmp);
	}
}
int hide_passward(char *passwd)//输入密码，并不回显
{
	//printf("\t请输入密码\n");
	char ch;
	int i = 0;
	setbuf(stdin,NULL);
	system("stty -echo raw");
	while(1)//输入密码
	{
		ch = getchar();
		if(ch == '\r')
		{
			break;
		}
		putchar('*');
		passwd[i] = ch;
		i++;
		if(i>=6)
			break;
	}
	system("stty echo -raw");
	passwd[i] = '\0';
	putchar('\n');
	return 0;
}
