#ifndef _PLATFORMH_
#define _PLATFORMH_

#if _WIN32
    #include <Windows.h>
    #include "utilities\windows\Mutex.h"
    #include "utilities\windows\ConditionVariable.h"

    typedef HANDLE THREAD_HANDLE;
    #define pool_thread_start DWORD WINAPI

    inline int AtomicAdd(volatile unsigned int *Point2Var, int NewValue, int OldValue)
    {
        return InterlockedCompareExchange(Point2Var, NewValue, OldValue);
    }

    inline THREAD_HANDLE PoolThreadStart(LPTHREAD_START_ROUTINE l_Function, void *l_Class)
    {
        THREAD_HANDLE l_ThreadHandle = 0;
        l_ThreadHandle = CreateThread(NULL, 0, l_Function, l_Class, 0, 0);

        return l_ThreadHandle;
    }

    inline void PoolThreadStop(THREAD_HANDLE l_Handle)
    {
        CloseHandle(l_Handle);
    }

    #if _DEBUG
        #define ASSERT(x) if(!(x)){ __asm{ int 3 } }
    #else
        #define ASSERT(x) {}
    #endif

#elif __linux__
    #include <pthread.h>
    #include "./utilities/linux/Mutex.h"
    #include "./utilities/linux/ConditionVariable.h"

    typedef pthread_t THREAD_HANDLE;
    #define pool_thread_start void *

    inline int AtomicAdd(volatile unsigned int *Point2Var, int NewValue, int OldValue)
    {
        return __sync_val_compare_and_swap(Point2Var, OldValue, NewValue);
    }

    inline THREAD_HANDLE PoolThreadStart(void * (* l_Function)(void *), void *l_Class)
    {
        THREAD_HANDLE l_ThreadHandle = 0;
        pthread_create(&l_ThreadHandle, 0, l_Function, l_Class);

        return l_ThreadHandle;
    }

    inline void PoolThreadStop(THREAD_HANDLE l_Handle)
    {
        pthread_join(l_Handle, 0);
    }

    #if _DEBUG
        #define ASSERT(x) if(!(x)){ __asm__("int3"); }
    #else
        #define ASSERT(x) {}
    #endif
#else
    #error Platform not found.
#endif

#endif
