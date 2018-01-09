#ifndef GAME_CLIENT_VIEW_HEAD_FILE
#define GAME_CLIENT_VIEW_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "CardControl.h"
#include "RecordGameList.h"
#include "ApplyUserList.h"
#include "../��Ϸ������/GameLogic.h"
#include "ClientControl.h"
#include "flashpaly\ShowLotteryFlash.h"
//////////////////////////////////////////////////////////////////////////
//����
enum enCartoonIds
{
	CTN_CHANGE_BANKER = 0,												//�ֻ�ׯ��
	CTN_BANKER_MAX,
	CTN_RESULT_FLASH,													//��Ӯѡ�����˸
	CTN_OPEN_CARD,														//����
	CTN_POST_CARD,														//����
	CTN_DISPATCH_CARD,													//���Ʊ�ʶ
	CTN_DISPATCH_TIP,													//������ʾ
	CTN_STAR_SELECT,													//����ѡ��	
	CTN_FLASH_WAIT,														//FLASH�����ȴ�  
	CTN_FLASH_EFFECT,													//FLASH����  
	CTN_COUNT,
};
struct stuCartoonInfo
{
	bool							bShow;							//�Ƿ񶯻�
	int								nShowFrame;						//��������
	DWORD							dwShowTime;						//���嶯��ʱ��
	int								nShowTimer;						//ˢ�¼�������룩
};
const conCartoonTimer[CTN_COUNT] = {30,30,500,50,500,30,2000,500,4000,40};

//���붨��
#define JETTON_COUNT				6									//������Ŀ
#define JETTON_RADII				68									//����뾶

//��Ϣ����
#define IDM_PLACE_JETTON			WM_USER+200							//��ס��Ϣ
#define IDM_APPLY_BANKER			WM_USER+201							//������Ϣ
#define IDM_CONTINUE_CARD			WM_USER+202							//��������
#define IDM_AUTO_OPEN_CARD			WM_USER+203							//��ť��ʶ
#define IDM_OPEN_CARD				WM_USER+204		
#define IDM_MANAGE_CONTROL			WM_USER+205							//������Ϣ
#define IDM_OPEN_SOUND			    WM_USER+206							//��ť��ʶ
#define IDM_CLOSE_SOUND				WM_USER+207		
#define IDM_POSTCARD_SOUND          WM_USER+208	                        //�������
#define IDM_START				    WM_USER+209				        	//��ʼ��Ϣ


//��������
#define INDEX_PLAYER				0									//�м�����
#define INDEX_BANKER				1									//ׯ������

//////////////////////////////////////////////////////////////////////////
//�ṹ����

//��ʷ��¼
//#ifdef _DEBUG
//#define MAX_SCORE_HISTORY			18									//��ʷ����
//#else
//#define MAX_SCORE_HISTORY			65									//��ʷ����
//#endif

#define MAX_SCORE_HISTORY			65									//��ʷ����

#define MAX_FALG_COUNT				19									//��ʶ����

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

//��¼��Ϣ
struct tagClientGameRecord
{
	enOperateResult					enOperateShunMen;					//������ʶ
	enOperateResult					enOperateDuiMen;					//������ʶ
	enOperateResult					enOperateDaoMen;					//������ʶ
	BYTE							bWinShunMen;						//˳��ʤ��
	BYTE							bWinDuiMen;							//����ʤ��
	BYTE							bWinDaoMen;							//����ʤ��
	BYTE							bWinShunMenSame;						//˳��ʤ��
	BYTE							bWinDuiMenSame;							//����ʤ��
	BYTE							bWinDaoMenSame;							//����ʤ��
};

//������ʾ
enum enDispatchCardTip
{
	enDispatchCardTip_NULL,
	enDispatchCardTip_Continue,											//��������
	enDispatchCardTip_Dispatch											//����ϴ��
};

//��������
typedef CWHArray<tagJettonInfo,tagJettonInfo&> CJettonInfoArray;

//������
class CGameClientEngine;
//////////////////////////////////////////////////////////////////////////

//��Ϸ��ͼ
class CGameClientView : public CGameFrameViewD3D//CGameFrameViewGDI
{
	//������Ϣ
protected:
	LONGLONG						m_lMeMaxScore;						//�����ע
    LONGLONG						m_lAreaLimitScore;					//��������
	LONGLONG						m_lUserLimitScore;					//������ע

