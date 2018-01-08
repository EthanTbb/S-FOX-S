#include "StdAfx.h"
#include "Resource.h"
#include "DlgInformation.h"

//////////////////////////////////////////////////////////////////////////////////


BEGIN_MESSAGE_MAP(CDlgInformation, CFGuiDialog)
	ON_WM_TIMER()
END_MESSAGE_MAP()

#define IDI_MESSAGE 100

//////////////////////////////////////////////////////////////////////////////////

//图标标识
const TCHAR* const szLableIcoWarnControlName = TEXT("LableIcoWarn");
const TCHAR* const szLableIcoErrorControlName = TEXT("LableIcoError");
const TCHAR* const szLableIcoQuestControlName = TEXT("LableIcoQuest");
const TCHAR* const szLableIcoInfoControlName = TEXT("LableIcoInfo");

//按钮标识
const TCHAR* const szButtonAbortControlName = TEXT("ButtonAbort");
const TCHAR* const szButtonRetryControlName = TEXT("ButtonRetry");
const TCHAR* const szButtonIgnoreControlName = TEXT("ButtonIgnore");
const TCHAR* const szButtonYESControlName = TEXT("ButtonYES");
const TCHAR* const szButtonNOControlName = TEXT("ButtonNO");
const TCHAR* const szButtonOKControlName = TEXT("ButtonOK");
const TCHAR* const szButtonCancleControlName = TEXT("ButtonCancle");

//文字信息
const TCHAR* const szTextTipControlName = TEXT("TextTip");

//关闭按钮
const TCHAR* const szButtonCloseControlName = TEXT("ButtonClose");
//////////////////////////////////////////////////////////////////////////////////

//构造函数
CDlgInformation::CDlgInformation(CWnd * pParentWnd) : CFGuiDialog(IDD_DLG_SIMPLE, pParentWnd)
{
	//设置变量
	m_uType=MB_OK;

	//辅助变量
	m_nElapse=0;
	m_nBtCount=0;

	//控件变量

	ZeroMemory(m_uButtonID,sizeof(m_uButtonID));

	return;
}

//析构函数
CDlgInformation::~CDlgInformation()
{
}

//初始化消息
BOOL CDlgInformation::OnInitDialog()
{
	__super::OnInitDialog();

	//设置标题
	SetWindowText(m_strCaption);


	//居中窗口
	SetActiveWindow();
	SetForegroundWindow();
	CenterWindow(GetParent());

	//设置时间
	//if (m_nElapse!=0) SetTimer(IDI_MESSAGE,1000,NULL);

	UpdateUI();

	return FALSE;
}

//初始控件
void CDlgInformation::InitControlUI()
{
	__super::InitControlUI();


}

//消息提醒
void CDlgInformation::Notify(TNotifyUI &  msg)
{
	__super::Notify(msg);

	//获取对象
	CControlUI * pControlUI = msg.pSender;

	//点击事件
	if (lstrcmp(msg.sType, TEXT("click")) == 0)
	{
		if(lstrcmp(pControlUI->GetName(), szButtonAbortControlName)==0)
		{
			EndDialog(IDABORT);
		}
		if(lstrcmp(pControlUI->GetName(), szButtonRetryControlName)==0)
		{
			EndDialog(IDRETRY);
		}
		if(lstrcmp(pControlUI->GetName(), szButtonIgnoreControlName)==0)
		{
			EndDialog(IDIGNORE);
		}
		if(lstrcmp(pControlUI->GetName(), szButtonYESControlName)==0)
		{
			EndDialog(IDYES);
		}
		if(lstrcmp(pControlUI->GetName(), szButtonNOControlName)==0)
		{
			EndDialog(IDNO);
		}
		if(lstrcmp(pControlUI->GetName(), szButtonOKControlName)==0)
		{
			EndDialog(IDOK);
		}
		if((lstrcmp(pControlUI->GetName(), szButtonCancleControlName)==0)
			||(lstrcmp(pControlUI->GetName(), szButtonCloseControlName)==0))
		{
			EndDialog(IDCANCEL);
		}
	}	
}


//显示消息
INT CDlgInformation::ShowMessageBox(LPCTSTR pszString, UINT uType, UINT nElapse)
{
	return ShowMessageBox(TEXT("系统提示"),pszString,uType,nElapse);
}

