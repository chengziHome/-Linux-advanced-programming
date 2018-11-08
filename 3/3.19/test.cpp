#include <iostream>
#include <unistd.h>
#include "CLThread.h"
#include "CLExecutiveFunctionProvider.h"
#include "CLMutex.h"
#include "CLCriticalSection.h"
#include "CLConditionVariable.h"

using namespace std;

/**
 * 认真读一下这个版本，信息比较关键
 */



/**
 * 首先，condition是用race condition的一个封装，用来协调多个线程之间的合作。
 * 在condition的模式里面一个问题就是，先wakeup了，在去wait的时候怎么办(也就是上个版本遇到的问题)
 * 所以这个时候用一个flag去标记是否已经执行过wakeup了，是的话，就不去wait了。不是的话，才wait。
 * 然后mutex的本质是用来给flag上锁，互斥它的读写的。
 * 另外condition和mutex什么关系呢？其中只有pthread_cond_wait有mutex这个参数。
 * mutex绝不是为了满足condition内部的某些互斥要求，你想，当一个线程执行到pthread_cond_wait之前，一定读取了flag，
 * 现在线程要挂起，说明一，读取flag已经完成，二我要放开锁啊。因为执行single的线程一定先修改flag在去wakeup的(先阻止新的县城wait，在去wakeup)
 * 要是不放开，别人怎么修改这个flag呢，不就成了死锁了吗。所以，pthread_cond_wait一定执行了mutex.unlock操作。
 *
 */
struct SPara {

    CLConditionVariable condition;
    CLMutex mutex;
    int flag;
};

class CLMyFunction : public CLExecutiveFunctionProvider {
public:
    CLMyFunction() {
    }

    virtual ~CLMyFunction() {
    }

    virtual CLStatus RunExecutiveFunction(void *pContext) {
        SPara *p = (SPara *) pContext;

        {

            CLCriticalSection cs(&(p->mutex));
            cout << "mythread first?\n" << endl;

            p->flag = 1;
        }
        cout << "mythread out ,the flag=" << p->flag << "\n" << endl;

        p->condition.Wakeup();
        cout << "mythread wakeup  \n" << endl;

        return CLStatus(0, 0);
    }
};


/**
 * 这个版本解决的最大的问题是，先wakeup在wait怎么办？
 * 下面演绎一下这两种情况。
 * （1）先main，后thread(实际情况)
 *  main获得mutex
 *  main读取flag，发现是0
 *  main执行condition.wait():线程挂起，并且放弃了mutex
 *  ......
 *  调度到mythread
 *  mythread获取mutex
 *  修改flag=1
 *  mythread放弃mutex(析构函数)
 *  mythread执行condition.wakeup()
 *  mythread推出。
 *  ...
 *  重新调度到main，main从wait状态中醒来
 *  执行while循环的第二次，发现flag已经是1
 *  于是退出while循环。
 *  然后再次放弃mutex，(也许这里是不冲突的吧)
 *  main执行完毕
 *  (2)如果是先thread，后main(可以通过给main执行sleep(1)来实现)
 *  main执行到mythread.
 *  main开始sleep......
 *  mythread执行
 *  获取mutex，
 *  更改了flag=1,
 *  放弃mutex
 *  执行了wakeup
 *  推出mythread
 *  ......main睡醒了
 *  main获得了mutex
 *  查看flag=1，直接跳过while(也就是说根本不执行wait)
 *  放弃了mutex
 *  main执行完毕
 *
 * @return
 */


int main() {
    SPara *p = new SPara;
    p->flag = 0;

    CLExecutiveFunctionProvider *myfunction = new CLMyFunction();
    CLExecutive *pThread = new CLThread(myfunction);
    pThread->Run((void *) p);

//    sleep(1);

    {
        CLCriticalSection cs(&(p->mutex));
        cout << "main first?\n" << endl;


        while (p->flag == 0){
            cout << "main:getin  wait" << endl;

            p->condition.Wait(&(p->mutex));
            cout << "main: after wait" << endl;
        }
    }
    cout << "main out ?\n" << endl;

    pThread->WaitForDeath();

    cout << "in main thread" << endl;

    delete pThread;
    delete myfunction;
    delete p;

    return 0;
}
