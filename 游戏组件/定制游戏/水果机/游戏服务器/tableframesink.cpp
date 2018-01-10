#include "StdAfx.h"
#include "TableFrameSink.h"
#include "GameServerManager.h"
#include <locale>

//////////////////////////////////////////////////////////////////////////

//��������
#define SEND_COUNT					300									//���ʹ���

//��������
#define INDEX_PLAYER				0									//�м�����
#define INDEX_BANKER				1									//ׯ������

//��עʱ��
#define IDI_FREE					1									//����ʱ��
#define TIME_FREE					10									//����ʱ��

//��עʱ��
#define IDI_PLACE_JETTON			2									//��עʱ��
#define TIME_PLACE_JETTON			20									//��עʱ��
//����ʱ��
#define IDI_GAME_END				3									//����ʱ��
#define TIME_GAME_END				15									//����ʱ��

//��עʱ��
#define IDI_SMALL_BIG				4									//��עʱ��
#define TIME_SMALL_BIG				10									//��עʱ��
#define TIME_GAME_ADD               15                                  //����ʱ��
//////////////////////////////////////////////////////////////////////////
//��̬����
const WORD			CTableFrameSink::m_wPlayerCount=GAME_PLAYER;				//��Ϸ����
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
		fp=fopen("���������.log","w");	
	}	
	else	
	{		
		fp=fopen("���������.log","a");	
	}	
	if(fp == NULL)	
		return ;	
	num++;	
	SYSTEMTIME time;	
	GetLocalTime(&time);	
	fprintf(fp, "%d:%s - %d/%d/%d %d:%d:%d \n", num, buf, time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);	
	fclose(fp); 
}

//////////////////////////////////////////////////////////////////////////

//���캯��
CTableFrameSink::CTableFrameSink()
{
	//����ע��
	ZeroMemory(m_lAllJettonScore,sizeof(m_lAllJettonScore));

	//������ע
	ZeroMemory(m_lUserJettonScore,sizeof(m_lUserJettonScore));

	m_CheckImage = 0;

	//��ҳɼ�	
	ZeroMemory(m_lUserWinScore,sizeof(m_lUserWinScore));
	ZeroMemory(m_lUserTempScore, sizeof(m_lUserTempScore));
	ZeroMemory(m_lUserReturnScore,sizeof(m_lUserReturnScore));
	ZeroMemory(m_lUserRevenue,sizeof(m_lUserRevenue));
	ZeroMemory(m_lUserEveryTurnScore,sizeof(m_lUserEveryTurnScore));
	ZeroMemory(m_lUserTotalScore,sizeof(m_lUserTotalScore));
	//�˿���Ϣ
	ZeroMemory(m_cbCardCount,sizeof(m_cbCardCount));
	ZeroMemory(m_cbTableCardArray,sizeof(m_cbTableCardArray));

	//״̬����
	m_dwJettonTime=0L;
	m_bControl=false;

	//ׯ����Ϣ
	m_ApplyUserArray.RemoveAll();
	m_wCurrentBanker=INVALID_CHAIR;
	m_lBankerScore=0l;
	m_wBankerTime=0;
	m_lBankerWinScore=0L;		
	m_lBankerCurGameScore=0L;
	m_bEnableSysBanker=true;
	m_cbLeftCardCount=0;
	m_bContiueCard=false;
	m_bBankerQiut = false;

	//��¼����
	ZeroMemory(m_GameRecordArrary,sizeof(m_GameRecordArrary));
	ZeroMemory(m_lExchangeGold,sizeof(m_lExchangeGold));
	m_nRecordFirst=0;
	m_nRecordLast=0;
	m_dwRecordCount=0;	
	m_nProbability=0;
	m_nRand=0;

	//���Ʊ���
	m_StorageStart = 0l;
	m_StorageDeduct = 0l;

	//�����˿���					
	m_lRobotAreaLimit = 0l;
	m_lRobotBetCount = 0l;		
	m_nRobotListMaxCount =0;

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
	m_cbCompareTime = TIME_FREE;
	m_cbAddTime=TIME_GAME_ADD;

	//�����˿���
	m_nChipRobotCount = 0;
	ZeroMemory(m_lRobotAreaScore, sizeof(m_lRobotAreaScore));

	//�������
	m_hInst = NULL;
	m_pServerContro = NULL;
	m_hInst = LoadLibrary(_TEXT("FruitBattleServerControl.dll"));
	if ( m_hInst )
	{
		typedef void * (*CREATE)(); 
		CREATE ServerControl = (CREATE)GetProcAddress(m_hInst,"CreateServerControl"); 
		if ( ServerControl )
		{
			m_pServerContro = static_cast<IServerControl*>(ServerControl());
		}
	}

	return;
}

//��������
CTableFrameSink::~CTableFrameSink(void)
{
}

//�ӿڲ�ѯ
VOID * CTableFrameSink::QueryInterface(const IID & Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(ITableFrameSink,Guid,dwQueryVer);
	QUERYINTERFACE(ITableUserAction,Guid,dwQueryVer);
#ifdef __SPECIAL___
	QUERYINTERFACE(ITableUserActionEX,Guid,dwQueryVer);	
#endif
	QUERYINTERFACE_IUNKNOWNEX(ITableFrameSink,Guid,dwQueryVer);
	return NULL;
}

//��ʼ��
bool CTableFrameSink::Initialization(IUnknownEx * pIUnknownEx)
{
	//��ѯ�ӿ�
	ASSERT(pIUnknownEx!=NULL);
	m_pITableFrame=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,ITableFrame);
	if (m_pITableFrame==NULL) return false;

	//���ƽӿ�
	//m_pITableFrameControl=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,ITableFrameControl);
	//if (m_pITableFrameControl==NULL) return false;
	m_pITableFrame->SetStartMode(START_MODE_TIME_CONTROL);

	//��ȡ����
	m_pGameServiceOption=m_pITableFrame->GetGameServiceOption();
	ASSERT(m_pGameServiceOption!=NULL);

	//�����ļ���
	TCHAR szPath[MAX_PATH]=TEXT("");
	GetCurrentDirectory(sizeof(szPath),szPath);
	_sntprintf(m_szConfigFileName,sizeof(m_szConfigFileName),TEXT("%s\\FruitBattle.ini"),szPath);
	memcpy(m_szGameRoomName, m_pGameServiceOption->szServerName, sizeof(m_szGameRoomName));

	ReadConfigInformation(true);
	return true;
}

//��λ����
VOID CTableFrameSink::RepositionSink()
{
	//����ע��
	ZeroMemory(m_lAllJettonScore,sizeof(m_lAllJettonScore));

	//������ע
	ZeroMemory(m_lUserJettonScore,sizeof(m_lUserJettonScore));

	//��ҳɼ�	
	ZeroMemory(m_lUserTempScore, sizeof(m_lUserTempScore));
	ZeroMemory(m_lUserReturnScore,sizeof(m_lUserReturnScore));
	ZeroMemory(m_lUserRevenue,sizeof(m_lUserRevenue));
	m_bBankerQiut = false;
	
	//�����˿���
	m_nChipRobotCount = 0;
	ZeroMemory(m_lRobotAreaScore, sizeof(m_lRobotAreaScore));
	m_bControl=false;

	return;
}

