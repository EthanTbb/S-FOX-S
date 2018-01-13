#include "StdAfx.h"
#include "TableFrameSink.h"

//////////////////////////////////////////////////////////////////////////////////
//��̬����
const WORD			CTableFrameSink::m_wPlayerCount=GAME_PLAYER;			//��Ϸ����

//���캯��
CTableFrameSink::CTableFrameSink()
{
	//��������
	m_dwPlayTimeCount=0L;

	//����״̬
	ZeroMemory(m_cbResponses,sizeof(m_cbResponses));
	ZeroMemory(m_wRequestTimes,sizeof(m_wRequestTimes));

	//��Ϸ����
	m_wHeadUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;
	ZeroMemory(m_cbContinue,sizeof(m_cbContinue));
	ZeroMemory(m_wPersistInfo,sizeof(m_wPersistInfo));
	ZeroMemory(m_bAutoPlayer,sizeof(m_bAutoPlayer));
	ZeroMemory(m_510KCardRecord,sizeof(m_510KCardRecord));
	m_bFirstUserLastTurn=false;

	//�÷ֱ���
	m_TurnScore=0;
	ZeroMemory(m_PlayerScore,sizeof(m_PlayerScore));

	//��Ϸ�ȼ�
	m_wOrderIndex=0;
	m_cbMainValue=0x02;
	m_cbValueOrder[0]=0x02;
	m_cbValueOrder[1]=0x02;

	//ʤ����Ϣ
	m_wWinCount=0;
	ZeroMemory(m_wWinOrder,sizeof(m_wWinOrder));

	//������Ϣ
	m_cbTurnCardCount=0;
	m_cbTurnCardType=CT_ERROR;
	m_wTurnWiner=INVALID_CHAIR;
	ZeroMemory(m_cbTurnCardData,sizeof(m_cbTurnCardData));
	ZeroMemory(m_cbMagicCardData,sizeof(m_cbMagicCardData));

	//�˿���Ϣ
	ZeroMemory(m_cbHandCardData,sizeof(m_cbHandCardData));
	ZeroMemory(m_cbHandCardCount,sizeof(m_cbHandCardCount));

	//�������
	m_pITableFrame=NULL;
	//m_pITableFrameControl=NULL;
	m_pGameServiceOption=NULL;

	return;
}

//��������
CTableFrameSink::~CTableFrameSink()
{
}

//�ӿڲ�ѯ
void *  CTableFrameSink::QueryInterface(const IID & Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(ITableFrameSink,Guid,dwQueryVer);
	QUERYINTERFACE(ITableUserAction,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(ITableFrameSink,Guid,dwQueryVer);
	return NULL;
}

//��������
bool  CTableFrameSink::Initialization(IUnknownEx * pIUnknownEx)
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

	//��ʼģʽ
	m_pITableFrame->SetStartMode(START_MODE_FULL_READY);

	return true;
}

//��������
LONGLONG CTableFrameSink::QueryConsumeQuota(IServerUserItem * pIServerUserItem)
{
	//Ч��״̬
	ASSERT((pIServerUserItem!=NULL)&&(pIServerUserItem->GetTableID()==m_pITableFrame->GetTableID()));
	if ((pIServerUserItem==NULL)||(pIServerUserItem->GetTableID()!=m_pITableFrame->GetTableID())) return 0;


	//�û��ж�
	switch (pIServerUserItem->GetUserStatus())
	{
	case US_SIT:		//����״̬
	case US_READY:		//׼��״̬
		{
			return pIServerUserItem->GetUserScore()-m_pGameServiceOption->lMinTableScore;
		}
	case US_PLAYING:	//��Ϸ״̬
	case US_OFFLINE:	//����״̬
		{
			//WORD wMaxTimes=m_pGameCustomRule->wMaxScoreTimes;
			//return pIServerUserItem->GetUserScore()-m_pGameServiceOption->lCellScore*wMaxTimes;
			return pIServerUserItem->GetUserScore()-m_pGameServiceOption->lCellScore;
		}
	case US_LOOKON:		//�Թ�״̬
		{
			return pIServerUserItem->GetUserScore();
		}
	}

	return 0L;
}

//���ٻ���
LONGLONG CTableFrameSink::QueryLessEnterScore(WORD wChairID, IServerUserItem * pIServerUserItem)
{
	return 0L;
}


//��λ����
VOID CTableFrameSink::RepositionSink()
{
	//��������
	m_dwPlayTimeCount=0L;

	//����״̬
	ZeroMemory(m_cbResponses,sizeof(m_cbResponses));
	ZeroMemory(m_wRequestTimes,sizeof(m_wRequestTimes));
	m_bFirstUserLastTurn=false;

	//��Ϸ����
	m_wCurrentUser=INVALID_CHAIR;
	ZeroMemory(m_cbContinue,sizeof(m_cbContinue));
	ZeroMemory(m_bAutoPlayer,sizeof(m_bAutoPlayer));
	ZeroMemory(m_510KCardRecord,sizeof(m_510KCardRecord));

	//�÷ֱ���
	m_TurnScore=0;

	//ʤ����Ϣ
	m_wWinCount=0;
	ZeroMemory(m_wWinOrder,sizeof(m_wWinOrder));


	//������Ϣ
	m_cbTurnCardCount=0;
	m_cbTurnCardType=CT_ERROR;
	m_wTurnWiner=INVALID_CHAIR;
	ZeroMemory(m_cbTurnCardData,sizeof(m_cbTurnCardData));
	ZeroMemory(m_cbMagicCardData,sizeof(m_cbMagicCardData));

	//�˿���Ϣ
	ZeroMemory(m_cbHandCardData,sizeof(m_cbHandCardData));
	ZeroMemory(m_cbHandCardCount,sizeof(m_cbHandCardCount));

	return;
}

