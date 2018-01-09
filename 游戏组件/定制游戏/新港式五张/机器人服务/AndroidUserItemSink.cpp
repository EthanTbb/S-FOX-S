#include "Stdafx.h"
#include "AndroidUserItemSink.h"
#include <math.h>

//////////////////////////////////////////////////////////////////////////

//游戏时间
#define TIME_USER_START_GAME		3									//开始定时器
#define TIME_USER_ADD_SCORE			1									//下注定时器

//游戏时间
#define IDI_START_GAME				(103)								//开始时间
#define IDI_USER_ADD_SCORE			(104)								//下注时间
#define IDI_BANK_OPERATION			(105)								//银行操作

//////////////////////////////////////////////////////////////////////////

//构造函数
CAndroidUserItemSink::CAndroidUserItemSink()
{
	//接口变量
	m_pIAndroidUserItem=NULL; 
	
	return;
}

//析构函数
CAndroidUserItemSink::~CAndroidUserItemSink()
{
}

//接口查询
void * CAndroidUserItemSink::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IAndroidUserItemSink,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IAndroidUserItemSink,Guid,dwQueryVer);
	return NULL;
}

//初始接口
bool CAndroidUserItemSink::Initialization(IUnknownEx * pIUnknownEx)
{
	//查询接口
	m_pIAndroidUserItem=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,IAndroidUserItem);
	if (m_pIAndroidUserItem==NULL) return false;

	return true;
}

//重置接口
bool CAndroidUserItemSink::RepositionSink()
{


	return true;
}

