#include "StdAfx.h"
#include "Resource.h"
#include "GameClient.h"
#include "GameClientEngine.h"


//////////////////////////////////////////////////////////////////////////
//�궨��

//��Ϸ��ʱ��
#define IDI_OUT_CARD					200								//���ƶ�ʱ��
#define IDI_MOST_CARD					201								//���ʱ��
#define IDI_START_GAME					202								//��ʼ��ʱ��


#define TIME_OUT_CARD_FAST				10								//��ʼ��ʱ��
#define TIME_OUT_CARD					30								//��ʼ��ʱ��



//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameClientEngine, CGameFrameEngine)
	ON_WM_TIMER()
	ON_MESSAGE(IDM_START,OnStart)
	ON_MESSAGE(IDM_OUT_CARD,OnOutCard)
	ON_MESSAGE(IDM_PASS_CARD,OnPassCard)
	ON_MESSAGE(IDM_OUT_PROMPT,OnOutPrompt)
	ON_MESSAGE(IDM_LEFT_HIT_CARD,OnLeftHitCard)
	ON_MESSAGE(IDM_RIGHT_HIT_CARD,OnRightHitCard)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CGameClientEngine::CGameClientEngine()
{
	//��Ϸ����
	m_bHandCardCount=0;
	m_wBankerUser=INVALID_CHAIR;
	ZeroMemory(m_bBombCount,sizeof(m_bBombCount));
	ZeroMemory(m_bCardCount,sizeof(m_bCardCount));
	ZeroMemory(m_bHandCardData,sizeof(m_bHandCardData));

	//���ñ���
	m_bDeasilOrder=false;
	m_dwCardHSpace=17;//DEFAULT_PELS;

	//���Ʊ���
	m_bTurnCardCount=0;
	m_bTurnOutType=CT_ERROR;
	ZeroMemory(m_bTurnCardData,sizeof(m_bTurnCardData));

	//��������
	m_wTimeOutCount=0;
	m_wMostUser=INVALID_CHAIR;
}

//��������
CGameClientEngine::~CGameClientEngine()
{
}

//��������
bool CGameClientEngine::OnInitGameEngine()
{
	//��������
	HICON hIcon=LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME));
	m_pIClientKernel->SetGameAttribute(KIND_ID,GAME_PLAYER,VERSION_CLIENT,hIcon,GAME_NAME);

	//���ÿؼ�
	m_GameClientView.SetUserOrder(m_bDeasilOrder);
	m_GameClientView.m_HandCardControl.SetCardSpace(m_dwCardHSpace,0,20);

	return true;
}

//���ú���
bool CGameClientEngine::OnResetGameEngine()
{
	//��Ϸ����
	m_bHandCardCount=0;
	m_wBankerUser=INVALID_CHAIR;
	ZeroMemory(m_bBombCount,sizeof(m_bBombCount));
	ZeroMemory(m_bCardCount,sizeof(m_bCardCount));
	ZeroMemory(m_bHandCardData,sizeof(m_bHandCardData));

	//���Ʊ���
	m_bTurnCardCount=0;
	m_bTurnOutType=CT_ERROR;
	ZeroMemory(m_bTurnCardData,sizeof(m_bTurnCardData));

	//��������
	m_wTimeOutCount=0;
	m_wMostUser=INVALID_CHAIR;

	//ɾ����ʱ
	KillGameClock(0);
	KillTimer(IDI_MOST_CARD);

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
	case IDI_OUT_CARD:			//�û�����
		{
			//��ʱ�ж�
			if (nElapse==0)
			{
				if ((IsLookonMode()==false)&&(wChairID==GetMeChairID())) AutomatismOutCard();
				return false;
			}

			//��������
			if (m_bHandCardCount<m_bTurnCardCount) return true;
			if ((nElapse<=3)&&(wChairID==GetMeChairID())&&(IsLookonMode()==false)) PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_WARN"));

			return true;
		}
	case IDI_START_GAME:		//��ʼ��Ϸ
		{
			if (nElapse==0)
			{
				if ((IsLookonMode()==false)&&(wChairID==GetMeChairID())) OnStart(0,0);
				return false;
			}
			if ((nElapse<=3)&&(wChairID==GetMeChairID())&&(IsLookonMode()==false)) PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_WARN"));

			return true;
		}
	}
	return false;
}

//�Թ���Ϣ
bool CGameClientEngine::OnEventLookonMode(VOID * pData, WORD wDataSize)
{
	return true;
}

