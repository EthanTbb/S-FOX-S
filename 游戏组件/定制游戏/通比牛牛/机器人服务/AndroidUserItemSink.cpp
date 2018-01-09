#include "Stdafx.h"
#include "AndroidUserItemSink.h"

//////////////////////////////////////////////////////////////////////////

//����ʱ��
#define TIME_LESS           2                 //����ʱ��

//��Ϸʱ��
#define TIME_USER_START_GAME      2                 //��ʼʱ��
#define TIME_USER_OPEN_CARD       4                 //̯��ʱ��

//��Ϸʱ��
#define IDI_START_GAME          (100)               //��ʼ��ʱ��
#define IDI_OPEN_CARD         (101)               //���ƶ�ʱ��
#define IDI_DELAY_TIME          (102)               //��ʱ��ʱ��

//////////////////////////////////////////////////////////////////////////

//���캯��
CAndroidUserItemSink::CAndroidUserItemSink()
{
  m_nRobotBankStorageMul=0;
  m_lRobotBankGetScore=0;
  m_lRobotBankGetScoreBanker=0;
  ZeroMemory(m_lRobotScoreRange,sizeof(m_lRobotScoreRange));
  ZeroMemory(m_HandCardData,sizeof(m_HandCardData));

  //�ӿڱ���
  m_pIAndroidUserItem=NULL;
  srand((unsigned)time(NULL));

  return;
}

//��������
CAndroidUserItemSink::~CAndroidUserItemSink()
{
}

//�ӿڲ�ѯ
void * CAndroidUserItemSink::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
  QUERYINTERFACE(IAndroidUserItemSink,Guid,dwQueryVer);
  QUERYINTERFACE_IUNKNOWNEX(IAndroidUserItemSink,Guid,dwQueryVer);

  return NULL;
}

//��ʼ�ӿ�
bool CAndroidUserItemSink::Initialization(IUnknownEx * pIUnknownEx)
{
  //��ѯ�ӿ�
  m_pIAndroidUserItem=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,IAndroidUserItem);
  if(m_pIAndroidUserItem==NULL)
  {
    return false;
  }

  return true;
}

//���ýӿ�
bool CAndroidUserItemSink::RepositionSink()
{
  ZeroMemory(m_HandCardData,sizeof(m_HandCardData));

  return true;
}

//ʱ����Ϣ
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
      case IDI_START_GAME:    //��ʼ��ʱ��
      {
        //����׼��
        m_pIAndroidUserItem->SendUserReady(NULL,0);

        return true;
      }
      case IDI_OPEN_CARD:     //���ƶ�ʱ
      {
        //������Ϣ
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
    cs.Format(TEXT("�쳣ID=%d"),nTimerID);
    CTraceService::TraceString(cs,TraceLevel_Debug);
  }

  return false;
}

//��Ϸ��Ϣ
bool CAndroidUserItemSink::OnEventGameMessage(WORD wSubCmdID, void * pData, WORD wDataSize)
{
  switch(wSubCmdID)
  {
    case SUB_S_GAME_START:  //��Ϸ��ʼ
    {
      //��Ϣ����
      return OnSubGameStart(pData,wDataSize);
    }
    case SUB_S_PLAYER_EXIT: //�û�ǿ��
    {
      //��Ϣ����
      return OnSubPlayerExit(pData,wDataSize);
    }
    case SUB_S_SEND_CARD: //������Ϣ
    {
      //��Ϣ����
      return OnSubSendCard(pData,wDataSize);
    }
    case SUB_S_GAME_END:  //��Ϸ����
    {
      //��Ϣ����
      return OnSubGameEnd(pData,wDataSize);
    }
    case SUB_S_OPEN_CARD: //�û�̯��
    {
      //��Ϣ����
      return OnSubOpenCard(pData,wDataSize);
    }
    case SUB_S_ADMIN_STORAGE_INFO:
    {
      return true;
    }
  }

  //�������
  ASSERT(FALSE);

  return true;
}

