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
#define IDI_MOVECARD_END			105									//�Ƅ��ƽY��
#define IDI_POSTCARD				106									//������ʾ
#define IDI_SHOW_DICE				107									//��ʾ����

#define TIME_SHOW_DICE				600

#define IDC_AUTO_OPEN_CARD			213									//��ť��ʶ
#define IDC_APPY_BANKER				206									//��ť��ʶ
#define IDC_CANCEL_BANKER			207									//��ť��ʶ
#define IDC_VIEW_CHART				211									//��ť��ʶ
#define IDC_BT_USER_LIST			212									//��ť��ʶ
#define IDC_BT_CHAT					213									//��ť��ʶ
#define IDC_SEND_MESSAGE			208									//��ť��ʶ

#define IDC_OPEN_CARD				214									//��ť��ʶ
#define IDC_BANK					215									//��ť��ʶ
#define IDC_CONTINUE_CARD			216									//��ť��ʶ
#define IDC_UP						223									//��ť��ʶ
#define IDC_DOWN					224								//��ť��ʶ
#define IDC_CLOSE_SOUND				225								//��ť��ʶ
#define IDC_OPEN_SOUND				226		
#define IDC_USER_LIST				227									//�ؼ���ʶ
#define IDC_CHAT_DISPLAY			228									//�ؼ���ʶ
#define IDC_CHAT_INPUT				229									//�ؼ���ʶ
//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameClientView, CGameFrameViewGDI)
	ON_WM_TIMER()
	ON_WM_CREATE()
	ON_WM_SETCURSOR()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_WM_LBUTTONDOWN()

	ON_MESSAGE(WM_VIEWLBTUP,OnViLBtUp)
	ON_MESSAGE(WM_ANIMATIONFINISH,OnAnimatEnd)

	ON_BN_CLICKED(IDC_UP, OnUp)
	ON_BN_CLICKED(IDC_DOWN, OnDown)
	ON_BN_CLICKED(IDC_BT_ADMIN,OpenAdminWnd)
	ON_BN_CLICKED(IDC_APPY_BANKER, OnApplyBanker)
	ON_BN_CLICKED(IDC_CANCEL_BANKER, OnCancelBanker)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameClientView::CGameClientView() : m_CartoonDice(this)
{
	
	//��ע��Ϣ
	ZeroMemory(m_lUserBet,sizeof(m_lUserBet));

	//ȫ����ע
	ZeroMemory(m_lAllUserBet,sizeof(m_lAllUserBet));

	//ׯ����Ϣ
	m_cbPrintCount=0;
	m_bShowRecord=false;
	m_bAnimatFinish=false;
	m_wBankerUser=INVALID_CHAIR;		
	m_wBankerTime=0;
	m_lBankerScore=0L;	
	m_lBankerWinScore=0L;
	m_lTmpBankerWinScore=0;
	m_nShowValleyIndex=0;
	m_bShowList = false;
	m_bShowChat = false;

	//���ֳɼ�
	m_lMeCurGameScore=0L;	
	m_lMeCurGameReturnScore=0L;
	m_lBankerCurGameScore=0L;
	m_lGameRevenue=0L;

	m_bDrawResult=false;
	m_bRecordWinner=true;
	//״̬��Ϣ
	m_nJettonIndex=-1;
	m_lCurrentJetton=0L;
	m_cbAreaFlash=0xFF;
	m_wMeChairID=INVALID_CHAIR;
	m_bShowChangeBanker=false;
	m_bNeedSetGameRecord=false;
	ZeroMemory(m_bWinFlag,sizeof(m_bWinFlag));
	m_bFlashResult=false;
	m_blMoveFinish = false;

	m_lMeCurGameScore=0L;			
	m_lMeCurGameReturnScore=0L;	
	m_lBankerCurGameScore=0L;		

	m_lAreaLimitScore=0L;	

	//λ����Ϣ
	m_nRecordFirst=0;	
	m_nRecordLast=-1;	

    m_nRecordCount=0;
	m_bShowGameResult=false;

	//��ʷ�ɼ�
	m_lMeStatisticScore=0;

	//�ؼ�����
	m_pGameClientEngine=CONTAINING_RECORD(this,CGameClientEngine,m_GameClientView);
    
	
	m_cbGameStatus = GAME_STATUS_FREE;

	return;
}

//��������
CGameClientView::~CGameClientView(void)
{
}


//������Ϣ
int CGameClientView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{

	if (__super::OnCreate(lpCreateStruct)==-1) return -1;

	AfxSetResourceHandle(AfxGetInstanceHandle());

	HINSTANCE hResInstance=AfxGetInstanceHandle();


	//����λͼ
	m_ImageViewFill.LoadFromResource(hResInstance,IDB_VIEW_FILL);
	m_ImageViewBack.LoadFromResource(hResInstance,IDB_VIEW_BACK);
	m_ImageWinFlags.LoadFromResource(hResInstance,IDB_WIN_FLAGS);
	m_ImageJettonView.LoadImage(hResInstance,TEXT("JETTOM_VIEW"));
	m_ImageScoreNumber.LoadFromResource(hResInstance,IDB_SCORE_NUMBER);
	m_ImageMeScoreNumber.LoadFromResource(hResInstance,IDB_ME_SCORE_NUMBER);

	//�߿���Դ
	m_ImageFrame.LoadFromResource(hResInstance,IDB_FRAME_EX);
	m_ImageGameEnd.LoadImage(hResInstance, _T("GAME_END") );

	m_ImageMeBanker.LoadImage(hResInstance, _T("ME_BANKER") );
	m_ImageChangeBanker.LoadImage(hResInstance, _T("CHANGE_BANKER") );
	m_ImageNoBanker.LoadImage(hResInstance, _T("NO_BANKER") );	

	m_ImageTimeFlag.LoadFromResource(hResInstance,IDB_TIME_FLAG);

	//���沿�ֵ�������
	m_PngAreaFrame[0].LoadImage(hResInstance,_T("EFFECT_UP_1"));
	m_PngAreaFrame[1].LoadImage(hResInstance,_T("EFFECT_UP_2"));
	m_PngAreaFrame[2].LoadImage(hResInstance,_T("EFFECT_UP_3"));
	m_PngAreaFrame[3].LoadImage(hResInstance,_T("EFFECT_UP_4"));
	m_PngAreaFrame[4].LoadImage(hResInstance,_T("EFFECT_UP_5"));
	m_PngAreaFrame[5].LoadImage(hResInstance,_T("EFFECT_UP_6"));
	m_PngAreaFrame[6].LoadImage(hResInstance,_T("EFFECT_UP_7"));
	m_PngAreaFrame[7].LoadImage(hResInstance,_T("EFFECT_UP_8"));
	m_PngAreaFrame[13].LoadImage(hResInstance,_T("EFFECT_UP_9"));
	m_PngAreaFrame[12].LoadImage(hResInstance,_T("EFFECT_UP_10"));
	m_PngAreaFrame[11].LoadImage(hResInstance,_T("EFFECT_UP_11"));
	m_PngAreaFrame[10].LoadImage(hResInstance,_T("EFFECT_UP_12"));
	m_PngAreaFrame[9].LoadImage(hResInstance,_T("EFFECT_UP_13"));
	m_PngAreaFrame[8].LoadImage(hResInstance,_T("EFFECT_UP_14"));
	m_PngAreaFrame[14].LoadImage(hResInstance,_T("EFFECT_UP_15"));
	m_PngAreaFrame[50].LoadImage(hResInstance,_T("EFFECT_UP_16"));
	m_PngAreaFrame[51].LoadImage(hResInstance,_T("EFFECT_UP_17"));

	//�м䲿�ֵ�������
	m_PngAreaFrame[28].LoadImage(hResInstance,_T("EFFECT_MID_1"));
	m_PngAreaFrame[27].LoadImage(hResInstance,_T("EFFECT_MID_2"));
	m_PngAreaFrame[26].LoadImage(hResInstance,_T("EFFECT_MID_3"));
	m_PngAreaFrame[25].LoadImage(hResInstance,_T("EFFECT_MID_4"));
	m_PngAreaFrame[24].LoadImage(hResInstance,_T("EFFECT_MID_5"));
	m_PngAreaFrame[23].LoadImage(hResInstance,_T("EFFECT_MID_6"));
	m_PngAreaFrame[22].LoadImage(hResInstance,_T("EFFECT_MID_7"));
	m_PngAreaFrame[21].LoadImage(hResInstance,_T("EFFECT_MID_8"));
	m_PngAreaFrame[20].LoadImage(hResInstance,_T("EFFECT_MID_9"));
	m_PngAreaFrame[19].LoadImage(hResInstance,_T("EFFECT_MID_10"));
	m_PngAreaFrame[18].LoadImage(hResInstance,_T("EFFECT_MID_11"));
	m_PngAreaFrame[17].LoadImage(hResInstance,_T("EFFECT_MID_12"));
	m_PngAreaFrame[16].LoadImage(hResInstance,_T("EFFECT_MID_13"));
	m_PngAreaFrame[15].LoadImage(hResInstance,_T("EFFECT_MID_14"));
	m_PngAreaFrame[43].LoadImage(hResInstance,_T("EFFECT_MID_15"));
	m_PngAreaFrame[42].LoadImage(hResInstance,_T("EFFECT_MID_16"));
	m_PngAreaFrame[41].LoadImage(hResInstance,_T("EFFECT_MID_17"));
	m_PngAreaFrame[40].LoadImage(hResInstance,_T("EFFECT_MID_18"));
	m_PngAreaFrame[39].LoadImage(hResInstance,_T("EFFECT_MID_19"));
	m_PngAreaFrame[38].LoadImage(hResInstance,_T("EFFECT_MID_20"));
	m_PngAreaFrame[37].LoadImage(hResInstance,_T("EFFECT_MID_21"));
	m_PngAreaFrame[36].LoadImage(hResInstance,_T("EFFECT_MID_22"));
	m_PngAreaFrame[35].LoadImage(hResInstance,_T("EFFECT_MID_23"));
	m_PngAreaFrame[34].LoadImage(hResInstance,_T("EFFECT_MID_24"));
	m_PngAreaFrame[33].LoadImage(hResInstance,_T("EFFECT_MID_25"));
	m_PngAreaFrame[32].LoadImage(hResInstance,_T("EFFECT_MID_26"));
	m_PngAreaFrame[31].LoadImage(hResInstance,_T("EFFECT_MID_27"));
	m_PngAreaFrame[30].LoadImage(hResInstance,_T("EFFECT_MID_28"));
	m_PngAreaFrame[29].LoadImage(hResInstance,_T("EFFECT_MID_29"));	

	//���沿�ֵ�������
	m_PngAreaFrame[49].LoadImage(hResInstance,_T("EFFECT_DOWN_1"));
	m_PngAreaFrame[48].LoadImage(hResInstance,_T("EFFECT_DOWN_2"));
	m_PngAreaFrame[47].LoadImage(hResInstance,_T("EFFECT_DOWN_3"));
	m_PngAreaFrame[46].LoadImage(hResInstance,_T("EFFECT_DOWN_4"));
	m_PngAreaFrame[45].LoadImage(hResInstance,_T("EFFECT_DOWN_5"));
	m_PngAreaFrame[44].LoadImage(hResInstance,_T("EFFECT_DOWN_6"));

	m_PngCommer.LoadImage(hResInstance,_T("IDP_NUMMER_COMMER"));
	m_ImageUserBack.LoadImage(hResInstance,TEXT("USER_BACK"));
	m_ImageChatBack.LoadImage(hResInstance,TEXT("CHAT_BACK"));
	m_ImageJettonBack.LoadImage(hResInstance,TEXT("SCORE_BACK"));
	m_ImageMeJetton.LoadImage(hResInstance,TEXT("ME_SCORE_BACK"));

	m_PngRecord.LoadImage(hResInstance,_T("IDP_HISTORY_RECORD"));
	m_PngDice3D.LoadImage(hResInstance,_T("IDP_DICE_3D"));
	m_PngDice2D.LoadImage(hResInstance,_T("IDP_DICE_2D"));
	m_PngFlagsTip.LoadImage(hResInstance,_T("NOTICE"));						//��ע����
	m_PngTimeNumber.LoadImage(hResInstance,_T("IDP_TIME_NUMBER"));						
	m_PngAnimatDice.LoadImage(hResInstance,_T("IDP_DICE_SHACK"));	
	m_PngTimerBackGround.LoadImage(hResInstance,_T("IDP_TIME_BACKGROUND"));
	m_PngScoreNumber.LoadImage(hResInstance,_T("SCORE_NUMBER"));
	m_PngMeScoreNumber.LoadImage(hResInstance,_T("ME_SCORE_NUMBER"));
	m_FrameDice.SetCartoonInfo(AM_GAMEDEAL,8,NULL);									//��ע����֡

	//���ƶ���֡��Ϣ
	int nIndex=0;
	for(nIndex=0;nIndex<7;nIndex++)
		m_FrameDice.SetFrameInfo(nIndex,60,nIndex*150,0,150,150,&m_PngAnimatDice);	

	//�����ؼ�
	CRect rcCreate(0,0,0,0);
	//m_ApplyUser.Create(IDD_DLG_GAME_RECORD,this);
	m_GameRecord.Create(IDD_DLG_GAME_RECORD,this);
	m_GameRecord.ShowWindow(SW_HIDE);
		
	//���밴ť
	m_btLookRecord.Create(NULL,WS_CHILD|WS_DISABLED,rcCreate,this,IDC_BT_LOOK_RECORD);
	m_btCancelBanker.Create(NULL,WS_CHILD|WS_DISABLED,rcCreate,this,IDC_CANCEL_BANKER);
	m_btApplyBanker.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_APPY_BANKER);
	m_btCloseRecord.Create(NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_BT_CLOSE);

	m_btUserList.Create(NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_BT_USER_LIST);
	m_btUserList.SetButtonImage(IDB_BT_USER_LIST,hResInstance,false,false);
	m_btChat.Create(NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_BT_CHAT);
	m_btChat.SetButtonImage(IDB_BT_CHAT,hResInstance,false,false);
	m_btSendMessage.Create(NULL,WS_CHILD|WS_VISIBLE, rcCreate, this, IDC_SEND_MESSAGE);
	m_btSendMessage.SetButtonImage(IDB_BT_SEND_MESSAGE, AfxGetInstanceHandle(), false, true);

	m_btCloseRecord.SetButtonImage(IDB_BT_CLOSE,hResInstance,false,false);
	m_btLookRecord.SetButtonImage(IDB_BT_LOOK_RECORD,hResInstance,false,false);
	m_btApplyBanker.SetButtonImage(IDB_BT_APPLY_BANKER,hResInstance,false,false);
	m_btCancelBanker.SetButtonImage(IDB_BT_CANCEL_APPLY,hResInstance,false,false);

	m_CartoonDice.Create(NULL,NULL,WS_CHILD|WS_VISIBLE,CRect(0,0,174,159),this,IDC_CARTOON_DICE);
	m_CartoonDice.CreateMemDC();

	m_CartoonDice.SetBGImage(IDB_VIEW_BACK,false);

	m_ChatDisplay.Create(ES_AUTOVSCROLL|ES_WANTRETURN|ES_MULTILINE | WS_CHILD | WS_VISIBLE | WS_TABSTOP |WS_VSCROLL, rcCreate, this, IDC_CHAT_DISPLAY);
	m_ChatInput.Create(ES_AUTOHSCROLL|WS_CHILD|WS_VISIBLE|WS_TABSTOP, rcCreate, this, IDC_CHAT_INPUT);

	LOGFONT lf;
	memset(&lf, 0, sizeof(LOGFONT));      
	lf.lfHeight = 15;                      
	mystrcpy(lf.lfFaceName, TEXT("Arial"));

	m_ChatInput.SetLimitText(30);

	m_ChatDisplay.SetTextFont(lf);
	//m_ChatDisplay.SetBackColor(RGB(88,68,43));
	m_ChatDisplay.SetTextColor(RGB(255,255,255));
	m_ChatDisplay.SetBackgroundColor(false,RGB(37,55,73));
	m_ChatDisplay.SetReadOnly(TRUE);
	m_brush.CreateSolidBrush(RGB(255,0,255));

	m_UserList.Create(LVS_REPORT | WS_CHILD | WS_VISIBLE,rcCreate, this, IDC_USER_LIST);
	m_UserList.SetExtendedStyle(LVS_EX_FULLROWSELECT );
	m_UserList.SetBkColor(RGB(37,55,73));
	m_UserList.SetTextColor(RGB(255,255,255));
	m_UserList.SetTextBkColor(RGB(37,55,73));
	int nColumnCount=0;
	//�û���־
	m_UserList.InsertColumn(nColumnCount++,TEXT(""),LVCFMT_CENTER,10,0);
	m_UserList.InsertColumn(nColumnCount++,TEXT("�ǳ�"),LVCFMT_LEFT,80);
	m_UserList.InsertColumn(nColumnCount++,TEXT("�Ƹ�"),LVCFMT_LEFT,111);
	m_UserList.ShowWindow(SW_HIDE);

	//�ƶ���¼��ť
