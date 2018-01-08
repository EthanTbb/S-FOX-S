#ifndef DLG_SEARCH_TABLE_HEAD_FILE
#define DLG_SEARCH_TABLE_HEAD_FILE
#pragma once

//////////////////////////////////////////////////////////////////////////
//类说明
class CTableViewFrame;
class CPlazaViewServer;

//////////////////////////////////////////////////////////////////////////

//搜索窗口
class CDlgSearchTable : public CFGuiDialog
{
	//函数定义
public:
	//构造函数
	CDlgSearchTable();
	//析构函数
	virtual ~CDlgSearchTable();

	//重载函数
protected:
	//消息解释
	virtual BOOL PreTranslateMessage(MSG * pMsg);
	//配置函数
	virtual BOOL OnInitDialog();
	//确定函数
	virtual VOID OnOK();
	//取消函数
	virtual VOID OnCancel();

	//FGui重载
public:
	//初始函数
	virtual void InitControlUI();
	//皮肤函数
	virtual LPCTSTR GetSkinFile();
	//消息函数
	virtual void Notify(TNotifyUI &  msg);	

	//功能函数
public:
	//设置桌子
	VOID SetTableViewFrame(ITableViewFrame * pITableViewFrame);
	//设置房间
	VOID SetServerViewItem(CPlazaViewServer * pServerViewItem);

	//用户函数
protected:
	//快速加入
	VOID OnBnClickedAutoJoin();

	//接口变量
protected:
	ITableViewFrame *				m_pITableViewFrame;					//桌子接口
	CPlazaViewServer *				m_pServerViewItem;					//游戏房间
	WORD							m_wResultTableID;					//结果桌子
};

//////////////////////////////////////////////////////////////////////////

#endif