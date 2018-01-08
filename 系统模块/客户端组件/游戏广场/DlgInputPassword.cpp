#include "StdAfx.h"
#include "Resource.h"
#include "DlgInputPassword.h"
#include "DlgInformation.h"
//////////////////////////////////////////////////////////////////////////////////

//Բ�Ǵ�С
#define ROUND_CX					5									//Բ�ǿ��
#define ROUND_CY					5									//Բ�Ǹ߶�

//��Ļλ��
#define LAYERED_SIZE				4									//�ֲ��С
#define CAPTION_SIZE				15									//�����С

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CDlgInputPassword::CDlgInputPassword() : CFGuiDialog(IDD_DLG_POPUP)
{
	//����ؼ�
	m_pbtOk=NULL;
	m_pbtCancel=NULL;
	m_pEditPassword=NULL;

	//���ñ���
	ZeroMemory(m_szPassword,sizeof(m_szPassword));

	return;
}

//��������
CDlgInputPassword::~CDlgInputPassword()
{
}

LPCTSTR CDlgInputPassword::GetSkinFile()
{
	return TEXT("DlgInputPassword.xml");
}

//��������
BOOL CDlgInputPassword::OnInitDialog()
{
	__super::OnInitDialog();

	//���ñ���
	SetWindowText(TEXT("��������"));

	//���д���
	CenterWindow(this);

	return TRUE;
}

//ȷ������
VOID CDlgInputPassword::OnOK()
{
	//��ȡ��Ϣ
	lstrcpyn(m_szPassword,m_pEditPassword->GetText(),CountArray(m_szPassword));

	//�����ж�
	if (m_szPassword[0]==0)
	{
		//������ʾ
		CDlgInformation Information(this);
		Information.ShowMessageBox(TEXT("���벻��Ϊ�գ������뷿�����룡"),MB_ICONERROR);

		//���ý���
		m_pEditPassword->SetFocus();

		return;
	}

	__super::OnOK();
}

//ȡ����ť
VOID CDlgInputPassword::OnCancel()
{
	__super::OnCancel();
}

//��ʼ�ؼ�
void CDlgInputPassword::InitControlUI()
{
	__super::InitControlUI();

	//ȷ�ϰ�ť
	m_pbtOk = static_cast<CButtonUI*>(GetControlByName(TEXT("ButtonOk")));

	//�رհ�ť
	m_pbtCancel = static_cast<CButtonUI*>(GetControlByName(TEXT("ButtonCancle")));

	//��������
	m_pEditPassword = static_cast<CEditUI*>(GetControlByName(TEXT("EditPwd")));
	m_pEditPassword->SetMaxChar(LEN_PASSWORD-1);
	m_pEditPassword->SetBkColor(0);
	m_pEditPassword->SetPasswordMode(true);
	m_pEditPassword->SetFocus();
}

//��Ϣ����
void CDlgInputPassword::Notify(TNotifyUI &  msg)
{
	//��ȡ����
	CControlUI * pControlUI = msg.pSender;

	//����¼�
	if (lstrcmp(msg.sType, TEXT("click")) == 0)
	{
		if(pControlUI==m_pbtOk )
		{
			return OnOK(); 
		}
		else if(pControlUI==m_pbtCancel|| pControlUI->GetName()==TEXT("ButtonClose")) 
		{
			return OnCancel(); 
		}		
	}
}

//��ȡ����
VOID  CDlgInputPassword::GetServerPasswd(TCHAR szServerPasswd[LEN_PASSWORD])
{
	lstrcpyn(szServerPasswd,m_szPassword,CountArray(m_szPassword));

	return;
}

//////////////////////////////////////////////////////////////////////////////////
