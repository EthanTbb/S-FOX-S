 #include "Stdafx.h"
#include "GameClient.h"
#include "GameOption.h"
#include "GameClientEngine.h"
#include ".\GameClientEngine.h"

//////////////////////////////////////////////////////////////////////////

//ʱ���ʶ
#define IDI_FREE					99									//����ʱ��
#define IDI_PLACE_JETTON			100									//��עʱ��
#define IDI_DISPATCH_CARD			301									//����ʱ��
#define IDI_OPEN_CARD				302								    //����ʱ��

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
//���캯��
CGameClientEngine::CGameClientEngine() 
{
	//������Ϣ
	m_lMeMaxScore=0L;			
	m_lAreaLimitScore=0L;		
	m_lApplyBankerCondition=0L;	
	m_GameEndTime = 0;

	m_blUsing = false;



	//������ע
	ZeroMemory(m_lUserJettonScore,sizeof(m_lUserJettonScore));
	ZeroMemory(m_lPlayerBetAll,sizeof(m_lPlayerBetAll));

	ZeroMemory(m_lInitUserScore,sizeof(m_lInitUserScore));
	//���������ע
	ZeroMemory(m_lAllPlayBet,sizeof(m_lAllPlayBet));
	ZeroMemory(m_lUserStartScore,sizeof(m_lUserStartScore));
	//ׯ����Ϣ
	m_lBankerScore=0L;
	m_wCurrentBanker=0L;
	
	//״̬����
	m_bMeApplyBanker=false;

	m_bPlaceEnd = false;
	m_nCurSetBackMusic=0;
	for ( int i = 0; i < AREA_COUNT+1; ++i )
		m_lPlayLastBet[i] = -1;
	return;
}

//��������
CGameClientEngine::~CGameClientEngine()
{
}

//��ʼ����
bool CGameClientEngine::OnInitGameEngine()
{
	CGlobalUnits * m_pGlobalUnits=(CGlobalUnits *)CGlobalUnits::GetInstance();
	ASSERT(m_pGlobalUnits!=NULL);

	m_pGlobalUnits->m_bMuteStatuts=false;
	//m_pGlobalUnits->m_bAllowSound=true;
	//m_pGlobalUnits->m_bAllowBackGroundSound=true;

	//����ͼ��
	HICON hIcon=LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME));
	m_pIClientKernel->SetGameAttribute(KIND_ID,GAME_PLAYER,VERSION_CLIENT,hIcon,GAME_NAME);

	SetIcon(hIcon,TRUE);
	SetIcon(hIcon,FALSE);
	return true;
}

//���ÿ��
bool CGameClientEngine::OnResetGameEngine()
{
	//������Ϣ
	m_lMeMaxScore=0L;			

	//������ע
	ZeroMemory(m_lUserJettonScore,sizeof(m_lUserJettonScore));
	ZeroMemory(m_lPlayerBetAll,sizeof(m_lPlayerBetAll));
	//���������ע
	ZeroMemory(m_lAllPlayBet,sizeof(m_lAllPlayBet));
	//ׯ����Ϣ
	m_lBankerScore=0L;
	m_wCurrentBanker=0L;
	//״̬����
	m_bMeApplyBanker=false;
	for ( int i = 0; i < AREA_COUNT+1; ++i )
		m_lPlayLastBet[i] = -1;
	return true;
}

//��Ϸ����
void CGameClientEngine::OnGameOptionSet()
{
	return;
}

//����
LRESULT CGameClientEngine::OnGameSetting(WPARAM wParam, LPARAM lParam)
{
	
		CDlgGameOption DlgGameOption;
	if (DlgGameOption.CreateGameOption(NULL,0)==true)
	{
		return 0;
	}
	return 0;
}

//ʱ��ɾ��
bool CGameClientEngine::OnEventGameClockKill(WORD wChairID)
{
	return true;
}

//�Թ�״̬
bool CGameClientEngine::OnEventLookonMode(VOID * pData, WORD wDataSize)
{
	return true;
}

