#include "StdAfx.h"
#include "Resource.h"
#include "GlobalUnits.h"
#include "GameFrameWnd.h"
#include "DlgGameOption.h"
#include ".\gameframewnd.h"

//////////////////////////////////////////////////////////////////////////////////

//���ư�ť
#define IDC_MIN						100									//��С��ť
#define IDC_MAX						101									//���ť
#define IDC_CLOSE					102									//�رհ�ť
#define IDC_OPTION					103									//ѡ�ť
#define IDC_MUTE					104									//������ť
#define IDC_RULE					105									//�Թ۰�ť

//�ؼ���ʶ
#define IDC_SKIN_SPLITTER			200									//��ֿؼ�
#define IDC_GAME_CLIENT_VIEW		201									//��ͼ��ʶ

//��Ļλ��
#define BORAD_SIZE					3									//�߿��С
#define CAPTION_SIZE				32									//�����С

//�ؼ���С
#define SPLITTER_CX					0									//��ֿ��
#define CAPTION_SIZE				32									//�����С

//������С
#define FALSH_MOVIE_CX				300									//�������
#define FALSH_MOVIE_CY				300									//�����߶�

//��Ļ����
#define LESS_SCREEN_CY				740									//��С�߶�
#define LESS_SCREEN_CX				1024								//��С���

//////////////////////////////////////////////////////////////////////////////////
//�ؼ�����

//��ť�ؼ�
const TCHAR* const szButtonMinControlName = TEXT("ButtonMin");
const TCHAR* const szButtonMaxControlName = TEXT("ButtonMax");
const TCHAR* const szButtonCloseControlName = TEXT("ButtonClose");
const TCHAR* const szButtonResoreControlName = TEXT("ButtonResore");

//��ť�ؼ�
const TCHAR* const szButtonRuleControlName = TEXT("ButtonRule");
const TCHAR* const szButtonMusicControlName = TEXT("ButtonMusic");
const TCHAR* const szButtonRuietControlName = TEXT("ButtonRuiet");
const TCHAR* const szButtonOptionControlName = TEXT("ButtonOption");

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameFrameWnd, CFGuiFrameWnd)

	//ϵͳ��Ϣ
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_WM_NCHITTEST()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_GETMINMAXINFO()
	ON_WM_SETTINGCHANGE()

	//�Զ���Ϣ
	ON_MESSAGE(WM_SETTEXT,OnSetTextMesage)

	ON_WM_DESTROY()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CFrameEncircle::CFrameEncircle()
{
}

//��������
CFrameEncircle::~CFrameEncircle()
{
}

//�滭����
bool CFrameEncircle::PreDrawEncircleImage(tagEncircleBMP & EncircleImage)
{
	////����ͼ��
	//CBitImage ImageLogo;
	//ImageLogo.LoadFromResource(AfxGetInstanceHandle(),TEXT("GAME_LOGO"));

	////�滭ͼ��
	//if (ImageLogo.IsNull()==false)
	//{
	//	//��������
	//	CDC * pDC=CDC::FromHandle(EncircleImage.ImageTL.GetDC());

	//	//�滭ͼ��
	//	ImageLogo.TransDrawImage(pDC,10,6,RGB(255,0,255));

	//	//�ͷ���Դ
	//	EncircleImage.ImageTL.ReleaseDC();
	//}

	return true;
}

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CGameFrameWnd::CGameFrameWnd()
{
	//��������
	m_nScrollXPos=0L;
	m_nScrollYPos=0L;
	m_nScrollXMax=0L;
	m_nScrollYMax=0L;

	//״̬����
	m_bMaxShow=false;
	m_bShowControl=true;
	m_rcNormalSize.SetRect(0,0,0,0);

	//����ӿ�
	m_pIClientKernel=NULL;
	m_pIGameFrameView=NULL;

	//�ؼ�ָ��
	m_pDlgGameRule=NULL;

	//��ܻ���
	tagEncircleResource	EncircleFrame;
	EncircleFrame.pszImageTL=MAKEINTRESOURCE(IDB_FRAME_TL);
	EncircleFrame.pszImageTM=MAKEINTRESOURCE(IDB_FRAME_TM);
	EncircleFrame.pszImageTR=MAKEINTRESOURCE(IDB_FRAME_TR);
	EncircleFrame.pszImageML=MAKEINTRESOURCE(IDB_FRAME_ML);
	EncircleFrame.pszImageMR=MAKEINTRESOURCE(IDB_FRAME_MR);
	EncircleFrame.pszImageBL=MAKEINTRESOURCE(IDB_FRAME_BL);
	EncircleFrame.pszImageBM=MAKEINTRESOURCE(IDB_FRAME_BM);
	EncircleFrame.pszImageBR=MAKEINTRESOURCE(IDB_FRAME_BR);
	m_FrameEncircle.InitEncircleResource(EncircleFrame,GetModuleHandle(GAME_FRAME_DLL_NAME));

	//ע�����
	CGlobalUnits * pGlobalUnits=CGlobalUnits::GetInstance();
	pGlobalUnits->RegisterGlobalModule(MODULE_GAME_FRAME_WND,QUERY_ME_INTERFACE(IUnknownEx));

	return;
}

