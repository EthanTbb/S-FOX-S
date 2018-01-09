#include "StdAfx.h"
#include "TableFrameSink.h"

//////////////////////////////////////////////////////////////////////////

#define IDI_SO_OPERATE              2       //����ʱ��
#define TIME_SO_OPERATE             40000     //����ʱ��

//������ʶ
#define IDI_DELAY_ENDGAME           10        //������ʶ
#define IDI_DELAY_TIME              3000      //��ʱʱ��


//////////////////////////////////////////////////////////////////////////
//ȫ�ֱ���
LONGLONG          g_lStorageStart = 0;            //�����ʼֵ
LONGLONG          g_lStorageCurrent = 0;            //��ǰ���
LONGLONG          g_lStorageDeduct = 0;           //�ؿ۱���
LONGLONG          g_lStorageMax = 50000000;         //���ⶥ
WORD            g_wStorageMul = 0;              //ϵͳ��Ǯ����

//���������Ϣ
CMap<DWORD, DWORD, ROOMUSERINFO, ROOMUSERINFO> g_MapRoomUserInfo; //���USERIDӳ�������Ϣ

//���캯��
CTableFrameSink::CTableFrameSink()
{
  //��Ϸ����
  m_lCellScore = 0;
  m_lExitScore = 0;
  m_wBankerUser = INVALID_CHAIR;

  //�û�״̬
  ZeroMemory(m_lTableScore, sizeof(m_lTableScore));
  ZeroMemory(m_cbPlayStatus, sizeof(m_cbPlayStatus));
  ZeroMemory(m_cbDynamicJoin, sizeof(m_cbDynamicJoin));
  for(BYTE i = 0; i < GAME_PLAYER; i++)
  {
    m_cbOxCard[i] = 0xFF;
  }

  //�˿˱���
  ZeroMemory(m_cbOxCardData, sizeof(m_cbOxCardData));
  ZeroMemory(m_cbHandCardData, sizeof(m_cbHandCardData));

  //�������
  m_pITableFrame=NULL;
  m_pGameServiceOption=NULL;
  m_pGameCustomRule = NULL;

  //�������
  m_hInst = NULL;
  m_pServerContro = NULL;
  m_hInst = LoadLibrary(TEXT("OxSixExServerControl.dll"));
  if(m_hInst != NULL)
  {
    typedef void * (*CREATE)();
    CREATE ServerControl = (CREATE)GetProcAddress(m_hInst,"CreateServerControl");
    if(ServerControl != NULL)
    {
      m_pServerContro = static_cast<IServerControl*>(ServerControl());
    }
  }

  return;
}

//��������
CTableFrameSink::~CTableFrameSink(void)
{
  if(m_pServerContro != NULL)
  {
    delete m_pServerContro;
    m_pServerContro = NULL;
  }

  if(m_hInst != NULL)
  {
    FreeLibrary(m_hInst);
    m_hInst = NULL;
  }
}

//�ӿڲ�ѯ--��������Ϣ�汾
void * CTableFrameSink::QueryInterface(const IID & Guid, DWORD dwQueryVer)
{
  QUERYINTERFACE(ITableFrameSink,Guid,dwQueryVer);
  QUERYINTERFACE(ITableUserAction,Guid,dwQueryVer);
  QUERYINTERFACE_IUNKNOWNEX(ITableFrameSink,Guid,dwQueryVer);
  return NULL;
}

//��ʼ��
bool CTableFrameSink::Initialization(IUnknownEx * pIUnknownEx)
{
  //��ѯ�ӿ�
  ASSERT(pIUnknownEx!=NULL);
  m_pITableFrame=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,ITableFrame);
  if(m_pITableFrame==NULL)
  {
    return false;
  }

  m_pITableFrame->SetStartMode(START_MODE_ALL_READY);

  //��Ϸ����
  m_pGameServiceAttrib=m_pITableFrame->GetGameServiceAttrib();
  m_pGameServiceOption=m_pITableFrame->GetGameServiceOption();

  //�Զ�����
  ASSERT(m_pITableFrame->GetCustomRule()!=NULL);
  m_pGameCustomRule=(tagCustomRule *)m_pITableFrame->GetCustomRule();

  //���õ׷�
  m_lCellScore = __max(m_pGameServiceOption->lCellScore,m_pGameServiceOption->lCellScore*10);

  //��ȡ���
  tagCustomRule *pGameCustomRule = (tagCustomRule *)m_pITableFrame->GetCustomRule();
  ASSERT(pGameCustomRule != NULL);

  //��ʼ�����
  g_lStorageCurrent = pGameCustomRule->lStorageStart;
  g_lStorageStart = pGameCustomRule->lStorageStart;
  g_lStorageDeduct = pGameCustomRule->lStorageDeduct;
  g_lStorageMax = pGameCustomRule->lStorageMax;
  g_wStorageMul = (WORD)(pGameCustomRule->lStorageMul);
  m_lBonus = pGameCustomRule->lBonus;

  //У������
  if(g_lStorageDeduct < 0 || g_lStorageDeduct > 1000)
  {
    g_lStorageDeduct = 0;
  }

  if(g_lStorageDeduct > 1000)
  {
    g_lStorageDeduct = 1000;
  }

  if(g_wStorageMul < 0 || g_wStorageMul > 100)
  {
    g_wStorageMul = 50;
  }


  return true;
}

//��λ����
void CTableFrameSink::RepositionSink()
{
  //��Ϸ����
  m_lExitScore=0;

  //�û�״̬
  ZeroMemory(m_lTableScore, sizeof(m_lTableScore));
  ZeroMemory(m_cbPlayStatus, sizeof(m_cbPlayStatus));
  ZeroMemory(m_cbDynamicJoin, sizeof(m_cbDynamicJoin));
  for(BYTE i = 0; i < GAME_PLAYER; i++)
  {
    m_cbOxCard[i] = 0xFF;
  }

  //�˿˱���
  ZeroMemory(m_cbOxCardData, sizeof(m_cbOxCardData));
  ZeroMemory(m_cbHandCardData,sizeof(m_cbHandCardData));

  return;
}

//��Ϸ״̬
bool CTableFrameSink::IsUserPlaying(WORD wChairID)
{
  ASSERT(wChairID<GAME_PLAYER && m_cbPlayStatus[wChairID]==TRUE);
  if(wChairID<GAME_PLAYER && m_cbPlayStatus[wChairID]==TRUE)
  {
    return true;
  }
  return false;
}


//�û�����
bool CTableFrameSink::OnActionUserSitDown(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
  //��ʷ����
  if(bLookonUser==false)
  {
    m_HistoryScore.OnEventUserEnter(pIServerUserItem->GetChairID());
  }

  //��̬����
  if(m_pITableFrame->GetGameStatus()!=GS_TK_FREE)
  {
    m_cbDynamicJoin[pIServerUserItem->GetChairID()]=true;
  }

  //���˻�����
  if(pIServerUserItem->IsAndroidUser() == false)
  {
    //�����Ѵ��ڵ��û�
    ROOMUSERINFO lastroomUserInfo;
    ZeroMemory(&lastroomUserInfo, sizeof(lastroomUserInfo));
    if(g_MapRoomUserInfo.Lookup(pIServerUserItem->GetUserID(), lastroomUserInfo) == false)
    {
      UpdateUserRosterInfo(pIServerUserItem);
    }
    else //�����û�
    {
      RemoveUserRoster();
    }
  }

  return true;
}

//�û�����
bool CTableFrameSink::OnActionUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
  //��ʷ����
  if(bLookonUser==false)
  {
    m_cbDynamicJoin[pIServerUserItem->GetChairID()]=false;
    m_HistoryScore.OnEventUserLeave(pIServerUserItem->GetChairID());
  }

  RemoveUserRoster();

  return true;
}

