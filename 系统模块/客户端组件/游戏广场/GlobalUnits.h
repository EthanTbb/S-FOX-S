#ifndef GLOBAL_UNITES_HEAD_FILE
#define GLOBAL_UNITES_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "DlgDownLoad.h"
class CDlgBagPlaza;
class CDlgSetup;
//////////////////////////////////////////////////////////////////////////////////

//全局单元
class CGlobalUnits
{
	//版本信息
protected:
	DWORD							m_dwPlazaVersion;					//大厅版本
	DWORD							m_dwFrameVersion;					//框架版本

	//追踪变量
protected:
	WORD							m_wTrackServerID;					//追踪房间
	DWORD							m_dwTrackUserID;					//追踪标识

	//公用配置
protected:
	tagAuthRealParameter			m_AuthRealParameter;				//实名参数

	//组件指针
protected:
	//CDlgDownLoad *					m_pDlgDownLoad;						//下载组件
	CDlgBagPlaza *					m_pDlgBagPlaza;						//背包组件
	CDlgSetup	 *					m_pDlgSetup;						//系统设置

	//静态变量
protected:
	static CGlobalUnits *			m_pGlobalUnits;						//对象指针

	//函数定义
public:
	//构造函数
	CGlobalUnits();
	//析构函数
	virtual ~CGlobalUnits();

	//版本信息
public:
	//大厅版本
	virtual DWORD GetPlazaVersion() { return m_dwPlazaVersion; }
	//框架版本
	virtual DWORD GetFrameVersion() { return m_dwFrameVersion; }

	//甜饼信息
public:
	//记录甜饼
	bool WriteUserCookie();
	//删除甜饼
	bool DeleteUserCookie();

	//声音函数
public:
	//播放声音
	bool PlayGameSound(LPCTSTR pszSoundName);
	//播放声音
	bool PlayGameSound(HINSTANCE hInstance, LPCTSTR pszSoundName);

	//追踪函数
public:
	//获取用户
	DWORD GetTrackUserID();
	//获取房间
	WORD GetTrackServerID();
	//设置标识
	VOID SetTrackUserID(DWORD dwTrackUserID) { m_dwTrackUserID=dwTrackUserID;}
	//设置标识
	VOID SetTrackServerID(WORD wTrackServerID) { m_wTrackServerID=wTrackServerID;}

	//功能函数
public:
	////下载游戏
	//bool DownLoadClient(LPCTSTR pszClientName, WORD wKindID, WORD wServerID);
	//查询赠送道具
	bool QuerySendPresentMisson();

	//功能函数
public:
	//显示提示
	VOID PerformShowRealTip();
	//显示个人
	VOID PerformShowPersonerCenter(BYTE cbSelectedIndex = 0);
	//显示银行
	VOID PerformShowInsure();
	//显示任务
	VOID PerformShowTaskView();
	//显示推广
	VOID PerformShowMySpread();	
	//显示兑换
	VOID PerformShowExchange();
	//显示签到
	VOID PerformShowCheckIn();	
	//显示低保
	VOID PerformShowBaseEnsure();
	//锁定机器
	VOID PerformShowLockMachine();
	//查找用户
	VOID PerformShowSearchUser();
	//充值中心
	VOID PerformShowRecharge();
	//游戏商场
	bool PerformShowShop();
	//我的背包
	bool PerformShowBag();
	//我的背包
	bool PerformShowBagRecord();
	//房间菜单道具使用
	VOID PerformShowPropertyUse(IClientUserItem* pClientUserItem);

	//我的好友
	VOID PerformShowFriend();
	//系统设置
	VOID PerformShowSetup();
	//联系客服
	VOID PerformShowContact();
	//反馈信息
	VOID PerformShowFeedBack();
	//帮助中心
	VOID PerformShowHelp();
	//显示信息
	VOID PerformShowGameAd();
	//设置配置
public:
	VOID SetAuthRealParameter(const tagAuthRealParameter* pInfo);
	const tagAuthRealParameter* GetAuthRealParameter();

	//静态函数
public:
	//获取对象
	static CGlobalUnits * GetInstance() { return m_pGlobalUnits; }
};

//////////////////////////////////////////////////////////////////////////////////

#endif