//��������
CGameFrameWnd::~CGameFrameWnd()
{
	//���ٿؼ�
	if(m_pDlgGameRule)
	{
		if(m_pDlgGameRule->m_hWnd != NULL)m_pDlgGameRule->DestroyWindow();
		delete m_pDlgGameRule;
		m_pDlgGameRule=NULL;
	}
}

//�ӿڲ�ѯ
VOID * CGameFrameWnd::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IGameFrameWnd,Guid,dwQueryVer);
	QUERYINTERFACE(IFlashControlSink,Guid,dwQueryVer);
	QUERYINTERFACE(IClientPropertySink,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IGameFrameWnd,Guid,dwQueryVer);
	return NULL;
}

//��ʼ�滭
void CGameFrameWnd::OnBeginPaintWindow(HDC hDC)
{
	////��ȡ�豸
	//CDC * pDC = CDC::FromHandle(hDC);

	////��ȡ����
	//CString strTitle;
	//GetWindowText(strTitle);

	////��ȡλ��
	//CRect rcClient;
	//GetClientRect(&rcClient);

	////���λ��
	//tagEncircleInfo FrameEncircleInfo;
	//m_FrameEncircle.GetEncircleInfo(FrameEncircleInfo);

	//////�滭���
	////m_FrameEncircle.DrawEncircleFrame(pDC,rcClient);

	////����ͼ��
	//CBitImage ImageLogo;
	//ImageLogo.LoadFromResource(AfxGetInstanceHandle(),TEXT("GAME_LOGO"));

	////�滭ͼ��
	//if (ImageLogo.IsNull()==false)
	//{
	//	//�滭ͼ��
	//	ImageLogo.TransDrawImage(pDC,10,7,RGB(255,0,255));
	//}

	////�滭����
	//if (strTitle.IsEmpty()==false)
	//{
	//	//��������
	//	CRect rcTitle;
	//	rcTitle.SetRect(35,8,rcClient.Width()-150,FrameEncircleInfo.nTBorder-8);

	//	//��������
	//	INT nXMove[8]={1,1,1,0,-1,-1,-1,0};
	//	INT nYMove[8]={-1,0,1,1,1,0,-1,-1};

	//	//���û���
	//	pDC->SetBkMode(TRANSPARENT);
	//	pDC->SetTextColor(RGB(100,100,100));
	//	pDC->SelectObject(CSkinResourceManager::GetInstance()->GetDefaultFont());

	//	//�滭�߿�
	//	for (INT i=0;i<CountArray(nXMove);i++)
	//	{
	//		//����λ��
	//		CRect rcTitleFrame;
	//		rcTitleFrame.top=rcTitle.top+nYMove[i];
	//		rcTitleFrame.left=rcTitle.left+nXMove[i];
	//		rcTitleFrame.right=rcTitle.right+nXMove[i];
	//		rcTitleFrame.bottom=rcTitle.bottom+nYMove[i];

	//		//�滭�ַ�
	//		pDC->DrawText(strTitle,&rcTitleFrame,DT_LEFT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);
	//	}

	//	//�滭����
	//	pDC->SetTextColor(RGB(255,255,255));
	//	pDC->DrawText(strTitle,&rcTitle,DT_LEFT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);
	//}

	return;
}

//��ʼ�ؼ�
void CGameFrameWnd::InitControlUI()
{
	//�������
	CGlobalUnits *pGlobalUnits=CGlobalUnits::GetInstance();

	//������ť
	CControlUI * pControlUI = GetControlByName(szButtonMusicControlName); 
	if(pControlUI!=NULL) pControlUI->SetVisible(pGlobalUnits->m_bMuteStatuts);

	//������ť
	pControlUI = GetControlByName(szButtonRuietControlName);
	if(pControlUI!=NULL) pControlUI->SetVisible(!pGlobalUnits->m_bMuteStatuts);
}

