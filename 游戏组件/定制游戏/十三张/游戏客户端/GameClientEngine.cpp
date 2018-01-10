#include "Stdafx.h"
#include "GameClient.h"
#include "GameClientEngine.h"

#include "Log4File.h"
//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameClientEngine, CGameFrameEngine)
		ON_WM_TIMER()
		ON_WM_CREATE()
		ON_MESSAGE(IDM_START, OnStart)
		ON_MESSAGE(IDM_CALL_BANKER, OnCallBanker)
		ON_MESSAGE(IDM_NO_CALL_BANKER, OnNoCallBanker)
		ON_MESSAGE(IDM_SETCHIP, OnSetChip)		
		ON_MESSAGE(IDM_SPLIT_CARD, OnSplitCard)
		ON_MESSAGE(IDM_SPLIT_FINISH, OnSplitFinish)
		ON_MESSAGE(IDM_SHUFFLE_FINISH, OnShuffleFinish)
		ON_MESSAGE(IDM_DISPATCH_CARD_FINISH, OnDispatchCardFinish)
		ON_MESSAGE(IDM_PROMPT, OnPrompt)
		ON_MESSAGE(IDM_VOICE_PHRASE, OnVoicePhrase)
		ON_MESSAGE(IDM_SORT_CARD, OnSortCard)	
		ON_MESSAGE(IDM_TRUSTEE, OnTrustee)		
		ON_MESSAGE(IDM_FRONTCARD, OnFrontCard)
		ON_MESSAGE(IDM_BACKCARD, OnBackCard)
		ON_MESSAGE(IDM_MIDCARD, OnMidCard)
		ON_MESSAGE(IDM_SHOWCARD, OnShowCard)
		ON_MESSAGE(IDM_LEFT_HIT_CARD, OnLeftHitCard)
		ON_MESSAGE(IDM_RESTORE, OnRestoreCard)
		ON_MESSAGE(IDM_REVERSAL_TURN_FINISH, OnRevesalTurnFinish)
		ON_MESSAGE(IDM_MSG_INFO, OnMsgInfo)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
//宏定义

//结束原因
#define GER_NORMAL						0x00							//常规结束
#define GER_DISMISS						0x01							//游戏解散
#define GER_USER_LEFT					0x02							//用户离开

//游戏定时器
#define IDI_START_GAME					200								//开始定时器
#define IDI_SETCHIP						201								//下注定时器
#define IDI_CALL_BANKER					202								//叫庄定时器
#define IDI_RANGE_CARD					203								//摊牌定时器
#define IDI_SHOW_CARD					209								//开牌定时器
#define IDI_COMPARE_FRONT				204								//比较定时器
#define IDI_COMPARE_MID					205								//比较定时器
#define IDI_COMPARE_BACK				206								//比较定时器
#define IDI_COMPLETE_COMPARE			207								//完成定时器
#define IDI_CAN_PLAY_VOICE				208								//语音定时器

#define IDI_ALL_RANGE_CARD				209								//通用定时器

#define IDI_FU_ZHU						801								//辅助定时器
////////////////////////////////////////////////////////////////////////////////////

//构造函数
CGameClientEngine::CGameClientEngine() 
{
	
	//时间定义
	m_cbTimeStartGame = 0;
	m_cbTimeCallBanker = 0;
	m_cbTimeSetChip = 0;
	m_cbTimeRangeCard = 0;
	m_cbTimeShowCard = 0;
	
	m_cbFuZhuTime=0;

	//游戏变量
	m_wBanker =	INVALID_CHAIR ; 
	for(WORD i=0 ; i<GAME_PLAYER ; ++i) m_lUserChip[i] = 0 ;
	m_bHandCardCount=0 ;
	ZeroMemory(m_bSegmentCard , sizeof(m_bSegmentCard)) ;
	ZeroMemory(m_lScoreTimes , sizeof(m_lScoreTimes)) ;
	m_wMaxChip=0 ;
	m_bFinishSegment=false ;
	m_bIsShowCard = false;
	m_tagSegInfo.Init();

	ZeroMemory(m_bGameStatus, sizeof(m_bGameStatus));
	m_bRequestSplit = false;
	m_bTrustee = false;

	ZeroMemory(&m_SearchCardResult, sizeof(m_SearchCardResult));
	m_cbSearchTypeIndex = 0;
	m_cbSortTypeIndex = 0;

	m_nCompareTime = 6000;
	m_bCanPlayVoice = true;
	m_bComparedCard = FALSE;

	m_bCheatRight = false;
	ZeroMemory(m_bUserCheat,sizeof(m_bUserCheat));

	return;
}

//析构函数
CGameClientEngine::~CGameClientEngine()
{
	if (AfxGetMainWnd())
		this->DestroyWindow();
}

//初始函数
bool CGameClientEngine::OnInitGameEngine()
{
	//设置图标
	HICON hIcon=LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME));
	m_pIClientKernel->SetGameAttribute(KIND_ID,GAME_PLAYER,VERSION_CLIENT,hIcon,GAME_NAME);
	SetIcon(hIcon,TRUE);
	SetIcon(hIcon,FALSE);

	//BYTE cbCardData[52];
	//m_GameLogic.RandCardList(cbCardData, 52);
	//BYTE cbHandData[13];
	//CopyMemory(cbHandData, cbCardData, 13);
	//m_GameLogic.SortCardList(cbHandData, 13);

	//m_GameClientView.SetCardData(2, cbHandData, 13, enCCTHandCard);
	//m_GameClientView.m_UserCardControl[2].ShowWindow(SW_SHOW);
	//m_GameClientView.m_UserCardControl[2].SetDisplayFlag(true);
	//CLog4CFile::WriteLog(AfxGetApp()->GetIDispatch())
	/*OUTPUT_LOG4C(TEXT("GameClientEngine To Initialize"));*/

	return true;
}

//重置框架
bool CGameClientEngine::OnResetGameEngine()
{
	//时间定义
	m_cbTimeStartGame = 0;
	m_cbTimeCallBanker = 0;
	m_cbTimeSetChip = 0;
	m_cbTimeRangeCard = 0;
	m_cbTimeShowCard = 0;
	
	m_cbFuZhuTime=0;

	//游戏变量
	m_wBanker =	INVALID_CHAIR ;
	for(WORD i=0 ; i<GAME_PLAYER ; ++i) m_lUserChip[i] = 0 ;
	m_bHandCardCount=0 ;
	ZeroMemory(m_bSegmentCard , sizeof(m_bSegmentCard)) ;
	ZeroMemory(m_lScoreTimes , sizeof(m_lScoreTimes)) ;
	ZeroMemory(m_bHandCardData , sizeof(m_bHandCardData)) ;
	ZeroMemory(m_bTmpHandCardData , sizeof(m_bTmpHandCardData)) ;
	m_wMaxChip=0 ;
	m_tagSegInfo.Init();

	ZeroMemory(m_bGameStatus, sizeof(m_bGameStatus));
	m_bRequestSplit = false;
	m_bIsShowCard = false;
	m_bTrustee = false;

	ZeroMemory(&m_SearchCardResult, sizeof(m_SearchCardResult));
	m_cbSearchTypeIndex = 0;	
	m_cbSortTypeIndex = 0;

	//语音短语
	m_bCanPlayVoice = true;
	m_bComparedCard = FALSE;

	//设置扑克
	m_GameClientView.SetCardData(INVALID_CHAIR , NULL , 0 , enCCTHandCard) ;
	m_GameClientView.SetCardData(INVALID_CHAIR , NULL , 0 , enCCTAllSegmentCard) ;
	m_GameClientView.m_CenterCardControl.SetCardData(-1, NULL, 0);

	//设置界面
	m_GameClientView.SetBanker(INVALID_CHAIR) ;
	m_GameClientView.SetCallBankerStatus(INVALID_CHAIR, false) ;
	m_GameClientView.SetTrustee(INVALID_CHAIR, false);

	m_bCheatRight = false;
	ZeroMemory(m_bUserCheat,sizeof(m_bUserCheat));

	return true;
}

//时钟删除
bool CGameClientEngine::OnEventGameClockKill(WORD wChairID)
{
	return true;
}
//时间消息
bool CGameClientEngine::OnEventGameClockInfo(WORD wChairID, UINT nElapse, WORD wClockID)
{
	switch (wClockID)
	{
	case IDI_START_GAME:		//开始游戏
		{
			if (!IsLookonMode() && (wChairID==GetMeChairID()) &&
				((nElapse <= 0) || (m_bTrustee && nElapse < (UINT)m_cbTimeStartGame-1)))
			{
				if (true == m_bTrustee)
				{
					OnStart(0, 0);
				}
				else
				{
					ASSERT(AfxGetMainWnd());
					if(AfxGetMainWnd()) AfxGetMainWnd()->PostMessage(WM_CLOSE);
					return false;
				}
			}
			
			if ((nElapse <= 5) && (wChairID == GetMeChairID()) && (false == IsLookonMode()))
				PlayGameSound(AfxGetInstanceHandle(), TEXT("GAME_WARN"));

			return true;
		}
	case IDI_CALL_BANKER :		//玩家叫庄
		{
			if ((nElapse <= 0) || (m_bTrustee && nElapse < (UINT)m_cbTimeCallBanker-1))
			{
				if ((wChairID == GetMeChairID()) && (false == IsLookonMode()))
					OnNoCallBanker(0, 0);
				return true;
			}

			if ((nElapse <= 5) && (wChairID == GetMeChairID()) && (false == IsLookonMode()))
				PlayGameSound(AfxGetInstanceHandle(), TEXT("GAME_WARN")) ;

			return true;
		}
	case IDI_SETCHIP:			//设置下注
		{
			if (wChairID != GetMeChairID() || IsLookonMode()) return true;

			if ((nElapse <= 0) || (m_bTrustee && nElapse < (UINT)m_cbTimeSetChip-1))
			{
				if ((wChairID == GetMeChairID()) && (false == IsLookonMode()))
					OnSetChip(0, 0);
				return true;
			}
			
			if ((nElapse <= 5) && (wChairID == GetMeChairID()) && (false == IsLookonMode()))
				PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_WARN"));
			
			return true;
		}
	case IDI_RANGE_CARD:         //玩家亮牌
		{
			if ((nElapse <= 0) || (m_bTrustee && nElapse < (UINT)m_cbTimeRangeCard-1))
			{
				if ((wChairID == GetMeChairID()) && (false == IsLookonMode()))
				{
					//尚未理牌或理牌未完成
					if (m_bHandCardData[wChairID]!=0)
					{
						//自动代打
						AutomaticOutCard();
						//摊牌
						OnShowCard(0 , 0);
					}
					else
					{
						//直接摊牌
						OnShowCard(0, 0);
					}
				}
				return false;
			}
			
			if ((nElapse<=5)&&(wChairID==GetMeChairID())) 
				PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_WARN"));
			
			return true;
		}
	case IDI_SHOW_CARD:
		{
			if ((nElapse <= 0) || (m_bTrustee && nElapse < (UINT)m_cbTimeShowCard-1))
			{
				if ((wChairID == GetMeChairID()) && (false == IsLookonMode()))
				{
					//牌已理好，倒计时到，自动开牌
					OnShowCard(0, 0);
				}
				return false;
			}
			return true;
		}
	}

	return false;
}
//用户进入
VOID CGameClientEngine::OnEventUserEnter(IClientUserItem * pIClientUserItem, bool bLookonUser)
{
	if (bLookonUser==false)
	{
		//更新界面
		m_GameClientView.InvalidGameView(0,0,0,0);
	}
	return;
}

//用户离开
VOID CGameClientEngine::OnEventUserLeave(IClientUserItem * pIClientUserItem, bool bLookonUser)
{
	if (bLookonUser==false)
	{
		//更新界面
		m_GameClientView.InvalidGameView(0,0,0,0);
	}
	return;
}

//旁观状态
bool CGameClientEngine::OnEventLookonMode(VOID * pData, WORD wDataSize)
{
	return true;
}

//网络消息
bool CGameClientEngine::OnEventGameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{	
	switch (wSubCmdID)
	{
	case SUB_S_START_CALL_BANKER:		//开始叫庄
		{
			return OnSubBeginCall(pData, wDataSize);
		}
	case SUB_S_CALL_BANKER :			//玩家叫庄
		{
			return OnSubUserCall(pData, wDataSize);
		}
	case SUB_S_END_ALL_NO_CALL:			//都不叫庄
		{
			return OnSubAllNotCall(pData, wDataSize);
		}
	case SUB_S_SEND_CARD_EX:			//发送扑克
		{
			return OnSubSendCardEx(pData,wDataSize);
		}
	case SUB_S_SETCHIP:					//设置压注
		{
			return OnSubSetChip(pData , wDataSize) ;
		}
	case SUB_S_SETSEGMENT:				//设置分段
		{
            return OnSubSetSegment(pData , wDataSize) ;
		}
	case SUB_S_SHOW_CARD:				//玩家摊牌
		{
			return OnSubShowCard(pData , wDataSize) ;
		}
	case SUB_S_COMPARE_CARD:			//比较扑克
		{
			return OnSubCompareCard(pData , wDataSize) ;
		}
	case SUB_S_GAME_END:				//游戏结束
		{
			return OnSubGameEnd(pData,wDataSize);
		}	
	case SUB_S_PLAYER_EXIT:				//用户强退
		{
			return OnSubPlayerExit(pData,wDataSize);
		}
	case SUB_S_DISPATCH_START:			//开始发牌
		{
			return OnSubDispatchStart(pData, wDataSize);
		}
	case SUB_S_DISPATCH_FINISH:			//发牌结束
		{
			return true;
		}
	case SUB_S_SPLIT_CARD:				//切牌消息
		{
			return OnSubSplitCard(pData, wDataSize);
		}
	case SUB_S_TRUSTEE:					//托管控制
		{
			return	OnSubTrustee(pData, wDataSize);
		}
	case SUB_S_VOICE_PHRASE:			//语音广播
		{
			return OnSubVoicePhrase(pData, wDataSize);
		}
	case SUB_S_CHEAT_CARD:				//作弊数据	
		{
			return OnSubCheatCard(pData,wDataSize);
		}
    case SUB_S_ROBOT_CONTROL:           //机器人控制
		{
			return true;
		}
	}
	
	return false;
}

