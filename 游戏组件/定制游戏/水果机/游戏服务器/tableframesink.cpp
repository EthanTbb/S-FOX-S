#include "StdAfx.h"
#include "TableFrameSink.h"
#include "GameServerManager.h"
#include <locale>

//////////////////////////////////////////////////////////////////////////

//常量定义
#define SEND_COUNT					300									//发送次数

//索引定义
#define INDEX_PLAYER				0									//闲家索引
#define INDEX_BANKER				1									//庄家索引

//下注时间
#define IDI_FREE					1									//空闲时间
#define TIME_FREE					10									//空闲时间

//下注时间
#define IDI_PLACE_JETTON			2									//下注时间
#define TIME_PLACE_JETTON			20									//下注时间
//结束时间
#define IDI_GAME_END				3									//结束时间
#define TIME_GAME_END				15									//结束时间

//下注时间
#define IDI_SMALL_BIG				4									//下注时间
#define TIME_SMALL_BIG				10									//下注时间
#define TIME_GAME_ADD               15                                  //附加时间
//////////////////////////////////////////////////////////////////////////
//静态变量
const WORD			CTableFrameSink::m_wPlayerCount=GAME_PLAYER;				//游戏人数
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
		fp=fopen("碰碰车库存.log","w");	
	}	
	else	
	{		
		fp=fopen("碰碰车库存.log","a");	
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

//构造函数
CTableFrameSink::CTableFrameSink()
{
	//总下注数
	ZeroMemory(m_lAllJettonScore,sizeof(m_lAllJettonScore));

	//个人下注
	ZeroMemory(m_lUserJettonScore,sizeof(m_lUserJettonScore));

	m_CheckImage = 0;

	//玩家成绩	
	ZeroMemory(m_lUserWinScore,sizeof(m_lUserWinScore));
	ZeroMemory(m_lUserTempScore, sizeof(m_lUserTempScore));
	ZeroMemory(m_lUserReturnScore,sizeof(m_lUserReturnScore));
	ZeroMemory(m_lUserRevenue,sizeof(m_lUserRevenue));
	ZeroMemory(m_lUserEveryTurnScore,sizeof(m_lUserEveryTurnScore));
	ZeroMemory(m_lUserTotalScore,sizeof(m_lUserTotalScore));
	//扑克信息
	ZeroMemory(m_cbCardCount,sizeof(m_cbCardCount));
	ZeroMemory(m_cbTableCardArray,sizeof(m_cbTableCardArray));

	//状态变量
	m_dwJettonTime=0L;
	m_bControl=false;

	//庄家信息
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

	//记录变量
	ZeroMemory(m_GameRecordArrary,sizeof(m_GameRecordArrary));
	ZeroMemory(m_lExchangeGold,sizeof(m_lExchangeGold));
	m_nRecordFirst=0;
	m_nRecordLast=0;
	m_dwRecordCount=0;	
	m_nProbability=0;
	m_nRand=0;

	//控制变量
	m_StorageStart = 0l;
	m_StorageDeduct = 0l;

	//机器人控制					
	m_lRobotAreaLimit = 0l;
	m_lRobotBetCount = 0l;		
	m_nRobotListMaxCount =0;

	//庄家设置
	m_lBankerMAX = 0l;
	m_lBankerAdd = 0l;							
	m_lBankerScoreMAX = 0l;
	m_lBankerScoreAdd = 0l;
	m_lPlayerBankerMAX = 0l;
	m_bExchangeBanker = true;

	//时间控制
	m_cbFreeTime = TIME_FREE;
	m_cbBetTime = TIME_PLACE_JETTON;
	m_cbEndTime = TIME_GAME_END;
	m_cbCompareTime = TIME_FREE;
	m_cbAddTime=TIME_GAME_ADD;

	//机器人控制
	m_nChipRobotCount = 0;
	ZeroMemory(m_lRobotAreaScore, sizeof(m_lRobotAreaScore));

	//服务控制
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

//析构函数
CTableFrameSink::~CTableFrameSink(void)
{
}

//接口查询
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

//初始化
bool CTableFrameSink::Initialization(IUnknownEx * pIUnknownEx)
{
	//查询接口
	ASSERT(pIUnknownEx!=NULL);
	m_pITableFrame=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,ITableFrame);
	if (m_pITableFrame==NULL) return false;

	//控制接口
	//m_pITableFrameControl=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,ITableFrameControl);
	//if (m_pITableFrameControl==NULL) return false;
	m_pITableFrame->SetStartMode(START_MODE_TIME_CONTROL);

	//获取参数
	m_pGameServiceOption=m_pITableFrame->GetGameServiceOption();
	ASSERT(m_pGameServiceOption!=NULL);

	//设置文件名
	TCHAR szPath[MAX_PATH]=TEXT("");
	GetCurrentDirectory(sizeof(szPath),szPath);
	_sntprintf(m_szConfigFileName,sizeof(m_szConfigFileName),TEXT("%s\\FruitBattle.ini"),szPath);
	memcpy(m_szGameRoomName, m_pGameServiceOption->szServerName, sizeof(m_szGameRoomName));

	ReadConfigInformation(true);
	return true;
}

//复位桌子
VOID CTableFrameSink::RepositionSink()
{
	//总下注数
	ZeroMemory(m_lAllJettonScore,sizeof(m_lAllJettonScore));

	//个人下注
	ZeroMemory(m_lUserJettonScore,sizeof(m_lUserJettonScore));

	//玩家成绩	
	ZeroMemory(m_lUserTempScore, sizeof(m_lUserTempScore));
	ZeroMemory(m_lUserReturnScore,sizeof(m_lUserReturnScore));
	ZeroMemory(m_lUserRevenue,sizeof(m_lUserRevenue));
	m_bBankerQiut = false;
	
	//机器人控制
	m_nChipRobotCount = 0;
	ZeroMemory(m_lRobotAreaScore, sizeof(m_lRobotAreaScore));
	m_bControl=false;

	return;
}

bool CTableFrameSink::OnSubAmdinCommand(IServerUserItem*pIServerUserItem,const void*pDataBuffer)
{
	//如果不具有管理员权限 则返回错误
	if( !CUserRight::IsGameCheatUser(pIServerUserItem->GetUserRight()) )
		return false;

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
		return __max(pIServerUserItem->GetUserScore()-m_pGameServiceOption->lMinTableScore, 0L);
	}
}