//显示消息
INT CDlgInformation::ShowMessageBox(LPCTSTR pszCaption, LPCTSTR pszString, UINT uType, UINT nElapse)
{
	//设置变量
	m_uType=uType;
	m_nElapse=nElapse;
	m_strString=pszString;
	m_strCaption=pszCaption;

	//显示窗口
	INT nResult=(INT)DoModal();

	return nResult;
}

//激活按钮
VOID CDlgInformation::CreateBoxButton()
{
	//变量定义
	LPCTSTR pszString[3]={NULL,NULL,NULL};

	//设置按钮
	switch (m_uType&MB_TYPEMASK)
	{
	case MB_OKCANCEL:
		{
			m_nBtCount=2;
			m_uButtonID[0]=IDOK;
			m_uButtonID[1]=IDCANCEL;
			pszString[0]=TEXT("确定(O)");
			pszString[1]=TEXT("取消(C)");
			break;
		}
	case MB_YESNO:
		{
			m_nBtCount=2;
			m_uButtonID[0]=IDYES;
			m_uButtonID[1]=IDNO;
			pszString[0]=TEXT("是(Y)");
			pszString[1]=TEXT("否(N)");
			break;
		}
	case MB_YESNOCANCEL:
		{
			m_nBtCount=3;
			m_uButtonID[0]=IDYES;
			m_uButtonID[1]=IDNO;
			m_uButtonID[2]=IDCANCEL;
			pszString[0]=TEXT("是(Y)");
			pszString[1]=TEXT("否(N)");
			pszString[2]=TEXT("取消(C)");
			break;
		}
	case MB_RETRYCANCEL:
		{
			m_nBtCount=2;
			m_uButtonID[0]=IDRETRY;
			m_uButtonID[1]=IDCANCEL;
			pszString[0]=TEXT("重试(R)");
			pszString[1]=TEXT("取消(C)");
			break;
		}
	case MB_ABORTRETRYIGNORE:
		{
			m_nBtCount=3;
			m_uButtonID[0]=IDABORT;
			m_uButtonID[1]=IDRETRY;
			m_uButtonID[2]=IDIGNORE;
			pszString[0]=TEXT("中止(A)");
			pszString[1]=TEXT("重试(R)");
			pszString[2]=TEXT("忽略(I)");
			break;
		}
	default:
		{
			m_nBtCount=1;
			m_uButtonID[0]=IDOK;
			pszString[0]=TEXT("确定(O)");
			break;
		}
	}

	//设置按钮
	for (UINT i=0;i<m_nBtCount;i++) 
	{
		m_pbtButton[i]->SetTag(m_uButtonID[i]);
		m_pbtButton[i]->SetText(pszString[i]);
	}

	return;
}

//时间消息
VOID CDlgInformation::OnTimer(UINT_PTR nIDEvent)
{
	//倒数时间
	if (nIDEvent==IDI_MESSAGE)
	{
		//删除判断
		if (m_nElapse==0)
		{
			PostMessage(WM_CLOSE,0,0);
			return;
		}

		//设置界面
		TCHAR szTitle[32]=TEXT("");
		_sntprintf(szTitle,CountArray(szTitle),TEXT("%s （%ld 秒）"),m_strCaption,m_nElapse--);
		SetWindowText(szTitle);

		return;
	}

	__super::OnTimer(nIDEvent);
}

