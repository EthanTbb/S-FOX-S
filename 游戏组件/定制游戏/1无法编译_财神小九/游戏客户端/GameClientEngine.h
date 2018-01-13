#pragma once

#include "Stdafx.h"
#include "GameClientView.h"
#include "DirectSound.h"
#include "DlgControl.h"

//////////////////////////////////////////////////////////////////////////

//��Ϸ�Ի���
class CGameClientEngine : public CGameFrameEngine
{
	//��Ԫ����
	friend class CGameClientView;

	//������Ϣ
protected:
	LONGLONG						m_lMeMaxScore;						//�����ע
	LONGLONG						m_lUserLimitScore;					//������ע
	LONGLONG						m_lAreaLimitScore;					//��������
	LONGLONG						m_lApplyBankerCondition;			//��������

	bool							m_bBackGroundSound;					//��������

	//������ע
protected:
	LONGLONG						m_lUserJettonScore[AREA_ARRY_COUNT];	//������ע

	//ׯ����Ϣ
protected:
	LONGLONG						m_lBankerScore;						//ׯ�һ���
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
	CDlgControl						m_DlgControl;

	//������Դ
protected:
	//CDirectSound					m_DTSDBackground;					//��������
	//CDirectSound					m_DTSDCheer[3];						//��������

	//��������
	CWHArray<CMD_S_PlaceJetton,CMD_S_PlaceJetton> m_PlaceJettonArray;

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

	bool OnFrameMessage(WORD wSubCmdID, const void * pBuffer, WORD wDataSize);

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


	//��ֵ����
protected:
	//����ׯ��
	void SetBankerInfo(WORD wBanker,LONGLONG lScore);
	//������ע
	void SetMePlaceJetton(BYTE cbViewIndex, LONGLONG lJettonCount);

	//���ܺ���
protected:
	//���¿���
	void UpdateButtonContron();	
	

	//��Ϣӳ��
protected:
	//��ע��Ϣ
	LRESULT OnStart(WPARAM wParam, LPARAM lParam);
	//��ע��Ϣ
	LRESULT OnPlaceJetton(WPARAM wParam, LPARAM lParam);
	//������Ϣ
	LRESULT OnApplyBanker(WPARAM wParam, LPARAM lParam);
	//��������
	LRESULT OnContinueCard(WPARAM wParam, LPARAM lParam);
	//�ֹ�����
	LRESULT  OnOpenCard(WPARAM wParam, LPARAM lParam);
	//�Զ�����
	LRESULT  OnAutoOpenCard(WPARAM wParam, LPARAM lParam);
	//�Զ�����
	LRESULT  OnOpenSound(WPARAM wParam, LPARAM lParam);
	//�Զ�����
	LRESULT  OnCloseSound(WPARAM wParam, LPARAM lParam);
	//
	LRESULT OnManageControl(WPARAM wParam, LPARAM lParam);
	//������Ϣ
	LRESULT OnPostCardWav(WPARAM wParam, LPARAM lParam);
	//����Ա����
	LRESULT OnAdminCommand(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT nIDEvent);
};

//////////////////////////////////////////////////////////////////////////
