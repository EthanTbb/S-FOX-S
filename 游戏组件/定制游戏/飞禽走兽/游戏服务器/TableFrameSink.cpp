#include "StdAfx.h"
#include "TableFrameSink.h"
#include "DlgCustomRule.h"
#include <locale>
//////////////////////////////////////////////////////////////////////////

//常量定义
#define SEND_COUNT          300                 //发送次数

//索引定义
#define INDEX_PLAYER        0                 //闲家索引
#define INDEX_BANKER        1                 //庄家索引

//下注时间
#define IDI_FREE          1                 //空闲时间
#define TIME_FREE         10                  //空闲时间

//下注时间
#define IDI_PLACE_JETTON      2                 //下注时间
#define TIME_PLACE_JETTON     10                  //下注时间

//结束时间
#define IDI_GAME_END        3                 //结束时间
#define TIME_GAME_END       15                  //结束时间
#define TIME_GAME_ADD               10                                  //附加时间
//////////////////////////////////////////////////////////////////////////

//静态变量
const WORD      CTableFrameSink::m_wPlayerCount=GAME_PLAYER;        //游戏人数

//////////////////////////////////////////////////////////////////////////
int  Findstr(char *buf,char findchar,int len)
{
  int i = 0;
  while(i<len)
  {
    if(buf[i++]==findchar)
    {
      return (i-1);
    }
  }
  return -1;
}
char* strGetbuffer(char *buf,int ipos,char *OutBuf,int outLen)
{
  if(ipos<outLen)
  {
    memcpy(OutBuf,buf,ipos);
  }
  else
  {
    memcpy(OutBuf,buf,outLen);
  }
  return  OutBuf;

}

//构造函数
CTableFrameSink::CTableFrameSink()
{
  //总下注数
  ZeroMemory(m_lAllJettonScore,sizeof(m_lAllJettonScore));

  //个人下注
  ZeroMemory(m_lUserJettonScore,sizeof(m_lUserJettonScore));

  //上局信息
  ZeroMemory(m_lLastAllJettonPlace,sizeof(m_lLastAllJettonPlace));
  ZeroMemory(m_lLastAllJettonArea,sizeof(m_lLastAllJettonArea));

  //起始分数
  ZeroMemory(m_lUserStartScore,sizeof(m_lUserStartScore));
  m_pServerControl=NULL;
  m_cbControlArea = 0xff;
  m_cbControlTimes = 0;
  m_lBonus = 0l;

  //玩家成绩
  ZeroMemory(m_lUserWinScore,sizeof(m_lUserWinScore));
  ZeroMemory(m_lUserReturnScore,sizeof(m_lUserReturnScore));
  ZeroMemory(m_lUserRevenue,sizeof(m_lUserRevenue));

  //扑克信息
  ZeroMemory(m_cbTableCardArray,sizeof(m_cbTableCardArray));

  //状态变量
  m_dwJettonTime=0L;
  m_bControl=false;
  m_lControlStorage=0L;
  CopyMemory(m_szControlName,TEXT("无人"),sizeof(m_szControlName));
  m_cbShaYuAddMulti = 0;
  //庄家信息
  m_ApplyUserArray.RemoveAll();
  m_wCurrentBanker=INVALID_CHAIR;
  m_wBankerTime=0;
  m_lBankerWinScore=0L;
  m_lBankerCurGameScore=0L;
  m_bEnableSysBanker=true;
  m_nRobotListMaxCount =0;
  //记录变量
  ZeroMemory(m_GameRecordArrary,sizeof(m_GameRecordArrary));
  m_nRecordFirst=0;
  m_nRecordLast=0;

  //控制变量
  m_StorageControl =0l;
  m_lStorageCurrent = 0l;
  m_StorageDeduct = 0l;
  m_lCompensateRadio = 0l;

  m_lStorageMax1 = 0;
  m_lStorageMul1 = 0;
  m_lStorageMax2 = 0;
  m_lStorageMul2 = 0;
  //机器人控制
  m_lRobotAreaLimit = 0l;
  m_lRobotBetCount = 0l;

  //庄家设置
  m_lBankerMAX = 0l;
  m_lBankerAdd = 0l;
  m_lBankerScoreMAX = 0l;
  m_lBankerScoreAdd = 0l;
  m_lPlayerBankerMAX = 0l;
  m_bExchangeBanker = true;

  //时间控制
  m_cbFreeTime = TIME_FREE;
  m_cbBetTime = TIME_PLACE_JETTON;
  m_cbEndTime = TIME_GAME_END;

  m_cbAddTime = TIME_GAME_ADD;
  //机器人控制
  m_nChipRobotCount = 0;
  ZeroMemory(m_lRobotAreaScore, sizeof(m_lRobotAreaScore));

  ZeroMemory(m_nAnimalPercent, sizeof(m_nAnimalPercent));
  ZeroMemory(m_nAnimalTimes, sizeof(m_nAnimalTimes));

  //随机种子
  srand(time(NULL));

  return;
}

//析构函数
CTableFrameSink::~CTableFrameSink(void)
{
  if(m_pServerControl)
  {
    delete m_pServerControl;
    m_pServerControl = NULL;
  }

  if(m_hInst)
  {
    FreeLibrary(m_hInst);
    m_hInst = NULL;
  }
}

//接口查询
void *  CTableFrameSink::QueryInterface(const IID & Guid, DWORD dwQueryVer)
{


  QUERYINTERFACE(ITableFrameSink,Guid,dwQueryVer);
  QUERYINTERFACE(ITableUserAction,Guid,dwQueryVer);
#ifdef __BANKER___
  QUERYINTERFACE(ITableUserActionEX,Guid,dwQueryVer);
#endif
  QUERYINTERFACE_IUNKNOWNEX(ITableFrameSink,Guid,dwQueryVer);
  return NULL;

}

//初始化
bool  CTableFrameSink::Initialization(IUnknownEx * pIUnknownEx)
{
  //查询接口
  ASSERT(pIUnknownEx!=NULL);
  m_pITableFrame=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,ITableFrame);
  if(m_pITableFrame==NULL)
  {
    return false;
  }

  //查询配置
  m_pGameServiceOption=m_pITableFrame->GetGameServiceOption();
  m_pGameServiceAttrib=m_pITableFrame->GetGameServiceAttrib();

  //开始模式
  m_pITableFrame->SetStartMode(START_MODE_ALL_READY);


  //设置文件名
  TCHAR szPath[MAX_PATH]=TEXT("");
  GetCurrentDirectory(sizeof(szPath),szPath);
  myprintf(m_szConfigFileName,sizeof(m_szConfigFileName),TEXT("%s\\AnimalBattleConfig.ini"),szPath);
  memcpy(m_szGameRoomName, m_pGameServiceOption->szServerName, sizeof(m_szGameRoomName));

  ReadConfigInformation();

  //服务控制
  m_hInst = NULL;
  m_pServerControl = NULL;
  m_hInst = LoadLibrary(TEXT("AnimalBattleServerControl.dll"));
  if(m_hInst)
  {
    typedef void * (*CREATE)();
    CREATE ServerControl = (CREATE)GetProcAddress(m_hInst,"CreateServerControl");
    if(ServerControl)
    {
      m_pServerControl = static_cast<IServerControl*>(ServerControl());
    }
  }

  return true;
}

//复位桌子
VOID  CTableFrameSink::RepositionSink()
{
  //总下注数
  ZeroMemory(m_lAllJettonScore,sizeof(m_lAllJettonScore));

  //个人下注
  ZeroMemory(m_lUserJettonScore,sizeof(m_lUserJettonScore));

  //本局最后一注
  ZeroMemory(m_lLastAllJettonArea,sizeof(m_lLastAllJettonArea));

  //玩家成绩
  ZeroMemory(m_lUserWinScore,sizeof(m_lUserWinScore));
  ZeroMemory(m_lUserReturnScore,sizeof(m_lUserReturnScore));
  ZeroMemory(m_lUserRevenue,sizeof(m_lUserRevenue));

  //机器人控制
  m_nChipRobotCount = 0;
  ZeroMemory(m_lRobotAreaScore, sizeof(m_lRobotAreaScore));

  ZeroMemory(m_cbTableCardArray,sizeof(m_cbTableCardArray));

  m_cbShaYuAddMulti=0;
  m_bControl=false;
  m_lControlStorage=0L;

  return;
}

//游戏状态
bool  CTableFrameSink::IsUserPlaying(WORD wChairID)
{
  return true;
}

//查询限额
SCORE CTableFrameSink::QueryConsumeQuota(IServerUserItem * pIServerUserItem)
{

  if(pIServerUserItem->GetUserStatus() == US_PLAYING)
  {
    return 0L;
  }
  else
  {
    return __max(pIServerUserItem->GetUserScore()-m_pGameServiceOption->lMinTableScore, 0L);
  }
}

//查询服务费
bool CTableFrameSink::QueryBuckleServiceCharge(WORD wChairID)
{

  for(int nAreaIndex=1; nAreaIndex<=AREA_COUNT; ++nAreaIndex)
  {
    if(m_lUserJettonScore[nAreaIndex][wChairID]>0)
    {
      return true;
    }
  }
  return false;
}

//游戏开始
bool  CTableFrameSink::OnEventGameStart()
{
  //变量定义
  CMD_S_GameStart GameStart;
  ZeroMemory(&GameStart,sizeof(GameStart));

  //获取庄家
  IServerUserItem *pIBankerServerUserItem=NULL;
  if(INVALID_CHAIR!=m_wCurrentBanker)
  {
    pIBankerServerUserItem=m_pITableFrame->GetTableUserItem(m_wCurrentBanker);
  }

  //设置变量
  GameStart.cbTimeLeave=m_cbBetTime;
  GameStart.wBankerUser=m_wCurrentBanker;
  GameStart.lBankerScore = 0x7fffffffffffffff;
  GameStart.lStorageStart = m_lStorageCurrent;
  GameStart.lBonus = m_lBonus;
  if(pIBankerServerUserItem!=NULL)
  {
    GameStart.lBankerScore=pIBankerServerUserItem->GetUserScore();
  }

  GameStart.nChipRobotCount=m_nMaxChipRobot;

  //机器人控制
  m_nChipRobotCount = 0;

  //旁观玩家
  m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_START,&GameStart,sizeof(GameStart));

  //游戏玩家
  for(WORD wChairID=0; wChairID<GAME_PLAYER; ++wChairID)
  {
    IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(wChairID);
    if(pIServerUserItem==NULL)
    {
      continue;
    }

    if(m_bEnableSysBanker==false && m_wCurrentBanker==INVALID_CHAIR)
    {
      GameStart.lBankerScore = 1;
    }

    //设置积分
    GameStart.lUserMaxScore=min(pIServerUserItem->GetUserScore(),m_lUserLimitScore);

    m_pITableFrame->SendTableData(wChairID,SUB_S_GAME_START,&GameStart,sizeof(GameStart));
  }

  return true;
}

