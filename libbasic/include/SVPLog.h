#ifndef __SVP_LOG_H__
#define __SVP_LOG_H__

#include "SVPType.h"
#include "SVPString.h"
#include <string.h>
#include "log.h"


#ifdef SVP_LOG_TAG
	#undef SVP_LOG_TAG
#endif
#define SVP_LOG_TAG "default"

#define get_file_name(x) strrchr(x,'/') ? strrchr(x,'/')+1 : x		/* usage: get_file_name(__FILE__) */

#define SVP_DEBUG(argfmt, ...) \
	{ \
		log_by_level(LVL_DEBUG, "[%s][%s: %3d]" argfmt "\n", SVP_LOG_TAG, get_file_name(__FILE__), __LINE__, ##__VA_ARGS__); \
	}

#define SVP_INFO(argfmt, ...) \
	{ \
		log_by_level(LVL_INFO, "[%s][%s: %3d]" argfmt "\n", SVP_LOG_TAG, get_file_name(__FILE__), __LINE__, ##__VA_ARGS__); \
	}

#define SVP_WARN(argfmt, ...) \
	{ \
		log_by_level(LVL_WARN, "[%s][%s: %3d]" argfmt "\n", SVP_LOG_TAG, get_file_name(__FILE__), __LINE__, ##__VA_ARGS__); \
	}

#define SVP_ERROR(argfmt, ...) \
	{ \
		log_by_level(LVL_ERROR, "[%s][%s: %3d]" argfmt "\n", SVP_LOG_TAG, get_file_name(__FILE__), __LINE__, ##__VA_ARGS__); \
	}

#define SVP_FATAL(argfmt, ...) \
	{ \
		log_by_level(LVL_FATAL, "[%s][%s: %3d]" argfmt "\n", SVP_LOG_TAG, get_file_name(__FILE__), __LINE__, ##__VA_ARGS__); \
	}

// extension
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

#endif// __SVP_LOG_H__
