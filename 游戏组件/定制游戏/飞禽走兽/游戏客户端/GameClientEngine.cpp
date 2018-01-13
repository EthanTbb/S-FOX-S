#include "Stdafx.h"
#include "GameClient.h"
//#include "GameOption.h"
#include "GameClientEngine.h"
#include ".\GameClientEngine.h"

//////////////////////////////////////////////////////////////////////////

//时间标识
#define IDI_FREE          99                  //空闲时间
#define IDI_PLACE_JETTON      100                 //下注时间
#define IDI_DISPATCH_CARD     301                 //发牌时间
#define IDI_OPEN_CARD       302                   //发牌时间

#define IDI_ANDROID_BET       1000


//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameClientEngine, CGameFrameEngine)
  ON_MESSAGE(IDM_PLACE_JETTON,OnPlaceJetton)
  ON_MESSAGE(IDM_APPLY_BANKER, OnApplyBanker)
  ON_MESSAGE(IDM_SOUND,OnPlaySound)
  ON_MESSAGE(IDM_ANIMAL_SOUND,OnAnimalSound)
  ON_MESSAGE(IDM_ADMIN_COMMDN, OnAdminControl)
  ON_MESSAGE(IDM_UPDATE_STORAGE,OnUpdateStorage)
  ON_MESSAGE(IDM_CLEAN_JETTON,OnCleanJetton)
  ON_MESSAGE(IDM_LAST_JETTON,OnLastPlaceJetton)

  ON_WM_TIMER()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
//构造函数
CGameClientEngine::CGameClientEngine()
{
  try
  {
    //限制信息
    m_lMeMaxScore=0L;
    m_lAreaLimitScore=0L;
    m_lApplyBankerCondition=0L;
    m_GameEndTime = 0;

    m_blUsing = false;

    m_bAddScore=false;

    //个人下注
    ZeroMemory(m_lUserJettonScore,sizeof(m_lUserJettonScore));
    ZeroMemory(m_lPlayerBetAll,sizeof(m_lPlayerBetAll));
    //所有玩家下注
    ZeroMemory(m_lAllPlayBet,sizeof(m_lAllPlayBet));
    //庄家信息
    m_lBankerScore=0L;
    m_wCurrentBanker=0L;

    //状态变量
    m_bMeApplyBanker=false;

    //区域几率
    for(int i = 0; i < CountArray(m_nAnimalPercent); ++i)
    {
      m_nAnimalPercent[i] = 1;
    }

    m_bPlaceEnd = false;
  }
  catch(...)
  {
    //AfxMessageBox(TEXT("引擎构造Error"));
  }

  return;
}

//析构函数
CGameClientEngine::~CGameClientEngine()
{
}

//初始函数
bool CGameClientEngine::OnInitGameEngine()
{

  try
  {
    CGlobalUnits * m_pGlobalUnits=(CGlobalUnits *)CGlobalUnits::GetInstance();
    ASSERT(m_pGlobalUnits!=NULL);

    //设置标题
    //SetWindowText(TEXT("飞禽走兽游戏  --  Ver：6.6.0.3"));

    //设置图标
    HICON hIcon=LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME));
    m_pIClientKernel->SetGameAttribute(KIND_ID,GAME_PLAYER,VERSION_CLIENT,hIcon,GAME_NAME);
    SetIcon(hIcon,TRUE);
    SetIcon(hIcon,FALSE);
  }
  catch(...)
  {
    //AfxMessageBox(TEXT("引擎初始化 Error"));
  }

  return true;
}

//重置框架
bool CGameClientEngine::OnResetGameEngine()
{
  //限制信息
  m_lMeMaxScore=0L;

  //个人下注
  ZeroMemory(m_lUserJettonScore,sizeof(m_lUserJettonScore));
  ZeroMemory(m_lPlayerBetAll,sizeof(m_lPlayerBetAll));


  //庄家信息
  m_lBankerScore=0L;
  m_wCurrentBanker=0L;
  //状态变量
  m_bMeApplyBanker=false;
  m_bAddScore=false;
  return true;
}

//游戏设置
void CGameClientEngine::OnGameOptionSet()
{
  try
  {
    return;
  }
  catch(...)
  {
    //AfxMessageBox(TEXT("OnGameOptionSet() Error"));
  }
}

//时钟删除
bool CGameClientEngine::OnEventGameClockKill(WORD wChairID)
{
  try
  {
    return true;
  }
  catch(...)
  {
    //AfxMessageBox(TEXT("OnEventGameClockKill(WORD wChairID) Error"));
  }

}

//旁观状态
bool CGameClientEngine::OnEventLookonMode(VOID * pData, WORD wDataSize)
{
  try
  {
    return true;
  }
  catch(...)
  {
    //AfxMessageBox(TEXT("OnEventLookonMode(VOID * pData, WORD wDataSize) Error"));
  }
}

//时间消息
bool CGameClientEngine::OnEventGameClockInfo(WORD wChairID, UINT nElapse, WORD nTimerID)
{
  if((nTimerID==IDI_PLACE_JETTON)&&(nElapse<2))
  {
    //禁止按钮
    m_GameClientView.m_btJetton100.EnableWindow(FALSE);
    m_GameClientView.m_btJetton1000.EnableWindow(FALSE);
    m_GameClientView.m_btJetton10000.EnableWindow(FALSE);
    m_GameClientView.m_btJetton100000.EnableWindow(FALSE);
    m_GameClientView.m_btJetton1000000.EnableWindow(FALSE);

    m_GameClientView.m_lCurrentJetton = 0L;
  }

  if(IDI_DISPATCH_CARD == nTimerID&&(nElapse==0))
  {
    m_GameClientView.SetAnimalPercent(m_nAnimalPercent);

  }
  if(m_GameEndTime==5&&m_bAddScore)
  {
    m_bAddScore=false;
    CString strInfo=TEXT("");
    strInfo=TEXT("本局结束,成绩统计:");
    m_pIStringMessage->InsertSystemString(strInfo);
    strInfo.Format(TEXT("庄家:%I64d"),m_bankerScore);
    m_pIStringMessage->InsertNormalString(strInfo);
    strInfo.Format(TEXT("本家:%I64d"),m_userScore);
    m_pIStringMessage->InsertNormalString(strInfo);
    m_GameEndTime--;
  }
  else
  {
    if(m_GameEndTime>0)
    {
      m_GameEndTime--;
    }
  }
  if(nTimerID==IDI_PLACE_JETTON&&nElapse<=5)
  {
    PlayGameSound(AfxGetInstanceHandle(),TEXT("TIME_WARIMG"));
  }

  return true;
}

