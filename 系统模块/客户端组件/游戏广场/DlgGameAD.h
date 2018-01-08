#ifndef DLG_DLGGAMEAD_HEAD_FILE
#define DLG_DLGGAMEAD_HEAD_FILE
#pragma once
//////////////////////////////////////////////////////////////////////////////////

//反馈窗口
class CDlgGameAD : public CFGuiDialog
{
	//函数定义
public:
	//构造函数
	CDlgGameAD();
	//析构函数
	virtual ~CDlgGameAD();

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
};

//////////////////////////////////////////////////////////////////////////////////

#endif