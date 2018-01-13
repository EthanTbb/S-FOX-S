#include "Stdafx.h"
#include "GameClient.h"
#include "GameOption.h"
#include "GameClientEngine.h"

//////////////////////////////////////////////////////////////////////////

//ʱ���ʶ
#define IDI_FREE					99									//����ʱ��
#define IDI_PLACE_JETTON			100									//��עʱ��
#define IDI_DISPATCH_CARD			301									//����ʱ��
#define IDI_OPEN_CARD				302								    //����ʱ��
#define IDI_GAME_COMPARE			303									//�´�Сʱ��

#define IDI_ANDROID_BET				1000	


//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameClientEngine, CGameFrameEngine)
	ON_MESSAGE(IDM_PLACE_JETTON,OnPlaceJetton)
	ON_MESSAGE(IDM_APPLY_BANKER, OnApplyBanker)
	ON_MESSAGE(IDM_CONTINUE_CARD, OnContinueCard)
	ON_MESSAGE(IDM_AUTO_OPEN_CARD, OnAutoOpenCard)	
	ON_MESSAGE(IDM_OPEN_CARD, OnOpenCard)	
	ON_MESSAGE(IDM_SHOW_RESULT,OnShowResult)
	ON_MESSAGE(IDM_SOUND,OnPlaySound)
	ON_MESSAGE(IDM_ADMIN_COMMDN, OnAdminControl)	
	ON_MESSAGE(IDM_ADMIN_STORAGE,OnStorage)
	ON_MESSAGE(IDM_ADMIN_DEUCT,OnDeuct)
	ON_MESSAGE(IDM_ADMIN_STORAGEINFO,OnStorageInfo)
	ON_MESSAGE(IDM_CHAGE_MONEY,OnChageMoney)
	ON_MESSAGE(IDM_CONTINUE_JETTON,OnContinueJetton)
	ON_MESSAGE(IDM_CLOSE,OnColse)
	ON_MESSAGE(IDM_OK,OnSureChange)
	ON_MESSAGE(IDM_CHANGE_GOLD,OnChangeGold)
	ON_MESSAGE(IDM_BIG_SMALL,OnGuess)
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
	m_cbCompareTime=0;
	m_lAddGold = 0;

	m_bGuess=false;
	m_blUsing=false;
	m_bMoreJetton = false;
	m_bBackGroundSound = false;

	//������ע
	ZeroMemory(m_lUserJettonScore,sizeof(m_lUserJettonScore));
	ZeroMemory(m_lLastUserJetton,sizeof(m_lLastUserJetton));
	ZeroMemory(m_lUserReturnScore,sizeof(m_lUserReturnScore));

	//ׯ����Ϣ
	m_lBankerScore=0L;
	m_wCurrentBanker=0;
	m_cbLeftCardCount=0;
	m_lMyGameEndScore=0;
	//״̬����
	m_bMeApplyBanker=false;

	return;
}

//��������
CGameClientEngine::~CGameClientEngine()
{
}

//��ʼ����
bool CGameClientEngine::OnInitGameEngine()
{
	//��������
	CGlobalUnits *pGlobalUnits=CGlobalUnits::GetInstance();
	if ( pGlobalUnits->m_bAllowBackGroundSound && !pGlobalUnits->m_bMuteStatuts)
	{
		m_bBackGroundSound = true;
		PlayBackGroundSound(AfxGetInstanceHandle(), TEXT("BACK_GROUND"));
	}

	//����ͼ��
	HICON hIcon=LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME));
	m_pIClientKernel->SetGameAttribute(KIND_ID,GAME_PLAYER,VERSION_CLIENT,hIcon,GAME_NAME);
	SetIcon(hIcon,TRUE);
	SetIcon(hIcon,FALSE);
	
	//��������
	//VERIFY(m_DTSDBackground.Create(TEXT("BACK_GROUND")));
	//VERIFY(m_DTSDCheer[0].Create(TEXT("CHEER1")));
	//VERIFY(m_DTSDCheer[1].Create(TEXT("CHEER2")));
	//VERIFY(m_DTSDCheer[2].Create(TEXT("CHEER3")));
	//VERIFY(m_DTSDCheer[3].Create(TEXT("SPEEKSOUNDWAV")));

	//if(0)
	//{
	//	for (int i = 0;i<10;i++)
	//	{
	//		tagApplyUser ApplyUser;
	//		ApplyUser.strUserName="sfdasfds";
	//		ApplyUser.lUserScore=12121;
	//		m_GameClientView.m_ApplyUser.InserUser(ApplyUser);
	//	}
	//}

	m_GameClientView.SetClientEngine(this);
	return true;
}

//���ÿ��
bool CGameClientEngine::OnResetGameEngine()
{
	//������Ϣ
	m_lMeMaxScore=0L;			

	//������ע
	ZeroMemory(m_lUserJettonScore,sizeof(m_lUserJettonScore));
	ZeroMemory(m_lUserReturnScore,sizeof(m_lUserReturnScore));

	//ׯ����Ϣ
	m_lBankerScore=0L;
	m_wCurrentBanker=0;
	m_cbLeftCardCount=0;
	m_lMyGameEndScore=0;
	//״̬����
	m_bMeApplyBanker=false;
	m_bGuess=false;
	m_blUsing=false;	
	
	return true;
}

//��Ϸ����
void CGameClientEngine::OnGameOptionSet()
{

	//��������
	CGameOption GameOption;
	//GameOption.m_bEnableSound=m_pGlobalUnits->m_bAllowSound;
	//��������
	//if (GameOption.DoModal()==IDOK)
	//{
	//	//���ÿؼ�
	////	m_pGlobalUnits->m_bAllowSound = GameOption.m_bEnableSound;
	//}

	return;
}

