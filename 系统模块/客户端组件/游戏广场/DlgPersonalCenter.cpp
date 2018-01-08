#include "Stdafx.h"
#include "Resource.h"
#include "DlgPersonalCenter.h"

//////////////////////////////////////////////////////////////////////////////////
//��ť�ؼ�
const TCHAR* const szButtonCloseControlName = TEXT("ButtonClose");
const TCHAR* const szButtonButtonHelpControlName = TEXT("ButtonButtonHelp");

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CDlgPersonalCenter::CDlgPersonalCenter() : CFGuiDialog(IDD_DLG_POPUP)
{
}

//��������
CDlgPersonalCenter::~CDlgPersonalCenter()
{
}

//��������
BOOL CDlgPersonalCenter::OnInitDialog()
{
	__super::OnInitDialog();

	//�޸���ʽ
	ModifyStyle(0, WS_MINIMIZEBOX|WS_MAXIMIZEBOX);

	//���ñ���
	SetWindowText(TEXT("��������"));

	//���д���
	CenterWindow(this);


	return TRUE;
}

//ȷ������
VOID CDlgPersonalCenter::OnOK()
{

	return __super::OnOK();
}

//ȡ����Ϣ
VOID CDlgPersonalCenter::OnCancel()
{
	return __super::OnCancel();
}

//��Ϣ����
void CDlgPersonalCenter::Notify(TNotifyUI &  msg)
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
	else if(msg.sType==_T("selectchanged"))
	{
		CTabLayoutUI* pControl = static_cast<CTabLayoutUI*>(GetControlByName(_T("TLayoutPersonal")));
		if(NULL != pControl) 
		{
			if (lstrcmp(pControlUI->GetName(), _T("OptionStandard")) == 0)
			{
				pControl->SelectItem(0);
			}
			else if (lstrcmp(pControlUI->GetName(), _T("OptionRealInfo")) == 0)
			{
				pControl->SelectItem(1);
			}
			else if (lstrcmp(pControlUI->GetName(), _T("OptionChangePW")) == 0)
			{
				pControl->SelectItem(2);
			}
		}
	}

	
	return;
}

//////////////////////////////////////////////////////////////////////////////////