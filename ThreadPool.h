#ifndef _THREAD_POOL_H_
#define _THREAD_POOL_H_
#include <list>
#include <pthread.h>
#include "RunTask.h"


#define POLL_SIZE  10

using namespace std;

enum ThreadPriority
{
	High,
	Low
};

class ThreadPool
{
public:
	ThreadPool(int poolSize = POLL_SIZE, bool allConsume = false);
	~ThreadPool();
	static void * threadFunc(void *arg);
	bool run(RunTask* runTask, ThreadPriority priority = Low);
	int getTaskNum();

private:
	bool create();
	void destroy();
	bool getRunTask(RunTask** task);

private:
	int maxThreadNum;
	list<RunTask*> runTasks;
	list<pthread_t> threads;
	pthread_mutex_t taskMutex; 
	bool poolExitFlag;
	bool taskAllConsume;
};

#endif
