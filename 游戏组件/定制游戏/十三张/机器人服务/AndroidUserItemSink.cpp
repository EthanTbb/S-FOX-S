#include "Stdafx.h"
#include "AndroidUserItemSink.h"
#include "math.h"
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////

//���캯��
CAndroidUserItemSink::CAndroidUserItemSink()
{
	//ʱ�䶨��
	m_cbTimeStartGame = 0;
	m_cbTimeCallBanker = 0;
	m_cbTimeSetChip = 0;
	m_cbTimeRangeCard = 0;
	m_cbTimeShowCard = 0;
    m_cbCompareTime = 0;

	//��Ϸ����
	m_wBanker=INVALID_CHAIR;
	m_lBankerScore = 0;
	m_lUserChip = 0 ;				


	//�˿˱���
	m_bHandCardCount=0;
	ZeroMemory(m_bHandCardData,sizeof(m_bHandCardData));
	ZeroMemory(m_bSegmentCard , sizeof(m_bSegmentCard)) ;
	m_tagSegInfo.Init();
	m_bFinishSegment = false;


	//�ӿڱ���
	m_pIAndroidUserItem=NULL;

	

	

	return;
}

//��������
CAndroidUserItemSink::~CAndroidUserItemSink()
{
}

//�ӿڲ�ѯ
VOID * CAndroidUserItemSink::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
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
	//��Ϸ����
	m_wBanker=INVALID_CHAIR;
	m_lBankerScore = 0;
	m_lUserChip = 0 ;				

	//�˿˱���
	m_bHandCardCount=0;
	ZeroMemory(m_bHandCardData,sizeof(m_bHandCardData));
	ZeroMemory(m_bSegmentCard , sizeof(m_bSegmentCard)) ;
	m_tagSegInfo.Init();
	m_bFinishSegment = false;

	return true;
}

