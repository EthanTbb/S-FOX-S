#include "StdAfx.h"
#include "Math.h"
#include "Resource.h"
#include "GameClientView.h"
#include "GameClientEngine.h"
#include ".\gameclientview.h"

//////////////////////////////////////////////////////////////////////////

//��ť��ʶ
#define IDC_JETTON_BUTTON_10000000  199
#define IDC_JETTON_BUTTON_100		200									//��ť��ʶ
#define IDC_JETTON_BUTTON_1000		201									//��ť��ʶ
#define IDC_JETTON_BUTTON_10000		202									//��ť��ʶ
#define IDC_JETTON_BUTTON_100000	203									//��ť��ʶ
#define IDC_JETTON_BUTTON_1000000	204									//��ť��ʶ
#define IDC_JETTON_BUTTON_5000000	205									//��ť��ʶ
#define IDC_APPY_BANKER				206									//��ť��ʶ
#define IDC_CANCEL_BANKER			207									//��ť��ʶ
#define IDC_SCORE_MOVE_L			209									//��ť��ʶ
#define IDC_SCORE_MOVE_R			210									//��ť��ʶ
#define IDC_VIEW_CHART				211									//��ť��ʶ
#define IDC_JETTON_BUTTON_500000	212									//��ť��ʶ
#define IDC_AUTO_OPEN_CARD			213									//��ť��ʶ
#define IDC_OPEN_CARD				214									//��ť��ʶ
#define IDC_BANK					215									//��ť��ʶ
#define IDC_CONTINUE_CARD			216									//��ť��ʶ
#define IDC_BANK_STORAGE			217									//��ť��ʶ
#define IDC_BANK_DRAW				218									//��ť��ʶ
#define IDC_CONTROL_BUTTON			230									//��ť��ʶ
#define  IDC_SOUND_CLOSE			231
#define  IDC_SOUND_OPEN				232

#define IDC_UP						219									//��ť��ʶ
#define IDC_DOWN					220								    //��ť��ʶ
#define IDC_BT_ADMIN				230                                 //��ť��ʶ
#define IDC_BT_START                231                                 //��ť��ʶ
//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameClientView, CGameFrameViewD3D)//CGameFrameViewGDI)
	ON_WM_TIMER()
	ON_WM_CREATE()
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_WM_MOUSEMOVE()

	ON_BN_CLICKED(IDC_BT_ADMIN,OpenAdminWnd)
	//ON_BN_CLICKED(IDC_BT_START,OnStart)
	ON_BN_CLICKED(IDC_APPY_BANKER, OnApplyBanker)
	ON_BN_CLICKED(IDC_CANCEL_BANKER, OnCancelBanker)
	ON_BN_CLICKED(IDC_SCORE_MOVE_L, OnScoreMoveL)
	ON_BN_CLICKED(IDC_SCORE_MOVE_R, OnScoreMoveR)
	ON_BN_CLICKED(IDC_OPEN_CARD, OnOpenCard)
	ON_BN_CLICKED(IDC_AUTO_OPEN_CARD,OnAutoOpenCard)
	ON_BN_CLICKED(IDC_SOUND_CLOSE, OnOpenSound)
	ON_BN_CLICKED(IDC_SOUND_OPEN ,OnCloseSound)

	ON_MESSAGE(WM_VIEWLBTUP,OnViLBtUp)
	ON_BN_CLICKED(IDC_CONTROL_BUTTON, OnControl)

	ON_BN_CLICKED(IDC_BANK_STORAGE, OnBankStorage)
	ON_BN_CLICKED(IDC_BANK_DRAW, OnBankDraw)

    ON_BN_CLICKED(IDC_UP, OnUp)
	ON_BN_CLICKED(IDC_DOWN, OnDown)

	ON_MESSAGE(IDM_FALSH_FINISH, OnPlayFlashEndEx)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameClientView::CGameClientView()
{
	m_pD3DDevice = NULL;
	//��ע��Ϣ
	ZeroMemory(m_lUserJettonScore,sizeof(m_lUserJettonScore));

	//ȫ����ע
	ZeroMemory(m_lAllJettonScore,sizeof(m_lAllJettonScore));

	//���������ע
	ZeroMemory(m_lAllPlayBet,sizeof(m_lAllPlayBet));

	//ׯ����Ϣ
	m_wBankerUser=INVALID_CHAIR;		
	m_wBankerTime=0;
	m_lBankerScore=0L;	
	m_lBankerWinScore=0L;
	m_lTmpBankerWinScore=0;

	//���ֳɼ�
	m_lMeCurGameScore=0L;	
	m_lMeCurGameReturnScore=0L;
	m_lBankerCurGameScore=0L;
	m_lGameRevenue=0L;

	//״̬��Ϣ
	m_lCurrentJetton=0L;
	m_cbAreaFlash=0xFF;
	m_wMeChairID=INVALID_CHAIR;
	m_bShowChangeBanker=false;
	m_bNeedSetGameRecord=false;
	m_bWinShunMen=false;
	m_bWinDuiMen=false;
	m_bWinDaoMen=false;	
	m_blMoveFinish = false;
	m_bShowHalf = false;
	m_blAutoOpenCard = true;
	m_blCanStore=FALSE;
	m_enDispatchCardTip=enDispatchCardTip_NULL;
	m_bBankerMax=false;
	m_bToBankerMax = false;
	//����
	ZeroMemory(m_stuCartoonInfo,sizeof(m_stuCartoonInfo));
	for (int i = 0; i < CTN_COUNT; ++i)
	{
		m_stuCartoonInfo[i].nShowTimer = conCartoonTimer[i];
	}

	m_lMeCurGameScore=0L;			
	m_lMeCurGameReturnScore=0L;	
	m_lBankerCurGameScore=0L;		

	m_lAreaLimitScore=0L;	

	//λ����Ϣ
	m_nScoreHead = 0;
	m_nRecordFirst= 0;	
	m_nRecordLast= 0;	

	//
	m_nApplyFocusIndex = 0;
	m_bApplyLBDown = false;						//�Ƿ�����ڻ����ϰ���
	m_bSelectStar = false;
	memset(m_cbStarSelect,-1,sizeof(m_cbStarSelect));
	for (int i = 0; i < STAR_COUNT; ++i)
	{
		m_fStarWinRate[i] = 0.0;
		m_llStarAllCount[i] = 0;
		m_llStarWinCount[i] = 0;
	}
	m_bFlashShow = false;
	m_wFlashId = 0;
	m_llAllStarCount = 0;
	
	//��ʷ�ɼ�
	m_lMeStatisticScore=0;

	//�ؼ�����
	m_pGameClientEngine=CONTAINING_RECORD(this,CGameClientEngine,m_GameClientView);

	m_pClientControlDlg = NULL;
	m_hInst = NULL;

	//��������
	for (BYTE i=0;i<CountArray(m_JettonInfoArray);i++)
	{
		m_JettonInfoArray[i].RemoveAll();
	}

	/*m_cbTableCardArray[BANKER_INDEX][0]=0x0B;
	m_cbTableCardArray[BANKER_INDEX][1]=0x14;
	m_cbTableCardArray[SHUN_MEN_INDEX][0]=0x0B;
	m_cbTableCardArray[SHUN_MEN_INDEX][1]=0x2B;
 	bool bWinShunMen=m_GameLogic.CompareCard(m_cbTableCardArray[BANKER_INDEX],2,m_cbTableCardArray[SHUN_MEN_INDEX],2)==1?true:false;*/

	m_lUserLimitScore = 0L;
	return;
}

//��������
CGameClientView::~CGameClientView(void)
{
	for (int i = 0; i < 2; ++i)
	{
		m_ShowLotteryFlashArray[i].StopFlash();
		m_ShowLotteryFlashArray[i].OnCloseFalsh();
	}
	
	if( m_pClientControlDlg )
	{
		delete m_pClientControlDlg;
		m_pClientControlDlg = NULL;
	}

	if( m_hInst )
	{
		FreeLibrary(m_hInst);
		m_hInst = NULL;
	}
}
void CGameClientView::SetMoveCardTimer()
{
	//����
	m_stuCartoonInfo[CTN_POST_CARD].bShow = false;						//�Ƿ񶯻�
	m_stuCartoonInfo[CTN_POST_CARD].nShowFrame = 0;					//��������
	m_OpenCardIndex = 1;
	m_bcShowCount = 0;
	m_PostStartIndex =  m_GameLogic.GetCardValue(m_bcfirstShowCard);
	if(m_PostStartIndex>1)
	m_PostStartIndex  = (m_PostStartIndex-1)%4;
	m_PostCardIndex=m_PostStartIndex;
	for (int i = 0;i<4;i++)
	{
		m_CardControl[i].m_blAdd = true;
		m_CardControl[i].m_blAuto = m_blAutoOpenCard;
		m_CardControl[i].m_blGameEnd = true;	
	}
	//����	
	m_stuCartoonInfo[CTN_POST_CARD].nShowFrame = 0;					//��������
	m_stuCartoonInfo[CTN_POST_CARD].dwShowTime = GetTickCount();					//���嶯��ʱ��
	m_stuCartoonInfo[CTN_POST_CARD].bShow = true;						//�Ƿ񶯻�
}

//���ճ���ʱ��������
void CGameClientView::SetFinishDiscard()
{
	//�ȿ�����
	while (1)
	{
		bool blCopy = false;
		for (int i = 0; i < 4; i++)
		{
			if(m_CardControl[i].m_CardItemArray.GetCount()!=m_CardControl[i].m_CardTempItemArray.GetCount())
			{
				blCopy = true;
				break;
			}
		}
		if(false == blCopy)
		{
			break;
		}
		else
		{
			m_CardControl[i].OnCopyCard();
		}
	}
	//
	m_stuCartoonInfo[CTN_DISPATCH_CARD].bShow = false;
	m_stuCartoonInfo[CTN_DISPATCH_CARD].nShowFrame = 0;
}

 void CGameClientView::OnOpenSound()
{
	SendEngineMessage(IDM_OPEN_SOUND,0,0);

}
 void CGameClientView::OnCloseSound()
{
	SendEngineMessage(IDM_CLOSE_SOUND,0,0);

}

//���ý���
VOID CGameClientView::InitGameView(CD3DDevice * pD3DDevice, INT nWidth, INT nHeight)
{
	m_pD3DDevice = pD3DDevice;
	//��������
	CRect rcCreate(0,0,0,0);
	HINSTANCE hResInstance=AfxGetInstanceHandle();
	m_TexViewFill.LoadImage(pD3DDevice,hResInstance,TEXT("PNG_VIEW_FILL"),TEXT("PNG"));
	m_TexViewBack.LoadImage(pD3DDevice,hResInstance,TEXT("PNG_VIEW_BACK"),TEXT("PNG"));	
	m_TexWinFlags.LoadImage(pD3DDevice,hResInstance,TEXT("PNG_WIN_FLAGS"),TEXT("PNG"));
	m_TexJettonView.LoadImage(pD3DDevice,hResInstance,TEXT("PNG_JETTOM_VIEW"),TEXT("PNG"));
	m_TexScoreNumber.LoadImage(pD3DDevice,hResInstance,TEXT("PNG_SCORE_NUMBER"),TEXT("PNG"));
	m_TexMeScoreNumber.LoadImage(pD3DDevice,hResInstance,TEXT("PNG_ME_SCORE_NUMBER"),TEXT("PNG"));
	m_TexFrameShunMen.LoadImage(pD3DDevice,hResInstance,TEXT("PNG_FRAME_SHUN_MEN"),TEXT("PNG"));
	m_TexFrameDuiMen.LoadImage(pD3DDevice,hResInstance,TEXT("PNG_FRAME_DUI_MEN"),TEXT("PNG"));
	m_TexFrameDaoMen.LoadImage(pD3DDevice,hResInstance,TEXT("PNG_FRAME_DAO_MEN"),TEXT("PNG"));
	m_TexFocusShunMen.LoadImage(pD3DDevice,hResInstance,TEXT("PNG_FOCUS_SHUN_MEN"),TEXT("PNG"));
	m_TexFocusDuiMen.LoadImage(pD3DDevice,hResInstance,TEXT("PNG_FOCUS_DUI_MEN"),TEXT("PNG"));
	m_TexFocusDaoMen.LoadImage(pD3DDevice,hResInstance,TEXT("PNG_FOCUS_DAO_MEN"),TEXT("PNG"));

	m_TexGameEnd.LoadImage(pD3DDevice,hResInstance,TEXT("PNG_GAME_END"),TEXT("PNG"));
	m_TexMeBanker.LoadImage(pD3DDevice,hResInstance,TEXT("PNG_ME_BANKER"),TEXT("PNG"));
	m_TexTimerNumber.LoadImage(pD3DDevice,hResInstance,TEXT("PNG_TIMERNUMBER"),TEXT("PNG"));
	m_TexChangeBanker.LoadImage(pD3DDevice,hResInstance,TEXT("PNG_CHANGE_BANKER"),TEXT("PNG"));
	m_TexNoBanker.LoadImage(pD3DDevice,hResInstance,TEXT("PNG_NO_BANKER"),TEXT("PNG"));
	m_TexTimeFlag.LoadImage(pD3DDevice,hResInstance,TEXT("PNG_TIME_FLAG"),TEXT("PNG"));
	m_TexPastHalf.LoadImage(pD3DDevice,hResInstance,TEXT("PNG_PAST_HALF"),TEXT("PNG"));
	m_TexBankerBomb.LoadImage(pD3DDevice,hResInstance,TEXT("PNG_BANKER_BOMB"),TEXT("PNG"));
	m_TotoalPress.Tex_tatoalflag.LoadImage(pD3DDevice,hResInstance,TEXT("IDB_IDB_TIPP_PNG"),TEXT("PNG"));
	m_TotoalPress.Tex_tipC.LoadImage(pD3DDevice,hResInstance,TEXT("IDB_IDB_TIPC_PNG"),TEXT("PNG"));
	m_TotoalPress.Tex_tipN.LoadImage(pD3DDevice,hResInstance,TEXT("IDB_IDB_TIPN_PNG"),TEXT("PNG"));
	m_TexPngScoreNumber.LoadImage(pD3DDevice,hResInstance,TEXT("IDB_IDB_TIPN_PNG"),TEXT("PNG"));
	m_TexPngScoreNumber1.LoadImage(pD3DDevice,hResInstance,TEXT("IDB_IDB_TIPN_PNG1"),TEXT("PNG"));
	m_TexTimer.LoadImage(pD3DDevice,hResInstance,TEXT("IDB_IDB_CLOCK_PNG"),TEXT("PNG"));
	m_TexBankerMax.LoadImage(pD3DDevice,hResInstance,TEXT("BANKER_MAX"),TEXT("PNG"));
	m_TexApplyBlock.LoadImage(pD3DDevice,hResInstance,TEXT("PNG_APPLY_BLOCK"),TEXT("PNG"));	
	m_TexApplyBlockBack.LoadImage(pD3DDevice,hResInstance,TEXT("PNG_APPLY_BLOCK_BACK"),TEXT("PNG"));	
	m_TexScoreBack.LoadImage(pD3DDevice,hResInstance,TEXT("PNG_SCORE_BACK"),TEXT("PNG"));
	m_TexGod.LoadImage(pD3DDevice,hResInstance,TEXT("PNG_GOD"),TEXT("PNG"));
	m_TexStar.LoadImage(pD3DDevice,hResInstance,TEXT("PNG_STAR"),TEXT("PNG"));
	m_TexStarShow[0].LoadImage(pD3DDevice,hResInstance,TEXT("PNG_STAR_SHUN"),TEXT("PNG"));
	m_TexStarShow[1].LoadImage(pD3DDevice,hResInstance,TEXT("PNG_STAR_DUI"),TEXT("PNG"));
	m_TexStarShow[2].LoadImage(pD3DDevice,hResInstance,TEXT("PNG_STAR_DAO"),TEXT("PNG"));
	m_TexProgress.LoadImage(pD3DDevice,hResInstance,TEXT("PNG_PROGRESS"),TEXT("PNG"));
	m_TexProgressBack.LoadImage(pD3DDevice,hResInstance,TEXT("PNG_PROGRESS_BACK"),TEXT("PNG"));
	m_TexProgressNum.LoadImage(pD3DDevice,hResInstance,TEXT("PNG_PROGRESS_NUM"),TEXT("PNG"));
	//
	m_btTexJetton1000.LoadImage(pD3DDevice,hResInstance,TEXT("PNG_BT_JETTON_1000"),TEXT("PNG"));	
	m_btTexJetton10000.LoadImage(pD3DDevice,hResInstance,TEXT("PNG_BT_JETTON_10000"),TEXT("PNG"));		
	m_btTexJetton100000.LoadImage(pD3DDevice,hResInstance,TEXT("PNG_BT_JETTON_100000"),TEXT("PNG"));			
	m_btTexJetton500000.LoadImage(pD3DDevice,hResInstance,TEXT("PNG_BT_JETTON_500000"),TEXT("PNG"));	
	m_btTexJetton1000000.LoadImage(pD3DDevice,hResInstance,TEXT("PNG_BT_JETTON_1000000"),TEXT("PNG"));			
	m_btTexJetton5000000.LoadImage(pD3DDevice,hResInstance,TEXT("PNG_BT_JETTON_5000000"),TEXT("PNG"));			
	m_btTexJetton10000000.LoadImage(pD3DDevice,hResInstance,TEXT("PNG_BT_JETTON_10000000"),TEXT("PNG"));	
	//��Ŧ
	m_btJetton100.ActiveWindow(rcCreate,WS_CHILD|WS_VISIBLE|WS_DISABLED,IDC_JETTON_BUTTON_100,&m_VirtualEngine,NULL);
	m_btJetton1000.ActiveWindow(rcCreate,WS_CHILD|WS_VISIBLE|WS_DISABLED,IDC_JETTON_BUTTON_1000,&m_VirtualEngine,NULL);
	m_btJetton10000.ActiveWindow(rcCreate,WS_CHILD|WS_VISIBLE|WS_DISABLED,IDC_JETTON_BUTTON_10000,&m_VirtualEngine,NULL);
	m_btJetton100000.ActiveWindow(rcCreate,WS_CHILD|WS_VISIBLE|WS_DISABLED,IDC_JETTON_BUTTON_100000,&m_VirtualEngine,NULL);
	m_btJetton500000.ActiveWindow(rcCreate,WS_CHILD|WS_VISIBLE|WS_DISABLED,IDC_JETTON_BUTTON_500000,&m_VirtualEngine,NULL);
	m_btJetton1000000.ActiveWindow(rcCreate,WS_CHILD|WS_VISIBLE|WS_DISABLED,IDC_JETTON_BUTTON_1000000,&m_VirtualEngine,NULL);
	m_btJetton5000000.ActiveWindow(rcCreate,WS_CHILD|WS_VISIBLE|WS_DISABLED,IDC_JETTON_BUTTON_5000000,&m_VirtualEngine,NULL);
	m_btJetton10000000.ActiveWindow(rcCreate,WS_CHILD|WS_VISIBLE|WS_DISABLED,IDC_JETTON_BUTTON_10000000,&m_VirtualEngine,NULL);
	m_btApplyBanker.ActiveWindow(rcCreate,WS_CHILD|WS_VISIBLE|WS_DISABLED,IDC_APPY_BANKER,&m_VirtualEngine,NULL);
	m_btCancelBanker.ActiveWindow(rcCreate,WS_CHILD|WS_DISABLED,IDC_CANCEL_BANKER,&m_VirtualEngine,NULL);
	m_btScoreMoveL.ActiveWindow(rcCreate,WS_CHILD|WS_VISIBLE|WS_DISABLED,IDC_SCORE_MOVE_L,&m_VirtualEngine,NULL);
	m_btScoreMoveR.ActiveWindow(rcCreate,WS_CHILD|WS_VISIBLE|WS_DISABLED,IDC_SCORE_MOVE_R,&m_VirtualEngine,NULL);
	m_btAutoOpenCard.ActiveWindow(rcCreate,WS_CHILD|WS_DISABLED ,IDC_AUTO_OPEN_CARD,&m_VirtualEngine,NULL);
	m_btOpenCard.ActiveWindow(rcCreate,WS_CHILD|WS_VISIBLE,IDC_OPEN_CARD,&m_VirtualEngine,NULL);
	m_btBank.ActiveWindow(rcCreate,WS_CHILD,IDC_BANK,&m_VirtualEngine,NULL);
	m_btContinueCard.ActiveWindow(rcCreate,WS_CHILD,IDC_CONTINUE_CARD,&m_VirtualEngine,NULL);
	m_btBankerStorage.ActiveWindow(rcCreate,WS_CHILD|WS_VISIBLE|WS_DISABLED,IDC_BANK_STORAGE,&m_VirtualEngine,NULL);
	m_btBankerDraw.ActiveWindow(rcCreate,WS_CHILD|WS_VISIBLE,IDC_BANK_DRAW,&m_VirtualEngine,NULL);
	m_btUp.ActiveWindow(rcCreate,WS_CHILD|WS_VISIBLE|WS_DISABLED,IDC_UP,&m_VirtualEngine,NULL);
	m_btDown.ActiveWindow(rcCreate,WS_CHILD|WS_VISIBLE,IDC_DOWN,&m_VirtualEngine,NULL);
	//m_btStart.ActiveWindow(rcCreate,WS_CHILD|WS_VISIBLE,IDC_BT_START,&m_VirtualEngine,NULL);
	m_btJetton100.SetButtonImage(pD3DDevice,TEXT("PNG_BT_JETTON_100"),TEXT("PNG"),hResInstance);
	m_btJetton1000.SetButtonImage(pD3DDevice,TEXT("PNG_BT_JETTON_1000"),TEXT("PNG"),hResInstance);
	m_btJetton10000.SetButtonImage(pD3DDevice,TEXT("PNG_BT_JETTON_10000"),TEXT("PNG"),hResInstance);
	m_btJetton100000.SetButtonImage(pD3DDevice,TEXT("PNG_BT_JETTON_100000"),TEXT("PNG"),hResInstance);
	m_btJetton500000.SetButtonImage(pD3DDevice,TEXT("PNG_BT_JETTON_500000"),TEXT("PNG"),hResInstance);
	m_btJetton1000000.SetButtonImage(pD3DDevice,TEXT("PNG_BT_JETTON_1000000"),TEXT("PNG"),hResInstance);
	m_btJetton5000000.SetButtonImage(pD3DDevice,TEXT("PNG_BT_JETTON_5000000"),TEXT("PNG"),hResInstance);
	m_btJetton10000000.SetButtonImage(pD3DDevice,TEXT("PNG_BT_JETTON_10000000"),TEXT("PNG"),hResInstance);
	m_btApplyBanker.SetButtonImage(pD3DDevice,TEXT("PNG_BT_APPLY_BANKER"),TEXT("PNG"),hResInstance);
	m_btCancelBanker.SetButtonImage(pD3DDevice,TEXT("PNG_BT_CANCEL_APPLY"),TEXT("PNG"),hResInstance);
	m_btScoreMoveL.SetButtonImage(pD3DDevice,TEXT("PNG_BT_SCORE_MOVE_L"),TEXT("PNG"),hResInstance);
	m_btScoreMoveR.SetButtonImage(pD3DDevice,TEXT("PNG_BT_SCORE_MOVE_R"),TEXT("PNG"),hResInstance);
	m_btBank.SetButtonImage(pD3DDevice,TEXT("PNG_BT_BANK"),TEXT("PNG"),hResInstance);
	m_btContinueCard.SetButtonImage(pD3DDevice,TEXT("PNG_BT_CONTINUE_CARD"),TEXT("PNG"),hResInstance);
	m_btBankerStorage.SetButtonImage(pD3DDevice,TEXT("PNG_BT_STORAGE"),TEXT("PNG"),hResInstance);
	m_btBankerDraw.SetButtonImage(pD3DDevice,TEXT("PNG_BT_DRAW"),TEXT("PNG"),hResInstance);
	m_btAutoOpenCard.SetButtonImage(pD3DDevice,TEXT("PNG_BT_AUTO_OPEN_CARD"),TEXT("PNG"),hResInstance);
	m_btOpenCard.SetButtonImage(pD3DDevice,TEXT("PNG_BT_OPEN_CARD"),TEXT("PNG"),hResInstance);
	m_btUp.SetButtonImage(pD3DDevice,TEXT("PNG_BT_BT_S"),TEXT("PNG"),hResInstance);
	m_btDown.SetButtonImage(pD3DDevice,TEXT("PNG_BT_BT_X"),TEXT("PNG"),hResInstance);
	//m_btStart.SetButtonImage(pD3DDevice,TEXT("PNG_BT_START"),TEXT("PNG"),hResInstance);
	//m_btStart.ShowWindow(SW_HIDE);
	//���������ơ��������ư�Ŧ������
	m_btContinueCard.ShowWindow(false);
	m_btOpenCard.ShowWindow(false);
	m_btAutoOpenCard.ShowWindow(false);
	m_btJetton500000.ShowWindow(false);
	for (int i = 0; i < 5; ++i)
	{
		m_CardControl[i].LoadGameResource(pD3DDevice,nWidth,nHeight);
	}
	//��ʼ��FLASH����
	for (int i = 0; i < 2; ++i)
	{
		StartShowCardEx(i,m_ptFlash[i].x,m_ptFlash[i].y,false);
	}
	//������Ⱦ
	StartRenderThread();
}

