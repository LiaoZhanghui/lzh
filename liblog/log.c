#include "log.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <libgen.h>
#include <sys/mman.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <regex.h>

#ifdef __PID__
#undef __PID__
#endif
#define __PID__ getpid()

#ifdef __TID__
#undef __TID__
#endif
#define __TID__ pthread_self()

#define LVL_DEFAULT 1 /* default log level */
#define LVL_2_FILE 1  /* default log level to file */

#ifdef PC_TEST
#define LOG_DIR "log"
#define LOG_DIR_PREFIX(file) "log/"#file
#define LOG_CFG_FILE_ORIGINAL "g5r2.svp.log.conf.orig"
#define LOG_CFG_FILE_BACKUP LOG_DIR_PREFIX(g5r2.svp.log.conf.bak)
#define LOG_CFG_FILE "/tmp/g5r2.svp.log.conf"
#define META_FILE "/tmp/.meta"
#define LOG_FILE ".log"
#define MAP_BUF_SIZE (1024 + 1024 * 1024U) /* mmap file size: 2Mb, actually 1Kb  */
#define LOG_BUF_SIZE (4 * 1024U)           /* mmap size, data size write to file once a time, 4Kb */
#define MAX_FILE_SIZE (10 * 1024U)         /* 10K */
#else
#define LOG_DIR "/storage/data/data/log"
#define LOG_DIR_PREFIX(file) "/storage/data/data/log/"#file
#define LOG_CFG_FILE_ORIGINAL "/svp/etc/svp.log.conf.orig"
#define LOG_CFG_FILE_BACKUP LOG_DIR_PREFIX(svp.log.conf.bak)
#define LOG_CFG_FILE "/tmp/svp.log.conf"
#define META_FILE "/tmp/.meta"
#define LOG_FILE LOG_DIR_PREFIX(.log)
#define MAP_BUF_SIZE (2 * 1024 * 1024U)   /* mmap file  size:  2Mb, actually 1Mb  */
#define LOG_BUF_SIZE (1024 * 1024U)       /* mmap size, data size write to file once a time, 1Mb */
#define MAX_FILE_SIZE (10 * 1024 * 1024U) /* log file size: 10M */
#endif

#define LOG_NAME_PREFIX "dsv_log_"
#define LOG_NAME_SUFFIX ".txt"
#define LOG_COPY_FLAG_FILE "/media/usbstorage0/GetLog.txt" /* file in usb  */
#define LINE_BUF_LEN 1024U                                 /* max line length of log */
#define LOG_SERVICE_NAME "svp.svc.log"

#define get_file_name(x) strrchr(x, '/') ? strrchr(x, '/') + 1 : x /* usage: get_file_name(__FILE__) */

#ifndef _POSIX_THREAD_PROCESS_SHARED
#error "This platform does not support process shared mutex"
#endif

typedef void* ptfunc(void*);  // type of thread function pointer

stLogCfg g_cfg = { false, false, LVL_DEFAULT, LVL_2_FILE, MAX_FILE_SIZE, 7, 3, LOG_DIR, 0L, 0UL, NULL, NULL };
static bool bFiltered = false;

