#include "Stdafx.h"
#include "Resource.h"
#include "DlgFeedBack.h"
#include "DlgInformation.h"
//////////////////////////////////////////////////////////////////////////////////
//�ؼ�����
const TCHAR* const szButtonCloseControlName = TEXT("ButtonClose");
const TCHAR* const szWebBrowserFeedBackControlName = TEXT("WebBrowserFeedBack");
//////////////////////////////////////////////////////////////////////////////////

//���캯��
CDlgFeedBack::CDlgFeedBack() : CFGuiDialog(IDD_DLG_POPUP)
{
}

//��������
CDlgFeedBack::~CDlgFeedBack()
{
}

//��������
BOOL CDlgFeedBack::OnInitDialog()
{
	//���ñ���
	SetWindowText(TEXT("���߷���"));

	//���д���
	CenterWindow(this);

	//Js����
	CWkeBrowserUI::JSBindFunction("msgBox", js_msgBox, 1);

	return TRUE;
}

//ȷ������
VOID CDlgFeedBack::OnOK()
{

}

//ȡ����Ϣ
VOID CDlgFeedBack::OnCancel()
{
	return __super::OnCancel();
}

//��ʼ����
void CDlgFeedBack::InitControlUI()
{
	//�����ַ
	CGlobalWebLink * pGlobalWebLink=CGlobalWebLink::GetInstance();
	TCHAR szBillUrl[256]=TEXT("");
	_sntprintf(szBillUrl,CountArray(szBillUrl),TEXT("%s/%sUserService/Feedback.aspx"),pGlobalWebLink->GetPlatformLink(),pGlobalWebLink->GetValidateLink());
	CWkeBrowserUI* pActiveXUI1 = static_cast<CWkeBrowserUI*>(GetControlByName(szWebBrowserFeedBackControlName));
	if( pActiveXUI1 ) 
	{
		pActiveXUI1->Navigate(szBillUrl);
	}
}

//Ƥ������
LPCTSTR CDlgFeedBack::GetSkinFile()
{
	return TEXT("DlgFeedBack.xml"); 
}

//��Ϣ����
void CDlgFeedBack::Notify(TNotifyUI &  msg)
{
	//��ȡ����
	CControlUI * pControlUI = msg.pSender;
	if (lstrcmp(msg.sType, TEXT("click")) == 0)
	{
		if(lstrcmp(pControlUI->GetName(), szButtonCloseControlName)==0)
		{
			return OnCancel();
		}
	}
	return;
}

//JS����
jsValue JS_CALL CDlgFeedBack::js_msgBox(jsExecState es)
{
	CDlgInformation Information;
	Information.ShowMessageBox(CWkeBrowserUI::JSBindGetArg(es, 0));
	return 0;
}

//////////////////////////////////////////////////////////////////////////////////