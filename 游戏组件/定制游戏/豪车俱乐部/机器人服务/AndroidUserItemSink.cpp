#include "Stdafx.h"
#include "AndroidUserItemSink.h"
#include "math.h"

//////////////////////////////////////////////////////////////////////////

//ʱ���ʶ
#define IDI_BANK_OPERATE_GET		101									//���ж�ʱ
#define IDI_BANK_OPERATE_SAVE		102									//���ж�ʱ

#define IDI_CHECK_BANKER			103									//�����ׯ
#define IDI_REQUEST_BANKER			104									//���붨ʱ
#define IDI_GIVEUP_BANKER			105									//��ׯ��ʱ
#define IDI_PLACE_JETTON			110									//��ע���� (Ԥ��110-160)

//////////////////////////////////////////////////////////////////////////

int CAndroidUserItemSink::m_stlApplyBanker = 0L;

//���캯��
CAndroidUserItemSink::CAndroidUserItemSink()
{
	//��Ϸ����
	m_lMaxChipBanker = 0;
	m_lMaxChipUser = 0;
	m_wCurrentBanker = 0;
	m_nChipTime = 0;
	m_nChipTimeCount = 0;
	m_cbTimeLeave = 0;
	ZeroMemory(m_lAreaChip, sizeof(m_lAreaChip));
	ZeroMemory(m_nChipLimit, sizeof(m_nChipLimit));
	m_nRobotBetTimeLimit[0]=3;
	m_nRobotBetTimeLimit[1]=8;
	m_lRobotJettonLimit[0]=10;
	m_lRobotJettonLimit[1]=1000000;

	//��ׯ����
	m_bMeApplyBanker=false;
	m_bReduceJettonLimit=false;
	m_nWaitBanker=0;
	m_nBankerCount=0;

	m_lRobotGiveUpWinMin=20000;				
	m_lRobotGiveUpWinMax=300000;				
	m_lBankerGetScore=0;

	m_lUserScore=0;

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
	m_cbTimeLeave = 0;
	ZeroMemory(m_lAreaChip, sizeof(m_lAreaChip));
	ZeroMemory(m_nChipLimit, sizeof(m_nChipLimit));

	//��ׯ����
	m_bMeApplyBanker=false;
	m_nWaitBanker=0;
	m_nBankerCount=0;
	m_lUserScore=0;

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
			//��ׯ
			m_nWaitBanker++;

				//��������ׯ
			if ( m_bRobotBanker 
				&& !m_bMeApplyBanker
				&& m_nWaitBanker >= m_nRobotWaitBanker 
				&& m_stlApplyBanker < m_nRobotApplyBanker)
			{
					//�Ϸ��ж�
				IServerUserItem *pIUserItemBanker = m_pIAndroidUserItem->GetMeUserItem();
				if (pIUserItemBanker->GetUserScore()*m_nJettonMultiple > m_lBankerCondition) 
				{
					//��������ׯ
					m_nBankerCount = 0;
					m_stlApplyBanker++;
					m_pIAndroidUserItem->SetGameTimer(IDI_REQUEST_BANKER, (rand() % m_cbTimeLeave) + 1);
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

			return false;
		}
	case IDI_BANK_OPERATE_GET:	//���в���
		{
			m_pIAndroidUserItem->KillGameTimer(nTimerID);

			//��������
			IServerUserItem *pUserItem = m_pIAndroidUserItem->GetMeUserItem();
			LONGLONG lRobotScore = (pUserItem->GetUserScore()+m_lUserScore)*m_nJettonMultiple;			
			
			if (lRobotScore < m_lRobotScoreRange[0])
			{
				SCORE lScore=rand()%m_lRobotBankGetScoreBanker+m_lRobotBankGetScore;
				if (lScore > 0)
					m_pIAndroidUserItem->PerformTakeScore(lScore/m_nJettonMultiple);

				LONGLONG lRobotNewScore = pUserItem->GetUserScore()*m_nJettonMultiple;

			}
			
			return false;
		}

	case IDI_BANK_OPERATE_SAVE:	//���в���
		{
			m_pIAndroidUserItem->KillGameTimer(nTimerID);

			//��������
			IServerUserItem *pUserItem = m_pIAndroidUserItem->GetMeUserItem();
			LONGLONG lRobotScore = pUserItem->GetUserScore()*m_nJettonMultiple;			
			
			//�жϴ�ȡ
			if (lRobotScore > m_lRobotScoreRange[1])
			{
				LONGLONG lSaveScore=0L;

				lSaveScore = LONGLONG(lRobotScore*m_nRobotBankStorageMul/100);
				if (lSaveScore > lRobotScore)  lSaveScore = lRobotScore;

				if (lSaveScore > 0 && m_wCurrentBanker != m_pIAndroidUserItem->GetChairID())
					m_pIAndroidUserItem->PerformSaveScore(lSaveScore/m_nJettonMultiple);

				LONGLONG lRobotNewScore = pUserItem->GetUserScore();

				CString strInfo;
				strInfo.Format(TEXT("[%s] ִ�д����ǰ���(%I64d)��������(%I64d)"),pUserItem->GetNickName(),lRobotScore,lRobotNewScore);

				//NcaTextOut(strInfo);

			}
				
			return false;
		}

	default:
		{
			if (nTimerID >= IDI_PLACE_JETTON && nTimerID <= IDI_PLACE_JETTON+MAX_CHIP_TIME)
			{
				//srand(GetTickCount());

				//��������
				int nRandNum = 0, nChipArea = 0, nCurChip = 0, nACTotal = 0, nCurJetLmt[2] = {};
				WORD wMyID = m_pIAndroidUserItem->GetChairID();
				for (int i = 0; i < AREA_COUNT; i++)
					nACTotal += m_RobotInfo.nAreaChance[i];

				//ͳ����ע
				LONGLONG lScore=m_lMaxChipBanker;
				for(BYTE i=0;i<AREA_COUNT;i++)
				{
					BYTE bcOutCadDataWin[AREA_COUNT];
					BYTE bcData[1];
					bcData[0]=i+1;
					m_GameLogic.GetCardType(bcData[0],1,bcOutCadDataWin);
					for (int nAreaIndex=0; nAreaIndex<AREA_COUNT; ++nAreaIndex) 
					{
						LONGLONG lAreaScore=m_lAreaLimitScore-m_lAreaChip[nAreaIndex];

						if(lAreaScore < 0)
							lAreaScore=m_lAreaChip[nAreaIndex];

						if(bcOutCadDataWin[nAreaIndex] > 0)
						{
							if(nAreaIndex == nChipArea)
							{
								lScore-=lAreaScore*(bcOutCadDataWin[nAreaIndex]-1);
							}
							else
								lScore+=lAreaScore;
						}
					}
				}

				LONGLONG lMaxChipLmt = __min(lScore, m_lMaxChipUser);			//������עֵ
				//ͳ�ƴ���
				m_nChipTimeCount++;

				//����˳�
				if (lMaxChipLmt < m_RobotInfo.nChip[m_nChipLimit[0]])	return false;
				for (int i = 0; i < AREA_COUNT; i++)
				{
					if (m_lAreaChip[i] >= m_RobotInfo.nChip[m_nChipLimit[0]])	break;
					if (i == AREA_COUNT-1)	return false;
				}
				for (int i = 0; i <AREA_COUNT; i++)
				{
					if (m_lAreaChip[i] >= m_RobotInfo.nChip[m_nChipLimit[0]])	break;
					if (i == AREA_COUNT-1)	return false;
				}
				//��ע����
				ASSERT(nACTotal>0);
				static int nStFluc = 1;				//�������
				if (nACTotal <= 0)	return false;
				do {
					nRandNum = (rand()+wMyID+nStFluc*3) % nACTotal;
					for (int i = 0; i < AREA_COUNT; i++)
					{
						nRandNum -= m_RobotInfo.nAreaChance[i];
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
						LONGLONG lLeftJetton = LONGLONG( (lMaxChipLmt-m_RobotInfo.nChip[nCurChip])/(m_nChipTime-m_nChipTimeCount) );

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
				PlaceJetton.cbJettonArea = nChipArea+1;		//������1��ʼ
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
	case SUB_S_WAIT_BANKER:			//�ȴ�ׯ��
		{
			return true;
		}
	case SUB_S_ADMIN_COMMDN:		//����
		{
			return true;
		}
	case SUB_S_ROBOT_BANKER:		//��ׯ֪ͨ
		{
			return OnSubRobotBanker(pBuffer, wDataSize);
		}

	}

	//�������
	ASSERT(FALSE);

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
			m_nJettonMultiple= pStatusFree->nMultiple;
			if(m_nJettonMultiple<1) m_nJettonMultiple=1;
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
	case GAME_STATUS_PLAY:	//��Ϸ״̬
	case GS_GAME_END:		//����״̬
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
			m_nJettonMultiple= pStatusPlay->nMultiple;
			if(m_nJettonMultiple<1) m_nJettonMultiple=1;

			memcpy(m_szRoomName, pStatusPlay->szGameRoomName, sizeof(m_szRoomName));

			ReadConfigInformation(&pStatusPlay->CustomAndroid);

			return true;
		}
	}

	return true;
}

//�û�����
void  CAndroidUserItemSink::OnEventUserEnter(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

//�û��뿪
void  CAndroidUserItemSink::OnEventUserLeave(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

//�û�����
void  CAndroidUserItemSink::OnEventUserScore(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

//�û�״̬
void  CAndroidUserItemSink::OnEventUserStatus(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

//��Ϸ����
bool CAndroidUserItemSink::OnSubGameFree(const void * pBuffer, WORD wDataSize)
{
	//��Ϣ����
	CMD_S_GameFree* pGameFree=(CMD_S_GameFree *)pBuffer;

	m_cbTimeLeave = pGameFree->cbTimeLeave;

	//���в���
	if (pGameFree->cbTimeLeave > 1)
		m_pIAndroidUserItem->SetGameTimer(IDI_BANK_OPERATE_SAVE, (rand() % (pGameFree->cbTimeLeave-1)) + 1);

	bool bMeGiveUp = false;
	if (m_wCurrentBanker == m_pIAndroidUserItem->GetChairID())
	{
		m_nBankerCount++;
		
		bool bCanGiveUpBanker=false;
		if ( m_nBankerCount >= m_nRobotBankerCount )
		{
			bCanGiveUpBanker=true;
		}

// 		//�����ׯ
// 		if(m_lBankerGetScore>m_lRobotGiveUpWinMin&&m_lBankerGetScore<m_lRobotGiveUpWinMax)
// 		{
// 			if(rand()%2==0)
// 				bCanGiveUpBanker=true;
// 		}
// 
// 		if(m_lBankerGetScore>m_lRobotGiveUpWinMax)
// 			bCanGiveUpBanker=true;


		if(bCanGiveUpBanker)
		{
			//��������ׯ
			m_nBankerCount = 0;
			m_pIAndroidUserItem->SetGameTimer(IDI_GIVEUP_BANKER, rand()%2 + 1);

			bMeGiveUp = true;
		}



	}

	//�����ׯ
	if (m_wCurrentBanker != m_pIAndroidUserItem->GetChairID() || bMeGiveUp)
	{
		m_cbTimeLeave = pGameFree->cbTimeLeave - 3;
		m_pIAndroidUserItem->SetGameTimer(IDI_CHECK_BANKER, 3);
	}

	return true;
}

//��Ϸ��ʼ
bool CAndroidUserItemSink::OnSubGameStart(const void * pBuffer, WORD wDataSize)
{	
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_GameStart));
	if (wDataSize!=sizeof(CMD_S_GameStart)) return false;

	m_lUserScore=0;

	//��Ϣ����
	CMD_S_GameStart * pGameStart=(CMD_S_GameStart *)pBuffer;

	//�Լ���ׯ������ע������
	if (pGameStart->wBankerUser == m_pIAndroidUserItem->GetChairID() || pGameStart->nChipRobotCount <= 0)
		return true;

	m_wCurrentBanker = pGameStart->wBankerUser;

	//if (m_wCurrentBanker==INVALID_CHAIR)
	//	return true;

	//���ñ���
	m_lMaxChipBanker = pGameStart->lBankerScore*m_nJettonMultiple;
	m_lMaxChipUser = pGameStart->lUserMaxScore*m_nJettonMultiple;
	
		
	m_nChipTimeCount = 0;
	ZeroMemory(m_nChipLimit, sizeof(m_nChipLimit));
	for (int i = 0; i < AREA_COUNT; i++)
		m_lAreaChip[i] = m_lAreaLimitScore;

	//ϵͳ��ׯ
	if (pGameStart->wBankerUser == INVALID_CHAIR)
	{
		m_lMaxChipBanker = 2147483647/m_RobotInfo.nMaxTime;
	}
	else
		m_lMaxChipUser = __min( m_lMaxChipUser, m_lMaxChipBanker );

	if(pGameStart->nAndriodCount > 0)
		m_stlApplyBanker=pGameStart->nAndriodCount;
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
	if (m_nChipTime <= 0)	return false;								//��ȷ,2����������
	if (m_nChipTime > MAX_CHIP_TIME)	m_nChipTime = MAX_CHIP_TIME;	//�޶�MAX_CHIP����ע

	//���㷶Χ
	if (!CalcJettonRange(__min(m_lMaxChipBanker, m_lMaxChipUser), m_lRobotJettonLimit, m_nChipTime, m_nChipLimit))
		return true;

	//����ʱ��
	int nTimeGrid = int(pGameStart->cbTimeLeave-2)*800/m_nChipTime;		//ʱ���,ǰ2�벻��ע,����-2,800��ʾ��������עʱ�䷶Χǧ�ֱ�
	for (int i = 0; i < m_nChipTime; i++)
	{
		int nRandRage = int( nTimeGrid * i / (1500*sqrt((double)m_nChipTime)) ) + 1;		//������Χ
		nElapse = 2 + (nTimeGrid*i)/1000 + ( (rand()+wMyID)%(nRandRage*2) - (nRandRage-1) );
		ASSERT(nElapse>=2&&nElapse<=pGameStart->cbTimeLeave);
		if (nElapse < 2 || nElapse > pGameStart->cbTimeLeave)	continue;
		
		m_pIAndroidUserItem->SetGameTimer(IDI_PLACE_JETTON+i+1, nElapse);
	}
	return true;
}

//�û���ע
bool CAndroidUserItemSink::OnSubPlaceJetton(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_PlaceJetton));
	if (wDataSize!=sizeof(CMD_S_PlaceJetton)) return false;

	//��Ϣ����
	CMD_S_PlaceJetton * pPlaceJetton=(CMD_S_PlaceJetton *)pBuffer;

	//���ñ���
	m_lMaxChipBanker -= pPlaceJetton->lJettonScore;
	m_lAreaChip[pPlaceJetton->cbJettonArea-1] -= pPlaceJetton->lJettonScore;
	if (pPlaceJetton->wChairID == m_pIAndroidUserItem->GetChairID())
		m_lMaxChipUser -= pPlaceJetton->lJettonScore;

	return true;
}

//��עʧ��
bool CAndroidUserItemSink::OnSubPlaceJettonFail(const void * pBuffer, WORD wDataSize)
{
	return true;
}

//��ׯ֪ͨ
bool CAndroidUserItemSink::OnSubRobotBanker(const void * pBuffer, WORD wDataSize)
{
	if (!m_bRobotBanker)
		return true;

	//�������
	IServerUserItem *pUserItem = m_pIAndroidUserItem->GetMeUserItem();
	LONGLONG lRobotScore = pUserItem->GetUserScore();
	if (m_pIAndroidUserItem->GetChairID() != m_wCurrentBanker && lRobotScore >= m_lBankerCondition)
	{
		m_pIAndroidUserItem->SetGameTimer(IDI_REQUEST_BANKER, (rand() % 5) + 1);
	}

	return true;
}

//��Ϸ����
bool CAndroidUserItemSink::OnSubGameEnd(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_GameEnd));
	if (wDataSize!=sizeof(CMD_S_GameEnd)) return false;

	//��Ϣ����
	CMD_S_GameEnd * pGameEnd=(CMD_S_GameEnd *)pBuffer;

	m_lBankerGetScore=pGameEnd->lBankerTotallScore;
	
	m_lUserScore=pGameEnd->lUserScore;

	m_cbTimeLeave = pGameEnd->cbTimeLeave;

	//���в���
	if (pGameEnd->cbTimeLeave > 1)
		m_pIAndroidUserItem->SetGameTimer(IDI_BANK_OPERATE_GET, (rand() % (pGameEnd->cbTimeLeave-1)) + 1);

	return true;
}

