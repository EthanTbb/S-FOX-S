#ifndef GAME_CLIENT_VIEW_HEAD_FILE
#define GAME_CLIENT_VIEW_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "CardControl.h"
#include "RecordGameList.h"
#include "ApplyUserList.h"
#include "..\��Ϸ������\GameLogic.h"
#include "ClientControl.h"
#include "SkinListCtrl.h"
#include "MessageTipDlg.h"
#include "BankTipsDlg.h"
#include "ChatMessage.h"
#include "StringMessage.h"
#include "EditEx.h"
//////////////////////////////////////////////////////////////////////////

//���붨��
#define JETTON_COUNT				6									//������Ŀ
#define JETTON_RADII				68									//����뾶

//��Ϣ����
#define IDM_PLACE_JETTON			WM_USER+200							//��ס��Ϣ
#define IDM_APPLY_BANKER			WM_USER+201							//������Ϣ		
#define IDM_SOUND					WM_USER+206		
//#define IDM_ADMIN_COMMDN			WM_USER+207
#define IDM_CHAT_MESSAGE			WM_USER+208
#define IDM_GAME_OPTIONS			WM_USER+209		
#define IDM_PLACE_XUYA				WM_USER+210							//��ѹ��Ϣ


//��������
#define INDEX_PLAYER				0									//�м�����
#define INDEX_BANKER				1									//ׯ������


#define MAX_MOVE_ANIMAL_INDEX       20									//�������
#define MAX_DIAL_INDEX				6									//�������
#define MAX_END_ANIMAL_INDEX		6									//�������

#define BUTTON_COUNT                19
#define SELECT_ANIMAL_COUNT			12
//////////////////////////////////////////////////////////////////////////
//�ṹ����

//��ʷ��¼
#define MAX_SCORE_HISTORY			65									//��ʷ����
#define MAX_FALG_COUNT				8									//��ʶ����
#define COUNT_DRAWINFO              20

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


//��ť״̬
enum enButtonState 
{ 					
	enNormal,
	enDown,
	enMouseOver,
	enDisabled

};
//���а�ť
enum enButtonName 
{ 					
	enButtonClose=0,
	enButtonStore,
	enButtonMax,
	enButtonMin,
	enButtonOption,
	enButtonApplyBanker,
	enButtonGiveupBanker,
	enButtonDisListUp,
	enButtonDisListDown,
	enButtonListUp,
	enButtonListDown,
	enButtonLuziUp,
	enButtonLuziDown,
	enButtonBanker,
	enButton1w,
	enButton10w,
	enButton50w,
	enButton100w,
	enButton500w,
	enButton1000w
};

//��ʾ״̬
enum enTipState 
{ 					
	enNull,									//��״̬
	enWaitBanker,							//�ȴ���ׯ
	enStartBet,								//��ʼ��ע
	enEndBet,								//������ע
	enChangeBanker,							//����ׯ��
	enAreaOverFlow,							//����������ע��
	enNoMoneyForBanker,						//��Ҳ��㣬�޷���ׯ
	enBankerNoMoneyForBet,					//ׯ�ҽ�Ҳ��㣬�޷���ע
	enUserNoMoneyForBet,					//��ҽ�Ҳ��㣬�޷���ע
	enOverMaxLimitMoney,					//������������ע��޷���ע
	enOverThreeBetArea,						//����������ע����
	enOverThisJuMoney,						//�������������ע��
	enBetWillEnd							//��עʱ�����Ͻ���

};

//��¼��Ϣ
struct tagClientGameRecord
{
	BYTE							cbAnimal;						   //���ж���

};

struct tagAndroidBet
{
	BYTE							cbJettonArea;						//��������
	LONGLONG						lJettonScore;						//��ע��Ŀ
	WORD							wChairID;							//���λ��
	int								nLeftTime;							//ʣ��ʱ�� (100msΪ��λ)
};

//��������
typedef CWHArray<tagJettonInfo,tagJettonInfo&> CJettonInfoArray;

//������
class CGameClientEngine;

//��Ϸ��ͼ
class CGameClientView : public CGameFrameViewGDI,public IGamePropertyUseSink,public IExpressionSink
{
	//������Ϣ,
protected:
	LONGLONG						m_lMeMaxScore;						//�����ע
	LONGLONG						m_lAreaLimitScore;					//��������
	LONGLONG						m_lRobotMaxJetton;					//������

	//��ע��Ϣ
protected:
	LONGLONG						m_lUserJettonScore[AREA_COUNT+1];	//������ע
	LONGLONG						m_lAllJettonScore[AREA_COUNT+1];	//ȫ����ע
	LONGLONG						m_lUserAddScore[AREA_COUNT];		//��ע��Ϣ       
	LONGLONG						m_lAllPlayBet[AREA_COUNT+1];//������ʵ�����ע
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
	WORD							m_wMeChairID;						//�ҵ�λ��
	LONGLONG						m_lMeCurGameScore;					//�ҵĳɼ�
	LONGLONG						m_lMeCurGameReturnScore;			//�ҵĳɼ�
	LONGLONG						m_lBankerCurGameScore;				//ׯ�ҳɼ�
	LONGLONG						m_lGameRevenue;						//��Ϸ˰��

	//״̬����
public:
	LONGLONG						m_lCurrentJetton;					//��ǰ����
	BYTE							m_cbAreaFlash;						//ѡ������
	bool							m_bFlashResult;						//��ʾ���
	bool							m_bFlashAllArea;					//ȫ����˸
	bool							m_bShowGameResult;					//��ʾ���
	bool							m_bNeedSetGameRecord;				//�������	
	int								m_nMultule;							//����
	//��������
protected:
	bool							m_blRungingCar;						//�Ƿ��ƶ�
	int								m_nTimerStep;						//�ƶ���ֵ
	int								m_nCarOpenSide;						//���ж���
	int								m_nCurRunIndex;						//��ǰ����
	int								m_nTotoalRun;						//�ƶ�Ȧ��
	int								m_CarIndex;							//��ǰindex

	//λ����Ϣ
protected:
	CRect							m_rectBigRect;						//��ľ���
	CRect							m_rectSmallRect;					//С�ľ���
// 	CPoint							m_PointJettonLight[AREA_COUNT];		//����λ��
	CRect						    m_RectJettonScore[AREA_COUNT];		//����λ��
	CPoint							m_BaseAnimalPt[ANIMAL_COUNT];		//λ����Ϣ	
	CPoint                          m_LuZiStart;						//·����Ϣ

	//�˿���Ϣ
public:	
	BYTE							m_cbTableCard;						//�����˿�
	tagClientGameRecord				m_GameRecordArrary[MAX_SCORE_HISTORY];	//·��
	int								m_nRecordFirst;						//��ʼ��¼
	int								m_nRecordLast;						//����¼
	int								m_nScoreHead;						//�ɼ�λ��
	LONGLONG						m_lMeStatisticScore;				//��Ϸ�ɼ�

	//�������
protected:
	CPngImage						m_ViewBackPng;						//����ͼƬ
	CPngImage						m_BackMutiple;						//����ͼƬ
	CPngImage						m_ChangeBanker;						//�л�ׯ��
	CPngImage						m_CurrentBankerTime;				//��ǰׯ��
	CPngImage						m_UserInfo;							//�û���Ϣ	
	CPngImage						m_BetFlagMe;						//��ע��־
	CPngImage						m_BetFlagTotal;						//��ע��־
	CPngImage						m_EndCarInfo;						//��������
	CPngImage						m_EndMaskBack;						//��������
	CPngImage						m_HistroyCar;						//��ʷ��¼
	CPngImage						m_RecordLast;						//��ʷ��¼
	CPngImage						m_SelPng;							//ת������
	CPngImage						m_TimeFree;							//ʱ���־
	CPngImage						m_TimeBet;							//ʱ���־
	CPngImage						m_TimeDrawe;						//ʱ���־

	CPngImage						m_EndImage;							//����ͼƬ
	CPngImage						m_BetBack;							//��ע����
	CPngImage						m_AearMaskLeft;						//ѡ��������
	CPngImage						m_AearMaskRight;					//ѡ��������

	CPngImage						m_MeBetNum;							//��ע����
	CPngImage						m_ScoreNum;							//��ע����
	CPngImage						m_EndWinScore;						//��������
	CPngImage						m_EndLostScore;						//��������
	CPngImage						m_TimeNum;							//ʱ������
		