//��Ϸ��Ϣ
bool CAndroidUserItemSink::OnEventFrameMessage(WORD wSubCmdID, void * pData, WORD wDataSize)
{
  return true;
}

//������Ϣ
bool CAndroidUserItemSink::OnEventSceneMessage(BYTE cbGameStatus, bool bLookonOther, void * pData, WORD wDataSize)
{
  switch(cbGameStatus)
  {
    case GAME_STATUS_FREE:    //����״̬
    {
      //Ч������
      if(wDataSize!=sizeof(CMD_S_StatusFree))
      {
        return false;
      }

      //��Ϣ����
      CMD_S_StatusFree * pStatusFree=(CMD_S_StatusFree *)pData;

      memcpy(m_szRoomName, pStatusFree->szGameRoomName, sizeof(m_szRoomName));

      ReadConfigInformation(&(pStatusFree->RobotConfig));
      BankOperate(2);

      //��ʼʱ��
      UINT nElapse = TIME_LESS + (rand() % TIME_USER_START_GAME);
      m_pIAndroidUserItem->SetGameTimer(IDI_START_GAME,nElapse);

      return true;
    }
    case GS_TK_PLAYING: //��Ϸ״̬
    {
      //Ч������
      if(wDataSize!=sizeof(CMD_S_StatusPlay))
      {
        return false;
      }
      CMD_S_StatusPlay * pStatusPlay=(CMD_S_StatusPlay *)pData;

      memcpy(m_szRoomName, pStatusPlay->szGameRoomName, sizeof(m_szRoomName));

      ReadConfigInformation(&(pStatusPlay->RobotConfig));
      BankOperate(2);

      //���ñ���
      WORD wMeChiarID=m_pIAndroidUserItem->GetChairID();
      CopyMemory(m_HandCardData,pStatusPlay->cbHandCardData[wMeChiarID],MAX_COUNT);

      //�ؼ�����
      if(pStatusPlay->bOxCard[wMeChiarID]==0xFF)
      {
        //����ʱ��
        UINT nElapse = TIME_LESS + (rand() % TIME_USER_OPEN_CARD);
        m_pIAndroidUserItem->SetGameTimer(IDI_OPEN_CARD,nElapse);
      }

      return true;
    }
  }

  ASSERT(FALSE);

  return false;
}

//�û�����
void CAndroidUserItemSink::OnEventUserEnter(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
  return;
}

//�û��뿪
void CAndroidUserItemSink::OnEventUserLeave(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
  return;
}

//�û�����
void CAndroidUserItemSink::OnEventUserScore(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
  return;
}

//�û�״̬
void CAndroidUserItemSink::OnEventUserStatus(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
  return;
}

//�û���λ
void CAndroidUserItemSink::OnEventUserSegment(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
  return;
}

//��Ϸ��ʼ
bool CAndroidUserItemSink::OnSubGameStart(const void * pBuffer, WORD wDataSize)
{
  //Ч������
  if(wDataSize!=sizeof(CMD_S_GameStart))
  {
    return false;
  }
  CMD_S_GameStart * pGameStart=(CMD_S_GameStart *)pBuffer;

  //��������
  WORD wMeChiarID=m_pIAndroidUserItem->GetChairID();
  CopyMemory(m_HandCardData,pGameStart->cbCardData[wMeChiarID],sizeof(m_HandCardData));

  //��עʱ��
  UINT nElapse = TIME_LESS + (rand() % TIME_USER_OPEN_CARD);
  m_pIAndroidUserItem->SetGameTimer(IDI_OPEN_CARD,nElapse);

  return true;
}