//��Ϸ��ʼ
bool CTableFrameSink::OnEventGameStart()
{
  //�������
  srand(GetTickCount());

  //����״̬
  m_pITableFrame->SetGameStatus(GS_TK_PLAYING);

  //���˥��
  if(g_lStorageCurrent > 0 && NeedDeductStorage())
  {
    g_lStorageCurrent = g_lStorageCurrent - g_lStorageCurrent * g_lStorageDeduct / 1000;
  }
  CString strInfo;
  strInfo.Format(TEXT("��%s�� ��ǰ��棺%I64d"), m_pGameServiceOption->szServerName, g_lStorageCurrent);
  NcaTextOut(strInfo);

  //�û�״̬
  for(WORD i=0; i<GAME_PLAYER; i++)
  {
    //��ȡ�û�
    IServerUserItem *pIServerUser=m_pITableFrame->GetTableUserItem(i);

    if(pIServerUser==NULL)
    {
      m_cbPlayStatus[i]=FALSE;
    }
    else
    {
      m_cbPlayStatus[i]=TRUE;
    }
  }

  //���ׯ��
  if(m_wBankerUser == INVALID_CHAIR)
  {
    m_wBankerUser = 0;
    //m_wBankerUser=rand()%GAME_PLAYER;
  }
  while(m_cbPlayStatus[m_wBankerUser] != TRUE)
  {
    m_wBankerUser = (m_wBankerUser + 1) % GAME_PLAYER;
  }

  //��ע���
  for(WORD i = 0; i < GAME_PLAYER; i++)
  {
    //�û�����
    if(m_cbPlayStatus[i] == FALSE)
    {
      continue;
    }

    m_lTableScore[i] = m_lCellScore;
  }

  //����˿�
  BYTE bTempArray[GAME_PLAYER*MAX_COUNT];
  m_GameLogic.RandCardList(bTempArray, sizeof(bTempArray));
  for(WORD i = 0; i < GAME_PLAYER; i++)
  {
    if(m_cbPlayStatus[i] == FALSE)
    {
      continue;
    }

    //�ɷ��˿�
    CopyMemory(m_cbHandCardData[i], &bTempArray[i*MAX_COUNT], MAX_COUNT);
  }

  ////TEST+
  //BYTE cbCardTemp[6][5] =
  //{
  //  0x04, 0x36, 0x4F, 0x07, 0x01,
  //  0x0B, 0x2B, 0x1B, 0x3C, 0x1A,
  //  0x09, 0x3D, 0x4E, 0x33, 0x15,
  //  0x0A, 0x1B, 0x1A, 0x0C, 0x1D,
  //  0x34, 0x35, 0x17, 0x13, 0x3B,
  //  0x0A, 0x1B, 0x1A, 0x0C, 0x1D
  //};
  //CopyMemory(m_cbHandCardData, cbCardTemp, 30*sizeof(BYTE));
  ////TEST+

  BYTE cbWhiteRosterCount = 0;
  BYTE cbBlackRosterCount = 0;
  BYTE cbInvalidRosterCount = 0;
  BYTE cbAndroidCount = 0;

  for(WORD i=0; i<GAME_PLAYER; i++)
  {
    //��ȡ�û�
    IServerUserItem *pIServerUserItem = m_pITableFrame->GetTableUserItem(i);

    if(!pIServerUserItem)
    {
      continue;
    }

    if(pIServerUserItem->IsAndroidUser())
    {
      cbAndroidCount++;
    }

    //�����Ѵ��ڵ��û�
    ROOMUSERINFO lastroomUserInfo;
    ZeroMemory(&lastroomUserInfo, sizeof(lastroomUserInfo));
    if(g_MapRoomUserInfo.Lookup(pIServerUserItem->GetUserID(), lastroomUserInfo) == TRUE)
    {
      if(lastroomUserInfo.userroster == WHITE_ROSTER)
      {
        cbWhiteRosterCount++;
      }
      else if(lastroomUserInfo.userroster == BLACK_ROSTER)
      {
        cbBlackRosterCount++;
      }
      else if(lastroomUserInfo.userroster == INVALID_ROSTER)
      {
        cbInvalidRosterCount++;
      }
    }


  }

  //��Ϊ�����˻�����Ч����
  if(cbAndroidCount == GAME_PLAYER || cbInvalidRosterCount == GAME_PLAYER
     || (cbInvalidRosterCount != 0 && cbWhiteRosterCount == 0 && cbBlackRosterCount == 0) || cbWhiteRosterCount == GAME_PLAYER || cbBlackRosterCount == GAME_PLAYER)
  {
    //�����˿�
    AnalyseCard();
  }
  else if(cbWhiteRosterCount != 0 || cbBlackRosterCount != 0)
  {
    USERROSTER userroster[GAME_PLAYER] = { INVALID_ROSTER, INVALID_ROSTER, INVALID_ROSTER, INVALID_ROSTER, INVALID_ROSTER, INVALID_ROSTER};

    //�����û�����
    for(WORD i=0; i<GAME_PLAYER; i++)
    {
      //��ȡ�û�
      IServerUserItem *pIServerUserItem = m_pITableFrame->GetTableUserItem(i);

      if(!pIServerUserItem)
      {
        continue;

      }

      //�����Ѵ��ڵ��û�
      ROOMUSERINFO lastroomUserInfo;
      ZeroMemory(&lastroomUserInfo, sizeof(lastroomUserInfo));
      if(g_MapRoomUserInfo.Lookup(pIServerUserItem->GetUserID(), lastroomUserInfo))
      {
        userroster[i] = lastroomUserInfo.userroster;
      }
    }

    m_pServerContro->ControlResult(m_cbHandCardData, userroster);
  }

  //���ñ���
  CMD_S_GameStart GameStart;
  GameStart.lCellScore = m_lCellScore;
  GameStart.wBankerUser = m_wBankerUser;
  CopyMemory(GameStart.cbPlayStatus, m_cbPlayStatus, sizeof(m_cbPlayStatus));

  //�����˿�
  for(WORD i = 0; i < GAME_PLAYER; i++)
  {
    if(m_cbPlayStatus[i] == FALSE)
    {
      continue;
    }

    //�ɷ��˿�
    CopyMemory(GameStart.cbCardData[i], m_cbHandCardData[i], MAX_COUNT);
  }

  //��������
  for(WORD i = 0; i < GAME_PLAYER; i++)
  {
    if(m_cbPlayStatus[i] == FALSE && m_cbDynamicJoin[i] == FALSE)
    {
      continue;
    }
    m_pITableFrame->SendTableData(i, SUB_S_GAME_START, &GameStart, sizeof(GameStart));
  }
  m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_GAME_START, &GameStart, sizeof(GameStart));
  m_pITableFrame->SetGameTimer(IDI_SO_OPERATE, TIME_SO_OPERATE, 1, 0);

  return true;
}

