
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <cstdlib>
#include <pthread.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

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

int main (int argc, char *argv[])
{
    if (argc < 2)
    {
        exit(-1);
    }

    char strArg[256] = {0};
    snprintf(strArg, 256, "%s", argv[1]);

GUARD_ACTIVE:

    if ( 0 == fork())
    {
        if (execvp(argv[1], &argv[1]) == -1)
        {
            SVP_ERROR("execvp(%s) failed, error no = %d.\n", strArg, errno);
            exit(-1);
        }
        exit(0);
    }
    else
    {
        int status = 0;
        pid_t pid = wait(&status);

        if (status < 0 || status > 255)
        {
            // Range of exit status is 0 ~ 255
            SVP_WARN("%s(%d) exit with status %d, guard unusable, exiting now ...\n", strArg, pid, status);
        }
        else
        {
            SVP_INFO("%s(%d) exit with status %d, restart after 1s ...\n", strArg, pid, status);
            sleep(1);
            goto GUARD_ACTIVE;
        }
    }
    exit(0);
}