#ifdef dbg
#undef dbg
#endif
#define dbg(fmt, ...)                                                                                                                         \
    {                                                                                                                                         \
        printf("[%d] (%u|0x%08lX) [%s(%d)] " fmt "\n", get_tick_count(), __PID__, __TID__, get_file_name(__FILE__), __LINE__, ##__VA_ARGS__); \
    }
#define error_handle(msg)   \
    do {                    \
        perror(msg);        \
        exit(EXIT_FAILURE); \
    } while (0)
#define void_error_handle(msg) \
    do {                       \
        perror(msg);           \
        return;                \
    } while (0)

// declaration
int get_tick_count(void);
const char* get_time(int format);
const char* get_log_date(char* buf, int size);
const char* generate_file_name(char* buf, int size);
const char* lvl2str(int level);
int str2lvl(const char* level);
void get_pname(char pid_name[], unsigned int size);
long get_file_size(const char* path);
void set_filter_status(bool bFiltered);
void load_log_cfg();
void apply_configurations(const char* tag, const char* value);
void output_cfg();
bool init_cfg_file();
void check_cfg_file(const char* path);
void check_usb_file(const char* path);
void rotate_log_file(const char* filepath);
void rotate_log_line(const char* filepath);
void archive_log_file(const char* filepath);
void compress_log_file(const char* filepath);
bool copy_file_mm(const char* src_path, const char* dest_path);  // copy file via memory mapping
bool copy_folder(const char* src_path, const char* dest_path);
void rename_file(const char* old_path, const char* new_path);
void delete_file(const char* filepath);  // full path
void replace_in_file(const char* file_path, const char* from, const char* to);
long regex_search_the_first_occurrence_in_file(const char* file_path, const char* pattern);
const char* regex_search_the_first_occurrence_in_string(const char* src, const char* pattern);
void* monitor_congfiguration(void* arg);
void* monitor_for_log_copy(void* arg);
void* manage_log_file(void* arg);
void* log_test(void* arg);
meta_t* get_meta_map();
char* get_log_map();
pthread_t init_sub_thread(ptfunc function);

// definition
int getlvl()
{
    // dbg("[INFO] log level: %d", g_cfg._level);
    return g_cfg._level;
}

int get_lvl_to_file()
{
    return g_cfg._level_to_file;
}

// time
inline int get_tick_count(void)
{
    struct timespec time;
    clock_gettime(CLOCK_MONOTONIC, &time);                       // since power on
    return (time.tv_sec * 1000 + time.tv_nsec / (1000 * 1000));  // millisecond
}

#define TIME_FORMAT_NORMAL "%04d-%02d-%02d %02d:%02d:%02d"
#define TIME_FORMAT_FNAME "%04d-%02d-%02d"
const char* get_time(int format)  // format: TIME_FORMAT_NORMAL || TIME_FORMAT_FNAME
{
    struct timespec time;
    clock_gettime(CLOCK_REALTIME, &time);  // seconds since 1970-1-1 00:00:00

    struct tm now;
    localtime_r(&time.tv_sec, &now);
    static char p[32];  // it would be overwritten after calling everytime
    memset(p, 0, sizeof(p));
    if (1 == format) {
        sprintf(p, TIME_FORMAT_FNAME, now.tm_year + 1900, now.tm_mon + 1, now.tm_mday);
    } else {
        sprintf(p, TIME_FORMAT_NORMAL, now.tm_year + 1900, now.tm_mon + 1, now.tm_mday, now.tm_hour, now.tm_min, now.tm_sec);
    }
    return p;
}

const char* get_log_date(char* buf, int size)
{
    if (NULL == buf) return "";
    time_t t = time(NULL);
    struct tm now;
    memset(&now, 0, sizeof(now));
    localtime_r(&t, &now);
    snprintf(buf, size, "%04d%02d%02d", now.tm_year + 1900, now.tm_mon + 1, now.tm_mday);
    // dbg("[INFO] date: %s", buf);
    return buf;
}

const char* generate_file_name(char* buf, int size)
{
    if (NULL == buf) return "";
    // get log date
    char date[9] = { 0 };
    memset(date, 0, sizeof(date));
    get_log_date(date, sizeof(date));
    snprintf(buf, size, "%s/%s%s%s", g_cfg._dir, LOG_NAME_PREFIX, date, LOG_NAME_SUFFIX);
    // dbg("[INFO] file name: %s", buf);
    return buf;
}

// level
const char* lvl2str(int level)
{
    switch (level) {
    case LVL_DEBUG: return "DEBUG";
    case LVL_INFO: return "INFO";
    case LVL_WARN: return "WARN";
    case LVL_ERROR: return "ERROR";
    case LVL_FATAL: return "FATAL";
    default: return "UNKNOWN";
    }
}

int str2lvl(const char* level)
{
    // dbg("[INFO] %s(%s)", __FUNCTION__, level);
    if (0 == strcmp(level, "DEBUG")) {
        return LVL_DEBUG;
    } else if (0 == strcmp(level, "INFO")) {
        return LVL_INFO;
    } else if (0 == strcmp(level, "WARN")) {
        return LVL_WARN;
    } else if (0 == strcmp(level, "ERROR")) {
        return LVL_ERROR;
    } else if (0 == strcmp(level, "FATAL")) {
        return LVL_FATAL;
    } else {
        return LVL_DEFAULT;
    }
}

// pid name
void get_pname(char pid_name[], unsigned int size)
{
    if (NULL == pid_name) return;

    char buf[256] = "";
    errno = 0;
    // the pid name in "/proc/self/status" was limited within 16 byte (including tailed '\0')
    // so, it cannot be extract from "/proc/self/status"
    int ret = readlink("/proc/self/exe", buf, sizeof(buf) - 1);
    if (-1 == ret) {
        dbg("[ERROR]: failed to get process name, %s", strerror(errno));
        return;
    }

    snprintf(pid_name, size, "%s", get_file_name(buf));
    // dbg("[INFO] pid[%u], pid_name[%s]", __PID__, pid_name);
}

long get_file_size(const char* path)
{
    long size = 0L;
    struct stat st;
    if (0 == stat(path, &st)) {
        size = st.st_size;
    }
    return size;
}

void set_log_level(int level)
{
    if (level == g_cfg._level) return;

    char tag[] = "LOG_LEVEL=";
    char from[128] = "";
    char to[128] = "";

    snprintf(from, sizeof(from), "%s%s", tag, lvl2str(g_cfg._level));
    snprintf(to, sizeof(to), "%s%s", tag, lvl2str(level));

    replace_in_file(LOG_CFG_FILE, from, to);
}

void set_filter_status(bool bFiltered)
{
    g_cfg._bfiltered = bFiltered;
   // dbg("[INFO] pid[%u] filter flag: %d", __PID__, bFiltered);
}

bool synchronizer_init()
{
    // create/open meta map
    if (g_cfg._meta_data == NULL)
        g_cfg._meta_data = get_meta_map();
    if (NULL ==  g_cfg._meta_data) return false;
    return true;
}

int init_lock(lock_t* lock)
{
    // dbg("[INFO] initializing lock");
    int ret = -1;
    if (NULL == lock) return ret;
#ifdef SEM
    ret = sem_init(&g_cfg._meta_data->_lock, 1, 1)  // used for sharing between processes
#else
    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED);
    pthread_mutexattr_setrobust(&attr, PTHREAD_MUTEX_ROBUST);
    ret = pthread_mutex_init(lock, &attr);
#endif
    if (0 != ret) {
        dbg("[ERROR] failed to init lock");
    }
    return ret;
}

int release_lock(lock_t* lock)
{
    int ret = -1;
    if (NULL == lock) return ret;
#ifdef SEM
    ret = sem_destroy(lock);
#else
    ret = pthread_mutex_destroy(lock);
#endif
    if (0 != ret) {
        dbg("[ERROR] failed to destroy lock, %s", strerror(ret));
    }
    return ret;
}