//��Ϸ��ʼ
bool CTableFrameSink::OnEventGameStart()
{
	//��Ϸʱ��
	m_dwPlayTimeCount=(DWORD)time(NULL);

	//����״̬
	m_pITableFrame->SetGameStatus(GAME_SCENE_PLAY);

	do{
		//�����˿�
		BYTE cbRandCard[FULL_COUNT];
		m_GameLogic.RandCardList(cbRandCard,CountArray(cbRandCard));

		//�û��˿�
		for (WORD i=0;i<GAME_PLAYER;i++)
		{
			m_cbHandCardCount[i]=(MAX_COUNT-1);
			CopyMemory(&m_cbHandCardData[i],&cbRandCard[i*m_cbHandCardCount[i]],sizeof(BYTE)*m_cbHandCardCount[i]);
		}
	}while(IsAllUserHaveScore(m_cbHandCardData)==false);

	//�����û�

	m_wCurrentUser=rand()%GAME_PLAYER;
	m_wTurnWiner=m_wCurrentUser;

	//�����߼�
	m_GameLogic.SetMainValue(m_cbMainValue);

	//��������
	CMD_S_GameStart GameStart;
	ZeroMemory(&GameStart,sizeof(GameStart));

	//�������
	GameStart.cbMainValue=m_cbMainValue;
	GameStart.wCurrentUser=m_wCurrentUser;
	CopyMemory(GameStart.cbValueOrder,m_cbValueOrder,sizeof(m_cbValueOrder));
	CopyMemory(GameStart.wPersistInfo,m_wPersistInfo,sizeof(m_wPersistInfo));

	//��������
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//�����˿�
		ASSERT(CountArray(GameStart.cbCardData)>=m_cbHandCardCount[i]);		
		CopyMemory(GameStart.cbCardData,m_cbHandCardData[i],sizeof(BYTE)*m_cbHandCardCount[i]);

		//��������
		m_pITableFrame->SendTableData(i,SUB_S_GAME_START,&GameStart,sizeof(GameStart));
		m_pITableFrame->SendLookonData(i,SUB_S_GAME_START,&GameStart,sizeof(GameStart));
		ZeroMemory(GameStart.cbCardData,sizeof(GameStart.cbCardData));
	}

	//�����˿�
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		m_GameLogic.SortCardList(m_cbHandCardData[i],m_cbHandCardCount[i],ST_ORDER);
	}

	return true;
}

