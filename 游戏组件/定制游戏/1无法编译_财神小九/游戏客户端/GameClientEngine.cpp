 #include "Stdafx.h"
#include "GameClient.h"
#include "GameClientEngine.h"
#include ".\GameClientEngine.h"

//////////////////////////////////////////////////////////////////////////

//ʱ���ʶ
#define IDI_FREE					99									//����ʱ��
#define IDI_PLACE_JETTON			100									//��עʱ��
#define IDI_DISPATCH_CARD			301									//����ʱ��
#define IDI_PLACE_JETTON_BUFFER		302									//����ʱ��

//����ʱ��
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

//���캯��
CGameClientEngine::CGameClientEngine() 
{

	//������Ϣ
	m_lMeMaxScore=0L;			
	m_lAreaLimitScore=0L;		
	m_lApplyBankerCondition=0L;	

	m_bBackGroundSound = false;

	//������ע
	ZeroMemory(m_lUserJettonScore,sizeof(m_lUserJettonScore));

	//ׯ����Ϣ
	m_lBankerScore=0L;
	m_wCurrentBanker=0L;
	m_cbLeftCardCount=0;

	//״̬����
	m_bMeApplyBanker=false;
	m_bCanPlaceJetton=false;
	m_GameClientView.m_bBankerMax=false;
	m_lUserLimitScore = 0L;
	return;
}

//��������
CGameClientEngine::~CGameClientEngine()
{
}
//�Զ�����
LRESULT  CGameClientEngine::OnOpenSound(WPARAM wParam, LPARAM lParam)
{
	
	return 1;

}
//�Զ�����
LRESULT  CGameClientEngine::OnCloseSound(WPARAM wParam, LPARAM lParam)
{
	return 1;

}
//��ʼ����
bool CGameClientEngine::OnInitGameEngine()
{
	//���ñ���
	SetWindowText(TEXT("������Ϸ  --  Ver��6.6.1.0"));

	//����ͼ��
	HICON hIcon=LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME));
	m_pIClientKernel->SetGameAttribute(KIND_ID,GAME_PLAYER,VERSION_CLIENT,hIcon,GAME_NAME);
	SetIcon(hIcon,TRUE);
	SetIcon(hIcon,FALSE);
	//��������
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

//���ÿ��
bool CGameClientEngine::OnResetGameEngine()
{
	//������Ϣ
	m_lMeMaxScore=0L;			

	//������ע
	ZeroMemory(m_lUserJettonScore,sizeof(m_lUserJettonScore));

	//ׯ����Ϣ
	m_lBankerScore=0L;
	m_wCurrentBanker=0L;
	m_cbLeftCardCount=0;

	//״̬����
	m_bMeApplyBanker=false;

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
	if(nTimerID == 0) return true;

	if ((nTimerID==IDI_PLACE_JETTON)&&(nElapse==0))
	{
		//���ù��
		m_GameClientView.SetCurrentJetton(0L);

		//��ֹ��ť
		m_GameClientView.m_btJetton100.EnableWindow(FALSE);		
		m_GameClientView.m_btJetton1000.EnableWindow(FALSE);		
		m_GameClientView.m_btJetton10000.EnableWindow(FALSE);	
		m_GameClientView.m_btJetton100000.EnableWindow(FALSE);
		m_GameClientView.m_btJetton500000.EnableWindow(FALSE);
		m_GameClientView.m_btJetton1000000.EnableWindow(FALSE);
		m_GameClientView.m_btJetton5000000.EnableWindow(FALSE);
		m_GameClientView.m_btJetton10000000.EnableWindow(FALSE);

		//ׯ�Ұ�ť
		m_GameClientView.m_btApplyBanker.EnableWindow( FALSE );
		m_GameClientView.m_btCancelBanker.EnableWindow( FALSE );
	}
    else if(nElapse==0&&nTimerID==IDI_START_GAME&&IsLookonMode()==false)
	{
		//�ر�ʱ��
		KillGameClock(IDI_START_GAME);

		//������Ϣ
		SendUserReady(NULL,0);

		//���ذ�ť
		//m_GameClientView.m_btStart.ShowWindow(SW_HIDE);
	}
	
	
	if (nTimerID==IDI_PLACE_JETTON&&nElapse<=5) PlayGameSound(AfxGetInstanceHandle(),TEXT("TIME_WARIMG"));
	

	return true;
}

