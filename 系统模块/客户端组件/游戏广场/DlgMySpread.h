#ifndef DLG_MYSPREAD_HEAD_FILE
#define DLG_MYSPREAD_HEAD_FILE
#pragma once
///////////////////////////////////////////////////////////////////////////////////////////////////

//推广窗口
class CDlgMySpread : public CFGuiDialog,public CMissionItem
{
public:
	//构造函数
	CDlgMySpread();
	//析构函数
	virtual ~CDlgMySpread();

    //MFC重载
protected:
	//创建函数
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
	//复制函数
	void OnNotifyCopy(TNotifyUI &  msg);
	//链接函数
	void OnNotifyLink(TNotifyUI &  msg);

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
	//加载会话
	void OnMissionLinkLoad();
	//加载会话
	void OnMissionReadLoad(VOID * pData, WORD wDataSize);

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
	//加载奖励
	void PerformLoadSpreadReward();

	//配置变量
protected:
	tagUserSpreadInfo				m_UserSpreadInfo;					//推广信息
	TCHAR							m_szSpreadLink[MAX_PATH];			//推广链接
	//组件变量
protected:
	CMissionManager					m_MissionManager;					//任务管理
	BYTE							m_byMissionType;					//会话标识

};
///////////////////////////////////////////////////////////////////////////////////////////////////
#endif
