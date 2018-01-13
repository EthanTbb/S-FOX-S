// DlgCustomRule.cpp : 实现文件
//

#include "stdafx.h"
#include "Resource.h"
#include "DlgCustomRule.h"


// CDlgCustomRule 对话框

BEGIN_MESSAGE_MAP(CDlgCustomRule, CDialog)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_CUSTOM, OnTcnSelchangeTab)
END_MESSAGE_MAP()

CDlgCustomRule::CDlgCustomRule() : CDialog(IDD_CUSTOM_RULE)
{
}

CDlgCustomRule::~CDlgCustomRule()
{
}

void CDlgCustomRule::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

//初始化函数
BOOL CDlgCustomRule::OnInitDialog()
{
	__super::OnInitDialog();

	((CTabCtrl *)GetDlgItem(IDC_TAB_CUSTOM))->InsertItem(0,TEXT("通用功能"));
	((CTabCtrl *)GetDlgItem(IDC_TAB_CUSTOM))->InsertItem(1,TEXT("机器人"));

	m_DlgCustomGeneral.Create(IDD_CUSTOM_GENERAL,GetDlgItem(IDC_TAB_CUSTOM)); 
	m_DlgCustomAndroid.Create(IDD_CUSTOM_ANDROID,GetDlgItem(IDC_TAB_CUSTOM)); 

	CRect rcTabClient;
	GetDlgItem(IDC_TAB_CUSTOM)->GetClientRect(rcTabClient);
	rcTabClient.top+=20;
	rcTabClient.bottom-=4; 
	rcTabClient.left+=4; 
	rcTabClient.right-=4; 
	m_DlgCustomGeneral.MoveWindow(rcTabClient);
	m_DlgCustomAndroid.MoveWindow(rcTabClient);

	m_DlgCustomGeneral.ShowWindow(TRUE);
	m_DlgCustomAndroid.ShowWindow(FALSE);
	((CTabCtrl *)GetDlgItem(IDC_TAB_CUSTOM))->SetCurSel(0);

	m_wGeneralErrorCount=0;
	m_wAndroidErrorCount=0;
	m_strErrorMessage.Empty();
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
//更新数据
BOOL CDlgCustomRule::UpdateConfigData(BOOL bSaveAndValidate)
{
	if(bSaveAndValidate)	//从控件读取数据到变量
	{
		m_DlgCustomGeneral.UpdateData(TRUE);
		m_DlgCustomAndroid.UpdateData(TRUE);

		CopyMemory(&m_CustomConfig.CustomGeneral, &m_DlgCustomGeneral.m_CustomGeneral, sizeof(tagCustomGeneral));
		CopyMemory(&m_CustomConfig.CustomAndroid, &m_DlgCustomAndroid.m_CustomAndroid, sizeof(tagCustomAndroid));

	}
	else					//拷贝变量值到控件显示
	{
		CopyMemory(&m_DlgCustomGeneral.m_CustomGeneral, &m_CustomConfig.CustomGeneral, sizeof(tagCustomGeneral));
		CopyMemory(&m_DlgCustomAndroid.m_CustomAndroid, &m_CustomConfig.CustomAndroid, sizeof(tagCustomAndroid));

		m_DlgCustomGeneral.UpdateData(FALSE);
		m_DlgCustomAndroid.UpdateData(FALSE);
	}

	return TRUE;
}

//设置配置
bool CDlgCustomRule::SetCustomRule(LPBYTE pcbCustomRule, WORD wCustomSize)
{
	//设置变量
	m_wCustomSize=wCustomSize;
	m_pcbCustomRule=pcbCustomRule;

	//配置变量
	ASSERT(m_pcbCustomRule);
	if( !m_pcbCustomRule ) return false;
	tagCustomConfig *pCustomConfig = (tagCustomConfig *)m_pcbCustomRule;
	//if( pCustomConfig->lApplyBankerCondition != 0 )
	memcpy(&m_CustomConfig, pCustomConfig, sizeof(tagCustomConfig));

	//更新界面
	if( m_hWnd )
		UpdateConfigData(FALSE);  //拷贝变量值到控件显示

	return true;
}

//保存数据
bool CDlgCustomRule::SaveCustomRule(LPBYTE pcbCustomRule, WORD wCustomSize)
{
	//更新界面
	if( m_hWnd )
	{
		//保存前先记录
		tagCustomConfig tempCustomConfig;
		CopyMemory(&tempCustomConfig,&m_CustomConfig,sizeof(m_CustomConfig));
		UpdateConfigData(TRUE); //从控件读取数据到变量
		//如果数据错误,还原到保存前的数据
		if(CheckData() == false)
		{
			CopyMemory(&m_CustomConfig,&tempCustomConfig,sizeof(m_CustomConfig));
		}
	}

	//设置变量
	m_wCustomSize=wCustomSize;
	m_pcbCustomRule=pcbCustomRule;

	//配置变量
	ASSERT(m_pcbCustomRule);
	if( !m_pcbCustomRule ) return true;
	tagCustomConfig *pCustomConfig = (tagCustomConfig *)m_pcbCustomRule;
	memcpy(pCustomConfig, &m_CustomConfig, sizeof(tagCustomConfig));

	return true;
}

//默认数据
bool CDlgCustomRule::DefaultCustomRule(LPBYTE pcbCustomRule, WORD wCustomSize)
{
	//设置变量
	m_wCustomSize=wCustomSize;
	m_pcbCustomRule=pcbCustomRule;

	//配置变量
	ASSERT(m_pcbCustomRule);
	if( !m_pcbCustomRule ) return true;
	tagCustomConfig *pCustomConfig = (tagCustomConfig *)m_pcbCustomRule;

	m_CustomConfig.DefaultCustomRule();
	memcpy(pCustomConfig, &m_CustomConfig, sizeof(tagCustomConfig));

	//更新界面
	if( m_hWnd )
		UpdateConfigData(FALSE); // 拷贝变量值到控件显示

	return true;
}
//机器人组件不存在时 删除机器人配置选项
void CDlgCustomRule::DeleteAndroid()
{
	((CTabCtrl *)GetDlgItem(IDC_TAB_CUSTOM))->DeleteItem(1);	
}
void CDlgCustomRule::OnTcnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult)
{   
	INT CurSel =((CTabCtrl *)GetDlgItem(IDC_TAB_CUSTOM))->GetCurSel();

	switch(CurSel)
	{
	case 0:
		m_DlgCustomGeneral.ShowWindow(true);
		m_DlgCustomAndroid.ShowWindow(false);
		break;
	case 1:
		m_DlgCustomGeneral.ShowWindow(false);
		m_DlgCustomAndroid.ShowWindow(true);
		break;
	}
	*pResult = 0;

	return;
}

