#ifndef TABLE_FRAME_SINK_HEAD_FILE
#define TABLE_FRAME_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "GameLogic.h"
#include "RobotManage.h"
#include "afxtempl.h"

#define MAX_CARD					2
#define MAX_CARDGROUP				4
#define CONTROL_AREA				3

//////////////////////////////////////////////////////////////////////////
//历史记录
#define MAX_SCORE_HISTORY			16									//历史个数
//////////////////////////////////////////////////////////////////////////

//游戏桌子类
class CTableFrameSink : public ITableFrameSink, public ITableUserAction
{
	//下注信息
protected:
	SCORE						m_lAllUserBet[AREA_COUNT];					//全体总注
	SCORE						m_lAndroidBet[AREA_COUNT];					//机器人下注
	SCORE						m_lFleeUserBet[AREA_COUNT];					//逃跑玩家下注
	SCORE						m_lUserAllBet[AREA_COUNT][GAME_PLAYER];		//个人总注
	SCORE						m_lUserStartScore[GAME_PLAYER];		//起始分数
	LONGLONG					m_lQueryGameID;						//查询ID
	BYTE						m_cbUserWinLose;					//单个玩家输赢状态，0为无效，1为赢，2为输
	bool						m_bControlWin;						//控制单个玩家输赢
	LONGLONG					m_lControlStorage;						//控制后的损耗写入
	TCHAR						m_szControlName[LEN_NICKNAME];			//房间名称
	
	//控制变量
protected:
	SCORE						m_lAreaLimitScore;							//区域限制
	SCORE						m_lUserLimitScore;							//区域限制
	SCORE						m_lApplyBankerCondition;					//申请条件
	INT							m_nEndGameMul;								//提前开牌百分比

	//坐庄控制	
	int								m_nBankerTimeLimit;						//次数限制
	int								m_nBankerTimeAdd;						//增加次数 (金币大于其他玩家时)
	LONGLONG						m_lExtraBankerScore;					//额外条件 (大于此值时可以无视条件额外坐庄)
	int								m_nExtraBankerTime;						//额外次数
	WORD							m_wAddTime;								//额外坐庄 
	bool							m_bExchangeBanker;						//变换庄家
	
	TCHAR						m_szConfigFileName[MAX_PATH];				//配置文件
	LONGLONG                    m_lCurSystemWinScore;						//系统赢分

	//玩家成绩
protected:
	bool						m_bContiueCard;								//继续发牌
	BYTE						m_bcFirstPostCard;							//最新發牌點數
	BYTE						m_cbLeftCardCount;							//扑克数目
	LONG						m_lUserRevenue[GAME_PLAYER];				//玩家税收
	SCORE						m_lUserWinScore[GAME_PLAYER];				//玩家成绩
	SCORE						m_lUserReturnScore[GAME_PLAYER];			//返回下注
	BYTE						m_bWinFlag[AREA_COUNT];						//输赢信息 (机器人)						

	//扑克信息
protected:
	BYTE						m_cbDiceValue[DICE_COUNT];

	//时间设置
public:
	DWORD						m_dwTimeGameFree;							//空闲时间
	DWORD						m_dwTimeGameCard;							//派牌时间
	DWORD						m_dwTimeGameBet;							//下注时间
	DWORD						m_dwTimeGameEnd;							//结束时间	
	CString						m_strAppName;								//文件名称

	//状态变量
protected:
	DWORD						m_dwStartTime;								//开始时间

	//玩家控制						
protected:
	BYTE						m_cbControlStyle;							//控制方式
	bool						m_bWinArea[CONTROL_AREA];					//赢家区域
	BYTE						m_cbWinAreaCount;
	BYTE						m_cbReqType;								//当前接收类型
	//库存控制
	int							m_nStorageDeduct;							//库存衰减 
	SCORE						m_lStorageStart;							//库存数值
	SCORE						m_lStorageCurrent;							//当前库存

