 #include "Stdafx.h"
#include "GameClient.h"
#include "GameOption.h"
#include "GameClientEngine.h"

//////////////////////////////////////////////////////////////////////////

//时间标识
#define IDI_FREE					200									//空闲时间
#define IDI_PLACE_JETTON			201									//下注时间
#define IDI_DISPATCH_CARD			202									//发牌时间
#define IDI_SHOWRESULT				203

#define IDI_ANDROID_BET				204	

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameClientEngine, CGameFrameEngine)
	ON_MESSAGE(IDM_PLACE_JETTON,OnPlaceJetton)
	ON_MESSAGE(IDM_APPLY_BANKER, OnApplyBanker)
	ON_MESSAGE(IDM_ADMIN_COMMDN,OnAdminCommand)
	ON_MESSAGE(IDM_CONTINUE_CARD, OnContinueCard)
	ON_MESSAGE(IDM_ADMIN_STORAGE,OnStorage)
	ON_MESSAGE(IDM_GAME_CLOSE,OnMessageGameClose)
	ON_MESSAGE(IDM_CHAT_MESSAGE,OnChatMessage)
	ON_MESSAGE(IDM_CONTROL_WIN,OnControlWin)
	ON_WM_TIMER()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//构造函数
CGameClientEngine::CGameClientEngine()
{
	//限制信息
	m_lMeMaxScore=0L;			
	m_lAreaLimitScore=0L;		
	m_lApplyBankerCondition=0L;	

	//个人下注
	ZeroMemory(m_lUserBet,sizeof(m_lUserBet));
	ZeroMemory(m_lInitUserScore,sizeof(m_lInitUserScore));
	ZeroMemory(m_lUserJettonScore,sizeof(m_lUserJettonScore));

	//庄家信息
	m_lBankerScore=0L;
	m_wCurrentBanker=0L;
	m_cbLeftCardCount=0;

	//状态变量
	m_bMeApplyBanker=false;

	//机器变量
	m_ListAndroid.RemoveAll();
	m_bCanPlaceJetton=false;
	return;
}

//析构函数
CGameClientEngine::~CGameClientEngine()
{
}

//初始函数
bool CGameClientEngine::OnInitGameEngine()
{
	//设置标题
	SetWindowText(TEXT("百人骰宝  --  Ver：6.6.1.0"));

	//设置图标
	HICON hIcon=LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME));
	m_pIClientKernel->SetGameAttribute(KIND_ID,GAME_PLAYER,VERSION_CLIENT,hIcon,GAME_NAME);
	SetIcon(hIcon,TRUE);
	SetIcon(hIcon,FALSE);
	//加载声音
	//VERIFY(m_DTSDBackground.Create(TEXT("BACK_GROUND")));
	return true;
}

//重置框架
bool CGameClientEngine::OnResetGameEngine()
{
	//限制信息
	m_lMeMaxScore=0L;			

	//个人下注
	ZeroMemory(m_lUserBet,sizeof(m_lUserBet));
	ZeroMemory(m_lUserJettonScore,sizeof(m_lUserJettonScore));

	//庄家信息
	m_lBankerScore=0L;
	m_wCurrentBanker=0L;
	m_cbLeftCardCount=0;

	//状态变量
	m_bMeApplyBanker=false;

	//机器变量
	m_ListAndroid.RemoveAll();

	return true;
}

//游戏设置
void CGameClientEngine::OnGameOptionSet()
{
	return;
}

//时钟删除
bool CGameClientEngine::OnEventGameClockKill(WORD wChairID)
{
	return true;
}

//时间消息
bool CGameClientEngine::OnEventGameClockInfo(WORD wChairID, UINT nElapse, WORD nTimerID)
{
	if ((nTimerID==IDI_PLACE_JETTON)&&(nElapse==0))
	{
		//设置光标
		m_GameClientView.SetCurrentJetton(0L,-1);

		//禁止按钮
		BYTE cbIndex=0;
		for(cbIndex=0;cbIndex<JETTON_COUNT;cbIndex++)
			m_GameClientView.m_btJetton[cbIndex].EnableWindow(FALSE);
	
		//庄家按钮
		m_GameClientView.m_btApplyBanker.EnableWindow( FALSE );
		m_GameClientView.m_btCancelBanker.EnableWindow( FALSE );
	}

	
	if (nTimerID==IDI_PLACE_JETTON&&nElapse<=5) PlayGameSound(AfxGetInstanceHandle(),TEXT("TIME_WARIMG"));
	

	return true;
}

//旁观消息
bool CGameClientEngine::OnEventLookonMode(VOID * pData, WORD wDataSize)
{
	return true;
}

//用户进入
VOID  CGameClientEngine::OnEventUserEnter(IClientUserItem * pIClientUserItem, bool bLookonUser)
{
	//更新用户列表
	UpdateUserList();

	//设置成绩
	if (bLookonUser==false)
	{

		//更新界面
		m_GameClientView.InvalidGameView(0,0,0,0);
	}

	return;
}

//用户离开
VOID  CGameClientEngine::OnEventUserLeave(IClientUserItem * pIClientUserItem, bool bLookonUser)
{


	sUserInfo userInfo;
	userInfo.strUserName=pIClientUserItem->GetNickName();
	userInfo.lUserScore=pIClientUserItem->GetUserScore();
	userInfo.lWinScore=0;

	m_GameClientView.m_UserList.DeleteUser(userInfo);


	//设置成绩
	if (bLookonUser==false)
	{


		WORD wViewChairId = SwitchViewChairID(pIClientUserItem->GetChairID());

		//更新界面
		m_GameClientView.InvalidGameView(0,0,0,0);
	}

	return;
}

//用户状态
VOID  CGameClientEngine::OnEventUserStatus(IClientUserItem * pIClientUserItem, bool bLookonUser)
{
	//变量定义
	IClientUserItem * pIMySelfUserItem=GetMeUserItem();
	BYTE cbUserStatus=pIClientUserItem->GetUserStatus();
	return;
}

