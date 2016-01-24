# ThreadPool

A simple cross-platform, windows and linux, thread pool.


Task information:
```
typedef struct Task
{
    unsigned int ID;                // Not used       
    TaskPriority Priority;          // Low, Normal, Hight

    void *Params;                   // Task Params
    Function2Execute Function;      // Function that has to be executed (entry_point)
    struct Task *NextTask;          // For the tasks list, don't do nothing with it
} Task, *PTask;
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

Submit jobs
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

    l_TaskPool.WaitForWorkers(); // Wait until no job remain in the queue
```


