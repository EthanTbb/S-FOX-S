#include "StdAfx.h"
#include "TableFrameSink.h"


//////////////////////////////////////////////////////////////////////////
#define IDI_CHECK_TABLE				1					//检查桌子
//#define IDI_DELAY					2					//延时ID

///////////////////////////////////////////////////////////////////////////
//变量定义
#define			ROBOT_LNVENTORY_MIN					9223372036854775808			
#define			ROBOT_LNVENTORY_MAX					9223372036854775809	

//////////////////////////////////////////////////////////////////////////
//构造函数
CTableFrameSink::CTableFrameSink()
{
	//组件变量
	m_pITableFrame=NULL;
	m_pGameServiceOption=NULL;
	m_pGameCustomRule=NULL;
	m_cbWaitTime = 0;
	//游戏变量
	ZeroMemory(m_lUserChip, sizeof(m_lUserChip));
	ZeroMemory(m_lScoreTimes, sizeof(m_lScoreTimes));
	ZeroMemory(m_bSpecialType, sizeof(m_bSpecialType));
	ZeroMemory(m_bUserCardData , sizeof(m_bUserCardData)) ;
	ZeroMemory(m_bSegmentCard , sizeof(m_bSegmentCard)) ;
	
	
	//用户变量
	m_wBanker = INVALID_CHAIR ;
	m_wFirstCallBankerUser = INVALID_CHAIR ;
	m_wCurrentCaller = INVALID_CHAIR;

	//计数变量
	m_wShuffleFinishCount = 0;
	m_wDispatchFinishCount = 0;
	m_wSpliFinishCount = 0;	
	m_wShowCardCount = 0; 
	m_wCompareCount = 0;
	m_wCompleteCompareCount = 0;
	m_wSetChipCount=0;
	m_bCompared = true;

	//状态变量
	ZeroMemory(m_bGameStatus, sizeof(m_bGameStatus));
	ZeroMemory(m_bFinishSegment , sizeof(m_bFinishSegment)) ;
	ZeroMemory(m_bIsReady, sizeof(m_bIsReady));

	//控制变量
	m_bNeedControl = false;
	ZeroMemory(m_bMysteryUser, sizeof(m_bMysteryUser));

	srand(time(NULL));

	//服务控制
	m_hControlInst = NULL;
	m_pServerControl = NULL;
	m_hControlInst = LoadLibrary(TEXT("ThirteenServerControl.dll"));
	if ( m_hControlInst )
	{
		typedef void * (*CREATE)(); 
		CREATE ServerControl = (CREATE)GetProcAddress(m_hControlInst,"CreateServerControl"); 
		if ( ServerControl )
		{
			m_pServerControl = static_cast<IServerControl*>(ServerControl());
		}
	}

	m_sRobotLnventory = 0;
	return;
}

//析构函数
CTableFrameSink::~CTableFrameSink(void)
{
	if( m_pServerControl )
	{
		delete m_pServerControl;
		m_pServerControl = NULL;
	}

	if( m_hControlInst )
	{
		FreeLibrary(m_hControlInst);
		m_hControlInst = NULL;
	}
}

//接口查询
VOID * CTableFrameSink::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(ITableFrameSink,Guid,dwQueryVer);
	QUERYINTERFACE(ITableUserAction,Guid,dwQueryVer);
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

	//获取参数
	m_pGameServiceOption=m_pITableFrame->GetGameServiceOption();
	ASSERT(m_pGameServiceOption!=NULL);

	//自定规则
	ASSERT(m_pITableFrame->GetCustomRule()!=NULL);
	m_pGameCustomRule=(tagCustomRule *)m_pITableFrame->GetCustomRule();

	return true;
}

//复位桌子
VOID CTableFrameSink::RepositionSink()
{
	//游戏变量
	ZeroMemory(m_lUserChip, sizeof(m_lUserChip));
	ZeroMemory(m_lScoreTimes, sizeof(m_lScoreTimes));
	ZeroMemory(m_bSpecialType, sizeof(m_bSpecialType));
	ZeroMemory(m_bUserCardData , sizeof(m_bUserCardData)) ;
	ZeroMemory(m_bSegmentCard , sizeof(m_bSegmentCard)) ;

	//计数变量
	m_cbWaitTime = 0;
	m_wShuffleFinishCount = 0;
	m_wDispatchFinishCount = 0;
	m_wSpliFinishCount = 0;	
	m_wShowCardCount=0 ; 
	m_wCompleteCompareCount=0 ;
	m_wSetChipCount=0 ;
	m_bCompared = true;

	//状态变量
	ZeroMemory(m_bGameStatus, sizeof(m_bGameStatus));
	ZeroMemory(m_bFinishSegment , sizeof(m_bFinishSegment));
	ZeroMemory(m_bIsReady , sizeof(m_bIsReady)) ;

	return;
}

//消费能力
SCORE CTableFrameSink::QueryConsumeQuota(IServerUserItem * pIServerUserItem)
{
	//效验状态
	ASSERT((pIServerUserItem!=NULL)&&(pIServerUserItem->GetTableID()==m_pITableFrame->GetTableID()));
	if ((pIServerUserItem==NULL)||(pIServerUserItem->GetTableID()!=m_pITableFrame->GetTableID())) return 0;

	//用户判断
	switch (pIServerUserItem->GetUserStatus())
	{
	case US_SIT:		//坐下状态
	case US_READY:		//准备状态
		{
			return pIServerUserItem->GetUserScore()-m_pGameServiceOption->lMinTableScore;
		}
	case US_PLAYING:	//游戏状态
	case US_OFFLINE:	//断线状态
		{
			return pIServerUserItem->GetUserScore()-m_pGameServiceOption->lCellScore;
		}
	case US_LOOKON:		//旁观状态
		{
			return pIServerUserItem->GetUserScore();
		}
	}

	return 0L;
}

//最少积分
SCORE CTableFrameSink::QueryLessEnterScore(WORD wChairID, IServerUserItem * pIServerUserItem)
{
	return 0L;
}

//游戏开始
bool CTableFrameSink::OnEventGameStart()
{
	//游戏变量
	ZeroMemory(m_lUserChip, sizeof(m_lUserChip));
	ZeroMemory(m_lScoreTimes, sizeof(m_lScoreTimes));
	ZeroMemory(m_bSpecialType, sizeof(m_bSpecialType));
	ZeroMemory(m_bUserCardData , sizeof(m_bUserCardData)) ;
	ZeroMemory(m_bSegmentCard , sizeof(m_bSegmentCard)) ;
	ZeroMemory(m_bFinishSegment , sizeof(m_bFinishSegment)) ;
	ZeroMemory(m_bGameStatus,sizeof(m_bGameStatus));
	m_wShowCardCount=0 ; 
	m_wCompleteCompareCount=0 ;
	m_wShowCardCount=0 ;
	m_wCompareCount=0;
	m_wSetChipCount=0;

	//设置状态
	m_pITableFrame->SetGameStatus(GS_WK_CALL_BANKER);

	CMD_S_StartCallBanker  StartCallBanker ;
	ZeroMemory(&StartCallBanker, sizeof(StartCallBanker)) ;

	//设置状态
	for(BYTE i=0;i<GAME_PLAYER;i++)
	{
		IServerUserItem *pServerUserItem = m_pITableFrame->GetTableUserItem( i ) ;
		if (pServerUserItem != NULL) m_bGameStatus[i] = true;
	}

	//首次叫庄
	if ( m_wFirstCallBankerUser == INVALID_CHAIR )
	{
		//在座玩家
		WORD wSitDownUser[GAME_PLAYER] ;
		BYTE cbSitDownUsercount = 0 ;
		for ( WORD wChairID = 0; wChairID < GAME_PLAYER; ++wChairID )
		{
			IServerUserItem *pServerUserItem = m_pITableFrame->GetTableUserItem( wChairID ) ;
			if ( pServerUserItem ) wSitDownUser[cbSitDownUsercount++] = wChairID ;
		}

		//叫庄玩家
		BYTE cbCallerIndex = rand() % cbSitDownUsercount;

		m_wFirstCallBankerUser = wSitDownUser[cbCallerIndex] ;
	}
	else
	{
		//轮换庄家
		while ( true )
		{
			m_wFirstCallBankerUser = (m_wFirstCallBankerUser + 1) % GAME_PLAYER ;
			if (m_pITableFrame->GetTableUserItem(m_wFirstCallBankerUser) != NULL) break ;
		}
	}
	
	StartCallBanker.wCurrentCaller = m_wFirstCallBankerUser ;
	CopyMemory(StartCallBanker.bGameStatus, m_bGameStatus, sizeof(m_bGameStatus));
	m_wCurrentCaller = m_wFirstCallBankerUser ;

	////发送消息
	//m_pITableFrame->SendTableData( INVALID_CHAIR, SUB_S_START_CALL_BANKER, &StartCallBanker, sizeof( StartCallBanker ) ) ;
	//m_pITableFrame->SendLookonData( INVALID_CHAIR, SUB_S_START_CALL_BANKER, &StartCallBanker, sizeof( StartCallBanker ) ) ;

	//洗牌发牌
	BYTE bCardData[52] ;
	m_GameLogic.RandCardList(bCardData , 52) ;
	CopyMemory(m_bUserCardData, bCardData, sizeof(BYTE)*52);
	

	//发送作弊
	if(!IsNeedControl())
	{
		SendCheatCard();
	}

	m_pITableFrame->SetGameTimer(IDI_CHECK_TABLE,30000,-1,NULL);
	m_cbWaitTime=0;
	
	_DebugOutPut(TEXT("Start The Game TableID:%d UserCount:%d \r\n ")
		, m_pITableFrame->GetTableID(), GetPlayerCount());

	//发送与机器人叫庄有关的控制消息
	CMD_A_Control Control;
	Control.bIsWin = rand() % 2;
	Control.bPlayCount = 0;
	for(int i = 0; i < GAME_PLAYER; i++)
	{
		if (m_pITableFrame->GetTableUserItem(i) != NULL)
		{
			Control.bPlayCount ++;
		}
	}
	m_pITableFrame->SendTableData( INVALID_CHAIR, SUB_S_ROBOT_CONTROL, &Control, sizeof( Control ) ) ;
	m_pITableFrame->SendLookonData( INVALID_CHAIR, SUB_S_ROBOT_CONTROL, &Control, sizeof( Control ) ) ;
	
	//发送消息
	m_pITableFrame->SendTableData( INVALID_CHAIR, SUB_S_START_CALL_BANKER, &StartCallBanker, sizeof( StartCallBanker ) ) ;
	m_pITableFrame->SendLookonData( INVALID_CHAIR, SUB_S_START_CALL_BANKER, &StartCallBanker, sizeof( StartCallBanker ) ) ;

	return true;
}

