#ifndef __SVP_LOG_H__
#define __SVP_LOG_H__

#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <libgen.h>
#include <pthread.h>
#include <string.h>

#ifdef __PID__
	#undef __PID__
#endif
#define __PID__ getpid()

#ifdef __TID__
	#undef __TID__
#endif
#define __TID__ pthread_self()

#ifdef SVP_LOG_TAG
	#undef SVP_LOG_TAG
#endif
#define SVP_LOG_TAG "app"

#define get_file_name(x) strrchr(x, '/') ? strrchr(x, '/')+1 : x

static inline int get_tick_count(void)
{
	struct timespec time;
	clock_gettime(CLOCK_MONOTONIC, &time);
	return (time.tv_sec * 1000 + time.tv_nsec/(1000 * 1000));// millisecond
}

//#define SVP_NO_DEBUG
//#define SVP_NO_INFO
//#define SVP_NO_WARN
//#define SVP_NO_ERROR

#ifdef SVP_NO_DEBUG
	#define SVP_DEBUG(argfmt, ...)
#else
	#define SVP_DEBUG(argfmt, ...) \
	{ \
		fprintf(stderr, "[%d](%u|0x%08lX) [%s] [DEBUG] [%s: %3d]" argfmt "\r\n", get_tick_count(), __PID__, __TID__, SVP_LOG_TAG, get_file_name(__FILE__), __LINE__, ##__VA_ARGS__); \
	}
#endif

#ifdef SVP_NO_INFO
	#define SVP_INFO(argfmt, ...)
#else
	#define SVP_INFO(argfmt, ...) \
	{ \
		fprintf(stderr, "[%d](%u|0x%08lX) [%s] [INFO] [%s: %3d]" argfmt "\r\n", get_tick_count(), __PID__, __TID__, SVP_LOG_TAG, get_file_name(__FILE__), __LINE__, ##__VA_ARGS__); \
	}
#endif

#ifdef SVP_NO_WARN
	#define SVP_WARN(argfmt, ...)
#else
	#define SVP_WARN(argfmt, ...) \
	{ \
		fprintf(stderr, "[%d](%u|0x%08lX) [%s] [WARN] [%s: %3d]" argfmt "\r\n", get_tick_count(), __PID__, __TID__, SVP_LOG_TAG, get_file_name(__FILE__), __LINE__, ##__VA_ARGS__); \
	}
#endif

#ifdef SVP_NO_ERROR
	#define SVP_ERROR(argfmt, ...)
#else
	#define SVP_ERROR(argfmt, ...) \
	{ \
		fprintf(stderr, "[%d](%u|0x%08lX) [%s] [ERROR] [%s: %3d]" argfmt "\r\n", get_tick_count(), __PID__, __TID__, SVP_LOG_TAG, get_file_name(__FILE__), __LINE__, ##__VA_ARGS__); \
	}
#endif

#ifdef SVP_NO_IO_INFO
	#define SVP_IO_INFO(argfmt, ...)
#else
	#define SVP_IO_INFO(argfmt, ...) \
			{\
				fprintf(stderr, "(%u|0x%08lX) [%s] INFO: " argfmt "\r\n", get_tick_count(),__PID__, __TID__, SVP_LOG_TAG,get_file_name(__FILE__), __LINE__, ##__VA_ARGS__);\
			}
#endif
// extension
#ifdef SVP_NO_INFO
	#define SVP_INFO_FUNC()
	#define SVP_INFO_FUNC_BEGIN()
	#define SVP_INFO_FUNC_END()
	#define SVP_INFO_BUILD_TIME()
#else
	#define SVP_INFO_FUNC() \
	{ \
		SVP_INFO("%s()", __FUNCTION__); \
	}
	#define SVP_INFO_FUNC_BEGIN() \
	{ \
		SVP_INFO("%s() - Begin", __FUNCTION__); \
	}
	#define SVP_INFO_FUNC_END() \
	{ \
		SVP_INFO("%s() - End", __FUNCTION__); \
	}
	#define SVP_INFO_BUILD_TIME() \
	{ \
		SVP_INFO("build time -- %s %s", __DATE__, __TIME__); \
	}
#endif

#endif// __SVP_LOG_H__
