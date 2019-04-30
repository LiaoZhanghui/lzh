#ifndef __LOG_H__
#define __LOG_H__

#include <stdarg.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

enum _LOG_LEVEL
{
	LVL_DEBUG = 0,
	LVL_INFO,
	LVL_WARN,
	LVL_ERROR,
	LVL_FATAL,
};

// public
bool log_init();
void log_exit();

void log_by_level(int level, const char *format, ...);

void set_log_level(int level);
void save_to_file();
void export_all_logs(const char* dest_path);
void delete_all_logs();

#ifdef __cplusplus
}
#endif

#endif// __LOG_H__
