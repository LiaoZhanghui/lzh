
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
#define SVP_LOG_TAG     "Client"

using namespace std;

void PrintNotes()
{
    SVP_WARN("missing parameter or wrong parameter");
    SVP_PRINT("***********************notes...***********************");
    SVP_PRINT(" please enter message type(numbers) parameter");
    SVP_PRINT("*********************notes end ***********************");
}

void MSG_CreatAndSend(long int msgType)
{
    int msgid = -1;
    msgid = msgget((key_t)VR_MSG_KEY, 0666 | IPC_CREAT);
    if (msgid == -1)
    {
        SVP_ERROR("msgget failed errno: %d", errno);
        return;
    }

    VR_Msg msg;
    msg.msg_type = msgType;
    msg.msg_param = 0;
    SVP_INFO("send msgtype is %ld, param is %d", msg.msg_type, msg.msg_param);
    if (msgsnd(msgid, (void *)&msg, sizeof(VR_Msg)-sizeof(long int), 0) == -1)
    {
        SVP_ERROR("[ERROR] msgsnd failed");
    }

    //msgctl(msgid, IPC_RMID, 0);
}



int main(int argc, char **argv)
{
    if (argc <= 1)
    {
        PrintNotes();
    }
    else
    {
        const char * szParam = argv[1];

        int type;
        try 
        {
            type = std::stoi(szParam);
        }
        catch (...)
        {
            SVP_ERROR("input error!");
            PrintNotes();
            return 0;
        }

        MSG_CreatAndSend(type);
    }

    return 0;
}