//�Թ���Ϣ
bool CGameClientEngine::OnEventLookonMode(VOID * pData, WORD wDataSize)
{
	return true;
}

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
	case SUB_S_SEND_RECORD:		//��Ϸ��¼
		{
			return OnSubGameRecord(pBuffer,wDataSize);
		}
	case SUB_S_PLACE_JETTON_FAIL:	//��עʧ��
		{
			return OnSubPlaceJettonFail(pBuffer,wDataSize);
		}
	case SUB_S_AMDIN_COMMAND_RESULT:
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_S_AdminControl));
			if (wDataSize!=sizeof(CMD_S_AdminControl)) return false;

			//��Ϣ����
			CMD_S_AdminControl * pAdminControl=(CMD_S_AdminControl *)pBuffer;
			if ( pAdminControl->cbControlType == 0 )
			{
				AfxMessageBox(TEXT("�����쳣ʧ�ܣ�"));
			}
			else if ( pAdminControl->cbControlType == 1 )
			{
				AfxMessageBox(TEXT("ȡ�����ƣ�"));
			}
			else if ( pAdminControl->cbControlType == 2 )
			{
				AfxMessageBox(TEXT("���ÿ��Ƴɹ�����������Ч��"));
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
				//m_pIStringMessage->InsertSystemString(TEXT("����Ѻע�ﵽׯ������!"));
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
			//������Ϣ
			m_GameClientView.TransportBackRes(pStatusFree->nRedBack);
			memcpy(m_GameClientView.m_fStarWinRate,pStatusFree->fStarWinRate,sizeof(pStatusFree->fStarWinRate));
			memcpy(m_GameClientView.m_llStarAllCount,pStatusFree->llStarAllCount,sizeof(pStatusFree->llStarAllCount));
			memcpy(m_GameClientView.m_llStarWinCount,pStatusFree->llStarWinCount,sizeof(pStatusFree->llStarWinCount));
			m_GameClientView.m_llAllStarCount = pStatusFree->llAllStarCount;

            //�����Ϣ
			m_lMeMaxScore=pStatusFree->lUserMaxScore;
			m_GameClientView.SetMeMaxScore(m_lMeMaxScore);
			IClientUserItem *pMeUserData=GetTableUserItem(GetMeChairID());
			m_GameClientView.SetMeChairID(pMeUserData->GetUserID());

            //ׯ����Ϣ
			SetBankerInfo(pStatusFree->wBankerUser,pStatusFree->lBankerScore);
			m_GameClientView.SetBankerScore(pStatusFree->cbBankerTime,pStatusFree->lBankerWinScore);
			m_GameClientView.SetBankerAllResult(pStatusFree->lBankerWinScore);
			m_bEnableSysBanker=pStatusFree->bEnableSysBanker;
			m_GameClientView.EnableSysBanker(m_bEnableSysBanker);

            //������Ϣ
			m_lApplyBankerCondition=pStatusFree->lApplyBankerCondition;
			m_lAreaLimitScore=pStatusFree->lAreaLimitScore;
			m_GameClientView.SetAreaLimitScore(m_lAreaLimitScore);

			//���ÿؼ�
			if (IsLookonMode()==false)
			{
				if(GetMeUserItem()->GetUserStatus()!=US_READY)
				{
					//m_GameClientView.m_btStart.ShowWindow(TRUE);
//					m_GameClientView.m_btStart.SetFocus();

					//����ʱ��
					if(pStatusFree->wBankerUser!=INVALID_CHAIR)
					{
						SetGameClock(GetMeChairID(),IDI_START_GAME,pStatusFree->cbTimeLeave);
					}
				}

			}

			//����״̬
			SetGameStatus(GAME_STATUS_FREE);

			//����ʱ��
			//SetGameClock(GetMeChairID(),IDI_FREE,pStatusFree->cbTimeLeave);

			//�����ж�
			/*if(CUserRight::IsGameCheatUser(m_pIClientKernel->GetUserAttribute()->dwUserRight)&&m_GameClientView.m_hInst)
			{
				m_GameClientView.m_btOpenAdmin.ShowWindow(SW_SHOW);

				m_GameClientView.m_SkinListCtrl[0].ShowWindow(SW_SHOW);
				m_GameClientView.m_SkinListCtrl[1].ShowWindow(SW_SHOW);
				m_GameClientView.m_SkinListCtrl[2].ShowWindow(SW_SHOW);
			}*/

			//���¿���
			UpdateButtonContron();
			return true;
		}
	case GAME_STATUS_PLAY:		//��Ϸ״̬
	case GS_GAME_END:		//����״̬
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_S_StatusPlay));
			if (wDataSize!=sizeof(CMD_S_StatusPlay)) return false;

			//��Ϣ����
			CMD_S_StatusPlay * pStatusPlay=(CMD_S_StatusPlay *)pBuffer;
			//������Ϣ
			m_GameClientView.TransportBackRes(pStatusPlay->nRedBack);
			memcpy(m_GameClientView.m_fStarWinRate,pStatusPlay->fStarWinRate,sizeof(pStatusPlay->fStarWinRate));
			memcpy(m_GameClientView.m_llStarAllCount,pStatusPlay->llStarAllCount,sizeof(pStatusPlay->llStarAllCount));
			memcpy(m_GameClientView.m_llStarWinCount,pStatusPlay->llStarWinCount,sizeof(pStatusPlay->llStarWinCount));
			m_GameClientView.m_llAllStarCount = pStatusPlay->llAllStarCount;
			memcpy(m_GameClientView.m_cbStarSelect,pStatusPlay->cbStarSelect,sizeof(pStatusPlay->cbStarSelect));

			//��ע��Ϣ
			for (int nAreaIndex=1; nAreaIndex<=AREA_COUNT; ++nAreaIndex)
			{
				m_GameClientView.PlaceUserJetton(nAreaIndex,pStatusPlay->lAllJettonScore[nAreaIndex]);
				SetMePlaceJetton(nAreaIndex,pStatusPlay->lUserJettonScore[nAreaIndex]);
			}

			//��һ���
			m_lMeMaxScore=pStatusPlay->lUserMaxScore;			
			m_GameClientView.SetMeMaxScore(m_lMeMaxScore);
			IClientUserItem *pMeUserData=GetTableUserItem(GetMeChairID());
			m_GameClientView.SetMeChairID(pMeUserData->GetUserID());

			//������Ϣ
			m_lApplyBankerCondition=pStatusPlay->lApplyBankerCondition;
			m_lAreaLimitScore=pStatusPlay->lAreaLimitScore;
			m_GameClientView.SetAreaLimitScore(m_lAreaLimitScore);

			if (pStatusPlay->cbGameStatus==GS_GAME_END)
			{
				//�˿���Ϣ
				m_GameClientView.SetCardInfo(pStatusPlay->cbTableCardArray);
				////�����˿�
				for (int i=0; i<CountArray(m_GameClientView.m_CardControl); ++i) m_GameClientView.m_CardControl[i].SetCardData(m_GameClientView.m_cbTableCardArray[i],2);
				m_GameClientView.SetFinishDiscard();

				//���óɼ�
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

			//ׯ����Ϣ
			SetBankerInfo(pStatusPlay->wBankerUser,pStatusPlay->lBankerScore);
			m_GameClientView.SetBankerScore(pStatusPlay->cbBankerTime,pStatusPlay->lBankerWinScore);
			m_GameClientView.SetBankerAllResult(pStatusPlay->lBankerWinScore);
			m_bEnableSysBanker=pStatusPlay->bEnableSysBanker;
			m_GameClientView.EnableSysBanker(m_bEnableSysBanker);

			//����״̬
			SetGameStatus(pStatusPlay->cbGameStatus);

			//����ʱ��
			SetGameClock(GetMeChairID(),(pStatusPlay->cbGameStatus==GS_GAME_END)?IDI_DISPATCH_CARD:IDI_PLACE_JETTON,pStatusPlay->cbTimeLeave);

			//�����ж�
			if(CUserRight::IsGameCheatUser(m_pIClientKernel->GetUserAttribute()->dwUserRight)&&m_GameClientView.m_hInst)
			{
				m_GameClientView.m_btOpenAdmin.ShowWindow(SW_SHOW);

				m_GameClientView.m_SkinListCtrl[0].ShowWindow(SW_SHOW);
				m_GameClientView.m_SkinListCtrl[1].ShowWindow(SW_SHOW);
				m_GameClientView.m_SkinListCtrl[2].ShowWindow(SW_SHOW);
			}

			//���°�ť
			UpdateButtonContron();
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

	//��Ϣ����
	CMD_S_GameStart * pGameStart=(CMD_S_GameStart *)pBuffer;
	//������Ϣ
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

	//ׯ����Ϣ
	SetBankerInfo(pGameStart->wBankerUser,pGameStart->lBankerScore);

	//�����Ϣ
	m_lMeMaxScore=pGameStart->lUserMaxScore;
	m_lUserLimitScore = pGameStart->lUserLimitScore;
	m_GameClientView.SetMeMaxScore(m_lMeMaxScore);
	m_GameClientView.SetUserLimitScore(m_lUserLimitScore);

	//����״̬
	SetGameStatus(GS_PLACE_JETTON);

	//����ʱ��
	SetGameClock(GetMeChairID(),IDI_PLACE_JETTON,pGameStart->cbTimeLeave);

	m_bCanPlaceJetton=true;
	//���¿���
	UpdateButtonContron();

	//������ʾ
	m_GameClientView.SetDispatchCardTip(pGameStart->bContiueCard ? enDispatchCardTip_Continue : enDispatchCardTip_Dispatch);

	//��������
	//PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_START"));

	m_PlaceJettonArray.RemoveAll();

	//�ر�ʱ��
	KillGameClock(IDI_START_GAME);

	//���ؿ�ʼ��ť
	//m_GameClientView.m_btStart.ShowWindow(SW_HIDE);

	return true;
}