//游戏场景
bool CGameClientEngine::OnEventSceneMessage(BYTE cbGameStatus, bool bLookonUser, VOID * pData, WORD wDataSize)
{
	switch (cbGameStatus)
	{
	case GS_WK_FREE:	//空闲状态
		{
			//效验数据
			ASSERT(wDataSize == sizeof(CMD_S_StatusFree));
			if (wDataSize != sizeof(CMD_S_StatusFree)) return false;

			//变量定义
			CMD_S_StatusFree* pStatusFree = (CMD_S_StatusFree*)pData;

			//时间定义
			m_cbTimeStartGame = pStatusFree->cbTimeStartGame;
			m_cbTimeCallBanker = pStatusFree->cbTimeCallBanker;
			m_cbTimeSetChip = pStatusFree->cbTimeSetChip;
			m_cbTimeRangeCard = pStatusFree->cbTimeRangeCard;
			m_cbTimeShowCard = pStatusFree->cbTimeShowCard;
			
			//历史成绩
			for (WORD i = 0; i < GAME_PLAYER; i++)
			{
				//获取变量
				tagHistoryScore * pHistoryScore = m_HistroyScore.GetHistoryScore(i);

				//设置积分
				pHistoryScore->lTurnScore = pStatusFree->lTurnScore[i];
				pHistoryScore->lCollectScore = pStatusFree->lCollectScore[i];

				//绑定设置
				WORD wViewChairID=SwitchViewChairID(i);
				m_GameClientView.SetHistoryScore(wViewChairID, pHistoryScore);
			}

			//设置状态
			CopyMemory(m_bGameStatus, pStatusFree->bGameStatus, sizeof(m_bGameStatus));
			for(BYTE i=0;i<GAME_PLAYER;i++)
			{
				m_GameClientView.SetGameStatus(SwitchViewChairID(i),m_bGameStatus[i]);
			}

			//设置控件
			if (IsLookonMode() == false)
			{
				

                UpdateButtonState();

				//设置扑克
				 m_GameClientView.m_UserCardControl[2].SetDisplayFlag(true);
				
				 //允许托管
				m_GameClientView.m_btTrustee.EnableWindow(TRUE);
				
				//开始设置
				if (GetMeUserItem()->GetUserStatus()!=US_READY)
				{
					tagServerAttribute *pServerAttribute=m_pIClientKernel->GetServerAttribute();
					bool bShowStart = (0 == (pServerAttribute->wServerType & GAME_GENRE_MATCH) &&
						0 == (pServerAttribute->dwServerRule & SR_ALLOW_AVERT_CHEAT_MODE));

					m_GameClientView.m_btStart.ShowWindow(bShowStart);
					m_GameClientView.m_btStart.EnableWindow(bShowStart) ;

					if(bShowStart)
					{
						m_GameClientView.m_btStart.SetFocus();

						//开始计时
						SetGameClock(GetMeChairID(),IDI_START_GAME,m_cbTimeStartGame);
					}
					
				}	
			}
			
			return true;
		}

	case GS_WK_CALL_BANKER :	//叫庄状态
		{
			//验证参数
			ASSERT( sizeof( CMD_S_StatusCallBanker ) == wDataSize ) ;
			if ( sizeof( CMD_S_StatusCallBanker ) != wDataSize ) return false ;

			//关定时器
			m_GameClientView.StopDispatchCard();

			CMD_S_StatusCallBanker  *pStatusCallBanker = ( CMD_S_StatusCallBanker * ) pData ;
			
			if ( GetMeChairID() == pStatusCallBanker->wCurrentCaller  && !IsLookonMode() )
			{
				//设置按钮
				m_GameClientView.m_btCallBanker.ShowWindow( SW_SHOW ) ;
				m_GameClientView.m_btNoCallBanker.ShowWindow( SW_SHOW ) ;
			}
			
			//提示信息
			m_GameClientView.SetTipInfoState(1);

			//时间定义
			m_cbTimeStartGame = pStatusCallBanker->cbTimeStartGame;
			m_cbTimeCallBanker = pStatusCallBanker->cbTimeCallBanker;
			m_cbTimeSetChip = pStatusCallBanker->cbTimeSetChip;
			m_cbTimeRangeCard = pStatusCallBanker->cbTimeRangeCard;
			m_cbTimeShowCard = pStatusCallBanker->cbTimeShowCard;

			//历史成绩
			for (WORD i = 0; i < GAME_PLAYER; i++)
			{
				//获取变量
				tagHistoryScore * pHistoryScore = m_HistroyScore.GetHistoryScore(i);

				//设置积分
				pHistoryScore->lTurnScore = pStatusCallBanker->lTurnScore[i];
				pHistoryScore->lCollectScore = pStatusCallBanker->lCollectScore[i];

				//绑定设置
				WORD wViewChairID=SwitchViewChairID(i);
				m_GameClientView.SetHistoryScore(wViewChairID, pHistoryScore);
			}

			//设置状态
			CopyMemory(m_bGameStatus,pStatusCallBanker->bGameStatus,sizeof(m_bGameStatus));
			for(BYTE i=0;i<GAME_PLAYER;i++)
			{
				m_GameClientView.SetGameStatus(SwitchViewChairID(i),m_bGameStatus[i]);
			}

			if (m_bGameStatus[GetMeChairID()] == false || IsLookonMode() )
			{
				m_GameClientView.m_btTrustee.EnableWindow(FALSE);
			}
			//设置定时器
			SetGameClock( pStatusCallBanker->wCurrentCaller, IDI_CALL_BANKER, m_cbTimeCallBanker ) ;
			
			return true ;
		}

	case GS_WK_SETCHIP:		//下注状态
		{
			//验证参数
			ASSERT( sizeof( CMD_S_StatusSetChip ) == wDataSize ) ;
			if ( sizeof( CMD_S_StatusSetChip ) != wDataSize ) return false ;
			//关定时器
			m_GameClientView.StopDispatchCard();

			CMD_S_StatusSetChip *pStatusSetChip=(CMD_S_StatusSetChip *)pData ;
			
			m_wBanker = pStatusSetChip->wBanker ;
			m_GameClientView.SetBanker(SwitchViewChairID(m_wBanker)) ;

			//时间定义
			m_cbTimeStartGame = pStatusSetChip->cbTimeStartGame;
			m_cbTimeCallBanker = pStatusSetChip->cbTimeCallBanker;
			m_cbTimeSetChip = pStatusSetChip->cbTimeSetChip;
			m_cbTimeRangeCard = pStatusSetChip->cbTimeRangeCard;
			m_cbTimeShowCard = pStatusSetChip->cbTimeShowCard;

			//历史成绩
			for (WORD i = 0; i < GAME_PLAYER; i++)
			{
				//获取变量
				tagHistoryScore * pHistoryScore = m_HistroyScore.GetHistoryScore(i);

				//设置积分
				pHistoryScore->lTurnScore = pStatusSetChip->lTurnScore[i];
				pHistoryScore->lCollectScore = pStatusSetChip->lCollectScore[i];

				//绑定设置
				WORD wViewChairID=SwitchViewChairID(i);
				m_GameClientView.SetHistoryScore(wViewChairID, pHistoryScore);
			}

			//最高下注
			m_GameClientView.SetMaxChip(m_wMaxChip) ;
		
			//跟新下注按钮
			UpdateBlockButton(0, SW_HIDE, FALSE);
	
			//设置状态
			CopyMemory(m_bGameStatus,pStatusSetChip->bGameStatus,sizeof(m_bGameStatus));			
			for(BYTE i=0;i<GAME_PLAYER;i++)
				m_GameClientView.SetGameStatus(SwitchViewChairID(i),m_bGameStatus[i]);

			//设置下注
			CopyMemory(m_lUserChip , pStatusSetChip->lChip, sizeof(m_lUserChip)) ;
			for(WORD i=0 ; i<GAME_PLAYER ; ++i) 
				m_GameClientView.SetChip(SwitchViewChairID(i) , m_lUserChip[i]);

			//设置按钮
			if(false==IsLookonMode() )
			{
				WORD wMeChairID = GetMeChairID();

				if(0==m_lUserChip[wMeChairID] && wMeChairID!=m_wBanker && m_bGameStatus[wMeChairID] != false)//(-1==m_lUserChip[wMeChairID] && wMeChairID!=m_wBanker && m_bGameStatus[wMeChairID] != false)
				{
					//下注大小
					SCORE lChipsArray[SET_CHIP_COUNT] = {0};
					CopyMemory(lChipsArray, pStatusSetChip->lChipArray[wMeChairID], sizeof(lChipsArray));

					m_GameClientView.SetChipArray(lChipsArray) ;
                    
					UpdateChipButtonState(SW_SHOW, TRUE) ;
	
					//设置时间
					SetGameClock(wMeChairID , IDI_SETCHIP , m_cbTimeSetChip) ;
				}

				//设置辅助时钟
				if (!IsLookonMode() && m_bGameStatus[GetMeChairID()] == true && !pStatusSetChip->bSetChipFinally)
				{
					for(BYTE i=0;i<GAME_PLAYER;i++)
					{
						//下注完成不用开启定时器
						if(pStatusSetChip->bSetChipFinally) break;
						if(i != m_wBanker && i != GetMeChairID() && m_bGameStatus[i] == true)
						{
							m_GameClientView.ShowFuZhuClock(SwitchViewChairID(i),true);
						}
					}
					m_cbFuZhuTime=m_cbTimeSetChip;
					m_GameClientView.SetFuZhuTime(m_cbFuZhuTime--);
					//设置时间
					SetTimer(IDI_FU_ZHU, 1000, NULL);
				}

				//提示信息
				m_GameClientView.SetTipInfoState(2);
			}
			else
			{
				m_GameClientView.m_btTrustee.EnableWindow(FALSE);
			}


			//_DebugOutPut(TEXT("isLookonMode : %d  gamestate:%d"), IsLookonMode(), m_bGameStatus[i]);

			return true ;
		}
	case GS_WK_PLAYING:		//游戏状态
		{
			//关定时器
			m_GameClientView.StopDispatchCard();

			CMD_S_StatusPlay *pStatusPlay = (CMD_S_StatusPlay *)pData;

			m_wBanker = pStatusPlay->wBanker ;
			m_GameClientView.SetBanker(SwitchViewChairID(m_wBanker)) ;

			//时间定义
			m_cbTimeStartGame = pStatusPlay->cbTimeStartGame;
			m_cbTimeCallBanker = pStatusPlay->cbTimeCallBanker;
			m_cbTimeSetChip = pStatusPlay->cbTimeSetChip;
			m_cbTimeRangeCard = pStatusPlay->cbTimeRangeCard;
			m_cbTimeShowCard = pStatusPlay->cbTimeShowCard;

			//历史成绩
			for (WORD i = 0; i < GAME_PLAYER; i++)
			{
				//获取变量
				tagHistoryScore * pHistoryScore = m_HistroyScore.GetHistoryScore(i);

				//设置积分
				pHistoryScore->lTurnScore = pStatusPlay->lTurnScore[i];
				pHistoryScore->lCollectScore = pStatusPlay->lCollectScore[i];

				//绑定设置
				WORD wViewChairID=SwitchViewChairID(i);
				m_GameClientView.SetHistoryScore(wViewChairID, pHistoryScore);
			}

			//最高下注
			m_GameClientView.SetMaxChip(m_wMaxChip) ;
			UpdateChipButtonState(SW_HIDE, FALSE) ;

			bool bFinishiShowCard = true ;
			
			//完成摊牌
			for(WORD wID = 0 ; wID<GAME_PLAYER ; ++wID)
			{
				IClientUserItem * pClientUserItem=GetTableUserItem(wID);
				if(pClientUserItem) 
				{
					if(false==pStatusPlay->bFinishSegment[wID])
					{
						bFinishiShowCard = false ;
						break ;
					}
				}
			}
	
			//设置状态
			CopyMemory(m_bGameStatus,pStatusPlay->bGameStatus,sizeof(m_bGameStatus));			
			for(BYTE i=0;i<GAME_PLAYER;i++)
			{
				m_GameClientView.SetGameStatus(SwitchViewChairID(i),m_bGameStatus[i]);
			}

			CopyMemory(m_bHandCardData[GetMeChairID()] , pStatusPlay->bHandCardData , sizeof(m_bHandCardData[GetMeChairID()]));
			CopyMemory(m_bTmpHandCardData , m_bHandCardData[GetMeChairID()] , sizeof(m_bTmpHandCardData));
			m_bHandCardCount = HAND_CARD_COUNT;
			CopyMemory(m_bSegmentCard , pStatusPlay->bSegmentCard , sizeof(m_bSegmentCard));
			if(false==pStatusPlay->bFinishSegment[GetMeChairID()] && m_bGameStatus[GetMeChairID()] != false)	
			{
				m_GameClientView.SetCardData(2, m_bHandCardData[GetMeChairID()], m_bHandCardCount, enCCTHandCard);
			}

			if((false==IsLookonMode()) && (true==m_bGameStatus[GetMeChairID()]))
			{
				//计算提示
				if (!pStatusPlay->bFinishSegment[GetMeChairID()])
				{
					UpdateButtonState();
				}
				
				//设置扑克
				m_GameClientView.m_UserCardControl[2].SetDisplayFlag(true) ;
				m_GameClientView.m_UserCardControl[2].SetPositively(true) ;

				if(true==pStatusPlay->bFinishSegment[GetMeChairID()])
				{
					UpdateBlockButton(0, SW_HIDE, FALSE);
				}
				else
				{
					UpdateBlockButton(0, SW_SHOW, FALSE);
				}
			}
			else
			{
				//不许托管
				m_GameClientView.m_btTrustee.EnableWindow(FALSE) ;

				//设置扑克
				m_GameClientView.m_UserCardControl[2].SetDisplayFlag(false) ;
				m_GameClientView.m_UserCardControl[2].SetPositively(false) ;
				UpdateBlockButton(0, SW_HIDE, FALSE);	
			}

			//没有摊牌
			for(WORD wID = 0; wID<GAME_PLAYER; ++wID)
			{
				WORD wViewChairID = SwitchViewChairID(wID) ;
				IClientUserItem * pClientUserItem=GetTableUserItem(wID);

				if(NULL!=pClientUserItem)
				{
					WORD wMeChairID = GetMeChairID();

					//摊牌扑克
					if (true==pStatusPlay->bFinishSegment[wID] && m_bGameStatus[wID] != false)
					{
						if (wID != wMeChairID || IsLookonMode())
						{
							BYTE bZeroCard[5]={0};
							ZeroMemory(bZeroCard, sizeof(bZeroCard));
							m_GameClientView.m_AllSegCardControl[wViewChairID].SetCardData(0, bZeroCard, 3);
							m_GameClientView.m_AllSegCardControl[wViewChairID].SetCardData(1, bZeroCard, 5);
							m_GameClientView.m_AllSegCardControl[wViewChairID].SetCardData(2, bZeroCard, 5);
							m_GameClientView.m_AllSegCardControl[wViewChairID].SetDisplayFlag(false);
						}
						else
						{
							m_GameClientView.m_AllSegCardControl[wViewChairID].SetCardData(0, m_bSegmentCard[wID][0], 3);
							m_GameClientView.m_AllSegCardControl[wViewChairID].SetCardData(1, m_bSegmentCard[wID][1], 5);
							m_GameClientView.m_AllSegCardControl[wViewChairID].SetCardData(2, m_bSegmentCard[wID][2], 5);
						}
					}
					//手上扑克
					else if (false==pStatusPlay->bFinishSegment[wID] && m_bGameStatus[wID] != false)
					{
						if (wID == wMeChairID)
							m_GameClientView.SetCardData(2, m_bHandCardData[wMeChairID], 13, enCCTHandCard);
						else
						{
							BYTE bCardData[13] = {0};
							ZeroMemory(bCardData, sizeof(bCardData));
							m_GameClientView.SetCardData(wViewChairID , bCardData , 13 , enCCTHandCard);
						}
					}
				}
			}

			CopyMemory(m_lUserChip , pStatusPlay->lChip , sizeof(m_lUserChip)) ;
			for(WORD i=0 ; i<GAME_PLAYER ; ++i) 
				m_GameClientView.SetChip(SwitchViewChairID(i) , m_lUserChip[i]) ;

			if(true==bFinishiShowCard)
			{
				KillTimer(m_nTimer) ;
				m_GameClientView.m_btTrustee.EnableWindow(FALSE) ;

				//模拟消息
				CMD_S_CompareCard compareCard;
				ZeroMemory(&compareCard, sizeof(compareCard));

				compareCard.wBankUser = m_wBanker;
				CopyMemory(compareCard.bSegmentCard, pStatusPlay->bSegmentCard, sizeof(compareCard.bSegmentCard));
				CopyMemory(compareCard.lScoreTimes, pStatusPlay->lScoreTimes, sizeof(compareCard.lScoreTimes));
				CopyMemory(compareCard.bSpecialType, pStatusPlay->bSpecialType, sizeof(compareCard.bSpecialType));
				compareCard.bCompared = pStatusPlay->bCompared;
				OnSubCompareCard((void*)&compareCard, sizeof(compareCard)) ;

				

			}
			//设置按钮
			if(false==IsLookonMode() && m_bGameStatus[GetMeChairID()] != false)
			{
				if(-1==m_lUserChip[GetMeChairID()] && GetMeChairID()!=m_wBanker)
				{
					//下注大小
					SCORE lChipsArray[SET_CHIP_COUNT] = {0};
					CopyMemory(lChipsArray, pStatusPlay->lChipArray[GetMeChairID()], sizeof(lChipsArray));

					m_GameClientView.SetChipArray( lChipsArray ) ;
                   
					UpdateChipButtonState(SW_SHOW, TRUE) ;
					m_GameClientView.SetGlobalClock(true);
					m_GameClientView.SetElapseTime(m_cbTimeRangeCard);
					m_TimerCount = m_cbTimeRangeCard;
					
					m_GameClientView.ShowCenterControl(true);
					//设置时间
					SetTimer(IDI_ALL_RANGE_CARD, 1000, NULL);
					//SetGameClock(GetMeChairID() , IDI_RANGE_CARD , m_cbTimeRangeCard) ;
				}
				else if(false==pStatusPlay->bFinishSegment[GetMeChairID()])
				{
					UpdateBlockButton(0, SW_SHOW, FALSE);
					m_GameClientView.SetGlobalClock(true);
					m_GameClientView.SetElapseTime(m_cbTimeRangeCard);
					m_TimerCount = m_cbTimeRangeCard;
					
					m_GameClientView.ShowCenterControl(true);
					//设置时间
					SetTimer(IDI_ALL_RANGE_CARD, 1000, NULL);
					//SetGameClock(GetMeChairID() , IDI_RANGE_CARD , m_cbTimeRangeCard) ;
				}
			}
			return true ;
		}
		default:
			return true;
	}	

	
}

//开始按钮
LRESULT CGameClientEngine::OnStart(WPARAM wParam, LPARAM lParam)
{
	
	UNREFERENCED_PARAMETER(wParam) ;
	UNREFERENCED_PARAMETER(lParam) ;

	//停止倒计时
	KillGameClock(IDI_START_GAME);

	//设置变量
	for (WORD i=0 ; i<GAME_PLAYER ; ++i) m_lUserChip[i] = 0;
	m_bHandCardCount = 0;
	ZeroMemory(m_bHandCardData, sizeof(m_bHandCardData));
	ZeroMemory(m_bTmpHandCardData, sizeof(m_bTmpHandCardData));	
	ZeroMemory(m_bSegmentCard, sizeof(m_bSegmentCard));
	ZeroMemory(m_lScoreTimes , sizeof(m_lScoreTimes));
	m_tagSegInfo.Init();

	m_wBanker = INVALID_CHAIR;
	m_wMaxChip = 0;
	m_bFinishSegment = false;
	m_bIsShowCard = false;
	ZeroMemory(&m_SearchCardResult, sizeof(m_SearchCardResult));
	m_cbSearchTypeIndex = 0;	
	m_cbSortTypeIndex = 0;

	//停止动画	
	m_GameClientView.StopShuffleCard();
	m_GameClientView.StopSplitCard();
	m_GameClientView.StopDispatchCard();
	m_GameClientView.StopSwatCartoon();
	m_GameClientView.StopGunCartoon();

	//设置视图
	m_GameClientView.SetChip(INVALID_CHAIR , -1) ;
	m_GameClientView.SetGameScore(INVALID_CHAIR, -1);	
	m_GameClientView.SetScoreTimes(INVALID_CHAIR, MAXLONGLONG);
	m_GameClientView.SetCallBankerStatus(INVALID_CHAIR, false);
	m_GameClientView.ShowCenterControl(false);
	m_GameClientView.SetBanker(INVALID_CHAIR) ;
	m_GameClientView.SetCardTypeSpec(false,0,0);
	m_GameClientView.SetShowCardTip(false);

	//设置按钮
	m_GameClientView.m_btStart.ShowWindow(SW_HIDE);
	m_GameClientView.m_btStart.EnableWindow(FALSE);

	if (m_bTrustee == false)
	{
		m_GameClientView.m_btTrustee.EnableWindow(TRUE);

	}
	m_GameClientView.m_UserCardControl[ME_VIEW_CHAIR].SetDisplayFlag(true) ;

	UpdateBlockButton(0, SW_HIDE, FALSE);

	//设置扑克
	m_GameClientView.SetCardData(INVALID_CHAIR , NULL, 0, enCCTHandCard) ;
	m_GameClientView.SetCardData(INVALID_CHAIR , NULL, 0, enCCTAllSegmentCard) ;
	m_GameClientView.m_CenterCardControl.SetCardData(-1, NULL, 0);

	//发送消息
	SendUserReady(NULL,0);

	return 0;
}