//��Ϸ����
bool CTableFrameSink::OnEventGameConclude(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)
{
	ZeroMemory(m_bAutoPlayer,sizeof(m_bAutoPlayer));
	switch (cbReason)
	{
	case GER_NORMAL:		//�������
		{
			//��������
			CMD_S_GameEnd GameEnd;
			ZeroMemory(&GameEnd,sizeof(GameEnd));

			m_TurnScore=0;
			GameEnd.TurnScore=m_TurnScore;

			//ʤ����Ϣ
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				if (m_cbHandCardCount[i]!=0)
				{
					m_wWinOrder[m_wWinCount++]=i;
				}
			}

			//�û��˿�
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				GameEnd.cbCardCount[i]=m_cbHandCardCount[i];
				CopyMemory(&GameEnd.cbCardData[i],m_cbHandCardData[i],m_cbHandCardCount[i]*sizeof(BYTE));
			}

			//�����ȼ�
			BYTE cbValueOrder=0;
			if ((m_wWinOrder[0]%2)==(m_wWinOrder[1]%2)) cbValueOrder=3;
			else if ((m_wWinOrder[0]%2)==((m_wWinOrder[3]+1)%2)) cbValueOrder=2;
			else cbValueOrder=1;

			//��������
			bool bStatScore=false;
			bool bFinishDraw=false;
			WORD wWinnerUser=m_wWinOrder[0];
			CString szMessage;
			if( (m_wWinOrder[0]==0)&&(m_wWinOrder[1]==2) || (m_wWinOrder[0]==2)&&(m_wWinOrder[1]==0) )
			{
				m_PlayerScore[0]=50;
				m_PlayerScore[2]=50;
				//˫��ʤ����Ϣ

				szMessage.Format(TEXT("[ %s ]��[ %s ]˫��ʤ������Ϸ������\n"),m_pITableFrame->GetTableUserItem(0)->GetNickName(),m_pITableFrame->GetTableUserItem(2)->GetNickName());
				//����ʤ����Ϣ
				m_pITableFrame->SendGameMessage(szMessage,SMT_PROMPT);

			}
			else if( (m_wWinOrder[0]==1)&&(m_wWinOrder[1]==3) || (m_wWinOrder[0]==3)&&(m_wWinOrder[1]==1))
			{
				m_PlayerScore[1]=50;
				m_PlayerScore[3]=50;	
				//˫��ʤ����Ϣ
				szMessage.Format(TEXT("[ %s ]��[ %s ]˫��ʤ������Ϸ������\n"),m_pITableFrame->GetTableUserItem(1)->GetNickName(),m_pITableFrame->GetTableUserItem(3)->GetNickName());
				//����ʤ����Ϣ
				m_pITableFrame->SendGameMessage(szMessage,SMT_PROMPT);

			}
			else
			{
				int LeaveScore=0;
				LeaveScore=m_GameLogic.GetCardScore(m_cbHandCardData[m_wWinOrder[3]],m_cbHandCardCount[m_wWinOrder[3]]);
				//������Ϣ
				szMessage.Format(TEXT("���[ %s ]�ĵ÷� %I64d ���׸���һ��[ %s ]�������Ʒ� %d ���׸��Է���\n"),
					m_pITableFrame->GetTableUserItem(m_wWinOrder[3])->GetNickName(),m_PlayerScore[m_wWinOrder[3]],m_pITableFrame->GetTableUserItem(wWinnerUser)->GetNickName(),LeaveScore);
				//���͹�����Ϣ
				m_pITableFrame->SendGameMessage(szMessage,SMT_PROMPT);

				//���η���������
				m_PlayerScore[wWinnerUser]+=m_PlayerScore[m_wWinOrder[3]];
				m_PlayerScore[m_wWinOrder[3]]=0;

				//�Է��õ�ʣ�������Ʒ�
				m_PlayerScore[(m_wWinOrder[3]+1)%GAME_PLAYER]+=LeaveScore;
			}

			//�����������
			CopyMemory(GameEnd.PlayerScore,m_PlayerScore,sizeof(m_PlayerScore));

			//�����ж�
			//if ( (m_PlayerScore[0]+m_PlayerScore[2])!=0 || (m_PlayerScore[1]+m_PlayerScore[3])!=0 
			//	|| (m_wWinOrder[0]==0)&&(m_wWinOrder[1]==2) || (m_wWinOrder[0]==2)&&(m_wWinOrder[1]==0) 
			//	|| (m_wWinOrder[0]==1)&&(m_wWinOrder[1]==3) || (m_wWinOrder[0]==3)&&(m_wWinOrder[1]==1)
			//	)
			{
				//���ñ���
				bStatScore=true;
				bFinishDraw=true;

				//���ñ���
				m_wOrderIndex=0;
				m_cbMainValue=0x02;
				m_cbValueOrder[0]=0x02;
				m_cbValueOrder[1]=0x02;
			}

			//ȡ����ͷ�ȷ������
			LONGLONG minScore;
			IServerUserItem * pIServerUserItem=NULL;			
			pIServerUserItem=m_pITableFrame->GetTableUserItem(0);			
			minScore=pIServerUserItem->GetUserScore();
			for(WORD i=0;i<GAME_PLAYER;i++)
			{
				pIServerUserItem=m_pITableFrame->GetTableUserItem(i);
				if (pIServerUserItem==NULL) continue;
				
				if(pIServerUserItem->GetUserScore()<minScore)
					minScore=pIServerUserItem->GetUserScore();					
			}
			LONGLONG lScoreTimes=1L;
			if(minScore<=9L)				lScoreTimes=1L;
			else if(minScore<39L)		lScoreTimes=2L;
			else if(minScore<79L)		lScoreTimes=4L;
			else if(minScore<159L)		lScoreTimes=8L;
			else if(minScore<319L)		lScoreTimes=16L;
			else if(minScore<639L)		lScoreTimes=32L;
			else if(minScore<1279L)		lScoreTimes=64L;
			else if(minScore<2559L)		lScoreTimes=128L;
			else if(minScore<5119L)		lScoreTimes=256L;
			else if(minScore<10239L)		lScoreTimes=512L;
			else if(minScore<20479L)		lScoreTimes=1024L;
			else if(minScore<40959L)		lScoreTimes=2048L;
			else if(minScore<81919L)		lScoreTimes=4096L;
			else if(minScore<163839L)	lScoreTimes=8192L;
			else if(minScore<327679L)	lScoreTimes=16384L;
			else if(minScore<655359L)	lScoreTimes=32768L;
			else if(minScore<1310719L)	lScoreTimes=65536L;
			else if(minScore<2621439L)	lScoreTimes=131072L;
			else if(minScore<5242879L)	lScoreTimes=262144L;
			else						lScoreTimes=524288L;


			//����ͳ��
			if (bStatScore==true)
			{
				//��������
				LONGLONG lRevenue=0L;
				LONGLONG lUserScore[4];
				ZeroMemory(lUserScore,sizeof(lUserScore));
				LONGLONG lCellScore=m_pGameServiceOption->lCellScore*lScoreTimes;

				//��ͨ���
				if( (m_PlayerScore[1]+m_PlayerScore[3])>(m_PlayerScore[0]+m_PlayerScore[2]) )
				{
					lUserScore[0]=-1L*lCellScore;
					lUserScore[2]=-1L*lCellScore;
					lUserScore[1]=1L*lCellScore;
					lUserScore[3]=1L*lCellScore;
				}
				else if( (m_PlayerScore[0]+m_PlayerScore[2])>(m_PlayerScore[1]+m_PlayerScore[3]) )
				{
					lUserScore[0]=1L*lCellScore;
					lUserScore[2]=1L*lCellScore;
					lUserScore[1]=-1L*lCellScore;
					lUserScore[3]=-1L*lCellScore;
				}

				//������ͬ�����
				if( (m_PlayerScore[0]+m_PlayerScore[2]) == (m_PlayerScore[1]+m_PlayerScore[3]) )
				{
					lUserScore[m_wWinOrder[0]]=1L*lCellScore;
					lUserScore[(m_wWinOrder[0]+2)%GAME_PLAYER]=1L*lCellScore;
					lUserScore[(m_wWinOrder[0]+1)%GAME_PLAYER]=-1L*lCellScore;
					lUserScore[(m_wWinOrder[0]+3)%GAME_PLAYER]=-1L*lCellScore;
				}

				//�������
				if(m_PlayerScore[m_wWinOrder[0]]<=0)//ͷ�����
				{
					if(	((m_wWinOrder[0]+2)%GAME_PLAYER)==m_wWinOrder[3]  && 
						(m_PlayerScore[m_wWinOrder[0]]+m_PlayerScore[m_wWinOrder[3]])<=0 
						)//ͷ�ҵĶԼҵ�4���������ܷ�Ϊ0
					{
						lUserScore[m_wWinOrder[0]]=-4L*lCellScore;
						lUserScore[m_wWinOrder[3]]=-4L*lCellScore;
						lUserScore[m_wWinOrder[1]]=4L*lCellScore;
						lUserScore[m_wWinOrder[2]]=4L*lCellScore;
					}
				}
				else if(m_PlayerScore[m_wWinOrder[1]]<=0)//�������
				{
					if( ((m_wWinOrder[1]+2)%GAME_PLAYER)==m_wWinOrder[3]  && 
						(m_PlayerScore[m_wWinOrder[1]]+m_PlayerScore[m_wWinOrder[3]])<=0 
						)//ͷ�ҵĶԼҵ�4���������ܷ�Ϊ0
					{
						lUserScore[m_wWinOrder[1]]=-3L*lCellScore;
						lUserScore[m_wWinOrder[3]]=-3L*lCellScore;
						lUserScore[m_wWinOrder[0]]=3L*lCellScore;
						lUserScore[m_wWinOrder[2]]=3L*lCellScore;
					}
				}

				//˫�����
				if( (m_wWinOrder[0]==0)&&(m_wWinOrder[1]==2) || (m_wWinOrder[0]==2)&&(m_wWinOrder[1]==0) )
				{
					lUserScore[0]=2L*lCellScore;
					lUserScore[2]=2L*lCellScore;
					lUserScore[1]=-2L*lCellScore;
					lUserScore[3]=-2L*lCellScore;
				}
				else if( (m_wWinOrder[0]==1)&&(m_wWinOrder[1]==3) || (m_wWinOrder[0]==3)&&(m_wWinOrder[1]==1) )
				{
					lUserScore[0]=-2L*lCellScore;
					lUserScore[2]=-2L*lCellScore;
					lUserScore[1]=2L*lCellScore;
					lUserScore[3]=2L*lCellScore;	
				}

				CopyMemory(GameEnd.lGameScore,lUserScore,sizeof(lUserScore));

				tagScoreInfo ScoreInfo[GAME_PLAYER];
				ZeroMemory(ScoreInfo,sizeof(ScoreInfo));
				//ͳ�ƻ���
				for (WORD i=0;i<GAME_PLAYER;i++)
				{
					//���û���
					if ((m_pGameServiceOption->wServerType&GAME_GENRE_GOLD)!=0&&lUserScore[i]>0)
					{
						lRevenue=lUserScore[i]*m_pGameServiceOption->wRevenueRatio/1000;
						GameEnd.lGameScore[i]=lUserScore[i]-lRevenue;
					}
					else
					{
						lRevenue=0;
						GameEnd.lGameScore[i]=lUserScore[i];
					}
					
					//д�����
					BYTE ScoreKind=(GameEnd.lGameScore[i]>=0L)?SCORE_TYPE_WIN:SCORE_TYPE_LOSE;
					ScoreInfo[i].cbType=ScoreKind;
					ScoreInfo[i].lScore = GameEnd.lGameScore[i];
					ScoreInfo[i].lRevenue = lRevenue;
					
				}

				m_pITableFrame->WriteTableScore(ScoreInfo,CountArray(ScoreInfo));

				//�����Ϸ�÷�
				ZeroMemory(m_PlayerScore,sizeof(m_PlayerScore));
				CopyMemory(GameEnd.PlayerScore,m_PlayerScore,sizeof(m_PlayerScore));
			}

			//�ۼ�����
			CopyMemory(GameEnd.wPersistInfo,m_wPersistInfo,sizeof(m_wPersistInfo));

			if(bFinishDraw==true)
			{
				//������Ϣ
				CString szEndMessage;
				szEndMessage.Format(TEXT("���ֽ������ɼ�ͳ�ƣ�\n %s �� %I64d ��\n %s �� %I64d ��\n %s �� %I64d ��\n %s �� %I64d ��\n"),
					m_pITableFrame->GetTableUserItem(0)->GetNickName(),GameEnd.lGameScore[0],m_pITableFrame->GetTableUserItem(1)->GetNickName(),GameEnd.lGameScore[1],
					m_pITableFrame->GetTableUserItem(2)->GetNickName(),GameEnd.lGameScore[2],m_pITableFrame->GetTableUserItem(3)->GetNickName(),GameEnd.lGameScore[3]);

				//���ͽ�����Ϣ
				m_pITableFrame->SendGameMessage(szEndMessage,SMT_PROMPT);

			}

			//������Ϣ
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));

			//���ñ���
			m_wHeadUser=m_wWinOrder[0];

			//��������
			if (bFinishDraw==true) ZeroMemory(m_wPersistInfo,sizeof(m_wPersistInfo));

			//������Ϸ
			if (bFinishDraw==true) m_pITableFrame->ConcludeGame(GAME_SCENE_FREE);

			//����״̬
			if (bFinishDraw==false)
			{
				RepositionSink();
				m_pITableFrame->SetGameStatus(GAME_SCENE_WAIT);
			}

			return true;
		}
	case GER_DISMISS:		//��Ϸ��ɢ
		{
			//��������
			CMD_S_GameEnd GameEnd;
			ZeroMemory(&GameEnd,sizeof(GameEnd));

			//�û��˿�
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				GameEnd.cbCardCount[i]=m_cbHandCardCount[i];
				CopyMemory(&GameEnd.cbCardData[i],m_cbHandCardData[i],m_cbHandCardCount[i]*sizeof(BYTE));
			}

			//������Ϣ
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));

			//��Ϸ�ȼ�
			m_wOrderIndex=0;
			m_cbMainValue=0x02;
			m_cbValueOrder[0]=0x02;
			m_cbValueOrder[1]=0x02;

			//������Ϣ
			ZeroMemory(m_wPersistInfo,sizeof(m_wPersistInfo));

			//������Ϸ
			m_pITableFrame->ConcludeGame(GAME_SCENE_FREE);

			return true;
		}
	case GER_USER_LEAVE:		//�û�ǿ��
		{
			//�������
			CMD_S_GameEnd GameEnd;
			ZeroMemory(&GameEnd,sizeof(GameEnd));

			//��Ϸ�ȼ�
			m_wOrderIndex=0;
			m_cbMainValue=0x02;
			m_cbValueOrder[0]=0x02;
			m_cbValueOrder[1]=0x02;

			//������Ϣ
			ZeroMemory(m_wPersistInfo,sizeof(m_wPersistInfo));

			//�û��˿�
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				GameEnd.cbCardCount[i]=m_cbHandCardCount[i];
				CopyMemory(&GameEnd.cbCardData[i],m_cbHandCardData[i],m_cbHandCardCount[i]*sizeof(BYTE));
			}

			//��ҵ÷�
			GameEnd.lGameScore[wChairID]=-5*m_pGameServiceOption->lCellScore;

			//������Ϣ			
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));

			tagScoreInfo ScoreInfo[GAME_PLAYER];
			ZeroMemory(ScoreInfo,sizeof(ScoreInfo));

			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				if (i==wChairID)
				{
					ScoreInfo[i].lScore = GameEnd.lGameScore[wChairID];
					ScoreInfo[i].cbType = SCORE_TYPE_FLEE;
				}
				else
				{
					ScoreInfo[i].lScore = GameEnd.lGameScore[i];
					ScoreInfo[i].cbType = SCORE_TYPE_DRAW;
				}
			}
			//�޸Ļ���
			m_pITableFrame->WriteTableScore(ScoreInfo,CountArray(ScoreInfo));
			//֪ͨ��Ϣ
			CString szMessage;
			szMessage.Format(TEXT("���� [ %s ] �뿪��Ϸ����Ϸ����"),pIServerUserItem->GetNickName());
			m_pITableFrame->SendGameMessage(szMessage,SMT_PROMPT);
		

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
bool  CTableFrameSink::OnEventSendGameScene(WORD wChiarID, IServerUserItem * pIServerUserItem, BYTE cbGameStatus, bool bSendSecret)
{
	switch (cbGameStatus)
	{
	case GAME_SCENE_FREE:	//����״̬
		{
			//��������
			CMD_S_StatusFree StatusFree;
			ZeroMemory(&StatusFree,sizeof(StatusFree));

			//��Ϸ����
			StatusFree.lCellScore=m_pGameServiceOption->lCellScore;
			CopyMemory(StatusFree.bAutoStatus,m_bAutoPlayer,sizeof(StatusFree.bAutoStatus));

			//���ͳ���
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusFree,sizeof(StatusFree));
		}
	case GAME_SCENE_PLAY:	//��Ϸ״̬
		{
			//��������
			CMD_S_StatusPlay StatusPlay;
			ZeroMemory(&StatusPlay,sizeof(StatusPlay));

			//��Ϸ����
			StatusPlay.wCurrentUser=m_wCurrentUser;
			StatusPlay.lCellScore=m_pGameServiceOption->lCellScore;
			CopyMemory(StatusPlay.wPersistInfo,m_wPersistInfo,sizeof(m_wPersistInfo));
			CopyMemory(StatusPlay.bAutoStatus,m_bAutoPlayer,sizeof(StatusPlay.bAutoStatus));

			//�ȼ�����
			StatusPlay.cbMainValue=m_cbMainValue;
			CopyMemory(StatusPlay.cbValueOrder,m_cbValueOrder,sizeof(StatusPlay.cbValueOrder));

			//ʤ����Ϣ
			StatusPlay.wWinCount=m_wWinCount;
			CopyMemory(StatusPlay.wWinOrder,m_wWinOrder,sizeof(StatusPlay.wWinOrder));

			//������Ϣ
			StatusPlay.wTurnWiner=m_wTurnWiner;
			StatusPlay.cbTurnCardType=m_cbTurnCardType;
			StatusPlay.cbTurnCardCount=m_cbTurnCardCount;
			CopyMemory(StatusPlay.cbTurnCardData,m_cbTurnCardData,m_cbTurnCardCount*sizeof(BYTE));
			CopyMemory(StatusPlay.cbMagicCardData,m_cbMagicCardData,m_cbTurnCardCount*sizeof(BYTE));

			//�˿���Ϣ
			WORD wUserIndex=(m_cbHandCardCount[wChiarID]==0)?(wChiarID+2)%GAME_PLAYER:wChiarID;
			CopyMemory(StatusPlay.cbHandCardCount,m_cbHandCardCount,sizeof(m_cbHandCardCount));
			CopyMemory(StatusPlay.cbHandCardData,m_cbHandCardData[wUserIndex],sizeof(BYTE)*m_cbHandCardCount[wUserIndex]);

			CopyMemory(StatusPlay.b510KCardRecord,m_510KCardRecord,sizeof(m_510KCardRecord));

			//���ͳ���
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusPlay,sizeof(StatusPlay));
		}
	case GAME_SCENE_WAIT:	//�ȴ�״̬
		{
			//��������
			CMD_S_StatusWait StatusWait;
			ZeroMemory(&StatusWait,sizeof(StatusWait));

			//���ñ���
			if (pIServerUserItem->GetUserStatus()!=US_LOOKON)
			{
				m_cbContinue[wChiarID]=FALSE;
			}

			//��Ϸ����
			StatusWait.lCellScore=m_pGameServiceOption->lCellScore;
			CopyMemory(StatusWait.cbContinue,m_cbContinue,sizeof(m_cbContinue));
			CopyMemory(StatusWait.wPersistInfo,m_wPersistInfo,sizeof(m_wPersistInfo));
			CopyMemory(StatusWait.bAutoStatus,m_bAutoPlayer,sizeof(StatusWait.bAutoStatus));

			//�ȼ�����
			StatusWait.cbMainValue=m_cbMainValue;
			CopyMemory(StatusWait.cbValueOrder,m_cbValueOrder,sizeof(StatusWait.cbValueOrder));

			//���ͳ���
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusWait,sizeof(StatusWait));
		}
	}

	//�������
	ASSERT(FALSE);

	return false;
}

