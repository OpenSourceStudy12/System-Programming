#include <iostream>
#include <functional>
using namespace std;

/*
 *可调用对象：
 *	函数
 *	函数指针
 *	类中的调用运算符重载
 *	lambda表达式
 */

int main()
{
	int n = 10;
	int (*p)(int, int) = 	//不可以指向带捕获器列表的lambda表达式
		[](int a, int b)->int
		{
			return a + b;
		};
	cout << p(2, 3) << endl;

	//函数包装器可以包装任意类型的可调用对象
	function<int(int, int)> f = 
		[n](int a, int b)->int
		{
			return a + b + n;
		};
	cout << f(2, 3) << endl;

	return 0;
}
