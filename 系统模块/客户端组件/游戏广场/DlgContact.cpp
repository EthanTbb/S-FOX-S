#include "Stdafx.h"
#include "Resource.h"
#include "DlgContact.h"

//////////////////////////////////////////////////////////////////////////////////
//控件名字
const TCHAR* const szButtonCloseControlName = TEXT("ButtonClose");
const TCHAR* const szWebBrowserContactControlName = TEXT("WebBrowserContact");
//////////////////////////////////////////////////////////////////////////////////
//构造函数
CDlgContact::CDlgContact() : CFGuiDialog(IDD_DLG_POPUP)
{
}

//析构函数
CDlgContact::~CDlgContact()
{
}

//创建函数
BOOL CDlgContact::OnInitDialog()
{
	//设置标题
	SetWindowText(TEXT("联络客服"));

	//居中窗口
	CenterWindow(this);

	return TRUE;
}

//确定函数
VOID CDlgContact::OnOK()
{
	return;
}

//取消消息
VOID CDlgContact::OnCancel()
{
	return __super::OnCancel();
}

//初始函数
void CDlgContact::InitControlUI()
{
	//获取对象
	CGlobalWebLink * pGlobalWebLink=CGlobalWebLink::GetInstance();
	//构造地址
	TCHAR szBillUrl[256]=TEXT("");
	_sntprintf(szBillUrl,CountArray(szBillUrl),TEXT("%s/ads/Contact.aspx"),pGlobalWebLink->GetPlatformLink());
	CWkeBrowserUI* pActiveXUI1 = static_cast<CWkeBrowserUI*>(GetControlByName(szWebBrowserContactControlName));
	if( pActiveXUI1 ) 
	{
		pActiveXUI1->Navigate(szBillUrl);
	}

}

//皮肤函数
LPCTSTR CDlgContact::GetSkinFile()
{
	return TEXT("DlgContact.xml"); 
}

//消息函数
void CDlgContact::Notify(TNotifyUI &  msg)
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