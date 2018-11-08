#ifndef CLLogger_H
#define CLLogger_H

#include "CLStatus.h"

/*
�������ļ�LOG_FILE_NAME�У���¼��־��Ϣ
*/
class CLLogger {
public:
    static CLLogger *GetInstance();

    static CLStatus WriteLogMsg(const char *pstrMsg, long lErrorCode);

    CLStatus WriteLog(const char *pstrMsg, long lErrorCode);

    CLStatus Flush();

private:
    static void OnProcessExit();

private:
    CLLogger(const CLLogger &);

    CLLogger &operator=(const CLLogger &);

    CLLogger();

    ~CLLogger();

private:
    int m_Fd;
    static CLLogger *m_pLog;

private:
    char *m_pLogBuffer;
    unsigned int m_nUsedBytesForBuffer;

private:
    bool m_bFlagForProcessExit;
};

#endif