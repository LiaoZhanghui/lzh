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

#include <stdlib.h>

/**
 * \file SVPEnv.h
 * \brief Sample usage of getenv().
 */

/**
 * \brief Macro to getenv, return \a defualt_value when fail
 */
#define SVP_ENV(env_name, default_value)    ({const char* env = getenv(env_name); env ? env : default_value;})

/**
 * \brief Macro to getenv \a SVP_VERSION, default value: ""
 */
#define SVP_VERSION                 SVP_ENV("SVP_VERSION",                  "")

/**
 * \brief Macro to getenv \a SVP_BUILD_DATE, default value: ""
 */
#define SVP_BUILD_DATE              SVP_ENV("SVP_BUILD_DATE",               "")

/**
 * \brief Macro to getenv \a SVP_PATH, default value: "/svp"
 */
#define SVP_PATH                    SVP_ENV("SVP_PATH",                     "/svp")

/**
 * \brief Macro to getenv \a SVP_MEDIA_PATH, default value: "/media"
 */
#define SVP_MEDIA_PATH              SVP_ENV("SVP_MEDIA_PATH",               "/media")

/**
 * \brief Macro to getenv \a SVP_STORAGE_PATH, default value: "/storage"
 */
#define SVP_STORAGE_PATH            SVP_ENV("SVP_STORAGE_PATH",             "/storage")

/**
 * \brief Macro to getenv \a SVP_BIN_PATH, default value: "/svp/bin"
 */
#define SVP_BIN_PATH                SVP_ENV("SVP_BIN_PATH",                 "/svp/bin")

/**
 * \brief Macro to getenv \a SVP_LIB_PATH, default value: "/svp/lib"
 */
#define SVP_LIB_PATH                SVP_ENV("SVP_LIB_PATH",                 "/svp/lib")

/**
 * \brief Macro to getenv \a SVP_ETC_PATH, default value: "/svp/etc"
 */
#define SVP_ETC_PATH                SVP_ENV("SVP_ETC_PATH",                 "/svp/etc")

/**
 * \brief Macro to getenv \a SVP_RES_PATH, default value: "/svp/share"
 */
#define SVP_RES_PATH                SVP_ENV("SVP_RES_PATH",                 "/svp/share")

/**
 * \brief Macro to getenv \a SVP_FONT_PATH, default value: "/svp/share/fonts"
 */
#define SVP_FONT_PATH               SVP_ENV("SVP_FONT_PATH",                "/svp/share/fonts")

/**
 * \brief Macro to getenv \a SVP_SOUND_PATH, default value: "/svp/share/sound"
 */
#define SVP_SOUND_PATH              SVP_ENV("SVP_SOUND_PATH",               "/svp/share/sound")

/**
 * \brief Macro to getenv \a SVP_TMP_PATH, default value: "/svp/run"
 */
#define SVP_TMP_PATH                SVP_ENV("SVP_TMP_PATH",                 "/tmp/run")

/**
 * \brief Macro to getenv \a SVP_TMP_STORAGE_PATH, default value: "/tmp/run/storage"
 */
#define SVP_TMP_STORAGE_PATH        SVP_ENV("SVP_TMP_STORAGE_PATH",         "/tmp/run/storage")

/**
 * \brief Macro to getenv \a SVP_TMP_DATA_PATH, default value: "/tmp/run/storage/data"
*/
#define SVP_TMP_DATA_PATH           SVP_ENV("SVP_TMP_DATA_PATH",            "/tmp/run/storage/data")

/**
 * \brief Macro to getenv \a SVP_SYS_APP_INSTALL_PATH, default value: "/storage/app"
 */
#define SVP_SYS_APP_INSTALL_PATH    SVP_ENV("SVP_SYS_APP_INSTALL_PATH",     "/storage/app")

/**
 * \brief Macro to getenv \a SVP_SYS_DATA_PATH, default value: "/storage"
 */
#define SVP_SYS_DATA_PATH           SVP_ENV("SVP_SYS_DATA_PATH",            "/storage")

/**
 * \brief Macro to getenv \a SVP_UPDATE_PATH, default value: "/storage/update"
 */
#define SVP_UPDATE_PATH             SVP_ENV("SVP_UPDATE_PATH",              "/storage/update")

/**
 * \brief Macro to getenv \a SVP_APP_PKG_PATH, default value: "/storage/update/app-pkg"
 */
#define SVP_APP_PKG_PATH            SVP_ENV("SVP_APP_PKG_PATH",             "/storage/update/app-pkg")

/**
 * \brief Macro to getenv \a SVP_RMT_PKG_FILE, default value: "/storage/update/radio.bin"
 */
#define SVP_RMT_PKG_FILE            SVP_ENV("SVP_RMT_PKG_FILE",             "/storage/update/radio.bin")

/**
 * \brief Macro to getenv \a SVP_RMT_DSP_FILE, default value: "/storage/update/dsp.bin"
 */
#define SVP_RMT_DSP_FILE            SVP_ENV("SVP_RMT_DSP_FILE",             "/storage/update/dsp.bin")

/**
 * \brief Macro to getenv \a SVP_RMT_GPS_BOOT_FILE, default value: "/storage/update/gps-boot.bin"
 */
#define SVP_RMT_GPS_BOOT_FILE       SVP_ENV("SVP_RMT_GPS_BOOT_FILE",        "/storage/update/gps-boot.bin")

/**
 * \brief Macro to getenv \a SVP_RMT_GPS_UPGRADE_FILE, default value: "/storage/update/gps-upgrade.bin"
 */
#define SVP_RMT_GPS_UPGRADE_FILE    SVP_ENV("SVP_RMT_GPS_UPGRADE_FILE",     "/storage/update/gps-upgrade.bin")

/**
 * \brief Macro to getenv \a SVP_USER_ROOT_PATH, default value: "/storage/data"
 */
#define SVP_USER_ROOT_PATH          SVP_ENV("SVP_USER_ROOT_PATH",           "/storage/data")

/**
 * \brief Macro to getenv \a SVP_USER_APP_INSTALL_PATH, default value: "/storage/data/app"
 */
#define SVP_USER_APP_INSTALL_PATH   SVP_ENV("SVP_USER_APP_INSTALL_PATH",    "/storage/data/app")

/**
 * \brief Macro to getenv \a SVP_USER_DATA_PATH, default value: "/storage/data/data"
 */
#define SVP_USER_DATA_PATH          SVP_ENV("SVP_USER_DATA_PATH",           "/storage/data/data")

/**
 * \brief Macro to getenv \a SVP_USER_ETC_PATH, default value: "/storage/data/etc"
 */
#define SVP_USER_ETC_PATH           SVP_ENV("SVP_USER_ETC_PATH",            "/storage/data/etc")
