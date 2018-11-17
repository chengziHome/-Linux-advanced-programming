//
// Created by chengzi on 18-11-16.
//


#include "CLMsgControllerForSTLqueue.h"
#include <iostream>
#include "CLThread.h"
//#include "CLExecutiveFunctionProvider.h"
#include "CLMessageQueueBySTLqueue.h"
//#include "CLMessage.h"
//#include "CLMsgLoopManagerForSTLqueue.h"
#include "CLAdder.h"

CLMsgControllerForSTLqueue::~CLMsgControllerForSTLqueue() {
    delete m_queue;
}

CLMsgControllerForSTLqueue::CLMsgControllerForSTLqueue(CLMessageQueueBySTLqueue *queue,int k):CLMsgLoopManagerForSTLqueue(queue) {
    m_count = 0;

    m_k = k;
    m_queue = new CLMessageQueueBySTLqueue*[m_k];
    for(int i=0;i<k;i++){
        CLMessageQueueBySTLqueue *queue = new CLMessageQueueBySTLqueue();
        CLMsgComputeForSTLqueue *compute = new CLMsgComputeForSTLqueue(queue);
        CLThread *t = new CLThread(new CLAdder(compute), true);
        t->Run(0);
        m_queue[i] = queue;
    }

}

CLStatus CLMsgControllerForSTLqueue::DispatchMessage(CLMessage *pM) {
    CLMessageQueueBySTLqueue *queue = m_queue[m_count%m_k];
    queue->PushMessage(pM);
    m_count++;
}