//	m_btScrollLeft.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_SCORE_MOVE_L);
//	m_btScrollRight.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_SCORE_MOVE_R);
//	m_btScrollLeft.SetButtonImage(IDB_BT_SCORE_MOVE_L,hResInstance,false);
//	m_btScrollRight.SetButtonImage(IDB_BT_SCORE_MOVE_R,hResInstance,false);

	//�ƶ���ׯ���밴ť
	m_btScrollUp.Create(NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_UP);
	m_btScrollDown.Create(NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_DOWN);
	m_btScrollUp.SetButtonImage(IDB_BT_BT_S,hResInstance,false,false);
	m_btScrollDown.SetButtonImage(IDB_BT_BT_X,hResInstance,false,false);

	//��ȡ��ť

	//m_btBankStorage.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_BANK_STORAGE);
	m_btBankDraw.Create(NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_BANK_DRAW);
	//m_btBankStorage.SetButtonImage(IDB_BT_STORAGE,hResInstance,false,false);
	m_btBankDraw.SetButtonImage(IDB_BT_DRAW,hResInstance,false,false);	


	m_btOpenAdmin.Create(NULL,WS_CHILD|WS_VISIBLE,CRect(4,4,11,11),this,IDC_BT_ADMIN);
	m_btOpenAdmin.ShowWindow(SW_HIDE);

	//���밴ť
	BYTE cbIndex=0;
	for(cbIndex=0;cbIndex<JETTON_COUNT;cbIndex++)
	{
		m_btJetton[cbIndex].Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,g_nJettonIDC[cbIndex]);
		m_btJetton[cbIndex].SetButtonImage(g_nJettonIDB[cbIndex],hResInstance,false,false);
	}

	
	m_btLookRecord.ShowWindow(SW_SHOW);
	m_btLookRecord.EnableWindow(true);	
	m_btCloseRecord.ShowWindow(SW_HIDE);
	m_ChatDisplay.ShowWindow(SW_HIDE);
	m_ChatDisplay.SetHideWindow(true);
	m_ChatInput.ShowWindow(SW_HIDE);
	m_btSendMessage.ShowWindow(SW_HIDE);


	//����
	m_hInst = NULL;
	m_pClientControlDlg = NULL;
	m_hInst = LoadLibrary(TEXT("SicBoClientControl.dll"));
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

	return 0;
}

