#ifndef DLG_CHECK_IN_HEAD_FILE
#define DLG_CHECK_IN_HEAD_FILE
#pragma once

/////////////////////////////////////////////////////////////////////////////////////////
//签到窗口
class CDlgCheckIn :	public CFGuiDialog,public CMissionItem
{
	//是否领取
protected:
	bool							m_bButtonEnableMemberDayGift;
	bool							m_bButtonEnableMemberDayPresent;
	//签到配置
protected:
	bool							m_bTodayChecked;					//签到标识
	WORD							m_wDateIndex;						//日期索引
	SCORE							m_lRewardGold[LEN_WEEK];			//奖励金币														

	//任务标识
protected:
	bool							m_bCheckInDone;						//执行签到
	bool							m_bQueryCheckInfo;					//查询信息	
	//会员标识
protected:
	bool							m_bMemberQueryInfo;					//会员查询
	bool							m_bMemberDayGift;					//会员礼包
	bool							m_bMemberDayPresent;				//会员送金	
	tagGiftPropertyInfo				m_GiftSub[50];						//包含道具
	DWORD							m_wGiftSubCount;					//道具数量
	//组件变量
protected:
	CMissionManager					m_MissionManager;					//任务管理

public:
	CDlgCheckIn();
	virtual ~CDlgCheckIn();

	//重载函数
public:
	//皮肤名称
	virtual LPCTSTR GetSkinFile();

	//接口函数
public:
	//初始控件
	virtual void InitControlUI();
	//消息提醒
	virtual void Notify(TNotifyUI &  msg);	

	//重载函数
protected:
	//创建函数
	virtual BOOL OnInitDialog();

	//按钮事件
protected:
	//点击签到
	VOID OnButtonCheckInClicked();

	//点击礼包
	VOID OnButtonGiftClicked();

	//辅助函数
protected:
	//执行查询
	virtual VOID PerformQueryCheckInfo();
	//执行签到
	virtual VOID PerformCheckInDone();
	//会员礼包
	virtual VOID PerformMemberDayGift();
	//会员送金
	virtual VOID PerformMemberDayPresent();
	//会员查询
	virtual VOID PerformMemberQueryInfo();

	//更新会员
	VOID UpdateMember();
	//更新界面
	VOID UpdateCheckIn();
	//设置布局
	VOID SetSignLayout(LPCTSTR szLayoutName, const int nDay, const bool bSign, const int nSignGold);
	//事件处理
public:
	//开始任务
	VOID OnMissionStart();
	//终止任务
	VOID OnMissionConclude();

	//网络事件
public:
	//连接事件
	virtual bool OnEventMissionLink(INT nErrorCode);
	//关闭事件
	virtual bool OnEventMissionShut(BYTE cbShutReason);
	//读取事件
	virtual bool OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize);

	DECLARE_MESSAGE_MAP()
};


//房间签到
class CDlgCheckInServer : public CDlgCheckIn
{
	//函数定义
public:
	//构造函数
	CDlgCheckInServer();
	//析构函数
	virtual ~CDlgCheckInServer();

	//辅助函数
protected:
	//执行查询
	virtual VOID PerformQueryCheckInfo();
	//执行签到
	virtual VOID PerformCheckInDone();
	//会员礼包
	virtual VOID PerformMemberDayGift();
	//会员送金
	virtual VOID PerformMemberDayPresent();
	//会员查询
	virtual VOID PerformMemberQueryInfo();
	//功能函数
public:
	//配置
	VOID Initialization(ITCPSocket * pITCPSocket, IClientUserItem * pIMySelfUserItem);
	//签到消息
	bool OnServerCheckInMessage(WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//变量定义
protected:
	ITCPSocket *					m_pITCPSocket;						//网络接口
	IClientUserItem *				m_pIMySelfUserItem;					//自己指针



};


/////////////////////////////////////////////////////////////////////////////////////////
#endif
