#include "Stdafx.h"
#include "GameClient.h"
#include "GameClientEngine.h"

#include "Log4File.h"
//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameClientEngine, CGameFrameEngine)
		ON_WM_TIMER()
		ON_WM_CREATE()
		ON_MESSAGE(IDM_START, OnStart)
		ON_MESSAGE(IDM_CALL_BANKER, OnCallBanker)
		ON_MESSAGE(IDM_NO_CALL_BANKER, OnNoCallBanker)
		ON_MESSAGE(IDM_SETCHIP, OnSetChip)		
		ON_MESSAGE(IDM_SPLIT_CARD, OnSplitCard)
		ON_MESSAGE(IDM_SPLIT_FINISH, OnSplitFinish)
		ON_MESSAGE(IDM_SHUFFLE_FINISH, OnShuffleFinish)
		ON_MESSAGE(IDM_DISPATCH_CARD_FINISH, OnDispatchCardFinish)
		ON_MESSAGE(IDM_PROMPT, OnPrompt)
		ON_MESSAGE(IDM_VOICE_PHRASE, OnVoicePhrase)
		ON_MESSAGE(IDM_SORT_CARD, OnSortCard)	
		ON_MESSAGE(IDM_TRUSTEE, OnTrustee)		
		ON_MESSAGE(IDM_FRONTCARD, OnFrontCard)
		ON_MESSAGE(IDM_BACKCARD, OnBackCard)
		ON_MESSAGE(IDM_MIDCARD, OnMidCard)
		ON_MESSAGE(IDM_SHOWCARD, OnShowCard)
		ON_MESSAGE(IDM_LEFT_HIT_CARD, OnLeftHitCard)
		ON_MESSAGE(IDM_RESTORE, OnRestoreCard)
		ON_MESSAGE(IDM_REVERSAL_TURN_FINISH, OnRevesalTurnFinish)
		ON_MESSAGE(IDM_MSG_INFO, OnMsgInfo)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
//�궨��

//����ԭ��
#define GER_NORMAL						0x00							//�������
#define GER_DISMISS						0x01							//��Ϸ��ɢ
#define GER_USER_LEFT					0x02							//�û��뿪

//��Ϸ��ʱ��
#define IDI_START_GAME					200								//��ʼ��ʱ��
#define IDI_SETCHIP						201								//��ע��ʱ��
#define IDI_CALL_BANKER					202								//��ׯ��ʱ��
#define IDI_RANGE_CARD					203								//̯�ƶ�ʱ��
#define IDI_SHOW_CARD					209								//���ƶ�ʱ��
#define IDI_COMPARE_FRONT				204								//�Ƚ϶�ʱ��
#define IDI_COMPARE_MID					205								//�Ƚ϶�ʱ��
#define IDI_COMPARE_BACK				206								//�Ƚ϶�ʱ��
#define IDI_COMPLETE_COMPARE			207								//��ɶ�ʱ��
#define IDI_CAN_PLAY_VOICE				208								//������ʱ��

#define IDI_ALL_RANGE_CARD				209								//ͨ�ö�ʱ��

#define IDI_FU_ZHU						801								//������ʱ��
////////////////////////////////////////////////////////////////////////////////////

//���캯��
CGameClientEngine::CGameClientEngine() 
{
	
	//ʱ�䶨��
	m_cbTimeStartGame = 0;
	m_cbTimeCallBanker = 0;
	m_cbTimeSetChip = 0;
	m_cbTimeRangeCard = 0;
	m_cbTimeShowCard = 0;
	
	m_cbFuZhuTime=0;

	//��Ϸ����
	m_wBanker =	INVALID_CHAIR ; 
	for(WORD i=0 ; i<GAME_PLAYER ; ++i) m_lUserChip[i] = 0 ;
	m_bHandCardCount=0 ;
	ZeroMemory(m_bSegmentCard , sizeof(m_bSegmentCard)) ;
	ZeroMemory(m_lScoreTimes , sizeof(m_lScoreTimes)) ;
	m_wMaxChip=0 ;
	m_bFinishSegment=false ;
	m_bIsShowCard = false;
	m_tagSegInfo.Init();

	ZeroMemory(m_bGameStatus, sizeof(m_bGameStatus));
	m_bRequestSplit = false;
	m_bTrustee = false;

	ZeroMemory(&m_SearchCardResult, sizeof(m_SearchCardResult));
	m_cbSearchTypeIndex = 0;
	m_cbSortTypeIndex = 0;

	m_nCompareTime = 6000;
	m_bCanPlayVoice = true;
	m_bComparedCard = FALSE;

	m_bCheatRight = false;
	ZeroMemory(m_bUserCheat,sizeof(m_bUserCheat));

	return;
}

//��������
CGameClientEngine::~CGameClientEngine()
{
	if (AfxGetMainWnd())
		this->DestroyWindow();
}

//��ʼ����
bool CGameClientEngine::OnInitGameEngine()
{
	//����ͼ��
	HICON hIcon=LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME));
	m_pIClientKernel->SetGameAttribute(KIND_ID,GAME_PLAYER,VERSION_CLIENT,hIcon,GAME_NAME);
	SetIcon(hIcon,TRUE);
	SetIcon(hIcon,FALSE);

	//BYTE cbCardData[52];
	//m_GameLogic.RandCardList(cbCardData, 52);
	//BYTE cbHandData[13];
	//CopyMemory(cbHandData, cbCardData, 13);
	//m_GameLogic.SortCardList(cbHandData, 13);

	//m_GameClientView.SetCardData(2, cbHandData, 13, enCCTHandCard);
	//m_GameClientView.m_UserCardControl[2].ShowWindow(SW_SHOW);
	//m_GameClientView.m_UserCardControl[2].SetDisplayFlag(true);
	//CLog4CFile::WriteLog(AfxGetApp()->GetIDispatch())
	/*OUTPUT_LOG4C(TEXT("GameClientEngine To Initialize"));*/

	return true;
}

//���ÿ��
bool CGameClientEngine::OnResetGameEngine()
{
	//ʱ�䶨��
	m_cbTimeStartGame = 0;
	m_cbTimeCallBanker = 0;
	m_cbTimeSetChip = 0;
	m_cbTimeRangeCard = 0;
	m_cbTimeShowCard = 0;
	
	m_cbFuZhuTime=0;

	//��Ϸ����
	m_wBanker =	INVALID_CHAIR ;
	for(WORD i=0 ; i<GAME_PLAYER ; ++i) m_lUserChip[i] = 0 ;
	m_bHandCardCount=0 ;
	ZeroMemory(m_bSegmentCard , sizeof(m_bSegmentCard)) ;
	ZeroMemory(m_lScoreTimes , sizeof(m_lScoreTimes)) ;
	ZeroMemory(m_bHandCardData , sizeof(m_bHandCardData)) ;
	ZeroMemory(m_bTmpHandCardData , sizeof(m_bTmpHandCardData)) ;
	m_wMaxChip=0 ;
	m_tagSegInfo.Init();

	ZeroMemory(m_bGameStatus, sizeof(m_bGameStatus));
	m_bRequestSplit = false;
	m_bIsShowCard = false;
	m_bTrustee = false;

	ZeroMemory(&m_SearchCardResult, sizeof(m_SearchCardResult));
	m_cbSearchTypeIndex = 0;	
	m_cbSortTypeIndex = 0;

	//��������
	m_bCanPlayVoice = true;
	m_bComparedCard = FALSE;

	//�����˿�
	m_GameClientView.SetCardData(INVALID_CHAIR , NULL , 0 , enCCTHandCard) ;
	m_GameClientView.SetCardData(INVALID_CHAIR , NULL , 0 , enCCTAllSegmentCard) ;
	m_GameClientView.m_CenterCardControl.SetCardData(-1, NULL, 0);

	//���ý���
	m_GameClientView.SetBanker(INVALID_CHAIR) ;
	m_GameClientView.SetCallBankerStatus(INVALID_CHAIR, false) ;
	m_GameClientView.SetTrustee(INVALID_CHAIR, false);

	m_bCheatRight = false;
	ZeroMemory(m_bUserCheat,sizeof(m_bUserCheat));

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
	case IDI_START_GAME:		//��ʼ��Ϸ
		{
			if (!IsLookonMode() && (wChairID==GetMeChairID()) &&
				((nElapse <= 0) || (m_bTrustee && nElapse < (UINT)m_cbTimeStartGame-1)))
			{
				if (true == m_bTrustee)
				{
					OnStart(0, 0);
				}
				else
				{
					ASSERT(AfxGetMainWnd());
					if(AfxGetMainWnd()) AfxGetMainWnd()->PostMessage(WM_CLOSE);
					return false;
				}
			}
			
			if ((nElapse <= 5) && (wChairID == GetMeChairID()) && (false == IsLookonMode()))
				PlayGameSound(AfxGetInstanceHandle(), TEXT("GAME_WARN"));

			return true;
		}
	case IDI_CALL_BANKER :		//��ҽ�ׯ
		{
			if ((nElapse <= 0) || (m_bTrustee && nElapse < (UINT)m_cbTimeCallBanker-1))
			{
				if ((wChairID == GetMeChairID()) && (false == IsLookonMode()))
					OnNoCallBanker(0, 0);
				return true;
			}

			if ((nElapse <= 5) && (wChairID == GetMeChairID()) && (false == IsLookonMode()))
				PlayGameSound(AfxGetInstanceHandle(), TEXT("GAME_WARN")) ;

			return true;
		}
	case IDI_SETCHIP:			//������ע
		{
			if (wChairID != GetMeChairID() || IsLookonMode()) return true;

			if ((nElapse <= 0) || (m_bTrustee && nElapse < (UINT)m_cbTimeSetChip-1))
			{
				if ((wChairID == GetMeChairID()) && (false == IsLookonMode()))
					OnSetChip(0, 0);
				return true;
			}
			
			if ((nElapse <= 5) && (wChairID == GetMeChairID()) && (false == IsLookonMode()))
				PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_WARN"));
			
			return true;
		}
	case IDI_RANGE_CARD:         //�������
		{
			if ((nElapse <= 0) || (m_bTrustee && nElapse < (UINT)m_cbTimeRangeCard-1))
			{
				if ((wChairID == GetMeChairID()) && (false == IsLookonMode()))
				{
					//��δ���ƻ�����δ���
					if (m_bHandCardData[wChairID]!=0)
					{
						//�Զ�����
						AutomaticOutCard();
						//̯��
						OnShowCard(0 , 0);
					}
					else
					{
						//ֱ��̯��
						OnShowCard(0, 0);
					}
				}
				return false;
			}
			
			if ((nElapse<=5)&&(wChairID==GetMeChairID())) 
				PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_WARN"));
			
			return true;
		}
	case IDI_SHOW_CARD:
		{
			if ((nElapse <= 0) || (m_bTrustee && nElapse < (UINT)m_cbTimeShowCard-1))
			{
				if ((wChairID == GetMeChairID()) && (false == IsLookonMode()))
				{
					//������ã�����ʱ�����Զ�����
					OnShowCard(0, 0);
				}
				return false;
			}
			return true;
		}
	}

	return false;
}
//�û�����
VOID CGameClientEngine::OnEventUserEnter(IClientUserItem * pIClientUserItem, bool bLookonUser)
{
	if (bLookonUser==false)
	{
		//���½���
		m_GameClientView.InvalidGameView(0,0,0,0);
	}
	return;
}

//�û��뿪
VOID CGameClientEngine::OnEventUserLeave(IClientUserItem * pIClientUserItem, bool bLookonUser)
{
	if (bLookonUser==false)
	{
		//���½���
		m_GameClientView.InvalidGameView(0,0,0,0);
	}
	return;
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
	case SUB_S_SETSEGMENT:				//���÷ֶ�
		{
            return OnSubSetSegment(pData , wDataSize) ;
		}
	case SUB_S_SHOW_CARD:				//���̯��
		{
			return OnSubShowCard(pData , wDataSize) ;
		}
	case SUB_S_COMPARE_CARD:			//�Ƚ��˿�
		{
			return OnSubCompareCard(pData , wDataSize) ;
		}
	case SUB_S_GAME_END:				//��Ϸ����
		{
			return OnSubGameEnd(pData,wDataSize);
		}	
	case SUB_S_PLAYER_EXIT:				//�û�ǿ��
		{
			return OnSubPlayerExit(pData,wDataSize);
		}
	case SUB_S_DISPATCH_START:			//��ʼ����
		{
			return OnSubDispatchStart(pData, wDataSize);
		}
	case SUB_S_DISPATCH_FINISH:			//���ƽ���
		{
			return true;
		}
	case SUB_S_SPLIT_CARD:				//������Ϣ
		{
			return OnSubSplitCard(pData, wDataSize);
		}
	case SUB_S_TRUSTEE:					//�йܿ���
		{
			return	OnSubTrustee(pData, wDataSize);
		}
	case SUB_S_VOICE_PHRASE:			//�����㲥
		{
			return OnSubVoicePhrase(pData, wDataSize);
		}
	case SUB_S_CHEAT_CARD:				//��������	
		{
			return OnSubCheatCard(pData,wDataSize);
		}
    case SUB_S_ROBOT_CONTROL:           //�����˿���
		{
			return true;
		}
	}
	
	return false;
}