//��������
VOID CGameClientView::CartoonMovie()
{
	BYTE cbGameStatus = m_pGameClientEngine->GetGameStatus();
	DWORD dwCurrentTime = GetTickCount();
	//�ֻ�ׯ��
	if ( m_stuCartoonInfo[CTN_CHANGE_BANKER].bShow)
	{
		if(dwCurrentTime-m_stuCartoonInfo[CTN_CHANGE_BANKER].dwShowTime>=m_stuCartoonInfo[CTN_CHANGE_BANKER].nShowTimer)
		{
			m_stuCartoonInfo[CTN_CHANGE_BANKER].dwShowTime = dwCurrentTime;
			m_stuCartoonInfo[CTN_CHANGE_BANKER].nShowFrame++;
			if(m_stuCartoonInfo[CTN_CHANGE_BANKER].nShowFrame>=80)
			{
 				m_stuCartoonInfo[CTN_CHANGE_BANKER].bShow = false;
 				m_stuCartoonInfo[CTN_CHANGE_BANKER].nShowFrame = 0;
 				m_bShowChangeBanker = false ;
			}
		}
	}
	//��ע��
	if ( m_stuCartoonInfo[CTN_BANKER_MAX].bShow)
	{
		if(dwCurrentTime-m_stuCartoonInfo[CTN_BANKER_MAX].dwShowTime>=m_stuCartoonInfo[CTN_BANKER_MAX].nShowTimer)
		{
			m_stuCartoonInfo[CTN_BANKER_MAX].dwShowTime = dwCurrentTime;
			m_stuCartoonInfo[CTN_BANKER_MAX].nShowFrame++;
			if(m_stuCartoonInfo[CTN_BANKER_MAX].nShowFrame>=80)
			{
				m_stuCartoonInfo[CTN_BANKER_MAX].bShow = false;
				m_stuCartoonInfo[CTN_BANKER_MAX].nShowFrame = 0;				
			}
		}
	}
	if(m_stuCartoonInfo[CTN_STAR_SELECT].bShow)
	{
		if(dwCurrentTime-m_stuCartoonInfo[CTN_STAR_SELECT].dwShowTime>=m_stuCartoonInfo[CTN_STAR_SELECT].nShowTimer)
		{
			m_stuCartoonInfo[CTN_STAR_SELECT].nShowFrame++;
			m_stuCartoonInfo[CTN_STAR_SELECT].dwShowTime = dwCurrentTime;
			if(m_stuCartoonInfo[CTN_STAR_SELECT].nShowFrame*m_stuCartoonInfo[CTN_STAR_SELECT].nShowTimer>=2000)
			{
				m_stuCartoonInfo[CTN_STAR_SELECT].bShow = false;
				m_stuCartoonInfo[CTN_STAR_SELECT].nShowFrame = 0;
				m_bSelectStar = false;
			}			
		}		
	}
	if(m_stuCartoonInfo[CTN_FLASH_EFFECT].bShow)
	{
		if(dwCurrentTime-m_stuCartoonInfo[CTN_FLASH_EFFECT].dwShowTime>=m_stuCartoonInfo[CTN_FLASH_EFFECT].nShowTimer)
		{
			m_stuCartoonInfo[CTN_FLASH_EFFECT].dwShowTime = dwCurrentTime;
			m_stuCartoonInfo[CTN_FLASH_EFFECT].nShowFrame++;
			bool bWindowMin = GetParent()->IsIconic();
			if(bWindowMin==false&&m_ShowLotteryFlashArray[m_wFlashId].IsWindowVisible()==false)
			{
				m_ShowLotteryFlashArray[m_wFlashId].ShowWindow(SW_SHOW);
			}
			//���ÿ�μ�����Դ�����ź�ɾ�� bool bFlashStop = m_ShowLotteryFlash.DrawAnimation(0);	
			//���ͷ�μ��أ���Ϸ�ͻ��˹ص����ͷ�
			bool bFlashStop = m_ShowLotteryFlashArray[m_wFlashId].DrawAnimation(0);	
			if(bFlashStop)
			{
				m_TexGod.DrawImage(m_pD3DDevice,m_ptGodBack.x,m_ptGodBack.y);
				m_bFlashShow = false;				
				m_stuCartoonInfo[CTN_FLASH_EFFECT].bShow = false;
				m_stuCartoonInfo[CTN_FLASH_EFFECT].nShowFrame = 0;
			}
		}		
	}
	if(m_stuCartoonInfo[CTN_FLASH_WAIT].bShow)
	{
		if(dwCurrentTime-m_stuCartoonInfo[CTN_FLASH_WAIT].dwShowTime>=m_stuCartoonInfo[CTN_FLASH_WAIT].nShowTimer)
		{
			m_stuCartoonInfo[CTN_FLASH_WAIT].dwShowTime = dwCurrentTime;
			m_stuCartoonInfo[CTN_FLASH_WAIT].bShow = false;
			m_stuCartoonInfo[CTN_FLASH_WAIT].nShowFrame = 0;
			OnPlayFlashEndEx(0,0);
		}
	}
	if (m_stuCartoonInfo[CTN_DISPATCH_TIP].bShow)
	{
		if(dwCurrentTime-m_stuCartoonInfo[CTN_DISPATCH_TIP].dwShowTime>=m_stuCartoonInfo[CTN_DISPATCH_TIP].nShowTimer)
		{
			m_stuCartoonInfo[CTN_DISPATCH_TIP].dwShowTime = dwCurrentTime;
			m_stuCartoonInfo[CTN_DISPATCH_TIP].bShow = false;
			m_stuCartoonInfo[CTN_DISPATCH_TIP].nShowFrame = 0;
			SetDispatchCardTip(enDispatchCardTip_NULL);
		}
	}
	if (m_stuCartoonInfo[CTN_DISPATCH_CARD].bShow)
	{
		if(dwCurrentTime-m_stuCartoonInfo[CTN_DISPATCH_CARD].dwShowTime>=m_stuCartoonInfo[CTN_DISPATCH_CARD].nShowTimer)
		{
			m_stuCartoonInfo[CTN_DISPATCH_CARD].dwShowTime = dwCurrentTime;
			m_stuCartoonInfo[CTN_DISPATCH_CARD].bShow = false;
			m_stuCartoonInfo[CTN_DISPATCH_CARD].nShowFrame = 0;
			//��ɷ���
			FinishDispatchCard();
			for (int i = 0;i<4;i++)
			{
				m_CardControl[i].m_blGameEnd = true;
				m_CardControl[i].m_blhideOneCard = false;
			}
			m_stuCartoonInfo[CTN_OPEN_CARD].bShow = false;
			m_stuCartoonInfo[CTN_OPEN_CARD].nShowFrame = 0;
			//�ȿ�����
			while (1)
			{
				bool blCopy = false;
				for (int i = 0;i<4;i++)
				{
					if(m_CardControl[i].m_CardItemArray.GetCount()!=m_CardControl[i].m_CardTempItemArray.GetCount())
					{
						blCopy = true;
						break;
					}
				}
				if(false == blCopy)
				{
					break;
				}else
				{
					m_CardControl[i].OnCopyCard();
				}
			}
			m_blMoveFinish = true;
		}		
	}
	//��Ӯѡ�����˸
	if(m_stuCartoonInfo[CTN_RESULT_FLASH].bShow)
	{
		if(dwCurrentTime-m_stuCartoonInfo[CTN_RESULT_FLASH].dwShowTime>=m_stuCartoonInfo[CTN_RESULT_FLASH].nShowTimer)
		{
			m_stuCartoonInfo[CTN_RESULT_FLASH].dwShowTime = dwCurrentTime;				//���嶯��ʱ��
			if(m_stuCartoonInfo[CTN_RESULT_FLASH].nShowFrame)
				m_stuCartoonInfo[CTN_RESULT_FLASH].nShowFrame = 0;
			else
				m_stuCartoonInfo[CTN_RESULT_FLASH].nShowFrame = 1;
			if(cbGameStatus!=GS_GAME_END)
			{
				m_stuCartoonInfo[CTN_RESULT_FLASH].bShow = false;
				m_cbAreaFlash = 0xFF;
				m_stuCartoonInfo[CTN_RESULT_FLASH].nShowFrame = 0;
			}
		}		
	}
	//����
	if(m_stuCartoonInfo[CTN_OPEN_CARD].bShow)
	{
		if(dwCurrentTime-m_stuCartoonInfo[CTN_OPEN_CARD].dwShowTime>=m_stuCartoonInfo[CTN_OPEN_CARD].nShowTimer)
		{
			m_stuCartoonInfo[CTN_OPEN_CARD].dwShowTime = dwCurrentTime;
			m_stuCartoonInfo[CTN_OPEN_CARD].nShowFrame++;
			m_blMoveFinish = false;
			if(m_CardControl[m_OpenCardIndex%4].m_MovePoint.x<17&&m_CardControl[m_OpenCardIndex%4].m_blAdd)//���ְ�
			{
				m_CardControl[m_OpenCardIndex%4].m_blGameEnd = false;
				m_CardControl[m_OpenCardIndex%4].m_MovePoint.x+=1;//3;
				m_CardControl[m_OpenCardIndex%4].m_MovePoint.y+=1;//7;
				m_CardControl[m_OpenCardIndex%4].m_blhideOneCard = false;
			}
			else
			{
				m_CardControl[m_OpenCardIndex%4].m_tMoveCard.m_blMoveCard = true;
				m_CardControl[m_OpenCardIndex%4].m_tMoveCard.m_blMoveFinish = true;
				m_CardControl[m_OpenCardIndex%4].m_MovePoint.x = 0;
				m_CardControl[m_OpenCardIndex%4].m_MovePoint.y = 0;
				m_CardControl[m_OpenCardIndex%4].m_blAdd = true;
				m_CardControl[m_OpenCardIndex%4].m_blGameEnd = true;
				m_OpenCardIndex++;
				if(m_OpenCardIndex>=5)
				{
					//����
					m_stuCartoonInfo[CTN_POST_CARD].bShow = false;						//�Ƿ񶯻�
					m_stuCartoonInfo[CTN_POST_CARD].nShowFrame = 0;					//��������
					for (int i = 0;i<4;i++)
					{
						m_CardControl[i].m_blGameEnd = true;
					}
					m_stuCartoonInfo[CTN_OPEN_CARD].bShow = false;
					m_stuCartoonInfo[CTN_OPEN_CARD].nShowFrame = 0;
					//������ʾ�ƶ�ʱ��
					m_stuCartoonInfo[CTN_DISPATCH_CARD].bShow = false;
					m_stuCartoonInfo[CTN_DISPATCH_CARD].dwShowTime = GetTickCount();
					m_stuCartoonInfo[CTN_DISPATCH_CARD].nShowFrame = 0;
					m_stuCartoonInfo[CTN_DISPATCH_CARD].bShow = true;
				}
			}
		}
	}
	//����	
	if(m_stuCartoonInfo[CTN_POST_CARD].bShow)
	{
		if(dwCurrentTime-m_stuCartoonInfo[CTN_POST_CARD].dwShowTime>=m_stuCartoonInfo[CTN_POST_CARD].nShowTimer)
		{
			m_stuCartoonInfo[CTN_POST_CARD].dwShowTime = dwCurrentTime;
			m_stuCartoonInfo[CTN_POST_CARD].nShowFrame++;
			if(m_bcShowCount<2)
			{
				if(m_CardControl[4].m_CardTempItemArray.GetCount()>0)
				{
					m_CardControl[4].m_blhideOneCard = false;
					m_CardControl[4].m_blGameEnd = true;
					m_CardControl[4].SetDisplayFlag(true);
					m_CardControl[4].ShowCardControl(true);
					m_CardControl[4].m_CardItemArray.SetSize(1);
					m_CardControl[4].m_CardItemArray[0]=m_CardControl[4].m_CardTempItemArray[0];
					m_CardControl[4].m_CardItemArray[0].cbCardData = m_bcfirstShowCard;
					m_CardControl[4].m_CardItemArray[0].bShoot = false;
				}
			}
			else
			{
				m_CardControl[4].m_CardItemArray.SetSize(0);

				bool blKillTimer = true;
				for (int i = 0;i<4;i++)
				{
					if(m_CardControl[i].m_CardItemArray.GetCount()!=m_CardControl[i].m_CardTempItemArray.GetCount())
					{
						blKillTimer = false;
						break;
					}
				}
				m_PostCardIndex = m_PostCardIndex%4;
				m_CardControl[m_PostCardIndex].OnCopyCard();
				m_PostCardIndex++;
				if(blKillTimer)
				{							
					m_stuCartoonInfo[CTN_OPEN_CARD].nShowFrame = 0;					//��������
					m_stuCartoonInfo[CTN_OPEN_CARD].dwShowTime = GetTickCount();					//���嶯��ʱ��
					m_stuCartoonInfo[CTN_OPEN_CARD].bShow = true;						//�Ƿ񶯻�
					m_stuCartoonInfo[CTN_POST_CARD].bShow = false;						//�Ƿ񶯻�
					m_stuCartoonInfo[CTN_POST_CARD].nShowFrame = 0;					//��������
				}
				else
				{
					::AfxGetMainWnd()->PostMessage(IDM_POSTCARD_SOUND,0,0);
				}
			}
			m_bcShowCount++;
		}
	}
	return;
}

//������Ϣ
int CGameClientView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct)==-1) return -1;

	AfxSetResourceHandle(AfxGetInstanceHandle());


	//����λͼ
	HINSTANCE hInstance=AfxGetInstanceHandle();

	//�����ؼ�
	CRect rcCreate(0,0,0,0);
	m_GameRecord.Create(IDD_DLG_GAME_RECORD,this);
	m_ApplyUser.Create( IDD_DLG_GAME_RECORD	, this );
	m_ApplyUser.ShowWindow(SW_HIDE);
	m_GameRecord.ShowWindow(SW_HIDE);

	m_btOpenAdmin.Create(NULL,WS_CHILD|WS_VISIBLE,CRect(4,4,11,11),this,IDC_BT_ADMIN);
	m_btOpenAdmin.ShowWindow(SW_HIDE);


	//���ð�ť
	HINSTANCE hResInstance=AfxGetInstanceHandle();

	//�˿˿ؼ�
	for (int i=0; i<CountArray(m_CardControl); ++i) 
	{
			m_CardControl[i].SetDisplayFlag(true);
	}

	//����
	m_hInst = NULL;
	m_pClientControlDlg = NULL;
	m_hInst = LoadLibrary(TEXT("GodInClientControl.dll"));
	if ( m_hInst )
	{
		typedef void * (*CREATE)(CWnd* pParentWnd); 
		CREATE ClientControl = (CREATE)GetProcAddress(m_hInst,"CreateClientControl"); 
		if ( ClientControl )
		{
			m_pClientControlDlg = static_cast<IClientControlDlg*>(ClientControl(this));
			m_pClientControlDlg->ShowWindow( SW_HIDE );
		}
	}

	if ( m_pClientControlDlg )
	{
		for ( int i = 0; i < CountArray(m_SkinListCtrl); ++i )
		{
			m_SkinListCtrl[i].Create(WS_CHILD, rcCreate, this, 2023 + i);
			m_SkinListCtrl[i].ModifyStyle(0,LVS_REPORT);

			m_SkinListCtrl[i].InsertColumn(0,TEXT("���"));
			m_SkinListCtrl[i].InsertColumn(1,TEXT("���"));

			m_SkinListCtrl[i].ShowWindow(SW_HIDE);
		}
	}
	
	return 0;
}

//���ý���
void CGameClientView::ResetGameView()
{
	//��ע��Ϣ
	ZeroMemory(m_lUserJettonScore,sizeof(m_lUserJettonScore));

	//ȫ����ע
	ZeroMemory(m_lAllJettonScore,sizeof(m_lAllJettonScore));

	//���������ע
	ZeroMemory(m_lAllPlayBet,sizeof(m_lAllPlayBet));


	//ׯ����Ϣ
	m_wBankerUser=INVALID_CHAIR;		
	m_wBankerTime=0;
	m_lBankerScore=0L;	
	m_lBankerWinScore=0L;
	m_lTmpBankerWinScore=0;

	//���ֳɼ�
	m_lMeCurGameScore=0L;	
	m_lMeCurGameReturnScore=0L;
	m_lBankerCurGameScore=0L;
	m_lGameRevenue=0L;

	//״̬��Ϣ
	m_lCurrentJetton=0L;
	m_cbAreaFlash=0xFF;
	m_wMeChairID=INVALID_CHAIR;
	m_bShowChangeBanker=false;
	m_bNeedSetGameRecord=false;
	m_bWinShunMen=false;
	m_bWinDuiMen=false;
	m_bWinDaoMen=false;
	m_bShowHalf = false;
	m_bShowGameResult=false;
	m_enDispatchCardTip=enDispatchCardTip_NULL;
	//����
	for (int i = 0; i < CTN_COUNT; ++i)
	{
		m_stuCartoonInfo[i].bShow = false;
		m_stuCartoonInfo[i].nShowFrame = 0;
	}

	m_lMeCurGameScore=0L;			
	m_lMeCurGameReturnScore=0L;	
	m_lBankerCurGameScore=0L;		

	m_lAreaLimitScore=0L;	

	//λ����Ϣ
	m_nScoreHead = 0;
	m_nRecordFirst= 0;	
	m_nRecordLast= 0;	

	//��ʷ�ɼ�
	m_lMeStatisticScore=0;

	//����б�
	m_ApplyUser.ClearAll();
	m_btUp.ShowWindow(SW_HIDE);
	m_btUp.EnableWindow(true);
	m_btDown.ShowWindow(SW_HIDE);
	m_btDown.EnableWindow(true);

	//�������
	CleanUserJetton();

	//���ð�ť
	m_btApplyBanker.ShowWindow(SW_SHOW);
	m_btApplyBanker.EnableWindow(FALSE);
	m_btCancelBanker.ShowWindow(SW_HIDE);
	//m_btStart.ShowWindow(SW_HIDE);
	m_btCancelBanker.SetButtonImage(m_pD3DDevice,TEXT("PNG_BT_CANCEL_APPLY"),TEXT("PNG"),AfxGetInstanceHandle());

	return;
}

