#include "waiter_func.h"

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
	printf("***********        手    持    终    端        *************");	
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
	printf("|          1 点             菜          |");
	position(1,4,1);
	printf("|                                       |");
	position(1,4,1);
	printf("|          2 加             菜          |");
	position(1,4,1);
	printf("|                                       |");
	position(1,4,1);
	printf("|          3 查 看 已 点 菜 品          |");
	position(1,4,1);
	printf("|                                       |");
	position(1,4,1);
	printf("|          4 查 看 菜 品 状 态          |");
	position(1,4,1);
	printf("|                                       |");
	position(1,4,1);
	printf("|          5 减             菜          |");
	position(1,4,1);
	printf("|                                       |");
	position(1,4,1);
	printf("|          6 提             交          |");
	position(1,4,1);
	printf("|                                       |");
	position(1,4,1);
	printf("|          0 退             出          |");
	position(1,4,1);
	printf("-----------------------------------------");
	position(1,0,0);
}
/*
   void dish_menu()
   {
   head_menu();
   position(1,5,0);
   printf("请 输 入 选 项");
   position(1,4,0);
   printf("-----------------------------------------");
   position(1,5,0);
   printf("1 凉             菜");
   position(1,5,0);
   printf("2 素             菜");
   position(1,5,0);
   printf("3 干             锅");
   position(1,5,0);
   printf("4 招      牌     菜");
   position(1,5,0);
   printf("5 汤             类");
   position(1,5,0);
   printf("6 酒             水");
   position(1,5,0);
   printf("0 返             回");
   position(1,4,0);
   printf("-----------------------------------------");
   position(1,0,0);
   }
 */