//��Ϸ��Ϣ
bool CGameClientEngine::OnEventGameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_S_GAME_START:		//��Ϸ��ʼ
		{
			return OnSubGameStart(pData,wDataSize);
		}
	case SUB_S_OUT_CARD:		//�û�����
		{
			return OnSubOutCard(pData,wDataSize);
		}
	case SUB_S_PASS_CARD:		//��������
		{
			return OnSubPassCard(pData,wDataSize);
		}
	case SUB_S_GAME_END:		//��Ϸ����
		{
			return OnSubGameEnd(pData,wDataSize);
		}
	}
	return true;
}

//������Ϣ
bool CGameClientEngine::OnEventSceneMessage(BYTE cbGameStatus, bool bLookonUser, VOID * pData, WORD wDataSize)
{
	switch (cbGameStatus)
	{
	case GAME_SCENE_FREE:	//����״̬
		{
			//Ч������
			if (wDataSize!=sizeof(CMD_S_StatusFree)) return false;
			CMD_S_StatusFree * pStatusFree=(CMD_S_StatusFree *)pData;

			//���ý���
			m_GameClientView.SetCellScore(pStatusFree->lBaseScore);

			//���ÿؼ�
			if (IsLookonMode()==false && GetMeUserItem()->GetUserStatus()!=US_READY)
			{
				m_GameClientView.m_btStart.ShowWindow(TRUE);
				m_GameClientView.m_btStart.SetFocus();
#ifdef DEBUG
				SetGameClock(GetMeChairID(),IDI_START_GAME,10);
#else
				SetGameClock(GetMeChairID(),IDI_START_GAME,90);
#endif
			}
			//�����˿�
			if (IsLookonMode()==false) m_GameClientView.m_HandCardControl.SetDisplayFlag(true);

			return true;
		}
	case GAME_SCENE_PLAY:		//��Ϸ״̬
		{
			//Ч������
			if (wDataSize!=sizeof(CMD_S_StatusPlay)) return false;
			CMD_S_StatusPlay * pStatusPlay=(CMD_S_StatusPlay *)pData;

			//���ñ���
			m_wBankerUser=pStatusPlay->wBankerUser;
			m_bTurnCardCount=pStatusPlay->bTurnCardCount;
			m_bHandCardCount=pStatusPlay->bCardCount[GetMeChairID()];
			m_bTurnOutType=m_GameLogic.GetCardType(pStatusPlay->bTurnCardData,m_bTurnCardCount);
			CopyMemory(m_bHandCardData,pStatusPlay->bCardData,m_bHandCardCount);
			CopyMemory(m_bTurnCardData,pStatusPlay->bTurnCardData,pStatusPlay->bTurnCardCount);

			//���ý���
			for (BYTE i=0;i<GAME_PLAYER;i++)
			{
				WORD wViewChairID=SwitchViewChairID(i);
				m_bCardCount[i]=pStatusPlay->bCardCount[i];
				m_GameClientView.SetBombCount(wViewChairID,pStatusPlay->bBombCount[i]);
				m_GameClientView.SetCardCount(wViewChairID,pStatusPlay->bCardCount[i]);
				m_GameClientView.m_lAllTurnScore[wViewChairID]=pStatusPlay->lAllTurnScore[i];
				m_GameClientView.m_lLastTurnScore[wViewChairID]=pStatusPlay->lLastTurnScore[i];
			}
			m_GameClientView.SetCellScore(pStatusPlay->lBaseScore);
			m_GameClientView.m_HandCardControl.SetCardData(m_bHandCardData,m_bHandCardCount);

			//�������
			if ((IsLookonMode()==false)&&(pStatusPlay->wCurrentUser==GetMeChairID()))
			{
				m_GameClientView.m_btOutCard.EnableWindow(FALSE);
				m_GameClientView.m_btOutCard.ShowWindow(SW_SHOW);
				m_GameClientView.m_btOutPrompt.ShowWindow(SW_SHOW);
				m_GameClientView.m_btPassCard.ShowWindow(SW_SHOW);
				m_GameClientView.m_btPassCard.EnableWindow((VerdictPassCard()==true)?TRUE:FALSE);
			}

			//��������
			if (m_bTurnCardCount!=0)
			{
				WORD wViewChairID=SwitchViewChairID(pStatusPlay->wLastOutUser);
				m_GameClientView.m_UserCardControl[wViewChairID].SetCardData(m_bTurnCardData,m_bTurnCardCount);
			}

			//���ö�ʱ��
			WORD wTimeCount=TIME_OUT_CARD;
			if ((IsLookonMode()==false)&&(m_GameClientView.m_btPassCard.IsWindowEnabled())) wTimeCount=TIME_OUT_CARD_FAST;
			SetGameClock(pStatusPlay->wCurrentUser,IDI_OUT_CARD,wTimeCount);

			//�����˿�
			if (IsLookonMode()==false) 
			{
				m_GameClientView.m_HandCardControl.SetPositively(true);
				m_GameClientView.m_HandCardControl.SetDisplayFlag(true);
			}

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

	//���ñ���
	m_bTurnCardCount=0;
	m_bTurnOutType=CT_ERROR;
	m_wBankerUser=pGameStart->wBankerUser;
	ZeroMemory(m_bBombCount,sizeof(m_bBombCount));
	ZeroMemory(m_bTurnCardData,sizeof(m_bTurnCardData));

	//���ý���
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		m_GameClientView.SetPassFlag(i,false);
		m_GameClientView.m_UserCardControl[i].SetCardData(NULL,0);
	}
	if (IsLookonMode()==true)
	{
		m_GameClientView.SetBombCount(INVALID_CHAIR,0);
		m_GameClientView.m_ScoreView.ShowWindow(SW_HIDE);
		m_GameClientView.m_LeaveCardControl[0].SetCardData(NULL,0);
		m_GameClientView.m_LeaveCardControl[1].SetCardData(NULL,0);
	}

	//�����˿�
	m_bHandCardCount=CountArray(pGameStart->cbCardData);
	CopyMemory(m_bHandCardData,pGameStart->cbCardData,sizeof(pGameStart->cbCardData));
	m_GameClientView.m_HandCardControl.SetCardData(pGameStart->cbCardData,m_bHandCardCount);

	//�˿���Ŀ
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		m_bCardCount[i]=CountArray(pGameStart->cbCardData);
		m_GameClientView.SetCardCount(SwitchViewChairID(i),m_bCardCount[i]);
	}

	//��������
	bool bPlayerMode=(IsLookonMode()==false);
	m_GameClientView.m_HandCardControl.SetPositively(bPlayerMode);
	m_GameClientView.m_HandCardControl.SetDisplayFlag(bPlayerMode);

	//��ǰ���
	if ((IsLookonMode()==false)&&(pGameStart->wCurrentUser==GetMeChairID()))
	{
		ActiveGameFrame();
		m_GameClientView.m_btOutCard.ShowWindow(SW_SHOW);
		m_GameClientView.m_btOutCard.EnableWindow(FALSE);
		m_GameClientView.m_btPassCard.EnableWindow(FALSE);
		m_GameClientView.m_btPassCard.ShowWindow(SW_SHOW);
		m_GameClientView.m_btOutPrompt.ShowWindow(SW_SHOW);
	}

	//���ö�ʱ��
	WORD wTimeCount=TIME_OUT_CARD;
	if ((IsLookonMode()==false)&&(m_GameClientView.m_btPassCard.IsWindowEnabled())) wTimeCount=TIME_OUT_CARD_FAST;
	if (pGameStart->wCurrentUser!=INVALID_CHAIR) SetGameClock(pGameStart->wCurrentUser,IDI_OUT_CARD,wTimeCount);

	//��������
	PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_START"));

	return true;
}

