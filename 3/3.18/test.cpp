#include <iostream>
#include <unistd.h>
#include "CLThread.h"
#include "CLExecutiveFunctionProvider.h"
#include "CLMutex.h"
#include "CLCriticalSection.h"
#include "CLConditionVariable.h"

using namespace std;

class CLMyFunction : public CLExecutiveFunctionProvider {
public:
    CLMyFunction() {
    }

    virtual ~CLMyFunction() {
    }

    virtual CLStatus RunExecutiveFunction(void *pContext) {
        CLConditionVariable *pCondition = (CLConditionVariable *) pContext;
        //sleep(2);

        pCondition->Wakeup();
        cout << "CLMyFunction wakeup \n" << endl;
        return CLStatus(0, 0);
    }
};


/**
 * 之所以主线程会阻塞是因为：
 * 首先搞清楚这里慢的线程的关系。
 * 一个主线程，一个子线程。
 * 两个线程的同步是通过同一个condition变量来实现的。
 * 之所以主线程阻塞是因为，子线程先调用了wakeup.然后main再wait就没人去唤醒它了。
 * 加上了sleep之后，就刚好把上面的顺序纠正了。
 * @return
 */

int main() {
    CLConditionVariable *pCondition = new CLConditionVariable();
    CLMutex *pMutex = new CLMutex();

    CLExecutiveFunctionProvider *myfunction = new CLMyFunction();
    CLExecutive *pThread = new CLThread(myfunction);
    pThread->Run((void *) pCondition);

    {
        CLCriticalSection cs(pMutex);

//        sleep(1);
        pCondition->Wait(pMutex);
        cout << "main thread wait :\n" << endl;
    }

    pThread->WaitForDeath();

    cout << "in main thread" << endl;

    delete pThread;
    delete myfunction;
    delete pMutex;
    delete pCondition;

    return 0;
}
