#include "StdAfx.h"
#include "TableFrameSink.h"
#include "DlgCustomRule.h"
#include <locale>
//////////////////////////////////////////////////////////////////////////

//��������
#define SEND_COUNT          300                 //���ʹ���

//��������
#define INDEX_PLAYER        0                 //�м�����
#define INDEX_BANKER        1                 //ׯ������

//��עʱ��
#define IDI_FREE          1                 //����ʱ��
#define TIME_FREE         10                  //����ʱ��

//��עʱ��
#define IDI_PLACE_JETTON      2                 //��עʱ��
#define TIME_PLACE_JETTON     10                  //��עʱ��

//����ʱ��
#define IDI_GAME_END        3                 //����ʱ��
#define TIME_GAME_END       15                  //����ʱ��
#define TIME_GAME_ADD               10                                  //����ʱ��
//////////////////////////////////////////////////////////////////////////

//��̬����
const WORD      CTableFrameSink::m_wPlayerCount=GAME_PLAYER;        //��Ϸ����

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

//���캯��
CTableFrameSink::CTableFrameSink()
{
  //����ע��
  ZeroMemory(m_lAllJettonScore,sizeof(m_lAllJettonScore));

  //������ע
  ZeroMemory(m_lUserJettonScore,sizeof(m_lUserJettonScore));

  //�Ͼ���Ϣ
  ZeroMemory(m_lLastAllJettonPlace,sizeof(m_lLastAllJettonPlace));
  ZeroMemory(m_lLastAllJettonArea,sizeof(m_lLastAllJettonArea));

  //��ʼ����
  ZeroMemory(m_lUserStartScore,sizeof(m_lUserStartScore));
  m_pServerControl=NULL;
  m_cbControlArea = 0xff;
  m_cbControlTimes = 0;
  m_lBonus = 0l;

  //��ҳɼ�
  ZeroMemory(m_lUserWinScore,sizeof(m_lUserWinScore));
  ZeroMemory(m_lUserReturnScore,sizeof(m_lUserReturnScore));
  ZeroMemory(m_lUserRevenue,sizeof(m_lUserRevenue));

  //�˿���Ϣ
  ZeroMemory(m_cbTableCardArray,sizeof(m_cbTableCardArray));

  //״̬����
  m_dwJettonTime=0L;
  m_bControl=false;
  m_lControlStorage=0L;
  CopyMemory(m_szControlName,TEXT("����"),sizeof(m_szControlName));
  m_cbShaYuAddMulti = 0;
  //ׯ����Ϣ
  m_ApplyUserArray.RemoveAll();
  m_wCurrentBanker=INVALID_CHAIR;
  m_wBankerTime=0;
  m_lBankerWinScore=0L;
  m_lBankerCurGameScore=0L;
  m_bEnableSysBanker=true;
  m_nRobotListMaxCount =0;
  //��¼����
  ZeroMemory(m_GameRecordArrary,sizeof(m_GameRecordArrary));
  m_nRecordFirst=0;
  m_nRecordLast=0;

  //���Ʊ���
  m_StorageControl =0l;
  m_lStorageCurrent = 0l;
  m_StorageDeduct = 0l;
  m_lCompensateRadio = 0l;

  m_lStorageMax1 = 0;
  m_lStorageMul1 = 0;
  m_lStorageMax2 = 0;
  m_lStorageMul2 = 0;
  //�����˿���
  m_lRobotAreaLimit = 0l;
  m_lRobotBetCount = 0l;

  //ׯ������
  m_lBankerMAX = 0l;
  m_lBankerAdd = 0l;
  m_lBankerScoreMAX = 0l;
  m_lBankerScoreAdd = 0l;
  m_lPlayerBankerMAX = 0l;
  m_bExchangeBanker = true;

  //ʱ�����
  m_cbFreeTime = TIME_FREE;
  m_cbBetTime = TIME_PLACE_JETTON;
  m_cbEndTime = TIME_GAME_END;

  m_cbAddTime = TIME_GAME_ADD;
  //�����˿���
  m_nChipRobotCount = 0;
  ZeroMemory(m_lRobotAreaScore, sizeof(m_lRobotAreaScore));

  ZeroMemory(m_nAnimalPercent, sizeof(m_nAnimalPercent));
  ZeroMemory(m_nAnimalTimes, sizeof(m_nAnimalTimes));

  //�������
  srand(time(NULL));

  return;
}

//��������
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

//�ӿڲ�ѯ
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

//��ʼ��
bool  CTableFrameSink::Initialization(IUnknownEx * pIUnknownEx)
{
  //��ѯ�ӿ�
  ASSERT(pIUnknownEx!=NULL);
  m_pITableFrame=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,ITableFrame);
  if(m_pITableFrame==NULL)
  {
    return false;
  }

  //��ѯ����
  m_pGameServiceOption=m_pITableFrame->GetGameServiceOption();
  m_pGameServiceAttrib=m_pITableFrame->GetGameServiceAttrib();

  //��ʼģʽ
  m_pITableFrame->SetStartMode(START_MODE_ALL_READY);


  //�����ļ���
  TCHAR szPath[MAX_PATH]=TEXT("");
  GetCurrentDirectory(sizeof(szPath),szPath);
  myprintf(m_szConfigFileName,sizeof(m_szConfigFileName),TEXT("%s\\AnimalBattleConfig.ini"),szPath);
  memcpy(m_szGameRoomName, m_pGameServiceOption->szServerName, sizeof(m_szGameRoomName));

  ReadConfigInformation();

  //�������
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

//��λ����
VOID  CTableFrameSink::RepositionSink()
{
  //����ע��
  ZeroMemory(m_lAllJettonScore,sizeof(m_lAllJettonScore));

  //������ע
  ZeroMemory(m_lUserJettonScore,sizeof(m_lUserJettonScore));

  //�������һע
  ZeroMemory(m_lLastAllJettonArea,sizeof(m_lLastAllJettonArea));

  //��ҳɼ�
  ZeroMemory(m_lUserWinScore,sizeof(m_lUserWinScore));
  ZeroMemory(m_lUserReturnScore,sizeof(m_lUserReturnScore));
  ZeroMemory(m_lUserRevenue,sizeof(m_lUserRevenue));

  //�����˿���
  m_nChipRobotCount = 0;
  ZeroMemory(m_lRobotAreaScore, sizeof(m_lRobotAreaScore));

  ZeroMemory(m_cbTableCardArray,sizeof(m_cbTableCardArray));

  m_cbShaYuAddMulti=0;
  m_bControl=false;
  m_lControlStorage=0L;

  return;
}

//��Ϸ״̬
bool  CTableFrameSink::IsUserPlaying(WORD wChairID)
{
  return true;
}

//��ѯ�޶�
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

//��ѯ�����
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

//��Ϸ��ʼ
bool  CTableFrameSink::OnEventGameStart()
{
  //��������
  CMD_S_GameStart GameStart;
  ZeroMemory(&GameStart,sizeof(GameStart));

  //��ȡׯ��
  IServerUserItem *pIBankerServerUserItem=NULL;
  if(INVALID_CHAIR!=m_wCurrentBanker)
  {
    pIBankerServerUserItem=m_pITableFrame->GetTableUserItem(m_wCurrentBanker);
  }

  //���ñ���
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

  //�����˿���
  m_nChipRobotCount = 0;

  //�Թ����
  m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_START,&GameStart,sizeof(GameStart));

  //��Ϸ���
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

    //���û���
    GameStart.lUserMaxScore=min(pIServerUserItem->GetUserScore(),m_lUserLimitScore);

    m_pITableFrame->SendTableData(wChairID,SUB_S_GAME_START,&GameStart,sizeof(GameStart));
  }

  return true;
}