//������ׯ
bool CAndroidUserItemSink::OnSubUserApplyBanker(const void * pBuffer, WORD wDataSize)
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
bool CAndroidUserItemSink::OnSubUserCancelBanker(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_CancelBanker));
	if (wDataSize!=sizeof(CMD_S_CancelBanker)) return false;

	//��Ϣ����
	CMD_S_CancelBanker * pCancelBanker=(CMD_S_CancelBanker *)pBuffer;

	//�Լ��ж�
	if (pCancelBanker->wCancelUser == m_pIAndroidUserItem->GetUserID()) 
		m_bMeApplyBanker=false;

	return true;
}

//�л�ׯ��
bool CAndroidUserItemSink::OnSubChangeBanker(const void * pBuffer, WORD wDataSize)
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

//��ȡ����
void CAndroidUserItemSink::ReadConfigInformation(tagCustomAndroid *pCustomAndroid)
{
	m_nRobotBetTimeLimit[0] = pCustomAndroid->lRobotMinBetTime;
	m_nRobotBetTimeLimit[1] = pCustomAndroid->lRobotMaxBetTime;
	if (m_nRobotBetTimeLimit[0] < 0) m_nRobotBetTimeLimit[0] = 0;
	if (m_nRobotBetTimeLimit[1] < m_nRobotBetTimeLimit[0]) m_nRobotBetTimeLimit[1] = m_nRobotBetTimeLimit[0];

	//��������
	m_lRobotJettonLimit[0] = pCustomAndroid->lRobotMinJetton;
	m_lRobotJettonLimit[1] = pCustomAndroid->lRobotMaxJetton;
	if (m_lRobotJettonLimit[1] > 100000000)					m_lRobotJettonLimit[1] = 100000000;
	if (m_lRobotJettonLimit[0] < 100)						m_lRobotJettonLimit[0] = 100;
	if (m_lRobotJettonLimit[1] < m_lRobotJettonLimit[0])	m_lRobotJettonLimit[1] = m_lRobotJettonLimit[0];

	//�Ƿ���ׯ
	m_bRobotBanker = (pCustomAndroid->nEnableRobotBanker == TRUE)?true:false;

	//��ׯ����
	LONGLONG lRobotBankerCountMin = pCustomAndroid->lRobotBankerCountMin;
	LONGLONG lRobotBankerCountMax = pCustomAndroid->lRobotBankerCountMax;
	m_nRobotBankerCount = rand()%(lRobotBankerCountMax-lRobotBankerCountMin+1) + lRobotBankerCountMin;
	if(m_nRobotBankerCount<0) m_nRobotBankerCount=8;
	// 	//�б�����
	//	m_nMinRobotListCount = pCustomAndroid->lRobotListMinCount;

	//������
	m_nRobotApplyBanker = pCustomAndroid->lRobotApplyBanker;

	//��������
	m_nRobotWaitBanker = pCustomAndroid->lRobotWaitBanker;

	//��������
	m_lRobotScoreRange[0] = pCustomAndroid->lRobotScoreMin;
	m_lRobotScoreRange[1] = pCustomAndroid->lRobotScoreMax;
	if (m_lRobotScoreRange[1] < m_lRobotScoreRange[0])	m_lRobotScoreRange[1] = m_lRobotScoreRange[0];

	//�������
	LONGLONG lRobotBankGetScoreMin = pCustomAndroid->lRobotBankGetMin;
	LONGLONG lRobotBankGetScoreMax = pCustomAndroid->lRobotBankGetMax;
	m_lRobotBankGetScore=lRobotBankGetScoreMin+(rand()%(lRobotBankGetScoreMax-lRobotBankGetScoreMin+1))/10*10;
	//������� (ׯ��)
	LONGLONG lRobotBankGetScoreBankerMin = pCustomAndroid->lRobotBankGetBankerMin;
	LONGLONG lRobotBankGetScoreBankerMax = pCustomAndroid->lRobotBankGetBankerMax;
	m_lRobotBankGetScoreBanker = lRobotBankGetScoreBankerMin+(rand()%(lRobotBankGetScoreBankerMax-lRobotBankGetScoreBankerMin+1))/10*10;

	//����
	m_nRobotBankStorageMul = pCustomAndroid->lRobotBankStoMul;
	if (m_nRobotBankStorageMul<0 || m_nRobotBankStorageMul>100) m_nRobotBankStorageMul = 20;
}