	//��ע��Ϣ
protected:
	LONGLONG						m_lUserJettonScore[AREA_ARRY_COUNT];	//������ע
	LONGLONG						m_lAllJettonScore[AREA_ARRY_COUNT];	//ȫ����ע
	LONGLONG						m_lAllPlayBet[GAME_PLAYER][AREA_ARRY_COUNT];//���������ע

	//λ����Ϣ
protected:
	int								m_nWinFlagsExcursionX;				//ƫ��λ��
	int								m_nWinFlagsExcursionY;				//ƫ��λ��
	int								m_nScoreHead;						//�ɼ�λ��
	CRect							m_rcShunMen;						//�м�����
	CRect							m_rcJiaoL;							//����������
	CRect							m_rcQiao;							//��������
	CRect							m_rcDuiMen;							//ƽ����
	CRect							m_rcDaoMen;							//ͬ��ƽ����
	CRect							m_rcJiaoR;							//ׯ������
	int								m_OpenCardIndex;					//�_�����
	int								m_PostCardIndex;					//�l�����
	int								m_PostStartIndex;					//������ʼλ��
	

	//�˿���Ϣ
public:	
    BYTE							m_cbTableCardArray[4][2];			//�����˿�
	bool							m_blMoveFinish;
	BYTE							m_bcfirstShowCard;
	BYTE							m_bcShowCount;
	bool							m_blAutoOpenCard;					//���Զ�
	bool							m_bBankerMax;						//�Ƿ�ﵽׯ������
	bool                            m_bShowHalf;
	int								m_nRecordLast;						//����¼
	bool                            m_bToBankerMax;

	//��ʷ��Ϣ
protected:
	LONGLONG						m_lMeStatisticScore;				//��Ϸ�ɼ�
	tagClientGameRecord				m_GameRecordArrary[MAX_SCORE_HISTORY];//��Ϸ��¼
	int								m_nRecordFirst;						//��ʼ��¼

	//״̬����
protected:
	WORD							m_wMeChairID;						//�ҵ�λ��
	BYTE							m_cbAreaFlash;						//ʤ�����
	LONGLONG						m_lCurrentJetton;					//��ǰ����
	bool							m_bShowChangeBanker;				//�ֻ�ׯ��
	bool							m_bNeedSetGameRecord;				//�������
	BYTE							m_bWinShunMen;						//ʤ����ʶ
	BYTE							m_bWinDuiMen;						//ʤ����ʶ
	BYTE							m_bWinDaoMen;						//ʤ����ʶ	
	bool							m_bShowGameResult;					//��ʾ���
	enDispatchCardTip				m_enDispatchCardTip;				//������ʾ
public:
	//����
	stuCartoonInfo					m_stuCartoonInfo[CTN_COUNT];

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
	LONGLONG						m_lMeCurGameReturnScore;			//�ҵĳɼ�
	LONGLONG						m_lBankerCurGameScore;				//ׯ�ҳɼ�
	LONGLONG						m_lGameRevenue;						//��Ϸ˰��

	//���ݱ���
protected:
	CPoint							m_PointJetton[JETTON_COUNT];			//����λ��
	CPoint							m_PointJettonNumber[JETTON_COUNT];	//����λ��
	CJettonInfoArray				m_JettonInfoArray[JETTON_COUNT];		//��������
	COLORREF                        m_CurSelectColor;					//��ǰ��ɫ
	//�ؼ�����
public:
	CVirtualButton					m_btJetton100;						//���밴ť
	CVirtualButton					m_btJetton1000;						//���밴ť
	CVirtualButton					m_btJetton10000;					//���밴ť
	CVirtualButton					m_btJetton100000;					//���밴ť
	CVirtualButton					m_btJetton500000;					//���밴ť
	CVirtualButton					m_btJetton1000000;					//���밴ť
	CVirtualButton					m_btJetton5000000;					//���밴ť
	CVirtualButton					m_btJetton10000000;					//���밴ť
	CVirtualButton					m_btApplyBanker;					//����ׯ��
	CVirtualButton					m_btCancelBanker;					//ȡ��ׯ��
	CVirtualButton					m_btScoreMoveL;						//�ƶ��ɼ�
	CVirtualButton					m_btScoreMoveR;						//�ƶ��ɼ�
	CVirtualButton					m_btAutoOpenCard;					//�Զ�����
	CVirtualButton					m_btOpenCard;						//�ֶ�����
	CVirtualButton					m_btBank;							//���а�ť
	CVirtualButton					m_btContinueCard;					//��������
	CVirtualButton				    m_btBankerStorage;					//��ť
	CVirtualButton					m_btBankerDraw;						//ȡ�ť
	//CVirtualButton                  m_btStart;                          //��ʼ��ť
	CVirtualButton					m_btUp;					            //��ť
	CVirtualButton					m_btDown;
	CButton							m_btOpenAdmin;                      //����Ա����

	CD3DTexture						m_btTexJetton1000;					//���밴ť
	CD3DTexture						m_btTexJetton10000;					//���밴ť
	CD3DTexture						m_btTexJetton100000;					//���밴ť
	CD3DTexture						m_btTexJetton500000;					//���밴ť
	CD3DTexture						m_btTexJetton1000000;					//���밴ť
	CD3DTexture						m_btTexJetton5000000;					//���밴ť
	CD3DTexture						m_btTexJetton10000000;				//���밴ť

	//�ؼ�����
public:
	CApplyUser						m_ApplyUser;						//�����б�
	CGameRecord						m_GameRecord;						//��¼�б�
	CCardControl					m_CardControl[5];					//�˿˿ؼ�	
	CGameClientEngine				*m_pGameClientEngine;				//����ָ��
	CGameLogic						m_GameLogic;						//��Ϸ�߼�
	bool							m_blCanStore;						//�Ƿ��ܱ���
	
public:
	//˳�Ե���ͼ����
	CRect							m_rcShunMenBack[3];					//˳�Ե�����
	//
	CRect							m_rcBack;
	CD3DTexture						m_TexApplyBlockBack;				//��ׯ�б����
	CD3DTexture						m_TexApplyBlock;					//��ׯ�б���ͼ
	CRect							m_rcApplyBlock;						//��ׯ�б���
	int								m_nApplyFocusIndex;					//
	bool							m_bApplyLBDown;						//�Ƿ�����ڻ����ϰ���
	CD3DTexture						m_TexScoreBack;						//��ע��ͼ
	//ѡ��Ů
	CD3DTexture						m_TexGod;							//����
	CD3DTexture						m_TexProgress;						//������
	CD3DTexture						m_TexProgressBack;					//��������
	CD3DTexture						m_TexProgressNum;					//����������
	CD3DTexture						m_TexStar;							//����ȫͼ
	CD3DTexture						m_TexStarShow[3];					//����ѡ��
	BYTE							m_cbStarSelect[3];
	float							m_fStarWinRate[STAR_COUNT];
	LONGLONG						m_llStarAllCount[STAR_COUNT];
	LONGLONG						m_llStarWinCount[STAR_COUNT];
	LONGLONG						m_llAllStarCount;
	bool							m_bSelectStar;
	//Falsh����
	CShowLotteryFlash2				m_ShowLotteryFlash;
	CShowLotteryFlash2				m_ShowLotteryFlashArray[2];
	bool							m_bFlashShow;						//flash����
	WORD							m_wFlashId;
	CPoint							m_ptGodBack;						//����λ��
	CPoint                          m_ptFlash[2];						//0����1ɸԪ��

	//����
public:
	HINSTANCE						m_hInst;
	IClientControlDlg*				m_pClientControlDlg;		

	//�������
public:
	CD3DDevice *					m_pD3DDevice;
protected:	
	CD3DTexture						m_TexViewFill;						//����λͼ
	CD3DTexture						m_TexViewBack;						//����λͼ
	CD3DTexture						m_TexWinFlags;					//��־λͼ
	CD3DTexture						m_TexJettonView;					//������ͼ
	CD3DTexture						m_TexScoreNumber;					//������ͼ
	CD3DTexture						m_TexMeScoreNumber;				//������ͼ
	CD3DTexture						m_TexTimeFlag;					//ʱ���ʶ
	CD3DTexture						m_TexDispatchCardTip;				//������ʾ
	CD3DTexture						m_TexPastHalf;
	CD3DTexture						m_TexBankerBomb;
	CD3DTexture						m_TextureDispatchCard;				//��Դ����

	struct TotoalPress
	{
		CD3DTexture						Tex_tatoalflag;
		CD3DTexture						Tex_tipC;
		CD3DTexture						Tex_tipN;
		
	};


	CD3DTexture						m_TexPngScoreNumber;					//������ͼ
	CD3DTexture						m_TexPngScoreNumber1;					//������ͼ

	TotoalPress                     m_TotoalPress;               

