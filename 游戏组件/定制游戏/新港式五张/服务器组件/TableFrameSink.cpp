#include "StdAfx.h"
#include "TableFrameSink.h"

//////////////////////////////////////////////////////////////////////////

//���Ʊ���
bool				CTableFrameSink::g_bControl=false;					//���ر�־
LONGLONG			CTableFrameSink::g_lMaxWinScore=5000000L;			//���Ӯ��
LONGLONG			CTableFrameSink::g_lMaxVarietycore=2000000L;		//���仯

//��������
CUserTracingMap		CTableFrameSink::g_UserTracingMap;					//�û���¼
CUserAddressMap		CTableFrameSink::g_UserAddressMap;					//���Ƶ�ַ
CBlackListIDMap		CTableFrameSink::g_BlackListIDMap;					//�����û�

LONGLONG			CTableFrameSink::g_lStockScore = 0l;
LONGLONG			CTableFrameSink::g_lRobotScoreMin = 0l;
LONGLONG			CTableFrameSink::g_lRobotScoreMax = 0l;
LONGLONG			CTableFrameSink::g_lRobotBankTake[2] = {0, 0};
LONGLONG			CTableFrameSink::g_lRobotBankSave = 0l;

//������ʶ
#define IDI_ADD_SCORE				1									//��ע��ʱ��
#define TIME_ADD_SCORE				1000								//������ʱ��

//////////////////////////////////////////////////////////////////////////

//���캯��
CTableFrameSink::CTableFrameSink()
{
	//��Ϸ����
	m_bShowHand=false;
	m_bChaosCard=false;
	m_wCurrentUser=INVALID_CHAIR;
	m_lServiceCharge = 0L;

	//��ע��Ϣ
	m_lDrawMaxScore=0L;
	m_lTurnMaxScore=0L;
	m_lTurnLessScore=0L;
	m_lDrawCellScore=0L;

	//�û�״̬
	ZeroMemory(m_cbPlayStatus,sizeof(m_cbPlayStatus));
	ZeroMemory(m_cbOperaScore,sizeof(m_cbOperaScore));
	ZeroMemory(m_bGiveUp,sizeof(m_bGiveUp));

	//�����Ϣ
	ZeroMemory(m_lUserScore,sizeof(m_lUserScore));
	ZeroMemory(m_lTableScore,sizeof(m_lTableScore));
	ZeroMemory(m_lUserMaxScore,sizeof(m_lUserMaxScore));

	//�˿˱���
	m_cbSendCardCount=0;
	ZeroMemory(m_cbCardCount,sizeof(m_cbCardCount));
	ZeroMemory(m_cbHandCardData,sizeof(m_cbHandCardData));

	//�������
	m_pITableFrame=NULL;
	m_pGameServiceOption=NULL;

	//�������
	m_bAllAndroid = false;
	m_hControlInst = NULL;
	m_pServerContro = NULL;
	m_hControlInst = LoadLibrary(TEXT("HKFiveCardNewServerControl.dll"));
	if ( m_hControlInst )
	{
		typedef void * (*CREATE)(); 
		CREATE ServerControl = (CREATE)GetProcAddress(m_hControlInst,"CreateServerControl"); 
		if ( ServerControl )
		{
			m_pServerContro = static_cast<IServerControl*>(ServerControl());
		}
	}

	return;
}

//��������
CTableFrameSink::~CTableFrameSink()
{
	if( m_pServerContro )
	{
		delete m_pServerContro;
		m_pServerContro = NULL;
	}

	if( m_hControlInst )
	{
		FreeLibrary(m_hControlInst);
		m_hControlInst = NULL;
	}
}

//�ͷŶ���
VOID CTableFrameSink::Release()
{
	if( m_pServerContro )
	{
		delete m_pServerContro;
		m_pServerContro = NULL;
	}

	if( m_hControlInst )
	{
		FreeLibrary(m_hControlInst);
		m_hControlInst = NULL;
	}

	delete this;
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
	if (m_pITableFrame==NULL) return false;

	//��ѯ����
	m_pGameServiceOption=m_pITableFrame->GetGameServiceOption();

	//��ʼģʽ
	m_pITableFrame->SetStartMode(START_MODE_ALL_READY);

	//�����ļ���
	TCHAR szPath[MAX_PATH]=TEXT("");
	TCHAR szConfigFileName[MAX_PATH]=TEXT("");
	GetCurrentDirectory(sizeof(szPath),szPath);
	_sntprintf(szConfigFileName, sizeof(szConfigFileName), TEXT("%s\\HKFiveCardNewConfig.ini"), szPath);

	//��������
	TCHAR OutBuf[255];
	ZeroMemory(OutBuf, sizeof(OutBuf));
	GetPrivateProfileString(m_pGameServiceOption->szServerName, TEXT("StorageStart"), TEXT("0"), OutBuf, CountArray(OutBuf),szConfigFileName);
	_sntscanf(OutBuf, CountArray(OutBuf), TEXT("%I64d"), &g_lStockScore);
	
	ZeroMemory(OutBuf, sizeof(OutBuf));
	GetPrivateProfileString(m_pGameServiceOption->szServerName, TEXT("StorageDeduct"), TEXT("0"), OutBuf, 255, szConfigFileName);
	_sntscanf(OutBuf, lstrlen(OutBuf), TEXT("%I64d"), &m_lStorageDeduct);

	ZeroMemory(OutBuf, sizeof(OutBuf));
	GetPrivateProfileString(m_pGameServiceOption->szServerName, TEXT("StorageMax"), TEXT("50000000"), OutBuf, 255, szConfigFileName);
	_sntscanf(OutBuf, lstrlen(OutBuf), TEXT("%I64d"), &m_lStorageMax);
	
	ZeroMemory(OutBuf, sizeof(OutBuf));
	GetPrivateProfileString(m_pGameServiceOption->szServerName, TEXT("StorageMul"), TEXT("50"), OutBuf, 255, szConfigFileName);
	_sntscanf(OutBuf, lstrlen(OutBuf), TEXT("%I64d"), &m_nStorageMul);

	ZeroMemory(OutBuf, sizeof(OutBuf));
	GetPrivateProfileString(m_pGameServiceOption->szServerName, TEXT("RobotScoreMin"), TEXT("5000"), OutBuf, CountArray(OutBuf),szConfigFileName);
	_sntscanf(OutBuf, CountArray(OutBuf), TEXT("%I64d"), &g_lRobotScoreMin);

	ZeroMemory(OutBuf, sizeof(OutBuf));
	GetPrivateProfileString(m_pGameServiceOption->szServerName, TEXT("RobotScoreMax"), TEXT("10000000"), OutBuf, CountArray(OutBuf),szConfigFileName);
	_sntscanf(OutBuf, CountArray(OutBuf), TEXT("%I64d"), &g_lRobotScoreMax);

	ZeroMemory(OutBuf, sizeof(OutBuf));
	GetPrivateProfileString(m_pGameServiceOption->szServerName, TEXT("RobotBankTakeMin"), TEXT("10000"), OutBuf, CountArray(OutBuf),szConfigFileName);
	_sntscanf(OutBuf, CountArray(OutBuf), TEXT("%I64d"), &g_lRobotBankTake[0]);

	ZeroMemory(OutBuf, sizeof(OutBuf));
	GetPrivateProfileString(m_pGameServiceOption->szServerName, TEXT("RobotBankTakeMax"), TEXT("1000000"), OutBuf, CountArray(OutBuf),szConfigFileName);
	_sntscanf(OutBuf, CountArray(OutBuf), TEXT("%I64d"), &g_lRobotBankTake[1]);

	ZeroMemory(OutBuf, sizeof(OutBuf));
	GetPrivateProfileString(m_pGameServiceOption->szServerName, TEXT("RobotBankSave"), TEXT("10"), OutBuf, CountArray(OutBuf),szConfigFileName);
	_sntscanf(OutBuf, CountArray(OutBuf), TEXT("%I64d"), &g_lRobotBankSave);

	if( g_lRobotScoreMax < g_lRobotScoreMin )
		g_lRobotScoreMax = g_lRobotScoreMin;

	if( g_lRobotBankTake[1] <= g_lRobotBankTake[0] )
		g_lRobotBankTake[1] = g_lRobotBankTake[0] + 1;

	if( g_lRobotBankSave > 100 )
		g_lRobotBankSave = 100;

	return true;
}

