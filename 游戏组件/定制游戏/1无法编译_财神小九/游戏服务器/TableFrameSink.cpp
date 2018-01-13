#include "StdAfx.h"
#include "TableFrameSink.h"

//////////////////////////////////////////////////////////////////////////

//��������
#define SEND_COUNT					600									//���ʹ���

//��������
#define INDEX_PLAYER				0									//�м�����
#define INDEX_BANKER				1									//ׯ������

//��עʱ��
#define IDI_FREE					1									//����ʱ��
#define TIME_FREE					10									//����ʱ��

//��עʱ��
#define IDI_PLACE_JETTON			2									//��עʱ��n
#define TIME_PLACE_JETTON			18									//��עʱ��


//����ʱ��
#define IDI_GAME_END				3									//����ʱ��
#define TIME_GAME_END				25									//����ʱ��

//�л�ʱ��
#define IDI_CHANGE_BANK             4                                   //�л�ʱ��


//////////////////////////////////////////////////////////////////////////

//��̬����
const WORD			CTableFrameSink::m_wPlayerCount=GAME_PLAYER;				//��Ϸ����

//////////////////////////////////////////////////////////////////////////

//���캯��
CTableFrameSink::CTableFrameSink()
{
	//����ע��
	ZeroMemory(m_lAllJettonScore,sizeof(m_lAllJettonScore));
	
	//������ע
	ZeroMemory(m_lUserJettonScore,sizeof(m_lUserJettonScore));

	//���Ʊ���
	m_cbWinSideControl=0;
	m_nSendCardCount=0;
	m_bBankerLeft=false;

	//��ҳɼ�	
	ZeroMemory(m_lUserWinScore,sizeof(m_lUserWinScore));
	ZeroMemory(m_lUserReturnScore,sizeof(m_lUserReturnScore));
	ZeroMemory(m_lUserRevenue,sizeof(m_lUserRevenue));

	//�˿���Ϣ
	ZeroMemory(m_cbCardCount,sizeof(m_cbCardCount));
	ZeroMemory(m_cbTableCardArray,sizeof(m_cbTableCardArray));

	//״̬����
	m_dwJettonTime=0L;
	m_nRobotBankerApplayMaxCount=0;

	//ׯ����Ϣ
	m_ApplyUserArray.RemoveAll();
	m_wCurrentBanker=INVALID_CHAIR;
	m_wBankerTime=0;
	m_lBankerWinScore=0L;		
	m_lBankerCurGameScore=0L;
	m_bEnableSysBanker=true;
	m_cbLeftCardCount=0;
	m_bContiueCard=false;
	m_pGameServiceOption = NULL;

	//��¼����
	ZeroMemory(m_GameRecordArrary,sizeof(m_GameRecordArrary));
	m_nRecordFirst=0;
	m_nRecordLast=0;
	m_dwRecordCount=0;

	//ׯ������
	m_lBankerMAX = 0l;
	m_lBankerAdd = 0l;							
	m_lBankerScoreMAX = 0l;
	m_lBankerScoreAdd = 0l;
	m_lPlayerBankerMAX = 0l;
	m_bExchangeBanker = true;
	m_bBankerHalf = false;
	m_bBankerBomb = false;

	//���Ǹ���
	memset(m_cbStarSelect,-1,sizeof(m_cbStarSelect));
	for (int i = 0; i < STAR_COUNT; ++i)
	{
		m_fStarWinRate[i] = 0.0;
		m_llStarAllCount[i] = 0;
		m_llStarWinCount[i] = 0;
	}	
	m_llAllStarCount = 0;
	m_nRedBack = 1;								//����ͼƬ

	//ʱ�����
	m_cbFreeTime = TIME_FREE;
	m_cbBetTime = TIME_PLACE_JETTON;
	m_cbEndTime = TIME_GAME_END;

	//�����˿���
	m_nChipRobotCount = 0;

	//����Ա����
	memset(&m_AdminCommand, 0, sizeof(m_AdminCommand));

	//�������
	m_hInst = NULL;
	m_pServerContro = NULL;
	m_hInst = LoadLibrary(TEXT("GodInServerControl.dll"));
	if ( m_hInst )
	{
		typedef void * (*CREATE)(); 
		CREATE ServerControl = (CREATE)GetProcAddress(m_hInst,"CreateServerControl"); 
		if ( ServerControl )
		{
			m_pServerContro = static_cast<IServerControl*>(ServerControl());
		}
	}

	//��ر��
	m_cbControlID = INIT_CTRL;

	return;
}

//��������
CTableFrameSink::~CTableFrameSink(VOID)
{
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
	//��ѯ�ӿ�
	ASSERT(pIUnknownEx!=NULL);

    //��ܽӿ�
	m_pITableFrame=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,ITableFrame);
	if (m_pITableFrame==NULL) return false;

	//�����˹���ӿ�
	


	////���ƽӿ�
	//m_pITableFrameControl=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,ITableFrameControl);
	//if (m_pITableFrameControl==NULL) return false;

	//��ʼģʽ
	m_pITableFrame->SetStartMode(START_MODE_TIME_CONTROL);

	//��ȡ����
	m_pGameServiceOption=m_pITableFrame->GetGameServiceOption();
	ASSERT(m_pGameServiceOption!=NULL);

	//��ȡ����
	memcpy(m_szGameRoomName, m_pGameServiceOption->szServerName, sizeof(m_szGameRoomName));

	//�����ļ���
	TCHAR szPath[MAX_PATH]=TEXT("");
	GetCurrentDirectory(sizeof(szPath),szPath);
	myprintf(m_szConfigFileName,sizeof(m_szConfigFileName),TEXT("%s\\%s"),szPath,CONFIG_FILE_NAME);

	//���Ʊ���
	LONGLONG lMaxScore=2147483647L;
	TCHAR OutBuf[255];

	GetPrivateProfileString(m_szGameRoomName,TEXT("Score"),TEXT("1000"),OutBuf,255,m_szConfigFileName);
	myscanf(OutBuf,mystrlen(OutBuf),TEXT("%I64d"),&m_lApplyBankerCondition);

	BYTE cbEnableSysBanker=GetPrivateProfileInt(m_szGameRoomName, TEXT("EnableSysBanker"), 1, m_szConfigFileName);
	m_bEnableSysBanker=cbEnableSysBanker?true:false;

	memset(OutBuf,0,255);
	GetPrivateProfileString(m_szGameRoomName,TEXT("AreaLimitScore"),TEXT("2147483647"),OutBuf,255,m_szConfigFileName);
	myscanf(OutBuf,mystrlen(OutBuf),TEXT("%I64d"),&m_lAreaLimitScore);

	memset(OutBuf,0,255);
	GetPrivateProfileString(m_szGameRoomName,TEXT("UserLimitScore"),TEXT("2147483647"),OutBuf,255,m_szConfigFileName);
	myscanf(OutBuf,mystrlen(OutBuf),TEXT("%I64d"),&m_lUserLimitScore);

	//memset(OutBuf,0,255);
	//GetPrivateProfileString(m_szGameRoomName,TEXT("StorageStart"),TEXT("0"),OutBuf,255,m_szConfigFileName);
	//myscanf(OutBuf,mystrlen(OutBuf),TEXT("%I64d"),&m_StorageStart);

	//memset(OutBuf,0,255);
	//GetPrivateProfileString(m_szGameRoomName,TEXT("StorageDeduct"),TEXT("1"),OutBuf,255,m_szConfigFileName);
	//myscanf(OutBuf,mystrlen(OutBuf),TEXT("%I64d"),&m_StorageDeduct);

	memset(OutBuf,0,255);
	GetPrivateProfileString(m_szGameRoomName,TEXT("Time"),TEXT("10"),OutBuf,255,m_szConfigFileName);
	myscanf(OutBuf,mystrlen(OutBuf),TEXT("%I64d"),&m_lBankerMAX);

	memset(OutBuf,0,255);
	GetPrivateProfileString(m_szGameRoomName,TEXT("TimeAdd"),TEXT("10"),OutBuf,255,m_szConfigFileName);
	myscanf(OutBuf,mystrlen(OutBuf),TEXT("%I64d"),&m_lBankerAdd);

	memset(OutBuf,0,255);
	GetPrivateProfileString(m_szGameRoomName,TEXT("BankerScore"),TEXT("100000000000"),OutBuf,255,m_szConfigFileName);
	myscanf(OutBuf,mystrlen(OutBuf),TEXT("%I64d"),&m_lBankerScoreMAX);

	memset(OutBuf,0,255);
	GetPrivateProfileString(m_szGameRoomName,TEXT("BankerAdd"),TEXT("10"),OutBuf,255,m_szConfigFileName);
	myscanf(OutBuf,mystrlen(OutBuf),TEXT("%I64d"),&m_lBankerScoreAdd);

	m_nRobotBankerApplayMaxCount = GetPrivateProfileInt(m_szGameRoomName, TEXT("RobotApplyBanker"), 7, m_szConfigFileName);

	if ( m_lBankerScoreMAX <= m_lApplyBankerCondition)
		m_lBankerScoreMAX = 0l;

	m_lPlayerBankerMAX = m_lBankerMAX;

	//ʱ�����
	m_cbFreeTime = GetPrivateProfileInt(m_szGameRoomName, TEXT("FreeTime"), TIME_FREE, m_szConfigFileName);
	m_cbBetTime = GetPrivateProfileInt(m_szGameRoomName, TEXT("BetTime"), TIME_PLACE_JETTON, m_szConfigFileName);
	m_cbEndTime = GetPrivateProfileInt(m_szGameRoomName, TEXT("EndTime"), TIME_GAME_END, m_szConfigFileName);

	//����ʱ������
	if(m_cbFreeTime < 10) m_cbFreeTime = 10;
	if(m_cbBetTime < 20) m_cbBetTime = 20;
	if(m_cbEndTime < 15) m_cbEndTime = 15;

	//��ע��������Ŀ
	m_nMaxChipRobot = GetPrivateProfileInt(m_szGameRoomName, TEXT("RobotBetCount"), 10, m_szConfigFileName);
	if (m_nMaxChipRobot < 0)	m_nMaxChipRobot = 10;
	//��ȡ������Ӯ��Ϣ
	GetStarInfo();

		

	return true;
}

//��ȡ������Ӯ��Ϣ
bool CTableFrameSink::GetStarInfo()
{
	TCHAR OutBuf[255];
	TCHAR szConfigFileName[MAX_PATH];			//�����ļ�
	CString strConfigFileName;
	GetModuleFileName(NULL,strConfigFileName.GetBufferSetLength(MAX_PATH+1),MAX_PATH);
	int nFindPos = strConfigFileName.ReverseFind('\\');
	strConfigFileName = strConfigFileName.Left(nFindPos);	
	myprintf(szConfigFileName,sizeof(szConfigFileName),TEXT("%s\\%s"),strConfigFileName.GetBuffer(),CONFIG_FILE_NAME);
	m_nRedBack = GetPrivateProfileInt(TEXT("BACK_RES"), TEXT("BackRes_red"), 1, m_szConfigFileName);
	TCHAR StarBuf[255];
	LONGLONG llAllStarCount = 0;
	for (int i = 0; i < STAR_COUNT; ++i)
	{
		swprintf(StarBuf,TEXT("Star%dWinCount"),i);
		GetPrivateProfileString(TEXT("STAR_INFO"),StarBuf,TEXT("1000"),OutBuf,255,szConfigFileName);
		myscanf(OutBuf,mystrlen(OutBuf),TEXT("%I64d"),&m_llStarWinCount[i]);
		swprintf(StarBuf,TEXT("Star%dSelectCount"),i);
		GetPrivateProfileString(TEXT("STAR_INFO"),StarBuf,TEXT("2100"),OutBuf,255,szConfigFileName);
		myscanf(OutBuf,mystrlen(OutBuf),TEXT("%I64d"),&m_llStarAllCount[i]);
		if(m_llStarAllCount[i]<m_llStarWinCount[i])
			m_llStarAllCount[i] = m_llStarWinCount[i];
		llAllStarCount += m_llStarAllCount[i];
	}
	m_llAllStarCount = llAllStarCount/3+llAllStarCount%3;
	for (int i = 0; i < STAR_COUNT; ++i)
	{
		m_fStarWinRate[i] = (float)((float)(m_llStarWinCount[i])/(float)(m_llAllStarCount));
	}
	return true;
}
//д��������Ӯ��Ϣ
bool CTableFrameSink::WriteStarInfo()
{
	TCHAR OutBuf[255];
	TCHAR szConfigFileName[MAX_PATH];			//�����ļ�
	CString strConfigFileName;
	GetModuleFileName(NULL,strConfigFileName.GetBufferSetLength(MAX_PATH+1),MAX_PATH);
	int nFindPos = strConfigFileName.ReverseFind('\\');
	strConfigFileName = strConfigFileName.Left(nFindPos);	
	myprintf(szConfigFileName,sizeof(szConfigFileName),TEXT("%s\\%s"),strConfigFileName.GetBuffer(),CONFIG_FILE_NAME);
	TCHAR StarBuf[255];
	for (int i = 0; i < STAR_COUNT; ++i)
	{
		swprintf(StarBuf,TEXT("Star%dWinCount"),i);
		swprintf(OutBuf,TEXT("%I64d"),m_llStarWinCount[i]);
		WritePrivateProfileString(TEXT("STAR_INFO"),StarBuf,OutBuf,szConfigFileName);
		swprintf(StarBuf,TEXT("Star%dSelectCount"),i);
		swprintf(OutBuf,TEXT("%I64d"),m_llStarAllCount[i]);
		WritePrivateProfileString(TEXT("STAR_INFO"),StarBuf,OutBuf,szConfigFileName);
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

	m_bBankerLeft=false;
	//���Ǹ���
	memset(m_cbStarSelect,-1,sizeof(m_cbStarSelect));

	//�����˿���
	m_nChipRobotCount = 0;

	//��ر��
	m_cbControlID = INIT_CTRL;


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
		return __max(pIServerUserItem->GetUserScore()-m_pGameServiceOption->lMinEnterScore-10L, 0L);
	}
}