//�û�����
bool CGameClientEngine::OnSubOutCard(const void * pBuffer, WORD wDataSize)
{
	//��������
	CMD_S_OutCard * pOutCard=(CMD_S_OutCard *)pBuffer;
	WORD wHeadSize=sizeof(CMD_S_OutCard)-sizeof(pOutCard->bCardData);

	//Ч������
	ASSERT(wDataSize>=wHeadSize);
	if (wDataSize<wHeadSize) return false;
	ASSERT(wDataSize==(wHeadSize+pOutCard->bCardCount*sizeof(pOutCard->bCardData[0])));
	if (wDataSize!=(wHeadSize+pOutCard->bCardCount*sizeof(pOutCard->bCardData[0]))) return false;

	//ɾ����ʱ��
	KillTimer(IDI_MOST_CARD);
	KillGameClock(IDI_OUT_CARD);

	//��������
	WORD wOutViewChairID=SwitchViewChairID(pOutCard->wOutCardUser);
	m_bCardCount[pOutCard->wOutCardUser]-=pOutCard->bCardCount;
	m_GameClientView.SetCardCount(wOutViewChairID,m_bCardCount[pOutCard->wOutCardUser]);

	//��������
	if ((IsLookonMode()==true)||(GetMeChairID()!=pOutCard->wOutCardUser))
	{
		m_GameClientView.m_UserCardControl[wOutViewChairID].SetCardData(pOutCard->bCardData,pOutCard->bCardCount);
	}

	//��������
	if (m_bTurnCardCount==0)
	{
		for (WORD i=0;i<GAME_PLAYER;i++) 
		{
			if (i==pOutCard->wOutCardUser) continue;
			m_GameClientView.m_UserCardControl[SwitchViewChairID(i)].SetCardData(NULL,0);
		}
		m_GameClientView.SetPassFlag(INVALID_CHAIR,false);
	}

	//��¼����
	m_bTurnCardCount=pOutCard->bCardCount;
	m_bTurnOutType=m_GameLogic.GetCardType(pOutCard->bCardData,pOutCard->bCardCount);
	CopyMemory(m_bTurnCardData,pOutCard->bCardData,sizeof(BYTE)*pOutCard->bCardCount);

	//�Լ��˿�
	if ((IsLookonMode()==true)&&(pOutCard->wOutCardUser==GetMeChairID()))
	{
		//ɾ���˿� 
		BYTE bSourceCount=m_bHandCardCount;
		m_bHandCardCount-=pOutCard->bCardCount;
		m_GameLogic.RemoveCard(pOutCard->bCardData,pOutCard->bCardCount,m_bHandCardData,bSourceCount);

		//���ý���
		m_GameClientView.m_HandCardControl.SetCardData(m_bHandCardData,m_bHandCardCount);
	}

	//����ж�
	if (pOutCard->wCurrentUser==pOutCard->wOutCardUser)
	{
		//���ñ���
		m_bTurnCardCount=0;
		m_bTurnOutType=CT_ERROR;
		m_wMostUser=pOutCard->wCurrentUser;
		ZeroMemory(m_bTurnCardData,sizeof(m_bTurnCardData));

		//���ý���
		for (WORD i=0;i<GAME_PLAYER;i++)
		{
			if (i!=pOutCard->wOutCardUser)
			{
				WORD wViewChairID=SwitchViewChairID(i);
				m_GameClientView.SetPassFlag(wViewChairID,true);
				m_GameClientView.m_UserCardControl[wViewChairID].SetCardData(NULL,0);
			}
		}

		//��������
		SetTimer(IDI_MOST_CARD,3000,NULL);
		PlayGameSound(AfxGetInstanceHandle(),TEXT("MOST_CARD"));

		return true;
	}

	//��������
	if ((IsLookonMode()==true)||(GetMeChairID()!=pOutCard->wOutCardUser)) PlayGameSound(AfxGetInstanceHandle(),TEXT("OUT_CARD"));

	//�������
	if (pOutCard->wCurrentUser!=INVALID_CHAIR)
	{
		WORD wViewChairID=SwitchViewChairID(pOutCard->wCurrentUser);
		m_GameClientView.SetPassFlag(wViewChairID,false);
		m_GameClientView.m_UserCardControl[wViewChairID].SetCardData(NULL,0);
	}

	//�������
	if ((IsLookonMode()==false)&&(pOutCard->wCurrentUser==GetMeChairID()))
	{
		ActiveGameFrame();
		m_GameClientView.m_btOutCard.ShowWindow(SW_SHOW);
		m_GameClientView.m_btPassCard.ShowWindow(SW_SHOW);
		m_GameClientView.m_btOutPrompt.ShowWindow(SW_SHOW);
		m_GameClientView.m_btOutCard.EnableWindow((VerdictOutCard()==true)?TRUE:FALSE);
		m_GameClientView.m_btPassCard.EnableWindow((VerdictPassCard()==true)?TRUE:FALSE);
	}

	//���ö�ʱ��
	if (pOutCard->wCurrentUser!=INVALID_CHAIR)
	{
		WORD wTimeCount=TIME_OUT_CARD;
		if ((IsLookonMode()==false)&&(m_GameClientView.m_btPassCard.IsWindowEnabled())) wTimeCount=TIME_OUT_CARD_FAST;
		SetGameClock(pOutCard->wCurrentUser,IDI_OUT_CARD,wTimeCount);
	}

	return true;
}

