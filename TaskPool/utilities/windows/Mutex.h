#pragma once
#include <Windows.h>

class CMutex
{
    public:
        CMutex  (void) { InitializeCriticalSection(&m_CriticalSection); };
        ~CMutex (void) { DeleteCriticalSection(&m_CriticalSection); };

        inline void Lock   (void) { EnterCriticalSection(&m_CriticalSection); };
        inline void UnLock (void) { LeaveCriticalSection(&m_CriticalSection); };

        CRITICAL_SECTION    m_CriticalSection;
};
