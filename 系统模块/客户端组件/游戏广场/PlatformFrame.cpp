#include "Stdafx.h"
#include "GamePlaza.h"
#include "DlgEnquire.h"
#include "DlgSearchUser.h"
#include "PlatformFrame.h"
#include "DlgHelp.h"
#include "DlgRecharge.h"
#include "DlgShop.h"
#include "DlgFriend.h"
#include "DlgAddGame.h"
#include "DlgInformation.h"
//////////////////////////////////////////////////////////////////////////////////

//屏幕限制
#define LESS_LIST_CX				285									//列表宽度
#define SPLITTER_WIDTH				8									//拆分宽度

//屏幕限制
#define LESS_SCREEN_CY				740									//最小高度
#define LESS_SCREEN_CX				1024								//最小宽度

//////////////////////////////////////////////////////////////////////////////////

//控件标识
#define IDC_SERVER_LIST				300									//游戏列表
#define IDC_WEB_PUBLICIZE			301									//浏览控件
#define IDC_SKIN_SPLITTER			302									//拆分控件
#define IDC_SYSTEM_TRAY_ICON		303									//任务图标
#define IDC_PLAZA_VIEW_GAME_TYPE	304									//类型控件
#define IDC_PLAZA_VIEW_GAME			305									//游戏视图
#define IDC_PLAZA_GAME_CONTAINER	306									//游戏容器
#define IDC_PLAZA_SERVER_CONTAINER	307									//房间容器

//控件标识
#define IDC_PLAZA_VIEW_AFFICHE		310									//公告面板
#define IDC_PLAZA_VIEW_USER_INFO	311									//用户信息
#define IDC_PLAZA_VIEW_SERVER_CTRL	312									//房间控制	

//////////////////////////////////////////////////////////////////////////////////

//房间数目
#define MAX_SERVER_COUNT			5									//房间数目

//时间标识
#define IDI_FLASH_TRAY_ICON			10									//闪动图标
#define IDI_SHOW_GAME_AD			156									//显示活动

//时间标识
#define IDI_UPDATA_ONLINE			16									//更新人数
#define TIME_UPDATA_ONLINE			10000								//更新人数

//////////////////////////////////////////////////////////////////////////////////
//消息命令
#define WM_TASK_BAR					0x0313								//任务栏右键
#define WM_INSTALL_CANCEL_EVENT		8888								//取消安装


//////////////////////////////////////////////////////////////////////////////////

//控件定义

//按钮控件
const TCHAR* const szButtonCloseControlName = TEXT("ButtonClose");
const TCHAR* const szButtonResoreControlName = TEXT("ButtonResore");
const TCHAR* const szButtonMaxControlName = TEXT("ButtonMax");
const TCHAR* const szButtonMinControlName = TEXT("ButtonMin");
const TCHAR* const szButtonNavigation1ControlName = TEXT("ButtonNavigation1");
const TCHAR* const szButtonNavigation2ControlName = TEXT("ButtonNavigation2");
const TCHAR* const szButtonNavigation3ControlName = TEXT("ButtonNavigation3");
const TCHAR* const szButtonNavigation4ControlName = TEXT("ButtonNavigation4");

const TCHAR* const szButtonButtonMenu1ControlName = TEXT("ButtonMenu1");
const TCHAR* const szButtonButtonMenu2ControlName = TEXT("ButtonMenu2");
const TCHAR* const szButtonButtonMenu3ControlName = TEXT("ButtonMenu3");
const TCHAR* const szButtonButtonMenu4ControlName = TEXT("ButtonMenu4");
const TCHAR* const szButtonButtonMenu5ControlName = TEXT("ButtonMenu5");

//网页控件
const TCHAR* const szWebBrowserNoticeControlName = TEXT("WebBrowserNotice");
const TCHAR* const szWebBrowserUserRankControlName = TEXT("WebBrowserUserRank");
const TCHAR* const szWebBrowserBottomAdControlName = TEXT("WebBrowserBottomAd");
const TCHAR* const szWebBrowserMarqueeControlName = TEXT("WebBrowserMarquee");
//标题菜单

//按钮控件
const TCHAR* const szButtonMainControlName = TEXT("ButtonMain");
const TCHAR* const szButtonMySpreadControlName = TEXT("ButtonMySpread");
const TCHAR* const szButtonLockMachineControlName = TEXT("ButtonLockMachine");
const TCHAR* const szButtonHlepControlName = TEXT("ButtonHlep");
const TCHAR* const szButtonMunuControlName = TEXT("ButtonMunu");

//菜单控件
const TCHAR* const szMenuSettingControlName = TEXT("MenuSetting");
const TCHAR* const szMenuOnlineControlName = TEXT("MenuOnline");
const TCHAR* const szMenuContactControlName = TEXT("MenuContact");
const TCHAR* const szMenuSwitchControlName = TEXT("MenuSwitch");
const TCHAR* const szMenuReStoreControlName = TEXT("MenuReStore");
const TCHAR* const szMenuMinControlName = TEXT("MenuMin");
const TCHAR* const szMenuMaxControlName = TEXT("MenuMax");
const TCHAR* const szMenuExitControlName = TEXT("MenuExit");
const TCHAR* const szMenuShowMainControlName = TEXT("MenuShowMain");


//中间导航
const TCHAR* const szButtonMidRechargeControlName = TEXT("ButtonMidRecharge");
const TCHAR* const szButtonMidShopControlName = TEXT("ButtonMidShop");
const TCHAR* const szButtonMidBagControlName = TEXT("ButtonMidBag");
const TCHAR* const szButtonMidBankControlName = TEXT("ButtonMidBank");

//////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////
//静态变量

CPlatformFrame * CPlatformFrame::m_pPlatformFrame=NULL;					//框架指针

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CPlatformFrame, CFGuiFrameWnd)

	//系统消息
	ON_WM_SIZE()
	ON_WM_CLOSE()
	ON_WM_TIMER()
	ON_WM_CREATE()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_GETMINMAXINFO()
	ON_WM_SETTINGCHANGE()

	//自定消息
	ON_MESSAGE(WM_HOTKEY, OnMessageHotKey)
	ON_MESSAGE(WM_TASK_BAR, OnMessageTaskBar)	
	ON_MESSAGE(WM_TRAY_ICON, OnMessageTrayIcon)
	ON_MESSAGE(WM_PLATFORM_EVENT, OnMessagePlatformEvent)
	ON_MESSAGE(WM_INSTALL_CANCEL_EVENT, OnMessageInstallCancelEvent)

	//自定消息
	ON_MESSAGE(WM_INSUREPLAZA_EVENT, OnMessageInsurePlazaEvent)

END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CPlatformFrame::CPlatformFrame()
:m_PlazaHallList(&m_PaintManager)
,m_PlazaUserInfo(&m_PaintManager)
,m_PlazaHallGame(&m_PaintManager)
{
	//界面变量
	m_bMaxShow=false;
	m_bRectify=false;
	m_rcNormalSize.SetRect(0,0,0,0);

	//状态变量
	m_bWhisperIcon=false;
	m_bAutoMenuEnable=FALSE;

	//任务组件
	m_MissionManager.InsertMissionItem(&m_MissionList);
	m_MissionManager.InsertMissionItem(&m_MissionLogon);

	//平台变量
	ASSERT(m_pPlatformFrame==NULL);
	if (m_pPlatformFrame==NULL) m_pPlatformFrame=this;

	//设置接口
	CCustomFaceManager * pCustomFaceManager=CCustomFaceManager::GetInstance();
	pCustomFaceManager->SetCustomFaceEvent(QUERY_OBJECT_PTR_INTERFACE(this,IUnknownEx));

	m_wAVServerPort=0;
	m_dwAVServerAddr=0;

	m_pDlgEnquire=NULL;
	return;
}

//析构函数
CPlatformFrame::~CPlatformFrame()
{
	//平台变量
	ASSERT(m_pPlatformFrame==this);
	if (m_pPlatformFrame==this) m_pPlatformFrame=NULL;

	return;
}

//接口查询
VOID * CPlatformFrame::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(ICustomFaceEvent,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(ICustomFaceEvent,Guid,dwQueryVer);
	return NULL;
}

