#include <iostream>
#include <unistd.h>
#include "CLThread.h"
#include "CLExecutiveFunctionProvider.h"
#include "CLMutex.h"
#include "CLCriticalSection.h"

using namespace std;

struct SPara {
    int Flag;
    CLMutex mutex;
};


/**
 * 以前我还一直在想，一行代码如何控制到它所在的整个code block的生命周期呢？
 * 根据CLCriticalSection的实现来看，原来是利用了栈对象的在作用域结束后调用
 * 析构函数完成的。
 */

class CLMyFunction : public CLExecutiveFunctionProvider {
public:
    CLMyFunction() {
    }

    virtual ~CLMyFunction() {
    }

    void test() {
        throw 32;
    }

    virtual CLStatus RunExecutiveFunction(void *pContext) {
        try {
            SPara *p = (SPara *) pContext;

            CLCriticalSection cs(&(p->mutex));

            p->Flag++;

            test();
        }
        catch (...) {
            cout << "exception" << endl;
        }

        return CLStatus(0, 0);
    }
};

int main() {
    CLExecutiveFunctionProvider *myfunction = new CLMyFunction();
    CLExecutive *pThread = new CLThread(myfunction);

    SPara *p = new SPara;
    p->Flag = 3;

    pThread->Run((void *) p);

    sleep(2);

    {
        CLCriticalSection cs(&(p->mutex));

        p->Flag++;

        cout << p->Flag << endl;
    }

    pThread->WaitForDeath();

    delete p;

    delete pThread;
    delete myfunction;

    return 0;
}