//��Ϸ����
bool CGameClientEngine::OnEventSceneMessage(BYTE cbGameStatus, bool bLookonUser, VOID * pData, WORD wDataSize)
{
	switch (cbGameStatus)
	{
	case GS_WK_FREE:	//����״̬
		{
			//Ч������
			ASSERT(wDataSize == sizeof(CMD_S_StatusFree));
			if (wDataSize != sizeof(CMD_S_StatusFree)) return false;

			//��������
			CMD_S_StatusFree* pStatusFree = (CMD_S_StatusFree*)pData;

			//ʱ�䶨��
			m_cbTimeStartGame = pStatusFree->cbTimeStartGame;
			m_cbTimeCallBanker = pStatusFree->cbTimeCallBanker;
			m_cbTimeSetChip = pStatusFree->cbTimeSetChip;
			m_cbTimeRangeCard = pStatusFree->cbTimeRangeCard;
			m_cbTimeShowCard = pStatusFree->cbTimeShowCard;
			
			//��ʷ�ɼ�
			for (WORD i = 0; i < GAME_PLAYER; i++)
			{
				//��ȡ����
				tagHistoryScore * pHistoryScore = m_HistroyScore.GetHistoryScore(i);

				//���û���
				pHistoryScore->lTurnScore = pStatusFree->lTurnScore[i];
				pHistoryScore->lCollectScore = pStatusFree->lCollectScore[i];

				//������
				WORD wViewChairID=SwitchViewChairID(i);
				m_GameClientView.SetHistoryScore(wViewChairID, pHistoryScore);
			}

			//����״̬
			CopyMemory(m_bGameStatus, pStatusFree->bGameStatus, sizeof(m_bGameStatus));
			for(BYTE i=0;i<GAME_PLAYER;i++)
			{
				m_GameClientView.SetGameStatus(SwitchViewChairID(i),m_bGameStatus[i]);
			}

			//���ÿؼ�
			if (IsLookonMode() == false)
			{
				

                UpdateButtonState();

				//�����˿�
				 m_GameClientView.m_UserCardControl[2].SetDisplayFlag(true);
				
				 //�����й�
				m_GameClientView.m_btTrustee.EnableWindow(TRUE);
				
				//��ʼ����
				if (GetMeUserItem()->GetUserStatus()!=US_READY)
				{
					tagServerAttribute *pServerAttribute=m_pIClientKernel->GetServerAttribute();
					bool bShowStart = (0 == (pServerAttribute->wServerType & GAME_GENRE_MATCH) &&
						0 == (pServerAttribute->dwServerRule & SR_ALLOW_AVERT_CHEAT_MODE));

					m_GameClientView.m_btStart.ShowWindow(bShowStart);
					m_GameClientView.m_btStart.EnableWindow(bShowStart) ;

					if(bShowStart)
					{
						m_GameClientView.m_btStart.SetFocus();

						//��ʼ��ʱ
						SetGameClock(GetMeChairID(),IDI_START_GAME,m_cbTimeStartGame);
					}
					
				}	
			}
			
			return true;
		}

	case GS_WK_CALL_BANKER :	//��ׯ״̬
		{
			//��֤����
			ASSERT( sizeof( CMD_S_StatusCallBanker ) == wDataSize ) ;
			if ( sizeof( CMD_S_StatusCallBanker ) != wDataSize ) return false ;

			//�ض�ʱ��
			m_GameClientView.StopDispatchCard();

			CMD_S_StatusCallBanker  *pStatusCallBanker = ( CMD_S_StatusCallBanker * ) pData ;
			
			if ( GetMeChairID() == pStatusCallBanker->wCurrentCaller  && !IsLookonMode() )
			{
				//���ð�ť
				m_GameClientView.m_btCallBanker.ShowWindow( SW_SHOW ) ;
				m_GameClientView.m_btNoCallBanker.ShowWindow( SW_SHOW ) ;
			}
			
			//��ʾ��Ϣ
			m_GameClientView.SetTipInfoState(1);

			//ʱ�䶨��
			m_cbTimeStartGame = pStatusCallBanker->cbTimeStartGame;
			m_cbTimeCallBanker = pStatusCallBanker->cbTimeCallBanker;
			m_cbTimeSetChip = pStatusCallBanker->cbTimeSetChip;
			m_cbTimeRangeCard = pStatusCallBanker->cbTimeRangeCard;
			m_cbTimeShowCard = pStatusCallBanker->cbTimeShowCard;

			//��ʷ�ɼ�
			for (WORD i = 0; i < GAME_PLAYER; i++)
			{
				//��ȡ����
				tagHistoryScore * pHistoryScore = m_HistroyScore.GetHistoryScore(i);

				//���û���
				pHistoryScore->lTurnScore = pStatusCallBanker->lTurnScore[i];
				pHistoryScore->lCollectScore = pStatusCallBanker->lCollectScore[i];

				//������
				WORD wViewChairID=SwitchViewChairID(i);
				m_GameClientView.SetHistoryScore(wViewChairID, pHistoryScore);
			}

			//����״̬
			CopyMemory(m_bGameStatus,pStatusCallBanker->bGameStatus,sizeof(m_bGameStatus));
			for(BYTE i=0;i<GAME_PLAYER;i++)
			{
				m_GameClientView.SetGameStatus(SwitchViewChairID(i),m_bGameStatus[i]);
			}

			if (m_bGameStatus[GetMeChairID()] == false || IsLookonMode() )
			{
				m_GameClientView.m_btTrustee.EnableWindow(FALSE);
			}
			//���ö�ʱ��
			SetGameClock( pStatusCallBanker->wCurrentCaller, IDI_CALL_BANKER, m_cbTimeCallBanker ) ;
			
			return true ;
		}

	case GS_WK_SETCHIP:		//��ע״̬
		{
			//��֤����
			ASSERT( sizeof( CMD_S_StatusSetChip ) == wDataSize ) ;
			if ( sizeof( CMD_S_StatusSetChip ) != wDataSize ) return false ;
			//�ض�ʱ��
			m_GameClientView.StopDispatchCard();

			CMD_S_StatusSetChip *pStatusSetChip=(CMD_S_StatusSetChip *)pData ;
			
			m_wBanker = pStatusSetChip->wBanker ;
			m_GameClientView.SetBanker(SwitchViewChairID(m_wBanker)) ;

			//ʱ�䶨��
			m_cbTimeStartGame = pStatusSetChip->cbTimeStartGame;
			m_cbTimeCallBanker = pStatusSetChip->cbTimeCallBanker;
			m_cbTimeSetChip = pStatusSetChip->cbTimeSetChip;
			m_cbTimeRangeCard = pStatusSetChip->cbTimeRangeCard;
			m_cbTimeShowCard = pStatusSetChip->cbTimeShowCard;

			//��ʷ�ɼ�
			for (WORD i = 0; i < GAME_PLAYER; i++)
			{
				//��ȡ����
				tagHistoryScore * pHistoryScore = m_HistroyScore.GetHistoryScore(i);

				//���û���
				pHistoryScore->lTurnScore = pStatusSetChip->lTurnScore[i];
				pHistoryScore->lCollectScore = pStatusSetChip->lCollectScore[i];

				//������
				WORD wViewChairID=SwitchViewChairID(i);
				m_GameClientView.SetHistoryScore(wViewChairID, pHistoryScore);
			}

			//�����ע
			m_GameClientView.SetMaxChip(m_wMaxChip) ;
		
			//������ע��ť
			UpdateBlockButton(0, SW_HIDE, FALSE);
	
			//����״̬
			CopyMemory(m_bGameStatus,pStatusSetChip->bGameStatus,sizeof(m_bGameStatus));			
			for(BYTE i=0;i<GAME_PLAYER;i++)
				m_GameClientView.SetGameStatus(SwitchViewChairID(i),m_bGameStatus[i]);

			//������ע
			CopyMemory(m_lUserChip , pStatusSetChip->lChip, sizeof(m_lUserChip)) ;
			for(WORD i=0 ; i<GAME_PLAYER ; ++i) 
				m_GameClientView.SetChip(SwitchViewChairID(i) , m_lUserChip[i]);

			//���ð�ť
			if(false==IsLookonMode() )
			{
				WORD wMeChairID = GetMeChairID();

				if(0==m_lUserChip[wMeChairID] && wMeChairID!=m_wBanker && m_bGameStatus[wMeChairID] != false)//(-1==m_lUserChip[wMeChairID] && wMeChairID!=m_wBanker && m_bGameStatus[wMeChairID] != false)
				{
					//��ע��С
					SCORE lChipsArray[SET_CHIP_COUNT] = {0};
					CopyMemory(lChipsArray, pStatusSetChip->lChipArray[wMeChairID], sizeof(lChipsArray));

					m_GameClientView.SetChipArray(lChipsArray) ;
                    
					UpdateChipButtonState(SW_SHOW, TRUE) ;
	
					//����ʱ��
					SetGameClock(wMeChairID , IDI_SETCHIP , m_cbTimeSetChip) ;
				}

				//���ø���ʱ��
				if (!IsLookonMode() && m_bGameStatus[GetMeChairID()] == true && !pStatusSetChip->bSetChipFinally)
				{
					for(BYTE i=0;i<GAME_PLAYER;i++)
					{
						//��ע��ɲ��ÿ�����ʱ��
						if(pStatusSetChip->bSetChipFinally) break;
						if(i != m_wBanker && i != GetMeChairID() && m_bGameStatus[i] == true)
						{
							m_GameClientView.ShowFuZhuClock(SwitchViewChairID(i),true);
						}
					}
					m_cbFuZhuTime=m_cbTimeSetChip;
					m_GameClientView.SetFuZhuTime(m_cbFuZhuTime--);
					//����ʱ��
					SetTimer(IDI_FU_ZHU, 1000, NULL);
				}

				//��ʾ��Ϣ
				m_GameClientView.SetTipInfoState(2);
			}
			else
			{
				m_GameClientView.m_btTrustee.EnableWindow(FALSE);
			}


			//_DebugOutPut(TEXT("isLookonMode : %d  gamestate:%d"), IsLookonMode(), m_bGameStatus[i]);

			return true ;
		}
	case GS_WK_PLAYING:		//��Ϸ״̬
		{
			//�ض�ʱ��
			m_GameClientView.StopDispatchCard();

			CMD_S_StatusPlay *pStatusPlay = (CMD_S_StatusPlay *)pData;

			m_wBanker = pStatusPlay->wBanker ;
			m_GameClientView.SetBanker(SwitchViewChairID(m_wBanker)) ;

			//ʱ�䶨��
			m_cbTimeStartGame = pStatusPlay->cbTimeStartGame;
			m_cbTimeCallBanker = pStatusPlay->cbTimeCallBanker;
			m_cbTimeSetChip = pStatusPlay->cbTimeSetChip;
			m_cbTimeRangeCard = pStatusPlay->cbTimeRangeCard;
			m_cbTimeShowCard = pStatusPlay->cbTimeShowCard;

			//��ʷ�ɼ�
			for (WORD i = 0; i < GAME_PLAYER; i++)
			{
				//��ȡ����
				tagHistoryScore * pHistoryScore = m_HistroyScore.GetHistoryScore(i);

				//���û���
				pHistoryScore->lTurnScore = pStatusPlay->lTurnScore[i];
				pHistoryScore->lCollectScore = pStatusPlay->lCollectScore[i];

				//������
				WORD wViewChairID=SwitchViewChairID(i);
				m_GameClientView.SetHistoryScore(wViewChairID, pHistoryScore);
			}

			//�����ע
			m_GameClientView.SetMaxChip(m_wMaxChip) ;
			UpdateChipButtonState(SW_HIDE, FALSE) ;

			bool bFinishiShowCard = true ;
			
			//���̯��
			for(WORD wID = 0 ; wID<GAME_PLAYER ; ++wID)
			{
				IClientUserItem * pClientUserItem=GetTableUserItem(wID);
				if(pClientUserItem) 
				{
					if(false==pStatusPlay->bFinishSegment[wID])
					{
						bFinishiShowCard = false ;
						break ;
					}
				}
			}
	
			//����״̬
			CopyMemory(m_bGameStatus,pStatusPlay->bGameStatus,sizeof(m_bGameStatus));			
			for(BYTE i=0;i<GAME_PLAYER;i++)
			{
				m_GameClientView.SetGameStatus(SwitchViewChairID(i),m_bGameStatus[i]);
			}

			CopyMemory(m_bHandCardData[GetMeChairID()] , pStatusPlay->bHandCardData , sizeof(m_bHandCardData[GetMeChairID()]));
			CopyMemory(m_bTmpHandCardData , m_bHandCardData[GetMeChairID()] , sizeof(m_bTmpHandCardData));
			m_bHandCardCount = HAND_CARD_COUNT;
			CopyMemory(m_bSegmentCard , pStatusPlay->bSegmentCard , sizeof(m_bSegmentCard));
			if(false==pStatusPlay->bFinishSegment[GetMeChairID()] && m_bGameStatus[GetMeChairID()] != false)	
			{
				m_GameClientView.SetCardData(2, m_bHandCardData[GetMeChairID()], m_bHandCardCount, enCCTHandCard);
			}

			if((false==IsLookonMode()) && (true==m_bGameStatus[GetMeChairID()]))
			{
				//������ʾ
				if (!pStatusPlay->bFinishSegment[GetMeChairID()])
				{
					UpdateButtonState();
				}
				
				//�����˿�
				m_GameClientView.m_UserCardControl[2].SetDisplayFlag(true) ;
				m_GameClientView.m_UserCardControl[2].SetPositively(true) ;

				if(true==pStatusPlay->bFinishSegment[GetMeChairID()])
				{
					UpdateBlockButton(0, SW_HIDE, FALSE);
				}
				else
				{
					UpdateBlockButton(0, SW_SHOW, FALSE);
				}
			}
			else
			{
				//�����й�
				m_GameClientView.m_btTrustee.EnableWindow(FALSE) ;

				//�����˿�
				m_GameClientView.m_UserCardControl[2].SetDisplayFlag(false) ;
				m_GameClientView.m_UserCardControl[2].SetPositively(false) ;
				UpdateBlockButton(0, SW_HIDE, FALSE);	
			}

			//û��̯��
			for(WORD wID = 0; wID<GAME_PLAYER; ++wID)
			{
				WORD wViewChairID = SwitchViewChairID(wID) ;
				IClientUserItem * pClientUserItem=GetTableUserItem(wID);

				if(NULL!=pClientUserItem)
				{
					WORD wMeChairID = GetMeChairID();

					//̯���˿�
					if (true==pStatusPlay->bFinishSegment[wID] && m_bGameStatus[wID] != false)
					{
						if (wID != wMeChairID || IsLookonMode())
						{
							BYTE bZeroCard[5]={0};
							ZeroMemory(bZeroCard, sizeof(bZeroCard));
							m_GameClientView.m_AllSegCardControl[wViewChairID].SetCardData(0, bZeroCard, 3);
							m_GameClientView.m_AllSegCardControl[wViewChairID].SetCardData(1, bZeroCard, 5);
							m_GameClientView.m_AllSegCardControl[wViewChairID].SetCardData(2, bZeroCard, 5);
							m_GameClientView.m_AllSegCardControl[wViewChairID].SetDisplayFlag(false);
						}
						else
						{
							m_GameClientView.m_AllSegCardControl[wViewChairID].SetCardData(0, m_bSegmentCard[wID][0], 3);
							m_GameClientView.m_AllSegCardControl[wViewChairID].SetCardData(1, m_bSegmentCard[wID][1], 5);
							m_GameClientView.m_AllSegCardControl[wViewChairID].SetCardData(2, m_bSegmentCard[wID][2], 5);
						}
					}
					//�����˿�
					else if (false==pStatusPlay->bFinishSegment[wID] && m_bGameStatus[wID] != false)
					{
						if (wID == wMeChairID)
							m_GameClientView.SetCardData(2, m_bHandCardData[wMeChairID], 13, enCCTHandCard);
						else
						{
							BYTE bCardData[13] = {0};
							ZeroMemory(bCardData, sizeof(bCardData));
							m_GameClientView.SetCardData(wViewChairID , bCardData , 13 , enCCTHandCard);
						}
					}
				}
			}

			CopyMemory(m_lUserChip , pStatusPlay->lChip , sizeof(m_lUserChip)) ;
			for(WORD i=0 ; i<GAME_PLAYER ; ++i) 
				m_GameClientView.SetChip(SwitchViewChairID(i) , m_lUserChip[i]) ;

			if(true==bFinishiShowCard)
			{
				KillTimer(m_nTimer) ;
				m_GameClientView.m_btTrustee.EnableWindow(FALSE) ;

				//ģ����Ϣ
				CMD_S_CompareCard compareCard;
				ZeroMemory(&compareCard, sizeof(compareCard));

				compareCard.wBankUser = m_wBanker;
				CopyMemory(compareCard.bSegmentCard, pStatusPlay->bSegmentCard, sizeof(compareCard.bSegmentCard));
				CopyMemory(compareCard.lScoreTimes, pStatusPlay->lScoreTimes, sizeof(compareCard.lScoreTimes));
				CopyMemory(compareCard.bSpecialType, pStatusPlay->bSpecialType, sizeof(compareCard.bSpecialType));
				compareCard.bCompared = pStatusPlay->bCompared;
				OnSubCompareCard((void*)&compareCard, sizeof(compareCard)) ;

				

			}
			//���ð�ť
			if(false==IsLookonMode() && m_bGameStatus[GetMeChairID()] != false)
			{
				if(-1==m_lUserChip[GetMeChairID()] && GetMeChairID()!=m_wBanker)
				{
					//��ע��С
					SCORE lChipsArray[SET_CHIP_COUNT] = {0};
					CopyMemory(lChipsArray, pStatusPlay->lChipArray[GetMeChairID()], sizeof(lChipsArray));

					m_GameClientView.SetChipArray( lChipsArray ) ;
                   
					UpdateChipButtonState(SW_SHOW, TRUE) ;
					m_GameClientView.SetGlobalClock(true);
					m_GameClientView.SetElapseTime(m_cbTimeRangeCard);
					m_TimerCount = m_cbTimeRangeCard;
					
					m_GameClientView.ShowCenterControl(true);
					//����ʱ��
					SetTimer(IDI_ALL_RANGE_CARD, 1000, NULL);
					//SetGameClock(GetMeChairID() , IDI_RANGE_CARD , m_cbTimeRangeCard) ;
				}
				else if(false==pStatusPlay->bFinishSegment[GetMeChairID()])
				{
					UpdateBlockButton(0, SW_SHOW, FALSE);
					m_GameClientView.SetGlobalClock(true);
					m_GameClientView.SetElapseTime(m_cbTimeRangeCard);
					m_TimerCount = m_cbTimeRangeCard;
					
					m_GameClientView.ShowCenterControl(true);
					//����ʱ��
					SetTimer(IDI_ALL_RANGE_CARD, 1000, NULL);
					//SetGameClock(GetMeChairID() , IDI_RANGE_CARD , m_cbTimeRangeCard) ;
				}
			}
			return true ;
		}
		default:
			return true;
	}	

	
}

//��ʼ��ť
LRESULT CGameClientEngine::OnStart(WPARAM wParam, LPARAM lParam)
{
	
	UNREFERENCED_PARAMETER(wParam) ;
	UNREFERENCED_PARAMETER(lParam) ;

	//ֹͣ����ʱ
	KillGameClock(IDI_START_GAME);

	//���ñ���
	for (WORD i=0 ; i<GAME_PLAYER ; ++i) m_lUserChip[i] = 0;
	m_bHandCardCount = 0;
	ZeroMemory(m_bHandCardData, sizeof(m_bHandCardData));
	ZeroMemory(m_bTmpHandCardData, sizeof(m_bTmpHandCardData));	
	ZeroMemory(m_bSegmentCard, sizeof(m_bSegmentCard));
	ZeroMemory(m_lScoreTimes , sizeof(m_lScoreTimes));
	m_tagSegInfo.Init();

	m_wBanker = INVALID_CHAIR;
	m_wMaxChip = 0;
	m_bFinishSegment = false;
	m_bIsShowCard = false;
	ZeroMemory(&m_SearchCardResult, sizeof(m_SearchCardResult));
	m_cbSearchTypeIndex = 0;	
	m_cbSortTypeIndex = 0;

	//ֹͣ����	
	m_GameClientView.StopShuffleCard();
	m_GameClientView.StopSplitCard();
	m_GameClientView.StopDispatchCard();
	m_GameClientView.StopSwatCartoon();
	m_GameClientView.StopGunCartoon();

	//������ͼ
	m_GameClientView.SetChip(INVALID_CHAIR , -1) ;
	m_GameClientView.SetGameScore(INVALID_CHAIR, -1);	
	m_GameClientView.SetScoreTimes(INVALID_CHAIR, MAXLONGLONG);
	m_GameClientView.SetCallBankerStatus(INVALID_CHAIR, false);
	m_GameClientView.ShowCenterControl(false);
	m_GameClientView.SetBanker(INVALID_CHAIR) ;
	m_GameClientView.SetCardTypeSpec(false,0,0);
	m_GameClientView.SetShowCardTip(false);

	//���ð�ť
	m_GameClientView.m_btStart.ShowWindow(SW_HIDE);
	m_GameClientView.m_btStart.EnableWindow(FALSE);

	if (m_bTrustee == false)
	{
		m_GameClientView.m_btTrustee.EnableWindow(TRUE);

	}
	m_GameClientView.m_UserCardControl[ME_VIEW_CHAIR].SetDisplayFlag(true) ;

	UpdateBlockButton(0, SW_HIDE, FALSE);

	//�����˿�
	m_GameClientView.SetCardData(INVALID_CHAIR , NULL, 0, enCCTHandCard) ;
	m_GameClientView.SetCardData(INVALID_CHAIR , NULL, 0, enCCTAllSegmentCard) ;
	m_GameClientView.m_CenterCardControl.SetCardData(-1, NULL, 0);

	//������Ϣ
	SendUserReady(NULL,0);

	return 0;
}