//��ʱ���¼�
bool CTableFrameSink::OnTimerMessage(DWORD dwTimerID, WPARAM dwBindParam)
{
	return false;
}

//�����¼�
bool CTableFrameSink::OnDataBaseMessage(WORD wRequestID, VOID * pData, WORD wDataSize)
{
	return false;
}

//�����¼�
bool CTableFrameSink::OnUserScroeNotify(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)
{
	return true;
}


//��Ϸ��Ϣ
bool CTableFrameSink::OnGameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	switch (wSubCmdID)
	{
	case SUB_C_AUTOMATISM:
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_C_Automatism));
			if (wDataSize!=sizeof(CMD_C_Automatism)) return false;

			CMD_C_Automatism * pAutomatism =(CMD_C_Automatism *)pData;

			m_bAutoPlayer[pIServerUserItem->GetChairID()]=pAutomatism->bAutomatism;

			CMD_S_UserAutomatism UserAutomatismData;
			UserAutomatismData.bTrusee=pAutomatism->bAutomatism;
			UserAutomatismData.wChairID=pIServerUserItem->GetChairID();
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_AUTOMATISM,&UserAutomatismData,sizeof(UserAutomatismData));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_AUTOMATISM,&UserAutomatismData,sizeof(UserAutomatismData));

			return true;
		}
	case SUB_C_OUT_CARD:			//�û�����
		{
			//��������
			CMD_C_OutCard * pOutCard=(CMD_C_OutCard *)pData;
			WORD wHeadSize=sizeof(CMD_C_OutCard)-sizeof(pOutCard->cbCardData);

			//Ч������
			ASSERT((wDataSize>=wHeadSize)&&(wDataSize==(wHeadSize+pOutCard->cbCardCount*sizeof(BYTE))));
			if ((wDataSize<wHeadSize)||(wDataSize!=(wHeadSize+pOutCard->cbCardCount*sizeof(BYTE)))) return false;

			//״̬Ч��
			ASSERT(m_pITableFrame->GetGameStatus()==GAME_SCENE_PLAY);
			if (m_pITableFrame->GetGameStatus()!=GAME_SCENE_PLAY) return true;

			//�û�Ч��
			ASSERT(pIServerUserItem->GetUserStatus()==US_PLAYING);
			if (pIServerUserItem->GetUserStatus()!=US_PLAYING) return true;

			//��Ϣ����
			WORD wChairID=pIServerUserItem->GetChairID();
			return OnUserOutCard(wChairID,pOutCard->cbCardData,pOutCard->cbCardCount);
		}
	case SUB_C_PASS_CARD:			//�û�����
		{
			//״̬Ч��
			ASSERT(m_pITableFrame->GetGameStatus()==GAME_SCENE_PLAY);
			if (m_pITableFrame->GetGameStatus()!=GAME_SCENE_PLAY) return true;

			//�û�Ч��
			ASSERT(pIServerUserItem->GetUserStatus()==US_PLAYING);
			if (pIServerUserItem->GetUserStatus()!=US_PLAYING) return true;

			//��Ϣ����
			return OnUserPassCard(pIServerUserItem->GetChairID());
		}
	case SUB_C_CONTINUE_GAME:		//������Ϸ
		{
			//״̬Ч��
			ASSERT(m_pITableFrame->GetGameStatus()==GAME_SCENE_WAIT);
			if (m_pITableFrame->GetGameStatus()!=GAME_SCENE_WAIT) return true;

			//�û�Ч��
			ASSERT(pIServerUserItem->GetUserStatus()==US_PLAYING);
			if (pIServerUserItem->GetUserStatus()!=US_PLAYING) return true;

			//��Ϣ����
			return OnUserContinueGame(pIServerUserItem->GetChairID());
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
bool CTableFrameSink::OnActionUserSitDown(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	//����ׯ��
	if (m_wHeadUser==INVALID_CHAIR) m_wHeadUser=pIServerUserItem->GetChairID();

	return true;
}

//�û�����
bool CTableFrameSink::OnActionUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	//��Ϸ��Ϣ
	if (bLookonUser==false)
	{
		//������ֵ
		m_wOrderIndex=0;
		m_cbMainValue=0x02;
		m_cbValueOrder[0]=0x02;
		m_cbValueOrder[1]=0x02;

		//��Ϸ����
		ZeroMemory(m_wPersistInfo,sizeof(m_wPersistInfo));
	}

	//ׯ������
	if ((bLookonUser==false)&&(m_wHeadUser==pIServerUserItem->GetChairID())) m_wHeadUser=INVALID_CHAIR;

	return true;
}

