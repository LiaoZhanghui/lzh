/******************************************************************************
*
* Company       : Huizhou Desay SV Automotive Co., Ltd.
*
* Division      : Automotive Electronics, Desay Corporation
*
* Business Unit : Central Technology
*
* Department    : Advanced Development (Huizhou)
*
******************************************************************************/

#include "SVPThread.h"

#if !defined(SVP_WIN_32) && !defined(SVP_WIN_CE)
    #include <pthread.h>
    #include <sched.h>
    #include <string.h>
    #include <errno.h>
#endif

#include "SVPLog.h"

#ifdef SVP_LOG_TAG
    #undef SVP_LOG_TAG
#endif
#define SVP_LOG_TAG     "libbasic|thread"


SVPThread::SVPThread()
#if defined(SVP_WIN_32) || defined(SVP_WIN_CE)
    : m_hThread(nullptr), m_ulThreadId(-1)
#else
    : m_thread(0)
#endif
{
#if !defined(SVP_WIN_32) && !defined(SVP_WIN_CE)
    pthread_attr_init(&m_attr);
    pthread_attr_setschedpolicy(&m_attr, SCHED_RR);
#endif
}

SVPThread::~SVPThread() {
#if defined(SVP_WIN_32) || defined(SVP_WIN_CE)

    if (m_hThread != nullptr) {
        CloseHandle(m_hThread);
    }

#else
    pthread_attr_destroy(&m_attr);
#endif
}

int32_t SVPThread::start(SVPThreadProc Proc, void* pThreadParameter ) {
    int32_t err = 0;
#if defined(SVP_WIN_32) || defined(SVP_WIN_CE)

    if ( m_hThread != nullptr) {
        CloseHandle(m_hThread);
    }

    m_hThread = CreateThread(nullptr, 0 , Proc, pThreadParameter, 0, &m_ulThreadId);
#else
    err = pthread_create(&m_thread, &m_attr, Proc, pThreadParameter);

    if (0 != err) {
        m_thread = 0;
        SVP_ERROR("Can't create thread: %s!", strerror(err));
    }

#endif
    return err;
}

bool SVPThread::terminate() {
#if defined(SVP_WIN_32) || defined(SVP_WIN_CE)

    if (m_hThread != nullptr) {
        return TerminateThread(m_hThread, 0);
    } else {
        return false;
    }

#else

    if (m_thread) {
        return !pthread_cancel(m_thread);
    } else {
        return false;
    }

#endif
}

void SVPThread::waitQuit() {
#if defined(SVP_WIN_32) || defined(SVP_WIN_CE)

    if (m_hThread != nullptr) {
        WaitForSingleObject(m_hThread, INFINITE);
        CloseHandle(m_hThread);
        m_hThread = nullptr;
    }

#else

    if (m_thread != 0) {
        void* lpv = nullptr;
        pthread_join(m_thread, &lpv);
        m_thread = 0;
    }

#endif
}

void SVPThread::setPriority(uint32_t Proi) {
#if defined(SVP_WIN_32) || defined(SVP_WIN_CE)

    if (m_hThread != nullptr) {
        SetThreadPriority(m_hThread, Proi);
    }

#else
    struct sched_param sched;

    if (Proi == THREAD_PRIORITY_LOWEST) {
        sched.sched_priority = sched_get_priority_min(SCHED_RR);
    } else if (Proi == THREAD_PRIORITY_HIGHEST) {
        sched.sched_priority = sched_get_priority_max(SCHED_RR);
    } else if (Proi == THREAD_PRIORITY_NORMAL) {
        sched.sched_priority = (sched_get_priority_min(SCHED_RR) + sched_get_priority_max(SCHED_RR)) * 0.5;
    } else if (Proi == THREAD_PRIORITY_ABOVE_NORMAL) {
        sched.sched_priority = (sched_get_priority_min(SCHED_RR) + sched_get_priority_max(SCHED_RR)) * 0.7;
    } else if (Proi == THREAD_PRIORITY_BELOW_NORMAL) {
        sched.sched_priority = (sched_get_priority_min(SCHED_RR) + sched_get_priority_max(SCHED_RR)) * 0.5;
    } else if (Proi == THREAD_PRIORITY_IDLE) {
        sched.sched_priority = (sched_get_priority_min(SCHED_RR) + sched_get_priority_max(SCHED_RR)) * 0.3;
    } else {
        sched.sched_priority = (sched_get_priority_min(SCHED_RR) + sched_get_priority_max(SCHED_RR)) * 0.5;
    }

    if (0 != pthread_attr_setschedparam (&m_attr, &sched)) {
        SVP_ERROR("SetPriority(%lu) -- %d fail!", Proi, sched.sched_priority);
    }

#endif
}

uint32_t SVPThread::getPriority() {
#if defined(SVP_WIN_32) || defined(SVP_WIN_CE)

    if (m_hThread != nullptr) {
        return GetThreadPriority(m_hThread);
    } else {
        return 0;
    }

#else
    struct sched_param sched;
    pthread_attr_getschedparam(&m_attr, &sched);
    return sched.sched_priority;
#endif
}

bool SVPThread::isRunning() {
#if defined(SVP_WIN_32) || defined(SVP_WIN_CE)
    return (m_hThread != nullptr) ? true : false;
#else
    return m_thread ? true : false;
#endif
}

uint32_t SVPThread::getThreadID() {
#if defined(SVP_WIN_32) || defined(SVP_WIN_CE)
    return m_ulThreadId;
#else
    return m_thread;
#endif
}

uint32_t SVPThread::getCurrentThreadID() {
#if defined(SVP_WIN_32) || defined(SVP_WIN_CE)
    return GetCurrentThreadId();
#else
    return pthread_self();
#endif
}
