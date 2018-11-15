#ifndef CLSTATUS_H
#define CLSTATUS_H

/*
���ڱ��溯���Ĵ�����
*/
class CLStatus
{
public:
	/**
	 * 业务出错不一定有系统错误码，但是系统错误一定会导致业务失败
	 * @param lReturnCode	你自己业务的返回麻
	 * @param lErrorCode 	系统错误的返回码
	 */
	CLStatus(long lReturnCode, long lErrorCode);
	CLStatus(const CLStatus& s);
	virtual ~CLStatus();

public:
	bool IsSuccess();

public:
	const long& m_clReturnCode;
	const long& m_clErrorCode;

private:
	long m_lReturnCode;
	long m_lErrorCode;
};

#endif
