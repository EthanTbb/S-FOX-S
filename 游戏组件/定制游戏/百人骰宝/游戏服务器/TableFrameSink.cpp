#include "StdAfx.h"
#include "DlgCustomRule.h"
#include "TableFrameSink.h"
#include <locale>

//////////////////////////////////////////////////////////////////////////

//��������
#define SEND_COUNT					100									//���ʹ���

#define IDI_FREE					1									//����ʱ��
#define IDI_PLAYER_BET				3									//��עʱ��
#define IDI_GAME_END				4									//����ʱ��


#define TIME_FREE					10									//����ʱ��
#define TIME_PLACE_JETTON			15									//��עʱ��
#define TIME_GAME_END				18									//����ʱ��

//////////////////////////////////////////////////////////////////////////

//��̬����
const WORD			CTableFrameSink::m_wPlayerCount=GAME_PLAYER;				//��Ϸ����

//////////////////////////////////////////////////////////////////////////

//���캯��
CTableFrameSink::CTableFrameSink()
{
	//����ע��
	ZeroMemory(m_lAllUserBet,sizeof(m_lAllUserBet));
	//��ʼ����
	ZeroMemory(m_lUserStartScore,sizeof(m_lUserStartScore));
	
	//������ע
	ZeroMemory(m_lUserAllBet,sizeof(m_lUserAllBet));
	
	//��������ע
	ZeroMemory(m_lAndroidBet,sizeof(m_lAndroidBet));

	//���Ʊ���
	m_cbWinSideControl=0;
	m_nSendCardCount=0;
	m_cbControlTotal = 0;
	m_cbReqType = 0;

	//��ҳɼ�	
	ZeroMemory(m_lUserRevenue,sizeof(m_lUserRevenue));
	ZeroMemory(m_lUserWinScore,sizeof(m_lUserWinScore));
	ZeroMemory(m_lUserReturnScore,sizeof(m_lUserReturnScore));
	m_cbControlDiceValue[0]=1;
	m_cbControlDiceValue[1]=1;
	m_cbControlDiceValue[2]=1;

	//�˿���Ϣ
	ZeroMemory(m_cbDiceValue,sizeof(m_cbDiceValue));

	m_cbControlStyle = 0;	
	m_cbWinAreaCount = 0;
	m_lStorageCurrent=0;
	m_lCurSystemWinScore=0;
	ZeroMemory(m_bWinArea, sizeof(m_bWinArea));
	m_lQueryGameID = -1;
	m_cbUserWinLose = 0;
	m_lControlStorage=0;
	CopyMemory(m_szControlName,TEXT("����"),sizeof(m_szControlName));

	//״̬����
	m_dwStartTime=0L;

	//ׯ����Ϣ
	m_ApplyUserArray.RemoveAll();
	m_wCurrentBanker=INVALID_CHAIR;
	m_wBankerTime=0;
	m_cbLeftCardCount=0;
	m_lBankerWinScore=0L;		
	m_lBankerCurGameScore=0L;
	m_wAddTime=0;
	m_bExchangeBanker=true;

	m_bContiueCard=false;
	m_bEnableSysBanker=true;

	m_lStorageMax1 = 0;
	m_lStorageMul1 = 0;
	m_lStorageMax2 = 0;
	m_lStorageMul2 = 0;

	//�����˿���
	m_nChipRobotCount = 0;
	m_nRobotListMaxCount =0;
	ZeroMemory(m_lRobotAreaScore, sizeof(m_lRobotAreaScore));

	//�����˿���
	m_nChipRobotCount = 0;
	ZeroMemory(m_bRobotChip, sizeof(m_bRobotChip));
	
	//��¼����
	ZeroMemory(m_GameRecordList,sizeof(m_GameRecordList));
	m_nRecordFirst=0;
	m_nRecordLast=0;
	m_dwRecordCount=0;

	m_cbNeedControl=false;
	m_bControlWin = false;

	srand((unsigned)time(NULL));   

	return;
}

//��������
CTableFrameSink::~CTableFrameSink(VOID)
{
}

VOID CTableFrameSink::RandList(BYTE cbCardBuffer[], BYTE cbBufferCount)
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


//�ӿڲ�ѯ
VOID *  CTableFrameSink::QueryInterface(const IID & Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(ITableFrameSink,Guid,dwQueryVer);
	QUERYINTERFACE(ITableUserAction,Guid,dwQueryVer);
	return NULL;
}

//��ʼ��
bool  CTableFrameSink::Initialization(IUnknownEx * pIUnknownEx)
{
	

	////���ƽӿ�
	//m_pITableFrameControl=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,ITableFrameControl);
	//if (m_pITableFrameControl==NULL) return false;

	//��ѯ�ӿ�
	ASSERT(pIUnknownEx!=NULL);
	m_pITableFrame=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,ITableFrame);
	if (m_pITableFrame==NULL) return false;

	//��ʼģʽ
	m_pITableFrame->SetStartMode(START_MODE_TIME_CONTROL);


	//��ȡ����
	m_pGameServiceOption=m_pITableFrame->GetGameServiceOption();
	ASSERT(m_pGameServiceOption!=NULL);

	ReadConfigInformation();	

	return true;
}

//��λ����
VOID  CTableFrameSink::RepositionSink()
{
	//����ע��
	ZeroMemory(m_lAllUserBet,sizeof(m_lAllUserBet));
	
	//������ע
	ZeroMemory(m_lUserAllBet,sizeof(m_lUserAllBet));

	//��ҳɼ�	
	ZeroMemory(m_lUserWinScore,sizeof(m_lUserWinScore));
	ZeroMemory(m_lUserReturnScore,sizeof(m_lUserReturnScore));
	ZeroMemory(m_lUserRevenue,sizeof(m_lUserRevenue));

	//�����˿���
	m_nChipRobotCount = 0;
	ZeroMemory(m_bRobotChip, sizeof(m_bRobotChip));
	m_lCurSystemWinScore=0;
	ZeroMemory(m_lRobotAreaScore, sizeof(m_lRobotAreaScore));
	m_lControlStorage=0;

	return;
}

//��Ϸ״̬
bool  CTableFrameSink::IsUserPlaying(WORD wChairID)
{
	return true;
}