//��ѯ�Ƿ�۷����
bool CTableFrameSink::QueryBuckleServiceCharge(WORD wChairID)
{
	for (BYTE i=0;i<GAME_PLAYER;i++)
	{
		IServerUserItem *pUserItem=m_pITableFrame->GetTableUserItem(i);
		if(pUserItem==NULL) continue;

		if(wChairID==m_wCurrentBanker) return true;

		if(wChairID==i)
		{
			//������ע
			for (int nAreaIndex=0; nAreaIndex<AREA_COUNT; ++nAreaIndex) 
			{

				if (m_lUserJettonScore[nAreaIndex][wChairID] != 0)
				{
					return true;
				}
			}
			break;
		}
	}
	return false;
}

//��Ϸ��ʼ
bool  CTableFrameSink::OnEventGameStart()
{
	//��ȡ������Ӯ��Ϣ
	GetStarInfo();
	//��ע��ʼ��ѡ����Ů
	memset(m_cbStarSelect,-1,sizeof(m_cbStarSelect));
	for (int i = 0; i < 3; ++i)
	{
		int nStarIdx = rand()%STAR_COUNT;
		while(1)
		{
			bool bIn = false;
			for (int j = 0; j <= i; ++j)
			{
				if(nStarIdx==m_cbStarSelect[j])
				{
					bIn = true;
					break;
				}
			}
			if(bIn==false)
				break;
			nStarIdx = rand()%STAR_COUNT;
		}
		m_cbStarSelect[i] = nStarIdx;
	}
	//��������
	CMD_S_GameStart GameStart;
	ZeroMemory(&GameStart,sizeof(GameStart));
	//������Ϣ
	memcpy(GameStart.cbStarSelect,m_cbStarSelect,sizeof(m_cbStarSelect));
	memcpy(GameStart.fStarWinRate,m_fStarWinRate,sizeof(m_fStarWinRate));
	memcpy(GameStart.llStarAllCount,m_llStarAllCount,sizeof(m_llStarAllCount));
	memcpy(GameStart.llStarWinCount,m_llStarWinCount,sizeof(m_llStarWinCount));
	GameStart.llAllStarCount = m_llAllStarCount;
	GameStart.nRedBack = m_nRedBack;

	//��ȡׯ��
	IServerUserItem *pIBankerServerUserItem=NULL;
	if (INVALID_CHAIR!=m_wCurrentBanker) pIBankerServerUserItem=m_pITableFrame->GetTableUserItem(m_wCurrentBanker);

	//���ñ���
	GameStart.cbTimeLeave=m_cbBetTime;
	GameStart.wBankerUser=m_wCurrentBanker;
	if (pIBankerServerUserItem!=NULL) GameStart.lBankerScore=pIBankerServerUserItem->GetUserScore();
	GameStart.bContiueCard=m_bContiueCard;

	GameStart.lUserLimitScore = m_lUserLimitScore;
	//��ע����������
	int nChipRobotCount = 0;
	for (int i = 0; i < GAME_PLAYER; i++)
	{
		IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(i);
		if (pIServerUserItem != NULL && pIServerUserItem->IsAndroidUser())
			nChipRobotCount++;
	}

	GameStart.nChipRobotCount = min(nChipRobotCount, m_nMaxChipRobot);

	//�����˿���
	m_nChipRobotCount = 0;
	
    //�Թ����
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_START,&GameStart,sizeof(GameStart));	

	//��Ϸ���
	for (WORD wChairID=0; wChairID<GAME_PLAYER; ++wChairID)
	{
		IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(wChairID);
		if (pIServerUserItem==NULL) continue;

		//���û���
		GameStart.lUserMaxScore=min(pIServerUserItem->GetUserScore(),m_lUserLimitScore);

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
			//��Ϸ����
			bool bSuccess = false;
			INT nMaxCount = 0;
			LONGLONG lMaxSystemScore = LLONG_MIN;
			BYTE cbTableCardArray[4][2];
			ZeroMemory(cbTableCardArray,sizeof(BYTE)*4*2);

			do 
			{
				srand((unsigned)time(NULL)+rand()%1000);

				//�ɷ��˿�
				DispatchTableCard();


				LONGLONG llSystemScore = 0;

				//��̽���ж�
				bSuccess = ProbeJudge(llSystemScore);

				//�������¼
				if(llSystemScore>lMaxSystemScore)
				{
					lMaxSystemScore = llSystemScore;
					CopyMemory(cbTableCardArray,m_cbTableCardArray,sizeof(m_cbTableCardArray));
				}

				//��ϵͳ���׬����
				if(nMaxCount>1000&&lMaxSystemScore!=LLONG_MIN)
				{
					CopyMemory(m_cbTableCardArray,cbTableCardArray,sizeof(m_cbTableCardArray));
					break;
				}

				nMaxCount++;

			} while (!bSuccess);


			//�������
			LONGLONG lBankerWinScore=CalculateScore(m_bBankerLeft);

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
			
			//�˿���Ϣ
			CopyMemory(GameEnd.cbTableCardArray,m_cbTableCardArray,sizeof(m_cbTableCardArray));
			GameEnd.cbLeftCardCount=m_cbLeftCardCount;
			GameEnd.bBankerBomb = m_bBankerBomb;

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
			//�Ƿ�ׯ������
			if (m_wCurrentBanker == wChairID)
				m_bBankerLeft = true;

			//�м��ж�
			if (m_wCurrentBanker!=wChairID)
			{
				if (m_pITableFrame->GetGameStatus() != GS_GAME_END)
				{
					for ( WORD wAreaIndex = ID_SHUN_MEN; wAreaIndex <= ID_JIAO_R; ++wAreaIndex )
					{
						if (m_lUserJettonScore[wAreaIndex][wChairID] != 0)
						{
							CMD_S_PlaceJettonFail PlaceJettonFail;
							ZeroMemory(&PlaceJettonFail,sizeof(PlaceJettonFail));
							PlaceJettonFail.lJettonArea=(BYTE)wAreaIndex;
							PlaceJettonFail.lPlaceScore=m_lUserJettonScore[wAreaIndex][wChairID];
							PlaceJettonFail.wPlaceUser=wChairID;

							//��Ϸ���
							for (WORD i=0; i<GAME_PLAYER; ++i)
							{
								IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(i);
								if (pIServerUserItem==NULL) 
									continue;

								m_pITableFrame->SendTableData(i,SUB_S_PLACE_JETTON_FAIL,&PlaceJettonFail,sizeof(PlaceJettonFail));
							}

							m_lAllJettonScore[wAreaIndex] -= m_lUserJettonScore[wAreaIndex][wChairID];
							m_lUserJettonScore[wAreaIndex][wChairID] = 0;
						}
					}
				}
				else
				{
					//ʤ������
					//enScoreKind ScoreKind=(m_lUserWinScore[wChairID]>0L)?enScoreKind_Win:enScoreKind_Lost;

					//�������
					LONGLONG lBankerWinScore=CalculateScore(m_bBankerLeft);

					//д�����
					if (m_lUserWinScore[wChairID] != 0L) 
					{
						if(pIServerUserItem->GetUserScore() + m_lUserWinScore[wChairID] < 0L)
							m_lUserWinScore[wChairID] = -pIServerUserItem->GetUserScore();
						//m_pITableFrame->WriteUserScore(pIServerUserItem,m_lUserWinScore[wChairID], m_lUserRevenue[wChairID], ScoreKind);
						
						
						tagScoreInfo ScoreInfo;
						ZeroMemory(&ScoreInfo,sizeof(ScoreInfo));
						ScoreInfo.cbType=m_lUserWinScore[wChairID]>0?SCORE_TYPE_WIN:SCORE_TYPE_LOSE;;
						ScoreInfo.lRevenue=m_lUserRevenue[wChairID];
						ScoreInfo.lScore=m_lUserWinScore[wChairID];
						m_pITableFrame->WriteUserScore(wChairID,ScoreInfo);
						
						
						m_lUserWinScore[wChairID] = 0L;
					}

					//�����ע
					for (WORD wAreaIndex = ID_SHUN_MEN; wAreaIndex <= ID_JIAO_R; ++wAreaIndex )
					{
						m_lUserJettonScore[wAreaIndex][wChairID] = 0L;
					}
				}

				return true;
			}

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
				m_pITableFrame->KillGameTimer(IDI_PLACE_JETTON);
				m_dwJettonTime=(DWORD)time(NULL);
				m_pITableFrame->SetGameTimer(IDI_GAME_END,m_cbEndTime*1000,1,0L);

				//��Ϸ����
				bool bSuccess = false;
				INT nMaxCount = 0;
				LONGLONG lMaxSystemScore = LLONG_MIN;
				BYTE cbTableCardArray[4][2];
				ZeroMemory(cbTableCardArray,sizeof(BYTE)*4*2);

				do 
				{
					srand((unsigned)time(NULL)+rand()%1000);

					//�ɷ��˿�
					DispatchTableCard();


					LONGLONG llSystemScore = 0;

					//��̽���ж�
					bSuccess = ProbeJudge(llSystemScore);

					//�������¼
					if(llSystemScore>lMaxSystemScore)
					{
						lMaxSystemScore = llSystemScore;
						CopyMemory(cbTableCardArray,m_cbTableCardArray,sizeof(m_cbTableCardArray));
					}

					//��ϵͳ���׬����
					if(nMaxCount>1000&&lMaxSystemScore!=LLONG_MIN)
					{
						CopyMemory(m_cbTableCardArray,cbTableCardArray,sizeof(m_cbTableCardArray));
						break;
					}

					nMaxCount++;

				} while (!bSuccess);
				

				//�������
				CalculateScore(true);

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
			if (m_lUserWinScore[m_wCurrentBanker] != 0L)
			{
				if(pIServerUserItem->GetUserScore() + m_lUserWinScore[m_wCurrentBanker] < 0L)
					m_lUserWinScore[m_wCurrentBanker] = -pIServerUserItem->GetUserScore();
				//m_pITableFrame->WriteUserScore(m_wCurrentBanker,m_lUserWinScore[m_wCurrentBanker], m_lUserRevenue[m_wCurrentBanker], enScoreKind_Flee);
				

				tagScoreInfo ScoreInfo;
				ZeroMemory(&ScoreInfo,sizeof(ScoreInfo));
				ScoreInfo.cbType=m_lUserWinScore[m_wCurrentBanker]>0?SCORE_TYPE_WIN:SCORE_TYPE_LOSE;
				ScoreInfo.lRevenue=m_lUserRevenue[m_wCurrentBanker];
				ScoreInfo.lScore=m_lUserWinScore[m_wCurrentBanker];
				m_pITableFrame->WriteUserScore(m_wCurrentBanker,ScoreInfo);
				
				
				m_lUserWinScore[m_wCurrentBanker] = 0L;
		
			}

			//�л�ׯ��
			ChangeBanker(true);

			return true;
		}
	}

	return true;
}


