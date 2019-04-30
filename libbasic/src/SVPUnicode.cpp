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

#include "SVPUnicode.h"

#if !defined(SVP_WIN_32) && !defined(SVP_WIN_CE)
    #include "utf.h"
#endif

#if defined(SVP_WIN_32) || defined(SVP_WIN_CE)
int32_t SVPUnicode::ANSItoUTF16(uint16_t* pUTF16, const uint8_t* pANSI, int32_t iLen) {
    int32_t len = MultiByteToWideChar(CP_ACP, 0, pANSI, -1, NULL, 0);

    if (len >= iLen)
        len = iLen - 1;

    if (len >= SVP_MAX_STRING_LENGTH)
        len = SVP_MAX_STRING_LENGTH - 1;

    return MultiByteToWideChar(CP_ACP, 0, pANSI, -1, pUTF16, len);
}

int32_t SVPUnicode::UTF16toANSI(uint8_t* pANSI, const uint16_t* pUTF16, int32_t iLen) {
    uint32_t len = WideCharToMultiByte(CP_ACP, 0, pUTF16, -1, NULL, 0, 0, 0);

    if (len >= iLen)
        len = iLen - 1;

    if (len >= SVP_MAX_STRING_LENGTH)
        len = SVP_MAX_STRING_LENGTH - 1;

    return WideCharToMultiByte(CP_ACP, 0, pUTF16, -1, pANSI, len, 0, 0);
}

int32_t SVPUnicode::UTF8toUTF16(uint16_t* pUTF16, const uint8_t* pUTF8, int32_t iLen) {
    int32_t len = MultiByteToWideChar(CP_UTF8, 0, pUTF8, -1, NULL, 0);

    if (len >= iLen)
        len = iLen - 1;

    if (len >= SVP_MAX_STRING_LENGTH)
        len = SVP_MAX_STRING_LENGTH - 1;

    return MultiByteToWideChar(CP_UTF8, 0, pUTF8, -1, pUTF16, len);
}

int32_t SVPUnicode::UTF16toUTF8(uint8_t* pUTF8, const uint16_t* pUTF16, int32_t iLen) {
    uint32_t len = WideCharToMultiByte(CP_UTF8, 0, pUTF16, -1, NULL, 0, 0, 0);

    if (len >= iLen)
        len = iLen - 1;

    if (len >= SVP_MAX_STRING_LENGTH)
        len = SVP_MAX_STRING_LENGTH - 1;

    return WideCharToMultiByte(CP_UTF8, 0, pUTF16, -1, pUTF8, len, 0, 0);
}

#else
int32_t SVPUnicode::UTF8toUTF16(uint16_t* pUTF16, const uint8_t* pUTF8, int32_t iLen) {
    const UTF8** sourceStart    = (const UTF8**)(&pUTF8);
    const UTF8* sourceEnd       = (const UTF8*)(pUTF8 + iLen);
    UTF16** targetStart         = (UTF16**)(&pUTF16);
    UTF16* targetEnd            = (UTF16*)(pUTF16 + SVP_MAX_STRING_LENGTH - 1);
    return ConvertUTF8toUTF16(sourceStart, sourceEnd, targetStart, targetEnd, lenientConversion);
}

int32_t SVPUnicode::UTF16toUTF8(uint8_t* pUTF8, const uint16_t* pUTF16, int32_t iLen) {
    const UTF16** sourceStart   = (const UTF16**)(&pUTF16);
    const UTF16* sourceEnd      = (const UTF16*)(pUTF16 + iLen);
    UTF8** targetStart          = (UTF8**)(&pUTF8);
    UTF8* targetEnd             = (UTF8*)(pUTF8 + SVP_MAX_STRING_LENGTH - 1);
    return ConvertUTF16toUTF8(sourceStart, sourceEnd, targetStart, targetEnd, lenientConversion);
}

int32_t SVPUnicode::UTF8toUTF32(uint32_t* pUTF32, const uint8_t* pUTF8, int32_t iLen) {
    const UTF8** sourceStart    = (const UTF8**)(&pUTF8);
    const UTF8* sourceEnd       = (const UTF8*)(pUTF8 + iLen);
    UTF32** targetStart         = (UTF32**)(&pUTF32);
    UTF32* targetEnd            = (UTF32*)(pUTF32 + SVP_MAX_STRING_LENGTH - 1);
    return ConvertUTF8toUTF32(sourceStart, sourceEnd, targetStart, targetEnd, lenientConversion);
}

int32_t SVPUnicode::UTF32toUTF8(uint8_t* pUTF8, const uint32_t* pUTF32, int32_t iLen) {
    const UTF32** sourceStart   = (const UTF32**)(&pUTF32);
    const UTF32* sourceEnd      = (const UTF32*)(pUTF32 + iLen);
    UTF8** targetStart          = (UTF8**)(&pUTF8);
    UTF8* targetEnd             = (UTF8*)(pUTF8 + SVP_MAX_STRING_LENGTH - 1);
    return ConvertUTF32toUTF8(sourceStart, sourceEnd, targetStart, targetEnd, lenientConversion);
}

int32_t SVPUnicode::UTF16toUTF32(uint32_t* pUTF32, const uint16_t* pUTF16, int32_t iLen) {
    const UTF16** sourceStart   = (const UTF16**)(&pUTF16);
    const UTF16* sourceEnd      = (const UTF16*)(pUTF16 + iLen);
    UTF32** targetStart         = (UTF32**)(&pUTF32);
    UTF32* targetEnd            = (UTF32*)(pUTF32 + SVP_MAX_STRING_LENGTH - 1);
    return ConvertUTF16toUTF32(sourceStart, sourceEnd, targetStart, targetEnd, lenientConversion);
}

int32_t SVPUnicode::UTF32toUTF16(uint16_t* pUTF16, const uint32_t* pUTF32, int32_t iLen) {
    const UTF32** sourceStart   = (const UTF32**)(&pUTF32);
    const UTF32* sourceEnd      = (const UTF32*)(pUTF32 + iLen);
    UTF16** targetStart         = (UTF16**)(&pUTF16);
    UTF16* targetEnd            = (UTF16*)(pUTF16 + SVP_MAX_STRING_LENGTH - 1);
    return ConvertUTF32toUTF16(sourceStart, sourceEnd, targetStart, targetEnd, lenientConversion);
}
#endif
