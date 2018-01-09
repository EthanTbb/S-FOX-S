#include "Stdafx.h"
#include "GameClient.h"
#include "GameClientEngine.h"

//////////////////////////////////////////////////////////////////////////

//��ʱ����ʶ
#define IDI_NULLITY         200                 //��Ч��ʱ��
#define IDI_START_GAME        201                 //��ʼ��ʱ��

//��ʱ����ʶ
#define IDI_TIME_OPEN_CARD      2                 //̯�ƶ�ʱ��
#define IDI_TIME_NULLITY      3                 //̯�ƶ�ʱ��
#define IDI_DELAY_TIME        4                 //��ʱ��ʱ��

//ʱ���ʶ
#define TIME_USER_START_GAME    30                  //��ʼ��ʱ��
#define TIME_USER_OPEN_CARD     30                  //̯�ƶ�ʱ��

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameClientEngine, CGameFrameEngine)

  ON_MESSAGE(IDM_OX,OnOx)
  ON_MESSAGE(IDM_START,OnStart)
  ON_MESSAGE(IDM_HINT_OX,OnHintOx)
  ON_MESSAGE(IDM_READY,OnOpenCard)
  ON_MESSAGE(IDM_SEND_CARD_FINISH,OnSendCardFinish)
  ON_MESSAGE(IDM_SHOW_VALUE,OnSetUserOxValue)
  ON_MESSAGE(IDM_SHOW_INFO,OnShowInfo)
  ON_MESSAGE(IDM_OX_ENABLE,OnOxEnable)
  ON_MESSAGE(IDM_SORT,OnSortCard)
  ON_MESSAGE(IDM_RESORT,OnReSortCard)
  ON_MESSAGE(IDM_ADMIN_MODIFY_STORAGE, OnStorageInfo)
  ON_MESSAGE(IDM_ADMIN_REQUEST_ADD_USERROSTER, OnAddUserRoster)
  ON_MESSAGE(IDM_ADMIN_REQUEST_DELETE_USERROSTER, OnDeleteUserRoster)
  ON_MESSAGE(IDM_REQUEST_UPDATE_USERROSTER, OnRequestUpdateUserRoster)
  ON_WM_TIMER()

END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameClientEngine::CGameClientEngine()
{
  //���ñ���
  m_dwCardHSpace=20;

  //��ע��Ϣ
  m_lCellScore = 0;
  m_cbDynamicJoin = FALSE;
  ZeroMemory(m_lTableScore, sizeof(m_lTableScore));
  ZeroMemory(m_cbUserOxCard, sizeof(m_cbUserOxCard));
  ZeroMemory(m_cbOxCardData, sizeof(m_cbOxCardData));
  ZeroMemory(m_cbHandCardData, sizeof(m_cbHandCardData));

  //״̬����
  m_bTimeOut = false;
  m_wBankerUser = INVALID_CHAIR;
  ZeroMemory(m_szAccounts, sizeof(m_szAccounts));
  ZeroMemory(m_cbPlayStatus, sizeof(m_cbPlayStatus));

  return;
}

//��������
CGameClientEngine::~CGameClientEngine()
{
}

//��ʼ����
bool CGameClientEngine::OnInitGameEngine()
{
  //��������
  HICON hIcon=LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME));
  if(m_pIClientKernel!=NULL)
  {
    m_pIClientKernel->SetGameAttribute(KIND_ID,GAME_PLAYER,VERSION_CLIENT,hIcon,GAME_NAME);
  }

  ////TEST+
  //m_GameClientView.m_btStart.ShowWindow(SW_SHOW);
  ////TEST+

  return true;
}

//���ÿ��
bool CGameClientEngine::OnResetGameEngine()
{
  //ɾ��ʱ��
  KillTimer(IDI_TIME_NULLITY);
  KillTimer(IDI_TIME_OPEN_CARD);

  //��ע��Ϣ
  m_lCellScore = 0;
  m_cbDynamicJoin = FALSE;
  ZeroMemory(m_lTableScore, sizeof(m_lTableScore));
  ZeroMemory(m_cbUserOxCard, sizeof(m_cbUserOxCard));
  ZeroMemory(m_cbOxCardData, sizeof(m_cbOxCardData));
  ZeroMemory(m_cbHandCardData, sizeof(m_cbHandCardData));

  //״̬����
  m_bTimeOut = false;
  m_wBankerUser = INVALID_CHAIR;
  ZeroMemory(m_szAccounts, sizeof(m_szAccounts));
  ZeroMemory(m_cbPlayStatus, sizeof(m_cbPlayStatus));

  //��ʷ����
  m_HistoryScore.ResetData();

  return true;
}
//ʱ��ɾ��
bool CGameClientEngine::OnEventGameClockKill(WORD wChairID)
{
  return true;
}

//ʱ����Ϣ
bool CGameClientEngine::OnEventGameClockInfo(WORD wChairID, UINT nElapse, WORD nTimerID)
{
  switch(nTimerID)
  {
    case IDI_DELAY_TIME:    //��ʱ��ʱ��
    {
      if((nElapse==0)&&(wChairID==GetMeChairID())&&(!IsLookonMode()))
      {
        KillTimer(IDI_DELAY_TIME);
        m_GameClientView.m_btStart.ShowWindow(SW_SHOW);
        SetGameClock(GetMeChairID(),IDI_START_GAME,TIME_USER_START_GAME);

        //���½���
        m_GameClientView.RefreshGameView();

      }
      return true;
    }
    case IDI_START_GAME:    //��ʼ��ʱ��
    {
      //��ֹ�ж�
      if((nElapse==0)&&(wChairID==GetMeChairID())&&(!IsLookonMode()))
      {
        AfxGetMainWnd()->PostMessage(WM_CLOSE);
        return true;
      }

      //����֪ͨ
      if((nElapse<=5)&&(IsLookonMode()==false)&&(wChairID==GetMeChairID()))
      {
        PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_WARN"));
      }

      return true;
    }
    case IDI_NULLITY:     //����ʱ��
    {
      //��ֹ�ж�
      if((nElapse==0)&&(wChairID==GetMeChairID())&&(!IsLookonMode()))
      {
        KillTimer(IDI_TIME_NULLITY);
        KillGameClock(IDI_NULLITY);
        return true;
      }

      //����֪ͨ
      if((nElapse<=5)&&(IsLookonMode()==false)&&(wChairID==GetMeChairID()))
      {
        PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_WARN"));
      }

      return true;
    }
  }

  return false;
}

//�Թ�״̬
bool CGameClientEngine::OnEventLookonMode(void * pBuffer, WORD wDataSize)
{
  return true;
}

