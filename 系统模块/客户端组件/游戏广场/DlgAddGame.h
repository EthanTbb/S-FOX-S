#ifndef DLG_ADD_GAME_HEAD_FILE
#define DLG_ADD_GAME_HEAD_FILE

#pragma once
//////////////////////////////////////////////////////////////////////////////////
#include "ServerListData.h"

typedef vector<CCheckButtonUI*> VECheckButtonList;	//一个控件行

//////////////////////////////////////////////////////////////////////////
//添加游戏
class CDlgAddGame : public CFGuiDialog
{
	//函数定义
public:
	//构造函数
	CDlgAddGame();
	//析构函数
	virtual ~CDlgAddGame();

	//MFC函数
protected:
	//初始函数
	virtual BOOL OnInitDialog();
	//确定函数
	virtual VOID OnOK();
	//取消消息
	virtual VOID OnCancel();

	//FGui函数
public:
	//初始控件
	virtual void InitControlUI();
	//消息提醒
    virtual void Notify(TNotifyUI &  msg);
	//皮肤名称
	virtual LPCTSTR GetSkinFile();

	//消息函数
public:
	//全选消息
	void OnNotifyAllSelect(TNotifyUI &  msg);
	//选定消息
	void OnNotifySelect(TNotifyUI &  msg);
	//用户函数
public:
	//加载数据
	void LoadDataFromExternal(vector<CGameKindItem *> & vMyGameKind);

	//刷新数据
	void UpdateDataToExternal(vector<CGameKindItem *> & vMyGameKind);

	//加载数据
	void LoadDataFromMemory();

	//创建页面
	void CreateUI();

	//加载数据
	void LoadDataFromUI();

	//刷新界面
	void UpdateUI();

	//辅助函数
private:
	//进程名称
	inline void GetProcessName(CGameKindItem *vMyGameKind,TCHAR szProcessName[LEN_PROCESS]);

protected:
	vector<CGameTypeItem *> m_vGameType;
	vector<CGameKindItem *> m_vGameKind;
    vector<CGameKindItem *> m_vMyGameKind;
	vector<VECheckButtonList> m_vecCheckbutton;
	vector<CCheckButtonUI*> m_vecAllGameCheckList;
};

//////////////////////////////////////////////////////////////////////////////////

#endif