	// 11/11/2013 [Wan.MY]
	LONGLONG						m_lStorageMax1;							//库存封顶1
	LONGLONG						m_lStorageMul1;							//系统输钱概率
	LONGLONG						m_lStorageMax2;							//库存封顶2
	LONGLONG						m_lStorageMul2;							//系统输钱概率

	//机器人控制
	int								m_nMaxChipRobot;						//最大数目 (下注机器人)
	int								m_nChipRobotCount;						//人数统计 (下注机器人)
	bool							m_bRobotChip[GAME_PLAYER];					//机器人是否下注
	LONGLONG						m_lRobotAreaLimit;						//区域统计 (机器人)
	LONGLONG						m_lRobotAreaScore[AREA_COUNT];		//区域统计 (机器人)
	int								m_nRobotListMaxCount;					//最多人数

	//庄家信息
protected:
	WORD						m_wBankerTime;								//做庄次数
	WORD						m_wCurrentBanker;							//当前庄家
	bool						m_bEnableSysBanker;							//系统做庄
	SCORE						m_lBankerScore;								//开局金币						
	SCORE						m_lBankerWinScore;							//累计成绩
	SCORE						m_lBankerCurGameScore;						//当前成绩
	CWHArray<WORD>				m_ApplyUserArray;							//申请玩家

	//控制变量
protected:
	BYTE						m_cbWinSideControl;							//控制输赢
	int							m_nSendCardCount;							//发送次数

	bool                        m_cbNeedControl;							//状态控制
	BYTE						m_cbControlDiceValue[DICE_COUNT];
	BYTE						m_cbControlTotal;							//控制总点数

	//记录变量
protected:
	int							m_nRecordFirst;								//开始记录
	int							m_nRecordLast;								//最后记录
	CFile						m_FileRecord;								//记录结果
	DWORD						m_dwRecordCount;							//记录数目
	tagGameRecord				m_GameRecordList[MAX_SCORE_HISTORY];		//游戏记录
	
	//组件变量
protected:
	CGameLogic						m_GameLogic;							//游戏逻辑
	CRobotConfigManage				m_RobotCfgManage;						//机器管理
	ITableFrame						* m_pITableFrame;						//框架接口
	//ITableFrameControl				* m_pITableFrameControl;			//框架接口
	const tagGameServiceOption		* m_pGameServiceOption;					//配置参数

	//属性变量
protected:
	static const WORD				m_wPlayerCount;							//游戏人数
	
	//函数定义
public:
	//构造函数
	CTableFrameSink();
	//析构函数
	virtual ~CTableFrameSink();

	//基础接口
public:
	//释放对象
	virtual VOID  Release() { }
	
	//接口查询
	virtual VOID *  QueryInterface(const IID & Guid, DWORD dwQueryVer);


	//管理接口
public:
	//初始化
	virtual bool Initialization(IUnknownEx * pIUnknownEx);
	//复位桌子
	virtual VOID RepositionSink();

	//查询接口
public:
	//查询限额
	virtual SCORE QueryConsumeQuota(IServerUserItem * pIServerUserItem);
	//最少积分
	virtual SCORE QueryLessEnterScore(WORD wChairID, IServerUserItem * pIServerUserItem){return 0;}
	//数据事件
	virtual bool OnDataBaseMessage(WORD wRequestID, VOID * pData, WORD wDataSize){return false;}
	//积分事件
	virtual bool OnUserScroeNotify(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason){return false;}
	//查询是否扣服务费
	virtual bool QueryBuckleServiceCharge(WORD wChairID);
	//信息接口
public:
	
	//游戏状态
	virtual bool  IsUserPlaying(WORD wChairID);


	//游戏事件
public:
	//游戏开始
	virtual bool OnEventGameStart();
	//游戏结束
	virtual bool OnEventGameConclude(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason);
	//发送场景
	virtual bool OnEventSendGameScene(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbGameStatus, bool bSendSecret);

