#ifndef GAME_CLIENT_ENGINE_HEAD_FILE
#define GAME_CLIENT_ENGINE_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "GameLogic.h"
#include "GameClientView.h"
#include "HistoryScore.h"
#include "MessageBox.h"

//////////////////////////////////////////////////////////////////////////

//��Ϸ�Ի���
class CGameClientEngine : public CGameFrameEngine
{
	//ʱ�䶨��
protected:
	BYTE							m_cbTimeStartGame;					//��ʼʱ��
	BYTE							m_cbTimeCallBanker;					//��ׯʱ��
	BYTE							m_cbTimeSetChip;					//��עʱ��
	BYTE							m_cbTimeRangeCard;					//����ʱ��
	BYTE							m_cbTimeShowCard;					//����ʱ��
	
	BYTE							m_cbFuZhuTime;						//����ʱ��

	//�ؼ�����
protected:
	CGameLogic						m_GameLogic;						//��Ϸ�߼�
	CGameClientView					m_GameClientView;					//��Ϸ��ͼ
	CHistoryScore					m_HistroyScore;						//��ʷ�ɼ�

	//���ñ���
protected:
	UINT_PTR						m_nTimer ;							//��ʱ��ID
	UINT							m_nCompareTime;						//�Ƚ�ʱ��
	bool							m_bCanPlayVoice;					//�Ƿ�����

	//��Ϸ����
protected:
	BYTE							m_bHandCardData[GAME_PLAYER][HAND_CARD_COUNT];	//�����˿�
	BYTE							m_bHandCardCount ;					//�˿���Ŀ
	BYTE							m_bTmpHandCardData[HAND_CARD_COUNT];//�����˿�
	BYTE							m_bSegmentCard[GAME_PLAYER][3][5];	//�ֶ��˿�
	tagSegInfo						m_tagSegInfo;						//��ʱ�ֶ�

	WORD							m_wBanker;							//ׯ�����
	SCORE							m_lScoreTimes[GAME_PLAYER][3];		//��������
	SCORE							m_lUserChip[GAME_PLAYER];			//ѹע��С
	SCORE							m_wMaxChip;							//�����ע
	bool							m_bFinishSegment;					//��ɷֶ�
	bool							m_bGameStatus[GAME_PLAYER];			//��Ϸ״̬
	bool							m_bRequestSplit;					//�Ƿ�����
	bool							m_bIsShowCard;						//�Ƿ�ȷ��
	bool							m_bTrustee;							//�Ƿ��й�
	BYTE							m_bComparedCard;					//��������

	tagSearchCardResult				m_SearchCardResult;					//�������
	BYTE							m_cbSearchTypeIndex;				//�������
	BYTE							m_cbSortTypeIndex;					//��������
	DWORD							m_TimerCount;						//ʱ�����

	bool							m_bCheatRight;						//�����û�
	bool							m_bUserCheat[GAME_PLAYER];			//���ױ�־

	//��������
public:
	//���캯��
	CGameClientEngine();
	//��������
	virtual ~CGameClientEngine();

	//��Ϣ����
protected:
	//��ʼ��ׯ
	bool OnSubBeginCall(const void * pBuffer, WORD wDataSize);
	//��ҽ�ׯ
	bool OnSubUserCall(const void * pBuffer, WORD wDataSize);
	//������ׯ
	bool OnSubAllNotCall(const void * pBuffer, WORD wDataSize);
	//�����˿�
	bool OnSubSendCardEx(const void * pBuffer, WORD wDataSize);
	//��Ϸ����
	bool OnSubGameEnd(const void * pBuffer, WORD wDataSize) ;
	//������ע
	bool OnSubSetChip(const void * pBuffer , WORD wDataSize) ;	
	//��ʼϴ��
	bool OnSubStartShuffle(const void * pBuffer , WORD wDataSize) ;
	//���÷ֶ�
    bool OnSubSetSegment(const void * pBuffer , WORD wDataSize) ;
    //���̯��
	bool OnSubShowCard(const void * pBuffer , WORD wDataSize) ;
	//�Ƚ��˿�
	bool OnSubCompareCard(const void * pBuffer , WORD wDataSize) ;
	//�û�ǿ��
	bool OnSubPlayerExit(const void * pBuffer, WORD wDataSize);
	//�û�����
	bool OnSubSplitCard(const void * pBuffer, WORD wDataSize);
	//��ʼ����
	bool OnSubDispatchStart(const void * pBuffer, WORD wDataSize);
	//�û��й�
	bool OnSubTrustee(const void * pBuffer, WORD wDataSize);
	//��������
	bool OnSubVoicePhrase(const void * pBuffer, WORD wDataSize);
	//��������
	bool OnSubCheatCard(VOID * pData, WORD wDataSize);