//��Ϸ����
bool  CTableFrameSink::OnEventGameConclude(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)
{
  switch(cbReason)
  {
    case GER_NORMAL:    //�������
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

        //ϵͳ��Ӯ
        LONGLONG lSystemScore = 0l;   //ϵͳʵ����Ӯ�������˲��㣩
        LONGLONG lAllScore = 0l;    //ϵͳȫ����Ӯ(�����˺����һ����)

        JudgeSystemScore(lSystemScore,lAllScore);
        m_lStorageCurrent+=lSystemScore;
        LONGLONG lTempDeduct=0;
        lTempDeduct=m_StorageDeduct;
        bool bDeduct=NeedDeductStorage();
        lTempDeduct=bDeduct?lTempDeduct:0;
        m_lStorageCurrent = m_lStorageCurrent - (m_lStorageCurrent * lTempDeduct / 1000);

        //�ʽ��
        m_lBonus += lAllScore;

        //�ʽ�ر���ʵ���С
        if(m_lBonus <= m_lStorageCurrent)
        {
          m_lBonus = m_lStorageCurrent + rand() % 1000000;
        }
      }
      else
      {
        while(true)
        {
          //�ɷ��˿�
          DispatchTableCard();

          //��̽���ж�
          if(ProbeJudge() == TRUE)
          {
            break;
          }
        }
      }

      //�����Ͼ���ע
      CopyMemory(m_lLastAllJettonPlace,m_lUserJettonScore,sizeof(m_lLastAllJettonPlace));
      //��ձ������һע
      ZeroMemory(m_lLastAllJettonArea,sizeof(m_lLastAllJettonArea));

      //�������
      LONGLONG lBankerWinScore=CalculateScore();
      //�������
      CalculateAnimalPercent();

      //д����
      WriteStorageInfo(m_lStorageCurrent,FALSE);

      //��������
      m_wBankerTime++;

      //������Ϣ
      CMD_S_GameEnd GameEnd;
      ZeroMemory(&GameEnd,sizeof(GameEnd));

      //ׯ����Ϣ
      GameEnd.nBankerTime = m_wBankerTime;
      GameEnd.lBankerTotallScore=m_lBankerWinScore;
      GameEnd.lBankerScore=lBankerWinScore;
      GameEnd.cbShaYuAddMulti=m_cbShaYuAddMulti;
      //GameEnd.lStorageStart = m_lStorageCurrent;

      //����
      memcpy(GameEnd.nAnimalPercent, m_nAnimalPercent, sizeof(GameEnd.nAnimalPercent));

      //�˿���Ϣ
      CopyMemory(GameEnd.cbTableCardArray,m_cbTableCardArray,sizeof(m_cbTableCardArray));

      //���ͻ���
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

        //���óɼ�
        GameEnd.lUserScore=m_lUserWinScore[wUserIndex];

        //��������
        GameEnd.lUserReturnScore=m_lUserReturnScore[wUserIndex];

        //����˰��
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

        //������Ϣ
        m_pITableFrame->SendTableData(wUserIndex,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
        m_pITableFrame->SendLookonData(wUserIndex,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
      }

      return true;
    }
    case GER_USER_LEAVE:    //�û��뿪
    case GER_NETWORK_ERROR:
    {
      //�м��ж�
      if(m_wCurrentBanker!=wChairID)
      {
        //��������
        LONGLONG lScore=0;
        LONGLONG lRevenue=0;

        //ͳ�Ƴɼ�
        for(int nAreaIndex=1; nAreaIndex<=AREA_COUNT; ++nAreaIndex)
        {
          lScore -= m_lUserJettonScore[nAreaIndex][wChairID];
        }

        //д�����
        if(m_pITableFrame->GetGameStatus() != GS_GAME_END)
        {
          for(int nAreaIndex=1; nAreaIndex<=AREA_COUNT; ++nAreaIndex)
          {
            if(m_lUserJettonScore[nAreaIndex][wChairID] != 0)
            {
              CMD_S_PlaceJettonFail PlaceJettonFail;
              ZeroMemory(&PlaceJettonFail,sizeof(PlaceJettonFail));

              //��Ϸ���
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

          //д�����
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
      //д����
      WriteStorageInfo(m_lStorageCurrent,FALSE);
      return true;
    }
  }

  return false;
}

//���ͳ���
bool  CTableFrameSink::OnEventSendGameScene(WORD wChiarID, IServerUserItem * pIServerUserItem, BYTE cbGameStatus, bool bSendSecret)
{
  switch(cbGameStatus)
  {
    case GAME_STATUS_FREE:      //����״̬
    {
      //���ͼ�¼
      SendGameRecord(pIServerUserItem);

      //��������
      CMD_S_StatusFree StatusFree;
      ZeroMemory(&StatusFree,sizeof(StatusFree));

      //�ر���ͷ
      m_bJettonState =false;
      //������Ϣ
      StatusFree.lApplyBankerCondition = m_lApplyBankerCondition;
      StatusFree.lAreaLimitScore = m_lAreaLimitScore;

      //ׯ����Ϣ
      StatusFree.bEnableSysBanker=m_bEnableSysBanker;
      StatusFree.wBankerUser=m_wCurrentBanker;
      StatusFree.cbBankerTime=m_wBankerTime;
      StatusFree.lBankerWinScore=m_lBankerWinScore;
      StatusFree.lBankerScore = 0x7fffffffffffffff;
      StatusFree.lStorageStart = m_lStorageCurrent;
      StatusFree.lBonus = m_lBonus;

      //����
      memcpy(StatusFree.nAnimalPercent, m_nAnimalPercent, sizeof(StatusFree.nAnimalPercent));

      if(m_wCurrentBanker!=INVALID_CHAIR)
      {
        IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(m_wCurrentBanker);
        StatusFree.lBankerScore=pIServerUserItem->GetUserScore();
      }

      //�����Ϣ
      if(pIServerUserItem->GetUserStatus()!=US_LOOKON)
      {
        StatusFree.lUserMaxScore=min(pIServerUserItem->GetUserScore(),m_lUserLimitScore*4);
      }

      //ȫ����Ϣ
      DWORD dwPassTime=(DWORD)time(NULL)-m_dwJettonTime;
      StatusFree.cbTimeLeave=(BYTE)(m_cbFreeTime-__min(dwPassTime,m_cbFreeTime));

      //��������
      CopyMemory(StatusFree.szGameRoomName, m_pGameServiceOption->szServerName, sizeof(StatusFree.szGameRoomName));
      //����������
      if(pIServerUserItem->IsAndroidUser())
      {
        tagCustomConfig *pCustomConfig = (tagCustomConfig *)m_pITableFrame->GetCustomRule();
        ASSERT(pCustomConfig);

        CopyMemory(&StatusFree.CustomAndroid, &pCustomConfig->CustomAndroid, sizeof(tagCustomAndroid));
      }
      //���ͳ���
      bool bSuccess = m_pITableFrame->SendGameScene(pIServerUserItem,&StatusFree,sizeof(StatusFree));

      //����������
      SendApplyUser(pIServerUserItem);
      //���¿����Ϣ
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
    case GS_PLACE_JETTON:   //��Ϸ״̬
    case GS_GAME_END:     //����״̬
    {

      //���ͼ�¼
      SendGameRecord(pIServerUserItem);

      //��������
      CMD_S_StatusPlay StatusPlay= {0};

      //�Ͼ���Ϣ

      //�ر���ͷ
      m_bJettonState = false;

      //ȫ����ע
      CopyMemory(StatusPlay.lAllJettonScore,m_lAllJettonScore,sizeof(StatusPlay.lAllJettonScore));

      //�����ע
      if(pIServerUserItem->GetUserStatus()!=US_LOOKON)
      {
        for(int nAreaIndex=1; nAreaIndex<=AREA_COUNT; ++nAreaIndex)
        {
          StatusPlay.lUserJettonScore[nAreaIndex] = m_lUserJettonScore[nAreaIndex][wChiarID];
        }

        //�����ע
        StatusPlay.lUserMaxScore=min(pIServerUserItem->GetUserScore(),m_lUserLimitScore);
      }

      //������Ϣ
      StatusPlay.lApplyBankerCondition=m_lApplyBankerCondition;
      StatusPlay.lAreaLimitScore=m_lAreaLimitScore;

      //ׯ����Ϣ
      StatusPlay.bEnableSysBanker=m_bEnableSysBanker;
      StatusPlay.wBankerUser=m_wCurrentBanker;
      StatusPlay.cbBankerTime=m_wBankerTime;
      StatusPlay.lBankerWinScore=m_lBankerWinScore;
      StatusPlay.lBankerScore = 0x7fffffffffffffff;
      StatusPlay.lStorageStart = m_lStorageCurrent;
      StatusPlay.lBonus = m_lBonus;

      //����
      memcpy(StatusPlay.nAnimalPercent, m_nAnimalPercent, sizeof(StatusPlay.nAnimalPercent));

      if(m_wCurrentBanker!=INVALID_CHAIR)
      {
        IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(m_wCurrentBanker);
        StatusPlay.lBankerScore=pIServerUserItem->GetUserScore();
      }

      //ȫ����Ϣ
      DWORD dwPassTime=(DWORD)time(NULL)-m_dwJettonTime;
      StatusPlay.cbTimeLeave=(BYTE)(m_cbBetTime-__min(dwPassTime,m_cbBetTime));
      StatusPlay.cbGameStatus=m_pITableFrame->GetGameStatus();

      //�����ж�
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

        //���óɼ�
        StatusPlay.lEndUserScore=m_lUserWinScore[wChiarID];

        //��������
        StatusPlay.lEndUserReturnScore=m_lUserReturnScore[wChiarID];

        //����˰��
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

        //ׯ�ҳɼ�
        StatusPlay.lEndBankerScore=m_lBankerCurGameScore;

        //�˿���Ϣ
        CopyMemory(StatusPlay.cbTableCardArray,m_cbTableCardArray,sizeof(m_cbTableCardArray));
      }

      //��������
      CopyMemory(StatusPlay.szGameRoomName, m_pGameServiceOption->szServerName, sizeof(StatusPlay.szGameRoomName));
      //����������
      if(pIServerUserItem->IsAndroidUser())
      {
        tagCustomConfig *pCustomConfig = (tagCustomConfig *)m_pITableFrame->GetCustomRule();
        ASSERT(pCustomConfig);

        CopyMemory(&StatusPlay.CustomAndroid, &pCustomConfig->CustomAndroid, sizeof(tagCustomAndroid));
      }
      //���ͳ���
      bool bSuccess = m_pITableFrame->SendGameScene(pIServerUserItem,&StatusPlay,sizeof(StatusPlay));

      //����������
      SendApplyUser(pIServerUserItem);
      //���¿����Ϣ
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

//��ʱ���¼�
bool  CTableFrameSink::OnTimerMessage(DWORD wTimerID, WPARAM wBindParam)
{
  switch(wTimerID)
  {
    case IDI_FREE:    //����ʱ��
    {
      //��ʼ��Ϸ
      m_pITableFrame->StartGame();

      //����ʱ��
      m_dwJettonTime=(DWORD)time(NULL);
      m_pITableFrame->SetGameTimer(IDI_PLACE_JETTON,m_cbBetTime*1000,1,0L);

      //����״̬
      m_pITableFrame->SetGameStatus(GS_PLACE_JETTON);

      return true;
    }
    case IDI_PLACE_JETTON:    //��עʱ��
    {
      //״̬�ж�(��ֹǿ���ظ�����)
      if(m_pITableFrame->GetGameStatus()!=GS_GAME_END)
      {
        //����״̬
        m_pITableFrame->SetGameStatus(GS_GAME_END);

        //������Ϸ
        OnEventGameConclude(INVALID_CHAIR,NULL,GER_NORMAL);

        //����ʱ��
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
    case IDI_GAME_END:      //������Ϸ
    {
      tagScoreInfo ScoreInfo[GAME_PLAYER];
      ZeroMemory(ScoreInfo,sizeof(ScoreInfo));
      LONGLONG TempStartScore=0;
      //д�����
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
        cs.Format(TEXT("�������Ϊ��%I64d���˺�Ϊ��%s"),m_lControlStorage,m_szControlName);
        //CTraceService::TraceString(cs,TraceLevel_Exception);
      }
      m_pITableFrame->WriteTableScore(ScoreInfo,CountArray(ScoreInfo));

      //������Ϸ
      m_pITableFrame->ConcludeGame(GAME_STATUS_FREE);

      //ReadConfigInformation();
      //�л�ׯ��
      ChangeBanker(false);

      //����ʱ��
      m_dwJettonTime=(DWORD)time(NULL);
      m_pITableFrame->SetGameTimer(IDI_FREE,m_cbFreeTime*1000,1,0L);

      //������Ϣ
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

//��Ϸ��Ϣ����
bool  CTableFrameSink::OnGameMessage(WORD wSubCmdID, VOID * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
  switch(wSubCmdID)
  {
    case SUB_C_PLACE_JETTON:    //�û���ע
    {
      //Ч������
      ASSERT(wDataSize==sizeof(CMD_C_PlaceJetton));
      if(wDataSize!=sizeof(CMD_C_PlaceJetton))
      {
        return false;
      }

      //�û�Ч��

      tagUserInfo * pUserData=pIServerUserItem->GetUserInfo();

      if(pUserData->cbUserStatus!=US_PLAYING)
      {
        return true;
      }

      //��Ϣ����
      CMD_C_PlaceJetton * pPlaceJetton=(CMD_C_PlaceJetton *)pDataBuffer;
      return OnUserPlaceJetton(pUserData->wChairID,pPlaceJetton->cbJettonArea,pPlaceJetton->lJettonScore);
    }
    case SUB_C_APPLY_BANKER:    //������ׯ
    {
      //�û�Ч��
      tagUserInfo * pUserData=pIServerUserItem->GetUserInfo();
      if(pUserData->cbUserStatus==US_LOOKON)
      {
        return true;
      }

      return OnUserApplyBanker(pIServerUserItem);
    }
    case SUB_C_CANCEL_BANKER:   //ȡ����ׯ
    {
      //�û�Ч��
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

      //���ÿ��
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
      //Ч������
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

      //�Ͼ���Ϣ
      //ZeroMemory(m_lLastAllJettonPlace,sizeof(m_lLastAllJettonPlace));
      //ZeroMemory(m_lLastAllJettonArea,sizeof(m_lLastAllJettonArea));

      //����Ͷ
      //m_bJettonState = true;


      //ͳ�������ע
      for(int nAreaIndex=0; nAreaIndex<=AREA_COUNT; nAreaIndex++)
      {
        m_lAllJettonScore[nAreaIndex]-=m_lUserJettonScore[nAreaIndex][pCleanMeJetton->wChairID];
        m_lUserJettonScore[nAreaIndex][pCleanMeJetton->wChairID] = 0;
      }


      ////�������
      //for (WORD wChairID=0; wChairID<GAME_PLAYER; wChairID++)
      //{
      //  //ׯ���ж�
      //  if (m_wCurrentBanker==wChairID) continue;

      //  //��ȡ�û�
      //  IServerUserItem * pIServerUserItem=m_pITableFrame->GetTableUserItem(wChairID);
      //  if (pIServerUserItem==NULL) continue;
      //
      //  //������ע
      //  for (int nAreaIndex=0; nAreaIndex<=AREA_COUNT; nAreaIndex++)
      //      CurAllJetton[nAreaIndex] += m_lUserJettonScore[nAreaIndex][wChairID];
      //}


      //��������
      CMD_S_CeanJetton CleanJetton;
      ZeroMemory(&CleanJetton,sizeof(CleanJetton));
      CopyMemory(&CleanJetton.lAllCPlaceScore,m_lAllJettonScore,sizeof(CleanJetton.lAllCPlaceScore));


      //������Ϣ
      m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_CEAN_JETTON,&CleanJetton,sizeof(CleanJetton));
      m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_CEAN_JETTON,&CleanJetton,sizeof(CleanJetton));

      return true;
    }

    case SUB_C_UPDATE_STORAGE:    //���¿��
    {
      ASSERT(wDataSize==sizeof(CMD_C_UpdateStorage));
      if(wDataSize!=sizeof(CMD_C_UpdateStorage))
      {
        return false;
      }

      //Ȩ���ж�
      if(CUserRight::IsGameCheatUser(pIServerUserItem->GetUserRight())==false)
      {
        return false;
      }

      //��Ϣ����
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
      //Ч������
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


      //��һ���
      IServerUserItem * pIServerUserItem=m_pITableFrame->GetTableUserItem(pLastJetton->wChairID);
      LONGLONG lUserScore = pIServerUserItem->GetUserScore();

      LONGLONG lLastAllJettonArea = 0L;
      LONGLONG lLastAllJettonPlace = 0L;
      LONGLONG lUserJettonScore = 0L;
      //ͳ�Ʊ��������ע
      for(int nAreaIndex=0; nAreaIndex<=AREA_COUNT; ++nAreaIndex)
      {
        lLastAllJettonArea +=m_lLastAllJettonArea[nAreaIndex][pLastJetton->wChairID];
      }

      //ͳ���Ͼ�����ע
      for(int nAreaIndex=0; nAreaIndex<=AREA_COUNT; ++nAreaIndex)
      {
        lLastAllJettonPlace += m_lLastAllJettonPlace[nAreaIndex][pLastJetton->wChairID];
      }

      //ͳ�Ʊ�������ע
      for(int nAreaIndex=0; nAreaIndex<=AREA_COUNT; ++nAreaIndex)
      {
        lUserJettonScore += m_lUserJettonScore[nAreaIndex][pLastJetton->wChairID];
      }

      bool bIsLast = false;
      //���һע
      for(int nAreaIndex=0; nAreaIndex<=AREA_COUNT; ++nAreaIndex)
      {
        //�����������һע
        if(m_lLastAllJettonArea[nAreaIndex][pLastJetton->wChairID] > 0)
        {
          bIsLast = false;
          break;
        }
        //���������һע
        if(m_lLastAllJettonArea[nAreaIndex][pLastJetton->wChairID] == 0)
        {
          bIsLast = true;
        }
      }

      bCheckPlaceJettonIsOk=true;
      //�ж��û�����Ƿ��㹻
      if(bIsLast) //�����һע
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
        //AfxMessageBox(L"�û���Ҳ�����");
        return true;
      }

      //�û�����
      if(bIsLast) //�����һע
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
        //AfxMessageBox(L"�����û����ƣ�");
        return true;
      }

      //��������
      if(bIsLast) //�����һע
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
      else  //�����һע
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
        //AfxMessageBox(L"�����������ƣ�");
        return true;
      }



      //��������
      LONGLONG  TemplAllJettonScore[AREA_COUNT+1];           //ȫ����ע
      LONGLONG  TemplUserJettonScore[AREA_COUNT+1][GAME_PLAYER];   //������ע
      LONGLONG  TemplLastAllJettonPlace[AREA_COUNT +1][GAME_PLAYER]; //�Ͼ�����ע
      LONGLONG  TempLastAllJettonArea[AREA_COUNT +1][GAME_PLAYER];   //���������ע

      CopyMemory(TemplAllJettonScore,m_lAllJettonScore,sizeof(TemplAllJettonScore));
      CopyMemory(TemplUserJettonScore,m_lUserJettonScore,sizeof(m_lUserJettonScore));
      CopyMemory(TemplLastAllJettonPlace,m_lLastAllJettonPlace,sizeof(m_lLastAllJettonPlace));
      CopyMemory(TempLastAllJettonArea,m_lLastAllJettonArea,sizeof(TempLastAllJettonArea));

      /////////////////////////////////////////////////////////////////////

      //    m_lLastAllJettonPlace[AREA_COUNT +1][GAME_PLAYER]; //�Ͼ�����ע

      //    m_lLastAllJettonArea[AREA_COUNT +1][GAME_PLAYER];  //�������һ�β���

      /////////////////////////////////////////////////////////////////////

      //�����һע �ظ���һ������ע
      if(bIsLast == true)
      {
        if(bCheckPlaceJettonIsOk == true)
        {
          //��������
          for(int nAreaIndex=0; nAreaIndex<=AREA_COUNT; nAreaIndex++)
          {
            //�Ͼ�����ע ����

            //�������һ�β��� ����   0 (��Ϸ����ʱ���)

            //ȫ����ע
            TemplAllJettonScore[nAreaIndex]+=TemplLastAllJettonPlace[nAreaIndex][pLastJetton->wChairID];

            //������ע
            TemplUserJettonScore[nAreaIndex][pLastJetton->wChairID]+=TemplLastAllJettonPlace[nAreaIndex][pLastJetton->wChairID];

          }
        }
      }

      //�����һע �ظ��������һע
      else
      {

        //�����һע ֻ��һע
        if(bCheckPlaceJettonIsOk == true)
        {
          //��������
          for(int nAreaIndex=0; nAreaIndex<=AREA_COUNT; nAreaIndex++)
          {
            //�Ͼ�����ע ����     0 (�ڱ����û���һעʱ���)

            //�������һ�β��� ����

            //ȫ����ע
            TemplAllJettonScore[nAreaIndex]+=TempLastAllJettonArea[nAreaIndex][pLastJetton->wChairID];

            //������ע
            TemplUserJettonScore[nAreaIndex][pLastJetton->wChairID]+=TempLastAllJettonArea[nAreaIndex][pLastJetton->wChairID];

          }

        }
      }


      if(bCheckPlaceJettonIsOk)
      {
        //��������
        CMD_S_ContiueJetton PlaceLastJetton;
        ZeroMemory(&PlaceLastJetton,sizeof(PlaceLastJetton));

        //�������
        PlaceLastJetton.wChairID=pLastJetton->wChairID;
        //��ȡ�û�
        IServerUserItem * pIServerUserItem=m_pITableFrame->GetTableUserItem(pLastJetton->wChairID);
        if(pIServerUserItem != NULL)
        {
          PlaceLastJetton.cbAndroid = pIServerUserItem->IsAndroidUser()? TRUE : FALSE;
        }

        //���ñ���
        CopyMemory(m_lAllJettonScore,TemplAllJettonScore,sizeof(TemplAllJettonScore));
        CopyMemory(m_lUserJettonScore,TemplUserJettonScore,sizeof(TemplUserJettonScore));
        CopyMemory(m_lLastAllJettonPlace,TemplLastAllJettonPlace,sizeof(TemplLastAllJettonPlace));
        CopyMemory(m_lLastAllJettonArea,TempLastAllJettonArea,sizeof(TempLastAllJettonArea));

        //LONGLONG  TemplAllJettonScore[AREA_COUNT+1];           //ȫ����ע
        //LONGLONG  TemplUserJettonScore[AREA_COUNT+1][GAME_PLAYER];   //������ע
        //LONGLONG  TemplLastAllJettonPlace[AREA_COUNT +1][GAME_PLAYER]; //�Ͼ�����ע
        //LONGLONG  TempLastAllJettonArea[AREA_COUNT +1][GAME_PLAYER];   //���������ע

        CopyMemory(PlaceLastJetton.lAllJettonScore,m_lAllJettonScore,sizeof(PlaceLastJetton.lAllJettonScore));

        for(int nAreaIndex=0; nAreaIndex<=AREA_COUNT; nAreaIndex++)
        {
          PlaceLastJetton.lUserJettonScore[nAreaIndex] += m_lUserJettonScore[nAreaIndex][pLastJetton->wChairID];
        }

        //������Ϣ
        m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_CONTINU_JETTON,&PlaceLastJetton,sizeof(PlaceLastJetton));
        m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_CONTINU_JETTON,&PlaceLastJetton,sizeof(PlaceLastJetton));
      }

    }
    return true;
  }

  return false;
}


//�����Ϣ����
bool  CTableFrameSink::OnFrameMessage(WORD wSubCmdID, VOID * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
  return false;
}

//�����¼�
bool CTableFrameSink::OnGameDataBase(WORD wRequestID, VOID * pData, WORD wDataSize)
{
  return false;
}

//�û�����
bool  CTableFrameSink::OnActionUserSitDown(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
  //��ʼ����
  m_lUserStartScore[wChairID] = pIServerUserItem->GetUserScore();
  //����ʱ��
  if((bLookonUser==false)&&(m_dwJettonTime==0L))
  {
    m_dwJettonTime=(DWORD)time(NULL);
    m_pITableFrame->SetGameTimer(IDI_FREE,m_cbFreeTime*1000,1,NULL);
    m_pITableFrame->SetGameStatus(GAME_STATUS_FREE);
  }
  return true;
}

//�û�����
bool  CTableFrameSink::OnActionUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
  //��ʼ����
  m_lUserStartScore[wChairID] = 0;
  //��¼�ɼ�
  if(bLookonUser==false)
  {
    //�л�ׯ��
    if(wChairID==m_wCurrentBanker)
    {
      ChangeBanker(true);
    }

    //ȡ������
    for(WORD i=0; i<m_ApplyUserArray.GetCount(); ++i)
    {
      if(wChairID!=m_ApplyUserArray[i])
      {
        continue;
      }

      //ɾ�����
      m_ApplyUserArray.RemoveAt(i);

      //�������
      CMD_S_CancelBanker CancelBanker;
      ZeroMemory(&CancelBanker,sizeof(CancelBanker));

      tagUserInfo * pUserInfo = pIServerUserItem->GetUserInfo();
      //���ñ���
      lstrcpyn(CancelBanker.szCancelUser,pUserInfo->szNickName,CountArray(CancelBanker.szCancelUser));

      //������Ϣ
      m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));
      m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));

      break;
    }

    return true;
  }

  return true;
}

