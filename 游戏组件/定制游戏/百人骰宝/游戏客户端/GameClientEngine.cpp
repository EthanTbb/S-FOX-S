 #include "Stdafx.h"
#include "GameClient.h"
#include "GameOption.h"
#include "GameClientEngine.h"

//////////////////////////////////////////////////////////////////////////

//ʱ���ʶ
#define IDI_FREE					200									//����ʱ��
#define IDI_PLACE_JETTON			201									//��עʱ��
#define IDI_DISPATCH_CARD			202									//����ʱ��
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

//���캯��
CGameClientEngine::CGameClientEngine()
{
	//������Ϣ
	m_lMeMaxScore=0L;			
	m_lAreaLimitScore=0L;		
	m_lApplyBankerCondition=0L;	

	//������ע
	ZeroMemory(m_lUserBet,sizeof(m_lUserBet));
	ZeroMemory(m_lInitUserScore,sizeof(m_lInitUserScore));
	ZeroMemory(m_lUserJettonScore,sizeof(m_lUserJettonScore));

	//ׯ����Ϣ
	m_lBankerScore=0L;
	m_wCurrentBanker=0L;
	m_cbLeftCardCount=0;

	//״̬����
	m_bMeApplyBanker=false;

	//��������
	m_ListAndroid.RemoveAll();
	m_bCanPlaceJetton=false;
	return;
}

//��������
CGameClientEngine::~CGameClientEngine()
{
}

//��ʼ����
bool CGameClientEngine::OnInitGameEngine()
{
	//���ñ���
	SetWindowText(TEXT("��������  --  Ver��6.6.1.0"));

	//����ͼ��
	HICON hIcon=LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME));
	m_pIClientKernel->SetGameAttribute(KIND_ID,GAME_PLAYER,VERSION_CLIENT,hIcon,GAME_NAME);
	SetIcon(hIcon,TRUE);
	SetIcon(hIcon,FALSE);
	//��������
	//VERIFY(m_DTSDBackground.Create(TEXT("BACK_GROUND")));
	return true;
}

//���ÿ��
bool CGameClientEngine::OnResetGameEngine()
{
	//������Ϣ
	m_lMeMaxScore=0L;			

	//������ע
	ZeroMemory(m_lUserBet,sizeof(m_lUserBet));
	ZeroMemory(m_lUserJettonScore,sizeof(m_lUserJettonScore));

	//ׯ����Ϣ
	m_lBankerScore=0L;
	m_wCurrentBanker=0L;
	m_cbLeftCardCount=0;

	//״̬����
	m_bMeApplyBanker=false;

	//��������
	m_ListAndroid.RemoveAll();

	return true;
}

//��Ϸ����
void CGameClientEngine::OnGameOptionSet()
{
	return;
}

//ʱ��ɾ��
bool CGameClientEngine::OnEventGameClockKill(WORD wChairID)
{
	return true;
}

//ʱ����Ϣ
bool CGameClientEngine::OnEventGameClockInfo(WORD wChairID, UINT nElapse, WORD nTimerID)
{
	if ((nTimerID==IDI_PLACE_JETTON)&&(nElapse==0))
	{
		//���ù��
		m_GameClientView.SetCurrentJetton(0L,-1);

		//��ֹ��ť
		BYTE cbIndex=0;
		for(cbIndex=0;cbIndex<JETTON_COUNT;cbIndex++)
			m_GameClientView.m_btJetton[cbIndex].EnableWindow(FALSE);
	
		//ׯ�Ұ�ť
		m_GameClientView.m_btApplyBanker.EnableWindow( FALSE );
		m_GameClientView.m_btCancelBanker.EnableWindow( FALSE );
	}

	
	if (nTimerID==IDI_PLACE_JETTON&&nElapse<=5) PlayGameSound(AfxGetInstanceHandle(),TEXT("TIME_WARIMG"));
	

	return true;
}

//�Թ���Ϣ
bool CGameClientEngine::OnEventLookonMode(VOID * pData, WORD wDataSize)
{
	return true;
}

//�û�����
VOID  CGameClientEngine::OnEventUserEnter(IClientUserItem * pIClientUserItem, bool bLookonUser)
{
	//�����û��б�
	UpdateUserList();

	//���óɼ�
	if (bLookonUser==false)
	{

		//���½���
		m_GameClientView.InvalidGameView(0,0,0,0);
	}

	return;
}

