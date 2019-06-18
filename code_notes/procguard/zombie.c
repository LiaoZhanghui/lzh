#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/syslog.h>
#include <sys/param.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <pthread.h>


#ifdef SVP_LOG_TAG
    #undef SVP_LOG_TAG
#endif
#define SVP_LOG_TAG     "procguard"

#define get_file_name(x) strrchr(x, '/') ? strrchr(x, '/')+1 : x

#ifdef __PID__
    #undef __PID__
#endif
#define __PID__ getpid()

#ifdef __TID__
    #undef __TID__
#endif
#define __TID__ pthread_self()

#define SVP_INFO(argfmt, ...) \
{ \
    printf("[%s](%u|0x%08lX) [%s] [INFO] [%s: %3d]" argfmt "\r\n", __TIME__, __PID__, __TID__, SVP_LOG_TAG, get_file_name(__FILE__), __LINE__, ##__VA_ARGS__); \
}

#define SVP_WARN(argfmt, ...) \
{ \
    printf("[%s](%u|0x%08lX) [%s] [WARN] [%s: %3d]" argfmt "\r\n", __TIME__, __PID__, __TID__, SVP_LOG_TAG, get_file_name(__FILE__), __LINE__, ##__VA_ARGS__); \
}

#define SVP_ERROR(argfmt, ...) \
{ \
    printf("[%s](%u|0x%08lX) [%s] [ERROR] [%s: %3d]" argfmt "\r\n", __TIME__, __PID__, __TID__, SVP_LOG_TAG, get_file_name(__FILE__), __LINE__, ##__VA_ARGS__); \
}

void ChileProcessHandle()
{
    SVP_INFO("child process ...");
    for (int i = 0; i < 6; ++i)
    {
        SVP_INFO("i is %d", i);
        sleep(2);
    }
}

int init_daemon(void)
{
    int pid;
    pid = fork();
    if (pid < 0)
    {
        SVP_ERROR("fork error");
        return 0;
    }
    else if(pid == 0)
    {
        // 子进程
        ChileProcessHandle();
        exit(0);
    }
    
    SVP_INFO("father process ...");
    //int pidWait = wait(NULL);
    //SVP_INFO("I catched a child process with pid of %d", pidWait);
    return 0;
}

int main(int argc, char *argv[])
{
    init_daemon();

    SVP_INFO("press key to quit");
    getchar();
    return 0;
}
