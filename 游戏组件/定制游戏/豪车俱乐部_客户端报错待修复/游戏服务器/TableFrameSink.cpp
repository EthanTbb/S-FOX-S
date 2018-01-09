#include "StdAfx.h"
#include "TableFrameSink.h"
#include <locale>
#include "DlgCustomRule.h"
//////////////////////////////////////////////////////////////////////////

//��������
#define SEND_COUNT					300									//���ʹ���

//��������
#define INDEX_PLAYER				0									//�м�����
#define INDEX_BANKER				1									//ׯ������

//��עʱ��
#define IDI_FREE					1									//����ʱ��
#define TIME_FREE					5									//����ʱ��

//��עʱ��
#define IDI_PLACE_JETTON			2									//��עʱ��

#define TIME_PLACE_JETTON			15									//��עʱ��

//����ʱ��
#define IDI_GAME_END				3									//����ʱ��
#define TIME_GAME_END				20									//����ʱ��
#define TIME_GAME_ADD               10                                  //����ʱ��
//////////////////////////////////////////////////////////////////////////

//��̬����
const WORD			CTableFrameSink::m_wPlayerCount=GAME_PLAYER;				//��Ϸ����
//ConsoleWindow	    CTableFrameSink::m_DebugWindow;

//////////////////////////////////////////////////////////////////////////

//���캯��
CTableFrameSink::CTableFrameSink()
{
	//����ע��
	ZeroMemory(m_lAllJettonScore,sizeof(m_lAllJettonScore));

	//������ע
	ZeroMemory(m_lUserJettonScore,sizeof(m_lUserJettonScore));


	m_pServerControl=NULL;
	m_cbControlArea = 0xff;
	m_cbControlTimes = 0;

	m_lStorageMax1 = 0;
	m_lStorageMul1 = 0;
	m_lStorageMax2 = 0;
	m_lStorageMul2 = 0;
	//��ҳɼ�	
	ZeroMemory(m_lUserWinScore,sizeof(m_lUserWinScore));
	ZeroMemory(m_lUserReturnScore,sizeof(m_lUserReturnScore));
	ZeroMemory(m_lUserRevenue,sizeof(m_lUserRevenue));
	//��ʼ����
	ZeroMemory(m_lUserStartScore,sizeof(m_lUserStartScore));
	//�˿���Ϣ
	m_cbTableCard = 0;
	//״̬����
	m_dwJettonTime=0L;
      m_nRobotListMaxCount =0;
	//ׯ����Ϣ
	m_ApplyUserArray.RemoveAll();
	m_wCurrentBanker=INVALID_CHAIR;
	m_wBankerTime=0;
	m_lBankerWinScore=0L;		
	m_lBankerCurGameScore=0L;
	m_bEnableSysBanker = false;
		
	//��¼����
	ZeroMemory(m_GameRecordArrary,sizeof(m_GameRecordArrary));
	m_nRecordFirst=0;
	m_nRecordLast=0;

	//���Ʊ���
	m_lStorageCurrent = 0l;
	m_StorageDeduct = 0l;

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
	ZeroMemory(m_nAnimalTimes, sizeof(m_nAnimalTimes));

	srand(GetTickCount());

	return;
}

