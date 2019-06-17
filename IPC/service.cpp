
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/time.h>
#include <locale.h>
#include <mutex>
#include <thread>
#include <sys/msg.h>
#include <errno.h>
#include "log.h"
#include "ipc.h"

#ifdef SVP_LOG_TAG
    #undef SVP_LOG_TAG
#endif
#define SVP_LOG_TAG     "Service"

using namespace std;

void HandleOfMsg(long int msgType)
{
    switch(msgType)
    {
        case 200:
        {
            
        }
        break;
        default:
        break;
    }
}

void MSG_CreatAndRcv( )
{
    int msgid = -1;
    msgid = msgget((key_t)VR_MSG_KEY, 0666 | IPC_CREAT);
    if (msgid == -1)
    {
        SVP_ERROR("msgget failed errno: %d", errno);
        return;
    }
    
    SVP_INFO("msgget success, msgid is %d", msgid);

    VR_Msg msgData;
    while (1)
    {
        if (msgrcv(msgid, (void *)&msgData, sizeof(msgData)-sizeof(long int), 0, 0) == -1)
        {
            SVP_ERROR("msg rcv failed width erro: %d", errno);
            break;
        }
        SVP_INFO("receive msg, message type is %ld", msgData.msg_type);
        HandleOfMsg(msgData.msg_type);
    }

    return;
}

int main(int argc, char **argv)
{
    MSG_CreatAndRcv();
}