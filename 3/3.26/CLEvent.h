#ifndef CLEVENT_H
#define CLEVENT_H

#include "CLStatus.h"
#include "CLMutex.h"
#include "CLConditionVariable.h"

/*
Ĭ������£�����һ����ʼ���źţ��Զ����õ��¼������ڻ���һ���ȴ��̣߳�
�ڹ��캯���У���bSemaphore������Ϊtrue������ü���Set�����ɻ��Ѽ��λ���߳�
*/


/**
 * 到现在再重新解读一下这个Event类。
 * 之前我吧他理解为业务代码层，到现在也不能说错，而是说这个Event的职能目前好像很单一，仅仅是控制线程协调。
 *
 * 这个版本里面的主要改动是给Event加上了一个semaphore变量，顾名思义就是信号量嘛。
 * 但是之前我对condition的理解一直不够透彻，condition内部是有cond_wait,cond_single,cond_broadcast这些方法的。
 * 这本来就是个阻塞队列。所以说，这个semaphore的作用就是标记是否要把Event当作型号量看待？如果是的话，那么flag就变成了
 * condition阻塞队列的计数器。这不就是OS里面经典的两种信号量嘛。
 */
class CLEvent {
public:
    /*
    ���캯����������������ʱ�����׳��ַ��������쳣
    */
    CLEvent();

    explicit CLEvent(bool bSemaphore);

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
