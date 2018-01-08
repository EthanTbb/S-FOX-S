#ifndef DLG_ENQUIRE_HEAD_FILE
#define DLG_ENQUIRE_HEAD_FILE
#pragma once
//////////////////////////////////////////////////////////////////////////////////

//关闭窗口
class CDlgEnquire : public CFGuiDialog
{

	//函数定义
public:
	//构造函数
	CDlgEnquire();
	//析构函数
	virtual ~CDlgEnquire();

	//MFC重载
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

	//按钮消息
protected:
	//关闭大厅
	VOID OnBnClickedClosePlaza();
	//取消操作
	VOID OnBnClickedCloseServer();
	//切换帐号
	VOID OnBnClickedSwitchAccounts();

};

//////////////////////////////////////////////////////////////////////////////////

#endif