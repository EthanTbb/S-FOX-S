#include "Stdafx.h"
#include "Resource.h"
#include "DlgPhoneScanCode.h"

//////////////////////////////////////////////////////////////////////////////////
//按钮控件
const TCHAR* const szButtonCloseControlName = TEXT("ButtonClose");
const TCHAR* const szWebPhoneScanCodeControlName = TEXT("WebPhoneScanCode");

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CPhoneScanCode::CPhoneScanCode() : CFGuiDialog(IDD_DLG_POPUP)
{
	m_pGameKind = NULL;
}

//析构函数
CPhoneScanCode::~CPhoneScanCode()
{
}

//创建函数
BOOL CPhoneScanCode::OnInitDialog()
{
	//设置标题
	SetWindowText(TEXT("手机中心"));

	//居中窗口
	CenterWindow(this);

	//下移距离
	CRect rc;
	GetWindowRect(&rc);
	rc.top += 30;
	rc.bottom += 30;
	MoveWindow(rc);

	return TRUE;
}

//确定函数
VOID CPhoneScanCode::OnOK()
{
	return __super::OnOK();
}

//取消消息
VOID CPhoneScanCode::OnCancel()
{
	return __super::OnCancel();
}

//初始函数
void CPhoneScanCode::InitControlUI()
{
	//获取对象
	ASSERT(CGlobalWebLink::GetInstance()!=NULL);
	CGlobalWebLink * pGlobalWebLink=CGlobalWebLink::GetInstance();

	if (m_pGameKind != NULL)
	{
		//构造地址
		TCHAR szBillUrl[256]=TEXT("");
		_sntprintf(szBillUrl,CountArray(szBillUrl),TEXT("%s/QrDownload.aspx?KindID=%d"),pGlobalWebLink->GetPlatformLink(),m_pGameKind->m_GameKind.wKindID);

		CWkeBrowserUI* pActiveXUI1 = static_cast<CWkeBrowserUI*>(GetControlByName(szWebPhoneScanCodeControlName));
		if( pActiveXUI1 ) 
		{
			pActiveXUI1->Navigate(szBillUrl);
		}
	}
}

//皮肤函数
LPCTSTR CPhoneScanCode::GetSkinFile()
{
	return TEXT("DlgPhoneScanCode.xml"); 
}

//消息函数
void CPhoneScanCode::Notify(TNotifyUI &  msg)
{
	//获取对象
	CControlUI * pControlUI = msg.pSender;

	//点击事件
	if (lstrcmp(msg.sType, TEXT("click")) == 0)
	{
		if(lstrcmp(pControlUI->GetName(), szButtonCloseControlName)==0)
		{
			return OnCancel();
		}
	}
	
	return;
}

//加载数据
void CPhoneScanCode::LoadDataFromExternal(CGameKindItem * pGameKind)
{
	m_pGameKind = pGameKind;
}

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CPhoneScanCodePlaza::CPhoneScanCodePlaza() : CFGuiDialog(IDD_DLG_POPUP)
{

}

//析构函数
CPhoneScanCodePlaza::~CPhoneScanCodePlaza()
{
}

//创建函数
BOOL CPhoneScanCodePlaza::OnInitDialog()
{
	//设置标题
	SetWindowText(TEXT("手机中心"));

	//居中窗口
	CenterWindow(this);

	//下移距离
	CRect rc;
	GetWindowRect(&rc);
	rc.top += 30;
	rc.bottom += 30;
	MoveWindow(rc);

	return TRUE;
}

//确定函数
VOID CPhoneScanCodePlaza::OnOK()
{
	return __super::OnOK();
}

//取消消息
VOID CPhoneScanCodePlaza::OnCancel()
{
	return __super::OnCancel();
}

//初始函数
void CPhoneScanCodePlaza::InitControlUI()
{
	//获取对象
	ASSERT(CGlobalWebLink::GetInstance()!=NULL);
	CGlobalWebLink * pGlobalWebLink=CGlobalWebLink::GetInstance();


	//构造地址
	TCHAR szBillUrl[256]=TEXT("");
	_sntprintf(szBillUrl,CountArray(szBillUrl),TEXT("%s/QrDownload.aspx"),pGlobalWebLink->GetPlatformLink());

	CWkeBrowserUI* pActiveXUI1 = static_cast<CWkeBrowserUI*>(GetControlByName(szWebPhoneScanCodeControlName));
	if( pActiveXUI1 ) 
	{
		pActiveXUI1->Navigate(szBillUrl);
	}

}

//皮肤函数
LPCTSTR CPhoneScanCodePlaza::GetSkinFile()
{
	return TEXT("DlgPhoneScanCode.xml"); 
}

//消息函数
void CPhoneScanCodePlaza::Notify(TNotifyUI &  msg)
{
	//获取对象
	CControlUI * pControlUI = msg.pSender;

	//点击事件
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