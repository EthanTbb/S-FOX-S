 #include "Stdafx.h"
#include "GameClient.h"
#include "GameOption.h"
#include "GameClientEngine.h"
#include ".\GameClientEngine.h"

//////////////////////////////////////////////////////////////////////////

//时间标识
#define IDI_FREE					99									//空闲时间
#define IDI_PLACE_JETTON			100									//下注时间
#define IDI_DISPATCH_CARD			301									//发牌时间
#define IDI_OPEN_CARD				302								    //发牌时间

#define IDI_BET_BACK_GROUND         330
#define IDI_END_BACK_GROUND			331
#define IDI_ANDROID_BET				1000	


//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameClientEngine, CGameFrameEngine)
	ON_MESSAGE(IDM_PLACE_JETTON,OnPlaceJetton)
	ON_MESSAGE(IDM_APPLY_BANKER, OnApplyBanker)
	ON_MESSAGE(IDM_SOUND,OnPlaySound)
	ON_MESSAGE(IDM_ADMIN_COMMDN, OnAdminControl)	
	ON_MESSAGE(IDM_CHAT_MESSAGE,OnChatMessage)
	ON_MESSAGE(IDM_GAME_OPTIONS,OnGameSetting)
	ON_MESSAGE(IDM_UPDATE_STORAGE,OnUpdataStorage)
	ON_MESSAGE(IDM_PLACE_XUYA,OnMessageOperationRenewal)
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
	m_GameEndTime = 0;

	m_blUsing = false;



	//个人下注
	ZeroMemory(m_lUserJettonScore,sizeof(m_lUserJettonScore));
	ZeroMemory(m_lPlayerBetAll,sizeof(m_lPlayerBetAll));

	ZeroMemory(m_lInitUserScore,sizeof(m_lInitUserScore));
	//所有玩家下注
	ZeroMemory(m_lAllPlayBet,sizeof(m_lAllPlayBet));
	ZeroMemory(m_lUserStartScore,sizeof(m_lUserStartScore));
	//庄家信息
	m_lBankerScore=0L;
	m_wCurrentBanker=0L;
	
	//状态变量
	m_bMeApplyBanker=false;

	m_bPlaceEnd = false;
	m_nCurSetBackMusic=0;
	for ( int i = 0; i < AREA_COUNT+1; ++i )
		m_lPlayLastBet[i] = -1;
	return;
}

//析构函数
CGameClientEngine::~CGameClientEngine()
{
}

//初始函数
bool CGameClientEngine::OnInitGameEngine()
{
	CGlobalUnits * m_pGlobalUnits=(CGlobalUnits *)CGlobalUnits::GetInstance();
	ASSERT(m_pGlobalUnits!=NULL);

	m_pGlobalUnits->m_bMuteStatuts=false;
	//m_pGlobalUnits->m_bAllowSound=true;
	//m_pGlobalUnits->m_bAllowBackGroundSound=true;

	//设置图标
	HICON hIcon=LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME));
	m_pIClientKernel->SetGameAttribute(KIND_ID,GAME_PLAYER,VERSION_CLIENT,hIcon,GAME_NAME);

	SetIcon(hIcon,TRUE);
	SetIcon(hIcon,FALSE);
	return true;
}

//重置框架
bool CGameClientEngine::OnResetGameEngine()
{
	//限制信息
	m_lMeMaxScore=0L;			

	//个人下注
	ZeroMemory(m_lUserJettonScore,sizeof(m_lUserJettonScore));
	ZeroMemory(m_lPlayerBetAll,sizeof(m_lPlayerBetAll));
	//所有玩家下注
	ZeroMemory(m_lAllPlayBet,sizeof(m_lAllPlayBet));
	//庄家信息
	m_lBankerScore=0L;
	m_wCurrentBanker=0L;
	//状态变量
	m_bMeApplyBanker=false;
	for ( int i = 0; i < AREA_COUNT+1; ++i )
		m_lPlayLastBet[i] = -1;
	return true;
}

//游戏设置
void CGameClientEngine::OnGameOptionSet()
{
	return;
}

//设置
LRESULT CGameClientEngine::OnGameSetting(WPARAM wParam, LPARAM lParam)
{
	
		CDlgGameOption DlgGameOption;
	if (DlgGameOption.CreateGameOption(NULL,0)==true)
	{
		return 0;
	}
	return 0;
}

//时钟删除
bool CGameClientEngine::OnEventGameClockKill(WORD wChairID)
{
	return true;
}

//旁观状态
bool CGameClientEngine::OnEventLookonMode(VOID * pData, WORD wDataSize)
{
	return true;
}

//时间消息
bool CGameClientEngine::OnEventGameClockInfo(WORD wChairID, UINT nElapse, WORD nTimerID)
{
	if ((nTimerID==IDI_PLACE_JETTON)&&(nElapse==0))
	{
		//设置光标	
		//m_GameClientView.m_btState[6]=enDisabled;
		m_GameClientView.ReflashButton(5);
		m_GameClientView.SetStateTips(enEndBet);

	}
	
	if (nTimerID==IDI_PLACE_JETTON&&nElapse==5) 
	{
		//PlayGameSound(AfxGetInstanceHandle(),TEXT("TIME_WARIMG"));
		m_GameClientView.SetStateTips(enBetWillEnd,false);
	}

	return true;
}

////旁观状态
//void CGameClientEngine::OnEventLookonMode(VOID * pData, WORD wDataSize)
//{
//}

//网络消息
bool CGameClientEngine::OnEventGameMessage(WORD wSubCmdID, VOID * pBuffer, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_S_GAME_FREE:		//游戏空闲
		{
			return OnSubGameFree(pBuffer,wDataSize);
		}
	case SUB_S_GAME_START:		//游戏开始
		{
			return OnSubGameStart(pBuffer,wDataSize);
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
	/*case SUB_S_CHECK_IMAGE:
		{
			return OnSubCheckImageIndex(pBuffer,wDataSize);
		}*/
	case SUB_S_ADMIN_COMMDN:		//设置
		{
			return OnSubAdminControl(pBuffer,wDataSize);
		}
	case SUB_S_WAIT_BANKER:		    //等待上庄
		{
			m_GameClientView.SetStateTips(enWaitBanker,false);
			return true;
		}	
	case SUB_S_UPDATE_STORAGE:	//更新库存
		{
			return OnSubUpdateStorage(pBuffer,wDataSize);
		}
	}

	//错误断言
	ASSERT(FALSE);

	return true;
}

