#include "admin_func.h"

/**************************************
 *function:管理员登录
 **************************************/
int admin_login(int socketfd)
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
	//hide_passward(data.passward);
	while(getchar() != '\n');

	data.flag = ADMIN;
	for(;;)
	{
		ret = package_send(socketfd,ADMIN,LOGIN,sizeof(data),&data);
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
		printf("opt = %d\b\n",head.opt);////////////////////////////////
		return ret;
	}
}

/****************************************
 *function:添加会员
 ***************************************/
int member_add(int socketfd)
{
	member_t member;
	int ret;
	pack_head_t head;
	check_t check;

	for(;;)
	{
		system("clear");
		position(5,0,0);
		/********************************设置帐号********************************/
		/*对帐号进行检测*/
		for(;;)
		{
			bzero(member.tel,sizeof(member.tel));
			position(0,5,0);
			fprintf(stdout,"联系方式：");
			scanf("%s",member.tel);	
			while(getchar() != '\n');

			if(strlen(member.tel) < TEL)
				break;
			else
			{
				position(1,0,0);
				printf("手机号长度格式不正确\n");
			}
		}

		/*****************手机号查重*********************/
		strcpy(check.str,member.tel);
		check.flag = MEMBER;
		ret = package_send(socketfd,MEMBER,CHECK,sizeof(check),&check);
		if(ret < 0)
		{
			position(1,0,0);
			fprintf(stdout,"发送消息失败\n");
			position(1,0,0);
			printf("%s : %d\n",__FILE__,__LINE__);
			sleep(1);
			return -1;
		}

		/*获取查询结果*/
		ret = read(socketfd,&head,sizeof(head));
		if(ret < 0)
		{
			position(1,0,0);
			printf("信息读取失败\n");
			position(1,0,0);
			printf("%s : %d\n",__FILE__,__LINE__);
			sleep(1);
			return -1;
		}
		else if(ret == 0)
		{
			position(1,0,0);
			printf("与服务器断开连接\n");
			position(1,0,0);
			printf("%s : %d\n",__FILE__,__LINE__);
			sleep(1);
			return -1;
		}
		printf("opt = %d\n",head.opt);////////////////////////////
		if(head.opt == NO)
			break;
		else if(head.opt == YES)
		{
			position(1,0,0);
			printf("该手机号已存在!\n");
			sleep(1);
		}
	}

	//设置密码
	set_pass(member.passward);
	//录入其他信息
	position(1,5,0);
	printf("姓   名：");
	scanf("%s",member.name);
	position(1,5,0);
	printf("年   龄：");
	scanf("%d",&member.age);
	while(getchar() != '\n');
	position(1,5,0);
	printf("性   别：");
	scanf("%s",member.sex);
	position(1,5,0);
	printf("身份证号：");
	scanf("%s",member.id);
	while(getchar() != '\n');

	member.integral = 0;

	/*******************************发送注册信息到服务器******************************/
	ret = package_send(socketfd,MEMBER,ADD,sizeof(member),&member);
	if(ret < 0)
	{
		position(1,0,0);
		printf("会员办理失败!\n");
	}
	/********************************查看服务器返回值*************************************/
	ret = read(socketfd,&head,sizeof(pack_head_t));
	if(ret < 0)
	{
		position(1,0,0);
		printf("会员添加失败");
		position(1,0,0);
		printf("%s : %d\n",__FILE__,__LINE__);
		sleep(1);
		return -1;
	}
	else if(ret == 0)
	{
		position(1,0,0);
		printf("与服务器断开连接，添加失败");
		position(1,0,0);
		printf("%s : %d\n",__FILE__,__LINE__);
		sleep(1);
		exit(0);
	}
	if(head.opt == FAIL)
	{
		position(1,0,0);
		printf("会员办理失败!");
		position(1,0,0);
		printf("%s : %d\n",__FILE__,__LINE__);
		sleep(1);
		return -1;
	}
	else if(head.opt == OK)
	{
		position(1,0,0);
		printf("会员办理成功!");
		position(1,0,0);
		sleep(1);
		return 0;
	}
}


/********************************************
 *function:获取会员信息
 ********************************************/
list_t* member_info(int socketfd)
{
	int ret,n,flag,num;
	pack_head_t head;
	member_t member;
	list_t* list = NULL;
	char select;

	ret = package_send(socketfd,MEMBER,SHOW,0,NULL);
	if(ret < 0)
	{
		printf("查看失败!\n");
		printf("%s %d\n",__FILE__,__LINE__);
		sleep(1);
		return NULL;
	}
	ret = read(socketfd,&head,sizeof(head));

	//读取会员的数量
	ret = read(socketfd,&n,sizeof(n));
	if(ret < 0)
	{
		printf("查看失败!\n");
		printf("%s %d\n",__FILE__,__LINE__);
		sleep(1);
		return NULL;
	}
	else if(ret == 0)
	{
		printf("与服务器断开连接，查看失败!\n");
		sleep(1);
		return NULL;
	}
	if( n != 0)
	{
		for(int i=0;i<n;i++)
		{
			ret = read(socketfd,&member,sizeof(member));
			if(ret < 0)
			{
				printf("查看失败!\n");
				printf("%s %d\n",__FILE__,__LINE__);
				sleep(1);
				list_destroy(list);
				return NULL;
			}
			else if(ret == 0)
			{
				printf("与服务器断开连接，查看失败!\n");
				sleep(1);
				list_destroy(list);
				return NULL;
			}

			list = list_create(list,&member,sizeof(member));
		}
		return list;
	}
}

/***********************************************
 *function:查看会员信息
 ***********************************************/
