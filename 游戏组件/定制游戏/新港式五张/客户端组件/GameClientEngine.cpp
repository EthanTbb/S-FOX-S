#include "StdAfx.h"
#include "Resource.h"
#include "GameClient.h"
#include "GameClientEngine.h"

//////////////////////////////////////////////////////////////////////////

//��Ϸʱ��
#define IDI_ADD_SCORE				200									//��ע��ʱ��
#define IDI_START_GAME				201									//��ʼ��ʱ��

//��Ϸʱ��
#define TIME_ADD_SCORE				30									//��ע��ʱ��
#define TIME_START_GAME				15									//��ʼ��ʱ��

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameClientEngine, CGameFrameEngine)
	ON_MESSAGE(IDM_START,OnMessageStart)
	ON_MESSAGE(IDM_FOLLOW,OnMessageFollow)
	ON_MESSAGE(IDM_GIVE_UP,OnMessageGiveUp)
	ON_MESSAGE(IDM_ADD_SCORE,OnMessageAddScore)
	ON_MESSAGE(IDM_SHOW_HAND,OnMessageShowHand)
	ON_MESSAGE(IDM_GAME_CLOSE,OnMessageClose)
	ON_MESSAGE(IDM_SEND_CARD_FINISH,OnMessageSendCardFinish)
	ON_MESSAGE(IDM_CHAT_MESSAGE,OnChatMessage)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameClientEngine::CGameClientEngine()
{
	//��ע��Ϣ
	m_lDrawMaxScore=0L;
	m_lTurnMaxScore=0L;
	m_lTurnLessScore=0L;
	m_lServiceCharge = 0l;
	ZeroMemory(m_lUserScore,sizeof(m_lUserScore));
	ZeroMemory(m_lTableScore,sizeof(m_lTableScore));

	//״̬����
	m_bShowHand=false;
	m_wCurrentUser=INVALID_CHAIR;
	ZeroMemory(m_szAccounts,sizeof(m_szAccounts));
	ZeroMemory(m_cbPlayStatus,sizeof(m_cbPlayStatus));

	//��������
	ZeroMemory(&m_GameEndPacket,sizeof(m_GameEndPacket));
	m_bXiaZhu=true;
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
	HICON hIcon=LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME));
	if (m_pIClientKernel!=NULL) m_pIClientKernel->SetGameAttribute(KIND_ID,GAME_PLAYER,VERSION_CLIENT,hIcon,GAME_NAME);

	////��������
	//CGlobalUnits *pGlobalUnits=CGlobalUnits::GetInstance();
	//if ( pGlobalUnits->m_bAllowBackGroundSound )
	//{
	//	m_bBackGroundSound = true;
	//	PlayBackGroundSound(AfxGetInstanceHandle(), TEXT("BACK_GROUND"));
	//}
//	pGlobalUnits->m_bAllowSound = true;
	return true;
}

//��������
bool CGameClientEngine::OnResetGameEngine()
{
	//��ע��Ϣ
	m_lDrawMaxScore=0L;
	m_lTurnMaxScore=0L;
	m_lTurnLessScore=0L;
	ZeroMemory(m_lUserScore,sizeof(m_lUserScore));
	ZeroMemory(m_lTableScore,sizeof(m_lTableScore));

	//״̬����
	m_bShowHand=false;
	m_wCurrentUser=INVALID_CHAIR;
	ZeroMemory(m_szAccounts,sizeof(m_szAccounts));
	ZeroMemory(m_cbPlayStatus,sizeof(m_cbPlayStatus));

	//��������
	ZeroMemory(&m_GameEndPacket,sizeof(m_GameEndPacket));

	return true;
}


//�Թ�״̬
bool CGameClientEngine::OnEventLookonMode(VOID * pData, WORD wDataSize)
{
	return true;
}

//ʱ��ɾ��
bool CGameClientEngine::OnEventGameClockKill(WORD wChairID)
{
	return true;
}

//ʱ����Ϣ
bool CGameClientEngine::OnEventGameClockInfo(WORD wChairID, UINT nElapse, WORD wClockID)
{
	switch (wClockID)
	{
	case IDI_START_GAME:	//��Ϸ��ʼ
		{
			//�ر���Ϸ
			if ((nElapse==0)&&(IsLookonMode()==false)&&(wChairID==GetMeChairID()))
			{
				AfxGetMainWnd()->PostMessage(WM_CLOSE);
				return true;
			}

			//��ʱ����
			if ((nElapse<=5)&&(wChairID==GetMeChairID())&&(IsLookonMode()==false))
			{
				PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_WARN"));
			}

			return true;
		}
	case IDI_ADD_SCORE:		//�û���ע
		{
			//�Զ�����
			if (nElapse==0)
			{
				if ((IsLookonMode()==false)&&(wChairID==GetMeChairID())) OnMessageGiveUp(0,0);
				return true;
			}

			//��ʱ����
			if ((nElapse<=5)&&(wChairID==GetMeChairID())&&(IsLookonMode()==false))
			{
				PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_WARN"));
			}

			return true;
		}
	}

	return false;
}

//������Ϣ
bool CGameClientEngine::OnEventGameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_S_GAME_START:	//��Ϸ��ʼ
		{
			//��������
			m_GameClientView.ConcludeDispatch();
			return OnSubGameStart(pData,wDataSize);
		}
	case SUB_S_GIVE_UP:		//�û�����
		{
			//��������
			m_GameClientView.ConcludeDispatch();
			return OnSubGiveUp(pData,wDataSize);
		}
	case SUB_S_ADD_SCORE:	//�û���ע
		{
			//��������
			m_GameClientView.ConcludeDispatch();
			return OnSubAddScore(pData,wDataSize);
		}
	case SUB_S_SEND_CARD:	//�����˿�
		{
			//��������
			m_GameClientView.ConcludeDispatch();
			return OnSubSendCard(pData,wDataSize);
		}
	case SUB_S_GAME_END:	//��Ϸ����
		{
			//��������
			m_GameClientView.ConcludeDispatch();
			return OnSubGameEnd(pData,wDataSize);
		}
	case SUB_S_TRUE_END:
		{
			return true;
		}
#ifdef CHEAT_VERSION
	case SUB_S_GET_WINNER:
		{
			//��Ϣ����
			return OnSubGetWinner( pData,wDataSize );
		}
#endif
	}

	//�������
	ASSERT(FALSE);

	return true;
}