	//��ťͼƬ
	CPngImage						m_pngCloseButton;					//�رհ�ť0
	CPngImage						m_pngMinButton;						//��С��1
	CPngImage						m_pngGameOption;					//��Ϸ����2

	CPngImage						m_pngBtJetton_100;					//������Ϣ3
	CPngImage						m_pngBtJetton_1000;					//������Ϣ4
	CPngImage						m_pngBtJetton_1W;					//������Ϣ5
	CPngImage						m_pngBtJetton_10W;					//������Ϣ6
	CPngImage						m_pngBtJetton_100W;					//������Ϣ7
	CPngImage						m_pngBtJetton_500W;					//������Ϣ8
	CPngImage						m_pngBtJetton_1000W;				//������Ϣ9
	CPngImage						m_pngBtJetton_XUYA;					//������Ϣ10
	//CPngImage						m_pngBtJetton_5W;					//������Ϣ6
	//CPngImage						m_pngBtJetton_5000W;				//������Ϣ8
	//CPngImage						m_pngBtJetton_1Y;					//������Ϣ9

	CPngImage						m_pngBtBank;						//���а�ť11
	CPngImage						m_pngBtUpList;						//�б�ť12
	CPngImage						m_pngApplyBanker;					//����ׯ��13
	CPngImage						m_pngCancerBanker;					//ȡ������14
	CPngImage						m_pngBtDownList;					//�б�ť15

	CPngImage						m_pngBtSwitchChat;					//�л���ť16
	CPngImage						m_pngBtSwitchUserList;				//�л���ť17

	CPngImage						m_pngBtSendChat;					//������Ϣ18
// 	CPngImage						m_pngBtExpress;						//���鰴ť19
// 	CPngImage						m_pngBtColor;						//��ɫ��ť20
// 	CPngImage						m_pngBtTrumpet;						//���Ȱ�ť21
	
	CPngImage						m_pngCarMaserati;					//��ɯ����
	CPngImage						m_pngCarBMW;						//����
	CPngImage						m_pngCarPorsche;					//��ʱ��
	CPngImage						m_pngCarFerrari;					//������
	CPngImage						m_pngCarJaguar;						//�ݱ�
	CPngImage						m_pngCarLamborghini;				//��������
	CPngImage						m_pngCarLandrover;					//·��
	CPngImage						m_pngCarBenz;						//����
	//��������
	CPngImage						m_pngBetMaserati;					//��ɯ����
	CPngImage						m_pngBetBMW;						//����
	CPngImage						m_pngBetPorsche;					//��ʱ��
	CPngImage						m_pngBetFerrari;					//������
	CPngImage						m_pngBetJaguar;						//�ݱ�
	CPngImage						m_pngBetLamborghini;				//��������
	CPngImage						m_pngBetLandrover;					//·��
	CPngImage						m_pngBetBenz;						//����	
	//������Դ
	CPngImage						m_ListBack;							//�����б�

	//״̬����
public:
	INT								m_nSwitchIndex;						//�л���ť
	INT								m_nShowValleyIndex;					//��ʾ��ׯ�б�
	CWHArray<WORD>					m_ValleysList;						//��ׯ�б�
	COLORREF						m_texColor;							//�ı���ɫ
	//��ť���
	bool							m_bTrackmouse[BUTTON_COUNT];		//��ť״̬
	enButtonState					m_btState[BUTTON_COUNT];			//��ť״̬
	bool							m_bBtShow[BUTTON_COUNT];			//�Ƿ���ʾ
	CRect                           m_RectBt[BUTTON_COUNT];				//��ťλ��
	CRect							m_RectCurSelect;					//��ǰѡ��
	BYTE                            m_cbSelectIndex;					//ѡ��״̬
	CString							m_strTotalRoomName;					//����ȫ��

	//�ؼ�����
public:
	CButton							m_btOpenAdmin;						//ϵͳ����
	HINSTANCE						m_hInst;
	IClientControlDlg			    *m_pAdminControl;					//���ƿؼ�
	CEditEx							m_ChatInput;						//����ؼ�
	CChatMessage					m_ChatMessage;						//���촰��
	CSkinListCtrlEx                 m_UserList;							//�б�ؼ�
	CGameClientEngine				*m_pGameClientDlg;					//����ָ��
	CGameLogic						m_GameLogic;						//��Ϸ�߼�
	CDlgTrumpetItem *               m_pTrumpetItem;                     //���ȴ��� 
	CExpressionControl *			m_pExpressionControl;				//���鴰��

	//��������
public:
	//���캯��
	CGameClientView();
	//��������
	virtual ~CGameClientView();

	//�̳к���
	//���ý���
	virtual VOID ResetGameView();
private:
	
	//�����ؼ�
	virtual VOID RectifyControl(INT nWidth, INT nHeight);
	//�滭����
	virtual VOID DrawGameView(CDC * pDC, INT nWidth, INT nHeight);

	//WIN7֧��
	virtual bool RealizeWIN7() { return true; }
	//�ͷŶ���
	virtual VOID Release() { }
	//�ӿڲ�ѯ
	virtual VOID * QueryInterface(REFGUID Guid, DWORD dwQueryVer);
	//ʹ�õ���
	virtual bool OnEventUseProperty(WORD wPropertyIndex);	
	//�����¼�
	virtual VOID OnExpressionSelect(CExpression * pUserExpression, tagExpressionInfo * pExpressionInfo);

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
	//���ֳɼ�
	void SetCurGameScore(LONGLONG lMeCurGameScore, LONGLONG lMeCurGameReturnScore, LONGLONG lBankerCurGameScore, LONGLONG lGameRevenue);
	//���ó���
	void PlaceUserJetton(BYTE cbViewIndex, LONGLONG lScoreCount);
	//��������
	void SetAreaLimitScore(LONGLONG lAreaLimitScore);	
	//�����˿�
	void SetCardInfo(BYTE cbTableCard);
	//���ó���
	void SetCurrentJetton(LONGLONG lCurrentJetton);
	//��ʷ��¼
	void SetGameHistory(BYTE bcResulte);
	//�ֻ�ׯ��
	void ShowChangeBanker( bool bChangeBanker );
	//�ɼ�����
	void SetGameScore(LONGLONG lMeCurGameScore, LONGLONG lMeCurGameReturnScore, LONGLONG lBankerCurGameScore);
	//����ϵͳ��ׯ
	void EnableSysBanker(bool bEnableSysBanker) {m_bEnableSysBanker=bEnableSysBanker;}
	//���ñ���
	void SetMultule(int nMul){m_nMultule = nMul;}
	//ִ��ʣ�����еĻ��嶯��
	void PerformAllBetAnimation();
	//���û�����������
	//void SetRobotMaxJetton(LONGLONG lRobotMaxJetton){ m_lRobotMaxJetton = lRobotMaxJetton; };
	//������ͼ
	void RefreshGameView();
	//ˢ�°�ť
	void ReflashButton(BYTE cbButtonIndex,bool bAll=false);
	//�����ƶ�
	void StartRunCar(int m_nTimerStep);
	//�����ƶ�
	void RuningCar(int m_nTimerStep);
	//������Ϣ
	void InsertAllChatMessage(ChatMsg *pMsg,int nMessageType) ;
	//����״̬��ʾ
	void SetStateTips(enTipState newState,bool bTimer=true);
	//����ȫ��
	void SetTotalRoomName(CString strName);
	//ȫ����˸
	void FlashAllArea();
	//������ע���
	void ControlBetDetection(BYTE cbViewIndex, LONGLONG lScoreCount, WORD wChair );
			
	//��������
public:	
	//��ʼ����
	void DispatchCard();
	//��������
	void FinishDispatchCard( bool bRecord = true );
	
	//��������
public:
	//�����ע
	LONGLONG GetUserMaxJetton(BYTE cbJettonArea = 0);
	//������
	void  GetAllWinArea(BYTE bcWinArea[],BYTE bcAreaCount,BYTE InArea);
	//���ͳ���
	void ReduceJettonNumber();
	//�滭����
protected:
	//��˸����
	void FlashLightAnimal(CDC *pDC);
	//ȫ����˸
	void FlashAllAnimal(CDC *pDC);
	//�������
	void JettonAreaLight(CDC * pDC);