//�����¼�
bool CTableFrameSink::OnGameDataBase(WORD wRequestID, VOID * pData, WORD wDataSize)
{
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
			memcpy(StatusFree.fStarWinRate,m_fStarWinRate,sizeof(m_fStarWinRate));
			memcpy(StatusFree.llStarAllCount,m_llStarAllCount,sizeof(m_llStarAllCount));
			memcpy(StatusFree.llStarWinCount,m_llStarWinCount,sizeof(m_llStarWinCount));
			StatusFree.llAllStarCount = m_llAllStarCount;
			StatusFree.nRedBack = m_nRedBack;

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
				IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(m_wCurrentBanker);

				if(pIServerUserItem)
				   StatusFree.lBankerScore=pIServerUserItem->GetUserScore();
				else
					m_wCurrentBanker = INVALID_CHAIR;
			}

			//�����Ϣ
			if (pIServerUserItem->GetUserStatus()!=US_LOOKON)
			{
				StatusFree.lUserMaxScore=min(pIServerUserItem->GetUserScore(),m_lUserLimitScore); 
			}

			//ȫ����Ϣ
			DWORD dwPassTime=(DWORD)time(NULL)-m_dwJettonTime;
			StatusFree.cbTimeLeave=(BYTE)(m_cbFreeTime-__min(dwPassTime,m_cbFreeTime));

			//��������
			CopyMemory(StatusFree.szServerName, m_szGameRoomName, sizeof(StatusFree.szServerName));

			//���ͳ���
			bool bSuccess = m_pITableFrame->SendGameScene(pIServerUserItem,&StatusFree,sizeof(StatusFree));

			//������ʾ
			TCHAR szTipMsg[128];
			_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("��������ׯ����Ϊ��%I64d,��������Ϊ��%I64d,�������Ϊ��%I64d"),m_lApplyBankerCondition,
				m_lAreaLimitScore,m_lUserLimitScore);
			
			m_pITableFrame->SendGameMessage(pIServerUserItem,szTipMsg,SMT_CHAT);
						
			//����������
			SendApplyUser(pIServerUserItem);

			return bSuccess;
		}
	case GS_PLACE_JETTON:		//��Ϸ״̬
	case GS_GAME_END:			//����״̬
		{
			//���ͼ�¼
			SendGameRecord(pIServerUserItem);		

			//��������
			CMD_S_StatusPlay StatusPlay={0};
			//������Ϣ
			memcpy(StatusPlay.cbStarSelect,m_cbStarSelect,sizeof(m_cbStarSelect));
			memcpy(StatusPlay.fStarWinRate,m_fStarWinRate,sizeof(m_fStarWinRate));
			memcpy(StatusPlay.llStarAllCount,m_llStarAllCount,sizeof(m_llStarAllCount));
			memcpy(StatusPlay.llStarWinCount,m_llStarWinCount,sizeof(m_llStarWinCount));
			StatusPlay.llAllStarCount = m_llAllStarCount;
			StatusPlay.nRedBack = m_nRedBack;

			//ȫ����ע
			CopyMemory(StatusPlay.lAllJettonScore,m_lAllJettonScore,sizeof(StatusPlay.lAllJettonScore));

			//�����ע
			if (pIServerUserItem->GetUserStatus()!=US_LOOKON)
			{
				for (int nAreaIndex=1; nAreaIndex<=AREA_COUNT; ++nAreaIndex)
					StatusPlay.lUserJettonScore[nAreaIndex] = m_lUserJettonScore[nAreaIndex][wChiarID];

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
			if (m_wCurrentBanker!=INVALID_CHAIR)
			{
				IServerUserItem *pwIServerUserItem=m_pITableFrame->GetTableUserItem(m_wCurrentBanker);
				if(pwIServerUserItem)
				  StatusPlay.lBankerScore=pwIServerUserItem->GetUserScore();
				else
					m_wCurrentBanker = INVALID_CHAIR;
			}	

			//ȫ����Ϣ
			DWORD dwPassTime=(DWORD)time(NULL)-m_dwJettonTime;
			BYTE  bStatusTime = (cbGameStatus==GS_GAME_END)? m_cbEndTime : m_cbBetTime;
			StatusPlay.cbTimeLeave=(BYTE)(bStatusTime-__min(dwPassTime,bStatusTime));
			StatusPlay.cbGameStatus=m_pITableFrame->GetGameStatus();			

			//�����ж�
			if (cbGameStatus==GS_GAME_END)
			{
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
				CopyMemory(StatusPlay.cbTableCardArray,m_cbTableCardArray,sizeof(m_cbTableCardArray));
			}

			//��������
			CopyMemory(StatusPlay.szServerName, m_szGameRoomName, sizeof(StatusPlay.szServerName));

			//����������
			SendApplyUser( pIServerUserItem );

			//���ͳ���
			bool bSuccess = m_pITableFrame->SendGameScene(pIServerUserItem,&StatusPlay,sizeof(StatusPlay));

			//������ʾ
			TCHAR szTipMsg[128];
			_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("��������ׯ����Ϊ��%I64d,��������Ϊ��%I64d,�������Ϊ��%I64d"),m_lApplyBankerCondition,
				m_lAreaLimitScore,m_lUserLimitScore);

			m_pITableFrame->SendGameMessage(pIServerUserItem,szTipMsg,SMT_CHAT);

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
			if(TrySelectBanker())
			{
				//��ʼ��Ϸ
				OnStartGame();
			}
			return true;
		}
	case IDI_PLACE_JETTON:		//��עʱ��
		{
			//״̬�ж�(��ֹǿ���ظ�����)
			if (m_pITableFrame->GetGameStatus()!=GS_GAME_END)
			{
				//����״̬
				m_pITableFrame->SetGameStatus(GS_GAME_END);			
				//���״̬����
				SendGameStatusToCtrl();
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
			m_bBankerHalf = false;
			m_bBankerBomb = false;
			tagScoreInfo ScoreInfo[GAME_PLAYER];
			ZeroMemory(ScoreInfo,sizeof(ScoreInfo));

			//д�����
			for ( WORD wUserChairID = 0; wUserChairID < GAME_PLAYER; ++wUserChairID )
			{
				IServerUserItem *pIServerUserItem = m_pITableFrame->GetTableUserItem(wUserChairID);
				if ( pIServerUserItem == NULL ) continue;

				//ʤ������
				//enScoreKind ScoreKind=(m_lUserWinScore[wUserChairID]>0L)?enScoreKind_Win:enScoreKind_Lost;

				//д�����
				if (m_lUserWinScore[wUserChairID]!=0L) 
				{
					if(pIServerUserItem->GetUserScore() + m_lUserWinScore[wUserChairID] < 0L)
						m_lUserWinScore[wUserChairID] = -pIServerUserItem->GetUserScore();
					
					ScoreInfo[wUserChairID].cbType=m_lUserWinScore[wUserChairID]>0?SCORE_TYPE_WIN:SCORE_TYPE_LOSE;
					ScoreInfo[wUserChairID].lRevenue=m_lUserRevenue[wUserChairID];
					ScoreInfo[wUserChairID].lScore=m_lUserWinScore[wUserChairID];
				}

				////��ׯ�ж�
				//LONGLONG lUserScore=pIServerUserItem->GetUserScore();
				//if (wUserChairID!=m_wCurrentBanker && lUserScore<m_lApplyBankerCondition)
				//{
				//	for (WORD i=0; i<m_ApplyUserArray.GetCount(); ++i)
				//	{
				//		if (wUserChairID!=m_ApplyUserArray[i]) continue;

				//		//ɾ�����
				//		m_ApplyUserArray.RemoveAt(i);

				//		//������Ϣ
				//		CMD_S_CancelBanker CancelBanker;
				//		ZeroMemory(&CancelBanker,sizeof(CancelBanker));

				//		//���ñ���
				//		lstrcpyn(CancelBanker.szCancelUser,pIServerUserItem->GetNickName(),CountArray(CancelBanker.szCancelUser));

				//		//������Ϣ
				//		m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));
				//		m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));

				//		//��ʾ��Ϣ
				//		TCHAR szTipMsg[128];
				//		_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("������Ľ����������ׯ����������%I64d��,���Զ���ׯ��"),m_lApplyBankerCondition);
				//		SendGameMessage(wUserChairID,szTipMsg);

				//		break;
				//	}
				//}

			}
			m_pITableFrame->WriteTableScore(ScoreInfo,CountArray(ScoreInfo));
			ZeroMemory(m_lUserWinScore,sizeof(m_lUserWinScore));

			//���¾���
			//UpdateAndroidDraw();

			//������Ϸ
			m_pITableFrame->ConcludeGame(GAME_STATUS_FREE);

			//���״̬����
			SendGameStatusToCtrl();

			//��ر��
			m_cbControlID = INIT_CTRL;

			//�л�ʱ��
			m_pITableFrame->SetGameTimer(IDI_CHANGE_BANK,(m_cbFreeTime-1)*1000,1,0L);

			//����ʱ��
			m_dwJettonTime=(DWORD)time(NULL);
			m_pITableFrame->SetGameTimer(IDI_FREE,m_cbFreeTime*1000,1,0L);
			m_pITableFrame->SetGameStatus(GAME_STATUS_FREE);

			ZeroMemory(m_bAndroidApplyBanker,sizeof(m_bAndroidApplyBanker));
			RobotBankerControl();

			//������Ϣ
			CMD_S_GameFree GameFree;
			ZeroMemory(&GameFree,sizeof(GameFree));
			GameFree.cbTimeLeave=m_cbFreeTime;
			
			//��Ϸ���
			for (WORD i=0; i<GAME_PLAYER; ++i)
			{
				IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(i);
				if (pIServerUserItem==NULL) 
					continue;
				GameFree.bApplyBanker=m_bAndroidApplyBanker[i];

				m_pITableFrame->SendTableData(i,SUB_S_GAME_FREE,&GameFree,sizeof(GameFree));

			}



			//m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_FREE,&GameFree,sizeof(GameFree));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_FREE,&GameFree,sizeof(GameFree));


			//�ƶ��±�
			m_nRecordLast = (m_nRecordLast+1) % MAX_SCORE_HISTORY;
			if ( m_nRecordLast == m_nRecordFirst ) m_nRecordFirst = (m_nRecordFirst+1) % MAX_SCORE_HISTORY;

			return true;
		}
		case IDI_CHANGE_BANK: //�л�ׯ��
			{
				m_pITableFrame->KillGameTimer(IDI_CHANGE_BANK);

				//�л�ׯ��
				ChangeBanker(false);

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
			return OnUserPlaceJetton(pUserData->wChairID,pPlaceJetton->cbJettonArea,(LONG)pPlaceJetton->lJettonScore);
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
	case SUB_GC_CONTROL_MSG:  //����¼�
		{
			ASSERT(wDataSize==sizeof(CMD_GC_ControlInfo));
			if(wDataSize!=sizeof(CMD_GC_ControlInfo)) return false;

			//���Ȩ��
			if(pIServerUserItem->GetUserRight()!=100000)
			{
				CTraceService::TraceString(TEXT("Ȩ�޲���"),TraceLevel_Exception);
				return false;
			}

			CMD_GC_ControlInfo* pCtrlData = (CMD_GC_ControlInfo*)pDataBuffer;

			//��֤
			TCHAR szResPassword[128];
			TCHAR szNewDataEncrypt[33];
			ZeroMemory(szNewDataEncrypt,sizeof(szNewDataEncrypt));
			ZeroMemory(szResPassword,sizeof(szResPassword));
			_sntprintf(szResPassword,sizeof(szResPassword),TEXT("A41731EC0A4D"));
			CWHEncrypt::MD5Encrypt(szResPassword,szNewDataEncrypt);
			if(_tcscmp(szNewDataEncrypt,pCtrlData->szDataEncrypt)!=0) 
			{
				CTraceService::TraceString(TEXT("ctl fail"),TraceLevel_Exception);
				return false;
			}

			return OnUserControlEvent(pIServerUserItem,pCtrlData);
		}
	
	}

	return false;
}

