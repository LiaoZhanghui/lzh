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

#include <signal.h>

struct sig_name {
    int32_t     signo;  // signal number
    const char* signm;  // singal name
};

sig_name sig_name_table[] = {
    {SIGHUP,        "SIGHUP"},
    {SIGINT,        "SIGINT"},
    {SIGQUIT,       "SIGQUIT"},
    {SIGILL,        "SIGILL"},
    {SIGTRAP,       "SIGTRAP"},
    {SIGABRT,       "SIGABRT"},
    {SIGIOT,        "SIGIOT"},
    {SIGBUS,        "SIGBUS"},
    {SIGFPE,        "SIGFPE"},
    {SIGKILL,       "SIGKILL"},
    {SIGUSR1,       "SIGUSR1"},
    {SIGSEGV,       "SIGSEGV"},
    {SIGUSR2,       "SIGUSR2"},
    {SIGPIPE,       "SIGPIPE"},
    {SIGALRM,       "SIGALRM"},
    {SIGTERM,       "SIGTERM"},
    {SIGSTKFLT,     "SIGSTKFLT"},
    {SIGCHLD,       "SIGCHLD"},
    {SIGCONT,       "SIGCONT"},
    {SIGSTOP,       "SIGSTOP"},
    {SIGTSTP,       "SIGTSTP"},
    {SIGTTIN,       "SIGTTIN"},
    {SIGTTOU,       "SIGTTOU"},
    {SIGURG,        "SIGURG"},
    {SIGXCPU,       "SIGXCPU"},
    {SIGXFSZ,       "SIGXFSZ"},
    {SIGVTALRM,     "SIGVTALRM"},
    {SIGPROF,       "SIGPROF"},
    {SIGWINCH,      "SIGWINCH"},
    {SIGIO,         "SIGIO|SIGPOLL"}
};

int32_t bin_search_signo(const sig_name* array, int32_t start, int32_t end, int32_t signo) {
    int32_t left = start;
    int32_t right = end;

    while (left <= right) {
        int32_t mid = (left + right) / 2;

        if (signo < array[mid].signo) {
            right = mid - 1;
        } else if (signo > array[mid].signo) {
            left = mid + 1;
        } else
            return mid;
    }

    return -1;
}

const char* svp_sig_to_str(int32_t signo) {
    int32_t sig_idx = bin_search_signo(sig_name_table, 0, sizeof(sig_name_table) / sizeof(sig_name) - 1, signo);

    if (sig_idx != -1)
        return sig_name_table[sig_idx].signm;
    else
        return "SIGUNKOWN";
}