//游戏结束
bool  CTableFrameSink::OnEventGameConclude(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)
{
  switch(cbReason)
  {
    case GER_NORMAL:    //常规结束
    {
      if(m_pServerControl != NULL && m_pServerControl->NeedControl())
      {
        m_bControl=true;
        tagControlInfo ControlInfo;
        m_pServerControl->ReturnControlArea(ControlInfo,m_lAllJettonScore);
        m_cbTableCardArray[0] = ControlInfo.cbControlArea1;
        if(m_cbTableCardArray[0]==18)
        {
          INT cbControlArea[74] = { 6,  7,  8,   9,
                                    10, 11, 12, 13, 14, 15, 16, 17, 18,19,
                                    20, 21, 22, 23, 24, 25, 26, 27, 28,29,
                                    30, 31, 32, 33, 34, 35, 36, 37, 38,39,
                                    40, 41, 42, 43, 44, 45, 46, 47, 48,49,
                                    50, 51, 52, 53, 54, 55, 56, 57, 58,59,
                                    60, 61, 62, 63, 64, 65, 66, 67, 68,69,
                                    70, 71, 72, 73, 74, 75, 76, 77, 78,79
                                  };
          m_cbShaYuAddMulti=cbControlArea[rand()%74];

        }
        m_cbTableCardArray[1] = ControlInfo.cbControlArea2;
        m_pServerControl->CompleteControl();

        //系统输赢
        LONGLONG lSystemScore = 0l;   //系统实际输赢（机器人不算）
        LONGLONG lAllScore = 0l;    //系统全部输赢(机器人和玩家一样算)

        JudgeSystemScore(lSystemScore,lAllScore);
        m_lStorageCurrent+=lSystemScore;
        LONGLONG lTempDeduct=0;
        lTempDeduct=m_StorageDeduct;
        bool bDeduct=NeedDeductStorage();
        lTempDeduct=bDeduct?lTempDeduct:0;
        m_lStorageCurrent = m_lStorageCurrent - (m_lStorageCurrent * lTempDeduct / 1000);

        //彩金池
        m_lBonus += lAllScore;

        //彩金池比真实库存小
        if(m_lBonus <= m_lStorageCurrent)
        {
          m_lBonus = m_lStorageCurrent + rand() % 1000000;
        }
      }
      else
      {
        while(true)
        {
          //派发扑克
          DispatchTableCard();

          //试探性判断
          if(ProbeJudge() == TRUE)
          {
            break;
          }
        }
      }

      //保存上局总注
      CopyMemory(m_lLastAllJettonPlace,m_lUserJettonScore,sizeof(m_lLastAllJettonPlace));
      //清空本局最后一注
      ZeroMemory(m_lLastAllJettonArea,sizeof(m_lLastAllJettonArea));

      //计算分数
      LONGLONG lBankerWinScore=CalculateScore();
      //计算比例
      CalculateAnimalPercent();

      //写入库存
      WriteStorageInfo(m_lStorageCurrent,FALSE);

      //递增次数
      m_wBankerTime++;

      //结束消息
      CMD_S_GameEnd GameEnd;
      ZeroMemory(&GameEnd,sizeof(GameEnd));

      //庄家信息
      GameEnd.nBankerTime = m_wBankerTime;
      GameEnd.lBankerTotallScore=m_lBankerWinScore;
      GameEnd.lBankerScore=lBankerWinScore;
      GameEnd.cbShaYuAddMulti=m_cbShaYuAddMulti;
      //GameEnd.lStorageStart = m_lStorageCurrent;

      //倍率
      memcpy(GameEnd.nAnimalPercent, m_nAnimalPercent, sizeof(GameEnd.nAnimalPercent));

      //扑克信息
      CopyMemory(GameEnd.cbTableCardArray,m_cbTableCardArray,sizeof(m_cbTableCardArray));

      //发送积分
      GameEnd.cbTimeLeave=m_cbEndTime;

      if(m_cbTableCardArray[1]!=0)
      {
        GameEnd.cbTimeLeave+=m_cbAddTime;

      }
      for(WORD wUserIndex = 0; wUserIndex < GAME_PLAYER; ++wUserIndex)
      {
        IServerUserItem *pIServerUserItem = m_pITableFrame->GetTableUserItem(wUserIndex);
        if(pIServerUserItem == NULL)
        {
          continue;
        }

        //设置成绩
        GameEnd.lUserScore=m_lUserWinScore[wUserIndex];

        //返还积分
        GameEnd.lUserReturnScore=m_lUserReturnScore[wUserIndex];

        //设置税收
        if(m_lUserRevenue[wUserIndex]>0)
        {
          GameEnd.lRevenue=m_lUserRevenue[wUserIndex];
        }
        else if(m_wCurrentBanker!=INVALID_CHAIR)
        {
          GameEnd.lRevenue=m_lUserRevenue[m_wCurrentBanker];
        }
        else
        {
          GameEnd.lRevenue=0;
        }

        //发送消息
        m_pITableFrame->SendTableData(wUserIndex,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
        m_pITableFrame->SendLookonData(wUserIndex,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
      }

      return true;
    }
    case GER_USER_LEAVE:    //用户离开
    case GER_NETWORK_ERROR:
    {
      //闲家判断
      if(m_wCurrentBanker!=wChairID)
      {
        //变量定义
        LONGLONG lScore=0;
        LONGLONG lRevenue=0;

        //统计成绩
        for(int nAreaIndex=1; nAreaIndex<=AREA_COUNT; ++nAreaIndex)
        {
          lScore -= m_lUserJettonScore[nAreaIndex][wChairID];
        }

        //写入积分
        if(m_pITableFrame->GetGameStatus() != GS_GAME_END)
        {
          for(int nAreaIndex=1; nAreaIndex<=AREA_COUNT; ++nAreaIndex)
          {
            if(m_lUserJettonScore[nAreaIndex][wChairID] != 0)
            {
              CMD_S_PlaceJettonFail PlaceJettonFail;
              ZeroMemory(&PlaceJettonFail,sizeof(PlaceJettonFail));

              //游戏玩家
              for(WORD i=0; i<GAME_PLAYER; ++i)
              {
                IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(i);
                if(pIServerUserItem==NULL)
                {
                  continue;
                }

                m_pITableFrame->SendTableData(i,SUB_S_PLACE_JETTON_FAIL,&PlaceJettonFail,sizeof(PlaceJettonFail));
              }

              m_lAllJettonScore[nAreaIndex] -= m_lUserJettonScore[nAreaIndex][wChairID];
              m_lUserJettonScore[nAreaIndex][wChairID] = 0;
            }
          }
        }
        else
        {

          for(int nAreaIndex=1; nAreaIndex<=AREA_COUNT; ++nAreaIndex)
          {
            if(m_lUserJettonScore[nAreaIndex][wChairID] != 0)
            {
              m_lUserJettonScore[nAreaIndex][wChairID] = 0;
            }
          }

          //写入积分
          if(m_lUserWinScore[wChairID]!=0L)
          {

            tagScoreInfo ScoreInfo[GAME_PLAYER];
            ZeroMemory(ScoreInfo,sizeof(ScoreInfo));
            ScoreInfo[wChairID].cbType=(m_lUserWinScore[wChairID]>0L)?SCORE_TYPE_WIN:SCORE_TYPE_LOSE;
            ScoreInfo[wChairID].lRevenue=m_lUserRevenue[wChairID];
            ScoreInfo[wChairID].lScore=m_lUserWinScore[wChairID];
            m_pITableFrame->WriteTableScore(ScoreInfo,CountArray(ScoreInfo));

            m_lUserWinScore[wChairID] = 0;
          }
        }

      }
      //写入库存
      WriteStorageInfo(m_lStorageCurrent,FALSE);
      return true;
    }
  }

  return false;
}

//发送场景
bool  CTableFrameSink::OnEventSendGameScene(WORD wChiarID, IServerUserItem * pIServerUserItem, BYTE cbGameStatus, bool bSendSecret)
{
  switch(cbGameStatus)
  {
    case GAME_STATUS_FREE:      //空闲状态
    {
      //发送记录
      SendGameRecord(pIServerUserItem);

      //构造数据
      CMD_S_StatusFree StatusFree;
      ZeroMemory(&StatusFree,sizeof(StatusFree));

      //关闭续头
      m_bJettonState =false;
      //控制信息
      StatusFree.lApplyBankerCondition = m_lApplyBankerCondition;
      StatusFree.lAreaLimitScore = m_lAreaLimitScore;

      //庄家信息
      StatusFree.bEnableSysBanker=m_bEnableSysBanker;
      StatusFree.wBankerUser=m_wCurrentBanker;
      StatusFree.cbBankerTime=m_wBankerTime;
      StatusFree.lBankerWinScore=m_lBankerWinScore;
      StatusFree.lBankerScore = 0x7fffffffffffffff;
      StatusFree.lStorageStart = m_lStorageCurrent;
      StatusFree.lBonus = m_lBonus;

      //倍率
      memcpy(StatusFree.nAnimalPercent, m_nAnimalPercent, sizeof(StatusFree.nAnimalPercent));

      if(m_wCurrentBanker!=INVALID_CHAIR)
      {
        IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(m_wCurrentBanker);
        StatusFree.lBankerScore=pIServerUserItem->GetUserScore();
      }

      //玩家信息
      if(pIServerUserItem->GetUserStatus()!=US_LOOKON)
      {
        StatusFree.lUserMaxScore=min(pIServerUserItem->GetUserScore(),m_lUserLimitScore*4);
      }

      //全局信息
      DWORD dwPassTime=(DWORD)time(NULL)-m_dwJettonTime;
      StatusFree.cbTimeLeave=(BYTE)(m_cbFreeTime-__min(dwPassTime,m_cbFreeTime));

      //房间名称
      CopyMemory(StatusFree.szGameRoomName, m_pGameServiceOption->szServerName, sizeof(StatusFree.szGameRoomName));
      //机器人配置
      if(pIServerUserItem->IsAndroidUser())
      {
        tagCustomConfig *pCustomConfig = (tagCustomConfig *)m_pITableFrame->GetCustomRule();
        ASSERT(pCustomConfig);

        CopyMemory(&StatusFree.CustomAndroid, &pCustomConfig->CustomAndroid, sizeof(tagCustomAndroid));
      }
      //发送场景
      bool bSuccess = m_pITableFrame->SendGameScene(pIServerUserItem,&StatusFree,sizeof(StatusFree));

      //发送申请者
      SendApplyUser(pIServerUserItem);
      //更新库存信息
      if(CUserRight::IsGameCheatUser(pIServerUserItem->GetUserRight()))
      {
        CMD_S_UpdateStorage updateStorage;
        ZeroMemory(&updateStorage, sizeof(updateStorage));

        updateStorage.cbReqType = RQ_REFRESH_STORAGE;
        updateStorage.lStorageStart = m_lStorageStart;
        updateStorage.lStorageDeduct = m_StorageDeduct;
        updateStorage.lStorageCurrent = m_lStorageCurrent;
        updateStorage.lCurrentBonus = m_lBonus;
        updateStorage.lStorageMax1 = m_lStorageMax1;
        updateStorage.lStorageMul1 = m_lStorageMul1;
        updateStorage.lStorageMax2 = m_lStorageMax2;
        updateStorage.lStorageMul2 = m_lStorageMul2;

        m_pITableFrame->SendUserItemData(pIServerUserItem,SUB_S_UPDATE_STORAGE,&updateStorage,sizeof(updateStorage));
      }
      return bSuccess;
    }
    case GS_PLACE_JETTON:   //游戏状态
    case GS_GAME_END:     //结束状态
    {

      //发送记录
      SendGameRecord(pIServerUserItem);

      //构造数据
      CMD_S_StatusPlay StatusPlay= {0};

      //上局信息

      //关闭续头
      m_bJettonState = false;

      //全局下注
      CopyMemory(StatusPlay.lAllJettonScore,m_lAllJettonScore,sizeof(StatusPlay.lAllJettonScore));

      //玩家下注
      if(pIServerUserItem->GetUserStatus()!=US_LOOKON)
      {
        for(int nAreaIndex=1; nAreaIndex<=AREA_COUNT; ++nAreaIndex)
        {
          StatusPlay.lUserJettonScore[nAreaIndex] = m_lUserJettonScore[nAreaIndex][wChiarID];
        }

        //最大下注
        StatusPlay.lUserMaxScore=min(pIServerUserItem->GetUserScore(),m_lUserLimitScore);
      }

      //控制信息
      StatusPlay.lApplyBankerCondition=m_lApplyBankerCondition;
      StatusPlay.lAreaLimitScore=m_lAreaLimitScore;

      //庄家信息
      StatusPlay.bEnableSysBanker=m_bEnableSysBanker;
      StatusPlay.wBankerUser=m_wCurrentBanker;
      StatusPlay.cbBankerTime=m_wBankerTime;
      StatusPlay.lBankerWinScore=m_lBankerWinScore;
      StatusPlay.lBankerScore = 0x7fffffffffffffff;
      StatusPlay.lStorageStart = m_lStorageCurrent;
      StatusPlay.lBonus = m_lBonus;

      //倍率
      memcpy(StatusPlay.nAnimalPercent, m_nAnimalPercent, sizeof(StatusPlay.nAnimalPercent));

      if(m_wCurrentBanker!=INVALID_CHAIR)
      {
        IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(m_wCurrentBanker);
        StatusPlay.lBankerScore=pIServerUserItem->GetUserScore();
      }

      //全局信息
      DWORD dwPassTime=(DWORD)time(NULL)-m_dwJettonTime;
      StatusPlay.cbTimeLeave=(BYTE)(m_cbBetTime-__min(dwPassTime,m_cbBetTime));
      StatusPlay.cbGameStatus=m_pITableFrame->GetGameStatus();

      //结束判断
      if(cbGameStatus==GS_GAME_END)
      {

        BYTE cbEndTime=0;

        if(m_cbTableCardArray[1]!=0)
        {
          cbEndTime=m_cbEndTime+m_cbAddTime;
        }
        else
        {
          cbEndTime=m_cbEndTime;
        }

        StatusPlay.cbTimeLeave=(BYTE)(cbEndTime-__min(dwPassTime,cbEndTime));

        //设置成绩
        StatusPlay.lEndUserScore=m_lUserWinScore[wChiarID];

        //返还积分
        StatusPlay.lEndUserReturnScore=m_lUserReturnScore[wChiarID];

        //设置税收
        if(m_lUserRevenue[wChiarID]>0)
        {
          StatusPlay.lEndRevenue=m_lUserRevenue[wChiarID];
        }
        else if(m_wCurrentBanker!=INVALID_CHAIR)
        {
          StatusPlay.lEndRevenue=m_lUserRevenue[m_wCurrentBanker];
        }
        else
        {
          StatusPlay.lEndRevenue=0;
        }

        //庄家成绩
        StatusPlay.lEndBankerScore=m_lBankerCurGameScore;

        //扑克信息
        CopyMemory(StatusPlay.cbTableCardArray,m_cbTableCardArray,sizeof(m_cbTableCardArray));
      }

      //房间名称
      CopyMemory(StatusPlay.szGameRoomName, m_pGameServiceOption->szServerName, sizeof(StatusPlay.szGameRoomName));
      //机器人配置
      if(pIServerUserItem->IsAndroidUser())
      {
        tagCustomConfig *pCustomConfig = (tagCustomConfig *)m_pITableFrame->GetCustomRule();
        ASSERT(pCustomConfig);

        CopyMemory(&StatusPlay.CustomAndroid, &pCustomConfig->CustomAndroid, sizeof(tagCustomAndroid));
      }
      //发送场景
      bool bSuccess = m_pITableFrame->SendGameScene(pIServerUserItem,&StatusPlay,sizeof(StatusPlay));

      //发送申请者
      SendApplyUser(pIServerUserItem);
      //更新库存信息
      if(CUserRight::IsGameCheatUser(pIServerUserItem->GetUserRight()))
      {
        CMD_S_UpdateStorage updateStorage;
        ZeroMemory(&updateStorage, sizeof(updateStorage));

        updateStorage.cbReqType = RQ_REFRESH_STORAGE;
        updateStorage.lStorageStart = m_lStorageStart;
        updateStorage.lStorageDeduct = m_StorageDeduct;
        updateStorage.lStorageCurrent = m_lStorageCurrent;
        updateStorage.lCurrentBonus = m_lBonus;
        updateStorage.lStorageMax1 = m_lStorageMax1;
        updateStorage.lStorageMul1 = m_lStorageMul1;
        updateStorage.lStorageMax2 = m_lStorageMax2;
        updateStorage.lStorageMul2 = m_lStorageMul2;

        m_pITableFrame->SendUserItemData(pIServerUserItem,SUB_S_UPDATE_STORAGE,&updateStorage,sizeof(updateStorage));
      }
      return bSuccess;
    }
  }

  return false;
}

//定时器事件
bool  CTableFrameSink::OnTimerMessage(DWORD wTimerID, WPARAM wBindParam)
{
  switch(wTimerID)
  {
    case IDI_FREE:    //空闲时间
    {
      //开始游戏
      m_pITableFrame->StartGame();

      //设置时间
      m_dwJettonTime=(DWORD)time(NULL);
      m_pITableFrame->SetGameTimer(IDI_PLACE_JETTON,m_cbBetTime*1000,1,0L);

      //设置状态
      m_pITableFrame->SetGameStatus(GS_PLACE_JETTON);

      return true;
    }
    case IDI_PLACE_JETTON:    //下注时间
    {
      //状态判断(防止强退重复设置)
      if(m_pITableFrame->GetGameStatus()!=GS_GAME_END)
      {
        //设置状态
        m_pITableFrame->SetGameStatus(GS_GAME_END);

        //结束游戏
        OnEventGameConclude(INVALID_CHAIR,NULL,GER_NORMAL);

        //设置时间
        m_dwJettonTime=(DWORD)time(NULL);
        if(m_cbTableCardArray[1]!=0)
        {
          m_pITableFrame->SetGameTimer(IDI_GAME_END,(m_cbEndTime+m_cbAddTime)*1000,1,0L);
        }
        else
        {
          m_pITableFrame->SetGameTimer(IDI_GAME_END,m_cbEndTime*1000,1,0L);
        }
      }

      return true;
    }
    case IDI_GAME_END:      //结束游戏
    {
      tagScoreInfo ScoreInfo[GAME_PLAYER];
      ZeroMemory(ScoreInfo,sizeof(ScoreInfo));
      LONGLONG TempStartScore=0;
      //写入积分
      for(WORD wUserChairID = 0; wUserChairID < GAME_PLAYER; ++wUserChairID)
      {
        IServerUserItem *pIServerUserItem = m_pITableFrame->GetTableUserItem(wUserChairID);
        if(pIServerUserItem == NULL)
        {
          continue;
        }

        if(m_lUserWinScore[wUserChairID]!=0L)
        {
          ScoreInfo[wUserChairID].cbType=(m_lUserWinScore[wUserChairID]>0L)?SCORE_TYPE_WIN:SCORE_TYPE_LOSE;
          ScoreInfo[wUserChairID].lRevenue=m_lUserRevenue[wUserChairID];
          ScoreInfo[wUserChairID].lScore=m_lUserWinScore[wUserChairID];
        }
        if(!pIServerUserItem->IsAndroidUser())
        {
          TempStartScore -= m_lUserWinScore[wUserChairID];
        }

      }

      if(m_pServerControl!=NULL&&m_bControl)
      {
        m_lControlStorage=TempStartScore;
        CString cs;
        cs.Format(TEXT("当局损耗为：%I64d，账号为：%s"),m_lControlStorage,m_szControlName);
        //CTraceService::TraceString(cs,TraceLevel_Exception);
      }
      m_pITableFrame->WriteTableScore(ScoreInfo,CountArray(ScoreInfo));

      //结束游戏
      m_pITableFrame->ConcludeGame(GAME_STATUS_FREE);

      //ReadConfigInformation();
      //切换庄家
      ChangeBanker(false);

      //设置时间
      m_dwJettonTime=(DWORD)time(NULL);
      m_pITableFrame->SetGameTimer(IDI_FREE,m_cbFreeTime*1000,1,0L);

      //发送消息
      CMD_S_GameFree GameFree;
      ZeroMemory(&GameFree,sizeof(GameFree));

      GameFree.cbTimeLeave = m_cbFreeTime;
      GameFree.lStorageControl=m_StorageControl;
      GameFree.lStorageStart=m_lStorageCurrent;
      GameFree.lBonus = m_lBonus;
      m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_FREE,&GameFree,sizeof(GameFree));
      m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_FREE,&GameFree,sizeof(GameFree));

      return true;
    }
  }

  return false;
}