////旁观状态
//void CGameClientEngine::OnEventLookonMode(VOID * pData, WORD wDataSize)
//{
//}

//网络消息
bool CGameClientEngine::OnEventGameMessage(WORD wSubCmdID, VOID * pBuffer, WORD wDataSize)
{
  try
  {
    switch(wSubCmdID)
    {
      case SUB_S_GAME_FREE:   //游戏空闲
      {
        return OnSubGameFree(pBuffer,wDataSize);
      }
      case SUB_S_GAME_START:    //游戏开始
      {
        return OnSubGameStart(pBuffer,wDataSize);
      }
      case SUB_S_PLACE_JETTON:  //用户加注
      {
        return OnSubPlaceJetton(pBuffer,wDataSize);
      }
      case SUB_S_APPLY_BANKER:  //申请做庄
      {
        return OnSubUserApplyBanker(pBuffer, wDataSize);
      }
      case SUB_S_CANCEL_BANKER: //取消做庄
      {
        return OnSubUserCancelBanker(pBuffer, wDataSize);
      }
      case SUB_S_CHANGE_BANKER: //切换庄家
      {
        return OnSubChangeBanker(pBuffer, wDataSize);
      }
      case SUB_S_GAME_END:    //游戏结束
      {
        return OnSubGameEnd(pBuffer,wDataSize);
      }
      case SUB_S_SEND_RECORD:   //游戏记录
      {
        return OnSubGameRecord(pBuffer,wDataSize);
      }
      case SUB_S_PLACE_JETTON_FAIL: //下注失败
      {
        return OnSubPlaceJettonFail(pBuffer,wDataSize);
      }
      /*case SUB_S_CHECK_IMAGE:
        {
          return OnSubCheckImageIndex(pBuffer,wDataSize);
        }*/
      case SUB_S_CEAN_JETTON:   //清楚
      {
        //效验数据
        ASSERT(wDataSize==sizeof(CMD_S_CeanJetton));
        if(wDataSize!=sizeof(CMD_S_CeanJetton))
        {
          return false;
        }

        //消息处理
        CMD_S_CeanJetton * pCleanJetton=(CMD_S_CeanJetton *)pBuffer;

        ZeroMemory(m_GameClientView.m_lAllJettonScore,sizeof(m_GameClientView.m_lAllJettonScore));

        //所有下注
        for(int nAreaIndex=0; nAreaIndex<AREA_COUNT+1; nAreaIndex++)
        {
          m_GameClientView.m_lAllJettonScore[nAreaIndex] += pCleanJetton->lAllCPlaceScore[nAreaIndex];
        }

        m_GameClientView.RefreshGameView();
        return true;
      }
      case SUB_S_ADMIN_COMMDN:    //设置
      {
        return OnSubAdminControl(pBuffer,wDataSize);
      }
      case SUB_S_UPDATE_STORAGE:  //更新库存
      {
        return OnSubUpdateStorage(pBuffer,wDataSize);
      }
      case SUB_S_CONTINU_JETTON:
      {
        //效验数据
        ASSERT(wDataSize==sizeof(CMD_S_ContiueJetton));
        if(wDataSize!=sizeof(CMD_S_ContiueJetton))
        {
          return false;
        }


        //消息处理
        CMD_S_ContiueJetton * pLastJetton=(CMD_S_ContiueJetton *)pBuffer;

        //取得上局下注信息 为当前 下注信息
        //if (GetMeChairID()==pLastJetton->wChairID || IsLookonMode())
        {

          //特殊账户不隐藏机器人下注
          if(CUserRight::IsGameCheatUser(m_pIClientKernel->GetUserAttribute()->dwUserRight))
          {
            if(pLastJetton->cbAndroid == TRUE)
            {
              return true;
            }
          }

          if(GetMeChairID() == pLastJetton->wChairID)
          {
            CopyMemory(m_GameClientView.m_lUserJettonScore, pLastJetton->lUserJettonScore, sizeof(m_GameClientView.m_lUserJettonScore));
          }

          CopyMemory(m_GameClientView.m_lAllJettonScore, pLastJetton->lAllJettonScore, sizeof(m_GameClientView.m_lUserJettonScore));

          /*if (pLastJetton->wChairID!=GetMeChairID() || IsLookonMode())
          {
            if (pLastJetton->lJettonScore==5000000) PlayGameSound(AfxGetInstanceHandle(),TEXT("ADD_GOLD_EX"));
            else PlayGameSound(AfxGetInstanceHandle(),TEXT("ADD_GOLD"));
          }*/

          //播放声音
          PlayGameSound(AfxGetInstanceHandle(),TEXT("ADD_GOLD"));

          m_GameClientView.RefreshGameView();
        }


        return true;
      }
    }

    //错误断言
    ASSERT(FALSE);
  }
  catch(...)
  {
    //AfxMessageBox(TEXT("网络消息异常"));
  }

  return true;
}

