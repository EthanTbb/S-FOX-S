#include "stdafx.h"
#include "Resource.h"
#include "DlgCollocateRoom.h"
#include "DlgInformation.h"

//////////////////////////////////////////////////////////////////////////////////
//控件定义

//按钮控件
const TCHAR * szButtonOKControlName=TEXT("ButtonOK");
const TCHAR * szButtonCloseControlName2=TEXT("ButtonClose");
const TCHAR * szButtonCancelControlName2=TEXT("ButtonCancel");

//编辑控件
const TCHAR * szEditWinRateControlName=TEXT("EditWinRate");
const TCHAR * szEditFleeRateControlName=TEXT("EditFleeRate");
const TCHAR * szEditScoreMinControlName=TEXT("EditScoreMin");
const TCHAR * szEditScoreMaxControlName=TEXT("EditScoreMax");

//复选控件
const TCHAR * szCheckOptionWinRateControlName=TEXT("CheckOptionWinRate");
const TCHAR * szCheckOptionFleeRateControlName=TEXT("CheckOptionFleeRate");
const TCHAR * szCheckOptionScoreRangeControlName=TEXT("CheckOptionScoreRange");
const TCHAR * szCheckOptionLimitDetestControlName=TEXT("CheckOptionLimitDetest");
const TCHAR * szCheckOptionLimitSameIPControlName=TEXT("CheckOptionLimitSameIP");

//密码控件
const TCHAR * szCheckOptionPwdControlName=TEXT("CheckOptionPwd");
const TCHAR * szEditPwdControlName=TEXT("EditPwd");

//////////////////////////////////////////////////////////////////////////////////

//房间设置
CDlgCollocateRoom::CDlgCollocateRoom() : CFGuiDialog(IDD_DLG_POPUP)
{
	//配置变量
	m_pParameterGame=NULL;
	m_pParameterServer=NULL;
}

CDlgCollocateRoom::~CDlgCollocateRoom()
{
}

//配置参数
bool CDlgCollocateRoom::InitCollocate(CParameterGame * pParameterGame, CParameterServer * pParameterServer)
{
	//设置变量
	m_pParameterGame=pParameterGame;
	m_pParameterServer=pParameterServer;

	//默认参数
	DefaultParameter();

	return true;
}

//初始控件
VOID CDlgCollocateRoom::InitControlUI()
{
	__super::InitControlUI();

	//变量定义
	CEditUI * pEditUIControl=NULL;

	//限制输入
	pEditUIControl = static_cast<CEditUI *>(GetControlByName(szEditWinRateControlName));
	if(pEditUIControl!=NULL) 
	{
		pEditUIControl->SetMaxChar(5);
		pEditUIControl->SetNumberOnly();
	}

	//限制输入
	pEditUIControl = static_cast<CEditUI *>(GetControlByName(szEditFleeRateControlName));
	if(pEditUIControl!=NULL) 
	{
		pEditUIControl->SetMaxChar(5);
		pEditUIControl->SetNumberOnly();
	}

	//限制输入
	pEditUIControl = static_cast<CEditUI *>(GetControlByName(szEditScoreMinControlName));
	if(pEditUIControl!=NULL) 
	{
		pEditUIControl->SetMaxChar(11);
		pEditUIControl->SetNumberOnly();
	}

	//限制输入
	pEditUIControl = static_cast<CEditUI *>(GetControlByName(szEditScoreMaxControlName));
	if(pEditUIControl!=NULL) 
	{
		pEditUIControl->SetMaxChar(10);
		pEditUIControl->SetNumberOnly();
	}
}

//消息提醒
VOID CDlgCollocateRoom::Notify(TNotifyUI &  msg)
{
	__super::Notify(msg);

	//获取对象
	CControlUI * pControlUI = msg.pSender;

	//点击事件
	if (lstrcmp(msg.sType, TEXT("click")) == 0)
	{
		if(lstrcmp(pControlUI->GetName(), szButtonOKControlName)==0)
		{
			return OnOK();
		}
		else if((lstrcmp(pControlUI->GetName(), szButtonCancelControlName2)==0)
			   ||(lstrcmp(pControlUI->GetName(), szButtonCloseControlName2)==0))
		{
			return OnCancel();
		}
	}
}