//�û��뿪
VOID  CGameClientEngine::OnEventUserLeave(IClientUserItem * pIClientUserItem, bool bLookonUser)
{


	sUserInfo userInfo;
	userInfo.strUserName=pIClientUserItem->GetNickName();
	userInfo.lUserScore=pIClientUserItem->GetUserScore();
	userInfo.lWinScore=0;

	m_GameClientView.m_UserList.DeleteUser(userInfo);


	//���óɼ�
	if (bLookonUser==false)
	{


		WORD wViewChairId = SwitchViewChairID(pIClientUserItem->GetChairID());

		//���½���
		m_GameClientView.InvalidGameView(0,0,0,0);
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

//������Ϣ
bool CGameClientEngine::OnEventGameMessage(WORD wSubCmdID, VOID * pBuffer, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_S_REVOCAT_BET:	//����Ѻע
		{
			return OnRevocatBet(pBuffer,wDataSize);
		}
	case SUB_S_GAME_FREE:		//��Ϸ����
		{
			return OnSubGameFree(pBuffer,wDataSize);
		}
	case SUB_S_GAME_START:		//��Ϸ��ʼ
		{
			return OnSubGameStart(pBuffer,wDataSize);
		}
	case SUB_S_START_BET:
		{
			return OnUserStartBet(pBuffer,wDataSize);
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
	case SUB_S_AMDIN_COMMAND:
		{
			return OnSubReqResult(pBuffer,wDataSize);
		}
	case SUB_S_SEND_USER_BET_INFO:	//������ע��¼
		{
			return OnSubSendUserBetInfo(pBuffer,wDataSize);
		}
	case SUB_S_UPDATE_STORAGE:	//���¿��
		{
			return OnSubUpdateStorage(pBuffer,wDataSize);
		}
	case SUB_S_CONTROL_WIN:
		{
			return OnSubControlWinLose(pBuffer,wDataSize);
		}

	}

	//�������
	ASSERT(FALSE);
	return true;
}

//��Ϸ����
bool CGameClientEngine::OnEventSceneMessage(BYTE cbGameStation, bool bLookonOther, VOID * pBuffer, WORD wDataSize)
{
	//������ʾЧ��

	switch (cbGameStation)
	{
	case GAME_STATUS_FREE:			//����״̬
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_S_StatusFree));
			if (wDataSize!=sizeof(CMD_S_StatusFree)) return false;

			//��Ϣ����
			CMD_S_StatusFree * pStatusFree=(CMD_S_StatusFree *)pBuffer;

			

            //�����Ϣ
			m_lMeMaxScore=pStatusFree->lUserMaxScore;
			m_GameClientView.SetMeMaxScore(m_lMeMaxScore);
			IClientUserItem *pMeUserData=GetTableUserItem(GetMeChairID());
			m_GameClientView.SetMeChairID(SwitchViewChairID(GetMeChairID()));


			//����ʱ��
			SetGameClock(GetMeChairID(),IDI_FREE,pStatusFree->cbTimeLeave);

            //ׯ����Ϣ
			SetBankerInfo(pStatusFree->wBankerUser,pStatusFree->lBankerScore);
			m_GameClientView.SetBankerScore(pStatusFree->cbBankerTime,pStatusFree->lBankerWinScore);
			m_bEnableSysBanker=pStatusFree->bEnableSysBanker;
			m_GameClientView.EnableSysBanker(m_bEnableSysBanker);

            //������Ϣ
			m_lApplyBankerCondition=pStatusFree->lApplyBankerCondition;
			m_lAreaLimitScore=pStatusFree->lAreaLimitScore;
			m_GameClientView.SetAreaLimitScore(m_lAreaLimitScore);

			//����״̬
			SetGameStatus(GAME_STATUS_FREE);
			m_GameClientView.SetGameStatus(GAME_STATUS_FREE);
			//����״̬
			if(CUserRight::IsGameCheatUser(m_pIClientKernel->GetUserAttribute()->dwUserRight)&&m_GameClientView.m_hInst)
				m_GameClientView.m_btOpenAdmin.ShowWindow(SW_SHOW);


			PlayBackGroundSound(AfxGetInstanceHandle(), TEXT("BACK_GROUND"));

			//���¿���
			UpdateButtonContron();
			m_GameClientView.RefreshGameView();

			return true;
		}

	case GS_GAME_START:		//��Ϸ״̬
	case GS_PLAYER_BET:
	case GS_GAME_END:		//����״̬
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_S_StatusPlay));
			if (wDataSize!=sizeof(CMD_S_StatusPlay)) return false;

			//��Ϣ����
			CMD_S_StatusPlay * pStatusPlay=(CMD_S_StatusPlay *)pBuffer;

			//��ע��Ϣ
			for (int nAreaIndex=0; nAreaIndex<AREA_COUNT; ++nAreaIndex)
			{
				m_GameClientView.PlaceUserJetton(nAreaIndex,(LONG)pStatusPlay->lAllJettonScore[nAreaIndex]);
				SetMePlaceJetton(nAreaIndex,(LONG)pStatusPlay->lUserJettonScore[nAreaIndex]);
			}

			//��һ���
			m_lMeMaxScore=pStatusPlay->lUserMaxScore;			
			m_GameClientView.SetMeMaxScore(m_lMeMaxScore);
			IClientUserItem *pMeUserData=GetTableUserItem(GetMeChairID());
			m_GameClientView.SetMeChairID(SwitchViewChairID(GetMeChairID()));

			//������Ϣ
			m_lApplyBankerCondition=pStatusPlay->lApplyBankerCondition;
			m_lAreaLimitScore=pStatusPlay->lAreaLimitScore;
			m_GameClientView.SetAreaLimitScore(m_lAreaLimitScore);

			PlayBackGroundSound(AfxGetInstanceHandle(), TEXT("BACK_GROUND"));

			switch(pStatusPlay->cbGameStatus)
			{
				case GS_GAME_END:
					{
						m_GameClientView.m_bRecordWinner=false;
						//����ֽ��
						m_GameClientView.SetDiceValue(pStatusPlay->cbDiceValue);
						//���óɼ�
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
			
			//ׯ����Ϣ
			SetBankerInfo(pStatusPlay->wBankerUser,pStatusPlay->lBankerScore);
			m_GameClientView.SetBankerScore(pStatusPlay->cbBankerTime,pStatusPlay->lBankerWinScore);
			m_bEnableSysBanker=pStatusPlay->bEnableSysBanker;
			m_GameClientView.EnableSysBanker(m_bEnableSysBanker);

			//����״̬
			SetGameStatus(pStatusPlay->cbGameStatus);
			m_GameClientView.SetGameStatus(pStatusPlay->cbGameStatus);
			//if((GetTableUserItem(GetMeChairID())->dwUserRight&UR_GAME_CONTROL)!=0)
			//	m_GameClientView.m_btOpenAdmin.ShowWindow(SW_SHOW);


			//����ʱ��
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
			
			//����״̬
			if(CUserRight::IsGameCheatUser(m_pIClientKernel->GetUserAttribute()->dwUserRight)&&m_GameClientView.m_hInst)
				m_GameClientView.m_btOpenAdmin.ShowWindow(SW_SHOW);

			//���°�ť
			UpdateButtonContron();
			m_GameClientView.RefreshGameView();

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

	//AfxMessageBox(TEXT("1"));
	//��Ϣ����
	CMD_S_GameStart * pGameStart=(CMD_S_GameStart *)pBuffer;

	m_GameClientView.KillCardTime();
	ZeroMemory(m_lUserJettonScore,sizeof(m_lUserJettonScore));

	//AfxMessageBox(TEXT("2"));

	//ׯ����Ϣ
	SetBankerInfo(pGameStart->wBankerUser,pGameStart->lBankerScore);

	//�����Ϣ
	m_lMeMaxScore=pGameStart->lUserMaxScore;
	m_GameClientView.SetMeMaxScore(m_lMeMaxScore);

	//AfxMessageBox(TEXT("3"));

	//����״̬
	SetGameStatus(GS_GAME_START);
	m_GameClientView.SetGameStatus(GS_GAME_START);

	//AfxMessageBox(TEXT("4"));

	//��������
	m_ListAndroid.RemoveAll();

	//����ʱ��
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
	
	//����״̬
	SetGameStatus(GS_PLAYER_BET);
	m_GameClientView.SetGameStatus(GS_PLAYER_BET);

	//AfxMessageBox(TEXT("OnUserStartBet 2"));

	//����ʱ��
	SetGameClock(GetMeChairID(),IDI_PLACE_JETTON, pStartBet->cbTimeLeave);
	//���°�ť
	UpdateButtonContron();
	
	PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_START"));
	

	//AfxMessageBox(TEXT("OnUserStartBet 3"));

	m_GameClientView.RefreshGameView();

	return true;
}


bool CGameClientEngine::OnRevocatBet(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_RevocatBet));
	if (wDataSize!=sizeof(CMD_S_RevocatBet)) return false;

	//��Ϣ����
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

