#include "chef_func.h"

void head_menu()
{
	system("clear");
	position(1,3,0);
	printf("************************************************************");	
	position(1,3,0);
	printf("***********  酒    店    管    理    系    统  *************");
	position(1,3,0);
	printf("***********                                    *************");	
	position(1,3,0);
	printf("***********        厨    房    终    端        *************");	
	position(1,3,0);
	printf("************************************************************");
}

void main_menu()
{
	head_menu();
	position(1,5,6);
	printf("请 输 入 选 项");
	position(1,4,1);
	printf("-----------------------------------------");
	position(1,4,1);
	printf("|          1 登             录          |");
	position(1,4,1);
	printf("|                                       |");
	position(1,4,1);
	printf("|          0 退             出          |");
	position(1,4,1);
	printf("-----------------------------------------");
	position(1,0,0);
}

void man_menu()
{
	head_menu();
	position(1,5,6);
	printf("请 输 入 选 项");
	position(1,4,1);
	printf("-----------------------------------------");
	position(1,4,1);
	printf("|          1 准   备   菜   式          |");
	position(1,4,1);
	printf("|                                       |");
	position(1,4,1);
	printf("|          2 完   成   菜   式          |");
	position(1,4,1);
	printf("|                                       |");
	position(1,4,1);
	printf("|          0 退             出          |");
	position(1,4,1);
	printf("-----------------------------------------");
	position(1,0,0);
}

void marter_menu()
{
	position(1,1,0);
	printf("1 调  料");
	position(0,1,0);
	printf("2 面  粉");
	position(0,1,0);
	printf("3 米");
	position(0,2,0);
	printf("4 酒  水");
	position(2,1,0);
	printf("5 土  豆");
	position(0,1,0);
	printf("6 青  菜");
	position(0,1,0);
	printf("7 黄  瓜");
	position(0,1,0);
	printf("8 西红柿");
	position(2,1,0);
	printf("9 莴  笋");
	position(0,1,0);
	printf("a 青  椒");
	position(0,1,0);
	printf("b 木  耳");
	position(0,1,0);
	printf("c 豆  腐");
	position(2,1,0);
	printf("d 鸡  蛋");
	position(0,1,0);
	printf("e 蒜  苔");
	position(0,1,0);
	printf("f 豆  角");
	position(0,1,0);
	printf("g 茄  子");
	position(2,1,0);
	printf("h 鸡  肉");
	position(0,1,0);
	printf("i 猪  肉");
	position(0,1,0);
	printf("j 鱼  肉");
	position(0,1,0);
	printf("k 肥  肠");
}

void main_fun(int socketfd)
{
	int ret;
	//	setbuf(stdin,NULL);
	for(char select = '1';select != '0';)
	{
		main_menu();
		scanf("%c",&select);
		while(getchar() != '\n');

		switch(select)
		{
			case ENTER:
				ret = login(socketfd);
				if(ret == 0)
					man_fun(socketfd);
				break;
			case EXIT:
				select = EXIT;
				system("clear");
				break;
			default:
				printf("选项不存在\n");
				sleep(1);
				break;
		}
	}
}

void man_fun(int socketfd)
{
	for(char select = '1';select != '0';)
	{
		man_menu();
		scanf("%c",&select);
		while(getchar() != '\n');

		switch(select)
		{
			case PREPARE:
				dish_prepare(socketfd);
				break;
			case FINSH:
				dish_finsh(socketfd);
				break;
			case EXIT:
				select = EXIT;
				break;
			default:
				printf("选项不存在\n");
				sleep(1);
				break;
		}
	}
}

int login(int socketfd)
{
	int ret;
	pack_head_t head;
	login_t data;

	system("clear");
	position(5,5,0);
	printf("帐     号：");
	scanf("%s",data.account);
	position(1,5,0);
	printf("密     码：");
	scanf("%s",data.passward);
	while(getchar() != '\n');

	data.flag = CHEF;

	for(;;)
	{
		ret = package_send(socketfd,CHEF,LOGIN,sizeof(data),&data);
		if(ret < 0)
		{
			usleep(100*1000);
			continue;
		}
		break;
	}

	for(;;)
	{
		ret = read(socketfd,&head,sizeof(head));
		if(ret < 0)
		{
			perror("read");
			position(1,0,0);
			printf("error in :%s %d\n",__FILE__,__LINE__);
			usleep(100*1000);
			continue;
		}
		else if(ret == 0)
		{
			printf("与服务器断开连接,登录失败\n");
			sleep(1);
			exit(0);
		}

		if(head.opt == OK)
		{
			position(1,0,0);
			printf("登录成功\n");
			ret = 0;
		}
		else if(head.opt == FAIL)
		{
			position(1,0,0);
			printf("登录失败\n");
			ret = -1;
		}
		sleep(1);
		return ret;
	}
}


