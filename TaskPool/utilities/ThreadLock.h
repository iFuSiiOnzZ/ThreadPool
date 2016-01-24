#ifndef _THREADLOCK_
#define _THREADLOCK_

#include "RWLock.h"

#if _WIN32
    #include "windows\Mutex.h"
    #include "windows\ConditionVariable.h"
#elif __linux__
    #include "./linux/Mutex.h"
    #include "./linux/ConditionVariable.h"
#else
    #error Mutex and ConditionVariable not found.
#endif

class CAutoLock
{
    private:
        CMutex *m_pLock;

    public:
        CAutoLock  (CMutex &l_Lock) : m_pLock(&l_Lock) { m_pLock->Lock(); }
        ~CAutoLock (void) { m_pLock->UnLock(); }
};

class COptionalAutoLock
{
    private:
        CMutex *m_pLock;
        bool m_bAcquired;

    public:
        COptionalAutoLock  (CMutex &l_Lock, bool l_bAcquired) : m_pLock(&l_Lock), m_bAcquired(false) { if(l_bAcquired) Adquire(); }
        ~COptionalAutoLock (void) { Release(); }

        void Adquire (void)
        {
            if(!m_bAcquired)
            {
                m_pLock->Lock();
                m_bAcquired = true;
            }
        }

        void Release (void)
        {
            if(m_bAcquired)
            {
                m_bAcquired = false;
                m_pLock->UnLock();
            }
        }
};

class CRWReadAutoLock
{
    private:
        CRWLock *m_pLock;

    public:
        CRWReadAutoLock  (CRWLock &l_Lock) : m_pLock(&l_Lock) { m_pLock->ReadLock(); }
        ~CRWReadAutoLock (void) { m_pLock->ReadUnLock(); }
};

class CRWWriteAutoLock
{
    private:
        CRWLock *m_pLock;

    public:
        CRWWriteAutoLock  (CRWLock &l_Lock) : m_pLock(&l_Lock) { m_pLock->WriteLock(); }
        ~CRWWriteAutoLock (void) { m_pLock->WriteUnLock(); }
};
#endif

