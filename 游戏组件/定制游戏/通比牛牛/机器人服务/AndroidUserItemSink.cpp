#include "Stdafx.h"
#include "AndroidUserItemSink.h"

//////////////////////////////////////////////////////////////////////////

//辅助时间
#define TIME_LESS           2                 //最少时间

//游戏时间
#define TIME_USER_START_GAME      2                 //开始时间
#define TIME_USER_OPEN_CARD       4                 //摊牌时间

//游戏时间
#define IDI_START_GAME          (100)               //开始定时器
#define IDI_OPEN_CARD         (101)               //开牌定时器
#define IDI_DELAY_TIME          (102)               //延时定时器

//////////////////////////////////////////////////////////////////////////

//构造函数
CAndroidUserItemSink::CAndroidUserItemSink()
{
  m_nRobotBankStorageMul=0;
  m_lRobotBankGetScore=0;
  m_lRobotBankGetScoreBanker=0;
  ZeroMemory(m_lRobotScoreRange,sizeof(m_lRobotScoreRange));
  ZeroMemory(m_HandCardData,sizeof(m_HandCardData));

  //接口变量
  m_pIAndroidUserItem=NULL;
  srand((unsigned)time(NULL));

  return;
}

//析构函数
CAndroidUserItemSink::~CAndroidUserItemSink()
{
}

//接口查询
void * CAndroidUserItemSink::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
  QUERYINTERFACE(IAndroidUserItemSink,Guid,dwQueryVer);
  QUERYINTERFACE_IUNKNOWNEX(IAndroidUserItemSink,Guid,dwQueryVer);

  return NULL;
}

//初始接口
bool CAndroidUserItemSink::Initialization(IUnknownEx * pIUnknownEx)
{
  //查询接口
  m_pIAndroidUserItem=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,IAndroidUserItem);
  if(m_pIAndroidUserItem==NULL)
  {
    return false;
  }

  return true;
}

//重置接口
bool CAndroidUserItemSink::RepositionSink()
{
  ZeroMemory(m_HandCardData,sizeof(m_HandCardData));

  return true;
}

//时间消息
bool CAndroidUserItemSink::OnEventTimer(UINT nTimerID)
{
  try
  {
    switch(nTimerID)
    {
      case IDI_DELAY_TIME:
      {
        UINT nElapse = TIME_LESS + (rand() % TIME_USER_START_GAME);
        m_pIAndroidUserItem->SetGameTimer(IDI_START_GAME,nElapse);

        return true;
      }
      case IDI_START_GAME:    //开始定时器
      {
        //发送准备
        m_pIAndroidUserItem->SendUserReady(NULL,0);

        return true;
      }
      case IDI_OPEN_CARD:     //开牌定时
      {
        //发送消息
        CMD_C_OxCard OxCard;
        if(m_GameLogic.GetOxCard(m_HandCardData, MAX_COUNT))
        {
          OxCard.bOX = true;
        }
        else
        {
          OxCard.bOX = false;
        }
        CopyMemory(OxCard.cbOxCardData, m_HandCardData, MAX_COUNT*sizeof(BYTE));
        m_pIAndroidUserItem->SendSocketData(SUB_C_OPEN_CARD, &OxCard, sizeof(OxCard));

        return true;
      }
    }
  }
  catch(...)
  {
    CString cs;
    cs.Format(TEXT("异常ID=%d"),nTimerID);
    CTraceService::TraceString(cs,TraceLevel_Debug);
  }

  return false;
}

//游戏消息
bool CAndroidUserItemSink::OnEventGameMessage(WORD wSubCmdID, void * pData, WORD wDataSize)
{
  switch(wSubCmdID)
  {
    case SUB_S_GAME_START:  //游戏开始
    {
      //消息处理
      return OnSubGameStart(pData,wDataSize);
    }
    case SUB_S_PLAYER_EXIT: //用户强退
    {
      //消息处理
      return OnSubPlayerExit(pData,wDataSize);
    }
    case SUB_S_SEND_CARD: //发牌消息
    {
      //消息处理
      return OnSubSendCard(pData,wDataSize);
    }
    case SUB_S_GAME_END:  //游戏结束
    {
      //消息处理
      return OnSubGameEnd(pData,wDataSize);
    }
    case SUB_S_OPEN_CARD: //用户摊牌
    {
      //消息处理
      return OnSubOpenCard(pData,wDataSize);
    }
    case SUB_S_ADMIN_STORAGE_INFO:
    {
      return true;
    }
  }

  //错误断言
  ASSERT(FALSE);

  return true;
}

//游戏消息
bool CAndroidUserItemSink::OnEventFrameMessage(WORD wSubCmdID, void * pData, WORD wDataSize)
{
  return true;
}