//��Ϣ����
void CGameFrameWnd::Notify(TNotifyUI &  msg)
{
	//��ȡ����
	CControlUI * pControlUI = msg.pSender;

	//����¼�
	if (lstrcmp(msg.sType, TEXT("click")) == 0)
	{		
		if(lstrcmp(pControlUI->GetName(), szButtonCloseControlName)==0)    //�رհ�ť
		{
			//��ȡ�û�
			ASSERT(m_pIClientKernel!=NULL);
			IClientUserItem * pIClientUserItem=m_pIClientKernel->GetMeUserItem();

			//ǿ����ʾ
			if ((pIClientUserItem!=NULL)&&(pIClientUserItem->GetUserStatus()==US_PLAYING))
			{
				//��ʾ��Ϣ
				CInformation Information(this);
				INT nRes=0;
				if (m_pIClientKernel->GetServerAttribute()->wServerType!=GAME_GENRE_MATCH)
				{
					nRes=Information.ShowMessageBox(TEXT("��������Ϸ�У�ǿ���˳������۷֣�ȷʵҪǿ����"),MB_ICONQUESTION|MB_YESNO,0);
				}
				else if(m_pIClientKernel->GetServerAttribute()->wServerType==GAME_GENRE_MATCH && m_pIClientKernel->GetServerAttribute()->cbMatchType==MATCH_TYPE_IMMEDIATE)
				{
					nRes=Information.ShowMessageBox(TEXT("�����ڱ����У�ǿ���˳����ᱻϵͳ�йܣ���Ӯ�Ը���ȷʵҪǿ����"),MB_ICONQUESTION|MB_YESNO,0);
				}
				else if(m_pIClientKernel->GetServerAttribute()->wServerType==GAME_GENRE_MATCH && m_pIClientKernel->GetServerAttribute()->cbMatchType==MATCH_TYPE_LOCKTIME)
				{
					nRes=Information.ShowMessageBox(TEXT("�����ڱ����У�ȷʵҪǿ����"),MB_ICONQUESTION|MB_YESNO,0);
				}
				if (nRes!=IDYES)
				{
					return;
				}
			}
			if(m_pIClientKernel->GetServerAttribute()->wServerType==GAME_GENRE_MATCH)
			{
				m_pIClientKernel->SendProcessData(IPC_CMD_GF_MATCH_INFO,IPC_SUB_GF_EXIT_MATCH);
			}
			
			//Ͷ�ݹر�
			PostMessage(WM_CLOSE,0,0);

			return;
		}
		else if(lstrcmp(pControlUI->GetName(), szButtonMinControlName)==0)	//��С����ť
		{
			ShowWindow(SW_MINIMIZE);
			return;
		}
		else if(lstrcmp(pControlUI->GetName(), szButtonMaxControlName)==0 || //��󻯰�ť
				lstrcmp(pControlUI->GetName(), szButtonResoreControlName)==0 )
		{
			//���ڿ���
			if (m_bMaxShow==true)
			{
				RestoreWindow();
			}
			else
			{
				MaxSizeWindow();
			}

			//���½���
			RedrawWindow(NULL,NULL,RDW_ERASE|RDW_INVALIDATE|RDW_ERASENOW|RDW_UPDATENOW);

			return;
		}
		else if(lstrcmp(pControlUI->GetName(), szButtonRuleControlName)==0)		//����ť
		{
			CGlobalUnits *pGlobalUnits=CGlobalUnits::GetInstance();
			tagGameAttribute * pGameAttribute=pGlobalUnits->m_ClientKernelModule->GetGameAttribute();
			tagServerAttribute * pServerAttribute=pGlobalUnits->m_ClientKernelModule->GetServerAttribute();

			//��ȡ����
			CGlobalWebLink * pGlobalWebLink=CGlobalWebLink::GetInstance();

			//�����ַ
			TCHAR szBrowser[MAX_PATH]=TEXT("");
			_sntprintf(szBrowser,CountArray(szBrowser),TEXT("%s/GameRule.aspx?GameID=%ld&KindID=%ld"),pGlobalWebLink->GetPlatformLink(),pServerAttribute->wKindID,pGameAttribute->wKindID);

			//��ҳ��
			ShellExecute(NULL,TEXT("OPEN"),szBrowser,NULL,NULL,SW_NORMAL);

			return;
		}
		else if(lstrcmp(pControlUI->GetName(), szButtonOptionControlName)==0)	//���ð�ť
		{
			//��ʾ����
			CDlgGameOption DlgGameOption;
			DlgGameOption.DoModal();

			return;
		}
		else if(lstrcmp(pControlUI->GetName(), szButtonMusicControlName)==0 ||	//��������
				lstrcmp(pControlUI->GetName(), szButtonRuietControlName)==0)
		{
			//�������
			CGlobalUnits *pGlobalUnits=CGlobalUnits::GetInstance();
			pGlobalUnits->m_bMuteStatuts=!pGlobalUnits->m_bMuteStatuts;

			//������ť
			CControlUI * pControlUI = GetControlByName(szButtonMusicControlName); 
			if(pControlUI!=NULL) pControlUI->SetVisible(pGlobalUnits->m_bMuteStatuts);

			//������ť
			pControlUI = GetControlByName(szButtonRuietControlName);
			if(pControlUI!=NULL) pControlUI->SetVisible(!pGlobalUnits->m_bMuteStatuts);

			//״̬����
			if(pGlobalUnits->m_bMuteStatuts) pGlobalUnits->StopSound();

			//֪ͨ��Ϸ
			if(m_pIClientKernel) m_pIClientKernel->OnGameOptionChange();

			return;
		}
	}
}

