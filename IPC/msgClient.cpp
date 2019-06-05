
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

#define PRINT(argfmt, ...) fprintf(stderr, "VRTEST:" argfmt "\r\n", ##__VA_ARGS__);
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
        PRINT("[ERROR] msgget failed errno: %d", errno);
        return;
    }

    VR_Msg msg;
    msg.msg_type = msgType;
    msg.msg_param = 0;
    if (msgsnd(msgid, (void *)&msg, sizeof(VR_Msg)-sizeof(long int), 0) == -1)
    {
        PRINT("[ERROR] msgsnd failed");
    }
    else
    {
        PRINT("send msg[%ld] succeed", msgType);
    }

    //msgctl(msgid, IPC_RMID, 0);
}

void PrintNotes()
{
    PRINT("******************VR TEST: notes...******************");
    PRINT("* You can use the following parameters:");
    PRINT("* missing parameter, please enter the message type");
    PRINT("******************VR TEST: end **********************");
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
            PRINT("input error!");
            PrintNotes();
            return 0;
        }

        PRINT("type is %d", type);
        MSG_CreatAndSend(type);
    }
    return 0;
}