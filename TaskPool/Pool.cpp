#include "./Pool.h"

CPool::CPool(void)
{
    m_pThreadsHandle = 0;
    m_ThreadRun = true;
}

CPool::~CPool(void)
{
    if(m_pThreadsHandle == 0) return;
    m_ThreadRun = false;
    m_pTasks.clear();

    m_CondVar.WakeAll();
    m_CondVarTaskFinished.WakeAll();
    WaitForMultipleObjects(m_NumOfTreads, m_pThreadsHandle, true, INFINITE);

    for(unsigned int i = 0; i < m_NumOfTreads; ++i)
    {
        CloseHandle(*(m_pThreadsHandle + i));
    }

    free(m_pThreadsHandle);
    m_pThreadsHandle = 0;
}

void CPool::Init(unsigned int l_NumThreads)
{
    if(l_NumThreads <= 0) return;

    m_pThreadsHandle = (HANDLE *) malloc (l_NumThreads * sizeof(HANDLE));
    m_NumOfTreads = l_NumThreads;

    for(unsigned int i = 0; i < l_NumThreads; ++i)
    {
        *(m_pThreadsHandle + i) = CreateThread(NULL, 0, &CPool::ThreadStart, (void *) this, 0, 0);
    }
}

void CPool::AddTask(CTask *l_pTask)
{
    m_Mutex.Lock();
        m_pTasks.push_back(l_pTask);
    m_Mutex.UnLock();

    m_CondVar.Wake();
}

void CPool::ThreadStop(void)
{
    m_ThreadRun = false;
}

DWORD WINAPI CPool::ThreadStart(void *l_pParam)
{
    CPool *refToThis = (CPool *) l_pParam;
    return refToThis->MainThread();
}

DWORD CPool::MainThread(void)
{
    while(m_ThreadRun)
    {
        CTask *l_pTask = 0;

        m_Mutex.Lock();
            while(m_pTasks.empty() && m_ThreadRun) m_CondVar.Sleep(m_Mutex.m_CriticalSection);
            if(!m_ThreadRun){ m_Mutex.UnLock(); return 0; }

             l_pTask = m_pTasks.front();
             m_pTasks.pop_front();
        m_Mutex.UnLock();

        CTask *l_pDependencyTask = l_pTask->GetDependency();
        if(l_pDependencyTask != 0) l_pDependencyTask->Execute();

        l_pTask->Execute();
        m_CondVarTaskFinished.Wake();
    }

    return 0;
}

void CPool::WaitForWorkers(void)
{
    m_Mutex.Lock();
        while(!m_pTasks.empty()) m_CondVarTaskFinished.Sleep(m_Mutex.m_CriticalSection);
    m_Mutex.UnLock();
}