//��Ϸ����
bool CGameClientEngine::OnSubGameFree(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_GameFree));
	if (wDataSize!=sizeof(CMD_S_GameFree)) return false;

	//��Ϣ����
	CMD_S_GameFree * pGameFree=(CMD_S_GameFree *)pBuffer;
	
	//����״̬
	SetGameStatus(GAME_STATUS_FREE);

	//��������
	m_GameClientView.SetWinnerSide(false, false, false, false);
	m_GameClientView.m_bBankerMax=false;
	m_GameClientView.CleanUserJetton();
	for (int nAreaIndex=ID_SHUN_MEN; nAreaIndex<=ID_JIAO_R; ++nAreaIndex) SetMePlaceJetton(nAreaIndex,0);
	
	////�����˿�
	for (int i=0; i<CountArray(m_GameClientView.m_CardControl); ++i) m_GameClientView.m_CardControl[i].SetCardData(NULL,0);

	//���¿ؼ�
	UpdateButtonContron();

	//��ɷ���
	m_GameClientView.FinishDispatchCard();

	//���³ɼ�
	for (WORD wUserIndex = 0; wUserIndex < MAX_CHAIR; ++wUserIndex)
	{
		IClientUserItem *pUserData = GetTableUserItem(wUserIndex);

		if ( pUserData == NULL ) continue;
		tagApplyUser ApplyUser ;

		//������Ϣ
		ApplyUser.lUserScore = pUserData->GetUserScore();
		ApplyUser.strUserName = pUserData->GetNickName();
		m_GameClientView.m_ApplyUser.UpdateUser(ApplyUser);
	}
	m_GameClientView.m_blMoveFinish = false;

	//���ý���
	if (IsLookonMode()==false)
	{
		//m_GameClientView.m_btStart.ShowWindow(SW_SHOW);

		//����ʱ��
		SetGameClock(GetMeChairID(),IDI_START_GAME,pGameFree->cbTimeLeave);
	}
	//����ʱ�䣬������
	ZeroMemory(m_GameClientView.m_stuCartoonInfo,sizeof(m_GameClientView.m_stuCartoonInfo));
	for (int i = 0; i < CTN_COUNT; ++i)
	{
		m_GameClientView.m_stuCartoonInfo[i].nShowTimer = conCartoonTimer[i];
	}


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

	//���ƶ� ������������ע
	if ( pPlaceJetton->cbAndroidUserT && CUserRight::IsGameCheatUser(m_pIClientKernel->GetUserAttribute()->dwUserRight) && m_GameClientView.m_hInst )
	{
		return true;
	}

	//�����ж�
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
		//��ע����
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
//��Ϸ����
bool CGameClientEngine::OnSubGameEnd(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_GameEnd));
	if (wDataSize!=sizeof(CMD_S_GameEnd)) return false;
    
	//��Ϣ����
	CMD_S_GameEnd * pGameEnd=(CMD_S_GameEnd *)pBuffer;
	//��ǰ���ƶ���
	if(pGameEnd->bBankerBomb)
	{
		m_GameClientView.m_bBankerMax = false;
		m_GameClientView.m_stuCartoonInfo[CTN_BANKER_MAX].bShow = false;
		m_GameClientView.m_stuCartoonInfo[CTN_BANKER_MAX].nShowFrame = 0;
		m_GameClientView.m_stuCartoonInfo[CTN_BANKER_MAX].dwShowTime = GetTickCount();
		m_GameClientView.m_stuCartoonInfo[CTN_BANKER_MAX].bShow = true;
	}
	//��ע������ɸԪ��
	m_GameClientView.StartShowCardEx(1,m_GameClientView.m_ptFlash[1].x,m_GameClientView.m_ptFlash[1].y,true);

	m_GameClientView.m_blMoveFinish = false;
	m_GameClientView.m_bBankerMax=false;

	KillGameClock(IDI_DISPATCH_CARD);

	//�˿���Ϣ
	m_GameClientView.SetCardInfo(pGameEnd->cbTableCardArray);

	m_GameClientView.SetFirstShowCard(pGameEnd->bcFirstCard);

	////�����˿�
	for (int i=0; i<CountArray(m_GameClientView.m_CardControl); ++i) 
		m_GameClientView.m_CardControl[i].SetCardData(m_GameClientView.m_cbTableCardArray[i],2);

	//�O�Ó���Ƅ�
	m_GameClientView.SetMoveCardTimer();

	//����״̬
	SetGameStatus(GS_GAME_END);

	//����ʱ��
	SetGameClock(GetMeChairID(),IDI_DISPATCH_CARD, pGameEnd->cbTimeLeave);

	m_cbLeftCardCount=pGameEnd->cbLeftCardCount;

	m_GameClientView.m_bShowHalf = false;

	//ׯ����Ϣ
	m_GameClientView.SetBankerScore(pGameEnd->nBankerTime, pGameEnd->lBankerTotallScore);

	//�ɼ���Ϣ
	m_GameClientView.SetCurGameScore(pGameEnd->lUserScore,pGameEnd->lUserReturnScore,pGameEnd->lBankerScore,pGameEnd->lRevenue);

	for (int i = 0;i<4;i++)
	{
		m_GameClientView.m_CardControl[i].m_blGameEnd = false;
	}

	//
	//���¿ؼ�
	UpdateButtonContron();

	//ֹͣ����
	//for (int i=0; i<CountArray(m_DTSDCheer); ++i) m_DTSDCheer[i].Stop();

	return true;
}

