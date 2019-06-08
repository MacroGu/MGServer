/*
	author: Macro Gu
	email: macrogu@qq.com
	QQ: 877188891
*/

#pragma once


#include <queue>
#include <vector>
#include <condition_variable>  
#include <mutex>
#include <thread>
#include <functional>
#include <stdint.h>
#include "EpollSocket.h"
#include "defines.h"


class EpollSocket;

class Task
{
    public: 
        Task(std::function<void (void*)> callBackFun, void* arg); // pass a free function pointer
        ~Task();
        void ExecuteTask();
		void* getArgs() { return m_arg; }
    private:
		std::function<void (void*)> mFTaskRunCB;
        void* m_arg;
};

struct stWorkerTaskData
{
	epoll_event event;
	EpollSocket* es;
};

struct stAcceptTaskData
{
	epoll_event acceptedEvent;
	std::chrono::steady_clock::time_point acceptTime;
};

class BaseThread
{
public:
	BaseThread();
	virtual ~BaseThread();

public:
	virtual bool Start();
	virtual void Stop();
	virtual bool AddTask(Task* task) = 0;
	// 执行线程 
	virtual void ExecuteThread() = 0;
	// the thread's call back function
	virtual void ThreadCallBackFunc(void* arg) = 0;
	virtual void SetTaskSizeLimit(int size) { TasksNumsLimitSize = size; }
	virtual void SetThreadCallBackTime(uint32_t mTime) { ThreadCallBackTime = mTime; }


protected:
	// 需要线程池处理的任务队列
	std::queue<Task*> TasksQueue;

	// ture: 表面线程池正在运行  false: 表面 线程停止运行
	bool bThreadRunning;

	// 线程 能同时处理的最大 task 的数量
	uint32_t TasksNumsLimitSize;

	// 线程回调的时间 单位秒
	// 0: 表示只要task 队列中有数据， 就进行回调处理
	// >0 : 就是定时回调需要的时间间隔
	uint32_t ThreadCallBackTime;

	// 线程
	std::thread mThread;
	// 线程池共享的互斥锁
	std::mutex ThreadsSharedMutex;
	// 线程池共享的条件变量
	std::condition_variable_any ThreadSharedCondVar;
};

// 当有task 的时候就执行， 不会在queue 非空的时候进行等待
class DealThread : public BaseThread
{
public:
	DealThread();
	~DealThread();

public:
	bool Start();
	void Stop();
	bool AddTask(Task* task);
	void ExecuteThread();
	void ThreadCallBackFunc(void* arg);
};


// 定时检查 全部 task queue 中的数据， 进行处理
class AcceptThread : public BaseThread
{
public:
	AcceptThread();
	~AcceptThread();

public:
	bool Start();
	void Stop();
	bool AddTask(Task* task);
	void ExecuteThread();
	void ThreadCallBackFunc(void* arg);

private:

};