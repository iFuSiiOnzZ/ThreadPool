# ThreadPool

A simple cross-platform, Windows and Linux, thread pool.


Task information:
```
typedef struct Task
{
    unsigned int ID;                // Not used, it could be used as a friendly ID       
    TaskPriority Priority;          // Low, Normal, High

    void *Params;                   // Task Params
    Function2Execute Function;      // Function that has to be executed (entry_point)
    struct Task *NextTask;          // For the task list, don't do nothing with it
} Task, *PTask;
```

TaskParams, a small template class that allows us to pass any kind of information we need to the executed function.
For now only allows a maxim of 4 different types of parameters, but if you need more add a new type and create a new constructor.
```
template <class T1, class T2 = T1, class T3 = T1, class T4 = T1> struct TaskParams
{
    T1 param1;
    T2 param2;
    T3 param3;
    T4 param4;

    TaskParams(T1 p1) : param1(p1) { }
    TaskParams(T1 p1, T2 p2) : param1(p1), param2(p2) { }
    TaskParams(T1 p1, T2 p2, T3 p3) : param1(p1), param2(p2), param3(p3) { }
    TaskParams(T1 p1, T2 p2, T3 p3, T4 p4) : param1(p1), param2(p2), param3(p3), param4(p4) { }
};
```

## Basic usage

With a class
```
class test_1
{
    public:
        entry_point Execute01(void *l_pTaskParams) 
        { 
            TaskParams<int> *param = (TaskParams<int> *)l_pTaskParams;
            printf("%d\n", param->param1); 
        }
};
```

With a function
```
entry_point Run(void *l_pTaskParams) 
{ 
    TaskParams<int, float, char *> *param = (TaskParams<int, float, char *> *)l_pTaskParams;
    printf("%d   %f   %s\n", param->param1, param->param2, param->param3);
}
```

Set the pool
```
CPool l_TaskPool;       // Create a pool instance
l_TaskPool.Init(4);     // Start the pool with 4 threads
```

Submit taks
```
    test_1 te1;         // Your class instance
    Task t1 = { 0 };    // Task structure instance
    Task t3 = { 0 };    // Task structure instance
    
    t1.Params = new TaskParams<int>(1); // Task Params
    t1.Function = te1.Execute01;        // CLass function to be executed
    t1.Priority = Low;                  // Task priority in queue
    
    t3.Params = new TaskParams<int, float, char *>(1, 3.14f, "Hola Mundo"); // Task Params
    t3.Function = Run;                                                      // Function to be executed
    t3.Priority = High;                                                     // Task priority in queue
    
    l_TaskPool.AddTask(&t1);    // Submit task to queue
    l_TaskPool.AddTask(&t3);    // Submit task to queue

    l_TaskPool.WaitForWorkers(); // Wait until all jobs in the queue are executed
    // if you have to run them several times tasks have to be resubmitted (no auto queueing)
```