//叫庄消息
LRESULT CGameClientEngine::OnCallBanker(WPARAM wParam, LPARAM lParam)
{
	//删除定时器
	KillGameClock(IDI_CALL_BANKER);

	//设置按钮
	m_GameClientView.m_btNoCallBanker.ShowWindow(SW_HIDE);
	m_GameClientView.m_btCallBanker.ShowWindow(SW_HIDE);
	
	//发送消息
	SendSocketData(SUB_C_CALL_BANKER);

	return true ;
}

//不叫消息
LRESULT CGameClientEngine::OnNoCallBanker(WPARAM wParam, LPARAM lParam)
{
	//删除定时器
	KillGameClock(IDI_CALL_BANKER);

	//设置按钮
	m_GameClientView.m_btNoCallBanker.ShowWindow(SW_HIDE);
	m_GameClientView.m_btCallBanker.ShowWindow(SW_HIDE);

	//发送消息
	SendSocketData(SUB_C_NO_CALL_BANKER);

	return true ;
}

//压注消息
LRESULT CGameClientEngine::OnSetChip(WPARAM wParam, LPARAM lParam)
{
	KillGameClock(IDI_SETCHIP) ;

	CMD_C_SetChip  SetChip ;
	ZeroMemory(&SetChip, sizeof(SetChip)) ;
	SetChip.nChip = m_GameClientView.GetChipArrayItem((int)wParam);

	if(-1==SetChip.nChip)
	{
		SetChip.nChip = 100;
	}

	//发送消息
	SendSocketData(SUB_C_SETCHIP , &SetChip , sizeof(SetChip)) ;

	//更新按钮
	UpdateChipButtonState(SW_HIDE, FALSE) ;

	//播放声音
	PlayGameSound(AfxGetInstanceHandle(),TEXT("ADD_SCORE"));

	return true ;
}

//切牌消息
LRESULT CGameClientEngine::OnSplitCard(WPARAM wParam, LPARAM lParam)
{	
	//请求切牌
	SendSocketData(SUB_C_SPLIT_CARD);

	//禁用切牌
	//m_GameClientView.m_btSplitCard.EnableWindow(FALSE);

	return 0L;
}

//切牌完毕
LRESULT CGameClientEngine::OnSplitFinish(WPARAM wParam, LPARAM lParam)
{
	//发送消息
	SendSocketData(SUB_C_SPLIT_FINISH);
	return 0L;
}

//洗牌完毕
LRESULT CGameClientEngine::OnShuffleFinish(WPARAM wParam, LPARAM lParam)
{
	//发送消息
	SendSocketData(SUB_C_SHUFFLE_FINISH);

	return 0L;
}

//发牌完成
LRESULT CGameClientEngine::OnDispatchCardFinish(WPARAM wParam, LPARAM lParam)
{
	//过虑用户
	WORD wMeChairID=GetMeChairID();

	m_GameLogic.SortCardList(m_bHandCardData[wMeChairID], m_bHandCardCount, enDescend);
	m_GameClientView.SetCardData(ME_VIEW_CHAIR, m_bHandCardData[wMeChairID], m_bHandCardCount, enCCTHandCard);

	//闪动玩家
	if ((false == IsLookonMode()) && (false == m_bTrustee))
		ActiveGameFrame();

	if(!m_bCheatRight)
	{
		//界面清理
		m_GameClientView.SetCardData(INVALID_CHAIR, NULL, 0, enCCTAllSegmentCard);
	}
	else
	{
		m_GameClientView.SetCardData(SwitchViewChairID(wMeChairID), NULL, 0, enCCTAllSegmentCard);
	}
	

	//设置扑克
	if (!IsLookonMode())
	{
		m_GameClientView.m_UserCardControl[ME_VIEW_CHAIR].SetPositively(true);
		m_GameClientView.m_UserCardControl[ME_VIEW_CHAIR].ShootAllCard(false);
	}
	else
	{
		m_GameClientView.m_UserCardControl[ME_VIEW_CHAIR].SetPositively(false);
	
	}
	

	if (false == IsLookonMode() &&(m_bGameStatus[wMeChairID] != false))
	{
		//特殊牌提示框
		BYTE cbCardType = m_GameLogic.GetSpecialType(m_bHandCardData[wMeChairID], m_bHandCardCount);
		
		if (cbCardType > CT_EX_INVALID) 
		{
			if ((cbCardType==CT_EX_ZHIZUNQINGLONG)||(cbCardType==CT_EX_YITIAOLONG))
			{
				OnMsgInfo(1,0);
				m_GameClientView.SetCardTypeSpec(true, ME_VIEW_CHAIR,cbCardType);
				SetTimer(IDI_ALL_RANGE_CARD, 1000, NULL);
				return 0;
			}
			else
			{			
				CString szCardName, szTipMsg;
				switch (cbCardType)
				{
					case CT_EX_LIUDUIBAN:		szCardName=TEXT("六对半"); break;
					case CT_EX_SANSHUNZI:		szCardName=TEXT("三顺子"); break;
					case CT_EX_SANTONGHUA:		szCardName=TEXT("三同花");break;	
				}					
				
				szTipMsg.Format(TEXT("您的牌型符合【%s】,\n您是否要打此牌型？"),szCardName);
				m_GameClientView.m_MsgBox.ShowMessage(szTipMsg);
				return 0;
			}
		}
	}

	//设置按钮
	if ((IsLookonMode()==false)&&(m_bGameStatus[wMeChairID] != false))
	{		
		//更新按钮
		UpdateButtonState();
		UpdateBlockButton(0, SW_SHOW, FALSE);	

		//启用排序
		m_GameClientView.m_btSortCard.EnableWindow(TRUE);
		//启用切牌
		//m_GameClientView.m_btSplitCard.EnableWindow(TRUE);
		//显示理牌框
		m_GameClientView.ShowCenterControl(true);

		//开始计时
		//SetGameClock(wMeChairID, IDI_RANGE_CARD, m_cbTimeRangeCard);

		
		m_GameClientView.SetGlobalClock(true);
		m_GameClientView.SetElapseTime(m_cbTimeRangeCard);
		m_TimerCount = m_cbTimeRangeCard;
		SetTimer(IDI_ALL_RANGE_CARD, 1000, NULL);
	}
	
	return 0;
}


//提示消息
LRESULT CGameClientEngine::OnPrompt(WPARAM wParam, LPARAM lParam)
{
	BYTE cbShootCount = 0;
	BYTE cbShootCard[HAND_CARD_COUNT] = {0};
	
	//还原弹起
	m_GameClientView.m_UserCardControl[ME_VIEW_CHAIR].ShootAllCard(false);
	
	UINT nSearchCmd = (UINT)wParam;
	//排序扑克
	switch (nSearchCmd)
	{
	case 0:		//一对
	case 1:		//两对		
		{
				//搜索对子
			m_GameLogic.SearchSameCard(m_bHandCardData[GetMeChairID()], m_bHandCardCount, 2, &m_SearchCardResult);

			if(m_SearchCardResult.cbSearchCount >= (nSearchCmd+1))
			{
				cbShootCount = 2*(nSearchCmd+1);
				for(UINT i = 0; i < nSearchCmd+1; i++)
				{
					CopyMemory(&cbShootCard[2*i], &m_SearchCardResult.cbResultCard[(m_cbSearchTypeIndex+i)%m_SearchCardResult.cbSearchCount], sizeof(BYTE)*2);
				}
			}
			break;
		}
	case 2:		//三条
		{
				//搜索三条
			m_GameLogic.SearchSameCard(m_bHandCardData[GetMeChairID()], m_bHandCardCount, 3, &m_SearchCardResult);

			if(m_SearchCardResult.cbSearchCount >= 1)
			{
				cbShootCount = m_SearchCardResult.cbCardCount[m_cbSearchTypeIndex];
				CopyMemory(cbShootCard, &m_SearchCardResult.cbResultCard[m_cbSearchTypeIndex], sizeof(BYTE)*cbShootCount);
			}
			break;
		}
	case 3:		//顺子
		{	
			m_GameLogic.SearchLineCardType(m_bHandCardData[GetMeChairID()], m_bHandCardCount, 5, &m_SearchCardResult);

			if (m_SearchCardResult.cbSearchCount > 0)
			{
				cbShootCount = m_SearchCardResult.cbCardCount[m_cbSearchTypeIndex];
				CopyMemory(cbShootCard, m_SearchCardResult.cbResultCard[m_cbSearchTypeIndex], sizeof(BYTE)*cbShootCount);
			}

			break;
		}
	case 4:		//同花
		{
			m_GameLogic.SearchSameColorType(m_bHandCardData[GetMeChairID()], m_bHandCardCount, 5, &m_SearchCardResult);
			
			if (m_SearchCardResult.cbSearchCount > 0)
			{
				cbShootCount = m_SearchCardResult.cbCardCount[m_cbSearchTypeIndex];
				CopyMemory(cbShootCard, m_SearchCardResult.cbResultCard[m_cbSearchTypeIndex], sizeof(BYTE)*cbShootCount);
			}

			break;
		}
	case 5:		//葫芦. (三条加一对)
		{
			m_GameLogic.SearchTakeCardType(m_bHandCardData[GetMeChairID()], m_bHandCardCount, 3, 2, &m_SearchCardResult);

			if(m_SearchCardResult.cbSearchCount > 0)
			{
				cbShootCount = m_SearchCardResult.cbCardCount[m_cbSearchTypeIndex];
				CopyMemory(cbShootCard, m_SearchCardResult.cbResultCard[m_cbSearchTypeIndex], sizeof(BYTE)*cbShootCount);
			}
			break;
		}
	case 6:		//铁支 （四条加一个）
		{
			m_GameLogic.SearchTakeCardType(m_bHandCardData[GetMeChairID()], m_bHandCardCount, 4, 1, &m_SearchCardResult);

			if(m_SearchCardResult.cbSearchCount > 0)
			{
				cbShootCount = m_SearchCardResult.cbCardCount[m_cbSearchTypeIndex];
				CopyMemory(cbShootCard, m_SearchCardResult.cbResultCard[m_cbSearchTypeIndex], sizeof(BYTE)*cbShootCount);
			}
			break;
		}
	case 7:		//同花顺
		{
			m_GameLogic.SearchSameColorLineType(m_bHandCardData[GetMeChairID()], m_bHandCardCount, 5, &m_SearchCardResult);

			if(m_SearchCardResult.cbSearchCount > 0)
			{
				cbShootCount = m_SearchCardResult.cbCardCount[m_cbSearchTypeIndex];
				CopyMemory(cbShootCard, m_SearchCardResult.cbResultCard[m_cbSearchTypeIndex], sizeof(BYTE)*cbShootCount);
			}
			break;
		}
	}

	//弹起结果
	m_GameClientView.m_UserCardControl[ME_VIEW_CHAIR].SetShootCard(cbShootCard, cbShootCount);

	//切换结果索引
	if (m_SearchCardResult.cbSearchCount > 0)
		m_cbSearchTypeIndex = (m_cbSearchTypeIndex+1)%m_SearchCardResult.cbSearchCount;
	
	//更新按钮
	if ((3 != cbShootCount) && (5 != cbShootCount))
	{
		UpdateBlockButton(0, SW_SHOW, FALSE);
	}

	if (3 == cbShootCount)
	{
		UpdateBlockButton(1, SW_SHOW, TRUE);
	}

	if (5 == cbShootCount)
	{
		if (m_GameClientView.m_CenterCardControl.GetCardCount(1) == 0)
			UpdateBlockButton(2, SW_SHOW, TRUE);
		else
			UpdateBlockButton(2, SW_SHOW, FALSE);

		if (m_GameClientView.m_CenterCardControl.GetCardCount(2) == 0)
			UpdateBlockButton(3, SW_SHOW, TRUE);
		else
			UpdateBlockButton(3, SW_SHOW, FALSE);
	}

	//刷新界面
	m_GameClientView.RefreshGameView();

	return 0L;
}

//语音消息
LRESULT CGameClientEngine::OnVoicePhrase(WPARAM wParam, LPARAM lParam)
{
	if (false == CGlobalUnits::GetInstance()->m_bAllowSound)
	{
		m_pIStringMessage->InsertSystemString(TEXT("语音功能未启用！"));		
		return 0L;
	}

	if (false == m_bCanPlayVoice) 
	{
		m_pIStringMessage->InsertSystemString(TEXT("您说话太快了!请喝杯茶慢慢聊！"));
		return 0L;
	}
	else
	{
		//发送数据
		CMD_C_ChoseYuYin ChoseYuYin;
		ChoseYuYin.bType=(BYTE)wParam;
		ChoseYuYin.wChairID=GetMeChairID();

		IClientUserItem *pIClientUserItem=GetTableUserItem(GetMeChairID());
		ChoseYuYin.bYuYinIndex=RandYuYin(0,pIClientUserItem->GetGender());

		m_bCanPlayVoice = false;
		SetTimer(IDI_CAN_PLAY_VOICE, 3600, NULL);

		//发送消息
		SendSocketData(SUB_C_VOICE_PHRASE, &ChoseYuYin, sizeof(ChoseYuYin));
	}


	return 0L;
}

//排序消息
LRESULT	CGameClientEngine::OnSortCard(WPARAM wParam, LPARAM lParam) 
{
	if (m_bHandCardCount <= 0) return 0L;

	//变量定义
	enSortCardType enSCT;

	if (0 == m_cbSortTypeIndex) enSCT = enAscend;
	if (1 == m_cbSortTypeIndex) enSCT = enDescend;
	if (2 == m_cbSortTypeIndex) enSCT = enColor;

	m_GameLogic.SortCardList(m_bHandCardData[GetMeChairID()], m_bHandCardCount, enSCT);
	m_GameClientView.SetCardData(ME_VIEW_CHAIR, m_bHandCardData[GetMeChairID()], m_bHandCardCount, enCCTHandCard);

	m_cbSortTypeIndex = (m_cbSortTypeIndex+1)%3;

	return true ;
}

//托管消息
LRESULT CGameClientEngine::OnTrustee(WPARAM wParam, LPARAM lParam)
{	
	m_bTrustee = !m_bTrustee;

	CMD_C_Trustee TrusteeControl;
	ZeroMemory(&TrusteeControl, sizeof(TrusteeControl));
	TrusteeControl.bTrustee = m_bTrustee;
	SendSocketData(SUB_C_TRUSTEE, &TrusteeControl, sizeof(TrusteeControl));

	//设置控件
	m_GameClientView.m_btTrustee.SetButtonImage((true==m_bTrustee)?IDB_BT_TRUSTEE_CANCEL:IDB_BT_TRUSTEE_OPEN, AfxGetInstanceHandle(), false, false);
	
	if (m_bTrustee && m_pIStringMessage)
		m_pIStringMessage->InsertSystemString(TEXT("您选择了托管功能！"));
	if (!m_bTrustee && m_pIStringMessage)
		m_pIStringMessage->InsertSystemString(TEXT("您取消了托管功能！"));
	
	return true ;
}