//下载失败
VOID CPlatformFrame::OnEventSystemFace(DWORD dwUserID, WORD wFaceID)
{
	//变量定义
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	//自我数据
	if (pGlobalUserData->dwUserID==dwUserID)
	{
		//设置变量
		pGlobalUserData->wFaceID=wFaceID;

		//清理数据
		pGlobalUserData->dwCustomID=0L;
		ZeroMemory(&pGlobalUserData->CustomFaceInfo,sizeof(pGlobalUserData->CustomFaceInfo));

		//发送事件
		CPlatformEvent * pPlatformEvent=CPlatformEvent::GetInstance();
		if (pPlatformEvent!=NULL) pPlatformEvent->SendPlatformEvent(EVENT_USER_INFO_UPDATE,0L);
	}

	return;
}

//头像数据
VOID CPlatformFrame::OnEventCustomFace(DWORD dwUserID, DWORD dwCustomID, tagCustomFaceInfo & CustomFaceInfo)
{
	//变量定义
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	//自我数据
	if (pGlobalUserData->dwUserID==dwUserID)
	{
		//设置变量
		pGlobalUserData->dwCustomID=dwCustomID;
		CopyMemory(&pGlobalUserData->CustomFaceInfo,&CustomFaceInfo,sizeof(CustomFaceInfo));

		//发送事件
		CPlatformEvent * pPlatformEvent=CPlatformEvent::GetInstance();
		if (pPlatformEvent!=NULL) pPlatformEvent->SendPlatformEvent(EVENT_USER_INFO_UPDATE,0L);
	}

	//当前房间
	if (m_PlazaViewServer.m_hWnd!=NULL)
	{
		m_PlazaViewServer.UpdateUserCustomFace(dwUserID,dwCustomID,CustomFaceInfo);
	}

	return;
}


//获取通知
VOID CPlatformFrame::OnGameItemFinish()
{
	//显示列表
	//m_PlazaViewGame.ShowKindItemView(m_PlazaViewGameType.GetCurrActiveIndex(),FALSE);

	m_PlazaHallList.ShowPlazaHallList();
	m_PlazaHallGame.ShowPlazaHallGame();

	return;
}

//完成通知
VOID CPlatformFrame::OnGameMatchFinish()
{
	////显示比赛
	//if(m_PlazaViewGame.GetShowItemMode()==VIEW_MODE_MATCH)
	//{
	//	m_PlazaViewGame.ShowMatchItemView(FALSE);
	//}

	return;
}

//获取通知
VOID CPlatformFrame::OnGameKindFinish(WORD wKindID)
{
	////显示房间
	//if(wKindID!=INVALID_WORD)
	//{
	//	m_PlazaViewGame.ShowServerItemView(wKindID,TRUE);
	//}

	return;
}

//插入通知
VOID CPlatformFrame::OnGameItemInsert(CGameListItem * pGameListItem)
{
	//效验参数
	ASSERT(pGameListItem!=NULL);
	if (pGameListItem==NULL) return;

	//更新时间
	pGameListItem->m_dwUpdateTime=(DWORD)time(NULL);

	//插入处理
	switch (pGameListItem->GetItemGenre())
	{
	case ItemGenre_Type:	//游戏种类
		{
			//插入种类
			//m_PlazaViewGameType.InsertGameType((CGameTypeItem *)pGameListItem);

			break;
		}
	case ItemGenre_Kind:	//游戏类型
		{
			break;
		}
	case ItemGenre_Server:	//游戏房间
		{
			break;
		}
	}

	return;
}

//更新通知
VOID CPlatformFrame::OnGameItemUpdate(CGameListItem * pGameListItem)
{
	//更新时间
	pGameListItem->m_dwUpdateTime=(DWORD)time(NULL);

	switch(pGameListItem->GetItemGenre())
	{		
	case ItemGenre_Type:	//类型子项
		{
			break;
		}
	case ItemGenre_Kind:	//游戏子项
		{
			////获取对象
			//CGameKindItem * pGameKindItem = (CGameKindItem *)pGameListItem;
			//m_PlazaViewGame.UpdateKindViewItem(pGameKindItem->m_GameKind.wKindID);

			break;
		}
	case ItemGenre_Server:	//房间子项
		{
			break;
		}
	}

	return;
}

//删除通知
VOID CPlatformFrame::OnGameItemDelete(CGameListItem * pGameListItem)
{
	return;
}

//消息过虑
BOOL CPlatformFrame::PreTranslateMessage(MSG * pMsg)
{
	//按键过虑
	if (pMsg->message==WM_KEYDOWN)
	{
		if(pMsg->wParam==VK_RETURN) return TRUE;
	}

	return __super::PreTranslateMessage(pMsg);
}

//命令函数
BOOL CPlatformFrame::OnCommand(WPARAM wParam, LPARAM lParam)
{
	//变量定义
	UINT nCommandID=LOWORD(wParam);

	//菜单命令
	switch (nCommandID)
	{
	case IDM_USER_LOGON:		//用户登录
		{
			m_MissionLogon.ShowLogon();

			return TRUE;
		}
	case IDM_SWITCH_ACCOUNTS:	//切换帐号
		{
			//切换询问
			if (m_PlazaViewServer.m_hWnd!=NULL)
			{
				CDlgInformation Information(this);
				if (Information.ShowMessageBox(TEXT("游戏房间即将关闭，确定要“切换帐号”吗？ "),MB_YESNO)!=IDYES) return TRUE;
			}

			//全局设置
			CGlobalUnits::GetInstance()->DeleteUserCookie();
			CGlobalUserInfo::GetInstance()->ResetUserInfoData();

			//设置视图
			//m_PlazaViewGameType.ResetGameTypeView();

			//获取对象
			CServerListData * pServerListData = CServerListData::GetInstance();

			//清除报名
			POSITION Position=NULL;
			do
			{
				//获取房间
				CGameServerItem * pGameServerItem=pServerListData->EmunGameServerItem(Position);
				if(pGameServerItem!=NULL)
				{
					pGameServerItem->m_bSignuped=false;
					ZeroMemory(&pGameServerItem->m_GameMatch,sizeof(pGameServerItem->m_GameMatch));
				}

			} while (Position!=NULL);

			//清除代理
			do
			{
				//获取房间
				CAgentGameKindItem * pAgentGameKindItem=pServerListData->EmunAgentGameKindItem(Position);
				if(pAgentGameKindItem!=NULL)
				{
					pServerListData->DeleteAgentGameKindItem(pAgentGameKindItem->m_AgentGameKind.wKindID);
				}

			} while (Position!=NULL);

			//发送事件
			CPlatformEvent * pPlatformEvent=CPlatformEvent::GetInstance();
			if (pPlatformEvent!=NULL) pPlatformEvent->SendPlatformEvent(EVENT_USER_LOGOUT,0L);

			return TRUE;
		}
	case IDM_CLOSE:		//关闭
		{
			//投递消息
			PostMessage(WM_CLOSE,0,0);

			return TRUE;
		}
	case IDM_RESTORE:   //还原
		{
			if(IsIconic())
			{
				ShowWindow(SW_RESTORE);
			}
			return TRUE;
		}
	case IDM_MINIMIZE:  //最小化
		{
			if(IsWindowVisible())
			{
				ShowWindow(SW_MINIMIZE);
			}
			return TRUE;
		}
	case IDM_MAXIMIZE:  //最大化
		{
			if(IsIconic())
			{
				ShowWindow(SW_RESTORE);
			}
			MaxSizeWindow();
			return TRUE;
		}
	case IDM_SHOW_LOGON: //显示登录
		{
			m_MissionLogon.ShowLogon(SW_RESTORE);

			return TRUE;
		}
	}

	return __super::OnCommand(wParam,lParam);
}

