#ifndef PLAT_FORM_EVENT_HEAD_FILE
#define PLAT_FORM_EVENT_HEAD_FILE

#pragma once

#include "Stdafx.h"

//////////////////////////////////////////////////////////////////////////////////
//事件定义

//用户事件
#define EVENT_USER_LOGON			1									//登录成功
#define EVENT_USER_LOGOUT			2									//注销成功
#define EVENT_USER_INFO_UPDATE		3									//资料更新
#define EVENT_USER_MOOR_MACHINE		4									//绑定机器
#define EVENT_USER_SHOW_CHECKIN		5									//显示签到
#define EVENT_USER_SHOW_TASKVIEW	6									//显示签到
#define EVENT_USER_SHOW_MY_SPREAD   7									//显示推广	
#define EVENT_USER_SHOW_RECHARGE    8									//显示充值
#define EVENT_USER_TRACK_ACCOUNTS	9									//追踪帐号
#define EVENT_USER_SHOW_PERSONER	10									//个人中心
#define EVENT_USER_SHOW_BASE_ENSURE	11									//领取低保
#define EVENT_USER_SHOW_BAG			12									//显示背包
#define EVENT_USER_SHOW_SHOP		13									//显示商城
#define EVENT_USER_SHOW_EXCHANGE	14									//显示兑换
#define EVENT_USER_SHOW_MY_INSURE	15									//显示银行
#define EVENT_USER_SHOW_REAL_TIP	16									//显示实名
#define EVENT_USER_SHOW_UPDATE_BUFF	17									//更新BUff
#define EVENT_USER_HIDE_REAL_TIP	18									//隐藏实名
#define EVENT_USER_SHOW_BAG_RECORD	19									//显示记录
#define EVENT_DOWN_LOAD_FINISH		20									//下载成功
#define EVENT_SKIN_CONFIG_UPDATE	21									//资源更新
#define EVENT_USER_CLOSE_GAME_AD	22									//游戏活动

//////////////////////////////////////////////////////////////////////////////////

//通知消息
#define WM_PLATFORM_EVENT			(WM_USER+1000)						//平台事件

//////////////////////////////////////////////////////////////////////////////////

//事件管理
class CPlatformEvent
{
	//变量定义
protected:
	CWHArray<HWND>					m_EventWndArray;					//事件窗口

	//静态变量
protected:
	static CPlatformEvent *			m_pPlatformEvent;					//对象指针

	//函数定义
public:
	//构造函数
	CPlatformEvent();
	//析构函数
	virtual ~CPlatformEvent();

	//注册窗口
public:
	//注册窗口
	bool RegisterEventWnd(HWND hEventWnd);
	//注销窗口
	bool UnRegisterEventWnd(HWND hEventWnd);

	//发送事件
public:
	//发送事件
	VOID SendPlatformEvent(WORD wEventID, LPARAM lParam);
	//发送事件
	VOID PostPlatformEvent(WORD wEventID, LPARAM lParam);

	//静态函数
public:
	//获取对象
	static CPlatformEvent * GetInstance() { return m_pPlatformEvent; }
};

//////////////////////////////////////////////////////////////////////////////////

#endif