//ʱ����Ϣ
bool CGameClientEngine::OnEventGameClockInfo(WORD wChairID, UINT nElapse, WORD nTimerID)
{
	if ((nTimerID==IDI_PLACE_JETTON)&&(nElapse==0))
	{
		//���ù��	
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

////�Թ�״̬
//void CGameClientEngine::OnEventLookonMode(VOID * pData, WORD wDataSize)
//{
//}

//������Ϣ
bool CGameClientEngine::OnEventGameMessage(WORD wSubCmdID, VOID * pBuffer, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_S_GAME_FREE:		//��Ϸ����
		{
			return OnSubGameFree(pBuffer,wDataSize);
		}
	case SUB_S_GAME_START:		//��Ϸ��ʼ
		{
			return OnSubGameStart(pBuffer,wDataSize);
		}
	case SUB_S_PLACE_JETTON:	//�û���ע
		{
			return OnSubPlaceJetton(pBuffer,wDataSize);
		}
	case SUB_S_APPLY_BANKER:	//������ׯ
		{
			return OnSubUserApplyBanker(pBuffer, wDataSize);
		}
	case SUB_S_CANCEL_BANKER:	//ȡ����ׯ
		{
			return OnSubUserCancelBanker(pBuffer, wDataSize);
		}
	case SUB_S_CHANGE_BANKER:	//�л�ׯ��
		{
			return OnSubChangeBanker(pBuffer, wDataSize);
		}
	case SUB_S_GAME_END:		//��Ϸ����
		{
			return OnSubGameEnd(pBuffer,wDataSize);
		}
	case SUB_S_SEND_RECORD:		//��Ϸ��¼
		{
			return OnSubGameRecord(pBuffer,wDataSize);
		}
	case SUB_S_PLACE_JETTON_FAIL:	//��עʧ��
		{
			return OnSubPlaceJettonFail(pBuffer,wDataSize);
		}
	/*case SUB_S_CHECK_IMAGE:
		{
			return OnSubCheckImageIndex(pBuffer,wDataSize);
		}*/
	case SUB_S_ADMIN_COMMDN:		//����
		{
			return OnSubAdminControl(pBuffer,wDataSize);
		}
	case SUB_S_WAIT_BANKER:		    //�ȴ���ׯ
		{
			m_GameClientView.SetStateTips(enWaitBanker,false);
			return true;
		}	
	case SUB_S_UPDATE_STORAGE:	//���¿��
		{
			return OnSubUpdateStorage(pBuffer,wDataSize);
		}
	}

	//�������
	ASSERT(FALSE);

	return true;
}

//��Ϸ����
bool CGameClientEngine::OnEventSceneMessage(BYTE cbGameStation, bool bLookonOther, VOID * pBuffer, WORD wDataSize)
{
	switch (cbGameStation)
	{
	case GAME_STATUS_FREE:			//����״̬
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_S_StatusFree));
			if (wDataSize!=sizeof(CMD_S_StatusFree)) return false;

			//��Ϣ����
			CMD_S_StatusFree * pStatusFree=(CMD_S_StatusFree *)pBuffer;
			m_bEnableSysBanker=pStatusFree->bEnableSysBanker;
			//��������
			if(pStatusFree->bGenreEducate) m_GameClientView.m_btState[11] = enDisabled;
            //�����Ϣ
			m_lMeMaxScore=pStatusFree->lUserMaxScore;
			m_GameClientView.SetMeMaxScore(m_lMeMaxScore);
			//IClientUserItem *pMeUserData=GetTableUserItem(GetMeChairID());

			IClientUserItem *pMeUserData=GetTableUserItem(GetMeChairID());

			m_GameClientView.SetMeChairID(pMeUserData->GetUserID());

            //ׯ����Ϣ
			SetBankerInfo(pStatusFree->wBankerUser,pStatusFree->lBankerScore);
			m_GameClientView.SetBankerScore(pStatusFree->cbBankerTime,pStatusFree->lBankerWinScore);
			m_GameClientView.SetMultule(pStatusFree->nMultiple);
			m_GameClientView.EnableSysBanker(m_bEnableSysBanker);

			if (!m_bEnableSysBanker&&pStatusFree->wBankerUser==INVALID_CHAIR)
			{
				m_GameClientView.SetStateTips(enWaitBanker,false);
			}
            //������Ϣ
			m_lApplyBankerCondition=pStatusFree->lApplyBankerCondition;
			m_lAreaLimitScore=pStatusFree->lAreaLimitScore;
			m_GameClientView.SetAreaLimitScore(m_lAreaLimitScore);
			
			//����ȫ��
			CString strAllRoomName=pStatusFree->szRoomTotalName;
			m_GameClientView.SetTotalRoomName(strAllRoomName);
			CopyMemory(m_lUserStartScore,pStatusFree->lUserStartScore,sizeof(m_lUserStartScore));
			//����״̬
			SetGameStatus(GAME_STATUS_FREE);

			//���¿���
			UpdateButtonContron();
			m_GameClientView.RefreshGameView();

			//����ʱ��
			SetGameClock(GetMeChairID(),IDI_FREE,pStatusFree->cbTimeLeave);
			
			//��������
			PlayBackGroundSound(AfxGetInstanceHandle(),TEXT("BACK_GROUND"));
			
			//�����б�
			UpdateUserList();

			//����
			if(CUserRight::IsGameCheatUser(m_pIClientKernel->GetUserAttribute()->dwUserRight)&&m_GameClientView.m_hInst)
			{
				m_GameClientView.m_btOpenAdmin.ShowWindow(SW_SHOW);
			}
			//m_nCurSetBackMusic=0;
			//AllowBackGroundSound(false);

			return true;
		}
	case GS_PLACE_JETTON:	//��Ϸ״̬
	case GS_GAME_END:		//����״̬
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_S_StatusPlay));
			if (wDataSize!=sizeof(CMD_S_StatusPlay)) return false;

			
			//��Ϣ����
			CMD_S_StatusPlay * pStatusPlay=(CMD_S_StatusPlay *)pBuffer;
			//��������
			if(pStatusPlay->bGenreEducate) m_GameClientView.m_btState[11] = enDisabled;

			//��ע��Ϣ
			for (int nAreaIndex=1; nAreaIndex<=AREA_COUNT; ++nAreaIndex)
			{
				m_GameClientView.PlaceUserJetton(nAreaIndex,pStatusPlay->lAllJettonScore[nAreaIndex]*m_GameClientView.m_nMultule);
				SetMePlaceJetton(nAreaIndex,pStatusPlay->lUserJettonScore[nAreaIndex]*m_GameClientView.m_nMultule);
			}

			//��һ���
			m_lMeMaxScore=pStatusPlay->lUserMaxScore;			
			m_GameClientView.SetMeMaxScore(m_lMeMaxScore);
			IClientUserItem *pMeUserData=GetTableUserItem(GetMeChairID());
			m_GameClientView.SetMeChairID(pMeUserData->GetUserID());
			m_GameClientView.SetMultule(pStatusPlay->nMultiple);

			//������Ϣ
			m_lApplyBankerCondition=pStatusPlay->lApplyBankerCondition;
			m_lAreaLimitScore=pStatusPlay->lAreaLimitScore;
			m_GameClientView.SetAreaLimitScore(m_lAreaLimitScore);

			if (pStatusPlay->cbGameStatus==GS_GAME_END)
			{
				//�˿���Ϣ
				m_GameClientView.SetCardInfo(pStatusPlay->cbTableCard);
				m_GameClientView.FinishDispatchCard(false);

				//���óɼ�
				m_GameClientView.SetCurGameScore(pStatusPlay->lEndUserScore,pStatusPlay->lEndUserReturnScore,pStatusPlay->lEndBankerScore,pStatusPlay->lEndRevenue);
			}
			else
			{
				m_GameClientView.SetCardInfo(0);
				m_blUsing = true;
				
			}

			//��������
			if(cbGameStation==GS_GAME_END)
			PlayBackGroundSound(AfxGetInstanceHandle(),TEXT("BACK_GROUND_DRAW"));
			//m_nCurSetBackMusic=0;
			//AllowBackGroundSound(false);

			//ׯ����Ϣ
			SetBankerInfo(pStatusPlay->wBankerUser,pStatusPlay->lBankerScore);
			m_GameClientView.SetBankerScore(pStatusPlay->cbBankerTime,pStatusPlay->lBankerWinScore);
			m_bEnableSysBanker=pStatusPlay->bEnableSysBanker;
			m_GameClientView.EnableSysBanker(m_bEnableSysBanker);
			CopyMemory(m_lUserStartScore,pStatusPlay->lUserStartScore,sizeof(m_lUserStartScore));
			//����
			if(CUserRight::IsGameCheatUser(m_pIClientKernel->GetUserAttribute()->dwUserRight)&&m_GameClientView.m_hInst)
			{
				m_GameClientView.m_btOpenAdmin.ShowWindow(SW_SHOW);
			}
			

			//����״̬
			SetGameStatus(pStatusPlay->cbGameStatus);

			//����ʱ��
			UINT nTimerID = pStatusPlay->cbGameStatus==GS_GAME_END ? IDI_OPEN_CARD : IDI_PLACE_JETTON;
			SetGameClock(GetMeChairID(), nTimerID, pStatusPlay->cbTimeLeave);

			//����ȫ��
			CString strAllRoomName=pStatusPlay->szRoomTotalName;
			m_GameClientView.SetTotalRoomName(strAllRoomName);

			//���°�ť
			UpdateButtonContron();
			m_GameClientView.RefreshGameView();
			//�����б�
			UpdateUserList();
			return true;
		}
	}

	return false;
}

