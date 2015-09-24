#pragma once
#include <Windows.h>

class CConditionVarible
{
    public:
        CConditionVarible  (void);
        ~CConditionVarible (void);

        void Sleep   (CRITICAL_SECTION &l_CriticalSenction);

        void Wake    (void);
        void WakeAll (void);

        CONDITION_VARIABLE m_ConditionVariable;
};