int lock(lock_t* lock)
{
    int ret = -1;
    if (NULL == lock) return ret;
#ifdef SEM
    ret = sem_wait(lock);
#else
    ret = pthread_mutex_lock(lock);
    if (EOWNERDEAD == ret) {
        dbg("[WARN] inconsistent mutex lock, caused by the owner teminates while holding the mutex");
        ret = pthread_mutex_consistent(lock);
        if (0 != ret) dbg("[INFO] failed to mark consistent");
    }
#endif
    if (0 != ret) {
        dbg("[ERROR] failed to lock, %s", strerror(ret));
    }
    return ret;
}

int unlock(lock_t* lock)
{
    int ret = -1;
    if (NULL == lock) return ret;
#ifdef SEM
    ret = sem_post(lock);
#else
    ret = pthread_mutex_unlock(lock);
#endif
    if (0 != ret) {
        dbg("[ERROR] failed to unlock, %s", strerror(ret));
    }
    return ret;
}

// logging
void set_log_header(int level)
{
#ifdef DEV
    fprintf(stdout, "[%d] (%u|0x%08lX) [%s] ", get_tick_count(), __PID__, __TID__, lvl2str(level));
#endif
    size_t size = strlen(g_cfg._pmap);
    //snprintf(g_cfg._pmap + size, MAP_BUF_SIZE - size, "[%d] (%u|0x%08lX) [%s] ", get_tick_count(), __PID__, __TID__, lvl2str(level));
}

void log_by_level(int level, const char* format, ...)
{
    if (!log_init() || g_cfg._bfiltered || (NULL == format) || (0 == format[0])) return;
    if (level < getlvl()) return;
    if ((NULL == g_cfg._meta_data) || (NULL == g_cfg._pmap)) {
        dbg("[ERROR] +++++++++++++ map does not exist +++++++++++++");
        return;
    }

    lock(&g_cfg._meta_data->_lock);

    // dbg("[INFO] position: %p, log size: [%luK %luB]", g_cfg._pmap+size, size/1024, size%1024);
    /*if (strlen(g_cfg._pmap) > LOG_BUF_SIZE)  // save log to file when data size reached 1Mb
    {
        save_to_file();
        memset(g_cfg._pmap, 0, MAP_BUF_SIZE);
    }*/

    // log header
    set_log_header(level);

    // log content
    va_list ap;
    va_start(ap, format);
#ifdef DEV
    va_list copy;
    va_copy(copy, ap);
    vprintf(format, copy);  // the value of ap is undefined after the v*printf family of functions call, and ap cannot be used again
    va_end(copy);
#endif
    //vsnprintf(g_cfg._pmap + strlen(g_cfg._pmap), LINE_BUF_LEN, format, ap);  // the value of ap is undefined after the v*printf family of functions call
    va_end(ap);

    unlock(&g_cfg._meta_data->_lock);
}

