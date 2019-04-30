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

/**
 * \brief Cross platform file operation class
 */

class SVPFile {
  public:
    /**
    * \brief Check wether the path is a file
    * \param[in] pPath Path to check
    * \return true if path is a file
    * \return false if path is not a file
    */
    static bool isFile(const char* pPath);

    /**
     * \brief Check wether the path is a directory
     * \param[in] pPath Path to check
     * \return true if path is a directory
     * \return false if path is not a directory
     */
    static bool isDirectory(const char* pPath);

    /**
     * \brief Check wether the file/directory is exist
     * \param[in] pFilePath Path to check
     * \return true if file/directory is exist
     * \return false if file/directory is not exist
     */
    static bool fileExist(const char* pFilePath);

    /**
     * \brief File delete
     * \param[in] pFilePath File/Directory to delete
     * \return true if File delete successfull
     * \return false if File delete fail
     */
    static bool fileDelete(const char* pFilePath);
};