	//�߿���Դ
protected:
	CD3DTexture						m_TexFrameShunMen;				//�߿�ͼƬ
	CD3DTexture						m_TexFrameDuiMen;					//�߿�ͼƬ
	CD3DTexture						m_TexFrameDaoMen;					//�߿�ͼƬ
	CD3DTexture						m_TexFocusShunMen;				//�߿�ͼƬ
	CD3DTexture						m_TexFocusDuiMen;					//�߿�ͼƬ
	CD3DTexture						m_TexFocusDaoMen;					//�߿�ͼƬ
	CD3DTexture						m_TexMeBanker;					//�л�ׯ��
	CD3DTexture						m_TexChangeBanker;				//�л�ׯ��
	CD3DTexture						m_TexNoBanker;					//�л�ׯ��
	CD3DTexture						m_TexTimerNumber;

	CD3DTexture						m_TexTimer;						//�л�ׯ��	
	CD3DTexture						m_TexBankerMax;					//

	//������Դ
protected:
	CD3DTexture						m_TexGameEnd;						//�ɼ�ͼƬ

	//������Դ
public:
	CListCtrl						m_SkinListCtrl[3];

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
	//virtual VOID DrawGameView(CDC * pDC, INT nWidth, INT nHeight);
	//WIN7֧��
	virtual bool RealizeWIN7() { return false; }
	//�رմ���Ȩ��
	virtual bool GetCloseFrame(TCHAR szInfo[]);

	//CGameFrameViewD3D���溯��
private:
	//��������
	virtual VOID CartoonMovie();
	//���ý���
	virtual VOID InitGameView(CD3DDevice * pD3DDevice, INT nWidth, INT nHeight);
	//�滭����
	virtual VOID DrawGameView(CD3DDevice * pD3DDevice, INT nWidth, INT nHeight);

	//���ú���
public:
	//������Ϣ
	void SetMeMaxScore(LONGLONG lMeMaxScore);
	//������Ϣ
	void SetUserLimitScore(LONGLONG lUserLimitScore);
	//�ҵ�λ��
	void SetMeChairID(DWORD dwMeUserID);
	//������ע
	void SetMePlaceJetton(BYTE cbViewIndex, LONGLONG lJettonCount);
	//ׯ����Ϣ
	void SetBankerInfo(DWORD dwBankerUserID, LONGLONG lBankerScore);
	//ׯ�ҳɼ�
	void SetBankerScore(WORD wBankerTime, LONGLONG lWinScore) {m_wBankerTime=wBankerTime; m_lTmpBankerWinScore=lWinScore;}
	//���ֳɼ�
	void SetCurGameScore(LONGLONG lMeCurGameScore, LONGLONG lMeCurGameReturnScore, LONGLONG lBankerCurGameScore, LONGLONG lGameRevenue);
	//���ó���
	void PlaceUserJetton(BYTE cbViewIndex, LONGLONG lScoreCount);
	//��������
	void SetAreaLimitScore(LONGLONG lAreaLimitScore);	
	//�����˿�
	void SetCardInfo(BYTE cbTableCardArray[4][2]);
	//���ó���
	void SetCurrentJetton(LONGLONG lCurrentJetton);
	//��ʷ��¼
	void SetGameHistory(BYTE bWinShunMen, BYTE bWinDaoMen, BYTE bWinDuiMen);
	//�ֻ�ׯ��
	void ShowChangeBanker( bool bChangeBanker );
	//�ɼ�����
	void SetGameScore(LONGLONG lMeCurGameScore, LONGLONG lMeCurGameReturnScore, LONGLONG lBankerCurGameScore);
	//����ϵͳ��ׯ
	void EnableSysBanker(bool bEnableSysBanker) {m_bEnableSysBanker=bEnableSysBanker;}
	//������ʾ
	void SetDispatchCardTip(enDispatchCardTip DispatchCardTip);

	void SetFirstShowCard(BYTE bcCard);

	void ClearBank();
	//
	void SetBankerAllResult(LONGLONG lAllWinScore){m_lBankerWinScore = lAllWinScore;}
	//�л���ͼ
	void TransportBackRes(int nRedBackRes);

	//��������
public:	
	//��ʼ����
	void DispatchCard();
	//��������
	void FinishDispatchCard();
	//���ñ�ը
	bool SetBombEffect(bool bBombEffect, WORD wAreaIndex);

	//��������
public:
	//�����ע
	LONGLONG GetUserMaxJetton();

	//�滭����
protected:
	//��˸�߿�
	void FlashJettonAreaFrame(int nWidth, int nHeight, CD3DDevice * pD3DDevice);

