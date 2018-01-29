#include "admin_menu.h"

void head_menu()
{
	system("clear");
	position(1,3,0);
	printf("************************************************************");	
	position(1,3,0);
	printf("**************  酒   店   管   理   系   统  ***************");
	position(1,3,0);
	printf("**************                               ***************");	
	position(1,3,0);
	printf("**************       吧   台   终   端       ***************");	
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
	printf("|          1 会   员   管   理          |");
	position(1,4,1);
	printf("|                                       |");
	position(1,4,1);
	printf("|          2 员   工   管   理          |");
	position(1,4,1);
	printf("|                                       |");
	position(1,4,1);
	printf("|          3 菜   品   管   理          |");
	position(1,4,1);
	printf("|                                       |");
	position(1,4,1);
	printf("|          4 材   料   管   理          |");
	position(1,4,1);
	printf("|                                       |");
	position(1,4,1);
	printf("|          5 消   费   管   理          |");
	position(1,4,1);
	printf("|                                       |");
	position(1,4,1);
	printf("|          0 退             出          |");
	position(1,4,1);
	printf("-----------------------------------------");
	position(1,0,0);
}

void member_menu()
{
	head_menu();
	position(1,5,6);
	printf("请 输 入 选 项");
	position(1,4,1);
	printf("-----------------------------------------");
	position(1,4,1);
	printf("|          1 会   员   办   理          |");
	position(1,4,1);
	printf("|                                       |");
	position(1,4,1);
	printf("|          2 会 员 信 息 查 看          |");
	position(1,4,1);
	printf("|                                       |");
	position(1,4,1);
	printf("|          3 会 员 信 息 修 改          |");
	position(1,4,1);
	printf("|                                       |");
	position(1,4,1);
	printf("|          4 会   员   删   除          |");
	position(1,4,1);
	printf("|                                       |");
	position(1,4,1);
	printf("|          0 返             回          |");
	position(1,4,1);
	printf("-----------------------------------------");
	position(1,0,0);
}

void employee_menu()
{
	head_menu();
	position(1,5,6);
	printf("请 输 入 选 项");
	position(1,4,1);
	printf("-----------------------------------------");
	position(1,4,1);
	printf("|          1 添   加   员   工          |");
	position(1,4,1);
	printf("|                                       |");
	position(1,4,1);
	printf("|          2 查 看 员 工 信 息          |");
	position(1,4,1);
	printf("|                                       |");
	position(1,4,1);
	printf("|          3 修 改 员 工 信 息          |");
	position(1,4,1);
	printf("|                                       |");
	position(1,4,1);
	printf("|          4 删   除   员   工          |");
	position(1,4,1);
	printf("|                                       |");
	position(1,4,1);
	printf("|          5 薪   酬   管   理          |");
	position(1,4,1);
	printf("|                                       |");
	position(1,4,1);
	printf("|          0 返             回          |");
	position(1,4,1);
	printf("-----------------------------------------");
	position(1,0,0);
}

void food_menu()
{
	head_menu();
	position(1,5,6);
	printf("请 输 入 选 项");
	position(1,4,1);
	printf("-----------------------------------------");
	position(1,4,1);
	printf("|          1 菜   品   添   加          |");
	position(1,4,1);
	printf("|                                       |");
	position(1,4,1);
	printf("|          2 菜   品   查   看          |");
	position(1,4,1);
	printf("|                                       |");
	position(1,4,1);
	printf("|          3 菜   品   修   改          |");
	position(1,4,1);
	printf("|                                       |");
	position(1,4,1);
	printf("|          4 菜   品   删   除          |");
	position(1,4,1);
	printf("|                                       |");
	position(1,4,1);
	printf("|          0 返             回          |");
	position(1,4,1);
	printf("-----------------------------------------");
	position(1,0,0);
}

void material_menu()
{
	head_menu();
	position(1,5,6);
	printf("请 输 入 选 项");
	position(1,4,1);
	printf("-----------------------------------------");
	position(1,4,1);
	printf("|          1 材   料   添   加          |");
	position(1,4,1);
	printf("|                                       |");
	position(1,4,1);
	printf("|          2 材   料   查   看          |");
	position(1,4,1);
	printf("|                                       |");
	position(1,4,1);
	printf("|          3 材   料   修   改          |");
	position(1,4,1);
	printf("|                                       |");
	position(1,4,1);
	printf("|          0 返             回          |");
	position(1,4,1);
	printf("-----------------------------------------");
	position(1,0,0);
}

