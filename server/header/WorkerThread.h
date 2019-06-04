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

		void SetThreadCallBackTime(uint32_t mTime);

private:

	// �ͷ������̳߳���Դ
	void ReleaseThreadPool();

	// ִ���߳� 
	void ExecuteThread();

	// the thread's call back function
	void ThreadCallBackFunc(void* arg);		

private:

		// �����߳�
		std::thread mWorkerThread;

		// �̳߳ع���Ļ�����
		std::mutex ThreadsSharedMutex;

		// �̳߳ع������������
		std::condition_variable_any ThreadSharedCondVar;

		// ��Ҫ�̳߳ش�����������
        std::queue<Task *> TasksQueue;

		// ture: �����̳߳���������  false: ���� �߳�ֹͣ����
		bool bThreadPoolRunning;

		// �߳� ��ͬʱ�������� task ������
        uint32_t TasksNumsLimitSize;

		// �̻߳ص���ʱ�� ��λ��
		// 0: ��ʾֻҪtask �����������ݣ� �ͽ��лص�����
		// >0 : ���Ƕ�ʱ�ص���Ҫ��ʱ����
		uint32_t ThreadCallBackTime;
};