//��Ϣ����
BOOL CGameFrameWnd::PreTranslateMessage(MSG * pMsg)
{
	//��ʾ��Ϣ
	if (m_ToolTipCtrl.m_hWnd!=NULL)
	{
		m_ToolTipCtrl.RelayEvent(pMsg);
	}

	return __super::PreTranslateMessage(pMsg);
}

//��ԭ����
bool CGameFrameWnd::RestoreWindow()
{
	//״̬�ж�
	if (m_bMaxShow==true)
	{
		//���ñ���
		m_bMaxShow=false;

		//��󻯰�ť
		CControlUI * pControlUI = GetControlByName(szButtonResoreControlName); 
		if(pControlUI!=NULL) pControlUI->SetVisible(false);

		//��ԭ��ť
		pControlUI = GetControlByName(szButtonMaxControlName);
		if(pControlUI!=NULL) pControlUI->SetVisible();

		//�ƶ�����
		LockWindowUpdate();
		SetWindowPos(NULL,m_rcNormalSize.left,m_rcNormalSize.top,m_rcNormalSize.Width(),m_rcNormalSize.Height(),SWP_NOZORDER);
		UnlockWindowUpdate();
	}

	return true;
}

//��󴰿�
bool CGameFrameWnd::MaxSizeWindow()
{
	//״̬�ж�
	if (m_bMaxShow==false)
	{
		//���ñ���
		m_bMaxShow=true;

		//Ĭ��λ��
		GetWindowRect(&m_rcNormalSize);

		//��󻯰�ť
		CControlUI * pControlUI = GetControlByName(szButtonMaxControlName); 
		if(pControlUI!=NULL) pControlUI->SetVisible(false);

		//��ԭ��ť
		pControlUI = GetControlByName(szButtonResoreControlName);
		if(pControlUI!=NULL) pControlUI->SetVisible();

		//��ȡλ��
		CRect rcArce;
		SystemParametersInfo(SPI_GETWORKAREA,0,&rcArce,SPIF_SENDCHANGE);

		//�ƶ�����
		LockWindowUpdate();
		SetWindowPos(NULL,rcArce.left-2,rcArce.top-2,rcArce.Width()+4,rcArce.Height()+4,SWP_NOZORDER);
		UnlockWindowUpdate();
	}

	return true;
}

