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
 * \brief signal no to string
 * \param[in] signo Signal No.
 * \return Signal name in string
 */
const char* svp_sig_to_str(int32_t signo);

/**
 * \brief print last 10 backtraces
 */
void svp_print_backtrace();
