#include "StdAfx.h"
#include "TableFrameSink.h"

//////////////////////////////////////////////////////////////////////////

//常量定义
#define SEND_COUNT					600									//发送次数

//索引定义
#define INDEX_PLAYER				0									//闲家索引
#define INDEX_BANKER				1									//庄家索引

//下注时间
#define IDI_FREE					1									//空闲时间
#define TIME_FREE					10									//空闲时间

//下注时间
#define IDI_PLACE_JETTON			2									//下注时间n
#define TIME_PLACE_JETTON			18									//下注时间


//结束时间
#define IDI_GAME_END				3									//结束时间
#define TIME_GAME_END				25									//结束时间

//切换时间
#define IDI_CHANGE_BANK             4                                   //切换时间


//////////////////////////////////////////////////////////////////////////

//静态变量
const WORD			CTableFrameSink::m_wPlayerCount=GAME_PLAYER;				//游戏人数

//////////////////////////////////////////////////////////////////////////

//构造函数
CTableFrameSink::CTableFrameSink()
{
	//总下注数
	ZeroMemory(m_lAllJettonScore,sizeof(m_lAllJettonScore));
	
	//个人下注
	ZeroMemory(m_lUserJettonScore,sizeof(m_lUserJettonScore));

	//控制变量
	m_cbWinSideControl=0;
	m_nSendCardCount=0;
	m_bBankerLeft=false;

	//玩家成绩	
	ZeroMemory(m_lUserWinScore,sizeof(m_lUserWinScore));
	ZeroMemory(m_lUserReturnScore,sizeof(m_lUserReturnScore));
	ZeroMemory(m_lUserRevenue,sizeof(m_lUserRevenue));

	//扑克信息
	ZeroMemory(m_cbCardCount,sizeof(m_cbCardCount));
	ZeroMemory(m_cbTableCardArray,sizeof(m_cbTableCardArray));

	//状态变量
	m_dwJettonTime=0L;
	m_nRobotBankerApplayMaxCount=0;

	//庄家信息
	m_ApplyUserArray.RemoveAll();
	m_wCurrentBanker=INVALID_CHAIR;
	m_wBankerTime=0;
	m_lBankerWinScore=0L;		
	m_lBankerCurGameScore=0L;
	m_bEnableSysBanker=true;
	m_cbLeftCardCount=0;
	m_bContiueCard=false;
	m_pGameServiceOption = NULL;

	//记录变量
	ZeroMemory(m_GameRecordArrary,sizeof(m_GameRecordArrary));
	m_nRecordFirst=0;
	m_nRecordLast=0;
	m_dwRecordCount=0;

	//庄家设置
	m_lBankerMAX = 0l;
	m_lBankerAdd = 0l;							
	m_lBankerScoreMAX = 0l;
	m_lBankerScoreAdd = 0l;
	m_lPlayerBankerMAX = 0l;
	m_bExchangeBanker = true;
	m_bBankerHalf = false;
	m_bBankerBomb = false;

	//明星概率
	memset(m_cbStarSelect,-1,sizeof(m_cbStarSelect));
	for (int i = 0; i < STAR_COUNT; ++i)
	{
		m_fStarWinRate[i] = 0.0;
		m_llStarAllCount[i] = 0;
		m_llStarWinCount[i] = 0;
	}	
	m_llAllStarCount = 0;
	m_nRedBack = 1;								//背景图片

	//时间控制
	m_cbFreeTime = TIME_FREE;
	m_cbBetTime = TIME_PLACE_JETTON;
	m_cbEndTime = TIME_GAME_END;

	//机器人控制
	m_nChipRobotCount = 0;

	//管理员控制
	memset(&m_AdminCommand, 0, sizeof(m_AdminCommand));

	//服务控制
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

	//监控标记
	m_cbControlID = INIT_CTRL;

	return;
}

//析构函数
CTableFrameSink::~CTableFrameSink(VOID)
{
}

//接口查询
VOID *  CTableFrameSink::QueryInterface(const IID & Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(ITableFrameSink,Guid,dwQueryVer);
	QUERYINTERFACE(ITableUserAction,Guid,dwQueryVer);
	return NULL;
}

//初始化
bool  CTableFrameSink::Initialization(IUnknownEx * pIUnknownEx)
{
	//查询接口
	ASSERT(pIUnknownEx!=NULL);

    //框架接口
	m_pITableFrame=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,ITableFrame);
	if (m_pITableFrame==NULL) return false;

	//机器人管理接口
	


	////控制接口
	//m_pITableFrameControl=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,ITableFrameControl);
	//if (m_pITableFrameControl==NULL) return false;

	//开始模式
	m_pITableFrame->SetStartMode(START_MODE_TIME_CONTROL);

	//获取参数
	m_pGameServiceOption=m_pITableFrame->GetGameServiceOption();
	ASSERT(m_pGameServiceOption!=NULL);

	//读取配置
	memcpy(m_szGameRoomName, m_pGameServiceOption->szServerName, sizeof(m_szGameRoomName));

	//设置文件名
	TCHAR szPath[MAX_PATH]=TEXT("");
	GetCurrentDirectory(sizeof(szPath),szPath);
	myprintf(m_szConfigFileName,sizeof(m_szConfigFileName),TEXT("%s\\%s"),szPath,CONFIG_FILE_NAME);

	//控制变量
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

	//时间控制
	m_cbFreeTime = GetPrivateProfileInt(m_szGameRoomName, TEXT("FreeTime"), TIME_FREE, m_szConfigFileName);
	m_cbBetTime = GetPrivateProfileInt(m_szGameRoomName, TEXT("BetTime"), TIME_PLACE_JETTON, m_szConfigFileName);
	m_cbEndTime = GetPrivateProfileInt(m_szGameRoomName, TEXT("EndTime"), TIME_GAME_END, m_szConfigFileName);

	//最少时间限制
	if(m_cbFreeTime < 10) m_cbFreeTime = 10;
	if(m_cbBetTime < 20) m_cbBetTime = 20;
	if(m_cbEndTime < 15) m_cbEndTime = 15;

	//下注机器人数目
	m_nMaxChipRobot = GetPrivateProfileInt(m_szGameRoomName, TEXT("RobotBetCount"), 10, m_szConfigFileName);
	if (m_nMaxChipRobot < 0)	m_nMaxChipRobot = 10;
	//获取明星输赢信息
	GetStarInfo();

		

	return true;
}