void CPlatformFrame::InitControlUI()
{
	m_PlazaUserInfo.InitControlUI();
	m_PlazaHallList.InitControlUI();

	//获取对象
	CGlobalWebLink * pGlobalWebLink=CGlobalWebLink::GetInstance();

	//构造地址
	TCHAR szBillUrl[256]=TEXT("");
	_sntprintf(szBillUrl,CountArray(szBillUrl),TEXT("%s/Notice.aspx"),pGlobalWebLink->GetPlatformLink());

	CWkeBrowserUI* pActiveXUI1 = static_cast<CWkeBrowserUI*>(GetControlByName(szWebBrowserNoticeControlName));
	if( pActiveXUI1 ) 
	{
		pActiveXUI1->Navigate(szBillUrl);
	}

	_sntprintf(szBillUrl,CountArray(szBillUrl),TEXT("%s/UserRank.aspx"),pGlobalWebLink->GetPlatformLink());

	CWkeBrowserUI* pActiveXUI2 = static_cast<CWkeBrowserUI*>(GetControlByName(szWebBrowserUserRankControlName));
	if( pActiveXUI2 ) 
	{
		pActiveXUI2->Navigate(szBillUrl);
	}

	_sntprintf(szBillUrl,CountArray(szBillUrl),TEXT("%s/ads/PlatformBottom.aspx"),pGlobalWebLink->GetPlatformLink());

	CWkeBrowserUI* pActiveXUI3 = static_cast<CWkeBrowserUI*>(GetControlByName(szWebBrowserBottomAdControlName));
	if( pActiveXUI3 ) 
	{
		pActiveXUI3->Navigate(szBillUrl);
	}

	_sntprintf(szBillUrl,CountArray(szBillUrl),TEXT("%s/SystemMessage.aspx"),pGlobalWebLink->GetPlatformLink());

	CWkeBrowserUI* pActiveXUI4 = static_cast<CWkeBrowserUI*>(GetControlByName(szWebBrowserMarqueeControlName));
	if( pActiveXUI4 ) 
	{
		pActiveXUI4->Navigate(szBillUrl);
	}
}


//消息提醒
void CPlatformFrame::Notify(TNotifyUI &  msg)
{	
	//获取对象
	CControlUI * pControlUI = msg.pSender;

	//点击事件
	if ((lstrcmp(msg.sType, TEXT("click")) == 0))
	{
		if(lstrcmp(pControlUI->GetName(), _T("ButtonEdit"))!=0)
		{
			if(lstrcmp(pControlUI->GetName(), _T("ItemGameDelete"))!=0)
			{
				m_PlazaHallGame.CloseDeleteState();
			}
			
		}
		
	}
	else if ((lstrcmp(msg.sType, TEXT("itemactivate")) == 0) ||  (lstrcmp(msg.sType, TEXT("itemclick")) == 0))
	{
		if (_tcsicmp(msg.pSender->GetClass(), _T("NodeElementUI")) == 0)
		{
			m_PlazaHallGame.CloseDeleteState();
		}
	}
	//用户面板
	if(m_PlazaUserInfo.Notify(msg))
	{
		return;
	}
	if(m_PlazaHallList.Notify(msg))
	{
		return;
	}
	if (m_PlazaHallGame.Notify(msg))
	{
		return;
	}



	//点击事件
	if (lstrcmp(msg.sType, TEXT("click")) == 0)
	{
		//标题菜单
		if(lstrcmp(pControlUI->GetName(), szButtonMainControlName)==0)
		{
						//变量定义
			ASSERT(CParameterGlobal::GetInstance()!=NULL);
			CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();

			//获取对象
			ASSERT(CGlobalWebLink::GetInstance()!=NULL);
			CGlobalWebLink * pGlobalWebLink=CGlobalWebLink::GetInstance();

			//构造地址
			TCHAR szNavigation[256]=TEXT("");
			_sntprintf(szNavigation,CountArray(szNavigation),TEXT("%s/%sNavigation%ld.aspx"),pGlobalWebLink->GetPlatformLink(),pGlobalWebLink->GetValidateLink(),1);

			//打开页面
			ShellExecute(NULL,TEXT("OPEN"),szNavigation,NULL,NULL,SW_NORMAL);	

			return;
		}
		else if(lstrcmp(pControlUI->GetName(), szButtonMySpreadControlName)==0)
		{
			ASSERT(CGlobalUnits::GetInstance());
			CGlobalUnits::GetInstance()->PerformShowMySpread();

			return;
		}
		else if(lstrcmp(pControlUI->GetName(), szButtonLockMachineControlName)==0)
		{
			ASSERT(CGlobalUnits::GetInstance());
			CGlobalUnits::GetInstance()->PerformShowLockMachine();

			return;
		}
		else if(lstrcmp(pControlUI->GetName(), szButtonHlepControlName)==0)
		{
			ASSERT(CGlobalUnits::GetInstance());
			CGlobalUnits::GetInstance()->PerformShowHelp();	

			return;
		}
		else if(lstrcmp(pControlUI->GetName(), szButtonMunuControlName)==0)
		{
			CMenuWnd* pMenu = new CMenuWnd();

			int x = pControlUI->GetX();
			int y = pControlUI->GetY();
			int w=  pControlUI->GetWidth();
			int h=  pControlUI->GetHeight();

			CPoint point;
			point.x = x + w/2;
			point.y = y + h;

			point.x -= 50;
			

			//pControlUI = new
			
			::ClientToScreen(m_hWnd, &point);
			pMenu->Init(NULL, _T("PlazaMenu.xml"),point, &m_PaintManager, NULL);
		}

		//充值中心
		else if(lstrcmp(pControlUI->GetName(), szButtonMidRechargeControlName)==0) 
		{
			ASSERT(CGlobalUnits::GetInstance());
			CGlobalUnits::GetInstance()->PerformShowRecharge();

			return;
		}
		//游戏商城
		else if(lstrcmp(pControlUI->GetName(), szButtonMidShopControlName)==0) 
		{
			ASSERT(CGlobalUnits::GetInstance());
			CGlobalUnits::GetInstance()->PerformShowShop();

			return;
		}
		//我的背包
		else if(lstrcmp(pControlUI->GetName(), szButtonMidBagControlName)==0) 
		{
			ASSERT(CGlobalUnits::GetInstance());
			CGlobalUnits::GetInstance()->PerformShowBag();

			return;
		}
		//我的银行
		else if(lstrcmp(pControlUI->GetName(), szButtonMidBankControlName)==0) 
		{
			ASSERT(CGlobalUnits::GetInstance());
			CGlobalUnits::GetInstance()->PerformShowInsure();

			return;
		}


		if(lstrcmp(pControlUI->GetName(), szButtonCloseControlName)==0) 
		{
			PostMessage(WM_CLOSE,0,0);
			return;
		}
		else if(lstrcmp(pControlUI->GetName(), szButtonMinControlName)==0)
		{
			ShowWindow(SW_MINIMIZE);
			return;
		}
		else if(lstrcmp(pControlUI->GetName(), szButtonMaxControlName)==0)
		{
			//窗口控制
			if (m_bMaxShow==false) 
			{
				//最大化窗口
				MaxSizeWindow();				
			}

			//更新界面
			RedrawWindow(NULL,NULL,RDW_ERASE|RDW_INVALIDATE|RDW_ERASENOW|RDW_UPDATENOW);

			return;
		}
		else if(lstrcmp(pControlUI->GetName(), szButtonResoreControlName)==0)
		{
			//窗口控制
			if (m_bMaxShow==true)
			{
				//还原窗口
				RestoreWindow();

				//设置控件
				pControlUI->SetVisible(false);
				pControlUI = GetControlByName(szButtonMaxControlName);
				if(pControlUI!=NULL) pControlUI->SetVisible();
			}

			//更新界面
			RedrawWindow(NULL,NULL,RDW_ERASE|RDW_INVALIDATE|RDW_ERASENOW|RDW_UPDATENOW);

			return;
		}
		else if(lstrcmp(pControlUI->GetName(), szButtonNavigation1ControlName)==0 ||
				lstrcmp(pControlUI->GetName(), szButtonNavigation2ControlName)==0 ||
				lstrcmp(pControlUI->GetName(), szButtonNavigation3ControlName)==0 ||
				lstrcmp(pControlUI->GetName(), szButtonNavigation4ControlName)==0)
		{
			//变量定义
			ASSERT(CParameterGlobal::GetInstance()!=NULL);
			CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();

			//获取对象
			ASSERT(CGlobalWebLink::GetInstance()!=NULL);
			CGlobalWebLink * pGlobalWebLink=CGlobalWebLink::GetInstance();

			//构造地址
			TCHAR szNavigation[256]=TEXT("");
			_sntprintf(szNavigation,CountArray(szNavigation),TEXT("%s/%sNavigation%ld.aspx"),pGlobalWebLink->GetPlatformLink(),pGlobalWebLink->GetValidateLink(),pControlUI->GetTag());

			//打开页面
			ShellExecute(NULL,TEXT("OPEN"),szNavigation,NULL,NULL,SW_NORMAL);

			return;
		}
		else if(lstrcmp(pControlUI->GetName(), _T("ButtonQuit"))==0  )
		{
			//查找对象
			CContainerUI * pCnt = (CContainerUI * )GetControlByName(_T("PlazaHallMain"));
			if(pCnt!=NULL)
			{
				pCnt->SetVisible();
			}
			//查找对象
			pCnt = (CContainerUI * )GetControlByName(_T("PlazaRoomMain"));
			if(pCnt!=NULL)
			{
				pCnt->SetVisible(false);
			}
			return;
		}
	}
			//查找对象

}

