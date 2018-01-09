 #include "Stdafx.h"
#include "GameClient.h"
#include "GameClientEngine.h"
#include ".\GameClientEngine.h"

//////////////////////////////////////////////////////////////////////////

//时间标识
#define IDI_FREE					99									//空闲时间
#define IDI_PLACE_JETTON			100									//下注时间
#define IDI_DISPATCH_CARD			301									//发牌时间
#define IDI_PLACE_JETTON_BUFFER		302									//发牌时间

//控制时间
#define IDI_START_GAME				200								
#ifdef _DEBUG
#define TIME_START_GAME				15
#else
#define TIME_START_GAME				30
#endif


//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameClientEngine, CGameFrameEngine)
	ON_MESSAGE(IDM_PLACE_JETTON,OnPlaceJetton)
	ON_MESSAGE(IDM_APPLY_BANKER, OnApplyBanker)
	ON_MESSAGE(IDM_CONTINUE_CARD, OnContinueCard)
	ON_MESSAGE(IDM_AUTO_OPEN_CARD, OnAutoOpenCard)	
	ON_MESSAGE(IDM_OPEN_SOUND, OnOpenSound)	
	ON_MESSAGE(IDM_CLOSE_SOUND, OnCloseSound)	
	ON_MESSAGE(IDM_POSTCARD_SOUND,OnPostCardWav)
	ON_MESSAGE(IDM_OPEN_CARD, OnOpenCard)	
	ON_MESSAGE(IDM_MANAGE_CONTROL, OnManageControl)
	ON_MESSAGE(IDM_ADMIN_COMMDN,OnAdminCommand)
	ON_MESSAGE(IDM_START,OnStart)
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

	m_bBackGroundSound = false;

	//个人下注
	ZeroMemory(m_lUserJettonScore,sizeof(m_lUserJettonScore));

	//庄家信息
	m_lBankerScore=0L;
	m_wCurrentBanker=0L;
	m_cbLeftCardCount=0;

	//状态变量
	m_bMeApplyBanker=false;
	m_bCanPlaceJetton=false;
	m_GameClientView.m_bBankerMax=false;
	m_lUserLimitScore = 0L;
	return;
}

//析构函数
CGameClientEngine::~CGameClientEngine()
{
}
//自动搓牌
LRESULT  CGameClientEngine::OnOpenSound(WPARAM wParam, LPARAM lParam)
{
	
	return 1;

}
//自动搓牌
LRESULT  CGameClientEngine::OnCloseSound(WPARAM wParam, LPARAM lParam)
{
	return 1;

}
//初始函数
bool CGameClientEngine::OnInitGameEngine()
{
	//设置标题
	SetWindowText(TEXT("两张游戏  --  Ver：6.6.1.0"));

	//设置图标
	HICON hIcon=LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME));
	m_pIClientKernel->SetGameAttribute(KIND_ID,GAME_PLAYER,VERSION_CLIENT,hIcon,GAME_NAME);
	SetIcon(hIcon,TRUE);
	SetIcon(hIcon,FALSE);
	//播放声音
	CGlobalUnits *pGlobalUnits=CGlobalUnits::GetInstance();
	if ( pGlobalUnits->m_bAllowBackGroundSound && !pGlobalUnits->m_bMuteStatuts)
	{
		m_bBackGroundSound = true;
		PlayBackGroundSound(AfxGetInstanceHandle(), TEXT("BACK_GROUND"));
	}

	if(0)
	{
		BYTE cbCardData[2]={1,2};
		for (int i=0; i<CountArray(m_GameClientView.m_CardControl); ++i) 
			m_GameClientView.m_CardControl[i].SetCardData(cbCardData,2);

		m_GameClientView.SetMoveCardTimer();
		SetGameStatus(GS_GAME_END);

		tagApplyUser ApplyUser;
		ApplyUser.strUserName = "33333";
		ApplyUser.lUserScore = 1234567;

		m_GameClientView.m_ApplyUser.InserUser(ApplyUser);
		ApplyUser.strUserName = "4435435";
		ApplyUser.lUserScore = 4200000000;
		m_GameClientView.m_ApplyUser.InserUser(ApplyUser);

		ApplyUser.strUserName = "57755";
		ApplyUser.lUserScore = 5300000000;
		m_GameClientView.m_ApplyUser.InserUser(ApplyUser);
		ApplyUser.strUserName = "643565665";
		ApplyUser.lUserScore = 6400000000;
		m_GameClientView.m_ApplyUser.InserUser(ApplyUser);
		ApplyUser.strUserName = "753565665";
		ApplyUser.lUserScore = 7500000000;

		m_GameClientView.m_ApplyUser.InserUser(ApplyUser);
		ApplyUser.strUserName = "863565665";
		ApplyUser.lUserScore = 8600000000;
		m_GameClientView.m_ApplyUser.InserUser(ApplyUser);
		ApplyUser.strUserName = "973565665";
		ApplyUser.lUserScore = 9700000000;
		m_GameClientView.m_ApplyUser.InserUser(ApplyUser);
		m_GameClientView.m_ApplyUser.InserUser(ApplyUser);


		//m_GameClientView.m_ApplyUser.InserUser(ApplyUser);
		if(m_GameClientView.m_ApplyUser.GetItemCount()>4)
		{
			//m_GameClientView.m_btUp.ShowWindow(SW_SHOW);
			m_GameClientView.m_btUp.EnableWindow(true);
			//m_GameClientView.m_btDown.ShowWindow(SW_SHOW);
			m_GameClientView.m_btDown.EnableWindow(true);  
		}
		else
		{
			m_GameClientView.m_btUp.ShowWindow(SW_HIDE);
			m_GameClientView.m_btUp.EnableWindow(true);
			m_GameClientView.m_btDown.ShowWindow(SW_HIDE);
			m_GameClientView.m_btDown.EnableWindow(true); 

			m_GameClientView.m_ApplyUser.m_AppyUserList.SendMessage(WM_VSCROLL, MAKELONG(SB_TOP,0),NULL);
			m_GameClientView.m_ApplyUser.m_AppyUserList.Invalidate(TRUE);
		}


	}
	
	m_PlaceJettonArray.RemoveAll();
	
	return true;
}

