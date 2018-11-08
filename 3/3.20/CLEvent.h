#ifndef CLEVENT_H
#define CLEVENT_H

#include "CLStatus.h"
#include "CLMutex.h"
#include "CLConditionVariable.h"

/*
 ����һ����ʼ���źţ��Զ������źŵ��źţ����ڻ���һ�� �ȴ��̣߳�
*/
class CLEvent {
public:
    /*
    ���캯����������������ʱ�����׳��ַ��������쳣
    */
    CLEvent();

    virtual ~CLEvent();

public:
    CLStatus Set();

    CLStatus Wait();

private:
    CLEvent(const CLEvent &);

    CLEvent &operator=(const CLEvent &);

private:
    CLMutex m_Mutex;
    CLConditionVariable m_Cond;
    int m_Flag;
};

#endif