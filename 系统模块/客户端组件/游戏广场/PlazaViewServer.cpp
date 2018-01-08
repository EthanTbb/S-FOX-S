#include "StdAfx.h"
#include "GamePlaza.h"
#include "PlazaViewServer.h"

//数据控件
#include "GlobalUnits.h"
#include "PlatformEvent.h"

//界面控件
#include "PlatformFrame.h"

//对话框类
#include "DlgSearchUser.h"
#include "DlgServerManager.h"
#include "DlgTablePassword.h"
#include "DlgInputPassword.h"
#include "DlgCollocateRoom.h"
#include "DlgIssueMessage.h"
#include "DlgInformation.h"
//////////////////////////////////////////////////////////////////////////////////

//常量定义
#define PROPERTY_CY					89									//游戏道具
#define PUBLICIZE_CY				90									//游戏广告
#define INCISE_SCALE				47/100								//界面分割
#define RIGHT_VIEW_WIDTH			260									//右视图宽度

//动画大小
#define FALSH_MOVIE_CX				300									//动画宽度
#define FALSH_MOVIE_CY				300									//动画高度

//常量定义
#define SPLITTER_CY					3									//拆分高度
#define SPLITTER_CX					8									//拆分宽度
#define TABLE_BUTTON_SX				5									//按钮空隙
#define	TABLE_BUTTON_CX				330									//按钮区域

//控件标识
#define IDC_CHAT_CONTROL			100									//聊天控制
#define IDC_SERVER_TABLE			101									//桌子控件
#define IDC_GAME_PROPERTY			102									//游戏道具
#define IDC_WEB_PUBLICIZE			103									//浏览控件

//时钟定义
#define IDI_GUIDE_TASK				200									//引领任务
#define TIME_GUIDE_TASK				3000								//引领任务

//网络中断
#define CLOSE_NETWORK_DOWN          88                                  //网络中断

//////////////////////////////////////////////////////////////////////////////////
//控件定义

//按钮控件
const TCHAR * szButtonQuitServerControlName=TEXT("ButtonClose");
const TCHAR * szButtonTableButton1ControlName=TEXT("ButtonTable1");
const TCHAR * szButtonTableButton2ControlName=TEXT("ButtonTable2");
const TCHAR * szButtonTableButton3ControlName=TEXT("ButtonTable3");
const TCHAR * szButtonTableButton4ControlName=TEXT("ButtonTable4");
const TCHAR * szButtonTableButton5ControlName=TEXT("ButtonTable5");

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CPlazaViewServer, CFGuiDialog)

	//系统消息
	ON_WM_SIZE()
	ON_WM_NCDESTROY()

	////管理命令
	ON_COMMAND(IDM_MANAGER_SERVER, OnManageServer)
	ON_COMMAND(IDM_MANAGER_USER, OnManageUser)
	ON_COMMAND(IDM_MANAGER_MESSAGE, OnManageSystemMsg)

	//自定消息
	ON_MESSAGE(WM_ADD_FRIEND, OnWhisperAddFriend)
	ON_MESSAGE(WM_SET_DETEST, OnWhisperSetDetest)

	ON_WM_TIMER()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CPlazaViewServer::CPlazaViewServer()
{
	//界面变量
	m_bCreateFlag=false;

	//辅助变量
	m_dwChatTime=0L;
	m_wReqTableID=INVALID_TABLE;
	m_wReqChairID=INVALID_CHAIR;
	m_wFindTableID=INVALID_TABLE;
	m_wInvitedTableID=INVALID_TABLE;
	m_bUserInRoom=false;

	//用户属性
	m_dwUserRight=0L;
	m_dwMasterRight=0L;
	m_pIMySelfUserItem=NULL;

	//游戏桌子
	m_pITableViewFrame=NULL;

	//房间属性
	m_wServerType=0L;
	m_dwServerRule=0L;
	m_ServiceStatus=ServiceStatus_Unknow;
	m_wAVServerPort=0;
	m_dwAVServerAddr=0;
	m_szServerPassword[0]=0;

	//配置参数
	m_pParameterGame=NULL;
	m_pParameterServer=NULL;

	//控件变量
	m_pDlgSearchTable=NULL;
	m_pDlgInsureServer=NULL;
	m_pDlgServerManager=NULL;
	m_pDlgExchangeServer=NULL;
	m_pPlazaViewServerCtrl=NULL;
	m_pDlgTaskServerCenter=NULL;
	m_pDlgCheckIn =NULL;
	m_pDlgBaseEnsure = NULL;
	m_pDlgShopServer = NULL;
	m_pDlgBagServer = NULL;
	m_pDlgPropertyUse = NULL;
	m_pDlgCheckIn=NULL;

	m_bPlaying = false;

	//管理控件
	m_pDlgManageUser=NULL;

	//设置组件
	m_DlgStatus.SetStatusViewSink(this);

	//配置信息
	m_cbColumnCount=0;
	ZeroMemory(m_ColumnItem,sizeof(m_ColumnItem));


	//房间属性
	ZeroMemory(&m_GameKind,sizeof(m_GameKind));
	ZeroMemory(&m_GameServer,sizeof(m_GameServer));

	//桌子资源
	tagEncircleResource	EncircleTable;
	EncircleTable.pszImageTL=MAKEINTRESOURCE(IDB_TABLE_TL);
	EncircleTable.pszImageTM=MAKEINTRESOURCE(IDB_TABLE_TM);
	EncircleTable.pszImageTR=MAKEINTRESOURCE(IDB_TABLE_TR);
	EncircleTable.pszImageML=MAKEINTRESOURCE(IDB_TABLE_ML);
	EncircleTable.pszImageMR=MAKEINTRESOURCE(IDB_TABLE_MR);
	EncircleTable.pszImageBL=MAKEINTRESOURCE(IDB_TABLE_BL);
	EncircleTable.pszImageBM=MAKEINTRESOURCE(IDB_TABLE_BM);
	EncircleTable.pszImageBR=MAKEINTRESOURCE(IDB_TABLE_BR);
	m_TableEncircle.InitEncircleResource(EncircleTable,AfxGetInstanceHandle());

	return;
}

//析构函数
CPlazaViewServer::~CPlazaViewServer()
{
	if (m_ImageGameName.IsNull()==false) m_ImageGameName.DestroyImage();

	return;
}

//接口查询
VOID * CPlazaViewServer::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(ITCPSocketSink,Guid,dwQueryVer);
	QUERYINTERFACE(IServerViewItem,Guid,dwQueryVer);
	QUERYINTERFACE(IUserManagerSink,Guid,dwQueryVer);
	QUERYINTERFACE(IFlashControlSink,Guid,dwQueryVer);
	QUERYINTERFACE(IGamePropertyUseSink,Guid,dwQueryVer);
	QUERYINTERFACE(ITableViewFrameSink,Guid,dwQueryVer);
	QUERYINTERFACE(IProcessManagerSink,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IServerViewItem,Guid,dwQueryVer);
	return NULL;
}

//消息提醒
void CPlazaViewServer::Notify(TNotifyUI &  msg)
{
	//获取对象
	CControlUI * pControlUI = msg.pSender;

	//点击事件
	if (lstrcmp(msg.sType, TEXT("click")) == 0)
	{
		if(lstrcmp(pControlUI->GetName(), szButtonQuitServerControlName)==0) 
		{
			PostMessage(WM_COMMAND,IDM_DELETE_SERVER_ITEM,0L);
			return;
		}
		else if(lstrcmp(pControlUI->GetName(), szButtonTableButton1ControlName)==0)
		{
			return OnTableButton1Clicked();
		}
		else if(lstrcmp(pControlUI->GetName(), szButtonTableButton2ControlName)==0)
		{
			return OnTableButton2Clicked();
		}
		else if(lstrcmp(pControlUI->GetName(), szButtonTableButton3ControlName)==0)
		{
			return OnTableButton3Clicked();
		}
		else if(lstrcmp(pControlUI->GetName(), szButtonTableButton4ControlName)==0)
		{
			return OnTableButton4Clicked();
		}	
		else if(lstrcmp(pControlUI->GetName(), szButtonTableButton5ControlName)==0)
		{
			return OnTableButton5Clicked();
		}
	}

	return;
}

//开始绘画
void CPlazaViewServer::OnBeginPaintWindow(HDC hDC)
{
	//获取对象
	CDC * pDC = CDC::FromHandle(hDC);

	//获取位置
	CRect rcClient;
	GetClientRect(&rcClient);

	//绘画桌子
	CRect rcTable;
	rcTable.top=0;
	rcTable.left=0;
	rcTable.right=rcClient.right;
	rcTable.bottom=rcClient.Height();
	m_TableEncircle.DrawEncircleFrame(pDC,rcTable);

	//获取对象
	ASSERT(CSkinRenderManager::GetInstance()!=NULL);
	CSkinRenderManager * pSkinRenderManager=CSkinRenderManager::GetInstance();

	//设置 DC
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(RGB(13,45,75));
	pDC->SelectObject(CSkinResourceManager::GetInstance()->GetDefaultFont());

	//位置信息
	CRect rcTextTitle(20,10,120,23);
	TCHAR szTextTitle[64]=TEXT("您的位置：");

	//设置 DC
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(RGB(154,109,43));
	pDC->DrawText(szTextTitle,lstrlen(szTextTitle),&rcTextTitle,DT_END_ELLIPSIS|DT_VCENTER|DT_SINGLELINE);

	//标题信息
	pDC->SetTextColor(RGB(219,183,131));
	GetWindowText(szTextTitle,CountArray(szTextTitle));

	//绘画标题
	rcTextTitle.SetRect(80,10,rcClient.Width()-TABLE_BUTTON_CX-8,22);
	pDC->DrawText(szTextTitle,lstrlen(szTextTitle),&rcTextTitle,DT_END_ELLIPSIS|DT_VCENTER|DT_SINGLELINE);
}

//控件绑定
VOID CPlazaViewServer::DoDataExchange(CDataExchange * pDX)
{
	__super::DoDataExchange(pDX);

	return;
}

//创建函数
BOOL CPlazaViewServer::OnInitDialog()
{
	__super::OnInitDialog();

	//变量定义
	CRect rcCreate(0,0,0,0);
	HINSTANCE hInstance=AfxGetInstanceHandle();



	//动画控件
	IUnknownEx * pIUnknownEx=QUERY_ME_INTERFACE(IFlashControlSink);
	m_FlashControl.CreateControl(this,CRect(0,0,FALSH_MOVIE_CX,FALSH_MOVIE_CY),pIUnknownEx);
	m_FlashControl.ShowControl(SW_HIDE);

	//提示控件
	m_ToolTipCtrl.Create(this);
	m_ToolTipCtrl.Activate(TRUE);

	//获取对象
	ASSERT(CPlatformFrame::GetInstance()!=NULL);
	m_pPlazaViewServerCtrl = CPlatformFrame::GetInstance()->GetPlazaViewServerCtrl();	

	//创建组件
	if (m_GameLevelParserModule.CreateInstance()==false) throw TEXT("等级解释组件创建失败！");
	if (m_PlazaUserManagerModule.CreateInstance()==false) throw TEXT("用户管理组件创建失败！");

	//设置组件
	m_ProcessManager.SetProcessManagerSink(QUERY_ME_INTERFACE(IUnknownEx));

	//配置组件
	CUserInformation * pUserInformation=CUserInformation::GetInstance();
	if (m_PlazaUserManagerModule->SetUserManagerSink(pIUnknownEx)==false) throw TEXT("用户管理组件配置失败！");
	if (m_PlazaUserManagerModule->SetUserInformation(pUserInformation)==false) throw TEXT("用户管理组件配置失败！");

	//设置变量
	m_bCreateFlag=true;

	//设置界面
	InitServerViewCtrl(CSkinRenderManager::GetInstance());

	//注册事件
	CPlatformEvent * pPlatformEvent=CPlatformEvent::GetInstance();
	if (pPlatformEvent!=NULL) pPlatformEvent->RegisterEventWnd(m_hWnd);

	return TRUE;
}

//消息过虑
BOOL CPlazaViewServer::PreTranslateMessage(MSG * pMsg)
{
	//提示消息
	if (m_ToolTipCtrl.m_hWnd!=NULL)
	{
		m_ToolTipCtrl.RelayEvent(pMsg);
	}

	//按键消息
	if (pMsg->message==WM_KEYDOWN)
	{
		//取消按钮
		if (pMsg->wParam==VK_ESCAPE)
		{
			return TRUE;
		}

		//回车按钮
		if (pMsg->wParam==VK_RETURN)
		{
			PostMessage(WM_COMMAND,IDC_BT_SEND_CHAT,0);
			return TRUE;
		}
	}

	return __super::PreTranslateMessage(pMsg);
}

//命令函数
BOOL CPlazaViewServer::OnCommand(WPARAM wParam, LPARAM lParam)
{
	//变量定义
	UINT nCommandID=LOWORD(wParam);

	//按钮命令
	/*switch (nCommandID)
	{*/
	//case IDC_BT_TABLE_BUTTON_4:		//游戏设置
	//	{
	//		//构造变量
	//		CCollocateRoom DlgCollocate;
	//		DlgCollocate.InitCollocate(GetParameterGame(),GetParameterServer());

	//		//显示配置
	//		if (DlgCollocate.DoModal()==IDOK)
	//		{
	//			//发送规则
	//			SendUserRulePacket();
	//		}
	//		return TRUE;
	//	}
	
	//菜单命令
	switch (nCommandID)
	{
	case IDM_GAME_RULE:				//游戏规则
		{
			//获取对象
			CGlobalWebLink * pGlobalWebLink=CGlobalWebLink::GetInstance();

			//构造地址
			TCHAR szBrowser[MAX_PATH]=TEXT("");
			_sntprintf(szBrowser,CountArray(szBrowser),TEXT("%s/GameRule.aspx?GameID=%ld&KindID=%ld"),pGlobalWebLink->GetPlatformLink(),m_GameKind.wGameID,m_GameServer.wKindID);

			//打开页面
			ShellExecute(NULL,TEXT("OPEN"),szBrowser,NULL,NULL,SW_NORMAL);

			return TRUE;
		}
	case IDM_GAME_CHART:			//游戏排行
		{
			//变量定义
			CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
			tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

			//获取对象
			CGlobalWebLink * pGlobalWebLink=CGlobalWebLink::GetInstance();

			//构造地址
			TCHAR szBrowser[256]=TEXT("");
			_sntprintf(szBrowser,CountArray(szBrowser),TEXT("%s/Rank/GameChart.aspx?GameID=%ld&KindID=%ld&ServerID=%ld&UserID=%ld"),pGlobalWebLink->GetPlatformLink(),m_GameKind.wGameID,m_GameServer.wKindID,m_GameServer.wServerID,pGlobalUserData->dwUserID);

			//打开页面
			ShellExecute(NULL,TEXT("OPEN"),szBrowser,NULL,NULL,SW_NORMAL);

			return TRUE;
		}
	case IDM_DELETE_SERVER_ITEM:	//关闭房间
		{
			if(IsPlayingMySelf() && lParam!=CLOSE_NETWORK_DOWN)
			{
				//提示消息
				CDlgInformation Information(this);
				INT nRes=0;
				if (m_wServerType!=GAME_GENRE_MATCH)
					nRes=Information.ShowMessageBox(TEXT("您正在游戏中，强行退出将被扣分，确实要强退吗？"),MB_ICONQUESTION|MB_YESNO,0);
				else
					nRes=Information.ShowMessageBox(TEXT("您正在比赛中，强行退出将会被系统托管，输赢自负，确实要强退吗？"),MB_ICONQUESTION|MB_YESNO,0);
				if (nRes!=IDYES)
				{
					return TRUE;
				}
				OnGameProcessClose(0);
				m_pIMySelfUserItem->GetUserInfo()->cbUserStatus=US_FREE;
			}

			//中断连接
			if (m_TCPSocketModule.GetInterface()!=NULL)
			{
				m_TCPSocketModule->CloseSocket();
				m_ServiceStatus=ServiceStatus_NetworkDown;
			}

			m_bPlaying =false;

			//关闭房间
			CPlatformFrame * pPlatformFrame=CPlatformFrame::GetInstance();
			if (pPlatformFrame!=NULL) pPlatformFrame->DeleteServerViewItem();

			return TRUE;
		}
	}

	return __super::OnCommand(wParam,lParam);
}

//鼠标双击
VOID CPlazaViewServer::OnDButtonHitTable(WORD wTableID)
{
	return;
}

//左键按下
VOID CPlazaViewServer::OnLButtonHitTable(WORD wTableID, WORD wChairID)
{
	//动作过虑
	if (m_ServiceStatus!=ServiceStatus_ServiceIng) return;
	if ((wTableID==INVALID_TABLE)||(wChairID==INVALID_CHAIR)) return;
	if ((wTableID==m_wReqTableID)&&(wChairID==m_wReqChairID)) return;

	//获取用户
	IClientUserItem * pTableUserItem=NULL;
	if (wChairID!=INDEX_ENTER_CHAIR) pTableUserItem=m_pITableViewFrame->GetClientUserItem(wTableID,wChairID);

	//随机座位
	if (CServerRule::IsAllowAvertCheatMode(m_dwServerRule)==true)
	{
		PerformSitDownAction(INVALID_TABLE,INVALID_CHAIR,false);
		return;
	}

	//消息处理
	if (pTableUserItem!=NULL)
	{
		if (pTableUserItem==m_pIMySelfUserItem)
		{
			//状态过滤
			if (m_pIMySelfUserItem->GetUserStatus()>=US_PLAYING) return;
			if ((m_wReqTableID!=INVALID_TABLE)||(m_wReqChairID!=INVALID_CHAIR)) return;

			//起立动作
			PerformStandUpAction(wTableID,wChairID);
		}
		else
		{
			//状态过滤
			WORD wMeTableID=m_pIMySelfUserItem->GetTableID();
			WORD wMeChairID=m_pIMySelfUserItem->GetChairID();
			if ((wMeTableID==wTableID)&&(wMeChairID==wChairID)) return;
			if ((m_wReqTableID!=INVALID_TABLE)||(m_wReqChairID!=INVALID_CHAIR)) return;

			//旁观动作
			PerformLookonAction(wTableID,wChairID);
		}
	}
	else
	{
		//激活界面
		if (wChairID==INDEX_ENTER_CHAIR)
		{
			//变量定义
			WORD wUserTableID=m_pIMySelfUserItem->GetTableID();
			BYTE cbUserStatus=m_pIMySelfUserItem->GetUserStatus();

			//激活界面
			if ((wTableID==wUserTableID)&&(cbUserStatus!=US_LOOKON))
			{
				m_ProcessManager.SendProcessData(IPC_CMD_GF_CONTROL,IPC_SUB_GF_ACTIVE_PROCESS);
				return;
			}
		}

		//坐下动作
		PerformSitDownAction(wTableID,wChairID,true);
	}

	return;
}

//右键按下
VOID CPlazaViewServer::OnRButtonHitTable(WORD wTableID, WORD wChairID)
{
	//点击位置
	if ((wTableID!=INVALID_TABLE)&&(wChairID!=INVALID_CHAIR)&&(wChairID!=INDEX_ENTER_CHAIR))
	{
		//获取用户
		IClientUserItem * pIClientUserItem=m_pITableViewFrame->GetClientUserItem(wTableID,wChairID);

		//用户菜单
		if (pIClientUserItem!=NULL)
		{
			TrackUserItemMenu(pIClientUserItem);
		}

		return;
	}

	return;
}

//桌子按钮
VOID CPlazaViewServer::OnHitTableButton(BYTE cbButton)
{
	//按钮处理
	switch (cbButton)
	{
	case BT_JOIN_GAME:		//加入游戏
	case BT_JOIN_MATCH:		//加入比赛
	case BT_GROUP_MATCH:	//分组比赛
		{
			//进入游戏
			if ((m_pIMySelfUserItem!=NULL)&&(m_pIMySelfUserItem->GetTableID()==INVALID_TABLE))
			{
				PerformSitDownAction(INVALID_TABLE,INVALID_CHAIR,false);
			}

			break;
		}
	case BT_QUIT_MATCH:	//退出游戏
		{
			//退出游戏
			if ((m_pIMySelfUserItem!=NULL)&&(m_pIMySelfUserItem->GetTableID()==INVALID_TABLE))
			{
				m_TCPSocketModule->SendData(MDM_GR_MATCH,SUB_GR_LEAVE_MATCH);
			}
		}
	case BT_REVIVE_MATH:
		{
			//构造结构
			if ((m_pIMySelfUserItem!=NULL)&&(m_pIMySelfUserItem->GetTableID()==INVALID_TABLE))
			{
				//发送数据
				m_TCPSocketModule->SendData(MDM_GR_MATCH,SUB_GR_MATCH_USER_REVIVE,NULL,0);
			}
		}
	}
}

//连接事件
bool CPlazaViewServer::OnEventTCPSocketLink(WORD wSocketID, INT nErrorCode)
{
	//错误提示
	if (nErrorCode!=0L)
	{
		//关闭提示
		m_DlgStatus.HideStatusWindow();

		//关闭房间
		PostMessage(WM_COMMAND,IDM_DELETE_SERVER_ITEM,0);

		//提示消息
		CDlgInformation Information(this);
		Information.ShowMessageBox(m_GameServer.szServerName,TEXT("很抱歉，游戏房间连接失败，请稍后再试或留意网站公告！"),MB_ICONSTOP);

		return true;
	}

	//发送登录
	SendLogonPacket(m_szServerPassword);

	//设置状态
	SetServiceStatus(ServiceStatus_Validate);

	return true;
}

//关闭事件
bool CPlazaViewServer::OnEventTCPSocketShut(WORD wSocketID, BYTE cbShutReason)
{
	//关闭状态
	m_DlgStatus.HideStatusWindow();

	//关闭游戏
	m_ProcessManager.CloseGameProcess(false);

	//关闭处理
	if (cbShutReason!=SHUT_REASON_NORMAL)
	{
		if (m_ServiceStatus==ServiceStatus_ServiceIng)
		{
			//获取指针
			IStringMessage * pIStringMessage = m_pPlazaViewServerCtrl->GetStringMessage();
			ASSERT(pIStringMessage!=NULL);

			//提示消息
			pIStringMessage->InsertSystemString(TEXT("与游戏服务器的连接中断了"));

			//关闭提示
			CDlgInformation Information(this);
			INT_PTR nResult=Information.ShowMessageBox(m_GameServer.szServerName,TEXT("与游戏服务器的连接已经中断，是否关闭当前游戏房间？"),
				MB_ICONERROR|MB_YESNO|MB_DEFBUTTON2,0L);

			//关闭房间
			if (nResult==IDYES)
			{
				PostMessage(WM_COMMAND,IDM_DELETE_SERVER_ITEM);
			}
		}
		else
		{
			//关闭提示
			CDlgInformation Information(this);
			Information.ShowMessageBox(m_GameServer.szServerName,TEXT("由于意外的原因，房间连接失败，请稍后重新尝试！"),MB_ICONERROR);

			//关闭房间
			PostMessage(WM_COMMAND,IDM_DELETE_SERVER_ITEM,0);
		}
	}

	//设置状态
	m_ServiceStatus=ServiceStatus_NetworkDown;

	return true;
}

//读取事件
bool CPlazaViewServer::OnEventTCPSocketRead(WORD wSocketID, TCP_Command Command, VOID * pData, WORD wDataSize)
{
	bool br=false;
	switch (Command.wMainCmdID)
	{
	case MDM_GR_LOGON:		//登录消息
		{
			br=OnSocketMainLogon(Command.wSubCmdID,pData,wDataSize);

			return true;
		}
	case MDM_GR_CONFIG:		//配置信息
		{
			br=OnSocketMainConfig(Command.wSubCmdID,pData,wDataSize);

			return true;
		}
	case MDM_GR_USER:		//用户信息
		{
			br=OnSocketMainUser(Command.wSubCmdID,pData,wDataSize);

			return true;
		}
	case MDM_GR_STATUS:		//状态信息
		{
			br=OnSocketMainStatus(Command.wSubCmdID,pData,wDataSize);

			return true;
		}
	case MDM_GR_INSURE:		//银行消息
		{
			br=OnSocketMainInsure(Command.wSubCmdID,pData,wDataSize);

			return true;
		}
	case MDM_GR_BASEENSURE:		//低保消息
		{
			br=OnSocketMainBaseEnsure(Command.wSubCmdID,pData,wDataSize);

			return true;
		}
	case MDM_GR_MEMBER:
		{
			br=OnSocketMainMember(Command.wSubCmdID,pData,wDataSize);
			return true;
		}
	case MDM_GR_CHECKIN:		//签到消息
		{
			br=OnSocketMainCheckIn(Command.wSubCmdID,pData,wDataSize);

			return true;
		}
	case MDM_GR_TASK:		//任务消息
		{
			br=OnSocketMainTask(Command.wSubCmdID,pData,wDataSize);

			return true;
		}
	case MDM_GR_EXCHANGE:	//兑换消息
		{
			br=OnSocketMainExchange(Command.wSubCmdID,pData,wDataSize);

			return true;
		}
	case MDM_GR_PROPERTY:	//道具消息
		{
			br=OnSocketMainProperty(Command.wSubCmdID,pData,wDataSize);

			return true;
		}
	case MDM_GR_MANAGE:		//管理消息
		{
			br=OnSocketMainManager(Command.wSubCmdID,pData,wDataSize);

			return true;
		}
	case MDM_CM_SYSTEM:		//系统消息
		{
			br=OnSocketMainSystem(Command.wSubCmdID,pData,wDataSize);

			return true;
		}
	case MDM_GF_GAME:		//游戏消息
	case MDM_GF_FRAME:		//框架消息
		{
			br=OnSocketMainGameFrame(Command.wMainCmdID,Command.wSubCmdID,pData,wDataSize);

			return true;
		}
	case MDM_GR_MATCH:		//比赛消息
		{
			br=OnSocketMainMatch(Command.wSubCmdID,pData,wDataSize);

			return true;
		}
	}

	return true;
}

//播放完成
VOID CPlazaViewServer::OnEventFlashFinish()
{
	m_FlashControl.ShowControl(SW_HIDE);
}

//购买道具
bool CPlazaViewServer::OnEventBuyProperty(LPCTSTR pszNickName, WORD wItemCount, WORD wPropertyIndex)
{
	//效验参数
	ASSERT((pszNickName!=NULL)&&(wItemCount>0));
	if ((pszNickName==NULL)||(wItemCount==0)) return false;

	//执行购买
	return PerformBuyProperty(PT_ISSUE_AREA_SERVER,pszNickName,wItemCount,wPropertyIndex);
}

bool CPlazaViewServer::OnEventBuyPropertyPrep(LPCTSTR pszNickName,WORD wPropertyIndex,LPTSTR pszMessage)
{
	//防作弊场
	if (CServerRule::IsAllowAvertCheatMode(m_dwServerRule))
	{
		TCHAR szMessage[128]=TEXT("防作弊场不允许赠送礼物！");
		lstrcpyn(pszMessage,szMessage,CountArray(szMessage));

		return false;
	}

	//检查用户
	IClientUserItem * pIClientUserItem=m_PlazaUserManagerModule->SearchUserByNickName(pszNickName);
	if(pIClientUserItem==NULL)
	{
		TCHAR szMessage[128]=TEXT("您指定的使用对象不在本房间中或不存在！");
		lstrcpyn(pszMessage,szMessage,CountArray(szMessage));

		return false;
	}

	return true;
}

