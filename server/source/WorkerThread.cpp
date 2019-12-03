/*
	author: Macro Gu
	email: macrogu@qq.com
	QQ: 877188891
*/


#include <errno.h>
#include <string.h>
#include <sstream>
#include "WorkerThread.h"


Task::Task(std::function<void (void*)> callBackFun, void* arg) 
{
	mFTaskRunCB = callBackFun;
    m_arg = arg;
}

Task::~Task() {}

void Task::ExecuteTask() 
{
	mFTaskRunCB(m_arg);
}

WorkerThread::WorkerThread() 
{
    TasksNumsLimitSize = -1;	// means unlimit
    bThreadPoolRunning = false;
}

WorkerThread::~WorkerThread()
{
    // Release resources
    if (bThreadPoolRunning != false) 
	{
        ReleaseWorkerThread();
    }
}

void WorkerThread::ThreadCallBackFunc(void* arg)
{
	WorkerThread* tp = (WorkerThread*)arg;
	if (!tp)
	{
		LOG_ERROR("thread pool is null");
	}

	tp->ExecuteThread();
}

bool WorkerThread::Start()
{
    if (bThreadPoolRunning == true) 
	{
        LOG_WARN("ThreadPool has started, but call start thread once again");
        return true;
    }

    bThreadPoolRunning = true;
	mWorkerThread = std::thread(&WorkerThread::ThreadCallBackFunc, this, this);
    return true;
}

void WorkerThread::SetTaskSizeLimit(int size) 
{
    TasksNumsLimitSize = size;
}

void WorkerThread::ReleaseWorkerThread()
{
	bThreadPoolRunning = false;

	if (mWorkerThread.joinable())
	{
		mWorkerThread.join();
	}
	else
	{
		std::stringstream ss;
		ss << mWorkerThread.get_id();
		uint64_t id = std::stoull(ss.str());
		LOG_WARN("thread ID: {} can not join ", id);
	}
}

void WorkerThread::ExecuteThread()
{
	while(bThreadPoolRunning != false) 
	{
		// If the thread was woken up to notify process shutdown, return from here
		// ÓÎÏ·Âß¼­Ö÷Ñ­»·
		if (bThreadPoolRunning == false) 
		{
			std::stringstream ss;
			ss << std::this_thread::get_id();
			uint64_t id = std::stoull(ss.str());
			LOG_WARN("thread ID: {} can not join ", id);
			return ;
		}

		while(!TasksQueue.empty())
		{
			Task* task = TasksQueue.front();
			task->ExecuteTask(); // execute the task
			delete task;
			TasksQueue.pop();
		}
	}
}

bool WorkerThread::AddTask(Task *task)
{
    if (TasksNumsLimitSize > 0 && TasksQueue.size() > TasksNumsLimitSize) 
	{
        LOG_WARN("task size reach limit: {}" , TasksNumsLimitSize);
        return false;
    }

    TasksQueue.push(task);
    return true;
}