//��ע�¼�
bool CTableFrameSink::OnUserPlaceJetton(WORD wChairID, BYTE cbJettonArea, LONGLONG lJettonScore)
{
  //Ч�����
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

    //������Ϣ
    m_pITableFrame->SendTableData(wChairID,SUB_S_PLACE_JETTON_FAIL,&PlaceJettonFail,sizeof(PlaceJettonFail));
    return true;
  }

  //ׯ���ж�
  if(m_wCurrentBanker==wChairID)
  {
    return true;
  }
  if(m_bEnableSysBanker==false && m_wCurrentBanker==INVALID_CHAIR)
  {
    return true;
  }

  //��������
  IServerUserItem * pIServerUserItem=m_pITableFrame->GetTableUserItem(wChairID);
  LONGLONG lJettonCount=0L;
  LONGLONG lJettonPlace = 0L;
  for(int nAreaIndex=0; nAreaIndex<=AREA_COUNT; ++nAreaIndex)
  {
    lJettonCount += m_lUserJettonScore[nAreaIndex][wChairID];
  }

  //��һ���
  LONGLONG lUserScore = pIServerUserItem->GetUserScore();

  //�Ϸ�У��
  if(lUserScore < lJettonCount + lJettonScore)
  {
    return true;
  }
  if(m_lUserLimitScore < lJettonCount + lJettonScore)
  {
    return true;
  }

  //�ɹ���ʶ
  bool bPlaceJettonSuccess=true;
  //����Ͷ
  m_bJettonState = true;

  //�Ϸ���֤
  if(GetUserMaxJetton(wChairID,cbJettonArea) >= lJettonScore)
  {
    //��������֤
    if(pIServerUserItem->IsAndroidUser())
    {
      //��������
      if(m_lRobotAreaScore[cbJettonArea] + lJettonScore > m_lRobotAreaLimit)
      {
        return true;
      }

      //��Ŀ����
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

      //ͳ�Ʒ���
      if(bPlaceJettonSuccess)
      {
        m_lRobotAreaScore[cbJettonArea] += lJettonScore;
      }
    }


    if(bPlaceJettonSuccess)
    {

      for(int nAreaIndex=0; nAreaIndex<=AREA_COUNT; ++nAreaIndex)
      {
        //����Ͼ���ע
        m_lLastAllJettonPlace[nAreaIndex][wChairID]=0;
        //��ձ������һע
        m_lLastAllJettonArea[nAreaIndex][wChairID]=0;
      }
      //���汾�����һע
      m_lLastAllJettonArea[cbJettonArea][wChairID] += lJettonScore;

      //ȫ����ע
      m_lAllJettonScore[cbJettonArea] += lJettonScore;

      //������ע
      m_lUserJettonScore[cbJettonArea][wChairID] += lJettonScore;
    }
  }
  else
  {
    bPlaceJettonSuccess=false;
  }

  if(bPlaceJettonSuccess)
  {
    //��������
    CMD_S_PlaceJetton PlaceJetton;
    ZeroMemory(&PlaceJetton,sizeof(PlaceJetton));

    CopyMemory(&PlaceJetton.lUserStartScore, m_lUserStartScore, sizeof(m_lUserStartScore));
    //�������
    PlaceJetton.wChairID=wChairID;
    PlaceJetton.cbJettonArea=cbJettonArea;
    PlaceJetton.lJettonScore=lJettonScore;

    for(int nAreaIndex=0; nAreaIndex<=AREA_COUNT; ++nAreaIndex)
    {
      PlaceJetton.lUserJettonScore[nAreaIndex] += m_lUserJettonScore[nAreaIndex][wChairID];
    }

    //��ȡ�û�
    IServerUserItem * pIServerUserItem=m_pITableFrame->GetTableUserItem(wChairID);
    if(pIServerUserItem != NULL)
    {
      PlaceJetton.cbAndroid = pIServerUserItem->IsAndroidUser()? TRUE : FALSE;
    }

    //������Ϣ
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

    //������Ϣ
    m_pITableFrame->SendTableData(wChairID,SUB_S_PLACE_JETTON_FAIL,&PlaceJettonFail,sizeof(PlaceJettonFail));
  }

  return true;
}
void CTableFrameSink::RandList(BYTE cbCardBuffer[], BYTE cbBufferCount)
{

  //����׼��
  BYTE *cbCardData = new BYTE[cbBufferCount];
  CopyMemory(cbCardData,cbCardBuffer,cbBufferCount);

  //�����˿�
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


//�����˿�
void CTableFrameSink::DispatchTableCard()
{
  //����˿�
  ZeroMemory(m_cbTableCardArray,sizeof(m_cbTableCardArray));

  //��ȡ�˿�
  m_cbTableCardArray[0] = GetRandCardValue();

  //ͨɱ
  if(m_cbTableCardArray[0] == 11)
  {
    m_cbShaYuAddMulti = 0;
    return;
  }

  //ͨ��
  if(m_cbTableCardArray[0] == 25)
  {
    m_cbShaYuAddMulti = 0;
    return;
  }

  //����
  if(m_cbTableCardArray[0] == 4)
  {
    m_cbShaYuAddMulti =0;
    return;
  }

  //���� �������
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

    //�������
    DWORD wTick = GetTickCount();

    //���ʺ�ֵ
    INT nChanceAndValue = 0;
    for(int n = 0; n < CountArray(cbnChance); ++n)
    {
      nChanceAndValue += cbnChance[n];
    }

    INT nMuIndex = 0;
    int nRandNum = 0;         //�������
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

    m_cbShaYuAddMulti=cbControlArea[nMuIndex]; //6-100���
  }
  else
  {
    m_cbShaYuAddMulti = 0;
  }
}