list_t* look_dish(int socketfd,int flag1)
{
	int ret,n,m,flag;
	pack_head_t head;
	list_t* list = NULL, *tmp = NULL;
	chef_t chef;

	ret = package_send(socketfd,ORDER,L_K,sizeof(flag1),&flag1);
	if(ret < 0)
	{
		printf("菜式查看错误!");
		printf("error in : %s %d\n",__FILE__,__LINE__);
		//	pthread_exit(0);
		return NULL;
	}

	read(socketfd,&head,sizeof(head));
	ret = read(socketfd,&m,sizeof(m));//消费数量

	//	printf("num = %d\n",m);/////////////////////////////////////////////

	if(ret < 0)
	{
		printf("数据读取错误!\n");
		printf("error in : %s %d\n",__FILE__,__LINE__);
		//	pthread_exit(0);
		return NULL;
	}
	else if(ret == 0)
	{
		printf("与服务器断开连接!\n");	
		printf("%s %d\n",__FILE__,__LINE__);
		//	pthread_exit(0);
		return NULL;
	}

	for(;m;m--)//点菜的菜桌数量
	{
		read(socketfd,&head,sizeof(head));
		ret = read(socketfd,&n,sizeof(n));//每桌的菜式数量
		if(ret < 0)
		{
			printf("数据读取错误!\n");
			printf("error in : %s %d\n",__FILE__,__LINE__);
			list_destroy(list);
			return NULL;
		}
		else if(ret == 0)
		{
			printf("与服务器断开连接!\n");	
			printf("%s %d\n",__FILE__,__LINE__);
			list_destroy(list);
			return NULL;
		}

		if(n == 0)
			continue;

		for(;n;n--)//每桌的菜量
		{
			ret = read(socketfd,&chef,sizeof(chef));
			if(ret < 0)
			{
				printf("数据读取错误!\n");
				printf("error in : %s %d\n",__FILE__,__LINE__);
				list_destroy(list);
				return NULL;
			}
			else if(ret == 0)
			{
				printf("与服务器断开连接!\n");	
				printf("%s %d\n",__FILE__,__LINE__);
				list_destroy(list);
				return NULL;
			}

			list = list_create(list,&chef,sizeof(chef));
		}
		printf("\n%d 桌：\n",chef.table_num);
		printf("编 号\t\t菜    名\t\t状  态\n");
		if(tmp == NULL)
			tmp = list_show(list,chef_look);
		else
			tmp = list_show(tmp->next,chef_look);
	}
	return list;
}


