#ifndef PLAZA_VIEW_SERVER_HEAD_FILE
#define PLAZA_VIEW_SERVER_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "DlgStatus.h"
#include "DlgWhisper.h"
#include "ServerListData.h"
#include "ProcessManager.h"
#include "DlgCollocateRoom.h"
#include "DlgSearchTable.h"
#include "DlgServerManager.h"
#include "DlgTaskCenter.h"
#include "DlgExchange.h"
#include "DlgManageUser.h"
#include "PlatformPublicize.h"
#include "ChatHistory.h"
#include "TableViewFrame.h"
#include "PlazaViewServerCtrl.h"
#include "DlgShop.h"
#include "DlgBag.h"
#include "DlgPropertyUse.h"
#include "DlgInsure.h"
#include "DlgCheckIn.h"
#include "DlgBaseEnsure.h"
#include "DlgTask.h"
//////////////////////////////////////////////////////////////////////////

//加为好友
#define WM_ADD_FRIEND				(WM_USER+400)						//加为好友
#define WM_SET_DETEST				(WM_USER+401)						//设为厌恶

//////////////////////////////////////////////////////////////////////////////////

//服务状态
enum enServiceStatus
{
	ServiceStatus_Unknow,			//未知状态
	ServiceStatus_Entering,			//进入状态
	ServiceStatus_Validate,			//验证状态
	ServiceStatus_RecvInfo,			//读取状态
	ServiceStatus_ServiceIng,		//服务状态
	ServiceStatus_NetworkDown,		//中断状态
};

//查找桌子
struct tagSearchTable
{
	bool							bOneNull;							//一个空位
	bool							bTwoNull;							//两个空位
	bool							bAllNull;							//全空位置
	bool							bNotFull;							//不全满位
	bool							bFilterPass;						//过滤密码
	WORD							wStartTableID;						//起始桌子
	WORD							wResultTableID;						//结果桌子
	WORD							wResultChairID;						//结果椅子
};

//用户数组
typedef CWHArray<IClientUserItem *> CClientUserItemArray;

//////////////////////////////////////////////////////////////////////////////////

//房间视图
class CPlazaViewServer :  public CFGuiDialog, public IServerViewItem, public ITCPSocketSink,public ITableViewFrameSink, public IUserManagerSink, public IProcessManagerSink,public IGamePropertySink, public IStatusViewSink, public IFlashControlSink,public IGamePropertyUseSink
{
	
	//界面变量
protected:
	bool							m_bCreateFlag;						//创建标志

	//辅助变量
protected:
	WORD							m_wReqTableID;						//请求桌子
	WORD							m_wReqChairID;						//请求位置
	WORD							m_wFindTableID;						//查找桌子
	WORD							m_wInvitedTableID;					//被邀桌子
	bool							m_bUserInRoom;						//是否在房间

	//聊天变量
protected:
	DWORD							m_dwChatTime;						//聊天时间
	CString							m_strChatString;					//发送信息

	//用户属性
protected:
	DWORD							m_dwUserRight;						//用户权限
	DWORD							m_dwMasterRight;					//管理权限
	IClientUserItem *				m_pIMySelfUserItem;					//自己指针
	
	//房间属性
protected:
	WORD							m_wServerType;						//房间类型
	DWORD							m_dwServerRule;						//房间规则
	enServiceStatus					m_ServiceStatus;					//房间状态
	WORD							m_wAVServerPort;					//视频端口
	DWORD							m_dwAVServerAddr;					//视频地址
	TCHAR							m_szServerPassword[LEN_PASSWORD];	//房间密码

	//配置信息
protected:
	BYTE							m_cbColumnCount;					//列表数目
	tagColumnItem					m_ColumnItem[MAX_COLUMN];			//列表描述

	//房间属性
protected:
	WORD							m_wChairCount;						//椅子数量
	tagGameKind						m_GameKind;							//类型信息
	tagGameServer					m_GameServer;						//房间信息

	//配置参数
protected:
	CParameterGame *				m_pParameterGame;					//游戏配置
	CParameterServer *				m_pParameterServer;					//房间配置

	//数组变量
protected:
	CClientUserItemArray			m_MenuUserItemArray;				//用户数组

