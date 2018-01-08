#ifndef DLG_DLGINDIVIDUAL_HEAD_FILE
#define DLG_DLGINDIVIDUAL_HEAD_FILE

#pragma once

//////////////////////////////////////////////////////////////////////////////////

//个人中心
class CDlgIndividual : public CFGuiDialog, public CMissionItem
{
	//函数定义
public:
	//构造函数
	CDlgIndividual();
	//析构函数
	virtual ~CDlgIndividual();

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
	//皮肤名称
	virtual LPCTSTR GetSkinFile();
	//消息提醒
	virtual void Notify(TNotifyUI &  msg);

	//网络事件
public:
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
	//辅助函数
protected:
	//查询资料
	void OnMissionLinkIndividualLoad();
	//修改资料
	void OnMissionLinkIndividualModify();
	//实名认证
	void OnMissionLinkIndividualAuth();
	//登录密码
	void OnMissionLinkLogonModify();
	//银行密码
	void OnMissionLinkInsureModify();
	//银行设置
	void OnMissionLinkInsureSet();

	//功能函数
public:
	//更新界面
	void UpdateUI();
	//更新数据
	void UpdateData();
	//加载数据
	void LoadDataFromMemory();
	//更新头像
	void UpdateUserFace();
	//设置字符
	VOID SetControlItemText(LPCTSTR pszControlName,LPCTSTR pszText);
	//获取字符
	VOID GetControlItemText(LPCTSTR pszControlName, TCHAR szString[], WORD wMaxCount);
	//密码强度
	VOID SetPassLevel(LPCTSTR szPowerScale, LPCTSTR szPassControl);
	//设置焦点
	VOID SetControlFocus(LPCTSTR pszControlName);
	//验证账户
	bool EfficacyStandard();
	//验证详细
	bool EfficacyExtendInfo();
	//验证实名
	bool EfficacyReal();
	//验证登录
	bool EfficacyModifyLogonPassward();
	//验证银行
	bool EfficacyModifyInsurePassward();
	//验证银行
	bool EfficacyOpenInsurePassward();

	//功能函数
public:
	//设置页面
	void SetTabSelectedIndex(BYTE cbIndex = 0);

	//任务变量
protected:
	BYTE							m_byMissionType;						//任务类型
	CMissionManager					m_MissionManager;						//任务管理

	//页面变量
protected:
	BYTE							m_cbSelectedIndex;						//激活页面
	tagCustomFaceInfo96				m_CustomFaceInfo96;						//自定义的头像96*96
};

//////////////////////////////////////////////////////////////////////////////////

#endif