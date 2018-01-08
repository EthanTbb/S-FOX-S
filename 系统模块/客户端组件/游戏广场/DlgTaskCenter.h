#ifndef DLG_TASK_CENTER_HEAD_FILE
#define DLG_TASK_CENTER_HEAD_FILE
#pragma once

//引入文件
#include "UITaskList.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
//类型定义
typedef CMap<WORD,WORD,tagTaskStatus *,tagTaskStatus *>  CTaskStatusMap;
typedef CMap<WORD,WORD,tagTaskParameter *,tagTaskParameter *>  CTaskParameterMap;

////////////////////////////////////////////////////////////////////////////////////////////////////
//任务指引
class CDlgTaskGuide : public CFGuiDialog
{
	//资源变量
protected:
	CButtonUI *						m_btLook;							//查看按钮
	CButtonUI *						m_btReturn;							//返回按钮	

	//资源变量
protected:
	CFont							m_FontText;							//字体变量

	//界面控件
protected:
	CSkinLayered					m_SkinLayered;						//分层窗口

	//函数定义
public:
	//构造函数
	CDlgTaskGuide();
	//析构函数
	virtual ~CDlgTaskGuide();

    //接口函数
public:
	//初始控件
	virtual void InitControlUI();
	//消息提醒
	virtual void Notify(TNotifyUI &  msg);	

	//绘画接口
public:
	//开始绘画
	virtual void OnBeginPaintWindow(HDC hDC);

	//重载函数
protected:
	//创建函数
	virtual BOOL OnInitDialog();
	//消息解释
	virtual BOOL PreTranslateMessage(MSG * pMsg);
	//取消事件
	virtual VOID OnCancel();
};


////////////////////////////////////////////////////////////////////////////////////////////////////
//任务提醒
class CDlgTaskNotifyer : public CFGuiDialog
{
	//信息变量
protected:
	TCHAR							m_szTaskName[LEN_TASK_NAME];		//任务名称

	//资源变量
protected:
	CFont							m_FontText;							//字体变量

	//界面控件
protected:
	CTextUI *						m_TextReward;						//奖励标签
	CButtonUI *						m_btClose;							//关闭按钮
	CSkinLayered					m_SkinLayered;						//分层窗口

	//函数定义
public:
	//构造函数
	CDlgTaskNotifyer();
	//析构函数
	virtual ~CDlgTaskNotifyer();

    //接口函数
public:
	//初始控件
	virtual void InitControlUI();
	//消息提醒
	virtual void Notify(TNotifyUI &  msg);	

	//绘画接口
public:
	//开始绘画
	virtual void OnBeginPaintWindow(HDC hDC);

	//功能函数
public:
	//任务名称
	VOID SetTaskName(LPCTSTR pszTaskName);

	//重载函数
protected:
	//创建函数
	virtual BOOL OnInitDialog();
	//消息解释
	virtual BOOL PreTranslateMessage(MSG * pMsg);
	//取消事件
	virtual VOID OnCancel();
};

////////////////////////////////////////////////////////////////////////////////////////////////////
//任务中心
class CDlgTaskCenter : public CSkinDialog
{
	//变量定义
protected:
	CFont									m_FontText;							//字体变量
	CRect									m_rcItemSelected;					//子项区域	

	//任务变量
protected:	
	WORD									m_wTaskID;							//任务标识
	tagTaskStatus *							m_pTaskStatus;						//任务状态	
	tagTaskParameter *						m_pTaskParameter;					//任务参数	

	//存储变量
protected:
	CTaskStatusMap							m_TaskStatusMap;					//任务状态
	CTaskParameterMap						m_TaskParameterMap;					//任务参数

	//控件变量
protected:
	CTaskUI *								m_pTaskList;						//任务列表
	CButtonUI *								m_btTaskTake;						//领取任务
	CButtonUI *								m_btTakeReward;						//领取奖励

	//节点变量
protected:
	Node *									m_pNodeSelfTask;					//我的任务
	Node *									m_pNodeAllTask;						//所有任务

	//函数定义
public:
	//构造函数
	CDlgTaskCenter();
	//析构函数
	virtual ~CDlgTaskCenter();

    //接口函数
public:
	//初始控件
	virtual void InitControlUI();
	//消息提醒
	virtual void Notify(TNotifyUI &  msg);	

	//绘画接口
public:
	//绘画标题
	virtual VOID OnDrawSystemTitle(CDC * pDC, INT nWdith, INT nHeight);
	//绘画消息
	virtual VOID OnDrawClientAreaBegin(CDC * pDC, INT nWidth, INT nHeight);

	//功能函数
protected:
	//加载任务
	virtual VOID PerformLoadTask() { return; }
	//领取任务
	virtual VOID PerformTakeTask() { return; }
	//领取奖励
	virtual VOID PerformRewardTask() { return; }

	//事件处理
public:
	//开始任务
	virtual VOID OnMissionStart();
	//终止任务
	virtual VOID OnMissionConclude();

	//辅助函数
protected:
	//调整控件
	VOID RectifyControlUI();
	//默认选择
	VOID SelectDefaultItem();
	//添加任务
	BOOL AddMineTaskItem(tagTaskStatus & TaskStatus);
	//添加任务
	BOOL AddAllTaskItem(VOID * pTaskParameterData,WORD wParameterSize);
	//更新状态
	VOID UpdateTaskStatus(WORD wTaskID,BYTE cbTaskStatus);

	//重载函数
protected:
	//创建函数
	virtual BOOL OnInitDialog();
	//消息解释
	virtual BOOL PreTranslateMessage(MSG * pMsg);
};


////////////////////////////////////////////////////////////////////////////////////////////////////

//大厅任务
class CDlgTaskPlazaCenter : public CDlgTaskCenter,public CMissionItem
{
	//任务标识
protected:
	bool									m_bLoadTask;						//加载任务
	bool									m_bTakeTask;						//领取任务
	bool									m_bTaskReward;						//领取奖励

	//组件变量
protected:
	CMissionManager							m_MissionManager;					//任务管理

	//函数定义
public:
	//构造函数
	CDlgTaskPlazaCenter();
	//析构函数
	virtual ~CDlgTaskPlazaCenter();

	//网络事件
public:
	//连接事件
	virtual bool OnEventMissionLink(INT nErrorCode);
	//关闭事件
	virtual bool OnEventMissionShut(BYTE cbShutReason);
	//读取事件
	virtual bool OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize);

	//功能函数
protected:
	//加载任务
	virtual VOID PerformLoadTask();
	//领取任务
	virtual VOID PerformTakeTask();
	//领取奖励
	virtual VOID PerformRewardTask();

};

////////////////////////////////////////////////////////////////////////////////////////////////////

//房间任务
class CDlgTaskServerCenter : public CDlgTaskCenter
{
	//变量定义
protected:
	ITCPSocket *					m_pITCPSocket;						//网络接口

	//函数定义
public:
	//构造函数
	CDlgTaskServerCenter();
	//析构函数
	virtual ~CDlgTaskServerCenter();

	//功能函数
protected:
	//加载任务
	virtual VOID PerformLoadTask();
	//领取任务
	virtual VOID PerformTakeTask();
	//领取奖励
	virtual VOID PerformRewardTask();

	//功能函数
public:
	//配置任务
	VOID Initialization(ITCPSocket * pITCPSocket);
	//任务消息
	bool OnTaskMessage(WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
#endif