//初始化函数
BOOL CDlgCollocateRoom::OnInitDialog()
{
	__super::OnInitDialog();

	//获取位置
	CRect rcClient;
	GetClientRect(&rcClient);

	//设置标题
	SetWindowText(TEXT("房间设置"));

	//设置窗口
	CSize SizeWindow(m_PaintManager.GetInitSize());
	SetWindowPos(NULL, 0, 0, SizeWindow.cx, SizeWindow.cy, SWP_NOZORDER|SWP_NOMOVE);	

	//变量定义
	ASSERT(CParameterGlobal::GetInstance()!=NULL);
	CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();

	if ((m_pParameterServer!=NULL)&&(m_pParameterGame!=NULL))
	{
		//游戏配置
		m_wMinWinRate=m_pParameterGame->m_wMinWinRate;
		m_wMaxFleeRate=m_pParameterGame->m_wMaxFleeRate;
		m_bLimitWinRate=m_pParameterGame->m_bLimitWinRate;
		m_lMaxGameScore=m_pParameterGame->m_lMaxGameScore;
		m_lMinGameScore=m_pParameterGame->m_lMinGameScore;
		m_bLimitFleeRate=m_pParameterGame->m_bLimitFleeRate;
		m_bLimitGameScore=m_pParameterGame->m_bLimitGameScore;

		//其他配置
		m_bLimitDetest=pParameterGlobal->m_bLimitDetest;
		m_bLimitSameIP=pParameterGlobal->m_bLimitSameIP;

		m_bTakePassword = m_pParameterServer->m_bTakePassword;
		lstrcpyn(m_szTablePassword,m_pParameterServer->m_szPassword,CountArray(m_szTablePassword));
		
	}

	//更新控件
	UpdateControlStatus();

	return TRUE; 
}

//确定函数
VOID CDlgCollocateRoom::OnOK()
{
	//保存参数
	if(SaveParameter()==false)return;

	//变量定义
	ASSERT(CParameterGlobal::GetInstance()!=NULL);
	CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();

	//房间配置
	if ((m_pParameterServer!=NULL)&&(m_pParameterGame!=NULL))
	{
		//其他配置
		pParameterGlobal->m_bLimitDetest=m_bLimitDetest;
		pParameterGlobal->m_bLimitSameIP=m_bLimitSameIP;

		//游戏配置
		m_pParameterGame->m_wMinWinRate=m_wMinWinRate;
		m_pParameterGame->m_wMaxFleeRate=m_wMaxFleeRate;
		m_pParameterGame->m_lMaxGameScore=m_lMaxGameScore;
		m_pParameterGame->m_lMinGameScore=m_lMinGameScore;
		m_pParameterGame->m_bLimitWinRate=m_bLimitWinRate;
		m_pParameterGame->m_bLimitFleeRate=m_bLimitFleeRate;
		m_pParameterGame->m_bLimitGameScore=m_bLimitGameScore;

		m_pParameterServer->m_bTakePassword = m_bTakePassword;
		lstrcpyn(m_pParameterServer->m_szPassword,m_szTablePassword,CountArray(m_pParameterServer->m_szPassword));
	}

	//销毁窗口
	__super::OnOK();
}

//获取状态
bool CDlgCollocateRoom::GetButtonCheckStatus(LPCTSTR pszButtonName)
{
	//获取控件
	CCheckButtonUI * pControlUI= static_cast<CCheckButtonUI *>(GetControlByName(pszButtonName));
	if(pControlUI==NULL) return false;

	return pControlUI->GetCheck();
}

//获取字符
VOID CDlgCollocateRoom::GetControlText(LPCTSTR pszControlName,TCHAR szString[], WORD wMaxCount)
{
	//默认输出
	szString[0]=0;

	//获取控件
	CControlUI * pControlUI=GetControlByName(pszControlName);
	if(pControlUI==NULL) return;

	//获取字符
	CString strString(pControlUI->GetText());

	//去掉空格
	strString.TrimLeft();
	strString.TrimRight();

	//拷贝字符
	lstrcpyn(szString,strString,wMaxCount);

	return;
}

//保存参数
bool CDlgCollocateRoom::SaveParameter()
{
	//获取参数
	m_bLimitWinRate=GetButtonCheckStatus(szCheckOptionWinRateControlName);
	m_bLimitFleeRate=GetButtonCheckStatus(szCheckOptionFleeRateControlName);
	m_bLimitGameScore=GetButtonCheckStatus(szCheckOptionScoreRangeControlName);
	m_bLimitDetest=GetButtonCheckStatus(szCheckOptionLimitDetestControlName);
	m_bLimitSameIP=GetButtonCheckStatus(szCheckOptionLimitSameIPControlName);



	//变量定义
	TCHAR szBuffer[128]=TEXT("");
	CControlUI * pControlUI=NULL;

	//最低胜率	
	GetControlText(szEditWinRateControlName,szBuffer,CountArray(szBuffer));
	m_wMinWinRate=(WORD)(_tstof(szBuffer)*100.0)%10000;

	//最高逃率
	GetControlText(szEditFleeRateControlName,szBuffer,CountArray(szBuffer));
	m_wMaxFleeRate=(WORD)(_tstof(szBuffer)*100.0)%10000;

	//积分限制
	GetControlText(szEditScoreMaxControlName,szBuffer,CountArray(szBuffer));
	m_lMaxGameScore=(DWORD)_tstoi64(szBuffer);

	//积分限制
	GetControlText(szEditScoreMinControlName,szBuffer,CountArray(szBuffer));
	m_lMinGameScore=(DWORD)_tstoi64(szBuffer);

	if ((m_bLimitGameScore)&&(m_lMinGameScore>=m_lMaxGameScore))
	{
		//显示消息
		CDlgInformation Information(this);
		Information.ShowMessageBox(TEXT("积分限制范围参数不正确，请正确设置积分限制范围！"),MB_ICONINFORMATION);

		//设置焦点
		pControlUI=GetControlByName(szEditScoreMinControlName);
		pControlUI->SetFocus();

		return false;
	}

	//获取密码
	CEditUI * pEditPass1Control = (CEditUI *)GetControlByName(szEditPwdControlName);
	if(pEditPass1Control!=NULL) lstrcpyn(m_szTablePassword,pEditPass1Control->GetText(),CountArray(m_szTablePassword));

	//设置密码
	if(m_szTablePassword[0]!=0) m_bTakePassword = true;

	


	return true;
}

