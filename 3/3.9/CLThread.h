#ifndef CLTHREAD_H
#define CLTHREAD_H

#include <pthread.h>
#include "CLStatus.h"
#include "CLLogger.h"

template<typename T>
class CLThread {
public:
    CLThread() {}

    ~CLThread() {}

    CLStatus Run(void *pContext = 0) {
        m_pContext = pContext;

        //运行时，这个this应该已经是MyCLThread了，不然的话，没有办法在下面的时候旧爱那个pThreadThis就能调用RunThreadFunction()啊
        int r = pthread_create(&m_ThreadID, 0, StartFunctionOfThread, this);
        if (r != 0) {
            CLLogger::WriteLogMsg("In CLThread::Run(), pthread_create error", r);
            return CLStatus(-1, 0);
        }

        return CLStatus(0, 0);
    }

    CLStatus WaitForDeath() {
        int r = pthread_join(m_ThreadID, 0);
        if (r != 0) {
            CLLogger::WriteLogMsg("In CLThread::WaitForDeath(), pthread_join error", r);
            return CLStatus(-1, 0);
        }

        return CLStatus(0, 0);
    }

private:
    static void *StartFunctionOfThread(void *pContext) {

        T *pThreadThis = (T *) pContext;

        CLStatus s = pThreadThis->RunThreadFunction();

        return (void *) s.m_clReturnCode;
    }

protected:
    void *m_pContext;
    pthread_t m_ThreadID;
};

#endif
