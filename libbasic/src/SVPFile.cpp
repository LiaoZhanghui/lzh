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

#include "SVPFile.h"
#include "SVPString.h"
#include "SVPLimits.h"

#include "SVPLog.h"

#ifdef SVP_LOG_TAG
    #undef SVP_LOG_TAG
#endif
#define SVP_LOG_TAG     "libbasic|file"

#if defined(WIN32) || defined(_WIN32_WCE)
#else
    #include <sys/stat.h>
    #include <unistd.h>
    #include <fcntl.h>
    #include <dirent.h>
#endif

bool SVPFile::isFile(const char* pPath) {
#if defined(SVP_WIN_32) || defined(SVP_WIN_CE)
    // To be implemented
#else
    struct stat statbuf;

    if (lstat(pPath, &statbuf) == 0) {
        return (S_ISREG(statbuf.st_mode) != 0); //判断文件是否为常规文件
    }

#endif
    return false;
}

bool SVPFile::isDirectory(const char* pPath) {
#if defined(SVP_WIN_32) || defined(SVP_WIN_CE)
    // To be implemented
#else
    struct stat statbuf;

    if (lstat(pPath, &statbuf) == 0) {
        return (S_ISDIR(statbuf.st_mode) != 0);//S_ISDIR宏，判断文件类型是否为目录
    }

#endif
    return false;
}

bool SVPFile::fileExist(const char* pFilePath) {
#if defined(SVP_WIN_32) || defined(SVP_WIN_CE)
    WIN32_FIND_DATA fd;
    HANDLE hFile = FindFirstFile(pFilePath, &fd);

    if (hFile == SVP_INVALID_HANDLE_VALUE)
        return false;

    FindClose(hFile);
    return true;
#else

    if (access(pFilePath, F_OK) == 0)
        return true;
    else
        return false;

#endif
}

bool SVPFile::fileDelete(const char* pFilePath) {
#if defined(SVP_WIN_32) || defined(SVP_WIN_CE)
    // To be implemented
#else

    if (isFile(pFilePath)) {
        if (remove(pFilePath) == 0)
            return true;
        else
            return false;
    }

    if (isDirectory(pFilePath)) {
        DIR* dir = NULL;

        if ((dir = opendir(pFilePath)) == NULL)
            return false;

        dirent* dir_info = NULL;
        char file_path[SVP_MAX_PATH] = {0};

        while ((dir_info = readdir(dir)) != NULL) {
            if (svp_strcmp(dir_info->d_name, ".") == 0 || svp_strcmp(dir_info->d_name, "..") == 0)
                continue;

            memset(file_path, 0, SVP_MAX_PATH);

            if (pFilePath[strlen(pFilePath) - 1] != '/')
                svp_sprintf(file_path, SVP_MAX_PATH, "%s/%s", pFilePath, dir_info->d_name);
            else
                svp_sprintf(file_path, SVP_MAX_PATH, "%s%s", pFilePath, dir_info->d_name);

            if (!fileDelete(file_path)) {
                closedir(dir);
                return false;
            }
        }

        closedir(dir);

        if (rmdir(pFilePath) == 0) {
            return true;
        }
    }

#endif
    return false;
}
