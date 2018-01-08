#include "Stdafx.h"
#include "AndroidManager.h"
#include "PlatformFrame.h"
#include ".\platformframe.h"

//////////////////////////////////////////////////////////////////////////////////

//控制按钮
#define IDC_MIN						100									//最小按钮
#define IDC_MAX						101									//最大按钮
#define IDC_CLOSE					102									//关闭按钮

//控件标识
#define IDC_SYSTEM_TRAY_ICON		303									//任务图标

//常量定义
#define MIN_INOUTINTERVAL			30*60								//最小间隔
#define MAX_INOUTINTERVAL			12*3600								//最大间隔

//////////////////////////////////////////////////////////////////////////////////
//静态变量

CPlatformFrame * CPlatformFrame::m_pPlatformFrame=NULL;					//框架指针

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CPlatformFrame,CSkinDialog)

	//系统消息
	ON_WM_SIZE()
	ON_WM_CLOSE()
	ON_WM_TIMER()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_GETMINMAXINFO()
	ON_WM_SETTINGCHANGE()

	//自定消息
	ON_MESSAGE(WM_PLATFORM_EVENT, OnMessagePlatformEvent)

	ON_CBN_SELCHANGE(IDC_COMBO_GAME, OnCbnComboGameSelchange)
	ON_BN_CLICKED(IDC_BT_ADD, OnBnClickedBtAdd)
	ON_BN_CLICKED(IDC_BT_MODIFY, OnBnClickedBtModify)
	ON_BN_CLICKED(IDC_BT_DELETE, OnBnClickedBtDelete)
	ON_BN_CLICKED(IDC_BT_QUERY, OnBnClickedBtQuery)	

	ON_NOTIFY(LVN_DELETEITEM, IDC_LIST_PARAMETER, OnLvnListParameterDeleteitem)
	ON_NOTIFY(NM_CLICK, IDC_LIST_PARAMETER, OnHdnListParameterItemclick)
	ON_NOTIFY(LVN_KEYDOWN, IDC_LIST_PARAMETER, OnLvnKeydownListParameter)

	ON_WM_DESTROY()	
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CPlatformFrame::CPlatformFrame() : CSkinDialog(IDD_PLATFORM_FRAME)
{
	//设置变量
	m_bLogonSuccess=false;
	m_nSelectItem=-1;

	//任务组件
	m_MissionManager.InsertMissionItem(&m_MissionList);
	m_MissionManager.InsertMissionItem(&m_MissionLogon);
	m_MissionManager.InsertMissionItem(&m_MissionAndroid);

	//平台变量
	ASSERT(m_pPlatformFrame==NULL);
	if (m_pPlatformFrame==NULL) m_pPlatformFrame=this;

	//创建画刷
	m_brBackGround.CreateSolidBrush(RGB(215,223,228));

	return;
}

//析构函数
CPlatformFrame::~CPlatformFrame()
{
	//释放对象
	m_brBackGround.DeleteObject();	

	//平台变量
	ASSERT(m_pPlatformFrame==this);
	if (m_pPlatformFrame==this) m_pPlatformFrame=NULL;

	return;
}

//消息过虑
BOOL CPlatformFrame::PreTranslateMessage(MSG * pMsg)
{
	return __super::PreTranslateMessage(pMsg);
}

//命令函数
BOOL CPlatformFrame::OnCommand(WPARAM wParam, LPARAM lParam)
{
	//变量定义
	UINT nCommandID=LOWORD(wParam);

	//功能按钮
	switch (nCommandID)
	{
	case IDC_MIN:				//最小按钮
		{
			ShowWindow(SW_MINIMIZE);
			return TRUE;
		}
	case IDC_MAX:				//最大按钮
		{
			//更新界面
			RedrawWindow(NULL,NULL,RDW_ERASE|RDW_INVALIDATE|RDW_ERASENOW|RDW_UPDATENOW);

			return TRUE;
		}
	case IDC_CLOSE:				//关闭按钮
		{
			PostMessage(WM_CLOSE,0,0);
			return TRUE;
		}
	}

	//菜单命令
	switch (nCommandID)
	{
	case IDM_USER_LOGON:		//用户登录
		{
			m_MissionLogon.ShowLogon();

			return TRUE;
		}
	}
	
	return __super::OnCommand(wParam,lParam);	
}

//交换数据
VOID CPlatformFrame::DoDataExchange(CDataExchange* pDX)
{
	//绑定控件
	DDX_Control(pDX, IDC_COMBO_GAME, m_cbGameList);
	DDX_Control(pDX, IDC_COMBO_SERVER, m_cbServerList);
	DDX_Control(pDX, IDC_COMBO_ENTER_HOUR, m_cbEnterHour);
	DDX_Control(pDX, IDC_COMBO_ENTER_MINUTE, m_cbEnterMinute);
	DDX_Control(pDX, IDC_COMBO_ENTER_SECOND, m_cbEnterSecond);
	DDX_Control(pDX, IDC_COMBO_LEAVE_HOUR, m_cbLeaveHour);
	DDX_Control(pDX, IDC_COMBO_LEAVE_MINUTE, m_cbLeaveMinute);
	DDX_Control(pDX, IDC_COMBO_LEAVE_SECOND, m_cbLeaveSecond);

	//绑定控件
	DDX_Control(pDX, IDC_EDIT_ANDROID_COUNT, m_edtAndroidCount);
	DDX_Control(pDX, IDC_EDIT_TAKESCORE_MIN, m_edtTableScoreMin);
	DDX_Control(pDX, IDC_EDIT_TAKESCORE_MAX, m_edtTableScoreMax);
	DDX_Control(pDX, IDC_EDIT_ENTERINTERVAL_MIN, m_edtEnterIntervalMin);
	DDX_Control(pDX, IDC_EDIT_ENTERINTERVAL_MAX, m_edtEnterIntervalMax);
	DDX_Control(pDX, IDC_EDIT_LEAVEINTERVAL_MIN, m_edtLeaveIntervalMin);
	DDX_Control(pDX, IDC_EDIT_LEAVEINTERVAL_MAX, m_edtLeaveIntervalMax);
	DDX_Control(pDX, IDC_EDIT_SWITCHTABLE_MIN, m_edtSwitchTableMin);
	DDX_Control(pDX, IDC_EDIT_SWITCHTABLE_MAX, m_edtSwitchTableMax);
	DDX_Control(pDX, IDC_EDIT_COUNT_MEMBER0, m_edtCountMember0);
	DDX_Control(pDX, IDC_EDIT_COUNT_MEMBER1, m_edtCountMember1);
	DDX_Control(pDX, IDC_EDIT_COUNT_MEMBER2, m_edtCountMember2);
	DDX_Control(pDX, IDC_EDIT_COUNT_MEMBER3, m_edtCountMember3);
	DDX_Control(pDX, IDC_EDIT_COUNT_MEMBER4, m_edtCountMember4);
	DDX_Control(pDX, IDC_EDIT_COUNT_MEMBER5, m_edtCountMember5);

	//绑定控件
	DDX_Control(pDX, IDC_BT_QUERY, m_btQuery);
	DDX_Control(pDX, IDC_BT_ADD, m_btAddItem);
	DDX_Control(pDX, IDC_BT_MODIFY, m_btModifyItem);
	DDX_Control(pDX, IDC_BT_DELETE, m_btDeleteItem);

	//绑定控件
	DDX_Control(pDX, IDC_LIST_PARAMETER, m_ParemeterList);
    

	__super::DoDataExchange(pDX);
}