//�����ؼ�
VOID CGameFrameWnd::RectifyControl(INT nWidth, INT nHeight)
{
	//״̬�ж�
	if ((nWidth==0)||(nHeight==0)) return;
	if ((m_nScrollXMax==0)||(m_nScrollYMax==0)) return;

	//�ƶ�׼��
	HDWP hDwp=BeginDeferWindowPos(32);
	UINT uFlags=SWP_NOACTIVATE|SWP_NOCOPYBITS|SWP_NOZORDER;

	//������Ϣ
	tagEncircleInfo EncircleInfoFrame;
	m_FrameEncircle.GetEncircleInfo(EncircleInfoFrame);

	//΢��һ��
	EncircleInfoFrame.nTBorder-=2;
	EncircleInfoFrame.nLBorder+=2;
	EncircleInfoFrame.nRBorder+=2;
	EncircleInfoFrame.nBBorder+=2;

	//��������
	CRect rcSplitter;
	rcSplitter.top=EncircleInfoFrame.nTBorder;
	rcSplitter.bottom=m_nScrollYMax-EncircleInfoFrame.nBBorder;

	//��ֿؼ�
	if (m_bShowControl==true)
	{
		rcSplitter.left=m_nScrollXMax*75L/100L;
		rcSplitter.right=rcSplitter.left+SPLITTER_CX;
	}
	else
	{
		rcSplitter.left=m_nScrollXMax-EncircleInfoFrame.nRBorder-SPLITTER_CX;
		rcSplitter.right=m_nScrollXMax-EncircleInfoFrame.nRBorder;
	}

	//��ѯ��Ϸ
	CGlobalUnits * pGlobalUnits=CGlobalUnits::GetInstance();
	IGameFrameView * pIGameFrameView=(IGameFrameView *)pGlobalUnits->QueryGlobalModule(MODULE_GAME_FRAME_VIEW,IID_IGameFrameView,VER_IGameFrameView);

	//��Ϸ��ͼ
	if (pIGameFrameView!=NULL)
	{
		HWND hWndView=pIGameFrameView->GetGameViewHwnd();
		DeferWindowPos(hDwp,hWndView,NULL,EncircleInfoFrame.nLBorder,EncircleInfoFrame.nTBorder,rcSplitter.left-EncircleInfoFrame.nLBorder,m_nScrollYMax-EncircleInfoFrame.nTBorder-EncircleInfoFrame.nBBorder,uFlags);
	}

	//�ƶ��ؼ�
	DeferWindowPos(hDwp,m_SkinSplitter,NULL,rcSplitter.left,rcSplitter.top,rcSplitter.Width(),rcSplitter.Height(),uFlags);
	DeferWindowPos(hDwp,m_GameFrameControl,NULL,rcSplitter.right,EncircleInfoFrame.nTBorder,m_nScrollXMax-rcSplitter.right-EncircleInfoFrame.nRBorder,m_nScrollYMax-EncircleInfoFrame.nBBorder-EncircleInfoFrame.nTBorder,uFlags);

	//�ƶ�����
	EndDeferWindowPos(hDwp);

	//�����ؼ�
	CRect rcWindow;
	HWND hGameViewWnd = m_pIGameFrameView->GetGameViewHwnd();
	::GetWindowRect(hGameViewWnd,rcWindow);
	m_FlashControl.SetPosition(rcWindow.left+(rcWindow.Width()-FALSH_MOVIE_CX)/2,rcWindow.top+(rcWindow.Height()-FALSH_MOVIE_CY)*2/5,FALSH_MOVIE_CX,FALSH_MOVIE_CY,SWP_NOSIZE);

	return;
}

//λ����Ϣ
VOID CGameFrameWnd::OnSize(UINT nType, INT cx, INT cy) 
{
	__super::OnSize(nType, cx, cy);

	//��������
	m_nScrollXMax=__max(LESS_SCREEN_CX,cx);
	m_nScrollYMax=__max(LESS_SCREEN_CY,cy);
	m_nScrollXPos=__min(0L,m_nScrollXMax-cy);
	m_nScrollYPos=__min(0L,m_nScrollYMax-cy);
	
	//��������
	SCROLLINFO ScrollInfoVert;
	SCROLLINFO ScrollInfoHorz;
	ZeroMemory(&ScrollInfoVert,sizeof(ScrollInfoVert));
	ZeroMemory(&ScrollInfoHorz,sizeof(ScrollInfoHorz));

	//���ù���
	ScrollInfoVert.cbSize=sizeof(ScrollInfoVert);
	ScrollInfoVert.fMask=SIF_RANGE|SIF_PAGE|SIF_POS;
	ScrollInfoVert.nMin=0;
	ScrollInfoVert.nMax=m_nScrollYMax;
	ScrollInfoVert.nPage=cy;
	ScrollInfoVert.nPos=m_nScrollYPos;

	//���ù���
	ScrollInfoHorz.cbSize=sizeof(ScrollInfoHorz);
	ScrollInfoHorz.fMask=SIF_RANGE|SIF_PAGE|SIF_POS;
	ScrollInfoHorz.nMin=0;
	ScrollInfoHorz.nMax=m_nScrollXMax;
	ScrollInfoHorz.nPage=cx;
	ScrollInfoHorz.nPos=m_nScrollXPos;

	//�����ؼ�
	RectifyControl(cx,cy);

	return;
}

