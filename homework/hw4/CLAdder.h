//
// Created by chengzi on 18-11-16.
//

#ifndef CODE_CLADDER_H
#define CODE_CLADDER_H

#endif //CODE_CLADDER_H

#include "CLExecutiveFunctionProvider.h"
#include "CLMessageLoopManager.h"

class CLAdder:public CLExecutiveFunctionProvider{

public:
    CLAdder(CLMessageLoopManager *pMsgLoopManager);
    ~CLAdder();
    CLStatus RunExecutiveFunction(void *pContext);

private:
    CLMessageLoopManager *m_pMsgLoopManager;
};



