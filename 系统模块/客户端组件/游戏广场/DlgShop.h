#ifndef DLG_DLGSHOP_HEAD_FILE
#define DLG_DLGSHOP_HEAD_FILE

#pragma once
#include "ServerListData.h"
#include "DlgShopDetails.h"
//////////////////////////////////////////////////////////////////////////////////

//商城窗口
class CDlgShopMain : public CFGuiDialog, public IGamePropertyAction
{
	//函数定义
public:
	//构造函数
	CDlgShopMain();
	//析构函数
	virtual ~CDlgShopMain();
	//结束创库
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
	//消息提醒
	virtual void Notify(TNotifyUI &  msg);

	//重载函数
public:
	//皮肤名称
	virtual LPCTSTR GetSkinFile();

	//用户函数
public:

	//缓冲数据
	VOID InitData();



	VOID LoadDataFromUI();

	VOID UpdateData();

	//界面操控
	VOID InitUI();

	VOID UpdateUI();

	//加载内存
	void LoadDataFromMemory();
	//创建页面
	void CreateUI();

public:
	//查询事件
	virtual VOID PerformQueryInfo();
	//查询事件
	virtual VOID PerformBuyGameProperty(tagBuyGamePropertyInfo *ptagBuyGamePropertyInfo);
	//使用事件
	virtual VOID PerformUseGameProperty(const tagUseGamePropertyInfo *pPropertyUseInfo);

	//道具属性
public:
	//道具属性
	VOID SetPropertyInfo(CPageLayoutUI* pPageLayoutUI, tagPropertyInfo *ptagPropertyInfo, BYTE cbIndex = 0);
	
	//显示成功消息
	void ShowSuccessMessage(const CMD_GP_S_PropertyUse* pPropertyUse);

	//任务变量
protected:
	//CMissionManager				m_MissionManager;						//任务管理
	bool							m_bQueryProperty;						//查询道具
	bool							m_bBuyProperty;							//购买道具
	bool							m_bUseProperty;							//使用道具
	tagUseGamePropertyInfo			m_PropertyUseInfo;						//使用信息
	
	//控件变量
protected:	
	CDlgShopDetails*				m_pDlgShopDetails;
public:					
	tagBuyGamePropertyInfo			m_tagBuyGamePropertyInfo;

public:
	vector<CGamePropertyTypeItem *> m_vGamePropertyTypeList;
	vector<CGamePropertyRelatItem *> m_vGamePropertyRelatList;
	vector<CGamePropertyItem *> m_vGamePropertyList;
	CGamePropertyTypeItem * m_CurrGamePropertyType;
};

//商城窗口
class CDlgShopPlaza : public CDlgShopMain, public CMissionItem
{
	//函数定义
public:
	//构造函数
	CDlgShopPlaza();
	//析构函数
	virtual ~CDlgShopPlaza();

	//重载函数
protected:
	//创建函数
	virtual BOOL OnInitDialog();

public:
	//查询事件
	virtual VOID PerformQueryInfo();
	//查询事件
	virtual VOID PerformBuyGameProperty(tagBuyGamePropertyInfo *ptagBuyGamePropertyInfo);
	//使用事件
	virtual VOID PerformUseGameProperty(const tagUseGamePropertyInfo *pPropertyUseInfo);

	//网络事件
private:
	//连接事件
	virtual bool OnEventMissionLink(INT nErrorCode);
	//关闭事件
	virtual bool OnEventMissionShut(BYTE cbShutReason);
	//读取事件
	virtual bool OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize);

	//任务变量
protected:
	CMissionManager					m_MissionManager;						//任务管理


};

//房间商城
class CDlgShopServer : public CDlgShopMain
{
	//函数定义
public:
	//构造函数
	CDlgShopServer();
	//析构函数
	virtual ~CDlgShopServer();

	//重载函数
protected:
	//创建函数
	virtual BOOL OnInitDialog();

public:
	//查询事件
	virtual VOID PerformQueryInfo();
	//查询事件
	virtual VOID PerformBuyGameProperty(tagBuyGamePropertyInfo *ptagBuyGamePropertyInfo);
	//使用事件
	virtual VOID PerformUseGameProperty(const tagUseGamePropertyInfo *pPropertyUseInfo);

	//功能函数
public:
	//配置银行
	VOID Initialization(ITCPSocket * pITCPSocket, IClientUserItem * pIMySelfUserItem);
	//银行消息
	bool OnServerShopMessage(WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize);
	
	//变量定义
protected:
	ITCPSocket *					m_pITCPSocket;						//网络接口
	IClientUserItem *				m_pIMySelfUserItem;					//自己指针
};

//////////////////////////////////////////////////////////////////////////////////

#endif