//网络消息
bool CGameClientEngine::OnEventGameMessage(WORD wSubCmdID, VOID * pBuffer, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_S_REVOCAT_BET:	//撤销押注
		{
			return OnRevocatBet(pBuffer,wDataSize);
		}
	case SUB_S_GAME_FREE:		//游戏空闲
		{
			return OnSubGameFree(pBuffer,wDataSize);
		}
	case SUB_S_GAME_START:		//游戏开始
		{
			return OnSubGameStart(pBuffer,wDataSize);
		}
	case SUB_S_START_BET:
		{
			return OnUserStartBet(pBuffer,wDataSize);
		}
	case SUB_S_PLACE_JETTON:	//用户加注
		{
			return OnSubPlaceJetton(pBuffer,wDataSize);
		}
	case SUB_S_APPLY_BANKER:	//申请做庄
		{
			return OnSubUserApplyBanker(pBuffer, wDataSize);
		}
	case SUB_S_CANCEL_BANKER:	//取消做庄
		{
			return OnSubUserCancelBanker(pBuffer, wDataSize);
		}
	case SUB_S_CHANGE_BANKER:	//切换庄家
		{
			return OnSubChangeBanker(pBuffer, wDataSize);
		}
	case SUB_S_GAME_END:		//游戏结束
		{
			return OnSubGameEnd(pBuffer,wDataSize);
		}
	case SUB_S_SEND_RECORD:		//游戏记录
		{
			return OnSubGameRecord(pBuffer,wDataSize);
		}
	case SUB_S_PLACE_JETTON_FAIL:	//下注失败
		{
			return OnSubPlaceJettonFail(pBuffer,wDataSize);
		}
	case SUB_S_AMDIN_COMMAND:
		{
			return OnSubReqResult(pBuffer,wDataSize);
		}
	case SUB_S_SEND_USER_BET_INFO:	//更新下注记录
		{
			return OnSubSendUserBetInfo(pBuffer,wDataSize);
		}
	case SUB_S_UPDATE_STORAGE:	//更新库存
		{
			return OnSubUpdateStorage(pBuffer,wDataSize);
		}
	case SUB_S_CONTROL_WIN:
		{
			return OnSubControlWinLose(pBuffer,wDataSize);
		}

	}

	//错误断言
	ASSERT(FALSE);
	return true;
}

//游戏场景
bool CGameClientEngine::OnEventSceneMessage(BYTE cbGameStation, bool bLookonOther, VOID * pBuffer, WORD wDataSize)
{
	//设置显示效果

	switch (cbGameStation)
	{
	case GAME_STATUS_FREE:			//空闲状态
		{
			//效验数据
			ASSERT(wDataSize==sizeof(CMD_S_StatusFree));
			if (wDataSize!=sizeof(CMD_S_StatusFree)) return false;

			//消息处理
			CMD_S_StatusFree * pStatusFree=(CMD_S_StatusFree *)pBuffer;

			

            //玩家信息
			m_lMeMaxScore=pStatusFree->lUserMaxScore;
			m_GameClientView.SetMeMaxScore(m_lMeMaxScore);
			IClientUserItem *pMeUserData=GetTableUserItem(GetMeChairID());
			m_GameClientView.SetMeChairID(SwitchViewChairID(GetMeChairID()));


			//设置时间
			SetGameClock(GetMeChairID(),IDI_FREE,pStatusFree->cbTimeLeave);

            //庄家信息
			SetBankerInfo(pStatusFree->wBankerUser,pStatusFree->lBankerScore);
			m_GameClientView.SetBankerScore(pStatusFree->cbBankerTime,pStatusFree->lBankerWinScore);
			m_bEnableSysBanker=pStatusFree->bEnableSysBanker;
			m_GameClientView.EnableSysBanker(m_bEnableSysBanker);

            //控制信息
			m_lApplyBankerCondition=pStatusFree->lApplyBankerCondition;
			m_lAreaLimitScore=pStatusFree->lAreaLimitScore;
			m_GameClientView.SetAreaLimitScore(m_lAreaLimitScore);

			//设置状态
			SetGameStatus(GAME_STATUS_FREE);
			m_GameClientView.SetGameStatus(GAME_STATUS_FREE);
			//设置状态
			if(CUserRight::IsGameCheatUser(m_pIClientKernel->GetUserAttribute()->dwUserRight)&&m_GameClientView.m_hInst)
				m_GameClientView.m_btOpenAdmin.ShowWindow(SW_SHOW);


			PlayBackGroundSound(AfxGetInstanceHandle(), TEXT("BACK_GROUND"));

			//更新控制
			UpdateButtonContron();
			m_GameClientView.RefreshGameView();

			return true;
		}

	case GS_GAME_START:		//游戏状态
	case GS_PLAYER_BET:
	case GS_GAME_END:		//结束状态
		{
			//效验数据
			ASSERT(wDataSize==sizeof(CMD_S_StatusPlay));
			if (wDataSize!=sizeof(CMD_S_StatusPlay)) return false;

			//消息处理
			CMD_S_StatusPlay * pStatusPlay=(CMD_S_StatusPlay *)pBuffer;

			//下注信息
			for (int nAreaIndex=0; nAreaIndex<AREA_COUNT; ++nAreaIndex)
			{
				m_GameClientView.PlaceUserJetton(nAreaIndex,(LONG)pStatusPlay->lAllJettonScore[nAreaIndex]);
				SetMePlaceJetton(nAreaIndex,(LONG)pStatusPlay->lUserJettonScore[nAreaIndex]);
			}

			//玩家积分
			m_lMeMaxScore=pStatusPlay->lUserMaxScore;			
			m_GameClientView.SetMeMaxScore(m_lMeMaxScore);
			IClientUserItem *pMeUserData=GetTableUserItem(GetMeChairID());
			m_GameClientView.SetMeChairID(SwitchViewChairID(GetMeChairID()));

			//控制信息
			m_lApplyBankerCondition=pStatusPlay->lApplyBankerCondition;
			m_lAreaLimitScore=pStatusPlay->lAreaLimitScore;
			m_GameClientView.SetAreaLimitScore(m_lAreaLimitScore);

			PlayBackGroundSound(AfxGetInstanceHandle(), TEXT("BACK_GROUND"));

			switch(pStatusPlay->cbGameStatus)
			{
				case GS_GAME_END:
					{
						m_GameClientView.m_bRecordWinner=false;
						//设置纸牌
						m_GameClientView.SetDiceValue(pStatusPlay->cbDiceValue);
						//设置成绩
						m_GameClientView.SetCurGameScore(pStatusPlay->lEndUserScore,pStatusPlay->lEndUserReturnScore,pStatusPlay->lEndBankerScore,pStatusPlay->lEndRevenue);
						
						//m_GameClientView.OnTimeGameEnd();
						break;
					}
				case GS_PLAYER_BET:
				case GS_GAME_START:
					{
						break;
					}
				default: break;
			}
			
			//庄家信息
			SetBankerInfo(pStatusPlay->wBankerUser,pStatusPlay->lBankerScore);
			m_GameClientView.SetBankerScore(pStatusPlay->cbBankerTime,pStatusPlay->lBankerWinScore);
			m_bEnableSysBanker=pStatusPlay->bEnableSysBanker;
			m_GameClientView.EnableSysBanker(m_bEnableSysBanker);

			//设置状态
			SetGameStatus(pStatusPlay->cbGameStatus);
			m_GameClientView.SetGameStatus(pStatusPlay->cbGameStatus);
			//if((GetTableUserItem(GetMeChairID())->dwUserRight&UR_GAME_CONTROL)!=0)
			//	m_GameClientView.m_btOpenAdmin.ShowWindow(SW_SHOW);


			//设置时间
			switch(cbGameStation)
			{
				case GS_GAME_START:
					SetGameClock(GetMeChairID(),IDI_DISPATCH_CARD,pStatusPlay->cbTimeLeave);
					break;
				case GS_PLAYER_BET:
					SetGameClock(GetMeChairID(),IDI_PLACE_JETTON,pStatusPlay->cbTimeLeave);
					break;
				case GS_GAME_END:
					SetGameClock(GetMeChairID(),IDI_SHOWRESULT,pStatusPlay->cbTimeLeave);
					break;
				default:break;
			}
			
			//设置状态
			if(CUserRight::IsGameCheatUser(m_pIClientKernel->GetUserAttribute()->dwUserRight)&&m_GameClientView.m_hInst)
				m_GameClientView.m_btOpenAdmin.ShowWindow(SW_SHOW);

			//更新按钮
			UpdateButtonContron();
			m_GameClientView.RefreshGameView();

			return true;
		}
	}

	return false;
}

