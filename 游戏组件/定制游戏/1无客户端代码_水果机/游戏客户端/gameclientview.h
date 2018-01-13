#ifndef GAME_CLIENT_VIEW_HEAD_FILE
#define GAME_CLIENT_VIEW_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "..\��Ϸ������\GameLogic.h"
#include "ClientControl.h"
#include "SkinButtonEx.h"


//////////////////////////////////////////////////////////////////////////

//���붨��
#define JETTON_COUNT				6									//������Ŀ
#define JETTON_RADII				68									//����뾶

//��Ϣ����
#define IDM_PLACE_JETTON			WM_USER+200							//��ס��Ϣ
#define IDM_APPLY_BANKER			WM_USER+201							//������Ϣ
#define IDM_CONTINUE_CARD			WM_USER+202							//��������
#define IDM_AUTO_OPEN_CARD			WM_USER+203							//��ť��ʶ
#define IDM_OPEN_CARD				WM_USER+204		
#define IDM_SHOW_RESULT				WM_USER+205		
#define IDM_SOUND					WM_USER+206		
#define IDM_CHAGE_MONEY				WM_USER+207							//�һ�����
#define IDM_CONTINUE_JETTON			WM_USER+208							//��Ѻ
#define	IDM_BIG_SMALL				WM_USER+209							//Ѻ��С
#define IDM_CLOSE					WM_USER+210							//�ر�
#define IDM_OK						WM_USER+211							//ȷ��
#define	IDM_CHANGE_GOLD				WM_USER+222							//�һ�����

//��������
#define INDEX_PLAYER				0									//�м�����
#define INDEX_BANKER				1									//ׯ������

//////////////////////////////////////////////////////////////////////////
//�ṹ����

//��ʷ��¼
#define MAX_SCORE_HISTORY			65									//��ʷ����
#define MAX_FALG_COUNT				10									//��ʶ����
#define COUNT_DRAWINFO              18

//������Ϣ
struct tagJettonInfo
{
	int								nXPos;								//����λ��
	int								nYPos;								//����λ��
	BYTE							cbJettonIndex;						//��������
	bool							bShow;
};

//�������
enum enOperateResult
{
	enOperateResult_NULL,
	enOperateResult_RedLucky,
	enOperateResult_GreenLucky,
	enOperateResult_Big,
	enOperateResult_Small
};

//��¼��Ϣ
struct tagClientGameRecord
{
	enOperateResult					enOperateMen[AREA_COUNT+1];					//������ʶ
	bool							bWinMen[AREA_COUNT+1];						//˳��ʤ��

};

//������ʾ
enum enDispatchCardTip
{
	enDispatchCardTip_NULL,
	enDispatchCardTip_Continue,											//��������
	enDispatchCardTip_Dispatch											//����ϴ��
};

struct  sT_ShowInfo
{
	CRect RcRect[AREA_COUNT+1];
	CPoint point[AREA_COUNT+1];
	bool  blShow[AREA_COUNT+1];
};
struct  sT_ShowJetton
{
	bool  blShow[6];
	sT_ShowJetton()
	{
		for (int i = 0;i<6;i++){
			blShow[i]=true;
		}
	}
};
//��������
typedef CWHArray<tagJettonInfo,tagJettonInfo&> CJettonInfoArray;

//������
class CGameClientEngine;
//////////////////////////////////////////////////////////////////////////

struct tagAndroidBet
{
	BYTE							cbJettonArea;						//��������
	LONGLONG						lJettonScore;						//��ע��Ŀ
	WORD							wChairID;							//���λ��
	int								nLeftTime;							//ʣ��ʱ�� (100msΪ��λ)
};

//��Ϸ��ͼ
class CGameClientView : public CGameFrameViewGDI
{
	//������Ϣ
protected:
	LONGLONG						m_lMeMaxScore;						//�����ע
    LONGLONG						m_lAreaLimitScore;					//��������
	LONGLONG						m_lRobotMaxJetton;					//������������

	//��ע��Ϣ
protected:
	LONGLONG						m_lUserJettonScore[AREA_COUNT+1];	//������ע
	LONGLONG						m_lAllJettonScore[AREA_COUNT+1];	//ȫ����ע	
	LONGLONG						m_lAllPlayBet[GAME_PLAYER][AREA_COUNT];//���������ע

	//λ����Ϣ
protected:
	int								m_nWinFlagsExcursionX;				//ƫ��λ��
	int								m_nWinFlagsExcursionY;				//ƫ��λ��
	int								m_nScoreHead;						//�ɼ�λ��
	CRect							m_rcTianMen;						//�м�����
	CRect							m_rcDimen;							//����������
	CRect							m_rcQiao;							//��������
	CRect							m_rcHuangMen;							//ƽ����
	CRect							m_rcXuanMen;							//ͬ��ƽ����
	CRect							m_rcHuang;							//ͬ��ƽ����
	CRect							m_rcJiaoR;							//ׯ������
	CRect							m_RectScore[AREA_COUNT];			//��ʾ��ע����
	int								m_OpenCardIndex;					//�_�����
	int								m_PostCardIndex;					//�l�����
	int								m_PostStartIndex;					//������ʼλ��
	CPoint							m_CardTypePoint[AREA_COUNT+1];
   

	//�˿���Ϣ
public:	
    BYTE							m_cbTableCardArray[5];			//�����˿�	
	bool							m_blMoveFinish;
	BYTE							m_bcfirstShowCard;
	BYTE							m_bcShowCount;
	bool							m_blAutoOpenCard;					//���Զ�
	LONGLONG						m_lUserCardType[AREA_COUNT+1];	//������ע
	CRect							m_JettonAreaRect[AREA_COUNT];
	CRect m_cTmpRect[64];
	sT_ShowJetton                   m_T_ShowJetton;	

	//��ʷ��Ϣ
protected:
	LONGLONG						m_lMeStatisticScore;				//��Ϸ�ɼ�
	tagClientGameRecord				m_GameRecordArrary[MAX_SCORE_HISTORY];//��Ϸ��¼
	int								m_nRecordFirst;						//��ʼ��¼
	int								m_nRecordLast;						//����¼

	//״̬����
protected:
	WORD							m_wMeChairID;						//�ҵ�λ��
	BYTE							m_cbAreaFlash;						//ʤ�����
	LONGLONG						m_lCurrentJetton;					//��ǰ����
	bool							m_bShowChangeBanker;				//�ֻ�ׯ��
	bool							m_bNeedSetGameRecord;				//�������
	bool							m_bWinTianMen;						//ʤ����ʶ
	bool							m_bWinHuangMen;						//ʤ����ʶ
	bool                            m_bWinDiMen;
	bool							m_bWinXuanMen;						//ʤ����ʶ
	bool							m_bFlashResult;						//��ʾ���
	bool							m_bShowGameResult;					//��ʾ���
	enDispatchCardTip				m_enDispatchCardTip;				//������ʾ	

	//ׯ����Ϣ
protected:	
	WORD							m_wBankerUser;						//��ǰׯ��
	WORD							m_wBankerTime;						//��ׯ����
	LONGLONG						m_lBankerScore;						//ׯ�һ���
	LONGLONG						m_lBankerWinScore;					//ׯ�ҳɼ�	
	LONGLONG						m_lTmpBankerWinScore;				//ׯ�ҳɼ�	
	bool							m_bEnableSysBanker;					//ϵͳ��ׯ