void dish_prepare(int socketfd)
{
	int table_num,dish_num,flag,n,ret;
	list_t *list,*marter = NULL;
	void* tmp;
	char date[20];
	pack_head_t head;
	char select;
	marterial_t mar;//材料

	for(;;)//记录准备的菜式数量
	{
		system("clear");
		/*******************查看菜式信息**********************/
		list = look_dish(socketfd,0);
		if(list == NULL)
		{
			position(5,4,0);
			printf("暂无需要准备的菜!\n");
			sleep(1);
			return;
		}

		/*******************选择桌号和菜号*******************/
		for(;;)
		{
			position(2,4,0);
			printf("选 择 桌 号(0取消)：");
			scanf("%d",&table_num);
			position(2,4,0);
			printf("菜 式 编 号(0取消)：");
			scanf("%d",&dish_num);
			while(getchar() != '\n');
			if(table_num == 0 || dish_num == 0)
			{
				list_destroy(list);
				return;
			}

			tmp = list_comp(list,&table_num,&dish_num,check);
			if(tmp == NULL)
				printf("输入有误!\n");
			else
			{
				if(!strcmp(((chef_t*)tmp)->order.state,"等待中"))
					break;
				else
					printf("该菜式已在准备中!\n");
			}
		}

		/**********************选择材料***********************/
		for(n=1;;n++)//记录所用的原料数量
		{
			system("clear");
			marter_menu();
			for(;;)
			{
				position(2,4,0);
				printf("原料序号：");
				scanf("%c",&select);
				while(getchar() != '\n');
				if(select >= FLAVOUR && select <= LETTUCE)
					break; 
				if(select >= 'a' && select <= 'k')
					break;
				else
					printf("无 该 材  料!\n");
			}

			count_time(date,(((chef_t*)tmp)->order.s_time));//设置菜式准备时间

			//原料名字
			switch(select)
			{
				case FLAVOUR:
					strcpy(mar.name,"调料");
					break;
				case FLOUR:
					strcpy(mar.name,"面");
					break;
				case RICE:
					strcpy(mar.name,"米");
					break;
				case DRINKS:
					strcpy(mar.name,"酒水");
					break;
				case POTATO:
					strcpy(mar.name,"土豆");
					break;
				case GREEN:
					strcpy(mar.name,"青菜");
					break;
				case CUKE:
					strcpy(mar.name,"黄瓜");
					break;
				case TOMATO:
					strcpy(mar.name,"西红柿");
					break;
				case LETTUCE:
					strcpy(mar.name,"莴笋");
					break;
				case PEPPER:
					strcpy(mar.name,"青椒");
					break;
				case AGARIC:
					strcpy(mar.name,"木耳");
					break;
				case BEAN_SURD:
					strcpy(mar.name,"豆腐");
					break;
				case EGG:
					strcpy(mar.name,"鸡蛋");
					break;
				case GARLIC:
					strcpy(mar.name,"蒜苔");
					break;
				case LONG_BEAN:
					strcpy(mar.name,"豆角");
					break;
				case EGGPLANT:
					strcpy(mar.name,"茄子");
					break;
				case CHICKEN:
					strcpy(mar.name,"鸡肉");
					break;
				case MEAT:
					strcpy(mar.name,"猪肉");
					break;
				case FISH:
					strcpy(mar.name,"鱼肉");
					break;
				case INTES:
					strcpy(mar.name,"肥肠");
					break;
				default:
					strcpy(mar.name,"NULL");
					break;
			}

			position(1,4,0);
			printf("数   量：");
			scanf("%f",&mar.repertory);
			while(getchar() != '\n');

			marter = list_create(marter,&mar,sizeof(mar));

			for(;;)
			{
				flag = 0;
				position(1,0,0);
				printf("是否还需其他材料(y/n) ? ");
				scanf("%c",&select);
				while(getchar() != '\n');

				if(select == 'Y' || select == 'y' || 
						select == 'N' || select == 'n')
					flag = 1;
				if(!flag)
					printf("输入错误!");
				else
					break;
			}
			if(select == 'Y' || select == 'y')
				continue;
			else
				break;

		}

		/************************扣除材料库存************************/
		ret = package_list_send(socketfd,MARTERIAL,SUB,sizeof(mar),n,marter);
		if(ret < 0)
		{
			printf("该菜式准备失败!\n");
			printf("error in : %s %d\n",__FILE__,__LINE__);
			sleep(1);
			list_destroy(list);
			list = NULL;	
			return;
		}

		//删除材料信息
		list_destroy(marter);	
		marter = NULL;

		/*********************接收反馈结果(材料)********************/
		ret = read(socketfd,&head,sizeof(head));
		if(ret < 0)
		{
			printf("数据读取错误!\n");
			printf("error in : %s %d\n",__FILE__,__LINE__);
			list_destroy(list);
			list = NULL;	
			return;
		}
		else if(ret == 0)
		{
			printf("与服务器断开连接!\n");	
			printf("%s %d\n",__FILE__,__LINE__);
			list_destroy(list);
			list = NULL;	
			return;
		}

		if(head.opt == FAIL)
		{
			position(1,0,0);
			printf("%s %d\n",__FILE__,__LINE__);
			printf("准备失败\n");
		}
		/**********************修改菜式************************/
		strcpy(((chef_t*)tmp)->order.state,"准备中");//改变菜式状态
		((chef_t*)tmp)->flag = MODIFY;
		ret = package_send(socketfd,ORDER,MODIFY,sizeof(chef_t),(chef_t*)tmp);
		if(ret < 0)
		{
			printf("数据发送错误!");
			printf("error in : %s %d\n",__FILE__,__LINE__);
			list_destroy(list);
			list = NULL;	
			return;
		}

		/************************接收反馈结果(菜式)*************************/
		ret = read(socketfd,&head,sizeof(head));
		if(ret < 0)
		{
			printf("数据读取错误!\n");
			printf("error in : %s %d\n",__FILE__,__LINE__);
			list_destroy(list);
			list = NULL;	
			return;
		}
		else if(ret == 0)
		{
			printf("与服务器断开连接!\n");	
			printf("%s %d\n",__FILE__,__LINE__);
			list_destroy(list);
			list = NULL;	
			return;
		}

		if(head.opt == FAIL)
		{
			position(1,0,0);
			printf("%s %d\n",__FILE__,__LINE__);
			printf("准备失败\n");
		}
		else if(head.opt == OK)
		{
			position(1,0,0);
			printf("该菜式正在准备\n");
		}

		/*	
		//检查对应号上的菜式是否都已准备完
		if(list_compare(list,((chef_t*)tmp)->table_num,dish_finsh_check) == NULL)
		{
		ret = package_send(socketfd,CONSUM,MODIFY,((chef_t*)tmp))
		}
		 */

		//删除菜式信息
		list_destroy(list);
		list = NULL;	
		/*************************准备第二道菜***********************/
		position(1,0,0);
		for(;;)
		{
			flag = 0;
			printf("是否继续准备其它菜式(y/n) ? ");
			scanf("%c",&select);
			while(getchar() != '\n');

			if(select == 'Y' || select == 'y' || 
					select == 'N' || select == 'n')
				flag = 1;
			if(!flag)
				printf("输入错误!");
			else
				break;
		}

		if(select == 'Y' || select == 'y')
			continue;
		else
			break;
	}
}


