#include <iostream>
#include <unistd.h>
#include "CLThread.h"
#include "CLExecutiveFunctionProvider.h"

using namespace std;

/**
 * 这个版本讨论的问题是：新线程要是没创建成功或者是执行太快怎么办。
 * 通过两个Event的锁来做了协调，具体查看CLThread.cpp里面的Run和Start...方法。
 */


class CLParaPrinter : public CLExecutiveFunctionProvider {
public:
    CLParaPrinter() {
    }

    virtual ~CLParaPrinter() {
    }

    virtual CLStatus RunExecutiveFunction(void *pContext) {
        long i = (long) pContext;
        cout << i << endl;
        return CLStatus(0, 0);
    }
};

int main() {
    CLExecutive *pThread = new CLThread(new CLParaPrinter());
    pThread->Run((void *) 2);

    sleep(2);
    return 0;
}