VOID CDlgInformation::UpdateUI()
{
	CControlUI * pControlUI =NULL;
	//隐藏图标
	pControlUI=GetControlByName(szLableIcoWarnControlName);
	if(pControlUI!=NULL) pControlUI->SetVisible(false);

	pControlUI=GetControlByName(szLableIcoErrorControlName);
	if(pControlUI!=NULL) pControlUI->SetVisible(false);

	pControlUI=GetControlByName(szLableIcoQuestControlName);
	if(pControlUI!=NULL) pControlUI->SetVisible(false);

	pControlUI=GetControlByName(szLableIcoInfoControlName);
	if(pControlUI!=NULL) pControlUI->SetVisible(false);

	//设置图标

	switch(m_uType&MB_ICONMASK)
	{
		case MB_ICONWARNING:
			{
				pControlUI=GetControlByName(szLableIcoWarnControlName);
				if(pControlUI!=NULL) pControlUI->SetVisible(true);
				break;
			}
		case MB_ICONERROR:
			{
				pControlUI=GetControlByName(szLableIcoErrorControlName);
				if(pControlUI!=NULL) pControlUI->SetVisible(true);
				break;
			}
		case MB_ICONINFORMATION:
			{
				pControlUI=GetControlByName(szLableIcoInfoControlName);
				if(pControlUI!=NULL) pControlUI->SetVisible(true);
				break;
			}
		case MB_ICONQUESTION:
			{
				pControlUI=GetControlByName(szLableIcoQuestControlName);
				if(pControlUI!=NULL) pControlUI->SetVisible(true);
				break;
			}
		default:
			{
				pControlUI=GetControlByName(szLableIcoInfoControlName);
				if(pControlUI!=NULL) pControlUI->SetVisible(true);
				break;
			}
	}

	//隐藏按钮
	pControlUI=GetControlByName(szButtonRetryControlName);
	if(pControlUI!=NULL) pControlUI->SetVisible(false);

	pControlUI=GetControlByName(szButtonAbortControlName);
	if(pControlUI!=NULL) pControlUI->SetVisible(false);

	pControlUI=GetControlByName(szButtonIgnoreControlName);
	if(pControlUI!=NULL) pControlUI->SetVisible(false);

	pControlUI=GetControlByName(szButtonYESControlName);
	if(pControlUI!=NULL) pControlUI->SetVisible(false);

	pControlUI=GetControlByName(szButtonNOControlName);
	if(pControlUI!=NULL) pControlUI->SetVisible(false);

	pControlUI=GetControlByName(szButtonOKControlName);
	if(pControlUI!=NULL) pControlUI->SetVisible(false);

	pControlUI=GetControlByName(szButtonCancleControlName);
	if(pControlUI!=NULL) pControlUI->SetVisible(false);

	//设置按钮
	switch (m_uType&MB_TYPEMASK)
	{
	case MB_OKCANCEL:
		{
			pControlUI=GetControlByName(szButtonOKControlName);
			if(pControlUI!=NULL) pControlUI->SetVisible(true);

			pControlUI=GetControlByName(szButtonCancleControlName);
			if(pControlUI!=NULL) pControlUI->SetVisible(true);
			break;
		}
	case MB_YESNO:
		{
			pControlUI=GetControlByName(szButtonYESControlName);
			if(pControlUI!=NULL) pControlUI->SetVisible(true);

			pControlUI=GetControlByName(szButtonNOControlName);
			if(pControlUI!=NULL) pControlUI->SetVisible(true);

			break;
		}
	case MB_YESNOCANCEL:
		{
			pControlUI=GetControlByName(szButtonYESControlName);
			if(pControlUI!=NULL) pControlUI->SetVisible(true);

			pControlUI=GetControlByName(szButtonNOControlName);
			if(pControlUI!=NULL) pControlUI->SetVisible(true);

			pControlUI=GetControlByName(szButtonCancleControlName);
			if(pControlUI!=NULL) pControlUI->SetVisible(true);
			break;
		}
	case MB_RETRYCANCEL:
		{
			pControlUI=GetControlByName(szButtonRetryControlName);
			if(pControlUI!=NULL) pControlUI->SetVisible(true);

			pControlUI=GetControlByName(szButtonCancleControlName);
			if(pControlUI!=NULL) pControlUI->SetVisible(true);
			break;
		}
	case MB_ABORTRETRYIGNORE:
		{
			pControlUI=GetControlByName(szButtonAbortControlName);
			if(pControlUI!=NULL) pControlUI->SetVisible(true);

			pControlUI=GetControlByName(szButtonRetryControlName);
			if(pControlUI!=NULL) pControlUI->SetVisible(true);

			pControlUI=GetControlByName(szButtonIgnoreControlName);
			if(pControlUI!=NULL) pControlUI->SetVisible(true);
			break;
		}
	default:
		{
			pControlUI=GetControlByName(szButtonOKControlName);
			if(pControlUI!=NULL) pControlUI->SetVisible(true);
			break;
		}
	}

	//设置文字
	pControlUI=GetControlByName(szTextTipControlName);
	if(pControlUI!=NULL) pControlUI->SetText(m_strString);

}

LPCTSTR CDlgInformation::GetSkinFile()
{
	return TEXT("DlgInformation.xml");
}

//////////////////////////////////////////////////////////////////////////////////
