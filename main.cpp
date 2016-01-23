#include <stdio.h>
#include <stdlib.h>

#include "./TaskPool/Pool.h"

class test_1
{
    public:
        entry_point Execute01(void *l_pTaskParams) 
        { 
            TaskParams<int> *param = (TaskParams<int> *)l_pTaskParams;
            printf("%d\n", param->param1); 
        }
};

class test_2
{
    public:
        entry_point Execute02(void *l_pTaskParams)
        {
            TaskParams<int, float> *param = (TaskParams<int, float> *)l_pTaskParams;
            printf("%d   %f\n", param->param1, param->param2);  
        }
};

entry_point Run(void *l_pTaskParams) 
{ 
    TaskParams<int, float, char *> *param = (TaskParams<int, float, char *> *)l_pTaskParams;
    printf("%d   %f   %s\n", param->param1, param->param2, param->param3);
}

int main(int argc, char **argv)
{
    /*CPool l_TaskPool;
    SYSTEM_INFO l_SystemInfo;

    GetSystemInfo(&l_SystemInfo);
    l_TaskPool.Init(l_SystemInfo.dwNumberOfProcessors);

    test_1 te1;
    test_2 te2;

    Task t1 = { 0 };
    Task t2 = { 0 };
    Task t3 = { 0 };

    t1.Params = new TaskParams<int>(1);
    t1.Function = te1.Execute01;
    t1.Priority = Low;

    t2.Params = new TaskParams<int, float>(1, 3.14f);
    t2.Function = te2.Execute02;
    t2.Priority = Normal;

    t3.Params = new TaskParams<int, float, char *>(1, 3.14f, "Hola Mundo");
    t3.Function = Run;
    t3.Priority = High;

    l_TaskPool.AddTask(&t1);
    l_TaskPool.AddTask(&t2);
    l_TaskPool.AddTask(&t3);

    l_TaskPool.WaitForWorkers();*/
    printf("Main Thread Wait\n");
    

    getchar();
    return EXIT_SUCCESS;
}