// configuration
void apply_configurations(const char* tag, const char* value)
{
    // dbg("[INFO] apply_configurations(%s, %s)", tag, value);
    if ((NULL == tag) || (NULL == value) || ("" == value)) return;
    if (0 == strcmp(tag, "LOG_LEVEL")) {
        char tmp[8] = "";
        errno = 0;
        if (1 != sscanf(value, "%7s", tmp)) {
            dbg("[ERROR] failed to set log level, %s", strerror(errno));
            return;
        }
        g_cfg._level = str2lvl(tmp);
        // dbg("[INFO] _level = %d (%s)", g_cfg._level, lvl2str(g_cfg._level));
    } else if (0 == strcmp(tag, "LOG_LEVEL_TO_FILE")) {
        char tmp[8] = "";
        errno = 0;
        if (1 != sscanf(value, "%7s", tmp)) {
            dbg("[ERROR] failed to set log (to file) level, %s", strerror(errno));
            return;
        }
        g_cfg._level_to_file = str2lvl(tmp);
    } else if (0 == strcmp(tag, "LOG_DIR")) {
        char tmp[256] = "";
        errno = 0;
#ifdef PC_TEST
        if (1 != sscanf(LOG_DIR, "%255s", tmp)) {
#else
        if (1 != sscanf(value, "%255s", tmp)) {
#endif
            dbg("[ERROR] failed to set log dir, %s", strerror(errno));
            return;
        }
        if (0 != access(tmp, F_OK)) {
            dbg("[ERROR] dir doesn't exist, using default: %s", g_cfg._dir);
            return;
        }
        memset(g_cfg._dir, 0, sizeof(g_cfg._dir));
        strncpy(g_cfg._dir, tmp, 256);
    } else if (0 == strcmp(tag, "FILTER_PID")) {
        /*
        g_cfg._filters.clear();
        char* token = strsep(&value, " ");
        for (; token!=NULL; token=strsep(&value, " "))
        {
            if (0 == strlen(token)) continue;
            dbg("[INFO] token = %s", token);
            g_cfg._filters.insert(token);
        }
        */
        char pid_name[512] = { '"' };
        memset(pid_name + 1, 0, sizeof(pid_name) - 1);
        get_pname(pid_name + 1, sizeof(pid_name) - 2);  // reserve 1 byte for tailed '"', 1 byte for string teminator '\0'
        pid_name[strlen(pid_name)] = '"';
        //dbg("[INFO] pid[%u], pid_name[%s]", __PID__, pid_name);
        char* p = strstr(value, pid_name);
        if (NULL != p) set_filter_status(true);
    } else if (0 == strcmp(tag, "LOG_SAVE_DAYS")) {
        int val = 0;
        errno = 0;
        if (1 != sscanf(value, "%d", &val)) {
            dbg("[ERROR] failed to set log file count, keep <7d> as default, %s", strerror(errno));
            return;
        }
        g_cfg._days_of_log = val;
    } else if (0 == strcmp(tag, "MAX_LOG_FILE_SIZE")) {
        int val = 0;
#ifdef PC_TEST
        val = 10;
#else
        errno = 0;
        if (1 != sscanf(value, "%d", &val)) {
            dbg("[ERROR] failed to set log file size, keep <10M> as default, %s", strerror(errno));
            return;
        }
#endif
        if (val > 10) val = 10;
        if (val < 1) val = 1;
        g_cfg._max_file_size = val;
    } else if (0 == strcmp(tag, "MAX_DAILY_ROTATION_INDEX")) {
        int val = 0;
        errno = 0;
        if (1 != sscanf(value, "%d", &val)) {
            dbg("[ERROR] failed to set log rotation count, keep <3> as default, %s", strerror(errno));
            return;
        }
        if (val > 5) val = 5;
        if (val < 0) val = 0;
        g_cfg._max_daily_rotation_index = val;
    }
}

void output_cfg()
{
    //dbg("[INFO] log configurations:");
    //dbg("[INFO] running        : %d", g_cfg._running);
   // dbg("[INFO] bfiltered      : %d", g_cfg._bfiltered);
   // dbg("[INFO] level          : %d", g_cfg._level);
    // dbg("[INFO] level to file  : %d", g_cfg._level_to_file);
    //dbg("[INFO] dir            : %s", g_cfg._dir);
   // dbg("[INFO] configuration last modified: %s", ctime(&g_cfg._last_modified));
}

void load_log_cfg()
{
    // dbg("[INFO] loading configurations");
    errno = 0;
    FILE* fp = fopen(LOG_CFG_FILE, "r");  // LOG_CFG_FILE must be exist
    if (NULL == fp) {
        dbg("[ERROR] failed to open log config file[%s], %s", LOG_CFG_FILE, strerror(errno));
        return;
    }

    char buf[LINE_BUF_LEN] = { 0 };
    while (NULL != fgets(buf, LINE_BUF_LEN - 1, fp)) {
        // ignore comment line begin with '#'
        if ('#' == buf[0]) continue;
        size_t len = strlen(buf);
        // ignore null line
        if (0 == len) continue;
        // delete tailed '\n'
        if ('\n' == buf[len - 1])  // last line contains EOF, not '\n'
        {
            buf[len - 1] = '\0';
        }

        char* token = strchr(buf, '=');
        if (NULL == token) continue;
        // dbg("[INFO] config '%s'", buf);

        *token = '\0';
        apply_configurations(buf, token + 1);
    }
    fclose(fp);
    // output_cfg();
}

bool init_cfg_file()
{
    // dbg("[INFO] %s", __FUNCTION__);
    int fd = open(LOG_CFG_FILE_BACKUP, O_CREAT | O_EXCL, 0644);
    if (-1 != fd)  // expect success for the first startup after reset
    {
        close(fd);
        // copy config file if it does not exist
        if (!copy_file(LOG_CFG_FILE_ORIGINAL, LOG_CFG_FILE_BACKUP)) {
            dbg("[ERROR] failed to init configuration file");
            return false;
        }
    }

    fd = open(LOG_CFG_FILE, O_CREAT | O_EXCL, 0644);
    if (-1 != fd)  // expect success on every startup, recover config file saved last time
    {
        close(fd);
        // copy config file if it does not exist
        if (!copy_file(LOG_CFG_FILE_BACKUP, LOG_CFG_FILE)) {
            dbg("[ERROR] failed to init configuration file for current session");
            return false;
        }
    }

    return true;
}

void check_cfg_file(const char* path)
{
    // dbg("[INFO] %s", __FUNCTION__);
    if (NULL == path) return;

    struct stat s;
    if (0 != stat(path, &s)) {
        dbg("[WARN] file does not exist - %s", path);
        return;
    }

    if (g_cfg._last_modified != s.st_mtime) {
        //dbg("[INFO] log configurations changed, reload");
        g_cfg._last_modified = s.st_mtime;
        load_log_cfg();
    }
}

void check_usb_file(const char* path)
{
    if (NULL == path) return;
    if (0 != access(path, F_OK)) return;
    save_to_file();

    char cmd[256];
    memset(cmd, 0, sizeof(cmd));
    snprintf(cmd, 255, "cp -rf %s /media/usbstorage0/", g_cfg._dir);
    errno = 0;
    if (0 != system(cmd)) {
        dbg("[WARN] failed to copy logs to usb, %s", strerror(errno));
        return;
    }
    remove(LOG_COPY_FLAG_FILE);
    sleep(1);
    system("sync");
    //dbg("[INFO] log dumped to usb");
}

void* monitor_congfiguration(void* arg)
{
    //dbg("[INFO] configuration monitor thread created - 0x%08lX", pthread_self());
    while (g_cfg._running) {
        check_cfg_file(LOG_CFG_FILE);
        sleep(2);
    }
    return NULL;
}

void* monitor_for_log_copy(void* arg)
{
    dbg("[INFO] log copy monitor thread created - 0x%08lX", pthread_self());
    while (g_cfg._running) {
        check_usb_file(LOG_COPY_FLAG_FILE);
        sleep(5);
    }
    return NULL;
}

#define IS_TAR_FILE(x) ((0 == strncmp(x, ".tar", sizeof(".tar") - 1)) || (0 == strncmp(x, ".TAR", sizeof(".TAR") - 1)))
#define IS_GZ_FILE(x) ((0 == strncmp(x, ".gz", sizeof(".gz") - 1)) || (0 == strncmp(x, ".GZ", sizeof(".GZ") - 1)))
#define IS_TXT_FILE(x) ((0 == strncmp(x, ".txt", sizeof(".txt") - 1)) || (0 == strncmp(x, ".TXT", sizeof(".TXT") - 1)))
void* manage_log_file(void* arg)
{
    dbg("[INFO] log manage thread created - 0x%08lX", pthread_self());
    errno = 0;
    DIR* pdir = opendir(g_cfg._dir);
    if (NULL == pdir) {
        dbg("[ERROR] failed to open log dir[%s], %s", g_cfg._dir, strerror(errno));
        pthread_detach(pthread_self());
        return NULL;
    }

    struct dirent* pentry;
    struct stat st;
    char file_path[1024];
    while (pentry = readdir(pdir)) {
        // dbg("[INFO] file name: %s", pentry->d_name);
        if (0 == strncmp(pentry->d_name, ".", 1)) continue;  // current dir(.) or parent dir(..) or hidden files

        memset(file_path, 0, sizeof(file_path));
        snprintf(file_path, sizeof(file_path) - 1, "%s/%s", g_cfg._dir, pentry->d_name);  // full path

        char* ext = strchr(pentry->d_name, '.');
        if ((NULL != ext) && (IS_TXT_FILE(ext))) {
            // archive
            archive_log_file(file_path);
        }
    }
    closedir(pdir);

    pdir = opendir(g_cfg._dir);
    while (pentry = readdir(pdir)) {
        if (0 == strncmp(pentry->d_name, ".", 1)) continue;

        memset(file_path, 0, sizeof(file_path));
        snprintf(file_path, sizeof(file_path) - 1, "%s/%s", g_cfg._dir, pentry->d_name);  // full path

        char* ext = strrchr(pentry->d_name, '.');  // NOTE: not strchr
        if (NULL != ext) {
            if (IS_GZ_FILE(ext)) {
                errno = 0;
                if (0 != lstat(file_path, &st)) {
                    dbg("[ERROR] failed to get file stat[%s], %s", pentry->d_name, strerror(errno));
                    continue;
                }

                time_t t = time(NULL);
                t -= g_cfg._days_of_log * 24 * 3600;
                // remove older files
                if (st.st_ctime < t) {
                    delete_file(file_path);
                }
            } else if (IS_TAR_FILE(ext)) {
                // compress
                compress_log_file(file_path);
            } else {
                // delete_file(file_path);
            }
        }
    }
    closedir(pdir);

    pthread_detach(pthread_self());
    return NULL;
}

void* log_test(void* arg)
{
    dbg("[INFO] log test thread created - 0x%08lX", pthread_self());
    while (g_cfg._running) {
        usleep(10000);
        static int count = 0;
        if (count > 1000) break;
        log_by_level(0, "[%d]this is a test log", ++count);
    }
    return NULL;
}

pthread_t init_sub_thread(ptfunc function)
{
    if (NULL == function) return 0UL;
    pthread_t tid = 0UL;
    errno = 0;
    if (0 != pthread_create(&tid, NULL, function, NULL)) {
        dbg("[ERROR] failed to create sub thread, %s", strerror(errno));
    }
    return tid;
}

bool log_init()
{
    // dbg("[INFO] %s - %u", __FUNCTION__, getpid());
    if (g_cfg._running) return true;
   // dbg("[INFO] initializing log service, pid - %u", getpid());
    g_cfg._running = true;

    // configuration
    if (init_cfg_file()) {
        check_cfg_file(LOG_CFG_FILE);
    }

    // test log dir
    if (0 != access(g_cfg._dir, F_OK)) {
        errno = 0;
        if (0 != mkdir(g_cfg._dir, 0755)) {
            dbg("[ERROR] failed to create log dir [%s], %s", g_cfg._dir, strerror(errno));
        }
    }

    // synchronizer initialization
    if (!synchronizer_init()) return false;

    // create/open log map
    g_cfg._pmap = get_log_map();
    if (NULL == g_cfg._pmap) {
        return false;
    } else {
        //dbg("[INFO] log size: %lu", strlen(g_cfg._pmap));
        //if (strlen(g_cfg._pmap) > LOG_BUF_SIZE) save_to_file();
    }

    // configuration monitor
    g_cfg._tid_cfg = init_sub_thread(monitor_congfiguration);

#ifndef PC_TEST
    char pid_name[256];
    memset(pid_name, 0, sizeof(pid_name));
    get_pname(pid_name, sizeof(pid_name));
    if (0 == strcmp(LOG_SERVICE_NAME, pid_name)) {
#endif
        // log copy monitor
        init_sub_thread(monitor_for_log_copy);

        // manage log file
        init_sub_thread(manage_log_file);

#ifdef PC_TEST
        // test
        // init_sub_thread(log_test);
#endif
#ifndef PC_TEST
    }
#endif
    //dbg("[INFO] log service initialized");
    return true;
}

void* map_existing_file(const char* file_path, unsigned long start_pos, size_t length)
{
    // Note that most hardware designs cannot support write permission without read permission, and
    // many do not distinguish read and execute permission. Thus, you may receive wider permissions
    // than you ask for, and mappings of write-only files may be denied even if you do not use PROT_READ.
    // that means, O_WRONLY in "open()" may cause mmap failure
    // open file
    errno = 0;
    int fd = open(file_path, O_RDWR);  // O_WRONLY will cause mmap failure
    if (fd < 0) {
        dbg("[ERROR] failed to open file[%s], %s", file_path, strerror(errno));
        return NULL;
    }

    // resize if file size is less than specified map length
    long file_size = get_file_size(file_path);
    if ((-1 != file_size) && ((size_t)file_size < length)) {
        ftruncate(fd, length);
    }

    // map file into memory
    errno = 0;
    void* pmap = mmap(NULL, length, PROT_READ | PROT_WRITE, MAP_SHARED, fd, start_pos);
    if (MAP_FAILED == pmap)  // ((void*)-1)
    {
        dbg("[ERROR] failed to map file: %s", strerror(errno));
        pmap = NULL;
    }

    // close file after mapping
    errno = 0;
    if (0 != close(fd)) {
        dbg("[WARN] failed to close log file[%s], %s", LOG_FILE, strerror(errno));
    }

    return pmap;
}

meta_t* get_meta_map()
{
    // dbg("[INFO] start to create/open meta map");
    size_t size = sizeof(meta_t);
    // create map file
    int fd = open(META_FILE, O_CREAT | O_EXCL | O_RDWR, 0644);  // O_WRONLY will cause mmap failure
    if (fd < 0) {
        //dbg("[INFO] file exists and already initialized");
    } else {
        ftruncate(fd, size);
        close(fd);
        // initialize after created, will be executed only once due to flag O_EXCL
        meta_t* ptr = (meta_t*)map_existing_file(META_FILE, 0, size);
        if (NULL != ptr) {
            if (1 != ptr->_init) {
                memset(ptr, 0, size);
                init_lock(&ptr->_lock);
                ptr->_init = 1;
            }
            //dbg("[INFO] lock init status: %d", ptr->_init);
            munmap(ptr, size);
        }
    }

    return (meta_t*)map_existing_file(META_FILE, 0, size);
}

char* get_log_map()
{
    // dbg("[INFO] start to create/open log map");
    errno = 0;
    int fd = open(LOG_FILE, O_CREAT | O_RDWR, 0644);  // create if the file does not exist, otherwise, open it
    if (fd < 0) {
        dbg("[ERROR] failed to create/open log file[%s], %s", LOG_FILE, strerror(errno));
        return NULL;
    }
    ftruncate(fd, MAP_BUF_SIZE);
    close(fd);

    // memory mapping
    return (char*)map_existing_file(LOG_FILE, 0, MAP_BUF_SIZE);
}

void save_to_file()
{
    // dbg("[INFO] start to save log");

    // get file name
    char file_name[256];
    memset(file_name, 0, sizeof(file_name));
    generate_file_name(file_name, sizeof(file_name) - 1);
    long file_size   = get_file_size(file_name);
    size_t size_left = strlen(g_cfg._pmap);

    // rollover
    if (size_left + get_file_size(file_name) > MAX_FILE_SIZE) {
        if (g_cfg._max_daily_rotation_index > 0) {
            rotate_log_file(file_name);
        } else {
            rotate_log_line(file_name);
        }
    }

    // save to file
    errno = 0;
    int fd = open(file_name, O_CREAT | O_WRONLY | O_APPEND, 0644);
    if (fd < 0) {
        dbg("[ERROR] failed to open/create log file[%s], %s", file_name, strerror(errno));
        return;
    }

    char* p    = g_cfg._pmap;
    size_t len = 0;
    errno = 0;
    do {
        // dbg("[INFO] log size: %u", size_left);
        len           = (size_left > LOG_BUF_SIZE) ? LOG_BUF_SIZE : size_left;  // write 1Mb once a time
        ssize_t bytes = write(fd, p, len);
        if (-1 == bytes) {
            dbg("[ERROR] failed to write file [%d][%s], %s", fd, file_name, strerror(errno));
            continue;
        }
        p += bytes;
        size_left -= bytes;
    } while (size_left != 0);

    fsync(fd);
    close(fd);
    // dbg("[INFO] log saved: %s", file_name);
}

void rotate_log_file(const char* filepath)
{
    if (NULL == filepath) return;
    char new_path[256] = { 0 };
    char old_path[256] = { 0 };
    snprintf(old_path, sizeof(old_path), "%s.%d", filepath, g_cfg._max_daily_rotation_index);
    for (int i = g_cfg._max_daily_rotation_index; i > 1; --i) {
        memcpy(new_path, old_path, sizeof(new_path));
        snprintf(old_path, sizeof(old_path), "%s.%d", filepath, i - 1);
        rename_file(old_path, new_path);
    }
    rename_file(filepath, old_path);
}

void rotate_log_line(const char* filepath)
{
    errno = 0;
    int fd = open(filepath, O_RDONLY);
    if (fd < 0) {
        dbg("[ERROR] failed to open file[%s], %s", filepath, strerror(errno));
        return;
    }

    char temp_file_path[256] = { 0 };
    snprintf(temp_file_path, sizeof(temp_file_path), "%s.tmp", filepath);
    errno = 0;
    int tmp_fd = open(temp_file_path, O_CREAT | O_RDWR | O_TRUNC, 0644);
    if (tmp_fd < 0) {
        dbg("[ERROR] failed to open file[%s], %s", filepath, strerror(errno));
        close(fd);
        return;
    }

    errno = 0;
    off_t offset = lseek(fd, get_file_size(filepath) * 0.7, SEEK_SET);  // rollover for 70%
    if (offset < 0) {
        dbg("[ERROR] failed to lseek with file[%s], %s", filepath, strerror(errno));
        close(fd);
        return;
    }

    char buf[LOG_BUF_SIZE] = { 0 };
    int bytes              = 0;
    while (bytes = read(fd, buf, sizeof(buf))) {
        write(fd, buf, bytes);
        memset(buf, 0, sizeof(buf));
    }

    fsync(tmp_fd);
    close(tmp_fd);
    close(fd);
    rename_file(temp_file_path, filepath);
    //dbg("[INFO] file rotated for %ld bytes", offset);
}

void rename_file(const char* old_path, const char* new_path)
{
    errno = 0;
    if (0 != rename(old_path, new_path)) {
        dbg("[WARN] failed to rotate log file[%s], %s", old_path, strerror(errno));
        return;
    }
    // dbg("[INFO] file renamed [%s -> %s]", old_path, new_path);
}

void archive_log_file(const char* filepath)
{
    //dbg("[INFO] %s(%s)", __FUNCTION__, filepath);
    if (NULL == filepath) return;
    // get log date according to file name
    char log_date[9] = { 0 };
    errno = 0;
    if (1 != sscanf(get_file_name(filepath), "dsv_log_%[0-9]", log_date)) {
        dbg("[ERROR] invalid file name[%s], %s", filepath, strerror(errno));
        delete_file(filepath);
        return;
    }

    // archive
    char cmd[1024] = { 0 };
    snprintf(cmd, sizeof(cmd) - 1, "tar -uf %s/dsv_log_%s.tar -C%s %s", g_cfg._dir, log_date, g_cfg._dir, get_file_name(filepath));
    errno = 0;
    int ret = system(cmd);
    if ((-1 == ret) || ((0 != WEXITSTATUS(ret)) && (1 != WEXITSTATUS(ret)))) {
        dbg("[ERROR] failed to archive log file[%s], %s", filepath, strerror(errno));
        return;
    }

    // delete
    delete_file(filepath);
}

void compress_log_file(const char* filepath)
{
    if (NULL == filepath) return;
    char log_date[9] = { 0 };
    errno = 0;
    if (1 != sscanf(get_file_name(filepath), "dsv_log_%[0-9]", log_date)) {
        dbg("[ERROR] invalid file name[%s], %s", filepath, strerror(errno));
        delete_file(filepath);
        return;
    }

    char cmd[1024] = { 0 };
    snprintf(cmd, sizeof(cmd) - 1, "tar -czf %s/dsv_log_%s.tar.gz -C%s %s", g_cfg._dir, log_date, g_cfg._dir, get_file_name(filepath));
    errno = 0;
    if (0 != system(cmd)) {
        dbg("[ERROR] failed to compress log file[%s], %s", filepath, strerror(errno));
        return;
    }

    delete_file(filepath);
}

bool copy_file(const char* src_path, const char* dest_path)
{
    //dbg("[INFO] %s(%s -> %s)", __FUNCTION__, src_path, dest_path);
    int fd = open(src_path, O_RDONLY);
    if (fd < 0) return false;

    int new_fd = open(dest_path, O_CREAT | O_RDWR | O_TRUNC, 0644);
    if (new_fd < 0) {
        close(fd);
        return false;
    }

    char buf[LOG_BUF_SIZE] = { 0 };
    int len                = 0;
    while (len = read(fd, buf, sizeof(buf))) {
        write(new_fd, buf, len);
        memset(buf, 0, sizeof(buf));
    }
    fsync(new_fd);
    close(new_fd);
    close(fd);

    // if (0 != chmod(dest_path, 0644)) {
    //     dbg("[ERROR] failed to set file mode[%s], %s", dest_path, strerror(errno));
    // }
    system("sync");
    return true;
}

bool copy_file_mm(const char* src_path, const char* dest_path)
{
   // dbg("[INFO] %s(%s -> %s)", __FUNCTION__, src_path, dest_path);
    long size = get_file_size(src_path);
    void* rbuf = map_existing_file(src_path, 0, size);
    if (NULL == rbuf) return false;

    // create new file
    int fd = open(dest_path, O_CREAT | O_RDWR, 0644);
    if (fd < 0) {
        // dbg("[ERROR] failed to create target file");
        munmap(rbuf, size);
        return false;
    }
    // ftruncate(fd, size);
    close(fd);

    bool ret = false;
    void* wbuf = map_existing_file(dest_path, 0, size);
    if (NULL != wbuf) {
        // copy
        if (NULL != memcpy(wbuf, rbuf, size)) {
            // sync
            if (0 == msync(wbuf, size, MS_SYNC)) ret = true;
        }
        munmap(wbuf, size);
    }

    munmap(rbuf, size);
    return ret;
}

bool copy_folder(const char* src_path, const char* dest_path)
{
    if ((NULL == src_path) || (NULL == dest_path)) return false;

    DIR* pddir = opendir(dest_path);
    if (NULL == pddir) {
        if (ENOENT == errno) {
            errno = 0;
            if (0 != mkdir(dest_path, 0755)) {
                dbg("[ERROR] failed to create dir[%s], %s", dest_path, strerror(errno));
                return false;
            }
        } else {
            return false;
        }
    }

    DIR* psdir = opendir(src_path);
    if (NULL == psdir) {
        closedir(pddir);
        return false;
    }

    struct dirent* pentry;
    struct stat st;
    char s_path[512] = { 0 };
    char d_path[512] = { 0 };
    while (pentry = readdir(psdir)) {
        if (0 == strncmp(pentry->d_name, ".", 1)) continue;

        memset(s_path, 0, sizeof(s_path));
        snprintf(s_path, sizeof(s_path) - 1, "%s/%s", src_path, pentry->d_name);  // full path
        errno = 0;
        int ret = stat(s_path, &st);
        if (0 != ret) {
            dbg("[ERROR] failed to get file stat[%s], %s", s_path, strerror(errno));
            continue;
        }

        memset(d_path, 0, sizeof(d_path));
        snprintf(d_path, sizeof(d_path), "%s/%s", dest_path, pentry->d_name);
        if (S_ISDIR(st.st_mode)) {
            copy_folder(s_path, d_path);
        } else if (S_ISREG(st.st_mode)) {
            copy_file(s_path, d_path);
        }
    }

    closedir(pddir);
    closedir(psdir);
}

void delete_file(const char* filepath)
{
    if (NULL == filepath) return;
    errno = 0;
    int ret = unlink(filepath);
    if (0 != ret) {
        dbg("[ERROR] failed to remove file[%s], %s", filepath, strerror(errno));
    }
}

long regex_search_the_first_occurrence_in_file(const char* file_path, const char* pattern)
{
    if ((NULL == file_path) || (NULL == pattern)) return -1L;

    long offset = -1;
    long size = get_file_size(file_path);
    char* ptr = (char*)map_existing_file(file_path, 0, size + 1); // size+1: 1 byte for tailed '\0'
    if (NULL != ptr) {
        // dbg("[INFO] %s", ptr);
        const char* loc = regex_search_the_first_occurrence_in_string(ptr, pattern);
        offset = loc - ptr;
        // dbg("[INFO] offset = %lu", offset);
        munmap(ptr, size + 1);
        truncate(file_path, size);  // remove the last null byte, added when mapping
    }
    return offset;
}

// 'src' must be null-terminated
const char* regex_search_the_first_occurrence_in_string(const char* src, const char* pattern)
{
    // dbg("[INFO] %s(%s, %s)", __FUNCTION__, src, pattern);
    if ((NULL == src) || (NULL == pattern) || (0 == strcmp(pattern, "")) || (0 == strcmp(src, ""))) return NULL;

    regex_t re;
    size_t max_subs_count = 10;
    regmatch_t subs[max_subs_count];
    char errbuf[LINE_BUF_LEN];

    int ret = regcomp(&re, pattern, REG_EXTENDED | REG_NEWLINE);
    if (0 != ret) {
        memset(errbuf, 0, LINE_BUF_LEN);
        regerror(ret, &re, errbuf, LINE_BUF_LEN);
        dbg("[ERROR] %s", errbuf);
    }

    ret = regexec(&re, src, max_subs_count, subs, 0);
    if (REG_NOMATCH == ret) {
      //  dbg("[INFO] no match");
        return NULL;
    }

    regfree(&re);
    return src + subs[0].rm_so;
}

void replace_in_file(const char* file_path, const char* from, const char* to)
{
    char pattern[256] = "";
    snprintf(pattern, sizeof(pattern), "^%s$", from);
    // dbg("[INFO] pattern = %s", pattern);
    long offset = regex_search_the_first_occurrence_in_file(file_path, pattern);
    if (-1 == offset) return;

    // create temp file
    char temp_file[256] = "";
    snprintf(temp_file, sizeof(temp_file), "/tmp/%s.tmp", get_file_name(file_path));
    int temp_fd = open(temp_file, O_CREAT | O_EXCL | O_TRUNC | O_RDWR | O_APPEND, 0644);
    if (temp_fd < 0) return;

    long file_size = get_file_size(file_path);
    size_t from_len = strlen(from);
    size_t to_len = strlen(to);

    char* p = (char*)map_existing_file(file_path, 0, file_size);
    if (NULL != p) {
        write(temp_fd, p, offset);
        write(temp_fd, to, to_len);
        write(temp_fd, p + offset + from_len, file_size - offset - from_len);
    }

    munmap(p, file_size);
    fsync(temp_fd);
    close(temp_fd);

#ifdef PC_TEST_FOR_FILE_CHECKING
    // add this definiton when you want to check if the replacement executed correctly or not
    memset(pattern, 0, sizeof(pattern));
    snprintf(pattern, sizeof(pattern), "%s.renamed", temp_file);
    rename(temp_file, pattern);
#else
    rename(temp_file, file_path);
#endif
}

void delete_all_logs()
{
    errno = 0;
    if (0 != system("rm -rf /storage/data/data/log/*.tar.gz")) {
        dbg("[ERROR] failed to remove log files, %s", strerror(errno));
    }
}

void export_all_logs(const char* dest_path)
{
    save_to_file();
    copy_folder(LOG_DIR, dest_path);
}

void log_exit()
{
    g_cfg._running = false;
    if (0UL != g_cfg._tid_cfg) {
        pthread_join(g_cfg._tid_cfg, NULL);
        g_cfg._tid_cfg = 0UL;
    }

    if (NULL != g_cfg._pmap) {
        munmap(g_cfg._pmap, MAP_BUF_SIZE);
        g_cfg._pmap = NULL;
    }

    if (NULL != g_cfg._meta_data) {
        munmap(g_cfg._meta_data, sizeof(meta_t));
        g_cfg._meta_data = NULL;
        unlink(META_FILE);
        unlink(LOG_CFG_FILE);
    }

    release_lock(&g_cfg._meta_data->_lock);
}