//重置框架
bool CGameClientEngine::OnResetGameEngine()
{
	//限制信息
	m_lMeMaxScore=0L;			

	//个人下注
	ZeroMemory(m_lUserJettonScore,sizeof(m_lUserJettonScore));

	//庄家信息
	m_lBankerScore=0L;
	m_wCurrentBanker=0L;
	m_cbLeftCardCount=0;

	//状态变量
	m_bMeApplyBanker=false;

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
	if(nTimerID == 0) return true;

	if ((nTimerID==IDI_PLACE_JETTON)&&(nElapse==0))
	{
		//设置光标
		m_GameClientView.SetCurrentJetton(0L);

		//禁止按钮
		m_GameClientView.m_btJetton100.EnableWindow(FALSE);		
		m_GameClientView.m_btJetton1000.EnableWindow(FALSE);		
		m_GameClientView.m_btJetton10000.EnableWindow(FALSE);	
		m_GameClientView.m_btJetton100000.EnableWindow(FALSE);
		m_GameClientView.m_btJetton500000.EnableWindow(FALSE);
		m_GameClientView.m_btJetton1000000.EnableWindow(FALSE);
		m_GameClientView.m_btJetton5000000.EnableWindow(FALSE);
		m_GameClientView.m_btJetton10000000.EnableWindow(FALSE);

		//庄家按钮
		m_GameClientView.m_btApplyBanker.EnableWindow( FALSE );
		m_GameClientView.m_btCancelBanker.EnableWindow( FALSE );
	}
    else if(nElapse==0&&nTimerID==IDI_START_GAME&&IsLookonMode()==false)
	{
		//关闭时间
		KillGameClock(IDI_START_GAME);

		//发送消息
		SendUserReady(NULL,0);

		//隐藏按钮
		//m_GameClientView.m_btStart.ShowWindow(SW_HIDE);
	}
	
	
	if (nTimerID==IDI_PLACE_JETTON&&nElapse<=5) PlayGameSound(AfxGetInstanceHandle(),TEXT("TIME_WARIMG"));
	

	return true;
}

