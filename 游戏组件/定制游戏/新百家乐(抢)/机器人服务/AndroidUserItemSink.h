#ifndef ANDROID_USER_ITEM_SINK_HEAD_FILE
#define ANDROID_USER_ITEM_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
//#include "GameLogic.h"

//////////////////////////////////////////////////////////////////////////
//�궨��

//�����ע����
#define MAX_CHIP_TIME								50

#define AREA_COUNT					                8					//������Ŀ

//��������Ϣ
struct tagRobotInfo
{
	int nChip[7];														//���붨��
	int nAreaChance[AREA_COUNT];										//������
	int	nMaxTime;														//�������

	tagRobotInfo()
	{
		int nTmpChip[7] = {100, 1000, 10000, 100000, 1000000, 5000000, 10000000};
		int nTmpAreaChance[AREA_COUNT] = {3, 1, 3, 1, 1, 1, 1, 2};

		nMaxTime = 1;
		memcpy(nChip, nTmpChip, sizeof(nChip));
		memcpy(nAreaChance, nTmpAreaChance, sizeof(nAreaChance));
	}
};
//////////////////////////////////////////////////////////////////////////

//��������
class CAndroidUserItemSink : public IAndroidUserItemSink
{
	//��Ϸ����
protected:
	LONGLONG						m_lMaxChipBanker;					//�����ע (ׯ��)
	LONGLONG						m_lMaxChipUser;						//�����ע (����)
	LONGLONG						m_lAreaChip[AREA_COUNT];			//������ע 
	WORD							m_wCurrentBanker;					//ׯ��λ��
	BYTE							m_cbTimeLeave;						//ʣ��ʱ��

	LONGLONG						m_lBankerScore;						//ׯ��Ǯ
	LONGLONG						m_lAllBet[AREA_MAX];				//����ע
	LONGLONG						m_lAndroidBet;						//��������ע

	int								m_nChipLimit[2];					//��ע��Χ (0-AREA_COUNT)
	int								m_nChipTime;						//��ע���� (����)
	int								m_nChipTimeCount;					//���´��� (����)
	
	//��ׯ����
protected:
	int								m_nListUserCount;					//�б�����
	bool							m_bMeApplyBanker;					//�����ʶ
	int								m_nBankerCount;						//�������˵���ׯ����
	int								m_nWaitBanker;						//�ռ���
	static int						m_stlApplyBanker;					//������
	static int						m_stnApplyCount;					//������

	int                             m_nMinBankerTimes;					//��С��ׯ����
	int                             m_nMaxBankerTimes;					//�����ׯ����

	//���ñ���  (ȫ������)
protected:
	tagRobotInfo					m_RobotInfo;						//ȫ������
	TCHAR							m_szRoomName[32];					//���÷���

	//���ñ���	(��Ϸ����)
protected:
	bool							m_bRefreshCfg;						//ÿ��ˢ��
	LONGLONG						m_lAreaLimitScore;					//��������
	LONGLONG						m_lUserLimitScore;					//��ע����
	LONGLONG						m_lBankerCondition;					//��ׯ����		

	//���ñ���  (����������)
protected:
	LONGLONG						m_lRobotBetLimit[2];				//��������	
	int								m_nRobotBetTimeLimit[2];			//��������	
	bool							m_bRobotBanker;						//�Ƿ���ׯ
	int								m_nRobotBankerCount;				//��ׯ����
	int								m_nRobotWaitBanker;					//��������
	int								m_nRobotListMaxCount;				//��ׯ����
	int								m_nRobotListMinCount;				//��ׯ����
	int								m_nRobotApplyBanker;				//��ׯ����
	bool							m_bReduceBetLimit;				//��������

	//�����˴�ȡ��
	LONGLONG						m_lRobotScoreRange[2];				//���Χ
	LONGLONG						m_lRobotBankGetScoreMin;				//�������
	LONGLONG						m_lRobotBankGetScoreMax;				//�������
	LONGLONG						m_lRobotBankGetScoreBankerMin;			//������� (ׯ��)
	LONGLONG						m_lRobotBankGetScoreBankerMax;			//������� (ׯ��)
	int								m_nRobotBankStorageMul;				//����

	//�ؼ�����
protected:
	//CGameLogic						m_GameLogic;						//��Ϸ�߼�
	IAndroidUserItem *				m_pIAndroidUserItem;				//�û��ӿ�

	//��������
public:
	//���캯��
	CAndroidUserItemSink();
	//��������
	virtual ~CAndroidUserItemSink();

	//�����ӿ�
public:
	//�ͷŶ���
	virtual VOID Release() { }
	//�ӿڲ�ѯ
	virtual void * QueryInterface(REFGUID Guid, DWORD dwQueryVer);

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


	//��Ϣ����
public:
	//��Ϸ����
	bool OnSubGameFree(const void * pBuffer, WORD wDataSize);
	//��Ϸ��ʼ
	bool OnSubGameStart(const void * pBuffer, WORD wDataSize);
	//�û���ע
	bool OnSubPlaceBet(const void * pBuffer, WORD wDataSize);
	//��עʧ��
	bool OnSubPlaceBetFail(const void * pBuffer, WORD wDataSize);
	//��Ϸ����
	bool OnSubGameEnd(const void * pBuffer, WORD wDataSize);
	//������ׯ
	bool OnSubUserApplyBanker(const void * pBuffer, WORD wDataSize);
	//ȡ����ׯ
	bool OnSubUserCancelBanker(const void * pBuffer, WORD wDataSize);
	//�л�ׯ��
	bool OnSubChangeBanker(const void * pBuffer, WORD wDataSize);

	//���ܺ���
public:
	//��ȡ����
	void ReadConfigInformation(tagCustomAndroid *pCustomAndroid);
	//���㷶Χ
	bool CalcBetRange(LONGLONG lMaxScore, LONGLONG lChipLmt[], int & nChipTime, int lJetLmt[]);
	//�����ע
	LONGLONG GetMaxPlayerScore( BYTE cbBetArea );
};

//////////////////////////////////////////////////////////////////////////

#endif