//��ׯ��Ϣ
LRESULT CGameClientEngine::OnCallBanker(WPARAM wParam, LPARAM lParam)
{
	//ɾ����ʱ��
	KillGameClock(IDI_CALL_BANKER);

	//���ð�ť
	m_GameClientView.m_btNoCallBanker.ShowWindow(SW_HIDE);
	m_GameClientView.m_btCallBanker.ShowWindow(SW_HIDE);
	
	//������Ϣ
	SendSocketData(SUB_C_CALL_BANKER);

	return true ;
}

//������Ϣ
LRESULT CGameClientEngine::OnNoCallBanker(WPARAM wParam, LPARAM lParam)
{
	//ɾ����ʱ��
	KillGameClock(IDI_CALL_BANKER);

	//���ð�ť
	m_GameClientView.m_btNoCallBanker.ShowWindow(SW_HIDE);
	m_GameClientView.m_btCallBanker.ShowWindow(SW_HIDE);

	//������Ϣ
	SendSocketData(SUB_C_NO_CALL_BANKER);

	return true ;
}

//ѹע��Ϣ
LRESULT CGameClientEngine::OnSetChip(WPARAM wParam, LPARAM lParam)
{
	KillGameClock(IDI_SETCHIP) ;

	CMD_C_SetChip  SetChip ;
	ZeroMemory(&SetChip, sizeof(SetChip)) ;
	SetChip.nChip = m_GameClientView.GetChipArrayItem((int)wParam);

	if(-1==SetChip.nChip)
	{
		SetChip.nChip = 100;
	}

	//������Ϣ
	SendSocketData(SUB_C_SETCHIP , &SetChip , sizeof(SetChip)) ;

	//���°�ť
	UpdateChipButtonState(SW_HIDE, FALSE) ;

	//��������
	PlayGameSound(AfxGetInstanceHandle(),TEXT("ADD_SCORE"));

	return true ;
}

//������Ϣ
LRESULT CGameClientEngine::OnSplitCard(WPARAM wParam, LPARAM lParam)
{	
	//��������
	SendSocketData(SUB_C_SPLIT_CARD);

	//��������
	//m_GameClientView.m_btSplitCard.EnableWindow(FALSE);

	return 0L;
}

//�������
LRESULT CGameClientEngine::OnSplitFinish(WPARAM wParam, LPARAM lParam)
{
	//������Ϣ
	SendSocketData(SUB_C_SPLIT_FINISH);
	return 0L;
}

//ϴ�����
LRESULT CGameClientEngine::OnShuffleFinish(WPARAM wParam, LPARAM lParam)
{
	//������Ϣ
	SendSocketData(SUB_C_SHUFFLE_FINISH);

	return 0L;
}

//�������
LRESULT CGameClientEngine::OnDispatchCardFinish(WPARAM wParam, LPARAM lParam)
{
	//�����û�
	WORD wMeChairID=GetMeChairID();

	m_GameLogic.SortCardList(m_bHandCardData[wMeChairID], m_bHandCardCount, enDescend);
	m_GameClientView.SetCardData(ME_VIEW_CHAIR, m_bHandCardData[wMeChairID], m_bHandCardCount, enCCTHandCard);

	//�������
	if ((false == IsLookonMode()) && (false == m_bTrustee))
		ActiveGameFrame();

	if(!m_bCheatRight)
	{
		//��������
		m_GameClientView.SetCardData(INVALID_CHAIR, NULL, 0, enCCTAllSegmentCard);
	}
	else
	{
		m_GameClientView.SetCardData(SwitchViewChairID(wMeChairID), NULL, 0, enCCTAllSegmentCard);
	}
	

	//�����˿�
	if (!IsLookonMode())
	{
		m_GameClientView.m_UserCardControl[ME_VIEW_CHAIR].SetPositively(true);
		m_GameClientView.m_UserCardControl[ME_VIEW_CHAIR].ShootAllCard(false);
	}
	else
	{
		m_GameClientView.m_UserCardControl[ME_VIEW_CHAIR].SetPositively(false);
	
	}
	

	if (false == IsLookonMode() &&(m_bGameStatus[wMeChairID] != false))
	{
		//��������ʾ��
		BYTE cbCardType = m_GameLogic.GetSpecialType(m_bHandCardData[wMeChairID], m_bHandCardCount);
		
		if (cbCardType > CT_EX_INVALID) 
		{
			if ((cbCardType==CT_EX_ZHIZUNQINGLONG)||(cbCardType==CT_EX_YITIAOLONG))
			{
				OnMsgInfo(1,0);
				m_GameClientView.SetCardTypeSpec(true, ME_VIEW_CHAIR,cbCardType);
				SetTimer(IDI_ALL_RANGE_CARD, 1000, NULL);
				return 0;
			}
			else
			{			
				CString szCardName, szTipMsg;
				switch (cbCardType)
				{
					case CT_EX_LIUDUIBAN:		szCardName=TEXT("���԰�"); break;
					case CT_EX_SANSHUNZI:		szCardName=TEXT("��˳��"); break;
					case CT_EX_SANTONGHUA:		szCardName=TEXT("��ͬ��");break;	
				}					
				
				szTipMsg.Format(TEXT("�������ͷ��ϡ�%s��,\n���Ƿ�Ҫ������ͣ�"),szCardName);
				m_GameClientView.m_MsgBox.ShowMessage(szTipMsg);
				return 0;
			}
		}
	}

	//���ð�ť
	if ((IsLookonMode()==false)&&(m_bGameStatus[wMeChairID] != false))
	{		
		//���°�ť
		UpdateButtonState();
		UpdateBlockButton(0, SW_SHOW, FALSE);	

		//��������
		m_GameClientView.m_btSortCard.EnableWindow(TRUE);
		//��������
		//m_GameClientView.m_btSplitCard.EnableWindow(TRUE);
		//��ʾ���ƿ�
		m_GameClientView.ShowCenterControl(true);

		//��ʼ��ʱ
		//SetGameClock(wMeChairID, IDI_RANGE_CARD, m_cbTimeRangeCard);

		
		m_GameClientView.SetGlobalClock(true);
		m_GameClientView.SetElapseTime(m_cbTimeRangeCard);
		m_TimerCount = m_cbTimeRangeCard;
		SetTimer(IDI_ALL_RANGE_CARD, 1000, NULL);
	}
	
	return 0;
}


//��ʾ��Ϣ
LRESULT CGameClientEngine::OnPrompt(WPARAM wParam, LPARAM lParam)
{
	BYTE cbShootCount = 0;
	BYTE cbShootCard[HAND_CARD_COUNT] = {0};
	
	//��ԭ����
	m_GameClientView.m_UserCardControl[ME_VIEW_CHAIR].ShootAllCard(false);
	
	UINT nSearchCmd = (UINT)wParam;
	//�����˿�
	switch (nSearchCmd)
	{
	case 0:		//һ��
	case 1:		//����		
		{
				//��������
			m_GameLogic.SearchSameCard(m_bHandCardData[GetMeChairID()], m_bHandCardCount, 2, &m_SearchCardResult);

			if(m_SearchCardResult.cbSearchCount >= (nSearchCmd+1))
			{
				cbShootCount = 2*(nSearchCmd+1);
				for(UINT i = 0; i < nSearchCmd+1; i++)
				{
					CopyMemory(&cbShootCard[2*i], &m_SearchCardResult.cbResultCard[(m_cbSearchTypeIndex+i)%m_SearchCardResult.cbSearchCount], sizeof(BYTE)*2);
				}
			}
			break;
		}
	case 2:		//����
		{
				//��������
			m_GameLogic.SearchSameCard(m_bHandCardData[GetMeChairID()], m_bHandCardCount, 3, &m_SearchCardResult);

			if(m_SearchCardResult.cbSearchCount >= 1)
			{
				cbShootCount = m_SearchCardResult.cbCardCount[m_cbSearchTypeIndex];
				CopyMemory(cbShootCard, &m_SearchCardResult.cbResultCard[m_cbSearchTypeIndex], sizeof(BYTE)*cbShootCount);
			}
			break;
		}
	case 3:		//˳��
		{	
			m_GameLogic.SearchLineCardType(m_bHandCardData[GetMeChairID()], m_bHandCardCount, 5, &m_SearchCardResult);

			if (m_SearchCardResult.cbSearchCount > 0)
			{
				cbShootCount = m_SearchCardResult.cbCardCount[m_cbSearchTypeIndex];
				CopyMemory(cbShootCard, m_SearchCardResult.cbResultCard[m_cbSearchTypeIndex], sizeof(BYTE)*cbShootCount);
			}

			break;
		}
	case 4:		//ͬ��
		{
			m_GameLogic.SearchSameColorType(m_bHandCardData[GetMeChairID()], m_bHandCardCount, 5, &m_SearchCardResult);
			
			if (m_SearchCardResult.cbSearchCount > 0)
			{
				cbShootCount = m_SearchCardResult.cbCardCount[m_cbSearchTypeIndex];
				CopyMemory(cbShootCard, m_SearchCardResult.cbResultCard[m_cbSearchTypeIndex], sizeof(BYTE)*cbShootCount);
			}

			break;
		}
	case 5:		//��«. (������һ��)
		{
			m_GameLogic.SearchTakeCardType(m_bHandCardData[GetMeChairID()], m_bHandCardCount, 3, 2, &m_SearchCardResult);

			if(m_SearchCardResult.cbSearchCount > 0)
			{
				cbShootCount = m_SearchCardResult.cbCardCount[m_cbSearchTypeIndex];
				CopyMemory(cbShootCard, m_SearchCardResult.cbResultCard[m_cbSearchTypeIndex], sizeof(BYTE)*cbShootCount);
			}
			break;
		}
	case 6:		//��֧ ��������һ����
		{
			m_GameLogic.SearchTakeCardType(m_bHandCardData[GetMeChairID()], m_bHandCardCount, 4, 1, &m_SearchCardResult);

			if(m_SearchCardResult.cbSearchCount > 0)
			{
				cbShootCount = m_SearchCardResult.cbCardCount[m_cbSearchTypeIndex];
				CopyMemory(cbShootCard, m_SearchCardResult.cbResultCard[m_cbSearchTypeIndex], sizeof(BYTE)*cbShootCount);
			}
			break;
		}
	case 7:		//ͬ��˳
		{
			m_GameLogic.SearchSameColorLineType(m_bHandCardData[GetMeChairID()], m_bHandCardCount, 5, &m_SearchCardResult);

			if(m_SearchCardResult.cbSearchCount > 0)
			{
				cbShootCount = m_SearchCardResult.cbCardCount[m_cbSearchTypeIndex];
				CopyMemory(cbShootCard, m_SearchCardResult.cbResultCard[m_cbSearchTypeIndex], sizeof(BYTE)*cbShootCount);
			}
			break;
		}
	}

	//������
	m_GameClientView.m_UserCardControl[ME_VIEW_CHAIR].SetShootCard(cbShootCard, cbShootCount);

	//�л��������
	if (m_SearchCardResult.cbSearchCount > 0)
		m_cbSearchTypeIndex = (m_cbSearchTypeIndex+1)%m_SearchCardResult.cbSearchCount;
	
	//���°�ť
	if ((3 != cbShootCount) && (5 != cbShootCount))
	{
		UpdateBlockButton(0, SW_SHOW, FALSE);
	}

	if (3 == cbShootCount)
	{
		UpdateBlockButton(1, SW_SHOW, TRUE);
	}

	if (5 == cbShootCount)
	{
		if (m_GameClientView.m_CenterCardControl.GetCardCount(1) == 0)
			UpdateBlockButton(2, SW_SHOW, TRUE);
		else
			UpdateBlockButton(2, SW_SHOW, FALSE);

		if (m_GameClientView.m_CenterCardControl.GetCardCount(2) == 0)
			UpdateBlockButton(3, SW_SHOW, TRUE);
		else
			UpdateBlockButton(3, SW_SHOW, FALSE);
	}

	//ˢ�½���
	m_GameClientView.RefreshGameView();

	return 0L;
}

//������Ϣ
LRESULT CGameClientEngine::OnVoicePhrase(WPARAM wParam, LPARAM lParam)
{
	if (false == CGlobalUnits::GetInstance()->m_bAllowSound)
	{
		m_pIStringMessage->InsertSystemString(TEXT("��������δ���ã�"));		
		return 0L;
	}

	if (false == m_bCanPlayVoice) 
	{
		m_pIStringMessage->InsertSystemString(TEXT("��˵��̫����!��ȱ��������ģ�"));
		return 0L;
	}
	else
	{
		//��������
		CMD_C_ChoseYuYin ChoseYuYin;
		ChoseYuYin.bType=(BYTE)wParam;
		ChoseYuYin.wChairID=GetMeChairID();

		IClientUserItem *pIClientUserItem=GetTableUserItem(GetMeChairID());
		ChoseYuYin.bYuYinIndex=RandYuYin(0,pIClientUserItem->GetGender());

		m_bCanPlayVoice = false;
		SetTimer(IDI_CAN_PLAY_VOICE, 3600, NULL);

		//������Ϣ
		SendSocketData(SUB_C_VOICE_PHRASE, &ChoseYuYin, sizeof(ChoseYuYin));
	}


	return 0L;
}

//������Ϣ
LRESULT	CGameClientEngine::OnSortCard(WPARAM wParam, LPARAM lParam) 
{
	if (m_bHandCardCount <= 0) return 0L;

	//��������
	enSortCardType enSCT;

	if (0 == m_cbSortTypeIndex) enSCT = enAscend;
	if (1 == m_cbSortTypeIndex) enSCT = enDescend;
	if (2 == m_cbSortTypeIndex) enSCT = enColor;

	m_GameLogic.SortCardList(m_bHandCardData[GetMeChairID()], m_bHandCardCount, enSCT);
	m_GameClientView.SetCardData(ME_VIEW_CHAIR, m_bHandCardData[GetMeChairID()], m_bHandCardCount, enCCTHandCard);

	m_cbSortTypeIndex = (m_cbSortTypeIndex+1)%3;

	return true ;
}

//�й���Ϣ
LRESULT CGameClientEngine::OnTrustee(WPARAM wParam, LPARAM lParam)
{	
	m_bTrustee = !m_bTrustee;

	CMD_C_Trustee TrusteeControl;
	ZeroMemory(&TrusteeControl, sizeof(TrusteeControl));
	TrusteeControl.bTrustee = m_bTrustee;
	SendSocketData(SUB_C_TRUSTEE, &TrusteeControl, sizeof(TrusteeControl));

	//���ÿؼ�
	m_GameClientView.m_btTrustee.SetButtonImage((true==m_bTrustee)?IDB_BT_TRUSTEE_CANCEL:IDB_BT_TRUSTEE_OPEN, AfxGetInstanceHandle(), false, false);
	
	if (m_bTrustee && m_pIStringMessage)
		m_pIStringMessage->InsertSystemString(TEXT("��ѡ�����йܹ��ܣ�"));
	if (!m_bTrustee && m_pIStringMessage)
		m_pIStringMessage->InsertSystemString(TEXT("��ȡ�����йܹ��ܣ�"));
	
	return true ;
}