//ʱ����Ϣ
bool CAndroidUserItemSink::OnEventTimer(UINT nTimerID)
{
	switch (nTimerID)
	{
	case IDI_START_GAME:	//��ʼ��Ϸ
		{
			//��ʼ�ж�
			m_pIAndroidUserItem->SendUserReady(NULL,0);

			_DebugOutPut(TEXT("User:%s ChairID:%d  to Ready")
				, m_pIAndroidUserItem->GetMeUserItem()->GetNickName()
				, m_pIAndroidUserItem->GetChairID());
			return true;
		}
	case IDI_CALL_BANKER:   //�жϽ�ׯ
		{
			int nCall = rand() % 100;
			int nProbability = 0;
			if(TRUE == m_bIsWin)
			{
				nProbability = (int)(70 * pow(1.42,1.0 * m_bCallCount / m_bPlayCount));
			}
			else
			{
				nProbability = (int)(15 * pow(6.6,1.0 * m_bCallCount / m_bPlayCount));
			}
			CString sdp;
			sdp.Format(_T("kz nProbability = %d nCall = %d m_bCallCount = %d m_bPlayCount = %d"),nProbability,nCall,m_bCallCount,m_bPlayCount);
			OutputDebugString(sdp);
			if (nCall < nProbability)
			{
				m_pIAndroidUserItem->SendSocketData(SUB_C_CALL_BANKER);
				m_wBanker = m_pIAndroidUserItem->GetChairID();
				m_lBankerScore = m_pIAndroidUserItem->GetMeUserItem()->GetUserScore();
			}
			else
			{
				m_pIAndroidUserItem->SendSocketData(SUB_C_NO_CALL_BANKER);
			}
		
			_DebugOutPut(TEXT("User:%s ChairID:%d to CallBanker %d")
				, m_pIAndroidUserItem->GetMeUserItem()->GetNickName()
				, m_pIAndroidUserItem->GetChairID()
				, nCall);
			
			return true;
		}
	case IDI_SET_CHIP:  //������ע
		{
			//�����ע
			LONGLONG lChipArray[4];
			LONGLONG lBankerScore, lMeScore, lMaxChip;
			lBankerScore = m_lBankerScore;

			lMeScore = m_pIAndroidUserItem->GetMeUserItem()->GetUserScore() ;
			lMaxChip = (min(lMeScore,  lBankerScore))/36;
			for (BYTE cbChipIndex = 0; cbChipIndex < 4; ++cbChipIndex)
			{
				lChipArray[cbChipIndex] = LONGLONG ((cbChipIndex + 1.0 ) / 4 * lMaxChip) ;
			}

			//����ѹע����
			int index;
			IServerUserItem *pIServerUserItem = m_pIAndroidUserItem->GetTableUserItem(m_wBanker);
			if( pIServerUserItem != NULL && pIServerUserItem->IsAndroidUser())
			{
				int iRand = rand() % 100;
				if(iRand < 10)
				{
					index = 0;
				}
				else if(iRand < 30)
				{
					index = 1;
				}
				else if(iRand < 60)
				{
					index = 2;
				}
				else
				{
					index = 3;
				}
			}
			else
			{
				int iRand = rand() % 100;
				if(iRand < 20)
				{
					index = 0;
				}
				else if(iRand < 60)
				{
					index = 1;
				}
				else if(iRand < 90)
				{
					index = 2;
				}
				else
				{
					index = 3;
				}
			}
			m_lUserChip = lChipArray[index];

			CMD_C_SetChip  SetChip ;
			ZeroMemory(&SetChip, sizeof(SetChip)) ;
			SetChip.nChip = m_lUserChip;
			m_pIAndroidUserItem->SendSocketData(SUB_C_SETCHIP, &SetChip, sizeof(SetChip));

			_DebugOutPut(TEXT("User:%s ChairID:%d to SetChip %d")
				, m_pIAndroidUserItem->GetMeUserItem()->GetNickName()
				, m_pIAndroidUserItem->GetChairID()
				, m_lUserChip);

			return true;

		}
	case IDI_DISPATCHCARDFINISH:  //ϴ�����
		{

			//��������
			BYTE cbSegCardData[3][5] = {0};
			BYTE cbCardType = m_GameLogic.GetSpecialCardData(m_bHandCardData, m_bHandCardCount, cbSegCardData);
			if (cbCardType > CT_EX_INVALID) 
			{
				m_tagSegInfo.SetSegData(0, cbSegCardData[0], 3);
				m_tagSegInfo.SetSegData(1, cbSegCardData[1], 5);
				m_tagSegInfo.SetSegData(2, cbSegCardData[2], 5);


				//��ֹ����
				ASSERT (enCRGreater == m_GameLogic.CompareCard(m_tagSegInfo.bSegData[0], m_tagSegInfo.bSegData[1], 3, 5, false));
				ASSERT (enCRGreater == m_GameLogic.CompareCard(m_tagSegInfo.bSegData[1], m_tagSegInfo.bSegData[2], 5, 5, true));

				CMD_C_ShowCard AllSegmentCard;
				ZeroMemory(&AllSegmentCard, sizeof(AllSegmentCard));

				m_tagSegInfo.GetSegData(0, AllSegmentCard.bFrontCard, 3);
				m_GameLogic.SortCardList(AllSegmentCard.bFrontCard, 3) ;

				m_tagSegInfo.GetSegData(1, AllSegmentCard.bMidCard, 5);
				m_GameLogic.SortCardList(AllSegmentCard.bMidCard, 5);

				m_tagSegInfo.GetSegData(2, AllSegmentCard.bBackCard, 5);
				m_GameLogic.SortCardList(AllSegmentCard.bBackCard, 5);


				m_pIAndroidUserItem->SendSocketData(SUB_C_SHOWARD, &AllSegmentCard, sizeof(AllSegmentCard)) ;

				return true;

			}
			//��ʼ����
			ASSERT(m_cbTimeRangeCard != 0);
			UINT nElapse= rand() % (m_cbTimeRangeCard/2) + TIME_LESS;

			m_pIAndroidUserItem->SetGameTimer(IDI_RANGE_CARD,nElapse);

			return true;
		}
	case IDI_RANGE_CARD:  //�Զ�����
		{
			AutomaticOutCard();

			_DebugOutPut(TEXT("User:%s ChairID:%d to OutPutCard")
				, m_pIAndroidUserItem->GetMeUserItem()->GetNickName()
				, m_pIAndroidUserItem->GetChairID()
				, m_lUserChip);

			return true;
		}
	case IDI_SHUFFLE_CARD: 	//���ϴ��
		{
			//������Ϣ
			m_pIAndroidUserItem->SendSocketData(SUB_C_SHUFFLE_FINISH);
			
			return true;
		}
	case IDI_COMPARECARD: //��ɱ���
		{
			m_pIAndroidUserItem->SendSocketData(SUB_C_COMPLETE_COMPARE , NULL , 0);

			_DebugOutPut(TEXT("User:%s ChairID:%d to ComparecardFinish")
				, m_pIAndroidUserItem->GetMeUserItem()->GetNickName()
				, m_pIAndroidUserItem->GetChairID()
				, m_lUserChip);

			return true;
		}
	}

	
	return false;
}


