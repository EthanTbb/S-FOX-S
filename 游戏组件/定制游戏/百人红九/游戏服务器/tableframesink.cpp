#include "StdAfx.h"
#include "DlgCustomRule.h"
#include "TableFrameSink.h"
#include <locale>
//////////////////////////////////////////////////////////////////////////

//常量定义
#define SEND_COUNT          300                 //发送次数

//索引定义
#define INDEX_PLAYER        0                 //闲家索引
#define INDEX_BANKER        1                 //庄家索引

//时间定义
#define IDI_FREE          1                 //空闲时间
#define IDI_PLACE_JETTON      2                 //下注时间
#define IDI_GAME_END        3                 //结束时间

#define TIME_FREE         5                 //空闲时间
#define TIME_PLACE_JETTON     10                  //下注时间
#define TIME_GAME_END       20                  //结束时间

//////////////////////////////////////////////////////////////////////////
//静态变量
const WORD      CTableFrameSink::m_wPlayerCount=GAME_PLAYER;        //游戏人数

void Debug(char *text,...)
{
  static DWORD num=0;
  char buf[1024];
  FILE *fp=NULL;
  va_list ap;
  va_start(ap,text);
  vsprintf(buf,text,ap);
  va_end(ap);
  if(num == 0)
  {
    fp=fopen("服务器端下注.log","w");
  }
  else
  {
    fp=fopen("服务器端下注.log","a");
  }
  if(fp == NULL)
  {
    return ;
  }
  num++;
  SYSTEMTIME time;
  GetLocalTime(&time);
  fprintf(fp, "%d:%s - %d/%d/%d %d:%d:%d \n", num, buf, time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);
  fclose(fp);
}
//////////////////////////////////////////////////////////////////////////

//构造函数
CTableFrameSink::CTableFrameSink()
{
  //起始分数
  ZeroMemory(m_lUserStartScore,sizeof(m_lUserStartScore));

  //总下注数
  ZeroMemory(m_lAllJettonScore,sizeof(m_lAllJettonScore));

  //个人下注
  ZeroMemory(m_lUserJettonScore,sizeof(m_lUserJettonScore));

  //控制变量
  m_cbWinSideControl=0;
  m_nSendCardCount=0;

  //玩家成绩
  ZeroMemory(m_lUserWinScore,sizeof(m_lUserWinScore));
  ZeroMemory(m_lUserReturnScore,sizeof(m_lUserReturnScore));
  ZeroMemory(m_lUserRevenue,sizeof(m_lUserRevenue));

  //扑克信息
  ZeroMemory(m_cbCardCount,sizeof(m_cbCardCount));
  ZeroMemory(m_cbTableCardArray,sizeof(m_cbTableCardArray));

  //状态变量
  m_dwJettonTime=0L;
  m_bExchangeBanker=true;
  m_wAddTime=0;
  m_bControl=false;
  CopyMemory(m_szControlName,TEXT("无人"),sizeof(m_szControlName));

  //庄家信息
  m_ApplyUserArray.RemoveAll();
  m_wCurrentBanker=INVALID_CHAIR;
  m_wBankerTime=0;
  m_lBankerWinScore=0L;
  m_lBankerCurGameScore=0L;
  m_bEnableSysBanker=true;
  m_cbLeftCardCount=0;
  m_bContiueCard=false;

  //记录变量
  m_nRecordFirst=0;
  m_nRecordLast=0;
  m_dwRecordCount=0;
  ZeroMemory(m_GameRecordArrary,sizeof(m_GameRecordArrary));

  //机器人控制
  m_nChipRobotCount = 0;
  m_nRobotListMaxCount =0;
  ZeroMemory(m_lRobotAreaScore, sizeof(m_lRobotAreaScore));

  m_lStorageMax1 = 0;
  m_lStorageMul1 = 0;
  m_lStorageMax2 = 0;
  m_lStorageMul2 = 0;

  //服务控制
  m_hInst = NULL;
  m_pServerContro = NULL;
  m_hInst = LoadLibrary(TEXT("RedNineBattleServerControl.dll"));
  if(m_hInst)
  {
    typedef void * (*CREATE)();
    CREATE ServerControl = (CREATE)GetProcAddress(m_hInst,"CreateServerControl");
    if(ServerControl)
    {
      m_pServerContro = static_cast<IServerControl*>(ServerControl());
    }
  }

  return;
}

//析构函数
CTableFrameSink::~CTableFrameSink(void)
{
}

//释放对象
VOID CTableFrameSink::Release()
{
  if(m_pServerContro)
  {
    delete m_pServerContro;
    m_pServerContro = NULL;
  }

  if(m_hInst)
  {
    FreeLibrary(m_hInst);
    m_hInst = NULL;
  }
}

//接口查询
VOID * CTableFrameSink::QueryInterface(const IID & Guid, DWORD dwQueryVer)
{
  QUERYINTERFACE(ITableFrameSink,Guid,dwQueryVer);
  QUERYINTERFACE(ITableUserAction,Guid,dwQueryVer);
  return NULL;
}

//初始化
bool CTableFrameSink::Initialization(IUnknownEx * pIUnknownEx)
{
  //查询接口
  ASSERT(pIUnknownEx!=NULL);
  m_pITableFrame=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,ITableFrame);
  if(m_pITableFrame==NULL)
  {
    return false;
  }

  //开始模式
  m_pITableFrame->SetStartMode(START_MODE_TIME_CONTROL);

  //设置文件名
  TCHAR szPath[MAX_PATH]=TEXT("");
  GetCurrentDirectory(sizeof(szPath),szPath);
  _sntprintf(m_szConfigFileName,sizeof(m_szConfigFileName),TEXT("%s\\RedNineBattleConfig.ini"),szPath);

  //获取参数
  m_pGameServiceOption=m_pITableFrame->GetGameServiceOption();
  ASSERT(m_pGameServiceOption!=NULL);

  //房间名称
  memcpy(m_szRoomName, m_pGameServiceOption->szServerName, sizeof(m_szRoomName));

  ReadConfigInformation();

  return true;
}

//复位桌子
VOID CTableFrameSink::RepositionSink()
{
  //总下注数
  ZeroMemory(m_lAllJettonScore,sizeof(m_lAllJettonScore));

  //个人下注
  ZeroMemory(m_lUserJettonScore,sizeof(m_lUserJettonScore));

  //玩家成绩
  ZeroMemory(m_lUserWinScore,sizeof(m_lUserWinScore));
  ZeroMemory(m_lUserReturnScore,sizeof(m_lUserReturnScore));
  ZeroMemory(m_lUserRevenue,sizeof(m_lUserRevenue));

  //机器人控制
  m_nChipRobotCount = 0;
  m_bControl=false;
  ZeroMemory(m_lRobotAreaScore, sizeof(m_lRobotAreaScore));

  return;
}


//游戏状态
bool CTableFrameSink::IsUserPlaying(WORD wChairID)
{
  return true;
}

bool CTableFrameSink::OnSubAmdinCommand(IServerUserItem*pIServerUserItem,const void*pDataBuffer)
{
  //如果不具有管理员权限 则返回错误
  if(!CUserRight::IsGameCheatUser(pIServerUserItem->GetUserRight()))
  {
    return false;
  }

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

//积分事件
bool CTableFrameSink::OnUserScroeNotify(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)
{
  //当庄家分数在空闲时间变动时(即庄家进行了存取款)校验庄家的上庄条件
  if(wChairID == m_wCurrentBanker && m_pITableFrame->GetGameStatus()==GAME_SCENE_FREE)
  {
    ChangeBanker(false);
  }

  return true;
}

//游戏开始
bool CTableFrameSink::OnEventGameStart()
{
  //设置随机种子
  srand(GetTickCount());

  //发送库存消息
  for(WORD i=0; i<GAME_PLAYER; i++)
  {
    //获取用户
    IServerUserItem * pIServerUserItem=m_pITableFrame->GetTableUserItem(i);
    if(pIServerUserItem == NULL)
    {
      continue;
    }

    if(CUserRight::IsGameCheatUser(m_pITableFrame->GetTableUserItem(i)->GetUserRight()))
    {
      CString strInfo;
      strInfo.Format(TEXT("当前库存：%I64d"), m_lStorageCurrent);

      m_pITableFrame->SendGameMessage(pIServerUserItem,strInfo,SMT_CHAT);
    }
  }

  CString strStorage;
  CTime Time(CTime::GetCurrentTime());
  strStorage.Format(TEXT(" 房间: %s | 时间: %d-%d-%d %d:%d:%d | 库存: %I64d \n"), m_pGameServiceOption->szServerName, Time.GetYear(), Time.GetMonth(), Time.GetDay(), Time.GetHour(), Time.GetMinute(), Time.GetSecond(), m_lStorageCurrent);
  WriteInfo(TEXT("百人红九库存日志.log"), strStorage);

  //变量定义
  CMD_S_GameStart GameStart;
  ZeroMemory(&GameStart,sizeof(GameStart));

  //获取庄家
  IServerUserItem *pIBankerServerUserItem=NULL;
  if(INVALID_CHAIR!=m_wCurrentBanker)
  {
    pIBankerServerUserItem=m_pITableFrame->GetTableUserItem(m_wCurrentBanker);
    m_lBankerScore=pIBankerServerUserItem->GetUserScore();
  }

  //设置变量
  GameStart.cbTimeLeave=m_nPlaceJettonTime;
  GameStart.wBankerUser=m_wCurrentBanker;
  if(pIBankerServerUserItem!=NULL)
  {
    GameStart.lBankerScore=m_lBankerScore;
  }
  GameStart.bContiueCard=m_bContiueCard;

  //下注机器人数量
  int nChipRobotCount = 0;
  for(int i = 0; i < GAME_PLAYER; i++)
  {
    IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(i);
    if(pIServerUserItem != NULL && pIServerUserItem->IsAndroidUser())
    {
      nChipRobotCount++;
    }
  }

  GameStart.nChipRobotCount = min(nChipRobotCount, m_nMaxChipRobot);

  nChipRobotCount = 0;
  for(int i = 0; i < m_ApplyUserArray.GetCount(); i++)
  {
    IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(m_ApplyUserArray.GetAt(i));
    if(pIServerUserItem != NULL && pIServerUserItem->IsAndroidUser())
    {
      nChipRobotCount++;
    }
  }

  if(nChipRobotCount > 0)
  {
    GameStart.nAndriodApplyCount=nChipRobotCount-1;
  }

  //机器人控制
  m_nChipRobotCount = 0;
  ZeroMemory(m_lRobotAreaScore, sizeof(m_lRobotAreaScore));
  //Debug("当前库存为：%d",m_lStorageStart);

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

    //设置积分
    GameStart.lUserMaxScore=min(pIServerUserItem->GetUserScore(),m_lUserLimitScore);

    m_pITableFrame->SendTableData(wChairID,SUB_S_GAME_START,&GameStart,sizeof(GameStart));
  }

  return true;
}

