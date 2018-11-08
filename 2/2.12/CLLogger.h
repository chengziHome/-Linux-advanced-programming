#ifndef CLLogger_H
#define CLLogger_H

#include "CLStatus.h"

/*
�������ļ�LOG_FILE_NAME�У���¼��־��Ϣ
*/
class CLLogger
{
public:
	static CLLogger* GetInstance();
	CLStatus WriteLog(const char *pstrMsg, long lErrorCode);

private:
	CLLogger(const CLLogger&);
	CLLogger& operator=(const CLLogger&);

	CLLogger();
	~CLLogger();

	int m_Fd;
	static CLLogger *m_pLog;
};

#endif