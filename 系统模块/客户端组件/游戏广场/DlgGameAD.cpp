#include "Stdafx.h"
#include "Resource.h"
#include "DlgGame.h"
#include "DlgInformation.h"
#include "PlatformEvent.h"
//////////////////////////////////////////////////////////////////////////////////
//�ؼ�����
const TCHAR* const szButtonCloseControlName = TEXT("ButtonClose");
const TCHAR* const szWebBrowserFeedBackControlName = TEXT("WebBrowserFeedBack");
//////////////////////////////////////////////////////////////////////////////////

//���캯��
CDlgGameAD::CDlgGameAD() : CFGuiDialog(IDD_DLG_POPUP)
{
}

//��������
CDlgGameAD::~CDlgGameAD()
{
}

//��������
BOOL CDlgGameAD::OnInitDialog()
{
	//���ñ���
	SetWindowText(TEXT("��Ϸ�"));

	//���д���
	CenterWindow(this);


	return TRUE;
}

//ȷ������
VOID CDlgGameAD::OnOK()
{

}

//ȡ����Ϣ
VOID CDlgGameAD::OnCancel()
{
	return __super::OnCancel();
}

//��ʼ����
void CDlgGameAD::InitControlUI()
{
	//�����ַ
	CGlobalWebLink * pGlobalWebLink=CGlobalWebLink::GetInstance();
	TCHAR szBillUrl[256]=TEXT("");
	_sntprintf(szBillUrl,CountArray(szBillUrl),TEXT("%s/ads/AlertImage.aspx"),pGlobalWebLink->GetPlatformLink());
	CWkeBrowserUI* pActiveXUI1 = static_cast<CWkeBrowserUI*>(GetControlByName(szWebBrowserFeedBackControlName));
	if( pActiveXUI1 ) 
	{
		pActiveXUI1->Navigate(szBillUrl);
	}
}

//Ƥ������
LPCTSTR CDlgGameAD::GetSkinFile()
{
	return TEXT("DlgGameAd.xml"); 
}

//��Ϣ����
void CDlgGameAD::Notify(TNotifyUI &  msg)
{
	//��ȡ����
	CControlUI * pControlUI = msg.pSender;
	if (lstrcmp(msg.sType, TEXT("click")) == 0)
	{
		if(lstrcmp(pControlUI->GetName(), szButtonCloseControlName)==0)
		{
			CPlatformEvent * pPlatformEvent=CPlatformEvent::GetInstance();
			if (pPlatformEvent!=NULL) pPlatformEvent->PostPlatformEvent(EVENT_USER_CLOSE_GAME_AD,0L);

			return OnCancel();
		}
	}
	return;
}

//////////////////////////////////////////////////////////////////////////////////