#include <stdlib.h>
#include "./Pool.h"

CPool::CPool(void)
{
    m_pThreadsHandle = 0;
    m_ThreadRun = true;

    m_TaskInQueue = 0;
    m_TaskToFinish = 0;
}

CPool::~CPool(void)
{
    m_ThreadRun = false;

    m_CondVar.WakeAll();
    m_CondVarTaskFinished.WakeAll();
    //WaitForMultipleObjects(m_NumOfTreads, m_pThreadsHandle, true, INFINITE);

    for(unsigned int i = 0; i < m_NumOfTreads; ++i)
    {
        PoolThreadStop(*(m_pThreadsHandle + i));
    }

    free(m_pThreadsHandle);
    m_pThreadsHandle = 0;
}

void CPool::Init(unsigned int l_NumThreads)
{
    ASSERT(l_NumThreads > 0);
    m_NumOfTreads = l_NumThreads;

    m_pThreadsHandle = (THREAD_HANDLE *) malloc (l_NumThreads * sizeof(THREAD_HANDLE));
    ASSERT(m_pThreadsHandle != NULL);

    for(unsigned int i = 0; i < l_NumThreads; ++i)
    {
        *(m_pThreadsHandle + i) = PoolThreadStart(&CPool::ThreadStart, (void *) this);
        ASSERT(*(m_pThreadsHandle + i) != NULL);
    }
}

void CPool::AddTask(Task *l_pTask)
{
    m_Mutex.Lock();
        m_Task.push(l_pTask);
    m_Mutex.UnLock();

    AtomicAdd(&m_TaskToFinish, m_TaskToFinish + 1, m_TaskToFinish);
    AtomicAdd(&m_TaskInQueue, m_TaskInQueue + 1, m_TaskInQueue);

    m_CondVar.Wake();
}

void CPool::ThreadStop(void)
{
    m_ThreadRun = false;
}

pool_thread_start CPool::ThreadStart(void *l_pParam)
{
    CPool *refToThis = (CPool *) l_pParam;
    refToThis->MainThread();
    
    return 0;
}

void CPool::MainThread(void)
{
    PTask l_pTask = NULL;

    while(m_ThreadRun)
    {
        m_Mutex.Lock();
            while(m_TaskInQueue == 0 && m_ThreadRun) m_CondVar.Sleep(m_Mutex);
            if(!m_ThreadRun){ m_Mutex.UnLock(); return; }

            l_pTask = m_Task.pop();
        m_Mutex.UnLock();

        AtomicAdd(&m_TaskInQueue, m_TaskInQueue - 1, m_TaskInQueue);
        l_pTask->Function(l_pTask->Params);
        AtomicAdd(&m_TaskToFinish, m_TaskToFinish - 1, m_TaskToFinish);

        m_CondVarTaskFinished.Wake();
    }
}

void CPool::WaitForWorkers(void)
{
    m_Mutex.Lock();
        while(m_TaskToFinish > 0) m_CondVarTaskFinished.Sleep(m_Mutex);
    m_Mutex.UnLock();
}