void dish_menu()
{
	position(1,3,0);
	printf("1 凉    菜");
	position(0,3,0);
	printf("2 素    菜");
	position(2,3,0);
	printf("3 荤    菜");
	position(0,3,0);
	printf("4 干    锅");
	position(2,3,0);
	printf("5 招 牌 菜");
	position(0,3,0);
	printf("6 铁    板");
	position(2,3,0);
	printf("7 汤    类");
	position(0,3,0);
	printf("8 酒    水");
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
			//case REGISTER:
			//	regist(socketfd);
			//	break;
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
		//	setbuf(stdin,NULL);

		switch(select)
		{
			case BOOK:
				order_dish(socketfd);//点菜
				break;
			case DISH_ADD:
				dish_add(socketfd);//加菜
				break;
			case DISH_LOOK:
				dish_look();//查看已点菜式
				break;
			case STATE_LOOK:
				state_look(socketfd);//菜式状态查看
				break;
			case DISH_SUB:
				dish_sub(socketfd);//减菜(从订单中移除)
				break;
			case SUBMIT:
				dish_submit(socketfd);//提交菜式
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

/******************************
 *funtion:登录服务器
 ******************************/
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

	data.flag = WAITER;

	for(;;)
	{
		ret = package_send(socketfd,WAITER,LOGIN,sizeof(data),&data);
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
		else
		{
			position(1,0,0);
			printf("登录失败\n");
			ret = -1;
		}
		sleep(1);
		return ret;
	}
}

/*************点菜信息保存***************/
list_t* list = NULL;   //保存点餐信息
consum_t consum;       //保存消费信息

/*****************************************
 *function:function:点菜
 *****************************************/
void order_dish(int socketfd)
{
	int ret,n,flag,flag1,num;
	pack_head_t head;
	dishes_t dish,*Dish;
	char select;
	list_t* list_head = NULL;
	order_t order,*tmp;

	system("clear");

	if(list != NULL)
	{
		printf("有客人未点完餐，请稍等!\n");
		sleep(1);
		return;
	}
	/**************选择桌号***************/
	for(;;)
	{
		for	(;;)
		{
			position(2,4,0);
			printf("桌   号：");
			scanf("%d",&consum.table_num);
			while(getchar() != '\n');

			if(consum.table_num > 0 && consum.table_num <= TABLE_NUM)
				break;
			else
				printf("桌号不存在!");
		}
		/*		
				if(list_compare(List,&consum.table_num,table_numcheck) == 1)
				{
				printf("点菜失败，该桌已有客人点菜\n");
				sleep(1);
				return NULL;
				}
		 */		 
		ret = package_send(socketfd,CONSUME,ADD,sizeof(consum.table_num),&consum.table_num);
		if(ret < 0)
		{
			printf("菜式添加失败\n");
			printf("error in : %s %d\n",__FILE__,__LINE__);
			sleep(1);
			return;
		}
		ret = read(socketfd,&head,sizeof(head));
		if(ret < 0)
		{
			printf("菜式添加失败\n");
			printf("error in : %s %d\n",__FILE__,__LINE__);
			sleep(1);
			return;
		}
		else if(ret == 0) 
		{
			printf("与服务器断开连接\n");
			printf("%s %d\n",__FILE__,__LINE__);
			sleep(1);
			return;
		}

		printf("opt = %d\n",head.opt);/////////////////////////////
		if(head.opt == YES)
		{
			printf("该桌已有客人点菜!\n");
		}
		else if(head.opt == NO)
			break;
	}

	consum.price = 0;
	for(consum.dish_num=0;;)
	{
		system("clear");
		dish_menu();
		/*******************选择菜类*******************/
		for(;;)
		{
			flag1 = 0;
			position(2,4,0);
			printf("菜  类  序  号(0取消)：");
			scanf("%c",&select);
			while(getchar() != '\n');
			if(select == '0')
			{
				flag1 = 1;
				break;
			}

			if(select >= CLOD && select <= DRINK)
				break;
			else
				printf("无 该 菜 品 类!\n");
		}
		if(flag1 == 1)
			break;

		ret = package_send(socketfd,FOOD,SHOW,sizeof(select),&select);
		if(ret < 0)
		{
			printf("点菜失败!\n");
			printf("%s %d\n",__FILE__,__LINE__);
			sleep(1);
			return;
		}
		ret = read(socketfd,&head,sizeof(head));

		/******************读取菜式的数量*******************/
		ret = read(socketfd,&n,sizeof(n));
		if(ret < 0)
		{
			printf("点菜失败!\n");
			printf("%s %d\n",__FILE__,__LINE__);
			sleep(1);
			return;
		}
		else if(ret == 0)
		{
			printf("与服务器断开连接，查看失败!\n");
			sleep(1);
			return;
		}
		/*********************打印菜式**********************/
		if( n != 0)
		{
			for(int i=0;i<n;i++)
			{
				ret = read(socketfd,&dish,sizeof(dish));
				if(ret < 0)
				{
					printf("查看失败!\n");
					printf("%s %d\n",__FILE__,__LINE__);
					sleep(1);
					list_destroy(list_head);
					return;
				}
				else if(ret == 0)
				{
					printf("与服务器断开连接，查看失败!\n");
					sleep(1);
					list_destroy(list_head);
					return;
				}

				list_head = list_create(list_head,&dish,sizeof(dish));
			}
			system("clear");
			position(1,0,0);
			printf("编 号\t菜  类\t菜    名\t价格(元)\n");
			list_show(list_head,dish_show);//菜式显示
			//getchar();//阻塞在此，一直显示菜式

			/**********************选择菜品******************/
			for(;;)
			{	
				flag1 = 0;
				position(1,4,0);
				printf("菜 品 编 号(0取消)：");
				scanf("%d",&order.num);
				if(order.num == 0)
				{
					flag1 = 1;
					break;
				}
				Dish =(dishes_t*)list_compare(list_head,&order.num,dishnum_check);
				if(Dish == NULL)
					printf("无该菜式!\n");
				else
					break;
			}

			if(flag1 == 1)
			{			
				list_destroy(list_head);
				list_head = NULL;
				continue;
			}

			position(1,4,0);
			printf("数     量：");
			scanf("%d",&order.dish_num);
			while(getchar() != '\n');
			
			tmp = list_compare(list,&order.num,order_num_compare);//检查菜式是否已点过
			if(tmp == NULL)//未点过该菜式
			{	
				consum.dish_num ++;
				strcpy(order.class_name,Dish->class_name);
				strcpy(order.name,Dish->name);
				order.price = Dish->price;
				strcpy(order.state,"等待中");
				sprintf(order.s_time,"%s","-1:00:00");
				sprintf(order.e_time,"%s","-1:00:00");

				list = list_create(list,&order,sizeof(order));//保存点菜信息
			}
			else //已点过该菜式
				tmp->dish_num += order.dish_num;

			consum.price += order.dish_num*Dish->price;
			list_destroy(list_head);
			list_head = NULL;
		}
		else
			printf("该菜类暂无菜式信息!\n");

		for(;;)
		{
			flag = 0;
			position(2,0,0);
			printf("是否继续点菜(y/n) ? ");
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

/*************************************
 *function:加菜
 *************************************/
void dish_add(int socketfd)
{
	int ret,n,flag,flag1,num;
	pack_head_t head;
	dishes_t dish,*Dish;
	char select;
	list_t* list_head = NULL;
	//	check_t check;
	order_t order,*tmp;

	system("clear");
	if(list != NULL)
	{
		for(;;)
		{
			system("clear");
			dish_menu();
			/*******************选择菜类*******************/
			for(;;)
			{
				flag1 = 0;
				position(2,4,0);
				printf("菜  类  序  号(0取消)：");
				scanf("%c",&select);
				while(getchar() != '\n');
				if(select == '0')
				{
					flag1 = 1;
					break;
				}

				if(select >= CLOD && select <= DRINK)
					break;
				else
					printf("无 该 菜 品 类!\n");
			}
			if(flag1 == 1)
				break;

			ret = package_send(socketfd,FOOD,SHOW,sizeof(select),&select);
			if(ret < 0)
			{
				printf("点菜失败!\n");
				printf("%s %d\n",__FILE__,__LINE__);
				sleep(1);
				return;
			}
			ret = read(socketfd,&head,sizeof(head));

			/******************读取菜式的数量*******************/
			ret = read(socketfd,&n,sizeof(n));
			if(ret < 0)
			{
				printf("点菜失败!\n");
				printf("%s %d\n",__FILE__,__LINE__);
				sleep(1);
				return;
			}
			else if(ret == 0)
			{
				printf("与服务器断开连接，查看失败!\n");
				sleep(1);
				return;
			}
			/*********************打印菜式**********************/
			if( n != 0)
			{
				for(int i=0;i<n;i++)
				{
					ret = read(socketfd,&dish,sizeof(dish));
					if(ret < 0)
					{
						printf("查看失败!\n");
						printf("%s %d\n",__FILE__,__LINE__);
						sleep(1);
						list_destroy(list_head);
						return;
					}
					else if(ret == 0)
					{
						printf("与服务器断开连接，查看失败!\n");
						sleep(1);
						list_destroy(list_head);
						return;
					}

					list_head = list_create(list_head,&dish,sizeof(dish));
				}
				system("clear");
				position(1,0,0);
				printf("编 号\t菜  类\t菜    名\t价格(元)\n");
				list_show(list_head,dish_show);//菜式显示
				//getchar();//阻塞在此，一直显示菜式

				/**********************选择菜品******************/
				for(;;)
				{	
					flag1 = 0;
					position(1,4,0);
					printf("菜 品 编 号(0取消)：");
					scanf("%d",&order.num);
					while(getchar() != '\n');
					
					if(order.num == 0)
					{
						flag1 = 1;
						break;
					}
					Dish = list_compare(list_head,&order.num,dishnum_check);
					if(Dish == NULL)
						printf("无该菜式!\n");
					else
						break;
				}

				if(flag1 == 1)
				{
					list_destroy(list_head);
					list_head = NULL;
					continue;
				}

				position(1,4,0);
				printf("数     量：");
				scanf("%d",&order.dish_num);
				while(getchar() != '\n');

				tmp = list_compare(list,&order.num,order_num_compare);//检查菜式是否已点过
				if(tmp == NULL)//未点过该菜式
				{	
					consum.dish_num ++;
					strcpy(order.class_name,Dish->class_name);
					strcpy(order.name,Dish->name);
					order.price = Dish->price;
					strcpy(order.state,"等待中");
					sprintf(order.s_time,"%s","-1:00:00");
					sprintf(order.e_time,"%s","-1:00:00");

					list = list_create(list,&order,sizeof(order));//保存点菜信息
				}
				else //已点过该菜式
					tmp->dish_num += order.dish_num;

				consum.price += order.dish_num*Dish->price;
				list_destroy(list_head);
				list_head = NULL;
			}
			else
				printf("该菜类暂无菜式信息!\n");

			for(;;)
			{
				flag = 0;
				position(2,0,0);
				printf("是否继续点菜(y/n) ? ");
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
	else
	{
		position(4,4,0);
		printf("订单已提交!\n");
		sleep(1);
	}
}

/**************************************************
 *function:减菜
 **************************************************/
void dish_sub(int socketfd)
{
	int num,n;
	order_t order,*tmp;

	system("clear");
	if(list != NULL)
	{

		position(1,0,0);
		printf("编 号\t菜  类\t菜    名\t价格(元)\t数 量\n");
		list_show(list,dish_printf);//菜式显示

		for(;;)
		{
			position(2,4,0);
			printf("编  号(0取消)：");
			scanf("%d",&num);
			while(getchar() != '\n');
			
			if(num == 0)
				return;

			tmp = list_compare(list,&num,order_num_compare);
			if(tmp == NULL)
			{
				printf("输入错误!");
				continue;
			}
			else
				break;
		}

		position(2,4,0);
		printf("份      数：");
		scanf("%d",&n);
		while(getchar() != '\n');

		if(tmp->dish_num > n)
		{
			tmp->dish_num -= n;
			consum.price -= tmp->price*n;
		}
		else if(tmp->dish_num == n)
		{
			consum.dish_num--;
			consum.price -= tmp->price*n;
			list = list_node_del(list,&num,order_num_compare);
		//	printf("菜式移除成功\n");
		//	sleep(1);
		}
		else
		{
			printf("输入错误!\n");
			sleep(1);
		}
	}
	else
	{
		position(4,4,0);
		printf("订单已提交!\n");
		sleep(1);
	}
}

void dish_look()
{
	system("clear");
	if(list == NULL)
	{
		position(5,5,0);
		printf("您还未进行点餐!\n");
		sleep(1);
		return;	
	}
	position(1,0,0);
	printf("编 号\t菜  类\t菜    名\t价格(元)\t数量\n");
	list_show(list,dish_printf);//菜式显示
	position(1,0,0);
	printf("总   价：%.1f\n",consum.price);
	getchar();
}

/*******************************************
 *function:手持端查看菜式状态
 *******************************************/
void state_look(int socketfd)
{
	int ret,num;
	pack_head_t head;
	consum_t consum;
	order_t order;
	list_t *list = NULL;

	system("clear");
	for	(;;)
	{
		position(2,4,0);
		printf("桌   号：");
		scanf("%d",&num);
		while(getchar() != '\n');

		if(num > 0 && num <= TABLE_NUM)
			break;
		else
			printf("桌号不存在!");
	}
	ret = package_send(socketfd,WAITER,L_K,sizeof(num),&num);
	if(ret < 0)
	{
		printf("查看失败\n");
		printf("error in : %s %d\n",__FILE__,__LINE__);
		sleep(1);
		return;
	}

	read(socketfd,&head,sizeof(head));
	if(head.opt == FAIL)
	{
		position(5,4,0);
		printf("没有该餐桌的消费信息!\n");
		sleep(1);
		return;
	}

	ret = read(socketfd,&consum,sizeof(consum));
/*	if(consum.dish_num <= 0)
	{
		position(5,4,0);
		printf("没有该餐桌的消费信息!\n");
		sleep(1);
		return;
	}
*/
	ret = read(socketfd,&num,sizeof(num));
	if(num <= 0)
	{
		system("clear");
		position(5,4,0);
		printf("没有该餐桌的消费信息!\n");
		sleep(1);
		return;
	}

	for(int i=0;i<num;i++)
	{
		ret = read(socketfd,&order,sizeof(order));
		if(ret < 0)
		{
			printf("read fail in : %s %d\n",__FILE__,__LINE__);
			list_destroy(list);
			sleep(1);
			return;
		}
		else if(ret == 0)
		{
			printf("与服务器断开连接!\n");
			list_destroy(list);
			sleep(1);
			return;
		}
		list = list_create(list,&order,sizeof(order));
	}
	system("clear");
	position(1,0,0);
	printf("编 号\t菜    名\t价格(元)\t数量\t准备时间\t完成时间\t状 态\n");
	list_show(list,order_state);
	list_destroy(list);
	getchar();
}


/*******************************************
 *cfunction:订单提交
 *******************************************/
void dish_submit(int socketfd)
{
	int ret,opt;
	pack_head_t head;

	system("clear");
	if(list == NULL)
	{
		position(5,5,0);
		printf("您还未进行点菜\n");	
		sleep(1);	
		return;
	}

	position(5,5,0);
	strcpy(consum.state,"等待中");
	strcpy(consum.state1,"未结帐");
	count_time(consum.date,consum.submit_time);
	sprintf(consum.pay_time,"%s","-01:00:00");
	/*	ret = package_send(socketfd,CONSUME,SAVE,sizeof(consum),&consum);
		if(ret < 0)
		{
		printf("订单提交失败\n");
		list_destroy(list);
		list = NULL;
		return;
		}
	 */
	//	list = list_insert(list,&consum,sizeof(consum));
	printf("订单提交中.....\n");
	position(1,0,0);
	ret = packagesend(socketfd,CONSUME,SAVE,sizeof(consum),
			&consum,sizeof(order_t),consum.dish_num,list);
	if(ret < 0)
	{
		printf("订单提交失败\n");
		list_destroy(list);
		list = NULL;
		sleep(1);
		return;
	}
	list_destroy(list);
	list = NULL;
	ret = read(socketfd,&head,sizeof(head));
	if(ret < 0)
	{
		printf("订单提交失败\n");
		sleep(1);
		return;
	}
	else if(ret == 0)
	{
		printf("与服务器断开连接，订单提交失败\n");
		sleep(1);
		return;
	}
	if(head.opt == OK)
	{
		printf("订单提交成功\n");
	}
	else if(head.opt == FAIL)
		printf("订单提交失败\n");

	sleep(1);
}


/**********************************************
 *function:查看对应桌号是否已有订餐
 return values:0表示没有，1表示已有
 **********************************************/
/*int tablenumcheck(void* num,void* data)
  {
  if(num == NULL || data == NULL)
  return 0;

  if(*(int*)num == (consum_t*)data->tablenum)
  return 1;
  else
  return 0;
  }
 */

/**********************************************
 *function:查看菜式号是否存在
 return values:0表示存在，-1表示不存在
 **********************************************/
int dishnum_check(void* num,void* data)
{
	if(num == NULL || data == NULL)
		return -1;

	if(*(int*)num == ((dishes_t*)data)->num)
		return 0;
	else
		return -1;
}

/*****************************************
 *function:打印菜式
 ******************************************/
void dish_show(void* data)
{
	dishes_t* dish = (dishes_t*)data;

	if(dish == NULL)
		return;
	position(1,0,0);
	printf("%d\t%s\t%s\t%.1f\n",dish->num,dish->class_name,
			dish->name,dish->price);
}


void dish_printf(void* data)
{
	order_t* order = (order_t*)data;

	if(order == NULL)
		return;
	position(1,0,0);
	printf("%d\t%s\t%s\t%.1f\t\t%d\n",order->num,
			order->class_name,order->name,order->price,
			order->dish_num);
}


/**********************************************
 *function:比较订单编号是否匹配
 **********************************************/
int order_num_compare(void* data,void* ptr)
{
	order_t* tmp = (order_t*)ptr;

	if(data == NULL || ptr == NULL)
		return -1;

	if(tmp->num == *(int*)data)
		return 0;
	else
		return -1;
}

/************************************
 *function:打印菜式
 ************************************/
void order_state(void* argv)
{
	order_t *order = (order_t*)argv;

	printf("%d\t%s\t%.1f\t\t%d\t%s\t%s\t%s\n",
			order->num,order->name,order->price,
			order->dish_num,order->s_time,order->e_time,
			order->state);
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
		printf("与服务器断开连接\n");
		sleep(2);
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
			sleep(2);
			system("clear");
			close(clientfd);
			FD_CLR(clientfd,&set);
			exit(0);
		}
	}
	alarm(10);
}