//��Ϸ��ʼ
bool  CTableFrameSink::OnEventGameStart()
{
	//�����˻����Ϳ��ֵ
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		//��ȡ�û�
		IServerUserItem * pIServerUserItem=m_pITableFrame->GetTableUserItem(i);
		if ( pIServerUserItem == NULL )
			continue;

		if( !CUserRight::IsGameCheatUser(pIServerUserItem->GetUserRight())) 
			continue;

			CString strInfo;
			strInfo.Format(TEXT("��ǰ��棺%I64d"), m_lStorageCurrent);

			m_pITableFrame->SendGameMessage(pIServerUserItem,strInfo,SMT_CHAT);
			
	}

	CString strStorage;
	CTime Time(CTime::GetCurrentTime());
	strStorage.Format(TEXT(" ����: %s | ʱ��: %d-%d-%d %d:%d:%d | ���: %I64d \n"), m_pGameServiceOption->szServerName, Time.GetYear(), Time.GetMonth(), Time.GetDay(), Time.GetHour(), Time.GetMinute(), Time.GetSecond(), m_lStorageCurrent );
	WriteInfo(TEXT("�������������Ϣ.log"),strStorage);

	//��������
	CMD_S_GameStart GameStart;
	ZeroMemory(&GameStart,sizeof(GameStart));
	
	m_lBankerScore=(m_wCurrentBanker==INVALID_CHAIR)?210000000:m_pITableFrame->GetTableUserItem(m_wCurrentBanker)->GetUserScore();
	
	//�ɷ��˿�
	/*for(BYTE cbIndex=0;cbIndex<DICE_COUNT;cbIndex++)
	{
		m_cbDiceValue[cbIndex]=rand()%6+1;
	}*/

	//��ȡׯ��
	IServerUserItem *pIBankerServerUserItem=NULL;
	if (INVALID_CHAIR!=m_wCurrentBanker) pIBankerServerUserItem=m_pITableFrame->GetTableUserItem(m_wCurrentBanker);

	
	//���ñ���
	GameStart.cbTimeLeave=(BYTE)m_dwTimeGameBet;
	GameStart.wBankerUser=m_wCurrentBanker;

	if (pIBankerServerUserItem!=NULL) 
		GameStart.lBankerScore=m_lBankerScore;

	//��Ӯ��Ϣ
	memcpy(GameStart.bWinFlag, m_bWinFlag, sizeof(m_bWinFlag));
		
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
		GameStart.nAndriodApplyCount=nChipRobotCount-1;

	//�����˿���
	m_nChipRobotCount = 0;
	ZeroMemory(m_bRobotChip, sizeof(m_bRobotChip));
	ZeroMemory(m_lRobotAreaScore, sizeof(m_lRobotAreaScore));
	
	//�Թ����
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_START,&GameStart,sizeof(GameStart));	

	//��Ϸ���
	for (WORD wChairID=0; wChairID<GAME_PLAYER; ++wChairID)
	{
		IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(wChairID);
		if (pIServerUserItem==NULL) continue;
		//���û���
		GameStart.lUserMaxScore=m_lUserLimitScore;
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
			//����
			if (m_cbNeedControl)
			{
				memcpy(m_cbDiceValue,m_cbControlDiceValue,sizeof(BYTE)*DICE_COUNT);				

				//������Ϣ
				for (WORD i=0;i<m_wPlayerCount;i++)
				{
					//��ȡ�û�
					IServerUserItem * pIServerUserItem=m_pITableFrame->GetTableUserItem(i);
					if ( pIServerUserItem == NULL )
						continue;

					if( CUserRight::IsGameCheatUser(m_pITableFrame->GetTableUserItem(i)->GetUserRight()))
					{
						CMD_S_CommandResult cResult;
						cResult.cbResult=CR_INVALID;
						cResult.cbAckType=ACK_SET_WIN_AREA;
						m_pITableFrame->SendTableData(i,SUB_S_AMDIN_COMMAND,&cResult,sizeof(CMD_S_CommandResult));
					}	
				}

			}
			else
			{
				if (m_bControlWin)
				{					
					BYTE cbSuccess = 0;
					if (m_cbUserWinLose > 0)
					{
						do
						{
							//���³�ǩ
							DispatchTableLabel();
							//��̽�ж�
							cbSuccess = ControlUserWin();

						} while (m_cbUserWinLose!=cbSuccess);

						//������Ϣ
						for (WORD i=0;i<m_wPlayerCount;i++)
						{
							//��ȡ�û�
							IServerUserItem * pIServerUserItem=m_pITableFrame->GetTableUserItem(i);
							if ( pIServerUserItem == NULL )
								continue;

							if( CUserRight::IsGameCheatUser(m_pITableFrame->GetTableUserItem(i)->GetUserRight()))
							{
								CMD_S_ControlWinLose WinLose;
								WinLose.cbWinLose = m_cbUserWinLose;
								WinLose.lQueryGameID = m_lQueryGameID;
								m_pITableFrame->SendTableData(i,SUB_S_CONTROL_WIN,&WinLose,sizeof(CMD_S_ControlWinLose));
							}	
						}
					}				
				}
				else
				{
					bool bSuccess = false;
					m_lCurSystemWinScore = 0L;
					int n = 0;//��ֹ���Ƴ��ָ��ֺ������ע���������ʱ��ѭ��
					do 
					{
						//���³�ǩ
						DispatchTableLabel();
						//��̽�ж�
						bSuccess = ProbeJudge();

					} while (!bSuccess && ++n < 65534);
				}							
			}			
			
			//�������
			//SCORE AllScore=0;
			//bool     bWinFlags[AREA_COUNT]={0};
			SCORE lBankerWinScore=CalculateScore(false);

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
			memcpy(GameEnd.cbDiceValue,m_cbDiceValue,sizeof(m_cbDiceValue));
			
			//�˿���Ϣ
			CopyMemory(GameEnd.cbDiceValue,m_cbDiceValue,sizeof(m_cbDiceValue));
			GameEnd.cbLeftCardCount=m_cbLeftCardCount;

			
			//���ͻ���
			GameEnd.cbTimeLeave=(BYTE)m_dwTimeGameEnd;	
			for ( WORD wUserIndex = 0; wUserIndex < GAME_PLAYER; ++wUserIndex )
			{
				IServerUserItem *pIServerUserItem = m_pITableFrame->GetTableUserItem(wUserIndex);
				if ( pIServerUserItem == NULL ) continue;

				//���óɼ�
				GameEnd.lUserScore=m_lUserWinScore[wUserIndex];
				//��������
				GameEnd.lUserReturnScore=m_lUserReturnScore[wUserIndex];
				//����˰��
				if (m_lUserRevenue[wUserIndex]>0) 
					GameEnd.lRevenue=m_lUserRevenue[wUserIndex];
				else 
					if (m_wCurrentBanker!=INVALID_CHAIR) 
						GameEnd.lRevenue=m_lUserRevenue[m_wCurrentBanker];
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
			//�м�����
			if (m_wCurrentBanker!=wChairID)
			{
				return PlayerFlee(wChairID,pIServerUserItem);
			}
			else
			{
				//״̬�ж�
				if (m_pITableFrame->GetGameStatus()!=GS_GAME_END)
				{
					//��ʾ��Ϣ
					TCHAR szTipMsg[128];
					_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("����ׯ��[ %s ]ǿ�ˣ���Ϸ��ǰ������"),pIServerUserItem->GetNickName());

					//������Ϣ
					SendGameMessage(INVALID_CHAIR,szTipMsg);	

					//����״̬
					m_pITableFrame->SetGameStatus(GS_GAME_END);

					//����ʱ��
					m_pITableFrame->KillGameTimer(IDI_PLAYER_BET);
					m_pITableFrame->SetGameTimer(IDI_GAME_END,m_dwTimeGameEnd*1000,1,0L);

					bool bSuccess = false;
					m_lCurSystemWinScore = 0L;	
					
					int n = 0;//��ֹ���Ƴ��ָ��ֺ������ע���������ʱ��ѭ��
					do 
					{
						//���³�ǩ
						DispatchTableLabel();
						//��̽�ж�
						bSuccess = ProbeJudge();

					} while (!bSuccess && ++n < 65534);


					//�������
					SCORE lBankerWinScore=CalculateScore(false);

					//������Ϣ
					CMD_S_GameEnd GameEnd;
					ZeroMemory(&GameEnd,sizeof(GameEnd));

					//ׯ����Ϣ
					GameEnd.nBankerTime = m_wBankerTime;
					GameEnd.lBankerTotallScore=m_lBankerWinScore;
					GameEnd.lBankerScore=lBankerWinScore;

					//�˿���Ϣ
					CopyMemory(GameEnd.cbDiceValue,m_cbDiceValue,sizeof(m_cbDiceValue));
					GameEnd.cbLeftCardCount=m_cbLeftCardCount;

					//���ͻ���
					GameEnd.cbTimeLeave=(BYTE)m_dwTimeGameEnd;	
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
				if (m_lUserWinScore[m_wCurrentBanker]!=0)
				{
					
					tagScoreInfo ScoreInfo[GAME_PLAYER];
					ZeroMemory(ScoreInfo,sizeof(ScoreInfo));
					ScoreInfo[m_wCurrentBanker].cbType=(m_lUserWinScore[m_wCurrentBanker]>0L)?SCORE_TYPE_WIN:SCORE_TYPE_LOSE;
					ScoreInfo[m_wCurrentBanker].lRevenue=m_lUserRevenue[m_wCurrentBanker];
					ScoreInfo[m_wCurrentBanker].lScore=m_lUserWinScore[m_wCurrentBanker];
					m_pITableFrame->WriteTableScore(ScoreInfo,CountArray(ScoreInfo));


					IServerUserItem *pIServerUserItem = m_pITableFrame->GetTableUserItem(m_wCurrentBanker);
					if(!pIServerUserItem->IsAndroidUser())
					//m_lStorageCurrent-=ScoreInfo[m_wCurrentBanker].lScore;
					
					m_lUserWinScore[m_wCurrentBanker]=0;
				}

				//�л�ׯ��
				ChangeBanker(true);
			}
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
	case GER_NORMAL:			//����״̬
		{
			//���ͼ�¼
			SendGameRecord(pIServerUserItem);

			//��������
			CMD_S_StatusFree StatusFree;
			ZeroMemory(&StatusFree,sizeof(StatusFree));			

			//������Ϣ
			StatusFree.lApplyBankerCondition = m_lApplyBankerCondition;
			StatusFree.lAreaLimitScore = m_lAreaLimitScore;

			//ׯ����Ϣ
			StatusFree.bEnableSysBanker=m_bEnableSysBanker;
			StatusFree.wBankerUser=m_wCurrentBanker;	
			StatusFree.cbBankerTime=m_wBankerTime;
			StatusFree.lBankerWinScore=m_lBankerWinScore;
			if (m_wCurrentBanker!=INVALID_CHAIR)
			{
				StatusFree.lBankerScore=m_lBankerScore;
			}

			//�����Ϣ
			if (pIServerUserItem->GetUserStatus()!=US_LOOKON)
			{
				StatusFree.lUserMaxScore=m_lUserLimitScore; 
			}

			//ȫ����Ϣ
			DWORD dwPassTime=(DWORD)time(NULL)-m_dwStartTime;
			StatusFree.cbTimeLeave=(BYTE)(m_dwTimeGameFree-__min(dwPassTime,m_dwTimeGameFree));

			//��������
			CopyMemory(StatusFree.szGameRoomName, m_strAppName.GetBuffer(), sizeof(StatusFree.szGameRoomName));

			//����������
			if(pIServerUserItem->IsAndroidUser())
			{
				tagCustomConfig *pCustomConfig = (tagCustomConfig *)m_pGameServiceOption->cbCustomRule;
				ASSERT(pCustomConfig);

				CopyMemory(&StatusFree.CustomAndroid, &pCustomConfig->CustomAndroid, sizeof(tagCustomAndroid));
			}

			//���ͳ���
			bool bSuccess = m_pITableFrame->SendGameScene(pIServerUserItem,&StatusFree,sizeof(StatusFree));

			//������ʾ
			TCHAR szTipMsg[128];
			_sntprintf(szTipMsg,CountArray(szTipMsg),_T("��������ׯ����Ϊ:%I64d,��������Ϊ:%I64d,�������Ϊ:%I64d"),m_lApplyBankerCondition,m_lAreaLimitScore,m_lUserLimitScore);
			m_pITableFrame->SendGameMessage(pIServerUserItem,szTipMsg,SMT_CHAT);		
			//����������
			SendApplyUser(pIServerUserItem);
			return bSuccess;
		}
	case GS_GAME_END:			//����״̬
	case GS_PLAYER_BET:			//��Ϸ״̬
	case GS_GAME_START:			//��Ϸ����
		{
			//���ͼ�¼
			SendGameRecord(pIServerUserItem);		

			//��������
			CMD_S_StatusPlay StatusPlay={0};

			//ȫ����ע
			CopyMemory(StatusPlay.lAllJettonScore,m_lAllUserBet,sizeof(StatusPlay.lAllJettonScore));

			//�����ע
			if (pIServerUserItem->GetUserStatus()!=US_LOOKON)
			{
				for (int nAreaIndex=0; nAreaIndex<AREA_COUNT; ++nAreaIndex)
					StatusPlay.lUserJettonScore[nAreaIndex] = m_lUserAllBet[nAreaIndex][wChiarID];

				//�����ע
				StatusPlay.lUserMaxScore=m_lUserLimitScore;
			}

			//������Ϣ
			StatusPlay.lApplyBankerCondition=m_lApplyBankerCondition;		
			StatusPlay.lAreaLimitScore=m_lAreaLimitScore;		

			//ׯ����Ϣ
			StatusPlay.bEnableSysBanker=m_bEnableSysBanker;
			StatusPlay.wBankerUser=m_wCurrentBanker;			
			StatusPlay.cbBankerTime=m_wBankerTime;
			StatusPlay.lBankerWinScore=m_lBankerWinScore;	
			if (m_wCurrentBanker!=INVALID_CHAIR)
			{
				StatusPlay.lBankerScore=m_lBankerScore;
			}	

			//ȫ����Ϣ
			DWORD dwPassTime=(DWORD)time(NULL)-m_dwStartTime;
			switch(cbGameStatus)
			{
				case GS_GAME_END:
					StatusPlay.cbTimeLeave=(BYTE)(m_dwTimeGameEnd-__min(dwPassTime,m_dwTimeGameEnd));
					break;
				case GS_PLAYER_BET:
					StatusPlay.cbTimeLeave=(BYTE)(m_dwTimeGameBet-__min(dwPassTime,m_dwTimeGameBet));
					break;
				case GS_GAME_START:
					StatusPlay.cbTimeLeave=(BYTE)(m_dwTimeGameCard-__min(dwPassTime,m_dwTimeGameCard));
					break;
				default: break;
			}
			
			StatusPlay.cbGameStatus=m_pITableFrame->GetGameStatus();			

				//�����ж�
			if (cbGameStatus==GS_GAME_END)
			{
				CopyMemory(StatusPlay.cbDiceValue,m_cbDiceValue,sizeof(m_cbDiceValue));
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
			}
			
			//��������
			CopyMemory(StatusPlay.szGameRoomName, m_strAppName.GetBuffer(), sizeof(StatusPlay.szGameRoomName));

			//����������
			if(pIServerUserItem->IsAndroidUser())
			{
				tagCustomConfig *pCustomConfig = (tagCustomConfig *)m_pGameServiceOption->cbCustomRule;
				ASSERT(pCustomConfig);

				CopyMemory(&StatusPlay.CustomAndroid, &pCustomConfig->CustomAndroid, sizeof(tagCustomAndroid));
			}
		
			m_pITableFrame->SendGameScene(pIServerUserItem,&StatusPlay,sizeof(StatusPlay));
			//����������
			SendApplyUser( pIServerUserItem );
			//������ʾ
			TCHAR szTipMsg[138];
			_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("��������ׯ����Ϊ:%I64d,��������Ϊ:%I64d,�������Ϊ:%I64d"),m_lApplyBankerCondition,m_lAreaLimitScore,m_lUserLimitScore);
			m_pITableFrame->SendGameMessage(pIServerUserItem,szTipMsg,SMT_CHAT);

			//���������ע��Ϣ
			if(CUserRight::IsGameCheatUser(pIServerUserItem->GetUserRight()))
			{
				SendUserBetInfo(pIServerUserItem);
			}

			return true;
		}
	}

	return false;
}

