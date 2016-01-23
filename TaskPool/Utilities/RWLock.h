#pragma once

#if _WIN32
    #include "windows\Mutex.h"
    #include "windows\ConditionVariable.h"
#elif __linux__
    #include "linux\Mutex.h"
    #include "linux\ConditionVariable.h"
#else
    #error Mutex and ConditionVariable not found.
#endif

class CRWLock
{
    private:
        CMutex m_Mutex;
        CConditionVarible m_Writers;
        CConditionVarible m_Readers;

        int m_ActiveReaders;
        int m_ActiveWriters;
        int m_WritersWaiting;

    public:
        CRWLock  (void) : m_ActiveReaders(0), m_ActiveWriters(0), m_WritersWaiting(0) {}
        ~CRWLock (void) {}

        void ReadLock(void)
        {
            m_Mutex.Lock();
            while(m_ActiveWriters > 0)
                m_Readers.Sleep(m_Mutex);

            m_ActiveReaders++;
            m_Mutex.UnLock();
        }

        void ReadUnLock(void)
        {
            m_Mutex.Lock();
            m_ActiveReaders--;

            if(m_WritersWaiting > 0 && m_ActiveReaders == 0) m_Writers.Wake();
            m_Mutex.UnLock();
        }

        void WriteLock(void)
        {
            m_Mutex.Lock();
            m_WritersWaiting++;

            while (m_ActiveReaders > 0 || m_ActiveWriters > 0)
                m_Writers.Sleep(m_Mutex);

            m_ActiveWriters++;
            m_WritersWaiting--;
            m_Mutex.UnLock();
        }

        void WriteUnLock(void)
        {
            m_Mutex.Lock();
            m_ActiveWriters--;

            if(m_WritersWaiting > 0) m_Writers.Wake();
            else m_Readers.WakeAll();
            m_Mutex.UnLock();
        }
};
