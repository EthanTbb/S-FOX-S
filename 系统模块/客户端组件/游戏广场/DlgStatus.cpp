#include "StdAfx.h"
#include "DlgStatus.h"
#include "PlatformFrame.h"
#include "PlazaViewServer.h"

//////////////////////////////////////////////////////////////////////////////////

//圆角大小
#define ROUND_CX					5									//圆角宽度
#define ROUND_CY					5									//圆角高度

//时间标识
#define IDT_STATUS					10									//状态时间
#define TIMER_STATUS				20									//状态时间


const TCHAR* const szStatusProgressControlName = TEXT("StatusProgress");
const TCHAR* const szStatusLableControlName = TEXT("LableStatus");

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CDlgStatus, CFGuiDialog)

	//系统消息
	ON_WM_TIMER()

END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CDlgStatus::CDlgStatus() : CFGuiDialog(IDD_DLG_POPUP)
{
	//状态变量
	m_uImageIndex=0;
	m_bFinished = false;

	//控件变量
	m_pbtGiveUp=NULL;

	//接口变量
	m_pIStatusViewSink=NULL;

	return;
}

//析构函数
CDlgStatus::~CDlgStatus()
{
}

//消息解释
BOOL CDlgStatus::PreTranslateMessage(MSG * pMsg)
{
	//按键过虑
	if ((pMsg->message==WM_KEYDOWN)&&((pMsg->wParam==VK_ESCAPE)||(pMsg->wParam==VK_RETURN)))
	{
		return TRUE;
	}

	return __super::PreTranslateMessage(pMsg);
}

//配置函数
BOOL CDlgStatus::OnInitDialog()
{
	__super::OnInitDialog();

	//设置标题
	SetWindowText(szProduct);

	
	//居中窗口
	CenterWindow(CPlatformFrame::GetInstance());
	//设置控件
	SetTimer(IDT_STATUS,TIMER_STATUS,NULL);

	return TRUE;
}

//初始控件
void CDlgStatus::InitControlUI()
{
	__super::InitControlUI();
	return;
}

//消息提醒
void CDlgStatus::Notify(TNotifyUI &  msg)
{
	//获取对象
	CControlUI * pControlUI = msg.pSender;

	//点击事件
	if (lstrcmp(msg.sType, TEXT("click")) == 0)
	{
		if(m_pbtGiveUp==pControlUI) 
		{
			return OnBnClickedGiveUp(); 
		}
	}
}

//皮肤名称
LPCTSTR CDlgStatus::GetSkinFile()
{
	return TEXT("DlgStatus.xml");
}

//开始绘画
void CDlgStatus::OnBeginPaintWindow(HDC hDC)
{
	return;
}

//关闭窗口
VOID CDlgStatus::HideStatusWindow()
{
	//关闭窗口
	CProgressUI *pCProgressUI =  (CProgressUI * )GetControlByName(szStatusProgressControlName);
	if(NULL != pCProgressUI)
	{
		pCProgressUI->SetValue(pCProgressUI->GetMaxValue());
	}

	//置顶窗口
	//SetActiveWindow();
	//BringWindowToTop();
	SetForegroundWindow();

	m_bFinished = true;
	return;
}

//设置消息
VOID CDlgStatus::ShowStatusWindow(LPCTSTR pszDescribe)
{
	//创建窗口
	if (m_hWnd==NULL)
	{
		if (CPlatformFrame::GetInstance()->IsWindowVisible()==FALSE)
		{
			Create(IDD_DLG_POPUP,GetDesktopWindow());
		}
		else
		{
			Create(IDD_DLG_POPUP,CPlatformFrame::GetInstance());
		}
	}

	//设置描述
	m_strDescribe=pszDescribe;

	CLabelUI *pCProgressUI =  (CLabelUI *)GetControlByName(szStatusLableControlName);
	if(NULL != pCProgressUI)
	{
		pCProgressUI->SetText(m_strDescribe);
	}

	//显示窗口
	ShowWindow(SW_SHOW);

	//屏幕居中
	CRect rtDesk;  
	CRect rtDlg;  
	::GetWindowRect(::GetDesktopWindow(), &rtDesk);  
	GetWindowRect(&rtDlg);  
	int iXpos = rtDesk.Width() / 2 - rtDlg.Width() / 2;  
	int iYpos = rtDesk.Height() / 2 - rtDlg.Height() / 2;  
	SetWindowPos(NULL, iXpos, iYpos, 0, 0, SWP_NOOWNERZORDER|SWP_NOSIZE|SWP_NOZORDER);

	return;
}

//设置接口
VOID CDlgStatus::SetStatusViewSink(IStatusViewSink * pIStatusViewSink)
{
	//设置变量
	m_pIStatusViewSink=pIStatusViewSink;

	return;
}

//时间消息
VOID CDlgStatus::OnTimer(UINT nIDEvent)
{
	//设置进度
	if (nIDEvent==IDT_STATUS)
	{	
		if(true == m_bFinished)
		{
			m_bFinished = false;
			if (m_hWnd!=NULL) ShowWindow(SW_HIDE);
			return;
		}

		CProgressUI *pCProgressUI =  (CProgressUI * )GetControlByName(szStatusProgressControlName);
		if(NULL != pCProgressUI)
		{
			pCProgressUI->SetValue(m_uImageIndex);
		}

		//设置索引
		m_uImageIndex=(m_uImageIndex>=100)?0:(m_uImageIndex+1L);

		return;
	}

	__super::OnTimer(nIDEvent);
}

//取消按钮
VOID CDlgStatus::OnBnClickedGiveUp()
{
	//隐藏界面
	HideStatusWindow();

	//投递消息
	ASSERT(m_pIStatusViewSink!=NULL);
	if (m_pIStatusViewSink!=NULL) m_pIStatusViewSink->OnStatusCancel();

	return;
}



//////////////////////////////////////////////////////////////////////////////////
