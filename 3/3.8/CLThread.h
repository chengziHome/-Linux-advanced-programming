#ifndef CLTHREAD_H
#define CLTHREAD_H

#include <pthread.h>
#include "CLExecutive.h"
#include "CLStatus.h"

class CLThread : public CLExecutive {
public:

    /**
     * explicit关键子是表明该构造函数是显式的。默认是隐式的(implicit)。
     * 什么叫隐式的构造函数关键是。
     * String str = "123"
     * 就是一个隐式的构造函数。因为后面的“123”实际上是隐藏的调用了String("123")
     *
     * @param pExecutiveFunctionProvider
     */
    explicit CLThread(CLExecutiveFunctionProvider *pExecutiveFunctionProvider);

    virtual ~CLThread();

    virtual CLStatus Run(void *pContext = 0);

    virtual CLStatus WaitForDeath();

private:
    static void *StartFunctionOfThread(void *pContext);

private:
    void *m_pContext;
    pthread_t m_ThreadID;
};

#endif
