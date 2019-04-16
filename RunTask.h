#ifndef _RUNTASK_H_
#define _RUNTASK_H_

class RunTask
{
public:
	RunTask(){};
	virtual void run() = 0;
	virtual bool autoDelete() = 0;
	virtual ~RunTask(){}
};

#endif
