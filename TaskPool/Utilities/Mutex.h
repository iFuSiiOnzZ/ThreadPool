#pragma once
#include <Windows.h>

class CMutex
{
    public:
        CMutex  (void);
        ~CMutex (void);

        void Lock   (void);
        void UnLock (void);

        CRITICAL_SECTION    m_CriticalSection;
};