//默认参数
bool CDlgCollocateRoom::DefaultParameter()
{
	//胜率限制
	m_wMinWinRate=0;
	m_bLimitWinRate=false;

	//逃率限制
	m_wMaxFleeRate=5000;
	m_bLimitFleeRate=false;

	//积分限制
	m_bLimitGameScore=false;
	m_lMaxGameScore=2147483647L;
	m_lMinGameScore=-2147483646L;

	//其他配置
	m_bLimitDetest=false;
	m_bLimitSameIP=false;

	m_bTakePassword = false;
	ZeroMemory(m_szTablePassword,sizeof(m_szTablePassword));

	return true;
}

//更新控制
bool CDlgCollocateRoom::UpdateControlStatus()
{
	//变量定义
	TCHAR szBuffer[128]=TEXT("");
	CControlUI * pControlUI=NULL;

	//设置控件
	CCheckButtonUI * pCheckOption = static_cast<CCheckButtonUI *>(GetControlByName(szCheckOptionWinRateControlName));
	if(pCheckOption) pCheckOption->SetCheck(m_bLimitWinRate);

	//设置控件
	pCheckOption = static_cast<CCheckButtonUI *>(GetControlByName(szCheckOptionFleeRateControlName));
	if(pCheckOption) pCheckOption->SetCheck(m_bLimitFleeRate);

	//设置控件
	pCheckOption = static_cast<CCheckButtonUI *>(GetControlByName(szCheckOptionScoreRangeControlName));
	if(pCheckOption) pCheckOption->SetCheck(m_bLimitGameScore);

	//设置控件
	pCheckOption = static_cast<CCheckButtonUI *>(GetControlByName(szCheckOptionLimitDetestControlName));
	if(pCheckOption) pCheckOption->SetCheck(m_bLimitDetest);

	//设置控件
	pCheckOption = static_cast<CCheckButtonUI *>(GetControlByName(szCheckOptionLimitSameIPControlName));
	if(pCheckOption) pCheckOption->SetCheck(m_bLimitSameIP);

	//最低胜率	
	_sntprintf(szBuffer,CountArray(szBuffer),TEXT("%.2f"),((DOUBLE)(m_wMinWinRate))/100.0);
	pControlUI = GetControlByName(szEditWinRateControlName);
	if(pControlUI) pControlUI->SetText(szBuffer);

	//最高逃率
	_sntprintf(szBuffer,CountArray(szBuffer),TEXT("%.2f"),((DOUBLE)(m_wMaxFleeRate))/100.0);
	pControlUI = GetControlByName(szEditFleeRateControlName);
	if(pControlUI) pControlUI->SetText(szBuffer);

	//积分限制
	_sntprintf(szBuffer,CountArray(szBuffer),TEXT("%ld"),m_lMinGameScore);
	pControlUI = GetControlByName(szEditScoreMinControlName);
	if(pControlUI) pControlUI->SetText(szBuffer);

	//积分限制
	_sntprintf(szBuffer,CountArray(szBuffer),TEXT("%ld"),m_lMaxGameScore);
	pControlUI = GetControlByName(szEditScoreMaxControlName);
	if(pControlUI) pControlUI->SetText(szBuffer);

	//密码
	//pCheckOption = static_cast<CCheckButtonUI *>(GetControlByName(szCheckOptionPwdControlName));
	//if(pCheckOption) pCheckOption->SetCheck(m_bTakePassword);


	pControlUI = GetControlByName(szEditPwdControlName);
	if(pControlUI) 
	{
		pControlUI->SetText(m_szTablePassword);
		/*pControlUI->SetEnabled(m_bTakePassword);*/
	}



	return true;
}
