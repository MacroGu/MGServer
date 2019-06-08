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
	// ִ���߳� 
	virtual void ExecuteThread() = 0;
	// the thread's call back function
	virtual void ThreadCallBackFunc(void* arg) = 0;
	virtual void SetTaskSizeLimit(int size) { TasksNumsLimitSize = size; }
	virtual void SetThreadCallBackTime(uint32_t mTime) { ThreadCallBackTime = mTime; }


protected:
	// ��Ҫ�̳߳ش�����������
	std::queue<Task*> TasksQueue;

	// ture: �����̳߳���������  false: ���� �߳�ֹͣ����
	bool bThreadRunning;

	// �߳� ��ͬʱ�������� task ������
	uint32_t TasksNumsLimitSize;

	// �̻߳ص���ʱ�� ��λ��
	// 0: ��ʾֻҪtask �����������ݣ� �ͽ��лص�����
	// >0 : ���Ƕ�ʱ�ص���Ҫ��ʱ����
	uint32_t ThreadCallBackTime;

	// �߳�
	std::thread mThread;
	// �̳߳ع���Ļ�����
	std::mutex ThreadsSharedMutex;
	// �̳߳ع������������
	std::condition_variable_any ThreadSharedCondVar;
};

// ����task ��ʱ���ִ�У� ������queue �ǿյ�ʱ����еȴ�
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


// ��ʱ��� ȫ�� task queue �е����ݣ� ���д���
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