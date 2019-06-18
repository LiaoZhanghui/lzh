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

#pragma once

#include "SVPType.h"

#ifdef __cplusplus
    #include <string>
    #include <string.h>
    #include <stdarg.h>
#endif

#include <wchar.h>

#define SVP_MAX_STRING_LENGTH    2048

//String Operations
#define svp_strlen              strlen

#define svp_strcpy              strncpy
#define svp_strcat              strcat
#define svp_strcmp              strcmp

#define svp_strchr              strchr
#define svp_strrchr             strrchr
#define svp_strstr              strstr

#define svp_strlwr              strlwr
#define svp_strupr              strupr
#define svp_strtok              strtok
#define svp_strtol              strtol

#if defined(SVP_WIN_32) || defined(SVP_WIN_CE)
    #define svp_sprintf         _snprintf
    #define svp_vsnprintf       _vsnprintf
#else
    #define svp_sprintf         snprintf
    #define svp_vsnprintf       vsnprintf
#endif

#define svp_memset              memset

//WString Operations
#define svp_wcslen              wcslen

#define svp_wcsncpy             wcsncpy
#define svp_wcscat              wcscat
#define svp_wcscmp              wcscmp
#define svp_wcschr              wcschr
#define svp_wcsrchr             wcsrchr
#define svp_wcsstr              wcsstr

#define svp_wcslwr              wcslwr
#define svp_wcsupr              wcsupr
#define svp_wcstok              wcstok
#define svp_wcstol              wcstol

#if defined(SVP_WIN_32) || defined(SVP_WIN_CE)
    #define svp_snwprintf       _snwprintf
    #define svp_vsnwprintf      _vsnwprintf
#else
    #define svp_snwprintf       swprintf
    #define svp_vsnwprintf      vswprintf
#endif

#define svp_wmemset             wmemset

#ifdef __cplusplus
    std::string SVP_Format(const char* format, ...);

    std::wstring SVP_Format(const wchar_t* format, ...);

    void SVP_Replace(std::string& str, const std::string& src, const std::string& dst);

    void SVP_Replace(std::wstring& str, const std::wstring& src, const std::wstring& dst);
#endif
