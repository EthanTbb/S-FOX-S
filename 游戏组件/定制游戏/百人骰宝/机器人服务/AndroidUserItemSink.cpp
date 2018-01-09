#include "Stdafx.h"
#include "AndroidUserItemSink.h"
#include "math.h"

//////////////////////////////////////////////////////////////////////////

//时间标识
#define IDI_REQUEST_BANKER			100									//申请定时
#define IDI_GIVEUP_BANKER			101									//下庄定时
#define IDI_BANK_OPERATE			102									//银行定时
#define IDI_CHECK_BANKER			103									//检查上庄
#define IDI_PLACE_JETTON			110									//下注定义 (预留110-160)


int CAndroidUserItemSink::m_stlApplyBanker = 0L;
int CAndroidUserItemSink::m_stnApplyCount = 0L;

//////////////////////////////////////////////////////////////////////////

//构造函数
CAndroidUserItemSink::CAndroidUserItemSink()
{
	//游戏变量
	m_nRobotBetDelay = 0;
	m_bHaveConfig = false;
	m_lMaxChipBanker = 0;
	m_lMaxChipUser = 0;
	m_wCurrentBanker = 0;
	m_nChipTime = 0;
	m_nChipTimeCount = 0;
	m_lBankerLostScore = 0;
	m_lBankerDrawCount = 0;
	m_lBankerWinCount = 0;
	m_lBankerLoseCount = 0;
	ZeroMemory(m_lAreaChip, sizeof(m_lAreaChip));
	ZeroMemory(m_nChipLimit, sizeof(m_nChipLimit));
	ZeroMemory(m_bWinFlag, sizeof(m_bWinFlag));

	//上庄变量
	m_nBankerCount=0;
	m_nCurMaxBankerCount=0;
	m_nRobotBetTimeLimit[0]=3;
	m_nRobotBetTimeLimit[1]=8;
	m_lRobotJettonLimit[0]=1000;
	m_lRobotJettonLimit[1]=1000000;
	//上庄变量
	m_bMeApplyBanker=false;
	m_nWaitBanker=0;	
	m_nListUserCount=0;

	srand(GetTickCount());

	return;
}

//析构函数
CAndroidUserItemSink::~CAndroidUserItemSink()
{
}

//接口查询
VOID *  CAndroidUserItemSink::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IAndroidUserItemSink,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IAndroidUserItemSink,Guid,dwQueryVer);
	return NULL;
}

//初始接口
bool  CAndroidUserItemSink::Initialization(IUnknownEx * pIUnknownEx)
{
	//查询接口
	ASSERT(QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,IAndroidUserItem)!=NULL);
	m_pIAndroidUserItem=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,IAndroidUserItem);

	//错误判断
	if (m_pIAndroidUserItem==NULL)
	{
		ASSERT(FALSE);
		return false;
	}

	return true;
}

//重置接口
bool  CAndroidUserItemSink::RepositionSink()
{
	//游戏变量
	m_lMaxChipBanker = 0;
	m_lMaxChipUser = 0;
	m_wCurrentBanker = 0;
	m_nChipTime = 0;
	m_nChipTimeCount = 0;
	m_lBankerLostScore = 0;
	ZeroMemory(m_lAreaChip, sizeof(m_lAreaChip));
	ZeroMemory(m_nChipLimit, sizeof(m_nChipLimit));

	//上庄变量
	m_nBankerCount=0;
	m_nCurMaxBankerCount=0;

	//上庄变量
	m_bMeApplyBanker=false;
	m_nWaitBanker=0;

	return true;
}

