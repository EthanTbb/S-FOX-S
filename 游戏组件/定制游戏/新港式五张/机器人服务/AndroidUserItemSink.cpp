#include "Stdafx.h"
#include "AndroidUserItemSink.h"
#include <math.h>

//////////////////////////////////////////////////////////////////////////

//��Ϸʱ��
#define TIME_USER_START_GAME		3									//��ʼ��ʱ��
#define TIME_USER_ADD_SCORE			1									//��ע��ʱ��

//��Ϸʱ��
#define IDI_START_GAME				(103)								//��ʼʱ��
#define IDI_USER_ADD_SCORE			(104)								//��עʱ��
#define IDI_BANK_OPERATION			(105)								//���в���

//////////////////////////////////////////////////////////////////////////

//���캯��
CAndroidUserItemSink::CAndroidUserItemSink()
{
	//�ӿڱ���
	m_pIAndroidUserItem=NULL; 
	
	return;
}

//��������
CAndroidUserItemSink::~CAndroidUserItemSink()
{
}

//�ӿڲ�ѯ
void * CAndroidUserItemSink::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IAndroidUserItemSink,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IAndroidUserItemSink,Guid,dwQueryVer);
	return NULL;
}

//��ʼ�ӿ�
bool CAndroidUserItemSink::Initialization(IUnknownEx * pIUnknownEx)
{
	//��ѯ�ӿ�
	m_pIAndroidUserItem=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,IAndroidUserItem);
	if (m_pIAndroidUserItem==NULL) return false;

	return true;
}

//���ýӿ�
bool CAndroidUserItemSink::RepositionSink()
{


	return true;
}

