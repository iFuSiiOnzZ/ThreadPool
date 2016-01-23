#pragma once
#include <pthread.h>
#include "./Mutex.h"

class CConditionVarible
{
    public:
        CConditionVarible  (void) { pthread_cond_init(&m_ConditionVariable, 0); }
        ~CConditionVarible (void) { pthread_cond_destroy(&m_ConditionVariable); }

        inline void Sleep   (CMutex &l_CriticalSenction) { pthread_cond_wait(&m_ConditionVariable, &l_CriticalSenction.m_CriticalSection); }

        inline void Wake    (void) { pthread_cond_signal(&m_ConditionVariable); }
        inline void WakeAll (void) { pthread_cond_broadcast(&m_ConditionVariable); }

        pthread_cond_t   m_ConditionVariable;
};