//时间消息
bool CAndroidUserItemSink::OnEventTimer(UINT nTimerID)
{
	switch (nTimerID)
	{
	case IDI_BANK_OPERATION:	//操作银行
		{
			// 取钱
			if( m_pIAndroidUserItem->GetMeUserItem()->GetUserScore() < m_lRobotScoreMin )
			{
				int nRandCount = (int)(m_lRobotBankTake[1] - m_lRobotBankTake[0]) / RAND_MAX;
				if( nRandCount == 0 ) nRandCount = 1;

				m_pIAndroidUserItem->PerformTakeScore( ((rand() * nRandCount) % (m_lRobotBankTake[1] - m_lRobotBankTake[0])) + m_lRobotBankTake[0] );
			}
			// 存钱
			else if	( m_pIAndroidUserItem->GetMeUserItem()->GetUserScore() > m_lRobotScoreMax )
			{
				m_pIAndroidUserItem->PerformSaveScore( m_pIAndroidUserItem->GetMeUserItem()->GetUserScore() * m_lRobotBankSave / 100 );
			}

			// 准备
			m_pIAndroidUserItem->SetGameTimer(IDI_START_GAME, (rand()%3) + 2);

			return true;
		}
	case IDI_START_GAME:		//开始定时器
		{			
			// 发送准备消息
			m_pIAndroidUserItem->SendUserReady( NULL, 0 );

			return true;
		}	
	case IDI_USER_ADD_SCORE:	//加注定时
		{
			// 本机器人位置
			WORD wChairID = m_pIAndroidUserItem->GetChairID();

			// 玩家数量
			WORD wPlayCount = 0;
			for( int nPlayIndex = 0; nPlayIndex < GAME_PLAYER; ++nPlayIndex )
			{
				if( m_cbPlayStatus[nPlayIndex] )
					wPlayCount++;
			}

			// 排名
			WORD wPlayMingRank[GAME_PLAYER];
			WORD wPlayShouRank[GAME_PLAYER];
			WORD wPlayFullRank[GAME_PLAYER];
			ZeroMemory(wPlayMingRank, sizeof(wPlayMingRank));
			ZeroMemory(wPlayShouRank, sizeof(wPlayShouRank));
			ZeroMemory(wPlayFullRank, sizeof(wPlayFullRank));

			// 牌型
			BYTE cbMingGenre[GAME_PLAYER] = { CT_SINGLE, CT_SINGLE, CT_SINGLE, CT_SINGLE, CT_SINGLE };
			BYTE cbShouGenre[GAME_PLAYER] = { CT_SINGLE, CT_SINGLE, CT_SINGLE, CT_SINGLE, CT_SINGLE };
			BYTE cbFullGenre[GAME_PLAYER] = { CT_SINGLE, CT_SINGLE, CT_SINGLE, CT_SINGLE, CT_SINGLE };

			// ---------------------------------------------------------------
			// 获取明牌
			BYTE cbMingCardData[GAME_PLAYER][MAX_COUNT];
			ZeroMemory( cbMingCardData, sizeof(cbMingCardData) );
			for( int nPlayIndex = 0; nPlayIndex < GAME_PLAYER; ++nPlayIndex )
			{
				CopyMemory( cbMingCardData[nPlayIndex], &m_cbHandCardData[nPlayIndex][1], sizeof(BYTE)*m_cbCurRound );
				m_GameLogic.SortCardList( cbMingCardData[nPlayIndex], m_cbCurRound );
				cbMingGenre[nPlayIndex] = m_GameLogic.GetCardGenre(cbMingCardData[nPlayIndex], m_cbCurRound);
			}

			// 计算明牌排名
			CalculateRank(wPlayMingRank, cbMingCardData, m_cbCurRound );
			
			// ---------------------------------------------------------------
			// 获取手牌排名
			BYTE cbShouCardData[GAME_PLAYER][MAX_COUNT];
			ZeroMemory( cbShouCardData, sizeof(cbMingCardData) );
			for( int nPlayIndex = 0; nPlayIndex < GAME_PLAYER; ++nPlayIndex )
			{
				CopyMemory( cbShouCardData[nPlayIndex], &m_cbHandCardData[nPlayIndex][0], sizeof(BYTE)*(m_cbCurRound + 1) );
				m_GameLogic.SortCardList( cbShouCardData[nPlayIndex], m_cbCurRound + 1 );
				cbShouGenre[nPlayIndex] = m_GameLogic.GetCardGenre(cbShouCardData[nPlayIndex], m_cbCurRound + 1);
			}

			// 计算手牌排名
			CalculateRank(wPlayShouRank, cbShouCardData, m_cbCurRound + 1 );
			
			// ---------------------------------------------------------------
			// 计算完整牌排名
			BYTE cbFullCardData[GAME_PLAYER][MAX_COUNT];
			ZeroMemory( cbFullCardData, sizeof(cbMingCardData) );
			for( int nPlayIndex = 0; nPlayIndex < GAME_PLAYER; ++nPlayIndex )
			{
				CopyMemory( cbFullCardData[nPlayIndex], &m_cbHandCardData[nPlayIndex][0], sizeof(BYTE)*(MAX_COUNT) );
				m_GameLogic.SortCardList( cbFullCardData[nPlayIndex], MAX_COUNT );
				cbFullGenre[nPlayIndex] = m_GameLogic.GetCardGenre(cbFullCardData[nPlayIndex], MAX_COUNT);
			}

			// 计算完整牌排名
			CalculateRank(wPlayFullRank, cbFullCardData, MAX_COUNT );
			
			// 判断放弃
			do
			{
				// 排名第一 直接下注
				if( wPlayMingRank[wChairID] == 1 || wPlayShouRank[wChairID] == 1 || wPlayFullRank[wChairID] == 1 )
					break;

				// ---------------------------------------------------------------
				// 计算放弃几率
				float fGiveUp = 0.f;

				// 根据明牌修改
				fGiveUp += wPlayMingRank[wChairID] * 1.f;

				// 根据手牌修改
				fGiveUp += wPlayShouRank[wChairID] * 3.f;

				// 别人的明牌和自己的完整牌判断大小
				for( int nPlayIndex = 0; nPlayIndex < GAME_PLAYER; ++nPlayIndex )
				{
					if( nPlayIndex != wChairID && cbMingGenre[nPlayIndex] > cbFullGenre[wChairID] )
					{
						fGiveUp += 1000.f;
						break;
					}
				}

				// 根据完整牌牌型修改
				switch( cbShouGenre[wChairID] )
				{
				case CT_SINGLE:
					fGiveUp += 20.f;
					break;
				case CT_ONE_DOUBLE:
					fGiveUp += 10.f;
					break;
				case CT_TWO_DOUBLE:
					fGiveUp += 0.f;
					break;
				case CT_THREE_TIAO:
					fGiveUp -= 5.f;
					break;
				case CT_SHUN_ZI:
					fGiveUp -= 10.f;
					break;
				case CT_TONG_HUA:
					fGiveUp -= 20.f;
					break;
				case CT_HU_LU:
					fGiveUp -= 30.f;
					break;
				case CT_TIE_ZHI:
					fGiveUp -= 100.f;
					break;
				case CT_TONG_HUA_SHUN:
					fGiveUp -= 200.f;
					break;
				}
				
				// 获取桌面最大下注
				LONGLONG lTableMax = 0;
				for( int nPlayIndex = 0; nPlayIndex < GAME_PLAYER; ++nPlayIndex )
				{
					lTableMax = max(m_lTableScore[nPlayIndex] + m_lUserScore[nPlayIndex], 0);
				}

				// 根据下注修改
				fGiveUp += (sin((float)(m_lDrawMaxScore - lTableMax) / (float)m_lDrawMaxScore * 3.14f) * 60.f / 100.f);
				
				// 修正几率
				int nGiveUp = (int)fGiveUp;
				if( nGiveUp > 100 ) nGiveUp = 100;
				else if( nGiveUp < 0 ) nGiveUp = 0;

				// 判断放弃
				if( m_GameLogic.RandomArea( 2, nGiveUp, 100 - nGiveUp ) == 0 )
				{
					m_pIAndroidUserItem->SendSocketData(SUB_C_GIVE_UP, NULL, NULL);
					return true;
				}
				
			}while(false);


			// ---------------------------------------------------------------
			// 计算下注
			float	 fCanBeBet = static_cast<float>(m_lTurnMaxScore - m_lTurnLessScore);
			LONGLONG lBetAmount = 0;
			
			// 最后两轮加大下注
			float fMultiple = 1.f;
			if( m_cbCurRound >= MAX_COUNT - 1 )
				fMultiple = 2.f;

			// 根据明牌修改
			lBetAmount += (LONGLONG)( 0.04f * fCanBeBet * (float)( wPlayCount - wPlayMingRank[wChairID] + 1 ) * fMultiple );

			// 根据手牌修改
			lBetAmount += (LONGLONG)( 0.03f * fCanBeBet * (float)( wPlayCount - wPlayShouRank[wChairID] + 1  ) * fMultiple );

			// 根据完整牌修改
			lBetAmount += (LONGLONG)( 0.02f * fCanBeBet * (float)( wPlayCount - wPlayFullRank[wChairID] + 1  ) * fMultiple );
			
			// 根据完整牌牌型修改
			switch( cbShouGenre[wChairID] )
			{
			case CT_SINGLE:
				lBetAmount += (LONGLONG)( 0.02f * fCanBeBet );
				break;
			case CT_ONE_DOUBLE:
				lBetAmount += (LONGLONG)( 0.03f * fCanBeBet );
				break;
			case CT_TWO_DOUBLE:
				lBetAmount += (LONGLONG)( 0.08f * fCanBeBet );
				break;
			case CT_THREE_TIAO:
				lBetAmount += (LONGLONG)( 0.10f * fCanBeBet );
				break;
			case CT_SHUN_ZI:
				lBetAmount += (LONGLONG)( 0.15f * fCanBeBet );
				break;
			case CT_TONG_HUA:
				lBetAmount += (LONGLONG)( 0.18f * fCanBeBet );
				break;
			case CT_HU_LU:
				lBetAmount += (LONGLONG)( 0.20f * fCanBeBet );
				break;
			case CT_TIE_ZHI:
				lBetAmount += (LONGLONG)( 0.35f * fCanBeBet );
				break;
			case CT_TONG_HUA_SHUN:
				lBetAmount += (LONGLONG)( 0.50f * fCanBeBet );
				break;
			}
			
			// 下注范围
			LONGLONG lTurnMinScore = m_lTurnLessScore;
			LONGLONG lTurnMaxScore = m_lTurnLessScore + lBetAmount;
			if( lTurnMaxScore > m_lTurnMaxScore ) lTurnMaxScore = m_lTurnMaxScore;
			if( lTurnMaxScore < lTurnMinScore ) lTurnMaxScore = lTurnMinScore;

			// 下注数量
			LONGLONG lAddScore = lTurnMinScore;
			if( lTurnMaxScore > lTurnMinScore )
				lAddScore += ((rand()*( (lTurnMaxScore - lTurnMinScore) / RAND_MAX ))%( lTurnMaxScore - lTurnMinScore ));

			// 大于最大注的80%， 有80%直接最大
			if( lAddScore > m_lTurnMaxScore * 80 / 100 )
			{
				if( m_GameLogic.RandomArea( 2, 80, 20 ) == 0 )
				{
					lAddScore = lTurnMaxScore;
				}
			}
			// 只加注15%，有60%只跟注
			else if( lAddScore < m_lTurnLessScore + ((m_lTurnMaxScore - m_lTurnLessScore) * 15 / 100) )
			{
				if( m_GameLogic.RandomArea( 2, 60, 40 ) == 0 )
				{
					lAddScore = m_lTurnLessScore;
				}
			}

			// 加注次数过多， 直接下最小值
			m_wBetCount++;
			if( m_wBetCount > 2 && m_GameLogic.RandomArea( 2, 80, 20 ) == 0 )
			{
				lAddScore = m_lTurnLessScore;
			}
			
			// 真实下注量
			lAddScore = lAddScore - (m_lTableScore[wChairID]+m_lUserScore[wChairID]);

			// 发送下注数量
			CMD_C_AddScore AddScore;
			AddScore.lScore = lAddScore;
			m_pIAndroidUserItem->SendSocketData(SUB_C_ADD_SCORE,&AddScore,sizeof(AddScore));

			return true;	
		}
	}

	return false;
}