//时间消息
bool  CAndroidUserItemSink::OnEventTimer(UINT nTimerID)
{
	switch (nTimerID)
	{
	case IDI_CHECK_BANKER:		//检查上庄
		{
			m_pIAndroidUserItem->KillGameTimer(nTimerID);

			if (m_wCurrentBanker == m_pIAndroidUserItem->GetChairID())
				return false;

			int nMinCount = m_nRobotListMaxCount;
			if ( m_nRobotListMaxCount > m_nRobotListMinCount)
				nMinCount = (rand()%(m_nRobotListMaxCount - m_nRobotListMinCount+1)) + m_nRobotListMinCount;

			//空庄
			m_nWaitBanker++;

			//机器人上庄
			if ( m_bRobotBanker 
				&& !m_bMeApplyBanker 
				&& m_nWaitBanker >= m_nRobotWaitBanker 
				&& m_nListUserCount < m_nRobotListMaxCount 
				&& m_stlApplyBanker < m_nRobotListMaxCount 
				&& m_stlApplyBanker < nMinCount 
				/*&& m_stnApplyCount < 2*/ )
			{
				m_nWaitBanker = 0;

				//合法判断
				IServerUserItem *pIUserItemBanker = m_pIAndroidUserItem->GetMeUserItem();
				if (pIUserItemBanker->GetUserScore() > m_lBankerCondition ) 
				{
					//机器人上庄
					m_nBankerCount = 0;
					m_stlApplyBanker++;
					//m_stnApplyCount++;
					m_pIAndroidUserItem->SetGameTimer(IDI_REQUEST_BANKER, (rand() % 3) + 2);
				}
				else
				{
					//执行取款
					SCORE lDiffScore = (m_lRobotBankGetScoreBankerMax-m_lRobotBankGetScoreBankerMin)/100;
					SCORE lScore = m_lRobotBankGetScoreBankerMin + (m_pIAndroidUserItem->GetUserID()%10)*(rand()%10)*lDiffScore + 
						(rand()%10+1)*(rand()%(lDiffScore/10));

					if (lScore > 0)
					{
						m_pIAndroidUserItem->PerformTakeScore(lScore);
					}

					IServerUserItem *pIUserItemBanker = m_pIAndroidUserItem->GetMeUserItem();
					if (pIUserItemBanker->GetUserScore() > m_lBankerCondition) 
					{
						//机器人上庄
						m_nBankerCount = 0;
						m_stlApplyBanker++;
						m_pIAndroidUserItem->SetGameTimer(IDI_REQUEST_BANKER, (rand() % 3) + 2);
					}
				}
			}

			return false;
		}
	case IDI_REQUEST_BANKER:	//申请上庄
		{
			m_pIAndroidUserItem->KillGameTimer(nTimerID);

			m_pIAndroidUserItem->SendSocketData(SUB_C_APPLY_BANKER);

			return false;
		}
	case IDI_GIVEUP_BANKER:		//申请下庄
		{
			m_pIAndroidUserItem->KillGameTimer(nTimerID);

			m_pIAndroidUserItem->SendSocketData(SUB_C_CANCEL_BANKER);

			m_nCurMaxBankerCount = 0;

			return false;
		}
	case IDI_BANK_OPERATE:		//银行操作
		{
			m_pIAndroidUserItem->KillGameTimer(nTimerID);

			//变量定义
			IServerUserItem *pUserItem = m_pIAndroidUserItem->GetMeUserItem();
			LONGLONG lRobotScore = pUserItem->GetUserScore();			
			{

				//判断存取
				if (lRobotScore > m_lRobotScoreRange[1])
				{
					LONGLONG lSaveScore=0L;

					lSaveScore = LONGLONG(lRobotScore*m_nRobotBankStorageMul/100);
					if (lSaveScore > lRobotScore)  lSaveScore = lRobotScore;

					if (lSaveScore > 0 && m_wCurrentBanker != m_pIAndroidUserItem->GetChairID())
						m_pIAndroidUserItem->PerformSaveScore(lSaveScore);

				}
				else if (lRobotScore < m_lRobotScoreRange[0])
				{
					SCORE lDiffScore = (m_lRobotBankGetScoreMax-m_lRobotBankGetScoreMin)/100;
					SCORE lScore = m_lRobotBankGetScoreMin + (m_pIAndroidUserItem->GetUserID()%10)*(rand()%10)*lDiffScore + 
						(rand()%10+1)*(rand()%(lDiffScore/10));

					if (lScore > 0)
						m_pIAndroidUserItem->PerformTakeScore(lScore);
				}
			}
			return false;

			
		}
	default:
		{
			if (nTimerID >= IDI_PLACE_JETTON && nTimerID <= IDI_PLACE_JETTON+MAX_CHIP_TIME)
			{
				

				//变量定义
				int nRandNum = 0, nChipArea = 0, nCurChip = 0, nACTotal = 0, nCurJetLmt[2] = {};
				SCORE lMaxChipLmt = __min(m_lMaxChipBanker, m_lMaxChipUser);			//最大可下注值
				WORD wMyID = m_pIAndroidUserItem->GetChairID();
				int nAreaChance[AREA_COUNT] = {};

				//下注偏移
				for (int i = 0; i < AREA_COUNT; i++)
					nAreaChance[i] = int((8+m_bWinFlag[i]*2)*m_RobotInfo.nAreaChance[i]);
				for (int i = 0; i < AREA_COUNT; i++)
					nACTotal += nAreaChance[i];

				//MyDebug(_T("AreaChance [%d %d %d %d %d %d] [%d %d %d %d %d %d]"), m_bWinFlag[0], m_bWinFlag[1], m_bWinFlag[2], m_bWinFlag[3], m_bWinFlag[4], m_bWinFlag[5],
				//	nAreaChance[0], nAreaChance[1], nAreaChance[2], nAreaChance[3], nAreaChance[4], nAreaChance[5]);

				//统计次数
				m_nChipTimeCount++;

				//检测退出
				if (lMaxChipLmt < m_RobotInfo.nChip[m_nChipLimit[0]])	return false;
				for (int i = 0; i < AREA_COUNT; i++)
				{
					if (m_lAreaChip[i] >= m_RobotInfo.nChip[m_nChipLimit[0]])	break;
					if (i == AREA_COUNT-1)	return false;
				}

				//下注区域
				ASSERT(nACTotal>0);
				static int nStFluc = 1;				//随机辅助
				if (nACTotal <= 0)	return false;
				do {
					int nTemp = rand();
					nRandNum = UINT(nTemp*nTemp*(wMyID+1)*nStFluc*3) % nACTotal;
					//MyDebug(_T("Rand [%d] [%d / %d]"), nTemp, nRandNum, nACTotal);
					for (int i = 0; i < AREA_COUNT; i++)
					{
						nRandNum -= nAreaChance[i];
						if (nRandNum < 0)
						{
							nChipArea = i;
							break;
						}
					}
				}
				while (m_lAreaChip[nChipArea] < m_RobotInfo.nChip[m_nChipLimit[0]]);
				nStFluc = nStFluc%3 + 1;

				//下注大小
				if (m_nChipLimit[0] == m_nChipLimit[1])
					nCurChip = m_nChipLimit[0];
				else
				{
					//设置变量
					lMaxChipLmt = __min(lMaxChipLmt, m_lAreaChip[nChipArea]);
					nCurJetLmt[0] = m_nChipLimit[0];
					nCurJetLmt[1] = m_nChipLimit[0];

					//计算当前最大筹码
					for (int i = m_nChipLimit[1]; i > m_nChipLimit[0]; i--)
					{
						if (lMaxChipLmt > m_RobotInfo.nChip[i]) 
						{
							nCurJetLmt[1] = i;
							break;
						}
					}

					//随机下注
					nRandNum = (rand()+wMyID) % (nCurJetLmt[1]-nCurJetLmt[0]+1);
					nCurChip = nCurJetLmt[0] + nRandNum;

					//多下控制 (当庄家金币较少时会尽量保证下足次数)
					if (m_nChipTimeCount < m_nChipTime)
					{
						SCORE lLeftJetton = SCORE( (lMaxChipLmt-m_RobotInfo.nChip[nCurChip])/(m_nChipTime-m_nChipTimeCount) );

						//不够次数 (即全用最小限制筹码下注也少了)
						if (lLeftJetton < m_RobotInfo.nChip[m_nChipLimit[0]] && nCurChip > m_nChipLimit[0])
							nCurChip--;
					}
				}

				/*ASSERT( MyDebug(_T("机器人下注 %d 下注次数 [%d/%d] 下注 [%d %d] 范围 [%d %d] 限制 [%I64d %I64d %I64d]"), wMyID, nTimerID-IDI_PLACE_JETTON, m_nChipTime, 
					nChipArea, m_RobotInfo.nChip[nCurChip], m_nChipLimit[0], m_nChipLimit[1], m_lMaxChipBanker, m_lMaxChipUser, lMaxChipLmt) );*/

				//变量定义
				CMD_C_PlaceJetton PlaceJetton = {};

				//构造变量
				PlaceJetton.cbJettonArea = nChipArea;		
				PlaceJetton.lJettonScore = m_RobotInfo.nChip[nCurChip];

				//发送消息
				m_pIAndroidUserItem->SendSocketData(SUB_C_PLACE_JETTON, &PlaceJetton, sizeof(PlaceJetton));
			}

			m_pIAndroidUserItem->KillGameTimer(nTimerID);
			return false;
		}
	}
	return false;
}

