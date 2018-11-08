#include <iostream>
#include "CLThread.h"
#include "CLExecutiveFunctionProvider.h"
#include "CLMessageQueueBySTLqueue.h"
#include "CLMessage.h"
#include "CLMsgLoopManagerForSTLqueue.h"
#include "CLMessageObserver.h"

using namespace std;

/**
 * 这个版本的任务也很好理解，就是消除之前provider中dispatch中的switch/case代码
 * 首先说说为什么消除，坑定要消除啊，业务的功能竟然没写死了，新增业务要改provider吗，这是非常严重地
 * 违反了”开放/封闭“法则。
 * 那么接下来是如何该？思路也清晰啊，就是吧新的业务都定义成新的类。
 * 这里的实现方法是：不同业务实现不同Processor,然后和Message的类别一一对应，
 * 给每中业务都有一个id，然后这些所有业务以键值对<msgID,Processor>的形式都注册到LoopManager的一个map里面，所以当Manager
 * 从消息队列中获得message后，就id找到之前存放的对应的Processor，调用这个Processor的notify方法，将message传递给它执行。
 *
 *
 * 到这里我有点反思哈，就是这里的message消息机制，和我理解的Task任务框架虽然很类似，但是还是不同的哈。
 * 消息机制中的message最终还只是一个信号标记，它不携带业务本身。而任务框架中Task本身就是一类业务的实体对象，
 * 然后按说你给到线程池，线程去调用task的run方法就行了。
 * 这两个东西确实有点不一样啊。
 *
 */

#define ADD_MSG 0
#define QUIT_MSG 1

class CLAddMsgProcessor;

class CLAddMessage : public CLMessage {
public:
    friend class CLAddMsgProcessor;

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

class CLAddMsgProcessor : public CLMessageObserver {
public:
    CLAddMsgProcessor() {
    }

    virtual ~CLAddMsgProcessor() {
    }

    virtual CLStatus Initialize(void *pContext) {
        return CLStatus(0, 0);
    }

    virtual CLStatus Notify(CLMessage *pM) {
        CLAddMessage *pAddMsg = (CLAddMessage *) pM;
        cout << pAddMsg->m_Op1 + pAddMsg->m_Op2 << endl;

        return CLStatus(0, 0);
    }
};

class CLQuitMsgProcessor : public CLMessageObserver {
public:
    CLQuitMsgProcessor() {
    }

    virtual ~CLQuitMsgProcessor() {
    }

    virtual CLStatus Initialize(void *pContext) {
        return CLStatus(0, 0);
    }

    virtual CLStatus Notify(CLMessage *pM) {
        CLQuitMessage *pQuitMsg = (CLQuitMessage *) pM;
        cout << "quit..." << endl;

        return CLStatus(QUIT_MESSAGE_LOOP, 0);
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
    CLMessageLoopManager *pLM = new CLMsgLoopManagerForSTLqueue(pQ);

    pLM->Register(ADD_MSG, new CLAddMsgProcessor);
    pLM->Register(QUIT_MSG, new CLQuitMsgProcessor);

    CLThread *t = new CLThread(new CLAdder(pLM), true);
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
