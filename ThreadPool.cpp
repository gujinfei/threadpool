#include "ThreadPool.h"
#include <iostream>
#include <unistd.h>

void* ThreadPool::threadFunc(void *arg)
{
	ThreadPool* threadPool = (ThreadPool*)arg;
	RunTask* task;
	while (1) {
		if (threadPool->poolExitFlag) {
			if (!threadPool->taskAllConsume) 
				break;
			else if (0 == threadPool->getTaskNum()) 
				break;
		}

		if (!threadPool->getRunTask(&task)) {
			usleep(1000);
			continue;
		}
		task->run();
		if (task->autoDelete()) {
			delete task;
		}
		usleep(1000);
	}
	
	return 0;
}

ThreadPool::ThreadPool(int poolSize, bool allConsume)
{
	if (poolSize <= 0) {
		cout << "pool size must bigger than 0" << endl;
	}
	maxThreadNum = poolSize;
	taskAllConsume = allConsume;

	poolExitFlag = false;

	pthread_mutex_init(&taskMutex, NULL);

	if (!create()) {
		throw 1;
	}
}

ThreadPool::~ThreadPool()
{
	destroy();
}

bool ThreadPool::create()
{
	for (int i = 0; i < maxThreadNum; i++) {
		pthread_t tidp;
		if (0 != pthread_create(&tidp, NULL, threadFunc, this)) {
			return false;
		}
		threads.push_back(tidp);
	}
	return true;
}

void ThreadPool::destroy()
{
	poolExitFlag = true;
	list<pthread_t>::iterator iter;
	for (iter= threads.begin(); iter != threads.end(); iter++) {
		pthread_join(*iter,NULL);
	}
}

bool ThreadPool::run(RunTask* runTask, ThreadPriority priority)
{
	bool ret = false;
	if (!poolExitFlag) {
		pthread_mutex_lock(&taskMutex);
		if (High == priority) {
			runTasks.push_front(runTask);
		} else {
			runTasks.push_back(runTask);
		}
		pthread_mutex_unlock(&taskMutex);
		ret = true;
	}
	return ret;
}

bool ThreadPool::getRunTask(RunTask** task)
{
	pthread_mutex_lock(&taskMutex);
	list<RunTask *>::iterator iter = runTasks.begin();
	if (iter == runTasks.end()) {
		pthread_mutex_unlock(&taskMutex);
		return false;
	}
	*task = (*iter);
	runTasks.pop_front();
	pthread_mutex_unlock(&taskMutex);
	return true;
}

int ThreadPool::getTaskNum()
{
	int size = 0;
	pthread_mutex_lock(&taskMutex);
	size = runTasks.size();
	pthread_mutex_unlock(&taskMutex);
	return size;
}

