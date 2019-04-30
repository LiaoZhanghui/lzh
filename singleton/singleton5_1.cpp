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
class SVPSingleton {
  public:
    /* brief Get instance
       return Instance of SVPSingleton<T> */
    static T* getInstance() {
        std::lock_guard<std::mutex> lock(_mtx);

        if (_singleton == nullptr)
            _singleton = new T();

        return _singleton;
    }

    /* brief Destroy instance */
    static void destroyInstance() {
        std::lock_guard<std::mutex> lock(_mtx);

        if (_singleton != nullptr)
            delete _singleton;

        _singleton = nullptr;
    }

  protected:
    SVPSingleton() {
        printf("[%lx]Singleton begin\n", pthread_self());
        sleep(1);
        printf("[%lx]Singleton end\n", pthread_self());
    }
    virtual ~SVPSingleton() {
        printf("[%lx]~Singleton\n", pthread_self());
    }

  private:
    static std::mutex _mtx;
    static T*                   _singleton;
};

template <class T>
std::mutex SVPSingleton<T>::_mtx;
template <class T>
T* SVPSingleton<T>::_singleton = nullptr;

///////////////////////////////////////////////////
class Manger: public SVPSingleton<Manger>
{
    friend class SVPSingleton <Manger>;
protected:
    Manger() {
        printf("[%lx]Manger begin, m_count is %d\n", pthread_self(), m_count);
        sleep(1);
        m_count = 123;
        printf("[%lx]Manger end, m_count is %d\n", pthread_self(), m_count);
    }
    ~Manger() {
        printf("[%lx]~Manger\n", pthread_self());
    }
public:
    void Print() {
        printf("[%lx]print out, m_count is %d \n", pthread_self(), ++m_count);
    }
private:
    int m_count;
};


void ThreadFunc(int n)
{
    printf("[%lx]thread begin \n", pthread_self());
    Manger::getInstance()->Print();
    Manger::destroyInstance();
    return;
}

int main()
{
    Manger::getInstance()->Print();
    std::thread threads[3];
    for (int i = 0; i < 3; ++i)
    {
        threads[i] = std::thread(ThreadFunc, 0);
        threads[i].detach();
    }
    getchar();
    Manger::getInstance()->Print();
    return 0;
}