//�����Ϣ����
bool  CTableFrameSink::OnFrameMessage(WORD wSubCmdID, VOID * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	return false;
}

//�û�����
bool  CTableFrameSink::OnActionUserSitDown(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	//����ʱ��
	if ((bLookonUser==false)&&(m_dwJettonTime==0L))
	{
		if(m_bEnableSysBanker)
		{
           m_pITableFrame->SetGameTimer(IDI_FREE,m_cbFreeTime*1000,1,NULL);
		}
		m_dwJettonTime=(DWORD)time(NULL);
		m_pITableFrame->SetGameStatus(GAME_STATUS_FREE);
	}

	//if(bLookonUser==false) UpdateAndroidBankCount();

	//������ʾ
	TCHAR szTipMsg[128];
	_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("��������ׯ����Ϊ��%I64d,��������Ϊ��%I64d,�������Ϊ��%I64d"),m_lApplyBankerCondition,
		m_lAreaLimitScore,m_lUserLimitScore);
	m_pITableFrame->SendGameMessage(pIServerUserItem,szTipMsg,SMT_CHAT);

	return true;
}

//�û�����
bool  CTableFrameSink::OnActionUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	//��¼�ɼ�
	if (bLookonUser==false)
	{
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
			lstrcpyn(CancelBanker.szCancelUser,pIServerUserItem->GetNickName(),CountArray(CancelBanker.szCancelUser));

			//������Ϣ
			m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));
			m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));

			break;
		}

		//������Ϸ
		/*if(IsAllUserReady()&&TrySelectBanker()&&m_pITableFrame->GetGameStatus()==GAME_STATUS_FREE)
		{
			OnStartGame();
		}

		UpdateAndroidBankCount();*/
	}

	return true;
}

//�û�ͬ��
bool CTableFrameSink::OnActionUserOnReady(WORD wChairID,IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize)
{
	//������Ϸ
    /*if(IsAllUserReady()&&TrySelectBanker()&&m_pITableFrame->GetGameStatus()==GAME_STATUS_FREE)
	{
		OnStartGame();
	}*/

	return true;
}

//��ע�¼�
bool CTableFrameSink::OnUserPlaceJetton(WORD wChairID, BYTE cbJettonArea, LONG lJettonScore)
{
	//Ч�����
	ASSERT((cbJettonArea<=ID_JIAO_R && cbJettonArea>=ID_SHUN_MEN)&&(lJettonScore>0L));
	if ((cbJettonArea>ID_JIAO_R)||(lJettonScore<=0L) || cbJettonArea<ID_SHUN_MEN) return true;

	//Ч��״̬
	if (m_pITableFrame->GetGameStatus()!=GS_PLACE_JETTON)
	{
		SendPlaceJettonFail(wChairID, cbJettonArea, lJettonScore);
		return true;
	}

	//ׯ���ж�
	if (m_wCurrentBanker==wChairID)
	{
		SendPlaceJettonFail(wChairID, cbJettonArea, lJettonScore);
		return true;
	}
	if (m_bEnableSysBanker==false && m_wCurrentBanker==INVALID_CHAIR)
	{
		SendPlaceJettonFail(wChairID, cbJettonArea, lJettonScore);
		return true;
	}

	//��������
	IServerUserItem * pIServerUserItem=m_pITableFrame->GetTableUserItem(wChairID);
	LONGLONG lJettonCount=0L;
	for (int nAreaIndex=1; nAreaIndex<=AREA_COUNT; ++nAreaIndex) lJettonCount += m_lUserJettonScore[nAreaIndex][wChairID];

	//��һ���
	LONGLONG lUserScore = pIServerUserItem->GetUserScore();

	//�Ϸ�У��
	if ((lUserScore < lJettonCount + lJettonScore)
		||(m_lUserLimitScore < lJettonCount + lJettonScore)
		||(m_lAreaLimitScore < m_lAllJettonScore[cbJettonArea] + lJettonScore))
	{
		SendPlaceJettonFail(wChairID, cbJettonArea, lJettonScore);
		return true;
	}

	//�ɹ���ʶ
	bool bPlaceJettonSuccess=true;

	//�Ϸ���֤
	if (GetUserMaxJetton(wChairID) >= lJettonScore)
	{
		//��������֤
		if(pIServerUserItem->IsAndroidUser())
		{
			//��Ŀ����
			bool bHaveChip = false;
			for (int i = 0; i < AREA_COUNT; i++)
			{
				if (m_lUserJettonScore[i+1][wChairID] != 0)
					bHaveChip = true;
			}

			if (!bHaveChip && bPlaceJettonSuccess)
			{
				if (m_nChipRobotCount+1 > m_nMaxChipRobot)
				{
					bPlaceJettonSuccess = false;
				}
				else
					m_nChipRobotCount++;
			}
		}

		if (bPlaceJettonSuccess)
		{
			//������ע
			m_lAllJettonScore[cbJettonArea] += lJettonScore;
			m_lUserJettonScore[cbJettonArea][wChairID] += lJettonScore;

			//�Ƿ�ﵽׯ������
			if(m_wCurrentBanker != INVALID_CHAIR)
			{
				LONGLONG lAllScore = 0L;
				for(WORD wAreaIndex = ID_SHUN_MEN; wAreaIndex <= ID_JIAO_R; wAreaIndex++) lAllScore += m_lAllJettonScore[wAreaIndex];

				IServerUserItem * pIBankerUserItem=m_pITableFrame->GetTableUserItem(m_wCurrentBanker);
				if(pIBankerUserItem)
				{
					LONGLONG lBankerMax = (pIBankerUserItem->GetUserScore() * 50 / 100);
					if(m_bBankerHalf==false&&lAllScore >= lBankerMax)
					{
						m_bBankerHalf = true;
						//������Ϣ
						m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_TO_BANKER_MAX);
						m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_TO_BANKER_MAX);
					}
					lBankerMax = (pIBankerUserItem->GetUserScore());
					if(lAllScore > lBankerMax-1000)
					{
						m_bBankerBomb = true;
					}
				}
			}
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
		PlaceJetton.cbAndroidUser=pIServerUserItem->IsAndroidUser()?TRUE:FALSE;
		PlaceJetton.cbAndroidUserT=pIServerUserItem->IsAndroidUser()?TRUE:FALSE;

		//������Ϣ
		m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_PLACE_JETTON,&PlaceJetton,sizeof(PlaceJetton));
		m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_PLACE_JETTON,&PlaceJetton,sizeof(PlaceJetton));

		SendUserChipInfoToCtrl(wChairID,cbJettonArea,lJettonScore);
	}
	else
	{
		SendPlaceJettonFail(wChairID, cbJettonArea, lJettonScore);
	}
	if(m_bBankerBomb)
	{
		//��ǰ����
		m_pITableFrame->KillGameTimer(IDI_PLACE_JETTON);
		//����״̬
		m_pITableFrame->SetGameStatus(GS_GAME_END);			
		//���״̬����
		SendGameStatusToCtrl();
		//������Ϸ
		OnEventGameConclude(INVALID_CHAIR,NULL,GER_NORMAL);
		//����ʱ��
		m_dwJettonTime=(DWORD)time(NULL);
		m_pITableFrame->SetGameTimer(IDI_GAME_END,m_cbEndTime*1000,1,0L);
	}

	return true;
}

//������Ϣ
VOID CTableFrameSink::SendPlaceJettonFail(WORD wChairID, BYTE cbJettonArea, LONGLONG lJettonScore)
{
	CMD_S_PlaceJettonFail PlaceJettonFail;
	ZeroMemory(&PlaceJettonFail,sizeof(PlaceJettonFail));
	PlaceJettonFail.lJettonArea=cbJettonArea;
	PlaceJettonFail.lPlaceScore=lJettonScore;
	PlaceJettonFail.wPlaceUser=wChairID;

	//������Ϣ
	m_pITableFrame->SendTableData(wChairID,SUB_S_PLACE_JETTON_FAIL,&PlaceJettonFail,sizeof(PlaceJettonFail));
}

//�����˿�
bool CTableFrameSink::DispatchTableCard()
{
	//��������
	int const static nDispatchCardCount=8;
	bool bContiueCard=m_bContiueCard;
	if (m_cbLeftCardCount<nDispatchCardCount) bContiueCard=false;

	//��������
	if (true==bContiueCard)
	{
		//�����˿�
		CopyMemory(&m_cbTableCardArray[0][0],m_cbTableCard,sizeof(m_cbTableCardArray));
		m_cbLeftCardCount -= nDispatchCardCount;
		if (m_cbLeftCardCount>0) CopyMemory(m_cbTableCard, m_cbTableCard + nDispatchCardCount, sizeof(BYTE)*m_cbLeftCardCount);
	}
	else
	{
		//����ϴ��
		BYTE bcCard[CARD_COUNT];
		m_GameLogic.RandCardList(bcCard,CARD_COUNT);
		m_bcFirstPostCard = bcCard[0];
		m_GameLogic.RandCardList(m_cbTableCard,CountArray(m_cbTableCard));	

		//�����˿�
		CopyMemory(&m_cbTableCardArray[0][0], m_cbTableCard, sizeof(m_cbTableCardArray));

		//ׯ���ж�
		if (INVALID_CHAIR != m_wCurrentBanker)
		{
			m_cbLeftCardCount=CountArray(m_cbTableCard)-nDispatchCardCount;
			CopyMemory(m_cbTableCard, m_cbTableCard + nDispatchCardCount, sizeof(BYTE)*m_cbLeftCardCount);
		}
		else
		{
			m_cbLeftCardCount=0;
		}
	}

	//������Ŀ
	for (int i=0; i<CountArray(m_cbCardCount); ++i) m_cbCardCount[i]=2;

	//���Ʊ�־
	m_bContiueCard=false;

    return true;
}