//�˿˷���
void CTableFrameSink::AnalyseCard()
{
  //��������
  WORD wAiCount = 0;
  WORD wPlayerCount = 0;
  for(WORD i = 0; i < GAME_PLAYER; i++)
  {
    //��ȡ�û�
    IServerUserItem * pIServerUserItem=m_pITableFrame->GetTableUserItem(i);
    if(pIServerUserItem != NULL)
    {
      if(m_cbPlayStatus[i] == FALSE && m_cbDynamicJoin[i] == FALSE)
      {
        continue;
      }
      if(pIServerUserItem->IsAndroidUser())
      {
        wAiCount++ ;
      }
      wPlayerCount++;
    }
  }

  //ȫ������
  if(wPlayerCount == wAiCount || wAiCount==0)
  {
    return;
  }

  //��������˷���
  WORD wWinUser = INVALID_CHAIR;
  LONGLONG lAndroidScore = CalculateAndroidScore(wWinUser);
  ASSERT(wWinUser != INVALID_CHAIR);

  //�⸶����
  double dclaimraido = 0.3;

  // �ͷ��ж�
  if(g_lStorageCurrent > 0 && g_lStorageCurrent > g_lStorageMax  && rand()%100 < g_wStorageMul)
  {
    //��ȡ�û�
    IServerUserItem * pIWinUserUserItem = m_pITableFrame->GetTableUserItem(wWinUser);

    //�ǻ�����Ӯ
    if(pIWinUserUserItem != NULL && pIWinUserUserItem->IsAndroidUser())
    {
      //���Ӯ��
      WORD wWinPlay = INVALID_CHAIR;

      INT nCirculateCount = 0;

      while(true)
      {
        do
        {
          wWinPlay = rand()%GAME_PLAYER;
        }
        while(m_pITableFrame->GetTableUserItem(wWinPlay) == NULL || m_pITableFrame->GetTableUserItem(wWinPlay)->IsAndroidUser());

        //��������
        BYTE cbTempData[MAX_COUNT];
        CopyMemory(cbTempData, m_cbHandCardData[wWinPlay], MAX_COUNT*sizeof(BYTE));
        CopyMemory(m_cbHandCardData[wWinPlay], m_cbHandCardData[wWinUser], MAX_COUNT*sizeof(BYTE));
        CopyMemory(m_cbHandCardData[wWinUser], cbTempData, MAX_COUNT*sizeof(BYTE));

        lAndroidScore = CalculateAndroidScore(wWinUser);
        nCirculateCount++;

        if(g_lStorageCurrent + lAndroidScore > g_lStorageCurrent * dclaimraido)
        {
          break;
        }

        if(nCirculateCount >= GAME_PLAYER)
        {
          //�����������������Ƹ�ֵ������һ��������
          WORD wRandAndroid = INVALID_CHAIR;
          do
          {
            wRandAndroid = rand()%GAME_PLAYER;
          }
          while(m_pITableFrame->GetTableUserItem(wRandAndroid) == NULL || !(m_pITableFrame->GetTableUserItem(wRandAndroid)->IsAndroidUser()));

          //��������
          BYTE cbTempData[MAX_COUNT];
          CopyMemory(cbTempData, m_cbHandCardData[wRandAndroid], MAX_COUNT*sizeof(BYTE));
          CopyMemory(m_cbHandCardData[wRandAndroid], m_cbHandCardData[wWinUser], MAX_COUNT*sizeof(BYTE));
          CopyMemory(m_cbHandCardData[wWinUser], cbTempData, MAX_COUNT*sizeof(BYTE));

          break;
        }
      }
    }
    else
    {
      goto LABEL;
    }

    return;

  }

LABEL:
  //�жϿ��
  if(g_lStorageCurrent + lAndroidScore <= 0)
  {
    //��ȡ�û�
    IServerUserItem * pIWinUserUserItem = m_pITableFrame->GetTableUserItem(wWinUser);

    //�ǻ�����Ӯ
    if(pIWinUserUserItem != NULL && !pIWinUserUserItem->IsAndroidUser())
    {
      //���Ӯ��
      WORD wWinAndroid = INVALID_CHAIR;

      do
      {
        wWinAndroid = rand()%GAME_PLAYER;
      }
      while(m_pITableFrame->GetTableUserItem(wWinAndroid) == NULL || !m_pITableFrame->GetTableUserItem(wWinAndroid)->IsAndroidUser());

      //��������
      BYTE cbTempData[MAX_COUNT];
      CopyMemory(cbTempData, m_cbHandCardData[wWinAndroid], MAX_COUNT*sizeof(BYTE));
      CopyMemory(m_cbHandCardData[wWinAndroid], m_cbHandCardData[wWinUser], MAX_COUNT*sizeof(BYTE));
      CopyMemory(m_cbHandCardData[wWinUser], cbTempData, MAX_COUNT*sizeof(BYTE));
    }
  }

  return;
}