//游戏消息处理
bool  CTableFrameSink::OnGameMessage(WORD wSubCmdID, VOID * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
  switch(wSubCmdID)
  {
    case SUB_C_PLACE_JETTON:    //用户加注
    {
      //效验数据
      ASSERT(wDataSize==sizeof(CMD_C_PlaceJetton));
      if(wDataSize!=sizeof(CMD_C_PlaceJetton))
      {
        return false;
      }

      //用户效验

      tagUserInfo * pUserData=pIServerUserItem->GetUserInfo();

      if(pUserData->cbUserStatus!=US_PLAYING)
      {
        return true;
      }

      //消息处理
      CMD_C_PlaceJetton * pPlaceJetton=(CMD_C_PlaceJetton *)pDataBuffer;
      return OnUserPlaceJetton(pUserData->wChairID,pPlaceJetton->cbJettonArea,pPlaceJetton->lJettonScore);
    }
    case SUB_C_APPLY_BANKER:    //申请做庄
    {
      //用户效验
      tagUserInfo * pUserData=pIServerUserItem->GetUserInfo();
      if(pUserData->cbUserStatus==US_LOOKON)
      {
        return true;
      }

      return OnUserApplyBanker(pIServerUserItem);
    }
    case SUB_C_CANCEL_BANKER:   //取消做庄
    {
      //用户效验
      tagUserInfo * pUserData=pIServerUserItem->GetUserInfo();
      if(pUserData->cbUserStatus==US_LOOKON)
      {
        return true;
      }

      return OnUserCancelBanker(pIServerUserItem);
    }

    case SUB_C_ADMIN_COMMDN:
    {
      ASSERT(wDataSize==sizeof(CMD_C_ControlApplication));
      if(wDataSize!=sizeof(CMD_C_ControlApplication))
      {
        return false;
      }
      if(m_pServerControl == NULL)
      {
        return true;
      }

      if(!CUserRight::IsGameCheatUser(pIServerUserItem->GetUserRight()))
      {
        return false;
      }

      //设置库存
      const CMD_C_ControlApplication* AdminReq=static_cast<const CMD_C_ControlApplication*>(pDataBuffer);
      if(AdminReq->cbControlAppType==C_CA_SET_STORAGE)
      {
        m_lStorageCurrent=AdminReq->lSetStorage;
        m_StorageDeduct=AdminReq->lSetDeduct;
        m_pServerControl->SetStorage(m_lStorageCurrent);

        return true;
      }
      CopyMemory(m_szControlName,pIServerUserItem->GetNickName(),sizeof(m_szControlName));

      m_pServerControl->SetStorage(m_lStorageCurrent);

      return m_pServerControl->ServerControl(wSubCmdID, pDataBuffer, wDataSize, pIServerUserItem, m_pITableFrame);

    }
    case  SUB_C_CLEAN_JETTON:
    {
      //效验数据
      ASSERT(wDataSize==sizeof(CMD_C_CleanMeJetto));
      if(wDataSize!=sizeof(CMD_C_CleanMeJetto))
      {
        return false;
      }
      CMD_C_CleanMeJetto * pCleanMeJetton=(CMD_C_CleanMeJetto *)pDataBuffer;

      //ZeroMemory(m_lAllJettonScore,sizeof(m_lAllJettonScore));

      //ZeroMemory(m_lUserWinScore,sizeof(m_lUserWinScore));
      //ZeroMemory(m_lUserRevenue,sizeof(m_lUserRevenue));
      //ZeroMemory(m_lUserReturnScore,sizeof(m_lUserReturnScore));

      //ZeroMemory(m_lRobotAreaScore,sizeof(m_lRobotAreaScore));

      //上局信息
      //ZeroMemory(m_lLastAllJettonPlace,sizeof(m_lLastAllJettonPlace));
      //ZeroMemory(m_lLastAllJettonArea,sizeof(m_lLastAllJettonArea));

      //打开续投
      //m_bJettonState = true;


      //统计玩家下注
      for(int nAreaIndex=0; nAreaIndex<=AREA_COUNT; nAreaIndex++)
      {
        m_lAllJettonScore[nAreaIndex]-=m_lUserJettonScore[nAreaIndex][pCleanMeJetton->wChairID];
        m_lUserJettonScore[nAreaIndex][pCleanMeJetton->wChairID] = 0;
      }


      ////计算积分
      //for (WORD wChairID=0; wChairID<GAME_PLAYER; wChairID++)
      //{
      //  //庄家判断
      //  if (m_wCurrentBanker==wChairID) continue;

      //  //获取用户
      //  IServerUserItem * pIServerUserItem=m_pITableFrame->GetTableUserItem(wChairID);
      //  if (pIServerUserItem==NULL) continue;
      //
      //  //所有下注
      //  for (int nAreaIndex=0; nAreaIndex<=AREA_COUNT; nAreaIndex++)
      //      CurAllJetton[nAreaIndex] += m_lUserJettonScore[nAreaIndex][wChairID];
      //}


      //构造数据
      CMD_S_CeanJetton CleanJetton;
      ZeroMemory(&CleanJetton,sizeof(CleanJetton));
      CopyMemory(&CleanJetton.lAllCPlaceScore,m_lAllJettonScore,sizeof(CleanJetton.lAllCPlaceScore));


      //发送消息
      m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_CEAN_JETTON,&CleanJetton,sizeof(CleanJetton));
      m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_CEAN_JETTON,&CleanJetton,sizeof(CleanJetton));

      return true;
    }

    case SUB_C_UPDATE_STORAGE:    //更新库存
    {
      ASSERT(wDataSize==sizeof(CMD_C_UpdateStorage));
      if(wDataSize!=sizeof(CMD_C_UpdateStorage))
      {
        return false;
      }

      //权限判断
      if(CUserRight::IsGameCheatUser(pIServerUserItem->GetUserRight())==false)
      {
        return false;
      }

      //消息处理
      CMD_C_UpdateStorage * pUpdateStorage=(CMD_C_UpdateStorage *)pDataBuffer;
      if(pUpdateStorage->cbReqType==RQ_SET_STORAGE)
      {
        m_StorageDeduct = pUpdateStorage->lStorageDeduct;
        m_lStorageCurrent = pUpdateStorage->lStorageCurrent;
        m_lBonus = pUpdateStorage->lCurrentBonus;
        m_lStorageMax1 = pUpdateStorage->lStorageMax1;
        m_lStorageMul1 = pUpdateStorage->lStorageMul1;
        m_lStorageMax2 = pUpdateStorage->lStorageMax2;
        m_lStorageMul2 = pUpdateStorage->lStorageMul2;

        WriteStorageInfo(m_lStorageCurrent,TRUE);
      }
      return TRUE;
    }

    case SUB_C_CONTINUE_JETTON:
    {
      //效验数据
      ASSERT(wDataSize==sizeof(CMD_C_ContinueJetton));
      if(wDataSize!=sizeof(CMD_C_ContinueJetton))
      {
        return false;
      }
      CMD_C_ContinueJetton * pLastJetton=(CMD_C_ContinueJetton *)pDataBuffer;

      bool bCheckPlaceJettonIsOk = false;
      if(m_bJettonState == false)
      {
        return true;
      }




      //LONGLONG lLastJettonPlace = 0L;
      //LONGLONG lCurJettonPlace = 0L;

      //for (int nAreaIndex=0; nAreaIndex<=AREA_COUNT; nAreaIndex++)
      //  lLastJettonPlace+= m_lLastAllJettonPlace[nAreaIndex][pLastJetton->wChairID];

      //for (int nAreaIndex=0; nAreaIndex<=AREA_COUNT; nAreaIndex++)
      //  lCurJettonPlace+= m_lUserJettonScore[nAreaIndex][pLastJetton->wChairID];

      //if(lUserScore < lLastJettonPlace + lCurJettonPlace) return true;

      //
      //
      //if((lUserScoreCount + lUserJettonCount) <lUserScore){bCheckPlaceJettonIsOk = true;}
      //else{bCheckPlaceJettonIsOk = false;}


      //玩家积分
      IServerUserItem * pIServerUserItem=m_pITableFrame->GetTableUserItem(pLastJetton->wChairID);
      LONGLONG lUserScore = pIServerUserItem->GetUserScore();

      LONGLONG lLastAllJettonArea = 0L;
      LONGLONG lLastAllJettonPlace = 0L;
      LONGLONG lUserJettonScore = 0L;
      //统计本局最后下注
      for(int nAreaIndex=0; nAreaIndex<=AREA_COUNT; ++nAreaIndex)
      {
        lLastAllJettonArea +=m_lLastAllJettonArea[nAreaIndex][pLastJetton->wChairID];
      }

      //统计上局总下注
      for(int nAreaIndex=0; nAreaIndex<=AREA_COUNT; ++nAreaIndex)
      {
        lLastAllJettonPlace += m_lLastAllJettonPlace[nAreaIndex][pLastJetton->wChairID];
      }

      //统计本局已下注
      for(int nAreaIndex=0; nAreaIndex<=AREA_COUNT; ++nAreaIndex)
      {
        lUserJettonScore += m_lUserJettonScore[nAreaIndex][pLastJetton->wChairID];
      }

      bool bIsLast = false;
      //最后一注
      for(int nAreaIndex=0; nAreaIndex<=AREA_COUNT; ++nAreaIndex)
      {
        //本局有最后下一注
        if(m_lLastAllJettonArea[nAreaIndex][pLastJetton->wChairID] > 0)
        {
          bIsLast = false;
          break;
        }
        //本局无最后一注
        if(m_lLastAllJettonArea[nAreaIndex][pLastJetton->wChairID] == 0)
        {
          bIsLast = true;
        }
      }

      bCheckPlaceJettonIsOk=true;
      //判断用户金币是否足够
      if(bIsLast) //无最后一注
      {
        if(lUserScore-lUserJettonScore < lLastAllJettonPlace)
        {
          bCheckPlaceJettonIsOk=false;
        }
      }
      else
      {
        if(lUserScore-lUserJettonScore < lLastAllJettonArea)
        {
          bCheckPlaceJettonIsOk=false;
        }
      }
      if(!bCheckPlaceJettonIsOk)
      {
        //AfxMessageBox(L"用户金币不够！");
        return true;
      }

      //用户限制
      if(bIsLast) //无最后一注
      {
        if(lUserJettonScore+lLastAllJettonPlace > m_lUserLimitScore)
        {
          bCheckPlaceJettonIsOk=false;
        }
      }
      else
      {
        if(lUserJettonScore+lLastAllJettonArea > m_lUserLimitScore)
        {
          bCheckPlaceJettonIsOk=false;
        }
      }
      if(!bCheckPlaceJettonIsOk)
      {
        //AfxMessageBox(L"超过用户限制！");
        return true;
      }

      //区域限制
      if(bIsLast) //无最后一注
      {
        for(int nAreaIndex=0; nAreaIndex<=AREA_COUNT; nAreaIndex++)
        {
          if(m_lLastAllJettonPlace[nAreaIndex][pLastJetton->wChairID]+m_lUserJettonScore[nAreaIndex][pLastJetton->wChairID] > m_lAreaLimitScore)
          {
            bCheckPlaceJettonIsOk=false;
            break;
          }
        }
      }
      else  //有最后一注
      {
        for(int nAreaIndex=0; nAreaIndex<=AREA_COUNT; nAreaIndex++)
        {
          if(m_lLastAllJettonArea[nAreaIndex][pLastJetton->wChairID]+m_lUserJettonScore[nAreaIndex][pLastJetton->wChairID] > m_lAreaLimitScore)
          {
            bCheckPlaceJettonIsOk=false;
            break;
          }
        }

      }
      if(!bCheckPlaceJettonIsOk)
      {
        //AfxMessageBox(L"超过区域限制！");
        return true;
      }



      //保存数据
      LONGLONG  TemplAllJettonScore[AREA_COUNT+1];           //全体总注
      LONGLONG  TemplUserJettonScore[AREA_COUNT+1][GAME_PLAYER];   //个人总注
      LONGLONG  TemplLastAllJettonPlace[AREA_COUNT +1][GAME_PLAYER]; //上局总下注
      LONGLONG  TempLastAllJettonArea[AREA_COUNT +1][GAME_PLAYER];   //本局最后下注

      CopyMemory(TemplAllJettonScore,m_lAllJettonScore,sizeof(TemplAllJettonScore));
      CopyMemory(TemplUserJettonScore,m_lUserJettonScore,sizeof(m_lUserJettonScore));
      CopyMemory(TemplLastAllJettonPlace,m_lLastAllJettonPlace,sizeof(m_lLastAllJettonPlace));
      CopyMemory(TempLastAllJettonArea,m_lLastAllJettonArea,sizeof(TempLastAllJettonArea));

      /////////////////////////////////////////////////////////////////////

      //    m_lLastAllJettonPlace[AREA_COUNT +1][GAME_PLAYER]; //上局总下注

      //    m_lLastAllJettonArea[AREA_COUNT +1][GAME_PLAYER];  //本局最后一次操作

      /////////////////////////////////////////////////////////////////////

      //无最后一注 重复上一局总下注
      if(bIsLast == true)
      {
        if(bCheckPlaceJettonIsOk == true)
        {
          //设置数据
          for(int nAreaIndex=0; nAreaIndex<=AREA_COUNT; nAreaIndex++)
          {
            //上局总下注 不变

            //本局最后一次操作 不变   0 (游戏结束时清空)

            //全体总注
            TemplAllJettonScore[nAreaIndex]+=TemplLastAllJettonPlace[nAreaIndex][pLastJetton->wChairID];

            //个人总注
            TemplUserJettonScore[nAreaIndex][pLastJetton->wChairID]+=TemplLastAllJettonPlace[nAreaIndex][pLastJetton->wChairID];

          }
        }
      }

      //有最后一注 重复本局最后一注
      else
      {

        //有最后一注 只加一注
        if(bCheckPlaceJettonIsOk == true)
        {
          //设置数据
          for(int nAreaIndex=0; nAreaIndex<=AREA_COUNT; nAreaIndex++)
          {
            //上局总下注 不变     0 (在本局用户下一注时清空)

            //本局最后一次操作 不变

            //全体总注
            TemplAllJettonScore[nAreaIndex]+=TempLastAllJettonArea[nAreaIndex][pLastJetton->wChairID];

            //个人总注
            TemplUserJettonScore[nAreaIndex][pLastJetton->wChairID]+=TempLastAllJettonArea[nAreaIndex][pLastJetton->wChairID];

          }

        }
      }


      if(bCheckPlaceJettonIsOk)
      {
        //变量定义
        CMD_S_ContiueJetton PlaceLastJetton;
        ZeroMemory(&PlaceLastJetton,sizeof(PlaceLastJetton));

        //构造变量
        PlaceLastJetton.wChairID=pLastJetton->wChairID;
        //获取用户
        IServerUserItem * pIServerUserItem=m_pITableFrame->GetTableUserItem(pLastJetton->wChairID);
        if(pIServerUserItem != NULL)
        {
          PlaceLastJetton.cbAndroid = pIServerUserItem->IsAndroidUser()? TRUE : FALSE;
        }

        //设置变量
        CopyMemory(m_lAllJettonScore,TemplAllJettonScore,sizeof(TemplAllJettonScore));
        CopyMemory(m_lUserJettonScore,TemplUserJettonScore,sizeof(TemplUserJettonScore));
        CopyMemory(m_lLastAllJettonPlace,TemplLastAllJettonPlace,sizeof(TemplLastAllJettonPlace));
        CopyMemory(m_lLastAllJettonArea,TempLastAllJettonArea,sizeof(TempLastAllJettonArea));

        //LONGLONG  TemplAllJettonScore[AREA_COUNT+1];           //全体总注
        //LONGLONG  TemplUserJettonScore[AREA_COUNT+1][GAME_PLAYER];   //个人总注
        //LONGLONG  TemplLastAllJettonPlace[AREA_COUNT +1][GAME_PLAYER]; //上局总下注
        //LONGLONG  TempLastAllJettonArea[AREA_COUNT +1][GAME_PLAYER];   //本局最后下注

        CopyMemory(PlaceLastJetton.lAllJettonScore,m_lAllJettonScore,sizeof(PlaceLastJetton.lAllJettonScore));

        for(int nAreaIndex=0; nAreaIndex<=AREA_COUNT; nAreaIndex++)
        {
          PlaceLastJetton.lUserJettonScore[nAreaIndex] += m_lUserJettonScore[nAreaIndex][pLastJetton->wChairID];
        }

        //发送消息
        m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_CONTINU_JETTON,&PlaceLastJetton,sizeof(PlaceLastJetton));
        m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_CONTINU_JETTON,&PlaceLastJetton,sizeof(PlaceLastJetton));
      }

    }
    return true;
  }

  return false;
}