//游戏消息
bool  CAndroidUserItemSink::OnEventGameMessage(WORD wSubCmdID, VOID * pBuffer, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_S_GAME_FREE:			//游戏空闲 
		{
			return OnSubGameFree(pBuffer, wDataSize);
		}
	case SUB_S_GAME_START:			//游戏开始
		{
			return OnSubGameStart(pBuffer, wDataSize);
		}
	case SUB_S_PLACE_JETTON:		//用户加注
		{
			return OnSubPlaceJetton(pBuffer, wDataSize);
		}
	case SUB_S_APPLY_BANKER:		//申请做庄 
		{
			return OnSubUserApplyBanker(pBuffer,wDataSize);
		}
	case SUB_S_CANCEL_BANKER:		//取消做庄 
		{
			return OnSubUserCancelBanker(pBuffer,wDataSize);
		}
	case SUB_S_CHANGE_BANKER:		//切换庄家 
		{
			return OnSubChangeBanker(pBuffer,wDataSize);
		}
	case SUB_S_GAME_END:			//游戏结束 
		{
			return OnSubGameEnd(pBuffer, wDataSize);
		}
	case SUB_S_SEND_RECORD:			//游戏记录 (忽略)
		{
			return true;
		}
	case SUB_S_PLACE_JETTON_FAIL:	//下注失败 (忽略)
		{
			return true;
		}
	case SUB_S_ROBOT_BANKER:		//上庄通知
		{
			return OnSubRobotBanker(pBuffer, wDataSize);
		}
	case SUB_S_ROBOT_CONFIG:		//机器配置
		{
			return OnSubRobotConfig(pBuffer, wDataSize);
		}
	}

	//错误断言
	//ASSERT(FALSE);

	return true;
}

