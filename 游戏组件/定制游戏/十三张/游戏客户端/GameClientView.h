#ifndef GAME_CLIENT_VIEW_HEAD_FILE
#define GAME_CLIENT_VIEW_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "CardControl.h"
#include "MulsecCardControlEx.h"
#include "HistoryScore.h"
#include "GameLogic.h"
#include "MessageBox.h"

/////////////////////////////////////////////////////////////////////////////////////////////////
//��Ϣ����
#define IDM_START					(WM_USER+100)						//��ʼ��Ϣ
#define IDM_CALL_BANKER				(WM_USER+101)                       //��ׯ��Ϣ
#define IDM_NO_CALL_BANKER			(WM_USER+102)                       //������Ϣ
#define IDM_SETCHIP					(WM_USER+103)						//ѹע��Ϣ
#define IDM_SHUFFLE_FINISH			(WM_USER+104)						//ϴ�����
#define IDM_SPLIT_CARD				(WM_USER+105)						//������Ϣ
#define IDM_SPLIT_FINISH			(WM_USER+106)						//�������
#define IDM_REVERSAL_FINISH			(WM_USER+107)						//�������
#define IDM_DISPATCH_CARD_FINISH	(WM_USER+108)						//�������
#define IDM_FRONTCARD				(WM_USER+109)						//����ǰ��  
#define IDM_MIDCARD					(WM_USER+110)						//�����ж�
#define IDM_BACKCARD				(WM_USER+111)						//���ú��
#define IDM_SHOWCARD				(WM_USER+112)						//ȷ����Ϣ
#define IDM_SORT_CARD				(WM_USER+113)						//������Ϣ
#define IDM_TRUSTEE			        (WM_USER+114)                       //�й���Ϣ
#define IDM_RESTORE_CARD			(WM_USER+115)                       //�ָ��˿�
#define IDM_PROMPT					(WM_USER+116)						//��ʾ��Ϣ
#define IDM_RESTORE					(WM_USER+117)						//�����˿�
#define IDM_REVERSAL_TURN_FINISH	(WM_USER+118)						//������Ϣ
#define IDM_VOICE_PHRASE			(WM_USER+200)						//������Ϣ

#define JETTON_COUNT				14									//����������
////////////////////////////////////////////////////////////////////////////////////////////////
//����״̬
struct tagJettonStatus
{
	//������Ϣ
	WORD							wMoveCount;							//�ƶ�����
	WORD							wMoveIndex;							//�ƶ�����

	//������Ϣ
	CPoint							ptFrom;								//����λ��
	CPoint							ptDest;								//Ŀ��λ��
	CPoint							ptCourse;							//����λ��
	LONGLONG						lGold;								//������Ŀ

	//�ƶ���ʽ
	INT								iMoveType;							//�ƶ���ʽ
};

//��ǹ����
struct tagGunStatus
{
	WORD							wSrcUser;							//��ǹ���
	WORD							wDestUser;							//Ŀ�����
};

//��������
struct tagSendCardItem
{
	WORD							wChairID;							//�����û�
	BYTE							cbCardData;							//��������
};

//����˵��
typedef CWHArray<tagSendCardItem,tagSendCardItem &> CSendCardItemArray;

//�˿�����
enum enCardControlType
{
	enCCTHandCard,						//�����˿�
	enCCTSegmentCard,					//�û��ֶ�
	enCCTAllSegmentCard					//���зֶ�
};

//��Ϸ��ͼ
class CGameClientView : public CGameFrameViewGDI
{
	//������
protected:
	bool							m_bMoveMouse;						//�ƶ���־
	WORD							m_wMouseDownItem;					//�������
	WORD							m_wHoverCardItem;					//��������
	//�������
protected:
	WORD							m_wBanker;							//ׯ�����
	enSegmentType					m_enSegmentType ;					//�Ƚ�����
	bool							m_bCallBankerStatus[GAME_PLAYER] ;	//���б�ʶ
	bool							m_bGameStatus[GAME_PLAYER];			//���״̬
	bool							m_bTrustee[GAME_PLAYER];			//�йܱ�ʶ
	bool							m_bShowHistory;						//��ʾ�ɼ�
	bool							m_bShowCardFrame;					//��ʾ�ƿ�
	bool							m_bVoicePanelStatus;				//�������
	bool							m_bGlobalClock;						//ȫ��ʱ��
	DWORD							m_dwElapseTime;						//ʱ�����
	TCHAR							m_szSoundPath[MAX_PATH];			//�ļ�·��
	
	bool							m_bShowFuZhuClock[GAME_PLAYER];		//����ʱ��
	DWORD							m_dwFuZhuTime;						//����ʱ��
	