//完成通知
VOID CPlatformFrame::OnGameItemFinish()
{
}

//完成通知
VOID CPlatformFrame::OnGameKindFinish(WORD wKindID)
{
}

//更新通知
VOID CPlatformFrame::OnGameItemUpdateFinish()
{
}

//插入通知
VOID CPlatformFrame::OnGameItemInsert(CGameListItem * pGameListItem)
{
	//效验参数
	ASSERT(pGameListItem!=NULL);
	if (pGameListItem==NULL) return;

	//插入处理
	switch (pGameListItem->GetItemGenre())
	{
	case ItemGenre_Type:	//游戏种类
		{
			break;
		}
	case ItemGenre_Kind:	//游戏类型
		{
			//变量定义
			CGameKindItem * pGameKindItem=(CGameKindItem *)pGameListItem;
			CGameListItem * pParentListItem=pGameListItem->GetParentListItem();
			
			//插入新项
			m_cbGameList.SetItemData(m_cbGameList.AddString(pGameKindItem->m_GameKind.szKindName),pGameKindItem->m_GameKind.wKindID);

			break;
		}
	case ItemGenre_Node:	//游戏节点
		{
			break;
		}
	case ItemGenre_Page:	//定制子项
		{
			break;
		}
	case ItemGenre_Server:	//游戏房间
		{
			////变量定义
			//CGameListItem * pParentListItem=pGameListItem->GetParentListItem();
			//CGameServerItem * pGameServerItem=(CGameServerItem *)pGameListItem;

			////变量定义
			//tagServerListItem *pServerListItem=NULL;
			//WORD wKindID=pGameServerItem->m_GameServer.wKindID;
			//POSITION pos=m_ServerList.GetHeadPosition();

			////查找父项
			//while(pos!=NULL)
			//{
			//	pServerListItem=m_ServerList.GetNext(pos);
			//	if(pServerListItem && pServerListItem->pGameKindItem->m_GameKind.wKindID == wKindID)
			//	{
			//		break;
			//	}
			//}

			////插入新项
			//if (pServerListItem!=NULL)
			//{
			//	//插入处理
			//	TCHAR szTitle[64]=TEXT("");
			//	GetGameItemTitle(pGameServerItem,szTitle,CountArray(szTitle));

			//	//常规插入
			//	pServerListItem->ServerList.AddTail(pGameServerItem);
			//}

			break;
		}
	}
}

//更新通知
VOID CPlatformFrame::OnGameItemUpdate(CGameListItem * pGameListItem)
{
}

//删除通知
VOID CPlatformFrame::OnGameItemDelete(CGameListItem * pGameListItem)
{

}

//控件使能
VOID CPlatformFrame::OnEventEnableControls(BOOL bEnabled)
{
	//设置控件
	m_btQuery.EnableWindow(bEnabled);
	m_btAddItem.EnableWindow(bEnabled);
	m_btModifyItem.EnableWindow(bEnabled);
	m_btDeleteItem.EnableWindow(bEnabled);
}

//机器参数
VOID CPlatformFrame::OnEventAndroidParenter(WORD wSubCommdID,WORD wParameterCount,tagAndroidParameter * pAndroidParameter)
{
	//获取参数
	if(wSubCommdID==SUB_GP_GET_PARAMETER)
	{
		//变量定义
		int nItemIndex=-1;
		tagAndroidParameter * pTempParameter=NULL;
		tagAndroidParameter * pItemData=NULL;

		//清除子项
		m_ParemeterList.DeleteAllItems();

		//变量定义
		LVITEM lVItem;
		lVItem.mask = LVIF_TEXT; 
		lVItem.iSubItem = 0; 
		lVItem.pszText = TEXT(""); 

		//循环插入
		for(WORD wIndex=0;wIndex<wParameterCount;wIndex++)
		{
			//获取参数
			pTempParameter = &pAndroidParameter[wIndex];
			if(pTempParameter==NULL) continue;

			//插入子项.
			lVItem.iItem=wIndex;
			nItemIndex=m_ParemeterList.InsertItem(&lVItem);
			if(nItemIndex==-1) continue;

			//设置子项
			SetItemToParameterList(nItemIndex,pTempParameter);		

			//申请资源
			pItemData = new tagAndroidParameter;
			if(pItemData!=NULL)
			{
				//拷贝数据
				CopyMemory(pItemData,pTempParameter,sizeof(tagAndroidParameter));

				//设置数据
				m_ParemeterList.SetItemData(nItemIndex,(DWORD_PTR)pItemData);
			}
		}

		return;
	}

	//添加参数
	if(wSubCommdID==SUB_GP_ADD_PARAMETER)
	{
		//变量定义
		int nItemIndex=-1;

		//变量定义
		LVITEM lVItem;
		lVItem.mask = LVIF_TEXT;   
		lVItem.iSubItem = 0; 
		lVItem.pszText = TEXT(""); 

		//插入子项
		lVItem.iItem=m_ParemeterList.GetItemCount();
		nItemIndex=m_ParemeterList.InsertItem(&lVItem);

		//设置子项
		SetItemToParameterList(nItemIndex,pAndroidParameter);	

		//申请资源
		tagAndroidParameter * pItemData = new tagAndroidParameter;
		if(pItemData!=NULL)
		{
			//拷贝数据
			CopyMemory(pItemData,pAndroidParameter,sizeof(tagAndroidParameter));

			//设置数据
			m_ParemeterList.SetItemData(nItemIndex,(DWORD_PTR)pItemData);
		}

		return;
	}

	//修改参数
	if(wSubCommdID==SUB_GP_MODIFY_PARAMETER)
	{
		//变量定义
		int nItemIndex=-1;
		tagAndroidParameter * pItemData=NULL;

		//查找对象
		for(int nIndex=0;nIndex<m_ParemeterList.GetItemCount();nIndex++)
		{
			//获取数据
			tagAndroidParameter * pItemData = (tagAndroidParameter *)m_ParemeterList.GetItemData(nIndex);
			if(pItemData==NULL) continue;
			if(pAndroidParameter->dwBatchID==pItemData->dwBatchID)
			{
				//设置子项
				SetItemToParameterList(nIndex,pAndroidParameter);

				//拷贝数据
				CopyMemory(pItemData,pAndroidParameter,sizeof(tagAndroidParameter));

				break;
			}
		}		

		return;
	}

	//删除参数
	if(wSubCommdID==SUB_GP_DELETE_PARAMETER)
	{

		//查找对象
		for(int nIndex=0;nIndex<m_ParemeterList.GetItemCount();nIndex++)
		{
			tagAndroidParameter * pItemData = (tagAndroidParameter *)m_ParemeterList.GetItemData(nIndex);
			if(pItemData==NULL) continue;
			if(pAndroidParameter->dwBatchID==pItemData->dwBatchID)
			{
				//设置子项
				m_ParemeterList.DeleteItem(nIndex);

				break;
			}
		}		

		return;
	}

	return;
}