//��������
bool CGameClientEngine::OnSubPassCard(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	if (wDataSize!=sizeof(CMD_S_PassCard)) return false;
	CMD_S_PassCard * pPassCard=(CMD_S_PassCard *)pBuffer;

	//ɾ����ʱ��
	KillGameClock(IDI_OUT_CARD);

	//�������
	if ((IsLookonMode()==true)||(pPassCard->wPassUser!=GetMeChairID()))
	{
		WORD wViewChairID=SwitchViewChairID(pPassCard->wPassUser);
		m_GameClientView.SetPassFlag(wViewChairID,true);
		m_GameClientView.m_UserCardControl[wViewChairID].SetCardData(NULL,0);
	}

	//һ���ж�
	if (pPassCard->bNewTurn==TRUE)
	{
		//ը���ж�
		if (m_bTurnOutType==CT_BOMB)
		{
			//���ñ���
			m_bBombCount[pPassCard->wCurrentUser]++;

			//������ͼ
			WORD wViewChairID=SwitchViewChairID(pPassCard->wCurrentUser);
			m_GameClientView.SetBombCount(wViewChairID,m_bBombCount[pPassCard->wCurrentUser]);
		}

		//���ñ���
		m_bTurnCardCount=0;
		m_bTurnOutType=CT_ERROR;
		ZeroMemory(m_bTurnCardData,sizeof(m_bTurnCardData));
	}

	//���ý���
	WORD wViewChairID=SwitchViewChairID(pPassCard->wCurrentUser);
	m_GameClientView.SetPassFlag(wViewChairID,false);
	m_GameClientView.m_UserCardControl[wViewChairID].SetCardData(NULL,0);

	//�������
	if ((IsLookonMode()==false)&&(pPassCard->wCurrentUser==GetMeChairID()))
	{
		ActiveGameFrame();
		m_GameClientView.m_btOutCard.ShowWindow(SW_SHOW);
		m_GameClientView.m_btPassCard.ShowWindow(SW_SHOW);
		m_GameClientView.m_btOutPrompt.ShowWindow(SW_SHOW);
		m_GameClientView.m_btOutCard.EnableWindow((VerdictOutCard()==true)?TRUE:FALSE);
		m_GameClientView.m_btPassCard.EnableWindow((VerdictPassCard()==true)?TRUE:FALSE);
	}

	//��������
	if ((IsLookonMode()==true)||(pPassCard->wPassUser!=GetMeChairID()))	PlayGameSound(AfxGetInstanceHandle(),TEXT("OUT_CARD"));

	//����ʱ��
	WORD wTimeCount=TIME_OUT_CARD;
	if ((IsLookonMode()==false)&&(m_GameClientView.m_btPassCard.IsWindowEnabled())) wTimeCount=TIME_OUT_CARD_FAST;
	SetGameClock(pPassCard->wCurrentUser,IDI_OUT_CARD,wTimeCount);

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

	//����״̬
	SetGameStatus(GAME_SCENE_FREE);

	//ɾ����ʱ��
	KillTimer(IDI_MOST_CARD);
	KillGameClock(IDI_OUT_CARD);

	//���ؿؼ�
	m_GameClientView.m_btOutCard.ShowWindow(SW_HIDE);
	m_GameClientView.m_btPassCard.ShowWindow(SW_HIDE);
	m_GameClientView.m_btOutPrompt.ShowWindow(SW_HIDE);

	//���ÿؼ�
	m_GameClientView.m_btOutCard.EnableWindow(FALSE);
	m_GameClientView.m_btPassCard.EnableWindow(FALSE);

	//���û���
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		IClientUserItem *pUserItem=GetTableUserItem(i);
		const tagUserInfo * pUserData=(pUserItem==NULL?NULL:pUserItem->GetUserInfo());
		if(pUserItem==NULL)	continue;
		m_GameClientView.m_ScoreView.SetGameScore(i,pUserData->szNickName,pGameEnd->lGameScore[i]);
		m_GameClientView.m_lAllTurnScore[SwitchViewChairID(i)]+=pGameEnd->lGameScore[i];
		m_GameClientView.m_lLastTurnScore[SwitchViewChairID(i)]=pGameEnd->lGameScore[i];
	}
	m_GameClientView.m_ScoreView.SetTax(pGameEnd->lGameTax);
	m_GameClientView.m_ScoreView.ShowWindow(SW_SHOW);

	//�����˿�
	BYTE bCardPos=0;
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		WORD wViewChairID=SwitchViewChairID(i);
		if (wViewChairID==0) m_GameClientView.m_LeaveCardControl[0].SetCardData(&pGameEnd->bCardData[bCardPos],pGameEnd->bCardCount[i]);
		else if (wViewChairID==2) m_GameClientView.m_LeaveCardControl[1].SetCardData(&pGameEnd->bCardData[bCardPos],pGameEnd->bCardCount[i]);
		bCardPos+=pGameEnd->bCardCount[i];
		if (pGameEnd->bCardCount[i]!=0)
		{
			m_GameClientView.SetPassFlag(wViewChairID,false);
			m_GameClientView.m_UserCardControl[wViewChairID].SetCardData(NULL,0);
		}
	}

	//��������
	if ((pGameEnd->bCardCount[m_wBankerUser]==12)&&(pGameEnd->lGameScore[m_wBankerUser]>0))
	{
		BYTE cbCardData[]={0x33,0x23,0x13,0x03};
		WORD wViewChairID=SwitchViewChairID(m_wBankerUser);
		m_GameClientView.m_UserCardControl[wViewChairID].SetCardData(cbCardData,CountArray(cbCardData));
	}

	//��ʾ�˿�
	if (IsLookonMode()==true) m_GameClientView.m_HandCardControl.SetDisplayFlag(true);

	//��������
	WORD wMeChairID=GetMeChairID();
	LONGLONG lMeScore=pGameEnd->lGameScore[GetMeChairID()];
	if (lMeScore>0L) PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_WIN"));
	else if (lMeScore<0L) PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_LOST"));
	else PlayGameSound(GetModuleHandle(NULL),TEXT("GAME_END"));

	//���ý���
	if (IsLookonMode()==false)
	{
		m_GameClientView.m_btStart.ShowWindow(SW_SHOW);
		SetGameClock(GetMeChairID(),IDI_START_GAME,90);
	}

	return true;
}

