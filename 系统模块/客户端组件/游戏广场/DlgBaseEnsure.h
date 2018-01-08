#ifndef DLG_BASE_ENSURE_HEAD_FILE
#define DLG_BASE_ENSURE_HEAD_FILE
#pragma once

///////////////////////////////////////////////////////////////////////////////////////////////////
//低保窗口
class CDlgBaseEnsure : public CFGuiDialog,public CMissionItem
{
	//控制变量
protected:
	bool							m_bLoadBaseEnsure;					//加载低保
	bool							m_bTakeBaseEnsure;					//领取低保
	//内存数据
protected:
	tagBaseEnsureParameter			m_BaseEnsureParameter;				//低保参数
	//组件变量
protected:
	CMissionManager					m_MissionManager;					//任务管理

	//函数定义
public:
	//构造函数
	CDlgBaseEnsure();
	//析构函数
	virtual ~CDlgBaseEnsure();

    //接口函数
public:
	//初始控件
	virtual void InitControlUI();
	//消息提醒
	virtual void Notify(TNotifyUI &  msg);	
	//重载函数
public:
	//皮肤名称
	virtual LPCTSTR GetSkinFile();
	//重载函数
protected:
	//创建函数
	virtual BOOL OnInitDialog();
	//确定函数
	virtual VOID OnOK();
	//取消消息
	virtual VOID OnCancel();
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

	//辅助变量
protected:
	//加载低保
	virtual VOID PerformLoadBaseEnsure();
	//领取低保
	virtual VOID PerformTakeBaseEnsure();
public:
	//刷新控件
	void UpdateUI();
};
///////////////////////////////////////////////////////////////////////////////////////////////////

//低保窗口
class CDlgBaseEnsureServer : public CDlgBaseEnsure
{
	//函数定义
public:
	//构造函数
	CDlgBaseEnsureServer();
	//析构函数
	virtual ~CDlgBaseEnsureServer();

	//辅助变量
protected:
	//加载低保
	virtual VOID PerformLoadBaseEnsure();
	//领取低保
	virtual VOID PerformTakeBaseEnsure();

	//功能函数
public:
	//配置
	VOID Initialization(ITCPSocket * pITCPSocket, IClientUserItem * pIMySelfUserItem);
	//签到消息
	bool OnServerBaseEnsureMessage(WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize);

	//变量定义
protected:
	ITCPSocket *					m_pITCPSocket;						//网络接口
	IClientUserItem *				m_pIMySelfUserItem;					//自己指针

};

///////////////////////////////////////////////////////////////////////////////////////////////////
#endif