//游戏消息
bool CAndroidUserItemSink::OnEventGameMessage(WORD wSubCmdID, void * pData, WORD wDataSize)
{
	switch (wSubCmdID)
	{	
	case SUB_S_GAME_START:	//游戏开始
		{
			return OnSubGameStart(pData,wDataSize);
		}
	case SUB_S_GIVE_UP:		//用户放弃
		{
			return OnSubGiveUp(pData,wDataSize);
		}
	case SUB_S_SEND_CARD:	//发牌消息
		{
			return OnSubSendCard(pData,wDataSize);
		}	
	case SUB_S_ADD_SCORE:	//玩家加注
		{
			return OnSubAddGold(pData,wDataSize);
		}
	case SUB_S_GAME_END:	//游戏结束
		{
			return OnSubGameEnd(pData,wDataSize);
		}
	case SUB_S_TRUE_END:	//游戏结束
		{
			return OnSubTrueEnd(pData,wDataSize);
		}
	}

	//错误断言
	ASSERT(FALSE);

	return true;
}

//游戏消息
bool CAndroidUserItemSink::OnEventFrameMessage(WORD wSubCmdID, void * pData, WORD wDataSize)
{
	return true;
}

//场景消息
bool CAndroidUserItemSink::OnEventSceneMessage(BYTE cbGameStatus, bool bLookonOther, void * pData, WORD wDataSize)
{
	switch (cbGameStatus)
	{
	case GAME_STATUS_FREE:		//空闲状态
		{
			//效验数据
			if (wDataSize!=sizeof(CMD_S_StatusFree)) return false;

			//消息处理
			CMD_S_StatusFree * pStatusFree=(CMD_S_StatusFree *)pData;

			//机器人
			m_lRobotScoreMin = pStatusFree->lRobotScoreMin;					// 积分低于取款
			m_lRobotScoreMax = pStatusFree->lRobotScoreMax;					// 积分高于存款
			m_lRobotBankTake[0] = pStatusFree->lRobotBankTake[0];			// 取款额度
			m_lRobotBankTake[1] = pStatusFree->lRobotBankTake[1];			// 取款额度
			m_lRobotBankSave = pStatusFree->lRobotBankSave;					// 存款额度

			// 判断操作
			if( m_pIAndroidUserItem->GetMeUserItem()->GetUserScore() < m_lRobotScoreMin
				|| m_pIAndroidUserItem->GetMeUserItem()->GetUserScore() > m_lRobotScoreMax )
			{
				m_pIAndroidUserItem->SetGameTimer(IDI_BANK_OPERATION, (rand()%3) + 2 );
			}
			else if( m_pIAndroidUserItem->GetMeUserItem()->GetUserStatus() != US_READY )
			{
				m_pIAndroidUserItem->SetGameTimer(IDI_START_GAME, (rand()%3) + 2);
			}

			return true;
		}
	case GAME_STATUS_PLAY:	//游戏状态
		{
			//效验数据
			if (wDataSize!=sizeof(CMD_S_StatusPlay)) return false;
			CMD_S_StatusPlay * pStatusPlay=(CMD_S_StatusPlay *)pData;
	
			//机器人
			m_lRobotScoreMin = pStatusPlay->lRobotScoreMin;					// 积分低于取款
			m_lRobotScoreMax = pStatusPlay->lRobotScoreMax;					// 积分高于存款
			m_lRobotBankTake[0] = pStatusPlay->lRobotBankTake[0];			// 取款额度
			m_lRobotBankTake[1] = pStatusPlay->lRobotBankTake[1];			// 取款额度
			m_lRobotBankSave = pStatusPlay->lRobotBankSave;					// 存款额度

			return true;
		}
	}

	ASSERT(FALSE);

	return false;
}