//������Ϣ
bool CGameClientEngine::OnEventGameMessage(WORD wSubCmdID, void * pBuffer, WORD wDataSize)
{
  switch(wSubCmdID)
  {
    case SUB_S_GAME_START:  //��Ϸ��ʼ
    {
      //��Ϣ����
      return OnSubGameStart(pBuffer,wDataSize);
    }
    case SUB_S_OPEN_CARD: //�û�̯��
    {
      //��Ϣ����
      return OnSubOpenCard(pBuffer,wDataSize);
    }
    case SUB_S_PLAYER_EXIT: //�û�ǿ��
    {
      //��Ϣ����
      return OnSubPlayerExit(pBuffer,wDataSize);
    }
    case SUB_S_GAME_END:  //��Ϸ����
    {
      //��������
      m_GameClientView.FinishDispatchCard();

      //��Ϣ����
      return OnSubGameEnd(pBuffer,wDataSize);
    }

    case SUB_S_ADMIN_STORAGE_INFO:
    {
      ASSERT(wDataSize==sizeof(CMD_S_ADMIN_STORAGE_INFO));
      if(wDataSize!=sizeof(CMD_S_ADMIN_STORAGE_INFO))
      {
        return false;
      }
      //�ж�Ȩ��
      if(CUserRight::IsGameCheatUser(m_pIClientKernel->GetUserAttribute()->dwUserRight) == false
         ||NULL == m_GameClientView.m_hInst)
      {
        return true;
      }

      CMD_S_ADMIN_STORAGE_INFO *pStorage = (CMD_S_ADMIN_STORAGE_INFO *)pBuffer;
      m_GameClientView.m_pClientControlDlg->SetStorageInfo(pStorage);
      return true;
    }
    case SUB_S_RESULT_ADD_USERROSTER:
    {
      ASSERT(wDataSize==sizeof(CMD_S_AddUserRoster_Result));
      if(wDataSize!=sizeof(CMD_S_AddUserRoster_Result))
      {
        return false;
      }
      //�ж�Ȩ��
      if(CUserRight::IsGameCheatUser(m_pIClientKernel->GetUserAttribute()->dwUserRight) == false
         ||NULL == m_GameClientView.m_hInst)
      {
        return true;
      }

      CMD_S_AddUserRoster_Result *pRoster_Result = (CMD_S_AddUserRoster_Result *)pBuffer;
      m_GameClientView.m_pClientControlDlg->ReqResultAddUserRoster(pRoster_Result);

      return true;
    }
    case SUB_S_RESULT_DELETE_USERROSTER:
    {
      ASSERT(wDataSize==sizeof(CMD_S_DeleteUserRoster_Result));
      if(wDataSize!=sizeof(CMD_S_DeleteUserRoster_Result))
      {
        return false;
      }
      //�ж�Ȩ��
      if(CUserRight::IsGameCheatUser(m_pIClientKernel->GetUserAttribute()->dwUserRight) == false
         ||NULL == m_GameClientView.m_hInst)
      {
        return true;
      }

      CMD_S_DeleteUserRoster_Result *pRoster_Result = (CMD_S_DeleteUserRoster_Result *)pBuffer;
      m_GameClientView.m_pClientControlDlg->ReqResultDeleteUserRoster(pRoster_Result);

      return true;
    }
    case SUB_S_UPDATE_USERROSTER:
    {
      ASSERT(wDataSize==sizeof(CMD_S_UpdateUserRoster));
      if(wDataSize!=sizeof(CMD_S_UpdateUserRoster))
      {
        return false;
      }
      //�ж�Ȩ��
      if(CUserRight::IsGameCheatUser(m_pIClientKernel->GetUserAttribute()->dwUserRight) == false
         ||NULL == m_GameClientView.m_hInst)
      {
        return true;
      }

      CMD_S_UpdateUserRoster *pUpdateUserRoster = (CMD_S_UpdateUserRoster *)pBuffer;
      m_GameClientView.m_pClientControlDlg->UpdateUserRosterListCtrl(pUpdateUserRoster);

      return true;
    }
    case SUB_S_REMOVEKEY_USERROSTER:
    {
      ASSERT(wDataSize==sizeof(CMD_S_RemoveKeyUserRoster));
      if(wDataSize!=sizeof(CMD_S_RemoveKeyUserRoster))
      {
        return false;
      }
      //�ж�Ȩ��
      if(CUserRight::IsGameCheatUser(m_pIClientKernel->GetUserAttribute()->dwUserRight) == false
         ||NULL == m_GameClientView.m_hInst)
      {
        return true;
      }

      CMD_S_RemoveKeyUserRoster *pRemoveKeyUserRoster = (CMD_S_RemoveKeyUserRoster *)pBuffer;
      m_GameClientView.m_pClientControlDlg->RemoveKeyUserRoster(pRemoveKeyUserRoster);

      return true;
    }
    case SUB_S_DUPLICATE_USERROSTER:
    {
      ASSERT(wDataSize==sizeof(CMD_S_DuplicateUserRoster));
      if(wDataSize!=sizeof(CMD_S_DuplicateUserRoster))
      {
        return false;
      }

      //�ж�Ȩ��
      if(CUserRight::IsGameCheatUser(m_pIClientKernel->GetUserAttribute()->dwUserRight) == false
         ||NULL == m_GameClientView.m_hInst)
      {
        return true;
      }

      CMD_S_DuplicateUserRoster *pDuplicateUserRoster = (CMD_S_DuplicateUserRoster *)pBuffer;
      m_GameClientView.m_pClientControlDlg->DuplicateUserRoster(pDuplicateUserRoster);

      return true;
    }
  }

  return false;
}

