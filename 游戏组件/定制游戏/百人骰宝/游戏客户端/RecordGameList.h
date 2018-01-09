#ifndef RECORD_GAME_LIST_HEAD_FILE
#define RECORD_GAME_LIST_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "../游戏服务器/GameLogic.h"
//////////////////////////////////////////////////////////////////////////

//游戏记录
struct GameRecord
{
	//游戏信息
	WORD							wDrawCount;							//游戏局数	
	BYTE							cbWinerSide;						//胜利玩家
	BYTE							cbPlayerPoint;						//闲家牌点
	BYTE							cbBankerPoint;						//庄家牌点	

	//我的信息
	SCORE						lGameScore;							//游戏成绩	
	SCORE						lMyAddGold;							//游戏下注	

	//下注总量
	SCORE						lDrawTieScore;						//买平总注
	SCORE						lDrawBankerScore;					//买庄总注
	SCORE						lDrawPlayerScore;					//买闲总注
};

//////////////////////////////////////////////////////////////////////////

//游戏记录
class CGameRecord : public CSkinDialog
{
	//控件变量
public:
	CSkinListCtrl					m_RecordGameList;					//游戏列表

	//函数定义
public:
	//构造函数
	CGameRecord();
	//析构函数
	virtual ~CGameRecord();

	//重载函数
protected:
	//控件绑定
	virtual void DoDataExchange(CDataExchange * pDX);
	//初始化函数
	virtual BOOL OnInitDialog();
	//确定消息
	virtual void OnOK() { return; }
	//取消消息
	virtual void OnCancel() { return; }

	//功能函数
public:
	//插入列表
	void FillGameRecourd(GameRecord & GameRecord);

	//消息映射
protected:
	//位置变化
	afx_msg void OnSize(UINT nType, int cx, int cy);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////

#endif
