#pragma once
#include "../��Ϸ������/ServerControl.h"
#include "../��Ϸ������/GameLogic.h"

#define INDEX_BANKER				1
#define CONTROL_AREA				4
//#define MAX_CARD					5
#define MAX_CARDGROUP				5


class CServerControlItemSink : public IServerControl
{
	//��ҿ���						
protected:
	BYTE							m_cbExcuteTimes;				//ִ�д���
	BYTE							m_cbControlStyle;				//���Ʒ�ʽ
	BYTE                            m_cbEnemyChairID;                //�Լ�ID
	//������Ϣ
protected:
	CGameLogic						m_GameLogic;					//��Ϸ�߼�

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

	//��ʼ����
	virtual void  GetSuitResult( BYTE cbTableCardArray[][2]);

};
