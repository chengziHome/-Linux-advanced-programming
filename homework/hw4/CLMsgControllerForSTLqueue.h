//
// Created by chengzi on 18-11-16.
//

#ifndef CODE_CLMSGCONTROLLERFORSTLQUEUE_H
#define CODE_CLMSGCONTROLLERFORSTLQUEUE_H

#endif //CODE_CLMSGCONTROLLERFORSTLQUEUE_H

#include "CLMsgLoopManagerForSTLqueue.h"
#include "CLMessageQueueBySTLqueue.h"
#include "CLMsgComputeForSTLqueue.h"


class CLMsgControllerForSTLqueue:public CLMsgLoopManagerForSTLqueue {

public:
    CLMsgControllerForSTLqueue(CLMessageQueueBySTLqueue *queue,int k);
    ~CLMsgControllerForSTLqueue();

    CLStatus DispatchMessage(CLMessage *pM);

private:
    int m_k;  //计算线程的数量
    int m_count;
    CLMessageQueueBySTLqueue **m_queue;

};