//框架消息处理
bool  CTableFrameSink::OnFrameMessage(WORD wSubCmdID, VOID * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
  return false;
}

//数据事件
bool CTableFrameSink::OnGameDataBase(WORD wRequestID, VOID * pData, WORD wDataSize)
{
  return false;
}

//用户坐下
bool  CTableFrameSink::OnActionUserSitDown(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
  //起始分数
  m_lUserStartScore[wChairID] = pIServerUserItem->GetUserScore();
  //设置时间
  if((bLookonUser==false)&&(m_dwJettonTime==0L))
  {
    m_dwJettonTime=(DWORD)time(NULL);
    m_pITableFrame->SetGameTimer(IDI_FREE,m_cbFreeTime*1000,1,NULL);
    m_pITableFrame->SetGameStatus(GAME_STATUS_FREE);
  }
  return true;
}

//用户起来
bool  CTableFrameSink::OnActionUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
  //起始分数
  m_lUserStartScore[wChairID] = 0;
  //记录成绩
  if(bLookonUser==false)
  {
    //切换庄家
    if(wChairID==m_wCurrentBanker)
    {
      ChangeBanker(true);
    }

    //取消申请
    for(WORD i=0; i<m_ApplyUserArray.GetCount(); ++i)
    {
      if(wChairID!=m_ApplyUserArray[i])
      {
        continue;
      }

      //删除玩家
      m_ApplyUserArray.RemoveAt(i);

      //构造变量
      CMD_S_CancelBanker CancelBanker;
      ZeroMemory(&CancelBanker,sizeof(CancelBanker));

      tagUserInfo * pUserInfo = pIServerUserItem->GetUserInfo();
      //设置变量
      lstrcpyn(CancelBanker.szCancelUser,pUserInfo->szNickName,CountArray(CancelBanker.szCancelUser));

      //发送消息
      m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));
      m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));

      break;
    }

    return true;
  }

  return true;
}

//加注事件
bool CTableFrameSink::OnUserPlaceJetton(WORD wChairID, BYTE cbJettonArea, LONGLONG lJettonScore)
{
  //效验参数
  ASSERT((cbJettonArea<=AREA_COUNT && cbJettonArea>=1)&&(lJettonScore>0L));
  if((cbJettonArea>AREA_COUNT)||(lJettonScore<=0L) || cbJettonArea<1)
  {
    return true;
  }

  if(m_pITableFrame->GetGameStatus()!=GS_PLACE_JETTON)
  {
    CMD_S_PlaceJettonFail PlaceJettonFail;
    ZeroMemory(&PlaceJettonFail,sizeof(PlaceJettonFail));
    PlaceJettonFail.lJettonArea=cbJettonArea;
    PlaceJettonFail.lPlaceScore=lJettonScore;
    PlaceJettonFail.wPlaceUser=wChairID;

    //发送消息
    m_pITableFrame->SendTableData(wChairID,SUB_S_PLACE_JETTON_FAIL,&PlaceJettonFail,sizeof(PlaceJettonFail));
    return true;
  }

  //庄家判断
  if(m_wCurrentBanker==wChairID)
  {
    return true;
  }
  if(m_bEnableSysBanker==false && m_wCurrentBanker==INVALID_CHAIR)
  {
    return true;
  }

  //变量定义
  IServerUserItem * pIServerUserItem=m_pITableFrame->GetTableUserItem(wChairID);
  LONGLONG lJettonCount=0L;
  LONGLONG lJettonPlace = 0L;
  for(int nAreaIndex=0; nAreaIndex<=AREA_COUNT; ++nAreaIndex)
  {
    lJettonCount += m_lUserJettonScore[nAreaIndex][wChairID];
  }

  //玩家积分
  LONGLONG lUserScore = pIServerUserItem->GetUserScore();

  //合法校验
  if(lUserScore < lJettonCount + lJettonScore)
  {
    return true;
  }
  if(m_lUserLimitScore < lJettonCount + lJettonScore)
  {
    return true;
  }

  //成功标识
  bool bPlaceJettonSuccess=true;
  //打开续投
  m_bJettonState = true;

  //合法验证
  if(GetUserMaxJetton(wChairID,cbJettonArea) >= lJettonScore)
  {
    //机器人验证
    if(pIServerUserItem->IsAndroidUser())
    {
      //区域限制
      if(m_lRobotAreaScore[cbJettonArea] + lJettonScore > m_lRobotAreaLimit)
      {
        return true;
      }

      //数目限制
      bool bHaveChip = false;
      for(int i = 0; i < AREA_COUNT; i++)
      {
        if(m_lUserJettonScore[i+1][wChairID] != 0)
        {
          bHaveChip = true;
        }
      }

      if(!bHaveChip)
      {
        if(m_nChipRobotCount+1 > m_nMaxChipRobot)
        {
          bPlaceJettonSuccess = false;
        }
        else
        {
          m_nChipRobotCount++;
        }
      }

      //统计分数
      if(bPlaceJettonSuccess)
      {
        m_lRobotAreaScore[cbJettonArea] += lJettonScore;
      }
    }


    if(bPlaceJettonSuccess)
    {

      for(int nAreaIndex=0; nAreaIndex<=AREA_COUNT; ++nAreaIndex)
      {
        //清空上局总注
        m_lLastAllJettonPlace[nAreaIndex][wChairID]=0;
        //清空本局最后一注
        m_lLastAllJettonArea[nAreaIndex][wChairID]=0;
      }
      //保存本局最后一注
      m_lLastAllJettonArea[cbJettonArea][wChairID] += lJettonScore;

      //全体总注
      m_lAllJettonScore[cbJettonArea] += lJettonScore;

      //个人总注
      m_lUserJettonScore[cbJettonArea][wChairID] += lJettonScore;
    }
  }
  else
  {
    bPlaceJettonSuccess=false;
  }

  if(bPlaceJettonSuccess)
  {
    //变量定义
    CMD_S_PlaceJetton PlaceJetton;
    ZeroMemory(&PlaceJetton,sizeof(PlaceJetton));

    CopyMemory(&PlaceJetton.lUserStartScore, m_lUserStartScore, sizeof(m_lUserStartScore));
    //构造变量
    PlaceJetton.wChairID=wChairID;
    PlaceJetton.cbJettonArea=cbJettonArea;
    PlaceJetton.lJettonScore=lJettonScore;

    for(int nAreaIndex=0; nAreaIndex<=AREA_COUNT; ++nAreaIndex)
    {
      PlaceJetton.lUserJettonScore[nAreaIndex] += m_lUserJettonScore[nAreaIndex][wChairID];
    }

    //获取用户
    IServerUserItem * pIServerUserItem=m_pITableFrame->GetTableUserItem(wChairID);
    if(pIServerUserItem != NULL)
    {
      PlaceJetton.cbAndroid = pIServerUserItem->IsAndroidUser()? TRUE : FALSE;
    }

    //发送消息
    m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_PLACE_JETTON,&PlaceJetton,sizeof(PlaceJetton));
    m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_PLACE_JETTON,&PlaceJetton,sizeof(PlaceJetton));
  }
  else
  {
    CMD_S_PlaceJettonFail PlaceJettonFail;
    ZeroMemory(&PlaceJettonFail,sizeof(PlaceJettonFail));
    PlaceJettonFail.lJettonArea=cbJettonArea;
    PlaceJettonFail.lPlaceScore=lJettonScore;
    PlaceJettonFail.wPlaceUser=wChairID;

    //发送消息
    m_pITableFrame->SendTableData(wChairID,SUB_S_PLACE_JETTON_FAIL,&PlaceJettonFail,sizeof(PlaceJettonFail));
  }

  return true;
}
void CTableFrameSink::RandList(BYTE cbCardBuffer[], BYTE cbBufferCount)
{

  //混乱准备
  BYTE *cbCardData = new BYTE[cbBufferCount];
  CopyMemory(cbCardData,cbCardBuffer,cbBufferCount);

  //混乱扑克
  BYTE cbRandCount=0,cbPosition=0;
  do
  {
    cbPosition=rand()%(cbBufferCount-cbRandCount);
    cbCardBuffer[cbRandCount++]=cbCardData[cbPosition];
    cbCardData[cbPosition]=cbCardData[cbBufferCount-cbRandCount];
  }
  while(cbRandCount<cbBufferCount);

  delete []cbCardData;
  cbCardData = NULL;

  return;
}


//发送扑克
void CTableFrameSink::DispatchTableCard()
{
  //清空扑克
  ZeroMemory(m_cbTableCardArray,sizeof(m_cbTableCardArray));

  //获取扑克
  m_cbTableCardArray[0] = GetRandCardValue();

  //通杀
  if(m_cbTableCardArray[0] == 11)
  {
    m_cbShaYuAddMulti = 0;
    return;
  }

  //通赔
  if(m_cbTableCardArray[0] == 25)
  {
    m_cbShaYuAddMulti = 0;
    return;
  }

  //金鲨
  if(m_cbTableCardArray[0] == 4)
  {
    m_cbShaYuAddMulti =0;
    return;
  }

  //鲨鱼 随机奖励
  if(m_cbTableCardArray[0]%7==4)
  {
    m_cbTableCardArray[1]=GetAnotherRandCardValue();
    if(m_cbTableCardArray[1] == m_cbTableCardArray[0])
    {
      while(true)
      {
        m_cbTableCardArray[1]=GetAnotherRandCardValue();
        if(m_cbTableCardArray[1] != m_cbTableCardArray[0])
        {
          break;
        }
      }
    }


    INT cbControlArea[74] = { 6,  7,  8,   9,
                              10, 11, 12, 13, 14, 15, 16, 17, 18,19,
                              20, 21, 22, 23, 24, 25, 26, 27, 28,29,
                              30, 31, 32, 33, 34, 35, 36, 37, 38,39,
                              40, 41, 42, 43, 44, 45, 46, 47, 48,49,
                              50, 51, 52, 53, 54, 55, 56, 57, 58,59,
                              60, 61, 62, 63, 64, 65, 66, 67, 68,69,
                              70, 71, 72, 73, 74, 75, 76, 77, 78,79
                            };
    INT cbnChance[74]   = { 80,  80,  80,  80,
                            50, 50, 50, 50, 50, 50, 50, 50, 50,50,
                            30, 30, 30, 30, 30, 30, 30, 30, 30,30,
                            20, 20, 20, 20, 20, 20, 20, 20, 20,20,
                            7, 7, 7, 7, 7, 7, 7, 7, 7,7,
                            6, 6, 6, 6, 6, 6, 6, 6, 6,6,
                            5, 5, 5, 5, 5, 5, 5, 5, 5,5,
                            4, 4, 4, 4, 4, 4, 4, 4, 4,4
                          };


    m_GameLogic.RandArray(cbControlArea, CountArray(cbControlArea), cbnChance, CountArray(cbnChance));

    //随机倍数
    DWORD wTick = GetTickCount();

    //几率和值
    INT nChanceAndValue = 0;
    for(int n = 0; n < CountArray(cbnChance); ++n)
    {
      nChanceAndValue += cbnChance[n];
    }

    INT nMuIndex = 0;
    int nRandNum = 0;         //随机辅助
    static int nStFluc = 1;
    nRandNum = (rand() + wTick + nStFluc*3) % nChanceAndValue;
    for(int j = 0; j < CountArray(cbnChance); j++)
    {
      nRandNum -= cbnChance[j];
      if(nRandNum < 0)
      {
        nMuIndex = j;
        break;
      }
    }
    nStFluc = nStFluc%3 + 1;

    m_cbShaYuAddMulti=cbControlArea[nMuIndex]; //6-100随机
  }
  else
  {
    m_cbShaYuAddMulti = 0;
  }
}

//获取数值
BYTE CTableFrameSink::GetRandCardValue()
{
  int n1= m_nMultiple[ID_TU_ZI-1] ;
  int n2=m_nMultiple[ID_YAN_ZI-1] ;
  int n3=m_nMultiple[ID_GE_ZI-1] ;
  int n4=m_nMultiple[ID_KONG_QUE-1] ;
  int n5=m_nMultiple[ID_LAO_YING-1] ;
  int n6=m_nMultiple[ID_SHI_ZI-1] ;
  int n7=m_nMultiple[ID_XIONG_MAO-1] ;
  int n8=m_nMultiple[ID_HOU_ZI-1] ;
  int n9=m_nMultiple[ID_SHA_YU-1];
  int n10=m_nMultiple[9];     //通杀
  int n11=m_nMultiple[10];    //通赔
  int n12=m_nMultiple[11];    //金鲨

  INT cbControlArea[28] = { 1, 2, 3, 4,  5, 6, 7, 8, 9, 10,  11,  12,13,14, 15,16,17, 18,  19,20, 21, 22, 23, 24,  25, 26, 27, 28 };
  INT cbnChance[28] = { n1,n1,n1,    n12,  n2, n2, n2, n3,n3,n3, n10, n4,n4,n4,n5,n5,n5, n9,  n6, n6, n6, n7,n7,n7,  n11, n8, n8, n8 };

  m_GameLogic.RandArray(cbControlArea, CountArray(cbControlArea), cbnChance, CountArray(cbnChance));

  //随机倍数
  DWORD wTick = GetTickCount();

  //几率和值
  INT nChanceAndValue = 0;
  for(int n = 0; n < CountArray(cbnChance); ++n)
  {
    nChanceAndValue += cbnChance[n];
  }

  INT nMuIndex = 0;
  int nRandNum = 0;         //随机辅助
  static int nStFluc = 1;
  nRandNum = (rand() + wTick + nStFluc*3) % nChanceAndValue;
  for(int j = 0; j < CountArray(cbnChance); j++)
  {
    nRandNum -= cbnChance[j];
    if(nRandNum < 0)
    {
      nMuIndex = j;
      break;
    }
  }
  nStFluc = nStFluc%3 + 1;

  return cbControlArea[nMuIndex];
}

