#ifndef DLG_DLGBAGPRESEND_HEAD_FILE
#define DLG_DLGBAGPRESEND_HEAD_FILE

#pragma once
enum PRESEND_MODE
{
	PRESEND_NICKNAME,
	PRESEND_GAMEID
};
//////////////////////////////////////////////////////////////////////////////////

//背包道具使用
class CDlgBagPresent: public CFGuiDialog
{
	//函数定义
public:
	//构造函数
	CDlgBagPresent();
	//析构函数
	virtual ~CDlgBagPresent();

	void SetProperty(const tagBackpackProperty* pBackpackProperty);
	
	//获取得到的个数
	int GetUseCount();

	//是否赠送道具
	bool IsGiveProperty();

	//赠送模式
	PRESEND_MODE GetPresendMode();

	//获得输入文字
	CStdString GetEditReceiveName();

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
	CEditUI*				m_pEditReceiveName;
	int						m_nUseCount;
	bool					m_bIsGiveProperty;
	PRESEND_MODE			m_ePresendMode;
};

//////////////////////////////////////////////////////////////////////////////////

#endif