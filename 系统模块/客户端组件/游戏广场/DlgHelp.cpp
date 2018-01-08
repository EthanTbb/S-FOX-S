#include "Stdafx.h"
#include "Resource.h"
#include "DlgHelp.h"

//////////////////////////////////////////////////////////////////////////////////
//�ؼ�����
const TCHAR* const szButtonCloseControlName = TEXT("ButtonClose");
const TCHAR* const szButtonHelp1ControlName = TEXT("ButtonHelp1");
const TCHAR* const szButtonHelp2ControlName = TEXT("ButtonHelp2");
const TCHAR* const szButtonHelp3ControlName = TEXT("ButtonHelp3");
const TCHAR* const szButtonHelp4ControlName = TEXT("ButtonHelp4");
const TCHAR* const szButtonHelp5ControlName = TEXT("ButtonHelp5");
const TCHAR* const szButtonHelp6ControlName = TEXT("ButtonHelp6");
const TCHAR* const szButtonHelp7ControlName = TEXT("ButtonHelp7");
const TCHAR* const szButtonHelp8ControlName = TEXT("ButtonHelp8");
//////////////////////////////////////////////////////////////////////////////////

//���캯��
CDlgHelp::CDlgHelp() : CFGuiDialog(IDD_DLG_POPUP)
{
}

//��������
CDlgHelp::~CDlgHelp()
{
}

//��������
BOOL CDlgHelp::OnInitDialog()
{
	__super::OnInitDialog();

	//���ñ���
	SetWindowText(TEXT("��Ϸ����"));

	//���д���
	CenterWindow(this);

	return TRUE;
}

//ȷ������
VOID CDlgHelp::OnOK()
{

}

//ȡ����Ϣ
VOID CDlgHelp::OnCancel()
{
	return __super::OnCancel();
}

//��ʼ����
void CDlgHelp::InitControlUI()
{
}

//Ƥ������
LPCTSTR CDlgHelp::GetSkinFile()
{
	return TEXT("DlgHelp.xml"); 
}

//��Ϣ����
void CDlgHelp::Notify(TNotifyUI &  msg)
{
	//��ȡ����
	CControlUI * pControlUI = msg.pSender;
	if (lstrcmp(msg.sType, TEXT("click")) == 0)
	{
		if(lstrcmp(pControlUI->GetName(), szButtonCloseControlName)==0)
		{
			return OnCancel();
		}
		else if(
			lstrcmp(pControlUI->GetName(), szButtonHelp1ControlName)==0 ||
			lstrcmp(pControlUI->GetName(), szButtonHelp2ControlName)==0 ||
			lstrcmp(pControlUI->GetName(), szButtonHelp3ControlName)==0 ||
			lstrcmp(pControlUI->GetName(), szButtonHelp4ControlName)==0 ||
			lstrcmp(pControlUI->GetName(), szButtonHelp5ControlName)==0 ||
			lstrcmp(pControlUI->GetName(), szButtonHelp6ControlName)==0 ||
			lstrcmp(pControlUI->GetName(), szButtonHelp7ControlName)==0 ||
			lstrcmp(pControlUI->GetName(), szButtonHelp8ControlName)==0
			)
		{
			//��ȡ����
			ASSERT(CGlobalWebLink::GetInstance()!=NULL);
			CGlobalWebLink * pGlobalWebLink=CGlobalWebLink::GetInstance();
			//�����ַ
			TCHAR szNavigation[256]=TEXT("");
			_sntprintf(szNavigation,CountArray(szNavigation),TEXT("%s/PlatService/PlatService%d.aspx"),pGlobalWebLink->GetPlatformLink(),pControlUI->GetTag());
			//��ҳ��
			ShellExecute(NULL,TEXT("OPEN"),szNavigation,NULL,NULL,SW_NORMAL);
			return;
		}
	}
	return;
}
//////////////////////////////////////////////////////////////////////////////////