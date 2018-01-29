#include <stdio.h>
#include <unistd.h>

//using int(*)(int, int) = pfunc;
//typedef int(*pfunc)(int, int);
int fun(int a, int b)
{
	return a+b;
}

int main()
{
	int a = 10; 
	int b = 20;
//	int (*p)(int, int);
	typedef int(*pfunc)(int, int);
	pfunc p;
	p = fun;
	
	int ret = p(a, b);
	printf("ret = %d\n", ret);
}