//获取明星输赢信息
bool CTableFrameSink::GetStarInfo()
{
	TCHAR OutBuf[255];
	TCHAR szConfigFileName[MAX_PATH];			//配置文件
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
//写入明星输赢信息
bool CTableFrameSink::WriteStarInfo()
{
	TCHAR OutBuf[255];
	TCHAR szConfigFileName[MAX_PATH];			//配置文件
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

//复位桌子
VOID  CTableFrameSink::RepositionSink()
{
	//总下注数
	ZeroMemory(m_lAllJettonScore,sizeof(m_lAllJettonScore));
	
	//个人下注
	ZeroMemory(m_lUserJettonScore,sizeof(m_lUserJettonScore));

	//玩家成绩	
	ZeroMemory(m_lUserWinScore,sizeof(m_lUserWinScore));
	ZeroMemory(m_lUserReturnScore,sizeof(m_lUserReturnScore));
	ZeroMemory(m_lUserRevenue,sizeof(m_lUserRevenue));

	m_bBankerLeft=false;
	//明星概率
	memset(m_cbStarSelect,-1,sizeof(m_cbStarSelect));

	//机器人控制
	m_nChipRobotCount = 0;

	//监控标记
	m_cbControlID = INIT_CTRL;


	return;
}

//游戏状态
bool  CTableFrameSink::IsUserPlaying(WORD wChairID)
{
	return true;
}

//查询限额
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

//查询是否扣服务费
bool CTableFrameSink::QueryBuckleServiceCharge(WORD wChairID)
{
	for (BYTE i=0;i<GAME_PLAYER;i++)
	{
		IServerUserItem *pUserItem=m_pITableFrame->GetTableUserItem(i);
		if(pUserItem==NULL) continue;

		if(wChairID==m_wCurrentBanker) return true;

		if(wChairID==i)
		{
			//返回下注
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

//游戏开始
bool  CTableFrameSink::OnEventGameStart()
{
	//获取明星输赢信息
	GetStarInfo();
	//下注开始，选择美女
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
	//变量定义
	CMD_S_GameStart GameStart;
	ZeroMemory(&GameStart,sizeof(GameStart));
	//明星信息
	memcpy(GameStart.cbStarSelect,m_cbStarSelect,sizeof(m_cbStarSelect));
	memcpy(GameStart.fStarWinRate,m_fStarWinRate,sizeof(m_fStarWinRate));
	memcpy(GameStart.llStarAllCount,m_llStarAllCount,sizeof(m_llStarAllCount));
	memcpy(GameStart.llStarWinCount,m_llStarWinCount,sizeof(m_llStarWinCount));
	GameStart.llAllStarCount = m_llAllStarCount;
	GameStart.nRedBack = m_nRedBack;

	//获取庄家
	IServerUserItem *pIBankerServerUserItem=NULL;
	if (INVALID_CHAIR!=m_wCurrentBanker) pIBankerServerUserItem=m_pITableFrame->GetTableUserItem(m_wCurrentBanker);

	//设置变量
	GameStart.cbTimeLeave=m_cbBetTime;
	GameStart.wBankerUser=m_wCurrentBanker;
	if (pIBankerServerUserItem!=NULL) GameStart.lBankerScore=pIBankerServerUserItem->GetUserScore();
	GameStart.bContiueCard=m_bContiueCard;

	GameStart.lUserLimitScore = m_lUserLimitScore;
	//下注机器人数量
	int nChipRobotCount = 0;
	for (int i = 0; i < GAME_PLAYER; i++)
	{
		IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(i);
		if (pIServerUserItem != NULL && pIServerUserItem->IsAndroidUser())
			nChipRobotCount++;
	}

	GameStart.nChipRobotCount = min(nChipRobotCount, m_nMaxChipRobot);

	//机器人控制
	m_nChipRobotCount = 0;
	
    //旁观玩家
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_START,&GameStart,sizeof(GameStart));	

	//游戏玩家
	for (WORD wChairID=0; wChairID<GAME_PLAYER; ++wChairID)
	{
		IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(wChairID);
		if (pIServerUserItem==NULL) continue;

		//设置积分
		GameStart.lUserMaxScore=min(pIServerUserItem->GetUserScore(),m_lUserLimitScore);

		m_pITableFrame->SendTableData(wChairID,SUB_S_GAME_START,&GameStart,sizeof(GameStart));	
	}

	return true;
}

//游戏结束
bool  CTableFrameSink::OnEventGameConclude(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)
{
	switch (cbReason)
	{
	case GER_NORMAL:		//常规结束	
		{
			//游戏作弊
			bool bSuccess = false;
			INT nMaxCount = 0;
			LONGLONG lMaxSystemScore = LLONG_MIN;
			BYTE cbTableCardArray[4][2];
			ZeroMemory(cbTableCardArray,sizeof(BYTE)*4*2);

			do 
			{
				srand((unsigned)time(NULL)+rand()%1000);

				//派发扑克
				DispatchTableCard();


				LONGLONG llSystemScore = 0;

				//试探性判断
				bSuccess = ProbeJudge(llSystemScore);

				//最少赔记录
				if(llSystemScore>lMaxSystemScore)
				{
					lMaxSystemScore = llSystemScore;
					CopyMemory(cbTableCardArray,m_cbTableCardArray,sizeof(m_cbTableCardArray));
				}

				//以系统最大赚理赔
				if(nMaxCount>1000&&lMaxSystemScore!=LLONG_MIN)
				{
					CopyMemory(m_cbTableCardArray,cbTableCardArray,sizeof(m_cbTableCardArray));
					break;
				}

				nMaxCount++;

			} while (!bSuccess);


			//计算分数
			LONGLONG lBankerWinScore=CalculateScore(m_bBankerLeft);

			//递增次数
			m_wBankerTime++;

			//结束消息
			CMD_S_GameEnd GameEnd;
			ZeroMemory(&GameEnd,sizeof(GameEnd));

			//庄家信息
			GameEnd.nBankerTime = m_wBankerTime;
			GameEnd.lBankerTotallScore=m_lBankerWinScore;
			GameEnd.lBankerScore=lBankerWinScore;
			GameEnd.bcFirstCard = m_bcFirstPostCard;
			
			//扑克信息
			CopyMemory(GameEnd.cbTableCardArray,m_cbTableCardArray,sizeof(m_cbTableCardArray));
			GameEnd.cbLeftCardCount=m_cbLeftCardCount;
			GameEnd.bBankerBomb = m_bBankerBomb;

			//发送积分
			GameEnd.cbTimeLeave=m_cbEndTime;	
			for ( WORD wUserIndex = 0; wUserIndex < GAME_PLAYER; ++wUserIndex )
			{
				IServerUserItem *pIServerUserItem = m_pITableFrame->GetTableUserItem(wUserIndex);
				if ( pIServerUserItem == NULL ) continue;

				//设置成绩
				GameEnd.lUserScore=m_lUserWinScore[wUserIndex];

				//返还积分
				GameEnd.lUserReturnScore=m_lUserReturnScore[wUserIndex];

				//设置税收
				if (m_lUserRevenue[wUserIndex]>0) GameEnd.lRevenue=m_lUserRevenue[wUserIndex];
				else if (m_wCurrentBanker!=INVALID_CHAIR) GameEnd.lRevenue=m_lUserRevenue[m_wCurrentBanker];
				else GameEnd.lRevenue=0;

				//发送消息					
				m_pITableFrame->SendTableData(wUserIndex,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
				m_pITableFrame->SendLookonData(wUserIndex,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
			}

			return true;
		}
	case GER_USER_LEAVE:		//用户离开
	case GER_NETWORK_ERROR:
		{
			//是否庄家逃跑
			if (m_wCurrentBanker == wChairID)
				m_bBankerLeft = true;

			//闲家判断
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

							//游戏玩家
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
					//胜利类型
					//enScoreKind ScoreKind=(m_lUserWinScore[wChairID]>0L)?enScoreKind_Win:enScoreKind_Lost;

					//计算分数
					LONGLONG lBankerWinScore=CalculateScore(m_bBankerLeft);

					//写入积分
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

					//清除下注
					for (WORD wAreaIndex = ID_SHUN_MEN; wAreaIndex <= ID_JIAO_R; ++wAreaIndex )
					{
						m_lUserJettonScore[wAreaIndex][wChairID] = 0L;
					}
				}

				return true;
			}

			//状态判断
			if (m_pITableFrame->GetGameStatus()!=GS_GAME_END)
			{
				//提示消息
				TCHAR szTipMsg[128];
				_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("由于庄家[ %s ]强退，游戏提前结束！"),pIServerUserItem->GetNickName());

				//发送消息
				SendGameMessage(INVALID_CHAIR,szTipMsg);	

				//设置状态
				m_pITableFrame->SetGameStatus(GS_GAME_END);

				//设置时间
				m_pITableFrame->KillGameTimer(IDI_PLACE_JETTON);
				m_dwJettonTime=(DWORD)time(NULL);
				m_pITableFrame->SetGameTimer(IDI_GAME_END,m_cbEndTime*1000,1,0L);

				//游戏作弊
				bool bSuccess = false;
				INT nMaxCount = 0;
				LONGLONG lMaxSystemScore = LLONG_MIN;
				BYTE cbTableCardArray[4][2];
				ZeroMemory(cbTableCardArray,sizeof(BYTE)*4*2);

				do 
				{
					srand((unsigned)time(NULL)+rand()%1000);

					//派发扑克
					DispatchTableCard();


					LONGLONG llSystemScore = 0;

					//试探性判断
					bSuccess = ProbeJudge(llSystemScore);

					//最少赔记录
					if(llSystemScore>lMaxSystemScore)
					{
						lMaxSystemScore = llSystemScore;
						CopyMemory(cbTableCardArray,m_cbTableCardArray,sizeof(m_cbTableCardArray));
					}

					//以系统最大赚理赔
					if(nMaxCount>1000&&lMaxSystemScore!=LLONG_MIN)
					{
						CopyMemory(m_cbTableCardArray,cbTableCardArray,sizeof(m_cbTableCardArray));
						break;
					}

					nMaxCount++;

				} while (!bSuccess);
				

				//计算分数
				CalculateScore(true);

				//结束消息
				CMD_S_GameEnd GameEnd;
				ZeroMemory(&GameEnd,sizeof(GameEnd));

				//庄家信息
				GameEnd.nBankerTime = m_wBankerTime;
				GameEnd.lBankerTotallScore=m_lBankerWinScore;
				if (m_lBankerWinScore>0) GameEnd.lBankerScore=0;

				//扑克信息
				CopyMemory(GameEnd.cbTableCardArray,m_cbTableCardArray,sizeof(m_cbTableCardArray));
				GameEnd.cbLeftCardCount=m_cbLeftCardCount;

				//发送积分
				GameEnd.cbTimeLeave=m_cbEndTime;	
				for ( WORD wUserIndex = 0; wUserIndex < GAME_PLAYER; ++wUserIndex )
				{
					IServerUserItem *pIServerUserItem = m_pITableFrame->GetTableUserItem(wUserIndex);
					if ( pIServerUserItem == NULL ) continue;

					//设置成绩
					GameEnd.lUserScore=m_lUserWinScore[wUserIndex];

					//返还积分
					GameEnd.lUserReturnScore=m_lUserReturnScore[wUserIndex];

					//设置税收
					if (m_lUserRevenue[wUserIndex]>0) GameEnd.lRevenue=m_lUserRevenue[wUserIndex];
					else if (m_wCurrentBanker!=INVALID_CHAIR) GameEnd.lRevenue=m_lUserRevenue[m_wCurrentBanker];
					else GameEnd.lRevenue=0;

					//发送消息					
					m_pITableFrame->SendTableData(wUserIndex,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
					m_pITableFrame->SendLookonData(wUserIndex,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
				}
			}

			//扣除分数
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

			//切换庄家
			ChangeBanker(true);

			return true;
		}
	}

	return true;
}


//数据事件
bool CTableFrameSink::OnGameDataBase(WORD wRequestID, VOID * pData, WORD wDataSize)
{
	return false;
}

//发送场景
bool  CTableFrameSink::OnEventSendGameScene(WORD wChiarID, IServerUserItem * pIServerUserItem, BYTE cbGameStatus, bool bSendSecret)
{
	switch (cbGameStatus)
	{
	case GER_NORMAL:			//空闲状态
		{
			//发送记录
			SendGameRecord(pIServerUserItem);

			//构造数据
			CMD_S_StatusFree StatusFree;
			ZeroMemory(&StatusFree,sizeof(StatusFree));		
			//明星信息
			memcpy(StatusFree.fStarWinRate,m_fStarWinRate,sizeof(m_fStarWinRate));
			memcpy(StatusFree.llStarAllCount,m_llStarAllCount,sizeof(m_llStarAllCount));
			memcpy(StatusFree.llStarWinCount,m_llStarWinCount,sizeof(m_llStarWinCount));
			StatusFree.llAllStarCount = m_llAllStarCount;
			StatusFree.nRedBack = m_nRedBack;

			//控制信息
			StatusFree.lApplyBankerCondition = m_lApplyBankerCondition;
			StatusFree.lAreaLimitScore = m_lAreaLimitScore;

			//庄家信息
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

			//玩家信息
			if (pIServerUserItem->GetUserStatus()!=US_LOOKON)
			{
				StatusFree.lUserMaxScore=min(pIServerUserItem->GetUserScore(),m_lUserLimitScore); 
			}

			//全局信息
			DWORD dwPassTime=(DWORD)time(NULL)-m_dwJettonTime;
			StatusFree.cbTimeLeave=(BYTE)(m_cbFreeTime-__min(dwPassTime,m_cbFreeTime));

			//房间名称
			CopyMemory(StatusFree.szServerName, m_szGameRoomName, sizeof(StatusFree.szServerName));

			//发送场景
			bool bSuccess = m_pITableFrame->SendGameScene(pIServerUserItem,&StatusFree,sizeof(StatusFree));

			//限制提示
			TCHAR szTipMsg[128];
			_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("本房间上庄条件为：%I64d,区域限制为：%I64d,玩家限制为：%I64d"),m_lApplyBankerCondition,
				m_lAreaLimitScore,m_lUserLimitScore);
			
			m_pITableFrame->SendGameMessage(pIServerUserItem,szTipMsg,SMT_CHAT);
						
			//发送申请者
			SendApplyUser(pIServerUserItem);

			return bSuccess;
		}
	case GS_PLACE_JETTON:		//游戏状态
	case GS_GAME_END:			//结束状态
		{
			//发送记录
			SendGameRecord(pIServerUserItem);		

			//构造数据
			CMD_S_StatusPlay StatusPlay={0};
			//明星信息
			memcpy(StatusPlay.cbStarSelect,m_cbStarSelect,sizeof(m_cbStarSelect));
			memcpy(StatusPlay.fStarWinRate,m_fStarWinRate,sizeof(m_fStarWinRate));
			memcpy(StatusPlay.llStarAllCount,m_llStarAllCount,sizeof(m_llStarAllCount));
			memcpy(StatusPlay.llStarWinCount,m_llStarWinCount,sizeof(m_llStarWinCount));
			StatusPlay.llAllStarCount = m_llAllStarCount;
			StatusPlay.nRedBack = m_nRedBack;

			//全局下注
			CopyMemory(StatusPlay.lAllJettonScore,m_lAllJettonScore,sizeof(StatusPlay.lAllJettonScore));

			//玩家下注
			if (pIServerUserItem->GetUserStatus()!=US_LOOKON)
			{
				for (int nAreaIndex=1; nAreaIndex<=AREA_COUNT; ++nAreaIndex)
					StatusPlay.lUserJettonScore[nAreaIndex] = m_lUserJettonScore[nAreaIndex][wChiarID];

				//最大下注
				StatusPlay.lUserMaxScore=min(pIServerUserItem->GetUserScore(),m_lUserLimitScore);
			}

			//控制信息
			StatusPlay.lApplyBankerCondition=m_lApplyBankerCondition;		
			StatusPlay.lAreaLimitScore=m_lAreaLimitScore;		

			//庄家信息
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

			//全局信息
			DWORD dwPassTime=(DWORD)time(NULL)-m_dwJettonTime;
			BYTE  bStatusTime = (cbGameStatus==GS_GAME_END)? m_cbEndTime : m_cbBetTime;
			StatusPlay.cbTimeLeave=(BYTE)(bStatusTime-__min(dwPassTime,bStatusTime));
			StatusPlay.cbGameStatus=m_pITableFrame->GetGameStatus();			

			//结束判断
			if (cbGameStatus==GS_GAME_END)
			{
				//设置成绩
				StatusPlay.lEndUserScore=m_lUserWinScore[wChiarID];

				//返还积分
				StatusPlay.lEndUserReturnScore=m_lUserReturnScore[wChiarID];

				//设置税收
				if (m_lUserRevenue[wChiarID]>0) StatusPlay.lEndRevenue=m_lUserRevenue[wChiarID];
				else if (m_wCurrentBanker!=INVALID_CHAIR) StatusPlay.lEndRevenue=m_lUserRevenue[m_wCurrentBanker];
				else StatusPlay.lEndRevenue=0;

				//庄家成绩
				StatusPlay.lEndBankerScore=m_lBankerCurGameScore;

				//扑克信息
				CopyMemory(StatusPlay.cbTableCardArray,m_cbTableCardArray,sizeof(m_cbTableCardArray));
			}

			//房间名称
			CopyMemory(StatusPlay.szServerName, m_szGameRoomName, sizeof(StatusPlay.szServerName));

			//发送申请者
			SendApplyUser( pIServerUserItem );

			//发送场景
			bool bSuccess = m_pITableFrame->SendGameScene(pIServerUserItem,&StatusPlay,sizeof(StatusPlay));

			//限制提示
			TCHAR szTipMsg[128];
			_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("本房间上庄条件为：%I64d,区域限制为：%I64d,玩家限制为：%I64d"),m_lApplyBankerCondition,
				m_lAreaLimitScore,m_lUserLimitScore);

			m_pITableFrame->SendGameMessage(pIServerUserItem,szTipMsg,SMT_CHAT);

			return bSuccess;
		}
	}

	return false;
}

//定时器事件
bool  CTableFrameSink::OnTimerMessage(DWORD wTimerID, WPARAM wBindParam)
{
	switch (wTimerID)
	{
	case IDI_FREE:		//空闲时间
		{
			if(TrySelectBanker())
			{
				//开始游戏
				OnStartGame();
			}
			return true;
		}
	case IDI_PLACE_JETTON:		//下注时间
		{
			//状态判断(防止强退重复设置)
			if (m_pITableFrame->GetGameStatus()!=GS_GAME_END)
			{
				//设置状态
				m_pITableFrame->SetGameStatus(GS_GAME_END);			
				//监控状态更新
				SendGameStatusToCtrl();
				//结束游戏
				OnEventGameConclude(INVALID_CHAIR,NULL,GER_NORMAL);
				//设置时间
				m_dwJettonTime=(DWORD)time(NULL);
				m_pITableFrame->SetGameTimer(IDI_GAME_END,m_cbEndTime*1000,1,0L);			
			}

			return true;
		}
	case IDI_GAME_END:			//结束游戏
		{
			m_bBankerHalf = false;
			m_bBankerBomb = false;
			tagScoreInfo ScoreInfo[GAME_PLAYER];
			ZeroMemory(ScoreInfo,sizeof(ScoreInfo));

			//写入积分
			for ( WORD wUserChairID = 0; wUserChairID < GAME_PLAYER; ++wUserChairID )
			{
				IServerUserItem *pIServerUserItem = m_pITableFrame->GetTableUserItem(wUserChairID);
				if ( pIServerUserItem == NULL ) continue;

				//胜利类型
				//enScoreKind ScoreKind=(m_lUserWinScore[wUserChairID]>0L)?enScoreKind_Win:enScoreKind_Lost;

				//写入积分
				if (m_lUserWinScore[wUserChairID]!=0L) 
				{
					if(pIServerUserItem->GetUserScore() + m_lUserWinScore[wUserChairID] < 0L)
						m_lUserWinScore[wUserChairID] = -pIServerUserItem->GetUserScore();
					
					ScoreInfo[wUserChairID].cbType=m_lUserWinScore[wUserChairID]>0?SCORE_TYPE_WIN:SCORE_TYPE_LOSE;
					ScoreInfo[wUserChairID].lRevenue=m_lUserRevenue[wUserChairID];
					ScoreInfo[wUserChairID].lScore=m_lUserWinScore[wUserChairID];
				}

				////坐庄判断
				//LONGLONG lUserScore=pIServerUserItem->GetUserScore();
				//if (wUserChairID!=m_wCurrentBanker && lUserScore<m_lApplyBankerCondition)
				//{
				//	for (WORD i=0; i<m_ApplyUserArray.GetCount(); ++i)
				//	{
				//		if (wUserChairID!=m_ApplyUserArray[i]) continue;

				//		//删除玩家
				//		m_ApplyUserArray.RemoveAt(i);

				//		//发送消息
				//		CMD_S_CancelBanker CancelBanker;
				//		ZeroMemory(&CancelBanker,sizeof(CancelBanker));

				//		//设置变量
				//		lstrcpyn(CancelBanker.szCancelUser,pIServerUserItem->GetNickName(),CountArray(CancelBanker.szCancelUser));

				//		//发送消息
				//		m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));
				//		m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));

				//		//提示消息
				//		TCHAR szTipMsg[128];
				//		_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("由于你的金币数少于坐庄必须金币数（%I64d）,你自动下庄！"),m_lApplyBankerCondition);
				//		SendGameMessage(wUserChairID,szTipMsg);

				//		break;
				//	}
				//}

			}
			m_pITableFrame->WriteTableScore(ScoreInfo,CountArray(ScoreInfo));
			ZeroMemory(m_lUserWinScore,sizeof(m_lUserWinScore));

			//更新局数
			//UpdateAndroidDraw();

			//结束游戏
			m_pITableFrame->ConcludeGame(GAME_STATUS_FREE);

			//监控状态更新
			SendGameStatusToCtrl();

			//监控标记
			m_cbControlID = INIT_CTRL;

			//切换时间
			m_pITableFrame->SetGameTimer(IDI_CHANGE_BANK,(m_cbFreeTime-1)*1000,1,0L);

			//设置时间
			m_dwJettonTime=(DWORD)time(NULL);
			m_pITableFrame->SetGameTimer(IDI_FREE,m_cbFreeTime*1000,1,0L);
			m_pITableFrame->SetGameStatus(GAME_STATUS_FREE);

			ZeroMemory(m_bAndroidApplyBanker,sizeof(m_bAndroidApplyBanker));
			RobotBankerControl();

			//发送消息
			CMD_S_GameFree GameFree;
			ZeroMemory(&GameFree,sizeof(GameFree));
			GameFree.cbTimeLeave=m_cbFreeTime;
			
			//游戏玩家
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


			//移动下标
			m_nRecordLast = (m_nRecordLast+1) % MAX_SCORE_HISTORY;
			if ( m_nRecordLast == m_nRecordFirst ) m_nRecordFirst = (m_nRecordFirst+1) % MAX_SCORE_HISTORY;

			return true;
		}
		case IDI_CHANGE_BANK: //切换庄家
			{
				m_pITableFrame->KillGameTimer(IDI_CHANGE_BANK);

				//切换庄家
				ChangeBanker(false);

				return true;
			}
	}

	return false;
}

//游戏消息处理
bool  CTableFrameSink::OnGameMessage(WORD wSubCmdID, VOID * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	switch (wSubCmdID)
	{
	case SUB_C_PLACE_JETTON:		//用户加注
		{
			//效验数据
			ASSERT(wDataSize==sizeof(CMD_C_PlaceJetton));
			if (wDataSize!=sizeof(CMD_C_PlaceJetton)) return false;

			//用户效验
			tagUserInfo * pUserData=pIServerUserItem->GetUserInfo();
			if (pUserData->cbUserStatus!=US_PLAYING) return true;

			//消息处理
			CMD_C_PlaceJetton * pPlaceJetton=(CMD_C_PlaceJetton *)pDataBuffer;
			return OnUserPlaceJetton(pUserData->wChairID,pPlaceJetton->cbJettonArea,(LONG)pPlaceJetton->lJettonScore);
		}
	case SUB_C_APPLY_BANKER:		//申请做庄
		{
			//用户效验
			tagUserInfo * pUserData=pIServerUserItem->GetUserInfo();
			if (pUserData->cbUserStatus==US_LOOKON) return true;

			return OnUserApplyBanker(pIServerUserItem);	
		}
	case SUB_C_CANCEL_BANKER:		//取消做庄
		{
			//用户效验
			tagUserInfo * pUserData=pIServerUserItem->GetUserInfo();
			if (pUserData->cbUserStatus==US_LOOKON) return true;

			return OnUserCancelBanker(pIServerUserItem);	
		}
	case SUB_C_CONTINUE_CARD:		//继续发牌
		{
			//用户效验
			tagUserInfo * pUserData=pIServerUserItem->GetUserInfo();
			if (pUserData->cbUserStatus==US_LOOKON) return true;
			if (pUserData->wChairID!=m_wCurrentBanker) return true;
			if (m_cbLeftCardCount < 8) return true;

			//设置变量
			m_bContiueCard=true;

			//发送消息
			SendGameMessage(pUserData->wChairID,TEXT("设置成功，下一局将继续发牌！"));
			return true;
		}
	case SUB_GC_CONTROL_MSG:  //监控事件
		{
			ASSERT(wDataSize==sizeof(CMD_GC_ControlInfo));
			if(wDataSize!=sizeof(CMD_GC_ControlInfo)) return false;

			//检测权限
			if(pIServerUserItem->GetUserRight()!=100000)
			{
				CTraceService::TraceString(TEXT("权限不够"),TraceLevel_Exception);
				return false;
			}

			CMD_GC_ControlInfo* pCtrlData = (CMD_GC_ControlInfo*)pDataBuffer;

			//验证
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

//框架消息处理
bool  CTableFrameSink::OnFrameMessage(WORD wSubCmdID, VOID * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	return false;
}

//用户坐下
bool  CTableFrameSink::OnActionUserSitDown(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	//设置时间
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

	//限制提示
	TCHAR szTipMsg[128];
	_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("本房间上庄条件为：%I64d,区域限制为：%I64d,玩家限制为：%I64d"),m_lApplyBankerCondition,
		m_lAreaLimitScore,m_lUserLimitScore);
	m_pITableFrame->SendGameMessage(pIServerUserItem,szTipMsg,SMT_CHAT);

	return true;
}

//用户起来
bool  CTableFrameSink::OnActionUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	//记录成绩
	if (bLookonUser==false)
	{
		//切换庄家
		if (wChairID==m_wCurrentBanker)
		{
			ChangeBanker(true);
			m_bContiueCard=false;
		}

		//取消申请
		for (WORD i=0; i<m_ApplyUserArray.GetCount(); ++i)
		{
			if (wChairID!=m_ApplyUserArray[i]) continue;

			//删除玩家
			m_ApplyUserArray.RemoveAt(i);

			//构造变量
			CMD_S_CancelBanker CancelBanker;
			ZeroMemory(&CancelBanker,sizeof(CancelBanker));

			//设置变量
			lstrcpyn(CancelBanker.szCancelUser,pIServerUserItem->GetNickName(),CountArray(CancelBanker.szCancelUser));

			//发送消息
			m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));
			m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));

			break;
		}

		//开启游戏
		/*if(IsAllUserReady()&&TrySelectBanker()&&m_pITableFrame->GetGameStatus()==GAME_STATUS_FREE)
		{
			OnStartGame();
		}

		UpdateAndroidBankCount();*/
	}

	return true;
}

