#ifndef DLG_INPUT_PASSWD_HEAD_FILE
#define DLG_INPUT_PASSWD_HEAD_FILE

#pragma once

#include "Stdafx.h"

//////////////////////////////////////////////////////////////////////////////////

//房间密码
class CDlgInputPassword : public CFGuiDialog
{
	//变量定义
protected:
	TCHAR							m_szPassword[LEN_PASSWORD];			//用户密码

	//控件变量
protected:
	CButtonUI *						m_pbtOk;							//确定按钮
	CButtonUI *                     m_pbtCancel;                        //取消按钮	
	CEditUI *						m_pEditPassword;					//用户密码

	//函数定义
public:
	//构造函数
	CDlgInputPassword();
	//析够函数
	virtual ~CDlgInputPassword();

	//重载函数
protected:
	//皮肤名称
	virtual LPCTSTR GetSkinFile();
	//创建函数
	virtual BOOL OnInitDialog();
	//确定函数
	virtual VOID OnOK();
	//取消按钮
	virtual VOID OnCancel();

    //接口函数
public:
	//初始控件
	virtual void InitControlUI();
	//消息提醒
	virtual void Notify(TNotifyUI &  msg);	

	//功能函数
public:
	//获取密码
	VOID GetServerPasswd(TCHAR szServerPasswd[LEN_PASSWORD]);
};

#endif