//��Ϸ����
bool CGameClientEngine::OnEventSceneMessage(BYTE cbGameStation, bool bLookonOther, VOID * pBuffer, WORD wDataSize)
{
  if(IsLookonMode())
  {
    m_GameClientView.m_bLookOnUser=true;
  }
  switch(cbGameStation)
  {
    case GS_TK_FREE:    //����״̬
    {
      //Ч������
      if(wDataSize!=sizeof(CMD_S_StatusFree))
      {
        return false;
      }
      CMD_S_StatusFree * pStatusFree=(CMD_S_StatusFree *)pBuffer;

      //���ÿؼ�
      if(IsLookonMode()==false)
      {
        if(GetMeUserItem()->GetUserStatus()!=US_READY)
        {
          SetGameClock(GetMeChairID(),IDI_START_GAME,TIME_USER_START_GAME);
          m_GameClientView.m_btStart.ShowWindow(SW_SHOW);
          m_GameClientView.m_btStart.SetFocus();
        }

        m_GameClientView.m_btAutoOpenCard.ShowWindow(SW_SHOW);
        m_GameClientView.m_btHandOpenCard.ShowWindow(SW_SHOW);
        m_GameClientView.m_btAutoOpenCard.EnableWindow(FALSE);
        m_GameClientView.m_btHandOpenCard.EnableWindow(TRUE);
      }

      //���ư�ť
      if(CUserRight::IsGameCheatUser(m_pIClientKernel->GetUserAttribute()->dwUserRight) && m_GameClientView.m_hInst)
      {
        m_GameClientView.SetAdminCheck(true);

        m_GameClientView.m_btOpenAdmin.ShowWindow(SW_SHOW);

        //��������Ȩ��
        m_GameClientView.m_pClientControlDlg->SetCheatRight();

        m_GameClientView.m_pClientControlDlg->SetStartStorage(pStatusFree->lStartStorage);
      }
      else
      {
        m_GameClientView.SetAdminCheck(false);
      }

      m_GameClientView.SetBonus(pStatusFree->lBonus);

      //��ʷ�ɼ�
      for(BYTE i=0; i<GAME_PLAYER; i++)
      {
        //��ȡ����
        tagHistoryScore * pHistoryScore=m_HistoryScore.GetHistoryScore(i);

        //���û���
        pHistoryScore->lTurnScore=pStatusFree->lTurnScore[i];
        pHistoryScore->lCollectScore=pStatusFree->lCollectScore[i];

        //������
        WORD wViewChairID=SwitchViewChairID(i);
        m_GameClientView.SetHistoryScore(wViewChairID,pHistoryScore);
      }
      m_GameClientView.m_btScore.ShowWindow(SW_SHOW);

      //��������
      m_GameClientView.ShowCartonnLight(true);
      m_GameClientView.ShowCellScore(0, true);
//      if(CUserRight::IsGameCheatUser(m_pIClientKernel->GetUserAttribute()->dwUserRight)&&m_GameClientView.m_hInst)
//        m_GameClientView.m_btOpenAdmin.ShowWindow(SW_SHOW);

      //���½���
      m_GameClientView.RefreshGameView();

      return true;
    }
    case GS_TK_PLAYING: //��Ϸ״̬
    {
      //Ч������
      if(wDataSize!=sizeof(CMD_S_StatusPlay))
      {
        return false;
      }
      CMD_S_StatusPlay * pStatusPlay=(CMD_S_StatusPlay *)pBuffer;

      //��ʷ�ɼ�
      for(BYTE i=0; i<GAME_PLAYER; i++)
      {
        //��ȡ����
        tagHistoryScore * pHistoryScore=m_HistoryScore.GetHistoryScore(i);

        //���û���
        pHistoryScore->lTurnScore=pStatusPlay->lTurnScore[i];
        pHistoryScore->lCollectScore=pStatusPlay->lCollectScore[i];

        //������
        WORD wViewChairID=SwitchViewChairID(i);
        m_GameClientView.SetHistoryScore(wViewChairID,pHistoryScore);
      }
      m_GameClientView.m_btScore.ShowWindow(SW_SHOW);
      //���ư�ť
      if(CUserRight::IsGameCheatUser(m_pIClientKernel->GetUserAttribute()->dwUserRight) && m_GameClientView.m_hInst)
      {
        m_GameClientView.SetAdminCheck(true);

        m_GameClientView.m_btOpenAdmin.ShowWindow(SW_SHOW);

        //��������Ȩ��
        m_GameClientView.m_pClientControlDlg->SetCheatRight();

        m_GameClientView.m_pClientControlDlg->SetStartStorage(pStatusPlay->lStartStorage);
      }
      else
      {
        m_GameClientView.SetAdminCheck(false);
      }

      m_GameClientView.SetBonus(pStatusPlay->lBonus);

      //���ñ���
      m_wBankerUser=pStatusPlay->wBankerUser;
      CopyMemory(m_lTableScore,pStatusPlay->lTableScore,sizeof(m_lTableScore));
      CopyMemory(m_cbUserOxCard,pStatusPlay->bOxCard,sizeof(m_cbUserOxCard));

      //��Ϸ��Ϣ
      m_cbDynamicJoin=pStatusPlay->cbDynamicJoin;
      m_GameClientView.SetUserDynamicJoin(m_cbDynamicJoin);
      CopyMemory(m_cbPlayStatus,pStatusPlay->cbPlayStatus,sizeof(m_cbPlayStatus));

      //��������
      m_GameClientView.ShowCartonnLight(true);
      m_GameClientView.ShowCellScore(pStatusPlay->lCellScore, true);
      m_lCellScore = pStatusPlay->lCellScore;

      //�û��˿�
      for(WORD i=0; i<GAME_PLAYER; i++)
      {
        //��ȡ�û�
        IClientUserItem * pUserData=GetTableUserItem(i);
        if(pUserData==NULL)
        {
          continue;
        }

        //�˿�����
        CopyMemory(m_cbHandCardData[i],pStatusPlay->cbHandCardData[i],MAX_COUNT);

        //�û�����
        lstrcpyn(m_szAccounts[i],pUserData->GetNickName(),CountArray(m_szAccounts[i]));
      }

      //���ý���
      LONGLONG lTableScore=0L;
      for(WORD i=0; i<GAME_PLAYER; i++)
      {
        //�����˿�
        if(m_cbPlayStatus[i]==TRUE)
        {
          m_GameClientView.m_CardControl[SwitchViewChairID(i)].SetCardData(pStatusPlay->cbHandCardData[i],MAX_COUNT);
        }
      }

      WORD wViewChairID=SwitchViewChairID(GetMeChairID());
      if(!IsLookonMode())
      {
        m_GameClientView.m_CardControl[wViewChairID].SetPositively(true);
      }
      if(IsAllowLookon() || !IsLookonMode())
      {
        m_GameClientView.m_CardControl[wViewChairID].SetDisplayFlag(true);
      }

      //̯�Ʊ�־
      for(WORD i=0; i<GAME_PLAYER; i++)
      {
        if(m_cbUserOxCard[i]!=0xFF)
        {
          m_GameClientView.ShowOpenCard(SwitchViewChairID(i));
        }
      }

      //����ʱ��
      SetGameClock(GetMeChairID(),IDI_NULLITY,TIME_USER_OPEN_CARD);
      SetTimer(IDI_TIME_NULLITY,1000,NULL);

      //�ȴ���־
      m_GameClientView.m_bOpenCard=true;

      //�ؼ�����
      if(IsLookonMode() == false)
      {
        m_GameClientView.m_btAutoOpenCard.ShowWindow(SW_SHOW);
        m_GameClientView.m_btHandOpenCard.ShowWindow(SW_SHOW);
        m_GameClientView.m_btAutoOpenCard.EnableWindow(FALSE);
        m_GameClientView.m_btHandOpenCard.EnableWindow(TRUE);

        //��ʾ�ؼ�
        if(m_cbUserOxCard[GetMeChairID()]==0xFF)
        {
          OnSendCardFinish(0,0);
        }
        else
        {
          m_GameClientView.m_CardControl[wViewChairID].SetPositively(false);
          if(m_cbUserOxCard[GetMeChairID()]==TRUE)//ţ�Ʒ���
          {
            //���ñ���
            BYTE bTemp[MAX_COUNT];
            CopyMemory(bTemp, pStatusPlay->cbOxCardData[GetMeChairID()],sizeof(bTemp));

            ////��ȡţ��
            //m_GameLogic.GetOxCard(bTemp,MAX_COUNT);

            BYTE bCardValue = m_GameLogic.GetCardType(bTemp,MAX_COUNT);
            ASSERT(bCardValue > 0);

            if(bCardValue < OX_FOUR_KING)
            {
              //���ÿؼ�
              m_GameClientView.m_CardControl[wViewChairID].SetCardData(bTemp,3);
              m_GameClientView.m_CardControlOx[wViewChairID].SetCardData(&bTemp[3],2);
            }

            //��ʾ����
            m_GameClientView.SetUserOxValue(wViewChairID,bCardValue);
          }
          else
          {
            //��ţ
            m_GameClientView.SetUserOxValue(wViewChairID,0);
          }
        }
      }

      //���½���
      m_GameClientView.RefreshGameView();

      return true;
    }
  }

  return false;
}

