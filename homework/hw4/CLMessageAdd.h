//
// Created by chengzi on 18-11-16.
//

#ifndef CODE_CLMESSAGEADD_H
#define CODE_CLMESSAGEADD_H

#endif //CODE_CLMESSAGEADD_H

#include "CLMessage.h"
//#include "CLMsgComputeForSTLqueue.h"

class CLMessageAdd: public CLMessage {
public:
    CLMessageAdd(int no,int Op1,int Op2);
    ~CLMessageAdd();



public:
    int m_no;
    int m_Op1;
    int m_Op2;
    int m_result;
};