//���ý���
void CGameClientView::ResetGameView()
{
	//��ע��Ϣ
	ZeroMemory(m_lUserBet,sizeof(m_lUserBet));

	//ȫ����ע
	ZeroMemory(m_lAllUserBet,sizeof(m_lAllUserBet));

	//ׯ����Ϣ
	m_wBankerUser=INVALID_CHAIR;		
	m_wBankerTime=0;
	m_lBankerScore=0L;	
	m_lBankerWinScore=0L;
	m_lTmpBankerWinScore=0;
	m_nShowValleyIndex=0;

	//���ֳɼ�
	m_lMeCurGameScore=0L;	
	m_lMeCurGameReturnScore=0L;
	m_lBankerCurGameScore=0L;
	m_lGameRevenue=0L;

	//״̬��Ϣ
	m_nJettonIndex=-1;
	m_lCurrentJetton=0L;
	m_cbAreaFlash=0xFF;
	//m_wMeChairID=INVALID_CHAIR;
	m_bShowChangeBanker=false;
	m_bNeedSetGameRecord=false;
	ZeroMemory(m_bWinFlag,sizeof(m_bWinFlag));
	m_bFlashResult=false;
	m_bShowGameResult=false;

	m_lMeCurGameScore=0L;			
	m_lMeCurGameReturnScore=0L;	
	m_lBankerCurGameScore=0L;		

	m_lAreaLimitScore=0L;	

	//λ����Ϣ
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
void CGameClientView::RectifyControl(INT nWidth, INT nHeight)
{
	//λ����Ϣ
	m_nWinFlagsExcursionX = nWidth/2-292;	
	m_nWinFlagsExcursionY = nHeight/2+238;

	CSize Size;
	Size.cy = Size.cy/2;

	//CImageHandle ImageHandle(&m_ImageViewBack);
	int LifeWidth = nWidth/2-m_ImageViewBack.GetWidth()/2;
	int TopHeight = nHeight/2-m_ImageViewBack.GetHeight()/2;

	int nAreaWidth=168;
	int nAreaHeight=146;
	int nBorderWidth=32;
	int ExcursionY=10;
	int nCenterX=nWidth/2-1;
	int nCenterY=nHeight/2;
	int nXPos=nWidth/2;
	int nYPos=nHeight/2+100;

	int cbIndex=0;
	//����λ��
	int cbAreaIndex=0;
	//����1-2
	m_rcBetArea[0].SetRect(nXPos-406,nYPos-318,nXPos-406+96,nYPos-318+117);
	m_rcBetArea[1].SetRect(nXPos+315,nYPos-318,nXPos+315+96,nYPos-318+117);

	m_rcNumArea[0].SetRect(nXPos-406,nYPos-270,nXPos-406+96,nYPos-270+20);
	m_rcNumArea[1].SetRect(nXPos+315,nYPos-270,nXPos+315+96,nYPos-270+20);

	m_rcMeNum[0].SetRect(nXPos-406,nYPos-220,nXPos-406+96,nYPos-220+20);
	m_rcMeNum[1].SetRect(nXPos+315,nYPos-220,nXPos+315+96,nYPos-220+20);
	
	//����3-5
	m_rcBetArea[2].SetRect(nXPos+197,nYPos-318,nXPos+197+102,nYPos-318+58);
	m_rcBetArea[3].SetRect(nXPos+190,nYPos-259,nXPos+190+116,nYPos-259+60);
	m_rcBetArea[4].SetRect(nXPos+195,nYPos-198,nXPos+195+119,nYPos-198+64);

	m_rcNumArea[2].SetRect(nXPos+197,nYPos-318,nXPos+197+102,nYPos-318+58);
	m_rcNumArea[3].SetRect(nXPos+190,nYPos-259,nXPos+190+116,nYPos-259+60);
	m_rcNumArea[4].SetRect(nXPos+195,nYPos-198,nXPos+195+119,nYPos-198+64);

	m_rcMeNum[2].SetRect(nXPos+197,nYPos-280,nXPos+197+102,nYPos-280+20);
	m_rcMeNum[3].SetRect(nXPos+190,nYPos-220,nXPos+190+116,nYPos-220+20);
	m_rcMeNum[4].SetRect(nXPos+195,nYPos-154,nXPos+195+119,nYPos-154+20);

	//����6-8
	m_rcBetArea[5].SetRect(nXPos-292,nYPos-318,nXPos-292+102,nYPos-318+58);
	m_rcBetArea[6].SetRect(nXPos-299,nYPos-259,nXPos-299+116,nYPos-259+60);
	m_rcBetArea[7].SetRect(nXPos-308,nYPos-198,nXPos-308+119,nYPos-198+64);

	m_rcNumArea[5].SetRect(nXPos-292,nYPos-318,nXPos-292+102,nYPos-318+58);
	m_rcNumArea[6].SetRect(nXPos-299,nYPos-259,nXPos-299+116,nYPos-259+60);
	m_rcNumArea[7].SetRect(nXPos-308,nYPos-198,nXPos-308+119,nYPos-198+64);

	m_rcMeNum[5].SetRect(nXPos-292,nYPos-280,nXPos-292+102,nYPos-280+20);
	m_rcMeNum[6].SetRect(nXPos-299,nYPos-220,nXPos-299+116,nYPos-220+20);
	m_rcMeNum[7].SetRect(nXPos-308,nYPos-154,nXPos-308+119,nYPos-154+20);

	//����9-14
	m_rcBetArea[13].SetRect(nXPos-175,nYPos-283,nXPos-175+54,nYPos-283+62);
	m_rcBetArea[12].SetRect(nXPos-117,nYPos-283,nXPos-117+57,nYPos-283+62);
	m_rcBetArea[11].SetRect(nXPos-56,nYPos-283,nXPos-56+57,nYPos-283+62);
	m_rcBetArea[10].SetRect(nXPos+3,nYPos-283,nXPos+3+57,nYPos-283+62);
	m_rcBetArea[9].SetRect(nXPos+64,nYPos-283,nXPos+64+57,nYPos-283+62);
	m_rcBetArea[8].SetRect(nXPos+126,nYPos-283,nXPos+126+57,nYPos-283+62);

	m_rcNumArea[13].SetRect(nXPos-175,nYPos-283,nXPos-175+54,nYPos-283+62);
	m_rcNumArea[12].SetRect(nXPos-117,nYPos-283,nXPos-117+57,nYPos-283+62);
	m_rcNumArea[11].SetRect(nXPos-56,nYPos-283,nXPos-56+57,nYPos-283+62);
	m_rcNumArea[10].SetRect(nXPos+3,nYPos-283,nXPos+3+57,nYPos-283+62);
	m_rcNumArea[9].SetRect(nXPos+64,nYPos-283,nXPos+64+57,nYPos-283+62);
	m_rcNumArea[8].SetRect(nXPos+126,nYPos-283,nXPos+126+57,nYPos-283+62);

	m_rcMeNum[13].SetRect(nXPos-177,nYPos-240,nXPos-177+54,nYPos-240+20);
	m_rcMeNum[12].SetRect(nXPos-117,nYPos-240,nXPos-117+57,nYPos-240+20);
	m_rcMeNum[11].SetRect(nXPos-56,nYPos-240,nXPos-56+57,nYPos-240+20);
	m_rcMeNum[10].SetRect(nXPos+5,nYPos-240,nXPos+5+57,nYPos-240+20);
	m_rcMeNum[9].SetRect(nXPos+66,nYPos-240,nXPos+66+57,nYPos-240+20);
	m_rcMeNum[8].SetRect(nXPos+128,nYPos-240,nXPos+128+57,nYPos-240+20);

	//����15
	m_rcBetArea[14].SetRect(nXPos-181,nYPos-220,nXPos-181+367,nYPos-220+86);

	m_rcNumArea[14].SetRect(nXPos-181,nYPos-220,nXPos-181+367,nYPos-220+86);

	m_rcMeNum[14].SetRect(nXPos-181,nYPos-154,nXPos-181+367,nYPos-154+20);

	//����16-29
	m_rcBetArea[28].SetRect(nXPos-446,nYPos-133,nXPos-446+50,nYPos-133+88);
	m_rcBetArea[27].SetRect(nXPos-380,nYPos-133,nXPos-380+50,nYPos-133+88);
	m_rcBetArea[26].SetRect(nXPos-318,nYPos-133,nXPos-318+55,nYPos-133+88);
	m_rcBetArea[25].SetRect(nXPos-252,nYPos-133,nXPos-252+57,nYPos-133+88);
	m_rcBetArea[24].SetRect(nXPos-187,nYPos-133,nXPos-187+55,nYPos-133+88);
	m_rcBetArea[23].SetRect(nXPos-126,nYPos-133,nXPos-126+60,nYPos-133+88);
	m_rcBetArea[22].SetRect(nXPos-61,nYPos-133,nXPos-61+62,nYPos-133+88);
	m_rcBetArea[21].SetRect(nXPos+2,nYPos-133,nXPos+2+62,nYPos-133+88);
	m_rcBetArea[20].SetRect(nXPos+68,nYPos-133,nXPos+68+62,nYPos-133+88);
	m_rcBetArea[19].SetRect(nXPos+135,nYPos-133,nXPos+135+58,nYPos-133+88);
	m_rcBetArea[18].SetRect(nXPos+200,nYPos-133,nXPos+200+57,nYPos-133+88);
	m_rcBetArea[17].SetRect(nXPos+268,nYPos-133,nXPos+268+53,nYPos-133+88);
	m_rcBetArea[16].SetRect(nXPos+333,nYPos-133,nXPos+333+52,nYPos-133+88);
	m_rcBetArea[15].SetRect(nXPos+400,nYPos-133,nXPos+400+50,nYPos-133+88);

	m_rcNumArea[28].SetRect(nXPos-451,nYPos-133,nXPos-451+60,nYPos-133+88);
	m_rcNumArea[27].SetRect(nXPos-385,nYPos-133,nXPos-385+60,nYPos-133+88);
	m_rcNumArea[26].SetRect(nXPos-323,nYPos-133,nXPos-323+65,nYPos-133+88);
	m_rcNumArea[25].SetRect(nXPos-252,nYPos-133,nXPos-252+57,nYPos-133+88);
	m_rcNumArea[24].SetRect(nXPos-187,nYPos-133,nXPos-187+55,nYPos-133+88);
	m_rcNumArea[23].SetRect(nXPos-126,nYPos-133,nXPos-126+60,nYPos-133+88);
	m_rcNumArea[22].SetRect(nXPos-61,nYPos-133,nXPos-61+62,nYPos-133+88);
	m_rcNumArea[21].SetRect(nXPos+2,nYPos-133,nXPos+2+62,nYPos-133+88);
	m_rcNumArea[20].SetRect(nXPos+68,nYPos-133,nXPos+68+62,nYPos-133+88);
	m_rcNumArea[19].SetRect(nXPos+135,nYPos-133,nXPos+135+58,nYPos-133+88);
	m_rcNumArea[18].SetRect(nXPos+200,nYPos-133,nXPos+200+57,nYPos-133+88);
	m_rcNumArea[17].SetRect(nXPos+263,nYPos-133,nXPos+263+63,nYPos-133+88);
	m_rcNumArea[16].SetRect(nXPos+328,nYPos-133,nXPos+328+62,nYPos-133+88);
	m_rcNumArea[15].SetRect(nXPos+395,nYPos-133,nXPos+395+60,nYPos-133+88);

	m_rcMeNum[28].SetRect(nXPos-457,nYPos-65,nXPos-457+60,nYPos-65+20);
	m_rcMeNum[27].SetRect(nXPos-391,nYPos-65,nXPos-391+60,nYPos-65+20);
	m_rcMeNum[26].SetRect(nXPos-329,nYPos-65,nXPos-329+65,nYPos-65+20);
	m_rcMeNum[25].SetRect(nXPos-258,nYPos-65,nXPos-258+57,nYPos-65+20);
	m_rcMeNum[24].SetRect(nXPos-187,nYPos-65,nXPos-187+55,nYPos-65+20);
	m_rcMeNum[23].SetRect(nXPos-126,nYPos-65,nXPos-126+60,nYPos-65+20);
	m_rcMeNum[22].SetRect(nXPos-61,nYPos-65,nXPos-61+62,nYPos-65+20);
	m_rcMeNum[21].SetRect(nXPos+2,nYPos-65,nXPos+2+62,nYPos-65+20);
	m_rcMeNum[20].SetRect(nXPos+68,nYPos-65,nXPos+68+62,nYPos-65+20);
	m_rcMeNum[19].SetRect(nXPos+135,nYPos-65,nXPos+135+58,nYPos-65+20);
	m_rcMeNum[18].SetRect(nXPos+206,nYPos-65,nXPos+206+57,nYPos-65+20);
	m_rcMeNum[17].SetRect(nXPos+269,nYPos-65,nXPos+269+63,nYPos-65+20);
	m_rcMeNum[16].SetRect(nXPos+334,nYPos-65,nXPos+334+62,nYPos-65+20);
	m_rcMeNum[15].SetRect(nXPos+401,nYPos-65,nXPos+401+60,nYPos-65+20);

	//����30-44
	m_rcBetArea[43].SetRect(nXPos-464,nYPos-43,nXPos-464+46,nYPos-43+95);
	m_rcBetArea[42].SetRect(nXPos-400,nYPos-43,nXPos-400+47,nYPos-43+95);
	m_rcBetArea[41].SetRect(nXPos-336,nYPos-43,nXPos-336+47,nYPos-43+95);
	m_rcBetArea[40].SetRect(nXPos-274,nYPos-43,nXPos-274+50,nYPos-43+95);
	m_rcBetArea[39].SetRect(nXPos-214,nYPos-43,nXPos-214+54,nYPos-43+95);
	m_rcBetArea[38].SetRect(nXPos-152,nYPos-43,nXPos-152+56,nYPos-43+95);
	m_rcBetArea[37].SetRect(nXPos-91,nYPos-43,nXPos-91+60,nYPos-43+95);
	m_rcBetArea[36].SetRect(nXPos-27,nYPos-43,nXPos-27+60,nYPos-43+95);
	m_rcBetArea[35].SetRect(nXPos+35,nYPos-43,nXPos+35+60,nYPos-43+95);
	m_rcBetArea[34].SetRect(nXPos+99,nYPos-43,nXPos+99+58,nYPos-43+95);
	m_rcBetArea[33].SetRect(nXPos+164,nYPos-43,nXPos+164+55,nYPos-43+95);
	m_rcBetArea[32].SetRect(nXPos+230,nYPos-43,nXPos+230+51,nYPos-43+95);
	m_rcBetArea[31].SetRect(nXPos+294,nYPos-43,nXPos+294+49,nYPos-43+95);
	m_rcBetArea[30].SetRect(nXPos+356,nYPos-43,nXPos+356+48,nYPos-43+95);
	m_rcBetArea[29].SetRect(nXPos+422,nYPos-43,nXPos+422+43,nYPos-43+95);

	m_rcNumArea[43].SetRect(nXPos-474,nYPos-43,nXPos-474+66,nYPos-43+95);
	m_rcNumArea[42].SetRect(nXPos-405,nYPos-43,nXPos-405+57,nYPos-43+95);
	m_rcNumArea[41].SetRect(nXPos-341,nYPos-43,nXPos-341+57,nYPos-43+95);
	m_rcNumArea[40].SetRect(nXPos-279,nYPos-43,nXPos-279+60,nYPos-43+95);
	m_rcNumArea[39].SetRect(nXPos-214,nYPos-43,nXPos-214+54,nYPos-43+95);
	m_rcNumArea[38].SetRect(nXPos-152,nYPos-43,nXPos-152+56,nYPos-43+95);
	m_rcNumArea[37].SetRect(nXPos-91,nYPos-43,nXPos-91+60,nYPos-43+95);
	m_rcNumArea[36].SetRect(nXPos-27,nYPos-43,nXPos-27+60,nYPos-43+95);
	m_rcNumArea[35].SetRect(nXPos+35,nYPos-43,nXPos+35+60,nYPos-43+95);
	m_rcNumArea[34].SetRect(nXPos+99,nYPos-43,nXPos+99+58,nYPos-43+95);
	m_rcNumArea[33].SetRect(nXPos+164,nYPos-43,nXPos+164+55,nYPos-43+95);
	m_rcNumArea[32].SetRect(nXPos+225,nYPos-43,nXPos+225+61,nYPos-43+95);
	m_rcNumArea[31].SetRect(nXPos+289,nYPos-43,nXPos+289+59,nYPos-43+95);
	m_rcNumArea[30].SetRect(nXPos+351,nYPos-43,nXPos+351+58,nYPos-43+95);
	m_rcNumArea[29].SetRect(nXPos+412,nYPos-43,nXPos+412+63,nYPos-43+95);

	m_rcMeNum[43].SetRect(nXPos-481,nYPos+32,nXPos-481+66,nYPos+32+20);
	m_rcMeNum[42].SetRect(nXPos-412,nYPos+32,nXPos-412+57,nYPos+32+20);
	m_rcMeNum[41].SetRect(nXPos-346,nYPos+32,nXPos-346+57,nYPos+32+20);
	m_rcMeNum[40].SetRect(nXPos-283,nYPos+32,nXPos-283+60,nYPos+32+20);
	m_rcMeNum[39].SetRect(nXPos-217,nYPos+32,nXPos-217+54,nYPos+32+20);
	m_rcMeNum[38].SetRect(nXPos-152,nYPos+32,nXPos-152+56,nYPos+32+20);
	m_rcMeNum[37].SetRect(nXPos-91,nYPos+32,nXPos-91+60,nYPos+32+20);
	m_rcMeNum[36].SetRect(nXPos-27,nYPos+32,nXPos-27+60,nYPos+32+20);
	m_rcMeNum[35].SetRect(nXPos+35,nYPos+32,nXPos+35+60,nYPos+32+20);
	m_rcMeNum[34].SetRect(nXPos+99,nYPos+32,nXPos+99+58,nYPos+32+20);
	m_rcMeNum[33].SetRect(nXPos+166,nYPos+32,nXPos+166+55,nYPos+32+20);
	m_rcMeNum[32].SetRect(nXPos+228,nYPos+32,nXPos+228+61,nYPos+32+20);
	m_rcMeNum[31].SetRect(nXPos+294,nYPos+32,nXPos+294+59,nYPos+32+20);
	m_rcMeNum[30].SetRect(nXPos+358,nYPos+32,nXPos+358+58,nYPos+32+20);
	m_rcMeNum[29].SetRect(nXPos+419,nYPos+32,nXPos+419+63,nYPos+32+20);

	//����45-50
	m_rcBetArea[49].SetRect(nXPos-482,nYPos+53,nXPos-482+148,nYPos+53+81);
	m_rcBetArea[48].SetRect(nXPos-321,nYPos+53,nXPos-321+151,nYPos+53+81);
	m_rcBetArea[47].SetRect(nXPos-160,nYPos+53,nXPos-160+156,nYPos+53+81);
	m_rcBetArea[46].SetRect(nXPos-1,nYPos+53,nXPos-1+157,nYPos+53+81);
	m_rcBetArea[45].SetRect(nXPos+162,nYPos+53,nXPos+162+154,nYPos+53+81);
	m_rcBetArea[44].SetRect(nXPos+327,nYPos+53,nXPos+327+154,nYPos+53+81);

	m_rcNumArea[49].SetRect(nXPos-482,nYPos+53,nXPos-482+148,nYPos+53+81);
	m_rcNumArea[48].SetRect(nXPos-321,nYPos+53,nXPos-321+151,nYPos+53+81);
	m_rcNumArea[47].SetRect(nXPos-160,nYPos+53,nXPos-160+156,nYPos+53+81);
	m_rcNumArea[46].SetRect(nXPos-1,nYPos+53,nXPos-1+157,nYPos+53+81);
	m_rcNumArea[45].SetRect(nXPos+162,nYPos+53,nXPos+162+154,nYPos+53+81);
	m_rcNumArea[44].SetRect(nXPos+327,nYPos+53,nXPos+327+154,nYPos+53+81);

	m_rcMeNum[49].SetRect(nXPos-482,nYPos+114,nXPos-482+148,nYPos+114+20);
	m_rcMeNum[48].SetRect(nXPos-321,nYPos+114,nXPos-321+151,nYPos+114+20);
	m_rcMeNum[47].SetRect(nXPos-160,nYPos+114,nXPos-160+156,nYPos+114+20);
	m_rcMeNum[46].SetRect(nXPos-1,nYPos+114,nXPos-1+157,nYPos+114+20);
	m_rcMeNum[45].SetRect(nXPos+162,nYPos+114,nXPos+162+154,nYPos+114+20);
	m_rcMeNum[44].SetRect(nXPos+327,nYPos+114,nXPos+327+154,nYPos+114+20);

	//����51-52
	m_rcBetArea[50].SetRect(nXPos-434,nYPos-198,nXPos-434+115,nYPos-198+64);
	m_rcBetArea[51].SetRect(nXPos+322,nYPos-198,nXPos+322+115,nYPos-198+64);

	m_rcNumArea[50].SetRect(nXPos-434,nYPos-198,nXPos-434+115,nYPos-198+64);
	m_rcNumArea[51].SetRect(nXPos+322,nYPos-198,nXPos+322+115,nYPos-198+64);

	m_rcMeNum[50].SetRect(nXPos-434,nYPos-154,nXPos-434+115,nYPos-154+20);
	m_rcMeNum[51].SetRect(nXPos+322,nYPos-154,nXPos+322+115,nYPos-154+20);
	////����3-5
	//for(cbIndex=0;cbIndex<3;cbIndex++)
	//	m_rcBetArea[cbAreaIndex++].SetRect(nXPos+144,nYPos-253+cbIndex*54,nXPos+240,nYPos-253+(cbIndex+1)*54);
	////����6-8
	//for(cbIndex=0;cbIndex<3;cbIndex++)
	//	m_rcBetArea[cbAreaIndex++].SetRect(nXPos-239,nYPos-253+cbIndex*55,nXPos-144,nYPos-253+(cbIndex+1)*55);
	////����9-14
	//for(cbIndex=5;cbIndex>=0;cbIndex--)
	//	m_rcBetArea[cbAreaIndex++].SetRect(nXPos-144+cbIndex*48,nYPos-253,nXPos-144+(cbIndex+1)*48,nYPos-181);
	////����15
	//m_rcBetArea[cbAreaIndex++].SetRect(nXPos-144,nYPos-185,nXPos+144,nYPos-88);

	////����16-29
	//for(cbIndex=13;cbIndex>=0;cbIndex--)
	//	m_rcBetArea[cbAreaIndex++].SetRect(nXPos-336+cbIndex*48,nYPos-91,nXPos-336+(1+cbIndex)*48,nYPos-1);

	////����30-44
	//for(cbIndex=14;cbIndex>=0;cbIndex--)
	//	m_rcBetArea[cbAreaIndex++].SetRect(nXPos-328+cbIndex*44,nYPos,nXPos-328+(1+cbIndex)*44,nYPos+90);

	////����45-50
	//for(cbIndex=5;cbIndex>=0;cbIndex--)
	//	m_rcBetArea[cbAreaIndex++].SetRect(nXPos-336+cbIndex*112,nYPos+88,nXPos-336+(1+cbIndex)*112,nYPos+154);

	////��˫
	//m_rcBetArea[cbAreaIndex++].SetRect(nXPos+240,nYPos-255,nXPos+336,nYPos-89);
	//m_rcBetArea[cbAreaIndex++].SetRect(nXPos-336,nYPos-255,nXPos-237,nYPos-89);
	
	//ˮƽ�������Ų�
	for(cbIndex=0;cbIndex<AREA_COUNT;cbIndex++)
	{
		//����λ��
		m_PointJetton[cbIndex].SetPoint(m_rcBetArea[cbIndex].left,m_rcBetArea[cbIndex].top);
		//��ע���
		m_PointJettonNumber[cbIndex].SetPoint((m_rcNumArea[cbIndex].right+m_rcNumArea[cbIndex].left)/2-33, (m_rcNumArea[cbIndex].bottom+m_rcNumArea[cbIndex].top)/2-10);
		m_MeJettonNumber[cbIndex].SetPoint((m_rcMeNum[cbIndex].right+m_rcMeNum[cbIndex].left)/2-29, (m_rcMeNum[cbIndex].bottom+m_rcMeNum[cbIndex].top)/2-10);
		//�ؼ�λ��
		//nXPos+=nAreaWidth+nBorderWidth;
	}

	//�û��б�λ��
	m_RectUserList.SetRect(nWidth-m_ImageUserBack.GetWidth()+6,nHeight-m_ImageUserBack.GetHeight()-17,nWidth-m_ImageUserBack.GetWidth()+6+217,nHeight-m_ImageUserBack.GetHeight()-17+162);
	m_RectChartDisplay.SetRect(nWidth-m_ImageUserBack.GetWidth()+6,nHeight-m_ImageUserBack.GetHeight()-17,nWidth-m_ImageUserBack.GetWidth()+6+217,nHeight-m_ImageUserBack.GetHeight()-17+126);
	
	//�ƶ��ؼ�
	HDWP hDwp=BeginDeferWindowPos(33);
	const UINT uFlags=SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOCOPYBITS;

	//m_ApplyUser.m_viewHandle = m_hWnd;
	
	//�б�ؼ�
	//DeferWindowPos(hDwp,m_ApplyUser,NULL,nWidth/2+180,nHeight/2-277,154,51,uFlags);
	DeferWindowPos(hDwp,m_btScrollUp,NULL,nWidth/2+479,nHeight/2-298,18,18,uFlags);
	DeferWindowPos(hDwp,m_btScrollDown,NULL,nWidth/2+479,nHeight/2-249,18,18,uFlags);
	
	
	m_ptPlayerInfo.SetPoint(63,nHeight-83);

	DeferWindowPos(hDwp,m_UserList,NULL,m_RectUserList.left,m_RectUserList.top,m_RectUserList.Width(),m_RectUserList.Height(),uFlags);
	DeferWindowPos(hDwp,m_ChatDisplay,NULL,m_RectChartDisplay.left,m_RectChartDisplay.top,m_RectChartDisplay.Width()-16,m_RectChartDisplay.Height(),uFlags);
	DeferWindowPos(hDwp, m_ChatInput, NULL, nWidth-m_ImageUserBack.GetWidth()+6, nHeight-m_ImageUserBack.GetHeight()-57+179, 182, 20, SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOCOPYBITS);

	//���밴ť
	CRect rcJetton;
	m_btJetton[0].GetWindowRect(&rcJetton);
	nYPos = nHeight/2+273;
	nXPos = nWidth/2-285;
	

	//����λ��
	int  nSpace=0;
    BYTE cbCol=0;
	int nJettonCount=0;
	for(cbIndex=0;cbIndex<JETTON_COUNT;cbIndex++)
			DeferWindowPos(hDwp,m_btJetton[cbIndex],NULL,nXPos+(nSpace+rcJetton.Width())*cbIndex,nYPos,0,0,uFlags|SWP_NOSIZE);
	
	//��ׯ��ť
	DeferWindowPos(hDwp,m_btApplyBanker,NULL,nWidth/2+390,27,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btCancelBanker,NULL,nWidth/2+390,27,0,0,uFlags|SWP_NOSIZE);

	//������ť
	DeferWindowPos(hDwp,m_btUserList,NULL,nWidth-125,nHeight-45,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btChat,NULL,nWidth/2+280,nHeight-45,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp, m_btSendMessage, NULL,nWidth-m_ImageUserBack.GetWidth()+189 , nHeight-m_ImageUserBack.GetHeight()-57+175, 0, 0, uFlags|SWP_NOSIZE);
	//DeferWindowPos(hDwp,m_btBankStorage,NULL,nWidth/2+280,nHeight/2-217,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btBankDraw,NULL,nWidth/2+180,nHeight-45,0,0,uFlags|SWP_NOSIZE);


	DeferWindowPos(hDwp,m_btLookRecord,NULL,nWidth/2+250,5,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btCloseRecord,NULL,(nWidth+m_PngRecord.GetWidth())/2-40,(nHeight-m_PngRecord.GetHeight())/2+20,0,0,uFlags|SWP_NOSIZE);
	//�ƶ���¼
	EndDeferWindowPos(hDwp);

	//��������
	CRect rcTemp;
	m_CartoonDice.GetWindowRect(&rcTemp);
	m_rcAnimatWnd.SetRect((nWidth-rcTemp.Width())/2-10,(nHeight-rcTemp.Height())/2-270,(nWidth+rcTemp.Width())/2-10,(nHeight+rcTemp.Height())/2-270);

	m_CartoonDice.SetWindowPos(NULL,m_rcAnimatWnd.left,m_rcAnimatWnd.top,m_rcAnimatWnd.Width(),m_rcAnimatWnd.Height(),SWP_NOSIZE);
	m_CartoonDice.SetPosInParent(m_rcAnimatWnd);

	m_ptDicePos[0].SetPoint(m_rcAnimatWnd.left+53,m_rcAnimatWnd.top+50);
	m_ptDicePos[1].SetPoint(m_rcAnimatWnd.left+33,m_rcAnimatWnd.top+80);
	m_ptDicePos[2].SetPoint(m_rcAnimatWnd.left+73,m_rcAnimatWnd.top+80);

	m_ptRtDicePos[0].SetPoint(nWidth/2-252,30);
	m_ptRtDicePos[1].SetPoint(nWidth/2-207,30);
	m_ptRtDicePos[2].SetPoint(nWidth/2-160,30);

	m_ptRecordPosEx.SetPoint(nWidth/2+100,29);
	m_rcDiceValue.SetRect(nWidth/2+70,m_rcAnimatWnd.top+150,nWidth/2+180,m_rcAnimatWnd.top+164);
	m_ptRecordBkPos.SetPoint((nWidth-m_PngRecord.GetWidth())/2,(nHeight-m_PngRecord.GetHeight())/2+18);
	m_ptRecordPos.SetPoint((nWidth-m_PngRecord.GetWidth())/2+57,(nHeight-m_PngRecord.GetHeight())/2+53);
	
	m_CartoonDice.ShowWindow(SW_HIDE);

	m_UserList.ReSetLoc(nWidth,nHeight);
	m_ChatDisplay.RectifyControl(nWidth,nHeight);

	//���ƿͻ���
	if ( m_pClientControlDlg )
	{
		CRect rcClient;
		GetWindowRect(rcClient);
		CRect rcControl;
		m_pClientControlDlg->GetWindowRect(rcControl);
		m_pClientControlDlg->MoveWindow(rcClient.left+(rcClient.Width()-rcControl.Width())/2,
			rcClient.top+(rcClient.Height()-rcControl.Height())/2, rcControl.Width(), rcControl.Height());
	}

	return;
}

//�滭ʱ��
VOID CGameClientView::DrawUserTimer(CDC * pDC, int nXPos, int nYPos, WORD wTime)
{

	m_PngTimerBackGround.DrawImage(pDC,nXPos,nYPos);

	//��ȡ����
	const INT nNumberHeight=m_PngTimeNumber.GetHeight();
	const INT nNumberWidth=m_PngTimeNumber.GetWidth()/10;

	//������Ŀ
	LONG lNumberCount=0;
	WORD wNumberTemp=wTime;
	do
	{
		lNumberCount++;
		wNumberTemp/=10;
	} while (wNumberTemp>0L);

	nXPos+=m_PngTimerBackGround.GetWidth()/2;
	nYPos+=m_PngTimerBackGround.GetHeight()/2;
	//λ�ö���
	INT nYDrawPos=nYPos-nNumberHeight/2-3;
	INT nXDrawPos=nXPos+(lNumberCount*nNumberWidth)/2-nNumberWidth;

	//�滭����
	for (LONG i=0;i<lNumberCount;i++)
	{
		//�滭����
		WORD wCellNumber=wTime%10;
		m_PngTimeNumber.DrawImage(pDC,nXDrawPos,nYDrawPos,nNumberWidth,nNumberHeight,wCellNumber*nNumberWidth,0);

		//���ñ���
		wTime/=10;
		nXDrawPos-=nNumberWidth;
	};
	return;
}


//�滭����
void CGameClientView::DrawGameView(CDC * pDC, INT nWidth, INT nHeight)
{
	CDC* pDCBuff = pDC;

	/*for ( int iW = 0 ; iW < nWidth; iW += m_ImageViewFill.GetWidth() )
	{
		for ( int iH = 0;  iH < nHeight; iH += m_ImageViewFill.GetHeight() )
		{
			m_ImageViewFill.BitBlt(pDC->GetSafeHdc(), iW, iH);
		}
	}*/
	m_ImageViewBack.BitBlt( pDC->GetSafeHdc(), nWidth/2 - m_ImageViewBack.GetWidth()/2, nHeight/2 - m_ImageViewBack.GetHeight()/2 );	

	//�滭����
	//DrawViewImage(pDC,m_ImageViewFill,DRAW_MODE_SPREAD);
	//DrawViewImage(pDC,m_ImageViewBack,DRAW_MODE_CENTENT);
	//ʱ����ʾ
	WORD wUserTimer=0;
	if (m_wMeChairID!=INVALID_CHAIR)
	{
		wUserTimer=GetUserClock(m_wMeChairID);
	}
	{
		DrawUserTimer(pDCBuff,nWidth/2+75,107,wUserTimer);
		int nTipWidth = m_PngFlagsTip.GetWidth()/3;
		int nTipHeight = m_PngFlagsTip.GetHeight();

		switch(m_cbGameStatus)
		{
			case GAME_STATUS_FREE:m_PngFlagsTip.DrawImage(pDCBuff,nWidth/2+140, 107, nTipWidth, nTipHeight, 0,0);break;
			case GS_PLAYER_BET:m_PngFlagsTip.DrawImage(pDCBuff,nWidth/2+140, 107, nTipWidth, nTipHeight, nTipWidth,0);break;
			case GS_GAME_END:m_PngFlagsTip.DrawImage(pDCBuff,nWidth/2+140, 107, nTipWidth, nTipHeight, nTipWidth*2,0);break;
			default: break;
		}
	}
	
	//ʤ���߿�
	FlashAreaFrame(nWidth,nHeight,pDCBuff);

	//������Դ
	CSize static SizeJettonItem(m_ImageJettonView.GetWidth()/JETTON_COUNT,m_ImageJettonView.GetHeight());

	//�滭����
	for (INT i=0;i<AREA_COUNT;i++)
	{
		//��������
		LONG lScoreCount=0L;
		int static nJettonViewIndex=0;

		//�滭����
		for (INT_PTR j=0;j<m_JettonInfoArray[i].GetCount();j++)
		{
			//��ȡ��Ϣ
			tagJettonInfo * pJettonInfo=&m_JettonInfoArray[i][j];
			//�ۼ�����
			ASSERT(pJettonInfo->cbJettonIndex<JETTON_COUNT);
			lScoreCount+=g_lScoreType[pJettonInfo->cbJettonIndex];
			nJettonViewIndex=pJettonInfo->cbJettonIndex;
			//�滭����
			m_ImageJettonView.DrawImage(pDCBuff,pJettonInfo->nXPos+m_PointJetton[i].x,
				pJettonInfo->nYPos+m_PointJetton[i].y,SizeJettonItem.cx,SizeJettonItem.cy,
				nJettonViewIndex*SizeJettonItem.cx,0);
		}		
		CString szPrint;
		MakeString(szPrint,lScoreCount);
		//�滭����
		if (lScoreCount>0L)
		{
			m_ImageMeJetton.DrawImage(pDC,m_MeJettonNumber[i].x,m_MeJettonNumber[i].y);
			DrawTextString(pDCBuff,szPrint,RGB(255,255,255),RGB(0,0,0),&m_rcMeNum[i],DT_CENTER);
		}
			//DrawNumber(pDCBuff,&m_PngScoreNumber,TEXT(",0123456789"),AddComma(lScoreCount),m_PointJettonNumber[i].x,m_PointJettonNumber[i].y,DT_CENTER);
	}
	//�ҵ���ע
	DrawMeJettonNumber(pDCBuff);
	//�滭ׯ��
	DrawBankerInfo(pDCBuff,nWidth,nHeight);
	//�滭�û�
	DrawMeInfo(pDCBuff,nWidth,nHeight);
	// �滭��ׯ�б�
	DrawBankerList(pDCBuff,nWidth,nHeight);

	//�л�ׯ��
	if ( m_bShowChangeBanker )
	{
		int	nXPos = nWidth / 2 - 60;
		int	nYPos = nHeight / 2 - 160;

		//������ׯ
		if ( m_wMeChairID == m_wBankerUser )
		{
			//CImageHandle ImageHandleBanker(&m_ImageMeBanker);
			m_ImageMeBanker.DrawImage(pDC, nXPos, nYPos);
		}
		else if ( m_wBankerUser != INVALID_CHAIR )
		{
			//CImageHandle ImageHandleBanker(&m_ImageChangeBanker);
			m_ImageChangeBanker.DrawImage(pDC, nXPos, nYPos);
		}
		else
		{
			//CImageHandle ImageHandleBanker(&m_ImageNoBanker);
			m_ImageNoBanker.DrawImage(pDC, nXPos, nYPos);
		}
	}	

	switch(m_cbGameStatus)
	{
		case GAME_STATUS_FREE:
		case GS_PLAYER_BET:
			{
				m_PngAnimatDice.DrawImage(pDCBuff,m_rcAnimatWnd.left,m_rcAnimatWnd.top,m_PngAnimatDice.GetWidth()/8,m_PngAnimatDice.GetHeight(),0,0);
				break;
			}
		case GS_GAME_END:
			{
				if(m_bAnimatFinish)
				{
					//CString strPrint;
					if(!m_bDrawResult)
						m_PngAnimatDice.DrawImage(pDCBuff,m_rcAnimatWnd.left,m_rcAnimatWnd.top,m_PngAnimatDice.GetWidth()/8,m_PngAnimatDice.GetHeight(),0*m_PngAnimatDice.GetWidth()/8,0);
					for(BYTE cbIndex=0;cbIndex<DICE_COUNT;cbIndex++)
					{	
						if(cbIndex<m_cbPrintCount)
						{
							m_PngDice2D.DrawImage(pDCBuff,m_ptRtDicePos[cbIndex].x,m_ptRtDicePos[cbIndex].y,m_PngDice2D.GetWidth()/6,m_PngDice2D.GetHeight(),(m_cbDiceValue[cbIndex]-1)*m_PngDice2D.GetWidth()/6,0);
							//strPrint.AppendFormat(cbIndex==0?_T("%d"):_T("       %d"),m_cbDiceValue[cbIndex]);
						}
					}
					//DrawTextString(pDCBuff,strPrint,RGB(255,0,0),RGB(0,0,0),&m_rcDiceValue);
				}
				
				if(m_bDrawResult)
				{
					m_PngAnimatDice.DrawImage(pDCBuff,m_rcAnimatWnd.left,m_rcAnimatWnd.top,m_PngAnimatDice.GetWidth()/8,m_PngAnimatDice.GetHeight(),7*m_PngAnimatDice.GetWidth()/8,0);
					for(BYTE cbIndex=0;cbIndex<DICE_COUNT;cbIndex++)
					{	
						m_PngDice3D.DrawImage(pDCBuff,m_ptDicePos[cbIndex].x,m_ptDicePos[cbIndex].y,m_PngDice3D.GetWidth()/6,m_PngDice3D.GetHeight(),(6-m_cbDiceValue[cbIndex])*m_PngDice3D.GetWidth()/6,0);
					}
					ShowGameResult(pDCBuff, nWidth, nHeight);
				}
				break;
			}
		default:break;
	}
	//pDC->FillSolidRect(m_rcDiceValue.left,m_rcDiceValue.top,m_rcDiceValue.Width(),m_rcDiceValue.Height(),RGB(255,0,0));
	DrawGameRecord(pDCBuff);
	//m_CartoonDice.CopyBackGroud(pDCBuff);
	//m_CartoonDice.Invalidate();
	if (m_bShowList)
	{		
		m_ImageUserBack.DrawImage(pDC,nWidth-m_ImageUserBack.GetWidth()-4,nHeight-m_ImageUserBack.GetHeight()-57);		
	}
	if (m_bShowChat)
	{
		m_ImageChatBack.DrawImage(pDC,nWidth-m_ImageChatBack.GetWidth()-4,nHeight-m_ImageChatBack.GetHeight()-57);
	}
	//��������
	CDC dcBuffer;
	CBitmap bmpBuffer;
	m_brush.DeleteObject();
	dcBuffer.CreateCompatibleDC(pDC);
	bmpBuffer.CreateCompatibleBitmap(pDC, 182, 20);
	CBitmap *pBmpBuffer = dcBuffer.SelectObject(&bmpBuffer);	
	dcBuffer.BitBlt(0, 0, 182, 20, pDC, nWidth-m_ImageUserBack.GetWidth()+6, nHeight-m_ImageUserBack.GetHeight()-57+179, SRCCOPY);
	dcBuffer.SelectObject(pBmpBuffer);
	m_brush.CreatePatternBrush(&bmpBuffer);
	m_ChatDisplay.Invalidate(TRUE);
	
	return;
}

void CGameClientView::DrawGameRecord(CDC*pDC)
{	
	int nIndex=0;
	if(m_nRecordCount<=4)
		nIndex=m_nRecordFirst;
	else
		nIndex=(m_nRecordLast-3+MAX_SCORE_HISTORY)%MAX_SCORE_HISTORY;	

	CString strPirnt;//=_T("����   �ܵ���  ��С");
	CRect	rcRecord(m_ptRecordPosEx.x,m_ptRecordPosEx.y-15,m_ptRecordPosEx.x+150,m_ptRecordPosEx.y-3);
	//DrawTextString(pDC,strPirnt,RGB(255,255,255),RGB(0,0,0),&rcRecord);

	for(int nTemp=0;nTemp<min(4,m_nRecordCount);nTemp++)
	{
		rcRecord.top=m_ptRecordPosEx.y+nTemp*19;
		rcRecord.bottom=m_ptRecordPosEx.y+(nTemp+1)*19;

		BYTE cbTemp=m_GameRecordArrary[nIndex].cbDiceValue[0]+m_GameRecordArrary[nIndex].cbDiceValue[1]+m_GameRecordArrary[nIndex].cbDiceValue[2];
		strPirnt.Format(_T("%d %d %d     %s%d       %s"),m_GameRecordArrary[nIndex].cbDiceValue[0],m_GameRecordArrary[nIndex].cbDiceValue[1],m_GameRecordArrary[nIndex].cbDiceValue[2],\
		   cbTemp<10?_T("0"):_T(""),cbTemp,cbTemp>10?_T("��"):_T("С"));
		DrawTextString(pDC,strPirnt,RGB(255,255,255),RGB(0,0,0),&rcRecord);
		nIndex++;
	}	

	if(m_bShowRecord)
	{
		int nIndex =m_nRecordFirst;
		int nDrawCount=0;
		CRect rcRecord=0;
		CString strPrint;
		m_PngRecord.DrawImage(pDC,m_ptRecordBkPos.x,m_ptRecordBkPos.y);
		BYTE cbDiceValue=0;

		while ((nIndex+1)%MAX_SCORE_HISTORY!=m_nRecordFirst&&nDrawCount<m_nRecordCount)
		{
			//ʤ����ʶ
			tagGameRecord &GameRecord = m_GameRecordArrary[nIndex];
			rcRecord.SetRect(m_ptRecordPos.x+nDrawCount%9*39,m_ptRecordPos.y+nDrawCount/9*62,m_ptRecordPos.x+(nDrawCount%9+1)*39,m_ptRecordPos.y+(nDrawCount/9)*62+20);
			strPrint.Format(_T("%d,%d,%d"),GameRecord.cbDiceValue[0],GameRecord.cbDiceValue[1],GameRecord.cbDiceValue[2]);
			DrawTextString(pDC,strPrint,((nDrawCount%9)%2==0)?RGB(194,215,255):RGB(255,222,66),RGB(0,0,0),&rcRecord,DT_CENTER);
			
			rcRecord.SetRect(m_ptRecordPos.x+nDrawCount%9*39,m_ptRecordPos.y+nDrawCount/9*62+20,m_ptRecordPos.x+(nDrawCount%9+1)*39,m_ptRecordPos.y+(nDrawCount/9)*62+40);
			cbDiceValue=GameRecord.cbDiceValue[0]+GameRecord.cbDiceValue[1]+GameRecord.cbDiceValue[2];
			strPrint.Format(TEXT("%d"),cbDiceValue);
			DrawTextString(pDC,strPrint,((nDrawCount%9)%2==0)?RGB(194,215,255):RGB(255,222,66),RGB(0,0,0),&rcRecord,DT_CENTER);
			
			rcRecord.SetRect(m_ptRecordPos.x+nDrawCount%9*39,m_ptRecordPos.y+nDrawCount/9*62+40,m_ptRecordPos.x+(nDrawCount%9+1)*39,m_ptRecordPos.y+(nDrawCount/9)*62+60);
			DrawTextString(pDC,(cbDiceValue>10)?_T("��"):_T("С"),((nDrawCount%9)%2==0)?RGB(194,215,255):RGB(255,222,66),RGB(0,0,0),&rcRecord,DT_CENTER);
			nIndex=(nIndex+1)%MAX_SCORE_HISTORY;
			nDrawCount++;
		}
	}
}

//������Ϣ
void CGameClientView::SetMeMaxScore(SCORE lMeMaxScore)
{
	if (m_lMeMaxScore!=lMeMaxScore)
	{
		//���ñ���
		m_lMeMaxScore=lMeMaxScore;
	}
	return;
}

//�����ע
void CGameClientView::SetAreaLimitScore(SCORE lAreaLimitScore)
{
	if ( m_lAreaLimitScore!= lAreaLimitScore )
	{
		//���ñ���
		m_lAreaLimitScore=lAreaLimitScore;
	}
}

//���ó���
void CGameClientView::SetCurrentJetton(LONG lCurrentJetton,int nJettonIndex)
{
	//���ñ���
	ASSERT(lCurrentJetton>=0L);
	m_lCurrentJetton=lCurrentJetton;
	m_nJettonIndex=nJettonIndex;
	return;
}

//��ʷ��¼
void CGameClientView::SetGameHistory(BYTE *cbDiceValue)
{
	//��������
	ASSERT(cbDiceValue!=NULL);
	if(cbDiceValue==NULL) return;


	m_nRecordLast=(m_nRecordLast+1)%MAX_SCORE_HISTORY;

	tagGameRecord &GameRecord = m_GameRecordArrary[m_nRecordLast];
	memcpy(GameRecord.cbDiceValue,cbDiceValue,sizeof(GameRecord.cbDiceValue));
	
	//�ƶ��±�
	if ((m_nRecordLast+1)%MAX_SCORE_HISTORY==m_nRecordFirst)
	{
		ZeroMemory(&m_GameRecordArrary[m_nRecordFirst],sizeof(tagGameRecord));
		m_nRecordFirst=(m_nRecordFirst+1)%MAX_SCORE_HISTORY;
	}

	m_nRecordCount = (m_nRecordLast-m_nRecordFirst+1+MAX_SCORE_HISTORY)%MAX_SCORE_HISTORY;

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
	ZeroMemory(m_lUserBet,sizeof(m_lUserBet));

	//ȫ����ע
	ZeroMemory(m_lAllUserBet,sizeof(m_lAllUserBet));

	if( NULL != m_pClientControlDlg && NULL != m_pClientControlDlg->GetSafeHwnd())
	{
		m_pClientControlDlg->m_UserBetArray.RemoveAll();
		m_pClientControlDlg->UpdateUserBet(true);
	}

	//���½���
	RefreshGameView();

	return;
}

//������ע
void CGameClientView::SetMePlaceJetton(BYTE cbViewIndex, SCORE lJettonCount)
{
	//Ч�����
	ASSERT(cbViewIndex>=0&& cbViewIndex<AREA_COUNT);
	if (!(cbViewIndex>=0&& cbViewIndex<AREA_COUNT)) return;

	m_lUserBet[cbViewIndex]=lJettonCount;

	//���½���
	RefreshGameView();
}

//�����˿�
void CGameClientView::SetDiceValue(BYTE cbDiceValue[])
{
	memcpy(m_cbDiceValue,cbDiceValue,sizeof(m_cbDiceValue));
}

//���ó���
void CGameClientView::PlaceUserJetton(BYTE cbViewIndex, SCORE lScoreCount,bool bReDraw)
{
	//Ч�����
	ASSERT(cbViewIndex<AREA_COUNT);
	if (cbViewIndex>=AREA_COUNT) return;

	//��������
	bool bPlaceJetton=false;
		//�߿���
	int nFrameWidth=0, nFrameHeight=0;
	int nBorderWidth=6;

	if(cbViewIndex>=0&&cbViewIndex<AREA_COUNT)
	{
		m_lAllUserBet[cbViewIndex]+=lScoreCount;
		nFrameWidth=m_rcBetArea[cbViewIndex].right-m_rcBetArea[cbViewIndex].left;
		nFrameHeight=m_rcBetArea[cbViewIndex].bottom-m_rcBetArea[cbViewIndex].top;
	}
	else
	{
		ASSERT(FALSE);
		return;
	}

	nFrameWidth += nBorderWidth;
	nFrameHeight += nBorderWidth;

	//�����ж�
	bool bAddJetton=lScoreCount>0?true:false;
	lScoreCount=(lScoreCount>0)?lScoreCount:-lScoreCount;

	//���ӳ���
	for (BYTE i=0;i<CountArray(g_lScoreType);i++)
	{
		//������Ŀ
		BYTE cbScoreIndex=CountArray(g_lScoreType)-i-1;
		SCORE lCellCount=lScoreCount/g_lScoreType[cbScoreIndex];

		//�������
		if (lCellCount==0L) continue;

		int nDeleteCount=0;
		//�������
		for (SCORE j=0;j<lCellCount;j++)
		{
			if (true==bAddJetton)
			{
				//�������
				tagJettonInfo JettonInfo;
				int nJettonSize=30;
				JettonInfo.cbJettonIndex=cbScoreIndex;
				JettonInfo.nXPos=rand()%(nFrameWidth-nJettonSize);
				JettonInfo.nYPos=rand()%(nFrameHeight-nJettonSize);

				//��������
				bPlaceJetton=true;
				m_JettonInfoArray[cbViewIndex].Add(JettonInfo);
			}
			else
			{
				for (int nIndex=0; nIndex<m_JettonInfoArray[cbViewIndex].GetCount(); ++nIndex)
				{
					//�Ƴ��ж�
					tagJettonInfo &JettonInfo=m_JettonInfoArray[cbViewIndex][nIndex];
					if (JettonInfo.cbJettonIndex==cbScoreIndex)
					{
						m_JettonInfoArray[cbViewIndex].RemoveAt(nIndex);
						nDeleteCount++;
						break;
					}
				}
			}
		}

		//������Ŀ
		if (!bAddJetton)
		    lScoreCount-=nDeleteCount*g_lScoreType[cbScoreIndex];
		else
		    lScoreCount-=lCellCount*g_lScoreType[cbScoreIndex];
	}

	//���½���
	if (bPlaceJetton&&bReDraw)
		  RefreshGameView();

	return;
}

//���ֳɼ�
void CGameClientView::SetCurGameScore(SCORE lMeCurGameScore, SCORE lMeCurGameReturnScore, SCORE lBankerCurGameScore, LONG lGameRevenue)
{
	m_lMeCurGameScore=lMeCurGameScore;			
	m_lMeCurGameReturnScore=lMeCurGameReturnScore;			
	m_lBankerCurGameScore=lBankerCurGameScore;			
	m_lGameRevenue=lGameRevenue;					
}

//����ʤ��
void CGameClientView::SetWinnerSide(bool *bWinArea,bool bSet)
{	
	//����ʱ��
	if (true==bSet)
	{
	

		//ȫʤ�ж�
		BYTE cbIndex=0;
		for(cbIndex=0;cbIndex<AREA_COUNT;cbIndex++)
		{
			if(bWinArea[cbIndex]!=false)break;
		}
		
		if (cbIndex==AREA_COUNT)
		{
			//�����Դ
			m_ImageFrame.Destroy();
			//������Դ
			HINSTANCE hInstance=AfxGetInstanceHandle();
			m_ImageFrame.LoadFromResource(hInstance,IDB_FRAME);
		}
	}
	else 
	{
		//�����ʱ��
		KillTimer(IDI_FLASH_WINNER);

		//ȫʤ�ж�
		BYTE cbIndex=0;
		for(cbIndex=0;cbIndex<AREA_COUNT;cbIndex++)
		{
			if(m_bWinFlag[cbIndex]!=false)break;
		}

		if (cbIndex==AREA_COUNT)
		{
			//�����Դ
			m_ImageFrame.Destroy();
			//������Դ
			HINSTANCE hInstance=AfxGetInstanceHandle();
			m_ImageFrame.LoadFromResource(hInstance,IDB_FRAME_EX);
		}
	}

	//���ñ���
	memcpy(m_bWinFlag,bWinArea,sizeof(m_bWinFlag));
	m_bFlashResult=bSet;
	m_bShowGameResult=bSet;
	m_cbAreaFlash=0xFF;

	//���½���
	RefreshGameView();

	return;
}

//��ȡ����
BYTE CGameClientView::GetJettonArea(CPoint MousePoint)
{
	BYTE cbIndex=0;
	for(cbIndex=0;cbIndex<AREA_COUNT;cbIndex++)
	{
		if(m_rcBetArea[cbIndex].PtInRect(MousePoint))
		{
			return cbIndex;
		}
	}
	return 0xFF;
}

BYTE CGameClientView::GetFramePngIndex(BYTE cbAreaIndex)
{
	if(cbAreaIndex==1)  return 0;
	if(cbAreaIndex==0)  return 1;
	if(cbAreaIndex==14)	return 4;
	if(cbAreaIndex==49) return 7;
	if(cbAreaIndex==44) return 9;
	if(cbAreaIndex>=2&&cbAreaIndex<=7)  return 2;
	if(cbAreaIndex>=8&&cbAreaIndex<=13) return 3;
	if(cbAreaIndex>=15&&cbAreaIndex<=28)return 5;
	if(cbAreaIndex>=29&&cbAreaIndex<=43)return 6;
	if(cbAreaIndex>44&&cbAreaIndex<49)return 8;
	return 0xFF;
}

//�滭����
void CGameClientView::DrawNumberString(CDC * pDC, LONG lNumber, INT nXPos, INT nYPos, bool bMeScore)
{
	//������Դ
	//CImageHandle HandleScoreNumber(&m_ImageScoreNumber);
	//CImageHandle HandleMeScoreNumber(&m_ImageMeScoreNumber);
	CSize SizeScoreNumber(m_ImageScoreNumber.GetWidth()/10,m_ImageScoreNumber.GetHeight());

	if ( bMeScore ) SizeScoreNumber.SetSize(m_ImageMeScoreNumber.GetWidth()/10, m_ImageMeScoreNumber.GetHeight());

	//������Ŀ
	LONG lNumberCount=0;
	SCORE lNumberTemp=lNumber;
	do
	{
		lNumberCount++;
		lNumberTemp/=10;
	} while (lNumberTemp>0);

	//�Ӷ���λ��
	LONG lAllCount =lNumberCount <= 3 ? lNumberCount : lNumberCount + (lNumberCount -1) / 3;

	//λ�ö���
	INT nYDrawPos=nYPos-SizeScoreNumber.cy/2;
	INT nXDrawPos=nXPos+lAllCount*SizeScoreNumber.cx/2-SizeScoreNumber.cx;

	//�滭����
	for (LONG i=0;i<lNumberCount;i++)
	{
		//�滭����
		LONG lCellNumber=(LONG)(lNumber%10);
		if ( bMeScore )
		{
			m_ImageMeScoreNumber.TransDrawImage(pDC,nXDrawPos,nYDrawPos,SizeScoreNumber.cx,SizeScoreNumber.cy,
				lCellNumber*SizeScoreNumber.cx,0,RGB(255,0,255));
		}
		else
		{
			m_ImageScoreNumber.TransDrawImage(pDC,nXDrawPos,nYDrawPos,SizeScoreNumber.cx,SizeScoreNumber.cy,
				lCellNumber*SizeScoreNumber.cx,0,RGB(255,0,255));
		}

		//���ñ���
		lNumber/=10;
		nXDrawPos-=SizeScoreNumber.cx;

		if (/*bMeScore && */lNumberCount - i > 1 && 2 == i % 3)
		{
			//m_ImageCommer.TransDrawImage(pDC,nXDrawPos,nYDrawPos-3,13,19,130,0,RGB(255,0,255));
			if (!bMeScore)
			{
				m_PngCommer.DrawImage(pDC, nXDrawPos+5, nYDrawPos+5, 13, 19, 130, 0, 13, 19);
			}
			else
				m_PngCommer.DrawImage(pDC, nXDrawPos, nYDrawPos-3, 13, 19, 130, 0, 13, 19);
			nXDrawPos -= 10;
		}
	};

	return;
}

//�滭����
void CGameClientView::DrawNumberStringWithSpace(CDC * pDC, LONGLONG lNumber, INT nXPos, INT nYPos)
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

		if (nNumberCount==4)
		{
			strTmpNumber2 += (TEXT(" ") +strNumber);
			strNumber=strTmpNumber2;
			nNumberCount=0;
			strTmpNumber2=TEXT("");
		}
		lNumber/=10;
	}

	if (strTmpNumber2.IsEmpty()==FALSE)
	{
		strTmpNumber2 += (TEXT(" ") +strNumber);
		strNumber=strTmpNumber2;
	}

	if (lTmpNumber<0) strNumber=TEXT("-")+strNumber;

	//�������
	pDC->TextOut(nXPos,nYPos,strNumber);
}

