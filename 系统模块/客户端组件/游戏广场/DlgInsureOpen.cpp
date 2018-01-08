#include "Stdafx.h"
#include "Resource.h"
#include "DlgInsureOpen.h"
#include "DlgInsure.h"

//////////////////////////////////////////////////////////////////////////////////
//按钮控件
const TCHAR* const szButtonCloseControlName = TEXT("ButtonClose");
const TCHAR* const szButtonCancleControlName = TEXT("ButtonCancle");
const TCHAR* const szButtonComfirmControlName = TEXT("ButtonComfirm");



//////////////////////////////////////////////////////////////////////////////////

//构造函数
CDlgInsureOpen::CDlgInsureOpen() : CFGuiDialog(IDD_DLG_POPUP)
{
}

//析构函数
CDlgInsureOpen::~CDlgInsureOpen()
{
}

//创建函数
BOOL CDlgInsureOpen::OnInitDialog()
{
	__super::OnInitDialog();
	
	//修改样式
	ModifyStyle(0, WS_MINIMIZEBOX|WS_MAXIMIZEBOX);

	//居中窗口
	CenterWindow(this);


	return TRUE;
}

//确定函数
VOID CDlgInsureOpen::OnOK()
{
	return __super::OnOK();
}

//取消消息
VOID CDlgInsureOpen::OnCancel()
{
	return __super::OnCancel();
}


//消息提醒
void CDlgInsureOpen::Notify(TNotifyUI &  msg)
{
	//获取对象
	CControlUI * pControlUI = msg.pSender;

	//点击事件
	if (lstrcmp(msg.sType, TEXT("click")) == 0)
	{
		if((lstrcmp(pControlUI->GetName(), szButtonCloseControlName)==0) || 
			(lstrcmp(pControlUI->GetName(), szButtonCancleControlName)==0))
		{
			OnCancel();
		}
		else if(lstrcmp(pControlUI->GetName(), szButtonComfirmControlName)==0)
		{
			OnOK();
		}
	}
	
	return;
}

LPCTSTR CDlgInsureOpen::GetSkinFile()
{
	return TEXT("DlgInsureOpen.xml"); 
}


//////////////////////////////////////////////////////////////////////////////////