//游戏结束
bool CTableFrameSink::OnEventGameConclude(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)
{
  switch(cbReason)
  {
    case GER_NORMAL:    //常规结束
    {
      LONGLONG lSystemLoseScore=CalculateScoreEx();
      //游戏作弊
      if(m_pServerContro != NULL && m_pServerContro->NeedControl())
      {
        //获取控制目标牌型
        m_bControl=true;
        m_pServerContro->GetSuitResult(m_cbTableCardArray, m_cbTableCard, m_lAllJettonScore);
        m_pServerContro->CompleteControl();
      }
      else
      {
        StorageOptimize();
      }

      //计算分数
      LONGLONG lBankerWinScore=CalculateScore();

      //递增次数
      m_wBankerTime++;

      //结束消息
      CMD_S_GameEnd GameEnd;
      ZeroMemory(&GameEnd,sizeof(GameEnd));

      //庄家信息
      GameEnd.nBankerTime = m_wBankerTime;
      GameEnd.lBankerTotallScore=m_lBankerWinScore;
      GameEnd.lBankerScore=lBankerWinScore;
      GameEnd.bcFirstCard = m_bcFirstPostCard;
      GameEnd.wCurrentBanker = m_wCurrentBanker;

      //扑克信息
      CopyMemory(GameEnd.cbTableCardArray,m_cbTableCardArray,sizeof(m_cbTableCardArray));
      GameEnd.cbLeftCardCount=m_cbLeftCardCount;

      //发送积分
      GameEnd.cbTimeLeave=m_nGameEndTime;
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

      /*Debug("顺门：%I64d  左下角：%I64d  桥：%I64d  对门：%I64d  倒门：%I64d  右下角：%I64d",
        m_lAllJettonScore[ID_SHUN_MEN],m_lAllJettonScore[ID_JIAO_L],m_lAllJettonScore[ID_QIAO],
        m_lAllJettonScore[ID_DUI_MEN],m_lAllJettonScore[ID_DAO_MEN],m_lAllJettonScore[ID_JIAO_R]);*/

      return true;
    }
    case GER_USER_LEAVE:    //用户离开
    case GER_NETWORK_ERROR:
    {
      //闲家判断
      if(m_wCurrentBanker!=wChairID)
      {
        //变量定义
        LONGLONG lRevenue=0;

        //是否开牌
        if(m_pITableFrame->GetGameStatus() == GS_PLACE_JETTON)
        {
          //返回下注
          for(int nAreaIndex=1; nAreaIndex<=AREA_COUNT; ++nAreaIndex)
          {

            if(m_lUserJettonScore[nAreaIndex][wChairID] != 0)
            {
              CMD_S_PlaceJettonFail PlaceJettonFail;
              ZeroMemory(&PlaceJettonFail,sizeof(PlaceJettonFail));
              PlaceJettonFail.lJettonArea=nAreaIndex;
              PlaceJettonFail.lPlaceScore=m_lUserJettonScore[nAreaIndex][wChairID];
              PlaceJettonFail.wPlaceUser=wChairID;

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
          //计算分数
          //LONGLONG lBankerWinScore=CalculateScore();

          //写入积分
          if(m_lUserWinScore[wChairID]!=0L)
          {
            //库存金币
            if(!pIServerUserItem->IsAndroidUser())
            {
              m_lStorageCurrent -= (m_lUserWinScore[wChairID]+m_lUserRevenue[wChairID]);
            }
            tagScoreInfo ScoreInfo[GAME_PLAYER];
            ZeroMemory(ScoreInfo,sizeof(ScoreInfo));
            ScoreInfo[wChairID].cbType=m_lUserWinScore[wChairID]>0?SCORE_TYPE_WIN:SCORE_TYPE_LOSE;
            ScoreInfo[wChairID].lRevenue=m_lUserRevenue[wChairID];
            ScoreInfo[wChairID].lScore=m_lUserWinScore[wChairID];
            m_pITableFrame->WriteTableScore(ScoreInfo,CountArray(ScoreInfo));
            m_lUserWinScore[wChairID] = 0;
          }
          //清除下注
          for(int nAreaIndex = 1; nAreaIndex <= AREA_COUNT; nAreaIndex++)
          {
            m_lUserJettonScore[nAreaIndex][wChairID] = 0;
          }
        }

        return true;
      }

      //状态判断
      if(m_pITableFrame->GetGameStatus()!=GS_GAME_END)
      {
        //提示消息
        TCHAR szTipMsg[128];
        _sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("由于庄家[ %s ]强退，游戏提前结束！"),pIServerUserItem->GetNickName());

        //发送消息
        SendGameMessage(INVALID_CHAIR,szTipMsg);

        //设置状态
        m_pITableFrame->SetGameStatus(GS_GAME_END);

        //设置时间
        m_dwJettonTime=(DWORD)time(NULL);
        m_pITableFrame->KillGameTimer(IDI_PLACE_JETTON);
        m_pITableFrame->SetGameTimer(IDI_GAME_END,m_nGameEndTime*1000,1,0L);

        //游戏作弊
        if(m_pServerContro != NULL && m_pServerContro->NeedControl())
        {
          //获取控制目标牌型
          m_pServerContro->GetSuitResult(m_cbTableCardArray, m_cbTableCard, m_lAllJettonScore);
          m_pServerContro->CompleteControl();
        }

        //计算分数
        CalculateScore();

        //结束消息
        CMD_S_GameEnd GameEnd;
        ZeroMemory(&GameEnd,sizeof(GameEnd));

        //庄家信息
        GameEnd.nBankerTime = m_wBankerTime;
        GameEnd.lBankerTotallScore=m_lBankerWinScore;
        GameEnd.lBankerScore=m_lUserWinScore[m_wCurrentBanker];
        GameEnd.wCurrentBanker = m_wCurrentBanker;

        //扑克信息
        CopyMemory(GameEnd.cbTableCardArray,m_cbTableCardArray,sizeof(m_cbTableCardArray));
        GameEnd.cbLeftCardCount=m_cbLeftCardCount;

        //发送积分
        GameEnd.cbTimeLeave=m_nGameEndTime;
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
      }

      //扣除分数
      tagScoreInfo ScoreInfo[GAME_PLAYER];
      ZeroMemory(ScoreInfo,sizeof(ScoreInfo));
      ScoreInfo[m_wCurrentBanker].cbType=m_lUserWinScore[m_wCurrentBanker]>0L?SCORE_TYPE_WIN:SCORE_TYPE_LOSE;
      ScoreInfo[m_wCurrentBanker].lRevenue=m_lUserRevenue[m_wCurrentBanker];
      ScoreInfo[m_wCurrentBanker].lScore=m_lUserWinScore[m_wCurrentBanker];
      m_pITableFrame->WriteTableScore(ScoreInfo,CountArray(ScoreInfo));

      //库存金币
      IServerUserItem *pIServerUserItem = m_pITableFrame->GetTableUserItem(m_wCurrentBanker);
      if(pIServerUserItem != NULL && !pIServerUserItem->IsAndroidUser())
      {
        m_lStorageCurrent -= (m_lUserWinScore[m_wCurrentBanker]+m_lUserRevenue[m_wCurrentBanker]);
      }

      m_lUserWinScore[m_wCurrentBanker] = 0;

      //切换庄家
      ChangeBanker(true);

      return true;
    }
  }

  return false;
}

//发送场景
bool CTableFrameSink::OnEventSendGameScene(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbGameStatus, bool bSendSecret)
{
  switch(cbGameStatus)
  {
    case GAME_SCENE_FREE:     //空闲状态
    {
      //发送记录
      SendGameRecord(pIServerUserItem);

      //构造数据
      CMD_S_StatusFree StatusFree;
      ZeroMemory(&StatusFree,sizeof(StatusFree));

      //控制信息
      StatusFree.lApplyBankerCondition = m_lApplyBankerCondition;
      StatusFree.lAreaLimitScore = m_lAreaLimitScore;
      StatusFree.bGenreEducate =  m_pGameServiceOption->wServerType&GAME_GENRE_EDUCATE;
      //庄家信息
      StatusFree.bEnableSysBanker=m_bEnableSysBanker;
      StatusFree.wBankerUser=m_wCurrentBanker;
      StatusFree.cbBankerTime=m_wBankerTime;
      StatusFree.lBankerWinScore=m_lBankerWinScore;
      if(m_wCurrentBanker!=INVALID_CHAIR)
      {
        IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(m_wCurrentBanker);
        StatusFree.lBankerScore=pIServerUserItem->GetUserScore();
      }
      StatusFree.nEndGameMul = m_nEndGameMul;

      //玩家信息
      if(pIServerUserItem->GetUserStatus()!=US_LOOKON)
      {
        StatusFree.lUserMaxScore=min(pIServerUserItem->GetUserScore(),m_lUserLimitScore);
      }

      //房间名称
      CopyMemory(StatusFree.szGameRoomName, m_pGameServiceOption->szServerName, sizeof(StatusFree.szGameRoomName));

      //全局信息
      DWORD dwPassTime=(DWORD)time(NULL)-m_dwJettonTime;
      StatusFree.cbTimeLeave=(BYTE)(m_nFreeTime-__min(dwPassTime,(DWORD)m_nFreeTime));

      //机器人配置
      if(pIServerUserItem->IsAndroidUser())
      {
        tagCustomConfig *pCustomConfig = (tagCustomConfig *)m_pGameServiceOption->cbCustomRule;
        ASSERT(pCustomConfig);

        CopyMemory(&StatusFree.CustomAndroid, &pCustomConfig->CustomAndroid, sizeof(tagCustomAndroid));
      }

      //发送场景
      bool bSuccess = m_pITableFrame->SendGameScene(pIServerUserItem,&StatusFree,sizeof(StatusFree));

      //限制提示
      TCHAR szTipMsg[128];
      _sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("本房间上庄金币必须大于：%I64d,区域限制为：%I64d,玩家限制为：%I64d"),m_lApplyBankerCondition,
                 m_lAreaLimitScore,m_lUserLimitScore);

      m_pITableFrame->SendGameMessage(pIServerUserItem,szTipMsg,SMT_CHAT);

      //发送申请者
      SendApplyUser(pIServerUserItem);

      //更新库存信息
      if(CUserRight::IsGameCheatUser(pIServerUserItem->GetUserRight()))
      {
        CMD_S_UpdateStorage updateStorage;
        ZeroMemory(&updateStorage, sizeof(updateStorage));

        updateStorage.cbReqType = RQ_REFRESH_STORAGE;
        updateStorage.lStorageStart = m_lStorageStart;
        updateStorage.lStorageDeduct = m_nStorageDeduct;
        updateStorage.lStorageCurrent = m_lStorageCurrent;
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

      //全局下注
      CopyMemory(StatusPlay.lAllJettonScore,m_lAllJettonScore,sizeof(StatusPlay.lAllJettonScore));

      //玩家下注
      if(pIServerUserItem->GetUserStatus()!=US_LOOKON)
      {
        for(int nAreaIndex=1; nAreaIndex<=AREA_COUNT; ++nAreaIndex)
        {
          StatusPlay.lUserJettonScore[nAreaIndex] = m_lUserJettonScore[nAreaIndex][wChairID];
        }

        //最大下注
        StatusPlay.lUserMaxScore=min(pIServerUserItem->GetUserScore(),m_lUserLimitScore);
      }

      //控制信息
      StatusPlay.lApplyBankerCondition=m_lApplyBankerCondition;
      StatusPlay.lAreaLimitScore=m_lAreaLimitScore;
      StatusPlay.bGenreEducate =  m_pGameServiceOption->wServerType&GAME_GENRE_EDUCATE;
      //庄家信息
      StatusPlay.bEnableSysBanker=m_bEnableSysBanker;
      StatusPlay.wBankerUser=m_wCurrentBanker;
      StatusPlay.cbBankerTime=m_wBankerTime;
      StatusPlay.lBankerWinScore=m_lBankerWinScore;
      if(m_wCurrentBanker!=INVALID_CHAIR)
      {
        StatusPlay.lBankerScore=m_lBankerScore;
      }
      StatusPlay.nEndGameMul = m_nEndGameMul;

      //全局信息
      DWORD dwPassTime=(DWORD)time(NULL)-m_dwJettonTime;
      int nTotalTime = (cbGameStatus==GS_PLACE_JETTON?m_nPlaceJettonTime:m_nGameEndTime);
      StatusPlay.cbTimeLeave=(BYTE)(nTotalTime-__min(dwPassTime,(DWORD)nTotalTime));
      StatusPlay.cbGameStatus=m_pITableFrame->GetGameStatus();

      //房间名称
      CopyMemory(StatusPlay.szGameRoomName, m_pGameServiceOption->szServerName, sizeof(StatusPlay.szGameRoomName));

      //结束判断
      if(cbGameStatus==GS_GAME_END)
      {
        //设置成绩
        StatusPlay.lEndUserScore=m_lUserWinScore[wChairID];

        //返还积分
        StatusPlay.lEndUserReturnScore=m_lUserReturnScore[wChairID];

        //设置税收
        if(m_lUserRevenue[wChairID]>0)
        {
          StatusPlay.lEndRevenue=m_lUserRevenue[wChairID];
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

      //机器人配置
      if(pIServerUserItem->IsAndroidUser())
      {
        tagCustomConfig *pCustomConfig = (tagCustomConfig *)m_pGameServiceOption->cbCustomRule;
        ASSERT(pCustomConfig);

        CopyMemory(&StatusPlay.CustomAndroid, &pCustomConfig->CustomAndroid, sizeof(tagCustomAndroid));
      }

      //发送场景
      bool bSuccess = m_pITableFrame->SendGameScene(pIServerUserItem,&StatusPlay,sizeof(StatusPlay));

      //限制提示
      TCHAR szTipMsg[128];
      _sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("本房间上庄金币必须大于：%I64d,区域限制为：%I64d,玩家限制为：%I64d"),m_lApplyBankerCondition,
                 m_lAreaLimitScore,m_lUserLimitScore);

      m_pITableFrame->SendGameMessage(pIServerUserItem,szTipMsg,SMT_CHAT);

      //发送申请者
      SendApplyUser(pIServerUserItem);

      //更新库存信息
      if(CUserRight::IsGameCheatUser(pIServerUserItem->GetUserRight()))
      {
        CMD_S_UpdateStorage updateStorage;
        ZeroMemory(&updateStorage, sizeof(updateStorage));

        updateStorage.cbReqType = RQ_REFRESH_STORAGE;
        updateStorage.lStorageStart = m_lStorageStart;
        updateStorage.lStorageDeduct = m_nStorageDeduct;
        updateStorage.lStorageCurrent = m_lStorageCurrent;
        updateStorage.lStorageMax1 = m_lStorageMax1;
        updateStorage.lStorageMul1 = m_lStorageMul1;
        updateStorage.lStorageMax2 = m_lStorageMax2;
        updateStorage.lStorageMul2 = m_lStorageMul2;

        m_pITableFrame->SendUserItemData(pIServerUserItem,SUB_S_UPDATE_STORAGE,&updateStorage,sizeof(updateStorage));
      }

      //发送玩家下注信息
      if(CUserRight::IsGameCheatUser(pIServerUserItem->GetUserRight()))
      {
        SendUserBetInfo(pIServerUserItem);
      }
      return bSuccess;
    }
  }

  return false;
}

//定时器事件
bool CTableFrameSink::OnTimerMessage(DWORD wTimerID, WPARAM wBindParam)
{
  switch(wTimerID)
  {
    case IDI_FREE:    //空闲时间
    {
      //开始游戏
      m_pITableFrame->StartGame();
      //设置时间
      m_dwJettonTime=(DWORD)time(NULL);
      m_pITableFrame->SetGameTimer(IDI_PLACE_JETTON,m_nPlaceJettonTime*1000,1,0L);

      //派发扑克
      DispatchTableCard();

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
        m_pITableFrame->SetGameTimer(IDI_GAME_END,m_nGameEndTime*1000,1,0L);
      }

      return true;
    }
    case IDI_GAME_END:      //结束游戏
    {
      tagScoreInfo ScoreInfo[GAME_PLAYER];
      ZeroMemory(ScoreInfo,sizeof(ScoreInfo));
      //写入积分
      for(WORD wUserChairID = 0; wUserChairID < GAME_PLAYER; ++wUserChairID)
      {
        IServerUserItem *pIServerUserItem = m_pITableFrame->GetTableUserItem(wUserChairID);
        if(pIServerUserItem == NULL||(QueryBuckleServiceCharge(wUserChairID)==false))
        {
          continue;
        }

        ScoreInfo[wUserChairID].cbType=m_lUserWinScore[wUserChairID]>0L?SCORE_TYPE_WIN:SCORE_TYPE_LOSE;
        ScoreInfo[wUserChairID].lRevenue=m_lUserRevenue[wUserChairID];
        ScoreInfo[wUserChairID].lScore=m_lUserWinScore[wUserChairID];

        //库存金币
        if(!pIServerUserItem->IsAndroidUser()/*&&!m_bControl*/)
        {
          m_lStorageCurrent -= (m_lUserWinScore[wUserChairID]+m_lUserRevenue[wUserChairID]);
        }
      }

      m_pITableFrame->WriteTableScore(ScoreInfo,CountArray(ScoreInfo));

      int lTempDeduct=0;
      lTempDeduct=m_nStorageDeduct;
      bool bDeduct=NeedDeductStorage();
      lTempDeduct=bDeduct?lTempDeduct:0;
      //库存衰减
      if(m_lStorageCurrent > 0)
      {
        m_lStorageCurrent = m_lStorageCurrent - m_lStorageCurrent*lTempDeduct/1000;
      }

      //结束游戏
      m_pITableFrame->ConcludeGame(GAME_SCENE_FREE);

      //设置时间
      m_dwJettonTime=(DWORD)time(NULL);
      m_pITableFrame->SetGameTimer(IDI_FREE,m_nFreeTime*1000,1,0L);
      m_pITableFrame->SetGameStatus(GAME_SCENE_FREE);

      //切换庄家
      ChangeBanker(false);

      //发送消息
      CMD_S_GameFree GameFree;
      ZeroMemory(&GameFree,sizeof(GameFree));
      GameFree.cbTimeLeave=m_nFreeTime;
      GameFree.lStorageStart=m_lStorageCurrent;
      GameFree.nBankerTime = m_wBankerTime;
      GameFree.wCurrentBanker = m_wCurrentBanker;
      GameFree.nListUserCount=m_ApplyUserArray.GetCount()-1;
      m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_FREE,&GameFree,sizeof(GameFree));
      m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_FREE,&GameFree,sizeof(GameFree));

      //更新库存信息
      for(WORD wUserID = 0; wUserID < GAME_PLAYER; ++wUserID)
      {
        IServerUserItem *pIServerUserItem = m_pITableFrame->GetTableUserItem(wUserID);
        if(pIServerUserItem == NULL)
        {
          continue;
        }
        if(!CUserRight::IsGameCheatUser(pIServerUserItem->GetUserRight()))
        {
          continue;
        }

        CMD_S_UpdateStorage updateStorage;
        ZeroMemory(&updateStorage, sizeof(updateStorage));

        updateStorage.cbReqType = RQ_REFRESH_STORAGE;
        updateStorage.lStorageStart = m_lStorageStart;
        updateStorage.lStorageDeduct = m_nStorageDeduct;
        updateStorage.lStorageCurrent = m_lStorageCurrent;
        updateStorage.lStorageMax1 = m_lStorageMax1;
        updateStorage.lStorageMul1 = m_lStorageMul1;
        updateStorage.lStorageMax2 = m_lStorageMax2;
        updateStorage.lStorageMul2 = m_lStorageMul2;

        m_pITableFrame->SendUserItemData(pIServerUserItem,SUB_S_UPDATE_STORAGE,&updateStorage,sizeof(updateStorage));
      }

      return true;
    }
  }

  return false;
}

