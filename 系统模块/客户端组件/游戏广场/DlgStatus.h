#ifndef DLG_STATUS_HEAD_FILE
#define DLG_STATUS_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "Resource.h"

//////////////////////////////////////////////////////////////////////////////////

//状态接口
interface IStatusViewSink
{
	//取消连接
	virtual VOID OnStatusCancel()=NULL;
};

//////////////////////////////////////////////////////////////////////////////////

//状态窗口
class CDlgStatus : public CFGuiDialog
{

	//函数定义
public:
	//构造函数
	CDlgStatus();
	//析构函数
	virtual ~CDlgStatus();

	//重载函数
protected:
	//消息解释
	virtual BOOL PreTranslateMessage(MSG * pMsg);
	//创建函数
	virtual BOOL OnInitDialog();

    //接口函数
public:
	//初始控件
	virtual void InitControlUI();
	//消息提醒
	virtual void Notify(TNotifyUI &  msg);	
	//皮肤名称
	virtual LPCTSTR GetSkinFile();

	//绘画接口
public:
	//开始绘画
	virtual void OnBeginPaintWindow(HDC hDC);

	//功能函数
public:
	//关闭窗口
	VOID HideStatusWindow();
	//设置消息
	VOID ShowStatusWindow(LPCTSTR pszDescribe);
	//设置接口
	VOID SetStatusViewSink(IStatusViewSink * pIStatusViewSink);

	//消息映射
protected:
	//取消按钮
	VOID OnBnClickedGiveUp();
	//时间消息
	VOID OnTimer(UINT nIDEvent);

	DECLARE_MESSAGE_MAP()

	//状态变量
protected:
	UINT							m_uImageIndex;						//位图位置
	bool							m_bFinished;						//完成标志
	CString							m_strDescribe;						//描述信息

	//接口变量
protected:
	IStatusViewSink *				m_pIStatusViewSink;					//回调接口

	//控件变量
protected:
	CButtonUI *						m_pbtGiveUp;						//取消按钮
	CSkinLayered					m_SkinLayered;						//分层窗口
};

//////////////////////////////////////////////////////////////////////////////////

#endif