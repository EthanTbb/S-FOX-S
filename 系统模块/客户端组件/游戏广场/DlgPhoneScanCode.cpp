#include "Stdafx.h"
#include "Resource.h"
#include "DlgPhoneScanCode.h"

//////////////////////////////////////////////////////////////////////////////////
//��ť�ؼ�
const TCHAR* const szButtonCloseControlName = TEXT("ButtonClose");
const TCHAR* const szWebPhoneScanCodeControlName = TEXT("WebPhoneScanCode");

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CPhoneScanCode::CPhoneScanCode() : CFGuiDialog(IDD_DLG_POPUP)
{
	m_pGameKind = NULL;
}

//��������
CPhoneScanCode::~CPhoneScanCode()
{
}

//��������
BOOL CPhoneScanCode::OnInitDialog()
{
	//���ñ���
	SetWindowText(TEXT("�ֻ�����"));

	//���д���
	CenterWindow(this);

	//���ƾ���
	CRect rc;
	GetWindowRect(&rc);
	rc.top += 30;
	rc.bottom += 30;
	MoveWindow(rc);

	return TRUE;
}

//ȷ������
VOID CPhoneScanCode::OnOK()
{
	return __super::OnOK();
}

//ȡ����Ϣ
VOID CPhoneScanCode::OnCancel()
{
	return __super::OnCancel();
}

//��ʼ����
void CPhoneScanCode::InitControlUI()
{
	//��ȡ����
	ASSERT(CGlobalWebLink::GetInstance()!=NULL);
	CGlobalWebLink * pGlobalWebLink=CGlobalWebLink::GetInstance();

	if (m_pGameKind != NULL)
	{
		//�����ַ
		TCHAR szBillUrl[256]=TEXT("");
		_sntprintf(szBillUrl,CountArray(szBillUrl),TEXT("%s/QrDownload.aspx?KindID=%d"),pGlobalWebLink->GetPlatformLink(),m_pGameKind->m_GameKind.wKindID);

		CWkeBrowserUI* pActiveXUI1 = static_cast<CWkeBrowserUI*>(GetControlByName(szWebPhoneScanCodeControlName));
		if( pActiveXUI1 ) 
		{
			pActiveXUI1->Navigate(szBillUrl);
		}
	}
}

//Ƥ������
LPCTSTR CPhoneScanCode::GetSkinFile()
{
	return TEXT("DlgPhoneScanCode.xml"); 
}

//��Ϣ����
void CPhoneScanCode::Notify(TNotifyUI &  msg)
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
	
	return;
}

//��������
void CPhoneScanCode::LoadDataFromExternal(CGameKindItem * pGameKind)
{
	m_pGameKind = pGameKind;
}

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CPhoneScanCodePlaza::CPhoneScanCodePlaza() : CFGuiDialog(IDD_DLG_POPUP)
{

}

//��������
CPhoneScanCodePlaza::~CPhoneScanCodePlaza()
{
}

//��������
BOOL CPhoneScanCodePlaza::OnInitDialog()
{
	//���ñ���
	SetWindowText(TEXT("�ֻ�����"));

	//���д���
	CenterWindow(this);

	//���ƾ���
	CRect rc;
	GetWindowRect(&rc);
	rc.top += 30;
	rc.bottom += 30;
	MoveWindow(rc);

	return TRUE;
}

//ȷ������
VOID CPhoneScanCodePlaza::OnOK()
{
	return __super::OnOK();
}

//ȡ����Ϣ
VOID CPhoneScanCodePlaza::OnCancel()
{
	return __super::OnCancel();
}

//��ʼ����
void CPhoneScanCodePlaza::InitControlUI()
{
	//��ȡ����
	ASSERT(CGlobalWebLink::GetInstance()!=NULL);
	CGlobalWebLink * pGlobalWebLink=CGlobalWebLink::GetInstance();


	//�����ַ
	TCHAR szBillUrl[256]=TEXT("");
	_sntprintf(szBillUrl,CountArray(szBillUrl),TEXT("%s/QrDownload.aspx"),pGlobalWebLink->GetPlatformLink());

	CWkeBrowserUI* pActiveXUI1 = static_cast<CWkeBrowserUI*>(GetControlByName(szWebPhoneScanCodeControlName));
	if( pActiveXUI1 ) 
	{
		pActiveXUI1->Navigate(szBillUrl);
	}

}

//Ƥ������
LPCTSTR CPhoneScanCodePlaza::GetSkinFile()
{
	return TEXT("DlgPhoneScanCode.xml"); 
}

//��Ϣ����
void CPhoneScanCodePlaza::Notify(TNotifyUI &  msg)
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

	return;
}

//////////////////////////////////////////////////////////////////////////////////