//�滭����
void CGameClientView::DrawNumberStringWithSpace(CDC * pDC, LONGLONG lNumber, CRect rcRect, INT nFormat)
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
			strTmpNumber2 += (TEXT(" ") +strNumber);
			strNumber=strTmpNumber2;
			nNumberCount=0;
			strTmpNumber2=TEXT("");
		}
		lNumber/=10;
	}

	if (strTmpNumber2.IsEmpty()==FALSE)
	{
		strTmpNumber2 += (TEXT(" ") +strNumber);
		strNumber=strTmpNumber2;
	}

	if (lTmpNumber<0) strNumber=TEXT("-")+strNumber;

	//�������
	if (nFormat==-1) 
		CDFontEx::DrawText(this,pDC,12,400,  strNumber,rcRect,RGB(255,255,255),DT_END_ELLIPSIS|DT_LEFT|DT_TOP|DT_SINGLELINE);
		//pDC->DrawText(strNumber,rcRect,DT_END_ELLIPSIS|DT_LEFT|DT_TOP|DT_SINGLELINE);
	else 
		CDFontEx::DrawText(this,pDC,12,400,  strNumber,rcRect,RGB(255,255,255),nFormat);
		//pDC->DrawText(strNumber,rcRect,nFormat);
}

// �滭����
void CGameClientView::DrawNumber( CDC * pDC, CPngImage* ImageNumber, TCHAR * szImageNum, LONGLONG lOutNum,INT nXPos, INT nYPos, UINT uFormat /*= DT_LEFT*/ )
{
	TCHAR szOutNum[128] = {0};
	_sntprintf(szOutNum,CountArray(szOutNum),TEXT("%I64d"),lOutNum);
	DrawNumber(pDC, ImageNumber, szImageNum, szOutNum, nXPos, nYPos, uFormat);
}

