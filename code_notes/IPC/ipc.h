#ifndef __IPC_H__
#define __IPC_H__

#define VR_MSG_KEY 11551

typedef struct msg_st
{
    long int msg_type;
    int32_t  msg_param;
}VR_Msg;

typedef enum msgtype
{
    MSG_OPENLOG = 0,
    MSG_CLOSELOG,
} msgType;

#endif //__IPC_H__