//ʱ����Ϣ
bool CAndroidUserItemSink::OnEventTimer(UINT nTimerID)
{
	switch (nTimerID)
	{
	case IDI_BANK_OPERATION:	//��������
		{
			// ȡǮ
			if( m_pIAndroidUserItem->GetMeUserItem()->GetUserScore() < m_lRobotScoreMin )
			{
				int nRandCount = (int)(m_lRobotBankTake[1] - m_lRobotBankTake[0]) / RAND_MAX;
				if( nRandCount == 0 ) nRandCount = 1;

				m_pIAndroidUserItem->PerformTakeScore( ((rand() * nRandCount) % (m_lRobotBankTake[1] - m_lRobotBankTake[0])) + m_lRobotBankTake[0] );
			}
			// ��Ǯ
			else if	( m_pIAndroidUserItem->GetMeUserItem()->GetUserScore() > m_lRobotScoreMax )
			{
				m_pIAndroidUserItem->PerformSaveScore( m_pIAndroidUserItem->GetMeUserItem()->GetUserScore() * m_lRobotBankSave / 100 );
			}

			// ׼��
			m_pIAndroidUserItem->SetGameTimer(IDI_START_GAME, (rand()%3) + 2);

			return true;
		}
	case IDI_START_GAME:		//��ʼ��ʱ��
		{			
			// ����׼����Ϣ
			m_pIAndroidUserItem->SendUserReady( NULL, 0 );

			return true;
		}	
	case IDI_USER_ADD_SCORE:	//��ע��ʱ
		{
			// ��������λ��
			WORD wChairID = m_pIAndroidUserItem->GetChairID();

			// �������
			WORD wPlayCount = 0;
			for( int nPlayIndex = 0; nPlayIndex < GAME_PLAYER; ++nPlayIndex )
			{
				if( m_cbPlayStatus[nPlayIndex] )
					wPlayCount++;
			}

			// ����
			WORD wPlayMingRank[GAME_PLAYER];
			WORD wPlayShouRank[GAME_PLAYER];
			WORD wPlayFullRank[GAME_PLAYER];
			ZeroMemory(wPlayMingRank, sizeof(wPlayMingRank));
			ZeroMemory(wPlayShouRank, sizeof(wPlayShouRank));
			ZeroMemory(wPlayFullRank, sizeof(wPlayFullRank));

			// ����
			BYTE cbMingGenre[GAME_PLAYER] = { CT_SINGLE, CT_SINGLE, CT_SINGLE, CT_SINGLE, CT_SINGLE };
			BYTE cbShouGenre[GAME_PLAYER] = { CT_SINGLE, CT_SINGLE, CT_SINGLE, CT_SINGLE, CT_SINGLE };
			BYTE cbFullGenre[GAME_PLAYER] = { CT_SINGLE, CT_SINGLE, CT_SINGLE, CT_SINGLE, CT_SINGLE };

			// ---------------------------------------------------------------
			// ��ȡ����
			BYTE cbMingCardData[GAME_PLAYER][MAX_COUNT];
			ZeroMemory( cbMingCardData, sizeof(cbMingCardData) );
			for( int nPlayIndex = 0; nPlayIndex < GAME_PLAYER; ++nPlayIndex )
			{
				CopyMemory( cbMingCardData[nPlayIndex], &m_cbHandCardData[nPlayIndex][1], sizeof(BYTE)*m_cbCurRound );
				m_GameLogic.SortCardList( cbMingCardData[nPlayIndex], m_cbCurRound );
				cbMingGenre[nPlayIndex] = m_GameLogic.GetCardGenre(cbMingCardData[nPlayIndex], m_cbCurRound);
			}

			// ������������
			CalculateRank(wPlayMingRank, cbMingCardData, m_cbCurRound );
			
			// ---------------------------------------------------------------
			// ��ȡ��������
			BYTE cbShouCardData[GAME_PLAYER][MAX_COUNT];
			ZeroMemory( cbShouCardData, sizeof(cbMingCardData) );
			for( int nPlayIndex = 0; nPlayIndex < GAME_PLAYER; ++nPlayIndex )
			{
				CopyMemory( cbShouCardData[nPlayIndex], &m_cbHandCardData[nPlayIndex][0], sizeof(BYTE)*(m_cbCurRound + 1) );
				m_GameLogic.SortCardList( cbShouCardData[nPlayIndex], m_cbCurRound + 1 );
				cbShouGenre[nPlayIndex] = m_GameLogic.GetCardGenre(cbShouCardData[nPlayIndex], m_cbCurRound + 1);
			}

			// ������������
			CalculateRank(wPlayShouRank, cbShouCardData, m_cbCurRound + 1 );
			
			// ---------------------------------------------------------------
			// ��������������
			BYTE cbFullCardData[GAME_PLAYER][MAX_COUNT];
			ZeroMemory( cbFullCardData, sizeof(cbMingCardData) );
			for( int nPlayIndex = 0; nPlayIndex < GAME_PLAYER; ++nPlayIndex )
			{
				CopyMemory( cbFullCardData[nPlayIndex], &m_cbHandCardData[nPlayIndex][0], sizeof(BYTE)*(MAX_COUNT) );
				m_GameLogic.SortCardList( cbFullCardData[nPlayIndex], MAX_COUNT );
				cbFullGenre[nPlayIndex] = m_GameLogic.GetCardGenre(cbFullCardData[nPlayIndex], MAX_COUNT);
			}

			// ��������������
			CalculateRank(wPlayFullRank, cbFullCardData, MAX_COUNT );
			
			// �жϷ���
			do
			{
				// ������һ ֱ����ע
				if( wPlayMingRank[wChairID] == 1 || wPlayShouRank[wChairID] == 1 || wPlayFullRank[wChairID] == 1 )
					break;

				// ---------------------------------------------------------------
				// �����������
				float fGiveUp = 0.f;

				// ���������޸�
				fGiveUp += wPlayMingRank[wChairID] * 1.f;

				// ���������޸�
				fGiveUp += wPlayShouRank[wChairID] * 3.f;

				// ���˵����ƺ��Լ����������жϴ�С
				for( int nPlayIndex = 0; nPlayIndex < GAME_PLAYER; ++nPlayIndex )
				{
					if( nPlayIndex != wChairID && cbMingGenre[nPlayIndex] > cbFullGenre[wChairID] )
					{
						fGiveUp += 1000.f;
						break;
					}
				}

				// ���������������޸�
				switch( cbShouGenre[wChairID] )
				{
				case CT_SINGLE:
					fGiveUp += 20.f;
					break;
				case CT_ONE_DOUBLE:
					fGiveUp += 10.f;
					break;
				case CT_TWO_DOUBLE:
					fGiveUp += 0.f;
					break;
				case CT_THREE_TIAO:
					fGiveUp -= 5.f;
					break;
				case CT_SHUN_ZI:
					fGiveUp -= 10.f;
					break;
				case CT_TONG_HUA:
					fGiveUp -= 20.f;
					break;
				case CT_HU_LU:
					fGiveUp -= 30.f;
					break;
				case CT_TIE_ZHI:
					fGiveUp -= 100.f;
					break;
				case CT_TONG_HUA_SHUN:
					fGiveUp -= 200.f;
					break;
				}
				
				// ��ȡ���������ע
				LONGLONG lTableMax = 0;
				for( int nPlayIndex = 0; nPlayIndex < GAME_PLAYER; ++nPlayIndex )
				{
					lTableMax = max(m_lTableScore[nPlayIndex] + m_lUserScore[nPlayIndex], 0);
				}

				// ������ע�޸�
				fGiveUp += (sin((float)(m_lDrawMaxScore - lTableMax) / (float)m_lDrawMaxScore * 3.14f) * 60.f / 100.f);
				
				// ��������
				int nGiveUp = (int)fGiveUp;
				if( nGiveUp > 100 ) nGiveUp = 100;
				else if( nGiveUp < 0 ) nGiveUp = 0;

				// �жϷ���
				if( m_GameLogic.RandomArea( 2, nGiveUp, 100 - nGiveUp ) == 0 )
				{
					m_pIAndroidUserItem->SendSocketData(SUB_C_GIVE_UP, NULL, NULL);
					return true;
				}
				
			}while(false);


			// ---------------------------------------------------------------
			// ������ע
			float	 fCanBeBet = static_cast<float>(m_lTurnMaxScore - m_lTurnLessScore);
			LONGLONG lBetAmount = 0;
			
			// ������ּӴ���ע
			float fMultiple = 1.f;
			if( m_cbCurRound >= MAX_COUNT - 1 )
				fMultiple = 2.f;

			// ���������޸�
			lBetAmount += (LONGLONG)( 0.04f * fCanBeBet * (float)( wPlayCount - wPlayMingRank[wChairID] + 1 ) * fMultiple );

			// ���������޸�
			lBetAmount += (LONGLONG)( 0.03f * fCanBeBet * (float)( wPlayCount - wPlayShouRank[wChairID] + 1  ) * fMultiple );

			// �����������޸�
			lBetAmount += (LONGLONG)( 0.02f * fCanBeBet * (float)( wPlayCount - wPlayFullRank[wChairID] + 1  ) * fMultiple );
			
			// ���������������޸�
			switch( cbShouGenre[wChairID] )
			{
			case CT_SINGLE:
				lBetAmount += (LONGLONG)( 0.02f * fCanBeBet );
				break;
			case CT_ONE_DOUBLE:
				lBetAmount += (LONGLONG)( 0.03f * fCanBeBet );
				break;
			case CT_TWO_DOUBLE:
				lBetAmount += (LONGLONG)( 0.08f * fCanBeBet );
				break;
			case CT_THREE_TIAO:
				lBetAmount += (LONGLONG)( 0.10f * fCanBeBet );
				break;
			case CT_SHUN_ZI:
				lBetAmount += (LONGLONG)( 0.15f * fCanBeBet );
				break;
			case CT_TONG_HUA:
				lBetAmount += (LONGLONG)( 0.18f * fCanBeBet );
				break;
			case CT_HU_LU:
				lBetAmount += (LONGLONG)( 0.20f * fCanBeBet );
				break;
			case CT_TIE_ZHI:
				lBetAmount += (LONGLONG)( 0.35f * fCanBeBet );
				break;
			case CT_TONG_HUA_SHUN:
				lBetAmount += (LONGLONG)( 0.50f * fCanBeBet );
				break;
			}
			
			// ��ע��Χ
			LONGLONG lTurnMinScore = m_lTurnLessScore;
			LONGLONG lTurnMaxScore = m_lTurnLessScore + lBetAmount;
			if( lTurnMaxScore > m_lTurnMaxScore ) lTurnMaxScore = m_lTurnMaxScore;
			if( lTurnMaxScore < lTurnMinScore ) lTurnMaxScore = lTurnMinScore;

			// ��ע����
			LONGLONG lAddScore = lTurnMinScore;
			if( lTurnMaxScore > lTurnMinScore )
				lAddScore += ((rand()*( (lTurnMaxScore - lTurnMinScore) / RAND_MAX ))%( lTurnMaxScore - lTurnMinScore ));

			// �������ע��80%�� ��80%ֱ�����
			if( lAddScore > m_lTurnMaxScore * 80 / 100 )
			{
				if( m_GameLogic.RandomArea( 2, 80, 20 ) == 0 )
				{
					lAddScore = lTurnMaxScore;
				}
			}
			// ֻ��ע15%����60%ֻ��ע
			else if( lAddScore < m_lTurnLessScore + ((m_lTurnMaxScore - m_lTurnLessScore) * 15 / 100) )
			{
				if( m_GameLogic.RandomArea( 2, 60, 40 ) == 0 )
				{
					lAddScore = m_lTurnLessScore;
				}
			}

			// ��ע�������࣬ ֱ������Сֵ
			m_wBetCount++;
			if( m_wBetCount > 2 && m_GameLogic.RandomArea( 2, 80, 20 ) == 0 )
			{
				lAddScore = m_lTurnLessScore;
			}
			
			// ��ʵ��ע��
			lAddScore = lAddScore - (m_lTableScore[wChairID]+m_lUserScore[wChairID]);

			// ������ע����
			CMD_C_AddScore AddScore;
			AddScore.lScore = lAddScore;
			m_pIAndroidUserItem->SendSocketData(SUB_C_ADD_SCORE,&AddScore,sizeof(AddScore));

			return true;	
		}
	}

	return false;
}

