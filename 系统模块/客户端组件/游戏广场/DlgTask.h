#ifndef DLG_DLGTASK_HEAD_FILE
#define DLG_DLGTASK_HEAD_FILE

#pragma once
//////////////////////////////////////////////////////////////////////////////////
//类型定义
typedef CMap<WORD,WORD,tagTaskStatus *,tagTaskStatus *>  CTaskStatusMap;
typedef CMap<WORD,WORD,tagTaskParameter *,tagTaskParameter *>  CTaskParameterMap;

//任务窗口
class CDlgTask : public CFGuiDialog ,public CMissionItem
{
	//任务标识
protected:
	bool									m_bLoadTask;						//加载任务
	bool									m_bTakeTask;						//领取任务
	bool									m_bTaskReward;						//领取奖励
	bool									m_bTaskGiveUp;						//放弃任务
	//任务变量
protected:	
	WORD									m_wTaskID;							//任务标识
	tagTaskStatus *							m_pTaskStatus;						//任务状态	
	tagTaskParameter *						m_pTaskParameter;					//任务参数	

	//存储变量
protected:
	CTaskStatusMap							m_TaskStatusMap;					//任务状态
	CTaskParameterMap						m_TaskParameterMap;					//所有任务
	//组件变量
protected:
	CMissionManager							m_MissionManager;					//任务管理
	//显示数据
protected:
	vector<tagTaskParameter *> m_vecReceiving;
	vector<tagTaskParameter *> m_vecReceived;
	vector<tagTaskParameter *> m_vecCompleted;
	vector<tagTaskParameter *> m_vecFail;
	vector<tagTaskParameter *> m_vecAward;
	//函数定义
public:
	//构造函数
	CDlgTask();
	//析构函数
	virtual ~CDlgTask();

	//重载函数
protected:
	//创建函数
	virtual BOOL OnInitDialog();
	//确定函数
	virtual VOID OnOK();
	//取消消息
	virtual VOID OnCancel();

	//接口函数
public:
	//初始控件
	virtual void InitControlUI();
	//消息提醒
    virtual void Notify(TNotifyUI &  msg);
	//皮肤名称
	virtual LPCTSTR GetSkinFile();

	//网络事件
public:
	//连接事件
	virtual bool OnEventMissionLink(INT nErrorCode);
	//关闭事件
	virtual bool OnEventMissionShut(BYTE cbShutReason);
	//读取事件
	virtual bool OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize);
	//事件处理
public:
	//开始任务
	virtual VOID OnMissionStart();
	//终止任务
	virtual VOID OnMissionConclude();
	//功能函数
protected:
	//加载任务
	virtual VOID PerformLoadTask();
	//领取任务
	virtual VOID PerformTakeTask();
	//领取奖励
	virtual VOID PerformRewardTask();
	//放弃任务
	virtual VOID PerformGiveUpTask();	

public:
	//添加任务
	BOOL AddAllTaskItem(VOID * pTaskParameterData,WORD wParameterSize);
	//添加任务
	BOOL AddMineTaskItem(tagTaskStatus & TaskStatus);
	//删除任务
	BOOL DeleteMineTaskItem(tagTaskStatus & TaskStatus);
	//更新数据
	VOID LoadDataFromMemory();

	//新增任务
	VOID AddTask(CPageLayoutUI * page,tagTaskParameter * pTaskParameter);

	//刷新UI
	VOID UpdateUI();

	//领取任务
	VOID ViewTaskReceiving(CControlUI * pControlUI);
	//放弃任务
	VOID ViewTaskReceived(CControlUI * pControlUI);
	//领取奖励
	VOID ViewTaskCompleted(CControlUI * pControlUI);
};

//////////////////////////////////////////////////////////////////////////////////

//房间任务
class CDlgTaskServer : public CDlgTask
{
	//函数定义
public:
	//构造函数
	CDlgTaskServer();
	//析构函数
	virtual ~CDlgTaskServer();

	//功能函数
protected:
	//加载任务
	virtual VOID PerformLoadTask();
	//领取任务
	virtual VOID PerformTakeTask();
	//领取奖励
	virtual VOID PerformRewardTask();
	//放弃任务
	virtual VOID PerformGiveUpTask();	

	//功能函数
public:
	//配置任务
	VOID Initialization(ITCPSocket * pITCPSocket, IClientUserItem * pIMySelfUserItem);
	//任务消息
	bool OnTaskMessage(WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//变量定义
protected:
	ITCPSocket *					m_pITCPSocket;						//网络接口
	IClientUserItem *				m_pIMySelfUserItem;					//自己指针
};

////////////////////////////////////////////////////////////////////////////////////////////////////

#endif