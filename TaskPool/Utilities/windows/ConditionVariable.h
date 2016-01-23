#pragma once
#include "Mutex.h"
#include <Windows.h>

class CConditionVarible
{
    public:
        CConditionVarible  (void) { InitializeConditionVariable(&m_ConditionVariable); };
        ~CConditionVarible (void) {};

        inline void Sleep   (CMutex &l_Mutex) { SleepConditionVariableCS(&m_ConditionVariable, &l_Mutex.m_CriticalSection, INFINITE); };

        inline void Wake    (void) { WakeConditionVariable(&m_ConditionVariable); };
        inline void WakeAll (void) { WakeAllConditionVariable(&m_ConditionVariable); };

        CONDITION_VARIABLE m_ConditionVariable;
};