	//事件接口
public:
	//定时器事件
	virtual bool OnTimerMessage(DWORD wTimerID, WPARAM wBindParam);
	//游戏消息处理
	virtual bool OnGameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem);
	//框架消息处理
	virtual bool OnFrameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem);
	//数据事件
	virtual bool OnGameDataBase(WORD wRequestID, VOID * pData, WORD wDataSize);


	//动作事件
public:
	//用户断线
	virtual bool OnActionUserOffLine(WORD wChairID,IServerUserItem * pIServerUserItem);
	//用户重入
	virtual bool OnActionUserConnect(WORD wChairID,IServerUserItem * pIServerUserItem) { return true; }
	//用户坐下
	virtual bool OnActionUserSitDown(WORD wChairID,IServerUserItem * pIServerUserItem, bool bLookonUser);
	//用户起来
	virtual bool OnActionUserStandUp(WORD wChairID,IServerUserItem * pIServerUserItem, bool bLookonUser);
	//用户同意
	virtual bool OnActionUserOnReady(WORD wChairID,IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize) { return true; }

	//比赛接口
public:
	//设置基数
	virtual void SetGameBaseScore(LONG lBaseScore){};


	//游戏事件
protected:
	//加注事件
	bool OnUserPlaceJetton(WORD wChairID, BYTE cbJettonArea, SCORE lJettonScore);
	//申请庄家
	bool OnUserApplyBanker(IServerUserItem *pIApplyServerUserItem);
	//取消申请
	bool OnUserCancelBanker(IServerUserItem *pICancelServerUserItem);
	//管理员命令
	bool OnSubAmdinCommand(IServerUserItem*pIServerUserItem,const VOID*pDataBuffer);

	//辅助函数
private:
	bool FindSuitBanker();
	//更换庄家
	bool ChangeBanker(bool bCancelCurrentBanker);
	//发送庄家
	VOID SendApplyUser(IServerUserItem *pServerUserItem);
	//发送记录
	VOID SendGameRecord(IServerUserItem *pIServerUserItem);
	//发送消息
	VOID SendGameMessage(WORD wChairID, LPCTSTR pszTipMsg);
	//玩家逃跑
	bool PlayerFlee(WORD wChairID,IServerUserItem*pIServerUserItem);
	//上庄控制 (机器人)
	VOID RobotBankerControl();
	//配置控制 (机器人)
	VOID RobotConfigControl();
	//发送下注信息
	void SendUserBetInfo( IServerUserItem *pIServerUserItem );
	//判断玩家输赢状态
	BYTE ControlUserWin();

	//下注计算
private:
	//交换纸牌
	VOID ChangeMaxUser(bool blMin=false);
	//最大下注
	SCORE GetUserMaxJetton(WORD wChairID,BYTE cbAreaID);
	//混乱纸牌
	VOID RandList(BYTE cbCardBuffer[], BYTE cbBufferCount);

	//定时事件
protected:
	//开始下注
	bool OnTimerStartBet();
	//显示结果
	bool OnTimerShowResult();
	//游戏结束
	bool OnTimerGameEnd();
//控制平衡
private:	
	//获取分数
	void GetStockScore(SCORE *lAndroidScore,SCORE *lUserScore,BYTE bDice[]);
	//读取配置
	void ReadConfigInformation();

	//游戏统计
private:
	//计算得分
    SCORE CalculateScore(bool bProbeJudgeScore);
	//纸牌转换
	CString TransformCardInfo( BYTE cbCardData ); 
	//转换牌型
	bool TransFromCardStyle(BYTE cbIndex,CString &str);	
	//重新发牌
	bool DispatchTableLabel();
	//试探性判断
	bool ProbeJudge();
	//写库存
	void WriteStorage(LONGLONG lSystemScore);
	//平衡下注
	void BalanceCurBetScore(LONGLONG lAllUserBet[],int nCount);
	//庄家最小得分
	SCORE GetBankerWinMinScore(BYTE cbAreaID);
	// 记录函数
	void WriteInfo( LPCTSTR pszFileName, LPCTSTR pszString );
};

//////////////////////////////////////////////////////////////////////////

#endif