//��Ϸ����
bool CGameClientEngine::OnEventSceneMessage(BYTE cbGameStatus, bool bLookonOther, VOID * pData, WORD wDataSize)
{
	switch (cbGameStatus)
	{
	case GAME_SCENE_FREE:	//����״̬
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_S_StatusFree));
			if (wDataSize!=sizeof(CMD_S_StatusFree)) return false;

			//��������
			CMD_S_StatusFree * pStatusFree=(CMD_S_StatusFree *)pData;

			//���ý���
			m_GameClientView.SetCellScore(pStatusFree->lCellScore);

			//�������
			IClientUserItem * pIClientUserItem=GetMeUserItem();
			if ((IsLookonMode()==false)&&(pIClientUserItem->GetUserStatus()!=US_READY))
			{
				tagServerAttribute *pServerAttribute=m_pIClientKernel->GetServerAttribute();
				bool bMatchMode=((pServerAttribute->wServerType&GAME_GENRE_MATCH)!=0);
				if(!bMatchMode)
				{
					PerformAutoStart();
					m_GameClientView.m_btStart.ShowWindow(TRUE);
				}
			}

			//��ʷ�ɼ�
			for (BYTE i=0;i<GAME_PLAYER;i++)
			{
				//��ȡ����
				tagHistoryScore * pHistoryScore=m_HistoryScore.GetHistoryScore(i);

				//���û���
				pHistoryScore->lTurnScore=pStatusFree->lTurnScore[i];
				pHistoryScore->lCollectScore=pStatusFree->lCollectScore[i];

				//������
				WORD wViewChairID=SwitchViewChairID(i);
				m_GameClientView.SetHistoryScore(wViewChairID,pHistoryScore);
			}

			return true;
		}
	case GAME_SCENE_PLAY:	//��Ϸ״̬
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_S_StatusPlay));
			if (wDataSize!=sizeof(CMD_S_StatusPlay)) return false;

			//��������
			CMD_S_StatusPlay * pStatusPlay=(CMD_S_StatusPlay *)pData;

			//��ע��Ϣ
			m_lCellScore = pStatusPlay->lCellScore;
			m_lServiceCharge = pStatusPlay->lServiceCharge;
			m_lDrawMaxScore=pStatusPlay->lDrawMaxScore;
			m_lTurnMaxScore=pStatusPlay->lTurnMaxScore;
			m_lTurnLessScore=pStatusPlay->lTurnLessScore;
			CopyMemory(m_lUserScore,pStatusPlay->lUserScore,sizeof(m_lUserScore));
			CopyMemory(m_lTableScore,pStatusPlay->lTableScore,sizeof(m_lTableScore));

			//״̬��Ϣ
			m_wCurrentUser=pStatusPlay->wCurrentUser;
			m_bShowHand=(pStatusPlay->cbShowHand==TRUE)?true:false;
			CopyMemory(m_cbPlayStatus,pStatusPlay->cbPlayStatus,sizeof(m_cbPlayStatus));

			//�ʺ�����
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				IClientUserItem * pIClientUserItem=GetTableUserItem(i);
				if (pIClientUserItem!=NULL) lstrcpyn(m_szAccounts[i],(pIClientUserItem->GetNickName()),CountArray(m_szAccounts[i]));
			}

			//�Թۿ���
			if ((IsLookonMode()==false)||(IsAllowLookon()==true))
			{
				m_GameClientView.m_CardControl[MYSELF_VIEW_ID].SetPositively(true);
			}

			//���ý���
			m_GameClientView.SetCellScore(pStatusPlay->lCellScore);
			m_GameClientView.SetDrawMaxScore(pStatusPlay->lDrawMaxScore);

			//���ý���
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				//��������
				WORD wViewChairID=SwitchViewChairID(i);
				IClientUserItem * pIClientUserItem=GetTableUserItem(i);

				//���û���
				if (pIClientUserItem!=NULL)
				{
					if (m_cbPlayStatus[i]==TRUE)
					{
						LONGLONG lUserScore=pIClientUserItem->GetUserScore();
						m_GameClientView.SetUserScore(wViewChairID,lUserScore - m_lUserScore[i] - m_lTableScore[i] - m_lServiceCharge);
					}
					else
					{
						m_GameClientView.SetUserScore(wViewChairID,pIClientUserItem->GetUserScore());
					}
				}

				//�����˿�
				if (m_lTableScore[i]!=0L)
				{
					if (m_cbPlayStatus[i]==FALSE)
					{
						BYTE cbCardData[MAX_COUNT];
						ZeroMemory(cbCardData,sizeof(cbCardData));
						m_GameClientView.m_CardControl[wViewChairID].SetCardData(cbCardData,pStatusPlay->cbCardCount[i]);
					}
					else
					{
						BYTE cbCardCount=pStatusPlay->cbCardCount[i];
						m_GameClientView.m_CardControl[wViewChairID].SetCardData(pStatusPlay->cbHandCardData[i],cbCardCount);
					}
				}

				//��������
				m_GameClientView.m_lPalyBetChip[wViewChairID] = m_lUserScore[i];
				m_GameClientView.m_lALLBetChip += m_lTableScore[i];
				m_GameClientView.SetUserTableScore(wViewChairID,m_lUserScore[i]+m_lTableScore[i]);
			}

			m_GameClientView.AddDeskChip( m_GameClientView.m_lALLBetChip, GAME_PLAYER );

			//��ǰ�û�
			if ((IsLookonMode()==false)&&(m_wCurrentUser==GetMeChairID())) UpdateScoreControl();

			//��ʷ�ɼ�
			for (BYTE i=0;i<GAME_PLAYER;i++)
			{
				//��ȡ����
				tagHistoryScore * pHistoryScore=m_HistoryScore.GetHistoryScore(i);

				//���û���
				pHistoryScore->lTurnScore=pStatusPlay->lTurnScore[i];
				pHistoryScore->lCollectScore=pStatusPlay->lCollectScore[i];

				//������
				WORD wViewChairID=SwitchViewChairID(i);
				m_GameClientView.SetHistoryScore(wViewChairID,pHistoryScore);
			}

			//����ʱ��
			SetGameClock(m_wCurrentUser,IDI_ADD_SCORE,TIME_ADD_SCORE);

//#ifdef CHEAT_VERSION
			////��ȡ��Ϣ
			//if( CUserRight::CanCheat(m_pIClientKernel->GetUserRight()) )
			//{
			//	//������Ϣ
			//	SendSocketData(SUB_C_GET_WINNER);
			//}
//#endif

			return true;
		}
	}

	return false;
}

//�û�����
VOID CGameClientEngine::OnEventUserEnter(IClientUserItem * pIClientUserItem, bool bLookonUser)
{
	//���óɼ�
	if (bLookonUser==false)
	{
		//��ʷ�ɼ�
		m_HistoryScore.OnEventUserEnter(pIClientUserItem->GetChairID());

		//���û���
		WORD wViewChairID=SwitchViewChairID(pIClientUserItem->GetChairID());
		m_GameClientView.SetUserScore(wViewChairID,pIClientUserItem->GetUserScore());

		//��������
		m_GameClientView.SetUserImage(wViewChairID, pIClientUserItem->GetChairID(), pIClientUserItem->GetGender() == 1);

		//���½���
		m_GameClientView.InvalidGameView(0,0,0,0);
	}

	return;
}