//��λ����
VOID CTableFrameSink::RepositionSink()
{
	//��Ϸ����
	m_bShowHand=false;
	m_bChaosCard=false;
	m_wCurrentUser=INVALID_CHAIR;

	//��ע��Ϣ
	m_lDrawMaxScore=0L;
	m_lTurnMaxScore=0L;
	m_lTurnLessScore=0L;
	m_lDrawCellScore=0L;

	//�û�״̬
	ZeroMemory(m_cbPlayStatus,sizeof(m_cbPlayStatus));
	ZeroMemory(m_cbOperaScore,sizeof(m_cbOperaScore));
	ZeroMemory(m_bGiveUp,sizeof(m_bGiveUp));

	//�����Ϣ
	ZeroMemory(m_lUserScore,sizeof(m_lUserScore));
	ZeroMemory(m_lTableScore,sizeof(m_lTableScore));
	ZeroMemory(m_lUserMaxScore,sizeof(m_lUserMaxScore));

	//�˿˱���
	m_cbSendCardCount=0;
	ZeroMemory(m_cbCardCount,sizeof(m_cbCardCount));
	ZeroMemory(m_cbHandCardData,sizeof(m_cbHandCardData));

	return;
}

//��Ϸ��ʼ
bool CTableFrameSink::OnEventGameStart()
{
	//��ǰ���
	SendStorage();

	//ɾ����ʱ��
	m_pITableFrame->KillGameTimer(IDI_ADD_SCORE);

	//����״̬
	m_pITableFrame->SetGameStatus(GAME_SCENE_PLAY);

	//�ɷ��˿�
	m_cbSendCardCount=2;
	m_GameLogic.RandCardList(m_cbHandCardData[0],sizeof(m_cbHandCardData)/sizeof(m_cbHandCardData[0][0]));

	//��ͻ���
	LONGLONG lUserLessScore=0L;
	int nPlayCount = 0;
	int nAndroidCount = 0;
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//��ȡ�û�
		IServerUserItem * pIServerUserItem=m_pITableFrame->GetTableUserItem(i);

		//���ñ���
		if (pIServerUserItem!=NULL)
		{
			if ((lUserLessScore <= 0L)||(pIServerUserItem->GetUserScore()<lUserLessScore))
			{
				lUserLessScore = pIServerUserItem->GetUserScore();
			}
			nPlayCount++;
			if (pIServerUserItem->IsAndroidUser())
			{
				nAndroidCount++;
			}
		}
	}

	//ȫ���ǻ�����
	m_bAllAndroid = (nAndroidCount != 0 && nAndroidCount == nPlayCount);


	//��ע����
	if (lUserLessScore > 100L * m_pGameServiceOption->lCellScore)
	{
		m_lDrawCellScore = lUserLessScore * 6.6 / 100L;
	}
	else
	{
		m_lDrawCellScore=m_pGameServiceOption->lCellScore;
	}

	//�����
	m_lServiceCharge = 0;

	//��������
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//��ȡ�û�
		IServerUserItem * pIServerUserItem=m_pITableFrame->GetTableUserItem(i);

		//���ñ���
		if (pIServerUserItem!=NULL)
		{
			//״̬����
			m_cbPlayStatus[i]=TRUE;
			m_lTableScore[i]=m_lDrawCellScore;
			m_lUserMaxScore[i] = (pIServerUserItem->GetUserScore() - m_lServiceCharge);

			//�˿�����
			m_cbCardCount[i]=m_cbSendCardCount;
		}
	}

	//��ע����
	RectifyMaxScore();

	//���ƴ���
	if (g_bControl==true) ControlUserCard();

	//���ñ���
	m_wCurrentUser=EstimateWinner(1,1);
	m_lTurnLessScore=m_lUserScore[m_wCurrentUser]+m_lTableScore[m_wCurrentUser];


	//��������
	CMD_S_GameStart GameStart;
	ZeroMemory(&GameStart,sizeof(GameStart));

	//���ñ���
	GameStart.lDrawMaxScore=m_lDrawMaxScore;
	GameStart.lTurnMaxScore=m_lTurnMaxScore;
	GameStart.lTurnLessScore=m_lTurnLessScore;
	GameStart.lCellScore=(LONG)m_lDrawCellScore;
	GameStart.lServiceCharge=(LONG)m_lServiceCharge;

	// �жϿ��
	if( g_lStockScore < m_lDrawMaxScore )
	{
		// ��ȡ�����λ��
		WORD wWinerUser = EstimateWinner(0, MAX_COUNT-1);

		// �ж�Ӯ�Ҳ��ǻ�����
		IServerUserItem * pIWinnerUserItem = m_pITableFrame->GetTableUserItem(wWinerUser);
		if( pIWinnerUserItem && !pIWinnerUserItem->IsAndroidUser() )
		{
			// ��ȡ���л�����λ��
			CWHArray<WORD>	ArrayRobot;
			for (WORD i = 0; i < GAME_PLAYER; i++)
			{
				IServerUserItem * pIServerUserItem = m_pITableFrame->GetTableUserItem(i);
				if( pIServerUserItem && pIServerUserItem->IsAndroidUser() )
				{
					// ��ӻ�����
					ArrayRobot.Add(i);
				}
			}

			if(ArrayRobot.GetCount() > 0)
			{
				// ����
				WORD wExchange = ArrayRobot[rand()%ArrayRobot.GetCount()];
				BYTE cbCardData[MAX_COUNT];
				CopyMemory(cbCardData, m_cbHandCardData[wExchange], sizeof(BYTE) * MAX_COUNT );
				CopyMemory(m_cbHandCardData[wExchange], m_cbHandCardData[wWinerUser], sizeof(BYTE) * MAX_COUNT );
				CopyMemory(m_cbHandCardData[wWinerUser], cbCardData, sizeof(BYTE) * MAX_COUNT );
			}
		}
	}
	else if(g_lStockScore > m_lStorageMax && rand()%100 < m_nStorageMul) //ϵͳ�ͷ�
	{
		// ��ȡ�����λ��
		WORD wWinerUser = EstimateWinner(0, MAX_COUNT-1);

		// �ж�Ӯ���ǻ�����
		IServerUserItem * pIWinnerUserItem = m_pITableFrame->GetTableUserItem(wWinerUser);

		if( pIWinnerUserItem && pIWinnerUserItem->IsAndroidUser())
		{
			// ��ȡ�����û�λ��
			CWHArray<WORD>	ArrayUser;
			for (WORD i = 0; i < GAME_PLAYER; i++)
			{
				IServerUserItem * pIServerUserItem = m_pITableFrame->GetTableUserItem(i);
				if(pIServerUserItem && !pIServerUserItem->IsAndroidUser())
				{
					// ����û�
					ArrayUser.Add(i);
				}
			}

			if(ArrayUser.GetCount() > 0)
			{
				//����
				WORD wExchange = ArrayUser[rand()%ArrayUser.GetCount()];
				BYTE cbCardData[MAX_COUNT];
				CopyMemory(cbCardData, m_cbHandCardData[wExchange], sizeof(BYTE) * MAX_COUNT );
				CopyMemory(m_cbHandCardData[wExchange], m_cbHandCardData[wWinerUser], sizeof(BYTE) * MAX_COUNT );
				CopyMemory(m_cbHandCardData[wWinerUser], cbCardData, sizeof(BYTE) * MAX_COUNT );
			}
		}
	}
	
	m_wCurrentUser=EstimateWinner(1,1);
	GameStart.wCurrentUser=m_wCurrentUser;

	//�����˿�
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		if (m_cbPlayStatus[i]==FALSE) GameStart.cbCardData[i]=0;
		else GameStart.cbCardData[i]=m_cbHandCardData[i][1];
	}

	//��������
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		// ����������
		IServerUserItem * pIServerUserItem=m_pITableFrame->GetTableUserItem(i);
		if ( pIServerUserItem != NULL)
		{
			if(pIServerUserItem->IsAndroidUser())
			{
				CopyMemory(GameStart.cbHandCardData, m_cbHandCardData, sizeof(GameStart.cbHandCardData));
			}
			else
			{
				ZeroMemory(GameStart.cbHandCardData, sizeof(GameStart.cbHandCardData));
				CopyMemory(GameStart.cbHandCardData[i], m_cbHandCardData[i], sizeof(GameStart.cbHandCardData[i]));
			}
		}

		//��Ϸ����
		if (m_cbPlayStatus[i]==TRUE)
		{
			GameStart.cbObscureCard=m_cbHandCardData[i][0];
			m_pITableFrame->SendTableData(i,SUB_S_GAME_START,&GameStart,sizeof(GameStart));
		}

		//�Թ�����
		GameStart.cbObscureCard=(m_cbPlayStatus[i]==TRUE)?0xFF:0x00;
		m_pITableFrame->SendLookonData(i,SUB_S_GAME_START,&GameStart,sizeof(GameStart));
	}
	
	//��������
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//��ȡ�û�
		IServerUserItem * pIServerUserItem=m_pITableFrame->GetTableUserItem(i);

		//���ñ���
		if (pIServerUserItem!=NULL)
		{
			m_strPalyName[i].Format(TEXT("%s"), pIServerUserItem->GetNickName() );
		}
	}

	if( m_pServerContro )
	{
		//������Ϣ
		BYTE cbHandCardData[GAME_PLAYER][5];
		CopyMemory(cbHandCardData, m_cbHandCardData, sizeof(cbHandCardData) );

		//�����˿�
		//for (WORD i= 0;i < GAME_PLAYER; i++)
		//	m_GameLogic.SortCardList(cbHandCardData[i], 5 );

		//��Ϣ����
		m_pServerContro->ServerControl(cbHandCardData, m_pITableFrame);
	}

	//������ʱ��
	m_pITableFrame->SetGameTimer(IDI_ADD_SCORE,TIME_ADD_SCORE*46,1,0);

	return true;
}

