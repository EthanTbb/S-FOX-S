#ifndef DLG_DLGBAGRECORD_HEAD_FILE
#define DLG_DLGBAGRECORD_HEAD_FILE

#pragma once

//////////////////////////////////////////////////////////////////////////////////

//我的背包
class CDlgBagRecord : public CFGuiDialog
{
	//函数定义
public:
	//构造函数
	CDlgBagRecord();
	//析构函数
	virtual ~CDlgBagRecord();

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
	//初始函数
	virtual void InitControlUI();
	//消息提醒
    virtual void Notify(TNotifyUI &  msg);
	//皮肤名称
	virtual LPCTSTR GetSkinFile();
};

//////////////////////////////////////////////////////////////////////////////////
#endif