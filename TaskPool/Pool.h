#pragma once

#if _WIN32
    #include <Windows.h>
    #include "utilities\windows\Mutex.h"
    #include "utilities\windows\ConditionVariable.h"
#elif __linux__
    #include "utilities\linux\Mutex.h"
    #include "utilities\linux\ConditionVariable.h"
#else
    #error Mutex and ConditionVariable not found.
#endif

#if _WIN32

inline int AtomicAdd(volatile unsigned int *Point2Var, int NewValue, int OldValue)
{
    return InterlockedCompareExchange(Point2Var, NewValue, OldValue);
}

#elif __linux__

inline int AtomicAdd(volatile unsigned int *Point2Var, int NewValue, int OldValue)
{
    return __sync_val_compare_and_swap(Point2Var, OldValue, NewValue);
}

#else
#error No free lock functions available.
#endif

#define entry_point static void

#if _DEBUG
#define ASSERT(x) if(!(x)){ __asm{ int 3 } }
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
    struct Task *NextTask;
} Task, *PTask;


typedef struct TaskQueue
{
    PTask Front;

    TaskQueue() : Front(NULL)
    {
    }

    ~TaskQueue()
    {
      
    }

    void push(PTask l_pTask)
    {
        if(Front == NULL || l_pTask->Priority > Front->Priority)
        {
            l_pTask->NextTask = Front;
            Front = l_pTask;
        }
        else
        {
            PTask l_SearchTask = Front;
            while(l_SearchTask->NextTask != NULL && l_pTask->Priority <= l_SearchTask->Priority)
            {
                l_SearchTask = l_SearchTask->NextTask;
            }

            l_pTask->NextTask = l_SearchTask->NextTask;;
            l_SearchTask->NextTask = l_pTask;
        }
    }

    PTask pop()
    {
        if(Front == NULL)
        {
            return NULL;
        }

        PTask l_Task = Front;
        Front = Front->NextTask;

        l_Task->NextTask = NULL;
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
        unsigned int         m_TaskToFinish;

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

        unsigned int        MainThread     (void);
        static DWORD WINAPI ThreadStart    (void *l_pParam);
};