//游戏开始
bool CGameClientEngine::OnSubGameStart(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_GameStart));
	if (wDataSize!=sizeof(CMD_S_GameStart)) return false;

	//AfxMessageBox(TEXT("1"));
	//消息处理
	CMD_S_GameStart * pGameStart=(CMD_S_GameStart *)pBuffer;

	m_GameClientView.KillCardTime();
	ZeroMemory(m_lUserJettonScore,sizeof(m_lUserJettonScore));

	//AfxMessageBox(TEXT("2"));

	//庄家信息
	SetBankerInfo(pGameStart->wBankerUser,pGameStart->lBankerScore);

	//玩家信息
	m_lMeMaxScore=pGameStart->lUserMaxScore;
	m_GameClientView.SetMeMaxScore(m_lMeMaxScore);

	//AfxMessageBox(TEXT("3"));

	//设置状态
	SetGameStatus(GS_GAME_START);
	m_GameClientView.SetGameStatus(GS_GAME_START);

	//AfxMessageBox(TEXT("4"));

	//机器变量
	m_ListAndroid.RemoveAll();

	//设置时间
	SetGameClock(GetMeChairID(),IDI_DISPATCH_CARD, pGameStart->cbTimeLeave);
	SetTimer(IDI_ANDROID_BET, 100, NULL);
	
	//AfxMessageBox(TEXT("5"));

	m_bCanPlaceJetton=true;

	ReSetBankCtrol(GS_GAME_START);

	m_GameClientView.RefreshGameView();

	return true;
}


bool CGameClientEngine::OnUserStartBet(const void * pBuffer, WORD wDataSize)
{
	CMD_S_StartBet* pStartBet=(CMD_S_StartBet*)pBuffer;
	
	//AfxMessageBox(TEXT("OnUserStartBet 1"));
	
	//设置状态
	SetGameStatus(GS_PLAYER_BET);
	m_GameClientView.SetGameStatus(GS_PLAYER_BET);

	//AfxMessageBox(TEXT("OnUserStartBet 2"));

	//设置时间
	SetGameClock(GetMeChairID(),IDI_PLACE_JETTON, pStartBet->cbTimeLeave);
	//更新按钮
	UpdateButtonContron();
	
	PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_START"));
	

	//AfxMessageBox(TEXT("OnUserStartBet 3"));

	m_GameClientView.RefreshGameView();

	return true;
}


bool CGameClientEngine::OnRevocatBet(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_RevocatBet));
	if (wDataSize!=sizeof(CMD_S_RevocatBet)) return false;

	//消息处理
	CMD_S_RevocatBet* pRevocatBet=(CMD_S_RevocatBet*)pBuffer;
	BYTE cbIndex;
	for(cbIndex=0;cbIndex<AREA_COUNT;cbIndex++)
	{
		if(pRevocatBet->lUserBet[cbIndex]>0)
		{	
			m_GameClientView.PlaceUserJetton(cbIndex,-pRevocatBet->lUserBet[cbIndex],false);
			m_lUserBet[cbIndex]-=pRevocatBet->lUserBet[cbIndex];
		}
	}
	m_GameClientView.RefreshGameView();
	return true;
}

