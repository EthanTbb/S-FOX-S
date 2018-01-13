#ifndef GAME_CLIENT_HEAD_FILE
#define GAME_CLIENT_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "ScoreView.h"
#include "CardControl.h"
//////////////////////////////////////////////////////////////////////////
//��Ϣ����

#define IDM_START					(WM_USER+100)						//��ʼ��Ϣ
#define IDM_OUT_CARD				(WM_USER+101)						//������Ϣ
#define IDM_PASS_CARD				(WM_USER+102)						//��������
#define IDM_OUT_PROMPT				(WM_USER+103)						//��ʾ����

//////////////////////////////////////////////////////////////////////////////////

//��Ϸ��ͼ
class CGameClientView : public CGameFrameViewGDI
{
	//��Ϸ����
protected:
	LONGLONG							m_lCellScore;						//��������

	//���ñ���
protected:
	bool							m_bDeasilOrder;						//����˳��

	//״̬����
public:
	bool							m_bPass[GAME_PLAYER];				//��������
	BYTE							m_bBombCount[GAME_PLAYER];			//ը����Ŀ
	BYTE							m_bCardCount[GAME_PLAYER];			//�˿���Ŀ
	CPoint							m_PointUserPass[GAME_PLAYER];		//����λ��
	CPoint							m_PointUserBomb[GAME_PLAYER];		//ը��λ��

	//��ʷ����
public:
	LONGLONG                            m_lAllTurnScore[3];					//�ֵܾ÷�
	LONGLONG                            m_lLastTurnScore[3];				//�Ͼֵ÷�

	//λͼ����
protected:
	CBitImageEx						m_ImageBomb;						//ը����Դ
	CBitImageEx						m_ImageBack;						//������Դ
	CBitImageEx						m_ImageFill;						//������Դ
	CBitImageEx						m_ImageGameInfo;					//��Ϸ��Ϣ
	CBitImageEx						m_ImageUserPass;					//������־

	//��ť�ؼ�
public:
	CSkinButton						m_btStart;							//��ʼ��ť
	CSkinButton						m_btOutCard;						//���ư�ť
	CSkinButton						m_btPassCard;						//������ť
	CSkinButton						m_btOutPrompt;						//��ʾ��ť

	//�˿˿ؼ�
public:
	CCardControl					m_HandCardControl;					//�����˿�
	CCardControl					m_UserCardControl[3];				//�˿���ͼ
	CCardControl					m_LeaveCardControl[2];				//�����˿�

	//�ؼ����
public:
	CScoreView						m_ScoreView;						//������ͼ
	//��������
public:
	//���캯��
	CGameClientView();
	//��������
	virtual ~CGameClientView();
	//WIN7֧��
	virtual bool RealizeWIN7() { return true; }
	//���غ���
private:
	//���ý���
	virtual VOID ResetGameView();
	//�����ؼ�
	virtual VOID RectifyControl(INT nWidth, INT nHeight);
	//�滭����
	virtual VOID DrawGameView(CDC * pDC, int nWidth, int nHeight);
	//�����
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//���ú���
public:
	//�û�˳��
	void SetUserOrder(bool bDeasilOrder);
	//��ȡ˳��
	bool IsDeasilOrder() { return m_bDeasilOrder; }

	//���ܺ���
public:
	//��������
	void SetCellScore(LONGLONG lCellScore);
	//���÷���
	void SetPassFlag(WORD wChairID, bool bPass);
	//ը����Ŀ
	void SetBombCount(WORD wChairID, BYTE bBombCount);
	//�˿���Ŀ
	void SetCardCount(WORD wChairID, BYTE bCardCount);

	//��Ϣӳ��
protected:
	//��������
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//�����Ϣ
	afx_msg void OnRButtonUp(UINT nFlags, CPoint Point);
	//���˫��
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

#endif