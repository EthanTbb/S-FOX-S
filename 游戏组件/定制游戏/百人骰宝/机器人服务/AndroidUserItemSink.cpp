#include "Stdafx.h"
#include "AndroidUserItemSink.h"
#include "math.h"

//////////////////////////////////////////////////////////////////////////

//ʱ���ʶ
#define IDI_REQUEST_BANKER			100									//���붨ʱ
#define IDI_GIVEUP_BANKER			101									//��ׯ��ʱ
#define IDI_BANK_OPERATE			102									//���ж�ʱ
#define IDI_CHECK_BANKER			103									//�����ׯ
#define IDI_PLACE_JETTON			110									//��ע���� (Ԥ��110-160)


int CAndroidUserItemSink::m_stlApplyBanker = 0L;
int CAndroidUserItemSink::m_stnApplyCount = 0L;

//////////////////////////////////////////////////////////////////////////

//���캯��
CAndroidUserItemSink::CAndroidUserItemSink()
{
	//��Ϸ����
	m_nRobotBetDelay = 0;
	m_bHaveConfig = false;
	m_lMaxChipBanker = 0;
	m_lMaxChipUser = 0;
	m_wCurrentBanker = 0;
	m_nChipTime = 0;
	m_nChipTimeCount = 0;
	m_lBankerLostScore = 0;
	m_lBankerDrawCount = 0;
	m_lBankerWinCount = 0;
	m_lBankerLoseCount = 0;
	ZeroMemory(m_lAreaChip, sizeof(m_lAreaChip));
	ZeroMemory(m_nChipLimit, sizeof(m_nChipLimit));
	ZeroMemory(m_bWinFlag, sizeof(m_bWinFlag));

	//��ׯ����
	m_nBankerCount=0;
	m_nCurMaxBankerCount=0;
	m_nRobotBetTimeLimit[0]=3;
	m_nRobotBetTimeLimit[1]=8;
	m_lRobotJettonLimit[0]=1000;
	m_lRobotJettonLimit[1]=1000000;
	//��ׯ����
	m_bMeApplyBanker=false;
	m_nWaitBanker=0;	
	m_nListUserCount=0;

	srand(GetTickCount());

	return;
}

//��������
CAndroidUserItemSink::~CAndroidUserItemSink()
{
}

//�ӿڲ�ѯ
VOID *  CAndroidUserItemSink::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IAndroidUserItemSink,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IAndroidUserItemSink,Guid,dwQueryVer);
	return NULL;
}

//��ʼ�ӿ�
bool  CAndroidUserItemSink::Initialization(IUnknownEx * pIUnknownEx)
{
	//��ѯ�ӿ�
	ASSERT(QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,IAndroidUserItem)!=NULL);
	m_pIAndroidUserItem=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,IAndroidUserItem);

	//�����ж�
	if (m_pIAndroidUserItem==NULL)
	{
		ASSERT(FALSE);
		return false;
	}

	return true;
}

//���ýӿ�
bool  CAndroidUserItemSink::RepositionSink()
{
	//��Ϸ����
	m_lMaxChipBanker = 0;
	m_lMaxChipUser = 0;
	m_wCurrentBanker = 0;
	m_nChipTime = 0;
	m_nChipTimeCount = 0;
	m_lBankerLostScore = 0;
	ZeroMemory(m_lAreaChip, sizeof(m_lAreaChip));
	ZeroMemory(m_nChipLimit, sizeof(m_nChipLimit));

	//��ׯ����
	m_nBankerCount=0;
	m_nCurMaxBankerCount=0;

	//��ׯ����
	m_bMeApplyBanker=false;
	m_nWaitBanker=0;

	return true;
}