//ʱ��ɾ��
bool CGameClientEngine::OnEventGameClockKill(WORD wChairID)
{
	return true;
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

//ʱ����Ϣ
bool CGameClientEngine::OnEventGameClockInfo(WORD wChairID, UINT nElapse, WORD wClockID)
{
	if((wClockID==IDI_FREE)&&(nElapse==0))
	{
		m_GameClientView.m_bTip=false;
	}
	if ((wClockID==IDI_PLACE_JETTON)&&(nElapse==0))
	{
		////���ù��
		//m_GameClientView.SetCurrentJetton(0L);

		////��ֹ��ť
		//m_GameClientView.m_btJetton100.EnableWindow(FALSE);		
		//m_GameClientView.m_btJetton1000.EnableWindow(FALSE);		
		//m_GameClientView.m_btJetton10000.EnableWindow(FALSE);
		//m_GameClientView.m_btJetton50000.EnableWindow(FALSE);
		//m_GameClientView.m_btJetton100000.EnableWindow(FALSE);
		//m_GameClientView.m_btJetton500000.EnableWindow(FALSE);
		//m_GameClientView.m_btJetton1000000.EnableWindow(FALSE);
		//m_GameClientView.m_btJetton5000000.EnableWindow(FALSE);
		//m_GameClientView.m_btContinueJetton.EnableWindow(FALSE);

		//ׯ�Ұ�ť
		m_GameClientView.m_btApplyBanker.EnableWindow( FALSE );
		m_GameClientView.m_btCancelBanker.EnableWindow( FALSE );
		m_GameClientView.m_bTip=false;
	}
	if(IDI_DISPATCH_CARD == wClockID && nElapse==0)
	{
		if(nElapse == 0)	
		{
			KillTimer(IDI_DISPATCH_CARD);
			CString cs;
			if(m_lMyGameEndScore>0)
				cs.Format(TEXT("���ֽ��������ĵ÷����: +%I64d"),m_lMyGameEndScore);
			else
				cs.Format(TEXT("���ֽ��������ĵ÷����: %I64d"),m_lMyGameEndScore);
			m_pIStringMessage->InsertSystemString(cs);
			m_lMyGameEndScore=0;
		}
		
		if ( m_GameEndTime <= 0 || m_GameEndTime > 100)
			m_GameEndTime = 25;
		
		return false;
	}
	/*if(IDI_GAME_COMPARE == wClockID)
	{
		if(nElapse<=3&&nElapse>0)
		{
			m_GameClientView.m_btBig.EnableWindow( FALSE );
			m_GameClientView.m_btSmall.EnableWindow( FALSE );
		}
		if(nElapse==0)
		{
			KillGameClock(IDI_GAME_COMPARE);
			m_GameClientView.m_bTip=false;
		}
	}*/

	//if (m_pGlobalUnits->m_bAllowSound) 
	{
		if (wClockID==IDI_PLACE_JETTON&&nElapse<=5) PlayGameSound(AfxGetInstanceHandle(),TEXT("TIME_WARIMG"));
	}

	////if (m_pGlobalUnits->m_bAllowSound) 
	//{
		/*if (wClockID==IDI_PLACE_JETTON)
		{
	
			if(nElapse%3==0)
				PlayGameSound(AfxGetInstanceHandle(),TEXT("SPEEKSOUNDWAV"));	
		}*/
	//}

	return true;
}

//�Թ�״̬
bool CGameClientEngine::OnEventLookonMode(VOID * pData, WORD wDataSize)
{
	return true;
}

//������Ϣ
bool CGameClientEngine::OnEventGameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_S_GAME_FREE:		//��Ϸ����
		{
			return OnSubGameFree(pData,wDataSize);
		}
	case SUB_S_GAME_START:		//��Ϸ��ʼ
		{
			return OnSubGameStart(pData,wDataSize);
		}
	case SUB_S_PLACE_JETTON:	//�û���ע
		{
			return OnSubPlaceJetton(pData,wDataSize);
		}
	case SUB_S_APPLY_BANKER:	//������ׯ
		{
			return OnSubUserApplyBanker(pData, wDataSize);
		}
	case SUB_S_CANCEL_BANKER:	//ȡ����ׯ
		{
			return OnSubUserCancelBanker(pData, wDataSize);
		}
	case SUB_S_CHANGE_BANKER:	//�л�ׯ��
		{
			return OnSubChangeBanker(pData, wDataSize);
		}
	case SUB_S_GAME_END:		//��Ϸ����
		{
			return OnSubGameEnd(pData,wDataSize);
		}
	case SUB_S_SEND_RECORD:		//��Ϸ��¼
		{
			return OnSubGameRecord(pData,wDataSize);
		}
	case SUB_S_PLACE_JETTON_FAIL:	//��עʧ��
		{
			return OnSubPlaceJettonFail(pData,wDataSize);
		}
	case SUB_S_CHECK_IMAGE:
		{
			return OnSubCheckImageIndex(pData,wDataSize);
		}
	case SUB_S_ADMIN_COMMDN:		//����
		{
			return OnSubAdminControl(pData,wDataSize);
		}
	case SUB_S_CHANGE_GOLD:
		{
			return OnSubChangeGold(pData,wDataSize);
		}
	case SUB_S_GAME_COMPARE:
		{
			return OnSubGameCompare(pData,wDataSize);
		}
	case SUB_S_GUESS:
		{
			return OnSubGuess(pData,wDataSize);
		}
	case SUB_S_ADMIN_STORAGE_INFO:
		{
			ASSERT(wDataSize == sizeof(CMD_S_ADMIN_STORAGE_INFO));
			if(wDataSize != sizeof(CMD_S_ADMIN_STORAGE_INFO)) return false;
			//�ж�Ȩ��
			if(CUserRight::IsGameCheatUser(m_pIClientKernel->GetUserAttribute()->dwUserRight) == false
			   ||NULL == m_GameClientView.m_hInst)
			return true;
			
			CMD_S_ADMIN_STORAGE_INFO *pStorageInfo = (CMD_S_ADMIN_STORAGE_INFO *)pData;

			m_GameClientView.m_pClientControlDlg->SetStorageInfo(pStorageInfo);

			return true;
		}
	}

	//�������
	ASSERT(FALSE);

	return true;
}