//��Ϸ����
bool CGameClientEngine::OnSubGameFree(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_GameFree));
	if (wDataSize!=sizeof(CMD_S_GameFree)) return false;
	
	m_GameClientView.m_bAnimatFinish=false;
	m_GameClientView.m_bDrawResult=false;
	m_GameClientView.m_cbPrintCount=0;
	
	//AfxMessageBox(TEXT("OnSubGameFree 1"));
	//��Ϣ����
	CMD_S_GameFree * pGameFree=(CMD_S_GameFree *)pBuffer;

	//����ʱ��
	SetGameClock(GetMeChairID(),IDI_FREE,pGameFree->cbTimeLeave);

	//AfxMessageBox(TEXT("OnSubGameFree 2"));
	//����ʱ��
	KillTimer(IDI_ANDROID_BET);

	//����״̬
	SetGameStatus(GAME_STATUS_FREE);
	m_GameClientView.SetGameStatus(GAME_STATUS_FREE);

	//AfxMessageBox(TEXT("OnSubGameFree 3"));

	//��������
	bool bWinArea[AREA_COUNT]={false};
	m_GameClientView.SetWinnerSide(bWinArea, false);
	m_GameClientView.CleanUserJetton();
	for (int nAreaIndex=0; nAreaIndex<AREA_COUNT; ++nAreaIndex) 
				SetMePlaceJetton(nAreaIndex,0);


	//���¿ؼ�
	UpdateButtonContron();	

	m_GameClientView.RefreshGameView();
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

	if (GetMeChairID()!=pPlaceJetton->wChairID || IsLookonMode())
	{
		//�Ƿ������
		if (pPlaceJetton->bIsAndroid)
		{
			//���λ�������ע
	/*		if(CUserRight::IsGameCheatUser(m_pIClientKernel->GetUserAttribute()->dwUserRight))
				return true;*/

			//����
			static WORD wStFluc = 1;	//�������
			tagAndroidBet androidBet = {};
			androidBet.cbJettonArea = pPlaceJetton->cbJettonArea;
			androidBet.lJettonScore = pPlaceJetton->lJettonScore;
			androidBet.wChairID = pPlaceJetton->wChairID;
			androidBet.nLeftTime = ((rand()+androidBet.wChairID+wStFluc*3)%10+1)*100;
			wStFluc = wStFluc%3 + 1;

			//MyDebug(_T("client::OnSubPlaceJetton ��� %d ���� %d ��ע %I64d ʱ�� %d]"), androidBet.wChairID,
			//	androidBet.cbJettonArea, androidBet.lJettonScore, androidBet.nLeftTime);

			m_ListAndroid.AddTail(androidBet);
		}
		else
		{
			//��ע����
			m_GameClientView.PlaceUserJetton(pPlaceJetton->cbJettonArea,(LONG)pPlaceJetton->lJettonScore);

			//��������
			
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

//��Ϸ����
bool CGameClientEngine::OnSubGameEnd(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_GameEnd));
	if (wDataSize!=sizeof(CMD_S_GameEnd)) return false;

	//��Ϣ����
	CMD_S_GameEnd * pGameEnd=(CMD_S_GameEnd *)pBuffer;

	m_GameClientView.m_blMoveFinish = false;

	//����״̬
	SetGameStatus(GS_GAME_END);
	m_GameClientView.SetGameStatus(GS_GAME_END);
	m_cbLeftCardCount=pGameEnd->cbLeftCardCount;

	//AfxMessageBox(TEXT("OnSubGameEnd 1"));
	//���¿ؼ�
	UpdateButtonContron();
	//����ʱ��
	SetGameClock(GetMeChairID(),IDI_SHOWRESULT,pGameEnd->cbTimeLeave);

	//��ʾ���
	m_GameClientView.OnGameEnd(pGameEnd);

	//AfxMessageBox(TEXT("OnSubGameEnd 2"));

	m_GameClientView.RefreshGameView();

	return true;
}