//��Ϸ��ʼ
bool CGameClientEngine::OnSubGameStart(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_GameStart));
	if (wDataSize!=sizeof(CMD_S_GameStart)) return false;

	//�����û��б�
	UpdateUserList();
	
	m_bPlaceEnd = false;
	//��Ϣ����
	CMD_S_GameStart * pGameStart=(CMD_S_GameStart *)pBuffer;

	m_GameClientView.KillCardTime();

	m_GameClientView.SetStateTips(enStartBet);

	m_blUsing = true;

	//ׯ����Ϣ
	SetBankerInfo(pGameStart->wBankerUser,pGameStart->lBankerScore);

	//�����Ϣ
	m_lMeMaxScore=pGameStart->lUserMaxScore;
	m_GameClientView.SetMeMaxScore(m_lMeMaxScore);

	//����ʱ��
	SetGameClock(GetMeChairID(),IDI_PLACE_JETTON,pGameStart->cbTimeLeave);

	//����״̬
	SetGameStatus(GS_PLACE_JETTON);
	
	//m_GameClientView.StartRandShowSide();

	SetTimer(IDI_ANDROID_BET, 100, NULL);
	
	//���¿���
	UpdateButtonContron();
	//��������
	StopSound();
	PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_START"));


	OnPlaySound(0,0);

//	SetTimer(IDI_BET_BACK_GROUND, 18000, NULL);
	
	return true;
}