//�û�����
bool CTableFrameSink::OnUserOutCard(WORD wChairID, BYTE cbCardData[], BYTE cbCardCount)
{
	//Ч��״̬
	ASSERT(wChairID==m_wCurrentUser);
	if (wChairID!=m_wCurrentUser) return false;

	//����˿�
	BYTE cbMagicCardData[MAX_COUNT];
	m_GameLogic.MagicCardData(cbCardData,cbCardCount,cbMagicCardData,m_cbTurnCardType);

	//��ȡ����
	BYTE cbCardType=m_GameLogic.GetCardType(cbMagicCardData,cbCardCount);

	//�����ж�
	if (cbCardType==CT_ERROR) 
	{
		ASSERT(FALSE);
		return false;
	}

	//�����ж�
	if (m_cbTurnCardCount!=0)
	{
		//�Ա��˿�
		if (m_GameLogic.CompareCard(m_cbMagicCardData,cbMagicCardData,m_cbTurnCardCount,cbCardCount)==false)
		{
			ASSERT(FALSE);
			return false;
		}
	}

	//ɾ���˿�
	if (m_GameLogic.RemoveCard(cbCardData,cbCardCount,m_cbHandCardData[wChairID],m_cbHandCardCount[wChairID])==false)
	{
		ASSERT(FALSE);
		return false;
	}

	//���ñ���
	m_cbTurnCardType=cbCardType;
	m_cbTurnCardCount=cbCardCount;
	m_cbHandCardCount[wChairID]-=cbCardCount;
	CopyMemory(m_cbTurnCardData,cbCardData,sizeof(BYTE)*cbCardCount);
	CopyMemory(m_cbMagicCardData,cbMagicCardData,sizeof(BYTE)*cbCardCount);

	//���ַ���
	m_TurnScore+=m_GameLogic.GetCardScore(cbCardData,cbCardCount);

	//ʤ���ж�
	if (m_cbHandCardCount[wChairID]==0) 
	{
		//�����û�
		m_wWinOrder[m_wWinCount++]=wChairID;
		if(m_wWinCount==1)	m_bFirstUserLastTurn=true;

		//ȡ�öԼҷ���
		if(wChairID==m_wWinOrder[0])
		{
			m_PlayerScore[wChairID]+=m_PlayerScore[(wChairID+2)%GAME_PLAYER];
			m_PlayerScore[(wChairID+2)%GAME_PLAYER]=0;
		}

		//�û�ͳ��
		if (m_cbHandCardCount[(wChairID+2)%GAME_PLAYER]==0) 
		{
			//���һ���Ƶķ�
			if(m_TurnScore>0) 
			{
				m_PlayerScore[wChairID]+=m_TurnScore;
				m_TurnScore=0;
			}
			m_wCurrentUser=INVALID_CHAIR;
		}
	}

	//�л��û�
	if (m_wCurrentUser!=INVALID_CHAIR)
	{
		//�����û�
		m_wTurnWiner=wChairID;
		m_wCurrentUser=(wChairID+1)%GAME_PLAYER;

		//�û�����
		while (m_cbHandCardCount[m_wCurrentUser]==0) m_wCurrentUser=(m_wCurrentUser+1)%GAME_PLAYER;
	}

	//510K��¼
	for(BYTE i=0;i<cbCardCount;i++)
	{
		if(m_GameLogic.GetCardValue(cbCardData[i])==5)
		{
			for(BYTE j=0;j<8;j++)
			{
				if(m_510KCardRecord[0][j]==0)
				{
					m_510KCardRecord[0][j]=cbCardData[i];
					break;
				}
			}
		}
		if(m_GameLogic.GetCardValue(cbCardData[i])==10)
		{
			for(BYTE j=0;j<8;j++)
			{
				if(m_510KCardRecord[1][j]==0)
				{
					m_510KCardRecord[1][j]=cbCardData[i];
					break;
				}
			}
		}
		if(m_GameLogic.GetCardValue(cbCardData[i])==13)
		{
			for(BYTE j=0;j<8;j++)
			{
				if(m_510KCardRecord[2][j]==0)
				{
					m_510KCardRecord[2][j]=cbCardData[i];
					break;
				}
			}
		}
	}

	//��������
	CMD_S_OutCard OutCard;
	OutCard.wOutCardUser=wChairID;
	OutCard.cbCardCount=cbCardCount;
	OutCard.TurnScore=m_TurnScore;
	OutCard.wCurrentUser=m_wCurrentUser;
	CopyMemory(OutCard.b510KCardRecord,m_510KCardRecord,sizeof(m_510KCardRecord));
	CopyMemory(OutCard.cbCardData,m_cbTurnCardData,m_cbTurnCardCount*sizeof(BYTE));

	//��������
	WORD wHeadSize=sizeof(OutCard)-sizeof(OutCard.cbCardData);
	WORD wSendSize=wHeadSize+OutCard.cbCardCount*sizeof(BYTE);
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_OUT_CARD,&OutCard,wSendSize);
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_OUT_CARD,&OutCard,wSendSize);

	//���Ϣ
	if ((m_cbHandCardCount[wChairID]==0)&&(m_wCurrentUser!=INVALID_CHAIR))
	{
		//�������
		CMD_S_CardInfo CardInfo;
		ZeroMemory(&CardInfo,sizeof(CardInfo));

		//�����˿�
		CardInfo.cbCardCount=m_cbHandCardCount[(wChairID+2)%GAME_PLAYER];
		CopyMemory(CardInfo.cbCardData,m_cbHandCardData[(wChairID+2)%GAME_PLAYER],sizeof(BYTE)*CardInfo.cbCardCount);

		//��������
		WORD wHeadSize=sizeof(CardInfo)-sizeof(CardInfo.cbCardData);
		m_pITableFrame->SendTableData(wChairID,SUB_S_CARD_INFO,&CardInfo,wHeadSize+CardInfo.cbCardCount*sizeof(BYTE));
		m_pITableFrame->SendLookonData(wChairID,SUB_S_CARD_INFO,&CardInfo,wHeadSize+CardInfo.cbCardCount*sizeof(BYTE));
	}

	//�����ж�
	if (m_wCurrentUser==INVALID_CHAIR) OnEventGameConclude(wChairID,NULL,GER_NORMAL);

	return true;
}