void CGameClientView::OnUp()
{
	m_ApplyUser.m_AppyUserList.SendMessage(WM_VSCROLL, MAKELONG(SB_LINEUP,0),NULL);
	m_ApplyUser.m_AppyUserList.Invalidate(TRUE);
	double nPos = m_ApplyUser.m_AppyUserList.GetScrollPos(SB_VERT);
	double nMax = m_ApplyUser.m_AppyUserList.GetScrollLimit(SB_VERT);

}

void CGameClientView::OnDown()
{
	double nPos = m_ApplyUser.m_AppyUserList.GetScrollPos(SB_VERT);
	if(nPos>m_ApplyUser.m_AppyUserList.GetItemCount()-3)
	{
		return ;
	}
	
	m_ApplyUser.m_AppyUserList.SendMessage(WM_VSCROLL, MAKELONG(SB_LINEDOWN,0),NULL);
	m_ApplyUser.m_AppyUserList.Invalidate(TRUE);
	double nMax = m_ApplyUser.m_AppyUserList.GetScrollLimit(SB_VERT);
};

//�����ؼ�
void CGameClientView::RectifyControl(INT nWidth, INT nHeight)
{
	//����λ��
	int nCenterX=nWidth/2, nCenterY=nHeight/2, nBorderWidth=4;
	m_rcBack.SetRect((nWidth-900)/2,(nHeight-699)/2,(nWidth-900)/2+900,(nHeight-699)/2+699);
	//λ����Ϣ
	m_nWinFlagsExcursionX = m_rcBack.left+116;	
	m_nWinFlagsExcursionY = m_rcBack.top+33;
	//107,290,218,174   343,355,224,171    577,296,210,144
// 	m_rcShunMen.left=nCenterX - 345-2;
// 	m_rcShunMen.top=nCenterY-230;
// 	m_rcShunMen.right=m_rcShunMen.left+200;
// 	m_rcShunMen.bottom=m_rcShunMen.top+306;
	m_rcShunMen.SetRect(m_rcBack.left+107+13,m_rcBack.top+290,m_rcBack.left+107+218+23,m_rcBack.top+290+174+31);
	
	m_rcJiaoL.left=m_rcShunMen.left;
	m_rcJiaoL.top=m_rcShunMen.bottom-nBorderWidth;
	m_rcJiaoL.right=m_rcShunMen.right;
	m_rcJiaoL.bottom=m_rcJiaoL.top+92;

	m_rcQiao.left=m_rcShunMen.right-nBorderWidth;
	m_rcQiao.top=m_rcShunMen.top;
	m_rcQiao.right=m_rcQiao.left+175;
	m_rcQiao.bottom=m_rcQiao.top+93;

// 	m_rcDuiMen.left=nCenterX-145-1;
// 	m_rcDuiMen.top=nCenterY-132-4;
// 	m_rcDuiMen.right=m_rcDuiMen.left+145*2;
// 	m_rcDuiMen.bottom=m_rcDuiMen.top+132*2;
	m_rcDuiMen.SetRect(m_rcBack.left+343-23,m_rcBack.top+355,m_rcBack.left+343+224+28,m_rcBack.top+355+171+59);

// 	m_rcDaoMen.left=m_rcDuiMen.right-3;
// 	m_rcDaoMen.top=m_rcDuiMen.top-100+4;
// 	m_rcDaoMen.right=m_rcDaoMen.left+197-1;
//	m_rcDaoMen.bottom=m_rcDaoMen.top+308;
	m_rcDaoMen.SetRect(m_rcBack.left+577-12,m_rcBack.top+296-13,m_rcBack.left+577+210+10,m_rcBack.top+296+144+53);

	m_rcJiaoR.left=m_rcDaoMen.left;
	m_rcJiaoR.top=m_rcDaoMen.bottom-nBorderWidth;
	m_rcJiaoR.right=m_rcDaoMen.right;
	m_rcJiaoR.bottom=m_rcDuiMen.bottom;
	//˳�Ե���ͼ����
	m_rcShunMenBack[0].SetRect(m_rcBack.left+59,m_rcBack.top+248,m_rcBack.left+57+314,m_rcBack.top+248+290);
	m_rcShunMenBack[1].SetRect(m_rcBack.left+57+240,m_rcBack.top+248+15,m_rcBack.left+57+240+311,m_rcBack.top+248+15+300);
	m_rcShunMenBack[2].SetRect(m_rcBack.left+57+476,m_rcBack.top+248,m_rcBack.left+57+476+314,m_rcBack.top+248+290);
	//flash����
	m_ptGodBack.SetPoint(m_rcBack.left+450-73+1,m_rcBack.top+132);
 	m_ptFlash[0].SetPoint(m_ptGodBack.x+73+10-21,m_ptGodBack.y-10-30+3);
 	m_ptFlash[1].SetPoint(m_ptGodBack.x+73-3+1,m_ptGodBack.y);
	MoveFlashEx(3);
	
	//��������
	int ExcursionY=60;
	m_PointJettonNumber[ID_SHUN_MEN-1].SetPoint((m_rcShunMen.right+m_rcShunMen.left)/2, (m_rcShunMen.bottom+m_rcShunMen.top)/2-ExcursionY-40+60);
	m_PointJettonNumber[ID_JIAO_L-1].SetPoint((m_rcJiaoL.right+m_rcJiaoL.left)/2, (m_rcJiaoL.bottom+m_rcJiaoL.top)/2-ExcursionY);
	m_PointJettonNumber[ID_QIAO-1].SetPoint((m_rcQiao.right+m_rcQiao.left)/2, (m_rcQiao.bottom+m_rcQiao.top)/2-ExcursionY);
	m_PointJettonNumber[ID_DUI_MEN-1].SetPoint((m_rcDuiMen.right+m_rcDuiMen.left)/2, (m_rcDuiMen.bottom+m_rcDuiMen.top)/2-ExcursionY+30);
	m_PointJettonNumber[ID_DAO_MEN-1].SetPoint((m_rcDaoMen.right+m_rcDaoMen.left)/2, (m_rcDaoMen.bottom+m_rcDaoMen.top)/2-ExcursionY-40+60);
	m_PointJettonNumber[ID_JIAO_R-1].SetPoint((m_rcJiaoR.right+m_rcJiaoR.left)/2, (m_rcJiaoR.bottom+m_rcJiaoR.top)/2-ExcursionY);
	
	//����λ��
	m_PointJetton[ID_SHUN_MEN-1].SetPoint(m_rcShunMen.left, m_rcShunMen.top);
	m_PointJetton[ID_JIAO_L-1].SetPoint(m_rcJiaoL.left, m_rcJiaoL.top);
	m_PointJetton[ID_QIAO-1].SetPoint(m_rcQiao.left, m_rcQiao.top);
	m_PointJetton[ID_DUI_MEN-1].SetPoint(m_rcDuiMen.left, m_rcDuiMen.top);
	m_PointJetton[ID_DAO_MEN-1].SetPoint(m_rcDaoMen.left, m_rcDaoMen.top);
	m_PointJetton[ID_JIAO_R-1].SetPoint(m_rcJiaoR.left, m_rcJiaoR.top);

	//�˿˿ؼ�
	m_CardControl[0].SetBenchmarkPos(CPoint(nWidth/2,m_rcBack.top+279),enXCenter,enYTop);
	m_CardControl[1].SetBenchmarkPos(CPoint(m_rcBack.left+181,m_rcBack.top+333),enXLeft,enYTop);
	m_CardControl[2].SetBenchmarkPos(CPoint(nWidth/2,m_rcBack.top+471-27),enXCenter,enYTop);
	m_CardControl[3].SetBenchmarkPos(CPoint(m_rcBack.left+723,m_rcBack.top+333),enXRight,enYTop);
	m_CardControl[4].SetBenchmarkPos(CPoint(nWidth/2,m_rcBack.top+374),enXCenter,enYTop);

	//�ƶ��ؼ�
	HDWP hDwp=BeginDeferWindowPos(34);
	UINT uFlags=SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOCOPYBITS;

	m_ApplyUser.m_viewHandle = m_hWnd;

	//�б�ؼ�
	DeferWindowPos(hDwp,m_ApplyUser,NULL,nWidth/2 + 148-10,nHeight/2-291-7,216,50,uFlags);
	uFlags = SWP_NOSIZE;
	m_btUp.SetWindowPos(nWidth/2 + 138+216-3,nHeight/2-291-9,0,0,uFlags);
	m_btDown.SetWindowPos(nWidth/2 + 138+216-3,nHeight/2-291+50-16-7,0,0,uFlags);
	if(m_ApplyUser.GetItemCount()>4)
	{
		//m_btUp.ShowWindow(SW_SHOW);
		m_btUp.EnableWindow(true);
		//m_btDown.ShowWindow(SW_SHOW);
		m_btDown.EnableWindow(true);  
	}
	else
	{
		m_btUp.ShowWindow(SW_HIDE);
		m_btUp.EnableWindow(true);
		m_btDown.ShowWindow(SW_HIDE);
		m_btDown.EnableWindow(true); 

		m_ApplyUser.m_AppyUserList.SendMessage(WM_VSCROLL, MAKELONG(SB_TOP,0),NULL);
		m_ApplyUser.m_AppyUserList.Invalidate(TRUE);
	}
	int nApplyCount = m_ApplyUser.GetItemCount();
	//if(nApplyCount<=4)
	{
		m_nApplyFocusIndex = 0;
		m_rcApplyBlock.SetRect(m_rcBack.left+863,m_rcBack.top+181,m_rcBack.left+863+8,m_rcBack.top+181+12);
	}
	
	//���밴ť
	CRect rcJetton;
	m_btJetton100.GetWindowRect(rcJetton);
	int nXPos = m_rcBack.left+275+72-13;//327;
	int nYPos = m_rcBack.bottom-76;
	int nSpace = 8;

	//DeferWindowPos(hDwp,m_btJetton100,NULL,nXPos,nYPos,0,0,uFlags|SWP_NOSIZE);
	m_btJetton1000.SetWindowPos(nXPos ,nYPos,0,0,uFlags);
	m_btJetton10000.SetWindowPos(nXPos + nSpace * 1 + rcJetton.Width() * 1,nYPos,0,0,uFlags);
	//m_btJetton500000.SetWindowPos(nXPos + nSpace * 2 + rcJetton.Width() * 2,nYPos,0,0,uFlags);
	m_btJetton100000.SetWindowPos(nXPos + nSpace * 2 + rcJetton.Width() * 2,nYPos,0,0,uFlags);
	m_btJetton1000000.SetWindowPos(nXPos + nSpace * 3 + rcJetton.Width() * 3,nYPos,0,0,uFlags);
	m_btJetton5000000.SetWindowPos(nXPos + nSpace * 4 + rcJetton.Width() * 4,nYPos,0,0,uFlags);
	m_btJetton10000000.SetWindowPos(nXPos + nSpace * 5 + rcJetton.Width() * 5,nYPos,0,0,uFlags);

	m_btJetton100.ShowWindow(SW_HIDE);
	

	//��ׯ��ť	
	m_btScoreMoveL.SetWindowPos(m_rcBack.left+22,m_rcBack.top+67,0,0,uFlags);
	m_btScoreMoveR.SetWindowPos(m_rcBack.left+534+319,m_rcBack.top+67,0,0,uFlags);	
	m_btBank.SetWindowPos(nWidth/2-345,nHeight/2+143,0,0,uFlags);
	m_btContinueCard.SetWindowPos(nWidth/2-220-96,nHeight/2+143-48,0,0,uFlags);	

	m_btApplyBanker.SetWindowPos(m_rcBack.left+812-14,m_rcBack.top+241,0,0,uFlags);
	m_btCancelBanker.SetWindowPos(m_rcBack.left+812-14,m_rcBack.top+241,0,0,uFlags);
	m_btAutoOpenCard.SetWindowPos(m_rcBack.right-134,m_rcBack.bottom-133,0,0,uFlags);
	m_btOpenCard.SetWindowPos(m_rcBack.right-134,m_rcBack.bottom-133,0,0,uFlags);
	//m_btStart.SetWindowPos(m_rcBack.left+405,m_rcBack.top+385,0,0,uFlags);
	m_btBankerStorage.SetWindowPos(m_rcBack.right-155,m_rcBack.bottom-80,0,0,uFlags);
	m_btBankerDraw.SetWindowPos(m_rcBack.right-155,m_rcBack.bottom-48,0,0,uFlags);

	//�����ƶ�
	EndDeferWindowPos(hDwp);

	if ( m_pClientControlDlg )
	{
		CRect m_WinRect(m_rcShunMen);
		m_WinRect.top += m_WinRect.Height() / 2;
		m_SkinListCtrl[0].MoveWindow(m_WinRect);
		m_SkinListCtrl[0].SetColumnWidth( 0, m_WinRect.Width()/2 );
		m_SkinListCtrl[0].SetColumnWidth( 1, m_WinRect.Width()/2 );

		m_WinRect = m_rcDuiMen;
		m_WinRect.top += m_WinRect.Height() / 2;
		m_WinRect.OffsetRect(0, -20);
		m_SkinListCtrl[1].MoveWindow(m_WinRect);
		m_SkinListCtrl[1].SetColumnWidth(0, m_WinRect.Width()/2 );
		m_SkinListCtrl[1].SetColumnWidth(1, m_WinRect.Width()/2 );

		m_WinRect = m_rcDaoMen;
		m_WinRect.top += m_WinRect.Height() / 2;
		m_WinRect.OffsetRect(5, 0);
		m_SkinListCtrl[2].MoveWindow(m_WinRect);
		m_SkinListCtrl[2].SetColumnWidth(0, m_WinRect.Width()/2 );
		m_SkinListCtrl[2].SetColumnWidth(1, m_WinRect.Width()/2 );
	}

	return;
}

LONGLONG CGameClientView::GetBankMaxJetton()
{
	LONGLONG lBankerScore=92147483647L;
	if(m_wBankerUser!=INVALID_CHAIR) lBankerScore=m_lBankerScore;
	if(lBankerScore > 99999999999L) lBankerScore = 99999999999L;
	return lBankerScore;

}