//��Ϸ����
bool CGameClientEngine::OnSubGameFree(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_GameFree));
	if (wDataSize!=sizeof(CMD_S_GameFree)) return false;

	//KillTimer(IDI_END_BACK_GROUND);
	
	//��Ϣ����
	CMD_S_GameFree * pGameFree=(CMD_S_GameFree *)pBuffer;

	m_blUsing = true;

	//�����û��б�
	UpdateUserList();

	//����ʱ��
	SetGameClock(GetMeChairID(),IDI_FREE,pGameFree->cbTimeLeave);

	//m_GameClientView.StartMove();
	//������һ��
	for( int i = 0; i < AREA_COUNT+1; ++i )
	{
		if ( m_lUserJettonScore[i] > 0 )
		{
			CopyMemory(m_lPlayLastBet, m_lUserJettonScore, sizeof(m_lUserJettonScore));
			break;
		}
	}
	//����״̬
	SetGameStatus(GAME_STATUS_FREE);

	//��������
	StopSound();
	PlayBackGroundSound(AfxGetInstanceHandle(),TEXT("BACK_GROUND_BET"));

	if(CUserRight::IsGameCheatUser(m_pIClientKernel->GetUserAttribute()->dwUserRight)&&m_GameClientView.m_hInst)
	{

		m_GameClientView.m_pAdminControl->SetStorageCurrentValue(pGameFree->lStorageCurrent);

		CString cs;
		cs.Format(TEXT("��ǰ��棺%I64d"),pGameFree->lStorageCurrent);	
		ChatMsg msg;
		ZeroMemory(&msg,sizeof(ChatMsg));
		msg.pszString = cs.GetBuffer(0);
		m_GameClientView.InsertAllChatMessage(&msg,enNormal);
		
	/*	if(m_pIStringMessage!=NULL)
			m_pIStringMessage->InsertSystemString(cs);*/
	}
	//����ʱ��
	KillTimer(IDI_ANDROID_BET);

	////��������
	bool blWin[AREA_COUNT];
	for (int i = 0;i<AREA_COUNT;i++){

			blWin[i]=false;
	}

	m_GameClientView.FinishDispatchCard();

	m_GameClientView.SetWinnerSide(blWin, false);



	for (int nAreaIndex=1; nAreaIndex<=AREA_COUNT; ++nAreaIndex) SetMePlaceJetton(nAreaIndex,0);

	m_GameClientView.ResetGameView();
	//���������ע
	ZeroMemory(m_lAllPlayBet,sizeof(m_lAllPlayBet));
	//���¿ؼ�
	UpdateButtonContron();

	return true;
}

//�û���ע
bool CGameClientEngine::OnSubPlaceJetton(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_PlaceJetton));
	if (wDataSize!=sizeof(CMD_S_PlaceJetton)) return false;

	//��Ϣ����
	CMD_S_PlaceJetton * pPlaceJetton=(CMD_S_PlaceJetton *)pBuffer;
	LONGLONG lJettonScore = pPlaceJetton->lJettonScore*m_GameClientView.m_nMultule;
	 m_lAllPlayBet[pPlaceJetton->wChairID][pPlaceJetton->cbJettonArea]+=lJettonScore;

	 if(pPlaceJetton->cbAndroid == FALSE)
	 {
		 //���¿ؼ�
		 m_GameClientView.ControlBetDetection(pPlaceJetton->cbJettonArea,lJettonScore,pPlaceJetton->wChairID);

	 }

	 //�����ж�
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

		//��������
		
		if (pPlaceJetton->wChairID!=GetMeChairID() || IsLookonMode())
		{
			if (pPlaceJetton->lJettonScore>=5000000) 
			{
				PlayGameSound(AfxGetInstanceHandle(),TEXT("ADD_GOLD_EX"));
				//CString strInfo;
				//strInfo.Format(TEXT("��������ע����500w"));

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

//��Ϸ����
bool CGameClientEngine::OnSubGameEnd(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_GameEnd));
	if (wDataSize!=sizeof(CMD_S_GameEnd)) return false;

	//m_nCurSetBackMusic=1;
	//AllowBackGroundSound(true);

	//StopSound();
//	KillTimer(IDI_BET_BACK_GROUND);

	PlayGameSound(AfxGetInstanceHandle(),TEXT("SND_STARTSHOWCARD"));

	//��Ϣ����
	CMD_S_GameEnd * pGameEnd=(CMD_S_GameEnd *)pBuffer;

	//m_GameClientView.m_blMoveFinish = false;

	
	m_GameEndTime = pGameEnd->cbTimeLeave;

	m_blUsing = true;

	//����ʱ��
	SetGameClock(GetMeChairID(),IDI_DISPATCH_CARD, pGameEnd->cbTimeLeave);

	//�˿���Ϣ
	m_GameClientView.SetCardInfo(pGameEnd->cbTableCard);

	m_GameClientView.ClearAreaFlash();
	
	if(pGameEnd->cbTableCard==1||pGameEnd->cbTableCard==5
		||pGameEnd->cbTableCard==9||pGameEnd->cbTableCard==13)
	{
		m_GameClientView.FlashAllArea();
	}
	else
		m_GameClientView.StartRunCar(20);

	
	//����״̬
	SetGameStatus(GS_GAME_END);
	StopSound();
	PlayBackGroundSound(AfxGetInstanceHandle(),TEXT("BACK_GROUND_DRAW"));

	//ׯ����Ϣ
	m_GameClientView.SetBankerScore(pGameEnd->nBankerTime, pGameEnd->lBankerTotallScore);
	//�ɼ���Ϣ
	m_GameClientView.SetCurGameScore(pGameEnd->lUserScore,pGameEnd->lUserReturnScore,pGameEnd->lBankerScore,pGameEnd->lRevenue);

	m_GameClientView.PerformAllBetAnimation();
	
	//���¿ؼ�
	UpdateButtonContron();

	////ֹͣ����
	//for (int i=0; i<CountArray(m_DTSDCheer); ++i) m_DTSDCheer[i].Stop();

	return true;
}