//初始化控件
VOID CPlatformFrame::InitControls()
{
	//输入限制
	m_edtAndroidCount.LimitText(3);
	m_edtTableScoreMin.LimitText(16);
	m_edtTableScoreMax.LimitText(16);
	m_edtSwitchTableMin.LimitText(4);
	m_edtSwitchTableMax.LimitText(4);
	m_edtEnterIntervalMin.LimitText(4);
	m_edtEnterIntervalMax.LimitText(4);
	m_edtLeaveIntervalMin.LimitText(4);
	m_edtLeaveIntervalMax.LimitText(4);
	m_edtCountMember0.LimitText(3);
	m_edtCountMember1.LimitText(3);
	m_edtCountMember2.LimitText(3);
	m_edtCountMember3.LimitText(3);
	m_edtCountMember4.LimitText(3);
	m_edtCountMember5.LimitText(3);

	//变量定义
	CString strItemText;

	//下拉控件
	for(BYTE cbIndex=0;cbIndex<24;cbIndex++)
	{
		strItemText.Format(TEXT("%d"),cbIndex);
		m_cbEnterHour.SetItemData(m_cbEnterHour.InsertString(cbIndex,strItemText),cbIndex);
		m_cbLeaveHour.SetItemData(m_cbLeaveHour.InsertString(cbIndex,strItemText),cbIndex);
	}

	//下拉控件
	for(BYTE cbIndex=0;cbIndex<60;cbIndex++)
	{
		strItemText.Format(TEXT("%d"),cbIndex);
		m_cbEnterMinute.SetItemData(m_cbEnterMinute.InsertString(cbIndex,strItemText),cbIndex);
		m_cbEnterSecond.SetItemData(m_cbEnterSecond.InsertString(cbIndex,strItemText),cbIndex);
		m_cbLeaveMinute.SetItemData(m_cbLeaveMinute.InsertString(cbIndex,strItemText),cbIndex);
		m_cbLeaveSecond.SetItemData(m_cbLeaveSecond.InsertString(cbIndex,strItemText),cbIndex);
	}

	//设置控件
	DWORD dwExstyle = m_ParemeterList.GetExtendedStyle();
	m_ParemeterList.SetExtendedStyle( dwExstyle|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|WS_EX_STATICEDGE );

	//列表控件
	m_ParemeterList.InsertColumn(0,TEXT("批次标识"),LVCFMT_CENTER,60);
	m_ParemeterList.InsertColumn(1,TEXT("机器数目"),LVCFMT_CENTER,60);
	m_ParemeterList.InsertColumn(2,TEXT("服务模式"),LVCFMT_CENTER,60);
	m_ParemeterList.InsertColumn(3,TEXT("进入时间"),LVCFMT_CENTER,60);
	m_ParemeterList.InsertColumn(4,TEXT("离开时间"),LVCFMT_CENTER,60);
	m_ParemeterList.InsertColumn(5,TEXT("携带最少分数"),LVCFMT_CENTER,80);
	m_ParemeterList.InsertColumn(6,TEXT("携带最大分数"),LVCFMT_CENTER,80);
	m_ParemeterList.InsertColumn(7,TEXT("进入最小间隔"),LVCFMT_CENTER,80);
	m_ParemeterList.InsertColumn(8,TEXT("进入最大间隔"),LVCFMT_CENTER,80);
	m_ParemeterList.InsertColumn(9,TEXT("离开最小间隔"),LVCFMT_CENTER,80);
	m_ParemeterList.InsertColumn(10,TEXT("离开最大间隔"),LVCFMT_CENTER,80);
	m_ParemeterList.InsertColumn(11,TEXT("换桌最小局数"),LVCFMT_CENTER,80);
	m_ParemeterList.InsertColumn(12,TEXT("换桌最大局数"),LVCFMT_CENTER,80);
	m_ParemeterList.InsertColumn(13,TEXT("普通会员"),LVCFMT_CENTER,60);
	m_ParemeterList.InsertColumn(14,TEXT("一级会员"),LVCFMT_CENTER,60);
	m_ParemeterList.InsertColumn(15,TEXT("二级会员"),LVCFMT_CENTER,60);
	m_ParemeterList.InsertColumn(16,TEXT("三级会员"),LVCFMT_CENTER,60);
	m_ParemeterList.InsertColumn(17,TEXT("四级会员"),LVCFMT_CENTER,60);
	m_ParemeterList.InsertColumn(18,TEXT("五级会员"),LVCFMT_CENTER,60);
}

