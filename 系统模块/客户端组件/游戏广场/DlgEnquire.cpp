#include "Stdafx.h"
#include "DlgEnquire.h"
#include "GlobalUnits.h"
#include "PlatformFrame.h"

//////////////////////////////////////////////////////////////////////////////////
//��ѡ�ؼ�
const TCHAR* const szCheckButtonSwitchControlName = TEXT("ButtonSwitch");
const TCHAR* const szCheckButtonQuitRoomControlName = TEXT("ButtonQuitRoom");
const TCHAR* const szCheckButtonQuitPlazaControlName = TEXT("ButtonQuitPlaza");
const TCHAR* const szCheckButtonCloseControlName = TEXT("ButtonClose");
const TCHAR* const szWebBrowserClosePlazaControlName = TEXT("WebBrowserClosePlaza");

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CDlgEnquire::CDlgEnquire() : CFGuiDialog(IDD_DLG_POPUP)
{
}

//��������
CDlgEnquire::~CDlgEnquire()
{
}

//��������
BOOL CDlgEnquire::OnInitDialog()
{
	//���ñ���
	SetWindowText(TEXT("�˳���ʾ"));

	//���д���
	CenterWindow(this);

	return TRUE;
}

//ȷ������
VOID CDlgEnquire::OnOK()
{
	return;
}

//ȡ����Ϣ
VOID CDlgEnquire::OnCancel()
{
	return __super::OnCancel();
}

//��ʼ�ؼ�
void CDlgEnquire::InitControlUI()
{
	CButtonUI *pButtonUI = static_cast<CButtonUI *>(GetControlByName(szCheckButtonQuitRoomControlName));
	if(pButtonUI == NULL) return;
	//��ȡ����
	CPlatformFrame * pPlatformFrame = CPlatformFrame::GetInstance();
	pButtonUI->SetEnabled(pPlatformFrame->IsEnteredServerItem());

	//��ȡ����
	CGlobalWebLink * pGlobalWebLink=CGlobalWebLink::GetInstance();

	//�����ַ
	TCHAR szBillUrl[256]=TEXT("");
	_sntprintf(szBillUrl,CountArray(szBillUrl),TEXT("%s/ads/ClosePlaza.aspx"),pGlobalWebLink->GetPlatformLink());

	CWkeBrowserUI* pActiveXUI1 = static_cast<CWkeBrowserUI*>(GetControlByName(szWebBrowserClosePlazaControlName));
	if( pActiveXUI1 ) 
	{
		pActiveXUI1->Navigate(szBillUrl);
	}

	return;
}

//��Ϣ����
void CDlgEnquire::Notify(TNotifyUI &  msg)
{
	__super::Notify(msg);

	//��ȡ����
	CControlUI * pControlUI = msg.pSender;

	if (lstrcmp(msg.sType, TEXT("click")) == 0)
	{
		if(lstrcmp(pControlUI->GetName(), szCheckButtonSwitchControlName) == 0)
		{
			return OnBnClickedSwitchAccounts();
		}
		else if(lstrcmp(pControlUI->GetName(), szCheckButtonQuitRoomControlName) == 0)
		{
			return OnBnClickedCloseServer();
		}
		else if(lstrcmp(pControlUI->GetName(), szCheckButtonQuitPlazaControlName) == 0)
		{
			return OnBnClickedClosePlaza();
		}
		else if(lstrcmp(pControlUI->GetName(), szCheckButtonCloseControlName) == 0)
		{
			return OnCancel();
		}
	}
}

//Ƥ������
LPCTSTR CDlgEnquire::GetSkinFile()
{
	return TEXT("DlgQuit.xml");
}

//�رմ���
VOID CDlgEnquire::OnBnClickedClosePlaza()
{
	EndDialog(IDC_CLOSE_PLAZA);

	return;
}

//�رշ���
VOID CDlgEnquire::OnBnClickedCloseServer()
{
	EndDialog(IDC_CLOSE_SERVER);

	return;
}

//�л��ʺ�
VOID CDlgEnquire::OnBnClickedSwitchAccounts()
{
	EndDialog(IDC_SWITCH_ACCOUNTS);

	return;
}

//////////////////////////////////////////////////////////////////////////////////
