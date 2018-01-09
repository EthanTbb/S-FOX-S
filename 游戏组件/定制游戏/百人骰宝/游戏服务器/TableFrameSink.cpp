#include "StdAfx.h"
#include "DlgCustomRule.h"
#include "TableFrameSink.h"
#include <locale>

//////////////////////////////////////////////////////////////////////////

//常量定义
#define SEND_COUNT					100									//发送次数

#define IDI_FREE					1									//空闲时间
#define IDI_PLAYER_BET				3									//下注时间
#define IDI_GAME_END				4									//结束时间


#define TIME_FREE					10									//空闲时间
#define TIME_PLACE_JETTON			15									//下注时间
#define TIME_GAME_END				18									//结束时间

//////////////////////////////////////////////////////////////////////////

//静态变量
const WORD			CTableFrameSink::m_wPlayerCount=GAME_PLAYER;				//游戏人数

//////////////////////////////////////////////////////////////////////////

//构造函数
CTableFrameSink::CTableFrameSink()
{
	//总下注数
	ZeroMemory(m_lAllUserBet,sizeof(m_lAllUserBet));
	//起始分数
	ZeroMemory(m_lUserStartScore,sizeof(m_lUserStartScore));
	
	//个人下注
	ZeroMemory(m_lUserAllBet,sizeof(m_lUserAllBet));
	
	//机器人下注
	ZeroMemory(m_lAndroidBet,sizeof(m_lAndroidBet));

	//控制变量
	m_cbWinSideControl=0;
	m_nSendCardCount=0;
	m_cbControlTotal = 0;
	m_cbReqType = 0;

	//玩家成绩	
	ZeroMemory(m_lUserRevenue,sizeof(m_lUserRevenue));
	ZeroMemory(m_lUserWinScore,sizeof(m_lUserWinScore));
	ZeroMemory(m_lUserReturnScore,sizeof(m_lUserReturnScore));
	m_cbControlDiceValue[0]=1;
	m_cbControlDiceValue[1]=1;
	m_cbControlDiceValue[2]=1;

	//扑克信息
	ZeroMemory(m_cbDiceValue,sizeof(m_cbDiceValue));

	m_cbControlStyle = 0;	
	m_cbWinAreaCount = 0;
	m_lStorageCurrent=0;
	m_lCurSystemWinScore=0;
	ZeroMemory(m_bWinArea, sizeof(m_bWinArea));
	m_lQueryGameID = -1;
	m_cbUserWinLose = 0;
	m_lControlStorage=0;
	CopyMemory(m_szControlName,TEXT("无人"),sizeof(m_szControlName));

	//状态变量
	m_dwStartTime=0L;

	//庄家信息
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

	//机器人控制
	m_nChipRobotCount = 0;
	m_nRobotListMaxCount =0;
	ZeroMemory(m_lRobotAreaScore, sizeof(m_lRobotAreaScore));

	//机器人控制
	m_nChipRobotCount = 0;
	ZeroMemory(m_bRobotChip, sizeof(m_bRobotChip));
	
	//记录变量
	ZeroMemory(m_GameRecordList,sizeof(m_GameRecordList));
	m_nRecordFirst=0;
	m_nRecordLast=0;
	m_dwRecordCount=0;

	m_cbNeedControl=false;
	m_bControlWin = false;

	srand((unsigned)time(NULL));   

	return;
}

//析构函数
CTableFrameSink::~CTableFrameSink(VOID)
{
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
	

	////控制接口
	//m_pITableFrameControl=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,ITableFrameControl);
	//if (m_pITableFrameControl==NULL) return false;

	//查询接口
	ASSERT(pIUnknownEx!=NULL);
	m_pITableFrame=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,ITableFrame);
	if (m_pITableFrame==NULL) return false;

	//开始模式
	m_pITableFrame->SetStartMode(START_MODE_TIME_CONTROL);


	//获取参数
	m_pGameServiceOption=m_pITableFrame->GetGameServiceOption();
	ASSERT(m_pGameServiceOption!=NULL);

	ReadConfigInformation();	

	return true;
}

//复位桌子
VOID  CTableFrameSink::RepositionSink()
{
	//总下注数
	ZeroMemory(m_lAllUserBet,sizeof(m_lAllUserBet));
	
	//个人下注
	ZeroMemory(m_lUserAllBet,sizeof(m_lUserAllBet));

	//玩家成绩	
	ZeroMemory(m_lUserWinScore,sizeof(m_lUserWinScore));
	ZeroMemory(m_lUserReturnScore,sizeof(m_lUserReturnScore));
	ZeroMemory(m_lUserRevenue,sizeof(m_lUserRevenue));

	//机器人控制
	m_nChipRobotCount = 0;
	ZeroMemory(m_bRobotChip, sizeof(m_bRobotChip));
	m_lCurSystemWinScore=0;
	ZeroMemory(m_lRobotAreaScore, sizeof(m_lRobotAreaScore));
	m_lControlStorage=0;

	return;
}

//游戏状态
bool  CTableFrameSink::IsUserPlaying(WORD wChairID)
{
	return true;
}