//�û��뿪
VOID CGameClientEngine::OnEventUserLeave(IClientUserItem * pIClientUserItem, bool bLookonUser)
{
	//�Զ���ʼ
	if (bLookonUser==false) 
	{
		tagServerAttribute *pServerAttribute=m_pIClientKernel->GetServerAttribute();
		bool bMatchMode=((pServerAttribute->wServerType&GAME_GENRE_MATCH)!=0);
		if(!bMatchMode)
		{
			PerformAutoStart();
		}
	}

	//���óɼ�
	if (bLookonUser==false)
	{
		//���ñ���
		m_HistoryScore.OnEventUserLeave(pIClientUserItem->GetChairID());

		//��������
		m_GameClientView.SetUserImage(SwitchViewChairID(pIClientUserItem->GetChairID()), INVALID_CHAIR, 0);

		//���½���
		m_GameClientView.InvalidGameView(0,0,0,0);
	}

	return;
}

//�û�����
VOID CGameClientEngine::OnEventUserScore(IClientUserItem * pIClientUserItem, bool bLookonUser)
{
	//���û���
	if (bLookonUser==false)
	{
		WORD wViewChairID=SwitchViewChairID(pIClientUserItem->GetChairID());
		m_GameClientView.SetUserSaveScore(wViewChairID,pIClientUserItem->GetUserScore());
	}

	return;
}

//�û�״̬
VOID CGameClientEngine::OnEventUserStatus(IClientUserItem * pIClientUserItem, bool bLookonUser)
{
	//��������
	IClientUserItem * pIMySelfUserItem=GetMeUserItem();
	BYTE cbUserStatus=pIClientUserItem->GetUserStatus();

	//�Զ���ʼ
	if ((pIClientUserItem!=pIMySelfUserItem)&&(cbUserStatus==US_READY)) 
	{
		tagServerAttribute *pServerAttribute=m_pIClientKernel->GetServerAttribute();
		bool bMatchMode=((pServerAttribute->wServerType&GAME_GENRE_MATCH)!=0);
		if(!bMatchMode)
		{
			PerformAutoStart();
		}
	}

	//ֹͣ��ʼ
	if ((pIClientUserItem==pIMySelfUserItem)&&(cbUserStatus==US_READY)) KillGameClock(IDI_START_GAME);

	return;
}

//��Ϸ��ʼ
bool CGameClientEngine::OnSubGameStart(VOID * pData, WORD wDataSize)
{
	//Ч�����
	ASSERT(wDataSize==sizeof(CMD_S_GameStart));
	if (wDataSize!=sizeof(CMD_S_GameStart)) return false;

	//��������
	CMD_S_GameStart * pGameStart=(CMD_S_GameStart *)pData;

	//����״̬
	SetGameStatus(GAME_SCENE_PLAY);
	//��ע��Ϣ
	m_lDrawMaxScore=0L;
	m_lTurnMaxScore=0L;
	m_lTurnLessScore=0L;
	ZeroMemory(m_lUserScore,sizeof(m_lUserScore));
	ZeroMemory(m_lTableScore,sizeof(m_lTableScore));

	//״̬����
	m_bShowHand=false;
	m_wCurrentUser=INVALID_CHAIR;
	ZeroMemory(m_szAccounts,sizeof(m_szAccounts));
	ZeroMemory(m_cbPlayStatus,sizeof(m_cbPlayStatus));
	m_bXiaZhu=true;
	//ɾ��ʱ��
	KillGameClock(IDI_START_GAME);

	//��ɷ���
	m_GameClientView.ConcludeDispatch();

	//���ý���
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		m_GameClientView.m_lPalyBetChip[i] = 0l;
		m_GameClientView.m_CardControl[i].SetCardData(NULL,0);
		m_GameClientView.SetOverScore(i, 0);
	}

	//��������
	m_GameClientView.SetDrawMaxScore(0L);
	m_GameClientView.SetUserAction(INVALID_CHAIR,0);
	m_GameClientView.SetUserTableScore(INVALID_CHAIR,0L);

	//���ÿؼ�
	m_GameClientView.m_btStart.ShowWindow(SW_HIDE);

	//״̬����
	m_bShowHand=false;
	m_wCurrentUser=pGameStart->wCurrentUser;
	ZeroMemory(m_szAccounts,sizeof(m_szAccounts));
	ZeroMemory(m_cbPlayStatus,sizeof(m_cbPlayStatus));

	//���ñ���
	m_lServiceCharge=pGameStart->lServiceCharge;
	m_lDrawMaxScore=pGameStart->lDrawMaxScore;
	m_lTurnMaxScore=pGameStart->lTurnMaxScore;
	m_lTurnLessScore=pGameStart->lTurnLessScore;
	ZeroMemory(m_lUserScore,sizeof(m_lUserScore));
	ZeroMemory(m_lTableScore,sizeof(m_lTableScore));

	//��������
	BYTE cbCardData[2][GAME_PLAYER];
	WORD wStartChairID=INVALID_CHAIR;
	ZeroMemory(cbCardData,sizeof(cbCardData));

	//ɾ������
	for ( int i = 0 ; i < m_GameClientView.m_ArrayPlayChip.GetCount(); ++i )
		SafeDelete(m_GameClientView.m_ArrayPlayChip[i]);
	m_GameClientView.m_ArrayPlayChip.RemoveAll();

	for ( int i = 0 ; i < m_GameClientView.m_ArrayBetChip.GetCount(); ++i )
		SafeDelete(m_GameClientView.m_ArrayBetChip[i]);
	m_GameClientView.m_ArrayBetChip.RemoveAll();

	for ( int i = 0 ; i < m_GameClientView.m_ArrayOverChip.GetCount(); ++i )
		SafeDelete(m_GameClientView.m_ArrayOverChip[i]);
	m_GameClientView.m_ArrayOverChip.RemoveAll();

	//���ý���
	m_GameClientView.SetUserAction(INVALID_CHAIR,0);
	m_GameClientView.SetDrawMaxScore(m_lDrawMaxScore);
	m_GameClientView.SetCellScore(pGameStart->lCellScore);
	m_lCellScore = pGameStart->lCellScore;

	//���ý���
	for (WORD i = 0;i < GAME_PLAYER; i++)
	{
		m_GameClientView.SetOverScore(i, 0);
	}

	//���ý���
	m_GameClientView.m_lALLBetChip = 0l;
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//��������
		WORD wViewChairID=SwitchViewChairID(i);
		IClientUserItem * pIClientUserItem=GetTableUserItem(i);

		//�����˿�
		if (pGameStart->cbCardData[i]!=0x00)
		{
			cbCardData[1][wViewChairID]=pGameStart->cbCardData[i];
			cbCardData[0][wViewChairID]=(wViewChairID==MYSELF_VIEW_ID)?pGameStart->cbObscureCard:0xFF;
		}

		//�������
		if (pGameStart->cbCardData[i]!=0x00)
		{
			//�����û�
			if (wStartChairID==INVALID_CHAIR) wStartChairID=wViewChairID;

			//״̬����
			m_cbPlayStatus[i]=TRUE;
			m_lTableScore[i]=pGameStart->lCellScore;
			lstrcpyn(m_szAccounts[i],(pIClientUserItem->GetNickName()),CountArray(m_szAccounts[i]));

			//���ó���
			m_GameClientView.m_lPalyBetChip[wViewChairID] = 0l;
			m_GameClientView.m_lALLBetChip += pGameStart->lCellScore;
			m_GameClientView.SetUserTableScore(wViewChairID,pGameStart->lCellScore);

			//�����������
			m_GameClientView.AddPlayChip( pGameStart->lCellScore, wViewChairID );

			//���û���
			ASSERT(pIClientUserItem!=NULL);
			LONGLONG lUserScore=pIClientUserItem->GetUserScore();
			m_GameClientView.SetUserScore(wViewChairID,lUserScore - m_lTableScore[i] - m_lUserScore[1] - m_lServiceCharge);
		}
		else
		{
			//���û���
			m_GameClientView.SetUserScore(wViewChairID,0L);

			//���ó���
			m_GameClientView.m_lPalyBetChip[wViewChairID] = 0l;
		}

		//�����˿�
		m_GameClientView.m_CardControl[i].SetCardData(NULL,0);
		m_GameClientView.m_CardControl[i].SetDisplayHead(false);
	}

	//�ɷ��˿�
	for (BYTE cbIndex=0;cbIndex<2;cbIndex++)
	{
		for (WORD i=0;i<GAME_PLAYER;i++)
		{
			if (m_cbPlayStatus[i]==TRUE)
			{
				//��������
				WORD wViewChairID=SwitchViewChairID(i);
				BYTE cbCardData[2]={0,pGameStart->cbCardData[i]};

				//�ɷ��˿�
				cbCardData[0]=(GetMeChairID()==i)?pGameStart->cbObscureCard:0;
				m_GameClientView.DispatchUserCard(wViewChairID,cbCardData[cbIndex]);
			}
		}
	}

	//�������
	if (IsLookonMode()==false)
	{
		ActiveGameFrame();
		m_GameClientView.m_CardControl[MYSELF_VIEW_ID].SetPositively(true);
	}

	//���½���
	m_GameClientView.InvalidGameView(0,0,0,0);

	//��������
	PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_START"));
	PlayGameSound(AfxGetInstanceHandle(),TEXT("BET"));