	//功能组件
protected:
	CDlgStatus						m_DlgStatus;						//状态窗口	
	CFlashControl					m_FlashControl;						//动画控件
	CProcessManager					m_ProcessManager;					//进程管理
	CDlgWhisperItemArray			m_DlgWhisperItemArray;				//私聊数组	
	CChatHistoryManager				m_ChatHistoryManager;				//聊天记录管理

	//控件变量
protected:
	CDlgSearchTable *				m_pDlgSearchTable;					//查找桌子	
	CDlgInsureServerNew *			m_pDlgInsureServer;					//银行对象	
	CDlgServerManager *				m_pDlgServerManager;				//管理对象
	CDlgExchangeServer *			m_pDlgExchangeServer;				//房间兑换
	CPlazaViewServerCtrl *			m_pPlazaViewServerCtrl;				//框架控件
	CDlgTaskServer *				m_pDlgTaskServerCenter;				//任务中心	
	CDlgShopServer*					m_pDlgShopServer;					//道具商城
	CDlgBagServer*					m_pDlgBagServer;					//我的背包
	CDlgPropertyUseServer*			m_pDlgPropertyUse;					//道具使用
	CDlgCheckInServer*				m_pDlgCheckIn;                      //签到对象
	CDlgBaseEnsureServer*           m_pDlgBaseEnsure;                   //低保对象

	//组件变量
protected:
	CTCPSocketHelper				m_TCPSocketModule;					//网络连接
	CGameLevelParserHelper			m_GameLevelParserModule;			//游戏等级
	CPlazaUserManagerHelper			m_PlazaUserManagerModule;			//用户管理

	//界面组件
public:
	CSkinButton						m_btControl;						//控制按钮
	CToolTipCtrl					m_ToolTipCtrl;						//提示控件

	//接口变量
public:
	ITableViewFrame *				m_pITableViewFrame;					//桌子框架

	//资源变量
protected:
	CEncircleBMP					m_TableEncircle;					//桌子资源
	CEncircleBMP					m_MainEncircle;						//主窗口资源
	CPngImage						m_ImageGameName;					//游戏名称

	//管理控件
public:
	CDlgManageUser *				m_pDlgManageUser;					//用户管理

public:
	bool							m_bPlaying;
	//函数定义
public:
	//构造函数
	CPlazaViewServer();
	//析构函数
	virtual ~CPlazaViewServer();

	//基础接口
public:
	//释放对象
	virtual VOID Release() { delete this; }
	//接口查询
	virtual VOID * QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	//接口函数
public:
	//消息提醒
	virtual void Notify(TNotifyUI &  msg);
	
	//重载函数
public:
	//皮肤名称
	virtual LPCTSTR GetSkinFile() { return TEXT("ServerView.xml"); } 

	//绘画接口
public:
	//开始绘画
	virtual void OnBeginPaintWindow(HDC hDC);

	//重载函数
protected:
	//控件绑定
	virtual VOID DoDataExchange(CDataExchange * pDX);
	//消息过虑
	virtual BOOL PreTranslateMessage(MSG * pMsg);
	//创建函数
	virtual BOOL OnInitDialog();
	//命令函数
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);

	//桌子接口
public:
	//鼠标双击
	virtual VOID OnDButtonHitTable(WORD wTableID);
	//左键按下
	virtual VOID OnLButtonHitTable(WORD wTableID, WORD wChairID);
	//右键按下
	virtual VOID OnRButtonHitTable(WORD wTableID, WORD wChairID);
	//桌子按钮
	virtual VOID OnHitTableButton(BYTE cbButton);

	//网络接口
protected:
	//连接事件
	virtual bool OnEventTCPSocketLink(WORD wSocketID, INT nErrorCode);
	//关闭事件
	virtual bool OnEventTCPSocketShut(WORD wSocketID, BYTE cbShutReason);
	//读取事件
	virtual bool OnEventTCPSocketRead(WORD wSocketID, TCP_Command Command, VOID * pData, WORD wDataSize);

	//道具接口
