#include "ThreadPool.h"
#include "RunTask.h"
#include <unistd.h>


class MyRunTask:public RunTask
{
public:
	MyRunTask()
	{

	}

	virtual void run() {
		printf("The address is %p\n", this);
		fflush(stdout);
	}

	virtual bool autoDelete() {
		return true;
	}

	~MyRunTask()
	{
		printf("%p will be deleted\n", this);
		fflush(stdout);
	}
};



int main (int argc, char *argv[])
{
	ThreadPool *pool = new ThreadPool(10, true);
	RunTask *task1 = new MyRunTask();
	RunTask *task2 = new MyRunTask();
	RunTask *task3 = new MyRunTask();
	pool->run(task1);
	pool->run(task2);
	pool->run(task3);
	delete pool;
}

