#pragma once

#include "Stdafx.h"
#include "GameClientView.h"
//#include "DirectSound.h"
//#include "afxtempl.h"

//////////////////////////////////////////////////////////////////////////

//��Ϸ�Ի���
class CGameClientEngine : public CGameFrameEngine
{
	//��Ԫ����
	friend class CGameClientView;

	//������Ϣ
protected:
	SCORE							m_lMeMaxScore;						//�����ע
	SCORE							m_lAreaLimitScore;					//��������
	SCORE							m_lApplyBankerCondition;			//��������

	//������ע
protected:
	SCORE							m_lUserBet[AREA_COUNT];				//������ע
	LONGLONG						m_lInitUserScore[GAME_PLAYER];		//ԭʼ����
	LONGLONG						m_lUserJettonScore[AREA_COUNT][GAME_PLAYER];

	//ׯ����Ϣ
protected:
	SCORE							m_lBankerScore;						//ׯ�һ���
	WORD							m_wCurrentBanker;					//��ǰׯ��
	BYTE							m_cbLeftCardCount;					//�˿���Ŀ
	bool							m_bEnableSysBanker;					//ϵͳ��ׯ

	//״̬����
protected:
	bool							m_bMeApplyBanker;					//�����ʶ
	bool                            m_bCanPlaceJetton;					//������ע
	//�ؼ�����
protected:
	CGameLogic						m_GameLogic;						//��Ϸ�߼�
	CGameClientView					m_GameClientView;					//��Ϸ��ͼ

	//������Դ
protected:
	
	//��������
protected:
	CList<tagAndroidBet,tagAndroidBet>		m_ListAndroid;				//��������ע����

	//��������
public:
	//���캯��
	CGameClientEngine();
	//��������
	virtual ~CGameClientEngine();

	//����̳�
private:
	//��ʼ����
	virtual bool OnInitGameEngine();
	//���ÿ��
	virtual bool OnResetGameEngine();
	//��Ϸ����
	virtual void OnGameOptionSet();

	//ʱ���¼�
public:
	//ʱ��ɾ��
	virtual bool OnEventGameClockKill(WORD wChairID);
	//ʱ����Ϣ
	virtual bool OnEventGameClockInfo(WORD wChairID, UINT nElapse, WORD wClockID);


	//��Ϸ�¼�
public:
	//�Թ���Ϣ
	virtual bool OnEventLookonMode(VOID * pData, WORD wDataSize);
	//��Ϸ��Ϣ
	virtual bool OnEventGameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//������Ϣ
	virtual bool OnEventSceneMessage(BYTE cbGameStatus, bool bLookonUser, VOID * pData, WORD wDataSize);
	//��������
	virtual bool AllowBackGroundSound(bool bAllowSound);

	//�û��¼�
public:
	//�û�����
	virtual VOID OnEventUserEnter(IClientUserItem * pIClientUserItem, bool bLookonUser);
	//�û��뿪
	virtual VOID OnEventUserLeave(IClientUserItem * pIClientUserItem, bool bLookonUser);
	//�û�����
	virtual VOID OnEventUserScore(IClientUserItem * pIClientUserItem, bool bLookonUser){};
	//�û�״̬
	virtual VOID OnEventUserStatus(IClientUserItem * pIClientUserItem, bool bLookonUser);
	//�û���λ
	virtual VOID OnEventUserSegment(IClientUserItem * pIClientUserItem, bool bLookonUser){};
	//�û�ͷ��
	virtual VOID OnEventCustomFace(IClientUserItem * pIClientUserItem, bool bLookonUser){};

	//��Ϣ����
protected:
	//��Ϸ��ʼ
	bool OnSubGameStart(const void * pBuffer, WORD wDataSize);
	//��Ϸ����
	bool OnSubGameFree(const void * pBuffer, WORD wDataSize);
	//�û���ע
	bool OnSubPlaceJetton(const void * pBuffer, WORD wDataSize);
	//��Ϸ����
	bool OnSubGameEnd(const void * pBuffer, WORD wDataSize);
	//������ׯ
	bool OnSubUserApplyBanker(const void * pBuffer, WORD wDataSize);
	//ȡ����ׯ
	bool OnSubUserCancelBanker(const void * pBuffer, WORD wDataSize);
	//�л�ׯ��
	bool OnSubChangeBanker(const void * pBuffer, WORD wDataSize);
	//��Ϸ��¼
	bool OnSubGameRecord(const void * pBuffer, WORD wDataSize);
	//��עʧ��
	bool OnSubPlaceJettonFail(const void * pBuffer, WORD wDataSize);
	//�˿���
	bool OnSubSendCard(const void * pBuffer, WORD wDataSize);
	//��ʼ��ע
	bool OnUserStartBet(const void * pBuffer, WORD wDataSize);
	//����Ѻע
	bool OnRevocatBet(const void * pBuffer, WORD wDataSize);
	//������
	bool OnSubReqResult(const void * pBuffer, WORD wDataSize);
	//������ע��Ϣ
	bool OnSubSendUserBetInfo(const void * pBuffer, WORD wDataSize);
	//���¿��
	bool OnSubUpdateStorage(const void * pBuffer, WORD wDataSize);
	//���������Ӯ
	bool OnSubControlWinLose(const void * pBuffer, WORD wDataSize);
	//��ֵ����
protected:
	//����ׯ��
	void SetBankerInfo(WORD wBanker,SCORE lScore);
	//������ע
	void SetMePlaceJetton(BYTE cbViewIndex, SCORE lJettonCount);
	//
	BYTE GetViewStatus();
	
	//���ܺ���
protected:
	//��������
	void ReSetBankCtrol(int nGameState);
	//���¿ؼ�
	void UpdateButtonContron();	
	//�����û��б�
	void UpdateUserList();

	//��Ϣӳ��
protected:
	//����Ա����
	LRESULT OnAdminCommand(WPARAM wParam, LPARAM lParam);
	//��ע��Ϣ
	LRESULT OnPlaceJetton(WPARAM wParam, LPARAM lParam);
	//������Ϣ
	LRESULT OnApplyBanker(WPARAM wParam, LPARAM lParam);
	//��������
	LRESULT OnContinueCard(WPARAM wParam, LPARAM lParam);
	//���¿��
	LRESULT OnStorage(WPARAM wParam,LPARAM lParam);
	//��Ϸ�ر�
	LRESULT OnMessageGameClose(WPARAM wParam, LPARAM lParam);
	//������Ϣ
	LRESULT OnChatMessage(WPARAM wParam, LPARAM lParam);
	//���Ƶ��������Ӯ
	LRESULT OnControlWin(WPARAM wParam,LPARAM lParam);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT nIDEvent);
};

//////////////////////////////////////////////////////////////////////////