//用户进入
void CAndroidUserItemSink::OnEventUserEnter(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

//用户离开
void CAndroidUserItemSink::OnEventUserLeave(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

//用户积分
void CAndroidUserItemSink::OnEventUserScore(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

//用户状态
void CAndroidUserItemSink::OnEventUserStatus(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

//用户段位
void CAndroidUserItemSink::OnEventUserSegment(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

//游戏开始
bool CAndroidUserItemSink::OnSubGameStart(const void * pBuffer, WORD wDataSize)
{
	// 效验数据
	if (wDataSize != sizeof(CMD_S_GameStart)) return false;
	CMD_S_GameStart * pGameStart = (CMD_S_GameStart *)pBuffer;

	//状态变量
	m_bShowHand=false;
	m_wCurrentUser=pGameStart->wCurrentUser;
	ZeroMemory(m_cbPlayStatus,sizeof(m_cbPlayStatus));

	//设置变量
	m_wBetCount = 0;
	m_lCellScore = pGameStart->lCellScore;
	m_lServiceCharge=pGameStart->lServiceCharge;
	m_lDrawMaxScore=pGameStart->lDrawMaxScore;
	m_lTurnMaxScore=pGameStart->lTurnMaxScore;
	m_lTurnLessScore=pGameStart->lTurnLessScore;
	ZeroMemory(m_lUserScore,sizeof(m_lUserScore));
	ZeroMemory(m_lTableScore,sizeof(m_lTableScore));

	//设置牌
	CopyMemory(m_cbHandCardData, pGameStart->cbHandCardData, sizeof(m_cbHandCardData));

	//玩家设置
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		if (pGameStart->cbCardData[i]!=0x00)
		{
			//状态变量
			m_cbPlayStatus[i] = TRUE;
			m_lTableScore[i] = pGameStart->lCellScore;
		}

	}

	//当前轮数
	m_cbCurRound = 1;

	//设置定时器
	if( m_wCurrentUser == m_pIAndroidUserItem->GetChairID() )
	{
		m_pIAndroidUserItem->SetGameTimer( IDI_USER_ADD_SCORE, TIME_USER_ADD_SCORE + (rand()%5) + 3 );
	}

	return true;
}

//用户下注
bool CAndroidUserItemSink::OnSubAddGold(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_AddScore));
	if (wDataSize!=sizeof(CMD_S_AddScore)) return false;

	//变量定义
	CMD_S_AddScore * pAddScore = (CMD_S_AddScore *)pBuffer;

	//变量定义
	WORD wAddScoreUser = pAddScore->wAddScoreUser;

	//变量定义
	LONGLONG lCurrentScore = pAddScore->lUserScoreCount;
	LONGLONG lDrawAddScore = m_lTableScore[wAddScoreUser];

	//筹码设置
	m_lUserScore[wAddScoreUser] = lCurrentScore;

	//设置变量
	m_wCurrentUser=pAddScore->wCurrentUser;
	m_lTurnLessScore=pAddScore->lTurnLessScore;

	//玩家下注
	if ( m_wCurrentUser == m_pIAndroidUserItem->GetChairID() )
	{
		m_pIAndroidUserItem->SetGameTimer( IDI_USER_ADD_SCORE, TIME_USER_ADD_SCORE + (rand()%2) );
	}

	return true;
}

// 发牌消息
bool CAndroidUserItemSink::OnSubSendCard(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_SendCard));
	if (wDataSize!=sizeof(CMD_S_SendCard)) return false;

	//变量定义
	CMD_S_SendCard * pSendCard=(CMD_S_SendCard *)pBuffer;


	//加注次数
	m_wBetCount = 0;

	//添加轮数
	m_cbCurRound++;
	if( m_cbCurRound > MAX_COUNT )
		m_cbCurRound = MAX_COUNT;

	//设置变量
	m_wCurrentUser = pSendCard->wCurrentUser;
	m_lTurnMaxScore = pSendCard->lTurnMaxScore;

	//汇集金币
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//桌面筹码
		m_lTableScore[i] += m_lUserScore[i];

		//用户筹码
		m_lUserScore[i]=0L;
	}

	//玩家下注
	if ( m_wCurrentUser == m_pIAndroidUserItem->GetChairID() )
	{
		m_pIAndroidUserItem->SetGameTimer( IDI_USER_ADD_SCORE, TIME_USER_ADD_SCORE + (rand()%5) + 1);
	}

	return true;
}

