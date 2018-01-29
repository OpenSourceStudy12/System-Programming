#include "Thread.h"
#include <unistd.h>

void fun(int num)
{
	for (int i = 0; i < 10; i++)
	{
		printf("fun is running: num = %d\n", num);
		sleep(1);
	}
}

int main()
{
	std::function <void(void)> pfunc;//函数指针变量
	pfunc = std::bind(fun, 10);

	Thread t(pfunc);

	t.start();
	
	for (int i = 0; i < 5; i++) {
		printf("main is running\n");
		sleep(1);
	}

	t.join();

	return 0;
}