//游戏开始
bool  CTableFrameSink::OnEventGameStart()
{
	//特殊账户发送库存值
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		//获取用户
		IServerUserItem * pIServerUserItem=m_pITableFrame->GetTableUserItem(i);
		if ( pIServerUserItem == NULL )
			continue;

		if( !CUserRight::IsGameCheatUser(pIServerUserItem->GetUserRight())) 
			continue;

			CString strInfo;
			strInfo.Format(TEXT("当前库存：%I64d"), m_lStorageCurrent);

			m_pITableFrame->SendGameMessage(pIServerUserItem,strInfo,SMT_CHAT);
			
	}

	CString strStorage;
	CTime Time(CTime::GetCurrentTime());
	strStorage.Format(TEXT(" 房间: %s | 时间: %d-%d-%d %d:%d:%d | 库存: %I64d \n"), m_pGameServiceOption->szServerName, Time.GetYear(), Time.GetMonth(), Time.GetDay(), Time.GetHour(), Time.GetMinute(), Time.GetSecond(), m_lStorageCurrent );
	WriteInfo(TEXT("百人骰宝库存信息.log"),strStorage);

	//变量定义
	CMD_S_GameStart GameStart;
	ZeroMemory(&GameStart,sizeof(GameStart));
	
	m_lBankerScore=(m_wCurrentBanker==INVALID_CHAIR)?210000000:m_pITableFrame->GetTableUserItem(m_wCurrentBanker)->GetUserScore();
	
	//派发扑克
	/*for(BYTE cbIndex=0;cbIndex<DICE_COUNT;cbIndex++)
	{
		m_cbDiceValue[cbIndex]=rand()%6+1;
	}*/

	//获取庄家
	IServerUserItem *pIBankerServerUserItem=NULL;
	if (INVALID_CHAIR!=m_wCurrentBanker) pIBankerServerUserItem=m_pITableFrame->GetTableUserItem(m_wCurrentBanker);

	
	//设置变量
	GameStart.cbTimeLeave=(BYTE)m_dwTimeGameBet;
	GameStart.wBankerUser=m_wCurrentBanker;

	if (pIBankerServerUserItem!=NULL) 
		GameStart.lBankerScore=m_lBankerScore;

	//输赢信息
	memcpy(GameStart.bWinFlag, m_bWinFlag, sizeof(m_bWinFlag));
		
	//下注机器人数量
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

	//机器人控制
	m_nChipRobotCount = 0;
	ZeroMemory(m_bRobotChip, sizeof(m_bRobotChip));
	ZeroMemory(m_lRobotAreaScore, sizeof(m_lRobotAreaScore));
	
	//旁观玩家
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_START,&GameStart,sizeof(GameStart));	

	//游戏玩家
	for (WORD wChairID=0; wChairID<GAME_PLAYER; ++wChairID)
	{
		IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(wChairID);
		if (pIServerUserItem==NULL) continue;
		//设置积分
		GameStart.lUserMaxScore=m_lUserLimitScore;
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
			//控制
			if (m_cbNeedControl)
			{
				memcpy(m_cbDiceValue,m_cbControlDiceValue,sizeof(BYTE)*DICE_COUNT);				

				//作弊信息
				for (WORD i=0;i<m_wPlayerCount;i++)
				{
					//获取用户
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
							//重新出签
							DispatchTableLabel();
							//试探判断
							cbSuccess = ControlUserWin();

						} while (m_cbUserWinLose!=cbSuccess);

						//作弊信息
						for (WORD i=0;i<m_wPlayerCount;i++)
						{
							//获取用户
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
					int n = 0;//防止控制出现负分后出现下注不够补库存时死循环
					do 
					{
						//重新出签
						DispatchTableLabel();
						//试探判断
						bSuccess = ProbeJudge();

					} while (!bSuccess && ++n < 65534);
				}							
			}			
			
			//计算分数
			//SCORE AllScore=0;
			//bool     bWinFlags[AREA_COUNT]={0};
			SCORE lBankerWinScore=CalculateScore(false);

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
			memcpy(GameEnd.cbDiceValue,m_cbDiceValue,sizeof(m_cbDiceValue));
			
			//扑克信息
			CopyMemory(GameEnd.cbDiceValue,m_cbDiceValue,sizeof(m_cbDiceValue));
			GameEnd.cbLeftCardCount=m_cbLeftCardCount;

			
			//发送积分
			GameEnd.cbTimeLeave=(BYTE)m_dwTimeGameEnd;	
			for ( WORD wUserIndex = 0; wUserIndex < GAME_PLAYER; ++wUserIndex )
			{
				IServerUserItem *pIServerUserItem = m_pITableFrame->GetTableUserItem(wUserIndex);
				if ( pIServerUserItem == NULL ) continue;

				//设置成绩
				GameEnd.lUserScore=m_lUserWinScore[wUserIndex];
				//返还积分
				GameEnd.lUserReturnScore=m_lUserReturnScore[wUserIndex];
				//设置税收
				if (m_lUserRevenue[wUserIndex]>0) 
					GameEnd.lRevenue=m_lUserRevenue[wUserIndex];
				else 
					if (m_wCurrentBanker!=INVALID_CHAIR) 
						GameEnd.lRevenue=m_lUserRevenue[m_wCurrentBanker];
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
			//闲家逃跑
			if (m_wCurrentBanker!=wChairID)
			{
				return PlayerFlee(wChairID,pIServerUserItem);
			}
			else
			{
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
					m_pITableFrame->KillGameTimer(IDI_PLAYER_BET);
					m_pITableFrame->SetGameTimer(IDI_GAME_END,m_dwTimeGameEnd*1000,1,0L);

					bool bSuccess = false;
					m_lCurSystemWinScore = 0L;	
					
					int n = 0;//防止控制出现负分后出现下注不够补库存时死循环
					do 
					{
						//重新出签
						DispatchTableLabel();
						//试探判断
						bSuccess = ProbeJudge();

					} while (!bSuccess && ++n < 65534);


					//计算分数
					SCORE lBankerWinScore=CalculateScore(false);

					//结束消息
					CMD_S_GameEnd GameEnd;
					ZeroMemory(&GameEnd,sizeof(GameEnd));

					//庄家信息
					GameEnd.nBankerTime = m_wBankerTime;
					GameEnd.lBankerTotallScore=m_lBankerWinScore;
					GameEnd.lBankerScore=lBankerWinScore;

					//扑克信息
					CopyMemory(GameEnd.cbDiceValue,m_cbDiceValue,sizeof(m_cbDiceValue));
					GameEnd.cbLeftCardCount=m_cbLeftCardCount;

					//发送积分
					GameEnd.cbTimeLeave=(BYTE)m_dwTimeGameEnd;	
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

				//切换庄家
				ChangeBanker(true);
			}
			return true;
		}
	}
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
				StatusFree.lBankerScore=m_lBankerScore;
			}

			//玩家信息
			if (pIServerUserItem->GetUserStatus()!=US_LOOKON)
			{
				StatusFree.lUserMaxScore=m_lUserLimitScore; 
			}

			//全局信息
			DWORD dwPassTime=(DWORD)time(NULL)-m_dwStartTime;
			StatusFree.cbTimeLeave=(BYTE)(m_dwTimeGameFree-__min(dwPassTime,m_dwTimeGameFree));

			//房间名称
			CopyMemory(StatusFree.szGameRoomName, m_strAppName.GetBuffer(), sizeof(StatusFree.szGameRoomName));

			//机器人配置
			if(pIServerUserItem->IsAndroidUser())
			{
				tagCustomConfig *pCustomConfig = (tagCustomConfig *)m_pGameServiceOption->cbCustomRule;
				ASSERT(pCustomConfig);

				CopyMemory(&StatusFree.CustomAndroid, &pCustomConfig->CustomAndroid, sizeof(tagCustomAndroid));
			}

			//发送场景
			bool bSuccess = m_pITableFrame->SendGameScene(pIServerUserItem,&StatusFree,sizeof(StatusFree));

			//限制提示
			TCHAR szTipMsg[128];
			_sntprintf(szTipMsg,CountArray(szTipMsg),_T("本房间上庄条件为:%I64d,区域限制为:%I64d,玩家限制为:%I64d"),m_lApplyBankerCondition,m_lAreaLimitScore,m_lUserLimitScore);
			m_pITableFrame->SendGameMessage(pIServerUserItem,szTipMsg,SMT_CHAT);		
			//发送申请者
			SendApplyUser(pIServerUserItem);
			return bSuccess;
		}
	case GS_GAME_END:			//结束状态
	case GS_PLAYER_BET:			//游戏状态
	case GS_GAME_START:			//游戏启动
		{
			//发送记录
			SendGameRecord(pIServerUserItem);		

			//构造数据
			CMD_S_StatusPlay StatusPlay={0};

			//全局下注
			CopyMemory(StatusPlay.lAllJettonScore,m_lAllUserBet,sizeof(StatusPlay.lAllJettonScore));

			//玩家下注
			if (pIServerUserItem->GetUserStatus()!=US_LOOKON)
			{
				for (int nAreaIndex=0; nAreaIndex<AREA_COUNT; ++nAreaIndex)
					StatusPlay.lUserJettonScore[nAreaIndex] = m_lUserAllBet[nAreaIndex][wChiarID];

				//最大下注
				StatusPlay.lUserMaxScore=m_lUserLimitScore;
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
				StatusPlay.lBankerScore=m_lBankerScore;
			}	

			//全局信息
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

				//结束判断
			if (cbGameStatus==GS_GAME_END)
			{
				CopyMemory(StatusPlay.cbDiceValue,m_cbDiceValue,sizeof(m_cbDiceValue));
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
			}
			
			//房间名称
			CopyMemory(StatusPlay.szGameRoomName, m_strAppName.GetBuffer(), sizeof(StatusPlay.szGameRoomName));

			//机器人配置
			if(pIServerUserItem->IsAndroidUser())
			{
				tagCustomConfig *pCustomConfig = (tagCustomConfig *)m_pGameServiceOption->cbCustomRule;
				ASSERT(pCustomConfig);

				CopyMemory(&StatusPlay.CustomAndroid, &pCustomConfig->CustomAndroid, sizeof(tagCustomAndroid));
			}
		
			m_pITableFrame->SendGameScene(pIServerUserItem,&StatusPlay,sizeof(StatusPlay));
			//发送申请者
			SendApplyUser( pIServerUserItem );
			//限制提示
			TCHAR szTipMsg[138];
			_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("本房间上庄条件为:%I64d,区域限制为:%I64d,玩家限制为:%I64d"),m_lApplyBankerCondition,m_lAreaLimitScore,m_lUserLimitScore);
			m_pITableFrame->SendGameMessage(pIServerUserItem,szTipMsg,SMT_CHAT);

			//发送玩家下注信息
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
	//配置控制
	//RobotConfigControl();
	//检测上庄列表
	//CheckBankerList();
	//发送下注消息
	//m_pITableFrameControl->StartGame();
	m_pITableFrame->StartGame();

	//设置时间
	m_pITableFrame->SetGameTimer(IDI_PLAYER_BET,m_dwTimeGameBet*1000,1,0L);

	//设置状态
	m_pITableFrame->SetGameStatus(GS_PLAYER_BET);

	CMD_S_StartBet startBet;
	startBet.cbTimeLeave=(BYTE)m_dwTimeGameBet;
	
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_START_BET,&startBet,sizeof(startBet));
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_START_BET,&startBet,sizeof(startBet));
	
	//阶段时间
	m_dwStartTime=(DWORD)time(NULL);

	return true;
}

