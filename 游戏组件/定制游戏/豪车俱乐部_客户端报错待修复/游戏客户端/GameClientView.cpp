#include "StdAfx.h"
#include "Math.h"
#include "Resource.h"
#include "GameClientView.h"
#include "GameClientEngine.h"
#include ".\gameclientview.h"


//////////////////////////////////////////////////////////////////////////

//时间标识
#define IDI_FLASH_WINNER			 100									//闪动标识
#define IDI_SHOW_CHANGE_BANKER		 101									//轮换庄家

#define IDI_SHOWDISPATCH_CARD_TIP	 103									//发牌提示
#define IDI_MOVECARD_END			 104									//移動牌結束
#define IDI_HANDLELEAVE_ANIMATION	 105  
#define IDI_FLASH_CARD				 106									//闪动标识
#define IDI_END_FLASH_CARD			 108		
#define IDI_SHOW_ANOTHER_CARD_RESULT 112	
#define IDI_SHOW_ANIMAL_NORMAL       113									//显示动物
#define IDI_SHOW_ANIMAL_RESERVE      125									//显示动物
#define IDI_BG_SHOW_ANIMAL_RESERVE   126									//显示动物
#define IDI_INIT_SHOW_ANIMAL         127									//显示动物

#define IDI_DIS_CARTOON_ANIMAL		 114
#define IDI_END_FLASH_WINNER		 115

#define IDI_BEGIN_RUN_DIAL			 116
#define IDI_BEGIN_SHOW_ANIMAL		 117
#define IDI_RUN_DIAL		         118

#define IDI_END_ANIMAL		         119
#define IDI_END_SHOW_ANIMAL		     120
#define IDI_SHOW_TIPS				 121
#define IDI_BUTTON_SELECT			 122

#define IDI_FLASH_ALL_AREA			 123
#define IDI_END_FLASH_ALL_AREA		 124

//变量定义
#define	BET_AREA_WIDTH				 444									//下注区域总长
#define	BET_AREA_HEIGHT				 444									//下注区域总宽

#define	TIME_AREA_WIDTH				 124									//时钟区域总长
#define	TIME_AREA_HEIGHT			 124									//时钟区域总宽
//机器人下注,以下所有定时器索引保留(1000以后),专机器人提供
#define IDI_ANDROID_BET				1000								

//按钮标识
#define IDC_JETTON_BUTTON_10		200									//按钮标识
#define IDC_JETTON_BUTTON_100		201									//按钮标识
#define IDC_JETTON_BUTTON_1000		202									//按钮标识
#define IDC_JETTON_BUTTON_10000		203									//按钮标识
#define IDC_JETTON_BUTTON_100000	204									//按钮标识
#define IDC_APPY_BANKER				206									//按钮标识
#define IDC_CANCEL_BANKER			207									//按钮标识
#define IDC_SCORE_MOVE_L			209									//按钮标识
#define IDC_SCORE_MOVE_R			210									//按钮标识
#define IDC_VIEW_CHART				211									//按钮标识
#define IDC_AUTO_OPEN_CARD			213									//按钮标识
#define IDC_OPEN_CARD				214									//按钮标识
#define IDC_BANK					215									//按钮标识
#define IDC_BANK_STORAGE			217									//按钮标识
#define IDC_BANK_DRAW				218									//按钮标识
#define IDC_ADMIN					226									//按钮标识
#define IDC_COMMIT_PLACE_JETTON     238									//按钮标识

#define IDC_CHAT_INPUT				239									//控件标识
#define IDC_CHAT_DISPLAY			240									//控件标识
#define IDC_USER_LIST				241									//控件标识

#define IDC_BUTTON_SENDMESSAGE		242
//颜色菜单
#define MAX_CHAT_COLOR				16									//最大数目

#define IDM_SELECT_MORE_COLOR		(WM_USER+1999)						//选择颜色
#define IDM_SELECT_CHAT_COLOR		(WM_USER+200)						//选择颜色

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameClientView, CGameFrameViewGDI)
	ON_WM_TIMER()
	ON_WM_CREATE()
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_ADMIN,OpenAdminWnd)
	ON_MESSAGE(WM_VIEWLBTUP,OnViLBtUp)
	ON_MESSAGE(WM_SENDEITD_MESSAGE,OnSendMessage)
	ON_COMMAND(IDM_SELECT_MORE_COLOR, OnMoreColor)
	ON_COMMAND_RANGE(IDM_SELECT_CHAT_COLOR, (IDM_SELECT_CHAT_COLOR+MAX_CHAT_COLOR), OnSelectChatColor)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
//构造函数
CGameClientView::CGameClientView()
{
	//限制信息
	m_lMeMaxScore=0;
	m_lAreaLimitScore=0;
	m_lRobotMaxJetton=0;
	//下注信息
	ZeroMemory(m_lUserJettonScore,sizeof(m_lUserJettonScore));
	ZeroMemory(m_lAllJettonScore,sizeof(m_lAllJettonScore));
	ZeroMemory(m_lUserAddScore,sizeof(m_lUserAddScore));
	ZeroMemory(m_lAllPlayBet,sizeof(m_lAllPlayBet));

	//庄家信息
	m_wBankerUser = INVALID_CHAIR;
	m_wBankerTime = 0;
	m_lBankerScore = 0L;
	m_lBankerWinScore = 0L;
	m_lTmpBankerWinScore = 0L;
	m_bEnableSysBanker = true;
	//当局成绩
	m_wMeChairID = INVALID_CHAIR;
	m_lMeCurGameScore = 0;
	m_lMeCurGameReturnScore=0;
	m_lBankerCurGameScore=0;
	m_lGameRevenue=0;
	m_lMeStatisticScore=0;
	//状态信息
	m_lCurrentJetton=0L;
	m_cbAreaFlash = 0xFF;
	m_bFlashResult=false;
	m_bFlashAllArea=false;
	m_bShowGameResult=false;
	m_bNeedSetGameRecord = false;
	m_nMultule=1;
	//结束控制
	m_blRungingCar= false;
	m_nTimerStep=0;
	m_nCarOpenSide=0;
	m_nCurRunIndex=0;
	m_nTotoalRun=0;
	m_CarIndex = 0;
	m_nShowValleyIndex=0;
	m_cbTableCard = 0;
	m_nScoreHead = 0;
	m_nRecordLast=0;
	m_nRecordFirst=0;
	m_nSwitchIndex=0;
	//绘画按钮
	for (int i=0;i<BUTTON_COUNT;i++)
	{
		m_bBtShow[i]=true;
		m_btState[i]=enNormal;
		m_bTrackmouse[i]=false;
	}

	m_bBtShow[14] = false;			//取消申请庄家

	//m_btState[16] = enDisabled;
	m_texColor = RGB(255,255,0);
	//控件变量
	m_pGameClientDlg=CONTAINING_RECORD(this,CGameClientEngine,m_GameClientView);
	m_pTrumpetItem=NULL;
	m_pExpressionControl = NULL;
}
//析构函数
CGameClientView::~CGameClientView(void)
{
	SafeDelete(m_pTrumpetItem);
}
//建立消息
int CGameClientView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{	
	INT cbControlArea[16] = { 1, 2,  3,  4,   5,  6,  7,  8,   9, 10, 11, 12,   13, 14, 15, 16 };
	//混乱数组
	DWORD wTick = GetTickCount();
	for (int nRandIndex=0; nRandIndex<15; nRandIndex++)
	{
		int nTempIndex = rand()%(15-nRandIndex);

		INT nTemp = cbControlArea[15-nRandIndex];
		cbControlArea[15-nRandIndex]=cbControlArea[nTempIndex];
		cbControlArea[nTempIndex]=nTemp;
	}

	if (__super::OnCreate(lpCreateStruct)==-1) return -1;

	AfxSetResourceHandle(AfxGetInstanceHandle());

	//加载位图
	HINSTANCE hInstance=AfxGetInstanceHandle();
	
	//背景资源
	m_ViewBackPng.LoadImage(hInstance,TEXT("PNG_CENTER"));
	
	m_ChangeBanker.LoadImage(hInstance,TEXT("CHANGE_BANKER"));
	m_CurrentBankerTime.LoadImage(hInstance,TEXT("BANKER_TIME"));
	m_EndWinScore.LoadImage(hInstance,TEXT("END_WIN_SCORE"));
	m_EndLostScore.LoadImage(hInstance,TEXT("END_LOSE_SCORE"));
	m_EndCarInfo.LoadImage(hInstance,TEXT("IDB_END_CAR"));
	m_EndMaskBack.LoadImage(hInstance,TEXT("IDB_GAME_END_PNG"));
	m_HistroyCar.LoadImage(hInstance,TEXT("IDB_CAR_FLAG"));
	m_SelPng.LoadImage(hInstance,TEXT("IDB_SEL_PNG"));
	m_MeBetNum.LoadImage(hInstance,TEXT("IDB_MENUM_PNG"));
	m_ScoreNum.LoadImage(hInstance,TEXT("IDB_SCORE_PNG"));
	m_TimeFree.LoadImage(hInstance,TEXT("TIME_FREE"));
	m_TimeBet.LoadImage(hInstance,TEXT("TIME_BET"));
	m_TimeDrawe.LoadImage(hInstance,TEXT("TIME_DRAWE"));
	m_TimeNum.LoadImage(hInstance,TEXT("IDB_TIMERCOUNT_PNG"));
	m_EndImage.LoadImage(hInstance,TEXT("IDB_VIEW_END"));
	m_BetBack.LoadImage(hInstance,TEXT("IDB_VIEW_MULTIPLE"));
	m_AearMaskLeft.LoadImage(hInstance,TEXT("BET_MASK_LEFT"));
	m_AearMaskRight.LoadImage(hInstance,TEXT("BET_MASK_RIGHT"));
	m_UserInfo.LoadImage(hInstance,TEXT("USER_BET_INFO"));
	m_BetFlagMe.LoadImage(hInstance,TEXT("BET_FLAG_ME"));
	m_BetFlagTotal.LoadImage(hInstance,TEXT("BET_FLAG_TOTAL"));

	m_RecordLast.LoadImage(hInstance,TEXT("RECORD_LAST"));
	m_BackMutiple.LoadImage(hInstance,TEXT("BACK_MUTIPLE"));

	m_pngCarMaserati.LoadImage(hInstance,TEXT("CAR_MASERATI"));
	m_pngCarBMW.LoadImage(hInstance,TEXT("CAR_BMW"));
	m_pngCarPorsche.LoadImage(hInstance,TEXT("CAR_PORSCHE"));
	m_pngCarBenz.LoadImage(hInstance,TEXT("CAR_BENZ"));
	m_pngCarFerrari.LoadImage(hInstance,TEXT("CAR_FERRARI"));
	m_pngCarJaguar.LoadImage(hInstance,TEXT("CAR_JAGUAR"));
	m_pngCarLamborghini.LoadImage(hInstance,TEXT("CAR_LAMBORGHINI"));
	m_pngCarLandrover.LoadImage(hInstance,TEXT("CAR_LANDROVER"));

	m_pngBetMaserati.LoadImage(hInstance,TEXT("BET_MASERATI"));
	m_pngBetBMW.LoadImage(hInstance,TEXT("BET_BMW"));
	m_pngBetPorsche.LoadImage(hInstance,TEXT("BET_PORSCHE"));
	m_pngBetBenz.LoadImage(hInstance,TEXT("BET_BENZ"));
	m_pngBetFerrari.LoadImage(hInstance,TEXT("BET_FERRARI"));
	m_pngBetJaguar.LoadImage(hInstance,TEXT("BET_JAGUAR"));
	m_pngBetLamborghini.LoadImage(hInstance,TEXT("BET_LAMBORGHINI"));
	m_pngBetLandrover.LoadImage(hInstance,TEXT("BET_LANDROVER"));
	//按钮资源
	m_pngCloseButton.LoadImage(hInstance,TEXT("PNG_BT_CLOSE_BUTTON"));									
	m_pngGameOption.LoadImage(hInstance,TEXT("PNG_BT_GAME_OPTION"));									
	m_pngMinButton.LoadImage(hInstance,TEXT("PNG_BT_MINBUTTON"));									

	m_pngBtJetton_100.LoadImage(hInstance,TEXT("PNG_BT_JETTON_100"));
	m_pngBtJetton_1000.LoadImage(hInstance,TEXT("PNG_BT_JETTON_1000"));
	m_pngBtJetton_1W.LoadImage(hInstance,TEXT("PNG_BT_JETTON_1W"));
//	m_pngBtJetton_5W.LoadImage(hInstance,TEXT("PNG_BT_JETTON_5W"));
	m_pngBtJetton_10W.LoadImage(hInstance,TEXT("PNG_BT_JETTON_10W"));
	m_pngBtJetton_100W.LoadImage(hInstance,TEXT("PNG_BT_JETTON_100W"));
	m_pngBtJetton_500W.LoadImage(hInstance,TEXT("PNG_BT_JETTON_500W"));
	m_pngBtJetton_1000W.LoadImage(hInstance,TEXT("PNG_BT_JETTON_1000W"));
	//m_pngBtJetton_5000W.LoadImage(hInstance,TEXT("PNG_BT_JETTON_5000W"));
//	m_pngBtJetton_1Y.LoadImage(hInstance,TEXT("PNG_BT_JETTON_1Y"));
	m_pngBtJetton_XUYA.LoadImage(hInstance,TEXT("PNG_BT_JETTON_XUYA"));
	
	m_pngBtBank.LoadImage(hInstance,TEXT("PNG_BT_BANK"));
	
	m_pngBtUpList.LoadImage(hInstance,TEXT("PNG_BT_UP"));
	m_pngApplyBanker.LoadImage(hInstance,TEXT("PNG_BT_APPLY_BANKER"));
	m_pngCancerBanker.LoadImage(hInstance,TEXT("PNG_BT_CANCEL_BANKER"));
	m_pngBtDownList.LoadImage(hInstance,TEXT("PNG_BT_DOWN"));

	m_pngBtSwitchChat.LoadImage(hInstance,TEXT("PNG_BT_SWITCH_CHAT"));
	m_pngBtSwitchUserList.LoadImage(hInstance,TEXT("PNG_BT_SWITCH_USER_LIST"));
	
	m_pngBtSendChat.LoadImage(hInstance,TEXT("PNG_BT_SEND_CHAT"));
// 	m_pngBtExpress.LoadImage(hInstance,TEXT("PNG_BT_CHAT_EXPRESSION"));
// 	m_pngBtColor.LoadImage(hInstance,TEXT("PNG_BT_CHAT_COLOR"));
// 	m_pngBtTrumpet.LoadImage(hInstance,TEXT("PNG_BT_CHAT_TRUMPET"));

	m_ListBack.LoadImage(hInstance,TEXT("USER_BANKER_LIST"));
	//创建控件
	CRect rcCreate(0,0,0,0);

	m_ChatInput.Create(ES_AUTOHSCROLL|WS_CHILD | WS_VISIBLE | WS_TABSTOP /*| WS_BORDER*/,
		rcCreate, this, IDC_CHAT_INPUT);
	CFont font;
	font.CreatePointFont(120,_T("宋体") ); 
	m_ChatInput.SetFont(&font);
	m_ChatInput.SetLimitText(24);
	m_ChatMessage.Create(ES_AUTOVSCROLL|ES_WANTRETURN|ES_MULTILINE|WS_CHILD|WS_VISIBLE|WS_TABSTOP|WS_VSCROLL, rcCreate, this, IDC_CHAT_DISPLAY);

	m_ChatMessage.SetBackgroundColor(false,RGB(19,39,76));
	m_ChatMessage.SetExpressionManager(CExpressionManager::GetInstance(),RGB(13,37,75));
	m_ChatMessage.SetReadOnly(TRUE);

	IClientKernel * m_pIClientKernel;
	CGlobalUnits * pGlobalUnits=CGlobalUnits::GetInstance();
	//查询接口
	ASSERT(pGlobalUnits->QueryGlobalModule(MODULE_CLIENT_KERNEL,IID_IClientKernel,VER_IClientKernel)!=NULL);
	m_pIClientKernel=(IClientKernel *)pGlobalUnits->QueryGlobalModule(MODULE_CLIENT_KERNEL,IID_IClientKernel,VER_IClientKernel);
	m_pIClientKernel->SetStringMessage(QUERY_OBJECT_INTERFACE(m_ChatMessage,IStringMessage));
	//控件属性
	LOGFONT lf;
	memset(&lf, 0, sizeof(LOGFONT));      
	lf.lfHeight = 15;
	wcscpy(lf.lfFaceName, TEXT("Arial"));
	CFont m_font;
	m_font.CreateFontIndirect(&lf);

	//绑定控件
	m_ChatMessage.SetFont(&m_font);
	ChatMsg msg;
	msg.crColor = COLOR_EVENT;
	msg.pszSendUser = _T("12314");
	msg.pszString = _T("qweqwa");
	
	//列表控件
	m_UserList.Create(LVS_REPORT | LVS_OWNERDRAWFIXED | WS_CHILD | WS_VISIBLE ,rcCreate, this, IDC_USER_LIST);

	m_UserList.SetBkColor(RGB(9,42,77));
	m_UserList.SetTextColor(RGB(255,255,255));
	m_UserList.SetTextBkColor(RGB(9,42,77));
	int nColumnCount=0;

	//用户标志
	m_UserList.InsertColumn(nColumnCount++,TEXT(""),LVCFMT_CENTER,13,0);
	m_UserList.InsertColumn(nColumnCount++,TEXT("昵称"),LVCFMT_LEFT,90);
	m_UserList.InsertColumn(nColumnCount++,TEXT("财富"),LVCFMT_LEFT,90);
	
	m_UserList.ShowWindow(SW_HIDE);

	m_btOpenAdmin.Create(NULL,WS_CHILD|WS_VISIBLE,CRect(4,4,11,11),this,IDC_ADMIN);
	m_btOpenAdmin.ShowWindow(SW_HIDE);

	//控制
	m_hInst = NULL;
	m_pAdminControl = NULL;
	m_hInst = LoadLibrary(TEXT("LuxuryCarClientControl.dll"));
	if ( m_hInst )
	{
		typedef void * (*CREATE)(CWnd* pParentWnd); 
		CREATE ClientControl = (CREATE)GetProcAddress(m_hInst,"CreateClientControl"); 
		if ( ClientControl )
		{
			m_pAdminControl = static_cast<IClientControlDlg*>(ClientControl(this));

			m_pAdminControl->ShowWindow( SW_HIDE );
		}
	}
	return 0;
}
//重置界面
VOID CGameClientView::ResetGameView()
{
	//清除桌面
	CleanUserJetton();
	if(m_pAdminControl!=NULL)
		m_pAdminControl->ClearClist();
}


