#pragma once
#include "../��Ϸ������/ServerControl.h"


class CServerControlItemSink : public IServerControl
{
	//���Ʋ���
protected:
	BYTE							m_cbControlArea;						//��������
	BYTE							m_cbControlTimes;						//���ƴ���


public:
	CServerControlItemSink(void);
	virtual ~CServerControlItemSink(void);


public:
	//����������
	virtual bool  ServerControl(WORD wSubCmdID, const void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem, ITableFrame * pITableFrame);

	//��Ҫ����
	virtual bool  NeedControl();

	//�������
	virtual bool  MeetControl(tagControlInfo ControlInfo);

	//��ɿ���
	virtual bool  CompleteControl();

	//���ؿ�������
	virtual bool  ReturnControlArea(tagControlInfo& ControlInfo);


};