bool CTableFrameSink::OnTimerShowResult()
{
	//状态判断(防止强退重复设置)
	if (m_pITableFrame->GetGameStatus()!=GS_GAME_END)
	{
		//设置状态
		m_pITableFrame->SetGameStatus(GS_GAME_END);			
		//结束游戏
		OnEventGameConclude(INVALID_CHAIR,NULL,GER_NORMAL);
		//设置时间
		m_pITableFrame->SetGameTimer(IDI_GAME_END,m_dwTimeGameEnd*1000,1,0L);
		//阶段时间
		m_dwStartTime=(DWORD)time(NULL);
	}

	return true;
}

bool CTableFrameSink::OnTimerGameEnd()
{
	
	tagScoreInfo ScoreInfo[GAME_PLAYER];
	ZeroMemory(ScoreInfo,sizeof(ScoreInfo));
	LONGLONG lStorage=0;
	
	//写入积分
	for ( WORD wUserChairID = 0; wUserChairID < GAME_PLAYER; ++wUserChairID )
	{
		IServerUserItem *pIServerUserItem = m_pITableFrame->GetTableUserItem(wUserChairID);
		if ( pIServerUserItem == NULL ) continue;
		//胜利类型
		//enScoreKind ScoreKind=(m_lUserWinScore[wUserChairID]>0L)?enScoreKind_Win:enScoreKind_Lost;
		//写入积分
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
		cs.Format(TEXT("当局损耗为：%I64d，账号为：%s"),m_lControlStorage,m_szControlName);
		CTraceService::TraceString(cs,TraceLevel_Exception);					
	}
	m_cbNeedControl=false;
	m_bControlWin = false;

	m_pITableFrame->WriteTableScore(ScoreInfo,CountArray(ScoreInfo));

	//库存
	/*for ( WORD wUserChairID = 0; wUserChairID < GAME_PLAYER; ++wUserChairID )
	{
		IServerUserItem *pIServerUserItem = m_pITableFrame->GetTableUserItem(wUserChairID);
		if ( pIServerUserItem == NULL ) continue;
		if(!pIServerUserItem->IsAndroidUser())
		{
			m_lStorageCurrent-=ScoreInfo[wUserChairID].lScore;
		}
	}*/

	//结束游戏
	m_pITableFrame->ConcludeGame(GAME_STATUS_FREE);
	//切换庄家
	ChangeBanker(false);
	
	//设置时间
	m_dwStartTime=(DWORD)time(NULL);
	m_pITableFrame->SetGameTimer(IDI_FREE,m_dwTimeGameFree*1000,1,0L);

	//发送消息
	CMD_S_GameFree GameFree;
	ZeroMemory(&GameFree,sizeof(GameFree));
	GameFree.cbTimeLeave=(BYTE)m_dwTimeGameFree;
	GameFree.nListUserCount=m_ApplyUserArray.GetCount()-1;
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_FREE,&GameFree,sizeof(GameFree));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_FREE,&GameFree,sizeof(GameFree));

	//机器人上庄控制
	//RobotBankerControl();

	return true;
}

