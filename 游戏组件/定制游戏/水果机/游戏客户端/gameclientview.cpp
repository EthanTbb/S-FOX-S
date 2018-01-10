#include "StdAfx.h"
#include "Math.h"
#include "Resource.h"
#include "GameClientView.h"
#include "GameClientEngine.h"
#include ".\gameclientview.h"

//////////////////////////////////////////////////////////////////////////

//ʱ���ʶ
#define IDI_FLASH_WINNER			100									//������ʶ
#define IDI_SHOW_CHANGE_BANKER		101									//�ֻ�ׯ��
#define IDI_DISPATCH_CARD			102									//���Ʊ�ʶ
#define IDI_SHOWDISPATCH_CARD_TIP	103									//������ʾ
#define IDI_OPENCARD				104									//������ʾ
#define IDI_FLASH_TIP				105									//�Ƅ��ƽY��
#define IDI_POSTCARD				106									//������ʾ
#define IDI_OUT_CARD_TIME			107
#define IDI_JETTON_ANIMATION		108
#define IDI_HANDLELEAVE_ANIMATION	109  
#define IDI_OPENBOX_ANIMATION		110  
#define IDI_LEAVHANDLESOUND			111
#define IDI_PLEASEJETTONSOUND		112
#define IDI_SPEEKSOUND				113
#define IDI_FLASH_CARD				115									//������ʶ
#define IDI_FLASH_RAND_SIDE			116									//������ʶ
#define IDI_SHOW_CARD_RESULT		118
#define IDI_SHOW_END				119									//��ʾ��������
#define	IDI_GUESS					120									//�´�С��ʱ��
#define	IDI_EFFECT					121									//������ʱ��s

//��������ע,�������ж�ʱ����������(1000�Ժ�),ר�������ṩ
#define IDI_ANDROID_BET				1000								

//��ť��ʶ
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
#define IDC_JETTON_BUTTON_50000	    219									//��ť��ʶ
#define IDC_JETTON_BUTTON_500000	212									//��ť��ʶ
#define IDC_AUTO_OPEN_CARD			213									//��ť��ʶ
#define IDC_OPEN_CARD				214									//��ť��ʶ
#define IDC_BANK					215									//��ť��ʶ
#define IDC_CONTINUE_CARD			216									//��ť��ʶ
#define IDC_BANK_STORAGE			217									//��ť��ʶ
#define IDC_BANK_DRAW				218									//��ť��ʶ
#define IDC_UP						223									//��ť��ʶ
#define IDC_DOWN					224									//��ť��ʶ
#define IDC_RADIO					225									//��ť��ʶ
#define IDC_ADMIN					229									//��ť��ʶ
#define IDC_CHAGE_MONEY				230									//��ť��ʶ
#define	IDC_CONTINUE_JETTON			231									//��ť��ʶ
#define IDC_SMALL					232									//��ť��ʶ
#define	IDC_BIG						233									//��ť��ʶ
#define	IDC_1_TIME					234									//��ť��ʶ
#define	IDC_10_TIME					235									//��ť��ʶ
#define	IDC_100_TIME				236									//��ť��ʶ
#define	IDC_CHIP_100				237									//��ť��ʶ
#define	IDC_CHIP_1000				238									//��ť��ʶ
#define	IDC_CHIP_10000				239									//��ť��ʶ
#define	IDC_CHIP_CLOSE				240									//��ť��ʶ
#define	IDC_CHIP_X					241									//��ť��ʶ
#define	IDC_CHIP_OK					242									//��ť��ʶ
#define	IDC_CHIP_ALL				243									//��ť��ʶ

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameClientView, CGameFrameViewGDI)
	ON_WM_TIMER()
	ON_WM_CREATE()
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_BN_CLICKED(IDC_ADMIN,OpenAdminWnd)
	ON_BN_CLICKED(IDC_APPY_BANKER, OnApplyBanker)
	ON_BN_CLICKED(IDC_CANCEL_BANKER, OnCancelBanker)
	ON_BN_CLICKED(IDC_SCORE_MOVE_L, OnScoreMoveL)
	ON_BN_CLICKED(IDC_SCORE_MOVE_R, OnScoreMoveR)
	ON_BN_CLICKED(IDC_OPEN_CARD, OnOpenCard)
	ON_BN_CLICKED(IDC_AUTO_OPEN_CARD,OnAutoOpenCard)	
#ifdef  __SPECIAL___
	ON_BN_CLICKED(IDC_BANK_STORAGE, OnBankStorage)
	ON_BN_CLICKED(IDC_BANK_DRAW, OnBankDraw)
#endif

	ON_BN_CLICKED(IDC_UP, OnUp)
	ON_BN_CLICKED(IDC_DOWN, OnDown)


//	ON_WM_KEYDOWN()
//ON_WM_KEYDOWN()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameClientView::CGameClientView()
{
	//��ע��Ϣ
	ZeroMemory(m_lUserJettonScore,sizeof(m_lUserJettonScore));

	//ȫ����ע
	ZeroMemory(m_lAllJettonScore,sizeof(m_lAllJettonScore));

	//���������ע
	ZeroMemory(m_lAllPlayBet,sizeof(m_lAllPlayBet));
	ZeroMemory(m_cbResulte,sizeof(m_cbResulte));

	//ׯ����Ϣ
	m_wBankerUser=INVALID_CHAIR;		
	m_wBankerTime=0;
	m_lBankerScore=0L;	
	m_lBankerWinScore=0L;
	m_lTmpBankerWinScore=0;
	m_Out_Bao_y = 0;
	m_CarIndex = 0;
	m_lRobotMaxJetton = 5000000l;
	m_nTime=1;
	m_bShow=false;
	m_bTip=false;
	m_bGuess=false;		

	//���ֳɼ�		
	m_lGameRevenue=0L;
	m_lLastJetton = 0;
	ZeroMemory(m_lExchangeScore,sizeof(m_lExchangeScore));
	m_nExchange=1;
	m_lUserScore=0;

	//״̬��Ϣ
	m_lCurrentJetton=0L;
	m_cbAreaFlash=0xFF;
	m_wMeChairID=INVALID_CHAIR;
	m_bShowChangeBanker=FALSE;
	m_bNeedSetGameRecord=FALSE;
	m_bWinTianMen=FALSE;
	m_bWinHuangMen=FALSE;
	m_bWinXuanMen=FALSE;
	m_bFlashResult=FALSE;
	m_blMoveFinish = FALSE;
	m_blAutoOpenCard = TRUE;
	m_bShowBao = FALSE;
	m_enDispatchCardTip=enDispatchCardTip_NULL;
	m_nEffectIndex=0;
	m_bShowJettonIndex = 0;
	m_bShowLeaveHandleIndex = 0;

	m_lMeCurGameScore=0L;
	ZeroMemory(m_lMeCurGameReturnScore,sizeof(m_lMeCurGameReturnScore));
	m_lBankerCurGameScore=0L;	
	m_blCanStore=false;
	m_bEffect=false;

	m_lAreaLimitScore=0L;
	m_nShowValleyIndex = 0;
	m_nFlashTipIndex=0;
	m_nGuessNum=0;
	m_bOPenBoxIndex = 0;
	m_bShowJettonAn = false;

	//λ����Ϣ
	m_nScoreHead = 0;
	m_nRecordFirst= 0;	
	m_nRecordLast= 0;	
	m_CheckImagIndex = 0;
	m_ShowImageIndex  = 0;

	//��ʷ�ɼ�
	m_lMeStatisticScore=0;

	//�ؼ�����
	m_pGameClientDlg=NULL;

	

	m_bEnablePlaceJetton = false;

	m_pClientControlDlg = NULL;
	m_hInst = NULL;

	return;
}

//��������
CGameClientView::~CGameClientView(void)
{
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
//������Ϣ
int CGameClientView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct)==-1) return -1;

	AfxSetResourceHandle(AfxGetInstanceHandle());


	//����λͼ
	HINSTANCE hInstance=AfxGetInstanceHandle();
	m_ImageViewFill.LoadFromResource(this,hInstance,IDB_VIEW_FILL);

	m_ImageWinFlags.LoadImage(this,hInstance,_TEXT("IDB_IDB_FLAG_PNG"));

	//m_ImageJettonView.LoadFromResource(this,hInstance,IDB_JETTOM_VIEW);	
	m_ViewBackPng.LoadImage(this,hInstance,_TEXT("IDB_VIEW_BACK_PNG"));
	m_idb_selPng.LoadImage(this,hInstance,_TEXT("IDB_IDB_SEL_PNG"));
		
	m_ImageTimeFlagPng.LoadImage(this,hInstance,_TEXT("IDB_TIME_PNG"));
	m_TimerCount_png.LoadImage(this,hInstance,_TEXT("IDB_TIMERCOUNT_PNG"));
	m_ImageMeScoreNumberPng.LoadImage(this,hInstance,_TEXT("IDB_MENUM_PNG"));
	m_ImageScoreNumberPng.LoadImage(this,hInstance,_TEXT("IDB_SCORE_PNG"));
	m_pngGameEnd.LoadImage(this,hInstance,_TEXT("IDB_GAME_END_PNG"));	
	m_ImageTimesFrame1.LoadImage(this,hInstance,_TEXT("TIME_FRAME_1"));
	m_ImageTimesFrame2.LoadImage(this,hInstance,_TEXT("TIME_FRAME_2"));
	m_ImageChangeBack.LoadImage(hInstance,TEXT("CHIP_BACK"));
	m_ImageChipNum.LoadImage(hInstance,TEXT("CHIP_NUM"));
	m_ImageTip.LoadImage(hInstance,TEXT("TREE_TIP"));
	m_ImageEat.LoadImage(hInstance,TEXT("EAT"));

	m_PngFrameTianMen[0].LoadImage(this,hInstance,_TEXT("EFFECT_1"));
	m_PngFrameTianMen[1].LoadImage(this,hInstance,_TEXT("EFFECT_2"));
	m_PngFrameTianMen[2].LoadImage(this,hInstance,_TEXT("EFFECT_3"));
	m_PngFrameTianMen[3].LoadImage(this,hInstance,_TEXT("EFFECT_4"));
	m_PngFrameTianMen[4].LoadImage(this,hInstance,_TEXT("EFFECT_5"));
	m_PngFrameTianMen[5].LoadImage(this,hInstance,_TEXT("EFFECT_6"));
	m_PngFrameTianMen[6].LoadImage(this,hInstance,_TEXT("EFFECT_7"));
	m_PngFrameTianMen[7].LoadImage(this,hInstance,_TEXT("EFFECT_8"));	
	m_ImageEffectTimes.LoadImage(hInstance,TEXT("GOOD_LUCK"));
	m_EffectBigSanYuan.LoadImage(hInstance,TEXT("BIG_SANYUAN"));
	m_EffectSmallSanYuan.LoadImage(hInstance,TEXT("SMALL_SANYUAN"));
	m_EffectBigSiXi.LoadImage(hInstance,TEXT("BIG_SIXI"));
	m_EffectBackLeft.LoadImage(hInstance,TEXT("BACK_EFFECT_L"));
	m_EffectBackRight.LoadImage(hInstance,TEXT("BACK_EFFECT_R"));

	//for (int i = 0;i<AREA_COUNT;i++) m_sT_ShowInfo.blShow[i]=false;

	TCHAR szBuffer[128]=TEXT("");
	//�����Ʊ���Դ
	for (int i = 0;i<7;i++)
	{
		_sntprintf(szBuffer,CountArray(szBuffer),TEXT("IDB_PUSHBOX%d"),i+1);
		m_PngPushBox[i].LoadImage(this,hInstance,szBuffer);
	}

	for (int i = 0;i<4;i++)
	{
		_sntprintf(szBuffer,CountArray(szBuffer),TEXT("IDB_JETTON%d"),i+1);
		m_PngShowJetton[i].LoadImage(this,hInstance,szBuffer);
	}

	for (int i = 0;i<4;i++)
	{
		_sntprintf(szBuffer,CountArray(szBuffer),TEXT("IDB_HANDLELEAVE%d"),i+1);
		m_PngShowLeaveHandle[i].LoadImage(this,hInstance,szBuffer);
	}


	for (int i = 0;i<4;i++)
	{
		_sntprintf(szBuffer,CountArray(szBuffer),TEXT("IDB_RESULT%d"),i+1);
		m_PngResult[i].LoadImage(this,hInstance,szBuffer);
	}

	//�����ؼ�
	CRect rcCreate(0,0,0,0);
	//m_ApplyUser.Create( IDD_DLG_GAME_RECORD	, this );

	//��ע��ť
	m_btJetton100.Create(NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_JETTON_BUTTON_100);
	m_btJetton1000.Create(NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_JETTON_BUTTON_1000);
	m_btJetton10000.Create(NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_JETTON_BUTTON_10000);
	m_btJetton50000.Create(NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_JETTON_BUTTON_50000);
	m_btJetton100000.Create(NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_JETTON_BUTTON_100000);
	m_btJetton500000.Create(NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_JETTON_BUTTON_500000);
	m_btJetton1000000.Create(NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_JETTON_BUTTON_1000000);
	m_btJetton5000000.Create(NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_JETTON_BUTTON_5000000);

	m_btChageMoney.Create(NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_CHAGE_MONEY);
	m_btContinueJetton.Create(NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_CONTINUE_JETTON);
	m_btSmall.Create(NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_SMALL);
	m_btBig.Create(NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_BIG);

	m_btOneTime.Create(NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_1_TIME);
	m_btTenTime.Create(NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_10_TIME);
	m_bt100Time.Create(NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_100_TIME);	

	//���밴ť
	m_btApplyBanker.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_APPY_BANKER);
	m_btCancelBanker.Create(NULL,WS_CHILD|WS_DISABLED,rcCreate,this,IDC_CANCEL_BANKER);

	m_btScoreMoveL.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_SCORE_MOVE_L);
	m_btScoreMoveR.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_SCORE_MOVE_R);

	//m_btBankerStorage.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_BANK_STORAGE);
	//m_btBankerDraw.Create(NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_BANK_DRAW);

	m_btUp.Create(NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_UP);
	m_btDown.Create(NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_DOWN);

	//�һ�����
	m_btChip100.Create(NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_CHIP_100);
	m_btChip1000.Create(NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_CHIP_1000);
	m_btChip10000.Create(NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_CHIP_10000);
	m_btChipClose.Create(NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_CHIP_CLOSE);
	m_btChip_X.Create(NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_CHIP_X);
	m_btChipOK.Create(NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_CHIP_OK);
	m_btChipAll.Create(NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_CHIP_ALL);

	m_btChip100.ShowWindow(SW_HIDE);
	m_btChip1000.ShowWindow(SW_HIDE);
	m_btChip10000.ShowWindow(SW_HIDE);
	m_btChipClose.ShowWindow(SW_HIDE);
	m_btChip_X.ShowWindow(SW_HIDE);
	m_btChipOK.ShowWindow(SW_HIDE);
	m_btChipAll.ShowWindow(SW_HIDE);


	//���ð�ť
	HINSTANCE hResInstance=AfxGetInstanceHandle();
	m_btJetton100.SetButtonImage(IDB_BT_JETTON_100,hResInstance,false,true);
	m_btJetton1000.SetButtonImage(IDB_BT_JETTON_1000,hResInstance,false,true);
	m_btJetton50000.SetButtonImage(IDB_BT_JETTON_50000,hResInstance,false,true);
	m_btJetton10000.SetButtonImage(IDB_BT_JETTON_10000,hResInstance,false,true);
	m_btJetton100000.SetButtonImage(IDB_BT_JETTON_100000,hResInstance,false,true);
	m_btJetton500000.SetButtonImage(IDB_BT_JETTON_500000,hResInstance,false,true);
	m_btJetton1000000.SetButtonImage(IDB_BT_JETTON_1000000,hResInstance,false,true);
	m_btJetton5000000.SetButtonImage(IDB_BT_JETTON_5000000,hResInstance,false,true);

	m_btChageMoney.SetButtonImage(IDB_BT_CHAGEMONEY,hResInstance,false,false);
	m_btContinueJetton.SetButtonImage(IDB_BT_CONTINUE_JETTON,hResInstance,false,true);
	m_btSmall.SetButtonImage(IDB_BT_SMALL,hResInstance,false,false);
	m_btBig.SetButtonImage(IDB_BT_BIG,hResInstance,false,false);

	m_btOneTime.SetButtonImage(BT_1_TIME,hResInstance,false,false);
	m_btTenTime.SetButtonImage(BT_10_TIME,hResInstance,false,false);
	m_bt100Time.SetButtonImage(BT_100_TIME,hResInstance,false,false);

	m_btApplyBanker.SetButtonImage(IDB_BT_APPLY_BANKER,hResInstance,false,false);
	m_btCancelBanker.SetButtonImage(IDB_BT_CANCEL_APPLY,hResInstance,false,false);


	m_btUp.SetButtonImage(IDB_BT_BT_S,hResInstance,false,false);
	m_btDown.SetButtonImage(IDB_BT_BT_X,hResInstance,false,false);

	m_btScoreMoveL.SetButtonImage(IDB_BT_SCORE_MOVE_L,hResInstance,false,false);
	m_btScoreMoveR.SetButtonImage(IDB_BT_SCORE_MOVE_R,hResInstance,false,false);

	m_btChip100.SetButtonImage(IDB_BT_CHIP_100,hResInstance,false,false);
	m_btChip1000.SetButtonImage(IDB_BT_CHIP_1000,hResInstance,false,false);
	m_btChip10000.SetButtonImage(IDB_BT_CHIP_10000,hResInstance,false,false);
	m_btChipClose.SetButtonImage(IDB_BT_CHIP_CLOSE,hResInstance,false,false);
	m_btChip_X.SetButtonImage(IDB_BT_CHIP_CLOSE_X,hResInstance,false,false);
	m_btChipOK.SetButtonImage(IDB_BT_CHIP_OK,hResInstance,false,false);
	m_btChipAll.SetButtonImage(IDB_BT_CHIP_ALL,hResInstance,false,false);

	//m_btBankerStorage.SetButtonImage(IDB_BT_STORAGE,hResInstance,false,false);
	//m_btBankerDraw.SetButtonImage(IDB_BT_DRAW,hResInstance,false,false);

	m_btOpenAdmin.Create(NULL,WS_CHILD|WS_VISIBLE,CRect(4,4,11,11),this,IDC_ADMIN);
	m_btOpenAdmin.ShowWindow(SW_HIDE);

	SwitchToCheck();
	SetJettonHide(0);

	//����
	m_hInst = NULL;
	m_pClientControlDlg = NULL;
	m_hInst = LoadLibrary(_TEXT("FruitBattleClientControl.dll"));
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
VOID CGameClientView::ResetGameView()
{
	//��ע��Ϣ
	ZeroMemory(m_lUserJettonScore,sizeof(m_lUserJettonScore));

	//ȫ����ע
	ZeroMemory(m_lAllJettonScore,sizeof(m_lAllJettonScore));

	//���������ע
	ZeroMemory(m_lAllPlayBet,sizeof(m_lAllPlayBet));
	ZeroMemory(m_cbResulte,sizeof(m_cbResulte));

	//ׯ����Ϣ
	m_wBankerUser=INVALID_CHAIR;		
	m_wBankerTime=0;
	m_lBankerScore=0L;	
	m_lBankerWinScore=0L;
	m_lTmpBankerWinScore=0;	

	//���ֳɼ�o
	m_lMeCurGameScore=0L;	
	ZeroMemory(m_lMeCurGameReturnScore,sizeof(m_lMeCurGameReturnScore));
	m_lBankerCurGameScore=0L;
	m_lGameRevenue=0L;

	//״̬��Ϣ
	m_lCurrentJetton=0L;
	m_cbAreaFlash=0xFF;
	m_wMeChairID=INVALID_CHAIR;
	m_bShowChangeBanker=false;
	m_bNeedSetGameRecord=false;
	m_bWinTianMen=false;
	m_bWinHuangMen=false;
	m_bWinXuanMen=false;
	m_bFlashResult=false;
	m_bShowGameResult=false;
	m_enDispatchCardTip=enDispatchCardTip_NULL;
	m_bShow=false;
	m_bTip=false;
	m_bEffect=false;

	m_lAreaLimitScore=0L;
	m_nShowValleyIndex = 0;
	m_nFlashTipIndex=0;
	m_nGuessNum=0;
	m_bGuess=false;
	m_nEffectIndex=0;
	m_bOPenBoxIndex = 0;
	m_bShowJettonAn = false;
	m_bShowJettonIndex = 0;	

	//λ����Ϣ
	m_nScoreHead = 0;
	m_nRecordFirst= 0;	
	m_nRecordLast= 0;	

	//��ʷ�ɼ�
	m_lMeStatisticScore=0;

	//����б�
	m_ValleysList.RemoveAll();

	//�������
	CleanUserJetton();

	//���ð�ť
	m_btApplyBanker.ShowWindow(SW_SHOW);
	m_btApplyBanker.EnableWindow(FALSE);
	m_btCancelBanker.ShowWindow(SW_HIDE);
	m_btCancelBanker.SetButtonImage(IDB_BT_CANCEL_APPLY,AfxGetInstanceHandle(),false,false);

	return;
}