//第一注消息
LRESULT CGameClientEngine::OnFrontCard(WPARAM wParam, LPARAM lParam) 
{
	//无用参数
	UNREFERENCED_PARAMETER(wParam) ;
	UNREFERENCED_PARAMETER(lParam) ;

	CMD_C_SegCard  SegCard ;
	ZeroMemory(&SegCard, sizeof(SegCard)) ;

	//获取扑克
	BYTE bCardData[HAND_CARD_COUNT];
	BYTE bShootCount=(BYTE)m_GameClientView.m_UserCardControl[ME_VIEW_CHAIR].GetShootCard(bCardData, HAND_CARD_COUNT);

	
	//数目验证
	ASSERT(3==bShootCount);
	if(3!=bShootCount) 
	{
		AfxMessageBox(TEXT("错误扑克,请重新选择!")) ;
		m_GameClientView.m_UserCardControl[ME_VIEW_CHAIR].ShootAllCard(false) ;
		return true ;
	}

	//删除扑克
	m_GameLogic.SortCardList(bCardData, bShootCount, enDescend) ;
	m_GameLogic.RemoveCard(bCardData, bShootCount, m_bHandCardData[GetMeChairID()], m_bHandCardCount);
	m_bHandCardCount -= bShootCount;

	m_tagSegInfo.SetSegData(0, bCardData, bShootCount);
	
	//发送消息
	SegCard.SegmentType=enSegFront ;
	SendSocketData(SUB_C_SEGCARD , &SegCard , sizeof(SegCard)) ;

	if(m_bHandCardCount > 0)
	{
		//排列扑克
		m_GameLogic.SortCardList(m_bHandCardData[GetMeChairID()], m_bHandCardCount, enDescend);
	}
	else if(m_bHandCardCount == 0)
	{
		//防止乌龙
		if((enCRGreater == m_GameLogic.CompareCard(m_tagSegInfo.bSegData[0] , m_tagSegInfo.bSegData[1] , 3 , 5 , false)) &&
			(enCRGreater == m_GameLogic.CompareCard(m_tagSegInfo.bSegData[1] , m_tagSegInfo.bSegData[2] , 5 , 5 , true)))
		{
			//满足条件( 第三注 > 第二注 > 第一注)

			//允许开牌
			m_GameClientView.m_btShowCard.ShowWindow(SW_SHOW);
			m_GameClientView.m_btShowCard.EnableWindow(TRUE);		


			//关闭提示按钮
			//禁用提示
			for (UINT i = 0; i < 8; i++)
				m_GameClientView.m_btPrompt[i].EnableWindow(FALSE);

			//开牌倒计时
			//	KillGameClock(IDI_RANGE_CARD);
			// 			SetGameClock(GetMeChairID(), IDI_SHOW_CARD, m_cbTimeShowCard);


		}
		else
		{
			//关闭提示按钮
			//禁用提示
			for (UINT i = 0; i < 8; i++)
				m_GameClientView.m_btPrompt[i].EnableWindow(FALSE);

			//不满足条件( 第三注 > 第二注 > 第一注)
			m_GameClientView.SetShowCardTip(true);
			m_GameClientView.RefreshGameView();
			return true;
		}
	}

	//设置界面	
	m_GameClientView.SetCardData(ME_VIEW_CHAIR, m_bHandCardData[GetMeChairID()] , m_bHandCardCount , enCCTHandCard) ;
	m_GameClientView.m_CenterCardControl.SetCardData(0, bCardData, bShootCount);
	m_GameClientView.m_CenterCardControl.ShowWindow(SW_SHOW, 0);
	m_GameClientView.m_btRestore[0].ShowWindow(SW_SHOW);
	m_GameClientView.m_btRestore[0].EnableWindow(TRUE);
	m_GameClientView.m_btFrontCard.ShowWindow(SW_SHOW);
	m_GameClientView.m_btFrontCard.EnableWindow(FALSE);

	if(3==m_bHandCardCount || 5==m_bHandCardCount)
	{
		m_bFinishSegment=true ; 

		ASSERT(m_tagSegInfo.bHaveSeg[1] != m_tagSegInfo.bHaveSeg[2]);
		m_tagSegInfo.SetSegData((m_tagSegInfo.bHaveSeg[1]?2:1), m_bHandCardData[GetMeChairID()], m_bHandCardCount);

		//清空手牌
		m_bHandCardCount = 0;
		ZeroMemory(m_bHandCardData[GetMeChairID()], sizeof(m_bHandCardData[GetMeChairID()]));
		
		if(true==IsLookonMode())
		{
			m_GameClientView.m_CenterCardControl.SetDisplayFlag(false) ;
		}

		//设置扑克
		m_GameClientView.SetCardData(ME_VIEW_CHAIR, NULL, 0, enCCTHandCard);
		m_GameClientView.m_CenterCardControl.SetCardData(0, m_tagSegInfo.bSegData[0] , 3);
		m_GameClientView.m_CenterCardControl.SetCardData(1, m_tagSegInfo.bSegData[1] , 5);
		m_GameClientView.m_CenterCardControl.SetCardData(2, m_tagSegInfo.bSegData[2] , 5);
		m_GameClientView.m_CenterCardControl.ShowWindow(SW_SHOW);

		//恢复按钮
		m_GameClientView.m_btRestore[0].ShowWindow(SW_SHOW);
		m_GameClientView.m_btRestore[0].EnableWindow(TRUE);
		m_GameClientView.m_btRestore[1].ShowWindow(SW_SHOW);
		m_GameClientView.m_btRestore[1].EnableWindow(TRUE);
		m_GameClientView.m_btRestore[2].ShowWindow(SW_SHOW);
		m_GameClientView.m_btRestore[2].EnableWindow(TRUE);
		m_GameClientView.m_btRestore[3].ShowWindow(SW_SHOW);
		m_GameClientView.m_btRestore[3].EnableWindow(TRUE);

		//隐藏按钮
		UpdateBlockButton(0, SW_SHOW, FALSE);
		
		//防止乌龙
		if((enCRGreater == m_GameLogic.CompareCard(m_tagSegInfo.bSegData[0] , m_tagSegInfo.bSegData[1] , 3 , 5 , false)) &&
			(enCRGreater == m_GameLogic.CompareCard(m_tagSegInfo.bSegData[1] , m_tagSegInfo.bSegData[2] , 5 , 5 , true)))
		{
			//满足条件( 第三注 > 第二注 > 第一注)

			//允许开牌
			m_GameClientView.m_btShowCard.ShowWindow(SW_SHOW);
			m_GameClientView.m_btShowCard.EnableWindow(TRUE);		


			//关闭提示按钮
			//禁用提示
			for (UINT i = 0; i < 8; i++)
				m_GameClientView.m_btPrompt[i].EnableWindow(FALSE);

			//开牌倒计时
		//	KillGameClock(IDI_RANGE_CARD);
// 			SetGameClock(GetMeChairID(), IDI_SHOW_CARD, m_cbTimeShowCard);

			
		}
		else
		{
			//关闭提示按钮
			//禁用提示
			for (UINT i = 0; i < 8; i++)
				m_GameClientView.m_btPrompt[i].EnableWindow(FALSE);

			//不满足条件( 第三注 > 第二注 > 第一注)
			m_GameClientView.SetShowCardTip(true);
			m_GameClientView.RefreshGameView();
			return true;
		}
	}

	//设置按钮
	UpdateButtonState();
	//更新界面
	m_GameClientView.RefreshGameView();

	//播放声音
	PlayGameSound(AfxGetInstanceHandle(),TEXT("OUT_CARD"));

	return true ;
}

//第二注消息
LRESULT CGameClientEngine::OnMidCard(WPARAM wParam, LPARAM lParam)
{
	//无用参数
	UNREFERENCED_PARAMETER(wParam) ;
	UNREFERENCED_PARAMETER(lParam) ;

	CMD_C_SegCard  SegCard ;
	ZeroMemory(&SegCard, sizeof(SegCard)) ;

	//获取扑克
	BYTE bCardData[HAND_CARD_COUNT];
	BYTE bShootCount=(BYTE)m_GameClientView.m_UserCardControl[ME_VIEW_CHAIR].GetShootCard(bCardData,HAND_CARD_COUNT);

	//验证数目
	ASSERT(5==bShootCount);
	if(5!=bShootCount) 
	{
		AfxMessageBox(TEXT("错误扑克,请重新选择!")) ;
		m_GameClientView.m_UserCardControl[ME_VIEW_CHAIR].ShootAllCard(false) ;
		return true ;
	}

	//删除扑克
	m_GameLogic.SortCardList(bCardData, bShootCount, enDescend) ;
	m_GameLogic.RemoveCard(bCardData, bShootCount, m_bHandCardData[GetMeChairID()], m_bHandCardCount);
	m_bHandCardCount -= bShootCount;
	
	m_tagSegInfo.SetSegData(1, bCardData, bShootCount);
	
	//发送消息
	SegCard.SegmentType=enSegMid;
	SendSocketData(SUB_C_SEGCARD , &SegCard , sizeof(SegCard)) ;


	if(m_bHandCardCount > 0)
	{
		//排列扑克
		m_GameLogic.SortCardList(m_bHandCardData[GetMeChairID()], m_bHandCardCount, enDescend);
	}
	else if(m_bHandCardCount == 0)
	{
		//防止乌龙
		if((enCRGreater == m_GameLogic.CompareCard(m_tagSegInfo.bSegData[0] , m_tagSegInfo.bSegData[1] , 3 , 5 , false)) &&
			(enCRGreater == m_GameLogic.CompareCard(m_tagSegInfo.bSegData[1] , m_tagSegInfo.bSegData[2] , 5 , 5 , true)))
		{
			//满足条件( 第三注 > 第二注 > 第一注)

			//允许开牌
			m_GameClientView.m_btShowCard.ShowWindow(SW_SHOW);
			m_GameClientView.m_btShowCard.EnableWindow(TRUE);		


			//关闭提示按钮
			//禁用提示
			for (UINT i = 0; i < 8; i++)
				m_GameClientView.m_btPrompt[i].EnableWindow(FALSE);

			//开牌倒计时
			//	KillGameClock(IDI_RANGE_CARD);
			// 			SetGameClock(GetMeChairID(), IDI_SHOW_CARD, m_cbTimeShowCard);


		}
		else
		{
			//关闭提示按钮
			//禁用提示
			for (UINT i = 0; i < 8; i++)
				m_GameClientView.m_btPrompt[i].EnableWindow(FALSE);

			//不满足条件( 第三注 > 第二注 > 第一注)
			m_GameClientView.SetShowCardTip(true);
			m_GameClientView.RefreshGameView();
			return true;
		}
	}



	//设置界面
	m_GameClientView.SetCardData(ME_VIEW_CHAIR, m_bHandCardData[GetMeChairID()], m_bHandCardCount, enCCTHandCard);
	m_GameClientView.m_CenterCardControl.SetCardData(1, bCardData, bShootCount);
	
	m_GameClientView.m_CenterCardControl.ShowWindow(SW_SHOW, 1);
	m_GameClientView.m_btRestore[1].ShowWindow(SW_SHOW);
	m_GameClientView.m_btRestore[1].EnableWindow(TRUE);
	m_GameClientView.m_btMidCard.ShowWindow(SW_SHOW);
	m_GameClientView.m_btMidCard.EnableWindow(FALSE);


	if(3==m_bHandCardCount || 5==m_bHandCardCount)
	{
		m_bFinishSegment=true ; 

		ASSERT(m_tagSegInfo.bHaveSeg[0] != m_tagSegInfo.bHaveSeg[2]);
		m_tagSegInfo.SetSegData((m_tagSegInfo.bHaveSeg[0]?2:0), m_bHandCardData[GetMeChairID()], m_bHandCardCount);

		//清空手牌
		m_bHandCardCount = 0;
		ZeroMemory(m_bHandCardData[GetMeChairID()], sizeof(m_bHandCardData[GetMeChairID()]));
		
		if(true==IsLookonMode())
		{
			m_GameClientView.m_CenterCardControl.SetDisplayFlag(false) ;
		}
		
		//设置扑克
		m_GameClientView.SetCardData(ME_VIEW_CHAIR, NULL, 0, enCCTHandCard);
		m_GameClientView.m_CenterCardControl.SetCardData(0, m_tagSegInfo.bSegData[0] , 3);
		m_GameClientView.m_CenterCardControl.SetCardData(1, m_tagSegInfo.bSegData[1] , 5);
		m_GameClientView.m_CenterCardControl.SetCardData(2, m_tagSegInfo.bSegData[2] , 5);
		m_GameClientView.m_CenterCardControl.ShowWindow(SW_SHOW) ;



		//恢复回收按钮
		m_GameClientView.m_btRestore[0].ShowWindow(SW_SHOW);
		m_GameClientView.m_btRestore[0].EnableWindow(TRUE);
		m_GameClientView.m_btRestore[1].ShowWindow(SW_SHOW);
		m_GameClientView.m_btRestore[1].EnableWindow(TRUE);
		m_GameClientView.m_btRestore[2].ShowWindow(SW_SHOW);
		m_GameClientView.m_btRestore[2].EnableWindow(TRUE);
		m_GameClientView.m_btRestore[3].ShowWindow(SW_SHOW);
		m_GameClientView.m_btRestore[3].EnableWindow(TRUE);			
		
		//隐藏按钮
		UpdateBlockButton(0, SW_SHOW, FALSE);

		//防止乌龙
		if (enCRGreater == m_GameLogic.CompareCard(m_tagSegInfo.bSegData[0], m_tagSegInfo.bSegData[1], 3, 5, false) &&
			enCRGreater == m_GameLogic.CompareCard(m_tagSegInfo.bSegData[1], m_tagSegInfo.bSegData[2], 5, 5, true)) 
		{
			//设置按钮
			
			//允许开牌
			m_GameClientView.m_btShowCard.ShowWindow(SW_SHOW) ;
			m_GameClientView.m_btShowCard.EnableWindow(TRUE) ;

			//关闭提示按钮
			//禁用提示
			for (UINT i = 0; i < 8; i++)
				m_GameClientView.m_btPrompt[i].EnableWindow(FALSE);

			//KillGameClock(IDI_RANGE_CARD);
			//SetGameClock(GetMeChairID(), IDI_SHOW_CARD, m_cbTimeShowCard);
		}
		else
		{
			//关闭提示按钮
			//禁用提示
			for (UINT i = 0; i < 8; i++)
				m_GameClientView.m_btPrompt[i].EnableWindow(FALSE);

			m_GameClientView.SetShowCardTip(true);
			m_GameClientView.RefreshGameView();



			return true;
		}
	}

	//设置按钮
	UpdateButtonState();
	m_GameClientView.m_btBackCard.EnableWindow(FALSE);

	//更新界面
	m_GameClientView.RefreshGameView();

	//播放声音
	PlayGameSound(AfxGetInstanceHandle(),TEXT("OUT_CARD"));

	return true ;
}

//第三注消息
LRESULT CGameClientEngine::OnBackCard(WPARAM wParam, LPARAM lParam)
{
	//无用参数
	UNREFERENCED_PARAMETER(wParam) ;
	UNREFERENCED_PARAMETER(lParam) ;

	CMD_C_SegCard  SegCard ;
	ZeroMemory(&SegCard, sizeof(SegCard)) ;

	//获取扑克
	BYTE bCardData[HAND_CARD_COUNT];
	BYTE bShootCount=(BYTE)m_GameClientView.m_UserCardControl[ME_VIEW_CHAIR].GetShootCard(bCardData,HAND_CARD_COUNT);

	//验证数目
	ASSERT(5==bShootCount) ;
	if(5!=bShootCount) 
	{
		AfxMessageBox(TEXT("错误扑克,请重新选择!")) ;
		m_GameClientView.m_UserCardControl[ME_VIEW_CHAIR].ShootAllCard(false) ;
		return true ;
	}

	//删除扑克
	m_GameLogic.SortCardList(bCardData , bShootCount, enDescend) ;
	m_GameLogic.RemoveCard(bCardData , bShootCount , m_bHandCardData[GetMeChairID()] , 	m_bHandCardCount) ;
	m_bHandCardCount -= bShootCount ;

	m_tagSegInfo.SetSegData(2, bCardData, bShootCount);
	
	//发送消息
	SegCard.SegmentType=enSegBack ;
	SendSocketData(SUB_C_SEGCARD , &SegCard , sizeof(SegCard)) ;
	
	if(m_bHandCardCount > 0)
	{
		//排列扑克
		m_GameLogic.SortCardList(m_bHandCardData[GetMeChairID()], m_bHandCardCount, enDescend);
	}
	else if(m_bHandCardCount == 0)
	{
		//防止乌龙
		if((enCRGreater == m_GameLogic.CompareCard(m_tagSegInfo.bSegData[0] , m_tagSegInfo.bSegData[1] , 3 , 5 , false)) &&
			(enCRGreater == m_GameLogic.CompareCard(m_tagSegInfo.bSegData[1] , m_tagSegInfo.bSegData[2] , 5 , 5 , true)))
		{
			//满足条件( 第三注 > 第二注 > 第一注)

			//允许开牌
			m_GameClientView.m_btShowCard.ShowWindow(SW_SHOW);
			m_GameClientView.m_btShowCard.EnableWindow(TRUE);		


			//关闭提示按钮
			//禁用提示
			for (UINT i = 0; i < 8; i++)
				m_GameClientView.m_btPrompt[i].EnableWindow(FALSE);

			//开牌倒计时
			//	KillGameClock(IDI_RANGE_CARD);
			// 			SetGameClock(GetMeChairID(), IDI_SHOW_CARD, m_cbTimeShowCard);


		}
		else
		{
			//关闭提示按钮
			//禁用提示
			for (UINT i = 0; i < 8; i++)
				m_GameClientView.m_btPrompt[i].EnableWindow(FALSE);

			//不满足条件( 第三注 > 第二注 > 第一注)
			m_GameClientView.SetShowCardTip(true);
			m_GameClientView.RefreshGameView();
			return true;
		}
	}

	

	//设置界面
	m_GameClientView.SetCardData(ME_VIEW_CHAIR , m_bHandCardData[GetMeChairID()] , m_bHandCardCount , enCCTHandCard) ;
	m_GameClientView.m_CenterCardControl.SetCardData(2, bCardData, bShootCount);
	m_GameClientView.m_CenterCardControl.ShowWindow(SW_SHOW, 2);
	m_GameClientView.m_btRestore[2].ShowWindow(SW_SHOW);
	m_GameClientView.m_btRestore[2].EnableWindow(TRUE);
	m_GameClientView.m_btBackCard.ShowWindow(SW_SHOW);
	m_GameClientView.m_btBackCard.EnableWindow(FALSE);

	if(3==m_bHandCardCount || 5==m_bHandCardCount)
	{
		m_bFinishSegment=true ; 

		ASSERT(m_tagSegInfo.bHaveSeg[0] != m_tagSegInfo.bHaveSeg[1]);
		m_tagSegInfo.SetSegData((m_tagSegInfo.bHaveSeg[0]?1:0), m_bHandCardData[GetMeChairID()], m_bHandCardCount);
		
		//清空手牌
		m_bHandCardCount = 0;
		ZeroMemory(m_bHandCardData[GetMeChairID()], sizeof(m_bHandCardData[GetMeChairID()]));

		if(true==IsLookonMode())
		{
			m_GameClientView.m_CenterCardControl.SetDisplayFlag(false) ;
		}

		//设置扑克
		m_GameClientView.SetCardData(ME_VIEW_CHAIR, NULL, 0, enCCTHandCard);
		m_GameClientView.m_CenterCardControl.SetCardData(0, m_tagSegInfo.bSegData[0] , 3);
		m_GameClientView.m_CenterCardControl.SetCardData(1, m_tagSegInfo.bSegData[1] , 5);
		m_GameClientView.m_CenterCardControl.SetCardData(2, m_tagSegInfo.bSegData[2] , 5);
		m_GameClientView.m_CenterCardControl.ShowWindow(SW_SHOW) ;

		//恢复回收按钮
		m_GameClientView.m_btRestore[0].ShowWindow(SW_SHOW);
		m_GameClientView.m_btRestore[0].EnableWindow(TRUE);
		m_GameClientView.m_btRestore[1].ShowWindow(SW_SHOW);
		m_GameClientView.m_btRestore[1].EnableWindow(TRUE);
		m_GameClientView.m_btRestore[2].ShowWindow(SW_SHOW);
		m_GameClientView.m_btRestore[2].EnableWindow(TRUE);
		m_GameClientView.m_btRestore[3].ShowWindow(SW_SHOW);
		m_GameClientView.m_btRestore[3].EnableWindow(TRUE);

		//隐藏按钮
		UpdateBlockButton(0, SW_SHOW, FALSE);

		//防止乌龙
		if (enCRGreater == m_GameLogic.CompareCard(m_tagSegInfo.bSegData[0], m_tagSegInfo.bSegData[1], 3, 5, false) &&
			enCRGreater == m_GameLogic.CompareCard(m_tagSegInfo.bSegData[1], m_tagSegInfo.bSegData[2], 5, 5, true))
		{
			//允许开牌
			m_GameClientView.m_btShowCard.ShowWindow(SW_SHOW);
			m_GameClientView.m_btShowCard.EnableWindow(TRUE);

			//关闭提示按钮
			//禁用提示
			for (UINT i = 0; i < 8; i++)
				m_GameClientView.m_btPrompt[i].EnableWindow(FALSE);

			//KillGameClock(IDI_RANGE_CARD);
			//SetGameClock(GetMeChairID(), IDI_SHOW_CARD, m_cbTimeShowCard);
		}
		else
		{
			//关闭提示按钮
			//禁用提示
			for (UINT i = 0; i < 8; i++)
				m_GameClientView.m_btPrompt[i].EnableWindow(FALSE);

			m_GameClientView.SetShowCardTip(true);
			m_GameClientView.RefreshGameView();


			return true;
		}
	}

	//设置按钮
	UpdateButtonState();
	m_GameClientView.m_btMidCard.EnableWindow(FALSE);

	//更新界面
	m_GameClientView.RefreshGameView();

	//播放声音
	PlayGameSound(AfxGetInstanceHandle(),TEXT("OUT_CARD"));

	return true ;
}