//定时器事件
bool  CTableFrameSink::OnTimerMessage(DWORD wTimerID, WPARAM wBindParam)
{
	switch (wTimerID)
	{
		case IDI_FREE:		//空闲时间
			{
				return OnTimerStartBet();
			}
		case IDI_PLAYER_BET:		//下注时间
			{
				return OnTimerShowResult();
			}
		case IDI_GAME_END:			//结束游戏
			{
				return OnTimerGameEnd();
			}
		default:break;
	}

	return false;
}

//游戏消息处理
bool  CTableFrameSink::OnGameMessage(WORD wSubCmdID,  VOID * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem)
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
			return OnUserPlaceJetton(pUserData->wChairID,pPlaceJetton->cbJettonArea,pPlaceJetton->lJettonScore);
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

			//权限判断
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

				//记录信息
				CString strControlInfo, str;
				str.Format(TEXT("%s"), TEXT("修改库存设置！"));
				CTime Time(CTime::GetCurrentTime());
				strControlInfo.Format(TEXT("房间: %s | 桌号: %u | 时间: %d-%d-%d %d:%d:%d\n控制人账号: %s | 控制人ID: %u\n%s\r\n"),
					m_pGameServiceOption->szServerName, m_pITableFrame->GetTableID()+1, Time.GetYear(), Time.GetMonth(), Time.GetDay(),
					Time.GetHour(), Time.GetMinute(), Time.GetSecond(), pIServerUserItem->GetNickName(), pIServerUserItem->GetGameID(), str);

				WriteInfo(TEXT("百人骰宝控制信息.log"),strControlInfo);
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

			//权限判断
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

//框架消息处理
bool  CTableFrameSink::OnFrameMessage(WORD wSubCmdID,  VOID * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	return false;
}


//数据事件
bool CTableFrameSink::OnGameDataBase(WORD wRequestID, VOID * pData, WORD wDataSize)
{
	return false;
}

//加注事件
bool CTableFrameSink::OnUserPlaceJetton(WORD wChairID, BYTE cbJettonArea, SCORE lJettonScore)
{
	//效验参数
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
		//发送消息
		m_pITableFrame->SendTableData(wChairID,SUB_S_PLACE_JETTON_FAIL,&PlaceJettonFail,sizeof(PlaceJettonFail));
		return true;
	}

	//庄家判断
	if (m_wCurrentBanker==wChairID)	return true;
	if (m_bEnableSysBanker==false && m_wCurrentBanker==INVALID_CHAIR) return true;

	//变量定义
	IServerUserItem * pIServerUserItem = m_pITableFrame->GetTableUserItem(wChairID);

	//机器人验证
	if(pIServerUserItem != NULL && pIServerUserItem->IsAndroidUser())
	{
		//区域限制
		if (m_lRobotAreaScore[cbJettonArea] + lJettonScore > m_lRobotAreaLimit)
			return true;
		//数目限制
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

	//庄家积分
	LONGLONG lBankerScore = 0;
	if(INVALID_CHAIR != m_wCurrentBanker)
	{
		IServerUserItem * pIServerUserItemBanker = m_pITableFrame->GetTableUserItem(m_wCurrentBanker);
		lBankerScore = pIServerUserItemBanker->GetUserScore();
	}
	LONGLONG lAllJettonCount=0L;
	for (int nAreaIndex=0; nAreaIndex<AREA_COUNT; ++nAreaIndex) lAllJettonCount += m_lAllUserBet[nAreaIndex];
	
	//合法验证
	if (GetUserMaxJetton(wChairID,cbJettonArea) >= lJettonScore)
	{
		//保存下注
		m_lAllUserBet[cbJettonArea] += lJettonScore;
		m_lUserAllBet[cbJettonArea][wChairID] += lJettonScore;
		m_lRobotAreaScore[cbJettonArea] += lJettonScore;

		//变量定义
		CMD_S_PlaceJetton PlaceJetton;
		ZeroMemory(&PlaceJetton,sizeof(PlaceJetton));

		//构造变量
		PlaceJetton.wChairID=wChairID;
		PlaceJetton.cbJettonArea=cbJettonArea;
		PlaceJetton.lJettonScore=lJettonScore;
		PlaceJetton.bIsAndroid=pIServerUserItem->IsAndroidUser();

		//发送消息
		m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_PLACE_JETTON,&PlaceJetton,sizeof(PlaceJetton));
		m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_PLACE_JETTON,&PlaceJetton,sizeof(PlaceJetton));

		//发送玩家下注信息
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

// 		//下注总额超过庄家能支付上限时提前结束游戏
// 		if(INVALID_CHAIR != m_wCurrentBanker)
// 		{
// 			LONGLONG lBankerCanUseScore = lBankerScore*m_nEndGameMul/100;
// 			if(lBankerCanUseScore <= (lAllJettonCount + lJettonScore))
// 			{
// 				if (m_pITableFrame->GetGameStatus()!=GS_GAME_END)
// 				{
// 					//设置状态
// 					m_pITableFrame->SetGameStatus(GS_GAME_END);			
// 
// 					//结束游戏
// 					OnEventGameConclude(INVALID_CHAIR,NULL,GER_NORMAL);
// 
// 					//设置时间
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

		//发送消息
		m_pITableFrame->SendTableData(wChairID,SUB_S_PLACE_JETTON_FAIL,&PlaceJettonFail,sizeof(PlaceJettonFail));

	}

	return true;
}


//申请庄家
bool CTableFrameSink::OnUserApplyBanker(IServerUserItem *pIApplyServerUserItem)
{
	
	//合法判断
	SCORE lUserScore=pIApplyServerUserItem->GetUserScore();
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

	if (pIApplyServerUserItem->IsAndroidUser()&&(m_ApplyUserArray.GetCount())>m_nRobotListMaxCount)
	{
		return true;
	}

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
		ChangeBanker(false);
	}

	return true;
}