//ʱ����Ϣ
bool  CAndroidUserItemSink::OnEventTimer(UINT nTimerID)
{
	switch (nTimerID)
	{
	case IDI_CHECK_BANKER:		//�����ׯ
		{
			m_pIAndroidUserItem->KillGameTimer(nTimerID);

			if (m_wCurrentBanker == m_pIAndroidUserItem->GetChairID())
				return false;

			int nMinCount = m_nRobotListMaxCount;
			if ( m_nRobotListMaxCount > m_nRobotListMinCount)
				nMinCount = (rand()%(m_nRobotListMaxCount - m_nRobotListMinCount+1)) + m_nRobotListMinCount;

			//��ׯ
			m_nWaitBanker++;

			//��������ׯ
			if ( m_bRobotBanker 
				&& !m_bMeApplyBanker 
				&& m_nWaitBanker >= m_nRobotWaitBanker 
				&& m_nListUserCount < m_nRobotListMaxCount 
				&& m_stlApplyBanker < m_nRobotListMaxCount 
				&& m_stlApplyBanker < nMinCount 
				/*&& m_stnApplyCount < 2*/ )
			{
				m_nWaitBanker = 0;

				//�Ϸ��ж�
				IServerUserItem *pIUserItemBanker = m_pIAndroidUserItem->GetMeUserItem();
				if (pIUserItemBanker->GetUserScore() > m_lBankerCondition ) 
				{
					//��������ׯ
					m_nBankerCount = 0;
					m_stlApplyBanker++;
					//m_stnApplyCount++;
					m_pIAndroidUserItem->SetGameTimer(IDI_REQUEST_BANKER, (rand() % 3) + 2);
				}
				else
				{
					//ִ��ȡ��
					SCORE lDiffScore = (m_lRobotBankGetScoreBankerMax-m_lRobotBankGetScoreBankerMin)/100;
					SCORE lScore = m_lRobotBankGetScoreBankerMin + (m_pIAndroidUserItem->GetUserID()%10)*(rand()%10)*lDiffScore + 
						(rand()%10+1)*(rand()%(lDiffScore/10));

					if (lScore > 0)
					{
						m_pIAndroidUserItem->PerformTakeScore(lScore);
					}

					IServerUserItem *pIUserItemBanker = m_pIAndroidUserItem->GetMeUserItem();
					if (pIUserItemBanker->GetUserScore() > m_lBankerCondition) 
					{
						//��������ׯ
						m_nBankerCount = 0;
						m_stlApplyBanker++;
						m_pIAndroidUserItem->SetGameTimer(IDI_REQUEST_BANKER, (rand() % 3) + 2);
					}
				}
			}

			return false;
		}
	case IDI_REQUEST_BANKER:	//������ׯ
		{
			m_pIAndroidUserItem->KillGameTimer(nTimerID);

			m_pIAndroidUserItem->SendSocketData(SUB_C_APPLY_BANKER);

			return false;
		}
	case IDI_GIVEUP_BANKER:		//������ׯ
		{
			m_pIAndroidUserItem->KillGameTimer(nTimerID);

			m_pIAndroidUserItem->SendSocketData(SUB_C_CANCEL_BANKER);

			m_nCurMaxBankerCount = 0;

			return false;
		}
	case IDI_BANK_OPERATE:		//���в���
		{
			m_pIAndroidUserItem->KillGameTimer(nTimerID);

			//��������
			IServerUserItem *pUserItem = m_pIAndroidUserItem->GetMeUserItem();
			LONGLONG lRobotScore = pUserItem->GetUserScore();			
			{

				//�жϴ�ȡ
				if (lRobotScore > m_lRobotScoreRange[1])
				{
					LONGLONG lSaveScore=0L;

					lSaveScore = LONGLONG(lRobotScore*m_nRobotBankStorageMul/100);
					if (lSaveScore > lRobotScore)  lSaveScore = lRobotScore;

					if (lSaveScore > 0 && m_wCurrentBanker != m_pIAndroidUserItem->GetChairID())
						m_pIAndroidUserItem->PerformSaveScore(lSaveScore);

				}
				else if (lRobotScore < m_lRobotScoreRange[0])
				{
					SCORE lDiffScore = (m_lRobotBankGetScoreMax-m_lRobotBankGetScoreMin)/100;
					SCORE lScore = m_lRobotBankGetScoreMin + (m_pIAndroidUserItem->GetUserID()%10)*(rand()%10)*lDiffScore + 
						(rand()%10+1)*(rand()%(lDiffScore/10));

					if (lScore > 0)
						m_pIAndroidUserItem->PerformTakeScore(lScore);
				}
			}
			return false;

			
		}
	default:
		{
			if (nTimerID >= IDI_PLACE_JETTON && nTimerID <= IDI_PLACE_JETTON+MAX_CHIP_TIME)
			{
				

				//��������
				int nRandNum = 0, nChipArea = 0, nCurChip = 0, nACTotal = 0, nCurJetLmt[2] = {};
				SCORE lMaxChipLmt = __min(m_lMaxChipBanker, m_lMaxChipUser);			//������עֵ
				WORD wMyID = m_pIAndroidUserItem->GetChairID();
				int nAreaChance[AREA_COUNT] = {};

				//��עƫ��
				for (int i = 0; i < AREA_COUNT; i++)
					nAreaChance[i] = int((8+m_bWinFlag[i]*2)*m_RobotInfo.nAreaChance[i]);
				for (int i = 0; i < AREA_COUNT; i++)
					nACTotal += nAreaChance[i];

				//MyDebug(_T("AreaChance [%d %d %d %d %d %d] [%d %d %d %d %d %d]"), m_bWinFlag[0], m_bWinFlag[1], m_bWinFlag[2], m_bWinFlag[3], m_bWinFlag[4], m_bWinFlag[5],
				//	nAreaChance[0], nAreaChance[1], nAreaChance[2], nAreaChance[3], nAreaChance[4], nAreaChance[5]);

				//ͳ�ƴ���
				m_nChipTimeCount++;

				//����˳�
				if (lMaxChipLmt < m_RobotInfo.nChip[m_nChipLimit[0]])	return false;
				for (int i = 0; i < AREA_COUNT; i++)
				{
					if (m_lAreaChip[i] >= m_RobotInfo.nChip[m_nChipLimit[0]])	break;
					if (i == AREA_COUNT-1)	return false;
				}

				//��ע����
				ASSERT(nACTotal>0);
				static int nStFluc = 1;				//�������
				if (nACTotal <= 0)	return false;
				do {
					int nTemp = rand();
					nRandNum = UINT(nTemp*nTemp*(wMyID+1)*nStFluc*3) % nACTotal;
					//MyDebug(_T("Rand [%d] [%d / %d]"), nTemp, nRandNum, nACTotal);
					for (int i = 0; i < AREA_COUNT; i++)
					{
						nRandNum -= nAreaChance[i];
						if (nRandNum < 0)
						{
							nChipArea = i;
							break;
						}
					}
				}
				while (m_lAreaChip[nChipArea] < m_RobotInfo.nChip[m_nChipLimit[0]]);
				nStFluc = nStFluc%3 + 1;

				//��ע��С
				if (m_nChipLimit[0] == m_nChipLimit[1])
					nCurChip = m_nChipLimit[0];
				else
				{
					//���ñ���
					lMaxChipLmt = __min(lMaxChipLmt, m_lAreaChip[nChipArea]);
					nCurJetLmt[0] = m_nChipLimit[0];
					nCurJetLmt[1] = m_nChipLimit[0];

					//���㵱ǰ������
					for (int i = m_nChipLimit[1]; i > m_nChipLimit[0]; i--)
					{
						if (lMaxChipLmt > m_RobotInfo.nChip[i]) 
						{
							nCurJetLmt[1] = i;
							break;
						}
					}

					//�����ע
					nRandNum = (rand()+wMyID) % (nCurJetLmt[1]-nCurJetLmt[0]+1);
					nCurChip = nCurJetLmt[0] + nRandNum;

					//���¿��� (��ׯ�ҽ�ҽ���ʱ�ᾡ����֤�������)
					if (m_nChipTimeCount < m_nChipTime)
					{
						SCORE lLeftJetton = SCORE( (lMaxChipLmt-m_RobotInfo.nChip[nCurChip])/(m_nChipTime-m_nChipTimeCount) );

						//�������� (��ȫ����С���Ƴ�����עҲ����)
						if (lLeftJetton < m_RobotInfo.nChip[m_nChipLimit[0]] && nCurChip > m_nChipLimit[0])
							nCurChip--;
					}
				}

				/*ASSERT( MyDebug(_T("��������ע %d ��ע���� [%d/%d] ��ע [%d %d] ��Χ [%d %d] ���� [%I64d %I64d %I64d]"), wMyID, nTimerID-IDI_PLACE_JETTON, m_nChipTime, 
					nChipArea, m_RobotInfo.nChip[nCurChip], m_nChipLimit[0], m_nChipLimit[1], m_lMaxChipBanker, m_lMaxChipUser, lMaxChipLmt) );*/

				//��������
				CMD_C_PlaceJetton PlaceJetton = {};

				//�������
				PlaceJetton.cbJettonArea = nChipArea;		
				PlaceJetton.lJettonScore = m_RobotInfo.nChip[nCurChip];

				//������Ϣ
				m_pIAndroidUserItem->SendSocketData(SUB_C_PLACE_JETTON, &PlaceJetton, sizeof(PlaceJetton));
			}

			m_pIAndroidUserItem->KillGameTimer(nTimerID);
			return false;
		}
	}
	return false;
}