//游戏结束
bool CTableFrameSink::OnEventGameConclude(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)
{

	_DebugOutPut(TEXT("The Game To End Reason: %d TableID:%d \r\n "), cbReason, m_pITableFrame->GetTableID());
	if (pIServerUserItem != NULL)
	{
		_DebugOutPut(TEXT("UserName %s: ChairID: UserStatus:%d  \r\n "),
			pIServerUserItem->GetNickName(), wChairID, pIServerUserItem->GetUserStatus());
	}
	
	
	m_pITableFrame->KillGameTimer(IDI_CHECK_TABLE);
	m_cbWaitTime = 0;

	switch (cbReason)
	{
	case GER_DISMISS:			//游戏解散
		{
			//效验参数
		/*	ASSERT(pIServerUserItem!=NULL);
			ASSERT(wChairID<GAME_PLAYER);*/

			//构造数据
			CMD_S_GameEnd GameEnd;
			ZeroMemory(&GameEnd, sizeof(GameEnd));

			GameEnd.bEndMode = GER_DISMISS ;

			//历史积分
			for (WORD wID = 0; wID < GAME_PLAYER; wID++)
			{
				m_HistoryScore.OnEventUserScore(wID, 0);
			}

			//发送信息
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));

			//结束游戏
			//m_pITableFrame->ConcludeGame(GS_WK_FREE);

			return true;
		}
	case GER_NORMAL:			//常规结束
		{
			//构造数据
			CMD_S_GameEnd GameEnd;
			ZeroMemory(&GameEnd, sizeof(GameEnd));
			GameEnd.bEndMode = GER_NORMAL ;

			for(WORD i=0 ; i<GAME_PLAYER ; ++i)
			{
				IServerUserItem *pIServerUserItem = m_pITableFrame->GetTableUserItem(i);
				if (m_bGameStatus[i] == false) continue;
				
				if (pIServerUserItem)
				{
					if(i!=m_wBanker)
					{
						//至尊青龙
						if ((m_bSpecialType[i] == CT_EX_ZHIZUNQINGLONG || m_bSpecialType[m_wBanker] == CT_EX_ZHIZUNQINGLONG))
						{
							int nMulOnSD = 0;
							if (m_bSpecialType[i] == CT_EX_ZHIZUNQINGLONG && m_bSpecialType[m_wBanker] != CT_EX_ZHIZUNQINGLONG)
								nMulOnSD = 36;
							else if (m_bSpecialType[i] != CT_EX_ZHIZUNQINGLONG && m_bSpecialType[m_wBanker] == CT_EX_ZHIZUNQINGLONG)
								nMulOnSD = -36;
							else if (m_bSpecialType[i] == CT_EX_ZHIZUNQINGLONG && m_bSpecialType[m_wBanker] == CT_EX_ZHIZUNQINGLONG)
								nMulOnSD = -36;

							GameEnd.lGameScore[i]		   = nMulOnSD * m_lUserChip[i];
							GameEnd.lGameScore[m_wBanker] -= nMulOnSD * m_lUserChip[i];

							//分数倍数
							GameEnd.lScoreTimes[i] = nMulOnSD;
							GameEnd.lScoreTimes[m_wBanker] -= nMulOnSD;

							if (nMulOnSD == 0)			GameEnd.bWinInfo[i] = 1;
							else if (nMulOnSD > 0)		GameEnd.bWinInfo[i] = 2;
							else if (nMulOnSD < 0)		GameEnd.bWinInfo[i] = 0;
						}

						//一条龙
						else if ((m_bSpecialType[i] == CT_EX_YITIAOLONG || m_bSpecialType[m_wBanker] == CT_EX_YITIAOLONG))
						{
							int nMulOnSD = 0;
							if (m_bSpecialType[i] == CT_EX_YITIAOLONG && m_bSpecialType[m_wBanker] != CT_EX_YITIAOLONG)
								nMulOnSD = 13;
							else if (m_bSpecialType[i] != CT_EX_YITIAOLONG && m_bSpecialType[m_wBanker] == CT_EX_YITIAOLONG)
								nMulOnSD = -13;
							else if (m_bSpecialType[i] == CT_EX_YITIAOLONG && m_bSpecialType[m_wBanker] == CT_EX_YITIAOLONG)
								nMulOnSD = -13;

							GameEnd.lGameScore[i]		   = nMulOnSD * m_lUserChip[i];
							GameEnd.lGameScore[m_wBanker] -= nMulOnSD * m_lUserChip[i];

							//分数倍数
							GameEnd.lScoreTimes[i] = nMulOnSD;
							GameEnd.lScoreTimes[m_wBanker] -= nMulOnSD;

							if (nMulOnSD == 0)			GameEnd.bWinInfo[i] = 1;
							else if (nMulOnSD > 0)		GameEnd.bWinInfo[i] = 2;
							else if (nMulOnSD < 0)		GameEnd.bWinInfo[i] = 0;
						}

						//六对半
						else if ((m_bSpecialType[i] == CT_EX_LIUDUIBAN || m_bSpecialType[m_wBanker] == CT_EX_LIUDUIBAN))
						{
							int nMulOnSD = 0;
							if (m_bSpecialType[i] == CT_EX_LIUDUIBAN && m_bSpecialType[m_wBanker] != CT_EX_LIUDUIBAN)
								nMulOnSD = 3;
							else if (m_bSpecialType[i] != CT_EX_LIUDUIBAN && m_bSpecialType[m_wBanker] == CT_EX_LIUDUIBAN)
								nMulOnSD = -3;
							else if (m_bSpecialType[i] == CT_EX_LIUDUIBAN && m_bSpecialType[m_wBanker] == CT_EX_LIUDUIBAN)
								nMulOnSD = -3;

							GameEnd.lGameScore[i]		   = nMulOnSD * m_lUserChip[i];
							GameEnd.lGameScore[m_wBanker] -= nMulOnSD * m_lUserChip[i];

							//分数倍数
							GameEnd.lScoreTimes[i] = nMulOnSD;
							GameEnd.lScoreTimes[m_wBanker] -= nMulOnSD;

							if (nMulOnSD == 0)			GameEnd.bWinInfo[i] = 1;
							else if (nMulOnSD > 0)		GameEnd.bWinInfo[i] = 2;
							else if (nMulOnSD < 0)		GameEnd.bWinInfo[i] = 0;
						}

						//三顺子
						else if ((m_bSpecialType[i] == CT_EX_SANSHUNZI || m_bSpecialType[m_wBanker] == CT_EX_SANSHUNZI))
						{
							int nMulOnSD = 0;
							if (m_bSpecialType[i] == CT_EX_SANSHUNZI && m_bSpecialType[m_wBanker] != CT_EX_SANSHUNZI)
								nMulOnSD = 3;
							else if (m_bSpecialType[i] != CT_EX_SANSHUNZI && m_bSpecialType[m_wBanker] == CT_EX_SANSHUNZI)
								nMulOnSD = -3;
							else if (m_bSpecialType[i] == CT_EX_SANSHUNZI && m_bSpecialType[m_wBanker] == CT_EX_SANSHUNZI)
								nMulOnSD = -3;

							GameEnd.lGameScore[i]		   = nMulOnSD * m_lUserChip[i];
							GameEnd.lGameScore[m_wBanker] -= nMulOnSD * m_lUserChip[i];

							//分数倍数
							GameEnd.lScoreTimes[i] = nMulOnSD;
							GameEnd.lScoreTimes[m_wBanker] -= nMulOnSD;

							if (nMulOnSD == 0)			GameEnd.bWinInfo[i] = 1;
							else if (nMulOnSD > 0)		GameEnd.bWinInfo[i] = 2;
							else if (nMulOnSD < 0)		GameEnd.bWinInfo[i] = 0;
						}

						//三同花
						else if ((m_bSpecialType[i] == CT_EX_SANTONGHUA || m_bSpecialType[m_wBanker] == CT_EX_SANTONGHUA))
						{
							int nMulOnSD = 0;
							if (m_bSpecialType[i] == CT_EX_SANTONGHUA && m_bSpecialType[m_wBanker] != CT_EX_SANTONGHUA)
								nMulOnSD = 3;
							else if (m_bSpecialType[i] != CT_EX_SANTONGHUA && m_bSpecialType[m_wBanker] == CT_EX_SANTONGHUA)
								nMulOnSD = -3;
							else if (m_bSpecialType[i] == CT_EX_SANTONGHUA && m_bSpecialType[m_wBanker] == CT_EX_SANTONGHUA)
								nMulOnSD = -3;

							GameEnd.lGameScore[i]		   = nMulOnSD * m_lUserChip[i];
							GameEnd.lGameScore[m_wBanker] -= nMulOnSD * m_lUserChip[i];

							//分数倍数
							GameEnd.lScoreTimes[i] = nMulOnSD;
							GameEnd.lScoreTimes[m_wBanker] -= nMulOnSD;

							if (nMulOnSD == 0)			GameEnd.bWinInfo[i] = 1;
							else if (nMulOnSD > 0)		GameEnd.bWinInfo[i] = 2;
							else if (nMulOnSD < 0)		GameEnd.bWinInfo[i] = 0;
						}
						//普通牌型
						else
						{
							//最终结算
							INT nMultiple = (m_lScoreTimes[i][0] + m_lScoreTimes[i][1] + m_lScoreTimes[i][2]);

							GameEnd.lScoreTimes[i]		   = nMultiple;
							GameEnd.lScoreTimes[m_wBanker]-= nMultiple;

							if (nMultiple == 0)			GameEnd.bWinInfo[i] = 1;
							else if (nMultiple > 0)		GameEnd.bWinInfo[i] = 2;
							else if (nMultiple < 0)		GameEnd.bWinInfo[i] = 0;

							GameEnd.lGameScore[i]		   = nMultiple * m_lUserChip[i];
							GameEnd.lGameScore[m_wBanker] -= nMultiple * m_lUserChip[i];
						}
					}
				}
			}

			//负分处理
			if (m_pGameServiceOption->wServerType&GAME_GENRE_GOLD)
			{
				SCORE lScore[GAME_PLAYER];
				ZeroMemory(lScore,sizeof(lScore));

				//防止数据库负分
				for(WORD i=0;i<GAME_PLAYER;i++)
				{
					if (m_bGameStatus[i] == false) continue;
					if (GameEnd.lGameScore[i] < 0)
					{
						SCORE lUserScore =  m_pITableFrame->GetTableUserItem(i)->GetUserScore();
						if (lUserScore < -(GameEnd.lGameScore[i]))
							GameEnd.lGameScore[i] = -lUserScore;
					}
				}

				//庄家分数
				for(WORD i=0;i<GAME_PLAYER;i++)
				{
					if(m_bGameStatus[i] == false) continue;
					if ((GameEnd.lGameScore[i] < 0) && (i != m_wBanker))
					{
						lScore[m_wBanker] += -GameEnd.lGameScore[i];
						lScore[i] = GameEnd.lGameScore[i];
					}
				}

				//闲家分数
				for(WORD i=0;i<GAME_PLAYER;i++)
				{
					if(m_bGameStatus[i] == false) continue;
					if ((GameEnd.lGameScore[i] > 0) && (i != m_wBanker))
					{
						lScore[m_wBanker] -= GameEnd.lGameScore[i];
						lScore[i] = GameEnd.lGameScore[i];
					}
				}

				//庄家不足
				if(-lScore[m_wBanker] > m_pITableFrame->GetTableUserItem(m_wBanker)->GetUserScore())
				{
					SCORE lChipScore=0l;
					BYTE bPlayer=0;
					for(WORD i=0;i<GAME_PLAYER;i++)
					{
						if(m_bGameStatus[i] == false) continue;
						if(GameEnd.lGameScore[i] > 0 && i != m_wBanker)
						{
							lScore[i]=m_lUserChip[i];
							lChipScore+=m_lUserChip[i];
							bPlayer++;
						}
					}

					for(WORD i=0;i<GAME_PLAYER;i++)
					{
						if(m_bGameStatus[i] == false) continue;
						if(GameEnd.lGameScore[i] > 0 && i != m_wBanker)
						{
							lScore[i]+=(pIServerUserItem->GetUserScore()-lChipScore)/bPlayer;
						}
					}
				}

				//拷贝分数
				CopyMemory(GameEnd.lGameScore,lScore,sizeof(lScore));
			}

			//计算税收
			LONGLONG lRobotGameScore = 0;
			if (m_pGameServiceOption->wServerType&GAME_GENRE_GOLD)
			{
				ReadConfiguration();
				tagScoreInfo ScoreInfoArray[GAME_PLAYER];
				ZeroMemory(ScoreInfoArray,sizeof(ScoreInfoArray));
				
				for (WORD i=0;i<GAME_PLAYER;i++)
				{
					
					IServerUserItem *pIServerUserItem = m_pITableFrame->GetTableUserItem(i);
					if(m_bGameStatus[i] == false) continue;

					if (pIServerUserItem)
					{
						LONGLONG lTmpTax = 0;
						if (GameEnd.lGameScore[i] > 0)
							lTmpTax = m_pITableFrame->CalculateRevenue(i,GameEnd.lGameScore[i]);
						GameEnd.lGameTax	  += lTmpTax;
						GameEnd.lGameScore[i] -= lTmpTax;

						ScoreInfoArray[i].lScore   = GameEnd.lGameScore[i];
						ScoreInfoArray[i].cbType   = (GameEnd.lGameScore[i]>0L)?SCORE_TYPE_WIN:SCORE_TYPE_LOSE;
						ScoreInfoArray[i].lRevenue = lTmpTax; 

						//计算库存
						if(pIServerUserItem->IsAndroidUser())
						{
							//m_sRobotLnventory += GameEnd.lGameScore[i];
							lRobotGameScore += (GameEnd.lGameScore[i]+lTmpTax);
						}
					}
				}
				
				if(lRobotGameScore < 0 && m_sRobotLnventory < 0)
				{
					if((ROBOT_LNVENTORY_MIN + m_sRobotLnventory + lRobotGameScore) < 0)
					{
						m_sRobotLnventory = 0-ROBOT_LNVENTORY_MIN;
					}
					else
					{
						m_sRobotLnventory += lRobotGameScore;
					}
				}
				else if(lRobotGameScore > 0 && m_sRobotLnventory > 0)
				{
					if((ROBOT_LNVENTORY_MAX - m_sRobotLnventory - lRobotGameScore) < 0)
					{
						m_sRobotLnventory = ROBOT_LNVENTORY_MAX;
					}
					else
					{
						m_sRobotLnventory += lRobotGameScore;
					}
				}
				else
				{
					m_sRobotLnventory += lRobotGameScore;
				}

				ASSERT(m_sRobotLnventory >= 0);

				CString sPath,sKey,sMsg,sDate;
				TCHAR buffer[MAX_PATH];
				// 得到初始化文件的全路径
				GetModuleFileName(NULL, buffer, MAX_PATH);
				sPath = CString(buffer);
				sPath = sPath.Left(sPath.ReverseFind('\\'));
				sPath += "\\Thirteen.ini";
				//房间号
				sKey.Format(_T("%d"),m_pGameServiceOption->wServerID);
				sDate.Format(_T("%I64d"),m_sRobotLnventory);
				WritePrivateProfileString(sKey,_T("RobotLnventory"),sDate, sPath);

				m_pITableFrame->WriteTableScore(ScoreInfoArray,CountArray(ScoreInfoArray));

			}	

			//历史积分
			for (WORD wID=0; wID<GAME_PLAYER; wID++)
			{
				m_HistoryScore.OnEventUserScore(wID, GameEnd.lGameScore[wID]);
			}

			//发送信息
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));

			//结束游戏
			m_pITableFrame->ConcludeGame(GS_WK_FREE);		
			
			//库存消息
			for(BYTE i=0; i<GAME_PLAYER; i++)
			{
				IServerUserItem *pIServerUserItem = m_pITableFrame->GetTableUserItem(i);
				if(NULL == pIServerUserItem) continue;

				if(CUserRight::IsGameCheatUser(pIServerUserItem->GetUserRight()))
				{
					ReadConfiguration();
					SendControlMessage(pIServerUserItem);
				}
			}
			return true;
		}
	case GER_USER_LEAVE:		//用户强退
	case GER_NETWORK_ERROR:
		{
			//效验参数
			ASSERT(pIServerUserItem!=NULL);
			ASSERT(wChairID<GAME_PLAYER && m_bGameStatus[wChairID]==true);

			//定义变量
			CMD_S_PlayerExit PlayerExit;
			ZeroMemory(&PlayerExit, sizeof(PlayerExit));

			//设置状态
			m_bGameStatus[wChairID]=false;
			
			//统计人数
			DWORD dwUserCount=0;
			for(WORD i=0;i<GAME_PLAYER;i++)
			{
				if(m_bGameStatus[i] == true) dwUserCount++;
			}

			//叫庄状态
			if(m_pITableFrame->GetGameStatus() == GS_WK_CALL_BANKER)
			{
				//当前玩家为庄, 且逃跑
				if(m_wCurrentCaller == wChairID || wChairID == m_wBanker)
				{
					CMD_S_CallBanker CallBanker ;
					ZeroMemory( &CallBanker, sizeof( CallBanker ) ) ;
					CallBanker.bCallBanker = false ;
					CallBanker.wLastCaller = wChairID ;

					//轮换玩家
					while (true)
					{
						m_wCurrentCaller = (m_wCurrentCaller + 1) % GAME_PLAYER ;
						if (m_bGameStatus[m_wCurrentCaller] == false) continue;
						if (m_pITableFrame->GetTableUserItem(m_wCurrentCaller) != NULL || m_wFirstCallBankerUser == m_wCurrentCaller) break ;
					}

					//结束判断
					if (m_wCurrentCaller == m_wFirstCallBankerUser)
					{
						//都不做庄
						m_pITableFrame->ConcludeGame(GS_WK_FREE) ;

						//发送消息
						m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_END_ALL_NO_CALL ) ;
						m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_END_ALL_NO_CALL ) ;

						return true ;
					}

					CallBanker.wCurrentCaller = m_wCurrentCaller ;

					//发送消息
					m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_CALL_BANKER, &CallBanker, sizeof( CallBanker ) ) ;
					m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_CALL_BANKER, &CallBanker, sizeof( CallBanker ) ) ;

					//写入分数
					tagScoreInfo ScoreInfoArray[GAME_PLAYER];
					ZeroMemory(ScoreInfoArray,sizeof(ScoreInfoArray));

					ScoreInfoArray[wChairID].cbType=SCORE_TYPE_FLEE;
					m_pITableFrame->WriteTableScore(ScoreInfoArray,CountArray(ScoreInfoArray));
				}
			}

			//下注状态
			if(m_pITableFrame->GetGameStatus() == GS_WK_SETCHIP)
			{
				//用户没下注
				if(m_lUserChip[wChairID] == 0 && wChairID != m_wBanker)
				{
					if ((GetPlayerCount()-1) == m_wSetChipCount)
					{
						CMD_S_SendCard SendCard ;
						ZeroMemory(&SendCard, sizeof(SendCard));

						//特殊玩家
						//GameCheat(m_bNeedControl, m_bMysteryUser, m_bUserCardData[0], m_wBanker);

						//设置状态
						m_pITableFrame->SetGameStatus(GS_WK_PLAYING);

						//设置庄家
						SendCard.wBanker = m_wBanker;

						//保存扑克
						for(WORD i = 0; i < GAME_PLAYER; i++)
						{
							if (m_bGameStatus[i] != false)
							{
								m_GameLogic.SortCardList(m_bUserCardData[i], HAND_CARD_COUNT) ;
								CopyMemory(SendCard.bCardData, m_bUserCardData[i], sizeof(BYTE)*HAND_CARD_COUNT);
								SendCard.wCurrentUser = i ;
							}

							IServerUserItem *pIServerUserItem = m_pITableFrame->GetTableUserItem(i);
							if ((pIServerUserItem != NULL) && (m_bGameStatus[i] != false))
							{								
								m_pITableFrame->SendTableData(i, SUB_S_SEND_CARD_EX, &SendCard, sizeof(SendCard));
								m_pITableFrame->SendLookonData(i, SUB_S_SEND_CARD_EX, &SendCard, sizeof(SendCard));
							}
						}
					}

					//写入分数
					tagScoreInfo ScoreInfoArray[GAME_PLAYER];
					ZeroMemory(ScoreInfoArray,sizeof(ScoreInfoArray));

					ScoreInfoArray[wChairID].cbType=SCORE_TYPE_FLEE;
					m_pITableFrame->WriteTableScore(ScoreInfoArray,CountArray(ScoreInfoArray));
				}
				else if(m_lUserChip[wChairID] != 0 )
				{
					//定义变量
					CMD_S_GameEnd GameEnd;
					ZeroMemory(&GameEnd, sizeof(GameEnd));

					LONGLONG wUserToltalChip=(pIServerUserItem->GetUserScore());

					GameEnd.bEndMode = GER_USER_LEAVE ;



					//闲家逃跑
					if(wChairID!=m_wBanker)
					{
						//自动摆牌
						BYTE bPutCard[13] = {0};
						m_GameLogic.AutoPutCard(m_bUserCardData[m_wBanker],bPutCard,false,false);
						CopyMemory(m_bSegmentCard[m_wBanker][0],bPutCard,3);
						CopyMemory(m_bSegmentCard[m_wBanker][1],bPutCard+3,5);
						CopyMemory(m_bSegmentCard[m_wBanker][2],bPutCard+5,5);

						//或取倍数
						int nTimes = GetCardTimes(m_wBanker);

						GameEnd.lGameScore[wChairID] = -nTimes * m_lUserChip[wChairID];
						GameEnd.lGameScore[m_wBanker] = nTimes * m_lUserChip[wChairID];
					}
					//庄家逃跑
					else
					{
						for(WORD i=0 ; i<GAME_PLAYER ; ++i)
						{
							IServerUserItem *pIServerUserItem = m_pITableFrame->GetTableUserItem(i);
							if(pIServerUserItem && m_bGameStatus[i] && i!=m_wBanker && 0==m_lUserChip[i])
							{
								//自动摆牌
								BYTE bPutCard[13] = {0};
								m_GameLogic.AutoPutCard(m_bUserCardData[i],bPutCard,false,false);
								CopyMemory(m_bSegmentCard[i][0],bPutCard,3);
								CopyMemory(m_bSegmentCard[i][1],bPutCard+3,5);
								CopyMemory(m_bSegmentCard[i][2],bPutCard+5,5);

								//或取倍数
								int nTimes = GetCardTimes(i);

								GameEnd.lGameScore[m_wBanker] -= nTimes * m_lUserChip[i] ;
								GameEnd.lGameScore[i] = nTimes * m_lUserChip[i] ;
							}
						}

						////负分处理
						//if(GameEnd.lGameScore[m_wBanker] <= 0)
						//{
						//	GameEnd.lGameScore[m_wBanker] = -min(nTimes * m_pGameServiceOption->lCellScore,wUserToltalChip);
						//}
					}

					//计算税收
					if (m_pGameServiceOption->wServerType&GAME_GENRE_GOLD)
					{
						tagScoreInfo ScoreInfoArray[GAME_PLAYER];
						ZeroMemory(ScoreInfoArray,sizeof(ScoreInfoArray));

						for (WORD i=0;i<GAME_PLAYER;i++)
						{
							IServerUserItem *pIServerUserItem = m_pITableFrame->GetTableUserItem(i);
							if (pIServerUserItem==NULL) continue;

							if(i != wChairID)
							{
								LONGLONG lTmpTax=0;
								if (GameEnd.lGameScore[i] > 0)
								{
									lTmpTax = m_pITableFrame->CalculateRevenue(i,GameEnd.lGameScore[i]);
									GameEnd.lGameTax	  += lTmpTax;
									GameEnd.lGameScore[i] -= lTmpTax;
								}

								ScoreInfoArray[i].lScore   = GameEnd.lGameScore[i];
								ScoreInfoArray[i].cbType   = (GameEnd.lGameScore[i]>0L)?SCORE_TYPE_WIN:SCORE_TYPE_LOSE;
								ScoreInfoArray[i].lRevenue = lTmpTax;
							}
							else
							{
								ScoreInfoArray[wChairID].lScore = GameEnd.lGameScore[wChairID];
								ScoreInfoArray[wChairID].cbType = SCORE_TYPE_FLEE;
							}

							PlayerExit.lScore[i]=ScoreInfoArray[i].lScore;

						}
						m_pITableFrame->WriteTableScore(ScoreInfoArray,CountArray(ScoreInfoArray));

						m_lUserChip[wChairID]=0;
					}

					//下注人数
					m_wSetChipCount--;

					if ((GetPlayerCount()-1) == m_wSetChipCount)
					{
						CMD_S_SendCard SendCard ;
						ZeroMemory(&SendCard, sizeof(SendCard)) ;

						//特殊玩家
						//GameCheat(m_bNeedControl, m_bMysteryUser, m_bUserCardData[0], m_wBanker);

						//设置状态
						m_pITableFrame->SetGameStatus(GS_WK_PLAYING);

						//设置庄家
						SendCard.wBanker = m_wBanker;

						//保存扑克
						for(WORD i = 0; i < GAME_PLAYER; i++)
						{
							if (m_bGameStatus[wChairID] != false)
							{
								m_GameLogic.SortCardList(m_bUserCardData[i], HAND_CARD_COUNT) ;
								CopyMemory(SendCard.bCardData, m_bUserCardData[i], sizeof(BYTE)*HAND_CARD_COUNT);
								SendCard.wCurrentUser = i ;
							}

							IServerUserItem *pIServerUserItem = m_pITableFrame->GetTableUserItem(i);
							if ((pIServerUserItem != NULL) && m_bGameStatus[i] != false)
							{								
								m_pITableFrame->SendTableData(i, SUB_S_SEND_CARD_EX, &SendCard, sizeof(SendCard));
								m_pITableFrame->SendLookonData(i, SUB_S_SEND_CARD_EX, &SendCard, sizeof(SendCard));
							}
						}
					}
				}
			}

			//游戏状态
			if(m_pITableFrame->GetGameStatus() == GS_WK_PLAYING && wChairID != m_wBanker && dwUserCount >= 2)
			{
				tagScoreInfo ScoreInfoArray[GAME_PLAYER];
				ZeroMemory(ScoreInfoArray,sizeof(ScoreInfoArray));

				//自动摆牌
				BYTE bPutCard[13] = {0};
				m_GameLogic.AutoPutCard(m_bUserCardData[m_wBanker],bPutCard,false,false);
				CopyMemory(m_bSegmentCard[m_wBanker][0],bPutCard,3);
				CopyMemory(m_bSegmentCard[m_wBanker][1],bPutCard+3,5);
				CopyMemory(m_bSegmentCard[m_wBanker][2],bPutCard+5,5);

				int nTimes = GetCardTimes(m_wBanker);

				IServerUserItem *pIServerUserItem = m_pITableFrame->GetTableUserItem(wChairID);
				if(pIServerUserItem)	
				{
					//逃跑用户
					ScoreInfoArray[wChairID].lScore=-nTimes*m_lUserChip[wChairID];
					ScoreInfoArray[wChairID].cbType =SCORE_TYPE_FLEE;
					ScoreInfoArray[wChairID].lRevenue=0l;

					//庄家
					ScoreInfoArray[m_wBanker].lScore=nTimes*m_lUserChip[wChairID];
					ScoreInfoArray[m_wBanker].cbType =SCORE_TYPE_FLEE;
					ScoreInfoArray[m_wBanker].lRevenue=0l;  

					//
					PlayerExit.lScore[wChairID]=ScoreInfoArray[wChairID].lScore;
					PlayerExit.lScore[m_wBanker]=ScoreInfoArray[m_wBanker].lScore;

				}

				m_pITableFrame->WriteTableScore(ScoreInfoArray,CountArray(ScoreInfoArray));

				m_lUserChip[wChairID]=0;

				////判断人数
				//if(m_bSegmentCard[wChairID][0][0] != 0)
				//	m_wShowCardCount--;
			}

			//结束游戏
			if(wChairID == m_wBanker || dwUserCount < 2)
			{
				//定义变量
				CMD_S_GameEnd GameEnd;
				ZeroMemory(&GameEnd, sizeof(GameEnd));

				LONGLONG wUserToltalChip=(pIServerUserItem->GetUserScore());

				GameEnd.bEndMode = GER_USER_LEAVE ;


				//统计积分
				BYTE bPlayerCount=0 ; 
				if(wChairID!=m_wBanker)
				{
					if(0 != m_lUserChip[wChairID])
					{
						//自动摆牌
						BYTE bPutCard[13] = {0};
						m_GameLogic.AutoPutCard(m_bUserCardData[m_wBanker],bPutCard,false,false);
						CopyMemory(m_bSegmentCard[m_wBanker][0],bPutCard,3);
						CopyMemory(m_bSegmentCard[m_wBanker][1],bPutCard+3,5);
						CopyMemory(m_bSegmentCard[m_wBanker][2],bPutCard+5,5);

						//庄家倍率
						int nTimes = GetCardTimes(m_wBanker);

						GameEnd.lGameScore[wChairID] = -nTimes * m_lUserChip[wChairID] ;
						GameEnd.lGameScore[m_wBanker] = nTimes * m_lUserChip[wChairID] ;
					}
					//else
					//{
					//	GameEnd.lGameScore[wChairID] = -min(nTimes * m_pGameServiceOption->lCellScore, wUserToltalChip ) ;
					//}
				}
				else
				{
					for(WORD i=0 ; i<GAME_PLAYER ; ++i)
					{
						IServerUserItem *pIServerUserItem = m_pITableFrame->GetTableUserItem(i);
						if(pIServerUserItem ) ++bPlayerCount ;

						if(pIServerUserItem && i!=m_wBanker && 0!=m_lUserChip[i])
						{
							//自动摆牌
							BYTE bPutCard[13] = {0};
							m_GameLogic.AutoPutCard(m_bUserCardData[i],bPutCard,false,false);
							CopyMemory(m_bSegmentCard[i][0],bPutCard,3);
							CopyMemory(m_bSegmentCard[i][1],bPutCard+3,5);
							CopyMemory(m_bSegmentCard[i][2],bPutCard+5,5);

							//玩家倍率
							int nTimes = GetCardTimes(i);

							GameEnd.lGameScore[m_wBanker] -= nTimes * m_lUserChip[i];
							GameEnd.lGameScore[i] = nTimes * m_lUserChip[i];
						}
					}

	/*				if (0 == GameEnd.lGameScore[m_wBanker])
					{
						GameEnd.lGameScore[m_wBanker] = -min(nTimes*m_pGameServiceOption->lCellScore,wUserToltalChip);
					}*/
				}

				//负分处理
				if (m_pGameServiceOption->wServerType&GAME_GENRE_GOLD)
				{
					if(wChairID!=m_wBanker)
					{
						IServerUserItem *pIServerUserItem = m_pITableFrame->GetTableUserItem(wChairID);
						if(-(GameEnd.lGameScore[wChairID]) > pIServerUserItem->GetUserScore())
						{
							GameEnd.lGameScore[wChairID]=-pIServerUserItem->GetUserScore();
							GameEnd.lGameScore[m_wBanker]=pIServerUserItem->GetUserScore();
						}
					}
					else
					{
						IServerUserItem *pIServerBanker = m_pITableFrame->GetTableUserItem(m_wBanker);
						for(WORD i=0 ; i<GAME_PLAYER ; ++i)
						{
							IServerUserItem *pIServerUserItem = m_pITableFrame->GetTableUserItem(i);
							if (pIServerUserItem == NULL) continue;

							if(-(GameEnd.lGameScore[wChairID]) > pIServerBanker->GetUserScore())
							{
								GameEnd.lGameScore[i]=pIServerUserItem->GetUserScore()/(bPlayerCount-1 <= 0?1:bPlayerCount-1);
								GameEnd.lGameScore[m_wBanker]=-pIServerUserItem->GetUserScore();
							}
						}
					}
				}

				//计算税收
				if (m_pGameServiceOption->wServerType&GAME_GENRE_GOLD)
				{
					tagScoreInfo ScoreInfoArray[GAME_PLAYER];
					ZeroMemory(ScoreInfoArray,sizeof(ScoreInfoArray));

					for (WORD i=0;i<GAME_PLAYER;i++)
					{
						IServerUserItem *pIServerUserItem = m_pITableFrame->GetTableUserItem(i);
						if (pIServerUserItem == NULL) continue;

						if(i != wChairID)
						{
							LONGLONG lTmpTax=0;
							if (GameEnd.lGameScore[i] > 0)
							{
								lTmpTax = m_pITableFrame->CalculateRevenue(i,GameEnd.lGameScore[i]);
								GameEnd.lGameTax      += lTmpTax;
								GameEnd.lGameScore[i] -= lTmpTax;
							}

							ScoreInfoArray[i].lScore = GameEnd.lGameScore[i];
							ScoreInfoArray[i].cbType = (GameEnd.lGameScore[i]>0L)?SCORE_TYPE_WIN:SCORE_TYPE_LOSE;
							ScoreInfoArray[i].lRevenue = lTmpTax; 
						}
						else
						{
							ScoreInfoArray[i].lScore=GameEnd.lGameScore[i];
							ScoreInfoArray[i].cbType =SCORE_TYPE_FLEE;
						}

					}
					m_pITableFrame->WriteTableScore(ScoreInfoArray,CountArray(ScoreInfoArray));
				}


				//历史积分
				for (WORD wID = 0; wID < GAME_PLAYER; wID++)
				{
					m_HistoryScore.OnEventUserScore(wID, GameEnd.lGameScore[wID]);
				}

				//发送信息
				m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
				m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));

				//结束游戏
				m_pITableFrame->ConcludeGame(GS_WK_FREE);
			}
			else
			{
				m_lUserChip[wChairID] = 0;
				PlayerExit.wPlayerID=wChairID;

				//发送信息
				m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_PLAYER_EXIT,&PlayerExit,sizeof(PlayerExit));
				m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_PLAYER_EXIT,&PlayerExit,sizeof(PlayerExit));

				//开始比牌
				StartCompareCard();
			}
            return true;
		}
	}

	ASSERT(FALSE);

	return false;
}

