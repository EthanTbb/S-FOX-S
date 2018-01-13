#include "Stdafx.h"
#include "GameClient.h"
//#include "GameOption.h"
#include "GameClientEngine.h"
#include ".\GameClientEngine.h"

//////////////////////////////////////////////////////////////////////////

//ʱ���ʶ
#define IDI_FREE          99                  //����ʱ��
#define IDI_PLACE_JETTON      100                 //��עʱ��
#define IDI_DISPATCH_CARD     301                 //����ʱ��
#define IDI_OPEN_CARD       302                   //����ʱ��

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
//���캯��
CGameClientEngine::CGameClientEngine()
{
  try
  {
    //������Ϣ
    m_lMeMaxScore=0L;
    m_lAreaLimitScore=0L;
    m_lApplyBankerCondition=0L;
    m_GameEndTime = 0;

    m_blUsing = false;

    m_bAddScore=false;

    //������ע
    ZeroMemory(m_lUserJettonScore,sizeof(m_lUserJettonScore));
    ZeroMemory(m_lPlayerBetAll,sizeof(m_lPlayerBetAll));
    //���������ע
    ZeroMemory(m_lAllPlayBet,sizeof(m_lAllPlayBet));
    //ׯ����Ϣ
    m_lBankerScore=0L;
    m_wCurrentBanker=0L;

    //״̬����
    m_bMeApplyBanker=false;

    //������
    for(int i = 0; i < CountArray(m_nAnimalPercent); ++i)
    {
      m_nAnimalPercent[i] = 1;
    }

    m_bPlaceEnd = false;
  }
  catch(...)
  {
    //AfxMessageBox(TEXT("���湹��Error"));
  }

  return;
}

//��������
CGameClientEngine::~CGameClientEngine()
{
}

//��ʼ����
bool CGameClientEngine::OnInitGameEngine()
{

  try
  {
    CGlobalUnits * m_pGlobalUnits=(CGlobalUnits *)CGlobalUnits::GetInstance();
    ASSERT(m_pGlobalUnits!=NULL);

    //���ñ���
    //SetWindowText(TEXT("����������Ϸ  --  Ver��6.6.0.3"));

    //����ͼ��
    HICON hIcon=LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME));
    m_pIClientKernel->SetGameAttribute(KIND_ID,GAME_PLAYER,VERSION_CLIENT,hIcon,GAME_NAME);
    SetIcon(hIcon,TRUE);
    SetIcon(hIcon,FALSE);
  }
  catch(...)
  {
    //AfxMessageBox(TEXT("�����ʼ�� Error"));
  }

  return true;
}

//���ÿ��
bool CGameClientEngine::OnResetGameEngine()
{
  //������Ϣ
  m_lMeMaxScore=0L;

  //������ע
  ZeroMemory(m_lUserJettonScore,sizeof(m_lUserJettonScore));
  ZeroMemory(m_lPlayerBetAll,sizeof(m_lPlayerBetAll));


  //ׯ����Ϣ
  m_lBankerScore=0L;
  m_wCurrentBanker=0L;
  //״̬����
  m_bMeApplyBanker=false;
  m_bAddScore=false;
  return true;
}

//��Ϸ����
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

//ʱ��ɾ��
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

//�Թ�״̬
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

