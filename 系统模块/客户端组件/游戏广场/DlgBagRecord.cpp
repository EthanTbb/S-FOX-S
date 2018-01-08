#include "Stdafx.h"
#include "Resource.h"
#include "DlgBagRecord.h"
#include "DlgInformation.h"
#include "PlatformEvent.h"
//////////////////////////////////////////////////////////////////////////////////
//按钮控件
const TCHAR* const szButtonCloseControlName = TEXT("ButtonClose");
const TCHAR* const szBagItemPropIOCControlName = TEXT("BagItemPropIOC");
const TCHAR* const szWebBrowserRecordControlName = TEXT("WebBrowserRecord");

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CDlgBagRecord::CDlgBagRecord() : CFGuiDialog(IDD_DLG_POPUP)
{

}

//析构函数
CDlgBagRecord::~CDlgBagRecord()
{
}

//创建函数
BOOL CDlgBagRecord::OnInitDialog()
{
	__super::OnInitDialog();

	//修改样式
	ModifyStyle(0, WS_MINIMIZEBOX|WS_MAXIMIZEBOX);

	//设置标题
	SetWindowText(TEXT("我的背包"));

	//居中窗口
	CenterWindow(this);

	//下移距离
	CRect rc;
	GetWindowRect(&rc);
	rc.top += 30;
	rc.bottom += 30;
	//移动窗口
	MoveWindow(rc);


	return TRUE;
}

//确定函数
VOID CDlgBagRecord::OnOK()
{

	//return __super::OnOK();
}

//取消消息
VOID CDlgBagRecord::OnCancel()
{
	return __super::OnCancel();
}



//消息提醒
void CDlgBagRecord::Notify(TNotifyUI &  msg)
{
	//获取对象
	CControlUI * pControlUI = msg.pSender;

	//点击事件
	if (lstrcmp(msg.sType, TEXT("click")) == 0)
	{
		if(lstrcmp(pControlUI->GetName(), szButtonCloseControlName)==0)
		{
			CPlatformEvent * pPlatformEvent=CPlatformEvent::GetInstance();
			if (pPlatformEvent!=NULL) pPlatformEvent->PostPlatformEvent(EVENT_USER_SHOW_BAG,0L);
			return OnCancel();
		}
	}
	
	return;
}

void CDlgBagRecord::InitControlUI()
{
	//获取对象
	ASSERT(CGlobalWebLink::GetInstance()!=NULL);
	CGlobalWebLink * pGlobalWebLink=CGlobalWebLink::GetInstance();
	//构造地址
	TCHAR szBillUrl[256]=TEXT("");
	_sntprintf(szBillUrl,CountArray(szBillUrl),TEXT("%s/%sUserService/PropertyUseRecord.aspx"),pGlobalWebLink->GetPlatformLink(),pGlobalWebLink->GetValidateLink());

	CWkeBrowserUI* pActiveXUI1 = static_cast<CWkeBrowserUI*>(GetControlByName(szWebBrowserRecordControlName));
	if( pActiveXUI1 ) 
	{
		pActiveXUI1->Navigate(szBillUrl);
	}
}

LPCTSTR CDlgBagRecord::GetSkinFile()
{
	return TEXT("DlgBagRecord.xml"); 
}




//////////////////////////////////////////////////////////////////////////////////