//�����ؼ�
VOID CGameClientView::RectifyControl(INT nWidth, INT nHeight)
{
	//λ����Ϣ


	CSize Size;

	Size.cy = Size.cy/2;

	//CImageHandle ImageHandle(&m_ImageViewBack[0]);
	int iWidth =756;// m_ImageViewBack[0].GetWidth();
	int iHeight =705;// m_ImageViewBack[0].GetHeight();

	int LifeWidth = nWidth/2-iWidth/2;
	int TopHeight = nHeight/2-iHeight/2;

	int LifeWidth1 = nWidth/2+iWidth/3-47;
	int TopHeight1 = nHeight/2+12;

	m_RectScore[0].SetRect(LifeWidth1,TopHeight1,LifeWidth1+80,TopHeight1+15);
	m_RectScore[1].SetRect(LifeWidth1+88,TopHeight1,LifeWidth1+80+88,TopHeight1+15);
	m_RectScore[2].SetRect(LifeWidth1,TopHeight1+57,LifeWidth1+80,TopHeight1+15+57);
	m_RectScore[3].SetRect(LifeWidth1+88,TopHeight1+57,LifeWidth1+80+88,TopHeight1+15+57);
	m_RectScore[4].SetRect(LifeWidth1,TopHeight1+57*2,LifeWidth1+80,TopHeight1+15+57*2);
	m_RectScore[5].SetRect(LifeWidth1+88,TopHeight1+57*2,LifeWidth1+80+88,TopHeight1+15+57*2);
	m_RectScore[6].SetRect(LifeWidth1,TopHeight1+57*3,LifeWidth1+80,TopHeight1+15+57*3);
	m_RectScore[7].SetRect(LifeWidth1+88,TopHeight1+57*3,LifeWidth1+80+88,TopHeight1+15+57*3);

	for(int i=0;i<24;i++)
	{
		if(i<7)
		{
			m_CarRect[i].top =nHeight/2-303;
			m_CarRect[i].left =nWidth/2-314+i*70;
		}
		else if(i>6&&i<12)
		{
			m_CarRect[i].top =nHeight/2-303+(i-6)*70;
			m_CarRect[i].left =nWidth/2-314+6*70;
		}
		else if(i>11&&i<19)
		{
			m_CarRect[i].top =nHeight/2+117;
			m_CarRect[i].left =nWidth/2+106-(i-12)*70;
		}
		else
		{
			m_CarRect[i].top =nHeight/2+117-(i-18)*70;
			m_CarRect[i].left =nWidth/2-314;
		}
	}
	
	for (int i = 0;i<8;i++)
	{
		if(i<4)
		{
			m_RectArea[i].top =nHeight/2-276-8+133;
			m_RectArea[i].left =nWidth/2-362-17+i*128-5+106-96-8+128;
			m_RectArea[i].bottom =m_RectArea[i].top+162;
			m_RectArea[i].right  = m_RectArea[i].left+128;

		}else
		{
			m_RectArea[i].top =nHeight/2-276-8+133+162;
			m_RectArea[i].left =nWidth/2-362-17+(i-4)*128-5+106-96-8+128;
			m_RectArea[i].bottom =m_RectArea[i].top+162;
			m_RectArea[i].right  = m_RectArea[i].left+128;

		}
	}

	m_PointJettonNumber[0].SetPoint(LifeWidth+113+63*7-1, nHeight/2+242);
	m_PointJettonNumber[1].SetPoint(LifeWidth+113+63*6, nHeight/2+242);
	m_PointJettonNumber[2].SetPoint(LifeWidth+113+63*5, nHeight/2+242);
	m_PointJettonNumber[3].SetPoint(LifeWidth+113+63*4-1, nHeight/2+242);
	m_PointJettonNumber[4].SetPoint(LifeWidth+113+63*3, nHeight/2+242);
	m_PointJettonNumber[5].SetPoint(LifeWidth+113+63*2, nHeight/2+242);
	m_PointJettonNumber[6].SetPoint(LifeWidth+113+63, nHeight/2+242);
	m_PointJettonNumber[7].SetPoint(LifeWidth+113, nHeight/2+242);
	
	for (int i = 0;i<AREA_COUNT;i++)
	{		
		m_PointJetton[i].SetPoint(m_RectArea[i].left, m_RectArea[i].top);
	}

	m_CardTypePoint[0] =CPoint(nWidth/2+125,nHeight/2-355+135+62); 

	//�ƶ��ؼ�
	HDWP hDwp=BeginDeferWindowPos(33);
	const UINT uFlags=SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOCOPYBITS;

	//m_ApplyUser.m_viewHandle = m_hWnd;

	//�б�ؼ�
	//DeferWindowPos(hDwp,m_ApplyUser,NULL,nWidth/2 + 148-10+50,nHeight/2-291-7-35,178,28,uFlags);
	DeferWindowPos(hDwp,m_btUp,NULL,LifeWidth+317+20+310+85,TopHeight+180+11+40,60/5,11,uFlags);
	DeferWindowPos(hDwp,m_btDown,NULL,LifeWidth+317+20+310+85,TopHeight+180+11+102,60/5,11,uFlags);

	//m_btBankerStorage.EnableWindow(TRUE);

	m_MeInfoRect.top = TopHeight+10+12;
	m_MeInfoRect.left = LifeWidth+245+20+389;

	m_BaoPosion.top = TopHeight;
	m_BaoPosion.left = nWidth/2;

	//���밴ť
	CRect rcJetton;
	m_btJetton100.GetWindowRect(&rcJetton);
	int nYPos =TopHeight+40+434+5+16-2+100+20;
	int nXPos =  LifeWidth+182+4+1+10 ;
	int nSpace = 15;

	m_TopHeight = TopHeight;
	m_LifeWidth = LifeWidth;

	m_nWinFlagsExcursionX = LifeWidth-13;	
	m_nWinFlagsExcursionY = TopHeight+112;

	nSpace = 0;

	DeferWindowPos(hDwp,m_btJetton100,NULL,nXPos-115+64*7+9,nYPos+19,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btJetton1000,NULL,nXPos -115+64*6+8,nYPos+19,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btJetton10000,NULL,nXPos -115+64*5+7,nYPos+19,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btJetton50000,NULL,nXPos -115+64*4+5,nYPos+19,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btJetton100000,NULL,nXPos -115+64*3+3,nYPos+19,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btJetton500000,NULL,nXPos -115+64*2,nYPos+19,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btJetton1000000,NULL,nXPos-115+64,nYPos+19,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btJetton5000000,NULL,nXPos-115,nYPos+19,0,0,uFlags|SWP_NOSIZE);

	DeferWindowPos(hDwp,m_btChageMoney,NULL,nXPos-180,nYPos-45,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btContinueJetton,NULL,nXPos-115+64*7+79,nYPos+25,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btSmall,NULL,nXPos-115+64*9+25,nYPos+25,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btBig,NULL,nXPos-115+64*9+35,nYPos+58,0,0,uFlags|SWP_NOSIZE);

	DeferWindowPos(hDwp,m_btOneTime,NULL,nXPos-115+64*8+9,nYPos-25,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btTenTime,NULL,nXPos-115+64*8+59,nYPos-25,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_bt100Time,NULL,nXPos-115+64*8+109,nYPos-25,0,0,uFlags|SWP_NOSIZE);

	DeferWindowPos(hDwp,m_btApplyBanker,NULL,m_LifeWidth+654-3-41,m_TopHeight+30-3+138,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btCancelBanker,NULL,m_LifeWidth+654-3-41,m_TopHeight+30-3+138,0,0,uFlags|SWP_NOSIZE);


	DeferWindowPos(hDwp,m_btScoreMoveL,NULL,LifeWidth+11,TopHeight+90,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btScoreMoveR,NULL,LifeWidth+11,TopHeight+533,0,0,uFlags|SWP_NOSIZE);

	DeferWindowPos(hDwp,m_btChip100,NULL,(nWidth-m_ImageChangeBack.GetWidth())/2+74,(nHeight-m_ImageChangeBack.GetHeight())/2+148,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btChip1000,NULL,(nWidth-m_ImageChangeBack.GetWidth())/2+74*2+4,(nHeight-m_ImageChangeBack.GetHeight())/2+148,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btChip10000,NULL,(nWidth-m_ImageChangeBack.GetWidth())/2+74*3+7,(nHeight-m_ImageChangeBack.GetHeight())/2+148,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btChipAll,NULL,(nWidth-m_ImageChangeBack.GetWidth())/2+74*4+10,(nHeight-m_ImageChangeBack.GetHeight())/2+148,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btChipClose,NULL,(nWidth-m_ImageChangeBack.GetWidth())/2+270,(nHeight-m_ImageChangeBack.GetHeight())/2+264,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btChip_X,NULL,(nWidth+m_ImageChangeBack.GetWidth())/2-34,(nHeight-m_ImageChangeBack.GetHeight())/2+12,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btChipOK,NULL,(nWidth-m_ImageChangeBack.GetWidth())/2+93,(nHeight-m_ImageChangeBack.GetHeight())/2+264,0,0,uFlags|SWP_NOSIZE);
	
	
	//������ť
	//������ť
	//DeferWindowPos(hDwp,m_btBankerStorage,NULL,LifeWidth+10-1+571,TopHeight+592-12+33,104,400,uFlags|SWP_NOSIZE);
	//DeferWindowPos(hDwp,m_btBankerDraw,NULL,LifeWidth+10-1+571,TopHeight+592+50-17+33,0,0,uFlags|SWP_NOSIZE);
	//�����ƶ�
	EndDeferWindowPos(hDwp);

	if ( m_pClientControlDlg )
	{

		for ( int i = 0; i < CountArray(m_SkinListCtrl); ++i )
		{
			CRect m_WinRect(m_RectArea[i]);
			m_WinRect.top += m_WinRect.Height() / 2;

			m_WinRect.right-=2;
			m_WinRect.OffsetRect(0, -77);

			m_SkinListCtrl[i].MoveWindow(m_WinRect);
			m_SkinListCtrl[i].SetColumnWidth( 0, m_WinRect.Width()/2 );
			m_SkinListCtrl[i].SetColumnWidth( 1, m_WinRect.Width()/2 );
		}

	}



	return;
}
void CGameClientView::SwitchToCheck()
{
	SwithToNormalView();
	return;


	//m_ApplyUser.ShowWindow(SW_HIDE);
	m_btApplyBanker.ShowWindow(SW_HIDE);
	m_btCancelBanker.ShowWindow(SW_HIDE);
	m_btScoreMoveL.ShowWindow(SW_HIDE);
	m_btScoreMoveR.ShowWindow(SW_HIDE);
	m_btJetton100.ShowWindow(SW_HIDE);
	m_btJetton1000.ShowWindow(SW_HIDE);
	m_btJetton10000.ShowWindow(SW_HIDE);
	m_btJetton50000.ShowWindow(SW_HIDE);
	m_btJetton100000.ShowWindow(SW_HIDE);
	m_btJetton500000.ShowWindow(SW_HIDE);
	m_btJetton1000000.ShowWindow(SW_HIDE);
	m_btJetton5000000.ShowWindow(SW_HIDE);
	m_btChageMoney.ShowWindow(SW_HIDE);
	m_btContinueJetton.ShowWindow(SW_HIDE);
	m_btSmall.ShowWindow(SW_HIDE);
	m_btBig.ShowWindow(SW_HIDE);
	m_DrawBack = false;
	SwithToNormalView();


}
void CGameClientView::SwithToNormalView()
{


	//m_ApplyUser.ShowWindow(SW_SHOW);
	m_btApplyBanker.ShowWindow(SW_SHOW);
	m_btCancelBanker.ShowWindow(SW_SHOW);
	m_btScoreMoveL.ShowWindow(SW_SHOW);
	m_btScoreMoveR.ShowWindow(SW_SHOW);

	m_btJetton100.ShowWindow(SW_SHOW);
	m_btJetton1000.ShowWindow(SW_SHOW);
	m_btJetton10000.ShowWindow(SW_SHOW);
	m_btJetton50000.ShowWindow(SW_SHOW);
	m_btJetton100000.ShowWindow(SW_SHOW);
	m_btJetton500000.ShowWindow(SW_SHOW);
	m_btJetton1000000.ShowWindow(SW_SHOW);	
	m_btJetton5000000.ShowWindow(SW_SHOW);
	m_btChageMoney.ShowWindow(SW_SHOW);
	m_btContinueJetton.ShowWindow(SW_SHOW);
	m_btSmall.ShowWindow(SW_SHOW);
	m_btBig.ShowWindow(SW_SHOW);

	m_DrawBack = true;

}