//获取数值
BYTE CTableFrameSink::GetAnotherRandCardValue()
{
  int n1=m_nMultiple[ID_TU_ZI-1] ;
  int n2=m_nMultiple[ID_YAN_ZI-1] ;
  int n3=m_nMultiple[ID_GE_ZI-1] ;
  int n4=m_nMultiple[ID_KONG_QUE-1] ;
  int n5=m_nMultiple[ID_LAO_YING-1] ;
  int n6=m_nMultiple[ID_SHI_ZI-1] ;
  int n7=m_nMultiple[ID_XIONG_MAO-1] ;
  int n8=m_nMultiple[ID_HOU_ZI-1] ;
  int n9=m_nMultiple[ID_SHA_YU-1];
  int n10=m_nMultiple[9];     //通杀
  int n11=m_nMultiple[10];    //通赔
  int n12=m_nMultiple[11];    //金鲨

  INT cbControlArea[24] = {1,2,3, 5,6,7, 8,9,10, 12,13,14, 15,16,17, 19,20,21, 22,23,24,26,27,28};

  INT cbnChance[24]= { n1,n1,n1, n2,n2,n2, n3,n3,n3, n4,n4,n4, n5,n5,n5, n6,n6,n6, n7,n7,n7, n8,n8,n8 };

  m_GameLogic.RandArray(cbControlArea, CountArray(cbControlArea), cbnChance, CountArray(cbnChance));

  //随机倍数
  DWORD wTick = GetTickCount();

  //几率和值
  INT nChanceAndValue = 0;
  for(int n = 0; n < CountArray(cbnChance); ++n)
  {
    nChanceAndValue += cbnChance[n];
  }

  INT nMuIndex = 0;
  int nRandNum = 0;         //随机辅助
  static int nStFluc = 1;
  nRandNum = (rand() + wTick + nStFluc*3) % nChanceAndValue;
  for(int j = 0; j < CountArray(cbnChance); j++)
  {
    nRandNum -= cbnChance[j];
    if(nRandNum < 0)
    {
      nMuIndex = j;
      break;
    }
  }
  nStFluc = nStFluc%3 + 1;

  return cbControlArea[nMuIndex];

}

//申请庄家
bool CTableFrameSink::OnUserApplyBanker(IServerUserItem *pIApplyServerUserItem)
{
  //合法判断
  LONGLONG lUserScore=pIApplyServerUserItem->GetUserScore();
  if(lUserScore<m_lApplyBankerCondition)
  {
    m_pITableFrame->SendGameMessage(pIApplyServerUserItem,TEXT("你的金币不足以申请庄家，申请失败！"),SMT_CHAT|SMT_EJECT);
    return true;
  }

  if(pIApplyServerUserItem->IsAndroidUser()&&(m_ApplyUserArray.GetCount())>m_nRobotListMaxCount)
  {
    return true;
  }
  //存在判断
  WORD wApplyUserChairID=pIApplyServerUserItem->GetChairID();
  for(INT_PTR nUserIdx=0; nUserIdx<m_ApplyUserArray.GetCount(); ++nUserIdx)
  {
    WORD wChairID=m_ApplyUserArray[nUserIdx];
    if(wChairID==wApplyUserChairID)
    {
      m_pITableFrame->SendGameMessage(pIApplyServerUserItem,TEXT("你已经申请了庄家，不需要再次申请！"),SMT_CHAT|SMT_EJECT);
      return true;
    }
  }

  //保存信息
  m_ApplyUserArray.Add(wApplyUserChairID);

  //构造变量
  CMD_S_ApplyBanker ApplyBanker;
  ZeroMemory(&ApplyBanker,sizeof(ApplyBanker));

  //设置变量
  ApplyBanker.wApplyUser=wApplyUserChairID;

  //发送消息
  m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_APPLY_BANKER, &ApplyBanker, sizeof(ApplyBanker));
  m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_APPLY_BANKER, &ApplyBanker, sizeof(ApplyBanker));

  //切换判断
  if(m_pITableFrame->GetGameStatus()==GAME_STATUS_FREE && m_ApplyUserArray.GetCount()==1)
  {
    ChangeBanker(false);
  }

  return true;
}

//取消申请
bool CTableFrameSink::OnUserCancelBanker(IServerUserItem *pICancelServerUserItem)
{
  //当前庄家
  if(pICancelServerUserItem->GetChairID()==m_wCurrentBanker && m_pITableFrame->GetGameStatus()!=GAME_STATUS_FREE)
  {
    //发送消息
    m_pITableFrame->SendGameMessage(pICancelServerUserItem,TEXT("游戏已经开始，不可以取消当庄！"),SMT_CHAT|SMT_EJECT);
    return true;
  }

  //存在判断
  for(WORD i=0; i<m_ApplyUserArray.GetCount(); ++i)
  {
    //获取玩家
    WORD wChairID=m_ApplyUserArray[i];
    IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(wChairID);

    //条件过滤
    if(pIServerUserItem==NULL)
    {
      continue;
    }
    if(pIServerUserItem->GetUserID()!=pICancelServerUserItem->GetUserID())
    {
      continue;
    }

    //删除玩家
    m_ApplyUserArray.RemoveAt(i);

    if(m_wCurrentBanker!=wChairID)
    {
      //构造变量
      CMD_S_CancelBanker CancelBanker;
      ZeroMemory(&CancelBanker,sizeof(CancelBanker));

      tagUserInfo * pUserInfo = pIServerUserItem->GetUserInfo();
      //设置变量
      lstrcpyn(CancelBanker.szCancelUser,pUserInfo->szNickName,CountArray(CancelBanker.szCancelUser));

      //发送消息
      m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));
      m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));
    }
    else if(m_wCurrentBanker==wChairID)
    {
      //切换庄家
      m_wCurrentBanker=INVALID_CHAIR;
      ChangeBanker(true);
    }

    return true;
  }

  return true;
}

//更换庄家
bool CTableFrameSink::ChangeBanker(bool bCancelCurrentBanker)
{
  //切换标识
  bool bChangeBanker=false;

  //取消当前
  if(bCancelCurrentBanker)
  {
    for(WORD i=0; i<m_ApplyUserArray.GetCount(); ++i)
    {
      //获取玩家
      WORD wChairID=m_ApplyUserArray[i];

      //条件过滤
      if(wChairID!=m_wCurrentBanker)
      {
        continue;
      }

      //删除玩家
      m_ApplyUserArray.RemoveAt(i);

      break;
    }

    //设置庄家
    m_wCurrentBanker=INVALID_CHAIR;

    //轮换判断
    if(m_pITableFrame->GetGameStatus()==GAME_STATUS_FREE && FindSuitBanker())
    {
      m_wCurrentBanker=m_ApplyUserArray[0];
    }

    //设置变量
    bChangeBanker=true;
    m_bExchangeBanker = true;
  }
  //轮庄判断
  else if(m_wCurrentBanker!=INVALID_CHAIR)
  {
    //获取庄家
    IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(m_wCurrentBanker);

    if(pIServerUserItem!= NULL)
    {
      LONGLONG lBankerScore=pIServerUserItem->GetUserScore();

      //次数判断
      if(m_lPlayerBankerMAX<=m_wBankerTime || lBankerScore<m_lApplyBankerCondition)
      {
        //庄家增加判断 同一个庄家情况下只判断一次
        if(m_lPlayerBankerMAX <= m_wBankerTime && m_bExchangeBanker)
        {
          bool bScoreMAX = true;
          m_bExchangeBanker = false;

          for(WORD i=0; i<m_ApplyUserArray.GetCount(); ++i)
          {
            //获取玩家
            WORD wChairID = m_ApplyUserArray[i];
            IServerUserItem *pIUserItem = m_pITableFrame->GetTableUserItem(wChairID);
            LONGLONG lScore = pIUserItem->GetUserScore();

            if(wChairID != m_wCurrentBanker && lBankerScore <= lScore)
            {
              bScoreMAX = false;
              break;
            }
          }

          LONGLONG wAddTimes=0;
          if(bScoreMAX || (lBankerScore > m_lBankerScoreMAX && m_lBankerScoreMAX != 0l))
          {
            if(bScoreMAX)
            {
              wAddTimes=m_lBankerAdd;
            }
            if(lBankerScore > m_lBankerScoreMAX && m_lBankerScoreMAX != 0l)
            {
              wAddTimes+=m_lBankerScoreAdd;
            }

            m_lPlayerBankerMAX+=wAddTimes;
            tagUserInfo * pUserInfo = pIServerUserItem->GetUserInfo();
            //提示消息
            TCHAR szTipMsg[128] = {};
            if(wAddTimes==m_lBankerAdd)
            {
              myprintf(szTipMsg,CountArray(szTipMsg),TEXT("[ %s ]的分数超过其他申请上庄玩家，获得%lld次额外坐庄次数!"),pUserInfo->szNickName,wAddTimes);
            }
            else if(wAddTimes==m_lBankerScoreAdd)
            {
              myprintf(szTipMsg,CountArray(szTipMsg),TEXT("[ %s ]的分数超过[%I64d]，获得%lld次额外坐庄次数!"),pUserInfo->szNickName,m_lBankerScoreMAX,wAddTimes);
            }
            else if(wAddTimes==(m_lBankerScoreAdd+m_lBankerAdd))
            {
              myprintf(szTipMsg,CountArray(szTipMsg),TEXT("[ %s ]的分数超过[%I64d]且超过其他申请上庄玩家，获得%lld次额外坐庄次数!"),pUserInfo->szNickName,m_lBankerScoreMAX,wAddTimes);
            }

            if(wAddTimes != 0)
            {
              //发送消息
              SendGameMessage(INVALID_CHAIR,szTipMsg);
              return true;
            }
            return true;
          }
        }

        //撤销玩家
        for(WORD i=0; i<m_ApplyUserArray.GetCount(); ++i)
        {
          //获取玩家
          WORD wChairID=m_ApplyUserArray[i];

          //条件过滤
          if(wChairID!=m_wCurrentBanker)
          {
            continue;
          }

          //删除玩家
          m_ApplyUserArray.RemoveAt(i);

          break;
        }

        //设置庄家
        m_wCurrentBanker=INVALID_CHAIR;

        //轮换判断
        if(FindSuitBanker())
        {
          m_wCurrentBanker=m_ApplyUserArray[0];
        }

        bChangeBanker=true;
        m_bExchangeBanker = true;
        tagUserInfo * pUserInfo = pIServerUserItem->GetUserInfo();
        //提示消息
        TCHAR szTipMsg[128];
        if(lBankerScore<m_lApplyBankerCondition)
        {
          myprintf(szTipMsg,CountArray(szTipMsg),TEXT("[ %s ]分数少于(%I64d)，强行换庄!"),pUserInfo->szNickName,m_lApplyBankerCondition);
        }
        else
        {
          myprintf(szTipMsg,CountArray(szTipMsg),TEXT("[ %s ]做庄次数达到(%lld)，强行换庄!"),pUserInfo->szNickName,m_lPlayerBankerMAX);
        }

        //发送消息
        SendGameMessage(INVALID_CHAIR,szTipMsg);
      }
    }
    else
    {
      for(WORD i=0; i<m_ApplyUserArray.GetCount(); ++i)
      {
        //获取玩家
        WORD wChairID=m_ApplyUserArray[i];

        //条件过滤
        if(wChairID!=m_wCurrentBanker)
        {
          continue;
        }

        //删除玩家
        m_ApplyUserArray.RemoveAt(i);

        break;
      }
      //设置庄家
      m_wCurrentBanker=INVALID_CHAIR;
    }

  }
  //系统做庄
  else if(m_wCurrentBanker==INVALID_CHAIR &&FindSuitBanker())
  {
    m_wCurrentBanker=m_ApplyUserArray[0];
    bChangeBanker=true;
    m_bExchangeBanker = true;
  }

  //切换判断
  if(bChangeBanker)
  {
    //最大坐庄数
    m_lPlayerBankerMAX = m_lBankerMAX;

    //设置变量
    m_wBankerTime = 0;
    m_lBankerWinScore=0;

    //发送消息
    CMD_S_ChangeBanker sChangeBanker;
    ZeroMemory(&sChangeBanker,sizeof(sChangeBanker));
    sChangeBanker.wBankerUser=m_wCurrentBanker;
    sChangeBanker.lBankerScore = 0x7fffffffffffffff;
    if(m_wCurrentBanker!=INVALID_CHAIR)
    {
      IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(m_wCurrentBanker);
      sChangeBanker.lBankerScore=pIServerUserItem->GetUserScore();
    }
    m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_CHANGE_BANKER,&sChangeBanker,sizeof(sChangeBanker));
    m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_CHANGE_BANKER,&sChangeBanker,sizeof(sChangeBanker));

    if(m_wCurrentBanker!=INVALID_CHAIR)
    {
      //读取消息
      LONGLONG lMessageCount=GetPrivateProfileInt(m_szGameRoomName,TEXT("MessageCount"),0,m_szConfigFileName);
      if(lMessageCount!=0)
      {
        //读取配置
        LONGLONG lIndex=rand()%lMessageCount;
        TCHAR szKeyName[32],szMessage1[256],szMessage2[256];
        myprintf(szKeyName,CountArray(szKeyName),TEXT("Item%I64d"),lIndex);
        GetPrivateProfileString(m_szGameRoomName,szKeyName,TEXT("恭喜[ %s ]上庄"),szMessage1,CountArray(szMessage1),m_szConfigFileName);

        //获取玩家
        IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(m_wCurrentBanker);
        tagUserInfo * pUserInfo = pIServerUserItem->GetUserInfo();
        //发送消息
        myprintf(szMessage2,CountArray(szMessage2),szMessage1,pUserInfo->szNickName);
        SendGameMessage(INVALID_CHAIR,szMessage2);
      }
    }
  }

  return bChangeBanker;
}

//发送庄家
void CTableFrameSink::SendApplyUser(IServerUserItem *pRcvServerUserItem)
{
  for(INT_PTR nUserIdx=0; nUserIdx<m_ApplyUserArray.GetCount(); ++nUserIdx)
  {
    WORD wChairID=m_ApplyUserArray[nUserIdx];

    //获取玩家
    IServerUserItem *pServerUserItem=m_pITableFrame->GetTableUserItem(wChairID);
    if(!pServerUserItem)
    {
      continue;
    }

    //庄家判断
    if(pServerUserItem->GetChairID()==m_wCurrentBanker)
    {
      continue;
    }

    //构造变量
    CMD_S_ApplyBanker ApplyBanker;
    ApplyBanker.wApplyUser=wChairID;

    //发送消息
    m_pITableFrame->SendUserItemData(pRcvServerUserItem, SUB_S_APPLY_BANKER, &ApplyBanker, sizeof(ApplyBanker));
  }
}

