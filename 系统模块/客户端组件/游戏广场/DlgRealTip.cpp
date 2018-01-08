#include "Stdafx.h"
#include "Resource.h"
#include "DlgRealTip.h"
#include "PlatformEvent.h"

//////////////////////////////////////////////////////////////////////////////////
//按钮控件
const TCHAR* const szButtonCloseControlName = TEXT("ButtonClose");
const TCHAR* const szButtonGoRealControlName = TEXT("ButtonGoReal");
const TCHAR* const szButtonWaitMomentControlName = TEXT("ButtonWaitMoment");
//////////////////////////////////////////////////////////////////////////////////

//构造函数
CDlgRealTip::CDlgRealTip() : CFGuiDialog(IDD_DLG_POPUP)
{
}

//析构函数
CDlgRealTip::~CDlgRealTip()
{
}

//创建函数
BOOL CDlgRealTip::OnInitDialog()
{
	//设置标题
	SetWindowText(TEXT("实名提示"));

	//居中窗口
	CenterWindow(this);

	return TRUE;
}

//确定函数
VOID CDlgRealTip::OnOK()
{
	return ;
}

//取消消息
VOID CDlgRealTip::OnCancel()
{
	return __super::OnCancel();
}

//初始函数
void CDlgRealTip::InitControlUI()
{
}

//皮肤函数
LPCTSTR CDlgRealTip::GetSkinFile()
{
	return TEXT("DlgRealTip.xml"); 
}

//消息提醒
void CDlgRealTip::Notify(TNotifyUI &  msg)
{
	//获取对象
	CControlUI * pControlUI = msg.pSender;

	//点击事件
	if (lstrcmp(msg.sType, TEXT("click")) == 0)
	{
		//取消
		if( (lstrcmp(pControlUI->GetName(), szButtonCloseControlName)==0) || 
			(lstrcmp(pControlUI->GetName(), szButtonWaitMomentControlName)==0) )
		{
			return OnCancel();
		}
		else if(lstrcmp(pControlUI->GetName(), szButtonGoRealControlName)==0)
		{
			//发送事件
			CPlatformEvent * pPlatformEvent=CPlatformEvent::GetInstance();
			if (pPlatformEvent!=NULL) pPlatformEvent->PostPlatformEvent(EVENT_USER_SHOW_PERSONER,1L);
			return OnCancel();
		}
	}
	return;
}

//////////////////////////////////////////////////////////////////////////////////