//��Ϸ����
bool CGameClientEngine::OnEventSceneMessage(BYTE cbGameStatus, bool bLookonUser, VOID * pData, WORD wDataSize)
{
	//����״̬
	//SetGameStatus(GS_GAME_FREE);
	switch (cbGameStatus)
	{
	case GS_GAME_FREE:			//����״̬
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_S_StatusFree));
			if (wDataSize!=sizeof(CMD_S_StatusFree)) return false;

			//��Ϣ����
			CMD_S_StatusFree * pStatusFree=(CMD_S_StatusFree *)pData;
			m_bEnableSysBanker=pStatusFree->bEnableSysBanker;	
		
			m_GameClientView.m_ShowImageIndex = pStatusFree->CheckImage;

            //�����Ϣ
			m_lMeMaxScore=pStatusFree->lUserMaxScore;
			m_GameClientView.SetMeMaxScore(m_lMeMaxScore);
			IClientUserItem *pMeUserData=GetTableUserItem(GetMeChairID());			
			m_GameClientView.SetMeChairID(pMeUserData->GetChairID());			
			m_lUserScore=pMeUserData->GetUserScore();
			m_lChangeGold=0;
			m_nExchange=pStatusFree->nExchange;
			//m_GameClientView.m_bShow=true;			
			ZeroMemory(m_lExchangeScore,sizeof(m_lExchangeScore));
			ZeroMemory(m_GameClientView.m_lExchangeScore,sizeof(m_GameClientView.m_lExchangeScore));			
			m_GameClientView.GetExchangeScore(m_lUserScore,m_nExchange,0);
			OnChageMoney(0,0);

            //ׯ����Ϣ
			SetBankerInfo(pStatusFree->wBankerUser,pStatusFree->lBankerScore);
			m_GameClientView.SetBankerScore(pStatusFree->cbBankerTime,pStatusFree->lBankerWinScore);
			m_GameClientView.SetBankerScoreEX(pStatusFree->lBankerWinScore);
		
			m_GameClientView.EnableSysBanker(m_bEnableSysBanker);

            //������Ϣ
			m_lApplyBankerCondition=pStatusFree->lApplyBankerCondition;
			m_lAreaLimitScore=pStatusFree->lAreaLimitScore;
			m_GameClientView.SetAreaLimitScore(m_lAreaLimitScore);

			//��������
			if (m_bBackGroundSound)
				PlayBackGroundSound(AfxGetInstanceHandle(),TEXT("BACK_GROUND"));
			else StopSound();

			//����
			/*if((GetTableUserItem(GetMeChairID())->dwUserRight&UR_GAME_CONTROL)!=0 && m_GameClientView.m_pClientControlDlg)
				m_GameClientView.m_btOpenAdmin.ShowWindow(SW_SHOW);*/

			//IClientUserItem *pMeUserData=GetTableUserItem(GetMeChairID());

		
			//�����ж�
			if(CUserRight::IsGameCheatUser(m_pIClientKernel->GetUserAttribute()->dwUserRight)&&m_GameClientView.m_hInst)
			{
				m_GameClientView.m_btOpenAdmin.ShowWindow(SW_SHOW);
				
				/*for ( int i = 0; i < CountArray(m_GameClientView.m_SkinListCtrl); ++i )
				{
					m_GameClientView.m_SkinListCtrl[i].ShowWindow(SW_SHOW);
				}*/
			}


			//����ʱ��
			SetGameClock(GetMeChairID(),IDI_FREE,pStatusFree->cbTimeLeave);

			if(IsLookonMode()==false && GetMeChairID() == m_wCurrentBanker)
			{
				m_bMeApplyBanker =true;
			}
			m_GameClientView.StartFlashTip(false,false);
			m_GameClientView.SetControlStartStorage(pStatusFree->lStartStorage);

			//���¿���
			UpdateButtonContron();
			m_GameClientView.RefreshGameView();
			CString strBuf;
			strBuf.Format(TEXT("��ӭ����ˮ������������ׯ����Ϊ�һ��ĳ��볬��%I64d"),pStatusFree->lApplyBankerCondition);
			m_pIStringMessage->InsertSystemString(strBuf);
			return true;
		}
	case GS_COMPARE_END:
	case GS_PLACE_JETTON:		//��Ϸ״̬
	case GS_GAME_END:		//����״̬
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_S_StatusPlay));
			if (wDataSize!=sizeof(CMD_S_StatusPlay)) return false;

			//��Ϣ����
			CMD_S_StatusPlay * pStatusPlay=(CMD_S_StatusPlay *)pData;

			m_GameClientView.m_ShowImageIndex = pStatusPlay->CheckImage;

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
			m_GameClientView.SetMeChairID(pMeUserData->GetChairID());
			m_lUserScore=pMeUserData->GetUserScore();
			m_lChangeGold=0;
			m_nExchange=pStatusPlay->nExchange;
			//m_GameClientView.m_bShow=true;			
			ZeroMemory(m_lExchangeScore,sizeof(m_lExchangeScore));
			ZeroMemory(m_GameClientView.m_lExchangeScore,sizeof(m_GameClientView.m_lExchangeScore));
			m_GameClientView.GetExchangeScore(m_lUserScore,m_nExchange,0);
			OnChageMoney(0,0);

			//������Ϣ
			m_lApplyBankerCondition=pStatusPlay->lApplyBankerCondition;
			m_lAreaLimitScore=pStatusPlay->lAreaLimitScore;
			m_GameClientView.SetAreaLimitScore(m_lAreaLimitScore);

			if (pStatusPlay->cbGameStatus==GS_GAME_END)
			{
				//�˿���Ϣ
				m_GameClientView.SetCardInfo(pStatusPlay->cbTableCardArray);
				m_GameClientView.FinishDispatchCard(false,0);
				BYTE  cbTempResulteOut[AREA_COUNT+1];
				memset(cbTempResulteOut,0,AREA_COUNT+1);
				m_GameLogic.GetCardTypeEx(pStatusPlay->cbTableCardArray[0],1,cbTempResulteOut);
				bool blWin[AREA_COUNT];
				for (int i = 0;i<AREA_COUNT;i++)
				{
					if(cbTempResulteOut[i]>0)
					{
						blWin[i]=true;
					}
					else
					{
						blWin[i]=false;
					}
				}				
				m_GameClientView.m_blRungingCar = false;
				m_GameClientView.m_bFlashrandShow = false;
				m_GameClientView.m_CarIndex = pStatusPlay->cbTableCardArray[0]-1;
				//����Ӯ��
				m_GameClientView.SetWinnerSide(blWin, true);

			}
			else
			{
				m_GameClientView.SetCardInfo(NULL);				
			}

			//��������
			if (m_bBackGroundSound)
				PlayBackGroundSound(AfxGetInstanceHandle(),TEXT("BACK_GROUND"));
			else StopSound();
			//ׯ����Ϣ
			SetBankerInfo(pStatusPlay->wBankerUser,pStatusPlay->lBankerScore);
			m_GameClientView.SetBankerScore(pStatusPlay->cbBankerTime,pStatusPlay->lBankerWinScore);
			m_GameClientView.SetBankerScoreEX(pStatusPlay->lBankerWinScore);
			m_bEnableSysBanker=pStatusPlay->bEnableSysBanker;
			m_GameClientView.EnableSysBanker(m_bEnableSysBanker);

			//����
			/*if((GetTableUserItem(GetMeChairID())->dwUserRight&UR_GAME_CONTROL)!=0 && m_GameClientView.m_pClientControlDlg)
				m_GameClientView.m_btOpenAdmin.ShowWindow(SW_SHOW);*/
		
			//IClientUserItem *pMeUserData=GetTableUserItem(GetMeChairID());

			//�����ж�
			if(CUserRight::IsGameCheatUser(m_pIClientKernel->GetUserAttribute()->dwUserRight)&&m_GameClientView.m_hInst)
			{
				m_GameClientView.m_btOpenAdmin.ShowWindow(SW_SHOW);
			}

			if(IsLookonMode()==false && GetMeChairID() == m_wCurrentBanker)
			{
				m_bMeApplyBanker =true;
			}

			//���°�ť
			UpdateButtonContron();			

			//����״̬
			SetGameStatus(pStatusPlay->cbGameStatus);
			if (pStatusPlay->cbGameStatus != GS_GAME_END)
			{
				m_GameClientView.StartFlashTip(false,false);
			}
			m_GameClientView.SetControlStartStorage(pStatusPlay->lStartStorage);

			//����ʱ��
			if(pStatusPlay->cbGameStatus == GS_GAME_FREE || pStatusPlay->cbGameStatus == GS_PLACE_JETTON)
			{
				SetGameClock(GetMeChairID(), IDI_PLACE_JETTON, pStatusPlay->cbTimeLeave);
			}
			else if(pStatusPlay->cbGameStatus == GS_GAME_END)
			{
				SetGameClock(GetMeChairID(),IDI_DISPATCH_CARD,pStatusPlay->cbTimeLeave);
			}
			m_GameClientView.RefreshGameView();
		
			CString strBuf;
			strBuf.Format(TEXT("��ӭ����ˮ������������ׯ����Ϊ�һ��ĳ��볬��%I64d"),pStatusPlay->lApplyBankerCondition);
			m_pIStringMessage->InsertSystemString(strBuf);
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

	m_GameClientView.SwithToNormalView();

	m_GameClientView.KillCardTime();	

	//ׯ����Ϣ
	SetBankerInfo(pGameStart->wBankerUser,pGameStart->lBankerScore);

	//�����Ϣ
	m_lMeMaxScore=m_lExchangeScore[GetMeChairID()];
	m_GameClientView.SetMeMaxScore(m_lMeMaxScore);

	//����ʱ��
	SetGameClock(GetMeChairID(),IDI_PLACE_JETTON,pGameStart->cbTimeLeave);

	m_GameClientView.StartRandShowSide();	

	//����״̬
	SetGameStatus(GS_PLACE_JETTON);
	SetTimer(IDI_ANDROID_BET, 100, NULL);
	m_GameClientView.StartFlashTip(false,false);

	m_GameClientView.m_bShowBao = true;


	//���¿���
	UpdateButtonContron();

	//������ʾ
	m_GameClientView.SetDispatchCardTip(pGameStart->bContiueCard ? enDispatchCardTip_Continue : enDispatchCardTip_Dispatch);

	

	//��������
	//if (m_pGlobalUnits->m_bAllowSound) 
	////{
	//	PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_START"));
	//	m_DTSDBackground.Play(0,true);
	//	// [5/27/2010 WMY]
	//}
	//else 
	//{
	//	m_DTSDBackground.Stop();
	//}
	PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_START"));	
	
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

	m_blUsing = false;
	m_bMoreJetton = false;
	m_GameClientView.PerformAllBetAnimation();

	//����ʱ��
	SetGameClock(GetMeChairID(),IDI_FREE,pGameFree->cbTimeLeave);	

	//����״̬
	SetGameStatus(GS_GAME_FREE);
	m_GameClientView.StartFlashTip(false,false);
	m_GameClientView.m_bGuess=false;
	m_GameClientView.m_bShowJettonAn = false;
	m_GameClientView.m_bShowJettonIndex = 0;
	m_GameClientView.SetMoveCardTimer();
	StopSound();
	if (m_bBackGroundSound)
		PlayBackGroundSound(AfxGetInstanceHandle(),TEXT("BACK_GROUND"));	

	//����ǹ���Ա��ÿ�ַ��Ϳ����
	CString cs;
	cs.Format(TEXT("��ǰ�����Ϊ��%I64d"),pGameFree->lStorageStart);	
	if(CUserRight::IsGameCheatUser(m_pIClientKernel->GetUserAttribute()->dwUserRight)&&m_GameClientView.m_hInst)
	{
		if(m_pIStringMessage!=NULL)
			m_pIStringMessage->InsertSystemString(cs);
	}

	//����ʱ��
	KillTimer(IDI_ANDROID_BET);

	////��������
	bool blWin[AREA_COUNT+1];
	for (int i = 0;i<=AREA_COUNT;i++){

			blWin[i]=false;
	}

	m_GameClientView.FinishDispatchCard(true,0);

	m_GameClientView.SetWinnerSide(blWin, false);
	
	m_GameClientView.SetBankerScoreEX(pGameFree->lBankerWinScore);
	CopyMemory(m_lExchangeScore,pGameFree->lExchangeGold,sizeof(m_lExchangeScore));
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		if(m_lExchangeScore[i] > 0)
			m_GameClientView.m_lExchangeScore[SwitchViewChairID(i)] = m_lExchangeScore[i];
	}
	ZeroMemory(m_GameClientView.m_lMeCurGameReturnScore,sizeof(m_GameClientView.m_lMeCurGameReturnScore));
	
	for (int nAreaIndex=1; nAreaIndex<=AREA_COUNT; ++nAreaIndex) SetMePlaceJetton(nAreaIndex,0);

	m_GameClientView.CleanUserJetton();
	ZeroMemory(&m_TempData,sizeof(m_TempData));
	
	//���³ɼ�
	//for (WORD wUserIndex = 0; wUserIndex < MAX_CHAIR; ++wUserIndex)
	//{
	//	IClientUserItem *pClientUserItem = GetTableUserItem(wUserIndex);

	//	if ( pClientUserItem == NULL ) continue;
	//	tagApplyUser ApplyUser ;

	//	//������Ϣ
	//	ApplyUser.lUserScore = pClientUserItem->GetUserScore();
	//	ApplyUser.strUserName = pClientUserItem->GetNickName();
	//	m_GameClientView.m_ApplyUser.UpdateUser(ApplyUser);
	//

	//}
	//��ɷ���
	m_GameClientView.SwitchToCheck();	

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


	if (!pPlaceJetton->bAndroid && CUserRight::IsGameCheatUser(m_pIClientKernel->GetUserAttribute()->dwUserRight) && m_GameClientView.m_hInst )
	{
		m_GameClientView.ControlBetDetection(pPlaceJetton->cbJettonArea,pPlaceJetton->lJettonScore,SwitchViewChairID(pPlaceJetton->wChairID),pPlaceJetton->wChairID);
	}

	if (GetMeChairID()!=pPlaceJetton->wChairID || IsLookonMode())
	{


		//��ע����
		if(pPlaceJetton->cbAndroid == TRUE)
		{
			//����
			static WORD wStFluc = 1;	//�������
			tagAndroidBet androidBet = {};
			androidBet.cbJettonArea = pPlaceJetton->cbJettonArea;
			androidBet.lJettonScore = pPlaceJetton->lJettonScore;
			androidBet.wChairID = pPlaceJetton->wChairID;
			androidBet.nLeftTime = ((rand()+androidBet.wChairID+wStFluc*3)%10+1)*100;
			wStFluc = wStFluc%3 + 1;

			m_ListAndroid.AddTail(androidBet);
		}
		else
		{
			m_GameClientView.PlaceUserJetton(pPlaceJetton->cbJettonArea,pPlaceJetton->lJettonScore);
			
			//��������
		//	if (m_pGlobalUnits->m_bAllowSound) 
			{
				/*if (pPlaceJetton->wChairID!=GetMeChairID() || IsLookonMode())
				{
					if (pPlaceJetton->lJettonScore==5000000) PlayGameSound(AfxGetInstanceHandle(),TEXT("ADD_GOLD_EX"));
					else PlayGameSound(AfxGetInstanceHandle(),TEXT("ADD_GOLD"));

					if(rand()%100 > 80 )
					{
						switch(rand()%3)
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
						}
					}	
				}*/
			}
		}
	}

	UpdateButtonContron();
    
	return true;
}