//使用道具
bool CPlazaViewServer::OnEventUseProperty(WORD wPropertyIndex)
{
	////发送喇叭
	//if(wPropertyIndex==PROPERTY_ID_TRUMPET || wPropertyIndex==PROPERTY_ID_TYPHON)
	//{
	//	//构造结果
	//	CMD_GR_C_SendTrumpet SendTrumpet;
	//	SendTrumpet.cbRequestArea=PT_ISSUE_AREA_SERVER;
	//	SendTrumpet.wPropertyIndex=wPropertyIndex;
	//	SendTrumpet.TrumpetColor=m_pTrumpetItem->GetTrumpetColor();
	//	m_pTrumpetItem->GetTrumpetContent(SendTrumpet.szTrumpetContent);

	//	//发送消息
	//	m_TCPSocketModule->SendData(MDM_GR_PROPERTY,SUB_GR_PROPERTY_TRUMPET,&SendTrumpet,sizeof(SendTrumpet));
	//}

	return true;
}

//进程错误
bool CPlazaViewServer::OnGameProcessError()
{
	OnGameProcessClose(0);

	return true;
}

//进程关闭
bool CPlazaViewServer::OnGameProcessClose(DWORD dwExitCode)
{
	//效验状态
	ASSERT(m_pIMySelfUserItem!=NULL);
	if (m_pIMySelfUserItem==NULL) return false;

	//变量定义
	WORD wTableID=m_pIMySelfUserItem->GetTableID();
	WORD wChairID=m_pIMySelfUserItem->GetChairID();
	BYTE cbUserStatus=m_pIMySelfUserItem->GetUserStatus();

	//退出游戏
	if ((wTableID!=INVALID_TABLE)&&(m_ServiceStatus==ServiceStatus_ServiceIng))
	{
		SendStandUpPacket(wTableID,wChairID,TRUE);
	}

	m_pITableViewFrame->SetTableStatus(false);

	//还原窗口
	CPlatformFrame * pPlatformFrame=CPlatformFrame::GetInstance();
	pPlatformFrame->PostMessage(WM_COMMAND,IDM_RESTORE,0);

	return true;
}

//进程启动
bool CPlazaViewServer::OnGameProcessCreate(bool bAlreadyExist)
{
	//变量定义
	WORD wTableID=m_pIMySelfUserItem->GetTableID();
	WORD wChairID=m_pIMySelfUserItem->GetChairID();

	//房间配置
	if (bAlreadyExist==false)
	{
		//获取等级
		tagLevelItem LevelItem[64];
		BYTE cbLevelCount=(BYTE)m_GameLevelParserModule->GetGameLevelItem(LevelItem,CountArray(LevelItem));

		//等级配置
		WORD wLevelCount;
		tagGrowLevelConfig GrowLevelItem[60];
		CUserItemElement * pUserItemElement=CUserItemElement::GetInstance();
		wLevelCount=pUserItemElement->GetExperienceInfo(GrowLevelItem,CountArray(GrowLevelItem));

		//发送配置
		m_ProcessManager.SendLevelInfo(LevelItem,cbLevelCount);
		m_ProcessManager.SendColumnInfo(m_ColumnItem,m_cbColumnCount);
		m_ProcessManager.SendGrowLevelConfig(GrowLevelItem,wLevelCount);
		m_ProcessManager.SendGamePropertyTrumpet(m_pPlazaViewServerCtrl->m_dwTrumpetCount,m_pPlazaViewServerCtrl->m_dwTyphonCount);
		m_ProcessManager.SendGamePropertyTypeInfo();
		m_ProcessManager.SendGamePropertyRelatInfo();
		m_ProcessManager.SendGamePropertyInfo();
	}

	//房间信息
	m_ProcessManager.SendServerInfo(m_pIMySelfUserItem,m_dwUserRight,m_dwMasterRight,m_dwServerRule,m_wServerType,m_GameServer,m_wAVServerPort,m_dwAVServerAddr);
	
	//比赛信息
	CServerListData * pServerListData = CServerListData::GetInstance();
	CGameServerItem * pGameServerItem=pServerListData->SearchGameServer(m_GameServer.wServerID);
	if (pGameServerItem!=NULL) m_ProcessManager.SendMatchInfo(pGameServerItem->m_GameMatch);

	//发送用户
	if ((wTableID!=INVALID_TABLE)&&(wChairID!=INVALID_CHAIR))
	{
		//变量定义
		WORD wEnumIndex=0;
		IPlazaUserManager * pIPlazaUserManager=m_PlazaUserManagerModule.GetInterface();

		//发送玩家
		while (true)
		{
			//获取用户
			IClientUserItem * pIClientUserItem=pIPlazaUserManager->EnumUserItem(wEnumIndex++);
			if (pIClientUserItem==NULL) break;

			//发送判断
			if (pIClientUserItem->GetTableID()!=wTableID) continue;
			if (pIClientUserItem->GetUserStatus()==US_LOOKON) continue;

			//发送用户
			m_ProcessManager.SendUserItem(pIClientUserItem);
		};

		//旁观用户
		wEnumIndex=0;
		while (true)
		{
			//获取用户
			IClientUserItem * pIClientUserItem=pIPlazaUserManager->EnumUserItem(wEnumIndex++);
			if (pIClientUserItem==NULL) break;

			//发送判断
			if (pIClientUserItem->GetTableID()!=wTableID) continue;
			if (pIClientUserItem->GetUserStatus()!=US_LOOKON) continue;

			//发送用户
			m_ProcessManager.SendUserItem(pIClientUserItem);
		};
	}

	//配置完成
	m_ProcessManager.SendProcessData(IPC_CMD_GF_CONFIG,IPC_SUB_GF_CONFIG_FINISH);

	return true;
}

//进程数据
bool CPlazaViewServer::OnProcessManagerData(WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	//用户属性
	if ((wMainCmdID==IPC_CMD_GF_USER_INFO)&&(wSubCmdID==IPC_SUB_GF_USER_ATTRIB))
	{
		//效验参数
		ASSERT(wDataSize==sizeof(IPC_GF_UserAttrib));
		if (wDataSize!=sizeof(IPC_GF_UserAttrib)) return false;

		//寻找用户
		IPC_GF_UserAttrib * pUserAttrib=(IPC_GF_UserAttrib *)pData;
		IClientUserItem * pIClientUserItem=m_PlazaUserManagerModule->SearchUserByUserID(pUserAttrib->dwUserID);

		//用户判断
		ASSERT(pIClientUserItem!=NULL);
		if (pIClientUserItem==NULL) return true;

		//变量定义
		ASSERT(CUserInformation::GetInstance()!=NULL);
		CUserInformation * pUserInformation=CUserInformation::GetInstance();

		//设置关系
		m_PlazaUserManagerModule->UpdateUserItemAttrib(pIClientUserItem,&pUserAttrib->UserAttrib);
		pUserInformation->InsertCompanionInfo(pIClientUserItem,pUserAttrib->UserAttrib.cbCompanion);

		return true;
	}

	//踢出用户
	if ((wMainCmdID==IPC_CMD_GF_USER_INFO)&&(wSubCmdID==IPC_SUB_GF_KICK_USER))
	{
		//效验参数
		ASSERT(wDataSize==sizeof(IPC_GF_KickUser));
		if (wDataSize!=sizeof(IPC_GF_KickUser)) return false;

		//寻找用户
		IPC_GF_KickUser * pKickUser=(IPC_GF_KickUser *)pData;
		IClientUserItem * pIClientUserItem=m_PlazaUserManagerModule->SearchUserByUserID(pKickUser->dwTargetUserID);

		//用户判断
		ASSERT(pIClientUserItem!=NULL);
		if (pIClientUserItem==NULL) return true;

		//百人游戏
		if((m_pIMySelfUserItem->GetMasterOrder()==0) && (m_pITableViewFrame->GetChairCount() >= INDEX_ENTER_CHAIR))
		{
			CDlgInformation Information(this);
			Information.ShowMessageBox(TEXT("很抱歉，百人游戏不许踢人！"),MB_ICONQUESTION);
			return true;
		}

		//变量定义
		CMD_GR_KickUser KickUser;
		KickUser.dwTargetUserID=pKickUser->dwTargetUserID;

		//发送数据
		m_TCPSocketModule->SendData(MDM_GR_USER,SUB_GR_USER_KICK_USER,&KickUser,sizeof(KickUser));

		return true;
	}

	//快速换位
	if ((wMainCmdID==IPC_CMD_GF_USER_INFO)&&(wSubCmdID==IPC_SUB_GF_QUICK_TRANSPOS))
	{
		//变量定义
		ASSERT(CParameterGlobal::GetInstance()!=NULL);
		CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();
		BYTE cbActionHitAutoJoin=pParameterGlobal->m_cbActionHitAutoJoin;

		//执行动作
		pParameterGlobal->m_cbActionHitAutoJoin=ACTION_SEARCH_TABLE;

		//发送命令
		SendMessage(WM_COMMAND,IDC_BT_TABLE_BUTTON_1,0L);

		//恢复设置
		pParameterGlobal->m_cbActionHitAutoJoin=cbActionHitAutoJoin;

		return true;
	}

	if ((wMainCmdID==IPC_CMD_GF_MATCH_INFO)&&(wSubCmdID==IPC_SUB_GF_EXIT_MATCH))
	{
		//发送数据
		m_TCPSocketModule->SendData(MDM_GR_MATCH,SUB_GR_LEAVE_MATCH);

		return true;
	}

	//购买消息
	if ((wMainCmdID==IPC_CMD_GF_PROPERTY_INFO))
	{
		//购买道具
		if(wSubCmdID==IPC_SUB_GF_BUY_PROPERTY)
		{
			//效验参数
			ASSERT(wDataSize==sizeof(IPC_GF_BuyProperty));
			if (wDataSize!=sizeof(IPC_GF_BuyProperty)) return false;

			//提取数据
			IPC_GF_BuyProperty * pBuyProperty=(IPC_GF_BuyProperty *)pData;
			ASSERT(pBuyProperty!=NULL);
			if(pBuyProperty==NULL) return false;

			//执行购买
			PerformBuyProperty(PT_ISSUE_AREA_GAME,pBuyProperty->szNickName,pBuyProperty->wItemCount,pBuyProperty->wPropertyIndex);
		}

		//喇叭道具
		if(wSubCmdID==IPC_SUB_GR_PROPERTY_TRUMPET)
		{
			//效验参数
			ASSERT(wDataSize==sizeof(IPC_GR_PropertyTrumpet));
			if (wDataSize!=sizeof(IPC_GR_PropertyTrumpet)) return false;

			//提取数据
			IPC_GR_PropertyTrumpet * pPropertyTrumpet=(IPC_GR_PropertyTrumpet *)pData;
			ASSERT(pPropertyTrumpet!=NULL);
			if(pPropertyTrumpet==NULL) return false;

			//构造结构
			CMD_GR_C_SendTrumpet SendTrumpet;
			SendTrumpet.cbRequestArea=PT_ISSUE_AREA_GAME;
			SendTrumpet.wPropertyIndex=pPropertyTrumpet->wPropertyIndex;
			SendTrumpet.TrumpetColor=pPropertyTrumpet->TrumpetColor;
			CopyMemory(SendTrumpet.szTrumpetContent,pPropertyTrumpet->szTrumpetContent,sizeof(SendTrumpet.szTrumpetContent));

			//发送消息
			m_TCPSocketModule->SendData(MDM_GR_PROPERTY,SUB_GR_PROPERTY_TRUMPET,&SendTrumpet,sizeof(SendTrumpet));
		}

        return true;                
	}

	//网络消息
	if ((wMainCmdID==IPC_CMD_GF_SOCKET)&&(wSubCmdID==IPC_SUB_GF_SOCKET_SEND))
	{
		//效验数据
		ASSERT(wDataSize>=sizeof(TCP_Command));
		if (wDataSize<sizeof(TCP_Command)) return false;

		//变量定义
		IPC_GF_SocketSend * pSocketPackage=(IPC_GF_SocketSend *)pData;
		WORD wPacketSize=wDataSize-(sizeof(IPC_GF_SocketSend)-sizeof(pSocketPackage->cbBuffer));

		//处理数据
		if (wPacketSize==0)
		{
			TCP_Command * pCommand=&pSocketPackage->CommandInfo;
			m_TCPSocketModule->SendData(pCommand->wMainCmdID,pCommand->wSubCmdID);
		}
		else 
		{
			TCP_Command * pCommand=&pSocketPackage->CommandInfo;
			if(pCommand->wMainCmdID==MDM_GR_INSURE && pCommand->wSubCmdID==SUB_GR_QUERY_INSURE_INFO)
			{
				//变量定义
				CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
				tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

				//变量定义
				CMD_GR_C_QueryInsureInfoRequest *pQueryInsureInfo=(CMD_GR_C_QueryInsureInfoRequest *)pSocketPackage->cbBuffer;
				pQueryInsureInfo->cbActivityGame=TRUE;
				lstrcpyn(pQueryInsureInfo->szInsurePass,pGlobalUserData->szPassword,CountArray(pQueryInsureInfo->szInsurePass));
			}
			m_TCPSocketModule->SendData(pCommand->wMainCmdID,pCommand->wSubCmdID,pSocketPackage->cbBuffer,wPacketSize);
		}

		return true;
	}

	//错误断言
	ASSERT(FALSE);

	return false;
}

//用户激活
VOID CPlazaViewServer::OnUserItemAcitve(IClientUserItem * pIClientUserItem)
{
	//判断自己
	if (m_pIMySelfUserItem==NULL)
	{
		m_pIMySelfUserItem=pIClientUserItem;
	}

	//设置桌子
	BYTE cbUserStatus=pIClientUserItem->GetUserStatus();
	if ((cbUserStatus>=US_SIT)&&(cbUserStatus!=US_LOOKON))
	{
		WORD wTableID=pIClientUserItem->GetTableID();
		WORD wChairID=pIClientUserItem->GetChairID();
		m_pITableViewFrame->SetClientUserItem(wTableID,wChairID,pIClientUserItem);
	}

	//用户进入
	m_pPlazaViewServerCtrl->OnEventUserEnter(pIClientUserItem);

	return;
}

//用户删除
VOID CPlazaViewServer::OnUserItemDelete(IClientUserItem * pIClientUserItem)
{
	//变量定义
	WORD wLastTableID=pIClientUserItem->GetTableID();
	WORD wLastChairID=pIClientUserItem->GetChairID();
	DWORD dwLeaveUserID=pIClientUserItem->GetUserID();
	tagUserInfo * pMeUserInfo=m_pIMySelfUserItem->GetUserInfo();

	//离开处理
	if ((wLastTableID!=INVALID_TABLE)&&(wLastChairID!=INVALID_CHAIR))
	{
		//获取用户
		IClientUserItem * pITableUserItem=m_pITableViewFrame->GetClientUserItem(wLastTableID,wLastChairID);
		if (pITableUserItem==pIClientUserItem) m_pITableViewFrame->SetClientUserItem(wLastTableID,wLastChairID,NULL);

		//离开通知
		if ((pIClientUserItem==m_pIMySelfUserItem)||(wLastTableID==pMeUserInfo->wTableID))
		{
			tagUserStatus UserStatus;
			UserStatus.cbUserStatus=US_NULL;
			UserStatus.wTableID=INVALID_TABLE;
			UserStatus.wChairID=INVALID_CHAIR;
			m_ProcessManager.SendUserStatus(pIClientUserItem->GetUserID(),UserStatus);
		}
	}

	//私聊窗口
	for (INT_PTR i=0;i<m_DlgWhisperItemArray.GetCount();i++)
	{
		CDlgWhisper * pDlgWhisper=m_DlgWhisperItemArray[i];
		if (pDlgWhisper->m_hWnd!=NULL) pDlgWhisper->OnEventUserLeave(pIClientUserItem);
	}

	//查找窗口
	CDlgSearchUser * pDlgSearchUser=CDlgSearchUser::GetInstance();
	if (pDlgSearchUser!=NULL) pDlgSearchUser->OnUserItemDelete(pIClientUserItem,this);

	//用户进入
	m_pPlazaViewServerCtrl->OnEventUserLeave(pIClientUserItem);

	return;
}

//用户更新
VOID CPlazaViewServer::OnUserFaceUpdate(IClientUserItem * pIClientUserItem)
{
	//变量定义
	tagUserInfo * pUserInfo=pIClientUserItem->GetUserInfo();
	tagCustomFaceInfo * pCustomFaceInfo=pIClientUserItem->GetCustomFaceInfo();

	//用户更新
	m_pPlazaViewServerCtrl->OnEventUserUpdate(pIClientUserItem);

	//查找窗口
	CDlgSearchUser * pDlgSearchUser=CDlgSearchUser::GetInstance();
	if (pDlgSearchUser!=NULL) pDlgSearchUser->OnUserItemUpdate(pIClientUserItem,this);

	//私聊窗口
	for (INT_PTR i=0;i<m_DlgWhisperItemArray.GetCount();i++)
	{
		CDlgWhisper * pDlgWhisper=m_DlgWhisperItemArray[i];
		if (pDlgWhisper->m_hWnd!=NULL) pDlgWhisper->OnEventUserStatus(pIClientUserItem);
	}

	//更新桌子
	if ((pUserInfo->wTableID!=INVALID_TABLE)&&(pUserInfo->cbUserStatus!=US_LOOKON))
	{
		m_pITableViewFrame->UpdateTableView(pUserInfo->wTableID);
	}

	//更新游戏
	if ((pUserInfo->wTableID!=INVALID_TABLE)&&(m_pIMySelfUserItem->GetTableID()==pUserInfo->wTableID))
	{
		m_ProcessManager.SendCustomFace(pUserInfo->dwUserID,pUserInfo->dwCustomID,pCustomFaceInfo);
	}

	return;
}

//用户更新
VOID CPlazaViewServer::OnUserItemUpdate(IClientUserItem * pIClientUserItem, tagUserScore & LastScore)
{
	//变量定义
	tagUserInfo * pUserInfo=pIClientUserItem->GetUserInfo();
	tagUserInfo * pMeUserInfo=m_pIMySelfUserItem->GetUserInfo();

	//用户更新
	m_pPlazaViewServerCtrl->OnEventUserUpdate(pIClientUserItem);

	//查找窗口
	CDlgSearchUser * pDlgSearchUser=CDlgSearchUser::GetInstance();
	if (pDlgSearchUser!=NULL) pDlgSearchUser->OnUserItemUpdate(pIClientUserItem,this);

	//私聊窗口
	for (INT_PTR i=0;i<m_DlgWhisperItemArray.GetCount();i++)
	{
		CDlgWhisper * pDlgWhisper=m_DlgWhisperItemArray[i];
		if (pDlgWhisper->m_hWnd!=NULL) pDlgWhisper->OnEventUserStatus(pIClientUserItem);
	}

	//界面通知
	if (pIClientUserItem==m_pIMySelfUserItem)
	{
		//变量定义
		CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
		tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

		//设置变量
		if(m_wServerType==GAME_GENRE_GOLD)
		{
			pGlobalUserData->lUserScore=pIClientUserItem->GetUserScore();
		}
		
		//更新财富
		pGlobalUserData->lLoveLiness=pUserInfo->lLoveLiness;
		pGlobalUserData->lUserIngot=pIClientUserItem->GetUserIngot();
		pGlobalUserData->lUserInsure=pIClientUserItem->GetUserInsure();
		pGlobalUserData->dUserBeans=pUserInfo->dBeans;

		//事件通知
		CPlatformEvent * pPlatformEvent=CPlatformEvent::GetInstance();
		if (pPlatformEvent!=NULL) pPlatformEvent->PostPlatformEvent(EVENT_USER_INFO_UPDATE,0L);
	}

	//游戏通知
	if ((pMeUserInfo->wTableID!=INVALID_TABLE)&&(pUserInfo->wTableID==pMeUserInfo->wTableID))
	{
		//变量定义
		tagUserScore UserScore;
		ZeroMemory(&UserScore,sizeof(UserScore));

		//设置变量
		UserScore.lScore=pUserInfo->lScore;
		UserScore.lGrade=pUserInfo->lGrade;
		UserScore.lIngot=pUserInfo->lIngot;
		UserScore.lInsure=pUserInfo->lInsure;
		UserScore.dBeans=pUserInfo->dBeans;
		UserScore.dwWinCount=pUserInfo->dwWinCount;
		UserScore.dwLostCount=pUserInfo->dwLostCount;
		UserScore.dwDrawCount=pUserInfo->dwDrawCount;
		UserScore.dwFleeCount=pUserInfo->dwFleeCount;
		UserScore.dwExperience=pUserInfo->dwExperience;
		UserScore.lLoveLiness=pUserInfo->lLoveLiness;
		UserScore.lIntegralCount=pUserInfo->lIntegralCount;

		//发送数据
		m_ProcessManager.SendUserScore(pUserInfo->dwUserID,UserScore);
	}

	return;
}

//用户更新
VOID CPlazaViewServer::OnUserItemUpdate(IClientUserItem * pIClientUserItem, tagUserStatus & LastStatus)
{
	//变量定义
	tagUserInfo * pUserInfo=pIClientUserItem->GetUserInfo();
	tagUserInfo * pMeUserInfo=m_pIMySelfUserItem->GetUserInfo();
	WORD wNowTableID=pIClientUserItem->GetTableID(),wLastTableID=LastStatus.wTableID;
	WORD wNowChairID=pIClientUserItem->GetChairID(),wLastChairID=LastStatus.wChairID;
	BYTE cbNowStatus=pIClientUserItem->GetUserStatus(),cbLastStatus=LastStatus.cbUserStatus;

	//用户更新
	m_pPlazaViewServerCtrl->OnEventUserUpdate(pIClientUserItem);

	//查找窗口
	CDlgSearchUser * pDlgSearchUser=CDlgSearchUser::GetInstance();
	if (pDlgSearchUser!=NULL) pDlgSearchUser->OnUserItemUpdate(pIClientUserItem,this);

	//私聊窗口
	for (INT_PTR i=0;i<m_DlgWhisperItemArray.GetCount();i++)
	{
		CDlgWhisper * pDlgWhisper=m_DlgWhisperItemArray[i];
		if (pDlgWhisper->m_hWnd!=NULL) pDlgWhisper->OnEventUserStatus(pIClientUserItem);
	}

	//桌子离开
	if ((wLastTableID!=INVALID_TABLE)&&((wLastTableID!=wNowTableID)||(wLastChairID!=wNowChairID)))
	{
		IClientUserItem * pITableUserItem=m_pITableViewFrame->GetClientUserItem(wLastTableID,wLastChairID);
		if (pITableUserItem==pIClientUserItem) m_pITableViewFrame->SetClientUserItem(wLastTableID,wLastChairID,NULL);
	}

	//桌子加入
	if ((wNowTableID!=INVALID_TABLE)&&(cbNowStatus!=US_LOOKON)&&((wNowTableID!=wLastTableID)||(wNowChairID!=wLastChairID)))
	{
		//厌恶判断（黑名单）
		if(pUserInfo->dwUserID != pMeUserInfo->dwUserID && cbNowStatus == US_SIT && pMeUserInfo->wTableID == wNowTableID)
		{
			//变量定义
			ASSERT(CParameterGlobal::GetInstance()!=NULL);
			CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();

			//厌恶玩家
			if (pParameterGlobal->m_bLimitDetest==true)
			{
				if (pIClientUserItem->GetUserCompanion()==CP_DETEST)
				{
					CMD_GR_UserRepulseSit UserRepulseSit;
					UserRepulseSit.wTableID=wNowTableID;
					UserRepulseSit.wChairID=wNowChairID;
					UserRepulseSit.dwUserID=pMeUserInfo->dwUserID;
					UserRepulseSit.dwRepulseUserID=pUserInfo->dwUserID;
					m_TCPSocketModule->SendData(MDM_GR_USER,SUB_GR_USER_REPULSE_SIT,&UserRepulseSit,sizeof(UserRepulseSit));
				}
			}
		}
		m_pITableViewFrame->SetClientUserItem(wNowTableID,wNowChairID,pIClientUserItem);
	}

	//桌子状态
	if ((m_pITableViewFrame->GetChairCount() < MAX_CHAIR)&&(wNowTableID!=INVALID_TABLE)&&(wNowTableID==wLastTableID)&&(wNowChairID==wLastChairID))
	{
		m_pITableViewFrame->UpdateTableView(wNowTableID);
	}

	//离开通知
	if ((wLastTableID!=INVALID_TABLE)&&((wNowTableID!=wLastTableID)||(wNowChairID!=wLastChairID)))
	{
		//游戏通知
		if ((pIClientUserItem==m_pIMySelfUserItem)||(wLastTableID==pMeUserInfo->wTableID))
		{
			tagUserStatus UserStatus;
			UserStatus.wTableID=wNowTableID;
			UserStatus.wChairID=wNowChairID;
			UserStatus.cbUserStatus=cbNowStatus;
			m_ProcessManager.SendUserStatus(pUserInfo->dwUserID,UserStatus);
		}

		return;
	}

	//加入处理
	if ((wNowTableID!=INVALID_TABLE)&&((wNowTableID!=wLastTableID)||(wNowChairID!=wLastChairID)))
	{
		//游戏通知
		if ((m_pIMySelfUserItem!=pIClientUserItem)&&(pMeUserInfo->wTableID==wNowTableID))
		{
			ASSERT(wNowChairID!=INVALID_CHAIR);
			m_ProcessManager.SendUserItem(pIClientUserItem);
		}

		//自己判断
		if (m_pIMySelfUserItem==pIClientUserItem)
		{
			//设置变量
			m_wReqTableID=INVALID_TABLE;
			m_wReqChairID=INVALID_CHAIR;

			//启动进程
			m_ProcessManager.CreateGameProcess(m_GameKind.szProcessName);
		}

		return;
	}

	//状态改变
	if ((wNowTableID!=INVALID_TABLE)&&(wNowTableID==wLastTableID)&&(pMeUserInfo->wTableID==wNowTableID))
	{
		//游戏通知
		tagUserStatus UserStatus;
		UserStatus.wTableID=wNowTableID;
		UserStatus.wChairID=wNowChairID;
		UserStatus.cbUserStatus=cbNowStatus;
		m_ProcessManager.SendUserStatus(pUserInfo->dwUserID,UserStatus);

		return;
	}

	return;
}