//游戏消息
bool  CAndroidUserItemSink::OnEventFrameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	return true;
}

//场景消息
bool  CAndroidUserItemSink::OnEventSceneMessage(BYTE cbGameStatus, bool bLookonOther, VOID * pData, WORD wDataSize)
{
	switch (cbGameStatus)
	{
	case GAME_STATUS_FREE:			//空闲状态
		{
			//效验数据
			ASSERT(wDataSize==sizeof(CMD_S_StatusFree));
			if (wDataSize!=sizeof(CMD_S_StatusFree)) return false;

			//消息处理
			CMD_S_StatusFree * pStatusFree=(CMD_S_StatusFree *)pData;

			m_lUserLimitScore = pStatusFree->lUserMaxScore;
			m_lAreaLimitScore = pStatusFree->lAreaLimitScore;
			m_lBankerCondition = pStatusFree->lApplyBankerCondition;

			memcpy(m_szRoomName, pStatusFree->szGameRoomName, sizeof(m_szRoomName));
			
			ReadConfigInformation(&pStatusFree->CustomAndroid);

			//上庄处理
			if (pStatusFree->wBankerUser == INVALID_CHAIR)
			{
				if (m_bRobotBanker && m_nRobotWaitBanker == 0  && m_stlApplyBanker < m_nRobotApplyBanker)
				{
					//合法判断
					IServerUserItem *pIUserItemBanker = m_pIAndroidUserItem->GetMeUserItem();
					if (pIUserItemBanker->GetUserScore() > m_lBankerCondition) 
					{
						//机器人上庄
						m_nBankerCount = 0;
						m_stlApplyBanker++;

						BYTE cbTime = (pStatusFree->cbTimeLeave>0?(rand()%pStatusFree->cbTimeLeave+1):2);
						if (cbTime == 0) cbTime = 2;

						m_pIAndroidUserItem->SetGameTimer(IDI_REQUEST_BANKER, cbTime);
					}
				}
			}

			return true;
		}
	case GS_GAME_START:		//游戏状态
	case GS_PLAYER_BET:		//下注状态
	case GS_GAME_END:		//结束状态
	case GS_MOVECARD_END:	//结束状态
		{
			//效验数据
			ASSERT(wDataSize==sizeof(CMD_S_StatusPlay));
			if (wDataSize!=sizeof(CMD_S_StatusPlay)) return false;

			//消息处理
			CMD_S_StatusPlay * pStatusPlay=(CMD_S_StatusPlay *)pData;

			//庄家信息
			m_wCurrentBanker = pStatusPlay->wBankerUser;

			m_lUserLimitScore = pStatusPlay->lUserMaxScore;
			m_lAreaLimitScore = pStatusPlay->lAreaLimitScore;
			m_lBankerCondition = pStatusPlay->lApplyBankerCondition;

			memcpy(m_szRoomName, pStatusPlay->szGameRoomName, sizeof(m_szRoomName));

			ReadConfigInformation(&pStatusPlay->CustomAndroid);

			return true;
		}
	}

	return true;
}

