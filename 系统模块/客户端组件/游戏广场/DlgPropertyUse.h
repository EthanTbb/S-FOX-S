#ifndef DLG_DLGPROPERTYUSE_HEAD_FILE
#define DLG_DLGPROPERTYUSE_HEAD_FILE

#pragma once

//////////////////////////////////////////////////////////////////////////////////

//道具使用
class CDlgPropertyUseServer : public CFGuiDialog
{
	//函数定义
public:
	//构造函数
	CDlgPropertyUseServer();
	//析构函数
	virtual ~CDlgPropertyUseServer();

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
	//皮肤名称
	virtual LPCTSTR GetSkinFile();
	//初始控件
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
	
	//创建UI
	CVerticalLayoutUI* CreateItem(tagBackpackProperty& BackpackProperty);

	//查找UI
	CContainerUI* FindUIItem(CPageLayoutUI* pPageLayoutUI, const DWORD dwPropID);

	//更新道具
	VOID UpdateItemUI(CContainerUI* pVerticalLayoutUI, const tagBackpackProperty& BackpackProperty);
	
	//更新道具
	bool UpdateProperty(const tagBackpackProperty& BackpackProperty);

public:
	//配置接口
	VOID Initialization(ITCPSocket * pITCPSocket, IClientUserItem* pIMySelfUserItem, IClientUserItem* pTargetUserItem);
	//消息函数
	bool OnServerPropertyUseMessage(WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize);

	//变量定义
protected:
	ITCPSocket *				m_pITCPSocket;						//网络接口
	IClientUserItem*			m_pIMySelfUserItem;					//自己指针
	IClientUserItem*			m_pTargetUserItem;					//目标指针
	vector<tagBackpackProperty> m_vBackPack;
	tagBackpackProperty			m_curSelectProp;					//选中道具
	CPageLayoutUI*				m_pPageLayoutUI;
};

//////////////////////////////////////////////////////////////////////////////////

#endif