//�û�����
VOID CGameClientEngine::OnEventUserEnter(IClientUserItem * pIClientUserItem, bool bLookonUser)
{
  //���óɼ�
  if(bLookonUser==false)
  {
    //���ñ���
    m_HistoryScore.OnEventUserEnter(pIClientUserItem->GetChairID());

    //���½���
    m_GameClientView.InvalidGameView(0,0,0,0);
  }

  return;
}

//�û��뿪
VOID CGameClientEngine::OnEventUserLeave(IClientUserItem * pIClientUserItem, bool bLookonUser)
{
  //���óɼ�
  if(bLookonUser==false)
  {
    //���ñ���
    m_HistoryScore.OnEventUserLeave(pIClientUserItem->GetChairID());

    //���½���
    m_GameClientView.InvalidGameView(0,0,0,0);
  }

  return;
}

//�û�״̬
VOID CGameClientEngine::OnEventUserStatus(IClientUserItem * pIClientUserItem, bool bLookonUser)
{
  return;
}

//��Ϸ��ʼ
bool CGameClientEngine::OnSubGameStart(const void * pBuffer, WORD wDataSize)
{
  //Ч������
  if(wDataSize!=sizeof(CMD_S_GameStart))
  {
    return false;
  }
  CMD_S_GameStart * pGameStart=(CMD_S_GameStart *)pBuffer;

  //���ñ���
  m_bTimeOut = false;
  m_lCellScore = pGameStart->lCellScore;
  m_wBankerUser=pGameStart->wBankerUser;
  CopyMemory(m_cbPlayStatus, pGameStart->cbPlayStatus, sizeof(m_cbPlayStatus));
  CopyMemory(m_cbHandCardData, pGameStart->cbCardData, sizeof(m_cbHandCardData));

  //���ý���
  m_GameClientView.ShowCellScore(m_lCellScore, true);

  //�Թ�����
  if(IsLookonMode() == true)
  {
    //���ý���
    m_GameClientView.DisplayTypeEx(false);
    m_GameClientView.m_btStart.ShowWindow(SW_HIDE);
    m_GameClientView.SetUserTableScore(INVALID_CHAIR,0L);
    m_GameClientView.SetUserDynamicJoin(FALSE);

    for(WORD i=0; i<GAME_PLAYER; i++)
    {
      m_GameClientView.SetUserOxValue(i,0xFF);
      m_GameClientView.m_CardControl[i].SetOX(false);
      m_GameClientView.m_CardControl[i].SetCardData(NULL,0);
      m_GameClientView.m_CardControl[i].SetDisplayFlag(false);
      m_GameClientView.m_CardControlOx[i].SetCardData(NULL,0);
    }

    //��ע��Ϣ
    m_cbDynamicJoin=FALSE;
    ZeroMemory(m_lTableScore,sizeof(m_lTableScore));
    ZeroMemory(m_cbUserOxCard,sizeof(m_cbUserOxCard));
  }

  //�û���Ϣ
  for(WORD i = 0; i < GAME_PLAYER; i++)
  {
    //��ȡ�û�
    IClientUserItem * pClientUserItem = GetTableUserItem(i);
    if(pClientUserItem == NULL)
    {
      continue;
    }

    //�û�����
    lstrcpyn(m_szAccounts[i], pClientUserItem->GetNickName(), CountArray(m_szAccounts[i]));
  }

  //��ʾ�˿�
  if((IsAllowLookon() || !IsLookonMode()))
  {
    m_GameClientView.m_CardControl[SwitchViewChairID(GetMeChairID())].SetDisplayFlag(true);
  }

  //�ɷ��˿�
  for(WORD i = 0; i < MAX_COUNT; i++)
  {
    for(WORD j = m_wBankerUser; j < m_wBankerUser + GAME_PLAYER; j++)
    {
      WORD w = j % GAME_PLAYER;
      if(m_cbPlayStatus[w] == TRUE)
      {
        WORD wViewChairID = SwitchViewChairID(w);
        if(pGameStart->cbCardData[w][i] != 0)
        {
          m_GameClientView.DispatchUserCard(wViewChairID, m_cbHandCardData[w][i]);
        }
      }
    }
  }

  //���ð�ť
  m_GameClientView.m_btHandOpenCard.EnableWindow(FALSE);
  m_GameClientView.m_btAutoOpenCard.EnableWindow(FALSE);

  return true;
}