//������ע��Ϣ
bool CGameClientEngine::OnSubSendUserBetInfo(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_SendUserBetInfo));
	if(wDataSize!=sizeof(CMD_S_SendUserBetInfo)) return false;

	//��Ϣ����
	CMD_S_SendUserBetInfo * pSendUserBetInfo=(CMD_S_SendUserBetInfo *)pBuffer;

	for(WORD i=0; i<GAME_PLAYER; i++)
	{
		m_lUserJettonScore[pSendUserBetInfo->cbAreaCount][i] += pSendUserBetInfo->lUserJettonScore[i];
	}	

	//У��Ȩ��
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

		//��������
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

		//��������
		pUserBetArray->Add(UserBet);
	}

	//���¿ؼ�
	m_GameClientView.m_pClientControlDlg->UpdateUserBet(false);

	return true;
}

//���¿��
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

//���������Ӯ
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



//���¿���
void CGameClientEngine::UpdateButtonContron()
{
		
	//�����ж�
	bool bEnablePlaceJetton=true;
	
	if (IsLookonMode()||GetViewStatus()!=GS_PLAYER_BET||m_wCurrentBanker==GetMeChairID()\
		||(m_bEnableSysBanker==false&&m_wCurrentBanker==INVALID_CHAIR))
	{
		bEnablePlaceJetton=false;
	}

	if (!m_bCanPlaceJetton) bEnablePlaceJetton=false;

	int nIndex=0;
	//��ע��ť
	if (bEnablePlaceJetton==true)
	{
		//�������
		SCORE lCurrentJetton=m_GameClientView.GetCurrentJetton();
		SCORE lLeaveScore=m_lMeMaxScore;
		for (int nAreaIndex=0; nAreaIndex<AREA_COUNT; ++nAreaIndex)
			lLeaveScore -= m_lUserBet[nAreaIndex];

		//�����ע
		SCORE lUserMaxJetton=m_GameClientView.GetUserMaxBet(3);

		//���ù��
		lLeaveScore = min(lLeaveScore,lUserMaxJetton); //�û����·� �����ֱȽ� �������屶 
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

		//���ư�ť
		for(nIndex=0;nIndex<JETTON_COUNT;nIndex++)
			m_GameClientView.m_btJetton[nIndex].EnableWindow((lLeaveScore>=g_lScoreType[nIndex]*MAX_ODDS&& lUserMaxJetton>=g_lScoreType[nIndex]*MAX_ODDS)?TRUE:FALSE);
		
		if(!m_GameClientView.m_btJetton[0].IsWindowEnabled())
			m_GameClientView.SetFlashArea(0xFF);
	}
	else
	{
		//���ù��
		m_GameClientView.SetCurrentJetton(0L,-1);

		//��ֹ��ť
		for(nIndex=0;nIndex<JETTON_COUNT;nIndex++)
			m_GameClientView.m_btJetton[nIndex].EnableWindow(FALSE);
	}


	//ׯ�Ұ�ť
	if (!IsLookonMode())
	{
		//��ȡ��Ϣ
		IClientUserItem *pMeUserData=GetTableUserItem(GetMeChairID());

		//���밴ť
		bool bEnableApply=true;
		if (m_wCurrentBanker==GetMeChairID()) bEnableApply=false;
		if (m_bMeApplyBanker) bEnableApply=false;
		if (pMeUserData->GetUserScore()<m_lApplyBankerCondition) bEnableApply=false;
		m_GameClientView.m_btApplyBanker.EnableWindow(bEnableApply?TRUE:FALSE);

		//ȡ����ť
		bool bEnableCancel=true;
		if (m_wCurrentBanker==GetMeChairID() && GetViewStatus()!=GAME_STATUS_FREE) bEnableCancel=false;
		if (m_bMeApplyBanker==false) bEnableCancel=false;
		m_GameClientView.m_btCancelBanker.EnableWindow(bEnableCancel?TRUE:FALSE);
		m_GameClientView.m_btCancelBanker.SetButtonImage(m_wCurrentBanker==GetMeChairID()?IDB_BT_CANCEL_BANKER:IDB_BT_CANCEL_APPLY,AfxGetInstanceHandle(),false,false);

		//��ʾ�ж�
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

		//���ư�ť
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

//��ע��Ϣ
LRESULT CGameClientEngine::OnPlaceJetton(WPARAM wParam, LPARAM lParam)
{
	//��������
	BYTE cbJettonArea=(BYTE)wParam;
	LONG lJettonScore=(LONG)lParam;

	//�Ϸ��ж�
	ASSERT(cbJettonArea>=0&& cbJettonArea<=AREA_COUNT);
	if (!(cbJettonArea>=0&& cbJettonArea<=AREA_COUNT)) return 0;

	//ׯ���ж�
	if ( GetMeChairID() == m_wCurrentBanker ) return true;

	//״̬�ж�
	if (GetViewStatus()!=GS_PLAYER_BET)
	{
		UpdateButtonContron();
		return true;
	}
	
	//���ñ���
	m_lUserBet[cbJettonArea] += lJettonScore;
	m_GameClientView.SetMePlaceJetton(cbJettonArea, (LONG)m_lUserBet[cbJettonArea]);

	//��������
	CMD_C_PlaceJetton PlaceJetton;
	ZeroMemory(&PlaceJetton,sizeof(PlaceJetton));

	//�������
	PlaceJetton.cbJettonArea=cbJettonArea;
	PlaceJetton.lJettonScore=lJettonScore;

	//������Ϣ
	SendSocketData(SUB_C_PLACE_JETTON,&PlaceJetton,sizeof(PlaceJetton));

	//���°�ť
	UpdateButtonContron();

	//Ԥ����ʾ
	m_GameClientView.PlaceUserJetton(cbJettonArea,lJettonScore);

	//��������
	{
		if (lJettonScore==g_lScoreType[JETTON_COUNT-1]) PlayGameSound(AfxGetInstanceHandle(),TEXT("ADD_GOLD_EX"));
		else PlayGameSound(AfxGetInstanceHandle(),TEXT("ADD_GOLD"));
	}
	return 0;
}

//��������
LRESULT CGameClientEngine::OnContinueCard(WPARAM wParam, LPARAM lParam)
{
	//�Ϸ��ж�
	if (GetMeChairID()!=m_wCurrentBanker) return 0;
	if (GetViewStatus()!=GAME_STATUS_FREE) return 0;
	if (m_cbLeftCardCount < 8) return 0;
	if (IsLookonMode()) return 0;

	//������Ϣ
	SendSocketData(SUB_C_CONTINUE_CARD);

	//���ð�ť
//	m_GameClientView.m_btContinueCard.EnableWindow(FALSE);

	return 0;
}

//������Ϣ
LRESULT CGameClientEngine::OnApplyBanker(WPARAM wParam, LPARAM lParam)
{
	//�Ϸ��ж�
	IClientUserItem *pMeUserData = GetTableUserItem( GetMeChairID() );
	//ת������
	bool bApplyBanker = wParam ? true:false;
	if (pMeUserData->GetUserScore() < m_lApplyBankerCondition && bApplyBanker) return true;
	//�Թ��ж�
	if (IsLookonMode()) return true;
	//��ǰ�ж�
	if (m_wCurrentBanker == GetMeChairID() && bApplyBanker) return true;
	//������Ϣ
	SendSocketData(bApplyBanker?SUB_C_APPLY_BANKER:SUB_C_CANCEL_BANKER, NULL, 0);
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
	IClientUserItem *pUserData=GetTableUserItem(pApplyBanker->wApplyUser);

	//�������
	if (m_wCurrentBanker != pApplyBanker->wApplyUser)
	{
		m_GameClientView.m_ValleysList.Add( SwitchViewChairID(pApplyBanker->wApplyUser) );
	}

	//�Լ��ж�
	if (IsLookonMode()==false && GetMeChairID()==pApplyBanker->wApplyUser) m_bMeApplyBanker=true;

	//���¿ؼ�
	UpdateButtonContron();

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
	for( int i = 0; i < m_GameClientView.m_ValleysList.GetCount(); ++i )
	{
		if ( SwitchViewChairID(pCancelBanker->wCancelUser) == m_GameClientView.m_ValleysList[i] )
		{
			m_GameClientView.m_ValleysList.RemoveAt(i);
			break;
		}
	}

	//�Լ��ж�
	if (IsLookonMode()==false && pCancelBanker->wCancelUser == GetMeChairID()) m_bMeApplyBanker=false;

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
	m_GameClientView.ShowChangeBanker(m_wCurrentBanker!=pChangeBanker->wBankerUser);

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

	//���½���
	UpdateButtonContron();
	m_GameClientView.RefreshGameView();

	return true;
}

//��Ϸ��¼
bool CGameClientEngine::OnSubGameRecord(const void * pBuffer, WORD wDataSize)
{
	//Ч�����
	ASSERT(wDataSize%sizeof(tagGameRecord)==0);
	if (wDataSize%sizeof(tagGameRecord)!=0) return false;
	//���ü�¼
	WORD wRecordCount=wDataSize/sizeof(tagGameRecord);
	tagGameRecord*pRecord=(tagGameRecord*)pBuffer;
	for (WORD wIndex=0;wIndex<wRecordCount;wIndex++) 
	{
		m_GameClientView.SetGameHistory(pRecord[wIndex].cbDiceValue);
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
	ASSERT(cbViewIndex>=0&& cbViewIndex<AREA_COUNT);
	if (!(cbViewIndex>=0&& cbViewIndex<AREA_COUNT)) return true;

	//�Լ��ж�
	if (GetMeChairID()==pPlaceJettonFail->wPlaceUser && false==IsLookonMode())
	{
		//��ע����
		m_GameClientView.PlaceUserJetton(pPlaceJettonFail->lJettonArea,-pPlaceJettonFail->lPlaceScore);

		SCORE lJettonCount=pPlaceJettonFail->lPlaceScore;
		//�Ϸ�У��
		ASSERT(m_lUserBet[cbViewIndex]>=lJettonCount);
		if (lJettonCount>m_lUserBet[cbViewIndex]) return false;

		//������ע
		m_lUserBet[cbViewIndex]-=lJettonCount;
		m_GameClientView.SetMePlaceJetton(cbViewIndex,(LONG)m_lUserBet[cbViewIndex]);
	}

	return true;
}

//����ׯ��
void CGameClientEngine::SetBankerInfo(WORD wBanker,SCORE lScore)
{
	m_wCurrentBanker=wBanker;
	m_lBankerScore=lScore;
	IClientUserItem *pUserData=m_wCurrentBanker==INVALID_CHAIR ? NULL : GetTableUserItem(m_wCurrentBanker);
	DWORD dwBankerUserID = (NULL==pUserData) ? 0 : pUserData->GetUserID();
	m_GameClientView.SetBankerInfo(dwBankerUserID,m_lBankerScore);
}

//������ע
void CGameClientEngine::SetMePlaceJetton(BYTE cbViewIndex, SCORE lJettonCount)
{
	//�Ϸ��ж�
	ASSERT(cbViewIndex>=0&& cbViewIndex<AREA_COUNT);
	if (!(cbViewIndex>=0&& cbViewIndex<AREA_COUNT)) return;

	//���ñ���
	m_lUserBet[cbViewIndex]=lJettonCount;

	//���ý���
	m_GameClientView.SetMePlaceJetton(cbViewIndex,lJettonCount);
}

//������Ϣ
LRESULT CGameClientEngine::OnChatMessage( WPARAM wParam, LPARAM lParam )
{
	m_GameClientView.InsertAllChatMessage((LPCTSTR)wParam,int(lParam));

	return 0;
}

//��Ϸ�ر�
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
			//��ʾ��Ϣ
			CInformation Information(this);
			INT nRes=0;
			nRes=Information.ShowMessageBox(TEXT("��������Ϸ�У�ǿ���˳������۷֣�ȷʵҪǿ����"),MB_ICONQUESTION|MB_YESNO,0);
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
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (nIDEvent == IDI_ANDROID_BET)
	{
		//MyDebug(_T("%d"), m_ListAndroid.GetCount());
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
					placeJetton.bIsAndroid = false;
					placeJetton.cbJettonArea = androidBet.cbJettonArea;
					placeJetton.lJettonScore = androidBet.lJettonScore;
					placeJetton.wChairID = androidBet.wChairID;

					//MyDebug(_T("client::OnTimer ��� %d ���� %d ��ע %I64d"), androidBet.wChairID, androidBet.cbJettonArea, androidBet.lJettonScore);

					OnSubPlaceJetton((void*)&placeJetton, sizeof(placeJetton));

					//ɾ��Ԫ��
					m_ListAndroid.RemoveAt(posTmp);
				}
			}
		}
	}

	CGameFrameEngine::OnTimer(nIDEvent);
}

//��������
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

//�����û��б�
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
		//ͼ������
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