//游戏消息处理
bool CTableFrameSink::OnGameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem)
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
      CMD_C_PlaceJetton * pPlaceJetton=(CMD_C_PlaceJetton *)pData;
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
    case SUB_C_CONTINUE_CARD:   //继续发牌
    {
      //用户效验
      tagUserInfo * pUserData=pIServerUserItem->GetUserInfo();
      if(pUserData->cbUserStatus==US_LOOKON)
      {
        return true;
      }
      if(pUserData->wChairID!=m_wCurrentBanker)
      {
        return true;
      }
      if(m_cbLeftCardCount < 8)
      {
        return true;
      }

      //设置变量
      m_bContiueCard=true;

      //发送消息
      SendGameMessage(pUserData->wChairID,TEXT("设置成功，下一局将继续发牌！"));
      return true;
    }
    case SUB_C_AMDIN_COMMAND:
    {
      ASSERT(wDataSize==sizeof(CMD_C_AdminReq));
      if(wDataSize!=sizeof(CMD_C_AdminReq))
      {
        return false;
      }

      //权限判断
      if(CUserRight::IsGameCheatUser(pIServerUserItem->GetUserRight())==false)
      {
        return false;
      }

      if(m_pServerContro == NULL)
      {
        return false;
      }
      CopyMemory(m_szControlName,pIServerUserItem->GetNickName(),sizeof(m_szControlName));

      return m_pServerContro->ServerControl(wSubCmdID, pData, wDataSize, pIServerUserItem, m_pITableFrame, m_pGameServiceOption);
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
      CMD_C_UpdateStorage * pUpdateStorage=(CMD_C_UpdateStorage *)pData;
      if(pUpdateStorage->cbReqType==RQ_SET_STORAGE)
      {
        m_nStorageDeduct = pUpdateStorage->lStorageDeduct;
        m_lStorageCurrent = pUpdateStorage->lStorageCurrent;
        m_lStorageMax1 = pUpdateStorage->lStorageMax1;
        m_lStorageMul1 = pUpdateStorage->lStorageMul1;
        m_lStorageMax2 = pUpdateStorage->lStorageMax2;
        m_lStorageMul2 = pUpdateStorage->lStorageMul2;

        //记录信息
        CString strControlInfo, str;
        str.Format(TEXT("%s"), TEXT("修改库存设置！"));
        CTime Time(CTime::GetCurrentTime());
        strControlInfo.Format(TEXT("房间: %s | 桌号: %u | 时间: %d-%d-%d %d:%d:%d\n控制人账号: %s | 控制人ID: %u\n%s\r\n"),
                              m_pGameServiceOption->szServerName, m_pITableFrame->GetTableID()+1, Time.GetYear(), Time.GetMonth(), Time.GetDay(),
                              Time.GetHour(), Time.GetMinute(), Time.GetSecond(), pIServerUserItem->GetNickName(), pIServerUserItem->GetGameID(), str);

        WriteInfo(TEXT("百人红九控制信息.log"),strControlInfo);
      }

      for(WORD wUserID = 0; wUserID < GAME_PLAYER; wUserID++)
      {
        IServerUserItem *pIServerUserItemSend = m_pITableFrame->GetTableUserItem(wUserID);
        if(pIServerUserItemSend == NULL)
        {
          continue;
        }
        if(!CUserRight::IsGameCheatUser(pIServerUserItemSend->GetUserRight()))
        {
          continue;
        }

        if(RQ_REFRESH_STORAGE == pUpdateStorage->cbReqType && pIServerUserItem->GetChairID() != wUserID)
        {
          continue;
        }

        CMD_S_UpdateStorage updateStorage;
        ZeroMemory(&updateStorage, sizeof(updateStorage));

        if(RQ_SET_STORAGE == pUpdateStorage->cbReqType && pIServerUserItem->GetChairID() == wUserID)
        {
          updateStorage.cbReqType = RQ_SET_STORAGE;
        }
        else
        {
          updateStorage.cbReqType = RQ_REFRESH_STORAGE;
        }

        updateStorage.lStorageStart = m_lStorageStart;
        updateStorage.lStorageDeduct = m_nStorageDeduct;
        updateStorage.lStorageCurrent = m_lStorageCurrent;
        updateStorage.lStorageMax1 = m_lStorageMax1;
        updateStorage.lStorageMul1 = m_lStorageMul1;
        updateStorage.lStorageMax2 = m_lStorageMax2;
        updateStorage.lStorageMul2 = m_lStorageMul2;

        m_pITableFrame->SendUserItemData(pIServerUserItem,SUB_S_UPDATE_STORAGE,&updateStorage,sizeof(updateStorage));
      }




      return true;
    }
  }

  return false;
}