LPCTSTR CPlatformFrame::GetSkinFile()
{
	return TEXT("PlatformFrame.xml");
}

//调整控件
VOID CPlatformFrame::RectifyControl(INT nWidth, INT nHeight)
{
	//状态判断
	if ((nWidth==0)||(nHeight==0)) return;



	//移动准备
	HDWP hDwp=BeginDeferWindowPos(32);
	UINT uFlags=SWP_NOACTIVATE|SWP_NOCOPYBITS|SWP_NOZORDER;

	//房间容器
	if (IsEnteredServerItem())
	{
		if(m_PlazaViewServer.m_bPlaying)
		{
            DeferWindowPos(hDwp,m_PlazaViewContainer,NULL,15,164,nWidth-30,nHeight-177,uFlags);
		}
		else
		{
			DeferWindowPos(hDwp,m_PlazaViewContainer,NULL,15,164,0,0,uFlags);
		}
	}
	else
	{
		DeferWindowPos(hDwp,m_PlazaViewContainer,NULL,15,164,0,0,uFlags);
	}
	

	//结束调整
	LockWindowUpdate();
	EndDeferWindowPos(hDwp);
	UnlockWindowUpdate();		

	//更新界面
	RedrawWindow(NULL,NULL,RDW_ERASE|RDW_INVALIDATE);

	return;
}

//类型区域
CRect CPlatformFrame::MapSelectedTypeRect()
{
	//变量定义
	CRect rcSourceType,rcDestType;

	////获取区域
	//m_PlazaViewGame.GetWindowRect(&rcDestType);
	//rcSourceType = m_PlazaViewGameType.GetSelectedTypeRect();	

	////设置区域
	//rcDestType.left = rcSourceType.left;
	//rcDestType.right= rcSourceType.right;
	//rcDestType.bottom = rcDestType.top + rcSourceType.Height();

	////转换坐标
	//ScreenToClient(&rcDestType);

	return rcDestType;
}

//任务提醒
VOID CPlatformFrame::ShowTaskNotifyer(LPCTSTR pszTaskName)
{


	return;
}

//浏览页面
bool CPlatformFrame::WebBrowse(LPCTSTR pszURL, bool bAutoFullView)
{
	//浏览页面
	//CPlazaViewGame::GetInstance()->WebBrowse(pszURL);

	//激活大厅
	//m_WndViewItemCtrl.ActiveViewItem(&m_PlazaViewGame);

	//界面控制
	if (bAutoFullView==true)
	{
		CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();
		//if (pParameterGlobal->m_bFullScreenBrowse==true) ControlServerList(ServerListControl_Hide,false);
	}

	return true;
}

bool CPlatformFrame::DownLoadGame( LPCTSTR pszClientName, WORD wKindID, WORD wServerID )
{
	return m_PlazaHallGame.DownLoadGame(pszClientName,wKindID,wServerID);
}


//提取私聊
bool CPlatformFrame::ShowWhisperItem()
{
	//提取消息
	if (m_DlgWhisperItemArray.GetCount()>0L)
	{
		//获取子项
		CDlgWhisper * pDlgWhisper=m_DlgWhisperItemArray[0];

		//显示窗口
		pDlgWhisper->ShowWindow(SW_RESTORE);

		//置顶窗口
		pDlgWhisper->SetActiveWindow();
		pDlgWhisper->BringWindowToTop();
		pDlgWhisper->SetForegroundWindow();

		//删除子项
		m_DlgWhisperItemArray.RemoveAt(0L);

		//删除图标
		if (m_DlgWhisperItemArray.GetCount()==0L)
		{
			//设置变量
			m_bWhisperIcon=false;

			//删除时间
			KillTimer(IDI_FLASH_TRAY_ICON);

			//设置图标
			m_SystemTrayIcon.ShowTrayIcon(LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME)));
		}

		return true;
	}

	return false;
}

//插入私聊
bool CPlatformFrame::InsertWhisperItem(CDlgWhisper * pDlgWhisper)
{
	//参数效验
	ASSERT((pDlgWhisper!=NULL)&&(pDlgWhisper->m_hWnd!=NULL));
	if ((pDlgWhisper==NULL)||(pDlgWhisper->m_hWnd==NULL)) return false;

	//状态判断
	if (pDlgWhisper->IsWindowVisible()) return false;

	//存在搜索
	for (INT_PTR i=0;i<m_DlgWhisperItemArray.GetCount();i++)
	{
		if (m_DlgWhisperItemArray[i]==pDlgWhisper) return false;
	}

	//插入私聊
	m_DlgWhisperItemArray.Add(pDlgWhisper);

	//设置图标
	if (m_DlgWhisperItemArray.GetCount()==1L)
	{
		//设置变量
		m_bWhisperIcon=true;

		//设置时间
		SetTimer(IDI_FLASH_TRAY_ICON,500,NULL);

		//设置图标
		m_SystemTrayIcon.ShowTrayIcon(LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDI_WHISPER)));
	}

	return true;
}

//删除私聊
bool CPlatformFrame::RemoveWhisperItem(CDlgWhisper * pDlgWhisper)
{
	//存在搜索
	for (INT_PTR i=0;i<m_DlgWhisperItemArray.GetCount();i++)
	{
		if (m_DlgWhisperItemArray[i]==pDlgWhisper)
		{
			//删除子项
			m_DlgWhisperItemArray.RemoveAt(i);

			//删除图标
			if (m_DlgWhisperItemArray.GetCount()==0L)
			{
				//设置变量
				m_bWhisperIcon=false;

				//删除时间
				KillTimer(IDI_FLASH_TRAY_ICON);

				//设置图标
				m_SystemTrayIcon.ShowTrayIcon(LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME)));
			}

			return true;
		}
	}

	return false;
}

//激活房间
bool CPlatformFrame::ActiveServerViewItem()
{
	//激活判断
	ASSERT(m_PlazaViewServer.m_hWnd!=NULL);
	if (m_PlazaViewServer.m_hWnd==NULL) return false;

	//锁住更新
	LockWindowUpdate();

	//设置视图
	m_PlazaViewContainer.RemoveViewItem(VIA_Left);
	m_PlazaViewContainer.RemoveViewItem(VIA_Top);	
	m_PlazaViewContainer.InsertViewItem(&m_PlazaViewServer,VIA_Center);	
	m_PlazaViewContainer.InsertViewItem(&m_PlazaViewServerCtrl,VIA_Right);		

	//解锁更新
	UnlockWindowUpdate();

	CRect rc;
	GetWindowRect(rc);
	RectifyControl(rc.right-rc.left,rc.bottom-rc.top);
	//调整子项
	m_PlazaViewContainer.RectifyViewItem();
	m_PlazaViewContainer.SetActiveWindow();

	//查找对象
	CContainerUI * pCnt = (CContainerUI * )GetControlByName(_T("PlazaRoomMain"));
	if(pCnt!=NULL)
	{
		pCnt->SetVisible();
	}
	//查找对象
	pCnt = (CContainerUI * )GetControlByName(_T("PlazaHallMain"));
	if(pCnt!=NULL)
	{
		pCnt->SetVisible(false);
	}

	return true;
}