	//���ֳɼ�
public:
	LONGLONG						m_lMeCurGameScore;					//�ҵĳɼ�
	LONGLONG						m_lMeCurGameReturnScore[GAME_PLAYER];	//�ҵĳɼ�
	LONGLONG						m_lBankerCurGameScore;				//ׯ�ҳɼ�
	LONGLONG						m_lGameRevenue;						//��Ϸ˰��
	int								m_nTime;							//��ע����
	int								m_nExchange;						//�һ�����
	LONGLONG						m_lExchangeScore[GAME_PLAYER];		//�һ���ĳ���
	LONGLONG						m_lUserScore;						//�������ʣ����
	LONGLONG						m_lCanExchangeScore;				//��������ܶһ����ٳ���
	bool							m_bTip;								//�Ƿ���ʾ
	bool							m_bGuess;							//�Ƿ����
	int								m_nGuessNum;						//���ִ�С
	bool							m_bEffect;							//�Ƿ񲥷Ŷ���
	int								m_nProbability;							//�н�����

	//���ݱ���
protected:
	CPoint							m_PointJetton[AREA_COUNT];			//����λ��
	CPoint							m_PointJettonNumber[AREA_COUNT];	//����λ��
	CJettonInfoArray				m_JettonInfoArray[AREA_COUNT];		//��������
	bool							m_bWinFlag[AREA_COUNT];				//ʤ����ʶ
	BYTE							m_cbResulte[AREA_COUNT+1];			//�������
	int								m_TopHeight;
	int								m_LifeWidth;
	int								m_CurArea;
	int								m_nFlashTipIndex;					//��ʾ����
	int								m_nEffectIndex;						//����֡��

	//�һ�����
public:
	CPngImage						m_ImageChangeBack;					//�һ���ͼ
	CSkinButton						m_btChip100;						//100����
	CSkinButton						m_btChip1000;						//1000����
	CSkinButton						m_btChip10000;						//10000����
	CSkinButton						m_btChipClose;						//�رհ�ť
	CSkinButton						m_btChip_X;							//�رհ�ť
	CSkinButton						m_btChipOK;							//ȷ����ť
	CSkinButton						m_btChipAll;						//ȫ������
	bool							m_bShow;							//�Ƿ���ʾ

	//��������ע
	CWHArray<tagAndroidBet>	m_ArrayAndroid;

	//�ؼ�����
public:	
	CSkinButtonEx					m_btJetton100;						//���밴ť
	CSkinButtonEx					m_btJetton1000;						//���밴ť
	CSkinButtonEx					m_btJetton10000;					//���밴ť
	CSkinButtonEx					m_btJetton50000;					//���밴ť
	CSkinButtonEx					m_btJetton100000;					//���밴ť
	CSkinButtonEx					m_btJetton500000;					//���밴ť
	CSkinButtonEx					m_btJetton1000000;					//���밴ť
	CSkinButtonEx					m_btJetton5000000;					//���밴ť
	CSkinButton						m_btApplyBanker;					//����ׯ��
	CSkinButton						m_btCancelBanker;					//ȡ��ׯ��
	CSkinButton						m_btScoreMoveL;						//�ƶ��ɼ�
	CSkinButton						m_btScoreMoveR;						//�ƶ��ɼ�
	CSkinButton						m_btChageMoney;						//�һ�����
	CSkinButtonEx					m_btContinueJetton;					//��Ѻ
	CSkinButton						m_btSmall;							//С
	CSkinButton						m_btBig;							//��
	CSkinButton						m_btOneTime;						//1����ע��ť
	CSkinButton						m_btTenTime;						//10����ע��ť
	CSkinButton						m_bt100Time;						//100����ע��ť

	//CSkinButton						m_btBankerStorage;					//��ť
	//CSkinButton						m_btBankerDraw;						//ȡ�ť
	CSkinButton						m_btUp;								//��ť
	CSkinButton						m_btDown;							//ȡ�ť
	CButton							m_btOpenAdmin;						//ϵͳ����

	int								m_ShowImageIndex;
	int								m_CheckImagIndex;
	//�ؼ�����
public:
	INT								m_nShowValleyIndex;					//��ʾ��ׯ�б�
	CWHArray<WORD>					m_ValleysList;						//��ׯ�б�
	
	CGameClientEngine				*m_pGameClientDlg;					//����ָ��
	CGameLogic						m_GameLogic;						//��Ϸ�߼�

	CRect							m_MeInfoRect;						//
	int								m_Out_Bao_y;
	bool						    m_bShowBao;
	CRect							m_CarRect[24];		
	CPngImageEx						m_idb_selPng;
	int								m_CarIndex;	
	bool							m_blUsing;

	//����
public:
	HINSTANCE						m_hInst;
	IClientControlDlg*				m_pClientControlDlg;	

	//������Դ
public:
	CListCtrl						m_SkinListCtrl[AREA_COUNT];

	//����
public:
	bool							m_blCanStore;                       //�Ƿ��ܱ���
	bool							m_bShowJettonAn;
	int                             m_bShowJettonIndex;	

	//�������
protected:
	CBitImageEx						m_ImageViewFill;					//����λͼ
	CPngImageEx						m_ViewBackPng;						//����λͼ
	CPngImageEx						m_ImageWinFlags;					//��־λͼ
	CBitImageEx						m_ImageJettonView;					//������ͼ
	CPngImageEx						m_ImageTimeFlagPng;					//ʱ���ʶ
	CPngImage						m_ImageTip;							//��ʾͼƬ
	CRect							m_BaoPosion;

	//�߿���Դ
protected:	
	CBitImageEx						m_ImageMeBanker;					//�л�ׯ��
	CBitImageEx						m_ImageChangeBanker;				//�л�ׯ��
	CBitImageEx						m_ImageNoBanker;					//�л�ׯ��
	bool							m_blMoveShowInfo;

protected:

	CPngImageEx                         m_PngPushBox[7];
	CPngImageEx                         m_PngShowJetton[4];
	CPngImageEx                         m_PngShowLeaveHandle[4];
	CPngImageEx                         m_PngResult[4];
	CPngImageEx                         m_TimerCount_png;
	CPngImageEx							m_ImageMeScoreNumberPng;				//������ͼ
	CPngImageEx							m_ImageScoreNumberPng;
	CPngImageEx							m_PngFrameTianMen[AREA_COUNT];		//����
	CPngImageEx							m_ImageTimesFrame1;						//������
	CPngImageEx							m_ImageTimesFrame2;						//������
	CPngImage							m_ImageChipNum;							//�һ���������
	CPngImage							m_ImageEffectTimes;						//��������	
	CPngImage							m_EffectBigSanYuan;						//����
	CPngImage							m_EffectSmallSanYuan;						//����
	CPngImage							m_EffectBigSiXi;						//����
	CPngImage							m_EffectBackLeft;							//����
	CPngImage							m_EffectBackRight;							//����
	CPngImage							m_ImageEat;								//�Ե�
	
	bool							   m_bShowLeaveHandleAn;
	bool							   m_bOPenBoxAn;
	bool                               m_blShowLastResult;
	bool							   m_bShowResult;
	bool                               m_blShowResultIndex;	
	int                                m_bShowLeaveHandleIndex;	
	int                                m_bOPenBoxIndex;		
	LONGLONG						   m_lLastJetton;


	//������Դ
protected:	
	CPngImageEx						m_pngGameEnd;
	bool							m_DrawBack;
	bool							m_bEnablePlaceJetton;

	//��������
public:
	//���캯��
	CGameClientView();
	//��������
	virtual ~CGameClientView();

	//�̳к���
private:
	//���ý���
	virtual VOID ResetGameView();
	//�����ؼ�
	virtual VOID RectifyControl(INT nWidth, INT nHeight);
	//�滭����
	virtual VOID DrawGameView(CDC * pDC, INT nWidth, INT nHeight);
	//WIN7֧��
	virtual bool RealizeWIN7() { return true; }