//��һע��Ϣ
LRESULT CGameClientEngine::OnFrontCard(WPARAM wParam, LPARAM lParam) 
{
	//���ò���
	UNREFERENCED_PARAMETER(wParam) ;
	UNREFERENCED_PARAMETER(lParam) ;

	CMD_C_SegCard  SegCard ;
	ZeroMemory(&SegCard, sizeof(SegCard)) ;

	//��ȡ�˿�
	BYTE bCardData[HAND_CARD_COUNT];
	BYTE bShootCount=(BYTE)m_GameClientView.m_UserCardControl[ME_VIEW_CHAIR].GetShootCard(bCardData, HAND_CARD_COUNT);

	
	//��Ŀ��֤
	ASSERT(3==bShootCount);
	if(3!=bShootCount) 
	{
		AfxMessageBox(TEXT("�����˿�,������ѡ��!")) ;
		m_GameClientView.m_UserCardControl[ME_VIEW_CHAIR].ShootAllCard(false) ;
		return true ;
	}

	//ɾ���˿�
	m_GameLogic.SortCardList(bCardData, bShootCount, enDescend) ;
	m_GameLogic.RemoveCard(bCardData, bShootCount, m_bHandCardData[GetMeChairID()], m_bHandCardCount);
	m_bHandCardCount -= bShootCount;

	m_tagSegInfo.SetSegData(0, bCardData, bShootCount);
	
	//������Ϣ
	SegCard.SegmentType=enSegFront ;
	SendSocketData(SUB_C_SEGCARD , &SegCard , sizeof(SegCard)) ;

	if(m_bHandCardCount > 0)
	{
		//�����˿�
		m_GameLogic.SortCardList(m_bHandCardData[GetMeChairID()], m_bHandCardCount, enDescend);
	}
	else if(m_bHandCardCount == 0)
	{
		//��ֹ����
		if((enCRGreater == m_GameLogic.CompareCard(m_tagSegInfo.bSegData[0] , m_tagSegInfo.bSegData[1] , 3 , 5 , false)) &&
			(enCRGreater == m_GameLogic.CompareCard(m_tagSegInfo.bSegData[1] , m_tagSegInfo.bSegData[2] , 5 , 5 , true)))
		{
			//��������( ����ע > �ڶ�ע > ��һע)

			//������
			m_GameClientView.m_btShowCard.ShowWindow(SW_SHOW);
			m_GameClientView.m_btShowCard.EnableWindow(TRUE);		


			//�ر���ʾ��ť
			//������ʾ
			for (UINT i = 0; i < 8; i++)
				m_GameClientView.m_btPrompt[i].EnableWindow(FALSE);

			//���Ƶ���ʱ
			//	KillGameClock(IDI_RANGE_CARD);
			// 			SetGameClock(GetMeChairID(), IDI_SHOW_CARD, m_cbTimeShowCard);


		}
		else
		{
			//�ر���ʾ��ť
			//������ʾ
			for (UINT i = 0; i < 8; i++)
				m_GameClientView.m_btPrompt[i].EnableWindow(FALSE);

			//����������( ����ע > �ڶ�ע > ��һע)
			m_GameClientView.SetShowCardTip(true);
			m_GameClientView.RefreshGameView();
			return true;
		}
	}

	//���ý���	
	m_GameClientView.SetCardData(ME_VIEW_CHAIR, m_bHandCardData[GetMeChairID()] , m_bHandCardCount , enCCTHandCard) ;
	m_GameClientView.m_CenterCardControl.SetCardData(0, bCardData, bShootCount);
	m_GameClientView.m_CenterCardControl.ShowWindow(SW_SHOW, 0);
	m_GameClientView.m_btRestore[0].ShowWindow(SW_SHOW);
	m_GameClientView.m_btRestore[0].EnableWindow(TRUE);
	m_GameClientView.m_btFrontCard.ShowWindow(SW_SHOW);
	m_GameClientView.m_btFrontCard.EnableWindow(FALSE);

	if(3==m_bHandCardCount || 5==m_bHandCardCount)
	{
		m_bFinishSegment=true ; 

		ASSERT(m_tagSegInfo.bHaveSeg[1] != m_tagSegInfo.bHaveSeg[2]);
		m_tagSegInfo.SetSegData((m_tagSegInfo.bHaveSeg[1]?2:1), m_bHandCardData[GetMeChairID()], m_bHandCardCount);

		//�������
		m_bHandCardCount = 0;
		ZeroMemory(m_bHandCardData[GetMeChairID()], sizeof(m_bHandCardData[GetMeChairID()]));
		
		if(true==IsLookonMode())
		{
			m_GameClientView.m_CenterCardControl.SetDisplayFlag(false) ;
		}

		//�����˿�
		m_GameClientView.SetCardData(ME_VIEW_CHAIR, NULL, 0, enCCTHandCard);
		m_GameClientView.m_CenterCardControl.SetCardData(0, m_tagSegInfo.bSegData[0] , 3);
		m_GameClientView.m_CenterCardControl.SetCardData(1, m_tagSegInfo.bSegData[1] , 5);
		m_GameClientView.m_CenterCardControl.SetCardData(2, m_tagSegInfo.bSegData[2] , 5);
		m_GameClientView.m_CenterCardControl.ShowWindow(SW_SHOW);

		//�ָ���ť
		m_GameClientView.m_btRestore[0].ShowWindow(SW_SHOW);
		m_GameClientView.m_btRestore[0].EnableWindow(TRUE);
		m_GameClientView.m_btRestore[1].ShowWindow(SW_SHOW);
		m_GameClientView.m_btRestore[1].EnableWindow(TRUE);
		m_GameClientView.m_btRestore[2].ShowWindow(SW_SHOW);
		m_GameClientView.m_btRestore[2].EnableWindow(TRUE);
		m_GameClientView.m_btRestore[3].ShowWindow(SW_SHOW);
		m_GameClientView.m_btRestore[3].EnableWindow(TRUE);

		//���ذ�ť
		UpdateBlockButton(0, SW_SHOW, FALSE);
		
		//��ֹ����
		if((enCRGreater == m_GameLogic.CompareCard(m_tagSegInfo.bSegData[0] , m_tagSegInfo.bSegData[1] , 3 , 5 , false)) &&
			(enCRGreater == m_GameLogic.CompareCard(m_tagSegInfo.bSegData[1] , m_tagSegInfo.bSegData[2] , 5 , 5 , true)))
		{
			//��������( ����ע > �ڶ�ע > ��һע)

			//������
			m_GameClientView.m_btShowCard.ShowWindow(SW_SHOW);
			m_GameClientView.m_btShowCard.EnableWindow(TRUE);		


			//�ر���ʾ��ť
			//������ʾ
			for (UINT i = 0; i < 8; i++)
				m_GameClientView.m_btPrompt[i].EnableWindow(FALSE);

			//���Ƶ���ʱ
		//	KillGameClock(IDI_RANGE_CARD);
// 			SetGameClock(GetMeChairID(), IDI_SHOW_CARD, m_cbTimeShowCard);

			
		}
		else
		{
			//�ر���ʾ��ť
			//������ʾ
			for (UINT i = 0; i < 8; i++)
				m_GameClientView.m_btPrompt[i].EnableWindow(FALSE);

			//����������( ����ע > �ڶ�ע > ��һע)
			m_GameClientView.SetShowCardTip(true);
			m_GameClientView.RefreshGameView();
			return true;
		}
	}

	//���ð�ť
	UpdateButtonState();
	//���½���
	m_GameClientView.RefreshGameView();

	//��������
	PlayGameSound(AfxGetInstanceHandle(),TEXT("OUT_CARD"));

	return true ;
}

//�ڶ�ע��Ϣ
LRESULT CGameClientEngine::OnMidCard(WPARAM wParam, LPARAM lParam)
{
	//���ò���
	UNREFERENCED_PARAMETER(wParam) ;
	UNREFERENCED_PARAMETER(lParam) ;

	CMD_C_SegCard  SegCard ;
	ZeroMemory(&SegCard, sizeof(SegCard)) ;

	//��ȡ�˿�
	BYTE bCardData[HAND_CARD_COUNT];
	BYTE bShootCount=(BYTE)m_GameClientView.m_UserCardControl[ME_VIEW_CHAIR].GetShootCard(bCardData,HAND_CARD_COUNT);

	//��֤��Ŀ
	ASSERT(5==bShootCount);
	if(5!=bShootCount) 
	{
		AfxMessageBox(TEXT("�����˿�,������ѡ��!")) ;
		m_GameClientView.m_UserCardControl[ME_VIEW_CHAIR].ShootAllCard(false) ;
		return true ;
	}

	//ɾ���˿�
	m_GameLogic.SortCardList(bCardData, bShootCount, enDescend) ;
	m_GameLogic.RemoveCard(bCardData, bShootCount, m_bHandCardData[GetMeChairID()], m_bHandCardCount);
	m_bHandCardCount -= bShootCount;
	
	m_tagSegInfo.SetSegData(1, bCardData, bShootCount);
	
	//������Ϣ
	SegCard.SegmentType=enSegMid;
	SendSocketData(SUB_C_SEGCARD , &SegCard , sizeof(SegCard)) ;


	if(m_bHandCardCount > 0)
	{
		//�����˿�
		m_GameLogic.SortCardList(m_bHandCardData[GetMeChairID()], m_bHandCardCount, enDescend);
	}
	else if(m_bHandCardCount == 0)
	{
		//��ֹ����
		if((enCRGreater == m_GameLogic.CompareCard(m_tagSegInfo.bSegData[0] , m_tagSegInfo.bSegData[1] , 3 , 5 , false)) &&
			(enCRGreater == m_GameLogic.CompareCard(m_tagSegInfo.bSegData[1] , m_tagSegInfo.bSegData[2] , 5 , 5 , true)))
		{
			//��������( ����ע > �ڶ�ע > ��һע)

			//������
			m_GameClientView.m_btShowCard.ShowWindow(SW_SHOW);
			m_GameClientView.m_btShowCard.EnableWindow(TRUE);		


			//�ر���ʾ��ť
			//������ʾ
			for (UINT i = 0; i < 8; i++)
				m_GameClientView.m_btPrompt[i].EnableWindow(FALSE);

			//���Ƶ���ʱ
			//	KillGameClock(IDI_RANGE_CARD);
			// 			SetGameClock(GetMeChairID(), IDI_SHOW_CARD, m_cbTimeShowCard);


		}
		else
		{
			//�ر���ʾ��ť
			//������ʾ
			for (UINT i = 0; i < 8; i++)
				m_GameClientView.m_btPrompt[i].EnableWindow(FALSE);

			//����������( ����ע > �ڶ�ע > ��һע)
			m_GameClientView.SetShowCardTip(true);
			m_GameClientView.RefreshGameView();
			return true;
		}
	}



	//���ý���
	m_GameClientView.SetCardData(ME_VIEW_CHAIR, m_bHandCardData[GetMeChairID()], m_bHandCardCount, enCCTHandCard);
	m_GameClientView.m_CenterCardControl.SetCardData(1, bCardData, bShootCount);
	
	m_GameClientView.m_CenterCardControl.ShowWindow(SW_SHOW, 1);
	m_GameClientView.m_btRestore[1].ShowWindow(SW_SHOW);
	m_GameClientView.m_btRestore[1].EnableWindow(TRUE);
	m_GameClientView.m_btMidCard.ShowWindow(SW_SHOW);
	m_GameClientView.m_btMidCard.EnableWindow(FALSE);


	if(3==m_bHandCardCount || 5==m_bHandCardCount)
	{
		m_bFinishSegment=true ; 

		ASSERT(m_tagSegInfo.bHaveSeg[0] != m_tagSegInfo.bHaveSeg[2]);
		m_tagSegInfo.SetSegData((m_tagSegInfo.bHaveSeg[0]?2:0), m_bHandCardData[GetMeChairID()], m_bHandCardCount);

		//�������
		m_bHandCardCount = 0;
		ZeroMemory(m_bHandCardData[GetMeChairID()], sizeof(m_bHandCardData[GetMeChairID()]));
		
		if(true==IsLookonMode())
		{
			m_GameClientView.m_CenterCardControl.SetDisplayFlag(false) ;
		}
		
		//�����˿�
		m_GameClientView.SetCardData(ME_VIEW_CHAIR, NULL, 0, enCCTHandCard);
		m_GameClientView.m_CenterCardControl.SetCardData(0, m_tagSegInfo.bSegData[0] , 3);
		m_GameClientView.m_CenterCardControl.SetCardData(1, m_tagSegInfo.bSegData[1] , 5);
		m_GameClientView.m_CenterCardControl.SetCardData(2, m_tagSegInfo.bSegData[2] , 5);
		m_GameClientView.m_CenterCardControl.ShowWindow(SW_SHOW) ;



		//�ָ����հ�ť
		m_GameClientView.m_btRestore[0].ShowWindow(SW_SHOW);
		m_GameClientView.m_btRestore[0].EnableWindow(TRUE);
		m_GameClientView.m_btRestore[1].ShowWindow(SW_SHOW);
		m_GameClientView.m_btRestore[1].EnableWindow(TRUE);
		m_GameClientView.m_btRestore[2].ShowWindow(SW_SHOW);
		m_GameClientView.m_btRestore[2].EnableWindow(TRUE);
		m_GameClientView.m_btRestore[3].ShowWindow(SW_SHOW);
		m_GameClientView.m_btRestore[3].EnableWindow(TRUE);			
		
		//���ذ�ť
		UpdateBlockButton(0, SW_SHOW, FALSE);

		//��ֹ����
		if (enCRGreater == m_GameLogic.CompareCard(m_tagSegInfo.bSegData[0], m_tagSegInfo.bSegData[1], 3, 5, false) &&
			enCRGreater == m_GameLogic.CompareCard(m_tagSegInfo.bSegData[1], m_tagSegInfo.bSegData[2], 5, 5, true)) 
		{
			//���ð�ť
			
			//������
			m_GameClientView.m_btShowCard.ShowWindow(SW_SHOW) ;
			m_GameClientView.m_btShowCard.EnableWindow(TRUE) ;

			//�ر���ʾ��ť
			//������ʾ
			for (UINT i = 0; i < 8; i++)
				m_GameClientView.m_btPrompt[i].EnableWindow(FALSE);

			//KillGameClock(IDI_RANGE_CARD);
			//SetGameClock(GetMeChairID(), IDI_SHOW_CARD, m_cbTimeShowCard);
		}
		else
		{
			//�ر���ʾ��ť
			//������ʾ
			for (UINT i = 0; i < 8; i++)
				m_GameClientView.m_btPrompt[i].EnableWindow(FALSE);

			m_GameClientView.SetShowCardTip(true);
			m_GameClientView.RefreshGameView();



			return true;
		}
	}

	//���ð�ť
	UpdateButtonState();
	m_GameClientView.m_btBackCard.EnableWindow(FALSE);

	//���½���
	m_GameClientView.RefreshGameView();

	//��������
	PlayGameSound(AfxGetInstanceHandle(),TEXT("OUT_CARD"));

	return true ;
}

//����ע��Ϣ
LRESULT CGameClientEngine::OnBackCard(WPARAM wParam, LPARAM lParam)
{
	//���ò���
	UNREFERENCED_PARAMETER(wParam) ;
	UNREFERENCED_PARAMETER(lParam) ;

	CMD_C_SegCard  SegCard ;
	ZeroMemory(&SegCard, sizeof(SegCard)) ;

	//��ȡ�˿�
	BYTE bCardData[HAND_CARD_COUNT];
	BYTE bShootCount=(BYTE)m_GameClientView.m_UserCardControl[ME_VIEW_CHAIR].GetShootCard(bCardData,HAND_CARD_COUNT);

	//��֤��Ŀ
	ASSERT(5==bShootCount) ;
	if(5!=bShootCount) 
	{
		AfxMessageBox(TEXT("�����˿�,������ѡ��!")) ;
		m_GameClientView.m_UserCardControl[ME_VIEW_CHAIR].ShootAllCard(false) ;
		return true ;
	}

	//ɾ���˿�
	m_GameLogic.SortCardList(bCardData , bShootCount, enDescend) ;
	m_GameLogic.RemoveCard(bCardData , bShootCount , m_bHandCardData[GetMeChairID()] , 	m_bHandCardCount) ;
	m_bHandCardCount -= bShootCount ;

	m_tagSegInfo.SetSegData(2, bCardData, bShootCount);
	
	//������Ϣ
	SegCard.SegmentType=enSegBack ;
	SendSocketData(SUB_C_SEGCARD , &SegCard , sizeof(SegCard)) ;
	
	if(m_bHandCardCount > 0)
	{
		//�����˿�
		m_GameLogic.SortCardList(m_bHandCardData[GetMeChairID()], m_bHandCardCount, enDescend);
	}
	else if(m_bHandCardCount == 0)
	{
		//��ֹ����
		if((enCRGreater == m_GameLogic.CompareCard(m_tagSegInfo.bSegData[0] , m_tagSegInfo.bSegData[1] , 3 , 5 , false)) &&
			(enCRGreater == m_GameLogic.CompareCard(m_tagSegInfo.bSegData[1] , m_tagSegInfo.bSegData[2] , 5 , 5 , true)))
		{
			//��������( ����ע > �ڶ�ע > ��һע)

			//������
			m_GameClientView.m_btShowCard.ShowWindow(SW_SHOW);
			m_GameClientView.m_btShowCard.EnableWindow(TRUE);		


			//�ر���ʾ��ť
			//������ʾ
			for (UINT i = 0; i < 8; i++)
				m_GameClientView.m_btPrompt[i].EnableWindow(FALSE);

			//���Ƶ���ʱ
			//	KillGameClock(IDI_RANGE_CARD);
			// 			SetGameClock(GetMeChairID(), IDI_SHOW_CARD, m_cbTimeShowCard);


		}
		else
		{
			//�ر���ʾ��ť
			//������ʾ
			for (UINT i = 0; i < 8; i++)
				m_GameClientView.m_btPrompt[i].EnableWindow(FALSE);

			//����������( ����ע > �ڶ�ע > ��һע)
			m_GameClientView.SetShowCardTip(true);
			m_GameClientView.RefreshGameView();
			return true;
		}
	}

	

	//���ý���
	m_GameClientView.SetCardData(ME_VIEW_CHAIR , m_bHandCardData[GetMeChairID()] , m_bHandCardCount , enCCTHandCard) ;
	m_GameClientView.m_CenterCardControl.SetCardData(2, bCardData, bShootCount);
	m_GameClientView.m_CenterCardControl.ShowWindow(SW_SHOW, 2);
	m_GameClientView.m_btRestore[2].ShowWindow(SW_SHOW);
	m_GameClientView.m_btRestore[2].EnableWindow(TRUE);
	m_GameClientView.m_btBackCard.ShowWindow(SW_SHOW);
	m_GameClientView.m_btBackCard.EnableWindow(FALSE);

	if(3==m_bHandCardCount || 5==m_bHandCardCount)
	{
		m_bFinishSegment=true ; 

		ASSERT(m_tagSegInfo.bHaveSeg[0] != m_tagSegInfo.bHaveSeg[1]);
		m_tagSegInfo.SetSegData((m_tagSegInfo.bHaveSeg[0]?1:0), m_bHandCardData[GetMeChairID()], m_bHandCardCount);
		
		//�������
		m_bHandCardCount = 0;
		ZeroMemory(m_bHandCardData[GetMeChairID()], sizeof(m_bHandCardData[GetMeChairID()]));

		if(true==IsLookonMode())
		{
			m_GameClientView.m_CenterCardControl.SetDisplayFlag(false) ;
		}

		//�����˿�
		m_GameClientView.SetCardData(ME_VIEW_CHAIR, NULL, 0, enCCTHandCard);
		m_GameClientView.m_CenterCardControl.SetCardData(0, m_tagSegInfo.bSegData[0] , 3);
		m_GameClientView.m_CenterCardControl.SetCardData(1, m_tagSegInfo.bSegData[1] , 5);
		m_GameClientView.m_CenterCardControl.SetCardData(2, m_tagSegInfo.bSegData[2] , 5);
		m_GameClientView.m_CenterCardControl.ShowWindow(SW_SHOW) ;

		//�ָ����հ�ť
		m_GameClientView.m_btRestore[0].ShowWindow(SW_SHOW);
		m_GameClientView.m_btRestore[0].EnableWindow(TRUE);
		m_GameClientView.m_btRestore[1].ShowWindow(SW_SHOW);
		m_GameClientView.m_btRestore[1].EnableWindow(TRUE);
		m_GameClientView.m_btRestore[2].ShowWindow(SW_SHOW);
		m_GameClientView.m_btRestore[2].EnableWindow(TRUE);
		m_GameClientView.m_btRestore[3].ShowWindow(SW_SHOW);
		m_GameClientView.m_btRestore[3].EnableWindow(TRUE);

		//���ذ�ť
		UpdateBlockButton(0, SW_SHOW, FALSE);

		//��ֹ����
		if (enCRGreater == m_GameLogic.CompareCard(m_tagSegInfo.bSegData[0], m_tagSegInfo.bSegData[1], 3, 5, false) &&
			enCRGreater == m_GameLogic.CompareCard(m_tagSegInfo.bSegData[1], m_tagSegInfo.bSegData[2], 5, 5, true))
		{
			//������
			m_GameClientView.m_btShowCard.ShowWindow(SW_SHOW);
			m_GameClientView.m_btShowCard.EnableWindow(TRUE);

			//�ر���ʾ��ť
			//������ʾ
			for (UINT i = 0; i < 8; i++)
				m_GameClientView.m_btPrompt[i].EnableWindow(FALSE);

			//KillGameClock(IDI_RANGE_CARD);
			//SetGameClock(GetMeChairID(), IDI_SHOW_CARD, m_cbTimeShowCard);
		}
		else
		{
			//�ر���ʾ��ť
			//������ʾ
			for (UINT i = 0; i < 8; i++)
				m_GameClientView.m_btPrompt[i].EnableWindow(FALSE);

			m_GameClientView.SetShowCardTip(true);
			m_GameClientView.RefreshGameView();


			return true;
		}
	}

	//���ð�ť
	UpdateButtonState();
	m_GameClientView.m_btMidCard.EnableWindow(FALSE);

	//���½���
	m_GameClientView.RefreshGameView();

	//��������
	PlayGameSound(AfxGetInstanceHandle(),TEXT("OUT_CARD"));

	return true ;
}