//�����ж�
bool CGameClientEngine::VerdictOutCard()
{
	//״̬�ж�
	if (m_GameClientView.m_btOutCard.IsWindowVisible()==FALSE) return false;

	//��ȡ�˿�
	BYTE bCardData[16];
	BYTE bShootCount=(BYTE)m_GameClientView.m_HandCardControl.GetShootCard(bCardData,CountArray(bCardData));

	//�����ж�
	if (bShootCount>0L)
	{
		//�����˿�
		m_GameLogic.SortCardList(bCardData,bShootCount);

		//��������
		BYTE bCardType=m_GameLogic.GetCardType(bCardData,bShootCount);

		//�����ж�
		if (bCardType==CT_ERROR) return false;

		//�׳��ж�
		if ((m_wBankerUser==GetMeChairID())&&(m_bCardCount[m_wBankerUser]==16))
		{
			//�����ж�
			BYTE i=0;
			for (i=0;i<bShootCount;i++)
			{
				if (bCardData[i]==0x33) break;
			}
			if (i==bShootCount) return false;
		}

		//�����ж�
		if (m_bTurnCardCount==0) return true;
		return m_GameLogic.CompareCard(bCardData,m_bTurnCardData,bShootCount,m_bTurnCardCount);
	}

	return false;
}