//���¿���
void CGameClientEngine::UpdateButtonContron()
{
	//�����ж�
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

	//��ע��ť
	if (bEnablePlaceJetton)
	{
		
		//�������
		LONGLONG lCurrentJetton=m_GameClientView.GetCurrentJetton();
		LONGLONG lLeaveScore=m_lMeMaxScore;
		for (int nAreaIndex=1; nAreaIndex<=AREA_COUNT; ++nAreaIndex) lLeaveScore -= m_lUserJettonScore[nAreaIndex];

		//�����ע
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
		lLeaveScore = min((lLeaveScore),lUserMaxJetton); //�û����·� �����ֱȽ� �������屶 

// 		lLeaveScore = lLeaveScore*m_GameClientView.m_nMultule;
// 		lUserMaxJetton*=m_GameClientView.m_nMultule;
		//���ư�ť
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

		//��Ͷ��ť  ����ע�Ļ��Ͳ��ܽ�����Ͷ
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
		
		//���ٳ���
		if (lCurrentJetton>lLeaveScore)
		{
			m_GameClientView.ReduceJettonNumber();
		}
	}
	else
	{
		//���ù��
		m_GameClientView.SetCurrentJetton(0L);

		//��ֹ��ť
		for (int i=3;i<11;i++)
		{
			m_GameClientView.m_btState[i]=enDisabled;
		}
		m_GameClientView.ReflashButton(0,true);
	}

	//ׯ�Ұ�ť
	if (!IsLookonMode() && m_GameClientView.m_nSwitchIndex==0)
	{
		//��ȡ��Ϣ
		IClientUserItem *pMeUserData=GetTableUserItem(GetMeChairID());

		//���밴ť
		bool bEnableApply=true;
		if (m_wCurrentBanker==GetMeChairID()) bEnableApply=false;
		if (m_bMeApplyBanker) bEnableApply=false;
		if (pMeUserData->GetUserScore()*m_GameClientView.m_nMultule<m_lApplyBankerCondition) bEnableApply=false;

		m_GameClientView.m_btState[13]=bEnableApply?enNormal:enDisabled;

		//ȡ����ť
		bool bEnableCancel=true;
		if (m_wCurrentBanker==GetMeChairID() && GetGameStatus()!=GAME_STATUS_FREE) bEnableCancel=false;
		if (m_bMeApplyBanker==false) bEnableCancel=false;
		m_GameClientView.m_btState[14]=bEnableCancel?enNormal:enDisabled;
		

		//��ʾ�ж�
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
	//��ȡ��Ϣ
	IClientUserItem *pMeUserData=GetTableUserItem(GetMeChairID());

	
	m_GameClientView.SetInsureOption(false, false);

	ReSetBankCtrol(GetGameStatus());
	m_GameClientView.UpdateButtonContron();

	return;
}

//��ע��Ϣ
LRESULT CGameClientEngine::OnPlaceJetton(WPARAM wParam, LPARAM lParam)
{
	//��������
	BYTE cbJettonArea=(BYTE)wParam;
	LONGLONG lJettonScore=(LONGLONG)(lParam);

	//�Ϸ��ж�
	ASSERT(cbJettonArea>=1 && cbJettonArea<=AREA_COUNT);
	if (!(cbJettonArea>=1 && cbJettonArea<=AREA_COUNT)) return 0;

	//ׯ���ж�
	if ( GetMeChairID() == m_wCurrentBanker ) return true;

	//״̬�ж�
	if (GetGameStatus()!=GS_PLACE_JETTON)
	{
		UpdateButtonContron();
		return true;
	}

	//���ñ���
	m_lUserJettonScore[cbJettonArea] += lJettonScore;
	m_GameClientView.SetMePlaceJetton(cbJettonArea, m_lUserJettonScore[cbJettonArea]);

	//��������
	CMD_C_PlaceJetton PlaceJetton;
	ZeroMemory(&PlaceJetton,sizeof(PlaceJetton));

	//�������
	PlaceJetton.cbJettonArea=cbJettonArea;
	PlaceJetton.lJettonScore=lJettonScore;

	//������Ϣ
	if(!SendSocketData(SUB_C_PLACE_JETTON,&PlaceJetton,sizeof(PlaceJetton)))
	{
		return 0;
	};

	//���°�ť
	UpdateButtonContron();

	//Ԥ����ʾ
	m_GameClientView.PlaceUserJetton(cbJettonArea,lJettonScore);

	//��������
	//if (IsEnableSound()) 
	{
		if (lJettonScore==5000000) PlayGameSound(AfxGetInstanceHandle(),TEXT("ADD_GOLD_EX"));
		else PlayGameSound(AfxGetInstanceHandle(),TEXT("ADD_GOLD"));
	}

	return 0;
}


//�Լ�����·�
LONGLONG CGameClientEngine::GetMeMaxBet( BYTE cbArea )
{
	//�ҳ����
	IClientUserItem * pUserData = GetTableUserItem(GetMeChairID());
	if ( pUserData == NULL || cbArea >= AREA_COUNT ) 
		return 0l;

	//��ҷ���
	LONGLONG lUserScore = pUserData->GetUserScore();

	//��ȥ�Լ�����ע
	for(int i = 0; i <=AREA_COUNT; ++i)
	{
		lUserScore -= m_lUserJettonScore[i];
	}

	//�����������ע. ��Ҫ�ж���������
	if ( cbArea <=AREA_COUNT )
	{
		//����������
		LONGLONG lAreaLimit = m_lAreaLimitScore - m_lPlayerBetAll[cbArea];
		lUserScore = __min( lUserScore, lAreaLimit);
	}

	//�쳣��������
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

//������Ϣ
LRESULT CGameClientEngine::OnApplyBanker(WPARAM wParam, LPARAM lParam)
{
	//�Ϸ��ж�
	IClientUserItem * pClientUserItem=GetTableUserItem(GetMeChairID());

	if (pClientUserItem->GetUserScore()*m_GameClientView.m_nMultule < m_lApplyBankerCondition) return true;

	//�Թ��ж�
	if (IsLookonMode()) return true;

	//ת������
	bool bApplyBanker = wParam ? true:false;

	//��ǰ�ж�
	if (m_wCurrentBanker == GetMeChairID() && bApplyBanker) return true;

	if (bApplyBanker)
	{
		//������Ϣ
		SendSocketData(SUB_C_APPLY_BANKER, NULL, 0);

		m_bMeApplyBanker=true;
	}
	else
	{
		//������Ϣ
		SendSocketData(SUB_C_CANCEL_BANKER, NULL, 0);

		m_bMeApplyBanker=false;
	}

	//���ð�ť
	UpdateButtonContron();

	return true;
}

//������ׯ
bool CGameClientEngine::OnSubUserApplyBanker(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_ApplyBanker));
	if (wDataSize!=sizeof(CMD_S_ApplyBanker)) return false;

	//��Ϣ����
	CMD_S_ApplyBanker * pApplyBanker=(CMD_S_ApplyBanker *)pBuffer;

	//��ȡ���
	//tagUserData const *pUserData=GetTableUserItem(pApplyBanker->wApplyUser);
	IClientUserItem * pClientUserItem=GetTableUserItem(pApplyBanker->wApplyUser);
	//�������
	if (m_wCurrentBanker!=pApplyBanker->wApplyUser)
	{
		m_GameClientView.m_ValleysList.Add(SwitchViewChairID(pApplyBanker->wApplyUser) );
		if(m_GameClientView.m_ValleysList.GetCount()-m_GameClientView.m_nShowValleyIndex>3)
		{
			m_GameClientView.m_nShowValleyIndex = m_GameClientView.m_ValleysList.GetCount()-3;
		}
	}

	//�Լ��ж�
	if (IsLookonMode()==false && GetMeChairID()==pApplyBanker->wApplyUser) m_bMeApplyBanker=true;

	//���¿ؼ�
	UpdateButtonContron();
	//m_GameClientView.m_btCancelBanker.EnableWindow(TRUE);

	return true;
}