VOID * CGameClientView::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IGameFrameView,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IGameFrameView,Guid,dwQueryVer);
	QUERYINTERFACE(IGamePropertyUseSink,Guid,dwQueryVer);

	return NULL;
}

//使用道具
bool CGameClientView::OnEventUseProperty(WORD wPropertyIndex)
{
	//发送喇叭
	if(wPropertyIndex==PROPERTY_ID_TRUMPET || wPropertyIndex==PROPERTY_ID_TYPHON)
	{
		//获取接口
		CGameClientEngine *pGameClientEngine=CONTAINING_RECORD(this,CGameClientEngine,m_GameClientView);
		IClientKernel *pIClientKernel=(IClientKernel *)pGameClientEngine->m_pIClientKernel;
		IClientUserItem *pMeUserItem = GetClientUserItem(m_wMeChairID);
		//构造结果
		IPC_GR_PropertyTrumpet PropertyTrumpet;
		PropertyTrumpet.wPropertyIndex=wPropertyIndex;
		PropertyTrumpet.TrumpetColor=m_pTrumpetItem->GetTrumpetColor();
		m_pTrumpetItem->GetTrumpetContent(PropertyTrumpet.szTrumpetContent);
		CopyMemory(PropertyTrumpet.szSendNickName,pMeUserItem->GetNickName(),sizeof(PropertyTrumpet.szSendNickName));

		return pIClientKernel->SendProcessData(IPC_CMD_GF_PROPERTY_INFO,IPC_SUB_GR_PROPERTY_TRUMPET,&PropertyTrumpet,sizeof(PropertyTrumpet));

	}

	return true;
}