//框架消息处理
bool CTableFrameSink::OnFrameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
  return false;
}

//数据事件
bool CTableFrameSink::OnGameDataBase(WORD wRequestID, VOID * pData, WORD wDataSize)
{
  return false;
}

//用户坐下
bool CTableFrameSink::OnActionUserSitDown(WORD wChairID,IServerUserItem * pIServerUserItem, bool bLookonUser)
{
  //起始分数
  m_lUserStartScore[wChairID] = pIServerUserItem->GetUserScore();

  //设置时间
  if((bLookonUser==false)&&(m_dwJettonTime==0L))
  {
    m_dwJettonTime=(DWORD)time(NULL);
    m_pITableFrame->SetGameTimer(IDI_FREE,m_nFreeTime*1000,1,NULL);
    m_pITableFrame->SetGameStatus(GAME_SCENE_FREE);
  }

  //限制提示
  TCHAR szTipMsg[128];
  _sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("本房间上庄金币必须大于：%I64d,区域限制为：%I64d,玩家限制为：%I64d"),m_lApplyBankerCondition,
             m_lAreaLimitScore,m_lUserLimitScore);
  m_pITableFrame->SendGameMessage(pIServerUserItem,szTipMsg,SMT_CHAT|SMT_EJECT);

  return true;
}