//游戏场景
bool CGameClientEngine::OnEventSceneMessage(BYTE cbGameStation, bool bLookonOther, VOID * pBuffer, WORD wDataSize)
{
  //SetGameStatus(GAME_STATUS_FREE);
  try
  {
    switch(cbGameStation)
    {
      case GAME_STATUS_FREE:      //空闲状态
      {
        //效验数据
        ASSERT(wDataSize==sizeof(CMD_S_StatusFree));
        if(wDataSize!=sizeof(CMD_S_StatusFree))
        {
          return false;
        }

        m_GameClientView.m_bGameStatus = 1;
        //消息处理
        CMD_S_StatusFree * pStatusFree=(CMD_S_StatusFree *)pBuffer;
        m_bEnableSysBanker=pStatusFree->bEnableSysBanker;

        m_GameClientView.m_bDrawYingZi =false;
        m_GameClientView.m_lCurStorage = pStatusFree->lBonus;

        //玩家信息
        m_lMeMaxScore=pStatusFree->lUserMaxScore;
        m_GameClientView.SetMeMaxScore(m_lMeMaxScore);
        //IClientUserItem *pMeUserData=GetTableUserItem(GetMeChairID());

        IClientUserItem *pMeUserData=GetTableUserItem(GetMeChairID());

        m_GameClientView.SetMeChairID(pMeUserData->GetUserID());

        //庄家信息
        SetBankerInfo(pStatusFree->wBankerUser,pStatusFree->lBankerScore);
        m_GameClientView.SetBankerScore(pStatusFree->cbBankerTime,pStatusFree->lBankerWinScore);

        m_GameClientView.EnableSysBanker(m_bEnableSysBanker);

        //控制信息
        m_lApplyBankerCondition=pStatusFree->lApplyBankerCondition;
        m_lAreaLimitScore=pStatusFree->lAreaLimitScore;
        m_GameClientView.SetAreaLimitScore(m_lAreaLimitScore);


        //设置几率
        memcpy(m_nAnimalPercent, pStatusFree->nAnimalPercent, sizeof(m_nAnimalPercent));
        m_GameClientView.SetAnimalPercent(m_nAnimalPercent);

        //开启
        //if((GetTableUserItem(GetMeChairID())->dwUserRight&UR_GAME_CONTROL)!=0&&m_GameClientView.m_pAdminControl!=NULL)
        //  m_GameClientView.m_btOpenAdmin.ShowWindow(SW_SHOW);

        //开启
        if(CUserRight::IsGameCheatUser(m_pIClientKernel->GetUserAttribute()->dwUserRight)&&m_GameClientView.m_hInst)
        {
          m_GameClientView.m_btOpenAdmin.ShowWindow(SW_SHOW);
        }

        //设置状态
        SetGameStatus(GAME_STATUS_FREE);

        //更新控制
        UpdateButtonContron();
        m_GameClientView.RefreshGameView();

        //设置时间
        SetGameClock(GetMeChairID(),IDI_FREE,pStatusFree->cbTimeLeave);

        //播放声音
        PlayBackGroundSound(AfxGetInstanceHandle(),TEXT("GAME_FREE"));

        return true;
      }
      case GS_PLACE_JETTON: //游戏状态
      case GS_GAME_END:   //结束状态
      {

        //效验数据
        ASSERT(wDataSize==sizeof(CMD_S_StatusPlay));
        if(wDataSize!=sizeof(CMD_S_StatusPlay))
        {
          return false;
        }

        m_GameClientView.m_bGameStatus = 1;
        //消息处理
        CMD_S_StatusPlay * pStatusPlay=(CMD_S_StatusPlay *)pBuffer;

        //下注信息
        for(int nAreaIndex=1; nAreaIndex<=AREA_COUNT; ++nAreaIndex)
        {
          m_GameClientView.PlaceUserJetton(nAreaIndex,pStatusPlay->lAllJettonScore[nAreaIndex]);
          SetMePlaceJetton(nAreaIndex,pStatusPlay->lUserJettonScore[nAreaIndex]);
        }

        m_GameClientView.m_bDrawYingZi =false;
        m_GameClientView.m_lCurStorage = pStatusPlay->lBonus;

        //玩家积分
        m_lMeMaxScore=pStatusPlay->lUserMaxScore;
        m_GameClientView.SetMeMaxScore(m_lMeMaxScore);
        IClientUserItem *pMeUserData=GetTableUserItem(GetMeChairID());
        m_GameClientView.SetMeChairID(pMeUserData->GetUserID());

        //控制信息
        m_lApplyBankerCondition=pStatusPlay->lApplyBankerCondition;
        m_lAreaLimitScore=pStatusPlay->lAreaLimitScore;
        m_GameClientView.SetAreaLimitScore(m_lAreaLimitScore);

        if(pStatusPlay->cbGameStatus==GS_GAME_END)
        {
          //扑克信息
          m_GameClientView.SetCardInfo(pStatusPlay->cbTableCardArray);
          m_GameClientView.FinishDispatchCard(false);

          //设置成绩
          m_GameClientView.SetCurGameScore(pStatusPlay->lEndUserScore,pStatusPlay->lEndUserReturnScore,pStatusPlay->lEndBankerScore,pStatusPlay->lEndRevenue);
        }
        else
        {
          m_GameClientView.SetCardInfo(NULL);
          m_blUsing = true;

        }

        //开启
        if(CUserRight::IsGameCheatUser(m_pIClientKernel->GetUserAttribute()->dwUserRight)&&m_GameClientView.m_hInst)
        {
          m_GameClientView.m_btOpenAdmin.ShowWindow(SW_SHOW);
        }

        //播放声音
        PlayBackGroundSound(AfxGetInstanceHandle(),TEXT("GAME_FREE"));

        //庄家信息
        SetBankerInfo(pStatusPlay->wBankerUser,pStatusPlay->lBankerScore);
        m_GameClientView.SetBankerScore(pStatusPlay->cbBankerTime,pStatusPlay->lBankerWinScore);
        m_bEnableSysBanker=pStatusPlay->bEnableSysBanker;
        m_GameClientView.EnableSysBanker(m_bEnableSysBanker);

        //设置几率
        memcpy(m_nAnimalPercent, pStatusPlay->nAnimalPercent, sizeof(m_nAnimalPercent));
        m_GameClientView.SetAnimalPercent(m_nAnimalPercent);

        //开启
        //if((GetTableUserItem(GetMeChairID())->dwUserRight&UR_GAME_CONTROL)!=0&&m_GameClientView.m_pAdminControl!=NULL)
        //  m_GameClientView.m_btOpenAdmin.ShowWindow(SW_SHOW);

        //更新按钮
        UpdateButtonContron();

        //设置时间
        UINT nTimerID = pStatusPlay->cbGameStatus==GS_GAME_END ? IDI_OPEN_CARD : IDI_PLACE_JETTON;
        SetGameClock(GetMeChairID(), nTimerID, pStatusPlay->cbTimeLeave);

        //设置状态
        SetGameStatus(pStatusPlay->cbGameStatus);

        m_GameClientView.StartRandShowSide();

        m_GameClientView.RefreshGameView();
        return true;
      }
    }


  }
  catch(...)
  {
    //AfxMessageBox(TEXT("场景消息异常"));
  }

  return false;
}

//游戏开始
bool CGameClientEngine::OnSubGameStart(const void * pBuffer, WORD wDataSize)
{
  try
  {
    //效验数据
    ASSERT(wDataSize==sizeof(CMD_S_GameStart));
    if(wDataSize!=sizeof(CMD_S_GameStart))
    {
      return false;
    }

    m_GameClientView.m_bGameStatus = 1;

    m_bPlaceEnd = false;
    //消息处理
    CMD_S_GameStart * pGameStart=(CMD_S_GameStart *)pBuffer;

    m_GameClientView.SwithToNormalView();

    m_GameClientView.KillCardTime();

    m_blUsing = true;

    m_GameClientView.m_lCurStorage = pGameStart->lBonus;

    //庄家信息
    SetBankerInfo(pGameStart->wBankerUser,pGameStart->lBankerScore);

    //玩家信息
    m_lMeMaxScore=pGameStart->lUserMaxScore;
    m_GameClientView.SetMeMaxScore(m_lMeMaxScore);

    //设置时间
    SetGameClock(GetMeChairID(),IDI_PLACE_JETTON,pGameStart->cbTimeLeave);

    //下注状态
    m_GameClientView.m_bAddScoreStatue=false;

    //设置状态
    SetGameStatus(GS_PLACE_JETTON);

    m_GameClientView.StartRandShowSide();

    SetTimer(IDI_ANDROID_BET, 100, NULL);

    //更新控制
    UpdateButtonContron(true);



    OnPlaySound(0,0);

  }
  catch(...)
  {
    //AfxMessageBox(TEXT("开始游戏异常"));
  }

  return true;
}

