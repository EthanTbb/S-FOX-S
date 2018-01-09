#ifndef GAME_CLIENT_ENGINE_HEAD_FILE
#define GAME_CLIENT_ENGINE_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "GameClientView.h"

//////////////////////////////////////////////////////////////////////////

//��Ϸ����
class CGameClientEngine : public CGameFrameEngine
{
	//��ע��Ϣ
protected:
	LONG							m_lCellScore;						//��Ϸ��ע
	LONGLONG						m_lServiceCharge;					//�����
	LONGLONG						m_lDrawMaxScore;					//�����ע
	LONGLONG						m_lTurnMaxScore;					//�����ע
	LONGLONG						m_lTurnLessScore;					//��С��ע
	LONGLONG						m_lUserScore[GAME_PLAYER];			//�û���ע
	LONGLONG						m_lTableScore[GAME_PLAYER];			//������ע

	//״̬����
protected:
	bool							m_bShowHand;						//�����־
	WORD							m_wCurrentUser;						//��ǰ�û�
	BYTE							m_cbPlayStatus[GAME_PLAYER];		//��Ϸ״̬
	TCHAR							m_szAccounts[GAME_PLAYER][32];		//�������
	bool							m_bXiaZhu;							//��ע��־

	//��������
protected:
	CMD_S_GameEnd					m_GameEndPacket;					//��������

	//�ؼ�����
protected:
	CGameLogic						m_GameLogic;						//��Ϸ�߼�
	CHistoryScore					m_HistoryScore;						//��ʷ����
	CGameClientView					m_GameClientView;					//��Ϸ��ͼ

	//��������
public:
	//���캯��
	CGameClientEngine();
	//��������
	virtual ~CGameClientEngine();

	//���ƽӿ�
public:
	//��ʼ����
	virtual bool OnInitGameEngine();
	//��������
	virtual bool OnResetGameEngine();

	//�¼��ӿ�
public:
	//�Թ�״̬
	virtual bool OnEventLookonMode(VOID * pData, WORD wDataSize);
	//ʱ��ɾ��
	virtual bool OnEventGameClockKill(WORD wChairID);
	//ʱ����Ϣ
	virtual bool OnEventGameClockInfo(WORD wChairID, UINT nElapse, WORD wClockID);
	//������Ϣ
	virtual bool OnEventGameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//��Ϸ����
	virtual bool OnEventSceneMessage(BYTE cbGameStatus, bool bLookonOther, VOID * pData, WORD wDataSize);

	//�û��¼�
public:
	//�û�����
	virtual VOID OnEventUserEnter(IClientUserItem * pIClientUserItem, bool bLookonUser);
	//�û��뿪
	virtual VOID OnEventUserLeave(IClientUserItem * pIClientUserItem, bool bLookonUser);
	//�û�����
	virtual VOID OnEventUserScore(IClientUserItem * pIClientUserItem, bool bLookonUser);
	//�û�״̬
	virtual VOID OnEventUserStatus(IClientUserItem * pIClientUserItem, bool bLookonUser);

	//��Ϣ����
protected:
	//��Ϸ��ʼ
	bool OnSubGameStart(VOID * pData, WORD wDataSize);
	//�û�����
	bool OnSubGiveUp(VOID * pData, WORD wDataSize);
	//�û���ע
	bool OnSubAddScore(VOID * pData, WORD wDataSize);
	//�����˿�
	bool OnSubSendCard(VOID * pData, WORD wDataSize);
	//��Ϸ����
	bool OnSubGameEnd(VOID * pData, WORD wDataSize);
	//��ȡ��Ϣ
	bool OnSubGetWinner( VOID * pData, WORD wDataSize );

	//��������
protected:
	//�Զ���ʼ
	bool PerformAutoStart();
	//���ؿ���
	VOID HideScoreControl();
	//���¿���
	VOID UpdateScoreControl();
	//ִ�н���
	VOID PerformGameConclude();

	//��Ϣӳ��
protected:
	//��ʼ��Ϣ
	LRESULT OnMessageStart(WPARAM wParam, LPARAM lParam);
	//��ע��Ϣ
	LRESULT OnMessageFollow(WPARAM wParam, LPARAM lParam);
	//������Ϣ
	LRESULT OnMessageGiveUp(WPARAM wParam, LPARAM lParam);
	//��ע��Ϣ
	LRESULT OnMessageAddScore(WPARAM wParam, LPARAM lParam);
	//�����Ϣ
	LRESULT OnMessageShowHand(WPARAM wParam, LPARAM lParam);
	//�ر���Ϣ
	LRESULT OnMessageClose(WPARAM wParam, LPARAM lParam);
	//�������
	LRESULT OnMessageSendCardFinish(WPARAM wParam, LPARAM lParam);
	//������Ϣ
	LRESULT OnChatMessage( WPARAM wParam, LPARAM lParam );

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////

#endif