bool CTableFrameSink::OnSubAmdinCommand(IServerUserItem*pIServerUserItem,const void*pDataBuffer)
{
	//��������й���ԱȨ�� �򷵻ش���
	if( !CUserRight::IsGameCheatUser(pIServerUserItem->GetUserRight()) )
		return false;

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

//��Ϸ��ʼ
bool CTableFrameSink::OnEventGameStart()
{

	//�����־
	StorageLog();
	//��������
	CMD_S_GameStart GameStart;
	ZeroMemory(&GameStart,sizeof(GameStart));

	//��ȡׯ��
	IServerUserItem *pIBankerServerUserItem=NULL;
	if (INVALID_CHAIR!=m_wCurrentBanker) 
	{
		pIBankerServerUserItem=m_pITableFrame->GetTableUserItem(m_wCurrentBanker);
		m_lBankerScore=m_lExchangeGold[m_wCurrentBanker];
	}

	//���ñ���
	GameStart.cbTimeLeave=m_cbBetTime;
	GameStart.wBankerUser=m_wCurrentBanker;
	GameStart.lBankerScore = 0x7fffffffffffffff;
	if (pIBankerServerUserItem!=NULL) 
		GameStart.lBankerScore=m_lExchangeGold[m_wCurrentBanker];

	GameStart.bContiueCard=m_bContiueCard;
	//��ע����������
	int nChipRobotCount = 0;
	for (int i = 0; i < GAME_PLAYER; i++)
		{
			IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(i);
			if (pIServerUserItem != NULL && pIServerUserItem->IsAndroidUser())
				nChipRobotCount++;
		}

	GameStart.nChipRobotCount = min(nChipRobotCount, m_nMaxChipRobot);

	//��ׯ�б�
	nChipRobotCount=0;
	for (int i = 0; i < m_ApplyUserArray.GetCount(); i++)
	{
		IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(m_ApplyUserArray.GetAt(i));
		if (pIServerUserItem != NULL && pIServerUserItem->IsAndroidUser())
			nChipRobotCount++;
	}
	GameStart.nAndroidCount=nChipRobotCount;
	//�����˿���
	m_nChipRobotCount = 0;

	//�Թ����
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_START,&GameStart,sizeof(GameStart));	

	//��Ϸ���
	for (WORD wChairID=0; wChairID<GAME_PLAYER; ++wChairID)
	{
		IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(wChairID);
		if (pIServerUserItem==NULL) continue;
		
		if(pIServerUserItem->GetUserScore() < m_nExchange*1)
		{
			CString szinfo;
			szinfo.Format(TEXT("������Ϸ�Ҳ����Խ�����Ϸ�����Ȳ�����Ϸ��"));
			m_pITableFrame->SendGameMessage(pIServerUserItem,szinfo,SMT_CLOSE_GAME|SMT_EJECT);
			continue;
		}

		
		if (m_bEnableSysBanker==false && m_wCurrentBanker==INVALID_CHAIR)
			{
				GameStart.lBankerScore = 1;
			}

		//���û���
		if (pIServerUserItem->IsAndroidUser())
			{
				GameStart.lUserMaxScore=min(m_lExchangeGold[wChairID],m_lUserLimitScore);
			}
		else
			{
				GameStart.lUserMaxScore=min(m_lExchangeGold[wChairID],m_lUserLimitScore);
			}


		m_pITableFrame->SendTableData(wChairID,SUB_S_GAME_START,&GameStart,sizeof(GameStart));	

		//Ȩ���ж�
		if(CUserRight::IsGameCheatUser(pIServerUserItem->GetUserRight()))
			{
				CMD_S_ADMIN_STORAGE_INFO StorageInfo;
				ZeroMemory(&StorageInfo,sizeof(StorageInfo));
				StorageInfo.lCurrentStorage = m_StorageStart;
				StorageInfo.lCurrentDeduct = m_StorageDeduct;
				CopyMemory(StorageInfo.lMaxStorage,m_lStorageMax,sizeof(m_lStorageMax));
				CopyMemory(StorageInfo.wStorageMul,m_wStorageMul,sizeof(m_wStorageMul));
				CopyMemory(StorageInfo.lUserTotalScore,m_lUserTotalScore,sizeof(m_lUserTotalScore));
				m_pITableFrame->SendTableData(wChairID,SUB_S_ADMIN_STORAGE_INFO,&StorageInfo,sizeof(StorageInfo));
				m_pITableFrame->SendLookonData(wChairID,SUB_S_ADMIN_STORAGE_INFO,&StorageInfo,sizeof(StorageInfo));
			}
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
			try
			{
				//����
				if ( m_pServerContro != NULL && m_pServerContro->NeedControl())
				{
						m_bControl=true;
						tagControlInfo ControlInfo;
						m_pServerContro->ReturnControlArea(ControlInfo);
						m_cbTableCardArray[0] = ControlInfo.cbControlArea;
						m_pServerContro->CompleteControl();

						BYTE  bcResulteOut[AREA_COUNT+1];
						memset(bcResulteOut,0,AREA_COUNT+1);
						m_GameLogic.GetCardType(m_cbTableCardArray[0],1,bcResulteOut);
						//ϵͳ��Ӯ
						LONGLONG lSystemScore = 0l;
						//�������
						for (WORD wChairID1=0; wChairID1<GAME_PLAYER; wChairID1++)
						{
							//ׯ���ж�
							if (m_wCurrentBanker == wChairID1) continue;

							//��ȡ�û�
							IServerUserItem * pIServerUserItem=m_pITableFrame->GetTableUserItem(wChairID1);
							if (pIServerUserItem==NULL) 
								continue;

							bool bIsAndroidUser = pIServerUserItem->IsAndroidUser();
							//�����ע
							LONGLONG *pUserScore[AREA_COUNT+1];
							pUserScore[0]=NULL;
							for (int i = 1;i<=AREA_COUNT;i++)
							{
								pUserScore[i]=m_lUserJettonScore[i];
							}
							//��ׯ�Ƿ��ǻ�����
							bool bIsBankerAndroidUser = false;
							if ( m_wCurrentBanker != INVALID_CHAIR )
							{
								IServerUserItem * pIBankerUserItem = m_pITableFrame->GetTableUserItem(m_wCurrentBanker);
								if (pIBankerUserItem != NULL) 
								{
									bIsBankerAndroidUser = pIBankerUserItem->IsAndroidUser();
								}
							}
							//ͳ�ƻ���
							for (WORD wAreaIndex = 1; wAreaIndex <= AREA_COUNT; ++wAreaIndex)
							{			
								if (bcResulteOut[wAreaIndex-1]>0) 
								{
									if ( bIsAndroidUser )
										lSystemScore += (pUserScore[wAreaIndex][wChairID1] *(bcResulteOut[wAreaIndex-1]-1));

									if (m_wCurrentBanker == INVALID_CHAIR || bIsBankerAndroidUser)
										lSystemScore -= (pUserScore[wAreaIndex][wChairID1] *(bcResulteOut[wAreaIndex-1]-1));
								}
								else
								{
									if ( bIsAndroidUser )
										lSystemScore -= pUserScore[wAreaIndex][wChairID1];

									if (m_wCurrentBanker == INVALID_CHAIR || bIsBankerAndroidUser)
										lSystemScore += pUserScore[wAreaIndex][wChairID1];
								}
							}
						}

						m_StorageStart += lSystemScore*m_nExchange;
						//ֻ�п�����0�Ż�˥��
						if(m_StorageStart > 0 && NeedDeductStorage())
							m_StorageStart = m_StorageStart - (m_StorageStart * m_StorageDeduct / 1000);
				}
				else
				{
					while(true)
					{
						//�ɷ��˿�
						DispatchTableCard();

						//��̽���ж�
						if (ProbeJudge())
						{
							break;
						}
					}
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
				GameEnd.bcFirstCard = m_bcFirstPostCard;
				GameEnd.nProbability=m_nProbability;
				GameEnd.nRand=m_nRand;
				//�˿���Ϣ
				CopyMemory(GameEnd.cbTableCardArray,m_cbTableCardArray,sizeof(m_cbTableCardArray));
				GameEnd.cbLeftCardCount=m_cbLeftCardCount;

				//���ͻ���
				GameEnd.cbTimeLeave=m_cbEndTime;	
				for ( WORD wUserIndex = 0; wUserIndex < GAME_PLAYER; ++wUserIndex )
				{
					IServerUserItem *pIServerUserItem = m_pITableFrame->GetTableUserItem(wUserIndex);
					if ( pIServerUserItem == NULL ) continue;

					//���óɼ�
					GameEnd.lUserScore=m_lUserEveryTurnScore[wUserIndex];

					//��������
					GameEnd.lUserReturnScore[wUserIndex]=m_lUserReturnScore[wUserIndex];

					//����˰��
					if (m_lUserRevenue[wUserIndex]>0) GameEnd.lRevenue=m_lUserRevenue[wUserIndex];
					else if (m_wCurrentBanker!=INVALID_CHAIR) GameEnd.lRevenue=m_lUserRevenue[m_wCurrentBanker];
					else GameEnd.lRevenue=0;

					//������Ϣ					
					m_pITableFrame->SendTableData(wUserIndex,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
					m_pITableFrame->SendLookonData(wUserIndex,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
				}

				for (WORD wChairIDIndex=0; wChairIDIndex<GAME_PLAYER; wChairIDIndex++)
				{
					//д�����
					if(m_lUserWinScore[wChairIDIndex]!=0L)
					{		
						IServerUserItem *pIServerUserItemTemp = m_pITableFrame->GetTableUserItem(wChairIDIndex);
						if ( pIServerUserItemTemp == NULL ) continue;

						//д�����ǰ ��¼�������Ӯ
						m_lUserTotalScore[wChairIDIndex]+=m_lUserWinScore[wChairIDIndex];
						//��������
						LONGLONG static cbRevenue=m_pGameServiceOption->wRevenueRatio;
						LONGLONG lTotalScore=0;
						if (0 < m_lUserWinScore[wChairIDIndex])
						{
							double fRevenuePer=double(cbRevenue/1000.0);
							lTotalScore=m_lUserWinScore[wChairIDIndex]*m_nExchange;
							m_lUserRevenue[wChairIDIndex]  = LONGLONG(lTotalScore*fRevenuePer);
							lTotalScore -= m_lUserRevenue[wChairIDIndex];
						}
						else if(m_lUserWinScore[wChairIDIndex]<0)
						{
							lTotalScore=m_lUserWinScore[wChairIDIndex]*m_nExchange;
							m_lUserRevenue[wChairIDIndex]  = 0;	

							if(-lTotalScore>pIServerUserItemTemp->GetUserScore())
							{
// 								CString str;
// 								str.Format(TEXT("���ID:%d,�ǳ�:%s,�����쳣��Я�����:%I64d,���:%I64d"),pIServerUserItemTemp->GetUserID(),pIServerUserItemTemp->GetNickName(),pIServerUserItemTemp->GetUserScore(),lTotalScore);
// 								CTraceService::TraceString(str,TraceLevel_Exception);
								lTotalScore = -pIServerUserItemTemp->GetUserScore();
							}
						}

						tagScoreInfo ScoreInfo;
						ZeroMemory(&ScoreInfo,sizeof(ScoreInfo));
						ScoreInfo.cbType =(lTotalScore>0L)?SCORE_TYPE_WIN:SCORE_TYPE_LOSE;
						ScoreInfo.lRevenue = m_lUserRevenue[wChairIDIndex];
						ScoreInfo.lScore = lTotalScore;
						m_pITableFrame->WriteUserScore(wChairIDIndex,ScoreInfo);

						m_lUserWinScore[wChairIDIndex] = 0;
						m_lUserRevenue[wChairIDIndex] = 0;
					}
				}
			}
			catch(...)
			{
				CString str;
				str.Format(TEXT("OnEventGameConclude,Reason:Normal"));
				CTraceService::TraceString(str,TraceLevel_Exception);
			}
			
			return true;
		}
	case GER_NETWORK_ERROR:
	case GER_USER_LEAVE:		//�û��뿪
		{
			//�м��ж�
			if (m_wCurrentBanker!=wChairID)
			{
				try
				{
					//��������
					LONGLONG lScore=0;
					LONGLONG lRevenue=0;

					//ͳ�Ƴɼ�
					for (int nAreaIndex=1; nAreaIndex<=AREA_COUNT; ++nAreaIndex) lScore -= m_lUserJettonScore[nAreaIndex][wChairID];

					//д�����
					if (m_pITableFrame->GetGameStatus() < GS_GAME_END)
					{
						for (int nAreaIndex=1; nAreaIndex<=AREA_COUNT; ++nAreaIndex)
						{
							if (m_lUserJettonScore[nAreaIndex][wChairID] > 0)
							{
								//������ҿ۳���ǰ����ע�ĳ����ϵͳ
								m_lUserWinScore[wChairID]-= m_lUserJettonScore[nAreaIndex][wChairID];

								CMD_S_PlaceJettonFail PlaceJettonFail;
								ZeroMemory(&PlaceJettonFail,sizeof(PlaceJettonFail));
								PlaceJettonFail.lJettonArea=nAreaIndex;
								PlaceJettonFail.lPlaceScore=m_lUserJettonScore[nAreaIndex][wChairID];
								PlaceJettonFail.wPlaceUser=wChairID;

								//��Ϸ���
								for (WORD i=0; i<GAME_PLAYER; ++i)
								{
									IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(i);
									if (pIServerUserItem==NULL) 
										continue;

									m_pITableFrame->SendTableData(i,SUB_S_PLACE_JETTON_FAIL,&PlaceJettonFail,sizeof(PlaceJettonFail));
								}

								m_lAllJettonScore[nAreaIndex] -= m_lUserJettonScore[nAreaIndex][wChairID];
								m_lUserJettonScore[nAreaIndex][wChairID] = 0;
							}
						}
						if(m_lUserWinScore[wChairID]!=0L)
						{
							//��������
							LONGLONG static cbRevenue=m_pGameServiceOption->wRevenueRatio;
							LONGLONG lTotalScore=0;
							if (0 < m_lUserWinScore[wChairID])
							{
								double fRevenuePer=double(cbRevenue/1000.0);
								lTotalScore=m_lUserWinScore[wChairID]*m_nExchange;
								m_lUserRevenue[wChairID]  = LONGLONG(lTotalScore*fRevenuePer);
								lTotalScore -= m_lUserRevenue[wChairID];
							}
							else if(m_lUserWinScore[wChairID]<0)
							{
								lTotalScore=m_lUserWinScore[wChairID]*m_nExchange;
								if(pIServerUserItem != NULL)
								{
									if(-lTotalScore>pIServerUserItem->GetUserScore())
									{
// 										CString str;
// 										str.Format(TEXT("���ID:%d,�ǳ�:%s,�����쳣��Я�����:%I64d,���:%I64d"),pIServerUserItem->GetUserID(),pIServerUserItem->GetNickName(),pIServerUserItem->GetUserScore(),lTotalScore);
// 										CTraceService::TraceString(str,TraceLevel_Exception);
										lTotalScore = -pIServerUserItem->GetUserScore();
									}
								}

								m_lUserRevenue[wChairID]  = 0;							
							}
							tagScoreInfo ScoreInfo;
							ZeroMemory(&ScoreInfo,sizeof(ScoreInfo));
							ScoreInfo.cbType =(lTotalScore>0L)?SCORE_TYPE_WIN:SCORE_TYPE_LOSE;
							ScoreInfo.lRevenue = m_lUserRevenue[wChairID];
							ScoreInfo.lScore = lTotalScore;
							m_pITableFrame->WriteUserScore(wChairID,ScoreInfo);
							m_lExchangeGold[wChairID] = 0;
							m_lUserWinScore[wChairID] = 0;
							m_lUserTotalScore[wChairID]=0;
						}
					}
					else
					{
						//д�����
						if(m_lUserWinScore[wChairID]!=0L)
						{
							//��������
							LONGLONG static cbRevenue=m_pGameServiceOption->wRevenueRatio;
							LONGLONG lTotalScore=0;
							if (0 < m_lUserWinScore[wChairID])
							{
								double fRevenuePer=double(cbRevenue/1000.0);
								lTotalScore=m_lUserWinScore[wChairID]*m_nExchange;
								m_lUserRevenue[wChairID]  = LONGLONG(lTotalScore*fRevenuePer);
								lTotalScore -= m_lUserRevenue[wChairID];
							}
							else if(m_lUserWinScore[wChairID]<0)
							{
								lTotalScore=m_lUserWinScore[wChairID]*m_nExchange;
								if(pIServerUserItem != NULL)
								{
									if(-lTotalScore>pIServerUserItem->GetUserScore())
									{
// 										CString str;
// 										str.Format(TEXT("���ID:%d,�ǳ�:%s,�����쳣��Я�����:%I64d,���:%I64d"),pIServerUserItem->GetUserID(),pIServerUserItem->GetNickName(),pIServerUserItem->GetUserScore(),lTotalScore);
// 										CTraceService::TraceString(str,TraceLevel_Exception);
										lTotalScore = -pIServerUserItem->GetUserScore();
									}
								}

								m_lUserRevenue[wChairID]  = 0;							
							}
							tagScoreInfo ScoreInfo;
							ZeroMemory(&ScoreInfo,sizeof(ScoreInfo));
							ScoreInfo.cbType =(lTotalScore>0L)?SCORE_TYPE_WIN:SCORE_TYPE_LOSE;
							ScoreInfo.lRevenue = m_lUserRevenue[wChairID];
							ScoreInfo.lScore = lTotalScore;
							m_pITableFrame->WriteUserScore(wChairID,ScoreInfo);
							m_lExchangeGold[wChairID] = 0;
							m_lUserWinScore[wChairID] = 0;
							m_lUserTotalScore[wChairID]=0;
						}
						for (int nAreaIndex=1; nAreaIndex<=AREA_COUNT; ++nAreaIndex)
						{
							if (m_lUserJettonScore[nAreaIndex][wChairID] != 0)
							{
								m_lUserJettonScore[nAreaIndex][wChairID] = 0;
							}
						}
					}
				}
				catch(...)
				{
					CString str;
					str.Format(TEXT("OnEventGameConclude,Reason:%s,Not Banker"),(cbReason==GER_USER_LEAVE)?TEXT("GER_USER_LEAVE"):TEXT("GER_NETWORK_ERROR") );
					CTraceService::TraceString(str,TraceLevel_Exception);

				}
				return true;
			}
			
			try
			{
				//״̬�ж�			
				if (m_pITableFrame->GetGameStatus()==GS_GAME_FREE)
				{
					m_pITableFrame->KillGameTimer(IDI_FREE);
					m_pITableFrame->StartGame();
					//����ʱ��
					m_dwJettonTime=(DWORD)time(NULL);
					m_pITableFrame->SetGameTimer(IDI_PLACE_JETTON,m_cbBetTime*1000,1,0L);

					//����״̬
					m_pITableFrame->SetGameStatus(GS_PLACE_JETTON);
				}
				else if (m_pITableFrame->GetGameStatus() < GS_GAME_END && m_pITableFrame->GetGameStatus() > GS_GAME_FREE)
				{
					//��ʾ��Ϣ
					TCHAR szTipMsg[128];
					_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("����ׯ��[ %s ]ǿ�ˣ���Ϸ��ǰ������"),pIServerUserItem->GetNickName());
					m_bBankerQiut = true;

					//������Ϣ
					SendGameMessage(INVALID_CHAIR,szTipMsg);				

					//����ʱ��
					//m_pITableFrame->KillGameTimer(IDI_FREE);
					m_pITableFrame->KillGameTimer(IDI_PLACE_JETTON);
					m_dwJettonTime=(DWORD)time(NULL);
					m_pITableFrame->SetGameTimer(IDI_GAME_END,m_cbEndTime*1000,1,0L);

					//����״̬
					m_pITableFrame->SetGameStatus(GS_GAME_END);

					//����
					if ( m_pServerContro != NULL && m_pServerContro->NeedControl() )
					{
						tagControlInfo ControlInfo;
						m_pServerContro->ReturnControlArea(ControlInfo);
						m_cbTableCardArray[0] = ControlInfo.cbControlArea;
						m_pServerContro->CompleteControl();
					}
					else
					{
						while(true)
						{
							//�ɷ��˿�
							DispatchTableCard();

							//��̽���ж�
							if (ProbeJudge())
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
					CopyMemory(GameEnd.cbTableCardArray,m_cbTableCardArray,sizeof(m_cbTableCardArray));
					GameEnd.cbLeftCardCount=m_cbLeftCardCount;

					//���ͻ���
					GameEnd.cbTimeLeave=m_cbEndTime;	
					for ( WORD wUserIndex = 0; wUserIndex < GAME_PLAYER; ++wUserIndex )
					{
						IServerUserItem *pIServerUserItem = m_pITableFrame->GetTableUserItem(wUserIndex);
						if ( pIServerUserItem == NULL ) continue;

						//���óɼ�
						GameEnd.lUserScore=m_lUserEveryTurnScore[wUserIndex];

						//��������
						GameEnd.lUserReturnScore[wUserIndex]=m_lUserReturnScore[wUserIndex];

						//����˰��
						if (m_lUserRevenue[wUserIndex]>0) GameEnd.lRevenue=m_lUserRevenue[wUserIndex];
						else if (m_wCurrentBanker!=INVALID_CHAIR) GameEnd.lRevenue=m_lUserRevenue[m_wCurrentBanker];
						else GameEnd.lRevenue=0;

						//������Ϣ					
						m_pITableFrame->SendTableData(wUserIndex,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
						m_pITableFrame->SendLookonData(wUserIndex,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
					}
				}
				else if (m_pITableFrame->GetGameStatus()==GS_GAME_END)
				{
					m_bBankerQiut = true;
				}
				else if (m_pITableFrame->GetGameStatus()==GS_COMPARE_END)
				{
					LONGLONG TempStartScore=0;
					for ( WORD wUserChairID = 0; wUserChairID < GAME_PLAYER; ++wUserChairID )
					{
						IServerUserItem *pIServerUserItem = m_pITableFrame->GetTableUserItem(wUserChairID);
						if ( pIServerUserItem == NULL||(QueryBuckleServiceCharge(wUserChairID)==false)) continue;

						//�����
						if (!pIServerUserItem->IsAndroidUser())
						{
							TempStartScore -= m_lUserWinScore[wUserChairID];
							m_lExchangeGold[wUserChairID]+=m_lUserReturnScore[wUserChairID];
						}
					}
					if(!(m_pServerContro!=NULL&&m_bControl))
						m_StorageStart+=TempStartScore*m_nExchange;

					//������Ϸ
					m_pITableFrame->ConcludeGame(GS_GAME_FREE);

					if (m_wCurrentBanker!=INVALID_CHAIR)
					{
						m_lExchangeGold[m_wCurrentBanker]+=m_lBankerCurGameScore;
					}

					//�۳�ׯ�ҷ���			
					if(m_lUserWinScore[m_wCurrentBanker]!=0L)
					{
						//��������
						LONGLONG static cbRevenue=m_pGameServiceOption->wRevenueRatio;
						LONGLONG lTotalScore=0;
						if (0 < m_lUserWinScore[m_wCurrentBanker])
						{
							double fRevenuePer=double(cbRevenue/1000.0);
							lTotalScore=m_lUserWinScore[m_wCurrentBanker]*m_nExchange;
							m_lUserRevenue[wChairID]  = LONGLONG(lTotalScore*fRevenuePer);
							lTotalScore -= m_lUserRevenue[m_wCurrentBanker];
						}
						else if(m_lUserWinScore[m_wCurrentBanker]<0)
						{
							lTotalScore=m_lUserWinScore[m_wCurrentBanker]*m_nExchange;
							if(pIServerUserItem !=NULL)
							{
								if(-lTotalScore>pIServerUserItem->GetUserScore())
								{
									CString str;
									str.Format(TEXT("���ID:%d,�ǳ�:%s,�����쳣��Я�����:%I64d,���:%I64d"),pIServerUserItem->GetUserID(),pIServerUserItem->GetNickName(),pIServerUserItem->GetUserScore(),lTotalScore);
									CTraceService::TraceString(str,TraceLevel_Exception);
									lTotalScore = -pIServerUserItem->GetUserScore();
								}
							}

							m_lUserRevenue[wChairID]  = 0;
						}
						tagScoreInfo ScoreInfo;
						ZeroMemory(&ScoreInfo,sizeof(ScoreInfo));
						ScoreInfo.cbType =(lTotalScore>0L)?SCORE_TYPE_WIN:SCORE_TYPE_LOSE;
						ScoreInfo.lRevenue = m_lUserRevenue[m_wCurrentBanker];
						ScoreInfo.lScore = lTotalScore;
						m_pITableFrame->WriteUserScore(m_wCurrentBanker,ScoreInfo);
						m_lExchangeGold[m_wCurrentBanker] = 0;
						m_lUserWinScore[m_wCurrentBanker] = 0;
						m_lUserTotalScore[m_wCurrentBanker]=0;
					}

					//�л�ׯ��
					ChangeBanker(true);

					//����ʱ��
					m_dwJettonTime=(DWORD)time(NULL);
					m_pITableFrame->SetGameTimer(IDI_FREE,m_cbFreeTime*1000,1,0L);

					//������Ϣ
					CMD_S_GameFree GameFree;
					ZeroMemory(&GameFree,sizeof(GameFree));			

					GameFree.cbTimeLeave = m_cbFreeTime;
					GameFree.lBankerWinScore = m_lBankerWinScore;
					GameFree.lStorageStart=m_StorageStart;			
					GameFree.nListUserCount=m_ApplyUserArray.GetCount()-1;			
					CopyMemory(GameFree.lExchangeGold,m_lExchangeGold,sizeof(GameFree.lExchangeGold));
					m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_FREE,&GameFree,sizeof(GameFree));
					m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_FREE,&GameFree,sizeof(GameFree));

					return true;
				}
				//�۳�����			
				if(m_lUserWinScore[m_wCurrentBanker]!=0L)
				{
					//��������
					LONGLONG static cbRevenue=m_pGameServiceOption->wRevenueRatio;
					LONGLONG lTotalScore=0;
					if (0 < m_lUserWinScore[m_wCurrentBanker])
					{
						double fRevenuePer=double(cbRevenue/1000.0);
						lTotalScore=m_lUserWinScore[m_wCurrentBanker]*m_nExchange;
						m_lUserRevenue[wChairID]  = LONGLONG(lTotalScore*fRevenuePer);
						lTotalScore -= m_lUserRevenue[m_wCurrentBanker];
					}
					else if(m_lUserWinScore[m_wCurrentBanker]<0)
					{
						lTotalScore=m_lUserWinScore[m_wCurrentBanker]*m_nExchange;
						if(pIServerUserItem != NULL)
						{
							if(-lTotalScore>pIServerUserItem->GetUserScore())
							{
								CString str;
								str.Format(TEXT("���ID:%d,�ǳ�:%s,�����쳣��Я�����:%I64d,���:%I64d"),pIServerUserItem->GetUserID(),pIServerUserItem->GetNickName(),pIServerUserItem->GetUserScore(),lTotalScore);
								CTraceService::TraceString(str,TraceLevel_Exception);
								lTotalScore = -pIServerUserItem->GetUserScore();
							}
						}

						m_lUserRevenue[wChairID]  = 0;
					}
					tagScoreInfo ScoreInfo;
					ZeroMemory(&ScoreInfo,sizeof(ScoreInfo));
					ScoreInfo.cbType =(lTotalScore>0L)?SCORE_TYPE_WIN:SCORE_TYPE_LOSE;
					ScoreInfo.lRevenue = m_lUserRevenue[m_wCurrentBanker];
					ScoreInfo.lScore = lTotalScore;
					m_pITableFrame->WriteUserScore(m_wCurrentBanker,ScoreInfo);
					m_lExchangeGold[m_wCurrentBanker] = 0;
					m_lUserWinScore[m_wCurrentBanker] = 0;
					m_lUserTotalScore[m_wCurrentBanker]=0;
				}

				//�л�ׯ��
				ChangeBanker(true);
			}
			catch(...)
			{
				CString str;
				str.Format(TEXT("OnEventGameConclude,Reason:%s,Banker"),(cbReason==GER_USER_LEAVE)?TEXT("GER_USER_LEAVE"):TEXT("GER_NETWORK_ERROR") );
				CTraceService::TraceString(str,TraceLevel_Exception);
			}
			return true;
		}
	}

	return false;
}

//���ͳ���
bool CTableFrameSink::OnEventSendGameScene(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbGameStatus, bool bSendSecret)
{
	switch (cbGameStatus)
	{
	case GS_GAME_FREE:			//����״̬
		{
			//���ͼ�¼
			SendGameRecord(pIServerUserItem);

			//��������
			CMD_S_StatusFree StatusFree;
			ZeroMemory(&StatusFree,sizeof(StatusFree));			

			//������Ϣ
			StatusFree.lApplyBankerCondition = m_lApplyBankerCondition;
			StatusFree.lAreaLimitScore = m_lAreaLimitScore;
			StatusFree.CheckImage = m_CheckImage;
			StatusFree.nExchange=m_nExchange;
			StatusFree.lStartStorage=m_lStartStorage;

			m_lExchangeGold[wChairID]=0;

			//ׯ����Ϣ
			StatusFree.bEnableSysBanker=m_bEnableSysBanker;
			StatusFree.wBankerUser=m_wCurrentBanker;	
			StatusFree.cbBankerTime=m_wBankerTime;
			StatusFree.lBankerWinScore=m_lBankerWinScore;
			StatusFree.lBankerScore = 0x7fffffffffffffff;
			if (m_wCurrentBanker!=INVALID_CHAIR)
			{
				IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(m_wCurrentBanker);
				StatusFree.lBankerScore=m_lExchangeGold[m_wCurrentBanker];
			}
			
			//�����Ϣ
			if (pIServerUserItem->GetUserStatus()!=US_LOOKON)
			{
				StatusFree.lUserMaxScore=min(m_lExchangeGold[wChairID],m_lUserLimitScore); 
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

				//���������� �Զ�ȫ���һ�����
// 				srand(GetTickCount());
// 				int nTimes = rand()%9+2;//2-10��
				m_lExchangeGold[wChairID] += pIServerUserItem->GetUserScore()/m_nExchange;
			}
			//���ͳ���
			bool bSuccess = m_pITableFrame->SendGameScene(pIServerUserItem,&StatusFree,sizeof(StatusFree));
// #ifndef DEBUG
// 			//������ʾ
// 			TCHAR szTipMsg[128];
// 			_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("��������ׯ����Ϊ��%I64d,��������Ϊ��%I64d,�������Ϊ��%I64d"),m_lApplyBankerCondition,
// 				m_lAreaLimitScore,m_lUserLimitScore);
// 
// 			m_pITableFrame->SendGameMessage(pIServerUserItem,szTipMsg,SMT_CHAT);
// #endif
			//����������
			SendApplyUser(pIServerUserItem);
			//Ȩ���ж�
			if(CUserRight::IsGameCheatUser(pIServerUserItem->GetUserRight()))
			{
				CMD_S_ADMIN_STORAGE_INFO StorageInfo;
				ZeroMemory(&StorageInfo,sizeof(StorageInfo));
				StorageInfo.lCurrentStorage = m_StorageStart;
				StorageInfo.lCurrentDeduct = m_StorageDeduct;
				CopyMemory(StorageInfo.lMaxStorage,m_lStorageMax,sizeof(m_lStorageMax));
				CopyMemory(StorageInfo.wStorageMul,m_wStorageMul,sizeof(m_wStorageMul));
				CopyMemory(StorageInfo.lUserTotalScore,m_lUserTotalScore,sizeof(m_lUserTotalScore));
				m_pITableFrame->SendTableData(wChairID,SUB_S_ADMIN_STORAGE_INFO,&StorageInfo,sizeof(StorageInfo));
				m_pITableFrame->SendLookonData(wChairID,SUB_S_ADMIN_STORAGE_INFO,&StorageInfo,sizeof(StorageInfo));
			}
			return bSuccess;
		}
	case GS_COMPARE_END:
	case GS_PLACE_JETTON:		//��Ϸ״̬
	case GS_GAME_END:			//����״̬
		{
			//���ͼ�¼
			SendGameRecord(pIServerUserItem);		

			//��������
			CMD_S_StatusPlay StatusPlay={0};

			//ȫ����ע
			CopyMemory(StatusPlay.lAllJettonScore,m_lAllJettonScore,sizeof(StatusPlay.lAllJettonScore));

			m_lExchangeGold[wChairID]=0;

			//�����ע
			if (pIServerUserItem->GetUserStatus()!=US_LOOKON)
			{
				for (int nAreaIndex=1; nAreaIndex<=AREA_COUNT; ++nAreaIndex)
					StatusPlay.lUserJettonScore[nAreaIndex] = m_lUserJettonScore[nAreaIndex][wChairID];

				//�����ע
				StatusPlay.lUserMaxScore=min(m_lExchangeGold[wChairID],m_lUserLimitScore);
			}

			//������Ϣ
			StatusPlay.lApplyBankerCondition=m_lApplyBankerCondition;		
			StatusPlay.lAreaLimitScore=m_lAreaLimitScore;		
			StatusPlay.lStartStorage = m_lStartStorage;
			//ׯ����Ϣ
			StatusPlay.bEnableSysBanker=m_bEnableSysBanker;
			StatusPlay.wBankerUser=m_wCurrentBanker;			
			StatusPlay.cbBankerTime=m_wBankerTime;
			StatusPlay.lBankerWinScore=m_lBankerWinScore;	
			StatusPlay.CheckImage = m_CheckImage;
			StatusPlay.nExchange=m_nExchange;
			StatusPlay.lBankerScore = 0x7fffffffffffffff;
			if (m_wCurrentBanker!=INVALID_CHAIR)
			{
				StatusPlay.lBankerScore=m_lBankerScore;
			}	

			//ȫ����Ϣ
			DWORD dwPassTime=(DWORD)time(NULL)-m_dwJettonTime;
			int	nTotalTime = (cbGameStatus==GS_PLACE_JETTON?m_cbBetTime:m_cbEndTime);
			StatusPlay.cbTimeLeave=(BYTE)(nTotalTime-__min(dwPassTime,(DWORD)nTotalTime));
			StatusPlay.cbGameStatus=m_pITableFrame->GetGameStatus();						

			//�����ж�
			if (cbGameStatus==GS_GAME_END)
			{
				if(m_nProbability==2||m_nProbability==3||m_nProbability==4)
					nTotalTime = m_cbEndTime+5;
				else
					nTotalTime = m_cbEndTime+m_cbAddTime*m_nRand;
				StatusPlay.cbTimeLeave=(BYTE)(nTotalTime-__min(dwPassTime,m_cbEndTime));

				//���óɼ�
				StatusPlay.lEndUserScore=m_lUserWinScore[wChairID];

				//��������
				StatusPlay.lEndUserReturnScore=m_lUserReturnScore[wChairID];

				//����˰��
				if (m_lUserRevenue[wChairID]>0) StatusPlay.lEndRevenue=m_lUserRevenue[wChairID];
				else if (m_wCurrentBanker!=INVALID_CHAIR) StatusPlay.lEndRevenue=m_lUserRevenue[m_wCurrentBanker];
				else StatusPlay.lEndRevenue=0;

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
				//���������� �Զ��һ�����
				m_lExchangeGold[wChairID] += pIServerUserItem->GetUserScore()/m_nExchange;
			}
			//���ͳ���
			bool bSuccess = m_pITableFrame->SendGameScene(pIServerUserItem,&StatusPlay,sizeof(StatusPlay));
// #ifndef DEBUG
// 			//������ʾ
// 			TCHAR szTipMsg[128];
// 			_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("��������ׯ����Ϊ��%I64d,��������Ϊ��%I64d,�������Ϊ��%I64d"),m_lApplyBankerCondition,
// 				m_lAreaLimitScore,m_lUserLimitScore);
// 			
// 			m_pITableFrame->SendGameMessage(pIServerUserItem,szTipMsg,SMT_CHAT);
// #endif	
			//����������
			SendApplyUser( pIServerUserItem );
			//Ȩ���ж�
			if(CUserRight::IsGameCheatUser(pIServerUserItem->GetUserRight()))
			{
				CMD_S_ADMIN_STORAGE_INFO StorageInfo;
				ZeroMemory(&StorageInfo,sizeof(StorageInfo));
				StorageInfo.lCurrentStorage = m_StorageStart;
				StorageInfo.lCurrentDeduct = m_StorageDeduct;
				CopyMemory(StorageInfo.lMaxStorage,m_lStorageMax,sizeof(m_lStorageMax));
				CopyMemory(StorageInfo.wStorageMul,m_wStorageMul,sizeof(m_wStorageMul));
				CopyMemory(StorageInfo.lUserTotalScore,m_lUserTotalScore,sizeof(m_lUserTotalScore));
				m_pITableFrame->SendTableData(wChairID,SUB_S_ADMIN_STORAGE_INFO,&StorageInfo,sizeof(StorageInfo));
				m_pITableFrame->SendLookonData(wChairID,SUB_S_ADMIN_STORAGE_INFO,&StorageInfo,sizeof(StorageInfo));
			}
			return bSuccess;
		}
	}

	return false;
}

//�´�Сʱ��
bool CTableFrameSink::OnCompareBigSmall()
{
	//������Ϣ
	CMD_S_Compare Compare;
	ZeroMemory(&Compare,sizeof(Compare));

	//���ͻ���
	Compare.cbTimeLeave=m_cbCompareTime;

	//������Ϣ					
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_COMPARE,&Compare,sizeof(Compare));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_COMPARE,&Compare,sizeof(Compare));
	

	return true;
}

//��ʱ���¼�
bool CTableFrameSink::OnTimerMessage(DWORD wTimerID, WPARAM wBindParam)
{
	switch (wTimerID)
	{
	case IDI_FREE:		//����ʱ��
		{
			//��ʼ��Ϸ
			//m_pITableFrameControl->StartGame();
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
				if(m_nProbability==2||m_nProbability==3||m_nProbability==4)
					m_pITableFrame->SetGameTimer(IDI_GAME_END,(m_cbEndTime+5)*1000,1,0L);
				else
					m_pITableFrame->SetGameTimer(IDI_GAME_END,(m_cbEndTime+m_cbAddTime*m_nRand)*1000,1,0L);
			}

			return true;
		}
	case IDI_GAME_END:			//������Ϸ
		{
			//ׯ�����ܺ󲻽���´�С���ڣ�ֱ�ӽ���
			if (m_bBankerQiut)
				{
					LONGLONG TempStartScore=0;
					for ( WORD wUserChairID = 0; wUserChairID < GAME_PLAYER; ++wUserChairID )
					{
						IServerUserItem *pIServerUserItem = m_pITableFrame->GetTableUserItem(wUserChairID);
						if ( pIServerUserItem == NULL||(QueryBuckleServiceCharge(wUserChairID)==false)) continue;

						//�����
						if (!pIServerUserItem->IsAndroidUser())
						{
							TempStartScore -= (m_lUserWinScore[wUserChairID]+m_lUserRevenue[wUserChairID]);
							m_lExchangeGold[wUserChairID]+=m_lUserReturnScore[wUserChairID];
						}
					}
					if(!(m_pServerContro!=NULL&&m_bControl))
						m_StorageStart+=TempStartScore*m_nExchange;

					//������Ϸ
					m_pITableFrame->ConcludeGame(GS_GAME_FREE);

					if (m_wCurrentBanker!=INVALID_CHAIR)
					{
						m_lExchangeGold[m_wCurrentBanker]+=m_lBankerCurGameScore;
					}

					//�л�ׯ��
					ChangeBanker(false);

					//����ʱ��
					m_dwJettonTime=(DWORD)time(NULL);
					m_pITableFrame->SetGameTimer(IDI_FREE,m_cbFreeTime*1000,1,0L);

					//������Ϣ
					CMD_S_GameFree GameFree;
					ZeroMemory(&GameFree,sizeof(GameFree));			

					GameFree.cbTimeLeave = m_cbFreeTime;
					GameFree.lBankerWinScore = m_lBankerWinScore;
					GameFree.lStorageStart=m_StorageStart;			
					GameFree.nListUserCount=m_ApplyUserArray.GetCount();		
					CopyMemory(GameFree.lExchangeGold,m_lExchangeGold,sizeof(GameFree.lExchangeGold));
					m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_FREE,&GameFree,sizeof(GameFree));
					m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_FREE,&GameFree,sizeof(GameFree));
				}
			else
			{
				//����״̬
				m_pITableFrame->SetGameStatus(GS_COMPARE_END);

				//�´�Сʱ��
				OnCompareBigSmall();
				//����ʱ��
				m_dwJettonTime=(DWORD)time(NULL);
				m_pITableFrame->SetGameTimer(IDI_SMALL_BIG,m_cbCompareTime*1000,1,0L);
			}			

			return true;
		}
	case IDI_SMALL_BIG://����Сʱ��
		{			
			LONGLONG TempStartScore=0;
			for ( WORD wUserChairID = 0; wUserChairID < GAME_PLAYER; ++wUserChairID )
			{
				IServerUserItem *pIServerUserItem = m_pITableFrame->GetTableUserItem(wUserChairID);
				if ( pIServerUserItem == NULL||(QueryBuckleServiceCharge(wUserChairID)==false)) continue;

				//�����
				if (!pIServerUserItem->IsAndroidUser())
				{
					TempStartScore -= (m_lUserWinScore[wUserChairID]+m_lUserRevenue[wUserChairID]);
					m_lExchangeGold[wUserChairID]+=m_lUserReturnScore[wUserChairID];
				}
			}
			if(!(m_pServerContro!=NULL&&m_bControl))
				m_StorageStart+=TempStartScore*m_nExchange;

			//������Ϸ
			m_pITableFrame->ConcludeGame(GS_GAME_FREE);

			if (m_wCurrentBanker!=INVALID_CHAIR)
				{
					m_lExchangeGold[m_wCurrentBanker]+=m_lBankerCurGameScore;
				}

			//�л�ׯ��
			ChangeBanker(false);

			//����ʱ��
			m_dwJettonTime=(DWORD)time(NULL);
			m_pITableFrame->SetGameTimer(IDI_FREE,m_cbFreeTime*1000,1,0L);

			//������Ϣ
			CMD_S_GameFree GameFree;
			ZeroMemory(&GameFree,sizeof(GameFree));			

			GameFree.cbTimeLeave = m_cbFreeTime;
			GameFree.lBankerWinScore = m_lBankerWinScore;
			GameFree.lStorageStart=m_StorageStart;			
			GameFree.nListUserCount=m_ApplyUserArray.GetCount()-1;			
			CopyMemory(GameFree.lExchangeGold,m_lExchangeGold,sizeof(GameFree.lExchangeGold));
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_FREE,&GameFree,sizeof(GameFree));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_FREE,&GameFree,sizeof(GameFree));
			
			return true;
		}
	}

	return false;
}

