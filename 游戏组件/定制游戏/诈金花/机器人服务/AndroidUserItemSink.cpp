#include "Stdafx.h"
#include "AndroidUserItemSink.h"

//////////////////////////////////////////////////////////////////////////

//辅助时间
#define TIME_LESS           2                 //最少时间
#define TIME_DELAY_TIME         3                 //延时时间

//游戏时间
#define TIME_START_GAME         4                 //开始时间
#define TIME_USER_ADD_SCORE       4                 //下注时间
#define TIME_USER_COMPARE_CARD      4                 //比牌时间
#define TIME_USER_FINISH_FLASH      4                 //比牌时间

//游戏时间
#define IDI_START_GAME          (100)               //开始定时器
#define IDI_USER_ADD_SCORE        (101)               //下注定时器
#define IDI_USER_COMPARE_CARD     (102)               //比牌定时器
#define IDI_DELAY_TIME          (103)               //延时定时器
#define IDI_USER_FINISH_FLASH     (104)               //动画定时器

//////////////////////////////////////////////////////////////////////////

//构造函数
CAndroidUserItemSink::CAndroidUserItemSink()
{
  //游戏变量
  m_lStockScore = 0;
  m_wBankerUser = INVALID_CHAIR;
  m_wCurrentUser = INVALID_CHAIR;
  m_cbCardType = 0;

  //加注信息
  m_bMingZhu = false;
  m_lCellScore = 0;
  m_lMaxCellScore = 0;
  m_lUserMaxScore = 0;
  m_lCurrentTimes = 0;
  ZeroMemory(m_lTableScore, sizeof(m_lTableScore));

  //库存操作
  m_nRobotBankStorageMul=0;
  m_lRobotBankGetScore=0;
  m_lRobotBankGetScoreBanker=0;
  ZeroMemory(m_lRobotScoreRange, sizeof(m_lRobotScoreRange));

  //游戏状态
  ZeroMemory(m_cbPlayStatus, sizeof(m_cbPlayStatus));
  ZeroMemory(m_cbRealPlayer, sizeof(m_cbRealPlayer));
  ZeroMemory(m_cbAndroidStatus, sizeof(m_cbAndroidStatus));

  //用户扑克
  ZeroMemory(m_cbHandCardData, sizeof(m_cbHandCardData));
  ZeroMemory(m_cbAllHandCardData, sizeof(m_cbAllHandCardData));

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
  //游戏变量
  m_lStockScore = 0;
  m_wBankerUser = INVALID_CHAIR;
  m_wCurrentUser = INVALID_CHAIR;
  m_cbCardType = 0;

  //加注信息
  m_bMingZhu = false;
  m_lCellScore = 0;
  m_lMaxCellScore = 0;
  m_lUserMaxScore = 0;
  m_lCurrentTimes = 0;
  ZeroMemory(m_lTableScore, sizeof(m_lTableScore));

  //游戏状态
  ZeroMemory(m_cbPlayStatus, sizeof(m_cbPlayStatus));
  ZeroMemory(m_cbRealPlayer, sizeof(m_cbRealPlayer));
  ZeroMemory(m_cbAndroidStatus, sizeof(m_cbAndroidStatus));

  //用户扑克
  ZeroMemory(m_cbHandCardData, sizeof(m_cbHandCardData));
  ZeroMemory(m_cbAllHandCardData, sizeof(m_cbAllHandCardData));

  return true;
}

