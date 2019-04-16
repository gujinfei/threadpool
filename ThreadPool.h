/*
 * Copyright (c) Jinfei.Gu
 *
 * Defines ThreadPool class.
 * How to use the thread pool.
 * First, create a ThreadPool object. The default constructor will create
 * 10 threads in the pool.
 * Second, you can create an object of a class which derives from IRunTask.
 * Pass a pointer to this object in the Run() method of the thread pool
 * 
 * if this code works,it was written by Jinfei.Gu, email:gjfly007@163.com
 */

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