//�û�����
bool CTableFrameSink::OnUserPassCard(WORD wChairID)
{
	//Ч��״̬
	ASSERT((wChairID==m_wCurrentUser)&&(m_cbTurnCardCount!=0));
	if ((wChairID!=m_wCurrentUser)||(m_cbTurnCardCount==0)) return false;

	//�û��л�
	do
	{
		//�����û�
		m_wCurrentUser=(m_wCurrentUser+1)%GAME_PLAYER;

		//����ж�
		if (m_wCurrentUser==m_wTurnWiner)
		{
			m_PlayerScore[m_wCurrentUser]+=m_TurnScore;
			m_TurnScore=0;

			//���ַ��ж�
			if( m_bFirstUserLastTurn==true && (m_wTurnWiner==(m_wWinOrder[0]+2)%GAME_PLAYER) )
			{
				m_PlayerScore[(m_wTurnWiner+2)%GAME_PLAYER]+=m_PlayerScore[m_wCurrentUser];
				m_PlayerScore[m_wCurrentUser]=0;
				m_bFirstUserLastTurn=false;
			}

			//���ñ���
			m_cbTurnCardCount=0;
			m_cbTurnCardType=CT_ERROR;
			m_bFirstUserLastTurn=false;

			//�ӷ紦��
			if (m_cbHandCardCount[m_wTurnWiner]==0)
			{
				m_wCurrentUser=(m_wTurnWiner+1)%GAME_PLAYER;
			}
		}

		//��������
		if (m_cbHandCardCount[m_wCurrentUser]!=0) break;

	} while(true);

	//������Ϣ
	CMD_S_PassCard PassCard;
	PassCard.TurnScore=m_TurnScore;
	CopyMemory(PassCard.PlayerScore,m_PlayerScore,sizeof(m_PlayerScore));
	PassCard.wPassCardUser=wChairID;
	PassCard.wCurrentUser=m_wCurrentUser;
	PassCard.cbTurnOver=(m_cbTurnCardCount==0)?TRUE:FALSE;

	//��������
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_PASS_CARD,&PassCard,sizeof(PassCard));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_PASS_CARD,&PassCard,sizeof(PassCard));

	return true;
}

