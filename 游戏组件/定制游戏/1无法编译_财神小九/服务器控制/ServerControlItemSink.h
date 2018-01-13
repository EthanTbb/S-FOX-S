#pragma once
#include "../��Ϸ������/ServerControl.h"
#include "../��Ϸ������/GameLogic.h"

#define INDEX_BANKER				1
#define CONTROL_AREA				4
//#define MAX_CARD					5
#define MAX_CARDGROUP				4

class CServerControlItemSink : public IServerControl
{
	//��ҿ���						
protected:
	BYTE						m_cbExcuteTimes;					//ִ�д���
	BYTE						m_cbControlArea[MAX_INDEX];			//��������

	//������Ϣ
protected:
	CGameLogic					m_GameLogic;						//��Ϸ�߼�

public:
	CServerControlItemSink(void);
	virtual ~CServerControlItemSink(void);

public:
	//�����Ƿ��ܳɹ�
	bool IsSettingSuccess(BYTE cbControlArea[MAX_INDEX]);

public:
	//����������
	virtual bool  ServerControl(WORD wSubCmdID, const void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem, ITableFrame * pITableFrame);

	//��Ҫ����
	virtual bool  NeedControl();

	//��ɿ���
	virtual bool  CompleteControl();

	//��ʼ����
	virtual void  GetSuitResult( BYTE cbTableCardArray[][MAX_CARD], BYTE cbTableCard[], LONGLONG lAllJettonScore[] );

	//��Ӯ����
protected:

	//�������
	bool GetSuitCardCombine(BYTE cbStack[]);

	//ׯ����Ӯ
	LONGLONG GetBankerWinScore(int nWinMultiple[]);

};