//游戏空闲
bool CGameClientEngine::OnSubGameFree(const void * pBuffer, WORD wDataSize)
{
  //效验数据
  ASSERT(wDataSize==sizeof(CMD_S_GameFree));
  if(wDataSize!=sizeof(CMD_S_GameFree))
  {
    return false;
  }

  //消息处理
  CMD_S_GameFree * pGameFree=(CMD_S_GameFree *)pBuffer;

  m_blUsing = true;

  //设置时间
  SetGameClock(GetMeChairID(),IDI_FREE,pGameFree->cbTimeLeave);

  //m_GameClientView.StartMove();

  //设置状态
  SetGameStatus(GAME_STATUS_FREE);
  //如果是管理员则每局发送库存量
  CString cs;
  cs.Format(TEXT("当前库存量为：%I64d"),pGameFree->lStorageStart);
  if(CUserRight::IsGameCheatUser(m_pIClientKernel->GetUserAttribute()->dwUserRight)&&m_GameClientView.m_hInst)
  {

    m_GameClientView.m_pAdminControl->SetStorageCurrentValue(pGameFree->lStorageStart, pGameFree->lBonus);
    if(m_pIStringMessage!=NULL)
    {
      m_pIStringMessage->InsertSystemString(cs);
    }
  }

  m_GameClientView.m_lCurStorage = pGameFree->lBonus;

  //清理时间
  KillTimer(IDI_ANDROID_BET);

  ////清理桌面
  bool blWin[AREA_COUNT];
  for(int i = 0; i<AREA_COUNT; i++)
  {

    blWin[i]=false;
  }

  m_GameClientView.FinishDispatchCard();

  m_GameClientView.SetWinnerSide(blWin, false);


  for(int nAreaIndex=1; nAreaIndex<=AREA_COUNT; ++nAreaIndex)
  {
    SetMePlaceJetton(nAreaIndex,0);
  }
  m_GameClientView.ResetGameView();
  //所有玩家下注
  ZeroMemory(m_lAllPlayBet,sizeof(m_lAllPlayBet));


  m_GameClientView.SwitchToCheck();

  //PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_FREE"));

  //更新控件
  UpdateButtonContron();

  return true;
}

//用户加注
bool CGameClientEngine::OnSubPlaceJetton(const void * pBuffer, WORD wDataSize)
{
  //效验数据
  ASSERT(wDataSize==sizeof(CMD_S_PlaceJetton));
  if(wDataSize!=sizeof(CMD_S_PlaceJetton))
  {
    return false;
  }

  //消息处理
  CMD_S_PlaceJetton * pPlaceJetton=(CMD_S_PlaceJetton *)pBuffer;
  if(GetMeChairID()!=pPlaceJetton->wChairID || IsLookonMode())
  {

//    //特殊账户不隐藏机器人下注
//    if(CUserRight::IsGameCheatUser(m_pIClientKernel->GetUserAttribute()->dwUserRight))
//    {
//      if(pPlaceJetton->cbAndroid == TRUE)
//      {
//        return true;
//      }
//    }

    m_GameClientView.PlaceUserJetton(pPlaceJetton->cbJettonArea,pPlaceJetton->lJettonScore);

    //播放声音
    //if (pPlaceJetton->wChairID!=GetMeChairID() || IsLookonMode())
    {
      if(pPlaceJetton->lJettonScore==5000000)
      {
        PlayGameSound(AfxGetInstanceHandle(),TEXT("ADD_GOLD_EX"));
      }
      else
      {
        PlayGameSound(AfxGetInstanceHandle(),TEXT("ADD_GOLD"));
      }


    }

  }

  //下注状态
  if(GetMeChairID()==pPlaceJetton->wChairID)
  {
    m_GameClientView.m_bAddScoreStatue=false;
  }

  m_lAllPlayBet[pPlaceJetton->wChairID][pPlaceJetton->cbJettonArea-1]+=pPlaceJetton->lJettonScore;
  CUserBetArray *pUserBetArray = &m_GameClientView.m_pAdminControl->m_UserBetArray;
  pUserBetArray->RemoveAll();

  for(WORD i=0; i<GAME_PLAYER; i++)
  {
    IClientUserItem *pClientUserItem=GetTableUserItem(i);

    if(NULL == pClientUserItem)
    {
      continue;
    }
    if(pClientUserItem->IsAndroidUser())
    {
      continue;
    }

    LONGLONG lUserAllBet = 0;
    for(BYTE j=0; j<=AREA_COUNT; j++)
    {
      lUserAllBet += m_lAllPlayBet[i][j];
    }
    if(0 == lUserAllBet)
    {
      continue;
    }

    //构造数据
    tagUserBet UserBet;
    ZeroMemory(&UserBet, sizeof(UserBet));

    CopyMemory(UserBet.szNickName, pClientUserItem->GetNickName(), sizeof(UserBet.szNickName));
    UserBet.dwUserGameID = pClientUserItem->GetGameID();
    UserBet.lUserStartScore =   pPlaceJetton->lUserStartScore[i];
    UserBet.lUserWinLost = pClientUserItem->GetUserScore() - UserBet.lUserStartScore;

    for(BYTE k=0; k<AREA_COUNT; k++)
    {
      UserBet.lUserBet[k] = m_lAllPlayBet[i][k];
    }

    //插入数据
    pUserBetArray->Add(UserBet);
  }

  //更新控件
  m_GameClientView.m_pAdminControl->UpdateUserBet(false);

  //UpdateButtonContron();

  return true;
}