//删除房间
bool CPlatformFrame::DeleteServerViewItem()
{
	//激活判断
	if (m_PlazaViewServer.m_hWnd==NULL) return false;

	//锁住更新
	LockWindowUpdate();

	//销毁房间
	m_PlazaViewServer.DestroyWindow();	
	m_PlazaViewServerCtrl.DestroyWindow();

	//游戏视图
	//m_PlazaViewContainer.InsertViewItem(&m_PlazaViewGameType,VIA_Top);	
	//m_PlazaViewContainer.InsertViewItem(&m_PlazaViewGame,VIA_Center);	
	//m_PlazaViewContainer.InsertViewItem(&m_PlazaViewAffiche,VIA_Right);	

	m_PlazaViewContainer.RemoveViewItem(VIA_Center);
	m_PlazaViewContainer.RemoveViewItem(VIA_Right);
	//解锁更新
	UnlockWindowUpdate();

	CRect rc;
	GetWindowRect(rc);
	RectifyControl(rc.right-rc.left,rc.bottom-rc.top);



	//调整子项
	m_PlazaViewContainer.RectifyViewItem();

	//查找对象
	CContainerUI * pCnt = (CContainerUI * )GetControlByName(_T("PlazaHallMain"));
	if(pCnt!=NULL)
	{
		pCnt->SetVisible();
	}
	//查找对象
	pCnt = (CContainerUI * )GetControlByName(_T("PlazaRoomMain"));
	if(pCnt!=NULL)
	{
		pCnt->SetVisible(false);
	}
	//删除状态
	m_PlazaHallGame.CloseDeleteState();

	return true;
}

//进入房间
bool CPlatformFrame::EntranceServerItem(CGameServerItem * pGameServerItem)
{
	//效验参数
	ASSERT(pGameServerItem!=NULL);
	if (pGameServerItem==NULL) return NULL;

	//变量定义
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	//登录判断
	if (pGlobalUserData->dwUserID==0L) return NULL;

	//变量定义
	tagGameServer * pGameServer=&pGameServerItem->m_GameServer;
	tagGameKind * pGameKind=&pGameServerItem->m_pGameKindItem->m_GameKind;

	//当前判断
	if (m_PlazaViewServer.m_hWnd!=NULL)
	{
		////提示消息
		//TCHAR szBuffer[256]=TEXT("");
		//_sntprintf(szBuffer,CountArray(szBuffer),TEXT("正在进入 [ %s ] 游戏房间中，请稍候等待片刻..."),m_PlazaViewServer.GetServerName());

		////提示消息
		//CDlgInformation Information(this);
		//Information.ShowMessageBox(szBuffer,MB_OK|MB_ICONWARNING,30);

		return false;
	}

	//取消实名
	m_PlazaUserInfo.CancleRealTip();



	//创建房间
	try
	{
		//配置房间
		m_PlazaViewServer.Create(IDD_DLG_CHILD,this);
		m_PlazaViewServer.InitServerViewItem(pGameServerItem,m_wAVServerPort,m_dwAVServerAddr);

		//配置控件
		m_PlazaViewServerCtrl.Create(IDD_GAME_RIGHT_CONTROL,this);
	}
	catch (LPCTSTR pszString)
	{
		//销毁窗口
		if(m_PlazaViewServer.m_hWnd) m_PlazaViewServer.DestroyWindow();
		if(m_PlazaViewServerCtrl.m_hWnd) m_PlazaViewServerCtrl.DestroyWindow();

		//提示信息
		CDlgInformation Information(this);
		Information.ShowMessageBox(pGameServer->szServerName,pszString,MB_ICONERROR);

		return false;
	}

	return true;
}

//获取对象
CControlUI * CPlatformFrame::GetPlazaControlByName(LPCTSTR pszControlName)
{
	return GetControlByName(pszControlName);
}


void CPlatformFrame::ReturnHallMain()
{
	m_PlazaHallGame.ReturnHallMain();
}

void CPlatformFrame::ShowWindowMaskLayer(bool bShow /*=true*/)
{
	CControlUI* pActiveXUI1 = GetControlByName(_T("Lableshadow"));
	if( pActiveXUI1 ) 
	{
		pActiveXUI1->SetVisible(bShow);
	}
}

//还原窗口
bool CPlatformFrame::RestoreWindow()
{
	//状态判断
	if (m_bMaxShow==true)
	{
		//设置变量
		m_bMaxShow=false;
		m_bRectify=false;

		//最大化按钮
		CControlUI * pControlUI = GetControlByName(szButtonResoreControlName); 
		if(pControlUI!=NULL) pControlUI->SetVisible(false);

		//还原按钮
		pControlUI = GetControlByName(szButtonMaxControlName);
		if(pControlUI!=NULL) pControlUI->SetVisible();

		//移动窗口
		LockWindowUpdate();
		SetWindowPos(NULL,m_rcNormalSize.left,m_rcNormalSize.top,m_rcNormalSize.Width(),m_rcNormalSize.Height(),SWP_NOZORDER);
		UnlockWindowUpdate();
	}

	return true;
}

//最大窗口
bool CPlatformFrame::MaxSizeWindow(bool bRecordPosition)
{
	//状态判断
	if (m_bMaxShow==false)
	{
		//设置变量
		m_bMaxShow=true;
		m_bRectify=false;

		//默认位置
		if(bRecordPosition==true) GetWindowRect(&m_rcNormalSize);

		//最大化按钮
		CControlUI * pControlUI = GetControlByName(szButtonMaxControlName); 
		if(pControlUI!=NULL) pControlUI->SetVisible(false);

		//还原按钮
		pControlUI = GetControlByName(szButtonResoreControlName);
		if(pControlUI!=NULL) pControlUI->SetVisible();

		//获取位置
		CRect rcArce;
		SystemParametersInfo(SPI_GETWORKAREA,0,&rcArce,SPIF_SENDCHANGE);

		//移动窗口
		LockWindowUpdate();
		SetWindowPos(NULL,rcArce.left-2,rcArce.top-2,rcArce.Width()+4,rcArce.Height()+4,SWP_NOZORDER);
		UnlockWindowUpdate();
	}

	return true;
}

//关闭消息
VOID CPlatformFrame::OnClose()
{
	//变量定义
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	//关闭提示
	if ((pGlobalUserData->dwUserID!=0L)&&(m_MissionLogon.GetActiveStatus()==false))
	{
		//显示窗口
		INT_PTR nResult = IDCANCEL;
		if( m_pDlgEnquire == NULL )
		{
			CDlgEnquire DlgEnquire;
			m_pDlgEnquire=&DlgEnquire;
			nResult=DlgEnquire.DoModal();
			m_pDlgEnquire = NULL;
		}
		
		//命令处理
		switch (nResult)
		{
		case IDCANCEL:				//取消动作
			{
				return;
			}
		case IDC_CLOSE_SERVER:		//退出房间
			{
				//关闭房间
				if(m_PlazaViewServer.m_hWnd!=NULL)
				{
					m_PlazaViewServer.SendMessage(WM_COMMAND,IDM_DELETE_SERVER_ITEM,0L);
				}

				return;
			}
		case IDC_SWITCH_ACCOUNTS:	//切换帐号
			{
				//投递消息
				PostMessage(WM_COMMAND,IDM_SWITCH_ACCOUNTS,0);

				return;
			}
		}
	}

	//注销热键
	UnregisterHotKey(m_hWnd,IDI_HOT_KEY_BOSS);
	UnregisterHotKey(m_hWnd,IDI_HOT_KEY_WHISPER);

	__super::OnClose();
}

