#ifndef PLATFORM_FRAME_HEAD_FILE
#define PLATFORM_FRAME_HEAD_FILE

#pragma once

//系统文件
#include "Stdafx.h"
#include "Resource.h"

//控件文件
#include "DlgWhisper.h"
#include "DlgCollocate.h"
#include "DlgTaskCenter.h"

//窗口控件
#include "PlazaViewServerCtrl.h"

//任务文件
#include "MissionList.h"
#include "MissionLogon.h"

//主体窗口
#include "PlazaViewServer.h"
#include "PlazaViewContainer.h"

//游戏列表
#include "PlazaUserInfo.h"
#include "PlazaHallList.h"
#include "PlazaHallGame.h"
//////////////////////////////////////////////////////////////////////////////////

//命令定义
#define IDM_CLOSE					105                                 //关闭窗口
#define IDM_RESTORE                 106                                 //还原显示
#define IDM_MINIMIZE                107                                 //最小化显示
#define IDM_MAXIMIZE                108                                 //最大化显示

//命令定义
#define IDM_SHOW_LOGON				200									//显示登录

//////////////////////////////////////////////////////////////////////////////////
class CDlgEnquire;

//列表状态
enum enServerListControl
{
	ServerListControl_Hide,			//隐藏列表
	ServerListControl_Show,			//显示列表
	ServerListControl_Turn,			//翻转列表
};

//数组说明
typedef CWHArray<CPlazaViewServer *>	CPlazaViewServerArray;				//房间数组

//////////////////////////////////////////////////////////////////////////////////

//平台框架
class CPlatformFrame : public CFGuiFrameWnd, public ICustomFaceEvent,public IServerListDataSink
{
	//界面变量
protected:
	bool							m_bMaxShow;							//最大标志
	bool							m_bRectify;							//调整标志
	CRect							m_rcNormalSize;						//正常位置

	//状态变量
protected:
	bool							m_bWhisperIcon;						//私聊图标

	//游戏列表
protected:
	CServerListData					m_ServerListData;					//游戏数据
	CGamePropertyListData			m_GamePropertyListData;				//道具数据

protected:
	CPlazaUserInfo					m_PlazaUserInfo;
	CPlazaHallList					m_PlazaHallList;
	CPlazaHallGame					m_PlazaHallGame;


	//视图控件
protected:	
	//CPlazaViewGame					m_PlazaViewGame;					//游戏广场
	//CPlazaViewAffiche				m_PlazaViewAffiche;					//公告窗口	
	//CPlazaViewGameType				m_PlazaViewGameType;				//游戏类型
	CPlazaViewServerCtrl			m_PlazaViewServerCtrl;				//房间控制

	//视图容器
protected:
	CPlazaViewContainer				m_PlazaViewContainer;				//视图容器

	//控件变量
protected:
	CSystemTrayIcon					m_SystemTrayIcon;					//任务图标

	//任务组件
protected:
	CMissionList					m_MissionList;						//列表任务
	CMissionLogon					m_MissionLogon;						//登录任务
	CMissionManager					m_MissionManager;					//任务管理
	CDlgEnquire*					m_pDlgEnquire;				

	//控件数组
protected:
	CPlazaViewServer 				m_PlazaViewServer;					//当前房间
	CDlgWhisperItemArray			m_DlgWhisperItemArray;				//私聊数组

	//视频配置
protected:
	WORD							m_wAVServerPort;					//视频端口
	DWORD							m_dwAVServerAddr;					//视频地址

	//静态变量
protected:
	static CPlatformFrame *			m_pPlatformFrame;					//框架指针

	//函数定义
public:
	//构造函数
	CPlatformFrame();
	//析构函数
	virtual ~CPlatformFrame();

	//基础接口
public:
	//释放对象
	virtual VOID Release() { delete this; }
	//接口查询
	virtual VOID * QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	//头像接口
protected:
	//下载失败
	virtual VOID OnEventSystemFace(DWORD dwUserID, WORD wFaceID);
	//头像数据
	virtual VOID OnEventCustomFace(DWORD dwUserID, DWORD dwCustomID, tagCustomFaceInfo & CustomFaceInfo);

	//状态通知
public:
	//获取通知
	virtual VOID OnGameItemFinish();
	//完成通知
	virtual VOID OnGameMatchFinish();
	//获取通知
	virtual VOID OnGameKindFinish(WORD wKindID);