//��Ϸ�´�С
bool CGameClientEngine::OnSubGameCompare(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_Compare));
	if (wDataSize!=sizeof(CMD_S_Compare)) return false;

	//��Ϣ����
	CMD_S_Compare * pCompare=(CMD_S_Compare *)pBuffer;
	m_cbCompareTime=pCompare->cbTimeLeave;

	//����ʱ��
	//SetGameClock(GetMeChairID(),IDI_GAME_COMPARE, m_cbCompareTime);
	//����״̬
	SetGameStatus(GS_COMPARE_END);
	m_GameClientView.StartFlashTip(false,false);
	m_GameClientView.m_bGuess=true;	
	m_GameClientView.SetMoveCardTimer();
	m_bGuess=true;	

	////��������
	bool blWin[AREA_COUNT+1];
	for (int i = 0;i<=AREA_COUNT;i++)
	{
		blWin[i]=false;
	}	

	m_GameClientView.SetWinnerSide(blWin, false);	

	//���¿ؼ�
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
	m_GameClientView.m_bTip=false;
	
	m_GameEndTime = pGameEnd->cbTimeLeave;
	m_GameClientView.m_nProbability=pGameEnd->nProbability;
	m_GameClientView.m_bShowJettonIndex=pGameEnd->nRand;	
	m_lMyGameEndScore=pGameEnd->lUserScore;
	//m_GameClientView.StartHandle_Leave();

	//����ʱ��
	SetGameClock(GetMeChairID(),IDI_DISPATCH_CARD, pGameEnd->cbTimeLeave);
	CopyMemory(m_lLastUserJetton,m_lUserJettonScore,sizeof(m_lLastUserJetton));

	//�˿���Ϣ
	m_GameClientView.SetCardInfo(pGameEnd->cbTableCardArray);
	m_GameClientView.ClearAreaFlash();

	/*CString cs;
	cs.Format(TEXT("�н������ǣ�%d"),pGameEnd->cbTableCardArray[1]);
	m_pIStringMessage->InsertSystemString(cs);*/

	//�O�Ó���Ƅ�
	m_GameClientView.StartRunCar(20);

	//����״̬
	SetGameStatus(GS_GAME_END);

	m_cbLeftCardCount=pGameEnd->cbLeftCardCount;

	//ׯ����Ϣ
	m_GameClientView.SetBankerScore(pGameEnd->nBankerTime, pGameEnd->lBankerTotallScore);	

	m_GameClientView.PerformAllBetAnimation();
	CopyMemory(m_lUserReturnScore,pGameEnd->lUserReturnScore,sizeof(m_lUserReturnScore));

	m_TempData.a = pGameEnd->lBankerTotallScore;	
	m_TempData.c = pGameEnd->nBankerTime;
	m_TempData.d = pGameEnd->lRevenue;
	
	//���¿ؼ�
	UpdateButtonContron();	
	//ֹͣ����
//	for (int i=0; i<CountArray(m_DTSDCheer); ++i) m_DTSDCheer[i].Stop();

	return true;
}