//��Ϸ����
bool CTableFrameSink::OnEventGameConclude(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)
{
	switch (cbReason)
	{
	case GER_NORMAL:		//�������
	case GER_NO_PLAYER:		//û�����
		{
			//�������
			CMD_S_GameEnd GameEnd;
			ZeroMemory(&GameEnd,sizeof(GameEnd));

			//������ע
			LONGLONG lDrawScore=0L;
			for (WORD i=0;i<CountArray(m_lTableScore);i++) lDrawScore+=m_lTableScore[i];

			//��������
			WORD wWinerUser=EstimateWinner(0,MAX_COUNT-1);

			//���ֱ���
			tagScoreInfo ScoreInfoArray[GAME_PLAYER];
			ZeroMemory(&ScoreInfoArray,sizeof(ScoreInfoArray));

			// ��ʱ�ر�
			bool bDelayOverGame=false;

			// ͳ�ƻ���
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				if ( m_cbPlayStatus[i] == TRUE ) 
				{
					//�ɼ�����
					LONGLONG lUserScore = (i == wWinerUser)?(lDrawScore - m_lTableScore[i]): -m_lTableScore[i];

					//���û���
					ScoreInfoArray[i].lScore = lUserScore;
					ScoreInfoArray[i].cbType = (ScoreInfoArray[i].lScore >= 0L) ? SCORE_TYPE_WIN : SCORE_TYPE_LOSE;
					//����˰��
					ScoreInfoArray[i].lRevenue=m_pITableFrame->CalculateRevenue(i,ScoreInfoArray[i].lScore);
					if (ScoreInfoArray[i].lRevenue>0L) ScoreInfoArray[i].lScore-=ScoreInfoArray[i].lRevenue;
					GameEnd.lGameScore[i] = ScoreInfoArray[i].lScore;

					//�˿���Ϣ
					GameEnd.cbCardData[i]=(cbReason!=GER_NO_PLAYER)?m_cbHandCardData[i][0]:0;

					//��Ϸ����
					IServerUserItem * pIServerUserItem = m_pITableFrame->GetTableUserItem(i);
					if ( g_bControl == true && pIServerUserItem) 
					{
						OnEventScoreAlter(pIServerUserItem->GetUserID(),ScoreInfoArray[i].lScore);
					}

					// �۳������
					if ( (ScoreInfoArray[i].lScore - m_lServiceCharge) < 0 
						 && -(ScoreInfoArray[i].lScore - m_lServiceCharge) > pIServerUserItem->GetUserScore() )
					{
						ScoreInfoArray[i].lScore = -pIServerUserItem->GetUserScore();
					}
					else
					{
						ScoreInfoArray[i].lScore -= m_lServiceCharge;
					}

				}
				else
				{
					//�����˿�
					GameEnd.cbCardData[i] = 0;

					//���óɼ�
					GameEnd.lGameScore[i] = -m_lTableScore[i];
				}

				//��ʷ����
				m_HistoryScore.OnEventUserScore(i,GameEnd.lGameScore[i]);
			}

			// ������Ϣ
			for ( int j = 0;j<GAME_PLAYER;j++)
			{
				IServerUserItem * pIServerUserItemSend = m_pITableFrame->GetTableUserItem(j);
				if (pIServerUserItemSend != NULL)
				{
					CString strOver(TEXT("���ֽ������ɼ�ͳ�ƣ�"));
					for ( int i = 0;i<GAME_PLAYER;i++)
					{						
						if ( GameEnd.lGameScore[i] != 0)
						{		
							CString str;	
							if (i == j)
							{
								int nStrLen = 0;
								for ( int j = 0 ; j < lstrlen(m_strPalyName[i]); ++j )
								{
									if( m_strPalyName[i].GetAt(j) >= 0xA0 )//ȫ�� ���� 
									{ 
										nStrLen += 12;
									} 
									else					//��� Ӣ�� 
									{ 
										nStrLen += 6;
									} 

									if ( nStrLen >= 60 )
									{
										for ( int k = j + 1; k < lstrlen(m_strPalyName[i]); ++k )
										{
											if ( k - j == 4 )
											{
												m_strPalyName[i].SetAt(k, '\0');
												break;
											}
											else
											{
												m_strPalyName[i].SetAt(k, '.');
											}
										}
										break;
									}
								}

								str.Format(TEXT("\n��%s����%I64d"), m_strPalyName[i], GameEnd.lGameScore[i]);
								strOver += str;
							}
							else
							{
								str.Format(TEXT("\n��%s����%I64d"), TEXT("��Ϸ���"), GameEnd.lGameScore[i]);
								strOver += str;
							}
						}						
					}
					if ( !strOver.IsEmpty() )
					{
						m_pITableFrame->SendGameMessage(pIServerUserItemSend, strOver, SMT_CHAT );
					}
				}
			}

			//д�����
			m_pITableFrame->WriteTableScore(ScoreInfoArray,CountArray(ScoreInfoArray));

			
			//��������
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
			
			//д����
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				//״̬�ж�
				if (m_cbPlayStatus[i]==FALSE) continue;

				//��������
				IServerUserItem * pIServerUserItem=m_pITableFrame->GetTableUserItem(i);
				if( pIServerUserItem->IsAndroidUser() && !m_bAllAndroid)
					g_lStockScore += ScoreInfoArray[i].lScore;
			}

			//���˥��
			if(g_lStockScore>0 && !m_bAllAndroid)
			{
				g_lStockScore=g_lStockScore-g_lStockScore*m_lStorageDeduct/1000;
			}

			m_pITableFrame->ConcludeGame(GAME_SCENE_FREE);

			// �������˷�������������Ϣ
			for( int nIndex = 0; nIndex < GAME_PLAYER; ++nIndex )
			{
				IServerUserItem* pAndroidUser = m_pITableFrame->GetTableUserItem(nIndex);
				if( pAndroidUser && pAndroidUser->IsAndroidUser() )
				{
					m_pITableFrame->SendUserItemData(pAndroidUser, SUB_S_TRUE_END);
				}
			}
		

			return true;
		}
	case GER_DISMISS:		//��Ϸ��ɢ
		{
			//��������
			CMD_S_GameEnd GameEnd;
			ZeroMemory(&GameEnd,sizeof(GameEnd));

			//������Ϣ
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));

			//������Ϸ
			m_pITableFrame->ConcludeGame(GAME_SCENE_FREE);

			// �������˷�������������Ϣ
			for( int nIndex = 0; nIndex < GAME_PLAYER; ++nIndex )
			{
				IServerUserItem* pAndroidUser = m_pITableFrame->GetTableUserItem(nIndex);
				if( pAndroidUser && pAndroidUser->IsAndroidUser() )
				{
					m_pITableFrame->SendUserItemData(pAndroidUser, SUB_S_TRUE_END);
				}
			}

			return true;
		}
	case GER_NETWORK_ERROR:		//�û�ǿ��
	case GER_USER_LEAVE:		
		{
			if ( wChairID < GAME_PLAYER  && m_cbPlayStatus[wChairID] == TRUE)
			{
				//��������
				OnUserGiveUp(wChairID);
			}
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
	case GAME_SCENE_FREE:	//����״̬
		{
			//��������
			CMD_S_StatusFree StatusFree;
			ZeroMemory(&StatusFree,sizeof(StatusFree));

			//���ñ���
			StatusFree.lCellScore=(LONG)m_lDrawCellScore;

			//��ʷ����
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				tagHistoryScore * pHistoryScore=m_HistoryScore.GetHistoryScore(i);
				StatusFree.lTurnScore[i]=pHistoryScore->lTurnScore;
				StatusFree.lCollectScore[i]=pHistoryScore->lCollectScore;
			}

			//������
			StatusFree.lRobotScoreMin = g_lRobotScoreMin;					// ���ֵ���ȡ��
			StatusFree.lRobotScoreMax = g_lRobotScoreMax;					// ���ָ��ڴ��
			StatusFree.lRobotBankTake[0] = g_lRobotBankTake[0];				// ȡ����
			StatusFree.lRobotBankTake[1] = g_lRobotBankTake[1];				// ȡ����
			StatusFree.lRobotBankSave = g_lRobotBankSave;					// �����

			//���ͳ���
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusFree,sizeof(StatusFree));
		}
	case GAME_SCENE_PLAY:	//��Ϸ״̬
		{
			//��������
			CMD_S_StatusPlay StatusPlay;
			ZeroMemory(&StatusPlay,sizeof(StatusPlay));

			//���ñ���
			StatusPlay.lCellScore=(LONG)m_lDrawCellScore;
			StatusPlay.lServiceCharge=(LONG)m_lServiceCharge;
			
			//��ע��Ϣ
			StatusPlay.lDrawMaxScore=m_lDrawMaxScore;
			StatusPlay.lTurnMaxScore=m_lTurnMaxScore;
			StatusPlay.lTurnLessScore=m_lTurnLessScore;
			CopyMemory(StatusPlay.lUserScore,m_lUserScore,sizeof(StatusPlay.lUserScore));
			CopyMemory(StatusPlay.lTableScore,m_lTableScore,sizeof(StatusPlay.lTableScore));

			//״̬��Ϣ
			StatusPlay.wCurrentUser=m_wCurrentUser;
			StatusPlay.cbShowHand=(m_bShowHand==true)?TRUE:FALSE;
			CopyMemory(StatusPlay.cbPlayStatus,m_cbPlayStatus,sizeof(StatusPlay.cbPlayStatus));

			//������
			StatusPlay.lRobotScoreMin = g_lRobotScoreMin;					// ���ֵ���ȡ��
			StatusPlay.lRobotScoreMax = g_lRobotScoreMax;					// ���ָ��ڴ��
			StatusPlay.lRobotBankTake[0] = g_lRobotBankTake[0];				// ȡ����
			StatusPlay.lRobotBankTake[1] = g_lRobotBankTake[1];				// ȡ����
			StatusPlay.lRobotBankSave = g_lRobotBankSave;					// �����

			//��ʷ����
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				tagHistoryScore * pHistoryScore=m_HistoryScore.GetHistoryScore(i);
				StatusPlay.lTurnScore[i]=pHistoryScore->lTurnScore;
				StatusPlay.lCollectScore[i]=pHistoryScore->lCollectScore;
			}

			//�����˿�
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				//������Ŀ
				StatusPlay.cbCardCount[i]=m_cbCardCount[i];

				//�����˿�
				if (m_cbPlayStatus[i]==TRUE)
				{
					if ((i==wChairID)&&(pIServerUserItem->GetUserStatus()==US_LOOKON)) 
					{
						StatusPlay.cbHandCardData[i][0]=0;
					}
					else if ((i==wChairID)&&(bSendSecret==true)&&(pIServerUserItem->GetUserStatus()!=US_LOOKON))
					{
						StatusPlay.cbHandCardData[i][0]=m_cbHandCardData[i][0];
					}
					CopyMemory(&StatusPlay.cbHandCardData[i][1],&m_cbHandCardData[i][1],(m_cbCardCount[i]-1)*sizeof(BYTE));
				}
			}

			//���ͳ���
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusPlay,sizeof(StatusPlay));
		}
	}

	//�������
	ASSERT(FALSE);

	return false;
}