bool CGameClientEngine::OnSubReqResult(const void * pBuffer, WORD wDataSize)
{
	ASSERT(wDataSize==sizeof(CMD_S_CommandResult));
	if(wDataSize!=sizeof(CMD_S_CommandResult)) return false;

	if( NULL != m_GameClientView.m_pClientControlDlg && NULL != m_GameClientView.m_pClientControlDlg->GetSafeHwnd() )
	{
		m_GameClientView.m_pClientControlDlg->ReqResult(pBuffer);
	}
	return true;
}

//游戏空闲
bool CGameClientEngine::OnSubGameFree(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_GameFree));
	if (wDataSize!=sizeof(CMD_S_GameFree)) return false;
	
	m_GameClientView.m_bAnimatFinish=false;
	m_GameClientView.m_bDrawResult=false;
	m_GameClientView.m_cbPrintCount=0;
	
	//AfxMessageBox(TEXT("OnSubGameFree 1"));
	//消息处理
	CMD_S_GameFree * pGameFree=(CMD_S_GameFree *)pBuffer;

	//设置时间
	SetGameClock(GetMeChairID(),IDI_FREE,pGameFree->cbTimeLeave);

	//AfxMessageBox(TEXT("OnSubGameFree 2"));
	//清理时间
	KillTimer(IDI_ANDROID_BET);

	//设置状态
	SetGameStatus(GAME_STATUS_FREE);
	m_GameClientView.SetGameStatus(GAME_STATUS_FREE);

	//AfxMessageBox(TEXT("OnSubGameFree 3"));

	//清理桌面
	bool bWinArea[AREA_COUNT]={false};
	m_GameClientView.SetWinnerSide(bWinArea, false);
	m_GameClientView.CleanUserJetton();
	for (int nAreaIndex=0; nAreaIndex<AREA_COUNT; ++nAreaIndex) 
				SetMePlaceJetton(nAreaIndex,0);


	//更新控件
	UpdateButtonContron();	

	m_GameClientView.RefreshGameView();
	return true;
}

//用户加注
bool CGameClientEngine::OnSubPlaceJetton(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_PlaceJetton));
	if (wDataSize!=sizeof(CMD_S_PlaceJetton)) return false;

	//消息处理
	CMD_S_PlaceJetton * pPlaceJetton=(CMD_S_PlaceJetton *)pBuffer;

	if (GetMeChairID()!=pPlaceJetton->wChairID || IsLookonMode())
	{
		//是否机器人
		if (pPlaceJetton->bIsAndroid)
		{
			//屏蔽机器人下注
	/*		if(CUserRight::IsGameCheatUser(m_pIClientKernel->GetUserAttribute()->dwUserRight))
				return true;*/

			//保存
			static WORD wStFluc = 1;	//随机辅助
			tagAndroidBet androidBet = {};
			androidBet.cbJettonArea = pPlaceJetton->cbJettonArea;
			androidBet.lJettonScore = pPlaceJetton->lJettonScore;
			androidBet.wChairID = pPlaceJetton->wChairID;
			androidBet.nLeftTime = ((rand()+androidBet.wChairID+wStFluc*3)%10+1)*100;
			wStFluc = wStFluc%3 + 1;

			//MyDebug(_T("client::OnSubPlaceJetton 玩家 %d 区域 %d 下注 %I64d 时间 %d]"), androidBet.wChairID,
			//	androidBet.cbJettonArea, androidBet.lJettonScore, androidBet.nLeftTime);

			m_ListAndroid.AddTail(androidBet);
		}
		else
		{
			//加注界面
			m_GameClientView.PlaceUserJetton(pPlaceJetton->cbJettonArea,(LONG)pPlaceJetton->lJettonScore);

			//播放声音
			
			if (pPlaceJetton->wChairID!=GetMeChairID() || IsLookonMode())
			{
				if (pPlaceJetton->lJettonScore==5000000) PlayGameSound(AfxGetInstanceHandle(),TEXT("ADD_GOLD_EX"));
				else PlayGameSound(AfxGetInstanceHandle(),TEXT("ADD_GOLD"));
			}
			
		}
	}

	UpdateButtonContron();
    
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

	m_GameClientView.m_blMoveFinish = false;

	//设置状态
	SetGameStatus(GS_GAME_END);
	m_GameClientView.SetGameStatus(GS_GAME_END);
	m_cbLeftCardCount=pGameEnd->cbLeftCardCount;

	//AfxMessageBox(TEXT("OnSubGameEnd 1"));
	//更新控件
	UpdateButtonContron();
	//设置时间
	SetGameClock(GetMeChairID(),IDI_SHOWRESULT,pGameEnd->cbTimeLeave);

	//显示结果
	m_GameClientView.OnGameEnd(pGameEnd);

	//AfxMessageBox(TEXT("OnSubGameEnd 2"));

	m_GameClientView.RefreshGameView();

	return true;
}