//λ����Ϣ
VOID CGameFrameWnd::OnGetMinMaxInfo(MINMAXINFO * lpMMI)
{
	__super::OnGetMinMaxInfo(lpMMI);

	//����λ��
	CRect rcArce;
	SystemParametersInfo(SPI_GETWORKAREA,0,&rcArce,SPIF_SENDCHANGE);

	//����λ��
	lpMMI->ptMinTrackSize.x=__min(LESS_SCREEN_CX,rcArce.Width());
	lpMMI->ptMinTrackSize.y=__min(LESS_SCREEN_CY,rcArce.Height());

	return;
}

//������Ϣ
INT CGameFrameWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct)==-1) return -1;

	//���ô���
	ModifyStyle(WS_CAPTION,0,0);
	ModifyStyleEx(WS_BORDER|WS_EX_CLIENTEDGE|WS_EX_WINDOWEDGE,0,0);

	//���ô���
	ModifyStyle(0, WS_MINIMIZEBOX);
	ModifyStyle(0, WS_MAXIMIZEBOX);

	//��������
	ASSERT(CGlobalUnits::GetInstance()!=NULL);
	CGlobalUnits * pGlobalUnits=CGlobalUnits::GetInstance();

	//��ѯ�ӿ�
	m_pIClientKernel=(IClientKernel *)pGlobalUnits->QueryGlobalModule(MODULE_CLIENT_KERNEL,IID_IClientKernel,VER_IClientKernel);
	m_pIGameFrameView=(IGameFrameView *)pGlobalUnits->QueryGlobalModule(MODULE_GAME_FRAME_VIEW,IID_IGameFrameView,VER_IGameFrameView);

	//������ť
	CRect rcCreate(0,0,0,0);
	//���ð�ť
	HINSTANCE hInstance=GetModuleHandle(GAME_FRAME_DLL_NAME);

	//���ƴ���
	AfxSetResourceHandle(hInstance);
	m_GameFrameControl.Create(IDD_GAME_CONTROL,this);
	m_SkinSplitter.Create(NULL,NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_SKIN_SPLITTER);

	//��ʾ�ؼ�
	m_ToolTipCtrl.Create(this);
	m_ToolTipCtrl.Activate(TRUE);

	//����λ��
	CSize SizeRestrict;
	SizeRestrict.SetSize(LESS_SCREEN_CX,LESS_SCREEN_CY);

	//����λ��
	CRect rcArce;
	SystemParametersInfo(SPI_GETWORKAREA,0,&rcArce,SPIF_SENDCHANGE);

	//��ȡλ��
	CWHRegKey InfoKeyItem;
	if (InfoKeyItem.OpenRegKey(REG_INTERFACE_INFO,false)==true)
	{
		SizeRestrict.cx=InfoKeyItem.GetValue(TEXT("ScreenWidth"),rcArce.Width()/2);
		SizeRestrict.cy=InfoKeyItem.GetValue(TEXT("ScreenHeight"),rcArce.Height()/2);
	}

	//λ�õ���
	SizeRestrict.cx=__max(LESS_SCREEN_CX,SizeRestrict.cx);
	SizeRestrict.cy=__max(LESS_SCREEN_CY,SizeRestrict.cy);
	SizeRestrict.cx=__min(rcArce.Width(),SizeRestrict.cx);
	SizeRestrict.cy=__min(rcArce.Height(),SizeRestrict.cy);

	//�ƶ�����
	m_rcNormalSize.top=(rcArce.Height()-SizeRestrict.cy)/2;
	m_rcNormalSize.left=(rcArce.Width()-SizeRestrict.cx)/2;
	m_rcNormalSize.right=m_rcNormalSize.left+SizeRestrict.cx;
	m_rcNormalSize.bottom=m_rcNormalSize.top+SizeRestrict.cy;
	SetWindowPos(NULL,m_rcNormalSize.left,m_rcNormalSize.top,m_rcNormalSize.Width(),m_rcNormalSize.Height(),SWP_NOZORDER);

	//������ͼ
	ASSERT(m_pIGameFrameView!=NULL);
	if (m_pIGameFrameView!=NULL) m_pIGameFrameView->CreateGameViewWnd(this,IDC_GAME_CLIENT_VIEW);

	//�����ؼ�
	IUnknownEx * pIUnknownEx=QUERY_ME_INTERFACE(IFlashControlSink);
	m_FlashControl.CreateControl(this,CRect(0,0,FALSH_MOVIE_CX,FALSH_MOVIE_CY),pIUnknownEx);
	m_FlashControl.ShowControl(SW_HIDE);

	//ע��ӿ�
	m_pIClientKernel->SetClientPropertySink(QUERY_ME_INTERFACE(IClientPropertySink));

	//��ʾ����
	MaxSizeWindow();

	return 0L;
}