//��Ϸ��Ϣ
bool  CAndroidUserItemSink::OnEventGameMessage(WORD wSubCmdID, VOID * pBuffer, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_S_GAME_FREE:			//��Ϸ���� 
		{
			return OnSubGameFree(pBuffer, wDataSize);
		}
	case SUB_S_GAME_START:			//��Ϸ��ʼ
		{
			return OnSubGameStart(pBuffer, wDataSize);
		}
	case SUB_S_PLACE_JETTON:		//�û���ע
		{
			return OnSubPlaceJetton(pBuffer, wDataSize);
		}
	case SUB_S_APPLY_BANKER:		//������ׯ 
		{
			return OnSubUserApplyBanker(pBuffer,wDataSize);
		}
	case SUB_S_CANCEL_BANKER:		//ȡ����ׯ 
		{
			return OnSubUserCancelBanker(pBuffer,wDataSize);
		}
	case SUB_S_CHANGE_BANKER:		//�л�ׯ�� 
		{
			return OnSubChangeBanker(pBuffer,wDataSize);
		}
	case SUB_S_GAME_END:			//��Ϸ���� 
		{
			return OnSubGameEnd(pBuffer, wDataSize);
		}
	case SUB_S_SEND_RECORD:			//��Ϸ��¼ (����)
		{
			return true;
		}
	case SUB_S_PLACE_JETTON_FAIL:	//��עʧ�� (����)
		{
			return true;
		}
	case SUB_S_ROBOT_BANKER:		//��ׯ֪ͨ
		{
			return OnSubRobotBanker(pBuffer, wDataSize);
		}
	case SUB_S_ROBOT_CONFIG:		//��������
		{
			return OnSubRobotConfig(pBuffer, wDataSize);
		}
	}

	//�������
	//ASSERT(FALSE);

	return true;
}