//添加修改
VOID CPlatformFrame::AddModifyParameter(bool bModify)
{
	//变量定义
	TCHAR szMessage[128]=TEXT("");
	tagAndroidParameter AndroidParameter;
	ZeroMemory(&AndroidParameter,sizeof(AndroidParameter));

	//选择判断
	if(bModify==true && m_nSelectItem==-1) return;

	AndroidParameter.AndroidCountMember0 = GetDlgItemInt(IDC_EDIT_COUNT_MEMBER0);
	AndroidParameter.AndroidCountMember1 = GetDlgItemInt(IDC_EDIT_COUNT_MEMBER1);
	AndroidParameter.AndroidCountMember2 = GetDlgItemInt(IDC_EDIT_COUNT_MEMBER2);
	AndroidParameter.AndroidCountMember3 = GetDlgItemInt(IDC_EDIT_COUNT_MEMBER3);
	AndroidParameter.AndroidCountMember4 = GetDlgItemInt(IDC_EDIT_COUNT_MEMBER4);
	AndroidParameter.AndroidCountMember5 = GetDlgItemInt(IDC_EDIT_COUNT_MEMBER5);

	//机器数目
    AndroidParameter.dwAndroidCount = 
		AndroidParameter.AndroidCountMember0 + 
		AndroidParameter.AndroidCountMember1 +
		AndroidParameter.AndroidCountMember2 + 
		AndroidParameter.AndroidCountMember3 +
		AndroidParameter.AndroidCountMember4 +
		AndroidParameter.AndroidCountMember5;

	//统计机器人
	DWORD dwAndroidCount=0;
	tagAndroidParameter * pItemData=NULL;
	for(int nIndex=0;nIndex<m_ParemeterList.GetItemCount();nIndex++)
	{
		pItemData = (tagAndroidParameter *)m_ParemeterList.GetItemData(nIndex);
		if(pItemData!=NULL)
		{
			if(bModify==false || nIndex!=m_nSelectItem)
			{
				dwAndroidCount += pItemData->dwAndroidCount;
			}
		}
	}

	if(AndroidParameter.dwAndroidCount<=0 || AndroidParameter.dwAndroidCount+dwAndroidCount>MAX_ANDROID)
	{		
		_sntprintf(szMessage,CountArray(szMessage),TEXT("抱歉,每个房间的机器人数目最多为%d个,请重新输入！"),MAX_ANDROID);

		if (AndroidParameter.AndroidCountMember0<=0)
		{
			_sntprintf(szMessage,CountArray(szMessage),TEXT("抱歉,每个房间的机器人数不可设置空！"));
		}
		//消息提示
		CInformation Information;
		Information.ShowMessageBox(TEXT("提示"),szMessage);

		//设置焦点
		m_edtCountMember0.SetFocus();

		return;
	}

	if(AndroidParameter.AndroidCountMember0<0 || AndroidParameter.AndroidCountMember0+dwAndroidCount>MAX_ANDROID)
	{	
		_sntprintf(szMessage,CountArray(szMessage),TEXT("抱歉,每个房间房间的普通会员机器人数目最多为%d个,请重新输入！"),MAX_ANDROID);		
		CInformation Information;
		Information.ShowMessageBox(TEXT("提示"),szMessage);
		m_edtCountMember0.SetFocus();
		return;
	}   
	if(AndroidParameter.AndroidCountMember1<0 || AndroidParameter.AndroidCountMember1+dwAndroidCount>MAX_ANDROID)
	{		
		_sntprintf(szMessage,CountArray(szMessage),TEXT("抱歉,每个房间的一级会员机器人数目最多为%d个,请重新输入！"),MAX_ANDROID);
		CInformation Information;
		Information.ShowMessageBox(TEXT("提示"),szMessage);
		m_edtCountMember1.SetFocus();
		return;
	}  
	if(AndroidParameter.AndroidCountMember2<0 || AndroidParameter.AndroidCountMember2+dwAndroidCount>MAX_ANDROID)
	{		
		_sntprintf(szMessage,CountArray(szMessage),TEXT("抱歉,每个房间的二级会员机器人数目最多为%d个,请重新输入！"),MAX_ANDROID);
		CInformation Information;
		Information.ShowMessageBox(TEXT("提示"),szMessage);
		m_edtCountMember2.SetFocus();
		return;
	}  
	if(AndroidParameter.AndroidCountMember3<0 || AndroidParameter.AndroidCountMember3+dwAndroidCount>MAX_ANDROID)
	{		
		_sntprintf(szMessage,CountArray(szMessage),TEXT("抱歉,每个房间的三级会员机器人数目最多为%d个,请重新输入！"),MAX_ANDROID);
		CInformation Information;
		Information.ShowMessageBox(TEXT("提示"),szMessage);
		m_edtCountMember3.SetFocus();
		return;
	}  
	if(AndroidParameter.AndroidCountMember4<0 || AndroidParameter.AndroidCountMember4+dwAndroidCount>MAX_ANDROID)
	{		
		_sntprintf(szMessage,CountArray(szMessage),TEXT("抱歉,每个房间的四级会员机器人数目最多为%d个,请重新输入！"),MAX_ANDROID);
		CInformation Information;
		Information.ShowMessageBox(TEXT("提示"),szMessage);
		m_edtCountMember4.SetFocus();
		return;
	}  
	if(AndroidParameter.AndroidCountMember5<0 || AndroidParameter.AndroidCountMember5+dwAndroidCount>MAX_ANDROID)
	{		
		_sntprintf(szMessage,CountArray(szMessage),TEXT("抱歉,每个房间的五级会员机器人数目最多为%d个,请重新输入！"),MAX_ANDROID);
		CInformation Information;
		Information.ShowMessageBox(TEXT("提示"),szMessage);
		m_edtCountMember5.SetFocus();
		return;
	}  


	//服务模式
	AndroidParameter.dwServiceMode |= ((CButton *)GetDlgItem(IDC_CHECK_SERVICE_MODE1))->GetCheck()==BST_CHECKED?ANDROID_SIMULATE:0;
	AndroidParameter.dwServiceMode |= ((CButton *)GetDlgItem(IDC_CHECK_SERVICE_MODE2))->GetCheck()==BST_CHECKED?ANDROID_INITIATIVE:0;
	AndroidParameter.dwServiceMode |= ((CButton *)GetDlgItem(IDC_CHECK_SERVICE_MODE3))->GetCheck()==BST_CHECKED?ANDROID_PASSIVITY:0;
	if(AndroidParameter.dwServiceMode==0)
	{		
		//消息提示
		CInformation Information;
		Information.ShowMessageBox(TEXT("提示"),TEXT("请您至少勾选一种服务模式！"));

		return;
	}

	//进出时间
	AndroidParameter.dwEnterTime = GetDlgItemInt(IDC_COMBO_ENTER_HOUR)*3600+GetDlgItemInt(IDC_COMBO_ENTER_MINUTE)*60+GetDlgItemInt(IDC_COMBO_ENTER_SECOND);
	AndroidParameter.dwLeaveTime = GetDlgItemInt(IDC_COMBO_LEAVE_HOUR)*3600+GetDlgItemInt(IDC_COMBO_LEAVE_MINUTE)*60+GetDlgItemInt(IDC_COMBO_LEAVE_SECOND);

	//计算间隔
    DWORD dwIntervalTime=0;
	if(AndroidParameter.dwLeaveTime>=AndroidParameter.dwEnterTime)
	{
		dwIntervalTime=AndroidParameter.dwLeaveTime-AndroidParameter.dwEnterTime;
	}
	else
	{
		dwIntervalTime=AndroidParameter.dwLeaveTime+24*3600-AndroidParameter.dwEnterTime;
	}

	//间隔太短
	if(dwIntervalTime<MIN_INOUTINTERVAL)
	{	
		_sntprintf(szMessage,CountArray(szMessage),TEXT("抱歉,机器人离开的时间最少要比进入的时间晚%d分钟！"),MIN_INOUTINTERVAL/60);

		//消息提示
		CInformation Information;
		Information.ShowMessageBox(TEXT("提示"),szMessage);

		return;
	}	

	//间隔太长
	if(dwIntervalTime>MAX_INOUTINTERVAL)
	{	
		_sntprintf(szMessage,CountArray(szMessage),TEXT("抱歉,机器人离开和进入的时间必须在%d小时之内！"),MAX_INOUTINTERVAL/3600);

		//消息提示
		CInformation Information;
		Information.ShowMessageBox(TEXT("提示"),szMessage);

		return;
	}		

	//变量定义
	TCHAR szValue[32]=TEXT("");

	//最小分数
	GetDlgItemText(IDC_EDIT_TAKESCORE_MIN,szValue,CountArray(szValue));
	AndroidParameter.lTakeMinScore = _tcstoi64(szValue,NULL,10);

	//最大分数
	GetDlgItemText(IDC_EDIT_TAKESCORE_MAX,szValue,CountArray(szValue));
	AndroidParameter.lTakeMaxScore = _tcstoi64(szValue,NULL,10);

	//分数校验
	if(AndroidParameter.lTakeMaxScore-AndroidParameter.lTakeMinScore<0)
	{
		//消息提示
		CInformation Information;
		Information.ShowMessageBox(TEXT("提示"),TEXT("抱歉,机器人携带的最大分数不能低于最小分数！"));

		return;
	}

	//进入间隔
	AndroidParameter.dwEnterMinInterval=GetDlgItemInt(IDC_EDIT_ENTERINTERVAL_MIN);
	AndroidParameter.dwEnterMaxInterval=GetDlgItemInt(IDC_EDIT_ENTERINTERVAL_MAX);

	//间隔校验
	if(AndroidParameter.dwEnterMaxInterval<AndroidParameter.dwEnterMinInterval)
	{
		//消息提示
		CInformation Information;
		Information.ShowMessageBox(TEXT("提示"),TEXT("抱歉,机器人进入房间的最大间隔不能低于最小间隔！"));

		return;
	}

	//离开间隔
	AndroidParameter.dwLeaveMinInterval=GetDlgItemInt(IDC_EDIT_LEAVEINTERVAL_MIN);
	AndroidParameter.dwLeaveMaxInterval=GetDlgItemInt(IDC_EDIT_LEAVEINTERVAL_MAX);

	//间隔校验
	if(AndroidParameter.dwLeaveMaxInterval<AndroidParameter.dwLeaveMinInterval)
	{
		//消息提示
		CInformation Information;
		Information.ShowMessageBox(TEXT("提示"),TEXT("抱歉,机器人离开房间的最大间隔不能低于最小间隔！"));

		return;
	}

	//离开间隔
	AndroidParameter.dwSwitchMinInnings=GetDlgItemInt(IDC_EDIT_SWITCHTABLE_MIN);
	AndroidParameter.dwSwitchMaxInnings=GetDlgItemInt(IDC_EDIT_SWITCHTABLE_MAX);

	//局数校验
	if(AndroidParameter.dwSwitchMaxInnings<AndroidParameter.dwSwitchMinInnings)
	{
		//消息提示
		CInformation Information;
		Information.ShowMessageBox(TEXT("提示"),TEXT("抱歉,机器人换桌的最大局数不能小于最小局数！"));

		return;
	}

	if(bModify==false)
	{
		//数目判断
		if(m_ParemeterList.GetItemCount()>=MAX_BATCH)
		{
			_sntprintf(szMessage,CountArray(szMessage),TEXT("抱歉,每个房间最多只能添加%d批机器人参数！"),MAX_BATCH);

			CInformation Information;
			Information.ShowMessageBox(TEXT("提示"),szMessage);

			return;
		}

		//变量定义
		int nCurSel = m_cbServerList.GetCurSel();
		if(nCurSel==-1) return;		

		//获取数据
		WORD wServerID = (WORD)m_cbServerList.GetItemData(nCurSel);
		m_MissionAndroid.AddAndroidParameter(wServerID,&AndroidParameter);
	}
	else
	{
		if(m_nSelectItem==-1) return;
		tagAndroidParameter * pItemData = (tagAndroidParameter *)m_ParemeterList.GetItemData(m_nSelectItem);
		if(pItemData==NULL) return;

		//变量定义
		int nCurSel = m_cbServerList.GetCurSel();
		if(nCurSel==-1) return;		

		//获取数据
		WORD wServerID = (WORD)m_cbServerList.GetItemData(nCurSel);
		AndroidParameter.dwBatchID=pItemData->dwBatchID;
		m_MissionAndroid.ModifyAndroidParameter(wServerID,&AndroidParameter);
	}

	return;
}

