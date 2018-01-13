#include "StdAfx.h"
#include "TableFrameSink.h"

//////////////////////////////////////////////////////////////////////////////////
//��̬����
const WORD			CTableFrameSink::m_wPlayerCount=GAME_PLAYER;			//��Ϸ����


//���캯��
CTableFrameSink::CTableFrameSink()
{
	//��Ϸ����
	m_wBankerUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;
	ZeroMemory(m_bBombCount,sizeof(m_bBombCount));
	ZeroMemory(m_lGameScore,sizeof(m_lGameScore));

	//��ʷ����
	ZeroMemory(m_lAllTurnScore,sizeof(m_lAllTurnScore));
	ZeroMemory(m_lLastTurnScore,sizeof(m_lLastTurnScore));

	//������Ϣ
	m_bBaoPei=false;
	m_bTurnCardCount=0;
	m_bTurnCardType=CT_ERROR;
	m_wTurnWiner=INVALID_CHAIR;
	ZeroMemory(m_bTurnCardData,sizeof(m_bTurnCardData));

	//�˿���Ϣ
	ZeroMemory(m_bCardCount,sizeof(m_bCardCount));
	ZeroMemory(m_bHandCardData,sizeof(m_bHandCardData));

	//�������
	m_pITableFrame=NULL;
	m_pGameServiceOption=NULL;

	return;
}

//��������
CTableFrameSink::~CTableFrameSink()
{
}

//�ӿڲ�ѯ
VOID * CTableFrameSink::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(ITableFrameSink,Guid,dwQueryVer);
	QUERYINTERFACE(ITableUserAction,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(ITableFrameSink,Guid,dwQueryVer);
	return NULL;
}

//��������
bool CTableFrameSink::Initialization(IUnknownEx * pIUnknownEx)
{
	//��ѯ�ӿ�
	m_pITableFrame=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,ITableFrame);

	//�����ж�
	if (m_pITableFrame==NULL)
	{
		CTraceService::TraceString(TEXT("��Ϸ���� CTableFrameSink ��ѯ ITableFrame �ӿ�ʧ��"),TraceLevel_Exception);
		return false;
	}
	//��ȡ����
	m_pGameServiceOption=m_pITableFrame->GetGameServiceOption();
	ASSERT(m_pGameServiceOption!=NULL);

	m_lCellScore=m_pGameServiceOption->lCellScore;
	if(m_lCellScore<=0) m_lCellScore=1;
	//��ʼģʽ
	m_pITableFrame->SetStartMode(START_MODE_FULL_READY);

	return true;
}

//��λ����
VOID CTableFrameSink::RepositionSink()
{
	//��Ϸ����
	m_wBankerUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;
	ZeroMemory(m_bBombCount,sizeof(m_bBombCount));
	ZeroMemory(m_lGameScore,sizeof(m_lGameScore));

	//������Ϣ
	m_bBaoPei=false;
	m_bTurnCardCount=0;
	m_bTurnCardType=CT_ERROR;
	m_wTurnWiner=INVALID_CHAIR;
	ZeroMemory(m_bTurnCardData,sizeof(m_bTurnCardData));

	//�˿���Ϣ
	ZeroMemory(m_bCardCount,sizeof(m_bCardCount));
	ZeroMemory(m_bHandCardData,sizeof(m_bHandCardData));


	return;
}