//��Ϸ��Ϣ
bool CAndroidUserItemSink::OnEventGameMessage(WORD wSubCmdID, void * pData, WORD wDataSize)
{
	switch (wSubCmdID)
	{	
	case SUB_S_GAME_START:	//��Ϸ��ʼ
		{
			return OnSubGameStart(pData,wDataSize);
		}
	case SUB_S_GIVE_UP:		//�û�����
		{
			return OnSubGiveUp(pData,wDataSize);
		}
	case SUB_S_SEND_CARD:	//������Ϣ
		{
			return OnSubSendCard(pData,wDataSize);
		}	
	case SUB_S_ADD_SCORE:	//��Ҽ�ע
		{
			return OnSubAddGold(pData,wDataSize);
		}
	case SUB_S_GAME_END:	//��Ϸ����
		{
			return OnSubGameEnd(pData,wDataSize);
		}
	case SUB_S_TRUE_END:	//��Ϸ����
		{
			return OnSubTrueEnd(pData,wDataSize);
		}
	}

	//�������
	ASSERT(FALSE);

	return true;
}

//��Ϸ��Ϣ
bool CAndroidUserItemSink::OnEventFrameMessage(WORD wSubCmdID, void * pData, WORD wDataSize)
{
	return true;
}

//������Ϣ
bool CAndroidUserItemSink::OnEventSceneMessage(BYTE cbGameStatus, bool bLookonOther, void * pData, WORD wDataSize)
{
	switch (cbGameStatus)
	{
	case GAME_STATUS_FREE:		//����״̬
		{
			//Ч������
			if (wDataSize!=sizeof(CMD_S_StatusFree)) return false;

			//��Ϣ����
			CMD_S_StatusFree * pStatusFree=(CMD_S_StatusFree *)pData;

			//������
			m_lRobotScoreMin = pStatusFree->lRobotScoreMin;					// ���ֵ���ȡ��
			m_lRobotScoreMax = pStatusFree->lRobotScoreMax;					// ���ָ��ڴ��
			m_lRobotBankTake[0] = pStatusFree->lRobotBankTake[0];			// ȡ����
			m_lRobotBankTake[1] = pStatusFree->lRobotBankTake[1];			// ȡ����
			m_lRobotBankSave = pStatusFree->lRobotBankSave;					// �����

			// �жϲ���
			if( m_pIAndroidUserItem->GetMeUserItem()->GetUserScore() < m_lRobotScoreMin
				|| m_pIAndroidUserItem->GetMeUserItem()->GetUserScore() > m_lRobotScoreMax )
			{
				m_pIAndroidUserItem->SetGameTimer(IDI_BANK_OPERATION, (rand()%3) + 2 );
			}
			else if( m_pIAndroidUserItem->GetMeUserItem()->GetUserStatus() != US_READY )
			{
				m_pIAndroidUserItem->SetGameTimer(IDI_START_GAME, (rand()%3) + 2);
			}

			return true;
		}
	case GAME_STATUS_PLAY:	//��Ϸ״̬
		{
			//Ч������
			if (wDataSize!=sizeof(CMD_S_StatusPlay)) return false;
			CMD_S_StatusPlay * pStatusPlay=(CMD_S_StatusPlay *)pData;
	
			//������
			m_lRobotScoreMin = pStatusPlay->lRobotScoreMin;					// ���ֵ���ȡ��
			m_lRobotScoreMax = pStatusPlay->lRobotScoreMax;					// ���ָ��ڴ��
			m_lRobotBankTake[0] = pStatusPlay->lRobotBankTake[0];			// ȡ����
			m_lRobotBankTake[1] = pStatusPlay->lRobotBankTake[1];			// ȡ����
			m_lRobotBankSave = pStatusPlay->lRobotBankSave;					// �����

			return true;
		}
	}

	ASSERT(FALSE);

	return false;
}