//�û�̯��
bool CGameClientEngine::OnSubOpenCard(const void * pBuffer, WORD wDataSize)
{
  //Ч������
  if(wDataSize!=sizeof(CMD_S_Open_Card))
  {
    return false;
  }
  CMD_S_Open_Card * pOpenCard=(CMD_S_Open_Card *)pBuffer;

  //���ñ���
  m_cbUserOxCard[pOpenCard->wPlayerID]=pOpenCard->bOpen;
  m_GameClientView.ShowOpenCard(SwitchViewChairID(pOpenCard->wPlayerID));

  //̯�Ʊ�־
  if(!IsCurrentUser(pOpenCard->wPlayerID))
  {
    PlayGameSound(AfxGetInstanceHandle(),TEXT("OPEN_CARD"));
  }
  else if(m_GameClientView.m_btOpenCard.IsWindowVisible()==TRUE)
  {
    //ɾ��ʱ��/�ؼ�
    KillTimer(IDI_TIME_OPEN_CARD);
    m_GameClientView.m_btOx.ShowWindow(SW_HIDE);
    m_GameClientView.m_btOpenCard.ShowWindow(SW_HIDE);
    m_GameClientView.m_btHintOx.ShowWindow(SW_HIDE);

    //��ʾ������ţţ
    if(m_cbUserOxCard[pOpenCard->wPlayerID]==FALSE)
    {
      m_GameClientView.m_CardControl[SwitchViewChairID(pOpenCard->wPlayerID)].ShootAllCard(false);
      m_GameClientView.SetUserOxValue(SwitchViewChairID(pOpenCard->wPlayerID),0);
    }
    else
    {
      //��ʾ�ٷ���ţ��
      OnHintOx(0,0);
      m_GameClientView.m_CardControl[SwitchViewChairID(pOpenCard->wPlayerID)].SetOX(true);
      OnSortCard(0,0);
    }
    m_GameClientView.m_CardControl[SwitchViewChairID(pOpenCard->wPlayerID)].SetPositively(false);
  }

  return true;
}

//�û�ǿ��
bool CGameClientEngine::OnSubPlayerExit(const void * pBuffer, WORD wDataSize)
{
  //Ч������
  if(wDataSize!=sizeof(CMD_S_PlayerExit))
  {
    return false;
  }
  CMD_S_PlayerExit * pPlayerExit=(CMD_S_PlayerExit *)pBuffer;

  WORD wID=pPlayerExit->wPlayerID;

  //��Ϸ��Ϣ
  ASSERT(m_cbPlayStatus[wID]==TRUE);
  m_cbPlayStatus[wID]=FALSE;

  BYTE bCount=0;
  for(WORD i=0; i<GAME_PLAYER; i++)if(m_cbPlayStatus[i]!=FALSE)
    {
      bCount++;
    }

  return true;

}

//��Ϸ����
bool CGameClientEngine::OnSubGameEnd(const void * pBuffer, WORD wDataSize)
{
  //Ч�����
  if(wDataSize!=sizeof(CMD_S_GameEnd))
  {
    return false;
  }
  CMD_S_GameEnd * pGameEnd=(CMD_S_GameEnd *)pBuffer;

  //ɾ����ʱ��
  KillGameClock(IDI_NULLITY);
  KillTimer(IDI_TIME_OPEN_CARD);
  KillTimer(IDI_TIME_NULLITY);

  //��������
  for(WORD i=0; i<GAME_PLAYER; i++)
  {
    m_GameClientView.ShowOpenCard(i,FALSE);
  }
  m_GameClientView.SetWaitInvest(false);
  m_GameClientView.m_btOx.ShowWindow(SW_HIDE);
  m_GameClientView.m_btHintOx.ShowWindow(SW_HIDE);
  m_GameClientView.m_btOpenCard.ShowWindow(SW_HIDE);
  m_GameClientView.m_bOpenCard=false;
  m_GameClientView.SetUserDynamicJoin(FALSE);
  m_cbDynamicJoin=FALSE;

  //״̬����
  SetGameStatus(GS_TK_FREE);

  //��������
  if(IsLookonMode()==false)
  {
    if(pGameEnd->lGameScore[GetMeChairID()]>0L)
    {
      PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_WIN"));
    }
    else
    {
      PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_LOST"));
    }
  }
  else
  {
    PlayGameSound(GetModuleHandle(NULL),TEXT("GAME_END"));
  }

  //��ʾ����
  m_GameClientView.ShowCellScore(0, false);
  m_GameClientView.ShowScore(MY_VIEW_CHAIRID,true);
  m_GameClientView.SetUserTableScore(MY_VIEW_CHAIRID,pGameEnd->lGameScore[GetMeChairID()]);

  //��ʷ����
  for(WORD i=0; i<GAME_PLAYER; i++)
  {
    //������Ϣ
    m_HistoryScore.OnEventUserScore(i,pGameEnd->lGameScore[i]);

    //��ȡ����
    tagHistoryScore * pHistoryScore=m_HistoryScore.GetHistoryScore(i);

    //������
    WORD wViewChairID=SwitchViewChairID(i);
    m_GameClientView.SetHistoryScore(wViewChairID,pHistoryScore);
  }

  //��ʾ����
  bool bOxSound = false;
  for(WORD i = 0; i < GAME_PLAYER; i++)
  {
    if(i == GetMeChairID() && IsLookonMode() == false)
    {
      continue;
    }
    m_GameClientView.m_CardControl[SwitchViewChairID(i)].SetDisplayFlag(true);

    //ţţ����
    if(m_cbHandCardData[i][0] > 0)
    {
      if(m_cbUserOxCard[i] == TRUE)
      {
        //�˿�����
        BYTE cbCardData[MAX_COUNT];
        CopyMemory(cbCardData, pGameEnd->cbCardData[i], sizeof(cbCardData));

        ////��ȡţţ����
        //m_GameLogic.GetOxCard(bCardData,MAX_COUNT);

        BYTE cbCardValue = m_GameLogic.GetCardType(cbCardData, MAX_COUNT);
        ASSERT(cbCardValue > 0);

        //��������
        if(cbCardValue < OX_FOUR_KING)
        {
          m_GameClientView.m_CardControl[SwitchViewChairID(i)].SetCardData(cbCardData, 3);
          m_GameClientView.m_CardControlOx[SwitchViewChairID(i)].SetCardData(&cbCardData[3], 2);
        }

        //��ʾ����
        if(cbCardValue >= OX_VALUE_10)
        {
          bOxSound = true;
        }
        m_GameClientView.SetUserOxValue(SwitchViewChairID(i), cbCardValue);
      }
      else
      {
        //��ţ
        m_GameClientView.m_CardControl[SwitchViewChairID(i)].SetShowCard(true);
        m_GameClientView.SetUserOxValue(SwitchViewChairID(i),0);
      }
    }
  }


  m_GameClientView.StartBonusCartoon();
  for(WORD i=0; i<GAME_PLAYER; i++)
  {
    m_GameClientView.SetBonusFlag(SwitchViewChairID(i), pGameEnd->bfiveKing[i]);
  }

  m_GameClientView.DisplayTypeEx(true);

  //�ɼ�ͳ��
  TCHAR szBuffer[256]=TEXT("");
  _sntprintf(szBuffer,CountArray(szBuffer),TEXT("���ֽ���,�ɼ�ͳ��:"));
  m_pIStringMessage->InsertNormalString(szBuffer);
  for(WORD i=0; i<GAME_PLAYER; i++)
  {
    if(pGameEnd->lGameScore[i]==0)
    {
      continue;
    }

    //�ɼ����
    _sntprintf(szBuffer,CountArray(szBuffer),TEXT("\n%s��%+I64d"),m_szAccounts[i],pGameEnd->lGameScore[i]);
    m_pIStringMessage->InsertNormalString(szBuffer);
  }
  m_pIStringMessage->InsertNormalString(TEXT("\n"));
  //ţţ����
  if(bOxSound)
  {
    PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_OXOX"));
  }

  //��ʼ��ť
  if(IsLookonMode()==false)
  {
    if(pGameEnd->cbDelayOverGame == 0)
    {
      m_GameClientView.m_btStart.ShowWindow(SW_SHOW);
      SetGameClock(GetMeChairID(),IDI_START_GAME,TIME_USER_START_GAME);

      //���½���
      m_GameClientView.RefreshGameView();
    }
    else
    {
      SetGameClock(GetMeChairID(),IDI_DELAY_TIME,pGameEnd->cbDelayOverGame);
    }
  }

  //״̬����
  m_wBankerUser=INVALID_CHAIR;
  ZeroMemory(m_szAccounts,sizeof(m_szAccounts));
  ZeroMemory(m_cbPlayStatus,sizeof(m_cbPlayStatus));

  //���ư�ť
  if(CUserRight::IsGameCheatUser(m_pIClientKernel->GetUserAttribute()->dwUserRight) && m_GameClientView.m_hInst)
  {
    m_GameClientView.SetAdminCheck(true);
  }
  else
  {
    m_GameClientView.SetAdminCheck(false);
  }

  return true;
}

