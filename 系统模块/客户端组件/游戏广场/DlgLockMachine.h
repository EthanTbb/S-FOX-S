#ifndef DLG_LOCK_MACHINE_HEAD_FILE
#define DLG_LOCK_MACHINE_HEAD_FILE
#pragma once
//////////////////////////////////////////////////////////////////////////////////

//锁机窗口
class CDlgLockMachine : public CFGuiDialog, public CMissionItem
{
public:
	//构造函数
	CDlgLockMachine();
	//析构函数
	virtual ~CDlgLockMachine();

	//MFC重载
protected:
	//初始函数
	virtual BOOL OnInitDialog();
	//确定函数
	virtual VOID OnOK();
	//取消函数
	virtual VOID OnCancel();

	//FGui重载
public:
	//初始函数
	virtual void InitControlUI();
	//皮肤函数
	virtual LPCTSTR GetSkinFile();
	//消息函数
	virtual void Notify(TNotifyUI &  msg);	

	//界面消息
public:
	//锁机函数
	void OnNotifyLock(TNotifyUI &  msg);
	//解锁函数
	void OnNotifyUnLock(TNotifyUI &  msg);

	//网络事件
public:
	//开始任务
	virtual VOID OnMissionStart();
	//终止任务
	virtual VOID OnMissionConclude();
	//连接事件
	virtual bool OnEventMissionLink(INT nErrorCode);
	//关闭事件
	virtual bool OnEventMissionShut(BYTE cbShutReason);
	//读取事件
	virtual bool OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize);

	//事件消息
public:	
	//终止会话
	bool ConcludeMission();
	//激活会话
	bool AvtiveMission(BYTE byMissionType);
	//失败会话
	void OnMissionLinkError(INT nErrorCode);
	//失败会话
	void OnMissionShutError(BYTE cbShutReason);

	//网络消息
public:	
	//锁机会话
	void OnMissionLinkLock();
	//锁机成功
	void OnMissionReadLockSuccess(VOID * pData, WORD wDataSize);
	//锁机失败
	void OnMissionReadLockFailure(VOID * pData, WORD wDataSize);


	//用户消息
public:
	//加载数据
	void LoadDataFromUI();
	//加载数据
	void LoadDataFromMemory();
	//更新数据
	void UpdateDataToUI();
	//更新数据
	void UpdateDataToMemory();
	//验证数据
	bool EfficacyData();
	//执行锁机
	void OnMachineLock(BYTE byLock);
	//发送数据
	void OnSendData();

	//变量定义
protected:
	BYTE							m_byMachine;						//绑定标志
	TCHAR							m_szPassword[LEN_PASSWORD];			//用户密码
	//组件变量
protected:
	CMissionManager					m_MissionManager;					//任务管理
	BYTE							m_byMissionType;					//会话标识

};

//////////////////////////////////////////////////////////////////////////////////

#endif