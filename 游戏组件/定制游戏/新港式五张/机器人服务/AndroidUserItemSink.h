#ifndef ANDROID_USER_ITEM_SINK_HEAD_FILE
#define ANDROID_USER_ITEM_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "GameLogic.h"

//////////////////////////////////////////////////////////////////////////



//��Ϸ�Ի���
class CAndroidUserItemSink : public IAndroidUserItemSink
{


	//��ע��Ϣ
protected:
	LONG							m_lCellScore;						//��Ϸ��ע
	LONGLONG						m_lServiceCharge;					//�����
	LONGLONG						m_lDrawMaxScore;					//�����ע
	LONGLONG						m_lTurnMaxScore;					//�����ע
	LONGLONG						m_lTurnLessScore;					//��С��ע
	LONGLONG						m_lUserScore[GAME_PLAYER];			//�û���ע
	LONGLONG						m_lTableScore[GAME_PLAYER];			//������ע

	//״̬����
protected:
	BYTE							m_cbCurRound;						//��ǰ����
	bool							m_bShowHand;						//�����־
	WORD							m_wCurrentUser;						//��ǰ�û�
	BYTE							m_cbPlayStatus[GAME_PLAYER];		//��Ϸ״̬
	WORD							m_wBetCount;						//��ע����

	//�û��˿�
protected:
	BYTE							m_cbHandCardData[GAME_PLAYER][MAX_COUNT];						

	//�ؼ�����
public:
	IAndroidUserItem *				m_pIAndroidUserItem;					//�û��ӿ�
	CGameLogic						m_GameLogic;							//��Ϸ�߼�

	//��ȡ����
	LONGLONG						m_lRobotScoreMin;					// ���ֵ���ȡ��
	LONGLONG						m_lRobotScoreMax;					// ���ָ��ڴ��
	LONGLONG						m_lRobotBankTake[2];				// ȡ����
	LONGLONG						m_lRobotBankSave;					// �����
	
	//��������
public:
	//���캯��
	CAndroidUserItemSink();
	//��������
	virtual ~CAndroidUserItemSink();

	//�����ӿ�
public:
	//�ͷŶ���
	virtual VOID Release() { delete this; }
	//�ӿڲ�ѯ
	virtual void * QueryInterface(const IID & Guid, DWORD dwQueryVer);

	//���ƽӿ�
public:
	//��ʼ�ӿ�
	virtual bool Initialization(IUnknownEx * pIUnknownEx);
	//���ýӿ�
	virtual bool RepositionSink();

	//��Ϸ�¼�
public:
	//ʱ����Ϣ
	virtual bool OnEventTimer(UINT nTimerID);
	//��Ϸ��Ϣ
	virtual bool OnEventGameMessage(WORD wSubCmdID, void * pData, WORD wDataSize);
	//��Ϸ��Ϣ
	virtual bool OnEventFrameMessage(WORD wSubCmdID, void * pData, WORD wDataSize);
	//������Ϣ
	virtual bool OnEventSceneMessage(BYTE cbGameStatus, bool bLookonOther, void * pData, WORD wDataSize);

	//�û��¼�
public:
	//�û�����
	virtual void OnEventUserEnter(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser);
	//�û��뿪
	virtual void OnEventUserLeave(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser);
	//�û�����
	virtual void OnEventUserScore(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser);
	//�û�״̬
	virtual void OnEventUserStatus(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser);
	//�û���λ
	virtual void OnEventUserSegment(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser);

	//��Ϣ����
protected:	
	//��Ϸ��ʼ
	bool OnSubGameStart(const void * pBuffer, WORD wDataSize);
	//�û���ע
	bool OnSubAddGold(const void * pBuffer, WORD wDataSize);
	//������Ϣ
	bool OnSubSendCard(const void * pBuffer, WORD wDataSize);
	//�û�����
	bool OnSubGiveUp(const void * pBuffer, WORD wDataSize);
	//��Ϸ����
	bool OnSubGameEnd(const void * pBuffer, WORD wDataSize);
	//��Ϸ����
	bool OnSubTrueEnd(const void * pBuffer, WORD wDataSize);

private:
	//��ȡ����
	void ReadConfigInformation();
	// ��������
	void CalculateRank( OUT WORD wRank[GAME_PLAYER], IN BYTE cbCardData[GAME_PLAYER][MAX_COUNT], IN BYTE cbCardCount );
};

//////////////////////////////////////////////////////////////////////////

#endif