//���¿���
void CGameClientEngine::UpdateButtonContron()
{
	/*if(__TEST__)
	{
		return ;
	}*/
	//�����ж�
	bool bEnablePlaceJetton=true;

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
	m_GameClientView.SetEnablePlaceJetton(bEnablePlaceJetton);

	//��ע��ť
	if (bEnablePlaceJetton==true)
	{		
		//�������
		//LONGLONG lCurrentJetton=m_GameClientView.GetCurrentJetton();
		LONGLONG lLeaveScore=m_lMeMaxScore;
		for (int nAreaIndex=1; nAreaIndex<=AREA_COUNT; ++nAreaIndex) lLeaveScore -= m_lUserJettonScore[nAreaIndex];

		if(m_GameClientView.m_nTime==100)
		{
			if(lLeaveScore/m_GameClientView.m_nTime<=0)
				m_GameClientView.m_nTime=10;
			if(lLeaveScore/m_GameClientView.m_nTime<=0)
				m_GameClientView.m_nTime=1;
		}
		if(m_GameClientView.m_nTime==10)
		{
			if(lLeaveScore/m_GameClientView.m_nTime<=0)
				m_GameClientView.m_nTime=1;			
		}

		if(lLeaveScore>0)
		{
			m_GameClientView.m_btJetton100.EnableWindow(TRUE);		
			m_GameClientView.m_btJetton1000.EnableWindow(TRUE);
			m_GameClientView.m_btJetton50000.EnableWindow(TRUE);
			m_GameClientView.m_btJetton10000.EnableWindow(TRUE);
			m_GameClientView.m_btJetton100000.EnableWindow(TRUE);
			m_GameClientView.m_btJetton500000.EnableWindow(TRUE);
			m_GameClientView.m_btJetton1000000.EnableWindow(TRUE);
			m_GameClientView.m_btJetton5000000.EnableWindow(TRUE);
			m_GameClientView.m_btContinueJetton.EnableWindow(TRUE);
		}
	}
	else
	{
		//���ù��
		//m_GameClientView.SetCurrentJetton(0L);

		//��ֹ��ť
		/*m_GameClientView.m_btJetton100.EnableWindow(FALSE);		
		m_GameClientView.m_btJetton1000.EnableWindow(FALSE);
		m_GameClientView.m_btJetton50000.EnableWindow(FALSE);
		m_GameClientView.m_btJetton10000.EnableWindow(FALSE);	
		m_GameClientView.m_btJetton100000.EnableWindow(FALSE);
		m_GameClientView.m_btJetton500000.EnableWindow(FALSE);
		m_GameClientView.m_btJetton1000000.EnableWindow(FALSE);
		m_GameClientView.m_btJetton5000000.EnableWindow(FALSE);
		m_GameClientView.m_btContinueJetton.EnableWindow(FALSE);*/
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
		if (m_lExchangeScore[GetMeChairID()]<m_lApplyBankerCondition) bEnableApply=false;
		m_GameClientView.m_btApplyBanker.EnableWindow(bEnableApply?TRUE:FALSE);

		//ȡ����ť
		bool bEnableCancel=true;
		if (m_wCurrentBanker==GetMeChairID() && GetGameStatus()!=GS_GAME_FREE) bEnableCancel=false;
		if (m_bMeApplyBanker==false) bEnableCancel=false;
		m_GameClientView.m_btCancelBanker.EnableWindow(bEnableCancel?TRUE:FALSE);
		m_GameClientView.m_btCancelBanker.SetButtonImage(m_wCurrentBanker==GetMeChairID()?IDB_BT_CANCEL_BANKER:IDB_BT_CANCEL_APPLY,AfxGetInstanceHandle(),false,false);

		//��ʾ�ж�
		if (m_bMeApplyBanker)
		{
			m_GameClientView.m_btApplyBanker.ShowWindow(SW_HIDE);
			m_GameClientView.m_btCancelBanker.ShowWindow(SW_SHOW);
			
		}
		else
		{
			m_GameClientView.m_btCancelBanker.ShowWindow(SW_HIDE);
			m_GameClientView.m_btApplyBanker.ShowWindow(SW_SHOW);
		}
	}
	else
	{
		m_GameClientView.m_btCancelBanker.EnableWindow(FALSE);
		m_GameClientView.m_btApplyBanker.EnableWindow(FALSE);
		m_GameClientView.m_btApplyBanker.ShowWindow(SW_SHOW);
		m_GameClientView.m_btCancelBanker.ShowWindow(SW_HIDE);	
	}

	if (!IsLookonMode())
	{
		//�жϴ�С��ť
		bool bEnableCompare=true;
		if (m_wCurrentBanker==GetMeChairID()) bEnableCompare=false;
		if (GetGameStatus()!=GS_COMPARE_END) bEnableCompare=false;
		if(m_GameClientView.m_lMeCurGameReturnScore[GetMeChairID()]==0) bEnableCompare=false;

		//��ʾ�ж�
		if (bEnableCompare)
		{
			m_GameClientView.m_btSmall.EnableWindow(TRUE);
			m_GameClientView.m_btBig.EnableWindow(TRUE);
		}
		else
		{
			/*m_GameClientView.m_btSmall.EnableWindow(FALSE);
			m_GameClientView.m_btBig.EnableWindow(FALSE);*/
		}
	}
	//��ȡ��Ϣ
	//IClientUserItem *pMeUserData=GetTableUserItem(GetMeChairID());

//#ifdef __SPECIAL___
//	//���а�ť
//	m_GameClientView.m_btBankerDraw.EnableWindow(TRUE);
//	m_GameClientView.m_btBankerStorage.EnableWindow(FALSE);
//	m_GameClientView.m_btBankerStorage.ShowWindow(SW_SHOW);
//	m_GameClientView.m_btBankerStorage.EnableWindow((GetGameStatus()==GS_GAME_FREE || IsLookonMode())?TRUE:FALSE);
//	if(m_wCurrentBanker==GetMeChairID()&&IsLookonMode()==false)
//	{
//		m_GameClientView.m_btBankerStorage.EnableWindow(FALSE);
//	}
//	if (m_wCurrentBanker==GetMeChairID()) m_GameClientView.m_blCanStore = false;
//	else  m_GameClientView.m_blCanStore = true;
//	m_GameClientView.SetInsureOption(false, m_GameClientView.m_blCanStore);
//#endif

	return;
}
bool CGameClientEngine::OnSubCheckImageIndex(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_C_CheckImage));
	if (wDataSize!=sizeof(CMD_C_CheckImage)) return false;

	CMD_C_CheckImage * pCheckImage=(CMD_C_CheckImage *)pBuffer;
	m_GameClientView.m_ShowImageIndex = pCheckImage->Index;

	return true;


}

//�´�С
LRESULT CGameClientEngine::OnGuess(WPARAM wParam, LPARAM lParam)
{
	//״̬�ж�
	if (GetGameStatus()!=GS_COMPARE_END||m_lUserReturnScore[GetMeChairID()]<=0)
	{
		UpdateButtonContron();
		return 0;
	}
	BOOL bBig=false;
	if(wParam==0) bBig=false;
	if(wParam==1) bBig=true;

	if(m_bGuess)
	{
		CMD_C_Guess Guess;
		ZeroMemory(&Guess,sizeof(Guess));
		Guess.bBig=bBig;
		//������Ϣ
		SendSocketData(SUB_C_GUESS,&Guess,sizeof(Guess));
		m_bGuess=false;
	}
	
	return 0;
}
//�һ�����
LRESULT CGameClientEngine::OnChangeGold(WPARAM wParam, LPARAM lParam)
{	
	if(wParam>0)
	{
		m_lChangeGold+=(LONGLONG)wParam;
		m_lUserScore-=(LONGLONG)wParam*m_nExchange;
		m_lAddGold-=(LONGLONG)wParam*m_nExchange;
	}
	if(lParam>0)
	{
		m_lChangeGold+=m_lUserScore/m_nExchange;
		if(m_lChangeGold>99999999999)
		{
			m_lChangeGold=99999999999;
			m_lAddGold = -1*99999999999*m_nExchange;
		}		
		m_lUserScore=0;
		m_lAddGold=-1*m_lChangeGold*m_nExchange;
	}

	LONGLONG lLeftScore = 99999999999 - m_lExchangeScore[GetMeChairID()];
	LONGLONG lTempScore=m_lUserScore/m_nExchange;

	if(lTempScore>=10000 && lLeftScore >= 10000)
	{
		m_GameClientView.m_btChip100.EnableWindow(TRUE);
		m_GameClientView.m_btChip1000.EnableWindow(TRUE);
		m_GameClientView.m_btChip10000.EnableWindow(TRUE);
		m_GameClientView.m_btChipAll.EnableWindow(TRUE);
	}
	else if(lTempScore >= 1000 && lLeftScore >= 1000 && lTempScore < 10000)
	{
		m_GameClientView.m_btChip100.EnableWindow(TRUE);
		m_GameClientView.m_btChip1000.EnableWindow(TRUE);
		m_GameClientView.m_btChip10000.EnableWindow(FALSE);	
		m_GameClientView.m_btChipAll.EnableWindow(TRUE);
	}
	else if(lTempScore >= 100 && lLeftScore >= 100 && lTempScore < 1000)
	{
		m_GameClientView.m_btChip100.EnableWindow(TRUE);
		m_GameClientView.m_btChip1000.EnableWindow(FALSE);
		m_GameClientView.m_btChip10000.EnableWindow(FALSE);
		m_GameClientView.m_btChipAll.EnableWindow(TRUE);
	}
	else if(lTempScore > 0 && lLeftScore > 0 && lTempScore < 100)
	{
		m_GameClientView.m_btChip100.EnableWindow(FALSE);
		m_GameClientView.m_btChip1000.EnableWindow(FALSE);
		m_GameClientView.m_btChip10000.EnableWindow(FALSE);
		m_GameClientView.m_btChipAll.EnableWindow(TRUE);
	}
	else
	{
		m_GameClientView.m_btChip100.EnableWindow(FALSE);
		m_GameClientView.m_btChip1000.EnableWindow(FALSE);
		m_GameClientView.m_btChip10000.EnableWindow(FALSE);
		m_GameClientView.m_btChipAll.EnableWindow(FALSE);
	}
	if(lParam>0) m_GameClientView.m_btChipAll.EnableWindow(FALSE);
	m_GameClientView.GetExchangeScore(m_lUserScore,m_nExchange,m_lChangeGold);

	m_GameClientView.RefreshGameView();
	return 0;
}
//�һ��������
LRESULT CGameClientEngine::OnChageMoney(WPARAM wParam, LPARAM lParam)
{	
	m_GameClientView.m_bShow=true;
	m_GameClientView.m_btChip100.ShowWindow(SW_SHOW);
	m_GameClientView.m_btChip1000.ShowWindow(SW_SHOW);
	m_GameClientView.m_btChip10000.ShowWindow(SW_SHOW);
	m_GameClientView.m_btChipClose.ShowWindow(SW_SHOW);
	m_GameClientView.m_btChip_X.ShowWindow(SW_SHOW);
	m_GameClientView.m_btChipOK.ShowWindow(SW_SHOW);
	m_GameClientView.m_btChipAll.ShowWindow(SW_SHOW);
	
	
	SCORE lTempScore=m_lUserScore/m_nExchange;
	SCORE lLeftScore = 99999999999 - m_lExchangeScore[GetMeChairID()];	

	if(lTempScore>=10000 && lLeftScore>=10000)
	{
		m_GameClientView.m_btChip100.EnableWindow(TRUE);
		m_GameClientView.m_btChip1000.EnableWindow(TRUE);
		m_GameClientView.m_btChip10000.EnableWindow(TRUE);
		m_GameClientView.m_btChipAll.EnableWindow(TRUE);
	}
	else if(lTempScore >= 1000 && lLeftScore >= 1000 && lTempScore < 10000)
	{
		m_GameClientView.m_btChip100.EnableWindow(TRUE);
		m_GameClientView.m_btChip1000.EnableWindow(TRUE);
		m_GameClientView.m_btChip10000.EnableWindow(FALSE);
		m_GameClientView.m_btChipAll.EnableWindow(TRUE);
	}
	else if(lTempScore >= 100 && lLeftScore >= 100 && lTempScore < 1000)
	{
		m_GameClientView.m_btChip100.EnableWindow(TRUE);
		m_GameClientView.m_btChip1000.EnableWindow(FALSE);
		m_GameClientView.m_btChip10000.EnableWindow(FALSE);
		m_GameClientView.m_btChipAll.EnableWindow(TRUE);
	}
	else if(lTempScore > 0 && lLeftScore > 0 && lTempScore < 100)
	{
		m_GameClientView.m_btChip100.EnableWindow(FALSE);
		m_GameClientView.m_btChip1000.EnableWindow(FALSE);
		m_GameClientView.m_btChip10000.EnableWindow(FALSE);
		m_GameClientView.m_btChipAll.EnableWindow(TRUE);
	}
	else
	{
		m_GameClientView.m_btChip100.EnableWindow(FALSE);
		m_GameClientView.m_btChip1000.EnableWindow(FALSE);
		m_GameClientView.m_btChip10000.EnableWindow(FALSE);
		m_GameClientView.m_btChipAll.EnableWindow(FALSE);
	}

	m_GameClientView.GetExchangeScore(m_lUserScore,m_nExchange,0);	

	m_GameClientView.RefreshGameView();
	return 0;
}

