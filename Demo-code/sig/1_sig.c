#include <stdio.h>
#include <string.h>
#include <unistd.h>
#define SIZE (sizeof(s))/(sizeof(s[0]))

typedef void(*PFUN)(int);

struct sig
{
	PFUN pfunc;
	int argv;
};

void police(int argv)
{
	printf("police is coming:%d\n", argv);
}

void fire(int argv)
{
	printf("触发事件:%d\n", argv);
}

void hospital(int argv)
{
	printf("触发事件:%d\n", argv);
}

int main()
{
	struct sig s[] = {{police, 110}, {fire, 119}, {hospital, 120}, {NULL, 0}};

	int num;

	int i;
	for (;;)
	{
		scanf("%d", &num);
		for (i = 0; i < SIZE; i++)
		{
			if (s[i].argv == num)
			{
				s[i].pfunc(s[i].argv);
				break;
			}
		}

		if (i == SIZE)
		{
			printf("没有相应的处理函数\n");
			break;
		}
		else
			printf("信号已处理\n");
	}	
	return 0;
}