//��Ϸ��Ϣ
bool CAndroidUserItemSink::OnEventGameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	if (m_pIAndroidUserItem != NULL)
	{
		IServerUserItem * pIServerUserItem = m_pIAndroidUserItem->GetMeUserItem();
		if (pIServerUserItem != NULL)
		{
			_DebugOutPut(TEXT("OnEventGameMessage User:%s TableID:%d ChairID:%d wSubCmdID:%d")
				, m_pIAndroidUserItem->GetMeUserItem()->GetNickName()
				, m_pIAndroidUserItem->GetTableID()
				, m_pIAndroidUserItem->GetChairID()
				, wSubCmdID);
			//CString UserName = pIServerUserItem->GetNickName();
			//BYTE UserStates = pIServerUserItem->GetUserStatus();
			//WORD wChairID = pIServerUserItem->GetChairID();
			//WORD wTableID = pIServerUserItem->GetTableID();
			//_DebugOutPut(TEXT("Call OnEventGameMessage Function (wSubCmdID :%d)")
			//	, wSubCmdID);
			//_DebugOutPut(TEXT("User(TableID:%d, ChairID:%d, Name:%s States:%d )")
			//	, wTableID
			//	, wChairID
			//	, UserName.GetBuffer()
			//	, UserStates );
		}

	}
	switch (wSubCmdID)
	{
	case SUB_S_START_CALL_BANKER:		//��ʼ��ׯ
		{
			return OnSubBeginCall(pData, wDataSize);
		}
	case SUB_S_CALL_BANKER :			//��ҽ�ׯ
		{
			return OnSubUserCall(pData, wDataSize);
		}
	case SUB_S_END_ALL_NO_CALL:			//������ׯ
		{
			return OnSubAllNotCall(pData, wDataSize);
		}
	case SUB_S_SEND_CARD_EX:			//�����˿�
		{
			return OnSubSendCardEx(pData,wDataSize);
		}
	case SUB_S_SETCHIP:					//����ѹע
		{
			return OnSubSetChip(pData , wDataSize) ;
		}
	case SUB_S_COMPARE_CARD:			//�Ƚ��˿�
		{
			return OnSubCompareCard(pData , wDataSize) ;
		}
	case SUB_S_GAME_END:				//��Ϸ����
		{
			return OnSubGameEnd(pData,wDataSize);
		}	

	case SUB_S_DISPATCH_START:			//��ʼ����
		{
			return OnSubDispatchStart(pData, wDataSize);
		}
	case SUB_S_DISPATCH_FINISH:			//���ƽ���
		{
			return true;
		}
	case SUB_S_ROBOT_CONTROL:           //�����˿���
		{
			if (m_pIAndroidUserItem->GetMeUserItem()->GetUserStatus() != US_PLAYING )
			{
				return true;
			}
			//������֤
			ASSERT( sizeof( CMD_A_Control ) == wDataSize ) ;
			if ( sizeof( CMD_A_Control ) != wDataSize ) return false ;

			CMD_A_Control *pControl = ( CMD_A_Control* )pData ;
			m_bIsWin = pControl->bIsWin;
			m_bPlayCount = pControl->bPlayCount;
		}
	default:
		{
			return true;
		}
	}

	return true;

}

//��Ϸ��Ϣ
bool CAndroidUserItemSink::OnEventFrameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	return true;
}