//��Ϸ����
bool CTableFrameSink::OnEventGameConclude(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)
{
  switch(cbReason)
  {
    case GER_DISMISS:   //��Ϸ��ɢ
    {
      //Ч�����
      ASSERT(pIServerUserItem!=NULL);
      ASSERT(wChairID<GAME_PLAYER);

      //��������
      CMD_S_GameEnd GameEnd = {0};

      //������Ϣ
      m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
      m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));

      //������Ϸ
      m_pITableFrame->ConcludeGame(GAME_STATUS_FREE);

      return true;
    }
    case GER_NORMAL:    //�������
    {
      //ɾ��ʱ��
      m_pITableFrame->KillGameTimer(IDI_SO_OPERATE);

      //�������
      CMD_S_GameEnd GameEnd;
      ZeroMemory(&GameEnd,sizeof(GameEnd));

      //�����˿�
      BYTE cbUserCardData[GAME_PLAYER][MAX_COUNT];
      CopyMemory(cbUserCardData, m_cbOxCardData, sizeof(cbUserCardData));

      //Ѱ�����
      for(WORD wWinner = 0; wWinner < GAME_PLAYER; wWinner++)
      {
        if(m_cbPlayStatus[wWinner] == TRUE)
        {
          break;
        }
      }

      //�Ա����
      for(WORD i = (wWinner + 1); i < GAME_PLAYER; i++)
      {
        //�û�����
        if(m_cbPlayStatus[i] == FALSE)
        {
          continue;
        }

        //�Ա��˿�
        if(m_GameLogic.CompareCard(cbUserCardData[i], cbUserCardData[wWinner], MAX_COUNT, m_cbOxCard[i], m_cbOxCard[wWinner]) == true)
        {
          wWinner = i;
        }
      }

      //��Ϸ����
      WORD wWinTimes = 0;
      ASSERT(m_cbOxCard[wWinner]<2);
      if(m_cbOxCard[wWinner]==FALSE)
      {
        wWinTimes = 1;
      }
      else
      {
        wWinTimes = m_GameLogic.GetTimes(cbUserCardData[wWinner], MAX_COUNT);
      }

      //ͳ�Ƶ÷�
      for(WORD i = 0; i < GAME_PLAYER; i++)
      {
        //�������
        if(i == wWinner || m_cbPlayStatus[i] == FALSE)
        {
          continue;
        }

        GameEnd.lGameScore[i] -= m_lTableScore[i]*wWinTimes;
        GameEnd.lGameScore[wWinner] += m_lTableScore[i]*wWinTimes;

        if((m_GameLogic.GetCardType(m_cbHandCardData[wWinner], MAX_COUNT) == OX_FIVE_KING ? true : false))
        {
          GameEnd.lGameScore[wWinner] += m_lBonus;
        }
      }

      //ǿ�˷���
      GameEnd.lGameScore[wWinner] += m_lExitScore;

      //�廨ţ
      for(WORD i=0; i<GAME_PLAYER; i++)
      {
        //��ȡ�û�
        IServerUserItem * pIServerUserItem=m_pITableFrame->GetTableUserItem(i);
        if(pIServerUserItem == NULL)
        {
          continue;
        }

        GameEnd.bfiveKing[i] = (m_GameLogic.GetCardType(m_cbHandCardData[i], MAX_COUNT) == OX_FIVE_KING ? true : false);

        if(GameEnd.bfiveKing[i] && !pIServerUserItem->IsAndroidUser())
        {
          CString strInfo;
          strInfo.Format(TEXT("�������[%s]�����廨ţ����òʽ�[%I64d]\n"), pIServerUserItem->GetNickName(), m_lBonus);
          NcaTextOut(strInfo);

          g_lStorageCurrent -= m_lBonus;
        }
      }

      //�޸Ļ���
      tagScoreInfo ScoreInfoArray[GAME_PLAYER];
      ZeroMemory(ScoreInfoArray,sizeof(ScoreInfoArray));

      bool bDelayOverGame=false;

      //����˰��
      for(WORD i = 0; i < GAME_PLAYER; i++)
      {
        if(m_cbPlayStatus[i] == FALSE)
        {
          continue;
        }

        if(GameEnd.lGameScore[i] > 0L)
        {
          GameEnd.lGameTax[i] = m_pITableFrame->CalculateRevenue(i,GameEnd.lGameScore[i]);
          if(GameEnd.lGameTax[i] > 0)
          {
            GameEnd.lGameScore[i] -= GameEnd.lGameTax[i];
          }
        }

        //��ʷ����
        m_HistoryScore.OnEventUserScore(i,GameEnd.lGameScore[i]);

        //�������
        ScoreInfoArray[i].lScore = GameEnd.lGameScore[i];
        ScoreInfoArray[i].lRevenue = GameEnd.lGameTax[i];
        ScoreInfoArray[i].cbType = (GameEnd.lGameScore[i] > 0L) ? SCORE_TYPE_WIN : SCORE_TYPE_LOSE;

        if(ScoreInfoArray[i].cbType ==SCORE_TYPE_LOSE && bDelayOverGame==false)
        {
          IServerUserItem *pUserItem = m_pITableFrame->GetTableUserItem(i);
          if(pUserItem!=NULL && (pUserItem->GetUserScore() + GameEnd.lGameScore[i]) < m_pGameServiceOption->lMinTableScore)
          {
            bDelayOverGame=true;
          }
        }
      }

      if(bDelayOverGame)
      {
        GameEnd.cbDelayOverGame = 3;
      }
      CopyMemory(GameEnd.cbCardData, m_cbOxCardData, sizeof(m_cbOxCardData));

      //������Ϣ
      for(WORD i = 0; i < GAME_PLAYER; i++)
      {
        if(m_cbPlayStatus[i] == FALSE && m_cbDynamicJoin[i] == FALSE)
        {
          continue;
        }
        m_pITableFrame->SendTableData(i, SUB_S_GAME_END, &GameEnd, sizeof(GameEnd));
      }

      m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));

      TryWriteTableScore(ScoreInfoArray);

      //���ͳ��
      for(WORD i=0; i<GAME_PLAYER; i++)
      {
        //��ȡ�û�
        IServerUserItem * pIServerUserIte=m_pITableFrame->GetTableUserItem(i);
        if(pIServerUserIte==NULL)
        {
          continue;
        }

        //����ۼ�
        if(!pIServerUserIte->IsAndroidUser())
        {
          g_lStorageCurrent-=GameEnd.lGameScore[i];
        }

      }

      //�����û�����
      for(WORD i=0; i<GAME_PLAYER; i++)
      {
        //��ȡ�û�
        IServerUserItem *pIServerUserItem = m_pITableFrame->GetTableUserItem(i);

        if(!pIServerUserItem)
        {
          continue;
        }

        LONGLONG lGameScore = 0;

        //�����Ѵ��ڵ��û�
        ROOMUSERINFO lastroomUserInfo;
        ZeroMemory(&lastroomUserInfo, sizeof(lastroomUserInfo));
        if(g_MapRoomUserInfo.Lookup(pIServerUserItem->GetUserID(), lastroomUserInfo)
           && lastroomUserInfo.userroster != INVALID_ROSTER)
        {
          lGameScore = lastroomUserInfo.lGameScore + GameEnd.lGameScore[i];
          UpdateUserRosterInfo(pIServerUserItem, lGameScore, lastroomUserInfo.userroster, lastroomUserInfo.lScoreLimit);
          RemoveUserRoster();
        }
      }

      //��Ϸ���
      for(WORD wChairID=0; wChairID<GAME_PLAYER; ++wChairID)
      {
        IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(wChairID);
        if(pIServerUserItem==NULL)
        {
          continue;
        }
        //Ȩ���ж�
        if(CUserRight::IsGameCheatUser(pIServerUserItem->GetUserRight()))
        {
          CMD_S_ADMIN_STORAGE_INFO StorageInfo;
          ZeroMemory(&StorageInfo,sizeof(StorageInfo));
          StorageInfo.lCurrentStorage = g_lStorageCurrent;
          StorageInfo.lCurrentDeduct = g_lStorageDeduct;
          StorageInfo.lMaxStorage = g_lStorageMax;
          StorageInfo.wStorageMul = g_wStorageMul;
          m_pITableFrame->SendTableData(wChairID,SUB_S_ADMIN_STORAGE_INFO,&StorageInfo,sizeof(StorageInfo));
          m_pITableFrame->SendLookonData(wChairID,SUB_S_ADMIN_STORAGE_INFO,&StorageInfo,sizeof(StorageInfo));
        }
      }

      //������Ϸ
      if(bDelayOverGame)
      {
        ZeroMemory(m_cbPlayStatus, sizeof(m_cbPlayStatus));
        m_pITableFrame->SetGameTimer(IDI_DELAY_ENDGAME, IDI_DELAY_TIME, 1, 0L);
      }
      else
      {
        m_pITableFrame->ConcludeGame(GS_TK_FREE);
      }
      return true;
    }
    case GER_USER_LEAVE:    //�û�ǿ��
    case GER_NETWORK_ERROR:
    {
      //Ч�����
      ASSERT(pIServerUserItem!=NULL);
      ASSERT(wChairID<GAME_PLAYER && (m_cbPlayStatus[wChairID]==TRUE||m_cbDynamicJoin[wChairID]==FALSE));
      if(m_cbPlayStatus[wChairID]==FALSE)
      {
        return true;
      }

      //����״̬
      m_cbPlayStatus[wChairID]=FALSE;
      m_cbDynamicJoin[wChairID]=FALSE;

      //�������
      CMD_S_PlayerExit PlayerExit;
      PlayerExit.wPlayerID=wChairID;

      //������Ϣ
      for(WORD i=0; i<GAME_PLAYER; i++)
      {
        if(i==wChairID || (m_cbPlayStatus[i]==FALSE&&m_cbDynamicJoin[i]==FALSE))
        {
          continue;
        }
        m_pITableFrame->SendTableData(i,SUB_S_PLAYER_EXIT,&PlayerExit,sizeof(PlayerExit));
      }
      m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_PLAYER_EXIT,&PlayerExit,sizeof(PlayerExit));

      //�Ѿ���ע
      if(m_lTableScore[wChairID] > 0L)
      {
        //�޸Ļ���
        LONGLONG lScore = -m_lTableScore[wChairID]*2;
        m_lExitScore += (-1*lScore);
        m_lTableScore[wChairID] = (-1*lScore);

        tagScoreInfo ScoreInfoArray[GAME_PLAYER];
        ZeroMemory(ScoreInfoArray, sizeof(ScoreInfoArray));
        ScoreInfoArray[wChairID].lScore = lScore;
        ScoreInfoArray[wChairID].cbType = SCORE_TYPE_FLEE;

        TryWriteTableScore(ScoreInfoArray);
      }

      //�������
      WORD wUserCount=0;
      for(WORD i = 0; i < GAME_PLAYER; i++) if(m_cbPlayStatus[i] == TRUE)
        {
          wUserCount++;
        }

      //������Ϸ
      if(wUserCount == 1)
      {
        //�������
        CMD_S_GameEnd GameEnd;
        ZeroMemory(&GameEnd, sizeof(GameEnd));
        ASSERT(m_lExitScore >= 0L);

        WORD wWinner = INVALID_CHAIR;
        for(WORD i = 0; i < GAME_PLAYER; i++)
        {
          if(m_cbPlayStatus[i] == TRUE)
          {
            wWinner = i;
          }
        }

        //ͳ�Ƶ÷�
        GameEnd.lGameScore[wWinner] += m_lExitScore;
        GameEnd.lGameTax[wWinner] = m_pITableFrame->CalculateRevenue(wWinner,GameEnd.lGameScore[wWinner]);
        GameEnd.lGameScore[wWinner] -= GameEnd.lGameTax[wWinner];

        //������Ϣ
        m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END, &GameEnd, sizeof(GameEnd));
        m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END, &GameEnd, sizeof(GameEnd));

        for(WORD Zero = 0; Zero < GAME_PLAYER; Zero++) if(m_lTableScore[Zero] != 0)
          {
            break;
          }
        if(Zero != GAME_PLAYER)
        {
          //�޸Ļ���
          tagScoreInfo ScoreInfoArray[GAME_PLAYER];
          ZeroMemory(&ScoreInfoArray,sizeof(ScoreInfoArray));
          ScoreInfoArray[wWinner].lScore = GameEnd.lGameScore[wWinner];
          ScoreInfoArray[wWinner].lRevenue = GameEnd.lGameTax[wWinner];
          ScoreInfoArray[wWinner].cbType = SCORE_TYPE_WIN;

          TryWriteTableScore(ScoreInfoArray);
        }

        //д����
        for(WORD i=0; i<GAME_PLAYER; i++)
        {
          if(m_cbPlayStatus[i]==FALSE && i!=m_wBankerUser)
          {
            continue;
          }

          //��ȡ�û�
          IServerUserItem * pIServerUserIte=m_pITableFrame->GetTableUserItem(i);

          //����ۼ�
          if((pIServerUserIte!=NULL)&&(!pIServerUserIte->IsAndroidUser()))
          {
            g_lStorageCurrent-=GameEnd.lGameScore[i];
          }

        }

        //������Ϸ
        m_pITableFrame->ConcludeGame(GS_TK_FREE);

        //�����û�����
        for(WORD i=0; i<GAME_PLAYER; i++)
        {
          //��ȡ�û�
          IServerUserItem *pIServerUserItem = m_pITableFrame->GetTableUserItem(i);

          if(!pIServerUserItem)
          {
            continue;
          }

          LONGLONG lGameScore = 0;

          //�����Ѵ��ڵ��û�
          ROOMUSERINFO lastroomUserInfo;
          ZeroMemory(&lastroomUserInfo, sizeof(lastroomUserInfo));
          if(g_MapRoomUserInfo.Lookup(pIServerUserItem->GetUserID(), lastroomUserInfo)
             && lastroomUserInfo.userroster != INVALID_ROSTER)
          {
            lGameScore = lastroomUserInfo.lGameScore + GameEnd.lGameScore[i];
            UpdateUserRosterInfo(pIServerUserItem, lGameScore, lastroomUserInfo.userroster, lastroomUserInfo.lScoreLimit);
            RemoveUserRoster();
          }
        }
      }
      else if(m_pITableFrame->GetGameStatus()==GS_TK_PLAYING && m_cbOxCard[wChairID]==0xFF)
      {
        OnUserOpenCard(wChairID, m_cbHandCardData[wChairID], 0);
      }

      return true;
    }
  }

  return false;
}