//�滭����
VOID CGameClientView::DrawGameView(CD3DDevice * pD3DDevice, INT nWidth, INT nHeight)
{		
	//�滭����
	//DrawViewImage(pD3DDevice,m_TexViewFill,DRAW_MODE_SPREAD);
	pD3DDevice->FillRect(0,0,nWidth,nHeight,D3DCOLOR_XRGB(0,51,102));
	m_TexViewBack.DrawImage(pD3DDevice,m_rcBack.left,m_rcBack.top);
	//������Ů
	if(m_bFlashShow==false)
	{
		m_TexGod.DrawImage(pD3DDevice,m_ptGodBack.x,m_ptGodBack.y);
	}
	int nStarW = m_TexStar.GetWidth()/10;
	int nStarH = m_TexStar.GetHeight()/3;
	int nStarX = m_rcBack.left + 58;
	int nStarY = m_rcBack.top + 22;	
	static int nRandStar = rand()%STAR_COUNT;
	if(m_stuCartoonInfo[CTN_STAR_SELECT].nShowFrame%2==0)
	{
		nRandStar = rand()%STAR_COUNT;
	}
// 	for (int i = 0; i < STAR_COUNT; ++i)
// 	{
// 		if(m_bSelectStar)
// 		{
// 			if(i==nRandStar)
// 				m_TexStar.DrawImage(pD3DDevice,nStarX+i*79,nStarY,nStarW,nStarH,i*nStarW,nStarH*2,nStarW,nStarH);
// 			else
// 				m_TexStar.DrawImage(pD3DDevice,nStarX+i*79,nStarY,nStarW,nStarH,i*nStarW,0,nStarW,nStarH);
// 		}
// 		else
// 		{
// 			bool bSelect = false;
// 			bool bWin = false;
// 			for (int j = 0; j < 3; ++j)
// 			{
// 				if(m_cbStarSelect[j]==i)
// 				{
// 					bSelect = true;
// 					if (j==0&&WIN_FLAG_WIN==m_bWinShunMen)
// 						bWin = true;
// 					else if (j==1&&WIN_FLAG_WIN==m_bWinDuiMen)
// 						bWin = true;
// 					else if (j==2&&WIN_FLAG_WIN==m_bWinDaoMen)
// 						bWin = true;
// 					break;
// 				}
// 			}
// 			if(bSelect)
// 			{
// 				if(bWin&&(m_stuCartoonInfo[CTN_RESULT_FLASH].nShowFrame>0))
// 					m_TexStar.DrawImage(pD3DDevice,nStarX+i*79,nStarY,nStarW,nStarH,i*nStarW,nStarH*2,nStarW,nStarH);
// 				else
// 					m_TexStar.DrawImage(pD3DDevice,nStarX+i*79,nStarY,nStarW,nStarH,i*nStarW,0,nStarW,nStarH);
// 			}
// 			else
// 				m_TexStar.DrawImage(pD3DDevice,nStarX+i*79,nStarY,nStarW,nStarH,i*nStarW,nStarH,nStarW,nStarH);
// 		}
// 		m_TexProgressBack.DrawImage(pD3DDevice,nStarX+i*79+nStarW/2-34,nStarY+107);
// 		int nWinW = (int)((float)(m_TexProgress.GetWidth())*m_fStarWinRate[i]);
// 		m_TexProgress.DrawImage(pD3DDevice,nStarX+i*79+nStarW/2-34,nStarY+107,nWinW,6,0,0,nWinW,6);		
// 		int nStarWin = (int)(m_fStarWinRate[i]*100.0);
// 		DrawNumberStringWithSign(pD3DDevice,&m_TexProgressNum, nStarWin, nStarX+i*79+nStarW/2-34,nStarY+107+8);
// 	}
	//
	CPoint ptShunMen[3];
	BYTE cbStarSelect[3] = {STAR_COUNT,STAR_COUNT,STAR_COUNT};
	for (int i = 0; i < 3; ++i)
	{
		ptShunMen[i].SetPoint(m_rcShunMenBack[i].left,m_rcShunMenBack[i].top);
		cbStarSelect[i] = m_cbStarSelect[i];
// 		if(m_bSelectStar)
// 		{
// 			cbStarSelect[i] = rand()%STAR_COUNT;
// 		}
	}
// 	for (int i = 0; i < 3; ++i)
// 	{
// 		if(m_bSelectStar==false&&cbStarSelect[i]<STAR_COUNT)
// 		{
// 			int nStarBackW = m_TexStarShow[i].GetWidth()/STAR_COUNT;
// 			int nStarBackH = m_TexStarShow[i].GetHeight();
// 			m_TexStarShow[i].DrawImage(pD3DDevice,ptShunMen[i].x,ptShunMen[i].y,nStarBackW,nStarBackH,
// 				cbStarSelect[i]*nStarBackW,0,nStarBackW,nStarBackH);
// 		}
// 	}
	//��ȡ״̬
	BYTE cbGameStatus=m_pGameClientEngine->GetGameStatus();
		
	//״̬��ʾ
	//CFont static InfoFont;
	//InfoFont.CreateFont(-16,0,0,0,400,0,0,0,134,3,2,ANTIALIASED_QUALITY,2,TEXT("����"));
	//CFont * pOldFont=pDC->SelectObject(&InfoFont);
	//pDC->SetTextColor(RGB(255,234,0));
	//
	//pDC->SelectObject(pOldFont);
	//InfoFont.DeleteObject();

	//ʱ����ʾ
	//CImageHandle ImageHandleTimeFlag(&m_ImageTimeFlag);
	int nTimeFlagWidth = m_TexTimeFlag.GetWidth()/3;
	int nFlagIndex=0;
	if (cbGameStatus==GAME_STATUS_FREE) nFlagIndex=0;
	else if (cbGameStatus==GS_PLACE_JETTON) nFlagIndex=2;
	else if (cbGameStatus==GS_GAME_END) nFlagIndex=1;
	m_TexTimeFlag.DrawImage(pD3DDevice, m_rcBack.left+58,m_rcBack.top+532, nTimeFlagWidth, m_TexTimeFlag.GetHeight(),
		nFlagIndex * nTimeFlagWidth, 0);

	//ʱ��
	if(m_wMeChairID != INVALID_CHAIR)
	{
		WORD wUserTimer=GetUserClock(m_wMeChairID);
		if (wUserTimer!=0) 
		{
			DrawGameTimer(pD3DDevice,m_rcBack.left+46,m_rcBack.top+551,wUserTimer);
		}
	}
	// ������ׯ�б�
	int nApplyCount = m_ApplyUser.GetItemCount();
	tagApplyUser applyUser;
	CRect StrRect;//705,181,158,55
	if(nApplyCount>4)
	{
		//m_TexApplyBlockBack.DrawImage(pD3DDevice,m_rcBack.left+866,m_rcBack.top+182);
		m_TexApplyBlock.DrawImage(pD3DDevice,m_rcApplyBlock.left,m_rcApplyBlock.top);
	}
	for (int i = m_nApplyFocusIndex,j = 0; i < nApplyCount,j < 4; ++i,++j)
	{
		if(m_ApplyUser.GetUserInfo(i,applyUser))
		{
			StrRect.left = m_rcBack.left+708;
			StrRect.top = m_rcBack.top+181+j*14;
			StrRect.right = StrRect.left + 152;
			StrRect.bottom = StrRect.top + 14;
			DrawTextString(pD3DDevice,applyUser.strUserName,
				StrRect,DT_LEFT|DT_END_ELLIPSIS,D3DCOLOR_XRGB(255,217,180));
			DrawNumberStringWithSpace(pD3DDevice,applyUser.lUserScore, StrRect,D3DCOLOR_XRGB(255,217,180),DT_RIGHT|DT_TOP);
		}
	}
	//ʤ���߿�
	FlashJettonAreaFrame(nWidth,nHeight,pD3DDevice);
	//��ȡ״̬
	cbGameStatus=m_pGameClientEngine->GetGameStatus();
	if(cbGameStatus == GS_PLACE_JETTON)
	{
		LONGLONG allJetton = 0;

		for (int i = 1;i<AREA_ARRY_COUNT;i++)
		{
			allJetton+=m_lAllJettonScore[i];
		}
		DrawJettonPress(pD3DDevice,nWidth,nHeight,GetBankMaxJetton(),allJetton);
	}

	//������Դn
	//CImageHandle HandleJettonView(&m_ImageJettonView);
	CSize static SizeJettonItem(m_TexJettonView.GetWidth()/6,m_TexJettonView.GetHeight());

	//�滭����
	for (INT i=0;i<AREA_COUNT;i++)
	{
		//��������
		LONGLONG lScoreCount=0L;
		LONGLONG static lScoreJetton[JETTON_COUNT]={1000L,10000L,100000L,1000000L,5000000L,10000000L};
		int static nJettonViewIndex=0;

		//�滭����
		for (INT_PTR j=0;j<m_JettonInfoArray[i].GetCount();j++)
		{
			//��ȡ��Ϣ
			tagJettonInfo * pJettonInfo=&m_JettonInfoArray[i][j];

			//�ۼ�����
			ASSERT(pJettonInfo->cbJettonIndex<JETTON_COUNT);
			lScoreCount+=lScoreJetton[pJettonInfo->cbJettonIndex];

			//ͼƬ����
		/*	if (1==pJettonInfo->cbJettonIndex || 2==pJettonInfo->cbJettonIndex) nJettonViewIndex=pJettonInfo->cbJettonIndex+1;
			else if (0==pJettonInfo->cbJettonIndex) nJettonViewIndex=pJettonInfo->cbJettonIndex;
			else nJettonViewIndex=pJettonInfo->cbJettonIndex+2;*/

			nJettonViewIndex=pJettonInfo->cbJettonIndex;

			//�滭����
			m_TexJettonView.DrawImage(pD3DDevice,pJettonInfo->nXPos+m_PointJetton[i].x+16,
				pJettonInfo->nYPos+m_PointJetton[i].y+16,SizeJettonItem.cx,SizeJettonItem.cy,
				nJettonViewIndex*SizeJettonItem.cx,0);
		}

		//�滭����
		if (lScoreCount>0L)	
		{
			m_TexScoreBack.DrawImage(pD3DDevice,m_PointJettonNumber[i].x-102,m_PointJettonNumber[i].y-17);
			DrawNumberString(pD3DDevice,lScoreCount,m_PointJettonNumber[i].x,m_PointJettonNumber[i].y);
		}
	}

	//�滭ׯ��
	DrawBankerInfo(pD3DDevice,nWidth,nHeight);

	//�滭�û�
	DrawMeInfo(pD3DDevice,nWidth,nHeight);

	//�л�ׯ��
	if ( m_bShowChangeBanker&&m_stuCartoonInfo[CTN_CHANGE_BANKER].bShow )
	{
		int	nXPos = nWidth / 2 - 130+20;
		int	nYPos = m_rcBack.top+230+70;//nHeight / 2 - 160+20;
		int nAlpha = 0xFF/80*m_stuCartoonInfo[CTN_CHANGE_BANKER].nShowFrame;
		if(nAlpha>0xff)
			nAlpha = 0xff;
		BYTE cbAlpha = (BYTE)nAlpha;

		//������ׯ
		if ( m_wMeChairID == m_wBankerUser )
		{
			//CImageHandle ImageHandleBanker(&m_ImageMeBanker);
			m_TexMeBanker.DrawImage(pD3DDevice, nXPos, nYPos, m_TexMeBanker.GetWidth(), m_TexMeBanker.GetHeight(),
				0, 0,cbAlpha);
		}
		else if ( m_wBankerUser != INVALID_CHAIR )
		{
			//CImageHandle ImageHandleBanker(&m_ImageChangeBanker);
			m_TexChangeBanker.DrawImage(pD3DDevice, nXPos, nYPos, m_TexChangeBanker.GetWidth(), m_TexChangeBanker.GetHeight(),
				0, 0,cbAlpha);
		}
		else
		{
			//CImageHandle ImageHandleBanker(&m_ImageNoBanker);
			m_TexNoBanker.DrawImage(pD3DDevice, nXPos, nYPos, m_TexNoBanker.GetWidth(), m_TexNoBanker.GetHeight(),
				0, 0,cbAlpha);
		}
	}

    //������ʾ
	if (enDispatchCardTip_NULL!=m_enDispatchCardTip)
	{
		if (m_TexDispatchCardTip.IsNull()==false) 
			m_TexDispatchCardTip.Destory();
		if (enDispatchCardTip_Dispatch==m_enDispatchCardTip)
			m_TexDispatchCardTip.LoadImage(pD3DDevice,AfxGetInstanceHandle(),TEXT("PNG_DISPATCH_CARD"),TEXT("PNG"));
		else
			m_TexDispatchCardTip.LoadImage(pD3DDevice,AfxGetInstanceHandle(),TEXT("PNG_CONTINUE_CARD"),TEXT("PNG"));
		//CImageHandle ImageHandle(&m_ImageDispatchCardTip);

		
		m_TexDispatchCardTip.DrawImage(pD3DDevice,nWidth/2-m_TexDispatchCardTip.GetWidth()/2, m_rcBack.top+375,m_TexDispatchCardTip.GetWidth(),\
			m_TexDispatchCardTip.GetHeight(),0,0);
		//m_ImageDispatchCardTip.BitBlt(pDC->GetSafeHdc(), (nWidth-m_ImageDispatchCardTip.GetWidth())/2, nHeight/2);
	}

	if (m_TexDispatchCardTip.IsNull()==false) 
		m_TexDispatchCardTip.Destory();
	if (enDispatchCardTip_Dispatch==m_enDispatchCardTip)
		m_TexDispatchCardTip.LoadImage(pD3DDevice,AfxGetInstanceHandle(),TEXT("PNG_DISPATCH_CARD"),TEXT("PNG"));
	else
		m_TexDispatchCardTip.LoadImage(pD3DDevice,AfxGetInstanceHandle(),TEXT("PNG_CONTINUE_CARD"),TEXT("PNG"));
	//CImageHandle ImageHandle(&m_ImageDispatchCardTip);

	//�ҵ���ע
	DrawMeJettonNumber(pD3DDevice);

	//ʤ����־
	DrawWinFlags(pD3DDevice);

	//����״̬
	if (cbGameStatus==GS_GAME_END)
	{
		CD3DTexture *ImageJettonPoint[]={&m_btTexJetton1000,&m_btTexJetton10000,&m_btTexJetton100000,&m_btTexJetton500000,\
			&m_btTexJetton1000000,&m_btTexJetton5000000,&m_btTexJetton10000000};

		CRect rcJetton(CPoint(0,0),CPoint(0,0));
		if(rcJetton.Width()==0)
			m_btJetton100.GetWindowRect(rcJetton);
		int nYPos = nHeight/2+175;
		int nXPos = nWidth/2-64-150-1+15;
		int nSpace = 3;

// 		for (int n = 0;n<7;n++)
// 		{
// 			ImageJettonPoint[n]->DrawImage( pD3DDevice, nXPos + nSpace * n + rcJetton.Width() * n, nYPos,ImageJettonPoint[n]->GetWidth()/5 , 
// 				ImageJettonPoint[n]->GetHeight(),ImageJettonPoint[n]->GetWidth()/5 * 4, 0, RGB(255, 0, 255) );
// 		}
		//�滭�˿�
		for (int i=0; i<CountArray(m_CardControl); ++i)
			m_CardControl[i].DrawCardControl(pD3DDevice);
	}

	if ( m_bBankerMax&&m_stuCartoonInfo[CTN_BANKER_MAX].bShow&&cbGameStatus==GS_PLACE_JETTON )
	{
		int nAlpha = 0xFF/80*m_stuCartoonInfo[CTN_BANKER_MAX].nShowFrame;
		if(nAlpha>0xff)
			nAlpha = 0xff;
		BYTE cbAlpha = (BYTE)nAlpha;	
		//m_ImageBankerMax.DrawImage(pDCBuff,(nWidth-m_ImageBankerMax.GetWidth())/2,(nHeight-m_ImageBankerMax.GetHeight())/2);
		m_TexPastHalf.DrawImage(pD3DDevice,(nWidth-m_TexPastHalf.GetWidth())/2,m_rcBack.top+375-130,
			m_TexPastHalf.GetWidth(),m_TexPastHalf.GetHeight(),0,0,cbAlpha);
	}
	else if ( m_stuCartoonInfo[CTN_BANKER_MAX].bShow&&cbGameStatus==GS_GAME_END )
	{
		int nAlpha = 0xFF/80*m_stuCartoonInfo[CTN_BANKER_MAX].nShowFrame;
		if(nAlpha>0xff)
			nAlpha = 0xff;
		BYTE cbAlpha = (BYTE)nAlpha;	
		//m_ImageBankerMax.DrawImage(pDCBuff,(nWidth-m_ImageBankerMax.GetWidth())/2,(nHeight-m_ImageBankerMax.GetHeight())/2);
		m_TexBankerBomb.DrawImage(pD3DDevice,(nWidth-m_TexBankerBomb.GetWidth())/2,m_rcBack.top+375-115,
			m_TexBankerBomb.GetWidth(),m_TexBankerBomb.GetHeight(),0,0,cbAlpha);
	}

	if(m_blMoveFinish)
	{
		//��ʾ���
		ShowGameResult(pD3DDevice, nWidth, nHeight);
	}	
		
	return;
}

//�滭����
/*void CGameClientView::DrawGameView(CDC * pDC, INT nWidth, INT nHeight)
{
	//�滭����
	//DrawViewImage(pDC,m_ImageViewFill,DRAW_MODE_SPREAD);
	//DrawViewImage(pDC,m_ImageViewBack, DRAW_MODE_CENTENT);


	CDC* pDCBuff = pDC;

	//�滭����
	//DrawViewImage(pDCBuff,m_ImageBack,DRAW_MODE_SPREAD);
	//DrawViewImage(pDCBuff,m_ImageCenter,DRAW_MODE_CENTENT);

	for ( int iW = 0 ; iW < nWidth; iW += m_ImageViewFill.GetWidth() )
	{
		for ( int iH = 0;  iH < nHeight; iH += m_ImageViewFill.GetHeight() )
		{
			m_ImageViewFill.BitBlt(pDC->GetSafeHdc(), iW, iH);
		}
	}
	m_ImageViewBack.BitBlt( pDC->GetSafeHdc(), nWidth/2 - m_ImageViewBack.GetWidth()/2, nHeight/2 - m_ImageViewBack.GetHeight()/2 );

	//��ȡ״̬
	BYTE cbGameStatus=m_pGameClientEngine->GetGameStatus();

	if(cbGameStatus == GS_PLACE_JETTON)
	{
		LONGLONG allJetton = 0;

		for (int i = 1;i<AREA_ARRY_COUNT;i++)
		{
			allJetton+=m_lAllJettonScore[i];
		}

		//DrawJettonPress(pDC,nWidth,nHeight,m_lBankerScore-allJetton,allJetton);
		DrawJettonPress(pDCBuff,nWidth,nHeight,GetBankMaxJetton(),allJetton);
	}

	//״̬��ʾ
	//CFont static InfoFont;
	//InfoFont.CreateFont(-16,0,0,0,400,0,0,0,134,3,2,ANTIALIASED_QUALITY,2,TEXT("����"));
	//CFont * pOldFont=pDC->SelectObject(&InfoFont);
	//pDC->SetTextColor(RGB(255,234,0));
	//
	//pDC->SelectObject(pOldFont);
	//InfoFont.DeleteObject();

	//ʱ����ʾ
	//CImageHandle ImageHandleTimeFlag(&m_ImageTimeFlag);
	int nTimeFlagWidth = m_ImageTimeFlag.GetWidth()/3;
	int nFlagIndex=0;
	if (cbGameStatus==GAME_STATUS_FREE) nFlagIndex=0;
	else if (cbGameStatus==GS_PLACE_JETTON) nFlagIndex=1;
	else if (cbGameStatus==GS_GAME_END) nFlagIndex=2;

	m_ImageTimeFlag.TransDrawImage(pDCBuff, nWidth/2-348+215, nHeight/2+225-495, nTimeFlagWidth, m_ImageTimeFlag.GetHeight(),
		nFlagIndex * nTimeFlagWidth, 0,RGB(255,0,255));

	//ʱ��
	if(m_wMeChairID != INVALID_CHAIR)
	{
		WORD wUserTimer=GetUserClock(m_wMeChairID);
		if (wUserTimer!=0) 
		{
			DrawGameTimer(pDCBuff,nWidth/2-323+226-30,nHeight/2+197-458-40-30,wUserTimer);
		}
	}

	//ʤ���߿�
	FlashJettonAreaFrame(nWidth,nHeight,pDCBuff);

	//������Դn
	//CImageHandle HandleJettonView(&m_ImageJettonView);
	CSize static SizeJettonItem(m_ImageJettonView.GetWidth()/7,m_ImageJettonView.GetHeight());

	//�滭����
	for (INT i=0;i<AREA_COUNT;i++)
	{
		//��������
		LONGLONG lScoreCount=0L;
		LONGLONG static lScoreJetton[JETTON_COUNT]={10000L,100000L,500000L,1000000L,5000000L,10000000L,30000000L};
		int static nJettonViewIndex=0;

		//�滭����
		for (INT_PTR j=0;j<m_JettonInfoArray[i].GetCount();j++)
		{
			//��ȡ��Ϣ
			tagJettonInfo * pJettonInfo=&m_JettonInfoArray[i][j];

			//�ۼ�����
			ASSERT(pJettonInfo->cbJettonIndex<JETTON_COUNT);
			lScoreCount+=lScoreJetton[pJettonInfo->cbJettonIndex];

			//ͼƬ����
			//if (1==pJettonInfo->cbJettonIndex || 2==pJettonInfo->cbJettonIndex) nJettonViewIndex=pJettonInfo->cbJettonIndex+1;
			//else if (0==pJettonInfo->cbJettonIndex) nJettonViewIndex=pJettonInfo->cbJettonIndex;
			//else nJettonViewIndex=pJettonInfo->cbJettonIndex+2;

			nJettonViewIndex=pJettonInfo->cbJettonIndex;

			//�滭����
			m_ImageJettonView.TransDrawImage(pDCBuff,pJettonInfo->nXPos+m_PointJetton[i].x+16,
				pJettonInfo->nYPos+m_PointJetton[i].y+16,SizeJettonItem.cx,SizeJettonItem.cy,
				nJettonViewIndex*SizeJettonItem.cx,0,RGB(255,0,255));
		}

		//�滭����
		if (lScoreCount>0L)	DrawNumberString(pDCBuff,lScoreCount,m_PointJettonNumber[i].x,m_PointJettonNumber[i].y);
	}

	//�滭ׯ��
	DrawBankerInfo(pDCBuff,nWidth,nHeight);

	//�滭�û�
	DrawMeInfo(pDCBuff,nWidth,nHeight);

	//�л�ׯ��
	if ( m_bShowChangeBanker )
	{
		int	nXPos = nWidth / 2 - 130+20;
		int	nYPos = nHeight / 2 - 160+20;

		//������ׯ
		if ( m_wMeChairID == m_wBankerUser )
		{
			//CImageHandle ImageHandleBanker(&m_ImageMeBanker);
			m_ImageMeBanker.TransDrawImage(pDCBuff, nXPos, nYPos, m_ImageMeBanker.GetWidth(), m_ImageMeBanker.GetHeight(),
				0, 0,RGB(255,0,255));
		}
		else if ( m_wBankerUser != INVALID_CHAIR )
		{
			//CImageHandle ImageHandleBanker(&m_ImageChangeBanker);
			m_ImageChangeBanker.TransDrawImage(pDCBuff, nXPos, nYPos, m_ImageChangeBanker.GetWidth(), m_ImageChangeBanker.GetHeight(),
				0, 0,RGB(255,0,255));
		}
		else
		{
			//CImageHandle ImageHandleBanker(&m_ImageNoBanker);
			m_ImageNoBanker.TransDrawImage(pDCBuff, nXPos, nYPos, m_ImageNoBanker.GetWidth(), m_ImageNoBanker.GetHeight(),
				0, 0,RGB(255,0,255));
		}
	}

    //������ʾ
	if (enDispatchCardTip_NULL!=m_enDispatchCardTip)
	{
		if (m_ImageDispatchCardTip.IsNull()==false) m_ImageDispatchCardTip.Destroy();
		if (enDispatchCardTip_Dispatch==m_enDispatchCardTip)
			m_ImageDispatchCardTip.LoadFromResource(this,AfxGetInstanceHandle(),IDB_DISPATCH_CARD);
		else
			m_ImageDispatchCardTip.LoadFromResource(this,AfxGetInstanceHandle(),IDB_CONTINUE_CARD);
		//CImageHandle ImageHandle(&m_ImageDispatchCardTip);

		
		m_ImageDispatchCardTip.TransDrawImage(pDCBuff,nWidth/2-m_ImageDispatchCardTip.GetWidth()/2, nHeight/2,m_ImageDispatchCardTip.GetWidth(),\
			m_ImageDispatchCardTip.GetHeight(),0,\
			0 ,RGB(255,0,255));
		//m_ImageDispatchCardTip.BitBlt(pDC->GetSafeHdc(), (nWidth-m_ImageDispatchCardTip.GetWidth())/2, nHeight/2);
	}

	if (m_ImageDispatchCardTip.IsNull()==false) m_ImageDispatchCardTip.Destroy();
	if (enDispatchCardTip_Dispatch==m_enDispatchCardTip)
		m_ImageDispatchCardTip.LoadFromResource(this,AfxGetInstanceHandle(),IDB_DISPATCH_CARD);
	else
		m_ImageDispatchCardTip.LoadFromResource(this,AfxGetInstanceHandle(),IDB_CONTINUE_CARD);
	//CImageHandle ImageHandle(&m_ImageDispatchCardTip);

	//�ҵ���ע
	DrawMeJettonNumber(pDCBuff);

	//ʤ����־
	DrawWinFlags(pDCBuff);

	//����״̬
	if (cbGameStatus==GS_GAME_END)
	{
		CBitImageEx *ImageJettonPoint[]={&m_btImageJetton10000,&m_btImageJetton100000,&m_btImageJetton500000,\
			&m_btImageJetton1000000,&m_btImageJetton5000000,&m_btImageJetton10000000,&m_btImageJetton30000000};

		static CRect rcJetton(CPoint(0,0),CPoint(0,0));
		if(rcJetton.Width()==0)
			m_btJetton100.GetWindowRect(&rcJetton);
		int nYPos = nHeight/2+175;
		int nXPos = nWidth/2-64-150-1+15;
		int nSpace = 3;

		for (int n = 0;n<7;n++)
		{
			//CImageHandle ImageHandleBanker(ImageJettonPoint[n]);
			ImageJettonPoint[n]->TransDrawImage( pDCBuff, nXPos + nSpace * n + rcJetton.Width() * n, nYPos,ImageJettonPoint[n]->GetWidth()/5 , 
				ImageJettonPoint[n]->GetHeight(),ImageJettonPoint[n]->GetWidth()/5 * 4, 0, RGB(255, 0, 255) );
		}
		//�滭�˿�
		for (int i=0; i<CountArray(m_CardControl); ++i) m_CardControl[i].DrawCardControl(pDCBuff);
	}

	if (m_bBankerMax)
	{
		//m_ImageBankerMax.DrawImage(pDCBuff,(nWidth-m_ImageBankerMax.GetWidth())/2,(nHeight-m_ImageBankerMax.GetHeight())/2);
		m_imgPastHalf.TransDrawImage(pDCBuff,(nWidth-m_imgPastHalf.GetWidth())/2,(nHeight-m_imgPastHalf.GetHeight())/2,
			m_imgPastHalf.GetWidth(),m_imgPastHalf.GetHeight(),0,0,RGB(255,0,255));
	}

	if(m_blMoveFinish)
	{
		//��ʾ���
		ShowGameResult(pDCBuff, nWidth, nHeight);
	}

	return;
}*/

//������Ϣ
void CGameClientView::SetMeMaxScore(LONGLONG lMeMaxScore)
{
	if (m_lMeMaxScore!=lMeMaxScore)
	{
		//���ñ���
		m_lMeMaxScore=lMeMaxScore;
	}

	return;
}

//������Ϣ
void CGameClientView::SetUserLimitScore(LONGLONG lUserLimitScore)
{
	if (m_lUserLimitScore!=lUserLimitScore)
	{
		//���ñ���
		m_lUserLimitScore=lUserLimitScore;
	}

	return;
}



