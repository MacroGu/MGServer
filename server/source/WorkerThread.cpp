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
        ReleaseThreadPool();
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

void WorkerThread::SetThreadCallBackTime(uint32_t mTime)
{
	ThreadCallBackTime = mTime;
}

void WorkerThread::ReleaseThreadPool()
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
	if (ThreadCallBackTime == 0)		// 只要有 task， 就会产生回调
	{
		while (bThreadPoolRunning != false)
		{
			ThreadsSharedMutex.lock();
			ThreadSharedCondVar.wait(ThreadsSharedMutex);

			while (!TasksQueue.empty())
			{
				Task* task = TasksQueue.front();
				task->ExecuteTask(); // execute the task
				delete task;
				TasksQueue.pop();
			}

			ThreadsSharedMutex.unlock();
		}
	}
	else			// 定时回调
	{
		while (bThreadPoolRunning != false)
		{
			auto nowTime = std::chrono::steady_clock::now();
			while (!TasksQueue.empty())
			{
				ThreadsSharedMutex.lock();
				Task* task = TasksQueue.front();
				if (!task)
				{
					LOG_WARN("task is nullptr");
					continue;
				}
				stAcceptTaskData* td = (stAcceptTaskData*)(task->getArgs());

				auto secondsDuring = std::chrono::steady_clock::now() - td->acceptTime;
				if (double(secondsDuring.count()) * std::chrono::microseconds::period::num / std::chrono::microseconds::period::den > 6000)
				{
					task->ExecuteTask(); // execute the task
					delete task;
					TasksQueue.pop();
				}
				ThreadsSharedMutex.unlock();
			}
			std::this_thread::sleep_for(std::chrono::microseconds(10));
		}
	}

}

bool WorkerThread::AddTask(Task *task)
{
	if (ThreadCallBackTime == 0)
	{
		ThreadsSharedMutex.lock();
		if (TasksNumsLimitSize > 0 && TasksQueue.size() > TasksNumsLimitSize)
		{
			LOG_WARN("task size reach limit: {}", TasksNumsLimitSize);
			ThreadsSharedMutex.unlock();
			return false;
		}

		TasksQueue.push(task);

		ThreadSharedCondVar.notify_one(); // wake up one thread that is waiting for a task to be available
		ThreadsSharedMutex.unlock();
	}
	else
	{
		ThreadsSharedMutex.lock();
		if (TasksNumsLimitSize > 0 && TasksQueue.size() > TasksNumsLimitSize)
		{
			LOG_WARN("task size reach limit: {}", TasksNumsLimitSize);
			ThreadsSharedMutex.unlock();
			return false;
		}

		TasksQueue.push(task);
		ThreadsSharedMutex.unlock();
	}

    return true;
}