//��������
CTableFrameSink::~CTableFrameSink(void)
{
	if( m_pServerControl )
	{
		delete m_pServerControl;
		m_pServerControl = NULL;
	}

	if( m_hInst )
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
	if (m_pITableFrame==NULL) return false;

	//��ѯ����
	m_pGameServiceOption=m_pITableFrame->GetGameServiceOption();
	m_pGameServiceAttrib=m_pITableFrame->GetGameServiceAttrib();

	//��ʼģʽ
	//m_pITableFrame->SetStartMode(START_MODE_ALL_READY);

	m_pITableFrame->SetStartMode(START_MODE_TIME_CONTROL);
      ReadConfigInformation();
	//�������
	m_hInst = NULL;
	m_pServerControl = NULL;
	m_hInst = LoadLibrary(TEXT("LuxuryCarServerControl.dll"));
	if ( m_hInst )
	{
		typedef void * (*CREATE)(); 
		CREATE ServerControl = (CREATE)GetProcAddress(m_hInst,"CreateServerControl"); 
		if ( ServerControl )
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

	//��ҳɼ�	
	ZeroMemory(m_lUserWinScore,sizeof(m_lUserWinScore));
	ZeroMemory(m_lUserReturnScore,sizeof(m_lUserReturnScore));
	ZeroMemory(m_lUserRevenue,sizeof(m_lUserRevenue));

	//�����˿���
	m_nChipRobotCount = 0;
	ZeroMemory(m_lRobotAreaScore, sizeof(m_lRobotAreaScore));
	//m_cbTableCard = 0;
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
	for (int nAreaIndex=1; nAreaIndex<=AREA_COUNT; ++nAreaIndex) 
	{
		if(m_lUserJettonScore[nAreaIndex][wChairID]>0)
			return true;
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
	if (INVALID_CHAIR!=m_wCurrentBanker) pIBankerServerUserItem=m_pITableFrame->GetTableUserItem(m_wCurrentBanker);

	//���ñ���
	GameStart.cbTimeLeave=m_cbBetTime;
	GameStart.wBankerUser=m_wCurrentBanker;
	GameStart.lBankerScore = 0x7fffffffffffffff;
	if (pIBankerServerUserItem!=NULL) 
		GameStart.lBankerScore=pIBankerServerUserItem->GetUserScore()*m_nJettonMultiple;

	//��ע����������
	int nChipRobotCount = 0;
	for (int i = 0; i < GAME_PLAYER; i++) 
	{
		IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(i);
		if (pIServerUserItem != NULL && pIServerUserItem->IsAndroidUser())
			nChipRobotCount++;
	}
	GameStart.nChipRobotCount = min(nChipRobotCount, m_nMaxChipRobot);

	nChipRobotCount = 0;
	for (int i = 0; i < m_ApplyUserArray.GetCount(); i++) 
	{
		IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(m_ApplyUserArray.GetAt(i));
		if (pIServerUserItem != NULL && pIServerUserItem->IsAndroidUser())
			nChipRobotCount++;
	}

	if(nChipRobotCount > 0)
		GameStart.nAndriodCount=nChipRobotCount-1;		
	
	//	��¼�����Ϣ
	CTime time = CTime::GetCurrentTime();
	CString strStorage;
	strStorage.Format(TEXT(" ����: %s | ʱ��: %d-%d-%d %d:%d:%d | ���: %I64d \n"),m_pGameServiceOption->szServerName,time.GetYear(), time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond(), m_lStorageCurrent);
	WriteInfo(strStorage);

	//�����˿���
	m_nChipRobotCount = 0;
	
    //�Թ����
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_START,&GameStart,sizeof(GameStart));	

	//��Ϸ���
	for (WORD wChairID=0; wChairID<GAME_PLAYER; ++wChairID)
	{
		IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(wChairID);
		if (pIServerUserItem==NULL) continue;

		if (m_bEnableSysBanker==false && m_wCurrentBanker==INVALID_CHAIR)
		{
			GameStart.lBankerScore = 1;
		}

		//���û���
		GameStart.lUserMaxScore=min(pIServerUserItem->GetUserScore()*m_nJettonMultiple,m_lUserLimitScore);

		m_pITableFrame->SendTableData(wChairID,SUB_S_GAME_START,&GameStart,sizeof(GameStart));	
	}

	return true;
}

//��Ϸ����
bool  CTableFrameSink::OnEventGameConclude(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)
{
	switch (cbReason)
	{
	case GER_NORMAL:		//�������	
		{
			if ( m_pServerControl != NULL && m_pServerControl->NeedControl() )
			{
				tagControlInfo ControlInfo;
				m_pServerControl->ReturnControlArea(ControlInfo,m_lAllJettonScore);
				m_cbTableCard = ControlInfo.cbControlArea;
				m_pServerControl->CompleteControl();
				//ϵͳ��Ӯ
				LONGLONG lSystemScore = 0l;

				JudgeSystemScore(lSystemScore);
				m_lStorageCurrent += lSystemScore;
				if(NeedDeductStorage())
					m_lStorageCurrent = m_lStorageCurrent - (m_lStorageCurrent * m_StorageDeduct / 1000);
			}
			else
			{
				//��������
				static BYTE cbCategoryAreaIndex[8]={8,2,1,5,13,9,4,3};						//������ת�̵�һ��������
				static BYTE	cbCarAreaIndex[4][3]={{8,11,14},{2,12,16},{4,7,10},{3,6,15}};	//�屶�����е�����
				//��������
				CWHArray<BYTE> DrawItemList,BigDrawItemList;
				CWHArray<BYTE> DrawItemWinList,DrawItemLostList;

				LONGLONG lSystemScore = 0l;		

				//����ɿ�����
				for (INT i=0; i<AREA_ALL; i++)
				{
					lSystemScore = 0l;	
					m_cbTableCard = cbCategoryAreaIndex[i];
					JudgeSystemScore(lSystemScore);
					//lSystemScore>0 ��ֹû�п�������
					if(lSystemScore+m_lStorageCurrent>0 || lSystemScore>=0) 
					{
						if(i<2||i>6)DrawItemList.Add(m_cbTableCard);
						else BigDrawItemList.Add(m_cbTableCard);

						if(lSystemScore>0) DrawItemWinList.Add(m_cbTableCard);
						else DrawItemLostList.Add(m_cbTableCard);
					}
				}
				ASSERT(DrawItemList.GetCount()+BigDrawItemList.GetCount()>0);
				ASSERT(DrawItemWinList.GetCount()+DrawItemLostList.GetCount()>0);

				if( (DrawItemList.GetCount()+BigDrawItemList.GetCount())==0
					||(DrawItemWinList.GetCount()+DrawItemLostList.GetCount())==0)
				{
					
					CTraceService::TraceString(TEXT("û�пɿ�������!"),TraceLevel_Warning);
					return true;
				}

				//���ÿ�������
				m_cbTableCard=0;
				bool bSystemLost = false;
				if ( m_lStorageCurrent > m_lStorageMax2)	bSystemLost= (rand()%100 < m_lStorageMul2);
				else if(m_lStorageCurrent>m_lStorageMax1)	bSystemLost= (rand()%100 < m_lStorageMul1);
				if(bSystemLost && DrawItemLostList.GetCount()>0)
				{
					m_cbTableCard = DrawItemLostList.GetAt(rand()%DrawItemLostList.GetCount());
				}
				else
				{
					if(DrawItemList.GetCount()==0)
					{
						ASSERT(BigDrawItemList.GetCount()!=0);

						m_cbTableCard = BigDrawItemList.GetAt(rand()%BigDrawItemList.GetCount());
					}
					else if(BigDrawItemList.GetCount()==0)
					{

						ASSERT(DrawItemList.GetCount()==0);
						BYTE cbIndex = DrawItemList.GetAt(rand()%DrawItemList.GetCount());
						for(BYTE cb=0;cb<4;cb++)
						{
							if(cbCarAreaIndex[cb][0]==cbIndex)
							{
								m_cbTableCard=cbCarAreaIndex[cb][rand()%3];
							}
						}
					}
					else
					{
						if(rand()%100<80)
						{
							BYTE cbIndex = DrawItemList.GetAt(rand()%DrawItemList.GetCount());
							for(BYTE cb=0;cb<4;cb++)
							{
								if(cbCarAreaIndex[cb][0]==cbIndex)
								{
									m_cbTableCard=cbCarAreaIndex[cb][rand()%3];
								}
							}
						}
						else
						{
							m_cbTableCard = BigDrawItemList.GetAt(rand()%BigDrawItemList.GetCount());
						}
					}
				}


				ASSERT(m_cbTableCard!=0);
				lSystemScore = 0l;
				JudgeSystemScore(lSystemScore);
				m_lStorageCurrent += lSystemScore;
				if(NeedDeductStorage())
					m_lStorageCurrent = m_lStorageCurrent - (m_lStorageCurrent * m_StorageDeduct / 1000);

				
// 				bool bSystemLost = false;
// 
// 				while(true)
// 				{
// 					if ( m_lStorageCurrent > m_lStorageMax2)
// 					{
// 						bSystemLost= (rand()%100 < m_lStorageMul2);
// 					}
// 					else if(m_lStorageCurrent>m_lStorageMax1)
// 					{
// 						bSystemLost= (rand()%100 < m_lStorageMul1);
// 					}
// 					//�ɷ��˿�
// 					m_cbTableCard = 0;
// 					DispatchTableCard(bSystemLost);
// 					//��̽���ж�
// 					if (ProbeJudge(bSystemLost))
// 					{
// 						break;
// 					}
// 				}
			}
			
			//�������
			LONGLONG lBankerWinScore=CalculateScore();
					
			//��������
			m_wBankerTime++;

			//������Ϣ
			CMD_S_GameEnd GameEnd;
			ZeroMemory(&GameEnd,sizeof(GameEnd));

			//ׯ����Ϣ
			GameEnd.nBankerTime = m_wBankerTime;
			GameEnd.lBankerTotallScore=m_lBankerWinScore;
			GameEnd.lBankerScore=lBankerWinScore;
							
			//�˿���Ϣ
			GameEnd.cbTableCard = m_cbTableCard;
			
			//���ͻ���
			GameEnd.cbTimeLeave=m_cbEndTime;	

			for ( WORD wUserIndex = 0; wUserIndex < GAME_PLAYER; ++wUserIndex )
			{
				IServerUserItem *pIServerUserItem = m_pITableFrame->GetTableUserItem(wUserIndex);
				if ( pIServerUserItem == NULL ) continue;

				//���óɼ�
				GameEnd.lUserScore=m_lUserWinScore[wUserIndex];

				//��������
				GameEnd.lUserReturnScore=m_lUserReturnScore[wUserIndex];

				//����˰��
				if (m_lUserRevenue[wUserIndex]>0) GameEnd.lRevenue=m_lUserRevenue[wUserIndex];
				else if (m_wCurrentBanker!=INVALID_CHAIR) GameEnd.lRevenue=m_lUserRevenue[m_wCurrentBanker];
				else GameEnd.lRevenue=0;

				//������Ϣ					
				m_pITableFrame->SendTableData(wUserIndex,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
				m_pITableFrame->SendLookonData(wUserIndex,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
			}

			return true;
		}
	case GER_USER_LEAVE:		//�û��뿪
	case GER_NETWORK_ERROR:
		{
			//�м��ж�
			if (m_wCurrentBanker!=wChairID)
			{
				//��������
				LONGLONG lScore=0;
				LONGLONG lRevenue=0;
				
				//ͳ�Ƴɼ�
				for (int nAreaIndex=1; nAreaIndex<=AREA_COUNT; ++nAreaIndex) lScore -= m_lUserJettonScore[nAreaIndex][wChairID];

				//д�����
				if (m_pITableFrame->GetGameStatus() != GS_GAME_END)
				{
					for (int nAreaIndex=1; nAreaIndex<=AREA_COUNT; ++nAreaIndex)
					{
						if (m_lUserJettonScore[nAreaIndex][wChairID] != 0)
						{
							CMD_S_PlaceJettonFail PlaceJettonFail;
							ZeroMemory(&PlaceJettonFail,sizeof(PlaceJettonFail));
							//PlaceJettonFail.lJettonArea=nAreaIndex;
							//PlaceJettonFail.lPlaceScore=m_lUserJettonScore[nAreaIndex][wChairID];
							//PlaceJettonFail.wPlaceUser=wChairID;

							//��Ϸ���
							for (WORD i=0; i<GAME_PLAYER; ++i)
							{
								IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(i);
								if (pIServerUserItem==NULL) 
									continue;

								//m_pITableFrame->SendTableData(i,SUB_S_PLACE_JETTON_FAIL,&PlaceJettonFail,sizeof(PlaceJettonFail));
							}

							m_lAllJettonScore[nAreaIndex] -= m_lUserJettonScore[nAreaIndex][wChairID];
							m_lUserJettonScore[nAreaIndex][wChairID] = 0;
						}
					}
				}
				else
				{

					for (int nAreaIndex=1; nAreaIndex<=AREA_COUNT; ++nAreaIndex)
					{
						if (m_lUserJettonScore[nAreaIndex][wChairID] != 0)
						{
							m_lUserJettonScore[nAreaIndex][wChairID] = 0;
						}
					}

					
					//д�����
					if (m_lUserWinScore[wChairID]!=0L) 
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
				return true;
			}

			//״̬�ж�
			if (m_pITableFrame->GetGameStatus()!=GS_GAME_END)
			{
				//��ʾ��Ϣ
				TCHAR szTipMsg[128];

				tagUserInfo * pUserInfo = pIServerUserItem->GetUserInfo();
				myprintf(szTipMsg,CountArray(szTipMsg),TEXT("����ׯ��[ %s ]ǿ�ˣ���Ϸ��ǰ������"),pUserInfo->szNickName);

				//������Ϣ
				SendGameMessage(INVALID_CHAIR,szTipMsg);	

				//����״̬
				m_pITableFrame->SetGameStatus(GS_GAME_END);

				//����ʱ��
				m_pITableFrame->KillGameTimer(IDI_PLACE_JETTON);
				m_dwJettonTime=(DWORD)time(NULL);
				m_pITableFrame->SetGameTimer(IDI_GAME_END,m_cbEndTime*1000,1,0L);

				//����
				if ( m_cbControlArea != 0xff && m_cbControlTimes > 0)
				{
					BYTE cbControlArea[8][4] = { 1, 9, 17, 25, 
												3, 11, 19, 27,
												5, 13, 21, 30,
												7, 15, 23, 32,
												2, 10, 18, 26,
												4, 12, 20, 28,
												6, 14, 22, 31,
												8, 16, 24, 0 };
					m_cbTableCard = cbControlArea[m_cbControlArea][rand()%4];
					m_cbControlTimes--;
					if(m_cbControlTimes == 0)
					{
						m_cbControlArea = 0xff;
					}
					//ϵͳ��Ӯ
					LONGLONG lSystemScore = 0l;

					JudgeSystemScore(lSystemScore);
					m_lStorageCurrent += lSystemScore;
					if(NeedDeductStorage())
						m_lStorageCurrent = m_lStorageCurrent - (m_lStorageCurrent * m_StorageDeduct / 1000);
				}
				else
				{
					bool bSystemLost = false;
					if ( m_lStorageCurrent > m_lStorageMax2)
					{
						bSystemLost= (rand()%100 < m_lStorageMul2);
					}
					else
					{
						bSystemLost= (rand()%100 < m_lStorageMul1);
					}

					while(true)
					{
						//�ɷ��˿�
						m_cbTableCard = 0;
						DispatchTableCard(bSystemLost);
						//��̽���ж�
						if (ProbeJudge(bSystemLost))
						{
							break;
						}
					}
				}

				//�������
				CalculateScore();

				//������Ϣ
				CMD_S_GameEnd GameEnd;
				ZeroMemory(&GameEnd,sizeof(GameEnd));

				//ׯ����Ϣ
				GameEnd.nBankerTime = m_wBankerTime;
				GameEnd.lBankerTotallScore=m_lBankerWinScore;
				
				if (m_lBankerWinScore>0) GameEnd.lBankerScore=0;
			
				//�˿���Ϣ
				GameEnd.cbTableCard = m_cbTableCard;
				
				//���ͻ���
				GameEnd.cbTimeLeave=m_cbEndTime;	
				for ( WORD wUserIndex = 0; wUserIndex < GAME_PLAYER; ++wUserIndex )
				{
					IServerUserItem *pIServerUserItem = m_pITableFrame->GetTableUserItem(wUserIndex);
					if ( pIServerUserItem == NULL ) continue;

					//���óɼ�
					GameEnd.lUserScore=m_lUserWinScore[wUserIndex];

					//��������
					GameEnd.lUserReturnScore=m_lUserReturnScore[wUserIndex];

					//����˰��
					if (m_lUserRevenue[wUserIndex]>0) GameEnd.lRevenue=m_lUserRevenue[wUserIndex];
					else if (m_wCurrentBanker!=INVALID_CHAIR) GameEnd.lRevenue=m_lUserRevenue[m_wCurrentBanker];
					else GameEnd.lRevenue=0;

					//������Ϣ					
					m_pITableFrame->SendTableData(wUserIndex,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
					m_pITableFrame->SendLookonData(wUserIndex,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
				}
			}

			//�۳�����
			if (m_lUserWinScore[m_wCurrentBanker] != 0l)
			{
				tagScoreInfo ScoreInfo[GAME_PLAYER];
				ZeroMemory(ScoreInfo,sizeof(ScoreInfo));
				ScoreInfo[m_wCurrentBanker].cbType=(m_lUserWinScore[m_wCurrentBanker]>0L)?SCORE_TYPE_WIN:SCORE_TYPE_LOSE;
				ScoreInfo[m_wCurrentBanker].lRevenue=m_lUserRevenue[m_wCurrentBanker];
				ScoreInfo[m_wCurrentBanker].lScore=m_lUserWinScore[m_wCurrentBanker];
				m_pITableFrame->WriteTableScore(ScoreInfo,CountArray(ScoreInfo));
				m_lUserWinScore[m_wCurrentBanker] = 0;
			}

			//�л�ׯ��
			ChangeBanker(true);

			return true;
		}
	}

	return false;
}

//���ͳ���
bool  CTableFrameSink::OnEventSendGameScene(WORD wChiarID, IServerUserItem * pIServerUserItem, BYTE cbGameStatus, bool bSendSecret)
{
	switch (cbGameStatus)
	{
	case GAME_STATUS_FREE:			//����״̬
		{
			//���ͼ�¼
			SendGameRecord(pIServerUserItem);

			//��������
			CMD_S_StatusFree StatusFree;
			ZeroMemory(&StatusFree,sizeof(StatusFree));			

			//������Ϣ
			StatusFree.lApplyBankerCondition = m_lApplyBankerCondition;
			StatusFree.lAreaLimitScore = m_lAreaLimitScore;
			//StatusFree.CheckImage = m_CheckImage;
			StatusFree.bGenreEducate =  m_pGameServiceOption->wServerType&GAME_GENRE_EDUCATE;
			//ׯ����Ϣ
			StatusFree.bEnableSysBanker=m_bEnableSysBanker;
			StatusFree.wBankerUser=m_wCurrentBanker;	
			StatusFree.cbBankerTime=m_wBankerTime;
			StatusFree.lBankerWinScore=m_lBankerWinScore;
			StatusFree.lBankerScore = 0x7fffffffffffffff;
			StatusFree.nMultiple = m_nJettonMultiple;
			if (m_wCurrentBanker!=INVALID_CHAIR)
			{
				IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(m_wCurrentBanker);
				StatusFree.lBankerScore=pIServerUserItem->GetUserScore()*m_nJettonMultiple;
			}

			//�����Ϣ
			if (pIServerUserItem->GetUserStatus()!=US_LOOKON)
			{
				StatusFree.lUserMaxScore=min(pIServerUserItem->GetUserScore()*m_nJettonMultiple,m_lUserLimitScore*4); 
			}

			//ȫ����Ϣ
			DWORD dwPassTime=(DWORD)time(NULL)-m_dwJettonTime;
			StatusFree.cbTimeLeave=(BYTE)(m_cbFreeTime-__min(dwPassTime,m_cbFreeTime));

			//��������
			CopyMemory(StatusFree.szGameRoomName, m_pGameServiceOption->szServerName, sizeof(StatusFree.szGameRoomName));
			
			//����ȫ��
			{
				TCHAR szAllName[256]=TEXT("");
				myprintf(szAllName,CountArray(szAllName),TEXT("%s > %s > ��%d��"),m_pGameServiceAttrib->szGameName,m_pGameServiceOption->szServerName,m_pITableFrame->GetTableID()+1);
				memcpy(StatusFree.szRoomTotalName, szAllName, sizeof(szAllName));
			}

			//����������
			if(pIServerUserItem->IsAndroidUser())
			{
				tagCustomConfig *pCustomConfig = (tagCustomConfig *)m_pITableFrame->GetCustomRule();				
				ASSERT(pCustomConfig);

				CopyMemory(&StatusFree.CustomAndroid, &pCustomConfig->CustomAndroid, sizeof(tagCustomAndroid));
			}
			CopyMemory(&StatusFree.lUserStartScore, m_lUserStartScore, sizeof(m_lUserStartScore));

			//���ͳ���
			bool bSuccess = m_pITableFrame->SendGameScene(pIServerUserItem,&StatusFree,sizeof(StatusFree));
			
			//������ʾ
			TCHAR szTipMsg[128];
			_sntprintf(szTipMsg,CountArray(szTipMsg),_T("��������ׯ����Ϊ:%I64d,��������Ϊ:%I64d,�������Ϊ:%I64d"),m_lApplyBankerCondition,m_lAreaLimitScore,m_lUserLimitScore);
			m_pITableFrame->SendGameMessage(pIServerUserItem,szTipMsg,SMT_CHAT);	

			//����������
			SendApplyUser(pIServerUserItem);
			//���¿����Ϣ
			if(CUserRight::IsGameCheatUser(pIServerUserItem->GetUserRight()))
			{
				CMD_S_UpdateStorage updateStorage;
				ZeroMemory(&updateStorage, sizeof(updateStorage));

				updateStorage.cbReqType = RQ_REFRESH_STORAGE;
				updateStorage.lStorageStart = m_StorageStart;
				updateStorage.lStorageDeduct = m_StorageDeduct;
				updateStorage.lStorageCurrent = m_lStorageCurrent;
				updateStorage.lStorageMax1 = m_lStorageMax1;
				updateStorage.lStorageMul1 = m_lStorageMul1;
				updateStorage.lStorageMax2 = m_lStorageMax2;
				updateStorage.lStorageMul2 = m_lStorageMul2;

				m_pITableFrame->SendUserItemData(pIServerUserItem,SUB_S_UPDATE_STORAGE,&updateStorage,sizeof(updateStorage));
			}
			return bSuccess;
		}
	case GS_PLACE_JETTON:		//��Ϸ״̬
	case GS_GAME_END:			//����״̬
		{
			//���ͼ�¼
			SendGameRecord(pIServerUserItem);		

			//��������
			CMD_S_StatusPlay StatusPlay={0};

			//ȫ����ע
			CopyMemory(StatusPlay.lAllJettonScore,m_lAllJettonScore,sizeof(StatusPlay.lAllJettonScore));

			//�����ע
			if (pIServerUserItem->GetUserStatus()!=US_LOOKON)
			{
				for (int nAreaIndex=1; nAreaIndex<=AREA_COUNT; ++nAreaIndex)
					StatusPlay.lUserJettonScore[nAreaIndex] = m_lUserJettonScore[nAreaIndex][wChiarID];

				//�����ע
				StatusPlay.lUserMaxScore=min(pIServerUserItem->GetUserScore()*m_nJettonMultiple,m_lUserLimitScore);
			}

			//������Ϣ
			StatusPlay.lApplyBankerCondition=m_lApplyBankerCondition;		
			StatusPlay.lAreaLimitScore=m_lAreaLimitScore;		
			StatusPlay.bGenreEducate =  m_pGameServiceOption->wServerType&GAME_GENRE_EDUCATE;
			//ׯ����Ϣ
			StatusPlay.bEnableSysBanker=m_bEnableSysBanker;
			StatusPlay.wBankerUser=m_wCurrentBanker;			
			StatusPlay.cbBankerTime=m_wBankerTime;
			StatusPlay.lBankerWinScore=m_lBankerWinScore;	
			StatusPlay.lBankerScore = 0x7fffffffffffffff;
			StatusPlay.nMultiple = m_nJettonMultiple;

			
			if (m_wCurrentBanker!=INVALID_CHAIR)
			{
				IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(m_wCurrentBanker);
				StatusPlay.lBankerScore=pIServerUserItem->GetUserScore()*m_nJettonMultiple;
			}	

			//ȫ����Ϣ
			DWORD dwPassTime=(DWORD)time(NULL)-m_dwJettonTime;
			StatusPlay.cbTimeLeave=(BYTE)(m_cbBetTime-__min(dwPassTime,m_cbBetTime));
			StatusPlay.cbGameStatus=m_pITableFrame->GetGameStatus();			

			//�����ж�
			if (cbGameStatus==GS_GAME_END)
			{
				
				BYTE cbEndTime=0;
				
				cbEndTime=m_cbEndTime;
				
				StatusPlay.cbTimeLeave=(BYTE)(cbEndTime-__min(dwPassTime,cbEndTime));

				//���óɼ�
				StatusPlay.lEndUserScore=m_lUserWinScore[wChiarID];

				//��������
				StatusPlay.lEndUserReturnScore=m_lUserReturnScore[wChiarID];

				//����˰��
				if (m_lUserRevenue[wChiarID]>0) StatusPlay.lEndRevenue=m_lUserRevenue[wChiarID];
				else if (m_wCurrentBanker!=INVALID_CHAIR) StatusPlay.lEndRevenue=m_lUserRevenue[m_wCurrentBanker];
				else StatusPlay.lEndRevenue=0;

				//ׯ�ҳɼ�
				StatusPlay.lEndBankerScore=m_lBankerCurGameScore;

				//�˿���Ϣ
				StatusPlay.cbTableCard = m_cbTableCard;
			}

			//��������
			CopyMemory(StatusPlay.szGameRoomName, m_pGameServiceOption->szServerName, sizeof(StatusPlay.szGameRoomName));

			//����ȫ��
			{
				TCHAR szAllName[256]=TEXT("");
				myprintf(szAllName,CountArray(szAllName),TEXT("%s > %s > ��%d��"),m_pGameServiceAttrib->szGameName,m_pGameServiceOption->szServerName,m_pITableFrame->GetTableID()+1);
				memcpy(StatusPlay.szRoomTotalName, szAllName, sizeof(szAllName));
			}

			//����������
			if(pIServerUserItem->IsAndroidUser())
			{
				tagCustomConfig *pCustomConfig = (tagCustomConfig *)m_pITableFrame->GetCustomRule();				
				ASSERT(pCustomConfig);

				CopyMemory(&StatusPlay.CustomAndroid, &pCustomConfig->CustomAndroid, sizeof(tagCustomAndroid));
			}

			CopyMemory(&StatusPlay.lUserStartScore, m_lUserStartScore, sizeof(m_lUserStartScore));

			//���ͳ���
			bool bSuccess = m_pITableFrame->SendGameScene(pIServerUserItem,&StatusPlay,sizeof(StatusPlay));

			////������ʾ
			//TCHAR szTipMsg[128];
			//myprintf(szTipMsg,CountArray(szTipMsg),TEXT("��������ׯ����Ϊ��%I64d,��������Ϊ��%I64d,�������Ϊ��%I64d"),m_lApplyBankerCondition,
			//	m_lAreaLimitScore,m_lUserLimitScore);
			//
			//m_pITableFrame->SendGameMessage(pIServerUserItem,szTipMsg,SMT_CHAT);
			
			//������ʾ
			TCHAR szTipMsg[128];
			_sntprintf(szTipMsg,CountArray(szTipMsg),_T("��������ׯ����Ϊ:%I64d,��������Ϊ:%I64d,�������Ϊ:%I64d"),m_lApplyBankerCondition,m_lAreaLimitScore,m_lUserLimitScore);
			m_pITableFrame->SendGameMessage(pIServerUserItem,szTipMsg,SMT_CHAT);	

			//����������
			SendApplyUser( pIServerUserItem );
			//���¿����Ϣ
			if(CUserRight::IsGameCheatUser(pIServerUserItem->GetUserRight()))
			{
				CMD_S_UpdateStorage updateStorage;
				ZeroMemory(&updateStorage, sizeof(updateStorage));

				updateStorage.cbReqType = RQ_REFRESH_STORAGE;
				updateStorage.lStorageStart = m_StorageStart;
				updateStorage.lStorageDeduct = m_StorageDeduct;
				updateStorage.lStorageCurrent = m_lStorageCurrent;
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
	switch (wTimerID)
	{
	case IDI_FREE:		//����ʱ��
		{
			//������ׯ
			if (m_bEnableSysBanker==false && m_wCurrentBanker==INVALID_CHAIR)
			{
				//����״̬
				//m_pITableFrame->SetGameStatus(GAME_STATUS_FREE);
				//m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_WAIT_BANKER);

				//����ʱ��
				//m_dwJettonTime=(DWORD)time(NULL);
				//m_pITableFrame->SetGameTimer(IDI_FREE,m_cbFreeTime*1000,1,0L);

				//������Ϣ
				//CMD_S_GameFree GameFree;
				//ZeroMemory(&GameFree,sizeof(GameFree));

				//GameFree.cbTimeLeave = m_cbFreeTime;
				//m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_FREE,&GameFree,sizeof(GameFree));
				//m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_FREE,&GameFree,sizeof(GameFree));

				//return true;
			}
			
			//��ʼ��Ϸ
			m_pITableFrame->StartGame();

			//����ʱ��
			m_dwJettonTime=(DWORD)time(NULL);
			m_pITableFrame->SetGameTimer(IDI_PLACE_JETTON,m_cbBetTime*1000,1,0L);

			//����״̬
			m_pITableFrame->SetGameStatus(GS_PLACE_JETTON);

			return true;
		}
	case IDI_PLACE_JETTON:		//��עʱ��
		{
			//״̬�ж�(��ֹǿ���ظ�����)
			if (m_pITableFrame->GetGameStatus()!=GS_GAME_END)
			{
				//����״̬
				m_pITableFrame->SetGameStatus(GS_GAME_END);			

				//������Ϸ
				OnEventGameConclude(INVALID_CHAIR,NULL,GER_NORMAL);

				//����ʱ��
				m_dwJettonTime=(DWORD)time(NULL);
				m_pITableFrame->SetGameTimer(IDI_GAME_END,m_cbEndTime*1000,1,0L);			
			}

			return true;
		}
	case IDI_GAME_END:			//������Ϸ
		{
			tagScoreInfo ScoreInfo;

			//д�����
			for ( WORD wUserChairID = 0; wUserChairID < GAME_PLAYER; ++wUserChairID )
			{
				IServerUserItem *pIServerUserItem = m_pITableFrame->GetTableUserItem(wUserChairID);
				if ( pIServerUserItem == NULL ) continue;
				ZeroMemory(&ScoreInfo,sizeof(ScoreInfo));

				if(m_lUserWinScore[wUserChairID] != 0)
				{
					ScoreInfo.cbType=(m_lUserWinScore[wUserChairID]>0L)?SCORE_TYPE_WIN:SCORE_TYPE_LOSE;
					ScoreInfo.lRevenue=m_lUserRevenue[wUserChairID];
					ScoreInfo.lScore=m_lUserWinScore[wUserChairID];
					
					m_pITableFrame->WriteUserScore(wUserChairID,ScoreInfo);
				}

			}
			//������Ϸ
			m_pITableFrame->ConcludeGame(GAME_STATUS_FREE);

			//�л�ׯ��
			ChangeBanker(false);

			RobotBankerControl();

			//����ʱ��
			m_dwJettonTime=(DWORD)time(NULL);
			m_pITableFrame->SetGameTimer(IDI_FREE,m_cbFreeTime*1000,1,0L);

			//������Ϣ
			CMD_S_GameFree GameFree;
			ZeroMemory(&GameFree,sizeof(GameFree));

			GameFree.cbTimeLeave = m_cbFreeTime;
			GameFree.lStorageCurrent=m_lStorageCurrent;
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
	
	switch (wSubCmdID)
	{
	case SUB_C_PLACE_JETTON:		//�û���ע
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_C_PlaceJetton));
			if (wDataSize!=sizeof(CMD_C_PlaceJetton)) return false;

			//�û�Ч��
			
			tagUserInfo * pUserData=pIServerUserItem->GetUserInfo();

			if (pUserData->cbUserStatus!=US_PLAYING) return true;

			//��Ϣ����
			CMD_C_PlaceJetton * pPlaceJetton=(CMD_C_PlaceJetton *)pDataBuffer;
			//�����ע��ʵ��ֵҪ���Զһ�����
			return OnUserPlaceJetton(pUserData->wChairID,pPlaceJetton->cbJettonArea,pPlaceJetton->lJettonScore/m_nJettonMultiple);
		}
	case SUB_C_APPLY_BANKER:		//������ׯ
		{
			//�û�Ч��
			tagUserInfo * pUserData=pIServerUserItem->GetUserInfo();
			if (pUserData->cbUserStatus==US_LOOKON) return true;

			return OnUserApplyBanker(pIServerUserItem);	
		}
	case SUB_C_CANCEL_BANKER:		//ȡ����ׯ
		{
			//�û�Ч��
			tagUserInfo * pUserData=pIServerUserItem->GetUserInfo();
			if (pUserData->cbUserStatus==US_LOOKON) return true;

			return OnUserCancelBanker(pIServerUserItem);	
		}
	case SUB_C_UPDATE_STORAGE:		//���¿��
		{
			ASSERT(wDataSize==sizeof(CMD_C_UpdateStorage));
			if(wDataSize!=sizeof(CMD_C_UpdateStorage)) return false;

			//Ȩ���ж�
			if(CUserRight::IsGameCheatUser(pIServerUserItem->GetUserRight())==false)
				return false;

			//��Ϣ����
			CMD_C_UpdateStorage * pUpdateStorage=(CMD_C_UpdateStorage *)pDataBuffer;
			if (pUpdateStorage->cbReqType==RQ_SET_STORAGE)
			{
				m_StorageDeduct = pUpdateStorage->lStorageDeduct;
				m_lStorageCurrent = pUpdateStorage->lStorageCurrent;
				m_lStorageMax1 = pUpdateStorage->lStorageMax1;
				m_lStorageMul1 = pUpdateStorage->lStorageMul1;
				m_lStorageMax2 = pUpdateStorage->lStorageMax2;
				m_lStorageMul2 = pUpdateStorage->lStorageMul2;

				//��¼��Ϣ
				CString strControlInfo, str;
				str.Format(TEXT("%s"), TEXT("�޸Ŀ�����ã�"));
				CTime Time(CTime::GetCurrentTime());
				strControlInfo.Format(TEXT("����: %s | ����: %u | ʱ��: %d-%d-%d %d:%d:%d\n�������˺�: %s | ������ID: %u\n%s\r\n"),
					m_pGameServiceOption->szServerName, m_pITableFrame->GetTableID()+1, Time.GetYear(), Time.GetMonth(), Time.GetDay(),
					Time.GetHour(), Time.GetMinute(), Time.GetSecond(), pIServerUserItem->GetNickName(), pIServerUserItem->GetGameID(), str);

				WriteInfo(strControlInfo);
			}
			return TRUE;
		}		
	case SUB_C_ADMIN_COMMDN:
		{
			ASSERT(wDataSize==sizeof(CMD_C_ControlApplication));
			if(wDataSize!=sizeof(CMD_C_ControlApplication))return false;
			if ( m_pServerControl == NULL) return true;

			if( !CUserRight::IsGameCheatUser(pIServerUserItem->GetUserRight()) ) return false;

			//���ÿ��
			const CMD_C_ControlApplication* AdminReq=static_cast<const CMD_C_ControlApplication*>(pDataBuffer);
			if(AdminReq->cbControlAppType==C_CA_SET_STORAGE)
			{
				m_lStorageCurrent=AdminReq->lSetStorage;
				m_pServerControl->SetStorage(m_lStorageCurrent);

				return true;
			}

			m_pServerControl->SetStorage(m_lStorageCurrent);

			return m_pServerControl->ServerControl(wSubCmdID, pDataBuffer, wDataSize, pIServerUserItem, m_pITableFrame);
		}
	case SUB_C_CLEAR_JETTON:	//�����ע
		{
			ASSERT(wDataSize==0);
			if(wDataSize!=0) return false;
			//�û�Ч��
// 			tagUserInfo * pUserData=pIServerUserItem->GetUserInfo();
// 
// 			if (pUserData->cbUserStatus!=US_PLAYING) return true;
// 			//״̬У��
// 			if(m_pITableFrame->GetGameStatus() != GS_PLACE_JETTON) return false;
// 
// 			for(int i=0;i<AREA_COUNT+1;i++)
// 			{
// 				m_lAllJettonScore[i]-=m_lUserJettonScore[i][pIServerUserItem->GetUserID()];
// 				m_lUserJettonScore[i][pIServerUserItem->GetUserID()]=0;
// 			}
			return true;
		}
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
	if ((bLookonUser==false)&&(m_dwJettonTime==0L))
	{
		m_dwJettonTime=(DWORD)time(NULL);
		m_pITableFrame->SetGameTimer(IDI_FREE,m_cbFreeTime*1000,1,NULL);
		m_pITableFrame->SetGameStatus(GAME_STATUS_FREE);
	}

	//������ʾ
	/*TCHAR szTipMsg[128];
	myprintf(szTipMsg,CountArray(szTipMsg),TEXT("��������ׯ����Ϊ��%I64d,��������Ϊ��%I64d,�������Ϊ��%I64d"),m_lApplyBankerCondition,
		m_lAreaLimitScore,m_lUserLimitScore);
	m_pITableFrame->SendGameMessage(pIServerUserItem,szTipMsg,SMT_CHAT);*/

	return true;
}

//�û�����
bool  CTableFrameSink::OnActionUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	//��ʼ����
	m_lUserStartScore[wChairID] = 0;
	//��¼�ɼ�
	if (bLookonUser==false)
	{
		//�л�ׯ��
		if (wChairID==m_wCurrentBanker)
		{
			ChangeBanker(true);
		}

		//ȡ������
		for (WORD i=0; i<m_ApplyUserArray.GetCount(); ++i)
		{
			if (wChairID!=m_ApplyUserArray[i]) continue;

			//ɾ�����
			m_ApplyUserArray.RemoveAt(i);

			//�������
			CMD_S_CancelBanker CancelBanker;
			ZeroMemory(&CancelBanker,sizeof(CancelBanker));
			CancelBanker.wCancelUser = wChairID;

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
	if ((cbJettonArea>AREA_COUNT)||(lJettonScore<=0L) || cbJettonArea<1)
	{
		return false;
	}

	if (m_pITableFrame->GetGameStatus()!=GS_PLACE_JETTON)
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
	if (m_wCurrentBanker==wChairID) return true;
	if (m_bEnableSysBanker==false && m_wCurrentBanker==INVALID_CHAIR) return true;

	//��������
	IServerUserItem * pIServerUserItem=m_pITableFrame->GetTableUserItem(wChairID);
	LONGLONG lJettonCount=0L;
	for (int nAreaIndex=1; nAreaIndex<=AREA_COUNT; ++nAreaIndex) lJettonCount += m_lUserJettonScore[nAreaIndex][wChairID];

	//��һ���
	LONGLONG lUserScore = pIServerUserItem->GetUserScore();

	//�Ϸ�У��
	if (lUserScore < lJettonCount + lJettonScore) return true;
	if (m_lUserLimitScore/m_nJettonMultiple < lJettonCount + lJettonScore) return true;

	//�ɹ���ʶ
	bool bPlaceJettonSuccess=true;

	//�Ϸ���֤
	if (GetUserMaxJetton(wChairID,cbJettonArea) >= lJettonScore)
	{
		//��������֤
		if(pIServerUserItem->IsAndroidUser())
		{
			//��������
			if (m_lRobotAreaScore[cbJettonArea] + lJettonScore > m_lRobotAreaLimit)
				return true;

			//��Ŀ����
			bool bHaveChip = false;
			for (int i = 0; i < AREA_COUNT; i++)
			{
				if (m_lUserJettonScore[i+1][wChairID] != 0)
					bHaveChip = true;
			}

			if (!bHaveChip)
			{
				if (m_nChipRobotCount+1 > m_nMaxChipRobot)
				{
					bPlaceJettonSuccess = false;
				}
				else
					m_nChipRobotCount++;
			}

			//ͳ�Ʒ���
			if (bPlaceJettonSuccess)
				m_lRobotAreaScore[cbJettonArea] += lJettonScore;
		}

		if (bPlaceJettonSuccess)
		{
			//������ע
			m_lAllJettonScore[cbJettonArea] += lJettonScore;
			m_lUserJettonScore[cbJettonArea][wChairID] += lJettonScore;	
			
		}	
	}
	else
	{
		bPlaceJettonSuccess=false;

		if(pIServerUserItem->IsAndroidUser())
		{
			CString strInfo;
			strInfo.Format(TEXT("��������עʧ�ܣ���ȣ�%I64d������%d"),lJettonScore,cbJettonArea);

			NcaTextOut(strInfo);

		}
	}

	if (bPlaceJettonSuccess)
	{
		//��������
		CMD_S_PlaceJetton PlaceJetton;
		ZeroMemory(&PlaceJetton,sizeof(PlaceJetton));
		//�������
		PlaceJetton.wChairID=wChairID;
		PlaceJetton.cbJettonArea=cbJettonArea;
		PlaceJetton.lJettonScore=lJettonScore;

		//��ȡ�û�
		IServerUserItem * pIServerUserItem=m_pITableFrame->GetTableUserItem(wChairID);
		if (pIServerUserItem != NULL) 
			PlaceJetton.cbAndroid = pIServerUserItem->IsAndroidUser()? TRUE : FALSE;

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
	} while (cbRandCount<cbBufferCount);

	delete []cbCardData;
	cbCardData = NULL;

	return;
}


//�����˿�
bool CTableFrameSink::DispatchTableCard(bool bCheat)
{
	//��û��������ע
	if(bCheat==true)
	{
		bool bHaveRealUser = false;
		for (WORD wChairID=0; wChairID<GAME_PLAYER; wChairID++)
		{
			IServerUserItem * pIServerUserItem=m_pITableFrame->GetTableUserItem(wChairID);
			if (pIServerUserItem==NULL ) continue;
			if(pIServerUserItem->IsAndroidUser()==true) continue;

			for (WORD wAreaIndex = 1; wAreaIndex <= AREA_COUNT; ++wAreaIndex)
			{
				if(m_lUserJettonScore[wAreaIndex][wChairID]>0)
				{
					bHaveRealUser = true;
					break;
				}
			}
		}
		if(bHaveRealUser==false) bCheat = false;
	}


	m_cbTableCard = GetRandCardValue(bCheat);

	return true;
}

//��ȡ��ֵ
BYTE CTableFrameSink::GetRandCardValue(bool bCheat)
{
	

	if(bCheat == false)
	{
		int n1=m_nMultiple[ID_LANDROVER-1] ;		
		int n2=m_nMultiple[ID_BMW-1] ;	
		int n3=m_nMultiple[ID_MASERATI-1] ;			
		int n4=m_nMultiple[ID_FERRARI-1] ;		
		int n5=m_nMultiple[ID_LAMBORGHINI-1] ;			
		int n6=m_nMultiple[ID_PORSCHE-1] ;			
		int n7=m_nMultiple[ID_JAGUAR-1] ;	
		int n8=m_nMultiple[ID_BENZ-1] ;		

		INT cbControlArea[16] = { 1, 2,  3,  4,   5,  6,  7,  8,   9, 10, 11, 12,   13, 14, 15, 16 };
		INT cbnChance[16]	  = { n3,n2, n8, n7, n4,  n8, n7,  n1, n6, n7, n1, n2, n5, n1, n8, n2 };

		m_GameLogic.RandArray(cbControlArea, CountArray(cbControlArea), cbnChance, CountArray(cbnChance));

		//�������
		DWORD wTick = GetTickCount();

		//���ʺ�ֵ
		INT nChanceAndValue = 0;
		for ( int n = 0; n < CountArray(cbnChance); ++n )
			nChanceAndValue += cbnChance[n];

		INT nMuIndex = 0;
		int nRandNum = 0;					//�������
		static int nStFluc = 1;				
		nRandNum = (rand() + wTick + nStFluc*3 ) % nChanceAndValue;
		for (int j = 0; j < CountArray(cbnChance); j++)
		{
			nRandNum -= cbnChance[j];
			if (nRandNum < 0)
			{
				nMuIndex = j;
				break;
			}
		}
		nStFluc = nStFluc%3 + 1;

		return cbControlArea[nMuIndex];
	}
	else
	{	
		INT cbControlArea[16] = { 1, 2,  3,  4,   5,  6,  7,  8,   9, 10, 11, 12,   13, 14, 15, 16 };
		//��������
		DWORD wTick = GetTickCount();
		for (int nRandIndex=0; nRandIndex<15; nRandIndex++)
		{
			int nTempIndex = rand()%(15-nRandIndex);

			INT nTemp = cbControlArea[15-nRandIndex];
			cbControlArea[15-nRandIndex]=cbControlArea[nTempIndex];
			cbControlArea[nTempIndex]=nTemp;
		}

		for (INT i=0; i<16; i++)
		{

			LONGLONG lSystemScore = 0l;		
			LONGLONG lAllScore = 0l;
			m_cbTableCard = cbControlArea[i];
			JudgeSystemScore(lSystemScore);
			if (lSystemScore < 0)
			{
				return cbControlArea[i];
			}
		}
	}
}

//����ׯ��
bool CTableFrameSink::OnUserApplyBanker(IServerUserItem *pIApplyServerUserItem)
{
	//�Ϸ��ж�
	LONGLONG lUserScore=pIApplyServerUserItem->GetUserScore()*m_nJettonMultiple;
	if (lUserScore<m_lApplyBankerCondition)
	{
		m_pITableFrame->SendGameMessage(pIApplyServerUserItem,TEXT("��Ľ�Ҳ���������ׯ�ң�����ʧ�ܣ�"),SMT_CHAT|SMT_EJECT);
		return true;
	}

	//�����ж�
	WORD wApplyUserChairID=pIApplyServerUserItem->GetChairID();
	for (INT_PTR nUserIdx=0; nUserIdx<m_ApplyUserArray.GetCount(); ++nUserIdx)
	{
		WORD wChairID=m_ApplyUserArray[nUserIdx];
		if (wChairID==wApplyUserChairID)
		{
			m_pITableFrame->SendGameMessage(pIApplyServerUserItem,TEXT("���Ѿ�������ׯ�ң�����Ҫ�ٴ����룡"),SMT_CHAT|SMT_EJECT);
			return true;
		}
	}
	if (pIApplyServerUserItem->IsAndroidUser()&&(m_ApplyUserArray.GetCount())>m_nRobotListMaxCount)
	{
		return true;
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
	if (m_pITableFrame->GetGameStatus()==GAME_STATUS_FREE && m_ApplyUserArray.GetCount()==1)
	{
		if (m_wCurrentBanker==INVALID_CHAIR)
		{
			ChangeBanker(false);

// 			m_dwJettonTime=(DWORD)time(NULL);
// 			m_pITableFrame->SetGameTimer(IDI_FREE,m_cbFreeTime*1000,1,NULL);
// 			m_pITableFrame->SetGameStatus(GAME_STATUS_FREE);
		}
	}

	return true;
}

//ȡ������
bool CTableFrameSink::OnUserCancelBanker(IServerUserItem *pICancelServerUserItem)
{
	//��ǰׯ��
	if (pICancelServerUserItem->GetChairID()==m_wCurrentBanker && m_pITableFrame->GetGameStatus()!=GAME_STATUS_FREE)
	{
		//������Ϣ
		m_pITableFrame->SendGameMessage(pICancelServerUserItem,TEXT("��Ϸ�Ѿ���ʼ��������ȡ����ׯ��"),SMT_CHAT|SMT_EJECT);
		return true;
	}

	//�����ж�
	for (WORD i=0; i<m_ApplyUserArray.GetCount(); ++i)
	{
		//��ȡ���
		WORD wChairID=m_ApplyUserArray[i];
		IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(wChairID);

		//��������
		if (pIServerUserItem==NULL) continue;
		if (pIServerUserItem->GetUserID()!=pICancelServerUserItem->GetUserID()) continue;

		//ɾ�����
		m_ApplyUserArray.RemoveAt(i);

		if (m_wCurrentBanker!=wChairID)
		{
			//�������
			CMD_S_CancelBanker CancelBanker;
			ZeroMemory(&CancelBanker,sizeof(CancelBanker));
			CancelBanker.wCancelUser = pICancelServerUserItem->GetChairID();

			//������Ϣ
			m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));
			m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));
		}
		else if (m_wCurrentBanker==wChairID)
		{
			//�л�ׯ�� 
			m_wCurrentBanker=INVALID_CHAIR;
			ChangeBanker(true);
		}

		return true;
	}

	return false;
}

//����ׯ��
bool CTableFrameSink::ChangeBanker(bool bCancelCurrentBanker)
{
	//�л���ʶ
	bool bChangeBanker=false;

	//ȡ����ǰ
	if (bCancelCurrentBanker)
	{
		for (WORD i=0; i<m_ApplyUserArray.GetCount(); ++i)
		{
			//��ȡ���
			WORD wChairID=m_ApplyUserArray[i];

			//��������
			if (wChairID!=m_wCurrentBanker) continue;

			//ɾ�����
			m_ApplyUserArray.RemoveAt(i);

			break;
		}

		//����ׯ��
		m_wCurrentBanker=INVALID_CHAIR;

		//�ֻ��ж�
		if (m_pITableFrame->GetGameStatus()==GAME_STATUS_FREE && FindSuitBanker())
		{
			m_wCurrentBanker=m_ApplyUserArray[0];
		}

		//���ñ���
		bChangeBanker=true;
		m_bExchangeBanker = true;
	}
	//��ׯ�ж�
	else if (m_wCurrentBanker!=INVALID_CHAIR)
	{
		//��ȡׯ��
		IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(m_wCurrentBanker);

		if(pIServerUserItem!= NULL)
		{
			LONGLONG lBankerScore=pIServerUserItem->GetUserScore()*m_nJettonMultiple;

			//�����ж�
			if (m_lPlayerBankerMAX<=m_wBankerTime || lBankerScore<m_lApplyBankerCondition)
			{
				//ׯ�������ж� ͬһ��ׯ�������ֻ�ж�һ��
				if(m_lPlayerBankerMAX <= m_wBankerTime && m_bExchangeBanker)
				{
					bool bScoreMAX = true;
					m_bExchangeBanker = false;
			
					for (WORD i=0; i<m_ApplyUserArray.GetCount(); ++i)
					{
						//��ȡ���
						WORD wChairID = m_ApplyUserArray[i];
						IServerUserItem *pIUserItem = m_pITableFrame->GetTableUserItem(wChairID);
						LONGLONG lScore = pIUserItem->GetUserScore()*m_nJettonMultiple;

						if ( wChairID != m_wCurrentBanker && lBankerScore <= lScore )
						{
							bScoreMAX = false;
							break;
						}
					}

					LONGLONG wAddTimes=0;
					if ( bScoreMAX || (lBankerScore > m_lBankerScoreMAX && m_lBankerScoreMAX != 0l) )
					{
						if ( bScoreMAX )
							wAddTimes=m_lBankerAdd;
						if ( lBankerScore > m_lBankerScoreMAX && m_lBankerScoreMAX != 0l )
							wAddTimes+=m_lBankerScoreAdd;

						m_lPlayerBankerMAX+=wAddTimes;
						tagUserInfo * pUserInfo = pIServerUserItem->GetUserInfo();
						//��ʾ��Ϣ
						TCHAR szTipMsg[128] = {};
						if (wAddTimes==m_lBankerAdd)
							myprintf(szTipMsg,CountArray(szTipMsg),TEXT("[ %s ]�ķ�����������������ׯ��ң����%d�ζ�����ׯ����!"),pUserInfo->szNickName,wAddTimes);
						else if (wAddTimes==m_lBankerScoreAdd)
							myprintf(szTipMsg,CountArray(szTipMsg),TEXT("[ %s ]�ķ�������[%I64d]�����%d�ζ�����ׯ����!"),pUserInfo->szNickName,m_lBankerScoreMAX,wAddTimes);
						else if(wAddTimes==(m_lBankerScoreAdd+m_lBankerAdd))
							myprintf(szTipMsg,CountArray(szTipMsg),TEXT("[ %s ]�ķ�������[%I64d]�ҳ�������������ׯ��ң����%d�ζ�����ׯ����!"),pUserInfo->szNickName,m_lBankerScoreMAX,wAddTimes);
				
						if (wAddTimes != 0)
						{
							//������Ϣ
							SendGameMessage(INVALID_CHAIR,szTipMsg);
							return true;
						}
						return true;
					}
				}

				//�������
				for (WORD i=0; i<m_ApplyUserArray.GetCount(); ++i)
				{
					//��ȡ���
					WORD wChairID=m_ApplyUserArray[i];

					//��������
					if (wChairID!=m_wCurrentBanker) continue;

					//ɾ�����
					m_ApplyUserArray.RemoveAt(i);

					break;
				}

				//����ׯ��
				m_wCurrentBanker=INVALID_CHAIR;

				//�ֻ��ж�
				if (FindSuitBanker())
				{
					m_wCurrentBanker=m_ApplyUserArray[0];
				}

				bChangeBanker=true;
				m_bExchangeBanker = true;
				tagUserInfo * pUserInfo = pIServerUserItem->GetUserInfo();
				//��ʾ��Ϣ
				TCHAR szTipMsg[128];
				if (lBankerScore<m_lApplyBankerCondition)
					myprintf(szTipMsg,CountArray(szTipMsg),TEXT("[ %s ]��������(%I64d)��ǿ�л�ׯ!"),pUserInfo->szNickName,m_lApplyBankerCondition);
				else
					myprintf(szTipMsg,CountArray(szTipMsg),TEXT("[ %s ]��ׯ�����ﵽ(%d)��ǿ�л�ׯ!"),pUserInfo->szNickName,m_lPlayerBankerMAX);

				//������Ϣ
				SendGameMessage(INVALID_CHAIR,szTipMsg);	
			}
		}
		else
		{
			for (WORD i=0; i<m_ApplyUserArray.GetCount(); ++i)
			{
				//��ȡ���
				WORD wChairID=m_ApplyUserArray[i];

				//��������
				if (wChairID!=m_wCurrentBanker) continue;

				//ɾ�����
				m_ApplyUserArray.RemoveAt(i);

				break;
			}
			//����ׯ��
			m_wCurrentBanker=INVALID_CHAIR;
		}

	}
	//ϵͳ��ׯ
	else if (m_wCurrentBanker==INVALID_CHAIR &&FindSuitBanker())
	{
		m_wCurrentBanker=m_ApplyUserArray[0];
		bChangeBanker=true;
		m_bExchangeBanker = true;
	}

	//�л��ж�
	if (bChangeBanker)
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
		if (m_wCurrentBanker!=INVALID_CHAIR)
		{
			IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(m_wCurrentBanker);
			sChangeBanker.lBankerScore=pIServerUserItem->GetUserScore()*m_nJettonMultiple;
		}
		m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_CHANGE_BANKER,&sChangeBanker,sizeof(sChangeBanker));
		m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_CHANGE_BANKER,&sChangeBanker,sizeof(sChangeBanker));

		if (m_wCurrentBanker!=INVALID_CHAIR)
		{
			//��ȡ��Ϣ
//  			LONGLONG lMessageCount=GetPrivateProfileInt(m_szGameRoomName,TEXT("MessageCount"),0,m_szConfigFileName);
//  			if (lMessageCount!=0)
// 			{
// 				//��ȡ����
//  				LONGLONG lIndex=rand()%lMessageCount;
// 				TCHAR szKeyName[32],szMessage1[256],szMessage2[256];				
// 				myprintf(szKeyName,CountArray(szKeyName),TEXT("Item%I64d"),lIndex);
// 				GetPrivateProfileString(m_szGameRoomName,szKeyName,TEXT("��ϲ[ %s ]��ׯ"),szMessage1,CountArray(szMessage1),m_szConfigFileName);

				//��ȡ���
				TCHAR	szMessage2[256];
				IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(m_wCurrentBanker);
	//			tagUserInfo * pUserInfo = pIServerUserItem->GetUserInfo();
				//������Ϣ
				
				myprintf(szMessage2,CountArray(szMessage2),TEXT("��ϲ[ %s ]��ׯ,�����Ǹ�������ɫ����!"),pIServerUserItem->GetNickName());
				SendGameMessage(INVALID_CHAIR,szMessage2);
			//}
		}
	}

	return bChangeBanker;
}



//����ׯ��
void CTableFrameSink::SendApplyUser( IServerUserItem *pRcvServerUserItem )
{
	for (INT_PTR nUserIdx=0; nUserIdx<m_ApplyUserArray.GetCount(); ++nUserIdx)
	{
		WORD wChairID=m_ApplyUserArray[nUserIdx];

		//��ȡ���
		IServerUserItem *pServerUserItem=m_pITableFrame->GetTableUserItem(wChairID);
		if (!pServerUserItem) continue;

		//ׯ���ж�
		if (pServerUserItem->GetChairID()==m_wCurrentBanker) continue;

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
	if (wChairID==m_wCurrentBanker) ChangeBanker(true);

	//ȡ������
	for (WORD i=0; i<m_ApplyUserArray.GetCount(); ++i)
	{
		if (wChairID!=m_ApplyUserArray[i]) continue;

		//ɾ�����
		m_ApplyUserArray.RemoveAt(i);

		//�������
		CMD_S_CancelBanker CancelBanker;
		ZeroMemory(&CancelBanker,sizeof(CancelBanker));
		CancelBanker.wCancelUser = pIServerUserItem->GetUserID();

		//������Ϣ
		m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));
		m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));

		break;
	}

	return true;
}
void   CTableFrameSink::GetAllWinArea(BYTE bcWinArea[],BYTE bcAreaCount,BYTE InArea)
{
	if (InArea==0xFF)
	{
		return ;
	}
	ZeroMemory(bcWinArea,bcAreaCount);


	LONGLONG lMaxSocre = 0;

	for (int i = 0;i<ANIMAL_COUNT;i++)
	{
		BYTE bcOutCadDataWin[AREA_COUNT];
		BYTE bcData[1];
		bcData[0]=i+1;
		m_GameLogic.GetCardType(bcData[0],1,bcOutCadDataWin);
		for (int j= 0;j<AREA_COUNT;j++)
		{

			if(bcOutCadDataWin[j]>1&&j==InArea-1)
			{
				LONGLONG Score = 0; 
				for (int nAreaIndex=1; nAreaIndex<=AREA_COUNT; ++nAreaIndex) 
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
	if (NULL==pIMeServerUserItem) return 0L;

	
	//����ע��
	LONGLONG lNowJetton = 0;
	ASSERT(AREA_COUNT<=CountArray(m_lUserJettonScore));
	for (int nAreaIndex=1; nAreaIndex<=AREA_COUNT; ++nAreaIndex) lNowJetton += m_lUserJettonScore[nAreaIndex][wChairID];

	//ׯ�ҽ��
	LONGLONG lBankerScore=0x7fffffffffffffff;
	if (m_wCurrentBanker!=INVALID_CHAIR)
	{
		IServerUserItem *pIUserItemBanker=m_pITableFrame->GetTableUserItem(m_wCurrentBanker);
		if (NULL!=pIUserItemBanker) lBankerScore=pIUserItemBanker->GetUserScore();
	}

	BYTE bcWinArea[AREA_COUNT];
	LONGLONG LosScore = 0;
	LONGLONG WinScore = 0;

	GetAllWinArea(bcWinArea,AREA_COUNT,Area);

	for (int nAreaIndex=1; nAreaIndex<=AREA_COUNT; ++nAreaIndex) 
	{
		if(bcWinArea[nAreaIndex-1]>1)
		{
			LosScore+=m_lAllJettonScore[nAreaIndex]*(bcWinArea[nAreaIndex-1]);
		}else
		{
			if(bcWinArea[nAreaIndex-1]==0)
			{
				WinScore+=m_lAllJettonScore[nAreaIndex];

			}
		}
	}
	lBankerScore = lBankerScore + WinScore - LosScore;

	if ( lBankerScore < 0 )
	{
		if (m_wCurrentBanker!=INVALID_CHAIR)
		{
			IServerUserItem *pIUserItemBanker=m_pITableFrame->GetTableUserItem(m_wCurrentBanker);
			if (NULL!=pIUserItemBanker) 
				lBankerScore=pIUserItemBanker->GetUserScore();
		}
		else
		{
			lBankerScore = 0x7fffffffffffffff;
		}
	}

	//��������
	LONGLONG lMeMaxScore = min((pIMeServerUserItem->GetUserScore()-lNowJetton), m_lUserLimitScore/m_nJettonMultiple);

	//��������
	lMeMaxScore=min(lMeMaxScore,m_lAreaLimitScore/m_nJettonMultiple);

	BYTE diMultiple[AREA_COUNT];

	for (int i = 0;i<ANIMAL_COUNT;i++)
	{
		   BYTE bcData[1];
		   bcData[0]= i+1;
		   BYTE  bcOutCadDataWin[AREA_COUNT];
		   m_GameLogic.GetCardType(bcData[0],1,bcOutCadDataWin);
		   for (int j = 0;j<AREA_COUNT;j++)
		   {
			   if(bcOutCadDataWin[j]>1)
			   {
				   diMultiple[j] = bcOutCadDataWin[j];

			   }
		   }
	}
	//ׯ������
	lMeMaxScore=(min(lMeMaxScore,lBankerScore/(diMultiple[Area-1])));

	//��������
	ASSERT(lMeMaxScore >= 0);
	lMeMaxScore = max(lMeMaxScore, 0);

	return (LONGLONG)(lMeMaxScore);
}
//����÷�
LONGLONG CTableFrameSink::CalculateScore()
{
	//��������
	float static cbRevenue=m_pGameServiceOption->wRevenueRatio;

	BYTE  bcResulteOut[AREA_COUNT];
	memset(bcResulteOut,0,AREA_COUNT);
	m_GameLogic.GetCardType(m_cbTableCard,1,bcResulteOut);
	
	//��Ϸ��¼
	tagServerGameRecord &GameRecord = m_GameRecordArrary[m_nRecordLast];

	GameRecord.cbAnimal=m_cbTableCard;
	BYTE  cbMultiple[AREA_COUNT]={1};

	//�ƶ��±�
	m_nRecordLast = (m_nRecordLast+1) % MAX_SCORE_HISTORY;
	if ( m_nRecordLast == m_nRecordFirst ) m_nRecordFirst = (m_nRecordFirst+1) % MAX_SCORE_HISTORY;

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
	for (int i = 1;i<AREA_COUNT+1;i++)
	{
		pUserScore[i]=m_lUserJettonScore[i];
	}

	//˰�ձ���
	float fRevenuePer=float(cbRevenue/1000);

	//������� ˰��
	for (WORD wChairID=0; wChairID<GAME_PLAYER; wChairID++)
	{
		//ׯ���ж�
		if (m_wCurrentBanker==wChairID) continue;

		//��ȡ�û�
		IServerUserItem * pIServerUserItem=m_pITableFrame->GetTableUserItem(wChairID);
		if (pIServerUserItem==NULL) continue;

		for (WORD wAreaIndex = 1; wAreaIndex <= AREA_COUNT; ++wAreaIndex)
		{
			if (bcResulteOut[wAreaIndex-1]>1) 
			{
				m_lUserWinScore[wChairID] += ( pUserScore[wAreaIndex][wChairID] *(bcResulteOut[wAreaIndex-1]-1)) ;
				m_lUserReturnScore[wChairID] += pUserScore[wAreaIndex][wChairID] ;
				lBankerWinScore -= ( pUserScore[wAreaIndex][wChairID] * (bcResulteOut[wAreaIndex-1]-1) ) ;
			}
			else
			{
				if (bcResulteOut[wAreaIndex-1]==0)
				{
					lUserLostScore[wChairID] -= pUserScore[wAreaIndex][wChairID];
					lBankerWinScore += pUserScore[wAreaIndex][wChairID];
				}
				
			}
		}
		//˰��
		m_lUserRevenue[wChairID]  = LONGLONG(m_lUserWinScore[wChairID]*fRevenuePer+0.5);

		//�ܵķ���
		m_lUserWinScore[wChairID] += lUserLostScore[wChairID];
		m_lUserWinScore[wChairID] -= m_lUserRevenue[wChairID];	

	}

	//ׯ�ҳɼ�
	if (m_wCurrentBanker!=INVALID_CHAIR)
	{
		m_lUserWinScore[m_wCurrentBanker] = lBankerWinScore;
	}
	
	//����ׯ��˰��
	for(WORD wChairID = 0;wChairID < GAME_PLAYER;wChairID++)
	{
		IServerUserItem * pIServerUserItem=m_pITableFrame->GetTableUserItem(wChairID);
		if (pIServerUserItem==NULL)
			continue;

		if(m_wCurrentBanker!=INVALID_CHAIR && wChairID == m_wCurrentBanker && lBankerWinScore > 0)
		{
			m_lUserRevenue[m_wCurrentBanker]  = LONGLONG(m_lUserWinScore[m_wCurrentBanker]*fRevenuePer+0.5);
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
bool CTableFrameSink::ProbeJudge(bool& bSystemLost)
{	
	//ϵͳ��Ӯ
	LONGLONG lSystemScore = 0l;
  
	JudgeSystemScore(lSystemScore);
	if (m_lStorageCurrent+lSystemScore>=0 || lSystemScore>=0)
	{
		m_lStorageCurrent += lSystemScore;
		if(NeedDeductStorage())
		{
			m_lStorageCurrent = m_lStorageCurrent - (m_lStorageCurrent * m_StorageDeduct / 1000);
		}
		return true;
	}
	//��治�����ؿ�
	else if (m_lStorageCurrent+lSystemScore<0)
	{
// 		if(bSystemLost==true)
// 		{
// 			if(m_lStorageCurrent + lSystemScore < 0)
// 			{
// 				bSystemLost = false;
// 				return false;
// 			}
// 			m_lStorageCurrent += lSystemScore;
// 			if(NeedDeductStorage())
// 			{
// 				m_lStorageCurrent = m_lStorageCurrent - (m_lStorageCurrent * m_StorageDeduct / 1000);
// 			}
// 			return true;
// 		}else
		return false;
	}
}




//���ͼ�¼
void CTableFrameSink::SendGameRecord(IServerUserItem *pIServerUserItem)
{
	WORD wBufferSize=0;
	BYTE cbBuffer[SOCKET_TCP_BUFFER];
	int nIndex = m_nRecordFirst;
	while ( nIndex != m_nRecordLast )
	{
		if ((wBufferSize+sizeof(tagServerGameRecord))>sizeof(cbBuffer))
		{
			m_pITableFrame->SendUserItemData(pIServerUserItem,SUB_S_SEND_RECORD,cbBuffer,wBufferSize);
			wBufferSize=0;
		}
		CopyMemory(cbBuffer+wBufferSize,&m_GameRecordArrary[nIndex],sizeof(tagServerGameRecord));
		wBufferSize+=sizeof(tagServerGameRecord);

		nIndex = (nIndex+1) % MAX_SCORE_HISTORY;
	}
	if (wBufferSize>0) m_pITableFrame->SendUserItemData(pIServerUserItem,SUB_S_SEND_RECORD,cbBuffer,wBufferSize);
}

//������Ϣ
void CTableFrameSink::SendGameMessage(WORD wChairID, LPCTSTR pszTipMsg)
{
	if (wChairID==INVALID_CHAIR)
	{
		//��Ϸ���
		for (WORD i=0; i<GAME_PLAYER; ++i)
		{
			IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(i);
			if (pIServerUserItem==NULL) continue;
			m_pITableFrame->SendGameMessage(pIServerUserItem,pszTipMsg,SMT_CHAT);
		}

		//�Թ����
		WORD wIndex=0;
		do {
			IServerUserItem *pILookonServerUserItem=m_pITableFrame->EnumLookonUserItem(wIndex++);
			if (pILookonServerUserItem==NULL) break;

			m_pITableFrame->SendGameMessage(pILookonServerUserItem,pszTipMsg,SMT_CHAT);

		}while(true);
	}
	else
	{
		IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(wChairID);
		if (pIServerUserItem!=NULL) m_pITableFrame->SendGameMessage(pIServerUserItem,pszTipMsg,SMT_CHAT|SMT_EJECT);
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
bool CTableFrameSink::OnAdminControl( CMD_C_ControlApplication* pData , IServerUserItem * pIServerUserItem)
{
	switch(pData->cbControlAppType)
	{
	case C_CA_UPDATE:	//����
		{
			CMD_S_ControlReturns ControlReturns;
			ZeroMemory(&ControlReturns,sizeof(ControlReturns));
			ControlReturns.cbReturnsType = S_CR_UPDATE_SUCCES;
			ControlReturns.cbControlArea = m_cbControlArea;
			ControlReturns.cbControlTimes = m_cbControlTimes;
			m_pITableFrame->SendUserItemData(pIServerUserItem,SUB_S_ADMIN_COMMDN,&ControlReturns,sizeof(ControlReturns));
		}
		return true;
	case C_CA_SET:		//����
		{
			if ( pData->cbControlArea == 0xff || pData->cbControlArea  > 7 )
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
	case C_CA_CANCELS:	//ȡ��
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
		if ( pIServerUserItem == NULL )
		{
			m_ApplyUserArray.RemoveAt(0);
		}
		else
		{
			//��ׯ�ж�
			LONGLONG lUserScore=pIServerUserItem->GetUserScore()*m_nJettonMultiple;
			if(lUserScore<m_lApplyBankerCondition)
			{
				//������Ϣ
				CMD_S_CancelBanker CancelBanker;
				ZeroMemory(&CancelBanker,sizeof(CancelBanker));
				CancelBanker.wCancelUser = pIServerUserItem->GetUserID();

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


//��ׯ���� (������)
void CTableFrameSink::RobotBankerControl()
{
	//�Ƿ������ׯ������
	if (m_ApplyUserArray.GetCount()-1 > 10)
		return;

	if (m_ApplyUserArray.GetCount()-1 > 3)
		return;

	//��������
	int nRobotBankerCount = rand() % (10-3) + (3-(m_ApplyUserArray.GetCount()-1));
	int nSendBankerMsgCount = 0;
	int nRandNum = rand() % GAME_PLAYER;

	//����ׯ����������
	for (int i = 0; i < GAME_PLAYER; i++)
	{
		WORD wChairID = (nRandNum+i) % GAME_PLAYER;
		IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(wChairID);
		if (pIServerUserItem != NULL && pIServerUserItem->IsAndroidUser())
		{
			if ( m_wCurrentBanker != wChairID && pIServerUserItem->GetUserScore()*m_nJettonMultiple >= m_lApplyBankerCondition)
			{
				//�����ׯ�б�
				bool bOnApplayArray = false;
				for (int j = 0; j < m_ApplyUserArray.GetCount(); j++)
				{
					if (wChairID == m_ApplyUserArray[j]) 
					{
						bOnApplayArray = true;
						break;
					}
				}

				if (!bOnApplayArray)
				{
					//������Ϣ
					m_pITableFrame->SendTableData(wChairID, SUB_S_ROBOT_BANKER);

					//����ͳ��
					nSendBankerMsgCount++;
					if (nSendBankerMsgCount >= nRobotBankerCount)
						break;
				}
			}
		}
	}
}

//�Ƿ�˥��
bool CTableFrameSink::NeedDeductStorage()
{

	for ( int i = 0; i < GAME_PLAYER; ++i )
	{
		IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(i);
		if (pIServerUserItem == NULL ) continue; 

		if(!pIServerUserItem->IsAndroidUser())
		{
			for (int nAreaIndex=0; nAreaIndex<=AREA_COUNT; ++nAreaIndex) 
			{
				if (m_lUserJettonScore[nAreaIndex][i]!=0)
				{
					return true;
				}				
			}	
		}
	}

	return false;

}

//����д��Ϣ
void CTableFrameSink::WriteInfo( LPCTSTR pszString )
{
	//������������
	char* old_locale = _strdup( setlocale(LC_CTYPE,NULL) );
	setlocale( LC_CTYPE, "chs" );

	CStdioFile myFile;
	CString strFileName;
	strFileName.Format(TEXT("�������ֲ������ռ�.txt"));
	BOOL bOpen = myFile.Open(strFileName, CFile::modeReadWrite|CFile::modeCreate|CFile::modeNoTruncate);
	if ( bOpen )
	{	
		myFile.SeekToEnd();
		myFile.WriteString( pszString );
		myFile.Flush();
		myFile.Close();
	}

	//��ԭ�����趨
	setlocale( LC_CTYPE, old_locale );
	free( old_locale );
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
	if(m_cbFreeTime < TIME_FREE	|| m_cbFreeTime > 99) m_cbFreeTime = TIME_FREE;
	if(m_cbBetTime < TIME_PLACE_JETTON || m_cbBetTime > 99) m_cbBetTime = TIME_PLACE_JETTON;
	if(m_cbEndTime < TIME_GAME_END || m_cbEndTime > 99) m_cbEndTime = TIME_GAME_END;

	//��ע
	m_lAreaLimitScore = pCustomConfig->CustomGeneral.lAreaLimitScore;
	m_lUserLimitScore = pCustomConfig->CustomGeneral.lUserLimitScore;
	m_nJettonMultiple = pCustomConfig->CustomGeneral.nMultiple;
	if(m_nJettonMultiple<=0) m_nJettonMultiple=1;
	//���
	m_StorageStart = pCustomConfig->CustomGeneral.StorageStart;
	m_lStorageCurrent = m_StorageStart;
	m_StorageDeduct = pCustomConfig->CustomGeneral.StorageDeduct;
	m_lStorageMax1 = pCustomConfig->CustomGeneral.StorageMax1;
	m_lStorageMul1 = pCustomConfig->CustomGeneral.StorageMul1;
	m_lStorageMax2 = pCustomConfig->CustomGeneral.StorageMax2;
	m_lStorageMul2 = pCustomConfig->CustomGeneral.StorageMul2;
	if(m_lStorageMul1 < 0 || m_lStorageMul1 > 100 ) m_lStorageMul1 = 50;
	if(m_lStorageMul2 < 0 || m_lStorageMul2 > 100 ) m_lStorageMul2 = 80;

	//������
	m_nRobotListMaxCount = pCustomConfig->CustomAndroid.lRobotListMaxCount;

	LONGLONG lRobotBetMinCount = pCustomConfig->CustomAndroid.lRobotBetMinCount;
	LONGLONG lRobotBetMaxCount = pCustomConfig->CustomAndroid.lRobotBetMaxCount;
	m_nMaxChipRobot = rand()%(lRobotBetMaxCount-lRobotBetMinCount+1) + lRobotBetMinCount;
	if (m_nMaxChipRobot < 0)	m_nMaxChipRobot = 8;
	m_lRobotAreaLimit = pCustomConfig->CustomAndroid.lRobotAreaLimit;


	if ( m_lBankerScoreMAX <= m_lApplyBankerCondition)
		m_lBankerScoreMAX = 0l;

	m_lPlayerBankerMAX = m_lBankerMAX;

	//������
	m_nMultiple[ID_LANDROVER-1] = 20;
	m_nMultiple[ID_BMW-1] = 20;
	m_nMultiple[ID_MASERATI-1] = 5;
	m_nMultiple[ID_FERRARI-1] =5;
	m_nMultiple[ID_LAMBORGHINI-1] = 5;
	m_nMultiple[ID_PORSCHE-1] = 5;
	m_nMultiple[ID_JAGUAR-1] = 20;
	m_nMultiple[ID_BENZ-1] = 20;

}
void CTableFrameSink::JudgeSystemScore( LONGLONG& lSystemScore)
{
	BYTE  bcResulteOut[AREA_COUNT];
	memset(bcResulteOut,0,AREA_COUNT);
	m_GameLogic.GetCardType(m_cbTableCard,1,bcResulteOut);
	//�����ע
	LONGLONG *pUserScore[AREA_COUNT+1];
	pUserScore[0] = NULL;
	for (int i = 1;i<AREA_COUNT+1;i++)
	{
		pUserScore[i] = m_lUserJettonScore[i];
	}

	//ׯ���ǲ��ǻ�����
	bool bIsBankerAndroidUser = false;
	if ( m_wCurrentBanker != INVALID_CHAIR )
	{
		IServerUserItem * pIBankerUserItem = m_pITableFrame->GetTableUserItem(m_wCurrentBanker);
		if (pIBankerUserItem != NULL) 
		{
			bIsBankerAndroidUser = pIBankerUserItem->IsAndroidUser();
		}
	}

	//�������
	for (WORD wChairID=0; wChairID<GAME_PLAYER; wChairID++)
	{
		//ׯ���ж�
		if (m_wCurrentBanker == wChairID) continue;

		//��ȡ�û�
		IServerUserItem * pIServerUserItem=m_pITableFrame->GetTableUserItem(wChairID);
		if (pIServerUserItem==NULL) 
			continue;

		bool bIsAndroidUser = pIServerUserItem->IsAndroidUser();

		for (WORD wAreaIndex = 1; wAreaIndex <= AREA_COUNT; ++wAreaIndex)
		{

			if (bcResulteOut[wAreaIndex-1]>1) 
			{
				if ( bIsAndroidUser )
					lSystemScore += (pUserScore[wAreaIndex][wChairID] *(bcResulteOut[wAreaIndex-1]-1));

				if (m_wCurrentBanker == INVALID_CHAIR || bIsBankerAndroidUser)
					lSystemScore -= (pUserScore[wAreaIndex][wChairID] *(bcResulteOut[wAreaIndex-1]-1));
			}
			else
			{
				if (bcResulteOut[wAreaIndex-1]==0)
				{
					if ( bIsAndroidUser )
						lSystemScore -= pUserScore[wAreaIndex][wChairID];

					if (m_wCurrentBanker == INVALID_CHAIR || bIsBankerAndroidUser)
						lSystemScore += pUserScore[wAreaIndex][wChairID];
				}
			}
		}
	}

}

//�����¼�
bool CTableFrameSink::OnUserScroeNotify(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)
{
	//��ׯ�ҷ����ڿ���ʱ��䶯ʱ(��ׯ�ҽ����˴�ȡ��)У��ׯ�ҵ���ׯ����
	if(wChairID == m_wCurrentBanker && m_pITableFrame->GetGameStatus() == GAME_STATUS_FREE)
	{
		ChangeBanker(false);
	}

	return true;
}