//旁观消息
bool CGameClientEngine::OnEventLookonMode(VOID * pData, WORD wDataSize)
{
	return true;
}

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
			while(m_PlaceJettonArray.GetCount()>0)
			{
				CMD_S_PlaceJetton &PlaceJetton=m_PlaceJettonArray[0];
				PlaceJetton.cbAndroidUser=FALSE;
				OnSubPlaceJetton(&PlaceJetton,sizeof(PlaceJetton));
				m_PlaceJettonArray.RemoveAt(0);
			}
			KillTimer(IDI_PLACE_JETTON_BUFFER);
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
	case SUB_S_AMDIN_COMMAND_RESULT:
		{
			//效验数据
			ASSERT(wDataSize==sizeof(CMD_S_AdminControl));
			if (wDataSize!=sizeof(CMD_S_AdminControl)) return false;

			//消息处理
			CMD_S_AdminControl * pAdminControl=(CMD_S_AdminControl *)pBuffer;
			if ( pAdminControl->cbControlType == 0 )
			{
				AfxMessageBox(TEXT("设置异常失败！"));
			}
			else if ( pAdminControl->cbControlType == 1 )
			{
				AfxMessageBox(TEXT("取消控制！"));
			}
			else if ( pAdminControl->cbControlType == 2 )
			{
				AfxMessageBox(TEXT("设置控制成功，仅当局有效！"));
			}
			return true;
		}
	case SUB_S_TO_BANKER_MAX:
		{
			m_GameClientView.m_bBankerMax=true;
			m_GameClientView.m_bToBankerMax = true;
			m_GameClientView.m_stuCartoonInfo[CTN_BANKER_MAX].nShowFrame = 0;
			m_GameClientView.m_stuCartoonInfo[CTN_BANKER_MAX].dwShowTime = GetTickCount();
			m_GameClientView.m_stuCartoonInfo[CTN_BANKER_MAX].bShow = true;
			if(m_pIStringMessage!=NULL)
			{
				//m_pIStringMessage->InsertSystemString(TEXT("本局押注达到庄家上限!"));
				m_GameClientView.m_bShowHalf = true;
			}
			return true;
		}
	case SUB_S_NOTICE_TIME:
		{
			if(GetMeUserItem()->GetUserStatus()!=US_READY)
			{
				if(sizeof(CMD_S_FreeTime)!=wDataSize) return false;
				 
				CMD_S_FreeTime* pTimeInfo = (CMD_S_FreeTime*)pBuffer;

			    SetGameClock(GetMeChairID(),IDI_START_GAME,pTimeInfo->cbTimeLeave);	
			}
			return true;
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
			//明星信息
			m_GameClientView.TransportBackRes(pStatusFree->nRedBack);
			memcpy(m_GameClientView.m_fStarWinRate,pStatusFree->fStarWinRate,sizeof(pStatusFree->fStarWinRate));
			memcpy(m_GameClientView.m_llStarAllCount,pStatusFree->llStarAllCount,sizeof(pStatusFree->llStarAllCount));
			memcpy(m_GameClientView.m_llStarWinCount,pStatusFree->llStarWinCount,sizeof(pStatusFree->llStarWinCount));
			m_GameClientView.m_llAllStarCount = pStatusFree->llAllStarCount;

            //玩家信息
			m_lMeMaxScore=pStatusFree->lUserMaxScore;
			m_GameClientView.SetMeMaxScore(m_lMeMaxScore);
			IClientUserItem *pMeUserData=GetTableUserItem(GetMeChairID());
			m_GameClientView.SetMeChairID(pMeUserData->GetUserID());

            //庄家信息
			SetBankerInfo(pStatusFree->wBankerUser,pStatusFree->lBankerScore);
			m_GameClientView.SetBankerScore(pStatusFree->cbBankerTime,pStatusFree->lBankerWinScore);
			m_GameClientView.SetBankerAllResult(pStatusFree->lBankerWinScore);
			m_bEnableSysBanker=pStatusFree->bEnableSysBanker;
			m_GameClientView.EnableSysBanker(m_bEnableSysBanker);

            //控制信息
			m_lApplyBankerCondition=pStatusFree->lApplyBankerCondition;
			m_lAreaLimitScore=pStatusFree->lAreaLimitScore;
			m_GameClientView.SetAreaLimitScore(m_lAreaLimitScore);

			//设置控件
			if (IsLookonMode()==false)
			{
				if(GetMeUserItem()->GetUserStatus()!=US_READY)
				{
					//m_GameClientView.m_btStart.ShowWindow(TRUE);
//					m_GameClientView.m_btStart.SetFocus();

					//设置时间
					if(pStatusFree->wBankerUser!=INVALID_CHAIR)
					{
						SetGameClock(GetMeChairID(),IDI_START_GAME,pStatusFree->cbTimeLeave);
					}
				}

			}

			//设置状态
			SetGameStatus(GAME_STATUS_FREE);

			//设置时间
			//SetGameClock(GetMeChairID(),IDI_FREE,pStatusFree->cbTimeLeave);

			//管理判断
			/*if(CUserRight::IsGameCheatUser(m_pIClientKernel->GetUserAttribute()->dwUserRight)&&m_GameClientView.m_hInst)
			{
				m_GameClientView.m_btOpenAdmin.ShowWindow(SW_SHOW);

				m_GameClientView.m_SkinListCtrl[0].ShowWindow(SW_SHOW);
				m_GameClientView.m_SkinListCtrl[1].ShowWindow(SW_SHOW);
				m_GameClientView.m_SkinListCtrl[2].ShowWindow(SW_SHOW);
			}*/

			//更新控制
			UpdateButtonContron();
			return true;
		}
	case GAME_STATUS_PLAY:		//游戏状态
	case GS_GAME_END:		//结束状态
		{
			//效验数据
			ASSERT(wDataSize==sizeof(CMD_S_StatusPlay));
			if (wDataSize!=sizeof(CMD_S_StatusPlay)) return false;

			//消息处理
			CMD_S_StatusPlay * pStatusPlay=(CMD_S_StatusPlay *)pBuffer;
			//明星信息
			m_GameClientView.TransportBackRes(pStatusPlay->nRedBack);
			memcpy(m_GameClientView.m_fStarWinRate,pStatusPlay->fStarWinRate,sizeof(pStatusPlay->fStarWinRate));
			memcpy(m_GameClientView.m_llStarAllCount,pStatusPlay->llStarAllCount,sizeof(pStatusPlay->llStarAllCount));
			memcpy(m_GameClientView.m_llStarWinCount,pStatusPlay->llStarWinCount,sizeof(pStatusPlay->llStarWinCount));
			m_GameClientView.m_llAllStarCount = pStatusPlay->llAllStarCount;
			memcpy(m_GameClientView.m_cbStarSelect,pStatusPlay->cbStarSelect,sizeof(pStatusPlay->cbStarSelect));

			//下注信息
			for (int nAreaIndex=1; nAreaIndex<=AREA_COUNT; ++nAreaIndex)
			{
				m_GameClientView.PlaceUserJetton(nAreaIndex,pStatusPlay->lAllJettonScore[nAreaIndex]);
				SetMePlaceJetton(nAreaIndex,pStatusPlay->lUserJettonScore[nAreaIndex]);
			}

			//玩家积分
			m_lMeMaxScore=pStatusPlay->lUserMaxScore;			
			m_GameClientView.SetMeMaxScore(m_lMeMaxScore);
			IClientUserItem *pMeUserData=GetTableUserItem(GetMeChairID());
			m_GameClientView.SetMeChairID(pMeUserData->GetUserID());

			//控制信息
			m_lApplyBankerCondition=pStatusPlay->lApplyBankerCondition;
			m_lAreaLimitScore=pStatusPlay->lAreaLimitScore;
			m_GameClientView.SetAreaLimitScore(m_lAreaLimitScore);

			if (pStatusPlay->cbGameStatus==GS_GAME_END)
			{
				//扑克信息
				m_GameClientView.SetCardInfo(pStatusPlay->cbTableCardArray);
				////设置扑克
				for (int i=0; i<CountArray(m_GameClientView.m_CardControl); ++i) m_GameClientView.m_CardControl[i].SetCardData(m_GameClientView.m_cbTableCardArray[i],2);
				m_GameClientView.SetFinishDiscard();

				//设置成绩
				m_GameClientView.SetCurGameScore(pStatusPlay->lEndUserScore,pStatusPlay->lEndUserReturnScore,pStatusPlay->lEndBankerScore,pStatusPlay->lEndRevenue);
			}
			else
			{
				m_GameClientView.SetCardInfo(NULL);

				for (int i = 0;i<5;i++)
				{
					m_GameClientView.m_CardControl[i].m_CardItemArray.SetSize(0);
				}

				
			}

			//庄家信息
			SetBankerInfo(pStatusPlay->wBankerUser,pStatusPlay->lBankerScore);
			m_GameClientView.SetBankerScore(pStatusPlay->cbBankerTime,pStatusPlay->lBankerWinScore);
			m_GameClientView.SetBankerAllResult(pStatusPlay->lBankerWinScore);
			m_bEnableSysBanker=pStatusPlay->bEnableSysBanker;
			m_GameClientView.EnableSysBanker(m_bEnableSysBanker);

			//设置状态
			SetGameStatus(pStatusPlay->cbGameStatus);

			//设置时间
			SetGameClock(GetMeChairID(),(pStatusPlay->cbGameStatus==GS_GAME_END)?IDI_DISPATCH_CARD:IDI_PLACE_JETTON,pStatusPlay->cbTimeLeave);

			//管理判断
			if(CUserRight::IsGameCheatUser(m_pIClientKernel->GetUserAttribute()->dwUserRight)&&m_GameClientView.m_hInst)
			{
				m_GameClientView.m_btOpenAdmin.ShowWindow(SW_SHOW);

				m_GameClientView.m_SkinListCtrl[0].ShowWindow(SW_SHOW);
				m_GameClientView.m_SkinListCtrl[1].ShowWindow(SW_SHOW);
				m_GameClientView.m_SkinListCtrl[2].ShowWindow(SW_SHOW);
			}

			//更新按钮
			UpdateButtonContron();
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

	//消息处理
	CMD_S_GameStart * pGameStart=(CMD_S_GameStart *)pBuffer;
	//明星信息
	m_GameClientView.TransportBackRes(pGameStart->nRedBack);
	memcpy(m_GameClientView.m_fStarWinRate,pGameStart->fStarWinRate,sizeof(pGameStart->fStarWinRate));
	memcpy(m_GameClientView.m_llStarAllCount,pGameStart->llStarAllCount,sizeof(pGameStart->llStarAllCount));
	memcpy(m_GameClientView.m_llStarWinCount,pGameStart->llStarWinCount,sizeof(pGameStart->llStarWinCount));
	m_GameClientView.m_llAllStarCount = pGameStart->llAllStarCount;
	memcpy(m_GameClientView.m_cbStarSelect,pGameStart->cbStarSelect,sizeof(pGameStart->cbStarSelect));
	m_GameClientView.m_bSelectStar = true;
	m_GameClientView.StartShowCardEx(0,m_GameClientView.m_ptFlash[0].x,m_GameClientView.m_ptFlash[0].y,true);	
	m_GameClientView.m_stuCartoonInfo[CTN_STAR_SELECT].nShowFrame = 0;
	m_GameClientView.m_stuCartoonInfo[CTN_STAR_SELECT].dwShowTime = GetTickCount();
	m_GameClientView.m_stuCartoonInfo[CTN_STAR_SELECT].bShow = true;

	//庄家信息
	SetBankerInfo(pGameStart->wBankerUser,pGameStart->lBankerScore);

	//玩家信息
	m_lMeMaxScore=pGameStart->lUserMaxScore;
	m_lUserLimitScore = pGameStart->lUserLimitScore;
	m_GameClientView.SetMeMaxScore(m_lMeMaxScore);
	m_GameClientView.SetUserLimitScore(m_lUserLimitScore);

	//设置状态
	SetGameStatus(GS_PLACE_JETTON);

	//设置时间
	SetGameClock(GetMeChairID(),IDI_PLACE_JETTON,pGameStart->cbTimeLeave);

	m_bCanPlaceJetton=true;
	//更新控制
	UpdateButtonContron();

	//设置提示
	m_GameClientView.SetDispatchCardTip(pGameStart->bContiueCard ? enDispatchCardTip_Continue : enDispatchCardTip_Dispatch);

	//播放声音
	//PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_START"));

	m_PlaceJettonArray.RemoveAll();

	//关闭时间
	KillGameClock(IDI_START_GAME);

	//隐藏开始按钮
	//m_GameClientView.m_btStart.ShowWindow(SW_HIDE);

	return true;
}