void dish_finsh(int socketfd)
{
	int table_num,dish_num,flag,n,ret;
	list_t *list,*marter = NULL;
	void* tmp;
	char date[20];
	pack_head_t head;
	char select;
	marterial_t mar;//材料

	for(;;)
	{
		system("clear");
		/*******************查看菜式信息**********************/
		list = look_dish(socketfd,1);
		if(list == NULL)
		{
			position(5,4,0);
			printf("暂无已准备好的菜!\n");
			sleep(1);
			return;
		}

		/*******************选择桌号和菜号*******************/
		for(;;)
		{
			position(2,4,0);
			printf("选 择 桌 号(0取消)：");
			scanf("%d",&table_num);
			position(2,4,0);
			printf("菜 式 编 号(0取消)：");
			scanf("%d",&dish_num);
			while(getchar() != '\n');

			if(table_num == 0 || dish_num == 0)
			{
				list_destroy(list);
				return;
			}

			tmp = list_comp(list,&table_num,&dish_num,check);
			if(tmp == NULL)
				printf("输入有误!\n");
			else
			{
				if(!strcmp(((chef_t*)tmp)->order.state,"准备中"))
					break;
				else
					printf("该菜式还未准备!\n");
			}
		}


		count_time(date,(((chef_t*)tmp)->order.e_time));//设置菜式完成时间

		/**********************修改菜式************************/
		strcpy(((chef_t*)tmp)->order.state,"完成");//改变菜式状态
		((chef_t*)tmp)->flag = SAVE;
		ret = package_send(socketfd,ORDER,MODIFY,sizeof(chef_t),(chef_t*)tmp);
		if(ret < 0)
		{
			printf("数据发送错误!");
			printf("error in : %s %d\n",__FILE__,__LINE__);
			list_destroy(list);
			list = NULL;	
			return;
		}

		/************************接收反馈结果(菜式)*************************/
		ret = read(socketfd,&head,sizeof(head));
		if(ret < 0)
		{
			printf("数据读取错误!\n");
			printf("error in : %s %d\n",__FILE__,__LINE__);
			list_destroy(list);
			list = NULL;	
			return;
		}
		else if(ret == 0)
		{
			printf("与服务器断开连接!\n");	
			printf("%s %d\n",__FILE__,__LINE__);
			list_destroy(list);
			list = NULL;	
			return;
		}

		if(head.opt == FAIL)
		{
			position(1,0,0);
			printf("%s %d\n",__FILE__,__LINE__);
			printf("准备失败\n");
		}
		else if(head.opt == OK)
		{
			position(1,0,0);
			printf("该菜式已完成!\n");
		}

		/*****************检查对应桌号上的菜式是否都已准备完*********************/
		if(list_compare(list,&table_num,dish_finsh_check) == NULL)
		{
	//		printf("-----------------------------\n");//////////////////////////////////
			ret = package_send(socketfd,ORDER,SAVE,sizeof(int),&(((chef_t*)tmp)->con_num));
			if(ret < 0)
			{
				printf("send error in : %s %d",__FILE__,__LINE__);
				list_destroy(list);
				list = NULL;	
				return;
			}

			/**************************反馈结果************************/
			ret = read(socketfd,&head,sizeof(head));
			if(ret < 0)
			{
				printf("数据读取错误!\n");
				printf("error in : %s %d\n",__FILE__,__LINE__);
				list_destroy(list);
				list = NULL;	
				return;
			}
			else if(ret == 0)
			{
				printf("与服务器断开连接!\n");	
				printf("%s %d\n",__FILE__,__LINE__);
				list_destroy(list);
				list = NULL;	
				return;
			}
		}

		//删除菜式信息
		list_destroy(list);
		list = NULL;	
		/*************************准备第二道菜***********************/
		position(1,0,0);
		for(;;)
		{
			flag = 0;
			printf("是否继续准备其它菜式(y/n) ? ");
			scanf("%c",&select);
			while(getchar() != '\n');

			if(select == 'Y' || select == 'y' || 
					select == 'N' || select == 'n')
				flag = 1;
			if(!flag)
				printf("输入错误!");
			else
				break;
		}

		if(select == 'Y' || select == 'y')
			continue;
		else
			break;
	}
}