//游戏结束
bool CGameClientEngine::OnSubGameEnd(const void * pBuffer, WORD wDataSize)
{
  //效验数据
  ASSERT(wDataSize==sizeof(CMD_S_GameEnd));
  if(wDataSize!=sizeof(CMD_S_GameEnd))
  {
    return false;
  }

  //消息处理
  CMD_S_GameEnd * pGameEnd=(CMD_S_GameEnd *)pBuffer;

  //m_GameClientView.m_lCurStorage = pGameEnd->lStorageStart;
  m_GameClientView.m_blMoveFinish = false;


  m_GameEndTime = pGameEnd->cbTimeLeave;

  m_blUsing = true;

  //设置时间
  SetGameClock(GetMeChairID(),IDI_DISPATCH_CARD, pGameEnd->cbTimeLeave);

  //扑克信息
  m_GameClientView.SetCardInfo(pGameEnd->cbTableCardArray);
  m_GameClientView.ClearAreaFlash();

  m_GameClientView.SetShaYuAddMulti(pGameEnd->cbShaYuAddMulti);

  m_GameClientView.StartRunCar(20);

  //保存几率
  memcpy(m_nAnimalPercent, pGameEnd->nAnimalPercent, sizeof(m_nAnimalPercent));


  memcpy(m_GameClientView.m_lUserLastJetton, pGameEnd->lLastJetton, sizeof(pGameEnd->lLastJetton));

  //设置状态
  SetGameStatus(GS_GAME_END);

  //庄家信息
  m_GameClientView.SetBankerScore(pGameEnd->nBankerTime, pGameEnd->lBankerTotallScore);
  //成绩信息
  m_GameClientView.SetCurGameScore(pGameEnd->lUserScore,pGameEnd->lUserReturnScore,pGameEnd->lBankerScore,pGameEnd->lRevenue);
  m_bankerScore=pGameEnd->lBankerScore;
  m_userScore=pGameEnd->lUserScore;
  m_GameClientView.PerformAllBetAnimation();

  //更新控件
  UpdateButtonContron();

  return true;
}

//更新控制
void CGameClientEngine::UpdateButtonContron(bool bCanPlaceJetton)
{

  //置能判断
  bool bEnablePlaceJetton=true;

  if(m_wCurrentBanker==INVALID_CHAIR)
  {
    bEnablePlaceJetton = true;
  }
  if(!bCanPlaceJetton)
  {

    bEnablePlaceJetton=false;
  }
  if(GetGameStatus()!=GS_PLACE_JETTON)
  {

    bEnablePlaceJetton=false;
  }
  if(m_wCurrentBanker==GetMeChairID())
  {

    bEnablePlaceJetton=false;
  }
  if(IsLookonMode())
  {
    bEnablePlaceJetton=false;
  }
//  if (m_bEnableSysBanker==false&&m_wCurrentBanker==INVALID_CHAIR)
//  {
//
//    bEnablePlaceJetton=false;
//  }

  //下注按钮
  if(bEnablePlaceJetton==true)
  {

    //计算积分
    LONGLONG lCurrentJetton=m_GameClientView.GetCurrentJetton();
    LONGLONG lLeaveScore=m_lMeMaxScore;

    for(int nAreaIndex=1; nAreaIndex<=AREA_COUNT; ++nAreaIndex)
    {
      lLeaveScore -= m_lUserJettonScore[nAreaIndex];
    }

    //最大下注
    LONGLONG lUserMaxJetton = 0;

    for(int nAreaIndex=1; nAreaIndex<=AREA_COUNT; ++nAreaIndex)
    {
      if(lUserMaxJetton==0&&nAreaIndex == 1)
      {
        lUserMaxJetton = m_GameClientView.GetUserMaxJetton(nAreaIndex);

      }
      else
      {
        if(m_GameClientView.GetUserMaxJetton(nAreaIndex)>lUserMaxJetton)
        {
          lUserMaxJetton = m_GameClientView.GetUserMaxJetton(nAreaIndex);
        }
      }
    }
    lLeaveScore = min((lLeaveScore),lUserMaxJetton); //用户可下分 和最大分比较 由于是五倍

    //控制按钮
    int iTimer = 1;

    if(m_blUsing==false)
    {
      lLeaveScore = 0;
      lUserMaxJetton = 0;


    }


    m_GameClientView.m_btJetton100.EnableWindow((lLeaveScore>=100*iTimer && lUserMaxJetton>=100*iTimer)?TRUE:FALSE);
    m_GameClientView.m_btJetton1000.EnableWindow((lLeaveScore>=1000*iTimer && lUserMaxJetton>=1000*iTimer)?TRUE:FALSE);
    m_GameClientView.m_btJetton10000.EnableWindow((lLeaveScore>=10000*iTimer && lUserMaxJetton>=10000*iTimer)?TRUE:FALSE);
    m_GameClientView.m_btJetton100000.EnableWindow((lLeaveScore>=100000*iTimer && lUserMaxJetton>=100000*iTimer)?TRUE:FALSE);
    m_GameClientView.m_btJetton1000000.EnableWindow((lLeaveScore>=1000000*iTimer && lUserMaxJetton>=1000000*iTimer)?TRUE:FALSE);

    //减少筹码
    if(lCurrentJetton>lLeaveScore)
    {
      m_GameClientView.ReduceJettonNumber();
    }

  }
  else
  {
    //禁止按钮
    m_GameClientView.m_btJetton100.EnableWindow(FALSE);
    m_GameClientView.m_btJetton1000.EnableWindow(FALSE);
    m_GameClientView.m_btJetton10000.EnableWindow(FALSE);
    m_GameClientView.m_btJetton100000.EnableWindow(FALSE);
    m_GameClientView.m_btJetton1000000.EnableWindow(FALSE);

    m_GameClientView.m_lCurrentJetton = 0L;
  }

  //获取信息
  IClientUserItem *pMeUserData=GetTableUserItem(GetMeChairID());
  //银行按钮
  m_GameClientView.m_btBankDraw.EnableWindow(TRUE);
  m_GameClientView.m_btBankStorage.EnableWindow(FALSE);
  //m_GameClientView.m_btBankStorage.ShowWindow(SW_SHOW);
  m_GameClientView.m_btBankStorage.EnableWindow((GetGameStatus()==GAME_STATUS_FREE || IsLookonMode())?TRUE:FALSE);
  if(m_wCurrentBanker==GetMeChairID()&&IsLookonMode()==false)
  {
    m_GameClientView.m_btBankStorage.EnableWindow(FALSE);
  }

  if(m_wCurrentBanker==GetMeChairID())
  {
    m_GameClientView.m_blCanStore = false;
  }
  else
  {
    m_GameClientView.m_blCanStore = true;
  }
  //m_GameClientView.SetInsureOption(false, m_GameClientView.m_blCanStore);

  ReSetBankCtrol(GetGameStatus());

  return;
}