//用户起来
bool CTableFrameSink::OnActionUserStandUp(WORD wChairID,IServerUserItem * pIServerUserItem, bool bLookonUser)
{
  //起始分数
  m_lUserStartScore[wChairID] = 0;

  //记录成绩
  if(bLookonUser==false)
  {
    //切换庄家
    if(pIServerUserItem->GetChairID()==m_wCurrentBanker)
    {
      ChangeBanker(true);
      m_bContiueCard=false;
    }

    //取消申请
    for(WORD i=0; i<m_ApplyUserArray.GetCount(); ++i)
    {
      if(pIServerUserItem->GetChairID()!=m_ApplyUserArray[i])
      {
        continue;
      }

      //删除玩家
      m_ApplyUserArray.RemoveAt(i);

      //构造变量
      CMD_S_CancelBanker CancelBanker;
      ZeroMemory(&CancelBanker,sizeof(CancelBanker));

      //设置变量
      CancelBanker.wCancelUser=wChairID;

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
  ASSERT((cbJettonArea<=ID_JIAO_R && cbJettonArea>=ID_SHUN_MEN)&&(lJettonScore>0L));
  if((cbJettonArea>ID_JIAO_R)||(lJettonScore<=0L) || cbJettonArea<ID_SHUN_MEN)
  {
    return false;
  }

  //效验状态
  ASSERT(m_pITableFrame->GetGameStatus()==GS_PLACE_JETTON||m_pITableFrame->GetGameStatus()==GS_GAME_END);
  if(m_pITableFrame->GetGameStatus() != GS_PLACE_JETTON)
  {
    if(m_pITableFrame->GetGameStatus() == GS_GAME_END)
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

  //庄家判断
  if(m_wCurrentBanker==wChairID)
  {
    return true;
  }
  if(m_bEnableSysBanker==false && m_wCurrentBanker==INVALID_CHAIR)
  {
    return true;
  }

  //庄家积分
  LONGLONG lBankerScore = 0;
  if(INVALID_CHAIR != m_wCurrentBanker)
  {
    IServerUserItem * pIServerUserItemBanker = m_pITableFrame->GetTableUserItem(m_wCurrentBanker);
    lBankerScore = pIServerUserItemBanker->GetUserScore();
  }

  //变量定义
  IServerUserItem * pIServerUserItem=m_pITableFrame->GetTableUserItem(wChairID);
  LONGLONG lJettonCount=0L;
  for(int nAreaIndex=1; nAreaIndex<=AREA_COUNT; ++nAreaIndex)
  {
    lJettonCount += m_lUserJettonScore[nAreaIndex][wChairID];
  }
  LONGLONG lAllJettonCount=0L;
  for(int nAreaIndex=1; nAreaIndex<=AREA_COUNT; ++nAreaIndex)
  {
    lAllJettonCount += m_lAllJettonScore[nAreaIndex];
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

  //合法验证
  if(GetUserMaxJetton(wChairID, cbJettonArea) >= lJettonScore)
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
      for(int i = 0; i <= AREA_COUNT; i++)
      {
        if(m_lUserJettonScore[i+1][wChairID] != 0)
        {
          bHaveChip = true;
        }
      }

      if(!bHaveChip && bPlaceJettonSuccess)
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
      //保存下注
      m_lAllJettonScore[cbJettonArea] += lJettonScore;
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

    //构造变量
    PlaceJetton.wChairID=wChairID;
    PlaceJetton.cbJettonArea=cbJettonArea;
    PlaceJetton.lJettonScore=lJettonScore;
    PlaceJetton.bIsAndroid=pIServerUserItem->IsAndroidUser();
    PlaceJetton.bAndroid=pIServerUserItem->IsAndroidUser()?TRUE:FALSE;

    //发送消息
    m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_PLACE_JETTON,&PlaceJetton,sizeof(PlaceJetton));
    m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_PLACE_JETTON,&PlaceJetton,sizeof(PlaceJetton));

    //发送玩家下注信息
    if(!pIServerUserItem->IsAndroidUser())
    {
      for(WORD i=0; i<GAME_PLAYER; i++)
      {
        IServerUserItem * pIServerUserItemSend = m_pITableFrame->GetTableUserItem(i);
        if(NULL == pIServerUserItemSend)
        {
          continue;
        }
        if(!CUserRight::IsGameCheatUser(pIServerUserItemSend->GetUserRight()))
        {
          continue;
        }

        SendUserBetInfo(pIServerUserItemSend);
      }
    }

    //下注总额超过庄家能支付上限时提前结束游戏
    if(INVALID_CHAIR != m_wCurrentBanker)
    {
      LONGLONG lBankerCanUseScore = lBankerScore*m_nEndGameMul/100;
      if(lBankerCanUseScore <= (lAllJettonCount + lJettonScore + 100))
      {
        if(m_pITableFrame->GetGameStatus()!=GS_GAME_END)
        {
          //设置状态
          m_pITableFrame->SetGameStatus(GS_GAME_END);

          //结束游戏
          OnEventGameConclude(INVALID_CHAIR,NULL,GER_NORMAL);

          //设置时间
          m_dwJettonTime=(DWORD)time(NULL);
          m_pITableFrame->SetGameTimer(IDI_GAME_END,m_nGameEndTime*1000,1,0L);
        }
      }
    }
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

//发送扑克
bool CTableFrameSink::DispatchTableCard()
{
  //继续发牌
  int const static nDispatchCardCount=8;
  bool bContiueCard=m_bContiueCard;
  if(m_cbLeftCardCount<nDispatchCardCount)
  {
    bContiueCard=false;
  }

  //继续发牌
  if(true==bContiueCard)
  {
    //设置扑克
    CopyMemory(&m_cbTableCardArray[0][0],m_cbTableCard,sizeof(m_cbTableCardArray));
    m_cbLeftCardCount -= nDispatchCardCount;
    if(m_cbLeftCardCount>0)
    {
      CopyMemory(m_cbTableCard, m_cbTableCard + nDispatchCardCount, sizeof(BYTE)*m_cbLeftCardCount);
    }
  }
  else
  {
    //-----------------------------------------------------

    /*
    0x0C,0x2C,        //12 44     //  Q
    0x0B,0x2B,        //11 43     //  J
    0x0A,0x1A,0x2A,0x3A,    //10 26 42 58 //  10
    0x09,0x29,        //9  41     //  9
    0x07,0x17,0x27,0x37,  //7       //  7
    0x08,0x18,0x28,0x38,  //8
    0x06,0x16,0x26,0x36,  //6
    0x05,0x25,        //5
    0x04,0x14,0x24,0x34,  //4
    0x02,0x22,        //2
    0x31,         //49
    0x33          //51*/

    /*m_cbTableCard[0] = 0x17;
    m_cbTableCard[1] = 0x37;

    m_cbTableCard[2] = 0x17;
    m_cbTableCard[3] = 0x37;

    m_cbTableCard[4] = 0x17;
    m_cbTableCard[5] = 0x37;

    m_cbTableCard[6] = 0x17;
    m_cbTableCard[7] = 0x37;*/

    //-----------------------------------------------------

    //重新洗牌
    BYTE bcCard[CARD_COUNT];
    m_GameLogic.RandCardList(bcCard,32);
    m_bcFirstPostCard = bcCard[0];
    m_GameLogic.RandCardList(m_cbTableCard,CountArray(m_cbTableCard));

    //设置扑克
    CopyMemory(&m_cbTableCardArray[0][0], m_cbTableCard, sizeof(m_cbTableCardArray));

    //庄家判断
    if(INVALID_CHAIR != m_wCurrentBanker)
    {
      m_cbLeftCardCount=CountArray(m_cbTableCard)-nDispatchCardCount;
      CopyMemory(m_cbTableCard, m_cbTableCard + nDispatchCardCount, sizeof(BYTE)*m_cbLeftCardCount);
    }
    else
    {
      m_cbLeftCardCount=0;
    }
  }

  //设置数目
  for(int i=0; i<CountArray(m_cbCardCount); ++i)
  {
    m_cbCardCount[i]=2;
  }

  //发牌标志
  m_bContiueCard=false;

  //作弊处理
  /*CMD_S_Cheat cheat = {};
  memcpy(cheat.cbTableCardArray, m_cbTableCardArray, sizeof(cheat.cbTableCardArray));
  for (int i = 0; i < GAME_PLAYER; i++)
  {
  IServerUserItem *pUserItem = m_pITableFrame->GetTableUserItem(i);
  if (pUserItem != NULL && (pUserItem->GetUserInfo()->dwUserRight&UR_GAME_CONTROL) != 0)
  m_pITableFrame->SendTableData(i, SUB_S_CHEAT, &cheat, sizeof(cheat));
  }*/

  return true;
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

  if(pIApplyServerUserItem->IsAndroidUser()&&(m_ApplyUserArray.GetCount())>m_nRobotListMaxCount)
  {
    return true;
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
  if(m_pITableFrame->GetGameStatus()==GAME_SCENE_FREE && m_ApplyUserArray.GetCount()==1)
  {
    ChangeBanker(false);
  }

  return true;
}

//取消申请
bool CTableFrameSink::OnUserCancelBanker(IServerUserItem *pICancelServerUserItem)
{
  //当前庄家
  if(pICancelServerUserItem->GetChairID()==m_wCurrentBanker && m_pITableFrame->GetGameStatus()!=GAME_SCENE_FREE)
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

      //设置变量
      CancelBanker.wCancelUser = pIServerUserItem->GetChairID();

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

  return false;
}

//更换庄家
bool CTableFrameSink::ChangeBanker(bool bCancelCurrentBanker)
{
  //切换标识
  bool bChangeBanker=false;

  //做庄次数
  WORD wBankerTime=m_nBankerTimeLimit;

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
    TakeTurns();

    //设置变量
    bChangeBanker=true;
    m_bExchangeBanker = true;
    m_wAddTime=0;
  }
  //轮庄判断
  else if(m_wCurrentBanker!=INVALID_CHAIR)
  {
    //获取庄家
    IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(m_wCurrentBanker);

    //次数判断
    if(pIServerUserItem!= NULL)
    {
      LONGLONG lBankerScore=pIServerUserItem->GetUserScore();

      //次数判断
      if(wBankerTime+m_wAddTime<=m_wBankerTime || lBankerScore<m_lApplyBankerCondition)
      {
        //庄家增加判断 同一个庄家情况下只判断一次
        if(wBankerTime <= m_wBankerTime && m_bExchangeBanker && lBankerScore >= m_lApplyBankerCondition)
        {
          //加庄局数设置：当庄家坐满设定的局数之后(m_wBankerTime)，
          //所带金币值还超过下面申请庄家列表里面所有玩家金币时，
          //可以再加坐庄m_lBankerAdd局，加庄局数可设置。

          //金币超过m_lExtraBankerScore之后，
          //就算是下面玩家的金币值大于他的金币值，他也可以再加庄m_lBankerScoreAdd局。
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

          if(bScoreMAX || (lBankerScore > m_lExtraBankerScore && m_lExtraBankerScore != 0l))
          {
            BYTE bType = 0;
            if(bScoreMAX && m_nBankerTimeAdd != 0 && m_nBankerTimeAdd != 0)
            {
              bType = 1;
              m_wAddTime = m_nBankerTimeAdd;
            }
            if(lBankerScore > m_lExtraBankerScore && m_lExtraBankerScore != 0l && m_nExtraBankerTime != 0)
            {
              bType += 2;
              if(bType == 3)
              {
                //bType = (m_nExtraBankerTime>m_nBankerTimeAdd?2:1);
                //m_wAddTime = (m_nExtraBankerTime>m_nBankerTimeAdd?m_nExtraBankerTime:m_nBankerTimeAdd);
                m_wAddTime = m_nBankerTimeAdd + m_nExtraBankerTime;
              }
              else
              {
                m_wAddTime = m_nExtraBankerTime;
              }
            }

            //提示消息
            TCHAR szTipMsg[128] = {};
            if(bType == 1)
            {
              _sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("[ %s ]的分数超过其他申请上庄玩家，获得%d次额外坐庄次数!"),pIServerUserItem->GetNickName(),m_wAddTime);
            }
            else if(bType == 2)
            {
              _sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("[ %s ]的分数超过[%I64d]，获得%d次额外坐庄次数!"),pIServerUserItem->GetNickName(),m_lExtraBankerScore,m_wAddTime);
            }
            else if(bType == 3)
            {
              _sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("[ %s ]的分数超过[%I64d]且超过其他申请玩家，获得%d次额外坐庄次数!"),pIServerUserItem->GetNickName(),m_lExtraBankerScore,m_wAddTime);
            }
            else
            {
              bType = 0;
            }

            if(bType != 0)
            {
              //发送消息
              SendGameMessage(INVALID_CHAIR,szTipMsg);
              return true;
            }
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
        TakeTurns();

        //提示消息
        TCHAR szTipMsg[128];
        if(lBankerScore<m_lApplyBankerCondition)
        {
          _sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("[ %s ]分数少于(%I64d)，强行换庄!"),pIServerUserItem->GetNickName(),m_lApplyBankerCondition);
        }
        else
        {
          _sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("[ %s ]做庄次数达到(%d)，强行换庄!"),pIServerUserItem->GetNickName(),wBankerTime+m_wAddTime);
        }

        bChangeBanker=true;
        m_bExchangeBanker = true;
        m_wAddTime = 0;

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
  else if(m_wCurrentBanker==INVALID_CHAIR && m_ApplyUserArray.GetCount()!=0)
  {
    //轮换判断
    TakeTurns();

    bChangeBanker=true;
    m_bExchangeBanker = true;
    m_wAddTime = 0;
  }

  //切换判断
  if(bChangeBanker)
  {
    //设置变量
    m_wBankerTime = 0;
    m_lBankerWinScore=0;

    //发送消息
    CMD_S_ChangeBanker ChangeBanker;
    ZeroMemory(&ChangeBanker,sizeof(ChangeBanker));
    ChangeBanker.wBankerUser=m_wCurrentBanker;
    if(m_wCurrentBanker!=INVALID_CHAIR)
    {
      IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(m_wCurrentBanker);
      ChangeBanker.lBankerScore=pIServerUserItem->GetUserScore();
    }
    m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_CHANGE_BANKER,&ChangeBanker,sizeof(ChangeBanker));
    m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_CHANGE_BANKER,&ChangeBanker,sizeof(ChangeBanker));

    if(m_wCurrentBanker!=INVALID_CHAIR)
    {
      //获取参数
      m_pGameServiceOption=m_pITableFrame->GetGameServiceOption();
      ASSERT(m_pGameServiceOption!=NULL);

      //读取消息
      int nMessageCount = 3;

      //读取配置
      INT nIndex=rand()%nMessageCount;
      TCHAR szMessage1[256],szMessage2[256];
      tagCustomConfig *pCustomConfig = (tagCustomConfig *)m_pGameServiceOption->cbCustomRule;
      ASSERT(pCustomConfig);
      if(0 == nIndex)
      {
        CopyMemory(szMessage1, pCustomConfig->CustomGeneral.szMessageItem1, sizeof(pCustomConfig->CustomGeneral.szMessageItem1));
      }
      else if(1 == nIndex)
      {
        CopyMemory(szMessage1, pCustomConfig->CustomGeneral.szMessageItem2, sizeof(pCustomConfig->CustomGeneral.szMessageItem2));
      }
      else if(2 == nIndex)
      {
        CopyMemory(szMessage1, pCustomConfig->CustomGeneral.szMessageItem3, sizeof(pCustomConfig->CustomGeneral.szMessageItem3));
      }

      //获取玩家
      IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(m_wCurrentBanker);

      //发送消息
      _sntprintf(szMessage2,CountArray(szMessage2),TEXT("【 %s 】上庄了，%s"),pIServerUserItem->GetNickName(), szMessage1);
      SendGameMessage(INVALID_CHAIR,szMessage2);
    }
  }

  return bChangeBanker;
}

//轮换判断
void CTableFrameSink::TakeTurns()
{
  //变量定义
  int nInvalidApply = 0;

  for(int i = 0; i < m_ApplyUserArray.GetCount(); i++)
  {
    if(m_pITableFrame->GetGameStatus() == GAME_SCENE_FREE)
    {
      //获取分数
      IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(m_ApplyUserArray[i]);
      if(pIServerUserItem != NULL)
      {
        if(pIServerUserItem->GetUserScore() >= m_lApplyBankerCondition)
        {
          m_wCurrentBanker=m_ApplyUserArray[i];
          break;
        }
        else
        {
          nInvalidApply = i + 1;

          //发送消息
          CMD_S_CancelBanker CancelBanker = {};

          //设置变量
          CancelBanker.wCancelUser=pIServerUserItem->GetChairID();

          //发送消息
          m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));
          m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));

          //提示消息
          TCHAR szTipMsg[128] = {};
          _sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("由于你的金币数（%I64d）少于坐庄必须金币数（%I64d）,你无法上庄！"),
                     pIServerUserItem->GetUserScore(), m_lApplyBankerCondition);
          SendGameMessage(m_ApplyUserArray[i],szTipMsg);
        }
      }
    }
  }

  //删除玩家
  if(nInvalidApply != 0)
  {
    m_ApplyUserArray.RemoveAt(0, nInvalidApply);
  }
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
bool CTableFrameSink::OnActionUserOffLine(WORD wChairID,IServerUserItem * pIServerUserItem)
{
  //切换庄家
  if(pIServerUserItem->GetChairID()==m_wCurrentBanker)
  {
    ChangeBanker(true);
  }

  //取消申请
  for(WORD i=0; i<m_ApplyUserArray.GetCount(); ++i)
  {
    if(pIServerUserItem->GetChairID()!=m_ApplyUserArray[i])
    {
      continue;
    }

    //删除玩家
    m_ApplyUserArray.RemoveAt(i);

    //构造变量
    CMD_S_CancelBanker CancelBanker;
    ZeroMemory(&CancelBanker,sizeof(CancelBanker));

    //设置变量
    CancelBanker.wCancelUser=wChairID;

    //发送消息
    m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));
    m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));

    break;
  }

  return true;
}

