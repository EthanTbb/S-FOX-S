#include "Stdafx.h"
#include "Resource.h"
#include "DlgRechargeEnquire.h"
#include "PlatformEvent.h"
//////////////////////////////////////////////////////////////////////////////////
//控件名字
const TCHAR* const szButtonCloseControlName = TEXT("ButtonClose");
const TCHAR* const szButtonOKControlName = TEXT("ButtonOK");

//文字信息
const TCHAR* const szTextTipControlName = TEXT("TextTip");
//////////////////////////////////////////////////////////////////////////////////

CDlgRechargeEnquire::CDlgRechargeEnquire(): CFGuiDialog(IDD_DLG_POPUP)
{

}

CDlgRechargeEnquire::~CDlgRechargeEnquire()
{

}

BOOL CDlgRechargeEnquire::OnInitDialog()
{
	//设置标题
	SetWindowText(TEXT("充值提示"));

	//居中窗口
	CenterWindow(this);

	UpdateUI();

	return TRUE;
}

VOID CDlgRechargeEnquire::OnOK()
{
	return __super::OnOK();
}

VOID CDlgRechargeEnquire::OnCancel()
{
	return __super::OnCancel();
}

void CDlgRechargeEnquire::InitControlUI()
{

}

LPCTSTR CDlgRechargeEnquire::GetSkinFile()
{
	return TEXT("DlgRechargeEnquire.xml");
}

void CDlgRechargeEnquire::Notify(TNotifyUI & msg)
{
	//获取对象
	CControlUI * pControlUI = msg.pSender;
	if (lstrcmp(msg.sType, TEXT("click")) == 0)
	{
		if(lstrcmp(pControlUI->GetName(), szButtonCloseControlName)==0)
		{
			return OnCancel();
		}
		else if(lstrcmp(pControlUI->GetName(), szButtonOKControlName)==0)
		{
			CPlatformEvent * pPlatformEvent=CPlatformEvent::GetInstance();
			if (pPlatformEvent!=NULL) pPlatformEvent->PostPlatformEvent(EVENT_USER_SHOW_RECHARGE,0L);
			return OnOK();
		}
	}
	return;
}

VOID CDlgRechargeEnquire::UpdateUI()
{
	CControlUI * pControlUI =NULL;
	//设置文字
	pControlUI=GetControlByName(szTextTipControlName);
	if(pControlUI!=NULL) pControlUI->SetText(m_strString);

}

//显示消息
INT CDlgRechargeEnquire::ShowMessageBox( LPCTSTR pszString)
{
	//设置变量
	m_strString=pszString;
	//显示窗口
	INT nResult=(INT)DoModal();

	return nResult;
}

//////////////////////////////////////////////////////////////////////////////////