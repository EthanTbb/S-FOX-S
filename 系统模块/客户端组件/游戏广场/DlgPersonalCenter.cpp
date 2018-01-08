#include "Stdafx.h"
#include "Resource.h"
#include "DlgPersonalCenter.h"

//////////////////////////////////////////////////////////////////////////////////
//按钮控件
const TCHAR* const szButtonCloseControlName = TEXT("ButtonClose");
const TCHAR* const szButtonButtonHelpControlName = TEXT("ButtonButtonHelp");

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CDlgPersonalCenter::CDlgPersonalCenter() : CFGuiDialog(IDD_DLG_POPUP)
{
}

//析构函数
CDlgPersonalCenter::~CDlgPersonalCenter()
{
}

//创建函数
BOOL CDlgPersonalCenter::OnInitDialog()
{
	__super::OnInitDialog();

	//修改样式
	ModifyStyle(0, WS_MINIMIZEBOX|WS_MAXIMIZEBOX);

	//设置标题
	SetWindowText(TEXT("个人中心"));

	//居中窗口
	CenterWindow(this);


	return TRUE;
}

//确定函数
VOID CDlgPersonalCenter::OnOK()
{

	return __super::OnOK();
}

//取消消息
VOID CDlgPersonalCenter::OnCancel()
{
	return __super::OnCancel();
}

//消息提醒
void CDlgPersonalCenter::Notify(TNotifyUI &  msg)
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
	else if(msg.sType==_T("selectchanged"))
	{
		CTabLayoutUI* pControl = static_cast<CTabLayoutUI*>(GetControlByName(_T("TLayoutPersonal")));
		if(NULL != pControl) 
		{
			if (lstrcmp(pControlUI->GetName(), _T("OptionStandard")) == 0)
			{
				pControl->SelectItem(0);
			}
			else if (lstrcmp(pControlUI->GetName(), _T("OptionRealInfo")) == 0)
			{
				pControl->SelectItem(1);
			}
			else if (lstrcmp(pControlUI->GetName(), _T("OptionChangePW")) == 0)
			{
				pControl->SelectItem(2);
			}
		}
	}

	
	return;
}

//////////////////////////////////////////////////////////////////////////////////