//�����ж�
bool CGameClientEngine::VerdictPassCard()
{
	//�׳��ж�
	if (m_bTurnCardCount==0) return false;

	//�������
	tagOutCardResult OutCardResult;
	return (m_GameLogic.SearchOutCard(m_bHandCardData,m_bHandCardCount,m_bTurnCardData,m_bTurnCardCount,m_bTurnOutType,OutCardResult)==false);
}

//�Զ�����
bool CGameClientEngine::AutomatismOutCard()
{
	//���Ʒ���
	tagOutCardResult OutCardResult;
	m_GameLogic.SearchOutCard(m_bHandCardData,m_bHandCardCount,m_bTurnCardData,m_bTurnCardCount,m_bTurnOutType,OutCardResult);

	//���ƴ���
	if (OutCardResult.cbCardCount>0)
	{
		//���ƽ���
		KillGameClock(IDI_OUT_CARD);
		m_GameClientView.m_btOutCard.ShowWindow(SW_HIDE);
		m_GameClientView.m_btOutCard.EnableWindow(FALSE);
		m_GameClientView.m_btPassCard.ShowWindow(SW_HIDE);
		m_GameClientView.m_btPassCard.EnableWindow(FALSE);
		m_GameClientView.m_btOutPrompt.ShowWindow(SW_HIDE);

		//��������
		CMD_C_OutCard OutCard;
		OutCard.bCardCount=OutCardResult.cbCardCount;
		CopyMemory(OutCard.bCardData,OutCardResult.cbResultCard,sizeof(BYTE)*OutCardResult.cbCardCount);
		SendSocketData(SUB_C_OUT_CART,&OutCard,sizeof(OutCard)-sizeof(OutCard.bCardData)+OutCard.bCardCount*sizeof(BYTE));

		//Ԥ�ȴ���
		PlayGameSound(AfxGetInstanceHandle(),TEXT("OUT_CARD"));
		m_GameClientView.m_UserCardControl[1].SetCardData(OutCard.bCardData,OutCard.bCardCount);

		//Ԥ��ɾ��
		BYTE bSourceCount=m_bHandCardCount;
		m_bHandCardCount-=OutCard.bCardCount;
		m_GameLogic.RemoveCard(OutCard.bCardData,OutCard.bCardCount,m_bHandCardData,bSourceCount);
		m_GameClientView.m_HandCardControl.SetCardData(m_bHandCardData,m_bHandCardCount);
	}
	else OnPassCard(0,0);

	return true;
}

