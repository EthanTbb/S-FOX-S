#ifndef DLG_DLGBAGUSE_HEAD_FILE
#define DLG_DLGBAGUSE_HEAD_FILE

#pragma once

//////////////////////////////////////////////////////////////////////////////////

//道具使用
class CDlgBagUse : public CFGuiDialog
{
	//函数定义
public:
	//构造函数
	CDlgBagUse();
	//析构函数
	virtual ~CDlgBagUse();

	void SetProperty(const tagBackpackProperty* pBackpackProperty);
	
	//获取个数
	int GetUseCount();
	//是否使用
	bool IsUseProperty();
	//是否赠送
	bool IsGiveProperty();

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
	
	//重载函数
public:
	//皮肤名称
	virtual LPCTSTR GetSkinFile();

private:
	tagBackpackProperty		m_BackpackProperty;
	CNumberUI*				m_pNumberUseCount;
	CButtonUI*				m_pButtonIncrease;
	CButtonUI*				m_pButtonDecrease;
	CLabelUI*				m_pLabelUseAfterCount;
	int						m_nUseCount;
	bool                    m_bIsUseProperty;
	bool					m_bIsGiveProperty;
};

//////////////////////////////////////////////////////////////////////////////////

#endif