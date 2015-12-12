#pragma once
#include <list>
#include <Windows.h>

#include "./Utilities/Mutex.h"
#include "./Utilities/ConditionVariable.h"

#define entry_point static void

typedef void (*Function2Execute) (void *l_pTaskParams);
typedef enum TaskPriority { Low, Normal, High } TaskPriority, *PTaskPriority;

typedef struct Task
{
    unsigned int ID;
    TaskPriority Priority;

    void *Params;
    Function2Execute Function;
} Task, *PTask;

template <class T1 = int, class T2 = T1, class T3 = T1, class T4 = T1> struct TaskParams
{
    T1 param1;
    T2 param2;
    T3 param3;
    T4 param4;

    TaskParams(T1 p1) : param1(p1) { }
    TaskParams(T1 p1, T2 p2) : param1(p1), param2(p2) { }
    TaskParams(T1 p1, T2 p2, T3 p3) : param1(p1), param2(p2), param3(p3) { }
    TaskParams(T1 p1, T2 p2, T3 p3, T4) : param1(p1), param2(p2), param3(p3), param4(p4) { }
};

class CPool
{
    private:
        std::list<Task *>  m_pTasks;
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
        void                AddTask         (Task *l_pTask);

        void                ThreadStop      (void);
        void                WaitForWorkers  (void);

        DWORD                MainThread     (void);
        static DWORD WINAPI  ThreadStart    (void *l_pParam);
};