	BYTE							m_cbTipInfoState;					//��ʾ��Ϣ	1:��ׯ 2:��ע

	//������ʾ
	bool							m_bShowSpecType[GAME_PLAYER];		//��ʾ����
	BYTE							m_cbSpecType[GAME_PLAYER];			//��������
	
	SCORE							m_wMaxChip;							//�����ע
	SCORE							m_lUserChip[GAME_PLAYER];			//���Ѻע
	SCORE							m_lChipArray[SET_CHIP_COUNT];		//Ѻע��ť
	SCORE							m_lGameScore[GAME_PLAYER];			//��ҵ÷�
	SCORE							m_lScoreTimes[GAME_PLAYER];			//��������
	
	//������Ϣ
protected:
	tagHistoryScore*				m_pHistoryScore[GAME_PLAYER];		//������Ϣ

	//��ʾ����
	bool							m_bShowTips;						//������ʶ
	
	//���붯��
	tagJettonStatus					m_JettonStatus[GAME_PLAYER];		//������Ϣ
	bool							m_bJettonAction;					//������ʶ
	BYTE							m_JettonData[GAME_PLAYER][JETTON_COUNT];//��������

	//ϴ�ƶ���
protected:
	bool							m_bShuffleCard;						//ϴ�Ʊ�ʶ
	WORD							m_wShuffleIndex;					//��������

	//���ƶ���
protected:
	bool							m_bSplitCard;						//���Ʊ�ʶ
	WORD							m_wSplitIndex;						//��������

	//���ƶ���
protected:
	bool							m_bReversalCard;					//���Ʊ�ʶ
	BYTE							m_cbReversalCount;					//��������
	WORD							m_wReversalIndex;					//��������
	WORD							m_wReversalUser;					//��ǰ���
	CPoint							m_ptReversal;						//����λ��
	BYTE							m_cbCardType;						//��ǰ����
	bool							m_bShowType;						//��ʾ��ʶ
    
	//���ƶ���
protected:
	WORD							m_wStartDispatcher;					//�׵�����
	bool							m_bDispatchCard;					//������־
	WORD							m_wDispatchCardIndex;				//֡������
	BYTE							m_cbHandCardData[HAND_CARD_COUNT];	//��������

	//ȫ�ݴ򶯻�
protected:
	WORD							m_wSwatUser;						//Ӯ��ID
	bool							m_bSwatCartoon;						//������־
	WORD							m_wSwatIndex;						//֡������
	CPoint							m_ptSwat;							//����λ��

	//��ǹ����
protected:
	tagGunStatus					m_GunStatus[GAME_PLAYER];			//��ǹ�ṹ
	WORD							m_wGroupCount;						//��������
	WORD							m_wCurrentIndex;					//��ǰ����
	bool							m_bGunCartoon;						//�������
	bool							m_bBullet;							//
	CPoint							m_ptSrc;							//��ǹλ��
	CPoint							m_ptDest;							//Ŀ��λ��



	//λ�ñ���
protected:
	CPoint							m_ptUserFrame[GAME_PLAYER];			//�û�λ��
	CPoint							m_ptBankerFlag[GAME_PLAYER];		//ׯ��λ��		
	CPoint							m_ptCardControl[GAME_PLAYER] ;		//�˿�λ��
	CPoint							m_ptChip[GAME_PLAYER];				//����λ��
	CPoint							m_ptCallScore[GAME_PLAYER];			//�з�λ��
	CPoint							m_ptTrusteeFlag[GAME_PLAYER];		//�й�λ��
	CPoint							m_ptClockCenter;					//ʱ��λ��
	CPoint							m_ptClockDir[GAME_PLAYER];			//ʱ�ӷ���
	CPoint							m_ptGameScore[GAME_PLAYER];			//��ҵ÷�
	CPoint							m_ptScoreTimes[GAME_PLAYER];		//����λ��
	CPoint							m_ptVoicePanel;						//�������

	//�ؼ�����
public:
	CSkinButton						m_btLookScore;						//�ɼ���ť
	CSkinButton						m_btStart;							//��ʼ��ť
	CSkinButton						m_btShowCard;						//ȷ����ť
	CSkinButton						m_btFrontCard;						//��һע��ť
	CSkinButton						m_btMidCard;						//�ڶ�ע��ť
	CSkinButton						m_btBackCard;						//����ע��ť
	CSkinButton						m_btCallBanker ;					//��ׯ��ť
	CSkinButton						m_btNoCallBanker ;					//���а�ť
	CSkinButton						m_btSetChip[4];						//ѹע��ť
	CSkinButton						m_btPrompt[8];						//��ʾ��ť
	CSkinButton						m_btSplitCard;						//���ư�ť
	CSkinButton						m_btSortCard;						//����ť
	CSkinButton						m_btTrustee;						//�йܰ�ť
	CSkinButton						m_btVoice;							//������ť
	CSkinButton						m_btRestore[4];						//�ջذ�ť

