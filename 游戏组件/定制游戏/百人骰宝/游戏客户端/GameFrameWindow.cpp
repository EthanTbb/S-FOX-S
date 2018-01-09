#include "StdAfx.h"
#include "Resource.h"
#include "GameFrameWindow.h"
#include "GameClientview.h"
#include ".\gameframewindow.h"

//////////////////////////////////////////////////////////////////////////////////

//���ư�ť
#define IDC_MIN						100									//��С��ť
#define IDC_CLOSE_BUTTON			101									//�رհ�ť
#define IDC_SOUND					102									//������ť
#define IDC_OPTION					103									//���ð�ť


//�ؼ���ʶ
#define IDC_SKIN_SPLITTER			200									//��ֿؼ�
#define IDC_GAME_CLIENT_VIEW		201									//��ͼ��ʶ

//��Ļλ��
#define BORAD_SIZE					6									//�߿��С
#define CAPTION_SIZE				32									//�����С

//�ؼ���С
#define SPLITTER_CX					0									//��ֿ��
#define CAPTION_SIZE				32									//�����С

//��Ļ����
#define LESS_SCREEN_CY				737									//��С�߶�
#define LESS_SCREEN_CX				1022								//��С���

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameFrameWindow, CFrameWnd)

	//ϵͳ��Ϣ
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_SETTINGCHANGE()
	ON_WM_NCHITTEST()

	//�Զ���Ϣ
	ON_MESSAGE(WM_SETTEXT,OnSetTextMesage)

	ON_WM_NCPAINT()
	ON_WM_PAINT()
END_MESSAGE_MAP()

//���캯��
CGameFrameWindow::CGameFrameWindow()
{
	//����ӿ�
	m_pIClientKernel=NULL;
	m_pIGameFrameView=NULL;
	m_pIGameFrameService=NULL;

	//ע�����
	CGlobalUnits * pGlobalUnits=CGlobalUnits::GetInstance();
	pGlobalUnits->RegisterGlobalModule(MODULE_GAME_FRAME_WND,QUERY_ME_INTERFACE(IUnknownEx));
	
	int cx = GetSystemMetrics(SM_CXSCREEN);
	int cy = GetSystemMetrics(SM_CYSCREEN);
	//m_CurWindowSize.SetSize(cx,cy);
	m_CurWindowSize.SetSize(LESS_SCREEN_CX,LESS_SCREEN_CY);
}

//��������
CGameFrameWindow::~CGameFrameWindow()
{
}

//�ӿڲ�ѯ
VOID * CGameFrameWindow::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IUserEventSink,Guid,dwQueryVer);
	QUERYINTERFACE(IGameFrameWnd,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IGameFrameWnd,Guid,dwQueryVer);
	return NULL;
}

//��Ϣ����
BOOL CGameFrameWindow::PreTranslateMessage(MSG * pMsg)
{
	return __super::PreTranslateMessage(pMsg);
}

//�����
BOOL CGameFrameWindow::OnCommand(WPARAM wParam, LPARAM lParam)
{
	//��������
	UINT nCommandID=LOWORD(wParam);

	//���ܰ�ť
	switch (nCommandID)
	{
	case IDC_MIN:				//��С��ť
		{
			if (IsIconic())	
				ShowWindow(SW_RESTORE);
			else
				ShowWindow(SW_MINIMIZE);
			
			return TRUE;
		}
	case IDC_CLOSE_BUTTON:				//�رհ�ť
		{
			//��ѯ��Ϸ
			CGlobalUnits * pGlobalUnits=CGlobalUnits::GetInstance();
			IGameFrameView * pIGameFrameView=(IGameFrameView *)pGlobalUnits->QueryGlobalModule(MODULE_GAME_FRAME_VIEW,IID_IGameFrameView,VER_IGameFrameView);
			CGameClientView *pGameClientView = (CGameClientView*)pIGameFrameView;
			pGameClientView->SendEngineMessage(IDM_GAME_CLOSE,0,0);

			return TRUE;
		}
	case IDC_OPTION:
		{
			//��ʾ����
			CDlgGameOption DlgGameOption;
			if (DlgGameOption.CreateGameOption(NULL,0)==true)
			{
				break;
			}
			return TRUE;
		}
	case IDC_SOUND:
		{			
			//��������
			CGlobalUnits *pGlobalUnits=CGlobalUnits::GetInstance();
			pGlobalUnits->m_bMuteStatuts = !pGlobalUnits->m_bMuteStatuts;
			//״̬����
			if(pGlobalUnits->m_bMuteStatuts)
			{
				pGlobalUnits->StopSound();
			}
			UpdateSound();

			return TRUE;
		}
	}

	return __super::OnCommand(wParam,lParam);
}