//�����Ϣ
VOID CGameFrameWnd::OnLButtonDblClk(UINT nFlags, CPoint Point)
{
	__super::OnLButtonDblClk(nFlags,Point);

	//״̬�ж�
	if (Point.y>CAPTION_SIZE) return;

	//���ڿ���
	if (m_bMaxShow==true)
	{
		RestoreWindow();
	}
	else
	{
		MaxSizeWindow();
	}
	
	return;
}

//���øı�
VOID CGameFrameWnd::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	__super::OnSettingChange(uFlags,lpszSection);

	//��ȡ��С
	CRect rcClient,rcWindow;
	GetClientRect(&rcClient);
	GetWindowRect(&rcWindow);
	ClientToScreen(&rcClient);

	//����λ��
	CRect rcArce;
	SystemParametersInfo(SPI_GETWORKAREA,0,&rcArce,0);

	//����λ��
	rcArce.top-=(rcClient.top-rcWindow.top);
	rcArce.left-=(rcClient.left-rcWindow.left);
	rcArce.right+=(rcWindow.right-rcClient.right);
	rcArce.bottom+=(rcWindow.bottom-rcClient.bottom);

	//�ƶ�����
	SetWindowPos(NULL,rcArce.left,rcArce.top,rcArce.Width(),rcArce.Height(),SWP_NOZORDER|SWP_NOCOPYBITS);

	return;
}

//������Ϣ
LRESULT	CGameFrameWnd::OnSetTextMesage(WPARAM wParam, LPARAM lParam)
{
	//Ĭ�ϵ���
	LRESULT lResult=DefWindowProc(WM_SETTEXT,wParam,lParam);

	//���±���
	Invalidate(TRUE);

	return lResult;
}

//��Ϸ����
bool CGameFrameWnd::ShowGameRule()
{
// 	try
// 	{
// 		//��������
// 		if (m_pDlgGameRule==NULL) m_pDlgGameRule=new CDlgGameRule;
// 
// 		//��������
// 		if (m_pDlgGameRule->m_hWnd==NULL)
// 		{
// 			AfxSetResourceHandle(GetModuleHandle(GAME_FRAME_DLL_NAME));
// 			m_pDlgGameRule->Create(IDD_GAME_RULE,GetDesktopWindow());
// 			AfxSetResourceHandle(GetModuleHandle(NULL));
// 		}
// 
// 		//��ʾ����
// 		m_pDlgGameRule->ShowWindow(SW_RESTORE);
// 		m_pDlgGameRule->SetForegroundWindow();
// 
// 		return true;
// 	}
// 	catch (...)
// 	{ 
// 		ASSERT(FALSE); 
// 		return false; 
// 	}

	return false;
}

//��������
bool CGameFrameWnd::AllowGameSound(bool bAllowSound)
{
	//���ñ���
	CGlobalUnits *pGlobalUnits=CGlobalUnits::GetInstance();
	pGlobalUnits->m_bAllowSound=bAllowSound;

	return true;
}

//���ƽӿ�
bool CGameFrameWnd::OnGameOptionChange()
{
	//�������
	CGlobalUnits *pGlobalUnits=CGlobalUnits::GetInstance();

	//֪ͨ��Ϸ
	if(m_pIClientKernel)
	{
		AllowGameSound(pGlobalUnits->m_bAllowSound);
		AllowGameLookon(0L, pGlobalUnits->m_bAllowLookon);
		m_pIClientKernel->OnGameOptionChange();
	}

	return true;
}

//�������
VOID CGameFrameWnd::OnEventFlashFinish()
{
	m_FlashControl.ShowControl(SW_HIDE);
}