//用户进入
VOID  CAndroidUserItemSink::OnEventUserEnter(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

//用户离开
VOID  CAndroidUserItemSink::OnEventUserLeave(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

//用户积分
VOID  CAndroidUserItemSink::OnEventUserScore(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

//用户状态
VOID  CAndroidUserItemSink::OnEventUserStatus(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}


//游戏空闲
bool CAndroidUserItemSink::OnSubGameFree(const VOID * pBuffer, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_GameFree));
	if (wDataSize!=sizeof(CMD_S_GameFree)) return false;

	//消息处理
	CMD_S_GameFree * pGameFree=(CMD_S_GameFree *)pBuffer;

	m_nListUserCount = pGameFree->nListUserCount;

	//银行操作
	if (pGameFree->cbTimeLeave > 1)
		m_pIAndroidUserItem->SetGameTimer(IDI_BANK_OPERATE, (rand() % (pGameFree->cbTimeLeave-1)) + 1);

	bool bMeGiveUp = false;
	if (m_wCurrentBanker == m_pIAndroidUserItem->GetChairID())
	{
		m_nBankerCount++;
		if ( m_nBankerCount >= m_nRobotBankerCount )
		{
			//机器人走庄
			m_nBankerCount = 0;
			m_pIAndroidUserItem->SetGameTimer(IDI_GIVEUP_BANKER, rand()%2 + 1);

			bMeGiveUp = true;
		}
	}

	if (m_wCurrentBanker != m_pIAndroidUserItem->GetChairID() || bMeGiveUp)
		m_pIAndroidUserItem->SetGameTimer(IDI_CHECK_BANKER, pGameFree->cbTimeLeave - 3);

	return true;
}

//游戏开始
bool CAndroidUserItemSink::OnSubGameStart(const VOID * pBuffer, WORD wDataSize)
{	
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_GameStart));
	if (wDataSize!=sizeof(CMD_S_GameStart)) return false;

	//消息处理
	CMD_S_GameStart * pGameStart=(CMD_S_GameStart *)pBuffer;

	m_wCurrentBanker = pGameStart->wBankerUser;
	memcpy(m_bWinFlag, pGameStart->bWinFlag, sizeof(m_bWinFlag));

	if (m_wCurrentBanker != m_pIAndroidUserItem->GetChairID())
	{
		m_nBankerCount = 0;
		m_lBankerDrawCount = 0;
		m_lBankerWinCount = 0;
		m_lBankerLoseCount = 0;
	}

	//自己当庄或无下注机器人
	if (pGameStart->wBankerUser == m_pIAndroidUserItem->GetChairID() || pGameStart->nChipRobotCount <= 0)
		return true;

	//设置变量
	m_lMaxChipBanker = pGameStart->lBankerScore/m_RobotInfo.nMaxTime;
	m_lMaxChipUser = __min(pGameStart->lUserMaxScore/m_RobotInfo.nMaxTime, m_lUserLimitScore);
	m_nChipTimeCount = 0;
	ZeroMemory(m_nChipLimit, sizeof(m_nChipLimit));
	for (int i = 0; i < AREA_COUNT; i++)
		m_lAreaChip[i] = m_lAreaLimitScore;

	//系统当庄
	if (pGameStart->wBankerUser == INVALID_CHAIR)
		m_lMaxChipBanker = 2147483647/m_RobotInfo.nMaxTime;
	else
		m_lMaxChipUser = __min( m_lMaxChipUser, m_lMaxChipBanker );

	if(pGameStart->nAndriodApplyCount > 0)
		m_stlApplyBanker=pGameStart->nAndriodApplyCount;
	else
		m_stlApplyBanker=0;

	//计算下注次数
	int nElapse = 0;												
	WORD wMyID = m_pIAndroidUserItem->GetChairID();

	if (m_nRobotBetTimeLimit[0] == m_nRobotBetTimeLimit[1])
		m_nChipTime = m_nRobotBetTimeLimit[0];
	else
		m_nChipTime = (rand()+wMyID)%(m_nRobotBetTimeLimit[1]-m_nRobotBetTimeLimit[0]+1) + m_nRobotBetTimeLimit[0];
	ASSERT(m_nChipTime>=0);		
	if (m_nChipTime <= 0)	return true;								//的确,2个都带等于
	if (m_nChipTime > MAX_CHIP_TIME)	m_nChipTime = MAX_CHIP_TIME;	//限定MAX_CHIP次下注

	//计算范围
	if (!CalcJettonRange(__min(m_lMaxChipBanker, m_lMaxChipUser), m_lRobotJettonLimit, m_nChipTime, m_nChipLimit))
		return true;

	//设置时间
	int nTimeGrid = int(pGameStart->cbTimeLeave-m_nRobotBetDelay)*800/m_nChipTime;		//时间格,前1秒不下注,所以-2,800表示机器人下注时间范围千分比
	for (int i = 0; i < m_nChipTime; i++)
	{
		int nRandRage = int( nTimeGrid * i / (1500*sqrt((double)m_nChipTime)) ) + 1;		//波动范围
		nElapse = m_nRobotBetDelay + (nTimeGrid*i)/1000 + ( (rand()+wMyID)%(nRandRage*2) - (nRandRage-1) );
		ASSERT(nElapse>=m_nRobotBetDelay&&nElapse<=pGameStart->cbTimeLeave);
		if (nElapse < m_nRobotBetDelay || nElapse > pGameStart->cbTimeLeave)	continue;
		
		m_pIAndroidUserItem->SetGameTimer(IDI_PLACE_JETTON+i+1, nElapse);
	}

	//ASSERT( MyDebug(_T("机器人 %d 下注次数 %d 范围 [%d %d] 总人数 %d 限制 [%I64d %I64d] 坐庄 [%d %d]"), wMyID, m_nChipTime, m_nChipLimit[0], m_nChipLimit[1], 
	//	pGameStart->nChipRobotCount, m_lMaxChipBanker, m_lMaxChipUser, m_nBankerCount, m_nCurMaxBankerCount) );

	return true;
}