//�û�����
void CAndroidUserItemSink::OnEventUserEnter(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

//�û��뿪
void CAndroidUserItemSink::OnEventUserLeave(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

//�û�����
void CAndroidUserItemSink::OnEventUserScore(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

//�û�״̬
void CAndroidUserItemSink::OnEventUserStatus(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

//�û���λ
void CAndroidUserItemSink::OnEventUserSegment(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

//��Ϸ��ʼ
bool CAndroidUserItemSink::OnSubGameStart(const void * pBuffer, WORD wDataSize)
{
	// Ч������
	if (wDataSize != sizeof(CMD_S_GameStart)) return false;
	CMD_S_GameStart * pGameStart = (CMD_S_GameStart *)pBuffer;

	//״̬����
	m_bShowHand=false;
	m_wCurrentUser=pGameStart->wCurrentUser;
	ZeroMemory(m_cbPlayStatus,sizeof(m_cbPlayStatus));

	//���ñ���
	m_wBetCount = 0;
	m_lCellScore = pGameStart->lCellScore;
	m_lServiceCharge=pGameStart->lServiceCharge;
	m_lDrawMaxScore=pGameStart->lDrawMaxScore;
	m_lTurnMaxScore=pGameStart->lTurnMaxScore;
	m_lTurnLessScore=pGameStart->lTurnLessScore;
	ZeroMemory(m_lUserScore,sizeof(m_lUserScore));
	ZeroMemory(m_lTableScore,sizeof(m_lTableScore));

	//������
	CopyMemory(m_cbHandCardData, pGameStart->cbHandCardData, sizeof(m_cbHandCardData));

	//�������
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		if (pGameStart->cbCardData[i]!=0x00)
		{
			//״̬����
			m_cbPlayStatus[i] = TRUE;
			m_lTableScore[i] = pGameStart->lCellScore;
		}

	}

	//��ǰ����
	m_cbCurRound = 1;

	//���ö�ʱ��
	if( m_wCurrentUser == m_pIAndroidUserItem->GetChairID() )
	{
		m_pIAndroidUserItem->SetGameTimer( IDI_USER_ADD_SCORE, TIME_USER_ADD_SCORE + (rand()%5) + 3 );
	}

	return true;
}

//�û���ע
bool CAndroidUserItemSink::OnSubAddGold(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_AddScore));
	if (wDataSize!=sizeof(CMD_S_AddScore)) return false;

	//��������
	CMD_S_AddScore * pAddScore = (CMD_S_AddScore *)pBuffer;

	//��������
	WORD wAddScoreUser = pAddScore->wAddScoreUser;

	//��������
	LONGLONG lCurrentScore = pAddScore->lUserScoreCount;
	LONGLONG lDrawAddScore = m_lTableScore[wAddScoreUser];

	//��������
	m_lUserScore[wAddScoreUser] = lCurrentScore;

	//���ñ���
	m_wCurrentUser=pAddScore->wCurrentUser;
	m_lTurnLessScore=pAddScore->lTurnLessScore;

	//�����ע
	if ( m_wCurrentUser == m_pIAndroidUserItem->GetChairID() )
	{
		m_pIAndroidUserItem->SetGameTimer( IDI_USER_ADD_SCORE, TIME_USER_ADD_SCORE + (rand()%2) );
	}

	return true;
}

// ������Ϣ
bool CAndroidUserItemSink::OnSubSendCard(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_SendCard));
	if (wDataSize!=sizeof(CMD_S_SendCard)) return false;

	//��������
	CMD_S_SendCard * pSendCard=(CMD_S_SendCard *)pBuffer;


	//��ע����
	m_wBetCount = 0;

	//�������
	m_cbCurRound++;
	if( m_cbCurRound > MAX_COUNT )
		m_cbCurRound = MAX_COUNT;

	//���ñ���
	m_wCurrentUser = pSendCard->wCurrentUser;
	m_lTurnMaxScore = pSendCard->lTurnMaxScore;

	//�㼯���
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//�������
		m_lTableScore[i] += m_lUserScore[i];

		//�û�����
		m_lUserScore[i]=0L;
	}

	//�����ע
	if ( m_wCurrentUser == m_pIAndroidUserItem->GetChairID() )
	{
		m_pIAndroidUserItem->SetGameTimer( IDI_USER_ADD_SCORE, TIME_USER_ADD_SCORE + (rand()%5) + 1);
	}

	return true;
}

