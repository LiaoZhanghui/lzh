// singleton.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>
#include <process.h>

class CSingleton2
{
public:
    static CSingleton2& GetInstance() {
        static CSingleton2 sg;
        return sg;
    }
    void Print() {
        printf("%d: printf out: count(%d)\n", ::GetCurrentThreadId(), m_count);
    }

private:
    CSingleton2() {
        printf("%d: CSingleton2 begin, count(%d)\n", ::GetCurrentThreadId(), m_count);
        Sleep(1000);
        m_count = 100;
        printf("%d: CSingleton2 end, count(%d)\n", ::GetCurrentThreadId(), m_count);
    }
    ~CSingleton2() {
        printf("%d: ~CSingleton1\n", ::GetCurrentThreadId());
    }

    CSingleton2(CSingleton2&){};
    CSingleton2& operator=(const CSingleton2 & other){};

private:
    int m_count;
};



unsigned int __stdcall thread2(void *)
{
    printf("thread is is %d \n", ::GetCurrentThreadId());
    CSingleton2::GetInstance().Print();

    return 0;
}

int main()
{
    for (int i = 0; i < 3; ++i)
    {
        uintptr_t t = ::_beginthreadex(NULL, 0, thread2, NULL, 0, NULL);
        ::CloseHandle( (HANDLE)t );
    }

    getchar();
    return 0;
}