//��ȡ��ֵ
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
  int n10=m_nMultiple[9];     //ͨɱ
  int n11=m_nMultiple[10];    //ͨ��
  int n12=m_nMultiple[11];    //����

  INT cbControlArea[28] = { 1, 2, 3, 4,  5, 6, 7, 8, 9, 10,  11,  12,13,14, 15,16,17, 18,  19,20, 21, 22, 23, 24,  25, 26, 27, 28 };
  INT cbnChance[28] = { n1,n1,n1,    n12,  n2, n2, n2, n3,n3,n3, n10, n4,n4,n4,n5,n5,n5, n9,  n6, n6, n6, n7,n7,n7,  n11, n8, n8, n8 };

  m_GameLogic.RandArray(cbControlArea, CountArray(cbControlArea), cbnChance, CountArray(cbnChance));

  //�������
  DWORD wTick = GetTickCount();

  //���ʺ�ֵ
  INT nChanceAndValue = 0;
  for(int n = 0; n < CountArray(cbnChance); ++n)
  {
    nChanceAndValue += cbnChance[n];
  }

  INT nMuIndex = 0;
  int nRandNum = 0;         //�������
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

//��ȡ��ֵ
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
  int n10=m_nMultiple[9];     //ͨɱ
  int n11=m_nMultiple[10];    //ͨ��
  int n12=m_nMultiple[11];    //����

  INT cbControlArea[24] = {1,2,3, 5,6,7, 8,9,10, 12,13,14, 15,16,17, 19,20,21, 22,23,24,26,27,28};

  INT cbnChance[24]= { n1,n1,n1, n2,n2,n2, n3,n3,n3, n4,n4,n4, n5,n5,n5, n6,n6,n6, n7,n7,n7, n8,n8,n8 };

  m_GameLogic.RandArray(cbControlArea, CountArray(cbControlArea), cbnChance, CountArray(cbnChance));

  //�������
  DWORD wTick = GetTickCount();

  //���ʺ�ֵ
  INT nChanceAndValue = 0;
  for(int n = 0; n < CountArray(cbnChance); ++n)
  {
    nChanceAndValue += cbnChance[n];
  }

  INT nMuIndex = 0;
  int nRandNum = 0;         //�������
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