//发送场景
bool CTableFrameSink::OnEventSendGameScene(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbGameStatus, bool bSendSecret)
{
	if (pIServerUserItem != NULL)
	{
		_DebugOutPut(TEXT("OnEventSendGameScene:(Table:%d wChairID:%d, Name:%s UserState:%d cbGameStatus:%d) \r\n ")
			,m_pITableFrame->GetTableID(), wChairID, pIServerUserItem->GetNickName(), pIServerUserItem->GetUserStatus(), cbGameStatus);
	}

	switch (cbGameStatus)
	{
	case GS_WK_FREE:		//空闲状态
		{
			//构造数据
			CMD_S_StatusFree StatusFree;
			ZeroMemory(&StatusFree, sizeof(StatusFree));

			//设置变量
			StatusFree.lBaseScore = m_pITableFrame->GetCellScore();

			//自定义规则
			StatusFree.cbTimeStartGame = m_pGameCustomRule->cbTimeStartGame;
			StatusFree.cbTimeCallBanker = m_pGameCustomRule->cbTimeCallBanker;
			StatusFree.cbTimeSetChip = m_pGameCustomRule->cbTimeSetChip;
			StatusFree.cbTimeRangeCard = m_pGameCustomRule->cbTimeRangeCard;
			StatusFree.cbTimeShowCard = m_pGameCustomRule->cbTimeShowCard;
			
			//房间标识
			StatusFree.wServerID = m_pGameServiceOption->wServerID;

			//设置状态
			for(WORD i=0;i<GAME_PLAYER;i++)
			{
				IServerUserItem *pServerUserItem = m_pITableFrame->GetTableUserItem(i) ;
				if (pServerUserItem != NULL) m_bGameStatus[i]=true;
			}

			CopyMemory(StatusFree.bGameStatus, m_bGameStatus, sizeof(m_bGameStatus));

			//历史成绩
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				//变量定义
				tagHistoryScore * pHistoryScore=m_HistoryScore.GetHistoryScore(i);

				//设置变量
				StatusFree.lTurnScore[i] = pHistoryScore->lTurnScore;
				StatusFree.lCollectScore[i] = pHistoryScore->lCollectScore;
			}

			//发送场景
			bool bSuccess =  m_pITableFrame->SendGameScene( pIServerUserItem, &StatusFree, sizeof(StatusFree));
			
			//库存消息
			if(CUserRight::IsGameCheatUser(pIServerUserItem->GetUserRight()))
			{
				ReadConfiguration();
				SendControlMessage(pIServerUserItem);
			}

			return bSuccess;
		}
	case GS_WK_CALL_BANKER :	//叫庄状态
		{
			CMD_S_StatusCallBanker StatusCallBanker ;
			ZeroMemory(&StatusCallBanker, sizeof( StatusCallBanker)) ;

			StatusCallBanker.wCurrentCaller = m_wCurrentCaller ;

			//自定义规则
			StatusCallBanker.cbTimeStartGame = m_pGameCustomRule->cbTimeStartGame;
			StatusCallBanker.cbTimeCallBanker = m_pGameCustomRule->cbTimeCallBanker;
			StatusCallBanker.cbTimeSetChip = m_pGameCustomRule->cbTimeSetChip;
			StatusCallBanker.cbTimeRangeCard = m_pGameCustomRule->cbTimeRangeCard;
			StatusCallBanker.cbTimeShowCard = m_pGameCustomRule->cbTimeShowCard;
			
			//房间标识
			StatusCallBanker.wServerID = m_pGameServiceOption->wServerID;

			//设置状态
			//for(WORD i=0;i<GAME_PLAYER;i++)
			//{
			//	IServerUserItem *pServerUserItem = m_pITableFrame->GetTableUserItem(i) ;
			//	if (pServerUserItem != NULL) m_bGameStatus[i]=true;
			//}
			CopyMemory(StatusCallBanker.bGameStatus,m_bGameStatus,sizeof(m_bGameStatus));

			//历史成绩
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				//变量定义
				tagHistoryScore * pHistoryScore=m_HistoryScore.GetHistoryScore(i);

				//设置变量
				StatusCallBanker.lTurnScore[i] = pHistoryScore->lTurnScore;
				StatusCallBanker.lCollectScore[i] = pHistoryScore->lCollectScore;
			}

			if(pIServerUserItem)
			{
				//作弊用户
				if(CUserRight::IsGameCheatUser(pIServerUserItem->GetUserRight()))
				{
					OnCheatCard(pIServerUserItem);
				}
			}

			//发送场景
			bool bSuccess =  m_pITableFrame->SendGameScene(pIServerUserItem,&StatusCallBanker,sizeof(StatusCallBanker));
			
			//库存消息
			if(CUserRight::IsGameCheatUser(pIServerUserItem->GetUserRight()))
			{
				ReadConfiguration();
				SendControlMessage(pIServerUserItem);
			}

			return bSuccess;
		}
	case GS_WK_SETCHIP:		//下注状态 
		{
			CMD_S_StatusSetChip StatusSetChip ;
			ZeroMemory(&StatusSetChip, sizeof(StatusSetChip)) ;

			//自定义规则
			StatusSetChip.cbTimeStartGame = m_pGameCustomRule->cbTimeStartGame;
			StatusSetChip.cbTimeCallBanker = m_pGameCustomRule->cbTimeCallBanker;
			StatusSetChip.cbTimeSetChip = m_pGameCustomRule->cbTimeSetChip;
			StatusSetChip.cbTimeRangeCard = m_pGameCustomRule->cbTimeRangeCard;
			StatusSetChip.cbTimeShowCard = m_pGameCustomRule->cbTimeShowCard;
			
			//房间标识
			StatusSetChip.wServerID = m_pGameServiceOption->wServerID;

			LONGLONG lBankerScore = m_pITableFrame->GetTableUserItem(m_wBanker)->GetUserScore();
			LONGLONG lMeScore = m_pITableFrame->GetTableUserItem(wChairID)->GetUserScore();

			StatusSetChip.wBanker	= m_wBanker ;
			
			//下注大小
			SCORE lChipsArray[GAME_PLAYER][SET_CHIP_COUNT] = {0};
			GetChips(lChipsArray) ;
			CopyMemory(StatusSetChip.lChipArray , lChipsArray, sizeof(lChipsArray)) ;

			CopyMemory(StatusSetChip.bHandCardData , m_bUserCardData[wChairID] , HAND_CARD_COUNT) ;
			CopyMemory(StatusSetChip.lChip , m_lUserChip , sizeof(m_lUserChip)) ;

			//设置状态
			for (int i = 0; i < GAME_PLAYER; ++i)
			{
				IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(i);
				if (pIServerUserItem == NULL) continue;
				tagUserInfo * pUserInfo=pIServerUserItem->GetUserInfo();
				if (pUserInfo == NULL) continue;
				if (pUserInfo->cbUserStatus == US_PLAYING)
				{
					m_bGameStatus[i] = true;
				}
			}
			CopyMemory(StatusSetChip.bGameStatus,m_bGameStatus,sizeof(m_bGameStatus));

			//历史成绩
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				//变量定义
				tagHistoryScore * pHistoryScore=m_HistoryScore.GetHistoryScore(i);

				//设置变量
				StatusSetChip.lTurnScore[i] = pHistoryScore->lTurnScore;
				StatusSetChip.lCollectScore[i] = pHistoryScore->lCollectScore;
			}

			if(pIServerUserItem)
			{
				//作弊用户
				if(CUserRight::IsGameCheatUser(pIServerUserItem->GetUserRight()))
				{
					OnCheatCard(pIServerUserItem);
				}
			}

			int iPlayerCount = 0;
			for (int i = 0; i < GAME_PLAYER; ++i)
			{
				IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(i);
				if (pIServerUserItem == NULL) continue;
				tagUserInfo * pUserInfo=pIServerUserItem->GetUserInfo();
				if (pUserInfo == NULL) continue;
				if (pUserInfo->cbUserStatus == US_PLAYING)
				{
					++iPlayerCount;
				}
			}

			//下注完成
			if (iPlayerCount - 1 == m_wSetChipCount)
			{
				StatusSetChip.bSetChipFinally = true;
			}

			m_pITableFrame->SendGameScene(pIServerUserItem,&StatusSetChip,sizeof(StatusSetChip));

			if (iPlayerCount-1 == m_wSetChipCount)//((GetPlayerCount()-1) == m_wSetChipCount)
			{
				////TEST+
				//BYTE cbCardData00[GAME_PLAYER][13] = 
				//			{
				//				{0x03, 0x04, 0x05, 0x07, 0x0d, 0x22, 0x27, 0x2a, 0x2c, 0x2d, 0x18, 0x38, 0x3c},	//三同花
				//				{0x17, 0x27, 0x37, 0x0d, 0x1d, 0x25, 0x06, 0x18, 0x38, 0x2a, 0x03, 0x24, 0x34},	//六对半//AA KK QQ JJ 99 77 2
				//				{0x32, 0x13, 0x24, 0x06, 0x17, 0x28, 0x39, 0x0A, 0x07, 0x08, 0x19, 0x2A, 0x3B},	//三顺子
				//				{0x33, 0x23, 0x13, 0x28, 0x38, 0x18, 0x2C, 0x2D, 0x02, 0x04, 0x06, 0x08, 0x09},	//前墩三条
				//			};

				//CopyMemory(m_bUserCardData, cbCardData00, sizeof(cbCardData00));
				//延时3S执行发牌
				//m_pITableFrame->SetGameTimer(IDI_CHECK_TABLE,3000,-1,NULL);

				//机器人的输赢判断
				JudgeResults();

				//控制发牌
				ResultsControl(0);
				////TEST+

				CMD_S_SendCard SendCard ;
				ZeroMemory(&SendCard, sizeof(SendCard)) ;

				//设置庄家
				SendCard.wBanker = m_wBanker;
				SendCard.cbPlayCount = GetPlayerCount();

				//发送扑克
				for(WORD i = 0; i < GAME_PLAYER; i++)
				{			
					CopyMemory(SendCard.bCardData, m_bUserCardData[i], sizeof(BYTE)*HAND_CARD_COUNT);
					SendCard.wCurrentUser = i ;

					IServerUserItem *pIServerUserItem = m_pITableFrame->GetTableUserItem(i);
					if ((pIServerUserItem != NULL))
					{
						//发送消息
						m_pITableFrame->SendTableData(i, SUB_S_SEND_CARD_EX, &SendCard, sizeof(SendCard));
						m_pITableFrame->SendLookonData(i, SUB_S_SEND_CARD_EX, &SendCard, sizeof(SendCard));
					}
				}

				for(WORD i = 0; i < GAME_PLAYER; i++)
				{			
					m_GameLogic.SortCardList(m_bUserCardData[i], HAND_CARD_COUNT) ;
				}
			}
			//发送场景

			//库存消息
			if(CUserRight::IsGameCheatUser(pIServerUserItem->GetUserRight()))
			{
				ReadConfiguration();
				SendControlMessage(pIServerUserItem);
			}

			return true;//m_pITableFrame->SendGameScene(pIServerUserItem,&StatusSetChip,sizeof(StatusSetChip));
		}
	case GS_WK_PLAYING:		//游戏状态
		{
			//构造数据
			CMD_S_StatusPlay StatusPlay;
			ZeroMemory(&StatusPlay, sizeof(StatusPlay));

			//自定义规则
			StatusPlay.cbTimeStartGame = m_pGameCustomRule->cbTimeStartGame;
			StatusPlay.cbTimeCallBanker = m_pGameCustomRule->cbTimeCallBanker;
			StatusPlay.cbTimeSetChip = m_pGameCustomRule->cbTimeSetChip;
			StatusPlay.cbTimeRangeCard = m_pGameCustomRule->cbTimeRangeCard;
			StatusPlay.cbTimeShowCard = m_pGameCustomRule->cbTimeShowCard;
			
			//房间标识
			StatusPlay.wServerID = m_pGameServiceOption->wServerID;

			LONGLONG lBankerScore = m_pITableFrame->GetTableUserItem(m_wBanker)->GetUserScore();

			StatusPlay.cbPlayCount = GetPlayerCount();
			StatusPlay.wBanker	= m_wBanker ;
			
			//下注大小
			SCORE lChipsArray[GAME_PLAYER][SET_CHIP_COUNT] = {0};
			GetChips(lChipsArray) ;
			CopyMemory(StatusPlay.lChipArray , lChipsArray, sizeof(lChipsArray)) ;

			CopyMemory(StatusPlay.bHandCardData , m_bUserCardData[wChairID] , HAND_CARD_COUNT) ;
			CopyMemory(StatusPlay.bSegmentCard , m_bSegmentCard , sizeof(m_bSegmentCard)) ;
			CopyMemory(StatusPlay.bFinishSegment , m_bFinishSegment , sizeof(m_bFinishSegment)) ;
			CopyMemory(StatusPlay.lChip , m_lUserChip , sizeof(m_lUserChip)) ;
			CopyMemory(StatusPlay.lScoreTimes, m_lScoreTimes, sizeof(m_lScoreTimes));
			CopyMemory(StatusPlay.bSpecialType, m_bSpecialType, sizeof(m_bSpecialType));
			StatusPlay.bCompared = m_bCompared;
			//设置状态
			//for(WORD i=0;i<GAME_PLAYER;i++)
			//{
			//	IServerUserItem *pServerUserItem = m_pITableFrame->GetTableUserItem(i) ;
			//	if (pServerUserItem != NULL) m_bGameStatus[i]=true;
			//}
			CopyMemory(StatusPlay.bGameStatus,m_bGameStatus,sizeof(m_bGameStatus));

			//历史成绩
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				//变量定义
				tagHistoryScore * pHistoryScore=m_HistoryScore.GetHistoryScore(i);

				//设置变量
				StatusPlay.lTurnScore[i] = pHistoryScore->lTurnScore;
				StatusPlay.lCollectScore[i] = pHistoryScore->lCollectScore;
			}

			if(pIServerUserItem)
			{
				//作弊用户
				if(CUserRight::IsGameCheatUser(pIServerUserItem->GetUserRight()))
				{
					OnCheatCard(pIServerUserItem);
				}
			}

			//发送场景
			bool bSuccess =  m_pITableFrame->SendGameScene(pIServerUserItem,&StatusPlay,sizeof(StatusPlay));
			
			//库存消息
			if(CUserRight::IsGameCheatUser(pIServerUserItem->GetUserRight()))
			{
				ReadConfiguration();
				SendControlMessage(pIServerUserItem);
			}

			return bSuccess;
		}
	}

	return false;
}