// �滭����
void CGameClientView::DrawNumber( CDC * pDC, CPngImage* ImageNumber, TCHAR * szImageNum, CString szOutNum, INT nXPos, INT nYPos, UINT uFormat /*= DT_LEFT*/ )
{
	TCHAR szOutNumT[128] = {0};
	_sntprintf(szOutNumT,CountArray(szOutNumT),TEXT("%s"),szOutNum);
	DrawNumber(pDC, ImageNumber, szImageNum, szOutNumT, nXPos, nYPos, uFormat);
}


// �滭����
void CGameClientView::DrawNumber(CDC * pDC , CPngImage* ImageNumber, TCHAR * szImageNum, TCHAR* szOutNum ,INT nXPos, INT nYPos,  UINT uFormat /*= DT_LEFT*/)
{
	// ������Դ
	INT nNumberHeight=ImageNumber->GetHeight();
	INT nNumberWidth=ImageNumber->GetWidth()/lstrlen(szImageNum);

	if ( uFormat == DT_CENTER )
	{
		nXPos -= (INT)(((DOUBLE)(lstrlen(szOutNum)) / 2.0) * nNumberWidth);
	}
	else if ( uFormat == DT_RIGHT )
	{
		nXPos -= lstrlen(szOutNum) * nNumberWidth;
	}

	for ( INT i = 0; i < lstrlen(szOutNum); ++i )
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

// ��Ӷ���
CString CGameClientView::AddComma( LONGLONG lScore , bool bPlus /*= false*/)
{
	CString strScore;
	CString strReturn;
	LONGLONG lNumber = lScore;
	if ( lScore < 0 )
		lNumber = -lNumber;

	strScore.Format(TEXT("%I64d"), lNumber);

	int nStrCount = 0;
	for( int i = strScore.GetLength() - 1; i >= 0; )
	{
		if( (nStrCount%3) == 0 && nStrCount != 0 )
		{
			strReturn.Insert(0, ',');
			nStrCount = 0;
		}
		else
		{
			strReturn.Insert(0, strScore.GetAt(i));
			nStrCount++;
			i--;
		}
	}

	if ( lScore < 0 )
		strReturn.Insert(0, '-');

	if ( bPlus && lScore > 0 )
		strReturn.Insert(0, '+');

	return strReturn;
}
void CGameClientView::KillCardTime()
{
	KillTimer(IDI_FLASH_WINNER);
	KillTimer(IDI_POSTCARD);
}


void CGameClientView::OnTimeGameEnd()
{
	
	m_bAnimatFinish=true;
	m_cbPrintCount=DICE_COUNT;
	m_bDrawResult=true;
	SetGameHistory(m_cbDiceValue);
	//�ۼƻ���
	m_lMeStatisticScore+=m_lMeCurGameScore;
	m_lBankerWinScore=m_lTmpBankerWinScore;
	ZeroMemory(m_nWinMutiple,sizeof(m_nWinMutiple));
	m_GameLogic.DeduceWinner(m_nWinMutiple,m_cbDiceValue);
	//��������
	if (m_lMeCurGameScore>0) m_pGameClientEngine->PlayGameSound(AfxGetInstanceHandle(),TEXT("END_WIN"));
	else if (m_lMeCurGameScore<0) m_pGameClientEngine->PlayGameSound(AfxGetInstanceHandle(),TEXT("END_LOST"));
	else m_pGameClientEngine->PlayGameSound(AfxGetInstanceHandle(),TEXT("END_DRAW"));
	SetTimer(IDI_FLASH_WINNER,500,NULL);
}

//��ʱ����Ϣ
void CGameClientView::OnTimer(UINT nIDEvent)
{

	switch(nIDEvent)
	{	
		case IDI_FLASH_WINNER:				//����ʤ��
		{
			m_bFlashResult=!m_bFlashResult;	//���ñ���
			RefreshGameView();			//���½���
			break;
		}
		//�ֻ�ׯ��
		case IDI_SHOW_CHANGE_BANKER:
		{
			ShowChangeBanker( false );
			break;
		}
		case IDI_MOVECARD_END:
		{
			KillTimer(IDI_MOVECARD_END);
			break;
		}
		case IDI_SHOW_DICE:
		{
			m_cbPrintCount++;
			if(m_cbPrintCount>DICE_COUNT)
			{	
				OnTimeGameEnd();
				KillTimer(IDI_SHOW_DICE);
			
				
			}
			else
				m_pGameClientEngine->PlayGameSound(AfxGetInstanceHandle(),_T("DISPATCH_CARD"));

			RefreshGameView();
			break;
		}

	}

	__super::OnTimer(nIDEvent);
}

//�����Ϣ
void CGameClientView::OnLButtonDown(UINT nFlags, CPoint Point)
{
	if (m_lCurrentJetton!=0L)
	{
		//��ע����
		BYTE cbJettonArea=GetJettonArea(Point);
		if(cbJettonArea>=0&&cbJettonArea<AREA_COUNT)
		{
			//�����ע
			if(m_lCurrentJetton>GetUserMaxBet(cbJettonArea))
				return;
			//������Ϣ
			if (cbJettonArea!=0xFF) 
				SendEngineMessage(IDM_PLACE_JETTON,cbJettonArea,m_lCurrentJetton);
		}
	}
	RefreshGameView();

	__super::OnLButtonDown(nFlags,Point);
}


//���������ؼ���������Ϣ
LRESULT CGameClientView::OnViLBtUp(WPARAM wParam, LPARAM lParam)
{
	CPoint *pPoint = (CPoint*)lParam;
	ScreenToClient(pPoint);
    OnLButtonUp(1,*pPoint);
	return 1;

}

LRESULT CGameClientView::OnAnimatEnd(WPARAM wParam,LPARAM lParam)
{
	m_bAnimatFinish=true;
	m_CartoonDice.ShowWindow(SW_HIDE);
	//���ö�ʱ��
	SetTimer(IDI_SHOW_DICE,TIME_SHOW_DICE,NULL);
	RefreshGameView();
	return true;
}

//�����Ϣ
void CGameClientView::OnRButtonDown(UINT nFlags, CPoint Point)
{
	//���ñ���
	m_lCurrentJetton=0L;

	if (m_cbGameStatus!=GS_GAME_END && m_cbAreaFlash!=0xFF)
	{
		m_cbAreaFlash=0xFF;
		RefreshGameView();
	}

	__super::OnLButtonDown(nFlags,Point);
}

//�����Ϣ
BOOL CGameClientView::OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage)
{
	if (m_lCurrentJetton!=0L)
	{
		//��ȡ����
		CPoint MousePoint;
		GetCursorPos(&MousePoint);
		ScreenToClient(&MousePoint);
		BYTE cbJettonArea=GetJettonArea(MousePoint);

		//���ñ���
		if ( m_cbAreaFlash!= cbJettonArea )
		{
			m_cbAreaFlash = cbJettonArea;
			RefreshGameView();
		}

		//�����ж�
		if (cbJettonArea==0xFF)
		{
			SetCursor(LoadCursor(NULL,IDC_ARROW));
			return TRUE;
		}

		//�����ע
		SCORE lMaxJettonScore=GetUserMaxBet(cbJettonArea);

		//�Ϸ��ж�
		if((m_lAllUserBet[cbJettonArea]+m_lCurrentJetton)>m_lAreaLimitScore)
		{
			SetCursor(LoadCursor(NULL,IDC_NO));
			return TRUE;
		}
	
		//���ù��
		if(m_nJettonIndex>=0&&m_nJettonIndex<JETTON_COUNT)
		{
			SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(g_nJettonIDI[m_nJettonIndex])));
			return TRUE;
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
	pDC->SetTextColor(crFrame);
	for (int i=0;i<CountArray(nXExcursion);i++)
	{
		//pDC->TextOut(nXPos+nXExcursion[i],nYPos+nYExcursion[i],pszString,nStringLength);

		TextOut(pDC,nXPos+nXExcursion[i],nYPos+nYExcursion[i],pszString,nStringLength);
	}

	//�滭����
	pDC->SetTextColor(crText);
	//pDC->TextOut(nXPos,nYPos,pszString,nStringLength);
	TextOut(pDC,nXPos,nYPos,pszString,nStringLength);

	return;
}
//ׯ����Ϣ
void CGameClientView::SetBankerInfo(DWORD dwBankerUserID, SCORE lBankerScore) 
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



