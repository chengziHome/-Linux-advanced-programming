#include <iostream>
#include <unistd.h>
#include "CLThread.h"
#include "CLExecutiveFunctionProvider.h"

using namespace std;

/**
 * 这个版本提出的问题是：Thread类的任务应该是一次性的，如何避免run多次调用。
 * 解决方案也和简单，用一个对象基本的布尔变量。
 * 这个没什么特别好讨论的了。
 */

class CLParaPrinter : public CLExecutiveFunctionProvider {
public:
    CLParaPrinter() {
    }

    virtual ~CLParaPrinter() {
    }

    virtual CLStatus RunExecutiveFunction(void *pContext) {
        long i = (long) pContext;
        cout << "CLParaPrinter; " << i << endl;
        return CLStatus(0, 0);
    }
};

int main() {
    CLExecutiveFunctionProvider *printer = new CLParaPrinter();
    CLExecutive *pThread = new CLThread(printer);

    pThread->Run((void *) 2);

    CLStatus s = pThread->Run();
    if (!s.IsSuccess())
        cout << "Run error" << endl;

    pThread->WaitForDeath();

    delete pThread;
    delete printer;

    return 0;
}