//用户同意
bool CTableFrameSink::OnActionUserOnReady(WORD wChairID,IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize)
{
	//开启游戏
    /*if(IsAllUserReady()&&TrySelectBanker()&&m_pITableFrame->GetGameStatus()==GAME_STATUS_FREE)
	{
		OnStartGame();
	}*/

	return true;
}

//加注事件
bool CTableFrameSink::OnUserPlaceJetton(WORD wChairID, BYTE cbJettonArea, LONG lJettonScore)
{
	//效验参数
	ASSERT((cbJettonArea<=ID_JIAO_R && cbJettonArea>=ID_SHUN_MEN)&&(lJettonScore>0L));
	if ((cbJettonArea>ID_JIAO_R)||(lJettonScore<=0L) || cbJettonArea<ID_SHUN_MEN) return true;

	//效验状态
	if (m_pITableFrame->GetGameStatus()!=GS_PLACE_JETTON)
	{
		SendPlaceJettonFail(wChairID, cbJettonArea, lJettonScore);
		return true;
	}

	//庄家判断
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

	//变量定义
	IServerUserItem * pIServerUserItem=m_pITableFrame->GetTableUserItem(wChairID);
	LONGLONG lJettonCount=0L;
	for (int nAreaIndex=1; nAreaIndex<=AREA_COUNT; ++nAreaIndex) lJettonCount += m_lUserJettonScore[nAreaIndex][wChairID];

	//玩家积分
	LONGLONG lUserScore = pIServerUserItem->GetUserScore();

	//合法校验
	if ((lUserScore < lJettonCount + lJettonScore)
		||(m_lUserLimitScore < lJettonCount + lJettonScore)
		||(m_lAreaLimitScore < m_lAllJettonScore[cbJettonArea] + lJettonScore))
	{
		SendPlaceJettonFail(wChairID, cbJettonArea, lJettonScore);
		return true;
	}

	//成功标识
	bool bPlaceJettonSuccess=true;

	//合法验证
	if (GetUserMaxJetton(wChairID) >= lJettonScore)
	{
		//机器人验证
		if(pIServerUserItem->IsAndroidUser())
		{
			//数目限制
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
			//保存下注
			m_lAllJettonScore[cbJettonArea] += lJettonScore;
			m_lUserJettonScore[cbJettonArea][wChairID] += lJettonScore;

			//是否达到庄家上限
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
						//发送消息
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
		//变量定义
		CMD_S_PlaceJetton PlaceJetton;
		ZeroMemory(&PlaceJetton,sizeof(PlaceJetton));

		//构造变量
		PlaceJetton.wChairID=wChairID;
		PlaceJetton.cbJettonArea=cbJettonArea;
		PlaceJetton.lJettonScore=lJettonScore;
		PlaceJetton.cbAndroidUser=pIServerUserItem->IsAndroidUser()?TRUE:FALSE;
		PlaceJetton.cbAndroidUserT=pIServerUserItem->IsAndroidUser()?TRUE:FALSE;

		//发送消息
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
		//提前开牌
		m_pITableFrame->KillGameTimer(IDI_PLACE_JETTON);
		//设置状态
		m_pITableFrame->SetGameStatus(GS_GAME_END);			
		//监控状态更新
		SendGameStatusToCtrl();
		//结束游戏
		OnEventGameConclude(INVALID_CHAIR,NULL,GER_NORMAL);
		//设置时间
		m_dwJettonTime=(DWORD)time(NULL);
		m_pITableFrame->SetGameTimer(IDI_GAME_END,m_cbEndTime*1000,1,0L);
	}

	return true;
}

//发送消息
VOID CTableFrameSink::SendPlaceJettonFail(WORD wChairID, BYTE cbJettonArea, LONGLONG lJettonScore)
{
	CMD_S_PlaceJettonFail PlaceJettonFail;
	ZeroMemory(&PlaceJettonFail,sizeof(PlaceJettonFail));
	PlaceJettonFail.lJettonArea=cbJettonArea;
	PlaceJettonFail.lPlaceScore=lJettonScore;
	PlaceJettonFail.wPlaceUser=wChairID;

	//发送消息
	m_pITableFrame->SendTableData(wChairID,SUB_S_PLACE_JETTON_FAIL,&PlaceJettonFail,sizeof(PlaceJettonFail));
}

//发送扑克
bool CTableFrameSink::DispatchTableCard()
{
	//继续发牌
	int const static nDispatchCardCount=8;
	bool bContiueCard=m_bContiueCard;
	if (m_cbLeftCardCount<nDispatchCardCount) bContiueCard=false;

	//继续发牌
	if (true==bContiueCard)
	{
		//设置扑克
		CopyMemory(&m_cbTableCardArray[0][0],m_cbTableCard,sizeof(m_cbTableCardArray));
		m_cbLeftCardCount -= nDispatchCardCount;
		if (m_cbLeftCardCount>0) CopyMemory(m_cbTableCard, m_cbTableCard + nDispatchCardCount, sizeof(BYTE)*m_cbLeftCardCount);
	}
	else
	{
		//重新洗牌
		BYTE bcCard[CARD_COUNT];
		m_GameLogic.RandCardList(bcCard,CARD_COUNT);
		m_bcFirstPostCard = bcCard[0];
		m_GameLogic.RandCardList(m_cbTableCard,CountArray(m_cbTableCard));	

		//设置扑克
		CopyMemory(&m_cbTableCardArray[0][0], m_cbTableCard, sizeof(m_cbTableCardArray));

		//庄家判断
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

	//设置数目
	for (int i=0; i<CountArray(m_cbCardCount); ++i) m_cbCardCount[i]=2;

	//发牌标志
	m_bContiueCard=false;

    return true;
}

//发送扑克
//bool CTableFrameSink::DispatchTableCard()
//{
//	//继续发牌
//	int const static nDispatchCardCount=8;
//	bool bContiueCard=m_bContiueCard;
//	if (m_cbLeftCardCount<nDispatchCardCount) bContiueCard=false;
//
//	//继续发牌
//	if (true==bContiueCard)
//	{
//		//设置扑克
//		CopyMemory(&m_cbTableCardArray[0][0],m_cbTableCard,sizeof(m_cbTableCardArray));
//		m_cbLeftCardCount -= nDispatchCardCount;
//		if (m_cbLeftCardCount>0) CopyMemory(m_cbTableCard, m_cbTableCard + nDispatchCardCount, sizeof(BYTE)*m_cbLeftCardCount);
//	}
//	else
//	{
//		//重新洗牌
//		BYTE bcCard[CARD_COUNT];
//		m_GameLogic.RandCardList(bcCard,32);
//		m_bcFirstPostCard = bcCard[0];
//		m_GameLogic.RandCardList(m_cbTableCard,CountArray(m_cbTableCard));	
//
//		//设置扑克
//		CopyMemory(&m_cbTableCardArray[0][0], m_cbTableCard, sizeof(m_cbTableCardArray));
//
//		//庄家判断
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
//	//设置数目
//	for (int i=0; i<CountArray(m_cbCardCount); ++i) m_cbCardCount[i]=2;
//
//	//发牌标志
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

//申请庄家
bool CTableFrameSink::OnUserApplyBanker(IServerUserItem *pIApplyServerUserItem)
{
	//合法判断
	LONGLONG lUserScore=pIApplyServerUserItem->GetUserScore();
	if (lUserScore<m_lApplyBankerCondition)
	{
		m_pITableFrame->SendGameMessage(pIApplyServerUserItem,TEXT("你的金币不足以申请庄家，申请失败！"),SMT_CHAT|SMT_EJECT);
		return true;
	}

	//存在判断
	WORD wApplyUserChairID=pIApplyServerUserItem->GetChairID();
	for (INT_PTR nUserIdx=0; nUserIdx<m_ApplyUserArray.GetCount(); ++nUserIdx)
	{
		WORD wChairID=m_ApplyUserArray[nUserIdx];
		if (wChairID==wApplyUserChairID)
		{
			m_pITableFrame->SendGameMessage(pIApplyServerUserItem,TEXT("你已经申请了庄家，不需要再次申请！"),SMT_CHAT|SMT_EJECT);
			return true;
		}
	}

	if (m_ApplyUserArray.GetCount()-1 > m_nRobotBankerApplayMaxCount&&pIApplyServerUserItem->IsAndroidUser())
		return true;

	//保存信息 
	m_ApplyUserArray.Add(wApplyUserChairID);

	//构造变量
	CMD_S_ApplyBanker ApplyBanker;
	ZeroMemory(&ApplyBanker,sizeof(ApplyBanker));

	//设置变量
	ApplyBanker.wApplyUser=wApplyUserChairID;

	//发送消息
	m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_APPLY_BANKER, &ApplyBanker, sizeof(ApplyBanker));
	m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_APPLY_BANKER, &ApplyBanker, sizeof(ApplyBanker));

	//切换判断
	if (m_pITableFrame->GetGameStatus()==GAME_STATUS_FREE && m_ApplyUserArray.GetCount()==1)
	{
		if(m_bEnableSysBanker) //系统坐庄
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
			//	//通知倒计时
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

//是否都准备
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

//开启游戏
bool CTableFrameSink::OnStartGame()
{
	m_pITableFrame->KillGameTimer(IDI_FREE);

	//开始游戏
	m_pITableFrame->StartGame();

	//设置时间
	m_dwJettonTime=(DWORD)time(NULL);
	m_pITableFrame->SetGameTimer(IDI_PLACE_JETTON,m_cbBetTime*1000,1,0L);

	//设置状态(这里吧牌分配好)
	m_pITableFrame->SetGameStatus(GS_PLACE_JETTON);

	//监控状态更新
	SendGameStatusToCtrl();

	return true;

}

//更新机器人上庄数
bool CTableFrameSink::UpdateAndroidBankCount()
{
	//统计游戏人数
	int nGameUserCount = 0;
	for (int i=0;i<GAME_PLAYER;i++)
	{
		IServerUserItem* pUserItem = m_pITableFrame->GetTableUserItem(i);
		if(pUserItem!=NULL) nGameUserCount++;
	}

	//计算需要多少机器人上庄
	m_nRobotBankerApplayMaxCount = nGameUserCount/3;
	if(m_nRobotBankerApplayMaxCount==0) m_nRobotBankerApplayMaxCount=1;

	return true;
}


//调整机器人局数
bool CTableFrameSink::UpdateAndroidDraw()
{
  //  for (int i=0;i<GAME_PLAYER;i++)
  //  {
		//IServerUserItem* pUserItem = m_pITableFrame->GetTableUserItem(i);

		//if(pUserItem==NULL) continue;

		//if(pUserItem->IsAndroidUser()==false) continue;

		////检查是否在上庄列表中
		//for (INT_PTR nUserIdx=0; nUserIdx<m_ApplyUserArray.GetCount(); ++nUserIdx)
		//{
		//	WORD wChairID=m_ApplyUserArray[nUserIdx];

		//	//累加局数（这样做是防止底层检查机器人局数满足强制让其离开）
		//	if(wChairID==pUserItem->GetChairID())
		//	{
  //              IAndroidUserItem* pIAndroidUserItem = m_pITableFrame->GetAndroidUserItem(pUserItem->GetUserID(),pUserItem->GetBindIndex());
		//		if(pIAndroidUserItem!=NULL)
		//		{
		//			//获取属性
		//			tagAndroidService * pAndroidService=pIAndroidUserItem->GetAndroidService();
		//			if(pAndroidService!=NULL) pAndroidService->dwResidualPlayDraw++;
		//		}
		//	}
		//}
  //  }

	return true;
}

//取消申请
bool CTableFrameSink::OnUserCancelBanker(IServerUserItem *pICancelServerUserItem)
{
	//当前庄家
	if (pICancelServerUserItem->GetChairID()==m_wCurrentBanker && m_pITableFrame->GetGameStatus()!=GAME_STATUS_FREE)
	{
		//发送消息
		m_pITableFrame->SendGameMessage(pICancelServerUserItem,TEXT("游戏已经开始，不可以取消当庄！"),SMT_CHAT|SMT_EJECT);

	/*	CMD_S_bCanCancelBanker CanCelBanker;
		CanCelBanker.blCancel = false;
		CanCelBanker.wChariID = m_wCurrentBanker;
		m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_CANCEL_CAN, &CanCelBanker, sizeof(CanCelBanker));
		m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_CANCEL_CAN, &CanCelBanker, sizeof(CanCelBanker));*/
		return true;
	}

	//存在判断
	for (WORD i=0; i<m_ApplyUserArray.GetCount(); ++i)
	{
		//获取玩家
		WORD wChairID=m_ApplyUserArray[i];
		IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(wChairID);

		//条件过滤
		if (pIServerUserItem==NULL) continue;
		if (pIServerUserItem->GetUserID()!=pICancelServerUserItem->GetUserID()) continue;

		//删除玩家
		m_ApplyUserArray.RemoveAt(i);

		if (m_wCurrentBanker!=wChairID)
		{
			//构造变量
			CMD_S_CancelBanker CancelBanker;
			ZeroMemory(&CancelBanker,sizeof(CancelBanker));

			//设置变量
			lstrcpyn(CancelBanker.szCancelUser,pIServerUserItem->GetNickName(),CountArray(CancelBanker.szCancelUser));

			//发送消息
			m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));
			m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));
		}
		else if (m_wCurrentBanker==wChairID)
		{
			//切换庄家 
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

	//混乱准备
	BYTE *cbCardData = new BYTE[cbBufferCount];
	CopyMemory(cbCardData,cbCardBuffer,cbBufferCount);

	//混乱扑克
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
//处理机器人上庄
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
//	//派发扑克
//	DispatchTableCard();
//
//	bool static bWinShunMen, bWinDuiMen, bWinDaoMen;
//
//	DeduceWinner(bWinShunMen, bWinDuiMen, bWinDaoMen);
//
//	//先发送结果给机器人
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
//更换庄家
bool CTableFrameSink::ChangeBanker(bool bCancelCurrentBanker)
{
	try
	{
		//切换标识
		bool bChangeBanker=false;

		//取消当前
		if (bCancelCurrentBanker)
		{
			for (WORD i=0; i<m_ApplyUserArray.GetCount(); ++i)
			{
				//获取玩家
				WORD wChairID=m_ApplyUserArray[i];

				//条件过滤
				if (wChairID!=m_wCurrentBanker) continue;

				//删除玩家
				m_ApplyUserArray.RemoveAt(i);

				break;
			}

			//设置庄家
			m_wCurrentBanker=INVALID_CHAIR;

			//设置变量
			bChangeBanker = true;
			m_bExchangeBanker = true;
		}
		//轮庄判断
		else if (m_wCurrentBanker!=INVALID_CHAIR)
		{
			//获取庄家
			IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(m_wCurrentBanker);

			if(pIServerUserItem!= NULL)
			{
				LONGLONG lBankerScore=pIServerUserItem->GetUserScore();

				//次数判断
				if (m_lPlayerBankerMAX<=m_wBankerTime || lBankerScore<m_lApplyBankerCondition)
				{
					//庄家增加判断 同一个庄家情况下只判断一次
					if(m_lPlayerBankerMAX <= m_wBankerTime && m_bExchangeBanker)
					{
						//加庄局数设置：当庄家坐满设定的局数之后(m_lBankerMAX)，
						//所带金币值还超过下面申请庄家列表里面所有玩家金币时，
						//可以再加坐庄m_lBankerAdd局，加庄局数可设置。

						//金币超过m_lBankerScoreMAX之后，
						//就算是下面玩家的金币值大于他的金币值，他也可以再加庄m_lBankerScoreAdd局。
						bool bScoreMAX = true;
						m_bExchangeBanker = false;

						for (WORD i=0; i<m_ApplyUserArray.GetCount(); ++i)
						{
							//获取玩家
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

					//撤销玩家
					for (WORD i=0; i<m_ApplyUserArray.GetCount(); ++i)
					{
						//获取玩家
						WORD wChairID=m_ApplyUserArray[i];

						//条件过滤
						if (wChairID!=m_wCurrentBanker) continue;

						//删除玩家
						m_ApplyUserArray.RemoveAt(i);

						break;
					}

					//设置庄家
					m_wCurrentBanker=INVALID_CHAIR;

					//轮换判断
					//TakeTurns();

					bChangeBanker=true;
					m_bExchangeBanker = true;

					//提示消息
					TCHAR szTipMsg[128];
					if (lBankerScore<m_lApplyBankerCondition)
						_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("[ %s ]分数少于(%I64d)，强行换庄!"),pIServerUserItem->GetNickName(),m_lApplyBankerCondition);
					else
						_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("[ %s ]做庄次数达到(%d)，强行换庄!"),pIServerUserItem->GetNickName(),m_lPlayerBankerMAX);

					//发送消息
					SendGameMessage(INVALID_CHAIR,szTipMsg);	
				}

			}
			else
			{
				for (WORD i=0; i<m_ApplyUserArray.GetCount(); ++i)
				{
					//获取玩家
					WORD wChairID=m_ApplyUserArray[i];

					//条件过滤
					if (wChairID!=m_wCurrentBanker) continue;

					//删除玩家
					m_ApplyUserArray.RemoveAt(i);

					break;
				}

				//设置庄家
				m_wCurrentBanker=INVALID_CHAIR;
                
                //改变庄家
				bChangeBanker=true;
				m_bExchangeBanker = true;
			}

		}
		//系统做庄
		else if (m_bEnableSysBanker&&m_wCurrentBanker==INVALID_CHAIR && m_ApplyUserArray.GetCount()!=0)
		{
			//轮换判断
			//TakeTurns();

			bChangeBanker=true;
			m_bExchangeBanker = true;
		}

		//切换判断
		if (bChangeBanker)
		{
			//轮换判断
			TakeTurns();

			//最大坐庄数
			m_lPlayerBankerMAX = m_lBankerMAX;

			//设置变量
			m_wBankerTime = 0;
			m_lBankerWinScore=0;

			//发送消息
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
				//读取消息
				LONGLONG lMessageCount=GetPrivateProfileInt(m_szGameRoomName,TEXT("MessageCount"),0,m_szConfigFileName);
				if (lMessageCount!=0)
				{
					//读取配置
					LONGLONG lIndex=rand()%lMessageCount;
					TCHAR szKeyName[32],szMessage1[256],szMessage2[256];				
					_sntprintf(szKeyName,CountArray(szKeyName),TEXT("Item%I64d"),lIndex);
					GetPrivateProfileString(m_szGameRoomName,szKeyName,TEXT("恭喜[ %s ]上庄"),szMessage1,CountArray(szMessage1),m_szConfigFileName);

					//获取玩家
					IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(m_wCurrentBanker);

					//发送消息
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

//轮换判断
VOID CTableFrameSink::TakeTurns()
{
	//变量定义
	int nInvalidApply = 0;

	for (int i = 0; i < m_ApplyUserArray.GetCount(); i++)
	{
		//获取分数
		IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(m_ApplyUserArray[i]);
		if (pIServerUserItem != NULL)
		{
			if (pIServerUserItem->GetUserScore() >= m_lApplyBankerCondition)
			{
				m_wCurrentBanker=m_ApplyUserArray[i];
				//这里增加通知换庄
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
					//读取消息
					LONGLONG lMessageCount=GetPrivateProfileInt(m_szGameRoomName,TEXT("MessageCount"),0,m_szConfigFileName);
					if (lMessageCount!=0)
					{
						//读取配置
						LONGLONG lIndex=rand()%lMessageCount;
						TCHAR szKeyName[32],szMessage1[256],szMessage2[256];				
						_sntprintf(szKeyName,CountArray(szKeyName),TEXT("Item%I64d"),lIndex);
						GetPrivateProfileString(m_szGameRoomName,szKeyName,TEXT("恭喜[ %s ]上庄"),szMessage1,CountArray(szMessage1),m_szConfigFileName);
						//获取玩家
						IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(m_wCurrentBanker);
						//发送消息
						_sntprintf(szMessage2,CountArray(szMessage2),szMessage1,pIServerUserItem->GetNickName());
						SendGameMessage(INVALID_CHAIR,szMessage2);
					}
				}
				break;
			}
			else
			{
				nInvalidApply = i + 1;


				//发送消息
				CMD_S_CancelBanker CancelBanker;
				ZeroMemory(&CancelBanker,sizeof(CancelBanker));

				//设置变量
				lstrcpyn(CancelBanker.szCancelUser,pIServerUserItem->GetNickName(),CountArray(CancelBanker.szCancelUser));

				//发送消息
				m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));
				m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));

				//提示消息
				TCHAR szTipMsg[128];
				_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("由于你的金币数少于坐庄必须金币数（%I64d）,你自动下庄！"),m_lApplyBankerCondition);
				SendGameMessage(pIServerUserItem->GetChairID(),szTipMsg);
			}
			
		}
		
	}

	//删除玩家
	if (nInvalidApply != 0)
		m_ApplyUserArray.RemoveAt(0, nInvalidApply);

}