//��Ϸ��Ϣ
bool  CAndroidUserItemSink::OnEventFrameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	return true;
}

//������Ϣ
bool  CAndroidUserItemSink::OnEventSceneMessage(BYTE cbGameStatus, bool bLookonOther, VOID * pData, WORD wDataSize)
{
	switch (cbGameStatus)
	{
	case GAME_STATUS_FREE:			//����״̬
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_S_StatusFree));
			if (wDataSize!=sizeof(CMD_S_StatusFree)) return false;

			//��Ϣ����
			CMD_S_StatusFree * pStatusFree=(CMD_S_StatusFree *)pData;

			m_lUserLimitScore = pStatusFree->lUserMaxScore;
			m_lAreaLimitScore = pStatusFree->lAreaLimitScore;
			m_lBankerCondition = pStatusFree->lApplyBankerCondition;

			memcpy(m_szRoomName, pStatusFree->szGameRoomName, sizeof(m_szRoomName));
			
			ReadConfigInformation(&pStatusFree->CustomAndroid);

			//��ׯ����
			if (pStatusFree->wBankerUser == INVALID_CHAIR)
			{
				if (m_bRobotBanker && m_nRobotWaitBanker == 0  && m_stlApplyBanker < m_nRobotApplyBanker)
				{
					//�Ϸ��ж�
					IServerUserItem *pIUserItemBanker = m_pIAndroidUserItem->GetMeUserItem();
					if (pIUserItemBanker->GetUserScore() > m_lBankerCondition) 
					{
						//��������ׯ
						m_nBankerCount = 0;
						m_stlApplyBanker++;

						BYTE cbTime = (pStatusFree->cbTimeLeave>0?(rand()%pStatusFree->cbTimeLeave+1):2);
						if (cbTime == 0) cbTime = 2;

						m_pIAndroidUserItem->SetGameTimer(IDI_REQUEST_BANKER, cbTime);
					}
				}
			}

			return true;
		}
	case GS_GAME_START:		//��Ϸ״̬
	case GS_PLAYER_BET:		//��ע״̬
	case GS_GAME_END:		//����״̬
	case GS_MOVECARD_END:	//����״̬
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_S_StatusPlay));
			if (wDataSize!=sizeof(CMD_S_StatusPlay)) return false;

			//��Ϣ����
			CMD_S_StatusPlay * pStatusPlay=(CMD_S_StatusPlay *)pData;

			//ׯ����Ϣ
			m_wCurrentBanker = pStatusPlay->wBankerUser;

			m_lUserLimitScore = pStatusPlay->lUserMaxScore;
			m_lAreaLimitScore = pStatusPlay->lAreaLimitScore;
			m_lBankerCondition = pStatusPlay->lApplyBankerCondition;

			memcpy(m_szRoomName, pStatusPlay->szGameRoomName, sizeof(m_szRoomName));

			ReadConfigInformation(&pStatusPlay->CustomAndroid);

			return true;
		}
	}

	return true;
}