//选中子项
VOID CPlatformFrame::SelectItem(int nIndex)
{
	//设置变量
	m_nSelectItem =nIndex;
	if(m_nSelectItem==-1) return;

	//获取数据
	tagAndroidParameter * pItemData = (tagAndroidParameter * )m_ParemeterList.GetItemData(m_nSelectItem); 
	if(pItemData==NULL) return;

	//机器数目
	SetDlgItemInt(IDC_EDIT_COUNT_MEMBER0,pItemData->AndroidCountMember0);
	SetDlgItemInt(IDC_EDIT_COUNT_MEMBER1,pItemData->AndroidCountMember1);
	SetDlgItemInt(IDC_EDIT_COUNT_MEMBER2,pItemData->AndroidCountMember2);
	SetDlgItemInt(IDC_EDIT_COUNT_MEMBER3,pItemData->AndroidCountMember3);
	SetDlgItemInt(IDC_EDIT_COUNT_MEMBER4,pItemData->AndroidCountMember4);
	SetDlgItemInt(IDC_EDIT_COUNT_MEMBER5,pItemData->AndroidCountMember5);

	//服务模式
	((CButton *)GetDlgItem(IDC_CHECK_SERVICE_MODE1))->SetCheck((pItemData->dwServiceMode&ANDROID_SIMULATE)?BST_CHECKED:BST_UNCHECKED);
	((CButton *)GetDlgItem(IDC_CHECK_SERVICE_MODE2))->SetCheck((pItemData->dwServiceMode&ANDROID_INITIATIVE)?BST_CHECKED:BST_UNCHECKED);
	((CButton *)GetDlgItem(IDC_CHECK_SERVICE_MODE3))->SetCheck((pItemData->dwServiceMode&ANDROID_PASSIVITY)?BST_CHECKED:BST_UNCHECKED);

	//进入小时
	int nHourIndex = pItemData->dwEnterTime/3600;
	m_cbEnterHour.SetCurSel(nHourIndex);

	//进入分钟
	int nMinuteIndex = (pItemData->dwEnterTime-nHourIndex*3600)/60;
	m_cbEnterMinute.SetCurSel(nMinuteIndex);

	//进入秒钟
	int nSecondIndex = pItemData->dwEnterTime-nHourIndex*3600-nMinuteIndex*60;
	m_cbEnterSecond.SetCurSel(nSecondIndex);

	//离开小时
	nHourIndex = pItemData->dwLeaveTime/3600;
	m_cbLeaveHour.SetCurSel(nHourIndex);

	//离开分钟
	nMinuteIndex = (pItemData->dwLeaveTime-nHourIndex*3600)/60;
	m_cbLeaveMinute.SetCurSel(nMinuteIndex);

	//离开秒钟
	nSecondIndex = pItemData->dwLeaveTime-nHourIndex*3600-nMinuteIndex*60;
	m_cbLeaveSecond.SetCurSel(nSecondIndex);

	//携带分数
	TCHAR szTakeScore[32]=TEXT("");
	_sntprintf(szTakeScore,CountArray(szTakeScore),TEXT("%I64d"),pItemData->lTakeMinScore);
	m_edtTableScoreMin.SetWindowText(szTakeScore);
	_sntprintf(szTakeScore,CountArray(szTakeScore),TEXT("%I64d"),pItemData->lTakeMaxScore);
	m_edtTableScoreMax.SetWindowText(szTakeScore);

	//进入间隔
	SetDlgItemInt(IDC_EDIT_ENTERINTERVAL_MIN,pItemData->dwEnterMinInterval);
	SetDlgItemInt(IDC_EDIT_ENTERINTERVAL_MAX,pItemData->dwEnterMaxInterval);
	
	//离开间隔
	SetDlgItemInt(IDC_EDIT_LEAVEINTERVAL_MIN,pItemData->dwLeaveMinInterval);
	SetDlgItemInt(IDC_EDIT_LEAVEINTERVAL_MAX,pItemData->dwLeaveMaxInterval);

	//换桌局数
	SetDlgItemInt(IDC_EDIT_SWITCHTABLE_MIN,pItemData->dwSwitchMinInnings);
	SetDlgItemInt(IDC_EDIT_SWITCHTABLE_MAX,pItemData->dwSwitchMaxInnings);
}