#ifdef CHEAT_VERSION
	//��ȡ��Ϣ
	//if( CUserRight::CanCheat(m_pIClientKernel->GetUserRight()) )
	//{
	//	//������Ϣ
	//	SendSocketData(SUB_C_GET_WINNER);
	//}
#endif

	return true;
}

//�û�����
bool CGameClientEngine::OnSubGiveUp(VOID * pData, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_GiveUp));
	if (wDataSize!=sizeof(CMD_S_GiveUp)) return false;

	//��������
	CMD_S_GiveUp * pGiveUp=(CMD_S_GiveUp *)pData;

	//��������
	WORD wGiveUpUser=pGiveUp->wGiveUpUser;
	WORD wViewChairID=SwitchViewChairID(wGiveUpUser);

	//��ɷ���
	m_GameClientView.ConcludeDispatch();

	//��������
	//if ((IsLookonMode()==true)||(wViewChairID!=MYSELF_VIEW_ID))
	{
		//���ö���
		m_GameClientView.SetUserAction(wViewChairID,AC_GIVE_UP);

		//��������
		BYTE cbCardData[MAX_COUNT];
		WORD wCardCount=m_GameClientView.m_CardControl[wViewChairID].GetCardCount();

		//�����˿�
		ZeroMemory(cbCardData,sizeof(cbCardData));
		m_GameClientView.m_CardControl[wViewChairID].SetDisplayHead(false);
		m_GameClientView.m_CardControl[wViewChairID].SetCardData(cbCardData,wCardCount);

		IClientUserItem *pIClientUserItem = GetTableUserItem(wGiveUpUser);

		//��������
		PlayGameSound(AfxGetInstanceHandle(),(pIClientUserItem->GetUserGrade()==0)?TEXT("GIVE_UP_1"):TEXT("GIVE_UP_0"));

		//���½���
		m_GameClientView.InvalidGameView(0,0,0,0);
	}

	//��ע����
	m_lDrawMaxScore=pGiveUp->lDrawMaxScore;
	m_lTurnMaxScore=pGiveUp->lTrunMaxScore;

	//״̬����
	m_cbPlayStatus[wGiveUpUser]=FALSE;

	//���ý���
	m_GameClientView.SetDrawMaxScore(m_lDrawMaxScore);

	//�����û�
	m_wCurrentUser=pGiveUp->wCurrentUser;

	//��ע����
	if ((IsLookonMode()==false)&&(m_wCurrentUser==GetMeChairID())) 
	{
		ActiveGameFrame();
		UpdateScoreControl();
	}

	//����ʱ��
	if (GetClockChairID()!=m_wCurrentUser) 
	{
		KillGameClock(IDI_ADD_SCORE);
		SetGameClock(m_wCurrentUser,IDI_ADD_SCORE,TIME_ADD_SCORE);
	}

	return true;
}