//��Ϸ��ʼ
bool CTableFrameSink::OnEventGameStart()
{
	//����״̬
	m_pITableFrame->SetGameStatus(GAME_SCENE_PLAY);

	//�����˿�
	BYTE bRandCard[48];
	m_GameLogic.RandCardList(bRandCard,CountArray(bRandCard));

	//�ַ��˿�
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		m_bCardCount[i]=16;
		CopyMemory(&m_bHandCardData[i],&bRandCard[i*m_bCardCount[i]],sizeof(BYTE)*m_bCardCount[i]);
		m_GameLogic.SortCardList(m_bHandCardData[i],m_bCardCount[i]);
	}

	//�����û�
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		for (BYTE j=0;j<m_bCardCount[i];j++)
		{
			if (m_bHandCardData[i][j]==0x33) 
			{
				//�����û�
				m_wBankerUser=i;
				m_wCurrentUser=i;

				goto SET_CURRENT_USER;
			}
		}
	}

	//�����ж�
	ASSERT(FALSE);
	SET_CURRENT_USER:

	//�����˿�
	tagAnalyseResult AnalyseResult;
	m_GameLogic.AnalysebCardData(m_bHandCardData[m_wCurrentUser],m_bCardCount[m_wCurrentUser],AnalyseResult);

	//���ƴ���
	if ((AnalyseResult.bFourCount>0)&&(AnalyseResult.bFourLogicVolue[AnalyseResult.bFourCount-1]==3))
	{
		//�����û�
		m_wCurrentUser=INVALID_CHAIR;

		//ɾ���˿�
		BYTE cbCardData[]={0x33,0x23,0x13,0x03};
		m_GameLogic.RemoveCard(cbCardData,CountArray(cbCardData),m_bHandCardData[m_wBankerUser],m_bCardCount[m_wBankerUser]);
		m_bCardCount[m_wBankerUser]-=CountArray(cbCardData);
	}

	//�����˿�
	CMD_S_GameStart GameStart;
	GameStart.wBankerUser=m_wBankerUser;
	GameStart.wCurrentUser=m_wCurrentUser;
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		CopyMemory(GameStart.cbCardData,m_bHandCardData[i],sizeof(GameStart.cbCardData));
		m_pITableFrame->SendTableData(i,SUB_S_GAME_START,&GameStart,sizeof(GameStart));
		m_pITableFrame->SendLookonData(i,SUB_S_GAME_START,&GameStart,sizeof(GameStart));
		ZeroMemory(GameStart.cbCardData,sizeof(GameStart.cbCardData));
	}

	//ʤ������
	if (m_wCurrentUser==INVALID_CHAIR) OnEventGameConclude(m_wBankerUser,NULL,GER_NORMAL);

	return true;
}

//��Ϸ����
bool CTableFrameSink::OnEventGameConclude(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)
{
	switch (cbReason)
	{
	case GER_DISMISS:		//��Ϸ��ɢ
		{
			//Ч�����
			ASSERT(pIServerUserItem!=NULL);
			ASSERT(wChairID<m_wPlayerCount);

			//��������
			CMD_S_GameEnd GameEnd;
			ZeroMemory(&GameEnd,sizeof(GameEnd));

			//ʣ���˿�
			BYTE bCardPos=0;
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				GameEnd.bCardCount[i]=m_bCardCount[i];
				CopyMemory(&GameEnd.bCardData[bCardPos],m_bHandCardData[i],m_bCardCount[i]*sizeof(BYTE));
				bCardPos+=m_bCardCount[i];
			}

			//������Ϣ
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));

			//������Ϸ
			m_pITableFrame->ConcludeGame(GAME_SCENE_FREE);

			return true;
		}
	case GER_NORMAL:		//�������
		{
			//�������
			CMD_S_GameEnd GameEnd;
			ZeroMemory(&GameEnd,sizeof(GameEnd));

			//ʣ���˿�
			BYTE bCardPos=0;
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				GameEnd.bCardCount[i]=m_bCardCount[i];
				CopyMemory(&GameEnd.bCardData[bCardPos],m_bHandCardData[i],m_bCardCount[i]*sizeof(BYTE));
				bCardPos+=m_bCardCount[i];
			}

			//ͳ�ƻ���
			LONGLONG lCellScore=m_pGameServiceOption->lCellScore;
			if ((wChairID==m_wBankerUser)&&(m_bCardCount[wChairID]==12))
			{
				//����ͳ��
				GameEnd.lGameScore[wChairID]=64L*lCellScore;
				GameEnd.lGameScore[(wChairID+1)%m_wPlayerCount]=-32L*lCellScore;
				GameEnd.lGameScore[(wChairID+2)%m_wPlayerCount]=-32L*lCellScore;

				//���⴦��
				if (m_bBaoPei==true)
				{
					WORD wBaoPeiUser=(wChairID+m_wPlayerCount-1)%m_wPlayerCount;
					GameEnd.lGameScore[wBaoPeiUser]=-GameEnd.lGameScore[wChairID];
					GameEnd.lGameScore[(wBaoPeiUser+2)%m_wPlayerCount]=0L;
				}
			}
			else
			{
				//����ͳ��
				for (WORD i=0;i<m_wPlayerCount;i++)
				{
					//��Ϸ����
					if (m_bCardCount[i]>1)
					{
						LONGLONG lScoreTimes=(m_bCardCount[i]==16)?2L:1L;
						GameEnd.lGameScore[i]-=m_bCardCount[i]*lCellScore*lScoreTimes;
						GameEnd.lGameScore[wChairID]+=m_bCardCount[i]*lCellScore*lScoreTimes;
					}
				}

				//���⴦��
				if (m_bBaoPei==true)
				{
					WORD wBaoPeiUser=(wChairID+m_wPlayerCount-1)%m_wPlayerCount;
					GameEnd.lGameScore[wBaoPeiUser]=-GameEnd.lGameScore[wChairID];
					GameEnd.lGameScore[(wBaoPeiUser+2)%m_wPlayerCount]=0L;
				}

				//ը������
				for (WORD i=0;i<m_wPlayerCount;i++)	GameEnd.lGameScore[i]+=m_lGameScore[i];
			}

			//����ͳ��
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				m_lAllTurnScore[i]+=GameEnd.lGameScore[i];
				m_lLastTurnScore[i]=GameEnd.lGameScore[i];
			}


			//��������
			LONGLONG lRevenue=0L;
			LONGLONG lUserScore[GAME_PLAYER];ZeroMemory(lUserScore,sizeof(lUserScore));
			CopyMemory(lUserScore,GameEnd.lGameScore,sizeof(lUserScore));

			//ͳ�ƻ���
			tagScoreInfo ScoreInfo[GAME_PLAYER];
			ZeroMemory(ScoreInfo,sizeof(ScoreInfo));
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				//���û���
				if ((m_pGameServiceOption->wServerType&GAME_GENRE_GOLD)!=0&&lUserScore[i]>0)
				{
					lRevenue=lUserScore[i]*m_pGameServiceOption->wRevenueRatio/1000;
					GameEnd.lGameScore[i]=lUserScore[i]-lRevenue;
					GameEnd.lGameTax+=lRevenue;
				}
				else
				{
					lRevenue=0;
					GameEnd.lGameScore[i]=lUserScore[i];
				}
				
				BYTE ScoreKind=(GameEnd.lGameScore[i]>=0L)?SCORE_TYPE_WIN:SCORE_TYPE_LOSE;
				ScoreInfo[i].cbType = ScoreKind;
				ScoreInfo[i].lRevenue = lRevenue;
				ScoreInfo[i].lScore = GameEnd.lGameScore[i];
			}
			//д�����
			m_pITableFrame->WriteTableScore(ScoreInfo,CountArray(ScoreInfo));
			//������Ϣ
			CString szEndMessage;
			szEndMessage.Format(TEXT("���ֽ������ɼ�ͳ�ƣ�\n %s �� %I64d ��\n %s �� %I64d ��\n %s �� %I64d ��\n"),
				m_pITableFrame->GetTableUserItem(0)->GetNickName(),GameEnd.lGameScore[0],m_pITableFrame->GetTableUserItem(1)->GetNickName(),GameEnd.lGameScore[1],
				m_pITableFrame->GetTableUserItem(2)->GetNickName(),GameEnd.lGameScore[2]);