//����ׯ��
bool CTableFrameSink::OnUserApplyBanker(IServerUserItem *pIApplyServerUserItem)
{
  //�Ϸ��ж�
  LONGLONG lUserScore=pIApplyServerUserItem->GetUserScore();
  if(lUserScore<m_lApplyBankerCondition)
  {
    m_pITableFrame->SendGameMessage(pIApplyServerUserItem,TEXT("��Ľ�Ҳ���������ׯ�ң�����ʧ�ܣ�"),SMT_CHAT|SMT_EJECT);
    return true;
  }

  if(pIApplyServerUserItem->IsAndroidUser()&&(m_ApplyUserArray.GetCount())>m_nRobotListMaxCount)
  {
    return true;
  }
  //�����ж�
  WORD wApplyUserChairID=pIApplyServerUserItem->GetChairID();
  for(INT_PTR nUserIdx=0; nUserIdx<m_ApplyUserArray.GetCount(); ++nUserIdx)
  {
    WORD wChairID=m_ApplyUserArray[nUserIdx];
    if(wChairID==wApplyUserChairID)
    {
      m_pITableFrame->SendGameMessage(pIApplyServerUserItem,TEXT("���Ѿ�������ׯ�ң�����Ҫ�ٴ����룡"),SMT_CHAT|SMT_EJECT);
      return true;
    }
  }

  //������Ϣ
  m_ApplyUserArray.Add(wApplyUserChairID);

  //�������
  CMD_S_ApplyBanker ApplyBanker;
  ZeroMemory(&ApplyBanker,sizeof(ApplyBanker));

  //���ñ���
  ApplyBanker.wApplyUser=wApplyUserChairID;

  //������Ϣ
  m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_APPLY_BANKER, &ApplyBanker, sizeof(ApplyBanker));
  m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_APPLY_BANKER, &ApplyBanker, sizeof(ApplyBanker));

  //�л��ж�
  if(m_pITableFrame->GetGameStatus()==GAME_STATUS_FREE && m_ApplyUserArray.GetCount()==1)
  {
    ChangeBanker(false);
  }

  return true;
}

//ȡ������
bool CTableFrameSink::OnUserCancelBanker(IServerUserItem *pICancelServerUserItem)
{
  //��ǰׯ��
  if(pICancelServerUserItem->GetChairID()==m_wCurrentBanker && m_pITableFrame->GetGameStatus()!=GAME_STATUS_FREE)
  {
    //������Ϣ
    m_pITableFrame->SendGameMessage(pICancelServerUserItem,TEXT("��Ϸ�Ѿ���ʼ��������ȡ����ׯ��"),SMT_CHAT|SMT_EJECT);
    return true;
  }

  //�����ж�
  for(WORD i=0; i<m_ApplyUserArray.GetCount(); ++i)
  {
    //��ȡ���
    WORD wChairID=m_ApplyUserArray[i];
    IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(wChairID);

    //��������
    if(pIServerUserItem==NULL)
    {
      continue;
    }
    if(pIServerUserItem->GetUserID()!=pICancelServerUserItem->GetUserID())
    {
      continue;
    }

    //ɾ�����
    m_ApplyUserArray.RemoveAt(i);

    if(m_wCurrentBanker!=wChairID)
    {
      //�������
      CMD_S_CancelBanker CancelBanker;
      ZeroMemory(&CancelBanker,sizeof(CancelBanker));

      tagUserInfo * pUserInfo = pIServerUserItem->GetUserInfo();
      //���ñ���
      lstrcpyn(CancelBanker.szCancelUser,pUserInfo->szNickName,CountArray(CancelBanker.szCancelUser));

      //������Ϣ
      m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));
      m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));
    }
    else if(m_wCurrentBanker==wChairID)
    {
      //�л�ׯ��
      m_wCurrentBanker=INVALID_CHAIR;
      ChangeBanker(true);
    }

    return true;
  }

  return true;
}

//����ׯ��
bool CTableFrameSink::ChangeBanker(bool bCancelCurrentBanker)
{
  //�л���ʶ
  bool bChangeBanker=false;

  //ȡ����ǰ
  if(bCancelCurrentBanker)
  {
    for(WORD i=0; i<m_ApplyUserArray.GetCount(); ++i)
    {
      //��ȡ���
      WORD wChairID=m_ApplyUserArray[i];

      //��������
      if(wChairID!=m_wCurrentBanker)
      {
        continue;
      }

      //ɾ�����
      m_ApplyUserArray.RemoveAt(i);

      break;
    }

    //����ׯ��
    m_wCurrentBanker=INVALID_CHAIR;

    //�ֻ��ж�
    if(m_pITableFrame->GetGameStatus()==GAME_STATUS_FREE && FindSuitBanker())
    {
      m_wCurrentBanker=m_ApplyUserArray[0];
    }

    //���ñ���
    bChangeBanker=true;
    m_bExchangeBanker = true;
  }
  //��ׯ�ж�
  else if(m_wCurrentBanker!=INVALID_CHAIR)
  {
    //��ȡׯ��
    IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(m_wCurrentBanker);

    if(pIServerUserItem!= NULL)
    {
      LONGLONG lBankerScore=pIServerUserItem->GetUserScore();

      //�����ж�
      if(m_lPlayerBankerMAX<=m_wBankerTime || lBankerScore<m_lApplyBankerCondition)
      {
        //ׯ�������ж� ͬһ��ׯ�������ֻ�ж�һ��
        if(m_lPlayerBankerMAX <= m_wBankerTime && m_bExchangeBanker)
        {
          bool bScoreMAX = true;
          m_bExchangeBanker = false;

          for(WORD i=0; i<m_ApplyUserArray.GetCount(); ++i)
          {
            //��ȡ���
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
            //��ʾ��Ϣ
            TCHAR szTipMsg[128] = {};
            if(wAddTimes==m_lBankerAdd)
            {
              myprintf(szTipMsg,CountArray(szTipMsg),TEXT("[ %s ]�ķ�����������������ׯ��ң����%lld�ζ�����ׯ����!"),pUserInfo->szNickName,wAddTimes);
            }
            else if(wAddTimes==m_lBankerScoreAdd)
            {
              myprintf(szTipMsg,CountArray(szTipMsg),TEXT("[ %s ]�ķ�������[%I64d]�����%lld�ζ�����ׯ����!"),pUserInfo->szNickName,m_lBankerScoreMAX,wAddTimes);
            }
            else if(wAddTimes==(m_lBankerScoreAdd+m_lBankerAdd))
            {
              myprintf(szTipMsg,CountArray(szTipMsg),TEXT("[ %s ]�ķ�������[%I64d]�ҳ�������������ׯ��ң����%lld�ζ�����ׯ����!"),pUserInfo->szNickName,m_lBankerScoreMAX,wAddTimes);
            }

            if(wAddTimes != 0)
            {
              //������Ϣ
              SendGameMessage(INVALID_CHAIR,szTipMsg);
              return true;
            }
            return true;
          }
        }

        //�������
        for(WORD i=0; i<m_ApplyUserArray.GetCount(); ++i)
        {
          //��ȡ���
          WORD wChairID=m_ApplyUserArray[i];

          //��������
          if(wChairID!=m_wCurrentBanker)
          {
            continue;
          }

          //ɾ�����
          m_ApplyUserArray.RemoveAt(i);

          break;
        }

        //����ׯ��
        m_wCurrentBanker=INVALID_CHAIR;

        //�ֻ��ж�
        if(FindSuitBanker())
        {
          m_wCurrentBanker=m_ApplyUserArray[0];
        }

        bChangeBanker=true;
        m_bExchangeBanker = true;
        tagUserInfo * pUserInfo = pIServerUserItem->GetUserInfo();
        //��ʾ��Ϣ
        TCHAR szTipMsg[128];
        if(lBankerScore<m_lApplyBankerCondition)
        {
          myprintf(szTipMsg,CountArray(szTipMsg),TEXT("[ %s ]��������(%I64d)��ǿ�л�ׯ!"),pUserInfo->szNickName,m_lApplyBankerCondition);
        }
        else
        {
          myprintf(szTipMsg,CountArray(szTipMsg),TEXT("[ %s ]��ׯ�����ﵽ(%lld)��ǿ�л�ׯ!"),pUserInfo->szNickName,m_lPlayerBankerMAX);
        }

        //������Ϣ
        SendGameMessage(INVALID_CHAIR,szTipMsg);
      }
    }
    else
    {
      for(WORD i=0; i<m_ApplyUserArray.GetCount(); ++i)
      {
        //��ȡ���
        WORD wChairID=m_ApplyUserArray[i];

        //��������
        if(wChairID!=m_wCurrentBanker)
        {
          continue;
        }

        //ɾ�����
        m_ApplyUserArray.RemoveAt(i);

        break;
      }
      //����ׯ��
      m_wCurrentBanker=INVALID_CHAIR;
    }

  }
  //ϵͳ��ׯ
  else if(m_wCurrentBanker==INVALID_CHAIR &&FindSuitBanker())
  {
    m_wCurrentBanker=m_ApplyUserArray[0];
    bChangeBanker=true;
    m_bExchangeBanker = true;
  }

  //�л��ж�
  if(bChangeBanker)
  {
    //�����ׯ��
    m_lPlayerBankerMAX = m_lBankerMAX;

    //���ñ���
    m_wBankerTime = 0;
    m_lBankerWinScore=0;

    //������Ϣ
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
      //��ȡ��Ϣ
      LONGLONG lMessageCount=GetPrivateProfileInt(m_szGameRoomName,TEXT("MessageCount"),0,m_szConfigFileName);
      if(lMessageCount!=0)
      {
        //��ȡ����
        LONGLONG lIndex=rand()%lMessageCount;
        TCHAR szKeyName[32],szMessage1[256],szMessage2[256];
        myprintf(szKeyName,CountArray(szKeyName),TEXT("Item%I64d"),lIndex);
        GetPrivateProfileString(m_szGameRoomName,szKeyName,TEXT("��ϲ[ %s ]��ׯ"),szMessage1,CountArray(szMessage1),m_szConfigFileName);

        //��ȡ���
        IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(m_wCurrentBanker);
        tagUserInfo * pUserInfo = pIServerUserItem->GetUserInfo();
        //������Ϣ
        myprintf(szMessage2,CountArray(szMessage2),szMessage1,pUserInfo->szNickName);
        SendGameMessage(INVALID_CHAIR,szMessage2);
      }
    }
  }

  return bChangeBanker;
}