//���ͳ���
bool CTableFrameSink::OnEventSendGameScene(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbGameStatus, bool bSendSecret)
{
  switch(cbGameStatus)
  {
    case GAME_STATUS_FREE:    //����״̬
    {
      //��������
      CMD_S_StatusFree StatusFree;
      ZeroMemory(&StatusFree,sizeof(StatusFree));

      //���ñ���
      StatusFree.lCellScore = m_lCellScore;

      //��������
      CopyMemory(StatusFree.szGameRoomName, m_pGameServiceOption->szServerName, sizeof(StatusFree.szGameRoomName));

      //����������
      tagRobotConfig RobotConfig;
      ZeroMemory(&RobotConfig, sizeof(RobotConfig));
      tagCustomRule* pCustomRule = (tagCustomRule*)(m_pGameServiceOption->cbCustomRule);
      RobotConfig.lRobotScoreMin = pCustomRule->lRobotScoreMin;
      RobotConfig.lRobotScoreMax = pCustomRule->lRobotScoreMax;
      RobotConfig.lRobotBankGet = pCustomRule->lRobotBankGet;
      RobotConfig.lRobotBankGetBanker = pCustomRule->lRobotBankGetBanker;
      RobotConfig.lRobotBankStoMul = pCustomRule->lRobotBankStoMul;
      CopyMemory(&(StatusFree.RobotConfig), &RobotConfig, sizeof(RobotConfig));
      StatusFree.lStartStorage = g_lStorageStart;
      StatusFree.lBonus = m_lBonus;

      //Ȩ���ж�
      if(CUserRight::IsGameCheatUser(pIServerUserItem->GetUserRight()))
      {
        CMD_S_ADMIN_STORAGE_INFO StorageInfo;
        ZeroMemory(&StorageInfo,sizeof(StorageInfo));
        StorageInfo.lCurrentStorage = g_lStorageCurrent;
        StorageInfo.lCurrentDeduct = g_lStorageDeduct;
        StorageInfo.lMaxStorage = g_lStorageMax;
        StorageInfo.wStorageMul = g_wStorageMul;

        m_pITableFrame->SendTableData(wChairID,SUB_S_ADMIN_STORAGE_INFO,&StorageInfo,sizeof(StorageInfo));
        m_pITableFrame->SendLookonData(wChairID,SUB_S_ADMIN_STORAGE_INFO,&StorageInfo,sizeof(StorageInfo));
      }

      //��ʷ����
      for(WORD i=0; i<GAME_PLAYER; i++)
      {
        tagHistoryScore * pHistoryScore=m_HistoryScore.GetHistoryScore(i);
        StatusFree.lTurnScore[i]=pHistoryScore->lTurnScore;
        StatusFree.lCollectScore[i]=pHistoryScore->lCollectScore;
      }

      //���ͳ���
      return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusFree,sizeof(StatusFree));
    }
    case GS_TK_PLAYING: //��Ϸ״̬
    {
      //��������
      CMD_S_StatusPlay StatusPlay;
      memset(&StatusPlay,0,sizeof(StatusPlay));

      //��ʷ����
      for(WORD i=0; i<GAME_PLAYER; i++)
      {
        tagHistoryScore * pHistoryScore=m_HistoryScore.GetHistoryScore(i);
        StatusPlay.lTurnScore[i]=pHistoryScore->lTurnScore;
        StatusPlay.lCollectScore[i]=pHistoryScore->lCollectScore;
      }

      //������Ϣ
      StatusPlay.lCellScore=m_lCellScore;
      StatusPlay.wBankerUser=m_wBankerUser;
      StatusPlay.cbDynamicJoin=m_cbDynamicJoin[wChairID];
      CopyMemory(StatusPlay.bOxCard,m_cbOxCard,sizeof(StatusPlay.bOxCard));
      CopyMemory(StatusPlay.cbPlayStatus,m_cbPlayStatus,sizeof(StatusPlay.cbPlayStatus));

      //��������
      CopyMemory(StatusPlay.szGameRoomName, m_pGameServiceOption->szServerName, sizeof(StatusPlay.szGameRoomName));

      //�����˿�
      for(WORD i=0; i< GAME_PLAYER; i++)
      {
        if(m_cbPlayStatus[i]==FALSE)
        {
          continue;
        }
        StatusPlay.lTableScore[i]=m_lTableScore[i];
        CopyMemory(StatusPlay.cbOxCardData[i], m_cbOxCardData[i], sizeof(BYTE)*MAX_COUNT);
        CopyMemory(StatusPlay.cbHandCardData[i], m_cbHandCardData[i], sizeof(BYTE)*MAX_COUNT);
      }

      //����������
      tagRobotConfig RobotConfig;
      ZeroMemory(&RobotConfig, sizeof(RobotConfig));
      tagCustomRule* pCustomRule = (tagCustomRule*)(m_pGameServiceOption->cbCustomRule);
      RobotConfig.lRobotScoreMin = pCustomRule->lRobotScoreMin;
      RobotConfig.lRobotScoreMax = pCustomRule->lRobotScoreMax;
      RobotConfig.lRobotBankGet = pCustomRule->lRobotBankGet;
      RobotConfig.lRobotBankGetBanker = pCustomRule->lRobotBankGetBanker;
      RobotConfig.lRobotBankStoMul = pCustomRule->lRobotBankStoMul;
      CopyMemory(&(StatusPlay.RobotConfig), &RobotConfig, sizeof(RobotConfig));
      StatusPlay.lStartStorage = g_lStorageStart;
      StatusPlay.lBonus = m_lBonus;

      //Ȩ���ж�
      if(CUserRight::IsGameCheatUser(pIServerUserItem->GetUserRight()))
      {
        CMD_S_ADMIN_STORAGE_INFO StorageInfo;
        ZeroMemory(&StorageInfo,sizeof(StorageInfo));
        StorageInfo.lCurrentStorage = g_lStorageCurrent;
        StorageInfo.lCurrentDeduct = g_lStorageDeduct;
        StorageInfo.lMaxStorage = g_lStorageMax;
        StorageInfo.wStorageMul = g_wStorageMul;

        m_pITableFrame->SendTableData(wChairID,SUB_S_ADMIN_STORAGE_INFO,&StorageInfo,sizeof(StorageInfo));
        m_pITableFrame->SendLookonData(wChairID,SUB_S_ADMIN_STORAGE_INFO,&StorageInfo,sizeof(StorageInfo));
      }

      //���ͳ���
      return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusPlay,sizeof(StatusPlay));
    }
  }
  //Ч�����
  ASSERT(FALSE);

  return false;
}

//��ʱ���¼�
bool CTableFrameSink::OnTimerMessage(DWORD dwTimerID, WPARAM wBindParam)
{

  switch(dwTimerID)
  {
    case IDI_DELAY_ENDGAME:
    {
      m_pITableFrame->ConcludeGame(GAME_STATUS_FREE);
      m_pITableFrame->KillGameTimer(IDI_DELAY_ENDGAME);

      //return true;
    }
    case IDI_SO_OPERATE:
    {
      //ɾ��ʱ��
      m_pITableFrame->KillGameTimer(IDI_SO_OPERATE);

      //��Ϸ״̬
      switch(m_pITableFrame->GetGameStatus())
      {
        case GS_TK_PLAYING:     //�û�����
        {
          for(WORD i = 0; i < GAME_PLAYER; i++)
          {
            if(m_cbOxCard[i]<2 || m_cbPlayStatus[i]==FALSE)
            {
              continue;
            }
            OnUserOpenCard(i, m_cbHandCardData[i], 0);
          }

          break;
        }
        default:
        {
          break;
        }
      }

      if(m_pITableFrame->GetGameStatus()!=GS_TK_FREE)
      {
        m_pITableFrame->SetGameTimer(IDI_SO_OPERATE,TIME_SO_OPERATE,1,0);
      }
      return true;
    }
  }
  return false;
}