//��Ϸ��Ϣ����
bool CTableFrameSink::OnGameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem)
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
			CMD_C_PlaceJetton * pPlaceJetton=(CMD_C_PlaceJetton *)pData;
			return OnUserPlaceJetton(pUserData->wChairID,pPlaceJetton->cbJettonArea,pPlaceJetton->lJettonScore);
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
	case SUB_C_CONTINUE_CARD:		//��������
		{
// 			//�û�Ч��
// 			tagUserInfo * pUserData=pIServerUserItem->GetUserInfo();
// 			if (pUserData->cbUserStatus==US_LOOKON) return true;
// 			if (pUserData->wChairID!=m_wCurrentBanker) return true;
// 			if (m_cbLeftCardCount < 8) return true;
// 
// 			//���ñ���
// 			m_bContiueCard=true;
// 
// 			//������Ϣ
// 			SendGameMessage(pUserData->wChairID,TEXT("���óɹ�����һ�ֽ��������ƣ�"));

			return true;
		}
	case SUB_C_CHECK_IMAGE:		//�û�ѡ�񱳾�ͼ
		{
// 			ASSERT(wDataSize==sizeof(CMD_C_CheckImage));
// 			if (wDataSize!=sizeof(CMD_C_CheckImage)) return false;
// 
// 			CMD_C_CheckImage * pCheckImage=(CMD_C_CheckImage *)pData;
// 			this->m_CheckImage = pCheckImage->Index;
// 			m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_CHECK_IMAGE, pCheckImage, sizeof(CMD_C_CheckImage));
// 			m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_CHECK_IMAGE, pCheckImage, sizeof(CMD_C_CheckImage));
			return true;

		}
	case SUB_C_ADMIN_COMMDN:
		{
			ASSERT(wDataSize==sizeof(CMD_C_ControlApplication));
			if(wDataSize!=sizeof(CMD_C_ControlApplication)) return false;

			//Ȩ���ж�
			if(CUserRight::IsGameCheatUser(pIServerUserItem->GetUserRight())==false)
				return false;
			if (  m_pServerContro == NULL )
				return false;

			//��¼����
			CMD_C_ControlApplication* pControlApplication = (CMD_C_ControlApplication*)pData;
			CString strBuf;

			if (pControlApplication->cbControlAppType == C_CA_UPDATE)
			{			
				strBuf.Format(TEXT("����������ע����:%d,���ÿ��ƴ���:%d\n"), pControlApplication->cbControlArea,pControlApplication->cbControlTimes);
				AdminOperateInfo(strBuf,pIServerUserItem);
			}
			else if(pControlApplication->cbControlAppType == C_CA_SET)
			{
				strBuf.Format(TEXT("������ע����:%d,���ÿ��ƴ���:%d\n"),pControlApplication->cbControlArea,pControlApplication->cbControlTimes);
				AdminOperateInfo(strBuf,pIServerUserItem);
			}
			return m_pServerContro->ServerControl(wSubCmdID, pData, wDataSize, pIServerUserItem, m_pITableFrame);
		}
	case SUB_C_STORAGE:
		{
			ASSERT(wDataSize==sizeof(CMD_C_FreshStorage));
			if(wDataSize!=sizeof(CMD_C_FreshStorage)) return false;

			//Ȩ���ж�
			if(CUserRight::IsGameCheatUser(pIServerUserItem->GetUserRight())==false)
				return false;

			CMD_C_FreshStorage *FreshStorage=(CMD_C_FreshStorage *)pData;
			m_StorageStart=FreshStorage->lStorageStart;
			CString szBuf;
			szBuf.Format(TEXT("���Ŀ��ֵΪ��%I64d\n"),m_StorageStart);
			AdminOperateInfo(szBuf,pIServerUserItem);

			return true;
		}
	case SUB_C_DEUCT:
		{
			ASSERT(wDataSize==sizeof(CMD_C_FreshDeuct));
			if(wDataSize!=sizeof(CMD_C_FreshDeuct)) return false;

			//Ȩ���ж�
			if(CUserRight::IsGameCheatUser(pIServerUserItem->GetUserRight())==false)
				return false;

			CMD_C_FreshDeuct *FreshDeuct=(CMD_C_FreshDeuct *)pData;
			m_StorageDeduct=FreshDeuct->lStorageDeuct;

			CString szBuf;
			szBuf.Format(TEXT("����˥��ֵΪ��%I64d\n"),m_StorageDeduct);
			AdminOperateInfo(szBuf,pIServerUserItem);
			return true;
		}
	case SUB_C_STORAGEINFO:
		{
			ASSERT(wDataSize==sizeof(CMD_C_StorageInfo));
			if(wDataSize!=sizeof(CMD_C_StorageInfo)) return false;

			//Ȩ���ж�
			if(CUserRight::IsGameCheatUser(pIServerUserItem->GetUserRight())==false)
				return false;
			CMD_C_StorageInfo *pStorageInfo = (CMD_C_StorageInfo *)pData;
			CopyMemory(m_lStorageMax,pStorageInfo->lMaxStorage,sizeof(m_lStorageMax));
			CopyMemory(m_wStorageMul,pStorageInfo->wStorageMul,sizeof(m_wStorageMul));
			
			CString szBuf;
			szBuf.Format(TEXT("���Ŀ������ֵ�Լ�ϵͳ��泬������ʱ�·ָ��ʣ���һ�׶Σ�%I64d���·ָ���Ϊ��%d%%;�ڶ��׶Σ�%I64d���·ָ���Ϊ%d%%\n"),m_lStorageMax[0],m_wStorageMul[0],m_lStorageMax[1],m_wStorageMul[1]);
			AdminOperateInfo(szBuf,pIServerUserItem);

			return true;

		}
	case SUB_C_CHANGE_GOLD:
		{
			ASSERT(wDataSize==sizeof(CMD_C_ChangeGold));
			if(wDataSize!=sizeof(CMD_C_ChangeGold)) return false;
			
			//��Ϣ����
			CMD_C_ChangeGold * pChangeGold=(CMD_C_ChangeGold *)pData;
			//�û�Ч��
			tagUserInfo * pUserData=pIServerUserItem->GetUserInfo();
			
			return OnUserChangeGold(pUserData->wChairID,pChangeGold->lChangeGold);
		}
	case SUB_C_GUESS:
		{
			ASSERT(wDataSize==sizeof(CMD_C_Guess));
			if(wDataSize!=sizeof(CMD_C_Guess)) return false;

			//��Ϣ����
			CMD_C_Guess * pGuess=(CMD_C_Guess *)pData;
			//�û�Ч��
			tagUserInfo * pUserData=pIServerUserItem->GetUserInfo();

			return OnUserGuess(pUserData->wChairID,pGuess->bBig);
		}
	}

	return false;
}

