#ifndef DLG_DLGFEEDBACK_HEAD_FILE
#define DLG_DLGFEEDBACK_HEAD_FILE
#pragma once
//////////////////////////////////////////////////////////////////////////////////

//反馈窗口
class CDlgFeedBack : public CFGuiDialog
{
	//函数定义
public:
	//构造函数
	CDlgFeedBack();
	//析构函数
	virtual ~CDlgFeedBack();

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

	//web调用
public:
	//JS弹框
	static jsValue JS_CALL js_msgBox(jsExecState es);
};

//////////////////////////////////////////////////////////////////////////////////

#endif