//��ʾ���
void CGameClientView::ShowGameResult(CDC *pDC, int nWidth, int nHeight)
{
	//��ʾ�ж�
	if (m_cbGameStatus!=GS_GAME_END) return;
	int	nXPos = nWidth / 2 - 148;
	int	nYPos = nHeight / 2 - 208;
	//CImageHandle ImageHandleGameEnd(&m_ImageGameEnd);

	/*CRect rcAlpha(nXPos+2, nYPos+70, nXPos+2 + m_ImageGameEnd.GetWidth(), nYPos+70+m_ImageGameEnd.GetHeight());
	DrawAlphaRect(pDC, &rcAlpha, RGB(74,70,73), 0.8f);*/
	m_ImageGameEnd.DrawImage(pDC, nXPos, nYPos+70);

	pDC->SetTextColor(RGB(255,255,255));
	CRect rcMeWinScore, rcMeReturnScore;
	rcMeWinScore.left = nXPos+64;
	rcMeWinScore.top = nYPos+52+70;
	rcMeWinScore.right = rcMeWinScore.left + 107;
	rcMeWinScore.bottom = rcMeWinScore.top + 30;

	rcMeReturnScore.left = nXPos+174;
	rcMeReturnScore.top = nYPos+52+70;
	rcMeReturnScore.right = rcMeReturnScore.left + 107;
	rcMeReturnScore.bottom = rcMeReturnScore.top + 30;

	//CString strMeGameScore, strMeReturnScore;
	//strMeGameScore.Format( "%I64d", m_lMeCurGameScore );
	//strMeReturnScore.Format( "%I64d", m_lMeCurGameReturnScore );
	//pDC->DrawText( NumberStringWithSpace(m_lMeCurGameScore), rcMeWinScore, DT_END_ELLIPSIS | DT_LEFT | DT_TOP| DT_SINGLELINE );
	//pDC->DrawText( NumberStringWithSpace(m_lMeCurGameReturnScore), rcMeReturnScore, DT_END_ELLIPSIS | DT_LEFT | DT_TOP| DT_SINGLELINE );

	CDFontEx::DrawText(this,pDC,12,400,  NumberStringWithSpace(m_lMeCurGameScore),rcMeWinScore,RGB(255,255,255),DT_END_ELLIPSIS|DT_CENTER|DT_VCENTER|DT_SINGLELINE);
	CDFontEx::DrawText(this,pDC,12,400,  NumberStringWithSpace(m_lMeCurGameReturnScore),rcMeReturnScore,RGB(255,255,255),DT_END_ELLIPSIS|DT_CENTER|DT_VCENTER|DT_SINGLELINE);

	CRect rcBankerWinScore;
	rcBankerWinScore.left = nXPos+64;
	rcBankerWinScore.top = nYPos+85+70;
	rcBankerWinScore.right = rcBankerWinScore.left + 107;
	rcBankerWinScore.bottom = rcBankerWinScore.top + 30;

	//CString strBankerCurGameScore;
	//strBankerCurGameScore.Format(TEXT("%I64d"), m_lBankerCurGameScore);
	//pDC->DrawText( NumberStringWithSpace(m_lBankerCurGameScore), rcBankerWinScore, DT_END_ELLIPSIS | DT_LEFT | DT_TOP| DT_SINGLELINE );
	CDFontEx::DrawText(this,pDC,12,400,  NumberStringWithSpace(m_lBankerCurGameScore),rcBankerWinScore,RGB(255,255,255),DT_END_ELLIPSIS|DT_CENTER|DT_VCENTER|DT_SINGLELINE);
}