//用户更新
VOID CPlazaViewServer::OnUserItemUpdate(IClientUserItem * pIClientUserItem, tagUserAttrib & UserAttrib)
{
	//变量定义
	WORD wMeTableID=m_pIMySelfUserItem->GetTableID();
	WORD wUserTableID=pIClientUserItem->GetTableID();

	//用户更新
	m_pPlazaViewServerCtrl->OnEventUserUpdate(pIClientUserItem);

	//查找窗口
	CDlgSearchUser * pDlgSearchUser=CDlgSearchUser::GetInstance();
	if (pDlgSearchUser!=NULL) pDlgSearchUser->OnUserItemUpdate(pIClientUserItem,this);

	//通知游戏
	if ((wMeTableID!=INVALID_TABLE)&&(wMeTableID==wUserTableID))
	{
		//变量定义
		tagUserAttrib UserAttrib;
		UserAttrib.cbCompanion=pIClientUserItem->GetUserCompanion();

		//发送通知
		m_ProcessManager.SendUserAttrib(pIClientUserItem->GetUserID(),UserAttrib);
	}

	//私聊窗口
	for (INT_PTR i=0;i<m_DlgWhisperItemArray.GetCount();i++)
	{
		CDlgWhisper * pDlgWhisper=m_DlgWhisperItemArray[i];
		if (pDlgWhisper->m_hWnd!=NULL) pDlgWhisper->OnEventUserStatus(pIClientUserItem);
	}

	return;
}

//取消连接
VOID CPlazaViewServer::OnStatusCancel()
{
	//关闭房间
	PostMessage(WM_COMMAND,IDM_DELETE_SERVER_ITEM,0);

	return;
}

//显示菜单
VOID CPlazaViewServer::TrackUserItemMenu(IClientUserItem * pIClientUserItem)
{
	m_pPlazaViewServerCtrl->TrackUserItemMenu(pIClientUserItem);
}

//显示菜单
VOID CPlazaViewServer::TrackUserItemMenu(IClientUserItem * pIClientUserItem[], WORD wUserCount)
{
	m_pPlazaViewServerCtrl->TrackUserItemMenu(pIClientUserItem,wUserCount);
}


VOID CPlazaViewServer::ShowIndividual()
{
	CDlgInformation Information;
	Information.ShowMessageBox(TEXT("系统提示"),TEXT("很抱歉，该房间禁止操作个人中心"),MB_ICONERROR);
}

VOID CPlazaViewServer::ShowBaseEnsure()
{
	//效验参数
	ASSERT(m_pIMySelfUserItem!=NULL);
	ASSERT(m_TCPSocketModule.GetInterface()!=NULL);

	//变量定义
	CDlgBaseEnsureServer DlgBaseEnsureServer;
	DlgBaseEnsureServer.Initialization(m_TCPSocketModule.GetInterface(),m_pIMySelfUserItem);

	//设置变量
	m_pDlgBaseEnsure=&DlgBaseEnsureServer;

	//显示窗口
	DlgBaseEnsureServer.DoModal();

	//设置变量
	m_pDlgBaseEnsure=NULL;

}


VOID CPlazaViewServer::ShowCheckIn()
{
	//效验参数
	ASSERT(m_pIMySelfUserItem!=NULL);
	ASSERT(m_TCPSocketModule.GetInterface()!=NULL);

	//变量定义
	CDlgCheckInServer DlgCheckInServer;
	DlgCheckInServer.Initialization(m_TCPSocketModule.GetInterface(),m_pIMySelfUserItem);

	//设置变量
	m_pDlgCheckIn=&DlgCheckInServer;

	//显示窗口
	DlgCheckInServer.DoModal();

	//设置变量
	m_pDlgCheckIn=NULL;

}


//显示银行
VOID CPlazaViewServer::ShowInsureView()
{
	//效验参数
	ASSERT(m_pIMySelfUserItem!=NULL);
	ASSERT(m_TCPSocketModule.GetInterface()!=NULL);

	//非金币房间
	if(m_wServerType!=GAME_GENRE_GOLD)
	{
		CDlgInformation Information;
		Information.ShowMessageBox(TEXT("系统提示"),TEXT("很抱歉，该房间禁止操作银行"),MB_ICONERROR);

		return;
	}

	//变量定义
	CDlgInsureServerNew DlgInsureServer;
	DlgInsureServer.Initialization(m_TCPSocketModule.GetInterface(),m_pIMySelfUserItem);
	
	//设置变量
	m_pDlgInsureServer=&DlgInsureServer;

	//显示窗口
	DlgInsureServer.DoModal();

	//设置变量
	m_pDlgInsureServer=NULL;

	return;
}

//显示任务
VOID CPlazaViewServer::ShowTask()
{
	//效验参数
	ASSERT(m_pIMySelfUserItem!=NULL);
	ASSERT(m_TCPSocketModule.GetInterface()!=NULL);

	//变量定义
	CDlgTaskServer TaskServerCenter;
	TaskServerCenter.Initialization(m_TCPSocketModule.GetInterface(),m_pIMySelfUserItem);
	
	//设置变量
	m_pDlgTaskServerCenter=&TaskServerCenter;

	//显示窗口
	TaskServerCenter.DoModal();

	//设置变量
	m_pDlgTaskServerCenter=NULL;
}

//显示兑换
VOID CPlazaViewServer::ShowExchange()
{
	//效验参数
	ASSERT(m_pIMySelfUserItem!=NULL);
	ASSERT(m_TCPSocketModule.GetInterface()!=NULL);

	//变量定义
	CDlgExchangeServer DlgExchangeServer;
	DlgExchangeServer.Initialization(m_TCPSocketModule.GetInterface());
	
	//设置变量
	m_pDlgExchangeServer=&DlgExchangeServer;

	//显示窗口
	DlgExchangeServer.DoModal();

	//设置变量
	m_pDlgExchangeServer=NULL;
}

//显示商城
VOID CPlazaViewServer::ShowShop()
{
	//效验参数
	ASSERT(m_pIMySelfUserItem!=NULL);
	ASSERT(m_TCPSocketModule.GetInterface()!=NULL);

	//非金币房间
	if(m_wServerType!=GAME_GENRE_GOLD)
	{
		CDlgInformation Information;
		Information.ShowMessageBox(TEXT("系统提示"),TEXT("很抱歉，该房间禁止操作商城"),MB_ICONERROR);

		return;
	}

	//变量定义
	CDlgShopServer DlgShopServer;
	DlgShopServer.Initialization(m_TCPSocketModule.GetInterface(),m_pIMySelfUserItem);

	//设置变量
	m_pDlgShopServer=&DlgShopServer;

	//显示窗口
	DlgShopServer.DoModal();

	//设置变量
	m_pDlgShopServer=NULL;
	return;
}

VOID CPlazaViewServer::ShowBag()
{
	//效验参数
	ASSERT(m_pIMySelfUserItem!=NULL);
	ASSERT(m_TCPSocketModule.GetInterface()!=NULL);

	//变量定义
	CDlgBagServer DlgBagServer;
	DlgBagServer.Initialization(m_TCPSocketModule.GetInterface(),m_pIMySelfUserItem);

	//设置变量
	m_pDlgBagServer=&DlgBagServer;

	//显示窗口
	DlgBagServer.DoModal();

	//设置变量
	m_pDlgBagServer=NULL;
}

VOID CPlazaViewServer::ShowPropertyUse(IClientUserItem* pClientUserItem)
{
	//效验参数
	ASSERT(m_pIMySelfUserItem!=NULL);
	ASSERT(m_TCPSocketModule.GetInterface()!=NULL);

	//变量定义
	CDlgPropertyUseServer DlgPropertyUseServer;
	DlgPropertyUseServer.Initialization(m_TCPSocketModule.GetInterface(),m_pIMySelfUserItem, pClientUserItem);

	//设置变量
	m_pDlgPropertyUse=&DlgPropertyUseServer;
	//显示窗口
	INT_PTR res = DlgPropertyUseServer.DoModal();
	m_pDlgPropertyUse = NULL;

	//购买
	if(res == IDOK)
	{
		ShowShop();
	}
}

bool CPlazaViewServer::QuerySendPresentMisson()
{
	if(m_pDlgBagServer != NULL)
	{
		return m_pDlgBagServer->QuerySendPresentMisson();
	}
	return false;
}

//老板事件
VOID CPlazaViewServer::NotifyBossCome(bool bBossCome)
{
	return;
}

//查找桌子
VOID CPlazaViewServer::SearchGameTable(tagSearchTable & SearchTable)
{
	//变量定义
	WORD wNullCount=0;
	WORD wChairUser=m_pITableViewFrame->GetChairCount();
	WORD wMeTableID=m_pIMySelfUserItem->GetTableID();

	//百人游戏
	if (CServerRule::IsAllowDynamicJoin(m_dwServerRule)==true)
	{
		if(m_pITableViewFrame->GetChairCount() >= INDEX_ENTER_CHAIR)
		{
			SearchTable.wResultTableID=0;
			SearchTable.wResultChairID=INDEX_ENTER_CHAIR;

			return;
		}
	}

	//搜索桌子
	for (WORD i=0;i<m_pITableViewFrame->GetTableCount();i++)
	{
		//设置索引
		SearchTable.wResultTableID=(SearchTable.wStartTableID+i)%m_pITableViewFrame->GetTableCount();

		//搜索处理
		if (wMeTableID!=SearchTable.wResultTableID)
		{
			//获取桌子
			ITableView * pITableView=m_pITableViewFrame->GetTableViewItem(SearchTable.wResultTableID);

			//搜索过虑
			if (pITableView->GetPlayFlag()==true) continue;
			if ((SearchTable.bFilterPass==true)&&(pITableView->GetLockerFlag()==true)) continue;

			//寻找空位置
			wNullCount=pITableView->GetNullChairCount(SearchTable.wResultChairID);

			//判断数目
			if (wNullCount>0)
			{
				//效验规则
				CString strDescribe;
				if (EfficacyTableRule(SearchTable.wResultTableID,SearchTable.wResultChairID,false,strDescribe)==false)
				{
					continue;
				}
		
				//数目判断
				if ((SearchTable.bOneNull==true)&&(wNullCount==1)) return;
				if ((SearchTable.bTwoNull==true)&&(wNullCount==2)) return;
				if ((SearchTable.bNotFull==true)&&(wNullCount<wChairUser)) return;
				if ((SearchTable.bAllNull==true)&&(wNullCount==wChairUser)) return;
			}
		}
	}

	//设置数据
	SearchTable.wResultTableID=INVALID_TABLE;
	SearchTable.wResultChairID=INVALID_CHAIR;

	return;
}

//更新头像
VOID CPlazaViewServer::UpdateUserCustomFace(DWORD dwUserID, DWORD dwCustomID, tagCustomFaceInfo & CustomFaceInfo)
{
	//查找用户
	ASSERT(m_PlazaUserManagerModule.GetInterface()!=NULL);
	IClientUserItem * pIClientUserItem=m_PlazaUserManagerModule.GetInterface()->SearchUserByUserID(dwUserID);

	//更新用户
	if (pIClientUserItem!=NULL)
	{
		ASSERT(m_PlazaUserManagerModule.GetInterface()!=NULL);
		m_PlazaUserManagerModule->UpdateUserCustomFace(pIClientUserItem,dwCustomID,CustomFaceInfo);
	}

	return;
}

//查询游戏客户端
bool CPlazaViewServer::IsGameClientReady()
{
	return m_ProcessManager.IsProcessReady();
}

//自己状态
bool CPlazaViewServer::IsPlayingMySelf()
{
	return ((m_pIMySelfUserItem!=NULL)&&(m_pIMySelfUserItem->GetUserStatus()==US_PLAYING));
}

//查找用户
IClientUserItem * CPlazaViewServer::SearchUserByUserID (DWORD dwUserID)
{
	//获取用户
	IClientUserItem * pTargetUserItem=NULL;
	pTargetUserItem=m_PlazaUserManagerModule->SearchUserByUserID(dwUserID);
	
	return pTargetUserItem;
}

//坐到邀请桌
bool CPlazaViewServer::SitDownInviteTable(WORD wTableID, bool bInRoom)
{
	//设置变量
	m_wInvitedTableID=wTableID;
	m_bUserInRoom=bInRoom;

	if (bInRoom==true)
	{
		//找空座位
		WORD wChairID=INVALID_CHAIR;
		WORD wNullCount=m_pITableViewFrame->GetNullChairCount(m_wInvitedTableID,wChairID);

		if (wNullCount==0) 
		{
			CDlgInformation Information2(this);
			Information2.ShowMessageBox(TEXT("此游戏桌已经没有空位置了！"),MB_ICONINFORMATION);
			return true;
		}

		//受邀坐下桌子
		bool bSitDown=PerformSitDownAction(m_wInvitedTableID,wChairID,false);
		m_wInvitedTableID=INVALID_TABLE;
	}

	return true;
}

//用户处理
bool CPlazaViewServer::OnSocketMainUser(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_GR_USER_ENTER:			//用户进入
		{
			return OnSocketSubUserEnter(pData,wDataSize);
		}
	case SUB_GR_USER_SCORE:			//用户积分
		{
			return OnSocketSubUserScore(pData,wDataSize);
		}
	case SUB_GR_USER_STATUS:		//用户状态
		{
			return OnSocketSubUserStatus(pData,wDataSize);
		}
	case SUB_GR_USER_GAME_DATA:		//用户游戏数据
		{
			return OnSocketSubUserItemGameData(pData,wDataSize);
		}
	case SUB_GR_REQUEST_FAILURE:	//请求失败
		{
			return OnSocketSubRequestFailure(pData,wDataSize);
		}
	case SUB_GR_USER_CHAT:			//用户聊天
		{
			return OnSocketSubUserChat(pData,wDataSize);
		}
	case SUB_GR_USER_EXPRESSION:	//用户表情
		{
			return OnSocketSubExpression(pData,wDataSize);
		}
	case SUB_GR_WISPER_CHAT:		//用户私聊
		{
			return OnSocketSubWisperUserChat(pData,wDataSize);
		}
	case SUB_GR_WISPER_EXPRESSION:	//私聊表情
		{
			return OnSocketSubWisperExpression(pData,wDataSize);
		}
	case SUB_GR_USER_INVITE:	   //邀请玩家
		{
			return OnSocketSubUserInvite(pData,wDataSize);
		}
	case SUB_GR_USER_WAIT_DISTRIBUTE: //等待分配
		{
			return OnSocketSubWaitDistribute(pData,wDataSize);
		}
	}

	return true;
}

//登录处理
bool CPlazaViewServer::OnSocketMainLogon(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_GR_LOGON_SUCCESS:	//登录成功
		{
			return OnSocketSubLogonSuccess(pData,wDataSize);
		}
	case SUB_GR_LOGON_FAILURE:	//登录失败
		{
			return OnSocketSubLogonFailure(pData,wDataSize);
		}
	case SUB_GR_LOGON_FINISH:	//登录完成
		{
			return OnSocketSubLogonFinish(pData,wDataSize);
		}
	case SUB_GR_UPDATE_NOTIFY:	//更新提示
		{
			return OnSocketSubUpdateNotify(pData,wDataSize);
		}
	}

	return true;
}

//配置处理
bool CPlazaViewServer::OnSocketMainConfig(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_GR_CONFIG_COLUMN:		//列表配置
		{
			//变量定义
			CMD_GR_ConfigColumn * pConfigColumn=(CMD_GR_ConfigColumn *)pData;
			WORD wHeadSize=sizeof(CMD_GR_ConfigColumn)-sizeof(pConfigColumn->ColumnItem);

			//效验参数
			ASSERT((wHeadSize+pConfigColumn->cbColumnCount*sizeof(pConfigColumn->ColumnItem[0]))==wDataSize);
			if ((wHeadSize+pConfigColumn->cbColumnCount*sizeof(pConfigColumn->ColumnItem[0]))!=wDataSize) return false;

			//保存信息
			m_cbColumnCount=__min(pConfigColumn->cbColumnCount,CountArray(m_ColumnItem));
			CopyMemory(m_ColumnItem,pConfigColumn->ColumnItem,sizeof(tagColumnItem)*m_cbColumnCount);

			//设置列表
			m_pPlazaViewServerCtrl->SetColumnDescribe(pConfigColumn->ColumnItem,pConfigColumn->cbColumnCount);

			return true;
		}
	case SUB_GR_CONFIG_SERVER:		//房间配置
		{
			//效验数据
			ASSERT(wDataSize==sizeof(CMD_GR_ConfigServer));
			if (wDataSize<sizeof(CMD_GR_ConfigServer)) return false;

			//消息处理
			CMD_GR_ConfigServer * pConfigServer=(CMD_GR_ConfigServer *)pData;

			//房间属性
			m_wServerType=pConfigServer->wServerType;
			m_dwServerRule=pConfigServer->dwServerRule;

          	//构造参数
			WORD wTableCount=pConfigServer->wTableCount;
			WORD wChairCount=pConfigServer->wChairCount;
			m_wChairCount=pConfigServer->wChairCount;
			IUnknownEx * pIUnknownEx=QUERY_ME_INTERFACE(IUnknownEx);

			//资源目录
			TCHAR szResDirectory[LEN_KIND]=TEXT("");
			GetGameResDirectory(szResDirectory,CountArray(szResDirectory));

			//配置桌子
			bool bSuccess=m_pITableViewFrame->ConfigTableFrame(wTableCount,wChairCount,m_dwServerRule,m_wServerType,m_GameServer.wServerID,szResDirectory);

			//错误处理
			if (bSuccess==false)
			{
				//隐藏提示
				m_DlgStatus.HideStatusWindow();

				//关闭连接
				m_TCPSocketModule->CloseSocket();

				//提示信息
				CDlgInformation Information(AfxGetMainWnd());
				INT nResult=Information.ShowMessageBox(TEXT("游戏资源文件加载失败，是否现在进行修复？"),MB_ICONERROR|MB_YESNO);

				//下载游戏
				if (nResult==IDYES)
				{
					CPlatformFrame::GetInstance()->DownLoadGame(m_GameKind.szKindName,m_GameKind.wKindID,m_GameServer.wServerID);
					CPlatformFrame::GetInstance()->ReturnHallMain();
					//CGlobalUnits * pGlobalUnits=CGlobalUnits::GetInstance();
					//pGlobalUnits->DownLoadClient(m_GameKind.szKindName,m_GameKind.wGameID,m_GameServer.wServerID);
				}

				//关闭房间
				PostMessage(WM_COMMAND,IDM_DELETE_SERVER_ITEM,0);
			}

			return true;
		}
	case SUB_GR_CONFIG_FINISH:		//配置完成
		{
			//资源目录
			TCHAR szResDirectory[LEN_KIND]=TEXT("");
			GetGameResDirectory(szResDirectory,CountArray(szResDirectory));

			//游戏等级
			ASSERT(m_GameLevelParserModule.GetInterface()!=NULL);
			bool bSuccess=m_GameLevelParserModule->LoadGameLevelItem(m_GameKind.szKindName,szResDirectory,m_wServerType);

			//错误处理
			if (bSuccess==false)
			{
				//关闭连接
				m_TCPSocketModule->CloseSocket();

				//提示信息
				CDlgInformation Information(this);
				INT nResult=Information.ShowMessageBox(TEXT("游戏等级配置读取失败或者格式错误，是否现在进行修复？"),MB_ICONERROR|MB_YESNO);

				//下载游戏
				if (nResult==IDYES)
				{
					CPlatformFrame::GetInstance()->DownLoadGame(m_GameKind.szKindName,m_GameKind.wKindID,m_GameServer.wServerID);
					//CGlobalUnits * pGlobalUnits=CGlobalUnits::GetInstance();
					//pGlobalUnits->DownLoadClient(m_GameKind.szKindName,m_GameKind.wGameID,m_GameServer.wServerID);
				}

				//关闭房间
				PostMessage(WM_COMMAND,IDM_DELETE_SERVER_ITEM,0);
			}

			//配置成功
			if(bSuccess==true)
			{
				m_pPlazaViewServerCtrl->SetServerInfo(m_wChairCount,m_wServerType,m_dwServerRule,m_dwUserRight,m_dwMasterRight);

				//创建道具
				CGamePropertyListData *pGamePropertyListData = CGamePropertyListData::GetInstance();
				CGamePropertyManager * pGamePropertyManager=CGamePropertyManager::GetInstance();
				POSITION Position=NULL;
				CGamePropertyItem * pGamePropertyItem=NULL;
				while (true)
				{
					pGamePropertyItem=pGamePropertyListData->EmunGamePropertyItem(Position);
					if (pGamePropertyItem==NULL) break;
					pGamePropertyManager->CreatePropertyItem(pGamePropertyItem->m_PropertyItem);
					if (Position==NULL) break;
				}			

			}
			return true;
		}
	case SUB_GR_CONFIG_USER_RIGHT:	//配置玩家权限
		{
			//效验数据
			ASSERT(wDataSize>=sizeof(CMD_GR_ConfigUserRight));
			if (wDataSize<sizeof(CMD_GR_ConfigUserRight)) return false;

			//消息处理
			CMD_GR_ConfigUserRight * pConfigUserRight=(CMD_GR_ConfigUserRight *)pData;

			//玩家权限
			m_dwUserRight=pConfigUserRight->dwUserRight;

			//设置私聊窗口
			for( INT_PTR i = 0; i < m_DlgWhisperItemArray.GetCount(); i++ )
			{
				CDlgWhisper *pdlg = m_DlgWhisperItemArray[i];
				if( pdlg ) pdlg->SetUserRight(m_dwUserRight);
			}

			//通知客户端
			if(IsGameClientReady())
			{
				//构造函数
				IPC_GF_UserRight ur = {0};
				ur.dwUserRight = m_dwUserRight;

				m_ProcessManager.SendProcessData(IPC_CMD_GF_CONFIG,IPC_SUB_GF_USER_RIGHT,&ur,sizeof(ur) );
			}

			return true;
		}
	}

	//错误断言
	ASSERT(FALSE);

	return true;
}

//状态处理
bool CPlazaViewServer::OnSocketMainStatus(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_GR_TABLE_INFO:		//桌子信息
		{
			//变量定义
			CMD_GR_TableInfo * pTableInfo=(CMD_GR_TableInfo *)pData;
			WORD wHeadSize=sizeof(CMD_GR_TableInfo)-sizeof(pTableInfo->TableStatusArray);

			//效验数据
			ASSERT((wHeadSize+pTableInfo->wTableCount*sizeof(pTableInfo->TableStatusArray[0]))==wDataSize);
			if ((wHeadSize+pTableInfo->wTableCount*sizeof(pTableInfo->TableStatusArray[0]))!=wDataSize) return false;

			//消息处理
			for (WORD i=0;i<pTableInfo->wTableCount;i++)
			{
				BYTE cbTableLock=pTableInfo->TableStatusArray[i].cbTableLock;
				BYTE cbPlayStatus=pTableInfo->TableStatusArray[i].cbPlayStatus;
				m_pITableViewFrame->SetTableStatus(i,(cbPlayStatus==TRUE),(cbTableLock==TRUE),pTableInfo->TableStatusArray[i].lCellScore);
			}

			return true;
		}
	case SUB_GR_TABLE_STATUS:	//桌子状态
		{
			//效验参数
			ASSERT(wDataSize==sizeof(CMD_GR_TableStatus));
			if (wDataSize!=sizeof(CMD_GR_TableStatus)) return false;

			//变量定义
			CMD_GR_TableStatus * pTableStatus=(CMD_GR_TableStatus *)pData;

			//设置桌子
			BYTE cbTableLock=pTableStatus->TableStatus.cbTableLock;
			BYTE cbPlayStatus=pTableStatus->TableStatus.cbPlayStatus;
			m_pITableViewFrame->SetTableStatus(pTableStatus->wTableID,(cbPlayStatus==TRUE),(cbTableLock==TRUE),pTableStatus->TableStatus.lCellScore);

			//设置桌子
			if(cbPlayStatus==TRUE && m_pIMySelfUserItem->GetTableID()==pTableStatus->wTableID && CServerRule::IsAllowAvertCheatMode(m_dwServerRule))
			{
				m_pITableViewFrame->SetTableStatus(false);
			}

			return true;
		}
	}

	return true;
}

//银行处理
bool CPlazaViewServer::OnSocketMainInsure(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	//效验状态
	ASSERT(pData!=NULL);
	if (pData==NULL) return true;

	//游戏处理
	BYTE *pActivityGame = (BYTE*)pData;
	if(*pActivityGame==TRUE)
	{
		return OnSocketMainGameFrame(MDM_GR_INSURE,wSubCmdID,pData,wDataSize);
	}

	//消息处理
	ASSERT(m_pDlgInsureServer!=NULL);
	if (m_pDlgInsureServer==NULL) return true;
	if (m_pDlgInsureServer->OnServerInsureMessage(MDM_GR_INSURE,wSubCmdID,pData,wDataSize)==false)
	{
		ASSERT(FALSE);
		return false;
	}

	return true;
}


bool CPlazaViewServer::OnSocketMainBaseEnsure( WORD wSubCmdID, VOID * pData, WORD wDataSize )
{
	//效验状态
	ASSERT(pData!=NULL);
	if (pData==NULL) return true;

	//消息处理
	ASSERT(m_pDlgBaseEnsure!=NULL);
	if (m_pDlgBaseEnsure==NULL) return true;
	if (m_pDlgBaseEnsure->OnServerBaseEnsureMessage(MDM_GR_BASEENSURE,wSubCmdID,pData,wDataSize)==false)
	{
		ASSERT(FALSE);
		return false;
	}

	return true;
}


bool CPlazaViewServer::OnSocketMainMember( WORD wSubCmdID, VOID * pData, WORD wDataSize )
{
	//效验状态
	ASSERT(pData!=NULL);
	if (pData==NULL) return true;

	//消息处理
	ASSERT(m_pDlgCheckIn!=NULL);
	if (m_pDlgCheckIn==NULL) return true;
	if (m_pDlgCheckIn->OnServerCheckInMessage(MDM_GR_MEMBER,wSubCmdID,pData,wDataSize)==false)
	{
		ASSERT(FALSE);
		return false;
	}
	return true;
}


bool CPlazaViewServer::OnSocketMainCheckIn( WORD wSubCmdID, VOID * pData, WORD wDataSize )
{
	//效验状态
	ASSERT(pData!=NULL);
	if (pData==NULL) return true;

	//消息处理
	ASSERT(m_pDlgCheckIn!=NULL);
	if (m_pDlgCheckIn==NULL) return true;
	if (m_pDlgCheckIn->OnServerCheckInMessage(MDM_GR_CHECKIN,wSubCmdID,pData,wDataSize)==false)
	{
		ASSERT(FALSE);
		return false;
	}

	return true;
}

//任务处理
bool CPlazaViewServer::OnSocketMainTask(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	//效验状态
	ASSERT(pData!=NULL);
	if (pData==NULL) return true;

	//不用提示
	////消息过滤
	//if(wSubCmdID==SUB_GR_TASK_FINISH)
	//{
	//	//参数校验
	//	ASSERT(wDataSize<=sizeof(CMD_GR_S_TaskFinish));
	//	if(wDataSize>sizeof(CMD_GR_S_TaskFinish)) return true;

	//	//提取数据
	//	CMD_GR_S_TaskFinish * pTaskFinish = (CMD_GR_S_TaskFinish *)pData;

	//	//显示提示框
	//	CPlatformFrame * pPlatformFrame = CPlatformFrame::GetInstance();
	//	pPlatformFrame->ShowTaskNotifyer(pTaskFinish->szTaskName);

	//	return true;
	//}
    
	//消息处理
	/*ASSERT(m_pDlgTaskServerCenter!=NULL);*/
	if (m_pDlgTaskServerCenter==NULL) return true;
	if (m_pDlgTaskServerCenter->OnTaskMessage(MDM_GR_TASK,wSubCmdID,pData,wDataSize)==false)
	{
		ASSERT(FALSE);
		return false;
	}

	return true;
}