//用户断线
bool  CTableFrameSink::OnActionUserOffLine(WORD wChairID, IServerUserItem * pIServerUserItem)
{
  //切换庄家
  if(wChairID==m_wCurrentBanker)
  {
    ChangeBanker(true);
  }

  //取消申请
  for(WORD i=0; i<m_ApplyUserArray.GetCount(); ++i)
  {
    if(wChairID!=m_ApplyUserArray[i])
    {
      continue;
    }

    //删除玩家
    m_ApplyUserArray.RemoveAt(i);

    //构造变量
    CMD_S_CancelBanker CancelBanker;
    ZeroMemory(&CancelBanker,sizeof(CancelBanker));
    tagUserInfo * pUserInfo = pIServerUserItem->GetUserInfo();
    //设置变量
    lstrcpyn(CancelBanker.szCancelUser,pUserInfo->szNickName,CountArray(CancelBanker.szCancelUser));

    //发送消息
    m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));
    m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));

    break;
  }

  return true;
}
void   CTableFrameSink::GetAllWinArea(BYTE bcWinArea[],BYTE bcAreaCount,BYTE InArea)
{
  if(InArea==0xFF)
  {
    return ;
  }
  ZeroMemory(bcWinArea,bcAreaCount);


  LONGLONG lMaxSocre = 0;

  for(int i = 0; i<ANIMAL_COUNT; i++)
  {
    BYTE bcOutCadDataWin[AREA_COUNT];
    BYTE bcData[1];
    bcData[0]=i+1;
    m_GameLogic.GetCardType(bcData[0],1,bcOutCadDataWin);

    for(int j= 0; j<AREA_COUNT; j++)
    {
      if(bcOutCadDataWin[j]>1&&j==InArea-1)
      {
        LONGLONG Score = 0;
        for(int nAreaIndex=0; nAreaIndex<=AREA_COUNT; ++nAreaIndex)
        {
          if(bcOutCadDataWin[nAreaIndex-1]>1)
          {
            Score += m_lAllJettonScore[nAreaIndex]*(bcOutCadDataWin[nAreaIndex-1]);
          }
        }
        if(Score>=lMaxSocre)
        {
          lMaxSocre = Score;
          CopyMemory(bcWinArea,bcOutCadDataWin,bcAreaCount);

        }
        break;
      }

    }
  }
}
//最大下注
LONGLONG CTableFrameSink::GetUserMaxJetton(WORD wChairID,BYTE Area)
{
  IServerUserItem *pIMeServerUserItem=m_pITableFrame->GetTableUserItem(wChairID);
  if(NULL==pIMeServerUserItem)
  {
    return 0L;
  }


  //已下注额
  LONGLONG lNowJetton = 0;
  ASSERT(AREA_COUNT<=CountArray(m_lUserJettonScore));
  for(int nAreaIndex=0; nAreaIndex<=AREA_COUNT; ++nAreaIndex)
  {
    lNowJetton += m_lUserJettonScore[nAreaIndex][wChairID];
  }

  //庄家金币
  LONGLONG lBankerScore=0x7fffffffffffffff;
  if(m_wCurrentBanker!=INVALID_CHAIR)
  {
    IServerUserItem *pIUserItemBanker=m_pITableFrame->GetTableUserItem(m_wCurrentBanker);
    if(NULL!=pIUserItemBanker)
    {
      lBankerScore=pIUserItemBanker->GetUserScore();
    }
  }

  BYTE bcWinArea[AREA_COUNT];
  LONGLONG LosScore = 0;
  LONGLONG WinScore = 0;

  GetAllWinArea(bcWinArea,AREA_COUNT,Area);

  for(int nAreaIndex=0; nAreaIndex<=AREA_COUNT; ++nAreaIndex)
  {
    if(bcWinArea[nAreaIndex-1]>1)
    {
      LosScore+=m_lAllJettonScore[nAreaIndex]*(bcWinArea[nAreaIndex-1]);
    }
    else
    {
      if(bcWinArea[nAreaIndex-1]==0)
      {
        WinScore+=m_lAllJettonScore[nAreaIndex];

      }
    }
  }
  lBankerScore = lBankerScore + WinScore - LosScore;

  if(lBankerScore < 0)
  {
    if(m_wCurrentBanker!=INVALID_CHAIR)
    {
      IServerUserItem *pIUserItemBanker=m_pITableFrame->GetTableUserItem(m_wCurrentBanker);
      if(NULL!=pIUserItemBanker)
      {
        lBankerScore=pIUserItemBanker->GetUserScore();
      }
    }
    else
    {
      lBankerScore = 0x7fffffffffffffff;
    }
  }

  //个人限制
  LONGLONG lMeMaxScore = min((pIMeServerUserItem->GetUserScore()-lNowJetton), m_lUserLimitScore);

  //区域限制
  lMeMaxScore=min(lMeMaxScore,m_lAreaLimitScore);

  BYTE diMultiple[AREA_COUNT];

  for(int i = 0; i<ANIMAL_COUNT; i++)
  {
    BYTE bcData[1];
    bcData[0]= i+1;
    BYTE  bcOutCadDataWin[AREA_COUNT];
    m_GameLogic.GetCardType(bcData[0],1,bcOutCadDataWin);

    for(int j = 0; j<AREA_COUNT; j++)
    {
      if(bcOutCadDataWin[j]>1)
      {
        diMultiple[j] = bcOutCadDataWin[j];

      }
    }
  }
  //庄家限制
  lMeMaxScore=min(lMeMaxScore,lBankerScore/(diMultiple[Area-1]));

  //非零限制
  ASSERT(lMeMaxScore >= 0);
  lMeMaxScore = max(lMeMaxScore, 0);

  return (LONGLONG)(lMeMaxScore);
}
//计算得分
LONGLONG CTableFrameSink::CalculateScore()
{
  BYTE  bcResulteOut[AREA_COUNT];
  memset(bcResulteOut,0,AREA_COUNT);
  m_GameLogic.GetCardType(m_cbTableCardArray[0],1,bcResulteOut);
  BYTE  bcResultTwo[AREA_COUNT];
  memset(bcResultTwo,0,AREA_COUNT);
  m_GameLogic.GetCardType(m_cbTableCardArray[1],1,bcResultTwo);

  //金鲨结算
  if(m_cbTableCardArray[0] == 4 && bcResulteOut[11] == 100)
  {
    //游戏记录
    tagServerGameRecord &GameRecord = m_GameRecordArrary[m_nRecordLast];

    BYTE  cbMultiple[AREA_COUNT]= {1};

    for(WORD wAreaIndex = 1; wAreaIndex <= AREA_COUNT; ++wAreaIndex)
    {
      if(bcResulteOut[wAreaIndex-1] == 100)
      {
        GameRecord.bWinMen[wAreaIndex-1] = 4;
      }
      else
      {
        GameRecord.bWinMen[wAreaIndex-1] = 0;
      }
    }

    //移动下标
    m_nRecordLast = (m_nRecordLast+1) % MAX_SCORE_HISTORY;
    if(m_nRecordLast == m_nRecordFirst)
    {
      m_nRecordFirst = (m_nRecordFirst+1) % MAX_SCORE_HISTORY;
    }

    //庄家总量
    LONGLONG lBankerWinScore = 0;

    //玩家成绩
    ZeroMemory(m_lUserWinScore, sizeof(m_lUserWinScore));
    ZeroMemory(m_lUserReturnScore, sizeof(m_lUserReturnScore));
    ZeroMemory(m_lUserRevenue, sizeof(m_lUserRevenue));
    LONGLONG lUserLostScore[GAME_PLAYER];
    ZeroMemory(lUserLostScore, sizeof(lUserLostScore));

    LONGLONG lUserWinScore = 0;

    //计算积分
    for(WORD wChairID=0; wChairID<GAME_PLAYER; wChairID++)
    {
      //庄家判断
      if(m_wCurrentBanker==wChairID)
      {
        continue;
      }

      //获取用户
      IServerUserItem * pIServerUserItem=m_pITableFrame->GetTableUserItem(wChairID);
      if(pIServerUserItem==NULL)
      {
        continue;
      }

      //燕子 2 //鸽子 3
      //int Milt[13] = {0,6, 6, 8, 8, 12, 12,8,8,24,2,2,0};

      LONGLONG lMeJetton=0L;
      LONGLONG lMeJettonReturn = 0L;

      //统计玩家赢了
      lMeJetton += m_lUserJettonScore[9][wChairID];
      /*for (int nAreaIndex=0; nAreaIndex<=AREA_COUNT; ++nAreaIndex)
      {
        lMeJetton += m_lUserJettonScore[nAreaIndex][wChairID];
      }*/

      //统计玩家下注
      lMeJettonReturn += m_lUserJettonScore[9][wChairID];
      /*for (int nAreaIndex=0; nAreaIndex<=AREA_COUNT; ++nAreaIndex)
        lMeJettonReturn += m_lUserJettonScore[nAreaIndex][wChairID];*/


      //if(lMeJettonReturn >0) m_lStorageCurrent-=(lMeJetton*100);

      if(lMeJetton > 0L)
      {
        LONGLONG Score = 0;
        Score += lMeJetton*100;
        m_lUserWinScore[wChairID] += Score;
        m_lUserReturnScore[wChairID] += lMeJettonReturn;
        lBankerWinScore -= Score;
      }
      else if(lMeJetton == 0L)
      {
        for(int nAreaIndex=0; nAreaIndex<=AREA_COUNT; ++nAreaIndex)
        {
          lMeJetton += m_lUserJettonScore[nAreaIndex][wChairID];
        }
        lUserLostScore[wChairID] -= lMeJetton;
      }

      //总的分数
      m_lUserWinScore[wChairID] += lUserLostScore[wChairID];
    }


    if(m_wCurrentBanker!=INVALID_CHAIR)
    {
      //庄家成绩
      m_lUserWinScore[m_wCurrentBanker] = lBankerWinScore;
    }

    //计算税收
    for(WORD wChairID = 0; wChairID < GAME_PLAYER; wChairID++)
    {
      IServerUserItem * pIServerUserItem=m_pITableFrame->GetTableUserItem(wChairID);
      if(pIServerUserItem==NULL)
      {
        continue;
      }
      if(wChairID != m_wCurrentBanker && m_lUserWinScore[wChairID] > 0)
      {
        SCORE Revenue = m_pITableFrame->CalculateRevenue(wChairID,m_lUserWinScore[wChairID]);
        m_lUserRevenue[wChairID] = Revenue;
        m_lUserWinScore[wChairID] -= m_lUserRevenue[wChairID];

      }
      else if(m_wCurrentBanker!=INVALID_CHAIR && wChairID == m_wCurrentBanker && lBankerWinScore > 0)
      {
        SCORE Revenue = m_pITableFrame->CalculateRevenue(wChairID,m_lUserWinScore[m_wCurrentBanker]);
        m_lUserRevenue[m_wCurrentBanker] = Revenue;
        m_lUserWinScore[m_wCurrentBanker] -= m_lUserRevenue[m_wCurrentBanker];
      }
    }



    //累计积分
    m_lBankerWinScore += lBankerWinScore;

    //当前积分
    m_lBankerCurGameScore=lBankerWinScore;

    return lBankerWinScore;
  }

  //通杀结算
  if(m_cbTableCardArray[0] == 11 && bcResulteOut[10] == 255)
  {
    //游戏记录
    tagServerGameRecord &GameRecord = m_GameRecordArrary[m_nRecordLast];
    BYTE  cbMultiple[AREA_COUNT]= {1};

    for(WORD wAreaIndex = 1; wAreaIndex <= AREA_COUNT; ++wAreaIndex)
    {
      if(bcResulteOut[wAreaIndex-1] == 255)
      {
        GameRecord.bWinMen[wAreaIndex-1] = 4;
      }
      else
      {
        GameRecord.bWinMen[wAreaIndex-1] = 0;
      }
    }

    //移动下标
    m_nRecordLast = (m_nRecordLast+1) % MAX_SCORE_HISTORY;
    if(m_nRecordLast == m_nRecordFirst)
    {
      m_nRecordFirst = (m_nRecordFirst+1) % MAX_SCORE_HISTORY;
    }

    //庄家总量
    LONGLONG lBankerWinScore = 0;

    //玩家成绩
    ZeroMemory(m_lUserWinScore, sizeof(m_lUserWinScore));
    ZeroMemory(m_lUserReturnScore, sizeof(m_lUserReturnScore));
    ZeroMemory(m_lUserRevenue, sizeof(m_lUserRevenue));
    LONGLONG lUserLostScore[GAME_PLAYER];
    ZeroMemory(lUserLostScore, sizeof(lUserLostScore));

    //玩家下注
    LONGLONG *pUserScore[AREA_COUNT+1];
    pUserScore[0]=NULL;
    for(int i = 1; i<AREA_COUNT+1; i++)
    {
      pUserScore[i]=m_lUserJettonScore[i];
    }


    for(WORD wChairID=0; wChairID<GAME_PLAYER; wChairID++)
    {
      //计算积分
      //庄家判断
      if(m_wCurrentBanker==wChairID)
      {
        continue;
      }

      //获取用户
      IServerUserItem * pIServerUserItem=m_pITableFrame->GetTableUserItem(wChairID);
      if(pIServerUserItem==NULL)
      {
        continue;
      }

      LONGLONG lMeJetton=0L;
      LONGLONG lMeJettonReturn = 0L;

      //统计玩家输了
      for(int nAreaIndex=0; nAreaIndex<=AREA_COUNT; ++nAreaIndex)
      {
        lMeJetton += (m_lUserJettonScore[nAreaIndex][wChairID]);
      }

      lMeJettonReturn=lMeJetton;
      //if(lMeJettonReturn >0)m_lStorageCurrent+=(lMeJettonReturn);

      if(lMeJetton>= 0L)
      {
        //通杀结果
        lUserLostScore[wChairID] -= lMeJetton;
        lBankerWinScore += lMeJettonReturn;
      }

      //总的分数
      m_lUserWinScore[wChairID] += lUserLostScore[wChairID];
    }

    //庄家成绩
    if(m_wCurrentBanker!=INVALID_CHAIR)
    {
      m_lUserWinScore[m_wCurrentBanker] = lBankerWinScore;
    }

    //计算税收
    for(WORD wChairID = 0; wChairID < GAME_PLAYER; wChairID++)
    {
      IServerUserItem * pIServerUserItem=m_pITableFrame->GetTableUserItem(wChairID);
      if(pIServerUserItem==NULL)
      {
        continue;
      }
      if(wChairID != m_wCurrentBanker && m_lUserWinScore[wChairID] > 0)
      {
        SCORE Revenue = m_pITableFrame->CalculateRevenue(wChairID,m_lUserWinScore[wChairID]);
        m_lUserRevenue[wChairID] = Revenue;
        m_lUserWinScore[wChairID] -= m_lUserRevenue[wChairID];
      }
      else if(m_wCurrentBanker!=INVALID_CHAIR && wChairID == m_wCurrentBanker && lBankerWinScore > 0)
      {
        SCORE Revenue = m_pITableFrame->CalculateRevenue(wChairID,m_lUserWinScore[m_wCurrentBanker]);
        m_lUserRevenue[m_wCurrentBanker] = Revenue;
        m_lUserWinScore[m_wCurrentBanker] -= m_lUserRevenue[m_wCurrentBanker];
        lBankerWinScore = m_lUserWinScore[m_wCurrentBanker];
      }
    }

    //累计积分
    m_lBankerWinScore += lBankerWinScore;

    //当前积分
    m_lBankerCurGameScore=lBankerWinScore;

    return lBankerWinScore;
  }

  //通赔结算
  if(m_cbTableCardArray[0] == 25 && bcResulteOut[9] == 1)
  {

    //游戏记录
    tagServerGameRecord &GameRecord = m_GameRecordArrary[m_nRecordLast];

    BYTE  cbMultiple[AREA_COUNT]= {1};

    for(WORD wAreaIndex = 1; wAreaIndex <= AREA_COUNT; ++wAreaIndex)
    {
      if(bcResulteOut[wAreaIndex-1] == 1)
      {
        GameRecord.bWinMen[wAreaIndex-1] = 4;
      }
      else
      {
        GameRecord.bWinMen[wAreaIndex-1] = 0;
      }
    }

    //移动下标
    m_nRecordLast = (m_nRecordLast+1) % MAX_SCORE_HISTORY;
    if(m_nRecordLast == m_nRecordFirst)
    {
      m_nRecordFirst = (m_nRecordFirst+1) % MAX_SCORE_HISTORY;
    }

    //庄家总量
    LONGLONG lBankerWinScore = 0;

    //玩家成绩
    ZeroMemory(m_lUserWinScore, sizeof(m_lUserWinScore));
    ZeroMemory(m_lUserReturnScore, sizeof(m_lUserReturnScore));
    ZeroMemory(m_lUserRevenue, sizeof(m_lUserRevenue));
    LONGLONG lUserLostScore[GAME_PLAYER];
    ZeroMemory(lUserLostScore, sizeof(lUserLostScore));



    LONGLONG lUserWinScore = 0;


    for(WORD wChairID=0; wChairID<GAME_PLAYER; wChairID++)
    {
      //计算积分
      //庄家判断
      if(m_wCurrentBanker==wChairID)
      {
        continue;
      }

      //获取用户
      IServerUserItem * pIServerUserItem=m_pITableFrame->GetTableUserItem(wChairID);
      if(pIServerUserItem==NULL)
      {
        continue;
      }

      //燕子 2 //鸽子 3
      int Milt[13] = {0,6,  6, 8, 8, 12, 12,8,8,24,2,2,0};

      LONGLONG lMeJetton=0L;
      LONGLONG lMeJettonReturn = 0L;

      //统计玩家赢了
      for(int nAreaIndex=0; nAreaIndex<=AREA_COUNT; ++nAreaIndex)
      {
        lMeJetton += (m_lUserJettonScore[nAreaIndex][wChairID] * Milt[nAreaIndex]);
      }

      //统计玩家下注
      for(int nAreaIndex=1; nAreaIndex<=AREA_COUNT; ++nAreaIndex)
      {
        lMeJettonReturn += m_lUserJettonScore[nAreaIndex][wChairID];
      }

      //if(lMeJettonReturn >0)m_lStorageCurrent-=lMeJettonReturn;
      if(lMeJetton>= 0L)
      {
        //通赔结果
        m_lUserWinScore[wChairID] += lMeJetton;
        m_lUserReturnScore[wChairID] += lMeJettonReturn;
        lBankerWinScore -= lMeJetton;
        //m_lStorageCurrent-=lMeJetton;
      }

      //总的分数
      m_lUserWinScore[wChairID] += lUserLostScore[wChairID];
    }

    //庄家成绩
    if(m_wCurrentBanker!=INVALID_CHAIR)
    {
      m_lUserWinScore[m_wCurrentBanker] = lBankerWinScore;
    }

    //计算税收
    for(WORD wChairID = 0; wChairID < GAME_PLAYER; wChairID++)
    {
      IServerUserItem * pIServerUserItem=m_pITableFrame->GetTableUserItem(wChairID);
      if(pIServerUserItem==NULL)
      {
        continue;
      }
      if(wChairID != m_wCurrentBanker && m_lUserWinScore[wChairID] > 0)
      {
        SCORE Revenue = m_pITableFrame->CalculateRevenue(wChairID,m_lUserWinScore[wChairID]);
        m_lUserRevenue[wChairID] = Revenue;
        m_lUserWinScore[wChairID] -= m_lUserRevenue[wChairID];
      }
      else if(m_wCurrentBanker!=INVALID_CHAIR && wChairID == m_wCurrentBanker && lBankerWinScore > 0)
      {
        SCORE Revenue = m_pITableFrame->CalculateRevenue(wChairID,m_lUserWinScore[m_wCurrentBanker]);
        m_lUserRevenue[m_wCurrentBanker] = Revenue;
        m_lUserWinScore[m_wCurrentBanker] -= m_lUserRevenue[m_wCurrentBanker];
        lBankerWinScore = m_lUserWinScore[m_wCurrentBanker];
      }
    }

    //累计积分
    m_lBankerWinScore += lBankerWinScore;

    //当前积分
    m_lBankerCurGameScore=lBankerWinScore;

    return lBankerWinScore;

  }


  /*-----------------------------正常结算-----------------------------------*/
  for(int i = 0; i<AREA_COUNT; i++)
  {
    bcResulteOut[i] += bcResultTwo[i];
  }

  //鲨鱼 额外奖励
  if(m_cbTableCardArray[0]%7==4)
  {
    bcResulteOut[ID_SHA_YU-1]+=m_cbShaYuAddMulti;
  }

  //游戏记录
  tagServerGameRecord &GameRecord = m_GameRecordArrary[m_nRecordLast];

  BYTE  cbMultiple[AREA_COUNT]= {1};

  for(WORD wAreaIndex = 1; wAreaIndex < AREA_COUNT +1; ++wAreaIndex)
  {
    if(bcResulteOut[wAreaIndex-1] == 2)
    {
      continue;
    }
    if(bcResulteOut[wAreaIndex-1] == 255)
    {
      continue;
    }
    if(bcResulteOut[wAreaIndex-1] == 1)
    {
      continue;
    }
    if(bcResulteOut[wAreaIndex-1] == 100)
    {
      continue;
    }
    if(bcResulteOut[wAreaIndex-1]>2)
    {
      GameRecord.bWinMen[wAreaIndex-1] = 4;
    }
    else
    {
      GameRecord.bWinMen[wAreaIndex-1] = 0;
    }
  }

  //移动下标
  m_nRecordLast = (m_nRecordLast+1) % MAX_SCORE_HISTORY;
  if(m_nRecordLast == m_nRecordFirst)
  {
    m_nRecordFirst = (m_nRecordFirst+1) % MAX_SCORE_HISTORY;
  }

  //庄家总量
  LONGLONG lBankerWinScore = 0;

  //玩家成绩
  ZeroMemory(m_lUserWinScore, sizeof(m_lUserWinScore));
  ZeroMemory(m_lUserReturnScore, sizeof(m_lUserReturnScore));
  ZeroMemory(m_lUserRevenue, sizeof(m_lUserRevenue));
  LONGLONG lUserLostScore[GAME_PLAYER];
  ZeroMemory(lUserLostScore, sizeof(lUserLostScore));

  //玩家下注
  LONGLONG *pUserScore[AREA_COUNT+1];
  pUserScore[0]=NULL;
  for(int i = 1; i<AREA_COUNT+1; i++)
  {
    pUserScore[i]=m_lUserJettonScore[i];
  }

  //计算积分
  for(WORD wChairID=0; wChairID<GAME_PLAYER; wChairID++)
  {
    //庄家判断
    if(m_wCurrentBanker==wChairID)
    {
      continue;
    }

    //获取用户
    IServerUserItem * pIServerUserItem=m_pITableFrame->GetTableUserItem(wChairID);
    if(pIServerUserItem==NULL)
    {
      continue;
    }


    for(WORD wAreaIndex = 1; wAreaIndex <= AREA_COUNT; ++wAreaIndex)
    {
      if(bcResulteOut[wAreaIndex-1]>1)
      {
        m_lUserWinScore[wChairID] += (pUserScore[wAreaIndex][wChairID] *(bcResulteOut[wAreaIndex-1]-1)) ;
        m_lUserReturnScore[wChairID] += pUserScore[wAreaIndex][wChairID] ;
        lBankerWinScore -= (pUserScore[wAreaIndex][wChairID] * (bcResulteOut[wAreaIndex-1]-1)) ;
        //if(m_lUserWinScore[wChairID] >0)m_lStorageCurrent-=( pUserScore[wAreaIndex][wChairID] * (bcResulteOut[wAreaIndex-1]-1) ) ;

      }
      else
      {
        if(bcResulteOut[wAreaIndex-1]==0)
        {
          lUserLostScore[wChairID] -= pUserScore[wAreaIndex][wChairID];
          lBankerWinScore += pUserScore[wAreaIndex][wChairID];
          //m_lStorageCurrent+=( pUserScore[wAreaIndex][wChairID]) ;
        }
        else
        {
          //如果为1则不少分
          m_lUserWinScore[wChairID] += 0;
          m_lUserReturnScore[wChairID] += pUserScore[wAreaIndex][wChairID] ;
        }
      }
    }

    //总的分数
    m_lUserWinScore[wChairID] += lUserLostScore[wChairID];
  }

  //庄家成绩
  if(m_wCurrentBanker!=INVALID_CHAIR)
  {
    m_lUserWinScore[m_wCurrentBanker] = lBankerWinScore;
  }

  //计算税收
  for(WORD wChairID = 0; wChairID < GAME_PLAYER; wChairID++)
  {
    IServerUserItem * pIServerUserItem=m_pITableFrame->GetTableUserItem(wChairID);
    if(pIServerUserItem==NULL)
    {
      continue;
    }
    if(wChairID != m_wCurrentBanker && m_lUserWinScore[wChairID] > 0)
    {
      SCORE Revenue = m_pITableFrame->CalculateRevenue(wChairID,m_lUserWinScore[wChairID]);
      m_lUserRevenue[wChairID] = Revenue;
      m_lUserWinScore[wChairID] -= m_lUserRevenue[wChairID];
    }
    else if(m_wCurrentBanker!=INVALID_CHAIR && wChairID == m_wCurrentBanker && lBankerWinScore > 0)
    {
      SCORE Revenue = m_pITableFrame->CalculateRevenue(wChairID,m_lUserWinScore[m_wCurrentBanker]);
      m_lUserRevenue[m_wCurrentBanker] = Revenue;
      m_lUserWinScore[m_wCurrentBanker] -= m_lUserRevenue[m_wCurrentBanker];
      lBankerWinScore = m_lUserWinScore[m_wCurrentBanker];
    }
  }

  //累计积分
  m_lBankerWinScore += lBankerWinScore;

  //当前积分
  m_lBankerCurGameScore=lBankerWinScore;

  return lBankerWinScore;
}