//
bool CTableFrameSink::OnTimerStartBet()
{
	//���ÿ���
	//RobotConfigControl();
	//�����ׯ�б�
	//CheckBankerList();
	//������ע��Ϣ
	//m_pITableFrameControl->StartGame();
	m_pITableFrame->StartGame();

	//����ʱ��
	m_pITableFrame->SetGameTimer(IDI_PLAYER_BET,m_dwTimeGameBet*1000,1,0L);

	//����״̬
	m_pITableFrame->SetGameStatus(GS_PLAYER_BET);

	CMD_S_StartBet startBet;
	startBet.cbTimeLeave=(BYTE)m_dwTimeGameBet;
	
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_START_BET,&startBet,sizeof(startBet));
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_START_BET,&startBet,sizeof(startBet));
	
	//�׶�ʱ��
	m_dwStartTime=(DWORD)time(NULL);

	return true;
}

bool CTableFrameSink::OnTimerShowResult()
{
	//״̬�ж�(��ֹǿ���ظ�����)
	if (m_pITableFrame->GetGameStatus()!=GS_GAME_END)
	{
		//����״̬
		m_pITableFrame->SetGameStatus(GS_GAME_END);			
		//������Ϸ
		OnEventGameConclude(INVALID_CHAIR,NULL,GER_NORMAL);
		//����ʱ��
		m_pITableFrame->SetGameTimer(IDI_GAME_END,m_dwTimeGameEnd*1000,1,0L);
		//�׶�ʱ��
		m_dwStartTime=(DWORD)time(NULL);
	}

	return true;
}

bool CTableFrameSink::OnTimerGameEnd()
{
	
	tagScoreInfo ScoreInfo[GAME_PLAYER];
	ZeroMemory(ScoreInfo,sizeof(ScoreInfo));
	LONGLONG lStorage=0;
	
	//д�����
	for ( WORD wUserChairID = 0; wUserChairID < GAME_PLAYER; ++wUserChairID )
	{
		IServerUserItem *pIServerUserItem = m_pITableFrame->GetTableUserItem(wUserChairID);
		if ( pIServerUserItem == NULL ) continue;
		//ʤ������
		//enScoreKind ScoreKind=(m_lUserWinScore[wUserChairID]>0L)?enScoreKind_Win:enScoreKind_Lost;
		//д�����
		//if (m_lUserWinScore[wUserChairID]!=0L)
		//	m_pITableFrame->WriteUserScore(pIServerUserItem,m_lUserWinScore[wUserChairID],m_lUserRevenue[wUserChairID],ScoreKind);

		if (m_lUserWinScore[wUserChairID]!=0)
		{
			ScoreInfo[wUserChairID].cbType=(m_lUserWinScore[wUserChairID]>0L)?SCORE_TYPE_WIN:SCORE_TYPE_LOSE;
			ScoreInfo[wUserChairID].lRevenue=m_lUserRevenue[wUserChairID];
			ScoreInfo[wUserChairID].lScore=m_lUserWinScore[wUserChairID];
		}
		if (!pIServerUserItem->IsAndroidUser()&&(m_bControlWin||m_cbNeedControl))
		{
			lStorage-=(m_lUserWinScore[wUserChairID]+m_lUserRevenue[wUserChairID]);
		}
	}
	if(m_bControlWin||m_cbNeedControl)
	{
		m_lControlStorage=lStorage;
		CString cs;
		cs.Format(TEXT("�������Ϊ��%I64d���˺�Ϊ��%s"),m_lControlStorage,m_szControlName);
		CTraceService::TraceString(cs,TraceLevel_Exception);					
	}
	m_cbNeedControl=false;
	m_bControlWin = false;

	m_pITableFrame->WriteTableScore(ScoreInfo,CountArray(ScoreInfo));

	//���
	/*for ( WORD wUserChairID = 0; wUserChairID < GAME_PLAYER; ++wUserChairID )
	{
		IServerUserItem *pIServerUserItem = m_pITableFrame->GetTableUserItem(wUserChairID);
		if ( pIServerUserItem == NULL ) continue;
		if(!pIServerUserItem->IsAndroidUser())
		{
			m_lStorageCurrent-=ScoreInfo[wUserChairID].lScore;
		}
	}*/

	//������Ϸ
	m_pITableFrame->ConcludeGame(GAME_STATUS_FREE);
	//�л�ׯ��
	ChangeBanker(false);
	
	//����ʱ��
	m_dwStartTime=(DWORD)time(NULL);
	m_pITableFrame->SetGameTimer(IDI_FREE,m_dwTimeGameFree*1000,1,0L);

	//������Ϣ
	CMD_S_GameFree GameFree;
	ZeroMemory(&GameFree,sizeof(GameFree));
	GameFree.cbTimeLeave=(BYTE)m_dwTimeGameFree;
	GameFree.nListUserCount=m_ApplyUserArray.GetCount()-1;
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_FREE,&GameFree,sizeof(GameFree));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_FREE,&GameFree,sizeof(GameFree));

	//��������ׯ����
	//RobotBankerControl();

	return true;
}

//��ʱ���¼�
bool  CTableFrameSink::OnTimerMessage(DWORD wTimerID, WPARAM wBindParam)
{
	switch (wTimerID)
	{
		case IDI_FREE:		//����ʱ��
			{
				return OnTimerStartBet();
			}
		case IDI_PLAYER_BET:		//��עʱ��
			{
				return OnTimerShowResult();
			}
		case IDI_GAME_END:			//������Ϸ
			{
				return OnTimerGameEnd();
			}
		default:break;
	}

	return false;
}

