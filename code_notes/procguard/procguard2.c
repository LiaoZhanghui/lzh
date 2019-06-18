#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/syslog.h>
#include <sys/param.h>
#include <sys/types.h>
#include <sys/stat.h>
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


int init_daemon(void)
{
    int pid; 
    int i;

    // 1）屏蔽一些控制终端操作的信号  
    signal(SIGTTOU,SIG_IGN);
    signal(SIGTTIN,SIG_IGN);
    signal(SIGTSTP,SIG_IGN);
    signal(SIGHUP,SIG_IGN);

    // 2）在后台运行
    if( pid=fork() ){   // 父进程
        SVP_INFO("exit 0");
        exit(0);        // 结束父进程，子进程继续
    }else if(pid< 0){   // 出错
        perror("fork");
        exit(-1);
    }

    // 3）脱离控制终端、登录会话和进程组
    SVP_INFO("setsid");
    setsid(); 

    // 4）禁止进程重新打开控制终端
    if( pid=fork() ){   // 父进程
        SVP_INFO("exit 0");
        exit(0);        // 结束第一子进程，第二子进程继续（第二子进程不再是会话组长）
    }else if(pid< 0){   // 出错
        perror("fork");
        exit(-1);
    } 

    // 5）关闭打开的文件描述符  
    // NOFILE 为 <sys/param.h> 的宏定义  
    // NOFILE 为文件描述符最大个数，不同系统有不同限制  
    for(i=0; i< 3; ++i){
        SVP_INFO("i is %d", i);
        sleep(2);
    }  

    // 6）改变当前工作目录
    SVP_INFO("chdir");
    chdir("/tmp");

    // 7）重设文件创建掩模
    SVP_INFO("umask");
    umask(0);

    // 8）处理 SIGCHLD 信号
    signal(SIGCHLD,SIG_IGN);

    return 0;
}

int main(int argc, char *argv[])
{
    init_daemon();

    while(1);

    return 0;
}