//���¿���
void CGameClientEngine::UpdateButtonContron()
{
	//�����ж�
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

	//��ע��ť
	if (bEnablePlaceJetton==true)
	{
		//�������
		LONGLONG lCurrentJetton=m_GameClientView.GetCurrentJetton();
		LONGLONG lLeaveScore=m_lMeMaxScore;
		for (int nAreaIndex=1; nAreaIndex<=AREA_COUNT; ++nAreaIndex) lLeaveScore -= m_lUserJettonScore[nAreaIndex];

		//�����ע
		LONGLONG lUserMaxJetton=m_GameClientView.GetUserMaxJetton();

		lLeaveScore= min(lLeaveScore,lUserMaxJetton);

		//���ù��
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


		//���ư�ť
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
		//���ù��
		m_GameClientView.SetCurrentJetton(0L);

		//��ֹ��ť
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
		if (m_wCurrentBanker==GetMeChairID() && GetGameStatus()!=GAME_STATUS_FREE) bEnableCancel=false;
		if (m_bMeApplyBanker==false) bEnableCancel=false;
		m_GameClientView.m_btCancelBanker.EnableWindow(bEnableCancel?TRUE:FALSE);
		m_GameClientView.m_btCancelBanker.SetButtonImage(m_GameClientView.m_pD3DDevice,
			m_wCurrentBanker==GetMeChairID()?TEXT("PNG_BT_CANCEL_BANKER"):TEXT("PNG_BT_CANCEL_APPLY"),TEXT("PNG"),AfxGetInstanceHandle());

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
	//��ȡ��Ϣ
	IClientUserItem *pMeUserData=GetTableUserItem(GetMeChairID());

#ifdef __SPECIAL___
	//���а�ť
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

//��ע��Ϣ
LRESULT CGameClientEngine::OnPlaceJetton(WPARAM wParam, LPARAM lParam)
{
	//��������
	BYTE cbJettonArea=(BYTE)wParam;
	LONGLONG lJettonScore=m_GameClientView.GetCurrentJetton();

	//�Ϸ��ж�
	ASSERT(cbJettonArea>=ID_SHUN_MEN && cbJettonArea<=ID_JIAO_R);
	if (!(cbJettonArea>=ID_SHUN_MEN && cbJettonArea<=ID_JIAO_R)) return 0;

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
	SendSocketData(SUB_C_PLACE_JETTON,&PlaceJetton,sizeof(PlaceJetton));

	//���°�ť
	UpdateButtonContron();

	//Ԥ����ʾ
	m_GameClientView.PlaceUserJetton(cbJettonArea,lJettonScore);

	//��������
	
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
//�ֹ�����
LRESULT  CGameClientEngine::OnOpenCard(WPARAM wParam, LPARAM lParam)
{
	if (GetGameStatus()==GS_GAME_END)
	{
		if(m_pIStringMessage!=NULL)
			m_pIStringMessage->InsertNormalString(TEXT("����Ϸ��ʾ�ݿ���ʱ�䲻���л�����ģʽ��"));
		return 1;

	}
	//m_GameClientView.m_btAutoOpenCard.ShowWindow(SW_SHOW );					//�Զ�����
	m_GameClientView.m_btOpenCard.ShowWindow(SW_HIDE);	
	m_GameClientView.m_blAutoOpenCard = false;
	if(m_pIStringMessage!=NULL)
		m_pIStringMessage->InsertNormalString(TEXT("����Ϸ��ʾ����ѡ�����ֹ�����ģʽ�����ƺ�������ʹ������϶��˿ˣ�"));
	return 1;
}

//�Զ�����
LRESULT  CGameClientEngine::OnAutoOpenCard(WPARAM wParam, LPARAM lParam)
{
	if (GetGameStatus()==GS_GAME_END)
	{
		if(m_pIStringMessage!=NULL)
			m_pIStringMessage->InsertNormalString(TEXT("����Ϸ��ʾ�ݿ���ʱ�䲻���л�����ģʽ��"));
		return 1;

	}
	m_GameClientView.m_blAutoOpenCard = true;
	m_GameClientView.m_btAutoOpenCard.ShowWindow(SW_HIDE);					//�Զ�����
	//m_GameClientView.m_btOpenCard.ShowWindow(SW_SHOW);	

	
	if(m_pIStringMessage!=NULL)
		m_pIStringMessage->InsertNormalString(TEXT("����Ϸ��ʾ����ѡ�����Զ�����ģʽ�����ƺ�ϵͳ���Զ��������ҵ��ƣ�"));
	return 1;
}
//��������
LRESULT CGameClientEngine::OnContinueCard(WPARAM wParam, LPARAM lParam)
{
	//�Ϸ��ж�
	if (GetMeChairID()!=m_wCurrentBanker) return 0;
	if (GetGameStatus()!=GAME_STATUS_FREE) return 0;
	if (m_cbLeftCardCount < 8) return 0;
	if (IsLookonMode()) return 0;

	//������Ϣ
	SendSocketData(SUB_C_CONTINUE_CARD);

	//���ð�ť
	m_GameClientView.m_btContinueCard.EnableWindow(FALSE);

	return 0;
}

//������Ϣ
LRESULT CGameClientEngine::OnApplyBanker(WPARAM wParam, LPARAM lParam)
{
	//�Ϸ��ж�
	IClientUserItem *pMeUserData = GetTableUserItem( GetMeChairID() );
	if (pMeUserData->GetUserScore() < m_lApplyBankerCondition) return true;

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

		//m_bMeApplyBanker=true;
	}
	else
	{
		//������Ϣ
		SendSocketData(SUB_C_CANCEL_BANKER, NULL, 0);

		//m_bMeApplyBanker=false;
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
	IClientUserItem *pUserData=GetTableUserItem(pApplyBanker->wApplyUser);

	//�������
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

	//�Լ��ж�
	IClientUserItem *pMeUserData=GetTableUserItem(GetMeChairID());
	if (IsLookonMode()==false && lstrcmp(pMeUserData->GetNickName(),pCancelBanker->szCancelUser)==0) m_bMeApplyBanker=false;

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

	//���½���
	UpdateButtonContron();
	return true;
}

//��Ϸ��¼
bool CGameClientEngine::OnSubGameRecord(const void * pBuffer, WORD wDataSize)
{
	//Ч�����
	ASSERT(wDataSize%sizeof(tagServerGameRecord)==0);
	if (wDataSize%sizeof(tagServerGameRecord)!=0) return false;

	//�������
	tagGameRecord GameRecord;
	ZeroMemory(&GameRecord,sizeof(GameRecord));

	//���ü�¼
	m_GameClientView.m_nRecordLast = 0;
	WORD wRecordCount=wDataSize/sizeof(tagServerGameRecord);
	for (WORD wIndex=0;wIndex<wRecordCount;wIndex++) 
	{
		tagServerGameRecord * pServerGameRecord=(((tagServerGameRecord *)pBuffer)+wIndex);

		m_GameClientView.SetGameHistory(pServerGameRecord->bWinShunMen, pServerGameRecord->bWinDaoMen, pServerGameRecord->bWinDuiMen);
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
	ASSERT(cbViewIndex<=ID_JIAO_R && cbViewIndex>=ID_SHUN_MEN);
	if (!(cbViewIndex<=ID_JIAO_R && cbViewIndex>=ID_SHUN_MEN)) return false;

	//�Լ��ж�
	if (GetMeChairID()==pPlaceJettonFail->wPlaceUser && false==IsLookonMode())
	{
		//��ע����
		m_GameClientView.PlaceUserJetton(pPlaceJettonFail->lJettonArea,-pPlaceJettonFail->lPlaceScore);

		LONGLONG lJettonCount=pPlaceJettonFail->lPlaceScore;
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
	IClientUserItem *pUserData=m_wCurrentBanker==INVALID_CHAIR ? NULL : GetTableUserItem(m_wCurrentBanker);
	DWORD dwBankerUserID = (NULL==pUserData) ? 0 : pUserData->GetUserID();
	m_GameClientView.SetBankerInfo(dwBankerUserID,m_lBankerScore);

	//�Լ��ж�
	if (IsLookonMode()==false && GetMeChairID()==m_wCurrentBanker) m_bMeApplyBanker=true;
}

//������ע
void CGameClientEngine::SetMePlaceJetton(BYTE cbViewIndex, LONGLONG lJettonCount)
{
	//�Ϸ��ж�
	ASSERT(cbViewIndex>=ID_SHUN_MEN && cbViewIndex<=ID_JIAO_R);
	if (!(cbViewIndex>=ID_SHUN_MEN && cbViewIndex<=ID_JIAO_R)) return;

	//���ñ���
	m_lUserJettonScore[cbViewIndex]=lJettonCount;

	//���ý���
	m_GameClientView.SetMePlaceJetton(cbViewIndex,lJettonCount);
}
LRESULT CGameClientEngine::OnManageControl(WPARAM wParam, LPARAM lParam)
{

	return 0;
}
//��������
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

//��ʼ��ť
LRESULT CGameClientEngine::OnStart(WPARAM wParam, LPARAM lParam)
{   
	//�ر�ʱ��
	KillGameClock(IDI_START_GAME);

	//m_GameClientView.m_btStart.ShowWindow(SW_HIDE);

	//������Ϣ
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

	//	//��Ϣ����
	//	WORD wHeadSize=sizeof(CMD_GF_Message)-sizeof(pMessage->szContent);
	//	ASSERT(wDataSize==(wHeadSize+pMessage->wMessageLength*sizeof(TCHAR)));
	//	if (wDataSize!=(wHeadSize+pMessage->wMessageLength*sizeof(TCHAR))) return false;
	//	pMessage->szContent[pMessage->wMessageLength-1]=0;

	//	CString str = pMessage->szContent;
	//	int iRet = str.Find(TEXT("��ȡ�ɹ�"),0);
	//	if(iRet != -1)
	//	{
	//		m_GameClientView.ClearBank();
	//	}
	//	else
	//	{
	//		iRet = str.Find(TEXT("�洢�ɹ�"),0);
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

//��������
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
