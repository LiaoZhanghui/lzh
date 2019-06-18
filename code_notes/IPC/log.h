#ifndef __LOG_H__
#define __LOG_H__

#ifdef SVP_LOG_TAG
    #undef SVP_LOG_TAG
#endif
#define SVP_LOG_TAG     "undefine"

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

#define SVP_PRINT(argfmt, ...) printf("%s:" argfmt "\r\n", SVP_LOG_TAG, ##__VA_ARGS__);

#endif //__LOG_H__
