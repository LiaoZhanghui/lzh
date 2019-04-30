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

#pragma once

#include "SVPType.h"

#if defined(SVP_WIN_32) || defined(SVP_WIN_CE)
    typedef uint32_t (__stdcall* SVPThreadProc)(void* pThreadParameter);
    #define THREAD_RETURN_PARA uint32_t
#else
    typedef void* (*SVPThreadProc)(void* pThreadParameter);

    #define THREAD_RETURN_PARA void*

    #define THREAD_PRIORITY_LOWEST          0
    #define THREAD_PRIORITY_HIGHEST         1
    #define THREAD_PRIORITY_NORMAL          2
    #define THREAD_PRIORITY_ABOVE_NORMAL    3
    #define THREAD_PRIORITY_BELOW_NORMAL    4
    #define THREAD_PRIORITY_IDLE            5
#endif

class SVPThread {
  public:
#if defined(SVP_WIN_32) || defined(SVP_WIN_CE)
    typedef HANDLE SVPThreadHandle;
#else
    typedef pthread_t* SVPThreadHandle;
#endif
    SVPThread();
    ~SVPThread();

    int32_t start(SVPThreadProc Proc, void* pThreadParameter);
    bool terminate();
    void waitQuit();
    void setPriority(uint32_t Proi);
    uint32_t getPriority();
    bool isRunning();
    uint32_t getThreadID();
    static uint32_t getCurrentThreadID();

    operator SVPThreadHandle() {
#if defined(SVP_WIN_32) || defined(SVP_WIN_CE)
        return m_hThread;
#else
        return &m_thread;
#endif
    }

    SVPThreadHandle getHandle() {
#if defined(SVP_WIN_32) || defined(SVP_WIN_CE)
        return m_hThread;
#else
        return &m_thread;
#endif
    }
  private:

#if defined(SVP_WIN_32) || defined(SVP_WIN_CE)
    HANDLE          m_hThread;
    uint32_t        m_ulThreadId;
#else
    pthread_t       m_thread;
    pthread_attr_t  m_attr;
#endif
};
