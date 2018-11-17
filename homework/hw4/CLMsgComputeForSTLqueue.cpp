//
// Created by chengzi on 18-11-16.
//


#include "CLMsgComputeForSTLqueue.h"
#include <iostream>

CLMsgComputeForSTLqueue::CLMsgComputeForSTLqueue(CLMessageQueueBySTLqueue *pM):CLMsgLoopManagerForSTLqueue(pM) {

}

CLMsgComputeForSTLqueue::~CLMsgComputeForSTLqueue() {}

CLStatus CLMsgComputeForSTLqueue::DispatchMessage(CLMessage *pM) {
    return Compute(pM);
}

CLStatus CLMsgComputeForSTLqueue::Compute(CLMessage *pM) {
    CLMessageAdd *pmsg = (CLMessageAdd *)pM;
//    std::cout << "Compute . No." << pmsg->m_no << ":" << pmsg->m_Op1 << "+" << pmsg->m_Op2 << "=" << (pmsg->m_Op1 + pmsg->m_Op2) << "  " <<std::endl;
    std::cout << "Compute . No." << pmsg->m_no <<std::endl;
}

