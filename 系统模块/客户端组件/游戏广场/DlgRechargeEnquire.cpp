#include "Stdafx.h"
#include "Resource.h"
#include "DlgRechargeEnquire.h"
#include "PlatformEvent.h"
//////////////////////////////////////////////////////////////////////////////////
//�ؼ�����
const TCHAR* const szButtonCloseControlName = TEXT("ButtonClose");
const TCHAR* const szButtonOKControlName = TEXT("ButtonOK");

//������Ϣ
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
	//���ñ���
	SetWindowText(TEXT("��ֵ��ʾ"));

	//���д���
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
	//��ȡ����
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
	//��������
	pControlUI=GetControlByName(szTextTipControlName);
	if(pControlUI!=NULL) pControlUI->SetText(m_strString);

}

//��ʾ��Ϣ
INT CDlgRechargeEnquire::ShowMessageBox( LPCTSTR pszString)
{
	//���ñ���
	m_strString=pszString;
	//��ʾ����
	INT nResult=(INT)DoModal();

	return nResult;
}

//////////////////////////////////////////////////////////////////////////////////