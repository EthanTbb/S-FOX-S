#ifndef DLG_DLGBAG_HEAD_FILE
#define DLG_DLGBAG_HEAD_FILE

#pragma once

#include "DlgBagPresent.h"
class CDlgBagUse;
//////////////////////////////////////////////////////////////////////////////////

//我的背包
class CDlgBagMain : public CFGuiDialog
{
	//函数定义
public:
	//构造函数
	CDlgBagMain();
	//析构函数
	virtual ~CDlgBagMain();
	//是否使用
	bool IsUseProperty();
	//是否赠送
	bool IsGiveProperty();
	//结束窗口
	void EndDialog();

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
	virtual void InitControlUI();
	//消息提醒
    virtual void Notify(TNotifyUI &  msg);

	//缓冲数据
	VOID InitData();

	VOID LoadDataFromMemory();

	VOID LoadDataFromUI();

	VOID UpdateData();

	//界面操控
	VOID InitUI();

	VOID UpdateUI();
	//清除数据
	VOID Clear();

	//创建道具
	CVerticalLayoutUI* CreateItem(tagBackpackProperty& BackpackProperty);

	//更新道具
	VOID UpdateItemUI(CContainerUI* pVerticalLayoutUI, const tagBackpackProperty& BackpackProperty);

	//添加类型
	bool AddByType(CContainerUI* pVerticalLayoutUI, const int nType=-1);

	//查找UI
	CContainerUI* FindUIItem(CPageLayoutUI* pPageLayoutUI, const DWORD dwPropID);

	//更新道具
	bool UpdateProperty(const tagBackpackProperty& BackpackProperty);

	//显示使用
	virtual INT_PTR ShowUseProperty(const tagBackpackProperty* pBackpackProperty);

	//减少道具
	bool ReduceProperty(const DWORD dwPropID, const DWORD dwPropCount);

	//成功消息
	void ShowSuccessMessage(const CMD_GP_S_PropertyUse* pPropertyUse);
	
	//重载函数
public:
	//皮肤名称
	virtual LPCTSTR GetSkinFile();

	//控件变量
private:
	CPageLayoutUI*				m_pPageLayoutUI[5];
	CDlgBagUse*					m_pDlgBagUse;

public:
	vector<tagBackpackProperty> m_vBackPack;
	tagBackpackProperty			m_curSelectProp;		//选中道具
	bool						m_bIsUseProperty;
	bool						m_bIsGiveProperty;
	CStdString					m_sGiveReceiveName;		//赠送昵称
	DWORD						m_dwGiveReceiveID;		//赠送I D
	PRESEND_MODE				m_ePresendMode;			//赠送方式
};

//////////////////////////////////////////////////////////////////////////////////

//我的背包(大厅)
class CDlgBagPlaza : public CDlgBagMain, public CMissionItem
{
	//函数定义
public:
	//构造函数
	CDlgBagPlaza();
	//析构函数
	virtual ~CDlgBagPlaza();

	//重载函数
protected:
	//创建函数
	virtual BOOL OnInitDialog();

	//网络事件
private:
	//连接事件
	virtual bool OnEventMissionLink(INT nErrorCode);
	//关闭事件
	virtual bool OnEventMissionShut(BYTE cbShutReason);
	//读取事件
	virtual bool OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize);

public:
	//使用道具
	virtual INT_PTR ShowUseProperty(const tagBackpackProperty* pBackpackProperty);
	
	//激活任务
	bool AvtiveMission();
	bool QuerySendPresentMisson();

private:
	CMissionManager				m_MissionManager;		//任务管理
	bool						m_bQueryBackpack;		//查询背包
	bool						m_bUseBackpackProp;		//使用物品
	bool						m_bGiveBackpackProp;	//赠送物品
	bool						m_bGetSendPresent;		//获取赠送
};

///////////////////////////////////////////////////////////////////////////////


//我的背包
class CDlgBagServer : public CDlgBagMain
{
	//函数定义
public:
	//构造函数
	CDlgBagServer();
	//析构函数
	virtual ~CDlgBagServer();

	//重载函数
protected:
	//创建函数
	virtual BOOL OnInitDialog();

public:
	//查询事件
	VOID PerformQueryInfo();
	//使用物品
	VOID PerformUseProperty();
	//查询赠送
	VOID PerformQuerySendPresent();

	//功能函数
public:
	//配置
	VOID Initialization(ITCPSocket * pITCPSocket, IClientUserItem * pIMySelfUserItem);
	//背包消息
	bool OnServerBagMessage(WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize);

public:
	//显示使用
	virtual INT_PTR ShowUseProperty(const tagBackpackProperty* pBackpackProperty);
	//查询赠送
	bool QuerySendPresentMisson();

	//变量定义
protected:
	ITCPSocket *					m_pITCPSocket;						//网络接口
	IClientUserItem *				m_pIMySelfUserItem;					//自己指针
	bool							m_bQueryBackpack;					//查询背包
	bool							m_bUseBackpackProp;					//使用物品
};

#endif