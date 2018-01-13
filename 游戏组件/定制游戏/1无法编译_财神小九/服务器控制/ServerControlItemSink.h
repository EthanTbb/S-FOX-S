#pragma once
#include "../游戏服务器/ServerControl.h"
#include "../游戏服务器/GameLogic.h"

#define INDEX_BANKER				1
#define CONTROL_AREA				4
//#define MAX_CARD					5
#define MAX_CARDGROUP				4

class CServerControlItemSink : public IServerControl
{
	//玩家控制						
protected:
	BYTE						m_cbExcuteTimes;					//执行次数
	BYTE						m_cbControlArea[MAX_INDEX];			//控制区域

	//辅助信息
protected:
	CGameLogic					m_GameLogic;						//游戏逻辑

public:
	CServerControlItemSink(void);
	virtual ~CServerControlItemSink(void);

public:
	//设置是否能成功
	bool IsSettingSuccess(BYTE cbControlArea[MAX_INDEX]);

public:
	//服务器控制
	virtual bool  ServerControl(WORD wSubCmdID, const void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem, ITableFrame * pITableFrame);

	//需要控制
	virtual bool  NeedControl();

	//完成控制
	virtual bool  CompleteControl();

	//开始控制
	virtual void  GetSuitResult( BYTE cbTableCardArray[][MAX_CARD], BYTE cbTableCard[], LONGLONG lAllJettonScore[] );

	//输赢控制
protected:

	//计算分配
	bool GetSuitCardCombine(BYTE cbStack[]);

	//庄家输赢
	LONGLONG GetBankerWinScore(int nWinMultiple[]);

};