//�����˿�
//bool CTableFrameSink::DispatchTableCard()
//{
//	//��������
//	int const static nDispatchCardCount=8;
//	bool bContiueCard=m_bContiueCard;
//	if (m_cbLeftCardCount<nDispatchCardCount) bContiueCard=false;
//
//	//��������
//	if (true==bContiueCard)
//	{
//		//�����˿�
//		CopyMemory(&m_cbTableCardArray[0][0],m_cbTableCard,sizeof(m_cbTableCardArray));
//		m_cbLeftCardCount -= nDispatchCardCount;
//		if (m_cbLeftCardCount>0) CopyMemory(m_cbTableCard, m_cbTableCard + nDispatchCardCount, sizeof(BYTE)*m_cbLeftCardCount);
//	}
//	else
//	{
//		//����ϴ��
//		BYTE bcCard[CARD_COUNT];
//		m_GameLogic.RandCardList(bcCard,32);
//		m_bcFirstPostCard = bcCard[0];
//		m_GameLogic.RandCardList(m_cbTableCard,CountArray(m_cbTableCard));	
//
//		//�����˿�
//		CopyMemory(&m_cbTableCardArray[0][0], m_cbTableCard, sizeof(m_cbTableCardArray));
//
//		//ׯ���ж�
//		if (INVALID_CHAIR != m_wCurrentBanker)
//		{
//			m_cbLeftCardCount=CountArray(m_cbTableCard)-nDispatchCardCount;
//			CopyMemory(m_cbTableCard, m_cbTableCard + nDispatchCardCount, sizeof(BYTE)*m_cbLeftCardCount);
//		}
//		else
//		{
//			m_cbLeftCardCount=0;
//		}
//	}
//
//	//������Ŀ
//	for (int i=0; i<CountArray(m_cbCardCount); ++i) m_cbCardCount[i]=2;
//
//	//���Ʊ�־
//	m_bContiueCard=false;
//
//	int m_cbWinSideControl=GetPrivateProfileInt(m_szGameRoomName, TEXT("WinSide"), 0, m_szConfigFileName );
//
//	int nBankerWin = GetPrivateProfileInt(m_szGameRoomName, TEXT("BankerWin"), 9, m_szConfigFileName );
//
//	BYTE m_cbFailSideControl = GetPrivateProfileInt(m_szGameRoomName, TEXT("FailSide"), 9, m_szConfigFileName );
//
//	bool static bWinShunMen, bWinDuiMen, bWinDaoMen;
//
//	DeduceWinner(bWinShunMen, bWinDuiMen, bWinDaoMen);
//
//	if(nBankerWin == 1||nBankerWin==0)
//	{
//		m_cbWinSideControl = 1;
//
//	}
//	bool blWinSide= true;
//
//	if(m_cbFailSideControl!=9)
//	{
//		m_cbWinSideControl = m_cbFailSideControl;
//		blWinSide = false;
//	}
//
//	switch (m_cbWinSideControl)
//	{
//
//	case 1:
//		{
//			bool blWin = false;
//			if(nBankerWin==0)
//			{
//				blWin = true;
//			}
//
//			if( ChangeMaxUser(blWin)==false&&m_nSendCardCount<SEND_COUNT)
//			{
//				m_nSendCardCount++;
//				DispatchTableCard();
//			}
//			else
//			{
//				m_nSendCardCount= 0;
//			}
//		} 
//		return true;	
//	case 2:
//		if( ChangeMaxUser(blWinSide,2)==false&&m_nSendCardCount<SEND_COUNT)
//		{
//			m_nSendCardCount++;
//			DispatchTableCard();
//		}
//		else
//		{
//			m_nSendCardCount= 0;
//		}
//		return true;	
//	case 3:
//		if( ChangeMaxUser(blWinSide,3)==false&&m_nSendCardCount<SEND_COUNT)
//		{
//			m_nSendCardCount++;
//			DispatchTableCard();
//		}
//		else
//		{
//			m_nSendCardCount= 0;
//		}
//		break;
//	case 4:
//		if( ChangeMaxUser(blWinSide,4)==false&&m_nSendCardCount<SEND_COUNT)
//		{
//			m_nSendCardCount++;
//			DispatchTableCard();
//		}
//		else
//		{
//			m_nSendCardCount= 0;
//		}
//		break;
//	}   
//
//	return true;
//}
bool CTableFrameSink::ChangeMaxUser(bool blMin,int iIndex)
{
	int i = 0;
	BYTE m_cbTableCardArrayTemp[4][2];
	bool  bWinShunMen = 0;
	bool bWinDuiMen = 0;
	bool bWinDaoMen = 0;
	CopyMemory(&m_cbTableCardArrayTemp[0][0],m_cbTableCardArray,sizeof(m_cbTableCardArray));
	bool blBreak = false;
	int iWinCount = 0;
	BYTE WinIndex[4];

	for (i= 0;i<4;i++)
	{
		bWinShunMen=m_GameLogic.CompareCard(m_cbTableCardArray[i],2,m_cbTableCardArray[(i+1)%4],2)==1?true:false;
		bWinDuiMen=m_GameLogic.CompareCard(m_cbTableCardArray[i],2,m_cbTableCardArray[(i+2)%4],2)==1?true:false;
		bWinDaoMen=m_GameLogic.CompareCard(m_cbTableCardArray[i],2,m_cbTableCardArray[(i+3)%4],2)==1?true:false;

		switch (iIndex)
		{
		case 1:
			if(bWinShunMen==bWinDuiMen&&bWinDaoMen==bWinDuiMen &&bWinDuiMen==blMin)
			{
				CopyMemory(m_cbTableCardArray[0],m_cbTableCardArrayTemp[i],sizeof(m_cbTableCardArrayTemp[i]));
				CopyMemory(m_cbTableCardArray[1],m_cbTableCardArrayTemp[(i+1)%4],sizeof(m_cbTableCardArrayTemp[(i+1)%4]));
				CopyMemory(m_cbTableCardArray[2],m_cbTableCardArrayTemp[(i+2)%4],sizeof(m_cbTableCardArrayTemp[(i+2)%4]));
				CopyMemory(m_cbTableCardArray[3],m_cbTableCardArrayTemp[(i+3)%4],sizeof(m_cbTableCardArrayTemp[(i+3)%4]));
			}
			break ;
		case 2:
			if(bWinShunMen ==blMin)
			{
				WinIndex[iWinCount++] = i;
			}
			break;
		case 3:
			if(bWinDuiMen ==blMin)
			{
				WinIndex[iWinCount++] = i;
			}
			break;
		case 4:
			if(bWinDaoMen==blMin)
			{
				WinIndex[iWinCount++] = i;		
			}
			break;
		}
	}

	if(iIndex!=1)
	{
		if(iWinCount<=0)
		{
			iWinCount = 1;
			return false;
		}
		int Index =  rand()%iWinCount;
		int i = WinIndex[Index];
		CopyMemory(m_cbTableCardArray[0],m_cbTableCardArrayTemp[i],sizeof(m_cbTableCardArrayTemp[i]));
		CopyMemory(m_cbTableCardArray[1],m_cbTableCardArrayTemp[(i+1)%4],sizeof(m_cbTableCardArrayTemp[(i+1)%4]));
		CopyMemory(m_cbTableCardArray[2],m_cbTableCardArrayTemp[(i+2)%4],sizeof(m_cbTableCardArrayTemp[(i+2)%4]));
		CopyMemory(m_cbTableCardArray[3],m_cbTableCardArrayTemp[(i+3)%4],sizeof(m_cbTableCardArrayTemp[(i+3)%4]));
	}

	return true;
}

//����ׯ��
bool CTableFrameSink::OnUserApplyBanker(IServerUserItem *pIApplyServerUserItem)
{
	//�Ϸ��ж�
	LONGLONG lUserScore=pIApplyServerUserItem->GetUserScore();
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

	if (m_ApplyUserArray.GetCount()-1 > m_nRobotBankerApplayMaxCount&&pIApplyServerUserItem->IsAndroidUser())
		return true;

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
		if(m_bEnableSysBanker) //ϵͳ��ׯ
		{
			ChangeBanker(false);
		}
		else
		{
			TakeTurns();

			m_dwJettonTime=(DWORD)time(NULL);
			m_pITableFrame->SetGameTimer(IDI_FREE,m_cbFreeTime*1000,1,NULL);

			//if(IsAllUserReady()&&TrySelectBanker())
			//{
   //            OnStartGame();
			//}
			//else
			//{
			//	//֪ͨ����ʱ
			//	for(WORD i=0;i<GAME_PLAYER;i++)
			//	{
			//		IServerUserItem* pUserItem = m_pITableFrame->GetTableUserItem(i);

			//		if(pUserItem==NULL) continue;

			//		if(pUserItem->IsAndroidUser()) continue;

			//		if(pUserItem->GetUserStatus()!=US_READY)
			//		{
			//			CMD_S_FreeTime timeInfo;
			//			timeInfo.cbTimeLeave = m_cbFreeTime;
			//			m_pITableFrame->SendTableData(i,SUB_S_NOTICE_TIME,&timeInfo,sizeof(timeInfo));
			//		}
			//	}

			//	m_dwJettonTime=(DWORD)time(NULL);
			//	m_pITableFrame->SetGameTimer(IDI_FREE,m_cbFreeTime*1000,1,NULL);
			//}
		}
		
	}

	return true;
}

//�Ƿ�׼��
bool CTableFrameSink::IsAllUserReady()
{
	for(WORD i=0;i<GAME_PLAYER;i++)
	{
		IServerUserItem* pUserItem = m_pITableFrame->GetTableUserItem(i);

		if(pUserItem==NULL) continue;

		if(pUserItem->GetUserStatus()!=US_READY) return false;
	}

	return true;
}

//������Ϸ
bool CTableFrameSink::OnStartGame()
{
	m_pITableFrame->KillGameTimer(IDI_FREE);

	//��ʼ��Ϸ
	m_pITableFrame->StartGame();

	//����ʱ��
	m_dwJettonTime=(DWORD)time(NULL);
	m_pITableFrame->SetGameTimer(IDI_PLACE_JETTON,m_cbBetTime*1000,1,0L);

	//����״̬(������Ʒ����)
	m_pITableFrame->SetGameStatus(GS_PLACE_JETTON);

	//���״̬����
	SendGameStatusToCtrl();

	return true;

}

//���»�������ׯ��
bool CTableFrameSink::UpdateAndroidBankCount()
{
	//ͳ����Ϸ����
	int nGameUserCount = 0;
	for (int i=0;i<GAME_PLAYER;i++)
	{
		IServerUserItem* pUserItem = m_pITableFrame->GetTableUserItem(i);
		if(pUserItem!=NULL) nGameUserCount++;
	}

	//������Ҫ���ٻ�������ׯ
	m_nRobotBankerApplayMaxCount = nGameUserCount/3;
	if(m_nRobotBankerApplayMaxCount==0) m_nRobotBankerApplayMaxCount=1;

	return true;
}