//�����ע
void CGameClientView::SetAreaLimitScore(LONGLONG lAreaLimitScore)
{
	if ( m_lAreaLimitScore!= lAreaLimitScore )
	{
		//���ñ���
		m_lAreaLimitScore=lAreaLimitScore;
	}
}

//���ó���
void CGameClientView::SetCurrentJetton(LONGLONG lCurrentJetton)
{
	//���ñ���
	ASSERT(lCurrentJetton>=0L);
	m_lCurrentJetton=lCurrentJetton;

	if(m_pGameClientEngine && m_pGameClientEngine->GetGameStatus() == GS_PLACE_JETTON)
	{
		if(m_lCurrentJetton == 0L)
		{
			m_cbAreaFlash = 0xFF;
		}
	}

	return;
}

//��ʷ��¼
void CGameClientView::SetGameHistory(BYTE bWinShunMen, BYTE bWinDaoMen, BYTE bWinDuiMen)
{
	//��������
	tagClientGameRecord &GameRecord = m_GameRecordArrary[m_nRecordLast];
	GameRecord.bWinShunMen = bWinShunMen;
	GameRecord.bWinDuiMen  = bWinDuiMen;
	GameRecord.bWinDaoMen  = bWinDaoMen;

	//��������
	if (0==m_lUserJettonScore[ID_SHUN_MEN]) GameRecord.enOperateShunMen=enOperateResult_NULL;
	else if (m_lUserJettonScore[ID_SHUN_MEN] > 0 && WIN_FLAG_WIN==bWinShunMen) GameRecord.enOperateShunMen=enOperateResult_Win;
	else if (m_lUserJettonScore[ID_SHUN_MEN] > 0 && WIN_FLAG_LOST==bWinShunMen) GameRecord.enOperateShunMen=enOperateResult_Lost;

	if (0==m_lUserJettonScore[ID_DAO_MEN]) GameRecord.enOperateDaoMen=enOperateResult_NULL;
	else if (m_lUserJettonScore[ID_DAO_MEN] > 0 && WIN_FLAG_WIN==bWinDaoMen) GameRecord.enOperateDaoMen=enOperateResult_Win;
	else if (m_lUserJettonScore[ID_DAO_MEN] >0 && WIN_FLAG_LOST==bWinDaoMen) GameRecord.enOperateDaoMen=enOperateResult_Lost;

	if (0==m_lUserJettonScore[ID_DUI_MEN]) GameRecord.enOperateDuiMen=enOperateResult_NULL;
	else if (m_lUserJettonScore[ID_DUI_MEN] > 0 && WIN_FLAG_WIN==bWinDuiMen) GameRecord.enOperateDuiMen=enOperateResult_Win;
	else if (m_lUserJettonScore[ID_DUI_MEN]>0 && WIN_FLAG_LOST==bWinDuiMen) GameRecord.enOperateDuiMen=enOperateResult_Lost;
	
	//�ƶ��±�
	int iOrCount = m_nRecordLast;
	m_nRecordLast = (m_nRecordLast+1) % MAX_SCORE_HISTORY;
	if ( m_nRecordLast == m_nRecordFirst )
	{
		m_nRecordFirst = (m_nRecordFirst+1) % MAX_SCORE_HISTORY;
		if ( m_nScoreHead < m_nRecordFirst ) m_nScoreHead = m_nRecordFirst;
	}


	int nHistoryCount = (m_nRecordLast - m_nRecordFirst + MAX_SCORE_HISTORY) % MAX_SCORE_HISTORY;
	if ( MAX_FALG_COUNT < nHistoryCount ) m_btScoreMoveR.EnableWindow(TRUE);

	//�Ƶ����¼�¼
	if ( MAX_FALG_COUNT < nHistoryCount )
	{
		m_nScoreHead = (m_nRecordLast - MAX_FALG_COUNT + MAX_SCORE_HISTORY) % MAX_SCORE_HISTORY;
		m_btScoreMoveL.EnableWindow(TRUE);
		m_btScoreMoveR.EnableWindow(FALSE);
	}

	return;
}

//�������
void CGameClientView::CleanUserJetton()
{
	//��������
	for (BYTE i=0;i<CountArray(m_JettonInfoArray);i++)
	{
		m_JettonInfoArray[i].RemoveAll();
	}

	//��ע��Ϣ
	ZeroMemory(m_lUserJettonScore,sizeof(m_lUserJettonScore));

	//ȫ����ע
	ZeroMemory(m_lAllJettonScore,sizeof(m_lAllJettonScore));

	//���������ע
	ZeroMemory(m_lAllPlayBet,sizeof(m_lAllPlayBet));
	for ( int i = 0; i < CountArray(m_SkinListCtrl); ++i )
	{
		if ( m_SkinListCtrl[i].GetSafeHwnd() )
			m_SkinListCtrl[i].DeleteAllItems();
	}
	return;
}

//������ע
void CGameClientView::SetMePlaceJetton(BYTE cbViewIndex, LONGLONG lJettonCount)
{
	//Ч�����
	ASSERT(cbViewIndex<=ID_JIAO_R);
	if (cbViewIndex>ID_JIAO_R) return;

	m_lUserJettonScore[cbViewIndex]=lJettonCount;

}

//�����˿�
void CGameClientView::SetCardInfo(BYTE cbTableCardArray[4][2])
{
	if (cbTableCardArray!=NULL)
	{
		CopyMemory(m_cbTableCardArray,cbTableCardArray,sizeof(m_cbTableCardArray));

		//��ʼ����
		DispatchCard();

		m_cbAreaFlash=0xFF;
		//��Ӯѡ�����˸
		m_stuCartoonInfo[CTN_RESULT_FLASH].bShow = false;						
		m_stuCartoonInfo[CTN_RESULT_FLASH].nShowFrame = 0;
	}
	else
	{
		ZeroMemory(m_cbTableCardArray,sizeof(m_cbTableCardArray));
	}
}

//���ó���
void CGameClientView::PlaceUserJetton(BYTE cbViewIndex, LONGLONG lScoreCount)
{
	//Ч�����
	ASSERT(cbViewIndex<=ID_JIAO_R);
	if (cbViewIndex>ID_JIAO_R) return;

	//��������
	bool bPlaceJetton=false;
	LONGLONG lScoreIndex[JETTON_COUNT]={1000L,10000L,100000L,1000000L,5000000L,10000000L};

	//�߿���
	int nFrameWidth=0, nFrameHeight=0;
	int nBorderWidth=32;

	switch (cbViewIndex)
	{
	case ID_SHUN_MEN:
		{ 
			m_lAllJettonScore[ID_SHUN_MEN] += lScoreCount;
			nFrameWidth = m_rcShunMen.right-m_rcShunMen.left;
			nFrameHeight = m_rcShunMen.bottom-m_rcShunMen.top;
			break;
		}
	case ID_JIAO_L:
		{
			m_lAllJettonScore[ID_JIAO_L] += lScoreCount;
			nFrameWidth = m_rcJiaoL.right-m_rcJiaoL.left;
			nFrameHeight = m_rcJiaoL.bottom-m_rcJiaoL.top;
			break;
		}
	case ID_QIAO:
		{ 
			m_lAllJettonScore[ID_QIAO] += lScoreCount;
			nFrameWidth = m_rcQiao.right-m_rcQiao.left;
			nFrameHeight = m_rcQiao.bottom-m_rcQiao.top;
			break;
		}
	case ID_DUI_MEN:
		{ 
			m_lAllJettonScore[ID_DUI_MEN] += lScoreCount;
			nFrameWidth = m_rcDuiMen.right-m_rcDuiMen.left;
			nFrameHeight = m_rcDuiMen.bottom-m_rcDuiMen.top;
			break;
		}
	case ID_DAO_MEN:
		{ 
			m_lAllJettonScore[ID_DAO_MEN] += lScoreCount;
			nFrameWidth = m_rcDaoMen.right-m_rcDaoMen.left;
			nFrameHeight = m_rcDaoMen.bottom-m_rcDaoMen.top;
			break;
		}
	case ID_JIAO_R:
		{ 
			m_lAllJettonScore[ID_JIAO_R] += lScoreCount;
			nFrameWidth = m_rcJiaoR.right-m_rcJiaoR.left;
			nFrameHeight = m_rcJiaoR.bottom-m_rcJiaoR.top;
			break;
		}	
	default:
		{
			ASSERT(FALSE);
			return;
		}
	}

	nFrameWidth -= nBorderWidth;
	nFrameHeight -= nBorderWidth;

	//�����ж�
	bool bAddJetton=lScoreCount>0?true:false;
	lScoreCount=abs((int)lScoreCount);

	//���ӳ���
	for (BYTE i=0;i<CountArray(lScoreIndex);i++)
	{
		//������Ŀ
		BYTE cbScoreIndex=JETTON_COUNT-i-1;
		LONGLONG lCellCount=lScoreCount/lScoreIndex[cbScoreIndex];

		//�������
		if (lCellCount==0L) continue;

		//�������
		for (LONG j=0;j<lCellCount;j++)
		{
			if (true==bAddJetton)
			{
				//�������
				tagJettonInfo JettonInfo;
				int nJettonSize=68;
				JettonInfo.cbJettonIndex=cbScoreIndex;
				JettonInfo.nXPos=rand()%(nFrameWidth-nJettonSize);
				JettonInfo.nYPos=rand()%(nFrameHeight-nJettonSize);

				//��������
				bPlaceJetton=true;
				m_JettonInfoArray[cbViewIndex-1].Add(JettonInfo);
			}
			else
			{
				for (int nIndex=0; nIndex<m_JettonInfoArray[cbViewIndex-1].GetCount(); ++nIndex)
				{
					//�Ƴ��ж�
					tagJettonInfo &JettonInfo=m_JettonInfoArray[cbViewIndex-1][nIndex];
					if (JettonInfo.cbJettonIndex==cbScoreIndex)
					{
						m_JettonInfoArray[cbViewIndex-1].RemoveAt(nIndex);
						break;
					}
				}
			}
		}

		//������Ŀ
		lScoreCount-=lCellCount*lScoreIndex[cbScoreIndex];
	}

	return;
}

//���ֳɼ�
void CGameClientView::SetCurGameScore(LONGLONG lMeCurGameScore, LONGLONG lMeCurGameReturnScore, LONGLONG lBankerCurGameScore, LONGLONG lGameRevenue)
{
	m_lMeCurGameScore=lMeCurGameScore;			
	m_lMeCurGameReturnScore=lMeCurGameReturnScore;			
	m_lBankerCurGameScore=lBankerCurGameScore;			
	m_lGameRevenue=lGameRevenue;					
}

//����ʤ��
void CGameClientView::SetWinnerSide(BYTE bWinShunMen, BYTE bWinDuiMen, BYTE bWinDaoMen, bool bSet)
{		
	//���ñ���
	m_bWinShunMen=bWinShunMen;
	m_bWinDuiMen=bWinDuiMen;
	m_bWinDaoMen=bWinDaoMen;	
	m_bShowGameResult=bSet;
	m_cbAreaFlash=0xFF;
	//��Ӯѡ�����˸
	m_stuCartoonInfo[CTN_RESULT_FLASH].bShow = bSet;						
	m_stuCartoonInfo[CTN_RESULT_FLASH].nShowFrame = 0;
	m_stuCartoonInfo[CTN_RESULT_FLASH].dwShowTime = GetTickCount();
	return;
}

//��ȡ����
BYTE CGameClientView::GetJettonArea(CPoint MousePoint)
{
	if (m_rcShunMen.PtInRect(MousePoint)) return ID_SHUN_MEN;
	if (m_rcDuiMen.PtInRect(MousePoint)) return ID_DUI_MEN;	
	if (m_rcDaoMen.PtInRect(MousePoint)) return ID_DAO_MEN;	
	return 0xFF;
}
//�滭����(nLeft=0��1��2��,nTop=0��1��2��)
void CGameClientView::DrawNumberStringWithPng(CD3DDevice * pD3DDevice,CD3DTexture* pTexNumBack, LONGLONG lNumber, INT nXPos, INT nYPos, bool bMeScore,int nLeft,int nTop)
{

	CSize SizeScoreNumber(pTexNumBack->GetWidth()/10,pTexNumBack->GetHeight());

	if ( bMeScore ) SizeScoreNumber.SetSize(pTexNumBack->GetWidth()/10, pTexNumBack->GetHeight());

	//������Ŀ
	LONGLONG lNumberCount=0;
	LONGLONG lNumberTemp=lNumber;
	do
	{
		lNumberCount++;
		lNumberTemp/=10;
	} while (lNumberTemp>0);

	//λ�ö���
	INT nYDrawPos=INT(nYPos-SizeScoreNumber.cy/2);
	INT nXDrawPos=INT(nXPos+lNumberCount*SizeScoreNumber.cx/2-SizeScoreNumber.cx);
	switch(nLeft)
	{
	case 0:
		nXDrawPos=INT(nXPos+lNumberCount*SizeScoreNumber.cx-SizeScoreNumber.cx);
		break;
	case 1:
		nXDrawPos=INT(nXPos+lNumberCount*SizeScoreNumber.cx/2-SizeScoreNumber.cx);
		break;
	default:
		nXDrawPos=INT(nXPos);
		break;
	}
	switch(nTop)
	{
	case 0:
		nYDrawPos=INT(nYPos);
		break;
	case 1:
		nYDrawPos=INT(nYPos-SizeScoreNumber.cy/2);
		break;
	default:
		nYDrawPos=INT(nYPos-SizeScoreNumber.cy);
		break;
	}

	//�滭����
	for (LONGLONG i=0;i<lNumberCount;i++)
	{
		//�滭����
		LONGLONG lCellNumber=(LONGLONG)(lNumber%10);

		pTexNumBack->DrawImage(pD3DDevice,nXDrawPos,nYDrawPos,SizeScoreNumber.cx,SizeScoreNumber.cy,
			INT(lCellNumber*SizeScoreNumber.cx),0);
		
		//���ñ���
		lNumber/=10;
		nXDrawPos-=SizeScoreNumber.cx;
	};

	return;

}

//�滭����(nLeft=0��1��2��,nTop=0��1��2��)
void CGameClientView::DrawNumberStringWithSign(CD3DDevice * pD3DDevice,CD3DTexture* pTexNumBack, LONGLONG lNumber, INT nXPos, INT nYPos,
											   bool bSign,int nFrame,int nLeft,int nTop)
{
	if(nFrame<=0)
		return;

	CSize SizeScoreNumber(pTexNumBack->GetWidth()/nFrame,pTexNumBack->GetHeight());
	//������Ŀ
	LONGLONG lNumberCount=0;
	LONGLONG lNumberTemp=lNumber;
	do
	{
		lNumberCount++;
		lNumberTemp/=10;
	} while (lNumberTemp>0);
	if(lNumberCount<2)
		lNumberCount = 2;

	//λ�ö���
	int nMove = SizeScoreNumber.cx-2;
	INT nYDrawPos=INT(nYPos-SizeScoreNumber.cy/2);
	INT nXDrawPos=INT(nXPos+lNumberCount*nMove/2-nMove);
	switch(nLeft)
	{
	case 0:
		nXDrawPos=INT(nXPos+lNumberCount*nMove);
		break;
	case 1:
		nXDrawPos=INT(nXPos+lNumberCount*nMove/2);
		break;
	default:
		nXDrawPos=INT(nXPos);
		break;
	}
	switch(nTop)
	{
	case 0:
		nYDrawPos=INT(nYPos);
		break;
	case 1:
		nYDrawPos=INT(nYPos-SizeScoreNumber.cy/2);
		break;
	default:
		nYDrawPos=INT(nYPos-SizeScoreNumber.cy);
		break;
	}	

	//�滭����
	for (LONGLONG i=0;i<lNumberCount;i++)
	{
		//�滭����
		LONGLONG lCellNumber=(LONGLONG)(lNumber%10);
		if(bSign&&i==0)
		{
			pTexNumBack->DrawImage(pD3DDevice,nXDrawPos,nYDrawPos,SizeScoreNumber.cx,SizeScoreNumber.cy,
				INT(10*SizeScoreNumber.cx),0,pTexNumBack->GetWidth()-10*SizeScoreNumber.cx,SizeScoreNumber.cy);
			nXDrawPos-=nMove;
		}

		pTexNumBack->DrawImage(pD3DDevice,nXDrawPos,nYDrawPos,SizeScoreNumber.cx,SizeScoreNumber.cy,
			INT(lCellNumber*SizeScoreNumber.cx),0);

		//���ñ���
		lNumber/=10;
		nXDrawPos-=nMove;
	};

	return;

}

//�滭����
void CGameClientView::DrawNumberString(CD3DDevice * pD3DDevice, LONGLONG lNumber, INT nXPos, INT nYPos, bool bMeScore,bool blTimer)
{
	//������Դ
	//CImageHandle HandleScoreNumber(&m_TexScoreNumber);
	//CImageHandle HandleMeScoreNumber(&m_TexMeScoreNumber);
	CSize SizeScoreNumber(m_TexScoreNumber.GetWidth()/11,m_TexScoreNumber.GetHeight());

	if ( bMeScore ) 
		SizeScoreNumber.SetSize(m_TexMeScoreNumber.GetWidth()/11,m_TexMeScoreNumber.GetHeight());

	if(blTimer)
	{
		SizeScoreNumber.SetSize(m_TexTimerNumber.GetWidth()/10, m_TexTimerNumber.GetHeight());
	}

	//������Ŀ
	LONGLONG lNumberCount=0;
	LONGLONG lNumberTemp=lNumber;
	do
	{
		lNumberCount++;
		lNumberTemp/=10;
	} while (lNumberTemp>0);

	//λ�ö���
	INT nOffset=8;
	INT nYDrawPos=nYPos-SizeScoreNumber.cy/2;
	INT nXDrawPos=(INT)(nXPos+lNumberCount*SizeScoreNumber.cx/2+lNumberCount/4*(SizeScoreNumber.cx-nOffset)/2-SizeScoreNumber.cx);


	//�滭����
	for (LONGLONG i=0;i<lNumberCount;i++)
	{
		//�滭����
		if(i!=0&&i%3==0)
		{
			if(!blTimer)
			{
				if ( bMeScore )
				{
					m_TexMeScoreNumber.DrawImage(pD3DDevice,nXDrawPos+nOffset,nYDrawPos,SizeScoreNumber.cx-nOffset,SizeScoreNumber.cy,
						10*SizeScoreNumber.cx+2,0);
				}
				else
				{
					m_TexScoreNumber.DrawImage(pD3DDevice,nXDrawPos+nOffset,nYDrawPos,SizeScoreNumber.cx-nOffset,SizeScoreNumber.cy,
						10*SizeScoreNumber.cx+2,0);
				}
				nXDrawPos-=(SizeScoreNumber.cx-nOffset);
			}
		}
		LONG lCellNumber=(LONG)(lNumber%10);
		if(blTimer)
		{
			m_TexTimerNumber.DrawImage(pD3DDevice,nXDrawPos,nYDrawPos,SizeScoreNumber.cx,SizeScoreNumber.cy,
				lCellNumber*SizeScoreNumber.cx,0);
		}
		else
		{
			if ( bMeScore )
			{
				m_TexMeScoreNumber.DrawImage(pD3DDevice,nXDrawPos,nYDrawPos,SizeScoreNumber.cx,SizeScoreNumber.cy,
					lCellNumber*SizeScoreNumber.cx,0);
			}
			else
			{
				m_TexScoreNumber.DrawImage(pD3DDevice,nXDrawPos,nYDrawPos,SizeScoreNumber.cx,SizeScoreNumber.cy,
					lCellNumber*SizeScoreNumber.cx,0);
			}

		}
		//���ñ���
		lNumber/=10;
		nXDrawPos-=SizeScoreNumber.cx;
	};
	return;
}

//�滭����
void CGameClientView::DrawNumberStringWithSpace(CD3DDevice * pD3DDevice, LONGLONG lNumber, INT nXPos, INT nYPos)
{
	CString strNumber=TEXT(""), strTmpNumber1,strTmpNumber2;
	if (lNumber==0) strNumber=TEXT("0");
	int nNumberCount=0;
	LONGLONG lTmpNumber=lNumber;
	if (lNumber<0) lNumber=-lNumber;
	while (lNumber>0)
	{
		strTmpNumber1.Format(TEXT("%I64d"),lNumber%10);
		nNumberCount++;
		strTmpNumber2 = strTmpNumber1+strTmpNumber2;

		if (nNumberCount==3)
		{
			if(strNumber.GetLength() > 1)
				strTmpNumber2 += (TEXT(",") +strNumber);
			strNumber=strTmpNumber2;
			nNumberCount=0;
			strTmpNumber2=TEXT("");
		}
		lNumber/=10;
	}

	if (strTmpNumber2.IsEmpty()==FALSE)
	{
		if(strNumber.GetLength() > 1)
			strTmpNumber2 += (TEXT(",") +strNumber);
		strNumber=strTmpNumber2;
	}

	if (lTmpNumber<0) strNumber=TEXT("-")+strNumber;

	//CDFontEx::DrawText(this,pDC,  12, 400, strNumber, nXPos,nYPos, m_CurSelectColor, DT_END_ELLIPSIS|DT_LEFT|DT_TOP|DT_SINGLELINE);
	CRect rcText(nXPos,nYPos,nXPos+72,nYPos+12);
	DrawTextString(pD3DDevice,strNumber,rcText,DT_LEFT|DT_END_ELLIPSIS,D3DCOLOR_XRGB(255,217,180));
}