	//���溯��
public:
	//�������
	void CleanUserJetton();
	//����ʤ��
	void SetWinnerSide(BYTE bWinShunMen, BYTE bWinDuiMen, BYTE bWinDaoMen, bool bSet);
	//��������
	//void DrawTextString(CD3DDevice * pD3DDevice, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, int nXPos, int nYPos);
	//�滭����
	void DrawMeJettonNumber(CD3DDevice * pD3DDevice);
	//�滭ׯ��
	void DrawBankerInfo(CD3DDevice * pD3DDevice,int nWidth,int nHeight);
	//�滭���
	void DrawMeInfo(CD3DDevice * pD3DDevice,int nWidth,int nHeight);

	//��������
public:
	//��ǰ����
	inline LONGLONG GetCurrentJetton() { return m_lCurrentJetton; }	
	
	void SetMoveCardTimer();
	//���ճ���ʱ��������
	void SetFinishDiscard();

	//�ڲ�����
private:
	//��ȡ����
	BYTE GetJettonArea(CPoint MousePoint);
	//�滭����
	void DrawNumberString(CD3DDevice * pD3DDevice, LONGLONG lNumber, INT nXPos, INT nYPos, bool bMeScore = false,bool blTimer=false);
	//�滭����(nLeft=0��1��2��,nTop=0��1��2��)
	void DrawNumberStringWithPng(CD3DDevice * pD3DDevice,CD3DTexture* pTexNumBack, LONGLONG lNumber, INT nXPos, INT nYPos, bool bMeScore = false,int nLeft=0,int nTop=0);
	//�滭����(nLeft=0��1��2��,nTop=0��1��2��)
	void DrawNumberStringWithSign(CD3DDevice * pD3DDevice,CD3DTexture* pTexNumBack, LONGLONG lNumber, INT nXPos, INT nYPos,
		bool bSign=true,int nFrame = 11,int nLeft=0,int nTop=0);
	//�滭����
	void DrawNumberStringWithSpace(CD3DDevice * pD3DDevice, LONGLONG lNumber, INT nXPos, INT nYPos);
	//�滭����
	void DrawNumberStringWithSpace(CD3DDevice * pD3DDevice, LONGLONG lNumber, CRect rcRect, DWORD dwClr,INT nFormat=-1);
	//�滭��ʶ
	void DrawWinFlags(CD3DDevice * pD3DDevice);
	//��ʾ���
	void ShowGameResult(CD3DDevice * pD3DDevice, int nWidth, int nHeight);
	//͸���滭
	bool DrawAlphaRect(CDC* pDC, LPRECT lpRect, COLORREF clr, FLOAT fAlpha);
	//�ƶ�Ӯ��
	void DeduceWinner(BYTE &bWinShunMen, BYTE &bWinDuiMen, BYTE &bWinDaoMen);

	void DrawJettonPress(CD3DDevice * pD3DDevice,int iWidth,int iHeight,LONGLONG lTotal,LONGLONG lHaveJetton);

	void DrawGameTimer(CD3DDevice * pD3DDevice,int iWidth,int iHeight,int iTimer);

	LONGLONG GetBankMaxJetton();

public:
	//������ע���
	void ControlBetDetection(BYTE cbViewIndex, LONGLONG lScoreCount, WORD wChair );

public:
	//����Falsh
	void StartShowCard(WORD wFalshID,int nXPos,int nYPos);
	//�ƶ�Flash
	void MoveFlash(WORD wFalshID);
	//�ƶ�Flash
	void MoveFlashEx(WORD wFalshID);
	//����Falsh
	void StartShowCardEx(WORD wFalshID,int nXPos,int nYPos,bool bShow);

	//��ť��Ϣ
protected:
	//��ʼ��ť
	//afx_msg void OnStart();
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

	afx_msg void OnOpenSound();

	afx_msg void OnCloseSound();
	//���д��
	afx_msg void OnBankStorage();
	//����ȡ��
	afx_msg void OnBankDraw();
	//�Ϸ�
	afx_msg void OnUp();
	//�·�
	afx_msg void OnDown();
	//����Ա����
	afx_msg void OpenAdminWnd();
	//��Ӯ����
	afx_msg void OnControl();

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
	//�����ؼ����UP��Ϣ
	LRESULT OnViLBtUp(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnPlayFlashEnd(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnPlayFlashEndEx(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()	
};

//////////////////////////////////////////////////////////////////////////

#endif