//时间事件
bool CTableFrameSink::OnTimerMessage(DWORD wTimerID, WPARAM wBindParam)
{

	if(wTimerID==IDI_CHECK_TABLE)
	{
		m_cbWaitTime++;
		if(m_cbWaitTime>=6)
		{
			OnEventGameConclude(INVALID_CHAIR,NULL,GER_DISMISS);
		}
		if(m_cbWaitTime>=4)
		{
			for(int i=0;i<GAME_PLAYER;i++)
			{
				IServerUserItem *pServerUserItem=m_pITableFrame->GetTableUserItem(i);
				if(pServerUserItem ==NULL ) continue;
				if (pServerUserItem->GetUserStatus()==US_OFFLINE)
				{
					OnEventGameConclude(pServerUserItem->GetChairID(), NULL, GER_USER_LEAVE);
				}
			}
			//OnEventGameConclude(INVALID_CHAIR,NULL,GER_DISMISS);
		}
		return true;
	}
	//else if (IDI_DELAY == wTimerID)
	//{
	//	m_pITableFrame->KillGameTimer(IDI_DELAY);
	//	ResultsControl(0);
	//	////TEST+
	//	CMD_S_SendCard SendCard ;
	//	ZeroMemory(&SendCard, sizeof(SendCard)) ;
	//	//设置庄家
	//	SendCard.wBanker = m_wBanker;
	//	SendCard.cbPlayCount = GetPlayerCount();

	//	//发送扑克
	//	for(WORD i = 0; i < GAME_PLAYER; i++)
	//	{			
	//		CopyMemory(SendCard.bCardData, m_bUserCardData[i], sizeof(BYTE)*HAND_CARD_COUNT);
	//		SendCard.wCurrentUser = i ;

	//		IServerUserItem *pIServerUserItem = m_pITableFrame->GetTableUserItem(i);
	//		if ((pIServerUserItem != NULL))
	//		{
	//			//发送消息
	//			m_pITableFrame->SendTableData(i, SUB_S_SEND_CARD_EX, &SendCard, sizeof(SendCard));
	//			m_pITableFrame->SendLookonData(i, SUB_S_SEND_CARD_EX, &SendCard, sizeof(SendCard));
	//		}
	//	}

	//	for(WORD i = 0; i < GAME_PLAYER; i++)
	//	{			
	//		m_GameLogic.SortCardList(m_bUserCardData[i], HAND_CARD_COUNT) ;
	//	}
	//}

	return false;
}