//时间消息
VOID CPlatformFrame::OnTimer(UINT_PTR nIDEvent)
{
	switch (nIDEvent)
	{
	case IDI_FLASH_TRAY_ICON:	//任务图标
		{
			//设置变量
			m_bWhisperIcon=!m_bWhisperIcon;

			//设置图标
			UINT uIconID=(m_bWhisperIcon==true)?IDI_WHISPER:IDI_NULL;
			m_SystemTrayIcon.ShowTrayIcon(LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(uIconID)));

			return;
		}
	case IDI_CHECK_USER_BUFF:		//检测道具状态
	case IDI_CHECK_SEND_PRESENT:	//检测有无被赠送	
		{
			m_PlazaUserInfo.OnTimer(nIDEvent);
			return;
		}
	case IDI_SHOW_GAME_AD:
		{
			KillTimer(nIDEvent);
			ASSERT(CGlobalUnits::GetInstance() != NULL);
			CGlobalUnits::GetInstance()->PerformShowGameAd();
			return;
		}
	}

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

	//窗口位置
	CRect rcArce;
	SystemParametersInfo(SPI_GETWORKAREA,0,&rcArce,SPIF_SENDCHANGE);

	//设置位置
	lpMMI->ptMinTrackSize.x=__min(LESS_SCREEN_CX,rcArce.Width());
	lpMMI->ptMinTrackSize.y=__min(LESS_SCREEN_CY,rcArce.Height());

	return;
}

//建立消息
INT CPlatformFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct)==-1) return -1;

	//设置窗口
	ModifyStyle(WS_CAPTION|WS_BORDER, WS_MINIMIZEBOX|WS_MAXIMIZEBOX);
	ModifyStyleEx(WS_EX_CLIENTEDGE|WS_EX_WINDOWEDGE,0,0);	

	//变量定义
	CRect rcCreate(0,0,0,0);

	//创建容器
	m_PlazaViewContainer.Create(NULL,NULL,WS_VISIBLE|WS_CHILD|WS_CLIPCHILDREN,rcCreate,this,IDC_PLAZA_GAME_CONTAINER);

	//游戏列表
	CServerListData::GetInstance()->SetServerListDataSink(this);

	//注册事件
	CPlatformEvent * pPlatformEvent=CPlatformEvent::GetInstance();
	if (pPlatformEvent!=NULL) pPlatformEvent->RegisterEventWnd(m_hWnd);

	//设置位置
	CSize SizeRestrict;
	SizeRestrict.SetSize(LESS_SCREEN_CX,LESS_SCREEN_CY);

	//窗口位置
	CRect rcArce;
	SystemParametersInfo(SPI_GETWORKAREA,0,&rcArce,SPIF_SENDCHANGE);

	//读取位置
	CWHRegKey InfoKeyItem;
	if (InfoKeyItem.OpenRegKey(REG_INTERFACE_INFO,false)==true)
	{
		SizeRestrict.cx=InfoKeyItem.GetValue(TEXT("ScreenWidth"),rcArce.Width()/2);
		SizeRestrict.cy=InfoKeyItem.GetValue(TEXT("ScreenHeight"),rcArce.Height()/2);
	}

	//位置调整
	SizeRestrict.cx=__max(LESS_SCREEN_CX,SizeRestrict.cx);
	SizeRestrict.cy=__max(LESS_SCREEN_CY,SizeRestrict.cy);
	SizeRestrict.cx=__min(rcArce.Width(),SizeRestrict.cx);
	SizeRestrict.cy=__min(rcArce.Height(),SizeRestrict.cy);

	//移动窗口
	m_rcNormalSize.top=(rcArce.Height()-SizeRestrict.cy)/2;
	m_rcNormalSize.left=(rcArce.Width()-SizeRestrict.cx)/2;
	m_rcNormalSize.right=m_rcNormalSize.left+SizeRestrict.cx;
	m_rcNormalSize.bottom=m_rcNormalSize.top+SizeRestrict.cy;

	//移动窗口
	SetWindowPos(NULL,m_rcNormalSize.left,m_rcNormalSize.top,m_rcNormalSize.Width(),m_rcNormalSize.Height(),SWP_NOZORDER);

	////显示窗口
	//SetWindowRgn(NULL,TRUE);

	//登录系统
	PostMessage(WM_COMMAND,IDM_USER_LOGON,0);

	//注册热键
	CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();

	if (CWHService::RegisterHotKey(AfxGetMainWnd()->GetSafeHwnd(),IDI_HOT_KEY_BOSS,pParameterGlobal->m_wBossHotKey)==false)
	{
		//CDlgInformation Information(this);
		//Information.ShowMessageBox(TEXT("系统热键冲突，老板热键注册失败！"));
	}

	if (CWHService::RegisterHotKey(AfxGetMainWnd()->GetSafeHwnd(),IDI_HOT_KEY_WHISPER,pParameterGlobal->m_wWhisperHotKey)==false)
	{
		//CDlgInformation Information(this);
		//Information.ShowMessageBox(TEXT("系统热键冲突，提取消息热键注册失败！"));
	}
	//设置托盘
	m_SystemTrayIcon.InitTrayIcon(m_hWnd,IDC_SYSTEM_TRAY_ICON);
	m_SystemTrayIcon.ShowTrayIcon(LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME)),szProduct);

	if(pParameterGlobal->m_bFullScreenBrowse)
	{
		MaxSizeWindow();
	}
	else
	{
		RestoreWindow();
	}

	//更新人数
	SetTimer(IDI_UPDATA_ONLINE, TIME_UPDATA_ONLINE, NULL);

	//设置字体
	CSkinDialog::SetWndFont(this,NULL);

	SetWindowText(szProduct);

	return 0;
}

//鼠标消息
VOID CPlatformFrame::OnLButtonDblClk(UINT nFlags, CPoint Point)
{
	__super::OnLButtonDblClk(nFlags,Point);

	//状态判断
	if (Point.y>GetCaptionRect().Height()) return;

	//窗口控制
	if (m_bMaxShow==true)
	{
		RestoreWindow();
	}
	else
	{
		MaxSizeWindow();
	}
	
	return;
}

//设置改变
VOID CPlatformFrame::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	__super::OnSettingChange(uFlags,lpszSection);

	//窗口控制
	if (m_bMaxShow==true)
	{
		m_bMaxShow=false;
		MaxSizeWindow(false);
	}
	else
	{
		m_bMaxShow=true;
		RestoreWindow();
	}

	return;
}

//热键消息
LRESULT CPlatformFrame::OnMessageHotKey(WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
	case IDI_HOT_KEY_BOSS:		//老板热键
		{
			//变量定义
			CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
			tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

			//登录判断
			if (pGlobalUserData->dwUserID!=0L)
			{
				//变量定义
				bool bBossCome=(IsWindowVisible()==FALSE)?false:true;

				//隐藏窗口
				if (bBossCome==false)
				{
					//还原窗口
					ShowWindow(SW_RESTORE);
					ShowWindow(SW_SHOW);

					//置顶窗口
					SetActiveWindow();
					BringWindowToTop();
					SetForegroundWindow();

					//显示图标
					m_SystemTrayIcon.ShowTrayIcon(LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME)));
				}
				else
				{
					//隐藏窗口
					ShowWindow(SW_MINIMIZE);
					ShowWindow(SW_HIDE);

					//隐藏图标
					m_SystemTrayIcon.HideTrayIcon();
				}

				////发送消息
				//for (INT_PTR i=0;i<m_PlazaViewServerArray.GetCount();i++)
				//{
				//	ASSERT(m_PlazaViewServerArray[i]!=NULL);
				//	m_PlazaViewServerArray[i]->NotifyBossCome(bBossCome);
				//}
			}

			return 0;
		}
	case IDI_HOT_KEY_WHISPER:	//私聊热键
		{
			//提取消息
			if (m_DlgWhisperItemArray.GetCount()>0L)
			{
				ShowWhisperItem();
				return 1;
			}

			return 0;
		}
	}

	return 0;
}

//任务消息
LRESULT CPlatformFrame::OnMessageTaskBar(WPARAM wParam, LPARAM lParam)
{
	//显示菜单
	CPoint MousePoint;
	GetCursorPos(&MousePoint);
	MousePoint.y-=10;

	CMenuWnd* pMenu = new CMenuWnd();
	pMenu->Init(NULL, _T("PlazaMenuTray.xml"),MousePoint, &m_PaintManager, NULL,eMenuAlignment_Bottom);

	return 0L;
}

