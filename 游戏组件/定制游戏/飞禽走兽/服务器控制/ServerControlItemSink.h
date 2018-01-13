#pragma once
#include "../游戏服务器/ServerControl.h"
#include "../游戏服务器/GameLogic.h"

class CServerControlItemSink : public IServerControl
{
  //控制操作
protected:
  LONGLONG            m_lAllJettonScore[AREA_COUNT+1];    //全体总注
  BYTE              m_cbControlArea;            //控制区域
  BYTE              m_cbControlTimes;           //控制次数
  LONGLONG                        m_lCurStorage;              //当前库存
  BYTE              m_cbControlStyle;           //控制方式
  CGameLogic            m_GameLogic;              //游戏逻辑

public:
  CServerControlItemSink(void);
  virtual ~CServerControlItemSink(void);

  //实际区域
  BYTE    GetRealArea(BYTE cbNeedControlArea);
  //设置区域
  BYTE    BankerWinCard(bool bIsWin);
  //庄家赢分
  LONGLONG  GetBankerWinScore(BYTE cbRealArea);
  //奖励区域
  BYTE    GetAnotherArea();


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
  virtual bool  ReturnControlArea(tagControlInfo& ControlInfo,LONGLONG lAllJettonScore[]) ;
  //记录库存
  virtual bool  SetStorage(LONGLONG lCurStorage);

  void  UpdateControl(CMD_S_ControlReturns* pControlReturns,IServerUserItem * pIServerUserItem);
  void WriteInfo(LPCTSTR pszString);

  //信息解析
  void PrintingInfo(TCHAR* pText, WORD cbCount, BYTE cbArea, BYTE cbTimes);
};