//���ȷ��
LRESULT CGameClientEngine::OnShowCard(WPARAM wParam, LPARAM lParam) 
{
	if(m_GameClientView.GetShowCardTip())
	{
		OnRestoreCard(3,0);
		//�Զ�����
		AutomaticOutCard();
	}

	if (m_bIsShowCard)
	{
		return false;
	}

	//���ò���
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

	ASSERT(m_tagSegInfo.GetCardCount(0) == 3 && m_tagSegInfo.GetCardCount(1) == 5 && m_tagSegInfo.GetCardCount(2) == 5) ;
	if (m_tagSegInfo.GetCardCount(0) != 3 || m_tagSegInfo.GetCardCount(1) != 5 || m_tagSegInfo.GetCardCount(2) != 5)
		return false;
    	
	//ֹͣ��ʱ
	//KillGameClock(IDI_RANGE_CARD);
	//KillGameClock(IDI_SHOW_CARD);

	//ȡ����ʾ
	m_GameClientView.SetShowCardTip(false);

	//���طֶ�
	m_GameClientView.ShowCenterControl(false);
	m_GameClientView.m_CenterCardControl.ShowWindow(SW_HIDE) ;
	

	CMD_C_ShowCard AllSegmentCard;
	ZeroMemory(&AllSegmentCard, sizeof(AllSegmentCard));

	m_tagSegInfo.GetSegData(0, AllSegmentCard.bFrontCard, 3);
	m_GameLogic.SortCardList(AllSegmentCard.bFrontCard, 3) ;

	m_tagSegInfo.GetSegData(1, AllSegmentCard.bMidCard, 5);
	m_GameLogic.SortCardList(AllSegmentCard.bMidCard, 5);

	m_tagSegInfo.GetSegData(2, AllSegmentCard.bBackCard, 5);
	m_GameLogic.SortCardList(AllSegmentCard.bBackCard, 5);

	if(false==IsLookonMode())
		SendSocketData(SUB_C_SHOWARD, &AllSegmentCard, sizeof(AllSegmentCard)) ;

	//���ذ�ť
	m_GameClientView.m_btRestore[0].ShowWindow(SW_HIDE);
	m_GameClientView.m_btRestore[0].EnableWindow(FALSE);
	m_GameClientView.m_btRestore[1].ShowWindow(SW_HIDE);
	m_GameClientView.m_btRestore[1].EnableWindow(FALSE);
	m_GameClientView.m_btRestore[2].ShowWindow(SW_HIDE);
	m_GameClientView.m_btRestore[2].EnableWindow(FALSE);
	m_GameClientView.m_btRestore[3].ShowWindow(SW_HIDE);
	m_GameClientView.m_btRestore[3].EnableWindow(FALSE);

	BYTE bCard[13] = {0};
	CopyMemory(bCard, m_tagSegInfo.bSegData[0], sizeof(BYTE)*3);
	CopyMemory(bCard+3, m_tagSegInfo.bSegData[1], sizeof(BYTE)*10);
	m_GameClientView.m_AllSegCardControl[2].SetCardData(bCard, 13);
	m_GameClientView.m_AllSegCardControl[2].ShowWindow(SW_SHOW);

	//���ð�ť
	UpdateChipButtonState(SW_HIDE, FALSE) ;
	UpdateBlockButton(0, SW_HIDE, FALSE);
	UpdateButtonState();
	m_GameClientView.m_btShowCard.ShowWindow(SW_HIDE) ;
	m_GameClientView.m_btShowCard.EnableWindow(FALSE) ;
	m_GameClientView.m_btTrustee.EnableWindow(FALSE) ;


	//���½���
	m_GameClientView.RefreshGameView();

	//��������
	PlayGameSound(AfxGetInstanceHandle(),TEXT("OUT_CARD"));

	m_bIsShowCard = true;

	return true ;
}

//�����˿�
LRESULT CGameClientEngine::OnRestoreCard(WPARAM wParam, LPARAM lParam)
{
	BYTE cbIndex = (BYTE)wParam;
	
	//����ȫ��ʱ����ʾ
	ASSERT(m_bHandCardCount < HAND_CARD_COUNT);
	if (m_bHandCardCount >= HAND_CARD_COUNT) return 0L;

	//ȡ����ʾ
	m_GameClientView.SetShowCardTip(false);

	//ȫ������
	if (cbIndex == 3)
	{
		//���ý���
		m_bFinishSegment = false;
		m_bHandCardCount = HAND_CARD_COUNT;
		CopyMemory(m_bHandCardData[GetMeChairID()], m_bTmpHandCardData, sizeof(BYTE)*m_bHandCardCount);
		m_GameLogic.SortCardList(m_bHandCardData[GetMeChairID()], m_bHandCardCount, enDescend);
		m_GameClientView.m_UserCardControl[ME_VIEW_CHAIR].SetCardData(m_bHandCardData[GetMeChairID()], m_bHandCardCount);
		m_GameClientView.m_CenterCardControl.SetCardData(-1, NULL, 0);
		m_tagSegInfo.Init();

		//���½���
		m_GameClientView.RefreshGameView();

		//���ð�ť
		m_GameClientView.m_btRestore[0].ShowWindow(SW_HIDE);
		m_GameClientView.m_btRestore[0].EnableWindow(FALSE);
		m_GameClientView.m_btRestore[1].ShowWindow(SW_HIDE);
		m_GameClientView.m_btRestore[1].EnableWindow(FALSE);
		m_GameClientView.m_btRestore[2].ShowWindow(SW_HIDE);
		m_GameClientView.m_btRestore[2].EnableWindow(FALSE);
		m_GameClientView.m_btRestore[3].ShowWindow(SW_HIDE);
		m_GameClientView.m_btRestore[3].EnableWindow(FALSE);
		m_GameClientView.m_btShowCard.ShowWindow(SW_HIDE);
		m_GameClientView.m_btShowCard.EnableWindow(FALSE);
		UpdateButtonState();
		UpdateBlockButton(0,SW_SHOW,FALSE);
	}
	//���ֻ���
	else
	{	
		//��������
		BYTE cbSegData[5] = {0};
		WORD cbSegCount = m_GameClientView.m_CenterCardControl.GetCardData(cbIndex, cbSegData, 5);

		//�����˿�
		m_bFinishSegment = false;
		CopyMemory(&m_bHandCardData[GetMeChairID()][m_bHandCardCount], cbSegData, sizeof(BYTE)*cbSegCount);
		m_bHandCardCount += cbSegCount;
		m_GameLogic.SortCardList(m_bHandCardData[GetMeChairID()], m_bHandCardCount, enDescend);
		m_GameClientView.m_UserCardControl[2].SetCardData(m_bHandCardData[GetMeChairID()], m_bHandCardCount);
		m_GameClientView.m_CenterCardControl.SetCardData(cbIndex, NULL, 0);
		m_tagSegInfo.SetSegData(cbIndex, NULL, 5);

		//���½���
		m_GameClientView.RefreshGameView();

		m_GameClientView.m_btRestore[3].ShowWindow(SW_HIDE);
		m_GameClientView.m_btRestore[3].EnableWindow(FALSE);
		m_GameClientView.m_btRestore[cbIndex].ShowWindow(SW_HIDE);
		m_GameClientView.m_btRestore[cbIndex].EnableWindow(FALSE);
		m_GameClientView.m_btShowCard.ShowWindow(SW_HIDE);
		m_GameClientView.m_btShowCard.EnableWindow(FALSE);
		UpdateButtonState();
		UpdateBlockButton(cbIndex+1,SW_SHOW,FALSE);
	}

	return 0L;
}

//һ�ַ��ƽ���
LRESULT CGameClientEngine::OnRevesalTurnFinish(WPARAM wParam, LPARAM lParam)
{
	BYTE cbScene = (BYTE)wParam;

	//��ʾ��Ӯ����
	for (WORD i=0; i<GAME_PLAYER; i++)
	{
		if ((GetTableUserItem(i) == NULL) || (m_bGameStatus[i] == false)) continue;

		SCORE lScoreTimes = 0L;
		if (cbScene == 0) lScoreTimes = m_lScoreTimes[i][0];
		else if (cbScene == 1) lScoreTimes = m_lScoreTimes[i][0]+m_lScoreTimes[i][1];
		else lScoreTimes = m_lScoreTimes[i][0]+m_lScoreTimes[i][1]+m_lScoreTimes[i][2];

		m_GameClientView.SetScoreTimes(SwitchViewChairID(i), lScoreTimes);
	}

	return 0L;
}

//��������
LRESULT	CGameClientEngine::OnMsgInfo(WPARAM wParam, LPARAM lParam)
{
	//�رմ���
	m_GameClientView.m_MsgBox.ShowWindow(SW_HIDE);

	//��ʾ��-ȷ��
	if ((BYTE)wParam == 1)
	{
		BYTE cbSegCardData[3][5] = {0};
		if (0 != m_GameLogic.GetSpecialCardData(m_bHandCardData[GetMeChairID()], m_bHandCardCount, cbSegCardData))
		{
			m_GameClientView.m_UserCardControl[ME_VIEW_CHAIR].SetShootCard(cbSegCardData[0], 3);
			OnFrontCard(0,0);

			m_GameClientView.m_UserCardControl[ME_VIEW_CHAIR].SetShootCard(cbSegCardData[1], 5);
			OnMidCard(0,0);
		}

		//̯��
		OnShowCard(0, 0);

		//��ʾ��ʾ
		if (m_GameLogic.GetDoubleCount(m_tagSegInfo.bSegData[0], m_tagSegInfo.bSegData[1], m_tagSegInfo.bSegData[2]) == 6)
		{
			m_GameClientView.SetCardTypeSpec(true, ME_VIEW_CHAIR, CT_EX_LIUDUIBAN);
		}

		if (m_GameLogic.IsLinkCard(m_tagSegInfo.bSegData[0], 3) && 
			m_GameLogic.IsLinkCard(m_tagSegInfo.bSegData[1], 5) && 
			m_GameLogic.IsLinkCard(m_tagSegInfo.bSegData[2], 5))
		{
			m_GameClientView.SetCardTypeSpec(true, ME_VIEW_CHAIR, CT_EX_SANSHUNZI);
		}

		if (m_GameLogic.IsSameColorCard(m_tagSegInfo.bSegData[0], 3) && 
			m_GameLogic.IsSameColorCard(m_tagSegInfo.bSegData[1], 5) && 
			m_GameLogic.IsSameColorCard(m_tagSegInfo.bSegData[2], 5))
		{
			m_GameClientView.SetCardTypeSpec(true, ME_VIEW_CHAIR, CT_EX_SANTONGHUA);
		}
	}
	//��ʾ��-ȡ��
	else
	{
		//���°�ť
		UpdateButtonState();
		UpdateBlockButton(0, SW_SHOW, FALSE);

		//��������
		//m_GameClientView.m_btSplitCard.EnableWindow(TRUE);
		m_GameClientView.ShowCenterControl(true);

		//��ʼ��ʱ
		//SetGameClock(GetMeChairID(), IDI_RANGE_CARD, m_cbTimeRangeCard);
	}

	return 0L;
}

//����˿�
LRESULT CGameClientEngine::OnLeftHitCard(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam) ;
	UNREFERENCED_PARAMETER(lParam) ;

	if(false==m_bFinishSegment)
	{
		//��ȡ�˿�
		BYTE bCardData[HAND_CARD_COUNT];
		BYTE bShootCount=(BYTE)m_GameClientView.m_UserCardControl[2].GetShootCard(bCardData,HAND_CARD_COUNT);

		//���ð�ť
		if(5==bShootCount)
		{
			
				m_GameClientView.m_btFrontCard.EnableWindow(FALSE) ;
			if(m_GameClientView.m_CenterCardControl.GetCardCount(1) == 0)
				m_GameClientView.m_btMidCard.EnableWindow(TRUE) ;
			if(m_GameClientView.m_CenterCardControl.GetCardCount(2) == 0)
				m_GameClientView.m_btBackCard.EnableWindow(TRUE) ;
		}
		else if(3==bShootCount)
		{
			if(m_GameClientView.m_CenterCardControl.GetCardCount(0) == 0)
				m_GameClientView.m_btFrontCard.EnableWindow(TRUE) ;
		}
		else
		{
			m_GameClientView.m_btFrontCard.EnableWindow(FALSE);
			m_GameClientView.m_btMidCard.EnableWindow(FALSE);
			m_GameClientView.m_btBackCard.EnableWindow(FALSE);	
		}
	}

	return 0;
}

//��ʼ��ׯ
bool CGameClientEngine::OnSubBeginCall(const void * pBuffer, WORD wDataSize)
{
	//������֤
	ASSERT( sizeof( CMD_S_StartCallBanker ) == wDataSize ) ;
	if ( sizeof( CMD_S_StartCallBanker ) != wDataSize ) return false ;

	CMD_S_StartCallBanker  *pStartCallBanker = ( CMD_S_StartCallBanker* )pBuffer ;

	ZeroMemory(m_bUserCheat,sizeof(m_bUserCheat));

	//ȡ�����ƿ�
	m_GameClientView.ShowCenterControl(false);
	UpdateBlockButton(0, SW_HIDE, FALSE);

	//ȡ���ϴ��˿�
	if (IsLookonMode()==true)
	{
		for (WORD i=0 ; i<GAME_PLAYER; ++i)
			m_GameClientView.m_AllSegCardControl[i].SetCardData(-1,NULL, 0);

		m_GameClientView.SetChip(INVALID_CHAIR , -1) ;
		m_GameClientView.SetGameScore(INVALID_CHAIR, -1);	
		m_GameClientView.SetScoreTimes(INVALID_CHAIR, MAXLONGLONG);
		m_GameClientView.SetCallBankerStatus(INVALID_CHAIR, false);
		m_GameClientView.SetBanker(INVALID_CHAIR) ;
		m_GameClientView.SetCardTypeSpec(false,0,0);
	}

	//��ׯʱ��
	SetGameClock( pStartCallBanker->wCurrentCaller, IDI_CALL_BANKER, m_cbTimeCallBanker ) ;

	//���ð�ť
	if ( GetMeChairID() == pStartCallBanker->wCurrentCaller && !IsLookonMode() )
	{
		m_GameClientView.m_btCallBanker.ShowWindow( SW_SHOW ) ;
		m_GameClientView.m_btNoCallBanker.ShowWindow( SW_SHOW ) ;
	}
	
	//��ʾ��Ϣ
	m_GameClientView.SetTipInfoState(1);

	//�������
	if ((false == IsLookonMode())  && (false == m_bTrustee) && (GetMeChairID() == pStartCallBanker->wCurrentCaller))
		ActiveGameFrame();

	//��Ϸ״̬
	SetGameStatus(GS_WK_CALL_BANKER);
	
	//�û�״̬
	CopyMemory(m_bGameStatus,pStartCallBanker->bGameStatus,sizeof(m_bGameStatus));
	for(WORD i=0;i<GAME_PLAYER;i++)
	{
		m_GameClientView.SetGameStatus(SwitchViewChairID(i),m_bGameStatus[i]);
	}

	return true ;
}

