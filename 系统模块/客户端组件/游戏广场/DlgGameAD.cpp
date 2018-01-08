#include "Stdafx.h"
#include "Resource.h"
#include "DlgGame.h"
#include "DlgInformation.h"
#include "PlatformEvent.h"
//////////////////////////////////////////////////////////////////////////////////
//控件名字
const TCHAR* const szButtonCloseControlName = TEXT("ButtonClose");
const TCHAR* const szWebBrowserFeedBackControlName = TEXT("WebBrowserFeedBack");
//////////////////////////////////////////////////////////////////////////////////

//构造函数
CDlgGameAD::CDlgGameAD() : CFGuiDialog(IDD_DLG_POPUP)
{
}

//析构函数
CDlgGameAD::~CDlgGameAD()
{
}

//创建函数
BOOL CDlgGameAD::OnInitDialog()
{
	//设置标题
	SetWindowText(TEXT("游戏活动"));

	//居中窗口
	CenterWindow(this);


	return TRUE;
}

//确定函数
VOID CDlgGameAD::OnOK()
{

}

//取消消息
VOID CDlgGameAD::OnCancel()
{
	return __super::OnCancel();
}

//初始函数
void CDlgGameAD::InitControlUI()
{
	//构造地址
	CGlobalWebLink * pGlobalWebLink=CGlobalWebLink::GetInstance();
	TCHAR szBillUrl[256]=TEXT("");
	_sntprintf(szBillUrl,CountArray(szBillUrl),TEXT("%s/ads/AlertImage.aspx"),pGlobalWebLink->GetPlatformLink());
	CWkeBrowserUI* pActiveXUI1 = static_cast<CWkeBrowserUI*>(GetControlByName(szWebBrowserFeedBackControlName));
	if( pActiveXUI1 ) 
	{
		pActiveXUI1->Navigate(szBillUrl);
	}
}

//皮肤函数
LPCTSTR CDlgGameAD::GetSkinFile()
{
	return TEXT("DlgGameAd.xml"); 
}

//消息函数
void CDlgGameAD::Notify(TNotifyUI &  msg)
{
	//获取对象
	CControlUI * pControlUI = msg.pSender;
	if (lstrcmp(msg.sType, TEXT("click")) == 0)
	{
		if(lstrcmp(pControlUI->GetName(), szButtonCloseControlName)==0)
		{
			CPlatformEvent * pPlatformEvent=CPlatformEvent::GetInstance();
			if (pPlatformEvent!=NULL) pPlatformEvent->PostPlatformEvent(EVENT_USER_CLOSE_GAME_AD,0L);

			return OnCancel();
		}
	}
	return;
}

//////////////////////////////////////////////////////////////////////////////////