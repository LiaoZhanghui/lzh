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

#include "SVPUtils.h"
#include "SVPLog.h"

#include <execinfo.h>

void svp_print_backtrace() {
    SVP_DEBUG("--------------------------------------------------------------------------------");
    SVP_DEBUG("Back Trace:");
    SVP_DEBUG("--------------------------------------------------------------------------------");
    void* array[10] = {0};
    size_t size = backtrace(array, 10);
    char** strNames = (char**)backtrace_symbols(array, size);

    for (size_t i = 0; i < size; i++) {
        SVP_DEBUG("%02d:%s", i, strNames[i]);
    }

    SVP_DEBUG("--------------------------------------------------------------------------------");
}
