#include <iostream>
#include <unistd.h>
#include "CLThread.h"
#include "CLExecutiveFunctionProvider.h"

using namespace std;

/**
 * 这个版本就解决了我上个版本遗留的问题。
 * 首先在这再重述一下线程操作的含义。
 * pthread_create,仅仅是创建了线程，你如果不在main里面join它，它执行结束后一些资源可能得不到很好的处理
 * pthread_join，就是等待，谁调用它谁等待它，它就是个阻塞的情景。比如果Web服务器里面绝对不会用这个。
 * pthread_detach，这个话就是明确的说，我main线程不管你，你自己成立门户，自生自灭吧。这样给它足够的主权，它刚好可以在
 * 自己结束的时候回收各种资源。
 *
 * 所以，关于问题Thread的堆对象怎么delete呢？从上面我们发现，后两种都是合理的方式，所以就需要一个布尔变量，来标记
 * 这个Thread到底选join还是detach
 * 这里的waitForDeath仅仅是个自定义的，它的本质还是内部的join操作。
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

//    sleep(2);
    return 0;
}