bool CTableFrameSink::OnSubAmdinCommand(IServerUserItem*pIServerUserItem,const VOID*pDataBuffer)
{

	//如果不具有管理员权限 则返回错误
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

//取消申请
bool CTableFrameSink::OnUserCancelBanker(IServerUserItem *pICancelServerUserItem)
{
	//当前庄家
	if (pICancelServerUserItem->GetChairID()==m_wCurrentBanker && m_pITableFrame->GetGameStatus()!=GAME_STATUS_FREE)
	{
		//发送消息
		m_pITableFrame->SendGameMessage(pICancelServerUserItem,TEXT("游戏已经开始，不可以取消当庄！"),SMT_CHAT|SMT_EJECT);
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
			CancelBanker.wCancelUser = pIServerUserItem->GetChairID();

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

		return true;
	}

	return false;
}

//更换庄家
bool CTableFrameSink::ChangeBanker(bool bCancelCurrentBanker)
{
	//切换标识
	bool bChangeBanker=false;
	
	//做庄次数
	WORD wBankerTime=m_nBankerTimeLimit;

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

		//轮换判断
		if (m_pITableFrame->GetGameStatus()==GAME_STATUS_FREE &&FindSuitBanker())
		{
			m_wCurrentBanker=m_ApplyUserArray[0];
		}

		//设置变量
		bChangeBanker=true;
		m_bExchangeBanker = true;
		m_wAddTime=0;
	}
	//轮庄判断
	else if (m_wCurrentBanker!=INVALID_CHAIR)
	{
		//获取庄家
		IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(m_wCurrentBanker);
		SCORE lBankerScore=pIServerUserItem->GetUserScore();

		//次数判断
		if(pIServerUserItem!= NULL)
		{
			SCORE lBankerScore=pIServerUserItem->GetUserScore();

			//次数判断
			if (wBankerTime+m_wAddTime<=m_wBankerTime || lBankerScore<m_lApplyBankerCondition)
			{
				//庄家增加判断 同一个庄家情况下只判断一次
				if(wBankerTime <= m_wBankerTime && m_bExchangeBanker && lBankerScore >= m_lApplyBankerCondition)
				{
					//加庄局数设置：当庄家坐满设定的局数之后(m_wBankerTime)，
					//所带金币值还超过下面申请庄家列表里面所有玩家金币时，
					//可以再加坐庄m_lBankerAdd局，加庄局数可设置。

					//金币超过m_lExtraBankerScore之后，
					//就算是下面玩家的金币值大于他的金币值，他也可以再加庄m_lBankerScoreAdd局。
					bool bScoreMAX = true;
					m_bExchangeBanker = false;

					for (WORD i=0; i<m_ApplyUserArray.GetCount(); ++i)
					{
						//获取玩家
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

						//提示消息
						TCHAR szTipMsg[128] = {};
						if (bType == 1 )
							_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("[ %s ]的分数超过其他申请上庄玩家，获得%d次额外坐庄次数!"),pIServerUserItem->GetNickName(),m_wAddTime);
						else if (bType == 2)
							_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("[ %s ]的分数超过[%I64d]，获得%d次额外坐庄次数!"),pIServerUserItem->GetNickName(),m_lExtraBankerScore,m_wAddTime);
						else if (bType == 3)
							_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("[ %s ]的分数超过[%I64d]且超过其他申请玩家，获得%d次额外坐庄次数!"),pIServerUserItem->GetNickName(),m_lExtraBankerScore,m_wAddTime);
						else
							bType = 0;

						if (bType != 0)
						{
							//发送消息
							SendGameMessage(INVALID_CHAIR,szTipMsg);
							return true;
						}
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
				if (FindSuitBanker())
				{
					m_wCurrentBanker=m_ApplyUserArray[0];
				}

				bChangeBanker=true;

				//提示消息
				TCHAR szTipMsg[128];
				if (lBankerScore<m_lApplyBankerCondition)
					_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("[ %s ]分数少于(%I64d)，强行换庄!"),pIServerUserItem->GetNickName(),m_lApplyBankerCondition);
				else
					_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("[ %s ]做庄次数达到(%d)，强行换庄!"),pIServerUserItem->GetNickName(),wBankerTime+m_wAddTime);

				bChangeBanker=true;
				m_bExchangeBanker = true;
				m_wAddTime = 0;

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
		}

	}
	//系统做庄
	else if (m_wCurrentBanker==INVALID_CHAIR &&FindSuitBanker())
	{
		m_wCurrentBanker=m_ApplyUserArray[0];
		bChangeBanker=true;
		m_bExchangeBanker = true;
		m_wAddTime = 0;
	}

	//切换判断
	if (bChangeBanker)
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
			//获取参数
			m_pGameServiceOption=m_pITableFrame->GetGameServiceOption();
			ASSERT(m_pGameServiceOption!=NULL);

			//读取消息
			int nMessageCount = 3;

			//读取配置
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

			//获取玩家
			IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(m_wCurrentBanker);

			//发送消息
			_sntprintf(szMessage2,CountArray(szMessage2),TEXT("【 %s 】上庄了，%s"),pIServerUserItem->GetNickName(), szMessage1);
			SendGameMessage(INVALID_CHAIR,szMessage2);
		}
	}

	return bChangeBanker;
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

//平衡下注
void CTableFrameSink::BalanceCurBetScore(LONGLONG lAllUserBet[],int nCount)
{
	//大、小 对冲
	SCORE lMinScore=lAllUserBet[0]>lAllUserBet[1]?lAllUserBet[1]:lAllUserBet[0];
	lAllUserBet[0]-=lMinScore;
	lAllUserBet[1]-=lMinScore;

}

//最大下注
SCORE CTableFrameSink::GetUserMaxJetton(WORD wChairID,BYTE cbAreaID)
{
	//已下注额
	SCORE lNowJetton = 0;
	SCORE lCurUserMaxBet=0;
	for (int nAreaIndex=0; nAreaIndex<AREA_COUNT; ++nAreaIndex) 
		lNowJetton += m_lUserAllBet[nAreaIndex][wChairID];
	IServerUserItem *pIMeServerUserItem=m_pITableFrame->GetTableUserItem(wChairID);
	//可下金币
	lCurUserMaxBet=pIMeServerUserItem->GetUserScore()-lNowJetton;
	//下注限制
	lCurUserMaxBet=__min(lCurUserMaxBet,m_lUserLimitScore-lNowJetton);
	//区域限制
	lCurUserMaxBet=__min(lCurUserMaxBet,m_lAreaLimitScore-m_lAllUserBet[cbAreaID]);
	
	
	////庄家限制
	//{
	//	SCORE lBankerScore=2147483647;
	//	if(m_wCurrentBanker!=INVALID_CHAIR)
	//		lBankerScore=m_lBankerScore;

	//	SCORE lAllUserBet[AREA_COUNT];
	//	CopyMemory(lAllUserBet,m_lAllUserBet,sizeof(lAllUserBet));
	//	//对冲区域
	//	BalanceCurBetScore(lAllUserBet,AREA_COUNT);
	//	for (int nAreaIndex=0; nAreaIndex<AREA_COUNT; ++nAreaIndex) 
	//		lBankerScore-=lAllUserBet[nAreaIndex]*g_cbAreaOdds[nAreaIndex];
	//	lCurUserMaxBet=__min(lCurUserMaxBet,lBankerScore/g_cbAreaOdds[cbAreaID]);
	//}


	//庄家限制
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
	

	//零值过滤
	lCurUserMaxBet=__max(lCurUserMaxBet,0);
	return lCurUserMaxBet;
}


