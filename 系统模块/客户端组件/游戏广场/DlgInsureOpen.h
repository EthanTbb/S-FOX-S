#ifndef DLG_DLGINSUREOPEN_HEAD_FILE
#define DLG_DLGINSUREOPEN_HEAD_FILE

#pragma once
class CDlgInsure;
//////////////////////////////////////////////////////////////////////////////////

//开通银行
class CDlgInsureOpen: public CFGuiDialog
{
	//函数定义
public:
	//构造函数
	CDlgInsureOpen();
	//析构函数
	virtual ~CDlgInsureOpen();

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
	//消息提醒
    virtual void Notify(TNotifyUI &  msg);

	
	//重载函数
public:
	//皮肤名称
	virtual LPCTSTR GetSkinFile();

};

//////////////////////////////////////////////////////////////////////////////////

#endif