#ifndef DEBUG
			//���ͽ�����Ϣ
			m_pITableFrame->SendGameMessage(szEndMessage,SMT_CHAT);
#endif
			//������Ϣ
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));

			//������Ϸ
			m_pITableFrame->ConcludeGame(GAME_SCENE_FREE);

			return true;
		}
	case GER_USER_LEAVE:		//�û�ǿ��
	case GER_NETWORK_ERROR:
		{
			//�������
			CMD_S_GameEnd GameEnd;
			ZeroMemory(&GameEnd,sizeof(GameEnd));

			//ʣ���˿�
			BYTE bCardPos=0;
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				GameEnd.bCardCount[i]=m_bCardCount[i];
				CopyMemory(&GameEnd.bCardData[bCardPos],m_bHandCardData[i],m_bCardCount[i]*sizeof(BYTE));
				bCardPos+=m_bCardCount[i];
			}

			//ͳ�ƻ���
			GameEnd.lGameScore[wChairID]=-32L*m_pGameServiceOption->lCellScore;
			GameEnd.lGameScore[(wChairID+1)%m_wPlayerCount]=16L*m_pGameServiceOption->lCellScore;
			GameEnd.lGameScore[(wChairID+2)%m_wPlayerCount]=16L*m_pGameServiceOption->lCellScore;

			//������Ϣ
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));

			//�޸Ļ���
			tagScoreInfo ScoreInfo[GAME_PLAYER];
			ZeroMemory(ScoreInfo,sizeof(ScoreInfo));
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				if(i==wChairID)
				{
					ScoreInfo[i].lScore=GameEnd.lGameScore[wChairID];
					ScoreInfo[i].cbType=SCORE_TYPE_FLEE;
				}
				else
				{
					ScoreInfo[i].lScore=GameEnd.lGameScore[i];
					ScoreInfo[i].cbType=SCORE_TYPE_WIN;
				}
			}			
			
			m_pITableFrame->WriteTableScore(ScoreInfo,CountArray(ScoreInfo));
			//������Ϸ
			m_pITableFrame->ConcludeGame(GAME_SCENE_FREE);

			return true;
		}
	}

	//�������
	ASSERT(FALSE);

	return false;
}

