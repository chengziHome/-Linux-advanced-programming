#include<pthread.h>
#include<iostream>
#include<unistd.h>
#include<stdlib.h>
#include"CLStatus.h"
#include"CLLogger.h"

using namespace std;

/**
 * 这个版本最大的进步是：将Task和executor也完全解耦。
 * 当然了task肯定要给executor(也就是这里面的thread)提供一个回调函数。
 * 开始慢慢的想AOP靠拢了。但是我还没具体明白，“增加切面”具体是怎么操作的。
 *
 * 这个不足就是，创建了线程之后，就立即去执行task了，实际上这一点也反映出协调者coordinator还是个初级的，
 * 他的协调能力还很弱，仅仅将两个凑拢到一块了。
 *
 */

class CMyFunction {
public:
    CLStatus RunExecutiveFunction(void *pContext) {
        long i = (long) pContext;
        cout << "CMyFunction : context:" << i << endl;
        return CLStatus(0, 0);
    }
};


class CLThread {
public:
    template<typename TDerivedClass>
    CLStatus Create() {
        pthread_t tid;
        int r = pthread_create(&tid, NULL, StartFunctionOfThread < TDerivedClass > , this);
        if (r != 0) {
            CLLogger::WriteLogMsg("pthread_create error", r);
            return CLStatus(-1, 0);
        }

        return CLStatus(0, 0);
    }

    template<typename TDerivedClass>
    static void *StartFunctionOfThread(void *pContext) {
        TDerivedClass *pThis = static_cast<TDerivedClass *>(pContext);

        pThis->ProcessByProxy();

        return 0;
    }
};

template<typename TExecutive, typename TExecutiveFunctionProvider>
class CLCoordinator : public TExecutive, public TExecutiveFunctionProvider {
public:
    CLStatus Run(void *pContext) {
        m_pContext = pContext;

        TExecutive *pExecutive = static_cast<TExecutive *>(this);

        typedef CLCoordinator<TExecutive, TExecutiveFunctionProvider> T;

        return (*pExecutive).template Create<T>();
    }

    void ProcessByProxy() {
        TExecutiveFunctionProvider *pProvider = static_cast<TExecutiveFunctionProvider *>(this);

        pProvider->RunExecutiveFunction(m_pContext);
    }

private:
    void *m_pContext;
};

int main() {
    CLCoordinator<CLThread, CMyFunction> t1;
    t1.Run((void *) 5);

    sleep(5);
    return 0;
}
