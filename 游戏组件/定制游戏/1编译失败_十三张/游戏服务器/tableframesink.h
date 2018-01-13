#ifndef TABLE_FRAME_SINK_HEAD_FILE
#define TABLE_FRAME_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "DlgCustomRule.h"
#include "..\��Ϣ����\CMD_Thirteen.h"
#include "..\��Ϸ�ͻ���\GameLogic.h"
#include "..\��Ϸ�ͻ���\HistoryScore.h"
#include "ServerControl.h"
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////

//��Ϸ������
class CTableFrameSink : public ITableFrameSink, public ITableUserAction
{
	//�������
protected:
	CGameLogic							m_GameLogic;										//��Ϸ�߼�
	ITableFrame							*m_pITableFrame;									//��ܽӿ�
	const tagGameServiceOption			*m_pGameServiceOption;								//���ò���
	tagCustomRule*						m_pGameCustomRule;									//�Զ�����
	CHistoryScore						m_HistoryScore;										//��ʷ�ɼ�
	BYTE								m_cbWaitTime;										//�ȴ�ʱ��
	//��Ϸ����
protected:
	SCORE								m_lUserChip[GAME_PLAYER];							//�û���ע
	SCORE								m_lScoreTimes[GAME_PLAYER][3];						//��Ӯע��
	BYTE								m_bSpecialType[GAME_PLAYER];						//��������
	BYTE								m_bUserCardData[GAME_PLAYER][HAND_CARD_COUNT];		//����˿�
	BYTE								m_bSegmentCard[GAME_PLAYER][3][5];					//�ֶ��˿�
    	
	WORD								m_wFirstCallBankerUser ;							//��ׯ���
	WORD								m_wBanker;											//ׯ�����
	WORD								m_wCurrentCaller ;									//��ǰ��ׯ	
	
	WORD								m_wSetChipCount;									//��ע����
	WORD								m_wShuffleFinishCount;								//ϴ�Ƽ���
	WORD								m_wDispatchFinishCount;								//���Ƽ���
	WORD								m_wSpliFinishCount;									//���Ƽ���	
	WORD								m_wShowCardCount;									//̯�Ƽ���
	WORD								m_wCompareCount;									//�Ƚϼ���
	WORD								m_wCompleteCompareCount ;							//�Ƚϼ���
	
	bool								m_bFinishSegment[GAME_PLAYER];						//��ɷֶ�
	bool								m_bGameStatus[GAME_PLAYER];							//��Ϸ״̬
	bool								m_bIsReady[GAME_PLAYER];							//�Ѿ�׼��
	bool								m_bCompared;										//�Ƿ����
	//���Ʊ���
protected:
	bool								m_bNeedControl;										//��Ҫ����
	bool								m_bMysteryUser[GAME_PLAYER];						//�������

	//�������
protected:
	HINSTANCE							m_hControlInst;
	IServerControl*						m_pServerControl;

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
	//�Ƿ���Ч
	virtual bool  IsValid() { return AfxIsValidAddress(this,sizeof(CTableFrameSink))?true:false; }
	//�ӿڲ�ѯ
	virtual VOID * QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	//����ӿ�
public:
	//��λ����
	virtual VOID RepositionSink();
	//��������
	virtual bool Initialization(IUnknownEx * pIUnknownEx);

	//��ѯ�ӿ�
public:
	//��ѯ�޶�
	virtual SCORE QueryConsumeQuota(IServerUserItem * pIServerUserItem);
	//���ٻ���
	virtual SCORE QueryLessEnterScore(WORD wChairID, IServerUserItem * pIServerUserItem);
	//��ѯ�Ƿ�۷����
	virtual bool QueryBuckleServiceCharge(WORD wChairID){return true;}

	//ͨ�ú���
public:
	//ͳ������
	DWORD GetPlayerCount();
	//��ȡ����
	void CompareCardGetTimes();
	//��ʼ����
	VOID StartCompareCard();
	//��ȡ����
	int GetCardTimes(WORD wUserID);
	//��ȡ��ע
	void GetChips( LONGLONG lChipArray[GAME_PLAYER][SET_CHIP_COUNT]);
	//��ȡ��ע
	void GetChips(WORD wChairID, LONGLONG lChipArray[SET_CHIP_COUNT]);

	//��Ϣ����
protected:
	//��ҽ�ׯ
	bool OnUserCallBanker(const WORD wChairID , const void * pDataBuffer, WORD wDataSize) ;
	//��Ҳ���
	bool OnUserNoCallBanker(const WORD wChairID , const void * pDataBuffer, WORD wDataSize) ;
	//�����ע
	bool OnUserSetChip(const WORD wChairID , const void * pDataBuffer, WORD wDataSize) ;
	//��ҷֶ�
	bool OnUserSetSegment(const WORD wChairID , const void * pDataBuffer, WORD wDataSize) ;
	//���̯��
	bool OnUserShowCard(const WORD wChairID , const void * pDataBuffer, WORD wDataSize) ;

	//��Ϸ�¼�
public:
	//��Ϸ��ʼ
	virtual bool OnEventGameStart();
	//��Ϸ����
	virtual bool OnEventGameConclude(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason);
	//���ͳ���
	virtual bool OnEventSendGameScene(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbGameStatus, bool bSendSecret);
	//�����û�
	bool OnCheatCard(IServerUserItem * pIServerUserItem);
	//����������Ϣ
	void SendCheatCard();

	//�¼��ӿ�
public:
	//ʱ���¼�
	virtual bool OnTimerMessage(DWORD wTimerID, WPARAM wBindParam);
	//�����¼�
	virtual bool OnDataBaseMessage(WORD wRequestID, VOID * pData, WORD wDataSize);
	//�����¼�
	virtual bool OnUserScroeNotify(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason);

	//����ӿ�
public:
	//��Ϸ��Ϣ
	virtual bool OnGameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem);
	//�����Ϣ
	virtual bool OnFrameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem);

	//////////////////////////////////////////////////////////////////////////
	//�û��¼�
public:
	//�û�����
	virtual bool OnActionUserOffLine(WORD wChairID, IServerUserItem * pIServerUserItem) { return true; }
	//�û�����
	virtual bool OnActionUserConnect(WORD wChairID, IServerUserItem * pIServerUserItem) { return true; }
	//�û�����
	virtual bool OnActionUserSitDown(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser);
	//�û�����
	virtual bool OnActionUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser);
	//�û�ͬ��
	virtual bool OnActionUserOnReady(WORD wChairID, IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize);
	//���û���
	virtual void SetGameBaseScore(LONG lBaseScore){};

public:
	BOOL  m_bIsWin;                                        //�������Ƿ���ҪӮǮ
	SCORE m_sRegionalControl[8];                           //��������ֵ
	SCORE m_sRobotLnventory;                               //������ӮǮ���
	BYTE  m_bRegionalProbability[9];                       //����������ӮǮ����
    
	//������ת��Ϊ���͵÷�
	int   CardTypeToScore(BYTE bCardType,BYTE bSpecies);
	//�Ի����˽�����Ӯ����
	void  ResultsControl(int iCount);
	//�жϻ�������Ӯ���
	void  JudgeResults();
	//��ȡ�����ļ�
	void ReadConfiguration();
	//�Ƿ����
	bool IsNeedControl();
	//�����Ϣ
	VOID SendControlMessage(IServerUserItem * pIServerUserItem);
	//������Ϣ
	void SendInfo(CString str, WORD wChairID = INVALID_CHAIR);
};

//////////////////////////////////////////////////////////////////////////

#endif
