#include "Stdafx.h"
#include "Resource.h"
#include "DlgGame.h"
#include "DlgInformation.h"
#include "PlatformEvent.h"
//////////////////////////////////////////////////////////////////////////////////
//控件名字
const TCHAR* const szButtonCloseControlName = TEXT("ButtonClose");
const TCHAR* const szWebBrowserFeedBackControlName = TEXT("WebBrowserFeedBack");
const TCHAR* const szCheckButtonShowControlName = TEXT("CheckButtonShow");
//////////////////////////////////////////////////////////////////////////////////

//构造函数
CDlgGame::CDlgGame() : CFGuiDialog(IDD_DLG_POPUP)
{
}

//析构函数
CDlgGame::~CDlgGame()
{
}

//创建函数
BOOL CDlgGame::OnInitDialog()
{
	//设置标题
	SetWindowText(TEXT("游戏活动"));

	//居中窗口
	CenterWindow(this);


	return TRUE;
}

//确定函数
VOID CDlgGame::OnOK()
{

}

//取消消息
VOID CDlgGame::OnCancel()
{
	return __super::OnCancel();
}

//初始函数
void CDlgGame::InitControlUI()
{
	//构造地址
	CGlobalWebLink * pGlobalWebLink=CGlobalWebLink::GetInstance();
	TCHAR szBillUrl[256]=TEXT("");
	_sntprintf(szBillUrl,CountArray(szBillUrl),TEXT("%s/ads/AdsNotice.aspx"),pGlobalWebLink->GetPlatformLink());
	CWkeBrowserUI* pActiveXUI1 = static_cast<CWkeBrowserUI*>(GetControlByName(szWebBrowserFeedBackControlName));
	if( pActiveXUI1 ) 
	{
		pActiveXUI1->Navigate(szBillUrl);
	}
}

//皮肤函数
LPCTSTR CDlgGame::GetSkinFile()
{
	return TEXT("DlgGameAd.xml"); 
}

//消息函数
void CDlgGame::Notify(TNotifyUI &  msg)
{
	//获取对象
	CControlUI * pControlUI = msg.pSender;
	if (lstrcmp(msg.sType, TEXT("click")) == 0)
	{
		if(lstrcmp(pControlUI->GetName(), szButtonCloseControlName)==0)
		{
			//今日不再提示
			bool bShow = false;
			CCheckButtonUI * pCheckButtonPasswd=(CCheckButtonUI *)GetControlByName(szCheckButtonShowControlName);;
			if(pCheckButtonPasswd!=NULL)
			{
				bShow = pCheckButtonPasswd->GetCheck();
			}
			if (bShow)
			{
				//获取时间
				SYSTEMTIME SystemTime;
				GetLocalTime(&SystemTime); 
				WORD wDay=SystemTime.wDay;	
				WORD wMonth=SystemTime.wMonth;
				WORD wYear=SystemTime.wYear-1980;
				WORD wCurrDate = wYear<<16|wMonth<<8|wDay;

				CWHRegKey RegOptionItem;
				RegOptionItem.OpenRegKey(TEXT(""),true);
				RegOptionItem.WriteValue(TEXT("TodayShowDate"),wCurrDate);
			}

			CPlatformEvent * pPlatformEvent=CPlatformEvent::GetInstance();
			if (pPlatformEvent!=NULL) pPlatformEvent->PostPlatformEvent(EVENT_USER_CLOSE_GAME_AD,0L);

			return OnCancel();
		}
	}
	return;
}

//////////////////////////////////////////////////////////////////////////////////