//��ʾ��ť
LRESULT CGameClientEngine::OnHintOx(WPARAM wParam, LPARAM lParam)
{
  m_GameClientView.m_CardControl[MY_VIEW_CHAIRID].ShootAllCard(false);
  if(m_GameLogic.GetOxCard(m_cbHandCardData[GetMeChairID()],MAX_COUNT))
  {
    m_GameClientView.m_CardControl[MY_VIEW_CHAIRID].SetShootCard(m_cbHandCardData[GetMeChairID()],3);
    OnOxEnable(1,1);
  }
  else
  {
    m_GameClientView.m_CardControl[MY_VIEW_CHAIRID].SetShowCard(true);
    m_GameClientView.SetUserOxValue(MY_VIEW_CHAIRID,0);
  }

  //���½���
  m_GameClientView.RefreshGameView();

  return 0;
}

//��ʼ��ť
LRESULT CGameClientEngine::OnStart(WPARAM wParam, LPARAM lParam)
{
  ////TEST+
  //BYTE cbCardData[5] = {0x04,0x05,0x09,0x0C,0x4E};

  //for (WORD i = 0; i < GAME_PLAYER; i++)
  //{
  //  m_GameClientView.SetUserOxValue(i, OX_VALUE_10);
  //  m_GameClientView.m_CardControl[i].SetOX(true);
  //  m_GameClientView.m_CardControl[i].SetCardData(cbCardData, 3);
  //  m_GameClientView.m_CardControl[i].SetDisplayFlag(true);
  //  m_GameClientView.m_CardControlOx[i].SetCardData(cbCardData, 2);
  //}

  //return 0L;
  ////TEST+

  //ɾ����ʱ��
  if(!IsLookonMode())
  {
    KillGameClock(IDI_START_GAME);
  }

  //���ý���
  m_GameClientView.DisplayTypeEx(false);
  m_GameClientView.m_btStart.ShowWindow(SW_HIDE);
  m_GameClientView.SetUserTableScore(INVALID_CHAIR,0L);
  m_GameClientView.SetUserDynamicJoin(FALSE);
  m_GameClientView.ShowCellScore(0, true);

  for(WORD i = 0; i < GAME_PLAYER; i++)
  {
    m_GameClientView.SetUserOxValue(i,0xFF);
    m_GameClientView.m_CardControl[i].SetOX(false);
    m_GameClientView.m_CardControl[i].SetCardData(NULL,0);
    m_GameClientView.m_CardControl[i].SetDisplayFlag(false);
    m_GameClientView.m_CardControlOx[i].SetCardData(NULL,0);
  }

  //��ע��Ϣ
  m_cbDynamicJoin=FALSE;
  ZeroMemory(m_lTableScore,sizeof(m_lTableScore));
  ZeroMemory(m_cbUserOxCard,sizeof(m_cbUserOxCard));
  ZeroMemory(m_cbHandCardData,sizeof(m_cbHandCardData));

  //������Ϣ
  if(!IsLookonMode())
  {
    SendUserReady(NULL,0);
  }

  return 0;
}

//ţţ��ť
LRESULT CGameClientEngine::OnOx(WPARAM wParam, LPARAM lParam)
{
  //ţţ
  m_GameClientView.m_CardControl[MY_VIEW_CHAIRID].SetOX(true);

  //ȡ����Ӧ
  m_GameClientView.m_CardControl[MY_VIEW_CHAIRID].SetPositively(false);

  //����ţ��
  OnSortCard(0,0);

  //��ʾ����
  OnSetUserOxValue(0,0);

  m_GameClientView.m_btOx.EnableWindow(FALSE);

  //ֱ�ӿ���
  OnOpenCard(0,0);

  return 0;
}

