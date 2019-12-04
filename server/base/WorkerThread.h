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
#include "defines.h"

class Task
{
    public: 
        Task(std::function<void (void*)> callBackFun, void* arg); // pass a free function pointer
        ~Task();
        void ExecuteTask();
    private:
		std::function<void (void*)> mFTaskRunCB;
        void* m_arg;
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

private:

	// 释放整个工作线程资源
	void ReleaseWorkerThread();

	// 执行线程 
	void ExecuteThread();

	// the thread's call back function
	void ThreadCallBackFunc(void* arg);		

private:

	// 工作线程
	std::thread mWorkerThread;

	// 需要线程池处理的任务队列
    std::queue<Task *> TasksQueue;

	// ture: 表面线程池正在运行  false: 表面 线程停止运行
	bool bThreadPoolRunning;

	// 线程 能同时处理的最大 task 的数量
    uint32_t TasksNumsLimitSize;
};
