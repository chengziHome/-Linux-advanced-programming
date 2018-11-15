#ifndef CLEVENT_H
#define CLEVENT_H

#include "CLStatus.h"
#include "CLMutex.h"
#include "CLConditionVariable.h"

/*
Ĭ������£�����һ����ʼ���źţ��Զ����õ��¼������ڻ���һ���ȴ��̣߳�
�ڹ��캯���У���bSemaphore������Ϊtrue������ü���Set�����ɻ��Ѽ��λ���߳�
*/
class CLEvent {
public:
    /*
    ���캯����������������ʱ�����׳��ַ��������쳣
    */
    CLEvent();

    explicit CLEvent(bool bSemaphore);
    explicit CLEvent(bool bSemaphore,int k);

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

    bool m_bSemaphore;
};

#endif