//庄家最小得分
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
	//变量定义
	CString strResult(TEXT(""));

	//大小王
	if(cbCardData==0x42) {strResult = TEXT("RJ");return strResult;}
	if(cbCardData==0x41) {strResult = TEXT("BJ");return strResult;}

	//花色,数值
	BYTE bCardColor = m_GameLogic.GetCardColor(cbCardData);
	BYTE bCardValue = m_GameLogic.GetCardValue(cbCardData);

	//转换花色
	switch( bCardColor )
	{
	case 0x00:		strResult = TEXT("D");		break; //方块
	case 0x01:		strResult = TEXT("C");		break; //梅花
	case 0x02:		strResult = TEXT("H");		break; //红桃
	case 0x03:		strResult = TEXT("S");		break; //黑桃
	default:		ASSERT( FALSE );				return strResult;
	}

	//转换牌值
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

//计算得分
SCORE CTableFrameSink::CalculateScore(bool bProbeJudgeScore)
{
	//变量定义
	LONG static cbRevenue=m_pGameServiceOption->wRevenueRatio;

	//推断玩家
	int  nWinMultiple[AREA_COUNT];
	m_GameLogic.DeduceWinner(nWinMultiple,m_cbDiceValue);
	
	if (!bProbeJudgeScore)
	{
		//添加游戏记录
		tagGameRecord &GameRecord = m_GameRecordList[m_nRecordLast];
		memcpy(GameRecord.cbDiceValue,m_cbDiceValue,sizeof(GameRecord.cbDiceValue));
		m_nRecordLast = (m_nRecordLast+1)%MAX_SCORE_HISTORY;
		if ( m_nRecordLast == m_nRecordFirst)
			m_nRecordFirst = (m_nRecordFirst+1)%MAX_SCORE_HISTORY;
	}
	

	//庄家总量
	SCORE lBankerWinScore = 0;

	//系统输赢
	LONGLONG lSystemScore = 0l;

	//玩家成绩
	SCORE lUserLostScore[GAME_PLAYER];
	ZeroMemory(m_lUserRevenue, sizeof(m_lUserRevenue));
	ZeroMemory(lUserLostScore, sizeof(lUserLostScore));
	ZeroMemory(m_lUserWinScore, sizeof(m_lUserWinScore));
	ZeroMemory(m_lUserReturnScore, sizeof(m_lUserReturnScore));
	
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

	//计算积分
	for (WORD wChairID=0; wChairID<GAME_PLAYER; wChairID++)
	{
		//庄家判断
		if (m_wCurrentBanker==wChairID) continue;

		//获取用户
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

				//系统得分
				if(bIsAndroidUser)
					lSystemScore += m_lUserAllBet[cbAreaIndex][wChairID] * nWinMultiple[cbAreaIndex];

				if (m_wCurrentBanker == INVALID_CHAIR || bIsBankerAndroidUser)
					lSystemScore -= m_lUserAllBet[cbAreaIndex][wChairID] * nWinMultiple[cbAreaIndex];
				
				
			}
			else
			{
				lUserLostScore[wChairID] +=m_lUserAllBet[cbAreaIndex][wChairID] * nWinMultiple[cbAreaIndex];

				//系统得分
				if(bIsAndroidUser)
					lSystemScore += m_lUserAllBet[cbAreaIndex][wChairID] * nWinMultiple[cbAreaIndex];
				if (m_wCurrentBanker == INVALID_CHAIR || bIsBankerAndroidUser)
					lSystemScore -= m_lUserAllBet[cbAreaIndex][wChairID] * nWinMultiple[cbAreaIndex];
			}
			lBankerWinScore -= m_lUserAllBet[cbAreaIndex][wChairID] * nWinMultiple[cbAreaIndex];
		}


		m_lUserWinScore[wChairID] += lUserLostScore[wChairID];

		//计算税收
		if (m_lUserWinScore[wChairID]>0)
		{
			float fRevenuePer=float(cbRevenue/1000.0);
			m_lUserRevenue[wChairID]  = static_cast<SCORE>(m_lUserWinScore[wChairID]*fRevenuePer);
			m_lUserWinScore[wChairID] -= m_lUserRevenue[wChairID];
		}

		
	}

	//庄家成绩
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

		//计算税收
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
		//累计积分
		m_lBankerWinScore += lBankerWinScore;
		//当前积分
		m_lBankerCurGameScore=lBankerWinScore;

		if (!m_cbNeedControl && !m_bControlWin)
		{
			//写库存
			WriteStorage(lSystemScore);
		}		
	}

	m_lCurSystemWinScore=lSystemScore;
	
	return lBankerWinScore;
}

//查询限额
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