//��ʱ���¼�
bool CTableFrameSink::OnTimerMessage(DWORD dwTimerID, WPARAM wBindParam)
{
	if(dwTimerID==IDI_ADD_SCORE)
	{
		m_pITableFrame->KillGameTimer(IDI_ADD_SCORE);
		if (m_wCurrentUser!=INVALID_CHAIR) return OnUserGiveUp(m_wCurrentUser);
	}

	return false;
}

//��Ϸ��Ϣ
bool CTableFrameSink::OnGameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	switch (wSubCmdID)
	{
	case SUB_C_GIVE_UP:			//�û�����
		{
			//״̬Ч��
			ASSERT(m_pITableFrame->GetGameStatus()==GAME_SCENE_PLAY);
			if (m_pITableFrame->GetGameStatus()!=GAME_SCENE_PLAY) return true;

			//�û�Ч��
			ASSERT(pIServerUserItem->GetUserStatus()==US_PLAYING);
			if (pIServerUserItem->GetUserStatus()!=US_PLAYING) return true;

			//��Ϣ����
			return OnUserGiveUp(pIServerUserItem->GetChairID());
		}
	case SUB_C_ADD_SCORE:		//�û���ע
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_C_AddScore));
			if (wDataSize!=sizeof(CMD_C_AddScore)) return false;

			//״̬Ч��
			ASSERT(m_pITableFrame->GetGameStatus()==GAME_SCENE_PLAY);
			if (m_pITableFrame->GetGameStatus()!=GAME_SCENE_PLAY) return true;

			//�û�Ч��
			ASSERT(pIServerUserItem->GetUserStatus()==US_PLAYING);
			if (pIServerUserItem->GetUserStatus()!=US_PLAYING) return true;

			//��������
			CMD_C_AddScore * pAddScore=(CMD_C_AddScore *)pData;

			//��Ϣ����
			WORD wChairID=pIServerUserItem->GetChairID();
			return OnUserAddScore(wChairID,pAddScore->lScore);
		}
	case SUB_C_GET_WINNER:			//��ȡʤ��
		{
			//״̬Ч��
			ASSERT(m_pITableFrame->GetGameStatus()==GAME_SCENE_PLAY);
			if (m_pITableFrame->GetGameStatus()!=GAME_SCENE_PLAY) return true;

			return OnUserGetWinner(pIServerUserItem);
		}
	case SUB_C_ADD_SCORE_TIME:		//������ʱ��
		{
// 			//�û�Ч��
// 			tagUserInfo * pUserData=pIServerUserItem->GetUserInfo();
// 			if (pUserData->cbUserStatus!=US_PLAYING) return true;
// 
// 			m_pITableFrame->SetGameTimer(IDI_ADD_SCORE,TIME_ADD_SCORE*36,1,0);
			return true;
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
	//��ʷ����
	if (bLookonUser==false) m_HistoryScore.OnEventUserEnter(pIServerUserItem->GetChairID());

	return true;
}

