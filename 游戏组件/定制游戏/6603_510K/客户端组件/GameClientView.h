#ifndef GAME_CLIENT_HEAD_FILE
#define GAME_CLIENT_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "CardControl.h"
#include "ScoreControl.h"
#include "HistoryRecord.h"

//////////////////////////////////////////////////////////////////////////
//�궨��

//��ͼλ��
#define MYSELF_VIEW_ID				2									//��ͼλ��

//������Ϣ
#define IDM_START					(WM_USER+200)						//��ʼ��Ϣ
#define IDM_OUT_CARD				(WM_USER+201)						//������Ϣ
#define IDM_PASS_CARD				(WM_USER+202)						//PASS��Ϣ
#define IDM_OUT_PROMPT				(WM_USER+203)						//��ʾ��Ϣ
#define	IDM_LEFT_HIT_CARD			(WM_USER+206)						//����˿�
#define IDM_SORT_HAND_CARD			(WM_USER+207)						//�����˿�
#define IDM_LAST_TURN_CARD			(WM_USER+208)						//�����˿�
#define IDM_TRUSTEE_CONTROL			(WM_USER+209)						//�йܿ���

//////////////////////////////////////////////////////////////////////////////////

//��Ϸ��ͼ
class CGameClientView : public CGameFrameViewGDI
{
	//��Ϸ����
protected:
	LONGLONG						m_lCellScore;						//��Ԫ����
	BYTE							m_cbMainValue;						//������ֵ
	BYTE							m_cbValueOrder[GAME_PLAYER];		//�ȼ���ֵ
	bool							m_bAutoPlayer[GAME_PLAYER];			//�й��û�
	LONGLONG						m_TurnScore;						//���ַ���
	LONGLONG						m_PlayerScore[GAME_PLAYER];			//�û�����
public:
	bool							m_bShowFirstCard;

	//�û�״̬
protected:
	bool							m_bUserPass[GAME_PLAYER];			//������־
	bool							m_bUserContinue[GAME_PLAYER];		//������־

	//��Ϸ״̬
protected:
	WORD							m_wWinOrder[GAME_PLAYER];			//ʤ���б�
	WORD							m_wPersistInfo[GAME_PLAYER][2];		//��Ϸ��Ϣ
	LONGLONG						m_nLButtonUpCount;					//�������

	//��ը����
protected:
	bool							m_bBombEffect;						//��ըЧ��
	BYTE							m_cbBombFrameIndex;					//֡������

	//�ƶ�����
protected:
	WORD							m_wHoverCardItem;					//��������
	WORD							m_wMouseDownItem;					//�������
	bool							m_bMoveMouse;						//�ƶ���־
	bool							m_bSwitchCard;						//������־
	bool							m_bSelectCard;						//ѡ���־
	CPoint							m_ptAuto[GAME_PLAYER];

	//״̬����
public:
	bool							m_bShowScore;						//����״̬
	bool							m_bLastTurnCard;					//���ֱ�־

	//���ư�ť
public:
	CSkinButton						m_btStart;							//��ʼ��ť
	CSkinButton						m_btStustee;						//�Ϲܿ���
	CSkinButton						m_btLastTurn;						//�����˿�
	CSkinButton						m_btSortCardCount;					//����ť
	CSkinButton						m_btSortCardColor;					//����ť
	CSkinButton						m_btSortCardOrder;					//����ť

	//��Ϸ��ť
public:
	CSkinButton						m_btOutCard;						//���ư�ť
	CSkinButton						m_btPassCard;						//PASS��ť
	//CSkinButton						m_btOutPrompt;						//��ʾ��ť
	CSkinButton						m_btSort;							//���ư�ť

	//��Ϸ�ؼ�
public:
	CToolTipCtrl					m_ToolTipCtrl;						//��ʾ�ؼ�
	CScoreControl					m_ScoreControl;						//���ֿؼ�

	//�˿˿ؼ�
public:
	CCardControl					m_HandCardControl[GAME_PLAYER];		//�û��˿�
	CCardControl					m_UserCardControl[GAME_PLAYER];		//�˿���ͼ
	CCardControl					m_5RecordCardControl;
	CCardControl					m_10RecordCardControl;
	CCardControl					m_KRecordCardControl;
	//λ�ñ���
protected:
	CPoint							m_ptHeap[GAME_PLAYER];				//�˿�λ��
	CPoint							m_ptPass[GAME_PLAYER];				//����λ��

	//��Դ����
protected:
	CBitImageEx						m_ImageNumber;						//������Դ
	CBitImageEx						m_ImageViewFill;					//������Դ
	CBitImageEx						m_ImageViewBack;					//������Դ
	CBitImageEx						m_ImageUserPass;					//������Դ
	CBitImageEx						m_ImageBombEffect;					//ը��Ч��
	CBitImageEx						m_ImageValueOrder;					//������Դ
	CBitImageEx						m_ImageStatusInfo;					//״̬��Ϣ
	CBitImageEx						m_ImageLastTurnTip;					//������ʾ
	CBitImageEx						m_ImageTurnScore;
	CPngImageEx						ImageScore;
	CPngImageEx						ImageWinOrder;
	//��������
public:
	//���캯��
	CGameClientView();
	//��������
	virtual ~CGameClientView();
	//���غ���
public:
	//��Ϣ����
	virtual BOOL PreTranslateMessage(MSG * pMsg);
	//WIN7֧��
	virtual bool RealizeWIN7() { return true; }
	//���غ���
private:
	//���ý���
	virtual VOID ResetGameView();
	//�����ؼ�
	virtual VOID RectifyControl(INT nWidth, INT nHeight);
	//�滭����
	virtual VOID DrawGameView(CDC * pDC, INT nWidth, INT nHeight);
	//�����
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);

	//���ܺ���
public:
	//��Ԫ����
	bool SetCellScore(LONGLONG lCellScore);
	//������ֵ
	bool SetMainValue(BYTE cbMainValue);
	//���ñ�ը
	bool SetBombEffect(bool bBombEffect);
	//��������
	VOID SetLastTurnCard(bool bLastTurnCard);
	//���÷���
	bool SetUserPass(WORD wChairID, bool bUserPass);
	//���ü���
	bool SetUserContinue(WORD wChairID, bool bContinue);
	//��������
	bool SetUserWinOrder(WORD wChairID, WORD wWinOrder);
	//������Ϣ
	bool SetUserPersist(WORD wChairID, WORD wPersistInfo[2]);
	//���õȼ�
	bool SetUserValueOrder(BYTE cbMySelfOrder, BYTE cbEnemyOrder);
	//������Ϸ����
	bool SetScoreInfo(LONGLONG TurnScore,LONGLONG PlayerScore[4]);
	//�й��û�
	void SetAutoUser(bool bAutoUser[4]);
	//�Ƽ�¼
	void Set510KRecord(BYTE bRecord[3][8]);

	//�ڲ�����
private:
	//�滭����
	VOID DrawNumberString(CDC * pDC, LONGLONG lNumber, INT nXPos, INT nYPos);

	//��Ϣ����
public:
	//ʱ����Ϣ
	VOID OnTimer(UINT nIDEvent);
	//��������
	INT OnCreate(LPCREATESTRUCT lpCreateStruct);
	//�����Ϣ
	VOID OnMouseMove(UINT nFlags, CPoint Point);
	//�����Ϣ
	VOID OnRButtonUp(UINT nFlags, CPoint Point);
	//�����Ϣ
	VOID OnLButtonUp(UINT nFlags, CPoint Point);
	//�����Ϣ
	VOID OnLButtonDown(UINT nFlags, CPoint Point);
	//�����Ϣ
	VOID OnLButtonDblClk(UINT nFlags, CPoint Point);
	//�����Ϣ
	BOOL OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

#endif