//��Ϸ��Ϣ����
bool  CTableFrameSink::OnGameMessage(WORD wSubCmdID,  VOID * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem)
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
			//�û�Ч��
			tagUserInfo * pUserData=pIServerUserItem->GetUserInfo();
			if (pUserData->cbUserStatus==US_LOOKON) return true;
			if (pUserData->wChairID!=m_wCurrentBanker) return true;
			if (m_cbLeftCardCount < 8) return true;

			//���ñ���
			m_bContiueCard=true;

			//������Ϣ
			SendGameMessage(pUserData->wChairID,TEXT("���óɹ�����һ�ֽ��������ƣ�"));

			return true;
		}
	case SUB_C_AMDIN_COMMAND:
		{
			ASSERT(wDataSize==sizeof(CMD_C_AdminReq));
			if(wDataSize!=sizeof(CMD_C_AdminReq)) return false;
			return OnSubAmdinCommand(pIServerUserItem,pDataBuffer);
		}
	case SUB_C_STORAGE:
		{
			ASSERT(wDataSize==sizeof(CMD_C_FreshStorage));
			if(wDataSize!=sizeof(CMD_C_FreshStorage)) return false;

			//Ȩ���ж�
			if(CUserRight::IsGameCheatUser(pIServerUserItem->GetUserRight())==false)
				return false;

			CMD_C_FreshStorage *FreshStorage=(CMD_C_FreshStorage *)pDataBuffer;
			
			if (FreshStorage->cbReqType==RQ_SET_STORAGE)
			{
				m_nStorageDeduct = FreshStorage->lStorageDeduct;
				m_lStorageCurrent = FreshStorage->lStorageCurrent;
				m_lStorageMax1 = FreshStorage->lStorageMax1;
				m_lStorageMul1 = FreshStorage->lStorageMul1;
				m_lStorageMax2 = FreshStorage->lStorageMax2;
				m_lStorageMul2 = FreshStorage->lStorageMul2;

				//��¼��Ϣ
				CString strControlInfo, str;
				str.Format(TEXT("%s"), TEXT("�޸Ŀ�����ã�"));
				CTime Time(CTime::GetCurrentTime());
				strControlInfo.Format(TEXT("����: %s | ����: %u | ʱ��: %d-%d-%d %d:%d:%d\n�������˺�: %s | ������ID: %u\n%s\r\n"),
					m_pGameServiceOption->szServerName, m_pITableFrame->GetTableID()+1, Time.GetYear(), Time.GetMonth(), Time.GetDay(),
					Time.GetHour(), Time.GetMinute(), Time.GetSecond(), pIServerUserItem->GetNickName(), pIServerUserItem->GetGameID(), str);

				WriteInfo(TEXT("��������������Ϣ.log"),strControlInfo);
			}

			for(WORD wUserID = 0; wUserID < GAME_PLAYER; wUserID++)
			{
				IServerUserItem *pIServerUserItemSend = m_pITableFrame->GetTableUserItem(wUserID);
				if ( pIServerUserItemSend == NULL ) continue;
				if( !CUserRight::IsGameCheatUser( pIServerUserItemSend->GetUserRight() ) ) continue;

				if(RQ_REFRESH_STORAGE == FreshStorage->cbReqType && pIServerUserItem->GetChairID() != wUserID) continue;

				CMD_S_UpdateStorage updateStorage;
				ZeroMemory(&updateStorage, sizeof(updateStorage));

				if(RQ_SET_STORAGE == FreshStorage->cbReqType && pIServerUserItem->GetChairID() == wUserID)
				{
					updateStorage.cbReqType = RQ_SET_STORAGE;
				}
				else
				{
					updateStorage.cbReqType = RQ_REFRESH_STORAGE;
				}

				updateStorage.lStorageStart = m_lStorageStart;
				updateStorage.lStorageDeduct = m_nStorageDeduct;
				updateStorage.lStorageCurrent = m_lStorageCurrent;
				updateStorage.lStorageMax1 = m_lStorageMax1;
				updateStorage.lStorageMul1 = m_lStorageMul1;
				updateStorage.lStorageMax2 = m_lStorageMax2;
				updateStorage.lStorageMul2 = m_lStorageMul2;

				m_pITableFrame->SendUserItemData(pIServerUserItem,SUB_S_UPDATE_STORAGE,&updateStorage,sizeof(updateStorage));
			}

			return true;
		}
	case SUB_C_CONTROL_WIN:
		{
			ASSERT(wDataSize==sizeof(CMD_C_ControlWinLose));
			if(wDataSize!=sizeof(CMD_C_ControlWinLose)) return false;

			//Ȩ���ж�
			if(CUserRight::IsGameCheatUser(pIServerUserItem->GetUserRight())==false)
				return false;

			CMD_C_ControlWinLose *pControlWinLose=(CMD_C_ControlWinLose *)pDataBuffer;

			m_cbUserWinLose = pControlWinLose->cbWinLose;
			m_lQueryGameID = pControlWinLose->lQueryGameID;
			m_bControlWin = true;

			CopyMemory(m_szControlName,pIServerUserItem->GetNickName(),sizeof(m_szControlName));

			return true;
		}
	}

	return false;
}

//�����Ϣ����
bool  CTableFrameSink::OnFrameMessage(WORD wSubCmdID,  VOID * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	return false;
}


//�����¼�
bool CTableFrameSink::OnGameDataBase(WORD wRequestID, VOID * pData, WORD wDataSize)
{
	return false;
}

//��ע�¼�
bool CTableFrameSink::OnUserPlaceJetton(WORD wChairID, BYTE cbJettonArea, SCORE lJettonScore)
{
	//Ч�����
	ASSERT((cbJettonArea<AREA_COUNT&& cbJettonArea>=0)&&(lJettonScore>0L));
	if ((cbJettonArea>=AREA_COUNT)||(lJettonScore<=0L) || cbJettonArea<0)
	{
		return false;
	}

	if (m_pITableFrame->GetGameStatus()!=GS_PLAYER_BET)
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
	if (m_wCurrentBanker==wChairID)	return true;
	if (m_bEnableSysBanker==false && m_wCurrentBanker==INVALID_CHAIR) return true;

	//��������
	IServerUserItem * pIServerUserItem = m_pITableFrame->GetTableUserItem(wChairID);

	//��������֤
	if(pIServerUserItem != NULL && pIServerUserItem->IsAndroidUser())
	{
		//��������
		if (m_lRobotAreaScore[cbJettonArea] + lJettonScore > m_lRobotAreaLimit)
			return true;
		//��Ŀ����
		if (!m_bRobotChip[wChairID])
		{
			if (m_nChipRobotCount+1 > m_nMaxChipRobot)
				return true;
			else
			{
				m_nChipRobotCount++;
				m_bRobotChip[wChairID] = true;
			}
		}
	}

	//ׯ�һ���
	LONGLONG lBankerScore = 0;
	if(INVALID_CHAIR != m_wCurrentBanker)
	{
		IServerUserItem * pIServerUserItemBanker = m_pITableFrame->GetTableUserItem(m_wCurrentBanker);
		lBankerScore = pIServerUserItemBanker->GetUserScore();
	}
	LONGLONG lAllJettonCount=0L;
	for (int nAreaIndex=0; nAreaIndex<AREA_COUNT; ++nAreaIndex) lAllJettonCount += m_lAllUserBet[nAreaIndex];
	
	//�Ϸ���֤
	if (GetUserMaxJetton(wChairID,cbJettonArea) >= lJettonScore)
	{
		//������ע
		m_lAllUserBet[cbJettonArea] += lJettonScore;
		m_lUserAllBet[cbJettonArea][wChairID] += lJettonScore;
		m_lRobotAreaScore[cbJettonArea] += lJettonScore;

		//��������
		CMD_S_PlaceJetton PlaceJetton;
		ZeroMemory(&PlaceJetton,sizeof(PlaceJetton));

		//�������
		PlaceJetton.wChairID=wChairID;
		PlaceJetton.cbJettonArea=cbJettonArea;
		PlaceJetton.lJettonScore=lJettonScore;
		PlaceJetton.bIsAndroid=pIServerUserItem->IsAndroidUser();

		//������Ϣ
		m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_PLACE_JETTON,&PlaceJetton,sizeof(PlaceJetton));
		m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_PLACE_JETTON,&PlaceJetton,sizeof(PlaceJetton));

		//���������ע��Ϣ
		if(!pIServerUserItem->IsAndroidUser())
		{
			for(WORD i=0; i<GAME_PLAYER; i++)
			{
				IServerUserItem * pIServerUserItemSend = m_pITableFrame->GetTableUserItem(i);
				if(NULL == pIServerUserItemSend) continue;
				if(!CUserRight::IsGameCheatUser(pIServerUserItemSend->GetUserRight())) continue;

				SendUserBetInfo(pIServerUserItemSend);
			}
		}

// 		//��ע�ܶ��ׯ����֧������ʱ��ǰ������Ϸ
// 		if(INVALID_CHAIR != m_wCurrentBanker)
// 		{
// 			LONGLONG lBankerCanUseScore = lBankerScore*m_nEndGameMul/100;
// 			if(lBankerCanUseScore <= (lAllJettonCount + lJettonScore))
// 			{
// 				if (m_pITableFrame->GetGameStatus()!=GS_GAME_END)
// 				{
// 					//����״̬
// 					m_pITableFrame->SetGameStatus(GS_GAME_END);			
// 
// 					//������Ϸ
// 					OnEventGameConclude(INVALID_CHAIR,NULL,GER_NORMAL);
// 
// 					//����ʱ��
// 					m_dwStartTime=(DWORD)time(NULL);
// 					m_pITableFrame->SetGameTimer(IDI_GAME_END,m_dwTimeGameEnd*1000,1,0L);			
// 				}
// 			}
// 		}

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


//����ׯ��
bool CTableFrameSink::OnUserApplyBanker(IServerUserItem *pIApplyServerUserItem)
{
	
	//�Ϸ��ж�
	SCORE lUserScore=pIApplyServerUserItem->GetUserScore();
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
		ChangeBanker(false);
	}

	return true;
}

