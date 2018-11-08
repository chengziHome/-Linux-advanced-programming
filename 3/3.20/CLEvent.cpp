#include "CLEvent.h"
#include "CLCriticalSection.h"
#include "CLLogger.h"
#include <iostream>

CLEvent::CLEvent() {
    m_Flag = 0;
}

CLEvent::~CLEvent() {
}

CLStatus CLEvent::Set() {
    try {
        CLCriticalSection cs(&m_Mutex);

        m_Flag = 1;
    }
    catch (const char *str) {
        CLLogger::WriteLogMsg("In CLEvent::Set(), exception arise", 0);
        return CLStatus(-1, 0);
    }

    CLStatus s = m_Cond.Wakeup();

    if (!s.IsSuccess()) {
        CLLogger::WriteLogMsg("In CLEvent::Set(), m_Cond.Wakeup error", 0);
        return CLStatus(-1, 0);
    }
    std::cout << "Event set ... flat=" << m_Flag << std::endl;
    return CLStatus(0, 0);
}

CLStatus CLEvent::Wait() {
    try {
        CLCriticalSection cs(&m_Mutex);
        std::cout << "Event Wait ... m_flag:" << m_Flag << std::endl;

        while (m_Flag == 0) {
            CLStatus s = m_Cond.Wait(&m_Mutex);
            if (!s.IsSuccess()) {
                CLLogger::WriteLogMsg("In CLEvent::Wait(), m_Cond.Wait error", 0);
                return CLStatus(-1, 0);
            }
            std::cout << "cond wait ... " << std::endl;
        }

        m_Flag = 0;
    }
    catch (const char *str) {
        CLLogger::WriteLogMsg("In CLEvent::Wait(), exception arise", 0);
        return CLStatus(-1, 0);
    }
    std::cout << "Event Wait ... " << std::endl;

    return CLStatus(0, 0);
}