protected:
	//播放完成
	virtual VOID OnEventFlashFinish();
	//购前检查
	virtual bool OnEventBuyPropertyPrep(LPCTSTR pszNickName,WORD wPropertyIndex,LPTSTR pszMessage);
	//购买道具
	virtual bool OnEventBuyProperty(LPCTSTR pszNickName, WORD wItemCount, WORD wPropertyIndex);
	//查询用户
	virtual IClientUserItem * OnQueryClientUserItem() { return m_pIMySelfUserItem; };
	//使用道具
	virtual bool OnEventUseProperty(WORD wPropertyIndex);

	//进程接口
public:
	//进程错误
	virtual bool OnGameProcessError();
	//进程关闭
	virtual bool OnGameProcessClose(DWORD dwExitCode);
	//进程启动
	virtual bool OnGameProcessCreate(bool bAlreadyExist);
	//进程数据
	virtual bool OnProcessManagerData(WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize);

	//用户接口
public:
	//用户激活
	virtual VOID OnUserItemAcitve(IClientUserItem * pIClientUserItem);
	//用户删除
	virtual VOID OnUserItemDelete(IClientUserItem * pIClientUserItem);
	//用户更新
	virtual VOID OnUserFaceUpdate(IClientUserItem * pIClientUserItem);
	//用户更新
	virtual VOID OnUserItemUpdate(IClientUserItem * pIClientUserItem, tagUserScore & LastScore);
	//用户更新
	virtual VOID OnUserItemUpdate(IClientUserItem * pIClientUserItem, tagUserStatus & LastStatus);
	//用户更新
	virtual VOID OnUserItemUpdate(IClientUserItem * pIClientUserItem, tagUserAttrib & UserAttrib);

	//事件接口
protected:
	//取消连接
	virtual VOID OnStatusCancel();

	//菜单接口
public:
	//显示菜单
	virtual VOID TrackUserItemMenu(IClientUserItem * pIClientUserItem);
	//显示菜单
	virtual VOID TrackUserItemMenu(IClientUserItem * pIClientUserItem[], WORD wUserCount);

	//信息函数
public:
	//房间标识
	virtual WORD GetServerID() { return m_GameServer.wServerID; }
	//房间类型
	virtual WORD GetServerType() { return m_wServerType; }
	//房间规则
	virtual DWORD GetServerRule() { return m_dwServerRule; }
	//游戏标识
	virtual WORD GetKindID() { return m_GameKind.wKindID; }
	//类型名字
	virtual LPCTSTR GetKindName() { return m_GameKind.szKindName; }
	//房间名字
	virtual LPCTSTR GetServerName() { return m_GameServer.szServerName; }
	//服务状态
	virtual enServiceStatus GetServiceStatus() { return m_ServiceStatus; }
	//游戏配置
	virtual CParameterGame * GetParameterGame() { return m_pParameterGame; }
	//房间配置
	virtual CParameterServer * GetParameterServer() { return m_pParameterServer; }

	//组件函数
public:
	//网络组件
	virtual ITCPSocket * GetTCPSocket() { return m_TCPSocketModule.GetInterface(); }	
	//桌子视图
	virtual ITableViewFrame * GetTableViewFrame() { return m_pITableViewFrame; }
	//进程组件
	virtual CProcessManager * GetProcessManager() { return &m_ProcessManager; }
	//等级组件
	virtual IGameLevelParser * GetGameLevelParser() { return m_GameLevelParserModule.GetInterface(); }
	//用户组件
	virtual IPlazaUserManager * GetPlazaUserManager() { return m_PlazaUserManagerModule.GetInterface(); }

	//功能函数
public:
	//显示个人
	VOID ShowIndividual(); 
	//显示低保
	VOID ShowBaseEnsure(); 
	//显示签到
	VOID ShowCheckIn();
	//显示银行
	VOID ShowInsureView();
	//显示任务
	VOID ShowTask();
	//显示兑换
	VOID ShowExchange();
	//显示商城
	VOID ShowShop();
	//显示背包
	VOID ShowBag();
	//显示道具使用
	VOID ShowPropertyUse(IClientUserItem* pClientUserItem);
	//查询赠送
	bool QuerySendPresentMisson();

	//功能函数