//�رմ���
LRESULT CGameClientEngine::OnColse(WPARAM wParam, LPARAM lParam)
{
	m_GameClientView.m_bShow=false;
	m_GameClientView.m_btChip100.ShowWindow(SW_HIDE);
	m_GameClientView.m_btChip1000.ShowWindow(SW_HIDE);
	m_GameClientView.m_btChip10000.ShowWindow(SW_HIDE);
	m_GameClientView.m_btChipClose.ShowWindow(SW_HIDE);
	m_GameClientView.m_btChip_X.ShowWindow(SW_HIDE);
	m_GameClientView.m_btChipOK.ShowWindow(SW_HIDE);
	m_GameClientView.m_btChipAll.ShowWindow(SW_HIDE);
	m_lChangeGold=0;
	m_lUserScore =m_lUserScore - m_lAddGold;
	m_lAddGold = 0;

	m_GameClientView.RefreshGameView();
	return 0;
}

//ȷ��
LRESULT CGameClientEngine::OnSureChange(WPARAM wParam, LPARAM lParam)
{
	CMD_C_ChangeGold ChangeGold;
	ZeroMemory(&ChangeGold,sizeof(ChangeGold));

	//�������
	ChangeGold.lChangeGold=m_lChangeGold;

	if (m_lChangeGold>0)
	{
		m_lUserScore =m_lUserScore - m_lAddGold;
		m_lAddGold = 0;
		//������Ϣ
		SendSocketData(SUB_C_CHANGE_GOLD,&ChangeGold,sizeof(ChangeGold));
	}

	m_GameClientView.m_bShow=false;
	m_GameClientView.m_btChip100.ShowWindow(SW_HIDE);
	m_GameClientView.m_btChip1000.ShowWindow(SW_HIDE);
	m_GameClientView.m_btChip10000.ShowWindow(SW_HIDE);
	m_GameClientView.m_btChipClose.ShowWindow(SW_HIDE);
	m_GameClientView.m_btChip_X.ShowWindow(SW_HIDE);
	m_GameClientView.m_btChipOK.ShowWindow(SW_HIDE);
	m_GameClientView.m_btChipAll.ShowWindow(SW_HIDE);
	m_lChangeGold=0;

	m_GameClientView.RefreshGameView();
	return 0;
}

//��Ѻ
LRESULT CGameClientEngine::OnContinueJetton(WPARAM wParam, LPARAM lParam)
{
	LONGLONG lTotal=0;
	for(int i=1;i<=AREA_COUNT;i++)
		lTotal+=m_lLastUserJetton[i];
	if(lTotal>m_lExchangeScore[GetMeChairID()]||lTotal==0) return 0;

	//״̬�ж�
	if (GetGameStatus()!=GS_PLACE_JETTON||m_blUsing)
	{
		UpdateButtonContron();
		return 0;
	}
	
	m_blUsing = true;

	BYTE cbJettonArea=0;
	LONGLONG lJettonScore=0;
	for(int i=1;i<=AREA_COUNT;i++)
	{		
		if(m_lLastUserJetton[i]>0)
		{
			OnPlaceJetton(i,m_lLastUserJetton[i]);			
		}
	}	
		
	//���°�ť
	UpdateButtonContron();	
	return 0;
}
//��ע��Ϣ
LRESULT CGameClientEngine::OnPlaceJetton(WPARAM wParam, LPARAM lParam)
{
	//��������
	BYTE cbJettonArea=(BYTE)wParam;
	LONGLONG lJettonScore=(LONGLONG)lParam;

	//�Ϸ��ж�
	ASSERT(cbJettonArea>=1 && cbJettonArea<=AREA_COUNT);
	if (!(cbJettonArea>=1 && cbJettonArea<=AREA_COUNT)) return 0;	

	//ׯ���ж�
	if ( GetMeChairID() == m_wCurrentBanker ) return 0;
	//��ע�ж�
	if (m_lExchangeScore[GetMeChairID()] < lJettonScore || m_lUserJettonScore[cbJettonArea]==999) return 0;

	//״̬�ж�
	if (GetGameStatus()!=GS_PLACE_JETTON||m_lExchangeScore[GetMeChairID()]==0)
	{
		UpdateButtonContron();
		return 0;
	}
	
	//���ñ���	
	if(m_lUserJettonScore[cbJettonArea]+lJettonScore > 999)
	{
		m_bMoreJetton = true;
		lJettonScore = 999-m_lUserJettonScore[cbJettonArea];
		m_lUserJettonScore[cbJettonArea] += lJettonScore;		
		if (m_bMoreJetton)
		{
			m_lExchangeScore[GetMeChairID()]-=lJettonScore;
			m_GameClientView.m_lExchangeScore[SwitchViewChairID(GetMeChairID())]-=lJettonScore;
		}
		m_bMoreJetton = false;
	}
	else
	{
		m_lUserJettonScore[cbJettonArea] += lJettonScore;
		m_GameClientView.m_lExchangeScore[SwitchViewChairID(GetMeChairID())]=m_lExchangeScore[GetMeChairID()];
		m_lExchangeScore[GetMeChairID()]-=lJettonScore;
		m_GameClientView.m_lExchangeScore[SwitchViewChairID(GetMeChairID())]-=lJettonScore;
	}
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
	//PlayGameSound(AfxGetInstanceHandle(),TEXT("ADD_GOLD"));
	switch(cbJettonArea)
	{
	case 1:
		{
			PlayGameSound(AfxGetInstanceHandle(),TEXT("KEY_SOUND_1"));
			break;
		}
	case 2:
		{
			PlayGameSound(AfxGetInstanceHandle(),TEXT("KEY_SOUND_2"));
			break;
		}
	case 3:
		{
			PlayGameSound(AfxGetInstanceHandle(),TEXT("KEY_SOUND_3"));
			break;
		}
	case 4:
		{
			PlayGameSound(AfxGetInstanceHandle(),TEXT("KEY_SOUND_4"));
			break;
		}
	case 5:
		{
			PlayGameSound(AfxGetInstanceHandle(),TEXT("KEY_SOUND_5"));
			break;
		}
	case 6:
		{
			PlayGameSound(AfxGetInstanceHandle(),TEXT("KEY_SOUND_6"));
			break;
		}
	case 7:
		{
			PlayGameSound(AfxGetInstanceHandle(),TEXT("KEY_SOUND_7"));
			break;
		}
	case 8:
		{
			PlayGameSound(AfxGetInstanceHandle(),TEXT("KEY_SOUND_8"));
			break;
		}
	}

	//��������
	//if (m_pGlobalUnits->m_bAllowSound) 
	//{
		/*if (lJettonScore==5000000) PlayGameSound(AfxGetInstanceHandle(),TEXT("ADD_GOLD_EX"));
		else PlayGameSound(AfxGetInstanceHandle(),TEXT("ADD_GOLD"));
		if(rand()%100 > 80 )
		{
			switch(rand()%3)
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
			}
		}*/	
	//}

	return 0;

}
LRESULT  CGameClientEngine::OnPlaySound(WPARAM wParam, LPARAM lParam)
{	
	//if (m_pGlobalUnits->m_bAllowSound) 
	{
		if(lParam == 0)
		{
			PlayGameSound(AfxGetInstanceHandle(),TEXT("FRUIT_0"));
		}
		else if(lParam==1)
		{
			PlayGameSound(AfxGetInstanceHandle(),TEXT("FRUIT_1"));
		}		
		else if(lParam==2)
		{
			PlayGameSound(AfxGetInstanceHandle(),TEXT("FRUIT_2"));
		}
		else if(lParam==3)
		{
			PlayGameSound(AfxGetInstanceHandle(),TEXT("FRUIT_3"));
		}
		else if(lParam==4)
		{
			PlayGameSound(AfxGetInstanceHandle(),TEXT("FRUIT_4"));
		}
		else if(lParam==5)
		{
			PlayGameSound(AfxGetInstanceHandle(),TEXT("FRUIT_5"));
		}
		else if(lParam==6)
		{
			PlayGameSound(AfxGetInstanceHandle(),TEXT("FRUIT_6"));
		}
		else if(lParam==7)
		{
			PlayGameSound(AfxGetInstanceHandle(),TEXT("FRUIT_7"));
		}
		else if(lParam==8)
		{
			PlayGameSound(AfxGetInstanceHandle(),TEXT("FRUIT_8"));
		}
		else if(lParam==9)
		{
			StopSound();
			PlayBackGroundSound(AfxGetInstanceHandle(),TEXT("Rand2"));
			//PlayGameSound(AfxGetInstanceHandle(),TEXT("Rand2"));
		}
		else if(lParam==10)
		{
			StopSound();
			PlayBackGroundSound(AfxGetInstanceHandle(),TEXT("Rand1"));
			//PlayGameSound(AfxGetInstanceHandle(),TEXT("Rand1"));
		}
		else if(lParam==11)
		{
			StopSound();
			PlayBackGroundSound(AfxGetInstanceHandle(),TEXT("Rand0"));
			//PlayGameSound(AfxGetInstanceHandle(),TEXT("Rand0"));
		}
		else if(lParam==12)
		{
			//PlayGameSound(AfxGetInstanceHandle(),TEXT("ADD_GOLD"));
		}
		else if(lParam==13)
		{
			PlayGameSound(AfxGetInstanceHandle(),TEXT("DISPATCH_CARD"));
		}
		else if(lParam==14)
		{
			PlayGameSound(AfxGetInstanceHandle(),TEXT("LUCKY_LOSE"));
		}
		else if(lParam==15)
		{
			//StopSound();
			PlayGameSound(AfxGetInstanceHandle(),TEXT("DU_WIN"));
		}
		else if(lParam==16)
		{
			//StopSound();
			PlayGameSound(AfxGetInstanceHandle(),TEXT("DU_LOSE"));
		}
	}
	

	return 1;
		
}

