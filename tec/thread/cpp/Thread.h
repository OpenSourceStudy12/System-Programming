#ifndef __THREAD_H
#define __THREAD_H

#include <iostream>
#include <pthread.h>
#include <stdlib.h>
#include <exception>
#include <functional>
#include "noncpyable.h"

class Thread : noncpyable
{
private:
	//using Pfunc = void(*)(void);
	using Pfunc = std::function<void(void)>;//声明函数指针（类型）
	//typedef void(*Pfunc)(void);//声明函数指针（类型）
	pthread_t pth_;
	bool started_;
	bool joined_;
	Pfunc pfunc_;//定义函数指针（变量）

public:
	Thread(const Pfunc &pfunc);
	~Thread();

	bool start();
	int join();
};

#endif
