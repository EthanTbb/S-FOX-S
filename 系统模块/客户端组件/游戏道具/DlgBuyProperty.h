#ifndef DLG_BUY_PROPERTY_HEAD_FILE
#define DLG_BUY_PROPERTY_HEAD_FILE

#pragma once

#include "GamePropertyHead.h"

//////////////////////////////////////////////////////////////////////////////////

//类说明
class CGamePropertyBase;

//////////////////////////////////////////////////////////////////////////////////

//道具购买
class CDlgBuyProperty : public CSkinDialog
{
	//友元定义
	friend class CGamePropertyBase;

	//变量定义
protected:
	LONGLONG                        m_lCanBuyCount;                     //最大数目 

	//接口变量
protected:
	CGamePropertyBase *				m_pGamePropertyItem;				//道具对象
	IGamePropertySink *				m_pIGamePropertySink;				//回调接口

	//静态变量
protected:
	static CDlgBuyProperty *		m_pDlgBuyProperty;					//对象指针

	//函数定义
public:
	//构造函数
	CDlgBuyProperty();
	//析构函数
	virtual ~CDlgBuyProperty();

	//重置函数
protected:
	//消息解释
	virtual BOOL PreTranslateMessage(MSG * pMsg);
	//创建函数
	virtual BOOL OnInitDialog();
	//确定函数
	virtual VOID OnOK();
	//取消消息
	virtual VOID OnCancel();

	//重载函数
public:
	//初始控件
	virtual VOID InitControlUI();
	//消息提醒
    virtual VOID Notify(TNotifyUI &  msg);	
	//皮肤名称
	virtual LPCTSTR GetSkinFile() { return TEXT("DlgBuyProperty.xml"); } 

	//绘画函数
protected:
	//绘画消息
	virtual VOID OnDrawClientAreaEnd(CDC * pDC, INT nWidth, INT nHeight);

	//功能函数
protected:
	//更新状态
	VOID UpdateCtrlStatus();

	//静态函数
protected:
	//显示窗口
	static VOID ShowBuyPropertyWnd(CGamePropertyBase * pGamePropertyItem, IGamePropertySink * pIGamePropertySink);
	//执行命令
	static VOID OnEventPerformCommand(WORD wCommandID,VOID * pCmdData);

	//消息函数
protected:
	//改变事件
	VOID OnEnChangePropertyCount();

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

#endif