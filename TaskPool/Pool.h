#pragma once
#include <list>
#include <Windows.h>

#include "./Utilities/Mutex.h"
#include "./Utilities/ConditionVariable.h"

#define MAX_NORMAL_PRIORITY_TASK 128
#define MAX_HIGH_PRIORITY_TASK 12
#define MAX_LOW_PRIORITY_TASK 32

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

typedef struct TaskQueue
{
    PTask *Tasks;

    unsigned int NumTasks;
    unsigned int MaxTasks;

    unsigned int Front;
    unsigned int Rear;

    TaskQueue(unsigned int l_NumTasks)
    {
        ASSERT(l_NumTasks > 0);
        Tasks = (PTask *) malloc (sizeof(PTask) * l_NumTasks);

        NumTasks = 0;
        MaxTasks = l_NumTasks;

        Front = 0;
        Rear = 0;
    }

    ~TaskQueue()
    {
        ASSERT(Tasks != 0);
        free(Tasks); Tasks = 0;
    }

    void push(PTask l_pTask)
    {
        ASSERT(Tasks != 0);
        if((Rear + 1) % MaxTasks != Front)
        {
            Tasks[Rear++ % MaxTasks] = l_pTask;
            NumTasks++;
        }
    }

    PTask pop()
    {
        if(NumTasks > 0)
        {
            NumTasks--;
            return Tasks[Front++ % MaxTasks];
        }

        return NULL;
    }
} TaskQueue, *PTaskQueue;

typedef PTaskQueue TaskQueueFnc;

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

        TaskQueue m_NormalPriorityTask;
        TaskQueue m_HighPriorityTask;
        TaskQueue m_LowPriorityTask;

        TaskQueueFnc m_TaskQueueFn[3];

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