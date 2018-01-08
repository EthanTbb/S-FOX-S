#ifndef DLG_DLGGAMEAD_HEAD_FILE
#define DLG_DLGGAMEAD_HEAD_FILE
#pragma once
//////////////////////////////////////////////////////////////////////////////////

//充值提示
class CDlgRechargeEnquire : public CFGuiDialog
{
public:
	CString							m_strString;						//提示消息
	//函数定义
public:
	//构造函数
	CDlgRechargeEnquire();
	//析构函数
	virtual ~CDlgRechargeEnquire();

	//重载函数
protected:
	//创建函数
	virtual BOOL OnInitDialog();
	//确定函数
	virtual VOID OnOK();
	//取消消息
	virtual VOID OnCancel();

	//FGUI重载
public:
	//初始函数
	virtual void InitControlUI();
	//皮肤函数
	virtual LPCTSTR GetSkinFile();
	//消息函数
	virtual void Notify(TNotifyUI &  msg);

public:
	VOID UpdateUI();

	INT ShowMessageBox( LPCTSTR pszString);
};

//////////////////////////////////////////////////////////////////////////////////

#endif