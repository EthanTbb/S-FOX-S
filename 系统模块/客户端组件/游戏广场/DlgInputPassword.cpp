#include "StdAfx.h"
#include "Resource.h"
#include "DlgInputPassword.h"
#include "DlgInformation.h"
//////////////////////////////////////////////////////////////////////////////////

//圆角大小
#define ROUND_CX					5									//圆角宽度
#define ROUND_CY					5									//圆角高度

//屏幕位置
#define LAYERED_SIZE				4									//分层大小
#define CAPTION_SIZE				15									//标题大小

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CDlgInputPassword::CDlgInputPassword() : CFGuiDialog(IDD_DLG_POPUP)
{
	//界面控件
	m_pbtOk=NULL;
	m_pbtCancel=NULL;
	m_pEditPassword=NULL;

	//设置变量
	ZeroMemory(m_szPassword,sizeof(m_szPassword));

	return;
}

//析够函数
CDlgInputPassword::~CDlgInputPassword()
{
}

LPCTSTR CDlgInputPassword::GetSkinFile()
{
	return TEXT("DlgInputPassword.xml");
}

//创建函数
BOOL CDlgInputPassword::OnInitDialog()
{
	__super::OnInitDialog();

	//设置标题
	SetWindowText(TEXT("房间密码"));

	//居中窗口
	CenterWindow(this);

	return TRUE;
}

//确定函数
VOID CDlgInputPassword::OnOK()
{
	//获取信息
	lstrcpyn(m_szPassword,m_pEditPassword->GetText(),CountArray(m_szPassword));

	//密码判断
	if (m_szPassword[0]==0)
	{
		//错误提示
		CDlgInformation Information(this);
		Information.ShowMessageBox(TEXT("密码不能为空，请输入房间密码！"),MB_ICONERROR);

		//设置焦点
		m_pEditPassword->SetFocus();

		return;
	}

	__super::OnOK();
}

//取消按钮
VOID CDlgInputPassword::OnCancel()
{
	__super::OnCancel();
}

//初始控件
void CDlgInputPassword::InitControlUI()
{
	__super::InitControlUI();

	//确认按钮
	m_pbtOk = static_cast<CButtonUI*>(GetControlByName(TEXT("ButtonOk")));

	//关闭按钮
	m_pbtCancel = static_cast<CButtonUI*>(GetControlByName(TEXT("ButtonCancle")));

	//房间密码
	m_pEditPassword = static_cast<CEditUI*>(GetControlByName(TEXT("EditPwd")));
	m_pEditPassword->SetMaxChar(LEN_PASSWORD-1);
	m_pEditPassword->SetBkColor(0);
	m_pEditPassword->SetPasswordMode(true);
	m_pEditPassword->SetFocus();
}

//消息提醒
void CDlgInputPassword::Notify(TNotifyUI &  msg)
{
	//获取对象
	CControlUI * pControlUI = msg.pSender;

	//点击事件
	if (lstrcmp(msg.sType, TEXT("click")) == 0)
	{
		if(pControlUI==m_pbtOk )
		{
			return OnOK(); 
		}
		else if(pControlUI==m_pbtCancel|| pControlUI->GetName()==TEXT("ButtonClose")) 
		{
			return OnCancel(); 
		}		
	}
}

//获取密码
VOID  CDlgInputPassword::GetServerPasswd(TCHAR szServerPasswd[LEN_PASSWORD])
{
	lstrcpyn(szServerPasswd,m_szPassword,CountArray(m_szPassword));

	return;
}

//////////////////////////////////////////////////////////////////////////////////