//�û�����
bool CTableFrameSink::OnActionUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	//��ʷ����
	if (bLookonUser==false) m_HistoryScore.OnEventUserLeave(pIServerUserItem->GetChairID());

	return true;
}

//�û�����
bool CTableFrameSink::OnUserGiveUp(WORD wChairID)
{
	ASSERT(m_cbPlayStatus[wChairID]==TRUE);
	if(m_cbPlayStatus[wChairID]==FALSE) return true;
	//���ñ���
	m_cbPlayStatus[wChairID]=FALSE;

	if (m_bGiveUp[wChairID])
	{
		return true;
	}
	// ��ȡ���
	IServerUserItem * pIServerUserItem = m_pITableFrame->GetTableUserItem(wChairID);

	// �۳�����
	LONGLONG lDeficiencyPoint = -(m_lUserScore[wChairID] + m_lTableScore[wChairID] + m_lServiceCharge);

	// �۳������
	if ( lDeficiencyPoint < 0 && -lDeficiencyPoint > pIServerUserItem->GetUserScore() )
	{
		lDeficiencyPoint = -pIServerUserItem->GetUserScore();
	}

	//д�����
	tagScoreInfo ScoreInfoArray[GAME_PLAYER];
	ZeroMemory(ScoreInfoArray,sizeof(ScoreInfoArray));
	ScoreInfoArray[wChairID].cbType=SCORE_TYPE_LOSE;
	ScoreInfoArray[wChairID].lScore=lDeficiencyPoint;
	m_pITableFrame->WriteTableScore(ScoreInfoArray,CountArray(ScoreInfoArray));

	//д����
	if( pIServerUserItem->IsAndroidUser() && !m_bAllAndroid)
		g_lStockScore += lDeficiencyPoint;

	m_bGiveUp[wChairID] = true;

	//��Ϸ����
	if (g_bControl==true)
	{
		OnEventScoreAlter(pIServerUserItem->GetUserID(),-(m_lUserScore[wChairID]+m_lTableScore[wChairID]));
	}

	//��������
	WORD wPlayerCount=0;
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		if (m_cbPlayStatus[i]==TRUE) wPlayerCount++;
	}

	//�����ж�
	if (wPlayerCount>=2)
	{
		//��ע����
		RectifyMaxScore();

		//�л��û�
		if (m_wCurrentUser==wChairID)
		{
			//�����û�
			m_wCurrentUser=INVALID_CHAIR;

			m_pITableFrame->KillGameTimer(IDI_ADD_SCORE);

			//�û�����
			for (WORD i=1;i<GAME_PLAYER;i++)
			{
				//��������
				WORD wCurrentUser=(wChairID+i)%GAME_PLAYER;
				LONGLONG lDrawAddScroe=m_lUserScore[wCurrentUser]+m_lTableScore[wCurrentUser];

				//״̬�ж�
				if (m_cbPlayStatus[wCurrentUser]==FALSE) continue;

				//�û��л�
				if ((m_cbOperaScore[wCurrentUser]==FALSE)||(lDrawAddScroe<m_lTurnLessScore))
				{
					m_wCurrentUser=wCurrentUser;
					m_pITableFrame->SetGameTimer(IDI_ADD_SCORE,TIME_ADD_SCORE*36,1,0);
					break;
				}
			}
		}
	}
	else 
	{
		//�㼯���
		for (WORD i=0;i<GAME_PLAYER;i++) 
		{
			m_lTableScore[i]+=m_lUserScore[i];
			m_lUserScore[i]=0L;
		}

		//�����û�
		m_wCurrentUser=INVALID_CHAIR;
	}

	//��������
	CMD_S_GiveUp GiveUp;
	ZeroMemory(&GiveUp,sizeof(GiveUp));

	//���ñ���
	GiveUp.wGiveUpUser=wChairID;
	GiveUp.wCurrentUser=m_wCurrentUser;
	GiveUp.lDrawMaxScore=m_lDrawMaxScore;
	GiveUp.lTrunMaxScore=m_lTurnMaxScore;
		
	//������Ϣ
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GIVE_UP,&GiveUp,sizeof(GiveUp));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GIVE_UP,&GiveUp,sizeof(GiveUp));

	//������Ϸ
	if (wPlayerCount<=1)
	{
		OnEventGameConclude(INVALID_CHAIR,NULL,GER_NO_PLAYER);
		return true;
	}

	//�����˿�
	if (m_wCurrentUser==INVALID_CHAIR) DispatchUserCard();

	return true;
}