//游戏空闲
bool CGameClientEngine::OnSubGameFree(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_GameFree));
	if (wDataSize!=sizeof(CMD_S_GameFree)) return false;

	//消息处理
	CMD_S_GameFree * pGameFree=(CMD_S_GameFree *)pBuffer;
	
	//设置状态
	SetGameStatus(GAME_STATUS_FREE);

	//清理桌面
	m_GameClientView.SetWinnerSide(false, false, false, false);
	m_GameClientView.m_bBankerMax=false;
	m_GameClientView.CleanUserJetton();
	for (int nAreaIndex=ID_SHUN_MEN; nAreaIndex<=ID_JIAO_R; ++nAreaIndex) SetMePlaceJetton(nAreaIndex,0);
	
	////设置扑克
	for (int i=0; i<CountArray(m_GameClientView.m_CardControl); ++i) m_GameClientView.m_CardControl[i].SetCardData(NULL,0);

	//更新控件
	UpdateButtonContron();

	//完成发牌
	m_GameClientView.FinishDispatchCard();

	//更新成绩
	for (WORD wUserIndex = 0; wUserIndex < MAX_CHAIR; ++wUserIndex)
	{
		IClientUserItem *pUserData = GetTableUserItem(wUserIndex);

		if ( pUserData == NULL ) continue;
		tagApplyUser ApplyUser ;

		//更新信息
		ApplyUser.lUserScore = pUserData->GetUserScore();
		ApplyUser.strUserName = pUserData->GetNickName();
		m_GameClientView.m_ApplyUser.UpdateUser(ApplyUser);
	}
	m_GameClientView.m_blMoveFinish = false;

	//设置界面
	if (IsLookonMode()==false)
	{
		//m_GameClientView.m_btStart.ShowWindow(SW_SHOW);

		//设置时间
		SetGameClock(GetMeChairID(),IDI_START_GAME,pGameFree->cbTimeLeave);
	}
	//空闲时间，清理动画
	ZeroMemory(m_GameClientView.m_stuCartoonInfo,sizeof(m_GameClientView.m_stuCartoonInfo));
	for (int i = 0; i < CTN_COUNT; ++i)
	{
		m_GameClientView.m_stuCartoonInfo[i].nShowTimer = conCartoonTimer[i];
	}


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

	//控制端 不看机器人下注
	if ( pPlaceJetton->cbAndroidUserT && CUserRight::IsGameCheatUser(m_pIClientKernel->GetUserAttribute()->dwUserRight) && m_GameClientView.m_hInst )
	{
		return true;
	}

	//缓冲判断
	if ( pPlaceJetton->cbAndroidUser )
	{
		m_PlaceJettonArray.Add(*pPlaceJetton);
		if (m_PlaceJettonArray.GetCount()==1) 
			SetTimer(IDI_PLACE_JETTON_BUFFER,70,NULL);
		return true;
	}

	if (!pPlaceJetton->cbAndroidUserT && CUserRight::IsGameCheatUser(m_pIClientKernel->GetUserAttribute()->dwUserRight) && m_GameClientView.m_hInst )
	{
		m_GameClientView.ControlBetDetection(pPlaceJetton->cbJettonArea,pPlaceJetton->lJettonScore,SwitchViewChairID(pPlaceJetton->wChairID));
	}

	if (GetMeChairID()!=pPlaceJetton->wChairID || IsLookonMode())
	{
		//加注界面
		m_GameClientView.PlaceUserJetton(pPlaceJetton->cbJettonArea,pPlaceJetton->lJettonScore);

		
		if ((pPlaceJetton->wChairID!=GetMeChairID() || IsLookonMode())&&m_GameClientView.m_bFlashShow==false)
		{
			//if (pPlaceJetton->lJettonScore==5000000) PlayGameSound(AfxGetInstanceHandle(),TEXT("ADD_GOLD_EX"));
			//else 
				PlayGameSound(AfxGetInstanceHandle(),TEXT("ADD_GOLD"));
			//m_DTSDCheer[rand()%3].Play();
// 			switch (rand()%3)
// 			{
// 			case 0:
// 				PlayGameSound(AfxGetInstanceHandle(),TEXT("CHEER1"));
// 				break;
// 			case 1:
// 				PlayGameSound(AfxGetInstanceHandle(),TEXT("CHEER2"));
// 				break;
// 			case 2:
// 				PlayGameSound(AfxGetInstanceHandle(),TEXT("CHEER3"));
// 				break;
// 			}
		}
		
	}

	UpdateButtonContron();
    
	return true;
}
//bool  CGameClientEngine::OnblSubCancelBanker(const void * pBuffer, WORD wDataSize)
//{
//	ASSERT(wDataSize==sizeof(CMD_S_bCanCancelBanker));
//	if (wDataSize!=sizeof(CMD_S_bCanCancelBanker)) return false;
//
//	 CMD_S_bCanCancelBanker *pCanCelBanker = (CMD_S_bCanCancelBanker*)pBuffer;
//
//}
//游戏结束
bool CGameClientEngine::OnSubGameEnd(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_GameEnd));
	if (wDataSize!=sizeof(CMD_S_GameEnd)) return false;
    
	//消息处理
	CMD_S_GameEnd * pGameEnd=(CMD_S_GameEnd *)pBuffer;
	//提前开牌动画
	if(pGameEnd->bBankerBomb)
	{
		m_GameClientView.m_bBankerMax = false;
		m_GameClientView.m_stuCartoonInfo[CTN_BANKER_MAX].bShow = false;
		m_GameClientView.m_stuCartoonInfo[CTN_BANKER_MAX].nShowFrame = 0;
		m_GameClientView.m_stuCartoonInfo[CTN_BANKER_MAX].dwShowTime = GetTickCount();
		m_GameClientView.m_stuCartoonInfo[CTN_BANKER_MAX].bShow = true;
	}
	//下注结束，筛元宝
	m_GameClientView.StartShowCardEx(1,m_GameClientView.m_ptFlash[1].x,m_GameClientView.m_ptFlash[1].y,true);

	m_GameClientView.m_blMoveFinish = false;
	m_GameClientView.m_bBankerMax=false;

	KillGameClock(IDI_DISPATCH_CARD);

	//扑克信息
	m_GameClientView.SetCardInfo(pGameEnd->cbTableCardArray);

	m_GameClientView.SetFirstShowCard(pGameEnd->bcFirstCard);

	////设置扑克
	for (int i=0; i<CountArray(m_GameClientView.m_CardControl); ++i) 
		m_GameClientView.m_CardControl[i].SetCardData(m_GameClientView.m_cbTableCardArray[i],2);

	//設置撲克移動
	m_GameClientView.SetMoveCardTimer();

	//设置状态
	SetGameStatus(GS_GAME_END);

	//设置时间
	SetGameClock(GetMeChairID(),IDI_DISPATCH_CARD, pGameEnd->cbTimeLeave);

	m_cbLeftCardCount=pGameEnd->cbLeftCardCount;

	m_GameClientView.m_bShowHalf = false;

	//庄家信息
	m_GameClientView.SetBankerScore(pGameEnd->nBankerTime, pGameEnd->lBankerTotallScore);

	//成绩信息
	m_GameClientView.SetCurGameScore(pGameEnd->lUserScore,pGameEnd->lUserReturnScore,pGameEnd->lBankerScore,pGameEnd->lRevenue);

	for (int i = 0;i<4;i++)
	{
		m_GameClientView.m_CardControl[i].m_blGameEnd = false;
	}

	//
	//更新控件
	UpdateButtonContron();

	//停止声音
	//for (int i=0; i<CountArray(m_DTSDCheer); ++i) m_DTSDCheer[i].Stop();

	return true;
}

