#include "Stdafx.h"
#include "Resource.h"
#include "DlgInsureOpen.h"
#include "DlgInsure.h"

//////////////////////////////////////////////////////////////////////////////////
//��ť�ؼ�
const TCHAR* const szButtonCloseControlName = TEXT("ButtonClose");
const TCHAR* const szButtonCancleControlName = TEXT("ButtonCancle");
const TCHAR* const szButtonComfirmControlName = TEXT("ButtonComfirm");



//////////////////////////////////////////////////////////////////////////////////

//���캯��
CDlgInsureOpen::CDlgInsureOpen() : CFGuiDialog(IDD_DLG_POPUP)
{
}

//��������
CDlgInsureOpen::~CDlgInsureOpen()
{
}

//��������
BOOL CDlgInsureOpen::OnInitDialog()
{
	__super::OnInitDialog();
	
	//�޸���ʽ
	ModifyStyle(0, WS_MINIMIZEBOX|WS_MAXIMIZEBOX);

	//���д���
	CenterWindow(this);


	return TRUE;
}

//ȷ������
VOID CDlgInsureOpen::OnOK()
{
	return __super::OnOK();
}

//ȡ����Ϣ
VOID CDlgInsureOpen::OnCancel()
{
	return __super::OnCancel();
}


//��Ϣ����
void CDlgInsureOpen::Notify(TNotifyUI &  msg)
{
	//��ȡ����
	CControlUI * pControlUI = msg.pSender;

	//����¼�
	if (lstrcmp(msg.sType, TEXT("click")) == 0)
	{
		if((lstrcmp(pControlUI->GetName(), szButtonCloseControlName)==0) || 
			(lstrcmp(pControlUI->GetName(), szButtonCancleControlName)==0))
		{
			OnCancel();
		}
		else if(lstrcmp(pControlUI->GetName(), szButtonComfirmControlName)==0)
		{
			OnOK();
		}
	}
	
	return;
}

LPCTSTR CDlgInsureOpen::GetSkinFile()
{
	return TEXT("DlgInsureOpen.xml"); 
}


//////////////////////////////////////////////////////////////////////////////////