//�û���ע
bool CTableFrameSink::OnUserAddScore(WORD wChairID, LONGLONG lScore)
{
	//״̬Ч��
	ASSERT(m_wCurrentUser==wChairID);
	if (m_wCurrentUser!=wChairID) return false;

	//��עЧ��
	ASSERT((lScore+m_lUserScore[wChairID]+m_lTableScore[wChairID])<=m_lTurnMaxScore);
	ASSERT((lScore>=0L)&&((lScore+m_lUserScore[wChairID]+m_lTableScore[wChairID])>=m_lTurnLessScore));

	//��עЧ��
	if ((lScore+m_lUserScore[wChairID]+m_lTableScore[wChairID])>m_lTurnMaxScore) return false;
	if ((lScore<0L)||((lScore+m_lUserScore[wChairID]+m_lTableScore[wChairID])<m_lTurnLessScore)) return false;

	//���ñ���
	m_cbOperaScore[wChairID]=TRUE;
	m_lUserScore[wChairID]+=lScore;
	m_lTurnLessScore=m_lUserScore[wChairID]+m_lTableScore[wChairID];

	//״̬����
	m_wCurrentUser=INVALID_CHAIR;
	m_bShowHand=(m_lTurnLessScore==m_lDrawMaxScore);

	//�û�����
	for (WORD i=1;i<GAME_PLAYER;i++)
	{
		//��������
		WORD wCurrentUser=(wChairID+i)%GAME_PLAYER;
		LONGLONG lDrawAddScroe=m_lUserScore[wCurrentUser]+m_lTableScore[wCurrentUser];

		//״̬�ж�
		if (m_cbPlayStatus[wCurrentUser]==FALSE) continue;

		//�û��л�
		if ((m_cbOperaScore[wCurrentUser]==FALSE)||(lDrawAddScroe<m_lTurnLessScore))
		{
			m_wCurrentUser=wCurrentUser;
			break;
		}
	}

	//��������
	CMD_S_AddScore AddScore;
	ZeroMemory(&AddScore,sizeof(AddScore));

	//���ñ���
	AddScore.wAddScoreUser=wChairID;
	AddScore.wCurrentUser=m_wCurrentUser;
	AddScore.lTurnLessScore=m_lTurnLessScore;
	AddScore.lUserScoreCount=m_lUserScore[wChairID];

	//��������
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_ADD_SCORE,&AddScore,sizeof(AddScore));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_ADD_SCORE,&AddScore,sizeof(AddScore));


	//�����˿�
	if (m_wCurrentUser==INVALID_CHAIR) 
	{
		DispatchUserCard();
	}
	else
	{
		//������ʱ��
		m_pITableFrame->SetGameTimer(IDI_ADD_SCORE,TIME_ADD_SCORE*36,1,0);
	}

	return true;
}

//������ע
VOID CTableFrameSink::RectifyMaxScore()
{
	//���ñ���
	m_lDrawMaxScore=0L;
	m_lTurnMaxScore=0L;

	//�����ע
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//�û��ж�
		if (m_cbPlayStatus[i]==FALSE) continue;

		//��������
		if ((m_lDrawMaxScore==0L)||(m_lUserMaxScore[i]<m_lDrawMaxScore)) m_lDrawMaxScore=m_lUserMaxScore[i];
	}

	//��ǰ��ע
	if (m_cbSendCardCount<=2) m_lTurnMaxScore=m_lDrawMaxScore/2L;
	else m_lTurnMaxScore=m_lDrawMaxScore;

	return;
}

