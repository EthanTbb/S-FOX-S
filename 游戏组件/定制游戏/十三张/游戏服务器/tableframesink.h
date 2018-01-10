#ifndef TABLE_FRAME_SINK_HEAD_FILE
#define TABLE_FRAME_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "DlgCustomRule.h"
#include "..\消息定义\CMD_Thirteen.h"
#include "..\游戏客户端\GameLogic.h"
#include "..\游戏客户端\HistoryScore.h"
#include "ServerControl.h"
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////

//游戏桌子类
class CTableFrameSink : public ITableFrameSink, public ITableUserAction
{
	//组件变量
protected:
	CGameLogic							m_GameLogic;										//游戏逻辑
	ITableFrame							*m_pITableFrame;									//框架接口
	const tagGameServiceOption			*m_pGameServiceOption;								//配置参数
	tagCustomRule*						m_pGameCustomRule;									//自定规则
	CHistoryScore						m_HistoryScore;										//历史成绩
	BYTE								m_cbWaitTime;										//等待时间
	//游戏变量
protected:
	SCORE								m_lUserChip[GAME_PLAYER];							//用户下注
	SCORE								m_lScoreTimes[GAME_PLAYER][3];						//输赢注数
	BYTE								m_bSpecialType[GAME_PLAYER];						//特殊牌型
	BYTE								m_bUserCardData[GAME_PLAYER][HAND_CARD_COUNT];		//玩家扑克
	BYTE								m_bSegmentCard[GAME_PLAYER][3][5];					//分段扑克
    	
	WORD								m_wFirstCallBankerUser ;							//叫庄玩家
	WORD								m_wBanker;											//庄家玩家
	WORD								m_wCurrentCaller ;									//当前叫庄	
	
	WORD								m_wSetChipCount;									//下注计数
	WORD								m_wShuffleFinishCount;								//洗牌计数
	WORD								m_wDispatchFinishCount;								//发牌计数
	WORD								m_wSpliFinishCount;									//切牌计数	
	WORD								m_wShowCardCount;									//摊牌计数
	WORD								m_wCompareCount;									//比较计数
	WORD								m_wCompleteCompareCount ;							//比较计数
	
	bool								m_bFinishSegment[GAME_PLAYER];						//完成分段
	bool								m_bGameStatus[GAME_PLAYER];							//游戏状态
	bool								m_bIsReady[GAME_PLAYER];							//已经准备
	bool								m_bCompared;										//是否比牌
	//控制变量
protected:
	bool								m_bNeedControl;										//需要控制
	bool								m_bMysteryUser[GAME_PLAYER];						//控制玩家

	//服务控制
protected:
	HINSTANCE							m_hControlInst;
	IServerControl*						m_pServerControl;

	//函数定义
public:
	//构造函数
	CTableFrameSink();
	//析构函数
	virtual ~CTableFrameSink();

	//基础接口
public:
	//释放对象
	virtual VOID Release() { delete this; }
	//是否有效
	virtual bool  IsValid() { return AfxIsValidAddress(this,sizeof(CTableFrameSink))?true:false; }
	//接口查询
	virtual VOID * QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	//管理接口
public:
	//复位桌子
	virtual VOID RepositionSink();
	//配置桌子
	virtual bool Initialization(IUnknownEx * pIUnknownEx);

	//查询接口
public:
	//查询限额
	virtual SCORE QueryConsumeQuota(IServerUserItem * pIServerUserItem);
	//最少积分
	virtual SCORE QueryLessEnterScore(WORD wChairID, IServerUserItem * pIServerUserItem);
	//查询是否扣服务费
	virtual bool QueryBuckleServiceCharge(WORD wChairID){return true;}

	//通用函数
public:
	//统计人数
	DWORD GetPlayerCount();
	//获取倍数
	void CompareCardGetTimes();
	//开始比牌
	VOID StartCompareCard();
	//获取倍数
	int GetCardTimes(WORD wUserID);
	//获取下注
	void GetChips( LONGLONG lChipArray[GAME_PLAYER][SET_CHIP_COUNT]);
	//获取下注
	void GetChips(WORD wChairID, LONGLONG lChipArray[SET_CHIP_COUNT]);

	//消息处理
protected:
	//玩家叫庄
	bool OnUserCallBanker(const WORD wChairID , const void * pDataBuffer, WORD wDataSize) ;
	//玩家不叫
	bool OnUserNoCallBanker(const WORD wChairID , const void * pDataBuffer, WORD wDataSize) ;
	//玩家下注
	bool OnUserSetChip(const WORD wChairID , const void * pDataBuffer, WORD wDataSize) ;
	//玩家分段
	bool OnUserSetSegment(const WORD wChairID , const void * pDataBuffer, WORD wDataSize) ;
	//玩家摊牌
	bool OnUserShowCard(const WORD wChairID , const void * pDataBuffer, WORD wDataSize) ;

	//游戏事件
public:
	//游戏开始
	virtual bool OnEventGameStart();
	//游戏结束
	virtual bool OnEventGameConclude(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason);
	//发送场景
	virtual bool OnEventSendGameScene(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbGameStatus, bool bSendSecret);
	//作弊用户
	bool OnCheatCard(IServerUserItem * pIServerUserItem);
	//发送作弊信息
	void SendCheatCard();

	//事件接口
public:
	//时间事件
	virtual bool OnTimerMessage(DWORD wTimerID, WPARAM wBindParam);
	//数据事件
	virtual bool OnDataBaseMessage(WORD wRequestID, VOID * pData, WORD wDataSize);
	//积分事件
	virtual bool OnUserScroeNotify(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason);

	//网络接口
public:
	//游戏消息
	virtual bool OnGameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem);
	//框架消息
	virtual bool OnFrameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem);

	//////////////////////////////////////////////////////////////////////////
	//用户事件
public:
	//用户断线
	virtual bool OnActionUserOffLine(WORD wChairID, IServerUserItem * pIServerUserItem) { return true; }
	//用户重入
	virtual bool OnActionUserConnect(WORD wChairID, IServerUserItem * pIServerUserItem) { return true; }
	//用户坐下
	virtual bool OnActionUserSitDown(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser);
	//用户起立
	virtual bool OnActionUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser);
	//用户同意
	virtual bool OnActionUserOnReady(WORD wChairID, IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize);
	//设置基数
	virtual void SetGameBaseScore(LONG lBaseScore){};

public:
	BOOL  m_bIsWin;                                        //机器人是否需要赢钱
	SCORE m_sRegionalControl[8];                           //控制区域值
	SCORE m_sRobotLnventory;                               //机器人赢钱库存
	BYTE  m_bRegionalProbability[9];                       //机器人区域赢钱概率
    
	//将牌型转化为牌型得分
	int   CardTypeToScore(BYTE bCardType,BYTE bSpecies);
	//对机器人进行输赢控制
	void  ResultsControl(int iCount);
	//判断机器人输赢结果
	void  JudgeResults();
	//读取配置文件
	void ReadConfiguration();
	//是否控制
	bool IsNeedControl();
	//库存消息
	VOID SendControlMessage(IServerUserItem * pIServerUserItem);
	//发送消息
	void SendInfo(CString str, WORD wChairID = INVALID_CHAIR);
};

//////////////////////////////////////////////////////////////////////////

#endif