//ȡ����ׯ
bool CGameClientEngine::OnSubUserCancelBanker(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_CancelBanker));
	if (wDataSize!=sizeof(CMD_S_CancelBanker)) return false;

	//��Ϣ����
	CMD_S_CancelBanker * pCancelBanker=(CMD_S_CancelBanker *)pBuffer;

	//ɾ�����
	//ɾ�����
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

	//�Լ��ж�
	IClientUserItem *pMeUserData=GetTableUserItem(GetMeChairID());
	if (IsLookonMode()==false && pMeUserData->GetUserID()==pCancelBanker->wCancelUser) m_bMeApplyBanker=false;

	//���¿ؼ�
	UpdateButtonContron();
	return true;
}

//�л�ׯ��
bool CGameClientEngine::OnSubChangeBanker(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_ChangeBanker));
	if (wDataSize!=sizeof(CMD_S_ChangeBanker)) return false;

	//��Ϣ����
	CMD_S_ChangeBanker * pChangeBanker=(CMD_S_ChangeBanker *)pBuffer;

	//��ʾͼƬ
	//m_GameClientView.ShowChangeBanker(m_wCurrentBanker!=pChangeBanker->wBankerUser);

	m_GameClientView.SetStateTips(enChangeBanker);

	//�Լ��ж�
	if (m_wCurrentBanker==GetMeChairID() && IsLookonMode() == false && pChangeBanker->wBankerUser!=GetMeChairID()) 
	{
		m_bMeApplyBanker=false;
	}
	else if (IsLookonMode() == false && pChangeBanker->wBankerUser==GetMeChairID())
	{
		m_bMeApplyBanker=true;
	}

	//ׯ����Ϣ
	SetBankerInfo(pChangeBanker->wBankerUser,pChangeBanker->lBankerScore);
	m_GameClientView.SetBankerScore(0,0);

	//ɾ�����
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

	//���½���
	UpdateButtonContron();
	m_GameClientView.RefreshGameView();

	return true;
}