void chef_look(void* data)
{
	chef_t* chef = (chef_t*)data;

	if(data == NULL)
		return;

	printf("%d\t\t%s\t\t%s\n",chef->order.num,chef->order.name,chef->order.state);
}

/**************************************************
 *function:检查输入的桌号上是否有输入的菜式
 return values: 0存在  -1不存在
 **************************************************/
int check(void* table,void* dish,void* data)
{
	chef_t* chef = (chef_t*)data;

	if(table == NULL || dish == NULL || data == NULL)
		return -1;

	if(chef->table_num == *(int*)table && chef->order.num == *(int*)dish)
		return 0;
	else
		return -1;
}

/**************************************************
 *function:检查每个桌号上的菜式是否完成
 return values:0表示未完成 -1表示完成
 **************************************************/
int dish_finsh_check(void* num,void* data)
{
	if(num == NULL || data == NULL)
		return -1;

	if((((chef_t*)data)->table_num == *(int*)num) && 
			((!strcmp(((chef_t*)data)->order.state,"等待中")) || 
			 (!strcmp(((chef_t*)data)->order.state,"准备中"))))
		return 0;
	else
		return -1;
}

/************************************
 *function:定时信号处理(发心跳包)
 ************************************/
void sig_handle(int argc)
{
	int nfound,maxfd;
	heart_t heart;
	struct timeval tval = {0,100};
	pack_head_t head;
	fd_set set;
	
	FD_ZERO(&set);
	FD_SET(clientfd,&set);
	maxfd = clientfd;

	strcpy(heart.str,"Are you ok ?");
	package_send(clientfd,HEART,0,sizeof(heart_t),&heart);

	bzero(heart.str,sizeof(heart.str));
	nfound = select(maxfd+1,&set,NULL,NULL,&tval);
	if(nfound == 0)
	{
		system("clear");
		position(5,5,6);
		sleep(1);
		printf("与服务器断开连接\n");
		system("clear");
		close(clientfd);
		FD_CLR(clientfd,&set);
		exit(0);
	//	printf("---------------------\n");
	}
	else if(nfound < 0)
	{
		printf("读取错误!\n");
	}
	else
	{
		read(clientfd,&head,sizeof(head));
		read(clientfd,&heart,sizeof(heart));
		if(strcmp(heart.str,"this is ok!"))
		{
			system("clear");
			position(5,5,6);
			printf("与服务器断开连接\n");
			sleep(1);
			system("clear");
			close(clientfd);
			FD_CLR(clientfd,&set);
			exit(0);
		}
	}
	alarm(10);
}