//������Ϸ
bool CTableFrameSink::OnUserContinueGame(WORD wChairID)
{
	//Ч��״̬
	ASSERT(m_cbContinue[wChairID]==FALSE);
	if (m_cbContinue[wChairID]==TRUE) return true;

	//���ñ���
	m_cbContinue[wChairID]=TRUE;

	//�û�ͳ��
	WORD i=0;
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		if (m_cbContinue[i]==FALSE) break;
	}

	//��ʼ�ж�
	if (i<GAME_PLAYER)
	{
		//��������
		CMD_S_ContinueGame ContinueGame;
		ZeroMemory(&ContinueGame,sizeof(ContinueGame));

		//���ñ���
		ContinueGame.wChairID=wChairID;

		//������Ϣ
		m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_CONTINUE_GAME,&ContinueGame,sizeof(ContinueGame));
		m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_CONTINUE_GAME,&ContinueGame,sizeof(ContinueGame));
	}
	else
	{
		//��ʼ��Ϸ
		OnEventGameStart();
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////
//�Ƿ��з�
bool CTableFrameSink::IsAllUserHaveScore(BYTE bCardData[GAME_PLAYER][MAX_COUNT])
{
	for(BYTE i=0;i<GAME_PLAYER;i++)
	{
		if(m_GameLogic.GetCardScore(bCardData[i],MAX_COUNT-1)==0)	return false;
	}
	return true;
}

//���öԼ�
void CTableFrameSink::SearchFriend(BYTE bFirstCard,const BYTE bHandCardData[4][28])
{
	//��������
	WORD wFirstCardUser[2];
	ZeroMemory(wFirstCardUser,sizeof(wFirstCardUser));

	//Ѱ���û�
	BYTE bUserCount=0;
	for(WORD i=0;i<GAME_PLAYER;i++) for(BYTE j=0;j<MAX_COUNT-1;j++)
	{
		if(m_cbHandCardData[i][j]==bFirstCard)
		{
			wFirstCardUser[bUserCount]=i;
			bUserCount++;
		}
	}
	ASSERT(bUserCount==2);

	//�׳��û�
	m_wHeadUser=wFirstCardUser[0];

	//ͬһ������������ ������
	if(wFirstCardUser[1]==m_wHeadUser) return;

	//���Լ�
	WORD bFriendUser=wFirstCardUser[1];						//�ѷ�Ŀǰλ��
	WORD bFriendPosUser=(wFirstCardUser[0]+2)%GAME_PLAYER;	//�ѷ�Ӧ���ڵ�λ��

	//�ѷ�δ�ڶ���
	if(bFriendPosUser!=bFriendUser)
	{
		WORD OriginStation[2]={bFriendPosUser,bFriendUser};
		WORD TargetStation[2]={bFriendUser,bFriendPosUser};
		//m_pITableFrameControl->SwitchUserChair(OriginStation,TargetStation,2);	

		//�����ݽ���
		BYTE tempCard[28];
		ZeroMemory(tempCard,sizeof(tempCard));
		CopyMemory(tempCard,m_cbHandCardData[bFriendPosUser],sizeof(BYTE)*m_cbHandCardCount[bFriendPosUser]);
		CopyMemory(m_cbHandCardData[bFriendPosUser],m_cbHandCardData[bFriendUser],sizeof(BYTE)*m_cbHandCardCount[bFriendPosUser]);
		CopyMemory(m_cbHandCardData[bFriendUser],tempCard,sizeof(BYTE)*m_cbHandCardCount[bFriendUser]);

		//������Ϣ
		TCHAR szMessage[512]=TEXT("");
		_sntprintf(szMessage,CountArray(szMessage),TEXT("������ %s ��, [ %s ] �� [ %s ] ������λ��\n"),TranslateCardText(bFirstCard),m_pITableFrame->GetTableUserItem(bFriendPosUser)->GetNickName(),m_pITableFrame->GetTableUserItem(bFriendUser)->GetNickName());
		for (WORD i=0;i<GAME_PLAYER;i++)
		{
			IServerUserItem* pIServerUserItem=m_pITableFrame->GetTableUserItem(i);
			m_pITableFrame->SendGameMessage(pIServerUserItem,szMessage,SMT_PROMPT);
		}
	}

	return;
}

//ת������
CString CTableFrameSink::TranslateCardText(BYTE bCardData)
{
	//��������
	TCHAR szColor[16]=TEXT("");
	TCHAR szValue[16]=TEXT("");
	TCHAR szBuffer[32]=TEXT("");

	//��С������
	if(bCardData==0x4F) _sntprintf(szBuffer,CountArray(szBuffer),TEXT("����"));
	if(bCardData==0x4E) _sntprintf(szBuffer,CountArray(szBuffer),TEXT("С��"));
	if(bCardData==0x4F||bCardData==0x4E) return szBuffer;	

	//ȡ������
	BYTE bColor=m_GameLogic.GetCardColor(bCardData);
	BYTE bValue=m_GameLogic.GetCardValue(bCardData);

	//��ɫ��Ϣ
	switch(bColor)
	{
	case 0x00: _sntprintf(szColor,CountArray(szColor),TEXT("����"));break;
	case 0x10: _sntprintf(szColor,CountArray(szColor),TEXT("�ݻ�"));break;
	case 0x20: _sntprintf(szColor,CountArray(szColor),TEXT("����"));break;
	case 0x30: _sntprintf(szColor,CountArray(szColor),TEXT("����"));break;
	}

	//������Ϣ
	switch(bValue)
	{
	case 1:  _sntprintf(szValue,CountArray(szValue),TEXT(" A"));break;
	case 2:  _sntprintf(szValue,CountArray(szValue),TEXT(" 2"));break;
	case 3:  _sntprintf(szValue,CountArray(szValue),TEXT(" 3"));break;
	case 4:  _sntprintf(szValue,CountArray(szValue),TEXT(" 4"));break;
	case 5:  _sntprintf(szValue,CountArray(szValue),TEXT(" 5"));break;
	case 6:  _sntprintf(szValue,CountArray(szValue),TEXT(" 6"));break;
	case 7:  _sntprintf(szValue,CountArray(szValue),TEXT(" 7"));break;
	case 8:  _sntprintf(szValue,CountArray(szValue),TEXT(" 8"));break;
	case 9:  _sntprintf(szValue,CountArray(szValue),TEXT(" 9"));break;
	case 10: _sntprintf(szValue,CountArray(szValue),TEXT(" 10"));break;
	case 11: _sntprintf(szValue,CountArray(szValue),TEXT(" J"));break;
	case 12: _sntprintf(szValue,CountArray(szValue),TEXT(" Q"));break;
	case 13: _sntprintf(szValue,CountArray(szValue),TEXT(" K"));break;
	}

	//������Ϣ
	_sntprintf(szBuffer,CountArray(szBuffer),TEXT("%s%s"),szColor,szValue);
	return szBuffer;	
}


//////////////////////////////////////////////////////////////////////////////////
