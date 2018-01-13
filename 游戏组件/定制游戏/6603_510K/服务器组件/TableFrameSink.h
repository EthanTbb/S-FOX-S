#ifndef TABLE_FRAME_SINK_HEAD_FILE
#define TABLE_FRAME_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "AfxTempl.h"
#include "GameLogic.h"

//////////////////////////////////////////////////////////////////////////////////

//��Ϸ����
class CTableFrameSink : public ITableFrameSink, public ITableUserAction
{
	//��������
protected:
	DWORD							m_dwPlayTimeCount;					//��Ϸʱ��

	//�뿪����
protected:
	BYTE							m_cbResponses[GAME_PLAYER];			//��Ӧ״̬
	WORD							m_wRequestTimes[GAME_PLAYER];		//�����뿪

	//��Ϸ����
protected:
	WORD							m_wHeadUser;						//�׳��û�
	WORD							m_wCurrentUser;						//��ǰ���
	BYTE							m_cbContinue[GAME_PLAYER];			//������־
	WORD							m_wPersistInfo[GAME_PLAYER][2];		//��Ϸ��Ϣ
	bool							m_bAutoPlayer[4];	
	LONGLONG						m_TurnScore;
	LONGLONG						m_PlayerScore[4];
	BYTE							m_510KCardRecord[3][8];				//510K����
	bool							m_bFirstUserLastTurn;				//ͷ��������һ�֡��������ַ�...��rz

	//��Ϸ�ȼ�
protected:
	WORD							m_wOrderIndex;						//�ȼ�����
	BYTE							m_cbMainValue;						//������ֵ
	BYTE							m_cbValueOrder[GAME_PLAYER];		//�ȼ���ֵ

	//ʤ����Ϣ
protected:
	WORD							m_wWinCount;						//ʤ������
	WORD							m_wWinOrder[GAME_PLAYER];			//ʤ���б�

	//������Ϣ
protected:
	WORD							m_wTurnWiner;						//ʤ�����
	BYTE							m_cbTurnCardType;					//�˿�����
	BYTE							m_cbTurnCardCount;					//������Ŀ
	BYTE							m_cbTurnCardData[MAX_COUNT];		//��������
	BYTE							m_cbMagicCardData[MAX_COUNT];		//����˿�

	//�˿���Ϣ
protected:
	BYTE							m_cbHandCardCount[GAME_PLAYER];		//�˿���Ŀ
	BYTE							m_cbHandCardData[GAME_PLAYER][MAX_COUNT];	//�����˿�

	//�������
protected:
	CGameLogic						m_GameLogic;						//��Ϸ�߼�

	//����ӿ�
protected:
	ITableFrame	*					m_pITableFrame;						//��ܽӿ�
	//ITableFrameControl *			m_pITableFrameControl;
	const tagGameServiceOption		* m_pGameServiceOption;				//���ò���

	//���Ա���
protected:
	static const WORD				m_wPlayerCount;						//��Ϸ����

	//��������
public:
	//���캯��
	CTableFrameSink();
	//��������
	virtual ~CTableFrameSink();

	//�����ӿ�
public:
	//�ͷŶ���
	virtual VOID Release() { delete this; }
	//�ӿڲ�ѯ
	virtual VOID * QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	//����ӿ�
public:
	//��λ����
	virtual VOID RepositionSink();
	//��������
	virtual bool Initialization(IUnknownEx * pIUnknownEx);
	//�����ӿ�
public:
	//���û���
	virtual void SetGameBaseScore(LONG lBaseScore){};
	//��ѯ�ӿ�
public:
	//��ѯ�޶�
	virtual LONGLONG QueryConsumeQuota(IServerUserItem * pIServerUserItem);
	//���ٻ���
	virtual LONGLONG QueryLessEnterScore(WORD wChairID, IServerUserItem * pIServerUserItem);
	//��ѯ�Ƿ�۷����
	virtual bool QueryBuckleServiceCharge(WORD wChairID){return true;}
	//��Ϸ�¼�
public:
	//��Ϸ��ʼ
	virtual bool OnEventGameStart();
	//��Ϸ����
	virtual bool OnEventGameConclude(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason);
	//���ͳ���
	virtual bool OnEventSendGameScene(WORD wChiarID, IServerUserItem * pIServerUserItem, BYTE bGameStatus, bool bSendSecret);

	//�¼��ӿ�
public:
	//��ʱ���¼�
	virtual bool OnTimerMessage(DWORD dwTimerID, WPARAM dwBindParameter);
	//�����¼�
	virtual bool OnDataBaseMessage(WORD wRequestID, VOID * pData, WORD wDataSize);
	//�����¼�
	virtual bool OnUserScroeNotify(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason);

	//����ӿ�
public:
	//��Ϸ��Ϣ����
	virtual bool OnGameMessage(WORD wSubCmdID,VOID * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem);
	//�����Ϣ����
	virtual bool OnFrameMessage(WORD wSubCmdID,VOID * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem);

	//�����¼�
public:
	//�û�����
	virtual bool OnActionUserOffLine(WORD wChairID, IServerUserItem * pIServerUserItem){ return true; }
	//�û�����
	virtual bool OnActionUserConnect(WORD wChairID, IServerUserItem * pIServerUserItem){ return true; };
	//�û�����
	virtual bool OnActionUserSitDown(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser);
	//�û�����
	virtual bool OnActionUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser);
	//�û�ͬ��
	virtual bool OnActionUserOnReady(WORD wChairID, IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize){ return true; }

	//��Ϸ�¼�
protected:
	//�û�����
	bool OnUserOutCard(WORD wChairID, BYTE cbCardData[], BYTE cbCardCount);
	//�û�����
	bool OnUserPassCard(WORD wChairID);
	//������Ϸ
	bool OnUserContinueGame(WORD wChairID);

protected:
	//�Ƿ��з�
	bool IsAllUserHaveScore(BYTE bCardData[GAME_PLAYER][MAX_COUNT]);
	//���öԼ�
	void SearchFriend(BYTE bFirstCard,const BYTE bHandCardData[4][28]);
	//ת������
	CString TranslateCardText(BYTE bCardData);
};

//////////////////////////////////////////////////////////////////////////////////

#endif