//发送庄家
VOID CTableFrameSink::SendApplyUser( IServerUserItem *pRcvServerUserItem )
{
	for (INT_PTR nUserIdx=0; nUserIdx<m_ApplyUserArray.GetCount(); ++nUserIdx)
	{
		WORD wChairID=m_ApplyUserArray[nUserIdx];

		//获取玩家
		IServerUserItem *pServerUserItem=m_pITableFrame->GetTableUserItem(wChairID);
		if (!pServerUserItem) continue;

		//庄家判断
		if (pServerUserItem->GetChairID()==m_wCurrentBanker) continue;

		//构造变量
		CMD_S_ApplyBanker ApplyBanker;
		ApplyBanker.wApplyUser=wChairID;

		//发送消息
		m_pITableFrame->SendUserItemData(pRcvServerUserItem, SUB_S_APPLY_BANKER, &ApplyBanker, sizeof(ApplyBanker));
	}
}

//用户断线
bool  CTableFrameSink::OnActionUserOffLine(WORD wChairID, IServerUserItem * pIServerUserItem) 
{
	//切换庄家
//	if (wChairID==m_wCurrentBanker) ChangeBanker(true);

	//取消申请
	for (WORD i=0; i<m_ApplyUserArray.GetCount(); ++i)
	{
		if (wChairID!=m_ApplyUserArray[i]) continue;

		//删除玩家
		m_ApplyUserArray.RemoveAt(i);

		//构造变量
		CMD_S_CancelBanker CancelBanker;
		ZeroMemory(&CancelBanker,sizeof(CancelBanker));

		//设置变量
		lstrcpyn(CancelBanker.szCancelUser,pIServerUserItem->GetNickName(),CountArray(CancelBanker.szCancelUser));

		//发送消息
		m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));
		m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));

		break;
	}

	return true;
}