//�û���ע
bool CGameClientEngine::OnSubAddScore(VOID * pData, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_AddScore));
	if (wDataSize!=sizeof(CMD_S_AddScore)) return false;

	//��������
	CMD_S_AddScore * pAddScore=(CMD_S_AddScore *)pData;

	//��������
	WORD wAddScoreUser=pAddScore->wAddScoreUser;
	WORD wViewChairID=SwitchViewChairID(wAddScoreUser);

	//��ɷ���
	m_GameClientView.ConcludeDispatch();

	//��ע����
	//if ((IsLookonMode()==true)||(wViewChairID!=MYSELF_VIEW_ID))
	{
		//��������
		LONGLONG lCurrentScore=pAddScore->lUserScoreCount;
		LONGLONG lDrawAddScore=m_lTableScore[wAddScoreUser];
		IClientUserItem * pIClientUserItem=GetTableUserItem(wAddScoreUser);

		//��������
		m_lUserScore[wAddScoreUser]=lCurrentScore;
		m_GameClientView.AddPlayBetChip(m_lUserScore[wAddScoreUser], wViewChairID);
		// �������
		m_GameClientView.AddDeskChip(m_lUserScore[wAddScoreUser], wViewChairID);
		m_GameClientView.SetUserTableScore(wViewChairID,m_lUserScore[wAddScoreUser]+m_lTableScore[wAddScoreUser]);

		//���û���
		if (pIClientUserItem!=NULL)
		{
			LONGLONG lUserDrawScore=m_lUserScore[wAddScoreUser] + m_lTableScore[wAddScoreUser] + m_lServiceCharge;
			m_GameClientView.SetUserScore(wViewChairID,pIClientUserItem->GetUserScore()-lUserDrawScore);
		}

		//����ж�
		if ((lCurrentScore+lDrawAddScore)==m_lDrawMaxScore)
		{
			//���ö���
			m_bShowHand=true;
			m_GameClientView.SetUserAction(wViewChairID,AC_SHOW_HAND);

			//��������
			PlayGameSound(AfxGetInstanceHandle(),TEXT("SHOW_HAND"));
			PlayGameSound(AfxGetInstanceHandle(),TEXT("BET"));

		}
		else
		{

			IClientUserItem * pIClientUserItem=GetTableUserItem(wAddScoreUser);
			bool bGirl = (pIClientUserItem->GetUserGrade()==0);
			if (lCurrentScore==0L)
			{
				//���Ӷ���
				PlayGameSound(AfxGetInstanceHandle(),bGirl?TEXT("NO_ADD_1"):TEXT("NO_ADD_0"));
				m_GameClientView.SetUserAction(wViewChairID,AC_NO_ADD);
			}
			else if ((lCurrentScore+lDrawAddScore)==m_lTurnLessScore)
			{
				//��ע����
				PlayGameSound(AfxGetInstanceHandle(),bGirl?TEXT("FOLLOW_1"):TEXT("FOLLOW_0"));
				if(bGirl==false) PlayGameSound(AfxGetInstanceHandle(),TEXT("BET"));

				m_GameClientView.SetUserAction(wViewChairID,AC_FOLLOW);
			}
			else
			{
				//��ע����

				if(m_bXiaZhu)
				{
					if(pIClientUserItem->GetGender()==GENDER_FEMALE	) PlayGameSound(AfxGetInstanceHandle(),TEXT("XIAZHU_1"));
					else	PlayGameSound(AfxGetInstanceHandle(),TEXT("XIAZHU_0"));
				}
				else
				{
					if(pIClientUserItem->GetGender()==GENDER_FEMALE	) PlayGameSound(AfxGetInstanceHandle(),TEXT("ADD_SCORE_1"));
					else	PlayGameSound(AfxGetInstanceHandle(),TEXT("ADD_SCORE_0"));
				}
				// 				PlayGameSound(AfxGetInstanceHandle(),bGirl?TEXT("ADD_SCORE_1"):TEXT("ADD_SCORE_0"));
				PlayGameSound(AfxGetInstanceHandle(),TEXT("BET"));
				m_GameClientView.SetUserAction(wViewChairID,AC_ADD_SCORE);
			}
		}
		if(m_bXiaZhu) m_bXiaZhu=false;

		//���½���
		m_GameClientView.InvalidGameView(0,0,0,0);
	}

	//���ñ���
	m_wCurrentUser=pAddScore->wCurrentUser;
	m_lTurnLessScore=pAddScore->lTurnLessScore;

	//�������
	if ((IsLookonMode()==false)&&(m_wCurrentUser==GetMeChairID()))
	{
		ActiveGameFrame();
		UpdateScoreControl();
	}

	//����ʱ��
	SetGameClock(m_wCurrentUser,IDI_ADD_SCORE,TIME_ADD_SCORE);

	return true;
}

//�����˿�
bool CGameClientEngine::OnSubSendCard(VOID * pData, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_SendCard));
	if (wDataSize!=sizeof(CMD_S_SendCard)) return false;

	//��������
	CMD_S_SendCard * pSendCard=(CMD_S_SendCard *)pData;

	//���ñ���
	m_wCurrentUser=pSendCard->wCurrentUser;
	m_lTurnMaxScore=pSendCard->lTurnMaxScore;
	m_bXiaZhu = true;
	//�㼯���
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//��������
		WORD wViewChairID=SwitchViewChairID(i);

		//�������
		m_lTableScore[i]+=m_lUserScore[i];
		m_GameClientView.m_lALLBetChip += m_lUserScore[i];
		m_GameClientView.SetUserTableScore(wViewChairID,m_lTableScore[i]);

		// �������
		m_GameClientView.AddDeskChip(m_lUserScore[i], wViewChairID);

		//�û�����
		m_lUserScore[i]=0L;
		m_GameClientView.AddPlayBetChip(m_lUserScore[i], wViewChairID);
	}

	//�ɷ��˿�
	for (BYTE i=0;i<pSendCard->cbSendCardCount;i++)
	{
		for (WORD j=0;j<GAME_PLAYER;j++)
		{
			WORD wChairId = (pSendCard->wStartChairID+j)%GAME_PLAYER;
			if (m_cbPlayStatus[wChairId]==TRUE)
			{
				WORD wViewChairID=SwitchViewChairID(wChairId);
				m_GameClientView.DispatchUserCard(wViewChairID,pSendCard->cbCardData[i][wChairId]);
			}
		}
	}

	return true;
}

//��Ϸ����
bool CGameClientEngine::OnSubGameEnd(VOID * pData, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_GameEnd));
	if (wDataSize!=sizeof(CMD_S_GameEnd)) return false;

	//��������
	CMD_S_GameEnd * pGameEnd=(CMD_S_GameEnd *)pData;

	//��������
	KillGameClock(IDI_ADD_SCORE);
	SetGameStatus(GAME_SCENE_FREE);

	//��������
	m_GameClientView.SetUserTableScore(INVALID_CHAIR,0L);

	//��������
	CopyMemory(&m_GameEndPacket,pGameEnd,sizeof(CMD_S_GameEnd));

	//������Ϸ
	PerformGameConclude();

	return true;
}

