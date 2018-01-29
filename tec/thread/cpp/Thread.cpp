#include "Thread.h"

struct ThreadData { 
	using Pfunc = std::function<void(void)>;
	Pfunc pfunc_;
	ThreadData(const Pfunc &pfunc) : pfunc_(pfunc) { }
	void start()
	{
		try {
			pfunc_();
		} catch (const std::exception &e) {
			::fprintf(stderr, "reasion: %s\n", e.what());
			::abort();
		} catch (...) {
			throw;
		}
	}
};

Thread::Thread(const Pfunc &pfunc) : 
	pfunc_(pfunc),
	pth_(0),
	started_(false),
	joined_(false)
{
}

void *thread_routine(void *argv)
{
	ThreadData *data = (ThreadData*)argv;
	data->start();
	delete data;

	return NULL;
}

bool Thread::start()
{
	if (started_)
		return false;
	if (joined_)
		return false;

	int ret;
	ThreadData *data = new ThreadData(pfunc_);
	ret = pthread_create(&pth_, NULL, thread_routine, data);
	if (ret < 0)
		return false;

	started_ = true;
	return true;
}

int Thread::join()
{
	return pthread_join(pth_, NULL);
}

Thread::~Thread()
{
	if (!joined_)
		pthread_detach(pth_);
}

