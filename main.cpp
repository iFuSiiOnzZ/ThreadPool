#include <stdio.h>
#include <stdlib.h>

#include "./TaskPool/Task.h"
#include "./TaskPool/Pool.h"

class test_1 : public CTask
{
    public:
        void Execute(void) { printf("test_1 Hi from thread %d\n", GetCurrentThreadId()); }
};

class test_2 : public CTask
{
    public:
        void Execute(void) { printf("test_2 Hi from thread %d\n", GetCurrentThreadId()); }
};

class test_3 : public CTask
{
    public:
        void Execute(void) { printf("test_3 Hi from thread %d\n", GetCurrentThreadId()); }
};

class test_4 : public CTask
{
    public:
        void Execute(void) { printf("test_4 Hi from thread %d\n", GetCurrentThreadId()); }
};

int main(int argc, char **argv)
{
    CPool l_TaskPool;
    SYSTEM_INFO l_SystemInfo;

    GetSystemInfo(&l_SystemInfo);
    l_TaskPool.Init(l_SystemInfo.dwNumberOfProcessors);

    test_1 t1;
    test_2 t2;
    test_3 t3;
    test_4 t4;

    t1.AddDependency(&t4);

    l_TaskPool.AddTask(&t1);
    l_TaskPool.AddTask(&t2);
    l_TaskPool.AddTask(&t3);

    l_TaskPool.WaitForWorkers();
    printf("Main Thread Wait\n");

    getchar();
    return EXIT_SUCCESS;
}