//�Զ���ʼ
bool CGameClientEngine::PerformAutoStart()
{
	//��������
	WORD wTableUserCount=0,wReadyUserCount=0;
	IClientUserItem * pIMySelfUserItem=GetMeUserItem();

	//״̬�ж�
	if ((IsLookonMode()==true)||(m_pIClientKernel->GetClockID()==IDI_START_GAME)) return false;
	if ((pIMySelfUserItem==NULL)||(pIMySelfUserItem->GetUserStatus()==US_READY)) return false;

	//������ģʽ
	if ( CServerRule::IsAllowAvertCheatMode(m_pIClientKernel->GetServerAttribute()->dwServerRule) )
	{
		//��ʼ����
		WORD wMeChairID=GetMeChairID();
		SetGameClock(wMeChairID,IDI_START_GAME,TIME_START_GAME);
		return true;
	}

	//ͳ���û�
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//��ȡ�û�
		IClientUserItem * pIClientUserItem=GetTableUserItem(i);

		//��Ŀͳ��
		if (pIClientUserItem!=NULL) 
		{
			wTableUserCount++;
			if (pIClientUserItem->GetUserStatus()==US_READY) wReadyUserCount++;
		}
	}

	//��ʼ�ж�
	if ((wTableUserCount>=2)&&((wReadyUserCount+1)==wTableUserCount))
	{
		//��ʼ����
		WORD wMeChairID=GetMeChairID();
		SetGameClock(wMeChairID,IDI_START_GAME,TIME_START_GAME);

		return true;
	}

	return false;
}

//���ؿ���
VOID CGameClientEngine::HideScoreControl()
{
	//���ؿؼ�
	m_GameClientView.m_PlayOperate.ShowOperate(false);

	//���ÿؼ�
	m_GameClientView.m_PlayOperate.m_btFollow.EnableWindow(FALSE);
	m_GameClientView.m_PlayOperate.m_btAddScore.EnableWindow(FALSE);
	m_GameClientView.m_PlayOperate.m_btShowHand.EnableWindow(FALSE);

	m_GameClientView.InvalidGameView(0,0,0,0);

	return;
}

//���¿���
VOID CGameClientEngine::UpdateScoreControl()
{
	//��������
	WORD wMeChairID=GetMeChairID();
	LONGLONG lDrawUserScore=m_lTableScore[wMeChairID]+m_lUserScore[wMeChairID];

	//��ǰ�û�
	if ((m_wCurrentUser==wMeChairID)&&(m_cbPlayStatus[wMeChairID]==TRUE))
	{
		// ��С��ע
		LONGLONG lMinBet = (m_lTurnLessScore==lDrawUserScore)?m_lCellScore:0;

		//��ʾ��ť
		m_GameClientView.m_PlayOperate.ShowOperate(true);
		m_GameClientView.m_PlayOperate.SetBetCount( m_lTurnMaxScore - m_lTurnLessScore, lMinBet );

		m_GameClientView.m_PlayOperate.m_btFollow.ShowWindow(SW_SHOW);
		m_GameClientView.m_PlayOperate.m_btGiveUp.ShowWindow(SW_SHOW);
		m_GameClientView.m_PlayOperate.m_btAddScore.ShowWindow(SW_SHOW);
		m_GameClientView.m_PlayOperate.m_btShowHand.ShowWindow(SW_SHOW);

		//��־����
		bool bEnableShowHand=(m_lDrawMaxScore==m_lTurnMaxScore);
		UINT uBtImageID=(m_lTurnLessScore==lDrawUserScore)?IDB_BT_CONTROL_NOADD:IDB_BT_CONTROL_FOLLOW;

		//���ư�ť
		m_GameClientView.m_PlayOperate.m_btFollow.EnableWindow((m_lTurnMaxScore>=m_lTurnLessScore)?TRUE:FALSE);
		m_GameClientView.m_PlayOperate.m_btShowHand.EnableWindow((bEnableShowHand==true)?TRUE:FALSE);
		m_GameClientView.m_PlayOperate.m_btAddScore.EnableWindow((m_lTurnMaxScore>m_lTurnLessScore && m_lTurnMaxScore - m_lTurnLessScore != 0)?TRUE:FALSE);
		m_GameClientView.m_PlayOperate.m_btFollow.SetButtonImage(uBtImageID,AfxGetInstanceHandle(),false,false);
	}
	else
	{
		//���ؿؼ�
		m_GameClientView.m_PlayOperate.ShowOperate(false);

		//���ÿؼ�
		m_GameClientView.m_PlayOperate.m_btFollow.EnableWindow(FALSE);
		m_GameClientView.m_PlayOperate.m_btAddScore.EnableWindow(FALSE);
		m_GameClientView.m_PlayOperate.m_btShowHand.EnableWindow(FALSE);
	}

	m_GameClientView.InvalidGameView(0,0,0,0);

	return;
}

//ִ�н���
VOID CGameClientEngine::PerformGameConclude()
{
	//���ؿؼ�
	HideScoreControl();

	//�û��ɼ�
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//��λ�û�
		if (m_GameEndPacket.lGameScore[i]==0L) continue;

		//���óɼ�
		m_HistoryScore.OnEventUserScore(i,m_GameEndPacket.lGameScore[i]);
	}

	//�ɼ�����
	for ( int i = 0; i < GAME_PLAYER; ++i )
	{
		m_GameClientView.SetOverScore(SwitchViewChairID(i), m_GameEndPacket.lGameScore[i]);
	}

	// ���ն���
	LONGLONG lMaxScore = 0l;
	WORD     wWinSite = INVALID_CHAIR;
	for ( int i = 0; i < GAME_PLAYER; ++i )
	{
		if (  m_GameEndPacket.lGameScore[i] > lMaxScore )
		{
			lMaxScore = m_GameEndPacket.lGameScore[i];
			wWinSite = SwitchViewChairID(i);
		}
	}

	if( wWinSite != INVALID_CHAIR )
	{
		m_GameClientView.ChioRecovery(wWinSite);
	}
	

	//�û��˿�
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//������Ϣ
		if (m_GameEndPacket.lGameScore[i]!=0L)
		{
			//�����ж�
			if (m_GameEndPacket.cbCardData[i]==0x00) continue;

			//��ȡ�˿�
			BYTE cbCardData[MAX_CHAIR];
			WORD wViewChairID=SwitchViewChairID(i);
			WORD wCardCount=m_GameClientView.m_CardControl[wViewChairID].GetCardData(cbCardData,CountArray(cbCardData));

			//�����˿�
			cbCardData[0]=m_GameEndPacket.cbCardData[i];
			m_GameClientView.m_CardControl[wViewChairID].SetDisplayHead(true);
			m_GameClientView.m_CardControl[wViewChairID].SetCardData(cbCardData,wCardCount);
		}
	}

	//���ÿؼ�
	m_GameClientView.m_CardControl[MYSELF_VIEW_ID].SetPositively(false);

	//��������
	LONGLONG lMeScore=m_GameEndPacket.lGameScore[GetMeChairID()];
	if (lMeScore>0L) PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_WIN"));
	else if (lMeScore<0L) PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_LOSE"));
	else PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_END"));

	//��ʼ����
	if ((IsLookonMode()==false)&&(GetMeChairID()!=INVALID_CHAIR))
	{
		tagServerAttribute *pServerAttribute=m_pIClientKernel->GetServerAttribute();
		bool bMatchMode=((pServerAttribute->wServerType&GAME_GENRE_MATCH)!=0);
		if(!bMatchMode)
		{
			PerformAutoStart();
			m_GameClientView.m_btStart.ShowWindow(SW_SHOW);
		}	
	}

	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//��������
		WORD wViewChairID=SwitchViewChairID(i);
		IClientUserItem * pIClientUserItem=GetTableUserItem(i);

		if(pIClientUserItem != NULL )
		{
			LONGLONG lUserScore=pIClientUserItem->GetUserScore();
			m_GameClientView.SetUserScore(wViewChairID,lUserScore);
		}
		else
		{
			//���û���
			m_GameClientView.SetUserScore(wViewChairID,0L);

			//���ó���
			m_GameClientView.m_lPalyBetChip[wViewChairID] = 0l;
		}
	}

	//���½���
	m_GameClientView.InvalidGameView(0,0,0,0);

	return;
}

