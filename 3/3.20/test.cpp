#include <iostream>
#include <unistd.h>
#include "CLThread.h"
#include "CLExecutiveFunctionProvider.h"
#include "CLEvent.h"

using namespace std;


/**
 * 我想我现在终于知道，为什么它吧业务类的结构叫做Provider了，因为在我印象里面它应该叫做task之类的东西。
 * 现在终于明白了，它吧业务代码还是抽了出来，叫做event。provider仅仅是衔接CLThread和Event的中间件。
 * 现在的一个基本的线程和任务分离调度的最基本简陋的架构就出来了。总共是3部分。
 * Thread部分是线程实体封装。Event代表具体业务代码。Provider是中间的，可以理解为给Thread装配一个Event接收器。
 * 再次强调，Thread仅仅代表线程，业务无关，业务完全分离为到了Event这一层。解耦的工作慢慢有了眉目。
 * 前提是，现在还是面向接口的架构。
 * 另外，
 * 线程同步控制还放在Event这一层。
 *
 * 无论如何，这个版本应该说可以给一个打版本的patch了
 */


class CLMyFunction : public CLExecutiveFunctionProvider {
public:
    CLMyFunction() {
    }

    virtual ~CLMyFunction() {
    }

    virtual CLStatus RunExecutiveFunction(void *pContext) {
        CLEvent *pEvent = (CLEvent *) pContext;

//        sleep(2);
        pEvent->Set();

        return CLStatus(0, 0);
    }
};


/**
 * 为了熟悉流程在写一次两个线程的执行顺序：(两种情况可以通过sleep函数相互调节转化)
 *  (1)main线程先执行(我机器上的实际情况)
 *  main调用了pThread->run，然后继续向下走
 *  main线程到pEvent.Wait().内部：
 *      获取pEvent的mutex
 *      判断了event的m_flag=0,
 *      while可以进入，
 *      执行了cond_wait
 *  main线程由于cond_wait而挂起，并且被迫释放了pEvent的mutex
 *  pThread开始执行
 *  pThread调用了pEvent.Set,内部过程：
 *      获取pEvent的mutex
 *      修改m_flag=1
 *      调用cond.wakeup.
 *  pThread主要任务执行完毕
 *  转到main线程，
 *  main接上pEvent->Wait()后面的代码继续执行
 *  (2)子线程先执行
 *  和上个版本类似，主要就是while直接跳过。实验也验证了这一点。
 * @return
 */

int main() {
    CLEvent *pEvent = new CLEvent;

    CLExecutiveFunctionProvider *myfunction = new CLMyFunction();
    CLExecutive *pThread = new CLThread(myfunction);
    pThread->Run((void *) pEvent);

//    sleep(1);
    pEvent->Wait();

    pThread->WaitForDeath();

    cout << "in main thread" << endl;

    delete pThread;
    delete myfunction;
    delete pEvent;

    return 0;
}