//�滭����
VOID CGameClientView::DrawGameView(CDC * pDC, INT nWidth, INT nHeight)
{	
	//��ȡ���
	IClientUserItem *pClientUserItem = m_wBankerUser==INVALID_CHAIR ? NULL : GetClientUserItem(m_wBankerUser);
	//�滭����
	//DrawViewImage(pDC,m_ImageViewFill,DRAW_MODE_SPREAD); 

	CDC* pDCBuff = pDC;

	//�滭����
	for ( int iW = 0 ; iW < nWidth; iW += m_ImageViewFill.GetWidth() )
	{
		for ( int iH = 0;  iH < nHeight; iH += m_ImageViewFill.GetHeight() )
		{
			m_ImageViewFill.BitBlt(pDC->GetSafeHdc(), iW, iH);
		}
	}
	m_ViewBackPng.DrawImage(pDCBuff,(nWidth-m_ViewBackPng.GetWidth())/2,nHeight/2-m_ViewBackPng.GetHeight()/2+10);

	//��ȡ״̬
	BYTE cbGameStatus=0;
	if(m_pGameClientDlg != NULL)
		cbGameStatus=m_pGameClientDlg->GetGameStatus();

	int nDestWidth = m_idb_selPng.GetWidth()/4;
	int nDestHeight = m_idb_selPng.GetHeight();
	if(m_blRungingCar && cbGameStatus == GS_GAME_END)
	{
		m_idb_selPng.DrawImage(pDCBuff ,m_CarRect[m_CarIndex].left,m_CarRect[m_CarIndex].top,nDestWidth,nDestHeight,0,0);
		m_idb_selPng.DrawImage(pDCBuff ,m_CarRect[(m_CarIndex-1+24)%24].left,m_CarRect[(m_CarIndex-1+24)%24].top,nDestWidth,nDestHeight,nDestWidth,0);
		m_idb_selPng.DrawImage(pDCBuff ,m_CarRect[(m_CarIndex-2+24)%24].left,m_CarRect[(m_CarIndex-2+24)%24].top,nDestWidth,nDestHeight,nDestWidth*2,0);
		m_idb_selPng.DrawImage(pDCBuff ,m_CarRect[(m_CarIndex-3+24)%24].left,m_CarRect[(m_CarIndex-3+24)%24].top,nDestWidth,nDestHeight,nDestWidth*3,0);
	}
	if(m_bFlashrandShow)
	{
		m_idb_selPng.DrawImage(pDCBuff ,m_CarRect[m_CarIndex].left,m_CarRect[m_CarIndex].top,nDestWidth,nDestHeight,0,0);
	}	
	if(m_bTip)
	{
		m_ImageTip.DrawImage(pDC,nWidth/2-m_ImageTip.GetWidth()/2+77,nHeight/2-m_ImageTip.GetHeight()/2-50,
			m_ImageTip.GetWidth()/2,m_ImageTip.GetHeight(),m_nFlashTipIndex*m_ImageTip.GetWidth()/2,0);
	}	

	//״̬��ʾ
	CFont static InfoFont;
	InfoFont.CreateFont(-16,0,0,0,400,0,0,0,134,3,2,ANTIALIASED_QUALITY,2,TEXT("����"));
	CFont * pOldFont=pDC->SelectObject(&InfoFont);
	pDC->SetTextColor(RGB(255,255,0));

	pDC->SelectObject(pOldFont);
	InfoFont.DeleteObject();	

	if(m_wMeChairID!=INVALID_CHAIR && m_lExchangeScore[m_wMeChairID]>=0)
	{
		//DrawNumberStringEX(pDCBuff,m_lExchangeScore,nWidth/2+153,nHeight/2-331);	
		DrawNumber(pDCBuff,&m_ImageScoreNumberPng,TEXT("0123456789"),m_lExchangeScore[m_wMeChairID],nWidth/2+170,nHeight/2-346,DT_RIGHT);
	}	

	if(cbGameStatus==GS_COMPARE_END)
	{
		if(m_bGuess)
		{
			if(m_nGuessNum<10)
			{
				DrawNumberString(pDCBuff,m_nGuessNum,m_LifeWidth+329,nHeight/2+72,true);
			}
			else
			{
				DrawNumberString(pDCBuff,m_nGuessNum,m_LifeWidth+313,nHeight/2+72,true);
			}
		}
	}	
	
	//ʱ����ʾ
	if(m_DrawBack)
	{
		WORD wUserTimer =((m_wMeChairID!=INVALID_CHAIR)?GetUserClock(m_wMeChairID):0);

		if (wUserTimer!=0)
		{
			if(wUserTimer<10)
				DrawNumberString(pDCBuff,wUserTimer,m_LifeWidth+329,nHeight/2+72,true);
			else
				DrawNumberString(pDCBuff,wUserTimer,m_LifeWidth+313,nHeight/2+72,true);
		}
	}	
	//ʤ���߿�
	FlashJettonAreaFrame(nWidth,nHeight,pDCBuff);	
	if(m_bEffect)
	{
		if(m_nProbability==2)
		{
			CSize SizeFrame(m_EffectBigSanYuan.GetWidth()/5,m_EffectBigSanYuan.GetHeight());
			m_EffectBigSanYuan.DrawImage(pDC,nWidth/2-183,nHeight/2-165,SizeFrame.cx,SizeFrame.cy,m_nEffectIndex*SizeFrame.cx,0);
		}
		else if(m_nProbability==3)
		{
			CSize SizeFrame(m_EffectSmallSanYuan.GetWidth()/5,m_EffectSmallSanYuan.GetHeight());
			m_EffectSmallSanYuan.DrawImage(pDC,nWidth/2-183,nHeight/2-165,SizeFrame.cx,SizeFrame.cy,m_nEffectIndex*SizeFrame.cx,0);
		}
		else if(m_nProbability==4)
		{
			CSize SizeFrame(m_EffectBigSiXi.GetWidth()/5,m_EffectBigSiXi.GetHeight());
			m_EffectBigSiXi.DrawImage(pDC,nWidth/2-183,nHeight/2-165,SizeFrame.cx,SizeFrame.cy,m_nEffectIndex*SizeFrame.cx,0);
		}
		else if(m_nProbability==5)//ͨ��
		{			
			m_ImageEat.DrawImage(pDC,nWidth/2-183,nHeight/2-165);
		}
		else
		{
			//���Ŷ���
			PlayEffect(pDCBuff,nWidth,nHeight);
		}		
	}
	if(m_blMoveFinish&&(cbGameStatus==GS_GAME_END||cbGameStatus==GS_COMPARE_END))
	{
		if(m_wMeChairID!=INVALID_CHAIR && m_lMeCurGameReturnScore[m_wMeChairID]>0)
		{			
			DrawNumber(pDCBuff,&m_ImageScoreNumberPng,TEXT("0123456789"),m_lMeCurGameReturnScore[m_wMeChairID],nWidth/2-100,nHeight/2-346,DT_RIGHT);
		}
	}	

	//�滭ׯ��
	if(m_DrawBack)
	{
		DrawBankerInfo(pDCBuff,nWidth,nHeight);		
	}
	//�滭��ׯ�б�
	DrawBankerList(pDC,nWidth,nHeight);
	//�滭�û�
	//DrawMeInfo(pDCBuff,nWidth,nHeight);

	//�л�ׯ��
	//if ( m_bShowChangeBanker )
	//{
	//	int	nXPos = nWidth / 2 - 130;
	//	int	nYPos = nHeight / 2 - 160+120;

	//�ҵ���ע
	DrawMeJettonNumber(pDCBuff);
	//ʤ����־
	DrawWinFlags(pDCBuff);

	if(cbGameStatus!=GS_GAME_FREE)
	{
		BYTE cbArea[AREA_COUNT]={5,10,10,10,20,20,20,50};
		LONGLONG lBankerScore=0;
		if(pClientUserItem!=NULL) lBankerScore=m_lBankerScore;
		else lBankerScore=2147483647;		
		for(int i=0;i<AREA_COUNT;i++)
		{
			TCHAR Text[256];
			_sntprintf(Text,256,TEXT("%I64d/%I64d"),m_lAllJettonScore[i+1],lBankerScore/cbArea[i]);
			DrawTextString( pDC, Text, RGB(255,255,255) , RGB(38,26,2), m_RectScore[i], DT_END_ELLIPSIS|DT_LEFT|DT_VCENTER|DT_SINGLELINE);
		}
	}


	if (cbGameStatus != GS_GAME_FREE)
	{
		m_Out_Bao_y = 6;
	}
	/*if(m_blMoveFinish&&cbGameStatus==GS_GAME_END)
	{
		ShowGameResult(pDCBuff, nWidth, nHeight);
	}*/
	/*if(m_DrawBack)
		DrawMoveInfo(pDCBuff,m_rcTianMen);*/
	
	//��������
	if(m_nTime==1) m_ImageTimesFrame1.DrawImage(pDC,nWidth/2-292+64*8,nHeight/2+231);
	else if(m_nTime==10) m_ImageTimesFrame1.DrawImage(pDC,nWidth/2-292+64*8+50,nHeight/2+231);
	else if(m_nTime==100) m_ImageTimesFrame2.DrawImage(pDC,nWidth/2-287+64*8+95,nHeight/2+231);
	
	if(m_bShow)
	{
		if (m_wMeChairID!=INVALID_CHAIR)		
			IClientUserItem *pClientUserItem =  GetClientUserItem(m_wMeChairID);
		m_ImageChangeBack.DrawImage(pDCBuff,nWidth/2-m_ImageChangeBack.GetWidth()/2,nHeight/2-m_ImageChangeBack.GetHeight()/2);
		//DrawNumberStringEX(pDCBuff,m_lUserScore,nWidth/2-m_ImageChangeBack.GetWidth()/2+314,nHeight/2-m_ImageChangeBack.GetHeight()/2+106,true);
		DrawNumber(pDCBuff,&m_ImageChipNum,TEXT("0123456789"),m_lUserScore,nWidth/2-m_ImageChangeBack.GetWidth()/2+238,nHeight/2-m_ImageChangeBack.GetHeight()/2+98);

		//DrawNumberStringEX(pDCBuff,1,nWidth/2-m_ImageChangeBack.GetWidth()/2+190,nHeight/2-m_ImageChangeBack.GetHeight()/2+208,true);
		DrawNumber(pDCBuff,&m_ImageChipNum,TEXT("0123456789"),1,nWidth/2-m_ImageChangeBack.GetWidth()/2+150,nHeight/2-m_ImageChangeBack.GetHeight()/2+198);

		//DrawNumberStringEX(pDCBuff,m_nExchange,nWidth/2-m_ImageChangeBack.GetWidth()/2+319,nHeight/2-m_ImageChangeBack.GetHeight()/2+208,true);
		DrawNumber(pDCBuff,&m_ImageChipNum,TEXT("0123456789"),m_nExchange,nWidth/2-m_ImageChangeBack.GetWidth()/2+279,nHeight/2-m_ImageChangeBack.GetHeight()/2+198);

		//DrawNumberStringEX(pDCBuff,m_lCanExchangeScore,nWidth/2-m_ImageChangeBack.GetWidth()/2+190,nHeight/2-m_ImageChangeBack.GetHeight()/2+233,true);
		//DrawNumber(pDCBuff,&m_ImageChipNum,TEXT("0123456789"),m_lCanExchangeScore,nWidth/2-m_ImageChangeBack.GetWidth()/2+150,nHeight/2-m_ImageChangeBack.GetHeight()/2+223);		
		CRect rcChip;
		rcChip.left = nWidth/2-m_ImageChangeBack.GetWidth()/2+150;
		rcChip.right = rcChip.left+78;
		rcChip.top = nHeight/2-m_ImageChangeBack.GetHeight()/2+224;
		rcChip.bottom = rcChip.top+15;
		TCHAR szChipNum[32];
		_sntprintf(szChipNum,32,TEXT("%I64d"),m_lCanExchangeScore);
		DrawTextString(pDC,szChipNum,RGB(255,127,127),RGB(0,0,0),rcChip,DT_END_ELLIPSIS|DT_CENTER|DT_VCENTER|DT_SINGLELINE,true);

		rcChip.left = nWidth/2-m_ImageChangeBack.GetWidth()/2+275;
		rcChip.right = rcChip.left+92;
		_sntprintf(szChipNum,32,TEXT("%I64d"),m_lCanExchangeScore*m_nExchange);
		DrawTextString(pDC,szChipNum,RGB(255,127,127),RGB(0,0,0),rcChip,DT_END_ELLIPSIS|DT_CENTER|DT_VCENTER|DT_SINGLELINE,true);
		//DrawNumberStringEX(pDCBuff,m_lCanExchangeScore*m_nExchange,nWidth/2-m_ImageChangeBack.GetWidth()/2+319,nHeight/2-m_ImageChangeBack.GetHeight()/2+233,true);
		//DrawNumber(pDCBuff,&m_ImageChipNum,TEXT("0123456789"),m_lCanExchangeScore*m_nExchange,nWidth/2-m_ImageChangeBack.GetWidth()/2+279,nHeight/2-m_ImageChangeBack.GetHeight()/2+223);

		m_btChip100.ShowWindow(SW_SHOW);
		m_btChip1000.ShowWindow(SW_SHOW);
		m_btChip10000.ShowWindow(SW_SHOW);
		m_btChipClose.ShowWindow(SW_SHOW);
		
		m_btChip_X.ShowWindow(SW_SHOW);
		m_btChipOK.ShowWindow(SW_SHOW);
		m_btChipAll.ShowWindow(SW_SHOW);
	}
	
	return;
}

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

//��ȡ�һ�����
void CGameClientView::GetExchangeScore(LONGLONG lScore,int nExchange,LONGLONG lChangeGold)
{	
	//���ñ���
	m_lUserScore=lScore;
	m_nExchange=nExchange;
	m_lCanExchangeScore=lChangeGold;

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
	if(lCurrentJetton==0)
	{
		SetJettonHide(0);
	}
	return;
}