    //��������
protected:
	//�Զ�����
	void AutomaticOutCard() ;
	//��ʾ����
	void ShowAutomaticOutCard(WORD wChairID) ;
	//���Ƴ���
	void CompareCard(BYTE bSceneName);
	//���°�ť
	void UpdateButtonState();
	//���°�ť
	void UpdateChipButtonState(int nShowCmd, BOOL bEnable);
	//���°�ť
	void UpdateBlockButton(BYTE cbBtnIndex, int nShowCmd, BOOL bEnable);
	//��ȡ����
	LPCTSTR GetPhraseString(BYTE bGender,BYTE bType,BYTE bIndex);
	//�������
	BYTE RandYuYin(BYTE bType,BYTE bGender);

	//����̳�
private:
	//��ʼ����
	virtual bool OnInitGameEngine();
	//���ÿ��
	virtual bool OnResetGameEngine();

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

	//�û�����
	virtual VOID OnEventUserEnter(IClientUserItem * pIClientUserItem, bool bLookonUser);
	//�û��뿪
	virtual VOID OnEventUserLeave(IClientUserItem * pIClientUserItem, bool bLookonUser);

public:
	//��������
	virtual bool AllowBackGroundSound(bool bAllowSound){return true;}

	//��Ϣ����
protected:
	//��ʼ��Ϣ
	LRESULT OnStart(WPARAM wParam, LPARAM lParam);
	//��ׯ��Ϣ
	LRESULT OnCallBanker(WPARAM wParam, LPARAM lParam);
	//������Ϣ
	LRESULT OnNoCallBanker(WPARAM wParam, LPARAM lParam);
	//ѹע��Ϣ
	LRESULT OnSetChip(WPARAM wParam, LPARAM lParam);
	//������Ϣ
	LRESULT OnSplitCard(WPARAM wParam, LPARAM lParam);
	//ϴ�����
	LRESULT OnSplitFinish(WPARAM wParam, LPARAM lParam);
	//ϴ�����
	LRESULT OnShuffleFinish(WPARAM wParam, LPARAM lParam);
	//�������
	LRESULT OnDispatchCardFinish(WPARAM wParam, LPARAM lParam);
	//��ʾ��Ϣ
	LRESULT OnPrompt(WPARAM wParam, LPARAM lParam);
	//������Ϣ
	LRESULT OnVoicePhrase(WPARAM wParam, LPARAM lParam);
	//������Ϣ
	LRESULT	OnSortCard(WPARAM wParam, LPARAM lParam);	
	//�й���Ϣ
	LRESULT OnTrustee(WPARAM wParam, LPARAM lParam);
	//��һע��Ϣ
	LRESULT OnFrontCard(WPARAM wParam, LPARAM lParam) ;
	//�ڶ�ע��Ϣ
	LRESULT OnMidCard(WPARAM wParam, LPARAM lParam) ;
	//����ע��Ϣ
	LRESULT OnBackCard(WPARAM wParam, LPARAM lParam) ;
	//���ȷ��
	LRESULT OnShowCard(WPARAM wParam, LPARAM lParam) ;	
	//����˿�
	LRESULT OnLeftHitCard(WPARAM wParam, LPARAM lParam) ;
	//�����л�
	LRESULT OnSwitchLang(WPARAM wParam, LPARAM lParam);
	//�����˿�
	LRESULT OnRestoreCard(WPARAM wParam, LPARAM lParam);
	//�������
	LRESULT OnRevesalTurnFinish(WPARAM wParam, LPARAM lParam);
	//��������
	LRESULT	OnMsgInfo(WPARAM wParam, LPARAM lParam);


	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

	DECLARE_MESSAGE_MAP()
};

#endif
//////////////////////////////////////////////////////////////////////////