//��Ϸ��¼
bool CGameClientEngine::OnSubGameRecord(const void * pBuffer, WORD wDataSize)
{
	//Ч�����
	ASSERT(wDataSize%sizeof(tagServerGameRecord)==0);
	if (wDataSize%sizeof(tagServerGameRecord)!=0) return false;

	//�������
	//tagGameRecord GameRecord;
	//ZeroMemory(&GameRecord,sizeof(GameRecord));

	//���ü�¼
	WORD wRecordCount=wDataSize/sizeof(tagServerGameRecord);
	for (WORD wIndex=0;wIndex<wRecordCount;wIndex++) 
	{
		tagServerGameRecord * pServerGameRecord=(((tagServerGameRecord *)pBuffer)+wIndex);

		m_GameClientView.SetGameHistory(pServerGameRecord->cbAnimal);
	}

	return true;
}

//��עʧ��
bool CGameClientEngine::OnSubPlaceJettonFail(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_PlaceJettonFail));
	if (wDataSize!=sizeof(CMD_S_PlaceJettonFail)) return false;

	//��Ϣ����
	CMD_S_PlaceJettonFail * pPlaceJettonFail=(CMD_S_PlaceJettonFail *)pBuffer;

	//Ч�����
	BYTE cbViewIndex=pPlaceJettonFail->lJettonArea;
	ASSERT(cbViewIndex>=1 && cbViewIndex<=AREA_COUNT);
	if (!(cbViewIndex>=1 && cbViewIndex<=AREA_COUNT)) return false;

	//��ע����
	m_GameClientView.PlaceUserJetton(pPlaceJettonFail->lJettonArea,-pPlaceJettonFail->lPlaceScore*m_GameClientView.m_nMultule);

	//�Լ��ж�
	if (GetMeChairID()==pPlaceJettonFail->wPlaceUser && false==IsLookonMode())
	{
		LONGLONG lJettonCount=pPlaceJettonFail->lPlaceScore*m_GameClientView.m_nMultule;
		//�Ϸ�У��
		ASSERT(m_lUserJettonScore[cbViewIndex]>=lJettonCount);
		if (lJettonCount>m_lUserJettonScore[cbViewIndex]) return false;

		//������ע
		m_lUserJettonScore[cbViewIndex]-=lJettonCount;
		m_GameClientView.SetMePlaceJetton(cbViewIndex,m_lUserJettonScore[cbViewIndex]);
	}


	return true;
}

//����ׯ��
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

//������ע
void CGameClientEngine::SetMePlaceJetton(BYTE cbViewIndex, LONGLONG lJettonCount)
{
	//�Ϸ��ж�
	ASSERT(cbViewIndex>=1 && cbViewIndex<=AREA_COUNT);
	if (!(cbViewIndex>=1 && cbViewIndex<=AREA_COUNT)) return;

	//���ñ���
	m_lUserJettonScore[cbViewIndex]=lJettonCount;

	//���ý���
	m_GameClientView.SetMePlaceJetton(cbViewIndex,lJettonCount);
}
//////////////////////////////////////////////////////////////////////////
//����
LRESULT CGameClientEngine::OnAdminControl( WPARAM wParam, LPARAM lParam )
{
	SendSocketData(SUB_C_ADMIN_COMMDN,(CMD_C_ControlApplication*)wParam,sizeof(CMD_C_ControlApplication));
	return true;
}

//������
LRESULT CGameClientEngine::OnUpdataStorage(WPARAM wParam, LPARAM lParam)
{
	SendSocketData(SUB_C_UPDATE_STORAGE,(CMD_C_UpdateStorage*)wParam,sizeof(CMD_C_UpdateStorage));
	return true;
}

//����
LRESULT CGameClientEngine::OnChatMessage( WPARAM wParam, LPARAM lParam )
{
//	m_GameClientView.InsertAllChatMessage((ChatMsg *)wParam,int(lParam));
	return true;
}

