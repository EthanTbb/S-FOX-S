#include "Stdafx.h"
#include "Resource.h"
#include "DlgTablePassword.h"
#include "DlgInformation.h"

//////////////////////////////////////////////////////////////////////////////////
//控件名字
const TCHAR* const szButtonCloseControlName = TEXT("ButtonClose");
const TCHAR* const szButtonCancleControlName = TEXT("ButtonCancle");
const TCHAR* const szButtonOkControlName = TEXT("ButtonOk");
//////////////////////////////////////////////////////////////////////////

//构造函数
CDlgTablePassword::CDlgTablePassword() :  CFGuiDialog(IDD_DLG_POPUP)
{
}

//析构函数
CDlgTablePassword::~CDlgTablePassword()
{
}

//创建函数
BOOL CDlgTablePassword::OnInitDialog()
{
	SetWindowText(TEXT("桌子密码"));

	//居中窗口
	CenterWindow(this);

	return TRUE;
}

//确定函数
VOID CDlgTablePassword::OnOK()
{
	CEditUI* editPassword = (CEditUI*)GetControlByName(TEXT("EditPwd"));
	if (editPassword!=NULL)
	{
        ZeroMemory(m_szPassword,sizeof(m_szPassword));
		//获取密码	
		lstrcpyn(m_szPassword,editPassword->GetText(),CountArray(m_szPassword));
		if (m_szPassword[0]==0)
		{
			//提示消息
			CDlgInformation Information(this);
			Information.ShowMessageBox(TEXT("游戏桌密码不能为空，请重新输入！"),MB_ICONINFORMATION);
			//设置焦点
			editPassword->SetFocus();
			return;
		}
	}
	__super::OnOK();
}

//取消消息
VOID CDlgTablePassword::OnCancel()
{
	return __super::OnCancel();
}

//初始函数
VOID CDlgTablePassword::InitControlUI()
{
}

//皮肤函数
LPCTSTR CDlgTablePassword::GetSkinFile()
{
	return TEXT("DlgTablePassword.xml");
}

//消息函数
VOID CDlgTablePassword::Notify(TNotifyUI &  msg)
{
	//获取对象
	CControlUI * pControlUI = msg.pSender;

	if (lstrcmp(msg.sType, TEXT("click")) == 0)
	{
		if(lstrcmp(pControlUI->GetName(), szButtonCloseControlName)==0)
		{
			return OnCancel();
		}
		else if (lstrcmp(pControlUI->GetName(), szButtonCancleControlName)==0)
		{
			return OnCancel();
		}
		else if (lstrcmp(pControlUI->GetName(), szButtonOkControlName)==0)
		{
			return OnOK();
		}
	}
	return;
}

//////////////////////////////////////////////////////////////////////////