//兑换处理
bool CPlazaViewServer::OnSocketMainExchange(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	//效验状态
	ASSERT(pData!=NULL);
	if (pData==NULL) return true;
   
	//消息处理
	ASSERT(m_pDlgExchangeServer!=NULL);
	if (m_pDlgExchangeServer==NULL) return true;
	if (m_pDlgExchangeServer->OnExchangeMessage(MDM_GR_EXCHANGE,wSubCmdID,pData,wDataSize)==false)
	{
		ASSERT(FALSE);
		return false;
	}

	return true;
}

//道具处理
bool CPlazaViewServer::OnSocketMainProperty(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	switch (wSubCmdID)
	{										
	case SUB_GR_QUERY_PROPERTY_RESULT:		//查询道具
	case SUB_GR_GAME_PROPERTY_BUY_RESULT:	//购买道具
	case SUB_GR_QUERY_PROPERTY_RESULT_FINISH:	//查询道具
	case SUB_GR_GAME_PROPERTY_FAILURE:
		{
			if(m_pDlgShopServer) return m_pDlgShopServer->OnServerShopMessage(MDM_GR_PROPERTY, wSubCmdID, pData, wDataSize);
		}
	case SUB_GR_PROPERTY_USE_RESULT:		//使用道具
		{
			return OnSocketSubPropertyUse(pData,wDataSize);
		}
	case SUB_GR_PROPERTY_BACKPACK_RESULT:	//玩家背包
		{
			return OnSocketSubUserBackpack(pData,wDataSize);
		}
	case SUB_GR_QUERY_SEND_PRESENT_RESULT:	//查询赠送
		{
			return	OnSocketSubQuerySendPresent(pData,wDataSize);
		}
	case SUB_GR_PROPERTY_PRESENT_RESULT:	//赠送道具
		{
			return OnSocketSubPropertyPresent(pData,wDataSize);
		}
	case SUB_GR_GET_SEND_PRESENT_RESULT:	//获取赠送
		{
			return OnSocketSubGetSendPresent(pData,wDataSize);
		}
	case SUB_GR_PROPERTY_SUCCESS:			//道具成功
		{
			return OnSocketSubPropertySuccess(pData,wDataSize);
		}
	case SUB_GR_PROPERTY_FAILURE:			//道具失败
		{
			return OnSocketSubPropertyFailure(pData,wDataSize);
		}
	case SUB_GR_PROPERTY_EFFECT:			//道具效应
		{
			return OnSocketSubPropertyEffect(pData,wDataSize);
		}
	case SUB_GR_PROPERTY_MESSAGE:			//礼物消息
		{
			return OnSocketPropertyMessage(pData,wDataSize);
		}
	case SUB_GR_PROPERTY_TRUMPET:			//喇叭消息
		{
			return OnSocketPropertyTrumpet(pData,wDataSize);
		}
	case SUB_GR_USER_PROP_BUFF:				//道具状态
		{
			return OnSocketSubUserPropBuff(pData,wDataSize);
		}
	case SUB_GR_USER_TRUMPET:			//喇叭数量
		{
			return OnSocketSubUserTrumpet(pData,wDataSize);
		}
	}
	return true;
}

//管理处理
bool CPlazaViewServer::OnSocketMainManager(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_GR_OPTION_CURRENT:	//当前配置
		{
			//效验参数
			ASSERT(wDataSize==sizeof(CMD_GR_OptionCurrent));
			if (wDataSize!=sizeof(CMD_GR_OptionCurrent)) return false;

			//消息处理
			if (m_pDlgServerManager!=NULL)
			{
				CMD_GR_OptionCurrent * pOptionCurrent=(CMD_GR_OptionCurrent *)pData;
				m_pDlgServerManager->SetServerType(m_wServerType);
				m_pDlgServerManager->SetServerOptionInfo(pOptionCurrent->ServerOptionInfo,pOptionCurrent->dwRuleMask);
			}

			return true;
		}
	}

	return true;
}

//系统处理
bool CPlazaViewServer::OnSocketMainSystem(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_CM_SYSTEM_MESSAGE:	//系统消息
		{
			return OnSocketSubSystemMessage(pData,wDataSize);
		}
	case SUB_CM_ACTION_MESSAGE:	//动作消息
		{
			return OnSocketSubActionMessage(pData,wDataSize);
		}
	}

	//错误断言
	ASSERT(FALSE);

	return true;
}

//游戏处理
bool CPlazaViewServer::OnSocketMainGameFrame(WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize<=SOCKET_TCP_PACKET);
	if (wDataSize>SOCKET_TCP_PACKET) return false;
	if(!IsGameClientReady()) return true;

	//构造数据
	IPC_GF_SocketRecv SocketRecv;
	SocketRecv.CommandInfo.wSubCmdID=wSubCmdID;
	SocketRecv.CommandInfo.wMainCmdID=wMainCmdID;

	//叠加数据
	if (wDataSize>0)
	{
		ASSERT(pData!=NULL);
		CopyMemory(SocketRecv.cbBuffer,pData,wDataSize);
	}

	//发送数据
	WORD wSendSize=sizeof(SocketRecv)-sizeof(SocketRecv.cbBuffer)+wDataSize;
	m_ProcessManager.SendProcessData(IPC_CMD_GF_SOCKET,IPC_SUB_GF_SOCKET_RECV,&SocketRecv,wSendSize);

	return true;
}

//比赛处理
bool CPlazaViewServer::OnSocketMainMatch(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_GR_MATCH_FEE:		//费用查询
		{
			//参数效验
			ASSERT(wDataSize<=sizeof(CMD_GR_Match_Fee));
			if(wDataSize>sizeof(CMD_GR_Match_Fee)) return false;

			//提取数据
			CMD_GR_Match_Fee * pMatchFee = (CMD_GR_Match_Fee *)pData;
			if(pMatchFee->cbSignupMode==SIGNUP_MODE_SIGNUP_FEE)
			{
				//房间扣费
				if (pMatchFee->cbDeductArea==DEDUCT_AREA_SERVER)
				{
					if (pMatchFee->lSignupFee>0)
					{
						INT nResult=ShowInformation(pMatchFee->szNotifyContent,MB_ICONINFORMATION|MB_YESNO,30L);
						if (nResult==IDYES) 
						{
							BYTE cbSignupMode=SIGNUP_MODE_SIGNUP_FEE;
							m_TCPSocketModule->SendData(MDM_GR_MATCH,SUB_GR_MATCH_FEE,&cbSignupMode,sizeof(cbSignupMode));
						}
					}
					else if (pMatchFee->lSignupFee ==0)
					{
						BYTE cbSignupMode=SIGNUP_MODE_SIGNUP_FEE;
						m_TCPSocketModule->SendData(MDM_GR_MATCH,SUB_GR_MATCH_FEE,&cbSignupMode,sizeof(cbSignupMode));
					}
				}

				//网页扣费
				if (pMatchFee->cbDeductArea==DEDUCT_AREA_WEBSITE)
				{
					//变量定义
					//CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
					//tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

					////构造链接
					//TCHAR szSignupLink[MAX_PATH];
					//_sntprintf(szSignupLink,CountArray(szSignupLink),TEXT("http://web.fox2046.com:81/MatchCenter/MatchXZ.aspx?UserID=%d&Password=%s&ServerID=%d&MatchID=%d&MatchNo=%I64d"),pGlobalUserData->dwUserID,pGlobalUserData->szPassword,m_GameServer.wServerID,pMatchFee->dwMatchID,pMatchFee->lMatchNO);

					////变量定义
					//CDlgSignupBrowse SingupBrowse;
					//SingupBrowse.SetInitSize(CSize(720,568));
					//SingupBrowse.SetBrowseInfo(TEXT("比赛报名"),szSignupLink);
					//SingupBrowse.DoModal();

					////报名成功
					//if (SingupBrowse.GetSignupStatus()==1)
					//{
					//	BYTE cbSignupMode=SIGNUP_MODE_SIGNUP_FEE;
					//	m_TCPSocketModule->SendData(MDM_GR_MATCH,SUB_GR_MATCH_FEE,&cbSignupMode,sizeof(cbSignupMode));
					//}
				}
			}

			return true;
		}
	case SUB_GR_MATCH_SAFECARD:		//保险卡
		{
			//参数效验
			ASSERT(wDataSize<=sizeof(CMD_GR_Match_SafeCard));
			if(wDataSize>sizeof(CMD_GR_Match_SafeCard)) return false;

			//提取数据
			CMD_GR_Match_SafeCard * pMatchSafeCard= (CMD_GR_Match_SafeCard *)pData;

			//提醒购买
			INT nResult=ShowInformation(CW2CT(pMatchSafeCard->szNotifyContent),MB_ICONINFORMATION|MB_YESNO,30L);
			
			//构造结构
			CMD_GR_Match_BuySafeCard MatchBuySafeCard;
			ZeroMemory(&MatchBuySafeCard,sizeof(MatchBuySafeCard));

			//设置标识
			MatchBuySafeCard.bBuySafeCard=nResult==IDYES;

			//发送数据
			m_TCPSocketModule->SendData(MDM_GR_MATCH,SUB_GR_MATCH_BUY_SAFECARD,&MatchBuySafeCard,sizeof(MatchBuySafeCard));

			return true;
		}
	case SUB_GR_MATCH_REVIVE:		//复活提醒
		{
			//参数效验
			ASSERT(wDataSize<=sizeof(CMD_GR_Match_Revive));
			if(wDataSize>sizeof(CMD_GR_Match_Revive)) return false;

			//提取数据
			CMD_GR_Match_Revive * pMatchRevive= (CMD_GR_Match_Revive *)pData;

			//提醒购买
			INT nResult=ShowInformation(CW2CT(pMatchRevive->szNotifyContent),MB_ICONINFORMATION|MB_YESNO,30L);
			if (nResult==IDYES) 
			{
				//构造结构
				CMD_GR_Match_UserRevive MatchUserRevive;
				ZeroMemory(&MatchUserRevive,sizeof(MatchUserRevive));

				//设置变量
				MatchUserRevive.bUseSafeCard=false;

				//发送数据
				m_TCPSocketModule->SendData(MDM_GR_MATCH,SUB_GR_MATCH_USER_REVIVE,&MatchUserRevive,sizeof(MatchUserRevive));
			}

			return true;
		}
	case SUB_GR_MATCH_GOLDUPDATE:	//金币更新
		{
			//参数校验
			ASSERT(sizeof(CMD_GR_MatchGoldUpdate)==wDataSize);
			if(sizeof(CMD_GR_MatchGoldUpdate)!=wDataSize) return false;

			//提取数据
			CMD_GR_MatchGoldUpdate * pMatchGoldUpdate=(CMD_GR_MatchGoldUpdate*)pData;

			//变量定义
			CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
			tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

			//设置变量
			pGlobalUserData->lUserScore=pMatchGoldUpdate->lCurrGold;

			//发送事件
			CPlatformEvent * pPlatformEvent=CPlatformEvent::GetInstance();
			if (pPlatformEvent!=NULL) pPlatformEvent->SendPlatformEvent(EVENT_USER_INFO_UPDATE,0L);

			return true;
		}
	case SUB_GR_MATCH_ELIMINATE:	//比赛淘汰
		{
			return true;
		}
	}

	return m_pITableViewFrame->OnEventViewData(wSubCmdID,pData,wDataSize);
}


//用户进入
bool CPlazaViewServer::OnSocketSubUserEnter(VOID * pData, WORD wDataSize)
{
	//效验参数
	ASSERT(wDataSize>=sizeof(tagUserInfoHead));
	if (wDataSize<sizeof(tagUserInfoHead)) return false;

	//变量定义
	tagUserInfo UserInfo;
	tagCustomFaceInfo CustomFaceInfo;
	ZeroMemory(&UserInfo,sizeof(UserInfo));
	ZeroMemory(&CustomFaceInfo,sizeof(CustomFaceInfo));

	//消息处理
	tagUserInfoHead * pUserInfoHead=(tagUserInfoHead *)pData;

	//变量定义
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	//变量定义
	bool bHideUserInfo=CServerRule::IsAllowAvertCheatMode(m_dwServerRule);
	bool bMySelfUserItem=pGlobalUserData->dwUserID==pUserInfoHead->dwUserID;
	bool bMasterUserOrder=((pUserInfoHead->cbMasterOrder>0)||((m_pIMySelfUserItem!=NULL)&&(m_pIMySelfUserItem->GetMasterOrder()>0)));

	if(bMySelfUserItem) 		m_bPlaying = true;

	//读取信息
	if ((bHideUserInfo==false)||(bMySelfUserItem==true)||(bMasterUserOrder==true))
	{
		//用户属性
		UserInfo.wFaceID=pUserInfoHead->wFaceID;
		UserInfo.dwGameID=pUserInfoHead->dwGameID;
		UserInfo.dwUserID=pUserInfoHead->dwUserID;
		UserInfo.dwGroupID=pUserInfoHead->dwGroupID;
		UserInfo.dwCustomID=pUserInfoHead->dwCustomID;
		UserInfo.dwAgentID=pUserInfoHead->dwAgentID;

		//用户状态
		UserInfo.wTableID=pUserInfoHead->wTableID;
		UserInfo.wChairID=pUserInfoHead->wChairID;
		UserInfo.cbUserStatus=pUserInfoHead->cbUserStatus;

		//用户属性
		UserInfo.cbGender=pUserInfoHead->cbGender;
		UserInfo.cbMemberOrder=pUserInfoHead->cbMemberOrder;
		UserInfo.cbMasterOrder=pUserInfoHead->cbMasterOrder;

		//用户积分
		UserInfo.lScore=pUserInfoHead->lScore;
		UserInfo.lGrade=pUserInfoHead->lGrade;
		UserInfo.lInsure=pUserInfoHead->lInsure;
		UserInfo.lIngot=pUserInfoHead->lIngot;
		UserInfo.dBeans=pUserInfoHead->dBeans;
		UserInfo.dwWinCount=pUserInfoHead->dwWinCount;
		UserInfo.dwLostCount=pUserInfoHead->dwLostCount;
		UserInfo.dwDrawCount=pUserInfoHead->dwDrawCount;
		UserInfo.dwFleeCount=pUserInfoHead->dwFleeCount;		
		UserInfo.dwExperience=pUserInfoHead->dwExperience;
		UserInfo.lLoveLiness=pUserInfoHead->lLoveLiness;
		UserInfo.lIntegralCount=pUserInfoHead->lIntegralCount;

		//变量定义
		VOID * pDataBuffer=NULL;
		tagDataDescribe DataDescribe;
		CRecvPacketHelper RecvPacket(pUserInfoHead+1,wDataSize-sizeof(tagUserInfoHead));

		//扩展信息
		while (true)
		{
			pDataBuffer=RecvPacket.GetData(DataDescribe);
			if (DataDescribe.wDataDescribe==DTP_NULL) break;
			switch (DataDescribe.wDataDescribe)
			{
			case DTP_GR_NICK_NAME:		//用户昵称
				{
					ASSERT(pDataBuffer!=NULL);
					ASSERT(DataDescribe.wDataSize<=sizeof(UserInfo.szNickName));
					if (DataDescribe.wDataSize<=sizeof(UserInfo.szNickName))
					{
						CopyMemory(&UserInfo.szNickName,pDataBuffer,DataDescribe.wDataSize);
						UserInfo.szNickName[CountArray(UserInfo.szNickName)-1]=0;
					}
					break;
				}
			case DTP_GR_GROUP_NAME:		//用户社团
				{
					ASSERT(pDataBuffer!=NULL);
					ASSERT(DataDescribe.wDataSize<=sizeof(UserInfo.szGroupName));
					if (DataDescribe.wDataSize<=sizeof(UserInfo.szGroupName))
					{
						CopyMemory(&UserInfo.szGroupName,pDataBuffer,DataDescribe.wDataSize);
						UserInfo.szGroupName[CountArray(UserInfo.szGroupName)-1]=0;
					}
					break;
				}
			case DTP_GR_UNDER_WRITE:	//个性签名
				{
					ASSERT(pDataBuffer!=NULL);
					ASSERT(DataDescribe.wDataSize<=sizeof(UserInfo.szUnderWrite));
					if (DataDescribe.wDataSize<=sizeof(UserInfo.szUnderWrite))
					{
						CopyMemory(UserInfo.szUnderWrite,pDataBuffer,DataDescribe.wDataSize);
						UserInfo.szUnderWrite[CountArray(UserInfo.szUnderWrite)-1]=0;
					}
					break;
				}
			case DTP_GR_QQ:	//QQ号码
				{
					ASSERT(pDataBuffer!=NULL);
					ASSERT(DataDescribe.wDataSize<=sizeof(UserInfo.szQQ));
					if (DataDescribe.wDataSize<=sizeof(UserInfo.szQQ))
					{
						CopyMemory(UserInfo.szQQ,pDataBuffer,DataDescribe.wDataSize);
						UserInfo.szQQ[CountArray(UserInfo.szQQ)-1]=0;
					}
					break;
				}
			case DTP_GR_MOBILE_PHONE:	//手机号码
				{
					ASSERT(pDataBuffer!=NULL);
					ASSERT(DataDescribe.wDataSize<=sizeof(UserInfo.szMobilePhone));
					if (DataDescribe.wDataSize<=sizeof(UserInfo.szMobilePhone))
					{
						CopyMemory(UserInfo.szMobilePhone,pDataBuffer,DataDescribe.wDataSize);
						UserInfo.szMobilePhone[CountArray(UserInfo.szMobilePhone)-1]=0;
					}
					break;
				}
			}
		}

		//自定头像
		if (pUserInfoHead->dwCustomID!=0L)
		{
			//加载头像
			CCustomFaceManager * pCustomFaceManager=CCustomFaceManager::GetInstance();
			bool bSuccess=pCustomFaceManager->LoadUserCustomFace(pUserInfoHead->dwUserID,pUserInfoHead->dwCustomID,CustomFaceInfo);

			//下载头像
			if (bSuccess==false)
			{
				pCustomFaceManager->LoadUserCustomFace(pUserInfoHead->dwUserID,pUserInfoHead->dwCustomID);
			}
		}
	}
	else
	{
		//用户信息
		UserInfo.dwUserID=pUserInfoHead->dwUserID;
		lstrcpyn(UserInfo.szNickName,TEXT("游戏玩家"),CountArray(UserInfo.szNickName));

		//用户状态
		UserInfo.wTableID=pUserInfoHead->wTableID;
		UserInfo.wChairID=pUserInfoHead->wChairID;
		UserInfo.cbUserStatus=pUserInfoHead->cbUserStatus;

		//用户属性
		UserInfo.cbGender=pUserInfoHead->cbGender;
		UserInfo.cbMemberOrder=pUserInfoHead->cbMemberOrder;
		UserInfo.cbMasterOrder=pUserInfoHead->cbMasterOrder;
	}

	//激活用户
	IClientUserItem * pIClientUserItem=m_PlazaUserManagerModule->SearchUserByUserID(UserInfo.dwUserID);
	if (pIClientUserItem==NULL) pIClientUserItem=m_PlazaUserManagerModule->ActiveUserItem(UserInfo,CustomFaceInfo,pUserInfoHead->bIsAndroid);

	//获取对象
	CServerListData * pServerListData=CServerListData::GetInstance();

	//人数更新
	pServerListData->SetServerOnLineCount(m_GameServer.wServerID,m_PlazaUserManagerModule->GetActiveUserCount());

	//变量定义
	ASSERT(CParameterGlobal::GetInstance()!=NULL);
	CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();

	//好友提示
	if (((bHideUserInfo==false)&&(bMySelfUserItem==false))||(bMasterUserOrder==true))
	{
		if(pParameterGlobal->m_bNotifyFriendCome && pIClientUserItem->GetUserCompanion()==CP_FRIEND)
		{
			//获取指针
			IStringMessage * pIStringMessage = m_pPlazaViewServerCtrl->GetStringMessage();
			ASSERT(pIStringMessage!=NULL);

			//提示消息
			CString strDescribe;
			strDescribe.Format(TEXT("您的好友 [%s] 进来了！"),pIClientUserItem->GetNickName());
			pIStringMessage->InsertSystemString(strDescribe);
		}
	}

	return true;
}

//用户积分
bool CPlazaViewServer::OnSocketSubUserScore(VOID * pData, WORD wDataSize)
{
	//效验参数
	ASSERT(wDataSize==sizeof(CMD_GR_UserScore));
	if (wDataSize<sizeof(CMD_GR_UserScore)) return false;

	//寻找用户
	CMD_GR_UserScore * pUserScore=(CMD_GR_UserScore *)pData;
	IClientUserItem * pIClientUserItem=m_PlazaUserManagerModule->SearchUserByUserID(pUserScore->dwUserID);

	//用户判断
	ASSERT(pIClientUserItem!=NULL);
	if (pIClientUserItem==NULL) return true;

	//变量定义
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	//变量定义
	bool bMySelfUserItem=pGlobalUserData->dwUserID==pUserScore->dwUserID;
	bool bAvertCheatMode=CServerRule::IsAllowAvertCheatMode(m_dwServerRule);
	bool bMasterUserOrder=((pIClientUserItem->GetMasterOrder()>0)||(m_pIMySelfUserItem->GetMasterOrder()>0));

	//更新用户
	if ((bAvertCheatMode==false)||(bMySelfUserItem==true)||(bMasterUserOrder==true))
	{
		m_PlazaUserManagerModule->UpdateUserItemScore(pIClientUserItem,&pUserScore->UserScore);
	}

	return true;
}

//用户状态
bool CPlazaViewServer::OnSocketSubUserStatus(VOID * pData, WORD wDataSize)
{
	//效验参数
	ASSERT(wDataSize==sizeof(CMD_GR_UserStatus));
	if (wDataSize<sizeof(CMD_GR_UserStatus)) return false;

	//寻找用户
	CMD_GR_UserStatus * pUserStatus=(CMD_GR_UserStatus *)pData;
	IClientUserItem * pIClientUserItem=m_PlazaUserManagerModule->SearchUserByUserID(pUserStatus->dwUserID);

	//用户判断
	ASSERT(pIClientUserItem!=NULL);
	if (pIClientUserItem==NULL) return true;

	//设置状态
	if (pUserStatus->UserStatus.cbUserStatus==US_NULL) 
	{
		//删除用户
		m_PlazaUserManagerModule->DeleteUserItem(pIClientUserItem);

		//获取对象
		CServerListData * pServerListData=CServerListData::GetInstance();

		//人数更新
		pServerListData->SetServerOnLineCount(m_GameServer.wServerID,m_PlazaUserManagerModule->GetActiveUserCount());
	}
	else
	{
		//更新用户
		m_PlazaUserManagerModule->UpdateUserItemStatus(pIClientUserItem,&pUserStatus->UserStatus);
	}

	return true;
}

//用户游戏数据
bool CPlazaViewServer::OnSocketSubUserItemGameData(VOID * pData, WORD wDataSize)
{
	//变量定义
	CMD_GR_UserGameData * pUserGameData=(CMD_GR_UserGameData *)pData;
	WORD wHeadSize=sizeof(CMD_GR_UserGameData)-sizeof(pUserGameData->szUserGameData);

	//效验参数
	ASSERT((wDataSize>wHeadSize)&&(wDataSize==(wHeadSize+CountStringBuffer(pUserGameData->szUserGameData))));
	if ((wDataSize<=wHeadSize)||(wDataSize!=(wHeadSize+CountStringBuffer(pUserGameData->szUserGameData)))) return false;

	//寻找用户
	IClientUserItem * pIClientUserItem=m_PlazaUserManagerModule->SearchUserByUserID(pUserGameData->dwUserID);

	//用户判断
	ASSERT(pIClientUserItem!=NULL);
	if (pIClientUserItem==NULL) return true;

	return true;
}