// 放弃
bool CAndroidUserItemSink::OnSubGiveUp(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize == sizeof(CMD_S_GiveUp));
	if (wDataSize != sizeof(CMD_S_GiveUp)) return false;

	//变量定义
	CMD_S_GiveUp * pGiveUp = (CMD_S_GiveUp *)pBuffer;

	//变量定义
	WORD wGiveUpUser = pGiveUp->wGiveUpUser;

	//下注变量
	m_lDrawMaxScore = pGiveUp->lDrawMaxScore;
	m_lTurnMaxScore = pGiveUp->lTrunMaxScore;

	//状态变量
	m_cbPlayStatus[wGiveUpUser]=FALSE;

	//设置用户
	m_wCurrentUser=pGiveUp->wCurrentUser;

	//玩家下注
	if ( m_wCurrentUser == m_pIAndroidUserItem->GetChairID() )
	{
		m_pIAndroidUserItem->SetGameTimer( IDI_USER_ADD_SCORE, TIME_USER_ADD_SCORE + (rand()%5) + 2);
	}

	return true;
}

// 游戏结束
bool CAndroidUserItemSink::OnSubGameEnd(const void * pBuffer, WORD wDataSize)
{
	//效验参数
	if (wDataSize!=sizeof(CMD_S_GameEnd)) return false;
	CMD_S_GameEnd * pGameEnd=(CMD_S_GameEnd *)pBuffer;

	//删除定时器	
	m_pIAndroidUserItem->KillGameTimer( IDI_USER_ADD_SCORE );

	//加注信息
	m_lDrawMaxScore=0L;
	m_lTurnMaxScore=0L;
	m_lTurnLessScore=0L;
	ZeroMemory(m_lUserScore,sizeof(m_lUserScore));
	ZeroMemory(m_lTableScore,sizeof(m_lTableScore));

	//状态变量
	m_bShowHand=false;
	m_wCurrentUser=INVALID_CHAIR;
	ZeroMemory(m_cbPlayStatus,sizeof(m_cbPlayStatus));

	return true;
}

