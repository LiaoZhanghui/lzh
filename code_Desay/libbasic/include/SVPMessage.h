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

typedef int32_t (*tMsgProc)(SVP_HANDLE mq, uint32_t id, uint32_t wparam, int32_t lparam);

namespace SVPMessage {
SVP_HANDLE createMsg(int32_t mqid, tMsgProc msgProc);

SVP_HANDLE findMsg(int32_t mqid, int32_t timeout = 3000);

bool postMsg(SVP_HANDLE mq, uint32_t id, uint32_t wparam = 0, int32_t lparam = 0);

bool postMsgEx(int32_t mqid, uint32_t id, uint32_t wparam = 0, int32_t lparam = 0);

bool destroyMsg(SVP_HANDLE mq);
};