//�����˿�
VOID CTableFrameSink::DispatchUserCard()
{
	//�㼯���
	for (WORD i=0;i<GAME_PLAYER;i++) 
	{
		m_lTableScore[i]+=m_lUserScore[i];
		m_lUserScore[i]=0L;
	}

	//�����ж�
	if (m_cbSendCardCount==MAX_COUNT)
	{
		OnEventGameConclude(INVALID_CHAIR,NULL,GER_NORMAL);
		return;
	}

	//�����˿�
	if (m_bChaosCard==true)
	{
		//�����˿�
		WORD wSwitchUser=rand()%GAME_PLAYER;
		BYTE cbSwitchCard=m_cbHandCardData[wSwitchUser][m_cbSendCardCount];

		//�����˿�
		for (WORD i=0;i<GAME_PLAYER-1;i++)
		{
			WORD wUserHead=(wSwitchUser+i)%GAME_PLAYER;
			WORD wUserNext=(wSwitchUser+i+1)%GAME_PLAYER;
			m_cbHandCardData[wUserHead][m_cbSendCardCount]=m_cbHandCardData[wUserNext][m_cbSendCardCount];
		}

		//�����˿�
		WORD wUserLeave=(wSwitchUser+GAME_PLAYER-1)%GAME_PLAYER;
		m_cbHandCardData[wUserLeave][m_cbSendCardCount]=cbSwitchCard;
	}

	//�ɷ��˿�
	BYTE cbSourceCount=m_cbSendCardCount;
	m_cbSendCardCount=(m_bShowHand==false)?(m_cbSendCardCount+1):MAX_COUNT;

	//��ǰ�û�
	if (m_bShowHand==false)
	{
		//״̬����
		ZeroMemory(m_cbOperaScore,sizeof(m_cbOperaScore));

		//�����û�
		m_wCurrentUser=EstimateWinner(1,m_cbSendCardCount-1);

		//��ע����
		m_lTurnMaxScore=(m_cbSendCardCount>=3)?m_lDrawMaxScore:m_lDrawMaxScore/2L;
		m_lTurnLessScore=m_lUserScore[m_wCurrentUser]+m_lTableScore[m_wCurrentUser];
	}
	else
	{
		//���ñ���
		m_wCurrentUser=INVALID_CHAIR;
		m_lTurnMaxScore=m_lDrawMaxScore;
		m_lTurnLessScore=m_lDrawMaxScore;
	}

	//��������
	CMD_S_SendCard SendCard;
	ZeroMemory(&SendCard,sizeof(SendCard));

	//���ñ���
	SendCard.wCurrentUser=m_wCurrentUser;
	SendCard.lTurnMaxScore=m_lTurnMaxScore;
	SendCard.wStartChairID=EstimateWinner(1,cbSourceCount-1);
	SendCard.cbSendCardCount=m_cbSendCardCount-cbSourceCount;

	//�����˿�
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//״̬�ж�
		if (m_cbPlayStatus[i]==FALSE) continue;

		//������Ŀ
		m_cbCardCount[i]=m_cbSendCardCount;

		//�ɷ��˿�
		for (BYTE j=0;j<(m_cbSendCardCount-cbSourceCount);j++)
		{
			ASSERT(j<CountArray(SendCard.cbCardData[i]));
			SendCard.cbCardData[j][i]=m_cbHandCardData[i][cbSourceCount+j];
		}
	}

	//��������
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_SEND_CARD,&SendCard,sizeof(SendCard));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_SEND_CARD,&SendCard,sizeof(SendCard));

	//��������
	if (m_wCurrentUser==INVALID_CHAIR) 
	{
		OnEventGameConclude(INVALID_CHAIR,NULL,GER_NORMAL);
	}
	else
	{
		//������ʱ��
		m_pITableFrame->SetGameTimer(IDI_ADD_SCORE,TIME_ADD_SCORE*46,1,0);
	}

	return;
}

//�����˿�
VOID CTableFrameSink::ControlUserCard()
{
	//�û�����
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//״̬�ж�
		if (m_cbPlayStatus[i]==FALSE) continue;

		//��������
		bool bSwitchCard=false;
		IServerUserItem * pIServerUserItem=m_pITableFrame->GetTableUserItem(i);

		//��ѯ��¼
		tagUserTracing * pUserTracing=NULL;
		g_UserTracingMap.Lookup(pIServerUserItem->GetUserID(),pUserTracing);

		//��������
		if (pUserTracing!=NULL)
		{
			if ((g_lMaxWinScore!=0L)&&(pUserTracing->lWinScore>g_lMaxWinScore)) bSwitchCard=true;
			if ((g_lMaxVarietycore!=0L)&&(pUserTracing->lVarietyScore>g_lMaxVarietycore)) bSwitchCard=true;
		}

		//�û�����
		if (bSwitchCard==false)
		{
			DWORD dwUserID=0L;
			if ((g_BlackListIDMap.Lookup(pIServerUserItem->GetUserID(),dwUserID)==TRUE)&&(dwUserID!=0L)) bSwitchCard=true;
		}

		//��ַ����
		if (bSwitchCard==false)
		{
			DWORD dwUserAddress=0L;
			if ((g_UserAddressMap.Lookup(pIServerUserItem->GetClientAddr(),dwUserAddress)==TRUE)&&(dwUserAddress!=0)) bSwitchCard=true;
		}

		//�����˿�
		if (bSwitchCard==true)
		{
			//��ȡ����
			WORD wLoseIndex=EstimateLoser(0,MAX_COUNT-1);

			//�����˿�
			if (wLoseIndex!=i)
			{
				BYTE cbTempCardData[MAX_COUNT];
				CopyMemory(cbTempCardData,m_cbHandCardData[i],sizeof(cbTempCardData));
				CopyMemory(m_cbHandCardData[i],m_cbHandCardData[wLoseIndex],sizeof(cbTempCardData));
				CopyMemory(m_cbHandCardData[wLoseIndex],cbTempCardData,sizeof(cbTempCardData));
			}

			//��ֹ����
			m_bChaosCard=false;

			break;
		}
	}

	return;
}

//�����¼�
VOID CTableFrameSink::OnEventScoreAlter(DWORD dwUserID, LONGLONG lVarietyScore)
{
	//��ȡʱ��
	SYSTEMTIME SystemTime;
	GetLocalTime(&SystemTime);

	//�����û�
	tagUserTracing * pUserTracing=NULL;
	g_UserTracingMap.Lookup(dwUserID,pUserTracing);

	//�����û�
	if (pUserTracing==NULL)
	{
		//��������
		pUserTracing=new tagUserTracing;
		g_UserTracingMap[dwUserID]=pUserTracing;

		//���ñ���
		pUserTracing->lWinScore=0L;
		pUserTracing->lVarietyScore=0L;
		pUserTracing->SystemTimeTrace=SystemTime;
	}

	//��ԭ�ж�
	if (pUserTracing->SystemTimeTrace.wDay!=SystemTime.wDay)
	{
		pUserTracing->lWinScore=0L;
		pUserTracing->lVarietyScore=0L;
		pUserTracing->SystemTimeTrace=SystemTime;
	}

	//���ñ���
	pUserTracing->lVarietyScore+=lVarietyScore;
	if (lVarietyScore>0L) pUserTracing->lWinScore+=lVarietyScore;

	return;
}