//更新控制
void CGameClientEngine::UpdateButtonContron()
{
	//置能判断
	bool bEnablePlaceJetton=true;
	if (m_bEnableSysBanker==false&&m_wCurrentBanker==INVALID_CHAIR) bEnablePlaceJetton=false;
	if (GetGameStatus()!=GS_PLACE_JETTON) bEnablePlaceJetton=false;
	if (m_wCurrentBanker==GetMeChairID()) bEnablePlaceJetton=false;
	if (IsLookonMode()) bEnablePlaceJetton=false;
	if (!m_bCanPlaceJetton) bEnablePlaceJetton=false;

	if(GetGameStatus()==GS_GAME_END)
	{
		m_GameClientView.m_btOpenCard.EnableWindow(false);
		m_GameClientView.m_btAutoOpenCard.EnableWindow(false);
	}
	else
	{
		m_GameClientView.m_btOpenCard.EnableWindow(true);
		m_GameClientView.m_btAutoOpenCard.EnableWindow(true);
	}

	//下注按钮
	if (bEnablePlaceJetton==true)
	{
		//计算积分
		LONGLONG lCurrentJetton=m_GameClientView.GetCurrentJetton();
		LONGLONG lLeaveScore=m_lMeMaxScore;
		for (int nAreaIndex=1; nAreaIndex<=AREA_COUNT; ++nAreaIndex) lLeaveScore -= m_lUserJettonScore[nAreaIndex];

		//最大下注
		LONGLONG lUserMaxJetton=m_GameClientView.GetUserMaxJetton();

		lLeaveScore= min(lLeaveScore,lUserMaxJetton);

		//设置光标
		if (lCurrentJetton>lLeaveScore)
		{
			if (lLeaveScore>=10000000L) m_GameClientView.SetCurrentJetton(10000000L);
			else if (lLeaveScore>=5000000L) m_GameClientView.SetCurrentJetton(5000000L);
			else if (lLeaveScore>=1000000L) m_GameClientView.SetCurrentJetton(1000000L);
			//else if (lLeaveScore>=500000L) m_GameClientView.SetCurrentJetton(500000L);
			else if (lLeaveScore>=100000L) m_GameClientView.SetCurrentJetton(100000L);
			else if (lLeaveScore>=10000L) m_GameClientView.SetCurrentJetton(10000L);
			else if (lLeaveScore>=1000L) m_GameClientView.SetCurrentJetton(1000L);
			else m_GameClientView.SetCurrentJetton(0L);
		}


#ifdef _DEBUG
		TCHAR TszBuf[256];
		myprintf(TszBuf,256,TEXT("lLeaveScore = %I64d lUserMaxJetton %I64d \n"),lLeaveScore,lUserMaxJetton);
		OutputDebugString(TszBuf);
#endif


		//控制按钮
		//m_GameClientView.m_btJetton100.EnableWindow((lLeaveScore>=100 && lUserMaxJetton>=100)?TRUE:FALSE);
		m_GameClientView.m_btJetton1000.EnableWindow((lLeaveScore>=1000 && lUserMaxJetton>=1000)?TRUE:FALSE);
		m_GameClientView.m_btJetton10000.EnableWindow((lLeaveScore>=10000 && lUserMaxJetton>=10000)?TRUE:FALSE);
		m_GameClientView.m_btJetton100000.EnableWindow((lLeaveScore>=100000 && lUserMaxJetton>=100000)?TRUE:FALSE);
		//m_GameClientView.m_btJetton500000.EnableWindow((lLeaveScore>=500000 && lUserMaxJetton>=500000)?TRUE:FALSE);		
		m_GameClientView.m_btJetton1000000.EnableWindow((lLeaveScore>=1000000 && lUserMaxJetton>=1000000)?TRUE:FALSE);		
		m_GameClientView.m_btJetton5000000.EnableWindow((lLeaveScore>=5000000 && lUserMaxJetton>=5000000)?TRUE:FALSE);
		m_GameClientView.m_btJetton10000000.EnableWindow((lLeaveScore>=10000000 && lUserMaxJetton>=10000000)?TRUE:FALSE);
	}
	else
	{
		//设置光标
		m_GameClientView.SetCurrentJetton(0L);

		//禁止按钮
		//m_GameClientView.m_btJetton100.EnableWindow(FALSE);		
		m_GameClientView.m_btJetton1000.EnableWindow(FALSE);		
		m_GameClientView.m_btJetton10000.EnableWindow(FALSE);	
		m_GameClientView.m_btJetton100000.EnableWindow(FALSE);
		m_GameClientView.m_btJetton500000.EnableWindow(FALSE);
		m_GameClientView.m_btJetton1000000.EnableWindow(FALSE);
		m_GameClientView.m_btJetton5000000.EnableWindow(FALSE);
		m_GameClientView.m_btJetton10000000.EnableWindow(FALSE);

		if(GetGameStatus()==GS_GAME_END)
		{
			//m_GameClientView.m_btJetton100.ShowWindow(SW_HIDE);
			//m_GameClientView.m_btJetton1000.ShowWindow(SW_HIDE);
// 			m_GameClientView.m_btJetton10000.ShowWindow(SW_HIDE);
// 			m_GameClientView.m_btJetton100000.ShowWindow(SW_HIDE);
// 			m_GameClientView.m_btJetton500000.ShowWindow(SW_HIDE);
// 			m_GameClientView.m_btJetton1000000.ShowWindow(SW_HIDE);
// 			m_GameClientView.m_btJetton5000000.ShowWindow(SW_HIDE);
// 			m_GameClientView.m_btJetton10000000.ShowWindow(SW_HIDE);
		}
		else
		{
			m_GameClientView.m_btJetton1000.ShowWindow(SW_SHOW);
			m_GameClientView.m_btJetton10000.ShowWindow(SW_SHOW);
			m_GameClientView.m_btJetton100000.ShowWindow(SW_SHOW);
			//m_GameClientView.m_btJetton500000.ShowWindow(SW_SHOW);
			m_GameClientView.m_btJetton1000000.ShowWindow(SW_SHOW);
			m_GameClientView.m_btJetton5000000.ShowWindow(SW_SHOW);
			m_GameClientView.m_btJetton10000000.ShowWindow(SW_SHOW);

		}
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
		if (m_wCurrentBanker==GetMeChairID() && GetGameStatus()!=GAME_STATUS_FREE) bEnableCancel=false;
		if (m_bMeApplyBanker==false) bEnableCancel=false;
		m_GameClientView.m_btCancelBanker.EnableWindow(bEnableCancel?TRUE:FALSE);
		m_GameClientView.m_btCancelBanker.SetButtonImage(m_GameClientView.m_pD3DDevice,
			m_wCurrentBanker==GetMeChairID()?TEXT("PNG_BT_CANCEL_BANKER"):TEXT("PNG_BT_CANCEL_APPLY"),TEXT("PNG"),AfxGetInstanceHandle());

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
			if (GetGameStatus()!=GAME_STATUS_FREE) bEnableButton=FALSE;
			if (m_cbLeftCardCount < 8) bEnableButton=FALSE;

// 			m_GameClientView.m_btContinueCard.ShowWindow(SW_SHOW);			
// 			m_GameClientView.m_btContinueCard.EnableWindow(bEnableButton);
		}
		else
		{
			m_GameClientView.m_btContinueCard.ShowWindow(SW_HIDE);
		}
	}
	else
	{
		m_GameClientView.m_btCancelBanker.EnableWindow(FALSE);
		m_GameClientView.m_btApplyBanker.EnableWindow(FALSE);
		m_GameClientView.m_btApplyBanker.ShowWindow(SW_SHOW);
		m_GameClientView.m_btCancelBanker.ShowWindow(SW_HIDE);
		m_GameClientView.m_btContinueCard.ShowWindow(SW_HIDE);
	}
	//获取信息
	IClientUserItem *pMeUserData=GetTableUserItem(GetMeChairID());