//登录完成
bool CPlazaViewServer::OnSocketSubLogonFinish(VOID * pData, WORD wDataSize)
{
	//参数校验
	ASSERT(wDataSize==sizeof(CMD_GR_LogonFinish));
	if(wDataSize!=sizeof(CMD_GR_LogonFinish)) return false;

	//提取数据
	CMD_GR_LogonFinish * pLogonFinish = (CMD_GR_LogonFinish *)pData;

	//关闭提示
	m_DlgStatus.HideStatusWindow();

	//设置状态
	SetServiceStatus(ServiceStatus_ServiceIng);

	//激活房间
	CPlatformFrame * pPlatformFrame=CPlatformFrame::GetInstance();
	if (pPlatformFrame!=NULL) pPlatformFrame->ActiveServerViewItem();

	//用户规则
	CControlUI * pButtonControl = GetControlByName(szButtonTableButton4ControlName);
	if(pButtonControl!=NULL)
	{
		pButtonControl->SetEnabled(false);		
		if (CServerRule::IsForfendGameRule(m_dwServerRule)==false && m_wChairCount < MAX_CHAIR)
		{
			//发送规则
			SendUserRulePacket();

			//使能设置
			pButtonControl->SetEnabled(true);		
		}
	}

	//锁桌判断
	pButtonControl = GetControlByName(szButtonTableButton3ControlName);
	if(pButtonControl!=NULL) 
	{
		pButtonControl->SetEnabled(false);
		if (CServerRule::IsForfendLockTable(m_dwServerRule)==false && m_wChairCount < MAX_CHAIR &&
			CServerRule::IsAllowAvertCheatMode(m_dwServerRule)==false)
		{
			 pButtonControl->SetEnabled(true);
		}
	}

	//查找按钮
	pButtonControl = GetControlByName(szButtonTableButton2ControlName);
	if(pButtonControl!=NULL)
	{	
		pButtonControl->SetEnabled(true);
		if(CServerRule::IsAllowAvertCheatMode(m_dwServerRule)==true || 
			m_wServerType==GAME_GENRE_MATCH )
		{
			pButtonControl->SetEnabled(false);
		}
	}

	//加入按钮
	pButtonControl = GetControlByName(szButtonTableButton1ControlName);
	if(pButtonControl!=NULL)
	{	
		pButtonControl->SetEnabled(true);
		if (CServerRule::IsAllowAvertCheatMode(m_dwServerRule)==true ||
			m_wServerType==GAME_GENRE_MATCH )
		{
			pButtonControl->SetEnabled(false);
		}
	}

	//重入判断
	if ((m_pIMySelfUserItem!=NULL)&&(m_pIMySelfUserItem->GetTableID()!=INVALID_TABLE))
	{
		//启动进程	
		bool bSuccess=m_ProcessManager.CreateGameProcess(m_GameKind.szProcessName);

		//错误处理
		if (bSuccess==false)
		{
			//中断连接
			m_TCPSocketModule->CloseSocket();
			m_ServiceStatus=ServiceStatus_NetworkDown;
		}		
	}

	////不用提示
	////引领任务
	//if(pLogonFinish->bGuideTask==true)
	//{
	//	SetTimer(IDI_GUIDE_TASK,1000L+rand()%TIME_GUIDE_TASK,NULL);
	//}

	//追踪玩家
	CGlobalUnits * pGlobalUnits = CGlobalUnits::GetInstance();
	DWORD dwTrackUserID = pGlobalUnits->GetTrackUserID();
	if(pGlobalUnits!=NULL && dwTrackUserID!=0)
	{
		//激活用户
		ASSERT(m_PlazaUserManagerModule.GetInterface()!=NULL);
		IClientUserItem * pIClientUserItem=m_PlazaUserManagerModule->SearchUserByUserID(dwTrackUserID);
		if(pIClientUserItem && pIClientUserItem->GetTableID()!=INVALID_TABLE && pIClientUserItem->GetChairID()!=INVALID_CHAIR)
		{
			//变量定义
			tagSearchTable SearchTable;
			ZeroMemory(&SearchTable,sizeof(SearchTable));

			//搜索条件
			SearchTable.bAllNull=true;
			SearchTable.bNotFull=true;
			SearchTable.bOneNull=true;
			SearchTable.bFilterPass=true;
			SearchTable.bTwoNull=(m_pITableViewFrame->GetChairCount()!=2);

			//搜索结果
			SearchTable.wResultChairID=INVALID_CHAIR;
			SearchTable.wResultTableID=pIClientUserItem->GetTableID();			
			SearchTable.wStartTableID=SearchTable.wResultTableID;

			//搜索桌子
			SearchGameTable(SearchTable);
			if(SearchTable.wResultChairID!=INVALID_CHAIR && SearchTable.wResultTableID==SearchTable.wStartTableID  && CServerRule::IsAllowAvertCheatMode(m_dwServerRule)==false)
			{
				//设置数据
				WORD wChairID=SearchTable.wResultChairID;
				m_pITableViewFrame->VisibleTable(SearchTable.wResultTableID);
				m_pITableViewFrame->FlashGameChair(SearchTable.wResultTableID,wChairID);

				//执行坐下
				PerformSitDownAction(SearchTable.wResultTableID,wChairID,true);
			}
			else
			{
				//执行旁观
				PerformLookonAction(SearchTable.wStartTableID,pIClientUserItem->GetChairID());
			}			

			return true;
		}
	}

	//查询赠送
	CMD_GR_C_QuerySendPresent QuerySendPresent={0};
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();
	QuerySendPresent.dwUserID=pGlobalUserData->dwUserID;

	//发送数据

	m_TCPSocketModule->SendData(MDM_GR_PROPERTY, SUB_GR_QUERY_SEND_PRESENT, &QuerySendPresent, sizeof(QuerySendPresent));

	return true;
}

//登录成功
bool CPlazaViewServer::OnSocketSubLogonSuccess(VOID * pData, WORD wDataSize)
{
	//效验参数
	ASSERT(wDataSize==sizeof(CMD_GR_LogonSuccess));
	if (wDataSize!=sizeof(CMD_GR_LogonSuccess)) return false;

	//消息处理
	CMD_GR_LogonSuccess * pLogonSuccess=(CMD_GR_LogonSuccess *)pData;

	//玩家属性
	m_dwUserRight=pLogonSuccess->dwUserRight;
	m_dwMasterRight=pLogonSuccess->dwMasterRight;

	//设置状态
	SetServiceStatus(ServiceStatus_RecvInfo);

	//关闭提示
	//m_DlgUpdateStatus.StartEnterServer(/*TEXT("正在读取房间信息...")*/);

	//获取对象
	CGlobalWebLink * pGlobalWebLink=CGlobalWebLink::GetInstance();

	//构造地址
	TCHAR szBillUrl[256]=TEXT("");
	_sntprintf(szBillUrl,CountArray(szBillUrl),TEXT("%s/Ads/GameRoom.aspx?KindID=%ld&ServerID=%ld"),pGlobalWebLink->GetPlatformLink(),m_GameKind.wKindID,m_GameServer.wServerID);

	////连接广告
	//m_PlatformPublicize.Navigate(szBillUrl);
	//m_PlatformPublicize.SetBoradColor(RGB(0,0,0));
	ASSERT(m_ImageGameName.IsNull()==true);
	if (m_ImageGameName.IsNull()==false) m_ImageGameName.DestroyImage();

	//工作路径
	TCHAR szDirectory[MAX_PATH]=TEXT("");
	CWHService::GetWorkDirectory(szDirectory,CountArray(szDirectory));

	//获取文件
	TCHAR szGameName[64]=TEXT("");
	CString strProcessName(m_GameKind.szProcessName);
	INT nPos=strProcessName.Find(TEXT("."));
	_sntprintf(szGameName,CountArray(szGameName),TEXT("%s"),strProcessName.Left(nPos));

	//构造路径
	TCHAR szFilePath[MAX_PATH]=TEXT("");
	_sntprintf(szFilePath,CountArray(szFilePath),TEXT("%s\\GameKindLogo\\Room_%s.png"),szDirectory,szGameName);

	//加载图片
	m_ImageGameName.LoadImage(szFilePath);

	return true;
}

//登录失败
bool CPlazaViewServer::OnSocketSubLogonFailure(VOID * pData, WORD wDataSize)
{
	//效验参数
	CMD_GR_LogonFailure * pLogonFailure=(CMD_GR_LogonFailure *)pData;
	ASSERT(wDataSize>=(sizeof(CMD_GR_LogonFailure)-sizeof(pLogonFailure->szDescribeString)));
	if (wDataSize<(sizeof(CMD_GR_LogonFailure)-sizeof(pLogonFailure->szDescribeString))) return false;

	//关闭提示
	m_DlgStatus.HideStatusWindow();

	//关闭处理
	m_TCPSocketModule->CloseSocket();

	//设置状态
	SetServiceStatus(ServiceStatus_NetworkDown);

	//更新信息
	if(pLogonFailure->lErrorCode == LOGON_FAIL_SERVER_INVALIDATION)
	{
		//设置任务
		CPlatformFrame * pPlatformFrame=CPlatformFrame::GetInstance();
		//if (pPlatformFrame!=NULL) pPlatformFrame->ListGameServerInvalidation(m_GameKind.wKindID, m_GameServer.wServerID);
	}

	//显示消息
	LPCTSTR pszDescribeString=pLogonFailure->szDescribeString;
	if (lstrlen(pszDescribeString)>0)
	{
		CDlgInformation Information(this);
		Information.ShowMessageBox(pszDescribeString,MB_ICONERROR,60);
	}

	//关闭房间
	PostMessage(WM_COMMAND,IDM_DELETE_SERVER_ITEM,0);

	return true;
}

//更新提示
bool CPlazaViewServer::OnSocketSubUpdateNotify(VOID * pData, WORD wDataSize)
{
	//效验参数
	ASSERT(wDataSize==sizeof(CMD_GR_UpdateNotify));
	if (wDataSize!=sizeof(CMD_GR_UpdateNotify)) return false;

	//变量定义
	CMD_GR_UpdateNotify * pUpdateNotify=(CMD_GR_UpdateNotify *)pData;

	//大厅更新
	if (pUpdateNotify->cbMustUpdatePlaza==TRUE)
	{
		//关闭处理
		m_DlgStatus.HideStatusWindow();
		m_TCPSocketModule->CloseSocket();

		//关闭房间
		PostMessage(WM_COMMAND,IDM_DELETE_SERVER_ITEM,0);

		//下载大厅
		//CGlobalUnits * pGlobalUnits=CGlobalUnits::GetInstance();
		//if (pGlobalUnits!=NULL) pGlobalUnits->DownLoadClient(TEXT("游戏广场"),0,0);

		//构造提示
		TCHAR szDescribe[512]=TEXT("");
		_sntprintf(szDescribe,CountArray(szDescribe),TEXT("大厅已经更新，需要更新大厅。"));

		//提示消息
		CDlgInformation Information(NULL);
		Information.ShowMessageBox(szDescribe,MB_ICONINFORMATION,0);

		//工作目录
		TCHAR szDirectory[MAX_PATH]=TEXT("");
		CWHService::GetWorkDirectory(szDirectory,CountArray(szDirectory));

		//目标目录
		TCHAR szTargetPath[MAX_PATH]=TEXT("");
		_sntprintf(szTargetPath,CountArray(szTargetPath),TEXT("%s\\Update.exe"),szDirectory);

		//启动升级
		ShellExecute(NULL,TEXT("open"),szTargetPath,TEXT(""),NULL,true);

		CPaintManagerUI::InvokAllCloseCallBack();

		CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
		tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();
		pGlobalUserData->dwUserID = 0L;

		//关闭窗口
		AfxGetMainWnd()->PostMessage(WM_CLOSE,0,0);

		return true;
	}

	//游戏更新
	if ((pUpdateNotify->cbMustUpdateClient==TRUE)||(pUpdateNotify->cbAdviceUpdateClient==TRUE))
	{
		//关闭处理
		if (pUpdateNotify->cbMustUpdateClient==TRUE)
		{
			m_DlgStatus.HideStatusWindow();
			m_TCPSocketModule->CloseSocket();
		}



		////构造提示
		//if (pUpdateNotify->cbMustUpdateClient==FALSE)
		//{
			//构造提示
			TCHAR szDescribe[512]=TEXT("");
			_sntprintf(szDescribe,CountArray(szDescribe),TEXT("“%s”已经更新了，我们强烈建议您进行更新，现在进行更新吗？"),m_GameKind.szKindName);

			//提示消息
			CDlgInformation Information(this);
			INT nResult = Information.ShowMessageBox(szDescribe,MB_ICONINFORMATION|MB_YESNO,0);
		//}

			if (nResult==IDYES)
			{
				//直接下载
				CPlatformFrame::GetInstance()->DownLoadGame(m_GameKind.szKindName,m_GameKind.wKindID,m_GameServer.wServerID);
				CPlatformFrame::GetInstance()->ReturnHallMain();
			}
			//关闭房间
			PostMessage(WM_COMMAND,IDM_DELETE_SERVER_ITEM,0);
	}

	return true;
}

//请求失败
bool CPlazaViewServer::OnSocketSubRequestFailure(VOID * pData, WORD wDataSize)
{
	//变量定义
	CMD_GR_RequestFailure * pRequestFailure=(CMD_GR_RequestFailure *)pData;

	//效验参数
	ASSERT(wDataSize>(sizeof(CMD_GR_RequestFailure)-sizeof(pRequestFailure->szDescribeString)));
	if (wDataSize<=(sizeof(CMD_GR_RequestFailure)-sizeof(pRequestFailure->szDescribeString))) return false;

	//变量定义
	IClientUserItem * pITableUserItem=NULL;
	WORD wMeTableID=m_pIMySelfUserItem->GetTableID();
	WORD wMeChairID=m_pIMySelfUserItem->GetChairID();

	//界面还原
	if ((m_wReqTableID!=INVALID_TABLE)&&(m_wReqChairID!=INVALID_TABLE)&&(m_wReqChairID!=INDEX_ENTER_CHAIR))
	{
		IClientUserItem * pITableUserItem=m_pITableViewFrame->GetClientUserItem(m_wReqTableID,m_wReqChairID);
		if (pITableUserItem==m_pIMySelfUserItem) m_pITableViewFrame->SetClientUserItem(m_wReqTableID,m_wReqChairID,NULL);
	}

	//设置变量
	m_wReqTableID=INVALID_TABLE;
	m_wReqChairID=INVALID_CHAIR;

	//提示消息
	if (pRequestFailure->szDescribeString[0]!=0)
	{
		CDlgInformation Information;
		Information.ShowMessageBox(pRequestFailure->szDescribeString,MB_ICONINFORMATION,30L);
	}

	return true;
}

//用户聊天
bool CPlazaViewServer::OnSocketSubUserChat(VOID * pData, WORD wDataSize)
{
	//变量定义
	CMD_GR_S_UserChat * pUserChat=(CMD_GR_S_UserChat *)pData;

	//效验参数
	ASSERT(wDataSize>=(sizeof(CMD_GR_S_UserChat)-sizeof(pUserChat->szChatString)));
	ASSERT(wDataSize==(sizeof(CMD_GR_S_UserChat)-sizeof(pUserChat->szChatString)+pUserChat->wChatLength*sizeof(pUserChat->szChatString[0])));

	//效验参数
	if (wDataSize<(sizeof(CMD_GR_S_UserChat)-sizeof(pUserChat->szChatString))) return false;
	if (wDataSize!=(sizeof(CMD_GR_S_UserChat)-sizeof(pUserChat->szChatString)+pUserChat->wChatLength*sizeof(pUserChat->szChatString[0]))) return false;

	//获取用户
	IClientUserItem * pISendUserItem=m_PlazaUserManagerModule->SearchUserByUserID(pUserChat->dwSendUserID);
	if (pISendUserItem==NULL) return true;

	//消息过虑
	if (pISendUserItem!=m_pIMySelfUserItem)
	{
		//社团变量
		DWORD dwUserGroupID=pISendUserItem->GetGroupID();
		DWORD dwMySelfGroupID=m_pIMySelfUserItem->GetGroupID();

		//社团判断
		bool bSameGroup=false;
		if ((dwMySelfGroupID!=0L)&&(dwMySelfGroupID==dwUserGroupID)) bSameGroup=true;

		//变量定义
		BYTE cbUserCompanion=pISendUserItem->GetUserCompanion();
		CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();

		//好友模式
		if ((pParameterGlobal->m_cbMessageMode==MESSAGE_MODE_FRIEND)&&(cbUserCompanion!=CP_FRIEND)&&(bSameGroup==false))
		{
			return true;
		}

		//厌恶模式
		if ((pParameterGlobal->m_cbMessageMode==MESSAGE_MODE_DETEST)&&(cbUserCompanion==CP_DETEST)&&(bSameGroup==false))
		{
			return true;
		}
	}

	//获取指针
	IStringMessage * pIStringMessage = m_pPlazaViewServerCtrl->GetStringMessage();
	ASSERT(pIStringMessage!=NULL);

	//显示消息
	if (pUserChat->dwTargetUserID!=0L)
	{
		//获取用户
		IClientUserItem * pIRecvUserItem=m_PlazaUserManagerModule->SearchUserByUserID(pUserChat->dwTargetUserID);
		if (pIRecvUserItem==NULL) return true;

		//显示消息
		LPCTSTR pszSendUser=pISendUserItem->GetNickName();
		LPCTSTR pszRecvUser=pIRecvUserItem->GetNickName();
		pIStringMessage->InsertUserChat(pszSendUser,pszRecvUser,pUserChat->szChatString,pUserChat->dwChatColor);
	}
	else 
	{
		//显示消息
		LPCTSTR pszSendUser=pISendUserItem->GetNickName();
		pIStringMessage->InsertUserChat(pszSendUser,pUserChat->szChatString,pUserChat->dwChatColor);
	}

	return true;
}

//用户表情
bool CPlazaViewServer::OnSocketSubExpression(VOID * pData, WORD wDataSize)
{
	//效验参数
	ASSERT(wDataSize==sizeof(CMD_GR_S_UserExpression));
	if (wDataSize!=sizeof(CMD_GR_S_UserExpression)) return false;

	//变量定义
	CMD_GR_S_UserExpression * pUserExpression=(CMD_GR_S_UserExpression *)pData;

	//获取用户
	IClientUserItem * pISendUserItem=m_PlazaUserManagerModule->SearchUserByUserID(pUserExpression->dwSendUserID);
	if (pISendUserItem==NULL) return true;

	//获取指针
	IStringMessage * pIStringMessage = m_pPlazaViewServerCtrl->GetStringMessage();
	ASSERT(pIStringMessage!=NULL);

	//消息过虑
	if (pISendUserItem!=m_pIMySelfUserItem)
	{
		//社团变量
		DWORD dwUserGroupID=pISendUserItem->GetGroupID();
		DWORD dwMySelfGroupID=m_pIMySelfUserItem->GetGroupID();

		//社团判断
		bool bSameGroup=false;
		if ((dwMySelfGroupID!=0L)&&(dwMySelfGroupID==dwUserGroupID)) bSameGroup=true;

		//变量定义
		BYTE cbUserCompanion=pISendUserItem->GetUserCompanion();
		CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();

		//好友模式
		if ((pParameterGlobal->m_cbMessageMode==MESSAGE_MODE_FRIEND)&&(cbUserCompanion!=CP_FRIEND)&&(bSameGroup==false))
		{
			return true;
		}

		//厌恶模式
		if ((pParameterGlobal->m_cbMessageMode==MESSAGE_MODE_DETEST)&&(cbUserCompanion==CP_DETEST)&&(bSameGroup==false))
		{
			return true;
		}
	}

	//公聊消息
	if (pUserExpression->dwTargetUserID==0L)
	{
		//变量定义
		CExpressionManager * pExpressionManager=CExpressionManager::GetInstance();
		CExpression * pExpressionItem=pExpressionManager->GetExpressionItem(pUserExpression->wItemIndex);

		//插入表情
		if (pExpressionItem!=NULL)
		{
			//获取路径
			TCHAR szImagePath[MAX_PATH]=TEXT("");
			pExpressionItem->GetExpressionPath(szImagePath,CountArray(szImagePath));

			//插入消息
			LPCTSTR pszSendUser=pISendUserItem->GetNickName();
			pIStringMessage->InsertExpression(pszSendUser,szImagePath);
		}
	}

	//私聊消息
	if (pUserExpression->dwTargetUserID!=0L)
	{
		//变量定义
		CExpressionManager * pExpressionManager=CExpressionManager::GetInstance();
		CExpression * pExpressionItem=pExpressionManager->GetExpressionItem(pUserExpression->wItemIndex);

		//插入表情
		if (pExpressionItem!=NULL)
		{
			//获取路径
			TCHAR szImagePath[MAX_PATH]=TEXT("");
			pExpressionItem->GetExpressionPath(szImagePath,CountArray(szImagePath));

			//获取用户
			IClientUserItem * pIRecvUserItem=m_PlazaUserManagerModule->SearchUserByUserID(pUserExpression->dwTargetUserID);
			if (pIRecvUserItem==NULL) return true;

			//显示消息
			LPCTSTR pszSendUser=pISendUserItem->GetNickName();
			LPCTSTR pszRecvUser=pIRecvUserItem->GetNickName();
			pIStringMessage->InsertExpression(pszSendUser,pszRecvUser,szImagePath);
		}
	}

	return true;
}

//用户私聊
bool CPlazaViewServer::OnSocketSubWisperUserChat(VOID * pData, WORD wDataSize)
{
	//变量定义
	CMD_GR_S_WisperChat * pWisperChat=(CMD_GR_S_WisperChat *)pData;

	//效验参数
	ASSERT(wDataSize>=(sizeof(CMD_GR_S_WisperChat)-sizeof(pWisperChat->szChatString)));
	ASSERT(wDataSize==(sizeof(CMD_GR_S_WisperChat)-sizeof(pWisperChat->szChatString)+pWisperChat->wChatLength*sizeof(pWisperChat->szChatString[0])));

	//效验参数
	if (wDataSize<(sizeof(CMD_GR_S_WisperChat)-sizeof(pWisperChat->szChatString))) return false;
	if (wDataSize!=(sizeof(CMD_GR_S_WisperChat)-sizeof(pWisperChat->szChatString)+pWisperChat->wChatLength*sizeof(pWisperChat->szChatString[0]))) return false;

	//获取用户
	IClientUserItem * pISendUserItem=m_PlazaUserManagerModule->SearchUserByUserID(pWisperChat->dwSendUserID);
	if (pISendUserItem==NULL) return true;

	//接收用户
	IClientUserItem * pIRecvUserItem=m_PlazaUserManagerModule->SearchUserByUserID(pWisperChat->dwTargetUserID);
	if (pIRecvUserItem==NULL) return true;

	//消息过虑
	if (pISendUserItem!=m_pIMySelfUserItem)
	{
		//社团变量
		DWORD dwUserGroupID=pISendUserItem->GetGroupID();
		DWORD dwMySelfGroupID=m_pIMySelfUserItem->GetGroupID();

		//社团判断
		bool bSameGroup=false;
		if ((dwMySelfGroupID!=0L)&&(dwMySelfGroupID==dwUserGroupID)) bSameGroup=true;

		//变量定义
		BYTE cbUserCompanion=pISendUserItem->GetUserCompanion();
		CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();

		//好友模式
		if ((pParameterGlobal->m_cbMessageMode==MESSAGE_MODE_FRIEND)&&(cbUserCompanion!=CP_FRIEND)&&(bSameGroup==false))
		{
			return true;
		}

		//厌恶模式
		if ((pParameterGlobal->m_cbMessageMode==MESSAGE_MODE_DETEST)&&(cbUserCompanion==CP_DETEST)&&(bSameGroup==false))
		{
			return true;
		}

		//屏蔽用户
		if(cbUserCompanion == CP_SHIELD) return true;
	}

	//创建窗口
	DWORD dwUserID[2]={0L,0L};
	dwUserID[0]=m_pIMySelfUserItem->GetUserID();
	dwUserID[1]=(pISendUserItem==m_pIMySelfUserItem)?pIRecvUserItem->GetUserID():pISendUserItem->GetUserID();
	CDlgWhisper * pDlgWhisper=CreateWhisperWnd(0L,pWisperChat->dwTargetUserID,dwUserID,CountArray(dwUserID),(pISendUserItem!=m_pIMySelfUserItem));

	//显示窗口
	if (pDlgWhisper!=NULL)
	{
		pDlgWhisper->OnEventWisperChat(pISendUserItem,pWisperChat->szChatString,pWisperChat->dwChatColor);
	}

	return true;
}

//私聊表情
bool CPlazaViewServer::OnSocketSubWisperExpression(VOID * pData, WORD wDataSize)
{
	//效验参数
	ASSERT(wDataSize==sizeof(CMD_GR_S_WisperExpression));
	if (wDataSize!=sizeof(CMD_GR_S_WisperExpression)) return false;

	//变量定义
	CMD_GR_S_WisperExpression * pWisperExpression=(CMD_GR_S_WisperExpression *)pData;

	//获取用户
	IClientUserItem * pISendUserItem=m_PlazaUserManagerModule->SearchUserByUserID(pWisperExpression->dwSendUserID);
	if (pISendUserItem==NULL) return true;

	//接收用户
	IClientUserItem * pIRecvUserItem=m_PlazaUserManagerModule->SearchUserByUserID(pWisperExpression->dwTargetUserID);
	if (pIRecvUserItem==NULL) return true;

	//消息过虑
	if (pISendUserItem!=m_pIMySelfUserItem)
	{
		//社团变量
		DWORD dwUserGroupID=pISendUserItem->GetGroupID();
		DWORD dwMySelfGroupID=m_pIMySelfUserItem->GetGroupID();

		//社团判断
		bool bSameGroup=false;
		if ((dwMySelfGroupID!=0L)&&(dwMySelfGroupID==dwUserGroupID)) bSameGroup=true;

		//变量定义
		BYTE cbUserCompanion=pISendUserItem->GetUserCompanion();
		CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();

		//好友模式
		if ((pParameterGlobal->m_cbMessageMode==MESSAGE_MODE_FRIEND)&&(cbUserCompanion!=CP_FRIEND)&&(bSameGroup==false))
		{
			return true;
		}

		//厌恶模式
		if ((pParameterGlobal->m_cbMessageMode==MESSAGE_MODE_DETEST)&&(cbUserCompanion==CP_DETEST)&&(bSameGroup==false))
		{
			return true;
		}

		//屏蔽用户
		if(cbUserCompanion == CP_SHIELD) return true;
	}

	//创建窗口
	DWORD dwUserID[2]={0L,0L};
	dwUserID[0]=m_pIMySelfUserItem->GetUserID();
	dwUserID[1]=(pISendUserItem==m_pIMySelfUserItem)?pIRecvUserItem->GetUserID():pISendUserItem->GetUserID();
	CDlgWhisper * pDlgWhisper=CreateWhisperWnd(0L,pWisperExpression->dwTargetUserID,dwUserID,CountArray(dwUserID),(pISendUserItem!=m_pIMySelfUserItem));

	//显示窗口
	if (pDlgWhisper!=NULL)
	{
		pDlgWhisper->OnEventExpression(pISendUserItem,pWisperExpression->wItemIndex);
	}

	return true;
}

//道具成功
bool CPlazaViewServer::OnSocketSubPropertySuccess(VOID * pData, WORD wDataSize)
{
	//效验参数
	ASSERT(wDataSize==sizeof(CMD_GR_S_PropertySuccess));
	if (wDataSize!=sizeof(CMD_GR_S_PropertySuccess)) return false;

	//变量定义
	CMD_GR_S_PropertySuccess * pPropertySuccess=(CMD_GR_S_PropertySuccess *)pData;

	//获取用户
	IClientUserItem * pISendUserItem=m_PlazaUserManagerModule->SearchUserByUserID(pPropertySuccess->dwSourceUserID);
	if (pISendUserItem==NULL) return true;

	//获取用户
	IClientUserItem * pTargetUserItem=m_PlazaUserManagerModule->SearchUserByUserID(pPropertySuccess->dwTargetUserID);
	if (pTargetUserItem==NULL) return true;

	//使用范围
	if(pPropertySuccess->wKind==1&& IsGameClientReady())
	{
		//设置变量
		IPC_GR_PropertySuccess  PropertySuccess;  
		PropertySuccess.wPropertyIndex=pPropertySuccess->wPropertyIndex;
		PropertySuccess.wPropertyCount=pPropertySuccess->wItemCount;
		PropertySuccess.dwSourceUserID=pPropertySuccess->dwSourceUserID;
		PropertySuccess.dwTargetUserID=pPropertySuccess->dwTargetUserID;
		CopyMemory(PropertySuccess.szSourceNickName,pISendUserItem->GetNickName(),sizeof(PropertySuccess.szSourceNickName));

		//发送数据
		bool bSendSuccess = m_ProcessManager.SendProcessData(IPC_CMD_GF_PROPERTY_INFO,IPC_SUB_GF_PROPERTY_SUCCESS,&PropertySuccess,sizeof(PropertySuccess));

		if(bSendSuccess==true) return true;
	}
	
	//获取对象
	ASSERT(CGamePropertyManager::GetInstance()!=NULL);
	CGamePropertyManager * pGamePropertyManager=CGamePropertyManager::GetInstance();

	//寻找道具
	ASSERT(pGamePropertyManager->GetPropertyItem(pPropertySuccess->wPropertyIndex)!=NULL);
	CGamePropertyBase * pGamePropertyItem=pGamePropertyManager->GetPropertyItem(pPropertySuccess->wPropertyIndex);

	//道具处理
	if (pPropertySuccess->wKind==1)
	{
		//图片信息
		tagPropertyImage PropertyImage={0};
		pGamePropertyItem->GetImageInfo(PropertyImage);

		//播放动画
		if ((PropertyImage.pszFlash!=NULL)&&(PropertyImage.pszFlash[0]!=0))
		{
			//工作路径
			TCHAR szDirectory[MAX_PATH]=TEXT("");
			CWHService::GetWorkDirectory(szDirectory,CountArray(szDirectory));

			//构造路径
			TCHAR szFlashPath[MAX_PATH]=TEXT("");
			_sntprintf(szFlashPath,CountArray(szFlashPath),TEXT("%s\\Property\\%s"),szDirectory,PropertyImage.pszFlash);

			try
			{
				//播放动画
				if (m_FlashControl.LoadMovieFile(szFlashPath)==false)
				{
					throw TEXT("动画文件加载失败");
				}

				//动画控件
				CRect rcWindow;
				m_pITableViewFrame->GetViewWnd()->GetWindowRect(&rcWindow);
				m_FlashControl.SetPosition(rcWindow.left+(rcWindow.Width()-FALSH_MOVIE_CX)/2,rcWindow.top+(rcWindow.Height()-FALSH_MOVIE_CY)*2/5,FALSH_MOVIE_CX,FALSH_MOVIE_CY,SWP_NOSIZE);

				//播放动画
				m_FlashControl.PlayMovie();
				m_FlashControl.ShowControl(SW_SHOW);
			}
			catch (...)
			{
				ASSERT(FALSE);
			}
		}
	}

	//道具提示
	if(pPropertySuccess->wKind != 1) //礼物类型的道具不提示
	{
		//变量定义
		TCHAR  szMessage[256]=TEXT("");
		
		//获取提示
		if(pPropertySuccess->dwSourceUserID==m_pIMySelfUserItem->GetUserID() && pPropertySuccess->dwSourceUserID!=pPropertySuccess->dwTargetUserID)
            _sntprintf(szMessage,CountArray(szMessage),TEXT("恭喜您,道具购买成功!"));
		else
			pGamePropertyItem->GetNotifyMessage(pISendUserItem->GetNickName(),m_pIMySelfUserItem->GetNickName(),pPropertySuccess->wItemCount,szMessage);

		//弹出消息
		CDlgInformation Information;
		Information.ShowMessageBox(szMessage,MB_ICONINFORMATION);

		//更新喇叭
		//if(m_pTrumpetItem!=NULL)  m_pTrumpetItem->UpdateControlSurface();
	}

	return true;
}