//玩家确定
LRESULT CGameClientEngine::OnShowCard(WPARAM wParam, LPARAM lParam) 
{
	if(m_GameClientView.GetShowCardTip())
	{
		OnRestoreCard(3,0);
		//自动代打
		AutomaticOutCard();
	}

	if (m_bIsShowCard)
	{
		return false;
	}

	//无用参数
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

	ASSERT(m_tagSegInfo.GetCardCount(0) == 3 && m_tagSegInfo.GetCardCount(1) == 5 && m_tagSegInfo.GetCardCount(2) == 5) ;
	if (m_tagSegInfo.GetCardCount(0) != 3 || m_tagSegInfo.GetCardCount(1) != 5 || m_tagSegInfo.GetCardCount(2) != 5)
		return false;
    	
	//停止计时
	//KillGameClock(IDI_RANGE_CARD);
	//KillGameClock(IDI_SHOW_CARD);

	//取消提示
	m_GameClientView.SetShowCardTip(false);

	//隐藏分段
	m_GameClientView.ShowCenterControl(false);
	m_GameClientView.m_CenterCardControl.ShowWindow(SW_HIDE) ;
	

	CMD_C_ShowCard AllSegmentCard;
	ZeroMemory(&AllSegmentCard, sizeof(AllSegmentCard));

	m_tagSegInfo.GetSegData(0, AllSegmentCard.bFrontCard, 3);
	m_GameLogic.SortCardList(AllSegmentCard.bFrontCard, 3) ;

	m_tagSegInfo.GetSegData(1, AllSegmentCard.bMidCard, 5);
	m_GameLogic.SortCardList(AllSegmentCard.bMidCard, 5);

	m_tagSegInfo.GetSegData(2, AllSegmentCard.bBackCard, 5);
	m_GameLogic.SortCardList(AllSegmentCard.bBackCard, 5);

	if(false==IsLookonMode())
		SendSocketData(SUB_C_SHOWARD, &AllSegmentCard, sizeof(AllSegmentCard)) ;

	//隐藏按钮
	m_GameClientView.m_btRestore[0].ShowWindow(SW_HIDE);
	m_GameClientView.m_btRestore[0].EnableWindow(FALSE);
	m_GameClientView.m_btRestore[1].ShowWindow(SW_HIDE);
	m_GameClientView.m_btRestore[1].EnableWindow(FALSE);
	m_GameClientView.m_btRestore[2].ShowWindow(SW_HIDE);
	m_GameClientView.m_btRestore[2].EnableWindow(FALSE);
	m_GameClientView.m_btRestore[3].ShowWindow(SW_HIDE);
	m_GameClientView.m_btRestore[3].EnableWindow(FALSE);

	BYTE bCard[13] = {0};
	CopyMemory(bCard, m_tagSegInfo.bSegData[0], sizeof(BYTE)*3);
	CopyMemory(bCard+3, m_tagSegInfo.bSegData[1], sizeof(BYTE)*10);
	m_GameClientView.m_AllSegCardControl[2].SetCardData(bCard, 13);
	m_GameClientView.m_AllSegCardControl[2].ShowWindow(SW_SHOW);

	//设置按钮
	UpdateChipButtonState(SW_HIDE, FALSE) ;
	UpdateBlockButton(0, SW_HIDE, FALSE);
	UpdateButtonState();
	m_GameClientView.m_btShowCard.ShowWindow(SW_HIDE) ;
	m_GameClientView.m_btShowCard.EnableWindow(FALSE) ;
	m_GameClientView.m_btTrustee.EnableWindow(FALSE) ;


	//更新界面
	m_GameClientView.RefreshGameView();

	//播放声音
	PlayGameSound(AfxGetInstanceHandle(),TEXT("OUT_CARD"));

	m_bIsShowCard = true;

	return true ;
}

//回收扑克
LRESULT CGameClientEngine::OnRestoreCard(WPARAM wParam, LPARAM lParam)
{
	BYTE cbIndex = (BYTE)wParam;
	
	//手牌全齐时不显示
	ASSERT(m_bHandCardCount < HAND_CARD_COUNT);
	if (m_bHandCardCount >= HAND_CARD_COUNT) return 0L;

	//取消提示
	m_GameClientView.SetShowCardTip(false);

	//全部回收
	if (cbIndex == 3)
	{
		//设置界面
		m_bFinishSegment = false;
		m_bHandCardCount = HAND_CARD_COUNT;
		CopyMemory(m_bHandCardData[GetMeChairID()], m_bTmpHandCardData, sizeof(BYTE)*m_bHandCardCount);
		m_GameLogic.SortCardList(m_bHandCardData[GetMeChairID()], m_bHandCardCount, enDescend);
		m_GameClientView.m_UserCardControl[ME_VIEW_CHAIR].SetCardData(m_bHandCardData[GetMeChairID()], m_bHandCardCount);
		m_GameClientView.m_CenterCardControl.SetCardData(-1, NULL, 0);
		m_tagSegInfo.Init();

		//更新界面
		m_GameClientView.RefreshGameView();

		//设置按钮
		m_GameClientView.m_btRestore[0].ShowWindow(SW_HIDE);
		m_GameClientView.m_btRestore[0].EnableWindow(FALSE);
		m_GameClientView.m_btRestore[1].ShowWindow(SW_HIDE);
		m_GameClientView.m_btRestore[1].EnableWindow(FALSE);
		m_GameClientView.m_btRestore[2].ShowWindow(SW_HIDE);
		m_GameClientView.m_btRestore[2].EnableWindow(FALSE);
		m_GameClientView.m_btRestore[3].ShowWindow(SW_HIDE);
		m_GameClientView.m_btRestore[3].EnableWindow(FALSE);
		m_GameClientView.m_btShowCard.ShowWindow(SW_HIDE);
		m_GameClientView.m_btShowCard.EnableWindow(FALSE);
		UpdateButtonState();
		UpdateBlockButton(0,SW_SHOW,FALSE);
	}
	//部分回收
	else
	{	
		//变量定义
		BYTE cbSegData[5] = {0};
		WORD cbSegCount = m_GameClientView.m_CenterCardControl.GetCardData(cbIndex, cbSegData, 5);

		//插入扑克
		m_bFinishSegment = false;
		CopyMemory(&m_bHandCardData[GetMeChairID()][m_bHandCardCount], cbSegData, sizeof(BYTE)*cbSegCount);
		m_bHandCardCount += cbSegCount;
		m_GameLogic.SortCardList(m_bHandCardData[GetMeChairID()], m_bHandCardCount, enDescend);
		m_GameClientView.m_UserCardControl[2].SetCardData(m_bHandCardData[GetMeChairID()], m_bHandCardCount);
		m_GameClientView.m_CenterCardControl.SetCardData(cbIndex, NULL, 0);
		m_tagSegInfo.SetSegData(cbIndex, NULL, 5);

		//更新界面
		m_GameClientView.RefreshGameView();

		m_GameClientView.m_btRestore[3].ShowWindow(SW_HIDE);
		m_GameClientView.m_btRestore[3].EnableWindow(FALSE);
		m_GameClientView.m_btRestore[cbIndex].ShowWindow(SW_HIDE);
		m_GameClientView.m_btRestore[cbIndex].EnableWindow(FALSE);
		m_GameClientView.m_btShowCard.ShowWindow(SW_HIDE);
		m_GameClientView.m_btShowCard.EnableWindow(FALSE);
		UpdateButtonState();
		UpdateBlockButton(cbIndex+1,SW_SHOW,FALSE);
	}

	return 0L;
}

//一轮翻牌结束
LRESULT CGameClientEngine::OnRevesalTurnFinish(WPARAM wParam, LPARAM lParam)
{
	BYTE cbScene = (BYTE)wParam;

	//显示输赢倍数
	for (WORD i=0; i<GAME_PLAYER; i++)
	{
		if ((GetTableUserItem(i) == NULL) || (m_bGameStatus[i] == false)) continue;

		SCORE lScoreTimes = 0L;
		if (cbScene == 0) lScoreTimes = m_lScoreTimes[i][0];
		else if (cbScene == 1) lScoreTimes = m_lScoreTimes[i][0]+m_lScoreTimes[i][1];
		else lScoreTimes = m_lScoreTimes[i][0]+m_lScoreTimes[i][1]+m_lScoreTimes[i][2];

		m_GameClientView.SetScoreTimes(SwitchViewChairID(i), lScoreTimes);
	}

	return 0L;
}

//特殊理牌
LRESULT	CGameClientEngine::OnMsgInfo(WPARAM wParam, LPARAM lParam)
{
	//关闭窗口
	m_GameClientView.m_MsgBox.ShowWindow(SW_HIDE);

	//提示框-确认
	if ((BYTE)wParam == 1)
	{
		BYTE cbSegCardData[3][5] = {0};
		if (0 != m_GameLogic.GetSpecialCardData(m_bHandCardData[GetMeChairID()], m_bHandCardCount, cbSegCardData))
		{
			m_GameClientView.m_UserCardControl[ME_VIEW_CHAIR].SetShootCard(cbSegCardData[0], 3);
			OnFrontCard(0,0);

			m_GameClientView.m_UserCardControl[ME_VIEW_CHAIR].SetShootCard(cbSegCardData[1], 5);
			OnMidCard(0,0);
		}

		//摊牌
		OnShowCard(0, 0);

		//显示提示
		if (m_GameLogic.GetDoubleCount(m_tagSegInfo.bSegData[0], m_tagSegInfo.bSegData[1], m_tagSegInfo.bSegData[2]) == 6)
		{
			m_GameClientView.SetCardTypeSpec(true, ME_VIEW_CHAIR, CT_EX_LIUDUIBAN);
		}

		if (m_GameLogic.IsLinkCard(m_tagSegInfo.bSegData[0], 3) && 
			m_GameLogic.IsLinkCard(m_tagSegInfo.bSegData[1], 5) && 
			m_GameLogic.IsLinkCard(m_tagSegInfo.bSegData[2], 5))
		{
			m_GameClientView.SetCardTypeSpec(true, ME_VIEW_CHAIR, CT_EX_SANSHUNZI);
		}

		if (m_GameLogic.IsSameColorCard(m_tagSegInfo.bSegData[0], 3) && 
			m_GameLogic.IsSameColorCard(m_tagSegInfo.bSegData[1], 5) && 
			m_GameLogic.IsSameColorCard(m_tagSegInfo.bSegData[2], 5))
		{
			m_GameClientView.SetCardTypeSpec(true, ME_VIEW_CHAIR, CT_EX_SANTONGHUA);
		}
	}
	//提示框-取消
	else
	{
		//更新按钮
		UpdateButtonState();
		UpdateBlockButton(0, SW_SHOW, FALSE);

		//启用切牌
		//m_GameClientView.m_btSplitCard.EnableWindow(TRUE);
		m_GameClientView.ShowCenterControl(true);

		//开始计时
		//SetGameClock(GetMeChairID(), IDI_RANGE_CARD, m_cbTimeRangeCard);
	}

	return 0L;
}

//左键扑克
LRESULT CGameClientEngine::OnLeftHitCard(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam) ;
	UNREFERENCED_PARAMETER(lParam) ;

	if(false==m_bFinishSegment)
	{
		//获取扑克
		BYTE bCardData[HAND_CARD_COUNT];
		BYTE bShootCount=(BYTE)m_GameClientView.m_UserCardControl[2].GetShootCard(bCardData,HAND_CARD_COUNT);

		//设置按钮
		if(5==bShootCount)
		{
			
				m_GameClientView.m_btFrontCard.EnableWindow(FALSE) ;
			if(m_GameClientView.m_CenterCardControl.GetCardCount(1) == 0)
				m_GameClientView.m_btMidCard.EnableWindow(TRUE) ;
			if(m_GameClientView.m_CenterCardControl.GetCardCount(2) == 0)
				m_GameClientView.m_btBackCard.EnableWindow(TRUE) ;
		}
		else if(3==bShootCount)
		{
			if(m_GameClientView.m_CenterCardControl.GetCardCount(0) == 0)
				m_GameClientView.m_btFrontCard.EnableWindow(TRUE) ;
		}
		else
		{
			m_GameClientView.m_btFrontCard.EnableWindow(FALSE);
			m_GameClientView.m_btMidCard.EnableWindow(FALSE);
			m_GameClientView.m_btBackCard.EnableWindow(FALSE);	
		}
	}

	return 0;
}

//开始叫庄
bool CGameClientEngine::OnSubBeginCall(const void * pBuffer, WORD wDataSize)
{
	//数据验证
	ASSERT( sizeof( CMD_S_StartCallBanker ) == wDataSize ) ;
	if ( sizeof( CMD_S_StartCallBanker ) != wDataSize ) return false ;

	CMD_S_StartCallBanker  *pStartCallBanker = ( CMD_S_StartCallBanker* )pBuffer ;

	ZeroMemory(m_bUserCheat,sizeof(m_bUserCheat));

	//取消理牌框
	m_GameClientView.ShowCenterControl(false);
	UpdateBlockButton(0, SW_HIDE, FALSE);

	//取消上次扑克
	if (IsLookonMode()==true)
	{
		for (WORD i=0 ; i<GAME_PLAYER; ++i)
			m_GameClientView.m_AllSegCardControl[i].SetCardData(-1,NULL, 0);

		m_GameClientView.SetChip(INVALID_CHAIR , -1) ;
		m_GameClientView.SetGameScore(INVALID_CHAIR, -1);	
		m_GameClientView.SetScoreTimes(INVALID_CHAIR, MAXLONGLONG);
		m_GameClientView.SetCallBankerStatus(INVALID_CHAIR, false);
		m_GameClientView.SetBanker(INVALID_CHAIR) ;
		m_GameClientView.SetCardTypeSpec(false,0,0);
	}

	//叫庄时间
	SetGameClock( pStartCallBanker->wCurrentCaller, IDI_CALL_BANKER, m_cbTimeCallBanker ) ;

	//设置按钮
	if ( GetMeChairID() == pStartCallBanker->wCurrentCaller && !IsLookonMode() )
	{
		m_GameClientView.m_btCallBanker.ShowWindow( SW_SHOW ) ;
		m_GameClientView.m_btNoCallBanker.ShowWindow( SW_SHOW ) ;
	}
	
	//提示信息
	m_GameClientView.SetTipInfoState(1);

	//闪动玩家
	if ((false == IsLookonMode())  && (false == m_bTrustee) && (GetMeChairID() == pStartCallBanker->wCurrentCaller))
		ActiveGameFrame();

	//游戏状态
	SetGameStatus(GS_WK_CALL_BANKER);
	
	//用户状态
	CopyMemory(m_bGameStatus,pStartCallBanker->bGameStatus,sizeof(m_bGameStatus));
	for(WORD i=0;i<GAME_PLAYER;i++)
	{
		m_GameClientView.SetGameStatus(SwitchViewChairID(i),m_bGameStatus[i]);
	}

	return true ;
}