	//更新通知
public:
	//插入通知
	virtual VOID OnGameItemInsert(CGameListItem * pGameListItem);
	//更新通知
	virtual VOID OnGameItemUpdate(CGameListItem * pGameListItem);
	//删除通知
	virtual VOID OnGameItemDelete(CGameListItem * pGameListItem);

	//重载函数
protected:
	//消息过虑
	virtual BOOL PreTranslateMessage(MSG * pMsg);
	//命令函数
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);

	//重载函数
public:
	virtual void InitControlUI();
	//消息提醒
	virtual void Notify(TNotifyUI &  msg);	
	//皮肤名称
	virtual LPCTSTR GetSkinFile();

	//界面函数
private:
	//调整控件
	VOID RectifyControl(INT nWidth, INT nHeight);
	//功能函数
public:
	//类型区域
	CRect MapSelectedTypeRect();
	//任务提醒
	VOID ShowTaskNotifyer(LPCTSTR pszTaskName);
	//浏览页面
	bool WebBrowse(LPCTSTR pszURL, bool bAutoFullView);	
	//游戏下载
	bool DownLoadGame(LPCTSTR pszClientName, WORD wKindID, WORD wServerID);

	//私聊函数
public:
	//提取私聊
	bool ShowWhisperItem();
	//插入私聊
	bool InsertWhisperItem(CDlgWhisper * pDlgWhisper);
	//删除私聊
	bool RemoveWhisperItem(CDlgWhisper * pDlgWhisper);

	//房间管理
public:
	//激活房间
	bool ActiveServerViewItem();
	//删除房间
	bool DeleteServerViewItem();	
	//进入房间
	bool EntranceServerItem(CGameServerItem * pGameServerItem);
	//进房标识
	bool IsEnteredServerItem() { return m_PlazaViewServer.m_hWnd!=NULL; }
	//获取房间
	CPlazaViewServer * GetPlazaViewServer() { return &m_PlazaViewServer; }

	//视图控件
public:
	//游戏视图
	//CPlazaViewGame * GetPlazaViewGame() { return &m_PlazaViewGame; }
	//类型视图
	//CPlazaViewGameType * GetPlazaViewGameType() { return &m_PlazaViewGameType; }
// 	//公告视图
// 	CPlazaViewAffiche * GetPlazaViewAffiche() { return &m_PlazaViewAffiche; }
	//控制视图
	CPlazaViewServerCtrl * GetPlazaViewServerCtrl() { return &m_PlazaViewServerCtrl; }

public:
	//大厅查找
	CControlUI * GetPlazaControlByName(LPCTSTR pszControlName);

	//返回大厅
	void ReturnHallMain();

	//设置遮盖
	void ShowWindowMaskLayer(bool bShow =true);
	//视频配置
public:
	void SetVideoOption(WORD wAVServerPort, DWORD dwAVServerAddr);

	//界面函数
protected:
	//还原窗口
	bool RestoreWindow();
	//最大窗口
	bool MaxSizeWindow(bool bRecordPosition=true);

	//消息函数
protected:
	//关闭消息
	VOID OnClose();
	//时间消息
	VOID OnTimer(UINT_PTR nIDEvent);
	//位置消息
	VOID OnSize(UINT nType, INT cx, INT cy);
	//位置消息
	VOID OnGetMinMaxInfo(MINMAXINFO * lpMMI);
	//创建消息
	INT OnCreate(LPCREATESTRUCT lpCreateStruct);
	//鼠标消息
	VOID OnLButtonDblClk(UINT nFlags, CPoint Point);
	//改变消息
	VOID OnSettingChange(UINT uFlags, LPCTSTR lpszSection);

	//自定消息
protected:
	//热键消息
	LRESULT OnMessageHotKey(WPARAM wParam, LPARAM lParam);
	//任务消息
	LRESULT OnMessageTaskBar(WPARAM wParam, LPARAM lParam);
	//图标消息
	LRESULT OnMessageTrayIcon(WPARAM wParam, LPARAM lParam);
	//事件消息
	LRESULT OnMessagePlatformEvent(WPARAM wParam, LPARAM lParam);
	//事件消息
	LRESULT OnMessageInstallCancelEvent(WPARAM wParam, LPARAM lParam);
	//事件消息
	LRESULT OnMessageInsurePlazaEvent(WPARAM wParam, LPARAM lParam);

public:
		//自定义消息
	virtual LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	
	void OnWkeURLHidden();

	//功能函数
public:
	//获取实例
	static CPlatformFrame * GetInstance() { return m_pPlatformFrame; }


	DECLARE_MESSAGE_MAP()

};

//////////////////////////////////////////////////////////////////////////////////

#endif