//��ʱ����Ϣ
void CGameClientEngine::OnTimer(UINT nIDEvent)
{
	if ((nIDEvent==IDI_MOST_CARD)&&(m_wMostUser!=INVALID_CHAIR))
	{
		//��������
		WORD wCurrentUser=m_wMostUser;
		m_wMostUser=INVALID_CHAIR;

		//ɾ����ʱ��
		KillTimer(IDI_MOST_CARD);

		//���ý���
		m_GameClientView.SetPassFlag(INVALID_CHAIR,false);
		for (WORD i=0;i<GAME_PLAYER;i++) m_GameClientView.m_UserCardControl[i].SetCardData(NULL,0);

		//�������
		if ((IsLookonMode()==false)&&(wCurrentUser==GetMeChairID()))
		{
			ActiveGameFrame();
			m_GameClientView.m_btOutCard.ShowWindow(SW_SHOW);
			m_GameClientView.m_btPassCard.ShowWindow(SW_SHOW);
			m_GameClientView.m_btOutPrompt.ShowWindow(SW_SHOW);
			m_GameClientView.m_btOutCard.EnableWindow((VerdictOutCard()==true)?TRUE:FALSE);
			m_GameClientView.m_btPassCard.EnableWindow((VerdictPassCard()==true)?TRUE:FALSE);
		}

		//����ʱ��
		WORD wTimeCount=TIME_OUT_CARD;
		if ((IsLookonMode()==false)&&(m_GameClientView.m_btPassCard.IsWindowEnabled())) wTimeCount=TIME_OUT_CARD_FAST;
		SetGameClock(wCurrentUser,IDI_OUT_CARD,wTimeCount);

		return;
	}

	__super::OnTimer(nIDEvent);
}

//��ʼ��ť
LRESULT CGameClientEngine::OnStart(WPARAM wParam, LPARAM lParam)
{
	//���ñ���
	m_wTimeOutCount=0;
	m_bHandCardCount=0;
	m_bTurnCardCount=0;
	m_bTurnOutType=CT_ERROR;
	m_wMostUser=INVALID_CHAIR;
	ZeroMemory(m_bHandCardData,sizeof(m_bHandCardData));
	ZeroMemory(m_bTurnCardData,sizeof(m_bTurnCardData));

	//���ý���
	KillGameClock(IDI_START_GAME);
	m_GameClientView.SetBombCount(INVALID_CHAIR,0);
	m_GameClientView.SetCardCount(INVALID_CHAIR,0);
	m_GameClientView.SetPassFlag(INVALID_CHAIR,false);

	//���ؿؼ�
	m_GameClientView.m_btStart.ShowWindow(FALSE);
	m_GameClientView.m_ScoreView.ShowWindow(SW_HIDE);

	//�����˿�
	m_GameClientView.m_HandCardControl.SetCardData(NULL,0);
	m_GameClientView.m_HandCardControl.SetPositively(false);
	m_GameClientView.m_LeaveCardControl[0].SetCardData(NULL,0);
	m_GameClientView.m_LeaveCardControl[1].SetCardData(NULL,0);
	for (WORD i=0;i<GAME_PLAYER;i++) m_GameClientView.m_UserCardControl[i].SetCardData(NULL,0);

	//������Ϣ
	SendUserReady(NULL,0);

	m_GameClientView.InvalidGameView(0,0,0,0);
	return 0;
}