//最大下注
LONGLONG CTableFrameSink::GetUserMaxJetton(WORD wChairID, BYTE cbArea)
{
  //获取玩家
  IServerUserItem *pIMeServerUserItem=m_pITableFrame->GetTableUserItem(wChairID);
  if(NULL==pIMeServerUserItem)
  {
    return 0L;
  }

  //已下注额
  LONGLONG lNowJetton = 0;
  ASSERT(AREA_COUNT<=CountArray(m_lUserJettonScore));
  for(int nAreaIndex=1; nAreaIndex<=AREA_COUNT; ++nAreaIndex)
  {
    lNowJetton += m_lUserJettonScore[nAreaIndex][wChairID];
  }

  //庄家金币
  LONGLONG lBankerScore=2147483647;
  if(m_wCurrentBanker!=INVALID_CHAIR)
  {
    lBankerScore=m_lBankerScore*m_nEndGameMul/100;
  }
  for(int nAreaIndex=1; nAreaIndex<=AREA_COUNT; ++nAreaIndex)
  {
    lBankerScore-=m_lAllJettonScore[nAreaIndex];
  }

  //个人限制
  LONGLONG lMeMaxScore = min((pIMeServerUserItem->GetUserScore()-lNowJetton), m_lUserLimitScore);

  //区域限制
  lMeMaxScore=min(lMeMaxScore,m_lAreaLimitScore-m_lAllJettonScore[cbArea]);

  //庄家限制
  lMeMaxScore=min(lMeMaxScore,lBankerScore);

  //非零限制
  ASSERT(lMeMaxScore >= 0);
  lMeMaxScore = max(lMeMaxScore, 0);

  return lMeMaxScore;
}