bool CTableFrameSink::OnSubAmdinCommand(IServerUserItem*pIServerUserItem,const VOID*pDataBuffer)
{

	//��������й���ԱȨ�� �򷵻ش���
	if( !CUserRight::IsGameCheatUser(pIServerUserItem->GetUserRight())) return false;

	const CMD_C_AdminReq* AdminReq=static_cast<const CMD_C_AdminReq*>(pDataBuffer);

	BYTE cbDice[3];
	BYTE cbTotalCount = 0;
	ZeroMemory(cbDice,sizeof(cbDice));
	CMD_S_CommandResult cResult;
	cResult.cbLastType = m_cbReqType;

	if (AdminReq->cbReqType==RQ_SET_COUNT)
	{
		if (AdminReq->cbTotalCount == 0)
		{
			m_cbControlDiceValue[0]=AdminReq->cbSicbo1;
			m_cbControlDiceValue[1]=AdminReq->cbSicbo2;
			m_cbControlDiceValue[2]=AdminReq->cbSicbo3;
			m_cbControlTotal=0;
		}
		else
		{
			do 
			{
				cbDice[0] = rand()%6+1;
				cbDice[1] = rand()%6+1;
				cbDice[2] = rand()%6+1;
				cbTotalCount = cbDice[0]+cbDice[1]+cbDice[2];
			} while (AdminReq->cbTotalCount!=cbTotalCount);
			m_cbControlTotal = cbTotalCount;
			CopyMemory(m_cbControlDiceValue,cbDice,sizeof(cbDice));
		}
		m_cbNeedControl=true;
		cResult.cbResult=CR_ACCEPT;
		cResult.cbAckType=ACK_SET_WIN_AREA;
		m_pITableFrame->SendUserItemData(pIServerUserItem,SUB_S_AMDIN_COMMAND,&cResult,sizeof(CMD_S_CommandResult));
	}
	else if (AdminReq->cbReqType==RQ_PRINT_SYN)
	{
		cResult.cbResult=CR_ACCEPT;
		cResult.cbAckType=ACK_PRINT_SYN;
		cResult.cbSicbo1 = m_cbControlDiceValue[0];
		cResult.cbSicbo2 = m_cbControlDiceValue[1];
		cResult.cbSicbo3 = m_cbControlDiceValue[2];
		cResult.cbTotalCount = m_cbControlTotal;
		m_pITableFrame->SendUserItemData(pIServerUserItem,SUB_S_AMDIN_COMMAND,&cResult,sizeof(CMD_S_CommandResult));
	}
	else if (AdminReq->cbReqType==RQ_RESET_CONTROL)
	{
		cResult.cbResult=CR_ACCEPT;
		cResult.cbAckType=ACK_RESET_CONTROL;
		cResult.cbSicbo1 = 1;
		cResult.cbSicbo2 = 1;
		cResult.cbSicbo3 = 1;
		cResult.cbTotalCount = 0;
		m_cbNeedControl = false;
		m_bControlWin = false;
		m_cbControlDiceValue[0]=1;
		m_cbControlDiceValue[1]=1;
		m_cbControlDiceValue[2]=1;
		m_cbControlTotal = 0;
		m_pITableFrame->SendUserItemData(pIServerUserItem,SUB_S_AMDIN_COMMAND,&cResult,sizeof(CMD_S_CommandResult));
	}

	if (m_cbReqType != AdminReq->cbReqType)
	{		
		m_cbReqType = AdminReq->cbReqType;
	}

	CopyMemory(m_szControlName,pIServerUserItem->GetNickName(),sizeof(m_szControlName));	

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
	
	//��ׯ����
	WORD wBankerTime=m_nBankerTimeLimit;

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
		if (m_pITableFrame->GetGameStatus()==GAME_STATUS_FREE &&FindSuitBanker())
		{
			m_wCurrentBanker=m_ApplyUserArray[0];
		}

		//���ñ���
		bChangeBanker=true;
		m_bExchangeBanker = true;
		m_wAddTime=0;
	}
	//��ׯ�ж�
	else if (m_wCurrentBanker!=INVALID_CHAIR)
	{
		//��ȡׯ��
		IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(m_wCurrentBanker);
		SCORE lBankerScore=pIServerUserItem->GetUserScore();

		//�����ж�
		if(pIServerUserItem!= NULL)
		{
			SCORE lBankerScore=pIServerUserItem->GetUserScore();

			//�����ж�
			if (wBankerTime+m_wAddTime<=m_wBankerTime || lBankerScore<m_lApplyBankerCondition)
			{
				//ׯ�������ж� ͬһ��ׯ�������ֻ�ж�һ��
				if(wBankerTime <= m_wBankerTime && m_bExchangeBanker && lBankerScore >= m_lApplyBankerCondition)
				{
					//��ׯ�������ã���ׯ�������趨�ľ���֮��(m_wBankerTime)��
					//�������ֵ��������������ׯ���б�����������ҽ��ʱ��
					//�����ټ���ׯm_lBankerAdd�֣���ׯ���������á�

					//��ҳ���m_lExtraBankerScore֮��
					//������������ҵĽ��ֵ�������Ľ��ֵ����Ҳ�����ټ�ׯm_lBankerScoreAdd�֡�
					bool bScoreMAX = true;
					m_bExchangeBanker = false;

					for (WORD i=0; i<m_ApplyUserArray.GetCount(); ++i)
					{
						//��ȡ���
						WORD wChairID = m_ApplyUserArray[i];
						IServerUserItem *pIUserItem = m_pITableFrame->GetTableUserItem(wChairID);
						LONGLONG lScore = pIUserItem->GetUserScore();

						if ( wChairID != m_wCurrentBanker && lBankerScore <= lScore )
						{
							bScoreMAX = false;
							break;
						}
					}

					if ( bScoreMAX || (lBankerScore > m_lExtraBankerScore && m_lExtraBankerScore != 0l) )
					{
						BYTE bType = 0;
						if ( bScoreMAX && m_nBankerTimeAdd != 0 && m_nBankerTimeAdd != 0 )
						{
							bType = 1;
							m_wAddTime = m_nBankerTimeAdd;
						}
						if ( lBankerScore > m_lExtraBankerScore && m_lExtraBankerScore != 0l && m_nExtraBankerTime != 0 )
						{
							bType += 2;
							if (bType == 3)
							{
								//bType = (m_nExtraBankerTime>m_nBankerTimeAdd?2:1);
								//m_wAddTime = (m_nExtraBankerTime>m_nBankerTimeAdd?m_nExtraBankerTime:m_nBankerTimeAdd);
								m_wAddTime = m_nBankerTimeAdd + m_nExtraBankerTime;
							}
							else
								m_wAddTime = m_nExtraBankerTime;
						}

						//��ʾ��Ϣ
						TCHAR szTipMsg[128] = {};
						if (bType == 1 )
							_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("[ %s ]�ķ�����������������ׯ��ң����%d�ζ�����ׯ����!"),pIServerUserItem->GetNickName(),m_wAddTime);
						else if (bType == 2)
							_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("[ %s ]�ķ�������[%I64d]�����%d�ζ�����ׯ����!"),pIServerUserItem->GetNickName(),m_lExtraBankerScore,m_wAddTime);
						else if (bType == 3)
							_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("[ %s ]�ķ�������[%I64d]�ҳ�������������ң����%d�ζ�����ׯ����!"),pIServerUserItem->GetNickName(),m_lExtraBankerScore,m_wAddTime);
						else
							bType = 0;

						if (bType != 0)
						{
							//������Ϣ
							SendGameMessage(INVALID_CHAIR,szTipMsg);
							return true;
						}
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

				//��ʾ��Ϣ
				TCHAR szTipMsg[128];
				if (lBankerScore<m_lApplyBankerCondition)
					_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("[ %s ]��������(%I64d)��ǿ�л�ׯ!"),pIServerUserItem->GetNickName(),m_lApplyBankerCondition);
				else
					_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("[ %s ]��ׯ�����ﵽ(%d)��ǿ�л�ׯ!"),pIServerUserItem->GetNickName(),wBankerTime+m_wAddTime);

				bChangeBanker=true;
				m_bExchangeBanker = true;
				m_wAddTime = 0;

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
		m_wAddTime = 0;
	}

	//�л��ж�
	if (bChangeBanker)
	{
		//���ñ���
		m_wBankerTime = 0;
		m_lBankerWinScore=0;

		//������Ϣ
		CMD_S_ChangeBanker ChangeBanker;
		ZeroMemory(&ChangeBanker,sizeof(ChangeBanker));
		ChangeBanker.wBankerUser=m_wCurrentBanker;
		if (m_wCurrentBanker!=INVALID_CHAIR)
		{
			IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(m_wCurrentBanker);
			ChangeBanker.lBankerScore=pIServerUserItem->GetUserScore();
		}
		m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_CHANGE_BANKER,&ChangeBanker,sizeof(ChangeBanker));
		m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_CHANGE_BANKER,&ChangeBanker,sizeof(ChangeBanker));

		if (m_wCurrentBanker!=INVALID_CHAIR)
		{
			//��ȡ����
			m_pGameServiceOption=m_pITableFrame->GetGameServiceOption();
			ASSERT(m_pGameServiceOption!=NULL);

			//��ȡ��Ϣ
			int nMessageCount = 3;

			//��ȡ����
			INT nIndex=rand()%nMessageCount;
			TCHAR szMessage1[256],szMessage2[256];
			tagCustomConfig *pCustomConfig = (tagCustomConfig *)m_pGameServiceOption->cbCustomRule;
			ASSERT(pCustomConfig);
			if(0 == nIndex)
			{
				CopyMemory(szMessage1, pCustomConfig->CustomGeneral.szMessageItem1, sizeof(pCustomConfig->CustomGeneral.szMessageItem1));
			}
			else if(1 == nIndex)
			{
				CopyMemory(szMessage1, pCustomConfig->CustomGeneral.szMessageItem2, sizeof(pCustomConfig->CustomGeneral.szMessageItem2));
			}
			else if(2 == nIndex)
			{
				CopyMemory(szMessage1, pCustomConfig->CustomGeneral.szMessageItem3, sizeof(pCustomConfig->CustomGeneral.szMessageItem3));
			}

			//��ȡ���
			IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(m_wCurrentBanker);

			//������Ϣ
			_sntprintf(szMessage2,CountArray(szMessage2),TEXT("�� %s ����ׯ�ˣ�%s"),pIServerUserItem->GetNickName(), szMessage1);
			SendGameMessage(INVALID_CHAIR,szMessage2);
		}
	}

	return bChangeBanker;
}



//����ׯ��
VOID CTableFrameSink::SendApplyUser( IServerUserItem *pRcvServerUserItem )
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

//ƽ����ע
void CTableFrameSink::BalanceCurBetScore(LONGLONG lAllUserBet[],int nCount)
{
	//��С �Գ�
	SCORE lMinScore=lAllUserBet[0]>lAllUserBet[1]?lAllUserBet[1]:lAllUserBet[0];
	lAllUserBet[0]-=lMinScore;
	lAllUserBet[1]-=lMinScore;

}

//�����ע
SCORE CTableFrameSink::GetUserMaxJetton(WORD wChairID,BYTE cbAreaID)
{
	//����ע��
	SCORE lNowJetton = 0;
	SCORE lCurUserMaxBet=0;
	for (int nAreaIndex=0; nAreaIndex<AREA_COUNT; ++nAreaIndex) 
		lNowJetton += m_lUserAllBet[nAreaIndex][wChairID];
	IServerUserItem *pIMeServerUserItem=m_pITableFrame->GetTableUserItem(wChairID);
	//���½��
	lCurUserMaxBet=pIMeServerUserItem->GetUserScore()-lNowJetton;
	//��ע����
	lCurUserMaxBet=__min(lCurUserMaxBet,m_lUserLimitScore-lNowJetton);
	//��������
	lCurUserMaxBet=__min(lCurUserMaxBet,m_lAreaLimitScore-m_lAllUserBet[cbAreaID]);
	
	
	////ׯ������
	//{
	//	SCORE lBankerScore=2147483647;
	//	if(m_wCurrentBanker!=INVALID_CHAIR)
	//		lBankerScore=m_lBankerScore;

	//	SCORE lAllUserBet[AREA_COUNT];
	//	CopyMemory(lAllUserBet,m_lAllUserBet,sizeof(lAllUserBet));
	//	//�Գ�����
	//	BalanceCurBetScore(lAllUserBet,AREA_COUNT);
	//	for (int nAreaIndex=0; nAreaIndex<AREA_COUNT; ++nAreaIndex) 
	//		lBankerScore-=lAllUserBet[nAreaIndex]*g_cbAreaOdds[nAreaIndex];
	//	lCurUserMaxBet=__min(lCurUserMaxBet,lBankerScore/g_cbAreaOdds[cbAreaID]);
	//}


	//ׯ������
	{
		SCORE lBankerScore=0x7fffffff;
		if(m_wCurrentBanker!=INVALID_CHAIR)
			lBankerScore=m_lBankerScore;

		lBankerScore=lBankerScore+GetBankerWinMinScore(cbAreaID);

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
				lBankerScore = 0x7fffffff;
			}
		}

		lCurUserMaxBet=__min(lCurUserMaxBet,lBankerScore/g_cbAreaOdds[cbAreaID]);
	}
	

	//��ֵ����
	lCurUserMaxBet=__max(lCurUserMaxBet,0);
	return lCurUserMaxBet;
}


