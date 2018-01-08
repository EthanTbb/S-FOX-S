#include "Stdafx.h"
#include "Resource.h"
#include "DlgContact.h"

//////////////////////////////////////////////////////////////////////////////////
//�ؼ�����
const TCHAR* const szButtonCloseControlName = TEXT("ButtonClose");
const TCHAR* const szWebBrowserContactControlName = TEXT("WebBrowserContact");
//////////////////////////////////////////////////////////////////////////////////
//���캯��
CDlgContact::CDlgContact() : CFGuiDialog(IDD_DLG_POPUP)
{
}

//��������
CDlgContact::~CDlgContact()
{
}

//��������
BOOL CDlgContact::OnInitDialog()
{
	//���ñ���
	SetWindowText(TEXT("����ͷ�"));

	//���д���
	CenterWindow(this);

	return TRUE;
}

//ȷ������
VOID CDlgContact::OnOK()
{
	return;
}

//ȡ����Ϣ
VOID CDlgContact::OnCancel()
{
	return __super::OnCancel();
}

//��ʼ����
void CDlgContact::InitControlUI()
{
	//��ȡ����
	CGlobalWebLink * pGlobalWebLink=CGlobalWebLink::GetInstance();
	//�����ַ
	TCHAR szBillUrl[256]=TEXT("");
	_sntprintf(szBillUrl,CountArray(szBillUrl),TEXT("%s/ads/Contact.aspx"),pGlobalWebLink->GetPlatformLink());
	CWkeBrowserUI* pActiveXUI1 = static_cast<CWkeBrowserUI*>(GetControlByName(szWebBrowserContactControlName));
	if( pActiveXUI1 ) 
	{
		pActiveXUI1->Navigate(szBillUrl);
	}

}

//Ƥ������
LPCTSTR CDlgContact::GetSkinFile()
{
	return TEXT("DlgContact.xml"); 
}

//��Ϣ����
void CDlgContact::Notify(TNotifyUI &  msg)
{
	//��ȡ����
	CControlUI * pControlUI = msg.pSender;

	//����¼�
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