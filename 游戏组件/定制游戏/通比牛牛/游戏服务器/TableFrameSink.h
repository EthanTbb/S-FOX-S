#ifndef TABLE_FRAME_SINK_HEAD_FILE
#define TABLE_FRAME_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "GameLogic.h"
#include "HistoryScore.h"
#include "ServerControl.h"
#include "DlgCustomRule.h"

//////////////////////////////////////////////////////////////////////////

//游戏桌子类
class CTableFrameSink : public ITableFrameSink, public ITableUserAction
{
  //游戏变量
protected:
  WORD              m_wBankerUser;              //庄家用户
  LONGLONG            m_lCellScore;             //游戏底分
  LONGLONG            m_lExitScore;             //强退分数
  LONGLONG            m_lBonus;

  //用户数据
protected:
  BYTE              m_cbPlayStatus[GAME_PLAYER];      //游戏状态
  BYTE                            m_cbDynamicJoin[GAME_PLAYER];           //动态加入
  BYTE              m_cbOxCard[GAME_PLAYER];        //牛牛数据
  LONGLONG            m_lTableScore[GAME_PLAYER];       //下注数目

  //扑克变量
protected:
  BYTE              m_cbOxCardData[GAME_PLAYER][MAX_COUNT]; //牛牛扑克
  BYTE              m_cbHandCardData[GAME_PLAYER][MAX_COUNT];//桌面扑克

  //服务控制
protected:
  HINSTANCE           m_hInst;                //控制句柄
  IServerControl*         m_pServerContro;            //控制组件

  //组件变量
protected:
  CGameLogic            m_GameLogic;              //游戏逻辑
  CHistoryScore         m_HistoryScore;             //历史成绩
  ITableFrame           * m_pITableFrame;           //框架接口
  tagGameServiceOption        *m_pGameServiceOption;          //配置参数
  tagGameServiceAttrib      *m_pGameServiceAttrib;          //游戏属性
  tagCustomRule *         m_pGameCustomRule;            //自定规则

  //函数定义
public:
  //构造函数
  CTableFrameSink();
  //析构函数
  virtual ~CTableFrameSink();

  //基础接口
public:
  //释放对象
  virtual VOID Release() {}
  //接口查询
  virtual void * QueryInterface(const IID & Guid, DWORD dwQueryVer);

  //管理接口
public:
  //初始化
  virtual bool Initialization(IUnknownEx * pIUnknownEx);
  //复位桌子
  virtual void RepositionSink();

  virtual bool IsUserPlaying(WORD wChairID);
  //游戏事件
public:
  //游戏开始
  virtual bool OnEventGameStart();
  //游戏结束
  virtual bool OnEventGameConclude(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason);
  //发送场景
  virtual bool OnEventSendGameScene(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE bGameStatus, bool bSendSecret);


  //事件接口
public:
  //定时器事件
  virtual bool OnTimerMessage(DWORD dwTimerID, WPARAM wBindParam);
  //游戏消息处理
  virtual bool OnGameMessage(WORD wSubCmdID, void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem);
  //框架消息处理
  virtual bool OnFrameMessage(WORD wSubCmdID, void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem);
  //数据事件
  virtual bool OnGameDataBase(WORD wRequestID, VOID * pData, WORD wDataSize)
  {
    return true;
  }

  //查询接口
public:
  //查询限额
  virtual SCORE QueryConsumeQuota(IServerUserItem * pIServerUserItem);
  //最少积分
  virtual SCORE QueryLessEnterScore(WORD wChairID, IServerUserItem * pIServerUserItem)
  {
    return 0;
  }
  //查询服务费
  virtual bool QueryBuckleServiceCharge(WORD wChairID);
  //数据事件
  virtual bool OnDataBaseMessage(WORD wRequestID, VOID * pData, WORD wDataSize)
  {
    return false;
  }
  //积分事件
  virtual bool OnUserScroeNotify(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)
  {
    return false;
  }
  //设置基数
  virtual void SetGameBaseScore(LONG lBaseScore)
  {
    return;
  }

  //用户事件
public:
  //用户断线
  virtual bool OnActionUserOffLine(WORD wChairID, IServerUserItem * pIServerUserItem)
  {
    return true;
  }
  //用户重入
  virtual bool OnActionUserConnect(WORD wChairID, IServerUserItem * pIServerUserItem)
  {
    return true;
  }
  //用户坐下
  virtual bool OnActionUserSitDown(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser);
  //用户起立
  virtual bool OnActionUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser);
  //用户同意
  virtual bool OnActionUserOnReady(WORD wChairID, IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize)
  {
    return true;
  }

  //游戏事件
protected:
  //摊牌事件
  bool OnUserOpenCard(WORD wChairID, BYTE cbOxCardData[], BYTE bOx);
  //写分函数
  bool TryWriteTableScore(tagScoreInfo ScoreInfoArray[]);
  //分析扑克
  void AnalyseCard();

  //功能函数
protected:
  //是否可加
  bool UserCanAddScore(WORD wChairID, LONGLONG lAddScore);
  //是否衰减
  bool NeedDeductStorage();
  //更新房间用户信息
  void UpdateUserRosterInfo(IServerUserItem *pIServerUserItem, LONGLONG lGameScore = 0LL, USERROSTER userRoster = INVALID_ROSTER, LONGLONG lScoreLimit = 0LL);
  //解除用户名单
  void RemoveUserRoster();
  //计算机器人分数
  LONGLONG CalculateAndroidScore(WORD &wWinUser);
};

//////////////////////////////////////////////////////////////////////////

#endif