//�����Ϣ����
bool  CTableFrameSink::OnFrameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	return false;
}

//�����¼�
bool CTableFrameSink::OnGameDataBase(WORD wRequestID, VOID * pData, WORD wDataSize)
{
	return false;
}

//�û�����
bool CTableFrameSink::OnActionUserSitDown(WORD wChairID,IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	//����ʱ��
	if ((bLookonUser==false)&&(m_dwJettonTime==0L))
	{
		m_dwJettonTime=(DWORD)time(NULL);
		m_pITableFrame->SetGameTimer(IDI_FREE,m_cbFreeTime*1000,1,NULL);
		m_pITableFrame->SetGameStatus(GS_GAME_FREE);
	}

	////������ʾ
	//TCHAR szTipMsg[128];
	//_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("��������ׯ����Ϊ��%I64d,��������Ϊ��%I64d,�������Ϊ��%I64d"),m_lApplyBankerCondition,
	//	m_lAreaLimitScore,m_lUserLimitScore);
	//m_pITableFrame->SendGameMessage(pIServerUserItem,szTipMsg,SMT_CHAT);

	return true;
}

//�û�����
bool CTableFrameSink::OnActionUserStandUp(WORD wChairID,IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	//��¼�ɼ�
	if (bLookonUser==false)
	{
		//�л�ׯ��
		if (pIServerUserItem->GetChairID()==m_wCurrentBanker)
		{
			ChangeBanker(true);
			m_bContiueCard=false;
		}

		//ȡ������
		for (WORD i=0; i<m_ApplyUserArray.GetCount(); ++i)
		{
			if (pIServerUserItem->GetChairID()!=m_ApplyUserArray[i]) continue;

			//ɾ�����
			m_ApplyUserArray.RemoveAt(i);

			//�������
			CMD_S_CancelBanker CancelBanker;
			ZeroMemory(&CancelBanker,sizeof(CancelBanker));

			//���ñ���
			CancelBanker.wCancelUser=wChairID;			

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
	ASSERT((cbJettonArea<=AREA_COUNT+1 && cbJettonArea>=1)&&(lJettonScore>0L));
	if ((cbJettonArea>=AREA_COUNT+1)||(lJettonScore<=0L) || cbJettonArea<1 || lJettonScore>m_lExchangeGold[wChairID] ||
		(m_lUserJettonScore[cbJettonArea][wChairID]+lJettonScore) <0L || (m_lAllJettonScore[cbJettonArea]+lJettonScore)<0L )
		{
			return false;
		}

	//��ע�Ϸ��Լ�� 
	IServerUserItem * pIServerUserItem=m_pITableFrame->GetTableUserItem(wChairID);
	if(pIServerUserItem != NULL)
		{
			LONGLONG lTotalUserBet = 0L;
			for(int i=0; i<AREA_COUNT+1;i++)	lTotalUserBet+=m_lUserJettonScore[i][wChairID];

			if(m_nExchange == 0)  m_nExchange=1;
			if( (lTotalUserBet+lJettonScore) > pIServerUserItem->GetUserScore()/m_nExchange
				|| (lJettonScore+lTotalUserBet) < 0L  ) return false;
		}
	else 
		return false;

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
		//IServerUserItem * pIServerUserItem=m_pITableFrame->GetTableUserItem(wChairID);
	LONGLONG lJettonCount=0L;
	for (int nAreaIndex=1; nAreaIndex<=AREA_COUNT; ++nAreaIndex) lJettonCount += m_lUserJettonScore[nAreaIndex][wChairID];
	//��һ���
	LONGLONG lUserScore = m_lExchangeGold[wChairID];

	//�ɹ���ʶ
	bool bPlaceJettonSuccess=true;
	//�Ϸ�У��
	if (lUserScore <= 0) bPlaceJettonSuccess=false;
	if (m_lUserLimitScore < lJettonCount + lJettonScore) bPlaceJettonSuccess=false;
	BYTE diMultiple[AREA_COUNT]={5,10,10,10,20,20,20,50};
	LONGLONG lUserTemp[AREA_COUNT+1][GAME_PLAYER];
	ZeroMemory(lUserTemp,sizeof(lUserTemp));
	CopyMemory(lUserTemp,m_lUserJettonScore,sizeof(m_lUserJettonScore));
	lUserTemp[cbJettonArea][wChairID] += lJettonScore;
	if ((m_wCurrentBanker!=INVALID_CHAIR) && (lUserTemp[cbJettonArea][wChairID] > m_lExchangeGold[m_wCurrentBanker]/diMultiple[cbJettonArea-1]))
		{
			bPlaceJettonSuccess=false;
		}
	//���������ע�ۺϳ���ׯ�����ʵĻ� ��עʧ��
	if(m_wCurrentBanker != INVALID_CHAIR)
		{
			if((m_lAllJettonScore[cbJettonArea]+lJettonScore) > m_lExchangeGold[m_wCurrentBanker]/diMultiple[cbJettonArea-1])
			{
				bPlaceJettonSuccess=false;
			}
		}	
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
				for (int i = 0; i <= AREA_COUNT; i++)
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
				m_lExchangeGold[wChairID]-=lJettonScore;
			}	
		}
	else
		{
			bPlaceJettonSuccess=false;
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
			PlaceJetton.bAndroid = pIServerUserItem->IsAndroidUser()? TRUE : FALSE;

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
bool CTableFrameSink::DispatchTableCard()
{
	ZeroMemory(m_cbTableCardArray,sizeof(m_cbTableCardArray));
	m_nProbability=0;
	m_nRand=0;
	//1,13�����ӡ�2,14��������24С����,3С����4������5,11,17,23��ƻ����6Сƻ����7,19�����ʡ�8�����ϡ�9С���ϡ�10��LUCK��12С���ӡ�15С˫7��16��˫7��18С���ʡ�20�����ǡ�21С���ǡ�22��LUCK
	INT cbControlArea[24] = { 1, 13,	2, 14, 24,    3,   4,	5, 11, 17, 23,   6,		7, 19,		8,   9,		10,		12,		15,   16, 18, 20, 21,  22};
	INT cbnChance[24]	  = { 6, 6,     5, 5,  7,     1,   1,	4,  4, 4,  4,	 7,		6, 6,		7,   7,		m_nLucky,		7,		7,	   7,  7,  7,  7,  m_nLucky};

	m_GameLogic.ChaosArray(cbControlArea, CountArray(cbControlArea), cbnChance, CountArray(cbnChance));

	//�������
	DWORD wTick = GetTickCount();

	//���ʺ�ֵ
	INT nChanceAndValue = 0;
	for ( int n = 0; n < CountArray(cbnChance); ++n )
		nChanceAndValue += cbnChance[n];

	INT nMuIndex = 0;
	int nRandNum = 0;					//�������
	
	nRandNum = (rand() + wTick ) % nChanceAndValue;
	for (int j = 0; j < CountArray(cbnChance); j++)
	{
		nRandNum -= cbnChance[j];
		if (nRandNum < 0)
		{
			nMuIndex = j;
			break;
		}
	}

	m_cbTableCardArray[0] = cbControlArea[nMuIndex];
	m_cbCardCount[0] = 1;
	
	if (m_wCurrentBanker!=INVALID_CHAIR)
	{		
		if (m_lExchangeGold[m_wCurrentBanker] < 6000000 && (m_cbTableCardArray[0]==10||m_cbTableCardArray[0]==22))
		{
			while(m_cbTableCardArray[0]==10||m_cbTableCardArray[0]==22)
			{
				m_cbTableCardArray[0] = rand()%24+1;
			}			
		}
	}


	if(m_cbTableCardArray[0]==10||m_cbTableCardArray[0]==22)//��LUCK,��LUCK
	{
		m_nProbability=rand()%5+1;		
		if(m_nProbability==1)//��ǹ
		{
			int nCount=0;
			m_nRand=rand()%4+1;
			do
			{
				nCount++;
				m_cbTableCardArray[nCount]=rand()%24+1;
				if(m_cbTableCardArray[nCount]==10||m_cbTableCardArray[nCount]==22)
					m_cbTableCardArray[nCount]=rand()%9+1;
			}while(nCount!=m_nRand);
		}
		if(m_nProbability==2)//����Ԫ
		{
			m_cbTableCardArray[1]=8;
			m_cbTableCardArray[2]=20;
			m_cbTableCardArray[3]=16;
		}
		if(m_nProbability==3)//С��Ԫ
		{
			m_cbTableCardArray[1]=1;
			m_cbTableCardArray[2]=7;
			m_cbTableCardArray[3]=14;
		}
		if(m_nProbability==4)//����ϲ
		{
			m_cbTableCardArray[1]=5;
			m_cbTableCardArray[2]=11;
			m_cbTableCardArray[3]=17;
			m_cbTableCardArray[4]=23;
		}
		if(m_nProbability==5)//ͨ��
		{

		}
	}	

	//���Ʊ�־
	m_bContiueCard = false;

	return true;
}

//�һ�����
bool CTableFrameSink::OnUserChangeGold(WORD wChairID,LONGLONG lGold)
{
	ASSERT((wChairID!=INVALID_CHAIR)||(lGold>0L));

	//�һ�����Ϸ���У�� 
	IServerUserItem * pIServerUserItem=m_pITableFrame->GetTableUserItem(wChairID);
	if(pIServerUserItem != NULL)
	{
		if(m_nExchange == 0)  m_nExchange = 1;

		if((m_lExchangeGold[wChairID]+lGold)>pIServerUserItem->GetUserScore()/m_nExchange 
			&& pIServerUserItem->IsAndroidUser()==false) return false;
	}
	else
		return false;
	
	if(pIServerUserItem->IsAndroidUser()==false)	m_lExchangeGold[wChairID]+=lGold;
	else m_lExchangeGold[wChairID] = pIServerUserItem->GetUserScore()/m_nExchange;
	//�������
	CMD_S_ChangeGold ChangeGold;
	ZeroMemory(&ChangeGold,sizeof(ChangeGold));

	//���ñ���
	CopyMemory(ChangeGold.lExchangeGold,m_lExchangeGold,sizeof(m_lExchangeGold));
	ChangeGold.wChairID = wChairID;

	//������Ϣ
	m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_CHANGE_GOLD, &ChangeGold, sizeof(ChangeGold));
	m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_CHANGE_GOLD, &ChangeGold, sizeof(ChangeGold));


	return true;
}

