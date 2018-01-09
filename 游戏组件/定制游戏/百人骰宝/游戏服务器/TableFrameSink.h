#ifndef TABLE_FRAME_SINK_HEAD_FILE
#define TABLE_FRAME_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "GameLogic.h"
#include "RobotManage.h"
#include "afxtempl.h"

#define MAX_CARD					2
#define MAX_CARDGROUP				4
#define CONTROL_AREA				3

//////////////////////////////////////////////////////////////////////////
//��ʷ��¼
#define MAX_SCORE_HISTORY			16									//��ʷ����
//////////////////////////////////////////////////////////////////////////

//��Ϸ������
class CTableFrameSink : public ITableFrameSink, public ITableUserAction
{
	//��ע��Ϣ
protected:
	SCORE						m_lAllUserBet[AREA_COUNT];					//ȫ����ע
	SCORE						m_lAndroidBet[AREA_COUNT];					//��������ע
	SCORE						m_lFleeUserBet[AREA_COUNT];					//���������ע
	SCORE						m_lUserAllBet[AREA_COUNT][GAME_PLAYER];		//������ע
	SCORE						m_lUserStartScore[GAME_PLAYER];		//��ʼ����
	LONGLONG					m_lQueryGameID;						//��ѯID
	BYTE						m_cbUserWinLose;					//���������Ӯ״̬��0Ϊ��Ч��1ΪӮ��2Ϊ��
	bool						m_bControlWin;						//���Ƶ��������Ӯ
	LONGLONG					m_lControlStorage;						//���ƺ�����д��
	TCHAR						m_szControlName[LEN_NICKNAME];			//��������
	
	//���Ʊ���
protected:
	SCORE						m_lAreaLimitScore;							//��������
	SCORE						m_lUserLimitScore;							//��������
	SCORE						m_lApplyBankerCondition;					//��������
	INT							m_nEndGameMul;								//��ǰ���ưٷֱ�

	//��ׯ����	
	int								m_nBankerTimeLimit;						//��������
	int								m_nBankerTimeAdd;						//���Ӵ��� (��Ҵ����������ʱ)
	LONGLONG						m_lExtraBankerScore;					//�������� (���ڴ�ֵʱ������������������ׯ)
	int								m_nExtraBankerTime;						//�������
	WORD							m_wAddTime;								//������ׯ 
	bool							m_bExchangeBanker;						//�任ׯ��
	
	TCHAR						m_szConfigFileName[MAX_PATH];				//�����ļ�
	LONGLONG                    m_lCurSystemWinScore;						//ϵͳӮ��

	//��ҳɼ�
protected:
	bool						m_bContiueCard;								//��������
	BYTE						m_bcFirstPostCard;							//���°l���c��
	BYTE						m_cbLeftCardCount;							//�˿���Ŀ
	LONG						m_lUserRevenue[GAME_PLAYER];				//���˰��
	SCORE						m_lUserWinScore[GAME_PLAYER];				//��ҳɼ�
	SCORE						m_lUserReturnScore[GAME_PLAYER];			//������ע
	BYTE						m_bWinFlag[AREA_COUNT];						//��Ӯ��Ϣ (������)						

	//�˿���Ϣ
protected:
	BYTE						m_cbDiceValue[DICE_COUNT];

	//ʱ������
public:
	DWORD						m_dwTimeGameFree;							//����ʱ��
	DWORD						m_dwTimeGameCard;							//����ʱ��
	DWORD						m_dwTimeGameBet;							//��עʱ��
	DWORD						m_dwTimeGameEnd;							//����ʱ��	
	CString						m_strAppName;								//�ļ�����

	//״̬����
protected:
	DWORD						m_dwStartTime;								//��ʼʱ��

	//��ҿ���						
protected:
	BYTE						m_cbControlStyle;							//���Ʒ�ʽ
	bool						m_bWinArea[CONTROL_AREA];					//Ӯ������
	BYTE						m_cbWinAreaCount;
	BYTE						m_cbReqType;								//��ǰ��������
	//������
	int							m_nStorageDeduct;							//���˥�� 
	SCORE						m_lStorageStart;							//�����ֵ
	SCORE						m_lStorageCurrent;							//��ǰ���

