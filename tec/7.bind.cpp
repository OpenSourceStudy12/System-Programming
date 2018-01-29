#include <iostream>
#include <functional>
using namespace std;
using namespace std::placeholders;

int fun(int a, int b, int c)
{
	cout << "a = " << a << ", b = " << b << ", c = " << c << endl;
	return a + b + c;
}

//	返回一个函数指针  
//	callback:	回调函数  
//	args:		回调函数的参数列表
//pfun = bind(callback, args);

int main()
{
//	cout << fun(12, 23) << endl;
	auto p1 = bind(fun, _2, 0, _1);
	p1(12, 23);

	auto p2 = bind(fun, _1, 0, _1);
	p2(12, 23);
	//_1   _2
	return 0;
}