//������Ϣ
bool CAndroidUserItemSink::OnEventSceneMessage(BYTE cbGameStatus, bool bLookonOther, VOID * pData, WORD wDataSize)
{
	if (m_pIAndroidUserItem != NULL)
	{
		IServerUserItem * pIServerUserItem = m_pIAndroidUserItem->GetMeUserItem();
		if (pIServerUserItem != NULL)
		{
			CString UserName = pIServerUserItem->GetNickName();
			BYTE UserStates = pIServerUserItem->GetUserStatus();
			WORD wChairID = pIServerUserItem->GetChairID();
			WORD wTableID = pIServerUserItem->GetTableID();
			DWORD wUserId = pIServerUserItem->GetUserID();

			//CString info;
			//info.Format(TEXT("Call OnEventSceneMessage Function (cbGameStatus :%d, )"), cbGameStatus);
			//WRITE_LOG4C(info.GetBuffer());
			//_DebugOutPut(TEXT("Call OnEventSceneMessage Function (cbGameStatus :%d)")
			//	, cbGameStatus);
			_DebugOutPut(TEXT("OnEventSceneMessage cbGameStatus:%d User(TableID:%d, ChairID:%d, ID:%d States:%d )")
				, cbGameStatus
				, wTableID
				, wChairID
				, wUserId
				, UserStates );

		}
	}
	switch (cbGameStatus)
	{
	case GS_WK_FREE:	//����״̬
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_S_StatusFree));
			if (wDataSize!=sizeof(CMD_S_StatusFree)) return false;

			//��������
			CMD_S_StatusFree * pStatusFree=(CMD_S_StatusFree *)pData;
			IServerUserItem * pIServerUserItem=m_pIAndroidUserItem->GetMeUserItem();

			//ʱ�䶨��
			m_cbTimeStartGame = pStatusFree->cbTimeStartGame;
			m_cbTimeCallBanker = pStatusFree->cbTimeCallBanker;
			m_cbTimeSetChip = pStatusFree->cbTimeSetChip;
			m_cbTimeRangeCard = pStatusFree->cbTimeRangeCard;
			m_cbTimeShowCard = pStatusFree->cbTimeShowCard;
			
			//�����ʶ
			m_wServerID = pStatusFree->wServerID;
			
			//��ȡ����
			ReadConfigInformation();

			//���в���
			BankOperate(2);

			//�������
			if (pIServerUserItem->GetUserStatus()!=US_READY)
			{

				UINT nElapse=rand()%TIMER_START_GAME+TIME_LESS;
				m_pIAndroidUserItem->SetGameTimer(IDI_START_GAME,nElapse);
			}

			return true;
		}
	case GS_WK_CALL_BANKER:	//��ׯ״̬
		{
			//��֤����
			ASSERT( sizeof( CMD_S_StatusCallBanker ) == wDataSize ) ;
			if ( sizeof( CMD_S_StatusCallBanker ) != wDataSize ) return false ;


			CMD_S_StatusCallBanker  *pStatusCallBanker = ( CMD_S_StatusCallBanker * ) pData ;
			IServerUserItem * pIServerUserItem=m_pIAndroidUserItem->GetMeUserItem();

			//ʱ�䶨��
			m_cbTimeStartGame = pStatusCallBanker->cbTimeStartGame;
			m_cbTimeCallBanker = pStatusCallBanker->cbTimeCallBanker;
			m_cbTimeSetChip = pStatusCallBanker->cbTimeSetChip;
			m_cbTimeRangeCard = pStatusCallBanker->cbTimeRangeCard;
			m_cbTimeShowCard = pStatusCallBanker->cbTimeShowCard;
			
			//�����ʶ
			m_wServerID = pStatusCallBanker->wServerID;
			
			//��ȡ����
			ReadConfigInformation();

			//���в���
			BankOperate(2);

			//�������
			if (pStatusCallBanker->wCurrentCaller == m_pIAndroidUserItem->GetChairID()
				&& m_pIAndroidUserItem->GetMeUserItem()->GetUserStatus() == US_PLAYING )
			{
				UINT nElapse=rand()%m_cbTimeCallBanker + TIME_LESS;
				m_pIAndroidUserItem->SetGameTimer(IDI_CALL_BANKER,nElapse);
			}

			return true;
		}
	case GS_WK_SETCHIP:	//��ע״̬
		{
			//��֤����
			ASSERT( sizeof( CMD_S_StatusSetChip ) == wDataSize ) ;
			if ( sizeof( CMD_S_StatusSetChip ) != wDataSize ) return false ;

			CMD_S_StatusSetChip *pStatusSetChip=(CMD_S_StatusSetChip *)pData ;

			m_wBanker = pStatusSetChip->wBanker ;
			

			//ʱ�䶨��
			m_cbTimeStartGame = pStatusSetChip->cbTimeStartGame;
			m_cbTimeCallBanker = pStatusSetChip->cbTimeCallBanker;
			m_cbTimeSetChip = pStatusSetChip->cbTimeSetChip;
			m_cbTimeRangeCard = pStatusSetChip->cbTimeRangeCard;
			m_cbTimeShowCard = pStatusSetChip->cbTimeShowCard;
			
			//�����ʶ
			m_wServerID = pStatusSetChip->wServerID;
			
			//��ȡ����
			ReadConfigInformation();

			//���в���
			BankOperate(2);

			//�������
			if (m_pIAndroidUserItem->GetMeUserItem()->GetUserStatus() == US_PLAYING
				&& pStatusSetChip->wBanker != m_pIAndroidUserItem->GetChairID() )
			{
				UINT nElapse=rand()%m_cbTimeSetChip + TIME_LESS;
				m_pIAndroidUserItem->SetGameTimer(IDI_SET_CHIP,nElapse);
			}



			return true;
		}
	case GS_WK_PLAYING: //��Ϸ״̬
		{
			//��֤����
			ASSERT( sizeof( CMD_S_StatusPlay ) == wDataSize ) ;
			if ( sizeof( CMD_S_StatusPlay ) != wDataSize ) return false ;

			CMD_S_StatusPlay *pStatusPlay = (CMD_S_StatusPlay *)pData;

			m_wBanker = pStatusPlay->wBanker ;
			

			//ʱ�䶨��
			m_cbTimeStartGame = pStatusPlay->cbTimeStartGame;
			m_cbTimeCallBanker = pStatusPlay->cbTimeCallBanker;
			m_cbTimeSetChip = pStatusPlay->cbTimeSetChip;
			m_cbTimeRangeCard = pStatusPlay->cbTimeRangeCard;
			m_cbTimeShowCard = pStatusPlay->cbTimeShowCard;
			
			//�����ʶ
			m_wServerID = pStatusPlay->wServerID;
			
			//��ȡ����
			ReadConfigInformation();

			//���в���
			BankOperate(2);

			if (pStatusPlay->cbPlayCount == 2) m_cbCompareTime = 5;
			else if (pStatusPlay->cbPlayCount == 3) m_cbCompareTime = 6;
			else m_cbCompareTime = 7;

			m_cbCompareTime *= 3;


			CopyMemory(m_bHandCardData , pStatusPlay->bHandCardData , sizeof(m_bHandCardData));
			m_bHandCardCount = HAND_CARD_COUNT;
			CopyMemory(m_bSegmentCard , pStatusPlay->bSegmentCard , sizeof(m_bSegmentCard));
			//�������
			if (m_pIAndroidUserItem->GetMeUserItem()->GetUserStatus() == US_PLAYING )
			{
				UINT nElapse=rand()%m_cbTimeRangeCard + TIME_LESS;
				m_pIAndroidUserItem->SetGameTimer(IDI_RANGE_CARD,nElapse);

				bool bFinishiShowCard = true ;

				//���̯��
				for(WORD wID = 0 ; wID<GAME_PLAYER ; ++wID)
				{
					IServerUserItem * pIServerUserItem=m_pIAndroidUserItem->GetTableUserItem(wID);
					if(pIServerUserItem) 
					{
						if(false==pStatusPlay->bFinishSegment[wID])
						{
							bFinishiShowCard = false ;
							break ;
						}
					}
				}

				if(true==bFinishiShowCard)
				{
					m_pIAndroidUserItem->KillGameTimer(IDI_RANGE_CARD);

					UINT nElapse=m_cbCompareTime;
					m_pIAndroidUserItem->SetGameTimer(IDI_COMPARECARD,nElapse);
				}

				
			}

			return true;
		}
	}
	ASSERT(false);
	return true;
}

