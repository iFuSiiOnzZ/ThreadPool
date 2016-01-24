#ifndef _MUTEXH_
#define _MUTEXH_

#include <pthread.h>

class CMutex
{
    public:
        CMutex  (void) { pthread_mutex_init(&m_CriticalSection, 0); }
        ~CMutex (void) { pthread_mutex_destroy(&m_CriticalSection); }

        inline void Lock   (void) { pthread_mutex_lock(&m_CriticalSection); }
        inline void UnLock (void) { pthread_mutex_unlock(&m_CriticalSection); }

        pthread_mutex_t    m_CriticalSection;
};
#endif