	//���溯��
public:
	//�������
	void CleanUserJetton();
	//����ʤ��
	void SetWinnerSide(bool blWin[],bool bSet);
	//��������
	void DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, int nXPos, int nYPos);
	//��������
	void DrawBaseScene(CDC *pDC,int nWidth, int nHeight);
	//�滭ׯ��
	void DrawBankerInfo(CDC *pDC,int nWidth,int nHeight);
	//�滭���
	void DrawMeInfo(CDC *pDC,int nWidth,int nHeight);
	//�滭��ť
	void DrawPngButton(CDC *pDC,int buttonIndex);
	//ȡ������
	void ClearAreaFlash() { m_cbAreaFlash = 0xFF; }
	//���°�ť
	VOID UpdateButtonContron();

	//��������
public:
	//��ǰ����
	inline LONGLONG GetCurrentJetton() { return m_lCurrentJetton; }	
	//ȡ����ʱ
	void KillCardTime();
	
	//�ڲ�����
private:
	//��ȡ����
	BYTE GetJettonArea(CPoint MousePoint);
	//�滭����
	int DrawNumberString(CDC * pDC, LONGLONG lNumber,  CRect rcRect,bool bMeScore=false);
	//�滭ʱ��
	void DrawTimeString(CDC * pDC, LONGLONG lNumber, INT nXPos, INT nYPos);
	//�滭����
	void DrawNumStrWithSpace(CDC * pDC, LONGLONG lNumber,CRect&rcPrint,INT nFormat=-1);
	//�滭����
	void DrawEndNum(CDC * pDC, LONGLONG lNumber, INT nXPos, INT nYPos,bool bWin=true);
	//�滭��ʶ
	void DrawWinFlags(CDC * pDC);
	//ת����ʽ
	CString NumberStringWithSpace(CString strNumber);
	//��ȡ����
	BYTE GetAnimalIndex(BYTE cbAllIndex);
	//������Ϣ
	void InsertGameEndInfo();
	//����״̬
	void ResetButtonState();
	//��ť״̬
	BYTE GetButtonState(int buttonIndex);
	//�����Ϣ
	 bool PngButtonOnLButtonDown(UINT nFlags, CPoint Point);	
	//�����Ϣ
	 void PngButtonOnLButtonUp(UINT nFlags, CPoint Point);	
	//����ƶ�
	 void PngButtonOnMouseMove(UINT nFlags, CPoint point);
	 //���ܰ�ť
	 void ButtonAction( int buttonIndex);
	 
	
	 //��ť��Ϣ
protected: 
	 //�رհ�ť	
	 void OnButtonClose();
	 //���ť		
	 void OnButtonMax();
	 //�ָ���ť	
	 void OnButtonStandard();
	 //��С��ť	
	 void OnButtonMin();
	 //���ð�ť	
	 void OnButtonGameOption();
	 //���밴ť	
	 void OnButtonAskForBanker();
	 //ȡ����ť	
	 void OnButtonGiveupBanker();
	 //�л��û��б�	
	 void OnButtonSwitchUserList();
	 //�л�����	
	 void OnButtonSwitchChat();
	 //�Ϸ���ť	
	 void OnButtonListUp();
	 //�·���ť	
	 void OnButtonListDown();
	 //���а�ť	
	 void OnButtonGoBanker();
	 //��ע��ť	
	 void OnButton100();
	 //��ע��ť	
	 void OnButton1000();
	 //��ע��ť	
	 void OnButton1w();
	 //��ע��ť	
	 void OnButton10w();
	 //��ע��ť	
	 void OnButton100w();
	 //��ע��ť	
	 void OnButton500w();
	 //��ע��ť	
	 void OnButton1000W();
	 //��ע��ť	
	 void OnButtonXUYA();
	//��ׯ��ť
	afx_msg void OnApplyBanker();
	//��ׯ��ť
	afx_msg void OnCancelBanker();
	//�ƶ���ť
	afx_msg void OnScoreMoveL();
	//�ƶ���ť
	afx_msg void OnScoreMoveR();
	//������Ϣ
	LRESULT OnSendMessage(WPARAM wParam,LPARAM lParam) ;
	//��ɫѡ��
	void SelectColor();
	//���Ȱ�ť
	void OnProperty();
	//ѡ��ָ����ɫ
	void OnSelectChatColor(UINT nCommandID);
	//���鰴ť
	void OnExpression();
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
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	
	//����Ա����
	afx_msg void OpenAdminWnd();
	DECLARE_MESSAGE_MAP()	
public:
	afx_msg void OnMoreColor();
};

//////////////////////////////////////////////////////////////////////////

#endif