//��ҽ�ׯ
bool CGameClientEngine::OnSubUserCall(const void * pBuffer, WORD wDataSize)
{
	//������֤
	ASSERT( sizeof( CMD_S_CallBanker ) == wDataSize ) ;
	if ( sizeof( CMD_S_CallBanker ) != wDataSize ) return false ;

	CMD_S_CallBanker *pCallBanker = ( CMD_S_CallBanker* )pBuffer ;

	//��ҽ�ׯ
	if (pCallBanker->bCallBanker)
	{
		//���ñ���
		m_wBanker = pCallBanker->wLastCaller ;
		m_wMaxChip = pCallBanker->lMaxChip;

		//ɾ����ʱ��
		KillGameClock(IDI_CALL_BANKER) ;

		//����״̬
		SetGameStatus(GS_WK_SETCHIP) ;
		
		//���ý���
		m_GameClientView.SetBanker(SwitchViewChairID(pCallBanker->wLastCaller)) ;
		m_GameClientView.SetCallBankerStatus(INVALID_CHAIR, false) ;

		//���ð�ť
		if (m_wBanker != GetMeChairID() && !IsLookonMode() && m_bGameStatus[GetMeChairID()] == true)
		{
			//��ע��С
			SCORE lChipsArray[SET_CHIP_COUNT] = {0};
			CopyMemory(lChipsArray, pCallBanker->lChipArray[GetMeChairID()], sizeof(lChipsArray));
			
			m_GameClientView.SetChipArray(lChipsArray) ;

			UpdateChipButtonState(SW_SHOW, TRUE) ;
			SetGameClock(GetMeChairID(), IDI_SETCHIP, m_cbTimeSetChip);
		}

		//���ø���ʱ��
		if (!IsLookonMode() && m_bGameStatus[GetMeChairID()] == true)
		{
			for(BYTE i=0;i<GAME_PLAYER;i++)
			{
				if(i != m_wBanker && i != GetMeChairID() && m_bGameStatus[i] == true)
				{
					m_GameClientView.ShowFuZhuClock(SwitchViewChairID(i),true);
				}
			}
			m_cbFuZhuTime=m_cbTimeSetChip;
			m_GameClientView.SetFuZhuTime(m_cbFuZhuTime--);
			//����ʱ��
			SetTimer(IDI_FU_ZHU, 1000, NULL);
		}

		//��ʾ��Ϣ
		m_GameClientView.SetTipInfoState(2);
	}
	//��Ҳ���
	else
	{
		m_wMaxChip = pCallBanker->lMaxChip;

		//���ö�ʱ��
		SetGameClock( pCallBanker->wCurrentCaller, IDI_CALL_BANKER, m_cbTimeCallBanker ) ;				

		//���ð�ť
		if ( pCallBanker->wCurrentCaller == GetMeChairID() && !IsLookonMode() && m_bGameStatus[GetMeChairID()] == true)
		{
			m_GameClientView.m_btCallBanker.ShowWindow(SW_SHOW);
			m_GameClientView.m_btNoCallBanker.ShowWindow(SW_SHOW);
		}

		//���ý���
		m_GameClientView.SetCallBankerStatus(SwitchViewChairID(pCallBanker->wLastCaller), true) ;
	}

	return true ;
}

//������ׯ
bool CGameClientEngine::OnSubAllNotCall(const void * pBuffer, WORD wDataSize)
{
	//��Ϣ��ʾ
	if(m_pIStringMessage != NULL)
	m_pIStringMessage->InsertSystemString(TEXT("�����˶�����ׯ����Ϸ����")) ;

	//���ý���
	m_GameClientView.SetCallBankerStatus(INVALID_CHAIR, false);
	m_GameClientView.SetTrustee(INVALID_CHAIR, false);

	//��ʾ��Ϣ
	m_GameClientView.SetTipInfoState(0);

	if (false == IsLookonMode()) 
	{
		//ȡ���й�
		m_bTrustee = false;
		m_GameClientView.m_btTrustee.SetButtonImage(IDB_BT_TRUSTEE_OPEN, AfxGetInstanceHandle(), false, false);
		m_GameClientView.m_btTrustee.EnableWindow(TRUE);

		m_GameClientView.m_btNoCallBanker.ShowWindow(SW_HIDE) ;
		m_GameClientView.m_btCallBanker.ShowWindow(SW_HIDE) ;

		//��ʾ����
		m_GameClientView.SetGameScore(INVALID_CHAIR, -1);

		//��ʼ��ť
		m_GameClientView.m_btStart.ShowWindow(SW_SHOW) ;
		m_GameClientView.m_btStart.EnableWindow(TRUE) ;

		//���ö�ʱ��
		SetGameClock(GetMeChairID(), IDI_START_GAME, m_cbTimeStartGame) ;
	}

	return true ;
}

//�����˿�
bool CGameClientEngine::OnSubSendCardEx(const void * pBuffer, WORD wDataSize)
{
	ASSERT(wDataSize == sizeof(CMD_S_SendCard));
	if (wDataSize != sizeof(CMD_S_SendCard)) return false;
	
	//ɾ����ʱ��
	KillTimer(IDI_FU_ZHU);
	m_cbFuZhuTime=0;
	
	//��ʾ��Ϣ
	m_GameClientView.SetTipInfoState(0);

	//��������
	CMD_S_SendCard * pSendCard = (CMD_S_SendCard *)pBuffer;

	//�����˿�
	if (GetMeChairID() == pSendCard->wCurrentUser && false == IsLookonMode() || IsLookonMode() && m_bUserCheat)
	{
		m_bHandCardCount = HAND_CARD_COUNT;
		CopyMemory(m_bHandCardData[GetMeChairID()], pSendCard->bCardData, sizeof(BYTE)*m_bHandCardCount);
		CopyMemory(m_bTmpHandCardData, pSendCard->bCardData, sizeof(BYTE)*m_bHandCardCount);
	}

	if(m_bUserCheat)
	{
		OnShuffleFinish(0, 0);
	}
	else
	{
		//��ս���
		m_GameClientView.SetCardData(INVALID_CHAIR, NULL,  0, enCCTAllSegmentCard);
		m_GameClientView.SetCardData(INVALID_CHAIR, NULL, 0, enCCTHandCard);


		//��ʼϴ��
		m_GameClientView.StartShuffleCard();
	}

	return true;
}


//������ע
bool CGameClientEngine::OnSubSetChip(const void * pBuffer , WORD wDataSize)
{
	//������֤
	ASSERT(wDataSize==(sizeof(CMD_S_SetChip))) ;
	if(sizeof(CMD_S_SetChip)!=wDataSize) return false ;
	CMD_S_SetChip *pSetChip = (CMD_S_SetChip*)pBuffer ;

	m_lUserChip[pSetChip->wChipUser] = pSetChip->lChip ;
	
	//���ø���ʱ��
	m_GameClientView.ShowFuZhuClock(SwitchViewChairID(pSetChip->wChipUser),false);

	//���ý���
	m_GameClientView.SetChip(SwitchViewChairID(pSetChip->wChipUser) , pSetChip->lChip, true);
	m_GameClientView.m_UserCardControl[2].ShootAllCard(false);

	return true ;
}

//���÷ֶ�
bool CGameClientEngine::OnSubSetSegment(const void * pBuffer , WORD wDataSize)
{
	//������֤
	ASSERT(sizeof(CMD_S_SetSegment)==wDataSize) ;
	if(sizeof(CMD_S_SetSegment)!=wDataSize) return false ;
    
	CMD_S_SetSegment *pSetSegment = (CMD_S_SetSegment*)pBuffer ;

	//�Թ��û�
	if (IsLookonMode())
	{
		m_GameClientView.m_CenterCardControl.ShowWindow(SW_SHOW);
		m_GameClientView.m_CenterCardControl.SetDisplayFlag(false);
	}

	BYTE bZeroCardData[5];
	ZeroMemory(bZeroCardData, sizeof(bZeroCardData));
	BYTE bCardCount = (pSetSegment->SegmentType==enSegFront) ? 3 : 5;

	//���������ͼ	
	if(GetMeChairID()!=pSetSegment->wCurrentUser)
	{
		//m_GameClientView.SetCardData(SwitchViewChairID(pSetSegment->wCurrentUser), bZeroCardData, bCardCount, enCCTSegmentCard, pSetSegment->SegmentType);
	}
	
	return true ;
}

//���̯��
bool CGameClientEngine::OnSubShowCard(const void * pBuffer , WORD wDataSize)
{
	//������֤
	ASSERT(sizeof(CMD_S_ShowCard)==wDataSize) ;
	if(sizeof(CMD_S_ShowCard)!=wDataSize) return false ;

	CMD_S_ShowCard *pShowCard = (CMD_S_ShowCard*)pBuffer ;

	WORD wCurrentUser = pShowCard->wCurrentUser;
	WORD wViewChairID = SwitchViewChairID(wCurrentUser);
	if ((GetMeChairID()==wCurrentUser) && (true == pShowCard->bCanSeeShowCard))
	{	
		//�����˿�
		BYTE bCard[13] = {0};
		CopyMemory(bCard, pShowCard->bFrontCard, 13);
		m_GameClientView.SetCardData(wViewChairID, bCard, 13, enCCTAllSegmentCard);

		if(IsLookonMode())
			m_GameClientView.m_AllSegCardControl[wViewChairID].SetDisplayFlag(false);
		if (GetMeChairID() != wCurrentUser)
			m_GameClientView.m_AllSegCardControl[wViewChairID].SetDisplayFlag(false);
	
		//��������
		CopyMemory(m_bSegmentCard[wCurrentUser][0] , pShowCard->bFrontCard , 3) ;
		CopyMemory(m_bSegmentCard[wCurrentUser][1] , pShowCard->bMidCard , 5) ;
		CopyMemory(m_bSegmentCard[wCurrentUser][2] , pShowCard->bBackCard , 5) ;
	}
	else
	{
		//��ʾ����
		BYTE bCard[13] = {0};
		m_GameClientView.m_AllSegCardControl[wViewChairID].SetCardData(bCard, 13);
		m_GameClientView.m_AllSegCardControl[wViewChairID].SetDisplayFlag(false);
	}

	m_GameClientView.SetCardData(wViewChairID , NULL , 0 , enCCTHandCard) ;
	
	return true ;
}

//�Ƚ��˿�
bool CGameClientEngine::OnSubCompareCard(const void * pBuffer , WORD wDataSize)
{
	//������֤
	ASSERT(sizeof(CMD_S_CompareCard)==wDataSize) ;
	if(sizeof(CMD_S_CompareCard)!=wDataSize) return false ;

	CMD_S_CompareCard *pCompareCard = (CMD_S_CompareCard*)pBuffer;
	CopyMemory(m_bSegmentCard, pCompareCard->bSegmentCard, sizeof(m_bSegmentCard));
	CopyMemory(m_lScoreTimes, pCompareCard->lScoreTimes, sizeof(m_lScoreTimes));  
	m_bComparedCard = pCompareCard->bCompared;
	
	m_GameClientView.SetGlobalClock(false);
	m_GameClientView.SetElapseTime(0);
	KillTimer(IDI_ALL_RANGE_CARD);

	for (int i = 0; i < GAME_PLAYER; i++)
	{
		if ((GetTableUserItem(i) == NULL) || (m_bGameStatus[i] == false)) 
		{
			continue;
		}

		BYTE bCard[13] = {};
		CopyMemory(bCard, pCompareCard->bSegmentCard[i][0], sizeof(BYTE)*3);
		CopyMemory(bCard+3, pCompareCard->bSegmentCard[i][1], sizeof(BYTE)*10);

		//��������
		WORD wViewChairID = SwitchViewChairID(i);

		//������������
		if (pCompareCard->bSpecialType[i] > CT_EX_INVALID)
		{
			m_GameClientView.PlaySpecCardTypeSound(wViewChairID, pCompareCard->bSpecialType[i]);
			m_GameClientView.SetCardTypeSpec(true, wViewChairID, pCompareCard->bSpecialType[i]);
		}
		//�����˿�
		m_GameClientView.m_UserCardControl[wViewChairID].ShowWindow(SW_HIDE);
		m_GameClientView.m_AllSegCardControl[wViewChairID].SetCardData(bCard, 13);

		if (pCompareCard->bCompared == TRUE)
			m_GameClientView.m_AllSegCardControl[wViewChairID].SetDisplayFlag(false);
		else
			m_GameClientView.m_AllSegCardControl[wViewChairID].SetDisplayFlag(true);
	}
	
	//��ͨ���ͣ��跭�ƱȽ�
	if (pCompareCard->bCompared == TRUE)
	{
		//�Ƚ�ǰ��
		m_nTimer=SetTimer(IDI_COMPARE_FRONT , m_nCompareTime , NULL);
		CompareCard(enSegFront);
	}
	
	return true ;
}

//�û�ǿ��
bool CGameClientEngine::OnSubPlayerExit(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	if (wDataSize!=sizeof(CMD_S_PlayerExit)) return false;
	CMD_S_PlayerExit * pPlayerExit=(CMD_S_PlayerExit *)pBuffer;

	WORD wID = pPlayerExit->wPlayerID;
	WORD wViewID = SwitchViewChairID(wID);

	//��Ϸ��Ϣ
	ASSERT(m_bGameStatus[wID]==true);
	m_bGameStatus[wID]=false;
	m_GameClientView.SetGameStatus(wViewID, false);
	m_GameClientView.SetChip(wViewID, -1);
	m_GameClientView.SetGameScore(wViewID, -1);
	m_GameClientView.SetScoreTimes(wViewID, MAXLONGLONG);
	m_GameClientView.m_UserCardControl[wViewID].ShowWindow(SW_HIDE);

	BYTE bCount=0;
	for(WORD i=0;i<GAME_PLAYER;i++) if(m_bGameStatus[i]!=false)bCount++;
	if(bCount==1) m_GameClientView.SetBanker(INVALID_CHAIR);

	if (GetMeChairID() != pPlayerExit->wPlayerID)
	{
		//������ʾ
		if (m_pIStringMessage != NULL) m_pIStringMessage->InsertSystemString(TEXT("\n�û�ǿ�ˣ�")) ;

		for (WORD i=0;i<GAME_PLAYER;i++)
		{
			IClientUserItem * pClientUserItem=GetTableUserItem(i);
			if(pClientUserItem != NULL)
			{
				CString strScoreRes ;
				strScoreRes.Format(TEXT("%s��%I64d\n") , pClientUserItem->GetNickName(), pPlayerExit->lScore[i]) ;
				if (m_pIStringMessage != NULL) m_pIStringMessage->InsertNormalString( strScoreRes);
			}
		}
	}

	return true;
}

//�û�����
bool CGameClientEngine::OnSubSplitCard(const void * pBuffer, WORD wDataSize)
{
	ASSERT(wDataSize == sizeof(CMD_S_SplitCard));
	if (wDataSize != sizeof(CMD_S_SplitCard)) return false;

	CMD_S_SplitCard *pSplitCard = (CMD_S_SplitCard *)pBuffer;

	//���ñ���
	m_bRequestSplit = true;

	//�㲥��Ϣ
	if (pSplitCard->wLastUser != GetMeChairID())
	{
		TCHAR szMsg[128] = TEXT("");
		_sntprintf(szMsg, CountArray(szMsg), TEXT("��ҡ�%s��������һ�����ƣ�"), GetTableUserItem(pSplitCard->wLastUser)->GetNickName());
		m_pIStringMessage->InsertSystemString(szMsg);
	}
	else m_pIStringMessage->InsertSystemString(TEXT("��������һ�����ƣ�"));

	return  true;
}

//��ʼ����
bool CGameClientEngine::OnSubDispatchStart(const void * pBuffer, WORD wDataSize)
{
	//����״̬
	SetGameStatus(GS_WK_PLAYING);

	WORD wMeChairID = GetMeChairID();

	//������ʾ
	if (m_bGameStatus[wMeChairID] != false)
	{
		m_GameClientView.m_UserCardControl[2].ShowWindow(SW_SHOW);
		
		
		if (IsLookonMode())
		{
			m_GameClientView.m_UserCardControl[2].SetDisplayFlag(false || m_bUserCheat);
			m_GameClientView.m_UserCardControl[2].SetPositively(false);
		}
		else
		{
			m_GameClientView.m_UserCardControl[2].SetPositively(true);
			m_GameClientView.m_UserCardControl[2].SetDisplayFlag(true);
		}
			
	}

	//�������
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//���ñ���
		if(wMeChairID!=i && m_bGameStatus[i] != false)
		{
			m_GameClientView.m_UserCardControl[SwitchViewChairID(i)].ShowWindow(SW_SHOW);
		}
	}

	//�������ʱ��
	WORD wPlayerCount = 0;
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		if ((GetTableUserItem(i) == NULL) && (m_bGameStatus[i] == false)) continue;
		wPlayerCount++;
	}
	if (wPlayerCount == 2) m_nCompareTime = 4000;
	else if (wPlayerCount == 3) m_nCompareTime = 5000;
	else m_nCompareTime = 6000;

	//����
	if ((false == m_bRequestSplit))
	{
		if(m_bCheatRight)		
		{
			OnDispatchCardFinish(0, 0);
		}
		else
		{
			//���ƶ���
			WORD wViewStartUser=SwitchViewChairID(m_wBanker);
			m_GameClientView.StartDispatchCard(m_bHandCardData[GetMeChairID()],wViewStartUser);

			//��������
			PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_START"));
		}
	}
	else		//���ƶ���
	{
		m_bRequestSplit = false;
		if(m_bCheatRight)		
		{
			//������Ϣ
			OnSplitFinish(0, 0);
		}
		else
		{
			m_GameClientView.StartSplitCard();
		}		
	}

	return true;
}

//�û��й�
bool CGameClientEngine::OnSubTrustee(const void * pBuffer,WORD wDataSize)
{
	//У������
	ASSERT(wDataSize == sizeof(CMD_S_Trustee));
	if (wDataSize != sizeof(CMD_S_Trustee)) return false;

	//��Ϣ����
	CMD_S_Trustee *pTrustee = (CMD_S_Trustee *)pBuffer;

	//����״̬
	m_GameClientView.SetTrustee(SwitchViewChairID(pTrustee->wChairID), pTrustee->bTrustee);
	if ((IsLookonMode()==true)||(pTrustee->wChairID!=GetMeChairID()))
	{
		IClientUserItem * pClientUserItem=GetTableUserItem(pTrustee->wChairID);
		TCHAR szBuffer[256];
		if(pTrustee->bTrustee==true)
			_sntprintf(szBuffer,sizeof(szBuffer),TEXT("���[%s]ѡ�����йܹ���."),pClientUserItem->GetNickName());
		else
			_sntprintf(szBuffer,sizeof(szBuffer),TEXT("���[%s]ȡ�����йܹ���."),pClientUserItem->GetNickName());
		if(m_pIStringMessage != NULL)
			m_pIStringMessage->InsertSystemString(szBuffer);
	}
	return true;
}

