#include "CLCriticalSection.h"
#include "CLMutex.h"
#include "CLLogger.h"
#include "CLStatus.h"
#include <iostream>

CLCriticalSection::CLCriticalSection(CLMutex *pMutex) {
    if (pMutex == 0) {
        CLLogger::WriteLogMsg("In CLCriticalSection::CLCriticalSection(), pMutex == 0", 0);
        throw "In CLCriticalSection::CLCriticalSection(), pMutex == 0";
    }

    m_pMutex = pMutex;

    CLStatus s = m_pMutex->Lock();
    if (!s.IsSuccess()) {
        CLLogger::WriteLogMsg("In CLCriticalSection::CLCriticalSection(), m_pMutex->Lock error", 0);
        std::cout << "throw a exception\n" << std::endl;
        throw "In CLCriticalSection::CLCriticalSection(), m_pMutex->Lock error";
    }else{
        std::cout << "lock success " << std::endl;
    }
}

CLCriticalSection::~CLCriticalSection() {
    CLStatus s = m_pMutex->Unlock();
    std::cout << "调用了析构函数，放开锁" << std::endl;
    if (!s.IsSuccess()) {
        std::cout << "unlock error\n" << std::endl;
        CLLogger::WriteLogMsg("In CLCriticalSection::~CLCriticalSection(), m_pMutex->Unlock error", 0);
        throw "In CLCriticalSection::~CLCriticalSection(), m_pMutex->Unlock error";
    }else{
        std::cout << "unlock success\n" << std::endl;

    }
}

