
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

using namespace std;

#define SERVICE_PRINT(argfmt, ...) fprintf(stderr, "SERVICE:" argfmt "\r\n", ##__VA_ARGS__);
#define VR_MSG_KEY 11551


typedef struct msg_st
{
    long int msg_type;
    int32_t  msg_param;
}VR_Msg;

void MSG_CreatAndSend(long int msgType)
{
    int msgid = -1;
    msgid = msgget((key_t)VR_MSG_KEY, 0666 | IPC_CREAT);
    if (msgid == -1)
    {
        SERVICE_PRINT("[ERROR] msgget failed errno: %d", errno);
        return;
    }

    VR_Msg msg;
    msg.msg_type = msgType;
    msg.msg_param = 0;
    if (msgsnd(msgid, (void *)&msg, sizeof(VR_Msg)-sizeof(long int), 0) == -1)
    {
        SERVICE_PRINT("[ERROR] msgsnd failed");
    }
    else
    {
        SERVICE_PRINT("send msg[%ld] succeed", msgType);
    }

    //msgctl(msgid, IPC_RMID, 0);
}

void PrintNotes()
{
    SERVICE_PRINT("******************VR TEST: notes...******************");
    SERVICE_PRINT("* You can use the following parameters:");
    SERVICE_PRINT("* openlog, closelog");
    SERVICE_PRINT("******************VR TEST: end **********************");
}

int main(int argc, char **argv)
{
    int msgid = -1;
    msgid = msgget((key_t)VR_MSG_KEY, 0666 | IPC_CREAT);
    if (msgid == -1)
    {
        SERVICE_PRINT("[ERROR] msgget failed errno: %d", errno);
        return 0;
    }

    VR_Msg msgData;
    while (1)
    {
        if (msgrcv(msgid, (void *)&msgData, sizeof(msgData)-sizeof(long int), 0, 0) == -1)
        {
            SERVICE_PRINT("msg rcv failed width erro: %d", errno);
            break;
        }

        SERVICE_PRINT("receive msg, message type is %ld", msgData.msg_type);
    }
}