//���߳ɹ�
VOID  CGameFrameWnd::OnPropertySuccess(LPCTSTR szSourceNickName,DWORD dwSourceUserID,DWORD dwTargerUserID,WORD wPropertyIndex,WORD wPropertyCount)
{
	//��ȡ����
	ASSERT(CGamePropertyManager::GetInstance()!=NULL);
	CGamePropertyManager * pGamePropertyManager=CGamePropertyManager::GetInstance();

	//Ѱ�ҵ���
	ASSERT(pGamePropertyManager->GetPropertyItem(wPropertyIndex)!=NULL);
	CGamePropertyBase * pGamePropertyItem=pGamePropertyManager->GetPropertyItem(wPropertyIndex);

	//���ߴ���
	if (pGamePropertyItem!=NULL && pGamePropertyItem->GetPropertyAttrib()->wPropertyType==PT_TYPE_PRESENT)
	{
		//ͼƬ��Ϣ
		tagPropertyImage PropertyImage;
		pGamePropertyItem->GetImageInfo(PropertyImage);

		//���Ŷ���
		if ((PropertyImage.pszFlash!=NULL)&&(PropertyImage.pszFlash[0]!=0))
		{
			//����·��
			TCHAR szDirectory[MAX_PATH]=TEXT("");
			CWHService::GetWorkDirectory(szDirectory,CountArray(szDirectory));

			//����·��
			TCHAR szFlashPath[MAX_PATH]=TEXT("");
			_sntprintf(szFlashPath,CountArray(szFlashPath),TEXT("%s\\Property\\%s"),szDirectory,PropertyImage.pszFlash);

			try
			{
				//���Ŷ���
				if (m_FlashControl.LoadMovieFile(szFlashPath)==false)
				{
					throw TEXT("�����ļ�����ʧ��");
				}

				//�����ؼ�
				CRect rcWindow;
				HWND hGameViewWnd = m_pIGameFrameView->GetGameViewHwnd();
				::GetWindowRect(hGameViewWnd,rcWindow);
				m_FlashControl.SetPosition(rcWindow.left+(rcWindow.Width()-FALSH_MOVIE_CX)/2,rcWindow.top+(rcWindow.Height()-FALSH_MOVIE_CY)*2/5,FALSH_MOVIE_CX,FALSH_MOVIE_CY,SWP_NOSIZE);

				//���Ŷ���
				m_FlashControl.PlayMovie();
				m_FlashControl.ShowControl(SW_SHOW);
			}
			catch (...)
			{
				ASSERT(FALSE);
			}
		}
	}

	//������ʾ
	if(pGamePropertyItem->GetPropertyAttrib()->wPropertyType==PT_TYPE_PROPERTY)
	{
		//��������
		TCHAR  szMessage[256]=TEXT("");
		IClientUserItem * pMeUserItem = m_pIClientKernel->GetMeUserItem();

		//��ȡ��ʾ
		if(dwSourceUserID==pMeUserItem->GetUserID() && dwSourceUserID!=dwTargerUserID)
            _sntprintf(szMessage,CountArray(szMessage),TEXT("��ϲ��,���߹���ɹ�!"));
		else
		{
			if(dwSourceUserID==dwTargerUserID)
				pGamePropertyItem->GetNotifyMessage(szSourceNickName,szSourceNickName,wPropertyCount,szMessage);
			else
				pGamePropertyItem->GetNotifyMessage(szSourceNickName,pMeUserItem->GetNickName(),wPropertyCount,szMessage);
		}

		//������Ϣ
		CInformation Information;
		Information.ShowMessageBox(szMessage,MB_ICONINFORMATION);
	}

	return;
}

//����ʧ��
VOID CGameFrameWnd::OnPropertyFailure( LONG lErrorCode,LPCTSTR szDescribeString)
{
	//������Ϣ
	CInformation Information;
	Information.ShowMessageBox(szDescribeString,MB_ICONINFORMATION);

	return;
}

bool CGameFrameWnd::SetTrumpetNum(DWORD dwTrumpetCount,DWORD dwTyphonCount)
{
	m_GameFrameControl.SetTrumpetNum(dwTrumpetCount,dwTyphonCount);
	return true;
}

//�Թۿ���
bool CGameFrameWnd::AllowGameLookon(DWORD dwUserID, bool bAllowLookon)
{
	//������Ϣ
	m_pIClientKernel->SendUserLookon(dwUserID,bAllowLookon);

	return true;
}

//��������
void CGameFrameWnd::OnDestroy()
{
	__super::OnDestroy();

	//ɾ��D2D����
	CD2DEngine::Delete();
}
//////////////////////////////////////////////////////////////////////////////////


