//
// Created by chengzi on 18-11-16.
//

#include "CLAdder.h"

CLAdder::CLAdder(CLMessageLoopManager *pMsgLoopManager) {
    if (pMsgLoopManager == 0)
        throw "In CLAdder::CLAdder(), pMsgLoopManager error";

    m_pMsgLoopManager = pMsgLoopManager;
}

CLAdder::~CLAdder() {
    delete m_pMsgLoopManager;
}

CLStatus CLAdder::RunExecutiveFunction(void *pContext) {
    return m_pMsgLoopManager->EnterMessageLoop(pContext);
}