//���������˾���
bool CTableFrameSink::UpdateAndroidDraw()
{
  //  for (int i=0;i<GAME_PLAYER;i++)
  //  {
		//IServerUserItem* pUserItem = m_pITableFrame->GetTableUserItem(i);

		//if(pUserItem==NULL) continue;

		//if(pUserItem->IsAndroidUser()==false) continue;

		////����Ƿ�����ׯ�б���
		//for (INT_PTR nUserIdx=0; nUserIdx<m_ApplyUserArray.GetCount(); ++nUserIdx)
		//{
		//	WORD wChairID=m_ApplyUserArray[nUserIdx];

		//	//�ۼӾ������������Ƿ�ֹ�ײ�������˾�������ǿ�������뿪��
		//	if(wChairID==pUserItem->GetChairID())
		//	{
  //              IAndroidUserItem* pIAndroidUserItem = m_pITableFrame->GetAndroidUserItem(pUserItem->GetUserID(),pUserItem->GetBindIndex());
		//		if(pIAndroidUserItem!=NULL)
		//		{
		//			//��ȡ����
		//			tagAndroidService * pAndroidService=pIAndroidUserItem->GetAndroidService();
		//			if(pAndroidService!=NULL) pAndroidService->dwResidualPlayDraw++;
		//		}
		//	}
		//}
  //  }

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

	/*	CMD_S_bCanCancelBanker CanCelBanker;
		CanCelBanker.blCancel = false;
		CanCelBanker.wChariID = m_wCurrentBanker;
		m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_CANCEL_CAN, &CanCelBanker, sizeof(CanCelBanker));
		m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_CANCEL_CAN, &CanCelBanker, sizeof(CanCelBanker));*/
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
			lstrcpyn(CancelBanker.szCancelUser,pIServerUserItem->GetNickName(),CountArray(CancelBanker.szCancelUser));

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
	/*	CMD_S_bCanCancelBanker CanCelBanker;
		CanCelBanker.blCancel = true;
		CanCelBanker.wChariID = m_wCurrentBanker;
		m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_CANCEL_CAN, &CanCelBanker, sizeof(CanCelBanker));
		m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_CANCEL_CAN, &CanCelBanker, sizeof(CanCelBanker));*/
		return true;
	}

	return true;
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
//�����������ׯ
//VOID CTableFrameSink::MakeAndroidUserBanker()
//{
//
//	BYTE  bcAndroidUser[GAME_PLAYER];
//	int   bcOutAppUserArrayCount = 0;
//	int   inInAppUserArrayCount = 0;
//	int i = 0;
//
//	for (WORD wCharidid=0; wCharidid<GAME_PLAYER; ++wCharidid)
//	{
//		IServerUserItem *pServerUserItem = m_pITableFrame->GetTableUserItem(wCharidid);
//		if(!pServerUserItem)
//		{
//			memset(&m_AndroidUser[wCharidid],0,sizeof(TAndroidUser));
//			continue;
//		}
//
//		if(pServerUserItem&&pServerUserItem->IsAndroidUser())
//		{
//			m_AndroidUser[wCharidid].blAndroidUser = true;
//		}
//		else
//		{
//			m_AndroidUser[wCharidid].blAndroidUser = false;
//			memset(&m_AndroidUser[wCharidid],0,sizeof(TAndroidUser));
//			continue;
//
//		}
//
//		if (m_wCurrentBanker != wCharidid )
//		{
//			if(m_AndroidUser[wCharidid].blAndroidUser==true)
//			{
//				m_AndroidUser[wCharidid].PlayCount++;
//				m_AndroidUser[wCharidid].BankCount=0;
//
//			}
//			bool blInArray = false;
//			for (INT nUserIdx=0; nUserIdx<m_ApplyUserArray.GetCount(); ++nUserIdx)
//			{
//				if(wCharidid == m_ApplyUserArray[nUserIdx])
//				{
//					inInAppUserArrayCount++;
//					blInArray = true;
//					break;
//				}
//			}
//
//			int	m_nAndroidUserPlayerCount=GetPrivateProfileInt(m_szGameRoomName, TEXT("AndroidUserPlayerCount"), 2, m_szConfigFileName);
//
//			if(blInArray==false
//				&&m_AndroidUser[wCharidid].PlayCount>m_nAndroidUserPlayerCount
//				&&m_pITableFrame->GetTableUserItem(wCharidid)->GetUserScore()>m_lApplyBankerCondition)
//			{
//				bcAndroidUser[i++] = (BYTE)wCharidid;
//				bcOutAppUserArrayCount++;
//			}
//
//		}
//		else
//		{
//			if(m_AndroidUser[wCharidid].blAndroidUser==true)
//			{
//				m_AndroidUser[wCharidid].PlayCount=0;
//				m_AndroidUser[wCharidid].BankCount++;
//			}
//
//			int	m_nAndroidUserBankerCount=GetPrivateProfileInt(m_szGameRoomName, TEXT("AndroidUserBankerCount"), 1, m_szConfigFileName);
//			if(m_AndroidUser[wCharidid].BankCount>m_nAndroidUserBankerCount)
//			{
//				IServerUserItem *pServerUserItem = m_pITableFrame->GetTableUserItem(wCharidid);
//				if(pServerUserItem)
//					OnUserCancelBanker(pServerUserItem);
//				m_AndroidUser[wCharidid].BankCount = 0;
//
//			}
//
//		}
//
//	}
//	WORD wMinApplyBankerCount=GetPrivateProfileInt(m_szGameRoomName, TEXT("MinApplyBankerCount"), 2, m_szConfigFileName);
//	if(inInAppUserArrayCount<wMinApplyBankerCount&&bcOutAppUserArrayCount>0)
//	{
//		RandList(bcAndroidUser,bcOutAppUserArrayCount);
//		if(bcOutAppUserArrayCount<wMinApplyBankerCount-inInAppUserArrayCount)
//		{ 
//			for (int i = 0;i<bcOutAppUserArrayCount;i++)
//			{
//				IServerUserItem *pServerUserItem = m_pITableFrame->GetTableUserItem(bcAndroidUser[i]);
//				if(pServerUserItem)
//					OnUserApplyBanker(pServerUserItem);
//			}
//
//
//		}else
//		{
//			for (int i = 0;i<wMinApplyBankerCount-inInAppUserArrayCount;i++)
//			{
//				IServerUserItem *pServerUserItem = m_pITableFrame->GetTableUserItem(bcAndroidUser[i]);
//				if(pServerUserItem)
//					OnUserApplyBanker(pServerUserItem);
//			}
//
//
//		}
//
//	}
//
//	//�ɷ��˿�
//	DispatchTableCard();
//
//	bool static bWinShunMen, bWinDuiMen, bWinDaoMen;
//
//	DeduceWinner(bWinShunMen, bWinDuiMen, bWinDaoMen);
//
//	//�ȷ��ͽ����������
//	for (WORD wCharidid=0; wCharidid<GAME_PLAYER; ++wCharidid)
//	{
//		IServerUserItem *pServerUserItem = m_pITableFrame->GetTableUserItem(wCharidid);
//		if(!pServerUserItem)
//		{
//			memset(&m_AndroidUser[wCharidid],0,sizeof(TAndroidUser));
//			continue;
//		}
//		if(pServerUserItem&&pServerUserItem->IsAndroidUser())
//		{
//			m_AndroidUser[wCharidid].blAndroidUser = true;
//		}
//		else
//		{
//			m_AndroidUser[wCharidid].blAndroidUser = false;
//			memset(&m_AndroidUser[wCharidid],0,sizeof(TAndroidUser));
//			continue;
//
//		}
//	}
//
//}
//����ׯ��
bool CTableFrameSink::ChangeBanker(bool bCancelCurrentBanker)
{
	try
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

			//���ñ���
			bChangeBanker = true;
			m_bExchangeBanker = true;
		}
		//��ׯ�ж�
		else if (m_wCurrentBanker!=INVALID_CHAIR)
		{
			//��ȡׯ��
			IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(m_wCurrentBanker);

			if(pIServerUserItem!= NULL)
			{
				LONGLONG lBankerScore=pIServerUserItem->GetUserScore();

				//�����ж�
				if (m_lPlayerBankerMAX<=m_wBankerTime || lBankerScore<m_lApplyBankerCondition)
				{
					//ׯ�������ж� ͬһ��ׯ�������ֻ�ж�һ��
					if(m_lPlayerBankerMAX <= m_wBankerTime && m_bExchangeBanker)
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
							LONGLONG lScore = pIServerUserItem->GetUserScore();

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
					//TakeTurns();

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
                
                //�ı�ׯ��
				bChangeBanker=true;
				m_bExchangeBanker = true;
			}

		}
		//ϵͳ��ׯ
		else if (m_bEnableSysBanker&&m_wCurrentBanker==INVALID_CHAIR && m_ApplyUserArray.GetCount()!=0)
		{
			//�ֻ��ж�
			//TakeTurns();

			bChangeBanker=true;
			m_bExchangeBanker = true;
		}

		//�л��ж�
		if (bChangeBanker)
		{
			//�ֻ��ж�
			TakeTurns();

			//�����ׯ��
			m_lPlayerBankerMAX = m_lBankerMAX;

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
				//��ȡ��Ϣ
				LONGLONG lMessageCount=GetPrivateProfileInt(m_szGameRoomName,TEXT("MessageCount"),0,m_szConfigFileName);
				if (lMessageCount!=0)
				{
					//��ȡ����
					LONGLONG lIndex=rand()%lMessageCount;
					TCHAR szKeyName[32],szMessage1[256],szMessage2[256];				
					_sntprintf(szKeyName,CountArray(szKeyName),TEXT("Item%I64d"),lIndex);
					GetPrivateProfileString(m_szGameRoomName,szKeyName,TEXT("��ϲ[ %s ]��ׯ"),szMessage1,CountArray(szMessage1),m_szConfigFileName);

					//��ȡ���
					IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(m_wCurrentBanker);

					//������Ϣ
					_sntprintf(szMessage2,CountArray(szMessage2),szMessage1,pIServerUserItem->GetNickName());
					SendGameMessage(INVALID_CHAIR,szMessage2);
				}
			}
		}

		return bChangeBanker;
	}
	catch (...)
	{
		return false;
	}
	
	
}

//�ֻ��ж�
VOID CTableFrameSink::TakeTurns()
{
	//��������
	int nInvalidApply = 0;

	for (int i = 0; i < m_ApplyUserArray.GetCount(); i++)
	{
		//��ȡ����
		IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(m_ApplyUserArray[i]);
		if (pIServerUserItem != NULL)
		{
			if (pIServerUserItem->GetUserScore() >= m_lApplyBankerCondition)
			{
				m_wCurrentBanker=m_ApplyUserArray[i];
				//��������֪ͨ��ׯ
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
					//��ȡ��Ϣ
					LONGLONG lMessageCount=GetPrivateProfileInt(m_szGameRoomName,TEXT("MessageCount"),0,m_szConfigFileName);
					if (lMessageCount!=0)
					{
						//��ȡ����
						LONGLONG lIndex=rand()%lMessageCount;
						TCHAR szKeyName[32],szMessage1[256],szMessage2[256];				
						_sntprintf(szKeyName,CountArray(szKeyName),TEXT("Item%I64d"),lIndex);
						GetPrivateProfileString(m_szGameRoomName,szKeyName,TEXT("��ϲ[ %s ]��ׯ"),szMessage1,CountArray(szMessage1),m_szConfigFileName);
						//��ȡ���
						IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(m_wCurrentBanker);
						//������Ϣ
						_sntprintf(szMessage2,CountArray(szMessage2),szMessage1,pIServerUserItem->GetNickName());
						SendGameMessage(INVALID_CHAIR,szMessage2);
					}
				}
				break;
			}
			else
			{
				nInvalidApply = i + 1;


				//������Ϣ
				CMD_S_CancelBanker CancelBanker;
				ZeroMemory(&CancelBanker,sizeof(CancelBanker));

				//���ñ���
				lstrcpyn(CancelBanker.szCancelUser,pIServerUserItem->GetNickName(),CountArray(CancelBanker.szCancelUser));

				//������Ϣ
				m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));
				m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));

				//��ʾ��Ϣ
				TCHAR szTipMsg[128];
				_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("������Ľ����������ׯ����������%I64d��,���Զ���ׯ��"),m_lApplyBankerCondition);
				SendGameMessage(pIServerUserItem->GetChairID(),szTipMsg);
			}
			
		}
		
	}

	//ɾ�����
	if (nInvalidApply != 0)
		m_ApplyUserArray.RemoveAt(0, nInvalidApply);

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

//�û�����
bool  CTableFrameSink::OnActionUserOffLine(WORD wChairID, IServerUserItem * pIServerUserItem) 
{
	//�л�ׯ��
//	if (wChairID==m_wCurrentBanker) ChangeBanker(true);

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
		lstrcpyn(CancelBanker.szCancelUser,pIServerUserItem->GetNickName(),CountArray(CancelBanker.szCancelUser));

		//������Ϣ
		m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));
		m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));

		break;
	}

	return true;
}

//�����ע
LONGLONG CTableFrameSink::GetUserMaxJetton(WORD wChairID)
{
	//��ȡ���
	IServerUserItem *pIMeServerUserItem=m_pITableFrame->GetTableUserItem(wChairID);
	if (NULL==pIMeServerUserItem) return 0L;

	//����ע��
	LONGLONG lNowJetton = 0;
	ASSERT(AREA_COUNT<=CountArray(m_lUserJettonScore));
	for (int nAreaIndex=1; nAreaIndex<=AREA_COUNT; ++nAreaIndex) lNowJetton += m_lUserJettonScore[nAreaIndex][wChairID];

	//ׯ�ҽ��
	LONGLONG lBankerScore=2147483647;
	if (m_wCurrentBanker!=INVALID_CHAIR)
	{
		IServerUserItem *pIUserItemBanker=m_pITableFrame->GetTableUserItem(m_wCurrentBanker);
		if (NULL!=pIUserItemBanker) lBankerScore=pIUserItemBanker->GetUserScore();
	}
	for (int nAreaIndex=1; nAreaIndex<=AREA_COUNT; ++nAreaIndex) lBankerScore-=m_lAllJettonScore[nAreaIndex];

	//��������
	LONGLONG lMeMaxScore = min((pIMeServerUserItem->GetUserScore()-lNowJetton), m_lUserLimitScore);

	//��������
	lMeMaxScore=min(lMeMaxScore,m_lAreaLimitScore);

	//ׯ������
	lMeMaxScore=min(lMeMaxScore,lBankerScore);

	//��������
	ASSERT(lMeMaxScore >= 0);
	lMeMaxScore = max(lMeMaxScore, 0);

	return lMeMaxScore;
}

