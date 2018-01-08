#ifndef PLAZA_HALL_GAME_HEAD_FILE
#define PLAZA_HALL_GAME_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "ServerListData.h"
#include "DlgDownLoad.h"
#include "DlgUpdateStatus.h"
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////

//类型视图
class CPlazaHallGame : public IDownLoadStateSink,public IUpdateStatusSink
{
	//函数定义
public:
	//构造函数
	CPlazaHallGame(CPaintManagerUI*	pPaintManager);
	//析构函数
	virtual ~CPlazaHallGame();
	//重载函数
public:
	//消息提醒
	bool Notify(TNotifyUI &  msg);
	//初始控件
	bool InitControlUI();
	//判断消息
	bool IsNotify(TNotifyUI &  msg);
	//获取控件
	CControlUI * GetControlByName(LPCTSTR pszControlName);
	//事件消息
public:
	//事件消息
	bool OnMessagePlatformEvent(WPARAM wParam, LPARAM lParam);

	//下载接口
public:
	//异步发送
	virtual bool SendMessage( UINT message, WPARAM wParam , LPARAM lParam );
public:
	//初始数据
	VOID InitData();

	//加载数据
	VOID ShowPlazaHallGame();

	//刷新界面
	VOID UpdateUI();

	//刷新推荐
	VOID UpdateAdUI();

	//刷新下载
	VOID UpdateDownUI(CPlazaHallGameDownLoadSink * pDownLoadSink);

	//刷新状态
	VOID UpdateDownStateUI(CContainerUI * pDwon,int nState);

	//刷新房间
	VOID UpdateRoomUI(CGameKindItem * pGameKindItem);

	//刷新删除
	VOID UpdateDeleteUI();

	//业务函数
public:
	//查询更新
	bool CheckUpdate(CGameKindItem *vMyGameKind);

	//下载游戏
	bool DownLoadGame(LPCTSTR pszClientName, WORD wKindID, WORD wServerID);

	//保存游戏
	bool SaveGameKind();

	//加载游戏
	bool LoadDataFromMemory();

	//加载游戏
	bool LoadDataFromFile();

	//匹配游戏
	bool FilterGameKind();

	//增加游戏
	bool AddGameKind();

	//删除游戏
	bool DeleteGameKind(CGameKindItem * pGameKindItem);

	//进入游戏
	bool EnterGameKind(CGameKindItem * pGameKindItem);

	//进入推荐游戏
	bool EnterGameAdKind(CGameKindItem * pGameKindItem);

	//进入房间
	bool EnterGameServer(CGameServerItem * pGameServerItem);

	//关闭删除
	VOID CloseDeleteState();

	//返回大厅
	VOID ReturnHallMain();

	VOID GetGameResDirectory(TCHAR szResDirectory[], WORD wBufferCount);
public:
	virtual VOID OnEventDownLoadTaskFinish() ;

	virtual VOID OnEventUpdateCheckFinish(bool bNeedUpdate);
	//辅助函数
private:
	//进程名称
	inline void GetProcessName(CGameKindItem *vMyGameKind,TCHAR szProcessName[LEN_PROCESS]);



	

	//显示数据
protected:
	CPaintManagerUI*		m_pPaintManager;
	CWHArray<CPlazaHallGameDownLoadSink *>		m_DownLoadSinkArray;				//下载数组
	vector<CPlazaHallGameDownLoadSink *>		m_vDwonLoadSink;
protected:
	vector<CGameKindItem *> m_vMyGameKind;
	vector<CGameKindItem *> m_vAdGameKind;
	vector<CGameKindItem *> m_vGameKind;
	vector<int>				m_vGameKindID;
	vector<CGameServerItem *> m_vGameServer;
	vector<CControlUI *>    m_vMyGameDelete;
	vector<CContainerUI *>  m_vMyGameDown;

	CGameKindItem * m_pEnterGame;

	//控件变量
protected:
	CUpdateStatusView *				m_pUpdateStatusView;				//更新视图

};

//////////////////////////////////////////////////////////////////////////////////

#endif