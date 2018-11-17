//
// Created by chengzi on 18-11-15.
//
#include "CLMessageQueueByMyqueue.h"
#include "CLCriticalSection.h"
#include "CLMessage.h"
#include "CLLogger.h"
#include <iostream>

CLMessageQueueByMyqueue::CLMessageQueueByMyqueue(int k):m_Put_Event(true,k),m_Get_Event(true,0),m_MessageQueue(k) {

}
CLMessageQueueByMyqueue::~CLMessageQueueByMyqueue() {}

CLStatus CLMessageQueueByMyqueue::PushMessage(CLMessage *pMessage) {
    if (pMessage == NULL)
        return CLStatus(-1, 0);

    m_Put_Event.Wait();

    CLStatus s = Push(pMessage);

    if(!s.IsSuccess()){
        CLLogger::WriteLogMsg("In CLMessageQueue::PushMessage(), Push error", 0);
        return CLStatus(-1, 0);
    }
    std::cout << "put a message" << std::endl;
    CLStatus s2 = m_Get_Event.Set();
    if (!s2.IsSuccess()) {
        CLLogger::WriteLogMsg("In CLMessageQueue::PushMessage(), m_Event.Set error", 0);
        return CLStatus(-1, 0);
    }
    return CLStatus(0, 0);
}

CLMessage* CLMessageQueueByMyqueue::GetMessage() {
    CLStatus s = m_Get_Event.Wait();
    if (!s.IsSuccess()) {
        CLLogger::WriteLogMsg("In CLMessageQueue::GetMessage(), m_Event.Wait error", 0);
        return 0;
    }

    CLMessage *p = Pop();
    std::cout << "get a message" << std::endl;
    CLStatus s2 = m_Put_Event.Set();
    if (!s2.IsSuccess()) {
        CLLogger::WriteLogMsg("In CLMessageQueue::GetMessage(), m_Event.Wait error", 0);
        return 0;
    }
    return p;

}

CLStatus CLMessageQueueByMyqueue::Push(CLMessage *pMessage) {

    try {
        CLCriticalSection cs(&m_Mutex);
        if(m_MessageQueue.isFull())
            return CLStatus(-1,0);
        m_MessageQueue.enqueue(pMessage);
    }catch (const char *str) {
        CLLogger::WriteLogMsg("In CLMessageQueue::Push(), exception arise", 0);
        return CLStatus(-1, 0);
    }

}

CLMessage* CLMessageQueueByMyqueue::Pop() {
    try {
        CLCriticalSection cs(&m_Mutex);
        if(m_MessageQueue.isEmpty())
            return 0;
        CLMessage *p = m_MessageQueue.dequeue();
        return p;
    }catch (const char *str) {
        CLLogger::WriteLogMsg("In CLMessageQueue::Pop(), exception arise", 0);
        return 0;
    }

}