//道具失败
bool CPlazaViewServer::OnSocketSubPropertyFailure(VOID * pData, WORD wDataSize)
{
	//变量定义
	CMD_GR_PropertyFailure * pPropertyFailure=(CMD_GR_PropertyFailure *)pData;
	if(pPropertyFailure==NULL) return false;

	//使用范围
	if(pPropertyFailure->wRequestArea==PT_ISSUE_AREA_GAME && IsGameClientReady())
	{
		//设置变量
		IPC_GR_PropertyFailure  PropertyFailure;  
		PropertyFailure.lErrorCode=pPropertyFailure->lErrorCode;
		CopyMemory(PropertyFailure.szDescribeString,pPropertyFailure->szDescribeString,sizeof(PropertyFailure.szDescribeString));

		//发送数据
		bool bSendSuccess = m_ProcessManager.SendProcessData(IPC_CMD_GF_PROPERTY_INFO,IPC_SUB_GF_PROPERTY_FAILURE,&PropertyFailure,sizeof(PropertyFailure));

		if(bSendSuccess==true) return true; 
	}

	//弹出消息
	CDlgInformation Information;
	Information.ShowMessageBox(pPropertyFailure->szDescribeString,MB_ICONINFORMATION);

	return true;
}

//道具效应
bool CPlazaViewServer::OnSocketSubPropertyEffect(VOID * pData, WORD wDataSize)
{
	//参数校验
	ASSERT(wDataSize==sizeof(CMD_GR_S_PropertyEffect));
	if(wDataSize!=sizeof(CMD_GR_S_PropertyEffect))  return false;

	//变量定义
	CMD_GR_S_PropertyEffect * pPropertyEffect=(CMD_GR_S_PropertyEffect *)pData;
	if(pPropertyEffect==NULL) return false;

	//查找用户
	IClientUserItem * pISendUserItem=m_PlazaUserManagerModule->SearchUserByUserID(pPropertyEffect->wUserID);
	if(pISendUserItem==NULL) return true;

	//设置变量
	tagUserInfo * pUserInfo = pISendUserItem->GetUserInfo();
	if(pPropertyEffect->wPropKind == PROP_KIND_VIP)
	{
		pUserInfo->cbMemberOrder=pPropertyEffect->cbMemberOrder;
	}
	else if( pPropertyEffect->wPropKind == PROP_KIND_ESCAPE_CLEAR)
	{
		pUserInfo->dwFleeCount = pPropertyEffect->dwFleeCount;
	}
	else if( pPropertyEffect->wPropKind == PROP_KIND_NEGATIVE_SCORE_CLEAR)
	{
		pUserInfo->lScore = pPropertyEffect->lScore;
	}
	//更新信息
	m_pPlazaViewServerCtrl->OnEventUserUpdate(pISendUserItem);
    
    return true;
}

//邀请玩家
bool CPlazaViewServer::OnSocketSubUserInvite(VOID * pData, WORD wDataSize)
{
	//效验参数
	ASSERT(wDataSize==sizeof(CMD_GR_UserInvite));
	if (wDataSize!=sizeof(CMD_GR_UserInvite)) return false;

	//消息处理
	CMD_GR_UserInvite * pUserInvite=(CMD_GR_UserInvite *)pData;
	ASSERT(pUserInvite->wTableID<m_pITableViewFrame->GetTableCount());
	if (m_pITableViewFrame->GetPlayFlag(pUserInvite->wTableID)==true) return true;

	//寻找用户
	IClientUserItem * pIUserItem=m_PlazaUserManagerModule->SearchUserByUserID(pUserInvite->dwUserID);
	if (pIUserItem==NULL) return true;
	if (pIUserItem->GetTableID()==INVALID_TABLE) return true;

	//变量定义
	ASSERT(CParameterGlobal::GetInstance()!=NULL);
	CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();

	//用户过虑
	if (pIUserItem->GetUserCompanion()==CP_DETEST) return true;
	if (pParameterGlobal->m_cbInviteMode==INVITE_MODE_NONE) return true;
	if ((pParameterGlobal->m_cbInviteMode==INVITE_MODE_FRIEND)&&(pIUserItem->GetUserCompanion()!=CP_FRIEND)) return true;

	//提示消息
	TCHAR szMessage[256]=TEXT("");
	_sntprintf(szMessage,CountArray(szMessage),TEXT("[ %s ] 邀请您加入第 [ %ld ] 游戏桌进行游戏，是否同意？"),pIUserItem->GetNickName(),pUserInvite->wTableID+1);
	//提示消息
	CDlgInformation Information(this);
	if (Information.ShowMessageBox(szMessage,MB_ICONINFORMATION|MB_YESNO)==IDYES)
	{
		WORD wChairID=INVALID_CHAIR;
		WORD wNullCount=m_pITableViewFrame->GetNullChairCount(pUserInvite->wTableID,wChairID);
		if (wNullCount==0) 
		{
			CDlgInformation Information2(this);
			Information2.ShowMessageBox(TEXT("此游戏桌已经没有空位置了！"),MB_ICONINFORMATION);
			return true;
		}
		PerformSitDownAction(pUserInvite->wTableID,wChairID,false);
	}

	return true;
}

//道具消息
bool CPlazaViewServer::OnSocketPropertyMessage(VOID * pData, WORD wDataSize)
{
	//效验参数
	ASSERT(wDataSize==sizeof(CMD_GR_S_PropertyMessage));
	if (wDataSize!=sizeof(CMD_GR_S_PropertyMessage)) return false;

	//校验数据
	CMD_GR_S_PropertyMessage * pPropertyMessage = (CMD_GR_S_PropertyMessage *)pData;
	if(pPropertyMessage==NULL) return false;

    //获取对象
	ASSERT(CGamePropertyManager::GetInstance()!=NULL);
	CGamePropertyManager * pGamePropertyManager=CGamePropertyManager::GetInstance();

	//寻找道具
	ASSERT(pGamePropertyManager->GetPropertyItem(pPropertyMessage->wPropertyIndex)!=NULL);
	CGamePropertyBase * pGamePropertyItem=pGamePropertyManager->GetPropertyItem(pPropertyMessage->wPropertyIndex);
	if(pGamePropertyItem==NULL) return true;

	//类型判断
	if(pGamePropertyItem->GetPropertyAttrib()->wPropertyType==PT_TYPE_PROPERTY) return true;

	//获取用户
	IClientUserItem * pISendUserItem=m_PlazaUserManagerModule->SearchUserByUserID(pPropertyMessage->dwSourceUserID);
	if (pISendUserItem==NULL) return true;

	//获取用户
	IClientUserItem * pIRecvUserItem=m_PlazaUserManagerModule->SearchUserByUserID(pPropertyMessage->dwTargerUserID);
	if (pIRecvUserItem==NULL) return true;

	//提取信息
	TCHAR szNotifyMessage[128]=TEXT("");	
	pGamePropertyItem->GetNotifyMessage(pISendUserItem->GetNickName(),pIRecvUserItem->GetNickName(),pPropertyMessage->wPropertyCount,szNotifyMessage);

	//获取指针
	IStringMessage * pIStringMessage = m_pPlazaViewServerCtrl->GetStringMessage();
	ASSERT(pIStringMessage!=NULL);

	//插入消息
	pIStringMessage->InsertPromptString(szNotifyMessage);

	if(IsGameClientReady())
	{
		//构造结构
		IPC_GR_PropertyMessage  PropertyMessage;
		PropertyMessage.wPropertyIndex=pPropertyMessage->wPropertyIndex;
		PropertyMessage.wPropertyCount=pPropertyMessage->wPropertyCount;
		CopyMemory(PropertyMessage.szSourceNickName,pISendUserItem->GetNickName(),sizeof(PropertyMessage.szSourceNickName));
		CopyMemory(PropertyMessage.szTargerNickName,pIRecvUserItem->GetNickName(),sizeof(PropertyMessage.szTargerNickName));

		//发送数据
		m_ProcessManager.SendProcessData(IPC_CMD_GF_PROPERTY_INFO,IPC_SUB_GR_PROPERTY_MESSAGE,&PropertyMessage,sizeof(PropertyMessage));
	}

	return true;
}

//道具喇叭
bool CPlazaViewServer::OnSocketPropertyTrumpet(VOID * pData, WORD wDataSize)
{
	//效验参数
	ASSERT(wDataSize==sizeof(CMD_GR_S_SendTrumpet));
	if (wDataSize!=sizeof(CMD_GR_S_SendTrumpet)) return false;

	//变量定义
	CMD_GR_S_SendTrumpet * pSendTrumpet=(CMD_GR_S_SendTrumpet *)pData;

	//获取指针
	IStringMessage * pIStringMessage = m_pPlazaViewServerCtrl->GetStringMessage();
	ASSERT(pIStringMessage!=NULL);

    //插入消息
	if(pSendTrumpet->wPropertyIndex==PROPERTY_ID_TRUMPET)
		pIStringMessage->InsertUserTrumpet(pSendTrumpet->szSendNickName,pSendTrumpet->szTrumpetContent,pSendTrumpet->TrumpetColor);

	//插入消息
	if(pSendTrumpet->wPropertyIndex==PROPERTY_ID_TYPHON)
        pIStringMessage->InsertUserTyphon(pSendTrumpet->szSendNickName,pSendTrumpet->szTrumpetContent,pSendTrumpet->TrumpetColor);

	//更新包裹
	if(pSendTrumpet->dwSendUserID==m_pIMySelfUserItem->GetUserID())
	{
		//道具包裹
		tagPropertyPackage * pPropertyPackage=m_pIMySelfUserItem->GetPropertyPackage();

		//更新包裹
		if(pSendTrumpet->wPropertyIndex==PROPERTY_ID_TRUMPET) --pPropertyPackage->wTrumpetCount;
		if(pSendTrumpet->wPropertyIndex==PROPERTY_ID_TYPHON) --pPropertyPackage->wTyphonCount;
		//if(m_pTrumpetItem!=NULL) m_pTrumpetItem->UpdateControlSurface();
	}

	if(IsGameClientReady())
	{
		//构造结构
		IPC_GR_PropertyTrumpet  PropertyTrumpet;
		PropertyTrumpet.dwSendUserID=pSendTrumpet->dwSendUserID;
		PropertyTrumpet.wPropertyIndex=pSendTrumpet->wPropertyIndex;
		PropertyTrumpet.TrumpetColor=pSendTrumpet->TrumpetColor;
		CopyMemory(PropertyTrumpet.szSendNickName,pSendTrumpet->szSendNickName,sizeof(PropertyTrumpet.szSendNickName));
		CopyMemory(PropertyTrumpet.szTrumpetContent,pSendTrumpet->szTrumpetContent,sizeof(PropertyTrumpet.szTrumpetContent));

		//发送数据
		m_ProcessManager.SendProcessData(IPC_CMD_GF_PROPERTY_INFO,IPC_SUB_GR_PROPERTY_TRUMPET,&PropertyTrumpet,sizeof(PropertyTrumpet));
	}

	//变量定义
	ASSERT(CParameterGlobal::GetInstance()!=NULL);
	CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();

	//播放声音
	if(pParameterGlobal->m_bAllowSound)
	{
		//变量定义
		HINSTANCE hInstance = AfxGetInstanceHandle();
		bool bPlaying=false;

		//获取对象
		CD3DSound * pD3DSound=CD3DSound::GetInstance();
		if(pD3DSound)
		{
			bPlaying = pD3DSound->PlaySound(hInstance, (pSendTrumpet->wPropertyIndex==PROPERTY_ID_TRUMPET)?TEXT("BUGLE"):TEXT("BUGLE_EX"), TEXT("WAVE"), false);
		}

		//系统播放
		if(!bPlaying)
		{
			HRSRC hResour=FindResource(hInstance,(pSendTrumpet->wPropertyIndex==PROPERTY_ID_TRUMPET)?TEXT("BUGLE"):TEXT("BUGLE_EX"),TEXT("WAVE"));
			if (hResour!=NULL)
			{
				HGLOBAL hGlobal=LoadResource(hInstance,hResour);
				if (hGlobal!=NULL) 
				{
					//播放声音
					LPCTSTR pszMemory=(LPCTSTR)LockResource(hGlobal);
					BOOL bResult = ::PlaySound(pszMemory,hInstance,SND_ASYNC|SND_MEMORY|SND_NODEFAULT);

					//清理资源
					UnlockResource(hGlobal);
				}
				FreeResource(hGlobal);
			}
		}
	}

    return true;
}

//等待分配
bool CPlazaViewServer::OnSocketSubWaitDistribute(VOID *pData, WORD wDataSize)
{
	//更新桌面
	m_pITableViewFrame->SetTableStatus(true);

	//游戏通知
	OnSocketMainGameFrame(MDM_GF_FRAME,SUB_GR_USER_WAIT_DISTRIBUTE,pData,wDataSize);

	return true;
}

bool CPlazaViewServer::OnSocketSubUserPropBuff(VOID *pData, WORD wDataSize)
{
	CMD_GR_S_PropertyBuff* pPropertyBuff = (CMD_GR_S_PropertyBuff*)pData;

	//变量定义
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();
	
	IStringMessage * pIStringMessage = m_pPlazaViewServerCtrl->GetStringMessage();
	ASSERT(pIStringMessage!=NULL);

	CPlatformEvent * pPlatformEvent=CPlatformEvent::GetInstance();

	TCHAR szString[128]={0};
	for(int i = 0; i < pPropertyBuff->cbBuffCount; i++)
	{
		//发送事件
		if (pPlatformEvent!=NULL) pPlatformEvent->SendPlatformEvent(EVENT_USER_SHOW_UPDATE_BUFF, (LPARAM)&pPropertyBuff->PropertyBuff[i]);

		tString tBuffInfo;
		CStdString sTmpString;
		
		DWORD nTotalMins =(DWORD)((pPropertyBuff->PropertyBuff[i].tUseTime + pPropertyBuff->PropertyBuff[i].UseResultsValidTime)-time(NULL)) / 60;
		
		DWORD nDays = nTotalMins/(24*60);
		DWORD nHours = (nTotalMins-(nDays*24*60))/60;
		DWORD nMins = nTotalMins%60;

		if(nDays > 0)
		{
			sTmpString.Format(TEXT("%d天"), nDays);
			tBuffInfo += sTmpString.GetData();
		}
		if(nHours > 0)
		{
			sTmpString.Format(TEXT("%d小时"), nHours);
			tBuffInfo += sTmpString.GetData();
		}
		if(nMins > 0)
		{
			sTmpString.Format(TEXT("%d分钟"), nMins);
			tBuffInfo += sTmpString.GetData();
		}

		wsprintf(szString, TEXT("%s你的[%s]道具效果，还剩余%s"), pGlobalUserData->szNickName, pPropertyBuff->PropertyBuff[i].szName, tBuffInfo.c_str());
		//插入系统消息
		if(pIStringMessage) pIStringMessage->InsertSystemString(szString);
	}
	
	return true;
}

bool CPlazaViewServer::OnSocketSubUserBackpack(VOID *pData, WORD wDataSize)
{
	//背包消息处理
	if( m_pDlgBagServer != NULL )
	{
		return m_pDlgBagServer->OnServerBagMessage(MDM_GR_USER, SUB_GR_PROPERTY_BACKPACK_RESULT, pData, wDataSize);
	}
	
	//房间菜单使用道具
	if( m_pDlgPropertyUse != NULL )
	{
		return m_pDlgPropertyUse->OnServerPropertyUseMessage(MDM_GR_USER, SUB_GR_PROPERTY_BACKPACK_RESULT, pData, wDataSize);
	}
	return false;
}

bool CPlazaViewServer::OnSocketSubPropertyUse(VOID *pData, WORD wDataSize)
{
	WORD wSubCmdID = SUB_GR_PROPERTY_USE_RESULT;

	//商城
	if(m_pDlgShopServer != NULL)
	{
		m_pDlgShopServer->OnServerShopMessage(MDM_GR_PROPERTY, wSubCmdID, pData, wDataSize);
	}

	//背包消息
	if( m_pDlgBagServer != NULL )
	{
		m_pDlgBagServer->OnServerBagMessage(MDM_GR_PROPERTY, wSubCmdID, pData, wDataSize);
	}
	
	//使用道具
	if( m_pDlgPropertyUse != NULL )
	{
		m_pDlgPropertyUse->OnServerPropertyUseMessage(MDM_GR_PROPERTY, wSubCmdID, pData, wDataSize);
	}
	
	CMD_GR_S_PropertyUse* pPropertyUse = (CMD_GR_S_PropertyUse*)pData;
	if(pPropertyUse->dwUserID == m_pIMySelfUserItem->GetUserID() )
	{
		if(pPropertyUse->dwPropID == PROPERTY_ID_TRUMPET || pPropertyUse->dwPropID == PROPERTY_ID_TYPHON)
		{
			if(pPropertyUse->dwPropID == PROPERTY_ID_TRUMPET)
			{
				CPlatformFrame::GetInstance()->GetPlazaViewServerCtrl()->SetTrumpetCount(pPropertyUse->dwRemainderPropCount);
			}
			else if(pPropertyUse->dwPropID == PROPERTY_ID_TYPHON)
			{
				CPlatformFrame::GetInstance()->GetPlazaViewServerCtrl()->SetTyphonCount(pPropertyUse->dwRemainderPropCount);
			}

			if (IsGameClientReady())
			{
				//设置变量
				IPC_GF_Trumpet_Number UserTrumpet;
				UserTrumpet.dwTrumpetCount = pPropertyUse->dwRemainderPropCount;
				UserTrumpet.dwTyphonCount = pPropertyUse->dwRemainderPropCount;

				//发送数据
				bool bSendSuccess = m_ProcessManager.SendProcessData(IPC_CMD_GF_PROPERTY_INFO,IPC_SUB_GF_USER_TRUMPET,&UserTrumpet,sizeof(UserTrumpet));

				if(bSendSuccess==true) return true;
			}

		}
		UINT uType =  (pPropertyUse->dwHandleCode==0)?(MB_OK|MB_ICONINFORMATION) : (MB_OK|MB_ICONSTOP);
		//提示消息
		CDlgInformation Information(NULL);
		Information.ShowMessageBox(pPropertyUse->szNotifyContent,uType,30);
		
	}

	return true;
}

bool CPlazaViewServer::OnSocketSubQuerySendPresent(VOID *pData, WORD wDataSize)
{
	CMD_GR_S_QuerySendPresent* pQuerySendPresentResult = (CMD_GR_S_QuerySendPresent*)pData;
	if(pQuerySendPresentResult != NULL )
	{
		CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
		tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();
		pGlobalUserData->dwPresentCount = pQuerySendPresentResult->wPresentCount;

		if( pQuerySendPresentResult->wPresentCount > 0 )
		{
			vector<SendPresent> vecSendPresent;
			for(int i = 0; i < pQuerySendPresentResult->wPresentCount; i++)
			{
				vecSendPresent.push_back(pQuerySendPresentResult->Present[i]);
			}
			CDlgInformation Information;
			CStdString strNotifyContent;
			strNotifyContent.Format(TEXT("有人赠送%d个道具给你"), vecSendPresent.size());
			Information.ShowMessageBox(strNotifyContent.GetData(),MB_ICONINFORMATION);

			CGlobalUnits::GetInstance()->QuerySendPresentMisson();
		}
	}
	return true;
}

bool CPlazaViewServer::OnSocketSubPropertyPresent(VOID *pData, WORD wDataSize)
{
	if( m_pDlgBagServer != NULL )
	{
		return m_pDlgBagServer->OnServerBagMessage(MDM_GR_PROPERTY, SUB_GR_PROPERTY_PRESENT_RESULT, pData, wDataSize);
	}
	return false;
}

bool CPlazaViewServer::OnSocketSubGetSendPresent(VOID *pData, WORD wDataSize)
{
	if( m_pDlgBagServer != NULL )
	{
		return m_pDlgBagServer->OnServerBagMessage(MDM_GR_PROPERTY, SUB_GR_GET_SEND_PRESENT_RESULT, pData, wDataSize);
	}
	return false;
}

bool CPlazaViewServer::OnSocketSubUserTrumpet(VOID *pData, WORD wDataSize)
{
	ASSERT(wDataSize == sizeof(CMD_GR_S_UserTrumpet));

	CMD_GR_S_UserTrumpet* pUserTrumpet = (CMD_GR_S_UserTrumpet*)pData;

	CPlatformFrame::GetInstance()->GetPlazaViewServerCtrl()->SetTrumpetCount(pUserTrumpet->dwTrumpetCount);
	CPlatformFrame::GetInstance()->GetPlazaViewServerCtrl()->SetTyphonCount(pUserTrumpet->dwTyphonCount);

	if (IsGameClientReady())
	{
		//设置变量
		IPC_GF_Trumpet_Number UserTrumpet;
		UserTrumpet.dwTrumpetCount = pUserTrumpet->dwTrumpetCount;
		UserTrumpet.dwTyphonCount = pUserTrumpet->dwTyphonCount;

		//发送数据
		bool bSendSuccess = m_ProcessManager.SendProcessData(IPC_CMD_GF_PROPERTY_INFO,IPC_SUB_GF_USER_TRUMPET,&UserTrumpet,sizeof(UserTrumpet));

		if(bSendSuccess==true) return true;
	}

	return true;
}

//系统消息
bool CPlazaViewServer::OnSocketSubSystemMessage(VOID * pData, WORD wDataSize)
{
	//变量定义
	CMD_CM_SystemMessage * pSystemMessage=(CMD_CM_SystemMessage *)pData;
	WORD wHeadSize=sizeof(CMD_CM_SystemMessage)-sizeof(pSystemMessage->szString);

	//效验参数
	ASSERT((wDataSize>wHeadSize)&&(wDataSize==(wHeadSize+pSystemMessage->wLength*sizeof(TCHAR))));
	if ((wDataSize<=wHeadSize)||(wDataSize!=(wHeadSize+pSystemMessage->wLength*sizeof(TCHAR)))) return false;

	WORD wType = pSystemMessage->wType;

	//关闭处理
	if ((wType&(SMT_CLOSE_ROOM|SMT_CLOSE_LINK))!=0)
	{
		//关闭提示
		m_DlgStatus.HideStatusWindow();

		//关闭处理
		m_TCPSocketModule->CloseSocket();
		m_ProcessManager.CloseGameProcess(false);

		//状态设置
		m_ServiceStatus=ServiceStatus_NetworkDown;
	}

	//获取指针
	IStringMessage * pIStringMessage = m_pPlazaViewServerCtrl->GetStringMessage();
	ASSERT(pIStringMessage!=NULL);

	//显示消息
	if (wType&SMT_CHAT) 
	{
		pIStringMessage->InsertSystemString(pSystemMessage->szString);
	}

	//关闭银行
	if (wType&SMT_CLOSE_INSURE)
	{
		if(m_pDlgInsureServer != NULL) 
		{
			m_pDlgInsureServer->EndDialog(IDOK);
			m_pDlgInsureServer->DestroyWindow();
		}
	}

	//关闭游戏
	if (wType&SMT_CLOSE_GAME)
	{
		m_ProcessManager.CloseGameProcess(false);
	}

	//弹出消息
	if (wType&SMT_EJECT)
	{
		CDlgInformation Information;
		Information.ShowMessageBox(pSystemMessage->szString,MB_ICONINFORMATION);
	}

	//关闭房间
	if (wType&SMT_CLOSE_ROOM)
	{
		PostMessage(WM_COMMAND,IDM_DELETE_SERVER_ITEM,0);
	}

	return true;
}