//���㷶Χ	(����ֵ��ʾ�Ƿ����ͨ���½����޴ﵽ��ע)
bool CAndroidUserItemSink::CalcJettonRange(LONGLONG lMaxScore, LONGLONG lChipLmt[], int & nChipTime, int lJetLmt[])
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
	if (m_bReduceJettonLimit)
	{
		if (nChipTime * m_RobotInfo.nChip[lJetLmt[0]] > lMaxScore)
		{
			//�Ƿ񽵵���ע����
			if (nChipTime * m_RobotInfo.nChip[0] > lMaxScore)
			{
				nChipTime = int(lMaxScore/m_RobotInfo.nChip[0]);
				lJetLmt[0] = 0;
				lJetLmt[1] = 0;
			}
			else
			{
				//���͵���������
				while (nChipTime * m_RobotInfo.nChip[lJetLmt[0]] > lMaxScore)
				{
					lJetLmt[0]--;
					ASSERT(lJetLmt[0]>=0);
				}
			}
		}
	}

	return true;
}

//��ȡ����
LONGLONG CAndroidUserItemSink::GetPrivateProfileInt64(LPCTSTR lpAppName, LPCTSTR lpKeyName, LONGLONG lDefault, LPCTSTR lpFileName)
{
	//��������
	TCHAR OutBuf[255] = _T("");
	TCHAR DefaultBuf[255] = {};
	LONGLONG lNumber = 0;

	GetPrivateProfileString(lpAppName, lpKeyName, DefaultBuf, OutBuf, 255, lpFileName);

	if (OutBuf[0] != 0)
		myscanf(OutBuf, CountArray(OutBuf), _T("%I64d"), &lNumber);
	else
		lNumber = lDefault;

	return lNumber;
}

//�����������
DECLARE_CREATE_MODULE(AndroidUserItemSink);

//////////////////////////////////////////////////////////////////////////
