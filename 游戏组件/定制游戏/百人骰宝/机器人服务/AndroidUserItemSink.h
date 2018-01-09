#ifndef ANDROID_USER_ITEM_SINK_HEAD_FILE
#define ANDROID_USER_ITEM_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "..\游戏服务器\GameLogic.h"
//////////////////////////////////////////////////////////////////////////
//宏定义

//最大下注次数
#define MAX_CHIP_TIME								50
//////////////////////////////////////////////////////////////////////////

//机器人类
class CAndroidUserItemSink : public IAndroidUserItemSink
{
	//游戏变量
protected:
	bool							m_bHaveConfig;						//是否配置
	SCORE							m_lMaxChipBanker;					//最大下注 (庄家)
	SCORE							m_lMaxChipUser;						//最大下注 (个人)
	SCORE							m_lAreaChip[AREA_COUNT];			//区域下注 
	WORD							m_wCurrentBanker;					//庄家位置

	int								m_nChipLimit[2];					//下注范围 (0-AREA_COUNT)
	int								m_nChipTime;						//下注次数 (本局)
	int								m_nChipTimeCount;					//已下次数 (本局)

	SCORE							m_lBankerLostScore;					//庄家输分 (当机器人为庄)

	SCORE							m_lBankerDrawCount;					//提款次数 (庄家)
	SCORE							m_lBankerWinCount;					//下庄条件 (赢分)
	SCORE							m_lBankerLoseCount;					//下庄条件 (输分)

	BYTE							m_bWinFlag[AREA_COUNT];				//输赢信息 (用于下注偏移)

	//上庄变量
protected:
	int								m_nBankerCount;						//本机器人的坐庄次数
	int								m_nCurMaxBankerCount;				//本次随机最大坐庄次数
	bool							m_bRobotBanker;						//是否坐庄
	bool							m_bMeApplyBanker;					//申请标识
	int								m_nRobotBankerCount;				//坐庄次数  
	static int						m_stlApplyBanker;					//申请数
	static int						m_stnApplyCount;					//申请数
	INT64                           m_nListUserCount;					//列表人数

	//配置变量  (全局配置)
protected:
	tagRobotInfo					m_RobotInfo;						//全局配置
	TCHAR							m_szRoomName[32];					//配置房间

	//配置变量	(游戏配置)
protected:
	SCORE							m_lAreaLimitScore;					//区域限制
	SCORE							m_lUserLimitScore;					//下注限制
	SCORE							m_lBankerCondition;					//上庄条件		

	//配置变量  (机器人配置)
protected:
	int								m_nRobotBetDelay;					//下注延迟

	SCORE							m_lRobotBankerCondition;			//上庄条件 (上面是游戏限制，这个是单个机器人限制)
	int								m_nRobotListMaxCount;				//上庄个数
	int								m_nRobotListMinCount;				//上庄个数
	int								m_nRobotApplyBanker;				//上庄个数
	SCORE							m_lRobotBankSaveCondition;			//金币上限
	SCORE							m_lRobotBankDrawCondition;			//金币下限
	SCORE							m_lRobotBankSaveScore;				//存款数额
	SCORE							m_lRobotBankDrawScore;				//提款数额

	SCORE							m_lRobotJettonLimit[2];				//筹码限制	
	int								m_nRobotBetTimeLimit[2];			//次数限制
	int								m_nRobotWaitBanker;					//空盘重申
	int								m_nWaitBanker;						//空几盘
	SCORE							m_lRobotBankDrawCount;				//提款次数 (庄家)
	SCORE							m_lRobotGiveUpWin;					//下庄条件 (赢分)
	SCORE							m_lRobotGiveUpLose;					//下庄条件 (输分)


	//机器人存取款
	LONGLONG						m_lRobotScoreRange[2];				//最大范围
	LONGLONG						m_lRobotBankGetScoreMin;				//提款数额
	LONGLONG						m_lRobotBankGetScoreMax;				//提款数额
	LONGLONG						m_lRobotBankGetScoreBankerMin;			//提款数额 (庄家)
	LONGLONG						m_lRobotBankGetScoreBankerMax;			//提款数额 (庄家)
	int								m_nRobotBankStorageMul;				//存款倍数


	//控件变量
protected:
	CGameLogic						m_GameLogic;						//游戏逻辑
	IAndroidUserItem *				m_pIAndroidUserItem;				//用户接口

	//函数定义
public:
	//构造函数
	CAndroidUserItemSink();
	//析构函数
	virtual ~CAndroidUserItemSink();

	//基础接口
public:
	//释放对象
	virtual VOID  Release() {delete this; }
	
	//接口查询
	virtual VOID *  QueryInterface(const IID & Guid, DWORD dwQueryVer);


	//控制接口
public:
	//初始接口
	virtual bool  Initialization(IUnknownEx * pIUnknownEx);
	//重置接口
	virtual bool  RepositionSink();


	//游戏事件
public:
	//时间消息
	virtual bool  OnEventTimer(UINT nTimerID);
	//游戏消息
	virtual bool  OnEventGameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//游戏消息
	virtual bool  OnEventFrameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//场景消息
	virtual bool  OnEventSceneMessage(BYTE cbGameStatus, bool bLookonOther, VOID * pData, WORD wDataSize);


	//用户事件
public:
	//用户进入
	virtual VOID  OnEventUserEnter(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser);
	//用户离开
	virtual VOID  OnEventUserLeave(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser);
	//用户积分
	virtual VOID  OnEventUserScore(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser);
	//用户状态
	virtual VOID  OnEventUserStatus(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser);
	
	//消息处理
public:
	//游戏空闲
	bool OnSubGameFree(const VOID * pBuffer, WORD wDataSize);
	//游戏开始
	bool OnSubGameStart(const VOID * pBuffer, WORD wDataSize);
	//用户加注
	bool OnSubPlaceJetton(const VOID * pBuffer, WORD wDataSize);
	//下注失败
	bool OnSubPlaceJettonFail(const VOID * pBuffer, WORD wDataSize);
	//游戏结束
	bool OnSubGameEnd(const VOID * pBuffer, WORD wDataSize);
	//申请做庄
	bool OnSubUserApplyBanker(const VOID * pBuffer, WORD wDataSize);
	//取消做庄
	bool OnSubUserCancelBanker(const VOID * pBuffer, WORD wDataSize);
	//切换庄家
	bool OnSubChangeBanker(const VOID * pBuffer, WORD wDataSize);
	//上庄通知
	bool OnSubRobotBanker(const VOID * pBuffer, WORD wDataSize);
	//配置通知
	bool OnSubRobotConfig(const VOID * pBuffer, WORD wDataSize);

	//功能函数
public:
	//读取配置
	VOID ReadConfigInformation(tagCustomAndroid *pCustomAndroid);
	//计算范围
	bool CalcJettonRange(SCORE lMaxScore, SCORE lChipLmt[], int & nChipTime, int lJetLmt[]);
	//读取长整
	SCORE GetPrivateProfileInt64(LPCTSTR lpAppName, LPCTSTR lpKeyName, SCORE lDefault, LPCTSTR lpFileName);
};

//////////////////////////////////////////////////////////////////////////

#endif
