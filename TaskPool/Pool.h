#pragma once
#include <list>
#include <Windows.h>

#include "./Task.h"
#include "./Utilities/Mutex.h"
#include "./Utilities/ConditionVariable.h"

class CPool
{
    private:
        std::list<CTask *>  m_pTasks;
        HANDLE              *m_pThreadsHandle;

        unsigned int        m_NumOfTreads;
        bool                m_ThreadRun;

        CMutex              m_Mutex;
        CConditionVarible   m_CondVar;
        CConditionVarible   m_CondVarTaskFinished;

    public:
        CPool  (void);
        ~CPool (void);

        void                Init            (unsigned int l_NumThreads = 0);
        void                AddTask         (CTask *l_pTask);

        void                ThreadStop      (void);
        void                WaitForWorkers  (void);

        DWORD                MainThread     (void);
        static DWORD WINAPI  ThreadStart    (void *l_pParam);
};