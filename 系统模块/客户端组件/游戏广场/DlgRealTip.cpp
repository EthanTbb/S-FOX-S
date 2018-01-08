#include "Stdafx.h"
#include "Resource.h"
#include "DlgRealTip.h"
#include "PlatformEvent.h"

//////////////////////////////////////////////////////////////////////////////////
//��ť�ؼ�
const TCHAR* const szButtonCloseControlName = TEXT("ButtonClose");
const TCHAR* const szButtonGoRealControlName = TEXT("ButtonGoReal");
const TCHAR* const szButtonWaitMomentControlName = TEXT("ButtonWaitMoment");
//////////////////////////////////////////////////////////////////////////////////

//���캯��
CDlgRealTip::CDlgRealTip() : CFGuiDialog(IDD_DLG_POPUP)
{
}

//��������
CDlgRealTip::~CDlgRealTip()
{
}

//��������
BOOL CDlgRealTip::OnInitDialog()
{
	//���ñ���
	SetWindowText(TEXT("ʵ����ʾ"));

	//���д���
	CenterWindow(this);

	return TRUE;
}

//ȷ������
VOID CDlgRealTip::OnOK()
{
	return ;
}

//ȡ����Ϣ
VOID CDlgRealTip::OnCancel()
{
	return __super::OnCancel();
}

//��ʼ����
void CDlgRealTip::InitControlUI()
{
}

//Ƥ������
LPCTSTR CDlgRealTip::GetSkinFile()
{
	return TEXT("DlgRealTip.xml"); 
}

//��Ϣ����
void CDlgRealTip::Notify(TNotifyUI &  msg)
{
	//��ȡ����
	CControlUI * pControlUI = msg.pSender;

	//����¼�
	if (lstrcmp(msg.sType, TEXT("click")) == 0)
	{
		//ȡ��
		if( (lstrcmp(pControlUI->GetName(), szButtonCloseControlName)==0) || 
			(lstrcmp(pControlUI->GetName(), szButtonWaitMomentControlName)==0) )
		{
			return OnCancel();
		}
		else if(lstrcmp(pControlUI->GetName(), szButtonGoRealControlName)==0)
		{
			//�����¼�
			CPlatformEvent * pPlatformEvent=CPlatformEvent::GetInstance();
			if (pPlatformEvent!=NULL) pPlatformEvent->PostPlatformEvent(EVENT_USER_SHOW_PERSONER,1L);
			return OnCancel();
		}
	}
	return;
}

//////////////////////////////////////////////////////////////////////////////////