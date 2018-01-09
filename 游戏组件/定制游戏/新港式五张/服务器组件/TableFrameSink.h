#ifndef TABLE_FRAME_SINK_HEAD_FILE
#define TABLE_FRAME_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "AfxTempl.h"
#include "GameLogic.h"
#include "HistoryScore.h"
#include "ServerControl.h"

//////////////////////////////////////////////////////////////////////////

//�û�׷��
struct tagUserTracing
{
	LONGLONG						lWinScore;								//ʤ������
	LONGLONG						lVarietyScore;							//�仯����
	SYSTEMTIME						SystemTimeTrace;						//��¼ʱ��
};

//��������
typedef CMap<DWORD,DWORD,DWORD,DWORD> CBlackListIDMap;
typedef CMap<DWORD,DWORD,DWORD,DWORD> CUserAddressMap;
typedef CMap<DWORD,DWORD,tagUserTracing *,tagUserTracing * &> CUserTracingMap;

//////////////////////////////////////////////////////////////////////////

//��Ϸ����
class CTableFrameSink : public ITableFrameSink, public ITableUserAction
{
	//��Ϸ����
protected:
	bool							m_bShowHand;						//�����־
	bool							m_bChaosCard;						//�����˿�
	WORD							m_wCurrentUser;						//��ǰ�û�
	LONGLONG						m_lServiceCharge;					//�����
	CString							m_strPalyName[GAME_PLAYER];			//�������

	//��ע��Ϣ
protected:
	LONGLONG						m_lDrawMaxScore;					//�����ע
	LONGLONG						m_lTurnMaxScore;					//�����ע
	LONGLONG						m_lTurnLessScore;					//��С��ע
	LONGLONG						m_lDrawCellScore;					//��ע����

	//�û�״̬
protected:
	BYTE							m_cbPlayStatus[GAME_PLAYER];		//��Ϸ״̬
	BYTE							m_cbOperaScore[GAME_PLAYER];		//������־
	bool							m_bGiveUp[GAME_PLAYER];				//����״̬

	//�����Ϣ
protected:
	LONGLONG						m_lUserScore[GAME_PLAYER];			//�û���ע
	LONGLONG						m_lTableScore[GAME_PLAYER];			//������ע
	LONGLONG						m_lUserMaxScore[GAME_PLAYER];		//�����ע

	//�˿˱���
protected:
	BYTE							m_cbSendCardCount;					//������Ŀ
	BYTE							m_cbCardCount[GAME_PLAYER];			//�˿���Ŀ
	BYTE							m_cbHandCardData[GAME_PLAYER][5];	//�����˿�

	//�������
protected:
	CGameLogic						m_GameLogic;						//��Ϸ�߼�
	CHistoryScore					m_HistoryScore;						//��ʷ�ɼ�

	//�������
protected:
	HINSTANCE						m_hControlInst;
	IServerControl*					m_pServerContro;

	//����ӿ�
protected:
	ITableFrame	*					m_pITableFrame;						//��ܽӿ�
	tagGameServiceOption *			m_pGameServiceOption;				//��������

	//���Ʊ���
protected:
	static bool						g_bControl;							//���ر�־
	static LONGLONG					g_lMaxWinScore;						//���Ӯ��
	static LONGLONG					g_lMaxVarietycore;					//���Ӯ��

	//��������
protected:
	static CUserTracingMap			g_UserTracingMap;					//�û���¼
	static CUserAddressMap			g_UserAddressMap;					//���Ƶ�ַ
	static CBlackListIDMap			g_BlackListIDMap;					//�����û�

	//�����
protected:
	LONGLONG						m_lStorageDeduct;						//�ؿ۱���
	LONGLONG						m_lStorageMax;							//���ⶥ
	LONGLONG						m_nStorageMul;							//ϵͳ��Ǯ����

	bool							m_bAllAndroid;						//ȫ��������
	static LONGLONG					g_lStockScore;						// ����Ӯ��
	static LONGLONG					g_lRobotScoreMin;					// ���ֵ���ȡ��
	static LONGLONG					g_lRobotScoreMax;					// ���ָ��ڴ��
	static LONGLONG					g_lRobotBankTake[2];				// ȡ����
	static LONGLONG					g_lRobotBankSave;					// �����


	//��������
public:
	//���캯��
	CTableFrameSink();
	//��������
	virtual ~CTableFrameSink();

	//�����ӿ�
public:
	//�ͷŶ���
	virtual VOID Release();
	//�ӿڲ�ѯ
	virtual VOID * QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	//����ӿ�
public:
	//��������
	virtual bool Initialization(IUnknownEx * pIUnknownEx);
	//��λ����
	virtual VOID RepositionSink();

	//��ѯ�ӿ�
public:
	//��ѯ�޶�
	virtual SCORE QueryConsumeQuota(IServerUserItem * pIServerUserItem){return 0;}
	//���ٻ���
	virtual SCORE QueryLessEnterScore(WORD wChairID, IServerUserItem * pIServerUserItem){return 0;}
	//��ѯ�Ƿ�۷����
	virtual bool QueryBuckleServiceCharge(WORD wChairID){return true;}

	//�����ӿ�
public:
	//���û���
	virtual void SetGameBaseScore(LONG lBaseScore){};

public:
	//�����¼�
	virtual bool OnDataBaseMessage(WORD wRequestID, VOID * pData, WORD wDataSize){return false;}
	//�����¼�
	virtual bool OnUserScroeNotify(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason){return false;}

	//��Ϸ�¼�
public:
	//��Ϸ��ʼ
	virtual bool OnEventGameStart();
	//��Ϸ����
	virtual bool OnEventGameConclude(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason);
	//���ͳ���
	virtual bool OnEventSendGameScene(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbGameStatus, bool bSendSecret);

	//�¼��ӿ�
public:
	//��ʱ���¼�
	virtual bool OnTimerMessage(DWORD dwTimerID, WPARAM wBindParam);
	//��Ϸ��Ϣ
	virtual bool OnGameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem);
	//�����Ϣ
	virtual bool OnFrameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem);

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
	virtual bool OnActionUserOnReady(WORD wChairID, IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize) { return true; }

	//��Ϸ�¼�
protected:
	//�û�����
	bool OnUserGiveUp(WORD wChairID);
	//�û���ע
	bool OnUserAddScore(WORD wChairID, LONGLONG lScore);
	//��ȡʤ��
	bool OnUserGetWinner(IServerUserItem * pIServerUserItem);

	//��������
protected:
	//������ע
	VOID RectifyMaxScore();
	//�����˿�
	VOID DispatchUserCard();

	//���غ���
protected:
	//�����˿�
	VOID ControlUserCard();
	//�����¼�
	VOID OnEventScoreAlter(DWORD dwUserID, LONGLONG lVarietyScore);

	//�߼�����
protected:
	//�ƶ�����
	WORD EstimateLoser(BYTE cbStartPos, BYTE cbConcludePos);
	//�ƶ�ʤ��
	WORD EstimateWinner(BYTE cbStartPos, BYTE cbConcludePos);
	//���Ϳ��
	void SendStorage();
};

//////////////////////////////////////////////////////////////////////////

#endif