//�滭����
void CGameClientView::DrawNumberStringWithSpace(CD3DDevice * pD3DDevice, LONGLONG lNumber, CRect rcRect, DWORD dwClr, INT nFormat)
{
	CString static strNumber=TEXT(""), strTmpNumber1,strTmpNumber2;
	strTmpNumber1.Empty();
	strTmpNumber2.Empty();
	strNumber.Empty();
	if (lNumber==0) strNumber=TEXT("0");
	int nNumberCount=0;
	LONGLONG lTmpNumber=lNumber;
	if (lNumber<0) lNumber=-lNumber;
	while (lNumber>0)
	{
		strTmpNumber1.Format(TEXT("%I64d"),lNumber%10);
		nNumberCount++;
		strTmpNumber2 = strTmpNumber1+strTmpNumber2;

		if (nNumberCount==4)
		{
			if(strNumber.GetLength() > 1)
				strTmpNumber2 += (TEXT(",") +strNumber);
			strNumber=strTmpNumber2;
			nNumberCount=0;
			strTmpNumber2=TEXT("");
		}
		lNumber/=10;
	}

	if (strTmpNumber2.IsEmpty()==FALSE)
	{
		if(strNumber.GetLength() > 1)
			strTmpNumber2 += (TEXT(",") +strNumber);
		strNumber=strTmpNumber2;
	}

	if (lTmpNumber<0) strNumber=TEXT("-")+strNumber;

	//�������
	if (nFormat==-1) 
	{
		//CDFontEx::DrawText(this,pDC,  12, 400, strNumber, rcRect, m_CurSelectColor, DT_END_ELLIPSIS|DT_LEFT|DT_TOP|DT_SINGLELINE);
		DrawTextString(pD3DDevice,strNumber,rcRect,nFormat,dwClr);
	}
	else 
	{
		//CDFontEx::DrawText(this,pDC,  12, 400, strNumber, rcRect, m_CurSelectColor, nFormat);
		DrawTextString(pD3DDevice,strNumber,rcRect,nFormat,dwClr);
	}
}

void CGameClientView::DrawJettonPress(CD3DDevice * pD3DDevice,int iWidth,int iHeight,LONGLONG lTotal,LONGLONG lHaveJetton)
{
	int iWidthPng = m_TotoalPress.Tex_tatoalflag.GetWidth();
	int nXPos = (iWidth-238)/2;
	int nYPos = m_rcBack.top + 301;
	
	LONGLONG haveJetton = (LONGLONG)(((DOUBLE)lHaveJetton/(DOUBLE)lTotal)*iWidthPng);
	DrawNumberStringWithPng(pD3DDevice,&m_TexPngScoreNumber,lTotal-lHaveJetton,nXPos+72,nYPos+1,FALSE);
	m_TotoalPress.Tex_tipC.DrawImage(pD3DDevice,nXPos,nYPos,m_TotoalPress.Tex_tipC.GetWidth(),m_TotoalPress.Tex_tipC.GetHeight(),0,0,m_TotoalPress.Tex_tipC.GetWidth(),m_TotoalPress.Tex_tipC.GetHeight());
	m_TotoalPress.Tex_tatoalflag .DrawImage(pD3DDevice,nXPos+5,nYPos+26,m_TotoalPress.Tex_tatoalflag.GetWidth()-(INT)haveJetton,m_TotoalPress.Tex_tatoalflag.GetHeight(),
		0,0,m_TotoalPress.Tex_tatoalflag.GetWidth()-(INT)haveJetton,m_TotoalPress.Tex_tatoalflag.GetHeight());
	//m_TotoalPress.Tex_tipC.DrawImage(pD3DDevice,iWidth/2-138,iHeight/2-224,m_TotoalPress.Tex_tipC.GetWidth(),m_TotoalPress.Tex_tipC.GetHeight()/2,0,0,m_TotoalPress.Tex_tipC.GetWidth(),m_TotoalPress.Tex_tipC.GetHeight()/2);
	//m_TotoalPress.Tex_tatoalflag .DrawImage(pD3DDevice,iWidth/2-138+8,iHeight/2-224+20,m_TotoalPress.Tex_tatoalflag.GetWidth()-(INT)haveJetton,m_TotoalPress.Tex_tatoalflag.GetHeight()/2,0,m_TotoalPress.Tex_tatoalflag.GetHeight()/2,m_TotoalPress.Tex_tatoalflag.GetWidth()-(INT)haveJetton,m_TotoalPress.Tex_tatoalflag.GetHeight()/2);
	//m_TotoalPress.Tex_tatoalflag .DrawImage(pD3DDevice,iWidth/2-138+8+m_TotoalPress.Tex_tatoalflag.GetWidth()-(INT)haveJetton,iHeight/2-224+20,(INT)haveJetton,m_TotoalPress.Tex_tatoalflag.GetHeight()/2,m_TotoalPress.Tex_tatoalflag.GetWidth()-(INT)haveJetton,0,(INT)haveJetton,m_TotoalPress.Tex_tatoalflag.GetHeight()/2);
	//m_TotoalPress.Tex_tipC.DrawImage(pD3DDevice,iWidth/2-138,iHeight/2-224+50,m_TotoalPress.Tex_tipC.GetWidth(),m_TotoalPress.Tex_tipC.GetHeight()/2,0,m_TotoalPress.Tex_tipC.GetHeight()/2,m_TotoalPress.Tex_tipC.GetWidth(),m_TotoalPress.Tex_tipC.GetHeight()/2);
	DrawNumberStringWithPng(pD3DDevice,&m_TexPngScoreNumber1,(LONGLONG)lHaveJetton,nXPos+72,nYPos+45,FALSE);
}

//��ʱ����Ϣ
void CGameClientView::OnTimer(UINT nIDEvent)
{	
	__super::OnTimer(nIDEvent);
}

//�����Ϣ
void CGameClientView::OnLButtonDown(UINT nFlags, CPoint Point)
{
	if(m_bFlashShow)
		return;
	for (int i = 0;i<4;i++)
	{
		if(!m_CardControl[i].m_blAuto)
		{
			m_CardControl[i].OnLButtonDown(nFlags, Point);
		}
	}
		
	if (m_lCurrentJetton!=0L)
	{
		//��ע����
		BYTE cbJettonArea=GetJettonArea(Point);
	
		//�����ע
		LONGLONG lMaxJettonScore=GetUserMaxJetton();

		//�Ϸ��ж�
		if (lMaxJettonScore < m_lCurrentJetton) return ;

		//������Ϣ
		if (cbJettonArea!=0xFF) SendEngineMessage(IDM_PLACE_JETTON,cbJettonArea,0);
	}
	//��ׯ�б���
	int nApplyCount = m_ApplyUser.GetItemCount();
	if(nApplyCount>4&&PtInRect(m_rcApplyBlock,Point))
	{
		m_bApplyLBDown = true;
	}
	__super::OnLButtonDown(nFlags,Point);
}
void CGameClientView::OnLButtonUp(UINT nFlags, CPoint Point)
{
	
	for (int i = 0;i<4;i++)
	{
		if(!m_CardControl[i].m_blAuto)
		{
			m_CardControl[i].OnLButtonUp(nFlags, Point);
		}
		
	}
	//��ׯ�б�	
	CRect rcApply = CRect(m_rcBack.left+863,m_rcBack.top+181,m_rcBack.left+863+8,m_rcBack.top+181+55);
	if(m_bApplyLBDown||PtInRect(rcApply,Point))
	{
		m_bApplyLBDown = false;		
		if(Point.y<=rcApply.top)
		{
			m_rcApplyBlock.SetRect(m_rcBack.left+863,m_rcBack.top+181,m_rcBack.left+863+8,m_rcBack.top+181+12);
		}
		else if(Point.y>=rcApply.bottom-12)
		{
			m_rcApplyBlock.SetRect(m_rcBack.left+863,m_rcBack.top+181+55-12,m_rcBack.left+863+8,m_rcBack.top+181+55);
		}
		else
		{
			m_rcApplyBlock.SetRect(m_rcBack.left+863,Point.y,m_rcBack.left+863+8,Point.y+12);
		}
		int nApplyCount = m_ApplyUser.GetItemCount();
		if(nApplyCount>4)
		{
			int nApplyH = m_rcApplyBlock.top-rcApply.top+6;
			int nIndex = nApplyH/(55/(nApplyCount-3));
			if(nIndex<nApplyCount-3)
				m_nApplyFocusIndex = nIndex;
			else
				m_nApplyFocusIndex = nApplyCount-4;

		}
		else
			m_nApplyFocusIndex = 0;
	}
	__super::OnLButtonUp(nFlags,Point);
}

//���������ؼ���������Ϣ
LRESULT CGameClientView::OnViLBtUp(WPARAM wParam, LPARAM lParam)
{
	CPoint *pPoint = (CPoint*)lParam;
	ScreenToClient(pPoint);
    OnLButtonUp(1,*pPoint);
	return 1;

}
//����ƶ���Ϣ
void CGameClientView::OnMouseMove(UINT nFlags, CPoint point)
{
	//flash����ʱ����ˢ�����ˣ���Ӱ���ٶ�
	if(m_bFlashShow)
		return __super::OnMouseMove(nFlags,point);;

	//�ֶ����ƽ����ˣ�������Բ���
// 	for (int i=0; i<4; i++)
// 	{
// 		if(!m_CardControl[i].m_blAuto)
// 		{
// 			m_CardControl[i].OnMouseMove(nFlags, point);
// 		}
// 	}
	__super::OnMouseMove(nFlags,point);
}
//�����Ϣ
void CGameClientView::OnRButtonDown(UINT nFlags, CPoint Point)
{		
#ifdef _OFFLINE_
	//���ƶ���
// 	m_stuCartoonInfo[CTN_RESULT_FLASH].bShow = false;
// 	BYTE cbCardData[10]={1,2,3,4,5,6,7,8,9,10};
// 	for (int i=0; i<CountArray(m_CardControl); ++i) 
// 	{
// 		m_CardControl[i].SetCardData(0);
// 		m_CardControl[i].SetCardData(cbCardData+i*2,2);
// 	}
// 	SetMoveCardTimer();
// 	m_pGameClientEngine->SetGameStatus(GS_GAME_END);
	//FLASH����
    if(Point.x<500)
    	StartShowCardEx(0,m_ptFlash[0].x,m_ptFlash[0].y,true);
    else
    	StartShowCardEx(1,m_ptFlash[1].x,m_ptFlash[1].y,true);
	m_stuCartoonInfo[CTN_CHANGE_BANKER].nShowFrame = 0;
	m_stuCartoonInfo[CTN_CHANGE_BANKER].nShowTimer = 100;
	m_stuCartoonInfo[CTN_CHANGE_BANKER].dwShowTime = GetTickCount();
	m_stuCartoonInfo[CTN_CHANGE_BANKER].bShow = true;
	//����ͼ	
	//if(Point.x<100)
	//{
	//	m_TexViewBack.Destory();
	//	if(Point.y>100)
	//		m_TexViewBack.LoadImage(m_pD3DDevice,AfxGetInstanceHandle(),TEXT("PNG_VIEW_BACK"),TEXT("PNG"));
	//	else
	//		m_TexViewBack.LoadImage(m_pD3DDevice,AfxGetInstanceHandle(),TEXT("PNG_VIEW_BACK1"),TEXT("PNG"));	
	//}
	//��ע(1,2,3��)107,290,218,174   343,355,224,171    577,296,210,144
//  	LONGLONG llJetton[JETTON_COUNT] = {1000,10000,100000,1000000,5000000,10000000};
//  	for (int i = 0; i < 1; ++i)
//  	{
//  		PlaceUserJetton(1,llJetton[rand()%JETTON_COUNT]);
//  		PlaceUserJetton(2,llJetton[rand()%JETTON_COUNT]);
//  		PlaceUserJetton(3,llJetton[rand()%JETTON_COUNT]);
//  	}
#endif

	//���ñ���
	m_lCurrentJetton=0L;

	if (m_pGameClientEngine->GetGameStatus()!=GS_GAME_END && m_cbAreaFlash!=0xFF)
	{
		m_cbAreaFlash=0xFF;
	}
	__super::OnLButtonDown(nFlags,Point);
}

//�����Ϣ
BOOL CGameClientView::OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage)
{
	if (m_lCurrentJetton != 0L)
	{
		//��ȡ����
		CPoint MousePoint;
		GetCursorPos(&MousePoint);
		ScreenToClient(&MousePoint);
		BYTE cbJettonArea=GetJettonArea(MousePoint);

		//״̬
		if(m_pGameClientEngine && m_pGameClientEngine->GetGameStatus() != GS_PLACE_JETTON)
			cbJettonArea=0xFF;

		//���ñ���
		if ( m_cbAreaFlash != cbJettonArea )
		{
			m_cbAreaFlash = cbJettonArea;
		}

		//�����ж�
		if (cbJettonArea==0xFF)
		{
			SetCursor(LoadCursor(NULL,IDC_ARROW));
			return TRUE;
		}

		//�����ע
		LONGLONG lMaxJettonScore=GetUserMaxJetton();

		//�Ϸ��ж�
		if (lMaxJettonScore < m_lCurrentJetton)
		{
			SetCursor(LoadCursor(NULL,IDC_NO));
			return TRUE;
		}
	
		//���ù��
		switch (m_lCurrentJetton)
		{
// 		case 100:
// 			{
// 				SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_SCORE_100)));
// 				return TRUE;
// 			}
		case 1000:
			{
				SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_SCORE_1000)));
				return TRUE;
			}
		case 10000:
			{
				SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_SCORE_10000)));
				return TRUE;
			}
		case 100000:
			{
				SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_SCORE_100000)));
				return TRUE;
			}
// 		case 500000:
// 			{
// 				SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_SCORE_500000)));
// 				return TRUE;
// 			}
		case 1000000:
			{
				SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_SCORE_1000000)));
				return TRUE;
			}
		case 5000000:
			{
				SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_SCORE_5000000)));
				return TRUE;
			}
		case 10000000:
			{
				SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_SCORE_10000000)));
				return TRUE;
			}
		}
	}

	return __super::OnSetCursor(pWnd, nHitTest, uMessage);
}

//�ֻ�ׯ��
void CGameClientView::ShowChangeBanker( bool bChangeBanker )
{
	//�ֻ�ׯ��
	if ( bChangeBanker )
	{		
		m_stuCartoonInfo[CTN_CHANGE_BANKER].nShowFrame = 0;
		m_stuCartoonInfo[CTN_CHANGE_BANKER].dwShowTime = GetTickCount();
		m_stuCartoonInfo[CTN_CHANGE_BANKER].bShow = true;
		m_bShowChangeBanker = true;
	}
// 	else
// 	{
// 		m_stuCartoonInfo[CTN_CHANGE_BANKER].bShow = false;
// 		m_stuCartoonInfo[CTN_CHANGE_BANKER].nShowFrame = 0;
// 		m_bShowChangeBanker = false ;
// 	}
}

//��ׯ��ť
void CGameClientView::OnApplyBanker()
{
	SendEngineMessage(IDM_APPLY_BANKER,1,0);

}

//��ׯ��ť
void CGameClientView::OnCancelBanker()
{
	SendEngineMessage(IDM_APPLY_BANKER,0,0);
}

//��������
// void CGameClientView::DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, int nXPos, int nYPos)
// {
// 	//��������
// 	int nStringLength=lstrlen(pszString);
// 	int nXExcursion[8]={1,1,1,0,-1,-1,-1,0};
// 	int nYExcursion[8]={-1,0,1,1,1,0,-1,-1};
// 
// 	//�滭�߿�
// 	pDC->SetTextColor(crFrame);
// 	for (int i=0;i<CountArray(nXExcursion);i++)
// 	{
// 		pDC->TextOut(nXPos+nXExcursion[i],nYPos+nYExcursion[i],pszString,nStringLength);
// 	}
// 
// 	//�滭����
// 	pDC->SetTextColor(crText);
// 	pDC->TextOut(nXPos,nYPos,pszString,nStringLength);
// 
// 	return;
// }
//ׯ����Ϣ
void CGameClientView::SetBankerInfo(DWORD dwBankerUserID, LONGLONG lBankerScore) 
{
	//ׯ�����Ӻ�
	WORD wBankerUser=INVALID_CHAIR;

	//�������Ӻ�
	if (0!=dwBankerUserID)
	{
		for (WORD wChairID=0; wChairID<MAX_CHAIR; ++wChairID)
		{
			IClientUserItem *pUserData=GetClientUserItem(wChairID);
			if (NULL!=pUserData && dwBankerUserID==pUserData->GetUserID())
			{
				wBankerUser=wChairID;
				break;
			}
		}
	}

	//�л��ж�
	if (m_wBankerUser!=wBankerUser)
	{
		m_wBankerUser=wBankerUser;
		m_wBankerTime=0L;
		m_lBankerWinScore=0L;	
		m_lTmpBankerWinScore=0L;
	}
	m_lBankerScore=lBankerScore;
}

//�滭��ʶ
void CGameClientView::DrawWinFlags(CD3DDevice * pD3DDevice)
{
	//{
	//	int   bWinCount[3] = {0};
	//	bWinCount[0]= 50;
	//	int iAllrecordCount = 70;
	//	for (int n = 0;n<3;n++)
	//	{
	//	int	nYPos=m_nWinFlagsExcursionY+5+n*33;
	//	int nXPos=m_nWinFlagsExcursionX-115+10;


	//		float fCountRe = (float)bWinCount[n]/(float)iAllrecordCount;
	//		fCountRe *=100;
	//		CString str;
	//		str.Format("%0.2f",fCountRe);
	//		str+="%";

	//		pDC->TextOut(nXPos,nYPos,str);
	//	}

	//}
	
	//�ǿ��ж�
	if (m_nRecordLast==m_nRecordFirst) return;

	//��Դ����
	//CImageHandle ImageHandleWinFlags(&m_ImageWinFlags);
	int nIndex = m_nScoreHead;
	COLORREF static clrOld ;

	//clrOld = pDC->SetTextColor(RGB(255,234,0));
	int nDrawCount=0;
	while (nIndex != m_nRecordLast && ( m_nRecordLast!=m_nRecordFirst ) && nDrawCount < MAX_FALG_COUNT )
	{
		//ʤ����ʶ
		tagClientGameRecord &ClientGameRecord = m_GameRecordArrary[nIndex];
		BYTE static bWinMen[3];
		bWinMen[0]=ClientGameRecord.bWinShunMen;
		bWinMen[1]=ClientGameRecord.bWinDaoMen;
		bWinMen[2]=ClientGameRecord.bWinDuiMen;

		//�������
		enOperateResult static OperateResult[3];
		OperateResult[0]=ClientGameRecord.enOperateShunMen;
		OperateResult[1]=ClientGameRecord.enOperateDaoMen;
		OperateResult[2]=ClientGameRecord.enOperateDuiMen;

		for (int i=0; i<3; ++i)
		{
			//λ�ñ���
			int static nYPos=0,nXPos=0;			
			nYPos=m_nWinFlagsExcursionY+i*39;
			if(i==1)
				nYPos=m_nWinFlagsExcursionY+2*39;
			else if(i==2)
				nYPos=m_nWinFlagsExcursionY+1*39;
			nXPos=m_nWinFlagsExcursionX + ((nIndex - m_nScoreHead + MAX_SCORE_HISTORY) % MAX_SCORE_HISTORY) * 39;

			//ʤ����ʶ
			int static nFlagsIndex=0;
			if (WIN_FLAG_LOST==bWinMen[i])
			{
				nFlagsIndex=2;
			}
			else
			{
				nFlagsIndex=0;

			}
			if (OperateResult[i] != enOperateResult_NULL)
			{			
				 nFlagsIndex+=1;

				if(nFlagsIndex>=4)
				{
					nFlagsIndex=3;
				}

				nFlagsIndex-=1;
			}else
			{
				nFlagsIndex+=1;
			}

			if(bWinMen[i] == WIN_FLAG_EQUAL) nFlagsIndex = 4;

			//�滭��ʶ
			m_TexWinFlags.DrawImage( pD3DDevice, nXPos, nYPos, m_TexWinFlags.GetWidth()/6 , 
				m_TexWinFlags.GetHeight(),(m_TexWinFlags.GetWidth()/6 )* nFlagsIndex, 0 );
		}

		//�ƶ��±�
		nIndex = (nIndex+1) % MAX_SCORE_HISTORY;
		nDrawCount++;
	}

	nIndex = 0;

// 	int   bWinCount[3] = {0};
// 	int iAllrecordCount = 0;
// 	while ( nIndex != m_nRecordLast && ( m_nRecordLast!=m_nRecordFirst ) )
// 	{
// 		//ʤ����ʶ
// 		tagClientGameRecord &ClientGameRecord = m_GameRecordArrary[nIndex];
// 		BYTE static bWinMen[3];
// 		bWinMen[0]=ClientGameRecord.bWinShunMen;
// 		bWinMen[1]=ClientGameRecord.bWinDaoMen;
// 		bWinMen[2]=ClientGameRecord.bWinDuiMen;
// 
// 		for (int n = 0;n<3;n++)
// 		{
// 			if(bWinMen[n]==WIN_FLAG_WIN)
// 			{
// 				bWinCount[n]++;
// 
// 			}
// 		}
// 		iAllrecordCount++;
// 
// 		nIndex = (nIndex+1);
// 
// 	}
// 	for (int n = 0;n<3;n++)
// 	{
// 		int	nYPos=m_nWinFlagsExcursionY+5+n*33;
// 		int nXPos=m_nWinFlagsExcursionX-115+10;
// 		float fCountRe = (float)bWinCount[n]/(float)iAllrecordCount;
// 		fCountRe *=100;
// 		CString str;
// 		str.Format(TEXT("%0.2f"),fCountRe);
// 		str+=TEXT("%");
// 		CRect rcText(nXPos,nYPos,nXPos+75,nYPos+12);
// 		DrawTextString(pD3DDevice,str,rcText,DT_LEFT|DT_END_ELLIPSIS,D3DCOLOR_XRGB(255,217,180));
// 	}
}
//�ֹ�����
afx_msg void CGameClientView::OnOpenCard()
{
	SendEngineMessage(IDM_OPEN_CARD,0,0);

}
//�Զ�����
afx_msg void CGameClientView::OnAutoOpenCard()
{
	SendEngineMessage(IDM_AUTO_OPEN_CARD,0,0);

}
//�ƶ���ť
void CGameClientView::OnScoreMoveL()
{
	if ( m_nRecordFirst == m_nScoreHead ) return;

	m_nScoreHead = (m_nScoreHead - 1 + MAX_SCORE_HISTORY) % MAX_SCORE_HISTORY;
	if ( m_nScoreHead == m_nRecordFirst ) m_btScoreMoveL.EnableWindow(FALSE);

	m_btScoreMoveR.EnableWindow(TRUE);
}