//游戏结束
bool CAndroidUserItemSink::OnSubTrueEnd(const void * pBuffer, WORD wDataSize)
{

	// 判断操作
	if( m_pIAndroidUserItem->GetMeUserItem()->GetUserScore() < m_lRobotScoreMin
		|| m_pIAndroidUserItem->GetMeUserItem()->GetUserScore() > m_lRobotScoreMax )
	{
		m_pIAndroidUserItem->SetGameTimer(IDI_BANK_OPERATION, (rand()%3) + 2);
	}
	else
	{
		m_pIAndroidUserItem->SetGameTimer(IDI_START_GAME, (rand()%3) + 2);
	}

	return true;
}

//读取配置
void CAndroidUserItemSink::ReadConfigInformation()
{

}

// 计算排名
void CAndroidUserItemSink::CalculateRank( OUT WORD wRank[GAME_PLAYER], IN BYTE cbCardData[GAME_PLAYER][MAX_COUNT], IN BYTE cbCardCount )
{
	// 清空排名
	for( int nPlay = 0; nPlay < GAME_PLAYER; ++nPlay )
	{
		wRank[nPlay] =  m_cbPlayStatus[nPlay];
	}

	// 计算明牌牌型
	for( int nPlayFirst = 0; nPlayFirst < GAME_PLAYER; ++nPlayFirst )
	{
		if( m_cbPlayStatus[nPlayFirst] )
		{
			for( int nNextIndex = 0; nNextIndex < GAME_PLAYER; ++nNextIndex )
			{
				if( m_cbPlayStatus[nNextIndex] && nNextIndex != nPlayFirst )
				{
					// 比较
					if( !m_GameLogic.CompareCard( cbCardData[nPlayFirst], cbCardData[nNextIndex], cbCardCount ) )
					{
						wRank[nPlayFirst]++;
					}
				}
			}
		}
	}
}


//组件创建函数
DECLARE_CREATE_MODULE(AndroidUserItemSink);

//////////////////////////////////////////////////////////////////////////