//查询是否扣服务费
bool CTableFrameSink::QueryBuckleServiceCharge(WORD wChairID)
{
	for (BYTE i=0;i<GAME_PLAYER;i++)
	{
		IServerUserItem *pUserItem=m_pITableFrame->GetTableUserItem(i);
		if(pUserItem==NULL) continue;

		if (m_wCurrentBanker==i) return true;

		if(wChairID==i)
		{
			//返回下注
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


//发送记录
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


//玩家逃跑
bool CTableFrameSink::PlayerFlee(WORD wChairID,IServerUserItem*pIServerUserItem)
{
	if(pIServerUserItem==NULL) return true;

	if(m_pITableFrame->GetGameStatus()==GS_GAME_END)
	{
		//胜利类型
		//enScoreKind ScoreKind=(m_lUserWinScore[wChairID]>0L)?enScoreKind_Win:enScoreKind_Lost;
		//写入积分
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
			//发送消息
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_REVOCAT_BET,&RevocatBet,sizeof(CMD_S_RevocatBet));
		}
	}
	return  true;
}


//上庄控制 (机器人)
VOID CTableFrameSink::RobotBankerControl()
{
	//是否添加上庄机器人
	//if (m_ApplyUserArray.GetCount()-1 > m_nRobotBankerApplayMaxCount)
	//	return;


	////变量定义
	//int nRobotBankerCount = m_nRobotBankerApplayMinCount + rand()%(m_nRobotBankerApplayMaxCount-m_nRobotBankerApplayMinCount)-(m_ApplyUserArray.GetCount()-1);
	//int nSendBankerMsgCount = 0;
	//int nRandNum = rand() % GAME_PLAYER;

	////可上庄机器人数量
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
	//			//检测上庄列表
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
	//				//发送消息
	//				m_pITableFrame->SendTableData(wChairID, SUB_S_ROBOT_BANKER);

	//				//MyDebug(_T("RobotBankerControl B %d [%d/%d]"), wChairID, nSendBankerMsgCount+1, nRobotBankerCount);

	//				//人数统计
	//				nSendBankerMsgCount++;
	//				if (nSendBankerMsgCount >= nRobotBankerCount)
	//					break;
	//			}
	//		}
	//	}
	//}
}

//配置控制 (机器人)
VOID CTableFrameSink::RobotConfigControl()
{
	//机器信息
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

	//动态管理
	m_RobotCfgManage.RobotDynamicManage(strRobotName, bRobotCount);

	//发送配置
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



//用户断线
bool  CTableFrameSink::OnActionUserOffLine(WORD wChairID, IServerUserItem * pIServerUserItem)
{
	//切换庄家
	if (wChairID==m_wCurrentBanker) ChangeBanker(true);

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
		CancelBanker.wCancelUser=wChairID;

		//发送消息
		m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));
		m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));

		break;
	}

	return true;
}
//用户坐下
bool  CTableFrameSink::OnActionUserSitDown(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	//起始分数
	m_lUserStartScore[wChairID] = pIServerUserItem->GetUserScore();
	//设置时间
	if ((bLookonUser==false)&&(m_dwStartTime==0L))
	{
		m_dwStartTime=(DWORD)time(NULL);
		m_pITableFrame->SetGameTimer(IDI_FREE,m_dwTimeGameFree*1000,1,NULL);
		m_pITableFrame->SetGameStatus(GAME_STATUS_FREE);
	}

	//限制提示
	TCHAR szTipMsg[138];
	_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("本房间上庄条件为:%I64d,区域限制为:%I64d,玩家限制为:%I64d"),m_lApplyBankerCondition,m_lAreaLimitScore,m_lUserLimitScore);
	m_pITableFrame->SendGameMessage(pIServerUserItem,szTipMsg,SMT_CHAT);

	return true;
}
//用户起来
bool  CTableFrameSink::OnActionUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	//起始分数
	m_lUserStartScore[wChairID] = 0;
	//记录成绩
	if (bLookonUser==false)
	{
		//玩家逃跑
		BYTE cbGameStatus=m_pITableFrame->GetGameStatus();
	   if(pIServerUserItem->GetUserStatus()!=US_PLAYING&&(cbGameStatus==GS_PLAYER_BET||cbGameStatus==GS_GAME_END))
	   {
		   PlayerFlee(wChairID,pIServerUserItem);
	   }
		
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
			CancelBanker.wCancelUser=wChairID;

			//发送消息
			m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));
			m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));

			break;
		}

		return true;
	}

	return true;
}
//查找庄家
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
			//坐庄判断
			SCORE lUserScore=pIServerUserItem->GetUserScore();
			if(lUserScore<m_lApplyBankerCondition)
			{
				//发送消息
				CMD_S_CancelBanker CancelBanker;
				ZeroMemory(&CancelBanker,sizeof(CancelBanker));

				//设置变量
				CancelBanker.wCancelUser = pIServerUserItem->GetChairID();

				//发送消息
				m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));
				m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));

				//提示消息
				TCHAR szTipMsg[128];
				_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("由于你的金币数少于坐庄必须金币数（%I64d）,你自动下庄！"),m_lApplyBankerCondition);
				SendGameMessage(m_ApplyUserArray[0],szTipMsg);

				//删除玩家
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
//获取分数
void CTableFrameSink::GetStockScore(SCORE *lAndroidScore,SCORE *lUserScore,BYTE bDice[])
{
	//系统做庄
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

	//计算分数
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

	//庄家分数
	if(bIsAndroid == true)
	{
		*lAndroidScore+=lBankerWinScore;
	}
	else
	{
		*lUserScore+=lBankerWinScore;
	}
}

//重新发牌
bool CTableFrameSink::DispatchTableLabel()
{
	//派发扑克
	for(BYTE cbIndex=0;cbIndex<DICE_COUNT;cbIndex++)
	{
		m_cbDiceValue[cbIndex]=rand()%6+1;
	}

	return true;
}

//判断玩家输赢状态
BYTE CTableFrameSink::ControlUserWin()
{
	BYTE cbUserWinType = 0;

	//推断玩家
	int  nWinMultiple[AREA_COUNT];
	m_GameLogic.DeduceWinner(nWinMultiple,m_cbDiceValue);

	//庄家总量
	SCORE lBankerWinScore = 0;

	//系统输赢
	LONGLONG lSystemScore = 0l;

	//玩家成绩
	SCORE lUserLostScore[GAME_PLAYER];
	SCORE lUserWinScore[GAME_PLAYER];
	ZeroMemory(lUserLostScore, sizeof(lUserLostScore));
	ZeroMemory(lUserWinScore, sizeof(lUserWinScore));

	//计算积分
	for (WORD wChairID=0; wChairID<GAME_PLAYER; wChairID++)
	{
		//庄家判断
		if (m_wCurrentBanker==wChairID) continue;

		//获取用户
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

		//计算税收
		if (lUserWinScore[wChairID]>=0)
		{
			cbUserWinType = 1;
		}
		else
			cbUserWinType = 2;
	}

	//不管庄家，当有人当庄时，有下面代码就控制不了个人了
	////庄家成绩
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

	//	//计算税收
	//	if (lUserWinScore[m_wCurrentBanker]>=0)
	//	{
	//		cbUserWinType = 1;
	//	}
	//	else
	//		cbUserWinType = 2;
	//}

	return cbUserWinType;
}