//��������
bool CGameClientEngine::OnSubVoicePhrase(const void * pBuffer, WORD wDataSize)
{
	//Ч�����
	ASSERT(wDataSize==sizeof(CMD_S_ChoseYuYin));
	if (wDataSize!=sizeof(CMD_S_ChoseYuYin)) return false;

	//��������
	CMD_S_ChoseYuYin * pChoseYuYin = (CMD_S_ChoseYuYin *)pBuffer;
	HINSTANCE hInstance=AfxGetInstanceHandle();
	IClientUserItem *pIClientUserItem=GetTableUserItem(pChoseYuYin->wChairID);
		
	//�������
	TCHAR szBuffer[512]=TEXT("");
	_sntprintf(szBuffer,sizeof(szBuffer),TEXT("%s"),GetPhraseString(pIClientUserItem->GetGender(),pChoseYuYin->bType,pChoseYuYin->bChoseYuYin));
	m_pIStringMessage->InsertUserChat(pIClientUserItem->GetNickName(),szBuffer,RGB(0,0,0));


	//��ȡ��ǰ����·��
	TCHAR szExePath[MAX_PATH] = {};
	GetModuleFileName( NULL,szExePath,CountArray(szExePath) );

	CString szTemp;
	szTemp = szExePath;
	int nPos = szTemp.ReverseFind(TEXT('\\'));
	_sntprintf(szExePath, CountArray(szExePath), TEXT("%s"), szTemp.Left(nPos));

	//��������
	if(pIClientUserItem->GetGender() == 1)
	{
		_sntprintf(szBuffer,sizeof(szBuffer),TEXT("%s\\Sound\\Man\\%d_%d.wav"),szExePath,pChoseYuYin->bType,pChoseYuYin->bChoseYuYin);
	}
	else
	{
		_sntprintf(szBuffer,sizeof(szBuffer),TEXT("%s\\Sound\\Women\\%d_%d.wav"),szExePath,pChoseYuYin->bType,pChoseYuYin->bChoseYuYin);
	}

	//��������
	PlayGameSound(szBuffer);

	return true;
}

//��������
bool CGameClientEngine::OnSubCheatCard(VOID * pData, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_CheatCard));
	if (wDataSize!=sizeof(CMD_S_CheatCard)) return false;

	//��������
	CMD_S_CheatCard * pCheatCard=(CMD_S_CheatCard *)pData;

	ASSERT(pCheatCard->cbUserCount <= GAME_PLAYER);
	if(pCheatCard->cbUserCount > GAME_PLAYER) return false;

	//��ս���
	m_GameClientView.SetCardData(INVALID_CHAIR, NULL,  0, enCCTAllSegmentCard);
	m_GameClientView.SetCardData(INVALID_CHAIR, NULL, 0, enCCTHandCard);

	m_bCheatRight = true;
	for(WORD i = 0; i < pCheatCard->cbUserCount ; i++)
	{
		//��ʾ�˿�
		m_bUserCheat[pCheatCard->wCardUser[i]] = true;
		CopyMemory(m_bHandCardData[pCheatCard->wCardUser[i]], pCheatCard->cbCardData[i], sizeof(BYTE) * pCheatCard->cbCardCount[i]);

		ShowAutomaticOutCard(pCheatCard->wCardUser[i]);

	}
	//���½���
	CRect rcClient;
	GetClientRect(&rcClient);
	m_GameClientView.InvalidGameView(0,0,rcClient.Width(),rcClient.Height());
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
	WORD wPlayerCount = 0;
	
	//��Ϸ״̬
	SetGameStatus(GS_WK_FREE);

	//ֹͣ����
	m_GameClientView.StopShuffleCard();
	m_GameClientView.StopSplitCard();
	m_GameClientView.StopDispatchCard();
	m_GameClientView.StopSwatCartoon();
	m_GameClientView.StopGunCartoon();

	//ɾ����ʱ��
	KillTimer(IDI_FU_ZHU) ;
	m_cbFuZhuTime=0;
	
	//��ʾ��Ϣ
	m_GameClientView.SetTipInfoState(0);
	
	//ɾ����ʱ��
	KillGameClock(IDI_START_GAME);
	KillGameClock(IDI_CALL_BANKER);
	KillGameClock(IDI_SETCHIP);
	KillTimer(IDI_ALL_RANGE_CARD);
	//KillGameClock(IDI_RANGE_CARD);
	//KillGameClock(/*IDI_SHOW_CARD*/);
	KillTimer(m_nTimer) ;

	//���ð�ť
	UpdateButtonState();
	UpdateChipButtonState(SW_HIDE, FALSE) ;
	UpdateBlockButton(0, SW_HIDE, FALSE);
	m_GameClientView.m_btNoCallBanker.ShowWindow(SW_HIDE) ;
	m_GameClientView.m_btCallBanker.ShowWindow(SW_HIDE) ;
	m_GameClientView.m_btRestore[0].ShowWindow(SW_HIDE);
	m_GameClientView.m_btRestore[0].EnableWindow(FALSE);
	m_GameClientView.m_btRestore[1].ShowWindow(SW_HIDE);
	m_GameClientView.m_btRestore[1].EnableWindow(FALSE);
	m_GameClientView.m_btRestore[2].ShowWindow(SW_HIDE);
	m_GameClientView.m_btRestore[2].EnableWindow(FALSE);
	m_GameClientView.m_btRestore[3].ShowWindow(SW_HIDE);
	m_GameClientView.m_btRestore[3].EnableWindow(FALSE);

	if (!m_bTrustee && !IsLookonMode())
	{
		m_GameClientView.m_btTrustee.ShowWindow(SW_SHOW);
		m_GameClientView.m_btTrustee.EnableWindow(TRUE);
	}

	//���ý���
	m_tagSegInfo.Init();
	m_GameClientView.SetGlobalClock(false);
	m_GameClientView.SetCallBankerStatus(INVALID_CHAIR, false);
	m_GameClientView.SetTrustee(INVALID_CHAIR, false);
	m_GameClientView.ShowCenterControl(false);
	m_GameClientView.SetCardData(INVALID_CHAIR , NULL , 0 , enCCTHandCard);
	m_GameClientView.m_CenterCardControl.SetCardData(-1, NULL, 0);

	//��ʾ�˿�
	for (WORD i=0; i<GAME_PLAYER; i++)
	{
		if (GetTableUserItem(i) == NULL || m_bGameStatus[i] == false) continue;

		wPlayerCount++;
		 WORD wViewChairID = SwitchViewChairID(i);
		
		if (m_bSegmentCard[i][0][0] != 0)
		{ 
			m_GameClientView.m_AllSegCardControl[wViewChairID].SetCardData(0, m_bSegmentCard[i][0], 3);
			m_GameClientView.m_AllSegCardControl[wViewChairID].SetCardData(1, m_bSegmentCard[i][1], 5);
			m_GameClientView.m_AllSegCardControl[wViewChairID].SetCardData(2, m_bSegmentCard[i][2], 5);

			m_GameClientView.m_AllSegCardControl[wViewChairID].SetDisplayFlag(true);
			m_GameClientView.m_AllSegCardControl[wViewChairID].ShowWindow(SW_SHOW);
		}

	}
	
	//ȡ���й�
	if ((true == m_bTrustee) && (false == IsLookonMode()))
	{
		m_bTrustee = false;
		m_GameClientView.m_btTrustee.SetButtonImage(IDB_BT_TRUSTEE_OPEN, AfxGetInstanceHandle(), false, false);
		m_GameClientView.m_btTrustee.EnableWindow(TRUE);
	}


	//ȫ�ݴ򶯻�		
	bool bAllSwat = true;
	if ((wPlayerCount == GAME_PLAYER) && (m_bComparedCard == TRUE))
	{
		//Ӯ�����
		WORD wWinnerUser=INVALID_CHAIR;
		for (WORD i=0; i<GAME_PLAYER; i++)
		{
			if(pGameEnd->bWinInfo[i]==2)
			{
				wWinnerUser=i;
			}
		}

		if(wWinnerUser!=INVALID_CHAIR)
		{
			//�������мұȽ�
			for (WORD i=0; i<GAME_PLAYER; i++)
			{
				if (i != wWinnerUser)
				{
					//���Ӯ����һע�䣬�����ѭ��
					if ((m_lScoreTimes[wWinnerUser][0] < m_lScoreTimes[i][0]) ||
						(m_lScoreTimes[wWinnerUser][1] < m_lScoreTimes[i][1]) ||
						(m_lScoreTimes[wWinnerUser][2] < m_lScoreTimes[i][2]))
					{
						bAllSwat = false;
						break;
					}
				}
			}
			if (bAllSwat == true)
				m_GameClientView.StartSwatCartoon(SwitchViewChairID(wWinnerUser));
		}
	}

	//////////////////////////////////////////////////////////////////////////
	//��ǹ����
	WORD wGunCounter = 0;
	tagGunStatus GunStatus[GAME_PLAYER];
	FillMemory(&GunStatus, sizeof(tagGunStatus)*GAME_PLAYER, INVALID_CHAIR);

	//�мұȽ�
	for (WORD i = 0; i < GAME_PLAYER; i++)
	{
		if (GetTableUserItem(i) == NULL ||
			m_bGameStatus[i] == false || i == m_wBanker) continue;

		for (WORD j = i + 1; j < GAME_PLAYER; j++)
		{
			if (GetTableUserItem(j) == NULL ||
				m_bGameStatus[j] == false || j == m_wBanker) continue;

			//��עȫ��
			if ((m_lScoreTimes[i][0] > m_lScoreTimes[j][0]) &&
				(m_lScoreTimes[i][1] > m_lScoreTimes[j][1]) &&
				(m_lScoreTimes[i][2] > m_lScoreTimes[j][2]) )
			{
				GunStatus[wGunCounter].wSrcUser = SwitchViewChairID(i);
				GunStatus[wGunCounter].wDestUser = SwitchViewChairID(j);
				wGunCounter++;
			}

			//��עȫС
			if ((m_lScoreTimes[i][0] < m_lScoreTimes[j][0]) &&
				(m_lScoreTimes[i][1] < m_lScoreTimes[j][1]) &&
				(m_lScoreTimes[i][2] < m_lScoreTimes[j][2]) )
			{
				GunStatus[wGunCounter].wSrcUser = SwitchViewChairID(j);
				GunStatus[wGunCounter].wDestUser = SwitchViewChairID(i);
				wGunCounter++;
			}
		}
	}

	//ׯ�бȽ�
	for (i = 0; i < GAME_PLAYER; i++)
	{
		if (GetTableUserItem(i) == NULL ||
			m_bGameStatus[i] == false || i == m_wBanker) continue;

		//��עȫ��
		if ((m_lScoreTimes[i][0] > 0) &&
			(m_lScoreTimes[i][1] > 0) &&
			(m_lScoreTimes[i][2] > 0) )
		{
			GunStatus[wGunCounter].wSrcUser = SwitchViewChairID(i);
			GunStatus[wGunCounter].wDestUser = SwitchViewChairID(m_wBanker);
			wGunCounter++;
		}

		//��עȫС
		if ((m_lScoreTimes[i][0] < 0) &&
			(m_lScoreTimes[i][1] < 0) &&
			(m_lScoreTimes[i][2] < 0) )
		{
			GunStatus[wGunCounter].wSrcUser = SwitchViewChairID(m_wBanker);
			GunStatus[wGunCounter].wDestUser = SwitchViewChairID(i);
			wGunCounter++;
		}
	}

	//for (WORD i=0; i<GAME_PLAYER; i++)
	//{
	//	if (GetTableUserItem(i) == NULL || m_bGameStatus[i] == false) continue;

	//	for (WORD j=i+1; j<GAME_PLAYER; j++)
	//	{
	//		if (GetTableUserItem(j) == NULL || m_bGameStatus[j] == false) continue;

	//		//��עȫ��
	//		if ((m_lScoreTimes[i][0] > m_lScoreTimes[j][0]) &&
	//			(m_lScoreTimes[i][1] > m_lScoreTimes[j][1]) &&
	//			(m_lScoreTimes[i][2] > m_lScoreTimes[j][2]) )
	//		{
	//			GunStatus[wGunCounter].wSrcUser = SwitchViewChairID(i);
	//			GunStatus[wGunCounter].wDestUser = SwitchViewChairID(j);
	//			wGunCounter++;
	//		}

	//		//��עȫС
	//		if ((m_lScoreTimes[i][0] < m_lScoreTimes[j][0]) &&
	//			(m_lScoreTimes[i][1] < m_lScoreTimes[j][1]) &&
	//			(m_lScoreTimes[i][2] < m_lScoreTimes[j][2]) )
	//		{
	//			GunStatus[wGunCounter].wSrcUser = SwitchViewChairID(j);
	//			GunStatus[wGunCounter].wDestUser = SwitchViewChairID(i);
	//			wGunCounter++;
	//		}
	//	}
	//}
	if (wGunCounter != 0)
	{
		m_GameClientView.SetGunCartoonGroup(GunStatus, wGunCounter);
		m_GameClientView.StartGunCartoon(GunStatus[0].wSrcUser, GunStatus[0].wDestUser);
	}
	//////////////////////////////////////////////////////////////////////////

	if(GER_DISMISS == pGameEnd->bEndMode)
	{
		if (m_pIStringMessage != NULL) m_pIStringMessage->InsertSystemString(TEXT("��Ϸ��ɢ��"));
	}
	//���û���
	if (m_pIStringMessage != NULL) m_pIStringMessage->InsertSystemString(TEXT("���ֵ÷֣�"));
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		IClientUserItem * pClientUserItem=GetTableUserItem(i);
		
		
		//if (m_bGameStatus[i] == false) continue;
		WORD wViewChairID = SwitchViewChairID(i);
		//������

		if (pClientUserItem !=NULL)
		{
			
			m_GameClientView.SetGameScore(wViewChairID, pGameEnd->lGameScore[i]);
			m_GameClientView.SetScoreTimes(wViewChairID, pGameEnd->lScoreTimes[i]);

			CString strScoreRes ;
			strScoreRes.Format( TEXT("%s��%I64d\n") , pClientUserItem->GetNickName(),pGameEnd->lGameScore[i]);
			if(m_pIStringMessage != NULL) m_pIStringMessage->InsertNormalString( strScoreRes) ;

			//���ֿ�
			m_HistroyScore.OnEventUserScore(i, pGameEnd->lGameScore[i]);
		}
		else 
		{
			m_GameClientView.SetChip(wViewChairID, -1);
			m_GameClientView.SetGameScore(wViewChairID, -1);
			m_GameClientView.SetScoreTimes(wViewChairID, MAXLONGLONG);
		}
	}

	
	if(GER_NORMAL==pGameEnd->bEndMode)
	{
		//��ʾ��Ϣ	
		if(m_pIStringMessage != NULL) m_pIStringMessage->InsertSystemString( TEXT( "���ƽ����\n" )) ;
		//ׯ����Ϣ
		IClientUserItem *pBanker = GetTableUserItem(m_wBanker);

		for(WORD i=0 ; i<GAME_PLAYER ; ++i)
		{
			//��������
			BYTE bUserWin = FALSE; 
			BYTE bWindCount = 0 ;
			CString strWindMsg = TEXT("") ;

			IClientUserItem * pClientUserItem=GetTableUserItem(i);
			if (pClientUserItem == NULL || m_bGameStatus[i] == false) continue;

			TCHAR szBuffer[64];
			_sntprintf(szBuffer,CountArray(szBuffer),TEXT("\n%s:%+I64d "),pClientUserItem->GetNickName(), 
				pGameEnd->lGameScore[pClientUserItem->GetChairID()]);
			
			if(i!=m_wBanker)
			{
				bUserWin = pGameEnd->bWinInfo[i];
				
				//�м�Ӯ
				if (bUserWin == 2)
					strWindMsg = TEXT("[ ") + (CString(pClientUserItem->GetNickName()) + CString(TEXT(" ] Ӯ ׯ��[ ")) + CString(pBanker->GetNickName()) + TEXT(" ]\n")) ;
				else if (bUserWin == 0)
					strWindMsg = (CString(TEXT("ׯ��[ ")) + CString(pBanker->GetNickName()) + CString(TEXT(" ] Ӯ [ ")) + CString(pClientUserItem->GetNickName()) + TEXT(" ]\n")) ;
				else if (bUserWin == 1)
					strWindMsg = (CString(TEXT("ׯ��[ ")) + CString(pBanker->GetNickName()) + CString(TEXT(" ] �� [ ")) + CString(pClientUserItem->GetNickName()) + TEXT(" ] ���\n")) ;

				if (m_pIStringMessage != NULL) m_pIStringMessage->InsertNormalString(strWindMsg) ;
			}

			//��������
			if(i==GetMeChairID())
			{
				if(pGameEnd->lGameScore[i]>0) PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_WIN"));
				else PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_LOST"));
			}
		}
	}

	//���ý���
	if (IsLookonMode()==false)
	{
		//��ʼ��ť
		m_GameClientView.m_btStart.ShowWindow(SW_SHOW);
		m_GameClientView.m_btStart.EnableWindow(TRUE);

		SetGameClock(GetMeChairID(),IDI_START_GAME,m_cbTimeStartGame);
	}

	//���ñ���
	m_wBanker = INVALID_CHAIR;

	return true;
}