//更新下注信息
bool CGameClientEngine::OnSubSendUserBetInfo(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_SendUserBetInfo));
	if(wDataSize!=sizeof(CMD_S_SendUserBetInfo)) return false;

	//消息处理
	CMD_S_SendUserBetInfo * pSendUserBetInfo=(CMD_S_SendUserBetInfo *)pBuffer;

	for(WORD i=0; i<GAME_PLAYER; i++)
	{
		m_lUserJettonScore[pSendUserBetInfo->cbAreaCount][i] += pSendUserBetInfo->lUserJettonScore[i];
	}	

	//校验权限
	if( NULL == m_GameClientView.m_pClientControlDlg || NULL == m_GameClientView.m_pClientControlDlg->GetSafeHwnd())
	{
		return true;
	}

	CUserBetArray *pUserBetArray = &m_GameClientView.m_pClientControlDlg->m_UserBetArray;
	pUserBetArray->RemoveAll();

	for(WORD i=0; i<GAME_PLAYER; i++)
	{
		IClientUserItem *pClientUserItem=GetTableUserItem(i);

		if(NULL == pClientUserItem) continue;
		if(pClientUserItem->IsAndroidUser()) continue;

		LONGLONG lUserAllBet = 0;
		for(BYTE j=0; j<AREA_COUNT; j++)
		{
			lUserAllBet += m_lUserJettonScore[j][i];
		}
		if(0 == lUserAllBet) continue;

		//构造数据
		tagUserBet UserBet;
		ZeroMemory(&UserBet, sizeof(UserBet));

		CopyMemory(UserBet.szNickName, pClientUserItem->GetNickName(), sizeof(UserBet.szNickName));
		UserBet.dwUserGameID = pClientUserItem->GetGameID();
		UserBet.lUserStartScore = pSendUserBetInfo->lUserStartScore[i];
		UserBet.lUserWinLost = pClientUserItem->GetUserScore() - UserBet.lUserStartScore;

		for(BYTE k=0; k<AREA_COUNT; k++)
		{
			UserBet.lUserBet[k] = m_lUserJettonScore[k][i];
		}

		//插入数据
		pUserBetArray->Add(UserBet);
	}

	//更新控件
	m_GameClientView.m_pClientControlDlg->UpdateUserBet(false);

	return true;
}

//更新库存
bool CGameClientEngine::OnSubUpdateStorage(const void * pBuffer, WORD wDataSize)
{
	ASSERT(wDataSize==sizeof(CMD_S_UpdateStorage));
	if(wDataSize!=sizeof(CMD_S_UpdateStorage)) return false;

	if( NULL != m_GameClientView.m_pClientControlDlg && NULL != m_GameClientView.m_pClientControlDlg->GetSafeHwnd() )
	{
		m_GameClientView.m_pClientControlDlg->UpdateStorage(pBuffer);
	}

	return true;
}

//控制玩家输赢
bool CGameClientEngine::OnSubControlWinLose(const void * pBuffer, WORD wDataSize)
{
	ASSERT(wDataSize==sizeof(CMD_S_ControlWinLose));
	if(wDataSize!=sizeof(CMD_S_ControlWinLose)) return false;

	if( NULL != m_GameClientView.m_pClientControlDlg && NULL != m_GameClientView.m_pClientControlDlg->GetSafeHwnd() )
	{
		m_GameClientView.m_pClientControlDlg->UpdateControlWin(pBuffer);
	}
	return true;
}

void CGameClientEngine::ReSetBankCtrol(int nGameState)
{

	if(!IsLookonMode())
	{
		switch (GetViewStatus())
		{
		case GAME_STATUS_FREE:
			{
				if(GetMeChairID()!=m_wCurrentBanker)
				{
					
					m_GameClientView.m_btBankDraw.EnableWindow(TRUE);
					//m_GameClientView.m_btBankStorage.EnableWindow(TRUE);
				}
				else
				{
					
					m_GameClientView.m_btBankDraw.EnableWindow(TRUE);
					//m_GameClientView.m_btBankStorage.EnableWindow(FALSE);
				}
				break;
			}
		case GS_PLAYER_BET:
		case GS_GAME_END:
		case GS_GAME_START:
			{
				if(GetMeChairID()!=m_wCurrentBanker)
				{
					
					m_GameClientView.m_btBankDraw.EnableWindow(TRUE);
					//m_GameClientView.m_btBankStorage.EnableWindow(FALSE);
				}
				else
				{
					
					m_GameClientView.m_btBankDraw.EnableWindow(FALSE);
					//m_GameClientView.m_btBankStorage.EnableWindow(FALSE);
				}
				break;
			}
		default:break;
		}
	}
	else
	{
		m_GameClientView.m_btBankDraw.EnableWindow(TRUE);
		//m_GameClientView.m_btBankStorage.EnableWindow(TRUE);
	}

}