//���ͳ���
bool CTableFrameSink::OnEventSendGameScene(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbGameStatus, bool bSendSecret)
{
	switch (cbGameStatus)
	{
	case GAME_SCENE_FREE:		//����״̬
		{
			//��������
			CMD_S_StatusFree StatusFree;
			ZeroMemory(&StatusFree,sizeof(StatusFree));

			//���ñ���
			StatusFree.lBaseScore=m_lCellScore;

			//���ͳ���
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusFree,sizeof(StatusFree));
		}
	case GAME_SCENE_PLAY:		//��Ϸ״̬
		{
			//��������
			CMD_S_StatusPlay StatusPlay;
			ZeroMemory(&StatusPlay,sizeof(StatusPlay));

			//���ñ���
			StatusPlay.wBankerUser=m_wBankerUser;
			StatusPlay.wLastOutUser=m_wTurnWiner;
			StatusPlay.wCurrentUser=m_wCurrentUser;
			StatusPlay.bTurnCardCount=m_bTurnCardCount;
			StatusPlay.lBaseScore=m_lCellScore;
			CopyMemory(StatusPlay.bBombCount,m_bBombCount,sizeof(m_bBombCount));
			CopyMemory(StatusPlay.bCardCount,m_bCardCount,sizeof(m_bCardCount));
			CopyMemory(StatusPlay.bTurnCardData,m_bTurnCardData,sizeof(BYTE)*m_bTurnCardCount);
			CopyMemory(StatusPlay.bCardData,m_bHandCardData[wChairID],sizeof(BYTE)*m_bCardCount[wChairID]);

			//��ʷ����
			CopyMemory(StatusPlay.lAllTurnScore,m_lAllTurnScore,sizeof(m_lAllTurnScore));
			CopyMemory(StatusPlay.lLastTurnScore,m_lLastTurnScore,sizeof(m_lLastTurnScore));

			//���ͳ���
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusPlay,sizeof(StatusPlay));
		}
	}

	//�������
	ASSERT(FALSE);

	return false;
}

//��ʱ���¼�
bool CTableFrameSink::OnTimerMessage(DWORD wTimerID, WPARAM wBindParam)
{
	return false;
}

//��Ϸ��Ϣ
bool CTableFrameSink::OnGameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	switch (wSubCmdID)
	{
	case SUB_C_OUT_CART:		//�û�����
		{
			//��������
			CMD_C_OutCard * pOutCard=(CMD_C_OutCard *)pData;
			WORD wHeadSize=sizeof(CMD_C_OutCard)-sizeof(pOutCard->bCardData);

			//Ч������
			ASSERT(wDataSize>=wHeadSize);
			if (wDataSize<wHeadSize) return false;
			ASSERT(wDataSize==(wHeadSize+pOutCard->bCardCount*sizeof(pOutCard->bCardData[0])));
			if (wDataSize!=(wHeadSize+pOutCard->bCardCount*sizeof(pOutCard->bCardData[0]))) return false;

			//�û�Ч��
			tagUserInfo * pUserData=pIServerUserItem->GetUserInfo();
			if (pUserData->cbUserStatus!=US_PLAYING) return true;

			//��Ϣ����
			return OnUserOutCard(pUserData->wChairID,pOutCard->bCardData,pOutCard->bCardCount);
		}
	case SUB_C_PASS_CARD:
		{
			//�û�Ч��
			tagUserInfo * pUserData=pIServerUserItem->GetUserInfo();
			if (pUserData->cbUserStatus!=US_PLAYING) return true;

			//��Ϣ����
			return OnUserPassCard(pUserData->wChairID);
		}
	}


	return false;
}

//�����Ϣ
bool CTableFrameSink::OnFrameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	return false;
}