//�ƶ���ť
void CGameClientView::OnScoreMoveR()
{
	int nHistoryCount = ( m_nRecordLast - m_nScoreHead + MAX_SCORE_HISTORY ) % MAX_SCORE_HISTORY;
	if ( nHistoryCount == MAX_FALG_COUNT ) return;

	m_nScoreHead = ( m_nScoreHead + 1 ) % MAX_SCORE_HISTORY;
	if ( nHistoryCount-1 == MAX_FALG_COUNT ) m_btScoreMoveR.EnableWindow(FALSE);

	m_btScoreMoveL.EnableWindow(TRUE);
}

//��ʾ���
void CGameClientView::ShowGameResult(CD3DDevice * pD3DDevice, int nWidth, int nHeight)
{
	//��ʾ�ж�
	if (false==m_bShowGameResult) return;
	CString strBankerCurGameScore;

	int	nXPos = nWidth / 2 - 130-2;
	int	nYPos = m_rcBack.top+282-70+70;

	CRect rcAlpha(nXPos+2, nYPos+70, nXPos+2 + m_TexGameEnd.GetWidth(), nYPos+70+m_TexGameEnd.GetHeight());
	//DrawAlphaRect(pDC, &rcAlpha, RGB(74,70,73), 0.8f);
	m_TexGameEnd.DrawImage(pD3DDevice, nXPos+2, nYPos+70);

	//pDC->SetTextColor(RGB(255,234,0));

	m_CurSelectColor = RGB(255,234,0);

	CRect rcMeWinScore, rcMeReturnScore;
	rcMeWinScore.left = nXPos+2 + 40;
	rcMeWinScore.top = nYPos+70 + 32;
	rcMeWinScore.right = rcMeWinScore.left + 111;
	rcMeWinScore.bottom = rcMeWinScore.top + 34;

	rcMeReturnScore.left = nXPos+2 + 150;
	rcMeReturnScore.top = nYPos+70 + 32;
	rcMeReturnScore.right = rcMeReturnScore.left + 111;
	rcMeReturnScore.bottom = rcMeReturnScore.top + 34;

	CString strMeGameScore, strMeReturnScore;
	DrawNumberStringWithSpace(pD3DDevice,m_lMeCurGameScore,rcMeWinScore,D3DCOLOR_XRGB(255,217,180), DT_END_ELLIPSIS | DT_VCENTER | DT_CENTER | DT_SINGLELINE);
	DrawNumberStringWithSpace(pD3DDevice,m_lMeCurGameReturnScore,rcMeReturnScore,D3DCOLOR_XRGB(255,217,180), DT_END_ELLIPSIS | DT_VCENTER | DT_CENTER | DT_SINGLELINE);

	CRect rcBankerWinScore;
	rcBankerWinScore.left = nXPos+2 + 40;
	rcBankerWinScore.top = nYPos+70 + 69;
	rcBankerWinScore.right = rcBankerWinScore.left + 111;
	rcBankerWinScore.bottom = rcBankerWinScore.top + 34;
	
	strBankerCurGameScore.Format(TEXT("%I64d"), m_lBankerCurGameScore);
	DrawNumberStringWithSpace(pD3DDevice,m_lBankerCurGameScore,rcBankerWinScore,D3DCOLOR_XRGB(255,217,180), DT_END_ELLIPSIS | DT_VCENTER | DT_CENTER | DT_SINGLELINE);
}

//͸���滭
bool CGameClientView::DrawAlphaRect(CDC* pDC, LPRECT lpRect, COLORREF clr, FLOAT fAlpha)
{
	ASSERT(pDC != 0 && lpRect != 0 && clr != CLR_NONE);
	if(pDC == 0 || lpRect == 0 || clr == CLR_NONE)
	{
		return false;
	}
	//ȫ͸��
	if( abs(fAlpha) <= 0.000001 )
	{
		return true;
	}

	for(LONG l=lpRect->top; l<lpRect->bottom; l++)
	{
		for(LONG k=lpRect->left; k<lpRect->right; k++)
		{
			COLORREF clrBk = pDC->GetPixel(k, l);
			COLORREF clrBlend = RGB(GetRValue(clrBk)*(1-fAlpha)+GetRValue(clr)*fAlpha, 
								GetGValue(clrBk)*(1-fAlpha)+GetGValue(clr)*fAlpha,
								GetBValue(clrBk)*(1-fAlpha)+GetBValue(clr)*fAlpha);
			pDC->SetPixel(k, l, clrBlend);
		}
	}

	return true;
}

//�����ע
LONGLONG CGameClientView::GetUserMaxJetton()
{
	//����ע��
	LONGLONG lNowJetton = 0;
	ASSERT(AREA_COUNT<=CountArray(m_lUserJettonScore));
	for (int nAreaIndex=1; nAreaIndex<=AREA_COUNT; ++nAreaIndex) lNowJetton += m_lUserJettonScore[nAreaIndex];

	//ׯ�ҽ��
	LONGLONG lBankerScore=2147483647;
	if (m_wBankerUser!=INVALID_CHAIR) lBankerScore=m_lBankerScore;
	for (int nAreaIndex=1; nAreaIndex<=AREA_COUNT; ++nAreaIndex) lBankerScore-=m_lAllJettonScore[nAreaIndex];


	IClientUserItem *pMeUserData = GetClientUserItem(m_wMeChairID);
	//��������
	LONGLONG lMeMaxScore,lMeLimitScore;

	lMeLimitScore = min(pMeUserData->GetUserScore(),m_lUserLimitScore);

	if(lMeLimitScore-lNowJetton>m_lAreaLimitScore)
	{
		lMeMaxScore= m_lAreaLimitScore;

	}else
	{
		lMeMaxScore = lMeLimitScore-lNowJetton;
	}
	//LONGLONG lMeMaxScore=min(m_lMeMaxScore-lNowJetton,m_lAreaLimitScore);

	//ׯ������
	lMeMaxScore=min(lMeMaxScore,lBankerScore);

	//��������
	//ASSERT(lMeMaxScore >= 0);
	lMeMaxScore = max(lMeMaxScore, 0);

	return lMeMaxScore;
}

//�ɼ�����
void CGameClientView::SetGameScore(LONGLONG lMeCurGameScore, LONGLONG lMeCurGameReturnScore, LONGLONG lBankerCurGameScore)
{
	m_lMeCurGameScore=lMeCurGameScore;
	m_lMeCurGameReturnScore=lMeCurGameReturnScore;
	m_lBankerCurGameScore=lBankerCurGameScore;	
}

//�滭����
void CGameClientView::DrawMeJettonNumber(CD3DDevice * pD3DDevice)
{
	//�滭����
	for (int nAreaIndex=1; nAreaIndex<=AREA_COUNT; ++nAreaIndex)
	{
		if (m_lUserJettonScore[nAreaIndex] > 0 )
		{
			m_TexScoreBack.DrawImage(pD3DDevice,m_PointJettonNumber[nAreaIndex-1].x-102,m_PointJettonNumber[nAreaIndex-1].y+40-17);
			DrawNumberString(pD3DDevice,m_lUserJettonScore[nAreaIndex],m_PointJettonNumber[nAreaIndex-1].x,m_PointJettonNumber[nAreaIndex-1].y+40, true);
		}
	}
}

//��ʼ����
void CGameClientView::DispatchCard()
{
	//���ý���
	for (int i=0; i<CountArray(m_CardControl); ++i) m_CardControl[i].m_CardItemArray.SetSize(0);
	//���ñ�ʶ
	m_bNeedSetGameRecord=true;
}

//��������
void CGameClientView::FinishDispatchCard()
{
	//����ж�
	if (m_bNeedSetGameRecord==false) return;

	//���ñ�ʶ
	m_bNeedSetGameRecord=false;

	//ɾ����ʱ��
	m_stuCartoonInfo[CTN_DISPATCH_CARD].bShow = false;
	m_stuCartoonInfo[CTN_DISPATCH_CARD].nShowFrame = 0;

	////�����˿�
	//for (int i=0; i<CountArray(m_CardControl); ++i) m_CardControl[i].SetCardData(m_cbTableCardArray[i],2);

	//�ƶ�Ӯ��
	BYTE bWinShunMen, bWinDuiMen, bWinDaoMen;
	DeduceWinner(bWinShunMen, bWinDuiMen, bWinDaoMen);	
	m_bToBankerMax = false;

	//�����¼
	SetGameHistory(bWinShunMen, bWinDaoMen, bWinDuiMen);

	//�ۼƻ���
	m_lMeStatisticScore+=m_lMeCurGameScore;
	m_lBankerWinScore=m_lTmpBankerWinScore;

	//����Ӯ��
	SetWinnerSide(bWinShunMen, bWinDuiMen, bWinDaoMen, true);
	//������Ϣ
	int nStarIdx = 0;
	m_llAllStarCount++;
	for (int i = 0; i < 3; ++i)
	{
		nStarIdx = m_cbStarSelect[i];
		m_llStarAllCount[nStarIdx]++;
		if(i==0&&bWinShunMen)
			m_llStarWinCount[nStarIdx]++;
		else if(i==1&&bWinDuiMen)
			m_llStarWinCount[nStarIdx]++;
		else if(i==2&&bWinDaoMen)
			m_llStarWinCount[nStarIdx]++;
		//m_fStarWinRate[nStarIdx] = (float)((float)m_llStarWinCount[nStarIdx]/(float)m_llStarAllCount[nStarIdx]);
	}
	for (int i = 0; i < STAR_COUNT; ++i)
	{
		m_fStarWinRate[i] = (float)((float)m_llStarWinCount[i]/(float)m_llAllStarCount);
	}

	//��������
	if (m_lMeCurGameScore>0) m_pGameClientEngine->PlayGameSound(AfxGetInstanceHandle(),TEXT("END_WIN"));
	else if (m_lMeCurGameScore<0) m_pGameClientEngine->PlayGameSound(AfxGetInstanceHandle(),TEXT("END_LOST"));
	else m_pGameClientEngine->PlayGameSound(AfxGetInstanceHandle(),TEXT("END_DRAW"));
}

//ʤ���߿�
void CGameClientView::FlashJettonAreaFrame(int nWidth, int nHeight, CD3DDevice * pD3DDevice)
{	
	//�Ϸ��ж�
	if (m_cbAreaFlash==0xFF && false==m_stuCartoonInfo[CTN_RESULT_FLASH].bShow) return;

		
	//��ע�ж�
	if (false==m_stuCartoonInfo[CTN_RESULT_FLASH].bShow)
	{
		switch (m_cbAreaFlash)
		{
		case ID_SHUN_MEN:	
			{
				//�滭ͼƬ
				//CImageHandle ImageHandle(&m_ImageFrameShunMen);
				m_TexFocusShunMen.DrawImage(pD3DDevice,m_rcShunMenBack[0].left+4,m_rcShunMenBack[0].top+2);
				break;
			}
		case ID_DUI_MEN:
			{
				//�滭ͼƬ
				//CImageHandle ImageHandle(&m_ImageFrameDuiMen);	
				m_TexFocusDuiMen.DrawImage(pD3DDevice,m_rcShunMenBack[1].left+4,m_rcShunMenBack[1].top+4);	
				break;
			}
		case ID_DAO_MEN:
			{
				//�滭ͼƬ
				//CImageHandle ImageHandle(&m_ImageFrameDaoMen);	
				m_TexFocusDaoMen.DrawImage(pD3DDevice,m_rcShunMenBack[2].left+4,m_rcShunMenBack[2].top+4);
				break;
			}
		}
	}
	else if(m_stuCartoonInfo[CTN_RESULT_FLASH].nShowFrame>0)
	{
		//ׯȫʤ�ж�
		if (WIN_FLAG_LOST==m_bWinDaoMen && WIN_FLAG_LOST==m_bWinShunMen && WIN_FLAG_LOST==m_bWinDuiMen)
		{
			//�滭ͼƬ
			m_TexFrameShunMen.DrawImage(pD3DDevice,m_rcShunMenBack[0].left,m_rcShunMenBack[0].top,m_TexFrameShunMen.GetWidth()/2,m_TexFrameShunMen.GetHeight(),
				m_TexFrameShunMen.GetWidth()/2,0,m_TexFrameShunMen.GetWidth()/2,m_TexFrameShunMen.GetHeight());
			m_TexFrameDuiMen.DrawImage(pD3DDevice,m_rcShunMenBack[1].left,m_rcShunMenBack[1].top,m_TexFrameDuiMen.GetWidth()/2,m_TexFrameDuiMen.GetHeight(),
				m_TexFrameDuiMen.GetWidth()/2,0,m_TexFrameDuiMen.GetWidth()/2,m_TexFrameDuiMen.GetHeight());
			m_TexFrameDaoMen.DrawImage(pD3DDevice,m_rcShunMenBack[2].left,m_rcShunMenBack[2].top,m_TexFrameDaoMen.GetWidth()/2,m_TexFrameDaoMen.GetHeight(),
				m_TexFrameDaoMen.GetWidth()/2,0,m_TexFrameDaoMen.GetWidth()/2,m_TexFrameDaoMen.GetHeight());

		}
		else
		{
			bool bShowRed = true;

			//���ж�
			if (WIN_FLAG_WIN==m_bWinDaoMen)
			{
				bShowRed = false;
				m_TexFrameDaoMen.DrawImage(pD3DDevice,m_rcShunMenBack[2].left,m_rcShunMenBack[2].top,m_TexFrameDaoMen.GetWidth()/2,m_TexFrameDaoMen.GetHeight(),
					0,0,m_TexFrameDaoMen.GetWidth()/2,m_TexFrameDaoMen.GetHeight());
			}
			if (WIN_FLAG_WIN==m_bWinShunMen)
			{
				bShowRed = false;
				m_TexFrameShunMen.DrawImage(pD3DDevice,m_rcShunMenBack[0].left,m_rcShunMenBack[0].top,m_TexFrameShunMen.GetWidth()/2,m_TexFrameShunMen.GetHeight(),
					0,0,m_TexFrameShunMen.GetWidth()/2,m_TexFrameShunMen.GetHeight());
			}
			if (WIN_FLAG_WIN==m_bWinDuiMen)
			{
				bShowRed = false;
				m_TexFrameDuiMen.DrawImage(pD3DDevice,m_rcShunMenBack[1].left,m_rcShunMenBack[1].top,m_TexFrameDuiMen.GetWidth()/2,m_TexFrameDuiMen.GetHeight(),
					0,0,m_TexFrameDuiMen.GetWidth()/2,m_TexFrameDuiMen.GetHeight());
			}
 
		}
	}
}

void CGameClientView::DrawGameTimer(CD3DDevice * pD3DDevice,int iWidth,int iHeight,int iTimer)
{

	int ix = iWidth+51;
	int iy = iHeight+25;
// 	if(iTimer<100)
// 	{
// 		ix+=3;
// 	}
// 	if(iTimer<10)
// 	{
// 		ix+=1;
// 	}


	//m_TexTimer.DrawImage(pD3DDevice,iWidth,iHeight,m_TexTimer.GetWidth(),m_TexTimer.GetHeight(),0,0,m_TexTimer.GetWidth(),m_TexTimer.GetHeight());
		DrawNumberString(pD3DDevice,iTimer,ix,iy,false,true);
}
//�ƶ�Ӯ��
void CGameClientView::DeduceWinner(BYTE &bWinShunMen, BYTE &bWinDuiMen, BYTE &bWinDaoMen)
{
	//����
	BYTE bAreaPoint[7] = {0};
	memset(bAreaPoint, 0, sizeof(bAreaPoint));
	BYTE bBankerPoint = m_GameLogic.GetCardListPip(m_cbTableCardArray[BANKER_INDEX],2);
	bAreaPoint[ID_SHUN_MEN] = m_GameLogic.GetCardListPip(m_cbTableCardArray[SHUN_MEN_INDEX],2);
	bAreaPoint[ID_DUI_MEN] = m_GameLogic.GetCardListPip(m_cbTableCardArray[DUI_MEN_INDEX],2);
	bAreaPoint[ID_DAO_MEN] = m_GameLogic.GetCardListPip(m_cbTableCardArray[DAO_MEN_INDEX],2);

	//��С�Ƚ�
	bool static bWinShunMen1, bWinDuiMen1, bWinDaoMen1;
	int nComShunMen = m_GameLogic.CompareCard(m_cbTableCardArray[BANKER_INDEX],2,m_cbTableCardArray[SHUN_MEN_INDEX],2);
	int nComDuiMen = m_GameLogic.CompareCard(m_cbTableCardArray[BANKER_INDEX],2,m_cbTableCardArray[DUI_MEN_INDEX],2);
	int nComDaoMen = m_GameLogic.CompareCard(m_cbTableCardArray[BANKER_INDEX],2,m_cbTableCardArray[DAO_MEN_INDEX],2);
	bWinShunMen1 = (nComShunMen==1)?true:false;
	bWinDuiMen1 = (nComDuiMen==1)?true:false;
	bWinDaoMen1 = (nComDaoMen==1)?true:false;

	//˳��
	if(bAreaPoint[ID_SHUN_MEN]==bBankerPoint&&m_bToBankerMax) bWinShunMen = WIN_FLAG_EQUAL;
	else bWinShunMen = bWinShunMen1?WIN_FLAG_WIN:WIN_FLAG_LOST;
	//����
	if(bAreaPoint[ID_DUI_MEN] == bBankerPoint&&m_bToBankerMax)bWinDuiMen = WIN_FLAG_EQUAL;
	else bWinDuiMen = bWinDuiMen1?WIN_FLAG_WIN:WIN_FLAG_LOST;
	//����
	if(bAreaPoint[ID_DAO_MEN] == bBankerPoint&&m_bToBankerMax) bWinDaoMen = WIN_FLAG_EQUAL;
	else bWinDaoMen = bWinDaoMen1?WIN_FLAG_WIN:WIN_FLAG_LOST;

	return;
}

//�ؼ�����
BOOL CGameClientView::OnCommand(WPARAM wParam, LPARAM lParam)
{
	//��ȡID
	WORD wControlID=LOWORD(wParam);

	//�ؼ��ж�
	switch (wControlID)
	{
	case IDC_JETTON_BUTTON_100:
		{
			//���ñ���
			m_lCurrentJetton=100L;
			break;
		}
	case IDC_JETTON_BUTTON_1000:
		{
			//���ñ���
			m_lCurrentJetton=1000L;
			break;
		}
	case IDC_JETTON_BUTTON_10000:
		{
			//���ñ���
			m_lCurrentJetton=10000L;
			break;
		}
	case IDC_JETTON_BUTTON_100000:
		{
			//���ñ���
			m_lCurrentJetton=100000L;
			break;
		}
// 	case IDC_JETTON_BUTTON_500000:
// 		{
// 			//���ñ���
// 			m_lCurrentJetton=500000L;
// 			break;
// 		}
	case IDC_JETTON_BUTTON_1000000:
		{
			//���ñ���
			m_lCurrentJetton=1000000L;
			break;
		}
	case IDC_JETTON_BUTTON_5000000:
		{
			//���ñ���
			m_lCurrentJetton=5000000L;
			break;
		}
	case IDC_JETTON_BUTTON_10000000:
		{
			//���ñ���
			m_lCurrentJetton=10000000L;
			break;
		}
	case IDC_AUTO_OPEN_CARD:
		{
			break;
		}
	case IDC_OPEN_CARD:
		{
			break;
		}
	case IDC_BANK:
		{
			break;
		}
	case IDC_CONTINUE_CARD:
		{
			SendEngineMessage(IDM_CONTINUE_CARD,0,0);
			break;
		}
	}

	return CGameFrameView::OnCommand(wParam, lParam);
}

//�ҵ�λ��
void CGameClientView::SetMeChairID(DWORD dwMeUserID)
{
	//�������Ӻ�
	for (WORD wChairID=0; wChairID<MAX_CHAIR; ++wChairID)
	{
		IClientUserItem *pUserData=GetClientUserItem(wChairID);
		if (NULL!=pUserData && dwMeUserID==pUserData->GetUserID())
		{
			m_wMeChairID=wChairID;
			break;
		}
	}
}

