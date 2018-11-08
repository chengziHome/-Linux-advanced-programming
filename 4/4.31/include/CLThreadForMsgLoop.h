#ifndef CLThreadForMsgLoop_H
#define CLThreadForMsgLoop_H

#include "CLStatus.h"

class CLMessageObserver;
class CLThread;

/************************************************************************/
/* CLThreadForMsgLoog��ķ����ͷ����⣬��ʹ���߸���                     */
/* ���ཫ�������̣߳���ʹ���߳̽�����Ϣѭ���У�����Ϣѭ����ʼ����Ϻ�� */
/* Run�����ŷ��ء�														*/
/************************************************************************/
class CLThreadForMsgLoop
{
public:
	/*
	pMsgObserverӦ�Ӷ��з��䣬�Ҳ��ص���delete��pstrThreadName��������߳����Ʊ�����Ψһ��
	Ĭ�������bWaitForDeathΪfalse����Ϊtrue����������������еȴ����߳�����
	*/
	CLThreadForMsgLoop(CLMessageObserver *pMsgObserver, const char *pstrThreadName);
	CLThreadForMsgLoop(CLMessageObserver *pMsgObserver, const char *pstrThreadName, bool bWaitForDeath);
	virtual ~CLThreadForMsgLoop();

	/*
	Run�������۷�����ȷ�������ֻ�ɵ���һ�Ρ�������һ��CLThreadProxy����
	*/
	CLStatus Run(void *pContext);

private:
	CLThreadForMsgLoop(const CLThreadForMsgLoop&);
	CLThreadForMsgLoop& operator=(const CLThreadForMsgLoop&);

private:
	CLThread *m_pThread;
	bool m_bWaitForDeath;
};

#endif