//�´�С
bool CTableFrameSink::OnUserGuess(WORD wChairID,BOOL bBig)
{	
	ASSERT(wChairID!=INVALID_CHAIR);	
	IServerUserItem * pIServerUserItem=m_pITableFrame->GetTableUserItem(wChairID);
	if(pIServerUserItem == NULL)  return false;

	int nCount=0;
	int nGuess=rand()%2;
	if(bBig==nGuess)
	{
		if(m_StorageStart-m_lUserReturnScore[wChairID]*2*m_nExchange < 0 && m_wCurrentBanker == INVALID_CHAIR)
		{
			m_lUserWinScore[wChairID]-=m_lUserTempScore[wChairID];
			if(m_wCurrentBanker!=INVALID_CHAIR)
				m_lUserWinScore[m_wCurrentBanker]+=m_lUserTempScore[wChairID];
			m_lBankerWinScore+=m_lUserTempScore[wChairID];
			m_lBankerCurGameScore+=m_lUserTempScore[wChairID];
			m_lUserReturnScore[wChairID]=0;
			if(bBig)
			{
				nCount=rand()%7+1;
			}
			else
			{
				nCount=rand()%7+8;
			}
		}
		else
		{
			m_lUserWinScore[wChairID]+=m_lUserReturnScore[wChairID];			
			if(m_wCurrentBanker!=INVALID_CHAIR)
				m_lUserWinScore[m_wCurrentBanker]-=m_lUserReturnScore[wChairID];
			m_lBankerWinScore-=m_lUserReturnScore[wChairID];
			m_lBankerCurGameScore-=m_lUserReturnScore[wChairID];
			m_lUserReturnScore[wChairID]*=2;
			if(bBig)
			{
				nCount=rand()%7+8;
			}
			else
			{
				nCount=rand()%7+1;
			}
		}
	}
	else
	{
		m_lUserWinScore[wChairID]-=m_lUserTempScore[wChairID];
		if(m_wCurrentBanker!=INVALID_CHAIR)
			m_lUserWinScore[m_wCurrentBanker]+=m_lUserTempScore[wChairID];
		m_lBankerWinScore+=m_lUserTempScore[wChairID];
		m_lBankerCurGameScore+=m_lUserTempScore[wChairID];
		m_lUserReturnScore[wChairID]=0;
		if(bBig)
		{
			nCount=rand()%7+1;
		}
		else
		{
			nCount=rand()%7+8;
		}
	}

	//�������
	CMD_S_Guess Guess;
	ZeroMemory(&Guess,sizeof(Guess));

	Guess.nGuessNum=nCount;
	Guess.wChairID = wChairID;
	Guess.lUserReturnScore=m_lUserReturnScore[wChairID];	
	Guess.nBankerTime=m_wBankerTime;

	//������Ϣ
	m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_GUESS, &Guess, sizeof(Guess));
	m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_GUESS, &Guess, sizeof(Guess));
	

	return true;
}