//数据事件
bool CTableFrameSink::OnDataBaseMessage(WORD wRequestID, VOID * pData, WORD wDataSize)
{
	return false;
}
//积分事件
bool CTableFrameSink::OnUserScroeNotify(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)
{
	return true;
}

//游戏消息处理
bool CTableFrameSink::OnGameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem)
{

	if (pIServerUserItem != NULL)
	{
		_DebugOutPut(TEXT("OnGameMessage:(Table:%d wChairID:%d, Name:%s UserState:%d wSubCmdID:%d) \r\n")
			,m_pITableFrame->GetTableID(), pIServerUserItem->GetChairID(), pIServerUserItem->GetNickName(), pIServerUserItem->GetUserStatus(), wSubCmdID);
	}

	switch (wSubCmdID)
	{
	case SUB_C_SETCHIP:				//设置下注
		{
			//状态效验
			ASSERT(m_pITableFrame->GetGameStatus()==GS_WK_SETCHIP);
			if (m_pITableFrame->GetGameStatus()!=GS_WK_SETCHIP) return true;

			tagUserInfo * pUserData=pIServerUserItem->GetUserInfo();
			return OnUserSetChip(pUserData->wChairID , pData, wDataSize) ;
		}
	case SUB_C_SEGCARD:				//设置分段
		{
			tagUserInfo * pUserData=pIServerUserItem->GetUserInfo();
			return OnUserSetSegment(pUserData->wChairID , pData, wDataSize) ;
		}
	case SUB_C_SHOWARD:				//玩家摊牌
		{
			tagUserInfo * pUserData=pIServerUserItem->GetUserInfo();
			return OnUserShowCard(pUserData->wChairID , pData, wDataSize) ;
		}
	case SUB_C_COMPLETE_COMPARE:	//完成比较
		{
			m_wCompleteCompareCount++;

			if(GetPlayerCount() == m_wCompleteCompareCount)
			{
				tagUserInfo * pUserData=pIServerUserItem->GetUserInfo();
				return OnEventGameConclude(pUserData->wChairID,NULL,GER_NORMAL);
			}
			return true ;
		}
	case SUB_C_CALL_BANKER :		//玩家叫庄
		{
			//状态效验
			ASSERT(m_pITableFrame->GetGameStatus()==GS_WK_CALL_BANKER);
			if (m_pITableFrame->GetGameStatus()!=GS_WK_CALL_BANKER) return true;

			tagUserInfo * pUserData = pIServerUserItem->GetUserInfo();
			return OnUserCallBanker(pUserData->wChairID, pData, wDataSize);
		}
	case SUB_C_NO_CALL_BANKER :		//玩家不叫
		{
			//状态效验
			ASSERT(m_pITableFrame->GetGameStatus()==GS_WK_CALL_BANKER);
			if (m_pITableFrame->GetGameStatus()!=GS_WK_CALL_BANKER) return true;

			tagUserInfo * pUserData=pIServerUserItem->GetUserInfo();
			return OnUserNoCallBanker(pUserData->wChairID, pData, wDataSize);
		}
	case SUB_C_TRUSTEE:				//玩家托管
		{
			//长度校验
			CMD_C_Trustee *pTrustee =(CMD_C_Trustee *)pData;
			if(wDataSize != sizeof(CMD_C_Trustee)) return false;

			//用户校验
			tagUserInfo * pUserData=pIServerUserItem->GetUserInfo();

			//构造数据
			CMD_S_Trustee Trustee;
			ZeroMemory(&Trustee, sizeof(Trustee));
			Trustee.wChairID = pUserData->wChairID;
			Trustee.bTrustee = pTrustee->bTrustee;

			m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_TRUSTEE, &Trustee, sizeof(Trustee));
			m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_TRUSTEE, &Trustee, sizeof(Trustee));

			return true;
		}
	case SUB_C_VOICE_PHRASE:
		{
			//效验消息
			ASSERT(wDataSize==sizeof(CMD_C_ChoseYuYin));
			if (wDataSize!=sizeof(CMD_C_ChoseYuYin)) return false;


			CMD_C_ChoseYuYin *pChoseYuYin = (CMD_C_ChoseYuYin *)pData;
		
			CMD_S_ChoseYuYin ChoseYuYin;
			ZeroMemory(&ChoseYuYin, sizeof(ChoseYuYin));
			ChoseYuYin.bChoseYuYin=pChoseYuYin->bYuYinIndex;
			ChoseYuYin.wChairID=pChoseYuYin->wChairID;
			ChoseYuYin.bType=pChoseYuYin->bType;

			//发送消息
			m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_VOICE_PHRASE, &ChoseYuYin, sizeof(CMD_S_ChoseYuYin));
			m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_VOICE_PHRASE, &ChoseYuYin, sizeof(CMD_S_ChoseYuYin));

			return true;
		}
	case SUB_C_SHUFFLE_FINISH:		//洗牌完毕
		{
			tagUserInfo * pUserData=pIServerUserItem->GetUserInfo();
			if (pUserData->wChairID != INVALID_CHAIR) m_wShuffleFinishCount++;

			//全部洗牌完毕
			if ((GetPlayerCount() == m_wShuffleFinishCount))
			{
				//设置状态
				m_pITableFrame->SetGameStatus(GS_WK_PLAYING);
		
				//开始发牌
				m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_DISPATCH_START);
				m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_DISPATCH_START);
			}

			return true;
		}
	case SUB_C_SPLIT_CARD:			//开始切牌
		{
			tagUserInfo * pUserData = pIServerUserItem->GetUserInfo();
	
			//设置结构
			CMD_S_SplitCard SplitCard;
			ZeroMemory(&SplitCard, sizeof(SplitCard));
			SplitCard.wLastUser = pUserData->wChairID;

			//发送消息
			m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_SPLIT_CARD, &SplitCard, sizeof(SplitCard));
			m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_SPLIT_CARD, &SplitCard, sizeof(SplitCard));

			return true;
		}
	case SUB_C_SPLIT_FINISH:		//切牌完毕
		{
			tagUserInfo * pUserData=pIServerUserItem->GetUserInfo();
			if (pUserData->wChairID != INVALID_CHAIR) 	m_wSpliFinishCount++;
			
			//全部切牌完毕
			if (GetPlayerCount() == m_wSpliFinishCount)
			{
				//发送消息
				m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_DISPATCH_START);
				m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_DISPATCH_START);
			}

			return true;
		}
	}
	
	return false;
}

//框架消息
bool CTableFrameSink::OnFrameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	return false;
}

//玩家叫庄
bool CTableFrameSink::OnUserCallBanker(const WORD wChairID , const void * pDataBuffer, WORD wDataSize)
{
	ASSERT((wChairID >= 0) && (wChairID < GAME_PLAYER));
	if ((wChairID < 0) || (wChairID >= GAME_PLAYER)) return false;

	//设置变量
	m_wFirstCallBankerUser = wChairID;
	m_wBanker = wChairID;
	LONGLONG lBankerScore = m_pITableFrame->GetTableUserItem(m_wBanker)->GetUserScore();

	CMD_S_CallBanker CallBanker ;
	ZeroMemory(&CallBanker, sizeof(CallBanker)) ;
	CallBanker.bCallBanker = true ;
	CallBanker.wCurrentCaller = INVALID_CHAIR;
	CallBanker.wLastCaller = wChairID ;
	CallBanker.lBankerScore = lBankerScore;

	//下注大小
	SCORE lChipsArray[GAME_PLAYER][SET_CHIP_COUNT] = {0};
	GetChips(lChipsArray) ;
	CopyMemory(CallBanker.lChipArray , lChipsArray, sizeof(lChipsArray)) ;

	//发送消息
	for (int i = 0; i < GAME_PLAYER; i++)
	{
		IServerUserItem *pIServerUserItem = m_pITableFrame->GetTableUserItem(i);
		if ((pIServerUserItem == NULL)) continue;
		if(m_pGameServiceOption->lRestrictScore != 0) CallBanker.lMaxChip=m_pGameServiceOption->lRestrictScore;

		m_pITableFrame->SendTableData(i, SUB_S_CALL_BANKER, &CallBanker, sizeof(CallBanker)) ;
		m_pITableFrame->SendLookonData(i, SUB_S_CALL_BANKER, &CallBanker, sizeof(CallBanker)) ;
	}

	//设置状态
	m_pITableFrame->SetGameStatus( GS_WK_SETCHIP) ;

	return true ;
}

//玩家不叫
bool CTableFrameSink::OnUserNoCallBanker(const WORD wChairID , const void * pDataBuffer, WORD wDataSize)
{
	ASSERT((wChairID >= 0) && (wChairID < GAME_PLAYER));
	if ((wChairID < 0) || (wChairID >= GAME_PLAYER)) return false;

	CMD_S_CallBanker CallBanker ;
	ZeroMemory(&CallBanker, sizeof(CallBanker)) ;
	CallBanker.bCallBanker = false ;
	CallBanker.wLastCaller = wChairID;

	//设置变量
	while (true)
	{
		//轮换玩家
		m_wCurrentCaller = (m_wCurrentCaller + 1) % GAME_PLAYER ;
		if(m_bGameStatus[m_wCurrentCaller] == false) continue;

		if ( (m_pITableFrame->GetTableUserItem(m_wCurrentCaller) != NULL || m_wFirstCallBankerUser == m_wCurrentCaller) 
		 && m_pITableFrame->GetTableUserItem(m_wCurrentCaller)->GetUserStatus() == US_PLAYING)
		{
			break;
		}
	}

	//结束判断
	if (m_wCurrentCaller == m_wFirstCallBankerUser)
	{		
		//都不做庄
		m_pITableFrame->ConcludeGame(GS_WK_FREE) ;
		
		//发送消息
		m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_END_ALL_NO_CALL);
		m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_END_ALL_NO_CALL);

		return true;
	}

	CallBanker.wCurrentCaller = m_wCurrentCaller;

	//发送消息
	m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_CALL_BANKER, &CallBanker, sizeof(CallBanker));
	m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_CALL_BANKER, &CallBanker, sizeof(CallBanker));

	return true ;
}