//试探性判断
bool CTableFrameSink::ProbeJudge()
{
  //系统输赢
  LONGLONG lSystemScore = 0l;   //系统实际输赢
  LONGLONG lAllScore = 0l;    //系统全部输赢(机器人和玩家一样算)

  //计算得分
  JudgeSystemScore(lSystemScore,lAllScore);

  //系统赢钱
  if(lSystemScore>0)
  {
    m_lStorageCurrent += lSystemScore;

    //彩金池
    m_lBonus += lAllScore;
    if(NeedDeductStorage())
    {
      m_lStorageCurrent = m_lStorageCurrent - (m_lStorageCurrent * m_StorageDeduct / 1000);
    }

    //彩金池比真实库存小
    if(m_lBonus <= m_lStorageCurrent)
    {
      m_lBonus = m_lStorageCurrent + rand() % 1000000;
    }

    return true;
  }
  else if(lSystemScore<0) //系统输钱
  {
    //库存不够赔重开
    if(m_lStorageCurrent + lSystemScore <= 0)
    {
      return false;
    }

    //检验最大赔付比例
    LONGLONG lStorageCurrent = m_lStorageCurrent + lSystemScore;
    double llCompensateRadio = (double)((double)m_lCompensateRadio / (double)100);
    if(m_lStorageCurrent * (1 - llCompensateRadio) >= lStorageCurrent)
    {
      return false;
    }

    //判断条件
    if(((m_lStorageCurrent > m_lStorageMax1) && (rand()%100 < m_lStorageMul1))
       || ((m_lStorageCurrent > m_lStorageMax2) && (rand()%100 < m_lStorageMul2))
       || rand() % 100 < 40)
    {
      //满足条件
      m_lStorageCurrent += lSystemScore;

      //彩金池
      m_lBonus += lAllScore;

      if(NeedDeductStorage() && m_lStorageCurrent > 0)
      {
        m_lStorageCurrent = m_lStorageCurrent - (m_lStorageCurrent * m_StorageDeduct / 1000);
      }

      //彩金池比真实库存小
      if(m_lBonus <= m_lStorageCurrent)
      {
        m_lBonus = m_lStorageCurrent + rand() % 1000000;
      }

      return true;
    }
    else
    {
      return false;
    }
  }
  else
  {
    //彩金池
    m_lBonus += lAllScore;

    //彩金池比真实库存小
    if(m_lBonus <= m_lStorageCurrent)
    {
      m_lBonus = m_lStorageCurrent + rand() % 1000000;
    }

    return true;
  }
}

//计算比例
void CTableFrameSink::CalculateAnimalPercent()
{

  for(int i=0; i<2; i++)
  {
    if(m_cbTableCardArray[i]!=0)
    {
      m_nAnimalTimes[GetAnimalIndex(m_cbTableCardArray[i])]++;
    }
  }

  int nAllTimes=0;
  for(int i=0; i<AREA_ALL; i++)
  {
    nAllTimes+=m_nAnimalTimes[i];
  }

  if(nAllTimes!=0)
  {
    for(int i=0; i<AREA_ALL; i++)
    {
      m_nAnimalPercent[i]=m_nAnimalTimes[i]*100/nAllTimes;
    }

  }
}

//获取索引
BYTE CTableFrameSink::GetAnimalIndex(BYTE cbAllIndex)
{

  static BYTE cbAnimalIndex[ANIMAL_COUNT]= {0,0,0,8,1,1,1,2,2,2,8,3,3,3,4,4,4,8,5,5,5,6,6,6,8,7,7,7};

  if(cbAllIndex>=1&&cbAllIndex<=28)
  {
    return cbAnimalIndex[cbAllIndex-1];
  }


  ////兔子
  //if(1==cbAllIndex||cbAllIndex==2||cbAllIndex==3)
  //{
  //  return 0;
  //}
  ////燕子
  //else if(5==cbAllIndex||cbAllIndex==6||cbAllIndex==7)
  //{
  //  return 1;

  //}
  ////鸽子
  //else if(8==cbAllIndex||cbAllIndex==9||cbAllIndex==10)
  //{
  //  return 2;
  //}
  ////孔雀
  //else if(12==cbAllIndex||cbAllIndex==13||cbAllIndex==14)
  //{
  //  return 3;
  //}
  ////老鹰
  //else if(15==cbAllIndex||cbAllIndex==16||cbAllIndex==17)
  //{
  //  return 4;
  //}
  ////狮子
  //else if(19==cbAllIndex||cbAllIndex==20||cbAllIndex==21)
  //{
  //  return 5;
  //}
  ////熊猫
  //else if(22==cbAllIndex||cbAllIndex==23||cbAllIndex==24)
  //{
  //  return 6;
  //}
  ////猴子
  //else if(26==cbAllIndex||cbAllIndex==27||cbAllIndex==28)
  //{
  //  return 7;
  //}
  ////鲨鱼
  //else
  //{
  //  return 8;
  //}
  return 0;
}


//发送记录
void CTableFrameSink::SendGameRecord(IServerUserItem *pIServerUserItem)
{
  WORD wBufferSize=0;
  BYTE cbBuffer[SOCKET_TCP_BUFFER];
  int nIndex = m_nRecordFirst;
  while(nIndex != m_nRecordLast)
  {
    if((wBufferSize+sizeof(tagServerGameRecord))>sizeof(cbBuffer))
    {
      m_pITableFrame->SendUserItemData(pIServerUserItem,SUB_S_SEND_RECORD,cbBuffer,wBufferSize);
      wBufferSize=0;
    }
    CopyMemory(cbBuffer+wBufferSize,&m_GameRecordArrary[nIndex],sizeof(tagServerGameRecord));
    wBufferSize+=sizeof(tagServerGameRecord);

    nIndex = (nIndex+1) % MAX_SCORE_HISTORY;
  }
  if(wBufferSize>0)
  {
    m_pITableFrame->SendUserItemData(pIServerUserItem,SUB_S_SEND_RECORD,cbBuffer,wBufferSize);
  }
}

//发送消息
void CTableFrameSink::SendGameMessage(WORD wChairID, LPCTSTR pszTipMsg)
{
  if(wChairID==INVALID_CHAIR)
  {
    //游戏玩家
    for(WORD i=0; i<GAME_PLAYER; ++i)
    {
      IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(i);
      if(pIServerUserItem==NULL)
      {
        continue;
      }
      m_pITableFrame->SendGameMessage(pIServerUserItem,pszTipMsg,SMT_CHAT);
    }

    //旁观玩家
    WORD wIndex=0;
    do
    {
      IServerUserItem *pILookonServerUserItem=m_pITableFrame->EnumLookonUserItem(wIndex++);
      if(pILookonServerUserItem==NULL)
      {
        break;
      }

      m_pITableFrame->SendGameMessage(pILookonServerUserItem,pszTipMsg,SMT_CHAT);

    }
    while(true);
  }
  else
  {
    IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(wChairID);
    if(pIServerUserItem!=NULL)
    {
      m_pITableFrame->SendGameMessage(pIServerUserItem,pszTipMsg,SMT_CHAT|SMT_EJECT);
    }
  }
}