//动作消息
bool CPlazaViewServer::OnSocketSubActionMessage(VOID * pData, WORD wDataSize)
{
	//变量定义
	CMD_CM_ActionMessage * pActionMessage=(CMD_CM_ActionMessage *)pData;
	WORD wHeadSize=sizeof(CMD_CM_ActionMessage)-sizeof(pActionMessage->szString);

	//效验参数
	ASSERT((wDataSize>wHeadSize)&&(wDataSize>=(wHeadSize+pActionMessage->wLength*sizeof(WCHAR))));
	if ((wDataSize<=wHeadSize)||(wDataSize<(wHeadSize+pActionMessage->wLength*sizeof(WCHAR)))) return false;

	//关闭处理
	if ((pActionMessage->wType&(SMT_CLOSE_ROOM|SMT_CLOSE_LINK))!=0)
	{
		//关闭提示
		m_DlgStatus.HideStatusWindow();

		//关闭处理
		m_TCPSocketModule->CloseSocket();
		m_ProcessManager.CloseGameProcess(false);

		//状态设置
		m_ServiceStatus=ServiceStatus_NetworkDown;
	}

	//弹出消息
	CDlgInformation Information;
	INT nResultCode=Information.ShowMessageBox(pActionMessage->szString,MB_ICONINFORMATION);

	//变量定义
	WORD wExcursion=wHeadSize+pActionMessage->wLength*sizeof(TCHAR);

	//提取动作
	while (wExcursion<wDataSize)
	{
		//变量定义
		tagActionHead * pActionHead=(tagActionHead *)((BYTE *)pData+wExcursion);

		//效验参数
		ASSERT((wExcursion+sizeof(tagActionHead))<=wDataSize);
		ASSERT((wExcursion+sizeof(tagActionHead)+pActionHead->wAppendSize)<=wDataSize);

		//效验参数
		if ((wExcursion+sizeof(tagActionHead))>wDataSize) return false;
		if ((wExcursion+sizeof(tagActionHead)+pActionHead->wAppendSize)>wDataSize) return false;

		//动作出来
		if (nResultCode==pActionHead->uResponseID)
		{
			switch (pActionHead->cbActionType)
			{
			case ACT_BROWSE:	//浏览动作
				{
					//变量定义
					WORD wDataPos=wExcursion+sizeof(tagActionHead);
					tagActionBrowse * pActionBrowse=(tagActionBrowse *)((BYTE *)pData+wDataPos);

					//I E 浏览
					if (pActionBrowse->cbBrowseType&BRT_IE)
					{
						ShellExecute(NULL,TEXT("OPEN"),pActionBrowse->szBrowseUrl,NULL,NULL,SW_NORMAL);
					}

					//大厅浏览
					if (pActionBrowse->cbBrowseType&BRT_PLAZA)
					{
						CPlatformFrame::GetInstance()->WebBrowse(pActionBrowse->szBrowseUrl,true);
					}

					break;
				}
			case ACT_DOWN_LOAD:	//下载动作
				{
					//变量定义
					WORD wDataPos=wExcursion+sizeof(tagActionHead);
					tagActionDownLoad * pActionDownLoad=(tagActionDownLoad *)((BYTE *)pData+wDataPos);

					//I E 下载
					if (pActionDownLoad->cbDownLoadMode&DLT_IE)
					{
						ShellExecute(NULL,TEXT("OPEN"),pActionDownLoad->szDownLoadUrl,NULL,NULL,SW_NORMAL);
					}

					//下载模块
					if (pActionDownLoad->cbDownLoadMode&DLT_MODULE)
					{
						ShellExecute(NULL,TEXT("OPEN"),pActionDownLoad->szDownLoadUrl,NULL,NULL,SW_NORMAL);
					}

					break;
				}
			}
		}

		//增加偏移
		wExcursion+=(sizeof(tagActionHead)+pActionHead->wAppendSize);
	}

	//关闭房间
	if (pActionMessage->wType&SMT_CLOSE_ROOM) PostMessage(WM_COMMAND,IDM_DELETE_SERVER_ITEM,0);

	return true;
}

//调整控件
VOID CPlazaViewServer::RectifyControl(INT nWidth, INT nHeight)
{
	//状态判断
	if (m_bCreateFlag==false) return;
	if ((nWidth==0)||(nHeight==0)) return;

	//环绕信息
	tagEncircleInfo EncircleInfoTable;
	m_TableEncircle.GetEncircleInfo(EncircleInfoTable);

	//桌子区域
	CRect rcTableArea(0,0,nWidth,nHeight);

	//移动准备
	HDWP hDwp=BeginDeferWindowPos(32);
	UINT uFlags=SWP_NOACTIVATE|SWP_NOCOPYBITS|SWP_NOZORDER;

	//环绕控件
	m_TableEncircle.DeferWindowPos(m_pITableViewFrame->GetViewWnd(),hDwp,rcTableArea);

	//移动结束
	EndDeferWindowPos(hDwp);

	//动画控件
	CRect rcWindow;
	m_pITableViewFrame->GetViewWnd()->GetWindowRect(&rcWindow);
	m_FlashControl.SetPosition(rcWindow.left+(rcWindow.Width()-FALSH_MOVIE_CX)/2,rcWindow.top+(rcWindow.Height()-FALSH_MOVIE_CY)*2/5,FALSH_MOVIE_CX,FALSH_MOVIE_CY,SWP_NOSIZE);
	


	//重画界面
	RedrawWindow(NULL,NULL,RDW_ERASE|RDW_INVALIDATE|RDW_ERASENOW|RDW_UPDATENOW);

	return;
}

//配置房间
VOID CPlazaViewServer::InitServerViewItem(CGameServerItem * pGameServerItem,WORD wAVServerPort,DWORD dwAVServerAddr)
{
	//设置状态
	SetServiceStatus(ServiceStatus_Entering);

	//房间属性
	CGameKindItem * pGameKindItem=pGameServerItem->m_pGameKindItem;
	CopyMemory(&m_GameKind,&pGameKindItem->m_GameKind,sizeof(m_GameKind));
	CopyMemory(&m_GameServer,&pGameServerItem->m_GameServer,sizeof(m_GameServer));
	m_wAVServerPort=wAVServerPort;
	m_dwAVServerAddr=dwAVServerAddr;
	m_szServerPassword[0]=0;

	//关闭判断
	ASSERT(m_GameServer.wServerID!=0);
	if (m_GameServer.wServerID==0) throw TEXT("很抱歉，此游戏房间已经关闭了，不允许继续进入！");

	//密码验证
	if((m_GameServer.wServerKind&SERVER_GENRE_PASSWD)!=0)
	{
		//变量定义
		CDlgInputPassword DlgInputPasswd;
		if(DlgInputPasswd.DoModal()==TRUE)
		{
			DlgInputPasswd.GetServerPasswd(m_szServerPassword);
		}
		else
		{
			//关闭房间
			PostMessage(WM_COMMAND,IDM_DELETE_SERVER_ITEM,0);

			return;
		}		
	}

	//变量定义
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	//创建框架
	if(CreateViewFrame()==false) throw TEXT("桌子视图框架创建失败！");

	//创建组件
	if (m_TCPSocketModule.CreateInstance()==false) throw TEXT("无法创建网络连接组件，游戏房间进入失败！");

	//加载配置
	m_pParameterGame=pParameterGlobal->GetParameterGame(&m_GameKind);
	m_pParameterServer=pParameterGlobal->GetParameterServer(&m_GameServer);

	//设置网络
	m_TCPSocketModule->SetTCPSocketSink(QUERY_ME_INTERFACE(IUnknownEx));
	m_TCPSocketModule->SetProxyServerInfo(pParameterGlobal->m_cbProxyType,pParameterGlobal->m_ProxyServer);

	//发起连接
	if (m_TCPSocketModule->Connect(m_GameServer.szServerAddr,m_GameServer.wServerPort)!=CONNECT_SUCCESS)
	{
		throw TEXT("游戏房间连接失败，请检查系统网络配置参数是否正确！");
		return;
	}

	//创建提示
	m_DlgStatus.ShowStatusWindow(TEXT("正在进入游戏房间，请稍候..."));

	//构造标题
	TCHAR szServerTitle[128]=TEXT("");
	_sntprintf(szServerTitle,CountArray(szServerTitle),TEXT("游戏大厅 > %s > %s"),m_GameKind.szKindName,m_GameServer.szServerName);

	//设置标题
	SetWindowText(szServerTitle);

	return;
}

//设置界面
VOID CPlazaViewServer::InitServerViewCtrl(CSkinRenderManager * pSkinRenderManager)
{
	return;
}

//私聊会话
VOID CPlazaViewServer::WhisperConversation(IClientUserItem * pIClientUserItem[], WORD wUserCount)
{
	//状态过滤
	if (m_ServiceStatus!=ServiceStatus_ServiceIng)
	{
		return;
	}

	//用户属性
	WORD wTableID=m_pIMySelfUserItem->GetTableID();
	BYTE cbMasterOrder=m_pIMySelfUserItem->GetMasterOrder();

	//变量定义
	bool bSameTable=false;
	bool bMasterUser=(cbMasterOrder>0);

	//管理判断
	if (bMasterUser==false)
	{
		for (WORD i=0;i<wUserCount;i++)
		{
			if (pIClientUserItem[i]->GetMasterOrder()>0)
			{
				bMasterUser=true;
				break;
			}
		}
	}

	//同桌判断
	if (wTableID!=INVALID_TABLE)
	{
		for (WORD i=0;i<wUserCount;i++)
		{
			if (pIClientUserItem[i]->GetTableID()==wTableID)
			{
				bSameTable=true;
				break;
			}
		}
	}

	//权限判断
	if (CUserRight::CanWisper(m_dwUserRight)==false)
	{
		CDlgInformation Information(this);
		Information.ShowMessageBox(TEXT("很抱歉，您没有发送私聊信息的权限！"),MB_ICONQUESTION);
		return;
	}

	//私聊权限
	if (CServerRule::IsForfendWisperChat(m_dwServerRule)&&(bMasterUser==false))
	{
		CDlgInformation Information(this);
		Information.ShowMessageBox(TEXT("很抱歉，此游戏房间不允许玩家私聊！"),MB_ICONQUESTION);
		return;
	}

	//变量定义
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	//创建参数
	CWHArray<DWORD> CUserIDArray;
	CUserIDArray.Add(pGlobalUserData->dwUserID);

	//插入用户
	for (WORD i=0;i<wUserCount;i++)
	{
		//用户过虑
		if (pIClientUserItem[i]->GetUserID()==0L) continue;
		if (pIClientUserItem[i]==m_pIMySelfUserItem) continue;

		//插入数组
		CUserIDArray.Add(pIClientUserItem[i]->GetUserID());
	}

	//创建私聊
	WORD wUserIDArray=(WORD)CUserIDArray.GetCount();
	CDlgWhisper * pDlgWhisper=CreateWhisperWnd(0L,m_pIMySelfUserItem->GetUserID(),CUserIDArray.GetData(),wUserIDArray,true);

	//显示窗口
	if (pDlgWhisper!=NULL)
	{
		//显示窗口
		pDlgWhisper->ShowWindow(SW_RESTORE);

		//置顶窗口
		pDlgWhisper->SetActiveWindow();
		pDlgWhisper->BringWindowToTop();

		//删除窗口
		CPlatformFrame * pPlatformFrame=CPlatformFrame::GetInstance();
		if (pPlatformFrame!=NULL) pPlatformFrame->RemoveWhisperItem(pDlgWhisper);
	}

	return;
}

//创建私聊
CDlgWhisper * CPlazaViewServer::CreateWhisperWnd(DWORD dwConversationID,DWORD dwSendUserID,DWORD dwUserID[], WORD wUserCount, bool bCreate)
{
	//寻找现存
	for (INT_PTR i=0;i<m_DlgWhisperItemArray.GetCount();i++)
	{
		//获取窗口
		CDlgWhisper * pDlgWhisper=m_DlgWhisperItemArray[i];
		if ((pDlgWhisper->m_hWnd==NULL)||(pDlgWhisper->CompareUserItem(dwUserID,wUserCount)==false)) continue;

		return pDlgWhisper;
	}

	//废弃判断
	for (INT_PTR i=0;i<m_DlgWhisperItemArray.GetCount();i++)
	{
		//获取窗口
		CDlgWhisper * pDlgWhisper=m_DlgWhisperItemArray[i];
		if ((pDlgWhisper->m_hWnd==NULL)||(pDlgWhisper->DisuseEstimate()==false)) continue;

		//变量定义
		CClientUserItemArray ClientUserItemArray;
		ClientUserItemArray.Add(m_pIMySelfUserItem);

		//构造参数
		for (WORD i=0;i<wUserCount;i++)
		{
			IClientUserItem * pIClientUserItem=m_PlazaUserManagerModule->SearchUserByUserID(dwUserID[i]);
			if ((pIClientUserItem!=NULL)&&(pIClientUserItem!=m_pIMySelfUserItem)) ClientUserItemArray.Add(pIClientUserItem);
		}

		//创建窗口
		ASSERT(ClientUserItemArray.GetCount()==wUserCount);
		WORD wUserItemCount=(WORD)ClientUserItemArray.GetCount();
		pDlgWhisper->CreateWhisperWnd(dwConversationID,dwSendUserID,ClientUserItemArray.GetData(),wUserItemCount,GetDesktopWindow());

		return pDlgWhisper;
	}

	//创建窗口
	if (bCreate==true)
	{
		//变量定义
		CDlgWhisper * pDlgWhisper=NULL;
		CClientUserItemArray ClientUserItemArray;
		ClientUserItemArray.Add(m_pIMySelfUserItem);

		//寻找关闭
		for (INT_PTR i=0;i<m_DlgWhisperItemArray.GetCount();i++)
		{
			//获取窗口
			CDlgWhisper * pDlgWhisperTemp=m_DlgWhisperItemArray[i];
			if (pDlgWhisperTemp->GetSafeHwnd()==NULL) 
			{
				pDlgWhisper=pDlgWhisperTemp;
				break;
			}
		}

		//创建窗口
		if (pDlgWhisper==NULL)
		{
			try
			{
				pDlgWhisper=new CDlgWhisper;
				if (pDlgWhisper==NULL) return NULL;
				m_DlgWhisperItemArray.Add(pDlgWhisper);
			}
			catch (...) { return NULL; }
		}

		//构造参数
		for (WORD i=0;i<wUserCount;i++)
		{
			IClientUserItem * pIClientUserItem=m_PlazaUserManagerModule->SearchUserByUserID(dwUserID[i]);
			if ((pIClientUserItem!=NULL)&&(pIClientUserItem!=m_pIMySelfUserItem)) ClientUserItemArray.Add(pIClientUserItem);
		}

		//设置接口
		m_ChatHistoryManager.SetMyselftUserItem(m_pIMySelfUserItem);

		//用户数目
		ASSERT(ClientUserItemArray.GetCount()==wUserCount);
		WORD wUserItemCount=(WORD)ClientUserItemArray.GetCount();

		//设置窗口
		pDlgWhisper->SetServerViewItem(this);
		pDlgWhisper->SetUserRight(m_dwUserRight);
		pDlgWhisper->SetMasterRight(m_dwMasterRight);
		pDlgWhisper->SetMySelfUserItem(m_pIMySelfUserItem);
		pDlgWhisper->SetChatHistoryManager(QUERY_OBJECT_INTERFACE(m_ChatHistoryManager,IChatHistoryManager));
		pDlgWhisper->SetTCPSocket(m_TCPSocketModule.GetInterface());
		pDlgWhisper->SetGameLevelParser(m_GameLevelParserModule.GetInterface());
		pDlgWhisper->SetUserOrderParser(theApp.m_UserOrderParserModule.GetInterface());
		pDlgWhisper->SetPlazaUserManager(m_PlazaUserManagerModule.GetInterface());

		//创建窗口
		pDlgWhisper->CreateWhisperWnd(dwConversationID,dwSendUserID,ClientUserItemArray.GetData(),wUserItemCount,GetDesktopWindow());

		return pDlgWhisper;
	}

	return NULL;
}

//发送登录
bool CPlazaViewServer::SendLogonPacket(LPCTSTR pszServerPasswd)
{
	//变量定义
	CMD_GR_LogonUserID LogonUserID;
	ZeroMemory(&LogonUserID,sizeof(LogonUserID));

	//变量定义
	CGlobalUnits * pGlobalUnits=CGlobalUnits::GetInstance();
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	//游戏版本
	LPCTSTR pszProcessName=m_GameKind.szProcessName;
	CWHService::GetModuleVersion(pszProcessName,LogonUserID.dwProcessVersion);

	//附加信息
	LogonUserID.dwPlazaVersion=pGlobalUnits->GetPlazaVersion();
	LogonUserID.dwFrameVersion=pGlobalUnits->GetFrameVersion();

	//登录信息
	LogonUserID.wKindID=m_GameServer.wKindID;
	LogonUserID.dwUserID=pGlobalUserData->dwUserID;	
	lstrcpyn(LogonUserID.szServerPasswd,pszServerPasswd,CountArray(LogonUserID.szServerPasswd));
	lstrcpyn(LogonUserID.szPassword,pGlobalUserData->szDynamicPass,CountArray(LogonUserID.szPassword));	

	//机器标识
	CWHService::GetMachineIDEx(LogonUserID.szMachineID);

	//发送数据
	m_TCPSocketModule->SendData(MDM_GR_LOGON,SUB_GR_LOGON_USERID,&LogonUserID,sizeof(LogonUserID));

	return true;
}

//发送配置
bool CPlazaViewServer::SendUserRulePacket()
{
	//变量定义
	BYTE cbBuffer[SOCKET_TCP_PACKET];
	CMD_GR_UserRule * pUserRule=(CMD_GR_UserRule *)cbBuffer;
	CSendPacketHelper SendPacket(cbBuffer+sizeof(CMD_GR_UserRule),sizeof(cbBuffer)-sizeof(CMD_GR_UserRule));

	//变量定义
	CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();

	//构造数据
	pUserRule->cbRuleMask=0x00;

	//限制地址
	if (pParameterGlobal->m_bLimitSameIP==true)
	{
		pUserRule->cbRuleMask|=UR_LIMIT_SAME_IP;
	}

	//限制胜率
	if (m_pParameterGame->m_bLimitWinRate==true)
	{
		pUserRule->cbRuleMask|=UR_LIMIT_WIN_RATE;
		pUserRule->wMinWinRate=m_pParameterGame->m_wMinWinRate;
	}

	//限制逃率
	if (m_pParameterGame->m_bLimitFleeRate==true)
	{
		pUserRule->cbRuleMask|=UR_LIMIT_FLEE_RATE;
		pUserRule->wMaxFleeRate=m_pParameterGame->m_wMaxFleeRate;
	}

	//限制积分
	if (m_pParameterGame->m_bLimitGameScore==true)
	{
		pUserRule->cbRuleMask|=UR_LIMIT_GAME_SCORE;
		pUserRule->lMaxGameScore=(LONG)m_pParameterGame->m_lMaxGameScore;
		pUserRule->lMinGameScore=(LONG)m_pParameterGame->m_lMinGameScore;
	}

	//携带密码
	if ((CServerRule::IsForfendLockTable(m_dwServerRule)==false)&&(m_pParameterServer->m_bTakePassword==true))
	{
		SendPacket.AddPacket(m_pParameterServer->m_szPassword,DTP_GR_TABLE_PASSWORD);
	}

	//发送数据
	WORD wSendSize=sizeof(CMD_GR_UserRule)+SendPacket.GetDataSize();
	m_TCPSocketModule->SendData(MDM_GR_USER,SUB_GR_USER_RULE,cbBuffer,wSendSize);

	return true;
}

//发送旁观
bool CPlazaViewServer::SendLookonPacket(WORD wTableID, WORD wChairID)
{
	//变量定义
	CMD_GR_UserLookon UserLookon;
	ZeroMemory(&UserLookon,sizeof(UserLookon));

	//构造数据
	UserLookon.wTableID=wTableID;
	UserLookon.wChairID=wChairID;

	//发送数据
	m_TCPSocketModule->SendData(MDM_GR_USER,SUB_GR_USER_LOOKON,&UserLookon,sizeof(UserLookon));

	return true;
}

//发送坐下
bool CPlazaViewServer::SendSitDownPacket(WORD wTableID, WORD wChairID, LPCTSTR lpszPassword)
{
	//变量定义
	CMD_GR_UserSitDown UserSitDown;
	ZeroMemory(&UserSitDown,sizeof(UserSitDown));

	//构造数据
	UserSitDown.wTableID=wTableID;
	UserSitDown.wChairID=wChairID;
	if ((lpszPassword!=NULL)&&(lpszPassword[0]!=0))
	{
		lstrcpyn(UserSitDown.szPassword,lpszPassword,CountArray(UserSitDown.szPassword));
	}
	//比赛报名时候
	if((m_wServerType&GAME_GENRE_MATCH)!=0&&m_ProcessManager.IsProcessReady())
	{
		CDlgInformation Information(this);
		Information.ShowMessageBox(TEXT("请先关闭游戏客户端！"));
		return true;
	}
	//发送数据
	m_TCPSocketModule->SendData(MDM_GR_USER,SUB_GR_USER_SITDOWN,&UserSitDown,sizeof(UserSitDown));

	return true;
}

//发送起立
bool CPlazaViewServer::SendStandUpPacket(WORD wTableID, WORD wChairID, BYTE cbForceLeave)
{
	//变量定义
	CMD_GR_UserStandUp UserStandUp;
	ZeroMemory(&UserStandUp,sizeof(UserStandUp));

	//构造数据
	UserStandUp.wTableID=wTableID;
	UserStandUp.wChairID=wChairID;
	UserStandUp.cbForceLeave=cbForceLeave;

	//发送数据
	m_TCPSocketModule->SendData(MDM_GR_USER,SUB_GR_USER_STANDUP,&UserStandUp,sizeof(UserStandUp));

	return true;
}

//发送聊天
bool CPlazaViewServer::SendUserChatPacket(DWORD dwTargetUserID, LPCTSTR pszChatString, COLORREF crColor)
{
	//构造信息
	CMD_GR_C_UserChat UserChat;
	lstrcpyn(UserChat.szChatString,pszChatString,CountArray(UserChat.szChatString));

	//构造数据
	UserChat.dwChatColor=crColor;
	UserChat.dwTargetUserID=dwTargetUserID;
	UserChat.wChatLength=CountStringBuffer(UserChat.szChatString);

	//发送命令
	WORD wHeadSize=sizeof(UserChat)-sizeof(UserChat.szChatString);
	m_TCPSocketModule->SendData(MDM_GR_USER,SUB_GR_USER_CHAT,&UserChat,wHeadSize+UserChat.wChatLength*sizeof(UserChat.szChatString[0]));

	return true;
}

//发送表情
bool CPlazaViewServer::SendExpressionPacket(DWORD dwTargetUserID, WORD wItemIndex)
{
	//变量定义
	CMD_GR_C_UserExpression UserExpression;
	ZeroMemory(&UserExpression,sizeof(UserExpression));

	//构造信息
	UserExpression.wItemIndex=wItemIndex;
	UserExpression.dwTargetUserID=dwTargetUserID;

	//发送命令
	m_TCPSocketModule->SendData(MDM_GR_USER,SUB_GR_USER_EXPRESSION,&UserExpression,sizeof(UserExpression));

	return true;
}

//执行旁观
bool CPlazaViewServer::PerformLookonAction(WORD wTableID, WORD wChairID)
{
	//效验数据
	ASSERT(wTableID!=INVALID_TABLE);
	ASSERT(wChairID!=INVALID_CHAIR);

	//状态过滤
	if (m_ServiceStatus!=ServiceStatus_ServiceIng) return false;
	if ((m_wReqTableID==wTableID)&&(m_wReqChairID==wChairID)) return false;

	//自己状态
	if (m_pIMySelfUserItem->GetUserStatus()>=US_PLAYING)
	{
		//提示消息
		CDlgInformation Information(this);
		Information.ShowMessageBox(m_GameServer.szServerName,TEXT("您正在游戏中，暂时不能离开，请先结束当前游戏！"));

		return false;
	}

	//权限判断
	if (CUserRight::CanLookon(m_dwUserRight)==false)
	{
		//提示消息
		CDlgInformation Information(this);
		Information.ShowMessageBox(m_GameServer.szServerName,TEXT("抱歉，您暂时没有旁观游戏的权限！"));

		return false;
	}

	//清理界面
	if ((m_wReqTableID!=INVALID_TABLE)&&(m_wReqChairID!=INVALID_CHAIR)&&(m_wReqChairID!=INDEX_ENTER_CHAIR))
	{
		IClientUserItem * pIClientUserItem=m_pITableViewFrame->GetClientUserItem(m_wReqTableID,m_wReqChairID);
		if (pIClientUserItem==m_pIMySelfUserItem) m_pITableViewFrame->SetClientUserItem(m_wReqTableID,m_wReqChairID,NULL);
	}

	//设置变量
	m_wReqTableID=wTableID;
	m_wReqChairID=wChairID;
	m_wFindTableID=INVALID_TABLE;

	//设置界面
	m_pITableViewFrame->VisibleTable(wTableID);

	//发送命令
	SendLookonPacket(wTableID,wChairID);

	return true;
}

//执行起立
bool CPlazaViewServer::PerformStandUpAction(WORD wTableID, WORD wChairID)
{
	//效验数据
	ASSERT(wTableID!=INVALID_TABLE);
	ASSERT(wChairID!=INVALID_CHAIR);

	//状态过滤
	if (m_ServiceStatus!=ServiceStatus_ServiceIng) return false;
	if ((m_wReqTableID==wTableID)&&(m_wReqChairID==wChairID)) return false;

	//自己状态
	if (m_pIMySelfUserItem->GetUserStatus()>=US_PLAYING)
	{
		//提示消息
		CDlgInformation Information(this);
		Information.ShowMessageBox(m_GameServer.szServerName,TEXT("您正在游戏中，暂时不能离开，请先结束当前游戏！"));

		return false;
	}

	//设置变量
	m_wReqTableID=INVALID_TABLE;
	m_wReqChairID=INVALID_CHAIR;
	m_wFindTableID=INVALID_TABLE;

	//设置界面
	m_pITableViewFrame->VisibleTable(wTableID);

	//发送命令
	SendStandUpPacket(wTableID,wChairID,FALSE);

	return true;
}

//执行坐下
bool CPlazaViewServer::PerformSitDownAction(WORD wTableID, WORD wChairID, bool bEfficacyPass)
{
	//状态过滤
	if (m_ServiceStatus!=ServiceStatus_ServiceIng) return false;
	if ((m_wReqTableID!=INVALID_TABLE)&&(m_wReqTableID==wTableID)) return false;
	if ((m_wReqChairID!=INVALID_CHAIR)&&(m_wReqChairID==wChairID)) return false;

	//自己状态
	if (m_pIMySelfUserItem->GetUserStatus()>=US_PLAYING)
	{
		//提示消息
		CDlgInformation Information(this);
		Information.ShowMessageBox(m_GameServer.szServerName,TEXT("您正在游戏中，暂时不能离开，请先结束当前游戏！"));

		return false;
	}

	//权限判断
	if (CUserRight::CanPlay(m_dwUserRight)==false)
	{
		//提示消息
		CDlgInformation Information(this);
		Information.ShowMessageBox(m_GameServer.szServerName,TEXT("抱歉，您暂时没有加入游戏的权限！"));

		return false;
	}

	//桌子效验
	if ((wTableID!=INVALID_TABLE)&&(wChairID!=INVALID_CHAIR))
	{
		CString strDescribe;
		if ((wChairID!=INDEX_ENTER_CHAIR)&&(EfficacyTableRule(wTableID,wChairID,false,strDescribe)==false))
		{
			//提示消息
			CDlgInformation Information(this);
			Information.ShowMessageBox(m_GameServer.szServerName,strDescribe,MB_ICONINFORMATION,30L);

			return false;
		}
	}

	//密码判断
	TCHAR szPassword[LEN_PASSWORD] = {0};
	if ((m_pIMySelfUserItem->GetMasterOrder()==0)&&(bEfficacyPass==true)&&(wTableID!=INVALID_TABLE)&&(wChairID!=INVALID_CHAIR)&&(wChairID!=INDEX_ENTER_CHAIR))
	{
		//变量定义
		bool bLocker=m_pITableViewFrame->GetLockerFlag(wTableID);

		//密码判断
		if(bLocker)
		{
			//设置密码
			CDlgTablePassword DlgTablePassword;
			if (DlgTablePassword.DoModal()!=IDOK) return false;

			//设置密码
			lstrcpyn(szPassword,DlgTablePassword.m_szPassword,CountArray(szPassword));
		}
	}

	//清理界面
	if ((m_wReqTableID!=INVALID_TABLE)&&(m_wReqChairID!=INVALID_CHAIR)&&(m_wReqChairID!=INDEX_ENTER_CHAIR))
	{
		IClientUserItem * pIClientUserItem=m_pITableViewFrame->GetClientUserItem(m_wReqTableID,m_wReqChairID);
		if (pIClientUserItem==m_pIMySelfUserItem) m_pITableViewFrame->SetClientUserItem(m_wReqTableID,m_wReqChairID,NULL);
	}

	//设置界面
	if ((wChairID!=INDEX_ENTER_CHAIR)&&(wTableID!=INVALID_TABLE)&&(wChairID!=INVALID_CHAIR))
	{
		m_pITableViewFrame->VisibleTable(wTableID);
		m_pITableViewFrame->SetClientUserItem(wTableID,wChairID,m_pIMySelfUserItem);
	}

	//设置变量
	m_wReqTableID=wTableID;
	m_wReqChairID=wChairID;
	m_wFindTableID=INVALID_TABLE;

	//发送命令
	SendSitDownPacket(wTableID,wChairID,szPassword);

	return true;
}