//更新控制
void CGameClientEngine::UpdateButtonContron()
{
		
	//置能判断
	bool bEnablePlaceJetton=true;
	
	if (IsLookonMode()||GetViewStatus()!=GS_PLAYER_BET||m_wCurrentBanker==GetMeChairID()\
		||(m_bEnableSysBanker==false&&m_wCurrentBanker==INVALID_CHAIR))
	{
		bEnablePlaceJetton=false;
	}

	if (!m_bCanPlaceJetton) bEnablePlaceJetton=false;

	int nIndex=0;
	//下注按钮
	if (bEnablePlaceJetton==true)
	{
		//计算积分
		SCORE lCurrentJetton=m_GameClientView.GetCurrentJetton();
		SCORE lLeaveScore=m_lMeMaxScore;
		for (int nAreaIndex=0; nAreaIndex<AREA_COUNT; ++nAreaIndex)
			lLeaveScore -= m_lUserBet[nAreaIndex];

		//最大下注
		SCORE lUserMaxJetton=m_GameClientView.GetUserMaxBet(3);

		//设置光标
		lLeaveScore = min(lLeaveScore,lUserMaxJetton); //用户可下分 和最大分比较 由于是五倍 
		if (lCurrentJetton>lLeaveScore)
		{
			for(nIndex=JETTON_COUNT-1;nIndex>=0;nIndex--)
			{
				if (lLeaveScore>=g_lScoreType[nIndex])
				{
					m_GameClientView.SetCurrentJetton(g_lScoreType[nIndex],nIndex);
					break;
				}
			}
			if(nIndex<0)
				m_GameClientView.SetCurrentJetton(0L,-1);
		}

		//控制按钮
		for(nIndex=0;nIndex<JETTON_COUNT;nIndex++)
			m_GameClientView.m_btJetton[nIndex].EnableWindow((lLeaveScore>=g_lScoreType[nIndex]*MAX_ODDS&& lUserMaxJetton>=g_lScoreType[nIndex]*MAX_ODDS)?TRUE:FALSE);
		
		if(!m_GameClientView.m_btJetton[0].IsWindowEnabled())
			m_GameClientView.SetFlashArea(0xFF);
	}
	else
	{
		//设置光标
		m_GameClientView.SetCurrentJetton(0L,-1);

		//禁止按钮
		for(nIndex=0;nIndex<JETTON_COUNT;nIndex++)
			m_GameClientView.m_btJetton[nIndex].EnableWindow(FALSE);
	}


	//庄家按钮
	if (!IsLookonMode())
	{
		//获取信息
		IClientUserItem *pMeUserData=GetTableUserItem(GetMeChairID());

		//申请按钮
		bool bEnableApply=true;
		if (m_wCurrentBanker==GetMeChairID()) bEnableApply=false;
		if (m_bMeApplyBanker) bEnableApply=false;
		if (pMeUserData->GetUserScore()<m_lApplyBankerCondition) bEnableApply=false;
		m_GameClientView.m_btApplyBanker.EnableWindow(bEnableApply?TRUE:FALSE);

		//取消按钮
		bool bEnableCancel=true;
		if (m_wCurrentBanker==GetMeChairID() && GetViewStatus()!=GAME_STATUS_FREE) bEnableCancel=false;
		if (m_bMeApplyBanker==false) bEnableCancel=false;
		m_GameClientView.m_btCancelBanker.EnableWindow(bEnableCancel?TRUE:FALSE);
		m_GameClientView.m_btCancelBanker.SetButtonImage(m_wCurrentBanker==GetMeChairID()?IDB_BT_CANCEL_BANKER:IDB_BT_CANCEL_APPLY,AfxGetInstanceHandle(),false,false);

		//显示判断
		if (m_bMeApplyBanker)
		{
			m_GameClientView.m_btCancelBanker.ShowWindow(SW_SHOW);
			m_GameClientView.m_btApplyBanker.ShowWindow(SW_HIDE);
		}
		else
		{
			m_GameClientView.m_btCancelBanker.ShowWindow(SW_HIDE);
			m_GameClientView.m_btApplyBanker.ShowWindow(SW_SHOW);
		}

		//发牌按钮
		if (GetMeChairID()==m_wCurrentBanker)
		{
			BOOL bEnableButton=TRUE;
			if (GetViewStatus()!=GAME_STATUS_FREE) bEnableButton=FALSE;
			if (m_cbLeftCardCount < 8) bEnableButton=FALSE;
		}
	}
	else
	{
		m_GameClientView.m_btCancelBanker.EnableWindow(FALSE);
		m_GameClientView.m_btApplyBanker.EnableWindow(FALSE);
		m_GameClientView.m_btApplyBanker.ShowWindow(SW_SHOW);
		m_GameClientView.m_btCancelBanker.ShowWindow(SW_HIDE);
	}

	if (m_wCurrentBanker==GetMeChairID()) 
		m_GameClientView.m_blCanStore = false;
	else  
		m_GameClientView.m_blCanStore = true;

	m_GameClientView.SetInsureOption(false, m_GameClientView.m_blCanStore);
	ReSetBankCtrol(GetViewStatus());
	return;
}

//加注消息
LRESULT CGameClientEngine::OnPlaceJetton(WPARAM wParam, LPARAM lParam)
{
	//变量定义
	BYTE cbJettonArea=(BYTE)wParam;
	LONG lJettonScore=(LONG)lParam;

	//合法判断
	ASSERT(cbJettonArea>=0&& cbJettonArea<=AREA_COUNT);
	if (!(cbJettonArea>=0&& cbJettonArea<=AREA_COUNT)) return 0;

	//庄家判断
	if ( GetMeChairID() == m_wCurrentBanker ) return true;

	//状态判断
	if (GetViewStatus()!=GS_PLAYER_BET)
	{
		UpdateButtonContron();
		return true;
	}
	
	//设置变量
	m_lUserBet[cbJettonArea] += lJettonScore;
	m_GameClientView.SetMePlaceJetton(cbJettonArea, (LONG)m_lUserBet[cbJettonArea]);

	//变量定义
	CMD_C_PlaceJetton PlaceJetton;
	ZeroMemory(&PlaceJetton,sizeof(PlaceJetton));

	//构造变量
	PlaceJetton.cbJettonArea=cbJettonArea;
	PlaceJetton.lJettonScore=lJettonScore;

	//发送消息
	SendSocketData(SUB_C_PLACE_JETTON,&PlaceJetton,sizeof(PlaceJetton));

	//更新按钮
	UpdateButtonContron();

	//预先显示
	m_GameClientView.PlaceUserJetton(cbJettonArea,lJettonScore);

	//播放声音
	{
		if (lJettonScore==g_lScoreType[JETTON_COUNT-1]) PlayGameSound(AfxGetInstanceHandle(),TEXT("ADD_GOLD_EX"));
		else PlayGameSound(AfxGetInstanceHandle(),TEXT("ADD_GOLD"));
	}
	return 0;
}

//继续发牌
LRESULT CGameClientEngine::OnContinueCard(WPARAM wParam, LPARAM lParam)
{
	//合法判断
	if (GetMeChairID()!=m_wCurrentBanker) return 0;
	if (GetViewStatus()!=GAME_STATUS_FREE) return 0;
	if (m_cbLeftCardCount < 8) return 0;
	if (IsLookonMode()) return 0;

	//发送消息
	SendSocketData(SUB_C_CONTINUE_CARD);

	//设置按钮
//	m_GameClientView.m_btContinueCard.EnableWindow(FALSE);

	return 0;
}

//申请消息
LRESULT CGameClientEngine::OnApplyBanker(WPARAM wParam, LPARAM lParam)
{
	//合法判断
	IClientUserItem *pMeUserData = GetTableUserItem( GetMeChairID() );
	//转换变量
	bool bApplyBanker = wParam ? true:false;
	if (pMeUserData->GetUserScore() < m_lApplyBankerCondition && bApplyBanker) return true;
	//旁观判断
	if (IsLookonMode()) return true;
	//当前判断
	if (m_wCurrentBanker == GetMeChairID() && bApplyBanker) return true;
	//发送消息
	SendSocketData(bApplyBanker?SUB_C_APPLY_BANKER:SUB_C_CANCEL_BANKER, NULL, 0);
	//设置按钮
	UpdateButtonContron();
	return true;
}