//��ʱ��
void CGameClientEngine::OnTimer(UINT nIDEvent)
{
	if (nIDEvent == IDI_FU_ZHU)		//����ʱ��
	{
		m_GameClientView.SetFuZhuTime(m_cbFuZhuTime--);
		m_GameClientView.RefreshGameView();
		if (m_cbFuZhuTime == 0)
		{
			KillTimer(IDI_FU_ZHU);
			for(BYTE i=0;i<GAME_PLAYER;i++)
			{
				if(i != m_wBanker && i != GetMeChairID() && m_bGameStatus[i] == true)
				{
					m_GameClientView.ShowFuZhuClock(SwitchViewChairID(i),false);
				}
			}
		}
	}

	if (nIDEvent == 1122)
	{
		m_GameClientView.RefreshGameView();
	}
	if (nIDEvent == IDI_ALL_RANGE_CARD)
	{	
		m_TimerCount--;

		if (m_bTrustee)
		{
			if (!m_bFinishSegment)
			{
				//��δ���ƻ�����δ���
				if (m_bHandCardCount!=0)
				{
					//�Զ�����
					AutomaticOutCard();
					//̯��
					OnShowCard(0 , 0);
					m_bFinishSegment = true;
					return;
				}
			}

			if (!m_bIsShowCard)
			{
				OnShowCard(0 , 0);
			}
		}

		if (m_TimerCount > 0)
		{
			m_GameClientView.SetElapseTime(m_TimerCount);
			m_GameClientView.RefreshGameView();
		}
		else
		{
			KillTimer(IDI_ALL_RANGE_CARD);
			m_GameClientView.SetGlobalClock(false);
			m_GameClientView.SetElapseTime(0);

			if(!m_bFinishSegment)
			{
				//��δ���ƻ�����δ���
				if (m_bHandCardCount!=0)
				{
					//�Զ�����
					AutomaticOutCard();
					//̯��
					OnShowCard(0 , 0);
					m_bFinishSegment = true;

				}
				else
				{
					OnShowCard(0 , 0);
					m_bFinishSegment = true;
				}
			}
			else
			{
				if (!m_bIsShowCard)
				{
					OnShowCard(0 , 0);
				}
			}

			//else
			//{
			//	//ֱ��̯��
			//	//OnShowCard(0, 0);
			//	//m_bFinishSegment = true;
			//}
		}
		m_GameClientView.RefreshGameView();
		return;
	}
	if(nIDEvent==IDI_COMPARE_FRONT)		//ǰ���˿�
	{
		KillTimer(m_nTimer) ;
		m_nTimer=SetTimer(IDI_COMPARE_MID , m_nCompareTime , NULL) ;

		//���ñ���
		CompareCard(enSegMid);

		return ;
	}

	if(nIDEvent==IDI_COMPARE_MID)		//�ж��˿�
	{
		KillTimer(m_nTimer) ;
		m_nTimer=SetTimer(IDI_COMPARE_BACK , m_nCompareTime , NULL) ;

		//���ñ���
		CompareCard(enSegBack);

		return  ;
	}

	if(nIDEvent==IDI_COMPARE_BACK)		//����˿�
	{
		KillTimer(m_nTimer) ;
		

		//��ʾ�˿�
		for(WORD i=0 ; i<GAME_PLAYER ; i++)
		{
			if (GetTableUserItem(i) == NULL || m_bGameStatus[i] == false) continue;

			WORD wViewChairID = SwitchViewChairID(i);
			m_GameClientView.m_AllSegCardControl[wViewChairID].SetDisplayFlag(true);
			m_GameClientView.m_AllSegCardControl[wViewChairID].ShowWindow(SW_SHOW);
		}

		m_nTimer=SetTimer(IDI_COMPLETE_COMPARE , 1800 , NULL) ;
		return ;
	}

	if(nIDEvent==IDI_COMPLETE_COMPARE)
	{
		KillTimer(m_nTimer) ;

		if(false==IsLookonMode()) SendSocketData(SUB_C_COMPLETE_COMPARE , NULL , 0) ;

		return ;
	}

	if(nIDEvent==IDI_CAN_PLAY_VOICE)
	{
		m_bCanPlayVoice = true;
		KillTimer(IDI_CAN_PLAY_VOICE);

		return;
	}
	__super::OnTimer(nIDEvent) ;
}

//�Զ�����
void CGameClientEngine::AutomaticOutCard() 
{
	//����ѹע
	if(-1==m_lUserChip[GetMeChairID()] && GetMeChairID()!=m_wBanker)
	{
		m_lUserChip[2] = min(300, 10) ;
		m_GameClientView.SetChip(2 , m_lUserChip[2], true) ;

		CMD_C_SetChip  SetChip ;
		ZeroMemory(&SetChip, sizeof(SetChip));
		SetChip.nChip = m_lUserChip[2] ;
		SendSocketData(SUB_C_SETCHIP , &SetChip , sizeof(SetChip)) ;
	}

	//�ָ��˿�
	CopyMemory(m_bHandCardData[GetMeChairID()] , m_bTmpHandCardData , sizeof(m_bHandCardData[GetMeChairID()])) ; //��ֹ��ҷ���һ�λ����κ�ʱ���ô˺���
	m_bHandCardCount = HAND_CARD_COUNT ;
	m_GameLogic.SortCardList(m_bHandCardData[GetMeChairID()], m_bHandCardCount, enDescend);
	m_GameClientView.m_UserCardControl[2].SetCardData(m_bHandCardData[GetMeChairID()] , m_bHandCardCount);
	m_GameClientView.m_UserCardControl[2].ShowWindow(SW_SHOW);
	m_GameClientView.m_AllSegCardControl[2].SetCardData(NULL, 0);

	//�ֶ��˿�
	m_GameClientView.m_CenterCardControl.SetCardData(-1, NULL, 0);

	m_bFinishSegment = false;
	m_tagSegInfo.Init();

	//�Զ�����
	BYTE bAutoPutCard[13], bIndex[13];
	ZeroMemory(bAutoPutCard, sizeof(bAutoPutCard));
	ZeroMemory(bIndex, sizeof(bIndex));

	m_GameLogic.AutoPutCard(m_bHandCardData[GetMeChairID()], bAutoPutCard, false,false);

	//��Ӧ����
	for (int i = 0; i < 13; i++)
	{
		for (int j = 0; j < 13; j++)
		{
			if (bAutoPutCard[i] == m_bHandCardData[GetMeChairID()][j])
			{
				bIndex[i] = j;
				break;
			}
		}
	}

	//����ǰ��
	m_GameClientView.m_UserCardControl[2].SetShootByIndex(bIndex, 3);
	OnFrontCard(0, 0);

	//��Ӧ����
	for (int i = 3; i < 13; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			if (bAutoPutCard[i] == m_bHandCardData[GetMeChairID()][j])
			{
				bIndex[i] = j;
				break;
			}
		}
	}

	//�����ж�
	m_GameClientView.m_UserCardControl[2].SetShootByIndex(bIndex+3, 5);
	OnMidCard(0, 0);

	return;
}

//��ʾ����
void CGameClientEngine::ShowAutomaticOutCard(WORD wChairID) 
{
	WORD wViewChairID = SwitchViewChairID(wChairID);

	//�Զ�����
	BYTE bAutoPutCard[13], bIndex[13];
	ZeroMemory(bAutoPutCard, sizeof(bAutoPutCard));
	ZeroMemory(bIndex, sizeof(bIndex));

	m_GameLogic.AutoPutCard(m_bHandCardData[wChairID], bAutoPutCard, true, false);

	m_GameClientView.m_AllSegCardControl[wViewChairID].SetCardData(bAutoPutCard, 13);
	m_GameClientView.m_AllSegCardControl[wViewChairID].SetDisplayFlag(true);
}

//////////////////////////////////////////////////////////////////////////

//���Ƴ���
void CGameClientEngine::CompareCard(BYTE bSceneName)
{
	//��ʾ��Ϣ
	m_GameClientView.SetCompareScene((enSegmentType)bSceneName);

	//���ƶ���
	m_GameClientView.ReversalCard(SwitchViewChairID(m_wBanker), (bSceneName==enSegFront)?3:5);

	////����Թ�
	//if(true==IsLookonMode())
	//{
	//	WORD  wViewChairID = SwitchViewChairID(m_wBanker) ;
	//	m_GameClientView.m_AllSegCardControl[wViewChairID].SetDisplayFlag(true) ;
	//}
}

//���°�ť

void CGameClientEngine::UpdateButtonState()
{		
	//������ʾ
	for (UINT i = 0; i < 8; i++)
		m_GameClientView.m_btPrompt[i].EnableWindow(FALSE);
	
	if (IsLookonMode() == true)
	{
		//m_GameClientView.m_btSplitCard.EnableWindow(FALSE);
		m_GameClientView.m_btTrustee.EnableWindow(FALSE);
		m_GameClientView.m_btSortCard.EnableWindow(FALSE);
	}
	
	if ((m_bHandCardCount <= 0) || GetGameStatus() != GS_WK_PLAYING)
	{
		//��������
		m_GameClientView.m_btSortCard.EnableWindow(FALSE);
		
		//��������
		//m_GameClientView.m_btSplitCard.EnableWindow(FALSE);

		//��������
		m_GameClientView.m_btFrontCard.ShowWindow(SW_HIDE);
		m_GameClientView.m_btFrontCard.EnableWindow(FALSE);
		m_GameClientView.m_btMidCard.ShowWindow(SW_HIDE);
		m_GameClientView.m_btMidCard.EnableWindow(FALSE);
		m_GameClientView.m_btBackCard.ShowWindow(SW_HIDE);
		m_GameClientView.m_btBackCard.EnableWindow(FALSE);

		return;
	}

	//
	OnPrompt(-1, 0);

	//�԰�ť
	m_GameLogic.SearchSameCard(m_bHandCardData[GetMeChairID()], m_bHandCardCount, 2, &m_SearchCardResult);
	if (m_SearchCardResult.cbSearchCount >= 2)
	{
		m_GameClientView.m_btPrompt[1].EnableWindow(TRUE);
		m_GameClientView.m_btPrompt[0].EnableWindow(TRUE);
	}

	if (m_SearchCardResult.cbSearchCount == 1)
	{
		m_GameClientView.m_btPrompt[0].EnableWindow(TRUE);
	}

	//������ť
	m_GameLogic.SearchSameCard(m_bHandCardData[GetMeChairID()], m_bHandCardCount, 3, &m_SearchCardResult);
	if (m_SearchCardResult.cbSearchCount >= 1)
	{
		m_GameClientView.m_btPrompt[2].EnableWindow(TRUE);
	}

	//˳�Ӱ�ť
	m_GameLogic.SearchLineCardType(m_bHandCardData[GetMeChairID()], m_bHandCardCount, 5, &m_SearchCardResult);
	if (m_SearchCardResult.cbSearchCount > 0)
	{
		m_GameClientView.m_btPrompt[3].EnableWindow(TRUE);
	}

	//ͬ����ť
	m_GameLogic.SearchSameColorType(m_bHandCardData[GetMeChairID()], m_bHandCardCount, 5, &m_SearchCardResult);
	if (m_SearchCardResult.cbSearchCount > 0)
	{
		m_GameClientView.m_btPrompt[4].EnableWindow(TRUE);
	}

	//��«��ť
	m_GameLogic.SearchTakeCardType(m_bHandCardData[GetMeChairID()], m_bHandCardCount, 3, 2, &m_SearchCardResult);
	if (m_SearchCardResult.cbSearchCount > 0)
	{
		m_GameClientView.m_btPrompt[5].EnableWindow(TRUE);
	}

	//��֧��ť
	m_GameLogic.SearchTakeCardType(m_bHandCardData[GetMeChairID()], m_bHandCardCount, 4, 1, &m_SearchCardResult);
	if (m_SearchCardResult.cbSearchCount > 0)
	{
		m_GameClientView.m_btPrompt[6].EnableWindow(TRUE);
	}

	//ͬ��˳��ť
	m_GameLogic.SearchSameColorLineType(m_bHandCardData[GetMeChairID()], m_bHandCardCount, 5, &m_SearchCardResult);
	if (m_SearchCardResult.cbSearchCount > 0)
	{
		m_GameClientView.m_btPrompt[7].EnableWindow(TRUE);
	}
}

//������ע��ť
VOID CGameClientEngine::UpdateChipButtonState(int nShowCmd, BOOL bEnable)
{
	for (UINT i = 0; i < 4; i++)
	{
		m_GameClientView.m_btSetChip[i].ShowWindow(nShowCmd);
		m_GameClientView.m_btSetChip[i].EnableWindow(bEnable);
	}
}

//���°�ť
void CGameClientEngine::UpdateBlockButton(BYTE cbBtnIndex, int nShowCmd, BOOL bEnable)
{
	if (0 == cbBtnIndex)
	{
		m_GameClientView.m_btFrontCard.ShowWindow(nShowCmd);
		m_GameClientView.m_btFrontCard.EnableWindow(bEnable);
		m_GameClientView.m_btMidCard.ShowWindow(nShowCmd);
		m_GameClientView.m_btMidCard.EnableWindow(bEnable);
		m_GameClientView.m_btBackCard.ShowWindow(nShowCmd);
		m_GameClientView.m_btBackCard.EnableWindow(bEnable);
	}

	if(1 == cbBtnIndex)
	{
		m_GameClientView.m_btFrontCard.ShowWindow(nShowCmd);
		m_GameClientView.m_btFrontCard.EnableWindow(bEnable);
	}

	if(2 == cbBtnIndex)
	{
		m_GameClientView.m_btMidCard.ShowWindow(nShowCmd);
		m_GameClientView.m_btMidCard.EnableWindow(bEnable);
	}

	if(3 == cbBtnIndex)
	{
		m_GameClientView.m_btBackCard.ShowWindow(nShowCmd);
		m_GameClientView.m_btBackCard.EnableWindow(bEnable);
	}
}

//��ȡ����
LPCTSTR CGameClientEngine::GetPhraseString(BYTE bGender,BYTE bType,BYTE bIndex)
{
	//��ȡ��ǰ����·��
	TCHAR szExePath[MAX_PATH] = {};
	GetModuleFileName( NULL,szExePath,CountArray(szExePath) );

	CString szTemp;
	szTemp = szExePath;
	int nPos = szTemp.ReverseFind(TEXT('\\'));
	_sntprintf(szExePath, CountArray(szExePath), TEXT("%s"), szTemp.Left(nPos));

	//��ȡ�����ļ�
	TCHAR szConfigFileName[MAX_PATH]=TEXT("");
	_sntprintf( szConfigFileName,CountArray(szConfigFileName),TEXT("%s\\PhraseVoice.ini"),szExePath );

	//���ض���
	TCHAR szConfigFileCount[MAX_PATH]=TEXT("");
	INT nPhraseCount = GetPrivateProfileInt( TEXT("PhraseInfo"),szConfigFileCount,0,szConfigFileName );
	TCHAR szItemName[MAX_PATH]=TEXT("");
	TCHAR szType[MAX_PATH]=TEXT("");
	TCHAR szBuffer[MAX_PATH]=TEXT("");

	//������
	_sntprintf( szItemName,CountArray(szItemName),TEXT("Phrase%ld"),bIndex);

	if(bGender == 1)
	{
		_sntprintf( szConfigFileCount,CountArray(szConfigFileCount),TEXT("BoyCount%d"),bType );
		_sntprintf( szType,CountArray(szType),TEXT("BoyPhrase%ld"),bType );
	}
	else
	{
		_sntprintf( szConfigFileCount,CountArray(szConfigFileCount),TEXT("GirlCount%d"),bType );
		_sntprintf( szType,CountArray(szType),TEXT("GirlPhrase%ld"),bType );
	}

	//�����ִ�
	GetPrivateProfileString( szType,szItemName,TEXT(""),szBuffer,CountArray(szBuffer),szConfigFileName );

	return szBuffer;
}

//�������
BYTE CGameClientEngine::RandYuYin(BYTE bType,BYTE bGender)
{
	//��ȡ��ǰ����·��
	TCHAR szExePath[MAX_PATH] = {};
	GetModuleFileName( NULL,szExePath,CountArray(szExePath) );

	CString szTemp;
	szTemp = szExePath;
	int nPos = szTemp.ReverseFind(TEXT('\\'));
	_sntprintf(szExePath, CountArray(szExePath), TEXT("%s"), szTemp.Left(nPos));

	//��ȡ�����ļ�
	TCHAR szConfigFileName[MAX_PATH] = {};
	_sntprintf( szConfigFileName,CountArray(szConfigFileName),TEXT("%s\\PhraseVoice.ini"),szExePath );

	//���ض���
	TCHAR szConfigFileCount[MAX_PATH]={};
	INT nPhraseCount = GetPrivateProfileInt( TEXT("PhraseInfo"),szConfigFileCount,0,szConfigFileName );
	TCHAR szItemName[256] = {};
	TCHAR szBuffer[MAX_PATH] = {};

	if(bGender == 1)
	{
		_sntprintf( szConfigFileCount,CountArray(szConfigFileCount),TEXT("BoyCount%d"),bType );
		_sntprintf( szItemName,CountArray(szItemName),TEXT("BoyPhrase%ld"),bType );
		nPhraseCount = GetPrivateProfileInt( TEXT("PhraseInfo"),szConfigFileCount,0,szConfigFileName );
	}
	else
	{
		_sntprintf( szConfigFileCount,CountArray(szConfigFileCount),TEXT("GirlCount%d"),bType );
		_sntprintf( szItemName,CountArray(szItemName),TEXT("GirlPhrase%ld"),bType );
		nPhraseCount = GetPrivateProfileInt( TEXT("PhraseInfo"),szConfigFileCount,0,szConfigFileName );
	}

	return rand()%nPhraseCount;
}

int CGameClientEngine::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
	__super::OnCreate(lpCreateStruct) ;



	return 1;
}