//设置下注
bool CTableFrameSink::OnUserSetChip(const WORD wChairID , const void * pDataBuffer, WORD wDataSize)
{
	//数据验证 
	ASSERT((wChairID >= 0) && (wChairID < GAME_PLAYER));
	if ((wChairID < 0) || (wChairID >= GAME_PLAYER)) return false;

	ASSERT(sizeof(CMD_C_SetChip) == wDataSize) ;
	if(sizeof(CMD_C_SetChip) != wDataSize) return false;

	CMD_C_SetChip *pSetChip = (CMD_C_SetChip*)pDataBuffer ;
	
	//筹码验证
	IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(wChairID);
	ASSERT(pSetChip->nChip <= pIServerUserItem->GetUserScore() && pSetChip->nChip >= 0);
	if (pSetChip->nChip > pIServerUserItem->GetUserScore() ||  pSetChip->nChip < 0) return false;
	
	//下注验证
	if(0 != m_lUserChip[wChairID]) return false;
	SCORE lChipsArray[SET_CHIP_COUNT] = {0};
	GetChips(wChairID, lChipsArray);
	for(BYTE i=0; i<SET_CHIP_COUNT; i++)
	{
		if(pSetChip->nChip == lChipsArray[i])
			break;
	}
	if(SET_CHIP_COUNT == i) return false;

	//设置变量
	m_lUserChip[wChairID] = pSetChip->nChip ;
	m_wSetChipCount++;

	//发送数据
	CMD_S_SetChip SetChip ;
	ZeroMemory(&SetChip, sizeof(SetChip));
	SetChip.wChipUser = wChairID;
	SetChip.lChip = pSetChip->nChip; 
	
	//发送消息
	m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_SETCHIP, &SetChip, sizeof(SetChip));
	m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_SETCHIP, &SetChip, sizeof(SetChip)); 

	//全部下注结束
	int iPlayerCount = 0;
	for (int i = 0; i < GAME_PLAYER; ++i)
	{
		IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(i);
		if (pIServerUserItem == NULL) continue;
		tagUserInfo * pUserInfo=pIServerUserItem->GetUserInfo();
		if (pUserInfo == NULL) continue;
		if (pUserInfo->cbUserStatus == US_PLAYING)
		{
			++iPlayerCount;
		}
	}

	if (iPlayerCount-1 == m_wSetChipCount)//((GetPlayerCount()-1) == m_wSetChipCount)
	{
		////TEST+
		//BYTE cbCardData00[GAME_PLAYER][13] = 
		//			{
		//				{0x33, 0x35, 0x37, 0x28, 0x29, 0x2B, 0x2C, 0x2D, 0x02, 0x04, 0x06, 0x08, 0x09},	//三同花
		//				{0x01, 0x01, 0x3D, 0x2D, 0x0C, 0x1C, 0x0B, 0x2B, 0x29, 0x19, 0x17, 0x37, 0x02},	//六对半//AA KK QQ JJ 99 77 2
		//				{0x32, 0x33, 0x34, 0x04, 0x05, 0x15, 0x06, 0x36, 0x27, 0x08, 0x18, 0x09, 0x0A},	//三顺子
		//				{0x33, 0x23, 0x13, 0x28, 0x38, 0x18, 0x2C, 0x2D, 0x02, 0x04, 0x06, 0x08, 0x09},	//前墩三条
		//			};

		//CopyMemory(m_bUserCardData, cbCardData00, sizeof(cbCardData00));
		////TEST+

		if(IsNeedControl())
		{
			//机器人的输赢判断
			JudgeResults();

			//控制发牌
			ResultsControl(0);
			
			//发送作弊
			SendCheatCard();
		}
		

		CMD_S_SendCard SendCard ;
		ZeroMemory(&SendCard, sizeof(SendCard)) ;

		//设置庄家
		SendCard.wBanker = m_wBanker;
		SendCard.cbPlayCount = GetPlayerCount();

		//发送扑克
		for(WORD i = 0; i < GAME_PLAYER; i++)
		{			
			CopyMemory(SendCard.bCardData, m_bUserCardData[i], sizeof(BYTE)*HAND_CARD_COUNT);
			SendCard.wCurrentUser = i ;

			IServerUserItem *pIServerUserItem = m_pITableFrame->GetTableUserItem(i);
			if ((pIServerUserItem != NULL))
			{
				//发送消息
				m_pITableFrame->SendTableData(i, SUB_S_SEND_CARD_EX, &SendCard, sizeof(SendCard));
				m_pITableFrame->SendLookonData(i, SUB_S_SEND_CARD_EX, &SendCard, sizeof(SendCard));
			}
		}

		for(WORD i = 0; i < GAME_PLAYER; i++)
		{			
			m_GameLogic.SortCardList(m_bUserCardData[i], HAND_CARD_COUNT) ;
		}
	}

	return true ;
}

//设置分段
bool CTableFrameSink::OnUserSetSegment(const WORD wChairID , const void * pDataBuffer, WORD wDataSize) 
{
	//数据验证 
	ASSERT(INVALID_CHAIR!=wChairID) ;
	if(INVALID_CHAIR==wChairID) return false ;

	ASSERT(sizeof(CMD_C_SegCard)==wDataSize) ;
	if(sizeof(CMD_C_SegCard)!=wDataSize) return false ;

	CMD_C_SegCard *pSetSeg = (CMD_C_SegCard*) pDataBuffer ;

	//发送数据
	CMD_S_SetSegment SetSegment ;
	ZeroMemory(&SetSegment, sizeof(SetSegment)) ;

	SetSegment.SegmentType = pSetSeg->SegmentType ; 
	SetSegment.wCurrentUser=wChairID ;

	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_SETSEGMENT,&SetSegment,sizeof(SetSegment));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_SETSEGMENT,&SetSegment,sizeof(SetSegment)); 

	return true ;
}

//玩家摊牌
bool CTableFrameSink::OnUserShowCard(const WORD wChairID , const void * pDataBuffer, WORD wDataSize)
{
	//数据验证 
	ASSERT(INVALID_CHAIR!=wChairID) ;
	if(INVALID_CHAIR==wChairID) return false ;

	ASSERT(sizeof(CMD_C_ShowCard)==wDataSize) ;
	if(sizeof(CMD_C_ShowCard)!=wDataSize) return false ;

	CMD_C_ShowCard *pSetSeg = (CMD_C_ShowCard*) pDataBuffer ;
	
	//校验扑克
	BYTE cbCardDataTemp[HAND_CARD_COUNT];
	ZeroMemory(cbCardDataTemp, sizeof(cbCardDataTemp));
	CopyMemory(&cbCardDataTemp[0], pSetSeg->bFrontCard, 3);
	CopyMemory(&cbCardDataTemp[3], pSetSeg->bMidCard,	 5);
	CopyMemory(&cbCardDataTemp[8], pSetSeg->bBackCard,  5);

	for(BYTE i=0; i<HAND_CARD_COUNT; i++)
	{
		for(BYTE j=0; j<HAND_CARD_COUNT; j++)
		{
			if(cbCardDataTemp[i] == m_bUserCardData[wChairID][j]) break;
		}
		if(HAND_CARD_COUNT == j) return false;
	}

	//设置数据
	CopyMemory(m_bSegmentCard[wChairID][0], pSetSeg->bFrontCard, 3);
	CopyMemory(m_bSegmentCard[wChairID][1], pSetSeg->bMidCard,	 5);
	CopyMemory(m_bSegmentCard[wChairID][2], pSetSeg->bBackCard,  5);
	m_bFinishSegment[wChairID] = true;
	m_wShowCardCount++;

	//通知开牌
	CMD_S_ShowCard  ShowCard ;
	ZeroMemory(&ShowCard, sizeof(ShowCard));
	for (WORD i = 0; i < GAME_PLAYER; i++)
	{
		IServerUserItem *pIServerUserItem = m_pITableFrame->GetTableUserItem(i);
		if ((pIServerUserItem == NULL) || (m_bGameStatus[i] == false))	continue;

		ShowCard.wCurrentUser = wChairID;

		if (i == wChairID)
		{
			ShowCard.bCanSeeShowCard  = true;
			CopyMemory(ShowCard.bFrontCard, m_bSegmentCard[wChairID][0], 3);
			CopyMemory(ShowCard.bMidCard,   m_bSegmentCard[wChairID][1], 5);
			CopyMemory(ShowCard.bBackCard,  m_bSegmentCard[wChairID][2], 5);
		}
		else
			ShowCard.bCanSeeShowCard  = false;

		m_pITableFrame->SendTableData(i,SUB_S_SHOW_CARD,&ShowCard,sizeof(ShowCard));
		m_pITableFrame->SendLookonData(i,SUB_S_SHOW_CARD,&ShowCard,sizeof(ShowCard)); 
	}

	//开始比牌
	StartCompareCard();

	return true ;
}

//获取下注
void CTableFrameSink::GetChips( LONGLONG lChipArray[GAME_PLAYER][SET_CHIP_COUNT])
{
	//结果清零
	ZeroMemory(lChipArray, sizeof(lChipArray));
	
	LONGLONG lBankerScore = m_pITableFrame->GetTableUserItem( m_wBanker )->GetUserScore();
	LONGLONG m_wMaxChip = m_pGameServiceOption->lRestrictScore;
	for(BYTE i=0; i<GAME_PLAYER;i++)
	{
		IServerUserItem *pIServerUserItem = m_pITableFrame->GetTableUserItem(i);
		if ((pIServerUserItem == NULL)) continue;

		//最大下注
		LONGLONG lMeScore, lMaxChip ;
		lMeScore = pIServerUserItem->GetUserScore() ;

		if (lMeScore > m_wMaxChip && m_wMaxChip != 0) lMeScore = m_wMaxChip;
		if (lBankerScore > m_wMaxChip && m_wMaxChip != 0) lBankerScore = m_wMaxChip;

		lMaxChip = (min(lMeScore,  lBankerScore))/36;

		for (BYTE cbChipIndex = 0; cbChipIndex < SET_CHIP_COUNT; ++cbChipIndex)
		{
			lChipArray[i][cbChipIndex] = LONGLONG ((cbChipIndex + 1.0 ) / SET_CHIP_COUNT * lMaxChip) ;

			////整百处理
			//if (lChipArray[cbChipIndex] > 100)
			//{
			//	LONGLONG lHundredCount = lChipArray[cbChipIndex] / 100;
			//	lChipArray[cbChipIndex] = lHundredCount * 100 ;
			//}
		}
	}
	
	return;
}

//获取下注
void CTableFrameSink::GetChips(WORD wChairID, LONGLONG lChipArray[SET_CHIP_COUNT])
{
	//结果清零
	ZeroMemory(lChipArray, sizeof(lChipArray));
	
	LONGLONG lBankerScore = m_pITableFrame->GetTableUserItem( m_wBanker )->GetUserScore();
	LONGLONG m_wMaxChip = m_pGameServiceOption->lRestrictScore;
	
	//最大下注
	LONGLONG lMeScore, lMaxChip ;
	lMeScore = m_pITableFrame->GetTableUserItem(wChairID)->GetUserScore() ;

	if (lMeScore > m_wMaxChip && m_wMaxChip != 0) lMeScore = m_wMaxChip;
	if (lBankerScore > m_wMaxChip && m_wMaxChip != 0) lBankerScore = m_wMaxChip;

	lMaxChip = (min(lMeScore,  lBankerScore))/36;

	for (BYTE cbChipIndex = 0; cbChipIndex < SET_CHIP_COUNT; ++cbChipIndex)
	{
		lChipArray[cbChipIndex] = LONGLONG ((cbChipIndex + 1.0 ) / SET_CHIP_COUNT * lMaxChip) ;

		////整百处理
		//if (lChipArray[cbChipIndex] > 100)
		//{
		//	LONGLONG lHundredCount = lChipArray[cbChipIndex] / 100;
		//	lChipArray[cbChipIndex] = lHundredCount * 100 ;
		//}
	}

	return;
}

//获取倍数
int CTableFrameSink::GetCardTimes(WORD wUserID)
{
	//组合扑克
	BYTE bCardData[13] = {0};
	CopyMemory(bCardData,	m_bSegmentCard[wUserID][0], sizeof(BYTE)*3);
	CopyMemory(bCardData+3, m_bSegmentCard[wUserID][1], sizeof(BYTE)*5);
	CopyMemory(bCardData+8, m_bSegmentCard[wUserID][2], sizeof(BYTE)*5);

	//获取牌型
	BYTE cbType = m_GameLogic.GetSpecialType(m_bUserCardData[wUserID], 13);

	if (cbType == CT_EX_ZHIZUNQINGLONG) return 36;
	else if (cbType == CT_EX_YITIAOLONG) return 16;
	else if (cbType == CT_EX_LIUDUIBAN) return 3;
	else if (cbType == CT_EX_SANSHUNZI) return 3;
	else if (cbType == CT_EX_SANTONGHUA) return 3;

	//牌型翻倍
	BYTE cbMaxCard = 0;
	int nMultiple[3] = {1,1,1};
	BYTE bFrontCT = m_GameLogic.GetCardType(m_bSegmentCard[wUserID][0], 3, cbMaxCard);
	BYTE bMidCT	  = m_GameLogic.GetCardType(m_bSegmentCard[wUserID][1], 5, cbMaxCard);
	BYTE bBackCT  = m_GameLogic.GetCardType(m_bSegmentCard[wUserID][2], 5, cbMaxCard);
			
	//第一注
	if (bFrontCT == CT_THREE)						nMultiple[0] = 3; //三条
				
	//第二注
	if (bMidCT >= CT_FIVE_STRAIGHT_FLUSH_FIRST_A)		nMultiple[1] = 10; //同花顺
	else if (bMidCT == CT_FIVE_FOUR_ONE)				nMultiple[1] = 8;//铁支					
	else if (bMidCT == CT_FIVE_THREE_DEOUBLE)			nMultiple[1] = 8;//葫芦

	//第三注			
	if (bBackCT >= CT_FIVE_STRAIGHT_FLUSH_FIRST_A)		nMultiple[2] = 5;	 //同花顺			
	else if (bBackCT == CT_FIVE_FOUR_ONE)				nMultiple[2] = 4; //铁支

	int nResult = nMultiple[0]+nMultiple[1]+nMultiple[2];
	return nResult;

}