void member_show(int socketfd)
{
	int ret,n,flag,num;
	pack_head_t head;
	member_t member,*tmp;
	list_t* list = NULL;
	char select;

	list = member_info(socketfd);
	if(list == NULL)
	{
		system("clear");
		position(4,5,0);
		printf("暂无会员信息!\n");
		sleep(1);
		return;
	}

	for(;;)
	{
		system("clear");
		position(1,0,0);
		list_show(list,member_num);//会员编号显示

		for(;;)
		{
			position(3,4,0);
			printf("想要查看会员的编号(0取消)：");
			scanf("%d",&num);
			while(getchar() != '\n');
			if(num == 0)
			{
				list_destroy(list);
				return;
			}

			tmp = list_compare(list,&num,member_num_check);//会员编号检查
			if(tmp == NULL)
				printf("输入有误\n");
			else
			{
				system("clear");
				member_printf(tmp);
				break;
			}
		}

		for(;;)
		{
			flag = 0;
			position(2,0,0);
			printf("是否继续查看(y/n) ? ");
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
	list_destroy(list);
}

void member_mod(int socketfd)
{

}

/***************************************************
 *function:删除会员
 ***************************************************/
void member_del(int socketfd)
{
	int ret,n,flag,num;
	pack_head_t head;
	member_t member,*tmp;
	list_t* list;
	char select;

	system("clear");
	list = member_info(socketfd);
	if(list == NULL)
	{
		position(4,5,0);
		printf("暂无会员信息!\n");
		sleep(1);
		return;
	}

	position(1,0,0);
	list_show(list,member_num);//会员编号显示

	position(3,4,0);
	printf("想要删除会员的编号(0取消)：");
	scanf("%d",&num);
	while(getchar() != '\n');

	if(num == 0)
		return;

	tmp = list_compare(list,&num,member_num_check);//会员编号检查
	if(tmp == NULL)
		printf("输入有误\n");
	else
	{
		ret = package_send(socketfd,MEMBER,DEL,sizeof(num),&num);
		if(ret < 0)
		{
			printf("send data error in : %s %d\n",__FILE__,__LINE__);
			list_destroy(list);
			return;
		}

		ret = read(socketfd,&head,sizeof(head));
		if(ret < 0)
		{
			printf("read data error in : %s %d\n",__FILE__,__LINE__);
			list_destroy(list);
			return;
		}
		position(2,0,0);
		if(head.opt == OK)
		{
			printf("删除成功!\n");
			list_destroy(list);
			sleep(1);
			return;
		}
		else if(head.opt == FAIL)
		{
			printf("删除失败!\n");
			list_destroy(list);
			sleep(1);
			return;
		}
	}
	list_destroy(list);
}

/********************************************
 *function:添加员工
 ********************************************/ 
void employee_add(int socketfd)
{
	char account[50],passward[PASSWARD],name[NAME],
	tel[TEL],id[ID],address[ADDR],sex[SEX],depart[DEPART];
	int ret,age;
	float salary;
	char buff[LENGTH];
	pack_head_t head;
	check_t check;
	regist_t regist;

	system("clear");
	position(5,0,0);
	/********************************设置帐号********************************/
	for(;;)
	{
		/*对帐号进行检测*/
		for(;;)
		{
			bzero(account,sizeof(account));
			position(0,5,0);
			fprintf(stdout,"帐   号：");
			scanf("%s",account);	
			while(getchar() != '\n');

			if(strlen(account) < ACCOUNT)
				break;
			else
			{
				position(1,0,0);
				printf("帐号长度格式不正确\n");
			}
		}

		for(;;)
		{
			position(1,5,0);
			printf("职    位：");
			scanf("%s",depart);
			while(getchar() != '\n');

			if(!strcmp(depart,"厨师") || !strcmp(depart,"服务员") /*||!strcmp(depart,"管理员")*/)
				break;
			else
				printf("无该职位!");

		}

		/*****************帐号查重*********************/
		strcpy(check.str,account);
		if(!strcmp(depart,"服务员"))
		{
			check.flag = WAITER;
			ret = package_send(socketfd,WAITER,CHECK,sizeof(check_t),&check);
		}
		else if(!strcmp(depart,"厨师"))
		{
			check.flag = CHEF;
			ret = package_send(socketfd,CHEF,CHECK,sizeof(check_t),&check);
		}
		if(ret < 0)
		{
			position(1,0,0);
			fprintf(stdout,"发送消息失败\n");
			position(1,0,0);
			printf("%s : %d\n",__FILE__,__LINE__);
			sleep(1);
			return;
		}

		/*获取查询结果*/
		ret = read(socketfd,&head,sizeof(head));
		if(ret < 0)
		{
			position(1,0,0);
			printf("信息读取失败\n");
			position(1,0,0);
			printf("%s : %d\n",__FILE__,__LINE__);
			sleep(1);
			return;
		}
		else if(ret == 0)
		{
			position(1,0,0);
			printf("与服务器断开连接\n");
			position(1,0,0);
			printf("%s : %d\n",__FILE__,__LINE__);
			sleep(1);
			exit(0);
		}
		if(head.opt == NO)
			break;
		else if(head.opt == YES)
		{
			position(1,0,0);
			printf("该帐号已存在\n");
		}
	}

	//设置密码
	set_pass(passward);
	//录入其他信息
	position(1,5,0);
	printf("姓   名：");
	scanf("%s",name);
	position(1,5,0);
	printf("年   龄：");
	scanf("%d",&age);
	while(getchar() != '\n');
	position(1,5,0);
	printf("性   别：");
	scanf("%s",sex);
	position(1,5,0);
	printf("身份证号：");
	scanf("%s",id);
	position(1,5,0);
	printf("联系方式：");
	scanf("%s",tel);
	position(1,5,0);
	printf("联系地址：");
	scanf("%s",address);
	/*	
		position(1,5,0);
		printf("职    位：");
		scanf("%d",&depart);
	 */
	position(1,5,0);
	printf("薪    水：");
	scanf("%f",&salary);
	while(getchar() != '\n');

	/*******************************发送注册信息到服务器******************************/
	strcpy(regist.account,account);
	strcpy(regist.passward,passward);
	strcpy(regist.name,name);
	regist.age = age;
	strcpy(regist.sex,sex);
	strcpy(regist.id,id);
	strcpy(regist.tel,tel);
	strcpy(regist.address,address);
	strcpy(regist.depart,depart);
	regist.salary = salary;

	for(;;)
	{
		ret = package_send(socketfd,WAITER,ADD,sizeof(regist_t),&regist);
		if(ret < 0)
		{
			position(1,0,0);
			//	printf("注册失败");
			position(1,0,0);
			printf("%s : %d\n",__FILE__,__LINE__);
			//	sleep(1);
			continue;
		}
		break;
	}
	/********************************查看服务器返回值*************************************/
	ret = read(socketfd,&head,sizeof(pack_head_t));
	if(ret < 0)
	{
		position(1,0,0);
		printf("员工添加失败");
		position(1,0,0);
		printf("%s : %d\n",__FILE__,__LINE__);
		sleep(1);
		return;
	}
	else if(ret == 0)
	{
		position(1,0,0);
		printf("与服务器断开连接，添加失败");
		position(1,0,0);
		printf("%s : %d\n",__FILE__,__LINE__);
		sleep(1);
		exit(0);
	}
	if(head.opt == FAIL)
	{
		position(1,0,0);
		printf("员工添加失败");
		position(1,0,0);
		printf("%s : %d\n",__FILE__,__LINE__);
		sleep(1);
		return;
	}
	else if(head.opt == OK)
	{
		position(1,0,0);
		printf("员工添加成功");
		position(1,0,0);
		sleep(1);
		return;
	}
}

/********************************************
 *function:获取员工信息
 ********************************************/
list_t* employee_info(int socketfd)
{
	int ret,n,flag,num;
	pack_head_t head;
	regist_t employee;
	list_t* list = NULL;
	char select;

	ret = package_send(socketfd,WAITER,SHOW,0,NULL);
	if(ret < 0)
	{
		printf("查看失败!\n");
		printf("%s %d\n",__FILE__,__LINE__);
		sleep(1);
		return NULL;
	}
	ret = read(socketfd,&head,sizeof(head));

	//读取员工的数量
	ret = read(socketfd,&n,sizeof(n));
	if(ret < 0)
	{
		printf("查看失败!\n");
		printf("%s %d\n",__FILE__,__LINE__);
		sleep(1);
		return NULL;
	}
	else if(ret == 0)
	{
		printf("与服务器断开连接，查看失败!\n");
		sleep(1);
		return NULL;
	}
	if( n != 0)
	{
		for(int i=0;i<n;i++)
		{
			ret = read(socketfd,&employee,sizeof(employee));
			if(ret < 0)
			{
				printf("查看失败!\n");
				printf("%s %d\n",__FILE__,__LINE__);
				sleep(1);
				list_destroy(list);
				return NULL;
			}
			else if(ret == 0)
			{
				printf("与服务器断开连接，查看失败!\n");
				sleep(1);
				list_destroy(list);
				return NULL;
			}

			list = list_create(list,&employee,sizeof(employee));
		}
		return list;
	}
}

/********************************************
 *function:员工信息查看
 ********************************************/
void emlpoyee_show(int socketfd)
{
	int ret,n,flag,num;
	pack_head_t head;
	regist_t employee,*tmp;
	list_t* list = NULL;
	char select;

	list = employee_info(socketfd);
	if(list == NULL)
	{
		system("clear");
		position(4,5,0);
		printf("暂无员工信息!\n");
		sleep(1);
		return;
	}

	for(;;)
	{
		system("clear");
		position(1,0,0);
		list_show(list,employee_num);//员工编号显示

		for(;;)
		{
			position(3,4,0);
			printf("想要查看的员工编号(0取消)：");
			scanf("%d",&num);
			while(getchar() != '\n');
			if(num == 0)
			{
				list_destroy(list);
				return;
			}

			tmp = list_compare(list,&num,employee_num_check);//员工编号检查
			if(tmp == NULL)
				printf("输入有误\n");
			else
			{
				system("clear");
				employee_printf(tmp);
				break;
			}
		}

		for(;;)
		{
			flag = 0;
			position(2,0,0);
			printf("是否继续查看(y/n) ? ");
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
	list_destroy(list);
}

/***************************************************
 *function:删除员工
 ***************************************************/
void employee_del(int socketfd)
{
	int ret,n,flag,num;
	pack_head_t head;
	regist_t employee,*tmp;
	list_t* list;
	char select;

	system("clear");
	list = employee_info(socketfd);
	if(list == NULL)
	{
		position(4,5,0);
		printf("暂无员工信息!\n");
		sleep(1);
		return;
	}

	position(1,0,0);
	list_show(list,employee_num);//员工编号显示

	position(3,4,0);
	printf("想要删除的员工编号(0取消)：");
	scanf("%d",&num);
	while(getchar() != '\n');

	if(num == 0)
		return;

	tmp = list_compare(list,&num,employee_num_check);//员工编号检查
	if(tmp == NULL)
		printf("输入有误\n");
	else
	{
		ret = package_send(socketfd,WAITER,DEL,sizeof(num),&num);
		if(ret < 0)
		{
			printf("send data error in : %s %d\n",__FILE__,__LINE__);
			list_destroy(list);
			return;
		}

		ret = read(socketfd,&head,sizeof(head));
		if(ret < 0)
		{
			printf("read data error in : %s %d\n",__FILE__,__LINE__);
			list_destroy(list);
			return;
		}
		position(2,0,0);
		if(head.opt == OK)
		{
			printf("删除成功!\n");
			list_destroy(list);
			sleep(1);
			return;
		}
		else if(head.opt == FAIL)
		{
			printf("删除失败!\n");
			list_destroy(list);
			sleep(1);
			return;
		}
	}
	list_destroy(list);
}

void employee_mod(int socketfd)
{

}


void salary_mod()
{

}

/****************************************
 *function:添加菜式
 ***************************************/
void food_add(int socketfd)
{
	int ret,flag = 0,n;
	char select;
	pack_head_t head;
	dishes_t dish;
	list_t* list_head = NULL; //菜式链表头指针


	for(n=1;;n++)
	{
		//菜品类选择
		for(;;)
		{
			system("clear");
			dish_menu();
			position(2,4,0);
			printf("菜  类  序  号：");
			scanf("%c",&select);
			while(getchar() != '\n');
			if(select >= CLOD && select <= DRINK)
				break;
			else
				printf("无 该 菜 品 类!\n");
		}
		//菜品类名字
		switch(select)
		{
			case CLOD:
				strcpy(dish.class_name,"凉  菜");
				break;
			case VEGETABLE:
				strcpy(dish.class_name,"素  菜");
				break;
			case MEATE:
				strcpy(dish.class_name,"荤  菜");
				break;
			case DRY:
				strcpy(dish.class_name,"干  锅");
				break;
			case SPECIAL:
				strcpy(dish.class_name,"招牌菜");
				break;
			case PLATE:
				strcpy(dish.class_name,"铁  板");
				break;
			case SOUP:
				strcpy(dish.class_name,"汤  类");
				break;
			case DRINK:
				strcpy(dish.class_name,"酒  水");
				break;
			default:
				strcpy(dish.class_name,"NULL");
				break;
		}
		//菜品名称和价格
		position(1,4,0);
		printf("菜  品  名  称：");
		//gets(dish.name);
		ret = read(STDOUT_FILENO,dish.name,sizeof(dish.name));
		dish.name[ret-1] = '\0';
		position(1,4,0);
		printf("菜  品  价  格：");
		scanf("%f",&dish.price);
		while(getchar() != '\n');

		//添加菜式到链表中
		list_head = list_create(list_head,&dish,sizeof(dish));

		for(;;)
		{
			printf("是否继续添加菜品(y/n) ? ");
			scanf("%c",&select);
			while(getchar() != '\n');

			if(select == 'Y' || select == 'y' || select == 'N' || select == 'n')
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

	//发送菜式链表数据
	ret = package_list_send(socketfd,FOOD,ADD,sizeof(dishes_t),n,list_head);
	if(ret < 0)
	{
		printf("菜品添加失败\n");
		sleep(1);
		list_destroy(list_head);//销毁菜式链表
		return;
	}

	ret = read(socketfd,&head,sizeof(head));
	if(ret < 0)
	{
		printf("菜品添加失败\n");
		sleep(1);
		list_destroy(list_head);//销毁菜式链表
		return;
	}
	else if(ret == 0)
	{
		printf("与服务器断开连接，菜品添加失败!\n");
		sleep(1);
		list_destroy(list_head);//销毁菜式链表
		return;
	}

	if(head.opt == OK)
	{
		printf("菜品添加成功!\n");
		sleep(1);
	}
	else
	{
		printf("菜品添加失败\n");
		sleep(1);
	}
	list_destroy(list_head);//销毁菜式链表
}


/***************************************
 *function:保存菜式信息
 **************************************/
list_t* food_info(int socketfd,char select)
{
	list_t* list_head = NULL;
	dishes_t dish;
	pack_head_t head;
	int ret,n;

	ret = package_send(socketfd,FOOD,SHOW,sizeof(select),&select);
	if(ret < 0)
	{
		printf("查看失败!\n");
		printf("%s %d\n",__FILE__,__LINE__);
		sleep(1);
		return NULL;
	}
	ret = read(socketfd,&head,sizeof(head));

	//读取菜式的数量
	ret = read(socketfd,&n,sizeof(n));
	if(ret < 0)
	{
		printf("查看失败!\n");
		printf("%s %d\n",__FILE__,__LINE__);
		sleep(1);
		return NULL;
	}
	else if(ret == 0)
	{
		printf("与服务器断开连接，查看失败!\n");
		sleep(1);
		return NULL;
	}
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
				return NULL;
			}
			else if(ret == 0)
			{
				printf("与服务器断开连接，查看失败!\n");
				sleep(1);
				list_destroy(list_head);
				return NULL;
			}

			list_head = list_create(list_head,&dish,sizeof(dish));
		}
		return list_head;
	}
	else
		return NULL;
}

/******************************************
 *function:菜式查看
 ******************************************/
void food_show(int socketfd)
{
	int ret,n,flag;
	pack_head_t head;
	dishes_t dish;
	list_t* list_head = NULL;
	char select;

	for(;;)
	{
		system("clear");
		dish_menu();
		for(;;)
		{
			position(2,4,0);
			printf("菜  类  序  号(0取消)：");
			scanf("%c",&select);
			while(getchar() != '\n');
			if(select == '0')
				return;

			else if(select >= CLOD && select <= DRINK)
				break;
			else
				printf("无 该 菜 品 类!\n");
		}

		list_head = food_info(socketfd,select);
		if(list_head == NULL)
		{
			system("clear");
			position(4,5,0);
			printf("当前无菜式!\n");
			sleep(1);
			list_destroy(list_head);
			list_head = NULL;
			continue;
		}

		system("clear");
		position(1,0,0);
		printf("编 号\t菜  类\t菜    名\t价格(元)\n");
		list_show(list_head,dish_look);//菜式显示
		list_destroy(list_head);
		list_head = NULL;
	//	getchar();//阻塞在此，一直显示菜式

		for(;;)
		{
			flag = 0;
			position(2,0,0);
			printf("是否继续查看(y/n) ? ");
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
		{
			list_destroy(list_head);
			break;
		}
	}
}

void food_mod()
{

}

/****************************************
 *function:删除菜式
 ****************************************/
void food_del(int socketfd)
{
	int ret,n,flag,num;
	pack_head_t head;
	dishes_t dish,*tmp;
	list_t* list_head = NULL;
	char select = 'a';

	for(;;)
	{
		list_head = food_info(socketfd,select);
		if(list_head == NULL)
		{
			system("clear");
			position(4,5,0);
			printf("当前无菜式!\n");
			sleep(1);
			return;
		}

		system("clear");
		position(1,0,0);
		printf("编 号\t菜  类\t菜    名\t价格(元)\n");
		list_show(list_head,dish_look);//菜式显示

		for(;;)
		{
			position(2,4,0);
			printf("选择想要删除的菜式编号(0取消)：");
			scanf("%d",&num);
			while(getchar() != '\n');
			if(num == 0)
			{
				list_destroy(list_head);
				return;
			}

			tmp = list_compare(list_head,&num,dish_num_check);
			if(tmp == NULL)
				printf("输入有误!");
			else
				break;
		}

		ret = package_send(socketfd,FOOD,DEL,sizeof(num),&num);
		if(ret < 0)
		{
			printf("send error in : %s %d\n",__FILE__,__LINE__);
			list_destroy(list_head);
			return;
		}
		list_destroy(list_head);

		ret = read(socketfd,&head,sizeof(head));
		if(ret < 0)
		{
			printf("read error in : %s %d\n",__FILE__,__LINE__);
			list_destroy(list_head);
			sleep(1);
			return;
		}
		else if(ret == 0)
		{
			printf("read error in : %s %d\n",__FILE__,__LINE__);
			list_destroy(list_head);
			sleep(1);
			return;
		}
		if(head.opt == OK)
			printf("删除成功!\n");
		else if(head.opt == FAIL)
			printf("删除失败!\n");

		for(;;)
		{
			flag = 0;
			position(2,0,0);
			printf("是否继续删除(y/n) ? ");
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

/**********************************************
 *function:添加原材料
 **********************************************/
void material_add(int socketfd)
{
	int ret,flag = 0,n;
	char select;
	pack_head_t head;
	marterial_t marterial;
	list_t* list_head = NULL; //菜式链表头指针

	system("clear");
	marter_menu();

	for(n=1;;n++)
	{
		//原料选择
		for(;;)
		{
			position(2,4,0);
			printf("原 材 料 序 号：");
			scanf("%c",&select);
			while(getchar() != '\n');
			if(select >= FLAVOUR && select <= LETTUCE)
				break;
			if(select >= 'a' && select <= 'k')
				break;
			else
				printf("无 该 原 材 料!\n");
		}
		//原料名字
		switch(select)
		{
			case FLAVOUR:
				strcpy(marterial.name,"调料");
				break;
			case FLOUR:
				strcpy(marterial.name,"面");
				break;
			case RICE:
				strcpy(marterial.name,"米");
				break;
			case DRINKS:
				strcpy(marterial.name,"酒水");
				break;
			case POTATO:
				strcpy(marterial.name,"土豆");
				break;
			case GREEN:
				strcpy(marterial.name,"青菜");
				break;
			case CUKE:
				strcpy(marterial.name,"黄瓜");
				break;
			case TOMATO:
				strcpy(marterial.name,"西红柿");
				break;
			case LETTUCE:
				strcpy(marterial.name,"莴笋");
				break;
			case PEPPER:
				strcpy(marterial.name,"青椒");
				break;
			case AGARIC:
				strcpy(marterial.name,"木耳");
				break;
			case BEAN_SURD:
				strcpy(marterial.name,"豆腐");
				break;
			case EGG:
				strcpy(marterial.name,"鸡蛋");
				break;
			case GARLIC:
				strcpy(marterial.name,"蒜苔");
				break;
			case LONG_BEAN:
				strcpy(marterial.name,"豆角");
				break;
			case EGGPLANT:
				strcpy(marterial.name,"茄子");
				break;
			case CHICKEN:
				strcpy(marterial.name,"鸡肉");
				break;
			case MEAT:
				strcpy(marterial.name,"猪肉");
				break;
			case FISH:
				strcpy(marterial.name,"鱼肉");
				break;
			case INTES:
				strcpy(marterial.name,"肥肠");
				break;
			default:
				strcpy(marterial.name,"NULL");
				break;
		}
		//菜品名称和价格
		position(1,4,0);
		printf("原  料  价  格：");
		scanf("%f",&marterial.price);
		position(1,4,0);
		printf("添  加  数  量：");
		scanf("%f",&marterial.repertory);
		while(getchar() != '\n');

		//添加菜式到链表中
		list_head = list_create(list_head,&marterial,sizeof(marterial));

		for(;;)
		{
			printf("是否继续添加原材料(y/n) ? ");
			scanf("%c",&select);
			while(getchar() != '\n');

			if(select == 'Y' || select == 'y' || select == 'N' || select == 'n')
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

	//发送菜式链表数据
	ret = package_list_send(socketfd,MARTERIAL,ADD,sizeof(marterial),n,list_head);
	if(ret < 0)
	{
		printf("原料添加失败\n");
		printf("%s %d\n",__FILE__,__LINE__);
		sleep(1);
		list_destroy(list_head);//销毁菜式链表
		return;
	}

	ret = read(socketfd,&head,sizeof(head));
	if(ret < 0)
	{
		printf("原料添加失败\n");
		printf("%s %d\n",__FILE__,__LINE__);
		sleep(1);
		list_destroy(list_head);//销毁菜式链表
		return;
	}
	else if(ret == 0)
	{
		printf("与服务器断开连接，菜品添加失败!\n");
		sleep(1);
		list_destroy(list_head);//销毁菜式链表
		return;
	}

	if(head.opt == OK)
	{
		printf("原料添加成功!\n");
		sleep(1);
	}
	else if(head.opt == FAIL)
	{
		printf("原料添加失败\n");
		printf("%s %d\n",__FILE__,__LINE__);
		sleep(1);
	}
	list_destroy(list_head);//销毁菜式链表
}

/*****************************************
 *function:原材料查看
 *****************************************/
void material_show(int socketfd)
{
	int ret,n,flag;
	pack_head_t head;
	marterial_t marterial;
	list_t* list_head = NULL;
	char select;

	for(;;)
	{
		system("clear");
		marter_menu();
		for(;;)
		{
			position(2,4,0);
			printf("原  料  序  号(0取消)：");
			scanf("%c",&select);
			while(getchar() != '\n');
			
			if(select == '0')
				return;

			if(select >= FLAVOUR && select <= LETTUCE)
				break; 
			if(select >= 'a' && select <= 'k')
				break;
			else
				printf("无 该 材  料!\n");
		}

		ret = package_send(socketfd,MARTERIAL,SHOW,sizeof(select),&select);
		if(ret < 0)
		{
			printf("查看失败!\n");
			printf("%s %d\n",__FILE__,__LINE__);
			sleep(1);
			return;
		}
		read(socketfd,&head,sizeof(head));

		//读取原料的数量
		ret = read(socketfd,&n,sizeof(n));
		if(ret < 0)
		{
			printf("查看失败!\n");
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
		if( n != 0)
		{
			for(int i=0;i<n;i++)
			{
				ret = read(socketfd,&marterial,sizeof(marterial));
				if(ret < 0)
				{
					printf("查看失败!\n");
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

				list_head = list_create(list_head,&marterial,sizeof(marterial));
			}
			system("clear");
			position(1,0,0);
			printf("编 号\t原 料\t价格(元/kg)\t库存(kg)\n");
			list_show(list_head,marterial_look);//菜式显示
			list_destroy(list_head);
			list_head = NULL;
			getchar();//阻塞在此，一直显示菜式
		}
		else
			printf("暂无该材料!\n");

		for(;;)
		{
			flag = 0;
			position(1,0,0);
			printf("是否继续查看(y/n) ? ");
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

void material_mod()
{

}

/***************************************
 *function:消费信息查看
 ***************************************/
void consum_show(int socketfd)
{
	int ret,n,num,flag;
	list_t* list = NULL;
	consum_t* tmp = NULL,consum;
	chef_t chef;
	char select;
	pack_head_t head;

	for(;;)
	{
		system("clear");
		flag = 0;
		ret = package_send(socketfd,CONSUME,SHOW,sizeof(flag),&flag);
		if(ret < 0)
		{
			printf("消费信息查看失败!\n");
			printf("error in : %s %d\n",__FILE__,__LINE__);
			sleep(1);
			return;
		}

		read(socketfd,&head,sizeof(head));
		ret = read(socketfd,&n,sizeof(n));
		if(ret < 0)
		{
			printf("消费信息查看失败!\n");
			printf("error in : %s %d\n",__FILE__,__LINE__);
			sleep(1);
			return;
		}
		else if(ret == 0)
		{
			printf("与服务器断开连接!\n");
			sleep(1);
			return;
		}

		if(n == 0)
		{
			position(5,5,0);
			printf("暂无消费信息\n");
			sleep(1);
			return;
		}

		for(int i=0;i<n;i++)
		{
			ret = read(socketfd,&consum,sizeof(consum));
			if(ret < 0)
			{
				printf("消费信息查看失败!\n");
				printf("error in : %s %d\n",__FILE__,__LINE__);
				sleep(1);
				return;
			}
			else if(ret == 0)
			{
				printf("与服务器断开连接!\n");
				sleep(1);
				return;
			}
			list = list_create(list,&consum,sizeof(consum));
		}

		if(list == NULL)
		{
			position(5,5,0);
			printf("暂无消费信息!\n");
			sleep(1);
			return;
		}

		position(1,0,0);
		printf("编 号\t桌号\t菜数\t状态1\t状态2\t消费总额(元)\t提交时间\t结帐时间\t日    期\n");
		list_show(list,consum_look);

		for(;;)
		{
			position(1,4,0);
			printf("请选择要查看的消费编号(0取消)：");
			scanf("%d",&num);
			while(getchar() != '\n');

			if(num == 0)
			{
				list_destroy(list);
				return;
			}

			tmp = list_compare(list,&num,consum_compare);
			if(tmp != NULL)
				break;
			else
				printf("输入有误!\n");
		}

		list_destroy(list);
		list = NULL;

		ret = package_send(socketfd,ORDER,SHOW,sizeof(num),&num);
		if(ret < 0)
		{
			printf("菜式信息查看失败!\n");
			printf("error in : %s %d\n",__FILE__,__LINE__);
			sleep(1);
			return;
		}

		read(socketfd,&head,sizeof(head));
		ret = read(socketfd,&n,sizeof(n));
		if(ret < 0)
		{
			printf("菜式信息查看失败!\n");
			printf("error in : %s %d\n",__FILE__,__LINE__);
			sleep(1);
			return;
		}
		else if(ret == 0)
		{
			printf("与服务器断开连接!\n");
			sleep(1);
			return;
		}

		if(n != 0)
		{
			for(int i=0;i<n;i++)
			{
				ret = read(socketfd,&chef,sizeof(chef));
				if(ret < 0)
				{
					printf("菜式信息查看失败!\n");
					printf("error in : %s %d\n",__FILE__,__LINE__);
					sleep(1);
					return;
				}
				else if(ret == 0)
				{
					printf("与服务器断开连接!\n");
					sleep(1);
					return;
				}

				list = list_create(list,&chef,sizeof(chef));
			}

			system("clear");
			position(1,0,0);
			printf("编 号\t菜 名\t价格(元)\t数量\t状态\t准备时间\t完成时间\n");
			list_show(list,look_dish);

			list_destroy(list);
			list = NULL;
		}

		for(;;)
		{
			flag = 0;
			position(1,0,0);
			printf("是否继续查看(y/n) ? ");
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
 *finction:结帐
 *************************************/
void consum_pay(int socketfd)
{
	int ret,n,num,flag,flag1 = 0;
	list_t* list = NULL;
	consum_t* tmp = NULL,consum;
	chef_t chef;
	char select;
	char date[20];
	login_t login;
	pack_head_t head;
	member_t member;

	system("clear");
	flag = -1;
	ret = package_send(socketfd,CONSUME,SHOW,sizeof(flag),&flag);
	if(ret < 0)
	{
		printf("消费信息查看失败!\n");
		printf("error in : %s %d\n",__FILE__,__LINE__);
		return;
	}

	read(socketfd,&head,sizeof(head));
	ret = read(socketfd,&n,sizeof(n));
	if(ret < 0)
	{
		printf("消费信息查看失败!\n");
		printf("error in : %s %d\n",__FILE__,__LINE__);
		return;
	}
	else if(ret == 0)
	{
		printf("与服务器断开连接!\n");
		return;
	}

	if(n == 0)
	{
		position(5,5,0);
		printf("暂无消费信息\n");
		return;
	}

	for(int i=0;i<n;i++)
	{
		ret = read(socketfd,&consum,sizeof(consum));
		if(ret < 0)
		{
			printf("消费信息查看失败!\n");
			printf("error in : %s %d\n",__FILE__,__LINE__);
			return;
		}
		else if(ret == 0)
		{
			printf("与服务器断开连接!\n");
			return;
		}
		list = list_create(list,&consum,sizeof(consum));
	}

	if(list == NULL)
	{
		position(5,5,0);
		printf("暂无消费信息!\n");
		return;
	}

	position(1,0,0);
	printf("编 号\t桌号\t菜数\t状态1\t状态2\t消费总额(元)\t提交时间\t结帐时间\t日    期\n");
	list_show(list,consum_look);

	for(;;)
	{
		position(1,4,0);
		printf("请选择要结帐的消费编号(0取消)：");
		scanf("%d",&num);
		while(getchar() != '\n');
		if(num == 0)
		{
			list_destroy(list);
			return;
		}

		tmp = list_compare(list,&num,consum_compare);
		if(tmp != NULL)
			break;
		else
			printf("输入有误!\n");
	}

	for(;;)
	{
		flag = 0;
		position(1,4,0);
		printf("是否是会员(y/n) ? ");
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

	if(select == 'N' || select == 'n')
	{
		for(;;)
		{
			flag = 0;
			position(1,4,0);
			printf("是否办理会员(y/n) ? ");
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
		{
			if(member_add(socketfd) < 0)//办理会员
				return;
			flag1 = 1;
		}
		else//非会员方式结帐
		{
			for(;;)
			{
				flag = 0;
				position(1,4,0);
				printf("是否确认结帐(y/n) ? ");
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
			{
				consum.num = num;
				count_time(date,consum.pay_time);
				ret = package_send(socketfd,CONSUME,PAY,sizeof(consum),&consum);	
				if(ret < 0)
				{
					printf("数据发送错误!\n");
					printf("error in : %s %d\n",__FILE__,__LINE__);
					list_destroy(list);
					return;
				}

				ret = read(socketfd,&head,sizeof(head));
				if(ret < 0)
				{
					printf("结帐失败!\n");
					printf("error in : %s %d\n",__FILE__,__LINE__);
					list_destroy(list);
					return;
				}
				else if(ret == 0)
				{
					printf("与服务器断开连接!\n");
					list_destroy(list);
					return;
				}

				if(head.opt == OK)
				{
					printf("结帐成功!\n");
					sleep(1);
					list_destroy(list);
					return;
				}
				else if(head.opt == FAIL)
				{
					printf("结帐失败!\n");
					sleep(1);
					list_destroy(list);
					return;	
				}
			}
			else
			{
				list_destroy(list);
				return;
			}
		}
	}

	/*******会员方式结帐*********/
	if(flag1 == 1)
		system("clear");
	position(1,4,0);
	printf("手机号 ：");
	scanf("%s",login.account);
	position(1,4,0);
	printf("密  码：");
	scanf("%s",login.passward);
	while(getchar() != '\n');

	login.flag = MEMBER;
	ret = package_send(socketfd,ADMIN,LOGIN,sizeof(login_t),&login);
	if(ret < 0)
	{
		printf("结帐失败!\n");
		printf("error in : %s %d\n",__FILE__,__LINE__);
		list_destroy(list);
		sleep(1);
		return;
	}

	ret = read(socketfd,&head,sizeof(head));//读取会员的积分
	if(ret < 0)
	{
		printf("结帐失败!\n");
		printf("error in : %s %d\n",__FILE__,__LINE__);
		list_destroy(list);
		sleep(1);
		return;
	}
	else if(ret == 0)
	{
		printf("与服务器断开连接!\n");
		list_destroy(list);
		return;
	}

	if(head.opt < 0)
	{
		printf("帐号或密码不正确!\n");
		list_destroy(list);
		sleep(1);
		return;
	}
	else
	{
		position(1,1,0);
		if(head.opt >= 0 && head.opt <= 100)//不打折
		{
			printf("您的消费总额为：%.1f 元，会员积分为 ：%d 分,未到达优惠积分额度\n",tmp->price,head.opt);
		}
		else if(head.opt > 100 && head.opt <= 200)//打九折
		{
			printf("您的消费总额为：%.1f 元，会员积分为 ：%d 分,经九折优惠后消费额为 ：%.1f 元\n",tmp->price,head.opt,tmp->price * 0.9);

		}
		else if(head.opt > 200 && head.opt <= 300)//打八折
		{
			printf("您的消费总额为：%.1f 元，会员积分为 ：%d 分,经八折优惠后消费额为 ：%.1f 元\n",tmp->price,head.opt,tmp->price * 0.8);

		}
		else//打七折
		{
			printf("您的消费总额为：%.1f 元，会员积分为 ：%d 分,经七折优惠后消费额为 ：%.1f 元\n",tmp->price,head.opt,tmp->price * 0.7);

		}

		for(;;)
		{
			flag = 0;
			position(1,4,0);
			printf("是否确认结帐(y/n) ? ");
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
		{
			consum.num = num;
			count_time(date,consum.pay_time);
			ret = package_send(socketfd,CONSUME,PAY,sizeof(consum),&consum);	
			if(ret < 0)
			{
				printf("数据发送错误!\n");
				printf("error in : %s %d\n",__FILE__,__LINE__);
				list_destroy(list);
				return;
			}

			ret = read(socketfd,&head,sizeof(head));
			if(ret < 0)
			{
				printf("结帐失败!\n");
				printf("error in : %s %d\n",__FILE__,__LINE__);
				list_destroy(list);
				return;
			}
			else if(ret == 0)
			{
				printf("与服务器断开连接!\n");
				list_destroy(list);
				return;
			}
			if(head.opt == FAIL)
			{
				printf("结帐失败!\n");
				sleep(1);
				list_destroy(list);
				return;	
			}

			strcpy(member.tel,login.account);
			member.integral = tmp->price;
			ret = package_send(socketfd,MEMBER,PAY,sizeof(member),&member);
			if(ret < 0)
			{
				printf("数据发送错误!\n");
				printf("error in : %s %d\n",__FILE__,__LINE__);
				list_destroy(list);
				return;
			}

			ret = read(socketfd,&head,sizeof(head));
			if(ret < 0)
			{
				printf("结帐失败!\n");
				printf("error in : %s %d\n",__FILE__,__LINE__);
				list_destroy(list);
				return;
			}
			else if(ret == 0)
			{
				printf("与服务器断开连接!\n");
				list_destroy(list);
				return;
			}

			if(head.opt == OK)
			{
				printf("结帐成功!\n");
				sleep(1);
				list_destroy(list);
				return;
			}
			else if(head.opt == FAIL)
			{
				printf("结帐失败!\n");
				sleep(1);
				list_destroy(list);
				return;
			}	
		}
		else
		{
			list_destroy(list);
			return;
		}
	}
}

/*
   void hisconsum_look()
   {

   }
 */

/***************************************************************
 *function:删除历史订单
 ***************************************************************/
void hisconsum_del(int socketfd)
{
	int ret,n,num,flag;
	list_t* list = NULL;
	consum_t* tmp = NULL,consum;
	chef_t chef;
	char select;
	pack_head_t head;

	for(;;)
	{
		system("clear");
		flag = 0;
		ret = package_send(socketfd,CONSUME,SHOW,sizeof(flag),&flag);
		if(ret < 0)
		{
			printf("消费信息查看失败!\n");
			printf("error in : %s %d\n",__FILE__,__LINE__);
			return;
		}

		read(socketfd,&head,sizeof(head));
		ret = read(socketfd,&n,sizeof(n));
		if(ret < 0)
		{
			printf("消费信息查看失败!\n");
			printf("error in : %s %d\n",__FILE__,__LINE__);
			return;
		}
		else if(ret == 0)
		{
			printf("与服务器断开连接!\n");
			return;
		}

		if(n == 0)
		{
			position(5,5,0);
			printf("暂无消费信息\n");
			return;
		}

		for(int i=0;i<n;i++)
		{
			ret = read(socketfd,&consum,sizeof(consum));
			if(ret < 0)
			{
				printf("消费信息查看失败!\n");
				printf("error in : %s %d\n",__FILE__,__LINE__);
				return;
			}
			else if(ret == 0)
			{
				printf("与服务器断开连接!\n");
				return;
			}
			list = list_create(list,&consum,sizeof(consum));
		}

		if(list == NULL)
		{
			position(5,5,0);
			printf("暂无消费信息!\n");
			return;
		}

		position(1,0,0);
		printf("编 号\t桌号\t菜数\t状态1\t状态2\t消费总额(元)\t提交时间\t结帐时间\t日    期\n");
		list_show(list,consum_look);

		for(;;)
		{
			position(1,4,0);
			printf("请选择想要删除的消费编号(0取消)：");
			scanf("%d",&num);
			while(getchar() != '\n');
			
			if(num == 0)
			{
				list_destroy(list);
				return;
			}
			while(getchar() != '\n');

			tmp = list_compare(list,&num,consum_compare);
			if(tmp != NULL)
				break;
			else
				printf("输入有误!\n");
		}

		list_destroy(list);
		list = NULL;

		ret = package_send(socketfd,CONSUME,DEL,sizeof(num),&num);
		if(ret < 0)
		{
			printf("数据发送失败!\n");
			printf("error in : %s %d\n",__FILE__,__LINE__);
			return;
		}

		ret = read(socketfd,&head,sizeof(head));
		if(ret < 0)
		{
			printf("菜式信息查看失败!\n");
			printf("error in : %s %d\n",__FILE__,__LINE__);
			return;
		}
		else if(ret == 0)
		{
			printf("与服务器断开连接!\n");
			return;
		}

		if(head.opt == OK)
		{
			printf("删除成功\n");
			sleep(1);
			return;
		}
		else if(head.opt == FAIL)
		{
			printf("删除失败\n");
			sleep(1);
			return;
		}
	}	
}

/********************************
 *function:打印菜式
 ********************************/
void dish_look(void* data)
{
	dishes_t* dish = (dishes_t*)data;

	if(dish == NULL)
		return;
	position(1,0,0);
	printf("%d\t%s\t%s\t%.1f\n",dish->num,dish->class_name,dish->name,dish->price);
}

/*************************************
 *function:打印原材料
 **************************************/
void marterial_look(void* data)
{
	marterial_t* marterial = (marterial_t*)data;

	if(marterial == NULL)
		return;
	position(1,0,0);
	printf("%d\t%s\t%.1f\t\t%.1f\n",marterial->num,marterial->name,marterial->price,marterial->repertory);
}

/*************************************
 *function:打印订菜信息
 *************************************/
void look_dish(void* data)
{
	order_t order = ((chef_t*)data)->order;
	if(data == NULL)
		return;

	printf("%d\t\t%s\t%.1f\t%d\t%s\t%s\t%s\n",order.num,order.name,order.price,order.dish_num,order.state,order.s_time,order.e_time);
}

void consum_look(void* argv)
{
	consum_t* consum = (consum_t*)argv;

	printf("%d\t%d\t%d\t%s\t%s\t%.1f\t\t%s\t%s\t%s\n",consum->num,consum->table_num,consum->dish_num,consum->state,consum->state1,consum->price,consum->submit_time,consum->pay_time,consum->date);
}

int consum_compare(void* num,void* data)
{
	if(num == NULL || data == NULL)
		return -1;

	if(((consum_t*)data)->num == *(int*)num)
		return 0;
	else
		return -1;
}

/**************************************
 *function:打印员工编号和账户
 **************************************/
void employee_num(void* data)
{
	if(data == NULL)
		return;

	regist_t* employee = (regist_t*)data;

	printf("(%d)%s\t",employee->num,employee->account);
}

/**************************************
 *function:打印会员编号和账户
 **************************************/
void member_num(void* data)
{
	if(data == NULL)
		return;

	member_t* member = (member_t*)data;
	
	printf("(%d)%s\t",member->num,member->name);
}

/*************************************************
 *function:检查输入的员工编号是否匹配
 *************************************************/
int employee_num_check(void* data,void* argv)
{
	if(data == NULL || argv == NULL)
		return -1;

	if(((regist_t*)argv)->num == *(int*)data)
		return 0;
	else
		return -1;
}

/*************************************************
 *function:检查输入的会员编号是否匹配
 *************************************************/
int member_num_check(void* data,void* argv)
{
	if(data == NULL || argv == NULL)
		return -1;

	if(((member_t*)argv)->num == *(int*)data)
		return 0;
	else
		return -1;
}

/*************************************************
 *function:检查输入的菜式编号是否匹配
 *************************************************/
int dish_num_check(void* data,void* argv)
{
	if(data == NULL || argv == NULL)
		return -1;

	if(((dishes_t*)argv)->num == *(int*)data)
		return 0;
	else
		return -1;
}

/****************************************************
 *function:打印员工信息
 ****************************************************/
void employee_printf(regist_t* ptr)
{
	if(ptr == NULL)
		return;
	position(1,1,0);
	printf("编    号：%d",ptr->num);
	position(2,1,0);
	printf("帐    号：%s",ptr->account);
	position(2,1,0);
	printf("密    码：%s",ptr->passward);
	position(2,1,0);
	printf("姓    名：%s",ptr->name);
	position(2,1,0);
	printf("年    龄：%d",ptr->age);
	position(2,1,0);
	printf("性    别：%s",ptr->sex);
	position(2,1,0);
	printf("联系方式：%s",ptr->tel);
	position(2,1,0);
	printf("证件号码：%s",ptr->id);
	position(2,1,0);
	printf("联系地址：%s",ptr->address);
	position(2,1,0);
	printf("职    位：%s",ptr->depart);
	position(2,1,0);
	printf("薪    水：%.1f",ptr->salary);
}

/****************************************************
 *function:打印会员信息
 ****************************************************/
void member_printf(member_t* ptr)
{
	if(ptr == NULL)
		return;
	position(1,1,0);
	printf("编    号：%d",ptr->num);
	position(2,1,0);
	printf("姓    名：%s",ptr->name);
	position(2,1,0);
	printf("性    别：%s",ptr->sex);
	position(2,1,0);
	printf("年    龄：%d",ptr->age);
	position(2,1,0);
	printf("联系方式：%s",ptr->tel);
	position(2,1,0);
	printf("证件号码：%s",ptr->id);
	position(2,1,0);
	printf("积    分：%.1f",ptr->integral);
	position(2,1,0);
	printf("密    码：%s",ptr->passward);
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
		sleep(1);
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
		{	system("clear");
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