//����ׯ��
bool CTableFrameSink::OnUserApplyBanker(IServerUserItem *pIApplyServerUserItem)
{

	//�Ϸ��ж�
	LONGLONG lUserScore=m_lExchangeGold[pIApplyServerUserItem->GetChairID()];
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
	if (m_pITableFrame->GetGameStatus()==GS_GAME_FREE && m_ApplyUserArray.GetCount()==1)
		{
			ChangeBanker(false);
		}

	

	return true;
}

//ȡ������
bool CTableFrameSink::OnUserCancelBanker(IServerUserItem *pICancelServerUserItem)
{
	//��ǰׯ��
	if (pICancelServerUserItem->GetChairID()==m_wCurrentBanker && m_pITableFrame->GetGameStatus()!=GS_GAME_FREE)
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

				//���ñ���
				CancelBanker.wCancelUser = pIServerUserItem->GetChairID();			

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
		TakeTurns();

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
			LONGLONG lBankerScore=m_lExchangeGold[m_wCurrentBanker];

			//�����ж�
			if (m_lPlayerBankerMAX<=m_wBankerTime || lBankerScore<m_lApplyBankerCondition)
			{
				//ׯ�������ж� ͬһ��ׯ�������ֻ�ж�һ��
				if(m_lPlayerBankerMAX <= m_wBankerTime && m_bExchangeBanker && lBankerScore>=m_lApplyBankerCondition)
				{
					//��ׯ�������ã���ׯ�������趨�ľ���֮��(m_lBankerMAX)��
					//�������ֵ��������������ׯ���б�����������ҽ��ʱ��
					//�����ټ���ׯm_lBankerAdd�֣���ׯ���������á�

					//��ҳ���m_lBankerScoreMAX֮��
					//������������ҵĽ��ֵ�������Ľ��ֵ����Ҳ�����ټ�ׯm_lBankerScoreAdd�֡�
					bool bScoreMAX = true;
					m_bExchangeBanker = false;

					for (WORD i=0; i<m_ApplyUserArray.GetCount(); ++i)
					{
						//��ȡ���
						WORD wChairID = m_ApplyUserArray[i];
						IServerUserItem *pIUserItem = m_pITableFrame->GetTableUserItem(wChairID);
						LONGLONG lScore = m_lExchangeGold[wChairID];

						if ( wChairID != m_wCurrentBanker && lBankerScore <= lScore )
						{
							bScoreMAX = false;
							break;
						}
					}

					if ( bScoreMAX || (lBankerScore > m_lBankerScoreMAX && m_lBankerScoreMAX != 0l) )
					{
						if ( bScoreMAX )
						{
							m_lPlayerBankerMAX += m_lBankerAdd;
						}
						if ( lBankerScore > m_lBankerScoreMAX && m_lBankerScoreMAX != 0l )
						{
							m_lPlayerBankerMAX += m_lBankerScoreAdd;
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
				TakeTurns();

				bChangeBanker=true;
				m_bExchangeBanker = true;

				//��ʾ��Ϣ
				TCHAR szTipMsg[128];
				if (lBankerScore<m_lApplyBankerCondition)
					_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("[ %s ]��������(%I64d)��ǿ�л�ׯ!"),pIServerUserItem->GetNickName(),m_lApplyBankerCondition);
				else
					_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("[ %s ]��ׯ�����ﵽ(%d)��ǿ�л�ׯ!"),pIServerUserItem->GetNickName(),m_lPlayerBankerMAX);

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
	else if (m_wCurrentBanker==INVALID_CHAIR && m_ApplyUserArray.GetCount()!=0)
	{
		//�ֻ��ж�
		TakeTurns();

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
			sChangeBanker.lBankerScore=m_lExchangeGold[m_wCurrentBanker];
		}
		m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_CHANGE_BANKER,&sChangeBanker,sizeof(sChangeBanker));
		m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_CHANGE_BANKER,&sChangeBanker,sizeof(sChangeBanker));
	}

	return bChangeBanker;
}