	// 11/11/2013 [Wan.MY]
	LONGLONG						m_lStorageMax1;							//���ⶥ1
	LONGLONG						m_lStorageMul1;							//ϵͳ��Ǯ����
	LONGLONG						m_lStorageMax2;							//���ⶥ2
	LONGLONG						m_lStorageMul2;							//ϵͳ��Ǯ����

	//�����˿���
	int								m_nMaxChipRobot;						//�����Ŀ (��ע������)
	int								m_nChipRobotCount;						//����ͳ�� (��ע������)
	bool							m_bRobotChip[GAME_PLAYER];					//�������Ƿ���ע
	LONGLONG						m_lRobotAreaLimit;						//����ͳ�� (������)
	LONGLONG						m_lRobotAreaScore[AREA_COUNT];		//����ͳ�� (������)
	int								m_nRobotListMaxCount;					//�������

	//ׯ����Ϣ
protected:
	WORD						m_wBankerTime;								//��ׯ����
	WORD						m_wCurrentBanker;							//��ǰׯ��
	bool						m_bEnableSysBanker;							//ϵͳ��ׯ
	SCORE						m_lBankerScore;								//���ֽ��						
	SCORE						m_lBankerWinScore;							//�ۼƳɼ�
	SCORE						m_lBankerCurGameScore;						//��ǰ�ɼ�
	CWHArray<WORD>				m_ApplyUserArray;							//�������

	//���Ʊ���
protected:
	BYTE						m_cbWinSideControl;							//������Ӯ
	int							m_nSendCardCount;							//���ʹ���

	bool                        m_cbNeedControl;							//״̬����
	BYTE						m_cbControlDiceValue[DICE_COUNT];
	BYTE						m_cbControlTotal;							//�����ܵ���

	//��¼����
protected:
	int							m_nRecordFirst;								//��ʼ��¼
	int							m_nRecordLast;								//����¼
	CFile						m_FileRecord;								//��¼���
	DWORD						m_dwRecordCount;							//��¼��Ŀ
	tagGameRecord				m_GameRecordList[MAX_SCORE_HISTORY];		//��Ϸ��¼
	
	//�������
protected:
	CGameLogic						m_GameLogic;							//��Ϸ�߼�
	CRobotConfigManage				m_RobotCfgManage;						//��������
	ITableFrame						* m_pITableFrame;						//��ܽӿ�
	//ITableFrameControl				* m_pITableFrameControl;			//��ܽӿ�
	const tagGameServiceOption		* m_pGameServiceOption;					//���ò���

	//���Ա���
protected:
	static const WORD				m_wPlayerCount;							//��Ϸ����
	
	//��������
public:
	//���캯��
	CTableFrameSink();
	//��������
	virtual ~CTableFrameSink();

	//�����ӿ�
public:
	//�ͷŶ���
	virtual VOID  Release() { }
	
	//�ӿڲ�ѯ
	virtual VOID *  QueryInterface(const IID & Guid, DWORD dwQueryVer);


	//����ӿ�
public:
	//��ʼ��
	virtual bool Initialization(IUnknownEx * pIUnknownEx);
	//��λ����
	virtual VOID RepositionSink();

	//��ѯ�ӿ�
public:
	//��ѯ�޶�
	virtual SCORE QueryConsumeQuota(IServerUserItem * pIServerUserItem);
	//���ٻ���
	virtual SCORE QueryLessEnterScore(WORD wChairID, IServerUserItem * pIServerUserItem){return 0;}
	//�����¼�
	virtual bool OnDataBaseMessage(WORD wRequestID, VOID * pData, WORD wDataSize){return false;}
	//�����¼�
	virtual bool OnUserScroeNotify(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason){return false;}
	//��ѯ�Ƿ�۷����
	virtual bool QueryBuckleServiceCharge(WORD wChairID);
	//��Ϣ�ӿ�
public:
	
