/******************************************************************************
*
* Company       : Huizhou Desay SV Automotive Co., Ltd.
*
* Division      : Automotive Electronics, Desay Corporation
*
* Business Unit : Central Technology
*
* Department    : Advanced Development (Huizhou)
*
******************************************************************************/

#include "SVPSystem.h"
#include "SVPString.h"

#include "SVPLog.h"

#ifdef SVP_LOG_TAG
    #undef SVP_LOG_TAG
#endif
#define SVP_LOG_TAG     "libbasic|system"

#if !defined(SVP_WIN_32) && !defined(SVP_WIN_CE)
    #include <dirent.h>
#endif

bool SVP_Find_Process(const char* process_name, int32_t* process_id) {
    bool process_found = false;

    if (process_id)
        *process_id = 0;

#if !defined(SVP_WIN_32) && !defined(SVP_WIN_CE)
    // Open the /proc directory.
    DIR* proc_dir = opendir("/proc");

    if (!proc_dir) {
        SVP_ERROR("SVP_Find_Process() - Cannot open \"/proc\"");
        return false;
    }

    int32_t process_name_len = strlen(process_name);
    struct dirent* proc_dirent = NULL;

    // Walk through the directory.
    while ((proc_dirent = readdir(proc_dir)) != NULL) {
        int32_t pid = atoi(proc_dirent->d_name);

        if (pid == 0)   // Not a pid
            continue;

        char exe [SVP_MAX_PATH] = {0};
        snprintf(exe, SVP_MAX_PATH, "/proc/%s/exe", proc_dirent->d_name);
        char path[SVP_MAX_PATH + 1] = {0};
        int32_t len = readlink(exe, path, SVP_MAX_PATH);

        if (len <= 0)
            continue;

        path[len] = '\0';
        // Find process name
        char* s = strrchr(path, '/');

        if (s == NULL)
            continue;

        s++;

        if (strlen(s) != process_name_len)
            continue;

        if (strncmp(process_name, s, process_name_len) == 0) {
            if (process_id)
                *process_id = pid;

            process_found = true;
            break;
        }
    }

    closedir(proc_dir);
#endif
    return process_found;
}

