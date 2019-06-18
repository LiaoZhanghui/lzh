// singleton.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>
#include <process.h>

class CSingleton1 {
public:
    static CSingleton1 *GetInstance() {
        if (m_instance == NULL) {
            m_instance = new CSingleton1();
        }
        return m_instance;
    }
    static void RealseInstance() {
        if (m_instance != NULL) {
            delete m_instance;
            m_instance = NULL;
        }
    }
    void Print() {
        printf("%d: printf out\n", ::GetCurrentThreadId());
    }

private:
    CSingleton1() {
        printf("%d: CSingleton1 begin\n", ::GetCurrentThreadId());
        Sleep(1000);
        printf("%d: CSingleton1 end\n", ::GetCurrentThreadId());
    }
    ~CSingleton1() {
        printf("%d: ~CSingleton1\n", ::GetCurrentThreadId());
    }
    CSingleton1(CSingleton1&){};
    CSingleton1& operator=(const CSingleton1 & other){};

private:
    static CSingleton1 * m_instance;
};

CSingleton1 * CSingleton1::m_instance = NULL;


unsigned int __stdcall thread1(void *)
{
    printf("thread is is %d \n", ::GetCurrentThreadId());
    CSingleton1::GetInstance()->Print();

    CSingleton1::RealseInstance();
    return 0;
}
/*
int main()
{
    for (int i = 0; i < 3; ++i){
        uintptr_t t = ::_beginthreadex(NULL, 0, thread1, NULL, 0, NULL);
        ::CloseHandle( (HANDLE)t );
    }

    getchar();
    return 0;
}*/
