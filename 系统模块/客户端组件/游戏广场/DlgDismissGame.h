#ifndef DLG_DISMISS_GAME_HEAD
#define DLG_DISMISS_GAME_HEAD
#pragma once

///////////////////////////////////////////////////////////////////////////////////////////
//解散游戏
class CDlgDismissGame : public CSkinDialog
{
	//变量定义
protected:
	WORD							m_wDismissTable;					//踢出提示

	//控制变量
protected:
	CSkinButton						m_btOK;								//确定按钮
	CSkinButton						m_btCancel;							//取消按钮

	//函数定义
public:
	//构造函数
	CDlgDismissGame(CWnd* pParent = NULL);
	//析构函数
	virtual ~CDlgDismissGame();

	//重载函数
protected:
	//初始化窗口
	virtual BOOL OnInitDialog();
	//数据交换
	virtual void DoDataExchange(CDataExchange* pDX);
	
	//功能函数
public:
	//获取桌号
	WORD GetDismissTableNum(){ return m_wDismissTable; }

	//消息函数
public:
	//确认按钮
	afx_msg void OnBnClickedOk();

	DECLARE_MESSAGE_MAP()	
};
///////////////////////////////////////////////////////////////////////////////////////////
#endif