bool CPlazaViewServer::PerformBuyProperty(BYTE cbRequestArea,LPCTSTR pszNickName, WORD wItemCount, WORD wPropertyIndex)
{
	//变量定义
	ASSERT(CGamePropertyManager::GetInstance()!=NULL);
	CGamePropertyManager * pGamePropertyManager=CGamePropertyManager::GetInstance();

	//查找道具
	ASSERT(pGamePropertyManager->GetPropertyItem(wPropertyIndex)!=NULL);
	CGamePropertyBase * pGamePropertyItem=pGamePropertyManager->GetPropertyItem(wPropertyIndex);
	if(pGamePropertyItem==NULL) return false;

	//查找用户
	IClientUserItem * pIClientUserItem=m_PlazaUserManagerModule->SearchUserByNickName(pszNickName);

	//用户判断
	if (pIClientUserItem==NULL)
	{
		return false;
	}

	////变量定义
	//CMD_GR_C_PropertyBuy PropertyBuy;
	//ZeroMemory(&PropertyBuy,sizeof(PropertyBuy));

	////设置变量
	//PropertyBuy.cbRequestArea=cbRequestArea;
	//PropertyBuy.wItemCount=wItemCount;
	//PropertyBuy.wPropertyIndex=wPropertyIndex;
	//PropertyBuy.dwTargetUserID=pIClientUserItem->GetUserID();
	//PropertyBuy.cbConsumeScore=FALSE;

	//发送消息
	//m_TCPSocketModule->SendData(MDM_GR_USER,SUB_GR_PROPERTY_BUY,&PropertyBuy,sizeof(PropertyBuy));

	return true;
}

//设置状态
VOID CPlazaViewServer::SetServiceStatus(enServiceStatus ServiceStatus)
{
	//设置变量
	m_ServiceStatus=ServiceStatus;

	return;
}

//资源目录
VOID CPlazaViewServer::GetGameResDirectory(TCHAR szResDirectory[], WORD wBufferCount)
{
	//变量定义
	WORD wStringIndex=0;

	//构造目录
	ZeroMemory(szResDirectory,wBufferCount*sizeof(TCHAR));
	lstrcpyn(szResDirectory,m_GameKind.szProcessName,wBufferCount);
	while ((szResDirectory[wStringIndex]!=0)&&(szResDirectory[wStringIndex]!=TEXT('.'))) wStringIndex++;

	//字符终止
	szResDirectory[wStringIndex]=0;

	return;
}

//加入按钮
VOID CPlazaViewServer::OnTableButton1Clicked()
{
	//动作过虑
	if (m_ServiceStatus!=ServiceStatus_ServiceIng) return;
	if ((m_wReqTableID!=INVALID_TABLE)&&(m_wReqChairID!=INVALID_CHAIR)) return;

	//状态判断
	if (m_pIMySelfUserItem->GetUserStatus()>=US_PLAYING)
	{
		CDlgInformation Information(this);
		Information.ShowMessageBox(TEXT("您正在游戏中，暂时不能离开，请先结束当前游戏！"),MB_ICONINFORMATION,30L);
		return;
	}

	//随机座位
	if (CServerRule::IsAllowAvertCheatMode(m_dwServerRule)==true)
	{
		PerformSitDownAction(INVALID_TABLE,INVALID_CHAIR,false);
		return;
	}

	//变量定义
	ASSERT(CParameterGlobal::GetInstance()!=NULL);
	CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();

	//执行动作
	switch (pParameterGlobal->m_cbActionHitAutoJoin)
	{
	case ACTION_SEARCH_TABLE:		//查找位置
		{
			//变量定义
			tagSearchTable SearchTable;
			ZeroMemory(&SearchTable,sizeof(SearchTable));

			//搜索条件
			SearchTable.bNotFull=true;
			SearchTable.bOneNull=true;
			SearchTable.bAllNull=false;
			SearchTable.bFilterPass=true;
			SearchTable.bTwoNull=(m_pITableViewFrame->GetChairCount()!=2);

			//搜索结果
			SearchTable.wResultTableID=INVALID_TABLE;
			SearchTable.wResultChairID=INVALID_CHAIR;
			SearchTable.wStartTableID=m_wFindTableID+1;

			//搜索桌子
			SearchGameTable(SearchTable);
			m_wFindTableID=SearchTable.wResultTableID;

			//搜索桌子
			if (m_wFindTableID==INVALID_CHAIR)
			{
				//搜索条件
				SearchTable.bAllNull=true;
				SearchTable.bNotFull=true;
				SearchTable.bOneNull=true;
				SearchTable.bFilterPass=true;
				SearchTable.bTwoNull=(m_pITableViewFrame->GetChairCount()!=2);

				//搜索结果
				SearchTable.wResultTableID=INVALID_TABLE;
				SearchTable.wResultChairID=INVALID_CHAIR;
				SearchTable.wStartTableID=m_wFindTableID+1;

				//搜索桌子
				SearchGameTable(SearchTable);
				m_wFindTableID=SearchTable.wResultTableID;
			}

			//结果判断
			if (m_wFindTableID!=INVALID_CHAIR)
			{
				//效验数据
				ASSERT(SearchTable.wResultTableID!=INVALID_TABLE);
				ASSERT(SearchTable.wResultChairID!=INVALID_CHAIR);

				//设置数据
				WORD wChairID=SearchTable.wResultChairID;
				m_pITableViewFrame->VisibleTable(m_wFindTableID);
				m_pITableViewFrame->FlashGameChair(m_wFindTableID,wChairID);

				//自动坐下
				CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();
				if (pParameterGlobal->m_bAutoSitDown==true) PerformSitDownAction(m_wFindTableID,wChairID,true);
			}
			else
			{
				//提示消息
				CDlgInformation Information(this);
				Information.ShowMessageBox(TEXT("抱歉，现在暂时没有可以加入的游戏桌，请稍后再次尝试！"),MB_ICONINFORMATION,30L);
			}

			break;
		}
	case ACTION_SHOW_SEARCH_DLG:	//查找桌子
		{
			//发送命令
			PostMessage(WM_COMMAND,IDC_BT_TABLE_BUTTON_2,0L);

			break;
		}
	}

	return;
}

//查找按钮
VOID CPlazaViewServer::OnTableButton2Clicked()
{
	//创建对象
	if (m_pDlgSearchTable==NULL)
	{
		//创建对象
		try
		{
			m_pDlgSearchTable=new CDlgSearchTable;
			if (m_pDlgSearchTable==NULL) return;
		}
		catch (...)
		{
			ASSERT(FALSE);
			return;
		}

		//设置对象
		m_pDlgSearchTable->SetServerViewItem(this);
		m_pDlgSearchTable->SetTableViewFrame(m_pITableViewFrame);
	}

	//创建窗口
	if (m_pDlgSearchTable->m_hWnd==NULL)
	{
		m_pDlgSearchTable->Create(IDD_DLG_POPUP,this);
	}

	//显示窗口
	m_pDlgSearchTable->ShowWindow(SW_SHOW);
	m_pDlgSearchTable->SetForegroundWindow();

	return;
}

//加密按钮
VOID CPlazaViewServer::OnTableButton3Clicked()
{
	////变量定义
	//CDlgTablePassword DlgTablePassword;

	////设置提示
	//if(m_pParameterServer->m_bTakePassword)
	//{
	//	lstrcpyn(DlgTablePassword.m_szPassword,m_pParameterServer->m_szPassword,CountArray(DlgTablePassword.m_szPassword));
	//	DlgTablePassword.SetPromptString(TEXT("该桌已设置进入密码，如要更改，请输入密码！如要解锁，请清空密码。"));
	//}
	//else
	//{
	//	DlgTablePassword.SetPromptString(TEXT("请输入桌子进入密码。"));
	//}

	////显示窗口
	//if (DlgTablePassword.DoModal()==IDOK)
	//{
	//	CString strTablePass = DlgTablePassword.m_szPassword;
	//	if(strTablePass.GetLength() > 0)
	//	{
	//		//设置密码
	//		m_pParameterServer->m_bTakePassword=true;
	//		lstrcpyn(m_pParameterServer->m_szPassword,DlgTablePassword.m_szPassword,CountArray(m_pParameterServer->m_szPassword));
	//	}
	//	else
	//	{
	//		//设置密码
	//		m_pParameterServer->m_bTakePassword=false;
	//		ZeroMemory(m_pParameterServer->m_szPassword,CountArray(m_pParameterServer->m_szPassword));
	//	}

	//	//发送规则
	//	SendUserRulePacket();
	//}

	return;
}

//设置按钮
VOID CPlazaViewServer::OnTableButton4Clicked()
{
	//变量定义
	CDlgCollocateRoom DlgCollocateRoom;
	DlgCollocateRoom.InitCollocate(m_pParameterGame, m_pParameterServer);

	//显示窗口
	if (DlgCollocateRoom.DoModal()==IDOK)
	{
		//发送规则
		SendUserRulePacket();
	}

	return;
}

//更多按钮
VOID CPlazaViewServer::OnTableButton5Clicked()
{
	//构造菜单
	CSkinMenu FunctionMenu;
	FunctionMenu.CreateMenu();

	//更多菜单
	FunctionMenu.AppendMenu(IDM_GAME_RULE,TEXT("规则介绍"));
	FunctionMenu.AppendMenu(IDM_GAME_CHART,TEXT("游戏排行榜"));

	//插入菜单
	if (m_pIMySelfUserItem->GetMasterOrder()>0)
	{
		//插入分割
		FunctionMenu.AppendSeparator();

		//插入菜单
		FunctionMenu.AppendMenu(IDM_MANAGER_USER,TEXT("游戏玩家管理..."),MF_DISABLED);
		FunctionMenu.AppendMenu(IDM_MANAGER_SERVER,TEXT("游戏房间管理..."),MF_DISABLED);
		FunctionMenu.AppendMenu(IDM_MANAGER_MESSAGE,TEXT("系统消息管理..."),MF_DISABLED);

		//设置菜单
		if (CMasterRight::CanKillUser(m_dwMasterRight)==true)
		{
			FunctionMenu.EnableMenuItem(IDM_MANAGER_USER,MF_ENABLED);
		}

		//设置菜单
		if (CMasterRight::CanManagerServer(m_dwMasterRight)==true)
		{
			FunctionMenu.EnableMenuItem(IDM_MANAGER_SERVER,MF_ENABLED);
		}

		//设置菜单
		if (CMasterRight::CanIssueMessage(m_dwMasterRight)==true)
		{
			FunctionMenu.EnableMenuItem(IDM_MANAGER_MESSAGE,MF_ENABLED);
		}
	}

	//弹出菜单	
	CControlUI * pButtonControl = GetControlByName(szButtonTableButton5ControlName);
	if(pButtonControl!=NULL)
	{
		CRect rcButton=pButtonControl->GetPos();
		ClientToScreen(&rcButton);
		FunctionMenu.TrackPopupMenu(rcButton.left,rcButton.bottom,this);
	}

	return;
}

//创建视图框架
bool CPlazaViewServer::CreateViewFrame()
{
	try
	{
		//比赛类型
		if(m_GameServer.wServerType&GAME_GENRE_MATCH)
		{
			CMatchViewFrame * pMatchViewFrame=new CMatchViewFrame;
			if(pMatchViewFrame!=NULL)
			{
				m_pITableViewFrame = QUERY_OBJECT_PTR_INTERFACE(pMatchViewFrame,ITableViewFrame);
			}
		}
		else
		{
			CTableViewFrame * pTableViewFrame=new CTableViewFrame;
			if(pTableViewFrame!=NULL)
			{
				m_pITableViewFrame = QUERY_OBJECT_PTR_INTERFACE(pTableViewFrame,ITableViewFrame);
			}
		}

		//创建桌子
		m_pITableViewFrame->CreateTableFrame(this,IDC_SERVER_TABLE,QUERY_ME_INTERFACE(IUnknownEx));

		return true;
	}
	catch(...)
	{
		ASSERT(FALSE);
	}

	return false;
}

//桌子效验
bool CPlazaViewServer::EfficacyTableRule(WORD wTableID, WORD wChairID, bool bReqLookon, CString & strDescribe)
{
	//效验参数
	ASSERT(wTableID<m_pITableViewFrame->GetTableCount());
	ASSERT(wChairID<m_pITableViewFrame->GetChairCount());

	//状态过滤
	if (m_ServiceStatus!=ServiceStatus_ServiceIng) return false;
	if (wTableID>=m_pITableViewFrame->GetTableCount()) return false;
	if (wChairID>=m_pITableViewFrame->GetChairCount()) return false;

	//变量定义
	ASSERT(CParameterGlobal::GetInstance()!=NULL);
	CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();

	//变量定义
	ITableView * pITableView=m_pITableViewFrame->GetTableViewItem(wTableID);
	IClientUserItem * pITableUserItem=pITableView->GetClientUserItem(wChairID);

	//变量定义
	bool bGameStart=pITableView->GetPlayFlag();
	bool bDynamicJoin=CServerRule::IsAllowDynamicJoin(m_dwServerRule);

	//游戏状态
	if ((bGameStart==true)&&(bDynamicJoin==false)&&(bReqLookon==false))
	{
		strDescribe=TEXT("此游戏桌已经开始游戏了，暂时不能加入！");
		return false;
	}

	//其他判断
	if ((bReqLookon==false)&&(m_pIMySelfUserItem->GetMasterOrder()==0))
	{
		//规则判断
		for (WORD i=0;i<m_pITableViewFrame->GetChairCount();i++)
		{
			//获取用户
			IClientUserItem * pITableUserItem=pITableView->GetClientUserItem(i);
			if ((pITableUserItem==NULL)||(pITableUserItem==m_pIMySelfUserItem)) continue;

			//厌恶玩家
			if (pParameterGlobal->m_bLimitDetest==true)
			{
				if (pITableUserItem->GetUserCompanion()==CP_DETEST)
				{
					//设置提示
					LPCTSTR pszNickName=pITableUserItem->GetNickName();
					strDescribe.Format(TEXT("您设置了不与不受欢迎的玩家游戏，此桌有您不欢迎的玩家 [ %s ] ！"),pszNickName);

					return false;
				}
			}

			//胜率效验
			if (m_pParameterGame->m_bLimitWinRate==true)
			{
				WORD wWinRate = (WORD)(pITableUserItem->GetUserWinRate()*100L);
				if (wWinRate < m_pParameterGame->m_wMinWinRate)
				{
					strDescribe.Format(TEXT("[ %s ] 的胜率太低了，与您的设置不符！"),pITableUserItem->GetNickName());
					return false;
				}
			}

			//逃率效验
			if (m_pParameterGame->m_bLimitFleeRate)
			{
				WORD wFleeRate = (WORD)(pITableUserItem->GetUserFleeRate()*100L);
				if (wFleeRate > m_pParameterGame->m_wMaxFleeRate)
				{
					strDescribe.Format(TEXT("[ %s ] 的逃率太高了，与您的设置不符！"),pITableUserItem->GetNickName());
					return false;
				}
			}

			//积分效验
			if (m_pParameterGame->m_bLimitGameScore)
			{
				//最高积分
				if (pITableUserItem->GetUserScore()>m_pParameterGame->m_lMaxGameScore)
				{
					strDescribe.Format(TEXT("[ %s ] 的积分太高了，与您的设置不符！"),pITableUserItem->GetNickName());
					return false;
				}

				//最低积分
				if (pITableUserItem->GetUserScore()<m_pParameterGame->m_lMinGameScore)
				{
					strDescribe.Format(TEXT("[ %s ] 的积分太低了，与您的设置不符！"),pITableUserItem->GetNickName());
					return false;
				}
			}
		}
	}

	return true;
}

//房间管理
VOID CPlazaViewServer::OnManageServer()
{
	//权限判断
	if (CMasterRight::CanManagerServer(m_dwMasterRight)==false) return;

	//变量定义
	CDlgServerManager DlgServerManager;
	DlgServerManager.Initialization(m_TCPSocketModule.GetInterface(),m_pIMySelfUserItem);

	//设置变量
	m_pDlgServerManager=&DlgServerManager;

	//显示窗口
	DlgServerManager.DoModal();

	//设置变量
	m_pDlgServerManager=NULL;

	return;
}

//用户管理
VOID CPlazaViewServer::OnManageUser()
{
	//权限判断
	if (CMasterRight::CanKillUser(m_dwMasterRight)==false) return;

	//显示窗口
	if (m_pDlgManageUser==NULL) 
	{
		m_pDlgManageUser=new CDlgManageUser;
	}

	if( m_pDlgManageUser )
		m_pDlgManageUser->SetMasterRight(m_dwMasterRight);

	//窗口窗口
	if (m_pDlgManageUser->m_hWnd==NULL)
	{
		m_pDlgManageUser->Create(IDD_DLG_MANAGE_USER,this);
		m_pDlgManageUser->SetTCPSocket(QUERY_OBJECT_PTR_INTERFACE(m_TCPSocketModule.GetInterface(),IUnknownEx));
	}

	//显示窗口
	m_pDlgManageUser->ShowWindow(SW_SHOW);
	m_pDlgManageUser->BringWindowToTop();

	return;
}

//系统消息管理
VOID CPlazaViewServer::OnManageSystemMsg()
{
	//权限判断
	if (CMasterRight::CanIssueMessage(m_dwMasterRight)==false) return;

	CDlgIssueMessage DlgIssueMess;
	if(IDOK == DlgIssueMess.DoModal())
	{
		//构造消息
		CMD_GR_SendMessage SendMessage = {};

		if( DlgIssueMess.m_bSendALLRoom )
			SendMessage.cbAllRoom = TRUE;
		if( DlgIssueMess.m_bSendRoom )
			SendMessage.cbRoom = TRUE;
		if( DlgIssueMess.m_bSendGame )
			SendMessage.cbGame = TRUE;
		if( DlgIssueMess.m_bLoop )
		{
			SendMessage.cbLoop = TRUE;
			SendMessage.dwTimeRate = DlgIssueMess.m_dwTimeRate;
			SendMessage.tConcludeTime = DlgIssueMess.tConcludeTime;
		}
		lstrcpyn(SendMessage.szSystemMessage,DlgIssueMess.m_strMessage,CountArray(SendMessage.szSystemMessage));
		SendMessage.wChatLength = lstrlen(SendMessage.szSystemMessage)+1;

		WORD wSendSize = sizeof(SendMessage)-sizeof(SendMessage.szSystemMessage)+CountStringBuffer(SendMessage.szSystemMessage);
		m_TCPSocketModule->SendData(MDM_GR_MANAGE,SUB_GR_SEND_MESSAGE,&SendMessage,wSendSize);
	}

	return;
}

//销毁消息
VOID CPlazaViewServer::OnNcDestroy()
{
	__super::OnNcDestroy();

	//界面变量
	m_bCreateFlag=false;

	//用户属性
	m_dwUserRight=0L;
	m_dwMasterRight=0L;
	m_pIMySelfUserItem=NULL;

	//房间属性
	m_wServerType=0L;
	m_dwServerRule=0L;

	//配置信息
	m_cbColumnCount=0;
	ZeroMemory(m_ColumnItem,sizeof(m_ColumnItem));

	//房间属性
	ZeroMemory(&m_GameKind,sizeof(m_GameKind));
	ZeroMemory(&m_GameServer,sizeof(m_GameServer));

	//销毁控件
	m_ToolTipCtrl.DestroyWindow();	

	//销毁窗口
	if(m_pITableViewFrame!=NULL) 
	{
		m_pITableViewFrame->GetViewWnd()->DestroyWindow();
	}
    


	//结束对话框
	if(m_pDlgInsureServer!=NULL) m_pDlgInsureServer->EndDialog(IDOK);
	if(m_pDlgTaskServerCenter!=NULL) m_pDlgTaskServerCenter->EndDialog(IDOK);
	if(m_pDlgCheckIn!=NULL) m_pDlgCheckIn->EndDialog(IDOK);	
	if(m_pDlgBaseEnsure!=NULL) m_pDlgBaseEnsure->EndDialog(IDOK);
	if(m_pDlgBagServer!=NULL) m_pDlgBagServer->EndDialog();
	if(m_pDlgShopServer!=NULL) m_pDlgShopServer->EndDialog();

	//销毁窗口
	if(m_pDlgSearchTable && IsWindow(m_pDlgSearchTable->m_hWnd)) m_pDlgSearchTable->DestroyWindow();
	if(m_pDlgManageUser && IsWindow(m_pDlgManageUser->m_hWnd)) m_pDlgManageUser->DestroyWindow();

	//删除对象
	SafeDelete(m_pDlgSearchTable);
	SafeDelete(m_pDlgManageUser);

	//释放接口
	SafeRelease(m_pITableViewFrame);

	//删除私聊
	for (INT_PTR i=0;i<m_DlgWhisperItemArray.GetCount();i++)
	{
		//获取子项
		CDlgWhisper * pDlgWhisper=m_DlgWhisperItemArray[i];

		//删除私聊
		CPlatformFrame::GetInstance()->RemoveWhisperItem(pDlgWhisper);

		//关闭窗口
		pDlgWhisper->DestroyWindow();

		//清理内存
		SafeDelete(pDlgWhisper);
	}

	//移除元素
	m_DlgWhisperItemArray.RemoveAll();
}

//时钟消息
VOID CPlazaViewServer::OnTimer(UINT nIDEvent)
{
	__super::OnTimer(nIDEvent);

	//显示向导
	if(nIDEvent==IDI_GUIDE_TASK)
	{
		//关闭定时器
		KillTimer(nIDEvent);



		//闪烁窗口
		CPlatformFrame::GetInstance()->FlashWindow(true);

		//调整控件
		CRect rcClient;
		GetClientRect(&rcClient);		
		RectifyControl(rcClient.Width(),rcClient.Height());

		return;
	}
}


//位置消息
VOID CPlazaViewServer::OnSize(UINT nType, INT cx, INT cy)
{
	__super::OnSize(nType, cx, cy);

	//调整控件
	RectifyControl(cx,cy);

	return;
}


//加为好友
LRESULT CPlazaViewServer::OnWhisperAddFriend(WPARAM wParam, LPARAM lPara)
{
	//变量定义
	IClientUserItem * pIClientUserItem=(IClientUserItem*)wParam;

	//效验参数
	ASSERT(pIClientUserItem!=NULL);
	if(pIClientUserItem==NULL) return 0;

	//变量定义
	ASSERT(CUserInformation::GetInstance()!=NULL);
	CUserInformation * pUserInformation=CUserInformation::GetInstance();
	BYTE cbSourCompanion=pIClientUserItem->GetUserCompanion();
	BYTE cbDestCompanion=(cbSourCompanion==CP_FRIEND)?CP_NORMAL:CP_FRIEND;

	//辅助变量
	DWORD dwMeGroupID=m_pIMySelfUserItem->GetGroupID();
	bool bSameGroup=((dwMeGroupID!=0L)&&(pIClientUserItem->GetGroupID()==dwMeGroupID));

	//设置关系
	pIClientUserItem->SetUserCompanion(cbDestCompanion);
	pUserInformation->InsertCompanionInfo(pIClientUserItem,cbDestCompanion);

	//变量定义
	tagUserAttrib UserAttrib;
	UserAttrib.cbCompanion=cbDestCompanion;
	(GetPlazaUserManager())->UpdateUserItemAttrib(pIClientUserItem,&UserAttrib);

	////更新列表
	//m_UserListControl.UpdateDataItem(pIClientUserItem);
	//if (cbDestCompanion==CP_FRIEND) m_UserListCustom.InsertUserItem(pIClientUserItem);
	//if ((bSameGroup==false)&&(cbDestCompanion!=CP_FRIEND)) m_UserListCustom.DeleteUserItem(pIClientUserItem);

	return 0;
}

//设为厌恶
LRESULT CPlazaViewServer::OnWhisperSetDetest(WPARAM wParam, LPARAM lPara)
{
	//变量定义
	IClientUserItem * pIClientUserItem=(IClientUserItem*)wParam;

	//效验参数
	ASSERT(pIClientUserItem!=NULL);
	if(pIClientUserItem==NULL) return 0;

	//变量定义
	ASSERT(CUserInformation::GetInstance()!=NULL);
	CUserInformation * pUserInformation=CUserInformation::GetInstance();
	BYTE cbSourCompanion=pIClientUserItem->GetUserCompanion();
	BYTE cbDestCompanion=(cbSourCompanion==CP_DETEST)?CP_NORMAL:CP_DETEST;

	//辅助变量
	DWORD dwMeGroupID=m_pIMySelfUserItem->GetGroupID();
	bool bSameGroup=((dwMeGroupID!=0L)&&(pIClientUserItem->GetGroupID()==dwMeGroupID));

	//设置关系
	pIClientUserItem->SetUserCompanion(cbDestCompanion);
	pUserInformation->InsertCompanionInfo(pIClientUserItem,cbDestCompanion);

	//变量定义
	tagUserAttrib UserAttrib;
	UserAttrib.cbCompanion=cbDestCompanion;
	(GetPlazaUserManager())->UpdateUserItemAttrib(pIClientUserItem,&UserAttrib);

	////更新列表
	//m_UserListControl.UpdateDataItem(pIClientUserItem);
	//if (bSameGroup==true) m_UserListCustom.UpdateUserItem(pIClientUserItem);
	//if ((bSameGroup==false)&&(cbDestCompanion!=CP_FRIEND)) m_UserListCustom.DeleteUserItem(pIClientUserItem);

	return 0;
}
//////////////////////////////////////////////////////////////////////////////////