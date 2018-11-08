#include "CLThread.h"
#include "CLExecutiveFunctionProvider.h"
#include "CLLogger.h"

CLThread::CLThread(CLExecutiveFunctionProvider *pExecutiveFunctionProvider) : CLExecutive(pExecutiveFunctionProvider) {
    m_pContext = 0;
    m_bThreadCreated = false;
    m_bWaitForDeath = false;
}

CLThread::CLThread(CLExecutiveFunctionProvider *pExecutiveFunctionProvider, bool bWaitForDeath) : CLExecutive(
        pExecutiveFunctionProvider) {
    m_pContext = 0;
    m_bWaitForDeath = bWaitForDeath;
    m_bThreadCreated = false;
}

CLThread::~CLThread() {
}


/**
 * 解读下面两个方法的同步问题。
 * 这里用到了两个Event.
 * 前面这个WaitForNewThread的Event是为了保证，Run结束之前确实是创建了新线程的，让我们简单演绎一下
 * Run函数创建了线程。然后往下执行，在event处wait，被阻塞，只有新线程真的创建了并且执行到了StartFunctionOfThread方法的Set,Run才能接着往下走。
 * 第二个Event是为了保证Run函数先执行完，要是子线程太快也得等着，或者说，子线程就是等着Run"马上要执行完了"才去真正的执行业务代码。
 * @param pContext
 * @return
 */

CLStatus CLThread::Run(void *pContext) {
    if (m_bThreadCreated)
        return CLStatus(-1, 0);

    m_pContext = pContext;

    int r = pthread_create(&m_ThreadID, 0, StartFunctionOfThread, this);
    if (r != 0) {
        CLLogger::WriteLogMsg("In CLThread::Run(), pthread_create error", r);
        delete this;
        return CLStatus(-1, 0);
    }

    m_bThreadCreated = true;

    if (!m_bWaitForDeath) {
        r = pthread_detach(m_ThreadID);
        if (r != 0)
            CLLogger::WriteLogMsg("In CLThread::Run(), pthread_detach error", r);
    }

    CLStatus s = m_EventForWaitingForNewThread.Wait();
    if (!s.IsSuccess()) {
        CLLogger::WriteLogMsg("In CLThread::Run(), m_EventForWaitingForNewThread.Wait error", 0);
    }

    CLStatus s1 = m_EventForWaitingForOldThread.Set();
    if (!s1.IsSuccess()) {
        CLLogger::WriteLogMsg("In CLThread::Run(), m_EventForWaitingForOldThread.Set error", 0);
    }

    return CLStatus(0, 0);
}

void *CLThread::StartFunctionOfThread(void *pThis) {
    CLThread *pThreadThis = (CLThread *) pThis;

    void *pContext = pThreadThis->m_pContext;
    pThreadThis->m_pContext = NULL;

    CLStatus s = pThreadThis->m_EventForWaitingForNewThread.Set();
    if (!s.IsSuccess()) {
        CLLogger::WriteLogMsg("In CLThread::StartFunctionOfThread(), m_pEventForThreadSynchronization.Set error", 0);
    }

    CLStatus s1 = pThreadThis->m_EventForWaitingForOldThread.Wait();
    if (!s1.IsSuccess()) {
        CLLogger::WriteLogMsg("In CLThread::StartFunctionOfThread(), m_EventForWaitingForOldThread.Wait error", 0);
    }

    pThreadThis->m_pExecutiveFunctionProvider->RunExecutiveFunction(pContext);

    if (!(pThreadThis->m_bWaitForDeath))
        delete pThreadThis;

    return 0;
}

CLStatus CLThread::WaitForDeath() {
    if (!m_bWaitForDeath)
        return CLStatus(-1, 0);

    if (!m_bThreadCreated)
        return CLStatus(-1, 0);

    int r = pthread_join(m_ThreadID, 0);
    if (r != 0) {
        CLLogger::WriteLogMsg("In CLThread::WaitForDeath(), pthread_join error", r);
        return CLStatus(-1, 0);
    }

    delete this;

    return CLStatus(0, 0);
}