//调整控件
VOID CGameClientView::RectifyControl(INT nWidth, INT nHeight)
{
	if(nHeight<740) nHeight = 740;
	//按钮位置
	int nButtonWidth,nButtonHeight;

	//功能按钮
	nButtonWidth=m_pngCloseButton.GetWidth()/5;
	nButtonHeight=m_pngCloseButton.GetHeight();
	
	m_RectBt[0].top=5;
	m_RectBt[0].left=nWidth-nButtonWidth-5;
	m_RectBt[0].right=m_RectBt[0].left+nButtonWidth;
	m_RectBt[0].bottom=m_RectBt[0].top+nButtonHeight;

	nButtonWidth=m_pngMinButton.GetWidth()/5;
	nButtonHeight=m_pngMinButton.GetHeight();
	m_RectBt[1].top=5;
	m_RectBt[1].left=m_RectBt[0].left-nButtonWidth;
	m_RectBt[1].right=m_RectBt[1].left+nButtonWidth;
	m_RectBt[1].bottom=m_RectBt[1].top+nButtonHeight;

	nButtonWidth = m_pngGameOption.GetWidth()/5;
	nButtonHeight=m_pngGameOption.GetHeight();
	m_RectBt[2].top=5;
	m_RectBt[2].left=m_RectBt[1].left-nButtonWidth;
	m_RectBt[2].right=m_RectBt[2].left+nButtonWidth;
	m_RectBt[2].bottom=m_RectBt[2].top+nButtonHeight;

	//筹码按钮
	nButtonWidth = m_pngBtJetton_XUYA.GetWidth()/5;
	nButtonHeight = m_pngBtJetton_XUYA.GetHeight();
	for(int i=3; i<=10; i++)
	{
		m_RectBt[i].bottom = nHeight-8;
		m_RectBt[i].top = m_RectBt[i].bottom-nButtonHeight;
		m_RectBt[i].left = 263+(i-3)*nButtonWidth+(i-3)*11+((i>6)?(i-6):0);
		m_RectBt[i].right = m_RectBt[i].left+nButtonWidth;
	}
	//银行按钮
	m_RectBt[11].left = 175;
	m_RectBt[11].right = m_RectBt[11].left+m_pngBtBank.GetWidth()/5;
	m_RectBt[11].top = nHeight-100;
	m_RectBt[11].bottom = m_RectBt[11].top+m_pngBtBank.GetHeight();

	//申请庄家
	m_RectBt[12].left = nWidth-200-1;
	m_RectBt[12].top = 125;
	m_RectBt[12].right = m_RectBt[12].left+m_pngBtUpList.GetWidth()/5;
	m_RectBt[12].bottom =125+m_pngBtUpList.GetHeight();

	m_RectBt[13].left = m_RectBt[12].right+14;
	m_RectBt[13].right = m_RectBt[13].left+m_pngApplyBanker.GetWidth()/5;
	m_RectBt[13].top = m_RectBt[12].top;
	m_RectBt[13].bottom = m_RectBt[13].top+m_pngApplyBanker.GetHeight();

	m_RectBt[14].left = m_RectBt[12].right+14;
	m_RectBt[14].right = m_RectBt[13].left+m_pngCancerBanker.GetWidth()/5;
	m_RectBt[14].top = m_RectBt[12].top;
	m_RectBt[14].bottom = m_RectBt[13].top+m_pngCancerBanker.GetHeight();

	m_RectBt[15].left = m_RectBt[13].right+14;
	m_RectBt[15].right = m_RectBt[15].left+m_pngBtDownList.GetWidth()/5;
	m_RectBt[15].top = m_RectBt[12].top;
	m_RectBt[15].bottom = m_RectBt[15].top+m_pngBtDownList.GetHeight();

	//切换按钮
	m_RectBt[16].left = nWidth-233;
	m_RectBt[16].right = m_RectBt[16].left+m_pngBtSwitchChat.GetWidth()/5;
	m_RectBt[16].top = 33+10;
	m_RectBt[16].bottom = m_RectBt[16].top+m_pngBtSwitchChat.GetHeight();

	m_RectBt[17].left = m_RectBt[16].left;
	m_RectBt[17].right = m_RectBt[17].left+m_pngBtSwitchUserList.GetWidth()/5;
	m_RectBt[17].top = 33+70;
	m_RectBt[17].bottom = m_RectBt[17].top+m_pngBtSwitchUserList.GetHeight();
	
	//聊天按钮
	m_RectBt[18].left = nWidth-55;
	m_RectBt[18].right = m_RectBt[18].left+m_pngBtSendChat.GetWidth()/5;
	m_RectBt[18].top = nHeight-28;
	m_RectBt[18].bottom = m_RectBt[18].top+m_pngBtSendChat.GetHeight();

// 	m_RectBt[19].left = m_RectBt[18].right+1;
// 	m_RectBt[19].right = m_RectBt[19].left+m_pngBtSendChat.GetWidth()/5;
// 	m_RectBt[19].top = 740-45;
// 	m_RectBt[19].bottom = m_RectBt[19].top+m_pngBtSendChat.GetHeight();
// 
// 	m_RectBt[20].left =  m_RectBt[19].right+1;
// 	m_RectBt[20].right = m_RectBt[20].left+m_pngBtSendChat.GetWidth()/5;
// 	m_RectBt[20].top = 740-45;
// 	m_RectBt[20].bottom = m_RectBt[20].top+m_pngBtSendChat.GetHeight();
// 
// 	m_RectBt[21].left =  m_RectBt[20].right+1;
// 	m_RectBt[21].right = m_RectBt[21].left+m_pngBtSendChat.GetWidth()/5;
// 	m_RectBt[21].top = nHeight-45;
// 	m_RectBt[21].bottom = m_RectBt[21].top+m_pngBtSendChat.GetHeight();

	//下注区域
	m_rectBigRect.SetRect(405+2,175+3,405+BET_AREA_WIDTH,175+BET_AREA_HEIGHT);
	m_rectSmallRect.SetRect(405+160+2,175+160+3,405+160+TIME_AREA_WIDTH,175+160+TIME_AREA_HEIGHT);
	//下注位置
	INT nAreaWidth = m_pngBetMaserati.GetWidth()/5-10;
	INT nAreaHeight = m_pngBetMaserati.GetHeight()-20;

	m_RectJettonScore[0].SetRect(0,210,nAreaWidth,210+nAreaHeight);
	m_RectJettonScore[1].SetRect(0,m_RectJettonScore[0].bottom+35,nAreaWidth,m_RectJettonScore[0].bottom+35+nAreaHeight);
	m_RectJettonScore[2].SetRect(0,m_RectJettonScore[1].bottom+35,nAreaWidth,m_RectJettonScore[1].bottom+35+nAreaHeight);
	m_RectJettonScore[3].SetRect(0,m_RectJettonScore[2].bottom+35,nAreaWidth,m_RectJettonScore[2].bottom+35+nAreaHeight);

	m_RectJettonScore[4].SetRect(nWidth-nAreaWidth,210,nWidth,210+nAreaHeight);
	m_RectJettonScore[5].SetRect(nWidth-nAreaWidth,m_RectJettonScore[4].bottom+35,nWidth,m_RectJettonScore[4].bottom+35+nAreaHeight);
	m_RectJettonScore[6].SetRect(nWidth-nAreaWidth,m_RectJettonScore[5].bottom+35,nWidth,m_RectJettonScore[5].bottom+35+nAreaHeight);
	m_RectJettonScore[7].SetRect(nWidth-nAreaWidth,m_RectJettonScore[6].bottom+35,nWidth,m_RectJettonScore[6].bottom+35+nAreaHeight);

	//转盘位置

	m_BaseAnimalPt[0].x=nWidth/2+59-5;		m_BaseAnimalPt[0].y=nHeight/2-312;
	m_BaseAnimalPt[1].x=nWidth/2+178-5;		m_BaseAnimalPt[1].y=nHeight/2-286+1;
	m_BaseAnimalPt[2].x=nWidth/2+260-5;		m_BaseAnimalPt[2].y=nHeight/2-225+1;
	m_BaseAnimalPt[3].x=nWidth/2+321-5;		m_BaseAnimalPt[3].y=nHeight/2-138+1;

	m_BaseAnimalPt[4].x=nWidth/2+337-5;		m_BaseAnimalPt[4].y=nHeight/2-33+1;
	m_BaseAnimalPt[5].x=nWidth/2+321-5;		m_BaseAnimalPt[5].y=nHeight/2+90+1;
	m_BaseAnimalPt[6].x=nWidth/2+260-5;		m_BaseAnimalPt[6].y=nHeight/2+175+1;
	m_BaseAnimalPt[7].x=nWidth/2+178-5;		m_BaseAnimalPt[7].y=nHeight/2+235+1;

	m_BaseAnimalPt[8].x=nWidth/2+59-5;		m_BaseAnimalPt[8].y=nHeight/2+250;
	m_BaseAnimalPt[9].x=nWidth/2-51-5;		m_BaseAnimalPt[9].y=nHeight/2+235+1;
	m_BaseAnimalPt[10].x=nWidth/2-137-5;	m_BaseAnimalPt[10].y=nHeight/2+175+1;
	m_BaseAnimalPt[11].x=nWidth/2-193-5;	m_BaseAnimalPt[11].y=nHeight/2+90+1;

	m_BaseAnimalPt[12].x=nWidth/2-216-5;	m_BaseAnimalPt[12].y=nHeight/2-33+1;
	m_BaseAnimalPt[13].x=nWidth/2-193-5;	m_BaseAnimalPt[13].y=nHeight/2-138+1;
	m_BaseAnimalPt[14].x=nWidth/2-137-5;	m_BaseAnimalPt[14].y=nHeight/2-225+1;
	m_BaseAnimalPt[15].x=nWidth/2-51-5;		m_BaseAnimalPt[15].y=nHeight/2-286+1;
	
	for (int i=0;i<ANIMAL_COUNT;i++) //调整位置
	{
		m_BaseAnimalPt[i].x-=110;
		m_BaseAnimalPt[i].y-=50;
	}

	//路单
	m_LuZiStart.SetPoint(360,225);
	//移动控件
	HDWP hDwp=BeginDeferWindowPos(33);
	const UINT uFlags=SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOCOPYBITS;

	DeferWindowPos(hDwp,m_ChatMessage,NULL,nWidth-268,nHeight-130,260,89,uFlags);
	
	DeferWindowPos(hDwp,m_ChatInput,NULL,nWidth-255,nHeight-28,200,15,uFlags);
	//列表控件
	DeferWindowPos(hDwp,m_UserList,NULL,m_RectBt[16].left+18,m_RectBt[16].top-7,210,122,uFlags);

	//结束移动
	EndDeferWindowPos(hDwp);
	
}
//绘画界面
VOID CGameClientView::DrawGameView(CDC * pDC, INT nWidth, INT nHeight)
{
	if(nHeight<740) nHeight = 740;
	CDC* pDCBuff = pDC;
	m_ViewBackPng.DrawImage(pDC,0,0);
	//房间名字
	if(m_strTotalRoomName.IsEmpty() == false)
	{
		CRect static StrRect;
		StrRect.left = 20;
		StrRect.top  = 10;
		StrRect.right = StrRect.left + 300;
		StrRect.bottom = StrRect.top + 14;

		CDFontEx::DrawText(this,pDC,  12, 400, m_strTotalRoomName, StrRect, COLOR_TEXT, DT_END_ELLIPSIS | DT_LEFT | DT_TOP|DT_SINGLELINE);
	}

	//申请列表
	m_ListBack.DrawImage(pDCBuff,nWidth-m_ListBack.GetWidth()/2+5,33,m_ListBack.GetWidth()/2,m_ListBack.GetHeight(),m_nSwitchIndex*m_ListBack.GetWidth()/2,0,m_ListBack.GetWidth()/2,m_ListBack.GetHeight());
	CPoint ptBegin(0,0);
	ptBegin.SetPoint( m_RectBt[12].left, 65);
	if(m_nSwitchIndex==0)
	{
		for( int i =m_nShowValleyIndex; i < m_ValleysList.GetCount(); ++i )
		{
			if(i>=m_nShowValleyIndex+3) break;
			WORD wChairID = m_ValleysList.GetAt(i);
			IClientUserItem* pIClientUserItem = GetClientUserItem( wChairID );
			if ( pIClientUserItem == NULL  )
				continue;
			TCHAR Text[256];
			//名字
			myprintf(Text,256,TEXT("%s"),pIClientUserItem->GetNickName());
			CDFontEx::DrawText(this,pDC,12, 400,Text,CRect(ptBegin.x,ptBegin.y+(i-m_nShowValleyIndex)*20,ptBegin.x+95,ptBegin.y+(i-m_nShowValleyIndex)*20+20),RGB(255,255,0),DT_END_ELLIPSIS | DT_LEFT | DT_TOP|DT_SINGLELINE);
			//金币
			myprintf(Text,256,TEXT("%I64d"),pIClientUserItem->GetUserScore()*m_nMultule);
			CDFontEx::DrawText(this,pDC,12, 400,Text,CRect(ptBegin.x+112,ptBegin.y+(i-m_nShowValleyIndex)*20,ptBegin.x+207,ptBegin.y+(i-m_nShowValleyIndex)*20+20),RGB(255,255,0),DT_END_ELLIPSIS | DT_LEFT | DT_TOP|DT_SINGLELINE);
		}
	}

	//绘画庄家
	DrawBankerInfo(pDCBuff,nWidth,nHeight);
	//绘画自己
	DrawMeInfo(pDCBuff,nWidth,nHeight);
	//绘画按钮
	for (int i=0;i<BUTTON_COUNT;i++)
	{
		if (m_bBtShow[i])
		{
			DrawPngButton(pDCBuff,i);
		}
	}


	//获取状态
	BYTE cbGameStatus=m_pGameClientDlg->GetGameStatus();

	//时间提示
	if (cbGameStatus==GAME_STATUS_FREE)	m_TimeFree.DrawImage(pDC,nWidth/2-m_TimeFree.GetWidth()/2,420);
	else if (cbGameStatus==GS_PLACE_JETTON)	m_TimeBet.DrawImage(pDC,nWidth/2-m_TimeBet.GetWidth()/2,420);
	else if (cbGameStatus==GS_GAME_END)	m_TimeDrawe.DrawImage(pDC,nWidth/2-m_TimeDrawe.GetWidth()/2,420);

	//绘画下注区域
	INT nAreaWidth = m_pngBetMaserati.GetWidth()/5-10;
	INT nAreaHeight = m_pngBetMaserati.GetHeight()-20;

	m_pngBetPorsche.DrawImage(pDC,m_RectJettonScore[0].left,m_RectJettonScore[0].top,nAreaWidth,nAreaHeight,5,8);
	m_pngBetFerrari.DrawImage(pDC,m_RectJettonScore[1].left,m_RectJettonScore[1].top,nAreaWidth,nAreaHeight,5,8);
	m_pngBetLamborghini.DrawImage(pDC,m_RectJettonScore[2].left,m_RectJettonScore[2].top,nAreaWidth,nAreaHeight,5,8);
	m_pngBetMaserati.DrawImage(pDC,m_RectJettonScore[3].left,m_RectJettonScore[3].top,nAreaWidth,nAreaHeight,5,8);
	m_pngBetBMW.DrawImage(pDC,m_RectJettonScore[4].left,m_RectJettonScore[4].top,nAreaWidth,nAreaHeight,5,8);
	m_pngBetBenz.DrawImage(pDC,m_RectJettonScore[5].left,m_RectJettonScore[5].top,nAreaWidth,nAreaHeight,5,8);
	m_pngBetJaguar.DrawImage(pDC,m_RectJettonScore[6].left,m_RectJettonScore[6].top,nAreaWidth,nAreaHeight,5,8);
	m_pngBetLandrover.DrawImage(pDC,m_RectJettonScore[7].left,m_RectJettonScore[7].top,nAreaWidth,nAreaHeight,5,8);
	
	//区域选中
	JettonAreaLight(pDC);
	//绘画加注 有控制端的话 下注显示分为机器人和真人总注
	for ( int i = 0; i < AREA_COUNT; ++i)
	{

		if (m_lAllJettonScore[i+1]==0) continue;

		int j=0;
		if(i==0) j=3;
		else if(i==1) j=1;
		else if(i==2) j=2;
		else if(i==3) j=0;
		else if(i==4) j=7;
		else if(i==5) j=4;
		else if(i==6) j=6;
		else if(i==7) j=5;
		//总注
		CRect meRect;
		meRect.CopyRect(m_RectJettonScore[j]);
		if(j<4)
		{
			meRect.left-=45;
			meRect.right-=45;
		}
		meRect.top+=20;
		meRect.bottom+=20;
		
		if(m_btOpenAdmin.IsWindowVisible())
		{
			int nX = DrawNumberString(pDCBuff,m_lAllJettonScore[i+1]-m_lAllPlayBet[i+1],meRect);//机器人总注
			CDFontEx::DrawText(this,pDC,12, 400,_T("机:"),CRect(nX-10,meRect.top+2,nX+10,meRect.bottom),RGB(25,255,11),DT_END_ELLIPSIS | DT_CENTER| DT_TOP|DT_SINGLELINE);

		}
		else 
		{
			int nX = DrawNumberString(pDCBuff,m_lAllJettonScore[i+1],meRect);
			m_BetFlagTotal.DrawImage(pDC,nX-10,meRect.top+2);
		}
		
		meRect.top+=20;
		meRect.bottom+=20;
		if(m_btOpenAdmin.IsWindowVisible())
		{
			if(m_lAllPlayBet[i+1]==0) continue;
			int nX =DrawNumberString(pDCBuff,m_lAllPlayBet[i+1],meRect,true);//真人总注
			CDFontEx::DrawText(this,pDC,12, 400,_T("玩:"),CRect(nX-10,meRect.top+2,nX+10,meRect.bottom),RGB(234,155,20),DT_END_ELLIPSIS | DT_CENTER| DT_TOP|DT_SINGLELINE);
		}
		else 
		{
			//自己下注
			if (m_lUserJettonScore[i+1]==0) continue;
			int nX = DrawNumberString(pDCBuff,m_lUserJettonScore[i+1],meRect,true);
			m_BetFlagMe.DrawImage(pDC,nX-10,meRect.top+2);
		}

	}
	//路单
	DrawWinFlags(pDC);

	//绘画时间
	WORD wUserTimer =0;
	if (m_wMeChairID!=INVALID_CHAIR)
	{
		wUserTimer = GetUserClock(m_wMeChairID);
	}
	if (wUserTimer>=0)
	{
		DrawTimeString(pDCBuff,wUserTimer, nWidth/2,nHeight/2-24);
	}
	if(m_bShowGameResult)
	{
		//车型美女
		int AnimalIndex=GetAnimalIndex(m_CarIndex+1);
		m_EndImage.DrawImage(pDC,nWidth/2-m_EndImage.GetWidth()/8,100,m_EndImage.GetWidth()/4,m_EndImage.GetHeight()/2,(AnimalIndex%4)*m_EndImage.GetWidth()/4,AnimalIndex/4*m_EndImage.GetHeight()/2);
		//车型
		m_EndCarInfo.DrawImage(pDC,nWidth/2-m_EndCarInfo.GetWidth()/8,nHeight/2-130,m_EndCarInfo.GetWidth()/4,m_EndCarInfo.GetHeight()/2,(AnimalIndex%4)*m_EndCarInfo.GetWidth()/4,AnimalIndex/4*m_EndCarInfo.GetHeight()/2);
		//结算背景
		m_EndMaskBack.DrawImage(pDC,nWidth/2-m_EndMaskBack.GetWidth()/2+3,nHeight/2-m_EndMaskBack.GetHeight()/2-30);
		//结算
		DrawEndNum(pDC,m_lBankerCurGameScore*m_nMultule,nWidth/2+155-m_EndMaskBack.GetWidth()/2+3,nHeight/2-10-m_EndMaskBack.GetHeight()/2,m_lBankerCurGameScore>=0);
		DrawEndNum(pDC,m_lMeCurGameScore*m_nMultule,nWidth/2+155-m_EndMaskBack.GetWidth()/2+3,nHeight/2+20-m_EndMaskBack.GetHeight()/2,m_lMeCurGameScore>=0);
	}

	//绘画车标
	m_BetBack.DrawImage(pDC,nWidth/2-m_BetBack.GetWidth()/2,12);
	if(!m_bShowGameResult) m_BackMutiple.DrawImage(pDC,nWidth/2-m_BackMutiple.GetWidth()/2,120);
	for(int nCardIndex=0;nCardIndex<ANIMAL_COUNT;nCardIndex++)
	{
		if(nCardIndex==0) m_pngCarMaserati.DrawImage(pDC,m_BaseAnimalPt[nCardIndex].x,m_BaseAnimalPt[nCardIndex].y);
		else if(nCardIndex==4) m_pngCarFerrari.DrawImage(pDC,m_BaseAnimalPt[nCardIndex].x,m_BaseAnimalPt[nCardIndex].y);
		else if(nCardIndex==8) m_pngCarPorsche.DrawImage(pDC,m_BaseAnimalPt[nCardIndex].x,m_BaseAnimalPt[nCardIndex].y);
		else if(nCardIndex==12) m_pngCarLamborghini.DrawImage(pDC,m_BaseAnimalPt[nCardIndex].x,m_BaseAnimalPt[nCardIndex].y);
		else if(nCardIndex==1||nCardIndex==11||nCardIndex==15) m_pngCarBMW.DrawImage(pDC,m_BaseAnimalPt[nCardIndex].x,m_BaseAnimalPt[nCardIndex].y);
		else if(nCardIndex==2||nCardIndex==5||nCardIndex==14) m_pngCarBenz.DrawImage(pDC,m_BaseAnimalPt[nCardIndex].x,m_BaseAnimalPt[nCardIndex].y);
		else if(nCardIndex==3||nCardIndex==6||nCardIndex==9) m_pngCarJaguar.DrawImage(pDC,m_BaseAnimalPt[nCardIndex].x,m_BaseAnimalPt[nCardIndex].y);
		else if(nCardIndex==7||nCardIndex==10||nCardIndex==13) m_pngCarLandrover.DrawImage(pDC,m_BaseAnimalPt[nCardIndex].x,m_BaseAnimalPt[nCardIndex].y);
	}


	//绘画转动
	if(m_blRungingCar)
	{
		int AnimalIndex=GetAnimalIndex(m_CarIndex+1);
		if(AnimalIndex>=2 && AnimalIndex<=5)
			m_SelPng.DrawImage(pDC,m_BaseAnimalPt[m_CarIndex].x-5,m_BaseAnimalPt[m_CarIndex].y-5,m_SelPng.GetWidth()/8,m_SelPng.GetHeight()/4,AnimalIndex*m_SelPng.GetWidth()/8,0);
		else
			m_SelPng.DrawImage(pDC,m_BaseAnimalPt[m_CarIndex].x-11,m_BaseAnimalPt[m_CarIndex].y-15,m_SelPng.GetWidth()/8,m_SelPng.GetHeight()/4,AnimalIndex*m_SelPng.GetWidth()/8,0);
		if(m_nCurRunIndex < m_nTotoalRun-15)
		{
			int nTempCar = m_CarIndex;
			for(int i=0; i<3;i++)
			{
				if(nTempCar==0) nTempCar=15;
				else	nTempCar--;
				AnimalIndex=GetAnimalIndex(nTempCar+1);
				if(AnimalIndex>=2 && AnimalIndex<=5)
					m_SelPng.DrawImage(pDC,m_BaseAnimalPt[nTempCar].x-5,m_BaseAnimalPt[nTempCar].y-5,m_SelPng.GetWidth()/8,m_SelPng.GetHeight()/4,AnimalIndex*m_SelPng.GetWidth()/8,0);
				else
					m_SelPng.DrawImage(pDC,m_BaseAnimalPt[nTempCar].x-11,m_BaseAnimalPt[nTempCar].y-15,m_SelPng.GetWidth()/8,m_SelPng.GetHeight()/4,AnimalIndex*m_SelPng.GetWidth()/8,(1+i)*m_SelPng.GetHeight()/4);
			}
		}
	}

	//闪烁动物
	FlashLightAnimal(pDCBuff);
	//全部闪烁
	FlashAllAnimal(pDCBuff);

}
//绘画按钮
void CGameClientView::DrawPngButton(CDC *pDC,int buttonIndex)
{
	int index=GetButtonState(buttonIndex);
	int startX=m_RectBt[buttonIndex].left;
	int startY=m_RectBt[buttonIndex].top;
	int xLen=m_RectBt[buttonIndex].right-m_RectBt[buttonIndex].left;
	int yLen=m_RectBt[buttonIndex].bottom-m_RectBt[buttonIndex].top;

	CPngImage *pButtonImage=NULL;
	if (buttonIndex==0) pButtonImage=&m_pngCloseButton;
	else if (buttonIndex==1) pButtonImage=&m_pngMinButton;
	else if (buttonIndex==2) pButtonImage=&m_pngGameOption;
	else if	(buttonIndex==3) pButtonImage=&m_pngBtJetton_100;
	else if	(buttonIndex==4) pButtonImage=&m_pngBtJetton_1000;
	else if	(buttonIndex==5) pButtonImage=&m_pngBtJetton_1W;
	else if	(buttonIndex==6) pButtonImage=&m_pngBtJetton_10W;
	else if	(buttonIndex==7) pButtonImage=&m_pngBtJetton_100W;
	else if	(buttonIndex==8) pButtonImage=&m_pngBtJetton_500W;
	else if	(buttonIndex==9) pButtonImage=&m_pngBtJetton_1000W;
	else if	(buttonIndex==10) pButtonImage=&m_pngBtJetton_XUYA;
	else if (buttonIndex==11) pButtonImage=&m_pngBtBank;
	else if	(buttonIndex==12) pButtonImage=&m_pngBtUpList;
	else if	(buttonIndex==13) pButtonImage=&m_pngApplyBanker;
	else if	(buttonIndex==14) pButtonImage=&m_pngCancerBanker;
	else if (buttonIndex==15) pButtonImage=&m_pngBtDownList;
	else if (buttonIndex==16) pButtonImage=&m_pngBtSwitchChat;
	else if (buttonIndex==17) pButtonImage=&m_pngBtSwitchUserList;
	else if	(buttonIndex==18) pButtonImage=&m_pngBtSendChat;
// 	else if (buttonIndex==19) pButtonImage=&m_pngBtExpress;
// 	else if (buttonIndex==20) pButtonImage=&m_pngBtColor;
// 	else if (buttonIndex==21) pButtonImage=&m_pngBtTrumpet;

	if (pButtonImage!=NULL)
	{
		pButtonImage->DrawImage(pDC,startX,startY,xLen,yLen,index*xLen,0,xLen,yLen);
	}
	return;
}
//按钮状态
BYTE CGameClientView::GetButtonState(int buttonIndex)
{
	if (m_btState[buttonIndex]==enNormal) return 0;
	else if (m_btState[buttonIndex]==enDown) return 1;
	else if (m_btState[buttonIndex]==enMouseOver) return 3;
	else if (m_btState[buttonIndex]==enDisabled) return 4;
	return 0;
}
//获取索引
BYTE CGameClientView::GetAnimalIndex(BYTE cbAllIndex)
{
	if(1==cbAllIndex)	return 3;		//宾利
	if(5==cbAllIndex)	return 2;		//法拉利
	if(9==cbAllIndex)	return 5;		//保时捷
	if(13==cbAllIndex)	return 4;		//兰博基尼
	if(8==cbAllIndex||cbAllIndex==11||cbAllIndex==14)	return 0;	//路虎
	if(2==cbAllIndex||cbAllIndex==12||cbAllIndex==16)	return 1;	//宝马
	if(4==cbAllIndex||cbAllIndex==7||cbAllIndex==10)	return 6;	//捷豹
	if(3==cbAllIndex||cbAllIndex==6||cbAllIndex==15)	return 7;	//奔驰

	return -1;
}
//设置信息
void CGameClientView::SetMeMaxScore(LONGLONG lMeMaxScore)
{
	m_lMeMaxScore = lMeMaxScore;
}
//最大下注
void CGameClientView::SetAreaLimitScore(LONGLONG lAreaLimitScore)
{
	m_lAreaLimitScore = lAreaLimitScore;
}
//设置筹码
void CGameClientView::SetCurrentJetton(LONGLONG lCurrentJetton)
{

}
//历史记录
void CGameClientView::SetGameHistory(BYTE bcResulte)
{
	//设置数据
	BYTE bcResulteTmp=bcResulte;

	tagClientGameRecord &GameRecord = m_GameRecordArrary[m_nRecordLast];

	GameRecord.cbAnimal=bcResulteTmp;
	//移动下标
	m_nRecordLast = (m_nRecordLast+1) % MAX_SCORE_HISTORY;
	if ( m_nRecordLast == m_nRecordFirst )
	{
		m_nRecordFirst = (m_nRecordFirst+1) % MAX_SCORE_HISTORY;
		if ( m_nScoreHead < m_nRecordFirst ) m_nScoreHead = m_nRecordFirst;
	}

	int nHistoryCount = (m_nRecordLast - m_nRecordFirst + MAX_SCORE_HISTORY) % MAX_SCORE_HISTORY;
	//移到最新记录
	if ( MAX_FALG_COUNT < nHistoryCount )
	{
		m_nScoreHead = (m_nRecordLast - MAX_FALG_COUNT + MAX_SCORE_HISTORY) % MAX_SCORE_HISTORY;
	}

}
//清理筹码
void CGameClientView::CleanUserJetton()
{
	//下注信息
	ZeroMemory(m_lUserJettonScore,sizeof(m_lUserJettonScore));
	//全体下注
	ZeroMemory(m_lAllJettonScore,sizeof(m_lAllJettonScore));
	//所有玩家下注
	ZeroMemory(m_lAllPlayBet,sizeof(m_lAllPlayBet));

	ZeroMemory(m_lUserAddScore,sizeof(m_lUserAddScore));

	if(m_pAdminControl)
	{
		m_pAdminControl->UpdateAllUserBet(NULL,true);
	}
	m_cbTableCard = 0;

	m_blRungingCar = false;

	m_bShowGameResult=false;
	m_lCurrentJetton=0L;
	m_RectCurSelect.SetRect(0,0,0,0);

	//更新界面
	RefreshGameView();
}
//个人下注
void CGameClientView::SetMePlaceJetton(BYTE cbViewIndex, LONGLONG lJettonCount)
{
	//效验参数
	ASSERT(cbViewIndex<=AREA_COUNT);
	if (cbViewIndex>AREA_COUNT) return;

	m_lUserJettonScore[cbViewIndex]=lJettonCount;

	//更新界面
	RefreshGameView();
}
//设置扑克
void CGameClientView::SetCardInfo(BYTE cbTableCard)
{
	if (cbTableCard!=0)
	{
		m_cbTableCard = cbTableCard;
		//开始发牌
		DispatchCard();
	}
	else
	{
		m_cbTableCard=0;
	}
}
//设置筹码
void CGameClientView::PlaceUserJetton(BYTE cbViewIndex, LONGLONG lScoreCount)
{
	//效验参数
	ASSERT(cbViewIndex<=AREA_COUNT);
	if (cbViewIndex>AREA_COUNT) return;

	m_lAllJettonScore[cbViewIndex] += lScoreCount;
	if(m_pAdminControl)	m_pAdminControl->UpdateAllUserBet(m_lAllJettonScore);
	RefreshGameView();
}

