#ifndef PLAZA_HALL_LIST_HEAD_FILE
#define PLAZA_HALL_LIST_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "ServerListData.h"
#include "DlgDownLoad.h"

//////////////////////////////////////////////////////////////////////////////////

//类型视图
class CPlazaHallList : public IDownLoadStateSink
{
	//函数定义
public:
	//构造函数
	CPlazaHallList(CPaintManagerUI*	pPaintManager);
	//析构函数
	virtual ~CPlazaHallList();

	//重载函数
public:
	//初始控件
	bool InitControlUI();
	//消息提醒
	bool Notify(TNotifyUI &  msg);

	//用户函数
public:
	//获取控件
	CControlUI * GetControlByName(LPCTSTR pszControlName);

	//消息函数
public:
	//点击增加
	void OnNotifyClickAddGame(TNotifyUI &  msg);
	//点击展开
	void OnNotifyClickExpandAll(TNotifyUI &  msg);
	//点击收缩
	void OnNotifyClickCollapseAll(TNotifyUI &  msg);
	//展开消息
	void OnNotifyItemExpand(TNotifyUI &  msg);
	//收缩消息
	void OnNotifyItemCollapse(TNotifyUI &  msg);
	//单击列表
	void OnNotifyListClickNode(TNotifyUI &  msg);
	//激活列表
	void OnNotifyListActivateNode(TNotifyUI &  msg);


public:
	//事件消息
	bool OnMessagePlatformEvent(WPARAM wParam, LPARAM lParam);
public:
	//异步发送
	virtual bool SendMessage( UINT message, WPARAM wParam , LPARAM lParam );


public:

	//显示列表
	void ShowPlazaHallList();

	//加载数据
	void LoadDataFromMemory();

	//初始界面
	void InitUI();

	//创建页面
	void CreateUI();

	//刷新界面
	void UpdateUI();

	//刷新下载
	void UpdateDownUI(CPlazaHallGameDownLoadSink * pDownLoadSink);

	//更新ICO
	void UpdateItemIco(CContainerUI* pContainerUI, CGameKindItem* pGameKindItem);

	//显示数据
protected:
	vector<CGameTypeItem *>   m_vGameType;
	vector<CGameKindItem *>   m_vGameKind;
	vector<CGameServerItem *> m_vGameServer;
	vector<CContainerUI *>	  m_vGameDown;
private:
	CPaintManagerUI*		m_pPaintManager;	
};

//////////////////////////////////////////////////////////////////////////////////

#endif