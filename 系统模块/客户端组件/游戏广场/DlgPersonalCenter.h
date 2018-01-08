#ifndef DLG_DLGPERSONALCENTER_HEAD_FILE
#define DLG_DLGPERSONALCENTER_HEAD_FILE

#pragma once

//////////////////////////////////////////////////////////////////////////////////

//帮助窗口
class CDlgPersonalCenter : public CFGuiDialog
{
	//函数定义
public:
	//构造函数
	CDlgPersonalCenter();
	//析构函数
	virtual ~CDlgPersonalCenter();

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
	virtual LPCTSTR GetSkinFile() { return TEXT("DlgPersonalCenter.xml"); } 

};

//////////////////////////////////////////////////////////////////////////////////

#endif