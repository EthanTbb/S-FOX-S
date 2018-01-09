#ifndef GAME_CLIENT_VIEW_HEAD_FILE
#define GAME_CLIENT_VIEW_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "CardControl.h"
#include "RecordGameList.h"
#include "ApplyUserList.h"
#include "..\��Ϸ������\GameLogic.h"
#include "AnimationWnd.h"
#include "ClientControl.h"
#include "ChatEdit.h"
#include "SkinListCtrl.h"
//////////////////////////////////////////////////////////////////////////

//���붨��
#define JETTON_RADII				68									//����뾶

//��Ϣ����
#define IDM_PLACE_JETTON			WM_USER+200							//��ס��Ϣ
#define IDM_APPLY_BANKER			WM_USER+201							//������Ϣ
#define IDM_CONTINUE_CARD			WM_USER+202							//��������
#define IDM_AUTO_OPEN_CARD			WM_USER+203							//��ť��ʶ
#define IDM_OPEN_CARD				WM_USER+204		
#define IDM_CHAT_MESSAGE			WM_USER+205							//��Ϸ������Ϣ
#define IDM_GAME_CLOSE				(WM_USER+206)						//��Ϸ�ر�

#define IDM_ADMIN_COMMDN			WM_USER+1000

//��������
#define INDEX_PLAYER				0									//�м�����
#define INDEX_BANKER				1									//ׯ������

//////////////////////////////////////////////////////////////////////////
//�ṹ����

//��ʷ��¼
#define MAX_SCORE_HISTORY			64									//��ʷ����
#define MAX_FALG_COUNT				63									//��ʶ����

//������Ϣ
struct tagJettonInfo
{
	int								nXPos;								//����λ��
	int								nYPos;								//����λ��
	BYTE							cbJettonIndex;						//��������
};

//�������
enum enOperateResult
{
	enOperateResult_NULL,
	enOperateResult_Win,
	enOperateResult_Lost
};


struct tagUserBetInfo
{
	BYTE cbAreaID;
	INT	 nChipsIndex;
};

//�ͻ��˻������·�
struct tagAndroidBet
{
	BYTE							cbJettonArea;						//��������
	SCORE							lJettonScore;						//��ע��Ŀ
	WORD							wChairID;							//���λ��
	int								nLeftTime;							//ʣ��ʱ�� (100msΪ��λ)
};

//��������
typedef CWHArray<tagJettonInfo,tagJettonInfo&> CJettonInfoArray;

//������
class CGameClientEngine;
//////////////////////////////////////////////////////////////////////////

//��Ϸ��ͼ
class CGameClientView : public CGameFrameViewGDI
{
	//������Ϣ
protected:
	SCORE							m_lMeMaxScore;						//�����ע
	SCORE							m_lAreaLimitScore;					//��������

	//��ע��Ϣ
protected:
	SCORE							m_lUserBet[AREA_COUNT];				//������ע
	SCORE							m_lAllUserBet[AREA_COUNT];			//ȫ����ע
	
	//λ����Ϣ
protected:
	int								m_nWinFlagsExcursionX;				//ƫ��λ��
	int								m_nWinFlagsExcursionY;				//ƫ��λ��
	CPoint							m_ptPlayerInfo;						//�����Ϣ
	CRect							m_rcDiceValue;						//�����ܼ�
	CRect							m_rcBetArea[AREA_COUNT];			//��ע����
	CRect						    m_RectUserList;						//�б�λ��
	CRect						    m_RectChartDisplay;					//����λ��
	CRect							m_rcNumArea[AREA_COUNT];			//��ע����
	CRect							m_rcMeNum[AREA_COUNT];				//��ע����

	//�˿���Ϣ
public:
	BYTE							m_cbPrintCount;						//��ʾ����
	BYTE							m_cbDiceValue[DICE_COUNT];
	BYTE							m_cbCurDirect;						//��ǰ��λ
	BYTE							m_cbCurRound;						//��ǰ�غ�
	BYTE							m_bcShowCount;
	bool							m_blMoveFinish;
	BYTE							m_bcfirstShowCard;
	BYTE							m_cbTableCardArray[DIRECT_COUNT][CARD_COUNT];			//�����˿�
	BYTE							m_cbTableSortCardArray[DIRECT_COUNT][CARD_COUNT];
	bool							m_bDrawResult;
	bool							m_bAnimatFinish;
	bool							m_bRecordWinner;					//�Ƿ��¼��Ӯ
	bool							m_bShowList;						//�Ƿ���ʾ����б�
	bool							m_bShowChat;						//�Ƿ���ʾ�����
	//��ʷ��Ϣ
protected:

	int								m_nRecordFirst;							//��ʼ��¼
	int								m_nRecordLast;							//����¼
	int								m_nRecordCount;
	SCORE							m_lMeStatisticScore;					//��Ϸ�ɼ�
	tagGameRecord					m_GameRecordArrary[MAX_SCORE_HISTORY];  //��Ϸ��¼
	//״̬����
protected:
	int								m_nWinMutiple[AREA_COUNT];
	WORD							m_wMeChairID;						//�ҵ�λ��
	BYTE							m_cbAreaFlash;						//ʤ�����
	int								m_nJettonIndex;						//��������
	LONG							m_lCurrentJetton;					//��ǰ����
	bool							m_bShowRecord;
	bool							m_bShowChangeBanker;				//�ֻ�ׯ��
	bool							m_bNeedSetGameRecord;				//�������
	bool							m_bWinFlag[AREA_COUNT];
	bool							m_bFlashResult;						//��ʾ���
	bool							m_bShowGameResult;					//��ʾ���
	BYTE                            m_cbGameStatus;						//״̬��Ϣ
	//ׯ����Ϣ
protected:	
	WORD							m_wBankerUser;						//��ǰׯ��
	WORD							m_wBankerTime;						//��ׯ����
	bool							m_bEnableSysBanker;					//ϵͳ��ׯ
	SCORE							m_lBankerScore;						//ׯ�һ���
	SCORE							m_lBankerWinScore;					//ׯ�ҳɼ�	
	SCORE							m_lTmpBankerWinScore;				//ׯ�ҳɼ�	

	//���ֳɼ�
public:
	LONG							m_lGameRevenue;						//��Ϸ˰��
	SCORE							m_lMeCurGameScore;					//�ҵĳɼ�
	SCORE							m_lMeCurGameReturnScore;			//�ҵĳɼ�
	SCORE							m_lBankerCurGameScore;				//ׯ�ҳɼ�


	//���ݱ���
protected:
	CPoint							m_ptRecordPos;						//��¼��ʼλ��
	CPoint							m_ptRecordBkPos;					//��¼����λ��
	CPoint							m_ptRecordPosEx;
	CPoint							m_ptDicePos[DICE_COUNT];			//����λ��
	CPoint							m_ptRtDicePos[DICE_COUNT];			//����λ��
	CPoint							m_PointJetton[AREA_COUNT];			//����λ��
	CPoint							m_PointJettonNumber[AREA_COUNT];	//����λ��
	CPoint							m_MeJettonNumber[AREA_COUNT];	//����λ��
	CJettonInfoArray				m_JettonInfoArray[AREA_COUNT];		//��������
	//�ؼ�����
public:
	CSkinButton						m_btCloseRecord;					//�رռ�¼
	CSkinButton						m_btScrollUp;						//�Ϸ���ť
	CSkinButton						m_btScrollDown;						//�·���ť
	CSkinButton						m_btLookRecord;						//�鿴��¼
	CSkinButton						m_btApplyBanker;					//����ׯ��
	CSkinButton						m_btCancelBanker;					//ȡ��ׯ��
	CSkinButton						m_btJetton[JETTON_COUNT];			//���밴ť
	CSkinButton						m_btChat;							//���찴ť
	CSkinButton						m_btUserList;						//����б�ť
	CSkinButton						m_btSendMessage;					//������Ϣ
public:
	CButton							m_btOpenAdmin;						//��������
	//�����ؼ�
public:
	CGameCartoon					m_FrameDice;						//��λ����
	CAnimationWnd					m_CartoonDice;						//��������

	CRect							m_rcAnimatWnd;						//����λ��
	CPngImage						m_PngAnimatDice;					//����ͼƬ
	//����ģ��
public:
	//CDlgBank						m_DlgBank;							//���пؼ�
	//CSkinButton						m_btBankStorage;					//��ť
	CSkinButton						m_btBankDraw;						//ȡ�ť
	bool							m_blCanStore;                       //�Ƿ��ܱ���
	//�ؼ�����
public:
	INT								m_nShowValleyIndex;					//��ʾ��ׯ�б�
	CWHArray<WORD>					m_ValleysList;						//��ׯ�б�
	CGameLogic						m_GameLogic;						//��Ϸ�߼�
	CGameRecord						m_GameRecord;						//��¼�б�
	CGameClientEngine				*m_pGameClientEngine;				//����ָ��
	CSkinListCtrlEx                 m_UserList;							//�б�ؼ�
	CChatEdit						m_ChatDisplay;						//��ʾ�ؼ�
	CEdit							m_ChatInput;						//����ؼ�
	CBrush							m_brush;							//������ˢ
	//�������
protected:
	CPngImage						m_PngDice3D;
	CPngImage						m_PngDice2D;
	CPngImage						m_PngRecord;
	CPngImage						m_PngFlagsTip;						//��ע����
	CPngImage						m_PngTimeNumber;					//ʱ������
	CPngImage						m_PngAreaFrame[52];					//�߿�ͼƬ
	CPngImage						m_PngTimerBackGround;				//ʱ�ӱ���
	CPngImage						m_PngScoreNumber;					//������Դ
	CPngImage						m_PngMeScoreNumber;					//������Դ
	CPngImage						m_ImageUserBack;					//��Ϸ����б�
	CPngImage						m_ImageChatBack;					//��Ϸ��Ϣ��
	CPngImage						m_ImageJettonBack;					//��ע���ֵ׿�
	CPngImage						m_ImageMeJetton;					//�Լ���ע�׿�

	CPngImage						m_PngCommer;						//����

	CBitImage						m_ImageTimeFlag;					//ʱ���ʶ
	CBitImage						m_ImageViewFill;					//����λͼ
	CBitImage						m_ImageViewBack;					//����λͼ
	CBitImage						m_ImageWinFlags;					//��־λͼ
	CPngImage						m_ImageJettonView;					//������ͼ
	CBitImage						m_ImageScoreNumber;					//������ͼ
	CBitImage						m_ImageMeScoreNumber;				//������ͼ
	CBitImage						m_ImageDispatchCardTip;				//������ʾ

	//�߿���Դ
protected:
	CBitImage						m_ImageFrame;						//�߿�ͼƬ
	CPngImage						m_ImageNoBanker;					//�л�ׯ��
	CPngImage						m_ImageMeBanker;					//�л�ׯ��
	CPngImage						m_ImageChangeBanker;				//�л�ׯ��

	//������Դ
protected:
	CPngImage						m_ImageGameEnd;						//�ɼ�ͼƬ

	//����
public:
	HINSTANCE						m_hInst;
	IClientControlDlg*				m_pClientControlDlg;

	//��������
public:
	//���캯��
	CGameClientView();
	//��������
	virtual ~CGameClientView();

	//�̳к���
private:
	//���ý���
	virtual void ResetGameView();
	//�����ؼ�
	virtual void RectifyControl(INT nWidth, INT nHeight);
	//�滭����
	virtual void DrawGameView(CDC * pDC, INT nWidth, INT nHeight);
	//WIN7֧��
	virtual bool RealizeWIN7() { return true; }

	//������Ϸ��¼
	void DrawGameRecord(CDC*pDC);
	//�滭ʱ��
	VOID DrawUserTimer(CDC * pDC, int nXPos, int nYPos, WORD wTime);

	//���ú���
public:
	void SetFlashArea(BYTE cbFlashArea){m_cbAreaFlash=cbFlashArea;}
	//�ҵ�λ��
	void SetMeChairID(WORD dwMeUserID);
	//������Ϣ
	void SetMeMaxScore(SCORE lMeMaxScore);
	//������ע
	void SetMePlaceJetton(BYTE cbViewIndex, SCORE lJettonCount);
	//ׯ����Ϣ
	void SetBankerInfo(DWORD dwBankerUserID, SCORE lBankerScore);
	//ׯ�ҳɼ�
	void SetBankerScore(WORD wBankerTime, SCORE lWinScore) {m_wBankerTime=wBankerTime; m_lTmpBankerWinScore=lWinScore;}
	//���ֳɼ�
	void SetCurGameScore(SCORE lMeCurGameScore, SCORE lMeCurGameReturnScore, SCORE lBankerCurGameScore, LONG lGameRevenue);
	//��ʷ��¼
	void SetGameHistory(BYTE *cbDiceValue);
	//���ó���
	void SetCurrentJetton(LONG lCurrentJetton,int nJettonIndex);
	//�ֻ�ׯ��
	void ShowChangeBanker( bool bChangeBanker );
	//��������
	void SetAreaLimitScore(SCORE lAreaLimitScore);
	//���ó���
	void PlaceUserJetton(BYTE cbViewIndex, SCORE lScoreCount,bool bReDraw=true);
	//�����˿�
	void SetDiceValue(BYTE cbDiceValue[]);	
	//�ɼ�����
	void SetGameScore(SCORE lMeCurGameScore, SCORE lMeCurGameReturnScore, SCORE lBankerCurGameScore);
	//����ϵͳ��ׯ
	void EnableSysBanker(bool bEnableSysBanker) {m_bEnableSysBanker=bEnableSysBanker;}
	//�״���ʾ
	void SetFirstShowCard(BYTE bcCard);
	//��Ϸ����
	void OnGameEnd(const CMD_S_GameEnd * pGameEnd);
	//ȡ�����
	void OnBankDraw();
	//������
	void OnBankStorage();
	//����״̬
	void SetGameStatus(BYTE cbStatus){m_cbGameStatus=cbStatus;}
	BYTE GetClientViewStatus(){ return m_cbGameStatus;}
	// ��Ӷ���
	CString AddComma( LONGLONG lScore , bool bPlus = false);
	//��������
public:	