	//��Ϸ״̬
	virtual bool  IsUserPlaying(WORD wChairID);


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
	virtual bool OnTimerMessage(DWORD wTimerID, WPARAM wBindParam);
	//��Ϸ��Ϣ����
	virtual bool OnGameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem);
	//�����Ϣ����
	virtual bool OnFrameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem);
	//�����¼�
	virtual bool OnGameDataBase(WORD wRequestID, VOID * pData, WORD wDataSize);


	//�����¼�
public:
	//�û�����
	virtual bool OnActionUserOffLine(WORD wChairID,IServerUserItem * pIServerUserItem);
	//�û�����
	virtual bool OnActionUserConnect(WORD wChairID,IServerUserItem * pIServerUserItem) { return true; }
	//�û�����
	virtual bool OnActionUserSitDown(WORD wChairID,IServerUserItem * pIServerUserItem, bool bLookonUser);
	//�û�����
	virtual bool OnActionUserStandUp(WORD wChairID,IServerUserItem * pIServerUserItem, bool bLookonUser);
	//�û�ͬ��
	virtual bool OnActionUserOnReady(WORD wChairID,IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize) { return true; }

	//�����ӿ�
public:
	//���û���
	virtual void SetGameBaseScore(LONG lBaseScore){};


	//��Ϸ�¼�
protected:
	//��ע�¼�
	bool OnUserPlaceJetton(WORD wChairID, BYTE cbJettonArea, SCORE lJettonScore);
	//����ׯ��
	bool OnUserApplyBanker(IServerUserItem *pIApplyServerUserItem);
	//ȡ������
	bool OnUserCancelBanker(IServerUserItem *pICancelServerUserItem);
	//����Ա����
	bool OnSubAmdinCommand(IServerUserItem*pIServerUserItem,const VOID*pDataBuffer);

	//��������
private:
	bool FindSuitBanker();
	//����ׯ��
	bool ChangeBanker(bool bCancelCurrentBanker);
	//����ׯ��
	VOID SendApplyUser(IServerUserItem *pServerUserItem);
	//���ͼ�¼
	VOID SendGameRecord(IServerUserItem *pIServerUserItem);
	//������Ϣ
	VOID SendGameMessage(WORD wChairID, LPCTSTR pszTipMsg);
	//�������
	bool PlayerFlee(WORD wChairID,IServerUserItem*pIServerUserItem);
	//��ׯ���� (������)
	VOID RobotBankerControl();
	//���ÿ��� (������)
	VOID RobotConfigControl();
	//������ע��Ϣ
	void SendUserBetInfo( IServerUserItem *pIServerUserItem );
	//�ж������Ӯ״̬
	BYTE ControlUserWin();

	//��ע����
private:
	//����ֽ��
	VOID ChangeMaxUser(bool blMin=false);
	//�����ע
	SCORE GetUserMaxJetton(WORD wChairID,BYTE cbAreaID);
	//����ֽ��
	VOID RandList(BYTE cbCardBuffer[], BYTE cbBufferCount);

	//��ʱ�¼�
protected:
	//��ʼ��ע
	bool OnTimerStartBet();
	//��ʾ���
	bool OnTimerShowResult();
	//��Ϸ����
	bool OnTimerGameEnd();
//����ƽ��
private:	
	//��ȡ����
	void GetStockScore(SCORE *lAndroidScore,SCORE *lUserScore,BYTE bDice[]);
	//��ȡ����
	void ReadConfigInformation();

	//��Ϸͳ��
private:
	//����÷�
    SCORE CalculateScore(bool bProbeJudgeScore);
	//ֽ��ת��
	CString TransformCardInfo( BYTE cbCardData ); 
	//ת������
	bool TransFromCardStyle(BYTE cbIndex,CString &str);	
	//���·���
	bool DispatchTableLabel();
	//��̽���ж�
	bool ProbeJudge();
	//д���
	void WriteStorage(LONGLONG lSystemScore);
	//ƽ����ע
	void BalanceCurBetScore(LONGLONG lAllUserBet[],int nCount);
	//ׯ����С�÷�
	SCORE GetBankerWinMinScore(BYTE cbAreaID);
	// ��¼����
	void WriteInfo( LPCTSTR pszFileName, LPCTSTR pszString );
};

//////////////////////////////////////////////////////////////////////////

#endif
