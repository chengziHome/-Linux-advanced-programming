#ifndef CLMutex_H
#define CLMutex_H

#include <pthread.h>
#include "CLStatus.h"

class CLMutex
{
public:
	friend class CLConditionVariable;
	
	/*
	���캯����������������ʱ�����׳��ַ��������쳣
	*/
	CLMutex();
	//��Ҫ��֤pMutexָ��Ļ������Ѿ�����ʼ����
	explicit CLMutex(pthread_mutex_t *pMutex);
	virtual ~CLMutex();

	CLStatus Lock();
	CLStatus Unlock();

private:
	CLMutex(const CLMutex&);
	CLMutex& operator=(const CLMutex&);

private:
	pthread_mutex_t *m_pMutex;
	bool m_bNeededDestroy;
};

#endif