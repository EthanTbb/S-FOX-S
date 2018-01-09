#ifndef TABLE_FRAME_SINK_HEAD_FILE
#define TABLE_FRAME_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "GameLogic.h"
#include "ServerControl.h"

//��س�ʼ
#define INIT_CTRL                   100

//////////////////////////////////////////////////////////////////////////
//��ʷ��¼
#define MAX_SCORE_HISTORY			65						//��ʷ����
//////////////////////////////////////////////////////////////////////////

struct TAndroidUser 
{
	bool blAndroidUser;
	int  PlayCount;
	int  BankCount;
	TAndroidUser()
	{
		memset(this,0,sizeof(*this));
	};
};
struct UserLeft
{
	WORD wChair;
	BOOL blWriteScore;
	LONGLONG WriteScore;

	UserLeft()
	{
		memset(this,0,sizeof(*this));
	}
};
//��Ϸ������
class CTableFrameSink : public ITableFrameSink, public ITableUserAction
{
	//����ע��
protected:
	LONGLONG						m_lAllJettonScore[AREA_ARRY_COUNT];	//ȫ����ע
	
	//������ע
protected:
	LONGLONG						m_lUserJettonScore[AREA_ARRY_COUNT][GAME_PLAYER];//������ע

	//���Ʊ���
protected:
	LONGLONG						m_lAreaLimitScore;						//��������
	LONGLONG						m_lUserLimitScore;						//��������
	LONGLONG						m_lApplyBankerCondition;				//��������
	TCHAR							m_szConfigFileName[MAX_PATH];			//�����ļ�
	bool		                    m_bAndroidApplyBanker[GAME_PLAYER];

	//��ҳɼ�
protected:
	LONGLONG						m_lUserWinScore[GAME_PLAYER];			//��ҳɼ�
	LONGLONG						m_lUserReturnScore[GAME_PLAYER];		//������ע
	LONGLONG						m_lUserRevenue[GAME_PLAYER];			//���˰��
	BYTE							m_cbLeftCardCount;						//�˿���Ŀ
	bool							m_bContiueCard;							//��������
	BYTE							m_bcFirstPostCard;						//���°l���c��

	//�˿���Ϣ
protected:
	BYTE							m_cbCardCount[4];						//�˿���Ŀ
    BYTE							m_cbTableCardArray[4][2];				//�����˿�
	BYTE							m_cbTableCard[CARD_COUNT];				//�����˿�
	BYTE							m_cbWinSideControl;					    //������Ӯ
	TAndroidUser                    m_AndroidUser[GAME_PLAYER];

	//״̬����
protected:
	DWORD							m_dwJettonTime;							//��ʼʱ��

	//ׯ����Ϣ
protected:
	CWHArray<WORD>					m_ApplyUserArray;						//�������
	WORD							m_wCurrentBanker;						//��ǰׯ��
	WORD							m_wBankerTime;							//��ׯ����
	LONGLONG						m_lBankerWinScore;						//�ۼƳɼ�
	LONGLONG						m_lBankerCurGameScore;					//��ǰ�ɼ�
	bool							m_bEnableSysBanker;						//ϵͳ��ׯ


	//���Ʊ���
protected:
	int								m_nSendCardCount;					    //���ʹ���
	bool							m_bBankerLeft;							//ׯ������
	int								m_nRobotBankerApplayMaxCount;			//�б����� (���)

	//��¼����
protected:
	tagServerGameRecord				m_GameRecordArrary[MAX_SCORE_HISTORY];	//��Ϸ��¼
	int								m_nRecordFirst;							//��ʼ��¼
	int								m_nRecordLast;							//����¼
	CFile							m_FileRecord;							//��¼���
	DWORD							m_dwRecordCount;						//��¼��Ŀ

	//���Ǹ���
public:
	BYTE							m_cbStarSelect[3];
	float							m_fStarWinRate[STAR_COUNT];
	LONGLONG						m_llStarAllCount[STAR_COUNT];
	LONGLONG						m_llStarWinCount[STAR_COUNT];
	LONGLONG						m_llAllStarCount;
	int								m_nRedBack;								//����ͼƬ

	//�������
protected:
	CGameLogic						m_GameLogic;							//��Ϸ�߼�
	ITableFrame						* m_pITableFrame;						//��ܽӿ�
	//ITableFrameControl				* m_pITableFrameControl;		    //��ܽӿ�
	const tagGameServiceOption		* m_pGameServiceOption;					//���ò���

protected:
	//�������
protected:
	HINSTANCE						m_hInst;
	IServerControl*					m_pServerContro;
    

	//���Ա���
protected:
	static const WORD				m_wPlayerCount;							//��Ϸ����
	
	//���Ʊ���
protected:
	TCHAR							m_szGameRoomName[SERVER_LEN];			//��������
	//LONGLONG						m_StorageStart;							//��������ÿ���ӵĿ����ֵ����ȡʧ�ܰ� 0 ����
	//LONGLONG						m_StorageDeduct;						//ÿ����Ϸ������۳��Ŀ���������ȡʧ�ܰ� 1.00 ����

	//ׯ������
protected:
	//��ׯ�������ã���ׯ�������趨�ľ���֮��(m_lBankerMAX)��
	//�������ֵ��������������ׯ���б�����������ҽ��ʱ��
	//�����ټ���ׯm_lBankerAdd�֣���ׯ���������á�
	LONGLONG						m_lBankerMAX;							//���ׯ����
	LONGLONG						m_lBankerAdd;							//ׯ��������

	//��ҳ���m_lBankerScoreMAX֮�󣬾�����������ҵĽ��ֵ�������Ľ��ֵ����Ҳ�����ټ�ׯm_lBankerScoreAdd�֡�
	LONGLONG						m_lBankerScoreMAX;						//ׯ��Ǯ
	LONGLONG						m_lBankerScoreAdd;						//ׯ��Ǯ��ʱ,��ׯ������

	//���ׯ����
	LONGLONG						m_lPlayerBankerMAX;						//������ׯ����

	//��ׯ
	bool							m_bExchangeBanker;						//����ׯ��
	bool							m_bBankerHalf;
	bool							m_bBankerBomb;							//ׯ��Ѻ������ǰ����

	//ʱ������
protected:
	BYTE							m_cbFreeTime;							//����ʱ��
	BYTE							m_cbBetTime;							//��עʱ��
	BYTE							m_cbEndTime;							//����ʱ��

	//�����˿���
	int								m_nMaxChipRobot;						//�����Ŀ (��ע������)
	int								m_nChipRobotCount;						//����ͳ�� (��ע������)

	//ʱ������
protected:
	CMD_C_AdminControl              m_AdminCommand;                         //����Ա����

	//��ر��
protected:
	BYTE                            m_cbControlID;                          //���Ʊ��


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
	virtual bool OnActionUserOnReady(WORD wChairID,IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize);

	//�����ӿ�
public:
	//���û���
	virtual void SetGameBaseScore(LONG lBaseScore){};

	//��Ϸ�¼�
protected:
	//��ע�¼�
	bool OnUserPlaceJetton(WORD wChairID, BYTE cbJettonArea, LONG lJettonScore);
	//����ׯ��
	bool OnUserApplyBanker(IServerUserItem *pIApplyServerUserItem);
	//ȡ������
	bool OnUserCancelBanker(IServerUserItem *pICancelServerUserItem);

	//��������
private:
	//�����˿�
	bool DispatchTableCard();
	//����ׯ��
	VOID SendApplyUser( IServerUserItem *pServerUserItem );
	//����ׯ��
	bool ChangeBanker(bool bCancelCurrentBanker);
	//�ֻ��ж�
	VOID TakeTurns();
	//���ͼ�¼
	VOID SendGameRecord(IServerUserItem *pIServerUserItem);
	//������Ϣ
	VOID SendGameMessage(WORD wChairID, LPCTSTR pszTipMsg);
	//������Ϣ
	VOID SendPlaceJettonFail(WORD wChairID, BYTE cbJettonArea, LONGLONG lJettonScore);
	//�Ƿ�׼��
	bool IsAllUserReady();
	//������Ϸ
	bool OnStartGame();
	//���»����˾���
	bool UpdateAndroidDraw();
	//���»�������ׯ��
	bool UpdateAndroidBankCount();

	//��ע����
private:
	//�����ע
	LONGLONG GetUserMaxJetton(WORD wChairID);
	VOID RandList(BYTE cbCardBuffer[], BYTE cbBufferCount);
	bool ChangeMaxUser(bool blMin = false,int iIndex = 1);

	//�����������ׯ
	//VOID MakeAndroidUserBanker();

	//��Ϸͳ��
private:
	//����÷�
    LONGLONG CalculateScore(bool bBankerLeft);
	//�ƶ�Ӯ��
	VOID DeduceWinner(bool &bWinShunMen, bool &bWinDuiMen, bool &bWinDaoMen);
	//��̽���ж�
	bool ProbeJudge(LONGLONG& lSystemScore);
	//�����ж�
	bool  IsAllowBankAction(WORD wChairID, IServerUserItem * pIServerUserItem);
	//��ׯ���� (������)
	VOID RobotBankerControl();
	//ѡ��ׯ��
	bool TrySelectBanker();

	//������Ϣ
public:
	//��ȡ������Ӯ��Ϣ
	bool GetStarInfo();
	//д��������Ӯ��Ϣ
	bool WriteStarInfo();

	//�����¼�
protected:
	//�����Ϣ
	bool OnUserControlEvent(IServerUserItem* pServerUserItem,CMD_GC_ControlInfo* pCtrlData);
	//״̬����
	VOID SendGameStatusToCtrl();
	//����Ѻע��Ϣ
	VOID SendUserChipInfoToCtrl(WORD wChairID, BYTE cbBetArea, LONGLONG lBetScore);


};

//////////////////////////////////////////////////////////////////////////

#endif
