#include <iostream>
#include "CLThread.h"
#include "CLExecutiveFunctionProvider.h"
#include "CLMessageQueueBySTLqueue.h"
#include "CLMessage.h"

using namespace std;

#define ADD_MSG 0
#define QUIT_MSG 1

class CLAdder;

/**
 * 生产者消费者模型已经逐渐显示出来了。但是现在还是有很多问题。
 * 现在Event已经很明显，彻底沦落为一个Semaphore了。就是说他和业务是无关的，
 * 目前的业务执行模式是：具体执行步骤暂时在Provider里面，message提供一个选择罢了。
 * 而消息队列在两者中间，实际上完成了生产者消费者的模型。(注意，虽然queue是给到thread的参数，但是它最后还是传递给了provider的实现者)
 * 消息队列类本身兼顾了Semaphore的操作，我觉得这个后面坑定要解偶出来
 *
 */

class CLAddMessage : public CLMessage {
public:
    friend class CLAdder;

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

class CLAdder : public CLExecutiveFunctionProvider {
public:
    CLAdder() {
    }

    virtual ~CLAdder() {
    }

    virtual CLStatus RunExecutiveFunction(void *pContext) {
        CLMessageQueueBySTLqueue *q = (CLMessageQueueBySTLqueue *) pContext;
        CLAddMessage *pAddMsg;

        while (1) {
            CLMessage *pM = q->GetMessage();
            switch (pM->m_clMsgID) {
                case ADD_MSG:
                    pAddMsg = (CLAddMessage *) pM;
                    cout << pAddMsg->m_Op1 + pAddMsg->m_Op2 << endl;
                    break;

                case QUIT_MSG:
                    cout << "quit..." << endl;
                    return CLStatus(0, 0);

                default:
                    break;
            }
        }
    }
};

int main() {
    CLMessageQueueBySTLqueue q;

    CLThread *t = new CLThread(new CLAdder, true);
    t->Run(&q);

    CLAddMessage addmsg(2, 4);
    q.PushMessage(&addmsg);

    CLAddMessage addmsg1(5, 4);
    q.PushMessage(&addmsg1);

    CLAddMessage addmsg2(2, 9);
    q.PushMessage(&addmsg2);

    CLQuitMessage quitmsg;
    q.PushMessage(&quitmsg);


    t->WaitForDeath();

    return 0;
}