//申请做庄
bool CGameClientEngine::OnSubUserApplyBanker(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_ApplyBanker));
	if (wDataSize!=sizeof(CMD_S_ApplyBanker)) return false;

	//消息处理
	CMD_S_ApplyBanker * pApplyBanker=(CMD_S_ApplyBanker *)pBuffer;

	//获取玩家
	IClientUserItem *pUserData=GetTableUserItem(pApplyBanker->wApplyUser);

	//插入玩家
	if (m_wCurrentBanker != pApplyBanker->wApplyUser)
	{
		m_GameClientView.m_ValleysList.Add( SwitchViewChairID(pApplyBanker->wApplyUser) );
	}

	//自己判断
	if (IsLookonMode()==false && GetMeChairID()==pApplyBanker->wApplyUser) m_bMeApplyBanker=true;

	//更新控件
	UpdateButtonContron();

	return true;
}

//取消做庄
bool CGameClientEngine::OnSubUserCancelBanker(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_CancelBanker));
	if (wDataSize!=sizeof(CMD_S_CancelBanker)) return false;

	//消息处理
	CMD_S_CancelBanker * pCancelBanker=(CMD_S_CancelBanker *)pBuffer;

	//删除玩家
	for( int i = 0; i < m_GameClientView.m_ValleysList.GetCount(); ++i )
	{
		if ( SwitchViewChairID(pCancelBanker->wCancelUser) == m_GameClientView.m_ValleysList[i] )
		{
			m_GameClientView.m_ValleysList.RemoveAt(i);
			break;
		}
	}

	//自己判断
	if (IsLookonMode()==false && pCancelBanker->wCancelUser == GetMeChairID()) m_bMeApplyBanker=false;

	//更新控件
	UpdateButtonContron();

	return true;
}

//切换庄家
bool CGameClientEngine::OnSubChangeBanker(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_ChangeBanker));
	if (wDataSize!=sizeof(CMD_S_ChangeBanker)) return false;

	//消息处理
	CMD_S_ChangeBanker * pChangeBanker=(CMD_S_ChangeBanker *)pBuffer;

	//显示图片
	m_GameClientView.ShowChangeBanker(m_wCurrentBanker!=pChangeBanker->wBankerUser);

	//自己判断
	if (m_wCurrentBanker==GetMeChairID() && IsLookonMode() == false && pChangeBanker->wBankerUser!=GetMeChairID()) 
	{
		m_bMeApplyBanker=false;
	}
	else if (IsLookonMode() == false && pChangeBanker->wBankerUser==GetMeChairID())
	{
		m_bMeApplyBanker=true;
	}

	//庄家信息
	SetBankerInfo(pChangeBanker->wBankerUser,pChangeBanker->lBankerScore);
	m_GameClientView.SetBankerScore(0,0);

	//删除玩家
	if (m_wCurrentBanker != INVALID_CHAIR)
	{
		for(int i = 0; i < m_GameClientView.m_ValleysList.GetCount(); ++i)
		{
			if( SwitchViewChairID(m_wCurrentBanker) == m_GameClientView.m_ValleysList[i] )
			{
				m_GameClientView.m_ValleysList.RemoveAt(i);
				break;
			}
		}
	}

	//更新界面
	UpdateButtonContron();
	m_GameClientView.RefreshGameView();

	return true;
}

//游戏记录
bool CGameClientEngine::OnSubGameRecord(const void * pBuffer, WORD wDataSize)
{
	//效验参数
	ASSERT(wDataSize%sizeof(tagGameRecord)==0);
	if (wDataSize%sizeof(tagGameRecord)!=0) return false;
	//设置记录
	WORD wRecordCount=wDataSize/sizeof(tagGameRecord);
	tagGameRecord*pRecord=(tagGameRecord*)pBuffer;
	for (WORD wIndex=0;wIndex<wRecordCount;wIndex++) 
	{
		m_GameClientView.SetGameHistory(pRecord[wIndex].cbDiceValue);
	}

	return true;
}

//下注失败
bool CGameClientEngine::OnSubPlaceJettonFail(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_PlaceJettonFail));
	if (wDataSize!=sizeof(CMD_S_PlaceJettonFail)) return false;

	//消息处理
	CMD_S_PlaceJettonFail * pPlaceJettonFail=(CMD_S_PlaceJettonFail *)pBuffer;

	//效验参数
	BYTE cbViewIndex=pPlaceJettonFail->lJettonArea;
	ASSERT(cbViewIndex>=0&& cbViewIndex<AREA_COUNT);
	if (!(cbViewIndex>=0&& cbViewIndex<AREA_COUNT)) return true;

	//自己判断
	if (GetMeChairID()==pPlaceJettonFail->wPlaceUser && false==IsLookonMode())
	{
		//加注界面
		m_GameClientView.PlaceUserJetton(pPlaceJettonFail->lJettonArea,-pPlaceJettonFail->lPlaceScore);

		SCORE lJettonCount=pPlaceJettonFail->lPlaceScore;
		//合法校验
		ASSERT(m_lUserBet[cbViewIndex]>=lJettonCount);
		if (lJettonCount>m_lUserBet[cbViewIndex]) return false;

		//设置下注
		m_lUserBet[cbViewIndex]-=lJettonCount;
		m_GameClientView.SetMePlaceJetton(cbViewIndex,(LONG)m_lUserBet[cbViewIndex]);
	}

	return true;
}

//设置庄家
void CGameClientEngine::SetBankerInfo(WORD wBanker,SCORE lScore)
{
	m_wCurrentBanker=wBanker;
	m_lBankerScore=lScore;
	IClientUserItem *pUserData=m_wCurrentBanker==INVALID_CHAIR ? NULL : GetTableUserItem(m_wCurrentBanker);
	DWORD dwBankerUserID = (NULL==pUserData) ? 0 : pUserData->GetUserID();
	m_GameClientView.SetBankerInfo(dwBankerUserID,m_lBankerScore);
}