//时间消息
bool CAndroidUserItemSink::OnEventTimer(UINT nTimerID)
{
  switch(nTimerID)
  {
    case IDI_START_GAME:    //开始定时器
    {
      //发送准备
      m_pIAndroidUserItem->SendUserReady(NULL, 0);

      return true;
    }
    case IDI_USER_ADD_SCORE:  //加注定时器
    {
      //删除定时器
      m_pIAndroidUserItem->KillGameTimer(IDI_USER_ADD_SCORE);

      //数据效验
      ASSERT(m_pIAndroidUserItem->GetChairID() == m_wCurrentUser);
      if(m_pIAndroidUserItem->GetChairID() != m_wCurrentUser)
      {
        return false;
      }

      //定义变量
      bool bGiveUp = false;
      BYTE cbProbability = 0;
      LONGLONG lCurrentScore = 0;
      LONGLONG lAndroidMaxScore = m_pIAndroidUserItem->GetMeUserItem()->GetUserScore();
      WORD wMeChairID = m_pIAndroidUserItem->GetChairID();

      //玩家人数
      BYTE cbPlayerCount = 0;
      for(WORD i=0; i<GAME_PLAYER; i++)
      {
        if(m_cbPlayStatus[i] == TRUE)
        {
          cbPlayerCount++;
        }
      }

      //是否真人
      if(m_cbRealPlayer[EstimateWinner()] == TRUE)
      {
        LONGLONG lTableScore = 0;
        for(WORD i = 0; i < GAME_PLAYER; i++)
        {
          if(m_cbAndroidStatus[i] == TRUE)
          {
            lTableScore += m_lTableScore[i];
          }
        }

        //是否输到底限
        if(lTableScore + m_lMaxCellScore > m_lStockScore)
        {
          bGiveUp = true;
        }
      }

      //首次操作
      if(m_lTableScore[wMeChairID] == m_lCellScore)
      {
        //随机概率
        cbProbability = rand()%(10);

        //看牌概率
        if(m_bMingZhu == false)
        {
          if(cbProbability < LOOK_CARD)
          {
            m_bMingZhu = true;
            m_pIAndroidUserItem->SendSocketData(SUB_C_LOOK_CARD, NULL, 0);

            return true;
          }
        }
        else
        {
          //散牌放弃
          if(m_cbCardType == 0 && wMeChairID != EstimateWinner())
          {
            m_pIAndroidUserItem->SendSocketData(SUB_C_GIVE_UP, NULL, 0);

            return true;
          }
          else if(m_cbCardType == CT_SINGLE && wMeChairID != EstimateWinner())
          {
            //放弃概率
            if(cbProbability < GIVE_UP)
            {
              m_pIAndroidUserItem->SendSocketData(SUB_C_GIVE_UP, NULL, 0);

              return true;
            }
          }
        }

        //庄家比牌
        if(m_wBankerUser == wMeChairID && m_bMingZhu == true)
        {
          if(m_cbCardType == 1 && cbProbability < COMPARE_CARD)
          {
            lCurrentScore = 4 * m_lCurrentTimes * m_lCellScore;

            //发送消息
            CMD_C_AddScore AddScore;
            AddScore.wState = TRUE;
            AddScore.lScore = lCurrentScore;
            m_pIAndroidUserItem->SendSocketData(SUB_C_ADD_SCORE, &AddScore, sizeof(AddScore));

            //构造变量
            CMD_C_CompareCard CompareCard;
            ZeroMemory(&CompareCard, sizeof(CompareCard));

            //查找上家
            WORD wTemp = 0xFFFF;
            for(WORD i = 1; i < GAME_PLAYER; i++)
            {
              //修改用户
              wTemp = (GAME_PLAYER + wMeChairID - i) % GAME_PLAYER;

              if(m_cbPlayStatus[wTemp] == TRUE && wTemp != wMeChairID)
              {
                CompareCard.wCompareUser = wTemp;
                break;
              }
            }

            //发送消息
            m_pIAndroidUserItem->SendSocketData(SUB_C_COMPARE_CARD, &CompareCard, sizeof(CompareCard));

            return true;
          }
        }

        //首轮加注
        ASSERT(m_lCurrentTimes <= 10 && m_lCurrentTimes >= 0);
        lCurrentScore = m_lCellScore * (m_lCurrentTimes + GetAddScoreTimes());

        //明注加倍
        if(m_bMingZhu == true)
        {
          lCurrentScore *= 2;
        }

        //发送消息
        CMD_C_AddScore AddScore;
        AddScore.wState = FALSE;
        AddScore.lScore = lCurrentScore;
        m_pIAndroidUserItem->SendSocketData(SUB_C_ADD_SCORE, &AddScore, sizeof(AddScore));

        return true;
      }
      else
      {
        //是否看牌
        if(m_bMingZhu == false)
        {
          //智能放弃
          if(bGiveUp == true)
          {
            m_pIAndroidUserItem->SendSocketData(SUB_C_GIVE_UP, NULL, 0);

            return true;
          }

          //随机概率
          cbProbability = rand()%(10);

          //看牌概率
          if(cbProbability < 2)
          {
            m_bMingZhu = true;
            m_pIAndroidUserItem->SendSocketData(SUB_C_LOOK_CARD, NULL, 0);

            return true;
          }

          //用户加注
          if(m_lTableScore[wMeChairID] < 2 * m_lMaxCellScore && (4 * cbPlayerCount * m_lMaxCellScore + m_lCellScore + m_lTableScore[wMeChairID]) < lAndroidMaxScore)
          {
            //加注积分
            ASSERT(m_lCurrentTimes <= 10 && m_lCurrentTimes >= 0);
            lCurrentScore = m_lCellScore * (m_lCurrentTimes + GetAddScoreTimes());

            //发送消息
            CMD_C_AddScore AddScore;
            AddScore.wState = FALSE;
            AddScore.lScore = lCurrentScore;
            m_pIAndroidUserItem->SendSocketData(SUB_C_ADD_SCORE, &AddScore, sizeof(AddScore));

            return true;
          }
          //比牌概率
          else
          {
            //加注积分
            lCurrentScore = m_lCurrentTimes * m_lCellScore * 2;

            //发送消息
            CMD_C_AddScore AddScore;
            AddScore.wState = TRUE;
            AddScore.lScore = lCurrentScore;
            m_pIAndroidUserItem->SendSocketData(SUB_C_ADD_SCORE, &AddScore, sizeof(AddScore));

            //构造消息
            CMD_C_CompareCard CompareCard;
            ZeroMemory(&CompareCard, sizeof(CompareCard));

            //判断人数
            if(cbPlayerCount > 2)
            {
              m_pIAndroidUserItem->SendSocketData(SUB_C_WAIT_COMPARE, &CompareCard, sizeof(CompareCard));
              //设置定时器
              UINT nElapse = TIME_LESS + (rand() % TIME_USER_COMPARE_CARD);
              m_pIAndroidUserItem->SetGameTimer(IDI_USER_COMPARE_CARD, nElapse);
            }
            else
            {
              //查找玩家
              for(WORD i = 0; i < GAME_PLAYER; i++)
              {
                if(m_cbPlayStatus[i] == TRUE && i != wMeChairID)
                {
                  CompareCard.wCompareUser = i;
                  break;
                }
              }
              m_pIAndroidUserItem->SendSocketData(SUB_C_COMPARE_CARD, &CompareCard, sizeof(CompareCard));
            }

            return true;
          }
        }
        else
        {
          //智能放弃
          if(bGiveUp == true)
          {
            m_pIAndroidUserItem->SendSocketData(SUB_C_GIVE_UP, NULL, 0);

            return true;
          }

          //随机概率
          cbProbability = rand()%(10);

          //散牌放弃
          if(m_cbCardType == 0 && wMeChairID != EstimateWinner())
          {
            m_pIAndroidUserItem->SendSocketData(SUB_C_GIVE_UP, NULL, 0);

            return true;
          }
          else if(m_cbCardType == CT_SINGLE && wMeChairID != EstimateWinner())
          {
            //放弃概率
            if(cbProbability < GIVE_UP)
            {
              m_pIAndroidUserItem->SendSocketData(SUB_C_GIVE_UP, NULL, 0);

              return true;
            }
          }

          //加注概率
          if(m_cbCardType < CT_JIN_HUA && m_lTableScore[wMeChairID] < (1 * 2 * m_lMaxCellScore * m_cbCardType)
             || m_cbCardType == CT_JIN_HUA && m_lTableScore[wMeChairID] < (3 * 2 * m_lMaxCellScore * m_cbCardType)
             || m_cbCardType > CT_JIN_HUA && m_lTableScore[wMeChairID] < (5 * 2 * m_lMaxCellScore * m_cbCardType)
             && (4 * cbPlayerCount * m_lMaxCellScore + m_lCellScore + m_lTableScore[wMeChairID]) < lAndroidMaxScore)
          {
            //下注积分
            ASSERT(m_lCurrentTimes <= 10 && m_lCurrentTimes >= 0);
            lCurrentScore = m_lCellScore * (m_lCurrentTimes + GetAddScoreTimes());

            //明注加倍
            if(m_bMingZhu == true)
            {
              lCurrentScore *= 2;
            }

            //发送消息
            CMD_C_AddScore AddScore;
            AddScore.wState = FALSE;
            AddScore.lScore = lCurrentScore;
            m_pIAndroidUserItem->SendSocketData(SUB_C_ADD_SCORE, &AddScore, sizeof(AddScore));

            return true;
          }
          //用户比牌
          else
          {
            //加注积分
            lCurrentScore = 4 * m_lCurrentTimes * m_lCellScore;

            //发送消息
            CMD_C_AddScore AddScore;
            AddScore.wState = TRUE;
            AddScore.lScore = lCurrentScore;
            m_pIAndroidUserItem->SendSocketData(SUB_C_ADD_SCORE, &AddScore, sizeof(AddScore));

            //构造消息
            CMD_C_CompareCard CompareCard;
            ZeroMemory(&CompareCard, sizeof(CompareCard));

            //判断人数
            if(cbPlayerCount > 2)
            {
              m_pIAndroidUserItem->SendSocketData(SUB_C_WAIT_COMPARE, &CompareCard, sizeof(CompareCard));
              //设置定时器
              UINT nElapse = TIME_LESS + (rand() % TIME_USER_COMPARE_CARD);
              m_pIAndroidUserItem->SetGameTimer(IDI_USER_COMPARE_CARD, nElapse);
            }
            else
            {
              //查找玩家
              for(WORD i = 0; i < GAME_PLAYER; i++)
              {
                if(m_cbPlayStatus[i] == TRUE && i != wMeChairID)
                {
                  CompareCard.wCompareUser = i;
                  break;
                }
              }
              m_pIAndroidUserItem->SendSocketData(SUB_C_COMPARE_CARD, &CompareCard, sizeof(CompareCard));
            }

            return true;
          }
        }
      }

      return false;
    }
    case IDI_USER_COMPARE_CARD:   //比牌定时器
    {
      //定义变量
      WORD wCompareUser = INVALID_CHAIR;
      WORD wMeChairID = m_pIAndroidUserItem->GetChairID();

      //构造消息
      CMD_C_CompareCard CompareCard;

      //查找玩家
      while(true)
      {
        wCompareUser = rand()%GAME_PLAYER;
        if(wCompareUser != wMeChairID && m_cbPlayStatus[wCompareUser] == TRUE)
        {
          CompareCard.wCompareUser = wCompareUser;
          break;
        }
      }

      //发送消息
      m_pIAndroidUserItem->SendSocketData(SUB_C_COMPARE_CARD, &CompareCard, sizeof(CompareCard));

      return true;
    }
    case IDI_DELAY_TIME:    //延时定时器
    {
      //删除定时器
      m_pIAndroidUserItem->KillGameTimer(IDI_DELAY_TIME);

      //设置定时器
      UINT nElapse=TIME_LESS + (rand() % TIME_START_GAME);
      m_pIAndroidUserItem->SetGameTimer(IDI_START_GAME, nElapse);
    }
    case IDI_USER_FINISH_FLASH:
    {
      m_pIAndroidUserItem->SendSocketData(SUB_C_FINISH_FLASH, NULL, 0);
      return true;
    }
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
      return OnSubGameStart(pData, wDataSize);
    }
    case SUB_S_ADD_SCORE: //用户下注
    {
      //消息处理
      return OnSubAddScore(pData, wDataSize);
    }
    case SUB_S_GIVE_UP:   //用户放弃
    {
      //消息处理
      return OnSubGiveUp(pData, wDataSize);
    }
    case SUB_S_SEND_CARD: //发牌消息
    {
      //消息处理
      return true;
    }
    case SUB_S_GAME_END:  //游戏结束
    {
      //消息处理
      return OnSubGameEnd(pData, wDataSize);
    }
    case SUB_S_COMPARE_CARD://用户比牌
    {
      //消息处理
      return OnSubCompareCard(pData, wDataSize);
    }
    case SUB_S_LOOK_CARD: //用户看牌
    {
      //消息处理
      return OnSubLookCard(pData, wDataSize);
    }
    case SUB_S_PLAYER_EXIT: //用户强退
    {
      //消息处理
      return OnSubPlayerExit(pData, wDataSize);
    }
    case SUB_S_OPEN_CARD: //用户摊牌
    {
      //消息处理
      return OnSubOpenCard(pData, wDataSize);
    }
    case SUB_S_WAIT_COMPARE://等待比牌
    {
      //消息处理
      if(wDataSize != sizeof(CMD_S_WaitCompare))
      {
        return false;
      }
      CMD_S_WaitCompare * pWaitCompare = (CMD_S_WaitCompare *)pData;

      //用户效验
      ASSERT(pWaitCompare->wCompareUser == m_wCurrentUser);

      ////重新定时
      //if (m_pIAndroidUserItem->GetChairID() == m_wCurrentUser)
      //{
      //  //设置定时器
      //  UINT nElapse = TIME_LESS + (rand() % TIME_USER_COMPARE_CARD);
      //  m_pIAndroidUserItem->SetGameTimer(IDI_USER_COMPARE_CARD, nElapse);
      //}
      return true;
    }
    case SUB_S_ANDROID_CARD:
    {
      return OnSubAndroidCard(pData, wDataSize);
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
      CMD_S_StatusFree * pStatusFree = (CMD_S_StatusFree *)pData;

      //获取房间
      CopyMemory(m_szRoomName, pStatusFree->szGameRoomName, sizeof(m_szRoomName));

      //读取配置
      ReadConfigInformation(&(pStatusFree->RobotConfig));

      //银行操作
      BankOperate(2);

      //设置定时器
      UINT nElapse=TIME_LESS + (rand() % TIME_START_GAME);
      m_pIAndroidUserItem->SetGameTimer(IDI_START_GAME, nElapse);

      return true;
    }
    case GAME_STATUS_PLAY:    //游戏状态
    {
      //效验数据
      if(wDataSize!=sizeof(CMD_S_StatusPlay))
      {
        return false;
      }
      CMD_S_StatusPlay * pStatusPlay = (CMD_S_StatusPlay *)pData;

      //获取房间
      CopyMemory(m_szRoomName, pStatusPlay->szGameRoomName, sizeof(m_szRoomName));

      //读取配置
      ReadConfigInformation(&(pStatusPlay->RobotConfig));

      //银行操作
      BankOperate(2);

      //用户信息
      m_wBankerUser = pStatusPlay->wBankerUser;
      m_wCurrentUser = pStatusPlay->wCurrentUser;

      //加注信息
      m_lCellScore = pStatusPlay->lCellScore;
      m_lMaxCellScore = pStatusPlay->lMaxCellScore;
      m_lCurrentTimes = pStatusPlay->lCurrentTimes;
      m_lUserMaxScore = pStatusPlay->lUserMaxScore;
      CopyMemory(m_lTableScore, pStatusPlay->lTableScore, sizeof(m_lTableScore));
      m_bMingZhu = pStatusPlay->bMingZhu[m_pIAndroidUserItem->GetChairID()];

      //游戏状态
      CopyMemory(m_cbPlayStatus, pStatusPlay->cbPlayStatus, sizeof(m_cbPlayStatus));

      //用户扑克
      CopyMemory(m_cbHandCardData, pStatusPlay->cbHandCardData, sizeof(m_cbHandCardData));

      //判断用户
      if(m_pIAndroidUserItem->GetChairID() == m_wCurrentUser)
      {
        //判断状态
        if(pStatusPlay->bCompareState == true)
        {
          //等待比牌
          m_pIAndroidUserItem->SendSocketData(SUB_C_WAIT_COMPARE, NULL, 0);

          //设置定时器
          UINT nElapse = TIME_LESS + (rand() % TIME_USER_COMPARE_CARD);
          m_pIAndroidUserItem->SetGameTimer(IDI_USER_COMPARE_CARD, nElapse);
        }
        else
        {
          //设置定时器
          UINT nElapse = TIME_LESS + (rand() % TIME_USER_ADD_SCORE);
          m_pIAndroidUserItem->SetGameTimer(IDI_USER_ADD_SCORE, nElapse);
        }
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

//机器人消息
bool CAndroidUserItemSink::OnSubAndroidCard(const void * pBuffer, WORD wDataSize)
{
  //效验参数
  ASSERT(wDataSize == sizeof(CMD_S_AndroidCard));
  if(wDataSize != sizeof(CMD_S_AndroidCard))
  {
    return false;
  }

  //变量定义
  CMD_S_AndroidCard * pAndroidCard = (CMD_S_AndroidCard *)pBuffer;

  //设置变量
  m_lStockScore = pAndroidCard->lStockScore;
  CopyMemory(m_cbRealPlayer, pAndroidCard->cbRealPlayer, sizeof(m_cbRealPlayer));
  CopyMemory(m_cbAllHandCardData, pAndroidCard->cbAllHandCardData, sizeof(m_cbAllHandCardData));
  CopyMemory(m_cbAndroidStatus, pAndroidCard->cbAndroidStatus, sizeof(m_cbAndroidStatus));

  return true;
}

//游戏开始
bool CAndroidUserItemSink::OnSubGameStart(const void * pBuffer, WORD wDataSize)
{
  //效验参数
  ASSERT(wDataSize==sizeof(CMD_S_GameStart));
  if(wDataSize!=sizeof(CMD_S_GameStart))
  {
    return false;
  }

  //变量定义
  CMD_S_GameStart * pGameStart=(CMD_S_GameStart *)pBuffer;

  //用户信息
  m_wBankerUser = pGameStart->wBankerUser;
  m_wCurrentUser = pGameStart->wCurrentUser;

  //加注信息
  m_bMingZhu = false;
  m_lCellScore = pGameStart->lCellScore;
  m_lMaxCellScore = pGameStart->lMaxScore;
  m_lUserMaxScore = pGameStart->lUserMaxScore;
  m_lCurrentTimes = pGameStart->lCurrentTimes;
  CopyMemory(m_cbPlayStatus, pGameStart->cbPlayStatus, sizeof(BYTE)*GAME_PLAYER);

  //用户状态
  for(WORD i = 0; i < GAME_PLAYER; i++)
  {
    //获取用户
    if(m_cbPlayStatus[i] != FALSE)
    {
      m_lTableScore[i] = m_lCellScore;
    }
  }

  //玩家处理
  ASSERT(m_pIAndroidUserItem != NULL);
  if(m_pIAndroidUserItem->GetChairID()==pGameStart->wCurrentUser)
  {
    //设置定时器
    UINT nElapse=TIME_LESS + (rand() % TIME_USER_ADD_SCORE);
    m_pIAndroidUserItem->SetGameTimer(IDI_USER_ADD_SCORE, nElapse);
  }

  return true;
}

//用户放弃
bool CAndroidUserItemSink::OnSubGiveUp(const void * pBuffer, WORD wDataSize)
{
  //效验数据
  ASSERT(wDataSize == sizeof(CMD_S_GiveUp));
  if(wDataSize!=sizeof(CMD_S_GiveUp))
  {
    return false;
  }

  //处理消息
  CMD_S_GiveUp * pGiveUp = (CMD_S_GiveUp *)pBuffer;

  //设置变量
  m_cbRealPlayer[pGiveUp->wGiveUpUser] = FALSE;
  m_cbPlayStatus[pGiveUp->wGiveUpUser] = FALSE;
  //m_cbAndroidStatus[pGiveUp->wGiveUpUser] = FALSE;

  return true;
}

//用户下注
bool CAndroidUserItemSink::OnSubAddScore(const void * pBuffer, WORD wDataSize)
{
  //效验数据
  ASSERT(wDataSize == sizeof(CMD_S_AddScore));
  if(wDataSize != sizeof(CMD_S_AddScore))
  {
    return false;
  }

  //处理消息
  CMD_S_AddScore * pAddScore = (CMD_S_AddScore *)pBuffer;

  //设置变量
  m_wCurrentUser = pAddScore->wCurrentUser;
  m_lCurrentTimes = pAddScore->lCurrentTimes;
  m_lTableScore[pAddScore->wAddScoreUser] += pAddScore->lAddScoreCount;
  WORD wMeChairID = m_pIAndroidUserItem->GetChairID();

  //用户动作
  if(wMeChairID==m_wCurrentUser && pAddScore->wCompareState==FALSE)
  {
    //设置定时器
    UINT nElapse=TIME_LESS + (rand() % TIME_USER_ADD_SCORE);
    m_pIAndroidUserItem->SetGameTimer(IDI_USER_ADD_SCORE, nElapse);
  }

  return true;
}

//用户看牌
bool CAndroidUserItemSink::OnSubLookCard(const void * pBuffer, WORD wDataSize)
{
  //效验数据
  ASSERT(wDataSize == sizeof(CMD_S_LookCard));
  if(wDataSize != sizeof(CMD_S_LookCard))
  {
    return false;
  }

  //处理消息
  CMD_S_LookCard * pLookCard = (CMD_S_LookCard *)pBuffer;

  //看牌用户
  if(m_wCurrentUser == m_pIAndroidUserItem->GetChairID())
  {
    //用户扑克
    CopyMemory(m_cbHandCardData, pLookCard->cbCardData, sizeof(m_cbHandCardData));

    //获取类型
    m_GameLogic.SortCardList(m_cbHandCardData, MAX_COUNT);
    m_cbCardType = m_GameLogic.GetCardType(m_cbHandCardData, MAX_COUNT);

    //如为散牌
    if(m_cbCardType < CT_DOUBLE)
    {
      for(BYTE i = 0; i < MAX_COUNT; i++)
      {
        m_cbCardType = 0;
        if(m_GameLogic.GetCardLogicValue(m_cbHandCardData[i]) > 11)
        {
          m_cbCardType = 1;
          break;
        }
      }
    }

    //设置定时器
    UINT nElapse=TIME_LESS + (rand() % TIME_USER_ADD_SCORE);
    m_pIAndroidUserItem->SetGameTimer(IDI_USER_ADD_SCORE, nElapse);
  }

  return true;
}

//用户比牌
bool CAndroidUserItemSink::OnSubCompareCard(const void * pBuffer, WORD wDataSize)
{
  //数据效验
  ASSERT(wDataSize == sizeof(CMD_S_CompareCard));
  if(wDataSize != sizeof(CMD_S_CompareCard))
  {
    return false;
  }

  //处理消息
  CMD_S_CompareCard * pCompareCard = (CMD_S_CompareCard *)pBuffer;

  //设置变量
  m_wCurrentUser=pCompareCard->wCurrentUser;
  m_cbPlayStatus[pCompareCard->wLostUser] = FALSE;

  //玩家人数
  BYTE cbUserCount = 0;
  for(WORD i=0; i<GAME_PLAYER; i++)
  {
    if(m_cbPlayStatus[i] == TRUE)
    {
      cbUserCount++;
    }
  }

  //判断结束
  if(cbUserCount > 1)
  {
    //当前用户
    if(m_pIAndroidUserItem->GetChairID() == m_wCurrentUser)
    {
      //设置定时器
      UINT nElapse=TIME_LESS + (rand() % TIME_USER_ADD_SCORE);
      m_pIAndroidUserItem->SetGameTimer(IDI_USER_ADD_SCORE, nElapse+TIME_DELAY_TIME);
    }
  }
  else if(m_pIAndroidUserItem->GetChairID() == pCompareCard->wCompareUser[0]
          || m_pIAndroidUserItem->GetChairID() == pCompareCard->wCompareUser[1])
  {
    m_pIAndroidUserItem->SetGameTimer(IDI_USER_FINISH_FLASH,TIME_USER_FINISH_FLASH);
  }

  return true;
}

//用户开牌
bool CAndroidUserItemSink::OnSubOpenCard(const void * pBuffer, WORD wDataSize)
{
  //数据效验
  ASSERT(wDataSize == sizeof(CMD_S_OpenCard));
  if(wDataSize != sizeof(CMD_S_OpenCard))
  {
    return false;
  }

  return true;
}

//用户强退
bool CAndroidUserItemSink::OnSubPlayerExit(const void * pBuffer, WORD wDataSize)
{
  //效验数据
  ASSERT(wDataSize == sizeof(CMD_S_PlayerExit));
  if(wDataSize != sizeof(CMD_S_PlayerExit))
  {
    return false;
  }

  //处理消息
  CMD_S_PlayerExit * pPlayerExit=(CMD_S_PlayerExit *)pBuffer;

  //设置变量
  m_cbPlayStatus[pPlayerExit->wPlayerID] = FALSE;

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
  CMD_S_GameEnd * pGameEnd = (CMD_S_GameEnd *)pBuffer;

  //删除定时器
  m_pIAndroidUserItem->KillGameTimer(IDI_START_GAME);
  m_pIAndroidUserItem->KillGameTimer(IDI_USER_ADD_SCORE);
  m_pIAndroidUserItem->KillGameTimer(IDI_USER_COMPARE_CARD);

  //清理变量
  m_lMaxCellScore = 0;
  ZeroMemory(m_cbHandCardData,sizeof(m_cbHandCardData));

  //开始设置
  WORD wServerType=pGameEnd->wServerType;
  if(wServerType==GAME_GENRE_MATCH)
  {
    return true;
  }
  //开始时间
  if(pGameEnd->bDelayOverGame==true)
  {
    UINT nElapse=TIME_LESS + (rand() % TIME_DELAY_TIME);
    m_pIAndroidUserItem->SetGameTimer(IDI_DELAY_TIME, nElapse);
  }
  else
  {
    UINT nElapse=TIME_LESS + (rand() % TIME_START_GAME);
    m_pIAndroidUserItem->SetGameTimer(IDI_START_GAME, nElapse);
  }

  return true;
}

//银行操作
void CAndroidUserItemSink::BankOperate(BYTE cbType)
{
  IServerUserItem *pUserItem = m_pIAndroidUserItem->GetMeUserItem();
  if(pUserItem->GetUserStatus()>=US_SIT)
  {
    if(cbType==1)
    {
      CString strInfo;
      strInfo.Format(TEXT("大厅：状态不对，不执行存取款"));
      NcaTextOut(strInfo, m_szRoomName);
      return;
    }
  }

  //变量定义
  LONGLONG lRobotScore = pUserItem->GetUserScore();

  {
    CString strInfo;
    strInfo.Format(TEXT("[%s] 分数(%I64d), 存款条件(%I64d), 取款条件(%I64d)"),pUserItem->GetNickName(),lRobotScore, m_lRobotScoreRange[1], m_lRobotScoreRange[0]);

    if(lRobotScore > m_lRobotScoreRange[1])
    {
      CString strInfo1;
      strInfo1.Format(TEXT("满足存款条件(%I64d)"),m_lRobotScoreRange[1]);
      strInfo+=strInfo1;

      NcaTextOut(strInfo, m_szRoomName);
    }
    else if(lRobotScore < m_lRobotScoreRange[0])
    {
      CString strInfo1;
      strInfo1.Format(TEXT("满足取款条件(%I64d)"),m_lRobotScoreRange[0]);
      strInfo+=strInfo1;

      NcaTextOut(strInfo, m_szRoomName);
    }
    else
    {
      CString strInfo1;
      strInfo1.Format(TEXT("不存不取--"));
      strInfo+=strInfo1;

      NcaTextOut(strInfo, m_szRoomName);
    }


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

      NcaTextOut(strInfo, m_szRoomName);
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

      NcaTextOut(strInfo, m_szRoomName);
    }
  }
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

//推断胜者
WORD CAndroidUserItemSink::EstimateWinner()
{
  //保存扑克
  BYTE cbUserCardData[GAME_PLAYER][MAX_COUNT];
  CopyMemory(cbUserCardData, m_cbAllHandCardData, sizeof(cbUserCardData));

  //排列扑克
  for(WORD i = 0; i < GAME_PLAYER; i++)
  {
    m_GameLogic.SortCardList(cbUserCardData[i], MAX_COUNT);
  }

  //变量定义
  WORD wWinUser = INVALID_CHAIR;

  //查找数据
  for(WORD i = 0; i < GAME_PLAYER; i++)
  {
    //用户过滤
    if(m_cbPlayStatus[i] == FALSE)
    {
      continue;
    }

    //设置用户
    if(wWinUser == INVALID_CHAIR)
    {
      wWinUser=i;
      continue;
    }

    //对比扑克
    if(m_GameLogic.CompareCard(cbUserCardData[i], cbUserCardData[wWinUser], MAX_COUNT) >= TRUE)
    {
      wWinUser = i;
    }
  }

  return wWinUser;
}

//加注倍数
LONGLONG CAndroidUserItemSink::GetAddScoreTimes()
{
  LONGLONG lTimesSelect[3] = {1,2,5};
  LONGLONG lTimes = 10 - m_lCurrentTimes;
  if(lTimes >= lTimesSelect[2])
  {
    lTimes = lTimesSelect[rand()%3];
  }
  else if(lTimes >= lTimesSelect[1])
  {
    lTimes = lTimesSelect[rand()%2];
  }
  else if(lTimes == 1)
  {
    lTimes = lTimesSelect[0];
  }
  else
  {
    lTimes = 0;
  }

  return lTimes;
}
//组件创建函数
DECLARE_CREATE_MODULE(AndroidUserItemSink);

//////////////////////////////////////////////////////////////////////////