#ifdef __SPECIAL___
	//银行按钮
	m_GameClientView.m_btBankerDraw.EnableWindow(TRUE);
	m_GameClientView.m_btBankerStorage.EnableWindow(FALSE);
	m_GameClientView.m_btBankerStorage.ShowWindow(SW_SHOW);
	m_GameClientView.m_btBankerStorage.EnableWindow((GetGameStatus()==GAME_STATUS_FREE || IsLookonMode())?TRUE:FALSE);
	if(m_wCurrentBanker==GetMeChairID()&&IsLookonMode()==false)
	{
		m_GameClientView.m_btBankerStorage.EnableWindow(FALSE);
	}
	
	/*if(m_GameClientView.m_DlgBank.m_hWnd !=NULL)
	{
		m_GameClientView.m_DlgBank.ShowItem();
	}*/

	if (m_wCurrentBanker==GetMeChairID()) 
		m_GameClientView.m_blCanStore = false;
	else  
		m_GameClientView.m_blCanStore = true;

	m_GameClientView.SetInsureOption(false, m_GameClientView.m_blCanStore);

#endif

	if ( GetGameStatus()!=GAME_STATUS_FREE ) m_GameClientView.m_btApplyBanker.EnableWindow(FALSE);
	else  m_GameClientView.m_btApplyBanker.EnableWindow(TRUE);

	if(pMeUserData->GetUserScore()<m_lApplyBankerCondition)
	{
		m_GameClientView.m_btApplyBanker.EnableWindow(FALSE);
	}
	else
	{
		m_GameClientView.m_btApplyBanker.EnableWindow(TRUE);
	}

	return;
}

//加注消息
LRESULT CGameClientEngine::OnPlaceJetton(WPARAM wParam, LPARAM lParam)
{
	//变量定义
	BYTE cbJettonArea=(BYTE)wParam;
	LONGLONG lJettonScore=m_GameClientView.GetCurrentJetton();

	//合法判断
	ASSERT(cbJettonArea>=ID_SHUN_MEN && cbJettonArea<=ID_JIAO_R);
	if (!(cbJettonArea>=ID_SHUN_MEN && cbJettonArea<=ID_JIAO_R)) return 0;

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
	SendSocketData(SUB_C_PLACE_JETTON,&PlaceJetton,sizeof(PlaceJetton));

	//更新按钮
	UpdateButtonContron();

	//预先显示
	m_GameClientView.PlaceUserJetton(cbJettonArea,lJettonScore);

	//播放声音
	
	//if (lJettonScore==5000000) PlayGameSound(AfxGetInstanceHandle(),TEXT("ADD_GOLD_EX"));
	//else 
	PlayGameSound(AfxGetInstanceHandle(),TEXT("ADD_GOLD"));
	//m_DTSDCheer[rand()%3].Play();
	/*switch (rand()%3)
	{
	case 0:
		PlayGameSound(AfxGetInstanceHandle(),TEXT("CHEER1"));
		break;
	case 1:
		PlayGameSound(AfxGetInstanceHandle(),TEXT("CHEER2"));
		break;
	case 2:
		PlayGameSound(AfxGetInstanceHandle(),TEXT("CHEER3"));
		break;
	}*/


	return 0;
}
//手工搓牌
LRESULT  CGameClientEngine::OnOpenCard(WPARAM wParam, LPARAM lParam)
{
	if (GetGameStatus()==GS_GAME_END)
	{
		if(m_pIStringMessage!=NULL)
			m_pIStringMessage->InsertNormalString(TEXT("［游戏提示］开牌时间不能切换腾牌模式！"));
		return 1;

	}
	//m_GameClientView.m_btAutoOpenCard.ShowWindow(SW_SHOW );					//自动开牌
	m_GameClientView.m_btOpenCard.ShowWindow(SW_HIDE);	
	m_GameClientView.m_blAutoOpenCard = false;
	if(m_pIStringMessage!=NULL)
		m_pIStringMessage->InsertNormalString(TEXT("［游戏提示］您选择了手工腾牌模式，发牌后您可以使用鼠标拖动扑克！"));
	return 1;
}

