#ifndef CLMessageLoopManager_H
#define CLMessageLoopManager_H

#include "CLStatus.h"

class CLMessage;

#define QUIT_MESSAGE_LOOP 1

class CLMessageLoopManager {
public:
    CLMessageLoopManager();

    virtual ~CLMessageLoopManager();

    virtual CLStatus EnterMessageLoop(void *pContext);

protected:
    /*
    ��ʼ���뷴��ʼ����Ϣѭ������Ҫ��֤��Ϣ�����Ѿ��������
    */
    virtual CLStatus Initialize() = 0;

    virtual CLStatus Uninitialize() = 0;

    virtual CLMessage *WaitForMessage() = 0;

    virtual CLStatus DispatchMessage(CLMessage *pMessage) = 0;

private:
    CLMessageLoopManager(const CLMessageLoopManager &);

    CLMessageLoopManager &operator=(const CLMessageLoopManager &);
};

#endif
