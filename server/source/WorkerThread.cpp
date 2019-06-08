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


BaseThread::BaseThread()
{
	bThreadRunning = false;
	TasksNumsLimitSize = -1;	// means unlimit
	ThreadCallBackTime = 0;
	Stop();
}

BaseThread::~BaseThread()
{
	bThreadRunning = false;
	TasksNumsLimitSize = -1;	// means unlimit
	ThreadCallBackTime = 0;
}

bool BaseThread::Start()
{
	bThreadRunning = true;
	return true;
}

void BaseThread::Stop()
{
	bThreadRunning = false;
}

DealThread::DealThread() :BaseThread()
{

}

DealThread::~DealThread()
{
	Stop();
}

bool DealThread::Start()
{
	if (bThreadRunning == true)
	{
		LOG_WARN("DealThread has started, but call start thread once again");
		return true;
	}

	bThreadRunning = true;
	mThread = std::thread(&DealThread::ThreadCallBackFunc, this, this);
	return true;
}

void DealThread::Stop()
{
	BaseThread::Stop();

	if (mThread.joinable())
	{
		mThread.join();
	}
	else
	{
		std::stringstream ss;
		ss << mThread.get_id();
		uint64_t id = std::stoull(ss.str());
		LOG_WARN("DealThread thread ID: {} can not join ", id);
	}
}

bool DealThread::AddTask(Task* task)
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

	return true;
}

void DealThread::ExecuteThread()
{
	while (bThreadRunning != false)
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

void DealThread::ThreadCallBackFunc(void* arg)
{
	DealThread* bt = (DealThread*)arg;
	if (!bt)
	{
		LOG_ERROR("thread DealThread is null");
	}

	bt->ExecuteThread();
}

AcceptThread::AcceptThread():BaseThread()
{

}

AcceptThread::~AcceptThread()
{

}

bool AcceptThread::Start()
{
	if (bThreadRunning == true)
	{
		LOG_WARN("AcceptThread has started, but call start thread once again");
		return true;
	}

	bThreadRunning = true;
	mThread = std::thread(&AcceptThread::ThreadCallBackFunc, this, this);
	return true;
}

void AcceptThread::Stop()
{
	BaseThread::Stop();

	if (mThread.joinable())
	{
		mThread.join();
	}
	else
	{
		std::stringstream ss;
		ss << mThread.get_id();
		uint64_t id = std::stoull(ss.str());
		LOG_WARN("AcceptThread thread ID: {} can not join ", id);
	}
}

bool AcceptThread::AddTask(Task* task)
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

	return true;
}

void AcceptThread::ExecuteThread()
{
	while (bThreadRunning != false)
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
			if (double(secondsDuring.count()) * std::chrono::microseconds::period::num / std::chrono::microseconds::period::den > ThreadCallBackTime)
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

void AcceptThread::ThreadCallBackFunc(void* arg)
{
	AcceptThread* at = (AcceptThread*)arg;
	if (!at)
	{
		LOG_ERROR("thread AcceptThread is null");
	}

	at->ExecuteThread();
}