// ����
bool CAndroidUserItemSink::OnSubGiveUp(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize == sizeof(CMD_S_GiveUp));
	if (wDataSize != sizeof(CMD_S_GiveUp)) return false;

	//��������
	CMD_S_GiveUp * pGiveUp = (CMD_S_GiveUp *)pBuffer;

	//��������
	WORD wGiveUpUser = pGiveUp->wGiveUpUser;

	//��ע����
	m_lDrawMaxScore = pGiveUp->lDrawMaxScore;
	m_lTurnMaxScore = pGiveUp->lTrunMaxScore;

	//״̬����
	m_cbPlayStatus[wGiveUpUser]=FALSE;

	//�����û�
	m_wCurrentUser=pGiveUp->wCurrentUser;

	//�����ע
	if ( m_wCurrentUser == m_pIAndroidUserItem->GetChairID() )
	{
		m_pIAndroidUserItem->SetGameTimer( IDI_USER_ADD_SCORE, TIME_USER_ADD_SCORE + (rand()%5) + 2);
	}

	return true;
}

// ��Ϸ����
bool CAndroidUserItemSink::OnSubGameEnd(const void * pBuffer, WORD wDataSize)
{
	//Ч�����
	if (wDataSize!=sizeof(CMD_S_GameEnd)) return false;
	CMD_S_GameEnd * pGameEnd=(CMD_S_GameEnd *)pBuffer;

	//ɾ����ʱ��	
	m_pIAndroidUserItem->KillGameTimer( IDI_USER_ADD_SCORE );

	//��ע��Ϣ
	m_lDrawMaxScore=0L;
	m_lTurnMaxScore=0L;
	m_lTurnLessScore=0L;
	ZeroMemory(m_lUserScore,sizeof(m_lUserScore));
	ZeroMemory(m_lTableScore,sizeof(m_lTableScore));

	//״̬����
	m_bShowHand=false;
	m_wCurrentUser=INVALID_CHAIR;
	ZeroMemory(m_cbPlayStatus,sizeof(m_cbPlayStatus));

	return true;
}