//数据校验(控件到数据)
bool CDlgCustomRule::CheckData()
{
	m_wGeneralErrorCount=0;
	m_wAndroidErrorCount=0;
	m_strErrorMessage.Empty();
//m_DlgCustomGeneral通用功能错误校验//////////////////////////////////////////////////////////////////////////////////////////////////////
	m_strErrorMessage+=TEXT("通用功能数据设置出错:");
	
	//上庄控制
	FormatDataCheck(m_CustomConfig.CustomGeneral.lApplyBankerCondition,	1000,	LLONG_MAX,	TEXT("上庄条件"),	true);
	FormatDataCheck(m_CustomConfig.CustomGeneral.lBankerTime,			1,		100,		TEXT("坐庄局数"),	true);
	FormatDataCheck(m_CustomConfig.CustomGeneral.lBankerTimeAdd,		1,		100,		TEXT("二次坐庄"),	true);
	FormatDataCheck(m_CustomConfig.CustomGeneral.lBankerScoreMAX,		1000,	LLONG_MAX,	TEXT("三次条件"),	true);
	FormatDataCheck(m_CustomConfig.CustomGeneral.lBankerTimeExtra,		1,		100,		TEXT("三次坐庄"),	true);
	//时间控制
	FormatDataCheck(m_CustomConfig.CustomGeneral.cbFreeTime,			5,		99,			TEXT("空闲时间"),	true);
	FormatDataCheck(m_CustomConfig.CustomGeneral.cbBetTime,				10,		99,			TEXT("下注时间"),	true);
	FormatDataCheck(m_CustomConfig.CustomGeneral.cbEndTime,				20,		99,			TEXT("结束时间"),	true);
	FormatDataCheck(m_CustomConfig.CustomGeneral.cbGuessTime,			15,		99,			TEXT("猜大小时间"),	true);
	//下注控制
	FormatDataCheck(m_CustomConfig.CustomGeneral.lAreaLimitScore,		10000,	LLONG_MAX,	TEXT("区域限制"),	true);
	FormatDataCheck(m_CustomConfig.CustomGeneral.lUserLimitScore,		10000,	LLONG_MAX,	TEXT("个人限制"),	true);
	FormatDataCheck(m_CustomConfig.CustomGeneral.lEndGameMul,			1,		10000000,	TEXT("兑换比例"),true);

	//库存控制
	FormatDataCheck(m_CustomConfig.CustomGeneral.StorageStart,			0,		LLONG_MAX,	TEXT("库存起始值"),	true);
	FormatDataCheck(m_CustomConfig.CustomGeneral.StorageDeduct,			0,		100,		TEXT("库存衰减值"),	true);
	FormatDataCheck(m_CustomConfig.CustomGeneral.StorageMax1,			10,		LLONG_MAX,	TEXT("库存上限值1"),true);
	FormatDataCheck(m_CustomConfig.CustomGeneral.StorageMul1,			0,		100,		TEXT("赢分概率1"),	true);
	FormatDataCheck(m_CustomConfig.CustomGeneral.StorageMax2,			20,		LLONG_MAX,	TEXT("库存上限值2"),true);
	FormatDataCheck(m_CustomConfig.CustomGeneral.StorageMul2,			0,		100,		TEXT("个人限制"),	true);

	if(m_CustomConfig.CustomGeneral.StorageMax2 < m_CustomConfig.CustomGeneral.StorageMax1)
	{
		m_strErrorMessage+=TEXT("\n  库存上限值2 比 库存上限值1 小，请重新设置");
		m_wGeneralErrorCount++;
	}

	if(m_wGeneralErrorCount == 0)  m_strErrorMessage.Empty();

//机器人数据设置检测//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	CString strBuf = m_strErrorMessage;
	if(m_strErrorMessage.IsEmpty() == false)
		m_strErrorMessage+=TEXT("\n机器人数据设置出错:");
	else
		m_strErrorMessage+=TEXT("机器人数据设置出错:");

	FormatDataCheck(m_CustomConfig.CustomAndroid.lRobotBankerCountMin,		1,		99,			TEXT("坐庄次数最小"),		false);
	FormatDataCheck(m_CustomConfig.CustomAndroid.lRobotBankerCountMax,		1,		99,			TEXT("坐庄次数最大"),		false);
	FormatDataCheck(m_CustomConfig.CustomAndroid.lRobotListMinCount,		1,		10,			TEXT("列表最低人数"),		false);
	FormatDataCheck(m_CustomConfig.CustomAndroid.lRobotListMaxCount,		1,		10,			TEXT("列表最高人数"),		false);
	FormatDataCheck(m_CustomConfig.CustomAndroid.lRobotApplyBanker,			1,		10,			TEXT("最多申请个数"),		false);
	FormatDataCheck(m_CustomConfig.CustomAndroid.lRobotWaitBanker,			1,		10,			TEXT("空盘重申局数"),		false);

	FormatDataCheck(m_CustomConfig.CustomAndroid.lRobotMinBetTime,			1,		50,			TEXT("筹码个数最少"),		false);
	FormatDataCheck(m_CustomConfig.CustomAndroid.lRobotMaxBetTime,			1,		50,			TEXT("筹码个数最多"),		false);
	if(m_CustomConfig.CustomAndroid.lRobotMaxBetTime<m_CustomConfig.CustomAndroid.lRobotMinBetTime)
	{
		m_strErrorMessage+=TEXT("\n  筹码个数最多 设置的值比 筹码个数最少 小，请重新设置");
		m_wAndroidErrorCount++;
	}
	FormatDataCheck(m_CustomConfig.CustomAndroid.lRobotMinJetton,			1,		100000000,	TEXT("筹码金额最小"),		false);
	FormatDataCheck(m_CustomConfig.CustomAndroid.lRobotMaxJetton,			1,		100000000,	TEXT("筹码金额最大"),		false);
	if(m_CustomConfig.CustomAndroid.lRobotMaxJetton<m_CustomConfig.CustomAndroid.lRobotMinJetton)
	{
		m_strErrorMessage+=TEXT("\n  筹码金额最大 设置的值比 筹码金额最小 小，请重新设置");
		m_wAndroidErrorCount++;
	}
	FormatDataCheck(m_CustomConfig.CustomAndroid.lRobotBetMinCount,			1,		255,		TEXT("下注人数最少"),		false);
	FormatDataCheck(m_CustomConfig.CustomAndroid.lRobotBetMaxCount,			1,		255,		TEXT("下注人数最多"),		false);
	if(m_CustomConfig.CustomAndroid.lRobotBetMaxCount<m_CustomConfig.CustomAndroid.lRobotBetMinCount)
	{
		m_strErrorMessage+=TEXT("\n  下注人数最多 设置的值比 下注人数最少 小，请重新设置");
		m_wAndroidErrorCount++;
	}
	FormatDataCheck(m_CustomConfig.CustomAndroid.lRobotAreaLimit,			100000,	LLONG_MAX,	TEXT("下注区域限制"),		false);

	FormatDataCheck(m_CustomConfig.CustomAndroid.lRobotScoreMin,			1000,	LLONG_MAX,	TEXT("机器人分数最小值"),	false);
	FormatDataCheck(m_CustomConfig.CustomAndroid.lRobotScoreMax,			1000,	LLONG_MAX,	TEXT("机器人分数最大值"),	false);
	if(m_CustomConfig.CustomAndroid.lRobotScoreMax<m_CustomConfig.CustomAndroid.lRobotScoreMin)
	{
		m_strErrorMessage+=TEXT("\n  机器人分数最大值 设置的值比 机器人分数最小值 小，请重新设置");
		m_wAndroidErrorCount++;
	}
	FormatDataCheck(m_CustomConfig.CustomAndroid.lRobotBankGetMin,			1000,	LLONG_MAX,	TEXT("机器人取款最小值"),	false);
	FormatDataCheck(m_CustomConfig.CustomAndroid.lRobotBankGetMax,			1000,	LLONG_MAX,	TEXT("机器人取款最大值"),	false);
	if(m_CustomConfig.CustomAndroid.lRobotBankGetMax<m_CustomConfig.CustomAndroid.lRobotBankGetMin)
	{
		m_strErrorMessage+=TEXT("\n  机器人取款最大值 设置的值比 机器人取款最小值 小，请重新设置");
		m_wAndroidErrorCount++;
	}
	FormatDataCheck(m_CustomConfig.CustomAndroid.lRobotBankGetBankerMin,	1000,	LLONG_MAX,	TEXT("庄家取款最小值"),		false);
	FormatDataCheck(m_CustomConfig.CustomAndroid.lRobotBankGetBankerMax,	1000,	LLONG_MAX,	TEXT("庄家取款最大值"),		false);
	if(m_CustomConfig.CustomAndroid.lRobotBankGetBankerMax<m_CustomConfig.CustomAndroid.lRobotBankGetBankerMin)
	{
		m_strErrorMessage+=TEXT("\n  庄家取款最大值 设置的值比 庄家取款最小值 小，请重新设置");
		m_wAndroidErrorCount++;
	}
	FormatDataCheck(m_CustomConfig.CustomAndroid.lRobotBankStoMul,			1,		99,			TEXT("存款百分比"),			false);


	if(m_wAndroidErrorCount == 0) m_strErrorMessage = strBuf;
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	if(m_wAndroidErrorCount+m_wGeneralErrorCount>8)
		m_strErrorMessage.Format(TEXT("你是故意的吧，设置的数据居然有超过8个是错误的。请务必仔细重新设置，这不好玩，真的！"));
	if(m_strErrorMessage.IsEmpty() == false)
	{
		MessageBox(m_strErrorMessage);
		return false;
	}
		
	return true;		
}