//�����ؼ�
VOID CGameFrameWindow::RectifyControl(INT nWidth, INT nHeight)
{
	//״̬�ж�
	if ((nWidth==0)||(nHeight==0)) return;

	//�ƶ�׼��
	HDWP hDwp=BeginDeferWindowPos(32);
	UINT uFlags=SWP_NOACTIVATE|SWP_NOCOPYBITS|SWP_NOZORDER;

	//��ѯ��Ϸ
	CGlobalUnits * pGlobalUnits=CGlobalUnits::GetInstance();
	IGameFrameView * pIGameFrameView=(IGameFrameView *)pGlobalUnits->QueryGlobalModule(MODULE_GAME_FRAME_VIEW,IID_IGameFrameView,VER_IGameFrameView);

	//��Ϸ��ͼ
	if (pIGameFrameView!=NULL)
	{
		CRect rcArce;
		CSize SizeRestrict=m_CurWindowSize;
		SystemParametersInfo(SPI_GETWORKAREA,0,&rcArce,SPIF_SENDCHANGE);
		SizeRestrict.cx=__min(rcArce.Width(),SizeRestrict.cx);
		SizeRestrict.cy=__min(rcArce.Height(),SizeRestrict.cy);
		DeferWindowPos(hDwp, pIGameFrameView->GetGameViewHwnd(), NULL, 0, 35, SizeRestrict.cx, SizeRestrict.cy-35, uFlags);
	}

	DeferWindowPos(hDwp, m_GameFrameControl, NULL, 0, 0, 0, 0, uFlags);

	uFlags = SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOCOPYBITS|SWP_NOSIZE;
	DeferWindowPos(hDwp,m_btOption,NULL,nWidth-111,4,0,0,uFlags);
	DeferWindowPos(hDwp, m_btMinButton, NULL, nWidth-57, 4, 0, 0, uFlags);
	DeferWindowPos( hDwp,m_btSound,NULL, nWidth-84, 4, 0, 0, uFlags);
	DeferWindowPos(hDwp, m_btCloseButton, NULL, nWidth-30, 4, 0, 0, uFlags);

	//�ƶ�����
	EndDeferWindowPos(hDwp);

	return;
}

//�û�����
VOID CGameFrameWindow::OnEventUserEnter(IClientUserItem * pIClientUserItem, bool bLookonUser)
{
	//��������
	ASSERT(CGlobalUnits::GetInstance()!=NULL);
	CGlobalUnits * pGlobalUnits=CGlobalUnits::GetInstance();

	//��ʾ��Ϣ
	if (pGlobalUnits->m_bNotifyUserInOut==true)
	{
		//CDialogChat::m_ChatMessage.InsertUserEnter(pIClientUserItem->GetNickName());
	}

	return;
}

//�û��뿪
VOID CGameFrameWindow::OnEventUserLeave(IClientUserItem * pIClientUserItem, bool bLookonUser)
{
	return;
}

//�û�����
VOID CGameFrameWindow::OnEventUserScore(IClientUserItem * pIClientUserItem, bool bLookonUser)
{

	return;
}

//�û�״̬
VOID CGameFrameWindow::OnEventUserStatus(IClientUserItem * pIClientUserItem, bool bLookonUser)
{
	return;
}

//�û�ͷ��
VOID CGameFrameWindow::OnEventCustomFace(IClientUserItem * pIClientUserItem, bool bLookonUser)
{
	return;

}

//�û�����
VOID CGameFrameWindow::OnEventUserAttrib(IClientUserItem * pIClientUserItem, bool bLookonUser)
{

}

//�滭����
BOOL CGameFrameWindow::OnEraseBkgnd(CDC * pDC)
{
	return TRUE;
}

//λ����Ϣ
VOID CGameFrameWindow::OnSize(UINT nType, INT cx, INT cy) 
{
	__super::OnSize(nType, cx, cy);

	//�����ؼ�
	RectifyControl(cx,cy);

	return;
}