//图标消息
LRESULT CPlatformFrame::OnMessageTrayIcon(WPARAM wParam, LPARAM lParam)
{
	//事件处理
	switch (lParam)
	{
	case WM_LBUTTONDOWN:		//鼠标单击
		{
			if(IsIconic())
			{
				ShowWindow(SW_RESTORE);
			}

			//激活窗口
			SetActiveWindow();
			BringWindowToTop();
			SetForegroundWindow();

			return 1;
		}
	case WM_LBUTTONDBLCLK:		//鼠标双击
		{
			//提取消息
			if (m_DlgWhisperItemArray.GetCount()>0L)
			{
				ShowWhisperItem();
				return 1;
			}			

			return 1;
		}
	case WM_RBUTTONUP:		//鼠标单击
	case WM_RBUTTONDBLCLK:		//鼠标双击
		{
			//变量定义
			CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
			tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();
			//用户未登陆
			if(pGlobalUserData->dwUserID == 0)
			{
				//显示菜单
				CPoint MousePoint;
				GetCursorPos(&MousePoint);
				MousePoint.y-=10;

				CMenuWnd* pMenu = new CMenuWnd();
				pMenu->Init(NULL, _T("PlazaMenuTrayLogon.xml"),MousePoint, &m_PaintManager, NULL,eMenuAlignment_Bottom);
			}
			else
			{
				//显示菜单
				CPoint MousePoint;
				GetCursorPos(&MousePoint);
				MousePoint.y-=10;

				CMenuWnd* pMenu = new CMenuWnd();
				pMenu->Init(NULL, _T("PlazaMenuTray.xml"),MousePoint, &m_PaintManager, NULL,eMenuAlignment_Bottom);
			}			

			return 1;
		}
	}

	return 0;
}

//事件消息
LRESULT CPlatformFrame::OnMessagePlatformEvent(WPARAM wParam, LPARAM lParam)
{
	//处理模块
	if(m_PlazaUserInfo.OnMessagePlatformEvent(wParam, lParam))
	{
		return 0L;
	}
	//处理模块
	if(m_PlazaHallGame.OnMessagePlatformEvent(wParam, lParam))
	{
		return 0L;
	}
	//处理模块
	if (m_PlazaHallList.OnMessagePlatformEvent(wParam,lParam))
	{
		return 0L;
	}

	//事件处理
	switch (wParam)
	{
	case EVENT_USER_LOGON:			//登录完成
		{
			//显示窗口
			ShowWindow(SW_SHOW);

			//前置窗口
			SetForegroundWindow();

			//用户甜饼
			ASSERT(CGlobalUnits::GetInstance()!=NULL);
			CGlobalUnits::GetInstance()->WriteUserCookie();

			//读取配置
			TCHAR szPath[MAX_PATH]=TEXT("");
			CWHService::GetWorkDirectory(szPath, sizeof(szPath));
			TCHAR szFileName[MAX_PATH]=TEXT("");
			_sntprintf(szFileName,CountArray(szFileName),TEXT("%s\\PlazaConfig.INI"),szPath);

			TCHAR	szGameAd[MAX_PATH];
			ZeroMemory(szGameAd,CountArray(szGameAd));
			GetPrivateProfileString(TEXT("GameAd"),TEXT("DisEnable"),TEXT(""),szGameAd,CountArray(szGameAd),szFileName);
			int nGameAd = _ttoi(szGameAd);
			if (nGameAd==0)
			{
				WORD wTodayShowDate = 0;
				CWHRegKey RegOptionItem;
				RegOptionItem.OpenRegKey(TEXT(""),true);
				wTodayShowDate=(WORD)RegOptionItem.GetValue(TEXT("TodayShowDate"),wTodayShowDate);

				//获取时间
				SYSTEMTIME SystemTime;
				GetLocalTime(&SystemTime); 
				WORD wDay=SystemTime.wDay;	
				WORD wMonth=SystemTime.wMonth;
				WORD wYear=SystemTime.wYear-1980;

				WORD wCurrDate = wYear<<16|wMonth<<8|wDay;

				if (wTodayShowDate != wCurrDate)
				{
					//设置遮盖
					ShowWindowMaskLayer();

					//显示广告
					SetTimer(IDI_SHOW_GAME_AD,800,NULL);
				}


			}

			return 0L;
		}
	case EVENT_USER_LOGOUT:			//注销成功
		{
			//隐藏窗口
			ShowWindow(SW_HIDE);

			//关闭房间			
			if(m_PlazaViewServer.m_hWnd!=NULL)
			{
				m_PlazaViewServer.SendMessage(WM_COMMAND,IDM_DELETE_SERVER_ITEM,0L);
			}
			m_PlazaUserInfo.ClearBuff();

			//显示登录
			m_MissionLogon.ShowLogon();

			return 0L;
		}
	case EVENT_DOWN_LOAD_FINISH:	//下载完成
		{
			//获取参数
			WORD wKindID=LOWORD(lParam);
			WORD wServerID=HIWORD(lParam);

			//更新列表
			CServerListData * pServerListData=CServerListData::GetInstance();
			if (pServerListData!=NULL) pServerListData->OnEventDownLoadFinish(wKindID);

			//进入房间
			if ((wKindID!=0)&&(wServerID!=0))
			{
				CGameServerItem * pGameServerItem=m_ServerListData.SearchGameServer(wServerID);
				if (pGameServerItem!=NULL) EntranceServerItem(pGameServerItem);
			}

			return 0L;
		}
	case EVENT_USER_SHOW_REAL_TIP:  //显示实名
		{
			//ASSERT(CGlobalUnits::GetInstance()!=NULL);
			//CGlobalUnits::GetInstance()->PerformShowRealTip();
			m_PlazaUserInfo.ShowRealTip();
			return 0L;
		}
	case EVENT_USER_HIDE_REAL_TIP:
		{
			m_PlazaUserInfo.HideRealTip();
			return 0L;
		}
	case EVENT_USER_SHOW_CHECKIN:	//显示签到
		{
			ASSERT(CGlobalUnits::GetInstance()!=NULL);
			CGlobalUnits::GetInstance()->PerformShowCheckIn();

			return 0L;
		}
	case EVENT_USER_SHOW_MY_SPREAD:	//显示推广
		{
			ASSERT(CGlobalUnits::GetInstance()!=NULL);
			CGlobalUnits::GetInstance()->PerformShowMySpread();

			return 0L;
		}
	case EVENT_USER_SHOW_TASKVIEW:	//显示任务
		{
			ASSERT(CGlobalUnits::GetInstance()!=NULL);
			CGlobalUnits::GetInstance()->PerformShowTaskView();

			return 0L;
		}
	case EVENT_USER_SHOW_RECHARGE: //显示充值
		{
			ASSERT(CGlobalUnits::GetInstance()!=NULL);
			CGlobalUnits::GetInstance()->PerformShowRecharge();
			return 0L;
		}
	case EVENT_USER_SHOW_PERSONER: //个人中心
		{
			ASSERT(CGlobalUnits::GetInstance()!=NULL);
			CGlobalUnits::GetInstance()->PerformShowPersonerCenter((BYTE)lParam);
			return 0L;
		}
	case EVENT_USER_SHOW_MY_INSURE:
		{
			ASSERT(CGlobalUnits::GetInstance() != NULL);
			CGlobalUnits::GetInstance()->PerformShowInsure();
			return 0L;
		}
	case EVENT_USER_SHOW_BAG_RECORD:
		{
			ASSERT(CGlobalUnits::GetInstance() != NULL);
			CGlobalUnits::GetInstance()->PerformShowBagRecord();
			return 0L;
		}
	case EVENT_USER_SHOW_BASE_ENSURE: //显示低保
		{
			ASSERT(CGlobalUnits::GetInstance() != NULL);
			CGlobalUnits::GetInstance()->PerformShowBaseEnsure();
			return 0L;
		}
	case EVENT_USER_SHOW_BAG: //显示背包
		{
			ASSERT(CGlobalUnits::GetInstance() != NULL);
			CGlobalUnits::GetInstance()->PerformShowBag();
			return 0L;
		}
	case EVENT_USER_SHOW_EXCHANGE: //显示兑换
		{
			ASSERT(CGlobalUnits::GetInstance() != NULL);
			CGlobalUnits::GetInstance()->PerformShowExchange();
			return 0L;
		}
	case EVENT_USER_SHOW_SHOP: //显示商城
		{
			ASSERT(CGlobalUnits::GetInstance() != NULL);
			CGlobalUnits::GetInstance()->PerformShowShop();
			return 0L;
		}
	case EVENT_USER_SHOW_UPDATE_BUFF: //更新BUFF
		{
			tagPropertyBuff * pPropertyBuff= (tagPropertyBuff*)lParam;
			m_PlazaUserInfo.UpdateBuffData(*pPropertyBuff);
			m_PlazaUserInfo.UpdateBuffUI();
			KillTimer(IDI_CHECK_USER_BUFF);
			SetTimer(IDI_CHECK_USER_BUFF, 1000, NULL);
			return 0L;
		}
	case EVENT_USER_CLOSE_GAME_AD:
		{
			ShowWindowMaskLayer(false);
			return 0L;
		}
	case EVENT_USER_TRACK_ACCOUNTS:	//追踪对象
		{
			//提取数据
			tagTrackAccountsInfo * pTrackAccountsInfo= (tagTrackAccountsInfo *)lParam;
			if(pTrackAccountsInfo!=NULL)
			{
				//获取对象
				ASSERT(CServerListData::GetInstance());
				CServerListData * pServerListData = CServerListData::GetInstance();
				if(pServerListData!=NULL)
				{
					//查找房间
					CGameServerItem * pGameServerItem = pServerListData->SearchGameServer(pTrackAccountsInfo->wServerID);
					if(pGameServerItem!=NULL)
					{
						//设置标识
						ASSERT(CGlobalUnits::GetInstance()!=NULL);
						CGlobalUnits::GetInstance()->SetTrackUserID(pTrackAccountsInfo->dwTargetID);
						
						//关闭房间
						if(m_PlazaViewServer.m_hWnd!=NULL)
						{
							m_PlazaViewServer.SendMessage(WM_COMMAND,IDM_DELETE_SERVER_ITEM,0L);
						}

						//进入房间
						if (m_PlazaViewServer.m_hWnd==NULL) 
						{
							//进入房间
							EntranceServerItem(pGameServerItem);
						}

						return 0L;
					}
				}

				////查找类型
				//CGameKindItem * pGameKindItem = pServerListData->SearchGameKind(pTrackAccountsInfo->wKindID);
    //            if(pGameKindItem != NULL)
				//{
				//	//设置标识
				//	ASSERT(CGlobalUnits::GetInstance()!=NULL);
				//	CGlobalUnits::GetInstance()->SetTrackUserID(pTrackAccountsInfo->dwTargetID);
				//	CGlobalUnits::GetInstance()->SetTrackServerID(pTrackAccountsInfo->wServerID);

				//	//进入类型
				//	tagGameKindInfo GameKindInfo;
				//	GameKindInfo.pGameKindItem=pGameKindItem;
				//	m_PlazaViewGame.OnViewPageKindItemClicked(&GameKindInfo);

				//	return 0L;
				//}
			}
			
			//失败提示
			CDlgInformation Information;
			Information.ShowMessageBox(TEXT("提示"),TEXT("没有找到该玩家所在的房间！"));

			return 0L;
		}
	}

	return 0L;
}