//试探性判断
bool CTableFrameSink::ProbeJudge()
{
	//系统输赢
	LONGLONG lSystemScore = 0l;
	{
		m_lCurSystemWinScore = 0L;
		CalculateScore(true);
		lSystemScore=m_lCurSystemWinScore;

		// 11/11/2013 [Wan.MY] 
		// 这里添加送分判断
		if(( m_lStorageCurrent > m_lStorageMax1 && m_lStorageCurrent <= m_lStorageMax2 && rand()%100 <= m_lStorageMul1 && lSystemScore != 0 )||
			(m_lStorageCurrent > m_lStorageMax2 && rand()%100 <= m_lStorageMul2))
		{
			// 如果系统还是赢钱，返回生成下一个
			if( lSystemScore > 0 )
			{
				return false;
			}

			// 够输，OK
			if( m_lStorageCurrent + lSystemScore >= 0)
			{
				return true;
			}

			// 输钱，但是库存不够赔钱, 那这一局就不让玩家赢钱了
			if( m_lStorageCurrent + lSystemScore < 0 )
			{				
				return false;
			}
		}


		//系统分值计算
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


//写库存
void CTableFrameSink::WriteStorage(LONGLONG lSystemScore)
{
	m_lStorageCurrent += lSystemScore;
	if (m_lStorageCurrent>0)
	{
		m_lStorageCurrent = m_lStorageCurrent - (m_lStorageCurrent * m_nStorageDeduct / 1000);
	}
	
	CString strLog;
	strLog.Format(TEXT("[第%d桌]：当前库存 %I64d"),m_pITableFrame->GetTableID()+1,m_lStorageCurrent);
	NcaTextOut(strLog);
}

//测试写信息
void CTableFrameSink::WriteInfo( LPCTSTR pszFileName, LPCTSTR pszString )
{
	//设置语言区域
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

	//还原区域设定
	setlocale( LC_CTYPE, old_locale );
	free( old_locale );
}

//读取配置
void CTableFrameSink::ReadConfigInformation()
{
	//获取自定义配置
	tagCustomConfig *pCustomConfig = (tagCustomConfig *)m_pGameServiceOption->cbCustomRule;
	ASSERT(pCustomConfig);

	//上庄
	m_lApplyBankerCondition = pCustomConfig->CustomGeneral.lApplyBankerCondition;
	m_nBankerTimeLimit = pCustomConfig->CustomGeneral.lBankerTime;
	m_nBankerTimeAdd = pCustomConfig->CustomGeneral.lBankerTimeAdd;
	m_lExtraBankerScore = pCustomConfig->CustomGeneral.lBankerScoreMAX;
	m_nExtraBankerTime = pCustomConfig->CustomGeneral.lBankerTimeExtra;
	m_bEnableSysBanker = (pCustomConfig->CustomGeneral.nEnableSysBanker == TRUE)?true:false;

	//时间
	m_dwTimeGameFree = pCustomConfig->CustomGeneral.lFreeTime;
	m_dwTimeGameBet = pCustomConfig->CustomGeneral.lBetTime;
	m_dwTimeGameEnd = pCustomConfig->CustomGeneral.lEndTime;
	if(m_dwTimeGameFree < TIME_FREE	|| m_dwTimeGameFree > 99) m_dwTimeGameFree = TIME_FREE;
	if(m_dwTimeGameBet < TIME_PLACE_JETTON || m_dwTimeGameBet > 99) m_dwTimeGameBet = TIME_PLACE_JETTON;
	if(m_dwTimeGameEnd < TIME_GAME_END || m_dwTimeGameEnd > 99) m_dwTimeGameEnd = TIME_GAME_END;

	//下注
	m_lAreaLimitScore = pCustomConfig->CustomGeneral.lAreaLimitScore;
	m_lUserLimitScore = pCustomConfig->CustomGeneral.lUserLimitScore;
	m_nEndGameMul = pCustomConfig->CustomGeneral.lEndGameMul;
	if(m_nEndGameMul < 1 || m_nEndGameMul > 100 ) m_nEndGameMul = 80;

	//库存
	m_lStorageStart = pCustomConfig->CustomGeneral.StorageStart;
	m_lStorageCurrent = m_lStorageStart;
	m_nStorageDeduct = pCustomConfig->CustomGeneral.StorageDeduct;
	m_lStorageMax1 = pCustomConfig->CustomGeneral.StorageMax1;
	m_lStorageMul1 = pCustomConfig->CustomGeneral.StorageMul1;
	m_lStorageMax2 = pCustomConfig->CustomGeneral.StorageMax2;
	m_lStorageMul2 = pCustomConfig->CustomGeneral.StorageMul2;
	if(m_lStorageMul1 < 0 || m_lStorageMul1 > 100 ) m_lStorageMul1 = 50;
	if(m_lStorageMul2 < 0 || m_lStorageMul2 > 100 ) m_lStorageMul2 = 80;

	//机器人
	m_nRobotListMaxCount = pCustomConfig->CustomAndroid.lRobotListMaxCount;

	LONGLONG lRobotBetMinCount = pCustomConfig->CustomAndroid.lRobotBetMinCount;
	LONGLONG lRobotBetMaxCount = pCustomConfig->CustomAndroid.lRobotBetMaxCount;
	m_nMaxChipRobot = rand()%(lRobotBetMaxCount-lRobotBetMinCount+1) + lRobotBetMinCount;
	if (m_nMaxChipRobot < 0)	m_nMaxChipRobot = 8;
	m_lRobotAreaLimit = pCustomConfig->CustomAndroid.lRobotAreaLimit;

	return;
}

//发送下注信息
void CTableFrameSink::SendUserBetInfo( IServerUserItem *pIServerUserItem )
{
	if(NULL == pIServerUserItem) return;

	//权限判断
	if(!CUserRight::IsGameCheatUser(pIServerUserItem->GetUserRight())) return;

	//构造数据
	CMD_S_SendUserBetInfo SendUserBetInfo;
	ZeroMemory(&SendUserBetInfo, sizeof(SendUserBetInfo));

	CopyMemory(&SendUserBetInfo.lUserStartScore, m_lUserStartScore, sizeof(m_lUserStartScore));

	for (int i=0;i<AREA_COUNT;i++)
	{
		if (m_lUserAllBet[i] > 0)
		{
			CopyMemory(&SendUserBetInfo.lUserJettonScore, m_lUserAllBet[i], sizeof(m_lUserAllBet[i]));
			SendUserBetInfo.cbAreaCount = i;
			//发送消息	
			m_pITableFrame->SendUserItemData(pIServerUserItem, SUB_S_SEND_USER_BET_INFO, &SendUserBetInfo, sizeof(SendUserBetInfo));
		}		
	}	

	return;
}