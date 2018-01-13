#pragma once

#include "Stdafx.h"
#include "GameClientView.h"

//////////////////////////////////////////////////////////////////////////

//��Ϸ�Ի���
class CGameClientEngine : public CGameFrameEngine
{
	//��Ԫ����
	friend class CGameClientView;


	//������Ϣ
protected:
	LONGLONG						m_lMeMaxScore;						//�����ע
	LONGLONG						m_lAreaLimitScore;					//��������
	LONGLONG						m_lApplyBankerCondition;			//��������
	int                             m_GameEndTime;						//����ʱ��
	bool							m_blUsing;							//�Ƿ�ʹ��
	bool                            m_bPlaceEnd;						//��ע����
	LONGLONG						m_lInitUserScore[GAME_PLAYER];		//ԭʼ����
	int                             m_nCurSetBackMusic;					//��ǰ����

	//������ע
protected:
	LONGLONG						m_lUserJettonScore[AREA_COUNT+1];	    //������ע
	LONGLONG                        m_lPlayerBetAll[AREA_COUNT+1];	    //ȫ����ע
	LONGLONG						m_lPlayLastBet[AREA_COUNT+1];			//�ϴ�ѹס
	LONGLONG						m_lUserStartScore[GAME_PLAYER];			//�����ʵ����
	//ׯ����Ϣ
protected:
	LONGLONG						m_lBankerScore;						//ׯ�һ���
	WORD							m_wCurrentBanker;					//��ǰׯ��
	bool							m_bEnableSysBanker;					//ϵͳ��ׯ
	LONGLONG						m_lAllPlayBet[GAME_PLAYER][AREA_COUNT+1];//���������ע
	//״̬����
public:
	bool							m_bMeApplyBanker;					//�����ʶ

	//�ؼ�����
protected:
	CGameLogic						m_GameLogic;						//��Ϸ�߼�
	CGameClientView					m_GameClientView;					//��Ϸ��ͼ
	CGlobalUnits *                  m_pGlobalUnits;                     //ȫ�ֵ�Ԫ
	//�ؼ�����
protected:
	CList<tagAndroidBet,tagAndroidBet>	m_ListAndroid;					//������ע

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

   bool OnSubUpdateStorage(const void * pBuffer, WORD wDataSize);
	//�û��¼�
public:
	//�û�����
	virtual VOID  OnEventUserEnter(IClientUserItem * pIClientUserItem, bool bLookonUser);
	//�û��뿪
	virtual VOID  OnEventUserLeave(IClientUserItem * pIClientUserItem, bool bLookonUser);
	//�û�״̬
	virtual VOID  OnEventUserStatus(IClientUserItem * pIClientUserItem, bool bLookonUser);


	//���������ص�
public:
	//ѡ���¼�
	virtual VOID  OnEventPhraseSelect(INT nPhraseIndex);
	//����
	LRESULT  OnChatMessage( WPARAM wParam, LPARAM lParam );

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

	/*bool OnSubCheckImageIndex(const void * pBuffer, WORD wDataSize);*/
	//����
	bool OnSubAdminControl(const void * pBuffer, WORD wDataSize);


	//��ֵ����
protected:
	//����ׯ��
	void SetBankerInfo(WORD wBanker,LONGLONG lScore);
	//������ע
	void SetMePlaceJetton(BYTE cbViewIndex, LONGLONG lJettonCount);


	//���ܺ���
protected:
	void ReSetBankCtrol(int nGameState);
	//���¿���
	void UpdateButtonContron();	
	//�Լ�����·�
	LONGLONG GetMeMaxBet( BYTE cbArea );
	//�����û��б�
	void UpdateUserList(bool Refresh=false);
	
	//��Ϣӳ��
protected:
	//��ע��Ϣ
	LRESULT OnPlaceJetton(WPARAM wParam, LPARAM lParam);
	//������Ϣ
	LRESULT OnApplyBanker(WPARAM wParam, LPARAM lParam);
	
	LRESULT  OnPlaySound(WPARAM wParam, LPARAM lParam);
	//����
	LRESULT  OnAdminControl(WPARAM wParam, LPARAM lParam);
	//����
	LRESULT  OnGameSetting( WPARAM wParam, LPARAM lParam );
	//������
	LRESULT OnUpdataStorage(WPARAM wParam, LPARAM lParam);
	//��ѹ��Ϣ
	LRESULT OnMessageOperationRenewal(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT nIDEvent);
};

//////////////////////////////////////////////////////////////////////////