//事件消息
LRESULT CPlatformFrame::OnMessageInstallCancelEvent(WPARAM wParam, LPARAM lParam)
{
	//显示登录
	m_MissionLogon.ShowLogon();

	return 0L;
}

void CPlatformFrame::SetVideoOption(WORD wAVServerPort, DWORD dwAVServerAddr)
{
	m_wAVServerPort=wAVServerPort;
	m_dwAVServerAddr=dwAVServerAddr;
}

//事件消息
LRESULT CPlatformFrame::OnMessageInsurePlazaEvent(WPARAM wParam, LPARAM lParam)
{
	tagInsurePlazaEvent * pInsureEvent = (tagInsurePlazaEvent*)wParam;
	if(pInsureEvent==NULL) return 0L;

	//更新命令
	if(pInsureEvent->wCommandID==INSURE_COMMAND_UPDATE)
	{
		//变量定义
		CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
		tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

		//更新
		pGlobalUserData->lUserScore = pInsureEvent->lUserScore;
		pGlobalUserData->lUserInsure = pInsureEvent->lUserInsure;

		//更新信息
		m_PlazaUserInfo.UpdateUI();
	}

	//兑换命令
	if(pInsureEvent->wCommandID==INSURE_COMMAND_EXCHANGE)
	{
		//显示兑换
		ASSERT(CGlobalUnits::GetInstance());
		CGlobalUnits::GetInstance()->PerformShowExchange();		
	}

	//释放对象
	if(pInsureEvent->bHeapData==true) SafeDelete(pInsureEvent);

	return 0;
}



//自定义消息
LRESULT CPlatformFrame::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (uMsg == WM_MENUCLICK)
	{
		CStdString *strMenuName = (CStdString*)wParam;	

		if ( *strMenuName == szMenuSettingControlName) 
		{
			ASSERT(CGlobalUnits::GetInstance());
			CGlobalUnits::GetInstance()->PerformShowSetup();	
	
		}
		else if( *strMenuName == szMenuOnlineControlName )
		{
			ASSERT(CGlobalUnits::GetInstance());
			CGlobalUnits::GetInstance()->PerformShowFeedBack();		
		}
		else if( *strMenuName == szMenuContactControlName )
		{
			ASSERT(CGlobalUnits::GetInstance());
			CGlobalUnits::GetInstance()->PerformShowContact();			
		}
		else if( *strMenuName == szMenuSwitchControlName )
		{
			CPaintManagerUI::InvokAllCloseCallBack();
			//投递消息
			PostMessage(WM_COMMAND,IDM_SWITCH_ACCOUNTS,0);
		}
		else if( *strMenuName == szMenuReStoreControlName )
		{
			//投递消息
			PostMessage(WM_COMMAND,IDM_RESTORE,0);
		}
		else if( *strMenuName == szMenuMinControlName )
		{
			//投递消息
			PostMessage(WM_COMMAND,IDM_MINIMIZE,0);
		}
		else if( *strMenuName == szMenuMaxControlName )
		{
			//投递消息
			PostMessage(WM_COMMAND,IDM_MAXIMIZE,0);

		}
		else if( *strMenuName == szMenuExitControlName )
		{
			CPaintManagerUI::InvokAllCloseCallBack();

			CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
			tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();
			pGlobalUserData->dwUserID = 0L;

			//关闭窗口
			AfxGetMainWnd()->PostMessage(WM_CLOSE,0,0);
		}
		else if( *strMenuName == szMenuShowMainControlName )
		{
			//投递消息
			PostMessage(WM_COMMAND,IDM_SHOW_LOGON,0);
		}
		delete strMenuName;
	}
	//else if(lParam == WM_RBUTTONUP)
	//{
	//	//显示菜单
	//	CPoint MousePoint;
	//	GetCursorPos(&MousePoint);

	//	CMenuWnd* pMenu = new CMenuWnd();
	//	pMenu->Init(NULL, _T("PlazaMenuTray.xml"),MousePoint, &m_PaintManager, NULL,eMenuAlignment_Bottom);
	//}

	return 1L;
}

void CPlatformFrame::OnWkeURLHidden()
{
	CWkeBrowserUI* pActiveXUI1 = static_cast<CWkeBrowserUI*>(GetControlByName(szWebBrowserNoticeControlName));
	if( pActiveXUI1 ) 
	{
		pActiveXUI1->RunJS(TEXT("document.body.style.overflow='hidden'"));
	}

	CWkeBrowserUI* pActiveXUI2 = static_cast<CWkeBrowserUI*>(GetControlByName(szWebBrowserUserRankControlName));
	if( pActiveXUI2 ) 
	{
		pActiveXUI2->RunJS(TEXT("document.body.style.overflow='hidden'"));
	}
}


//////////////////////////////////////////////////////////////////////////////////

