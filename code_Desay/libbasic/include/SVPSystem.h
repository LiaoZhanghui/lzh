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
 * \brief Find process id by name
 * \param[in] process_name Process name
 * \param[out] process_id process ids
 * \return true if process found
 * \return false if process not exist
 */
bool SVP_Find_Process(const char* process_name, int32_t* process_id = nullptr);
