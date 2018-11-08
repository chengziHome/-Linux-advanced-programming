#ifndef CLMessage_H
#define CLMessage_H

/*
�û��ɶ����Լ�����Ϣ�����Ӹ�������
CLMessage��������Ӧ�Ӷ��з��䣬����Ϣ���������غ󣬸���Ϣ�ᱻ�Զ�ɾ���������ص���delete
*/
class CLMessage {
public:
    CLMessage(unsigned long lMsgID);

    virtual ~CLMessage();

public:
    const unsigned long &m_clMsgID;

private:
    CLMessage(const CLMessage &);

    CLMessage &operator=(const CLMessage &);

protected:
    unsigned long m_lMsgID;
};

#endif