//////////////////////////////////////////////////////////////////////////
//银行操作
#ifdef __SPECIAL___
bool  CTableFrameSink::OnActionUserBank(WORD wChairID, IServerUserItem * pIServerUserItem)
{
  return true;
}
#endif

//控制
bool CTableFrameSink::OnAdminControl(CMD_C_ControlApplication* pData , IServerUserItem * pIServerUserItem)
{
  switch(pData->cbControlAppType)
  {
    case C_CA_UPDATE: //更新
    {
      CMD_S_ControlReturns ControlReturns;
      ZeroMemory(&ControlReturns,sizeof(ControlReturns));
      ControlReturns.cbReturnsType = S_CR_UPDATE_SUCCES;
      ControlReturns.cbControlArea = m_cbControlArea;
      ControlReturns.cbControlTimes = m_cbControlTimes;
      m_pITableFrame->SendUserItemData(pIServerUserItem,SUB_S_ADMIN_COMMDN,&ControlReturns,sizeof(ControlReturns));
    }
    return true;
    case C_CA_SET:    //设置
    {
      if(pData->cbControlArea == 0xff || pData->cbControlArea  > 7)
      {
        //设置失败
        CMD_S_ControlReturns ControlReturns;
        ZeroMemory(&ControlReturns,sizeof(ControlReturns));
        ControlReturns.cbReturnsType = S_CR_FAILURE;
        ControlReturns.cbControlArea = 0xff;
        m_pITableFrame->SendUserItemData(pIServerUserItem,SUB_S_ADMIN_COMMDN,&ControlReturns,sizeof(ControlReturns));
        return true;
      }
      m_cbControlArea = pData->cbControlArea;
      m_cbControlTimes = pData->cbControlTimes;
      CMD_S_ControlReturns ControlReturns;
      ZeroMemory(&ControlReturns,sizeof(ControlReturns));
      ControlReturns.cbReturnsType = S_CR_SET_SUCCESS;
      ControlReturns.cbControlArea = m_cbControlArea;
      ControlReturns.cbControlTimes = m_cbControlTimes;
      m_pITableFrame->SendUserItemData(pIServerUserItem,SUB_S_ADMIN_COMMDN,&ControlReturns,sizeof(ControlReturns));
    }
    return true;
    case C_CA_CANCELS:  //取消
    {
      m_cbControlArea = 0xff;
      m_cbControlTimes = 0;
      CMD_S_ControlReturns ControlReturns;
      ZeroMemory(&ControlReturns,sizeof(ControlReturns));
      ControlReturns.cbReturnsType = S_CR_CANCEL_SUCCESS;
      ControlReturns.cbControlArea = 0xff;
      m_pITableFrame->SendUserItemData(pIServerUserItem,SUB_S_ADMIN_COMMDN,&ControlReturns,sizeof(ControlReturns));
    }
    return true;
  }
  //设置失败
  CMD_S_ControlReturns ControlReturns;
  ZeroMemory(&ControlReturns,sizeof(ControlReturns));
  ControlReturns.cbReturnsType = S_CR_FAILURE;
  ControlReturns.cbControlArea = 0xff;
  m_pITableFrame->SendUserItemData(pIServerUserItem,SUB_S_ADMIN_COMMDN,&ControlReturns,sizeof(ControlReturns));
  return true;
}
//////////////////////////////////////////////////////////////////////////
//查找庄家
bool CTableFrameSink::FindSuitBanker()
{

  bool bReturn=false;
  while(m_ApplyUserArray.GetCount()>0)
  {
    IServerUserItem *pIServerUserItem = m_pITableFrame->GetTableUserItem(m_ApplyUserArray[0]);
    if(pIServerUserItem == NULL)
    {
      m_ApplyUserArray.RemoveAt(0);
    }
    else
    {
      //坐庄判断
      LONGLONG lUserScore=pIServerUserItem->GetUserScore();
      if(lUserScore<m_lApplyBankerCondition)
      {
        //发送消息
        CMD_S_CancelBanker CancelBanker;
        ZeroMemory(&CancelBanker,sizeof(CancelBanker));
        tagUserInfo * pUserInfo = pIServerUserItem->GetUserInfo();
        //设置变量
        lstrcpyn(CancelBanker.szCancelUser,pUserInfo->szNickName,CountArray(CancelBanker.szCancelUser));

        //发送消息
        m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));
        m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));

        //提示消息
        TCHAR szTipMsg[128];
        myprintf(szTipMsg,CountArray(szTipMsg),TEXT("由于你的金币数少于坐庄必须金币数（%I64d）,你自动下庄！"),m_lApplyBankerCondition);
        SendGameMessage(m_ApplyUserArray[0],szTipMsg);

        //删除玩家
        m_ApplyUserArray.RemoveAt(0);
      }
      else
      {
        bReturn=true;
        break;
      }
    }
  }

  return bReturn;
}

//是否衰减
bool CTableFrameSink::NeedDeductStorage()
{

  for(int i = 0; i < GAME_PLAYER; ++i)
  {
    IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(i);
    if(pIServerUserItem == NULL)
    {
      continue;
    }

    if(!pIServerUserItem->IsAndroidUser())
    {
      for(int nAreaIndex=0; nAreaIndex<=AREA_COUNT; ++nAreaIndex)
      {
        if(m_lUserJettonScore[nAreaIndex][i]!=0)
        {
          return true;
        }
      }
    }
  }

  return false;

}

//读取配置
void CTableFrameSink::ReadConfigInformation()
{
  //获取自定义配置
  tagCustomConfig *pCustomConfig = (tagCustomConfig *)m_pGameServiceOption->cbCustomRule;
  ASSERT(pCustomConfig);

  //上庄
  m_lApplyBankerCondition = pCustomConfig->CustomGeneral.lApplyBankerCondition;
  m_lBankerMAX = pCustomConfig->CustomGeneral.lBankerTime;
  m_lBankerAdd = pCustomConfig->CustomGeneral.lBankerTimeAdd;
  m_lBankerScoreMAX = pCustomConfig->CustomGeneral.lBankerScoreMAX;
  m_lBankerScoreAdd = pCustomConfig->CustomGeneral.lBankerTimeExtra;
  m_bEnableSysBanker = (pCustomConfig->CustomGeneral.nEnableSysBanker == TRUE)?true:false;

  //时间
  m_cbFreeTime = pCustomConfig->CustomGeneral.cbFreeTime;
  m_cbBetTime = pCustomConfig->CustomGeneral.cbBetTime;
  m_cbEndTime = pCustomConfig->CustomGeneral.cbEndTime;
  if(m_cbFreeTime < TIME_FREE || m_cbFreeTime > 99)
  {
    m_cbFreeTime = TIME_FREE;
  }
  if(m_cbBetTime < TIME_PLACE_JETTON || m_cbBetTime > 99)
  {
    m_cbBetTime = TIME_PLACE_JETTON;
  }
  if(m_cbEndTime < TIME_GAME_END || m_cbEndTime > 99)
  {
    m_cbEndTime = TIME_GAME_END;
  }

  //下注
  m_lAreaLimitScore = pCustomConfig->CustomGeneral.lAreaLimitScore;
  m_lUserLimitScore = pCustomConfig->CustomGeneral.lUserLimitScore;


  //库存
  m_lStorageStart = pCustomConfig->CustomGeneral.StorageStart;
  m_lBonus = pCustomConfig->CustomGeneral.lBonus;
  m_lStorageCurrent = m_lStorageStart;
  m_lCompensateRadio = pCustomConfig->CustomGeneral.lCompensateRadio;
  m_StorageDeduct = pCustomConfig->CustomGeneral.StorageDeduct;
  m_lStorageMax1 = pCustomConfig->CustomGeneral.StorageMax1;
  m_lStorageMul1 = pCustomConfig->CustomGeneral.StorageMul1;
  m_lStorageMax2 = pCustomConfig->CustomGeneral.StorageMax2;
  m_lStorageMul2 = pCustomConfig->CustomGeneral.StorageMul2;
  if(m_lStorageMul1 < 0 || m_lStorageMul1 > 100)
  {
    m_lStorageMul1 = 50;
  }
  if(m_lStorageMul2 < 0 || m_lStorageMul2 > 100)
  {
    m_lStorageMul2 = 80;
  }

  //机器人
  m_nRobotListMaxCount = pCustomConfig->CustomAndroid.lRobotListMaxCount;

  LONGLONG lRobotBetMinCount = pCustomConfig->CustomAndroid.lRobotBetMinCount;
  LONGLONG lRobotBetMaxCount = pCustomConfig->CustomAndroid.lRobotBetMaxCount;
  m_nMaxChipRobot = rand()%(lRobotBetMaxCount-lRobotBetMinCount+1) + lRobotBetMinCount;
  if(m_nMaxChipRobot < 0)
  {
    m_nMaxChipRobot = 8;
  }
  m_lRobotAreaLimit = pCustomConfig->CustomAndroid.lRobotAreaLimit;


  //区域几率
  m_nMultiple[ID_TU_ZI-1] = pCustomConfig->CustomGeneral.Area1Radio;
  m_nMultiple[ID_YAN_ZI-1] = pCustomConfig->CustomGeneral.Area2Radio;
  m_nMultiple[ID_GE_ZI-1] = pCustomConfig->CustomGeneral.Area3Radio;
  m_nMultiple[ID_KONG_QUE-1] = pCustomConfig->CustomGeneral.Area4Radio;
  m_nMultiple[ID_LAO_YING-1] = pCustomConfig->CustomGeneral.Area5Radio;
  m_nMultiple[ID_SHI_ZI-1] = pCustomConfig->CustomGeneral.Area6Radio;
  m_nMultiple[ID_XIONG_MAO-1] = pCustomConfig->CustomGeneral.Area7Radio;
  m_nMultiple[ID_HOU_ZI-1] = pCustomConfig->CustomGeneral.Area8Radio;
  m_nMultiple[ID_SHA_YU-1] = pCustomConfig->CustomGeneral.Area9Radio;
  m_nMultiple[9] = pCustomConfig->CustomGeneral.Area10Radio;
  m_nMultiple[10] = pCustomConfig->CustomGeneral.Area11Radio;
  m_nMultiple[11] = pCustomConfig->CustomGeneral.Area12Radio;

}

//测试写信息
void CTableFrameSink::WriteStorageInfo(SCORE lStorage,bool bControl)
{
  //设置语言区域
  char* old_locale = _strdup(setlocale(LC_CTYPE,NULL));
  setlocale(LC_CTYPE, "chs");
  CString pszString;
  CTime time = CTime::GetCurrentTime();
  if(bControl)
  {
    pszString.Format(TEXT("\n时间: %d-%d-%d %d:%d:%d,管理员修改库存为: %I64d "),time.GetYear(), time.GetMonth(), time.GetDay(),
                     time.GetHour(), time.GetMinute(), time.GetSecond(),lStorage);
  }
  else
  {
    pszString.Format(TEXT("\n时间: %d-%d-%d %d:%d:%d,当前库存为: %I64d "),time.GetYear(), time.GetMonth(), time.GetDay(),
                     time.GetHour(), time.GetMinute(), time.GetSecond(),lStorage);
  }
  CStdioFile myFile;
  CString strFileName;
  strFileName.Format(TEXT("飞禽走兽库存日志.txt"));
  BOOL bOpen = myFile.Open(strFileName, CFile::modeReadWrite|CFile::modeCreate|CFile::modeNoTruncate);
  if(bOpen)
  {
    myFile.SeekToEnd();
    myFile.WriteString(pszString);
    myFile.Flush();
    myFile.Close();
  }

  //还原区域设定
  setlocale(LC_CTYPE, old_locale);
  free(old_locale);
}

void CTableFrameSink::JudgeSystemScore(LONGLONG& lSystemScore, LONGLONG& lAllScore)
{

  BYTE  bcResulteOut[AREA_COUNT];
  memset(bcResulteOut,0,AREA_COUNT);
  m_GameLogic.GetCardType(m_cbTableCardArray[0],1,bcResulteOut);
  BYTE  bcResultTwo[AREA_COUNT];
  memset(bcResultTwo,0,AREA_COUNT);
  m_GameLogic.GetCardType(m_cbTableCardArray[1],1,bcResultTwo);

  //计算倍数
  if(m_cbTableCardArray[0] == 4 && bcResulteOut[11] == 100)   //金鲨
  {
    ::ZeroMemory(bcResulteOut,sizeof(bcResulteOut));
    bcResulteOut[ID_SHA_YU-1]=100;
  }
  else if(m_cbTableCardArray[0] == 11 && bcResulteOut[10] == 255)   //通杀
  {
    ::ZeroMemory(bcResulteOut,sizeof(bcResulteOut));
  }
  else if(m_cbTableCardArray[0] == 25 && bcResulteOut[9] == 1)  //通赔
  {
    ::ZeroMemory(bcResulteOut,sizeof(bcResulteOut));
    for(BYTE i=0; i<AREA_COUNT; i++)
    {
      if(i==0 || i==1)
      {
        bcResulteOut[i]=6;
      }
      if(i==2 || i==3)
      {
        bcResulteOut[i]=8;
      }
      if(i==4 || i==5)
      {
        bcResulteOut[i]=12;
      }
      if(i==6 || i==7)
      {
        bcResulteOut[i]=8;
      }
      if(i==8)
      {
        bcResulteOut[i]=24;
      }
      if(i==9 || i==10)
      {
        bcResulteOut[i]=2;
      }
    }
  }
  else  //其他
  {
    for(int i = 0; i<AREA_COUNT; i++)
    {
      bcResulteOut[i] += bcResultTwo[i];
    }

    //鲨鱼 额外奖励
    if(m_cbTableCardArray[0]%7==4)
    {
      bcResulteOut[ID_SHA_YU-1]+=m_cbShaYuAddMulti;
    }
  }
  //玩家下注
  LONGLONG *pUserScore[AREA_COUNT+1];
  pUserScore[0] = NULL;
  for(int i = 1; i<AREA_COUNT+1; i++)
  {
    pUserScore[i] = m_lUserJettonScore[i];
  }

  //计算积分
  for(WORD wChairID=0; wChairID<GAME_PLAYER; wChairID++)
  {
    //获取用户
    IServerUserItem * pIServerUserItem=m_pITableFrame->GetTableUserItem(wChairID);
    if(pIServerUserItem==NULL)
    {
      continue;
    }

    bool bIsAndroidUser = pIServerUserItem->IsAndroidUser();

    for(WORD wAreaIndex = 1; wAreaIndex <= AREA_COUNT; ++wAreaIndex)
    {

      if(bcResulteOut[wAreaIndex-1]>1)
      {
        //真人
        if(!bIsAndroidUser)
        {
          lSystemScore -= (pUserScore[wAreaIndex][wChairID] *(bcResulteOut[wAreaIndex-1]-1));
        }
        lAllScore -= (pUserScore[wAreaIndex][wChairID] *(bcResulteOut[wAreaIndex-1]-1));
      }
      else
      {
        if(bcResulteOut[wAreaIndex-1]==0)
        {
          //真人
          if(!bIsAndroidUser)
          {
            lSystemScore += pUserScore[wAreaIndex][wChairID];
          }
          lAllScore += pUserScore[wAreaIndex][wChairID];
        }
      }
    }
  }
}