//通用校验
void CDlgCustomRule::FormatDataCheck(LONGLONG lCheckData,LONGLONG lMindata,LONGLONG lMaxdata,LPCTSTR strDataName,bool IsGeneralData)
{
	if(lCheckData<lMindata || lCheckData>lMaxdata)
	{
		CString strBuf;
		strBuf.Format(TEXT("\n  %s参数设置错误，请输入%I64d-%I64d范围内的数据"),strDataName,lMindata,lMaxdata);
		m_strErrorMessage+=strBuf;

		if(IsGeneralData)
			m_wGeneralErrorCount++;
		else
			m_wAndroidErrorCount++;
	}
}

//配置校验
bool CDlgCustomRule::CheckCustomRule()
{
	if(m_CustomConfig.CustomGeneral.cbBetTime == 0 ||
	   m_CustomConfig.CustomGeneral.cbEndTime == 0 ||
	   m_CustomConfig.CustomGeneral.cbFreeTime ==0 ||
	   m_CustomConfig.CustomAndroid.lRobotAreaLimit==0)
	{
		return false;
	}

	return true;
}
//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CDlgCustomGeneral, CDialog)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//构造函数
CDlgCustomGeneral::CDlgCustomGeneral() : CDialog(IDD_CUSTOM_GENERAL)
{
}

