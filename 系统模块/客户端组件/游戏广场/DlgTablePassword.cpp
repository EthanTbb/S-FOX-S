#include "Stdafx.h"
#include "Resource.h"
#include "DlgTablePassword.h"
#include "DlgInformation.h"

//////////////////////////////////////////////////////////////////////////////////
//�ؼ�����
const TCHAR* const szButtonCloseControlName = TEXT("ButtonClose");
const TCHAR* const szButtonCancleControlName = TEXT("ButtonCancle");
const TCHAR* const szButtonOkControlName = TEXT("ButtonOk");
//////////////////////////////////////////////////////////////////////////

//���캯��
CDlgTablePassword::CDlgTablePassword() :  CFGuiDialog(IDD_DLG_POPUP)
{
}

//��������
CDlgTablePassword::~CDlgTablePassword()
{
}

//��������
BOOL CDlgTablePassword::OnInitDialog()
{
	SetWindowText(TEXT("��������"));

	//���д���
	CenterWindow(this);

	return TRUE;
}

//ȷ������
VOID CDlgTablePassword::OnOK()
{
	CEditUI* editPassword = (CEditUI*)GetControlByName(TEXT("EditPwd"));
	if (editPassword!=NULL)
	{
        ZeroMemory(m_szPassword,sizeof(m_szPassword));
		//��ȡ����	
		lstrcpyn(m_szPassword,editPassword->GetText(),CountArray(m_szPassword));
		if (m_szPassword[0]==0)
		{
			//��ʾ��Ϣ
			CDlgInformation Information(this);
			Information.ShowMessageBox(TEXT("��Ϸ�����벻��Ϊ�գ����������룡"),MB_ICONINFORMATION);
			//���ý���
			editPassword->SetFocus();
			return;
		}
	}
	__super::OnOK();
}

//ȡ����Ϣ
VOID CDlgTablePassword::OnCancel()
{
	return __super::OnCancel();
}

//��ʼ����
VOID CDlgTablePassword::InitControlUI()
{
}

//Ƥ������
LPCTSTR CDlgTablePassword::GetSkinFile()
{
	return TEXT("DlgTablePassword.xml");
}

//��Ϣ����
VOID CDlgTablePassword::Notify(TNotifyUI &  msg)
{
	//��ȡ����
	CControlUI * pControlUI = msg.pSender;

	if (lstrcmp(msg.sType, TEXT("click")) == 0)
	{
		if(lstrcmp(pControlUI->GetName(), szButtonCloseControlName)==0)
		{
			return OnCancel();
		}
		else if (lstrcmp(pControlUI->GetName(), szButtonCancleControlName)==0)
		{
			return OnCancel();
		}
		else if (lstrcmp(pControlUI->GetName(), szButtonOkControlName)==0)
		{
			return OnOK();
		}
	}
	return;
}

//////////////////////////////////////////////////////////////////////////