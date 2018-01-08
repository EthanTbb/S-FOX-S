#ifndef DLG_DLGHELP_HEAD_FILE
#define DLG_DLGHELP_HEAD_FILE
#pragma once
//////////////////////////////////////////////////////////////////////////////////

//帮助窗口
class CDlgHelp : public CFGuiDialog
{
	//函数定义
public:
	//构造函数
	CDlgHelp();
	//析构函数
	virtual ~CDlgHelp();

	//重载函数
protected:
	//创建函数
	virtual BOOL OnInitDialog();
	//确定函数
	virtual VOID OnOK();
	//取消消息
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