//��ʼ��Ϣ
LRESULT CGameClientEngine::OnMessageStart(WPARAM wParam, LPARAM lParam)
{
	//��ע��Ϣ
	m_lDrawMaxScore=0L;
	m_lTurnMaxScore=0L;
	m_lTurnLessScore=0L;
	ZeroMemory(m_lUserScore,sizeof(m_lUserScore));
	ZeroMemory(m_lTableScore,sizeof(m_lTableScore));

	//״̬����
	m_bShowHand=false;
	m_wCurrentUser=INVALID_CHAIR;
	ZeroMemory(m_szAccounts,sizeof(m_szAccounts));
	ZeroMemory(m_cbPlayStatus,sizeof(m_cbPlayStatus));

	//ɾ��ʱ��
	KillGameClock(IDI_START_GAME);

	//��ɷ���
	m_GameClientView.ConcludeDispatch();

	//���ý���
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		m_GameClientView.m_lPalyBetChip[i] = 0l;
		m_GameClientView.m_CardControl[i].SetCardData(NULL,0);
		m_GameClientView.SetOverScore(i, 0);
	}
	m_GameClientView.m_lALLBetChip = 0l;

	//ɾ������
	for ( int i = 0 ; i < m_GameClientView.m_ArrayPlayChip.GetCount(); ++i )
		SafeDelete(m_GameClientView.m_ArrayPlayChip[i]);
	m_GameClientView.m_ArrayPlayChip.RemoveAll();

	for ( int i = 0 ; i < m_GameClientView.m_ArrayBetChip.GetCount(); ++i )
		SafeDelete(m_GameClientView.m_ArrayBetChip[i]);
	m_GameClientView.m_ArrayBetChip.RemoveAll();

	for ( int i = 0 ; i < m_GameClientView.m_ArrayOverChip.GetCount(); ++i )
		SafeDelete(m_GameClientView.m_ArrayOverChip[i]);
	m_GameClientView.m_ArrayOverChip.RemoveAll();

	//��������
	m_GameClientView.SetDrawMaxScore(0L);
	m_GameClientView.SetUserAction(INVALID_CHAIR,0);
	m_GameClientView.SetUserTableScore(INVALID_CHAIR,0L);

	//���ÿؼ�
	m_GameClientView.m_btStart.ShowWindow(SW_HIDE);

	//���½���
	m_GameClientView.InvalidGameView(0,0,0,0);

	//������Ϣ
	m_pIClientKernel->SendUserReady(NULL,0);
	PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_READY"));
	return 0;
}

//��ע��Ϣ
LRESULT CGameClientEngine::OnMessageFollow(WPARAM wParam, LPARAM lParam)
{
	//���ؿؼ�
	HideScoreControl();

	//ɾ��ʱ��
	KillGameClock(IDI_ADD_SCORE);


	//���ÿؼ�
	WORD wMeChairID=GetMeChairID();
	LONGLONG lScoreLess = m_lTurnLessScore - ( m_lTableScore[wMeChairID] + m_lUserScore[wMeChairID] );
	LONGLONG lXXParam = wParam + (LONGLONG)lScoreLess;

	//��Ҽ���
	LONGLONG lDrawAddScore=m_lTableScore[wMeChairID]+m_lUserScore[wMeChairID];
	LONGLONG lCurrentAddScore=__max(m_lTurnLessScore-lDrawAddScore,lXXParam);

	////��������
	//m_lUserScore[wMeChairID]+=lCurrentAddScore;
	//m_GameClientView.AddPlayBetChip(m_lUserScore[wMeChairID], MYSELF_VIEW_ID);
	//m_GameClientView.SetUserTableScore(MYSELF_VIEW_ID,m_lUserScore[wMeChairID]+m_lTableScore[wMeChairID]);
	//// �������
	//m_GameClientView.AddDeskChip(m_lUserScore[wMeChairID], MYSELF_VIEW_ID);

	////���û���
	//IClientUserItem * pIClientUserItem=GetMeUserItem();
	//LONGLONG lUserItemScore=pIClientUserItem->GetUserScore();
	//m_GameClientView.SetUserScore(MYSELF_VIEW_ID,lUserItemScore-m_lUserScore[wMeChairID]-m_lTableScore[wMeChairID] - m_lServiceCharge);

	////����ж�
	//if ((m_bShowHand==false)&&((lCurrentAddScore+lDrawAddScore)==m_lDrawMaxScore))
	//{
	//	//���ö���
	//	m_bShowHand=true;
	//	m_GameClientView.SetUserAction(MYSELF_VIEW_ID,AC_SHOW_HAND);

	//	//��������
	//	PlayGameSound(AfxGetInstanceHandle(),TEXT("SHOW_HAND"));
	//}
	//else
	//{
	//	if (lCurrentAddScore==0L)
	//	{
	//		//���Ӷ���
	//		PlayGameSound(AfxGetInstanceHandle(),TEXT("NO_ADD"));
	//		m_GameClientView.SetUserAction(MYSELF_VIEW_ID,AC_NO_ADD);
	//	}
	//	else if ((lCurrentAddScore+lDrawAddScore)==m_lTurnLessScore)
	//	{
	//		//��ע����
	//		PlayGameSound(AfxGetInstanceHandle(),TEXT("FOLLOW"));
	//		m_GameClientView.SetUserAction(MYSELF_VIEW_ID,AC_FOLLOW);
	//	}
	//	else
	//	{
	//		//��ע����
	//		PlayGameSound(AfxGetInstanceHandle(),TEXT("ADD_SCORE"));
	//		m_GameClientView.SetUserAction(MYSELF_VIEW_ID,AC_ADD_SCORE);
	//	}
	//}

	//������Ϣ
	CMD_C_AddScore AddScore;
	AddScore.lScore=lCurrentAddScore;
	SendSocketData(SUB_C_ADD_SCORE,&AddScore,sizeof(AddScore));

	//���½���
	m_GameClientView.InvalidGameView(0,0,0,0);

	return 0;
}