//游戏场景
bool CGameClientEngine::OnEventSceneMessage(BYTE cbGameStation, bool bLookonOther, VOID * pBuffer, WORD wDataSize)
{
	switch (cbGameStation)
	{
	case GAME_STATUS_FREE:			//空闲状态
		{
			//效验数据
			ASSERT(wDataSize==sizeof(CMD_S_StatusFree));
			if (wDataSize!=sizeof(CMD_S_StatusFree)) return false;

			//消息处理
			CMD_S_StatusFree * pStatusFree=(CMD_S_StatusFree *)pBuffer;
			m_bEnableSysBanker=pStatusFree->bEnableSysBanker;
			//禁用银行
			if(pStatusFree->bGenreEducate) m_GameClientView.m_btState[11] = enDisabled;
            //玩家信息
			m_lMeMaxScore=pStatusFree->lUserMaxScore;
			m_GameClientView.SetMeMaxScore(m_lMeMaxScore);
			//IClientUserItem *pMeUserData=GetTableUserItem(GetMeChairID());

			IClientUserItem *pMeUserData=GetTableUserItem(GetMeChairID());

			m_GameClientView.SetMeChairID(pMeUserData->GetUserID());

            //庄家信息
			SetBankerInfo(pStatusFree->wBankerUser,pStatusFree->lBankerScore);
			m_GameClientView.SetBankerScore(pStatusFree->cbBankerTime,pStatusFree->lBankerWinScore);
			m_GameClientView.SetMultule(pStatusFree->nMultiple);
			m_GameClientView.EnableSysBanker(m_bEnableSysBanker);

			if (!m_bEnableSysBanker&&pStatusFree->wBankerUser==INVALID_CHAIR)
			{
				m_GameClientView.SetStateTips(enWaitBanker,false);
			}
            //控制信息
			m_lApplyBankerCondition=pStatusFree->lApplyBankerCondition;
			m_lAreaLimitScore=pStatusFree->lAreaLimitScore;
			m_GameClientView.SetAreaLimitScore(m_lAreaLimitScore);
			
			//房间全名
			CString strAllRoomName=pStatusFree->szRoomTotalName;
			m_GameClientView.SetTotalRoomName(strAllRoomName);
			CopyMemory(m_lUserStartScore,pStatusFree->lUserStartScore,sizeof(m_lUserStartScore));
			//设置状态
			SetGameStatus(GAME_STATUS_FREE);

			//更新控制
			UpdateButtonContron();
			m_GameClientView.RefreshGameView();

			//设置时间
			SetGameClock(GetMeChairID(),IDI_FREE,pStatusFree->cbTimeLeave);
			
			//播放声音
			PlayBackGroundSound(AfxGetInstanceHandle(),TEXT("BACK_GROUND"));
			
			//更新列表
			UpdateUserList();

			//开启
			if(CUserRight::IsGameCheatUser(m_pIClientKernel->GetUserAttribute()->dwUserRight)&&m_GameClientView.m_hInst)
			{
				m_GameClientView.m_btOpenAdmin.ShowWindow(SW_SHOW);
			}
			//m_nCurSetBackMusic=0;
			//AllowBackGroundSound(false);

			return true;
		}
	case GS_PLACE_JETTON:	//游戏状态
	case GS_GAME_END:		//结束状态
		{
			//效验数据
			ASSERT(wDataSize==sizeof(CMD_S_StatusPlay));
			if (wDataSize!=sizeof(CMD_S_StatusPlay)) return false;

			
			//消息处理
			CMD_S_StatusPlay * pStatusPlay=(CMD_S_StatusPlay *)pBuffer;
			//禁用银行
			if(pStatusPlay->bGenreEducate) m_GameClientView.m_btState[11] = enDisabled;

			//下注信息
			for (int nAreaIndex=1; nAreaIndex<=AREA_COUNT; ++nAreaIndex)
			{
				m_GameClientView.PlaceUserJetton(nAreaIndex,pStatusPlay->lAllJettonScore[nAreaIndex]*m_GameClientView.m_nMultule);
				SetMePlaceJetton(nAreaIndex,pStatusPlay->lUserJettonScore[nAreaIndex]*m_GameClientView.m_nMultule);
			}

			//玩家积分
			m_lMeMaxScore=pStatusPlay->lUserMaxScore;			
			m_GameClientView.SetMeMaxScore(m_lMeMaxScore);
			IClientUserItem *pMeUserData=GetTableUserItem(GetMeChairID());
			m_GameClientView.SetMeChairID(pMeUserData->GetUserID());
			m_GameClientView.SetMultule(pStatusPlay->nMultiple);

			//控制信息
			m_lApplyBankerCondition=pStatusPlay->lApplyBankerCondition;
			m_lAreaLimitScore=pStatusPlay->lAreaLimitScore;
			m_GameClientView.SetAreaLimitScore(m_lAreaLimitScore);

			if (pStatusPlay->cbGameStatus==GS_GAME_END)
			{
				//扑克信息
				m_GameClientView.SetCardInfo(pStatusPlay->cbTableCard);
				m_GameClientView.FinishDispatchCard(false);

				//设置成绩
				m_GameClientView.SetCurGameScore(pStatusPlay->lEndUserScore,pStatusPlay->lEndUserReturnScore,pStatusPlay->lEndBankerScore,pStatusPlay->lEndRevenue);
			}
			else
			{
				m_GameClientView.SetCardInfo(0);
				m_blUsing = true;
				
			}

			//播放声音
			if(cbGameStation==GS_GAME_END)
			PlayBackGroundSound(AfxGetInstanceHandle(),TEXT("BACK_GROUND_DRAW"));
			//m_nCurSetBackMusic=0;
			//AllowBackGroundSound(false);

			//庄家信息
			SetBankerInfo(pStatusPlay->wBankerUser,pStatusPlay->lBankerScore);
			m_GameClientView.SetBankerScore(pStatusPlay->cbBankerTime,pStatusPlay->lBankerWinScore);
			m_bEnableSysBanker=pStatusPlay->bEnableSysBanker;
			m_GameClientView.EnableSysBanker(m_bEnableSysBanker);
			CopyMemory(m_lUserStartScore,pStatusPlay->lUserStartScore,sizeof(m_lUserStartScore));
			//开启
			if(CUserRight::IsGameCheatUser(m_pIClientKernel->GetUserAttribute()->dwUserRight)&&m_GameClientView.m_hInst)
			{
				m_GameClientView.m_btOpenAdmin.ShowWindow(SW_SHOW);
			}
			

			//设置状态
			SetGameStatus(pStatusPlay->cbGameStatus);

			//设置时间
			UINT nTimerID = pStatusPlay->cbGameStatus==GS_GAME_END ? IDI_OPEN_CARD : IDI_PLACE_JETTON;
			SetGameClock(GetMeChairID(), nTimerID, pStatusPlay->cbTimeLeave);

			//房间全名
			CString strAllRoomName=pStatusPlay->szRoomTotalName;
			m_GameClientView.SetTotalRoomName(strAllRoomName);

			//更新按钮
			UpdateButtonContron();
			m_GameClientView.RefreshGameView();
			//更新列表
			UpdateUserList();
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

	//更新用户列表
	UpdateUserList();
	
	m_bPlaceEnd = false;
	//消息处理
	CMD_S_GameStart * pGameStart=(CMD_S_GameStart *)pBuffer;

	m_GameClientView.KillCardTime();

	m_GameClientView.SetStateTips(enStartBet);

	m_blUsing = true;

	//庄家信息
	SetBankerInfo(pGameStart->wBankerUser,pGameStart->lBankerScore);

	//玩家信息
	m_lMeMaxScore=pGameStart->lUserMaxScore;
	m_GameClientView.SetMeMaxScore(m_lMeMaxScore);

	//设置时间
	SetGameClock(GetMeChairID(),IDI_PLACE_JETTON,pGameStart->cbTimeLeave);

	//设置状态
	SetGameStatus(GS_PLACE_JETTON);
	
	//m_GameClientView.StartRandShowSide();

	SetTimer(IDI_ANDROID_BET, 100, NULL);
	
	//更新控制
	UpdateButtonContron();
	//播放声音
	StopSound();
	PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_START"));


	OnPlaySound(0,0);

//	SetTimer(IDI_BET_BACK_GROUND, 18000, NULL);
	
	return true;
}

//游戏空闲
bool CGameClientEngine::OnSubGameFree(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_GameFree));
	if (wDataSize!=sizeof(CMD_S_GameFree)) return false;

	//KillTimer(IDI_END_BACK_GROUND);
	
	//消息处理
	CMD_S_GameFree * pGameFree=(CMD_S_GameFree *)pBuffer;

	m_blUsing = true;

	//更新用户列表
	UpdateUserList();

	//设置时间
	SetGameClock(GetMeChairID(),IDI_FREE,pGameFree->cbTimeLeave);

	//m_GameClientView.StartMove();
	//保存上一次
	for( int i = 0; i < AREA_COUNT+1; ++i )
	{
		if ( m_lUserJettonScore[i] > 0 )
		{
			CopyMemory(m_lPlayLastBet, m_lUserJettonScore, sizeof(m_lUserJettonScore));
			break;
		}
	}
	//设置状态
	SetGameStatus(GAME_STATUS_FREE);

	//播放声音
	StopSound();
	PlayBackGroundSound(AfxGetInstanceHandle(),TEXT("BACK_GROUND_BET"));

	if(CUserRight::IsGameCheatUser(m_pIClientKernel->GetUserAttribute()->dwUserRight)&&m_GameClientView.m_hInst)
	{

		m_GameClientView.m_pAdminControl->SetStorageCurrentValue(pGameFree->lStorageCurrent);

		CString cs;
		cs.Format(TEXT("当前库存：%I64d"),pGameFree->lStorageCurrent);	
		ChatMsg msg;
		ZeroMemory(&msg,sizeof(ChatMsg));
		msg.pszString = cs.GetBuffer(0);
		m_GameClientView.InsertAllChatMessage(&msg,enNormal);
		
	/*	if(m_pIStringMessage!=NULL)
			m_pIStringMessage->InsertSystemString(cs);*/
	}
	//清理时间
	KillTimer(IDI_ANDROID_BET);

	////清理桌面
	bool blWin[AREA_COUNT];
	for (int i = 0;i<AREA_COUNT;i++){

			blWin[i]=false;
	}

	m_GameClientView.FinishDispatchCard();

	m_GameClientView.SetWinnerSide(blWin, false);



	for (int nAreaIndex=1; nAreaIndex<=AREA_COUNT; ++nAreaIndex) SetMePlaceJetton(nAreaIndex,0);

	m_GameClientView.ResetGameView();
	//所有玩家下注
	ZeroMemory(m_lAllPlayBet,sizeof(m_lAllPlayBet));
	//更新控件
	UpdateButtonContron();

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
	LONGLONG lJettonScore = pPlaceJetton->lJettonScore*m_GameClientView.m_nMultule;
	 m_lAllPlayBet[pPlaceJetton->wChairID][pPlaceJetton->cbJettonArea]+=lJettonScore;

	 if(pPlaceJetton->cbAndroid == FALSE)
	 {
		 //更新控件
		 m_GameClientView.ControlBetDetection(pPlaceJetton->cbJettonArea,lJettonScore,pPlaceJetton->wChairID);

	 }

	 //管理判断
	 if(CUserRight::IsGameCheatUser(m_pIClientKernel->GetUserAttribute()->dwUserRight)&&m_GameClientView.m_hInst)
	 {
		 IClientUserItem *pClientUserItem=GetTableUserItem(pPlaceJetton->wChairID);
		 CUserBetArray *pUserBetArray = &m_GameClientView.m_pAdminControl->m_UserBetArray;

		for(int i=0;i<pUserBetArray->GetCount();i++)
		{
			if(0==lstrcmp(pClientUserItem->GetNickName(),pUserBetArray->GetAt(i).szNickName))
			{
				pUserBetArray->GetAt(i).lUserBet[pPlaceJetton->cbJettonArea-1]+=lJettonScore;
				m_GameClientView.m_pAdminControl->UpdateUserList();
				break;
			}
		}
	 }

	if (GetMeChairID()!=pPlaceJetton->wChairID || IsLookonMode())
	{
	
		m_GameClientView.PlaceUserJetton(pPlaceJetton->cbJettonArea,lJettonScore);

		//播放声音
		
		if (pPlaceJetton->wChairID!=GetMeChairID() || IsLookonMode())
		{
			if (pPlaceJetton->lJettonScore>=5000000) 
			{
				PlayGameSound(AfxGetInstanceHandle(),TEXT("ADD_GOLD_EX"));
				//CString strInfo;
				//strInfo.Format(TEXT("机器人下注超过500w"));

				//NcaTextOut(strInfo);
			}
			else 
				PlayGameSound(AfxGetInstanceHandle(),TEXT("ADD_GOLD"));
		}

		m_GameClientView.RefreshGameView();
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

	//m_nCurSetBackMusic=1;
	//AllowBackGroundSound(true);

	//StopSound();
//	KillTimer(IDI_BET_BACK_GROUND);

	PlayGameSound(AfxGetInstanceHandle(),TEXT("SND_STARTSHOWCARD"));

	//消息处理
	CMD_S_GameEnd * pGameEnd=(CMD_S_GameEnd *)pBuffer;

	//m_GameClientView.m_blMoveFinish = false;

	
	m_GameEndTime = pGameEnd->cbTimeLeave;

	m_blUsing = true;

	//设置时间
	SetGameClock(GetMeChairID(),IDI_DISPATCH_CARD, pGameEnd->cbTimeLeave);

	//扑克信息
	m_GameClientView.SetCardInfo(pGameEnd->cbTableCard);

	m_GameClientView.ClearAreaFlash();
	
	if(pGameEnd->cbTableCard==1||pGameEnd->cbTableCard==5
		||pGameEnd->cbTableCard==9||pGameEnd->cbTableCard==13)
	{
		m_GameClientView.FlashAllArea();
	}
	else
		m_GameClientView.StartRunCar(20);

	
	//设置状态
	SetGameStatus(GS_GAME_END);
	StopSound();
	PlayBackGroundSound(AfxGetInstanceHandle(),TEXT("BACK_GROUND_DRAW"));

	//庄家信息
	m_GameClientView.SetBankerScore(pGameEnd->nBankerTime, pGameEnd->lBankerTotallScore);
	//成绩信息
	m_GameClientView.SetCurGameScore(pGameEnd->lUserScore,pGameEnd->lUserReturnScore,pGameEnd->lBankerScore,pGameEnd->lRevenue);

	m_GameClientView.PerformAllBetAnimation();
	
	//更新控件
	UpdateButtonContron();

	////停止声音
	//for (int i=0; i<CountArray(m_DTSDCheer); ++i) m_DTSDCheer[i].Stop();

	return true;
}

//更新控制
void CGameClientEngine::UpdateButtonContron()
{
	//置能判断
	bool bEnablePlaceJetton=true;

	if(m_wCurrentBanker==INVALID_CHAIR)
	{
		bEnablePlaceJetton = true;

	}
	if (GetGameStatus()!=GS_PLACE_JETTON)
	{
		bEnablePlaceJetton=false;
	}
	if (m_wCurrentBanker==GetMeChairID()) 
	{
		bEnablePlaceJetton=false;
	}
	if (IsLookonMode())
	{
		bEnablePlaceJetton=false;
	}
	if (m_bEnableSysBanker==false&&m_wCurrentBanker==INVALID_CHAIR) 
	{
		bEnablePlaceJetton=false;
	}

	//下注按钮
	if (bEnablePlaceJetton)
	{
		
		//计算积分
		LONGLONG lCurrentJetton=m_GameClientView.GetCurrentJetton();
		LONGLONG lLeaveScore=m_lMeMaxScore;
		for (int nAreaIndex=1; nAreaIndex<=AREA_COUNT; ++nAreaIndex) lLeaveScore -= m_lUserJettonScore[nAreaIndex];

		//最大下注
		LONGLONG lUserMaxJetton = 0;

		for (int nAreaIndex=1; nAreaIndex<=AREA_COUNT; ++nAreaIndex)
		{
			if(lUserMaxJetton==0&&nAreaIndex == 1)
			{
				lUserMaxJetton = m_GameClientView.GetUserMaxJetton(nAreaIndex);

			}else
			{
				if(m_GameClientView.GetUserMaxJetton(nAreaIndex)>lUserMaxJetton)
				{
					lUserMaxJetton = m_GameClientView.GetUserMaxJetton(nAreaIndex);
				}
			}
		}
		lLeaveScore = min((lLeaveScore),lUserMaxJetton); //用户可下分 和最大分比较 由于是五倍 

// 		lLeaveScore = lLeaveScore*m_GameClientView.m_nMultule;
// 		lUserMaxJetton*=m_GameClientView.m_nMultule;
		//控制按钮
		int iTimer = 1;

		if(m_blUsing==false)
		{
			lLeaveScore = 0;
			lUserMaxJetton = 0;

		}

		(lLeaveScore>=100*iTimer && lUserMaxJetton>=100*iTimer)?m_GameClientView.m_btState[3]=enNormal:m_GameClientView.m_btState[3]=enDisabled;
		(lLeaveScore>=1000*iTimer && lUserMaxJetton>=1000*iTimer)?m_GameClientView.m_btState[4]=enNormal:m_GameClientView.m_btState[4]=enDisabled;
		(lLeaveScore>=10000*iTimer && lUserMaxJetton>=10000*iTimer)?m_GameClientView.m_btState[5]=enNormal:m_GameClientView.m_btState[5]=enDisabled;
		(lLeaveScore>=100000*iTimer && lUserMaxJetton>=100000*iTimer)?m_GameClientView.m_btState[6]=enNormal:m_GameClientView.m_btState[6]=enDisabled;
		(lLeaveScore>=1000000*iTimer && lUserMaxJetton>=1000000*iTimer)?m_GameClientView.m_btState[7]=enNormal:m_GameClientView.m_btState[7]=enDisabled;
		(lLeaveScore>=5000000*iTimer && lUserMaxJetton>=5000000*iTimer)?m_GameClientView.m_btState[8]=enNormal:m_GameClientView.m_btState[8]=enDisabled;
		(lLeaveScore>=10000000*iTimer && lUserMaxJetton>=1000000*iTimer)?m_GameClientView.m_btState[9]=enNormal:m_GameClientView.m_btState[9]=enDisabled;

		//续投按钮  有下注的话就不能进行续投
		bool bEnable = m_lPlayLastBet[0]!=-1;
		for(int i=0;i<AREA_COUNT+1;i++)
		{
			if(m_lUserJettonScore[i]>0) 
			{
				bEnable=false;
				break;
			}
		}

		bEnable?m_GameClientView.m_btState[10]=enNormal:m_GameClientView.m_btState[10]=enDisabled;


		m_GameClientView.ReflashButton(0,true);
		
		//减少筹码
		if (lCurrentJetton>lLeaveScore)
		{
			m_GameClientView.ReduceJettonNumber();
		}
	}
	else
	{
		//设置光标
		m_GameClientView.SetCurrentJetton(0L);

		//禁止按钮
		for (int i=3;i<11;i++)
		{
			m_GameClientView.m_btState[i]=enDisabled;
		}
		m_GameClientView.ReflashButton(0,true);
	}

	//庄家按钮
	if (!IsLookonMode() && m_GameClientView.m_nSwitchIndex==0)
	{
		//获取信息
		IClientUserItem *pMeUserData=GetTableUserItem(GetMeChairID());

		//申请按钮
		bool bEnableApply=true;
		if (m_wCurrentBanker==GetMeChairID()) bEnableApply=false;
		if (m_bMeApplyBanker) bEnableApply=false;
		if (pMeUserData->GetUserScore()*m_GameClientView.m_nMultule<m_lApplyBankerCondition) bEnableApply=false;

		m_GameClientView.m_btState[13]=bEnableApply?enNormal:enDisabled;

		//取消按钮
		bool bEnableCancel=true;
		if (m_wCurrentBanker==GetMeChairID() && GetGameStatus()!=GAME_STATUS_FREE) bEnableCancel=false;
		if (m_bMeApplyBanker==false) bEnableCancel=false;
		m_GameClientView.m_btState[14]=bEnableCancel?enNormal:enDisabled;
		

		//显示判断
		if (m_bMeApplyBanker)
		{
			m_GameClientView.m_bBtShow[13]=false;
			m_GameClientView.m_bBtShow[14]=true;
		}
		else
		{
			m_GameClientView.m_bBtShow[13]=true;
			m_GameClientView.m_bBtShow[14]=false;
		}

		m_GameClientView.ReflashButton(13);
	}
	else
	{
 		m_GameClientView.m_bBtShow[13]=false;
 		m_GameClientView.m_bBtShow[14]=false;
	}
	
	m_GameClientView.RefreshGameView();
	//获取信息
	IClientUserItem *pMeUserData=GetTableUserItem(GetMeChairID());

	
	m_GameClientView.SetInsureOption(false, false);

	ReSetBankCtrol(GetGameStatus());
	m_GameClientView.UpdateButtonContron();

	return;
}

//加注消息
LRESULT CGameClientEngine::OnPlaceJetton(WPARAM wParam, LPARAM lParam)
{
	//变量定义
	BYTE cbJettonArea=(BYTE)wParam;
	LONGLONG lJettonScore=(LONGLONG)(lParam);

	//合法判断
	ASSERT(cbJettonArea>=1 && cbJettonArea<=AREA_COUNT);
	if (!(cbJettonArea>=1 && cbJettonArea<=AREA_COUNT)) return 0;

	//庄家判断
	if ( GetMeChairID() == m_wCurrentBanker ) return true;

	//状态判断
	if (GetGameStatus()!=GS_PLACE_JETTON)
	{
		UpdateButtonContron();
		return true;
	}

	//设置变量
	m_lUserJettonScore[cbJettonArea] += lJettonScore;
	m_GameClientView.SetMePlaceJetton(cbJettonArea, m_lUserJettonScore[cbJettonArea]);

	//变量定义
	CMD_C_PlaceJetton PlaceJetton;
	ZeroMemory(&PlaceJetton,sizeof(PlaceJetton));

	//构造变量
	PlaceJetton.cbJettonArea=cbJettonArea;
	PlaceJetton.lJettonScore=lJettonScore;

	//发送消息
	if(!SendSocketData(SUB_C_PLACE_JETTON,&PlaceJetton,sizeof(PlaceJetton)))
	{
		return 0;
	};

	//更新按钮
	UpdateButtonContron();

	//预先显示
	m_GameClientView.PlaceUserJetton(cbJettonArea,lJettonScore);

	//播放声音
	//if (IsEnableSound()) 
	{
		if (lJettonScore==5000000) PlayGameSound(AfxGetInstanceHandle(),TEXT("ADD_GOLD_EX"));
		else PlayGameSound(AfxGetInstanceHandle(),TEXT("ADD_GOLD"));
	}

	return 0;
}


//自己最大下分
LONGLONG CGameClientEngine::GetMeMaxBet( BYTE cbArea )
{
	//找出玩家
	IClientUserItem * pUserData = GetTableUserItem(GetMeChairID());
	if ( pUserData == NULL || cbArea >= AREA_COUNT ) 
		return 0l;

	//玩家分数
	LONGLONG lUserScore = pUserData->GetUserScore();

	//减去自己已下注
	for(int i = 0; i <=AREA_COUNT; ++i)
	{
		lUserScore -= m_lUserJettonScore[i];
	}

	//如果是区域下注. 还要判断区域限制
	if ( cbArea <=AREA_COUNT )
	{
		//区域总限制
		LONGLONG lAreaLimit = m_lAreaLimitScore - m_lPlayerBetAll[cbArea];
		lUserScore = __min( lUserScore, lAreaLimit);
	}

	//异常错误限制
	ASSERT( lUserScore >= 0l );
	if( lUserScore < 0 )
		lUserScore = 0;

	return lUserScore;
}

LRESULT  CGameClientEngine::OnPlaySound(WPARAM wParam, LPARAM lParam)
{
	
	if(lParam == 0)
	{
		PlayGameSound(AfxGetInstanceHandle(),TEXT("PLEASEJETTONWAV"));
	}
	else if(lParam==1)
	{
		//PlayGameSound(AfxGetInstanceHandle(),TEXT("LEAVEHANDLEWAV"));
	}
	else if(lParam==3)
	{
		PlayGameSound(AfxGetInstanceHandle(),TEXT("IDC_SNDWAV"));
	}
	else if(lParam==4)
	{
		PlayGameSound(AfxGetInstanceHandle(),TEXT("END_LOST"));
	}
	else if(lParam==5)
	{
		PlayGameSound(AfxGetInstanceHandle(),TEXT("END_WIN"));
	}
	else if(lParam==6)
	{
		PlayGameSound(AfxGetInstanceHandle(),TEXT("END_DRAW"));
	}
	else if(lParam==7)
	{
		PlayGameSound(AfxGetInstanceHandle(),TEXT("ADD_GOLD"));
	}
	else if(lParam==8)
	{
// 		PlayGameSound(AfxGetInstanceHandle(),TEXT("SND_FLASH"));
// 
// 		SetTimer(IDI_END_BACK_GROUND,4000,NULL);

	}
	else if(lParam==9)
	{
		PlayGameSound(AfxGetInstanceHandle(),TEXT("SND_SELGRID_BIGWARN"));
	}	
	return 1;
		
}

//申请消息
LRESULT CGameClientEngine::OnApplyBanker(WPARAM wParam, LPARAM lParam)
{
	//合法判断
	IClientUserItem * pClientUserItem=GetTableUserItem(GetMeChairID());

	if (pClientUserItem->GetUserScore()*m_GameClientView.m_nMultule < m_lApplyBankerCondition) return true;

	//旁观判断
	if (IsLookonMode()) return true;

	//转换变量
	bool bApplyBanker = wParam ? true:false;

	//当前判断
	if (m_wCurrentBanker == GetMeChairID() && bApplyBanker) return true;

	if (bApplyBanker)
	{
		//发送消息
		SendSocketData(SUB_C_APPLY_BANKER, NULL, 0);

		m_bMeApplyBanker=true;
	}
	else
	{
		//发送消息
		SendSocketData(SUB_C_CANCEL_BANKER, NULL, 0);

		m_bMeApplyBanker=false;
	}

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
	//tagUserData const *pUserData=GetTableUserItem(pApplyBanker->wApplyUser);
	IClientUserItem * pClientUserItem=GetTableUserItem(pApplyBanker->wApplyUser);
	//插入玩家
	if (m_wCurrentBanker!=pApplyBanker->wApplyUser)
	{
		m_GameClientView.m_ValleysList.Add(SwitchViewChairID(pApplyBanker->wApplyUser) );
		if(m_GameClientView.m_ValleysList.GetCount()-m_GameClientView.m_nShowValleyIndex>3)
		{
			m_GameClientView.m_nShowValleyIndex = m_GameClientView.m_ValleysList.GetCount()-3;
		}
	}

	//自己判断
	if (IsLookonMode()==false && GetMeChairID()==pApplyBanker->wApplyUser) m_bMeApplyBanker=true;

	//更新控件
	UpdateButtonContron();
	//m_GameClientView.m_btCancelBanker.EnableWindow(TRUE);

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
	//删除玩家
	for( int i = 0; i < m_GameClientView.m_ValleysList.GetCount(); ++i )
	{
		if (  SwitchViewChairID(pCancelBanker->wCancelUser) == m_GameClientView.m_ValleysList[i] )
		{
			m_GameClientView.m_ValleysList.RemoveAt(i);
			if(/*i<m_GameClientView.m_nShowValleyIndex && */m_GameClientView.m_nShowValleyIndex>0) 
				m_GameClientView.m_nShowValleyIndex--;
			if( m_GameClientView.m_ValleysList.GetCount()<=3)  m_GameClientView.m_nShowValleyIndex=0;
			break;
		}
	}

	//自己判断
	IClientUserItem *pMeUserData=GetTableUserItem(GetMeChairID());
	if (IsLookonMode()==false && pMeUserData->GetUserID()==pCancelBanker->wCancelUser) m_bMeApplyBanker=false;

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
	//m_GameClientView.ShowChangeBanker(m_wCurrentBanker!=pChangeBanker->wBankerUser);

	m_GameClientView.SetStateTips(enChangeBanker);

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
	if (m_wCurrentBanker!=INVALID_CHAIR)
	{
		for(int i = 0; i < m_GameClientView.m_ValleysList.GetCount(); ++i)
		{
			if(SwitchViewChairID(m_wCurrentBanker)== m_GameClientView.m_ValleysList[i] )
			{
				m_GameClientView.m_ValleysList.RemoveAt(i);
				if(m_GameClientView.m_nShowValleyIndex>0) m_GameClientView.m_nShowValleyIndex--;
				if( m_GameClientView.m_ValleysList.GetCount()<=3)  m_GameClientView.m_nShowValleyIndex=0;
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
	ASSERT(wDataSize%sizeof(tagServerGameRecord)==0);
	if (wDataSize%sizeof(tagServerGameRecord)!=0) return false;

	//结果变量
	//tagGameRecord GameRecord;
	//ZeroMemory(&GameRecord,sizeof(GameRecord));

	//设置记录
	WORD wRecordCount=wDataSize/sizeof(tagServerGameRecord);
	for (WORD wIndex=0;wIndex<wRecordCount;wIndex++) 
	{
		tagServerGameRecord * pServerGameRecord=(((tagServerGameRecord *)pBuffer)+wIndex);

		m_GameClientView.SetGameHistory(pServerGameRecord->cbAnimal);
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
	ASSERT(cbViewIndex>=1 && cbViewIndex<=AREA_COUNT);
	if (!(cbViewIndex>=1 && cbViewIndex<=AREA_COUNT)) return false;

	//加注界面
	m_GameClientView.PlaceUserJetton(pPlaceJettonFail->lJettonArea,-pPlaceJettonFail->lPlaceScore*m_GameClientView.m_nMultule);

	//自己判断
	if (GetMeChairID()==pPlaceJettonFail->wPlaceUser && false==IsLookonMode())
	{
		LONGLONG lJettonCount=pPlaceJettonFail->lPlaceScore*m_GameClientView.m_nMultule;
		//合法校验
		ASSERT(m_lUserJettonScore[cbViewIndex]>=lJettonCount);
		if (lJettonCount>m_lUserJettonScore[cbViewIndex]) return false;

		//设置下注
		m_lUserJettonScore[cbViewIndex]-=lJettonCount;
		m_GameClientView.SetMePlaceJetton(cbViewIndex,m_lUserJettonScore[cbViewIndex]);
	}


	return true;
}

//设置庄家
void CGameClientEngine::SetBankerInfo(WORD wBanker,LONGLONG lScore)
{
	m_wCurrentBanker=wBanker;
	m_lBankerScore=lScore;
	//tagUserData const *pUserData=m_wCurrentBanker==INVALID_CHAIR ? NULL : GetTableUserItem(m_wCurrentBanker);
	IClientUserItem * pClientUserItem=m_wCurrentBanker==INVALID_CHAIR ? NULL : GetTableUserItem(m_wCurrentBanker);
	DWORD dwBankerUserID = (NULL==pClientUserItem) ? 0 : pClientUserItem->GetUserID();
	m_GameClientView.SetBankerInfo(dwBankerUserID,m_lBankerScore);

	//if (IsLookonMode()==false && GetMeChairID()==m_wCurrentBanker) m_bMeApplyBanker=true;
	//else m_bMeApplyBanker = false;
	//UpdateButtonContron();

}

//个人下注
void CGameClientEngine::SetMePlaceJetton(BYTE cbViewIndex, LONGLONG lJettonCount)
{
	//合法判断
	ASSERT(cbViewIndex>=1 && cbViewIndex<=AREA_COUNT);
	if (!(cbViewIndex>=1 && cbViewIndex<=AREA_COUNT)) return;

	//设置变量
	m_lUserJettonScore[cbViewIndex]=lJettonCount;

	//设置界面
	m_GameClientView.SetMePlaceJetton(cbViewIndex,lJettonCount);
}
//////////////////////////////////////////////////////////////////////////
//控制
LRESULT CGameClientEngine::OnAdminControl( WPARAM wParam, LPARAM lParam )
{
	SendSocketData(SUB_C_ADMIN_COMMDN,(CMD_C_ControlApplication*)wParam,sizeof(CMD_C_ControlApplication));
	return true;
}

//库存控制
LRESULT CGameClientEngine::OnUpdataStorage(WPARAM wParam, LPARAM lParam)
{
	SendSocketData(SUB_C_UPDATE_STORAGE,(CMD_C_UpdateStorage*)wParam,sizeof(CMD_C_UpdateStorage));
	return true;
}

//聊天
LRESULT CGameClientEngine::OnChatMessage( WPARAM wParam, LPARAM lParam )
{
//	m_GameClientView.InsertAllChatMessage((ChatMsg *)wParam,int(lParam));
	return true;
}

//控制
bool CGameClientEngine::OnSubAdminControl( const void * pBuffer, WORD wDataSize )
{
	ASSERT(wDataSize==sizeof(CMD_S_ControlReturns));
	if(wDataSize!=sizeof(CMD_S_ControlReturns)) return false;

	if(m_GameClientView.m_pAdminControl!=NULL)
	{
		CMD_S_ControlReturns* pResult = (CMD_S_ControlReturns*)pBuffer;
		m_GameClientView.m_pAdminControl->UpdateControl(pResult);
	}
	return true;
}

void CGameClientEngine::OnTimer(UINT nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
// 	if(nIDEvent == IDI_BET_BACK_GROUND)
// 	{
// 		PlayGameSound(AfxGetInstanceHandle(),TEXT("BACK_GROUND"));
// 	}

// 	if(nIDEvent == IDI_END_BACK_GROUND)
// 	{
// 		PlayGameSound(AfxGetInstanceHandle(),TEXT("SND_FLASH"));
// 
// 		KillTimer(IDI_END_BACK_GROUND);
// 	}
// 	
	
	
	if (nIDEvent == IDI_ANDROID_BET)
	{
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
					placeJetton.cbAndroid = FALSE;
					//placeJetton.cbJettonArea = androidBet.cbJettonArea;
					//placeJetton.lJettonScore = androidBet.lJettonScore;
					placeJetton.wChairID = androidBet.wChairID;

					OnSubPlaceJetton((void*)&placeJetton, sizeof(placeJetton));

					//删除元素
					m_ListAndroid.RemoveAt(posTmp);
				}
			}
		}
	}
	CGameFrameEngine::OnTimer(nIDEvent);
}


void CGameClientEngine::ReSetBankCtrol(int nGameState)
{

	if(!IsLookonMode())
	{
		switch (GetGameStatus())
		{
		case GAME_STATUS_FREE:
			{
				if(GetMeChairID()!=m_wCurrentBanker)
				{					
					//m_GameClientView.m_btBankDraw.EnableWindow(TRUE);
					//m_GameClientView.m_btBankStorage.EnableWindow(TRUE);
				}
				else
				{					
					//m_GameClientView.m_btBankDraw.EnableWindow(TRUE);
					//m_GameClientView.m_btBankStorage.EnableWindow(FALSE);
				}
				break;
			}
		case GS_GAME_END:
		case GS_PLACE_JETTON:
		case GS_MOVECARD_END:
			{
				if(GetMeChairID()!=m_wCurrentBanker)
				{
					//m_GameClientView.m_btBankDraw.EnableWindow(TRUE);
					//m_GameClientView.m_btBankStorage.EnableWindow(FALSE);
				}
				else
				{
					//m_GameClientView.m_btBankDraw.EnableWindow(FALSE);
					//m_GameClientView.m_btBankStorage.EnableWindow(FALSE);
				}				
				break;
			}
		default:break;
		}
	}
	else
	{
		//m_GameClientView.m_btBankDraw.EnableWindow(TRUE);
		//m_GameClientView.m_btBankStorage.EnableWindow(TRUE);
	}

}

//用户进入
VOID  CGameClientEngine::OnEventUserEnter(IClientUserItem * pIClientUserItem, bool bLookonUser)
{
	m_lUserStartScore[pIClientUserItem->GetChairID()] = pIClientUserItem->GetUserScore();
	//更新用户列表
	UpdateUserList();
	//设置成绩
	if (bLookonUser==false)
	{		
		//更新界面
		m_GameClientView.RefreshGameView();
	}

	return;
}

//用户离开
VOID  CGameClientEngine::OnEventUserLeave(IClientUserItem * pIClientUserItem, bool bLookonUser)
{
	m_lUserStartScore[pIClientUserItem->GetChairID()] = 0;
	UpdateUserList(true);
	
	//设置成绩
	if (bLookonUser==false)
	{
		WORD wViewChairId =pIClientUserItem->GetChairID();
		
		//更新界面
		m_GameClientView.RefreshGameView();
	}
	//管理判断
	if(CUserRight::IsGameCheatUser(m_pIClientKernel->GetUserAttribute()->dwUserRight)&&m_GameClientView.m_hInst)
	{
		CUserBetArray *pUserBetArray = &m_GameClientView.m_pAdminControl->m_UserBetArray;
		for(int i=0;i<pUserBetArray->GetCount();i++)
		{
			if(0==lstrcmp(pIClientUserItem->GetNickName(),pUserBetArray->GetAt(i).szNickName))
			{
				bool bDel = false;
				if(pUserBetArray->GetAt(i).bAttention==true) bDel = true;
				pUserBetArray->RemoveAt(i);
				m_GameClientView.m_pAdminControl->UpdateUserList(bDel);
				break;
			}
		}
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

//选择事件
VOID  CGameClientEngine::OnEventPhraseSelect(INT nPhraseIndex)
{
	
}

//声音控制
bool CGameClientEngine::AllowBackGroundSound(bool bAllowSound)
{
	if(bAllowSound)
	{
		BYTE cbGameStatus = GetGameStatus();
		if(cbGameStatus == GAME_STATUS_FREE)	PlayBackGroundSound(AfxGetInstanceHandle(),TEXT("BACK_GROUND_BET"));
		else if(cbGameStatus == GS_GAME_END)	PlayBackGroundSound(AfxGetInstanceHandle(),TEXT("BACK_GROUND_DRAW"));
	}
	else
		StopSound();

	return true;
}

//更新用户列表
void CGameClientEngine::UpdateUserList(bool Refresh)
{
	if(Refresh)	
	{
		m_GameClientView.m_UserList.ClearAll();
	}

	
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
		//文本颜色
		COLORREF rgb = RGB(255,255,255);
		if ((pClientUserItem->GetMasterOrder()>0)) 
		{
			uImageIndex=INDEX_MASTER+(pUserInfo->cbMasterOrder-1);
			rgb = COLOR_MASTER_TX;
		}

		if (pClientUserItem->GetMemberOrder()>0) 
		{
			uImageIndex=INDEX_MEMBER+(pUserInfo->cbMemberOrder-1);
			if(CUserRight::IsGameCheatUser(m_pIClientKernel->GetUserAttribute()->dwUserRight))
				rgb = COLOR_MEMBER_TX;
			else
				rgb = COLOR_MASTER_TX;
		}
			

		sUserInfo userInfo;

		userInfo.strUserName=pClientUserItem->GetNickName();
		userInfo.lUserScore=pClientUserItem->GetUserScore()/**m_GameClientView.m_nMultule*/;
		userInfo.lWinScore=0;
		userInfo.nImageIndex=uImageIndex;
		bool bHave = false;
		if (m_GameClientView.m_UserList.FindUser(userInfo.strUserName))
		{
			userInfo.lWinScore=(pClientUserItem->GetUserScore()-m_lInitUserScore[i])*m_GameClientView.m_nMultule;
			bHave=true;
		}	
		else
		{
			m_lInitUserScore[i]=pClientUserItem->GetUserScore();
		}
		//管理判断
		if(CUserRight::IsGameCheatUser(m_pIClientKernel->GetUserAttribute()->dwUserRight)&&m_GameClientView.m_hInst)
		{			
			if(Refresh==false)
			{
				CUserBetArray *pUserBetArray = &m_GameClientView.m_pAdminControl->m_UserBetArray;

				if(bHave==false)
				{
					//构造数据
					tagUserBet UserBet;
					ZeroMemory(&UserBet, sizeof(UserBet));

					CopyMemory(UserBet.szNickName, pClientUserItem->GetNickName(), sizeof(UserBet.szNickName));
					UserBet.dwUserGameID = pClientUserItem->GetGameID();
					UserBet.lUserStartScore =   pClientUserItem->GetUserScore();
					UserBet.bAttention = false;
					UserBet.lUserInsure = pClientUserItem->GetUserInsure();
					UserBet.bAndroid = pClientUserItem->IsAndroidUser();
					UserBet.lUserWinLost = (pClientUserItem->GetUserScore()-m_lUserStartScore[i])*m_GameClientView.m_nMultule;
					//插入数据
					pUserBetArray->Add(UserBet);
				}
				else
				{
					//更新信息
					for(int j=0;j<pUserBetArray->GetCount();j++)
					{
						if(0==lstrcmp(pClientUserItem->GetNickName(),pUserBetArray->GetAt(j).szNickName))
						{
							pUserBetArray->GetAt(j).lUserStartScore= pClientUserItem->GetUserScore();
							pUserBetArray->GetAt(j).lUserInsure = pClientUserItem->GetUserInsure();
							pUserBetArray->GetAt(j).lUserWinLost = (pClientUserItem->GetUserScore()-m_lUserStartScore[i])*m_GameClientView.m_nMultule;
							break;
						}
					}
				}

				//刷新列表
				m_GameClientView.m_pAdminControl->UpdateUserList();

			}

			if (pClientUserItem->IsAndroidUser())
			{       
				m_GameClientView.m_UserList.InserUser(userInfo,true);
				continue;
			}
		}
		m_GameClientView.m_UserList.InserUser(userInfo);
 	}
	
}


//更新库存
bool CGameClientEngine::OnSubUpdateStorage(const void * pBuffer, WORD wDataSize)
{
	ASSERT(wDataSize==sizeof(CMD_S_UpdateStorage));
	if(wDataSize!=sizeof(CMD_S_UpdateStorage)) return false;

	if( NULL != m_GameClientView.m_pAdminControl && NULL != m_GameClientView.m_pAdminControl->GetSafeHwnd() )
	{
		m_GameClientView.m_pAdminControl->UpdateStorage(pBuffer);
	}

	return true;
}


//续压消息
LRESULT CGameClientEngine::OnMessageOperationRenewal(WPARAM wParam, LPARAM lParam)
{

	//第一局
	if ( m_lPlayLastBet[0] == -1 )
		return 0;
	//庄家判断
	if ( GetMeChairID() == m_wCurrentBanker ) return 0;

	//状态判断
	if (GetGameStatus()!=GS_PLACE_JETTON)
	{
		UpdateButtonContron();
		return 0;
	}

	//判断是否够下注
	LONGLONG lPlayChip = 0;
	bool bCanBet = true;
	for ( int i = 0; i <=AREA_COUNT; ++i )
	{
		//设置变量
		lPlayChip += m_lPlayLastBet[i];
		//该局有没有进行下注 有下注就不能续投
		if(m_lUserJettonScore[i]>0)
		{
			bCanBet = false;
			break;
		}
	}
	if(!bCanBet) 
	{
		UpdateButtonContron();
		return 0;
	}

	IClientUserItem *pMeUserData =  GetMeUserItem();
	if(pMeUserData == NULL) return 0;

	if( lPlayChip>pMeUserData->GetUserScore()*m_GameClientView.m_nMultule )
	{
		CInformation Information(this);
		Information.ShowMessageBox(TEXT("您的金币不足，无法续压上轮下注！"));
		return 0;
	}

	//SendSocketData(SUB_C_CLEAR_JETTON);
	//清空下注
	for (int i = 0; i <= AREA_COUNT; ++i)
	{
		m_lPlayerBetAll[i]-=m_lUserJettonScore[i];
		m_lAllPlayBet[GetMeChairID()][i]=0;
		m_lUserJettonScore[i]=0;
	}
	
	//重新下注
	for ( int i = 1; i <= AREA_COUNT; ++i )
	{
		//设置变量
		if(m_lPlayLastBet[i] > 0)
		{
			m_lUserJettonScore[i] += m_lPlayLastBet[i];
			m_GameClientView.SetMePlaceJetton(i, m_lUserJettonScore[i]);
			
			//发送消息
			CMD_C_PlaceJetton PlaceJetton;
			ZeroMemory(&PlaceJetton,sizeof(PlaceJetton));

			PlaceJetton.cbJettonArea=i;
			PlaceJetton.lJettonScore=m_lPlayLastBet[i];
			if(SendSocketData(SUB_C_PLACE_JETTON,&PlaceJetton,sizeof(PlaceJetton)) == false)
			{
				return 0;
			}
			//预先显示
			m_GameClientView.PlaceUserJetton(i,m_lUserJettonScore[i]);

		}
	}

	PlayGameSound(AfxGetInstanceHandle(),TEXT("ADD_GOLD"));
	UpdateButtonContron();
	return 0;
}
