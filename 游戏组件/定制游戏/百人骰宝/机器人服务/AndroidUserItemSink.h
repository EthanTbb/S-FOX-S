#ifndef ANDROID_USER_ITEM_SINK_HEAD_FILE
#define ANDROID_USER_ITEM_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "..\��Ϸ������\GameLogic.h"
//////////////////////////////////////////////////////////////////////////
//�궨��

//�����ע����
#define MAX_CHIP_TIME								50
//////////////////////////////////////////////////////////////////////////

//��������
class CAndroidUserItemSink : public IAndroidUserItemSink
{
	//��Ϸ����
protected:
	bool							m_bHaveConfig;						//�Ƿ�����
	SCORE							m_lMaxChipBanker;					//�����ע (ׯ��)
	SCORE							m_lMaxChipUser;						//�����ע (����)
	SCORE							m_lAreaChip[AREA_COUNT];			//������ע 
	WORD							m_wCurrentBanker;					//ׯ��λ��

	int								m_nChipLimit[2];					//��ע��Χ (0-AREA_COUNT)
	int								m_nChipTime;						//��ע���� (����)
	int								m_nChipTimeCount;					//���´��� (����)

	SCORE							m_lBankerLostScore;					//ׯ����� (��������Ϊׯ)

	SCORE							m_lBankerDrawCount;					//������ (ׯ��)
	SCORE							m_lBankerWinCount;					//��ׯ���� (Ӯ��)
	SCORE							m_lBankerLoseCount;					//��ׯ���� (���)

	BYTE							m_bWinFlag[AREA_COUNT];				//��Ӯ��Ϣ (������עƫ��)

	//��ׯ����
protected:
	int								m_nBankerCount;						//�������˵���ׯ����
	int								m_nCurMaxBankerCount;				//������������ׯ����
	bool							m_bRobotBanker;						//�Ƿ���ׯ
	bool							m_bMeApplyBanker;					//�����ʶ
	int								m_nRobotBankerCount;				//��ׯ����  
	static int						m_stlApplyBanker;					//������
	static int						m_stnApplyCount;					//������
	INT64                           m_nListUserCount;					//�б�����

	//���ñ���  (ȫ������)
protected:
	tagRobotInfo					m_RobotInfo;						//ȫ������
	TCHAR							m_szRoomName[32];					//���÷���

	//���ñ���	(��Ϸ����)
protected:
	SCORE							m_lAreaLimitScore;					//��������
	SCORE							m_lUserLimitScore;					//��ע����
	SCORE							m_lBankerCondition;					//��ׯ����		

	//���ñ���  (����������)
protected:
	int								m_nRobotBetDelay;					//��ע�ӳ�

	SCORE							m_lRobotBankerCondition;			//��ׯ���� (��������Ϸ���ƣ�����ǵ�������������)
	int								m_nRobotListMaxCount;				//��ׯ����
	int								m_nRobotListMinCount;				//��ׯ����
	int								m_nRobotApplyBanker;				//��ׯ����
	SCORE							m_lRobotBankSaveCondition;			//�������
	SCORE							m_lRobotBankDrawCondition;			//�������
	SCORE							m_lRobotBankSaveScore;				//�������
	SCORE							m_lRobotBankDrawScore;				//�������

	SCORE							m_lRobotJettonLimit[2];				//��������	
	int								m_nRobotBetTimeLimit[2];			//��������
	int								m_nRobotWaitBanker;					//��������
	int								m_nWaitBanker;						//�ռ���
	SCORE							m_lRobotBankDrawCount;				//������ (ׯ��)
	SCORE							m_lRobotGiveUpWin;					//��ׯ���� (Ӯ��)
	SCORE							m_lRobotGiveUpLose;					//��ׯ���� (���)


	//�����˴�ȡ��
	LONGLONG						m_lRobotScoreRange[2];				//���Χ
	LONGLONG						m_lRobotBankGetScoreMin;				//�������
	LONGLONG						m_lRobotBankGetScoreMax;				//�������
	LONGLONG						m_lRobotBankGetScoreBankerMin;			//������� (ׯ��)
	LONGLONG						m_lRobotBankGetScoreBankerMax;			//������� (ׯ��)
	int								m_nRobotBankStorageMul;				//����


	//�ؼ�����
protected:
	CGameLogic						m_GameLogic;						//��Ϸ�߼�
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
	virtual VOID  Release() {delete this; }
	
	//�ӿڲ�ѯ
	virtual VOID *  QueryInterface(const IID & Guid, DWORD dwQueryVer);


	//���ƽӿ�
public:
	//��ʼ�ӿ�
	virtual bool  Initialization(IUnknownEx * pIUnknownEx);
	//���ýӿ�
	virtual bool  RepositionSink();


	//��Ϸ�¼�
public:
	//ʱ����Ϣ
	virtual bool  OnEventTimer(UINT nTimerID);
	//��Ϸ��Ϣ
	virtual bool  OnEventGameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//��Ϸ��Ϣ
	virtual bool  OnEventFrameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//������Ϣ
	virtual bool  OnEventSceneMessage(BYTE cbGameStatus, bool bLookonOther, VOID * pData, WORD wDataSize);


	//�û��¼�
public:
	//�û�����
	virtual VOID  OnEventUserEnter(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser);
	//�û��뿪
	virtual VOID  OnEventUserLeave(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser);
	//�û�����
	virtual VOID  OnEventUserScore(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser);
	//�û�״̬
	virtual VOID  OnEventUserStatus(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser);
	
	//��Ϣ����
public:
	//��Ϸ����
	bool OnSubGameFree(const VOID * pBuffer, WORD wDataSize);
	//��Ϸ��ʼ
	bool OnSubGameStart(const VOID * pBuffer, WORD wDataSize);
	//�û���ע
	bool OnSubPlaceJetton(const VOID * pBuffer, WORD wDataSize);
	//��עʧ��
	bool OnSubPlaceJettonFail(const VOID * pBuffer, WORD wDataSize);
	//��Ϸ����
	bool OnSubGameEnd(const VOID * pBuffer, WORD wDataSize);
	//������ׯ
	bool OnSubUserApplyBanker(const VOID * pBuffer, WORD wDataSize);
	//ȡ����ׯ
	bool OnSubUserCancelBanker(const VOID * pBuffer, WORD wDataSize);
	//�л�ׯ��
	bool OnSubChangeBanker(const VOID * pBuffer, WORD wDataSize);
	//��ׯ֪ͨ
	bool OnSubRobotBanker(const VOID * pBuffer, WORD wDataSize);
	//����֪ͨ
	bool OnSubRobotConfig(const VOID * pBuffer, WORD wDataSize);

	//���ܺ���
public:
	//��ȡ����
	VOID ReadConfigInformation(tagCustomAndroid *pCustomAndroid);
	//���㷶Χ
	bool CalcJettonRange(SCORE lMaxScore, SCORE lChipLmt[], int & nChipTime, int lJetLmt[]);
	//��ȡ����
	SCORE GetPrivateProfileInt64(LPCTSTR lpAppName, LPCTSTR lpKeyName, SCORE lDefault, LPCTSTR lpFileName);
};

//////////////////////////////////////////////////////////////////////////

#endif
