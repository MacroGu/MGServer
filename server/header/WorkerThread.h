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
	epoll_event event;
	EpollSocket* es;
	std::chrono::steady_clock::time_point acceptTime;
};

// 工作线程， 用于处理从主线程接收到的客户端数据
// 服务器逻辑是单线程
class WorkerThread
{
public:

        WorkerThread();
        ~WorkerThread();

        bool Start();

		// 添加task 到线程池执行队列
        bool AddTask(Task *task);

		void SetTaskSizeLimit(int size);

		void SetThreadCallBackTime(uint32_t mTime);

private:

	// 释放整个线程池资源
	void ReleaseThreadPool();

	// 执行线程 
	void ExecuteThread();

	// the thread's call back function
	void ThreadCallBackFunc(void* arg);		

private:

		// 工作线程
		std::thread mWorkerThread;

		// 线程池共享的互斥锁
		std::mutex ThreadsSharedMutex;

		// 线程池共享的条件变量
		std::condition_variable_any ThreadSharedCondVar;

		// 需要线程池处理的任务队列
        std::queue<Task *> TasksQueue;

		// ture: 表面线程池正在运行  false: 表面 线程停止运行
		bool bThreadPoolRunning;

		// 线程 能同时处理的最大 task 的数量
        uint32_t TasksNumsLimitSize;

		// 线程回调的时间 单位秒
		// 0: 表示只要task 队列中有数据， 就进行回调处理
		// >0 : 就是定时回调需要的时间间隔
		uint32_t ThreadCallBackTime;
};
