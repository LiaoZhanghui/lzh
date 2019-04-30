// singleton.cpp : Defines the entry point for the console application.
//


#pragma G++ diagnostic error "-std=c++11" 

#include <unistd.h>
#include <stdio.h>
#include <memory>
#include <mutex>
#include <thread>
using namespace std;


template <class T>
class Singleton {
public:
    static std::shared_ptr<T> GetInstance() {
        m_mutex.lock();
        if (!m_singletonPtr)
        {
            std::shared_ptr<T> temp(new T());
            m_singletonPtr = temp;
        }
        m_mutex.unlock();
        printf("data init ok \n");
        return m_singletonPtr;
    }
protected:
    Singleton() {
        printf("Singleton begin\n");
        sleep(1);
        printf("Singleton end\n");
    }
public:
    virtual ~Singleton() {
        printf("~Singleton\n");
    }
private:
    Singleton(const Singleton &) {};
    Singleton & operator=(const Singleton &) {};
private:
    static std::mutex           m_mutex;
    static std::shared_ptr<T>   m_singletonPtr;
};

template <class T>
std::mutex Singleton<T>::m_mutex;
template <class T>
std::shared_ptr<T> Singleton<T>::m_singletonPtr = nullptr;

class Manger: public Singleton<Manger>
{
protected:
    Manger() {
        printf("Manger begin, m_count is %d\n", m_count);
        sleep(1);
        m_count = 123;
        printf("Manger end, m_count is %d\n", m_count);
    }
public:
    ~Manger() {
        printf("~Manger\n");
    }
public:
    void Print() {
        printf("print out, m_count is %d \n", ++m_count);
    }
private:
    int m_count;
};


void ThreadFunc(int n)
{
    printf("thread is is %ld \n", pthread_self());
    Manger::GetInstance()->Print();
    return;
}

int main()
{
    std::thread threads[3];
    for (int i = 0; i < 3; ++i)
    {
        threads[i] = std::thread(ThreadFunc, 0);
        threads[i].join();
    }
    getchar();
    return 0;
}