//ׯ����С�÷�
SCORE CTableFrameSink::GetBankerWinMinScore(BYTE cbAreaID)
{

	SCORE lAllUserBet[AREA_COUNT];
	CopyMemory(lAllUserBet,m_lAllUserBet,sizeof(lAllUserBet));

	SCORE lBankerWinMinScore=0;
	bool bFirst=true;
	for(int i=1;i<=6;i++)
		for(int j=1;j<=6;j++)
			for(int k=1;k<=6;k++)
			{
				BYTE cbDiceValue[DICE_COUNT];
				cbDiceValue[0]=i;
				cbDiceValue[1]=j;
				cbDiceValue[2]=k;

				int  nWinMultiple[AREA_COUNT];
				m_GameLogic.DeduceWinner(nWinMultiple,cbDiceValue);

				if(nWinMultiple[cbAreaID]<0) continue;

				SCORE lBankerWinScore=0;
				for (BYTE cbAreaIndex =0; cbAreaIndex<AREA_COUNT; cbAreaIndex++)
				{

					if (nWinMultiple[cbAreaIndex]>0) 
					{

						lBankerWinScore -= lAllUserBet[cbAreaIndex]* nWinMultiple[cbAreaIndex];

					}
					else
					{
						lBankerWinScore+= -lAllUserBet[cbAreaIndex] * nWinMultiple[cbAreaIndex];
					}

				}
				if(bFirst)
				{
					lBankerWinMinScore=lBankerWinScore;
					bFirst=false;
				}
				else
				{
					if(lBankerWinScore<lBankerWinMinScore)
					{
						lBankerWinMinScore=lBankerWinScore;

					}
				}
			}
			return lBankerWinMinScore;
}


CString CTableFrameSink::TransformCardInfo( BYTE cbCardData ) 
{
	//��������
	CString strResult(TEXT(""));

	//��С��
	if(cbCardData==0x42) {strResult = TEXT("RJ");return strResult;}
	if(cbCardData==0x41) {strResult = TEXT("BJ");return strResult;}

	//��ɫ,��ֵ
	BYTE bCardColor = m_GameLogic.GetCardColor(cbCardData);
	BYTE bCardValue = m_GameLogic.GetCardValue(cbCardData);

	//ת����ɫ
	switch( bCardColor )
	{
	case 0x00:		strResult = TEXT("D");		break; //����
	case 0x01:		strResult = TEXT("C");		break; //÷��
	case 0x02:		strResult = TEXT("H");		break; //����
	case 0x03:		strResult = TEXT("S");		break; //����
	default:		ASSERT( FALSE );				return strResult;
	}

	//ת����ֵ
	if( bCardValue > 0x01 && bCardValue < 0x0b )
	{
		strResult.AppendFormat(TEXT("%d"),bCardValue);
	}
	else
	{
		switch( bCardValue )
		{
		case 0x01:			strResult += TEXT("A");			break;
		case 0x0b:			strResult += TEXT("J");			break;
		case 0x0c:			strResult += TEXT("Q");			break;
		case 0x0d:			strResult += TEXT("K");			break;
		default:			ASSERT( FALSE );				return strResult;
		}
	}
	return strResult;
}

//����÷�
SCORE CTableFrameSink::CalculateScore(bool bProbeJudgeScore)
{
	//��������
	LONG static cbRevenue=m_pGameServiceOption->wRevenueRatio;

	//�ƶ����
	int  nWinMultiple[AREA_COUNT];
	m_GameLogic.DeduceWinner(nWinMultiple,m_cbDiceValue);
	
	if (!bProbeJudgeScore)
	{
		//�����Ϸ��¼
		tagGameRecord &GameRecord = m_GameRecordList[m_nRecordLast];
		memcpy(GameRecord.cbDiceValue,m_cbDiceValue,sizeof(GameRecord.cbDiceValue));
		m_nRecordLast = (m_nRecordLast+1)%MAX_SCORE_HISTORY;
		if ( m_nRecordLast == m_nRecordFirst)
			m_nRecordFirst = (m_nRecordFirst+1)%MAX_SCORE_HISTORY;
	}
	

	//ׯ������
	SCORE lBankerWinScore = 0;

	//ϵͳ��Ӯ
	LONGLONG lSystemScore = 0l;

	//��ҳɼ�
	SCORE lUserLostScore[GAME_PLAYER];
	ZeroMemory(m_lUserRevenue, sizeof(m_lUserRevenue));
	ZeroMemory(lUserLostScore, sizeof(lUserLostScore));
	ZeroMemory(m_lUserWinScore, sizeof(m_lUserWinScore));
	ZeroMemory(m_lUserReturnScore, sizeof(m_lUserReturnScore));
	
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
		if (m_wCurrentBanker==wChairID) continue;

		//��ȡ�û�
		IServerUserItem * pIServerUserItem=m_pITableFrame->GetTableUserItem(wChairID);
		if (pIServerUserItem==NULL) continue;

		bool bIsAndroidUser = pIServerUserItem->IsAndroidUser();

		for (BYTE cbAreaIndex =0; cbAreaIndex<AREA_COUNT; cbAreaIndex++)
		{
		
			if(m_lUserAllBet[cbAreaIndex][wChairID]<=0) continue;
			if (nWinMultiple[cbAreaIndex]>0) 
			{
				
				m_lUserWinScore[wChairID] +=m_lUserAllBet[cbAreaIndex][wChairID] * nWinMultiple[cbAreaIndex];
				m_lUserReturnScore[wChairID] +=m_lUserAllBet[cbAreaIndex][wChairID] ;

				//ϵͳ�÷�
				if(bIsAndroidUser)
					lSystemScore += m_lUserAllBet[cbAreaIndex][wChairID] * nWinMultiple[cbAreaIndex];

				if (m_wCurrentBanker == INVALID_CHAIR || bIsBankerAndroidUser)
					lSystemScore -= m_lUserAllBet[cbAreaIndex][wChairID] * nWinMultiple[cbAreaIndex];
				
				
			}
			else
			{
				lUserLostScore[wChairID] +=m_lUserAllBet[cbAreaIndex][wChairID] * nWinMultiple[cbAreaIndex];

				//ϵͳ�÷�
				if(bIsAndroidUser)
					lSystemScore += m_lUserAllBet[cbAreaIndex][wChairID] * nWinMultiple[cbAreaIndex];
				if (m_wCurrentBanker == INVALID_CHAIR || bIsBankerAndroidUser)
					lSystemScore -= m_lUserAllBet[cbAreaIndex][wChairID] * nWinMultiple[cbAreaIndex];
			}
			lBankerWinScore -= m_lUserAllBet[cbAreaIndex][wChairID] * nWinMultiple[cbAreaIndex];
		}


		m_lUserWinScore[wChairID] += lUserLostScore[wChairID];

		//����˰��
		if (m_lUserWinScore[wChairID]>0)
		{
			float fRevenuePer=float(cbRevenue/1000.0);
			m_lUserRevenue[wChairID]  = static_cast<SCORE>(m_lUserWinScore[wChairID]*fRevenuePer);
			m_lUserWinScore[wChairID] -= m_lUserRevenue[wChairID];
		}

		
	}

	//ׯ�ҳɼ�
	if (m_wCurrentBanker!=INVALID_CHAIR)
	{
		for (WORD wChairID=0; wChairID<GAME_PLAYER; wChairID++)
		{
			for (BYTE cbAreaIndex =0; cbAreaIndex<AREA_COUNT; cbAreaIndex++)
			{
				if (nWinMultiple[cbAreaIndex] < 0) 
				{
					m_lUserWinScore[m_wCurrentBanker] += m_lUserAllBet[cbAreaIndex][wChairID];
				}
				else
					m_lUserWinScore[m_wCurrentBanker] -= m_lUserAllBet[cbAreaIndex][wChairID]*nWinMultiple[cbAreaIndex];
			}
		}

		//����˰��
		if (0 < m_lUserWinScore[m_wCurrentBanker])
		{
			float fRevenuePer=float(cbRevenue/1000.0);
			m_lUserRevenue[m_wCurrentBanker]  =static_cast<LONG>(m_lUserWinScore[m_wCurrentBanker]*fRevenuePer);
			m_lUserWinScore[m_wCurrentBanker] -= m_lUserRevenue[m_wCurrentBanker];
			lBankerWinScore = m_lUserWinScore[m_wCurrentBanker];
		}	
	}

	if (!bProbeJudgeScore)
	{
		//�ۼƻ���
		m_lBankerWinScore += lBankerWinScore;
		//��ǰ����
		m_lBankerCurGameScore=lBankerWinScore;

		if (!m_cbNeedControl && !m_bControlWin)
		{
			//д���
			WriteStorage(lSystemScore);
		}		
	}

	m_lCurSystemWinScore=lSystemScore;
	
	return lBankerWinScore;
}

//��ѯ�޶�
SCORE CTableFrameSink::QueryConsumeQuota(IServerUserItem * pIServerUserItem)
{
	if (pIServerUserItem->GetUserStatus()==US_PLAYING)
	{
		return 0;
	}
	else
	{
		return __max(pIServerUserItem->GetUserScore()-m_pGameServiceOption->lMinEnterScore-10L,0L);
	}
}