//������Ϣ		 
LRESULT CGameClientEngine::OnMessageGiveUp(WPARAM wParam, LPARAM lParam)
{
	//���ؿؼ�
	HideScoreControl();

	//��������
	KillGameClock(IDI_ADD_SCORE);

	////���ö���
	//m_GameClientView.SetUserAction(MYSELF_VIEW_ID,AC_GIVE_UP);

	////��������
	//BYTE cbCardData[MAX_COUNT];
	//WORD wCardCount=m_GameClientView.m_CardControl[MYSELF_VIEW_ID].GetCardCount();

	////�����˿�
	//ZeroMemory(cbCardData,sizeof(cbCardData));
	//m_GameClientView.m_CardControl[MYSELF_VIEW_ID].SetCardData(cbCardData,wCardCount);

	////���ÿؼ�
	//m_GameClientView.m_CardControl[MYSELF_VIEW_ID].SetPositively(false);
	//m_GameClientView.m_CardControl[MYSELF_VIEW_ID].SetDisplayHead(false);

	////��������
	//PlayGameSound(AfxGetInstanceHandle(),TEXT("GIVE_UP"));

	//������Ϣ
	SendSocketData(SUB_C_GIVE_UP);

	//���½���
	m_GameClientView.InvalidGameView(0,0,0,0);

	return 0;
}

//��ע��Ϣ		 
LRESULT CGameClientEngine::OnMessageAddScore(WPARAM wParam, LPARAM lParam)
{
	//���ð�ť
	m_GameClientView.m_PlayOperate.m_btFollow.EnableWindow(FALSE);
	m_GameClientView.m_PlayOperate.m_btAddScore.EnableWindow(FALSE);
	m_GameClientView.m_PlayOperate.m_btShowHand.EnableWindow(FALSE);

	return 0;
}

//�����Ϣ
LRESULT CGameClientEngine::OnMessageShowHand(WPARAM wParam, LPARAM lParam)
{
	//���ؿؼ�
	HideScoreControl();

	//��������
	KillGameClock(IDI_ADD_SCORE);

	//���ö���
	m_bShowHand=true;
	m_GameClientView.SetUserAction(MYSELF_VIEW_ID,AC_SHOW_HAND);

	//��Ҽ���
	WORD wMeChairID=GetMeChairID();
	LONGLONG lCurrentAddScore=m_lDrawMaxScore-m_lTableScore[wMeChairID]-m_lUserScore[wMeChairID];

// 	//��������
// 	m_lUserScore[wMeChairID]+=lCurrentAddScore;
// 	m_GameClientView.AddPlayBetChip(m_lUserScore[wMeChairID], MYSELF_VIEW_ID);
// 	m_GameClientView.SetUserTableScore(MYSELF_VIEW_ID,m_lUserScore[wMeChairID]+m_lTableScore[wMeChairID]);
// 
// 	//���û���
// 	IClientUserItem * pIClientUserItem=GetMeUserItem();
// 	LONGLONG lUserItemScore=pIClientUserItem->GetUserScore();
// 	m_GameClientView.SetUserScore(MYSELF_VIEW_ID,lUserItemScore-m_lUserScore[wMeChairID]-m_lTableScore[wMeChairID]-m_lServiceCharge);
// 
// 	//��������
// 	PlayGameSound(AfxGetInstanceHandle(),TEXT("SHOW_HAND"));

	//������Ϣ
	CMD_C_AddScore AddScore;
	AddScore.lScore=lCurrentAddScore;
	SendSocketData(SUB_C_ADD_SCORE,&AddScore,sizeof(AddScore));

	//���½���
	m_GameClientView.InvalidGameView(0,0,0,0);

	return 0;
}

//�ر���Ϣ
LRESULT CGameClientEngine::OnMessageClose(WPARAM wParam, LPARAM lParam)
{
	if(IsLookonMode() == true)
	{
		AfxGetMainWnd()->PostMessage(WM_CLOSE);
		return 0;
	}

	IClientUserItem * pIClientUserItem=GetTableUserItem(GetMeChairID());
	if(pIClientUserItem!=NULL)
	{
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
	}

	return 0;
}

//�������
LRESULT CGameClientEngine::OnMessageSendCardFinish(WPARAM wParam, LPARAM lParam)
{
	//״̬�ж�
	if (m_wCurrentUser==INVALID_CHAIR) return 0;

	//���ý���
	m_GameClientView.SetUserAction(INVALID_CHAIR,0);

	//���ÿ���
	if ((IsLookonMode()==false)&&(m_wCurrentUser==GetMeChairID()))
	{
		ActiveGameFrame();
		UpdateScoreControl();
	}

	//����ʱ��
	SetGameClock(m_wCurrentUser,IDI_ADD_SCORE,TIME_ADD_SCORE);

	//������Ϣ
	SendSocketData(SUB_C_ADD_SCORE_TIME);

	return 0;
}

//��ȡʤ��
bool CGameClientEngine::OnSubGetWinner( VOID * pData, WORD wDataSize )
{
	return true;


	//��������
	CMD_S_GetWinner *pGetWinner = (CMD_S_GetWinner *)pData;

	//Ч������
	ASSERT( wDataSize >= sizeof(pGetWinner->wOrderCount) );
	ASSERT( wDataSize == sizeof(CMD_S_GetWinner)-sizeof(pGetWinner->wChairOrder)+
		sizeof(WORD)*pGetWinner->wOrderCount );
	if (wDataSize != sizeof(CMD_S_GetWinner)-sizeof(pGetWinner->wChairOrder)+
		sizeof(WORD)*pGetWinner->wOrderCount) 
		return false;

	CString strMessage = TEXT("������Ҵ�С�����ǣ�");

	//�����˿�
	for( WORD i = 0; i < pGetWinner->wOrderCount; i++ )
	{
		if( m_cbPlayStatus[pGetWinner->wChairOrder[i]] && m_szAccounts[pGetWinner->wChairOrder[i]][0] != 0 )
		{
			if( i > 0 ) strMessage += TEXT("��");

			strMessage += m_szAccounts[pGetWinner->wChairOrder[i]];
		}
	}

	if ( m_pIStringMessage )
		m_pIStringMessage->InsertSystemString(strMessage);

	return true;
}


//������Ϣ
LRESULT CGameClientEngine::OnChatMessage( WPARAM wParam, LPARAM lParam )
{
	//m_GameClientView.InsertAllChatMessage((LPCTSTR)wParam,int(lParam));

	return 0;
}

//////////////////////////////////////////////////////////////////////////
