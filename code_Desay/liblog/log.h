#ifndef __LOG_H__
#define __LOG_H__

#include <stddef.h>
#include <stdarg.h>
#include <stdbool.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef SEM
typedef sem_t lock_t;
#else
typedef pthread_mutex_t lock_t;
#endif

typedef struct _META_T
{
    lock_t _lock;  // shall be inited only once
    int _init;     // init or not, 1-yes, 0-no
} meta_t;

typedef struct _LOG_CFG
{
    bool _running;
    bool _bfiltered;
    int _level;
    int _level_to_file;
    int _max_file_size;
    int _days_of_log;
    int _max_daily_rotation_index;
    char _dir[256];
    time_t _last_modified;
    pthread_t _tid_cfg;
    meta_t* _meta_data;
    char* _pmap;
} stLogCfg;

extern stLogCfg g_cfg;

enum _LOG_LEVEL
{
	LVL_DEBUG = 0,
	LVL_INFO,
	LVL_WARN,
	LVL_ERROR,
	LVL_FATAL,
};

// public
/**
 * @brief initialize log component, get ready for logging
 *
 * @return true : success
 * @return false : failed
 */
bool log_init();

/**
 * @brief : exit logging process, unmap and remove sharing files
 *
 */
void log_exit();

/**
 * @brief : set the level of logging message
 *
 * @param level : 0~4
 * @param format : using like printf
 * @param ...
 */
void log_by_level(int level, const char *format, ...);

/**
 * @brief : set the log level, logs with level equal or greater then 'level' will be recorded
 *
 * @param level : 0~4
 */
void set_log_level(int level);

/**
 * @brief save the log in mapped memory to default log file, rotate lines or files if necessary
 *
 */
void save_to_file();

/**
 * @brief : exprot log files to external storage device
 *
 * @param dest_path
 */
void export_all_logs(const char* dest_path);

/**
 * @brief : delete all saved log files
 *
 */
void delete_all_logs();

/**
 * @brief copy file from src_path to dest_path
 *
 * @param src_path
 * @param dest_path
 * @return true
 * @return false
 */
bool copy_file(const char* src_path, const char* dest_path);  // full path

void* map_existing_file(const char* file_path, unsigned long start_pos, size_t length);

/**
 * @brief initialize a lock for processes synchronization
 *
 * @return true
 * @return false
 */
bool synchronizer_init();
int init_lock(lock_t* lock);
int release_lock(lock_t* lock);
int lock(lock_t* lock);
int unlock(lock_t* lock);

#ifdef __cplusplus
}
#endif

#endif// __LOG_H__