//������Ϣ
LRESULT CGameClientEngine::OnOutCard(WPARAM wParam, LPARAM lParam)
{
	//״̬�ж�
	if ((m_GameClientView.m_btOutCard.IsWindowEnabled()==FALSE)||(m_GameClientView.m_btOutCard.IsWindowVisible()==FALSE)) return 0;

	//���ý���
	KillGameClock(IDI_OUT_CARD);
	m_GameClientView.m_btOutCard.ShowWindow(SW_HIDE);
	m_GameClientView.m_btOutCard.EnableWindow(FALSE);
	m_GameClientView.m_btPassCard.ShowWindow(SW_HIDE);
	m_GameClientView.m_btPassCard.EnableWindow(FALSE);
	m_GameClientView.m_btOutPrompt.ShowWindow(SW_HIDE);

	//��������
	CMD_C_OutCard OutCard;
	OutCard.bCardCount=(BYTE)m_GameClientView.m_HandCardControl.GetShootCard(OutCard.bCardData,CountArray(OutCard.bCardData));
	m_GameLogic.SortCardList(OutCard.bCardData,OutCard.bCardCount);
	SendSocketData(SUB_C_OUT_CART,&OutCard,sizeof(OutCard)-sizeof(OutCard.bCardData)+OutCard.bCardCount*sizeof(BYTE));

	//Ԥ����ʾ
	PlayGameSound(AfxGetInstanceHandle(),TEXT("OUT_CARD"));
	m_GameClientView.m_UserCardControl[1].SetCardData(OutCard.bCardData,OutCard.bCardCount);

	//Ԥ��ɾ��
	BYTE bSourceCount=m_bHandCardCount;
	m_bHandCardCount-=OutCard.bCardCount;
	m_GameLogic.RemoveCard(OutCard.bCardData,OutCard.bCardCount,m_bHandCardData,bSourceCount);
	m_GameClientView.m_HandCardControl.SetCardData(m_bHandCardData,m_bHandCardCount);

	return 0;
}

//��������
LRESULT CGameClientEngine::OnPassCard(WPARAM wParam, LPARAM lParam)
{
	//״̬�ж�
	if (m_GameClientView.m_btPassCard.IsWindowEnabled()==FALSE) return 0;

	//���ý���
	KillGameClock(IDI_OUT_CARD);
	m_GameClientView.m_btOutCard.ShowWindow(SW_HIDE);
	m_GameClientView.m_btOutCard.EnableWindow(FALSE);
	m_GameClientView.m_btPassCard.ShowWindow(SW_HIDE);
	m_GameClientView.m_btPassCard.EnableWindow(FALSE);
	m_GameClientView.m_btOutPrompt.ShowWindow(SW_HIDE);

	//��������
	SendSocketData(SUB_C_PASS_CARD);

	//Ԥ����ʾ
	m_GameClientView.m_UserCardControl[1].SetCardData(NULL,0);
	m_GameClientView.SetPassFlag(1,true);
	PlayGameSound(AfxGetInstanceHandle(),TEXT("OUT_CARD"));
	m_GameClientView.m_HandCardControl.SetCardData(m_bHandCardData,m_bHandCardCount);

	return 0;
}

//������ʾ
LRESULT CGameClientEngine::OnOutPrompt(WPARAM wParam, LPARAM lParam)
{
	//���Ʒ���
	if (m_bTurnCardCount==0)
	{
		//�׳��ж�
		tagOutCardResult OutCardResult;
		m_GameLogic.SearchOutCard(m_bHandCardData,m_bHandCardCount,NULL,0,CT_ERROR,OutCardResult);

		//��ʾ����
		if (OutCardResult.cbCardCount!=0)
		{
			m_GameClientView.m_btOutCard.EnableWindow(TRUE);
			m_GameClientView.m_HandCardControl.SetShootCard(OutCardResult.cbResultCard,OutCardResult.cbCardCount);
		}
	}
	else 
	{
		//���Ʒ���
		tagOutCardResult OutCardResult;
		m_GameLogic.SearchOutCard(m_bHandCardData,m_bHandCardCount,m_bTurnCardData,m_bTurnCardCount,m_bTurnOutType,OutCardResult);

		//��ʾ����
		if (OutCardResult.cbCardCount!=0)
		{
			m_GameClientView.m_btOutCard.EnableWindow(TRUE);
			m_GameClientView.m_HandCardControl.SetShootCard(OutCardResult.cbResultCard,OutCardResult.cbCardCount);
		}
		else OnPassCard(0,0);
	}

	return 0;
}

//�Ҽ��˿�
LRESULT CGameClientEngine::OnLeftHitCard(WPARAM wParam, LPARAM lParam)
{
	//���ÿؼ�
	bool bOutCard=VerdictOutCard();
	m_GameClientView.m_btOutCard.EnableWindow(bOutCard?TRUE:FALSE);

	return 0;
}

//����˿�
LRESULT CGameClientEngine::OnRightHitCard(WPARAM wParam, LPARAM lParam)
{
	//�û�����
	OnOutCard(0,0);

	return 0;
}


//////////////////////////////////////////////////////////////////////////////////