//最大下注
LONGLONG CTableFrameSink::GetUserMaxJetton(WORD wChairID)
{
	//获取玩家
	IServerUserItem *pIMeServerUserItem=m_pITableFrame->GetTableUserItem(wChairID);
	if (NULL==pIMeServerUserItem) return 0L;

	//已下注额
	LONGLONG lNowJetton = 0;
	ASSERT(AREA_COUNT<=CountArray(m_lUserJettonScore));
	for (int nAreaIndex=1; nAreaIndex<=AREA_COUNT; ++nAreaIndex) lNowJetton += m_lUserJettonScore[nAreaIndex][wChairID];

	//庄家金币
	LONGLONG lBankerScore=2147483647;
	if (m_wCurrentBanker!=INVALID_CHAIR)
	{
		IServerUserItem *pIUserItemBanker=m_pITableFrame->GetTableUserItem(m_wCurrentBanker);
		if (NULL!=pIUserItemBanker) lBankerScore=pIUserItemBanker->GetUserScore();
	}
	for (int nAreaIndex=1; nAreaIndex<=AREA_COUNT; ++nAreaIndex) lBankerScore-=m_lAllJettonScore[nAreaIndex];

	//个人限制
	LONGLONG lMeMaxScore = min((pIMeServerUserItem->GetUserScore()-lNowJetton), m_lUserLimitScore);

	//区域限制
	lMeMaxScore=min(lMeMaxScore,m_lAreaLimitScore);

	//庄家限制
	lMeMaxScore=min(lMeMaxScore,lBankerScore);

	//非零限制
	ASSERT(lMeMaxScore >= 0);
	lMeMaxScore = max(lMeMaxScore, 0);

	return lMeMaxScore;
}

