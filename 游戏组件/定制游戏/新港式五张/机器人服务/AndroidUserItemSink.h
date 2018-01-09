#ifndef ANDROID_USER_ITEM_SINK_HEAD_FILE
#define ANDROID_USER_ITEM_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "GameLogic.h"

//////////////////////////////////////////////////////////////////////////



//游戏对话框
class CAndroidUserItemSink : public IAndroidUserItemSink
{


	//加注信息
protected:
	LONG							m_lCellScore;						//游戏底注
	LONGLONG						m_lServiceCharge;					//服务费
	LONGLONG						m_lDrawMaxScore;					//最大下注
	LONGLONG						m_lTurnMaxScore;					//最大下注
	LONGLONG						m_lTurnLessScore;					//最小下注
	LONGLONG						m_lUserScore[GAME_PLAYER];			//用户下注
	LONGLONG						m_lTableScore[GAME_PLAYER];			//桌面下注

	//状态变量
protected:
	BYTE							m_cbCurRound;						//当前轮数
	bool							m_bShowHand;						//梭哈标志
	WORD							m_wCurrentUser;						//当前用户
	BYTE							m_cbPlayStatus[GAME_PLAYER];		//游戏状态
	WORD							m_wBetCount;						//加注次数

	//用户扑克
protected:
	BYTE							m_cbHandCardData[GAME_PLAYER][MAX_COUNT];						

	//控件变量
public:
	IAndroidUserItem *				m_pIAndroidUserItem;					//用户接口
	CGameLogic						m_GameLogic;							//游戏逻辑

	//存取控制
	LONGLONG						m_lRobotScoreMin;					// 积分低于取款
	LONGLONG						m_lRobotScoreMax;					// 积分高于存款
	LONGLONG						m_lRobotBankTake[2];				// 取款额度
	LONGLONG						m_lRobotBankSave;					// 存款额度
	
	//函数定义
public:
	//构造函数
	CAndroidUserItemSink();
	//析构函数
	virtual ~CAndroidUserItemSink();

	//基础接口
public:
	//释放对象
	virtual VOID Release() { delete this; }
	//接口查询
	virtual void * QueryInterface(const IID & Guid, DWORD dwQueryVer);

	//控制接口
public:
	//初始接口
	virtual bool Initialization(IUnknownEx * pIUnknownEx);
	//重置接口
	virtual bool RepositionSink();

	//游戏事件
public:
	//时间消息
	virtual bool OnEventTimer(UINT nTimerID);
	//游戏消息
	virtual bool OnEventGameMessage(WORD wSubCmdID, void * pData, WORD wDataSize);
	//游戏消息
	virtual bool OnEventFrameMessage(WORD wSubCmdID, void * pData, WORD wDataSize);
	//场景消息
	virtual bool OnEventSceneMessage(BYTE cbGameStatus, bool bLookonOther, void * pData, WORD wDataSize);

	//用户事件
public:
	//用户进入
	virtual void OnEventUserEnter(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser);
	//用户离开
	virtual void OnEventUserLeave(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser);
	//用户积分
	virtual void OnEventUserScore(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser);
	//用户状态
	virtual void OnEventUserStatus(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser);
	//用户段位
	virtual void OnEventUserSegment(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser);

	//消息处理
protected:	
	//游戏开始
	bool OnSubGameStart(const void * pBuffer, WORD wDataSize);
	//用户加注
	bool OnSubAddGold(const void * pBuffer, WORD wDataSize);
	//发牌消息
	bool OnSubSendCard(const void * pBuffer, WORD wDataSize);
	//用户放弃
	bool OnSubGiveUp(const void * pBuffer, WORD wDataSize);
	//游戏结束
	bool OnSubGameEnd(const void * pBuffer, WORD wDataSize);
	//游戏结束
	bool OnSubTrueEnd(const void * pBuffer, WORD wDataSize);

private:
	//读取配置
	void ReadConfigInformation();
	// 计算排名
	void CalculateRank( OUT WORD wRank[GAME_PLAYER], IN BYTE cbCardData[GAME_PLAYER][MAX_COUNT], IN BYTE cbCardCount );
};

//////////////////////////////////////////////////////////////////////////

#endif