//�û�����
bool CTableFrameSink::OnUserOutCard(WORD wChairID, BYTE bCardData[], BYTE bCardCount)
{
	//Ч��״̬
	if (m_pITableFrame->GetGameStatus()!=GAME_SCENE_PLAY) return true;
	if (wChairID!=m_wCurrentUser) return false;

	//�����ж�
	BYTE bCardType=m_GameLogic.GetCardType(bCardData,bCardCount);
	if (bCardType==CT_ERROR) return false;

	//�׳��ж�
	if ((wChairID==m_wBankerUser)&&(m_bCardCount[wChairID]==16))
	{
		BYTE i=0;
		for (i=0;i<bCardCount;i++)
		{
			if (bCardData[i]==0x33) break;
		}
		if (i==bCardCount) return false;
	}

	//�������
	if (m_bTurnCardCount==0) m_bTurnCardCount=bCardCount;
	else if (m_GameLogic.CompareCard(bCardData,m_bTurnCardData,bCardCount,m_bTurnCardCount)==false) return false;

	//ɾ���˿�
	if (m_GameLogic.RemoveCard(bCardData,bCardCount,m_bHandCardData[wChairID],m_bCardCount[wChairID])==false) return false;
	m_bCardCount[wChairID]-=bCardCount;

	//�����ж�
	if ((m_bCardCount[wChairID]!=0))
	{
		//��ȡ�û�
		WORD wNextPlayer=(wChairID+1)%m_wPlayerCount;

		//�����ж�
		if ((bCardCount==1)&&(m_bCardCount[wNextPlayer]==1))
		{
			m_bBaoPei=(m_GameLogic.CompareCard(m_bHandCardData[wChairID],bCardData,1,1)==true);
		}
		else 
		{
			//���ñ���
			m_bBaoPei=false;
		}
	}

	//���Ƽ�¼
	m_bTurnCardType=bCardType;
	m_bTurnCardCount=bCardCount;
	CopyMemory(m_bTurnCardData,bCardData,sizeof(BYTE)*bCardCount);

	//�л��û�
	m_wTurnWiner=wChairID;
	if (m_bCardCount[wChairID]!=0) m_wCurrentUser=(m_wCurrentUser+1)%m_wPlayerCount;
	else m_wCurrentUser=INVALID_CHAIR;

	//��������
	CMD_S_OutCard OutCard;
	OutCard.bCardCount=bCardCount;
	OutCard.wOutCardUser=wChairID;
	OutCard.wCurrentUser=m_wCurrentUser;
	CopyMemory(OutCard.bCardData,m_bTurnCardData,m_bTurnCardCount*sizeof(BYTE));

	//��������
	WORD wSendSize=sizeof(OutCard)-sizeof(OutCard.bCardData)+OutCard.bCardCount*sizeof(BYTE);
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_OUT_CARD,&OutCard,wSendSize);
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_OUT_CARD,&OutCard,wSendSize);

	//�����ж�
	if (m_wCurrentUser==INVALID_CHAIR) OnEventGameConclude(wChairID,NULL,GER_NORMAL);

	return true;
}

//�û�����
bool CTableFrameSink::OnUserPassCard(WORD wChairID)
{
	//Ч��״̬
	if (m_pITableFrame->GetGameStatus()!=GAME_SCENE_PLAY) return true;
	if ((wChairID!=m_wCurrentUser)||(m_bTurnCardCount==0)) return false;

	//�˿˷���
	tagOutCardResult OutCardResult;
	m_GameLogic.SearchOutCard(m_bHandCardData[wChairID],m_bCardCount[wChairID],m_bTurnCardData,m_bTurnCardCount,m_bTurnCardType,OutCardResult);

	//�����ж�
	if (OutCardResult.cbCardCount!=0) return false;

	//���ñ���
	m_bBaoPei=false;
	m_wCurrentUser=(m_wCurrentUser+1)%m_wPlayerCount;
	if (m_wCurrentUser==m_wTurnWiner)
	{
		//ը���ж�
		if (m_GameLogic.GetCardType(m_bTurnCardData,m_bTurnCardCount)==CT_BOMB)
		{
			m_bBombCount[m_wTurnWiner]++;
			m_lGameScore[m_wTurnWiner]+=10*m_pGameServiceOption->lCellScore;
			m_lGameScore[(m_wTurnWiner+1)%m_wPlayerCount]+=-5*m_pGameServiceOption->lCellScore;
			m_lGameScore[(m_wTurnWiner+2)%m_wPlayerCount]+=-5*m_pGameServiceOption->lCellScore;
		}

		//���ñ���
		m_bTurnCardCount=0;
	}

	//��������
	CMD_S_PassCard PassCard;
	PassCard.wPassUser=wChairID;
	PassCard.wCurrentUser=m_wCurrentUser;
	PassCard.bNewTurn=(m_bTurnCardCount==0)?TRUE:FALSE;
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_PASS_CARD,&PassCard,sizeof(PassCard));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_PASS_CARD,&PassCard,sizeof(PassCard));

	return true;
}

//////////////////////////////////////////////////////////////////////////////////