//�û�����
VOID  CAndroidUserItemSink::OnEventUserEnter(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

//�û��뿪
VOID  CAndroidUserItemSink::OnEventUserLeave(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

//�û�����
VOID  CAndroidUserItemSink::OnEventUserScore(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

//�û�״̬
VOID  CAndroidUserItemSink::OnEventUserStatus(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}


//��Ϸ����
bool CAndroidUserItemSink::OnSubGameFree(const VOID * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_GameFree));
	if (wDataSize!=sizeof(CMD_S_GameFree)) return false;

	//��Ϣ����
	CMD_S_GameFree * pGameFree=(CMD_S_GameFree *)pBuffer;

	m_nListUserCount = pGameFree->nListUserCount;

	//���в���
	if (pGameFree->cbTimeLeave > 1)
		m_pIAndroidUserItem->SetGameTimer(IDI_BANK_OPERATE, (rand() % (pGameFree->cbTimeLeave-1)) + 1);

	bool bMeGiveUp = false;
	if (m_wCurrentBanker == m_pIAndroidUserItem->GetChairID())
	{
		m_nBankerCount++;
		if ( m_nBankerCount >= m_nRobotBankerCount )
		{
			//��������ׯ
			m_nBankerCount = 0;
			m_pIAndroidUserItem->SetGameTimer(IDI_GIVEUP_BANKER, rand()%2 + 1);

			bMeGiveUp = true;
		}
	}

	if (m_wCurrentBanker != m_pIAndroidUserItem->GetChairID() || bMeGiveUp)
		m_pIAndroidUserItem->SetGameTimer(IDI_CHECK_BANKER, pGameFree->cbTimeLeave - 3);

	return true;
}

//��Ϸ��ʼ
bool CAndroidUserItemSink::OnSubGameStart(const VOID * pBuffer, WORD wDataSize)
{	
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_GameStart));
	if (wDataSize!=sizeof(CMD_S_GameStart)) return false;

	//��Ϣ����
	CMD_S_GameStart * pGameStart=(CMD_S_GameStart *)pBuffer;

	m_wCurrentBanker = pGameStart->wBankerUser;
	memcpy(m_bWinFlag, pGameStart->bWinFlag, sizeof(m_bWinFlag));

	if (m_wCurrentBanker != m_pIAndroidUserItem->GetChairID())
	{
		m_nBankerCount = 0;
		m_lBankerDrawCount = 0;
		m_lBankerWinCount = 0;
		m_lBankerLoseCount = 0;
	}

	//�Լ���ׯ������ע������
	if (pGameStart->wBankerUser == m_pIAndroidUserItem->GetChairID() || pGameStart->nChipRobotCount <= 0)
		return true;

	//���ñ���
	m_lMaxChipBanker = pGameStart->lBankerScore/m_RobotInfo.nMaxTime;
	m_lMaxChipUser = __min(pGameStart->lUserMaxScore/m_RobotInfo.nMaxTime, m_lUserLimitScore);
	m_nChipTimeCount = 0;
	ZeroMemory(m_nChipLimit, sizeof(m_nChipLimit));
	for (int i = 0; i < AREA_COUNT; i++)
		m_lAreaChip[i] = m_lAreaLimitScore;

	//ϵͳ��ׯ
	if (pGameStart->wBankerUser == INVALID_CHAIR)
		m_lMaxChipBanker = 2147483647/m_RobotInfo.nMaxTime;
	else
		m_lMaxChipUser = __min( m_lMaxChipUser, m_lMaxChipBanker );

	if(pGameStart->nAndriodApplyCount > 0)
		m_stlApplyBanker=pGameStart->nAndriodApplyCount;
	else
		m_stlApplyBanker=0;

	//������ע����
	int nElapse = 0;												
	WORD wMyID = m_pIAndroidUserItem->GetChairID();

	if (m_nRobotBetTimeLimit[0] == m_nRobotBetTimeLimit[1])
		m_nChipTime = m_nRobotBetTimeLimit[0];
	else
		m_nChipTime = (rand()+wMyID)%(m_nRobotBetTimeLimit[1]-m_nRobotBetTimeLimit[0]+1) + m_nRobotBetTimeLimit[0];
	ASSERT(m_nChipTime>=0);		
	if (m_nChipTime <= 0)	return true;								//��ȷ,2����������
	if (m_nChipTime > MAX_CHIP_TIME)	m_nChipTime = MAX_CHIP_TIME;	//�޶�MAX_CHIP����ע

	//���㷶Χ
	if (!CalcJettonRange(__min(m_lMaxChipBanker, m_lMaxChipUser), m_lRobotJettonLimit, m_nChipTime, m_nChipLimit))
		return true;

	//����ʱ��
	int nTimeGrid = int(pGameStart->cbTimeLeave-m_nRobotBetDelay)*800/m_nChipTime;		//ʱ���,ǰ1�벻��ע,����-2,800��ʾ��������עʱ�䷶Χǧ�ֱ�
	for (int i = 0; i < m_nChipTime; i++)
	{
		int nRandRage = int( nTimeGrid * i / (1500*sqrt((double)m_nChipTime)) ) + 1;		//������Χ
		nElapse = m_nRobotBetDelay + (nTimeGrid*i)/1000 + ( (rand()+wMyID)%(nRandRage*2) - (nRandRage-1) );
		ASSERT(nElapse>=m_nRobotBetDelay&&nElapse<=pGameStart->cbTimeLeave);
		if (nElapse < m_nRobotBetDelay || nElapse > pGameStart->cbTimeLeave)	continue;
		
		m_pIAndroidUserItem->SetGameTimer(IDI_PLACE_JETTON+i+1, nElapse);
	}

	//ASSERT( MyDebug(_T("������ %d ��ע���� %d ��Χ [%d %d] ������ %d ���� [%I64d %I64d] ��ׯ [%d %d]"), wMyID, m_nChipTime, m_nChipLimit[0], m_nChipLimit[1], 
	//	pGameStart->nChipRobotCount, m_lMaxChipBanker, m_lMaxChipUser, m_nBankerCount, m_nCurMaxBankerCount) );

	return true;
}