LRESULT CGameClientEngine::OnCleanJetton(WPARAM wParam, LPARAM lParam)
{
  if(!(m_GameClientView.m_lCurrentJetton == 0L))
  {
    return 0;
  }

  //清空数据
  ZeroMemory(m_lPlayerBetAll,sizeof(m_lPlayerBetAll));
  ZeroMemory(m_lUserJettonScore,sizeof(m_lUserJettonScore));

  //构造数据
  CMD_C_CleanMeJetto CleanMeJetton;
  ZeroMemory(&CleanMeJetton,sizeof(CleanMeJetton));

  //当前玩家
  CleanMeJetton.wChairID=GetMeChairID();
  //发送消息
  SendSocketData(SUB_C_CLEAN_JETTON,&CleanMeJetton,sizeof(CleanMeJetton));

  //更新控件
  UpdateButtonContron(true);
  return 0;
}

//加注消息
LRESULT CGameClientEngine::OnPlaceJetton(WPARAM wParam, LPARAM lParam)
{
  //变量定义
  BYTE cbJettonArea=(BYTE)wParam;
  LONGLONG lJettonScore=(LONGLONG)(*((LONGLONG*)lParam));

  //合法判断
  ASSERT(cbJettonArea>=1 && cbJettonArea<=AREA_COUNT);
  if(!(cbJettonArea>=1 && cbJettonArea<=AREA_COUNT))
  {
    return 0;
  }

  //庄家判断
  if(GetMeChairID() == m_wCurrentBanker)
  {
    return true;
  }


  //状态判断
  if(GetGameStatus()!=GS_PLACE_JETTON)
  {
    m_GameClientView.m_lCurrentJetton = 0;
    UpdateButtonContron();
    return true;
  }

  //设置变量
  //m_lUserJettonScore[cbJettonArea] += lJettonScore;
  m_GameClientView.SetMePlaceJetton(cbJettonArea, lJettonScore);


  //变量定义
  CMD_C_PlaceJetton PlaceJetton;
  ZeroMemory(&PlaceJetton,sizeof(PlaceJetton));

  //构造变量
  PlaceJetton.cbJettonArea=cbJettonArea;
  PlaceJetton.lJettonScore=lJettonScore;
  m_bAddScore=true;
  //发送消息
  if(!SendSocketData(SUB_C_PLACE_JETTON,&PlaceJetton,sizeof(PlaceJetton)))
  {
    return 0;
  };

  //更新按钮
  UpdateButtonContron(true);

  //预先显示
  m_GameClientView.PlaceUserJetton(cbJettonArea,lJettonScore);

  //播放声音

  if(lJettonScore==5000000)
  {
    PlayGameSound(AfxGetInstanceHandle(),TEXT("ADD_GOLD_EX"));
  }
  else
  {
    PlayGameSound(AfxGetInstanceHandle(),TEXT("ADD_GOLD"));
  }



  return 0;

}
//续投
LRESULT CGameClientEngine::OnLastPlaceJetton(WPARAM wParam, LPARAM lParam)
{
  if(GetGameStatus() == GS_PLACE_JETTON)
  {
    //变量定义
    CMD_C_ContinueJetton PlaceLastJetton;
    ZeroMemory(&PlaceLastJetton,sizeof(PlaceLastJetton));

    //构造变量
    PlaceLastJetton.wChairID = GetMeChairID();
    //发送消息
    if(!SendSocketData(SUB_C_CONTINUE_JETTON,&PlaceLastJetton,sizeof(PlaceLastJetton)))
    {
      return 0;
    };

    return 0;
  }

  return 0;
}



//自己最大下分
LONGLONG CGameClientEngine::GetMeMaxBet(BYTE cbArea)
{
  //找出玩家
  IClientUserItem * pUserData = GetTableUserItem(GetMeChairID());
  if(pUserData == NULL || cbArea > AREA_COUNT)
  {
    return 0l;
  }

  //玩家分数
  LONGLONG lUserScore = pUserData->GetUserScore();

  //减去自己已下注
  for(int i = 0; i < AREA_COUNT; ++i)
  {
    lUserScore -= m_lUserJettonScore[i];
  }

  //如果是区域下注. 还要判断区域限制
  if(cbArea < AREA_COUNT)
  {
    //区域总限制
    LONGLONG lAreaLimit = m_lAreaLimitScore - m_lPlayerBetAll[cbArea];
    lUserScore = __min(lUserScore, lAreaLimit);
  }

  //异常错误限制
  ASSERT(lUserScore >= 0l);
  if(lUserScore < 0)
  {
    lUserScore = 0;
  }

  return lUserScore;
}

LRESULT  CGameClientEngine::OnPlaySound(WPARAM wParam, LPARAM lParam)
{

  if(lParam == 0)
  {
    PlayGameSound(AfxGetInstanceHandle(),TEXT("PLEASEJETTONWAV"));
  }
  else if(lParam==1)
  {
    //PlayGameSound(AfxGetInstanceHandle(),TEXT("LEAVEHANDLEWAV"));
  }
  else if(lParam==3)
  {
    PlayGameSound(AfxGetInstanceHandle(),TEXT("IDC_SNDWAV"));
  }
  else if(lParam==4)
  {
    PlayGameSound(AfxGetInstanceHandle(),TEXT("END_LOST"));
  }
  else if(lParam==5)
  {
    PlayGameSound(AfxGetInstanceHandle(),TEXT("END_WIN"));
  }
  else if(lParam==6)
  {
    PlayGameSound(AfxGetInstanceHandle(),TEXT("END_DRAW"));
  }
  else if(lParam==7)
  {
    PlayGameSound(AfxGetInstanceHandle(),TEXT("ADD_GOLD"));
  }



  return 1;

}


LRESULT  CGameClientEngine::OnAnimalSound(WPARAM wParam, LPARAM lParam)
{
  CString strSoundName;
  strSoundName.Format(TEXT("ANIMAL_SOUND_%d"),(BYTE)wParam+1);

  PlayGameSound(AfxGetInstanceHandle(),strSoundName);

  return 0;

}
//申请消息
LRESULT CGameClientEngine::OnApplyBanker(WPARAM wParam, LPARAM lParam)
{
  //合法判断
  IClientUserItem * pClientUserItem=GetTableUserItem(GetMeChairID());

  if(pClientUserItem->GetUserScore() < m_lApplyBankerCondition)
  {
    return true;
  }

  //旁观判断
  if(IsLookonMode())
  {
    return true;
  }

  //转换变量
  bool bApplyBanker = wParam ? true:false;

  //当前判断
  if(m_wCurrentBanker == GetMeChairID() && bApplyBanker)
  {
    return true;
  }

  if(bApplyBanker)
  {
    //发送消息
    SendSocketData(SUB_C_APPLY_BANKER, NULL, 0);

    //m_bMeApplyBanker=true;
  }
  else
  {
    //发送消息
    SendSocketData(SUB_C_CANCEL_BANKER, NULL, 0);

    //m_bMeApplyBanker=false;
  }

  //设置按钮
  UpdateButtonContron();

  return true;
}