//������Ϣ
INT CGameFrameWindow::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct)==-1) return -1;

	//���ô���
	ModifyStyle(WS_CAPTION,0,0);
	ModifyStyleEx(WS_BORDER|WS_EX_CLIENTEDGE|WS_EX_WINDOWEDGE,0,0);

	//��������
	ASSERT(CGlobalUnits::GetInstance()!=NULL);
	CGlobalUnits * pGlobalUnits=CGlobalUnits::GetInstance();

	//��ѯ�ӿ�
	m_pIClientKernel=(IClientKernel *)pGlobalUnits->QueryGlobalModule(MODULE_CLIENT_KERNEL,IID_IClientKernel,VER_IClientKernel);
	m_pIGameFrameView=(IGameFrameView *)pGlobalUnits->QueryGlobalModule(MODULE_GAME_FRAME_VIEW,IID_IGameFrameView,VER_IGameFrameView);
	m_pIGameFrameService=(IGameFrameService *)pGlobalUnits->QueryGlobalModule(MODULE_GAME_FRAME_SERVICE,IID_IGameFrameService,VER_IGameFrameService);

	//�û��ӿ�
	IUserEventSink * pIUserEventSink=QUERY_ME_INTERFACE(IUserEventSink);
	if (pIUserEventSink!=NULL) m_pIClientKernel->SetUserEventSink(pIUserEventSink);

	//���ƴ���
	AfxSetResourceHandle(GetModuleHandle(GAME_FRAME_DLL_NAME));
	m_GameFrameControl.Create(8011,this);
	AfxSetResourceHandle(GetModuleHandle(NULL));

	//����ӿ�
	m_pIClientKernel->SetStringMessage(QUERY_OBJECT_INTERFACE(m_StringMessage,IUnknownEx));
	
	//����λ��
	CRect rcArce;
	SystemParametersInfo(SPI_GETWORKAREA,0,&rcArce,SPIF_SENDCHANGE);

	//��ȡλ��
	CSize SizeRestrict=m_CurWindowSize;
	//����λ��
	CRect rcNormalSize;			

	//λ�õ���
	SizeRestrict.cx=__min(rcArce.Width(),SizeRestrict.cx);
	SizeRestrict.cy=__min(rcArce.Height(),SizeRestrict.cy);

	//�ƶ�����
	rcNormalSize.top=(rcArce.Height()-SizeRestrict.cy)/2;
	rcNormalSize.left=(rcArce.Width()-SizeRestrict.cx)/2;
	rcNormalSize.right=rcNormalSize.left+SizeRestrict.cx;
	rcNormalSize.bottom=rcNormalSize.top+SizeRestrict.cy;
	SetWindowPos(NULL,rcNormalSize.left,rcNormalSize.top,rcNormalSize.Width(),rcNormalSize.Height(),SWP_NOZORDER);

	//������ͼ
	ASSERT(m_pIGameFrameView!=NULL);
	if (m_pIGameFrameView!=NULL) m_pIGameFrameView->CreateGameViewWnd(this,IDC_GAME_CLIENT_VIEW);

	m_PngFrameTop.LoadImage(AfxGetInstanceHandle(),TEXT("FRAME_TOP"));

	//��������
	m_DFontEx.CreateFont(this, TEXT("����"), 12, 400 );

	//��С��ť
	CRect rcCreate(0,0,0,0);
	m_btMinButton.Create(NULL, WS_CHILD|WS_VISIBLE, rcCreate, this, IDC_MIN);
	m_btMinButton.SetButtonImage(IDB_BT_MIN_BUTTON, AfxGetInstanceHandle(), false, false);

	//�رհ�ť
	m_btCloseButton.Create(NULL, WS_CHILD|WS_VISIBLE, rcCreate, this, IDC_CLOSE_BUTTON);
	m_btCloseButton.SetButtonImage(IDB_BT_CLOSE_BUTTON, AfxGetInstanceHandle(), false, false);

	//������ť
	m_btSound.Create(NULL, WS_CHILD|WS_VISIBLE, rcCreate, this, IDC_SOUND);
	m_btSound.SetButtonImage(IDB_BT_SOUND_OPEN, AfxGetInstanceHandle(), false, false);

	//���ð�ť
	m_btOption.Create(NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_OPTION);
	m_btOption.SetButtonImage(IDB_BT_OPTION,AfxGetInstanceHandle(),false,false);

	UpdateSound();

	return 0L;
}