void consum_menu()
{
	head_menu();
	position(1,5,6);
	printf("请 输 入 选 项");
	position(1,4,1);
	printf("-----------------------------------------");
	position(1,4,1);
	printf("|          1 结             帐          |");
	position(1,4,1);
	printf("|                                       |");
	position(1,4,1);
	printf("|          2 查   看   消   费          |");
	position(1,4,1);
	printf("|                                       |");
	position(1,4,1); 
	printf("|          3 删 除 历 史 消 费          |");
	position(1,4,1);
	printf("|                                       |");
	position(1,4,1);
	printf("|          0 返             回          |");
	position(1,4,1);
	printf("-----------------------------------------");
	position(1,0,0);
}

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
/*
#define     FLAVOUR     '1'      //调料
#define     FLOUR       '2'      //面
#define     RICE        '3'      //米
#define     DRINKS      '4'      //酒水
#define     POTATO      '5'      //土豆
#define     GREEN       '6'      //青菜
#define     CUKE        '7'      //黄瓜         
#define     TOMATO      '8'      //西红柿
#define     LETTUCE     '9'      //莴笋
#define     PEPPER      'a'      //青椒
#define     AGARIC      'b'      //木耳
#define     BEAN_SURD   'C'      //豆腐
#define     EGG         'd'      //鸡蛋
#define     GARLIC      'e'      //蒜苔
#define     LONG_BEAN   'f'      //豆角
#define     EGGPLANT    'g'      //茄子
#define     CHICKEN     'h'      //鸡肉
#define     MEAT        'i'      //猪肉
#define     FISH        'j'      //鱼肉
#define     INTES       'K'      //肥肠
*/

void main_func(int socketfd)
{
	int ret;
	for(char select = '1';select != EXIT;)
	{
		main_menu();
		scanf("%c",&select);
		while(getchar() != '\n');

		switch(select)
		{
			case ENTER:
				ret = admin_login(socketfd);
				if(ret == 0)
					man_func(socketfd);
				break;
			case EXIT:
				select = EXIT;
				break;
			default:
				printf("输入选项无效\n");
				sleep(1);
				break;
		}
	}
}

void man_func(int socketfd)
{
	for(char select = '1';select != EXIT;)
	{
		man_menu();
		scanf("%c",&select);
		while(getchar() != '\n');
		switch(select)
		{
			case MEM:
				member_func(socketfd);
				break;
			case EMPLOY:
				employee_func(socketfd);
				break;
			case DISH:
				food_func(socketfd);
				break;
			case MATER:
				material_func(socketfd);
				break;
			case CONS:
				consum_func(socketfd);
				break;
			case EXIT:
				select = EXIT;
				break;
			default:
				printf("输入选项无效\n");
				sleep(1);
				break;
		}
	}
}

void member_func(int socketfd)
{
	for(char select = '1';select != EXIT;)
	{
		member_menu();
		scanf("%c",&select);
		while(getchar() != '\n');

		switch(select)
		{
			case INCREASE:
				member_add(socketfd);
				break;
			case LOOK:
				member_show(socketfd);
				break;
			case MOD:
				member_mod(socketfd);
				break;
			case DELETE:
				member_del(socketfd);
				break;
			case EXIT:
				select = EXIT;
				break;
			default:
				printf("输入选项无效\n");
				sleep(1);
				break;
		}
	}
}


void employee_func(int socketfd)
{
	for(char select = '1';select != EXIT;)
	{
		employee_menu();
		scanf("%c",&select);
		while(getchar() != '\n');

		switch(select)
		{
			case INCREASE:
				employee_add(socketfd);
				break;
			case LOOK:
				emlpoyee_show(socketfd);
				break;
			case MOD:
				employee_mod(socketfd);
				break;
			case DELETE:
				employee_del(socketfd);
				break;
			case SALARY:
				salary_mod();
				break;
			case EXIT:
				select = EXIT;
				break;
			default:
				printf("输入选项无效\n");
				sleep(1);
				break;
		}
	}
}


void food_func(int socketfd)
{
	for(char select = '1';select != EXIT;)
	{
		food_menu();
		scanf("%c",&select);
		while(getchar() != '\n');

		switch(select)
		{
			case INCREASE:
				food_add(socketfd);
				break;
			case LOOK:
				food_show(socketfd);
				break;
			case MOD:
				food_mod();
				break;
			case DELETE:
				food_del(socketfd);
				break;
			case EXIT:
				select = EXIT;
				break;
			default:
				printf("输入选项无效\n");
				sleep(1);
				break;
		}
	}
}


void material_func(int socketfd)
{
	for(char select = '1';select != EXIT;)
	{
		material_menu();
		scanf("%c",&select);
		while(getchar() != '\n');

		switch(select)
		{
			case INCREASE:
				material_add(socketfd);
				break;
			case LOOK:
				material_show(socketfd);
				break;
			case MOD:
				material_mod();
				break;
			case EXIT:
				select = EXIT;
				break;
			default:
				printf("输入选项无效\n");
				sleep(1);
				break;
		}
	}
}

void consum_func(int socketfd)
{
	for(char select = '1';select != EXIT;)
	{
		consum_menu();
		scanf("%c",&select);
		while(getchar() != '\n');

		switch(select)
		{
			case PAY_BILL:
				consum_pay(socketfd);
				break;
			case LOOK:
				consum_show(socketfd);
				break;
		//	case HISCONSUM_LOOK:
		//		hisconsum_look();
		//		break;
			case HISCONSUM_DEL:
				hisconsum_del(socketfd);
				break;
			case EXIT:
				select = EXIT;
				break;
			default:
				printf("输入选项无效\n");
				sleep(1);
				break;
		}
	}
}