//玩家叫庄
bool CGameClientEngine::OnSubUserCall(const void * pBuffer, WORD wDataSize)
{
	//数据验证
	ASSERT( sizeof( CMD_S_CallBanker ) == wDataSize ) ;
	if ( sizeof( CMD_S_CallBanker ) != wDataSize ) return false ;

	CMD_S_CallBanker *pCallBanker = ( CMD_S_CallBanker* )pBuffer ;

	//玩家叫庄
	if (pCallBanker->bCallBanker)
	{
		//设置变量
		m_wBanker = pCallBanker->wLastCaller ;
		m_wMaxChip = pCallBanker->lMaxChip;

		//删除定时器
		KillGameClock(IDI_CALL_BANKER) ;

		//设置状态
		SetGameStatus(GS_WK_SETCHIP) ;
		
		//设置界面
		m_GameClientView.SetBanker(SwitchViewChairID(pCallBanker->wLastCaller)) ;
		m_GameClientView.SetCallBankerStatus(INVALID_CHAIR, false) ;

		//设置按钮
		if (m_wBanker != GetMeChairID() && !IsLookonMode() && m_bGameStatus[GetMeChairID()] == true)
		{
			//下注大小
			SCORE lChipsArray[SET_CHIP_COUNT] = {0};
			CopyMemory(lChipsArray, pCallBanker->lChipArray[GetMeChairID()], sizeof(lChipsArray));
			
			m_GameClientView.SetChipArray(lChipsArray) ;

			UpdateChipButtonState(SW_SHOW, TRUE) ;
			SetGameClock(GetMeChairID(), IDI_SETCHIP, m_cbTimeSetChip);
		}

		//设置辅助时钟
		if (!IsLookonMode() && m_bGameStatus[GetMeChairID()] == true)
		{
			for(BYTE i=0;i<GAME_PLAYER;i++)
			{
				if(i != m_wBanker && i != GetMeChairID() && m_bGameStatus[i] == true)
				{
					m_GameClientView.ShowFuZhuClock(SwitchViewChairID(i),true);
				}
			}
			m_cbFuZhuTime=m_cbTimeSetChip;
			m_GameClientView.SetFuZhuTime(m_cbFuZhuTime--);
			//设置时间
			SetTimer(IDI_FU_ZHU, 1000, NULL);
		}

		//提示信息
		m_GameClientView.SetTipInfoState(2);
	}
	//玩家不叫
	else
	{
		m_wMaxChip = pCallBanker->lMaxChip;

		//设置定时器
		SetGameClock( pCallBanker->wCurrentCaller, IDI_CALL_BANKER, m_cbTimeCallBanker ) ;				

		//设置按钮
		if ( pCallBanker->wCurrentCaller == GetMeChairID() && !IsLookonMode() && m_bGameStatus[GetMeChairID()] == true)
		{
			m_GameClientView.m_btCallBanker.ShowWindow(SW_SHOW);
			m_GameClientView.m_btNoCallBanker.ShowWindow(SW_SHOW);
		}

		//设置界面
		m_GameClientView.SetCallBankerStatus(SwitchViewChairID(pCallBanker->wLastCaller), true) ;
	}

	return true ;
}

//都不叫庄
bool CGameClientEngine::OnSubAllNotCall(const void * pBuffer, WORD wDataSize)
{
	//消息提示
	if(m_pIStringMessage != NULL)
	m_pIStringMessage->InsertSystemString(TEXT("所有人都不叫庄，游戏结束")) ;

	//设置界面
	m_GameClientView.SetCallBankerStatus(INVALID_CHAIR, false);
	m_GameClientView.SetTrustee(INVALID_CHAIR, false);

	//提示信息
	m_GameClientView.SetTipInfoState(0);

	if (false == IsLookonMode()) 
	{
		//取消托管
		m_bTrustee = false;
		m_GameClientView.m_btTrustee.SetButtonImage(IDB_BT_TRUSTEE_OPEN, AfxGetInstanceHandle(), false, false);
		m_GameClientView.m_btTrustee.EnableWindow(TRUE);

		m_GameClientView.m_btNoCallBanker.ShowWindow(SW_HIDE) ;
		m_GameClientView.m_btCallBanker.ShowWindow(SW_HIDE) ;

		//显示结算
		m_GameClientView.SetGameScore(INVALID_CHAIR, -1);

		//开始按钮
		m_GameClientView.m_btStart.ShowWindow(SW_SHOW) ;
		m_GameClientView.m_btStart.EnableWindow(TRUE) ;

		//设置定时器
		SetGameClock(GetMeChairID(), IDI_START_GAME, m_cbTimeStartGame) ;
	}

	return true ;
}

//发送扑克
bool CGameClientEngine::OnSubSendCardEx(const void * pBuffer, WORD wDataSize)
{
	ASSERT(wDataSize == sizeof(CMD_S_SendCard));
	if (wDataSize != sizeof(CMD_S_SendCard)) return false;
	
	//删除定时器
	KillTimer(IDI_FU_ZHU);
	m_cbFuZhuTime=0;
	
	//提示信息
	m_GameClientView.SetTipInfoState(0);

	//变量定义
	CMD_S_SendCard * pSendCard = (CMD_S_SendCard *)pBuffer;

	//设置扑克
	if (GetMeChairID() == pSendCard->wCurrentUser && false == IsLookonMode() || IsLookonMode() && m_bUserCheat)
	{
		m_bHandCardCount = HAND_CARD_COUNT;
		CopyMemory(m_bHandCardData[GetMeChairID()], pSendCard->bCardData, sizeof(BYTE)*m_bHandCardCount);
		CopyMemory(m_bTmpHandCardData, pSendCard->bCardData, sizeof(BYTE)*m_bHandCardCount);
	}

	if(m_bUserCheat)
	{
		OnShuffleFinish(0, 0);
	}
	else
	{
		//清空界面
		m_GameClientView.SetCardData(INVALID_CHAIR, NULL,  0, enCCTAllSegmentCard);
		m_GameClientView.SetCardData(INVALID_CHAIR, NULL, 0, enCCTHandCard);


		//开始洗牌
		m_GameClientView.StartShuffleCard();
	}

	return true;
}


//设置下注
bool CGameClientEngine::OnSubSetChip(const void * pBuffer , WORD wDataSize)
{
	//数据验证
	ASSERT(wDataSize==(sizeof(CMD_S_SetChip))) ;
	if(sizeof(CMD_S_SetChip)!=wDataSize) return false ;
	CMD_S_SetChip *pSetChip = (CMD_S_SetChip*)pBuffer ;

	m_lUserChip[pSetChip->wChipUser] = pSetChip->lChip ;
	
	//设置辅助时钟
	m_GameClientView.ShowFuZhuClock(SwitchViewChairID(pSetChip->wChipUser),false);

	//设置界面
	m_GameClientView.SetChip(SwitchViewChairID(pSetChip->wChipUser) , pSetChip->lChip, true);
	m_GameClientView.m_UserCardControl[2].ShootAllCard(false);

	return true ;
}

//设置分段
bool CGameClientEngine::OnSubSetSegment(const void * pBuffer , WORD wDataSize)
{
	//数据验证
	ASSERT(sizeof(CMD_S_SetSegment)==wDataSize) ;
	if(sizeof(CMD_S_SetSegment)!=wDataSize) return false ;
    
	CMD_S_SetSegment *pSetSegment = (CMD_S_SetSegment*)pBuffer ;

	//旁观用户
	if (IsLookonMode())
	{
		m_GameClientView.m_CenterCardControl.ShowWindow(SW_SHOW);
		m_GameClientView.m_CenterCardControl.SetDisplayFlag(false);
	}

	BYTE bZeroCardData[5];
	ZeroMemory(bZeroCardData, sizeof(bZeroCardData));
	BYTE bCardCount = (pSetSegment->SegmentType==enSegFront) ? 3 : 5;

	//其他玩家视图	
	if(GetMeChairID()!=pSetSegment->wCurrentUser)
	{
		//m_GameClientView.SetCardData(SwitchViewChairID(pSetSegment->wCurrentUser), bZeroCardData, bCardCount, enCCTSegmentCard, pSetSegment->SegmentType);
	}
	
	return true ;
}

//玩家摊牌
bool CGameClientEngine::OnSubShowCard(const void * pBuffer , WORD wDataSize)
{
	//数据验证
	ASSERT(sizeof(CMD_S_ShowCard)==wDataSize) ;
	if(sizeof(CMD_S_ShowCard)!=wDataSize) return false ;

	CMD_S_ShowCard *pShowCard = (CMD_S_ShowCard*)pBuffer ;

	WORD wCurrentUser = pShowCard->wCurrentUser;
	WORD wViewChairID = SwitchViewChairID(wCurrentUser);
	if ((GetMeChairID()==wCurrentUser) && (true == pShowCard->bCanSeeShowCard))
	{	
		//设置扑克
		BYTE bCard[13] = {0};
		CopyMemory(bCard, pShowCard->bFrontCard, 13);
		m_GameClientView.SetCardData(wViewChairID, bCard, 13, enCCTAllSegmentCard);

		if(IsLookonMode())
			m_GameClientView.m_AllSegCardControl[wViewChairID].SetDisplayFlag(false);
		if (GetMeChairID() != wCurrentUser)
			m_GameClientView.m_AllSegCardControl[wViewChairID].SetDisplayFlag(false);
	
		//保存数据
		CopyMemory(m_bSegmentCard[wCurrentUser][0] , pShowCard->bFrontCard , 3) ;
		CopyMemory(m_bSegmentCard[wCurrentUser][1] , pShowCard->bMidCard , 5) ;
		CopyMemory(m_bSegmentCard[wCurrentUser][2] , pShowCard->bBackCard , 5) ;
	}
	else
	{
		//显示背面
		BYTE bCard[13] = {0};
		m_GameClientView.m_AllSegCardControl[wViewChairID].SetCardData(bCard, 13);
		m_GameClientView.m_AllSegCardControl[wViewChairID].SetDisplayFlag(false);
	}

	m_GameClientView.SetCardData(wViewChairID , NULL , 0 , enCCTHandCard) ;
	
	return true ;
}

//比较扑克
bool CGameClientEngine::OnSubCompareCard(const void * pBuffer , WORD wDataSize)
{
	//数据验证
	ASSERT(sizeof(CMD_S_CompareCard)==wDataSize) ;
	if(sizeof(CMD_S_CompareCard)!=wDataSize) return false ;

	CMD_S_CompareCard *pCompareCard = (CMD_S_CompareCard*)pBuffer;
	CopyMemory(m_bSegmentCard, pCompareCard->bSegmentCard, sizeof(m_bSegmentCard));
	CopyMemory(m_lScoreTimes, pCompareCard->lScoreTimes, sizeof(m_lScoreTimes));  
	m_bComparedCard = pCompareCard->bCompared;
	
	m_GameClientView.SetGlobalClock(false);
	m_GameClientView.SetElapseTime(0);
	KillTimer(IDI_ALL_RANGE_CARD);

	for (int i = 0; i < GAME_PLAYER; i++)
	{
		if ((GetTableUserItem(i) == NULL) || (m_bGameStatus[i] == false)) 
		{
			continue;
		}

		BYTE bCard[13] = {};
		CopyMemory(bCard, pCompareCard->bSegmentCard[i][0], sizeof(BYTE)*3);
		CopyMemory(bCard+3, pCompareCard->bSegmentCard[i][1], sizeof(BYTE)*10);

		//设置牌型
		WORD wViewChairID = SwitchViewChairID(i);

		//特殊牌型声音
		if (pCompareCard->bSpecialType[i] > CT_EX_INVALID)
		{
			m_GameClientView.PlaySpecCardTypeSound(wViewChairID, pCompareCard->bSpecialType[i]);
			m_GameClientView.SetCardTypeSpec(true, wViewChairID, pCompareCard->bSpecialType[i]);
		}
		//设置扑克
		m_GameClientView.m_UserCardControl[wViewChairID].ShowWindow(SW_HIDE);
		m_GameClientView.m_AllSegCardControl[wViewChairID].SetCardData(bCard, 13);

		if (pCompareCard->bCompared == TRUE)
			m_GameClientView.m_AllSegCardControl[wViewChairID].SetDisplayFlag(false);
		else
			m_GameClientView.m_AllSegCardControl[wViewChairID].SetDisplayFlag(true);
	}
	
	//普通牌型，需翻牌比较
	if (pCompareCard->bCompared == TRUE)
	{
		//比较前墩
		m_nTimer=SetTimer(IDI_COMPARE_FRONT , m_nCompareTime , NULL);
		CompareCard(enSegFront);
	}
	
	return true ;
}

//用户强退
bool CGameClientEngine::OnSubPlayerExit(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	if (wDataSize!=sizeof(CMD_S_PlayerExit)) return false;
	CMD_S_PlayerExit * pPlayerExit=(CMD_S_PlayerExit *)pBuffer;

	WORD wID = pPlayerExit->wPlayerID;
	WORD wViewID = SwitchViewChairID(wID);

	//游戏信息
	ASSERT(m_bGameStatus[wID]==true);
	m_bGameStatus[wID]=false;
	m_GameClientView.SetGameStatus(wViewID, false);
	m_GameClientView.SetChip(wViewID, -1);
	m_GameClientView.SetGameScore(wViewID, -1);
	m_GameClientView.SetScoreTimes(wViewID, MAXLONGLONG);
	m_GameClientView.m_UserCardControl[wViewID].ShowWindow(SW_HIDE);

	BYTE bCount=0;
	for(WORD i=0;i<GAME_PLAYER;i++) if(m_bGameStatus[i]!=false)bCount++;
	if(bCount==1) m_GameClientView.SetBanker(INVALID_CHAIR);

	if (GetMeChairID() != pPlayerExit->wPlayerID)
	{
		//积分提示
		if (m_pIStringMessage != NULL) m_pIStringMessage->InsertSystemString(TEXT("\n用户强退：")) ;

		for (WORD i=0;i<GAME_PLAYER;i++)
		{
			IClientUserItem * pClientUserItem=GetTableUserItem(i);
			if(pClientUserItem != NULL)
			{
				CString strScoreRes ;
				strScoreRes.Format(TEXT("%s：%I64d\n") , pClientUserItem->GetNickName(), pPlayerExit->lScore[i]) ;
				if (m_pIStringMessage != NULL) m_pIStringMessage->InsertNormalString( strScoreRes);
			}
		}
	}

	return true;
}

//用户切牌
bool CGameClientEngine::OnSubSplitCard(const void * pBuffer, WORD wDataSize)
{
	ASSERT(wDataSize == sizeof(CMD_S_SplitCard));
	if (wDataSize != sizeof(CMD_S_SplitCard)) return false;

	CMD_S_SplitCard *pSplitCard = (CMD_S_SplitCard *)pBuffer;

	//设置变量
	m_bRequestSplit = true;

	//广播消息
	if (pSplitCard->wLastUser != GetMeChairID())
	{
		TCHAR szMsg[128] = TEXT("");
		_sntprintf(szMsg, CountArray(szMsg), TEXT("玩家【%s】请求下一局切牌！"), GetTableUserItem(pSplitCard->wLastUser)->GetNickName());
		m_pIStringMessage->InsertSystemString(szMsg);
	}
	else m_pIStringMessage->InsertSystemString(TEXT("您请求下一局切牌！"));

	return  true;
}

//开始发牌
bool CGameClientEngine::OnSubDispatchStart(const void * pBuffer, WORD wDataSize)
{
	//设置状态
	SetGameStatus(GS_WK_PLAYING);

	WORD wMeChairID = GetMeChairID();

	//手牌显示
	if (m_bGameStatus[wMeChairID] != false)
	{
		m_GameClientView.m_UserCardControl[2].ShowWindow(SW_SHOW);
		
		
		if (IsLookonMode())
		{
			m_GameClientView.m_UserCardControl[2].SetDisplayFlag(false || m_bUserCheat);
			m_GameClientView.m_UserCardControl[2].SetPositively(false);
		}
		else
		{
			m_GameClientView.m_UserCardControl[2].SetPositively(true);
			m_GameClientView.m_UserCardControl[2].SetDisplayFlag(true);
		}
			
	}

	//其他玩家
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//设置背景
		if(wMeChairID!=i && m_bGameStatus[i] != false)
		{
			m_GameClientView.m_UserCardControl[SwitchViewChairID(i)].ShowWindow(SW_SHOW);
		}
	}

	//计算比牌时间
	WORD wPlayerCount = 0;
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		if ((GetTableUserItem(i) == NULL) && (m_bGameStatus[i] == false)) continue;
		wPlayerCount++;
	}
	if (wPlayerCount == 2) m_nCompareTime = 4000;
	else if (wPlayerCount == 3) m_nCompareTime = 5000;
	else m_nCompareTime = 6000;

	//发牌
	if ((false == m_bRequestSplit))
	{
		if(m_bCheatRight)		
		{
			OnDispatchCardFinish(0, 0);
		}
		else
		{
			//发牌动画
			WORD wViewStartUser=SwitchViewChairID(m_wBanker);
			m_GameClientView.StartDispatchCard(m_bHandCardData[GetMeChairID()],wViewStartUser);

			//播放声音
			PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_START"));
		}
	}
	else		//切牌动画
	{
		m_bRequestSplit = false;
		if(m_bCheatRight)		
		{
			//发送消息
			OnSplitFinish(0, 0);
		}
		else
		{
			m_GameClientView.StartSplitCard();
		}		
	}

	return true;
}

//用户托管
bool CGameClientEngine::OnSubTrustee(const void * pBuffer,WORD wDataSize)
{
	//校验数据
	ASSERT(wDataSize == sizeof(CMD_S_Trustee));
	if (wDataSize != sizeof(CMD_S_Trustee)) return false;

	//消息处理
	CMD_S_Trustee *pTrustee = (CMD_S_Trustee *)pBuffer;

	//设置状态
	m_GameClientView.SetTrustee(SwitchViewChairID(pTrustee->wChairID), pTrustee->bTrustee);
	if ((IsLookonMode()==true)||(pTrustee->wChairID!=GetMeChairID()))
	{
		IClientUserItem * pClientUserItem=GetTableUserItem(pTrustee->wChairID);
		TCHAR szBuffer[256];
		if(pTrustee->bTrustee==true)
			_sntprintf(szBuffer,sizeof(szBuffer),TEXT("玩家[%s]选择了托管功能."),pClientUserItem->GetNickName());
		else
			_sntprintf(szBuffer,sizeof(szBuffer),TEXT("玩家[%s]取消了托管功能."),pClientUserItem->GetNickName());
		if(m_pIStringMessage != NULL)
			m_pIStringMessage->InsertSystemString(szBuffer);
	}
	return true;
}