//�����Ϣ
VOID CGameFrameWindow::OnLButtonDown(UINT nFlags, CPoint Point)
{
	__super::OnLButtonDown(nFlags,Point);	

	return;
}

//�����Ϣ
VOID CGameFrameWindow::OnLButtonDblClk(UINT nFlags, CPoint Point)
{
	return __super::OnLButtonDblClk(nFlags,Point);
}

//���øı�
VOID CGameFrameWindow::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	__super::OnSettingChange(uFlags,lpszSection);
	return;
}

//������Ϣ
LRESULT	CGameFrameWindow::OnSetTextMesage(WPARAM wParam, LPARAM lParam)
{
	//Ĭ�ϵ���
	LRESULT lResult=DefWindowProc(WM_SETTEXT,wParam,lParam);

	//���±���
	Invalidate(TRUE);

	return lResult;
}

//��Ϸ����
bool CGameFrameWindow::ShowGameRule()
{
	return true;
}

//��󴰿�
bool CGameFrameWindow::MaxSizeWindow()
{	
	return true;
}
//��ԭ����
bool CGameFrameWindow::RestoreWindow()
{
	return true;
}

//��������
bool CGameFrameWindow::AllowGameSound(bool bAllowSound)
{
	return true;
}

//�Թۿ���
bool CGameFrameWindow::AllowGameLookon(DWORD dwUserID, bool bAllowLookon)
{
	return true;
}

//���ƽӿ�
bool CGameFrameWindow::OnGameOptionChange()
{
	return true;
}

//�����С
void CGameFrameWindow::ReSetDlgSize(int cx,int cy)
{
	m_CurWindowSize.SetSize(cx,cy);
	//����λ��
	CRect rcArce;
	SystemParametersInfo(SPI_GETWORKAREA,0,&rcArce,SPIF_SENDCHANGE);

	//��ȡλ��
	CSize SizeRestrict=m_CurWindowSize;
	//����λ��
	CRect rcNormalSize;			

	//λ�õ���
	SizeRestrict.cx=__min(rcArce.Width(),SizeRestrict.cx);
	SizeRestrict.cy=__min(rcArce.Height(),SizeRestrict.cy);

	//�ƶ�����
	rcNormalSize.top=(rcArce.Height()-SizeRestrict.cy)/2;
	rcNormalSize.left=(rcArce.Width()-SizeRestrict.cx)/2;
	rcNormalSize.right=rcNormalSize.left+SizeRestrict.cx;
	rcNormalSize.bottom=rcNormalSize.top+SizeRestrict.cy;

	m_CurWindowSize.SetSize(rcNormalSize.Width(),rcNormalSize.Height());
	SetWindowPos(NULL,rcNormalSize.left,rcNormalSize.top,rcNormalSize.Width(),rcNormalSize.Height(),SWP_NOZORDER);

	return;
}

//�������
UINT CGameFrameWindow::OnNcHitTest(CPoint point)
{
	CFrameWnd::OnNcHitTest(point);   
	return HTCAPTION;

}

// ���°�ť
VOID CGameFrameWindow::UpdateSound()
{
	CGlobalUnits *pGlobalUnits=CGlobalUnits::GetInstance();
	m_btSound.SetButtonImage(pGlobalUnits->m_bMuteStatuts ? IDB_BT_SOUND_CLOSE : IDB_BT_SOUND_OPEN, AfxGetInstanceHandle(), false, false);
}

//////////////////////////////////////////////////////////////////////////////////
void CGameFrameWindow::OnNcPaint()
{
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� __super::OnNcPaint()
}

void CGameFrameWindow::OnPaint()
{
	CPaintDC dc(this); // device context for painting	
	m_PngFrameTop.DrawImage(&dc,0,0);
	CString str;
	CRect rcTittle(10,7,1024,30);
	GetWindowText(str);

	dc.SetBkMode(TRANSPARENT);

	m_DFontEx.DrawText(&dc,str,&rcTittle,RGB(255,255,255),DT_VCENTER|DT_LEFT|DT_END_ELLIPSIS);
}