//用户加注
bool CAndroidUserItemSink::OnSubPlaceJetton(const VOID * pBuffer, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_PlaceJetton));
	if (wDataSize!=sizeof(CMD_S_PlaceJetton)) return false;

	//消息处理
	CMD_S_PlaceJetton * pPlaceJetton=(CMD_S_PlaceJetton *)pBuffer;

	//设置变量
	m_lMaxChipBanker -= pPlaceJetton->lJettonScore;
	m_lAreaChip[pPlaceJetton->cbJettonArea] -= pPlaceJetton->lJettonScore;
	if (pPlaceJetton->wChairID == m_pIAndroidUserItem->GetChairID())
		m_lMaxChipUser -= pPlaceJetton->lJettonScore;

	return true;
}

//下注失败
bool CAndroidUserItemSink::OnSubPlaceJettonFail(const VOID * pBuffer, WORD wDataSize)
{
	return true;
}

//游戏结束
bool CAndroidUserItemSink::OnSubGameEnd(const VOID * pBuffer, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_GameEnd));
	if (wDataSize!=sizeof(CMD_S_GameEnd)) return false;

	//消息处理
	CMD_S_GameEnd * pGameEnd=(CMD_S_GameEnd *)pBuffer;

	//if ( m_wCurrentBanker == m_pIAndroidUserItem->GetChairID() )
	//{
	//	m_nBankerCount++;
	//	if (pGameEnd->lBankerScore > 0)
	//		m_lBankerWinCount++;
	//	else if (pGameEnd->lBankerScore < 0)
	//		m_lBankerLoseCount++;
	//	//MyDebug(_T("机器人当庄 [%d / %d] W [%I64d / %I64d] L [%I64d / %I64d]"), m_nBankerCount, m_nCurMaxBankerCount,
	//	//	m_lBankerWinCount,m_lRobotGiveUpWin,m_lBankerLoseCount,m_lRobotGiveUpLose);
	//	if ( m_nBankerCount >= m_nCurMaxBankerCount || m_lBankerWinCount >= m_lRobotGiveUpWin || 
	//		 m_lBankerLoseCount >= m_lRobotGiveUpLose)
	//	{
	//		//机器人走庄
	//		m_nBankerCount = 0;
	//		m_lBankerDrawCount = 0;
	//		m_lBankerWinCount = 0;
	//		m_lBankerLoseCount = 0;
	//		m_pIAndroidUserItem->SetGameTimer(IDI_GIVEUP_BANKER, pGameEnd->cbTimeLeave + 1);
	//	}
	//	else
	//	{
	//		m_lBankerLostScore = pGameEnd->lBankerScore;

	//		BYTE bTime = (pGameEnd->cbTimeLeave>4?pGameEnd->cbTimeLeave-3:1);
	//		m_pIAndroidUserItem->SetGameTimer(IDI_BANK_OPERATE, bTime);

	//		IServerUserItem *pUserItem = m_pIAndroidUserItem->GetMeUserItem();
	//		SCORE lRobotScore = pUserItem->GetUserScore();
	//		//MyDebug(_T("庄家取钱 [%I64d + %I64d = %I64d]"), lRobotScore, m_lBankerLostScore, m_lBankerLostScore+lRobotScore);
	//	}
	//}

	return true;
}

//申请做庄
bool CAndroidUserItemSink::OnSubUserApplyBanker(const VOID * pBuffer, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_ApplyBanker));
	if (wDataSize!=sizeof(CMD_S_ApplyBanker)) return false;

	//消息处理
	CMD_S_ApplyBanker * pApplyBanker=(CMD_S_ApplyBanker *)pBuffer;

	//自己判断
	if (m_pIAndroidUserItem->GetChairID()==pApplyBanker->wApplyUser) 
		m_bMeApplyBanker=true;

	return true;
}

//取消做庄
bool CAndroidUserItemSink::OnSubUserCancelBanker(const VOID * pBuffer, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_CancelBanker));
	if (wDataSize!=sizeof(CMD_S_CancelBanker)) return false;

	//消息处理
	CMD_S_CancelBanker * pCancelBanker=(CMD_S_CancelBanker *)pBuffer;

	//自己判断
	if (m_pIAndroidUserItem->GetChairID()==pCancelBanker->wCancelUser) 
		m_bMeApplyBanker=false;

	return true;
}

