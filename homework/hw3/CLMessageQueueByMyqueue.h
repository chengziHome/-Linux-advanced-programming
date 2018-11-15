#ifndef CLMessageQueueBySTLqueue_H
#define CLMessageQueueBySTLqueue_H

#include "CLStatus.h"
#include "CLMutex.h"
#include "CLEvent.h"
#include "CLQueue.cpp"


class CLMessage;

/**
 * Push和Pop方法本身保证queue的线程安全性
 * PushMessage和GetMessage保证了生产者消费者之间的线程安全
 */

class CLMessageQueueByMyqueue {
public:

    CLMessageQueueByMyqueue(int k);

    virtual ~CLMessageQueueByMyqueue();

public:
    CLStatus PushMessage(CLMessage *pMessage);

    CLMessage *GetMessage();

private:
    CLStatus Push(CLMessage *pMessage);

    CLMessage *Pop();

private:
    CLMessageQueueByMyqueue(const CLMessageQueueByMyqueue &);

    CLMessageQueueByMyqueue &operator=(const CLMessageQueueByMyqueue &);

private:
    Queue<CLMessage *> m_MessageQueue;
    CLMutex m_Mutex;
    CLEvent m_Put_Event;    //生产者信号量
    CLEvent m_Get_Event;    //消费者信号量

};

#endif