CString CGameClientView::NumberStringWithSpace(SCORE lNumber)
{
	CString strTempNum, strNum;
	strTempNum.Format(_T("%I64d"), (lNumber>0?lNumber:-lNumber));

	int nLength = strTempNum.GetLength();
	for (int i = 0; i < int((nLength-1)/4); i++)
		strTempNum.Insert(nLength - 4*(i+1), _T(","));
	strNum.Format(_T("%s%s"), (lNumber<0?_T("-"):_T("")), strTempNum);

	return  strNum;
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

//ׯ����С�÷�
SCORE CGameClientView::GetBankerWinMinScore(BYTE cbAreaID)
{

	SCORE lAllUserBet[AREA_COUNT];
	CopyMemory(lAllUserBet,m_lAllUserBet,sizeof(lAllUserBet));

	SCORE lBankerWinMinScore=0;
	bool bFirst=true;
	for(int i=1;i<=6;i++)
		for(int j=1;j<=6;j++)
			for(int k=1;k<=6;k++)
			{
				BYTE cbDiceValue[DICE_COUNT];
				cbDiceValue[0]=i;
				cbDiceValue[1]=j;
				cbDiceValue[2]=k;

				int  nWinMultiple[AREA_COUNT];
				m_GameLogic.DeduceWinner(nWinMultiple,cbDiceValue);

				if(nWinMultiple[cbAreaID]<0) continue;

				SCORE lBankerWinScore=0;
				for (BYTE cbAreaIndex =0; cbAreaIndex<AREA_COUNT; cbAreaIndex++)
				{

					if (nWinMultiple[cbAreaIndex]>0) 
					{

						lBankerWinScore -= lAllUserBet[cbAreaIndex]* nWinMultiple[cbAreaIndex];

					}
					else
					{
						lBankerWinScore+= -lAllUserBet[cbAreaIndex] * nWinMultiple[cbAreaIndex];
					}

				}
				if(bFirst)
				{
					lBankerWinMinScore=lBankerWinScore;
					bFirst=false;
				}
				else
				{
					if(lBankerWinScore<lBankerWinMinScore)
					{
						lBankerWinMinScore=lBankerWinScore;

					}
				}
			}
			return lBankerWinMinScore;
}



SCORE CGameClientView::GetUserMaxBet(BYTE cbAreaID)
{
	//����ע��
	SCORE lNowJetton = 0;
	SCORE lCurUserMaxBet=0;
	ASSERT(AREA_COUNT<=CountArray(m_lUserBet));
	for (int nAreaIndex=0; nAreaIndex<AREA_COUNT; ++nAreaIndex) 
		lNowJetton += m_lUserBet[nAreaIndex];

	//���½��
	lCurUserMaxBet=GetClientUserItem(m_wMeChairID)->GetUserScore()-lNowJetton;
	//��ע����
	lCurUserMaxBet=__min(lCurUserMaxBet,m_lMeMaxScore-lNowJetton);
	//��������
	lCurUserMaxBet=__min(lCurUserMaxBet,m_lAreaLimitScore-m_lAllUserBet[cbAreaID]);
	////ׯ������
	//SCORE lBankerScore=2147483647;
	//if (m_wBankerUser!=INVALID_CHAIR) lBankerScore=m_lBankerScore;
	//for (int nAreaIndex=0; nAreaIndex<AREA_COUNT; ++nAreaIndex) 
	//	lBankerScore-=m_lAllUserBet[nAreaIndex]*g_cbAreaOdds[nAreaIndex];
	//lCurUserMaxBet=__min(lCurUserMaxBet,lBankerScore/g_cbAreaOdds[cbAreaID]);


	//ׯ������
	{
		SCORE lBankerScore=0x7fffffff;
		if(m_wBankerUser!=INVALID_CHAIR)
			lBankerScore=m_lBankerScore;

		lBankerScore=lBankerScore+GetBankerWinMinScore(cbAreaID);

		if ( lBankerScore < 0 )
		{
			if (m_wBankerUser!=INVALID_CHAIR)
			{
				lBankerScore=m_lBankerScore;
			}
			else
			{
				lBankerScore = 0x7fffffff;
			}
		}

		lCurUserMaxBet=__min(lCurUserMaxBet,lBankerScore/g_cbAreaOdds[cbAreaID]);
	}

	//��ֵ����
	lCurUserMaxBet=__max(lCurUserMaxBet,0);
	return lCurUserMaxBet;
	//ׯ�ҽ��
}

////�����ע
//SCORE CGameClientView::GetUserMaxJetton()
//{
//	//����ע��
//	SCORE lNowJetton = 0;
//	ASSERT(AREA_COUNT<=CountArray(m_lUserBet));
//	for (int nAreaIndex=0; nAreaIndex<AREA_COUNT; ++nAreaIndex) 
//		lNowJetton += m_lUserBet[nAreaIndex]*MAX_ODDS;
//
//	//ׯ�ҽ��
//	SCORE lBankerScore=2147483647;
//	if (m_wBankerUser!=INVALID_CHAIR) lBankerScore=m_lBankerScore;
//	for (int nAreaIndex=0; nAreaIndex<AREA_COUNT; ++nAreaIndex) 
//		lBankerScore-=m_lAllUserBet[nAreaIndex]*MAX_ODDS;
//
//	//��������
//	SCORE lMeMaxScore;
//	if((m_lMeMaxScore-lNowJetton)/MAX_ODDS>m_lAreaLimitScore)
//	{
//		lMeMaxScore= m_lAreaLimitScore*MAX_ODDS;
//
//	}
//	else
//	{
//		lMeMaxScore = m_lMeMaxScore-lNowJetton;
//		lMeMaxScore = lMeMaxScore;
//	}
//
//	//ׯ������
//	lMeMaxScore=min(lMeMaxScore,lBankerScore);
//
//	lMeMaxScore /= MAX_ODDS; 
//
//	//��������
//	lMeMaxScore = max(lMeMaxScore, 0);
//
//	return lMeMaxScore;
//}

//�ɼ�����
void CGameClientView::SetGameScore(SCORE lMeCurGameScore, SCORE lMeCurGameReturnScore, SCORE lBankerCurGameScore)
{
	m_lMeCurGameScore=lMeCurGameScore;
	m_lMeCurGameReturnScore=lMeCurGameReturnScore;
	m_lBankerCurGameScore=lBankerCurGameScore;	
}

//�滭����
void CGameClientView::DrawMeJettonNumber(CDC *pDC)
{
	//�滭����
	for (int nAreaIndex=0; nAreaIndex<AREA_COUNT; ++nAreaIndex)
	{
		if (m_lUserBet[nAreaIndex] > 0 )
		{
			m_ImageJettonBack.DrawImage(pDC,m_PointJettonNumber[nAreaIndex].x,m_PointJettonNumber[nAreaIndex].y);			
			CString szPrint;
			MakeString(szPrint,m_lUserBet[nAreaIndex]);
			DrawTextString(pDC,szPrint,RGB(255,217,91),RGB(0,0,0),&m_rcNumArea[nAreaIndex],DT_CENTER);
			//if(nAreaIndex%2==0)
			//	DrawNumber(pDC,&m_PngMeScoreNumber,TEXT("0123456789,"),AddComma((LONG)m_lUserBet[nAreaIndex]),m_PointJettonNumber[nAreaIndex].x,m_PointJettonNumber[nAreaIndex].y+25,DT_CENTER);
			//	//DrawNumberString(pDC,(LONG)m_lUserBet[nAreaIndex],m_PointJettonNumber[nAreaIndex].x,m_PointJettonNumber[nAreaIndex].y+25, true);
			//else
			//	DrawNumber(pDC,&m_PngMeScoreNumber,TEXT("0123456789,"),AddComma((LONG)m_lUserBet[nAreaIndex]),m_PointJettonNumber[nAreaIndex].x,m_PointJettonNumber[nAreaIndex].y+25,DT_CENTER);
				//DrawNumberString(pDC,(LONG)m_lUserBet[nAreaIndex],m_PointJettonNumber[nAreaIndex].x,m_PointJettonNumber[nAreaIndex].y+25, true);
		}
	}
}


////��������
//void CGameClientView::FinishDispatchCard()
//{
//	//���ñ�ʶ
//	m_bNeedSetGameRecord=false;
//
//	//�ƶ�Ӯ��
//	bool bWinArea[AREA_COUNT];
//	DeduceWinner(bWinArea);
//	//�����¼
//	if(m_bRecordWinner)
//		SetGameHistory(bWinArea);
//	m_bRecordWinner=true;
//	//�ۼƻ���
//	m_lMeStatisticScore+=m_lMeCurGameScore;
//	m_lBankerWinScore=m_lTmpBankerWinScore;
//	//����Ӯ��
//	SetWinnerSide(bWinArea,true);
//	//��������
//	if (m_lMeCurGameScore>0) m_pGameClientEngine->PlayGameSound(AfxGetInstanceHandle(),TEXT("END_WIN"));
//	else if (m_lMeCurGameScore<0) m_pGameClientEngine->PlayGameSound(AfxGetInstanceHandle(),TEXT("END_LOST"));
//	else m_pGameClientEngine->PlayGameSound(AfxGetInstanceHandle(),TEXT("END_DRAW"));
//}



void CGameClientView::MakeString(CString &strNum,SCORE lNumber)
{
	strNum = NumberStringWithSpace(lNumber);
	//CString static strNumber=TEXT(""), strTmpNumber1,strTmpNumber2;
	//strTmpNumber1.Empty();
	//strTmpNumber2.Empty();
	//strNumber.Empty();
	//if (lNumber==0) strNumber=TEXT("0");
	//int nNumberCount=0;
	//SCORE lTmpNumber=lNumber;
	//if (lNumber<0) lNumber=-lNumber;
	//bool blfirst = true;
	//bool bLongNum = false;
	//if(lNumber>=100)
	//{
	//	bLongNum = true;
	//}
	//while (lNumber>0)
	//{

	//	strTmpNumber1.Format(TEXT("%I64d"),lNumber%10);
	//	nNumberCount++;
	//	strTmpNumber2 = strTmpNumber1+strTmpNumber2;

	//	if (nNumberCount==3)
	//	{
	//		if(blfirst)
	//		{
	//			strTmpNumber2 += (TEXT("") +strNumber);
	//			blfirst = false;

	//		}
	//		else
	//		{
	//			strTmpNumber2 += (TEXT(",") +strNumber);

	//		}

	//		strNumber=strTmpNumber2;
	//		nNumberCount=0;
	//		strTmpNumber2=TEXT("");
	//	}
	//	lNumber/=10;
	//}

	//if (strTmpNumber2.IsEmpty()==FALSE)
	//{
	//	if(bLongNum)
	//		strTmpNumber2 += (TEXT(",") +strNumber);
	//	else
	//		strTmpNumber2 += strNumber;

	//	strNumber=strTmpNumber2;
	//}

	//if (lTmpNumber<0) strNumber=TEXT("-")+strNumber;

	//strNum = strNumber ;
}

//ʤ���߿�
void CGameClientView::FlashAreaFrame(int nWidth, int nHeight, CDC *pDC)
{
	//�Ϸ��ж�

	if (m_cbAreaFlash==0xFF && false==m_bFlashResult) return;

	int nXpos = m_rcBetArea[m_cbAreaFlash].left;
	int nYpos = m_rcBetArea[m_cbAreaFlash].top;
	switch(m_cbAreaFlash)
			{
			case 0:
				{
					nXpos=m_rcBetArea[m_cbAreaFlash].left-28;
					break;
				}
			case 1:
				{
					nXpos=m_rcBetArea[m_cbAreaFlash].left-18;
					break;
				}
			case 2:
			case 15:				
			case 40:
			case 41:
				{
					nXpos=m_rcBetArea[m_cbAreaFlash].left-14;
					break;
				}
			case 3:
			case 19:
			case 23:
			case 45:			
				{
					nXpos=m_rcBetArea[m_cbAreaFlash].left-5;
					break;
				}
			case 4:
			case 13:
			case 18:			
				{
					nXpos=m_rcBetArea[m_cbAreaFlash].left-6;
					break;
				}
			case 5:
			case 39:
			case 47:
				{
					nXpos=m_rcBetArea[m_cbAreaFlash].left-8;
					break;
				}
			case 6:
			case 24:
			case 32:			
			case 51:
				{
					nXpos=m_rcBetArea[m_cbAreaFlash].left-9;
					break;
				}
			case 7:
			case 17:
			case 25:				
			case 44:			
				{
					nXpos=m_rcBetArea[m_cbAreaFlash].left-10;
					break;
				}
			case 37:
			case 46:			
				{
					nXpos=m_rcBetArea[m_cbAreaFlash].left-3;
					break;
				}
			case 10:
			case 35:
			case 36:			
				{
					nXpos=m_rcBetArea[m_cbAreaFlash].left-1;
					break;
				}
			case 8:
			case 9:
			case 11:
			case 12:
			case 22:
			case 34:			
				{
					nXpos=m_rcBetArea[m_cbAreaFlash].left-4;
					break;
				}
			case 14:
			case 33:
			case 38:
				{
					nXpos=m_rcBetArea[m_cbAreaFlash].left-7;
					break;
				}
			case 27:
			case 28:
			case 43:
				{
					nXpos=m_rcBetArea[m_cbAreaFlash].left-17;
					break;
				}
			case 26:
			case 30:			
			case 50:
				{
					nXpos=m_rcBetArea[m_cbAreaFlash].left-12;
					break;
				}
			case 20:			
				{
					nXpos=m_rcBetArea[m_cbAreaFlash].left-2;
					break;
				}
			case 16:
			case 31:
			case 48:
			case 49:
				{
					nXpos=m_rcBetArea[m_cbAreaFlash].left-11;
					break;
				}
			case 29:
			case 42:			
				{
					nXpos=m_rcBetArea[m_cbAreaFlash].left-16;
					break;
				}
			default: break;
			}

	//��ע�ж�
	if (false==m_bFlashResult)
	{
		if (m_cbGameStatus==GS_PLAYER_BET)
		{
			m_PngAreaFrame[m_cbAreaFlash].DrawImage(pDC,nXpos,nYpos);
		}
	}
	else
	{
		//������Դ		
		int nAreaX=0;
		int nAreaY=0;
		for(BYTE cbArea=0;cbArea<AREA_COUNT;cbArea++)
		{
			nAreaX=m_rcBetArea[cbArea].left;
			nAreaY=m_rcBetArea[cbArea].top;
			switch(cbArea)
			{
			case 0:
				{
					nAreaX=m_rcBetArea[cbArea].left-28;
					break;
				}
			case 1:
				{
					nAreaX=m_rcBetArea[cbArea].left-18;
					break;
				}
			case 2:
			case 15:				
			case 40:
			case 41:
				{
					nAreaX=m_rcBetArea[cbArea].left-14;
					break;
				}
			case 3:
			case 19:
			case 23:
			case 45:			
				{
					nAreaX=m_rcBetArea[cbArea].left-5;
					break;
				}
			case 4:
			case 13:
			case 18:			
				{
					nAreaX=m_rcBetArea[cbArea].left-6;
					break;
				}
			case 5:
			case 39:
			case 47:
				{
					nAreaX=m_rcBetArea[cbArea].left-8;
					break;
				}
			case 6:
			case 24:
			case 32:			
			case 51:
				{
					nAreaX=m_rcBetArea[cbArea].left-9;
					break;
				}
			case 7:
			case 17:
			case 25:				
			case 44:			
				{
					nAreaX=m_rcBetArea[cbArea].left-10;
					break;
				}
			case 37:
			case 46:			
				{
					nAreaX=m_rcBetArea[cbArea].left-3;
					break;
				}
			case 10:
			case 35:
			case 36:			
				{
					nAreaX=m_rcBetArea[cbArea].left-1;
					break;
				}
			case 8:
			case 9:
			case 11:
			case 12:
			case 22:
			case 34:			
				{
					nAreaX=m_rcBetArea[cbArea].left-4;
					break;
				}
			case 14:
			case 33:
			case 38:
				{
					nAreaX=m_rcBetArea[cbArea].left-7;
					break;
				}
			case 27:
			case 28:
			case 43:
				{
					nAreaX=m_rcBetArea[cbArea].left-17;
					break;
				}
			case 26:
			case 30:			
			case 50:
				{
					nAreaX=m_rcBetArea[cbArea].left-12;
					break;
				}
			case 20:			
				{
					nAreaX=m_rcBetArea[cbArea].left-2;
					break;
				}
			case 16:
			case 31:
			case 48:
			case 49:
				{
					nAreaX=m_rcBetArea[cbArea].left-11;
					break;
				}
			case 29:
			case 42:			
				{
					nAreaX=m_rcBetArea[cbArea].left-16;
					break;
				}
			default: break;
			}
			if(m_nWinMutiple[cbArea]>0)
				m_PngAreaFrame[cbArea].DrawImage(pDC,nAreaX,nAreaY);
		}
	}
}


//
//
////�ƶ�Ӯ��
//void CGameClientView::DeduceWinner(int*nWinMultiple)
//{
//	//��������
//	tagAnalyseResult analyseResult;
//	memset(&analyseResult,0,sizeof(analyseResult));
//	for(BYTE cbIndex=0;cbIndex<DICE_COUNT;cbIndex++)
//	{
//		for(BYTE cbRecord=0;cbRecord<analyseResult.cbUnEqualDice;cbRecord++)
//		{
//			if(analyseResult.m_DiceType[cbRecord].cbDate==m_cbDiceValue[cbIndex])
//			{
//				analyseResult.m_DiceType[cbRecord].cbCount++;
//				break;
//			}
//		}
//
//		if(analyseResult.cbUnEqualDice==cbRecord)
//		{
//			analyseResult.m_DiceType[analyseResult.cbUnEqualDice].cbDate=m_cbDiceValue[cbIndex];
//			analyseResult.m_DiceType[analyseResult.cbUnEqualDice++].cbCount++;
//		}
//
//		analyseResult.cbAllDiceValue+=m_cbDiceValue[cbIndex];
//	}
//
//	//�ж϶��Ӻ����
//	for(BYTE cx=0;cx<3;cx++)
//	{
//		for(BYTE cy=cx+1;cy<3;cy++)
//		{
//			BYTE cbAreaIndex=DiceMutiple[m_cbDiceValue[cx]-1][m_cbDiceValue[cy]-1].cbAreaIndex;
//			nWinMultiple[cbAreaIndex]=DiceMutiple[m_cbDiceValue[cx]-1][m_cbDiceValue[cy]-1].nMutiple;
//		}
//	}
//
//	//�ж�������ֵ
//	if(analyseResult.cbAllDiceValue>=11&&analyseResult.cbAllDiceValue<=17) nWinMultiple[1]=1;
//	if(analyseResult.cbAllDiceValue>=4&&analyseResult.cbAllDiceValue<=10)  nWinMultiple[0]=1;
//	if(analyseResult.cbAllDiceValue>=4&&analyseResult.cbAllDiceValue<=17)
//	{
//		BYTE cbAreaIndex=DiceMutipleEx[analyseResult.cbAllDiceValue-4].cbAreaIndex;
//		nWinMultiple[cbAreaIndex]=DiceMutipleEx[analyseResult.cbAllDiceValue-4].nMutiple;
//	}
//	
//	//�ж�ͬ����ͬ�����͵���
//	for(BYTE cbIndex=0;cbIndex<analyseResult.cbUnEqualDice;cbIndex++)
//	{
//		//���·�6������
//		nWinMultiple[43+analyseResult.m_DiceType[cbIndex].cbDate]=analyseResult.m_DiceType[cbIndex].cbCount;
//		if(analyseResult.m_DiceType[cbIndex].cbCount==3)
//		{
//			nWinMultiple[7+analyseResult.m_DiceType[cbIndex].cbDate]=150;
//			nWinMultiple[14]=24;
//		}
//	}
//}

//�ؼ�����
BOOL CGameClientView::OnCommand(WPARAM wParam, LPARAM lParam)
{
	//��ȡID
	WORD wControlID=LOWORD(wParam);

	//�ؼ��ж�
	if(wControlID>=g_nJettonIDC[0]&&wControlID<=g_nJettonIDC[JETTON_COUNT-1])
	{
		SetCurrentJetton(g_lScoreType[wControlID-g_nJettonIDC[0]],wControlID-g_nJettonIDC[0]);
	}
	else
	{
		switch (wControlID)
		{
 			case IDC_CONTINUE_CARD:
				{
					SendEngineMessage(IDM_CONTINUE_CARD,0,0);
					break;
				}
			case IDC_BANK_DRAW:
				{
					OnBankDraw();
					break;
				}
			case IDC_BANK_STORAGE:
				{
					OnBankStorage();
					break;
				}
			case IDC_BT_LOOK_RECORD:
				{
					
					m_bShowRecord=!m_bShowRecord;
					m_btCloseRecord.ShowWindow(m_bShowRecord?SW_SHOW:SW_HIDE);
					RefreshGameView();
					break;
				}
			case IDC_BT_CLOSE:
				{
					m_bShowRecord=false;
					m_btCloseRecord.ShowWindow(m_bShowRecord?SW_SHOW:SW_HIDE);
					RefreshGameView();
					break;
				}
			case IDC_BT_USER_LIST:
				{
					m_bShowList = !m_bShowList;
					if (m_bShowList)
					{
						m_bShowChat = false;
						m_UserList.ShowWindow(SW_SHOW);
						m_ChatInput.ShowWindow(SW_HIDE);
						m_ChatDisplay.SetHideWindow(true);
						m_ChatDisplay.ShowWindow(SW_HIDE);
						m_btSendMessage.ShowWindow(SW_HIDE);
					}
					else
					{
						m_UserList.ShowWindow(SW_HIDE);
					}
					RefreshGameView();
					break;
				}
			case IDC_BT_CHAT:
				{
					m_bShowChat = !m_bShowChat;
					if (m_bShowChat)
					{
						m_bShowList=false;
						m_UserList.ShowWindow(SW_HIDE);
						m_ChatInput.ShowWindow(SW_SHOW);
						m_ChatDisplay.ShowWindow(SW_SHOW);
						m_ChatDisplay.SetHideWindow(false);
						m_btSendMessage.ShowWindow(SW_SHOW);
					}
					else
					{
						m_ChatInput.ShowWindow(SW_HIDE);
						m_ChatDisplay.ShowWindow(SW_HIDE);
						m_ChatDisplay.SetHideWindow(true);
						m_btSendMessage.ShowWindow(SW_HIDE);
					}
					RefreshGameView();
					break;
				}
			case IDC_SEND_MESSAGE:
				{
					OnSendMessage();
					RefreshGameView();
					break;
				}
		}
	}

	return CGameFrameView::OnCommand(wParam, lParam);
}

//�ҵ�λ��
void CGameClientView::SetMeChairID(WORD dwMeUserID)
{
	//�������Ӻ�
	/*for (WORD wChairID=0; wChairID<MAX_CHAIR; ++wChairID)
	{
		IClientUserItem *pUserData=GetClientUserItem(wChairID);
		if (NULL!=pUserData && dwMeUserID==pUserData->GetUserID())
		{
			m_wMeChairID=wChairID;
			break;
		}
	}*/

	//CString str;
	//str.Format(TEXT("chairid=%d"),dwMeUserID);
	//AfxMessageBox(str);
	m_wMeChairID = dwMeUserID;
}


void CGameClientView::DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame,CRect*rect,UINT nFormat)
{
	//��������
	int nStringLength=lstrlen(pszString);
	int nXExcursion[8]={1,1,1,0,-1,-1,-1,0};
	int nYExcursion[8]={-1,0,1,1,1,0,-1,-1};

	//�滭�߿�
	nFormat|=DT_END_ELLIPSIS|DT_VCENTER|DT_SINGLELINE;
	//pDC->SetTextColor(crFrame);
	for (int i=0;i<CountArray(nXExcursion);i++)
	{
		//pDC->DrawText(pszString,&CRect(rect->left+nXExcursion[i],rect->top+nYExcursion[i],rect->right+nXExcursion[i],rect->bottom+nYExcursion[i]),nFormat);

		CDFontEx::DrawText(this,pDC,12,400,  pszString,&CRect(rect->left+nXExcursion[i],rect->top+nYExcursion[i],rect->right+nXExcursion[i],rect->bottom+nYExcursion[i]),crFrame,nFormat);

	}

	//�滭����
	//pDC->SetTextColor(crText);
	//pDC->DrawText(pszString,rect,nFormat);
	CDFontEx::DrawText(this,pDC,12,400,  pszString,rect,crText,nFormat);

}