//析构函数
CDlgCustomGeneral::~CDlgCustomGeneral()
{
}

//控件绑定
VOID CDlgCustomGeneral::DoDataExchange(CDataExchange* pDX)
{
	__super::DoDataExchange(pDX);

	DDX_Text(pDX,IDC_EDIT_GENERAL_1,m_CustomGeneral.lApplyBankerCondition);
	DDX_Text(pDX,IDC_EDIT_GENERAL_2,m_CustomGeneral.lBankerTime);
	DDX_Text(pDX,IDC_EDIT_GENERAL_3,m_CustomGeneral.lBankerTimeAdd);
	DDX_Text(pDX,IDC_EDIT_GENERAL_4,m_CustomGeneral.lBankerScoreMAX);
	DDX_Text(pDX,IDC_EDIT_GENERAL_5,m_CustomGeneral.lBankerTimeExtra);
	DDX_Text(pDX,IDC_EDIT_GENERAL_6,m_CustomGeneral.cbFreeTime);
	DDX_Text(pDX,IDC_EDIT_GENERAL_7,m_CustomGeneral.cbBetTime);
	DDX_Text(pDX,IDC_EDIT_GENERAL_8,m_CustomGeneral.cbEndTime);
	DDX_Text(pDX,IDC_EDIT_GENERAL_9,m_CustomGeneral.cbGuessTime);
	
	DDX_Text(pDX,IDC_EDIT_GENERAL_10,m_CustomGeneral.lAreaLimitScore);
	DDX_Text(pDX,IDC_EDIT_GENERAL_11,m_CustomGeneral.lUserLimitScore);
	DDX_Text(pDX,IDC_EDIT_GENERAL_12,m_CustomGeneral.lEndGameMul);

	DDX_Text(pDX,IDC_EDIT_GENERAL_13,m_CustomGeneral.szMessageItem1,64);
	DDX_Text(pDX,IDC_EDIT_GENERAL_14,m_CustomGeneral.szMessageItem2,64);
	DDX_Text(pDX,IDC_EDIT_GENERAL_15,m_CustomGeneral.szMessageItem3,64);

	DDX_Text(pDX,IDC_EDIT_GENERAL_16,m_CustomGeneral.StorageStart);
	DDX_Text(pDX,IDC_EDIT_GENERAL_17,m_CustomGeneral.StorageDeduct);
	DDX_Text(pDX,IDC_EDIT_GENERAL_18,m_CustomGeneral.StorageMax1);
	DDX_Text(pDX,IDC_EDIT_GENERAL_19,m_CustomGeneral.StorageMul1);
	DDX_Text(pDX,IDC_EDIT_GENERAL_20,m_CustomGeneral.StorageMax2);
	DDX_Text(pDX,IDC_EDIT_GENERAL_21,m_CustomGeneral.StorageMul2);

	DDX_Check(pDX,IDC_CHECK_GENERAL_1,m_CustomGeneral.nEnableSysBanker);

	DDV_MaxChars(pDX,m_CustomGeneral.szMessageItem1,64);
	DDV_MaxChars(pDX,m_CustomGeneral.szMessageItem2,64);
	DDV_MaxChars(pDX,m_CustomGeneral.szMessageItem3,64);


}