//游戏开始
bool CTableFrameSink::OnEventGameStart()
{

	//库存日志
	StorageLog();
	//变量定义
	CMD_S_GameStart GameStart;
	ZeroMemory(&GameStart,sizeof(GameStart));

	//获取庄家
	IServerUserItem *pIBankerServerUserItem=NULL;
	if (INVALID_CHAIR!=m_wCurrentBanker) 
	{
		pIBankerServerUserItem=m_pITableFrame->GetTableUserItem(m_wCurrentBanker);
		m_lBankerScore=m_lExchangeGold[m_wCurrentBanker];
	}

	//设置变量
	GameStart.cbTimeLeave=m_cbBetTime;
	GameStart.wBankerUser=m_wCurrentBanker;
	GameStart.lBankerScore = 0x7fffffffffffffff;
	if (pIBankerServerUserItem!=NULL) 
		GameStart.lBankerScore=m_lExchangeGold[m_wCurrentBanker];

	GameStart.bContiueCard=m_bContiueCard;
	//下注机器人数量
	int nChipRobotCount = 0;
	for (int i = 0; i < GAME_PLAYER; i++)
		{
			IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(i);
			if (pIServerUserItem != NULL && pIServerUserItem->IsAndroidUser())
				nChipRobotCount++;
		}

	GameStart.nChipRobotCount = min(nChipRobotCount, m_nMaxChipRobot);

	//上庄列表
	nChipRobotCount=0;
	for (int i = 0; i < m_ApplyUserArray.GetCount(); i++)
	{
		IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(m_ApplyUserArray.GetAt(i));
		if (pIServerUserItem != NULL && pIServerUserItem->IsAndroidUser())
			nChipRobotCount++;
	}
	GameStart.nAndroidCount=nChipRobotCount;
	//机器人控制
	m_nChipRobotCount = 0;

	//旁观玩家
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_START,&GameStart,sizeof(GameStart));	

	//游戏玩家
	for (WORD wChairID=0; wChairID<GAME_PLAYER; ++wChairID)
	{
		IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(wChairID);
		if (pIServerUserItem==NULL) continue;
		
		if(pIServerUserItem->GetUserScore() < m_nExchange*1)
		{
			CString szinfo;
			szinfo.Format(TEXT("您的游戏币不足以进行游戏，请先补充游戏币"));
			m_pITableFrame->SendGameMessage(pIServerUserItem,szinfo,SMT_CLOSE_GAME|SMT_EJECT);
			continue;
		}

		
		if (m_bEnableSysBanker==false && m_wCurrentBanker==INVALID_CHAIR)
			{
				GameStart.lBankerScore = 1;
			}

		//设置积分
		if (pIServerUserItem->IsAndroidUser())
			{
				GameStart.lUserMaxScore=min(m_lExchangeGold[wChairID],m_lUserLimitScore);
			}
		else
			{
				GameStart.lUserMaxScore=min(m_lExchangeGold[wChairID],m_lUserLimitScore);
			}


		m_pITableFrame->SendTableData(wChairID,SUB_S_GAME_START,&GameStart,sizeof(GameStart));	

		//权限判断
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

//游戏结束
bool  CTableFrameSink::OnEventGameConclude(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)
{
	switch (cbReason)
	{
	case GER_NORMAL:		//常规结束	
		{
			try
			{
				//控制
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
						//系统输赢
						LONGLONG lSystemScore = 0l;
						//计算积分
						for (WORD wChairID1=0; wChairID1<GAME_PLAYER; wChairID1++)
						{
							//庄家判断
							if (m_wCurrentBanker == wChairID1) continue;

							//获取用户
							IServerUserItem * pIServerUserItem=m_pITableFrame->GetTableUserItem(wChairID1);
							if (pIServerUserItem==NULL) 
								continue;

							bool bIsAndroidUser = pIServerUserItem->IsAndroidUser();
							//玩家下注
							LONGLONG *pUserScore[AREA_COUNT+1];
							pUserScore[0]=NULL;
							for (int i = 1;i<=AREA_COUNT;i++)
							{
								pUserScore[i]=m_lUserJettonScore[i];
							}
							//坐庄是否是机器人
							bool bIsBankerAndroidUser = false;
							if ( m_wCurrentBanker != INVALID_CHAIR )
							{
								IServerUserItem * pIBankerUserItem = m_pITableFrame->GetTableUserItem(m_wCurrentBanker);
								if (pIBankerUserItem != NULL) 
								{
									bIsBankerAndroidUser = pIBankerUserItem->IsAndroidUser();
								}
							}
							//统计积分
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
						//只有库存大于0才会衰减
						if(m_StorageStart > 0 && NeedDeductStorage())
							m_StorageStart = m_StorageStart - (m_StorageStart * m_StorageDeduct / 1000);
				}
				else
				{
					while(true)
					{
						//派发扑克
						DispatchTableCard();

						//试探性判断
						if (ProbeJudge())
						{
							break;
						}
					}
				}			
				//计算分数
				LONGLONG lBankerWinScore=CalculateScore();
				
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
				GameEnd.nProbability=m_nProbability;
				GameEnd.nRand=m_nRand;
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
					GameEnd.lUserScore=m_lUserEveryTurnScore[wUserIndex];

					//返还积分
					GameEnd.lUserReturnScore[wUserIndex]=m_lUserReturnScore[wUserIndex];

					//设置税收
					if (m_lUserRevenue[wUserIndex]>0) GameEnd.lRevenue=m_lUserRevenue[wUserIndex];
					else if (m_wCurrentBanker!=INVALID_CHAIR) GameEnd.lRevenue=m_lUserRevenue[m_wCurrentBanker];
					else GameEnd.lRevenue=0;

					//发送消息					
					m_pITableFrame->SendTableData(wUserIndex,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
					m_pITableFrame->SendLookonData(wUserIndex,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
				}

				for (WORD wChairIDIndex=0; wChairIDIndex<GAME_PLAYER; wChairIDIndex++)
				{
					//写入积分
					if(m_lUserWinScore[wChairIDIndex]!=0L)
					{		
						IServerUserItem *pIServerUserItemTemp = m_pITableFrame->GetTableUserItem(wChairIDIndex);
						if ( pIServerUserItemTemp == NULL ) continue;

						//写入积分前 记录玩家总输赢
						m_lUserTotalScore[wChairIDIndex]+=m_lUserWinScore[wChairIDIndex];
						//变量定义
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
// 								str.Format(TEXT("玩家ID:%d,昵称:%s,结算异常，携带金币:%I64d,输分:%I64d"),pIServerUserItemTemp->GetUserID(),pIServerUserItemTemp->GetNickName(),pIServerUserItemTemp->GetUserScore(),lTotalScore);
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
	case GER_USER_LEAVE:		//用户离开
		{
			//闲家判断
			if (m_wCurrentBanker!=wChairID)
			{
				try
				{
					//变量定义
					LONGLONG lScore=0;
					LONGLONG lRevenue=0;

					//统计成绩
					for (int nAreaIndex=1; nAreaIndex<=AREA_COUNT; ++nAreaIndex) lScore -= m_lUserJettonScore[nAreaIndex][wChairID];

					//写入积分
					if (m_pITableFrame->GetGameStatus() < GS_GAME_END)
					{
						for (int nAreaIndex=1; nAreaIndex<=AREA_COUNT; ++nAreaIndex)
						{
							if (m_lUserJettonScore[nAreaIndex][wChairID] > 0)
							{
								//逃跑玩家扣除当前所下注的筹码给系统
								m_lUserWinScore[wChairID]-= m_lUserJettonScore[nAreaIndex][wChairID];

								CMD_S_PlaceJettonFail PlaceJettonFail;
								ZeroMemory(&PlaceJettonFail,sizeof(PlaceJettonFail));
								PlaceJettonFail.lJettonArea=nAreaIndex;
								PlaceJettonFail.lPlaceScore=m_lUserJettonScore[nAreaIndex][wChairID];
								PlaceJettonFail.wPlaceUser=wChairID;

								//游戏玩家
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
							//变量定义
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
// 										str.Format(TEXT("玩家ID:%d,昵称:%s,结算异常，携带金币:%I64d,输分:%I64d"),pIServerUserItem->GetUserID(),pIServerUserItem->GetNickName(),pIServerUserItem->GetUserScore(),lTotalScore);
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
						//写入积分
						if(m_lUserWinScore[wChairID]!=0L)
						{
							//变量定义
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
// 										str.Format(TEXT("玩家ID:%d,昵称:%s,结算异常，携带金币:%I64d,输分:%I64d"),pIServerUserItem->GetUserID(),pIServerUserItem->GetNickName(),pIServerUserItem->GetUserScore(),lTotalScore);
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
				//状态判断			
				if (m_pITableFrame->GetGameStatus()==GS_GAME_FREE)
				{
					m_pITableFrame->KillGameTimer(IDI_FREE);
					m_pITableFrame->StartGame();
					//设置时间
					m_dwJettonTime=(DWORD)time(NULL);
					m_pITableFrame->SetGameTimer(IDI_PLACE_JETTON,m_cbBetTime*1000,1,0L);

					//设置状态
					m_pITableFrame->SetGameStatus(GS_PLACE_JETTON);
				}
				else if (m_pITableFrame->GetGameStatus() < GS_GAME_END && m_pITableFrame->GetGameStatus() > GS_GAME_FREE)
				{
					//提示消息
					TCHAR szTipMsg[128];
					_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("由于庄家[ %s ]强退，游戏提前结束！"),pIServerUserItem->GetNickName());
					m_bBankerQiut = true;

					//发送消息
					SendGameMessage(INVALID_CHAIR,szTipMsg);				

					//设置时间
					//m_pITableFrame->KillGameTimer(IDI_FREE);
					m_pITableFrame->KillGameTimer(IDI_PLACE_JETTON);
					m_dwJettonTime=(DWORD)time(NULL);
					m_pITableFrame->SetGameTimer(IDI_GAME_END,m_cbEndTime*1000,1,0L);

					//设置状态
					m_pITableFrame->SetGameStatus(GS_GAME_END);

					//控制
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
							//派发扑克
							DispatchTableCard();

							//试探性判断
							if (ProbeJudge())
							{
								break;
							}
						}
					}

					//计算分数
					CalculateScore();

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
						GameEnd.lUserScore=m_lUserEveryTurnScore[wUserIndex];

						//返还积分
						GameEnd.lUserReturnScore[wUserIndex]=m_lUserReturnScore[wUserIndex];

						//设置税收
						if (m_lUserRevenue[wUserIndex]>0) GameEnd.lRevenue=m_lUserRevenue[wUserIndex];
						else if (m_wCurrentBanker!=INVALID_CHAIR) GameEnd.lRevenue=m_lUserRevenue[m_wCurrentBanker];
						else GameEnd.lRevenue=0;

						//发送消息					
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

						//库存金币
						if (!pIServerUserItem->IsAndroidUser())
						{
							TempStartScore -= m_lUserWinScore[wUserChairID];
							m_lExchangeGold[wUserChairID]+=m_lUserReturnScore[wUserChairID];
						}
					}
					if(!(m_pServerContro!=NULL&&m_bControl))
						m_StorageStart+=TempStartScore*m_nExchange;

					//结束游戏
					m_pITableFrame->ConcludeGame(GS_GAME_FREE);

					if (m_wCurrentBanker!=INVALID_CHAIR)
					{
						m_lExchangeGold[m_wCurrentBanker]+=m_lBankerCurGameScore;
					}

					//扣除庄家分数			
					if(m_lUserWinScore[m_wCurrentBanker]!=0L)
					{
						//变量定义
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
									str.Format(TEXT("玩家ID:%d,昵称:%s,结算异常，携带金币:%I64d,输分:%I64d"),pIServerUserItem->GetUserID(),pIServerUserItem->GetNickName(),pIServerUserItem->GetUserScore(),lTotalScore);
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

					//切换庄家
					ChangeBanker(true);

					//设置时间
					m_dwJettonTime=(DWORD)time(NULL);
					m_pITableFrame->SetGameTimer(IDI_FREE,m_cbFreeTime*1000,1,0L);

					//发送消息
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
				//扣除分数			
				if(m_lUserWinScore[m_wCurrentBanker]!=0L)
				{
					//变量定义
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
								str.Format(TEXT("玩家ID:%d,昵称:%s,结算异常，携带金币:%I64d,输分:%I64d"),pIServerUserItem->GetUserID(),pIServerUserItem->GetNickName(),pIServerUserItem->GetUserScore(),lTotalScore);
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

				//切换庄家
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

//发送场景
bool CTableFrameSink::OnEventSendGameScene(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbGameStatus, bool bSendSecret)
{
	switch (cbGameStatus)
	{
	case GS_GAME_FREE:			//空闲状态
		{
			//发送记录
			SendGameRecord(pIServerUserItem);

			//构造数据
			CMD_S_StatusFree StatusFree;
			ZeroMemory(&StatusFree,sizeof(StatusFree));			

			//控制信息
			StatusFree.lApplyBankerCondition = m_lApplyBankerCondition;
			StatusFree.lAreaLimitScore = m_lAreaLimitScore;
			StatusFree.CheckImage = m_CheckImage;
			StatusFree.nExchange=m_nExchange;
			StatusFree.lStartStorage=m_lStartStorage;

			m_lExchangeGold[wChairID]=0;

			//庄家信息
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
			
			//玩家信息
			if (pIServerUserItem->GetUserStatus()!=US_LOOKON)
			{
				StatusFree.lUserMaxScore=min(m_lExchangeGold[wChairID],m_lUserLimitScore); 
			}

			//全局信息
			DWORD dwPassTime=(DWORD)time(NULL)-m_dwJettonTime;
			StatusFree.cbTimeLeave=(BYTE)(m_cbFreeTime-__min(dwPassTime,m_cbFreeTime));

			//房间名称
			CopyMemory(StatusFree.szGameRoomName, m_pGameServiceOption->szServerName, sizeof(StatusFree.szGameRoomName));

			//机器人配置
			if(pIServerUserItem->IsAndroidUser())
			{
				tagCustomConfig *pCustomConfig = (tagCustomConfig *)m_pITableFrame->GetCustomRule();
				ASSERT(pCustomConfig);

				CopyMemory(&StatusFree.CustomAndroid, &pCustomConfig->CustomAndroid, sizeof(tagCustomAndroid));

				//机器人坐下 自动全部兑换筹码
// 				srand(GetTickCount());
// 				int nTimes = rand()%9+2;//2-10倍
				m_lExchangeGold[wChairID] += pIServerUserItem->GetUserScore()/m_nExchange;
			}
			//发送场景
			bool bSuccess = m_pITableFrame->SendGameScene(pIServerUserItem,&StatusFree,sizeof(StatusFree));
// #ifndef DEBUG
// 			//限制提示
// 			TCHAR szTipMsg[128];
// 			_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("本房间上庄条件为：%I64d,区域限制为：%I64d,玩家限制为：%I64d"),m_lApplyBankerCondition,
// 				m_lAreaLimitScore,m_lUserLimitScore);
// 
// 			m_pITableFrame->SendGameMessage(pIServerUserItem,szTipMsg,SMT_CHAT);
// #endif
			//发送申请者
			SendApplyUser(pIServerUserItem);
			//权限判断
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
	case GS_PLACE_JETTON:		//游戏状态
	case GS_GAME_END:			//结束状态
		{
			//发送记录
			SendGameRecord(pIServerUserItem);		

			//构造数据
			CMD_S_StatusPlay StatusPlay={0};

			//全局下注
			CopyMemory(StatusPlay.lAllJettonScore,m_lAllJettonScore,sizeof(StatusPlay.lAllJettonScore));

			m_lExchangeGold[wChairID]=0;

			//玩家下注
			if (pIServerUserItem->GetUserStatus()!=US_LOOKON)
			{
				for (int nAreaIndex=1; nAreaIndex<=AREA_COUNT; ++nAreaIndex)
					StatusPlay.lUserJettonScore[nAreaIndex] = m_lUserJettonScore[nAreaIndex][wChairID];

				//最大下注
				StatusPlay.lUserMaxScore=min(m_lExchangeGold[wChairID],m_lUserLimitScore);
			}

			//控制信息
			StatusPlay.lApplyBankerCondition=m_lApplyBankerCondition;		
			StatusPlay.lAreaLimitScore=m_lAreaLimitScore;		
			StatusPlay.lStartStorage = m_lStartStorage;
			//庄家信息
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

			//全局信息
			DWORD dwPassTime=(DWORD)time(NULL)-m_dwJettonTime;
			int	nTotalTime = (cbGameStatus==GS_PLACE_JETTON?m_cbBetTime:m_cbEndTime);
			StatusPlay.cbTimeLeave=(BYTE)(nTotalTime-__min(dwPassTime,(DWORD)nTotalTime));
			StatusPlay.cbGameStatus=m_pITableFrame->GetGameStatus();						

			//结束判断
			if (cbGameStatus==GS_GAME_END)
			{
				if(m_nProbability==2||m_nProbability==3||m_nProbability==4)
					nTotalTime = m_cbEndTime+5;
				else
					nTotalTime = m_cbEndTime+m_cbAddTime*m_nRand;
				StatusPlay.cbTimeLeave=(BYTE)(nTotalTime-__min(dwPassTime,m_cbEndTime));

				//设置成绩
				StatusPlay.lEndUserScore=m_lUserWinScore[wChairID];

				//返还积分
				StatusPlay.lEndUserReturnScore=m_lUserReturnScore[wChairID];

				//设置税收
				if (m_lUserRevenue[wChairID]>0) StatusPlay.lEndRevenue=m_lUserRevenue[wChairID];
				else if (m_wCurrentBanker!=INVALID_CHAIR) StatusPlay.lEndRevenue=m_lUserRevenue[m_wCurrentBanker];
				else StatusPlay.lEndRevenue=0;

				//庄家成绩
				StatusPlay.lEndBankerScore=m_lBankerCurGameScore;

				//扑克信息
				CopyMemory(StatusPlay.cbTableCardArray,m_cbTableCardArray,sizeof(m_cbTableCardArray));
			}

			//房间名称
			CopyMemory(StatusPlay.szGameRoomName, m_pGameServiceOption->szServerName, sizeof(StatusPlay.szGameRoomName));

			//机器人配置
			if(pIServerUserItem->IsAndroidUser())
			{
				tagCustomConfig *pCustomConfig = (tagCustomConfig *)m_pITableFrame->GetCustomRule();
				ASSERT(pCustomConfig);

				CopyMemory(&StatusPlay.CustomAndroid, &pCustomConfig->CustomAndroid, sizeof(tagCustomAndroid));
				//机器人坐下 自动兑换筹码
				m_lExchangeGold[wChairID] += pIServerUserItem->GetUserScore()/m_nExchange;
			}
			//发送场景
			bool bSuccess = m_pITableFrame->SendGameScene(pIServerUserItem,&StatusPlay,sizeof(StatusPlay));
// #ifndef DEBUG
// 			//限制提示
// 			TCHAR szTipMsg[128];
// 			_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("本房间上庄条件为：%I64d,区域限制为：%I64d,玩家限制为：%I64d"),m_lApplyBankerCondition,
// 				m_lAreaLimitScore,m_lUserLimitScore);
// 			
// 			m_pITableFrame->SendGameMessage(pIServerUserItem,szTipMsg,SMT_CHAT);
// #endif	
			//发送申请者
			SendApplyUser( pIServerUserItem );
			//权限判断
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

//猜大小时间
bool CTableFrameSink::OnCompareBigSmall()
{
	//结束消息
	CMD_S_Compare Compare;
	ZeroMemory(&Compare,sizeof(Compare));

	//发送积分
	Compare.cbTimeLeave=m_cbCompareTime;

	//发送消息					
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_COMPARE,&Compare,sizeof(Compare));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_COMPARE,&Compare,sizeof(Compare));
	

	return true;
}

//定时器事件
bool CTableFrameSink::OnTimerMessage(DWORD wTimerID, WPARAM wBindParam)
{
	switch (wTimerID)
	{
	case IDI_FREE:		//空闲时间
		{
			//开始游戏
			//m_pITableFrameControl->StartGame();
			m_pITableFrame->StartGame();
			//设置时间
			m_dwJettonTime=(DWORD)time(NULL);
			m_pITableFrame->SetGameTimer(IDI_PLACE_JETTON,m_cbBetTime*1000,1,0L);

			//设置状态
			m_pITableFrame->SetGameStatus(GS_PLACE_JETTON);

			return true;
		}
	case IDI_PLACE_JETTON:		//下注时间
		{
			//状态判断(防止强退重复设置)
			if (m_pITableFrame->GetGameStatus()!=GS_GAME_END)
			{
				//设置状态
				m_pITableFrame->SetGameStatus(GS_GAME_END);

				//结束游戏
				OnEventGameConclude(INVALID_CHAIR,NULL,GER_NORMAL);

				//设置时间
				m_dwJettonTime=(DWORD)time(NULL);
				if(m_nProbability==2||m_nProbability==3||m_nProbability==4)
					m_pITableFrame->SetGameTimer(IDI_GAME_END,(m_cbEndTime+5)*1000,1,0L);
				else
					m_pITableFrame->SetGameTimer(IDI_GAME_END,(m_cbEndTime+m_cbAddTime*m_nRand)*1000,1,0L);
			}

			return true;
		}
	case IDI_GAME_END:			//结束游戏
		{
			//庄家逃跑后不进入猜大小环节，直接结算
			if (m_bBankerQiut)
				{
					LONGLONG TempStartScore=0;
					for ( WORD wUserChairID = 0; wUserChairID < GAME_PLAYER; ++wUserChairID )
					{
						IServerUserItem *pIServerUserItem = m_pITableFrame->GetTableUserItem(wUserChairID);
						if ( pIServerUserItem == NULL||(QueryBuckleServiceCharge(wUserChairID)==false)) continue;

						//库存金币
						if (!pIServerUserItem->IsAndroidUser())
						{
							TempStartScore -= (m_lUserWinScore[wUserChairID]+m_lUserRevenue[wUserChairID]);
							m_lExchangeGold[wUserChairID]+=m_lUserReturnScore[wUserChairID];
						}
					}
					if(!(m_pServerContro!=NULL&&m_bControl))
						m_StorageStart+=TempStartScore*m_nExchange;

					//结束游戏
					m_pITableFrame->ConcludeGame(GS_GAME_FREE);

					if (m_wCurrentBanker!=INVALID_CHAIR)
					{
						m_lExchangeGold[m_wCurrentBanker]+=m_lBankerCurGameScore;
					}

					//切换庄家
					ChangeBanker(false);

					//设置时间
					m_dwJettonTime=(DWORD)time(NULL);
					m_pITableFrame->SetGameTimer(IDI_FREE,m_cbFreeTime*1000,1,0L);

					//发送消息
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
				//设置状态
				m_pITableFrame->SetGameStatus(GS_COMPARE_END);

				//猜大小时间
				OnCompareBigSmall();
				//设置时间
				m_dwJettonTime=(DWORD)time(NULL);
				m_pITableFrame->SetGameTimer(IDI_SMALL_BIG,m_cbCompareTime*1000,1,0L);
			}			

			return true;
		}
	case IDI_SMALL_BIG://开大小时间
		{			
			LONGLONG TempStartScore=0;
			for ( WORD wUserChairID = 0; wUserChairID < GAME_PLAYER; ++wUserChairID )
			{
				IServerUserItem *pIServerUserItem = m_pITableFrame->GetTableUserItem(wUserChairID);
				if ( pIServerUserItem == NULL||(QueryBuckleServiceCharge(wUserChairID)==false)) continue;

				//库存金币
				if (!pIServerUserItem->IsAndroidUser())
				{
					TempStartScore -= (m_lUserWinScore[wUserChairID]+m_lUserRevenue[wUserChairID]);
					m_lExchangeGold[wUserChairID]+=m_lUserReturnScore[wUserChairID];
				}
			}
			if(!(m_pServerContro!=NULL&&m_bControl))
				m_StorageStart+=TempStartScore*m_nExchange;

			//结束游戏
			m_pITableFrame->ConcludeGame(GS_GAME_FREE);

			if (m_wCurrentBanker!=INVALID_CHAIR)
				{
					m_lExchangeGold[m_wCurrentBanker]+=m_lBankerCurGameScore;
				}

			//切换庄家
			ChangeBanker(false);

			//设置时间
			m_dwJettonTime=(DWORD)time(NULL);
			m_pITableFrame->SetGameTimer(IDI_FREE,m_cbFreeTime*1000,1,0L);

			//发送消息
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

//游戏消息处理
bool CTableFrameSink::OnGameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem)
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
			CMD_C_PlaceJetton * pPlaceJetton=(CMD_C_PlaceJetton *)pData;
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
// 			//用户效验
// 			tagUserInfo * pUserData=pIServerUserItem->GetUserInfo();
// 			if (pUserData->cbUserStatus==US_LOOKON) return true;
// 			if (pUserData->wChairID!=m_wCurrentBanker) return true;
// 			if (m_cbLeftCardCount < 8) return true;
// 
// 			//设置变量
// 			m_bContiueCard=true;
// 
// 			//发送消息
// 			SendGameMessage(pUserData->wChairID,TEXT("设置成功，下一局将继续发牌！"));

			return true;
		}
	case SUB_C_CHECK_IMAGE:		//用户选择背景图
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

			//权限判断
			if(CUserRight::IsGameCheatUser(pIServerUserItem->GetUserRight())==false)
				return false;
			if (  m_pServerContro == NULL )
				return false;

			//记录操作
			CMD_C_ControlApplication* pControlApplication = (CMD_C_ControlApplication*)pData;
			CString strBuf;

			if (pControlApplication->cbControlAppType == C_CA_UPDATE)
			{			
				strBuf.Format(TEXT("重新设置下注区域:%d,设置控制次数:%d\n"), pControlApplication->cbControlArea,pControlApplication->cbControlTimes);
				AdminOperateInfo(strBuf,pIServerUserItem);
			}
			else if(pControlApplication->cbControlAppType == C_CA_SET)
			{
				strBuf.Format(TEXT("设置下注区域:%d,设置控制次数:%d\n"),pControlApplication->cbControlArea,pControlApplication->cbControlTimes);
				AdminOperateInfo(strBuf,pIServerUserItem);
			}
			return m_pServerContro->ServerControl(wSubCmdID, pData, wDataSize, pIServerUserItem, m_pITableFrame);
		}
	case SUB_C_STORAGE:
		{
			ASSERT(wDataSize==sizeof(CMD_C_FreshStorage));
			if(wDataSize!=sizeof(CMD_C_FreshStorage)) return false;

			//权限判断
			if(CUserRight::IsGameCheatUser(pIServerUserItem->GetUserRight())==false)
				return false;

			CMD_C_FreshStorage *FreshStorage=(CMD_C_FreshStorage *)pData;
			m_StorageStart=FreshStorage->lStorageStart;
			CString szBuf;
			szBuf.Format(TEXT("更改库存值为：%I64d\n"),m_StorageStart);
			AdminOperateInfo(szBuf,pIServerUserItem);

			return true;
		}
	case SUB_C_DEUCT:
		{
			ASSERT(wDataSize==sizeof(CMD_C_FreshDeuct));
			if(wDataSize!=sizeof(CMD_C_FreshDeuct)) return false;

			//权限判断
			if(CUserRight::IsGameCheatUser(pIServerUserItem->GetUserRight())==false)
				return false;

			CMD_C_FreshDeuct *FreshDeuct=(CMD_C_FreshDeuct *)pData;
			m_StorageDeduct=FreshDeuct->lStorageDeuct;

			CString szBuf;
			szBuf.Format(TEXT("更改衰减值为：%I64d\n"),m_StorageDeduct);
			AdminOperateInfo(szBuf,pIServerUserItem);
			return true;
		}
	case SUB_C_STORAGEINFO:
		{
			ASSERT(wDataSize==sizeof(CMD_C_StorageInfo));
			if(wDataSize!=sizeof(CMD_C_StorageInfo)) return false;

			//权限判断
			if(CUserRight::IsGameCheatUser(pIServerUserItem->GetUserRight())==false)
				return false;
			CMD_C_StorageInfo *pStorageInfo = (CMD_C_StorageInfo *)pData;
			CopyMemory(m_lStorageMax,pStorageInfo->lMaxStorage,sizeof(m_lStorageMax));
			CopyMemory(m_wStorageMul,pStorageInfo->wStorageMul,sizeof(m_wStorageMul));
			
			CString szBuf;
			szBuf.Format(TEXT("更改库存上限值以及系统库存超过上限时吐分概率，第一阶段：%I64d，吐分概率为：%d%%;第二阶段：%I64d，吐分概率为%d%%\n"),m_lStorageMax[0],m_wStorageMul[0],m_lStorageMax[1],m_wStorageMul[1]);
			AdminOperateInfo(szBuf,pIServerUserItem);

			return true;

		}
	case SUB_C_CHANGE_GOLD:
		{
			ASSERT(wDataSize==sizeof(CMD_C_ChangeGold));
			if(wDataSize!=sizeof(CMD_C_ChangeGold)) return false;
			
			//消息处理
			CMD_C_ChangeGold * pChangeGold=(CMD_C_ChangeGold *)pData;
			//用户效验
			tagUserInfo * pUserData=pIServerUserItem->GetUserInfo();
			
			return OnUserChangeGold(pUserData->wChairID,pChangeGold->lChangeGold);
		}
	case SUB_C_GUESS:
		{
			ASSERT(wDataSize==sizeof(CMD_C_Guess));
			if(wDataSize!=sizeof(CMD_C_Guess)) return false;

			//消息处理
			CMD_C_Guess * pGuess=(CMD_C_Guess *)pData;
			//用户效验
			tagUserInfo * pUserData=pIServerUserItem->GetUserInfo();

			return OnUserGuess(pUserData->wChairID,pGuess->bBig);
		}
	}

	return false;
}

//框架消息处理
bool  CTableFrameSink::OnFrameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	return false;
}

//数据事件
bool CTableFrameSink::OnGameDataBase(WORD wRequestID, VOID * pData, WORD wDataSize)
{
	return false;
}

//用户坐下
bool CTableFrameSink::OnActionUserSitDown(WORD wChairID,IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	//设置时间
	if ((bLookonUser==false)&&(m_dwJettonTime==0L))
	{
		m_dwJettonTime=(DWORD)time(NULL);
		m_pITableFrame->SetGameTimer(IDI_FREE,m_cbFreeTime*1000,1,NULL);
		m_pITableFrame->SetGameStatus(GS_GAME_FREE);
	}

	////限制提示
	//TCHAR szTipMsg[128];
	//_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("本房间上庄条件为：%I64d,区域限制为：%I64d,玩家限制为：%I64d"),m_lApplyBankerCondition,
	//	m_lAreaLimitScore,m_lUserLimitScore);
	//m_pITableFrame->SendGameMessage(pIServerUserItem,szTipMsg,SMT_CHAT);

	return true;
}

//用户起来
bool CTableFrameSink::OnActionUserStandUp(WORD wChairID,IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	//记录成绩
	if (bLookonUser==false)
	{
		//切换庄家
		if (pIServerUserItem->GetChairID()==m_wCurrentBanker)
		{
			ChangeBanker(true);
			m_bContiueCard=false;
		}

		//取消申请
		for (WORD i=0; i<m_ApplyUserArray.GetCount(); ++i)
		{
			if (pIServerUserItem->GetChairID()!=m_ApplyUserArray[i]) continue;

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

//加注事件
bool CTableFrameSink::OnUserPlaceJetton(WORD wChairID, BYTE cbJettonArea, LONGLONG lJettonScore)
{
	//效验参数
	ASSERT((cbJettonArea<=AREA_COUNT+1 && cbJettonArea>=1)&&(lJettonScore>0L));
	if ((cbJettonArea>=AREA_COUNT+1)||(lJettonScore<=0L) || cbJettonArea<1 || lJettonScore>m_lExchangeGold[wChairID] ||
		(m_lUserJettonScore[cbJettonArea][wChairID]+lJettonScore) <0L || (m_lAllJettonScore[cbJettonArea]+lJettonScore)<0L )
		{
			return false;
		}

	//下注合法性检测 
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

			//发送消息
			m_pITableFrame->SendTableData(wChairID,SUB_S_PLACE_JETTON_FAIL,&PlaceJettonFail,sizeof(PlaceJettonFail));
			return true;
		}

	//庄家判断
	if (m_wCurrentBanker==wChairID) return true;
	if (m_bEnableSysBanker==false && m_wCurrentBanker==INVALID_CHAIR) return true;

	//变量定义
		//IServerUserItem * pIServerUserItem=m_pITableFrame->GetTableUserItem(wChairID);
	LONGLONG lJettonCount=0L;
	for (int nAreaIndex=1; nAreaIndex<=AREA_COUNT; ++nAreaIndex) lJettonCount += m_lUserJettonScore[nAreaIndex][wChairID];
	//玩家积分
	LONGLONG lUserScore = m_lExchangeGold[wChairID];

	//成功标识
	bool bPlaceJettonSuccess=true;
	//合法校验
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
	//如果单项下注综合超过庄家赔率的话 下注失败
	if(m_wCurrentBanker != INVALID_CHAIR)
		{
			if((m_lAllJettonScore[cbJettonArea]+lJettonScore) > m_lExchangeGold[m_wCurrentBanker]/diMultiple[cbJettonArea-1])
			{
				bPlaceJettonSuccess=false;
			}
		}	
	//合法验证
	if (GetUserMaxJetton(wChairID,cbJettonArea) >= lJettonScore)
		{
			//机器人验证
			if(pIServerUserItem->IsAndroidUser())
			{
				//区域限制
				if (m_lRobotAreaScore[cbJettonArea] + lJettonScore > m_lRobotAreaLimit)
					return true;

				//数目限制
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

				//统计分数
				if (bPlaceJettonSuccess)
					m_lRobotAreaScore[cbJettonArea] += lJettonScore;
			}

			if (bPlaceJettonSuccess)
			{
				//保存下注
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
			//变量定义
			CMD_S_PlaceJetton PlaceJetton;
			ZeroMemory(&PlaceJetton,sizeof(PlaceJetton));

			//构造变量
			PlaceJetton.wChairID=wChairID;
			PlaceJetton.cbJettonArea=cbJettonArea;
			PlaceJetton.lJettonScore=lJettonScore;

			//获取用户
			IServerUserItem * pIServerUserItem=m_pITableFrame->GetTableUserItem(wChairID);
			if (pIServerUserItem != NULL) 
				PlaceJetton.cbAndroid = pIServerUserItem->IsAndroidUser()? TRUE : FALSE;
			PlaceJetton.bAndroid = pIServerUserItem->IsAndroidUser()? TRUE : FALSE;

			//发送消息
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

			//发送消息
			m_pITableFrame->SendTableData(wChairID,SUB_S_PLACE_JETTON_FAIL,&PlaceJettonFail,sizeof(PlaceJettonFail));
		}

	return true;
}
void CTableFrameSink::RandList(BYTE cbCardBuffer[], BYTE cbBufferCount)
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


//发送扑克
bool CTableFrameSink::DispatchTableCard()
{
	ZeroMemory(m_cbTableCardArray,sizeof(m_cbTableCardArray));
	m_nProbability=0;
	m_nRand=0;
	//1,13大橘子、2,14大铃铛、24小铃铛,3小王，4大王、5,11,17,23大苹果、6小苹果、7,19大柠檬、8大西瓜、9小西瓜、10绿LUCK、12小橘子、15小双7、16大双7、18小柠檬、20大星星、21小星星、22红LUCK
	INT cbControlArea[24] = { 1, 13,	2, 14, 24,    3,   4,	5, 11, 17, 23,   6,		7, 19,		8,   9,		10,		12,		15,   16, 18, 20, 21,  22};
	INT cbnChance[24]	  = { 6, 6,     5, 5,  7,     1,   1,	4,  4, 4,  4,	 7,		6, 6,		7,   7,		m_nLucky,		7,		7,	   7,  7,  7,  7,  m_nLucky};

	m_GameLogic.ChaosArray(cbControlArea, CountArray(cbControlArea), cbnChance, CountArray(cbnChance));

	//随机倍数
	DWORD wTick = GetTickCount();

	//几率和值
	INT nChanceAndValue = 0;
	for ( int n = 0; n < CountArray(cbnChance); ++n )
		nChanceAndValue += cbnChance[n];

	INT nMuIndex = 0;
	int nRandNum = 0;					//随机辅助
	
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


	if(m_cbTableCardArray[0]==10||m_cbTableCardArray[0]==22)//绿LUCK,红LUCK
	{
		m_nProbability=rand()%5+1;		
		if(m_nProbability==1)//打枪
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
		if(m_nProbability==2)//大三元
		{
			m_cbTableCardArray[1]=8;
			m_cbTableCardArray[2]=20;
			m_cbTableCardArray[3]=16;
		}
		if(m_nProbability==3)//小三元
		{
			m_cbTableCardArray[1]=1;
			m_cbTableCardArray[2]=7;
			m_cbTableCardArray[3]=14;
		}
		if(m_nProbability==4)//大四喜
		{
			m_cbTableCardArray[1]=5;
			m_cbTableCardArray[2]=11;
			m_cbTableCardArray[3]=17;
			m_cbTableCardArray[4]=23;
		}
		if(m_nProbability==5)//通吃
		{

		}
	}	

	//发牌标志
	m_bContiueCard = false;

	return true;
}

//兑换筹码
bool CTableFrameSink::OnUserChangeGold(WORD wChairID,LONGLONG lGold)
{
	ASSERT((wChairID!=INVALID_CHAIR)||(lGold>0L));

	//兑换筹码合法性校验 
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
	//构造变量
	CMD_S_ChangeGold ChangeGold;
	ZeroMemory(&ChangeGold,sizeof(ChangeGold));

	//设置变量
	CopyMemory(ChangeGold.lExchangeGold,m_lExchangeGold,sizeof(m_lExchangeGold));
	ChangeGold.wChairID = wChairID;

	//发送消息
	m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_CHANGE_GOLD, &ChangeGold, sizeof(ChangeGold));
	m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_CHANGE_GOLD, &ChangeGold, sizeof(ChangeGold));


	return true;
}

//猜大小
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

	//构造变量
	CMD_S_Guess Guess;
	ZeroMemory(&Guess,sizeof(Guess));

	Guess.nGuessNum=nCount;
	Guess.wChairID = wChairID;
	Guess.lUserReturnScore=m_lUserReturnScore[wChairID];	
	Guess.nBankerTime=m_wBankerTime;

	//发送消息
	m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_GUESS, &Guess, sizeof(Guess));
	m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_GUESS, &Guess, sizeof(Guess));
	

	return true;
}

//申请庄家
bool CTableFrameSink::OnUserApplyBanker(IServerUserItem *pIApplyServerUserItem)
{

	//合法判断
	LONGLONG lUserScore=m_lExchangeGold[pIApplyServerUserItem->GetChairID()];
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
	if (m_pITableFrame->GetGameStatus()==GS_GAME_FREE && m_ApplyUserArray.GetCount()==1)
		{
			ChangeBanker(false);
		}

	

	return true;
}

//取消申请
bool CTableFrameSink::OnUserCancelBanker(IServerUserItem *pICancelServerUserItem)
{
	//当前庄家
	if (pICancelServerUserItem->GetChairID()==m_wCurrentBanker && m_pITableFrame->GetGameStatus()!=GS_GAME_FREE)
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
		TakeTurns();

		//设置变量
		bChangeBanker=true;
		m_bExchangeBanker = true;
	}
	//轮庄判断
	else if (m_wCurrentBanker!=INVALID_CHAIR)
	{
		//获取庄家
		IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(m_wCurrentBanker);

		if(pIServerUserItem!= NULL)
		{
			LONGLONG lBankerScore=m_lExchangeGold[m_wCurrentBanker];

			//次数判断
			if (m_lPlayerBankerMAX<=m_wBankerTime || lBankerScore<m_lApplyBankerCondition)
			{
				//庄家增加判断 同一个庄家情况下只判断一次
				if(m_lPlayerBankerMAX <= m_wBankerTime && m_bExchangeBanker && lBankerScore>=m_lApplyBankerCondition)
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
				TakeTurns();

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
		}

	}
	//系统做庄
	else if (m_wCurrentBanker==INVALID_CHAIR && m_ApplyUserArray.GetCount()!=0)
	{
		//轮换判断
		TakeTurns();

		bChangeBanker=true;
		m_bExchangeBanker = true;
	}

	//切换判断
	if (bChangeBanker)
	{
		//最大坐庄数
		m_lPlayerBankerMAX = m_lBankerMAX;

		//设置变量
		m_wBankerTime = 0;
		m_lBankerWinScore=0;

		//发送消息
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

//轮换判断
void CTableFrameSink::TakeTurns()
{
	//变量定义
	int nInvalidApply = 0;

	for (int i = 0; i < m_ApplyUserArray.GetCount(); i++)
	{
		if (m_pITableFrame->GetGameStatus() == GS_GAME_FREE)
		{
			//获取分数
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

					//发送消息
					CMD_S_CancelBanker CancelBanker = {};

					//设置变量
					CancelBanker.wCancelUser = pIServerUserItem->GetChairID();					

					//发送消息
					m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));
					m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));

					//提示消息
					TCHAR szTipMsg[128] = {};
					_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("由于您的筹码（%I64d）少于坐庄必须筹码数（%I64d）,你无法上庄！"),
						pIServerUserItem->GetUserScore(), m_lApplyBankerCondition);
					SendGameMessage(m_ApplyUserArray[i],szTipMsg);
				}
			}
		}
	}

	//删除玩家
	if (nInvalidApply != 0)
		m_ApplyUserArray.RemoveAt(0, nInvalidApply);
}

//发送庄家
void CTableFrameSink::SendApplyUser( IServerUserItem *pRcvServerUserItem )
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
bool  CTableFrameSink::OnActionUserOffLine(WORD wChairID,IServerUserItem * pIServerUserItem)
{
	//切换庄家
	if (pIServerUserItem->GetChairID()==m_wCurrentBanker) ChangeBanker(true);

	//取消申请
	for (WORD i=0; i<m_ApplyUserArray.GetCount(); ++i)
	{
		if (pIServerUserItem->GetChairID()!=m_ApplyUserArray[i]) continue;

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
//最大下注
LONGLONG CTableFrameSink::GetUserMaxJetton(WORD wChairID,BYTE Area)
{
	IServerUserItem *pIMeServerUserItem=m_pITableFrame->GetTableUserItem(wChairID);
	if (NULL==pIMeServerUserItem) return 0L;


	//已下注额
	LONGLONG lNowJetton = 0;
	ASSERT(AREA_COUNT<=CountArray(m_lUserJettonScore));
	for (int nAreaIndex=1; nAreaIndex<=AREA_COUNT; ++nAreaIndex) lNowJetton += m_lUserJettonScore[nAreaIndex][wChairID];

	//庄家金币
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

	//个人限制
	LONGLONG lMeMaxScore = min((m_lExchangeGold[wChairID]), m_lUserLimitScore);

	//区域限制
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
	//庄家限制
	lMeMaxScore=min(lMeMaxScore,lBankerScore/(diMultiple[Area-1]));

	//非零限制
	ASSERT(lMeMaxScore >= 0);
	lMeMaxScore = max(lMeMaxScore, 0);

	return (LONGLONG)(lMeMaxScore);
}
//计算得分
LONGLONG CTableFrameSink::CalculateScore()
{
	//变量定义
	LONGLONG static cbRevenue=m_pGameServiceOption->wRevenueRatio;

	//推断玩家
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

	//游戏记录
	tagServerGameRecord &GameRecord = m_GameRecordArrary[m_nRecordLast];

	BYTE  cbMultiple[AREA_COUNT]={1};

	for (WORD wAreaIndex = 0; wAreaIndex <= AREA_COUNT; ++wAreaIndex)
	{		
		GameRecord.bWinMen[wAreaIndex] = cbTempResulteOut[wAreaIndex];		
	}

	//移动下标
	m_nRecordLast = (m_nRecordLast+1) % MAX_SCORE_HISTORY;
	if ( m_nRecordLast == m_nRecordFirst ) m_nRecordFirst = (m_nRecordFirst+1) % MAX_SCORE_HISTORY;

	//庄家总量
	LONGLONG lBankerWinScore = 0;
	m_lBankerCurGameScore = 0;

	//玩家成绩
	ZeroMemory(m_lUserTempScore, sizeof(m_lUserTempScore));
	ZeroMemory(m_lUserReturnScore, sizeof(m_lUserReturnScore));
	ZeroMemory(m_lUserRevenue, sizeof(m_lUserRevenue));
	LONGLONG lUserLostScore[GAME_PLAYER];
	ZeroMemory(lUserLostScore, sizeof(lUserLostScore));

	//玩家下注
	LONGLONG *pUserScore[AREA_COUNT+1];
	pUserScore[0]=NULL;
	for (int i = 1;i<=AREA_COUNT;i++)
	{
		pUserScore[i]=m_lUserJettonScore[i];
	}

	//计算积分
	ZeroMemory(m_lUserEveryTurnScore,sizeof(m_lUserEveryTurnScore));
	for (WORD wChairID=0; wChairID<GAME_PLAYER; wChairID++)
	{
		//庄家判断
		if (m_wCurrentBanker==wChairID) continue;

		//获取用户
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

		//计算税收
		/*if (0 < m_lUserWinScore[wChairID])
		{
			double fRevenuePer=double(cbRevenue/1000.0);
			m_lUserRevenue[wChairID]  = LONGLONG(m_lUserWinScore[wChairID]*fRevenuePer);
			m_lUserWinScore[wChairID] -= m_lUserRevenue[wChairID];
		}*/

		//总的分数
		m_lUserWinScore[wChairID] += lUserLostScore[wChairID];
		m_lUserEveryTurnScore[wChairID] += lUserLostScore[wChairID];
	}
	
	//庄家成绩
	if (m_wCurrentBanker!=INVALID_CHAIR)
	{
		m_lUserWinScore[m_wCurrentBanker] += lBankerWinScore;

		//计算税收
		if (0 < m_lUserWinScore[m_wCurrentBanker])
		{
			double fRevenuePer=double(cbRevenue/1000.0);
			m_lUserRevenue[m_wCurrentBanker]  = LONGLONG(m_lUserWinScore[m_wCurrentBanker]*fRevenuePer);
			m_lUserWinScore[m_wCurrentBanker] -= m_lUserRevenue[m_wCurrentBanker];
			lBankerWinScore = m_lUserWinScore[m_wCurrentBanker];
		}

		m_lUserEveryTurnScore[m_wCurrentBanker]=m_lUserWinScore[m_wCurrentBanker];
	}

	//累计积分
	m_lBankerWinScore += lBankerWinScore;

	//当前积分
	m_lBankerCurGameScore=lBankerWinScore;

	return lBankerWinScore;
}

//试探性判断
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

	//系统输赢
	LONGLONG lSystemScore = 0l;

	//玩家下注
	LONGLONG *pUserScore[AREA_COUNT+1];
	pUserScore[0] = NULL;
	for (int i = 1;i<=AREA_COUNT;i++)
	{
		pUserScore[i] = m_lUserJettonScore[i];
	}

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
	//库存封顶有比例系统故意输
	if(m_StorageStart>m_lStorageMax[0])
	{
		//排除LUCK 方便计算
		if(m_cbTableCardArray[0]==10||m_cbTableCardArray[0]==22)
			return false;
		srand(GetTickCount());

		LONGLONG lAndroidJettonScore[AREA_COUNT+1];			//机器人每个区域总下注
		LONGLONG lAndroidSumScore = 0l;						//机器人总下注
		LONGLONG lUserJettonScore[AREA_COUNT+1];			//玩家每个区域总下注
		LONGLONG lUserSumScore=0l;							//玩家总下注
		WORD	 wHitArea;									//命中区域

		ZeroMemory(lAndroidJettonScore,sizeof(lAndroidJettonScore));
		ZeroMemory(lUserJettonScore,sizeof(lUserJettonScore));

		//统计积分
		for (WORD wAreaIndex = 1; wAreaIndex <= AREA_COUNT; ++wAreaIndex)
		{
			for (WORD wChairID=0; wChairID<GAME_PLAYER; wChairID++)
			{
				//庄家判断
				if (m_wCurrentBanker == wChairID) continue;

				//获取用户
				IServerUserItem * pIServerUserItem=m_pITableFrame->GetTableUserItem(wChairID);
				if (pIServerUserItem==NULL) 
					continue;

				if(pIServerUserItem->IsAndroidUser())
				{
					//机器人下注
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
		//只有玩家下注了才会进行系统输分计算
		if(lUserSumScore > 0)
		{
			BYTE  cbTempResulteOut[AREA_COUNT+1];
			memset(cbTempResulteOut,0,AREA_COUNT+1);

			//如果玩家坐庄
			if(bIsBankerAndroidUser == false && m_wCurrentBanker != INVALID_CHAIR)
			{
				//命中区域系统赢分的话 有几率让系统故意输分
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
				//命中区域系统赢分的话 有几率让系统故意输分
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

	//计算积分
	for (WORD wChairID=0; wChairID<GAME_PLAYER; wChairID++)
	{
		//庄家判断
		if (m_wCurrentBanker == wChairID) continue;

		//获取用户
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

	//防止上把控制使得库存为负，而这里无论什么情况即使系统赢了也会使最终库存扔处于负值而导致进入死循环
	if ((lSystemScore*m_nExchange + m_StorageStart) < 0l && lSystemScore<0l)
	{
		return false;
	}
	else
	{
		m_StorageStart += lSystemScore*m_nExchange;
		//只有库存大于0才会衰减
		if(m_StorageStart > 0 && NeedDeductStorage())
			m_StorageStart = m_StorageStart - (m_StorageStart * m_StorageDeduct / 1000);
		return true;
	}
}

//发送记录
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

//发送消息
void CTableFrameSink::SendGameMessage(WORD wChairID, LPCTSTR pszTipMsg)
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

//读取配置
void CTableFrameSink::ReadConfigInformation(bool bReadFresh)
{
	//获取自定义配置
	tagCustomConfig *pCustomConfig = (tagCustomConfig *)m_pITableFrame->GetCustomRule();
	ASSERT(pCustomConfig);
	//时间变量
	m_cbFreeTime=pCustomConfig->CustomGeneral.cbFreeTime;
	if (m_cbFreeTime<5 || m_cbFreeTime>99) m_cbFreeTime=5;
	m_cbBetTime=pCustomConfig->CustomGeneral.cbBetTime;
	if(m_cbBetTime<10 || m_cbBetTime>99) m_cbBetTime=10;
	m_cbEndTime=pCustomConfig->CustomGeneral.cbEndTime;	
	if(m_cbEndTime<20 || m_cbEndTime>99) m_cbEndTime=20;
	m_cbCompareTime=pCustomConfig->CustomGeneral.cbGuessTime;
	//上庄控制
	m_lApplyBankerCondition=pCustomConfig->CustomGeneral.lApplyBankerCondition;
	m_bEnableSysBanker=pCustomConfig->CustomGeneral.nEnableSysBanker;
	m_lBankerMAX=pCustomConfig->CustomGeneral.lBankerTime;
	m_lBankerAdd=pCustomConfig->CustomGeneral.lBankerTimeAdd;
	m_lBankerScoreMAX=pCustomConfig->CustomGeneral.lBankerScoreMAX;
	m_lBankerScoreAdd=pCustomConfig->CustomGeneral.lBankerTimeExtra;
	//下注限制
	m_lAreaLimitScore=pCustomConfig->CustomGeneral.lAreaLimitScore;
	m_lUserLimitScore=pCustomConfig->CustomGeneral.lUserLimitScore;

	//机器人控制
	//机器人下注人数
	m_nMaxChipRobot=pCustomConfig->CustomAndroid.lRobotBetMinCount+rand()%(pCustomConfig->CustomAndroid.lRobotBetMaxCount-pCustomConfig->CustomAndroid.lRobotBetMinCount+1);

	m_lRobotAreaLimit=pCustomConfig->CustomAndroid.lRobotAreaLimit;
	m_nRobotListMaxCount=pCustomConfig->CustomAndroid.lRobotListMaxCount;
	//筹码兑换
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
//银行操作
#ifdef __SPECIAL___
bool __cdecl CTableFrameSink::OnActionUserBank(WORD wChairID, IServerUserItem * pIServerUserItem)
{
	return true;
}
#endif

//查询是否扣服务费
bool CTableFrameSink::QueryBuckleServiceCharge(WORD wChairID)
{
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		IServerUserItem *pUserItem=m_pITableFrame->GetTableUserItem(i);
		if(pUserItem==NULL) continue;
		if(wChairID==i)
		{
			//返回下注
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

//是否衰减
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

// 管理员操作日志
void CTableFrameSink::AdminOperateInfo( LPCTSTR pszString,IServerUserItem *pIServerUserItem)
{
	//设置语言区域
	char* old_locale = _strdup( setlocale(LC_CTYPE,NULL) );
	setlocale( LC_CTYPE, "chs" );

	CTime Time(CTime::GetCurrentTime());
	//转化地址
	CString strBuf;
	TCHAR szClientAddr[16]=TEXT("");
	DWORD dwAddr = pIServerUserItem->GetClientAddr();
	BYTE * pClientAddr=(BYTE *)&dwAddr;
	_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		
	strBuf.Format(TEXT("时间: %d-%d-%d %d:%d:%d | 管理员:%s ID:%d | 操作IP:%s 机器码:%s\n"),
		Time.GetYear(), Time.GetMonth(), Time.GetDay(), Time.GetHour(), Time.GetMinute(), Time.GetSecond(),
		pIServerUserItem->GetNickName(),pIServerUserItem->GetUserID(),szClientAddr,pIServerUserItem->GetMachineID());

	strBuf.Format(TEXT("%s    %s"),strBuf,pszString);

	CStdioFile AdminFile;
	CString strFileName;
	strFileName.Format(TEXT("水果机操作日志.txt"));

	BOOL bOpen = AdminFile.Open(strFileName, CFile::modeReadWrite|CFile::modeCreate|CFile::modeNoTruncate);
	if ( bOpen )
	{	
		AdminFile.SeekToEnd();
		AdminFile.WriteString( strBuf );
		AdminFile.Flush();
		AdminFile.Close();
	}

	//还原区域设定
	setlocale( LC_CTYPE, old_locale );
	free( old_locale );
}
// 库存日志
void CTableFrameSink::StorageLog()
{
	//设置语言区域
	char* old_locale = _strdup( setlocale(LC_CTYPE,NULL) );
	setlocale( LC_CTYPE, "chs" );

	CTime Time(CTime::GetCurrentTime());
	CString strBuf;
	strBuf.Format(TEXT("时间: %d-%d-%d %d:%d:%d | 库存:%I64d\n"),
		Time.GetYear(), Time.GetMonth(), Time.GetDay(), Time.GetHour(), Time.GetMinute(), Time.GetSecond(),m_StorageStart);
	
	CStdioFile StorageFile;
	CString strFileName;
	strFileName.Format(TEXT("水果机库存日志.txt"));

	BOOL bOpen = StorageFile.Open(strFileName, CFile::modeReadWrite|CFile::modeCreate|CFile::modeNoTruncate);
	if ( bOpen )
	{	
		StorageFile.SeekToEnd();
		StorageFile.WriteString( strBuf );
		StorageFile.Flush();
		StorageFile.Close();
	}

	//还原区域设定
	setlocale( LC_CTYPE, old_locale );
	free( old_locale );
}

//////////////////////////////////////////////////////////////////////////