	//λͼ����
protected:
	CBitImage						m_ImageBack;						//������Դ
	CBitImage						m_ImageCenter;						//������Դ
	CPngImage						m_PngShowUserChip;					//����ͼƬ
	CPngImage						m_PngShuffleCard;					//ϴ�ƶ���
	CPngImage						m_PngSplitCard;						//���ƶ���
	CPngImage						m_PngSwatCartoon;					//ȫ�ݴ򶯻�
	CPngImage						m_PngMovingCard;					//��ת����
	CPngImage						m_PngUserHeadBackH;					//�û�����
	CPngImage						m_PngUserHeadBackV;					//�û�����
	CPngImage						m_PngUserInfo;						//�û�����
	CPngImage						m_PngHistoryScore;					//�ɼ�����
	CPngImage						m_PngChipNumer;						//����ͼƬ
	CPngImage						m_PngResultNumber;					//����ͼƬ
	CPngImage						m_PngReadyFlag;						//׼��ͼƬ
	CPngImage						m_PngTrusteeFlag;					//�й�ͼƬ
	CPngImage						m_PngNoCallBanker;					//����ͼƬ
	CPngImage						m_PngBankerFlag;					//ׯ��ͼƬ	
	CPngImage						m_PngReversalCard;					//����ͼƬ
	CPngImage						m_PngCardType;						//������ʾ
	CPngImage						m_PngCardTypeSpec[GAME_PLAYER];		//������ʾ
	CPngImage						m_PngCardTip;						//�๫��ʾ
	CPngImage						m_PngCardFrame;						//�˿˱߿�
	CPngImage						m_PngVoicePanelBack;				//�������	

	CPngImage						m_PngGun;							//ͼƬ��Դ
	CPngImage						m_PngGunFire;						//ͼƬ��Դ
	CPngImage						m_PngGunHole[2];					//ͼƬ��Դ

	CPngImage						m_PngTimeNumber;					//����ͼƬ
	CPngImage						m_PngTimerFlag;						//ʱ��ͼƬ
	CPngImage						m_PngDirectionOp;					//��ͷͼƬ

	CPngImage						m_PngTipCallBanker;					//��ʾ��Ϣ
	CPngImage						m_PngTipCallScore;					//��ʾ��Ϣ

	//�˿˿ؼ�
public:
	CCardControl					m_UserCardControl[GAME_PLAYER];		//�����˿�
	CCardControl					m_HeapCardControl;					//�ƶ��˿�
	CMulSecCardControlEx			m_AllSegCardControl[GAME_PLAYER];	//�����ֶ�	
	CMulSecCardControlEx			m_CenterCardControl;				//�ֶ��˿�

	//�������
public:
	CGameLogic						m_GameLogic;						//�߼����
	CMessageBox						m_MsgBox;							//��Ϣ��
	CDFontEx						m_FontChip;							//Ѻע����
	CDFontEx						m_FontComp;							//�Ƚ�����

	//��������
public:
	//���캯��
	CGameClientView();
	//��������
	virtual ~CGameClientView();

	//���غ���
protected:
	//�����
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);

	//���ܺ���
public:
	//����״̬
	void SetGameStatus(WORD wChairID,bool bStatus) {m_bGameStatus[wChairID] = bStatus;};
	//�����˿�
	void SetCardData(WORD wChairID , BYTE bCardData[] , BYTE bCardCount , enCardControlType bCardType , BYTE bSeg=-1) ;
	//����ׯ��
	void SetBanker(WORD wBanker) ;
	//�����й�
	void SetTrustee(WORD wChairID, bool bTrustee);
	//����ѹע
	void SetChip(WORD wChairID, SCORE lChip, bool bMove=false) ;
	//���÷���
	void SetGameScore(WORD wChairID, SCORE lScore);
	//���ñ���
	void SetScoreTimes(WORD wChairID, SCORE lScoreTimes);
	//��ʾ�Ƚ�
	void SetCompareScene(enSegmentType SegmentType=enSegErr) ;
	//��ʾ�ƿ�
	void ShowCenterControl(bool bShow);
	//������ʾ
	void SetShowCardTip(bool bShow);
	//��ʾ����
	void SetCardType(BYTE cbCardType);
	//��ʾ����
	void SetCardTypeSpec(bool bShow, WORD wChairID, BYTE cbCardType);
	//�����ע
	void SetMaxChip(SCORE wMaxChip) ;
	//��ׯ��Ϣ
	void SetCallBankerStatus(WORD wChairID, bool bCallBankerStatus) ;
	//������ע
	void SetChipArray(SCORE lChipsArray[SET_CHIP_COUNT]) ;
	//��ȡע��
	SCORE GetChipArrayItem(int nIndex) { return m_lChipArray[nIndex]; }
	//���û���
	VOID SetHistoryScore(WORD wChairID, tagHistoryScore * pHistoryScore);	
	//����ļ�
	LPCTSTR GetRandomFile(LPCTSTR pszPath);
	//��������
	VOID PlayCardTypeSound(WORD wChairID, BYTE cbCardType, BYTE cbMaxCard);
	//��������
	VOID PlaySpecCardTypeSound(WORD wChairID, BYTE cbSpecialType);
	//����ʱ��
	void SetGlobalClock(bool isClock);
	//����ʱ��
	void SetElapseTime(DWORD dwElapseTimer);
	//��ȡ��ʾ
	bool GetShowCardTip();
	
	//����ʱ��
	void ShowFuZhuClock(WORD wChairID,bool bShow);
	//���ø���ʱ��
	void SetFuZhuTime(DWORD dwFuZhuTime);
	
	//������ʾ��Ϣ
	void SetTipInfoState(BYTE cbTipInfoState);

	//������ͼ
	VOID RefreshGameView();
	//ϴ�ƶ���
	VOID StartShuffleCard();
	//ֹͣϴ��
	VOID StopShuffleCard();
	//���ƶ���
	VOID StartDispatchCard(BYTE cbCardData[HAND_CARD_COUNT], WORD wStartUser);
	//ȫ�ݴ򶯻�
	VOID StartSwatCartoon(WORD wChairID);
	//ֹͣ����
	VOID StopSwatCartoon();
	//���ö���
	VOID SetGunCartoonGroup(tagGunStatus GunStatus[], WORD wGroupCount);
	//��ǹ����
	VOID StartGunCartoon(WORD wSrcUser, WORD wDestUser);
	//��ǹ����
	VOID StopGunCartoon();
	//ֹͣ����
	VOID StopDispatchCard();
	//���ƶ���
	VOID StartSplitCard();
	//ֹͣ����
	VOID StopSplitCard();
	//���ƶ���
	VOID ReversalCard(WORD wStartUserID, BYTE cbCardCount);
	// �滭����
	VOID DrawNumber(CDC * pDC, CPngImage* ImageNumber, TCHAR * szImageNum, SCORE lOutNum, INT nXPos, INT nYPos, UINT uFormat = DT_LEFT);
	//��������
	VOID DrawSignNumber(CDC * pDC, CPngImage* ImageNumber, TCHAR * szImageNum, SCORE lOutNum, INT nXPos, INT nYPos, UINT uFormat = DT_LEFT);
	//�滭ʱ��
	VOID DrawUserCustomClock(CDC * pDC, WORD wChairID, WORD wElapseTime);
	//����ȫ��ʱ��
	VOID DrawGlobalClock(CDC* pDc);
	//���Ƹ���ʱ��
	VOID DrawFuZhuClock(CDC* pDc);
	//���ƶ���
	VOID DrawMoveAnte(WORD wViewChairID, SCORE lTableScore);
	//�ƶ�����
	bool MoveJetton();
	//��������
	//VOID SetVoicePanelStatus();
	//��������
	VOID AnalyseChip(WORD wChairID, LONGLONG lChip);

	
	//�̳к���
private:
	//���ý���
	virtual VOID ResetGameView();
	//�����ؼ�
	virtual VOID RectifyControl(INT nWidth, INT nHeight);
	//�滭����
	virtual VOID DrawGameView(CDC * pDC, INT nWidth, INT nHeight);
	
	
	//�Ƿ��������
	virtual bool RealizeWIN7() { return true; }

	//��Ϣӳ��
protected:
	//��������
	INT OnCreate(LPCREATESTRUCT lpCreateStruct);
	//��ʱ��
	VOID OnTimer(UINT nIDEvent);
	//���˫��
	VOID OnLButtonDblClk(UINT nFlags, CPoint point);
	//�����Ϣ
	VOID OnLButtonUp(UINT nFlags, CPoint Point);
	//�����Ϣ
	VOID OnLButtonDown(UINT nFlags, CPoint Point);
	//����ƶ�
	VOID OnMouseMove(UINT nFlags, CPoint Point);
	//�����Ϣ
	BOOL OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage);
	

	DECLARE_MESSAGE_MAP()
};

#endif
//////////////////////////////////////////////////////////////////////////
