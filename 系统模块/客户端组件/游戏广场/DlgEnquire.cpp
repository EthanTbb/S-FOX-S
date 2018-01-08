#include "Stdafx.h"
#include "DlgEnquire.h"
#include "GlobalUnits.h"
#include "PlatformFrame.h"

//////////////////////////////////////////////////////////////////////////////////
//复选控件
const TCHAR* const szCheckButtonSwitchControlName = TEXT("ButtonSwitch");
const TCHAR* const szCheckButtonQuitRoomControlName = TEXT("ButtonQuitRoom");
const TCHAR* const szCheckButtonQuitPlazaControlName = TEXT("ButtonQuitPlaza");
const TCHAR* const szCheckButtonCloseControlName = TEXT("ButtonClose");
const TCHAR* const szWebBrowserClosePlazaControlName = TEXT("WebBrowserClosePlaza");

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CDlgEnquire::CDlgEnquire() : CFGuiDialog(IDD_DLG_POPUP)
{
}

//析构函数
CDlgEnquire::~CDlgEnquire()
{
}

//创建函数
BOOL CDlgEnquire::OnInitDialog()
{
	//设置标题
	SetWindowText(TEXT("退出提示"));

	//居中窗口
	CenterWindow(this);

	return TRUE;
}

//确定函数
VOID CDlgEnquire::OnOK()
{
	return;
}

//取消消息
VOID CDlgEnquire::OnCancel()
{
	return __super::OnCancel();
}

//初始控件
void CDlgEnquire::InitControlUI()
{
	CButtonUI *pButtonUI = static_cast<CButtonUI *>(GetControlByName(szCheckButtonQuitRoomControlName));
	if(pButtonUI == NULL) return;
	//获取对象
	CPlatformFrame * pPlatformFrame = CPlatformFrame::GetInstance();
	pButtonUI->SetEnabled(pPlatformFrame->IsEnteredServerItem());

	//获取对象
	CGlobalWebLink * pGlobalWebLink=CGlobalWebLink::GetInstance();

	//构造地址
	TCHAR szBillUrl[256]=TEXT("");
	_sntprintf(szBillUrl,CountArray(szBillUrl),TEXT("%s/ads/ClosePlaza.aspx"),pGlobalWebLink->GetPlatformLink());

	CWkeBrowserUI* pActiveXUI1 = static_cast<CWkeBrowserUI*>(GetControlByName(szWebBrowserClosePlazaControlName));
	if( pActiveXUI1 ) 
	{
		pActiveXUI1->Navigate(szBillUrl);
	}

	return;
}

//消息提醒
void CDlgEnquire::Notify(TNotifyUI &  msg)
{
	__super::Notify(msg);

	//获取对象
	CControlUI * pControlUI = msg.pSender;

	if (lstrcmp(msg.sType, TEXT("click")) == 0)
	{
		if(lstrcmp(pControlUI->GetName(), szCheckButtonSwitchControlName) == 0)
		{
			return OnBnClickedSwitchAccounts();
		}
		else if(lstrcmp(pControlUI->GetName(), szCheckButtonQuitRoomControlName) == 0)
		{
			return OnBnClickedCloseServer();
		}
		else if(lstrcmp(pControlUI->GetName(), szCheckButtonQuitPlazaControlName) == 0)
		{
			return OnBnClickedClosePlaza();
		}
		else if(lstrcmp(pControlUI->GetName(), szCheckButtonCloseControlName) == 0)
		{
			return OnCancel();
		}
	}
}

//皮肤名称
LPCTSTR CDlgEnquire::GetSkinFile()
{
	return TEXT("DlgQuit.xml");
}

//关闭大厅
VOID CDlgEnquire::OnBnClickedClosePlaza()
{
	EndDialog(IDC_CLOSE_PLAZA);

	return;
}

//关闭房间
VOID CDlgEnquire::OnBnClickedCloseServer()
{
	EndDialog(IDC_CLOSE_SERVER);

	return;
}

//切换帐号
VOID CDlgEnquire::OnBnClickedSwitchAccounts()
{
	EndDialog(IDC_SWITCH_ACCOUNTS);

	return;
}

//////////////////////////////////////////////////////////////////////////////////