//��ʷ��¼
void CGameClientView::SetGameHistory(BYTE *bcResulte)
{
	//��������
	BYTE bcResulteTmp[AREA_COUNT+1];
	memcpy(bcResulteTmp,bcResulte,AREA_COUNT+1);
	tagClientGameRecord &GameRecord = m_GameRecordArrary[m_nRecordLast];

	for (int i = 0;i<=AREA_COUNT;i++)
	{
		if(bcResulteTmp[i]>2&&bcResulteTmp[i]<=50)
		{
			GameRecord.enOperateMen[i]=enOperateResult_Big;
		}
		else if(bcResulteTmp[i]==2)
		{
			GameRecord.enOperateMen[i]=enOperateResult_Small;
		}
		else if(bcResulteTmp[i]==99)
			GameRecord.enOperateMen[i]=enOperateResult_GreenLucky;
		else if(bcResulteTmp[i]==100)
			GameRecord.enOperateMen[i]=enOperateResult_RedLucky;
		/*if (0==m_lUserJettonScore[i]) GameRecord.enOperateMen[i]=enOperateResult_NULL;
		else if (m_lUserJettonScore[i] > 0 && (bcResulte[i-1]==4)) GameRecord.enOperateMen[i]=enOperateResult_Win;
		else if (m_lUserJettonScore[i] > 0 && (0==bcResulte[i-1])) GameRecord.enOperateMen[i]=enOperateResult_Lost;*/

	}
	//�ƶ��±�
	m_nRecordLast = (m_nRecordLast+1) % MAX_SCORE_HISTORY;
	if ( m_nRecordLast == m_nRecordFirst )
	{
		//LQL��� ���︲�ǵ�ǰ��ļ�¼ Ҫ�ȰѸô���¼���
		ZeroMemory(&m_GameRecordArrary[m_nRecordLast],sizeof(tagClientGameRecord ));

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

	//���������ע
	ZeroMemory(m_lAllPlayBet,sizeof(m_lAllPlayBet));
// 	for ( int i = 0; i < CountArray(m_SkinListCtrl); ++i )
// 	{
// 		if ( m_SkinListCtrl[i].GetSafeHwnd() )
// 			m_SkinListCtrl[i].DeleteAllItems();
// 	}
	if(m_pClientControlDlg)
	{
		m_pClientControlDlg->ClearClist();
	}

	//��ע��Ϣ
	ZeroMemory(m_lUserJettonScore,sizeof(m_lUserJettonScore));

	//ȫ����ע
	ZeroMemory(m_lAllJettonScore,sizeof(m_lAllJettonScore));

	//���������ע
	ZeroMemory(m_lAllPlayBet,sizeof(m_lAllPlayBet));	

	//���½���
	RefreshGameView();

	return;
}

//������ע
void CGameClientView::SetMePlaceJetton(BYTE cbViewIndex, LONGLONG lJettonCount)
{
	//Ч�����
	ASSERT(cbViewIndex<=AREA_COUNT);
	if (cbViewIndex>AREA_COUNT) return;

	m_lUserJettonScore[cbViewIndex]=lJettonCount;

	//���½���
	RefreshGameView();
}

//�����˿�
void CGameClientView::SetCardInfo(BYTE cbTableCardArray[])
{
	if (cbTableCardArray!=NULL)
	{
		CopyMemory(m_cbTableCardArray,cbTableCardArray,sizeof(m_cbTableCardArray));

		//��ʼ����
		DispatchCard();
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
	ASSERT(cbViewIndex<=AREA_COUNT);
	if (cbViewIndex>AREA_COUNT) return;

	//��������
	bool bPlaceJetton = false;	

	//�߿���
	int nFrameWidth=0, nFrameHeight=0;
	int nBorderWidth=6;

	m_lAllJettonScore[cbViewIndex] += lScoreCount;
	nFrameWidth = m_RectArea [cbViewIndex-1].right-m_RectArea [cbViewIndex-1].left;
	nFrameHeight = m_RectArea[cbViewIndex-1].bottom-m_RectArea [cbViewIndex-1].top;
	

	if( lScoreCount < 0)
	{
		lScoreCount = -lScoreCount;
	}

	//���½���
	if (bPlaceJetton==true) 
		RefreshGameView();

	return;
}

//��������ע
void CGameClientView::AndroidBet(BYTE cbViewIndex, LONGLONG lScoreCount)
{
	//Ч�����
	ASSERT(cbViewIndex<=AREA_COUNT);
	if (cbViewIndex>AREA_COUNT) 
		return;

	if ( lScoreCount <= 0L )
		return;

	tagAndroidBet Androi;
	Androi.cbJettonArea = cbViewIndex;
	Androi.lJettonScore = lScoreCount;
	m_ArrayAndroid.Add(Androi);
	SetTimer(IDI_ANDROID_BET,100,NULL);
	int nHaveCount = 0;
	for ( int i = 0 ; i < m_ArrayAndroid.GetCount(); ++i)
	{
		if(m_ArrayAndroid[i].lJettonScore > 0)
			nHaveCount++;
	}
	UINT nElapse = 0;
	if ( nHaveCount <= 1 )
		nElapse = 260;
	else if ( nHaveCount <= 2 )
		nElapse = 160;
	else
		nElapse = 100;

	SetTimer(IDI_ANDROID_BET+m_ArrayAndroid.GetCount(),nElapse,NULL);
}

//���ֳɼ�
void CGameClientView::SetCurGameScore(LONGLONG lMeCurGameScore, LONGLONG lMeCurGameReturnScore, WORD wChairID)
{
	m_lMeCurGameScore=lMeCurGameScore;			
	m_lMeCurGameReturnScore[wChairID]=lMeCurGameReturnScore;

	RefreshGameView();
}

//����ʤ��
void CGameClientView::SetWinnerSide(bool blWin[], bool bSet)
{	
	//����ʱ��
	for (int i= 0;i<AREA_COUNT;i++)
	{
		m_bWinFlag[i]=blWin[i];
	}
	if (true==bSet)
	{
		//���ö�ʱ��
		SetTimer(IDI_FLASH_WINNER,500,NULL);

		//ȫʤ�ж�
		bool blWinAll = true;

		for (int i= 0;i<AREA_COUNT;i++)
		{
			if(m_bWinFlag[i]==true)
			{
				blWinAll = false;
			}
		}
		if (blWinAll)
		{
			//������Դ
			HINSTANCE hInstance=AfxGetInstanceHandle();
		}
	}
	else 
	{
		//�����ʱ��
		KillTimer(IDI_FLASH_WINNER);

		//ȫʤ�ж�
		bool blWinAll = true;

		for (int i= 0;i<AREA_COUNT;i++)
		{

			if(m_bWinFlag[i]==true)
			{
				blWinAll = false;
			}
		}
		if (blWinAll)
		{
		}
	}

	//���ñ���
	m_bFlashResult=bSet;
	m_cbAreaFlash=0xFF;

	if ( !bSet )
		m_bShowGameResult=bSet;
	else
		SetTimer(IDI_SHOW_END, 2000, NULL);

	//���½���
	//RefreshGameView();

	return;
}

//��ȡ����
BYTE CGameClientView::GetJettonArea(CPoint MousePoint)
{
	for (int i = 0;i<AREA_COUNT;i++)
	{
		if(m_RectArea[i].PtInRect(MousePoint))
		{
			return i+1;
		}
	}
	return 0xFF;
}

// �滭����
void CGameClientView::DrawNumber(CDC * pDC, CPngImageEx* ImageNumber, TCHAR * szImageNum, SCORE lOutNum, INT nXPos, INT nYPos, UINT uFormat)
{
	TCHAR szOutNum[128] = {0};
	_sntprintf(szOutNum,CountArray(szOutNum),SCORE_STRING,lOutNum);
	DrawNumber(pDC, ImageNumber, szImageNum, szOutNum, nXPos, nYPos, uFormat);
}

// �滭����
void CGameClientView::DrawNumber(CDC * pDC, CPngImageEx* ImageNumber, TCHAR * szImageNum, TCHAR* szOutNum ,INT nXPos, INT nYPos,  UINT uFormat)
{
	// ������Դ
	INT nNumberHeight=ImageNumber->GetHeight();
	INT nNumberWidth=ImageNumber->GetWidth()/lstrlen(szImageNum);

	//��������
	CString strOutNum(szOutNum);
	int nOutNumLen=lstrlen(szOutNum);
	int nDotPos = strOutNum.Find('.');
	if(nDotPos!=-1)
	{
		if(_tcstol((LPCTSTR)&szOutNum[nDotPos+1],NULL,10)==0)
		{
			nOutNumLen=nDotPos;
		}
	}

	if ( uFormat == DT_CENTER )
	{
		nXPos -= (INT)(((DOUBLE)(nOutNumLen) / 2.0) * nNumberWidth);
	}
	else if ( uFormat == DT_RIGHT )
	{
		nXPos -= nOutNumLen * nNumberWidth;
	}

	for ( INT i = 0; i < nOutNumLen; ++i )
	{
		for ( INT j = 0; j < lstrlen(szImageNum); ++j )
		{
			if ( szOutNum[i] == szImageNum[j] && szOutNum[i] != '\0' )
			{
				ImageNumber->DrawImage(pDC, nXPos, nYPos, nNumberWidth, nNumberHeight, j * nNumberWidth, 0, nNumberWidth, nNumberHeight);
				nXPos += nNumberWidth;
				break;
			}
		}
	}
}


// �滭����
void CGameClientView::DrawNumber(CDC * pDC, CPngImage* ImageNumber, TCHAR * szImageNum, SCORE lOutNum, INT nXPos, INT nYPos, UINT uFormat)
{
	TCHAR szOutNum[128] = {0};
	_sntprintf(szOutNum,CountArray(szOutNum),SCORE_STRING,lOutNum);
	DrawNumber(pDC, ImageNumber, szImageNum, szOutNum, nXPos, nYPos, uFormat);
}

// �滭����
void CGameClientView::DrawNumber(CDC * pDC, CPngImage* ImageNumber, TCHAR * szImageNum, TCHAR* szOutNum ,INT nXPos, INT nYPos,  UINT uFormat)
{
	// ������Դ
	INT nNumberHeight=ImageNumber->GetHeight();
	INT nNumberWidth=ImageNumber->GetWidth()/lstrlen(szImageNum);

	//��������
	CString strOutNum(szOutNum);
	int nOutNumLen=lstrlen(szOutNum);
	int nDotPos = strOutNum.Find('.');
	if(nDotPos!=-1)
	{
		if(_tcstol((LPCTSTR)&szOutNum[nDotPos+1],NULL,10)==0)
		{
			nOutNumLen=nDotPos;
		}
	}

	if ( uFormat == DT_CENTER )
	{
		nXPos -= (INT)(((DOUBLE)(nOutNumLen) / 2.0) * nNumberWidth);
	}
	else if ( uFormat == DT_RIGHT )
	{
		nXPos -= nOutNumLen * nNumberWidth;
	}

	for ( INT i = 0; i < nOutNumLen; ++i )
	{
		for ( INT j = 0; j < lstrlen(szImageNum); ++j )
		{
			if ( szOutNum[i] == szImageNum[j] && szOutNum[i] != '\0' )
			{
				ImageNumber->DrawImage(pDC, nXPos, nYPos, nNumberWidth, nNumberHeight, j * nNumberWidth, 0, nNumberWidth, nNumberHeight);
				nXPos += nNumberWidth;
				break;
			}
		}
	}
}

//�滭����
//void CGameClientView::DrawNumberStringEX(CDC * pDC, LONGLONG lNumber, INT nXPos, INT nYPos,bool bChip)
//{
//	//������Դ
//	CSize SizeScoreNumber(m_ImageScoreNumberPng.GetWidth()/10,m_ImageScoreNumberPng.GetHeight());
//
//	if(bChip) SizeScoreNumber.SetSize(m_ImageChipNum.GetWidth()/10,m_ImageChipNum.GetHeight());
//
//	//������Ŀ
//	int nNumberCount=0;
//	LONGLONG lNumberTemp=lNumber;
//	do
//	{
//		nNumberCount++;
//		lNumberTemp/=10;
//	} while (lNumberTemp>0);
//
//	//λ�ö���
//	INT nYDrawPos=nYPos-SizeScoreNumber.cy/2;
//	INT nXDrawPos=0;
//	if(bChip) 
//		nXDrawPos=nXPos+nNumberCount*SizeScoreNumber.cx/2-SizeScoreNumber.cx;
//	else
//		nXDrawPos=nXPos;
//
//	//�滭����
//	for (int i=0;i<nNumberCount;i++)
//	{
//		//�滭����
//		int nCellNumber=(int)(lNumber%10);		
//		
//		if(!bChip)
//			m_ImageScoreNumberPng.DrawImage(pDC,nXDrawPos,nYDrawPos,SizeScoreNumber.cx,SizeScoreNumber.cy,	nCellNumber*SizeScoreNumber.cx,0);
//		else
//			m_ImageChipNum.DrawImage(pDC,nXDrawPos,nYDrawPos,SizeScoreNumber.cx,SizeScoreNumber.cy,	nCellNumber*SizeScoreNumber.cx,0);
//
//		//���ñ���
//		lNumber/=10;
//		nXDrawPos-=SizeScoreNumber.cx;
//	};
//
//	return;
//}
//�滭����
void CGameClientView::DrawNumberString(CDC * pDC, LONGLONG lNumber, INT nXPos, INT nYPos, bool blTimer,bool bMeScore)
{
	//������Դ
	CSize SizeScoreNumber;

	if ( bMeScore ) SizeScoreNumber.SetSize(m_ImageMeScoreNumberPng.GetWidth()/10, m_ImageMeScoreNumberPng.GetHeight());

	if(blTimer)
	{
		SizeScoreNumber.SetSize(m_TimerCount_png.GetWidth()/10, m_TimerCount_png.GetHeight());
	}

	//������Ŀ
	LONGLONG lNumberCount=0;
	LONGLONG lNumberTemp=lNumber;
	do
	{
		lNumberCount++;
		lNumberTemp/=10;
	} while (lNumberTemp>0);

	LONGLONG tmpNum= lNumberCount+lNumberCount/4;

	//λ�ö���
	INT nYDrawPos=nYPos-SizeScoreNumber.cy/2;
	INT nXDrawPos=(INT)(nXPos+tmpNum*SizeScoreNumber.cx/2-SizeScoreNumber.cx);

	if (!blTimer)
		nXDrawPos = (INT)(nXPos+(SizeScoreNumber.cx-2)*lNumberCount/2+(SizeScoreNumber.cx-5)*(lNumberCount>3?0:(lNumberCount-1)/3)/2);

	//�滭����
	for (LONGLONG i=0;i<lNumberCount;i++)
	{
		//�滭����
		if(i!=0&&i%3==0)
		{
			if(blTimer)
			{
			}
			else
			{
				nXDrawPos += 5;
				if ( bMeScore )
				{
					m_ImageMeScoreNumberPng.DrawImage(pDC,nXDrawPos,nYDrawPos,SizeScoreNumber.cx,SizeScoreNumber.cy,
						10*SizeScoreNumber.cx,0);
				}
				else
				{
					m_ImageScoreNumberPng.DrawImage(pDC,nXDrawPos,nYDrawPos,SizeScoreNumber.cx,SizeScoreNumber.cy,
						10*SizeScoreNumber.cx,0);
				}
				nXDrawPos-=(SizeScoreNumber.cx-2);
			}
		}
		LONG lCellNumber=(LONG)(lNumber%10);
		if(blTimer)
		{			
			m_TimerCount_png.DrawImage(pDC,nXDrawPos,nYDrawPos,SizeScoreNumber.cx,SizeScoreNumber.cy,lCellNumber*SizeScoreNumber.cx,0);						
		}
		else
		{
			if ( bMeScore )
			{
				m_ImageMeScoreNumberPng.DrawImage(pDC,nXDrawPos,nYDrawPos,SizeScoreNumber.cx,SizeScoreNumber.cy,
					lCellNumber*SizeScoreNumber.cx,0);
			}
			else
			{
				m_ImageScoreNumberPng.DrawImage(pDC,nXDrawPos,nYDrawPos,SizeScoreNumber.cx,SizeScoreNumber.cy,
					lCellNumber*SizeScoreNumber.cx,0);
			}

		}
		//���ñ���
		lNumber/=10;

		if (blTimer)
			nXDrawPos-=SizeScoreNumber.cx;
		else
			nXDrawPos-=(SizeScoreNumber.cx-2);
	};

	return;
}

//�滭����
void CGameClientView::DrawNumberStringWithSpace(CDC * pDC, LONGLONG lNumber, INT nXPos, INT nYPos)
{
	COLORREF CurSelectColor=RGB(255,234,0);					//��ǰ��ɫ
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

	CDFontEx::DrawText(this,pDC,  12, 400, strNumber, nXPos,nYPos, CurSelectColor, DT_END_ELLIPSIS|DT_LEFT|DT_TOP|DT_SINGLELINE);
}
//�滭����
CString CGameClientView::NumberStringWithSpace(LONGLONG lNumber)
{

	CString static strNumber=TEXT(""), strTmpNumber1,strTmpNumber2;
	strTmpNumber1.Empty();
	strTmpNumber2.Empty();
	strNumber.Empty();
	if (lNumber==0) strNumber=TEXT("0");
	int nNumberCount=0;
	LONGLONG lTmpNumber=lNumber;
	if (lNumber<0) lNumber=-lNumber;
	bool blfirst = true;
	while (lNumber>0)
	{
		strTmpNumber1.Format(TEXT("%ld"),lNumber%10);
		nNumberCount++;
		strTmpNumber2 = strTmpNumber1+strTmpNumber2;

		if (nNumberCount==3)
		{
			if(blfirst)
			{
				strTmpNumber2 +=strNumber;
				blfirst = false;

			}else
			{
				strTmpNumber2 += (TEXT(",") +strNumber);
			}

			strNumber=strTmpNumber2;
			nNumberCount=0;
			strTmpNumber2=TEXT("");
		}
		lNumber/=10;
	}

	if (strTmpNumber2.IsEmpty()==FALSE)
	{
		if(blfirst==false)
			strTmpNumber2 += (TEXT(",") +strNumber);
		else
			strTmpNumber2 += strNumber;

		strNumber=strTmpNumber2;
	}

	if (lTmpNumber<0) strNumber=TEXT("-")+strNumber;

	return  strNumber;


}
//������
void CGameClientView::IsJettonDraw(CDC *pDC)
{
	int nTime = GetTickCount();

	//��������
	CRgn JettonRegion;				//��ʾ����
	CRgn SignedRegion;				//��������
	CRgn SignedAndJettonRegion;		//��������
	SignedAndJettonRegion.CreateRectRgn(0,0,0,0);

	//ͼƬ����
	CSize static SizeJettonItem(m_ImageJettonView.GetWidth()/6,m_ImageJettonView.GetHeight());

	//��������
	for (INT i = 0;i < AREA_COUNT; i++)
	{
		for (INT_PTR j = m_JettonInfoArray[i].GetCount() - 1; j >= 0; j--)
		{
			//��ȡ��Ϣ
			tagJettonInfo * pJettonInfo = &m_JettonInfoArray[i][j];
			pJettonInfo->bShow = false;
			CRect rcRect(pJettonInfo->nXPos+m_PointJetton[i].x,
				pJettonInfo->nYPos+m_PointJetton[i].y +16,
				pJettonInfo->nXPos+m_PointJetton[i].x + m_ImageJettonView.GetWidth()/6,
				pJettonInfo->nYPos+m_PointJetton[i].y +16 + m_ImageJettonView.GetHeight());

			//�״�����
			if( !JettonRegion.GetSafeHandle() ) 
			{
				JettonRegion.CreateEllipticRgnIndirect(rcRect);
				pJettonInfo->bShow = true;
				continue;
			}

			//�����ж�
			SignedRegion.CreateEllipticRgnIndirect(rcRect);
			SignedAndJettonRegion.CombineRgn(&JettonRegion,&SignedRegion, RGN_AND);
			if( SignedAndJettonRegion.EqualRgn(&SignedRegion) )
			{
				pJettonInfo->bShow = false;
			}
			else
			{
				pJettonInfo->bShow = true;
				JettonRegion.CombineRgn(&JettonRegion,&SignedRegion,RGN_OR);
			}

			//�ϲ�����
			SignedRegion.DeleteObject();
		}

		//ɾ����ʾ����
		if( JettonRegion.GetSafeHandle() )
			JettonRegion.DeleteObject();
	}

	//ɾ������
	if( JettonRegion.GetSafeHandle() )
		JettonRegion.DeleteObject();
	if( SignedRegion.GetSafeHandle() )
		SignedRegion.DeleteObject();
	if( SignedAndJettonRegion.GetSafeHandle() )
		SignedAndJettonRegion.DeleteObject();

	//�滭����
	int iJettonCount=0;
	for (INT i=0;i<AREA_COUNT;i++)
	{
		for (INT_PTR j=0;j<m_JettonInfoArray[i].GetCount();j++)
		{
			//��ȡ��Ϣ
			tagJettonInfo * pJettonInfo=&m_JettonInfoArray[i][j];
			if(!pJettonInfo->bShow)
				continue;

			iJettonCount++;

			//�滭����
			m_ImageJettonView.TransDrawImage(pDC,pJettonInfo->nXPos+m_PointJetton[i].x,
				pJettonInfo->nYPos+m_PointJetton[i].y+16,SizeJettonItem.cx,SizeJettonItem.cy,
				pJettonInfo->cbJettonIndex*SizeJettonItem.cx,0,RGB(255,0,255));


		}
	}

//#ifdef _DEBUG
	////�滭����
	//DrawNumberString(pDC,iJettonCount,50,500);
	//DrawNumberString(pDC,GetTickCount() - nTime,50,550);
//#endif


	////��������
	//JettonRegion.DeleteObject();
	//SignedRegionOne.DeleteObject();
}
//�滭����
void CGameClientView::DrawNumberStringWithSpace(CDC * pDC, LONGLONG lNumber, CRect rcRect, INT nFormat)
{
	LONGLONG lTmpNumber = lNumber;
	CString strNumber;
	CString strTmpNumber1;
	CString strTmpNumber2;
	bool blfirst = true;
	bool bLongNum = false;
	int nNumberCount = 0;

	strTmpNumber1.Empty();
	strTmpNumber2.Empty();
	strNumber.Empty();

	if ( lNumber == 0 )
		strNumber=TEXT("0");

	if ( lNumber < 0 ) 
		lNumber =- lNumber;

	if( lNumber >= 100 )
		bLongNum = true;

	while(lNumber > 0)
	{
		strTmpNumber1.Format(TEXT("%I64d"),lNumber%10);
		nNumberCount++;
		strTmpNumber2 = strTmpNumber1+strTmpNumber2;

		if ( nNumberCount == 3 )
		{
			if(blfirst)
			{
				strTmpNumber2 += (TEXT("") +strNumber);
				blfirst = false;
			}
			else
			{
				strTmpNumber2 += (TEXT(",") +strNumber);
			}

			strNumber = strTmpNumber2;
			nNumberCount = 0;
			strTmpNumber2 = TEXT("");
		}
		lNumber /= 10;
	}

	if ( strTmpNumber2.IsEmpty() == FALSE )
	{
		if( bLongNum )
			strTmpNumber2 += (TEXT(",") +strNumber);
		else
			strTmpNumber2 += strNumber;

		strNumber = strTmpNumber2;
	}

	if ( lTmpNumber < 0 ) 
		strNumber = TEXT("-") + strNumber;
	//�������
	//if (nFormat==-1) pDC->DrawText(strNumber,rcRect,DT_END_ELLIPSIS|DT_LEFT|DT_TOP|DT_SINGLELINE);
	//else pDC->DrawText(strNumber,rcRect,nFormat);

	//�������
	if (nFormat==-1) 
		CDFontEx::DrawText(this,pDC,  12, 400, strNumber, rcRect, RGB(255,255,255), DT_END_ELLIPSIS|DT_LEFT|DT_TOP|DT_SINGLELINE);
	else 
		CDFontEx::DrawText(this,pDC,  12, 400, strNumber, rcRect, RGB(255,255,255), nFormat);
}
void CGameClientView::SetMoveCardTimer()
{
	ZeroMemory(m_cbResulte,sizeof(m_cbResulte));
	KillTimer(IDI_EFFECT);
	m_bEffect=false;
	m_nEffectIndex=0;
}
void CGameClientView::KillCardTime()
{
	KillTimer(IDI_FLASH_WINNER);
	KillTimer(IDI_DISPATCH_CARD);
	KillTimer(IDI_SHOWDISPATCH_CARD_TIP);
	KillTimer(IDI_GUESS);	
}
void CGameClientView::StartRunCar(int iTimer)
{
	m_bFlashrandShow = false;
	KillTimer(IDI_FLASH_RAND_SIDE);
	KillTimer(IDI_FLASH_CARD);
	KillTimer(IDI_SHOW_CARD_RESULT);
	iTimerStep = 400;
	SetTimer(IDI_FLASH_CARD,iTimer,NULL);
	ifirstTimer = 0;
	iOpenSide = m_cbTableCardArray[0];
	iTotoalRun = iOpenSide+24*3-2;
	iRunIndex = 0;
	m_CarIndex = 1;
	m_blRungingCar = true;	
}

void CGameClientView::StartLuckyCar(int iTimer,BYTE cbIndex)
{
	m_bFlashrandShow = false;
	KillTimer(IDI_FLASH_RAND_SIDE);
	KillTimer(IDI_FLASH_CARD);	
	iTimerStep = 400;
	SetTimer(IDI_FLASH_CARD,iTimer,NULL);
	ifirstTimer = 0;
	if(m_cbTableCardArray[cbIndex]!=0)
		iOpenSide = m_cbTableCardArray[cbIndex];
	iTotoalRun = (24-m_cbTableCardArray[cbIndex-1]+1)+iOpenSide+24*3-2;
	iRunIndex = 0;
	m_CarIndex = m_cbTableCardArray[cbIndex-1];
	m_blRungingCar = true;
	//m_blMoveFinish=false;
	m_bEffect=false;
}
void CGameClientView::RuningCar(int iTimer)
{
	if(iRunIndex<10)
	{
		iTimerStep-=41;
	}
	if(iRunIndex >= iTotoalRun-15)
	{
		iTimerStep+=45 /*8*(iRunIndex-iTotoalRun+15)*/;
	}
	if(iRunIndex==iTotoalRun)
	{
		BYTE  bcResulteOut[AREA_COUNT+1];
		memset(bcResulteOut,0,AREA_COUNT+1);
		if(m_nProbability==1)//��ǹ
		{
			KillTimer(IDI_FLASH_CARD);
			m_blRungingCar = false;
			m_blMoveFinish = true;
			//���ö�ʱ��
			this->DispatchCard();
			FinishDispatchCard(true,m_bShowLeaveHandleIndex);
			m_bShowJettonAn = true;
			SetTimer(IDI_EFFECT,300,NULL);
			m_bEffect=true;
			if((m_bShowJettonIndex-1)==m_bShowLeaveHandleIndex)
			{
				m_nProbability=0;
			}
			m_GameLogic.GetCardTypeEx(m_cbTableCardArray[m_bShowLeaveHandleIndex],1,bcResulteOut);
			for(int k=0;k<=AREA_COUNT;k++)
			{
				if(bcResulteOut[k]>0)
					SendEngineMessage(IDM_SOUND,k,k);
			}
			SetTimer(IDI_SHOW_CARD_RESULT,1000,NULL);
		}
		else if(m_nProbability==2)//����Ԫ
		{
			KillTimer(IDI_FLASH_CARD);
			m_blRungingCar = false;
			m_blMoveFinish = true;
			m_nProbability=0;
			//���ö�ʱ��
			this->DispatchCard();
			FinishDispatchCard(true,m_bShowLeaveHandleIndex);
			m_bShowJettonAn = true;
			SetTimer(IDI_EFFECT,300,NULL);
			m_bEffect=true;
			SendEngineMessage(IDM_SOUND,9,9);
			SetTimer(IDI_OPENCARD,1000,NULL);
		}
		else if(m_nProbability==3)//С��Ԫ
		{
			KillTimer(IDI_FLASH_CARD);
			m_blRungingCar = false;
			m_blMoveFinish = true;
			m_nProbability=0;
			//���ö�ʱ��
			this->DispatchCard();
			FinishDispatchCard(true,m_bShowLeaveHandleIndex);
			m_bShowJettonAn = true;
			SetTimer(IDI_EFFECT,300,NULL);
			m_bEffect=true;
			SendEngineMessage(IDM_SOUND,10,10);
			SetTimer(IDI_POSTCARD,1000,NULL);
		}
		else if(m_nProbability==4)//����ϲ
		{
			KillTimer(IDI_FLASH_CARD);
			m_blRungingCar = false;
			m_blMoveFinish = true;
			m_nProbability=0;
			//���ö�ʱ��
			this->DispatchCard();
			FinishDispatchCard(true,m_bShowLeaveHandleIndex);
			m_bShowJettonAn = true;
			SetTimer(IDI_EFFECT,300,NULL);
			m_bEffect=true;
			SendEngineMessage(IDM_SOUND,11,11);
			SetTimer(IDI_OUT_CARD_TIME,1000,NULL);
		}
		else if(m_nProbability==5)//ͨ��
		{
			KillTimer(IDI_FLASH_CARD);
			m_blRungingCar = false;
			m_blMoveFinish = true;
			m_nProbability=0;
			//���ö�ʱ��
			this->DispatchCard();
			FinishDispatchCard(true,m_bShowLeaveHandleIndex);
			SendEngineMessage(IDM_SOUND,14,14);			
		}
		else
		{
			KillTimer(IDI_FLASH_CARD);
			m_blRungingCar = false;
			m_blMoveFinish = true;
			//���ö�ʱ��
			this->DispatchCard();
			FinishDispatchCard(true,m_bShowLeaveHandleIndex);
			m_bShowJettonAn = true;
			SetTimer(IDI_EFFECT,300,NULL);
			m_bEffect=true;
			m_GameLogic.GetCardTypeEx(m_cbTableCardArray[m_bShowLeaveHandleIndex],1,bcResulteOut);
			for(int k=0;k<AREA_COUNT;k++)
			{
				if(bcResulteOut[k]>0)
					SendEngineMessage(IDM_SOUND,k,k);
			}
			m_bShowLeaveHandleIndex=0;
		}		
		return ;
	}
	if(iTimerStep<0)
	{
		return ;
	}
	KillTimer(IDI_FLASH_CARD);
	SetTimer(IDI_FLASH_CARD,iTimer,NULL);

}
void CGameClientView::StartFlashTip(bool bShow,bool bWin)
{
	//��ȡ״̬
	BYTE cbGameStatus=0;
	if(m_pGameClientDlg != NULL)
		cbGameStatus=m_pGameClientDlg->GetGameStatus();
	KillTimer(IDI_FLASH_TIP);
	KillTimer(IDI_GUESS);
	m_bTip=true;
	if(cbGameStatus==GS_GAME_FREE)
	{
		m_ImageTip.DestroyImage();
		m_ImageTip.LoadImage(AfxGetInstanceHandle(),TEXT("TREE_TIP"));
	}
	if(cbGameStatus==GS_PLACE_JETTON)
	{
		m_ImageTip.DestroyImage();
		m_ImageTip.LoadImage(AfxGetInstanceHandle(),TEXT("JETTON_TIP"));
	}
	if(cbGameStatus==GS_COMPARE_END)
	{
		m_ImageTip.DestroyImage();
		m_ImageTip.LoadImage(AfxGetInstanceHandle(),TEXT("GUESS_TIP"));
		SetTimer(IDI_GUESS,50,NULL);
	}
	if (bShow)
	{
		if (bWin)
		{
			m_ImageTip.DestroyImage();
			m_ImageTip.LoadImage(AfxGetInstanceHandle(),TEXT("GUESS_WIN"));
		}
		else
		{
			m_ImageTip.DestroyImage();
			m_ImageTip.LoadImage(AfxGetInstanceHandle(),TEXT("GUESS_LOSE"));
		}
	}
	SetTimer(IDI_FLASH_TIP,500,NULL);
}
void CGameClientView::StartRandShowSide()
{
	KillTimer(IDI_FLASH_RAND_SIDE);
	iTimerStep = 200;
	m_bFlashrandShow = true;
	SetTimer(IDI_FLASH_RAND_SIDE,iTimerStep,NULL);
}
//��ʱ����Ϣ
void CGameClientView::OnTimer(UINT nIDEvent)
{
	if(IDI_GUESS==nIDEvent)
	{
		m_nGuessNum=rand()%14+1;
		//���½���
		RefreshGameView();
	}
	if(IDI_FLASH_TIP==nIDEvent)
	{
		m_nFlashTipIndex++;
		if(m_nFlashTipIndex>=2)
			m_nFlashTipIndex=0;
		//���½���
		RefreshGameView();
	}
	if(IDI_FLASH_CARD == nIDEvent)
	{
		m_CarIndex = (m_CarIndex+1)%24;
		iRunIndex++;
		RuningCar(iTimerStep);
		SendEngineMessage(IDM_SOUND,13,13);

		//���½���
		RefreshGameView();
	}
	if(IDI_SHOW_CARD_RESULT==nIDEvent)
	{
		m_bOPenBoxIndex++;
		if(m_bOPenBoxIndex>=5)
		{
			m_bShowLeaveHandleIndex++;
			m_bOPenBoxIndex=0;
			KillTimer(IDI_SHOW_CARD_RESULT);
			DispatchCard();
			StartLuckyCar(20,m_bShowLeaveHandleIndex);
		}
		//���½���
		RefreshGameView();
	}
	if(IDI_OPENCARD==nIDEvent)//����Ԫ
	{
		m_bOPenBoxIndex++;
		if(m_bOPenBoxIndex==4)
		{
			KillTimer(IDI_EFFECT);
			m_bEffect=false;
		}
		if(m_bOPenBoxIndex>=5)
		{
			m_bOPenBoxIndex=0;
			m_nProbability=2;
			KillTimer(IDI_OPENCARD);			
			for(int i=1;i<=4;i++)
			{
				DispatchCard();
				FinishDispatchCard(false,i);
			}
			SetTimer(IDI_EFFECT,300,NULL);			
			m_bEffect=true;
		}
		//���½���
		RefreshGameView();
	}
	if(IDI_POSTCARD==nIDEvent)//С��Ԫ
	{
		m_bOPenBoxIndex++;
		if(m_bOPenBoxIndex==4)
		{
			KillTimer(IDI_EFFECT);
			m_bEffect=false;
		}
		if(m_bOPenBoxIndex>=5)
		{
			m_bOPenBoxIndex=0;
			m_nProbability=3;
			KillTimer(IDI_POSTCARD);			
			for(int i=1;i<=4;i++)
			{
				DispatchCard();
				FinishDispatchCard(false,i);
			}
			SetTimer(IDI_EFFECT,300,NULL);			
			m_bEffect=true;
		}
		//���½���
		RefreshGameView();
	}
	if(IDI_OUT_CARD_TIME==nIDEvent)//����ϲ
	{
		m_bOPenBoxIndex++;
		if(m_bOPenBoxIndex==4)
		{
			KillTimer(IDI_EFFECT);
			m_bEffect=false;
		}
		if(m_bOPenBoxIndex>=5)
		{
			m_bOPenBoxIndex=0;
			m_nProbability=4;
			KillTimer(IDI_OUT_CARD_TIME);			
			for(int i=1;i<=4;i++)
			{
				DispatchCard();
				FinishDispatchCard(false,i);
			}
			SetTimer(IDI_EFFECT,300,NULL);			
			m_bEffect=true;
		}
		//���½���
		RefreshGameView();
	}
	if(IDI_FLASH_RAND_SIDE ==nIDEvent )
	{
		m_bFlashrandShow=!m_bFlashrandShow;
		m_CarIndex = rand()%24;
		//���½���
		RefreshGameView();
	}
	//����ʤ��
	if (nIDEvent==IDI_FLASH_WINNER)
	{
		//���ñ���
		m_bFlashResult=!m_bFlashResult;		
		return;
	}
	if(IDI_EFFECT==nIDEvent)
	{
		m_nEffectIndex++;
		if (m_nProbability == 2 || m_nProbability == 3 || m_nProbability == 4)
		{
			if(m_nEffectIndex>=5)
				m_nEffectIndex=0;
		}
		else
		{
			if(m_nEffectIndex>=3)
				m_nEffectIndex=0;
		}		

		//���½���
		RefreshGameView();
		return;
	}
	//�ֻ�ׯ��
	else if ( nIDEvent == IDI_SHOW_CHANGE_BANKER )
	{
		ShowChangeBanker( false );
		return;
	}
	else if (IDI_SHOWDISPATCH_CARD_TIP==nIDEvent)
	{
		SetDispatchCardTip(enDispatchCardTip_NULL);
	}
	else if ( nIDEvent == IDI_ANDROID_BET )
	{
		//���½���
		RefreshGameView();
		return;
	}
	else if ( nIDEvent >= (UINT)(IDI_ANDROID_BET + 1) && nIDEvent < (UINT)(IDI_ANDROID_BET + m_ArrayAndroid.GetCount() + 1) )
	{
		INT_PTR Index = nIDEvent - IDI_ANDROID_BET - 1;
		if (Index < 0 || Index >= m_ArrayAndroid.GetCount())
		{
			ASSERT(FALSE);
			KillTimer(nIDEvent);
			return;
		}

		if ( m_ArrayAndroid[Index].lJettonScore > 0 )
		{
			LONGLONG lScoreIndex[] = {5000000L,1000000L,100000L,10000L,1000L,100L};
			BYTE cbViewIndex = m_ArrayAndroid[Index].cbJettonArea;

			//���ӳ���
			for (BYTE i=0;i<CountArray(lScoreIndex);i++)
			{
				if(  lScoreIndex[i] > m_lRobotMaxJetton)
					continue;

				if ( m_ArrayAndroid[Index].lJettonScore >= lScoreIndex[i] )
				{
					m_ArrayAndroid[Index].lJettonScore -= lScoreIndex[i];
					m_lAllJettonScore[cbViewIndex] += lScoreIndex[i];

					tagJettonInfo JettonInfo;
					int iWSize = m_RectArea[cbViewIndex-1].right - m_RectArea[cbViewIndex-1].left - 60;
					int iHSize = m_RectArea[cbViewIndex-1].bottom - m_RectArea[cbViewIndex-1].top - 95;
					JettonInfo.nXPos=rand()%(iWSize);
					JettonInfo.nYPos=rand()%(iHSize);
					JettonInfo.cbJettonIndex = JETTON_COUNT - i - 1;

					//��������
					m_JettonInfoArray[cbViewIndex-1].Add(JettonInfo);
					//��������
					//PostEngineMessage(IDM_SOUND,12,12);
					break;
				}
			}
		}
		return;
	}
	else if ( IDI_SHOW_END == nIDEvent )
	{
		KillTimer(nIDEvent);

		BYTE cbGameStatus=0;
		if(m_pGameClientDlg != NULL)
		{
			cbGameStatus = m_pGameClientDlg->GetGameStatus();

			if( cbGameStatus == GS_GAME_END )
			{
				m_bShowGameResult=true;
			}
		}
	}

	__super::OnTimer(nIDEvent);
}

//�����Ϣ
void CGameClientView::OnLButtonDown(UINT nFlags, CPoint Point)
{
	//if (m_lCurrentJetton!=0L)
	//{
	//	int iTimer = 1;
	//	//��ע����
	//	BYTE cbJettonArea=GetJettonArea(Point);

	//	//�����ע
	//	LONGLONG lMaxJettonScore=GetUserMaxJetton(cbJettonArea);

	//	if((m_lAllJettonScore[cbJettonArea]+m_lCurrentJetton)>m_lAreaLimitScore)
	//	{		
	//		return ;
	//	}

	//	//�Ϸ��ж�
	//	if (lMaxJettonScore < m_lCurrentJetton)
	//	{
	//		SetJettonHide(0);
	//		return ;
	//	}

	//	//������Ϣ
	//	if (cbJettonArea!=0xFF) SendEngineMessage(IDM_PLACE_JETTON,cbJettonArea,(LPARAM)(&m_lCurrentJetton));
	//}
	//RefreshGameView();

	__super::OnLButtonDown(nFlags,Point);
}
void CGameClientView::OnLButtonUp(UINT nFlags, CPoint Point)
{
	__super::OnLButtonUp(nFlags,Point);
}
void CGameClientView::SetEnablePlaceJetton(bool bEnablePlaceJetton)
{
	m_bEnablePlaceJetton = bEnablePlaceJetton;
}
void CGameClientView::UpdataJettonButton()
{
	if(m_CurArea==0xFF||m_bEnablePlaceJetton == false)
	{
		return ;
	}
	//�������
	LONGLONG lCurrentJetton=GetCurrentJetton();
	LONGLONG lLeaveScore=m_lMeMaxScore;
	for (int nAreaIndex=1; nAreaIndex<=AREA_COUNT; ++nAreaIndex) lLeaveScore -= m_lUserJettonScore[nAreaIndex];
	//�����ע
	LONGLONG lUserMaxJetton = 0;

	lUserMaxJetton = GetUserMaxJetton(m_CurArea);

	lLeaveScore = min((lLeaveScore),lUserMaxJetton); //�û����·� �����ֱȽ� �������屶 


	lCurrentJetton = m_lLastJetton ;

	//���ù��
	if (lCurrentJetton>lLeaveScore)
	{
		if (lLeaveScore>=5000000L) SetCurrentJetton(5000000L);
		else if (lLeaveScore>=1000000L) SetCurrentJetton(1000000L);
		else if (lLeaveScore>=100000L) SetCurrentJetton(100000L);
		else if (lLeaveScore>=10000L) SetCurrentJetton(10000L);
		else if (lLeaveScore>=1000L) SetCurrentJetton(1000L);
		else if (lLeaveScore>=100L) SetCurrentJetton(100L);
		else SetCurrentJetton(0L);
	}
	else
	{
		SetCurrentJetton(m_lLastJetton);
	}
}
//����ƶ���Ϣ
void CGameClientView::OnMouseMove(UINT nFlags, CPoint point)
{
	return __super::OnMouseMove(nFlags,point);
}
//�����Ϣ
void CGameClientView::OnRButtonDown(UINT nFlags, CPoint Point)
{
	//���ñ���
	m_lCurrentJetton=0L;

	if(m_pGameClientDlg != NULL)
	{
		if (m_pGameClientDlg->GetGameStatus()!=GS_GAME_END && m_cbAreaFlash!=0xFF)
		{
			m_cbAreaFlash=0xFF;
			SetJettonHide(0);
			RefreshGameView();
		}
	}
	m_lLastJetton = 0L;

	__super::OnLButtonDown(nFlags,Point);
}
void CGameClientView::SetJettonHide(int ID)
{
	
}
//�����Ϣ
BOOL CGameClientView::OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage)
{
	return __super::OnSetCursor(pWnd, nHitTest, uMessage);
}

//�ֻ�ׯ��
void CGameClientView::ShowChangeBanker( bool bChangeBanker )
{
	//�ֻ�ׯ��
	if ( bChangeBanker )
	{
		SetTimer( IDI_SHOW_CHANGE_BANKER, 3000, NULL );
		m_bShowChangeBanker = true;
	}
	else
	{
		KillTimer( IDI_SHOW_CHANGE_BANKER );
		m_bShowChangeBanker = false ;
	}

	//���½���
	RefreshGameView();
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
void CGameClientView::DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, int nXPos, int nYPos)
{
	//��������
	int nStringLength=lstrlen(pszString);
	int nXExcursion[8]={1,1,1,0,-1,-1,-1,0};
	int nYExcursion[8]={-1,0,1,1,1,0,-1,-1};

	//�滭�߿�
	//pDC->SetTextColor(crFrame);
	for (int i=0;i<CountArray(nXExcursion);i++)
	{
		//pDC->TextOut(nXPos+nXExcursion[i],nYPos+nYExcursion[i],pszString,nStringLength);
		CDFontEx::DrawText(this,pDC,  nStringLength, 12, pszString, nXPos+nXExcursion[i], nYPos+nYExcursion[i], crFrame, TA_LEFT|TA_TOP);
	}

	//�滭����
	//pDC->SetTextColor(crText);
	//pDC->TextOut(nXPos,nYPos,pszString,nStringLength);
	CDFontEx::DrawText(this,pDC,  nStringLength, 12, pszString, nXPos, nYPos, crText, TA_LEFT|TA_TOP);

	return;
}
//��������
void CGameClientView::DrawTextString( CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, CRect rcRect, UINT nDrawFormat, bool bBigFont )
{
	//��������
	INT nStringLength=lstrlen(pszString);
	INT nXExcursion[8]={1,1,1,0,-1,-1,-1,0};
	INT nYExcursion[8]={-1,0,1,1,1,0,-1,-1};

	//�滭�߿�
	for (INT i=0;i<CountArray(nXExcursion);i++)
	{
		//����λ��
		CRect rcFrame;
		rcFrame.top=rcRect.top+nYExcursion[i];
		rcFrame.left=rcRect.left+nXExcursion[i];
		rcFrame.right=rcRect.right+nXExcursion[i];
		rcFrame.bottom=rcRect.bottom+nYExcursion[i];

		if (bBigFont)
		{
			//�滭�ַ�
			CDFontEx::DrawText(this,pDC,  16, 400, pszString, rcFrame, crFrame, nDrawFormat);
		}
		else
		{
			//�滭�ַ�
			CDFontEx::DrawText(this,pDC,  12, 400, pszString, rcFrame, crFrame, nDrawFormat);
		}
		
	}

	if (bBigFont)
	{
		//�滭�ַ�
		CDFontEx::DrawText(this,pDC,  16, 400, pszString, rcRect, crText, nDrawFormat);
	} 
	else
	{
		//�滭�ַ�
		CDFontEx::DrawText(this,pDC,  12, 400, pszString, rcRect, crText, nDrawFormat);
	}
	
}
//ׯ����Ϣ
void CGameClientView::SetBankerInfo(DWORD dwBankerUserID, LONGLONG lBankerScore) 
{
	//ׯ�����Ӻ�
	WORD wBankerUser=INVALID_CHAIR;

	//�������Ӻ�
	if (INVALID_CHAIR!=dwBankerUserID)
	{
		for (WORD wChairID=0; wChairID<MAX_CHAIR; ++wChairID)
		{
			IClientUserItem *pClientUserItem=GetClientUserItem(wChairID);
			if(pClientUserItem == NULL) continue;
			if (dwBankerUserID==pClientUserItem->GetChairID())
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
void CGameClientView::DrawMoveInfo(CDC* pDC,CRect rcRect)
{
	return ;
}
//�滭��ʶ
void CGameClientView::DrawWinFlags(CDC * pDC)
{
	//�ǿ��ж�
	//m_nRecordLast = 14;
	if (m_nRecordLast==m_nRecordFirst) return;

	//��Դ����

	int nIndex = m_nScoreHead;
	COLORREF static clrOld ;

	clrOld = pDC->SetTextColor(RGB(52,116,23));
	int nDrawCount=0;
	while ( nIndex != m_nRecordLast && ( m_nRecordLast!=m_nRecordFirst ) && nDrawCount < MAX_FALG_COUNT )
	{
		//ʤ����ʶ
		tagClientGameRecord &ClientGameRecord = m_GameRecordArrary[nIndex];

		for (int i=1; i<=1; ++i)
		{
			//λ�ñ���
			int static nYPos=0,nXPos=0;
			nXPos=m_nWinFlagsExcursionX+i*27-i*1;
			nYPos=m_nWinFlagsExcursionY + ((nIndex - m_nScoreHead + MAX_SCORE_HISTORY) % MAX_SCORE_HISTORY) * (42);

			//ʤ����ʶ
			int static nFlagsIndex=0;
			for (int i = 0;i<=AREA_COUNT;i++)
			{
				if(ClientGameRecord.enOperateMen[i]==enOperateResult_Big)
					nFlagsIndex=8+i;
				if(ClientGameRecord.enOperateMen[i]==enOperateResult_Small)
					nFlagsIndex=i;
				if(ClientGameRecord.enOperateMen[i]==enOperateResult_GreenLucky)
					nFlagsIndex=16;
				if(ClientGameRecord.enOperateMen[i]==enOperateResult_RedLucky)
					nFlagsIndex=17;
			}			
					
			//�滭��ʶ
			m_ImageWinFlags.DrawImage( pDC, nXPos, nYPos, m_ImageWinFlags.GetWidth()/18,m_ImageWinFlags.GetHeight(),m_ImageWinFlags.GetWidth()/18 * nFlagsIndex, 0);
		}

		//�ƶ��±�
		nIndex = (nIndex+1) % MAX_SCORE_HISTORY;
		nDrawCount++;
	}
	pDC->SetTextColor(clrOld);
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

	//���½���
	RefreshGameView();
}

//�ƶ���ť
void CGameClientView::OnScoreMoveR()
{
	int nHistoryCount = ( m_nRecordLast - m_nScoreHead + MAX_SCORE_HISTORY ) % MAX_SCORE_HISTORY;
	if ( nHistoryCount == MAX_FALG_COUNT ) return;

	m_nScoreHead = ( m_nScoreHead + 1 ) % MAX_SCORE_HISTORY;
	if ( nHistoryCount-1 == MAX_FALG_COUNT ) m_btScoreMoveR.EnableWindow(FALSE);

	m_btScoreMoveL.EnableWindow(TRUE);

	//���½���
	RefreshGameView();
}

//��ʾ���
void CGameClientView::ShowGameResult(CDC *pDC, int nWidth, int nHeight)
{

	if (false==m_bShowGameResult) return;

	int	nXPos = nWidth / 2 - 200+20+30;
	int	nYPos = nHeight / 2 - 208;
	m_pngGameEnd.DrawImage( pDC, nXPos+2-20-30, nYPos+60,
		m_pngGameEnd.GetWidth(), m_pngGameEnd.GetHeight(),0, 0);
	pDC->SetTextColor(RGB(255,255,255));


	CRect rcMeWinScore, rcMeReturnScore;
	rcMeWinScore.left = nXPos+2 + 40+30;
	rcMeWinScore.top = nYPos+70 + 32+10+10;
	rcMeWinScore.right = rcMeWinScore.left + 111;
	rcMeWinScore.bottom = rcMeWinScore.top + 34;

	rcMeReturnScore.left = nXPos+2 + 150+50;
	rcMeReturnScore.top = nYPos+70 + 32+10+10;
	rcMeReturnScore.right = rcMeReturnScore.left + 111;
	rcMeReturnScore.bottom = rcMeReturnScore.top + 34;

	CString strMeGameScore, strMeReturnScore;
	DrawNumberStringWithSpace(pDC,m_lMeCurGameScore,rcMeWinScore, DT_END_ELLIPSIS | DT_VCENTER | DT_CENTER | DT_SINGLELINE);
	//DrawNumberStringWithSpace(pDC,m_lMeCurGameReturnScore,rcMeReturnScore, DT_END_ELLIPSIS | DT_VCENTER | DT_CENTER | DT_SINGLELINE);

	CRect rcBankerWinScore;
	rcBankerWinScore.left = nXPos+2 + 40+30;
	rcBankerWinScore.top =  nYPos+70 + 32+10+35+10;
	rcBankerWinScore.right = rcBankerWinScore.left + 111;
	rcBankerWinScore.bottom = rcBankerWinScore.top + 34;

	CString strBankerCurGameScore;
	strBankerCurGameScore.Format(TEXT("%I64d"), m_lBankerCurGameScore);
	DrawNumberStringWithSpace(pDC,m_lBankerCurGameScore,rcBankerWinScore, DT_END_ELLIPSIS | DT_VCENTER | DT_CENTER | DT_SINGLELINE);
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
void   CGameClientView::GetAllWinArea(BYTE bcWinArea[],BYTE bcAreaCount,BYTE InArea)
{
	if (InArea==0xFF)
	{
		return ;
	}
	ZeroMemory(bcWinArea,bcAreaCount);


	LONGLONG lMaxSocre = 0;

	for (int i = 0;i<24;i++)
	{
		BYTE bcOutCadDataWin[AREA_COUNT+1];
		BYTE bcData[1];
		bcData[0]=i+1;
		m_GameLogic.GetCardType(bcData[0],1,bcOutCadDataWin);
		for (int j= 0;j<AREA_COUNT;j++)
		{

			if(bcOutCadDataWin[j]>0&&j==InArea-1)
			{
				LONGLONG Score = 0; 
				for (int nAreaIndex=1; nAreaIndex<=AREA_COUNT; ++nAreaIndex) 
				{
					if(bcOutCadDataWin[nAreaIndex-1]>0)
					{
						Score += m_lAllJettonScore[nAreaIndex]*(bcOutCadDataWin[nAreaIndex-1]);
					}
				}
				if(Score>=lMaxSocre)
				{
					lMaxSocre = Score;
					CopyMemory(bcWinArea,bcOutCadDataWin,bcAreaCount);

				}
				break;
			}
		}
	}
}
//�����ע
LONGLONG CGameClientView::GetUserMaxJetton(BYTE cbJettonArea)
{
	if (cbJettonArea==0xFF)
		return 0;

	//����ע��
	LONGLONG lNowJetton = 0;
	ASSERT(AREA_COUNT<=CountArray(m_lUserJettonScore));
	for (int nAreaIndex=1; nAreaIndex<=AREA_COUNT; ++nAreaIndex) lNowJetton += m_lUserJettonScore[nAreaIndex];
	//ׯ�ҽ��
	LONGLONG lBankerScore = 0x7fffffffffffffff;
	if (m_wBankerUser!=INVALID_CHAIR)
		lBankerScore = m_lBankerScore;

	//BYTE bcWinArea[AREA_COUNT];
	//LONGLONG LosScore = 0;
	//LONGLONG WinScore = 0;

	//GetAllWinArea(bcWinArea,AREA_COUNT,cbJettonArea);

	//for (int nAreaIndex=1; nAreaIndex<=AREA_COUNT; ++nAreaIndex) 
	//{
	//	if(bcWinArea[nAreaIndex-1]>0)
	//	{
	//		LosScore+=m_lAllJettonScore[nAreaIndex]*(bcWinArea[nAreaIndex-1]);
	//	}
	//	else
	//	{
	//		//if(bcWinArea[nAreaIndex-1]==0)
	//		{
	//			WinScore+=m_lAllJettonScore[nAreaIndex];
	//		}
	//	}
	//}

	//LONGLONG lTemp = lBankerScore;
	//lBankerScore = lBankerScore + WinScore - LosScore;

	//if ( lBankerScore < 0 )
	//{
	//	if (m_wBankerUser!=INVALID_CHAIR)
	//	{
	//		lBankerScore = m_lBankerScore;
	//	}
	//	else
	//	{
	//		lBankerScore = 0x7fffffffffffffff;
	//	}
	//}

	//��������
	LONGLONG lMeMaxScore;

	if((m_lMeMaxScore - lNowJetton)>m_lAreaLimitScore)
	{
		lMeMaxScore= m_lAreaLimitScore;
	}
	else
	{
		lMeMaxScore = m_lMeMaxScore-lNowJetton;
		lMeMaxScore = lMeMaxScore;
	}

	//ׯ������
	BYTE diMultiple[AREA_COUNT]={5,10,10,10,20,20,20,50};
	lMeMaxScore=min(lMeMaxScore,(lBankerScore)/(diMultiple[cbJettonArea-1]));

	//��������
	lMeMaxScore = max(lMeMaxScore, 0);

	return lMeMaxScore;
}

void CGameClientView::DrawMeJettonNumber(CDC *pDC)
{
	//�滭����
	for (int nAreaIndex=1; nAreaIndex<=AREA_COUNT; ++nAreaIndex)
	{
		if (m_lUserJettonScore[nAreaIndex] > 0 )
		{
			if(m_lUserJettonScore[nAreaIndex]<10)
				DrawNumberString(pDC,m_lUserJettonScore[nAreaIndex],m_PointJettonNumber[nAreaIndex-1].x+14,m_PointJettonNumber[nAreaIndex-1].y, false,true);
			else if(m_lUserJettonScore[nAreaIndex]>=10&&m_lUserJettonScore[nAreaIndex]<100)
				DrawNumberString(pDC,m_lUserJettonScore[nAreaIndex],m_PointJettonNumber[nAreaIndex-1].x+7,m_PointJettonNumber[nAreaIndex-1].y, false,true);
			else
				DrawNumberString(pDC,m_lUserJettonScore[nAreaIndex],m_PointJettonNumber[nAreaIndex-1].x,m_PointJettonNumber[nAreaIndex-1].y, false,true);
			//DrawNumber(pDC,&m_ImageMeScoreNumberPng,TEXT("0123456789"),m_lUserJettonScore[nAreaIndex],m_PointJettonNumber[nAreaIndex-1].x+35,m_PointJettonNumber[nAreaIndex-1].y-11,DT_RIGHT);
		}
	}
}

//��ʼ����
void CGameClientView::DispatchCard()
{

	//���ñ�ʶ
	m_bNeedSetGameRecord=true;
}

//��������
void CGameClientView::FinishDispatchCard( bool bRecord , BYTE cbIndex )
{
	//����ж�
	if (m_bNeedSetGameRecord==false) return;

	//���ñ�ʶ
	m_bNeedSetGameRecord=false;

	//ɾ����ʱ��
	KillTimer(IDI_DISPATCH_CARD);

	BYTE  bcResulteOut[AREA_COUNT+1];
	memset(bcResulteOut,0,AREA_COUNT+1);
	BYTE  cbTempResulteOut[AREA_COUNT+1];
	memset(cbTempResulteOut,0,AREA_COUNT+1);
	BYTE  bcTempOut[AREA_COUNT+1];
	memset(bcTempOut,0,AREA_COUNT+1);

	m_GameLogic.GetCardTypeEx(m_cbTableCardArray[0],1,bcResulteOut);
	m_GameLogic.GetCardTypeEx(m_cbTableCardArray[cbIndex],1,m_cbResulte);
	
	for(int k=1;k<=AREA_COUNT;k++)
	{
		if(m_cbResulte[k-1]>0)
		{
			m_lMeCurGameReturnScore[m_wMeChairID]+=m_cbResulte[k-1]*m_lUserJettonScore[k];
		}
	}

	//�����¼
	if (bRecord&&!m_bShowJettonAn)
	{
		SetGameHistory(bcResulteOut);		
	}

	//�ۼƻ���
	m_lMeStatisticScore+=m_lMeCurGameScore;
	//m_lBankerWinScore=m_lTmpBankerWinScore;


	bool blWin[AREA_COUNT];
	for (int i = 0;i<AREA_COUNT;i++)
	{
		if(cbTempResulteOut[i]>0)
		{
			blWin[i]=true;
		}
		else
		{
			blWin[i]=false;
		}
	}

	//����Ӯ��
	SetWinnerSide(blWin, true);

	//��������
	/*if (m_lMeCurGameScore>0) 
	{
		SendEngineMessage(IDM_SOUND,5,5);

	}
	else if (m_lMeCurGameScore<0) 
	{
		SendEngineMessage(IDM_SOUND,4,4);

	}
	else 
	{
		SendEngineMessage(IDM_SOUND,4,4);

	}*/
}

//���ſ��ж���
void CGameClientView::PlayEffect(CDC *pDC ,int nWidth, int nHeight)
{
	tagClientGameRecord &GameRecord = m_GameRecordArrary[m_nRecordLast];

	//ʤ����ʶ
	int static nFlagsIndex=0;
	for (int i = 0;i<=AREA_COUNT;i++)
	{
		if(m_cbResulte[i]==99||m_cbResulte[i]==100)
		{
			CSize SizeTimes(m_ImageEffectTimes.GetWidth()/3,m_ImageEffectTimes.GetHeight());
			m_ImageEffectTimes.DrawImage(pDC,nWidth/2-175,nHeight/2-140,SizeTimes.cx,SizeTimes.cy,SizeTimes.cx,0);
		}
		if(m_cbResulte[i]>0&&m_cbResulte[i]<=50)
		{
			CSize SizeFrame(m_PngFrameTianMen[i].GetWidth()/6,m_PngFrameTianMen[i].GetHeight());			
			if(m_cbResulte[i]==2 || m_cbResulte[i]==25)
			{
				m_PngFrameTianMen[i].DrawImage(pDC,nWidth/2-176,nHeight/2-170,SizeFrame.cx,SizeFrame.cy,(m_nEffectIndex+3)*SizeFrame.cx,0);	
			}
			else
			{
				m_PngFrameTianMen[i].DrawImage(pDC,nWidth/2-176,nHeight/2-170,SizeFrame.cx,SizeFrame.cy,m_nEffectIndex*SizeFrame.cx,0);
			}
		}
	}	
}

//ʤ���߿�
void CGameClientView::FlashJettonAreaFrame(int nWidth, int nHeight, CDC *pDC)
{
	//�Ϸ��ж�
	if(m_bFlashResult&&!m_blRungingCar)
	{
		int nDestWidth = m_idb_selPng.GetWidth()/4;
		int nDestHeight = m_idb_selPng.GetHeight();
		if(m_nProbability==2||m_nProbability==3)
		{
			m_idb_selPng.DrawImage(pDC ,m_CarRect[m_cbTableCardArray[1]-1].left,m_CarRect[m_cbTableCardArray[1]-1].top,nDestWidth,nDestHeight,0,0);
			m_idb_selPng.DrawImage(pDC ,m_CarRect[m_cbTableCardArray[2]-1].left,m_CarRect[m_cbTableCardArray[2]-1].top,nDestWidth,nDestHeight,0,0);
			m_idb_selPng.DrawImage(pDC ,m_CarRect[m_cbTableCardArray[3]-1].left,m_CarRect[m_cbTableCardArray[3]-1].top,nDestWidth,nDestHeight,0,0);
		}
		else if(m_nProbability==4)
		{
			m_idb_selPng.DrawImage(pDC ,m_CarRect[m_cbTableCardArray[1]-1].left,m_CarRect[m_cbTableCardArray[1]-1].top,nDestWidth,nDestHeight,0,0);
			m_idb_selPng.DrawImage(pDC ,m_CarRect[m_cbTableCardArray[2]-1].left,m_CarRect[m_cbTableCardArray[2]-1].top,nDestWidth,nDestHeight,0,0);
			m_idb_selPng.DrawImage(pDC ,m_CarRect[m_cbTableCardArray[3]-1].left,m_CarRect[m_cbTableCardArray[3]-1].top,nDestWidth,nDestHeight,0,0);
			m_idb_selPng.DrawImage(pDC ,m_CarRect[m_cbTableCardArray[4]-1].left,m_CarRect[m_cbTableCardArray[4]-1].top,nDestWidth,nDestHeight,0,0);
		}
		else
			m_idb_selPng.DrawImage(pDC ,m_CarRect[m_CarIndex].left,m_CarRect[m_CarIndex].top,nDestWidth,nDestHeight,0,0);
		m_EffectBackLeft.DrawImage(pDC,nWidth/2-557,nHeight/2-316);
		m_EffectBackRight.DrawImage(pDC,nWidth/2+375,nHeight/2-316);
	}

	if (m_cbAreaFlash==0xFF && false==m_bFlashResult) return;

	//��ע�ж�
	if (false==m_bFlashResult && m_pGameClientDlg != NULL)
	{
		/*if (m_pGameClientDlg->GetGameStatus()==GS_PLACE_JETTON)
		{			
			m_PngFrameTianMen.DrawImage(pDC,m_RectArea[m_cbAreaFlash-1].left,m_RectArea[m_cbAreaFlash-1].top);
		}*/
	}
	else
	{		
	}
}

//�ƶ�Ӯ��
void CGameClientView::DeduceWinner(bool bWinMen[])
{
	BYTE bcData = m_cbTableCardArray[0];
	if(1==bcData||bcData==2||bcData==1+8||bcData==2+8||bcData==1+2*8||bcData==2+2*8||bcData==1+3*8||bcData==2+3*8)
	{
		if(bcData%2==1)
			bWinMen[0]= true;
		else
			bWinMen[1]= true;

	}
	else if(3==bcData||bcData==4||bcData==3+8||bcData==4+8||bcData==3+2*8||bcData==4+2*8||bcData==3+3*8||bcData==4+3*8)
	{
		if(bcData%2==1)
			bWinMen[2]= true;
		else
			bWinMen[3]= true;



	}else if(5==bcData||bcData==6||bcData==5+8||bcData==6+8||bcData==5+2*8||bcData==6+2*8||bcData==5+3*8||bcData==6+3*8)
	{
		if(bcData%2==1)
			bWinMen[4]= true;
		else
			bWinMen[5]= true;

	}else if(7==bcData||bcData==8||bcData==7+8||bcData==8+8||bcData==7+2*8||bcData==8+2*8||bcData==7+3*8||bcData==8+3*8)
	{
		if(bcData%2==1)
			bWinMen[6]= true;
		else
			bWinMen[7]= true;

	}
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
			SendEngineMessage(IDM_PLACE_JETTON,1,m_nTime);
			break;
		}
	case IDC_JETTON_BUTTON_1000:
		{
			//���ñ���
			SendEngineMessage(IDM_PLACE_JETTON,2,m_nTime);
			break;
		}
	case IDC_JETTON_BUTTON_10000:
		{
			//���ñ���
			SendEngineMessage(IDM_PLACE_JETTON,3,m_nTime);
			break;
		}
	case IDC_JETTON_BUTTON_50000:
		{
			//���ñ���
			SendEngineMessage(IDM_PLACE_JETTON,4,m_nTime);
			break;
		}
	case IDC_JETTON_BUTTON_100000:
		{
			//���ñ���
			SendEngineMessage(IDM_PLACE_JETTON,5,m_nTime);
			break;
		}
	case IDC_JETTON_BUTTON_500000:
		{
			//���ñ���
			SendEngineMessage(IDM_PLACE_JETTON,6,m_nTime);
			break;
		}
	case IDC_JETTON_BUTTON_1000000:
		{
			//���ñ���
			SendEngineMessage(IDM_PLACE_JETTON,7,m_nTime);
			break;
		}
	case IDC_JETTON_BUTTON_5000000:
		{
			//���ñ���
			SendEngineMessage(IDM_PLACE_JETTON,8,m_nTime);
			break;
		}
	case IDC_CHAGE_MONEY:
		{
			SendEngineMessage(IDM_CHAGE_MONEY,0,0);
			break;
		}
	case IDC_CONTINUE_JETTON:
		{
			SendEngineMessage(IDM_CONTINUE_JETTON,0,0);
			break;
		}
	case IDC_SMALL:
		{
			SendEngineMessage(IDM_BIG_SMALL,0,0);
			break;
		}
	case IDC_BIG:
		{
			SendEngineMessage(IDM_BIG_SMALL,1,0);
			break;
		}
	case IDC_1_TIME:
		{			
			m_nTime=1;
			RefreshGameView();
			break;
		}
	case IDC_10_TIME:
		{			
			m_nTime=10;
			RefreshGameView();
			break;
		}
	case IDC_100_TIME:
		{			
			m_nTime=100;
			RefreshGameView();
			break;
		}
	case IDC_CHIP_CLOSE:
	case IDC_CHIP_X:
		{
			SendEngineMessage(IDM_CLOSE,0,0);
			break;
		}
	case IDC_CHIP_100:
		{
			SendEngineMessage(IDM_CHANGE_GOLD,100,0);
			break;
		}
	case IDC_CHIP_1000:
		{
			SendEngineMessage(IDM_CHANGE_GOLD,1000,0);
			break;
		}
	case IDC_CHIP_10000:
		{
			SendEngineMessage(IDM_CHANGE_GOLD,10000,0);
			break;
		}
	case IDC_CHIP_ALL:
		{
			SendEngineMessage(IDM_CHANGE_GOLD,0,1);
			break;
		}
	case IDC_CHIP_OK:
		{
			SendEngineMessage(IDM_OK,0,0);
			break;
		}
	case IDC_RADIO:
		{
			m_CheckImagIndex = 0;
		}
		break;
	case IDC_RADIO+1:
		{
			m_CheckImagIndex = 1;
		}
		break;
	case IDC_RADIO+2:
		{
			m_CheckImagIndex = 2;

		}
		break;
	case IDC_RADIO+3:
		{
			m_CheckImagIndex = 3;
		}
		break;

	}

	m_lLastJetton = m_lCurrentJetton;

	return CGameFrameViewGDI::OnCommand(wParam, lParam);
}

//�ҵ�λ��
void CGameClientView::SetMeChairID(DWORD dwMeUserID)
{
	//�������Ӻ�
	for (WORD wChairID=0; wChairID<MAX_CHAIR; ++wChairID)
	{
		IClientUserItem *pClientUserItem=GetClientUserItem(wChairID);
		if (NULL!=pClientUserItem && dwMeUserID==pClientUserItem->GetChairID())
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
	if (enDispatchCardTip_NULL!=DispatchCardTip) SetTimer(IDI_SHOWDISPATCH_CARD_TIP,2*1000,NULL);		
	else KillTimer(IDI_SHOWDISPATCH_CARD_TIP);

	//���½���
	RefreshGameView();
}

//�滭ׯ��
void CGameClientView::DrawBankerInfo(CDC *pDC,int nWidth,int nHeight)
{
	//ׯ����Ϣ																											
	pDC->SetTextColor(RGB(255,255,0));

	//��ȡ���
	IClientUserItem *pClientUserItem = m_wBankerUser==INVALID_CHAIR ? NULL : GetClientUserItem(m_wBankerUser);	

	CRect static StrRect;
	StrRect.left = nWidth/2+m_ViewBackPng.GetWidth()/2-300;
	StrRect.top = nHeight/2-m_ViewBackPng.GetHeight()/2+96;
	StrRect.right = StrRect.left + 115;
	StrRect.bottom = StrRect.top + 15;

	//ׯ������
	TCHAR Text[256];
	if(m_bEnableSysBanker)
	{
		_sntprintf(Text,256,_TEXT("%s"),pClientUserItem==NULL?(m_bEnableSysBanker?TEXT("ϵͳ��ׯ"):TEXT("ϵͳ��ׯ")):pClientUserItem->GetNickName());
		//pDC->DrawText(Text, StrRect, DT_END_ELLIPSIS | DT_LEFT | DT_TOP|DT_SINGLELINE );

		CDFontEx::DrawText(this,pDC,  12, 400, Text, StrRect, RGB(255,255,0), DT_END_ELLIPSIS | DT_LEFT | DT_TOP|DT_SINGLELINE);
	}
	else
	{
		_sntprintf(Text,256,_TEXT("%s"),pClientUserItem==NULL?(m_bEnableSysBanker==false?TEXT("������ׯ"):TEXT("������ׯ")):pClientUserItem->GetNickName());
		//pDC->DrawText(Text, StrRect, DT_END_ELLIPSIS | DT_LEFT | DT_TOP|DT_SINGLELINE );

		CDFontEx::DrawText(this,pDC,  12, 400, Text, StrRect, RGB(255,255,0), DT_END_ELLIPSIS | DT_LEFT | DT_TOP|DT_SINGLELINE);
	}

	StrRect.top = StrRect.bottom+6;
	StrRect.bottom = StrRect.top + 15;
	_sntprintf(Text,256,_TEXT("%s"),NumberStringWithSpace(pClientUserItem==NULL?0:m_lBankerScore));	
	CDFontEx::DrawText(this,pDC,  12, 400, Text, StrRect, RGB(255,255,0), DT_END_ELLIPSIS | DT_LEFT | DT_TOP|DT_SINGLELINE);

	StrRect.top = StrRect.bottom+7;
	StrRect.bottom = StrRect.top + 15;
	_sntprintf(Text,256,_TEXT("%d"),m_wBankerTime);
	CDFontEx::DrawText(this,pDC,  12, 400, Text, StrRect, RGB(255,255,0), DT_END_ELLIPSIS | DT_LEFT | DT_TOP|DT_SINGLELINE);	

	StrRect.top = StrRect.bottom+8;
	StrRect.bottom = StrRect.top + 15;
	_sntprintf(Text,256,_TEXT("%s"),NumberStringWithSpace(m_lBankerWinScore));	
	CDFontEx::DrawText(this,pDC,  12, 400, Text, StrRect, RGB(255,255,0), DT_END_ELLIPSIS | DT_LEFT | DT_TOP|DT_SINGLELINE);
}

// �滭��ׯ�б�
void CGameClientView::DrawBankerList(CDC *pDC, int nWidth, int nHeight)
{
	// �������
	CPoint ptBegin(0,0);
	// ������ׯ�б�
	ptBegin.SetPoint( nWidth/2 + 203, nHeight/2 - m_ViewBackPng.GetHeight()/2+224);

	int   nCount = 0;
	CRect rect(0, 0, 0, 0);	
	rect.SetRect(ptBegin.x + 18, ptBegin.y + 15, ptBegin.x + 175, ptBegin.y + 52);
	
	for( int i = m_nShowValleyIndex; i < m_ValleysList.GetCount(); ++i )
	{
		IClientUserItem* pIClientUserItem = GetClientUserItem( m_ValleysList[i] );
		if ( pIClientUserItem == NULL  )
			continue;
		// ����
		CRect rectName(rect);
		rectName.left = ptBegin.x;
		rectName.right = ptBegin.x + 75;
		DrawTextString( pDC, pIClientUserItem->GetNickName(), RGB(255,234,0) , RGB(38,26,2), rectName, DT_END_ELLIPSIS|DT_LEFT|DT_VCENTER|DT_SINGLELINE);

		// ����
		CRect rectScore(rect);
		rectScore.left = ptBegin.x + 80;
		rectScore.right = rectScore.left + 73;
		DrawNumberStringWithSpace( pDC,m_lExchangeScore[m_ValleysList[i]], rectScore, DT_END_ELLIPSIS|DT_LEFT|DT_VCENTER|DT_SINGLELINE);

		// λ�õ���
		rect.OffsetRect(0, 18);

		// λ������
		nCount++;

		if ( nCount == 4 )
		{
			break;
		}
	}

}

void CGameClientView::SetFirstShowCard(BYTE bcCard)
{
}
//�滭���
void CGameClientView::DrawMeInfo(CDC *pDC,int nWidth,int nHeight)
{
	//�Ϸ��ж�
	if (INVALID_CHAIR==m_wMeChairID) return;	

	//��ȡ���
	IClientUserItem *pClientUserItem =  GetClientUserItem(m_wMeChairID);

	//λ����Ϣ

	CRect static StrRect;
	StrRect.left = nWidth/2+228;
	StrRect.top = nHeight/2-m_ViewBackPng.GetHeight()/2+170;
	StrRect.right = StrRect.left + 100;
	StrRect.bottom = StrRect.top + 15;

	//ׯ������
	TCHAR Text[256];
	_sntprintf(Text,256,_TEXT("%s"),pClientUserItem->GetNickName());
	//pDC->DrawText(Text, StrRect, DT_END_ELLIPSIS | DT_LEFT | DT_TOP|DT_SINGLELINE );

	CDFontEx::DrawText(this,pDC,  12, 400, Text, StrRect, RGB(255,255,0), DT_END_ELLIPSIS | DT_LEFT | DT_TOP|DT_SINGLELINE);

	StrRect.top = StrRect.bottom+6;
	StrRect.bottom = StrRect.top + 15;
	_sntprintf(Text,256,_TEXT("%s"),NumberStringWithSpace(m_lExchangeScore[m_wMeChairID]));
	//pDC->DrawText(Text, StrRect, DT_END_ELLIPSIS | DT_LEFT | DT_TOP|DT_SINGLELINE );
	CDFontEx::DrawText(this,pDC,  12, 400, Text, StrRect, RGB(255,255,0), DT_END_ELLIPSIS | DT_LEFT | DT_TOP|DT_SINGLELINE);

	//StrRect.top = StrRect.bottom;
	//StrRect.bottom = StrRect.top + 15;
	//SCORE lMeJetton=0L;
	//for (int nAreaIndex=1; nAreaIndex<=AREA_COUNT; ++nAreaIndex) lMeJetton += m_lUserJettonScore[nAreaIndex];
	//_sntprintf(Text,256,_TEXT("���ã�%s"),NumberStringWithSpace(pClientUserItem==NULL?0:pClientUserItem->GetUserScore()-lMeJetton));
	////pDC->DrawText(Text, StrRect, DT_END_ELLIPSIS | DT_LEFT | DT_TOP|DT_SINGLELINE );
	//CDFontEx::DrawText(this,pDC,  12, 400, Text, StrRect, RGB(255,255,0), DT_END_ELLIPSIS | DT_LEFT | DT_TOP|DT_SINGLELINE);

	//StrRect.top = StrRect.bottom;
	//StrRect.bottom = StrRect.top + 15;
	//_sntprintf(Text,256,_TEXT("�ɼ���%s"),NumberStringWithSpace(m_lMeStatisticScore) );
	////pDC->DrawText(Text, StrRect, DT_END_ELLIPSIS | DT_LEFT | DT_TOP|DT_SINGLELINE );
	//CDFontEx::DrawText(this,pDC,  12, 400, Text, StrRect, RGB(255,255,0), DT_END_ELLIPSIS | DT_LEFT | DT_TOP|DT_SINGLELINE);
}
//////////////////////////////////////////////////////////////////////////
//���д��
void CGameClientView::OnBankStorage()
{
//#ifdef __SPECIAL___
//	//��ȡ�ӿ�
//	CGameClientEngine *pGameClientEngine=CONTAINING_RECORD(this,CGameClientEngine,m_GameClientView);
//	IClientKernel *pIClientKernel=(IClientKernel *)pGameClientEngine->m_pIClientKernel;
//
//	if(NULL!=pIClientKernel)
//	{
//		CRect btRect;
//		m_btBankerStorage.GetWindowRect(&btRect);
//		ShowInsureSave(pIClientKernel,CPoint(btRect.left-100,btRect.top));
//	}
//#endif
}

//����ȡ��
void CGameClientView::OnBankDraw()
{
//#ifdef __SPECIAL___
//	//��ȡ�ӿ�
//	CGameClientEngine *pGameClientEngine=CONTAINING_RECORD(this,CGameClientEngine,m_GameClientView);
//	IClientKernel *pIClientKernel=(IClientKernel *)pGameClientEngine->m_pIClientKernel;
//
//	if(NULL!=pIClientKernel)
//	{
//		CRect btRect;
//		m_btBankerDraw.GetWindowRect(&btRect);
//		ShowInsureGet(pIClientKernel,CPoint(btRect.left-100,btRect.top));
//	}
//#endif
}

void CGameClientView::OnUp()
{
	if ( m_nShowValleyIndex > 0 )
		m_nShowValleyIndex--;

	//���½���
	InvalidGameView(0,0,0,0);
}
void CGameClientView::OnDown()
{
	if( m_nShowValleyIndex < m_ValleysList.GetCount() - 1 )
		m_nShowValleyIndex++;

	//���½���
	InvalidGameView(0,0,0,0);
};
void CGameClientView::StartMove()
{
	m_Out_Bao_y = 0;	
	m_bShowBao = TRUE;	
	m_bShowLeaveHandleAn = false;
	m_bOPenBoxAn = false;	
	m_blShowLastResult = false;
	m_bShowResult = false;

}
void CGameClientView::StartJetton_AniMationN()
{

}

void CGameClientView::StartOPenBox()
{
	m_bOPenBoxAn = true;
	//m_bOPenBoxIndex = 0;
	m_bShowBao = false;
	m_Out_Bao_y = 6;
	SetJettonHide(0);

}

void CGameClientView::StartHandle_Leave()
{
	m_bShowLeaveHandleAn = true;	

	//SendEngineMessage(IDM_SOUND,0,1);


}
void CGameClientView::StarShowResult()
{
	m_bShowResult = true;

}
//�滭ʱ��
void CGameClientView::MyDrawUserTimer(CDC * pDC, int nXPos, int nYPos, WORD wTime, WORD wTimerArea)
{
	//������Դ
	CPngImageEx ImageTimeBack;
	CPngImageEx ImageTimeNumber;
	ImageTimeBack.LoadImage(this,GetModuleHandle(GAME_FRAME_DLL_NAME),TEXT("TIME_BACK"));
	ImageTimeNumber.LoadImage(this,GetModuleHandle(GAME_FRAME_DLL_NAME),TEXT("TIME_NUMBER"));

	//��ȡ����
	const INT nNumberHeight=ImageTimeNumber.GetHeight();
	const INT nNumberWidth=ImageTimeNumber.GetWidth()/10;

	//������Ŀ
	LONGLONG lNumberCount=0;
	WORD wNumberTemp=wTime;
	do
	{
		lNumberCount++;
		wNumberTemp/=10;
	} while (wNumberTemp>0L);

	//λ�ö���
	INT nYDrawPos=nYPos-nNumberHeight/2+1;
	INT nXDrawPos=static_cast<INT>(nXPos+(lNumberCount*nNumberWidth)/2-nNumberWidth);

	//�滭����
	CSize SizeTimeBack(ImageTimeBack.GetWidth(),ImageTimeBack.GetHeight());
	ImageTimeBack.DrawImage(pDC,nXPos-SizeTimeBack.cx/2,nYPos-SizeTimeBack.cy/2);

	//�滭����
	for (LONGLONG i=0;i<lNumberCount;i++)
	{
		//�滭����
		WORD wCellNumber=wTime%10;
		ImageTimeNumber.DrawImage(pDC,nXDrawPos,nYDrawPos,nNumberWidth,nNumberHeight,wCellNumber*nNumberWidth,0);

		//���ñ���
		wTime/=10;
		nXDrawPos-=nNumberWidth;
	};

	return;
}

//����Ա����
void CGameClientView::OpenAdminWnd()
{
	//��Ȩ��
	if(m_pClientControlDlg != NULL)
	{
		if(!m_pClientControlDlg->IsWindowVisible()) 
		{
			m_pClientControlDlg->ShowWindow(SW_SHOW);
			CRect rt;
			GetClientRect(&rt);
			ClientToScreen(&rt);
			CRect rect;
			m_pClientControlDlg->GetWindowRect(&rect);
			m_pClientControlDlg->MoveWindow((rt.Width()-rect.Width())/2+rt.left,(rt.Height()-rect.Height())/2+rt.top,rect.Width(),rect.Height());
		}
			
		else 
			m_pClientControlDlg->ShowWindow(SW_HIDE);
	}
}

//ִ��ʣ�����еĻ��嶯��
void CGameClientView::PerformAllBetAnimation()
{
	KillTimer(IDI_ANDROID_BET);
	for ( int i = 0 ; i < m_ArrayAndroid.GetCount(); ++i)
	{
		KillTimer(IDI_ANDROID_BET + i + 1);
		PlaceUserJetton(m_ArrayAndroid[i].cbJettonArea, m_ArrayAndroid[i].lJettonScore);
	}
	m_ArrayAndroid.RemoveAll();
	m_ImageTip.DestroyImage();
}

//������ͼ
void CGameClientView::RefreshGameView()
{
	CRect rect;
	GetClientRect(&rect);
	InvalidGameView(rect.left,rect.top,rect.Width(),rect.Height());

	return;
}

//������ע���
void CGameClientView::ControlBetDetection( BYTE cbViewIndex, LONGLONG lScoreCount, WORD wViewChair, WORD wChair)
{
	if (m_pClientControlDlg == NULL || wViewChair >= GAME_PLAYER )
		return;

	//��ȡ���
	m_lAllPlayBet[wViewChair][cbViewIndex-1] += lScoreCount;
	IClientUserItem* pUserData = GetClientUserItem(wViewChair);
	m_pClientControlDlg->UpdateClistCtrl(m_lAllPlayBet,cbViewIndex,lScoreCount,wChair,wViewChair,pUserData);

// 	//�����Ϣ
// 	IClientUserItem* pUserData = GetClientUserItem(wChair);
// 	int nListIndex = cbViewIndex-1;
// 	
// 	//�������
// 	LVFINDINFO lvFindInfo;
// 	ZeroMemory( &lvFindInfo, sizeof( lvFindInfo ) );
// 	lvFindInfo.flags = LVFI_STRING;
// 	lvFindInfo.psz = (LPCTSTR)pUserData->GetNickName();
// 
// 	//��������
// 	int nItem = m_SkinListCtrl[nListIndex].FindItem( &lvFindInfo );
// 	if ( nItem == -1 )
// 	{
// 		nItem = m_SkinListCtrl[nListIndex].InsertItem(m_SkinListCtrl[nListIndex].GetItemCount(), pUserData->GetNickName());//������
// 	}
// 
// 
// 	CString strScore;
// 	strScore.Format(TEXT("%I64d"), m_lAllPlayBet[wChair][cbViewIndex]);
// 	m_SkinListCtrl[nListIndex].SetItemText(nItem, 1, strScore);		//��������
}

BOOL CGameClientView::PreTranslateMessage(MSG* pMsg)
{
	BYTE cbGameStatus=0;
	if(m_pGameClientDlg != NULL)
		cbGameStatus=m_pGameClientDlg->GetGameStatus();
	// TODO: �ڴ����ר�ô����/����û���
	if(pMsg->message==WM_KEYDOWN)
	{
		if(cbGameStatus==GS_PLACE_JETTON)
		{
			if(pMsg->wParam=='1')
				SendEngineMessage(IDM_PLACE_JETTON,8,m_nTime);
			else if(pMsg->wParam=='2')
				SendEngineMessage(IDM_PLACE_JETTON,7,m_nTime);
			else if(pMsg->wParam=='3')
				SendEngineMessage(IDM_PLACE_JETTON,6,m_nTime);
			else if(pMsg->wParam=='4')
				SendEngineMessage(IDM_PLACE_JETTON,5,m_nTime);
			else if(pMsg->wParam=='5')
				SendEngineMessage(IDM_PLACE_JETTON,4,m_nTime);
			else if(pMsg->wParam=='6')
				SendEngineMessage(IDM_PLACE_JETTON,3,m_nTime);
			else if(pMsg->wParam=='7')
				SendEngineMessage(IDM_PLACE_JETTON,2,m_nTime);
			else if(pMsg->wParam=='8')
				SendEngineMessage(IDM_PLACE_JETTON,1,m_nTime);
			else if(pMsg->wParam==VK_UP)						
				SendEngineMessage(IDM_CONTINUE_JETTON,0,0);			
		}		
		if(pMsg->wParam==VK_LEFT)
		{
			if(m_lMeCurGameReturnScore[m_wMeChairID]>0)
				SendEngineMessage(IDM_BIG_SMALL,0,0);
		}
		if(pMsg->wParam==VK_RIGHT)
		{
			if(m_lMeCurGameReturnScore[m_wMeChairID]>0)
				SendEngineMessage(IDM_BIG_SMALL,1,0);
		}
	}	
	return CGameFrameViewGDI::PreTranslateMessage(pMsg);
}

//���ÿ��ƶ���ʼ���ֵ
void  CGameClientView::SetControlStartStorage(LONGLONG lStartStorage)
{
	if ( m_pClientControlDlg )
	{
		m_pClientControlDlg->SendMessage(IDM_ADMIN_STARTESTORAGE,(WPARAM)lStartStorage);
	}
}