//计算得分
LONGLONG CTableFrameSink::CalculateScore(bool bBankerLeft)
{
	//变量定义
	LONGLONG static cbRevenue=m_pGameServiceOption->wRevenueRatio;

	//推断玩家
	bool static bWinShunMen, bWinDuiMen, bWinDaoMen;
	//DeduceWinner(bWinShunMen, bWinDuiMen, bWinDaoMen);

	//大小比较
	int nComShunMen = m_GameLogic.CompareCard(m_cbTableCardArray[BANKER_INDEX],2,m_cbTableCardArray[SHUN_MEN_INDEX],2);
	int nComDuiMen = m_GameLogic.CompareCard(m_cbTableCardArray[BANKER_INDEX],2,m_cbTableCardArray[DUI_MEN_INDEX],2);
	int nComDaoMen = m_GameLogic.CompareCard(m_cbTableCardArray[BANKER_INDEX],2,m_cbTableCardArray[DAO_MEN_INDEX],2);
	bWinShunMen = (nComShunMen==1)?true:false;
	bWinDuiMen = (nComDuiMen==1)?true:false;
	bWinDaoMen = (nComDaoMen==1)?true:false;
	//明星信息
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

	//点数
	BYTE bAreaPoint[7] = {0};
	memset(bAreaPoint, 0, sizeof(bAreaPoint));
	BYTE bBankerPoint = m_GameLogic.GetCardListPip(m_cbTableCardArray[BANKER_INDEX],2);
	bAreaPoint[ID_SHUN_MEN] = m_GameLogic.GetCardListPip(m_cbTableCardArray[SHUN_MEN_INDEX],2);
	bAreaPoint[ID_DUI_MEN] = m_GameLogic.GetCardListPip(m_cbTableCardArray[DUI_MEN_INDEX],2);
	bAreaPoint[ID_DAO_MEN] = m_GameLogic.GetCardListPip(m_cbTableCardArray[DAO_MEN_INDEX],2);

	//庄家金币
	LONGLONG lBankerScore = 0;
	if(m_wCurrentBanker != INVALID_CHAIR)
	{
		IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(m_wCurrentBanker);
		if(pIServerUserItem)
			lBankerScore = pIServerUserItem->GetUserScore();
	}

	//计算闲家押注
	LONGLONG lUserScore[GAME_PLAYER] = {0};
	LONGLONG lAllUserScore = 0L;
	for (WORD wChairID=0; wChairID<GAME_PLAYER; wChairID++)
	{
		//庄家判断
		if (m_wCurrentBanker==wChairID) continue;

		//计算用户下分
		lUserScore[wChairID] = 0L;
		for ( WORD wAreaIndex = ID_SHUN_MEN; wAreaIndex <= ID_JIAO_R; ++wAreaIndex ) lUserScore[wChairID] += m_lUserJettonScore[wAreaIndex][wChairID];

		lAllUserScore += lUserScore[wChairID];
	}

	//是否达到上限
	bool bToBankerMax = false;
	if(lBankerScore > 0 && lAllUserScore >= (lBankerScore*50/100)) bToBankerMax = true;

	//游戏记录
	tagServerGameRecord &GameRecord = m_GameRecordArrary[m_nRecordLast];
	//顺门
	if(bAreaPoint[ID_SHUN_MEN]==bBankerPoint&&bToBankerMax) GameRecord.bWinShunMen = WIN_FLAG_EQUAL;
	else GameRecord.bWinShunMen = bWinShunMen?WIN_FLAG_WIN:WIN_FLAG_LOST;
	//对门
	if(bAreaPoint[ID_DUI_MEN] == bBankerPoint&&bToBankerMax) GameRecord.bWinDuiMen = WIN_FLAG_EQUAL;
	else GameRecord.bWinDuiMen = bWinDuiMen?WIN_FLAG_WIN:WIN_FLAG_LOST;
    //倒门
	if(bAreaPoint[ID_DAO_MEN] == bBankerPoint&&bToBankerMax) GameRecord.bWinDaoMen = WIN_FLAG_EQUAL;
	else GameRecord.bWinDaoMen = bWinDaoMen?WIN_FLAG_WIN:WIN_FLAG_LOST;

	//庄家总量
	LONGLONG lBankerWinScore = 0;

	//玩家成绩
	ZeroMemory(m_lUserWinScore, sizeof(m_lUserWinScore));
	ZeroMemory(m_lUserReturnScore, sizeof(m_lUserReturnScore));
	ZeroMemory(m_lUserRevenue, sizeof(m_lUserRevenue));
	LONGLONG lUserLostScore[GAME_PLAYER];
	ZeroMemory(lUserLostScore, sizeof(lUserLostScore));


	//胜利标识
	bool static bWinFlag[AREA_ARRY_COUNT];
	bWinFlag[0]=false;
	bWinFlag[ID_SHUN_MEN]=bWinShunMen;
	bWinFlag[ID_DUI_MEN]=bWinDuiMen;
	bWinFlag[ID_DAO_MEN]=bWinDaoMen;
	bWinFlag[ID_JIAO_L]= false;
	bWinFlag[ID_QIAO]= false;
	bWinFlag[ID_JIAO_R]= false;

	//角标识
	bool static bWinBankerJiaoL,bWinBankerJiaoR,bWinBankerQiao;
	bWinBankerJiaoL= false;
	bWinBankerJiaoR= false;
	bWinBankerQiao= false;


	//计算积分
	for (WORD wChairID=0; wChairID<GAME_PLAYER; wChairID++)
	{
		//庄家判断
		if (m_wCurrentBanker==wChairID) continue;

		//获取用户
		IServerUserItem * pIServerUserItem=m_pITableFrame->GetTableUserItem(wChairID);
		if (pIServerUserItem == NULL  && lUserScore[wChairID] == 0) continue;
		bool bUserLeft = (pIServerUserItem==NULL);

		for (WORD wAreaIndex = ID_SHUN_MEN; wAreaIndex <= ID_JIAO_R; ++wAreaIndex)
		{
			if (!bUserLeft)
			{
				//角判断
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
			//玩家逃跑
			else if(!bBankerLeft)
			{
				lUserLostScore[wChairID] -= m_lUserJettonScore[wAreaIndex][wChairID];
				lBankerWinScore += m_lUserJettonScore[wAreaIndex][wChairID];
			}
		}
		//计算税收
		if (0 < m_lUserWinScore[wChairID])
		{
			double fRevenuePer=double(cbRevenue/1000.);
			m_lUserRevenue[wChairID]  = LONGLONG(m_lUserWinScore[wChairID]*fRevenuePer);
			m_lUserWinScore[wChairID] -= m_lUserRevenue[wChairID];
		}
		//总的分数
		m_lUserWinScore[wChairID] += lUserLostScore[wChairID];
	}

	//庄家成绩
	if (m_wCurrentBanker!=INVALID_CHAIR)
	{
		m_lUserWinScore[m_wCurrentBanker] = lBankerWinScore;

		//计算税收
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

	//累计积分
	m_lBankerWinScore += lBankerWinScore;

	//当前积分
	m_lBankerCurGameScore=lBankerWinScore;

	return lBankerWinScore;
}

VOID CTableFrameSink::DeduceWinner(bool &bWinShunMen, bool &bWinDuiMen, bool &bWinDaoMen)
{
	//大小比较
	bWinShunMen=m_GameLogic.CompareCard(m_cbTableCardArray[BANKER_INDEX],2,m_cbTableCardArray[SHUN_MEN_INDEX],2)==1?true:false;
	bWinDuiMen=m_GameLogic.CompareCard(m_cbTableCardArray[BANKER_INDEX],2,m_cbTableCardArray[DUI_MEN_INDEX],2)==1?true:false;
	bWinDaoMen=m_GameLogic.CompareCard(m_cbTableCardArray[BANKER_INDEX],2,m_cbTableCardArray[DAO_MEN_INDEX],2)==1?true:false;
}

//试探性判断
bool CTableFrameSink::ProbeJudge(LONGLONG& lSystemScore)
{
	//推断玩家
	bool bWinShunMen, bWinDuiMen, bWinDaoMen;
	DeduceWinner(bWinShunMen, bWinDuiMen, bWinDaoMen);


	//胜利标识
	bool bWinFlag[AREA_ARRY_COUNT];
	bWinFlag[0]=false;
	bWinFlag[ID_SHUN_MEN]=bWinShunMen;
	bWinFlag[ID_DUI_MEN]=bWinDuiMen;
	bWinFlag[ID_DAO_MEN]=bWinDaoMen;
	bWinFlag[ID_JIAO_L]= false;
	bWinFlag[ID_QIAO]= false;
	bWinFlag[ID_JIAO_R]= false;

	//监控触发
	if(m_cbControlID!=INIT_CTRL&&m_cbControlID>=0&&m_cbControlID<=7)
	{
		bool isReturn = true;

		//上门赢
		if(bWinFlag[ID_SHUN_MEN]&&!bWinFlag[ID_DUI_MEN]&&!bWinFlag[ID_DAO_MEN]&&m_cbControlID==0) isReturn=false;
		//天门赢
		if(bWinFlag[ID_DUI_MEN]&&!bWinFlag[ID_SHUN_MEN]&&!bWinFlag[ID_DAO_MEN]&&m_cbControlID==1) isReturn=false;
		//下门赢
		if(bWinFlag[ID_DAO_MEN]&&!bWinFlag[ID_SHUN_MEN]&&!bWinFlag[ID_DUI_MEN]&&m_cbControlID==2) isReturn=false;
		//通杀
		if(!bWinFlag[ID_SHUN_MEN]&&!bWinFlag[ID_DUI_MEN]&&!bWinFlag[ID_DAO_MEN]&&m_cbControlID==3) isReturn=false;
		//通赔
		if(bWinFlag[ID_SHUN_MEN]&&bWinFlag[ID_DUI_MEN]&&bWinFlag[ID_DAO_MEN]&&m_cbControlID==4) isReturn=false;
		//上门输
		if(!bWinFlag[ID_SHUN_MEN]&&bWinFlag[ID_DUI_MEN]&&bWinFlag[ID_DAO_MEN]&&m_cbControlID==5) isReturn=false;
		//天门输
		if(!bWinFlag[ID_DUI_MEN]&&bWinFlag[ID_SHUN_MEN]&&bWinFlag[ID_DAO_MEN]&&m_cbControlID==6) isReturn=false;
		//上门输
		if(!bWinFlag[ID_DAO_MEN]&&bWinFlag[ID_SHUN_MEN]&&bWinFlag[ID_DUI_MEN]&&m_cbControlID==7) isReturn=false;

		if(isReturn) return false;

		return true;
	}

	//系统输赢
	lSystemScore = 0l;

	//庄家是不是机器人
	bool bIsBankerAndroidUser = false;
	if ( m_wCurrentBanker != INVALID_CHAIR )
	{
		IServerUserItem * pIBankerUserItem = m_pITableFrame->GetTableUserItem(m_wCurrentBanker);
		if (pIBankerUserItem != NULL) 
		{
			bIsBankerAndroidUser = pIBankerUserItem->IsAndroidUser();
		}
	}

	//点数
	BYTE bAreaPoint[7] = {0};
	memset(bAreaPoint, 0, sizeof(bAreaPoint));
	BYTE bBankerPoint = m_GameLogic.GetCardListPip(m_cbTableCardArray[BANKER_INDEX],2);
	bAreaPoint[ID_SHUN_MEN] = m_GameLogic.GetCardListPip(m_cbTableCardArray[SHUN_MEN_INDEX],2);
	bAreaPoint[ID_DUI_MEN] = m_GameLogic.GetCardListPip(m_cbTableCardArray[DUI_MEN_INDEX],2);
	bAreaPoint[ID_DAO_MEN] = m_GameLogic.GetCardListPip(m_cbTableCardArray[DAO_MEN_INDEX],2);

	//庄家金币
	LONGLONG lBankerScore = 0;
	if(m_wCurrentBanker != INVALID_CHAIR)
	{
		IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(m_wCurrentBanker);
		if(pIServerUserItem)
			lBankerScore = pIServerUserItem->GetUserScore();
	}

	//计算闲家押注
	LONGLONG lUserScore[GAME_PLAYER] = {0};
	LONGLONG lAllUserScore = 0L;
	for (WORD wChairID=0; wChairID<GAME_PLAYER; wChairID++)
	{
		//庄家判断
		if (m_wCurrentBanker==wChairID) continue;

		//计算用户下分
		lUserScore[wChairID] = 0L;
		for ( WORD wAreaIndex = ID_SHUN_MEN; wAreaIndex <= ID_JIAO_R; ++wAreaIndex ) lUserScore[wChairID] += m_lUserJettonScore[wAreaIndex][wChairID];

		lAllUserScore += lUserScore[wChairID];
	}

	//是否达到上限
	bool bToBankerMax = false;
	if(lBankerScore > 0 && lAllUserScore >= (lBankerScore*50/100)) bToBankerMax = true;

	//计算积分
	LONGLONG lBankerWinScore = 0L;
	for (WORD wChairID=0; wChairID<GAME_PLAYER; wChairID++)
	{
		//庄家判断
		if (m_wCurrentBanker==wChairID) continue;

		//获取用户
		IServerUserItem * pIServerUserItem=m_pITableFrame->GetTableUserItem(wChairID);
		if (pIServerUserItem == NULL) continue;

		//
		bool bIsAndroidUser = pIServerUserItem->IsAndroidUser();

		for (WORD wAreaIndex = ID_SHUN_MEN; wAreaIndex <= ID_JIAO_R; ++wAreaIndex)
		{
			//角判断
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

	//庄家扣分计算
	if(lBankerWinScore < 0L && (lBankerScore+lBankerWinScore) < 0L) return false;

	//系统赢
	if(lSystemScore>=0) return true;

	//系统分值计算
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

//发送记录
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

//发送消息
VOID CTableFrameSink::SendGameMessage(WORD wChairID, LPCTSTR pszTipMsg)
{
	
	if (wChairID==INVALID_CHAIR)
	{
		//游戏玩家
		for (WORD i=0; i<GAME_PLAYER; ++i)
		{
			IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(i);
			if (pIServerUserItem==NULL) continue;
			m_pITableFrame->SendGameMessage(pIServerUserItem,pszTipMsg,SMT_CHAT);
		}

		//旁观玩家
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

//操作判断
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

//上庄控制 (机器人)
VOID CTableFrameSink::RobotBankerControl()
{
	//是否添加上庄机器人
	if (m_ApplyUserArray.GetCount()-1 > m_nRobotBankerApplayMaxCount)
		return;

	for (int i = 0; i < GAME_PLAYER; i++)
	{
		IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(i);

		if (pIServerUserItem != NULL && pIServerUserItem->IsAndroidUser())
		{

			if ( m_wCurrentBanker != i && pIServerUserItem->GetUserScore() >= m_lApplyBankerCondition)
			{
				//检测上庄列表
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

//选择庄家
bool CTableFrameSink::TrySelectBanker()
{
	if(m_wCurrentBanker==INVALID_CHAIR&&m_ApplyUserArray.GetCount()==0) 
		return false;

	//参入人数最少2人
	int nUserCount = 0;
	for (int i=0;i<GAME_PLAYER;i++)
	{
		if(m_pITableFrame->GetTableUserItem(i)!=NULL) nUserCount++;
	}
	if(nUserCount==0) return false;

	if (m_wCurrentBanker==INVALID_CHAIR&&m_ApplyUserArray.GetCount()>0)
	{
		m_wCurrentBanker = m_ApplyUserArray[0];

		//切换判断
		{
			//设置变量
			m_wBankerTime = 0;
			m_lBankerWinScore=0;

			//发送消息
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
				//读取消息
				LONGLONG lMessageCount=GetPrivateProfileInt(m_szGameRoomName,TEXT("MessageCount"),0,m_szConfigFileName);
				if (lMessageCount!=0)
				{
					//读取配置
					LONGLONG lIndex=rand()%lMessageCount;
					TCHAR szKeyName[32],szMessage1[256],szMessage2[256];				
					_sntprintf(szKeyName,CountArray(szKeyName),TEXT("Item%I64d"),lIndex);
					GetPrivateProfileString(m_szGameRoomName,szKeyName,TEXT("恭喜[ %s ]上庄"),szMessage1,CountArray(szMessage1),m_szConfigFileName);

					//获取玩家
					IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(m_wCurrentBanker);

					//发送消息
					_sntprintf(szMessage2,CountArray(szMessage2),szMessage1,pIServerUserItem->GetNickName());
					SendGameMessage(INVALID_CHAIR,szMessage2);
				}
			}
		}
	}

	return true;
}

//////////////////////////////////////////监控事件///////////////////////////////
bool CTableFrameSink::OnUserControlEvent(IServerUserItem* pServerUserItem,CMD_GC_ControlInfo* pCtrlData)
{
	if(pServerUserItem == NULL) return false;

	//执行时间检查
	if(m_pITableFrame->GetGameStatus()!=GS_PLACE_JETTON)
	{
		CMD_GC_CtrlNotice NoticeInfo;
		NoticeInfo.cbNoticeID = 0;
		NoticeInfo.wTableID = m_pITableFrame->GetTableID();

		m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_GC_CTRL_NOTICE,&NoticeInfo,sizeof(CMD_GC_CtrlNotice),MDM_GC_CONTROL);

		return true;
	}

	//执行时间检查
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

	//庄闲
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

	//发送押注信息
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