//������Ϣ
bool CAndroidUserItemSink::OnSubSendCard(const void * pBuffer, WORD wDataSize)
{
  //Ч������
  if(wDataSize!=sizeof(CMD_S_SendCard))
  {
    return false;
  }
  CMD_S_SendCard * pSendCard=(CMD_S_SendCard *)pBuffer;

  //��������
  WORD wMeChiarID=m_pIAndroidUserItem->GetChairID();
  CopyMemory(m_HandCardData,pSendCard->cbCardData[wMeChiarID],sizeof(m_HandCardData));

  //����ʱ��
  UINT nElapse = TIME_LESS + (rand() % TIME_USER_OPEN_CARD);
  m_pIAndroidUserItem->SetGameTimer(IDI_OPEN_CARD,nElapse);

  return true;
}

//�û�̯��
bool CAndroidUserItemSink::OnSubOpenCard(const void * pBuffer, WORD wDataSize)
{
  //Ч������
  if(wDataSize!=sizeof(CMD_S_Open_Card))
  {
    return false;
  }
  CMD_S_Open_Card * pOpenCard=(CMD_S_Open_Card *)pBuffer;

  return true;
}

//�û�ǿ��
bool CAndroidUserItemSink::OnSubPlayerExit(const void * pBuffer, WORD wDataSize)
{
  //Ч������
  if(wDataSize!=sizeof(CMD_S_PlayerExit))
  {
    return false;
  }
  CMD_S_PlayerExit * pPlayerExit=(CMD_S_PlayerExit *)pBuffer;

  return true;
}

//��Ϸ����
bool CAndroidUserItemSink::OnSubGameEnd(const void * pBuffer, WORD wDataSize)
{
  //Ч�����
  if(wDataSize!=sizeof(CMD_S_GameEnd))
  {
    return false;
  }
  CMD_S_GameEnd * pGameEnd=(CMD_S_GameEnd *)pBuffer;

  //ɾ����ʱ��
  m_pIAndroidUserItem->KillGameTimer(IDI_OPEN_CARD);

  //�������
  ZeroMemory(m_HandCardData,sizeof(m_HandCardData));

  //��ʼʱ��
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

//���в���
void CAndroidUserItemSink::BankOperate(BYTE cbType)
{
  IServerUserItem *pUserItem = m_pIAndroidUserItem->GetMeUserItem();
  if(pUserItem->GetUserStatus() >= US_SIT)
  {
    if(cbType == 1)
    {
      CString strInfo;
      strInfo.Format(TEXT("������״̬���ԣ���ִ�д�ȡ��"));
      NcaTextOut(strInfo);
      return;
    }
  }

  //��������
  LONGLONG lRobotScore = pUserItem->GetUserScore();

  //CString strInfo;
  //strInfo.Format(TEXT("[%s] ����(%I64d)"),pUserItem->GetNickName(),lRobotScore);

  //if (lRobotScore > m_lRobotScoreRange[1])
  //{
  //  CString strInfo1;
  //  strInfo1.Format(TEXT("����������(%I64d)"),m_lRobotScoreRange[1]);
  //  strInfo+=strInfo1;
  //  NcaTextOut(strInfo);
  //}
  //else if (lRobotScore < m_lRobotScoreRange[0])
  //{
  //  CString strInfo1;
  //  strInfo1.Format(TEXT("����ȡ������(%I64d)"),m_lRobotScoreRange[0]);
  //  strInfo+=strInfo1;
  //  NcaTextOut(strInfo);
  //}

  //�жϴ�ȡ
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
    strInfo.Format(TEXT("[%s] ִ�д����ǰ���(%I64d)��������(%I64d)"),pUserItem->GetNickName(),lRobotScore,lRobotNewScore);
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
    strInfo.Format(TEXT("[%s] ִ��ȡ�ȡ��ǰ���(%I64d)��ȡ�����(%I64d)"),pUserItem->GetNickName(),lRobotScore,lRobotNewScore);
    NcaTextOut(strInfo);
  }

  return;
}


//��ȡ����
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


//�����������
DECLARE_CREATE_MODULE(AndroidUserItemSink);

//////////////////////////////////////////////////////////////////////////
