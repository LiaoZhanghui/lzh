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

#include "SVPString.h"

std::string SVP_Format(const char* format, ...) {
    char strBuffer[SVP_MAX_STRING_LENGTH + 1];
    svp_memset(strBuffer, 0, SVP_MAX_STRING_LENGTH + 1);
    va_list pArgList;
    va_start(pArgList, format);
    svp_vsnprintf(strBuffer, SVP_MAX_STRING_LENGTH, format, pArgList);
    va_end(pArgList);
    return std::string(strBuffer);
}

std::wstring SVP_Format(const wchar_t* format, ...) {
    wchar_t strBuffer[SVP_MAX_STRING_LENGTH + 1];
    svp_wmemset(strBuffer, 0, SVP_MAX_STRING_LENGTH + 1);
    va_list pArgList;
    va_start(pArgList, format);
    svp_vsnwprintf(strBuffer, SVP_MAX_STRING_LENGTH, format, pArgList);
    va_end(pArgList);
    return std::wstring(strBuffer);
}


void SVP_Replace(std::string& str, const std::string& src, const std::string& dst) {
    std::string::size_type pos = 0;

    while ( (pos = str.find(src, pos)) != std::string::npos) {
        str.replace(pos, src.length(), dst);
        pos += dst.length();
    }
}

void SVP_Replace(std::wstring& str, const std::wstring& src, const std::wstring& dst) {
    std::wstring::size_type pos = 0;

    while ( (pos = str.find(src, pos)) != std::wstring::npos) {
        str.replace(pos, src.length(), dst);
        pos += dst.length();
    }
}