//�û�����
VOID CAndroidUserItemSink::OnEventUserEnter(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

//�û��뿪
VOID CAndroidUserItemSink::OnEventUserLeave(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

//�û�����
VOID CAndroidUserItemSink::OnEventUserScore(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

//�û�״̬
VOID CAndroidUserItemSink::OnEventUserStatus(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

//�û���λ
VOID CAndroidUserItemSink::OnEventUserSegment(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

bool CAndroidUserItemSink::OnSubBeginCall( const void * pBuffer, WORD wDataSize )
{
	if (m_pIAndroidUserItem->GetMeUserItem()->GetUserStatus() != US_PLAYING )
	{
		return true;
	}

	//������֤
	ASSERT( sizeof( CMD_S_StartCallBanker ) == wDataSize ) ;
	if ( sizeof( CMD_S_StartCallBanker ) != wDataSize ) return false ;

	CMD_S_StartCallBanker  *pStartCallBanker = ( CMD_S_StartCallBanker* )pBuffer ;
	IServerUserItem * pIServerUserItem=m_pIAndroidUserItem->GetMeUserItem();

	//��Ϸ״̬
	m_pIAndroidUserItem->SetGameStatus(GS_WK_CALL_BANKER);

	//��Ϸ����
	m_wBanker = INVALID_CHAIR;
	m_lBankerScore = 0;
	m_lUserChip = 0;

	ZeroMemory(m_bHandCardData, HAND_CARD_COUNT);
	ZeroMemory(m_bSegmentCard, sizeof(m_bSegmentCard));
	m_bHandCardCount = 0;
	m_tagSegInfo.Init();
	m_bFinishSegment = false;

	m_bCallCount = 1;
	CString sdp;
	sdp.Format(_T("kz 0 m_bCallCount = %d m_bPlayCount = %d"),m_bCallCount,m_bPlayCount);
	OutputDebugString(sdp);
	if (pStartCallBanker->wCurrentCaller ==  pIServerUserItem->GetChairID())
	{
		//������ʱ��
		UINT nElapse=rand()% m_cbTimeCallBanker;
		m_pIAndroidUserItem->SetGameTimer(IDI_CALL_BANKER,nElapse);
	}



	return true;
}

bool CAndroidUserItemSink::OnSubUserCall( const void * pBuffer, WORD wDataSize )
{
	if (m_pIAndroidUserItem->GetMeUserItem()->GetUserStatus() != US_PLAYING )
	{
		return true;
	}

	//������֤
	ASSERT( sizeof( CMD_S_CallBanker ) == wDataSize ) ;
	if ( sizeof( CMD_S_CallBanker ) != wDataSize ) return false ;

	CMD_S_CallBanker *pCallBanker = ( CMD_S_CallBanker* )pBuffer ;
	IServerUserItem * pIServerUserItem=m_pIAndroidUserItem->GetMeUserItem();

	

	//��ҽ�ׯ
	if (pCallBanker->bCallBanker)
	{
		if ( pCallBanker->wLastCaller != pIServerUserItem->GetChairID())
		{

			//����״̬
			m_pIAndroidUserItem->SetGameStatus(GS_WK_SETCHIP) ;

			//���ñ���
			m_wBanker = pCallBanker->wLastCaller ;
			m_lBankerScore = pCallBanker->lBankerScore;



			//������ע
			UINT nElapse=rand() % m_cbTimeSetChip + TIME_LESS;
			m_pIAndroidUserItem->SetGameTimer(IDI_SET_CHIP,nElapse);

		}


	}
	//��Ҳ���
	else
	{
		m_bCallCount ++;
		CString sdp;
		sdp.Format(_T("kz 1 m_bCallCount = %d m_bPlayCount = %d"),m_bCallCount,m_bPlayCount);
		OutputDebugString(sdp);

		if ( pCallBanker->wCurrentCaller == pIServerUserItem->GetChairID())
		{
			//������ʱ��
			UINT nElapse=rand()% m_cbTimeCallBanker;
			m_pIAndroidUserItem->SetGameTimer(IDI_CALL_BANKER,nElapse);
		}
	}
	return true;
}


bool CAndroidUserItemSink::OnSubAllNotCall( const void * pBuffer, WORD wDataSize )
{
	IServerUserItem * pIServerUserItem=m_pIAndroidUserItem->GetMeUserItem();
	//if (pIServerUserItem->GetUserStatus()!=US_READY)
	//{
		UINT nElapse=rand()%TIMER_START_GAME+TIME_LESS;
		m_pIAndroidUserItem->SetGameTimer(IDI_START_GAME,nElapse);
	//}

	return true;
}

//�����˿�
bool CAndroidUserItemSink::OnSubSendCardEx( const void * pBuffer, WORD wDataSize )
{
	if (m_pIAndroidUserItem->GetMeUserItem()->GetUserStatus() != US_PLAYING )
	{
		return true;
	}

	ASSERT(wDataSize == sizeof(CMD_S_SendCard));
	if (wDataSize != sizeof(CMD_S_SendCard)) return false;

	//��������
	CMD_S_SendCard * pSendCard = (CMD_S_SendCard *)pBuffer;
	IServerUserItem * pIServerUserItem = m_pIAndroidUserItem->GetMeUserItem();

	m_cbUserCount=pSendCard->cbPlayCount;

	//�����˿�
	if (pIServerUserItem->GetChairID() == pSendCard->wCurrentUser
		&& m_pIAndroidUserItem->GetMeUserItem()->GetUserStatus() == US_PLAYING)
	{
		m_bHandCardCount = HAND_CARD_COUNT;
		CopyMemory(m_bHandCardData, pSendCard->bCardData, sizeof(BYTE)*m_bHandCardCount);
		m_GameLogic.SortCardList(m_bHandCardData, m_bHandCardCount);

		//ϴ��ʱ��
		UINT nElapse=TIMER_SHUFFLECARD;
		m_pIAndroidUserItem->SetGameTimer(IDI_SHUFFLE_CARD,nElapse);
		


	}

	return true;
}

//��ʼ����
bool CAndroidUserItemSink::OnSubDispatchStart( const void * pBuffer, WORD wDataSize )
{
	if ( m_pIAndroidUserItem->GetMeUserItem()->GetUserStatus() != US_PLAYING)
	{
		return true;
	}

	//����״̬
	m_pIAndroidUserItem->SetGameStatus(GS_WK_PLAYING);



	UINT nElapse=TIMER_DISPATCHCARDFINISH;
	m_pIAndroidUserItem->SetGameTimer(IDI_DISPATCHCARDFINISH,nElapse);

	return true;
}

//�Զ�����
void CAndroidUserItemSink::AutomaticOutCard()
{

	IServerUserItem* pIServerUserItem = m_pIAndroidUserItem->GetMeUserItem();



	BYTE cbHandCard[MAX_COUNT];
	BYTE cbHandLen = 0;

	CopyMemory(cbHandCard, m_bHandCardData, m_bHandCardCount);
	cbHandLen = m_bHandCardCount;

	m_GameLogic.SortCardList(cbHandCard, cbHandLen, enDescend);
	m_bFinishSegment = false;

	m_tagSegInfo.Init();


	//�Զ�����
	BYTE bAutoPutCard[13], bIndex[13];
	ZeroMemory(bAutoPutCard, sizeof(bAutoPutCard));
	ZeroMemory(bIndex, sizeof(bIndex));

	BYTE cbType = m_GameLogic.GetSpecialType(cbHandCard, 13);
	if (cbType != CT_EX_INVALID)
	{
		BYTE cbSegCardData[3][5] = {0};
		if (0 != m_GameLogic.GetSpecialCardData(cbHandCard, cbHandLen, cbSegCardData))
		{
			CMD_C_ShowCard AllSegmentCard;
			ZeroMemory(&AllSegmentCard, sizeof(AllSegmentCard));

			CopyMemory(AllSegmentCard.bFrontCard, cbSegCardData[0], sizeof(BYTE) * 3);
			m_GameLogic.SortCardList(AllSegmentCard.bFrontCard, 3) ;

			CopyMemory(AllSegmentCard.bMidCard, cbSegCardData[1], sizeof(BYTE) * 5);
			m_GameLogic.SortCardList(AllSegmentCard.bMidCard, 5);

			CopyMemory(AllSegmentCard.bBackCard, cbSegCardData[2], sizeof(BYTE) * 5);
			m_GameLogic.SortCardList(AllSegmentCard.bBackCard, 5);

			m_pIAndroidUserItem->SendSocketData(SUB_C_SHOWARD, &AllSegmentCard, sizeof(AllSegmentCard)) ;

			return;
		}	
	}

	m_GameLogic.AutoPutCard(cbHandCard, bAutoPutCard, true, false);

	//��Ӧ����
	for (int i = 0; i < 13; i++)
	{
		for (int j = 0; j < 13; j++)
		{
			if (bAutoPutCard[i] == cbHandCard[j])
			{
				bIndex[i] = j;
				break;
			}
		}
	}

	BYTE tmpCard[13];
	ZeroMemory(tmpCard, sizeof(tmpCard));

	GetCardIndex(bIndex, tmpCard, 3);
	m_GameLogic.SortCardList(tmpCard, 3, enDescend) ;
	m_tagSegInfo.SetSegData(0, tmpCard, 3);

	
	//m_GameLogic.RemoveCard(tmpCard, 3, cbHandCard, cbHandLen);
	//cbHandLen -= 3;


	//��Ӧ����
	//for (int i = 3; i < 13; i++)
	//{
	//	for (int j = 0; j < 10; j++)
	//	{
	//		if (bAutoPutCard[i] == cbHandCard[j])
	//		{
	//			bIndex[i] = j;
	//			break;
	//		}
	//	}
	//}

	m_bFinishSegment=true ; 

	ZeroMemory(tmpCard, sizeof(tmpCard));

	GetCardIndex((bIndex + 3), tmpCard, 5);
	m_GameLogic.SortCardList(tmpCard, 5, enDescend) ;
	m_tagSegInfo.SetSegData(1, tmpCard, 5);

	
	//m_GameLogic.RemoveCard(tmpCard, 5, cbHandCard, cbHandLen);
	//cbHandLen -= 5;

	ZeroMemory(tmpCard, sizeof(tmpCard));
	GetCardIndex((bIndex + 8), tmpCard, 5);
	m_GameLogic.SortCardList(tmpCard, 5, enDescend) ;
	m_tagSegInfo.SetSegData(2, tmpCard, 5);

	//��ֹ����
	ASSERT (enCRLess != m_GameLogic.CompareCard(m_tagSegInfo.bSegData[0], m_tagSegInfo.bSegData[1], 3, 5, false));
	ASSERT (enCRLess != m_GameLogic.CompareCard(m_tagSegInfo.bSegData[1], m_tagSegInfo.bSegData[2], 5, 5, true));


	CMD_C_ShowCard AllSegmentCard;
	ZeroMemory(&AllSegmentCard, sizeof(AllSegmentCard));

	m_tagSegInfo.GetSegData(0, AllSegmentCard.bFrontCard, 3);
	m_GameLogic.SortCardList(AllSegmentCard.bFrontCard, 3) ;

	m_tagSegInfo.GetSegData(1, AllSegmentCard.bMidCard, 5);
	m_GameLogic.SortCardList(AllSegmentCard.bMidCard, 5);

	m_tagSegInfo.GetSegData(2, AllSegmentCard.bBackCard, 5);
	m_GameLogic.SortCardList(AllSegmentCard.bBackCard, 5);


	m_pIAndroidUserItem->SendSocketData(SUB_C_SHOWARD, &AllSegmentCard, sizeof(AllSegmentCard)) ;

}


void CAndroidUserItemSink::GetCardIndex( BYTE cbSrcIdx[], BYTE cbDstCard[], BYTE cbDstLen)
{
	if (m_bHandCardCount < cbDstLen)
	{
		return;
	}
	for (int i = 0 ; i < cbDstLen; i++)
	{
		cbDstCard[i] = m_bHandCardData[cbSrcIdx[i]];
	}

}

bool CAndroidUserItemSink::OnSubGameEnd( const void * pBuffer, WORD wDataSize )

{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_GameEnd));
	if (wDataSize!=sizeof(CMD_S_GameEnd)) return false;

	//��Ϣ����
	CMD_S_GameEnd * pGameEnd=(CMD_S_GameEnd *)pBuffer;
	

	//��Ϸ״̬
	m_pIAndroidUserItem->SetGameStatus(GS_WK_FREE);

	//ɾ��ʱ��
	m_pIAndroidUserItem->KillGameTimer(IDI_START_GAME);
	m_pIAndroidUserItem->KillGameTimer(IDI_SET_CHIP);
	m_pIAndroidUserItem->KillGameTimer(IDI_CALL_BANKER);
	m_pIAndroidUserItem->KillGameTimer(IDI_COMPARECARD);
	m_pIAndroidUserItem->KillGameTimer(IDI_RANGE_CARD);
	m_pIAndroidUserItem->KillGameTimer(IDI_SHUFFLE_CARD);
	m_pIAndroidUserItem->KillGameTimer(IDI_DISPATCHCARDFINISH);

	//��Ϸ����
	m_wBanker=INVALID_CHAIR;
	m_lBankerScore = 0;
	m_lUserChip = 0 ;				


	//�˿˱���
	m_bHandCardCount=0;
	ZeroMemory(m_bHandCardData,sizeof(m_bHandCardData));
	ZeroMemory(m_bSegmentCard , sizeof(m_bSegmentCard)) ;
	m_tagSegInfo.Init();
	m_bFinishSegment = false;


	UINT nElapse=rand()%TIMER_START_GAME+TIME_LESS;
	m_pIAndroidUserItem->SetGameTimer(IDI_START_GAME,nElapse);


	return true;
}


bool CAndroidUserItemSink::OnSubSetChip( const void * pBuffer , WORD wDataSize )
{
	if (m_pIAndroidUserItem->GetMeUserItem()->GetUserStatus() != US_PLAYING )
	{
		return true;
	}

	//������֤
	ASSERT(wDataSize==(sizeof(CMD_S_SetChip))) ;
	if(sizeof(CMD_S_SetChip)!=wDataSize) return false ;
	CMD_S_SetChip *pSetChip = (CMD_S_SetChip*)pBuffer ;

	if (pSetChip->wChipUser == m_pIAndroidUserItem->GetChairID())
	{
		m_lUserChip = 100;
	}

	return true;
}

bool CAndroidUserItemSink::OnSubCompareCard( const void * pBuffer , WORD wDataSize )
{
	if (m_pIAndroidUserItem->GetMeUserItem()->GetUserStatus() != US_PLAYING )
	{
		return true;
	}

	//������֤
	ASSERT(sizeof(CMD_S_CompareCard)==wDataSize) ;
	if(sizeof(CMD_S_CompareCard)!=wDataSize) return false ;

	CMD_S_CompareCard* pCMD_S_CompareCard = (CMD_S_CompareCard*)pBuffer;

	if(pCMD_S_CompareCard->bCompared == TRUE)
	{
		if (pCMD_S_CompareCard->cbPlayCount == 2) m_cbCompareTime = 5;
		else if (pCMD_S_CompareCard->cbPlayCount == 3) m_cbCompareTime = 6;
		else m_cbCompareTime = 7;

		UINT nElapse= m_cbCompareTime * 3;
		m_pIAndroidUserItem->SetGameTimer(IDI_COMPARECARD,nElapse);
	}

	
	return true;
}

//��ȡ����
void CAndroidUserItemSink::ReadConfigInformation()
{
	//�����ļ���
	TCHAR szPath[MAX_PATH] = TEXT("");
	TCHAR szConfigFileName[MAX_PATH] = TEXT("");
	TCHAR OutBuf[255] = TEXT("");
	GetCurrentDirectory(sizeof(szPath), szPath);
	myprintf(szConfigFileName, sizeof(szConfigFileName), _T("%s\\Thirteen.ini"), szPath);
	CString strServerID;
	strServerID.Format(TEXT("%d"),m_wServerID);
	
	//��������
	ZeroMemory(OutBuf, sizeof(OutBuf));
	GetPrivateProfileString(strServerID, TEXT("RobotScoreMin"), _T("100000"), OutBuf, 255, szConfigFileName);
	myscanf(OutBuf, mystrlen(OutBuf), _T("%I64d"), &m_lRobotScoreRange[0]);

	ZeroMemory(OutBuf, sizeof(OutBuf));
	GetPrivateProfileString(strServerID, TEXT("RobotScoreMax"), _T("50000000"), OutBuf, 255, szConfigFileName);
	myscanf(OutBuf, mystrlen(OutBuf), _T("%I64d"), &m_lRobotScoreRange[1]);
	if (m_lRobotScoreRange[1] < m_lRobotScoreRange[0])	m_lRobotScoreRange[1] = m_lRobotScoreRange[0];

	//�������
	ZeroMemory(OutBuf, sizeof(OutBuf));
	GetPrivateProfileString(strServerID, TEXT("RobotBankGet"), _T("1000000"), OutBuf, 255, szConfigFileName);
	myscanf(OutBuf, mystrlen(OutBuf), _T("%I64d"), &m_lRobotBankGetScore);

	//������� (ׯ��)
	ZeroMemory(OutBuf, sizeof(OutBuf));
	GetPrivateProfileString(strServerID, TEXT("RobotBankGetBanker"), _T("10000000"), OutBuf, 255, szConfigFileName);
	myscanf(OutBuf, mystrlen(OutBuf), _T("%I64d"), &m_lRobotBankGetScoreBanker);

	//����
	m_nRobotBankStorageMul = GetPrivateProfileInt(strServerID, _T("RobotBankStoMul"), 20, szConfigFileName);

	if (m_nRobotBankStorageMul<0||m_nRobotBankStorageMul>100) m_nRobotBankStorageMul =20;

	return;
}

//���в���
void CAndroidUserItemSink::BankOperate(BYTE cbType)
{
	IServerUserItem *pUserItem = m_pIAndroidUserItem->GetMeUserItem();
	if(pUserItem->GetUserStatus()>=US_SIT)
	{
		if(cbType==1)
		{
			/*CString strInfo;
			strInfo.Format(TEXT("������״̬���ԣ���ִ�д�ȡ��"));
			NcaTextOut(strInfo);*/
			return;
		}
	}
	
	//��������
	LONGLONG lRobotScore = pUserItem->GetUserScore();
	
	//�жϴ�ȡ
	if (lRobotScore > m_lRobotScoreRange[1])
	{			
		LONGLONG lSaveScore=0L;
		lSaveScore = LONGLONG(lRobotScore*m_nRobotBankStorageMul/100);
		if (lSaveScore > lRobotScore) lSaveScore = lRobotScore;
		if (lSaveScore > 0) m_pIAndroidUserItem->PerformSaveScore(lSaveScore);
	}
	else if (lRobotScore < m_lRobotScoreRange[0])
	{
		SCORE lScore=rand()%m_lRobotBankGetScoreBanker+m_lRobotBankGetScore;
		if (lScore > 0)
			m_pIAndroidUserItem->PerformTakeScore(lScore);
	}
	return;
}

//////////////////////////////////////////////////////////////////////////
DECLARE_CREATE_MODULE(AndroidUserItemSink);