//自动搓牌
LRESULT  CGameClientEngine::OnAutoOpenCard(WPARAM wParam, LPARAM lParam)
{
	if (GetGameStatus()==GS_GAME_END)
	{
		if(m_pIStringMessage!=NULL)
			m_pIStringMessage->InsertNormalString(TEXT("［游戏提示］开牌时间不能切换腾牌模式！"));
		return 1;

	}
	m_GameClientView.m_blAutoOpenCard = true;
	m_GameClientView.m_btAutoOpenCard.ShowWindow(SW_HIDE);					//自动开牌
	//m_GameClientView.m_btOpenCard.ShowWindow(SW_SHOW);	

	
	if(m_pIStringMessage!=NULL)
		m_pIStringMessage->InsertNormalString(TEXT("［游戏提示］您选择了自动腾牌模式，开牌后系统将自动开出各家的牌！"));
	return 1;
}
//继续发牌
LRESULT CGameClientEngine::OnContinueCard(WPARAM wParam, LPARAM lParam)
{
	//合法判断
	if (GetMeChairID()!=m_wCurrentBanker) return 0;
	if (GetGameStatus()!=GAME_STATUS_FREE) return 0;
	if (m_cbLeftCardCount < 8) return 0;
	if (IsLookonMode()) return 0;

	//发送消息
	SendSocketData(SUB_C_CONTINUE_CARD);

	//设置按钮
	m_GameClientView.m_btContinueCard.EnableWindow(FALSE);

	return 0;
}