//语音请求
bool CGameClientEngine::OnSubVoicePhrase(const void * pBuffer, WORD wDataSize)
{
	//效验参数
	ASSERT(wDataSize==sizeof(CMD_S_ChoseYuYin));
	if (wDataSize!=sizeof(CMD_S_ChoseYuYin)) return false;

	//变量定义
	CMD_S_ChoseYuYin * pChoseYuYin = (CMD_S_ChoseYuYin *)pBuffer;
	HINSTANCE hInstance=AfxGetInstanceHandle();
	IClientUserItem *pIClientUserItem=GetTableUserItem(pChoseYuYin->wChairID);
		
	//输出文字
	TCHAR szBuffer[512]=TEXT("");
	_sntprintf(szBuffer,sizeof(szBuffer),TEXT("%s"),GetPhraseString(pIClientUserItem->GetGender(),pChoseYuYin->bType,pChoseYuYin->bChoseYuYin));
	m_pIStringMessage->InsertUserChat(pIClientUserItem->GetNickName(),szBuffer,RGB(0,0,0));


	//获取当前程序路径
	TCHAR szExePath[MAX_PATH] = {};
	GetModuleFileName( NULL,szExePath,CountArray(szExePath) );

	CString szTemp;
	szTemp = szExePath;
	int nPos = szTemp.ReverseFind(TEXT('\\'));
	_sntprintf(szExePath, CountArray(szExePath), TEXT("%s"), szTemp.Left(nPos));

	//播放声音
	if(pIClientUserItem->GetGender() == 1)
	{
		_sntprintf(szBuffer,sizeof(szBuffer),TEXT("%s\\Sound\\Man\\%d_%d.wav"),szExePath,pChoseYuYin->bType,pChoseYuYin->bChoseYuYin);
	}
	else
	{
		_sntprintf(szBuffer,sizeof(szBuffer),TEXT("%s\\Sound\\Women\\%d_%d.wav"),szExePath,pChoseYuYin->bType,pChoseYuYin->bChoseYuYin);
	}

	//播放声音
	PlayGameSound(szBuffer);

	return true;
}

//作弊数据
bool CGameClientEngine::OnSubCheatCard(VOID * pData, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_CheatCard));
	if (wDataSize!=sizeof(CMD_S_CheatCard)) return false;

	//变量定义
	CMD_S_CheatCard * pCheatCard=(CMD_S_CheatCard *)pData;

	ASSERT(pCheatCard->cbUserCount <= GAME_PLAYER);
	if(pCheatCard->cbUserCount > GAME_PLAYER) return false;

	//清空界面
	m_GameClientView.SetCardData(INVALID_CHAIR, NULL,  0, enCCTAllSegmentCard);
	m_GameClientView.SetCardData(INVALID_CHAIR, NULL, 0, enCCTHandCard);

	m_bCheatRight = true;
	for(WORD i = 0; i < pCheatCard->cbUserCount ; i++)
	{
		//显示扑克
		m_bUserCheat[pCheatCard->wCardUser[i]] = true;
		CopyMemory(m_bHandCardData[pCheatCard->wCardUser[i]], pCheatCard->cbCardData[i], sizeof(BYTE) * pCheatCard->cbCardCount[i]);

		ShowAutomaticOutCard(pCheatCard->wCardUser[i]);

	}
	//更新界面
	CRect rcClient;
	GetClientRect(&rcClient);
	m_GameClientView.InvalidGameView(0,0,rcClient.Width(),rcClient.Height());
	return true;
}

//游戏结束
bool CGameClientEngine::OnSubGameEnd(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_GameEnd));
	if (wDataSize!=sizeof(CMD_S_GameEnd)) return false;

	//消息处理
	CMD_S_GameEnd * pGameEnd=(CMD_S_GameEnd *)pBuffer;
	WORD wPlayerCount = 0;
	
	//游戏状态
	SetGameStatus(GS_WK_FREE);

	//停止动画
	m_GameClientView.StopShuffleCard();
	m_GameClientView.StopSplitCard();
	m_GameClientView.StopDispatchCard();
	m_GameClientView.StopSwatCartoon();
	m_GameClientView.StopGunCartoon();

	//删除定时器
	KillTimer(IDI_FU_ZHU) ;
	m_cbFuZhuTime=0;
	
	//提示信息
	m_GameClientView.SetTipInfoState(0);
	
	//删除定时器
	KillGameClock(IDI_START_GAME);
	KillGameClock(IDI_CALL_BANKER);
	KillGameClock(IDI_SETCHIP);
	KillTimer(IDI_ALL_RANGE_CARD);
	//KillGameClock(IDI_RANGE_CARD);
	//KillGameClock(/*IDI_SHOW_CARD*/);
	KillTimer(m_nTimer) ;

	//设置按钮
	UpdateButtonState();
	UpdateChipButtonState(SW_HIDE, FALSE) ;
	UpdateBlockButton(0, SW_HIDE, FALSE);
	m_GameClientView.m_btNoCallBanker.ShowWindow(SW_HIDE) ;
	m_GameClientView.m_btCallBanker.ShowWindow(SW_HIDE) ;
	m_GameClientView.m_btRestore[0].ShowWindow(SW_HIDE);
	m_GameClientView.m_btRestore[0].EnableWindow(FALSE);
	m_GameClientView.m_btRestore[1].ShowWindow(SW_HIDE);
	m_GameClientView.m_btRestore[1].EnableWindow(FALSE);
	m_GameClientView.m_btRestore[2].ShowWindow(SW_HIDE);
	m_GameClientView.m_btRestore[2].EnableWindow(FALSE);
	m_GameClientView.m_btRestore[3].ShowWindow(SW_HIDE);
	m_GameClientView.m_btRestore[3].EnableWindow(FALSE);

	if (!m_bTrustee && !IsLookonMode())
	{
		m_GameClientView.m_btTrustee.ShowWindow(SW_SHOW);
		m_GameClientView.m_btTrustee.EnableWindow(TRUE);
	}

	//设置界面
	m_tagSegInfo.Init();
	m_GameClientView.SetGlobalClock(false);
	m_GameClientView.SetCallBankerStatus(INVALID_CHAIR, false);
	m_GameClientView.SetTrustee(INVALID_CHAIR, false);
	m_GameClientView.ShowCenterControl(false);
	m_GameClientView.SetCardData(INVALID_CHAIR , NULL , 0 , enCCTHandCard);
	m_GameClientView.m_CenterCardControl.SetCardData(-1, NULL, 0);

	//显示扑克
	for (WORD i=0; i<GAME_PLAYER; i++)
	{
		if (GetTableUserItem(i) == NULL || m_bGameStatus[i] == false) continue;

		wPlayerCount++;
		 WORD wViewChairID = SwitchViewChairID(i);
		
		if (m_bSegmentCard[i][0][0] != 0)
		{ 
			m_GameClientView.m_AllSegCardControl[wViewChairID].SetCardData(0, m_bSegmentCard[i][0], 3);
			m_GameClientView.m_AllSegCardControl[wViewChairID].SetCardData(1, m_bSegmentCard[i][1], 5);
			m_GameClientView.m_AllSegCardControl[wViewChairID].SetCardData(2, m_bSegmentCard[i][2], 5);

			m_GameClientView.m_AllSegCardControl[wViewChairID].SetDisplayFlag(true);
			m_GameClientView.m_AllSegCardControl[wViewChairID].ShowWindow(SW_SHOW);
		}

	}
	
	//取消托管
	if ((true == m_bTrustee) && (false == IsLookonMode()))
	{
		m_bTrustee = false;
		m_GameClientView.m_btTrustee.SetButtonImage(IDB_BT_TRUSTEE_OPEN, AfxGetInstanceHandle(), false, false);
		m_GameClientView.m_btTrustee.EnableWindow(TRUE);
	}


	//全垒打动画		
	bool bAllSwat = true;
	if ((wPlayerCount == GAME_PLAYER) && (m_bComparedCard == TRUE))
	{
		//赢牌玩家
		WORD wWinnerUser=INVALID_CHAIR;
		for (WORD i=0; i<GAME_PLAYER; i++)
		{
			if(pGameEnd->bWinInfo[i]==2)
			{
				wWinnerUser=i;
			}
		}

		if(wWinnerUser!=INVALID_CHAIR)
		{
			//与所有闲家比较
			for (WORD i=0; i<GAME_PLAYER; i++)
			{
				if (i != wWinnerUser)
				{
					//如果赢家有一注输，则结束循环
					if ((m_lScoreTimes[wWinnerUser][0] < m_lScoreTimes[i][0]) ||
						(m_lScoreTimes[wWinnerUser][1] < m_lScoreTimes[i][1]) ||
						(m_lScoreTimes[wWinnerUser][2] < m_lScoreTimes[i][2]))
					{
						bAllSwat = false;
						break;
					}
				}
			}
			if (bAllSwat == true)
				m_GameClientView.StartSwatCartoon(SwitchViewChairID(wWinnerUser));
		}
	}

	//////////////////////////////////////////////////////////////////////////
	//打枪动画
	WORD wGunCounter = 0;
	tagGunStatus GunStatus[GAME_PLAYER];
	FillMemory(&GunStatus, sizeof(tagGunStatus)*GAME_PLAYER, INVALID_CHAIR);

	//闲家比较
	for (WORD i = 0; i < GAME_PLAYER; i++)
	{
		if (GetTableUserItem(i) == NULL ||
			m_bGameStatus[i] == false || i == m_wBanker) continue;

		for (WORD j = i + 1; j < GAME_PLAYER; j++)
		{
			if (GetTableUserItem(j) == NULL ||
				m_bGameStatus[j] == false || j == m_wBanker) continue;

			//三注全大
			if ((m_lScoreTimes[i][0] > m_lScoreTimes[j][0]) &&
				(m_lScoreTimes[i][1] > m_lScoreTimes[j][1]) &&
				(m_lScoreTimes[i][2] > m_lScoreTimes[j][2]) )
			{
				GunStatus[wGunCounter].wSrcUser = SwitchViewChairID(i);
				GunStatus[wGunCounter].wDestUser = SwitchViewChairID(j);
				wGunCounter++;
			}

			//三注全小
			if ((m_lScoreTimes[i][0] < m_lScoreTimes[j][0]) &&
				(m_lScoreTimes[i][1] < m_lScoreTimes[j][1]) &&
				(m_lScoreTimes[i][2] < m_lScoreTimes[j][2]) )
			{
				GunStatus[wGunCounter].wSrcUser = SwitchViewChairID(j);
				GunStatus[wGunCounter].wDestUser = SwitchViewChairID(i);
				wGunCounter++;
			}
		}
	}

	//庄闲比较
	for (i = 0; i < GAME_PLAYER; i++)
	{
		if (GetTableUserItem(i) == NULL ||
			m_bGameStatus[i] == false || i == m_wBanker) continue;

		//三注全大
		if ((m_lScoreTimes[i][0] > 0) &&
			(m_lScoreTimes[i][1] > 0) &&
			(m_lScoreTimes[i][2] > 0) )
		{
			GunStatus[wGunCounter].wSrcUser = SwitchViewChairID(i);
			GunStatus[wGunCounter].wDestUser = SwitchViewChairID(m_wBanker);
			wGunCounter++;
		}

		//三注全小
		if ((m_lScoreTimes[i][0] < 0) &&
			(m_lScoreTimes[i][1] < 0) &&
			(m_lScoreTimes[i][2] < 0) )
		{
			GunStatus[wGunCounter].wSrcUser = SwitchViewChairID(m_wBanker);
			GunStatus[wGunCounter].wDestUser = SwitchViewChairID(i);
			wGunCounter++;
		}
	}

	//for (WORD i=0; i<GAME_PLAYER; i++)
	//{
	//	if (GetTableUserItem(i) == NULL || m_bGameStatus[i] == false) continue;

	//	for (WORD j=i+1; j<GAME_PLAYER; j++)
	//	{
	//		if (GetTableUserItem(j) == NULL || m_bGameStatus[j] == false) continue;

	//		//三注全大
	//		if ((m_lScoreTimes[i][0] > m_lScoreTimes[j][0]) &&
	//			(m_lScoreTimes[i][1] > m_lScoreTimes[j][1]) &&
	//			(m_lScoreTimes[i][2] > m_lScoreTimes[j][2]) )
	//		{
	//			GunStatus[wGunCounter].wSrcUser = SwitchViewChairID(i);
	//			GunStatus[wGunCounter].wDestUser = SwitchViewChairID(j);
	//			wGunCounter++;
	//		}

	//		//三注全小
	//		if ((m_lScoreTimes[i][0] < m_lScoreTimes[j][0]) &&
	//			(m_lScoreTimes[i][1] < m_lScoreTimes[j][1]) &&
	//			(m_lScoreTimes[i][2] < m_lScoreTimes[j][2]) )
	//		{
	//			GunStatus[wGunCounter].wSrcUser = SwitchViewChairID(j);
	//			GunStatus[wGunCounter].wDestUser = SwitchViewChairID(i);
	//			wGunCounter++;
	//		}
	//	}
	//}
	if (wGunCounter != 0)
	{
		m_GameClientView.SetGunCartoonGroup(GunStatus, wGunCounter);
		m_GameClientView.StartGunCartoon(GunStatus[0].wSrcUser, GunStatus[0].wDestUser);
	}
	//////////////////////////////////////////////////////////////////////////

	if(GER_DISMISS == pGameEnd->bEndMode)
	{
		if (m_pIStringMessage != NULL) m_pIStringMessage->InsertSystemString(TEXT("游戏解散："));
	}
	//设置积分
	if (m_pIStringMessage != NULL) m_pIStringMessage->InsertSystemString(TEXT("本局得分："));
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		IClientUserItem * pClientUserItem=GetTableUserItem(i);
		
		
		//if (m_bGameStatus[i] == false) continue;
		WORD wViewChairID = SwitchViewChairID(i);
		//结算结果

		if (pClientUserItem !=NULL)
		{
			
			m_GameClientView.SetGameScore(wViewChairID, pGameEnd->lGameScore[i]);
			m_GameClientView.SetScoreTimes(wViewChairID, pGameEnd->lScoreTimes[i]);

			CString strScoreRes ;
			strScoreRes.Format( TEXT("%s：%I64d\n") , pClientUserItem->GetNickName(),pGameEnd->lGameScore[i]);
			if(m_pIStringMessage != NULL) m_pIStringMessage->InsertNormalString( strScoreRes) ;

			//积分框
			m_HistroyScore.OnEventUserScore(i, pGameEnd->lGameScore[i]);
		}
		else 
		{
			m_GameClientView.SetChip(wViewChairID, -1);
			m_GameClientView.SetGameScore(wViewChairID, -1);
			m_GameClientView.SetScoreTimes(wViewChairID, MAXLONGLONG);
		}
	}

	
	if(GER_NORMAL==pGameEnd->bEndMode)
	{
		//显示信息	
		if(m_pIStringMessage != NULL) m_pIStringMessage->InsertSystemString( TEXT( "比牌结果：\n" )) ;
		//庄家信息
		IClientUserItem *pBanker = GetTableUserItem(m_wBanker);

		for(WORD i=0 ; i<GAME_PLAYER ; ++i)
		{
			//变量定义
			BYTE bUserWin = FALSE; 
			BYTE bWindCount = 0 ;
			CString strWindMsg = TEXT("") ;

			IClientUserItem * pClientUserItem=GetTableUserItem(i);
			if (pClientUserItem == NULL || m_bGameStatus[i] == false) continue;

			TCHAR szBuffer[64];
			_sntprintf(szBuffer,CountArray(szBuffer),TEXT("\n%s:%+I64d "),pClientUserItem->GetNickName(), 
				pGameEnd->lGameScore[pClientUserItem->GetChairID()]);
			
			if(i!=m_wBanker)
			{
				bUserWin = pGameEnd->bWinInfo[i];
				
				//闲家赢
				if (bUserWin == 2)
					strWindMsg = TEXT("[ ") + (CString(pClientUserItem->GetNickName()) + CString(TEXT(" ] 赢 庄家[ ")) + CString(pBanker->GetNickName()) + TEXT(" ]\n")) ;
				else if (bUserWin == 0)
					strWindMsg = (CString(TEXT("庄家[ ")) + CString(pBanker->GetNickName()) + CString(TEXT(" ] 赢 [ ")) + CString(pClientUserItem->GetNickName()) + TEXT(" ]\n")) ;
				else if (bUserWin == 1)
					strWindMsg = (CString(TEXT("庄家[ ")) + CString(pBanker->GetNickName()) + CString(TEXT(" ] 和 [ ")) + CString(pClientUserItem->GetNickName()) + TEXT(" ] 打和\n")) ;

				if (m_pIStringMessage != NULL) m_pIStringMessage->InsertNormalString(strWindMsg) ;
			}

			//播放声音
			if(i==GetMeChairID())
			{
				if(pGameEnd->lGameScore[i]>0) PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_WIN"));
				else PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_LOST"));
			}
		}
	}

	//设置界面
	if (IsLookonMode()==false)
	{
		//开始按钮
		m_GameClientView.m_btStart.ShowWindow(SW_SHOW);
		m_GameClientView.m_btStart.EnableWindow(TRUE);

		SetGameClock(GetMeChairID(),IDI_START_GAME,m_cbTimeStartGame);
	}

	//设置变量
	m_wBanker = INVALID_CHAIR;

	return true;
}

