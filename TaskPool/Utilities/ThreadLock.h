#pragma once

#include "./Mutex.h"

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
                m_pLock->UnLock();
                m_bAcquired = false;
            }
        }
};