//����
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
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
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

			//������ע����
			while (true)
			{
				if (pos == 0)
					break;

				posTmp = pos;
				tagAndroidBet & androidBet = m_ListAndroid.GetPrev(pos);

				androidBet.nLeftTime -= 100;
				if (androidBet.nLeftTime <= 0)
				{		
					//ģ����Ϣ
					CMD_S_PlaceJetton placeJetton = {};
					placeJetton.cbAndroid = FALSE;
					//placeJetton.cbJettonArea = androidBet.cbJettonArea;
					//placeJetton.lJettonScore = androidBet.lJettonScore;
					placeJetton.wChairID = androidBet.wChairID;

					OnSubPlaceJetton((void*)&placeJetton, sizeof(placeJetton));

					//ɾ��Ԫ��
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

//�û�����
VOID  CGameClientEngine::OnEventUserEnter(IClientUserItem * pIClientUserItem, bool bLookonUser)
{
	m_lUserStartScore[pIClientUserItem->GetChairID()] = pIClientUserItem->GetUserScore();
	//�����û��б�
	UpdateUserList();
	//���óɼ�
	if (bLookonUser==false)
	{		
		//���½���
		m_GameClientView.RefreshGameView();
	}

	return;
}

//�û��뿪
VOID  CGameClientEngine::OnEventUserLeave(IClientUserItem * pIClientUserItem, bool bLookonUser)
{
	m_lUserStartScore[pIClientUserItem->GetChairID()] = 0;
	UpdateUserList(true);
	
	//���óɼ�
	if (bLookonUser==false)
	{
		WORD wViewChairId =pIClientUserItem->GetChairID();
		
		//���½���
		m_GameClientView.RefreshGameView();
	}
	//�����ж�
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

//�û�״̬
VOID  CGameClientEngine::OnEventUserStatus(IClientUserItem * pIClientUserItem, bool bLookonUser)
{
	//��������
	IClientUserItem * pIMySelfUserItem=GetMeUserItem();
	BYTE cbUserStatus=pIClientUserItem->GetUserStatus();
	return;
}

//ѡ���¼�
VOID  CGameClientEngine::OnEventPhraseSelect(INT nPhraseIndex)
{
	
}

//��������
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

//�����û��б�
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
		//ͼ������
		UINT uImageIndex=INDEX_NORMAL;
		//�ı���ɫ
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
		//�����ж�
		if(CUserRight::IsGameCheatUser(m_pIClientKernel->GetUserAttribute()->dwUserRight)&&m_GameClientView.m_hInst)
		{			
			if(Refresh==false)
			{
				CUserBetArray *pUserBetArray = &m_GameClientView.m_pAdminControl->m_UserBetArray;

				if(bHave==false)
				{
					//��������
					tagUserBet UserBet;
					ZeroMemory(&UserBet, sizeof(UserBet));

					CopyMemory(UserBet.szNickName, pClientUserItem->GetNickName(), sizeof(UserBet.szNickName));
					UserBet.dwUserGameID = pClientUserItem->GetGameID();
					UserBet.lUserStartScore =   pClientUserItem->GetUserScore();
					UserBet.bAttention = false;
					UserBet.lUserInsure = pClientUserItem->GetUserInsure();
					UserBet.bAndroid = pClientUserItem->IsAndroidUser();
					UserBet.lUserWinLost = (pClientUserItem->GetUserScore()-m_lUserStartScore[i])*m_GameClientView.m_nMultule;
					//��������
					pUserBetArray->Add(UserBet);
				}
				else
				{
					//������Ϣ
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

				//ˢ���б�
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


//���¿��
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


//��ѹ��Ϣ
LRESULT CGameClientEngine::OnMessageOperationRenewal(WPARAM wParam, LPARAM lParam)
{

	//��һ��
	if ( m_lPlayLastBet[0] == -1 )
		return 0;
	//ׯ���ж�
	if ( GetMeChairID() == m_wCurrentBanker ) return 0;

	//״̬�ж�
	if (GetGameStatus()!=GS_PLACE_JETTON)
	{
		UpdateButtonContron();
		return 0;
	}

	//�ж��Ƿ���ע
	LONGLONG lPlayChip = 0;
	bool bCanBet = true;
	for ( int i = 0; i <=AREA_COUNT; ++i )
	{
		//���ñ���
		lPlayChip += m_lPlayLastBet[i];
		//�þ���û�н�����ע ����ע�Ͳ�����Ͷ
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
		Information.ShowMessageBox(TEXT("���Ľ�Ҳ��㣬�޷���ѹ������ע��"));
		return 0;
	}

	//SendSocketData(SUB_C_CLEAR_JETTON);
	//�����ע
	for (int i = 0; i <= AREA_COUNT; ++i)
	{
		m_lPlayerBetAll[i]-=m_lUserJettonScore[i];
		m_lAllPlayBet[GetMeChairID()][i]=0;
		m_lUserJettonScore[i]=0;
	}
	
	//������ע
	for ( int i = 1; i <= AREA_COUNT; ++i )
	{
		//���ñ���
		if(m_lPlayLastBet[i] > 0)
		{
			m_lUserJettonScore[i] += m_lPlayLastBet[i];
			m_GameClientView.SetMePlaceJetton(i, m_lUserJettonScore[i]);
			
			//������Ϣ
			CMD_C_PlaceJetton PlaceJetton;
			ZeroMemory(&PlaceJetton,sizeof(PlaceJetton));

			PlaceJetton.cbJettonArea=i;
			PlaceJetton.lJettonScore=m_lPlayLastBet[i];
			if(SendSocketData(SUB_C_PLACE_JETTON,&PlaceJetton,sizeof(PlaceJetton)) == false)
			{
				return 0;
			}
			//Ԥ����ʾ
			m_GameClientView.PlaceUserJetton(i,m_lUserJettonScore[i]);

		}
	}

	PlayGameSound(AfxGetInstanceHandle(),TEXT("ADD_GOLD"));
	UpdateButtonContron();
	return 0;
}
