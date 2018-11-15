#ifndef CLConditionVariable_H
#define CLConditionVariable_H

#include <pthread.h>
#include "CLStatus.h"

class CLMutex;

class CLConditionVariable {
public:
    /*
    ���캯����������������ʱ�����׳��ַ��������쳣
    */
    CLConditionVariable();

    virtual ~CLConditionVariable();

    CLStatus Wait(CLMutex *pMutex);

    CLStatus Wakeup();

    CLStatus WakeupAll();

private:
    CLConditionVariable(const CLConditionVariable &);

    CLConditionVariable &operator=(const CLConditionVariable &);

private:
    pthread_cond_t m_ConditionVariable;
};

#endif