//定时器
void CGameClientEngine::OnTimer(UINT nIDEvent)
{
	if (nIDEvent == IDI_FU_ZHU)		//辅助时钟
	{
		m_GameClientView.SetFuZhuTime(m_cbFuZhuTime--);
		m_GameClientView.RefreshGameView();
		if (m_cbFuZhuTime == 0)
		{
			KillTimer(IDI_FU_ZHU);
			for(BYTE i=0;i<GAME_PLAYER;i++)
			{
				if(i != m_wBanker && i != GetMeChairID() && m_bGameStatus[i] == true)
				{
					m_GameClientView.ShowFuZhuClock(SwitchViewChairID(i),false);
				}
			}
		}
	}

	if (nIDEvent == 1122)
	{
		m_GameClientView.RefreshGameView();
	}
	if (nIDEvent == IDI_ALL_RANGE_CARD)
	{	
		m_TimerCount--;

		if (m_bTrustee)
		{
			if (!m_bFinishSegment)
			{
				//尚未理牌或理牌未完成
				if (m_bHandCardCount!=0)
				{
					//自动代打
					AutomaticOutCard();
					//摊牌
					OnShowCard(0 , 0);
					m_bFinishSegment = true;
					return;
				}
			}

			if (!m_bIsShowCard)
			{
				OnShowCard(0 , 0);
			}
		}

		if (m_TimerCount > 0)
		{
			m_GameClientView.SetElapseTime(m_TimerCount);
			m_GameClientView.RefreshGameView();
		}
		else
		{
			KillTimer(IDI_ALL_RANGE_CARD);
			m_GameClientView.SetGlobalClock(false);
			m_GameClientView.SetElapseTime(0);

			if(!m_bFinishSegment)
			{
				//尚未理牌或理牌未完成
				if (m_bHandCardCount!=0)
				{
					//自动代打
					AutomaticOutCard();
					//摊牌
					OnShowCard(0 , 0);
					m_bFinishSegment = true;

				}
				else
				{
					OnShowCard(0 , 0);
					m_bFinishSegment = true;
				}
			}
			else
			{
				if (!m_bIsShowCard)
				{
					OnShowCard(0 , 0);
				}
			}

			//else
			//{
			//	//直接摊牌
			//	//OnShowCard(0, 0);
			//	//m_bFinishSegment = true;
			//}
		}
		m_GameClientView.RefreshGameView();
		return;
	}
	if(nIDEvent==IDI_COMPARE_FRONT)		//前墩扑克
	{
		KillTimer(m_nTimer) ;
		m_nTimer=SetTimer(IDI_COMPARE_MID , m_nCompareTime , NULL) ;

		//设置比牌
		CompareCard(enSegMid);

		return ;
	}

	if(nIDEvent==IDI_COMPARE_MID)		//中墩扑克
	{
		KillTimer(m_nTimer) ;
		m_nTimer=SetTimer(IDI_COMPARE_BACK , m_nCompareTime , NULL) ;

		//设置比牌
		CompareCard(enSegBack);

		return  ;
	}

	if(nIDEvent==IDI_COMPARE_BACK)		//后墩扑克
	{
		KillTimer(m_nTimer) ;
		

		//显示扑克
		for(WORD i=0 ; i<GAME_PLAYER ; i++)
		{
			if (GetTableUserItem(i) == NULL || m_bGameStatus[i] == false) continue;

			WORD wViewChairID = SwitchViewChairID(i);
			m_GameClientView.m_AllSegCardControl[wViewChairID].SetDisplayFlag(true);
			m_GameClientView.m_AllSegCardControl[wViewChairID].ShowWindow(SW_SHOW);
		}

		m_nTimer=SetTimer(IDI_COMPLETE_COMPARE , 1800 , NULL) ;
		return ;
	}

	if(nIDEvent==IDI_COMPLETE_COMPARE)
	{
		KillTimer(m_nTimer) ;

		if(false==IsLookonMode()) SendSocketData(SUB_C_COMPLETE_COMPARE , NULL , 0) ;

		return ;
	}

	if(nIDEvent==IDI_CAN_PLAY_VOICE)
	{
		m_bCanPlayVoice = true;
		KillTimer(IDI_CAN_PLAY_VOICE);

		return;
	}
	__super::OnTimer(nIDEvent) ;
}

//自动代打
void CGameClientEngine::AutomaticOutCard() 
{
	//设置压注
	if(-1==m_lUserChip[GetMeChairID()] && GetMeChairID()!=m_wBanker)
	{
		m_lUserChip[2] = min(300, 10) ;
		m_GameClientView.SetChip(2 , m_lUserChip[2], true) ;

		CMD_C_SetChip  SetChip ;
		ZeroMemory(&SetChip, sizeof(SetChip));
		SetChip.nChip = m_lUserChip[2] ;
		SendSocketData(SUB_C_SETCHIP , &SetChip , sizeof(SetChip)) ;
	}

	//恢复扑克
	CopyMemory(m_bHandCardData[GetMeChairID()] , m_bTmpHandCardData , sizeof(m_bHandCardData[GetMeChairID()])) ; //防止玩家分了一段或两段后超时调用此函数
	m_bHandCardCount = HAND_CARD_COUNT ;
	m_GameLogic.SortCardList(m_bHandCardData[GetMeChairID()], m_bHandCardCount, enDescend);
	m_GameClientView.m_UserCardControl[2].SetCardData(m_bHandCardData[GetMeChairID()] , m_bHandCardCount);
	m_GameClientView.m_UserCardControl[2].ShowWindow(SW_SHOW);
	m_GameClientView.m_AllSegCardControl[2].SetCardData(NULL, 0);

	//分段扑克
	m_GameClientView.m_CenterCardControl.SetCardData(-1, NULL, 0);

	m_bFinishSegment = false;
	m_tagSegInfo.Init();

	//自动摆牌
	BYTE bAutoPutCard[13], bIndex[13];
	ZeroMemory(bAutoPutCard, sizeof(bAutoPutCard));
	ZeroMemory(bIndex, sizeof(bIndex));

	m_GameLogic.AutoPutCard(m_bHandCardData[GetMeChairID()], bAutoPutCard, false,false);

	//对应序列
	for (int i = 0; i < 13; i++)
	{
		for (int j = 0; j < 13; j++)
		{
			if (bAutoPutCard[i] == m_bHandCardData[GetMeChairID()][j])
			{
				bIndex[i] = j;
				break;
			}
		}
	}

	//设置前蹲
	m_GameClientView.m_UserCardControl[2].SetShootByIndex(bIndex, 3);
	OnFrontCard(0, 0);

	//对应序列
	for (int i = 3; i < 13; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			if (bAutoPutCard[i] == m_bHandCardData[GetMeChairID()][j])
			{
				bIndex[i] = j;
				break;
			}
		}
	}

	//设置中蹲
	m_GameClientView.m_UserCardControl[2].SetShootByIndex(bIndex+3, 5);
	OnMidCard(0, 0);

	return;
}

//显示牌型
void CGameClientEngine::ShowAutomaticOutCard(WORD wChairID) 
{
	WORD wViewChairID = SwitchViewChairID(wChairID);

	//自动摆牌
	BYTE bAutoPutCard[13], bIndex[13];
	ZeroMemory(bAutoPutCard, sizeof(bAutoPutCard));
	ZeroMemory(bIndex, sizeof(bIndex));

	m_GameLogic.AutoPutCard(m_bHandCardData[wChairID], bAutoPutCard, true, false);

	m_GameClientView.m_AllSegCardControl[wViewChairID].SetCardData(bAutoPutCard, 13);
	m_GameClientView.m_AllSegCardControl[wViewChairID].SetDisplayFlag(true);
}

//////////////////////////////////////////////////////////////////////////

//比牌场景
void CGameClientEngine::CompareCard(BYTE bSceneName)
{
	//显示信息
	m_GameClientView.SetCompareScene((enSegmentType)bSceneName);

	//翻牌动画
	m_GameClientView.ReversalCard(SwitchViewChairID(m_wBanker), (bSceneName==enSegFront)?3:5);

	////针对旁观
	//if(true==IsLookonMode())
	//{
	//	WORD  wViewChairID = SwitchViewChairID(m_wBanker) ;
	//	m_GameClientView.m_AllSegCardControl[wViewChairID].SetDisplayFlag(true) ;
	//}
}

//更新按钮

void CGameClientEngine::UpdateButtonState()
{		
	//禁用提示
	for (UINT i = 0; i < 8; i++)
		m_GameClientView.m_btPrompt[i].EnableWindow(FALSE);
	
	if (IsLookonMode() == true)
	{
		//m_GameClientView.m_btSplitCard.EnableWindow(FALSE);
		m_GameClientView.m_btTrustee.EnableWindow(FALSE);
		m_GameClientView.m_btSortCard.EnableWindow(FALSE);
	}
	
	if ((m_bHandCardCount <= 0) || GetGameStatus() != GS_WK_PLAYING)
	{
		//禁用排序
		m_GameClientView.m_btSortCard.EnableWindow(FALSE);
		
		//禁用切牌
		//m_GameClientView.m_btSplitCard.EnableWindow(FALSE);

		//禁用理牌
		m_GameClientView.m_btFrontCard.ShowWindow(SW_HIDE);
		m_GameClientView.m_btFrontCard.EnableWindow(FALSE);
		m_GameClientView.m_btMidCard.ShowWindow(SW_HIDE);
		m_GameClientView.m_btMidCard.EnableWindow(FALSE);
		m_GameClientView.m_btBackCard.ShowWindow(SW_HIDE);
		m_GameClientView.m_btBackCard.EnableWindow(FALSE);

		return;
	}

	//
	OnPrompt(-1, 0);

	//对按钮
	m_GameLogic.SearchSameCard(m_bHandCardData[GetMeChairID()], m_bHandCardCount, 2, &m_SearchCardResult);
	if (m_SearchCardResult.cbSearchCount >= 2)
	{
		m_GameClientView.m_btPrompt[1].EnableWindow(TRUE);
		m_GameClientView.m_btPrompt[0].EnableWindow(TRUE);
	}

	if (m_SearchCardResult.cbSearchCount == 1)
	{
		m_GameClientView.m_btPrompt[0].EnableWindow(TRUE);
	}

	//三条按钮
	m_GameLogic.SearchSameCard(m_bHandCardData[GetMeChairID()], m_bHandCardCount, 3, &m_SearchCardResult);
	if (m_SearchCardResult.cbSearchCount >= 1)
	{
		m_GameClientView.m_btPrompt[2].EnableWindow(TRUE);
	}

	//顺子按钮
	m_GameLogic.SearchLineCardType(m_bHandCardData[GetMeChairID()], m_bHandCardCount, 5, &m_SearchCardResult);
	if (m_SearchCardResult.cbSearchCount > 0)
	{
		m_GameClientView.m_btPrompt[3].EnableWindow(TRUE);
	}

	//同花按钮
	m_GameLogic.SearchSameColorType(m_bHandCardData[GetMeChairID()], m_bHandCardCount, 5, &m_SearchCardResult);
	if (m_SearchCardResult.cbSearchCount > 0)
	{
		m_GameClientView.m_btPrompt[4].EnableWindow(TRUE);
	}

	//葫芦按钮
	m_GameLogic.SearchTakeCardType(m_bHandCardData[GetMeChairID()], m_bHandCardCount, 3, 2, &m_SearchCardResult);
	if (m_SearchCardResult.cbSearchCount > 0)
	{
		m_GameClientView.m_btPrompt[5].EnableWindow(TRUE);
	}

	//铁支按钮
	m_GameLogic.SearchTakeCardType(m_bHandCardData[GetMeChairID()], m_bHandCardCount, 4, 1, &m_SearchCardResult);
	if (m_SearchCardResult.cbSearchCount > 0)
	{
		m_GameClientView.m_btPrompt[6].EnableWindow(TRUE);
	}

	//同花顺按钮
	m_GameLogic.SearchSameColorLineType(m_bHandCardData[GetMeChairID()], m_bHandCardCount, 5, &m_SearchCardResult);
	if (m_SearchCardResult.cbSearchCount > 0)
	{
		m_GameClientView.m_btPrompt[7].EnableWindow(TRUE);
	}
}

//更新下注按钮
VOID CGameClientEngine::UpdateChipButtonState(int nShowCmd, BOOL bEnable)
{
	for (UINT i = 0; i < 4; i++)
	{
		m_GameClientView.m_btSetChip[i].ShowWindow(nShowCmd);
		m_GameClientView.m_btSetChip[i].EnableWindow(bEnable);
	}
}

//更新按钮
void CGameClientEngine::UpdateBlockButton(BYTE cbBtnIndex, int nShowCmd, BOOL bEnable)
{
	if (0 == cbBtnIndex)
	{
		m_GameClientView.m_btFrontCard.ShowWindow(nShowCmd);
		m_GameClientView.m_btFrontCard.EnableWindow(bEnable);
		m_GameClientView.m_btMidCard.ShowWindow(nShowCmd);
		m_GameClientView.m_btMidCard.EnableWindow(bEnable);
		m_GameClientView.m_btBackCard.ShowWindow(nShowCmd);
		m_GameClientView.m_btBackCard.EnableWindow(bEnable);
	}

	if(1 == cbBtnIndex)
	{
		m_GameClientView.m_btFrontCard.ShowWindow(nShowCmd);
		m_GameClientView.m_btFrontCard.EnableWindow(bEnable);
	}

	if(2 == cbBtnIndex)
	{
		m_GameClientView.m_btMidCard.ShowWindow(nShowCmd);
		m_GameClientView.m_btMidCard.EnableWindow(bEnable);
	}

	if(3 == cbBtnIndex)
	{
		m_GameClientView.m_btBackCard.ShowWindow(nShowCmd);
		m_GameClientView.m_btBackCard.EnableWindow(bEnable);
	}
}

//读取短语
LPCTSTR CGameClientEngine::GetPhraseString(BYTE bGender,BYTE bType,BYTE bIndex)
{
	//获取当前程序路径
	TCHAR szExePath[MAX_PATH] = {};
	GetModuleFileName( NULL,szExePath,CountArray(szExePath) );

	CString szTemp;
	szTemp = szExePath;
	int nPos = szTemp.ReverseFind(TEXT('\\'));
	_sntprintf(szExePath, CountArray(szExePath), TEXT("%s"), szTemp.Left(nPos));

	//获取配置文件
	TCHAR szConfigFileName[MAX_PATH]=TEXT("");
	_sntprintf( szConfigFileName,CountArray(szConfigFileName),TEXT("%s\\PhraseVoice.ini"),szExePath );

	//加载短语
	TCHAR szConfigFileCount[MAX_PATH]=TEXT("");
	INT nPhraseCount = GetPrivateProfileInt( TEXT("PhraseInfo"),szConfigFileCount,0,szConfigFileName );
	TCHAR szItemName[MAX_PATH]=TEXT("");
	TCHAR szType[MAX_PATH]=TEXT("");
	TCHAR szBuffer[MAX_PATH]=TEXT("");

	//子项名
	_sntprintf( szItemName,CountArray(szItemName),TEXT("Phrase%ld"),bIndex);

	if(bGender == 1)
	{
		_sntprintf( szConfigFileCount,CountArray(szConfigFileCount),TEXT("BoyCount%d"),bType );
		_sntprintf( szType,CountArray(szType),TEXT("BoyPhrase%ld"),bType );
	}
	else
	{
		_sntprintf( szConfigFileCount,CountArray(szConfigFileCount),TEXT("GirlCount%d"),bType );
		_sntprintf( szType,CountArray(szType),TEXT("GirlPhrase%ld"),bType );
	}

	//短语字串
	GetPrivateProfileString( szType,szItemName,TEXT(""),szBuffer,CountArray(szBuffer),szConfigFileName );

	return szBuffer;
}

//随机短语
BYTE CGameClientEngine::RandYuYin(BYTE bType,BYTE bGender)
{
	//获取当前程序路径
	TCHAR szExePath[MAX_PATH] = {};
	GetModuleFileName( NULL,szExePath,CountArray(szExePath) );

	CString szTemp;
	szTemp = szExePath;
	int nPos = szTemp.ReverseFind(TEXT('\\'));
	_sntprintf(szExePath, CountArray(szExePath), TEXT("%s"), szTemp.Left(nPos));

	//获取配置文件
	TCHAR szConfigFileName[MAX_PATH] = {};
	_sntprintf( szConfigFileName,CountArray(szConfigFileName),TEXT("%s\\PhraseVoice.ini"),szExePath );

	//加载短语
	TCHAR szConfigFileCount[MAX_PATH]={};
	INT nPhraseCount = GetPrivateProfileInt( TEXT("PhraseInfo"),szConfigFileCount,0,szConfigFileName );
	TCHAR szItemName[256] = {};
	TCHAR szBuffer[MAX_PATH] = {};

	if(bGender == 1)
	{
		_sntprintf( szConfigFileCount,CountArray(szConfigFileCount),TEXT("BoyCount%d"),bType );
		_sntprintf( szItemName,CountArray(szItemName),TEXT("BoyPhrase%ld"),bType );
		nPhraseCount = GetPrivateProfileInt( TEXT("PhraseInfo"),szConfigFileCount,0,szConfigFileName );
	}
	else
	{
		_sntprintf( szConfigFileCount,CountArray(szConfigFileCount),TEXT("GirlCount%d"),bType );
		_sntprintf( szItemName,CountArray(szItemName),TEXT("GirlPhrase%ld"),bType );
		nPhraseCount = GetPrivateProfileInt( TEXT("PhraseInfo"),szConfigFileCount,0,szConfigFileName );
	}

	return rand()%nPhraseCount;
}

int CGameClientEngine::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
	__super::OnCreate(lpCreateStruct) ;



	return 1;
}