//设置子项
VOID CPlatformFrame::SetItemToParameterList(int nItemIndex,tagAndroidParameter * pAndroidParameter)
{
	//参数校验
	ASSERT(nItemIndex!=-1 && pAndroidParameter!=NULL);
	if(nItemIndex==-1 || pAndroidParameter==NULL) return;

	//变量定义
	TCHAR szItemValue[32]=TEXT("");

	//设置子项
	_sntprintf(szItemValue,CountArray(szItemValue),TEXT("%d"),pAndroidParameter->dwBatchID);
	m_ParemeterList.SetItemText(nItemIndex,0,szItemValue);

	//设置子项
	_sntprintf(szItemValue,CountArray(szItemValue),TEXT("%d"),pAndroidParameter->dwAndroidCount);
	m_ParemeterList.SetItemText(nItemIndex,1,szItemValue);

	//设置子项
	_sntprintf(szItemValue,CountArray(szItemValue),TEXT("%d"),pAndroidParameter->dwServiceMode);
	m_ParemeterList.SetItemText(nItemIndex,2,szItemValue);

	//设置子项
	_sntprintf(szItemValue,CountArray(szItemValue),TEXT("%d"),pAndroidParameter->dwEnterTime);
	m_ParemeterList.SetItemText(nItemIndex,3,szItemValue);

	//设置子项
	_sntprintf(szItemValue,CountArray(szItemValue),TEXT("%d"),pAndroidParameter->dwLeaveTime);
	m_ParemeterList.SetItemText(nItemIndex,4,szItemValue);

	//设置子项
	_sntprintf(szItemValue,CountArray(szItemValue),TEXT("%I64d"),pAndroidParameter->lTakeMinScore);
	m_ParemeterList.SetItemText(nItemIndex,5,szItemValue);

	//设置子项
	_sntprintf(szItemValue,CountArray(szItemValue),TEXT("%I64d"),pAndroidParameter->lTakeMaxScore);
	m_ParemeterList.SetItemText(nItemIndex,6,szItemValue);

	//设置子项
	_sntprintf(szItemValue,CountArray(szItemValue),TEXT("%d"),pAndroidParameter->dwEnterMinInterval);
	m_ParemeterList.SetItemText(nItemIndex,7,szItemValue);

	//设置子项
	_sntprintf(szItemValue,CountArray(szItemValue),TEXT("%d"),pAndroidParameter->dwEnterMaxInterval);
	m_ParemeterList.SetItemText(nItemIndex,8,szItemValue);

	//设置子项
	_sntprintf(szItemValue,CountArray(szItemValue),TEXT("%d"),pAndroidParameter->dwLeaveMinInterval);
	m_ParemeterList.SetItemText(nItemIndex,9,szItemValue);

	//设置子项
	_sntprintf(szItemValue,CountArray(szItemValue),TEXT("%d"),pAndroidParameter->dwLeaveMaxInterval);
	m_ParemeterList.SetItemText(nItemIndex,10,szItemValue);

	//设置子项
	_sntprintf(szItemValue,CountArray(szItemValue),TEXT("%d"),pAndroidParameter->dwSwitchMinInnings);
	m_ParemeterList.SetItemText(nItemIndex,11,szItemValue);

	//设置子项
	_sntprintf(szItemValue,CountArray(szItemValue),TEXT("%d"),pAndroidParameter->dwSwitchMaxInnings);
	m_ParemeterList.SetItemText(nItemIndex,12,szItemValue);

	//设置子项
	_sntprintf(szItemValue,CountArray(szItemValue),TEXT("%d"),pAndroidParameter->AndroidCountMember0);
	m_ParemeterList.SetItemText(nItemIndex,13,szItemValue);

	//设置子项
	_sntprintf(szItemValue,CountArray(szItemValue),TEXT("%d"),pAndroidParameter->AndroidCountMember1);
	m_ParemeterList.SetItemText(nItemIndex,14,szItemValue);

	//设置子项
	_sntprintf(szItemValue,CountArray(szItemValue),TEXT("%d"),pAndroidParameter->AndroidCountMember2);
	m_ParemeterList.SetItemText(nItemIndex,15,szItemValue);

	//设置子项
	_sntprintf(szItemValue,CountArray(szItemValue),TEXT("%d"),pAndroidParameter->AndroidCountMember3);
	m_ParemeterList.SetItemText(nItemIndex,16,szItemValue);

	//设置子项
	_sntprintf(szItemValue,CountArray(szItemValue),TEXT("%d"),pAndroidParameter->AndroidCountMember4);
	m_ParemeterList.SetItemText(nItemIndex,17,szItemValue);

	//设置子项
	_sntprintf(szItemValue,CountArray(szItemValue),TEXT("%d"),pAndroidParameter->AndroidCountMember5);
	m_ParemeterList.SetItemText(nItemIndex,18,szItemValue);

}

