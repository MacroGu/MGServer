/*
	author: Macro Gu
	email: macrogu@qq.com
	QQ: 877188891
*/


#include <errno.h>
#include <string.h>
#include "ThreadPool.h"


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

ThreadPool::ThreadPool() 
{
    TasksNumsLimitSize = -1;	// means unlimit
    WorkThreadsNums = 0;
    bThreadPoolRunning = false;
}

ThreadPool::~ThreadPool()
{
    // Release resources
    if (bThreadPoolRunning != false) 
	{
        ReleaseThreadPool();
    }
}

void ThreadPool::ThreadCallBackFunc(void* arg)
{
    ThreadPool* tp = (ThreadPool *)arg;
    if (tp->CallBackFunAtStart) 
	{
        tp->CallBackFunAtStart();
    } 
	else 
	{
        LOG_ERROR("thread start cb is null");
    }

    tp->ExecuteThread();
}

bool ThreadPool::Start()
{
    if (WorkThreadsNums == 0) {
		LOG_ERROR("pool size must be set!");
        return false;
    }
    if (bThreadPoolRunning == true) 
	{
        LOG_WARN("ThreadPool has started, but call start thread once again");
        return true;
    }
    bThreadPoolRunning = true;
	for (int i = 0 ; i < WorkThreadsNums; ++i)
	{
		AllWorkThreads.push_back(std::thread(&ThreadPool::ThreadCallBackFunc, this, this));
	}

    return true;
}

void ThreadPool::SetThreadCallBackAtStart(std::function<void(void)> startedCB) 
{
	CallBackFunAtStart = startedCB;
}

void ThreadPool::SetTaskSizeLimit(int size) 
{
    TasksNumsLimitSize = size;
}

bool ThreadPool::SetWorkThreadNums(int ThreadNums)
{
	if (ThreadNums <= 0 || ThreadNums >= MAX_THREAD_NUMS_THREAD_POOL)
	{
		return false;
	}

    WorkThreadsNums = ThreadNums;
	return true;
}

void ThreadPool::ReleaseThreadPool()
{
	ThreadsSharedMutex.lock();
	bThreadPoolRunning = false;
	ThreadsSharedMutex.unlock();

	LOG_DEBUG("Broadcasting STOP signal to all threads...");
    ThreadSharedCondVar.notify_all(); // notify all threads we are shttung down

	for (auto &m_thread : AllWorkThreads)
	{
		if (m_thread.joinable())
		{
			m_thread.join();	
			ThreadSharedCondVar.notify_all(); // try waking up a bunch of threads that are still waiting
		}
		else
		{
			std::thread::id tid = m_thread.get_id();
			_Thrd_t t = *(_Thrd_t*)(char*)&tid;
			 LOG_WARN("thread ID: {} can not join ", t._Id);
		}
	}

	LOG_DEBUG(" {}threads exited from the thread pool, task size: {} " , WorkThreadsNums, TasksQueue.size() );
}

void ThreadPool::ExecuteThread()
{
	while(bThreadPoolRunning != false) 
	{
		ThreadsSharedMutex.lock();
		ThreadSharedCondVar.wait(ThreadsSharedMutex);

		// If the thread was woken up to notify process shutdown, return from here
		if (bThreadPoolRunning == false) 
		{
			std::thread::id tid = std::this_thread::get_id();
			_Thrd_t t = *(_Thrd_t*)(char*)&tid;
			LOG_WARN("thread ID: {} can not join ", t._Id);
			return ;
		}

		while(!TasksQueue.empty())
		{
			Task* task = TasksQueue.front();
			task->ExecuteTask(); // execute the task
			delete task;
			TasksQueue.pop();
		}

		ThreadsSharedMutex.unlock();
	}

}

bool ThreadPool::AddTask(Task *task)
{
	ThreadsSharedMutex.lock();

    if (TasksNumsLimitSize > 0 && (int) TasksQueue.size() > TasksNumsLimitSize) 
	{
        LOG_WARN("task size reach limit: {}" , TasksNumsLimitSize);
        ThreadsSharedMutex.unlock();
        return false;
    }

    TasksQueue.push(task);

    ThreadSharedCondVar.notify_one(); // wake up one thread that is waiting for a task to be available

	ThreadsSharedMutex.unlock();

    return true;
}