//切换庄家
bool CAndroidUserItemSink::OnSubChangeBanker(const VOID * pBuffer, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_ChangeBanker));
	if (wDataSize!=sizeof(CMD_S_ChangeBanker)) return false;

	//消息处理
	CMD_S_ChangeBanker * pChangeBanker = (CMD_S_ChangeBanker *)pBuffer;

	if ( m_wCurrentBanker == m_pIAndroidUserItem->GetChairID() && m_wCurrentBanker != pChangeBanker->wBankerUser )
	{		
		m_nWaitBanker = 0;
		m_bMeApplyBanker = false;
	}

	m_wCurrentBanker = pChangeBanker->wBankerUser;

	return true;
}

//上庄通知
bool CAndroidUserItemSink::OnSubRobotBanker(const VOID * pBuffer, WORD wDataSize)
{
	if (!m_bRobotBanker)
		return true;

	//检查条件
	IServerUserItem *pUserItem = m_pIAndroidUserItem->GetMeUserItem();
	SCORE lRobotScore = pUserItem->GetUserScore();
	if (m_pIAndroidUserItem->GetChairID() != m_wCurrentBanker && lRobotScore >= m_lBankerCondition && lRobotScore >= m_lRobotBankerCondition)
	{
		m_pIAndroidUserItem->SetGameTimer(IDI_REQUEST_BANKER, (rand() % 5) + 1);
	}

	return true;
}

//配置通知
bool CAndroidUserItemSink::OnSubRobotConfig(const VOID * pBuffer, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_RobotConfig));
	if (wDataSize!=sizeof(CMD_S_RobotConfig)) return false;

	//消息处理
	CMD_S_RobotConfig * pRobotConfig = (CMD_S_RobotConfig *)pBuffer;

	m_lRobotBankerCondition		= pRobotConfig->robotConfig.lBankerCondition;
	m_lRobotBankSaveCondition	= pRobotConfig->robotConfig.lBankSaveCondition;
	m_lRobotBankDrawCondition	= pRobotConfig->robotConfig.lBankDrawCondition;
	m_lRobotBankSaveScore		= pRobotConfig->robotConfig.lBankSaveScore;
	m_lRobotBankDrawScore		= pRobotConfig->robotConfig.lBankDrawScore;
	m_lRobotJettonLimit[0]		= pRobotConfig->robotConfig.lJettonLimit[0];
	m_lRobotJettonLimit[1]		= pRobotConfig->robotConfig.lJettonLimit[1];
	m_nRobotBetTimeLimit[0]		= (int)pRobotConfig->robotConfig.lBetTimeLimit;
	m_nRobotBetTimeLimit[1]		= (int)pRobotConfig->robotConfig.lBetTimeLimit;
	m_lRobotBankDrawCount		= pRobotConfig->robotConfig.lBankerDrawCount;
	m_lRobotGiveUpWin			= pRobotConfig->robotConfig.lBankerWinGiveUp;
	m_lRobotGiveUpLose			= pRobotConfig->robotConfig.lBankerLoseGiveUp;

	m_bHaveConfig = true;

	//MyDebug(_T("机器人配置 %I64d %I64d %I64d %I64d %I64d -- %I64d %I64d %d %d --  %I64d %I64d %I64d"), m_lRobotBankerCondition, 
	//	m_lRobotBankSaveCondition, m_lRobotBankDrawCondition, m_lRobotBankSaveScore, m_lRobotBankDrawScore,
	//	m_lRobotJettonLimit[0], m_lRobotJettonLimit[1], m_nRobotBetTimeLimit[0], m_nRobotBetTimeLimit[1],
	//	m_lRobotBankDrawCount, m_lRobotGiveUpWin, m_lRobotGiveUpLose);

	return true;
}

