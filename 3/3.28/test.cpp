#include <iostream>
#include "CLThread.h"
#include "CLExecutiveFunctionProvider.h"
#include "CLMessageQueueBySTLqueue.h"
#include "CLMessage.h"
#include "CLMsgLoopManagerForSTLqueue.h"

using namespace std;

/**
 * 这一版本的主要任务是引入了消息循环机制，为此也引入了一个关于消息队列的Manager类。
 * 这个类核心方法有两个，一个是EnterLoop就是进入消息循环机制，
 * 第二个是渠道消息，你要把它派发给具体的Provider去执行啊，也就是DispatchMsg方法了
 * 可以看看ppt90页的类的继承图了
 *
 *
 * 这个时候你可以看看作业4的要求，发现已经慢慢有点模样了。
 * Manager就是那个主控线程。
 * 用户线程是用来产生消息的，目前是在Main方法里面手动添加的。
 * 然后最重要的应该是计算线程池，但很明显，目前仅仅有一个线程。
 *
 * 另外在这里点名一点线程是一个虚拟概念，它不是一个变量一个对象，它是一个环境。
 * 你看就算Manager都是作为参数传递给Thread的。确实是这个Manager要去协调多个线程。
 * 但是不是说Manager里面持有一个列表，标记了各个线程的属性。恰恰相反，是每个线程
 * 都持有一个相同的Manager的指针。不同的线程执行到manager方法的不同位置，在manager是根据环境
 * 来判断是那个线程，获取线程属性，并执行相关操作。
 *
 *
 *
 */


#define ADD_MSG 0
#define QUIT_MSG 1

class CLMyMsgProcessor;

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

class CLMyMsgProcessor : public CLMsgLoopManagerForSTLqueue {
public:
    CLMyMsgProcessor(CLMessageQueueBySTLqueue *pQ) : CLMsgLoopManagerForSTLqueue(pQ) {
    }

    virtual ~CLMyMsgProcessor() {
    }

    virtual CLStatus DispatchMessage(CLMessage *pM) {
        CLAddMessage *pAddMsg;
        switch (pM->m_clMsgID) {
            case ADD_MSG:
                pAddMsg = (CLAddMessage *) pM;
                cout << "pAddMsg:" << pAddMsg->m_Op1 + pAddMsg->m_Op2 << endl;
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
        if (pMsgLoopManager == 0)
            throw "In CLAdder::CLAdder(), pMsgLoopManager error";

        m_pMsgLoopManager = pMsgLoopManager;
    }

    virtual ~CLAdder() {
    }

    virtual CLStatus RunExecutiveFunction(void *pContext) {
        return m_pMsgLoopManager->EnterMessageLoop(pContext);
    }
};

int main() {
    CLMessageQueueBySTLqueue *pQ = new CLMessageQueueBySTLqueue();
    CLMessageLoopManager *pM = new CLMyMsgProcessor(pQ);

    CLThread *t = new CLThread(new CLAdder(pM), true);
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

    delete pquitmsg;
    delete paddmsg2;
    delete paddmsg1;
    delete paddmsg;
    delete pQ;
    delete pM;

    return 0;
}
