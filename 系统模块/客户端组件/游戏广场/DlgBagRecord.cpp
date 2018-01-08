#include "Stdafx.h"
#include "Resource.h"
#include "DlgBagRecord.h"
#include "DlgInformation.h"
#include "PlatformEvent.h"
//////////////////////////////////////////////////////////////////////////////////
//��ť�ؼ�
const TCHAR* const szButtonCloseControlName = TEXT("ButtonClose");
const TCHAR* const szBagItemPropIOCControlName = TEXT("BagItemPropIOC");
const TCHAR* const szWebBrowserRecordControlName = TEXT("WebBrowserRecord");

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CDlgBagRecord::CDlgBagRecord() : CFGuiDialog(IDD_DLG_POPUP)
{

}

//��������
CDlgBagRecord::~CDlgBagRecord()
{
}

//��������
BOOL CDlgBagRecord::OnInitDialog()
{
	__super::OnInitDialog();

	//�޸���ʽ
	ModifyStyle(0, WS_MINIMIZEBOX|WS_MAXIMIZEBOX);

	//���ñ���
	SetWindowText(TEXT("�ҵı���"));

	//���д���
	CenterWindow(this);

	//���ƾ���
	CRect rc;
	GetWindowRect(&rc);
	rc.top += 30;
	rc.bottom += 30;
	//�ƶ�����
	MoveWindow(rc);


	return TRUE;
}

//ȷ������
VOID CDlgBagRecord::OnOK()
{

	//return __super::OnOK();
}

//ȡ����Ϣ
VOID CDlgBagRecord::OnCancel()
{
	return __super::OnCancel();
}



//��Ϣ����
void CDlgBagRecord::Notify(TNotifyUI &  msg)
{
	//��ȡ����
	CControlUI * pControlUI = msg.pSender;

	//����¼�
	if (lstrcmp(msg.sType, TEXT("click")) == 0)
	{
		if(lstrcmp(pControlUI->GetName(), szButtonCloseControlName)==0)
		{
			CPlatformEvent * pPlatformEvent=CPlatformEvent::GetInstance();
			if (pPlatformEvent!=NULL) pPlatformEvent->PostPlatformEvent(EVENT_USER_SHOW_BAG,0L);
			return OnCancel();
		}
	}
	
	return;
}

void CDlgBagRecord::InitControlUI()
{
	//��ȡ����
	ASSERT(CGlobalWebLink::GetInstance()!=NULL);
	CGlobalWebLink * pGlobalWebLink=CGlobalWebLink::GetInstance();
	//�����ַ
	TCHAR szBillUrl[256]=TEXT("");
	_sntprintf(szBillUrl,CountArray(szBillUrl),TEXT("%s/%sUserService/PropertyUseRecord.aspx"),pGlobalWebLink->GetPlatformLink(),pGlobalWebLink->GetValidateLink());

	CWkeBrowserUI* pActiveXUI1 = static_cast<CWkeBrowserUI*>(GetControlByName(szWebBrowserRecordControlName));
	if( pActiveXUI1 ) 
	{
		pActiveXUI1->Navigate(szBillUrl);
	}
}

LPCTSTR CDlgBagRecord::GetSkinFile()
{
	return TEXT("DlgBagRecord.xml"); 
}




//////////////////////////////////////////////////////////////////////////////////
