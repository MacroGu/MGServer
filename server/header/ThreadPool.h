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


class ThreadPool
{
public:

        ThreadPool();
        ~ThreadPool();

        bool Start();

		// 添加task 到线程池执行队列
        bool AddTask(Task *task);

		void SetTaskSizeLimit(int size);

        bool SetWorkThreadNums(int threadNums);

private:

	// 释放整个线程池资源
	void ReleaseThreadPool();

	// 执行线程 
	void ExecuteThread();

	// the thread's call back function
	void ThreadCallBackFunc(void* arg);		

private:

		// 全部工作线程的数量 
		uint8_t WorkThreadsNums;
		
		// 线程池共享的互斥锁
		std::mutex ThreadsSharedMutex;

		// 线程池共享的条件变量
		std::condition_variable_any ThreadSharedCondVar;

		// 线程池全部的工作线程
		std::vector<std::thread> AllWorkThreads;

		// 需要线程池处理的任务队列
        std::queue<Task *> TasksQueue;

		// ture: 表面线程池正在运行  false: 表面 线程池停止运行
		bool bThreadPoolRunning;

		// 线程池 能同时处理的最大 task 的数量
        uint32_t TasksNumsLimitSize;
};