	//���ú���
public:
	//������Ϣ
	void SetMeMaxScore(LONGLONG lMeMaxScore);
	//�ҵ�λ��
	void SetMeChairID(DWORD dwMeUserID);
	//������ע
	void SetMePlaceJetton(BYTE cbViewIndex, LONGLONG lJettonCount);
	//ׯ����Ϣ
	void SetBankerInfo(DWORD dwBankerUserID, LONGLONG lBankerScore);
	//ׯ�ҳɼ�
	void SetBankerScore(WORD wBankerTime, LONGLONG lWinScore) {m_wBankerTime=wBankerTime; m_lTmpBankerWinScore=lWinScore;}
	//ׯ�ҳɼ�
	void SetBankerScoreEX(LONGLONG lWinScore) { m_lBankerWinScore=lWinScore;}
	//���ֳɼ�
	void SetCurGameScore(LONGLONG lMeCurGameScore, LONGLONG lMeCurGameReturnScore, WORD wChairID);
	//���ó���
	void PlaceUserJetton(BYTE cbViewIndex, LONGLONG lScoreCount);
	//��������ע
	void AndroidBet(BYTE cbViewIndex, LONGLONG lScoreCount);
	//��������
	void SetAreaLimitScore(LONGLONG lAreaLimitScore);	
	//�����˿�
	void SetCardInfo(BYTE cbTableCardArray[]);
	//���ó���
	void SetCurrentJetton(LONGLONG lCurrentJetton);
	//��ʷ��¼
	void SetGameHistory(BYTE *bcResulte);
	//�ֻ�ׯ��
	void ShowChangeBanker( bool bChangeBanker );	
	//����ϵͳ��ׯ
	void EnableSysBanker(bool bEnableSysBanker) {m_bEnableSysBanker=bEnableSysBanker;}
	//������ʾ
	void SetDispatchCardTip(enDispatchCardTip DispatchCardTip);
	//ִ��ʣ�����еĻ��嶯��
	void PerformAllBetAnimation();
	//���û�����������
	void SetRobotMaxJetton(LONGLONG lRobotMaxJetton){ m_lRobotMaxJetton = lRobotMaxJetton; };
	//������ͼ
	void RefreshGameView();
	//��ȡ�һ�����
	void GetExchangeScore(LONGLONG lScore,int nExchange,LONGLONG lChangeGold);

	void SetFirstShowCard(BYTE bcCard);
	//ȡ������
	void ClearAreaFlash() { m_cbAreaFlash = 0xFF; }

	void SetEnablePlaceJetton(bool bEnablePlaceJetton);

	void UpdataJettonButton();

	void StartRunCar(int iTimerStep);

	void StartLuckyCar(int iTimer,BYTE cbIndex);

	void RuningCar(int iTimerStep);

	void StartFlashTip(bool bShow,bool bWin);

	void StartRandShowSide();

	CString NumberStringWithSpace(LONGLONG lNumber);
	//������
	void IsJettonDraw(CDC *pDC);
	//
	void SetClientEngine(CGameClientEngine *pClientEngine){m_pGameClientDlg = pClientEngine;};

	//������ע���
	void ControlBetDetection(BYTE cbViewIndex, LONGLONG lScoreCount, WORD wViewChair, WORD wChairID );	
	//���ÿ��ƶ���ʼ���ֵ
	void SetControlStartStorage(LONGLONG lStartStorage);
	int ifirstTimer;

	bool m_bFlashrandShow;

	bool m_blRungingCar;

	int iTimerStep;

	int iOpenSide;

	int iRunIndex;

	int iTotoalRun;

	CRect  m_RectArea[8];

	//��������
public:	
	//��ʼ����
	void DispatchCard();
	//��������
	void FinishDispatchCard( bool bRecord, BYTE cbIndex);
	//���ñ�ը
	bool SetBombEffect(bool bBombEffect, WORD wAreaIndex);
	//���ſ��ж���
	void PlayEffect(CDC *pDC ,int nWidth, int nHeight);

	//��������
public:
	//�����ע
	LONGLONG GetUserMaxJetton(BYTE cbJettonArea = 0);

	void  GetAllWinArea(BYTE bcWinArea[],BYTE bcAreaCount,BYTE InArea);

	//�滭����
protected:
	//��˸�߿�
	void FlashJettonAreaFrame(int nWidth, int nHeight, CDC *pDC);	

