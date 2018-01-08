#include "Stdafx.h"
#include "Resource.h"
#include "DlgRecharge.h"

//////////////////////////////////////////////////////////////////////////////////
//��ť�ؼ�
const TCHAR* const szButtonCloseControlName = TEXT("ButtonClose");
const TCHAR* const szWebBrowserRechargeControlName = TEXT("WebBrowserRecharge");

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CDlgRecharge::CDlgRecharge() : CFGuiDialog(IDD_DLG_POPUP)
{
}

//��������
CDlgRecharge::~CDlgRecharge()
{
}

//��������
BOOL CDlgRecharge::OnInitDialog()
{
	//���ñ���
	SetWindowText(TEXT("��ֵ����"));

	//���д���
	CenterWindow(this);

	//���ƾ���
	CRect rc;
	GetWindowRect(&rc);
	rc.top += 30;
	rc.bottom += 30;
	MoveWindow(rc);

	return TRUE;
}

//ȷ������
VOID CDlgRecharge::OnOK()
{
	return ;
}

//ȡ����Ϣ
VOID CDlgRecharge::OnCancel()
{
	return __super::OnCancel();
}

//��ʼ����
void CDlgRecharge::InitControlUI()
{
	//��ȡ����
	ASSERT(CGlobalWebLink::GetInstance()!=NULL);
	CGlobalWebLink * pGlobalWebLink=CGlobalWebLink::GetInstance();

	//�����ַ
	TCHAR szBillUrl[256]=TEXT("");
	_sntprintf(szBillUrl,CountArray(szBillUrl),TEXT("%s/%sUserService/UserService2.aspx"),pGlobalWebLink->GetPlatformLink(),pGlobalWebLink->GetValidateLink());

	CWkeBrowserUI* pActiveXUI1 = static_cast<CWkeBrowserUI*>(GetControlByName(szWebBrowserRechargeControlName));
	if( pActiveXUI1 ) 
	{
		pActiveXUI1->Navigate(szBillUrl);
	}
}

//Ƥ������
LPCTSTR CDlgRecharge::GetSkinFile()
{
	return TEXT("DlgRecharge.xml"); 
}

//��Ϣ����
void CDlgRecharge::Notify(TNotifyUI &  msg)
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