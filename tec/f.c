#include<stdio.h>
#include<string.h>
int fun(int ,char*);

int main()
{
	int n,num;
	char str[100];
	printf("输入进制:");
	scanf("%d",&n);
	printf("输入字符串：");
	scanf("%s",str);
	num = fun(n,str);
	printf("数字为:%d\n",num);
}


int fun(int x,char* str)
{
	int i=0,j=0,num=0,s,n = strlen(str);
	int k[100];
	while(i<n)
	{
		k[i]=str[i++]-'0';
		//i++;
	}
	while(n)
	{
		s = k[j++];
		for(i=0;i<n-1;i++)
		{
			s *= x;
		}
		num += s;
		n--;
	}
	return num;
}