//�ƶ�����
WORD CTableFrameSink::EstimateLoser(BYTE cbStartPos, BYTE cbConcludePos)
{
	//�����˿�
	BYTE cbUserCardData[GAME_PLAYER][MAX_COUNT];
	CopyMemory(cbUserCardData,m_cbHandCardData,sizeof(cbUserCardData));

	//Ѱ�����
	for (WORD wLoser=0;wLoser<GAME_PLAYER;wLoser++)
	{
		if (m_cbPlayStatus[wLoser]==TRUE) 
		{
			m_GameLogic.SortCardList(cbUserCardData[wLoser]+cbStartPos,cbConcludePos-cbStartPos+1);
			break;
		}
	}

	//�Ա����
	for (WORD i=(wLoser+1);i<GAME_PLAYER;i++)
	{
		//�û�����
		if (m_cbPlayStatus[i]==FALSE) continue;

		//�����˿�
		m_GameLogic.SortCardList(cbUserCardData[i]+cbStartPos,cbConcludePos-cbStartPos+1);

		//�Ա��˿�
		if (m_GameLogic.CompareCard(cbUserCardData[i]+cbStartPos,cbUserCardData[wLoser]+cbStartPos,cbConcludePos-cbStartPos+1)==false) 
		{
			wLoser=i;
		}
	}

	return wLoser;
}

//�ƶ�ʤ��
WORD CTableFrameSink::EstimateWinner(BYTE cbStartPos, BYTE cbConcludePos)
{
	//�����˿�
	BYTE cbUserCardData[GAME_PLAYER][MAX_COUNT];
	CopyMemory(cbUserCardData,m_cbHandCardData,sizeof(cbUserCardData));

	//Ѱ�����
	for (WORD wWinner=0;wWinner<GAME_PLAYER;wWinner++)
	{
		if (m_cbPlayStatus[wWinner]==TRUE) 
		{
			m_GameLogic.SortCardList(cbUserCardData[wWinner]+cbStartPos,cbConcludePos-cbStartPos+1);
			break;
		}
	}

	//�Ա����
	for (WORD i=(wWinner+1);i<GAME_PLAYER;i++)
	{
		//�û�����
		if (m_cbPlayStatus[i]==FALSE) continue;

		//�����˿�
		m_GameLogic.SortCardList(cbUserCardData[i]+cbStartPos,cbConcludePos-cbStartPos+1);

		//�Ա��˿�
		if (m_GameLogic.CompareCard(cbUserCardData[i]+cbStartPos,cbUserCardData[wWinner]+cbStartPos,cbConcludePos-cbStartPos+1)==true) 
		{
			wWinner=i;
		}
	}

	return wWinner;
}

//��ȡ��Ϣ
bool CTableFrameSink::OnUserGetWinner( IServerUserItem * pIServerUserItem )
{
	return true;
	////Ч��Ȩ��
	//ASSERT( CUserRight::CanCheat(pIServerUserItem->GetUserRight()) );
	//if( !CUserRight::CanCheat(pIServerUserItem->GetUserRight()) ) return false;
	//if( pIServerUserItem->GetUserStatus() == US_LOOKON ) return true;

	////�����˿�
	//BYTE cbUserCardData[GAME_PLAYER][MAX_COUNT];
	//CopyMemory(cbUserCardData,m_cbHandCardData,sizeof(cbUserCardData));

	////��������
	//BYTE cbPlayStatus[GAME_PLAYER] = {0};
	//CopyMemory( cbPlayStatus,m_cbPlayStatus,sizeof(cbPlayStatus) );

	//CMD_S_GetWinner GetWinner = {0};
	//
	////�����˿�
	//for( WORD i = 0; i < GAME_PLAYER; i++ )
	//{
	//	if( cbPlayStatus[i] )
	//	{
	//		m_GameLogic.SortCardList(cbUserCardData[i],MAX_COUNT);
	//	}
	//}

	////Ѱ�����
	//do
	//{
	//	for( WORD i = 0; i < GAME_PLAYER; i++ )
	//	{
	//		if( cbPlayStatus[i] ) break;
	//	}
	//	if( i == GAME_PLAYER ) break;

	//	WORD wWinner = i;

	//	//�Ա����
	//	for (WORD j=0;j<GAME_PLAYER;j++)
	//	{
	//		//�û�����
	//		if (cbPlayStatus[j]==FALSE||j==wWinner) continue;

	//		//�Ա��˿�
	//		if (m_GameLogic.CompareCard(cbUserCardData[j],cbUserCardData[wWinner],MAX_COUNT)==true) 
	//		{
	//			wWinner=j;
	//		}
	//	}

	//	GetWinner.wChairOrder[GetWinner.wOrderCount++] = wWinner;

	//	cbPlayStatus[wWinner] = FALSE;
	//}while(true);

	////������Ϣ
	//WORD wSendSize = sizeof(GetWinner)-sizeof(GetWinner.wChairOrder)+sizeof(WORD)*GetWinner.wOrderCount;
	//m_pITableFrame->SendUserData(pIServerUserItem,SUB_S_GET_WINNER,&GetWinner,wSendSize);

	//return true;
}

//���Ϳ��
void CTableFrameSink::SendStorage()
{
	//֪ͨ��Ϣ
	TCHAR szMessage[128]=TEXT("");
	_sntprintf(szMessage,CountArray(szMessage),TEXT("��ǰ��棺%I64d"), g_lStockScore);

	//�û�״̬
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//��ȡ�û�
		IServerUserItem *pIServerUser=m_pITableFrame->GetTableUserItem(i);
		if(pIServerUser != NULL)
		{
			if (CUserRight::IsGameCheatUser(pIServerUser->GetUserRight()))
			{				
				m_pITableFrame->SendGameMessage(pIServerUser,szMessage,SMT_CHAT);
			}
		}
	}

	int nLookonCount = 0;
	IServerUserItem* pLookonUserItem = m_pITableFrame->EnumLookonUserItem(nLookonCount);
	while( pLookonUserItem )
	{
		if (CUserRight::IsGameCheatUser(pLookonUserItem->GetUserRight()))
		{				
			m_pITableFrame->SendGameMessage(pLookonUserItem,szMessage,SMT_CHAT);
		}

		nLookonCount++;
		pLookonUserItem = m_pITableFrame->EnumLookonUserItem(nLookonCount);
	}
}
//////////////////////////////////////////////////////////////////////////
