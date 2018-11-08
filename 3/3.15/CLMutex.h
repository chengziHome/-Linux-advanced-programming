#ifndef CLMutex_H
#define CLMutex_H

#include <pthread.h>
#include "CLStatus.h"


/**
 * 这个封装看起来就利爽多了
 */

class CLMutex {
public:
    /*
    ���캯����������������ʱ�����׳��ַ��������쳣
    */
    CLMutex();

    virtual ~CLMutex();

    CLStatus Lock();

    CLStatus Unlock();

private:
    CLMutex(const CLMutex &);

    CLMutex &operator=(const CLMutex &);

private:
    pthread_mutex_t m_Mutex;
};

#endif