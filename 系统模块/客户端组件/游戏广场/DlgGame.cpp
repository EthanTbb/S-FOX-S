#include "Stdafx.h"
#include "Resource.h"
#include "DlgGame.h"
#include "DlgInformation.h"
#include "PlatformEvent.h"
//////////////////////////////////////////////////////////////////////////////////
//�ؼ�����
const TCHAR* const szButtonCloseControlName = TEXT("ButtonClose");
const TCHAR* const szWebBrowserFeedBackControlName = TEXT("WebBrowserFeedBack");
const TCHAR* const szCheckButtonShowControlName = TEXT("CheckButtonShow");
//////////////////////////////////////////////////////////////////////////////////

//���캯��
CDlgGame::CDlgGame() : CFGuiDialog(IDD_DLG_POPUP)
{
}

//��������
CDlgGame::~CDlgGame()
{
}

//��������
BOOL CDlgGame::OnInitDialog()
{
	//���ñ���
	SetWindowText(TEXT("��Ϸ�"));

	//���д���
	CenterWindow(this);


	return TRUE;
}

//ȷ������
VOID CDlgGame::OnOK()
{

}

//ȡ����Ϣ
VOID CDlgGame::OnCancel()
{
	return __super::OnCancel();
}

//��ʼ����
void CDlgGame::InitControlUI()
{
	//�����ַ
	CGlobalWebLink * pGlobalWebLink=CGlobalWebLink::GetInstance();
	TCHAR szBillUrl[256]=TEXT("");
	_sntprintf(szBillUrl,CountArray(szBillUrl),TEXT("%s/ads/AdsNotice.aspx"),pGlobalWebLink->GetPlatformLink());
	CWkeBrowserUI* pActiveXUI1 = static_cast<CWkeBrowserUI*>(GetControlByName(szWebBrowserFeedBackControlName));
	if( pActiveXUI1 ) 
	{
		pActiveXUI1->Navigate(szBillUrl);
	}
}

//Ƥ������
LPCTSTR CDlgGame::GetSkinFile()
{
	return TEXT("DlgGameAd.xml"); 
}

//��Ϣ����
void CDlgGame::Notify(TNotifyUI &  msg)
{
	//��ȡ����
	CControlUI * pControlUI = msg.pSender;
	if (lstrcmp(msg.sType, TEXT("click")) == 0)
	{
		if(lstrcmp(pControlUI->GetName(), szButtonCloseControlName)==0)
		{
			//���ղ�����ʾ
			bool bShow = false;
			CCheckButtonUI * pCheckButtonPasswd=(CCheckButtonUI *)GetControlByName(szCheckButtonShowControlName);;
			if(pCheckButtonPasswd!=NULL)
			{
				bShow = pCheckButtonPasswd->GetCheck();
			}
			if (bShow)
			{
				//��ȡʱ��
				SYSTEMTIME SystemTime;
				GetLocalTime(&SystemTime); 
				WORD wDay=SystemTime.wDay;	
				WORD wMonth=SystemTime.wMonth;
				WORD wYear=SystemTime.wYear-1980;
				WORD wCurrDate = wYear<<16|wMonth<<8|wDay;

				CWHRegKey RegOptionItem;
				RegOptionItem.OpenRegKey(TEXT(""),true);
				RegOptionItem.WriteValue(TEXT("TodayShowDate"),wCurrDate);
			}

			CPlatformEvent * pPlatformEvent=CPlatformEvent::GetInstance();
			if (pPlatformEvent!=NULL) pPlatformEvent->PostPlatformEvent(EVENT_USER_CLOSE_GAME_AD,0L);

			return OnCancel();
		}
	}
	return;
}

//////////////////////////////////////////////////////////////////////////////////