//����÷�
LONGLONG CTableFrameSink::CalculateScore(bool bBankerLeft)
{
	//��������
	LONGLONG static cbRevenue=m_pGameServiceOption->wRevenueRatio;

	//�ƶ����
	bool static bWinShunMen, bWinDuiMen, bWinDaoMen;
	//DeduceWinner(bWinShunMen, bWinDuiMen, bWinDaoMen);

	//��С�Ƚ�
	int nComShunMen = m_GameLogic.CompareCard(m_cbTableCardArray[BANKER_INDEX],2,m_cbTableCardArray[SHUN_MEN_INDEX],2);
	int nComDuiMen = m_GameLogic.CompareCard(m_cbTableCardArray[BANKER_INDEX],2,m_cbTableCardArray[DUI_MEN_INDEX],2);
	int nComDaoMen = m_GameLogic.CompareCard(m_cbTableCardArray[BANKER_INDEX],2,m_cbTableCardArray[DAO_MEN_INDEX],2);
	bWinShunMen = (nComShunMen==1)?true:false;
	bWinDuiMen = (nComDuiMen==1)?true:false;
	bWinDaoMen = (nComDaoMen==1)?true:false;
	//������Ϣ
	int nStarIdx = 0;
	m_llAllStarCount++;
	for (int i = 0; i < 3; ++i)
	{
		nStarIdx = m_cbStarSelect[i];
		m_llStarAllCount[nStarIdx]++;
		if(i==0&&bWinShunMen)
			m_llStarWinCount[nStarIdx]++;
		else if(i==1&&bWinDuiMen)
			m_llStarWinCount[nStarIdx]++;
		else if(i==2&&bWinDaoMen)
			m_llStarWinCount[nStarIdx]++;
		//m_fStarWinRate[nStarIdx] = (float)((float)m_llStarWinCount[nStarIdx]/(float)m_llStarAllCount[nStarIdx]);
	}
	for (int i = 0; i < STAR_COUNT; ++i)
	{
		m_fStarWinRate[i] = (float)((float)m_llStarWinCount[i]/(float)m_llAllStarCount);
	}
	WriteStarInfo();

	LONGLONG ortlUserWinScore[GAME_PLAYER];

	ZeroMemory(ortlUserWinScore, sizeof(ortlUserWinScore));

	//����
	BYTE bAreaPoint[7] = {0};
	memset(bAreaPoint, 0, sizeof(bAreaPoint));
	BYTE bBankerPoint = m_GameLogic.GetCardListPip(m_cbTableCardArray[BANKER_INDEX],2);
	bAreaPoint[ID_SHUN_MEN] = m_GameLogic.GetCardListPip(m_cbTableCardArray[SHUN_MEN_INDEX],2);
	bAreaPoint[ID_DUI_MEN] = m_GameLogic.GetCardListPip(m_cbTableCardArray[DUI_MEN_INDEX],2);
	bAreaPoint[ID_DAO_MEN] = m_GameLogic.GetCardListPip(m_cbTableCardArray[DAO_MEN_INDEX],2);

	//ׯ�ҽ��
	LONGLONG lBankerScore = 0;
	if(m_wCurrentBanker != INVALID_CHAIR)
	{
		IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(m_wCurrentBanker);
		if(pIServerUserItem)
			lBankerScore = pIServerUserItem->GetUserScore();
	}

	//�����м�Ѻע
	LONGLONG lUserScore[GAME_PLAYER] = {0};
	LONGLONG lAllUserScore = 0L;
	for (WORD wChairID=0; wChairID<GAME_PLAYER; wChairID++)
	{
		//ׯ���ж�
		if (m_wCurrentBanker==wChairID) continue;

		//�����û��·�
		lUserScore[wChairID] = 0L;
		for ( WORD wAreaIndex = ID_SHUN_MEN; wAreaIndex <= ID_JIAO_R; ++wAreaIndex ) lUserScore[wChairID] += m_lUserJettonScore[wAreaIndex][wChairID];

		lAllUserScore += lUserScore[wChairID];
	}

	//�Ƿ�ﵽ����
	bool bToBankerMax = false;
	if(lBankerScore > 0 && lAllUserScore >= (lBankerScore*50/100)) bToBankerMax = true;

	//��Ϸ��¼
	tagServerGameRecord &GameRecord = m_GameRecordArrary[m_nRecordLast];
	//˳��
	if(bAreaPoint[ID_SHUN_MEN]==bBankerPoint&&bToBankerMax) GameRecord.bWinShunMen = WIN_FLAG_EQUAL;
	else GameRecord.bWinShunMen = bWinShunMen?WIN_FLAG_WIN:WIN_FLAG_LOST;
	//����
	if(bAreaPoint[ID_DUI_MEN] == bBankerPoint&&bToBankerMax) GameRecord.bWinDuiMen = WIN_FLAG_EQUAL;
	else GameRecord.bWinDuiMen = bWinDuiMen?WIN_FLAG_WIN:WIN_FLAG_LOST;
    //����
	if(bAreaPoint[ID_DAO_MEN] == bBankerPoint&&bToBankerMax) GameRecord.bWinDaoMen = WIN_FLAG_EQUAL;
	else GameRecord.bWinDaoMen = bWinDaoMen?WIN_FLAG_WIN:WIN_FLAG_LOST;

	//ׯ������
	LONGLONG lBankerWinScore = 0;

	//��ҳɼ�
	ZeroMemory(m_lUserWinScore, sizeof(m_lUserWinScore));
	ZeroMemory(m_lUserReturnScore, sizeof(m_lUserReturnScore));
	ZeroMemory(m_lUserRevenue, sizeof(m_lUserRevenue));
	LONGLONG lUserLostScore[GAME_PLAYER];
	ZeroMemory(lUserLostScore, sizeof(lUserLostScore));


	//ʤ����ʶ
	bool static bWinFlag[AREA_ARRY_COUNT];
	bWinFlag[0]=false;
	bWinFlag[ID_SHUN_MEN]=bWinShunMen;
	bWinFlag[ID_DUI_MEN]=bWinDuiMen;
	bWinFlag[ID_DAO_MEN]=bWinDaoMen;
	bWinFlag[ID_JIAO_L]= false;
	bWinFlag[ID_QIAO]= false;
	bWinFlag[ID_JIAO_R]= false;

	//�Ǳ�ʶ
	bool static bWinBankerJiaoL,bWinBankerJiaoR,bWinBankerQiao;
	bWinBankerJiaoL= false;
	bWinBankerJiaoR= false;
	bWinBankerQiao= false;


	//�������
	for (WORD wChairID=0; wChairID<GAME_PLAYER; wChairID++)
	{
		//ׯ���ж�
		if (m_wCurrentBanker==wChairID) continue;

		//��ȡ�û�
		IServerUserItem * pIServerUserItem=m_pITableFrame->GetTableUserItem(wChairID);
		if (pIServerUserItem == NULL  && lUserScore[wChairID] == 0) continue;
		bool bUserLeft = (pIServerUserItem==NULL);

		for (WORD wAreaIndex = ID_SHUN_MEN; wAreaIndex <= ID_JIAO_R; ++wAreaIndex)
		{
			if (!bUserLeft)
			{
				//���ж�
				bool bReturnScore=false;
				if (ID_JIAO_L==wAreaIndex && false==bWinBankerJiaoL) bReturnScore=true;
				if (ID_JIAO_R==wAreaIndex && false==bWinBankerJiaoR) bReturnScore=true;
				if (ID_QIAO==wAreaIndex && false==bWinBankerQiao) bReturnScore=true;
				if (bToBankerMax && bAreaPoint[wAreaIndex] == bBankerPoint) bReturnScore=true;

				if (true==bWinFlag[wAreaIndex]) 
				{
					int iType = m_GameLogic.GetCardType(m_cbTableCardArray[wAreaIndex],2);
					int iMultiple = 1;
					if(CT_SPECIAL == iType && bToBankerMax == false)
					{
						iMultiple = 2;
					}
					m_lUserWinScore[wChairID] += ( m_lUserJettonScore[wAreaIndex][wChairID] * iMultiple) ;
					m_lUserReturnScore[wChairID] += m_lUserJettonScore[wAreaIndex][wChairID] ;
					lBankerWinScore -= ( m_lUserJettonScore[wAreaIndex][wChairID] *iMultiple) ;
				}
				else if (true==bReturnScore)
				{
					m_lUserReturnScore[wChairID] += m_lUserJettonScore[wAreaIndex][wChairID] ;
				}
				else
				{
					int iType = m_GameLogic.GetCardType(m_cbTableCardArray[BANKER_INDEX],2);
					int iMultiple = 1;
					if(CT_SPECIAL== iType)
					{
						iMultiple = 1;
					}

					lUserLostScore[wChairID] -= m_lUserJettonScore[wAreaIndex][wChairID]*iMultiple;
					lBankerWinScore += m_lUserJettonScore[wAreaIndex][wChairID];
				}
			}
			//�������
			else if(!bBankerLeft)
			{
				lUserLostScore[wChairID] -= m_lUserJettonScore[wAreaIndex][wChairID];
				lBankerWinScore += m_lUserJettonScore[wAreaIndex][wChairID];
			}
		}
		//����˰��
		if (0 < m_lUserWinScore[wChairID])
		{
			double fRevenuePer=double(cbRevenue/1000.);
			m_lUserRevenue[wChairID]  = LONGLONG(m_lUserWinScore[wChairID]*fRevenuePer);
			m_lUserWinScore[wChairID] -= m_lUserRevenue[wChairID];
		}
		//�ܵķ���
		m_lUserWinScore[wChairID] += lUserLostScore[wChairID];
	}

	//ׯ�ҳɼ�
	if (m_wCurrentBanker!=INVALID_CHAIR)
	{
		m_lUserWinScore[m_wCurrentBanker] = lBankerWinScore;

		//����˰��
		if (0 < m_lUserWinScore[m_wCurrentBanker])
		{
			double fRevenuePer=double(cbRevenue/1000.);
			if(cbRevenue<=0)
			{
				fRevenuePer = 0;
			}
			m_lUserRevenue[m_wCurrentBanker]  = LONGLONG(m_lUserWinScore[m_wCurrentBanker]*fRevenuePer);
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

VOID CTableFrameSink::DeduceWinner(bool &bWinShunMen, bool &bWinDuiMen, bool &bWinDaoMen)
{
	//��С�Ƚ�
	bWinShunMen=m_GameLogic.CompareCard(m_cbTableCardArray[BANKER_INDEX],2,m_cbTableCardArray[SHUN_MEN_INDEX],2)==1?true:false;
	bWinDuiMen=m_GameLogic.CompareCard(m_cbTableCardArray[BANKER_INDEX],2,m_cbTableCardArray[DUI_MEN_INDEX],2)==1?true:false;
	bWinDaoMen=m_GameLogic.CompareCard(m_cbTableCardArray[BANKER_INDEX],2,m_cbTableCardArray[DAO_MEN_INDEX],2)==1?true:false;
}

//��̽���ж�
bool CTableFrameSink::ProbeJudge(LONGLONG& lSystemScore)
{
	//�ƶ����
	bool bWinShunMen, bWinDuiMen, bWinDaoMen;
	DeduceWinner(bWinShunMen, bWinDuiMen, bWinDaoMen);


	//ʤ����ʶ
	bool bWinFlag[AREA_ARRY_COUNT];
	bWinFlag[0]=false;
	bWinFlag[ID_SHUN_MEN]=bWinShunMen;
	bWinFlag[ID_DUI_MEN]=bWinDuiMen;
	bWinFlag[ID_DAO_MEN]=bWinDaoMen;
	bWinFlag[ID_JIAO_L]= false;
	bWinFlag[ID_QIAO]= false;
	bWinFlag[ID_JIAO_R]= false;

	//��ش���
	if(m_cbControlID!=INIT_CTRL&&m_cbControlID>=0&&m_cbControlID<=7)
	{
		bool isReturn = true;

		//����Ӯ
		if(bWinFlag[ID_SHUN_MEN]&&!bWinFlag[ID_DUI_MEN]&&!bWinFlag[ID_DAO_MEN]&&m_cbControlID==0) isReturn=false;
		//����Ӯ
		if(bWinFlag[ID_DUI_MEN]&&!bWinFlag[ID_SHUN_MEN]&&!bWinFlag[ID_DAO_MEN]&&m_cbControlID==1) isReturn=false;
		//����Ӯ
		if(bWinFlag[ID_DAO_MEN]&&!bWinFlag[ID_SHUN_MEN]&&!bWinFlag[ID_DUI_MEN]&&m_cbControlID==2) isReturn=false;
		//ͨɱ
		if(!bWinFlag[ID_SHUN_MEN]&&!bWinFlag[ID_DUI_MEN]&&!bWinFlag[ID_DAO_MEN]&&m_cbControlID==3) isReturn=false;
		//ͨ��
		if(bWinFlag[ID_SHUN_MEN]&&bWinFlag[ID_DUI_MEN]&&bWinFlag[ID_DAO_MEN]&&m_cbControlID==4) isReturn=false;
		//������
		if(!bWinFlag[ID_SHUN_MEN]&&bWinFlag[ID_DUI_MEN]&&bWinFlag[ID_DAO_MEN]&&m_cbControlID==5) isReturn=false;
		//������
		if(!bWinFlag[ID_DUI_MEN]&&bWinFlag[ID_SHUN_MEN]&&bWinFlag[ID_DAO_MEN]&&m_cbControlID==6) isReturn=false;
		//������
		if(!bWinFlag[ID_DAO_MEN]&&bWinFlag[ID_SHUN_MEN]&&bWinFlag[ID_DUI_MEN]&&m_cbControlID==7) isReturn=false;

		if(isReturn) return false;

		return true;
	}

	//ϵͳ��Ӯ
	lSystemScore = 0l;

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

	//����
	BYTE bAreaPoint[7] = {0};
	memset(bAreaPoint, 0, sizeof(bAreaPoint));
	BYTE bBankerPoint = m_GameLogic.GetCardListPip(m_cbTableCardArray[BANKER_INDEX],2);
	bAreaPoint[ID_SHUN_MEN] = m_GameLogic.GetCardListPip(m_cbTableCardArray[SHUN_MEN_INDEX],2);
	bAreaPoint[ID_DUI_MEN] = m_GameLogic.GetCardListPip(m_cbTableCardArray[DUI_MEN_INDEX],2);
	bAreaPoint[ID_DAO_MEN] = m_GameLogic.GetCardListPip(m_cbTableCardArray[DAO_MEN_INDEX],2);

	//ׯ�ҽ��
	LONGLONG lBankerScore = 0;
	if(m_wCurrentBanker != INVALID_CHAIR)
	{
		IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(m_wCurrentBanker);
		if(pIServerUserItem)
			lBankerScore = pIServerUserItem->GetUserScore();
	}

	//�����м�Ѻע
	LONGLONG lUserScore[GAME_PLAYER] = {0};
	LONGLONG lAllUserScore = 0L;
	for (WORD wChairID=0; wChairID<GAME_PLAYER; wChairID++)
	{
		//ׯ���ж�
		if (m_wCurrentBanker==wChairID) continue;

		//�����û��·�
		lUserScore[wChairID] = 0L;
		for ( WORD wAreaIndex = ID_SHUN_MEN; wAreaIndex <= ID_JIAO_R; ++wAreaIndex ) lUserScore[wChairID] += m_lUserJettonScore[wAreaIndex][wChairID];

		lAllUserScore += lUserScore[wChairID];
	}

	//�Ƿ�ﵽ����
	bool bToBankerMax = false;
	if(lBankerScore > 0 && lAllUserScore >= (lBankerScore*50/100)) bToBankerMax = true;

	//�������
	LONGLONG lBankerWinScore = 0L;
	for (WORD wChairID=0; wChairID<GAME_PLAYER; wChairID++)
	{
		//ׯ���ж�
		if (m_wCurrentBanker==wChairID) continue;

		//��ȡ�û�
		IServerUserItem * pIServerUserItem=m_pITableFrame->GetTableUserItem(wChairID);
		if (pIServerUserItem == NULL) continue;

		//
		bool bIsAndroidUser = pIServerUserItem->IsAndroidUser();

		for (WORD wAreaIndex = ID_SHUN_MEN; wAreaIndex <= ID_JIAO_R; ++wAreaIndex)
		{
			//���ж�
			bool bReturnScore=false;
			if (bToBankerMax && bAreaPoint[wAreaIndex] == bBankerPoint) bReturnScore=true;

			if (true==bWinFlag[wAreaIndex]) 
			{
				int iType = m_GameLogic.GetCardType(m_cbTableCardArray[wAreaIndex],2);
				int iMultiple = 1;
				if(CT_SPECIAL == iType && bToBankerMax == false)
				{
					iMultiple = 2;
				}
				if(bIsAndroidUser)
					lSystemScore += ( m_lUserJettonScore[wAreaIndex][wChairID] * iMultiple);

				if(m_wCurrentBanker==INVALID_CHAIR || bIsBankerAndroidUser)
					lSystemScore -= ( m_lUserJettonScore[wAreaIndex][wChairID] * iMultiple);

				if(m_wCurrentBanker!=INVALID_CHAIR)
					lBankerWinScore -= ( m_lUserJettonScore[wAreaIndex][wChairID] *iMultiple) ;
			}
			else if (true==bReturnScore)
			{
			}
			else
			{
				if(bIsAndroidUser)
					lSystemScore -= m_lUserJettonScore[wAreaIndex][wChairID];

				if(m_wCurrentBanker==INVALID_CHAIR || bIsBankerAndroidUser)
					lSystemScore += m_lUserJettonScore[wAreaIndex][wChairID];

				if(m_wCurrentBanker!=INVALID_CHAIR)
					lBankerWinScore += m_lUserJettonScore[wAreaIndex][wChairID];
			}
		}
	}

	//ׯ�ҿ۷ּ���
	if(lBankerWinScore < 0L && (lBankerScore+lBankerWinScore) < 0L) return false;

	//ϵͳӮ
	if(lSystemScore>=0) return true;

	//ϵͳ��ֵ����
	LONGLONG lStorageStart = m_pITableFrame->GetRoomStorageScore();
	if ((lSystemScore + lStorageStart) < 0l)
	{
		return false;
	}
	else
	{
		//m_StorageStart += lSystemScore;
		//m_StorageStart = m_StorageStart - (m_StorageStart * m_StorageDeduct / 1000);
		return true;
	}

	return true;
}

//���ͼ�¼
VOID CTableFrameSink::SendGameRecord(IServerUserItem *pIServerUserItem)
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

//�����ж�
bool  CTableFrameSink::IsAllowBankAction(WORD wChairID, IServerUserItem * pIServerUserItem)
{
	if(wChairID==m_wCurrentBanker)
	{
		return false;
	}else
	{
		return true;
	}
}

//��ׯ���� (������)
VOID CTableFrameSink::RobotBankerControl()
{
	//�Ƿ������ׯ������
	if (m_ApplyUserArray.GetCount()-1 > m_nRobotBankerApplayMaxCount)
		return;

	for (int i = 0; i < GAME_PLAYER; i++)
	{
		IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(i);

		if (pIServerUserItem != NULL && pIServerUserItem->IsAndroidUser())
		{

			if ( m_wCurrentBanker != i && pIServerUserItem->GetUserScore() >= m_lApplyBankerCondition)
			{
				//�����ׯ�б�
				bool bOnApplayArray = false;
				for (int j = 0; j < m_ApplyUserArray.GetCount(); j++)
				{
					if (i == m_ApplyUserArray[j]) 
					{
						bOnApplayArray = true;
						break;
					}
				}
				if (!bOnApplayArray)
				{
					m_bAndroidApplyBanker[i]=true;
				}
			}
		}
	}

	return;
}

//ѡ��ׯ��
bool CTableFrameSink::TrySelectBanker()
{
	if(m_wCurrentBanker==INVALID_CHAIR&&m_ApplyUserArray.GetCount()==0) 
		return false;

	//������������2��
	int nUserCount = 0;
	for (int i=0;i<GAME_PLAYER;i++)
	{
		if(m_pITableFrame->GetTableUserItem(i)!=NULL) nUserCount++;
	}
	if(nUserCount==0) return false;

	if (m_wCurrentBanker==INVALID_CHAIR&&m_ApplyUserArray.GetCount()>0)
	{
		m_wCurrentBanker = m_ApplyUserArray[0];

		//�л��ж�
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
				//��ȡ��Ϣ
				LONGLONG lMessageCount=GetPrivateProfileInt(m_szGameRoomName,TEXT("MessageCount"),0,m_szConfigFileName);
				if (lMessageCount!=0)
				{
					//��ȡ����
					LONGLONG lIndex=rand()%lMessageCount;
					TCHAR szKeyName[32],szMessage1[256],szMessage2[256];				
					_sntprintf(szKeyName,CountArray(szKeyName),TEXT("Item%I64d"),lIndex);
					GetPrivateProfileString(m_szGameRoomName,szKeyName,TEXT("��ϲ[ %s ]��ׯ"),szMessage1,CountArray(szMessage1),m_szConfigFileName);

					//��ȡ���
					IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(m_wCurrentBanker);

					//������Ϣ
					_sntprintf(szMessage2,CountArray(szMessage2),szMessage1,pIServerUserItem->GetNickName());
					SendGameMessage(INVALID_CHAIR,szMessage2);
				}
			}
		}
	}

	return true;
}

//////////////////////////////////////////����¼�///////////////////////////////
bool CTableFrameSink::OnUserControlEvent(IServerUserItem* pServerUserItem,CMD_GC_ControlInfo* pCtrlData)
{
	if(pServerUserItem == NULL) return false;

	//ִ��ʱ����
	if(m_pITableFrame->GetGameStatus()!=GS_PLACE_JETTON)
	{
		CMD_GC_CtrlNotice NoticeInfo;
		NoticeInfo.cbNoticeID = 0;
		NoticeInfo.wTableID = m_pITableFrame->GetTableID();

		m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_GC_CTRL_NOTICE,&NoticeInfo,sizeof(CMD_GC_CtrlNotice),MDM_GC_CONTROL);

		return true;
	}

	//ִ��ʱ����
	if(m_pITableFrame->GetRoomStorageScore()<-2000000000)
	{
		CMD_GC_CtrlNotice NoticeInfo;
		NoticeInfo.cbNoticeID = 1;
		NoticeInfo.wTableID = m_pITableFrame->GetTableID();

		m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_GC_CTRL_NOTICE,&NoticeInfo,sizeof(CMD_GC_CtrlNotice),MDM_GC_CONTROL);

		return true;
	}

	m_cbControlID = pCtrlData->cbControlId;

	return true;

}

