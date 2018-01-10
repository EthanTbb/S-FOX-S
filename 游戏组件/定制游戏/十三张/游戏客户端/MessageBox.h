#pragma once

#include "Stdafx.h"
#include "Resource.h"
#include "CardControl.h"

//////////////////////////////////////////////////////////////////////////

#define	IDM_MSG_INFO			(WM_USER+1000)							  //确定消息
//////////////////////////////////////////////////////////////////////////


//游戏配置
class CMessageBox : public CDialog
{
	//变量定义
protected:
	CString							m_szMsg;								//字符串
	UINT							m_nTimerCount;							//剩余时间

	//控件变量
protected:
	CSkinButton						m_btOK;									//确定按钮
	CSkinButton						m_btClose;								//取消按钮

	//资源变量
protected:
	CBitImage						m_ImageBack;							//背景图

	//函数定义
public:
	//构造函数
	CMessageBox();
	//析构函数
	virtual ~CMessageBox();

	//功能函数
public:
	//设置提示
	void ShowMessage(LPCTSTR pszMsg);

	//重载函数
protected:
	//控件绑定
	virtual void DoDataExchange(CDataExchange * pDX);
	//初始化函数
	virtual BOOL OnInitDialog();
	//确定消息
	virtual void OnOK();
	//取消消息
	virtual void OnCancel();

	//消息映射
protected:

	//重画函数
	afx_msg void OnPaint();
	//鼠标消息
	afx_msg void OnLButtonDown(UINT nFlags, CPoint Point);
	//背景消息
	afx_msg BOOL OnEraseBkgnd(CDC * pDC);
	//定时消息
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////