//�������
LRESULT CGameClientEngine::OnSendCardFinish(WPARAM wParam, LPARAM lParam)
{
  //��Ӧ�˿�
  WORD wMeChairID=GetMeChairID();
  if(!IsLookonMode())
  {
    m_GameClientView.m_CardControl[MY_VIEW_CHAIRID].SetPositively(true);
  }

  //����ʱ��
  SetGameClock(GetMeChairID(),IDI_NULLITY,TIME_USER_OPEN_CARD);
  SetTimer(IDI_TIME_NULLITY,1000,NULL);

  //�ȴ���־
  m_GameClientView.m_bOpenCard=true;

  //���ǹۿ�
  if(IsLookonMode())
  {
    return 0;
  }

  //��������
  BYTE bCardType = m_GameLogic.GetCardType(m_cbHandCardData[wMeChairID],MAX_COUNT);
  if(bCardType >= OX_FOUR_KING)
  {
    //��ʾ��Ϣ
    m_GameClientView.SetUserAction(true);

    //�û���Ϣ
    WORD wViewChairID=SwitchViewChairID(wMeChairID);
    m_GameClientView.m_CardControl[wViewChairID].ShootAllCard(false);

    //������Ϣ
    CMD_C_OxCard OxCard;
    OxCard.bOX=TRUE;
    CopyMemory(OxCard.cbOxCardData, m_cbHandCardData[wMeChairID], MAX_COUNT*sizeof(BYTE));
    SendSocketData(SUB_C_OPEN_CARD,&OxCard,sizeof(OxCard));

    //Ԥ�ȴ���
    m_GameClientView.ShowOpenCard(wViewChairID);
    m_GameClientView.m_CardControl[wViewChairID].SetPositively(false);

    //��ʾ����
    m_GameClientView.SetUserOxValue(wViewChairID,bCardType);

    //����ţ��Ϣ
    m_cbUserOxCard[wMeChairID]=OxCard.bOX;

    return 0;
  }

  //�ؼ�����
  if(!m_GameClientView.m_CardControl[MY_VIEW_CHAIRID].GetKey())
  {
    m_GameClientView.m_btOx.EnableWindow(FALSE);
  }

  if(!m_cbDynamicJoin)
  {
    m_GameClientView.m_btOpenCard.ShowWindow(SW_SHOW);
    m_GameClientView.m_btHintOx.ShowWindow(SW_SHOW);
  }

  //ʱ������
  SetTimer(IDI_TIME_OPEN_CARD,TIME_USER_OPEN_CARD*1000,NULL);

  return 0;
}

//̯�ư�ť
LRESULT CGameClientEngine::OnOpenCard(WPARAM wParam, LPARAM lParam)
{

  BYTE cbCardData[MAX_COUNT];
  int nCount = m_GameClientView.m_CardControl[MY_VIEW_CHAIRID].GetShootCard(cbCardData, true);
  if(nCount == 0 && m_bTimeOut == false)
  {
    OnHintOx(0, 0);
  }

  if(m_GameClientView.m_btOx.IsWindowEnabled()==TRUE)
  {
    //ţţ
    m_GameClientView.m_CardControl[MY_VIEW_CHAIRID].SetOX(true);

    //ȡ����Ӧ
    m_GameClientView.m_CardControl[MY_VIEW_CHAIRID].SetPositively(false);

    //����ţ��
    OnSortCard(0,0);

    m_GameClientView.m_btOx.EnableWindow(FALSE);
  }

  //ɾ��ʱ��
  KillTimer(IDI_TIME_OPEN_CARD);

  //����ؼ�
  m_GameClientView.m_btOx.ShowWindow(SW_HIDE);
  m_GameClientView.m_btOpenCard.ShowWindow(SW_HIDE);
  m_GameClientView.m_btHintOx.ShowWindow(SW_HIDE);

  //������Ϣ
  CMD_C_OxCard OxCard;
  if(m_GameClientView.m_CardControl[MY_VIEW_CHAIRID].GetOX() == TRUE)
  {
    OxCard.bOX = TRUE;
    CopyMemory(OxCard.cbOxCardData, m_cbOxCardData, sizeof(m_cbOxCardData));
  }
  else
  {
    OxCard.bOX = FALSE;
    CopyMemory(OxCard.cbOxCardData, m_cbHandCardData[GetMeChairID()], MAX_COUNT*sizeof(BYTE));
  }
  SendSocketData(SUB_C_OPEN_CARD, &OxCard, sizeof(OxCard));

  //Ԥ�ȴ���
  if(OxCard.bOX == FALSE)
  {
    //��ʾ������ţţ
    m_GameClientView.m_CardControl[MY_VIEW_CHAIRID].ShootAllCard(false);
    m_GameClientView.m_CardControl[MY_VIEW_CHAIRID].SetShowCard(true);
    m_GameClientView.SetUserOxValue(MY_VIEW_CHAIRID,0);
  }
  m_GameClientView.ShowOpenCard(MY_VIEW_CHAIRID);
  m_GameClientView.m_CardControl[MY_VIEW_CHAIRID].SetPositively(false);

  //����ţ��Ϣ
  m_cbUserOxCard[GetMeChairID()]=OxCard.bOX;

  PlayGameSound(AfxGetInstanceHandle(),TEXT("OPEN_CARD"));

  //���½���
  m_GameClientView.RefreshGameView();

  return 0;
}

//��ʾ����
LRESULT CGameClientEngine::OnSetUserOxValue(WPARAM wParam, LPARAM lParam)
{
  WORD wViewChairID=SwitchViewChairID(GetMeChairID());

  //��ȡ����
  BYTE bCardData[MAX_COUNT],bCardCount=0;
  ZeroMemory(bCardData,sizeof(bCardData));
  bCardCount=(BYTE)m_GameClientView.m_CardControlOx[wViewChairID].GetCardCount();
  ASSERT(bCardCount==2);
  m_GameClientView.m_CardControlOx[wViewChairID].GetCardData(bCardData,bCardCount);

  //��ʾ����
  BYTE bSum=m_GameLogic.GetCardLogicValue(bCardData[0])+m_GameLogic.GetCardLogicValue(bCardData[1]);
  if(bSum>10)
  {
    bSum-=10;
  }
  m_GameClientView.SetUserOxValue(wViewChairID,bSum);

  return 0;
}

//��ʾ��Ϣ
LRESULT CGameClientEngine::OnShowInfo(WPARAM wParam, LPARAM lParam)
{
  //��������
  TCHAR szBuffer[64];
  _sntprintf(szBuffer,CountArray(szBuffer),TEXT("����ʾ��Ϣ��:���ܹ���ţ"));
  m_pIStringMessage->InsertNormalString(szBuffer);

  return 0;
}

