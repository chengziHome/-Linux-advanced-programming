#ifndef CLLogger_H
#define CLLogger_H

#include <pthread.h>
#include "CLStatus.h"

/*
�������ļ�LOG_FILE_NAME�У���¼��־��Ϣ
*/
class CLLogger
{
public:
	static CLLogger* GetInstance();
	static CLStatus WriteLogMsg(const char *pstrMsg, long lErrorCode);

	CLStatus WriteLog(const char *pstrMsg, long lErrorCode);
	CLStatus Flush();

	friend class CLLibExecutiveInitializer;

private:
	static CLStatus Destroy();
	static CLStatus Create();

private:
	CLStatus WriteMsgAndErrcodeToFile(const char *pstrMsg, const char *pstrErrcode);

private:
	CLLogger(const CLLogger&);
	CLLogger& operator=(const CLLogger&);

	CLLogger();
	~CLLogger();

private:
	static CLLogger *m_pLog;
	//����߳���д��־
	//��һ���߳��ڵ���Destroy����һ���߳�����ͼд��־
	static pthread_mutex_t m_Mutex;

private:
	int m_Fd;
	char *m_pLogBuffer;
	unsigned int m_nUsedBytesForBuffer;
};

#endif