//
// Created by chengzi on 18-11-16.
//

#include "CLMessageAdd.h"


#define ADD_MSG 0
#define QUIT_MSG 1

CLMessageAdd::CLMessageAdd(int no,int Op1, int Op2):CLMessage(ADD_MSG) {
    m_no = no;
    m_Op1 = Op1;
    m_Op2 = Op2;
}

CLMessageAdd::~CLMessageAdd() {}