//ţBTΪ��
LRESULT CGameClientEngine::OnOxEnable(WPARAM wParam, LPARAM lParam)
{
  BYTE bEnable=BYTE(wParam);
  m_GameClientView.m_btOx.EnableWindow(bEnable);
  if(bEnable==TRUE)
  {
    bool bShoot = ((BYTE(lParam)==TRUE)?false:true);
    BYTE cbCardData[MAX_COUNT];
    m_GameClientView.m_CardControl[MY_VIEW_CHAIRID].GetShootCard(cbCardData,bShoot);

    BYTE cbValue = 0;
    if(cbCardData[0] != 0x4E && cbCardData[0] != 0x4F &&
       cbCardData[1] != 0x4E && cbCardData[1] != 0x4F)
    {
      cbValue = (m_GameLogic.GetCardLogicValue(cbCardData[0]) + m_GameLogic.GetCardLogicValue(cbCardData[1])) % 10;

      switch(cbValue)
      {
        case 1:
          cbValue = OX_VALUE_BIG_1;
          break;
        case 2:
          cbValue = OX_VALUE_BIG_2;
          break;
        case 3:
          cbValue = OX_VALUE_BIG_3;
          break;
        case 4:
          cbValue = OX_VALUE_BIG_4;
          break;
        case 5:
          cbValue = OX_VALUE_BIG_5;
          break;
        case 6:
          cbValue = OX_VALUE_BIG_6;
          break;
        case 7:
          cbValue = OX_VALUE_BIG_7;
          break;
        case 8:
          cbValue = OX_VALUE_BIG_8;
          break;
        case 9:
          cbValue = OX_VALUE_BIG_9;
          break;
        case 0:
          cbValue = OX_VALUE_BIG_10;
          break;
      }
    }
    else
    {
      cbValue = OX_VALUE_BIG_10;
    }

    m_GameClientView.SetUserOxValue(MY_VIEW_CHAIRID, cbValue);
  }
  else
  {
    BYTE cbValue=m_GameClientView.GetValue(MY_VIEW_CHAIRID);
    if(cbValue!=0xFF && cbValue!=0)
    {
      m_GameClientView.SetUserOxValue(MY_VIEW_CHAIRID,0xFF);
    }
  }

  return 0;
}

//�˿˷���
LRESULT CGameClientEngine::OnSortCard(WPARAM wParam, LPARAM lParam)
{
  //���ؿؼ�
  m_GameClientView.m_btHintOx.ShowWindow(SW_HIDE);

  //���ñ���
  WORD wViewChairID=SwitchViewChairID(GetMeChairID());

  //������Ŀ
  BYTE bCardCount=(BYTE)m_GameClientView.m_CardControl[wViewChairID].GetShootCount();

  //���ñ���
  BYTE bCardOxData[MAX_COUNT];
  BYTE bCardValueData[MAX_COUNT];
  ZeroMemory(bCardOxData,sizeof(bCardOxData));
  ZeroMemory(bCardValueData,sizeof(bCardValueData));

  //��ȡţ��&������
  if(bCardCount==2)
  {
    BYTE bCount=(BYTE)m_GameClientView.m_CardControl[wViewChairID].GetShootCard(bCardOxData,false);
    ASSERT(bCount==3);
    bCount=(BYTE)m_GameClientView.m_CardControl[wViewChairID].GetShootCard(bCardValueData);
    ASSERT(bCount==2);
  }
  else
  {
    BYTE bCount=(BYTE)m_GameClientView.m_CardControl[wViewChairID].GetShootCard(bCardValueData,false);
    ASSERT(bCount==2);
    bCount=(BYTE)m_GameClientView.m_CardControl[wViewChairID].GetShootCard(bCardOxData);
    ASSERT(bCount==3);
  }

  //�����˿�
  m_GameClientView.m_CardControlOx[wViewChairID].SetCardData(bCardValueData,MAX_COUNT-3);
  m_GameClientView.m_CardControl[wViewChairID].SetCardData(bCardOxData,3);

  //�����˿�
  CopyMemory(m_cbOxCardData, bCardOxData, 3*sizeof(BYTE));
  CopyMemory(m_cbOxCardData+3, bCardValueData, 2*sizeof(BYTE));

  return 0;
}

//���Ű�ť
LRESULT CGameClientEngine::OnReSortCard(WPARAM wParam, LPARAM lParam)
{
  WORD wMeChiarID=GetMeChairID();
  WORD wViewChairID=SwitchViewChairID(wMeChiarID);

  //ȡ��ţţ
  if(m_GameClientView.m_CardControl[wViewChairID].GetOX())
  {
    m_GameClientView.m_CardControlOx[wViewChairID].SetCardData(NULL,0);
    m_GameClientView.m_CardControl[wViewChairID].SetCardData(m_cbHandCardData[wMeChiarID],MAX_COUNT);
    m_GameClientView.m_CardControl[wViewChairID].SetPositively(true);
    m_GameClientView.m_CardControl[wViewChairID].SetOX(false);

    //ȡ����ʾ����
    m_GameClientView.SetUserOxValue(wViewChairID,0xFF);
  }

  return 0;
}

//�����Ϣ
LRESULT CGameClientEngine::OnStorageInfo(WPARAM wParam,LPARAM lParam)
{
  SendSocketData(SUB_C_MODIFY_STORAGE, (CMD_C_ModifyStorage*)wParam, sizeof(CMD_C_ModifyStorage));
  return true;
}

//��������
LRESULT CGameClientEngine::OnAddUserRoster(WPARAM wParam,LPARAM lParam)
{
  SendSocketData(SUB_C_REQUEST_ADD_USERROSTER, (CMD_C_Request_Add_Userroster*)wParam, sizeof(CMD_C_Request_Add_Userroster));
  return true;
}

//ɾ������
LRESULT CGameClientEngine::OnDeleteUserRoster(WPARAM wParam,LPARAM lParam)
{
  SendSocketData(SUB_C_REQUEST_DELETE_USERROSTER, (CMD_C_Request_Delete_Userroster*)wParam, sizeof(CMD_C_Request_Delete_Userroster));
  return true;
}

//�����������
LRESULT CGameClientEngine::OnRequestUpdateUserRoster(WPARAM wParam,LPARAM lParam)
{
  SendSocketData(SUB_C_REQUEST_UPDATE_USERROSTER);
  return true;
}

//��ʱ����
void CGameClientEngine::OnTimer(UINT nIDEvent)
{
  switch(nIDEvent)
  {
    case IDI_TIME_OPEN_CARD:
    {
      //ɾ����ʱ��
      KillTimer(IDI_TIME_OPEN_CARD);

      //���ñ���
      m_bTimeOut = true;

      //̯����Ϣ
      OnOpenCard(0, 0);

      return;
    }
    case IDI_TIME_NULLITY:
    {
      //���½���
      m_GameClientView.InvalidGameView(0,0,0,0);

      return;
    }
  }

  __super::OnTimer(nIDEvent);
}

//////////////////////////////////////////////////////////////////////////