//申请消息
LRESULT CGameClientEngine::OnApplyBanker(WPARAM wParam, LPARAM lParam)
{
	//合法判断
	IClientUserItem *pMeUserData = GetTableUserItem( GetMeChairID() );
	if (pMeUserData->GetUserScore() < m_lApplyBankerCondition) return true;

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

		//m_bMeApplyBanker=true;
	}
	else
	{
		//发送消息
		SendSocketData(SUB_C_CANCEL_BANKER, NULL, 0);

		//m_bMeApplyBanker=false;
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
	IClientUserItem *pUserData=GetTableUserItem(pApplyBanker->wApplyUser);

	//插入玩家
	if (m_wCurrentBanker!=pApplyBanker->wApplyUser)
	{
		tagApplyUser ApplyUser;
		ApplyUser.strUserName=pUserData->GetNickName();
		ApplyUser.lUserScore=pUserData->GetUserScore();
		m_GameClientView.m_ApplyUser.InserUser(ApplyUser);
		if(m_GameClientView.m_ApplyUser.GetItemCount()>4)
		{
			//m_GameClientView.m_btUp.ShowWindow(SW_SHOW);
			m_GameClientView.m_btUp.EnableWindow(true);
			//m_GameClientView.m_btDown.ShowWindow(SW_SHOW);
			m_GameClientView.m_btDown.EnableWindow(true);  
		}
		else
		{
			m_GameClientView.m_btUp.ShowWindow(SW_HIDE);
			m_GameClientView.m_btUp.EnableWindow(true);
			m_GameClientView.m_btDown.ShowWindow(SW_HIDE);
			m_GameClientView.m_btDown.EnableWindow(true); 

			m_GameClientView.m_ApplyUser.m_AppyUserList.SendMessage(WM_VSCROLL, MAKELONG(SB_TOP,0),NULL);
			m_GameClientView.m_ApplyUser.m_AppyUserList.Invalidate(TRUE);
		}
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
	tagApplyUser ApplyUser;
	ApplyUser.strUserName=pCancelBanker->szCancelUser;
	ApplyUser.lUserScore=0;
	m_GameClientView.m_ApplyUser.DeleteUser(ApplyUser);
	if(m_GameClientView.m_ApplyUser.GetItemCount()>4)
	{
		//m_GameClientView.m_btUp.ShowWindow(SW_SHOW);
		m_GameClientView.m_btUp.EnableWindow(true);
		//m_GameClientView.m_btDown.ShowWindow(SW_SHOW);
		m_GameClientView.m_btDown.EnableWindow(true);  

		m_GameClientView.m_ApplyUser.m_AppyUserList.SendMessage(WM_VSCROLL, MAKELONG(SB_TOP,0),NULL);
		m_GameClientView.m_ApplyUser.m_AppyUserList.Invalidate(TRUE);
	}
	else
	{
		m_GameClientView.m_btUp.ShowWindow(SW_HIDE);
		m_GameClientView.m_btUp.EnableWindow(true);
		m_GameClientView.m_btDown.ShowWindow(SW_HIDE);
		m_GameClientView.m_btDown.EnableWindow(true); 

		m_GameClientView.m_ApplyUser.m_AppyUserList.SendMessage(WM_VSCROLL, MAKELONG(SB_TOP,0),NULL);
		m_GameClientView.m_ApplyUser.m_AppyUserList.Invalidate(TRUE);
	}

	//自己判断
	IClientUserItem *pMeUserData=GetTableUserItem(GetMeChairID());
	if (IsLookonMode()==false && lstrcmp(pMeUserData->GetNickName(),pCancelBanker->szCancelUser)==0) m_bMeApplyBanker=false;

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
	if (m_wCurrentBanker!=INVALID_CHAIR)
	{
		IClientUserItem *pBankerUserData=GetTableUserItem(m_wCurrentBanker);
		if (pBankerUserData != NULL)
		{
			tagApplyUser ApplyUser;
			ApplyUser.strUserName = pBankerUserData->GetNickName();
			m_GameClientView.m_ApplyUser.DeleteUser(ApplyUser);
		}
	}

	//更新界面
	UpdateButtonContron();
	return true;
}

//游戏记录
bool CGameClientEngine::OnSubGameRecord(const void * pBuffer, WORD wDataSize)
{
	//效验参数
	ASSERT(wDataSize%sizeof(tagServerGameRecord)==0);
	if (wDataSize%sizeof(tagServerGameRecord)!=0) return false;

	//结果变量
	tagGameRecord GameRecord;
	ZeroMemory(&GameRecord,sizeof(GameRecord));

	//设置记录
	m_GameClientView.m_nRecordLast = 0;
	WORD wRecordCount=wDataSize/sizeof(tagServerGameRecord);
	for (WORD wIndex=0;wIndex<wRecordCount;wIndex++) 
	{
		tagServerGameRecord * pServerGameRecord=(((tagServerGameRecord *)pBuffer)+wIndex);

		m_GameClientView.SetGameHistory(pServerGameRecord->bWinShunMen, pServerGameRecord->bWinDaoMen, pServerGameRecord->bWinDuiMen);
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
	ASSERT(cbViewIndex<=ID_JIAO_R && cbViewIndex>=ID_SHUN_MEN);
	if (!(cbViewIndex<=ID_JIAO_R && cbViewIndex>=ID_SHUN_MEN)) return false;

	//自己判断
	if (GetMeChairID()==pPlaceJettonFail->wPlaceUser && false==IsLookonMode())
	{
		//加注界面
		m_GameClientView.PlaceUserJetton(pPlaceJettonFail->lJettonArea,-pPlaceJettonFail->lPlaceScore);

		LONGLONG lJettonCount=pPlaceJettonFail->lPlaceScore;
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
	IClientUserItem *pUserData=m_wCurrentBanker==INVALID_CHAIR ? NULL : GetTableUserItem(m_wCurrentBanker);
	DWORD dwBankerUserID = (NULL==pUserData) ? 0 : pUserData->GetUserID();
	m_GameClientView.SetBankerInfo(dwBankerUserID,m_lBankerScore);

	//自己判断
	if (IsLookonMode()==false && GetMeChairID()==m_wCurrentBanker) m_bMeApplyBanker=true;
}

//个人下注
void CGameClientEngine::SetMePlaceJetton(BYTE cbViewIndex, LONGLONG lJettonCount)
{
	//合法判断
	ASSERT(cbViewIndex>=ID_SHUN_MEN && cbViewIndex<=ID_JIAO_R);
	if (!(cbViewIndex>=ID_SHUN_MEN && cbViewIndex<=ID_JIAO_R)) return;

	//设置变量
	m_lUserJettonScore[cbViewIndex]=lJettonCount;

	//设置界面
	m_GameClientView.SetMePlaceJetton(cbViewIndex,lJettonCount);
}
LRESULT CGameClientEngine::OnManageControl(WPARAM wParam, LPARAM lParam)
{

	return 0;
}
//发牌声音
LRESULT CGameClientEngine::OnPostCardWav(WPARAM wParam, LPARAM lParam)
{
	PlayGameSound(AfxGetInstanceHandle(),TEXT("DISPATCH_CARD"));

	return 0;
}

LRESULT CGameClientEngine::OnAdminCommand(WPARAM wParam,LPARAM lParam)
{
	SendSocketData(SUB_C_AMDIN_COMMAND,(CMD_C_AdminControl*)wParam,sizeof(CMD_C_AdminControl));
	return true;
}

//开始按钮
LRESULT CGameClientEngine::OnStart(WPARAM wParam, LPARAM lParam)
{   
	//关闭时间
	KillGameClock(IDI_START_GAME);

	//m_GameClientView.m_btStart.ShowWindow(SW_HIDE);

	//发送消息
	SendUserReady(NULL,0);

	return 0;
}

//////////////////////////////////////////////////////////////////////////
 bool CGameClientEngine::OnFrameMessage(WORD wSubCmdID, const void * pBuffer, WORD wDataSize)
{
 //    if(SUB_GF_MESSAGE == wSubCmdID)
	//{
	//	CMD_GF_Message * pMessage=(CMD_GF_Message *)pBuffer;
	//	ASSERT(wDataSize>(sizeof(CMD_GF_Message)-sizeof(pMessage->szContent)));
	//	if (wDataSize<=(sizeof(CMD_GF_Message)-sizeof(pMessage->szContent))) return false;

	//	//消息处理
	//	WORD wHeadSize=sizeof(CMD_GF_Message)-sizeof(pMessage->szContent);
	//	ASSERT(wDataSize==(wHeadSize+pMessage->wMessageLength*sizeof(TCHAR)));
	//	if (wDataSize!=(wHeadSize+pMessage->wMessageLength*sizeof(TCHAR))) return false;
	//	pMessage->szContent[pMessage->wMessageLength-1]=0;

	//	CString str = pMessage->szContent;
	//	int iRet = str.Find(TEXT("提取成功"),0);
	//	if(iRet != -1)
	//	{
	//		m_GameClientView.ClearBank();
	//	}
	//	else
	//	{
	//		iRet = str.Find(TEXT("存储成功"),0);
	//		if(iRet!= -1)
	//		{
	//			m_GameClientView.ClearBank();
	//		}
	//	}
	//}
	return false;

}
void CGameClientEngine::OnTimer(UINT nIDEvent)
{
	if (IDI_PLACE_JETTON_BUFFER==nIDEvent)
	{
		if (m_PlaceJettonArray.GetCount()>0)
		{
			CMD_S_PlaceJetton &PlaceJetton=m_PlaceJettonArray[0];
			PlaceJetton.cbAndroidUser=FALSE;
			OnSubPlaceJetton(&PlaceJetton,sizeof(PlaceJetton));
			m_PlaceJettonArray.RemoveAt(0);

			if(m_PlaceJettonArray.GetCount()>20)
			{
				CMD_S_PlaceJetton &PlaceJetton=m_PlaceJettonArray[0];
				PlaceJetton.cbAndroidUser=FALSE;
				OnSubPlaceJetton(&PlaceJetton,sizeof(PlaceJetton));
				m_PlaceJettonArray.RemoveAt(0);
			}

			if(m_PlaceJettonArray.GetCount()>30)
			{
				CMD_S_PlaceJetton &PlaceJetton=m_PlaceJettonArray[0];
				PlaceJetton.cbAndroidUser=FALSE;
				OnSubPlaceJetton(&PlaceJetton,sizeof(PlaceJetton));
				m_PlaceJettonArray.RemoveAt(0);
			}
		}

		if (m_PlaceJettonArray.GetCount()==0) KillTimer(IDI_PLACE_JETTON_BUFFER);
		return;
	}

	CGameFrameEngine::OnTimer(nIDEvent);
}

//声音控制
bool CGameClientEngine::AllowBackGroundSound(bool bAllowSound)
{
	CGlobalUnits *pGlobalUnits=CGlobalUnits::GetInstance();
	if(bAllowSound && !m_bBackGroundSound && !pGlobalUnits->m_bMuteStatuts)
	{
		m_bBackGroundSound = true;
		PlayBackGroundSound(AfxGetInstanceHandle(), TEXT("BACK_GROUND"));
	}
	else if(!bAllowSound || pGlobalUnits->m_bMuteStatuts)
	{
		StopSound();
		m_bBackGroundSound = false;
	}
	return true;
}
