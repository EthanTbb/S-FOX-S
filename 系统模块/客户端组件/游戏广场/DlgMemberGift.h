#ifndef DLG_MEMBER_GIFT_HEAD_FILE
#define DLG_MEMBER_GIFT_HEAD_FILE

#pragma once

//////////////////////////////////////////////////////////////////////////////////

//会员窗口
class CDlgMemberGift : public CFGuiDialog
{
	//函数定义
public:
	//构造函数
	CDlgMemberGift();
	//析构函数
	virtual ~CDlgMemberGift();

	//重载函数
protected:
	//创建函数
	virtual BOOL OnInitDialog();
	//确定函数
	virtual VOID OnOK();
	//取消消息
	virtual VOID OnCancel();

	//重载函数
public:
	//初始控件
	virtual void InitControlUI();
	//消息提醒
    virtual void Notify(TNotifyUI &  msg);
	//皮肤名称
	virtual LPCTSTR GetSkinFile();

public:
	//加载数据
	void LoadDataFromMemory();

	//创建页面
	void CreateUI();

	//刷新界面
	void UpdateUI();
public:
	//设置领取
	void SetEnableReceive(bool EnableReceive);

public:
	vector<CGamePropertyItem *>		m_vGamePropertyList;
	vector<CGamePropertySubItem*>	m_vGamePropertySubList;		
	bool							m_bEnableReceive;
};

//////////////////////////////////////////////////////////////////////////////////

#endif