//当局成绩
void CGameClientView::SetCurGameScore(LONGLONG lMeCurGameScore, LONGLONG lMeCurGameReturnScore, LONGLONG lBankerCurGameScore, LONGLONG lGameRevenue)
{
	m_lMeCurGameScore=lMeCurGameScore;			
	m_lMeCurGameReturnScore=lMeCurGameReturnScore;			
	m_lBankerCurGameScore=lBankerCurGameScore;			
	m_lGameRevenue=lGameRevenue;

}
//设置胜方
void CGameClientView::SetWinnerSide(bool blWin[], bool bSet)
{

	if (true==bSet)
	{
		//设置定时器
		SetTimer(IDI_FLASH_WINNER,500,NULL);
	}
	else 
	{
		//清除定时器
		KillTimer(IDI_FLASH_WINNER);
	}

	//设置变量
	m_bFlashResult=bSet;
	m_cbAreaFlash=0xFF;

	//更新界面
	RefreshGameView();
}
//获取区域
BYTE CGameClientView::GetJettonArea(CPoint MousePoint)
{
	for(int i=0; i<8;i++)
	{
		if(m_RectJettonScore[i].PtInRect(MousePoint)==TRUE)
		{
			if(i==0) return ID_PORSCHE;
			else if(i==1) return ID_FERRARI;
			else if(i==2) return ID_LAMBORGHINI;
			else if(i==3) return ID_MASERATI;
			else if(i==4) return ID_BMW;
			else if(i==5) return ID_BENZ;
			else if(i==6) return ID_JAGUAR;
			else if(i==7) return ID_LANDROVER;
		}
	}

	return 0xFF;
}
//绘画数字
int CGameClientView::DrawNumberString(CDC * pDC, LONGLONG lNumber,  CRect rcRect,bool bMeScore)
{
	INT nXPos=rcRect.left, nYPos=rcRect.top;

	//加载资源
	CSize SizeScoreNumber(m_ScoreNum.GetWidth()/10,m_ScoreNum.GetHeight());

	if ( bMeScore ) 
		SizeScoreNumber.SetSize(m_MeBetNum.GetWidth()/10,m_MeBetNum.GetHeight());

	//计算数目
	int lNumberCount=0;
	LONGLONG lNumberTemp=lNumber;
	LONGLONG lNumberSymbolTemp=lNumber;
	do
	{
		lNumberCount++;
		lNumberTemp/=10;
	} while (lNumberTemp>0);

	int lSymbolCount=0;
	do
	{
		lNumberSymbolTemp/=1000;
		if(lNumberSymbolTemp>0) lSymbolCount++;

	} while (lNumberSymbolTemp>0);

	//位置定义
	INT nOffset=0;
	INT nYDrawPos=nYPos;//-SizeScoreNumber.cy/2;
	INT nXDrawPos;
	int nDis=0;
	int allNumberLegth=lNumberCount*SizeScoreNumber.cx;
	if (allNumberLegth>rcRect.right-rcRect.left)
	{
		nXDrawPos=nXPos+(lNumberCount+lSymbolCount-1)*(SizeScoreNumber.cx+nDis);
	}
	else
	{
		nXDrawPos=(nXPos+rcRect.right-allNumberLegth)/2+(lNumberCount+lSymbolCount-1)*(SizeScoreNumber.cx+nDis);
	}
	
	for (LONGLONG i=0;i<lNumberCount;i++)
	{
		//绘画号码
		LONG lCellNumber=(LONG)(lNumber%10);
			

		if ( bMeScore ) 
		{
			m_MeBetNum.DrawImage(pDC,nXDrawPos,nYDrawPos,SizeScoreNumber.cx,SizeScoreNumber.cy,
				lCellNumber*SizeScoreNumber.cx,0);

		}
		else
		{
			m_ScoreNum.DrawImage(pDC,nXDrawPos,nYDrawPos,SizeScoreNumber.cx,SizeScoreNumber.cy,
				lCellNumber*SizeScoreNumber.cx,0);
		}

		//设置变量
		lNumber/=10;
		nXDrawPos-=(SizeScoreNumber.cx-nOffset+nDis);;
	};
	return nXDrawPos;
}
//绘画时间
void CGameClientView::DrawTimeString(CDC * pDC, LONGLONG lNumber, INT nXPos, INT nYPos)
{
	//加载资源
	CSize SizeScoreNumber(m_TimeNum.GetWidth()/10,m_TimeNum.GetHeight());
	//计算数目
	LONGLONG lNumberCount=0;
	LONGLONG lNumberTemp=lNumber;
	do
	{
		lNumberCount++;
		lNumberTemp/=10;
	} while (lNumberTemp>0);
	//位置定义
	INT nOffset=8;
	INT nYDrawPos=nYPos-m_TimeNum.GetHeight()/2;
	INT nXDrawPos=(INT)(nXPos+lNumberCount*SizeScoreNumber.cx/2+lNumberCount/4*(SizeScoreNumber.cx-nOffset)/2-SizeScoreNumber.cx);

	for (LONGLONG i=0;i<lNumberCount;i++)
	{
		//绘画号码
		LONG lCellNumber=(LONG)(lNumber%10);		
		m_TimeNum.DrawImage(pDC,nXDrawPos,nYDrawPos,SizeScoreNumber.cx,SizeScoreNumber.cy,
			lCellNumber*SizeScoreNumber.cx,0);
		//设置变量
		lNumber/=10;
		nXDrawPos-=SizeScoreNumber.cx;
	};
}
//绘画数字
void CGameClientView::DrawNumStrWithSpace(CDC * pDC, LONGLONG lNumber,CRect&rcPrint,INT nFormat)
{


}
//绘画数字
void CGameClientView::DrawEndNum(CDC * pDC, LONGLONG lNumber, INT nXPos, INT nYPos,bool bWin)
{
	//加载资源
	CSize SizeScoreNumber(m_EndWinScore.GetWidth()/11,m_EndWinScore.GetHeight());
	if(bWin==false) SizeScoreNumber.SetSize(m_EndLostScore.GetWidth()/11,m_EndLostScore.GetHeight());

	//计算数目
	lNumber=lNumber>0?lNumber:-lNumber;
	LONGLONG lNumberCount=0;
	LONGLONG lNumberTemp=lNumber;
	do
	{
		lNumberCount++;
		lNumberTemp/=10;
	} while (lNumberTemp>0);

	LONGLONG lNumberSymbolTemp=lNumber;
	int lSymbolCount=0;
	do
	{
		lNumberSymbolTemp/=1000;
		if(lNumberSymbolTemp>0) lSymbolCount++;

	} while (lNumberSymbolTemp>0);

	if(bWin) m_EndWinScore.DrawImage(pDC,nXPos,nYPos,SizeScoreNumber.cx,SizeScoreNumber.cy,0,0);
	else	m_EndLostScore.DrawImage(pDC,nXPos,nYPos,SizeScoreNumber.cx,SizeScoreNumber.cy,0,0);

	//位置定义
	INT nXDrawPos=(INT)(nXPos+SizeScoreNumber.cx+(lNumberCount-1)*SizeScoreNumber.cx);
	for (LONGLONG i=0;i<lNumberCount;i++)
	{
		//绘画号码
		LONG lCellNumber=(LONG)(lNumber%10);
		if(bWin)
		{
			m_EndWinScore.DrawImage(pDC,nXDrawPos,nYPos,SizeScoreNumber.cx,SizeScoreNumber.cy,(lCellNumber+1)*SizeScoreNumber.cx,0);
		}
		else
		{
			m_EndLostScore.DrawImage(pDC,nXDrawPos,nYPos,SizeScoreNumber.cx,SizeScoreNumber.cy,(lCellNumber+1)*SizeScoreNumber.cx,0);
		}

// 		//绘画分隔符
// 		if(i%3==2 && i/3<lSymbolCount)
// 		{
// 			nXDrawPos-=(SizeScoreNumber.cx*3/5);
// 			if(bWin)
// 				m_EndWinScore.DrawImage(pDC,nXDrawPos,nYPos,SizeScoreNumber.cx*3/5,SizeScoreNumber.cy,11*SizeScoreNumber.cx+10,0,SizeScoreNumber.cx*3/5,SizeScoreNumber.cy);
// 			else
// 				m_EndLostScore.DrawImage(pDC,nXDrawPos,nYPos,SizeScoreNumber.cx*3/5,SizeScoreNumber.cy,11*SizeScoreNumber.cx+10,0,SizeScoreNumber.cx*3/5,SizeScoreNumber.cy);
// 
// 		}
		//设置变量
		lNumber/=10;
		nXDrawPos-=SizeScoreNumber.cx;
	};
}
void CGameClientView::KillCardTime()
{
	KillTimer(IDI_FLASH_WINNER);

}
void CGameClientView::StartRunCar(int iTimer)
{
	KillTimer(IDI_FLASH_CARD);
	KillTimer(IDI_END_FLASH_CARD);
	m_nTimerStep = 200;
	SetTimer(IDI_FLASH_CARD,iTimer,NULL);
	m_nCarOpenSide = m_cbTableCard;
	m_nTotoalRun = m_nCarOpenSide+ANIMAL_COUNT*3-2;
	m_nCurRunIndex = 0;
	m_CarIndex = 1;
	m_blRungingCar = true;
	SetTimer(IDI_END_FLASH_CARD,18*1000,NULL);
	//SetTimer(IDI_DIS_CARTOON_ANIMAL,60,NULL);
}
void CGameClientView::RuningCar(int iTimer)
{
	if(m_nCurRunIndex<8)
	{
		m_nTimerStep-=20;

	}
	if(m_nCurRunIndex >= m_nTotoalRun-15)
	{
		m_nTimerStep+=37 /*8*(m_nCurRunIndex-m_nTotoalRun+15)*/;
	}
	if(m_nCurRunIndex==m_nTotoalRun)
	{
		KillTimer(IDI_FLASH_CARD);
		KillTimer(IDI_END_FLASH_CARD);
		m_blRungingCar = false;
		SetTimer(IDI_FLASH_WINNER,500,NULL);
		m_bShowGameResult=true;
		SetTimer(IDI_END_FLASH_WINNER,1500,NULL);

		return;
	}
	if(m_nTimerStep<0)
	{
		return ;
	}
	KillTimer(IDI_FLASH_CARD);
	SetTimer(IDI_FLASH_CARD,iTimer,NULL);
}
//全部闪烁
void CGameClientView::FlashAllArea()
{
	m_bFlashAllArea=true;
	SetTimer(IDI_FLASH_ALL_AREA,300,NULL);
	SetTimer(IDI_END_FLASH_ALL_AREA,1500,NULL);
}
//定时器消息
void CGameClientView::OnTimer(UINT nIDEvent)
{
	//全部闪烁
	if (nIDEvent==IDI_FLASH_ALL_AREA)
	{
		//设置变量
		m_bFlashAllArea=!m_bFlashAllArea;

		//if(m_bFlashAllArea)SendEngineMessage(IDM_SOUND,9,9);
		//更新界面
		RefreshGameView();
		return;
	}
	//结束全部闪烁
	if (nIDEvent==IDI_END_FLASH_ALL_AREA)
	{
		m_bFlashAllArea=false;
		KillTimer(IDI_FLASH_ALL_AREA);
		KillTimer(IDI_END_FLASH_ALL_AREA);

		StartRunCar(20);
		return;

	}
	//移动
	if(IDI_FLASH_CARD == nIDEvent)
	{
		m_CarIndex = (m_CarIndex+1)%ANIMAL_COUNT;
		m_nCurRunIndex++;
		RuningCar(m_nTimerStep);
	//	SendEngineMessage(IDM_SOUND,3,3);

		//更新界面
		RefreshGameView();
	}
	//结束移动
	if(IDI_END_FLASH_CARD==nIDEvent)
	{
		for (int i = m_nCurRunIndex;i<m_nTotoalRun;i++)
		{
			m_CarIndex = (m_CarIndex+1)%ANIMAL_COUNT;
			m_nCurRunIndex++;
			m_bShowGameResult = true;
			if(m_nCurRunIndex==m_nTotoalRun)
			{
				KillTimer(IDI_FLASH_CARD);
				KillTimer(IDI_END_FLASH_CARD);				
				return ;
			}
		}
	}
	//闪动胜方
	if (nIDEvent==IDI_FLASH_WINNER)
	{
		//设置变量
		m_bFlashResult=!m_bFlashResult;

		//更新界面
		RefreshGameView();
		return;
	}
	//结束闪烁
	if (nIDEvent==IDI_END_FLASH_WINNER)
	{
		m_bFlashResult=true;
		//KillTimer(IDI_FLASH_WINNER);
		KillTimer(IDI_END_FLASH_WINNER);
	}
}
//鼠标消息
void CGameClientView::OnLButtonDown(UINT nFlags, CPoint Point)
{
	CRect rcClient;
	GetClientRect(&rcClient);
	//非客户区域
	CRect rcNoClientArea;
	rcNoClientArea.left = rcClient.left;
	rcNoClientArea.right = rcClient.right;
	rcNoClientArea.top = rcClient.top;
	rcNoClientArea.bottom = 30;
	bool bInButton= PngButtonOnLButtonDown(nFlags,Point);

	if (m_lCurrentJetton!=0L&&m_pGameClientDlg->GetGameStatus()==GS_PLACE_JETTON)
	{
		//下注区域
		BYTE cbJettonArea=GetJettonArea(Point);
		if(cbJettonArea>0&&cbJettonArea<=AREA_COUNT)
		{
			//最大下注
			if(m_lCurrentJetton>GetUserMaxJetton(cbJettonArea))
				return;
			//发送消息
			if (cbJettonArea!=0xFF) 
				SendEngineMessage(IDM_PLACE_JETTON,cbJettonArea,m_lCurrentJetton);
		}
	}

	__super::OnLButtonDown(nFlags,Point);

	if(!bInButton && rcNoClientArea.PtInRect(Point))
	{
		LPARAM lParam = MAKEWPARAM(Point.x,Point.y);
		AfxGetMainWnd()->PostMessage(WM_LBUTTONDOWN,0,lParam);
	}
}
void CGameClientView::OnLButtonUp(UINT nFlags, CPoint Point)
{
	PngButtonOnLButtonUp(nFlags,Point);

	__super::OnLButtonUp(nFlags,Point);
}
//接受其他控件传来的消息
LRESULT CGameClientView::OnViLBtUp(WPARAM wParam, LPARAM lParam)
{
	return 0;
}
//鼠标移动消息
void CGameClientView::OnMouseMove(UINT nFlags, CPoint point)
{
	PngButtonOnMouseMove(nFlags,point);
	__super::OnMouseMove(nFlags,point);
}
//鼠标消息
void CGameClientView::OnRButtonDown(UINT nFlags, CPoint Point)
{
	if (m_pGameClientDlg->GetGameStatus()!=GS_GAME_END && m_cbAreaFlash!=0xFF)
	{
		m_cbAreaFlash=0xFF;
		RefreshGameView();
	}

	return __super::OnRButtonDown(nFlags,Point);

}
//鼠标消息
bool CGameClientView::PngButtonOnLButtonDown(UINT nFlags, CPoint Point)
{
	bool bInButton=false;

	//绘画按钮
	for (int i=0;i<BUTTON_COUNT;i++)
	{
		if (m_RectBt[i].PtInRect(Point))
		{	
			bInButton=true;

			if (m_btState[i]==enDisabled||m_bBtShow[i]==false) continue;
			m_btState[i]=enDown;
			ReflashButton(i);

		}
	}
	return bInButton;

}
//鼠标消息
void CGameClientView::PngButtonOnLButtonUp(UINT nFlags, CPoint Point)
{
	//绘画按钮
	for (int i=0;i<BUTTON_COUNT;i++)
	{
		if (m_RectBt[i].PtInRect(Point))
		{

			if (m_btState[i]==enDisabled||m_bBtShow[i]==false) continue;
			ButtonAction(i);

			if (m_btState[i]==enDown)
			{
// 				if(i==16 || i==17) m_btState[i]=enDisabled;
// 				else 
				m_btState[i]=enNormal;
			}
				
			ReflashButton(i);
			return;
		}
	}
}
//鼠标移动
void CGameClientView::PngButtonOnMouseMove(UINT nFlags, CPoint Point)
{
	//绘画按钮
	for (int i=0;i<BUTTON_COUNT;i++)
	{
		//绘画按钮
		for (int i=0;i<BUTTON_COUNT;i++)
		{
			if (m_btState[i]==enDisabled) continue;

			if (m_RectBt[i].PtInRect(Point))
			{
				if (!m_bTrackmouse[i])
				{
// 					if( (i==16 || i==17) && m_btState[i]==enDisabled )
// 					{
// 					}
// 					else	
					m_btState[i]=enMouseOver;

					m_bTrackmouse[i]=true;
					ReflashButton(i);
				}	
			}
			else
			{
				if (m_bTrackmouse[i])
				{
					m_bTrackmouse[i]=false;
// 					if( (i==16 || i==17) && m_btState[i]==enDisabled )
// 					{
// 					}
// 					else	
					m_btState[i]=enNormal;
					ReflashButton(i);
				}

			}

		}
	}
}
//光标消息
BOOL CGameClientView::OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage)
{
	//获取区域
	CPoint MousePoint;
	GetCursorPos(&MousePoint);
	ScreenToClient(&MousePoint);
	if (m_lCurrentJetton!=0L)
	{
		BYTE cbJettonArea=GetJettonArea(MousePoint);

		//设置变量
		if ( m_cbAreaFlash != cbJettonArea )
		{
			m_cbAreaFlash = cbJettonArea;
			InvalidGameView(0,0,0,0);
		}
		//区域判断
		if ( cbJettonArea==0xFF || m_pGameClientDlg->GetGameStatus()!=GS_PLACE_JETTON)
		{
			SetCursor(LoadCursor(NULL,IDC_ARROW));
			return TRUE;
		}

		//设置光标
		switch (m_lCurrentJetton)
		{
		case 100:
			{
				SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_SCORE_100)));
				return TRUE;
			}
		case 1000:
			{
				SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_SCORE_1000)));
				return TRUE;
			}
		case 10000:
			{
				SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_SCORE_1W)));
				return TRUE;
			}