//������ʾ
void CGameClientView::SetDispatchCardTip(enDispatchCardTip DispatchCardTip)
{
	//���ñ���
	m_enDispatchCardTip=DispatchCardTip;

	//���ö�ʱ��
	if (enDispatchCardTip_NULL!=DispatchCardTip)
	{
		m_stuCartoonInfo[CTN_DISPATCH_TIP].nShowFrame = 0;
		m_stuCartoonInfo[CTN_DISPATCH_TIP].dwShowTime = GetTickCount();
		m_stuCartoonInfo[CTN_DISPATCH_TIP].bShow = true;
	}
	else 
	{
		m_stuCartoonInfo[CTN_DISPATCH_TIP].bShow = false;
		m_stuCartoonInfo[CTN_DISPATCH_TIP].nShowFrame = 0;
	}
}

//�滭ׯ��
void CGameClientView::DrawBankerInfo(CD3DDevice * pD3DDevice,int nWidth,int nHeight)
{
	//ׯ����Ϣ																											
	//pDC->SetTextColor(RGB(255,234,0));

	m_CurSelectColor = RGB(255,234,0);

	//��ȡ���
	IClientUserItem  *pUserData = m_wBankerUser==INVALID_CHAIR ? NULL : GetClientUserItem(m_wBankerUser);

	//λ����Ϣ
	CRect static StrRect;
	StrRect.left = m_rcBack.left+82;
	StrRect.top = m_rcBack.top+160;
	StrRect.right = StrRect.left + 85;
	StrRect.bottom = StrRect.top + 15;

	//ׯ������
	//pDC->DrawText(pUserData==NULL?(m_bEnableSysBanker?TEXT("ϵͳ��ׯ"):TEXT("������ׯ")):pUserData->GetNickName(), StrRect, DT_END_ELLIPSIS | DT_LEFT | DT_TOP| DT_SINGLELINE );

	
	//CDFontEx::DrawText(this,pDC,  12, 400, 
	//	pUserData==NULL?(m_bEnableSysBanker?TEXT("ϵͳ��ׯ"):TEXT("������ׯ")):	pUserData->GetNickName(), 
	//	StrRect, m_CurSelectColor, DT_END_ELLIPSIS|DT_LEFT|DT_TOP|DT_SINGLELINE);
	DrawTextString(pD3DDevice,pUserData==NULL?(m_bEnableSysBanker?TEXT("ϵͳ��ׯ"):TEXT("������ׯ")):	pUserData->GetNickName(),
		StrRect,DT_LEFT|DT_END_ELLIPSIS,D3DCOLOR_XRGB(255,217,180));
	

	//ׯ���ܷ�
	StrRect.left = m_rcBack.left+82;
	StrRect.top = m_rcBack.top+160+18;
	StrRect.right = StrRect.left + 190;
	StrRect.bottom = StrRect.top + 15;
	DrawNumberStringWithSpace(pD3DDevice,pUserData==NULL?0:pUserData->GetUserScore(), StrRect,D3DCOLOR_XRGB(255,217,180),DT_LEFT|DT_TOP);
	TCHAR szUserScore[64]=TEXT("");

	//ׯ�ҳɼ�
	StrRect.left = m_rcBack.left+82;
	StrRect.top = m_rcBack.top+160+18*2;
	StrRect.right = StrRect.left + 190;
	StrRect.bottom = StrRect.top + 15;
	DrawNumberStringWithSpace(pD3DDevice,m_lBankerWinScore,StrRect,D3DCOLOR_XRGB(255,217,180),DT_LEFT|DT_TOP);
	//ׯ�Ҿ���
	StrRect.left = m_rcBack.left+82;
	StrRect.top = m_rcBack.top+160+18*3;
	StrRect.right = StrRect.left + 190;
	StrRect.bottom = StrRect.top + 15;
	DrawNumberStringWithSpace(pD3DDevice,m_wBankerTime ,StrRect,D3DCOLOR_XRGB(255,217,180),DT_LEFT|DT_TOP);

	
}
void CGameClientView::SetFirstShowCard(BYTE bcCard)
{
	m_bcfirstShowCard =  bcCard;
	m_CardControl[4].SetCardData(NULL,0);
	m_CardControl[4].SetCardData(&bcCard,1);
	m_CardControl[4].m_blGameEnd = true;
	m_CardControl[4].SetDisplayFlag(true);
	m_CardControl[4].ShowCardControl(true);
}
//�滭���
void CGameClientView::DrawMeInfo(CD3DDevice * pD3DDevice,int nWidth,int nHeight)
{
	//�Ϸ��ж�
	if (INVALID_CHAIR==m_wMeChairID) return;
	TCHAR szUserScore[64]=TEXT("");

	IClientUserItem *pMeUserData = GetClientUserItem(m_wMeChairID);
	if ( pMeUserData != NULL )
	{
		//������ɫ																									
		//pDC->SetTextColor(RGB(255,234,0));

		m_CurSelectColor = RGB(255,234,0);
		//�ܹ���ע
		LONGLONG lMeJetton=0L;
		for (int nAreaIndex=1; nAreaIndex<=AREA_COUNT; ++nAreaIndex) lMeJetton += m_lUserJettonScore[nAreaIndex];

		//����λ��
		CRect static rcDraw;
		rcDraw.left=m_rcBack.left+97;
		rcDraw.top=m_rcBack.top+628;
		rcDraw.right=rcDraw.left+85;
		rcDraw.bottom=rcDraw.top+15;

		//�滭�ʺ�
		//pDC->DrawText(pMeUserData->GetNickName(),lstrlen(pMeUserData->GetNickName()),rcDraw,DT_VCENTER|DT_SINGLELINE|DT_LEFT|DT_END_ELLIPSIS);

		//CDFontEx::DrawText(this,pDC,  12, 400, pMeUserData->GetNickName(), rcDraw, m_CurSelectColor, DT_VCENTER|DT_SINGLELINE|DT_LEFT|DT_END_ELLIPSIS);
		DrawTextString(pD3DDevice,pMeUserData->GetNickName(),
			rcDraw,DT_LEFT|DT_END_ELLIPSIS,D3DCOLOR_XRGB(242,238,201));

		//�滭����
		rcDraw.left=m_rcBack.left+97;
		rcDraw.top=m_rcBack.top+628+24;
		rcDraw.right=rcDraw.left+85;
		rcDraw.bottom=rcDraw.top+15;
		DrawNumberStringWithSpace(pD3DDevice,pMeUserData->GetUserScore()-lMeJetton,rcDraw,D3DCOLOR_XRGB(242,238,201),DT_VCENTER|DT_SINGLELINE|DT_LEFT|DT_END_ELLIPSIS);

		//�滭�ɼ�
		rcDraw.left=m_rcBack.left+232;
		rcDraw.top=m_rcBack.top+628;
		rcDraw.right=rcDraw.left+85;
		rcDraw.bottom=rcDraw.top+15;
		DrawNumberStringWithSpace(pD3DDevice,m_lMeStatisticScore,rcDraw,D3DCOLOR_XRGB(242,238,201),DT_VCENTER|DT_SINGLELINE|DT_LEFT|DT_END_ELLIPSIS);

		//�滭�ɼ�
		rcDraw.left=m_rcBack.left+232;
		rcDraw.top=m_rcBack.top+628+24;
		rcDraw.right=rcDraw.left+85;
		rcDraw.bottom=rcDraw.top+15;
		DrawNumberStringWithSpace(pD3DDevice,lMeJetton,rcDraw,D3DCOLOR_XRGB(242,238,201),DT_VCENTER|DT_SINGLELINE|DT_LEFT|DT_END_ELLIPSIS);
	}
}

//////////////////////////////////////////////////////////////////////////
//���д��
void CGameClientView::OnBankStorage()
{
#ifdef __SPECIAL___
	//��ȡ�ӿ�
	CGameClientEngine *pGameClientEngine=CONTAINING_RECORD(this,CGameClientEngine,m_GameClientView);
	IClientKernel *pIClientKernel=(IClientKernel *)pGameClientEngine->m_pIClientKernel;

	if(NULL!=pIClientKernel)
	{
		CRect btRect;
		m_btBankerStorage.GetWindowRect(btRect);
		ShowInsureSave(pIClientKernel,CPoint(btRect.right,btRect.top));
	}
#endif
}

//����ȡ��
void CGameClientView::OnBankDraw()
{
//#ifdef __SPECIAL___
	//��ȡ�ӿ�
	CGameClientEngine *pGameClientEngine=CONTAINING_RECORD(this,CGameClientEngine,m_GameClientView);
	IClientKernel *pIClientKernel=(IClientKernel *)pGameClientEngine->m_pIClientKernel;

	if(NULL!=pIClientKernel)
	{
		CRect btRect;
		m_btBankerDraw.GetWindowRect(btRect);
		ShowInsureGet(pIClientKernel,CPoint(btRect.right,btRect.top));
	}
//#endif
}
//��Ӯ����
void CGameClientView::OnControl()
{
	SendEngineMessage(IDM_MANAGE_CONTROL,0,0);
}
void CGameClientView::ClearBank()
{
	//m_DlgBank.ShowWindow(SW_HIDE);
}

void CGameClientView::OpenAdminWnd()
{
	//����д�Ȩ��
	if(m_pClientControlDlg != NULL )
	{
		if(!m_pClientControlDlg->IsWindowVisible()) 
			m_pClientControlDlg->ShowWindow(SW_SHOW);
		else 
			m_pClientControlDlg->ShowWindow(SW_HIDE);
	}
}

//��ʼ��ť
//void CGameClientView::OnStart()
//{
//	SendEngineMessage(IDM_START,0,0);
//	return;
//}


//������ע���
void CGameClientView::ControlBetDetection( BYTE cbViewIndex, LONGLONG lScoreCount, WORD wChair )
{
	if (m_pClientControlDlg == NULL || wChair >= GAME_PLAYER )
		return;

	//��ȡ���
	m_lAllPlayBet[wChair][cbViewIndex] += lScoreCount;

	//�����Ϣ
	IClientUserItem* pUserData = GetClientUserItem(wChair);
	int nListIndex = 0;
	if ( cbViewIndex == ID_SHUN_MEN )
		nListIndex = 0;
	else if ( cbViewIndex == ID_DUI_MEN )
		nListIndex = 1;
	else if ( cbViewIndex == ID_DAO_MEN )
		nListIndex = 2;
	else
		ASSERT(FALSE);

	//�������
	LVFINDINFO lvFindInfo;
	ZeroMemory( &lvFindInfo, sizeof( lvFindInfo ) );
	lvFindInfo.flags = LVFI_STRING;
	lvFindInfo.psz = (LPCTSTR)pUserData->GetNickName();

	//��������
	int nItem = m_SkinListCtrl[nListIndex].FindItem( &lvFindInfo );
	if ( nItem == -1 )
	{
		nItem = m_SkinListCtrl[nListIndex].InsertItem(m_SkinListCtrl[nListIndex].GetItemCount(), pUserData->GetNickName());//������
	}


	CString strScore;
	strScore.Format(TEXT("%I64d"), m_lAllPlayBet[wChair][cbViewIndex]);
	m_SkinListCtrl[nListIndex].SetItemText(nItem, 1, strScore);		//��������
}

//////////////////////////////////////////////////////////////////////////
bool CGameClientView::GetCloseFrame(TCHAR szInfo[])
{
	TCHAR szMsg[256] = {0};

	//�ܹ���ע
	LONGLONG lMeJetton=0L;
	for (int nAreaIndex=1; nAreaIndex<=AREA_COUNT; ++nAreaIndex) lMeJetton += m_lUserJettonScore[nAreaIndex];
	if(m_wMeChairID==m_wBankerUser)
	{
		_sntprintf(szMsg,sizeof(szMsg),TEXT("��ȴ����ֽ����󡾿���ʱ�䡿����뿪")) ;
		CopyMemory(szInfo,szMsg,sizeof(szMsg));
		return true;
	}
	if(lMeJetton >0)
	{
		_sntprintf(szMsg,sizeof(szMsg),TEXT("��������Ѻע����ȴ����ֽ����󡾿���ʱ�䡿����뿪")) ;
		CopyMemory(szInfo,szMsg,sizeof(szMsg));
		return true;
	}
	else
	{
		_sntprintf(szMsg,sizeof(szMsg),TEXT("��Ϸ���ڽ��У���ȷ��Ҫ�뿪��")) ;
		CopyMemory(szInfo,szMsg,sizeof(szMsg));
	}
	
	return false;
}

void CGameClientView::StartShowCard(WORD wFalshID,int nXPos,int nYPos)
{
	m_stuCartoonInfo[CTN_FLASH_EFFECT].bShow = false;
	m_stuCartoonInfo[CTN_FLASH_WAIT].bShow = false;	
 	m_bFlashShow = false;
 	if (m_ShowLotteryFlash.GetSafeHwnd())
 	{
		m_ShowLotteryFlash.ShowWindow(SW_HIDE);
 		m_ShowLotteryFlash.StopFlash(); 		
 		m_ShowLotteryFlash.OnCloseFalsh();
	}
	m_wFlashId = wFalshID;
	//time_t t;
	//srand((unsigned) time(&t)); 
	m_ShowLotteryFlash.m_nDesicType = wFalshID;
	//m_ShowLotteryFlash.ShowWindow(SW_SHOW);


	//m_bTimeNum = 0;


	CPoint  cPoint;
	cPoint.x = nXPos;
	cPoint.y = nYPos;
	this->ClientToScreen(&cPoint);

	///ϴ�ƶ���
	if (wFalshID == 0)
	{
		//PlayGameSound(AfxGetInstanceHandle(),TEXT("GODIN"));
		m_ShowLotteryFlash.ShowFlash(this, cPoint.x-SHOWLOTTERYFLASH_W/2, cPoint.y,true);
	} 
	else
	{
		//PlayGameSound(AfxGetInstanceHandle(),TEXT("GODSHOW"));
		m_ShowLotteryFlash.ShowFlash(this, cPoint.x-SHOWLOTTERYFLASH1_W/2, cPoint.y,true);
	}
	m_stuCartoonInfo[CTN_FLASH_WAIT].nShowFrame = 0;
	m_stuCartoonInfo[CTN_FLASH_WAIT].dwShowTime = GetTickCount();
	m_stuCartoonInfo[CTN_FLASH_WAIT].bShow = true;
	m_stuCartoonInfo[CTN_FLASH_EFFECT].nShowFrame = 0;
	m_stuCartoonInfo[CTN_FLASH_EFFECT].dwShowTime = GetTickCount();
	m_stuCartoonInfo[CTN_FLASH_EFFECT].bShow = true;
	m_bFlashShow = true;
}

void CGameClientView::StartShowCardEx(WORD wFalshID,int nXPos,int nYPos,bool bShow)
{
	m_stuCartoonInfo[CTN_FLASH_EFFECT].bShow = false;
	m_stuCartoonInfo[CTN_FLASH_WAIT].bShow = false;	
	m_bFlashShow = false;
	m_wFlashId = wFalshID;
	if (m_ShowLotteryFlashArray[wFalshID].GetSafeHwnd())
	{
		m_ShowLotteryFlashArray[(wFalshID+1)%2].ShowWindow(SW_HIDE);
		if(bShow)
		{			
			bool bWindowMin = GetParent()->IsIconic();
			if(bWindowMin==false)
			{
				MoveFlashEx(wFalshID);
				m_ShowLotteryFlashArray[wFalshID].ShowWindow(SW_SHOW);
			}
		}
	}
	else
	{
		m_ShowLotteryFlashArray[wFalshID].m_nDesicType = wFalshID;
		CPoint  cPoint;
		cPoint.x = nXPos;
		cPoint.y = nYPos;
		this->ClientToScreen(&cPoint);
		///FLASH����
		if (wFalshID == 0)
		{
			m_ShowLotteryFlashArray[wFalshID].ShowFlash(this, cPoint.x-SHOWLOTTERYFLASH_W/2, cPoint.y,bShow);
		} 
		else
		{
			m_ShowLotteryFlashArray[wFalshID].ShowFlash(this, cPoint.x-SHOWLOTTERYFLASH1_W/2, cPoint.y,bShow);
		}
	}	
	if(bShow)
	{
		m_stuCartoonInfo[CTN_FLASH_WAIT].nShowFrame = 0;
		m_stuCartoonInfo[CTN_FLASH_WAIT].dwShowTime = GetTickCount();
		m_stuCartoonInfo[CTN_FLASH_WAIT].bShow = true;
		m_stuCartoonInfo[CTN_FLASH_EFFECT].nShowFrame = 0;
		m_stuCartoonInfo[CTN_FLASH_EFFECT].dwShowTime = GetTickCount();
		m_stuCartoonInfo[CTN_FLASH_EFFECT].bShow = true;
		if (wFalshID == 0)
		{
			PlayGameSound(AfxGetInstanceHandle(),TEXT("GODIN"));
		} 
		else
		{
			PlayGameSound(AfxGetInstanceHandle(),TEXT("GODSHOW"));
		}
		m_bFlashShow = true;
	}
}

void CGameClientView::MoveFlash(WORD wFalshID)
{
	CPoint  cPoint;	
	if (wFalshID == 0)
	{
		cPoint.x = m_ptFlash[0].x;
		cPoint.y = m_ptFlash[0].y;
		this->ClientToScreen(&cPoint);
		m_ShowLotteryFlash.MoveFlash(this, cPoint.x-SHOWLOTTERYFLASH_W/2, cPoint.y);
	} 
	else if (wFalshID == 1)
	{
		cPoint.x = m_ptFlash[1].x;
		cPoint.y = m_ptFlash[1].y;
		this->ClientToScreen(&cPoint);
		m_ShowLotteryFlash.MoveFlash(this, cPoint.x-SHOWLOTTERYFLASH1_W/2, cPoint.y);
	}
}

void CGameClientView::MoveFlashEx(WORD wFalshID)
{
	CPoint  cPoint;	
	if (wFalshID == 0)
	{
		cPoint.x = m_ptFlash[0].x;
		cPoint.y = m_ptFlash[0].y;
		this->ClientToScreen(&cPoint);
		m_ShowLotteryFlashArray[0].MoveFlash(this, cPoint.x-SHOWLOTTERYFLASH_W/2, cPoint.y);
	} 
	else if (wFalshID == 1)
	{
		cPoint.x = m_ptFlash[1].x;
		cPoint.y = m_ptFlash[1].y;
		this->ClientToScreen(&cPoint);
		m_ShowLotteryFlashArray[1].MoveFlash(this, cPoint.x-SHOWLOTTERYFLASH1_W/2, cPoint.y);
	}
	else
	{
		cPoint.x = m_ptFlash[0].x;
		cPoint.y = m_ptFlash[0].y;
		this->ClientToScreen(&cPoint);
		m_ShowLotteryFlashArray[0].MoveFlash(this, cPoint.x-SHOWLOTTERYFLASH_W/2, cPoint.y);
		cPoint.x = m_ptFlash[1].x;
		cPoint.y = m_ptFlash[1].y;
		this->ClientToScreen(&cPoint);
		m_ShowLotteryFlashArray[1].MoveFlash(this, cPoint.x-SHOWLOTTERYFLASH1_W/2, cPoint.y);
	}
}

LRESULT CGameClientView::OnPlayFlashEnd(WPARAM wParam, LPARAM lParam)
{		
	m_bFlashShow = false;
	m_stuCartoonInfo[CTN_FLASH_WAIT].bShow = false;
	m_stuCartoonInfo[CTN_FLASH_WAIT].nShowFrame = 0;
	m_stuCartoonInfo[CTN_FLASH_EFFECT].bShow = false;
	m_stuCartoonInfo[CTN_FLASH_EFFECT].nShowFrame = 0;
	///����FLASH
	if (m_ShowLotteryFlash.GetSafeHwnd())
	{
		m_ShowLotteryFlash.StopFlash();
		m_ShowLotteryFlash.ShowWindow(SW_HIDE);
		m_ShowLotteryFlash.OnCloseFalsh();
	}	
	return S_OK;
}

LRESULT CGameClientView::OnPlayFlashEndEx(WPARAM wParam, LPARAM lParam)
{	
	if(m_wFlashId!=0&&m_wFlashId!=1)
		return 0;
	m_bFlashShow = false;
	m_stuCartoonInfo[CTN_FLASH_WAIT].bShow = false;
	m_stuCartoonInfo[CTN_FLASH_WAIT].nShowFrame = 0;
	m_stuCartoonInfo[CTN_FLASH_EFFECT].bShow = false;
	m_stuCartoonInfo[CTN_FLASH_EFFECT].nShowFrame = 0;
	///����FLASH
	if (m_ShowLotteryFlashArray[m_wFlashId].GetSafeHwnd())
	{
		m_ShowLotteryFlashArray[m_wFlashId].StopFlash();
		m_ShowLotteryFlashArray[m_wFlashId].ShowWindow(SW_HIDE);
	}	
	return S_OK;
}

//�л���ͼ
void CGameClientView::TransportBackRes(int nRedBackRes)
{
// 	m_TexViewBack.Destory();
// 	if(nRedBackRes==1)
// 		m_TexViewBack.LoadImage(m_pD3DDevice,AfxGetInstanceHandle(),TEXT("PNG_VIEW_BACK"),TEXT("PNG"));
// 	else
// 		m_TexViewBack.LoadImage(m_pD3DDevice,AfxGetInstanceHandle(),TEXT("PNG_VIEW_BACK1"),TEXT("PNG"));
}