//申请做庄
bool CGameClientEngine::OnSubUserApplyBanker(const void * pBuffer, WORD wDataSize)
{
  try
  {
    //效验数据
    ASSERT(wDataSize==sizeof(CMD_S_ApplyBanker));
    if(wDataSize!=sizeof(CMD_S_ApplyBanker))
    {
      return false;
    }
  }
  catch(...)
  {
    //AfxMessageBox(TEXT("OnSubUserApplyBanker(const void * pBuffer, WORD wDataSize) Error"));
  }

  return true;
}

//取消做庄
bool CGameClientEngine::OnSubUserCancelBanker(const void * pBuffer, WORD wDataSize)
{
  try
  {
    //效验数据
    ASSERT(wDataSize==sizeof(CMD_S_CancelBanker));
    if(wDataSize!=sizeof(CMD_S_CancelBanker))
    {
      return false;
    }
  }
  catch(...)
  {
    //AfxMessageBox(TEXT("OnSubUserCancelBanker(const void * pBuffer, WORD wDataSize) Error"));
  }

  return true;
}

//切换庄家
bool CGameClientEngine::OnSubChangeBanker(const void * pBuffer, WORD wDataSize)
{
  try
  {
    //效验数据
    ASSERT(wDataSize==sizeof(CMD_S_ChangeBanker));
    if(wDataSize!=sizeof(CMD_S_ChangeBanker))
    {
      return false;
    }
  }
  catch(...)
  {
    //AfxMessageBox(TEXT("OnSubChangeBanker(const void * pBuffer, WORD wDataSize) Error"));
  }

  return true;
}

//游戏记录
bool CGameClientEngine::OnSubGameRecord(const void * pBuffer, WORD wDataSize)
{
  //效验参数
  ASSERT(wDataSize%sizeof(tagServerGameRecord)==0);
  if(wDataSize%sizeof(tagServerGameRecord)!=0)
  {
    return false;
  }
  //结果变量
  tagGameRecord GameRecord;
  ZeroMemory(&GameRecord,sizeof(GameRecord));

  //设置记录
  WORD wRecordCount=wDataSize/sizeof(tagServerGameRecord);
  for(WORD wIndex=0; wIndex<wRecordCount; wIndex++)
  {
    tagServerGameRecord * pServerGameRecord=(((tagServerGameRecord *)pBuffer)+wIndex);

    m_GameClientView.SetGameHistory(pServerGameRecord->bWinMen);
  }

  return true;
}

//下注失败
bool CGameClientEngine::OnSubPlaceJettonFail(const void * pBuffer, WORD wDataSize)
{
  try
  {
    //效验数据
    ASSERT(wDataSize==sizeof(CMD_S_PlaceJettonFail));
    if(wDataSize!=sizeof(CMD_S_PlaceJettonFail))
    {
      return false;
    }

    //消息处理
    CMD_S_PlaceJettonFail * pPlaceJettonFail=(CMD_S_PlaceJettonFail *)pBuffer;
    //效验参数
    BYTE cbViewIndex=pPlaceJettonFail->lJettonArea;
    ASSERT(cbViewIndex>=1 && cbViewIndex<=AREA_COUNT);
    if(!(cbViewIndex>=1 && cbViewIndex<=AREA_COUNT))
    {
      return false;
    }

    //加注界面
    m_GameClientView.PlaceUserJetton(pPlaceJettonFail->lJettonArea,-pPlaceJettonFail->lPlaceScore);

    //自己判断
    if(GetMeChairID()==pPlaceJettonFail->wPlaceUser && false==IsLookonMode())
    {
      LONGLONG lJettonCount=pPlaceJettonFail->lPlaceScore;
      //合法校验
      ASSERT(m_lUserJettonScore[cbViewIndex]>=lJettonCount);
      if(lJettonCount>m_lUserJettonScore[cbViewIndex])
      {
        return false;
      }

      //设置下注
      m_lUserJettonScore[cbViewIndex]-=lJettonCount;
      m_GameClientView.SetMePlaceJetton(cbViewIndex,m_lUserJettonScore[cbViewIndex]);

      m_GameClientView.m_bAddScoreStatue=false;
    }
  }
  catch(...)
  {
    //AfxMessageBox(TEXT("下注失败"));
  }

  return true;
}

//设置庄家
void CGameClientEngine::SetBankerInfo(WORD wBanker,LONGLONG lScore)
{
  try
  {
    m_wCurrentBanker=wBanker;
    m_lBankerScore=lScore;
    IClientUserItem * pClientUserItem=m_wCurrentBanker==INVALID_CHAIR ? NULL : GetTableUserItem(m_wCurrentBanker);
    DWORD dwBankerUserID = (NULL==pClientUserItem) ? 0 : pClientUserItem->GetUserID();
    m_GameClientView.SetBankerInfo(dwBankerUserID,m_lBankerScore);
  }
  catch(...)
  {
    //AfxMessageBox(TEXT("设置庄家 异常"));
  }

}

//个人下注
void CGameClientEngine::SetMePlaceJetton(BYTE cbViewIndex, LONGLONG lJettonCount)
{
  //合法判断
  ASSERT(cbViewIndex>=1 && cbViewIndex<=AREA_COUNT);
  if(!(cbViewIndex>=1 && cbViewIndex<=AREA_COUNT))
  {
    return;
  }

  //设置变量
  m_lUserJettonScore[cbViewIndex]=lJettonCount;

  //设置界面
  m_GameClientView.SetMePlaceJetton(cbViewIndex,lJettonCount);
}
//////////////////////////////////////////////////////////////////////////
//控制
LRESULT CGameClientEngine::OnAdminControl(WPARAM wParam, LPARAM lParam)
{
  try
  {
    SendSocketData(SUB_C_ADMIN_COMMDN,(CMD_C_ControlApplication*)wParam,sizeof(CMD_C_ControlApplication));
  }
  catch(...)
  {
    //AfxMessageBox(TEXT("OnAdminControl ( WPARAM wParam, LPARAM lParam ) 异常"));
  }

  return true;
}