//场景消息
bool CAndroidUserItemSink::OnEventSceneMessage(BYTE cbGameStatus, bool bLookonOther, void * pData, WORD wDataSize)
{
  switch(cbGameStatus)
  {
    case GAME_STATUS_FREE:    //空闲状态
    {
      //效验数据
      if(wDataSize!=sizeof(CMD_S_StatusFree))
      {
        return false;
      }

      //消息处理
      CMD_S_StatusFree * pStatusFree=(CMD_S_StatusFree *)pData;

      memcpy(m_szRoomName, pStatusFree->szGameRoomName, sizeof(m_szRoomName));

      ReadConfigInformation(&(pStatusFree->RobotConfig));
      BankOperate(2);

      //开始时间
      UINT nElapse = TIME_LESS + (rand() % TIME_USER_START_GAME);
      m_pIAndroidUserItem->SetGameTimer(IDI_START_GAME,nElapse);

      return true;
    }
    case GS_TK_PLAYING: //游戏状态
    {
      //效验数据
      if(wDataSize!=sizeof(CMD_S_StatusPlay))
      {
        return false;
      }
      CMD_S_StatusPlay * pStatusPlay=(CMD_S_StatusPlay *)pData;

      memcpy(m_szRoomName, pStatusPlay->szGameRoomName, sizeof(m_szRoomName));

      ReadConfigInformation(&(pStatusPlay->RobotConfig));
      BankOperate(2);

      //设置变量
      WORD wMeChiarID=m_pIAndroidUserItem->GetChairID();
      CopyMemory(m_HandCardData,pStatusPlay->cbHandCardData[wMeChiarID],MAX_COUNT);

      //控件处理
      if(pStatusPlay->bOxCard[wMeChiarID]==0xFF)
      {
        //开牌时间
        UINT nElapse = TIME_LESS + (rand() % TIME_USER_OPEN_CARD);
        m_pIAndroidUserItem->SetGameTimer(IDI_OPEN_CARD,nElapse);
      }

      return true;
    }
  }

  ASSERT(FALSE);

  return false;
}

//用户进入
void CAndroidUserItemSink::OnEventUserEnter(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
  return;
}

//用户离开
void CAndroidUserItemSink::OnEventUserLeave(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
  return;
}

//用户积分
void CAndroidUserItemSink::OnEventUserScore(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
  return;
}

//用户状态
void CAndroidUserItemSink::OnEventUserStatus(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
  return;
}

//用户段位
void CAndroidUserItemSink::OnEventUserSegment(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
  return;
}

//游戏开始
bool CAndroidUserItemSink::OnSubGameStart(const void * pBuffer, WORD wDataSize)
{
  //效验数据
  if(wDataSize!=sizeof(CMD_S_GameStart))
  {
    return false;
  }
  CMD_S_GameStart * pGameStart=(CMD_S_GameStart *)pBuffer;

  //设置数据
  WORD wMeChiarID=m_pIAndroidUserItem->GetChairID();
  CopyMemory(m_HandCardData,pGameStart->cbCardData[wMeChiarID],sizeof(m_HandCardData));

  //下注时间
  UINT nElapse = TIME_LESS + (rand() % TIME_USER_OPEN_CARD);
  m_pIAndroidUserItem->SetGameTimer(IDI_OPEN_CARD,nElapse);

  return true;
}

//发牌消息
bool CAndroidUserItemSink::OnSubSendCard(const void * pBuffer, WORD wDataSize)
{
  //效验数据
  if(wDataSize!=sizeof(CMD_S_SendCard))
  {
    return false;
  }
  CMD_S_SendCard * pSendCard=(CMD_S_SendCard *)pBuffer;

  //设置数据
  WORD wMeChiarID=m_pIAndroidUserItem->GetChairID();
  CopyMemory(m_HandCardData,pSendCard->cbCardData[wMeChiarID],sizeof(m_HandCardData));

  //开牌时间
  UINT nElapse = TIME_LESS + (rand() % TIME_USER_OPEN_CARD);
  m_pIAndroidUserItem->SetGameTimer(IDI_OPEN_CARD,nElapse);

  return true;
}

//用户摊牌
bool CAndroidUserItemSink::OnSubOpenCard(const void * pBuffer, WORD wDataSize)
{
  //效验数据
  if(wDataSize!=sizeof(CMD_S_Open_Card))
  {
    return false;
  }
  CMD_S_Open_Card * pOpenCard=(CMD_S_Open_Card *)pBuffer;

  return true;
}

//用户强退
bool CAndroidUserItemSink::OnSubPlayerExit(const void * pBuffer, WORD wDataSize)
{
  //效验数据
  if(wDataSize!=sizeof(CMD_S_PlayerExit))
  {
    return false;
  }
  CMD_S_PlayerExit * pPlayerExit=(CMD_S_PlayerExit *)pBuffer;

  return true;
}