//ʱ����Ϣ
bool CGameClientEngine::OnEventGameClockInfo(WORD wChairID, UINT nElapse, WORD nTimerID)
{
  if((nTimerID==IDI_PLACE_JETTON)&&(nElapse<2))
  {
    //��ֹ��ť
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
    strInfo=TEXT("���ֽ���,�ɼ�ͳ��:");
    m_pIStringMessage->InsertSystemString(strInfo);
    strInfo.Format(TEXT("ׯ��:%I64d"),m_bankerScore);
    m_pIStringMessage->InsertNormalString(strInfo);
    strInfo.Format(TEXT("����:%I64d"),m_userScore);
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

////�Թ�״̬
//void CGameClientEngine::OnEventLookonMode(VOID * pData, WORD wDataSize)
//{
//}

//������Ϣ
bool CGameClientEngine::OnEventGameMessage(WORD wSubCmdID, VOID * pBuffer, WORD wDataSize)
{
  try
  {
    switch(wSubCmdID)
    {
      case SUB_S_GAME_FREE:   //��Ϸ����
      {
        return OnSubGameFree(pBuffer,wDataSize);
      }
      case SUB_S_GAME_START:    //��Ϸ��ʼ
      {
        return OnSubGameStart(pBuffer,wDataSize);
      }
      case SUB_S_PLACE_JETTON:  //�û���ע
      {
        return OnSubPlaceJetton(pBuffer,wDataSize);
      }
      case SUB_S_APPLY_BANKER:  //������ׯ
      {
        return OnSubUserApplyBanker(pBuffer, wDataSize);
      }
      case SUB_S_CANCEL_BANKER: //ȡ����ׯ
      {
        return OnSubUserCancelBanker(pBuffer, wDataSize);
      }
      case SUB_S_CHANGE_BANKER: //�л�ׯ��
      {
        return OnSubChangeBanker(pBuffer, wDataSize);
      }
      case SUB_S_GAME_END:    //��Ϸ����
      {
        return OnSubGameEnd(pBuffer,wDataSize);
      }
      case SUB_S_SEND_RECORD:   //��Ϸ��¼
      {
        return OnSubGameRecord(pBuffer,wDataSize);
      }
      case SUB_S_PLACE_JETTON_FAIL: //��עʧ��
      {
        return OnSubPlaceJettonFail(pBuffer,wDataSize);
      }
      /*case SUB_S_CHECK_IMAGE:
        {
          return OnSubCheckImageIndex(pBuffer,wDataSize);
        }*/
      case SUB_S_CEAN_JETTON:   //���
      {
        //Ч������
        ASSERT(wDataSize==sizeof(CMD_S_CeanJetton));
        if(wDataSize!=sizeof(CMD_S_CeanJetton))
        {
          return false;
        }

        //��Ϣ����
        CMD_S_CeanJetton * pCleanJetton=(CMD_S_CeanJetton *)pBuffer;

        ZeroMemory(m_GameClientView.m_lAllJettonScore,sizeof(m_GameClientView.m_lAllJettonScore));

        //������ע
        for(int nAreaIndex=0; nAreaIndex<AREA_COUNT+1; nAreaIndex++)
        {
          m_GameClientView.m_lAllJettonScore[nAreaIndex] += pCleanJetton->lAllCPlaceScore[nAreaIndex];
        }

        m_GameClientView.RefreshGameView();
        return true;
      }
      case SUB_S_ADMIN_COMMDN:    //����
      {
        return OnSubAdminControl(pBuffer,wDataSize);
      }
      case SUB_S_UPDATE_STORAGE:  //���¿��
      {
        return OnSubUpdateStorage(pBuffer,wDataSize);
      }
      case SUB_S_CONTINU_JETTON:
      {
        //Ч������
        ASSERT(wDataSize==sizeof(CMD_S_ContiueJetton));
        if(wDataSize!=sizeof(CMD_S_ContiueJetton))
        {
          return false;
        }


        //��Ϣ����
        CMD_S_ContiueJetton * pLastJetton=(CMD_S_ContiueJetton *)pBuffer;

        //ȡ���Ͼ���ע��Ϣ Ϊ��ǰ ��ע��Ϣ
        //if (GetMeChairID()==pLastJetton->wChairID || IsLookonMode())
        {

          //�����˻������ػ�������ע
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

          //��������
          PlayGameSound(AfxGetInstanceHandle(),TEXT("ADD_GOLD"));

          m_GameClientView.RefreshGameView();
        }


        return true;
      }
    }

    //�������
    ASSERT(FALSE);
  }
  catch(...)
  {
    //AfxMessageBox(TEXT("������Ϣ�쳣"));
  }

  return true;
}

//��Ϸ����
bool CGameClientEngine::OnEventSceneMessage(BYTE cbGameStation, bool bLookonOther, VOID * pBuffer, WORD wDataSize)
{
  //SetGameStatus(GAME_STATUS_FREE);
  try
  {
    switch(cbGameStation)
    {
      case GAME_STATUS_FREE:      //����״̬
      {
        //Ч������
        ASSERT(wDataSize==sizeof(CMD_S_StatusFree));
        if(wDataSize!=sizeof(CMD_S_StatusFree))
        {
          return false;
        }

        m_GameClientView.m_bGameStatus = 1;
        //��Ϣ����
        CMD_S_StatusFree * pStatusFree=(CMD_S_StatusFree *)pBuffer;
        m_bEnableSysBanker=pStatusFree->bEnableSysBanker;

        m_GameClientView.m_bDrawYingZi =false;
        m_GameClientView.m_lCurStorage = pStatusFree->lBonus;

        //�����Ϣ
        m_lMeMaxScore=pStatusFree->lUserMaxScore;
        m_GameClientView.SetMeMaxScore(m_lMeMaxScore);
        //IClientUserItem *pMeUserData=GetTableUserItem(GetMeChairID());

        IClientUserItem *pMeUserData=GetTableUserItem(GetMeChairID());

        m_GameClientView.SetMeChairID(pMeUserData->GetUserID());

        //ׯ����Ϣ
        SetBankerInfo(pStatusFree->wBankerUser,pStatusFree->lBankerScore);
        m_GameClientView.SetBankerScore(pStatusFree->cbBankerTime,pStatusFree->lBankerWinScore);

        m_GameClientView.EnableSysBanker(m_bEnableSysBanker);

        //������Ϣ
        m_lApplyBankerCondition=pStatusFree->lApplyBankerCondition;
        m_lAreaLimitScore=pStatusFree->lAreaLimitScore;
        m_GameClientView.SetAreaLimitScore(m_lAreaLimitScore);


        //���ü���
        memcpy(m_nAnimalPercent, pStatusFree->nAnimalPercent, sizeof(m_nAnimalPercent));
        m_GameClientView.SetAnimalPercent(m_nAnimalPercent);

        //����
        //if((GetTableUserItem(GetMeChairID())->dwUserRight&UR_GAME_CONTROL)!=0&&m_GameClientView.m_pAdminControl!=NULL)
        //  m_GameClientView.m_btOpenAdmin.ShowWindow(SW_SHOW);

        //����
        if(CUserRight::IsGameCheatUser(m_pIClientKernel->GetUserAttribute()->dwUserRight)&&m_GameClientView.m_hInst)
        {
          m_GameClientView.m_btOpenAdmin.ShowWindow(SW_SHOW);
        }

        //����״̬
        SetGameStatus(GAME_STATUS_FREE);

        //���¿���
        UpdateButtonContron();
        m_GameClientView.RefreshGameView();

        //����ʱ��
        SetGameClock(GetMeChairID(),IDI_FREE,pStatusFree->cbTimeLeave);

        //��������
        PlayBackGroundSound(AfxGetInstanceHandle(),TEXT("GAME_FREE"));

        return true;
      }
      case GS_PLACE_JETTON: //��Ϸ״̬
      case GS_GAME_END:   //����״̬
      {

        //Ч������
        ASSERT(wDataSize==sizeof(CMD_S_StatusPlay));
        if(wDataSize!=sizeof(CMD_S_StatusPlay))
        {
          return false;
        }

        m_GameClientView.m_bGameStatus = 1;
        //��Ϣ����
        CMD_S_StatusPlay * pStatusPlay=(CMD_S_StatusPlay *)pBuffer;

        //��ע��Ϣ
        for(int nAreaIndex=1; nAreaIndex<=AREA_COUNT; ++nAreaIndex)
        {
          m_GameClientView.PlaceUserJetton(nAreaIndex,pStatusPlay->lAllJettonScore[nAreaIndex]);
          SetMePlaceJetton(nAreaIndex,pStatusPlay->lUserJettonScore[nAreaIndex]);
        }

        m_GameClientView.m_bDrawYingZi =false;
        m_GameClientView.m_lCurStorage = pStatusPlay->lBonus;

        //��һ���
        m_lMeMaxScore=pStatusPlay->lUserMaxScore;
        m_GameClientView.SetMeMaxScore(m_lMeMaxScore);
        IClientUserItem *pMeUserData=GetTableUserItem(GetMeChairID());
        m_GameClientView.SetMeChairID(pMeUserData->GetUserID());

        //������Ϣ
        m_lApplyBankerCondition=pStatusPlay->lApplyBankerCondition;
        m_lAreaLimitScore=pStatusPlay->lAreaLimitScore;
        m_GameClientView.SetAreaLimitScore(m_lAreaLimitScore);

        if(pStatusPlay->cbGameStatus==GS_GAME_END)
        {
          //�˿���Ϣ
          m_GameClientView.SetCardInfo(pStatusPlay->cbTableCardArray);
          m_GameClientView.FinishDispatchCard(false);

          //���óɼ�
          m_GameClientView.SetCurGameScore(pStatusPlay->lEndUserScore,pStatusPlay->lEndUserReturnScore,pStatusPlay->lEndBankerScore,pStatusPlay->lEndRevenue);
        }
        else
        {
          m_GameClientView.SetCardInfo(NULL);
          m_blUsing = true;

        }

        //����
        if(CUserRight::IsGameCheatUser(m_pIClientKernel->GetUserAttribute()->dwUserRight)&&m_GameClientView.m_hInst)
        {
          m_GameClientView.m_btOpenAdmin.ShowWindow(SW_SHOW);
        }

        //��������
        PlayBackGroundSound(AfxGetInstanceHandle(),TEXT("GAME_FREE"));

        //ׯ����Ϣ
        SetBankerInfo(pStatusPlay->wBankerUser,pStatusPlay->lBankerScore);
        m_GameClientView.SetBankerScore(pStatusPlay->cbBankerTime,pStatusPlay->lBankerWinScore);
        m_bEnableSysBanker=pStatusPlay->bEnableSysBanker;
        m_GameClientView.EnableSysBanker(m_bEnableSysBanker);

        //���ü���
        memcpy(m_nAnimalPercent, pStatusPlay->nAnimalPercent, sizeof(m_nAnimalPercent));
        m_GameClientView.SetAnimalPercent(m_nAnimalPercent);

        //����
        //if((GetTableUserItem(GetMeChairID())->dwUserRight&UR_GAME_CONTROL)!=0&&m_GameClientView.m_pAdminControl!=NULL)
        //  m_GameClientView.m_btOpenAdmin.ShowWindow(SW_SHOW);

        //���°�ť
        UpdateButtonContron();

        //����ʱ��
        UINT nTimerID = pStatusPlay->cbGameStatus==GS_GAME_END ? IDI_OPEN_CARD : IDI_PLACE_JETTON;
        SetGameClock(GetMeChairID(), nTimerID, pStatusPlay->cbTimeLeave);

        //����״̬
        SetGameStatus(pStatusPlay->cbGameStatus);

        m_GameClientView.StartRandShowSide();

        m_GameClientView.RefreshGameView();
        return true;
      }
    }


  }
  catch(...)
  {
    //AfxMessageBox(TEXT("������Ϣ�쳣"));
  }

  return false;
}

//��Ϸ��ʼ
bool CGameClientEngine::OnSubGameStart(const void * pBuffer, WORD wDataSize)
{
  try
  {
    //Ч������
    ASSERT(wDataSize==sizeof(CMD_S_GameStart));
    if(wDataSize!=sizeof(CMD_S_GameStart))
    {
      return false;
    }

    m_GameClientView.m_bGameStatus = 1;

    m_bPlaceEnd = false;
    //��Ϣ����
    CMD_S_GameStart * pGameStart=(CMD_S_GameStart *)pBuffer;

    m_GameClientView.SwithToNormalView();

    m_GameClientView.KillCardTime();

    m_blUsing = true;

    m_GameClientView.m_lCurStorage = pGameStart->lBonus;

    //ׯ����Ϣ
    SetBankerInfo(pGameStart->wBankerUser,pGameStart->lBankerScore);

    //�����Ϣ
    m_lMeMaxScore=pGameStart->lUserMaxScore;
    m_GameClientView.SetMeMaxScore(m_lMeMaxScore);

    //����ʱ��
    SetGameClock(GetMeChairID(),IDI_PLACE_JETTON,pGameStart->cbTimeLeave);

    //��ע״̬
    m_GameClientView.m_bAddScoreStatue=false;

    //����״̬
    SetGameStatus(GS_PLACE_JETTON);

    m_GameClientView.StartRandShowSide();

    SetTimer(IDI_ANDROID_BET, 100, NULL);

    //���¿���
    UpdateButtonContron(true);



    OnPlaySound(0,0);

  }
  catch(...)
  {
    //AfxMessageBox(TEXT("��ʼ��Ϸ�쳣"));
  }

  return true;
}

//��Ϸ����
bool CGameClientEngine::OnSubGameFree(const void * pBuffer, WORD wDataSize)
{
  //Ч������
  ASSERT(wDataSize==sizeof(CMD_S_GameFree));
  if(wDataSize!=sizeof(CMD_S_GameFree))
  {
    return false;
  }

  //��Ϣ����
  CMD_S_GameFree * pGameFree=(CMD_S_GameFree *)pBuffer;

  m_blUsing = true;

  //����ʱ��
  SetGameClock(GetMeChairID(),IDI_FREE,pGameFree->cbTimeLeave);

  //m_GameClientView.StartMove();

  //����״̬
  SetGameStatus(GAME_STATUS_FREE);
  //����ǹ���Ա��ÿ�ַ��Ϳ����
  CString cs;
  cs.Format(TEXT("��ǰ�����Ϊ��%I64d"),pGameFree->lStorageStart);
  if(CUserRight::IsGameCheatUser(m_pIClientKernel->GetUserAttribute()->dwUserRight)&&m_GameClientView.m_hInst)
  {

    m_GameClientView.m_pAdminControl->SetStorageCurrentValue(pGameFree->lStorageStart, pGameFree->lBonus);
    if(m_pIStringMessage!=NULL)
    {
      m_pIStringMessage->InsertSystemString(cs);
    }
  }

  m_GameClientView.m_lCurStorage = pGameFree->lBonus;

  //����ʱ��
  KillTimer(IDI_ANDROID_BET);

  ////��������
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
  //���������ע
  ZeroMemory(m_lAllPlayBet,sizeof(m_lAllPlayBet));


  m_GameClientView.SwitchToCheck();

  //PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_FREE"));

  //���¿ؼ�
  UpdateButtonContron();

  return true;
}

//�û���ע
bool CGameClientEngine::OnSubPlaceJetton(const void * pBuffer, WORD wDataSize)
{
  //Ч������
  ASSERT(wDataSize==sizeof(CMD_S_PlaceJetton));
  if(wDataSize!=sizeof(CMD_S_PlaceJetton))
  {
    return false;
  }

  //��Ϣ����
  CMD_S_PlaceJetton * pPlaceJetton=(CMD_S_PlaceJetton *)pBuffer;
  if(GetMeChairID()!=pPlaceJetton->wChairID || IsLookonMode())
  {

//    //�����˻������ػ�������ע
//    if(CUserRight::IsGameCheatUser(m_pIClientKernel->GetUserAttribute()->dwUserRight))
//    {
//      if(pPlaceJetton->cbAndroid == TRUE)
//      {
//        return true;
//      }
//    }

    m_GameClientView.PlaceUserJetton(pPlaceJetton->cbJettonArea,pPlaceJetton->lJettonScore);

    //��������
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

  //��ע״̬
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

    //��������
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

    //��������
    pUserBetArray->Add(UserBet);
  }

  //���¿ؼ�
  m_GameClientView.m_pAdminControl->UpdateUserBet(false);

  //UpdateButtonContron();

  return true;
}

//��Ϸ����
bool CGameClientEngine::OnSubGameEnd(const void * pBuffer, WORD wDataSize)
{
  //Ч������
  ASSERT(wDataSize==sizeof(CMD_S_GameEnd));
  if(wDataSize!=sizeof(CMD_S_GameEnd))
  {
    return false;
  }

  //��Ϣ����
  CMD_S_GameEnd * pGameEnd=(CMD_S_GameEnd *)pBuffer;

  //m_GameClientView.m_lCurStorage = pGameEnd->lStorageStart;
  m_GameClientView.m_blMoveFinish = false;


  m_GameEndTime = pGameEnd->cbTimeLeave;

  m_blUsing = true;

  //����ʱ��
  SetGameClock(GetMeChairID(),IDI_DISPATCH_CARD, pGameEnd->cbTimeLeave);

  //�˿���Ϣ
  m_GameClientView.SetCardInfo(pGameEnd->cbTableCardArray);
  m_GameClientView.ClearAreaFlash();

  m_GameClientView.SetShaYuAddMulti(pGameEnd->cbShaYuAddMulti);

  m_GameClientView.StartRunCar(20);

  //���漸��
  memcpy(m_nAnimalPercent, pGameEnd->nAnimalPercent, sizeof(m_nAnimalPercent));


  memcpy(m_GameClientView.m_lUserLastJetton, pGameEnd->lLastJetton, sizeof(pGameEnd->lLastJetton));

  //����״̬
  SetGameStatus(GS_GAME_END);

  //ׯ����Ϣ
  m_GameClientView.SetBankerScore(pGameEnd->nBankerTime, pGameEnd->lBankerTotallScore);
  //�ɼ���Ϣ
  m_GameClientView.SetCurGameScore(pGameEnd->lUserScore,pGameEnd->lUserReturnScore,pGameEnd->lBankerScore,pGameEnd->lRevenue);
  m_bankerScore=pGameEnd->lBankerScore;
  m_userScore=pGameEnd->lUserScore;
  m_GameClientView.PerformAllBetAnimation();

  //���¿ؼ�
  UpdateButtonContron();

  return true;
}

//���¿���
void CGameClientEngine::UpdateButtonContron(bool bCanPlaceJetton)
{

  //�����ж�
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

  //��ע��ť
  if(bEnablePlaceJetton==true)
  {

    //�������
    LONGLONG lCurrentJetton=m_GameClientView.GetCurrentJetton();
    LONGLONG lLeaveScore=m_lMeMaxScore;

    for(int nAreaIndex=1; nAreaIndex<=AREA_COUNT; ++nAreaIndex)
    {
      lLeaveScore -= m_lUserJettonScore[nAreaIndex];
    }

    //�����ע
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
    lLeaveScore = min((lLeaveScore),lUserMaxJetton); //�û����·� �����ֱȽ� �������屶

    //���ư�ť
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

    //���ٳ���
    if(lCurrentJetton>lLeaveScore)
    {
      m_GameClientView.ReduceJettonNumber();
    }

  }
  else
  {
    //��ֹ��ť
    m_GameClientView.m_btJetton100.EnableWindow(FALSE);
    m_GameClientView.m_btJetton1000.EnableWindow(FALSE);
    m_GameClientView.m_btJetton10000.EnableWindow(FALSE);
    m_GameClientView.m_btJetton100000.EnableWindow(FALSE);
    m_GameClientView.m_btJetton1000000.EnableWindow(FALSE);

    m_GameClientView.m_lCurrentJetton = 0L;
  }

  //��ȡ��Ϣ
  IClientUserItem *pMeUserData=GetTableUserItem(GetMeChairID());
  //���а�ť
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

  //�������
  ZeroMemory(m_lPlayerBetAll,sizeof(m_lPlayerBetAll));
  ZeroMemory(m_lUserJettonScore,sizeof(m_lUserJettonScore));

  //��������
  CMD_C_CleanMeJetto CleanMeJetton;
  ZeroMemory(&CleanMeJetton,sizeof(CleanMeJetton));

  //��ǰ���
  CleanMeJetton.wChairID=GetMeChairID();
  //������Ϣ
  SendSocketData(SUB_C_CLEAN_JETTON,&CleanMeJetton,sizeof(CleanMeJetton));

  //���¿ؼ�
  UpdateButtonContron(true);
  return 0;
}

//��ע��Ϣ
LRESULT CGameClientEngine::OnPlaceJetton(WPARAM wParam, LPARAM lParam)
{
  //��������
  BYTE cbJettonArea=(BYTE)wParam;
  LONGLONG lJettonScore=(LONGLONG)(*((LONGLONG*)lParam));

  //�Ϸ��ж�
  ASSERT(cbJettonArea>=1 && cbJettonArea<=AREA_COUNT);
  if(!(cbJettonArea>=1 && cbJettonArea<=AREA_COUNT))
  {
    return 0;
  }

  //ׯ���ж�
  if(GetMeChairID() == m_wCurrentBanker)
  {
    return true;
  }


  //״̬�ж�
  if(GetGameStatus()!=GS_PLACE_JETTON)
  {
    m_GameClientView.m_lCurrentJetton = 0;
    UpdateButtonContron();
    return true;
  }

  //���ñ���
  //m_lUserJettonScore[cbJettonArea] += lJettonScore;
  m_GameClientView.SetMePlaceJetton(cbJettonArea, lJettonScore);


  //��������
  CMD_C_PlaceJetton PlaceJetton;
  ZeroMemory(&PlaceJetton,sizeof(PlaceJetton));

  //�������
  PlaceJetton.cbJettonArea=cbJettonArea;
  PlaceJetton.lJettonScore=lJettonScore;
  m_bAddScore=true;
  //������Ϣ
  if(!SendSocketData(SUB_C_PLACE_JETTON,&PlaceJetton,sizeof(PlaceJetton)))
  {
    return 0;
  };

  //���°�ť
  UpdateButtonContron(true);

  //Ԥ����ʾ
  m_GameClientView.PlaceUserJetton(cbJettonArea,lJettonScore);

  //��������

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
//��Ͷ
LRESULT CGameClientEngine::OnLastPlaceJetton(WPARAM wParam, LPARAM lParam)
{
  if(GetGameStatus() == GS_PLACE_JETTON)
  {
    //��������
    CMD_C_ContinueJetton PlaceLastJetton;
    ZeroMemory(&PlaceLastJetton,sizeof(PlaceLastJetton));

    //�������
    PlaceLastJetton.wChairID = GetMeChairID();
    //������Ϣ
    if(!SendSocketData(SUB_C_CONTINUE_JETTON,&PlaceLastJetton,sizeof(PlaceLastJetton)))
    {
      return 0;
    };

    return 0;
  }

  return 0;
}



//�Լ�����·�
LONGLONG CGameClientEngine::GetMeMaxBet(BYTE cbArea)
{
  //�ҳ����
  IClientUserItem * pUserData = GetTableUserItem(GetMeChairID());
  if(pUserData == NULL || cbArea > AREA_COUNT)
  {
    return 0l;
  }

  //��ҷ���
  LONGLONG lUserScore = pUserData->GetUserScore();

  //��ȥ�Լ�����ע
  for(int i = 0; i < AREA_COUNT; ++i)
  {
    lUserScore -= m_lUserJettonScore[i];
  }

  //�����������ע. ��Ҫ�ж���������
  if(cbArea < AREA_COUNT)
  {
    //����������
    LONGLONG lAreaLimit = m_lAreaLimitScore - m_lPlayerBetAll[cbArea];
    lUserScore = __min(lUserScore, lAreaLimit);
  }

  //�쳣��������
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
//������Ϣ
LRESULT CGameClientEngine::OnApplyBanker(WPARAM wParam, LPARAM lParam)
{
  //�Ϸ��ж�
  IClientUserItem * pClientUserItem=GetTableUserItem(GetMeChairID());

  if(pClientUserItem->GetUserScore() < m_lApplyBankerCondition)
  {
    return true;
  }

  //�Թ��ж�
  if(IsLookonMode())
  {
    return true;
  }

  //ת������
  bool bApplyBanker = wParam ? true:false;

  //��ǰ�ж�
  if(m_wCurrentBanker == GetMeChairID() && bApplyBanker)
  {
    return true;
  }

  if(bApplyBanker)
  {
    //������Ϣ
    SendSocketData(SUB_C_APPLY_BANKER, NULL, 0);

    //m_bMeApplyBanker=true;
  }
  else
  {
    //������Ϣ
    SendSocketData(SUB_C_CANCEL_BANKER, NULL, 0);

    //m_bMeApplyBanker=false;
  }

  //���ð�ť
  UpdateButtonContron();

  return true;
}

//������ׯ
bool CGameClientEngine::OnSubUserApplyBanker(const void * pBuffer, WORD wDataSize)
{
  try
  {
    //Ч������
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

//ȡ����ׯ
bool CGameClientEngine::OnSubUserCancelBanker(const void * pBuffer, WORD wDataSize)
{
  try
  {
    //Ч������
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

//�л�ׯ��
bool CGameClientEngine::OnSubChangeBanker(const void * pBuffer, WORD wDataSize)
{
  try
  {
    //Ч������
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

//��Ϸ��¼
bool CGameClientEngine::OnSubGameRecord(const void * pBuffer, WORD wDataSize)
{
  //Ч�����
  ASSERT(wDataSize%sizeof(tagServerGameRecord)==0);
  if(wDataSize%sizeof(tagServerGameRecord)!=0)
  {
    return false;
  }
  //�������
  tagGameRecord GameRecord;
  ZeroMemory(&GameRecord,sizeof(GameRecord));

  //���ü�¼
  WORD wRecordCount=wDataSize/sizeof(tagServerGameRecord);
  for(WORD wIndex=0; wIndex<wRecordCount; wIndex++)
  {
    tagServerGameRecord * pServerGameRecord=(((tagServerGameRecord *)pBuffer)+wIndex);

    m_GameClientView.SetGameHistory(pServerGameRecord->bWinMen);
  }

  return true;
}

//��עʧ��
bool CGameClientEngine::OnSubPlaceJettonFail(const void * pBuffer, WORD wDataSize)
{
  try
  {
    //Ч������
    ASSERT(wDataSize==sizeof(CMD_S_PlaceJettonFail));
    if(wDataSize!=sizeof(CMD_S_PlaceJettonFail))
    {
      return false;
    }

    //��Ϣ����
    CMD_S_PlaceJettonFail * pPlaceJettonFail=(CMD_S_PlaceJettonFail *)pBuffer;
    //Ч�����
    BYTE cbViewIndex=pPlaceJettonFail->lJettonArea;
    ASSERT(cbViewIndex>=1 && cbViewIndex<=AREA_COUNT);
    if(!(cbViewIndex>=1 && cbViewIndex<=AREA_COUNT))
    {
      return false;
    }

    //��ע����
    m_GameClientView.PlaceUserJetton(pPlaceJettonFail->lJettonArea,-pPlaceJettonFail->lPlaceScore);

    //�Լ��ж�
    if(GetMeChairID()==pPlaceJettonFail->wPlaceUser && false==IsLookonMode())
    {
      LONGLONG lJettonCount=pPlaceJettonFail->lPlaceScore;
      //�Ϸ�У��
      ASSERT(m_lUserJettonScore[cbViewIndex]>=lJettonCount);
      if(lJettonCount>m_lUserJettonScore[cbViewIndex])
      {
        return false;
      }

      //������ע
      m_lUserJettonScore[cbViewIndex]-=lJettonCount;
      m_GameClientView.SetMePlaceJetton(cbViewIndex,m_lUserJettonScore[cbViewIndex]);

      m_GameClientView.m_bAddScoreStatue=false;
    }
  }
  catch(...)
  {
    //AfxMessageBox(TEXT("��עʧ��"));
  }

  return true;
}

//����ׯ��
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
    //AfxMessageBox(TEXT("����ׯ�� �쳣"));
  }

}

//������ע
void CGameClientEngine::SetMePlaceJetton(BYTE cbViewIndex, LONGLONG lJettonCount)
{
  //�Ϸ��ж�
  ASSERT(cbViewIndex>=1 && cbViewIndex<=AREA_COUNT);
  if(!(cbViewIndex>=1 && cbViewIndex<=AREA_COUNT))
  {
    return;
  }

  //���ñ���
  m_lUserJettonScore[cbViewIndex]=lJettonCount;

  //���ý���
  m_GameClientView.SetMePlaceJetton(cbViewIndex,lJettonCount);
}
//////////////////////////////////////////////////////////////////////////
//����
LRESULT CGameClientEngine::OnAdminControl(WPARAM wParam, LPARAM lParam)
{
  try
  {
    SendSocketData(SUB_C_ADMIN_COMMDN,(CMD_C_ControlApplication*)wParam,sizeof(CMD_C_ControlApplication));
  }
  catch(...)
  {
    //AfxMessageBox(TEXT("OnAdminControl ( WPARAM wParam, LPARAM lParam ) �쳣"));
  }

  return true;
}

//����
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
    //AfxMessageBox(TEXT("OnSubAdminControl( const void * pBuffer, WORD wDataSize ) �쳣"));
  }

  return true;
}

void CGameClientEngine::OnTimer(UINT nIDEvent)
{
  try
  {
    // TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
    if(nIDEvent == IDI_ANDROID_BET)
    {
      if(m_ListAndroid.GetCount() != 0)
      {
        POSITION pos = m_ListAndroid.GetTailPosition();
        POSITION posTmp = pos;

        //������ע����
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
            //ģ����Ϣ
            CMD_S_PlaceJetton placeJetton = {};
            placeJetton.cbAndroid = FALSE;
            //placeJetton.cbJettonArea = androidBet.cbJettonArea;
            //placeJetton.lJettonScore = androidBet.lJettonScore;
            placeJetton.wChairID = androidBet.wChairID;

            OnSubPlaceJetton((void*)&placeJetton, sizeof(placeJetton));

            //ɾ��Ԫ��
            m_ListAndroid.RemoveAt(posTmp);
          }
        }
      }
    }
    CGameFrameEngine::OnTimer(nIDEvent);
  }
  catch(...)
  {
    //AfxMessageBox(TEXT("��ʱ���쳣 OnTimer"));
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

//�û�����
VOID  CGameClientEngine::OnEventUserEnter(IClientUserItem * pIClientUserItem, bool bLookonUser)
{
  try
  {
    //���óɼ�
    if(bLookonUser==false)
    {
      //���½���
      m_GameClientView.RefreshGameView();
    }

    return;
  }
  catch(...)
  {
    //AfxMessageBox(TEXT("�û������쳣"));
  }
}

//�û��뿪
VOID  CGameClientEngine::OnEventUserLeave(IClientUserItem * pIClientUserItem, bool bLookonUser)
{
  try
  {
    //���óɼ�
    if(bLookonUser==false)
    {


      WORD wViewChairId = SwitchViewChairID(pIClientUserItem->GetChairID());

      //���½���
      m_GameClientView.RefreshGameView();
    }

    return;
  }
  catch(...)
  {
    //AfxMessageBox(TEXT("�û��뿪�쳣"));
  }

}

//�û�״̬
VOID  CGameClientEngine::OnEventUserStatus(IClientUserItem * pIClientUserItem, bool bLookonUser)
{
  try
  {
    //��������
    IClientUserItem * pIMySelfUserItem=GetMeUserItem();
    BYTE cbUserStatus=pIClientUserItem->GetUserStatus();
    return;
  }
  catch(...)
  {
    //AfxMessageBox(TEXT("�û�״̬�쳣"));
  }
}

//ѡ���¼�
VOID  CGameClientEngine::OnEventPhraseSelect(INT nPhraseIndex)
{
  try
  {
  }
  catch(...)
  {
    //AfxMessageBox(TEXT("ѡ���¼��쳣"));
  }
}

//��������
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
    //AfxMessageBox(TEXT("���������쳣"));
  }

  return true;
}

//���¿��
LRESULT CGameClientEngine::OnUpdateStorage(WPARAM wParam,LPARAM lParam)
{
  SendSocketData(SUB_C_UPDATE_STORAGE,(CMD_C_UpdateStorage*)wParam,sizeof(CMD_C_UpdateStorage));
  return true;
}

//���¿��
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