//��Ϸ����
bool CAndroidUserItemSink::OnSubTrueEnd(const void * pBuffer, WORD wDataSize)
{

	// �жϲ���
	if( m_pIAndroidUserItem->GetMeUserItem()->GetUserScore() < m_lRobotScoreMin
		|| m_pIAndroidUserItem->GetMeUserItem()->GetUserScore() > m_lRobotScoreMax )
	{
		m_pIAndroidUserItem->SetGameTimer(IDI_BANK_OPERATION, (rand()%3) + 2);
	}
	else
	{
		m_pIAndroidUserItem->SetGameTimer(IDI_START_GAME, (rand()%3) + 2);
	}

	return true;
}

//��ȡ����
void CAndroidUserItemSink::ReadConfigInformation()
{

}

// ��������
void CAndroidUserItemSink::CalculateRank( OUT WORD wRank[GAME_PLAYER], IN BYTE cbCardData[GAME_PLAYER][MAX_COUNT], IN BYTE cbCardCount )
{
	// �������
	for( int nPlay = 0; nPlay < GAME_PLAYER; ++nPlay )
	{
		wRank[nPlay] =  m_cbPlayStatus[nPlay];
	}

	// ������������
	for( int nPlayFirst = 0; nPlayFirst < GAME_PLAYER; ++nPlayFirst )
	{
		if( m_cbPlayStatus[nPlayFirst] )
		{
			for( int nNextIndex = 0; nNextIndex < GAME_PLAYER; ++nNextIndex )
			{
				if( m_cbPlayStatus[nNextIndex] && nNextIndex != nPlayFirst )
				{
					// �Ƚ�
					if( !m_GameLogic.CompareCard( cbCardData[nPlayFirst], cbCardData[nNextIndex], cbCardCount ) )
					{
						wRank[nPlayFirst]++;
					}
				}
			}
		}
	}
}


//�����������
DECLARE_CREATE_MODULE(AndroidUserItemSink);

//////////////////////////////////////////////////////////////////////////