//获取倍数
void CTableFrameSink::CompareCardGetTimes()
{
	//庄闲比牌
	for(WORD i=0 ; i<GAME_PLAYER ; i++)
	{
		IServerUserItem *pIServerUserItem = m_pITableFrame->GetTableUserItem(i);
		if ((pIServerUserItem == NULL) || (m_bGameStatus[i] == false)) continue;

		//计算输赢
		if (i != m_wBanker)
		{
			if(m_bSpecialType[i]!=0||m_bSpecialType[m_wBanker]!=0)
			{
				continue;
			}
			
			//变量定义
			BYTE bWindInfo[3] = {0};

			//检查胜负	
			if(enCRGreater == m_GameLogic.CompareCard(m_bSegmentCard[m_wBanker][0] , m_bSegmentCard[i][0] , 3 , 3 , true)) bWindInfo[0] = TRUE;
			else bWindInfo[0] = FALSE;
			if(enCRGreater == m_GameLogic.CompareCard(m_bSegmentCard[m_wBanker][1] , m_bSegmentCard[i][1] , 5 , 5 , true)) bWindInfo[1] = TRUE;
			else bWindInfo[1] = FALSE;
			if(enCRGreater == m_GameLogic.CompareCard(m_bSegmentCard[m_wBanker][2] , m_bSegmentCard[i][2] , 5 , 5 , true)) bWindInfo[2] = TRUE;
			else bWindInfo[2] = FALSE;

			//判断输赢
			for (BYTE bIdx=0; bIdx<3; bIdx++)
			{
				WORD wWinUser = (bWindInfo[bIdx] == TRUE) ? i : m_wBanker;
				INT nMultiple = (bWindInfo[bIdx] == TRUE) ? 1 : -1;

				//牌型翻倍
				BYTE cbMaxCard = 0;
				BYTE bFrontCT = m_GameLogic.GetCardType(m_bSegmentCard[wWinUser][0], 3, cbMaxCard);
				BYTE bMidCT	  = m_GameLogic.GetCardType(m_bSegmentCard[wWinUser][1], 5, cbMaxCard);
				BYTE bBackCT  = m_GameLogic.GetCardType(m_bSegmentCard[wWinUser][2], 5, cbMaxCard);

				//第一注
				if (bIdx == 0)
				{					
					//第一注三条
					if (bFrontCT == CT_THREE)						nMultiple *= 3;
				}

				//第二注
				if (bIdx == 1)
				{						
					//同花顺
					if (bMidCT >= CT_FIVE_STRAIGHT_FLUSH_FIRST_A)	nMultiple *= 10;

					//铁支		
					if (bMidCT == CT_FIVE_FOUR_ONE)					nMultiple *= 8;

					//葫芦
					if (bMidCT == CT_FIVE_THREE_DEOUBLE)			nMultiple *= 8;
				}

				//第三注
				if (bIdx == 2)
				{
					//同花顺
					if (bBackCT >= CT_FIVE_STRAIGHT_FLUSH_FIRST_A)	nMultiple *= 5;	

					//铁支
					if (bBackCT == CT_FIVE_FOUR_ONE)				nMultiple *= 4;
				}

				m_lScoreTimes[i][bIdx]		   = nMultiple;
				m_lScoreTimes[m_wBanker][bIdx]-= nMultiple;
			}
		}
	}
}


//用户坐下
bool CTableFrameSink::OnActionUserSitDown(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	//历史积分
	if (bLookonUser==false)
	{
		ASSERT(wChairID!=INVALID_CHAIR);
		m_HistoryScore.OnEventUserEnter(wChairID);
	}

	return true;
}

//用户起来
bool CTableFrameSink::OnActionUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	if(false == bLookonUser)
	{
		if (m_wFirstCallBankerUser == wChairID)
		{
			m_wFirstCallBankerUser = m_wCurrentCaller;
		}	
		
		ASSERT(wChairID!=INVALID_CHAIR);
		m_HistoryScore.OnEventUserLeave(wChairID);
	}

	return true;
}

//游戏人数
DWORD CTableFrameSink::GetPlayerCount()
{
	//计算人数
	WORD wPlayerCount = 0;
	for(WORD i = 0; i < GAME_PLAYER; i++)
	{
		IServerUserItem *pIServerUserItem = m_pITableFrame->GetTableUserItem(i);
		if (m_bGameStatus[i] == false)continue;
		if (pIServerUserItem) wPlayerCount++;
	}

	return wPlayerCount;
}

//开始比牌
VOID CTableFrameSink::StartCompareCard()
{
	if (GetPlayerCount() == m_wShowCardCount)
	{
		CMD_S_CompareCard CompareCard;
		CompareCard.cbPlayCount = GetPlayerCount();
		ZeroMemory (&CompareCard, sizeof(CompareCard));

		CompareCard.wBankUser = m_wBanker;
		CopyMemory(CompareCard.bSegmentCard, m_bSegmentCard, sizeof(m_bSegmentCard));

		//是否特殊牌
		bool bConclude = false;
		WORD wSpecUser = INVALID_CHAIR;
		for(WORD j=0 ; j<GAME_PLAYER ; j++)
		{
			IServerUserItem *pIServerUserItem = m_pITableFrame->GetTableUserItem(j);
			if ((pIServerUserItem == NULL) || (m_bGameStatus[j] == false)) continue;

			//组合扑克
			BYTE bCardData[13] = {0};
			CopyMemory(bCardData,	m_bSegmentCard[j][0], sizeof(BYTE)*3);
			CopyMemory(bCardData+3, m_bSegmentCard[j][1], sizeof(BYTE)*5);
			CopyMemory(bCardData+8, m_bSegmentCard[j][2], sizeof(BYTE)*5);

			//获取牌型
			BYTE cbType = m_GameLogic.GetSpecialType(bCardData, 13);
			if (cbType == CT_EX_ZHIZUNQINGLONG)
			{
				m_bSpecialType[j] = CT_EX_ZHIZUNQINGLONG;
				bConclude = true;
				wSpecUser = j;
			}
			else if (cbType == CT_EX_YITIAOLONG)
			{
				m_bSpecialType[j] = CT_EX_YITIAOLONG;
				bConclude = true;
				wSpecUser = j;
			}			
			else if (m_GameLogic.GetDoubleCount(m_bSegmentCard[j][0], m_bSegmentCard[j][1], m_bSegmentCard[j][2]) == 6)
			{
				m_bSpecialType[j] = CT_EX_LIUDUIBAN;
				bConclude = true;
				wSpecUser = j;
			}
			else if (m_GameLogic.IsLinkCard(m_bSegmentCard[j][0], 3) && 
				m_GameLogic.IsLinkCard(m_bSegmentCard[j][1], 5) && 
				m_GameLogic.IsLinkCard(m_bSegmentCard[j][2], 5))
			{
				m_bSpecialType[j] = CT_EX_SANSHUNZI;
				bConclude = true;
				wSpecUser = j;
			}
			else if (m_GameLogic.IsSameColorCard(m_bSegmentCard[j][0], 3) && 
				m_GameLogic.IsSameColorCard(m_bSegmentCard[j][1], 5) && 
				m_GameLogic.IsSameColorCard(m_bSegmentCard[j][2], 5))
			{
				m_bSpecialType[j] = CT_EX_SANTONGHUA;
				bConclude = true;
				wSpecUser = j;
			}
		}

		//有特殊牌，不用比牌，直接结算
		if (bConclude == true)
		{
			CopyMemory(CompareCard.bSpecialType, m_bSpecialType, sizeof(m_bSpecialType));
			CopyMemory(CompareCard.lScoreTimes, m_lScoreTimes, sizeof(m_lScoreTimes));
			CompareCard.bCompared = FALSE;
			m_bCompared = CompareCard.bCompared;

			CompareCardGetTimes();

			//发送数据
			m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_COMPARE_CARD, &CompareCard, sizeof(CompareCard));
			m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_COMPARE_CARD, &CompareCard, sizeof(CompareCard)); 

			ASSERT(wSpecUser != INVALID_CHAIR);
			OnEventGameConclude(wSpecUser,NULL,GER_NORMAL);
			return;
		}
		

		CompareCardGetTimes();
		CopyMemory(CompareCard.bSpecialType, m_bSpecialType, sizeof(m_bSpecialType));
		CopyMemory(CompareCard.lScoreTimes, m_lScoreTimes, sizeof(m_lScoreTimes));
		CompareCard.bCompared = TRUE;
		m_bCompared = CompareCard.bCompared;

		//发送数据
		m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_COMPARE_CARD , &CompareCard , sizeof(CompareCard));
		m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_COMPARE_CARD , &CompareCard , sizeof(CompareCard)); 
	}

	return;
}

//作弊用户
bool CTableFrameSink::OnCheatCard(IServerUserItem * pIServerUserItem)
{
	if(m_pServerControl)
	{
		CMD_S_CheatCard CheatCard ;
		ZeroMemory(&CheatCard, sizeof(CheatCard));

		for(WORD i = 0; i < GAME_PLAYER; i++)
		{
			CheatCard.wCardUser[CheatCard.cbUserCount] = i;
			CheatCard.cbCardCount[CheatCard.cbUserCount] = HAND_CARD_COUNT;
			CopyMemory(CheatCard.cbCardData[CheatCard.cbUserCount++],m_bUserCardData[i],sizeof(BYTE)*HAND_CARD_COUNT);
		}
		//发送作弊
		//WB改 这里这样发包存在BUG
		//m_pITableFrame->SendTableData(wChairID,SUB_S_CHEAT_CARD,&CheatCard,sizeof(CheatCard));
		//m_pITableFrame->SendLookonData(wChairID,SUB_S_CHEAT_CARD,&CheatCard,sizeof(CheatCard));
		m_pServerControl->ServerControl(&CheatCard, m_pITableFrame, pIServerUserItem);
	}

	return true;
}

//发送作弊信息
void CTableFrameSink::SendCheatCard()
{
	if(m_pServerControl)
	{
		CMD_S_CheatCard CheatCard ;
		ZeroMemory(&CheatCard, sizeof(CheatCard));

		for(WORD i = 0; i < GAME_PLAYER; i++)
		{
			CheatCard.wCardUser[CheatCard.cbUserCount] = i;
			CheatCard.cbCardCount[CheatCard.cbUserCount] = HAND_CARD_COUNT;
			CopyMemory(CheatCard.cbCardData[CheatCard.cbUserCount++],m_bUserCardData[i],sizeof(BYTE)*HAND_CARD_COUNT);
		}
		m_pServerControl->ServerControl(&CheatCard, m_pITableFrame, m_bGameStatus);
	}
}

bool CTableFrameSink::OnActionUserOnReady( WORD wChairID, IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize )
{
	return true; 
}
//
void CTableFrameSink::ResultsControl(int iCount)
{
	//是否控制
	if(!IsNeedControl()) return;

	if(iCount > 5)
	{	
		return;
	}

	bool bControlSuccess = false;

	BYTE bOptimalOrder[4];   //把最优的牌型保存
	BYTE bTempCard[4][13];   //保存系统所发的牌
	BYTE bSpecialType[4];    //保存特殊牌型

	BYTE bSubscript[4];      //下标是每个人对应的牌组
	//BOOL bAllWin,bAllLose;   //是否通杀、通赔
	SCORE sPalyScore[4];     //玩家得分
	SCORE sTotalScore;       //总分
	SCORE sOptimalScore;     //最优得分

	ZeroMemory(bOptimalOrder,sizeof(bOptimalOrder));
	ZeroMemory(bSpecialType,sizeof(bSpecialType));
	ZeroMemory(sPalyScore,sizeof(sPalyScore));
	CopyMemory(bTempCard,m_bUserCardData,sizeof(bTempCard));

	//判断是否为特殊牌型
	for(int i = 0 ; i < GAME_PLAYER ;i ++)
	{
		bSpecialType[i] = m_GameLogic.GetSpecialType(bTempCard[i],13);
		if(bSpecialType[i] == CT_EX_INVALID)
		{
			bool bIsAndroidUser = false;
			IServerUserItem *pIServerUserItem = m_pITableFrame->GetTableUserItem(i);
			if(NULL != pIServerUserItem && pIServerUserItem->IsAndroidUser())
			{
				bIsAndroidUser = true;
			}
			m_GameLogic.AutoPutCard(m_bUserCardData[i],bTempCard[i], bIsAndroidUser, true);
		}
	}

	//对牌组进行全排列
	for(bSubscript[0] = 0; bSubscript[0] < 4; bSubscript[0] ++)
	for(bSubscript[1] = 0; bSubscript[1] < 4; bSubscript[1] ++)
	for(bSubscript[2] = 0; bSubscript[2] < 4; bSubscript[2] ++)
	for(bSubscript[3] = 0; bSubscript[3] < 4; bSubscript[3] ++)
	{
		do{
			//每个玩家的下标要不相同
			if(bSubscript[0] == bSubscript[1] || bSubscript[0] == bSubscript[2] || bSubscript[0] == bSubscript[3])
			{
				break;
			}
			if(bSubscript[1] == bSubscript[2] || bSubscript[1] == bSubscript[3])
			{
				break;
			}
			if(bSubscript[2] == bSubscript[3])
			{
				break;
			}
			
			//计算如此发牌的输赢状况
			//bAllWin = TRUE;
			//bAllLose = TRUE;
			ZeroMemory(sPalyScore,sizeof(sPalyScore));
			for(int i = 0; i < GAME_PLAYER; i ++)
			{
				if(i == m_wBanker || m_lUserChip[i] == 0)
				{
					continue;
				}

				BYTE bActualBanker = bSubscript[m_wBanker];
				BYTE bActualI      = bSubscript[i];
				int nMulOnSD = 0;
				//特殊牌型比较
				if(bSpecialType[bActualBanker] != CT_EX_INVALID || bSpecialType[bActualI] != CT_EX_INVALID)
				{
					if(bSpecialType[bActualBanker] >= bSpecialType[bActualI])
					{
						nMulOnSD += CardTypeToScore(bSpecialType[bActualBanker],0);
					}
					else
					{
						nMulOnSD -= CardTypeToScore(bSpecialType[bActualI],0);
					}

				}
				else
				{
					//变量定义
					BYTE cbMaxCard = 0;
					//第一注比牌得分
					if(enCRGreater == m_GameLogic.CompareCard(bTempCard[bActualBanker] , bTempCard[bActualI] , 3 , 3 , true))
					{
						nMulOnSD -= CardTypeToScore(m_GameLogic.GetCardType(bTempCard[bActualI] , 3,cbMaxCard),1);
					}
					else
					{
						nMulOnSD += CardTypeToScore(m_GameLogic.GetCardType(bTempCard[bActualBanker] , 3,cbMaxCard),1);

					}
					//第二注比牌
					if(enCRGreater == m_GameLogic.CompareCard(bTempCard[bActualBanker] + 3 , bTempCard[bActualI] + 3, 5 , 5 , true)) 
					{
						nMulOnSD -= CardTypeToScore(m_GameLogic.GetCardType(bTempCard[bActualI] + 3 , 5,cbMaxCard),2);
					}
					else
					{
						nMulOnSD += CardTypeToScore(m_GameLogic.GetCardType(bTempCard[bActualBanker] + 3, 5,cbMaxCard),2);

					}
					//第三注比牌
					if(enCRGreater == m_GameLogic.CompareCard(bTempCard[bActualBanker] + 8 , bTempCard[bActualI] + 8, 5 , 5 , true))
					{
						nMulOnSD -= CardTypeToScore(m_GameLogic.GetCardType(bTempCard[bActualI] + 8, 5,cbMaxCard),3);
					}
					else
					{
						nMulOnSD += CardTypeToScore(m_GameLogic.GetCardType(bTempCard[bActualBanker] + 8, 5,cbMaxCard),3);

					}
				}
				////判断通吃通赔
				//if(nMulOnSD > 0)
				//{
				//	bAllLose = FALSE;
				//}
				//if(nMulOnSD < 0)
				//{
				//	bAllWin = FALSE;
				//}
				sPalyScore[m_wBanker] += nMulOnSD * m_lUserChip[i];
				sPalyScore[i] -= nMulOnSD * m_lUserChip[i];
			}
			sTotalScore = 0;
			//计算机器人总输赢
			for(int i = 0; i < GAME_PLAYER; i ++)
			{
				if(sPalyScore[i] != 0 && m_pITableFrame->GetTableUserItem(i)->IsAndroidUser())
				{
					sTotalScore += sPalyScore[i];
				}
			}

			if(TRUE == m_bIsWin && sTotalScore > 0)
			{
				if(bOptimalOrder[0] == bOptimalOrder[1] || sTotalScore > sOptimalScore )
				{
					for(int i = 0; i < GAME_PLAYER; i ++)
					{
						bOptimalOrder[i] = bSubscript[i];
					}
					sOptimalScore = sTotalScore;
					bControlSuccess = true;
				}
			}
 			if(FALSE == m_bIsWin && sTotalScore <= 0 && m_sRobotLnventory + sTotalScore >= 0)
 			{
 				if((bOptimalOrder[0] == bOptimalOrder[1] || sTotalScore < sOptimalScore))
 				{
 					for(int i = 0; i < GAME_PLAYER; i ++)
 					{
 						bOptimalOrder[i] = bSubscript[i];
 					}
 					sOptimalScore = sTotalScore;
 					bControlSuccess = true;
 				}
 			}
		}while(0);
	}
	
	

	//如果控制部不成功重新发牌
	if(bOptimalOrder[0] != bOptimalOrder[1])
	{
// 		BYTE bCardData[52] ;
// 		m_GameLogic.RandCardList(bCardData , 52) ;
// 		CopyMemory(m_bUserCardData, bCardData, sizeof(BYTE)*52);
// 
// 		ResultsControl(iCount + 1);
// 		return ;
// 	}
// 	else
// 	{
		CopyMemory(bTempCard,m_bUserCardData,sizeof(bTempCard));
		for(int i = 0; i < GAME_PLAYER; i++)
		{
			CopyMemory(m_bUserCardData[i],bTempCard[bOptimalOrder[i]],sizeof(BYTE) * 13);
		}

	//	SendInfo(m_bIsWin ? TEXT("控制赢牌！") : TEXT("控制输牌！"));
	}

}