//�滭ׯ��
void CGameClientView::DrawBankerInfo(CDC *pDC,int nWidth,int nHeight)
{

	//ׯ����Ϣ																											
	pDC->SetTextColor(RGB(255,255,255));
	//��ȡ���
	IClientUserItem *pUserData = m_wBankerUser==INVALID_CHAIR ? NULL : GetClientUserItem(m_wBankerUser);
	//λ����Ϣ
	CRect static StrRect;
	StrRect.left = 68;
	StrRect.top = 26;
	StrRect.right = StrRect.left+130;
	StrRect.bottom = StrRect.top+15;

	//ׯ������
	DrawTextString(pDC,pUserData==NULL?(m_bEnableSysBanker?TEXT("ϵͳ��ׯ"):TEXT("������ׯ")):pUserData->GetNickName(),RGB(255,255,255),RGB(0,0,0),&StrRect);
	CString szPrint;

	//ׯ���ܷ�	
	StrRect.top += 24;
	StrRect.bottom = StrRect.top+15;
	MakeString(szPrint,pUserData==NULL?0:pUserData->GetUserScore());
	DrawTextString(pDC,szPrint,RGB(255,255,255),RGB(0,0,0),&StrRect);
	
	//ׯ�Ҿ���
	StrRect.top += 24;
	StrRect.bottom = StrRect.top+15;
	MakeString(szPrint,m_wBankerTime);
	DrawTextString(pDC,szPrint,RGB(255,255,255),RGB(0,0,0),&StrRect);

	//ׯ�ҳɼ�
	StrRect.top += 24;
	StrRect.bottom = StrRect.top+15;
	MakeString(szPrint,m_lBankerWinScore);
	DrawTextString(pDC,szPrint,RGB(255,255,255),RGB(0,0,0),&StrRect);
}

// �滭��ׯ�б�
void CGameClientView::DrawBankerList(CDC *pDC, int nWidth, int nHeight)
{
	// �������
	CPoint ptBegin(0,0);
	// ������ׯ�б�
	ptBegin.SetPoint( nWidth - 216, nHeight/2-297);

	int   nCount = 0;
	CRect rect(0, 0, 0, 0);	
	rect.SetRect(ptBegin.x , ptBegin.y , ptBegin.x + 182, ptBegin.y + 16);	

	for( int i = m_nShowValleyIndex; i < m_ValleysList.GetCount(); ++i )
	{
		IClientUserItem* pIClientUserItem = GetClientUserItem( m_ValleysList[i] );
		if ( pIClientUserItem == NULL  )
			continue;
		// ����
		CRect rectName(rect);
		rectName.left = ptBegin.x ;
		rectName.right = ptBegin.x + 85;
		DrawTextString( pDC, pIClientUserItem->GetNickName(), RGB(255,255,255) , RGB(0,0,0), &rectName, DT_END_ELLIPSIS|DT_LEFT|DT_VCENTER|DT_SINGLELINE);

		// ����
		CRect rectScore(rect);
		rectScore.left = ptBegin.x + 95;
		DrawNumberStringWithSpace( pDC,pIClientUserItem->GetUserScore(), rectScore, DT_END_ELLIPSIS|DT_RIGHT|DT_VCENTER|DT_SINGLELINE);

		// λ�õ���
		rect.OffsetRect(0, 16);

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
	m_bcfirstShowCard =  bcCard;
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
		//m_btBankStorage.GetWindowRect(&btRect);
		ShowInsureSave(pIClientKernel,CPoint(btRect.right-300,btRect.top+300));
	}
#endif
}

//����ȡ��
void CGameClientView::OnBankDraw()
{	
#ifdef __SPECIAL___
	//��ȡ�ӿ�
	CGameClientEngine *pGameClientEngine=CONTAINING_RECORD(this,CGameClientEngine,m_GameClientView);
	IClientKernel *pIClientKernel=(IClientKernel *)pGameClientEngine->m_pIClientKernel;

	if(NULL!=pIClientKernel)
	{
		CRect btRect;
		m_btBankDraw.GetWindowRect(&btRect);
		ShowInsureGet(pIClientKernel,CPoint(btRect.right/2,btRect.top-100));
	}
#endif
}


//�滭���
void CGameClientView::DrawMeInfo(CDC *pDC,int nWidth,int nHeight)
{
	//�Ϸ��ж�
	if (INVALID_CHAIR==m_wMeChairID) return;
	IClientUserItem *pMeUserData = GetClientUserItem(m_wMeChairID);
	if ( pMeUserData != NULL )
	{
		//�ܹ���ע
		SCORE lMeJetton=0L;
		for (int nAreaIndex=0; nAreaIndex<AREA_COUNT; ++nAreaIndex) 
			lMeJetton += m_lUserBet[nAreaIndex];

		//����λ��
		CRect rcDraw;
		rcDraw.left=m_ptPlayerInfo.x;
		rcDraw.top=m_ptPlayerInfo.y;
		rcDraw.right=rcDraw.left+130;
		rcDraw.bottom=rcDraw.top+15;
		CString szPrint;
	
		//�滭�ʺ�
		DrawTextString(pDC,pMeUserData->GetNickName(),RGB(255,255,255),RGB(0,0,0),&rcDraw);
		
		//�滭����
		rcDraw.top+=24;
		rcDraw.bottom=rcDraw.top+15;
		//�滭����
		MakeString(szPrint,pMeUserData->GetUserScore()-lMeJetton);
		DrawTextString(pDC,szPrint,RGB(255,255,255),RGB(0,0,0),&rcDraw);
		
		//�滭�ɼ�
		rcDraw.top+=24;
		rcDraw.bottom=rcDraw.top+15;
		MakeString(szPrint,m_lMeStatisticScore);
		DrawTextString(pDC,szPrint,RGB(255,255,255),RGB(0,0,0),&rcDraw);
		
	}
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

//ϵͳ��Ϣ
void CGameClientView::InsertSystemMessage(LPCTSTR pszString)
{
	CString strMessage=TEXT("��ϵͳ��Ϣ����");
	InsertMessage(strMessage,RGB(255,0,0));
	InsertMessage(pszString);
	InsertMessage(TEXT("\r\n"));

	return;
}

//������Ϣ
void CGameClientView::InsertChatMessage(LPCTSTR pszString)
{
	CString  strInit=pszString;
	CString  FileLeft,FileRight;
	int index = strInit.Find(TEXT("��"));
	if (index < 0) return;

	FileLeft = strInit.Left(index+1);
	FileRight = strInit.Right(strInit.GetLength()-index-1);
	//FileRight = StringHalfToFull(FileRight);

	InsertMessage(FileLeft, RGB(0, 255, 0));
	InsertMessage(FileRight);
	InsertMessage(TEXT("\r\n"));

	return;
}

//������Ϣ
void CGameClientView::InsertNormalMessage(LPCTSTR pszString)
{
	InsertMessage(pszString);
	InsertMessage(TEXT("\r\n"));

	return;
}

//������Ϣ
void CGameClientView::InsertAllChatMessage(LPCTSTR pszString, int nMessageType) 
{
	if(nMessageType==1)
	{
		InsertSystemMessage(pszString);
	}
	else if(nMessageType==2)
	{
		InsertChatMessage(pszString);
	}
	else
	{
		InsertNormalMessage(pszString);
	}

	m_ChatDisplay.PostMessage(WM_VSCROLL, SB_BOTTOM, 0);

	return;
}

//��Ϣ����
void CGameClientView::InsertMessage(LPCTSTR pszString, COLORREF color)
{
	CString mes;
	CRichEditCtrl *pEditCtrl = (CRichEditCtrl *)GetDlgItem(IDC_CHAT_DISPLAY);
	pEditCtrl->GetWindowText(mes);
	int nlength = mes.GetLength();
	pEditCtrl->SetSel(nlength,nlength);

	//��������
	CHARFORMAT cf;
	m_ChatDisplay.GetSelectionCharFormat(cf);
	cf.cbSize = sizeof(cf);
	cf.dwMask = CFM_SIZE|CFM_COLOR;
	cf.dwEffects = CFE_PROTECTED;
	cf.crTextColor = color;
	m_ChatDisplay.SetSelectionCharFormat(cf); 

	pEditCtrl->ReplaceSel(pszString, FALSE);
	//pEditCtrl->LineScroll(pEditCtrl->GetLineCount()); 

	return;
}

//��������
void CGameClientView::OnSendMessage() 
{
	CString str, mes;
	GetDlgItem(IDC_CHAT_INPUT)->GetWindowText(str);
	if (str.GetLength() < 1) return;

	//��ѯ�ӿ�
	IClientKernel * m_pIClientKernel;
	CGlobalUnits * pGlobalUnits = CGlobalUnits::GetInstance();
	ASSERT(pGlobalUnits->QueryGlobalModule(MODULE_CLIENT_KERNEL,IID_IClientKernel,VER_IClientKernel)!=NULL);
	m_pIClientKernel=(IClientKernel *)pGlobalUnits->QueryGlobalModule(MODULE_CLIENT_KERNEL,IID_IClientKernel,VER_IClientKernel);
	m_pIClientKernel->SendUserChatMessage(0, str, RGB(0,0,0));

	GetDlgItem(IDC_CHAT_INPUT)->SetWindowText(TEXT(""));
	GetDlgItem(IDC_CHAT_INPUT)->SetFocus();

	return;
}

void CGameClientView::OnGameEnd(const CMD_S_GameEnd * pGameEnd)
{
	//��������
	memcpy(m_cbDiceValue,pGameEnd->cbDiceValue,sizeof(m_cbDiceValue));
	//ׯ����Ϣ
	SetBankerScore(pGameEnd->nBankerTime, pGameEnd->lBankerTotallScore);
	//�ɼ���Ϣ
	SetCurGameScore(pGameEnd->lUserScore,pGameEnd->lUserReturnScore,pGameEnd->lBankerScore,pGameEnd->lRevenue);

	m_CartoonDice.PlayAnimation(0,m_FrameDice,3,false,true);
	m_pGameClientEngine->PlayGameSound(AfxGetInstanceHandle(),TEXT("IDW_SHACK_DICE"));
	m_cbCurDirect=0;
}

void CGameClientView::OpenAdminWnd()
{
	//����д�Ȩ��
	if(m_pClientControlDlg != NULL )
	{
		if(!m_pClientControlDlg->IsWindowVisible())
		{
			m_pClientControlDlg->ShowWindow(SW_SHOW);
			m_pClientControlDlg->UpdateControl();
		}
		else 
			m_pClientControlDlg->ShowWindow(SW_HIDE);
	}
}

//������ͼ
void CGameClientView::RefreshGameView()
{
	CRect rect;
	GetClientRect(&rect);
	InvalidGameView(rect.left,rect.top,rect.Width(),rect.Height());
	//m_CartoonDice.Invalidate();

	return;
}

BOOL CGameClientView::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	if (pMsg->message == WM_KEYUP)
	{
		if (pMsg-> wParam == VK_RETURN)
		{
			CString str;
			GetDlgItem(IDC_CHAT_INPUT)->GetWindowText(str);
			if (str.GetLength() > 0)
			{
				OnSendMessage();
			}
			else
			{
				GetDlgItem(IDC_CHAT_INPUT)->SetFocus();
			}
		}
	}
	return CGameFrameViewGDI::PreTranslateMessage(pMsg);
}

HBRUSH CGameClientView::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CGameFrameViewGDI::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����
	if(pWnd->GetSafeHwnd() == GetDlgItem(IDC_CHAT_INPUT)->GetSafeHwnd())
	{
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(RGB(255,255,255));	
		return m_brush; 
	}
	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}