LRESULT  CGameClientEngine::OnShowResult(WPARAM wParam, LPARAM lParam)
{

	TCHAR szBuffer[128] = TEXT("");
	_sntprintf(szBuffer,CountArray(szBuffer),TEXT(" ���ַ��� %I64d ���ط��� %I64d ׯ�ҷ��� %I64d "),m_TempData.a,m_TempData.b,m_TempData.c);
	//InsertGeneralString(szBuffer,RGB(255,0,255),true);

	return 1;

}
//�ֹ�����
LRESULT  CGameClientEngine::OnOpenCard(WPARAM wParam, LPARAM lParam)
{
	if (GetGameStatus()==GS_GAME_END)
	{
		//InsertGeneralString(TEXT("��ϵͳ��ʾ�ݿ���ʱ�䲻���л�����ģʽ��"),RGB(255,0,255),true);
		return 1;

	}
	m_GameClientView.m_blAutoOpenCard = false;
	//InsertGeneralString(TEXT("��ϵͳ��ʾ����ѡ�����ֹ�����ģʽ�����ƺ�������ʹ������϶��˿ˣ�"),RGB(255,0,255),true);
	return 1;
}
//�Զ�����
LRESULT  CGameClientEngine::OnAutoOpenCard(WPARAM wParam, LPARAM lParam)
{
	if (GetGameStatus()==GS_GAME_END)
	{
		//InsertGeneralString(TEXT("��ϵͳ��ʾ�ݿ���ʱ�䲻���л�����ģʽ��"),RGB(255,0,255),true);
		return 1;

	}
	m_GameClientView.m_blAutoOpenCard = true;
	//InsertGeneralString(TEXT("��ϵͳ��ʾ����ѡ�����Զ�����ģʽ�����ƺ�ϵͳ���Զ��������ҵ��ƣ�"),RGB(255,0,255),true);

	return 1;
}
//��������
LRESULT CGameClientEngine::OnContinueCard(WPARAM wParam, LPARAM lParam)
{
	CString cs;
	cs.Format(TEXT("m_nGuessNum=%d"),0);
	//m_pIStringMessage->InsertSystemString(cs);
	//�Ϸ��ж�
	//if (GetMeChairID()!=m_wCurrentBanker) return 0;
	//if (GetGameStatus()!=GS_GAME_FREE) return 0;
	//if (m_cbLeftCardCount < 8) return 0;
	//if (IsLookonMode()) return 0;

	////������Ϣ
	////SendSocketData(SUB_C_CONTINUE_CARD);
 //   CMD_C_CheckImage CheckImage;
 //   CheckImage.Index =   m_GameClientView.m_CheckImagIndex;
	//SendSocketData(SUB_C_CHECK_IMAGE,&CheckImage,sizeof(CheckImage));
	//���ð�ť

	return 0;
}

//������Ϣ
LRESULT CGameClientEngine::OnApplyBanker(WPARAM wParam, LPARAM lParam)
{
	//�Ϸ��ж�
	IClientUserItem *pMeUserData = GetTableUserItem( GetMeChairID() );
	if (m_lExchangeScore[GetMeChairID()] < m_lApplyBankerCondition) return true;

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
	m_GameClientView.m_btCancelBanker.EnableWindow(FALSE);
	m_GameClientView.m_btApplyBanker.EnableWindow(FALSE);

	return true;
}

//�´�С
bool CGameClientEngine::OnSubGuess(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_Guess));
	if (wDataSize!=sizeof(CMD_S_Guess)) return false;

	//��Ϣ����
	CMD_S_Guess * pGuess=(CMD_S_Guess *)pBuffer;

	//�ɼ���Ϣ	
	m_GameClientView.SetCurGameScore(0,pGuess->lUserReturnScore,SwitchViewChairID(pGuess->wChairID));
	
	
	//ׯ����Ϣ
	if(pGuess->lUserReturnScore>0 && pGuess->wChairID==GetMeChairID())
	{
		OnPlaySound(0,15);
		m_GameClientView.StartFlashTip(true,true);

		CString strBuf;
		strBuf.Format(TEXT("��ϲ����������: +%I64d"),pGuess->lUserReturnScore/2);
		m_pIStringMessage->InsertSystemString(strBuf);
	}
	if(pGuess->lUserReturnScore==0 && pGuess->wChairID==GetMeChairID())
	{
		OnPlaySound(0,16);
		m_GameClientView.StartFlashTip(true,false);

		CString strBuf;
		strBuf.Format(TEXT("���ź������´���: -%I64d"),m_lUserReturnScore[GetMeChairID()]);
		m_pIStringMessage->InsertSystemString(strBuf);

	}
	
	if(pGuess->nGuessNum!=0 && pGuess->wChairID==GetMeChairID())
	{
		m_GameClientView.m_nGuessNum=pGuess->nGuessNum;
		m_GameClientView.KillCardTime();
	}	
	m_GameClientView.RefreshGameView();
	
	
	/*CString cs;
	cs.Format(TEXT("%I64d====%I64d====%I64d"),m_TempData.a,m_TempData.b,pGuess->lUserReturnScore);
	m_pIStringMessage->InsertSystemString(cs);*/
	//m_TempData.b=pGuess->lUserReturnScore;	
	//m_GameClientView.m_btBig.EnableWindow(FALSE);
	//m_GameClientView.m_btSmall.EnableWindow(FALSE);

	return true;
}