//读取配置
VOID CAndroidUserItemSink::ReadConfigInformation(tagCustomAndroid *pCustomAndroid)
{
	//次数限制
	m_nRobotBetTimeLimit[0] = pCustomAndroid->lRobotMinBetTime;
	m_nRobotBetTimeLimit[1] = pCustomAndroid->lRobotMaxBetTime;
	if (m_nRobotBetTimeLimit[0] < 0) m_nRobotBetTimeLimit[0] = 0;
	if (m_nRobotBetTimeLimit[1] < m_nRobotBetTimeLimit[0]) m_nRobotBetTimeLimit[1] = m_nRobotBetTimeLimit[0];

	//筹码限制
	m_lRobotJettonLimit[0] = pCustomAndroid->lRobotMinJetton;
	m_lRobotJettonLimit[1] = pCustomAndroid->lRobotMaxJetton;
	if (m_lRobotJettonLimit[1] > 5000000)					m_lRobotJettonLimit[1] = 5000000;
	if (m_lRobotJettonLimit[0] < 100)						m_lRobotJettonLimit[0] = 100;
	if (m_lRobotJettonLimit[1] < m_lRobotJettonLimit[0])	m_lRobotJettonLimit[1] = m_lRobotJettonLimit[0];

	//是否坐庄
	m_bRobotBanker = (pCustomAndroid->nEnableRobotBanker == TRUE)?true:false;

	//坐庄次数
	LONGLONG lRobotBankerCountMin = pCustomAndroid->lRobotBankerCountMin;
	LONGLONG lRobotBankerCountMax = pCustomAndroid->lRobotBankerCountMax;
	m_nRobotBankerCount = rand()%(lRobotBankerCountMax-lRobotBankerCountMin+1) + lRobotBankerCountMin;

	//列表人数
	m_nRobotListMinCount = pCustomAndroid->lRobotListMinCount;
	m_nRobotListMaxCount = pCustomAndroid->lRobotListMaxCount;

	//最多个数
	m_nRobotApplyBanker = pCustomAndroid->lRobotApplyBanker;

	//空盘重申
	m_nRobotWaitBanker = pCustomAndroid->lRobotWaitBanker;

	//分数限制
	m_lRobotScoreRange[0] = pCustomAndroid->lRobotScoreMin;
	m_lRobotScoreRange[1] = pCustomAndroid->lRobotScoreMax;
	if (m_lRobotScoreRange[1] < m_lRobotScoreRange[0])	m_lRobotScoreRange[1] = m_lRobotScoreRange[0];

	//提款数额
	m_lRobotBankGetScoreMin = pCustomAndroid->lRobotBankGetMin;
	m_lRobotBankGetScoreMax = pCustomAndroid->lRobotBankGetMax;

	//提款数额 (庄家)
	m_lRobotBankGetScoreBankerMin = pCustomAndroid->lRobotBankGetBankerMin;
	m_lRobotBankGetScoreBankerMax = pCustomAndroid->lRobotBankGetBankerMax;

	//存款倍数
	m_nRobotBankStorageMul = pCustomAndroid->lRobotBankStoMul;
	if (m_nRobotBankStorageMul<0 || m_nRobotBankStorageMul>100) m_nRobotBankStorageMul = 20;
}

//计算范围	(返回值表示是否可以通过下降下限达到下注)
bool CAndroidUserItemSink::CalcJettonRange(SCORE lMaxScore, SCORE lChipLmt[], int & nChipTime, int lJetLmt[])
{
	//定义变量
	bool bHaveSetMinChip = false;

	//不够一注
	if (lMaxScore < m_RobotInfo.nChip[0])	return false;

	//配置范围
	for (int i = 0; i < CountArray(m_RobotInfo.nChip); i++)
	{
		if (!bHaveSetMinChip && m_RobotInfo.nChip[i] >= lChipLmt[0])
		{ 
			lJetLmt[0] = i;
			bHaveSetMinChip = true;
		}
		if (m_RobotInfo.nChip[i] <= lChipLmt[1])
			lJetLmt[1] = i;
	}
	if (lJetLmt[0] > lJetLmt[1])	lJetLmt[0] = lJetLmt[1];

	//是否降低下限
	//if (nChipTime * m_RobotInfo.nChip[lJetLmt[0]] > lMaxScore)
	//{
	//	//是否降低下注次数
	//	if (nChipTime * m_RobotInfo.nChip[0] > lMaxScore)
	//	{
	//		nChipTime = int(lMaxScore/m_RobotInfo.nChip[0]);
	//		lJetLmt[0] = 0;
	//		lJetLmt[1] = 0;
	//	}
	//	else
	//	{
	//		//降低到合适下限
	//		while (nChipTime * m_RobotInfo.nChip[lJetLmt[0]] > lMaxScore)
	//		{
	//			lJetLmt[0]--;
	//			ASSERT(lJetLmt[0]>=0);
	//		}
	//	}
	//}

	return true;
}

//读取长整
SCORE CAndroidUserItemSink::GetPrivateProfileInt64(LPCTSTR lpAppName, LPCTSTR lpKeyName, SCORE lDefault, LPCTSTR lpFileName)
{
	//变量定义
	TCHAR OutBuf[255] = _T("");
	TCHAR DefaultBuf[255] = {};
	SCORE lNumber = 0;

	GetPrivateProfileString(lpAppName, lpKeyName, DefaultBuf, OutBuf, 255, lpFileName);

	if (OutBuf[0] != 0)
		myscanf(OutBuf, mystrlen(OutBuf), _T("%I64d"), &lNumber);
	else
		lNumber = lDefault;

	return lNumber;
}

//组件创建函数
DECLARE_CREATE_MODULE(AndroidUserItemSink);

//////////////////////////////////////////////////////////////////////////