//调整控件
VOID CPlatformFrame::RectifyControl(INT nWidth, INT nHeight)
{
	//状态判断
	if ((nWidth==0)||(nHeight==0)) return;

	//移动准备
	HDWP hDwp=BeginDeferWindowPos(64);
	UINT uFlags=SWP_NOACTIVATE|SWP_NOCOPYBITS|SWP_NOZORDER;

	//控制按钮
	/*DeferWindowPos(hDwp,m_btMin,NULL,nWidth-88,1,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btClose,NULL,nWidth-32,1,0,0,uFlags|SWP_NOSIZE);*/

	//结束调整
	LockWindowUpdate();
	EndDeferWindowPos(hDwp);
	UnlockWindowUpdate();

	//更新界面
	RedrawWindow(NULL,NULL,RDW_ERASE|RDW_INVALIDATE);

	return;
}

//关闭消息
VOID CPlatformFrame::OnClose()
{
	//登录标识
	if(m_bLogonSuccess==true)
	{
		//选择提示
		CInformation Information;
		INT nRetCode = Information.ShowMessageBox(TEXT("系统提示"),TEXT("您确定要退出管理器吗？"),MB_YESNO);

		//暂时不退出
		if(nRetCode==IDNO) return;
	}

	//销毁创口
	DestroyWindow();

	__super::OnClose();
}

//销毁消息
VOID CPlatformFrame::OnDestroy()
{
	__super::OnDestroy();

	//删除子项
	m_ParemeterList.DeleteAllItems();

    //delete this;
}

//绘画背景
BOOL CPlatformFrame::OnEraseBkgnd(CDC * pDC)
{
	//获取位置
	CRect rcClient;
	GetClientRect(&rcClient);

	//填充区域
	pDC->SetBkMode(TRANSPARENT);
	pDC->FillSolidRect(rcClient,RGB(215,223,228));

	return TRUE;
}

//时间消息
VOID CPlatformFrame::OnTimer(UINT_PTR nIDEvent)
{
	__super::OnTimer(nIDEvent);
}

//位置消息
VOID CPlatformFrame::OnSize(UINT nType, INT cx, INT cy) 
{
	__super::OnSize(nType, cx, cy);

	//调整控件
	RectifyControl(cx,cy);

	return;
}


//位置信息
VOID CPlatformFrame::OnGetMinMaxInfo(MINMAXINFO * lpMMI)
{
	__super::OnGetMinMaxInfo(lpMMI);

	return;
}

//初始窗口
BOOL CPlatformFrame::OnInitDialog()
{
	__super::OnInitDialog();

	//设置窗口
	ModifyStyle(WS_CAPTION|WS_MAXIMIZE|WS_MINIMIZE, WS_MINIMIZEBOX|WS_MAXIMIZEBOX);

	//设置图标
	HINSTANCE hInstance=AfxGetInstanceHandle();
	SetIcon(LoadIcon(hInstance,MAKEINTRESOURCE(IDR_MAINFRAME)),TRUE);
	SetIcon(LoadIcon(hInstance,MAKEINTRESOURCE(IDR_MAINFRAME)),FALSE);

	//设置标题
	SetWindowText(TEXT("机器人管理器"));

	//设置接口
	m_MissionAndroid.SetAndroidOperateCenter(this);

	//设置回调
	CServerListData::GetInstance()->SetServerListDataSink(this);

	//初始化控件
	InitControls();

	//注册事件
	CPlatformEvent * pPlatformEvent=CPlatformEvent::GetInstance();
	if (pPlatformEvent!=NULL) pPlatformEvent->RegisterEventWnd(m_hWnd);

	//登录系统
	PostMessage(WM_COMMAND,IDM_USER_LOGON,0);

	return TRUE;  
}