	//���ñ�ը
	bool SetBombEffect(bool bBombEffect, WORD wAreaIndex);

	//��������
public:
	//�����ע
	SCORE GetUserMaxBet(BYTE cbAreaID);
	//������ͼ
	void RefreshGameView();

	//�滭����
protected:
	void MakeString(CString &strNum,SCORE lNumber);
	//��˸�߿�
	void FlashAreaFrame(int nWidth, int nHeight, CDC *pDC);

	//���溯��
public:
	//�������
	void CleanUserJetton();
	//����ʤ��
	void SetWinnerSide(bool *bWinArea, bool bSet);
	//��������
	void DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame,CRect*rect,UINT nFormat=DT_LEFT);
	//��������
	void DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, int nXPos, int nYPos);
	//�滭����
	void DrawMeJettonNumber(CDC *pDC);
	//�滭ׯ��
	void DrawBankerInfo(CDC *pDC,int nWidth,int nHeight);
	//�滭���
	void DrawMeInfo(CDC *pDC,int nWidth,int nHeight);
	// �滭��ׯ�б�
	void DrawBankerList(CDC *pDC, int nWidth, int nHeight);

	//��������
public:
	//��ǰ����
	inline LONG GetCurrentJetton() { return m_lCurrentJetton; }	
	//ɾ����ʱ��
	void KillCardTime();

	//�ڲ�����
private:
	CString NumberStringWithSpace(SCORE lNumber);

	BYTE GetFramePngIndex(BYTE cbAreaIndex);
	//��ȡ����
	BYTE GetJettonArea(CPoint MousePoint);
	//�滭����
	void DrawNumberString(CDC * pDC, LONG lNumber, INT nXPos, INT nYPos, bool bMeScore = false);
	//�滭����
	void DrawNumberStringWithSpace(CDC * pDC, LONGLONG lNumber, INT nXPos, INT nYPos);
	//�滭����
	void DrawNumberStringWithSpace(CDC * pDC, LONGLONG lNumber, CRect rcRect, INT nFormat=-1);
	// �滭����
	void DrawNumber(CDC * pDC, CPngImage* ImageNumber, TCHAR * szImageNum, LONGLONG lOutNum, INT nXPos, INT nYPos, UINT uFormat = DT_LEFT);
	// �滭����
	void DrawNumber(CDC * pDC, CPngImage* ImageNumber, TCHAR * szImageNum, CString szOutNum, INT nXPos, INT nYPos, UINT uFormat = DT_LEFT);
	// �滭����
	void DrawNumber(CDC * pDC, CPngImage* ImageNumber, TCHAR * szImageNum, TCHAR* szOutNum ,INT nXPos, INT nYPos,  UINT uFormat = DT_LEFT);
	//��ʾ���
	void ShowGameResult(CDC *pDC, int nWidth, int nHeight);
	//͸���滭
	bool DrawAlphaRect(CDC* pDC, LPRECT lpRect, COLORREF clr, FLOAT fAlpha);
	//ׯ����С�÷�
	SCORE GetBankerWinMinScore(BYTE cbAreaID);	

	//��������
public:
	void OnTimeGameEnd();
	//ϵͳ��Ϣ
	void InsertSystemMessage(LPCTSTR pszString);
	//������Ϣ
	void InsertChatMessage(LPCTSTR pszString);
	//������Ϣ
	void InsertNormalMessage(LPCTSTR pszString);
	//������Ϣ
	void InsertAllChatMessage(LPCTSTR pszString, int nMessageType);
	//��Ϣ����
	void InsertMessage(LPCTSTR pszString,COLORREF color=RGB(193,167,108));
	//��ť��Ϣ
protected:
	//�Ϸ���ť
	afx_msg void OnUp();
	//�·���ť
	afx_msg void OnDown();
	//����Ա����
	afx_msg void OpenAdminWnd();
	//��ׯ��ť
	afx_msg void OnApplyBanker();
	//��ׯ��ť
	afx_msg void OnCancelBanker();
	//������Ϣ
	afx_msg void OnSendMessage();

	//��Ϣӳ��
protected:
	//��ʱ����Ϣ
	afx_msg void OnTimer(UINT nIDEvent);
	//��������
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//�����Ϣ
	afx_msg void OnLButtonDown(UINT nFlags, CPoint Point);	
	//�����Ϣ
	afx_msg void OnRButtonDown(UINT nFlags, CPoint Point);	
	//�����Ϣ
	afx_msg BOOL OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage);
	//�ؼ�����
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//�����ؼ����UP��Ϣ
	LRESULT OnViLBtUp(WPARAM wParam, LPARAM lParam);
	//��������
	LRESULT OnAnimatEnd(WPARAM wParam,LPARAM lParam);

	DECLARE_MESSAGE_MAP()	
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};

//////////////////////////////////////////////////////////////////////////

#endif
