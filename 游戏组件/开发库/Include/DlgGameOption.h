#ifndef DLG_GAME_OPTION_HEAD_FILE
#define DLG_GAME_OPTION_HEAD_FILE

#pragma once

#include "GameOptionItem.h"
#include "GameFrameHead.h"

//////////////////////////////////////////////////////////////////////////


//游戏帮助
class GAME_FRAME_CLASS CDlgGameOption : public CFGuiDialog
{
	//函数定义
public:
	//构造函数
	CDlgGameOption();
	//析构函数
	virtual ~CDlgGameOption();

	//功能函数
public:
	//游戏配置
	bool CreateGameOption(IUnknownEx * pIUnknownEx[], WORD wItemCount);

	//MFC重载
protected:
	//创建函数
	virtual BOOL OnInitDialog();
	//确定函数
	virtual VOID OnOK();
	//取消消息
	virtual VOID OnCancel();

	//FGUI重载
public:
	//初始控件
	virtual VOID InitControlUI();
	//消息提醒
	virtual void Notify(TNotifyUI &  msg);
	//皮肤名称
	virtual LPCTSTR GetSkinFile();

	//用户函数
public:

	//缓冲数据
	VOID InitData();

	VOID LoadDataFromMemory();

	VOID LoadDataFromUI();

	VOID UpdateData();

	//界面操控
	VOID InitUI();

	VOID UpdateUI();

	//界面逻辑
	VOID OnDefault();

	VOID DoApply();

public:
	//缓冲数据
	bool                            m_bMuteStatuts;                     //静音状态
	bool							m_bAllowSound;						//允许声音
	bool							m_bAllowBackGroundSound;			//允许背景声音
	bool							m_bFullGameView;					//全屏模式
	bool							m_bNotifyUserInOut;					//进出消息
	bool							m_bDebarDetestChat;					//屏蔽消息
	bool							m_bAllowLookon;						//允许旁观
	bool							m_bAllowFriendLookon;				//允许旁观
	//默认数据
	bool                            m_bDefaultMuteStatuts;                     //静音状态
	bool							m_bDefaultAllowSound;						//允许声音
	bool							m_bDefaultAllowBackGroundSound;			//允许背景声音
	bool							m_bDefaultFullGameView;					//全屏模式
	bool							m_bDefaultNotifyUserInOut;					//进出消息
	bool							m_bDefaultDebarDetestChat;					//屏蔽消息
	bool							m_bDefaultAllowLookon;						//允许旁观
	bool							m_bDefaultAllowFriendLookon;				//允许旁观

};

//////////////////////////////////////////////////////////////////////////

#endif