//�û���ע
bool CAndroidUserItemSink::OnSubPlaceJetton(const VOID * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_PlaceJetton));
	if (wDataSize!=sizeof(CMD_S_PlaceJetton)) return false;

	//��Ϣ����
	CMD_S_PlaceJetton * pPlaceJetton=(CMD_S_PlaceJetton *)pBuffer;

	//���ñ���
	m_lMaxChipBanker -= pPlaceJetton->lJettonScore;
	m_lAreaChip[pPlaceJetton->cbJettonArea] -= pPlaceJetton->lJettonScore;
	if (pPlaceJetton->wChairID == m_pIAndroidUserItem->GetChairID())
		m_lMaxChipUser -= pPlaceJetton->lJettonScore;

	return true;
}

//��עʧ��
bool CAndroidUserItemSink::OnSubPlaceJettonFail(const VOID * pBuffer, WORD wDataSize)
{
	return true;
}

//��Ϸ����
bool CAndroidUserItemSink::OnSubGameEnd(const VOID * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_GameEnd));
	if (wDataSize!=sizeof(CMD_S_GameEnd)) return false;

	//��Ϣ����
	CMD_S_GameEnd * pGameEnd=(CMD_S_GameEnd *)pBuffer;

	//if ( m_wCurrentBanker == m_pIAndroidUserItem->GetChairID() )
	//{
	//	m_nBankerCount++;
	//	if (pGameEnd->lBankerScore > 0)
	//		m_lBankerWinCount++;
	//	else if (pGameEnd->lBankerScore < 0)
	//		m_lBankerLoseCount++;
	//	//MyDebug(_T("�����˵�ׯ [%d / %d] W [%I64d / %I64d] L [%I64d / %I64d]"), m_nBankerCount, m_nCurMaxBankerCount,
	//	//	m_lBankerWinCount,m_lRobotGiveUpWin,m_lBankerLoseCount,m_lRobotGiveUpLose);
	//	if ( m_nBankerCount >= m_nCurMaxBankerCount || m_lBankerWinCount >= m_lRobotGiveUpWin || 
	//		 m_lBankerLoseCount >= m_lRobotGiveUpLose)
	//	{
	//		//��������ׯ
	//		m_nBankerCount = 0;
	//		m_lBankerDrawCount = 0;
	//		m_lBankerWinCount = 0;
	//		m_lBankerLoseCount = 0;
	//		m_pIAndroidUserItem->SetGameTimer(IDI_GIVEUP_BANKER, pGameEnd->cbTimeLeave + 1);
	//	}
	//	else
	//	{
	//		m_lBankerLostScore = pGameEnd->lBankerScore;

	//		BYTE bTime = (pGameEnd->cbTimeLeave>4?pGameEnd->cbTimeLeave-3:1);
	//		m_pIAndroidUserItem->SetGameTimer(IDI_BANK_OPERATE, bTime);

	//		IServerUserItem *pUserItem = m_pIAndroidUserItem->GetMeUserItem();
	//		SCORE lRobotScore = pUserItem->GetUserScore();
	//		//MyDebug(_T("ׯ��ȡǮ [%I64d + %I64d = %I64d]"), lRobotScore, m_lBankerLostScore, m_lBankerLostScore+lRobotScore);
	//	}
	//}

	return true;
}

//������ׯ
bool CAndroidUserItemSink::OnSubUserApplyBanker(const VOID * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_ApplyBanker));
	if (wDataSize!=sizeof(CMD_S_ApplyBanker)) return false;

	//��Ϣ����
	CMD_S_ApplyBanker * pApplyBanker=(CMD_S_ApplyBanker *)pBuffer;

	//�Լ��ж�
	if (m_pIAndroidUserItem->GetChairID()==pApplyBanker->wApplyUser) 
		m_bMeApplyBanker=true;

	return true;
}

//ȡ����ׯ
bool CAndroidUserItemSink::OnSubUserCancelBanker(const VOID * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_CancelBanker));
	if (wDataSize!=sizeof(CMD_S_CancelBanker)) return false;

	//��Ϣ����
	CMD_S_CancelBanker * pCancelBanker=(CMD_S_CancelBanker *)pBuffer;

	//�Լ��ж�
	if (m_pIAndroidUserItem->GetChairID()==pCancelBanker->wCancelUser) 
		m_bMeApplyBanker=false;

	return true;
}

