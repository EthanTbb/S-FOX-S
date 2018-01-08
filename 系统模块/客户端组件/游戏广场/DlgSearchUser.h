#ifndef DLG_SEARCH_USER_HEAD_FILE
#define DLG_SEARCH_USER_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "PlazaViewServer.h"

//////////////////////////////////////////////////////////////////////////////////

//追踪信息
struct tagTrackAccountsInfo
{
	WORD							wKindID;							//类型标识
	WORD							wServerID;							//房间标识
	DWORD							dwTargetID;							//目标标识
};

//////////////////////////////////////////////////////////////////////////////////

//查找用户
class CDlgSearchUser : public CSkinDialog, public CMissionItem
{
	//变量定义
protected:
	DWORD							m_dwGameID;							//游戏标识
	TCHAR							m_szNickName[LEN_NICKNAME];			//用户昵称

	//信息变量
protected:
	tagUserRemoteInfo				m_UserRemoteInfo;					//远程信息

	//组件变量
protected:
	CMissionManager					m_MissionManager;					//任务管理

	//静态变量
protected:
	static CDlgSearchUser *			m_pDlgSearchUser;					//对象指针

	//函数定义
public:
	//构造函数
	CDlgSearchUser();
	//析构函数
	virtual ~CDlgSearchUser();

	//重载函数
protected:
	//消息过虑
	virtual BOOL PreTranslateMessage(MSG * pMsg);
	//创建函数
	virtual BOOL OnInitDialog();
	//确定函数
	virtual VOID OnOK();

    //接口函数
public:
	//初始控件
	virtual void InitControlUI();
	//消息提醒
	virtual void Notify(TNotifyUI &  msg);	

	//重载函数
public:
	//皮肤名称
	virtual LPCTSTR GetSkinFile() { return TEXT("SearchUser.xml"); } 

	//重载函数
protected:
	//绘画标题
	virtual VOID OnDrawSystemTitle(CDC * pDC, INT nWdith, INT nHeight);
	//绘画消息
	virtual VOID OnDrawClientAreaEnd(CDC * pDC, INT nWidth, INT nHeight);

	//任务事件
private:
	//连接事件
	virtual bool OnEventMissionLink(INT nErrorCode);
	//关闭事件
	virtual bool OnEventMissionShut(BYTE cbShutReason);
	//读取事件
	virtual bool OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize);

	//事件函数
public:
	//房间关闭
	VOID OnViewItemDelete(CPlazaViewServer * pServerViewItem);
	//用户删除
	VOID OnUserItemDelete(IClientUserItem * pIClientUserItem, CPlazaViewServer * pServerViewItem);
	//用户更新
	VOID OnUserItemUpdate(IClientUserItem * pIClientUserItem, CPlazaViewServer * pServerViewItem);

	//辅助函数
protected:
	//加载好友
	VOID LoadFriendUserInfo();

	//静态函数
public:
	//获取对象
	static CDlgSearchUser * GetInstance() { return m_pDlgSearchUser; }

	//消息函数
protected:
	//时间消息
	VOID OnTimer(UINT_PTR nIDEvent);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

#endif