//个人下注
void CGameClientEngine::SetMePlaceJetton(BYTE cbViewIndex, SCORE lJettonCount)
{
	//合法判断
	ASSERT(cbViewIndex>=0&& cbViewIndex<AREA_COUNT);
	if (!(cbViewIndex>=0&& cbViewIndex<AREA_COUNT)) return;

	//设置变量
	m_lUserBet[cbViewIndex]=lJettonCount;

	//设置界面
	m_GameClientView.SetMePlaceJetton(cbViewIndex,lJettonCount);
}

//聊天消息
LRESULT CGameClientEngine::OnChatMessage( WPARAM wParam, LPARAM lParam )
{
	m_GameClientView.InsertAllChatMessage((LPCTSTR)wParam,int(lParam));

	return 0;
}

//游戏关闭
LRESULT CGameClientEngine::OnMessageGameClose(WPARAM wParam, LPARAM lParam)
{
	if(IsLookonMode() == true)
	{
		AfxGetMainWnd()->PostMessage(WM_CLOSE);
		return 0;
	}
	IClientUserItem * pIClientUserItem=GetTableUserItem(GetMeChairID());
	if(pIClientUserItem!=NULL)
		if(pIClientUserItem->GetUserStatus() == US_PLAYING)
		{
			//提示消息
			CInformation Information(this);
			INT nRes=0;
			nRes=Information.ShowMessageBox(TEXT("您正在游戏中，强行退出将被扣分，确实要强退吗？"),MB_ICONQUESTION|MB_YESNO,0);
			if (nRes!=IDYES)
			{
				return TRUE;
			}
		}
		AfxGetMainWnd()->PostMessage(WM_CLOSE);
		return 0;
}

LRESULT CGameClientEngine::OnAdminCommand(WPARAM wParam,LPARAM lParam)
{
	SendSocketData(SUB_C_AMDIN_COMMAND,(CMD_C_AdminReq*)wParam,sizeof(CMD_C_AdminReq));
	return true;
}

LRESULT CGameClientEngine::OnStorage(WPARAM wParam,LPARAM lParam)
{
	SendSocketData(SUB_C_STORAGE,(CMD_C_FreshStorage*)wParam,sizeof(CMD_C_FreshStorage));
	return true;
}

LRESULT CGameClientEngine::OnControlWin(WPARAM wParam,LPARAM lParam)
{
	SendSocketData(SUB_C_CONTROL_WIN,(CMD_C_ControlWinLose*)wParam,sizeof(CMD_C_ControlWinLose));
	return true;
}

void CGameClientEngine::OnTimer(UINT nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (nIDEvent == IDI_ANDROID_BET)
	{
		//MyDebug(_T("%d"), m_ListAndroid.GetCount());
		if (m_ListAndroid.GetCount() != 0)
		{
			POSITION pos = m_ListAndroid.GetTailPosition();
			POSITION posTmp = pos;

			//遍历下注链表
			while (true)
			{
				if (pos == 0)
					break;

				posTmp = pos;
				tagAndroidBet & androidBet = m_ListAndroid.GetPrev(pos);

				androidBet.nLeftTime -= 100;
				if (androidBet.nLeftTime <= 0)
				{		
					//模拟消息
					CMD_S_PlaceJetton placeJetton = {};
					placeJetton.bIsAndroid = false;
					placeJetton.cbJettonArea = androidBet.cbJettonArea;
					placeJetton.lJettonScore = androidBet.lJettonScore;
					placeJetton.wChairID = androidBet.wChairID;

					//MyDebug(_T("client::OnTimer 玩家 %d 区域 %d 下注 %I64d"), androidBet.wChairID, androidBet.cbJettonArea, androidBet.lJettonScore);

					OnSubPlaceJetton((void*)&placeJetton, sizeof(placeJetton));

					//删除元素
					m_ListAndroid.RemoveAt(posTmp);
				}
			}
		}
	}

	CGameFrameEngine::OnTimer(nIDEvent);
}

//声音控制
bool CGameClientEngine::AllowBackGroundSound(bool bAllowSound)
{
	if(bAllowSound)
		PlayBackGroundSound(AfxGetInstanceHandle(), TEXT("BACK_GROUND"));
	else
		StopSound();

	return true;
}

BYTE CGameClientEngine::GetViewStatus()
{
	return m_GameClientView.GetClientViewStatus();

}

//更新用户列表
void CGameClientEngine::UpdateUserList()
{

	for (int i=0;i<GAME_PLAYER;i++)
	{
		IClientUserItem *pClientUserItem=GetTableUserItem(i);
		if (pClientUserItem==NULL) 
		{
			m_lInitUserScore[i]=0;
			continue;
		}

		tagUserInfo * pUserInfo=pClientUserItem->GetUserInfo();
		//图标索引
		UINT uImageIndex=INDEX_NORMAL;

		if ((pClientUserItem->GetMasterOrder()>0)) 
		{
			uImageIndex=INDEX_MASTER+(pUserInfo->cbMasterOrder-1);
		}

		if (pClientUserItem->GetMemberOrder()>0) 
			uImageIndex=INDEX_MEMBER+(pUserInfo->cbMemberOrder-1);

		sUserInfo userInfo;

		userInfo.strUserName=pClientUserItem->GetNickName();
		userInfo.lUserScore=pClientUserItem->GetUserScore();
		userInfo.lWinScore=0;
		userInfo.nImageIndex=uImageIndex;

		if (m_GameClientView.m_UserList.FindUser(userInfo.strUserName))
		{
			userInfo.lWinScore=pClientUserItem->GetUserScore()-m_lInitUserScore[i];

		}
		else
		{
			m_lInitUserScore[i]=pClientUserItem->GetUserScore();
		}

		m_GameClientView.m_UserList.InserUser(userInfo);


	}

}
//////////////////////////////////////////////////////////////////////////