//�л�ׯ��
bool CAndroidUserItemSink::OnSubChangeBanker(const VOID * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_ChangeBanker));
	if (wDataSize!=sizeof(CMD_S_ChangeBanker)) return false;

	//��Ϣ����
	CMD_S_ChangeBanker * pChangeBanker = (CMD_S_ChangeBanker *)pBuffer;

	if ( m_wCurrentBanker == m_pIAndroidUserItem->GetChairID() && m_wCurrentBanker != pChangeBanker->wBankerUser )
	{		
		m_nWaitBanker = 0;
		m_bMeApplyBanker = false;
	}

	m_wCurrentBanker = pChangeBanker->wBankerUser;

	return true;
}

//��ׯ֪ͨ
bool CAndroidUserItemSink::OnSubRobotBanker(const VOID * pBuffer, WORD wDataSize)
{
	if (!m_bRobotBanker)
		return true;

	//�������
	IServerUserItem *pUserItem = m_pIAndroidUserItem->GetMeUserItem();
	SCORE lRobotScore = pUserItem->GetUserScore();
	if (m_pIAndroidUserItem->GetChairID() != m_wCurrentBanker && lRobotScore >= m_lBankerCondition && lRobotScore >= m_lRobotBankerCondition)
	{
		m_pIAndroidUserItem->SetGameTimer(IDI_REQUEST_BANKER, (rand() % 5) + 1);
	}

	return true;
}

//����֪ͨ
bool CAndroidUserItemSink::OnSubRobotConfig(const VOID * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_RobotConfig));
	if (wDataSize!=sizeof(CMD_S_RobotConfig)) return false;

	//��Ϣ����
	CMD_S_RobotConfig * pRobotConfig = (CMD_S_RobotConfig *)pBuffer;

	m_lRobotBankerCondition		= pRobotConfig->robotConfig.lBankerCondition;
	m_lRobotBankSaveCondition	= pRobotConfig->robotConfig.lBankSaveCondition;
	m_lRobotBankDrawCondition	= pRobotConfig->robotConfig.lBankDrawCondition;
	m_lRobotBankSaveScore		= pRobotConfig->robotConfig.lBankSaveScore;
	m_lRobotBankDrawScore		= pRobotConfig->robotConfig.lBankDrawScore;
	m_lRobotJettonLimit[0]		= pRobotConfig->robotConfig.lJettonLimit[0];
	m_lRobotJettonLimit[1]		= pRobotConfig->robotConfig.lJettonLimit[1];
	m_nRobotBetTimeLimit[0]		= (int)pRobotConfig->robotConfig.lBetTimeLimit;
	m_nRobotBetTimeLimit[1]		= (int)pRobotConfig->robotConfig.lBetTimeLimit;
	m_lRobotBankDrawCount		= pRobotConfig->robotConfig.lBankerDrawCount;
	m_lRobotGiveUpWin			= pRobotConfig->robotConfig.lBankerWinGiveUp;
	m_lRobotGiveUpLose			= pRobotConfig->robotConfig.lBankerLoseGiveUp;

	m_bHaveConfig = true;

	//MyDebug(_T("���������� %I64d %I64d %I64d %I64d %I64d -- %I64d %I64d %d %d --  %I64d %I64d %I64d"), m_lRobotBankerCondition, 
	//	m_lRobotBankSaveCondition, m_lRobotBankDrawCondition, m_lRobotBankSaveScore, m_lRobotBankDrawScore,
	//	m_lRobotJettonLimit[0], m_lRobotJettonLimit[1], m_nRobotBetTimeLimit[0], m_nRobotBetTimeLimit[1],
	//	m_lRobotBankDrawCount, m_lRobotGiveUpWin, m_lRobotGiveUpLose);

	return true;
}

