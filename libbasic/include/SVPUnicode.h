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
#include "SVPString.h"

/**
 * \brief Cross platform unicode coversion class
 */

namespace SVPUnicode {
#if defined(SVP_WIN_32) || defined(SVP_WIN_CE)
    /**
    * \brief ANSI to UTF16 conversion
    * \param[in] pUTF16 Buffer for coversion result in UTF16
    * \param[in] pANSI ANSI string for conversion
    * \param[in] iLen Buffer size
    */
    int32_t ANSItoUTF16(uint16_t* pUTF16, const uint8_t* pANSI, int32_t iLen = SVP_MAX_STRING_LENGTH - 1);
    /**
    * \brief UTF16 to ANSI conversion
    * \param[in] pANSI Buffer for coversion result in ANSI
    * \param[in] pUTF16 UTF16 string for conversion
    * \param[in] iLen Buffer size
    */
    int32_t UTF16toANSI(uint8_t* pANSI, const uint16_t* pUTF16, int32_t iLen = SVP_MAX_STRING_LENGTH - 1);

    /**
    * \brief UTF8 to UTF16 conversion
    * \param[in] pUTF16 Buffer for coversion result in UTF16
    * \param[in] pUTF8 UTF8 string for conversion
    * \param[in] iLen Buffer size
    */
    int32_t UTF8toUTF16(uint16_t* pUTF16, const uint8_t* pUTF8, int32_t iLen = SVP_MAX_STRING_LENGTH - 1);

    /**
    * \brief UTF16 to UTF8 conversion
    * \param[in] pUTF8 Buffer for coversion result in UTF8
    * \param[in] pUTF16 UTF16 string for conversion
    * \param[in] iLen Buffer size
    */
    int32_t UTF16toUTF8(uint8_t* pUTF8, const uint16_t* pUTF16, int32_t iLen = SVP_MAX_STRING_LENGTH - 1);

#else
    /**
    * \brief UTF8 to UTF16 conversion
    * \param[in] pUTF16 Buffer for coversion result in UTF16
    * \param[in] pUTF8 UTF8 string for conversion
    * \param[in] iLen Buffer size
    */
    int32_t UTF8toUTF16(uint16_t* pUTF16, const uint8_t* pUTF8, int32_t iLen = SVP_MAX_STRING_LENGTH - 1);

    /**
    * \brief UTF16 to UTF8 conversion
    * \param[in] pUTF8 Buffer for coversion result in UTF8
    * \param[in] pUTF16 UTF16 string for conversion
    * \param[in] iLen Buffer size
    */
    int32_t UTF16toUTF8(uint8_t* pUTF8, const uint16_t* pUTF16, int32_t iLen = SVP_MAX_STRING_LENGTH - 1);

    /**
    * \brief UTF8 to UTF32 conversion
    * \param[in] pUTF32 Buffer for coversion result in UTF32
    * \param[in] pUTF8 UTF8 string for conversion
    * \param[in] iLen Buffer size
    */
    int32_t UTF8toUTF32(uint32_t* pUTF32, const uint8_t* pUTF8, int32_t iLen = SVP_MAX_STRING_LENGTH - 1);

    /**
    * \brief UTF32 to UTF8 conversion
    * \param[in] pUTF8 Buffer for coversion result in UTF8
    * \param[in] pUTF32 UTF32 string for conversion
    * \param[in] iLen Buffer size
    */
    int32_t UTF32toUTF8(uint8_t* pUTF8, const uint32_t* pUTF32, int32_t iLen = SVP_MAX_STRING_LENGTH - 1);

    /**
    * \brief UTF16 to UTF32 conversion
    * \param[in] pUTF32 Buffer for coversion result in UTF32
    * \param[in] pUTF16 UTF16 string for conversion
    * \param[in] iLen Buffer size
    */
    int32_t UTF16toUTF32(uint32_t* pUTF32, const uint16_t* pUTF16, int32_t iLen = SVP_MAX_STRING_LENGTH - 1);

    /**
    * \brief UTF32 to UTF16 conversion
    * \param[in] pUTF16 Buffer for coversion result in UTF16
    * \param[in] pUTF32 UTF32 string for conversion
    * \param[in] iLen Buffer size
    */
    int32_t UTF32toUTF16(uint16_t* pUTF16, const uint32_t* pUTF32, int32_t iLen = SVP_MAX_STRING_LENGTH - 1);
#endif
};