//��ѯ�Ƿ�۷����
bool CTableFrameSink::QueryBuckleServiceCharge(WORD wChairID)
{
	for (BYTE i=0;i<GAME_PLAYER;i++)
	{
		IServerUserItem *pUserItem=m_pITableFrame->GetTableUserItem(i);
		if(pUserItem==NULL) continue;

		if (m_wCurrentBanker==i) return true;

		if(wChairID==i)
		{
			//������ע
			for (int nAreaIndex=0; nAreaIndex<AREA_COUNT; ++nAreaIndex) 
			{

				if (m_lUserAllBet[nAreaIndex][wChairID] != 0)
				{
					return true;
				}
			}
			break;
		}
	}
	return false;
}


//���ͼ�¼
VOID CTableFrameSink::SendGameRecord(IServerUserItem *pIServerUserItem)
{
	WORD wBufferSize=0;
	BYTE cbBuffer[SOCKET_TCP_BUFFER];
	int nIndex = m_nRecordFirst;
	while (nIndex !=m_nRecordLast)
	{
		CopyMemory(cbBuffer+wBufferSize,&m_GameRecordList[nIndex],sizeof(tagGameRecord));
		wBufferSize+=sizeof(tagGameRecord);
		nIndex = (nIndex+1) % MAX_SCORE_HISTORY;
	}
	if (wBufferSize>0) 
		m_pITableFrame->SendUserItemData(pIServerUserItem,SUB_S_SEND_RECORD,cbBuffer,wBufferSize);
}

//������Ϣ
VOID CTableFrameSink::SendGameMessage(WORD wChairID, LPCTSTR pszTipMsg)
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


//�������
bool CTableFrameSink::PlayerFlee(WORD wChairID,IServerUserItem*pIServerUserItem)
{
	if(pIServerUserItem==NULL) return true;

	if(m_pITableFrame->GetGameStatus()==GS_GAME_END)
	{
		//ʤ������
		//enScoreKind ScoreKind=(m_lUserWinScore[wChairID]>0L)?enScoreKind_Win:enScoreKind_Lost;
		//д�����
		if (m_lUserWinScore[wChairID]!=0L)
		{
			
			tagScoreInfo ScoreInfo[GAME_PLAYER];
			ZeroMemory(ScoreInfo,sizeof(ScoreInfo));
			ScoreInfo[wChairID].cbType=(m_lUserWinScore[wChairID]>0L)?SCORE_TYPE_WIN:SCORE_TYPE_LOSE;
			ScoreInfo[wChairID].lRevenue=m_lUserRevenue[wChairID];
			ScoreInfo[wChairID].lScore=m_lUserWinScore[wChairID];
			m_pITableFrame->WriteTableScore(ScoreInfo,CountArray(ScoreInfo));
			m_lUserRevenue[wChairID]=0;
			m_lUserWinScore[wChairID]=0;

			//IServerUserItem *pIServerUserItem = m_pITableFrame->GetTableUserItem(wChairID);
			//if(!pIServerUserItem->IsAndroidUser())
			//m_lStorageCurrent-=ScoreInfo[wChairID].lScore;
		}

		for(BYTE cbIndex=0;cbIndex<AREA_COUNT;cbIndex++)
			m_lUserAllBet[cbIndex][wChairID]=0;
	}
	else
	{
		if(m_pITableFrame->GetGameStatus()==GS_PLAYER_BET)
		{
			BYTE cbIndex=0;
			CMD_S_RevocatBet RevocatBet={0};
			for(cbIndex=0;cbIndex<AREA_COUNT;cbIndex++)
			{
				RevocatBet.lUserBet[cbIndex]=m_lUserAllBet[cbIndex][wChairID];
				m_lAllUserBet[cbIndex]-=m_lUserAllBet[cbIndex][wChairID];
				m_lUserAllBet[cbIndex][wChairID]=0;
			}
			//������Ϣ
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_REVOCAT_BET,&RevocatBet,sizeof(CMD_S_RevocatBet));
		}
	}
	return  true;
}


//��ׯ���� (������)
VOID CTableFrameSink::RobotBankerControl()
{
	//�Ƿ������ׯ������
	//if (m_ApplyUserArray.GetCount()-1 > m_nRobotBankerApplayMaxCount)
	//	return;


	////��������
	//int nRobotBankerCount = m_nRobotBankerApplayMinCount + rand()%(m_nRobotBankerApplayMaxCount-m_nRobotBankerApplayMinCount)-(m_ApplyUserArray.GetCount()-1);
	//int nSendBankerMsgCount = 0;
	//int nRandNum = rand() % GAME_PLAYER;

	////����ׯ����������
	//for (int i = 0; i < GAME_PLAYER; i++)
	//{
	//	WORD wChairID = (nRandNum+i) % GAME_PLAYER;
	//	IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(wChairID);
	//	tagRobotConfig robotConfig = {};

	//	if (pIServerUserItem != NULL && pIServerUserItem->IsAndroidUser())
	//	{
	//		if (-1 == m_RobotCfgManage.GetRobotConfigType(pIServerUserItem->GetNickName(), &robotConfig))
	//			continue;

	//		SCORE lBankerCondition = (robotConfig.lBankerCondition>m_lApplyBankerCondition?robotConfig.lBankerCondition:m_lApplyBankerCondition);
	//		if ( m_wCurrentBanker != wChairID && pIServerUserItem->GetUserScore() >= lBankerCondition)
	//		{
	//			//�����ׯ�б�
	//			bool bOnApplayArray = false;
	//			for (int j = 0; j < m_ApplyUserArray.GetCount(); j++)
	//			{
	//				if (wChairID == m_ApplyUserArray[j]) 
	//				{
	//					bOnApplayArray = true;
	//					break;
	//				}
	//			}

	//			if (!bOnApplayArray)
	//			{
	//				//������Ϣ
	//				m_pITableFrame->SendTableData(wChairID, SUB_S_ROBOT_BANKER);

	//				//MyDebug(_T("RobotBankerControl B %d [%d/%d]"), wChairID, nSendBankerMsgCount+1, nRobotBankerCount);

	//				//����ͳ��
	//				nSendBankerMsgCount++;
	//				if (nSendBankerMsgCount >= nRobotBankerCount)
	//					break;
	//			}
	//		}
	//	}
	//}
}

//���ÿ��� (������)
VOID CTableFrameSink::RobotConfigControl()
{
	//������Ϣ
	CString strRobotName[100];
	BYTE bRobotCount = 0;
	for (int i = 0; i < GAME_PLAYER; i++)
	{
		IServerUserItem *pUserItem = m_pITableFrame->GetTableUserItem(i);
		if (pUserItem != NULL && pUserItem->IsAndroidUser())
		{
			strRobotName[bRobotCount].Format(_T("%s"), pUserItem->GetNickName());
			bRobotCount++;
		}
	}

	//��̬����
	m_RobotCfgManage.RobotDynamicManage(strRobotName, bRobotCount);

	//��������
	CMD_S_RobotConfig robotConfig = {};
	for (int i = 0; i < GAME_PLAYER; i++)
	{
		IServerUserItem *pUserItem = m_pITableFrame->GetTableUserItem(i);
		if (pUserItem == NULL || !pUserItem->IsAndroidUser())
			continue;

		ZeroMemory(&robotConfig, sizeof(robotConfig));
		if (-1 != m_RobotCfgManage.GetRobotConfigType(pUserItem->GetNickName(), &robotConfig.robotConfig))
			m_pITableFrame->SendTableData(i, SUB_S_ROBOT_CONFIG, &robotConfig, sizeof(robotConfig));	
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

		//���ñ���
		CancelBanker.wCancelUser=wChairID;

		//������Ϣ
		m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));
		m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));

		break;
	}

	return true;
}
//�û�����
bool  CTableFrameSink::OnActionUserSitDown(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	//��ʼ����
	m_lUserStartScore[wChairID] = pIServerUserItem->GetUserScore();
	//����ʱ��
	if ((bLookonUser==false)&&(m_dwStartTime==0L))
	{
		m_dwStartTime=(DWORD)time(NULL);
		m_pITableFrame->SetGameTimer(IDI_FREE,m_dwTimeGameFree*1000,1,NULL);
		m_pITableFrame->SetGameStatus(GAME_STATUS_FREE);
	}

	//������ʾ
	TCHAR szTipMsg[138];
	_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("��������ׯ����Ϊ:%I64d,��������Ϊ:%I64d,�������Ϊ:%I64d"),m_lApplyBankerCondition,m_lAreaLimitScore,m_lUserLimitScore);
	m_pITableFrame->SendGameMessage(pIServerUserItem,szTipMsg,SMT_CHAT);

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
		//�������
		BYTE cbGameStatus=m_pITableFrame->GetGameStatus();
	   if(pIServerUserItem->GetUserStatus()!=US_PLAYING&&(cbGameStatus==GS_PLAYER_BET||cbGameStatus==GS_GAME_END))
	   {
		   PlayerFlee(wChairID,pIServerUserItem);
	   }
		
		//�л�ׯ��
		if (wChairID==m_wCurrentBanker)
		{
			ChangeBanker(true);
			m_bContiueCard=false;
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
			SCORE lUserScore=pIServerUserItem->GetUserScore();
			if(lUserScore<m_lApplyBankerCondition)
			{
				//������Ϣ
				CMD_S_CancelBanker CancelBanker;
				ZeroMemory(&CancelBanker,sizeof(CancelBanker));

				//���ñ���
				CancelBanker.wCancelUser = pIServerUserItem->GetChairID();

				//������Ϣ
				m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));
				m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));

				//��ʾ��Ϣ
				TCHAR szTipMsg[128];
				_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("������Ľ����������ׯ����������%I64d��,���Զ���ׯ��"),m_lApplyBankerCondition);
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
//��ȡ����
void CTableFrameSink::GetStockScore(SCORE *lAndroidScore,SCORE *lUserScore,BYTE bDice[])
{
	//ϵͳ��ׯ
	bool bIsAndroid=false;
	if(m_wCurrentBanker != INVALID_CHAIR)
	{
		IServerUserItem *pIServerUser=m_pITableFrame->GetTableUserItem(m_wCurrentBanker);
		if(pIServerUser == NULL)ASSERT(false);
		if(pIServerUser->IsAndroidUser() == TRUE)
			bIsAndroid=true;
	}
	else
	{
		bIsAndroid=true;
	}

	//�������
	SCORE AllScore=0;
	bool  bWinFlags[AREA_COUNT]={0};
	int   nWinMultiple[AREA_COUNT];
	SCORE lBankerWinScore=0l;
	m_GameLogic.DeduceWinner(nWinMultiple,bDice);

	for(BYTE i=0;i<GAME_PLAYER;i++)
	{
		IServerUserItem *pIServerUser=m_pITableFrame->GetTableUserItem(i);
		if(pIServerUser == NULL || i == m_wCurrentBanker)continue;

		for(BYTE j=0;j<AREA_COUNT;j++)
		{
			if(nWinMultiple[j] != -1)
			{
				if(pIServerUser->IsAndroidUser() == true && bIsAndroid != true)
				{
					*lAndroidScore+=m_lUserAllBet[j][i]*nWinMultiple[j];
				}
				else
					*lUserScore+=m_lUserAllBet[j][i]*nWinMultiple[j];
			}
			else
				lBankerWinScore+=m_lUserAllBet[j][i];
		}
	}

	//ׯ�ҷ���
	if(bIsAndroid == true)
	{
		*lAndroidScore+=lBankerWinScore;
	}
	else
	{
		*lUserScore+=lBankerWinScore;
	}
}