//控制
bool CGameClientEngine::OnSubAdminControl(const void * pBuffer, WORD wDataSize)
{
  try
  {
    ASSERT(wDataSize==sizeof(CMD_S_ControlReturns));
    if(wDataSize!=sizeof(CMD_S_ControlReturns))
    {
      return false;
    }

    if(m_GameClientView.m_pAdminControl!=NULL)
    {
      CMD_S_ControlReturns* pResult = (CMD_S_ControlReturns*)pBuffer;
      m_GameClientView.m_pAdminControl->UpdateControl(pResult);
    }

  }
  catch(...)
  {
    //AfxMessageBox(TEXT("OnSubAdminControl( const void * pBuffer, WORD wDataSize ) 异常"));
  }

  return true;
}

void CGameClientEngine::OnTimer(UINT nIDEvent)
{
  try
  {
    // TODO: 在此添加消息处理程序代码和/或调用默认值
    if(nIDEvent == IDI_ANDROID_BET)
    {
      if(m_ListAndroid.GetCount() != 0)
      {
        POSITION pos = m_ListAndroid.GetTailPosition();
        POSITION posTmp = pos;

        //遍历下注链表
        while(true)
        {
          if(pos == 0)
          {
            break;
          }

          posTmp = pos;
          tagAndroidBet & androidBet = m_ListAndroid.GetPrev(pos);

          androidBet.nLeftTime -= 100;
          if(androidBet.nLeftTime <= 0)
          {
            //模拟消息
            CMD_S_PlaceJetton placeJetton = {};
            placeJetton.cbAndroid = FALSE;
            //placeJetton.cbJettonArea = androidBet.cbJettonArea;
            //placeJetton.lJettonScore = androidBet.lJettonScore;
            placeJetton.wChairID = androidBet.wChairID;

            OnSubPlaceJetton((void*)&placeJetton, sizeof(placeJetton));

            //删除元素
            m_ListAndroid.RemoveAt(posTmp);
          }
        }
      }
    }
    CGameFrameEngine::OnTimer(nIDEvent);
  }
  catch(...)
  {
    //AfxMessageBox(TEXT("定时器异常 OnTimer"));
  }
}


void CGameClientEngine::ReSetBankCtrol(int nGameState)
{

  if(!IsLookonMode())
  {
    switch(GetGameStatus())
    {
      case GAME_STATUS_FREE:
      {
        if(GetMeChairID()!=m_wCurrentBanker)
        {
          m_GameClientView.m_btBankDraw.EnableWindow(TRUE);
          m_GameClientView.m_btBankStorage.EnableWindow(TRUE);
        }
        else
        {
          m_GameClientView.m_btBankDraw.EnableWindow(TRUE);
          m_GameClientView.m_btBankStorage.EnableWindow(FALSE);
        }
        break;
      }
      case GS_GAME_END:
      case GS_PLACE_JETTON:
      case GS_MOVECARD_END:
      {
        if(GetMeChairID()!=m_wCurrentBanker)
        {
          m_GameClientView.m_btBankDraw.EnableWindow(TRUE);
          m_GameClientView.m_btBankStorage.EnableWindow(FALSE);
        }
        else
        {
          m_GameClientView.m_btBankDraw.EnableWindow(FALSE);
          m_GameClientView.m_btBankStorage.EnableWindow(FALSE);
        }
        break;
      }
      default:
        break;
    }
  }
  else
  {
    m_GameClientView.m_btBankDraw.EnableWindow(TRUE);
    m_GameClientView.m_btBankStorage.EnableWindow(TRUE);
  }

}

//用户进入
VOID  CGameClientEngine::OnEventUserEnter(IClientUserItem * pIClientUserItem, bool bLookonUser)
{
  try
  {
    //设置成绩
    if(bLookonUser==false)
    {
      //更新界面
      m_GameClientView.RefreshGameView();
    }

    return;
  }
  catch(...)
  {
    //AfxMessageBox(TEXT("用户进入异常"));
  }
}

//用户离开
VOID  CGameClientEngine::OnEventUserLeave(IClientUserItem * pIClientUserItem, bool bLookonUser)
{
  try
  {
    //设置成绩
    if(bLookonUser==false)
    {


      WORD wViewChairId = SwitchViewChairID(pIClientUserItem->GetChairID());

      //更新界面
      m_GameClientView.RefreshGameView();
    }

    return;
  }
  catch(...)
  {
    //AfxMessageBox(TEXT("用户离开异常"));
  }

}

//用户状态
VOID  CGameClientEngine::OnEventUserStatus(IClientUserItem * pIClientUserItem, bool bLookonUser)
{
  try
  {
    //变量定义
    IClientUserItem * pIMySelfUserItem=GetMeUserItem();
    BYTE cbUserStatus=pIClientUserItem->GetUserStatus();
    return;
  }
  catch(...)
  {
    //AfxMessageBox(TEXT("用户状态异常"));
  }
}

//选择事件
VOID  CGameClientEngine::OnEventPhraseSelect(INT nPhraseIndex)
{
  try
  {
  }
  catch(...)
  {
    //AfxMessageBox(TEXT("选择事件异常"));
  }
}

//声音控制
bool CGameClientEngine::AllowBackGroundSound(bool bAllowSound)
{
  try
  {
    if(bAllowSound)
    {
      PlayBackGroundSound(AfxGetInstanceHandle(),TEXT("GAME_FREE"));
    }
    else
    {
      StopSound();
    }


  }
  catch(...)
  {
    //AfxMessageBox(TEXT("声音控制异常"));
  }

  return true;
}

//更新库存
LRESULT CGameClientEngine::OnUpdateStorage(WPARAM wParam,LPARAM lParam)
{
  SendSocketData(SUB_C_UPDATE_STORAGE,(CMD_C_UpdateStorage*)wParam,sizeof(CMD_C_UpdateStorage));
  return true;
}

//更新库存
bool CGameClientEngine::OnSubUpdateStorage(const void * pBuffer, WORD wDataSize)
{
  ASSERT(wDataSize==sizeof(CMD_S_UpdateStorage));
  if(wDataSize!=sizeof(CMD_S_UpdateStorage))
  {
    return false;
  }

  if(NULL != m_GameClientView.m_pAdminControl && NULL != m_GameClientView.m_pAdminControl->GetSafeHwnd())
  {
    m_GameClientView.m_pAdminControl->UpdateStorage(pBuffer);
  }

  return true;
}



