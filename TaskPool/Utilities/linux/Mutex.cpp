#include "./Mutex.h"

CMutex::CMutex(void)
{
    pthread_mutex_init(&m_CriticalSection, 0);
}

CMutex::~CMutex(void)
{
    pthread_mutex_destroy(&m_CriticalSection);
}

void CMutex::Lock(void)
{
    pthread_mutex_lock(&m_CriticalSection);
}

void CMutex::UnLock(void)
{
    pthread_mutex_unlock(&m_CriticalSection);
}