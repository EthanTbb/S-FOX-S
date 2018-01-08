#include "Stdafx.h"
#include "Resource.h"
#include "DlgFeedBack.h"
#include "DlgInformation.h"
//////////////////////////////////////////////////////////////////////////////////
//控件名字
const TCHAR* const szButtonCloseControlName = TEXT("ButtonClose");
const TCHAR* const szWebBrowserFeedBackControlName = TEXT("WebBrowserFeedBack");
//////////////////////////////////////////////////////////////////////////////////

//构造函数
CDlgFeedBack::CDlgFeedBack() : CFGuiDialog(IDD_DLG_POPUP)
{
}

//析构函数
CDlgFeedBack::~CDlgFeedBack()
{
}

//创建函数
BOOL CDlgFeedBack::OnInitDialog()
{
	//设置标题
	SetWindowText(TEXT("在线反馈"));

	//居中窗口
	CenterWindow(this);

	//Js调用
	CWkeBrowserUI::JSBindFunction("msgBox", js_msgBox, 1);

	return TRUE;
}

//确定函数
VOID CDlgFeedBack::OnOK()
{

}

//取消消息
VOID CDlgFeedBack::OnCancel()
{
	return __super::OnCancel();
}

//初始函数
void CDlgFeedBack::InitControlUI()
{
	//构造地址
	CGlobalWebLink * pGlobalWebLink=CGlobalWebLink::GetInstance();
	TCHAR szBillUrl[256]=TEXT("");
	_sntprintf(szBillUrl,CountArray(szBillUrl),TEXT("%s/%sUserService/Feedback.aspx"),pGlobalWebLink->GetPlatformLink(),pGlobalWebLink->GetValidateLink());
	CWkeBrowserUI* pActiveXUI1 = static_cast<CWkeBrowserUI*>(GetControlByName(szWebBrowserFeedBackControlName));
	if( pActiveXUI1 ) 
	{
		pActiveXUI1->Navigate(szBillUrl);
	}
}

//皮肤函数
LPCTSTR CDlgFeedBack::GetSkinFile()
{
	return TEXT("DlgFeedBack.xml"); 
}

//消息函数
void CDlgFeedBack::Notify(TNotifyUI &  msg)
{
	//获取对象
	CControlUI * pControlUI = msg.pSender;
	if (lstrcmp(msg.sType, TEXT("click")) == 0)
	{
		if(lstrcmp(pControlUI->GetName(), szButtonCloseControlName)==0)
		{
			return OnCancel();
		}
	}
	return;
}

//JS弹框
jsValue JS_CALL CDlgFeedBack::js_msgBox(jsExecState es)
{
	CDlgInformation Information;
	Information.ShowMessageBox(CWkeBrowserUI::JSBindGetArg(es, 0));
	return 0;
}

//////////////////////////////////////////////////////////////////////////////////