//游戏结束
bool CAndroidUserItemSink::OnSubGameEnd(const void * pBuffer, WORD wDataSize)
{
  //效验参数
  if(wDataSize!=sizeof(CMD_S_GameEnd))
  {
    return false;
  }
  CMD_S_GameEnd * pGameEnd=(CMD_S_GameEnd *)pBuffer;

  //删除定时器
  m_pIAndroidUserItem->KillGameTimer(IDI_OPEN_CARD);

  //清理变量
  ZeroMemory(m_HandCardData,sizeof(m_HandCardData));

  //开始时间
  if(pGameEnd->cbDelayOverGame==0)
  {
    UINT nElapse = TIME_LESS + (rand() % TIME_USER_START_GAME);
    m_pIAndroidUserItem->SetGameTimer(IDI_START_GAME,nElapse);
  }
  else
  {
    UINT nElapse=pGameEnd->cbDelayOverGame;
    m_pIAndroidUserItem->SetGameTimer(IDI_DELAY_TIME,nElapse);
  }

  return true;
}

//银行操作
void CAndroidUserItemSink::BankOperate(BYTE cbType)
{
  IServerUserItem *pUserItem = m_pIAndroidUserItem->GetMeUserItem();
  if(pUserItem->GetUserStatus() >= US_SIT)
  {
    if(cbType == 1)
    {
      CString strInfo;
      strInfo.Format(TEXT("大厅：状态不对，不执行存取款"));
      NcaTextOut(strInfo);
      return;
    }
  }

  //变量定义
  LONGLONG lRobotScore = pUserItem->GetUserScore();

  //CString strInfo;
  //strInfo.Format(TEXT("[%s] 分数(%I64d)"),pUserItem->GetNickName(),lRobotScore);

  //if (lRobotScore > m_lRobotScoreRange[1])
  //{
  //  CString strInfo1;
  //  strInfo1.Format(TEXT("满足存款条件(%I64d)"),m_lRobotScoreRange[1]);
  //  strInfo+=strInfo1;
  //  NcaTextOut(strInfo);
  //}
  //else if (lRobotScore < m_lRobotScoreRange[0])
  //{
  //  CString strInfo1;
  //  strInfo1.Format(TEXT("满足取款条件(%I64d)"),m_lRobotScoreRange[0]);
  //  strInfo+=strInfo1;
  //  NcaTextOut(strInfo);
  //}

  //判断存取
  if(lRobotScore > m_lRobotScoreRange[1])
  {
    LONGLONG lSaveScore=0L;
    lSaveScore = LONGLONG(lRobotScore*m_nRobotBankStorageMul/100);
    if(lSaveScore > lRobotScore)
    {
      lSaveScore = lRobotScore;
    }
    if(lSaveScore > 0)
    {
      m_pIAndroidUserItem->PerformSaveScore(lSaveScore);
    }
    LONGLONG lRobotNewScore = pUserItem->GetUserScore();

    CString strInfo;
    strInfo.Format(TEXT("[%s] 执行存款：存款前金币(%I64d)，存款后金币(%I64d)"),pUserItem->GetNickName(),lRobotScore,lRobotNewScore);
    NcaTextOut(strInfo);

  }
  else if(lRobotScore < m_lRobotScoreRange[0])
  {
    CString strInfo;
    SCORE lScore=rand()%m_lRobotBankGetScoreBanker+m_lRobotBankGetScore;
    if(lScore > 0)
    {
      m_pIAndroidUserItem->PerformTakeScore(lScore);
    }

    LONGLONG lRobotNewScore = pUserItem->GetUserScore();
    strInfo.Format(TEXT("[%s] 执行取款：取款前金币(%I64d)，取款后金币(%I64d)"),pUserItem->GetNickName(),lRobotScore,lRobotNewScore);
    NcaTextOut(strInfo);
  }

  return;
}


//读取配置
void CAndroidUserItemSink::ReadConfigInformation(tagRobotConfig *pConfig)
{
  m_lRobotScoreRange[0] = pConfig->lRobotScoreMin;
  m_lRobotScoreRange[1] = pConfig->lRobotScoreMax;


  if(m_lRobotScoreRange[1] < m_lRobotScoreRange[0])
  {
    m_lRobotScoreRange[1] = m_lRobotScoreRange[0];
  }

  m_lRobotBankGetScore = pConfig->lRobotBankGet;

  m_lRobotBankGetScoreBanker = pConfig->lRobotBankGetBanker;

  m_nRobotBankStorageMul = pConfig->lRobotBankStoMul;


  if(m_nRobotBankStorageMul < 0 || m_nRobotBankStorageMul > 100)
  {
    m_nRobotBankStorageMul = 20;
  }
}


//组件创建函数
DECLARE_CREATE_MODULE(AndroidUserItemSink);

//////////////////////////////////////////////////////////////////////////
