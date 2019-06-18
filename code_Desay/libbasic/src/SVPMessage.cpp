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

#include "SVPMessage.h"

#include "SVPPlatform.h"
#include "SVPTime.h"
#include "Poco/ThreadPool.h"
#include "Poco/Runnable.h"

#include <string.h>
#include <errno.h>
#include <sys/msg.h>

#define SVP_NO_INFO

#include "SVPLog.h"

#ifdef SVP_LOG_TAG
    #undef SVP_LOG_TAG
#endif
#define SVP_LOG_TAG        "libbasic|message"

namespace SVPMessage {

#define MAXMSGFINDTIME  3000
#define EXITMSGID 0

struct tMsg {
    unsigned int id;
    unsigned int wparam;
    long lparam;
};

class MsgWorker: public Poco::Runnable {
  public:
    MsgWorker(SVP_HANDLE mq, tMsgProc mproc)
        :  m_mq(mq), m_mproc(mproc) {}

    virtual void run() {
        while (true) {
            tMsg _msg;
            if (msgrcv(m_mq, (void*)&_msg, sizeof(tMsg), 0, 0) != -1) {
                if (EXITMSGID != _msg.id) {
//                    SVP_DEBUG("MsgWorker(%d) msgrcv: 0x%08x", m_mq, _msg.id);
                    m_mproc(m_mq, _msg.id, _msg.wparam, _msg.lparam);
                } else {
//                    SVP_DEBUG("MsgWorker(%d) exited.", m_mq);
                    break;
                }
            } else {
                SVP_ERROR("MsgWorker(%d) error: %s", m_mq, strerror(errno));
                break;
            }
        }
    }

  private:
    SVP_HANDLE m_mq;
    tMsgProc   m_mproc;
};

static std::map<SVP_HANDLE, std::pair<Poco::Thread*, MsgWorker*>> gs_Msgs;

SVP_HANDLE createMsg(int32_t mqid, tMsgProc mproc) {
    SVP_HANDLE _mq = msgget((key_t)mqid, 0666 | IPC_CREAT);
    if (_mq == -1) {
        SVP_ERROR("Msg(%ld) failed with error: %s", mqid, strerror(errno));
        return _mq;
    }

    if (gs_Msgs.find(_mq) != gs_Msgs.end()) {
        SVP_WARN("Msg(%ld) exist.", mqid);
        return _mq;
    }

//    SVP_DEBUG("Msg(%ld) created.", mqid);
    auto _ctx = std::make_pair(new Poco::Thread(), new MsgWorker(_mq, mproc));
    _ctx.first->setPriority(Poco::Thread::PRIO_HIGHEST);
    _ctx.first->start(*_ctx.second);
    gs_Msgs[_mq] = _ctx;
    return _mq;
}

SVP_HANDLE findMsg(int32_t mqid, int32_t timeout) {
    SVP_HANDLE _mq = -1;
    do {
        _mq = msgget((key_t)mqid, 0666);
        if (_mq == -1) {
            SVP_Sleep(50);
            timeout -= 50;
        }
    } while (_mq == -1 && timeout > 0);

    return _mq;
}

bool postMsg(SVP_HANDLE mq, uint32_t id, uint32_t wparam, int32_t lparam) {
    if (mq == -1) {
        // SVP_WARN("postMsg(%d) invalid mq, msg: 0x%08X", mq, id);
        return false;
    }

    tMsg _msg;
    _msg.id = id;
    _msg.wparam = wparam;
    _msg.lparam = lparam;

    if (msgsnd(mq, (void*)&_msg, sizeof(tMsg), 0) != -1) {
        return true;
    } else {
        SVP_ERROR("postMsg(%d) failed with error: %s", mq, strerror(errno));
        return false;
    }
}

bool postMsgEx(int32_t mqid, uint32_t id, uint32_t wparam, int32_t lparam) {
//    SVP_DEBUG("PostMsgEx( %-2d | msg: 0x%08X w: %-4u l: %-4ld )", mqid, id, wparam, lparam);
    return postMsg(findMsg(mqid, 0), id, wparam, lparam);
}

bool destroyMsg(SVP_HANDLE mq) {
    if (mq == -1) {
        SVP_ERROR("destroyMsg(%d) invalid mq!", mq);
        return false;
    }

    postMsg(mq, EXITMSGID);
    auto itMsg = gs_Msgs.find(mq);
    if (itMsg == gs_Msgs.end()) {
        SVP_WARN("destroyMsg(%d) mq not exist.", mq)
        return true;
    }

    itMsg->second.first->join();
    delete itMsg->second.first;
    delete itMsg->second.second;
    gs_Msgs.erase(itMsg);

    if (msgctl(mq, IPC_RMID, 0) == -1) {
        SVP_ERROR("destroyMsg(%d) msgctl(IPC_RMID) failed!", mq);
    }

    return true;
}

}; // namespace SVPMessage