int CTableFrameSink::CardTypeToScore(BYTE bCardType,BYTE bSpecies)
{
	//特殊牌型
	if(bSpecies == 0)
	{
		switch(bCardType)
		{
		case CT_EX_ZHIZUNQINGLONG://至尊青龙
			{
				return 36;
			}
		case CT_EX_YITIAOLONG://一条龙
			{
				return 13;
			}
		case CT_EX_LIUDUIBAN://六对半
		case CT_EX_SANSHUNZI://三顺子
		case CT_EX_SANTONGHUA://三同花
			{
				return 3;
			}
		}
		return 0;
	}
	//第一注
	if (bSpecies == 1)
	{	
		switch(bCardType)
		{
		case CT_THREE://三条
			{
				return 3;
			}
		}
		return 1;
	}
	//第二注
	if (bSpecies == 2)
	{	
		switch(bCardType)
		{
		case CT_FIVE_STRAIGHT_FLUSH_FIRST_A://A同花顺
		case CT_FIVE_STRAIGHT_FLUSH://同花顺牌:
			{
				return 10;
			}
		case CT_FIVE_FOUR_ONE://铁支                           
		case CT_FIVE_THREE_DEOUBLE://葫芦
			{
				return 8;
			}
		}
		return 1;
	}
	//第三注
	if (bSpecies == 3)
	{
		switch(bCardType)
		{
		case CT_FIVE_STRAIGHT_FLUSH_FIRST_A://A同花顺
		case CT_FIVE_STRAIGHT_FLUSH://同花顺牌:
			{
				return 5;
			}
		case CT_FIVE_FOUR_ONE://铁支                           
			{
				return 4;
			}
		}
		return 1;
	}
	return 0;

}
//判断机器人输赢结果
void CTableFrameSink::JudgeResults()
{
	
	ReadConfiguration();

	SCORE sPalyScore[4];     //玩家得分
	BYTE bSpecialType[4];    //保存特殊牌型
	BYTE bTempCard[4][13];   //保存系统所发的牌	

	ZeroMemory(bSpecialType,sizeof(bSpecialType));
	ZeroMemory(sPalyScore,sizeof(sPalyScore));
	CopyMemory(bTempCard,m_bUserCardData,sizeof(bTempCard));

	//判断是否为特殊牌型
	for(int i = 0 ; i < GAME_PLAYER ;i ++)
	{
		bSpecialType[i] = m_GameLogic.GetSpecialType(bTempCard[i],13);
		if(bSpecialType[i] == CT_EX_INVALID)
		{
			bool bIsAndroidUser = false;
			IServerUserItem *pIServerUserItem = m_pITableFrame->GetTableUserItem(i);
			if(NULL != pIServerUserItem && pIServerUserItem->IsAndroidUser())
			{
				bIsAndroidUser = true;
			}
			m_GameLogic.AutoPutCard(m_bUserCardData[i],bTempCard[i], bIsAndroidUser, true);
		}
	}

	for(int i = 0; i < GAME_PLAYER; i ++)
	{
		if(i == m_wBanker || m_lUserChip[i] == 0)
		{
			continue;
		}

		BYTE bActualBanker = m_wBanker;
		BYTE bActualI      = i;
		int nMulOnSD = 0;
		//特殊牌型比较
		if(bSpecialType[bActualBanker] != CT_EX_INVALID || bSpecialType[bActualI] != CT_EX_INVALID)
		{
			if(bSpecialType[bActualBanker] >= bSpecialType[bActualI])
			{
				nMulOnSD += CardTypeToScore(bSpecialType[bActualBanker],0);
			}
			else
			{
				nMulOnSD -= CardTypeToScore(bSpecialType[bActualI],0);
			}

		}
		else
		{
			//变量定义
			BYTE cbMaxCard = 0;
			//第一注比牌得分
			if(enCRGreater == m_GameLogic.CompareCard(bTempCard[bActualBanker] , bTempCard[bActualI] , 3 , 3 , true))
			{
				nMulOnSD -= CardTypeToScore(m_GameLogic.GetCardType(bTempCard[bActualI] , 3,cbMaxCard),1);
			}
			else
			{
				nMulOnSD += CardTypeToScore(m_GameLogic.GetCardType(bTempCard[bActualBanker] , 3,cbMaxCard),1);

			}
			//第二注比牌
			if(enCRGreater == m_GameLogic.CompareCard(bTempCard[bActualBanker] + 3 , bTempCard[bActualI] + 3, 5 , 5 , true)) 
			{
				nMulOnSD -= CardTypeToScore(m_GameLogic.GetCardType(bTempCard[bActualI] + 3 , 5,cbMaxCard),2);
			}
			else
			{
				nMulOnSD += CardTypeToScore(m_GameLogic.GetCardType(bTempCard[bActualBanker] + 3, 5,cbMaxCard),2);

			}
			//第三注比牌
			if(enCRGreater == m_GameLogic.CompareCard(bTempCard[bActualBanker] + 8 , bTempCard[bActualI] + 8, 5 , 5 , true))
			{
				nMulOnSD -= CardTypeToScore(m_GameLogic.GetCardType(bTempCard[bActualI] + 8, 5,cbMaxCard),3);
			}
			else
			{
				nMulOnSD += CardTypeToScore(m_GameLogic.GetCardType(bTempCard[bActualBanker] + 8, 5,cbMaxCard),3);

			}
		}
		////判断通吃通赔
		//if(nMulOnSD > 0)
		//{
		//	bAllLose = FALSE;
		//}
		//if(nMulOnSD < 0)
		//{
		//	bAllWin = FALSE;
		//}
		sPalyScore[m_wBanker] += nMulOnSD * m_lUserChip[i];
		sPalyScore[i] -= nMulOnSD * m_lUserChip[i];
	}
	SCORE sTotalScore = 0;
	//计算机器人总输赢
	for(int i = 0; i < GAME_PLAYER; i ++)
	{
		if(sPalyScore[i] != 0 && m_pITableFrame->GetTableUserItem(i)->IsAndroidUser())
		{
			sTotalScore += sPalyScore[i];
		}
	}

	//判断机器人这一局是要输还是赢
	m_bIsWin = FALSE;
	int iRandom = rand() % 100;
	if(m_sRobotLnventory < 0 && m_sRobotLnventory + ROBOT_LNVENTORY_MIN == 0)
	{
		ASSERT(NULL);
		m_bIsWin = TRUE;
	}
	else if(m_sRobotLnventory == ROBOT_LNVENTORY_MAX)
	{
		ASSERT(NULL);
		m_bIsWin = FALSE;
	}
	else if(m_sRobotLnventory + sTotalScore <= m_sRegionalControl[0])
	{
		if(iRandom < m_bRegionalProbability[0])
		{
			m_bIsWin = TRUE;
		}
	}
	else if(m_sRobotLnventory + sTotalScore<= m_sRegionalControl[1])
	{
		if(iRandom < m_bRegionalProbability[1])
		{
			m_bIsWin = TRUE;
		}
	}
	else if(m_sRobotLnventory + sTotalScore <= m_sRegionalControl[2])
	{
		if(iRandom < m_bRegionalProbability[2])
		{
			m_bIsWin = TRUE;
		}
	}
	else if(m_sRobotLnventory + sTotalScore <= m_sRegionalControl[3])
	{
		if(iRandom < m_bRegionalProbability[3])
		{
			m_bIsWin = TRUE;
		}
	}
	else if(m_sRobotLnventory + sTotalScore <= m_sRegionalControl[4])
	{
		if(iRandom < m_bRegionalProbability[4])
		{
			m_bIsWin = TRUE;
		}
	}
	else if(m_sRobotLnventory + sTotalScore <= m_sRegionalControl[5])
	{
		if(iRandom < m_bRegionalProbability[5])
		{
			m_bIsWin = TRUE;
		}
	}
	else if(m_sRobotLnventory + sTotalScore <= m_sRegionalControl[6])
	{
		if(iRandom < m_bRegionalProbability[6])
		{
			m_bIsWin = TRUE;
		}
	}
	else if(m_sRobotLnventory + sTotalScore <= m_sRegionalControl[7])
	{
		if(iRandom < m_bRegionalProbability[7])
		{
			m_bIsWin = TRUE;
		}
	}
	else
	{
		if(iRandom < m_bRegionalProbability[8])
		{
			m_bIsWin = TRUE;
		}
	}

	ASSERT(m_sRobotLnventory + sTotalScore >= 0 || m_sRobotLnventory + sTotalScore < 0 && m_bIsWin == TRUE);

	//防止负分
	if (sTotalScore < 0 && m_sRobotLnventory < -1 * sTotalScore)
	{
		m_bIsWin = TRUE;
	}

	return;
}
//读取配置文件
void CTableFrameSink::ReadConfiguration()
{
	CString sPath,sKey,sMsg,sTime,sDate;
	TCHAR buffer[MAX_PATH];

	// 得到初始化文件的全路径
	GetModuleFileName(NULL, buffer, MAX_PATH);
	sPath = CString(buffer);
	sPath = sPath.Left(sPath.ReverseFind('\\'));
	sPath += "\\Thirteen.ini";

	//房间号
	sKey.Format(_T("%d"),m_pGameServiceOption->wServerID);

	//读入库存
	GetPrivateProfileString(sKey,_T("RobotLnventory"),_T("0"),sDate.GetBuffer(MAX_PATH),MAX_PATH,sPath);
	m_sRobotLnventory = _ttoi64(sDate);
	

	for(int i = 0; i < 9; i ++)
	{
		sMsg.Format(_T("RegionalProbability[%d]"),i);
		m_bRegionalProbability[i] = GetPrivateProfileInt(sKey,sMsg,50,sPath);
		
	}

	for(int i = 0; i < 8; i ++)
	{
		sMsg.Format(_T("RegionalControl[%d]"),i);
		GetPrivateProfileString(sKey,sMsg,_T("10"),sDate.GetBuffer(MAX_PATH),MAX_PATH,sPath);
		m_sRegionalControl[i] = _ttoi64(sDate);

		
	}	
}

//是否控制
bool CTableFrameSink::IsNeedControl()
{
	bool bHasUser = false;
	bool bHasAndroid = false;
	bool bNeedControl = false;
	for(int i = 0 ; i < GAME_PLAYER ;i ++)
	{
		IServerUserItem *pIServerUserItem = m_pITableFrame->GetTableUserItem(i);
		if(NULL == pIServerUserItem) continue;

		if(pIServerUserItem->IsAndroidUser())
		{
			bHasAndroid = true;
		}
		else
		{
			bHasUser = true;
		}
		if(bHasAndroid && bHasUser)
		{
			bNeedControl = true;
			break;
		}
	}

	return bNeedControl;
}

//库存消息
VOID CTableFrameSink::SendControlMessage(IServerUserItem * pIServerUserItem)
{
	if(NULL == pIServerUserItem) return;
	if(!CUserRight::IsGameCheatUser(pIServerUserItem->GetUserRight())) return;

	TCHAR szControlMessage[256];				
	_sntprintf(szControlMessage,CountArray(szControlMessage),TEXT("当前库存：%I64d"),m_sRobotLnventory);

	m_pITableFrame->SendGameMessage(pIServerUserItem,szControlMessage,SMT_CHAT);

	return;
}

//发送消息
void CTableFrameSink::SendInfo(CString str, WORD wChairID)
{
	//通知消息
	TCHAR szMessage[128]=TEXT("");
	_sntprintf(szMessage,CountArray(szMessage),TEXT("%s"), str);

	//用户状态
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//获取用户
		IServerUserItem *pIServerUser=m_pITableFrame->GetTableUserItem(i);
		if(pIServerUser != NULL)
		{
			if (CUserRight::IsGameCheatUser(pIServerUser->GetUserRight()))
			{				
				m_pITableFrame->SendGameMessage(pIServerUser,szMessage,SMT_CHAT);
			}
		}
	}

	int nLookonCount = 0;
	IServerUserItem* pLookonUserItem = m_pITableFrame->EnumLookonUserItem(nLookonCount);
	while( pLookonUserItem )
	{
		if (CUserRight::IsGameCheatUser(pLookonUserItem->GetUserRight()))
		{				
			m_pITableFrame->SendGameMessage(pLookonUserItem,szMessage,SMT_CHAT);
		}

		nLookonCount++;
		pLookonUserItem = m_pITableFrame->EnumLookonUserItem(nLookonCount);
	}
}
//////////////////////////////////////////////////////////////////////////