//�һ�����
bool CGameClientEngine::OnSubChangeGold(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_ChangeGold));
	if (wDataSize!=sizeof(CMD_S_ChangeGold)) return false;

	//��Ϣ����
	CMD_S_ChangeGold * pChangeGold=(CMD_S_ChangeGold *)pBuffer;

	CopyMemory(m_lExchangeScore,pChangeGold->lExchangeGold,sizeof(m_lExchangeScore));
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		if(m_lExchangeScore[i] > 0)
			m_GameClientView.m_lExchangeScore[SwitchViewChairID(i)] = m_lExchangeScore[i];
	}	
	
	if (pChangeGold->wChairID == GetMeChairID())
	{
		LONGLONG lSumBet = 0;

		if (GetGameStatus() == GS_PLACE_JETTON)
		{
			for(int i=0; i<AREA_COUNT+1; i++) lSumBet += m_lUserJettonScore[i];
		}
		
		IClientUserItem *pMeUserData=GetTableUserItem(pChangeGold->wChairID);
		m_lUserScore=pMeUserData->GetUserScore()-(m_lExchangeScore[pChangeGold->wChairID]+lSumBet)*m_nExchange;
		m_lMeMaxScore=m_lExchangeScore[GetMeChairID()];
		m_GameClientView.SetMeMaxScore(m_lMeMaxScore);
	}

	if (pChangeGold->wChairID == m_wCurrentBanker)
	{
		SetBankerInfo(m_wCurrentBanker,m_lExchangeScore[pChangeGold->wChairID]);
	}	

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
	IClientUserItem *pClientUserItem=GetTableUserItem(pApplyBanker->wApplyUser);

	//�������
	if (m_wCurrentBanker != pApplyBanker->wApplyUser)
	{
		m_GameClientView.m_ValleysList.Add( SwitchViewChairID(pApplyBanker->wApplyUser) );
	}

	//�Լ��ж�
	if (IsLookonMode()==false && GetMeChairID()==pApplyBanker->wApplyUser) m_bMeApplyBanker=true;

	//���¿ؼ�
	UpdateButtonContron();
	m_GameClientView.m_btCancelBanker.EnableWindow(TRUE);

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

	for( int i = 0; i < m_GameClientView.m_ValleysList.GetCount(); ++i )
	{
		if ( SwitchViewChairID(pCancelBanker->wCancelUser) == m_GameClientView.m_ValleysList[i] )
		{
			m_GameClientView.m_ValleysList.RemoveAt(i);
			break;
		}
	}

	//�Լ��ж�
	IClientUserItem *pMeUserData=GetTableUserItem(GetMeChairID());
	if (IsLookonMode()==false && pCancelBanker->wCancelUser == GetMeChairID()) m_bMeApplyBanker=false;
	
	//���¿ؼ�
	UpdateButtonContron();

	m_GameClientView.m_btApplyBanker.EnableWindow(TRUE);

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
	ASSERT(wDataSize%sizeof(tagServerGameRecord)==0);
	if (wDataSize%sizeof(tagServerGameRecord)!=0) return false;

	//���ü�¼
	WORD wRecordCount=wDataSize/sizeof(tagServerGameRecord);
	for (WORD wIndex=0;wIndex<wRecordCount;wIndex++) 
	{
		tagServerGameRecord * pServerGameRecord=(((tagServerGameRecord *)pBuffer)+wIndex);

		m_GameClientView.SetGameHistory(pServerGameRecord->bWinMen);
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
	m_GameClientView.PlaceUserJetton(pPlaceJettonFail->lJettonArea,-pPlaceJettonFail->lPlaceScore);

	//�Լ��ж�
	if (GetMeChairID()==pPlaceJettonFail->wPlaceUser && false==IsLookonMode())
	{
		LONGLONG lJettonCount=pPlaceJettonFail->lPlaceScore;
		//�Ϸ�У��
		ASSERT(m_lUserJettonScore[cbViewIndex]>=lJettonCount);
		if (lJettonCount>m_lUserJettonScore[cbViewIndex]) return false;

		//������ע
		m_lUserJettonScore[cbViewIndex]-=lJettonCount;
		m_GameClientView.SetMePlaceJetton(cbViewIndex,m_lUserJettonScore[cbViewIndex]);		
		m_lExchangeScore[GetMeChairID()]+=lJettonCount;
		m_GameClientView.m_lExchangeScore[SwitchViewChairID(GetMeChairID())]=m_lExchangeScore[GetMeChairID()];
	}

	return true;
}

//����ׯ��
void CGameClientEngine::SetBankerInfo(WORD wBanker,LONGLONG lScore)
{
	m_wCurrentBanker=wBanker;
	m_lBankerScore=lScore;
	IClientUserItem *pClientUserItem=m_wCurrentBanker==INVALID_CHAIR ? NULL : GetTableUserItem(m_wCurrentBanker);
	DWORD dwBankerUserID = (NULL==pClientUserItem) ? INVALID_CHAIR : pClientUserItem->GetChairID();
	m_GameClientView.SetBankerInfo(dwBankerUserID,m_lBankerScore);

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


LRESULT CGameClientEngine::OnStorage(WPARAM wParam,LPARAM lParam)
{
	SendSocketData(SUB_C_STORAGE,(CMD_C_FreshStorage*)wParam,sizeof(CMD_C_FreshStorage));
	return true;
}

LRESULT CGameClientEngine::OnStorageInfo(WPARAM wParam,LPARAM lParam)
{
	SendSocketData(SUB_C_STORAGEINFO,(CMD_C_StorageInfo*)wParam,sizeof(CMD_C_StorageInfo));
	return true;
}
LRESULT CGameClientEngine::OnDeuct(WPARAM wParam,LPARAM lParam)
{
	SendSocketData(SUB_C_DEUCT,(CMD_C_FreshDeuct*)wParam,sizeof(CMD_C_FreshDeuct));
	return true;
}

//����
bool CGameClientEngine::OnSubAdminControl( const void * pBuffer, WORD wDataSize )
{
	ASSERT(wDataSize==sizeof(CMD_S_ControlReturns));
	if(wDataSize!=sizeof(CMD_S_ControlReturns)) return false;

	if( NULL != m_GameClientView.m_pClientControlDlg && NULL != m_GameClientView.m_pClientControlDlg->GetSafeHwnd() )
	{
		CMD_S_ControlReturns* pResult = (CMD_S_ControlReturns*)pBuffer;
		m_GameClientView.m_pClientControlDlg->UpdateControl(pResult);
	}
	return true;
}

void CGameClientEngine::OnTimer(UINT nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
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
					placeJetton.bAndroid=true;
					placeJetton.cbJettonArea = androidBet.cbJettonArea;
					placeJetton.lJettonScore = androidBet.lJettonScore;
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


//��������
bool CGameClientEngine::PlayBackGroundSound(HINSTANCE hInstance, LPCTSTR pszSoundName)
{
	//�����ж�
	if (AfxGetMainWnd() == NULL) return false;
	if (AfxGetMainWnd()->IsWindowVisible()==FALSE) return false;

	//������Դ
	HRSRC hResource=FindResource(hInstance,pszSoundName,TEXT("WAVE"));
	if (hResource==NULL) return false;

	//������Դ
	HGLOBAL hGlobal=LoadResource(hInstance,hResource);
	if (hGlobal==NULL) return false;

	//��������
	LPCTSTR pszMemory=(LPCTSTR)LockResource(hGlobal);
	PlaySound(pszMemory,hInstance,SND_ASYNC|SND_MEMORY|SND_LOOP);

	//������Դ
	UnlockResource(hGlobal);
	FreeResource(hGlobal);

	return true;
}


//ֹͣ����
bool CGameClientEngine::StopSound()
{
	if (AfxGetMainWnd() == NULL) return false;
	if (AfxGetMainWnd()->IsWindowVisible()==FALSE) return false;

	//ֹͣ����
	return PlaySound(NULL, NULL, SND_PURGE);
}
