#include "CLRegularCoordinator.h"
#include "CLExecutiveFunctionProvider.h"
#include "CLExecutive.h"
#include <iostream>
#include <unistd.h>

using namespace std;

CLRegularCoordinator::CLRegularCoordinator() {
}

CLRegularCoordinator::~CLRegularCoordinator() {
}

CLStatus CLRegularCoordinator::Run(void *pContext) {
    if ((m_pExecutive == 0) || (m_pExecutiveFunctionProvider == 0))
        return CLStatus(-1, 0);

    m_pContext = pContext;

    return m_pExecutive->Run();
}

/**
 * 这个版本的关键就在这个方法的实现。
 * 从test结果来看，好像都是thread创建之后就立即执行了任务，和上一个版本没有什么区别。
 * 但是不同在于上个版本run任务是在MyThread里面调用的，本质上还是回调的思路，而这次是在coordinator里面调用的。
 * 这次coordinator的干预权利就非常大了。具体就体现在，它可以在这个方法里面做任何事情(协调工作)
 *
 * @return
 */
CLStatus CLRegularCoordinator::ReturnControlRights() {

    std::cout << "Before I execute the task,I can do anything I want,cause I am a powerful coordinator.\n" <<
                 "Now I want to sleep 5 seconds." << std::endl;
    sleep(5);
    return m_pExecutiveFunctionProvider->RunExecutiveFunction(m_pContext);
}

CLStatus CLRegularCoordinator::WaitForDeath() {
    return m_pExecutive->WaitForDeath();
}