//��ȡ����
VOID CAndroidUserItemSink::ReadConfigInformation(tagCustomAndroid *pCustomAndroid)
{
	//��������
	m_nRobotBetTimeLimit[0] = pCustomAndroid->lRobotMinBetTime;
	m_nRobotBetTimeLimit[1] = pCustomAndroid->lRobotMaxBetTime;
	if (m_nRobotBetTimeLimit[0] < 0) m_nRobotBetTimeLimit[0] = 0;
	if (m_nRobotBetTimeLimit[1] < m_nRobotBetTimeLimit[0]) m_nRobotBetTimeLimit[1] = m_nRobotBetTimeLimit[0];

	//��������
	m_lRobotJettonLimit[0] = pCustomAndroid->lRobotMinJetton;
	m_lRobotJettonLimit[1] = pCustomAndroid->lRobotMaxJetton;
	if (m_lRobotJettonLimit[1] > 5000000)					m_lRobotJettonLimit[1] = 5000000;
	if (m_lRobotJettonLimit[0] < 100)						m_lRobotJettonLimit[0] = 100;
	if (m_lRobotJettonLimit[1] < m_lRobotJettonLimit[0])	m_lRobotJettonLimit[1] = m_lRobotJettonLimit[0];

	//�Ƿ���ׯ
	m_bRobotBanker = (pCustomAndroid->nEnableRobotBanker == TRUE)?true:false;

	//��ׯ����
	LONGLONG lRobotBankerCountMin = pCustomAndroid->lRobotBankerCountMin;
	LONGLONG lRobotBankerCountMax = pCustomAndroid->lRobotBankerCountMax;
	m_nRobotBankerCount = rand()%(lRobotBankerCountMax-lRobotBankerCountMin+1) + lRobotBankerCountMin;

	//�б�����
	m_nRobotListMinCount = pCustomAndroid->lRobotListMinCount;
	m_nRobotListMaxCount = pCustomAndroid->lRobotListMaxCount;

	//������
	m_nRobotApplyBanker = pCustomAndroid->lRobotApplyBanker;

	//��������
	m_nRobotWaitBanker = pCustomAndroid->lRobotWaitBanker;

	//��������
	m_lRobotScoreRange[0] = pCustomAndroid->lRobotScoreMin;
	m_lRobotScoreRange[1] = pCustomAndroid->lRobotScoreMax;
	if (m_lRobotScoreRange[1] < m_lRobotScoreRange[0])	m_lRobotScoreRange[1] = m_lRobotScoreRange[0];

	//�������
	m_lRobotBankGetScoreMin = pCustomAndroid->lRobotBankGetMin;
	m_lRobotBankGetScoreMax = pCustomAndroid->lRobotBankGetMax;

	//������� (ׯ��)
	m_lRobotBankGetScoreBankerMin = pCustomAndroid->lRobotBankGetBankerMin;
	m_lRobotBankGetScoreBankerMax = pCustomAndroid->lRobotBankGetBankerMax;

	//����
	m_nRobotBankStorageMul = pCustomAndroid->lRobotBankStoMul;
	if (m_nRobotBankStorageMul<0 || m_nRobotBankStorageMul>100) m_nRobotBankStorageMul = 20;
}

//���㷶Χ	(����ֵ��ʾ�Ƿ����ͨ���½����޴ﵽ��ע)
bool CAndroidUserItemSink::CalcJettonRange(SCORE lMaxScore, SCORE lChipLmt[], int & nChipTime, int lJetLmt[])
{
	//�������
	bool bHaveSetMinChip = false;

	//����һע
	if (lMaxScore < m_RobotInfo.nChip[0])	return false;

	//���÷�Χ
	for (int i = 0; i < CountArray(m_RobotInfo.nChip); i++)
	{
		if (!bHaveSetMinChip && m_RobotInfo.nChip[i] >= lChipLmt[0])
		{ 
			lJetLmt[0] = i;
			bHaveSetMinChip = true;
		}
		if (m_RobotInfo.nChip[i] <= lChipLmt[1])
			lJetLmt[1] = i;
	}
	if (lJetLmt[0] > lJetLmt[1])	lJetLmt[0] = lJetLmt[1];

	//�Ƿ񽵵�����
	//if (nChipTime * m_RobotInfo.nChip[lJetLmt[0]] > lMaxScore)
	//{
	//	//�Ƿ񽵵���ע����
	//	if (nChipTime * m_RobotInfo.nChip[0] > lMaxScore)
	//	{
	//		nChipTime = int(lMaxScore/m_RobotInfo.nChip[0]);
	//		lJetLmt[0] = 0;
	//		lJetLmt[1] = 0;
	//	}
	//	else
	//	{
	//		//���͵���������
	//		while (nChipTime * m_RobotInfo.nChip[lJetLmt[0]] > lMaxScore)
	//		{
	//			lJetLmt[0]--;
	//			ASSERT(lJetLmt[0]>=0);
	//		}
	//	}
	//}

	return true;
}

//��ȡ����
SCORE CAndroidUserItemSink::GetPrivateProfileInt64(LPCTSTR lpAppName, LPCTSTR lpKeyName, SCORE lDefault, LPCTSTR lpFileName)
{
	//��������
	TCHAR OutBuf[255] = _T("");
	TCHAR DefaultBuf[255] = {};
	SCORE lNumber = 0;

	GetPrivateProfileString(lpAppName, lpKeyName, DefaultBuf, OutBuf, 255, lpFileName);

	if (OutBuf[0] != 0)
		myscanf(OutBuf, mystrlen(OutBuf), _T("%I64d"), &lNumber);
	else
		lNumber = lDefault;

	return lNumber;
}

//�����������
DECLARE_CREATE_MODULE(AndroidUserItemSink);

//////////////////////////////////////////////////////////////////////////
