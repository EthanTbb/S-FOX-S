#ifndef GAME_CLIENT_ENGINE_HEAD_FILE
#define GAME_CLIENT_ENGINE_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "GameLogic.h"
#include "GameClientView.h"

//////////////////////////////////////////////////////////////////////////////////

//��Ϸ����
class CGameClientEngine : public CGameFrameEngine
{
	//��Ϸ����
protected:
	WORD							m_wBankerUser;						//ׯ���û�
	BYTE							m_bBombCount[3];					//ը����Ŀ
	BYTE							m_bCardCount[3];					//�˿���Ŀ
	BYTE							m_bHandCardCount;					//�˿���Ŀ
	BYTE							m_bHandCardData[16];				//�����˿�

	//���Ʊ���
protected:
	BYTE							m_bTurnOutType;						//��������
	BYTE							m_bTurnCardCount;					//������Ŀ
	BYTE							m_bTurnCardData[20];				//�����б�

	//���ñ���
protected:
	bool							m_bDeasilOrder;						//����˳��
	DWORD							m_dwCardHSpace;						//�˿�����

	//��������
protected:
	WORD							m_wMostUser;						//������
	WORD							m_wTimeOutCount;					//��ʱ����

	//�ؼ�����
protected:
	CGameLogic						m_GameLogic;						//��Ϸ�߼�
	CGameClientView					m_GameClientView;					//��Ϸ��ͼ
	//��������
public:
	//���캯��
	CGameClientEngine();
	//��������
	virtual ~CGameClientEngine();

	//ϵͳ�¼�
public:
	//��������
	virtual bool OnInitGameEngine();
	//���ú���
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
	//��Ϣ����
protected:
	//��Ϸ��ʼ
	bool OnSubGameStart(const void * pBuffer, WORD wDataSize);
	//�û�����
	bool OnSubOutCard(const void * pBuffer, WORD wDataSize);
	//��������
	bool OnSubPassCard(const void * pBuffer, WORD wDataSize);
	//��Ϸ����
	bool OnSubGameEnd(const void * pBuffer, WORD wDataSize);

	//��������
protected:
	//�����ж�
	bool VerdictOutCard();
	//�����ж�
	bool VerdictPassCard();
	//�Զ�����
	bool AutomatismOutCard();

	//��Ϣӳ��
protected:
	//��ʱ����Ϣ
	afx_msg void OnTimer(UINT nIDEvent);
	//��ʼ��Ϣ
	LRESULT OnStart(WPARAM wParam, LPARAM lParam);
	//������Ϣ
	LRESULT OnOutCard(WPARAM wParam, LPARAM lParam);
	//��������
	LRESULT OnPassCard(WPARAM wParam, LPARAM lParam);
	//������ʾ
	LRESULT OnOutPrompt(WPARAM wParam, LPARAM lParam);
	//�Ҽ��˿�
	LRESULT OnLeftHitCard(WPARAM wParam, LPARAM lParam);
	//����˿�
	LRESULT OnRightHitCard(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

#endif