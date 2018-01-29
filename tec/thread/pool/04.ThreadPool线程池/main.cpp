
   //           task8
   //     task7   |  task9      ---> 任务
   //           \ | /
   //  ---------     ---------
			// |task6|
			// |     |
			// |task5|
			// |     |      -------->list
			// |     |
			// |task4|
			// |     |
   //      ---- task3 -----
   //     /    /  |  \    \
   //    /    /   |   \    \
   // [t1] [t2] [t3] [t4] [t5]   -->线程组

#include "ThreadPool.h"
#include <unistd.h>
#include <iostream>
using namespace std;

void fun()
{
	cout << "hello" << endl;
}

int main()
{
	ThreadPool pool;
	pool.setMaxQueueSize(4);
	pool.start(2);
	
	pool.run(fun);
	pool.run(fun);
	pool.run(fun);
	pool.run(fun);
	pool.run(fun);
	pool.run(fun);
	pool.run(fun);
	pool.run(fun);
}