//����ׯ��
void CTableFrameSink::SendApplyUser(IServerUserItem *pRcvServerUserItem)
{
  for(INT_PTR nUserIdx=0; nUserIdx<m_ApplyUserArray.GetCount(); ++nUserIdx)
  {
    WORD wChairID=m_ApplyUserArray[nUserIdx];

    //��ȡ���
    IServerUserItem *pServerUserItem=m_pITableFrame->GetTableUserItem(wChairID);
    if(!pServerUserItem)
    {
      continue;
    }

    //ׯ���ж�
    if(pServerUserItem->GetChairID()==m_wCurrentBanker)
    {
      continue;
    }

    //�������
    CMD_S_ApplyBanker ApplyBanker;
    ApplyBanker.wApplyUser=wChairID;

    //������Ϣ
    m_pITableFrame->SendUserItemData(pRcvServerUserItem, SUB_S_APPLY_BANKER, &ApplyBanker, sizeof(ApplyBanker));
  }
}

//�û�����
bool  CTableFrameSink::OnActionUserOffLine(WORD wChairID, IServerUserItem * pIServerUserItem)
{
  //�л�ׯ��
  if(wChairID==m_wCurrentBanker)
  {
    ChangeBanker(true);
  }

  //ȡ������
  for(WORD i=0; i<m_ApplyUserArray.GetCount(); ++i)
  {
    if(wChairID!=m_ApplyUserArray[i])
    {
      continue;
    }

    //ɾ�����
    m_ApplyUserArray.RemoveAt(i);

    //�������
    CMD_S_CancelBanker CancelBanker;
    ZeroMemory(&CancelBanker,sizeof(CancelBanker));
    tagUserInfo * pUserInfo = pIServerUserItem->GetUserInfo();
    //���ñ���
    lstrcpyn(CancelBanker.szCancelUser,pUserInfo->szNickName,CountArray(CancelBanker.szCancelUser));

    //������Ϣ
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
//�����ע
LONGLONG CTableFrameSink::GetUserMaxJetton(WORD wChairID,BYTE Area)
{
  IServerUserItem *pIMeServerUserItem=m_pITableFrame->GetTableUserItem(wChairID);
  if(NULL==pIMeServerUserItem)
  {
    return 0L;
  }


  //����ע��
  LONGLONG lNowJetton = 0;
  ASSERT(AREA_COUNT<=CountArray(m_lUserJettonScore));
  for(int nAreaIndex=0; nAreaIndex<=AREA_COUNT; ++nAreaIndex)
  {
    lNowJetton += m_lUserJettonScore[nAreaIndex][wChairID];
  }

  //ׯ�ҽ��
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

  //��������
  LONGLONG lMeMaxScore = min((pIMeServerUserItem->GetUserScore()-lNowJetton), m_lUserLimitScore);

  //��������
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
  //ׯ������
  lMeMaxScore=min(lMeMaxScore,lBankerScore/(diMultiple[Area-1]));

  //��������
  ASSERT(lMeMaxScore >= 0);
  lMeMaxScore = max(lMeMaxScore, 0);

  return (LONGLONG)(lMeMaxScore);
}
//����÷�
LONGLONG CTableFrameSink::CalculateScore()
{
  BYTE  bcResulteOut[AREA_COUNT];
  memset(bcResulteOut,0,AREA_COUNT);
  m_GameLogic.GetCardType(m_cbTableCardArray[0],1,bcResulteOut);
  BYTE  bcResultTwo[AREA_COUNT];
  memset(bcResultTwo,0,AREA_COUNT);
  m_GameLogic.GetCardType(m_cbTableCardArray[1],1,bcResultTwo);

  //�������
  if(m_cbTableCardArray[0] == 4 && bcResulteOut[11] == 100)
  {
    //��Ϸ��¼
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

    //�ƶ��±�
    m_nRecordLast = (m_nRecordLast+1) % MAX_SCORE_HISTORY;
    if(m_nRecordLast == m_nRecordFirst)
    {
      m_nRecordFirst = (m_nRecordFirst+1) % MAX_SCORE_HISTORY;
    }

    //ׯ������
    LONGLONG lBankerWinScore = 0;

    //��ҳɼ�
    ZeroMemory(m_lUserWinScore, sizeof(m_lUserWinScore));
    ZeroMemory(m_lUserReturnScore, sizeof(m_lUserReturnScore));
    ZeroMemory(m_lUserRevenue, sizeof(m_lUserRevenue));
    LONGLONG lUserLostScore[GAME_PLAYER];
    ZeroMemory(lUserLostScore, sizeof(lUserLostScore));

    LONGLONG lUserWinScore = 0;

    //�������
    for(WORD wChairID=0; wChairID<GAME_PLAYER; wChairID++)
    {
      //ׯ���ж�
      if(m_wCurrentBanker==wChairID)
      {
        continue;
      }

      //��ȡ�û�
      IServerUserItem * pIServerUserItem=m_pITableFrame->GetTableUserItem(wChairID);
      if(pIServerUserItem==NULL)
      {
        continue;
      }

      //���� 2 //���� 3
      //int Milt[13] = {0,6, 6, 8, 8, 12, 12,8,8,24,2,2,0};

      LONGLONG lMeJetton=0L;
      LONGLONG lMeJettonReturn = 0L;

      //ͳ�����Ӯ��
      lMeJetton += m_lUserJettonScore[9][wChairID];
      /*for (int nAreaIndex=0; nAreaIndex<=AREA_COUNT; ++nAreaIndex)
      {
        lMeJetton += m_lUserJettonScore[nAreaIndex][wChairID];
      }*/

      //ͳ�������ע
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

      //�ܵķ���
      m_lUserWinScore[wChairID] += lUserLostScore[wChairID];
    }


    if(m_wCurrentBanker!=INVALID_CHAIR)
    {
      //ׯ�ҳɼ�
      m_lUserWinScore[m_wCurrentBanker] = lBankerWinScore;
    }

    //����˰��
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



    //�ۼƻ���
    m_lBankerWinScore += lBankerWinScore;

    //��ǰ����
    m_lBankerCurGameScore=lBankerWinScore;

    return lBankerWinScore;
  }

  //ͨɱ����
  if(m_cbTableCardArray[0] == 11 && bcResulteOut[10] == 255)
  {
    //��Ϸ��¼
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

    //�ƶ��±�
    m_nRecordLast = (m_nRecordLast+1) % MAX_SCORE_HISTORY;
    if(m_nRecordLast == m_nRecordFirst)
    {
      m_nRecordFirst = (m_nRecordFirst+1) % MAX_SCORE_HISTORY;
    }

    //ׯ������
    LONGLONG lBankerWinScore = 0;

    //��ҳɼ�
    ZeroMemory(m_lUserWinScore, sizeof(m_lUserWinScore));
    ZeroMemory(m_lUserReturnScore, sizeof(m_lUserReturnScore));
    ZeroMemory(m_lUserRevenue, sizeof(m_lUserRevenue));
    LONGLONG lUserLostScore[GAME_PLAYER];
    ZeroMemory(lUserLostScore, sizeof(lUserLostScore));

    //�����ע
    LONGLONG *pUserScore[AREA_COUNT+1];
    pUserScore[0]=NULL;
    for(int i = 1; i<AREA_COUNT+1; i++)
    {
      pUserScore[i]=m_lUserJettonScore[i];
    }


    for(WORD wChairID=0; wChairID<GAME_PLAYER; wChairID++)
    {
      //�������
      //ׯ���ж�
      if(m_wCurrentBanker==wChairID)
      {
        continue;
      }

      //��ȡ�û�
      IServerUserItem * pIServerUserItem=m_pITableFrame->GetTableUserItem(wChairID);
      if(pIServerUserItem==NULL)
      {
        continue;
      }

      LONGLONG lMeJetton=0L;
      LONGLONG lMeJettonReturn = 0L;

      //ͳ���������
      for(int nAreaIndex=0; nAreaIndex<=AREA_COUNT; ++nAreaIndex)
      {
        lMeJetton += (m_lUserJettonScore[nAreaIndex][wChairID]);
      }

      lMeJettonReturn=lMeJetton;
      //if(lMeJettonReturn >0)m_lStorageCurrent+=(lMeJettonReturn);

      if(lMeJetton>= 0L)
      {
        //ͨɱ���
        lUserLostScore[wChairID] -= lMeJetton;
        lBankerWinScore += lMeJettonReturn;
      }

      //�ܵķ���
      m_lUserWinScore[wChairID] += lUserLostScore[wChairID];
    }

    //ׯ�ҳɼ�
    if(m_wCurrentBanker!=INVALID_CHAIR)
    {
      m_lUserWinScore[m_wCurrentBanker] = lBankerWinScore;
    }

    //����˰��
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

    //�ۼƻ���
    m_lBankerWinScore += lBankerWinScore;

    //��ǰ����
    m_lBankerCurGameScore=lBankerWinScore;

    return lBankerWinScore;
  }

  //ͨ�����
  if(m_cbTableCardArray[0] == 25 && bcResulteOut[9] == 1)
  {

    //��Ϸ��¼
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

    //�ƶ��±�
    m_nRecordLast = (m_nRecordLast+1) % MAX_SCORE_HISTORY;
    if(m_nRecordLast == m_nRecordFirst)
    {
      m_nRecordFirst = (m_nRecordFirst+1) % MAX_SCORE_HISTORY;
    }

    //ׯ������
    LONGLONG lBankerWinScore = 0;

    //��ҳɼ�
    ZeroMemory(m_lUserWinScore, sizeof(m_lUserWinScore));
    ZeroMemory(m_lUserReturnScore, sizeof(m_lUserReturnScore));
    ZeroMemory(m_lUserRevenue, sizeof(m_lUserRevenue));
    LONGLONG lUserLostScore[GAME_PLAYER];
    ZeroMemory(lUserLostScore, sizeof(lUserLostScore));



    LONGLONG lUserWinScore = 0;


    for(WORD wChairID=0; wChairID<GAME_PLAYER; wChairID++)
    {
      //�������
      //ׯ���ж�
      if(m_wCurrentBanker==wChairID)
      {
        continue;
      }

      //��ȡ�û�
      IServerUserItem * pIServerUserItem=m_pITableFrame->GetTableUserItem(wChairID);
      if(pIServerUserItem==NULL)
      {
        continue;
      }

      //���� 2 //���� 3
      int Milt[13] = {0,6,  6, 8, 8, 12, 12,8,8,24,2,2,0};

      LONGLONG lMeJetton=0L;
      LONGLONG lMeJettonReturn = 0L;

      //ͳ�����Ӯ��
      for(int nAreaIndex=0; nAreaIndex<=AREA_COUNT; ++nAreaIndex)
      {
        lMeJetton += (m_lUserJettonScore[nAreaIndex][wChairID] * Milt[nAreaIndex]);
      }

      //ͳ�������ע
      for(int nAreaIndex=1; nAreaIndex<=AREA_COUNT; ++nAreaIndex)
      {
        lMeJettonReturn += m_lUserJettonScore[nAreaIndex][wChairID];
      }

      //if(lMeJettonReturn >0)m_lStorageCurrent-=lMeJettonReturn;
      if(lMeJetton>= 0L)
      {
        //ͨ����
        m_lUserWinScore[wChairID] += lMeJetton;
        m_lUserReturnScore[wChairID] += lMeJettonReturn;
        lBankerWinScore -= lMeJetton;
        //m_lStorageCurrent-=lMeJetton;
      }

      //�ܵķ���
      m_lUserWinScore[wChairID] += lUserLostScore[wChairID];
    }

    //ׯ�ҳɼ�
    if(m_wCurrentBanker!=INVALID_CHAIR)
    {
      m_lUserWinScore[m_wCurrentBanker] = lBankerWinScore;
    }

    //����˰��
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

    //�ۼƻ���
    m_lBankerWinScore += lBankerWinScore;

    //��ǰ����
    m_lBankerCurGameScore=lBankerWinScore;

    return lBankerWinScore;

  }


  /*-----------------------------��������-----------------------------------*/
  for(int i = 0; i<AREA_COUNT; i++)
  {
    bcResulteOut[i] += bcResultTwo[i];
  }

  //���� ���⽱��
  if(m_cbTableCardArray[0]%7==4)
  {
    bcResulteOut[ID_SHA_YU-1]+=m_cbShaYuAddMulti;
  }

  //��Ϸ��¼
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

  //�ƶ��±�
  m_nRecordLast = (m_nRecordLast+1) % MAX_SCORE_HISTORY;
  if(m_nRecordLast == m_nRecordFirst)
  {
    m_nRecordFirst = (m_nRecordFirst+1) % MAX_SCORE_HISTORY;
  }

  //ׯ������
  LONGLONG lBankerWinScore = 0;

  //��ҳɼ�
  ZeroMemory(m_lUserWinScore, sizeof(m_lUserWinScore));
  ZeroMemory(m_lUserReturnScore, sizeof(m_lUserReturnScore));
  ZeroMemory(m_lUserRevenue, sizeof(m_lUserRevenue));
  LONGLONG lUserLostScore[GAME_PLAYER];
  ZeroMemory(lUserLostScore, sizeof(lUserLostScore));

  //�����ע
  LONGLONG *pUserScore[AREA_COUNT+1];
  pUserScore[0]=NULL;
  for(int i = 1; i<AREA_COUNT+1; i++)
  {
    pUserScore[i]=m_lUserJettonScore[i];
  }

  //�������
  for(WORD wChairID=0; wChairID<GAME_PLAYER; wChairID++)
  {
    //ׯ���ж�
    if(m_wCurrentBanker==wChairID)
    {
      continue;
    }

    //��ȡ�û�
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
          //���Ϊ1���ٷ�
          m_lUserWinScore[wChairID] += 0;
          m_lUserReturnScore[wChairID] += pUserScore[wAreaIndex][wChairID] ;
        }
      }
    }

    //�ܵķ���
    m_lUserWinScore[wChairID] += lUserLostScore[wChairID];
  }

  //ׯ�ҳɼ�
  if(m_wCurrentBanker!=INVALID_CHAIR)
  {
    m_lUserWinScore[m_wCurrentBanker] = lBankerWinScore;
  }

  //����˰��
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

  //�ۼƻ���
  m_lBankerWinScore += lBankerWinScore;

  //��ǰ����
  m_lBankerCurGameScore=lBankerWinScore;

  return lBankerWinScore;
}

