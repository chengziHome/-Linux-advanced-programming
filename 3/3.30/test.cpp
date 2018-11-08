#include <iostream>
#include "CLThread.h"
#include "CLExecutiveFunctionProvider.h"
#include "CLMessageQueueBySTLqueue.h"
#include "CLMessage.h"
#include "CLMsgLoopManagerForSTLqueue.h"
#include "CLMessageObserver.h"

using namespace std;

#define ADD_MSG 0
#define QUIT_MSG 1

class CLMyMsgProcessor;

/**
 *
 * 这一版本说是将消息的两个属性解偶开来：
 *      消息从何而来
 *      消息如何处理
 */

class CLAddMessage : public CLMessage {
public:
    friend class CLMyMsgProcessor;

    CLAddMessage(int Op1, int Op2) : CLMessage(ADD_MSG) {
        m_Op1 = Op1;
        m_Op2 = Op2;
    }

    virtual ~CLAddMessage() {
    }

private:
    int m_Op1;
    int m_Op2;
};

class CLQuitMessage : public CLMessage {
public:
    CLQuitMessage() : CLMessage(QUIT_MSG) {
    }

    virtual ~CLQuitMessage() {
    }
};

class CLMyMsgProcessor : public CLMessageObserver {
public:
    CLMyMsgProcessor() {
    }

    virtual ~CLMyMsgProcessor() {
    }

    virtual CLStatus Initialize(void *pContext) {
        return CLStatus(0, 0);
    }

    virtual CLStatus DispatchMessage(CLMessage *pM) {
        CLAddMessage *pAddMsg;
        switch (pM->m_clMsgID) {
            case ADD_MSG:
                pAddMsg = (CLAddMessage *) pM;
                cout << pAddMsg->m_Op1 + pAddMsg->m_Op2 << endl;
                break;

            case QUIT_MSG:
                cout << "quit..." << endl;
                return CLStatus(QUIT_MESSAGE_LOOP, 0);

            default:
                break;
        }

        return CLStatus(0, 0);
    }
};

class CLAdder : public CLExecutiveFunctionProvider {
    CLMessageLoopManager *m_pMsgLoopManager;

public:
    CLAdder(CLMessageLoopManager *pMsgLoopManager) {
        m_pMsgLoopManager = pMsgLoopManager;
    }

    virtual ~CLAdder() {
        if (m_pMsgLoopManager != 0)
            delete m_pMsgLoopManager;
    }

    virtual CLStatus RunExecutiveFunction(void *pContext) {
        return m_pMsgLoopManager->EnterMessageLoop(pContext);
    }
};

int main() {
    CLMessageQueueBySTLqueue *pQ = new CLMessageQueueBySTLqueue();

    CLThread *t = new CLThread(new CLAdder(new CLMsgLoopManagerForSTLqueue(new CLMyMsgProcessor, pQ)), true);
    t->Run(0);

    CLAddMessage *paddmsg = new CLAddMessage(2, 4);
    pQ->PushMessage(paddmsg);

    CLAddMessage *paddmsg1 = new CLAddMessage(3, 6);
    pQ->PushMessage(paddmsg1);

    CLAddMessage *paddmsg2 = new CLAddMessage(5, 6);
    pQ->PushMessage(paddmsg2);

    CLQuitMessage *pquitmsg = new CLQuitMessage();
    pQ->PushMessage(pquitmsg);

    t->WaitForDeath();

    return 0;
}
