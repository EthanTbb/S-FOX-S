#ifndef DLG_DLGRECHARGE_HEAD_FILE
#define DLG_DLGRECHARGE_HEAD_FILE
#pragma once

//////////////////////////////////////////////////////////////////////////////////

//充值窗口
class CDlgRecharge : public CFGuiDialog
{
	//函数定义
public:
	//构造函数
	CDlgRecharge();
	//析构函数
	virtual ~CDlgRecharge();

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

};

//////////////////////////////////////////////////////////////////////////////////

#endif