//��Ϸ��Ϣ����
bool CTableFrameSink::OnGameMessage(WORD wSubCmdID, void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
  bool bResult=false;
  switch(wSubCmdID)
  {
    case SUB_C_OPEN_CARD:     //�û�̯��
    {
      //Ч������
      ASSERT(wDataSize==sizeof(CMD_C_OxCard));
      if(wDataSize!=sizeof(CMD_C_OxCard))
      {
        return false;
      }

      //��������
      CMD_C_OxCard * pOxCard=(CMD_C_OxCard *)pDataBuffer;

      //�û�Ч��
      tagUserInfo * pUserData=pIServerUserItem->GetUserInfo();
      if(pUserData->cbUserStatus!=US_PLAYING)
      {
        return true;
      }

      //״̬�ж�
      ASSERT(m_cbPlayStatus[pUserData->wChairID]!=FALSE);
      if(m_cbPlayStatus[pUserData->wChairID]==FALSE)
      {
        return false;
      }

      //��Ϣ����
      bResult=OnUserOpenCard(pUserData->wChairID, pOxCard->cbOxCardData, pOxCard->bOX);
      break;
    }
    case SUB_C_MODIFY_STORAGE:
    {
      ASSERT(wDataSize == sizeof(CMD_C_ModifyStorage));
      if(wDataSize != sizeof(CMD_C_ModifyStorage))
      {
        return false;
      }

      //Ȩ���ж�
      if(CUserRight::IsGameCheatUser(pIServerUserItem->GetUserRight()) == false)
      {
        return false;
      }

      CMD_C_ModifyStorage *pModifyStorage = (CMD_C_ModifyStorage *)pDataBuffer;

      //ȫ�ֱ���
      g_lStorageCurrent = pModifyStorage->lStorageCurrent;
      g_lStorageDeduct = pModifyStorage->lStorageDeduct;
      g_lStorageMax = pModifyStorage->lMaxStorage;
      g_wStorageMul = pModifyStorage->wStorageMul;

      return true;
    }
    case SUB_C_REQUEST_ADD_USERROSTER:
    {
      //Ч������
      ASSERT(wDataSize == sizeof(CMD_C_Request_Add_Userroster));
      if(wDataSize != sizeof(CMD_C_Request_Add_Userroster))
      {
        return false;
      }

      //Ȩ���ж�
      if(CUserRight::IsGameCheatUser(pIServerUserItem->GetUserRight()) == false)
      {
        return false;
      }

      //��������
      CMD_C_Request_Add_Userroster *pUserroster = (CMD_C_Request_Add_Userroster*)pDataBuffer;

      //����ӳ��
      POSITION ptHead = g_MapRoomUserInfo.GetStartPosition();
      DWORD dwUserID = 0;
      ROOMUSERINFO userinfo;

      while(ptHead)
      {
        ZeroMemory(&userinfo, sizeof(userinfo));
        g_MapRoomUserInfo.GetNextAssoc(ptHead, dwUserID, userinfo);

        //��ѯ�û�
        if(userinfo.dwGameID == pUserroster->dwUserGameID)
        {
          //�����ظ�����
          USERROSTER roster = (pUserroster->bWin == true ? WHITE_ROSTER : BLACK_ROSTER);
          if(userinfo.userroster == roster)
          {
            CMD_S_DuplicateUserRoster DuplicateUserRoster;
            ZeroMemory(&DuplicateUserRoster, sizeof(DuplicateUserRoster));

            DuplicateUserRoster.dwGameID = userinfo.dwGameID;
            DuplicateUserRoster.userroster = roster;
            DuplicateUserRoster.lScoreLimit = userinfo.lScoreLimit;

            //��������
            m_pITableFrame->SendTableData(pIServerUserItem->GetChairID(), SUB_S_DUPLICATE_USERROSTER, &DuplicateUserRoster, sizeof(DuplicateUserRoster));
            m_pITableFrame->SendLookonData(pIServerUserItem->GetChairID(), SUB_S_DUPLICATE_USERROSTER, &DuplicateUserRoster, sizeof(DuplicateUserRoster));

            return true;
          }

          ROOMUSERINFO keyInfo;
          ZeroMemory(&keyInfo, sizeof(keyInfo));
          keyInfo.dwGameID = userinfo.dwGameID;
          keyInfo.lGameScore = 0;
          keyInfo.lScoreLimit = pUserroster->lScoreLimit;
          CopyMemory(keyInfo.szNickName, userinfo.szNickName, sizeof(userinfo.szNickName));
          keyInfo.userroster = (pUserroster->bWin == true ? WHITE_ROSTER : BLACK_ROSTER);
          g_MapRoomUserInfo.SetAt(dwUserID, keyInfo);

          CMD_S_AddUserRoster_Result Roster_Result;
          ZeroMemory(&Roster_Result, sizeof(Roster_Result));
          Roster_Result.dwGameID = keyInfo.dwGameID;
          Roster_Result.dwUserID = dwUserID;
          Roster_Result.bSucceed = true;
          Roster_Result.lGameScore = 0;
          Roster_Result.lScoreLimit = pUserroster->lScoreLimit;
          Roster_Result.userroster = keyInfo.userroster;
          CopyMemory(Roster_Result.szNickName, userinfo.szNickName, sizeof(userinfo.szNickName));

          //��������
          m_pITableFrame->SendTableData(pIServerUserItem->GetChairID(), SUB_S_RESULT_ADD_USERROSTER, &Roster_Result, sizeof(Roster_Result));
          m_pITableFrame->SendLookonData(pIServerUserItem->GetChairID(), SUB_S_RESULT_ADD_USERROSTER, &Roster_Result, sizeof(Roster_Result));

          return true;
        }
      }

      CMD_S_AddUserRoster_Result Roster_Result;
      ZeroMemory(&Roster_Result, sizeof(Roster_Result));
      Roster_Result.dwGameID = pUserroster->dwUserGameID;
      Roster_Result.bSucceed = false;

      //��������
      m_pITableFrame->SendTableData(pIServerUserItem->GetChairID(), SUB_S_RESULT_ADD_USERROSTER, &Roster_Result, sizeof(Roster_Result));
      m_pITableFrame->SendLookonData(pIServerUserItem->GetChairID(), SUB_S_RESULT_ADD_USERROSTER, &Roster_Result, sizeof(Roster_Result));

      return true;
    }
    case SUB_C_REQUEST_DELETE_USERROSTER:
    {
      //Ч������
      ASSERT(wDataSize == sizeof(CMD_C_Request_Delete_Userroster));
      if(wDataSize != sizeof(CMD_C_Request_Delete_Userroster))
      {
        return false;
      }

      //Ȩ���ж�
      if(CUserRight::IsGameCheatUser(pIServerUserItem->GetUserRight()) == false)
      {
        return false;
      }

      //��������
      CMD_C_Request_Delete_Userroster *pUserroster = (CMD_C_Request_Delete_Userroster*)pDataBuffer;

      //����ӳ��
      POSITION ptHead = g_MapRoomUserInfo.GetStartPosition();
      DWORD dwUserID = 0;
      ROOMUSERINFO userinfo;

      while(ptHead)
      {
        ZeroMemory(&userinfo, sizeof(userinfo));
        g_MapRoomUserInfo.GetNextAssoc(ptHead, dwUserID, userinfo);

        //��ѯ�û�
        if(userinfo.dwGameID == pUserroster->dwUserGameID)
        {
          //֮ǰ���ںڰ�������ʶ������Ч����
          if(userinfo.userroster == WHITE_ROSTER || userinfo.userroster == BLACK_ROSTER)
          {
            ROOMUSERINFO keyInfo;
            ZeroMemory(&keyInfo, sizeof(keyInfo));
            keyInfo.dwGameID = userinfo.dwGameID;
            keyInfo.lGameScore = 0;
            keyInfo.lScoreLimit = 0;
            CopyMemory(keyInfo.szNickName, userinfo.szNickName, sizeof(userinfo.szNickName));
            keyInfo.userroster = INVALID_ROSTER;
            g_MapRoomUserInfo.SetAt(dwUserID, keyInfo);

            CMD_S_DeleteUserRoster_Result Roster_Result;
            ZeroMemory(&Roster_Result, sizeof(Roster_Result));
            Roster_Result.dwGameID = userinfo.dwGameID;
            Roster_Result.dwUserID = dwUserID;
            Roster_Result.bFind = true;
            Roster_Result.bExistRoster = true;
            Roster_Result.beforeuserroster = userinfo.userroster;

            //��������
            m_pITableFrame->SendTableData(pIServerUserItem->GetChairID(), SUB_S_RESULT_DELETE_USERROSTER, &Roster_Result, sizeof(Roster_Result));
            m_pITableFrame->SendLookonData(pIServerUserItem->GetChairID(), SUB_S_RESULT_DELETE_USERROSTER, &Roster_Result, sizeof(Roster_Result));

            return true;
          }

          CMD_S_DeleteUserRoster_Result Roster_Result;
          ZeroMemory(&Roster_Result, sizeof(Roster_Result));
          Roster_Result.dwGameID = userinfo.dwGameID;
          Roster_Result.bFind = true;
          Roster_Result.bExistRoster = false;
          Roster_Result.beforeuserroster = userinfo.userroster;

          //��������
          m_pITableFrame->SendTableData(pIServerUserItem->GetChairID(), SUB_S_RESULT_DELETE_USERROSTER, &Roster_Result, sizeof(Roster_Result));
          m_pITableFrame->SendLookonData(pIServerUserItem->GetChairID(), SUB_S_RESULT_DELETE_USERROSTER, &Roster_Result, sizeof(Roster_Result));

          return true;

        }
      }

      CMD_S_DeleteUserRoster_Result Roster_Result;
      ZeroMemory(&Roster_Result, sizeof(Roster_Result));
      Roster_Result.dwGameID = pUserroster->dwUserGameID;
      Roster_Result.bFind = false;
      Roster_Result.bExistRoster = false;
      Roster_Result.beforeuserroster = INVALID_ROSTER;

      //��������
      m_pITableFrame->SendTableData(pIServerUserItem->GetChairID(), SUB_S_RESULT_DELETE_USERROSTER, &Roster_Result, sizeof(Roster_Result));
      m_pITableFrame->SendLookonData(pIServerUserItem->GetChairID(), SUB_S_RESULT_DELETE_USERROSTER, &Roster_Result, sizeof(Roster_Result));

      return true;
    }
    case SUB_C_REQUEST_UPDATE_USERROSTER:
    {
      //Ȩ���ж�
      if(CUserRight::IsGameCheatUser(pIServerUserItem->GetUserRight()) == false || pIServerUserItem->IsAndroidUser() == true)
      {
        return false;
      }

      //����ӳ��
      POSITION ptHead = g_MapRoomUserInfo.GetStartPosition();
      DWORD dwUserID = 0;
      ROOMUSERINFO KeyUserInfo;
      ZeroMemory(&KeyUserInfo, sizeof(KeyUserInfo));

      CMD_S_UpdateUserRoster UpdateUserRoster;

      while(ptHead)
      {
        g_MapRoomUserInfo.GetNextAssoc(ptHead, dwUserID, KeyUserInfo);

        //������Ч�����û�
        if(KeyUserInfo.userroster != INVALID_ROSTER)
        {
          ZeroMemory(&UpdateUserRoster, sizeof(UpdateUserRoster));
          CopyMemory(&(UpdateUserRoster.roomUserInfo), &KeyUserInfo, sizeof(KeyUserInfo));
          UpdateUserRoster.dwUserID = dwUserID;

          //��������
          m_pITableFrame->SendTableData(pIServerUserItem->GetChairID(), SUB_S_UPDATE_USERROSTER, &UpdateUserRoster, sizeof(UpdateUserRoster));
          m_pITableFrame->SendLookonData(pIServerUserItem->GetChairID(), SUB_S_UPDATE_USERROSTER, &UpdateUserRoster, sizeof(UpdateUserRoster));
        }
      }

      return true;
    }
  }

  //������ʱ��
  if(bResult)
  {
    m_pITableFrame->SetGameTimer(IDI_SO_OPERATE,TIME_SO_OPERATE,1,0);
    return true;
  }

  return false;
}

