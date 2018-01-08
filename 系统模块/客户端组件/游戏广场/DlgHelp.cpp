#include "Stdafx.h"
#include "Resource.h"
#include "DlgHelp.h"

//////////////////////////////////////////////////////////////////////////////////
//控件名字
const TCHAR* const szButtonCloseControlName = TEXT("ButtonClose");
const TCHAR* const szButtonHelp1ControlName = TEXT("ButtonHelp1");
const TCHAR* const szButtonHelp2ControlName = TEXT("ButtonHelp2");
const TCHAR* const szButtonHelp3ControlName = TEXT("ButtonHelp3");
const TCHAR* const szButtonHelp4ControlName = TEXT("ButtonHelp4");
const TCHAR* const szButtonHelp5ControlName = TEXT("ButtonHelp5");
const TCHAR* const szButtonHelp6ControlName = TEXT("ButtonHelp6");
const TCHAR* const szButtonHelp7ControlName = TEXT("ButtonHelp7");
const TCHAR* const szButtonHelp8ControlName = TEXT("ButtonHelp8");
//////////////////////////////////////////////////////////////////////////////////

//构造函数
CDlgHelp::CDlgHelp() : CFGuiDialog(IDD_DLG_POPUP)
{
}

//析构函数
CDlgHelp::~CDlgHelp()
{
}

//创建函数
BOOL CDlgHelp::OnInitDialog()
{
	__super::OnInitDialog();

	//设置标题
	SetWindowText(TEXT("游戏帮助"));

	//居中窗口
	CenterWindow(this);

	return TRUE;
}

//确定函数
VOID CDlgHelp::OnOK()
{

}

//取消消息
VOID CDlgHelp::OnCancel()
{
	return __super::OnCancel();
}

//初始函数
void CDlgHelp::InitControlUI()
{
}

//皮肤函数
LPCTSTR CDlgHelp::GetSkinFile()
{
	return TEXT("DlgHelp.xml"); 
}

//消息函数
void CDlgHelp::Notify(TNotifyUI &  msg)
{
	//获取对象
	CControlUI * pControlUI = msg.pSender;
	if (lstrcmp(msg.sType, TEXT("click")) == 0)
	{
		if(lstrcmp(pControlUI->GetName(), szButtonCloseControlName)==0)
		{
			return OnCancel();
		}
		else if(
			lstrcmp(pControlUI->GetName(), szButtonHelp1ControlName)==0 ||
			lstrcmp(pControlUI->GetName(), szButtonHelp2ControlName)==0 ||
			lstrcmp(pControlUI->GetName(), szButtonHelp3ControlName)==0 ||
			lstrcmp(pControlUI->GetName(), szButtonHelp4ControlName)==0 ||
			lstrcmp(pControlUI->GetName(), szButtonHelp5ControlName)==0 ||
			lstrcmp(pControlUI->GetName(), szButtonHelp6ControlName)==0 ||
			lstrcmp(pControlUI->GetName(), szButtonHelp7ControlName)==0 ||
			lstrcmp(pControlUI->GetName(), szButtonHelp8ControlName)==0
			)
		{
			//获取对象
			ASSERT(CGlobalWebLink::GetInstance()!=NULL);
			CGlobalWebLink * pGlobalWebLink=CGlobalWebLink::GetInstance();
			//构造地址
			TCHAR szNavigation[256]=TEXT("");
			_sntprintf(szNavigation,CountArray(szNavigation),TEXT("%s/PlatService/PlatService%d.aspx"),pGlobalWebLink->GetPlatformLink(),pControlUI->GetTag());
			//打开页面
			ShellExecute(NULL,TEXT("OPEN"),szNavigation,NULL,NULL,SW_NORMAL);
			return;
		}
	}
	return;
}
//////////////////////////////////////////////////////////////////////////////////