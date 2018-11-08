#include <iostream>
#include <unistd.h>
#include "CLThread.h"
#include "CLExecutiveFunctionProvider.h"

using namespace std;

/**
 * 这一版本提出的问题是：基于这些模板设计的继承类，创建他们是堆对象，那么在main函数里面还要显示的delete,
 * 针对这一问题能否简化一下。
 * 思路还是类似之前处理mutex的析构函数。
 * 首先Thread用一个指针持有了Provider。所以，在Execute的析构函数中delete掉Provider的内存就可以了。
 * 但是Thread本身的delete稍微有点疑惑。
 * 它吧Thread的delete写在了WaitForDeath里面，这个意思是，既然子线程已经join到main线程里面了，
 * 所以就不需要这个子线程了。
 * 那要是不调用WaitForDeath呢，这个Thread怎么delete呢？
 *
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

    pThread->WaitForDeath();

    return 0;
}
