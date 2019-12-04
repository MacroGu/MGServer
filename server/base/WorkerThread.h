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

// �����̣߳� ���ڴ�������߳̽��յ��Ŀͻ�������
// �������߼��ǵ��߳�
class WorkerThread
{
public:

    WorkerThread();
    ~WorkerThread();

    bool Start();

	// ���task ���̳߳�ִ�ж���
    bool AddTask(Task *task);

	void SetTaskSizeLimit(int size);

private:

	// �ͷ����������߳���Դ
	void ReleaseWorkerThread();

	// ִ���߳� 
	void ExecuteThread();

	// the thread's call back function
	void ThreadCallBackFunc(void* arg);		

private:

	// �����߳�
	std::thread mWorkerThread;

	// ��Ҫ�̳߳ش�����������
    std::queue<Task *> TasksQueue;

	// ture: �����̳߳���������  false: ���� �߳�ֹͣ����
	bool bThreadPoolRunning;

	// �߳� ��ͬʱ�������� task ������
    uint32_t TasksNumsLimitSize;
};
