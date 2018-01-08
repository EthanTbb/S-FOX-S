#include "Stdafx.h"
#include "Resource.h"
#include "DlgRecharge.h"

//////////////////////////////////////////////////////////////////////////////////
//按钮控件
const TCHAR* const szButtonCloseControlName = TEXT("ButtonClose");
const TCHAR* const szWebBrowserRechargeControlName = TEXT("WebBrowserRecharge");

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CDlgRecharge::CDlgRecharge() : CFGuiDialog(IDD_DLG_POPUP)
{
}

//析构函数
CDlgRecharge::~CDlgRecharge()
{
}

//创建函数
BOOL CDlgRecharge::OnInitDialog()
{
	//设置标题
	SetWindowText(TEXT("充值中心"));

	//居中窗口
	CenterWindow(this);

	//下移距离
	CRect rc;
	GetWindowRect(&rc);
	rc.top += 30;
	rc.bottom += 30;
	MoveWindow(rc);

	return TRUE;
}

//确定函数
VOID CDlgRecharge::OnOK()
{
	return ;
}

//取消消息
VOID CDlgRecharge::OnCancel()
{
	return __super::OnCancel();
}

//初始函数
void CDlgRecharge::InitControlUI()
{
	//获取对象
	ASSERT(CGlobalWebLink::GetInstance()!=NULL);
	CGlobalWebLink * pGlobalWebLink=CGlobalWebLink::GetInstance();

	//构造地址
	TCHAR szBillUrl[256]=TEXT("");
	_sntprintf(szBillUrl,CountArray(szBillUrl),TEXT("%s/%sUserService/UserService2.aspx"),pGlobalWebLink->GetPlatformLink(),pGlobalWebLink->GetValidateLink());

	CWkeBrowserUI* pActiveXUI1 = static_cast<CWkeBrowserUI*>(GetControlByName(szWebBrowserRechargeControlName));
	if( pActiveXUI1 ) 
	{
		pActiveXUI1->Navigate(szBillUrl);
	}
}

//皮肤函数
LPCTSTR CDlgRecharge::GetSkinFile()
{
	return TEXT("DlgRecharge.xml"); 
}

//消息提醒
void CDlgRecharge::Notify(TNotifyUI &  msg)
{
	//获取对象
	CControlUI * pControlUI = msg.pSender;

	//点击事件
	if (lstrcmp(msg.sType, TEXT("click")) == 0)
	{
		if(lstrcmp(pControlUI->GetName(), szButtonCloseControlName)==0)
		{
			return OnCancel();
		}
	}
	return;
}

//////////////////////////////////////////////////////////////////////////////////