//�ֻ��ж�
void CTableFrameSink::TakeTurns()
{
	//��������
	int nInvalidApply = 0;

	for (int i = 0; i < m_ApplyUserArray.GetCount(); i++)
	{
		if (m_pITableFrame->GetGameStatus() == GS_GAME_FREE)
		{
			//��ȡ����
			IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(m_ApplyUserArray[i]);
			if (pIServerUserItem != NULL)
			{
				if (m_lExchangeGold[m_ApplyUserArray[i]] >= m_lApplyBankerCondition)
				{
					m_wCurrentBanker=m_ApplyUserArray[i];
					break;
				}
				else
				{
					nInvalidApply = i + 1;

					//������Ϣ
					CMD_S_CancelBanker CancelBanker = {};

					//���ñ���
					CancelBanker.wCancelUser = pIServerUserItem->GetChairID();					

					//������Ϣ
					m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));
					m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));

					//��ʾ��Ϣ
					TCHAR szTipMsg[128] = {};
					_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("�������ĳ��루%I64d��������ׯ�����������%I64d��,���޷���ׯ��"),
						pIServerUserItem->GetUserScore(), m_lApplyBankerCondition);
					SendGameMessage(m_ApplyUserArray[i],szTipMsg);
				}
			}
		}
	}

	//ɾ�����
	if (nInvalidApply != 0)
		m_ApplyUserArray.RemoveAt(0, nInvalidApply);
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
bool  CTableFrameSink::OnActionUserOffLine(WORD wChairID,IServerUserItem * pIServerUserItem)
{
	//�л�ׯ��
	if (pIServerUserItem->GetChairID()==m_wCurrentBanker) ChangeBanker(true);

	//ȡ������
	for (WORD i=0; i<m_ApplyUserArray.GetCount(); ++i)
	{
		if (pIServerUserItem->GetChairID()!=m_ApplyUserArray[i]) continue;

		//ɾ�����
		m_ApplyUserArray.RemoveAt(i);

		//�������
		CMD_S_CancelBanker CancelBanker;
		ZeroMemory(&CancelBanker,sizeof(CancelBanker));

		//���ñ���
		CancelBanker.wCancelUser=wChairID;		

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

	for (int i = 0;i<24;i++)
	{
		BYTE bcOutCadDataWin[AREA_COUNT+1];
		BYTE bcData[1];
		bcData[0]=i+1;
		m_GameLogic.GetCardType(bcData[0],1,bcOutCadDataWin);
		for (int j= 0;j<=AREA_COUNT;j++)
		{
			if(bcOutCadDataWin[j]>0&&j==InArea-1)
			{
				LONGLONG Score = 0; 
				for (int nAreaIndex=1; nAreaIndex<=AREA_COUNT; ++nAreaIndex) 
				{
					if(bcOutCadDataWin[nAreaIndex-1]>0)
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
		if (NULL!=pIUserItemBanker) lBankerScore=m_lExchangeGold[m_wCurrentBanker];
	}

	/*BYTE bcWinArea[AREA_COUNT+1];
	LONGLONG LosScore = 0;
	LONGLONG WinScore = 0;

	GetAllWinArea(bcWinArea,AREA_COUNT,Area);

	for (int nAreaIndex=1; nAreaIndex<=AREA_COUNT; ++nAreaIndex) 
	{
		if(bcWinArea[nAreaIndex-1]>0)
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
				lBankerScore=m_lExchangeGold[m_wCurrentBanker];
		}
		else
		{
			lBankerScore = 0x7fffffffffffffff;
		}
	}*/

	//��������
	LONGLONG lMeMaxScore = min((m_lExchangeGold[wChairID]), m_lUserLimitScore);

	//��������
	lMeMaxScore=min(lMeMaxScore,m_lAreaLimitScore);

	BYTE diMultiple[AREA_COUNT]={5,10,10,10,20,20,20,50};

	/*for (int i = 0;i<24;i++)
	{
		BYTE bcData[1];
		bcData[0]= i+1;
		BYTE  bcOutCadDataWin[AREA_COUNT];
		m_GameLogic.GetCardType(bcData[0],1,bcOutCadDataWin);
		for (int j = 0;j<=AREA_COUNT;j++)
		{
			if(bcOutCadDataWin[j]>1)
			{
				diMultiple[j] = bcOutCadDataWin[j];
			}
		}
	}*/
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
	//��������
	LONGLONG static cbRevenue=m_pGameServiceOption->wRevenueRatio;

	//�ƶ����
	bool static bWinTianMen, bWinDiMen, bWinXuanMen,bWinHuang;
	BYTE TianMultiple,diMultiple,TianXuanltiple,HuangMultiple;
	TianMultiple  = 1;
	diMultiple = 1 ;
	TianXuanltiple = 1;
	HuangMultiple = 1;

	BYTE  bcResulteOut[AREA_COUNT+1];
	memset(bcResulteOut,0,AREA_COUNT+1);
	BYTE  cbTempResulteOut[AREA_COUNT+1];
	memset(cbTempResulteOut,0,AREA_COUNT+1);
	BYTE  bcTempOut[AREA_COUNT+1];
	memset(bcTempOut,0,AREA_COUNT+1);
	if(m_cbTableCardArray[0]==10||m_cbTableCardArray[0]==22)//LUCK
	{		
		for(int i=0;i<CountArray(m_cbTableCardArray);i++)
		{
			if(i==0) continue;
			m_GameLogic.GetCardType(m_cbTableCardArray[i],1,bcTempOut);
			for(int j=0;j<AREA_COUNT+1;j++)
			{
				if(bcTempOut[j]>0)
					bcResulteOut[j]+=bcTempOut[j];
			}
		}
	}
	else
		m_GameLogic.GetCardType(m_cbTableCardArray[0],1,bcResulteOut);
	m_GameLogic.GetCardTypeEx(m_cbTableCardArray[0],1,cbTempResulteOut);

	//��Ϸ��¼
	tagServerGameRecord &GameRecord = m_GameRecordArrary[m_nRecordLast];

	BYTE  cbMultiple[AREA_COUNT]={1};

	for (WORD wAreaIndex = 0; wAreaIndex <= AREA_COUNT; ++wAreaIndex)
	{		
		GameRecord.bWinMen[wAreaIndex] = cbTempResulteOut[wAreaIndex];		
	}

	//�ƶ��±�
	m_nRecordLast = (m_nRecordLast+1) % MAX_SCORE_HISTORY;
	if ( m_nRecordLast == m_nRecordFirst ) m_nRecordFirst = (m_nRecordFirst+1) % MAX_SCORE_HISTORY;

	//ׯ������
	LONGLONG lBankerWinScore = 0;
	m_lBankerCurGameScore = 0;

	//��ҳɼ�
	ZeroMemory(m_lUserTempScore, sizeof(m_lUserTempScore));
	ZeroMemory(m_lUserReturnScore, sizeof(m_lUserReturnScore));
	ZeroMemory(m_lUserRevenue, sizeof(m_lUserRevenue));
	LONGLONG lUserLostScore[GAME_PLAYER];
	ZeroMemory(lUserLostScore, sizeof(lUserLostScore));

	//�����ע
	LONGLONG *pUserScore[AREA_COUNT+1];
	pUserScore[0]=NULL;
	for (int i = 1;i<=AREA_COUNT;i++)
	{
		pUserScore[i]=m_lUserJettonScore[i];
	}

	//�������
	ZeroMemory(m_lUserEveryTurnScore,sizeof(m_lUserEveryTurnScore));
	for (WORD wChairID=0; wChairID<GAME_PLAYER; wChairID++)
	{
		//ׯ���ж�
		if (m_wCurrentBanker==wChairID) continue;

		//��ȡ�û�
		IServerUserItem * pIServerUserItem=m_pITableFrame->GetTableUserItem(wChairID);
		if (pIServerUserItem==NULL) continue;

		for (WORD wAreaIndex = 1; wAreaIndex <= AREA_COUNT; ++wAreaIndex)
		{
			if (bcResulteOut[wAreaIndex-1]>0) 
			{
				m_lUserWinScore[wChairID] += ( pUserScore[wAreaIndex][wChairID] *(bcResulteOut[wAreaIndex-1]-1)) ;
				m_lUserEveryTurnScore[wChairID] += ( pUserScore[wAreaIndex][wChairID] *(bcResulteOut[wAreaIndex-1]-1)) ;
				m_lUserTempScore[wChairID] += ( pUserScore[wAreaIndex][wChairID] *(bcResulteOut[wAreaIndex-1])) ;
				m_lUserReturnScore[wChairID] +=( pUserScore[wAreaIndex][wChairID] *(bcResulteOut[wAreaIndex-1])) ;
				lBankerWinScore -= ( pUserScore[wAreaIndex][wChairID] * (bcResulteOut[wAreaIndex-1]-1) ) ;
			}
			else if (bcResulteOut[wAreaIndex-1]==0) 
			{				
				lUserLostScore[wChairID] -= pUserScore[wAreaIndex][wChairID];
				lBankerWinScore += pUserScore[wAreaIndex][wChairID];							
			}
		}

		//����˰��
		/*if (0 < m_lUserWinScore[wChairID])
		{
			double fRevenuePer=double(cbRevenue/1000.0);
			m_lUserRevenue[wChairID]  = LONGLONG(m_lUserWinScore[wChairID]*fRevenuePer);
			m_lUserWinScore[wChairID] -= m_lUserRevenue[wChairID];
		}*/

		//�ܵķ���
		m_lUserWinScore[wChairID] += lUserLostScore[wChairID];
		m_lUserEveryTurnScore[wChairID] += lUserLostScore[wChairID];
	}
	
	//ׯ�ҳɼ�
	if (m_wCurrentBanker!=INVALID_CHAIR)
	{
		m_lUserWinScore[m_wCurrentBanker] += lBankerWinScore;

		//����˰��
		if (0 < m_lUserWinScore[m_wCurrentBanker])
		{
			double fRevenuePer=double(cbRevenue/1000.0);
			m_lUserRevenue[m_wCurrentBanker]  = LONGLONG(m_lUserWinScore[m_wCurrentBanker]*fRevenuePer);
			m_lUserWinScore[m_wCurrentBanker] -= m_lUserRevenue[m_wCurrentBanker];
			lBankerWinScore = m_lUserWinScore[m_wCurrentBanker];
		}

		m_lUserEveryTurnScore[m_wCurrentBanker]=m_lUserWinScore[m_wCurrentBanker];
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
	BYTE  bcResulteOut[AREA_COUNT+1];
	memset(bcResulteOut,0,AREA_COUNT+1);
	BYTE  bcTempOut[AREA_COUNT+1];
	memset(bcTempOut,0,AREA_COUNT+1);
	if(m_cbTableCardArray[0]==10||m_cbTableCardArray[0]==22)//LUCK
	{		
		for(int i=0;i<CountArray(m_cbTableCardArray);i++)
		{
			if(i==0) continue;
			m_GameLogic.GetCardType(m_cbTableCardArray[i],1,bcTempOut);
			for(int j=0;j<AREA_COUNT+1;j++)
			{
				if(bcTempOut[j]>0)
					bcResulteOut[j]+=bcTempOut[j];
			}
		}
	}
	else
		m_GameLogic.GetCardType(m_cbTableCardArray[0],1,bcResulteOut);

	//ϵͳ��Ӯ
	LONGLONG lSystemScore = 0l;

	//�����ע
	LONGLONG *pUserScore[AREA_COUNT+1];
	pUserScore[0] = NULL;
	for (int i = 1;i<=AREA_COUNT;i++)
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
	//���ⶥ�б���ϵͳ������
	if(m_StorageStart>m_lStorageMax[0])
	{
		//�ų�LUCK �������
		if(m_cbTableCardArray[0]==10||m_cbTableCardArray[0]==22)
			return false;
		srand(GetTickCount());

		LONGLONG lAndroidJettonScore[AREA_COUNT+1];			//������ÿ����������ע
		LONGLONG lAndroidSumScore = 0l;						//����������ע
		LONGLONG lUserJettonScore[AREA_COUNT+1];			//���ÿ����������ע
		LONGLONG lUserSumScore=0l;							//�������ע
		WORD	 wHitArea;									//��������

		ZeroMemory(lAndroidJettonScore,sizeof(lAndroidJettonScore));
		ZeroMemory(lUserJettonScore,sizeof(lUserJettonScore));

		//ͳ�ƻ���
		for (WORD wAreaIndex = 1; wAreaIndex <= AREA_COUNT; ++wAreaIndex)
		{
			for (WORD wChairID=0; wChairID<GAME_PLAYER; wChairID++)
			{
				//ׯ���ж�
				if (m_wCurrentBanker == wChairID) continue;

				//��ȡ�û�
				IServerUserItem * pIServerUserItem=m_pITableFrame->GetTableUserItem(wChairID);
				if (pIServerUserItem==NULL) 
					continue;

				if(pIServerUserItem->IsAndroidUser())
				{
					//��������ע
					lAndroidJettonScore[wAreaIndex] += m_lUserJettonScore[wAreaIndex][wChairID];
					lAndroidSumScore += m_lUserJettonScore[wAreaIndex][wChairID];
				}
				else
				{
					lUserJettonScore[wAreaIndex] += m_lUserJettonScore[wAreaIndex][wChairID];
					lUserSumScore += m_lUserJettonScore[wAreaIndex][wChairID];
				}
			}


			if(bcResulteOut[wAreaIndex-1]>0)	wHitArea = wAreaIndex;
		}
		//ֻ�������ע�˲Ż����ϵͳ��ּ���
		if(lUserSumScore > 0)
		{
			BYTE  cbTempResulteOut[AREA_COUNT+1];
			memset(cbTempResulteOut,0,AREA_COUNT+1);

			//��������ׯ
			if(bIsBankerAndroidUser == false && m_wCurrentBanker != INVALID_CHAIR)
			{
				//��������ϵͳӮ�ֵĻ� �м�����ϵͳ�������
				if(lAndroidJettonScore[wHitArea]*bcResulteOut[wHitArea-1] > lAndroidSumScore)
				{
					if((m_StorageStart>m_lStorageMax[1] && rand()%100<m_wStorageMul[1]) || rand()%100<=m_wStorageMul[0]) 
					{
						bool bIsChange=false;
						for (BYTE i=1; i<=24;i++)
						{
							m_GameLogic.GetCardType(i,1,cbTempResulteOut);
							for(WORD wAreaIndex=1; wAreaIndex<=AREA_COUNT; wAreaIndex++)
							{
								if((lAndroidJettonScore[wHitArea]*cbTempResulteOut[wHitArea-1]<lAndroidSumScore))
								{
									m_cbTableCardArray[0] = i;
									CopyMemory(bcResulteOut,cbTempResulteOut,sizeof(bcResulteOut));
									bIsChange = true;
									break;
								}
							}
							if(bIsChange) break;
						}
					}
				}
			}
			else
			{
				//��������ϵͳӮ�ֵĻ� �м�����ϵͳ�������
				if(lUserJettonScore[wHitArea]*bcResulteOut[wHitArea-1] < lUserSumScore)
				{
					if((m_StorageStart>m_lStorageMax[1] && rand()%100<m_wStorageMul[1]) || rand()%100<=m_wStorageMul[0]) 
					{
						bool bIsChange=false;
						for (BYTE i=1; i<=24;i++)
						{
							m_GameLogic.GetCardType(i,1,cbTempResulteOut);
							for(WORD wAreaIndex=1; wAreaIndex<=AREA_COUNT; wAreaIndex++)
							{
								if(cbTempResulteOut[wAreaIndex-1]>0 && (lUserJettonScore[wAreaIndex]*cbTempResulteOut[wAreaIndex-1] > lUserSumScore))
								{
									m_cbTableCardArray[0] = i;
									CopyMemory(bcResulteOut,cbTempResulteOut,sizeof(bcResulteOut));
									bIsChange = true;
									break;
								}
							}
							if(bIsChange) break;
						}
					}
				}
			}
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
			if (bcResulteOut[wAreaIndex-1]>0) 
			{
				if ( bIsAndroidUser )
					lSystemScore += (pUserScore[wAreaIndex][wChairID] *(bcResulteOut[wAreaIndex-1]-1));

				if (m_wCurrentBanker == INVALID_CHAIR || bIsBankerAndroidUser)
					lSystemScore -= (pUserScore[wAreaIndex][wChairID] *(bcResulteOut[wAreaIndex-1]-1));
			}
			else
			{
				if ( bIsAndroidUser )
					lSystemScore -= pUserScore[wAreaIndex][wChairID];

				if (m_wCurrentBanker == INVALID_CHAIR || bIsBankerAndroidUser)
					lSystemScore += pUserScore[wAreaIndex][wChairID];
			}
		}
	}

	//��ֹ�ϰѿ���ʹ�ÿ��Ϊ��������������ʲô�����ʹϵͳӮ��Ҳ��ʹ���տ���Ӵ��ڸ�ֵ�����½�����ѭ��
	if ((lSystemScore*m_nExchange + m_StorageStart) < 0l && lSystemScore<0l)
	{
		return false;
	}
	else
	{
		m_StorageStart += lSystemScore*m_nExchange;
		//ֻ�п�����0�Ż�˥��
		if(m_StorageStart > 0 && NeedDeductStorage())
			m_StorageStart = m_StorageStart - (m_StorageStart * m_StorageDeduct / 1000);
		return true;
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

//��ȡ����
void CTableFrameSink::ReadConfigInformation(bool bReadFresh)
{
	//��ȡ�Զ�������
	tagCustomConfig *pCustomConfig = (tagCustomConfig *)m_pITableFrame->GetCustomRule();
	ASSERT(pCustomConfig);
	//ʱ�����
	m_cbFreeTime=pCustomConfig->CustomGeneral.cbFreeTime;
	if (m_cbFreeTime<5 || m_cbFreeTime>99) m_cbFreeTime=5;
	m_cbBetTime=pCustomConfig->CustomGeneral.cbBetTime;
	if(m_cbBetTime<10 || m_cbBetTime>99) m_cbBetTime=10;
	m_cbEndTime=pCustomConfig->CustomGeneral.cbEndTime;	
	if(m_cbEndTime<20 || m_cbEndTime>99) m_cbEndTime=20;
	m_cbCompareTime=pCustomConfig->CustomGeneral.cbGuessTime;
	//��ׯ����
	m_lApplyBankerCondition=pCustomConfig->CustomGeneral.lApplyBankerCondition;
	m_bEnableSysBanker=pCustomConfig->CustomGeneral.nEnableSysBanker;
	m_lBankerMAX=pCustomConfig->CustomGeneral.lBankerTime;
	m_lBankerAdd=pCustomConfig->CustomGeneral.lBankerTimeAdd;
	m_lBankerScoreMAX=pCustomConfig->CustomGeneral.lBankerScoreMAX;
	m_lBankerScoreAdd=pCustomConfig->CustomGeneral.lBankerTimeExtra;
	//��ע����
	m_lAreaLimitScore=pCustomConfig->CustomGeneral.lAreaLimitScore;
	m_lUserLimitScore=pCustomConfig->CustomGeneral.lUserLimitScore;

	//�����˿���
	//��������ע����
	m_nMaxChipRobot=pCustomConfig->CustomAndroid.lRobotBetMinCount+rand()%(pCustomConfig->CustomAndroid.lRobotBetMaxCount-pCustomConfig->CustomAndroid.lRobotBetMinCount+1);

	m_lRobotAreaLimit=pCustomConfig->CustomAndroid.lRobotAreaLimit;
	m_nRobotListMaxCount=pCustomConfig->CustomAndroid.lRobotListMaxCount;
	//����һ�
	m_nExchange = pCustomConfig->CustomGeneral.lEndGameMul;
	if (m_nExchange<1 || m_nExchange>10000000) m_nExchange=1;
	m_StorageStart=pCustomConfig->CustomGeneral.StorageStart;
	m_lStartStorage = m_StorageStart;
	m_StorageDeduct=pCustomConfig->CustomGeneral.StorageDeduct;
	if(m_StorageDeduct<0)		m_StorageDeduct=0;
	if(m_StorageDeduct>1000)	m_StorageDeduct=1000;
	m_lStorageMax[0]=pCustomConfig->CustomGeneral.StorageMax1;
	m_lStorageMax[1]=pCustomConfig->CustomGeneral.StorageMax2;
	m_wStorageMul[0]=pCustomConfig->CustomGeneral.StorageMul1;
	if(m_wStorageMul[0]<0 || m_wStorageMul[0]>100) m_wStorageMul[0]=80;
	m_wStorageMul[1]=pCustomConfig->CustomGeneral.StorageMul2;
	if(m_wStorageMul[1]<0 || m_wStorageMul[1]>100) m_wStorageMul[1]=80;

}

//////////////////////////////////////////////////////////////////////////
//���в���
#ifdef __SPECIAL___
bool __cdecl CTableFrameSink::OnActionUserBank(WORD wChairID, IServerUserItem * pIServerUserItem)
{
	return true;
}
#endif

//��ѯ�Ƿ�۷����
bool CTableFrameSink::QueryBuckleServiceCharge(WORD wChairID)
{
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		IServerUserItem *pUserItem=m_pITableFrame->GetTableUserItem(i);
		if(pUserItem==NULL) continue;
		if(wChairID==i)
		{
			//������ע
			for (int nAreaIndex=0; nAreaIndex<=AREA_COUNT; ++nAreaIndex) 
			{

				if (m_lUserJettonScore[nAreaIndex][wChairID] != 0)
				{
					return true;
				}
			}
			break;
		}
	}
	if (wChairID==m_wCurrentBanker)
	{
		return true;
	}
	return false;
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

// ����Ա������־
void CTableFrameSink::AdminOperateInfo( LPCTSTR pszString,IServerUserItem *pIServerUserItem)
{
	//������������
	char* old_locale = _strdup( setlocale(LC_CTYPE,NULL) );
	setlocale( LC_CTYPE, "chs" );

	CTime Time(CTime::GetCurrentTime());
	//ת����ַ
	CString strBuf;
	TCHAR szClientAddr[16]=TEXT("");
	DWORD dwAddr = pIServerUserItem->GetClientAddr();
	BYTE * pClientAddr=(BYTE *)&dwAddr;
	_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		
	strBuf.Format(TEXT("ʱ��: %d-%d-%d %d:%d:%d | ����Ա:%s ID:%d | ����IP:%s ������:%s\n"),
		Time.GetYear(), Time.GetMonth(), Time.GetDay(), Time.GetHour(), Time.GetMinute(), Time.GetSecond(),
		pIServerUserItem->GetNickName(),pIServerUserItem->GetUserID(),szClientAddr,pIServerUserItem->GetMachineID());

	strBuf.Format(TEXT("%s    %s"),strBuf,pszString);

	CStdioFile AdminFile;
	CString strFileName;
	strFileName.Format(TEXT("ˮ����������־.txt"));

	BOOL bOpen = AdminFile.Open(strFileName, CFile::modeReadWrite|CFile::modeCreate|CFile::modeNoTruncate);
	if ( bOpen )
	{	
		AdminFile.SeekToEnd();
		AdminFile.WriteString( strBuf );
		AdminFile.Flush();
		AdminFile.Close();
	}

	//��ԭ�����趨
	setlocale( LC_CTYPE, old_locale );
	free( old_locale );
}
// �����־
void CTableFrameSink::StorageLog()
{
	//������������
	char* old_locale = _strdup( setlocale(LC_CTYPE,NULL) );
	setlocale( LC_CTYPE, "chs" );

	CTime Time(CTime::GetCurrentTime());
	CString strBuf;
	strBuf.Format(TEXT("ʱ��: %d-%d-%d %d:%d:%d | ���:%I64d\n"),
		Time.GetYear(), Time.GetMonth(), Time.GetDay(), Time.GetHour(), Time.GetMinute(), Time.GetSecond(),m_StorageStart);
	
	CStdioFile StorageFile;
	CString strFileName;
	strFileName.Format(TEXT("ˮ���������־.txt"));

	BOOL bOpen = StorageFile.Open(strFileName, CFile::modeReadWrite|CFile::modeCreate|CFile::modeNoTruncate);
	if ( bOpen )
	{	
		StorageFile.SeekToEnd();
		StorageFile.WriteString( strBuf );
		StorageFile.Flush();
		StorageFile.Close();
	}

	//��ԭ�����趨
	setlocale( LC_CTYPE, old_locale );
	free( old_locale );
}

//////////////////////////////////////////////////////////////////////////