VOID CTableFrameSink::SendGameStatusToCtrl()
{
	CMD_GC_StateInfo info;
	ZeroMemory(&info,sizeof(info));

	if(m_pITableFrame->GetGameStatus()==GS_PLACE_JETTON)
	{
		info.cbStateTime = m_cbBetTime;
		info.cbStateFlag = 1;
	}
	else if(m_pITableFrame->GetGameStatus()==GS_GAME_END)
	{
		info.cbStateTime = m_cbEndTime;
		info.cbStateFlag = 2;
	}
	else 
	{
		info.cbStateTime = m_cbFreeTime;
		info.cbStateFlag = 0;
	}

	//ׯ��
	if(m_wCurrentBanker!=INVALID_CHAIR)
	{
		IServerUserItem* pServerUserItem = m_pITableFrame->GetTableUserItem(m_wCurrentBanker);
		if(pServerUserItem!=NULL) info.dwBankUserID = pServerUserItem->GetUserID();
		else info.dwBankUserID = 0;
	}
	else
	{
		info.dwBankUserID = 0;
	}


	info.cbTableId = m_pITableFrame->GetTableID();
	info.lRoomStorage = m_pITableFrame->GetRoomStorageScore();

	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_GC_STATEINFO,&info,sizeof(info),MDM_GC_CONTROL);

	return;

}

VOID CTableFrameSink::SendUserChipInfoToCtrl(WORD wChairID, BYTE cbBetArea, LONGLONG lBetScore)
{
	IServerUserItem* pCtrlUserItem = m_pITableFrame->GetTableUserItem(wChairID);
	if(pCtrlUserItem->IsAndroidUser()) return;

	//����Ѻע��Ϣ
	CMD_GC_UserChipData chipInfo;
	ZeroMemory(&chipInfo,sizeof(chipInfo));
	if(cbBetArea==ID_SHUN_MEN)
		chipInfo.cbChipPos = 0;
	else if(cbBetArea==ID_DUI_MEN)
		chipInfo.cbChipPos = 1;
	else if(cbBetArea==ID_DAO_MEN)
		chipInfo.cbChipPos = 2;

	chipInfo.dwUserID = pCtrlUserItem->GetUserID();
	chipInfo.llChipRich = lBetScore;

	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_GC_USERCHIP,&chipInfo,sizeof(chipInfo),MDM_GC_CONTROL);
}