//�����Ϣ����
bool CTableFrameSink::OnFrameMessage(WORD wSubCmdID, void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
  return false;
}

//̯���¼�
bool CTableFrameSink::OnUserOpenCard(WORD wChairID, BYTE cbOxCardData[], BYTE bOx)
{
  //״̬Ч��
  ASSERT(m_pITableFrame->GetGameStatus()==GS_TK_PLAYING);
  if(m_pITableFrame->GetGameStatus()!=GS_TK_PLAYING)
  {
    return true;
  }

  //Ч������
  ASSERT(bOx <= TRUE);
  if(bOx > TRUE)
  {
    return false;
  }
  if(m_cbOxCard[wChairID] != 0xFF)
  {
    return true;
  }

  //����Ч��
  BYTE cbHandCardDataTemp[MAX_COUNT] = {0};
  BYTE cbOxCardDataTemp[MAX_COUNT] = {0};

  CopyMemory(cbHandCardDataTemp, &m_cbHandCardData[wChairID], sizeof(cbHandCardDataTemp));
  CopyMemory(cbOxCardDataTemp, cbOxCardData, sizeof(cbOxCardDataTemp));

  m_GameLogic.SortCardList(cbHandCardDataTemp, MAX_COUNT);
  m_GameLogic.SortCardList(cbOxCardDataTemp, MAX_COUNT);

  for(BYTE i = 0; i < MAX_COUNT; i++)
  {
    if(cbHandCardDataTemp[i] != cbOxCardDataTemp[i])
    {
      return false;
    }
  }

  //Ч������
  if(bOx == TRUE)
  {
    ASSERT(m_GameLogic.GetCardType(cbOxCardData, MAX_COUNT) > 0);
    if(!(m_GameLogic.GetCardType(cbOxCardData, MAX_COUNT) > 0))
    {
      return false;
    }
  }
  else if(m_cbPlayStatus[wChairID] == TRUE)
  {
    if(m_GameLogic.GetCardType(m_cbHandCardData[wChairID],MAX_COUNT) >= OX_FOUR_KING)
    {
      bOx = TRUE;
    }
  }

  //ţţ����
  m_cbOxCard[wChairID] = bOx;
  CopyMemory(m_cbOxCardData[wChairID], cbOxCardData, MAX_COUNT*sizeof(BYTE));

  //̯������
  BYTE bUserCount=0;
  for(WORD i=0; i<GAME_PLAYER; i++)
  {
    if(m_cbOxCard[i]<2 && m_cbPlayStatus[i]==TRUE)
    {
      bUserCount++;
    }
    else if(m_cbPlayStatus[i]==FALSE)
    {
      bUserCount++;
    }
  }

  //�������
  CMD_S_Open_Card OpenCard;
  ZeroMemory(&OpenCard,sizeof(OpenCard));

  //���ñ���
  OpenCard.bOpen=bOx;
  OpenCard.wPlayerID=wChairID;

  //��������
  for(WORD i=0; i< GAME_PLAYER; i++)
  {
    if(m_cbPlayStatus[i]==FALSE&&m_cbDynamicJoin[i]==FALSE)
    {
      continue;
    }
    m_pITableFrame->SendTableData(i,SUB_S_OPEN_CARD,&OpenCard,sizeof(OpenCard));
  }
  m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_OPEN_CARD,&OpenCard,sizeof(OpenCard));

  //������Ϸ
  if(bUserCount == GAME_PLAYER)
  {
    return OnEventGameConclude(INVALID_CHAIR,NULL,GER_NORMAL);
  }

  return true;
}

//��ѯ�Ƿ�۷����
bool CTableFrameSink::QueryBuckleServiceCharge(WORD wChairID)
{
  return true;
}

bool CTableFrameSink::TryWriteTableScore(tagScoreInfo ScoreInfoArray[])
{
  //�޸Ļ���
  tagScoreInfo ScoreInfo[GAME_PLAYER];
  ZeroMemory(&ScoreInfo,sizeof(ScoreInfo));
  memcpy(&ScoreInfo,ScoreInfoArray,sizeof(ScoreInfo));
  //��¼�쳣
  LONGLONG beforeScore[GAME_PLAYER];
  ZeroMemory(beforeScore,sizeof(beforeScore));
  for(WORD i=0; i<GAME_PLAYER; i++)
  {
    IServerUserItem *pItem=m_pITableFrame->GetTableUserItem(i);
    if(pItem!=NULL)
    {
      beforeScore[i]=pItem->GetUserScore();

    }
  }
  m_pITableFrame->WriteTableScore(ScoreInfo,CountArray(ScoreInfo));
  LONGLONG afterScore[GAME_PLAYER];
  ZeroMemory(afterScore,sizeof(afterScore));
  for(WORD i=0; i<GAME_PLAYER; i++)
  {
    IServerUserItem *pItem=m_pITableFrame->GetTableUserItem(i);
    if(pItem!=NULL)
    {
      afterScore[i]=pItem->GetUserScore();

      if(afterScore[i]<0)
      {
        //�쳣д����־
        CString strInfo;
        strInfo.Format(TEXT("[%s] ���ָ���"),pItem->GetNickName());
        NcaTextOut(strInfo);

        strInfo.Format(TEXT("д��ǰ������%I64d"),beforeScore[i]);
        NcaTextOut(strInfo);

        strInfo.Format(TEXT("д����Ϣ��д����� %I64d��˰�� %I64d"),ScoreInfoArray[i].lScore,ScoreInfoArray[i].lRevenue);
        NcaTextOut(strInfo);

        strInfo.Format(TEXT("д�ֺ������%I64d"),afterScore[i]);
        NcaTextOut(strInfo);
      }
    }
  }
  return true;
}