//计算得分
LONGLONG CTableFrameSink::CalculateScore()
{
  //变量定义
  WORD static cbRevenue=m_pGameServiceOption->wRevenueRatio;

  //推断玩家
  bool static bWinShunMen, bWinDuiMen, bWinDaoMen;
  DeduceWinner(bWinShunMen, bWinDuiMen, bWinDaoMen);

  //游戏记录
  tagServerGameRecord &GameRecord = m_GameRecordArrary[m_nRecordLast];
  GameRecord.bWinShunMen=bWinShunMen;
  GameRecord.bWinDuiMen=bWinDuiMen;
  GameRecord.bWinDaoMen=bWinDaoMen;

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
  LONGLONG lUserLostScore[GAME_PLAYER] = {};
  ZeroMemory(lUserLostScore, sizeof(lUserLostScore));

  //玩家下注
  LONGLONG *const pUserScore[] = {NULL, m_lUserJettonScore[ID_SHUN_MEN], m_lUserJettonScore[ID_JIAO_L], m_lUserJettonScore[ID_QIAO],
                                  m_lUserJettonScore[ID_DUI_MEN], m_lUserJettonScore[ID_DAO_MEN], m_lUserJettonScore[ID_JIAO_R]
                                 };

  //区域倍率
  BYTE static const cbMultiple[] = {0, 1, 1, 1, 1, 1, 1};

  //胜利标识
  bool static bWinFlag[AREA_COUNT+1];
  bWinFlag[0]=false;
  bWinFlag[ID_SHUN_MEN]=bWinShunMen;
  bWinFlag[ID_JIAO_L]=(true==bWinShunMen && true==bWinDuiMen) ? true : false;
  bWinFlag[ID_QIAO]=(true==bWinShunMen && true==bWinDaoMen) ? true : false;
  bWinFlag[ID_DUI_MEN]=bWinDuiMen;
  bWinFlag[ID_DAO_MEN]=bWinDaoMen;
  bWinFlag[ID_JIAO_R]=(true==bWinDaoMen && true==bWinDuiMen) ? true : false;

  //角标识
  bool static bWinBankerJiaoL,bWinBankerJiaoR,bWinBankerQiao;
  bWinBankerJiaoL=(false==bWinShunMen && false==bWinDuiMen) ? true : false;
  bWinBankerJiaoR=(false==bWinDaoMen && false==bWinDuiMen) ? true : false;
  bWinBankerQiao=(false==bWinShunMen && false==bWinDaoMen) ? true : false;

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

    for(WORD wAreaIndex = ID_SHUN_MEN; wAreaIndex <= ID_JIAO_R; ++wAreaIndex)
    {
      //角判断
      bool bReturnScore=false;
      if(ID_JIAO_L==wAreaIndex && false==bWinBankerJiaoL)
      {
        bReturnScore=true;
      }
      if(ID_JIAO_R==wAreaIndex && false==bWinBankerJiaoR)
      {
        bReturnScore=true;
      }
      if(ID_QIAO==wAreaIndex && false==bWinBankerQiao)
      {
        bReturnScore=true;
      }

      if(true==bWinFlag[wAreaIndex])
      {
        m_lUserWinScore[wChairID] += (pUserScore[wAreaIndex][wChairID] * cbMultiple[wAreaIndex]) ;
        m_lUserReturnScore[wChairID] += pUserScore[wAreaIndex][wChairID] ;
        lBankerWinScore -= (pUserScore[wAreaIndex][wChairID] * cbMultiple[wAreaIndex]) ;
      }
      else if(true==bReturnScore)
      {
        m_lUserReturnScore[wChairID] += pUserScore[wAreaIndex][wChairID] ;
      }
      else
      {
        lUserLostScore[wChairID] -= pUserScore[wAreaIndex][wChairID];
        lBankerWinScore += pUserScore[wAreaIndex][wChairID];
      }
    }

    //总的分数
    m_lUserWinScore[wChairID] += lUserLostScore[wChairID];

    //计算税收
    if(0 < m_lUserWinScore[wChairID])
    {
      double fRevenuePer=double(cbRevenue/1000.0);
      m_lUserRevenue[wChairID]  = LONGLONG(m_lUserWinScore[wChairID]*fRevenuePer);
      m_lUserWinScore[wChairID] -= m_lUserRevenue[wChairID];
    }
  }

  //庄家成绩
  if(m_wCurrentBanker!=INVALID_CHAIR)
  {
    m_lUserWinScore[m_wCurrentBanker] = lBankerWinScore;

    //计算税收
    if(0 < m_lUserWinScore[m_wCurrentBanker])
    {
      double fRevenuePer=double(cbRevenue/1000.0);
      m_lUserRevenue[m_wCurrentBanker]  = LONGLONG(m_lUserWinScore[m_wCurrentBanker]*fRevenuePer);
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

//预先计算得分
LONGLONG CTableFrameSink::CalculateScoreEx()
{
  //变量定义
  WORD static cbRevenue=m_pGameServiceOption->wRevenueRatio;

  //推断玩家
  bool static bWinShunMen, bWinDuiMen, bWinDaoMen;
  DeduceWinner(bWinShunMen, bWinDuiMen, bWinDaoMen);

  //游戏记录
  /*tagServerGameRecord &GameRecord = m_GameRecordArrary[m_nRecordLast];
  GameRecord.bWinShunMen=bWinShunMen;
  GameRecord.bWinDuiMen=bWinDuiMen;
  GameRecord.bWinDaoMen=bWinDaoMen;*/

  //移动下标
  /*m_nRecordLast = (m_nRecordLast+1) % MAX_SCORE_HISTORY;
  if ( m_nRecordLast == m_nRecordFirst ) m_nRecordFirst = (m_nRecordFirst+1) % MAX_SCORE_HISTORY;*/

  //庄家总量
  LONGLONG lBankerWinScore = 0;

  //玩家成绩
  ZeroMemory(m_lUserWinScore, sizeof(m_lUserWinScore));
  ZeroMemory(m_lUserReturnScore, sizeof(m_lUserReturnScore));
  ZeroMemory(m_lUserRevenue, sizeof(m_lUserRevenue));
  LONGLONG lUserLostScore[GAME_PLAYER] = {};
  ZeroMemory(lUserLostScore, sizeof(lUserLostScore));

  //玩家下注
  LONGLONG *const pUserScore[] = {NULL, m_lUserJettonScore[ID_SHUN_MEN], m_lUserJettonScore[ID_JIAO_L], m_lUserJettonScore[ID_QIAO],
                                  m_lUserJettonScore[ID_DUI_MEN], m_lUserJettonScore[ID_DAO_MEN], m_lUserJettonScore[ID_JIAO_R]
                                 };

  //区域倍率
  BYTE static const cbMultiple[] = {0, 1, 1, 1, 1, 1, 1};

  //胜利标识
  bool static bWinFlag[AREA_COUNT+1];
  bWinFlag[0]=false;
  bWinFlag[ID_SHUN_MEN]=bWinShunMen;
  bWinFlag[ID_JIAO_L]=(true==bWinShunMen && true==bWinDuiMen) ? true : false;
  bWinFlag[ID_QIAO]=(true==bWinShunMen && true==bWinDaoMen) ? true : false;
  bWinFlag[ID_DUI_MEN]=bWinDuiMen;
  bWinFlag[ID_DAO_MEN]=bWinDaoMen;
  bWinFlag[ID_JIAO_R]=(true==bWinDaoMen && true==bWinDuiMen) ? true : false;

  //角标识
  bool static bWinBankerJiaoL,bWinBankerJiaoR,bWinBankerQiao;
  bWinBankerJiaoL=(false==bWinShunMen && false==bWinDuiMen) ? true : false;
  bWinBankerJiaoR=(false==bWinDaoMen && false==bWinDuiMen) ? true : false;
  bWinBankerQiao=(false==bWinShunMen && false==bWinDaoMen) ? true : false;

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

    for(WORD wAreaIndex = ID_SHUN_MEN; wAreaIndex <= ID_JIAO_R; ++wAreaIndex)
    {
      //角判断
      bool bReturnScore=false;
      if(ID_JIAO_L==wAreaIndex && false==bWinBankerJiaoL)
      {
        bReturnScore=true;
      }
      if(ID_JIAO_R==wAreaIndex && false==bWinBankerJiaoR)
      {
        bReturnScore=true;
      }
      if(ID_QIAO==wAreaIndex && false==bWinBankerQiao)
      {
        bReturnScore=true;
      }

      if(true==bWinFlag[wAreaIndex])
      {
        m_lUserWinScore[wChairID] += (pUserScore[wAreaIndex][wChairID] * cbMultiple[wAreaIndex]) ;
        m_lUserReturnScore[wChairID] += pUserScore[wAreaIndex][wChairID] ;
        lBankerWinScore -= (pUserScore[wAreaIndex][wChairID] * cbMultiple[wAreaIndex]) ;
      }
      else if(true==bReturnScore)
      {
        m_lUserReturnScore[wChairID] += pUserScore[wAreaIndex][wChairID] ;
      }
      else
      {
        lUserLostScore[wChairID] -= pUserScore[wAreaIndex][wChairID];
        lBankerWinScore += pUserScore[wAreaIndex][wChairID];
      }
    }

    //计算税收
    if(0 < m_lUserWinScore[wChairID])
    {
      double fRevenuePer=double(cbRevenue/1000.0);
      m_lUserRevenue[wChairID]  = LONGLONG(m_lUserWinScore[wChairID]*fRevenuePer);
      m_lUserWinScore[wChairID] -= m_lUserRevenue[wChairID];
    }

    //总的分数
    m_lUserWinScore[wChairID] += lUserLostScore[wChairID];
  }

  //庄家成绩
  if(m_wCurrentBanker!=INVALID_CHAIR)
  {
    m_lUserWinScore[m_wCurrentBanker] = lBankerWinScore;

    //计算税收
    if(0 < m_lUserWinScore[m_wCurrentBanker])
    {
      double fRevenuePer=double(cbRevenue/1000.0);
      m_lUserRevenue[m_wCurrentBanker]  = LONGLONG(m_lUserWinScore[m_wCurrentBanker]*fRevenuePer);
      m_lUserWinScore[m_wCurrentBanker] -= m_lUserRevenue[m_wCurrentBanker];
      lBankerWinScore = m_lUserWinScore[m_wCurrentBanker];
    }
  }
  //当前积分
  m_lBankerCurGameScore=lBankerWinScore;

  return lBankerWinScore;
}

void CTableFrameSink::DeduceWinner(bool &bWinShunMen, bool &bWinDuiMen, bool &bWinDaoMen)
{
  //大小比较
  bWinShunMen=m_GameLogic.CompareCard(m_cbTableCardArray[BANKER_INDEX],2,m_cbTableCardArray[SHUN_MEN_INDEX],2)==1?true:false;
  bWinDuiMen=m_GameLogic.CompareCard(m_cbTableCardArray[BANKER_INDEX],2,m_cbTableCardArray[DUI_MEN_INDEX],2)==1?true:false;
  bWinDaoMen=m_GameLogic.CompareCard(m_cbTableCardArray[BANKER_INDEX],2,m_cbTableCardArray[DAO_MEN_INDEX],2)==1?true:false;
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

//读取配置
void CTableFrameSink::ReadConfigInformation()
{
  //获取自定义配置
  tagCustomConfig *pCustomConfig = (tagCustomConfig *)m_pGameServiceOption->cbCustomRule;
  ASSERT(pCustomConfig);

  //上庄
  m_lApplyBankerCondition = pCustomConfig->CustomGeneral.lApplyBankerCondition;
  m_nBankerTimeLimit = pCustomConfig->CustomGeneral.lBankerTime;
  m_nBankerTimeAdd = pCustomConfig->CustomGeneral.lBankerTimeAdd;
  m_lExtraBankerScore = pCustomConfig->CustomGeneral.lBankerScoreMAX;
  m_nExtraBankerTime = pCustomConfig->CustomGeneral.lBankerTimeExtra;
  m_bEnableSysBanker = (pCustomConfig->CustomGeneral.nEnableSysBanker == TRUE)?true:false;

  //时间
  m_nFreeTime = pCustomConfig->CustomGeneral.lFreeTime;
  m_nPlaceJettonTime = pCustomConfig->CustomGeneral.lBetTime;
  m_nGameEndTime = pCustomConfig->CustomGeneral.lEndTime;
  if(m_nFreeTime < TIME_FREE  || m_nFreeTime > 99)
  {
    m_nFreeTime = TIME_FREE;
  }
  if(m_nPlaceJettonTime < TIME_PLACE_JETTON || m_nPlaceJettonTime > 99)
  {
    m_nPlaceJettonTime = TIME_PLACE_JETTON;
  }
  if(m_nGameEndTime < TIME_GAME_END || m_nGameEndTime > 99)
  {
    m_nGameEndTime = TIME_GAME_END;
  }

  //下注
  m_lAreaLimitScore = pCustomConfig->CustomGeneral.lAreaLimitScore;
  m_lUserLimitScore = pCustomConfig->CustomGeneral.lUserLimitScore;
  m_nEndGameMul = pCustomConfig->CustomGeneral.lEndGameMul;
  if(m_nEndGameMul < 1 || m_nEndGameMul > 100)
  {
    m_nEndGameMul = 80;
  }

  //库存
  m_lStorageStart = pCustomConfig->CustomGeneral.StorageStart;
  m_lStorageCurrent = m_lStorageStart;
  m_nStorageDeduct = pCustomConfig->CustomGeneral.StorageDeduct;
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

  return;
}

// 库存优化
VOID CTableFrameSink::StorageOptimize()
{
  // 获取系统输赢
  LONGLONG lSystemScore = 0;
  bool bAllEat = false;
  JudgeSystemScore(m_cbTableCardArray, lSystemScore, bAllEat);

  // 系统输钱
  if(lSystemScore < 0)
  {
    // 够钱
    if(m_lStorageCurrent > -lSystemScore)
    {
      return;
    }

    // 不够钱, 让系统赢钱

    // 交换后系统输赢
    LONGLONG lExchangeMoney[3] = { 0, 0, 0 };
    bool bAllEat[3] = { true, true, true };

    // 庄家牌
    BYTE bBankerCardData[2] = {0};
    CopyMemory(bBankerCardData, m_cbTableCardArray[0], sizeof(bBankerCardData));

    // 交换牌
    for(int nExchange = 0; nExchange < 3; ++nExchange)
    {
      BYTE bExchangeData[4][2] = {0};
      CopyMemory(bExchangeData, m_cbTableCardArray, sizeof(bExchangeData));
      CopyMemory(bExchangeData[0], m_cbTableCardArray[nExchange + 1], sizeof(BYTE) * 2);
      CopyMemory(bExchangeData[nExchange + 1], bBankerCardData, sizeof(BYTE) * 2);

      JudgeSystemScore(bExchangeData, lExchangeMoney[nExchange], bAllEat[nExchange]);
    }

    // 获取系统是赢
    int nOkInedex = -1;
    for(int nOk = 0; nOk < 3; ++nOk)
    {
      if(lExchangeMoney[nOk] > 0)
      {
        nOkInedex = nOk;

        // 如果这个不是通吃， 直接返回
        if(!bAllEat[nOk])
        {
          break;
        }
      }
    }

    // 查找到队列
    if(nOkInedex != -1)
    {
      //交换手牌
      BYTE bTempCardData[2] = {0};
      CopyMemory(bTempCardData,         m_cbTableCardArray[0],        sizeof(bTempCardData));
      CopyMemory(m_cbTableCardArray[0],     m_cbTableCardArray[nOkInedex + 1],    sizeof(bTempCardData));
      CopyMemory(m_cbTableCardArray[nOkInedex + 1], bTempCardData,            sizeof(bTempCardData));
    }

    return;
  }
  // 系统赢钱
  else if(lSystemScore > 0)
  {
    // 如果现在系统大于库存最大值， 那就让玩家赢钱
    if((m_lStorageCurrent > m_lStorageMax1 && m_lStorageCurrent <= m_lStorageMax2 && rand()%100 <= m_lStorageMul1) ||
       (m_lStorageCurrent > m_lStorageMax2 && rand()%100 <= m_lStorageMul2))
    {
      // 交换后系统输赢
      LONGLONG lExchangeMoney[3] = { 0, 0, 0 };
      bool bAllEat[3] = { true, true, true };

      // 庄家牌
      BYTE bBankerCardData[2] = {0};
      CopyMemory(bBankerCardData, m_cbTableCardArray[0], sizeof(bBankerCardData));

      // 交换牌
      for(int nExchange = 0; nExchange < 3; ++nExchange)
      {
        BYTE bExchangeData[4][2] = {0};
        CopyMemory(bExchangeData, m_cbTableCardArray, sizeof(bExchangeData));
        CopyMemory(bExchangeData[0], m_cbTableCardArray[nExchange + 1], sizeof(BYTE) * 2);
        CopyMemory(bExchangeData[nExchange + 1], bBankerCardData, sizeof(BYTE) * 2);

        JudgeSystemScore(bExchangeData, lExchangeMoney[nExchange], bAllEat[nExchange]);
      }

      // 获取系统是输，但是输的最少的牌
      int nOkInedex = -1;
      LONGLONG nOkMonye = LLONG_MIN;
      for(int nOk = 0; nOk < 3; ++nOk)
      {
        if(lExchangeMoney[nOk] < 0 && nOkMonye < lExchangeMoney[nOk] && (-lExchangeMoney[nOk]) < m_lStorageCurrent)
        {
          nOkMonye = lExchangeMoney[nOk];
          nOkInedex = nOk;

          // 如果这个不是通吃， 直接返回
          if(!bAllEat[nOk])
          {
            break;
          }
        }
      }

      // 查找到队列
      if(nOkInedex != -1)
      {
        //交换手牌
        BYTE bTempCardData[2] = {0};
        CopyMemory(bTempCardData,         m_cbTableCardArray[0],        sizeof(bTempCardData));
        CopyMemory(m_cbTableCardArray[0],     m_cbTableCardArray[nOkInedex + 1],    sizeof(bTempCardData));
        CopyMemory(m_cbTableCardArray[nOkInedex + 1], bTempCardData,            sizeof(bTempCardData));
      }
    }
    return;
  }
}

// 判断系统得分
void CTableFrameSink::JudgeSystemScore(BYTE bCardData[4][2], LONGLONG& lSystemScore, bool& bAllEat)
{
  // 系统输赢
  bAllEat = true;
  lSystemScore = 0l;

  //系统坐庄
  bool bSystemBanker = false;
  if(m_wCurrentBanker == INVALID_CHAIR)
  {
    bSystemBanker = true;
  }
  else
  {
    IServerUserItem *pServerUserItem = m_pITableFrame->GetTableUserItem(m_wCurrentBanker);
    if(pServerUserItem != NULL)
    {
      bSystemBanker = pServerUserItem->IsAndroidUser();
    }
  }

  //大小比较
  bool bWinShunMen = m_GameLogic.CompareCard(bCardData[BANKER_INDEX],2,bCardData[SHUN_MEN_INDEX],2)==1?true:false;
  bool bWinDuiMen = m_GameLogic.CompareCard(bCardData[BANKER_INDEX],2,bCardData[DUI_MEN_INDEX],2)==1?true:false;
  bool bWinDaoMen = m_GameLogic.CompareCard(bCardData[BANKER_INDEX],2,bCardData[DAO_MEN_INDEX],2)==1?true:false;

  //通吃
  bAllEat = (bWinShunMen == bWinDuiMen) && (bWinDuiMen == bWinDaoMen);

  //玩家下注
  LONGLONG *const pUserScore[] = {NULL, m_lUserJettonScore[ID_SHUN_MEN], m_lUserJettonScore[ID_JIAO_L], m_lUserJettonScore[ID_QIAO],
                                  m_lUserJettonScore[ID_DUI_MEN], m_lUserJettonScore[ID_DAO_MEN], m_lUserJettonScore[ID_JIAO_R]
                                 };

  //区域倍率
  BYTE static const cbMultiple[] = {0, 1, 1, 1, 1, 1, 1};

  //胜利标识
  bool static bWinFlag[AREA_COUNT+1];
  bWinFlag[0] = false;
  bWinFlag[ID_SHUN_MEN] = bWinShunMen;
  bWinFlag[ID_JIAO_L] = bWinShunMen && bWinDuiMen;
  bWinFlag[ID_QIAO] = bWinShunMen && bWinDaoMen;
  bWinFlag[ID_DUI_MEN] = bWinDuiMen;
  bWinFlag[ID_DAO_MEN] = bWinDaoMen;
  bWinFlag[ID_JIAO_R] = bWinDaoMen && bWinDuiMen;

  //角标识
  bool static bWinBankerJiaoL,bWinBankerJiaoR,bWinBankerQiao;
  bWinBankerJiaoL = !bWinShunMen && !bWinDuiMen;
  bWinBankerJiaoR = !bWinDaoMen && !bWinDuiMen;
  bWinBankerQiao = !bWinShunMen && !bWinDaoMen;

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

    for(WORD wAreaIndex = ID_SHUN_MEN; wAreaIndex <= ID_JIAO_R; ++wAreaIndex)
    {
      //角判断
      bool bReturnScore=false;
      if(ID_JIAO_L==wAreaIndex && false==bWinBankerJiaoL)
      {
        bReturnScore=true;
      }
      if(ID_JIAO_R==wAreaIndex && false==bWinBankerJiaoR)
      {
        bReturnScore=true;
      }
      if(ID_QIAO==wAreaIndex && false==bWinBankerQiao)
      {
        bReturnScore=true;
      }

      if(bWinFlag[wAreaIndex])
      {
        if(pIServerUserItem->IsAndroidUser() && !bSystemBanker)
        {
          lSystemScore += (pUserScore[wAreaIndex][wChairID] * cbMultiple[wAreaIndex]) ;
        }
        if(!pIServerUserItem->IsAndroidUser() && bSystemBanker)
        {
          lSystemScore -= (pUserScore[wAreaIndex][wChairID] * cbMultiple[wAreaIndex]) ;
        }
      }
      else if(bReturnScore)
      {

      }
      else
      {
        if(pIServerUserItem->IsAndroidUser() && !bSystemBanker)
        {
          lSystemScore -= pUserScore[wAreaIndex][wChairID];
        }
        if(!pIServerUserItem->IsAndroidUser() && bSystemBanker)
        {
          lSystemScore += pUserScore[wAreaIndex][wChairID];
        }
      }
    }
  }

  return ;
}



//查询是否扣服务费
bool CTableFrameSink::QueryBuckleServiceCharge(WORD wChairID)
{
  //返回下注
  for(int nAreaIndex=0; nAreaIndex<=AREA_COUNT; ++nAreaIndex)
  {

    if(m_lUserJettonScore[nAreaIndex][wChairID] != 0)
    {
      return true;
    }
  }
  if(wChairID==m_wCurrentBanker)
  {
    return true;
  }
  return false;
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

//发送下注信息
void CTableFrameSink::SendUserBetInfo(IServerUserItem *pIServerUserItem)
{
  if(NULL == pIServerUserItem)
  {
    return;
  }

  //权限判断
  if(!CUserRight::IsGameCheatUser(pIServerUserItem->GetUserRight()))
  {
    return;
  }

  //构造数据
  CMD_S_SendUserBetInfo SendUserBetInfo;
  ZeroMemory(&SendUserBetInfo, sizeof(SendUserBetInfo));

  CopyMemory(&SendUserBetInfo.lUserStartScore, m_lUserStartScore, sizeof(m_lUserStartScore));
  CopyMemory(&SendUserBetInfo.lUserJettonScore, m_lUserJettonScore, sizeof(m_lUserJettonScore));

  //发送消息
  m_pITableFrame->SendUserItemData(pIServerUserItem, SUB_S_SEND_USER_BET_INFO, &SendUserBetInfo, sizeof(SendUserBetInfo));

  return;
}

//测试写信息
void CTableFrameSink::WriteInfo(LPCTSTR pszFileName, LPCTSTR pszString)
{
  //设置语言区域
  char* old_locale = _strdup(setlocale(LC_CTYPE,NULL));
  setlocale(LC_CTYPE, "chs");

  CStdioFile myFile;
  CString strFileName;
  strFileName.Format(TEXT("%s"), pszFileName);
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
//////////////////////////////////////////////////////////////////////////