//��̽���ж�
bool CTableFrameSink::ProbeJudge()
{
  //ϵͳ��Ӯ
  LONGLONG lSystemScore = 0l;   //ϵͳʵ����Ӯ
  LONGLONG lAllScore = 0l;    //ϵͳȫ����Ӯ(�����˺����һ����)

  //����÷�
  JudgeSystemScore(lSystemScore,lAllScore);

  //ϵͳӮǮ
  if(lSystemScore>0)
  {
    m_lStorageCurrent += lSystemScore;

    //�ʽ��
    m_lBonus += lAllScore;
    if(NeedDeductStorage())
    {
      m_lStorageCurrent = m_lStorageCurrent - (m_lStorageCurrent * m_StorageDeduct / 1000);
    }

    //�ʽ�ر���ʵ���С
    if(m_lBonus <= m_lStorageCurrent)
    {
      m_lBonus = m_lStorageCurrent + rand() % 1000000;
    }

    return true;
  }
  else if(lSystemScore<0) //ϵͳ��Ǯ
  {
    //��治�����ؿ�
    if(m_lStorageCurrent + lSystemScore <= 0)
    {
      return false;
    }

    //��������⸶����
    LONGLONG lStorageCurrent = m_lStorageCurrent + lSystemScore;
    double llCompensateRadio = (double)((double)m_lCompensateRadio / (double)100);
    if(m_lStorageCurrent * (1 - llCompensateRadio) >= lStorageCurrent)
    {
      return false;
    }

    //�ж�����
    if(((m_lStorageCurrent > m_lStorageMax1) && (rand()%100 < m_lStorageMul1))
       || ((m_lStorageCurrent > m_lStorageMax2) && (rand()%100 < m_lStorageMul2))
       || rand() % 100 < 40)
    {
      //��������
      m_lStorageCurrent += lSystemScore;

      //�ʽ��
      m_lBonus += lAllScore;

      if(NeedDeductStorage() && m_lStorageCurrent > 0)
      {
        m_lStorageCurrent = m_lStorageCurrent - (m_lStorageCurrent * m_StorageDeduct / 1000);
      }

      //�ʽ�ر���ʵ���С
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
    //�ʽ��
    m_lBonus += lAllScore;

    //�ʽ�ر���ʵ���С
    if(m_lBonus <= m_lStorageCurrent)
    {
      m_lBonus = m_lStorageCurrent + rand() % 1000000;
    }

    return true;
  }
}

//�������
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

//��ȡ����
BYTE CTableFrameSink::GetAnimalIndex(BYTE cbAllIndex)
{

  static BYTE cbAnimalIndex[ANIMAL_COUNT]= {0,0,0,8,1,1,1,2,2,2,8,3,3,3,4,4,4,8,5,5,5,6,6,6,8,7,7,7};

  if(cbAllIndex>=1&&cbAllIndex<=28)
  {
    return cbAnimalIndex[cbAllIndex-1];
  }


  ////����
  //if(1==cbAllIndex||cbAllIndex==2||cbAllIndex==3)
  //{
  //  return 0;
  //}
  ////����
  //else if(5==cbAllIndex||cbAllIndex==6||cbAllIndex==7)
  //{
  //  return 1;

  //}
  ////����
  //else if(8==cbAllIndex||cbAllIndex==9||cbAllIndex==10)
  //{
  //  return 2;
  //}
  ////��ȸ
  //else if(12==cbAllIndex||cbAllIndex==13||cbAllIndex==14)
  //{
  //  return 3;
  //}
  ////��ӥ
  //else if(15==cbAllIndex||cbAllIndex==16||cbAllIndex==17)
  //{
  //  return 4;
  //}
  ////ʨ��
  //else if(19==cbAllIndex||cbAllIndex==20||cbAllIndex==21)
  //{
  //  return 5;
  //}
  ////��è
  //else if(22==cbAllIndex||cbAllIndex==23||cbAllIndex==24)
  //{
  //  return 6;
  //}
  ////����
  //else if(26==cbAllIndex||cbAllIndex==27||cbAllIndex==28)
  //{
  //  return 7;
  //}
  ////����
  //else
  //{
  //  return 8;
  //}
  return 0;
}


//���ͼ�¼
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

//������Ϣ
void CTableFrameSink::SendGameMessage(WORD wChairID, LPCTSTR pszTipMsg)
{
  if(wChairID==INVALID_CHAIR)
  {
    //��Ϸ���
    for(WORD i=0; i<GAME_PLAYER; ++i)
    {
      IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(i);
      if(pIServerUserItem==NULL)
      {
        continue;
      }
      m_pITableFrame->SendGameMessage(pIServerUserItem,pszTipMsg,SMT_CHAT);
    }

    //�Թ����
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
//���в���
#ifdef __SPECIAL___
bool  CTableFrameSink::OnActionUserBank(WORD wChairID, IServerUserItem * pIServerUserItem)
{
  return true;
}
#endif

//����
bool CTableFrameSink::OnAdminControl(CMD_C_ControlApplication* pData , IServerUserItem * pIServerUserItem)
{
  switch(pData->cbControlAppType)
  {
    case C_CA_UPDATE: //����
    {
      CMD_S_ControlReturns ControlReturns;
      ZeroMemory(&ControlReturns,sizeof(ControlReturns));
      ControlReturns.cbReturnsType = S_CR_UPDATE_SUCCES;
      ControlReturns.cbControlArea = m_cbControlArea;
      ControlReturns.cbControlTimes = m_cbControlTimes;
      m_pITableFrame->SendUserItemData(pIServerUserItem,SUB_S_ADMIN_COMMDN,&ControlReturns,sizeof(ControlReturns));
    }
    return true;
    case C_CA_SET:    //����
    {
      if(pData->cbControlArea == 0xff || pData->cbControlArea  > 7)
      {
        //����ʧ��
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
    case C_CA_CANCELS:  //ȡ��
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
  //����ʧ��
  CMD_S_ControlReturns ControlReturns;
  ZeroMemory(&ControlReturns,sizeof(ControlReturns));
  ControlReturns.cbReturnsType = S_CR_FAILURE;
  ControlReturns.cbControlArea = 0xff;
  m_pITableFrame->SendUserItemData(pIServerUserItem,SUB_S_ADMIN_COMMDN,&ControlReturns,sizeof(ControlReturns));
  return true;
}
//////////////////////////////////////////////////////////////////////////
//����ׯ��
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
      //��ׯ�ж�
      LONGLONG lUserScore=pIServerUserItem->GetUserScore();
      if(lUserScore<m_lApplyBankerCondition)
      {
        //������Ϣ
        CMD_S_CancelBanker CancelBanker;
        ZeroMemory(&CancelBanker,sizeof(CancelBanker));
        tagUserInfo * pUserInfo = pIServerUserItem->GetUserInfo();
        //���ñ���
        lstrcpyn(CancelBanker.szCancelUser,pUserInfo->szNickName,CountArray(CancelBanker.szCancelUser));

        //������Ϣ
        m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));
        m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));

        //��ʾ��Ϣ
        TCHAR szTipMsg[128];
        myprintf(szTipMsg,CountArray(szTipMsg),TEXT("������Ľ����������ׯ����������%I64d��,���Զ���ׯ��"),m_lApplyBankerCondition);
        SendGameMessage(m_ApplyUserArray[0],szTipMsg);

        //ɾ�����
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

//��ȡ����
void CTableFrameSink::ReadConfigInformation()
{
  //��ȡ�Զ�������
  tagCustomConfig *pCustomConfig = (tagCustomConfig *)m_pGameServiceOption->cbCustomRule;
  ASSERT(pCustomConfig);

  //��ׯ
  m_lApplyBankerCondition = pCustomConfig->CustomGeneral.lApplyBankerCondition;
  m_lBankerMAX = pCustomConfig->CustomGeneral.lBankerTime;
  m_lBankerAdd = pCustomConfig->CustomGeneral.lBankerTimeAdd;
  m_lBankerScoreMAX = pCustomConfig->CustomGeneral.lBankerScoreMAX;
  m_lBankerScoreAdd = pCustomConfig->CustomGeneral.lBankerTimeExtra;
  m_bEnableSysBanker = (pCustomConfig->CustomGeneral.nEnableSysBanker == TRUE)?true:false;

  //ʱ��
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

  //��ע
  m_lAreaLimitScore = pCustomConfig->CustomGeneral.lAreaLimitScore;
  m_lUserLimitScore = pCustomConfig->CustomGeneral.lUserLimitScore;


  //���
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

  //������
  m_nRobotListMaxCount = pCustomConfig->CustomAndroid.lRobotListMaxCount;

  LONGLONG lRobotBetMinCount = pCustomConfig->CustomAndroid.lRobotBetMinCount;
  LONGLONG lRobotBetMaxCount = pCustomConfig->CustomAndroid.lRobotBetMaxCount;
  m_nMaxChipRobot = rand()%(lRobotBetMaxCount-lRobotBetMinCount+1) + lRobotBetMinCount;
  if(m_nMaxChipRobot < 0)
  {
    m_nMaxChipRobot = 8;
  }
  m_lRobotAreaLimit = pCustomConfig->CustomAndroid.lRobotAreaLimit;


  //������
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

//����д��Ϣ
void CTableFrameSink::WriteStorageInfo(SCORE lStorage,bool bControl)
{
  //������������
  char* old_locale = _strdup(setlocale(LC_CTYPE,NULL));
  setlocale(LC_CTYPE, "chs");
  CString pszString;
  CTime time = CTime::GetCurrentTime();
  if(bControl)
  {
    pszString.Format(TEXT("\nʱ��: %d-%d-%d %d:%d:%d,����Ա�޸Ŀ��Ϊ: %I64d "),time.GetYear(), time.GetMonth(), time.GetDay(),
                     time.GetHour(), time.GetMinute(), time.GetSecond(),lStorage);
  }
  else
  {
    pszString.Format(TEXT("\nʱ��: %d-%d-%d %d:%d:%d,��ǰ���Ϊ: %I64d "),time.GetYear(), time.GetMonth(), time.GetDay(),
                     time.GetHour(), time.GetMinute(), time.GetSecond(),lStorage);
  }
  CStdioFile myFile;
  CString strFileName;
  strFileName.Format(TEXT("�������޿����־.txt"));
  BOOL bOpen = myFile.Open(strFileName, CFile::modeReadWrite|CFile::modeCreate|CFile::modeNoTruncate);
  if(bOpen)
  {
    myFile.SeekToEnd();
    myFile.WriteString(pszString);
    myFile.Flush();
    myFile.Close();
  }

  //��ԭ�����趨
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

  //���㱶��
  if(m_cbTableCardArray[0] == 4 && bcResulteOut[11] == 100)   //����
  {
    ::ZeroMemory(bcResulteOut,sizeof(bcResulteOut));
    bcResulteOut[ID_SHA_YU-1]=100;
  }
  else if(m_cbTableCardArray[0] == 11 && bcResulteOut[10] == 255)   //ͨɱ
  {
    ::ZeroMemory(bcResulteOut,sizeof(bcResulteOut));
  }
  else if(m_cbTableCardArray[0] == 25 && bcResulteOut[9] == 1)  //ͨ��
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
  else  //����
  {
    for(int i = 0; i<AREA_COUNT; i++)
    {
      bcResulteOut[i] += bcResultTwo[i];
    }

    //���� ���⽱��
    if(m_cbTableCardArray[0]%7==4)
    {
      bcResulteOut[ID_SHA_YU-1]+=m_cbShaYuAddMulti;
    }
  }
  //�����ע
  LONGLONG *pUserScore[AREA_COUNT+1];
  pUserScore[0] = NULL;
  for(int i = 1; i<AREA_COUNT+1; i++)
  {
    pUserScore[i] = m_lUserJettonScore[i];
  }

  //�������
  for(WORD wChairID=0; wChairID<GAME_PLAYER; wChairID++)
  {
    //��ȡ�û�
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
        //����
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
          //����
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
