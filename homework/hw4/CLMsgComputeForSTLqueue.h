//
// Created by chengzi on 18-11-16.
//

#ifndef CODE_CLMSGCOMPUTEFORSTLQUEUE_H
#define CODE_CLMSGCOMPUTEFORSTLQUEUE_H

#endif //CODE_CLMSGCOMPUTEFORSTLQUEUE_H

#include "CLMessageAdd.h"
#include "CLMsgLoopManagerForSTLqueue.h"

class CLMsgComputeForSTLqueue:public CLMsgLoopManagerForSTLqueue{

public:
    CLMsgComputeForSTLqueue(CLMessageQueueBySTLqueue *pM);
    ~CLMsgComputeForSTLqueue();
    CLStatus DispatchMessage(CLMessage *pM);
    CLStatus Compute(CLMessage *pM);

};







