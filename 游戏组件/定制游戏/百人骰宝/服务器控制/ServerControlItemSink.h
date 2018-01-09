#pragma once
#include "../游戏服务器/ServerControl.h"
#include "../游戏服务器/GameLogic.h"

#define INDEX_BANKER				1
#define CONTROL_AREA				4
//#define MAX_CARD					5
#define MAX_CARDGROUP				5


class CServerControlItemSink : public IServerControl
{
	//玩家控制						
protected:
	BYTE							m_cbExcuteTimes;				//执行次数
	BYTE							m_cbControlStyle;				//控制方式
	BYTE                            m_cbEnemyChairID;                //对家ID
	//辅助信息
protected:
	CGameLogic						m_GameLogic;					//游戏逻辑

public:
	CServerControlItemSink(void);
	virtual ~CServerControlItemSink(void);

public:
	//服务器控制
	virtual bool  ServerControl(WORD wSubCmdID, const void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem, ITableFrame * pITableFrame);

	//需要控制
	virtual bool  NeedControl();

	//满足控制
	virtual bool  MeetControl(tagControlInfo ControlInfo);

	//完成控制
	virtual bool  CompleteControl();

	//返回控制区域
	virtual bool  ReturnControlArea(tagControlInfo& ControlInfo);

	//开始控制
	virtual void  GetSuitResult( BYTE cbTableCardArray[][2]);

};