//���·���
bool CTableFrameSink::DispatchTableLabel()
{
	//�ɷ��˿�
	for(BYTE cbIndex=0;cbIndex<DICE_COUNT;cbIndex++)
	{
		m_cbDiceValue[cbIndex]=rand()%6+1;
	}

	return true;
}

//�ж������Ӯ״̬
BYTE CTableFrameSink::ControlUserWin()
{
	BYTE cbUserWinType = 0;

	//�ƶ����
	int  nWinMultiple[AREA_COUNT];
	m_GameLogic.DeduceWinner(nWinMultiple,m_cbDiceValue);

	//ׯ������
	SCORE lBankerWinScore = 0;

	//ϵͳ��Ӯ
	LONGLONG lSystemScore = 0l;

	//��ҳɼ�
	SCORE lUserLostScore[GAME_PLAYER];
	SCORE lUserWinScore[GAME_PLAYER];
	ZeroMemory(lUserLostScore, sizeof(lUserLostScore));
	ZeroMemory(lUserWinScore, sizeof(lUserWinScore));

	//�������
	for (WORD wChairID=0; wChairID<GAME_PLAYER; wChairID++)
	{
		//ׯ���ж�
		if (m_wCurrentBanker==wChairID) continue;

		//��ȡ�û�
		IServerUserItem * pIServerUserItem=m_pITableFrame->GetTableUserItem(wChairID);
		if (pIServerUserItem==NULL) continue;
		if (m_lQueryGameID!=pIServerUserItem->GetGameID())	continue;	

		for (BYTE cbAreaIndex =0; cbAreaIndex<AREA_COUNT; cbAreaIndex++)
		{
			if(m_lUserAllBet[cbAreaIndex][wChairID]<=0) continue;
			if (nWinMultiple[cbAreaIndex]>0) 
			{
				lUserWinScore[wChairID] +=m_lUserAllBet[cbAreaIndex][wChairID] * nWinMultiple[cbAreaIndex];
			}
			else
			{
				lUserLostScore[wChairID] +=m_lUserAllBet[cbAreaIndex][wChairID] * nWinMultiple[cbAreaIndex];
			}
		}

		lUserWinScore[wChairID] += lUserLostScore[wChairID];

		//����˰��
		if (lUserWinScore[wChairID]>=0)
		{
			cbUserWinType = 1;
		}
		else
			cbUserWinType = 2;
	}

	//����ׯ�ң������˵�ׯʱ�����������Ϳ��Ʋ��˸�����
	////ׯ�ҳɼ�
	//if (m_wCurrentBanker!=INVALID_CHAIR)
	//{
	//	for (WORD wChairID=0; wChairID<GAME_PLAYER; wChairID++)
	//	{
	//		for (BYTE cbAreaIndex =0; cbAreaIndex<AREA_COUNT; cbAreaIndex++)
	//		{
	//			if (nWinMultiple[cbAreaIndex] < 0) 
	//			{
	//				lUserWinScore[m_wCurrentBanker] += m_lUserAllBet[cbAreaIndex][wChairID];
	//			}
	//			else
	//				lUserWinScore[m_wCurrentBanker] -= m_lUserAllBet[cbAreaIndex][wChairID]*nWinMultiple[cbAreaIndex];
	//		}
	//	}

	//	//����˰��
	//	if (lUserWinScore[m_wCurrentBanker]>=0)
	//	{
	//		cbUserWinType = 1;
	//	}
	//	else
	//		cbUserWinType = 2;
	//}

	return cbUserWinType;
}

//��̽���ж�
bool CTableFrameSink::ProbeJudge()
{
	//ϵͳ��Ӯ
	LONGLONG lSystemScore = 0l;
	{
		m_lCurSystemWinScore = 0L;
		CalculateScore(true);
		lSystemScore=m_lCurSystemWinScore;

		// 11/11/2013 [Wan.MY] 
		// ��������ͷ��ж�
		if(( m_lStorageCurrent > m_lStorageMax1 && m_lStorageCurrent <= m_lStorageMax2 && rand()%100 <= m_lStorageMul1 && lSystemScore != 0 )||
			(m_lStorageCurrent > m_lStorageMax2 && rand()%100 <= m_lStorageMul2))
		{
			// ���ϵͳ����ӮǮ������������һ��
			if( lSystemScore > 0 )
			{
				return false;
			}

			// ���䣬OK
			if( m_lStorageCurrent + lSystemScore >= 0)
			{
				return true;
			}

			// ��Ǯ�����ǿ�治����Ǯ, ����һ�־Ͳ������ӮǮ��
			if( m_lStorageCurrent + lSystemScore < 0 )
			{				
				return false;
			}
		}


		//ϵͳ��ֵ����
		if ((lSystemScore + m_lStorageCurrent) < 0l)
		{
			return false;
		}
		else
		{
			return true;
		}


	}
	return true;
}


//д���
void CTableFrameSink::WriteStorage(LONGLONG lSystemScore)
{
	m_lStorageCurrent += lSystemScore;
	if (m_lStorageCurrent>0)
	{
		m_lStorageCurrent = m_lStorageCurrent - (m_lStorageCurrent * m_nStorageDeduct / 1000);
	}
	
	CString strLog;
	strLog.Format(TEXT("[��%d��]����ǰ��� %I64d"),m_pITableFrame->GetTableID()+1,m_lStorageCurrent);
	NcaTextOut(strLog);
}

//����д��Ϣ
void CTableFrameSink::WriteInfo( LPCTSTR pszFileName, LPCTSTR pszString )
{
	//������������
	char* old_locale = _strdup( setlocale(LC_CTYPE,NULL) );
	setlocale( LC_CTYPE, "chs" );

	CStdioFile myFile;
	CString strFileName;
	strFileName.Format(TEXT("%s"), pszFileName);
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
	m_nBankerTimeLimit = pCustomConfig->CustomGeneral.lBankerTime;
	m_nBankerTimeAdd = pCustomConfig->CustomGeneral.lBankerTimeAdd;
	m_lExtraBankerScore = pCustomConfig->CustomGeneral.lBankerScoreMAX;
	m_nExtraBankerTime = pCustomConfig->CustomGeneral.lBankerTimeExtra;
	m_bEnableSysBanker = (pCustomConfig->CustomGeneral.nEnableSysBanker == TRUE)?true:false;

	//ʱ��
	m_dwTimeGameFree = pCustomConfig->CustomGeneral.lFreeTime;
	m_dwTimeGameBet = pCustomConfig->CustomGeneral.lBetTime;
	m_dwTimeGameEnd = pCustomConfig->CustomGeneral.lEndTime;
	if(m_dwTimeGameFree < TIME_FREE	|| m_dwTimeGameFree > 99) m_dwTimeGameFree = TIME_FREE;
	if(m_dwTimeGameBet < TIME_PLACE_JETTON || m_dwTimeGameBet > 99) m_dwTimeGameBet = TIME_PLACE_JETTON;
	if(m_dwTimeGameEnd < TIME_GAME_END || m_dwTimeGameEnd > 99) m_dwTimeGameEnd = TIME_GAME_END;

	//��ע
	m_lAreaLimitScore = pCustomConfig->CustomGeneral.lAreaLimitScore;
	m_lUserLimitScore = pCustomConfig->CustomGeneral.lUserLimitScore;
	m_nEndGameMul = pCustomConfig->CustomGeneral.lEndGameMul;
	if(m_nEndGameMul < 1 || m_nEndGameMul > 100 ) m_nEndGameMul = 80;

	//���
	m_lStorageStart = pCustomConfig->CustomGeneral.StorageStart;
	m_lStorageCurrent = m_lStorageStart;
	m_nStorageDeduct = pCustomConfig->CustomGeneral.StorageDeduct;
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

	return;
}

//������ע��Ϣ
void CTableFrameSink::SendUserBetInfo( IServerUserItem *pIServerUserItem )
{
	if(NULL == pIServerUserItem) return;

	//Ȩ���ж�
	if(!CUserRight::IsGameCheatUser(pIServerUserItem->GetUserRight())) return;

	//��������
	CMD_S_SendUserBetInfo SendUserBetInfo;
	ZeroMemory(&SendUserBetInfo, sizeof(SendUserBetInfo));

	CopyMemory(&SendUserBetInfo.lUserStartScore, m_lUserStartScore, sizeof(m_lUserStartScore));

	for (int i=0;i<AREA_COUNT;i++)
	{
		if (m_lUserAllBet[i] > 0)
		{
			CopyMemory(&SendUserBetInfo.lUserJettonScore, m_lUserAllBet[i], sizeof(m_lUserAllBet[i]));
			SendUserBetInfo.cbAreaCount = i;
			//������Ϣ	
			m_pITableFrame->SendUserItemData(pIServerUserItem, SUB_S_SEND_USER_BET_INFO, &SendUserBetInfo, sizeof(SendUserBetInfo));
		}		
	}	

	return;
}