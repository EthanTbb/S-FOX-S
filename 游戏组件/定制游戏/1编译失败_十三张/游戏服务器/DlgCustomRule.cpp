#include "Stdafx.h"
#include "Resource.h"
#include "DlgCustomRule.h"

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CDlgCustomRule, CDialog)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CDlgCustomRule::CDlgCustomRule() : CDialog(IDD_CUSTOM_RULE)
{
	//设置变量
	m_CustomRule.cbTimeStartGame=30;
	m_CustomRule.cbTimeCallBanker=20;
	m_CustomRule.cbTimeSetChip=20;
	m_CustomRule.cbTimeRangeCard=60;
	m_CustomRule.cbTimeShowCard=10;

	return;
}

//析构函数
CDlgCustomRule::~CDlgCustomRule()
{
}

//配置函数
BOOL CDlgCustomRule::OnInitDialog()
{
	__super::OnInitDialog();

	//设置控件
	((CEdit *)GetDlgItem(IDC_TIME_START_GAME))->LimitText(2);
	((CEdit *)GetDlgItem(IDC_TIME_CALL_BANKER))->LimitText(2);
	((CEdit *)GetDlgItem(IDC_TIME_SET_CHIP))->LimitText(2);
	((CEdit *)GetDlgItem(IDC_TIME_RANGE_CARD))->LimitText(2);
	((CEdit *)GetDlgItem(IDC_TIME_SHOW_CARD))->LimitText(2);

	//更新参数
	FillDataToControl();

	return FALSE;
}

//确定函数
VOID CDlgCustomRule::OnOK() 
{ 
	//投递消息
	GetParent()->PostMessage(WM_COMMAND,MAKELONG(IDOK,0),0);

	return;
}

//取消消息
VOID CDlgCustomRule::OnCancel() 
{ 
	//投递消息
	GetParent()->PostMessage(WM_COMMAND,MAKELONG(IDCANCEL,0),0);

	return;
}

//更新控件
bool CDlgCustomRule::FillDataToControl()
{
	//设置数据
	
	SetDlgItemInt(IDC_TIME_START_GAME,m_CustomRule.cbTimeStartGame);
	SetDlgItemInt(IDC_TIME_CALL_BANKER,m_CustomRule.cbTimeCallBanker);
	SetDlgItemInt(IDC_TIME_SET_CHIP,m_CustomRule.cbTimeSetChip);
	SetDlgItemInt(IDC_TIME_RANGE_CARD,m_CustomRule.cbTimeRangeCard);
	SetDlgItemInt(IDC_TIME_SHOW_CARD,m_CustomRule.cbTimeShowCard);

	return true;
}

//更新数据
bool CDlgCustomRule::FillControlToData()
{
	//设置数据
	m_CustomRule.cbTimeStartGame=(BYTE)GetDlgItemInt(IDC_TIME_START_GAME);
	m_CustomRule.cbTimeCallBanker=(BYTE)GetDlgItemInt(IDC_TIME_CALL_BANKER);
	m_CustomRule.cbTimeSetChip=(BYTE)GetDlgItemInt(IDC_TIME_SET_CHIP);
	m_CustomRule.cbTimeRangeCard=(BYTE)GetDlgItemInt(IDC_TIME_RANGE_CARD);
	m_CustomRule.cbTimeShowCard=(BYTE)GetDlgItemInt(IDC_TIME_SHOW_CARD);

	//开始时间
	if ((m_CustomRule.cbTimeStartGame<5)||(m_CustomRule.cbTimeStartGame>60))
	{
		AfxMessageBox(TEXT("开始时间设置范围错误，请重新设置！"),MB_ICONSTOP);
		return false;
	}

	//叫庄时间
	if ((m_CustomRule.cbTimeCallBanker<5)||(m_CustomRule.cbTimeCallBanker>60))
	{
		AfxMessageBox(TEXT("叫庄时间设置范围错误，请重新设置！"),MB_ICONSTOP);
		return false;
	}

	//下注时间
	if ((m_CustomRule.cbTimeSetChip<5)||(m_CustomRule.cbTimeSetChip>60))
	{
		AfxMessageBox(TEXT("下注时间设置范围错误，请重新设置！"),MB_ICONSTOP);
		return false;
	}

	//理牌时间
	if ((m_CustomRule.cbTimeRangeCard<20)||(m_CustomRule.cbTimeRangeCard>99))
	{
		AfxMessageBox(TEXT("理牌时间设置范围错误，请重新设置！"),MB_ICONSTOP);
		return false;
	}

	//开牌时间
	if ((m_CustomRule.cbTimeShowCard<10)||(m_CustomRule.cbTimeShowCard>20))
	{
		AfxMessageBox(TEXT("开牌时间设置范围错误，请重新设置！"),MB_ICONSTOP);
		return false;
	}

	return true;
}

//读取配置
bool CDlgCustomRule::GetCustomRule(tagCustomRule & CustomRule)
{
	//读取参数
	if (FillControlToData()==true)
	{
		CustomRule=m_CustomRule;
		return true;
	}

	return false;
}

//设置配置
bool CDlgCustomRule::SetCustomRule(tagCustomRule & CustomRule)
{
	//设置变量
	m_CustomRule=CustomRule;

	//更新参数
	if (m_hWnd!=NULL) FillDataToControl();

	return true;
}

//////////////////////////////////////////////////////////////////////////////////
