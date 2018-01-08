#ifndef DLG_DLGFRIEND_HEAD_FILE
#define DLG_DLGFRIEND_HEAD_FILE

#pragma once
//////////////////////////////////////////////////////////////////////////////////


class CDlgFriend : public CFGuiDialog, public CMissionItem
{
	//函数定义
public:
	//构造函数
	CDlgFriend();
	//析构函数
	virtual ~CDlgFriend();
	//重载函数
protected:
	//创建函数
	virtual BOOL OnInitDialog();
	//重载函数
protected:
	//初始化
	void InitControlUI();
	//确定函数
	virtual VOID OnOK();
	//取消消息
	virtual VOID OnCancel();

	//接口函数
public:
	//消息提醒
    virtual void Notify(TNotifyUI &  msg);
	//自定义消息
	virtual LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	
	//重载函数
public:
	//皮肤名称
	virtual LPCTSTR GetSkinFile();
	virtual LPCTSTR GetWindowClassName() const;

	//任务事件
private:
	//连接事件
	virtual bool OnEventMissionLink(INT nErrorCode);
	//关闭事件
	virtual bool OnEventMissionShut(BYTE cbShutReason);
	//读取事件
	virtual bool OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize);

	//功能函数
public:
	void LoadFriendUserInfo();
	void UpdateFriendInfo(const tagUserRemoteInfo& UserInfo);
	CNodeElementUI* UpdateFriendOnLine(const tagUserRemoteInfo& UserInfo);
	bool IsOnline(const DWORD dwGameID);

	IClientUserItem* CanChat(const DWORD dwUserID);
	bool Chat(IClientUserItem* pUserItem);
	bool CanJoinGame(bool bTips, WORD& dwTableID, WORD& dwChairID);
	bool JoinGame(const WORD dwTableID, const WORD dwChairID);
	bool CanLookOnGame(bool bTips, WORD& dwTableID, WORD& dwChairID);
	bool LookOnGame(const WORD dwTableID, const WORD dwChairID);

	//内部数据
private:
	typedef vector<tagUserRemoteInfo> VSearchCorrespond;

	CMissionManager		m_MissionManager;		//任务管理
	VSearchCorrespond	m_vecFriendInfo;		//在线好友
	tagUserRemoteInfo	m_CurSelectUserInfo;	//当前选中的好友
	CListExUI*			m_pListFriend;			//好友列表
	CButtonUI*			m_pButtonJionGame;		//加入游戏
	CButtonUI*			m_pButtonWatchGame;		//旁观游戏
	CButtonUI*			m_pButtonChat;			//私聊
	CControlUI*			m_pFriendAvatar;		//头像
	CLabelUI*			m_pLabelFriendLevel;	//好友vip图标
	CLabelUI*			m_pLabelFriendNickName;	//好友昵称
	CLabelUI*			m_pLabelFriendInGame;	//在什么游戏
	CLabelUI*			m_pLabelFriendInRoom;	//在什么房间
	CLabelUI*			m_pLabelFriendInTable;	//在哪一桌
	CLabelUI*			m_pLabelFriendGameID;	//ID显示
};

//////////////////////////////////////////////////////////////////////////////////

#endif