//初始化函数
BOOL CDlgCustomGeneral::OnInitDialog()
{
	__super::OnInitDialog();

	return TRUE;
}


//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CDlgCustomAndroid, CDialog)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//构造函数
CDlgCustomAndroid::CDlgCustomAndroid() : CDialog(IDD_CUSTOM_ANDROID)
{
}

//析构函数
CDlgCustomAndroid::~CDlgCustomAndroid()
{
}

//控件绑定
VOID CDlgCustomAndroid::DoDataExchange(CDataExchange* pDX)
{
	__super::DoDataExchange(pDX);

	DDX_Text(pDX,IDC_EDIT_ANDROID_1,m_CustomAndroid.lRobotBankerCountMin);
	DDX_Text(pDX,IDC_EDIT_ANDROID_2,m_CustomAndroid.lRobotBankerCountMax);
	DDX_Text(pDX,IDC_EDIT_ANDROID_3,m_CustomAndroid.lRobotListMinCount);
	DDX_Text(pDX,IDC_EDIT_ANDROID_4,m_CustomAndroid.lRobotListMaxCount);
	DDX_Text(pDX,IDC_EDIT_ANDROID_5,m_CustomAndroid.lRobotApplyBanker);
	DDX_Text(pDX,IDC_EDIT_ANDROID_6,m_CustomAndroid.lRobotWaitBanker);

	DDX_Text(pDX,IDC_EDIT_ANDROID_7,m_CustomAndroid.lRobotMinBetTime);
	DDX_Text(pDX,IDC_EDIT_ANDROID_8,m_CustomAndroid.lRobotMaxBetTime);
	DDX_Text(pDX,IDC_EDIT_ANDROID_9,m_CustomAndroid.lRobotMinJetton);
	DDX_Text(pDX,IDC_EDIT_ANDROID_10,m_CustomAndroid.lRobotMaxJetton);
	DDX_Text(pDX,IDC_EDIT_ANDROID_11,m_CustomAndroid.lRobotBetMinCount);
	DDX_Text(pDX,IDC_EDIT_ANDROID_12,m_CustomAndroid.lRobotBetMaxCount);
	DDX_Text(pDX,IDC_EDIT_ANDROID_13,m_CustomAndroid.lRobotAreaLimit);

	DDX_Text(pDX,IDC_EDIT_ANDROID_14,m_CustomAndroid.lRobotScoreMin);
	DDX_Text(pDX,IDC_EDIT_ANDROID_15,m_CustomAndroid.lRobotScoreMax);
	DDX_Text(pDX,IDC_EDIT_ANDROID_16,m_CustomAndroid.lRobotBankGetMin);
	DDX_Text(pDX,IDC_EDIT_ANDROID_17,m_CustomAndroid.lRobotBankGetMax);
	DDX_Text(pDX,IDC_EDIT_ANDROID_18,m_CustomAndroid.lRobotBankGetBankerMin);
	DDX_Text(pDX,IDC_EDIT_ANDROID_19,m_CustomAndroid.lRobotBankGetBankerMax);
	DDX_Text(pDX,IDC_EDIT_ANDROID_20,m_CustomAndroid.lRobotBankStoMul);
	DDX_Check(pDX,IDC_CHECK_ANDROID_1,m_CustomAndroid.nEnableRobotBanker);

}

//初始化函数
BOOL CDlgCustomAndroid::OnInitDialog()
{
	__super::OnInitDialog();

	return TRUE;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////
