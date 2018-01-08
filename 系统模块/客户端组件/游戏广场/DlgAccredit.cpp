#include "Stdafx.h"
#include "DlgAccredit.h"
#include "MissionLogon.h"
#include "DlgInformation.h"

//////////////////////////////////////////////////////////////////////////////////
//号码定义
LPCTSTR g_pszPassPortID = TEXT("0123456789X");

//////////////////////////////////////////////////////////////////////////////////
//按钮控件
const TCHAR* const szButtonCloseControlName = TEXT("ButtonClose");
const TCHAR* const szButtonResetControlName = TEXT("ButtonReset");
const TCHAR* const szButtonNumberControlName= TEXT("ButtonNumber");
const TCHAR* const szButtonAccreditControlName = TEXT("ButtonAccredit");
const TCHAR* const szButtonBackSpaceControlName = TEXT("ButtonBackSpace");

//标签控件
const TCHAR* const szLabelPassPortIDControlName = TEXT("LabelPassPortID");

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CDlgAccredit::CDlgAccredit() : CFGuiDialog(IDD_DLG_SIMPLE)
{
}

//析构函数
CDlgAccredit::~CDlgAccredit()
{
}

//创建函数
BOOL CDlgAccredit::OnInitDialog()
{
	__super::OnInitDialog();

	//修改样式
	ModifyStyle(0, WS_MINIMIZEBOX|WS_MAXIMIZEBOX);

	//设置标题
	SetWindowText(TEXT("安全验证"));

	//居中窗口
	CenterWindow(this);

	//随机数字
	RandPassportNumber();

	return TRUE;
}

//确定函数
VOID CDlgAccredit::OnOK()
{
	//变量定义
	CString strPassPortID;
	TCHAR szDescribe[128]=TEXT("");
	CUserItemElement * pUserItemElement=CUserItemElement::GetInstance();

	//查找控件
	CControlUI * pLabelUI=GetControlByName(szLabelPassPortIDControlName);
	if(pLabelUI!=NULL) strPassPortID=pLabelUI->GetText();
	if(strPassPortID.IsEmpty()==true) return;

	//身份证号
	if (pUserItemElement->EfficacyPassPortID(strPassPortID,szDescribe,CountArray(szDescribe))==false)
	{
		//提示信息
		CDlgInformation Information(this);
		Information.ShowMessageBox(szDescribe,MB_ICONERROR,0);

		return;
	}
	
	return __super::OnOK();
}

//取消消息
VOID CDlgAccredit::OnCancel()
{
	return __super::OnCancel();
}

//消息提醒
void CDlgAccredit::Notify(TNotifyUI &  msg)
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
		if(lstrcmp(pControlUI->GetName(), szButtonAccreditControlName)==0)
		{
			return OnOK();
		}
		else if(lstrcmp(pControlUI->GetName(), szButtonBackSpaceControlName)==0) 
		{
			return OnClickedButtonBackSpace(pControlUI); 
		}
		else if(lstrcmp(pControlUI->GetName(), szButtonResetControlName)==0)
		{
			return OnClickedButtonReset(pControlUI);
		}
		else
		{
			if(lstrcmp(pControlUI->GetClass(), TEXT("ButtonUI"))==0) 
			{
				return OnClickedButtonNumber(pControlUI); 
			}
		}
	}
	
	return;
}

//获取号码
LPCTSTR CDlgAccredit::GetPassPortNumber()
{
	//查找控件
	CControlUI * pLabelUI=GetControlByName(szLabelPassPortIDControlName);
	if(pLabelUI!=NULL) return pLabelUI->GetText();

	return TEXT("");
}

//随机数字
VOID CDlgAccredit::RandPassportNumber()
{
	//变量定义
	TCHAR szPassPortID[12];
	lstrcpyn(szPassPortID,g_pszPassPortID,CountArray(szPassPortID));

	//变量定义
	TCHAR chaTrans;
	BYTE cbRandIndex=0;
	BYTE cbNumberCount=lstrlen(szPassPortID);

	//随机交换
	for(BYTE i=0;i<cbNumberCount;i++)
	{
		cbRandIndex=rand()%cbNumberCount;
		if(cbRandIndex!=i)
		{
			chaTrans=szPassPortID[i];
			szPassPortID[i]=szPassPortID[cbRandIndex];
			szPassPortID[cbRandIndex]=chaTrans;
		}
	}

	//变量定义
	TCHAR szNumberControlName[64]=TEXT("");
	TCHAR szControlText[2]=TEXT("");
	CControlUI * pControlUI=NULL;

	//设置文本
	for(BYTE i=0;i<cbNumberCount;i++)
	{
		_sntprintf(szNumberControlName,CountArray(szNumberControlName),TEXT("%s%d"),szButtonNumberControlName,i+1);
		pControlUI=GetControlByName(szNumberControlName);
        if(pControlUI!=NULL) 
		{
			szControlText[0]=szPassPortID[i];
			pControlUI->SetText(szControlText);
		}
	}
	 
	return;
}

//重置按钮
VOID CDlgAccredit::OnClickedButtonReset(CControlUI * pControlUI)
{
	CControlUI * pLabelUI=GetControlByName(szLabelPassPortIDControlName);
	if(pLabelUI!=NULL) pLabelUI->SetText(TEXT("")); 
}

//数组按钮
VOID CDlgAccredit::OnClickedButtonNumber(CControlUI * pControlUI)
{
	//参数校验
	ASSERT(pControlUI!=NULL);
	if(pControlUI==NULL) return;

	//变量定义
	CString strPassPortID;

	//查找控件
	CControlUI * pLabelUI = GetControlByName(szLabelPassPortIDControlName);
	if(pLabelUI!=NULL) strPassPortID = pLabelUI->GetText();

	//长度判断
	if(strPassPortID.GetLength() >= LEN_PASS_PORT_ID-1) return;

	//设置号码
	strPassPortID += pControlUI->GetText();
	pLabelUI->SetText(strPassPortID);

	return;
}

//退格按钮
VOID CDlgAccredit::OnClickedButtonBackSpace(CControlUI * pControlUI)
{
	//变量定义
	CString strPassPortID;

	//查找控件
	CControlUI * pLabelUI=GetControlByName(szLabelPassPortIDControlName);
	if(pLabelUI!=NULL) strPassPortID = pLabelUI->GetText();

	//设置号码
	strPassPortID.Delete(strPassPortID.GetLength()-1);
	pLabelUI->SetText(strPassPortID);

	return;
}
//////////////////////////////////////////////////////////////////////////////////