//鼠标消息
VOID CPlatformFrame::OnLButtonDown(UINT nFlags, CPoint Point)
{
	__super::OnLButtonDown(nFlags,Point);

	return;
}

//鼠标消息
VOID CPlatformFrame::OnLButtonDblClk(UINT nFlags, CPoint Point)
{
	__super::OnLButtonDblClk(nFlags,Point);
	
	return;
}

//设置改变
VOID CPlatformFrame::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	__super::OnSettingChange(uFlags,lpszSection);

	return;
}


//事件消息
LRESULT CPlatformFrame::OnMessagePlatformEvent(WPARAM wParam, LPARAM lParam)
{
	//事件处理
	switch (wParam)
	{
	case EVENT_USER_LOGON:			//登录完成
		{
			//显示窗口
			ShowWindow(SW_SHOW);
			SetForegroundWindow();

			//设置变量
			m_bLogonSuccess=true;

			return 0L;
		}	
	}

	return 0L;
}

//选择改变
VOID CPlatformFrame::OnCbnComboGameSelchange()
{
	//获取索引
	int nSelectIndex = m_cbGameList.GetCurSel();
	if(nSelectIndex==-1) return;

	//类型标识
	WORD wKindID = (WORD)m_cbGameList.GetItemData(nSelectIndex);

	//移除子项
	while(m_cbServerList.GetCount()>0) m_cbServerList.DeleteString(0);

	//变量定义
	POSITION Position=NULL;
	CGameServerItem * pGameServerItem=NULL;
    do
	{
		pGameServerItem=m_ServerListData.EmunGameServerItem(Position);
		if(pGameServerItem!=NULL && pGameServerItem->m_GameServer.wKindID==wKindID)
		{
			m_cbServerList.SetItemData(m_cbServerList.AddString(pGameServerItem->m_GameServer.szServerName),pGameServerItem->m_GameServer.wServerID);
		}
	}
	while(Position!=NULL);

	//默认选择
	if(m_cbServerList.GetCount()>0) m_cbServerList.SetCurSel(0);

	//更新控件
	m_cbServerList.Invalidate();
}

//单击子项
VOID CPlatformFrame::OnHdnListParameterItemclick(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);

	//选中子项
	SelectItem(phdr->iItem);

	*pResult = 0;
}

//删除子项
VOID CPlatformFrame::OnLvnListParameterDeleteitem(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	//获取数据
	tagAndroidParameter * pItemData = (tagAndroidParameter * )m_ParemeterList.GetItemData(pNMLV->iItem); 
	if(pItemData!=NULL)
	{
		SafeDelete(pItemData);
	}

	*pResult = 0;
}

//键盘按下
VOID CPlatformFrame::OnLvnKeydownListParameter(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLVKEYDOWN pLVKeyDow = reinterpret_cast<LPNMLVKEYDOWN>(pNMHDR);

	//变量定义
	int nItem=-1;
	POSITION pos;

	switch(pLVKeyDow->wVKey) 
	{
	case VK_DOWN:
		{
			pos=m_ParemeterList.GetFirstSelectedItemPosition();
			while (pos) 
			{
				//获取子项
				nItem=m_ParemeterList.GetNextSelectedItem(pos);
				if(nItem==-1) break;

				//设置子项
				nItem = __min(nItem+1,m_ParemeterList.GetItemCount()-1);
				SelectItem(nItem);
				break;
			}

			break;
		}
	case VK_UP:
		{
			pos=m_ParemeterList.GetFirstSelectedItemPosition();
			while (pos) 
			{
				//获取子项
				nItem=m_ParemeterList.GetNextSelectedItem(pos);
				if(nItem==-1) break;

				//设置子项
				nItem = __max(nItem-1,0);
				SelectItem(nItem);
				break;
			}
			break;
		}
	}

	*pResult = 0;
}

//添加子项
VOID CPlatformFrame::OnBnClickedBtAdd()
{
	//添加子项
	AddModifyParameter(false);
}

//编辑子项
VOID CPlatformFrame::OnBnClickedBtModify()
{
	//添加子项
	AddModifyParameter(true);
}

//删除子项
VOID CPlatformFrame::OnBnClickedBtDelete()
{
	//获取索引
	if(m_nSelectItem==-1) return;
	if(m_ParemeterList.GetItemCount()<=m_nSelectItem)
	{
		m_nSelectItem=-1;
		return;
	}

	//获取索引
	int nIndex = m_cbServerList.GetCurSel();
	if(nIndex==-1) return;

	//获取数据
	tagAndroidParameter * pItemData = (tagAndroidParameter * )m_ParemeterList.GetItemData(m_nSelectItem); 
	if(pItemData==NULL) return;

	//获取时间
	SYSTEMTIME SystemTime;
	GetLocalTime(&SystemTime);
	DWORD dwTodayTickCount= SystemTime.wHour*3600+SystemTime.wMinute*60+SystemTime.wSecond;

	//时间判断
	if(dwTodayTickCount>=pItemData->dwEnterTime && dwTodayTickCount<=pItemData->dwLeaveTime)
	{
		//选择提示
		CInformation Information;
		INT nRetCode = Information.ShowMessageBox(TEXT("警告"),TEXT("该批机器人正处在服务状态,您确定要删除吗？"),MB_YESNO);

		//暂时不退出
		if(nRetCode==IDNO) return;
	}

	//获取数据
	WORD wServerID = (WORD)m_cbServerList.GetItemData(nIndex);

	//删除参数
	m_MissionAndroid.DeleteAndroidParameter(wServerID,pItemData->dwBatchID);

	//设置变量
	m_nSelectItem=-1;

	return;
}

//查询参数
VOID CPlatformFrame::OnBnClickedBtQuery()
{
	//获取索引
	int nIndex = m_cbServerList.GetCurSel();
	if(nIndex==-1) return;

	//获取数据
	WORD wServerID = (WORD)m_cbServerList.GetItemData(nIndex);

	//获取参数
	m_MissionAndroid.GetAndroidParameter(wServerID);

	return;
}

//////////////////////////////////////////////////////////////////////////////////