// 		case 50000:
// 			{
// 				SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_SCORE_5W)));
// 				return TRUE;
// 			}
		case 100000:
			{
				SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_SCORE_10W)));			
				return TRUE;
			}
// 		case 500000:
// 			{
// 				SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_SCORE_50W)));			
// 				return TRUE;
// 			}
		case 1000000:
			{
				SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_SCORE_100W)));			
				return TRUE;
			}
		case 5000000:
			{
				SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_SCORE_500W)));			
				return TRUE;
			}
		case 10000000:
			{
				SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_SCORE_1000W)));			
				return TRUE;
			}
// 		case 50000000:
// 			{
// 				SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_SCORE_5000W)));			
// 				return TRUE;
// 			}
// 		case 100000000:
// 			{
// 				SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_SCORE_1Y)));			
// 				return TRUE;
// 			}
			RefreshGameView();
		}

		return TRUE;
	}
	return __super::OnSetCursor(pWnd, nHitTest, uMessage);
}
//轮换庄家
void CGameClientView::ShowChangeBanker( bool bChangeBanker )
{

}
//上庄按钮
void CGameClientView::OnApplyBanker()
{
	m_bBtShow[14] = true;
	m_bBtShow[13] = false;
	SendEngineMessage(IDM_APPLY_BANKER,1,0);


}
//下庄按钮
void CGameClientView::OnCancelBanker()
{
	m_bBtShow[14] = false;
	m_bBtShow[13] = true;
	SendEngineMessage(IDM_APPLY_BANKER,0,0);

}
//艺术字体
void CGameClientView::DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, int nXPos, int nYPos)
{

}
//庄家信息
void CGameClientView::SetBankerInfo(DWORD dwBankerUserID, LONGLONG lBankerScore)
{
	//庄家椅子号
	WORD wBankerUser=INVALID_CHAIR;

	//查找椅子号
	if (0!=dwBankerUserID)
	{
		for (WORD wChairID=0; wChairID<MAX_CHAIR; ++wChairID)
		{
			//tagUserData const *pUserData=GetUserInfo(wChairID);
			IClientUserItem * pClientUserItem=GetClientUserItem(wChairID);
			if (NULL!=pClientUserItem && dwBankerUserID==pClientUserItem->GetUserID())
			{
				wBankerUser=wChairID;
				break;
			}
		}
	}

	//切换判断
	if (m_wBankerUser!=wBankerUser)
	{
		m_wBankerUser=wBankerUser;
		m_wBankerTime=0L;
		m_lBankerWinScore=0L;	
		m_lTmpBankerWinScore=0L;
	}
	m_lBankerScore=lBankerScore;
}
//绘画标识
void CGameClientView::DrawWinFlags(CDC * pDC)
{
	//非空判断
	if (m_nRecordLast==m_nRecordFirst) return;
	int nIndex = m_nScoreHead;
	int nDrawCount=0;
	//位置变量
	int static nYPos=0,nXPos=0;
	while ( nIndex != m_nRecordLast && ( m_nRecordLast!=m_nRecordFirst ) && nDrawCount < MAX_FALG_COUNT )
	{
		//胜利标识
		tagClientGameRecord &ClientGameRecord = m_GameRecordArrary[nIndex];
		nYPos=m_LuZiStart.y;
		nXPos=m_LuZiStart.x+nDrawCount*m_HistroyCar.GetWidth()/8;
		//胜利标识
		int static nFlagsIndex=0;

		int nAnimalIndex=GetAnimalIndex(ClientGameRecord.cbAnimal);
		
		//绘画标识
		m_HistroyCar.DrawImage(pDC,nXPos,nYPos,m_HistroyCar.GetWidth()/8,m_HistroyCar.GetHeight(),nAnimalIndex*m_HistroyCar.GetWidth()/8,0);
		//移动下标
		nIndex = (nIndex+1) % MAX_SCORE_HISTORY;
		nDrawCount++;
	}
// 	if(nDrawCount!=0)
// 	{
// 		m_RecordLast.DrawImage(pDC,nXPos,nYPos,m_RecordLast.GetWidth()/2,m_RecordLast.GetHeight(),0,0);
// 	}
}
//移动按钮
void CGameClientView::OnScoreMoveL()
{

}
//移动按钮
void CGameClientView::OnScoreMoveR()
{

}
void CGameClientView::GetAllWinArea(BYTE bcWinArea[],BYTE bcAreaCount,BYTE InArea)
{
	if (InArea==0xFF)
	{
		return ;
	}
	ZeroMemory(bcWinArea,bcAreaCount);


	LONGLONG lMaxSocre = 0;

	for (int i = 0;i<ANIMAL_COUNT;i++)
	{
		BYTE bcOutCadDataWin[AREA_COUNT];
		BYTE bcData[1];
		bcData[0]=i+1;
		m_GameLogic.GetCardType(bcData[0],1,bcOutCadDataWin);
		for (int j= 0;j<AREA_COUNT;j++)
		{

			if(bcOutCadDataWin[j]>1&&j==InArea-1)
			{
				LONGLONG Score = 0; 
				for (int nAreaIndex=1; nAreaIndex<=AREA_COUNT; ++nAreaIndex) 
				{
					if(bcOutCadDataWin[nAreaIndex-1]>1)
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
//最大下注
LONGLONG CGameClientView::GetUserMaxJetton(BYTE cbJettonArea)
{
	if (cbJettonArea==0xFF)
		return 0;

	//已下注额
	LONGLONG lNowJetton = 0;
	ASSERT(AREA_COUNT<=CountArray(m_lUserJettonScore));
	for (int nAreaIndex=1; nAreaIndex<=AREA_COUNT; ++nAreaIndex) lNowJetton += m_lUserJettonScore[nAreaIndex];
	//庄家金币
	LONGLONG lBankerScore = 0x7fffffffffffffff;
	if (m_wBankerUser!=INVALID_CHAIR)
		lBankerScore = m_lBankerScore;

	BYTE bcWinArea[AREA_COUNT];
	LONGLONG LosScore = 0;
	LONGLONG WinScore = 0;

	GetAllWinArea(bcWinArea,AREA_COUNT,cbJettonArea);

	for (int nAreaIndex=1; nAreaIndex<=AREA_COUNT; ++nAreaIndex) 
	{
		if(bcWinArea[nAreaIndex-1]>1)
		{
			LosScore+=m_lAllJettonScore[nAreaIndex]*(bcWinArea[nAreaIndex-1]);
		}
		else
		{
			if(bcWinArea[nAreaIndex-1]==0)
			{
				WinScore+=m_lAllJettonScore[nAreaIndex];
			}
		}
	}

	LONGLONG lTemp = lBankerScore;
	lBankerScore = lBankerScore + WinScore - LosScore;

	if ( lBankerScore < 0 )
	{
		if (m_wBankerUser!=INVALID_CHAIR)
		{
			lBankerScore = m_lBankerScore;
		}
		else
		{
			lBankerScore = 0x7fffffffffffffff;
		}
	}

	//区域限制
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

	//庄家限制
	lMeMaxScore=min(lMeMaxScore,lBankerScore/(bcWinArea[cbJettonArea-1]));

	//非零限制
	lMeMaxScore = max(lMeMaxScore, 0);

	return lMeMaxScore;
}
//成绩设置
void CGameClientView::SetGameScore(LONGLONG lMeCurGameScore, LONGLONG lMeCurGameReturnScore, LONGLONG lBankerCurGameScore)
{
	m_lMeCurGameScore=lMeCurGameScore;
	m_lMeCurGameReturnScore=lMeCurGameReturnScore;
	m_lBankerCurGameScore=lBankerCurGameScore;	
}
//开始发牌
void CGameClientView::DispatchCard()
{
	//设置标识
	m_bNeedSetGameRecord=true;
}
//结束发牌
void CGameClientView::FinishDispatchCard( bool bRecord /*= true*/ )
{
	//完成判断
	if (m_bNeedSetGameRecord==false) return;

	//设置标识
	m_bNeedSetGameRecord=false;

	//删除定时器
	//KillTimer(IDI_DISPATCH_CARD);

	BYTE  bcResulteOut[AREA_COUNT];
	memset(bcResulteOut,0,AREA_COUNT);

	m_GameLogic.GetCardType(m_cbTableCard,1,bcResulteOut);

	//保存记录
	if (bRecord)
	{
		SetGameHistory(m_cbTableCard);
	}

	//累计积分
	m_lMeStatisticScore+=m_lMeCurGameScore;
	m_lBankerWinScore=m_lTmpBankerWinScore;
}
//闪烁动物
void CGameClientView::FlashLightAnimal(CDC *pDC)
{
	//合法判断
	if(m_bFlashResult&&!m_blRungingCar)
	{
		int AnimalIndex=GetAnimalIndex(m_CarIndex+1);

		if(AnimalIndex>=2 && AnimalIndex<=5)
			m_SelPng.DrawImage(pDC,m_BaseAnimalPt[m_CarIndex].x-5,m_BaseAnimalPt[m_CarIndex].y-5,m_SelPng.GetWidth()/8,m_SelPng.GetHeight()/4,AnimalIndex*m_SelPng.GetWidth()/8,0);
		else
			m_SelPng.DrawImage(pDC,m_BaseAnimalPt[m_CarIndex].x-11,m_BaseAnimalPt[m_CarIndex].y-15,m_SelPng.GetWidth()/8,m_SelPng.GetHeight()/4,AnimalIndex*m_SelPng.GetWidth()/8,0);
	}
}
//全部闪烁
void CGameClientView::FlashAllAnimal(CDC *pDC)
{
	//合法判断
	if(m_bFlashAllArea)
	{
		for(int i=0;i<ANIMAL_COUNT;i++)
		{
			int AnimalIndex=GetAnimalIndex(i+1);
			if(AnimalIndex>=2 && AnimalIndex<=5)
				m_SelPng.DrawImage(pDC,m_BaseAnimalPt[i].x-5,m_BaseAnimalPt[i].y-5,m_SelPng.GetWidth()/8,m_SelPng.GetHeight()/4,AnimalIndex*m_SelPng.GetWidth()/8,0);
			else
				m_SelPng.DrawImage(pDC,m_BaseAnimalPt[i].x-11,m_BaseAnimalPt[i].y-15,m_SelPng.GetWidth()/8,m_SelPng.GetHeight()/4,AnimalIndex*m_SelPng.GetWidth()/8,0);
		}
	}
}
//区域变亮
void CGameClientView::JettonAreaLight(CDC * pDC)
{
	//合法判断
	if (m_cbAreaFlash==0xFF) return;
	//if (m_pGameClientDlg->GetGameStatus()!=GS_PLACE_JETTON) return;

	int nIndex = 0;
	if(m_cbAreaFlash==ID_MASERATI)		nIndex=3;
	else if(m_cbAreaFlash==ID_FERRARI) nIndex=1;
	else if(m_cbAreaFlash==ID_LAMBORGHINI) nIndex=2;
	else if(m_cbAreaFlash==ID_PORSCHE) nIndex=0;
	else if(m_cbAreaFlash==ID_LANDROVER) nIndex=7;
	else if(m_cbAreaFlash==ID_BMW) nIndex=4;
	else if(m_cbAreaFlash==ID_JAGUAR) nIndex=6;
	else if(m_cbAreaFlash==ID_BENZ) nIndex=5;
	
	if(nIndex<4) 
	{
		m_AearMaskLeft.DrawImage(pDC,m_RectJettonScore[nIndex].left,m_RectJettonScore[nIndex].top-8,m_AearMaskLeft.GetWidth()-50,m_AearMaskLeft.GetHeight(),50,0);
	}
	else
	{
		m_AearMaskRight.DrawImage(pDC,m_RectJettonScore[nIndex].right-m_AearMaskRight.GetWidth()+50,m_RectJettonScore[nIndex].top-8,m_AearMaskRight.GetWidth()-50,m_AearMaskRight.GetHeight(),50,0);
	}
}
////控件命令
//BOOL CGameClientView::OnCommand(WPARAM wParam, LPARAM lParam)
//{
//	return TRUE;
//}
//重置状态
void CGameClientView::ResetButtonState()
{

}
//我的位置
void CGameClientView::SetMeChairID(DWORD dwMeUserID)
{
	//查找椅子号
	for (WORD wChairID=0; wChairID<MAX_CHAIR; ++wChairID)
	{
		IClientUserItem * pClientUserItem=GetClientUserItem(wChairID);
		if (NULL!=pClientUserItem && dwMeUserID==pClientUserItem->GetUserID())
		{
			m_wMeChairID=wChairID;
			break;
		}
	}
}
//绘画庄家
void CGameClientView::DrawBankerInfo(CDC *pDC,int nWidth,int nHeight)
{
	//获取用户
	IClientUserItem * pClientUserItem=m_wBankerUser==INVALID_CHAIR ? NULL :GetClientUserItem(m_wBankerUser);

	tagUserInfo* pUserInfo=NULL;
	if (pClientUserItem!=NULL)
	{
		pUserInfo =  pClientUserItem->GetUserInfo();
	}

	
	TCHAR Text[256];
	//庄家名字
	myprintf(Text,256,TEXT("%s"),pClientUserItem==NULL?(m_bEnableSysBanker?TEXT("系统坐庄"):TEXT("无人坐庄")):pUserInfo->szNickName);
	CDFontEx::DrawText(this,pDC,14, 800,Text,CRect(95,50,225,70),RGB(255,255,255),DT_END_ELLIPSIS | DT_CENTER | DT_TOP|DT_SINGLELINE );

	//庄家筹码
	myprintf(Text,256,TEXT("%I64d"),pClientUserItem==NULL?0:pClientUserItem->GetUserScore()*m_nMultule);
	CString Str = NumberStringWithSpace(Text);
	CDFontEx::DrawText(this,pDC,14, 800,Str,CRect(95,80,225,100),RGB(255,245,0),DT_END_ELLIPSIS | DT_CENTER | DT_TOP|DT_SINGLELINE );

	//庄家成绩
	if(m_lBankerWinScore>=0)
	{
		myprintf(Text,256,TEXT("%I64d"),m_lBankerWinScore*m_nMultule);
		Str.Format(_T("+%s"),NumberStringWithSpace(Text));
		CDFontEx::DrawText(this,pDC,14, 800,Str,CRect(95,105,225,125),RGB(0,245,0),DT_END_ELLIPSIS | DT_CENTER | DT_TOP|DT_SINGLELINE );
	}
	else
	{
		myprintf(Text,256,TEXT("%I64d"),-m_lBankerWinScore*m_nMultule);
		Str.Format(_T("-%s"),NumberStringWithSpace(Text));
		CDFontEx::DrawText(this,pDC,14, 800,Str,CRect(95,105,225,125),RGB(245,0,0),DT_END_ELLIPSIS | DT_CENTER | DT_TOP|DT_SINGLELINE );
	}


	//m_CurrentBankerTime.DrawImage(pDC,160,125);

	myprintf(Text,256,TEXT("%d"),m_wBankerTime);
	CDFontEx::DrawText(this,pDC,14, 800,Text,CRect(95,130,225,150),RGB(126,245,223),DT_END_ELLIPSIS | DT_CENTER | DT_TOP|DT_SINGLELINE );
}
//基本场景
void CGameClientView::DrawBaseScene(CDC *pDC,int nWidth, int nHeight)
{

}
//绘画玩家
void CGameClientView::DrawMeInfo(CDC *pDC,int nWidth,int nHeight)
{
	if (INVALID_CHAIR==m_wMeChairID)
		return;
	//获取用户
	IClientUserItem * pClientUserItem=GetClientUserItem(m_wMeChairID);

	m_UserInfo.DrawImage(pDC,0,nHeight-m_UserInfo.GetHeight());
	TCHAR Text[256];
	
	//玩家名字
	myprintf(Text,256,TEXT("%s"),pClientUserItem->GetNickName());
	CDFontEx::DrawText(this,pDC,14, 800,Text,CRect(85,nHeight-120,175,nHeight-100),RGB(255,255,255),DT_END_ELLIPSIS | DT_CENTER | DT_TOP|DT_SINGLELINE );
	
	LONGLONG lBet=0;
	for(int i=1;i<AREA_COUNT+1;i++)
	{
		lBet +=m_lUserJettonScore[i];
	}
	//玩家筹码
	myprintf(Text,256,TEXT("%I64d"),(pClientUserItem->GetUserScore()*m_nMultule-lBet));
	CString Str = NumberStringWithSpace(Text);
	CDFontEx::DrawText(this,pDC,14, 800,Str,CRect(85,nHeight-80,175,nHeight-60),RGB(255,245,0),DT_END_ELLIPSIS | DT_CENTER | DT_TOP|DT_SINGLELINE );

	//玩家成绩
	if(m_lMeStatisticScore>=0)
	{
		myprintf(Text,256,TEXT("%I64d"),m_lMeStatisticScore*m_nMultule);
		Str.Format(_T("+%s"),NumberStringWithSpace(Text));
		CDFontEx::DrawText(this,pDC,14, 800,Str,CRect(85,nHeight-50,175,nHeight-30),RGB(0,245,0),DT_END_ELLIPSIS | DT_CENTER | DT_TOP|DT_SINGLELINE );
	}
	else
	{
		myprintf(Text,256,TEXT("%I64d"),-m_lMeStatisticScore*m_nMultule);
		Str.Format(_T("-%s"),NumberStringWithSpace(Text));
		CDFontEx::DrawText(this,pDC,14, 800,Str,CRect(85,nHeight-50,175,nHeight-30),RGB(255,0,0),DT_END_ELLIPSIS | DT_CENTER | DT_TOP|DT_SINGLELINE );
	}


}
//降低筹码
void CGameClientView::ReduceJettonNumber()
{
	if (m_lCurrentJetton==10000000L)
	{
		m_lCurrentJetton=5000000L;
	}
	else if(m_lCurrentJetton==5000000L)
	{
		m_lCurrentJetton==1000000L;
	}
	else if (m_lCurrentJetton==1000000L)
	{
		m_lCurrentJetton=100000L;
	}
	else if (m_lCurrentJetton==100000L)
	{
		m_lCurrentJetton=10000L;
	}
// 	else if (m_lCurrentJetton==50000L)
// 	{
// 		m_lCurrentJetton=10000L;
// 	}
	else if (m_lCurrentJetton==10000L)
	{
		m_lCurrentJetton=1000L;
	}
	else if(m_lCurrentJetton==1000L)
	{
		m_lCurrentJetton=100L;
	}
	RefreshGameView();
}
//管理员控制
void CGameClientView::OpenAdminWnd()
{
	if(!m_pAdminControl->IsWindowVisible()) 
	{
		m_pAdminControl->CenterWindow();
		CRect rect;
		m_pAdminControl->GetWindowRect(&rect);
		rect.left-=110;
		rect.right-=110;
		m_pAdminControl->MoveWindow(rect);
		m_pAdminControl->ClearText();
		m_pAdminControl->ShowWindow(SW_SHOW);
	}
	else 
		m_pAdminControl->ShowWindow(SW_HIDE);
}
//控制下注检测
void CGameClientView::ControlBetDetection( BYTE cbViewIndex, LONGLONG lScoreCount, WORD wChair )
{
	if (m_pAdminControl == NULL || wChair >= GAME_PLAYER )
		return;

	//获取金币
	m_lAllPlayBet[cbViewIndex] += lScoreCount;
}
//执行剩余所有的缓冲动画
void CGameClientView::PerformAllBetAnimation()
{

}
//更新视图
void CGameClientView::RefreshGameView()
{
	CRect rect;
	GetClientRect(&rect);
	InvalidGameView(rect.left,rect.top,rect.Width(),rect.Height());

}
LRESULT CGameClientView::OnSendMessage(WPARAM wParam,LPARAM lParam)
{
	CString str, mes;
	GetDlgItem(IDC_CHAT_INPUT)->GetWindowText(str);
	if(str.GetLength()<1)
	{
		return 0;
	}

	IClientKernel * m_pIClientKernel;
	//查询接口
	CGlobalUnits * pGlobalUnits = CGlobalUnits::GetInstance();
	ASSERT(pGlobalUnits->QueryGlobalModule(MODULE_CLIENT_KERNEL,IID_IClientKernel,VER_IClientKernel)!=NULL);
	m_pIClientKernel=(IClientKernel *)pGlobalUnits->QueryGlobalModule(MODULE_CLIENT_KERNEL,IID_IClientKernel,VER_IClientKernel);

	m_pIClientKernel->SendUserChatMessage(0,str,m_texColor);

	GetDlgItem(IDC_CHAT_INPUT)->SetWindowText(TEXT(""));
	GetDlgItem(IDC_CHAT_INPUT)->SetFocus();
	return 0;
}
void CGameClientView::InsertAllChatMessage(ChatMsg *pMsg,int nMessageType)
{
	switch(nMessageType)
	{
	case enSystem:
		{
			m_ChatMessage.InsertSystemString(pMsg->pszString);
			break;
		}
	case enChat:			
		{
			m_ChatMessage.InsertUserChat(pMsg->pszSendUser,pMsg->pszString,pMsg->crColor);
			break;
		}
	case enNormalString:
		{
			m_ChatMessage.InsertNormalString(pMsg->pszString);
			break;
		}
	case enPrompt:
		{
			m_ChatMessage.InsertPromptString(pMsg->pszString);
			break;
		}
	case enAffiche:
		{
			m_ChatMessage.InsertAfficheString(pMsg->pszString);
			break;
		}
	case enCustom:
		{
			m_ChatMessage.InsertCustomString(pMsg->pszString,pMsg->crColor);
			break;
		}
	case enWhisperChat:
		{
			m_ChatMessage.InsertWhisperChat(pMsg->pszSendUser,pMsg->pszString,pMsg->crColor,true);
			break;
		}
	case enUserTrumpet:
		{
			m_ChatMessage.InsertUserTrumpet(pMsg->pszSendUser,pMsg->pszString,pMsg->crColor);
			break;
		}
	case enUserTyphon:
		{
			m_ChatMessage.InsertUserTyphon(pMsg->pszSendUser,pMsg->pszString,pMsg->crColor);
			break;
		}
	case enExpression:
		{
			m_ChatMessage.InsertExpression(pMsg->pszSendUser,pMsg->pszString);
			break;
		}
	}
	m_ChatMessage.PostMessage(WM_VSCROLL,   SB_BOTTOM,0);
}
//颜色选择
void CGameClientView::SelectColor()
{
	//变量定义
	ASSERT(CGlobalUnits::GetInstance()!=NULL);
	CGlobalUnits * pGlobalUnits=CGlobalUnits::GetInstance();

	//变量定义
	INT nWidth=72,nHeight=16;
	COLORREF crColor[]={RGB(255,0,0),RGB(255,255,0),RGB(128,255,0),RGB(0,255,255),
		RGB(0,0,128),RGB(0,128,0),RGB(128,0,128),RGB(128,0,0),RGB(0,0,0)};

	//构造颜色
	CImage ImageColor[CountArray(crColor)];
	for (INT i=0;i<CountArray(ImageColor);i++)
	{
		//创建位图
		ImageColor[i].Create(nWidth,nHeight,16);

		//获取设备
		CImageDC ImageDC(ImageColor[i]);
		CDC * pDC=CDC::FromHandle(ImageDC);

		//绘画位图
		pDC->FillSolidRect(2,2,nWidth-4,nHeight-4,crColor[i]);
		pDC->Draw3dRect(0,0,nWidth,nHeight,RGB(0,0,0),RGB(0,0,0));
		pDC->Draw3dRect(1,1,nWidth-2,nHeight-2,RGB(255,255,255),RGB(255,255,255));
	}

	//创建菜单
	CSkinMenu ColorMenu;
	ColorMenu.CreateMenu();

	//构造菜单
	ColorMenu.AppendMenu(IDM_SELECT_MORE_COLOR,TEXT("选择其他颜色"));

	//颜色选项
	ColorMenu.AppendSeparator();
	for (INT i=0;i<CountArray(ImageColor);i++) 
	{
		ColorMenu.AppendMenu(IDM_SELECT_CHAT_COLOR+i,ImageColor[i]);
		if (m_texColor==crColor[i]) ColorMenu.CheckMenuItem(IDM_SELECT_CHAT_COLOR+i,MF_BYCOMMAND|MF_CHECKED);
	}

	//弹出菜单
	CRect rcButton=m_RectBt[20];
	ClientToScreen(&rcButton);
	ColorMenu.TrackPopupMenu(rcButton.left,rcButton.top,this);
}
//喇叭按钮
void CGameClientView::OnProperty()
{
	//变量定义
	CGamePropertyManager * pGamePropertyManager=CGamePropertyManager::GetInstance();
	CGamePropertyItem * pGamePropertyTrumpet = pGamePropertyManager->GetPropertyItem(PROPERTY_ID_TRUMPET);
	CGamePropertyItem * pGamePropertyTyphon  = pGamePropertyManager->GetPropertyItem(PROPERTY_ID_TYPHON);

	//有效判断
	if(pGamePropertyTrumpet==NULL && pGamePropertyTyphon==NULL)
	{
		CInformation Information(this);
		Information.ShowMessageBox(TEXT("当前喇叭功能未启用！"),MB_ICONINFORMATION,30L);
		return;
	}

	//创建窗口
	//创建窗口
	if (m_pTrumpetItem==NULL)
	{
		m_pTrumpetItem=new CDlgTrumpetItem;
		m_pTrumpetItem->SetGameTrumpetSink(QUERY_ME_INTERFACE(IUnknownEx));
	}
	IClientUserItem *pMeUserItem = GetClientUserItem(m_wMeChairID);
	if(pMeUserItem != NULL)
	{
		//显示窗口
		m_pTrumpetItem->ShowTrumpetWindow(AfxGetMainWnd(),pMeUserItem);
		CRect rt,rtTrumpetItem;
 		GetWindowRect(&rt);
		m_pTrumpetItem->GetClientRect(&rtTrumpetItem);
 		m_pTrumpetItem->SetWindowPos(NULL,rt.left+(rt.Width()-rtTrumpetItem.Width())/2,rt.top+(rt.Height()-rtTrumpetItem.Height())/2,rtTrumpetItem.Width(),rtTrumpetItem.Height(),SWP_NOZORDER|SWP_SHOWWINDOW);
	}

}
void CGameClientView::SetStateTips(enTipState newState,bool bTimer)
{

}
void CGameClientView::ButtonAction( int buttonIndex)
{
	if (buttonIndex==0) OnButtonClose();
	else if (buttonIndex==1) OnButtonMin();
	else if (buttonIndex==2) OnButtonGameOption();
	else if (buttonIndex==3) OnButton100();
	else if (buttonIndex==4) OnButton1000();
	else if (buttonIndex==5) OnButton1w();
	else if (buttonIndex==6) OnButton10w();
	else if (buttonIndex==7) OnButton100w();
	else if (buttonIndex==8) OnButton500w();
	else if (buttonIndex==9) OnButton1000W();
	else if (buttonIndex==10) OnButtonXUYA();
	else if (buttonIndex==11) OnButtonGoBanker();
	else if (buttonIndex==12) OnButtonListUp();
	else if (buttonIndex==13) OnApplyBanker();
	else if (buttonIndex==14) OnCancelBanker();
	else if (buttonIndex==15) OnButtonListDown();
	else if (buttonIndex==16) OnButtonSwitchChat();
	else if (buttonIndex==17) OnButtonSwitchUserList();
	else if (buttonIndex==18) OnSendMessage(0,0);
// 	else if (buttonIndex==19) OnExpression();
// 	else if (buttonIndex==20) SelectColor();
// 	else if (buttonIndex==21) OnProperty();
}
#define IDC_MIN						100									//最小按钮
#define IDC_MAX						101									//最大按钮
#define IDC_CLOSE					103									//关闭按钮
#define IDC_STANDARD				104									//恢复按钮

void CGameClientView::OnButtonClose()
{
#ifdef _DEBUG
	//构造数据
	CMessageTipDlg GameTips;

	//配置数据
	if (GameTips.DoModal()==IDOK)
	{
		AfxGetMainWnd()->PostMessage(WM_COMMAND,IDC_CLOSE);
	} 
	else 
		return;

	return;
#endif

	
	if (m_pGameClientDlg->GetGameStatus()!=GAME_STATUS_FREE)
	{
		//构造数据
		CMessageTipDlg GameTips;

		//配置数据
		if (GameTips.DoModal()==IDOK)
		{
			AfxGetMainWnd()->PostMessage(WM_COMMAND,IDC_CLOSE);
		} 
		else 
			return;
	}
	else
 		AfxGetMainWnd()->PostMessage(WM_COMMAND,IDC_CLOSE);
	
}
void CGameClientView::OnButtonMax()
{
	AfxGetMainWnd()->PostMessage(WM_COMMAND,IDC_MAX,0);
// 	m_bBtShow[1]=false;	
// 	m_bBtShow[2]=true;
	ReflashButton(1);
}
void CGameClientView::OnButtonStandard()
{
	
	AfxGetMainWnd()->PostMessage(WM_COMMAND,IDC_STANDARD,0);
//	m_bBtShow[1]=true;
//	m_bBtShow[2]=false;	
	ReflashButton(1);
}
void CGameClientView::OnButtonMin()
{
	AfxGetMainWnd()->PostMessage(WM_COMMAND,IDC_MIN,0);
}
void CGameClientView::OnButtonGameOption()
{
	SendEngineMessage(IDM_GAME_OPTIONS,1,0);
}
void CGameClientView::OnButtonAskForBanker()
{
	SendEngineMessage(IDM_APPLY_BANKER,1,0);
}
void CGameClientView::OnButtonGiveupBanker()
{
	SendEngineMessage(IDM_APPLY_BANKER,0,0);
}
 //切换用户列表	
void CGameClientView::OnButtonSwitchUserList()
{
	if(m_nSwitchIndex==1) return;	
	m_nSwitchIndex=1;
	m_bBtShow[12]=false;
	m_bBtShow[13]=false;
	m_bBtShow[14]=false;
	m_bBtShow[15]=false;
	m_UserList.ShowWindow(SW_SHOW);

	InvalidGameView(0,0,0,0);

// 	if(m_btState[16] == enDisabled)
// 	{
// 		m_btState[16] = enNormal;
// 		ReflashButton(16);
// 		m_ChatMessage.ShowWindow(SW_HIDE);
// 		m_ChatInput.ShowWindow(SW_HIDE);
// 		m_UserList.ShowWindow(SW_SHOW);
// 		for(int i=0;i<4;i++)
// 		{
// 			m_bBtShow[18+i] = false;
// 			ReflashButton(18+i);
// 		}
// 	}
}
//切换聊天
void CGameClientView::OnButtonSwitchChat()
{
	if(m_nSwitchIndex==0) return;
	m_nSwitchIndex=0;
	m_bBtShow[12]=true;
	m_bBtShow[15]=true;
	if(m_pGameClientDlg->m_bMeApplyBanker || m_wBankerUser==m_wMeChairID)
	{
		m_bBtShow[13]=false;
		m_bBtShow[14]=true;
	}
	else
	{
		m_bBtShow[14]=false;
		m_bBtShow[13]=true;
	}
	m_UserList.ShowWindow(SW_HIDE);
	InvalidGameView(0,0,0,0);

// 	if(m_btState[17] == enDisabled)
// 	{
// 		m_btState[17] = enNormal;
// 		ReflashButton(17);
// 		m_ChatMessage.ShowWindow(SW_SHOW);
// 		m_ChatInput.ShowWindow(SW_SHOW);
// 		m_UserList.ShowWindow(SW_HIDE);
// 		for(int i=0;i<4;i++)
// 		{
// 			m_bBtShow[18+i] = true;
// 			ReflashButton(18+i);
// 		}
// 	}
}
void CGameClientView::OnButtonListUp()
{
	if ( m_nShowValleyIndex > 0 )
		m_nShowValleyIndex--;

	UpdateButtonContron();
	InvalidGameView(0,0,0,0);
}
void CGameClientView::OnButtonListDown()
{
	if( m_nShowValleyIndex < m_ValleysList.GetCount() - 1 )
		m_nShowValleyIndex++;

	UpdateButtonContron();
	InvalidGameView(0,0,0,0);
}
void CGameClientView::OnButtonGoBanker()
{
#ifdef __SPECIAL___
	//获取接口
	CGameClientEngine *pGameClientEngine=CONTAINING_RECORD(this,CGameClientEngine,m_GameClientView);
	IClientKernel *pIClientKernel=(IClientKernel *)pGameClientEngine->m_pIClientKernel;

	if(NULL!=pIClientKernel)
	{
		CRect btRect;
		GetWindowRect(&btRect);
		btRect.right=(btRect.left+btRect.right)/2;

		btRect.bottom=(btRect.top+ btRect.bottom)/2;

		ShowInsureGet(pIClientKernel,CPoint(btRect.right-250,btRect.bottom+100));
	}
#endif
	return;
}
void CGameClientView::OnButton100()
{
	m_lCurrentJetton = 100l;
}
void CGameClientView::OnButton1000()
{
	m_lCurrentJetton=1000L;
}

void CGameClientView::OnButton1w()
{
	m_lCurrentJetton=10000L;
}
void CGameClientView::OnButton10w()
{
	m_lCurrentJetton=100000L;
}
//下注按钮	
void CGameClientView::OnButton100w()
{
	m_lCurrentJetton=1000000L;
}
void CGameClientView::OnButton500w()
{
	m_lCurrentJetton=5000000L;
}
//下注按钮	
void CGameClientView::OnButton1000W()
{
	m_lCurrentJetton=10000000L;
}
//续压按钮	
void CGameClientView::OnButtonXUYA()
{
	SendEngineMessage(IDM_PLACE_XUYA,0,0);

}

void CGameClientView::InsertGameEndInfo()
{

}
//设置全名
void CGameClientView::SetTotalRoomName(CString strName)
{
	m_strTotalRoomName=strName;
}
//刷新按钮
void CGameClientView::ReflashButton(BYTE cbButtonIndex,bool bAll)
{
	if (bAll)
	{
		for (int i=0;i<BUTTON_COUNT;i++)
		{
			InvalidGameView(m_RectBt[i].left,m_RectBt[i].top,m_RectBt[i].Width(),m_RectBt[i].Height());
		}
	}
	else
		InvalidGameView(m_RectBt[cbButtonIndex].left,m_RectBt[cbButtonIndex].top,m_RectBt[cbButtonIndex].Width(),m_RectBt[cbButtonIndex].Height());
}

CString CGameClientView::NumberStringWithSpace(CString strNumber)
{
	CString strTempNum=strNumber;
	int nLength = strTempNum.GetLength();
	for (int i = 0; i < int((nLength-1)/3); i++)
		strTempNum.Insert(nLength - 3*(i+1), _T(","));

	return  strTempNum;
}
HBRUSH CGameClientView::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)

{
	if(pWnd->GetDlgCtrlID()==IDC_CHAT_INPUT)
	{
		CBrush   hbrDlg(RGB(19,15,37)); 
		pDC-> SetBkColor(RGB(19,15,37)); 
		pDC->SetTextColor(RGB(255,255,255));
		return   hbrDlg;
	}

	return NULL;
}
void CGameClientView::OnMoreColor()
{
	//设置颜色
	CColorDialog ColorDialog(m_texColor,CC_FULLOPEN,this);
	if (ColorDialog.DoModal()==IDOK)m_texColor=ColorDialog.GetColor();
}
//选择指定颜色
void CGameClientView::OnSelectChatColor(UINT nCommandID)
{	
	//变量定义
	UINT nIndex=nCommandID-IDM_SELECT_CHAT_COLOR;
	COLORREF crColor[]={RGB(255,0,0),RGB(255,255,0),RGB(128,255,0),RGB(0,255,255),
		RGB(0,0,128),RGB(0,128,0),RGB(128,0,128),RGB(128,0,0),RGB(0,0,0)};

	//选择颜色
	ASSERT(nIndex<CountArray(crColor));
	m_texColor = crColor[nIndex];
}

//命令函数
BOOL CGameClientView::OnCommand(WPARAM wParam, LPARAM lParam)
{
	////变量定义
	//UINT nCommandID=LOWORD(wParam);

	////菜单命令
	//switch (nCommandID)
	//{
	//case IDM_SELECT_MORE_COLOR:			//交谈对象
	//	{
	//		return TRUE;
	//	}
	//}
	return __super::OnCommand(wParam,lParam);
}
//更新按钮
VOID CGameClientView::UpdateButtonContron()
{
	//更新界面
	bool bEnableDown = false;
	bool bEnableUp = false;
	INT nUserCount = m_ValleysList.GetCount();

	if(nUserCount <= 3)
	{
		m_nShowValleyIndex = 0;
		bEnableDown = false;
		bEnableUp = false;
	}
	else
	{
		bEnableDown = ((m_nShowValleyIndex + 3) < nUserCount)?true:false;
		bEnableUp = (m_nShowValleyIndex > 0)?true:false;
	}
	if(bEnableUp) m_btState[12] = enNormal;
	else	m_btState[12] = enDisabled;

	if(bEnableDown) m_btState[15] = enNormal;
	else	m_btState[15] = enDisabled;
}
//表情按钮
void CGameClientView::OnExpression()
{
	//创建窗口
	if (m_pExpressionControl==NULL)
	{
		m_pExpressionControl=new CExpressionControl;
	}

	//显示窗口
	//弹出菜单
	CRect rcButton=m_RectBt[19];
	ClientToScreen(&rcButton);
	m_pExpressionControl->ShowExpression(this,rcButton.left,rcButton.top,this);

}

//表情事件
VOID CGameClientView::OnExpressionSelect(CExpression * pUserExpression, tagExpressionInfo * pExpressionInfo)
{
	if(pUserExpression->GetIndex()==INVALID_WORD)  return;
	//获取接口
	CGameClientEngine *pGameClientEngine=CONTAINING_RECORD(this,CGameClientEngine,m_GameClientView);
	IClientKernel *pIClientKernel=(IClientKernel *)pGameClientEngine->m_pIClientKernel;
	if(pIClientKernel!=NULL)
	{
		pIClientKernel->SendUserExpression(0,pUserExpression->GetIndex());
	}
	return;
}