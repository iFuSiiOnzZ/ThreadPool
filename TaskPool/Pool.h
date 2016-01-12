#pragma once
#include <list>
#include <Windows.h>

#include "./Utilities/Mutex.h"
#include "./Utilities/ConditionVariable.h"

#define entry_point static void

#if _DEBUG
#define ASSERT(x) if(!(x)){ int a = *((int *) 0); }
#else
#define ASSERT(x) {}
#endif

typedef void (*Function2Execute) (void *l_pTaskParams);
typedef enum TaskPriority { Low, Normal, High } TaskPriority, *PTaskPriority;

typedef struct Task
{
    unsigned int ID;
    TaskPriority Priority;

    void *Params;
    Function2Execute Function;
} Task, *PTask;

typedef struct NodeTask
{
    PTask TaskData;
    struct NodeTask *NextTask;

} NodeTask, *PNodeTask;

typedef struct TaskQueue
{
    PNodeTask Front;

    TaskQueue() : Front(NULL)
    {
    }

    ~TaskQueue()
    {
      
    }

    void push(PTask l_pTask)
    {
        PNodeTask l_NewTask = (PNodeTask) malloc (sizeof(NodeTask));
        l_NewTask->TaskData = l_pTask;
        l_NewTask->NextTask = NULL;

        if(Front == NULL || l_pTask->Priority > Front->TaskData->Priority)
        {
            l_NewTask->NextTask = Front;
            Front = l_NewTask;
        }
        else
        {
            PNodeTask l_SearchTask = Front;
            while(l_SearchTask->NextTask != NULL && l_pTask->Priority <= l_SearchTask->TaskData->Priority)
            {
                l_SearchTask = l_SearchTask->NextTask;
            }

            l_NewTask->NextTask = l_SearchTask->NextTask;;
            l_SearchTask->NextTask = l_NewTask;
        }
    }

    PTask pop()
    {
        if(Front == NULL)
        {
            return NULL;
        }

        PTask l_Task = Front->TaskData;
        PNodeTask l_Node = Front;
        Front = Front->NextTask;

        free(l_Node);
        return l_Task;
    }
} TaskQueue, *PTaskQueue;

template <class T1, class T2 = T1, class T3 = T1, class T4 = T1> struct TaskParams
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

        TaskQueue            m_Task;
        HANDLE              *m_pThreadsHandle;
        bool                 m_ThreadRun;

        unsigned int         m_NumOfTreads;
        unsigned int         m_TaskInQueue;

        CMutex               m_Mutex;
        CConditionVarible    m_CondVar;
        CConditionVarible    m_CondVarTaskFinished;

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