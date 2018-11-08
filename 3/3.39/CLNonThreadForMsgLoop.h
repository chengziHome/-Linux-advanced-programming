#ifndef CLNonThreadForMsgLoop_H
#define CLNonThreadForMsgLoop_H

#include "CLStatus.h"

class CLMessageObserver;
class CLExecutiveFunctionProvider;

/*
�����������߳�ֱ�ӽ�����Ϣѭ���������Ǵ������߳�
*/
class CLNonThreadForMsgLoop
{
public:
	/*
	pMsgObserverӦ�Ӷ��з��䣬�Ҳ��ص���delete��pstrThreadName��������߳����Ʊ�����Ψһ��
	*/
	CLNonThreadForMsgLoop(CLMessageObserver *pMsgObserver, const char *pstrThreadName);
	virtual ~CLNonThreadForMsgLoop();

	CLStatus Run(void *pContext);

private:
	CLNonThreadForMsgLoop(const CLNonThreadForMsgLoop&);
	CLNonThreadForMsgLoop& operator=(const CLNonThreadForMsgLoop&);

private:
	CLExecutiveFunctionProvider *m_pFunctionProvider;
};

#endif