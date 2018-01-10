#pragma once

#include "Stdafx.h"
#include "GameClientView.h"

//#include "DirectSound.h"
#include "afxtempl.h"

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
	int                             m_GameEndTime;
	BYTE							m_cbCompareTime;						//�ȴ�Сʱ��	
	bool							m_bGuess;							//�Ƿ�����´�С
	bool							m_blUsing;
	bool							m_bMoreJetton;						//������ע�Ƿ񳬹�����

	//������ע
protected:
	LONGLONG						m_lUserJettonScore[AREA_COUNT+1];	//������ע
	LONGLONG						m_lLastUserJetton[AREA_COUNT+1];	//��һ����ע���
	LONGLONG						m_lUserScore;						//�������ʣ����
	LONGLONG						m_lChangeGold;						//�һ������
	int								m_nExchange;						//�һ�����
	LONGLONG						m_lExchangeScore[GAME_PLAYER];		//�һ���ĳ���
	LONGLONG						m_lAddGold;							//�һ�ʱ���ӵĽ��
	LONGLONG						m_lUserReturnScore[GAME_PLAYER];	//Ѻ�к󵱾ֻ�õķ���
	LONGLONG						m_lMyGameEndScore;					//�Լ�ÿ�ֳɼ�
	//ׯ����Ϣ
protected:
	LONGLONG						m_lBankerScore;						//ׯ�һ���
	WORD							m_wCurrentBanker;					//��ǰׯ��
	BYTE							m_cbLeftCardCount;					//�˿���Ŀ
	bool							m_bEnableSysBanker;					//ϵͳ��ׯ


	//״̬����
protected:
	bool							m_bMeApplyBanker;					//�����ʶ

	//�ؼ�����
protected:
	CGameLogic						m_GameLogic;						//��Ϸ�߼�
	CGameClientView					m_GameClientView;					//��Ϸ��ͼ
	bool							m_bBackGroundSound;					//��������

	//�ؼ�����
protected:
	CList<tagAndroidBet,tagAndroidBet>		m_ListAndroid;				//��������ע����

	//������Դ
protected:
//	CDirectSound					m_DTSDBackground;					//��������
//	CDirectSound					m_DTSDCheer[4];						//��������

	//ȫ������
protected:
	//CGlobalUnits *                  m_pGlobalUnits;                     //ȫ�ֵ�Ԫ


	struct SortTemp
	{
		LONGLONG a;
		LONGLONG b;
		WORD	 c;
		LONGLONG d;
		SortTemp()
		{
			memset(this,0,sizeof(*this));
		}
	};


	SortTemp m_TempData;

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
	//��������
	virtual bool AllowBackGroundSound(bool bAllowSound);

	//��Ϸ�¼�
public:
	//�Թ���Ϣ
	virtual bool OnEventLookonMode(VOID * pData, WORD wDataSize);
	//��Ϸ��Ϣ
	virtual bool OnEventGameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//������Ϣ
	virtual bool OnEventSceneMessage(BYTE cbGameStatus, bool bLookonUser, VOID * pData, WORD wDataSize);

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

	bool OnSubCheckImageIndex(const void * pBuffer, WORD wDataSize);
	//����
	bool OnSubAdminControl(const void * pBuffer, WORD wDataSize);
	//�һ�����
	bool OnSubChangeGold(const void * pBuffer, WORD wDataSize);
	//��Ϸ�´�С
	bool OnSubGameCompare(const void * pBuffer, WORD wDataSize);
	//�´�С
	bool OnSubGuess(const void * pBuffer, WORD wDataSize);


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
	//��������
	bool PlayBackGroundSound(HINSTANCE hInstance, LPCTSTR pszSoundName);
	//ֹͣ����
	bool StopSound();

	//��Ϣӳ��
protected:
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
	LRESULT  OnShowResult(WPARAM wParam, LPARAM lParam);

	LRESULT  OnPlaySound(WPARAM wParam, LPARAM lParam);
	//����
	LRESULT  OnAdminControl(WPARAM wParam, LPARAM lParam);
	//���¿��
	LRESULT OnStorage(WPARAM wParam,LPARAM lParam);
	//���¿��
	LRESULT OnDeuct(WPARAM wParam,LPARAM lParam);
	//�޸��·�����
	LRESULT OnStorageInfo(WPARAM wParam,LPARAM lParam);
	//�һ�����
	LRESULT OnChageMoney(WPARAM wParam, LPARAM lParam);
	//��Ѻ
	LRESULT OnContinueJetton(WPARAM wParam, LPARAM lParam);
	//�رմ���
	LRESULT OnColse(WPARAM wParam, LPARAM lParam);
	//ȷ��
	LRESULT OnSureChange(WPARAM wParam, LPARAM lParam);
	//�һ�����
	LRESULT OnChangeGold(WPARAM wParam, LPARAM lParam);
	//�´�С
	LRESULT OnGuess(WPARAM wParam, LPARAM lParam);
	
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT nIDEvent);
};

//////////////////////////////////////////////////////////////////////////
