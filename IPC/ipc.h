#ifndef __IPC_H__
#define __IPC_H__

#define VR_MSG_KEY 11551

typedef struct msg_st
{
    long int msg_type;
    int32_t  msg_param;
}VR_Msg;

#endif //__IPC_H__