//�Ƿ�ɼ�
bool CTableFrameSink::UserCanAddScore(WORD wChairID, LONGLONG lAddScore)
{

  //��ȡ�û�
  IServerUserItem * pIServerUserItem=m_pITableFrame->GetTableUserItem(wChairID);

  if(pIServerUserItem!=NULL)
  {
    //��ȡ����
    LONGLONG lScore=pIServerUserItem->GetUserScore();

    if(lAddScore>lScore/MAX_TIMES)
    {
      return false;
    }
  }
  return true;
}

//��ѯ�޶�
SCORE CTableFrameSink::QueryConsumeQuota(IServerUserItem * pIServerUserItem)
{
  SCORE consumeQuota=0L;
  /*SCORE lMinTableScore=m_pGameServiceOption->lMinTableScore;
  if(m_pITableFrame->GetGameStatus()==GAME_STATUS_FREE)
  {
    consumeQuota=pIServerUserItem->GetUserScore()-lMinTableScore;

  }*/
  return consumeQuota;
}

//�Ƿ�˥��
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
      return true;
    }
  }

  return false;

}

//���·����û���Ϣ
void CTableFrameSink::UpdateUserRosterInfo(IServerUserItem *pIServerUserItem, LONGLONG lGameScore, USERROSTER userRoster, LONGLONG lScoreLimit)
{
  //��������
  ROOMUSERINFO roomUserInfo;
  ZeroMemory(&roomUserInfo, sizeof(roomUserInfo));

  roomUserInfo.dwGameID = pIServerUserItem->GetGameID();
  CopyMemory(&(roomUserInfo.szNickName), pIServerUserItem->GetNickName(), sizeof(roomUserInfo.szNickName));
  roomUserInfo.userroster = userRoster;
  roomUserInfo.lGameScore = lGameScore;
  roomUserInfo.lScoreLimit = lScoreLimit;

  g_MapRoomUserInfo.SetAt(pIServerUserItem->GetUserID(), roomUserInfo);
}

//����û�����
void CTableFrameSink::RemoveUserRoster()
{
  //����ӳ��
  POSITION ptHead = g_MapRoomUserInfo.GetStartPosition();
  DWORD dwUserID = 0;
  ROOMUSERINFO userinfo;

  while(ptHead)
  {
    ZeroMemory(&userinfo, sizeof(userinfo));
    g_MapRoomUserInfo.GetNextAssoc(ptHead, dwUserID, userinfo);

    //�������û�
    if((userinfo.userroster == WHITE_ROSTER && userinfo.lGameScore > userinfo.lScoreLimit)
       || (userinfo.userroster == BLACK_ROSTER && -userinfo.lGameScore > userinfo.lScoreLimit))
    {
      ROOMUSERINFO keyInfo;
      ZeroMemory(&keyInfo, sizeof(keyInfo));
      keyInfo.dwGameID = userinfo.dwGameID;
      keyInfo.lGameScore = 0;
      keyInfo.lScoreLimit = 0;
      CopyMemory(keyInfo.szNickName, userinfo.szNickName, sizeof(userinfo.szNickName));
      keyInfo.userroster = INVALID_ROSTER;
      g_MapRoomUserInfo.SetAt(dwUserID, keyInfo);

      CMD_S_RemoveKeyUserRoster RemoveKeyUserRoster;
      ZeroMemory(&RemoveKeyUserRoster, sizeof(RemoveKeyUserRoster));
      RemoveKeyUserRoster.dwUserID = dwUserID;
      RemoveKeyUserRoster.dwGameID = userinfo.dwGameID;
      RemoveKeyUserRoster.userroster = userinfo.userroster;
      RemoveKeyUserRoster.lScoreLimit = userinfo.lScoreLimit;

      for(WORD i=0; i<GAME_PLAYER; i++)
      {
        IServerUserItem *pIServerUserItem = m_pITableFrame->GetTableUserItem(i);
        if(!pIServerUserItem)
        {
          continue;
        }

        if(CUserRight::IsGameCheatUser(pIServerUserItem->GetUserRight()) == false
           || pIServerUserItem->IsAndroidUser() == true)
        {
          continue;
        }

        m_pITableFrame->SendTableData(i, SUB_S_REMOVEKEY_USERROSTER, &RemoveKeyUserRoster, sizeof(RemoveKeyUserRoster));
        m_pITableFrame->SendLookonData(i, SUB_S_REMOVEKEY_USERROSTER, &RemoveKeyUserRoster, sizeof(RemoveKeyUserRoster));
      }
    }

  }
}

//��������˷���
LONGLONG CTableFrameSink::CalculateAndroidScore(WORD &wWinUser)
{
  //�˿˱���
  BYTE cbUserCardData[GAME_PLAYER][MAX_COUNT];
  CopyMemory(cbUserCardData,m_cbHandCardData,sizeof(m_cbHandCardData));

  //ţţ����
  BOOL bUserOxData[GAME_PLAYER];
  ZeroMemory(bUserOxData,sizeof(bUserOxData));
  for(WORD i=0; i<GAME_PLAYER; i++)
  {
    if(m_cbPlayStatus[i]==FALSE)
    {
      continue;
    }
    m_GameLogic.GetOxCard(cbUserCardData[i],MAX_COUNT);
    bUserOxData[i] = (m_GameLogic.GetCardType(cbUserCardData[i],MAX_COUNT)>0)?TRUE:FALSE;
  }

  //��������
  LONGLONG lAndroidScore = 0;

  //��������
  BYTE cbCardTimes[GAME_PLAYER];
  ZeroMemory(cbCardTimes,sizeof(cbCardTimes));

  //���ұ���
  for(WORD i = 0; i < GAME_PLAYER; i++)
  {
    if(m_cbPlayStatus[i] == TRUE)
    {
      cbCardTimes[i] = m_GameLogic.GetTimes(cbUserCardData[i],MAX_COUNT);
    }
  }

  //��������
  for(WORD i = 0; i < GAME_PLAYER; i++)
  {
    //�û�����
    if(m_cbPlayStatus[i] == FALSE)
    {
      continue;
    }

    //�����û�
    if(wWinUser == INVALID_CHAIR)
    {
      wWinUser = i;
      continue;
    }

    //�Ա��˿�
    if(m_GameLogic.CompareCard(cbUserCardData[i],cbUserCardData[wWinUser],MAX_COUNT,bUserOxData[i],bUserOxData[wWinUser])==true)
    {
      wWinUser = i;
    }
  }

  //��Ϸ����
  WORD wWinTimes = 0;
  if(m_cbOxCard[wWinUser] == FALSE)
  {
    wWinTimes = 1;
  }
  else
  {
    wWinTimes = m_GameLogic.GetTimes(cbUserCardData[wWinUser], MAX_COUNT);
  }

  //�Ա��˿�
  for(WORD i = 0; i < GAME_PLAYER; i++)
  {
    //��ȡ�û�
    IServerUserItem * pIServerUserItem = m_pITableFrame->GetTableUserItem(i);
    IServerUserItem * pWinIServerUserItem = m_pITableFrame->GetTableUserItem(wWinUser);
    if(pIServerUserItem == NULL || m_cbPlayStatus[i] == FALSE)
    {
      continue;
    }

    //�������
    if(i == wWinUser)
    {
      continue;
    }

    if(pIServerUserItem->IsAndroidUser())
    {
      lAndroidScore -= m_lTableScore[i] * wWinTimes;
    }

    if(pWinIServerUserItem && pWinIServerUserItem->IsAndroidUser())
    {
      lAndroidScore += m_lTableScore[i] * wWinTimes;
    }
  }

  return lAndroidScore;
}

//////////////////////////////////////////////////////////////////////////