public:	
	//老板事件
	VOID NotifyBossCome(bool bBossCome);
	//查找桌子
	VOID SearchGameTable(tagSearchTable & SearchTable);
	//更新头像
	VOID UpdateUserCustomFace(DWORD dwUserID, DWORD dwCustomID, tagCustomFaceInfo & CustomFaceInfo);
	//查询游戏客户端
	bool IsGameClientReady();
	//自己状态
	bool IsPlayingMySelf();
	//查找用户
	IClientUserItem * SearchUserByUserID (DWORD dwUserID);
	//坐到邀请桌
	bool SitDownInviteTable(WORD wTableID, bool bInRoom);

	//私聊函数
public:
	//私聊会话
	VOID WhisperConversation(IClientUserItem * pIClientUserItem[], WORD wUserCount);
	//创建私聊
	CDlgWhisper * CreateWhisperWnd(DWORD dwConversationID,DWORD dwSendUserID,DWORD dwUserID[], WORD wUserCount, bool bCreate);

	//网络事件
protected:
	//用户处理
	bool OnSocketMainUser(WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//登录处理
	bool OnSocketMainLogon(WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//配置处理
	bool OnSocketMainConfig(WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//状态处理
	bool OnSocketMainStatus(WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//银行处理
	bool OnSocketMainInsure(WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//任务处理
	bool OnSocketMainTask(WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//兑换处理
	bool OnSocketMainExchange(WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//道具处理
	bool OnSocketMainProperty(WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//管理处理
	bool OnSocketMainManager(WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//系统处理
	bool OnSocketMainSystem(WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//游戏处理
	bool OnSocketMainGameFrame(WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//比赛处理
	bool OnSocketMainMatch(WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//签到处理
	bool OnSocketMainCheckIn(WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//会员处理
	bool OnSocketMainMember(WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//低保处理
	bool OnSocketMainBaseEnsure(WORD wSubCmdID, VOID * pData, WORD wDataSize);

	//网络事件
protected:
	//用户进入
	bool OnSocketSubUserEnter(VOID * pData, WORD wDataSize);
	//用户积分
	bool OnSocketSubUserScore(VOID * pData, WORD wDataSize);
	//用户状态
	bool OnSocketSubUserStatus(VOID * pData, WORD wDataSize);
	//用户游戏数据
	bool OnSocketSubUserItemGameData(VOID * pData, WORD wDataSize);
	//登录完成
	bool OnSocketSubLogonFinish(VOID * pData, WORD wDataSize);
	//登录成功
	bool OnSocketSubLogonSuccess(VOID * pData, WORD wDataSize);
	//登录失败
	bool OnSocketSubLogonFailure(VOID * pData, WORD wDataSize);
	//更新提示
	bool OnSocketSubUpdateNotify(VOID * pData, WORD wDataSize);
	//请求失败
	bool OnSocketSubRequestFailure(VOID * pData, WORD wDataSize);
	//用户聊天
	bool OnSocketSubUserChat(VOID * pData, WORD wDataSize);
	//用户表情
	bool OnSocketSubExpression(VOID * pData, WORD wDataSize);
	//用户私聊
	bool OnSocketSubWisperUserChat(VOID * pData, WORD wDataSize);
	//私聊表情
	bool OnSocketSubWisperExpression(VOID * pData, WORD wDataSize);
	//系统消息
	bool OnSocketSubSystemMessage(VOID * pData, WORD wDataSize);
	//动作消息
	bool OnSocketSubActionMessage(VOID * pData, WORD wDataSize);
	//邀请玩家
	bool OnSocketSubUserInvite(VOID * pData, WORD wDataSize);
	//道具成功
	bool OnSocketSubPropertySuccess(VOID * pData, WORD wDataSize);
	//道具失败
	bool OnSocketSubPropertyFailure(VOID * pData, WORD wDataSize);
	//道具效应
	bool OnSocketSubPropertyEffect(VOID * pData, WORD wDataSize);
	//道具消息
	bool OnSocketPropertyMessage(VOID * pData, WORD wDataSize);
	//道具喇叭
	bool OnSocketPropertyTrumpet(VOID * pData, WORD wDataSize);
	//等待分配
	bool OnSocketSubWaitDistribute(VOID *pData, WORD wDataSize);
	//道具效果
	bool OnSocketSubUserPropBuff(VOID *pData, WORD wDataSize);
	//玩家背包
	bool OnSocketSubUserBackpack(VOID *pData, WORD wDataSize);
	//道具使用
	bool OnSocketSubPropertyUse(VOID *pData, WORD wDataSize);
	//查询赠送
	bool OnSocketSubQuerySendPresent(VOID *pData, WORD wDataSize);
	//赠送道具
	bool OnSocketSubPropertyPresent(VOID *pData, WORD wDataSize);
	//获取赠送
	bool OnSocketSubGetSendPresent(VOID *pData, WORD wDataSize);
	//玩家喇叭
	bool OnSocketSubUserTrumpet(VOID *pData, WORD wDataSize);

	//界面函数
public:
	//调整控件
	VOID RectifyControl(INT nWidth, INT nHeight);
	//配置房间
	VOID InitServerViewItem(CGameServerItem * pGameServerItem,WORD wAVServerPort,DWORD dwAVServerAddr);
	//设置界面
	VOID InitServerViewCtrl(CSkinRenderManager * pSkinRenderManager);

	//网络命令
public:
	//发送规则
	bool SendUserRulePacket();
	//发送登录
	bool SendLogonPacket(LPCTSTR pszServerPasswd);	
	//发送旁观
	bool SendLookonPacket(WORD wTableID, WORD wChairID);
	//发送坐下
	bool SendSitDownPacket(WORD wTableID, WORD wChairID, LPCTSTR lpszPassword=NULL);
	//发送起立
	bool SendStandUpPacket(WORD wTableID, WORD wChairID, BYTE cbForceLeave);
	//发送表情
	bool SendExpressionPacket(DWORD dwTargetUserID, WORD wItemIndex);
	//发送聊天
	bool SendUserChatPacket(DWORD dwTargetUserID, LPCTSTR pszChatString, COLORREF crColor);

	//动作处理
public:
	//执行旁观
	bool PerformLookonAction(WORD wTableID, WORD wChairID);
	//执行起立
	bool PerformStandUpAction(WORD wTableID, WORD wChairID);
	//执行坐下
	bool PerformSitDownAction(WORD wTableID, WORD wChairID, bool bEfficacyPass);
	//执行购买
	bool PerformBuyProperty(BYTE cbRequestArea,LPCTSTR pszNickName, WORD wItemCount, WORD wPropertyIndex);

	//辅助函数
protected:
	//设置状态
	VOID SetServiceStatus(enServiceStatus ServiceStatus);
	//资源目录
	VOID GetGameResDirectory(TCHAR szResDirectory[], WORD wBufferCount);

	//按钮事件
protected:
	//加入按钮
	VOID OnTableButton1Clicked();
	//查找按钮
	VOID OnTableButton2Clicked();
	//加密按钮
	VOID OnTableButton3Clicked();
	//加密按钮
	VOID OnTableButton4Clicked();
	//更多按钮
	VOID OnTableButton5Clicked();

	//内部函数
protected:	
	//创建视图框架
	bool CreateViewFrame();
	//创建管理菜单
	VOID CreateServerManagerMenu( CSkinMenu &ManageMenu );	
	//桌子效验
	bool EfficacyTableRule(WORD wTableID, WORD wChairID, bool bReqLookon, CString & strDescribe);

	//管理命令
protected:
	//管理按钮
	VOID OnBnClickedServerManage();
	//房间管理
	VOID OnManageServer();
	//用户管理
	VOID OnManageUser();
	//系统消息管理
	VOID OnManageSystemMsg();
	//桌子规则管理
	VOID OnManageTableRule();
	//机器人管理
	VOID OnManageUserAndior();

	//消息函数
protected:
	//销毁消息
	VOID OnNcDestroy();
	//时钟消息
	VOID OnTimer(UINT nIDEvent);
	//位置消息
	VOID OnSize(UINT nType, INT cx, INT cy);

	//控件消息
protected:
	//加为好友
	LRESULT OnWhisperAddFriend(WPARAM wParam, LPARAM lPara);
	//设为厌恶
	LRESULT OnWhisperSetDetest(WPARAM wParam, LPARAM lPara);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

#endif