	//���溯��
public:
	//�������
	void CleanUserJetton();
	//����ʤ��
	void SetWinnerSide(bool blWin[],bool bSet);
	//��������
	void DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, int nXPos, int nYPos);
	//��������
	void DrawTextString( CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, CRect rcRect, UINT nDrawFormat, bool bBigFont=false);
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
	inline LONGLONG GetCurrentJetton() { return m_lCurrentJetton; }	

	void KillCardTime();

	void SwitchToCheck();

	void SwithToNormalView();

	void SetMoveCardTimer();

	void StartMove();

	void StartJetton_AniMationN();

	void StartOPenBox();

	void StartHandle_Leave();

	void StarShowResult();

	void MyDrawUserTimer(CDC * pDC, int nXPos, int nYPos, WORD wTime, WORD wTimerArea);

	//�ڲ�����
private:
	//��ȡ����
	BYTE GetJettonArea(CPoint MousePoint);
	//�滭����
	void DrawNumberString(CDC * pDC, LONGLONG lNumber, INT nXPos, INT nYPos,bool blTimer= false,bool bMeScore = false);
	//�滭����
	//void DrawNumberStringEX(CDC * pDC, LONGLONG lNumber, INT nXPos, INT nYPos,bool bChip=false);
	// �滭����
	void DrawNumber(CDC * pDC, CPngImageEx * ImageNumber, TCHAR * szImageNum, SCORE lOutNum, INT nXPos, INT nYPos, UINT uFormat = DT_LEFT);
	// �滭����
	void DrawNumber(CDC * pDC, CPngImageEx* ImageNumber, TCHAR * szImageNum, TCHAR* szOutNum ,INT nXPos, INT nYPos,  UINT uFormat = DT_LEFT);
	// �滭����
	void DrawNumber(CDC * pDC, CPngImage * ImageNumber, TCHAR * szImageNum, SCORE lOutNum, INT nXPos, INT nYPos, UINT uFormat = DT_LEFT);
	// �滭����
	void DrawNumber(CDC * pDC, CPngImage* ImageNumber, TCHAR * szImageNum, TCHAR* szOutNum ,INT nXPos, INT nYPos,  UINT uFormat = DT_LEFT);
	//�滭����
	void DrawNumberStringWithSpace(CDC * pDC, LONGLONG lNumber, INT nXPos, INT nYPos);
	//�滭����
	void DrawNumberStringWithSpace(CDC * pDC, LONGLONG lNumber, CRect rcRect, INT nFormat=-1);
	//�滭��ʶ
	void DrawWinFlags(CDC * pDC);
	//��ʾ���
	void ShowGameResult(CDC *pDC, int nWidth, int nHeight);
	//͸���滭
	bool DrawAlphaRect(CDC* pDC, LPRECT lpRect, COLORREF clr, FLOAT fAlpha);
	//�ƶ�Ӯ��
	void DeduceWinner(bool bWinMen[]);

	void DrawMoveInfo(CDC* pDC, CRect rcRect);

	void SetJettonHide(int ID);


	//��ť��Ϣ
protected:
	//��ׯ��ť
	afx_msg void OnApplyBanker();
	//��ׯ��ť
	afx_msg void OnCancelBanker();
	//�ƶ���ť
	afx_msg void OnScoreMoveL();
	//�ƶ���ť
	afx_msg void OnScoreMoveR();
	//�ֹ�����
	afx_msg void OnOpenCard();
	//�Զ�����
	afx_msg void OnAutoOpenCard();
	//���д��
	afx_msg void OnBankStorage();
	//����ȡ��
	afx_msg void OnBankDraw();

	afx_msg void OnUp();

	afx_msg void OnDown();


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
	afx_msg void OnLButtonUp(UINT nFlags, CPoint Point);
	//�����Ϣ
	afx_msg BOOL OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage);
	//�ؼ�����
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
    //����ƶ�
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);	
	//����Ա����
	afx_msg void OpenAdminWnd();

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};

//////////////////////////////////////////////////////////////////////////

#endif
