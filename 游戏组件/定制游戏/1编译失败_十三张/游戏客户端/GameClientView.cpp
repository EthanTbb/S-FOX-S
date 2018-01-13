#include "StdAfx.h"
#include "Resource.h"
#include "GameClientView.h"

//////////////////////////////////////////////////////////////////////////
//��ť��ʶ
#define IDC_LOOK_SCORE						99								//��ʷ�ɼ�
#define	IDC_START							100								//��ʼ��ť
#define IDC_CALL_BANKER						101								//��ׯ��ť
#define IDC_NO_CALL_BANKER					102								//���а�ť

#define IDC_SET_CHIP_0						103								//ѹע��ť
#define IDC_SET_CHIP_1						104								//ѹע��ť
#define IDC_SET_CHIP_2						105								//ѹע��ť
#define IDC_SET_CHIP_3						106								//ѹע��ť

#define IDC_FRONT_CARD						107								//��һע��ť
#define IDC_MID_CARD						108								//�ڶ�ע��ť
#define IDC_BACK_CARD						109								//����ע��ť
#define IDC_SHOW_CARD						110								//ȷ����ť

#define IDC_PROMPT_1						111								//��ʾ��ť
#define IDC_PROMPT_2						112								//��ʾ��ť
#define IDC_PROMPT_3						113								//��ʾ��ť
#define IDC_PROMPT_4						114								//��ʾ��ť
#define IDC_PROMPT_5						115								//��ʾ��ť
#define IDC_PROMPT_6						116								//��ʾ��ť
#define IDC_PROMPT_7						117								//��ʾ��ť
#define IDC_PROMPT_8						118								//��ʾ��ť
#define IDC_SPLIT_CARD						119								//���ư�ť
#define IDC_SORT_CARD						120								//����ť
#define IDC_TRUSTEE							121                             //�йܰ�ť

#define IDC_RESORE_0						122								//�ջذ�ť
#define IDC_RESORE_1						123								//�ջذ�ť
#define IDC_RESORE_2						124								//�ջذ�ť
#define IDC_RESORE_3						125								//�ջذ�ť

#define	IDC_CHOSE_YUYIN0					500								//���鰴ť
#define	IDC_CHOSE_YUYIN1					501								//���鰴ť
#define	IDC_CHOSE_YUYIN2					502								//���鰴ť
#define	IDC_CHOSE_YUYIN3					503								//���鰴ť
#define IDC_YU_YIN_OPEN						600								//�򿪰�ť


//////////////////////////////////////////////////////////////////////////
//��ʱ��
#define IDI_SHUFFLE_CARD					1								//ϴ�ƶ�ʱ��
#define IDI_SPLIT_CARD						2								//���ƶ�ʱ��
#define IDI_REVERSAL_CARD					3								//���ƶ�ʱ��
#define IDI_REVERSAL_DELAY					4								//��ʱ��ʱ��
#define IDI_DISPATCH_CARD					5								//���ƶ�ʱ��
#define IDI_JETTON_ACTION					6								//���붨ʱ��
#define IDI_CARD_TIP						7								//��ʾ��ʱ��
#define IDI_SWAT_CARTOON					8								//ȫ�ݴ򶯻�

#define IDI_START_GUN_CARTOON				9								//��ʼ��ǹ
#define IDI_GUN_FIRE						10								//��ǹ��ʱ��
#define IDI_BULLUT_HOLE						11								//�ӵ���ʱ��

//ϴ�ƶ���
#define SHUFFLE_X_COUNT						4								//������Ŀ
#define SHUFFLE_Y_COUNT						6								//������Ŀ
#define SHUFFLE_TOTAL_COUNT					SHUFFLE_X_COUNT*SHUFFLE_Y_COUNT //�ܹ���Ŀ

//���ƶ���
#define SPLIT_X_COUNT						9								//������Ŀ

//���ƶ���
#define REVERSAL_X_COUNT					5								//������Ŀ

//���ƶ���
#define DISPATCH_CARD_TIMES					7								//���ƴ���
#define DISPATCH_CARD_SPACE					2								//���Ƽ��
#define DISPATCH_HEAP_DISTANCE				3								//�ƶѼ��

//ȫ�ݴ򶯻�
#define SWAT_CARTOON_X						14								//������Ŀ

#define RAND_INDEX							RAND_MAX						//�����
#define RAND_FACTOR							4								//�������
//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameClientView, CGameFrameViewGDI)
	ON_WM_CREATE()
	ON_WM_TIMER()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_MOUSEMOVE()
	ON_WM_SETCURSOR()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameClientView::CGameClientView(void) 
{
	m_wMouseDownItem=INVALID_ITEM;
	m_wHoverCardItem=INVALID_ITEM;
	m_bMoveMouse = false;
	m_bVoicePanelStatus=false;

	//��Ϸ����
	m_wBanker=INVALID_CHAIR ;
	for(WORD i=0 ; i<GAME_PLAYER ; ++i) m_lUserChip[i] = -1;
	for(WORD i=0 ; i<GAME_PLAYER ; ++i) m_lGameScore[i] = -1 ;
	for(WORD i=0 ; i<GAME_PLAYER ; ++i) m_lScoreTimes[i] = MAXLONGLONG ;
	m_bShowCardFrame = false;
	m_bShowHistory = false;
	m_enSegmentType = enSegErr ;
	m_wMaxChip = 0 ;
	ZeroMemory(m_bCallBankerStatus, sizeof(m_bCallBankerStatus));
	ZeroMemory(m_bGameStatus, sizeof(m_bGameStatus));
	ZeroMemory(m_lChipArray, sizeof(m_lChipArray));
	ZeroMemory(&m_pHistoryScore, sizeof(m_pHistoryScore));
	ZeroMemory(m_szSoundPath, sizeof(m_szSoundPath));
	ZeroMemory(m_bShowSpecType,sizeof(m_bShowSpecType));
	ZeroMemory(m_cbSpecType,sizeof(m_cbSpecType));
		
	//����ʱ��
	ZeroMemory(m_bShowFuZhuClock, sizeof(m_bShowFuZhuClock));
	m_dwFuZhuTime=0;
	
	m_cbTipInfoState=0;

	//��ʾ����
	m_bShowTips = false;
	m_bGlobalClock = false;
	m_dwElapseTime = 0;
	//���붯��
	ZeroMemory(m_JettonStatus,sizeof(m_JettonStatus));
	m_bJettonAction = false;
	ZeroMemory(&m_JettonData, sizeof(m_JettonData));

	//ϴ�ƶ���
	m_bShuffleCard = false;
	m_wShuffleIndex = 0;

	//���ƶ���
	m_bSplitCard = false;
	m_wSplitIndex = 0;

	//���ƶ���
	m_bReversalCard = false;
	m_cbReversalCount=0;
	m_wReversalIndex = 0;
	m_wReversalUser = INVALID_CHAIR;
	m_cbCardType = CT_INVALID;
	m_bShowType = false;

	//���ƶ���
	m_wStartDispatcher=INVALID_CHAIR;
	m_bDispatchCard=false;
	m_wDispatchCardIndex=0L;
	ZeroMemory(m_cbHandCardData,sizeof(m_cbHandCardData));

	//ȫ�ݴ򶯻�
	m_wSwatUser = INVALID_CHAIR;
	m_bSwatCartoon = false;
	m_wSwatIndex = 0;

	//��ǹ����
	FillMemory(&m_GunStatus, sizeof(tagGunStatus)*GAME_PLAYER, INVALID_CHAIR);
	m_wGroupCount = 0;
	m_wCurrentIndex = 0;
	m_bGunCartoon = false;
	m_bBullet = false;
    	
	return;
}

//��������
CGameClientView::~CGameClientView(void)
{
	m_FontChip.DeleteFont();
	m_FontComp.DeleteFont();
}

//������Ϣ
INT CGameClientView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct)==-1) return -1;

	//��ȡ·��
	GetCurrentDirectory(MAX_PATH, m_szSoundPath);
	_sntprintf(m_szSoundPath, CountArray(m_szSoundPath), TEXT("%s\\%s"), m_szSoundPath, TEXT("Thirteen\\Sound"));

	//������Դ
	HINSTANCE hResInstance = AfxGetInstanceHandle();

	//BMPͼƬ
	m_ImageBack.LoadFromResource(hResInstance, IDB_VIEW_BACK);
	m_ImageCenter.LoadFromResource(hResInstance, IDB_VIEW_CENTER);

	//PngͼƬ
	m_PngUserHeadBackH.LoadImage(hResInstance, TEXT("USER_HEAD_BACK_H"));
	m_PngUserHeadBackV.LoadImage(hResInstance, TEXT("USER_HEAD_BACK_V"));
	m_PngUserInfo.LoadImage(hResInstance, TEXT("USER_INFO"));
	m_PngTrusteeFlag.LoadImage(hResInstance, TEXT("TRUSTEE_FLAG"));
	m_PngHistoryScore.LoadImage(hResInstance, TEXT("HISTORY_SCORE"));
	m_PngChipNumer.LoadImage(hResInstance, TEXT("CHIP_NUMBER"));
	m_PngResultNumber.LoadImage(hResInstance, TEXT("RESULT_NUMBER"));
	m_PngReadyFlag.LoadImage(hResInstance, TEXT("READY_FLAG"));
	m_PngShuffleCard.LoadImage(hResInstance, TEXT("SHUFFLE_CARD"));
	m_PngSplitCard.LoadImage(hResInstance, TEXT("SPLIT_CARD"));
	m_PngSwatCartoon.LoadImage(hResInstance, TEXT("SWAT_CARTOON"));
	m_PngMovingCard.LoadImage(hResInstance, TEXT("MOVING_CARD"));
	m_PngNoCallBanker.LoadImage(hResInstance, TEXT("NO_CALL_BANKER_FLAG"));
	m_PngBankerFlag.LoadImage(hResInstance, TEXT("BANKER_FLAG"));
	m_PngReversalCard.LoadImage(hResInstance, TEXT("REVERSAL_CARD"));
	m_PngCardFrame.LoadImage(hResInstance, TEXT("CARD_FRAME"));
	m_PngCardTip.LoadImage(hResInstance, TEXT("BAD_CARD_TIP"));
	m_PngVoicePanelBack.LoadImage(hResInstance, TEXT("EXPRESSION_BACK_OPEN"));
	m_PngShowUserChip.LoadImage(hResInstance, TEXT("USER_CHIP"));

	m_PngGun.LoadImage(hResInstance, TEXT("GUN"));
	m_PngGunFire.LoadImage(hResInstance, TEXT("GUN_FIRE"));
	m_PngGunHole[0].LoadImage(hResInstance, TEXT("GUN_HOLE_1"));
	m_PngGunHole[1].LoadImage(hResInstance, TEXT("GUN_HOLE_2"));

	m_PngTimeNumber.LoadImage(hResInstance, TEXT("TIMER_NUMBER"));
	m_PngTimerFlag.LoadImage(hResInstance, TEXT("TIMER_FLAG"));
	m_PngDirectionOp.LoadImage(hResInstance, TEXT("DIRECTION_OPERATE"));
	
	m_PngTipCallBanker.LoadImage(hResInstance, TEXT("TIP_CALL_BANKER"));
	m_PngTipCallScore.LoadImage(hResInstance, TEXT("TIP_CALL_SCORE"));

	//������Ϣ��
	m_MsgBox.Create(IDD_MESSAGE_BOX, this);

	//��������
	m_FontChip.CreateFont(this, TEXT("����"), 14, 400);
	m_FontComp.CreateFont(this, TEXT("����"), 22, 600);
  
	//�����˿�
	CCardControl::Load();
	for (WORD i=0 ; i<GAME_PLAYER ; i++)
	{
		//���ÿؼ�
		if (0 == i)
		{
			m_UserCardControl[i].SetDirection(true);
			m_UserCardControl[i].SetCardSpace(16 , 10 , 0) ;
		}
		else if ((1 == i) || (3 == i))
		{
			m_UserCardControl[i].SetDirection(false);
			m_UserCardControl[i].SetCardSpace(20 , 20 , 0) ;
		}
		else 
		{
			m_UserCardControl[i].SetDisplayFlag(true);
			m_UserCardControl[i].SetPositively(true);
			m_UserCardControl[i].SetCardSpace(20, 20, 20);
		}

		//���зֶ�
		m_AllSegCardControl[i].SetDisplayFlag(true);
		m_AllSegCardControl[i].SetCardSpace(18, 30);
	}

	//
	m_CenterCardControl.SetDisplayFlag(true);
	m_CenterCardControl.SetCardSpace(18, 30);

	//�ƶ��˿�
	m_HeapCardControl.SetDirection(true);
	m_HeapCardControl.SetCardSpace(2, 0, 0);
	
	//�����ؼ�
	CRect CreateRect(0,0,0,0);
	m_btLookScore.Create(TEXT(""), WS_CHILD|WS_VISIBLE, CreateRect, this, IDC_LOOK_SCORE);
	m_btStart.Create(TEXT(""),WS_CHILD,CreateRect,this,IDC_START);
	m_btFrontCard.Create(TEXT(""),WS_CHILD,CreateRect,this,IDC_FRONT_CARD);
	m_btMidCard.Create(TEXT(""),WS_CHILD,CreateRect,this,IDC_MID_CARD);	
	m_btBackCard.Create(TEXT(""),WS_CHILD,CreateRect,this,IDC_BACK_CARD);
	m_btShowCard.Create(TEXT(""),WS_CHILD,CreateRect,this,IDC_SHOW_CARD);
	m_btCallBanker.Create( TEXT( "" ), WS_CHILD, CreateRect, this, IDC_CALL_BANKER );
	m_btNoCallBanker.Create( TEXT( "" ), WS_CHILD, CreateRect, this, IDC_NO_CALL_BANKER );
	
	//��ע��ť
	for (INT i = 0; i < 4; i++)
		m_btSetChip[i].Create(TEXT(""), WS_CHILD, CreateRect, this, IDC_SET_CHIP_0+i);

	//��ʾ��ť
	for (INT i = 0; i < 8; i++)
		m_btPrompt[i].Create(TEXT(""), WS_CHILD|WS_VISIBLE, CreateRect, this, IDC_PROMPT_1+i);

	//���ܰ�ť
	m_btSortCard.Create(TEXT(""), WS_CHILD|WS_VISIBLE, CreateRect, this, IDC_SORT_CARD);
	//m_btSplitCard.Create(TEXT(""), WS_CHILD|WS_VISIBLE, CreateRect, this, IDC_SPLIT_CARD);
	m_btTrustee.Create(TEXT(""),WS_CHILD|WS_VISIBLE, CreateRect, this, IDC_TRUSTEE);
	//m_btVoice.Create(TEXT(""), WS_CHILD|WS_VISIBLE, CreateRect, this, IDC_YU_YIN_OPEN);

	//�ջذ�ť
	for (INT i = 0; i < 4; i++)
		m_btRestore[i].Create(TEXT(""),WS_CHILD, CreateRect, this, IDC_RESORE_0+i);

	//�������
	//m_btYuYin0.Create(TEXT(""), WS_CHILD, CreateRect, this, IDC_CHOSE_YUYIN0);
	//m_btYuYin1.Create(TEXT(""), WS_CHILD, CreateRect, this, IDC_CHOSE_YUYIN1);
	//m_btYuYin2.Create(TEXT(""), WS_CHILD, CreateRect, this, IDC_CHOSE_YUYIN2);
	//m_btYuYin3.Create(TEXT(""), WS_CHILD, CreateRect, this, IDC_CHOSE_YUYIN3);
    
	//����λͼ
	m_btLookScore.SetButtonImage(IDB_BT_SCORE_SHOW, hResInstance, false, false);
	m_btStart.SetButtonImage(IDB_BT_START,hResInstance,false,false);
	m_btFrontCard.SetButtonImage(IDB_BT_FRONT_CARD,hResInstance,false,false);
	m_btMidCard.SetButtonImage(IDB_BT_MID_CARD,hResInstance,false,false);
	m_btBackCard.SetButtonImage(IDB_BT_BACK_CARD,hResInstance,false,false);
	m_btShowCard.SetButtonImage(IDB_BT_SHOW_CARD,hResInstance,false,false);
	m_btCallBanker.SetButtonImage(IDB_BT_CALL_BANKER, hResInstance, false ,false) ;
	m_btNoCallBanker.SetButtonImage(IDB_BT_NO_CALL_BANKER, hResInstance, false ,false);
	//m_btSplitCard.SetButtonImage(IDB_BT_SPLICT_CARD, hResInstance, false ,false);
	//m_btVoice.SetButtonImage(IDB_BT_VOICE, hResInstance, false, false);

	//��ע��ť
	for (int i = 0; i < 4; i++)
		m_btSetChip[i].SetButtonImage(IDB_BT_SETCHIP,hResInstance,false,false);
	
	//��ʾ��ť
	m_btPrompt[0].SetButtonImage(IDB_BT_PROMPT_1, hResInstance, false, false);
	m_btPrompt[1].SetButtonImage(IDB_BT_PROMPT_2, hResInstance, false, false);
	m_btPrompt[2].SetButtonImage(IDB_BT_PROMPT_3, hResInstance, false, false);
	m_btPrompt[3].SetButtonImage(IDB_BT_PROMPT_4, hResInstance, false, false);
	m_btPrompt[4].SetButtonImage(IDB_BT_PROMPT_5, hResInstance, false, false);
	m_btPrompt[5].SetButtonImage(IDB_BT_PROMPT_6, hResInstance, false, false);
	m_btPrompt[6].SetButtonImage(IDB_BT_PROMPT_7, hResInstance, false, false);
	m_btPrompt[7].SetButtonImage(IDB_BT_PROMPT_8, hResInstance, false, false);
	m_btSortCard.SetButtonImage(IDB_BT_SORT, hResInstance, false, false);
	m_btTrustee.SetButtonImage(IDB_BT_TRUSTEE_OPEN, hResInstance, false, false);

	//�ջذ�ť
	m_btRestore[0].SetButtonImage(IDB_BT_RESTORE_ONE,hResInstance,false,false);
	m_btRestore[1].SetButtonImage(IDB_BT_RESTORE_ONE,hResInstance,false,false);
	m_btRestore[2].SetButtonImage(IDB_BT_RESTORE_ONE,hResInstance,false,false);
	m_btRestore[3].SetButtonImage(IDB_BT_RESTORE_ALL,hResInstance,false,false);

	//�������
	//m_btYuYin0.SetButtonImage(IDB_BT_EXPRESSION_0, hResInstance, false, false);
	//m_btYuYin1.SetButtonImage(IDB_BT_EXPRESSION_1, hResInstance, false, false);
	//m_btYuYin2.SetButtonImage(IDB_BT_EXPRESSION_2, hResInstance, false, false);
	//m_btYuYin3.SetButtonImage(IDB_BT_EXPRESSION_3, hResInstance, false, false);

    
	//���ð�ť
	for (UINT i = 0; i < 8; i++)
		m_btPrompt[i].EnableWindow(FALSE);
	m_btSortCard.EnableWindow(FALSE);
	//m_btSplitCard.EnableWindow(FALSE);
	//m_btVoice.EnableWindow(FALSE);
	m_btTrustee.EnableWindow(TRUE);

	return 0;
}

//��ʱ��
VOID CGameClientView::OnTimer(UINT nIDEvent)
{
	switch(nIDEvent)
	{

	case IDI_SHUFFLE_CARD:
		{
			//ֹͣ�ж�
			if (m_wShuffleIndex >= (SHUFFLE_TOTAL_COUNT+2))
			{			
				//ϴ�����
				KillTimer(IDI_SHUFFLE_CARD);
				m_bShuffleCard = false;
				m_wShuffleIndex = 0;

				//������Ϣ
				SendEngineMessage(IDM_SHUFFLE_FINISH, 0, 0);

				return;
			}
			else m_wShuffleIndex++;
			
			//���½���
			RefreshGameView();

			return;
		}
	case IDI_SPLIT_CARD:
		{
			//ֹͣ�ж�
			if (m_wSplitIndex >= SPLIT_X_COUNT)
			{			
				//�������
				KillTimer(IDI_SPLIT_CARD);
				m_bSplitCard = false;
				m_wSplitIndex = 0;

				//������Ϣ
				SendEngineMessage(IDM_SPLIT_FINISH, 0, 0);

				return;
			}
			else m_wSplitIndex++;

			//���½���
			RefreshGameView();

			return;
		}
	case IDI_REVERSAL_CARD:
		{				
			//��ȡ����λ��
			if (m_wReversalIndex == 0)
			{
				if (m_enSegmentType != enSegErr)
				{
					m_AllSegCardControl[m_wReversalUser].ShowWindow(SW_HIDE, m_enSegmentType);
					m_ptReversal = m_AllSegCardControl[m_wReversalUser].GetOrignPoint(m_enSegmentType);		
				}
				//��������
				m_bReversalCard = true;
				m_bShowType = true;

				//��ʾ����
				BYTE cbCardData[5];
				ZeroMemory(cbCardData, sizeof(cbCardData));
				WORD wCardCount = m_AllSegCardControl[m_wReversalUser].GetCardData(m_enSegmentType, cbCardData, 5);

				//��ȡ����
				BYTE cbMaxCard = 0;
				m_cbCardType = m_GameLogic.GetCardType(cbCardData, (BYTE)wCardCount, cbMaxCard);

				//������ʾ
				SetCardType(m_cbCardType);

				//��������
				PlayCardTypeSound(m_wReversalUser, m_cbCardType, cbMaxCard);
			}
			m_wReversalIndex++;

			//���һ�η���
			if (m_wReversalIndex >= REVERSAL_X_COUNT)
			{	
				m_bReversalCard = false;
				m_wReversalIndex = 0;

				//�ָ��˿�
				if (m_enSegmentType != enSegErr)
				{
					m_AllSegCardControl[m_wReversalUser].ShowWindow(SW_SHOW, m_enSegmentType);
					m_AllSegCardControl[m_wReversalUser].SetDisplayFlag(true, m_enSegmentType);
				}

				//ˢ�½���
				RefreshGameView();

				//��ʱ���л����¼�
				KillTimer(IDI_REVERSAL_CARD);
				SetTimer(IDI_REVERSAL_DELAY, 900, NULL);	
				return;
			}

			//���½���
			RefreshGameView();

			return;
		}
	case IDI_REVERSAL_DELAY:
		{			
			//ֹͣ��ʱ
			KillTimer(IDI_REVERSAL_DELAY);

			//�л����¼�
			while (true)
			{
				//�ֻ����
				m_wReversalUser = ( m_wReversalUser + 1 ) % GAME_PLAYER ;
				if (m_bGameStatus[m_wReversalUser] == false) continue;
				if (GetClientUserItem(m_wReversalUser) != NULL || m_wBanker == m_wReversalUser) break ;
			}

			//���һ�ַ���
			if (m_wBanker == m_wReversalUser)
			{	
				KillTimer(IDI_REVERSAL_CARD);			
				m_bReversalCard = false;
				m_wReversalIndex = 0;
				m_bShowType = false;

				//�����ֱ��ƽ��
				SendEngineMessage(IDM_REVERSAL_TURN_FINISH, m_enSegmentType, 0);

				//ˢ�½���
				RefreshGameView();
				return;
			}

			//���¼���
			ReversalCard(m_wReversalUser, 5);

			//ˢ�½���
			RefreshGameView();
			return;
		}
	case IDI_DISPATCH_CARD:
		{			
			//���ñ���
			m_wDispatchCardIndex++;

			//��������
			WORD wBaseIndex=DISPATCH_CARD_SPACE+DISPATCH_CARD_TIMES;

			//�����˿�
			if ((m_wDispatchCardIndex>=wBaseIndex)&&((m_wDispatchCardIndex-wBaseIndex)%DISPATCH_CARD_SPACE==0))
			{
				//��������
				WORD wCardIndex=m_wDispatchCardIndex-wBaseIndex;
				WORD wUserIndex=(m_wStartDispatcher+wCardIndex/DISPATCH_CARD_SPACE)%GAME_PLAYER;
				
				if ((GetClientUserItem(wUserIndex) != NULL) && (m_bGameStatus[wUserIndex] != false))
				{
					//�����˿�
					if (wUserIndex==ME_VIEW_CHAIR)
					{
						//��ȡ�˿�
						BYTE cbCardData[HAND_CARD_COUNT];
						BYTE cbCardCount=(BYTE)m_UserCardControl[wUserIndex].GetCardData(cbCardData,CountArray(cbCardData));

						//�����˿�
						cbCardData[cbCardCount]=m_cbHandCardData[cbCardCount++];
						//m_GameLogic.SortCardList(cbCardData,cbCardCount);

						//�����˿�
						m_UserCardControl[wUserIndex].SetCardData(cbCardData,cbCardCount);

						//��������
						PlayGameSound(AfxGetInstanceHandle(),TEXT("SEND_CARD"));
					}
					else
					{
						//����˿�
						m_UserCardControl[wUserIndex].SetCardData(m_UserCardControl[wUserIndex].GetCardCount()+1);
					}
				}
			}

			//ֹͣ�ж�
			if (m_wDispatchCardIndex>=DISPATCH_COUNT*DISPATCH_CARD_SPACE+DISPATCH_CARD_TIMES)
			{
				//ֹͣ����
				StopDispatchCard();

				//������Ϣ
				SendEngineMessage(IDM_DISPATCH_CARD_FINISH,0L,0L);
			}

			//ˢ�½���
			RefreshGameView();
			return;
		}
	case IDI_JETTON_ACTION:
		{
			bool bKillTimer = true;

			//�����ƶ�
			if(m_bJettonAction && MoveJetton()) bKillTimer = false;

			//ɾ����ʱ��
			if(bKillTimer)
			{
				m_bJettonAction = false;
				KillTimer(IDI_JETTON_ACTION);
			}

			return;
		}
	case IDI_SWAT_CARTOON:
		{
			m_wSwatIndex++;

			if (m_wSwatIndex >= SWAT_CARTOON_X)
			{
				StopSwatCartoon();
			}

			//���½���
			RefreshGameView();
			return;
		}
	case IDI_GUN_FIRE:
		{
			KillTimer(IDI_GUN_FIRE);

			if (m_wCurrentIndex < m_wGroupCount)
			{
				m_ptSrc.SetPoint(m_ptCardControl[m_GunStatus[m_wCurrentIndex].wSrcUser].x-90, m_ptCardControl[m_GunStatus[m_wCurrentIndex].wSrcUser].y-90);
				if (m_bGunCartoon == true) m_bBullet = true;
				SetTimer(IDI_BULLUT_HOLE, 1000, NULL);
			}

			//���½���
			RefreshGameView();
			return;
		}
	case IDI_BULLUT_HOLE:
		{
			KillTimer(IDI_BULLUT_HOLE);

			if (m_wCurrentIndex < m_wGroupCount)
			{
				m_ptSrc.SetPoint(m_ptCardControl[m_GunStatus[m_wCurrentIndex].wSrcUser].x-76, m_ptCardControl[m_GunStatus[m_wCurrentIndex].wSrcUser].y-76);	
				m_bBullet = false;
				m_bGunCartoon = false;			
			}		

			if (m_wCurrentIndex < m_wGroupCount-1)
			{	
				m_wCurrentIndex++;
				ASSERT(m_wCurrentIndex < m_wGroupCount);
				StartGunCartoon(m_GunStatus[m_wCurrentIndex].wSrcUser, m_GunStatus[m_wCurrentIndex].wDestUser);
				return;
			}
					
			//���½���
			ResetGameView();
			return;
		}
	}

	__super::OnTimer(nIDEvent);
}

//���ý���
VOID CGameClientView::ResetGameView()
{
	KillTimer(IDI_REVERSAL_CARD);

	m_wMouseDownItem=INVALID_ITEM;
	m_wHoverCardItem=INVALID_ITEM;
	m_bMoveMouse = false;
	m_bVoicePanelStatus=false;

	//��Ϸ����
	m_wBanker=INVALID_CHAIR ;
	for(WORD i=0 ; i<GAME_PLAYER ; ++i) m_lUserChip[i] = -1;
	for(WORD i=0 ; i<GAME_PLAYER ; ++i) m_lGameScore[i] = -1 ;
	for(WORD i=0 ; i<GAME_PLAYER ; ++i) m_lScoreTimes[i] = MAXLONGLONG ;
	m_bShowCardFrame=false;
	m_enSegmentType=enSegErr ;
	m_wMaxChip=0 ;
	ZeroMemory(m_bCallBankerStatus, sizeof(m_bCallBankerStatus));
	ZeroMemory(m_bGameStatus, sizeof(m_bGameStatus));
	//m_bShowHistory = false;
	ZeroMemory(m_lChipArray, sizeof(m_lChipArray));
	//ZeroMemory(&m_pHistoryScore, sizeof(m_pHistoryScore));
	ZeroMemory(m_bShowSpecType,sizeof(m_bShowSpecType));
	ZeroMemory(m_cbSpecType,sizeof(m_cbSpecType));
	
	//����ʱ��
	ZeroMemory(m_bShowFuZhuClock, sizeof(m_bShowFuZhuClock));
	m_dwFuZhuTime=0;
	
	m_cbTipInfoState=0;

	//��ʾ����
	m_bGlobalClock = false;
	m_dwElapseTime = 0;
	m_bShowTips = false;

	//���붯��
	ZeroMemory(m_JettonStatus,sizeof(m_JettonStatus));
	m_bJettonAction = false;
	ZeroMemory(&m_JettonData, sizeof(m_JettonData));

	//ϴ�ƶ���
	m_bShuffleCard = false;
	m_wShuffleIndex = 0;

	//���ƶ���
	m_bSplitCard = false;
	m_wSplitIndex = 0;

	//���ƶ���
	m_bReversalCard = false;
	m_cbReversalCount=0;
	m_wReversalIndex = 0;
	m_wReversalUser = INVALID_CHAIR;
	m_cbCardType = CT_INVALID;
	m_bShowType = false;

	//���ƶ���
	m_wStartDispatcher=INVALID_CHAIR;
	m_bDispatchCard=false;
	m_wDispatchCardIndex=0L;
	ZeroMemory(m_cbHandCardData,sizeof(m_cbHandCardData));

	//ȫ�ݴ򶯻�
	m_wSwatUser = INVALID_CHAIR;
	m_bSwatCartoon = false;
	m_wSwatIndex = 0;

	//��ǹ����
	FillMemory(&m_GunStatus, sizeof(tagGunStatus)*GAME_PLAYER, INVALID_CHAIR);
	m_wGroupCount = 0;
	m_wCurrentIndex = 0;
	m_bGunCartoon = false;
	m_bBullet = false;
	
	return;
}

//�����ؼ�
VOID CGameClientView::RectifyControl(INT nWidth, INT nHeight)
{
	//�û�����
	m_ptUserFrame[0].SetPoint(nWidth/2-76, 0);
	m_ptUserFrame[1].SetPoint(1, nHeight/2-125);
	m_ptUserFrame[2].SetPoint(nWidth/2-325, nHeight-76);
	m_ptUserFrame[3].SetPoint(nWidth-80, nHeight/2-125);

	//ͷ��λ��	
	m_ptAvatar[0].SetPoint(m_ptUserFrame[0].x + 13, m_ptUserFrame[0].y + 14);
	m_ptAvatar[1].SetPoint(m_ptUserFrame[1].x + 14, m_ptUserFrame[1].y + 17);
	m_ptAvatar[2].SetPoint(m_ptUserFrame[2].x + 13, m_ptUserFrame[2].y + 12);
	m_ptAvatar[3].SetPoint(m_ptUserFrame[3].x + 14, m_ptUserFrame[3].y + 17);

	//�ʺ�λ��
	m_ptNickName[0].SetPoint(m_ptAvatar[0].x+FACE_CX+15, m_ptAvatar[0].y+5);
	m_ptNickName[1].SetPoint(m_ptAvatar[1].x-3, m_ptAvatar[1].y+FACE_CY+17);
	m_ptNickName[2].SetPoint(m_ptAvatar[2].x+FACE_CX+10, m_ptAvatar[2].y+6);
	m_ptNickName[3].SetPoint(m_ptAvatar[3].x-3, m_ptAvatar[3].y+FACE_CY+17);

	//ʱ��λ��
	m_ptClock[0].SetPoint(m_ptAvatar[0].x-100, m_ptAvatar[0].y-6);
	m_ptClock[1].SetPoint(m_ptAvatar[1].x-8, m_ptAvatar[1].y+155);
	m_ptClock[2].SetPoint(nWidth/2+175, nHeight-140);
	m_ptClock[3].SetPoint(m_ptAvatar[3].x-8, m_ptAvatar[3].y+155);

	//ʱ��λ�ã����ģ�
	m_ptClockCenter.SetPoint(nWidth/2-37, nHeight/2-38);
	m_ptClockDir[0].SetPoint(m_ptClockCenter.x+22, m_ptClockCenter.y-46);
	m_ptClockDir[1].SetPoint(m_ptClockCenter.x-28, m_ptClockCenter.y+14);
	m_ptClockDir[2].SetPoint(m_ptClockCenter.x+22, m_ptClockCenter.y+65);
	m_ptClockDir[3].SetPoint(m_ptClockCenter.x+58, m_ptClockCenter.y+12);

	//׼��λ��
	m_ptReady[0].SetPoint(m_ptAvatar[0].x+25, m_ptAvatar[0].y+160);
	m_ptReady[1].SetPoint(m_ptAvatar[1].x+170, m_ptAvatar[1].y+30);
	m_ptReady[2].SetPoint(m_ptAvatar[2].x+280, nHeight-320);
	m_ptReady[3].SetPoint(m_ptAvatar[3].x-200, m_ptAvatar[3].y+30);

	//����λ��
	m_ptGameScore[0].SetPoint(m_ptAvatar[0].x+61, m_ptAvatar[0].y + 265);
	m_ptGameScore[1].SetPoint(m_ptAvatar[1].x+155, m_ptAvatar[1].y+155);
	m_ptGameScore[2].SetPoint(m_ptAvatar[2].x+310, nHeight-260);
	m_ptGameScore[3].SetPoint(m_ptAvatar[3].x-110, m_ptAvatar[3].y+155);

	//�з�λ��
	m_ptCallScore[0].SetPoint(m_ptReady[0].x-5, m_ptReady[0].y-8);
	m_ptCallScore[1].SetPoint(m_ptReady[1].x-5, m_ptReady[1].y-8);
	m_ptCallScore[2].SetPoint(m_ptReady[2].x-5, m_ptReady[2].y-8);
	m_ptCallScore[3].SetPoint(m_ptReady[3].x-5, m_ptReady[3].y-8);

	//����λ��
	m_ptScoreTimes[0].SetPoint(m_ptReady[0].x-69, m_ptReady[0].y-45);
	m_ptScoreTimes[1].SetPoint(m_ptReady[1].x+85, m_ptReady[1].y+5);
	m_ptScoreTimes[2].SetPoint(m_ptReady[2].x-75, m_ptReady[2].y+155);
	m_ptScoreTimes[3].SetPoint(m_ptReady[3].x-12, m_ptReady[3].y+5);

	//����λ��
	m_ptChip[0].SetPoint(m_ptAvatar[0].x-140, m_ptAvatar[0].y+80);
	m_ptChip[1].SetPoint(m_ptAvatar[1].x+75, m_ptAvatar[1].y+220);
	m_ptChip[2].SetPoint(m_ptAvatar[2].x+75, m_ptAvatar[2].y-105);
	m_ptChip[3].SetPoint(m_ptAvatar[3].x-75, m_ptAvatar[3].y+220);

	//ׯ�ұ�־
	m_ptBankerFlag[0].SetPoint(m_ptAvatar[0].x-60, m_ptAvatar[0].y+5);
	m_ptBankerFlag[1].SetPoint(m_ptAvatar[1].x+6, m_ptAvatar[1].y+128);
	m_ptBankerFlag[2].SetPoint(m_ptAvatar[2].x+8, m_ptAvatar[2].y-55);
	m_ptBankerFlag[3].SetPoint(m_ptAvatar[3].x+6, m_ptAvatar[3].y+128);

	//�йܱ�־
	m_ptTrusteeFlag[0].SetPoint(m_ptAvatar[0].x+180, m_ptAvatar[0].y+8);
	m_ptTrusteeFlag[1].SetPoint(m_ptAvatar[1].x+10, m_ptAvatar[1].y-57);
	m_ptTrusteeFlag[2].SetPoint(m_ptAvatar[2].x+52, m_ptAvatar[2].y-50);
	m_ptTrusteeFlag[3].SetPoint(m_ptAvatar[3].x+10, m_ptAvatar[3].y-57);

	//�û��˿�
	m_UserCardControl[0].SetBenchmarkPos(nWidth/2-15, 76, enXCenter, enYTop);
	m_UserCardControl[1].SetBenchmarkPos(92, nHeight/2-50, enXLeft , enYCenter);
	m_UserCardControl[2].SetBenchmarkPos(nWidth/2, nHeight-80, enXCenter , enYBottom);
	m_UserCardControl[3].SetBenchmarkPos(nWidth-92, nHeight/2-50, enXRight, enYCenter);

	//���ķֶ�
	m_CenterCardControl.SetBenchmarkPos(nWidth/2 - 35, nHeight/2 + 20, enXCenter, enYCenter);
	
	m_ptCardControl[0].SetPoint(nWidth/2, 180);
	m_ptCardControl[1].SetPoint(165, nHeight/2-50);
	m_ptCardControl[2].SetPoint(nWidth/2, nHeight-140);
	m_ptCardControl[3].SetPoint(nWidth-165, nHeight/2-50);

	//�����˿�
	m_AllSegCardControl[0].SetBenchmarkPos(m_ptCardControl[0].x, m_ptCardControl[0].y, enXCenter, enYCenter);
	m_AllSegCardControl[1].SetBenchmarkPos(m_ptCardControl[1].x, m_ptCardControl[1].y, enXCenter, enYCenter);
	m_AllSegCardControl[2].SetBenchmarkPos(m_ptCardControl[2].x, m_ptCardControl[2].y, enXCenter, enYCenter);
	m_AllSegCardControl[3].SetBenchmarkPos(m_ptCardControl[3].x, m_ptCardControl[3].y, enXCenter, enYCenter);

	//�ƶ�λ��
	m_HeapCardControl.SetBenchmarkPos(nWidth/2, nHeight/2, enXCenter, enYBottom);

	//��Ϣ��
	if (m_MsgBox.m_hWnd != NULL)
		m_MsgBox.CenterWindow(this);
	
	//�ƶ���ť
	CRect rcButton;
	HDWP hDwp=BeginDeferWindowPos(30);
	const UINT uFlags=SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOCOPYBITS|SWP_NOSIZE;
	
	//�������
	m_ptVoicePanel.SetPoint(nWidth- 202 - 50, nHeight - 110);

	//������ť
	INT nImgWidth = 43;
	//DeferWindowPos(hDwp, m_btYuYin0, NULL, m_ptVoicePanel.x+9+nImgWidth*0, m_ptVoicePanel.y+9, 0, 0, uFlags);
	//DeferWindowPos(hDwp, m_btYuYin1, NULL, m_ptVoicePanel.x+10+nImgWidth*1, m_ptVoicePanel.y+9, 0, 0, uFlags);
	//DeferWindowPos(hDwp, m_btYuYin2, NULL, m_ptVoicePanel.x+11+nImgWidth*2, m_ptVoicePanel.y+9, 0, 0, uFlags);
	//DeferWindowPos(hDwp, m_btYuYin3, NULL, m_ptVoicePanel.x+12+nImgWidth*3, m_ptVoicePanel.y+9, 0, 0, uFlags);

	//�������
	//SetVoicePanelStatus();

	//�ɼ���ť
	m_btLookScore.GetWindowRect(&rcButton);
	DeferWindowPos(hDwp, m_btLookScore, NULL, nWidth-rcButton.Width()-5, 4, 0, 0, uFlags);

	m_btStart.GetWindowRect(&rcButton);
	DeferWindowPos(hDwp, m_btStart, NULL, (nWidth - rcButton.Width())/2 , (nHeight-290)-5, 0, 0, uFlags);

	m_btCallBanker.GetWindowRect(&rcButton);
	DeferWindowPos(hDwp, m_btCallBanker, NULL, nWidth/2 - rcButton.Width() - 10 , (nHeight-260), 0, 0, uFlags);
	DeferWindowPos(hDwp, m_btNoCallBanker, NULL, nWidth/2 + 10 , (nHeight-260), 0, 0, uFlags);

	m_btSetChip[0].GetWindowRect(&rcButton);
	DeferWindowPos(hDwp, m_btSetChip[0], NULL, (nWidth - rcButton.Width() * 4 - 3 * 5 ) / 2, (nHeight-260), 0, 0, uFlags);
	DeferWindowPos(hDwp, m_btSetChip[1], NULL, (nWidth - rcButton.Width() * 4 - 3 * 5 ) / 2 + rcButton.Width() + 5 ,(nHeight-260), 0, 0, uFlags);
	DeferWindowPos(hDwp, m_btSetChip[2], NULL, (nWidth - rcButton.Width() * 4 - 3 * 5 ) / 2 + rcButton.Width() * 2 + 10, (nHeight-260), 0, 0, uFlags);
	DeferWindowPos(hDwp, m_btSetChip[3], NULL, (nWidth - rcButton.Width() * 4 - 3 * 5 ) / 2 + rcButton.Width() * 3 + 15, (nHeight-260), 0, 0, uFlags);

	//��ʾ��ť
	m_btPrompt[0].GetWindowRect(&rcButton);
	CPoint ptStart = CPoint((nWidth-rcButton.Width())/2-125, nHeight-37);
	DeferWindowPos(hDwp, m_btPrompt[0], NULL, ptStart.x+4, ptStart.y, 0, 0, uFlags);
	DeferWindowPos(hDwp, m_btPrompt[1], NULL, ptStart.x + 1*(rcButton.Width() + 6)+1, ptStart.y, 0, 0, uFlags);
	DeferWindowPos(hDwp, m_btPrompt[2], NULL, ptStart.x + 2*(rcButton.Width() + 5), ptStart.y, 0, 0, uFlags);
	DeferWindowPos(hDwp, m_btPrompt[3], NULL, ptStart.x + 3*(rcButton.Width() + 5)-3, ptStart.y, 0, 0, uFlags);
	DeferWindowPos(hDwp, m_btPrompt[4], NULL, ptStart.x + 4*(rcButton.Width() + 4)-1, ptStart.y, 0, 0, uFlags);
	DeferWindowPos(hDwp, m_btPrompt[5], NULL, ptStart.x + 5*(rcButton.Width() + 3)+2, ptStart.y, 0, 0, uFlags);
	DeferWindowPos(hDwp, m_btPrompt[6], NULL, ptStart.x + 6*(rcButton.Width() + 3)+2, ptStart.y, 0, 0, uFlags);
	DeferWindowPos(hDwp, m_btPrompt[7], NULL, ptStart.x + 7*(rcButton.Width() + 3)+2, ptStart.y, 0, 0, uFlags);



	//������ť
	m_btSortCard.GetWindowRect(&rcButton);
	DeferWindowPos(hDwp, m_btSortCard, NULL, nWidth-rcButton.Width()-5, nHeight - 135, 0, 0, uFlags);
	//DeferWindowPos(hDwp, m_btSplitCard, NULL, nWidth-rcButton.Width()-5, nHeight - 135 + 30, 0, 0, uFlags);
	DeferWindowPos(hDwp, m_btTrustee, NULL, nWidth-rcButton.Width()-5, nHeight - 135 + 30 , 0, 0, uFlags);
	//DeferWindowPos(hDwp, m_btVoice, NULL, nWidth-rcButton.Width()-5, nHeight - 135 + 30 * 3, 0, 0, uFlags);


	//���а�ť
	m_btFrontCard.GetWindowRect(&rcButton);
	ptStart.SetPoint((nWidth-rcButton.Width())/2-120, nHeight-75);
	DeferWindowPos(hDwp, m_btFrontCard, NULL, ptStart.x, ptStart.y, 0, 0, uFlags);
	DeferWindowPos(hDwp, m_btMidCard, NULL, ptStart.x + (rcButton.Width() + 20), ptStart.y, 0, 0, uFlags);
	DeferWindowPos(hDwp, m_btBackCard, NULL, ptStart.x + 2*(rcButton.Width() + 20), ptStart.y, 0, 0, uFlags);
	DeferWindowPos(hDwp, m_btShowCard, NULL, ptStart.x + 3*(rcButton.Width() + 20), ptStart.y, 0, 0, uFlags);

	//���հ�ť
	m_btRestore[0].GetWindowRect(&rcButton);
	DeferWindowPos(hDwp, m_btRestore[0], NULL, nWidth/2+60, nHeight/2-65, 0, 0, uFlags);
	DeferWindowPos(hDwp, m_btRestore[1], NULL, nWidth/2+60, nHeight/2-35, 0, 0, uFlags);
	DeferWindowPos(hDwp, m_btRestore[2], NULL, nWidth/2+60, nHeight/2-5, 0, 0, uFlags);
	DeferWindowPos(hDwp, m_btRestore[3], NULL, nWidth/2+60, nHeight/2+60, 0, 0, uFlags);


	

	EndDeferWindowPos(hDwp);

	
	return;
}

//�滭����
VOID CGameClientView::DrawGameView(CDC * pDC, INT nWidth, INT nHeight)
{
	//�滭����
	DrawViewImage(pDC,m_ImageBack,DRAW_MODE_SPREAD);
	DrawViewImage(pDC,m_ImageCenter,DRAW_MODE_CENTENT);

	//�滭�ƿ�
	if (true == m_bShowCardFrame)
		m_PngCardFrame.DrawImage(pDC, (nWidth-m_PngCardFrame.GetWidth())/2-35, (nHeight-m_PngCardFrame.GetHeight())/2 + 20);

	//�����˿�
	m_HeapCardControl.DrawCardControl(pDC);
	m_CenterCardControl.DrawCardControl(pDC);

	for (WORD i = 0; i < GAME_PLAYER; i++)
	{
		if ((GetClientUserItem(i) == NULL) || (m_bGameStatus[i] == false))continue;

		m_UserCardControl[i].DrawCardControl(pDC);
		if (m_enSegmentType == enSegFront) m_AllSegCardControl[i].DrawCardControl(pDC, 0);
		else if (m_enSegmentType == enSegMid)   m_AllSegCardControl[i].DrawCardControl(pDC, 1);
		else m_AllSegCardControl[i].DrawCardControl(pDC);
	}

	if (m_bGunCartoon == true)
	{		
		INT nImgW = m_PngGun.GetWidth()/GAME_PLAYER;
		INT nImgH = m_PngGun.GetHeight()/GAME_PLAYER;

		if (m_bBullet == false)
		{
			m_PngGun.DrawImage(pDC, m_ptSrc.x, m_ptSrc.y, nImgW, nImgH, 0, nImgH*m_GunStatus[m_wCurrentIndex].wSrcUser);
		}
		else
		{			
			m_PngGun.DrawImage(pDC, m_ptSrc.x, m_ptSrc.y, nImgW, nImgH, nImgW*m_GunStatus[m_wCurrentIndex].wDestUser, nImgH*m_GunStatus[m_wCurrentIndex].wSrcUser);
			m_PngGunHole[0].DrawImage(pDC, m_ptDest.x-105, m_ptDest.y-120);
			m_PngGunHole[1].DrawImage(pDC, m_ptDest.x-25, m_ptDest.y-20);
		}
	}
	
	//�滭ȫ��ʱ��
	DrawGlobalClock(pDC);

	//�滭����ʱ��
	DrawFuZhuClock(pDC);
	
	//�滭��ʾ��Ϣ
	if(m_cbTipInfoState>0)
	{
		if(m_cbTipInfoState == 1)	//��ׯ
		{
			m_PngTipCallBanker.DrawImage(pDC, nWidth/2-m_PngTipCallBanker.GetWidth()/2, nHeight/2-50);
		}
		if(m_cbTipInfoState == 2)	//��ע
		{
			m_PngTipCallScore.DrawImage(pDC, nWidth/2-m_PngTipCallScore.GetWidth()/2, nHeight/2-50);
		}
	}

	//�滭�û�
	for (WORD i = 0; i < GAME_PLAYER; i++)
	{			
		//�滭���
		if (0 == i)
			m_PngUserHeadBackH.DrawImage(pDC, m_ptUserFrame[i].x, m_ptUserFrame[i].y);
		else if ((1 == i) || (3 == i))
			m_PngUserHeadBackV.DrawImage(pDC, m_ptUserFrame[i].x, m_ptUserFrame[i].y);
		else
			m_PngUserInfo.DrawImage(pDC, m_ptUserFrame[i].x, m_ptUserFrame[i].y);	
		
		//�û��ӿ�
		IClientUserItem * pIClientUserItem = GetClientUserItem(i);
		WORD wUserTimer=GetUserClock(i);
		if (pIClientUserItem != NULL)
		{
			//�滭ʱ��
			DrawUserCustomClock(pDC, i, wUserTimer);

			//�û�ͷ��
			DrawUserAvatar(pDC, m_ptAvatar[i].x, m_ptAvatar[i].y, pIClientUserItem);

			//׼����ʶ
			if (pIClientUserItem->GetUserStatus() == US_READY)	
				m_PngReadyFlag.DrawImage(pDC, m_ptReady[i].x, m_ptReady[i].y);

			//λ�ö���
			CRect rcNickName(m_ptNickName[i].x, m_ptNickName[i].y, m_ptNickName[i].x+60, m_ptNickName[i].y+15);
			CRect rcUserScore(m_ptNickName[i].x, m_ptNickName[i].y+23, m_ptNickName[i].x+60, m_ptNickName[i].y+37);
			CRect rcUserScoreMySelf(m_ptNickName[i].x, m_ptNickName[i].y+25, m_ptNickName[i].x+60, m_ptNickName[i].y+37);

			//�û��ǳ�
			LPCTSTR pszNickName=pIClientUserItem->GetNickName();

			DrawTextString(pDC, pszNickName,RGB(153,255,0), RGB(0,50,0), rcNickName);

			//�û�����
			TCHAR szUserScore[64]=TEXT("");
			LONGLONG lMeScore = pIClientUserItem->GetUserScore();
			if (m_lUserChip[i] != -1)
				_sntprintf(szUserScore, CountArray(szUserScore), TEXT("%I64d"), lMeScore - m_lUserChip[i]);
			else 
				_sntprintf(szUserScore, CountArray(szUserScore), TEXT("%I64d"), lMeScore);

			if (i != 2) DrawTextString(pDC, szUserScore, RGB(255,255,0), RGB(0,50,0), rcUserScore);
			else DrawTextString(pDC, szUserScore, RGB(255,255,0), RGB(0,50,0), rcUserScoreMySelf);
		}
	}

	//�滭��ʶ
	for (WORD i = 0; i < GAME_PLAYER; i++)
	{
		//ׯ�ұ�ʶ
		if (i == m_wBanker)
			m_PngBankerFlag.DrawImage(pDC, m_ptBankerFlag[i].x, m_ptBankerFlag[i].y);				
		
		//�йܱ�ʶ
		if (true == m_bTrustee[i])
			m_PngTrusteeFlag.DrawImage(pDC, m_ptTrusteeFlag[i].x, m_ptTrusteeFlag[i].y);				
		
		//��ׯ��ʶ
		if (true == m_bCallBankerStatus[i] && m_bGameStatus[i] == true)
			m_PngNoCallBanker.DrawImage(pDC, m_ptCallScore[i].x, m_ptCallScore[i].y);
	}

	//�滭����
	int nChipHeight = m_PngShowUserChip.GetHeight();
	int nChipWidth = m_PngShowUserChip.GetWidth()/14;
	for(WORD i=0 ; i<GAME_PLAYER ; i++)
	{
		if(m_lUserChip[i] > 0)
		{			
			INT nPosX = 0;
			INT nPosY = 0;

			//����δ��ʼ���ѽ���
			if (m_JettonStatus[i].wMoveIndex==0)
			{
				nPosX = m_ptChip[i].x;
				nPosY = m_ptChip[i].y;
			}
			else	//������
			{
				nPosX = m_JettonStatus[i].ptCourse.x;
				nPosY = m_JettonStatus[i].ptCourse.y;
			}

			//�滭����
			INT nChipX = nPosX;
			INT nChipY = nPosY;
			for (INT cbNum=0; cbNum<JETTON_COUNT; cbNum++)
			{
				if (m_JettonData[i][cbNum] > 0)
				{
					for (INT cbIdx=0; cbIdx<m_JettonData[i][cbNum]; cbIdx++)
					{
						m_PngShowUserChip.DrawImage(pDC, nChipX, nChipY-cbIdx*2, nChipWidth, nChipHeight, nChipWidth*cbNum, 0, nChipWidth, nChipHeight);	
					}
					nChipY -= m_JettonData[i][cbNum]*2;
				}
			}
			//�滭����
			DrawNumber(pDC, &m_PngChipNumer, TEXT("0123456789+-"), m_lUserChip[i], nPosX+23, nPosY+33, DT_CENTER);
		}
	}

	//�滭������ʾ
	if (m_bShowTips == true)
	{
		m_PngCardTip.DrawImage(pDC, (nWidth-m_PngCardTip.GetWidth())/2-30, nHeight-300);
	}

	//�滭��������ʾ
	for (WORD i=0; i<GAME_PLAYER; i++)
	{
		if (m_bShowSpecType[i] == true)
		{
			CPoint ptCenter = m_AllSegCardControl[i].GetCenterPoint(2);
			m_PngCardTypeSpec[i].DrawImage(pDC, ptCenter.x-m_PngCardTypeSpec[i].GetWidth()/2, ptCenter.y-m_PngCardTypeSpec[i].GetHeight()/2);
		}
	}

	//�滭ϴ��
	if (m_bShuffleCard == true)
	{
		//��ȡ��С
		CSize SizeDispatchCard;
		SizeDispatchCard.SetSize(m_PngShuffleCard.GetWidth()/SHUFFLE_X_COUNT,m_PngShuffleCard.GetHeight()/SHUFFLE_Y_COUNT);
	
		//�滭Ч��
		m_PngShuffleCard.DrawImage(pDC, (nWidth-SizeDispatchCard.cx)/2, nHeight/2-120, SizeDispatchCard.cx, SizeDispatchCard.cy,
			SizeDispatchCard.cx*(m_wShuffleIndex%SHUFFLE_X_COUNT), SizeDispatchCard.cy*(m_wShuffleIndex/SHUFFLE_X_COUNT));
	}

	//�滭����
	if (m_bSplitCard == true)
	{
		//��ȡ��С
		CSize SizeSplitCard;
		SizeSplitCard.SetSize(m_PngSplitCard.GetWidth()/SPLIT_X_COUNT,m_PngSplitCard.GetHeight());

		//�滭Ч��
		m_PngSplitCard.DrawImage(pDC, (nWidth-SizeSplitCard.cx)/2, nHeight/2-120, SizeSplitCard.cx, SizeSplitCard.cy, SizeSplitCard.cx*(m_wSplitIndex%SPLIT_X_COUNT), 0);
	}

	//���ƶ���
	if (m_bReversalCard == true)
	{
		//��ȡ��С
		CSize SizeReversalCard;
		SizeReversalCard.SetSize(m_PngReversalCard.GetWidth()/REVERSAL_X_COUNT,m_PngReversalCard.GetHeight());

		//�滭Ч��
		for (BYTE i=0;i<m_cbReversalCount;i++)
		{
			m_PngReversalCard.DrawImage(pDC,m_ptReversal.x+i*18,m_ptReversal.y,SizeReversalCard.cx,SizeReversalCard.cy,SizeReversalCard.cx*(m_wReversalIndex%REVERSAL_X_COUNT),0);
		}
	}

	//���ƶ���
	if (m_bDispatchCard == true)
	{
		//��������
		CSize SizeControl[GAME_PLAYER];
		CPoint PointControl[GAME_PLAYER];

		//��ȡ��С
		CSize SizeCardItem;
		m_UserCardControl[ME_VIEW_CHAIR].GetCardSize(SizeCardItem);

		//
		CSize SizeCardMoving;
		SizeCardMoving.cx = m_PngMovingCard.GetWidth()/4;
		SizeCardMoving.cy = m_PngMovingCard.GetHeight();
		
		//��ȡ����
		CPngImage * pImageCard=NULL;
		m_UserCardControl[ME_VIEW_CHAIR].GetCardImage(&pImageCard);

		//����λ��
		INT nYHeapPos=nHeight/2-70;
		INT nXHeapPos=(nWidth-SizeCardItem.cx-(DISPATCH_COUNT-1)*DISPATCH_HEAP_DISTANCE)/2;

		//��ȡλ��
		for (WORD i=0;i<GAME_PLAYER;i++)
		{
			m_UserCardControl[i].GetControlSize(SizeControl[i]);
			m_UserCardControl[i].GetOriginPoint(PointControl[i]);
		}

		//�滭�ƶ�
		for (BYTE i=0;i<DISPATCH_COUNT;i++)
		{
			if ((DISPATCH_COUNT-i)*DISPATCH_CARD_SPACE>m_wDispatchCardIndex)
			{
				INT nXDrawPos=nXHeapPos+i*DISPATCH_HEAP_DISTANCE;
				pImageCard->DrawImage(pDC,nXDrawPos,nHeight/2-70,SizeCardItem.cx,SizeCardItem.cy,SizeCardItem.cx*2,SizeCardItem.cy*4);
			}
		}

		for (BYTE i=0;i<DISPATCH_COUNT;i++)
		{
			if ((m_wDispatchCardIndex>=(i+1)*DISPATCH_CARD_SPACE)&&(m_wDispatchCardIndex<(i+1)*DISPATCH_CARD_SPACE+DISPATCH_CARD_TIMES))
			{
				//��������
				WORD wSendUser=(m_wStartDispatcher+i)%GAME_PLAYER;
				if ((GetClientUserItem(wSendUser)==NULL) || (m_bGameStatus[wSendUser] == false)) continue;

				WORD wSendIndex=m_wDispatchCardIndex-(i+1)*DISPATCH_CARD_SPACE;

				//Ŀ��λ��
				INT nXTargetPos=PointControl[wSendUser].x+SizeControl[wSendUser].cx/2;
				INT nYTargetPos=PointControl[wSendUser].y+SizeControl[wSendUser].cy/2;

				//���λ��
				INT nYSourcePos=nYHeapPos+SizeCardItem.cy/2;
				INT nXSourcePos=nXHeapPos+(DISPATCH_COUNT-i-1)*DISPATCH_HEAP_DISTANCE+SizeCardItem.cx/2;

				//�滭�˿�
				INT nXDrawPos=nXSourcePos+wSendIndex*(nXTargetPos-nXSourcePos)/DISPATCH_CARD_TIMES-SizeCardMoving.cx/2;
				INT nYDrawPos=nYSourcePos+wSendIndex*(nYTargetPos-nYSourcePos)/DISPATCH_CARD_TIMES-SizeCardMoving.cy/2;
				m_PngMovingCard.DrawImage(pDC,nXDrawPos,nYDrawPos,SizeCardMoving.cx,SizeCardMoving.cy,SizeCardMoving.cx*(m_wDispatchCardIndex%4),0);
			}
		}
	}

	//ȫ�ݴ򶯻�
	if (m_bSwatCartoon == true)
	{
		INT nImgWidth = m_PngSwatCartoon.GetWidth()/SWAT_CARTOON_X;
		INT nImgHeight = m_PngSwatCartoon.GetHeight();
		m_PngSwatCartoon.DrawImage(pDC, m_ptSwat.x, m_ptSwat.y, nImgWidth, nImgHeight, nImgWidth*(m_wSwatIndex%SWAT_CARTOON_X), 0, nImgWidth, nImgHeight);
	}

	//�滭����
	for (WORD i = 0; i < GAME_PLAYER; i++)
	{
		if ((GetClientUserItem(i) == NULL) || (m_bGameStatus[i] == false)) continue;
		
		//�������
		if (m_lGameScore[i] != -1 )
		{
			DrawSignNumber(pDC, &m_PngResultNumber, TEXT("0123456789+-"), m_lGameScore[i], m_ptGameScore[i].x, m_ptGameScore[i].y, DT_CENTER);
		}

		//��������
		if (m_lScoreTimes[i] != MAXLONGLONG)
		{
			DrawSignNumber(pDC, &m_PngResultNumber, TEXT("0123456789+-"), m_lScoreTimes[i], m_ptScoreTimes[i].x, m_ptScoreTimes[i].y, DT_CENTER);
		}
	}

	//��ʾ��������		
	if ((m_enSegmentType != enSegErr) && (m_bShowType == true))
	{
		CPoint ptCenter = m_AllSegCardControl[m_wReversalUser].GetCenterPoint(m_enSegmentType);
		m_PngCardType.DrawImage(pDC, ptCenter.x-m_PngCardType.GetWidth()/2, ptCenter.y-m_PngCardType.GetHeight()/2);
	}

	//��ʷ�ɼ�
	if (m_bShowHistory == true)
	{
		//���ֱ���
		m_PngHistoryScore.DrawImage(pDC, nWidth-300, 5);

		//����λ��
		INT nXBenchmark = nWidth-300;
		INT nYBenchmark = 10;

		//�滭��Ϣ
		for (WORD i=0;i<GAME_PLAYER;i++)
		{			
			//��ȡ�û�
			IClientUserItem * pIClientUserItem = GetClientUserItem(i);
			if ((pIClientUserItem == NULL) || m_pHistoryScore[i] == NULL) continue;

			//λ�ü���
			CRect rcAccounts(nXBenchmark+60+i*51,nYBenchmark+4,nXBenchmark+110+i*51,nYBenchmark+16);
			CRect rcTurnScore(nXBenchmark+60+i*51,nYBenchmark+23,nXBenchmark+110+i*51,nYBenchmark+35);
			CRect rcCollectScore(nXBenchmark+60+i*51,nYBenchmark+42,nXBenchmark+110+i*51,nYBenchmark+54);

			//������Ϣ
			TCHAR szTurnScore[16]=TEXT("");
			TCHAR szCollectScore[16]=TEXT("");
			_sntprintf(szTurnScore,CountArray(szTurnScore),SCORE_STRING, m_pHistoryScore[i]->lTurnScore);
			_sntprintf(szCollectScore,CountArray(szCollectScore),SCORE_STRING,m_pHistoryScore[i]->lCollectScore);

			//�滭��Ϣ
			COLORREF crName = RGB(255, 255, 255);
			COLORREF crFrame = RGB(0, 0, 0);
			COLORREF crText1 = ((m_pHistoryScore[i]->lTurnScore>=0)?RGB(255, 152, 3):RGB(0, 246, 255));
			COLORREF crText2 = ((m_pHistoryScore[i]->lCollectScore>=0)?RGB(255, 152, 3):RGB(0, 246, 255));
			LPCTSTR pszNickName=pIClientUserItem->GetNickName();
			DrawTextString(pDC, pszNickName, crName, crFrame, &rcAccounts);
			DrawTextString(pDC, szTurnScore, crText1, crFrame, &rcTurnScore);
			DrawTextString(pDC, szCollectScore, crText2, crFrame, &rcCollectScore);
		}
	}

	//test
	//DrawSignNumber(pDC, &m_PngResultNumber, TEXT("0123456789+-"), m_ptScoreTimes[0].x, 50/*m_ptGameScore[i].x*/, 0/*m_ptGameScore[i].y*/, DT_LEFT);
	//DrawSignNumber(pDC, &m_PngResultNumber, TEXT("0123456789+-"), m_ptScoreTimes[0].y, 200/*m_ptGameScore[i].x*/, 0/*m_ptGameScore[i].y*/, DT_LEFT);

	//DrawSignNumber(pDC, &m_PngResultNumber, TEXT("0123456789+-"), m_ptScoreTimes[2].x, 50/*m_ptGameScore[i].x*/, 100/*m_ptGameScore[i].y*/, DT_LEFT);
	//DrawSignNumber(pDC, &m_PngResultNumber, TEXT("0123456789+-"), m_ptScoreTimes[2].y, 200/*m_ptGameScore[i].x*/, 100/*m_ptGameScore[i].y*/, DT_LEFT);

	////
	//DrawSignNumber(pDC, &m_PngResultNumber, TEXT("0123456789+-"), 20000,m_ptScoreTimes[0].x/*m_ptGameScore[i].x*/, m_ptScoreTimes[0].y/*m_ptGameScore[i].y*/, DT_CENTER);
	//DrawSignNumber(pDC, &m_PngResultNumber, TEXT("0123456789+-"), -20000, m_ptScoreTimes[2].x/*m_ptGameScore[i].x*/, m_ptScoreTimes[2].y/*m_ptGameScore[i].y*/, DT_CENTER);
	// 
	
	//for (int i = 0; i < 4; i++)
	//{
	//	//λ�ö���
	//	CRect rcNickName(m_ptNickName[i].x, m_ptNickName[i].y, m_ptNickName[i].x+60, m_ptNickName[i].y+15);
	//	CRect rcUserScore(m_ptNickName[i].x, m_ptNickName[i].y+23, m_ptNickName[i].x+60, m_ptNickName[i].y+37);
	//	CRect rcUserScoreMySelf(m_ptNickName[i].x, m_ptNickName[i].y+25, m_ptNickName[i].x+60, m_ptNickName[i].y+37);
	//

	//	//�û��ǳ�
	//	//LPCTSTR pszNickName=pIClientUserItem->GetNickName();
	//	DrawTextString(pDC, TEXT("1sdfdsfsadfadsfsadfasdf23456"),RGB(153,255,0), RGB(0,50,0), rcNickName);
	//
	//	//�û�����
	//	TCHAR szUserScore[64]=TEXT("");
	//	m_lUserChip[i] = 113213123;
	//	//LONGLONG lMeScore = pIClientUserItem->GetUserScore();
	//	if (m_lUserChip[i] != -1)
	//		_sntprintf(szUserScore, CountArray(szUserScore), TEXT("%I64d"), (LONGLONG)21000120000);
	//	else 
	//		_sntprintf(szUserScore, CountArray(szUserScore), TEXT("%I64d"), (LONGLONG)12412123120);
	//
	//	if (i != 2) DrawTextString(pDC, szUserScore, RGB(255,255,0), RGB(0,50,0), rcUserScore);
	//	else DrawTextString(pDC, szUserScore, RGB(255,255,0), RGB(0,50,0), rcUserScoreMySelf);
	//}


	return;
}

//�����˿�
void CGameClientView::SetCardData(WORD wChairID , BYTE bCardData[] , BYTE bCardCount , enCardControlType bCardType , BYTE bSeg) 
{
	switch(bCardType) 
	{
	case enCCTHandCard:			//�����˿�
		if(INVALID_CHAIR==wChairID)
		{
			for(WORD i=0 ; i<GAME_PLAYER ; ++i)	m_UserCardControl[i].SetCardData(NULL , 0) ;
			return ;
		}
		
		m_UserCardControl[wChairID].SetCardData(bCardData , bCardCount) ;

		break;	
    case enCCTAllSegmentCard:	//���зֶ�
		if(INVALID_CHAIR==wChairID)
		{
			for(WORD i=0 ; i<GAME_PLAYER ; i++) m_AllSegCardControl[i].SetCardData(NULL, 0);
			return ;
		}
		m_AllSegCardControl[wChairID].SetCardData(bCardData , bCardCount);

		break;
	default:
		AfxMessageBox(_T("�����˿����ͣ�")) ;
		break;
	}
}

//����ׯ��
void CGameClientView::SetBanker(WORD wBanker)
{
	m_wBanker = wBanker ;
	
	//���½���
	RefreshGameView();
	return;
}

//�����й�
void CGameClientView::SetTrustee(WORD wChairID, bool bTrustee)
{
	if (INVALID_CHAIR == wChairID)
	{
		for (WORD i = 0; i < GAME_PLAYER; i++) m_bTrustee[i] = false;

		//���½���
		RefreshGameView();
		return;
	}

	if (m_bTrustee[wChairID] != bTrustee)
		m_bTrustee[wChairID] = bTrustee;

	//���½���
	RefreshGameView();
	return;
}

//����ѹע
void CGameClientView::SetChip(WORD wChairID, SCORE lChip, bool bMove) 
{
	if(INVALID_CHAIR==wChairID)
	{
		for(WORD i=0 ; i<GAME_PLAYER ; i++) m_lUserChip[i] = lChip;

		//�������״̬
		ZeroMemory(&m_JettonStatus, sizeof(m_JettonStatus));
		ZeroMemory(&m_JettonData, sizeof(m_JettonData));
		//���½���
		RefreshGameView();
		return ;
	}

	//��֤�û�
	ASSERT((wChairID>=0) && (wChairID<GAME_PLAYER));

	//��������
	if (lChip>0) AnalyseChip(wChairID, lChip);
	if(bMove) DrawMoveAnte(wChairID,lChip);
	else m_lUserChip[wChairID] = lChip;
	
	//���½���
	RefreshGameView();
	return;
}

//���÷���
void CGameClientView::SetGameScore(WORD wChairID, SCORE lScore)
{
	if(INVALID_CHAIR==wChairID)
	{
		for(WORD i=0 ; i<GAME_PLAYER ; i++) m_lGameScore[i] = lScore ;

		//���½���
		RefreshGameView();
		return ;
	}

	m_lGameScore[wChairID] = lScore ;

	//���½���
	RefreshGameView();
	return;
}

//���ñ���
void CGameClientView::SetScoreTimes(WORD wChairID, SCORE lScoreTimes)
{
	if(INVALID_CHAIR==wChairID)
	{
		for(WORD i=0 ; i<GAME_PLAYER ; i++) m_lScoreTimes[i] = lScoreTimes ;

		//���½���
		RefreshGameView();
		return ;
	}

	m_lScoreTimes[wChairID] = lScoreTimes ;

	//���½���
	RefreshGameView();
	return;
}

//���ñȽϳ���
void CGameClientView::SetCompareScene(enSegmentType SegmentType)
{
	if (m_enSegmentType == SegmentType) return;

	//���ñ���
	m_enSegmentType = SegmentType;

	//���½���
	RefreshGameView();
	return;
}

//��ʾ�ƿ�
void CGameClientView::ShowCenterControl(bool bShow)
{
	if (m_bShowCardFrame == bShow) return;

	m_bShowCardFrame = bShow;
	if (true == m_bShowCardFrame)
	{
		m_CenterCardControl.ShowWindow(bShow);
		m_CenterCardControl.SetDisplayFlag(true);
	}
	else
	{
		m_CenterCardControl.ShowWindow(bShow);
		m_CenterCardControl.SetDisplayFlag(false);
	}

	//���½���
	RefreshGameView();
	return;
}

//��ʾ����
void CGameClientView::SetCardType(BYTE cbCardType)
{
	//������Դ
	if (m_PngCardType.IsNull() == false) m_PngCardType.DestroyImage();

	//������Դ
	HINSTANCE hResInstance = AfxGetInstanceHandle();
	switch(cbCardType)
	{
	case CT_ONE_DOUBLE:
		{
			m_PngCardType.LoadImage(hResInstance, TEXT("PNG_ONE_DOUBLE"));
			break;
		}
	case CT_TWO_DOUBLE:
		{
			m_PngCardType.LoadImage(hResInstance, TEXT("PNG_TWO_DOUBLE"));
			break;
		}
	case CT_THREE:
		{
			m_PngCardType.LoadImage(hResInstance, TEXT("PNG_THREE"));
			break;
		}
	case CT_FIVE_MIXED_FLUSH_FIRST_A:
	case CT_FIVE_MIXED_FLUSH_NO_A:
	case CT_FIVE_MIXED_FLUSH_BACK_A:
		{
			m_PngCardType.LoadImage(hResInstance, TEXT("PNG_LINE"));
			break;
		}
	case CT_FIVE_FLUSH:
		{
			m_PngCardType.LoadImage(hResInstance, TEXT("PNG_TONGHUA"));
			break;
		}
	case CT_FIVE_THREE_DEOUBLE:
		{
			m_PngCardType.LoadImage(hResInstance, TEXT("PNG_HULU"));
			break;
		}
	case CT_FIVE_FOUR_ONE:
		{
			m_PngCardType.LoadImage(hResInstance, TEXT("PNG_TIEZHI"));
			break;
		}
	case CT_FIVE_STRAIGHT_FLUSH_FIRST_A:
	case CT_FIVE_STRAIGHT_FLUSH:
		{
			m_PngCardType.LoadImage(hResInstance, TEXT("PNG_TONGHUASHUN"));
			break;
		}
	default:
		{
			m_PngCardType.LoadImage(hResInstance, TEXT("PNG_SINGLE"));
			break;
		}
	}

	//ˢ�½���
	RefreshGameView();
	return;
}

//��ʾ����
void CGameClientView::SetCardTypeSpec(bool bShow, WORD wChairID, BYTE cbCardType)
{
	if (bShow == false)
	{
		ZeroMemory(m_bShowSpecType,sizeof(m_bShowSpecType));
		ZeroMemory(m_cbSpecType,sizeof(m_cbSpecType));
		return;
	}

	m_bShowSpecType[wChairID] = true;
	m_cbSpecType[wChairID] = cbCardType; 

	//������Դ
	if (m_PngCardTypeSpec[wChairID].IsNull() == false) m_PngCardTypeSpec[wChairID].DestroyImage();

	//������Դ
	HINSTANCE hResInstance = AfxGetInstanceHandle();
	switch(cbCardType)
	{
	case CT_EX_ZHIZUNQINGLONG:
		{
			m_PngCardTypeSpec[wChairID].LoadImage(hResInstance, TEXT("PNG_ZHIZONGQINGLONG"));
			break;
		}
	case CT_EX_YITIAOLONG:
		{
			m_PngCardTypeSpec[wChairID].LoadImage(hResInstance, TEXT("PNG_YITIAOLONG"));
			break;
		}
	case CT_EX_LIUDUIBAN:
		{
			m_PngCardTypeSpec[wChairID].LoadImage(hResInstance, TEXT("PNG_LIUDUIBAN"));
			break;
		}
	case CT_EX_SANSHUNZI:
		{
			m_PngCardTypeSpec[wChairID].LoadImage(hResInstance, TEXT("PNG_SANSHUNZI"));
			break;
		}
	case CT_EX_SANTONGHUA:
		{
			m_PngCardTypeSpec[wChairID].LoadImage(hResInstance, TEXT("PNG_SANTONGHUA"));
			break;
		}
	}

	//ˢ�½���
	RefreshGameView();
	return;
}

//�����ע
void CGameClientView::SetMaxChip(LONGLONG wMaxChip)
{
	m_wMaxChip = wMaxChip ;
}

//��ׯ��Ϣ
void CGameClientView::SetCallBankerStatus(WORD wChairID, bool bCallBankerStatus)
{
	if (INVALID_CHAIR==wChairID)
	{	
		for ( WORD wChairIndx = 0; wChairIndx < GAME_PLAYER; ++wChairIndx )
			m_bCallBankerStatus[wChairIndx] = bCallBankerStatus;
	}
	else
		m_bCallBankerStatus[wChairID] = bCallBankerStatus ;

	//���½���
	RefreshGameView();
}

//������ע
void CGameClientView::SetChipArray(SCORE lChipsArray[SET_CHIP_COUNT])
{
	CopyMemory(m_lChipArray, lChipsArray, sizeof(m_lChipArray)) ;

	CFont font;
	VERIFY(font.CreateFont(
		80,                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_NORMAL,                 // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		_T("Arial")));             // lpszFacename

	CDC *pDC = GetDC();
	CFont* def_font = pDC->SelectObject(&font);
	pDC->SetTextAlign(TA_CENTER);
	
	for (int i = 0; i < 4; i++)
	{
		TCHAR szChip[32] = TEXT("");
		_sntprintf(szChip, CountArray(szChip), SCORE_STRING, m_lChipArray[i]);
		GetDlgItem(IDC_SET_CHIP_0+i)->SetFont(&font);
		m_btSetChip[i].SetWindowText(szChip);
	}
	pDC->SelectObject(def_font);
	font.DeleteObject();
}

//���û���
void CGameClientView::SetHistoryScore(WORD wChairID, tagHistoryScore * pHistoryScore)
{
	//Ч�����
	ASSERT((wChairID >= 0) && (wChairID < GAME_PLAYER));
	if ((wChairID < 0) || (wChairID >= GAME_PLAYER)) return;

	//���ñ���
	m_pHistoryScore[wChairID] = pHistoryScore;

	//���½���
	RefreshGameView();

	return;
}

//������ʾ
void CGameClientView::SetShowCardTip(bool bShow)
{
	if (m_bShowTips == bShow) return;

	m_bShowTips = bShow;

	//���½���
	RefreshGameView();
	return;
}
//////////////////////////////////////////////////////////////////////////


//������ͼ
VOID CGameClientView::RefreshGameView()
{
	CRect rect;
	GetClientRect(&rect);
	InvalidGameView(rect.left,rect.top,rect.Width(),rect.Height());

	return;
}

//ϴ�ƶ���
VOID CGameClientView::StartShuffleCard()
{
	m_bShuffleCard = true;
	m_wShuffleIndex = 0;
	SetTimer(IDI_SHUFFLE_CARD, 100, NULL);

	//��������
	PlayGameSound(AfxGetInstanceHandle(), TEXT("SHUFFLE_CARD"));

	//���½���
	RefreshGameView();
	return;
}

//���ƶ���
VOID CGameClientView::StartSplitCard()
{
	m_bSplitCard = true;
	m_wSplitIndex = 0;
	SetTimer(IDI_SPLIT_CARD, 100, NULL);

	//���½���
	RefreshGameView();
	return;
}

//���ƶ���
VOID CGameClientView::ReversalCard(WORD wStartUserID, BYTE cbCardCount)
{	
	m_wReversalUser = wStartUserID;
	m_cbReversalCount = cbCardCount;
	m_wReversalIndex = 0;
	m_bShowType = false;
	SetTimer(IDI_REVERSAL_CARD, 80, NULL);

	//���½���
	RefreshGameView();
	return;
}

//ȫ�ݴ򶯻�
VOID CGameClientView::StartSwatCartoon(WORD wChairID)
{
	ASSERT(wChairID>=0 && wChairID<GAME_PLAYER);

	m_wSwatIndex = 0;
	m_wSwatUser = wChairID;
	m_ptSwat.x = m_ptCardControl[wChairID].x-75;
	m_ptSwat.y = m_ptCardControl[wChairID].y-30;
	m_bSwatCartoon = true;
	SetTimer(IDI_SWAT_CARTOON, 200, NULL);

    //���½���
	RefreshGameView();
	return;
}

//ֹͣ����
VOID CGameClientView::StopSwatCartoon()
{
	m_wSwatIndex = 0;
	m_bSwatCartoon = false;
	KillTimer(IDI_SWAT_CARTOON);

	//���½���
	RefreshGameView();
	return;
}

//���ö���
VOID CGameClientView::SetGunCartoonGroup(tagGunStatus GunStatus[], WORD wGroupCount)
{
	ASSERT(GunStatus !=NULL);
	if ((GunStatus == NULL)) return;

	CopyMemory(m_GunStatus, GunStatus, sizeof(tagGunStatus)*wGroupCount);
	m_wGroupCount = wGroupCount;

	return;
}

//��ǹ����
VOID CGameClientView::StartGunCartoon(WORD wSrcUser, WORD wDestUser)
{
	//ֹͣ����
	StopGunCartoon();
	
	if (m_wCurrentIndex < m_wGroupCount)
	{
		m_ptSrc.SetPoint(m_ptCardControl[wSrcUser].x-76, m_ptCardControl[wSrcUser].y-76);
		m_ptDest.SetPoint(m_ptCardControl[wDestUser].x, m_ptCardControl[wDestUser].y);
		m_bGunCartoon =true;
		SetTimer(IDI_GUN_FIRE, 100, NULL);
	}

	//���½���
	RefreshGameView();
	PlayGameSound(AfxGetInstanceHandle(), TEXT("GUN_FIRE"));

	return;
}

//��ǹ����
VOID CGameClientView::StopGunCartoon()
{
	m_bGunCartoon = false;
	m_bBullet = false;
	KillTimer(IDI_BULLUT_HOLE);
	KillTimer(IDI_GUN_FIRE);

	//���½���
	RefreshGameView();
	return;
}

//���ƶ���
VOID CGameClientView::StartDispatchCard(BYTE cbCardData[HAND_CARD_COUNT], WORD wStartUser)
{
	//���ñ���
	m_bDispatchCard=true;
	m_wDispatchCardIndex=0L;

	//���ñ���
	m_wStartDispatcher=wStartUser;
	CopyMemory(m_cbHandCardData,cbCardData,sizeof(BYTE)*HAND_CARD_COUNT);

	//�����˿�
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		m_UserCardControl[i].SetCardData(0);
	}

	//���ö�ʱ��
	SetTimer(IDI_DISPATCH_CARD, 50, NULL);

	//���½���
	RefreshGameView();

	return;
}

//ֹͣ����
VOID CGameClientView::StopDispatchCard()
{
	//ֹͣ����
	KillTimer(IDI_DISPATCH_CARD);
	if (m_bDispatchCard==true)
	{
		//���ñ���
		m_bDispatchCard=false;
		m_wDispatchCardIndex=0L;
		
		//�����˿�
		for (WORD i=0;i<GAME_PLAYER;i++)
		{
			if ((GetClientUserItem(i) == NULL) && (m_bGameStatus[i] == false)) continue;

			if (i==ME_VIEW_CHAIR)
				m_UserCardControl[i].SetCardData(m_cbHandCardData,CountArray(m_cbHandCardData));
			else
				m_UserCardControl[i].SetCardData(HAND_CARD_COUNT);
		}

		//���½���
		RefreshGameView();
	}

	return;
}

//ֹͣϴ��
VOID CGameClientView::StopShuffleCard()
{
	m_bShuffleCard = false;
	m_wShuffleIndex = 0;
	KillTimer(IDI_SHUFFLE_CARD);

	//���½���
	RefreshGameView();
	return;
}

//ֹͣ����
VOID CGameClientView::StopSplitCard()
{
	m_bSplitCard = false;
	m_wSplitIndex = 0;
	KillTimer(IDI_SPLIT_CARD);

	//���½���
	RefreshGameView();
	return;
}

// �滭����
VOID CGameClientView::DrawNumber(CDC * pDC, CPngImage* ImageNumber, TCHAR * szImageNum, SCORE lOutNum,INT nXPos, INT nYPos, UINT uFormat /*= DT_LEFT*/ )
{
	ASSERT((pDC != NULL) && (ImageNumber != NULL));
	
	TCHAR szOutNum[128] = {0};
	_sntprintf(szOutNum,CountArray(szOutNum),SCORE_STRING,lOutNum);

	// �������
	INT nNumberHeight=ImageNumber->GetHeight();
	INT nNumberWidth=ImageNumber->GetWidth()/lstrlen(szImageNum);

	if ( uFormat == DT_CENTER )
	{
		nXPos -= (INT)(((DOUBLE)(lstrlen(szOutNum)) / 2.0) * nNumberWidth);
	}
	else if ( uFormat == DT_RIGHT )
	{
		nXPos -= lstrlen(szOutNum) * nNumberWidth;
	}

	for ( INT i = 0; i < lstrlen(szOutNum); ++i )
	{
		for ( INT j = 0; j < lstrlen(szImageNum); ++j )
		{
			if ( szOutNum[i] == szImageNum[j] && szOutNum[i] != '\0' )
			{
				ImageNumber->DrawImage(pDC, nXPos, nYPos, nNumberWidth, nNumberHeight, j * nNumberWidth, 0, nNumberWidth, nNumberHeight);
				nXPos += nNumberWidth;
				break;
			}
		}
	}
}

//��������
VOID CGameClientView::DrawSignNumber(CDC * pDC, CPngImage* ImageNumber ,TCHAR * szImageNum, SCORE lOutNum, INT nXPos, INT nYPos, UINT uFormat/* = DT_LEFT*/)
{
	ASSERT((pDC != NULL) && (ImageNumber != NULL) && (szImageNum != NULL));
	
	UINT nSpace = -3;
	
	LONGLONG  tmpNum = labs((LONG)lOutNum);
	TCHAR szOutNum[32] = {0};
	_sntprintf(szOutNum,CountArray(szOutNum), SCORE_STRING, tmpNum);

	// �������
	int nLengthOutNum = lstrlen(szOutNum);
	int nLengthImgaeNum = lstrlen(szImageNum);
	INT nNumberHeight=ImageNumber->GetHeight();
	INT nNumberWidth=ImageNumber->GetWidth()/nLengthImgaeNum;


	if ( uFormat == DT_CENTER )
	{
		//��λ���
		if (nLengthOutNum%2) nXPos -= (INT)(((nLengthOutNum+1) / 2.0 -1) * (nNumberWidth+nSpace));
		else nXPos -= (INT)(((nLengthOutNum) / 2.0) * (nNumberWidth+nSpace));

		//if (lOutNum < 0) nXPos -= (nNumberWidth+nSpace);
	}
	else if ( uFormat == DT_RIGHT )
	{
		nXPos -= lstrlen(szOutNum) * (nNumberWidth+nSpace);
	}

	//��������
	for ( INT i = 0; i < nLengthOutNum; i++ )
	{
		for ( INT j = 0; j < nLengthImgaeNum; j++ )
		{
			if (szOutNum[i] == szImageNum[j] && szOutNum[i] != '\0')
			{
				ImageNumber->DrawImage(pDC, nXPos, nYPos, nNumberWidth, nNumberHeight, j * nNumberWidth, 0, nNumberWidth, nNumberHeight);
				nXPos += (nNumberWidth + nSpace);
				break;
			}
		}
	}

	//���Ʒ���
	if (lOutNum > 0) 
		ImageNumber->DrawImage(pDC, nXPos-(nLengthOutNum+1)*(nNumberWidth+nSpace), nYPos, nNumberWidth, nNumberHeight, 10*nNumberWidth, 0, nNumberWidth, nNumberHeight);
	else if (lOutNum < 0)
		ImageNumber->DrawImage(pDC, nXPos-(nLengthOutNum+1)*(nNumberWidth+nSpace), nYPos, nNumberWidth, nNumberHeight, 11*nNumberWidth, 0, nNumberWidth, nNumberHeight);
	
	return;
}

//�滭ʱ��
VOID CGameClientView::DrawUserCustomClock(CDC * pDC, WORD wChairID, WORD wElapseTime)
{
	if (wElapseTime > 0)
	{
		if (GetClientUserItem(wChairID) != NULL && GetClientUserItem(wChairID)->GetUserStatus() != US_PLAYING)
		{		
			//����ʱ��
			m_PngTimerFlag.DrawImage(pDC, m_ptClockCenter.x, m_ptClockCenter.y);
			DrawNumber(pDC, &m_PngTimeNumber, TEXT("0123456789"), wElapseTime, m_ptClockCenter.x+36, m_ptClockCenter.y+18, DT_CENTER);

			//���Ƽ�ͷ
			INT nImgWidth = m_PngDirectionOp.GetWidth()/4;
			INT nImgHeight = m_PngDirectionOp.GetHeight();
			m_PngDirectionOp.DrawImage(pDC, m_ptClockDir[wChairID].x, m_ptClockDir[wChairID].y, nImgWidth, nImgHeight, wChairID*nImgWidth, 0, nImgWidth, nImgHeight);
		}
		else
		{
			//����ʱ��
			m_PngTimerFlag.DrawImage(pDC, m_ptClock[wChairID].x, m_ptClock[wChairID].y);
			DrawNumber(pDC, &m_PngTimeNumber, TEXT("0123456789"), wElapseTime, m_ptClock[wChairID].x+36, m_ptClock[wChairID].y+18, DT_CENTER);
		}
	}

	return;
}

VOID CGameClientView::DrawGlobalClock( CDC* pDC)
{
	if (m_bGlobalClock && m_dwElapseTime > 0)
	{
		//����ʱ��
		m_PngTimerFlag.DrawImage(pDC, m_ptClock[ME_VIEW_CHAIR].x, m_ptClock[ME_VIEW_CHAIR].y);
		DrawNumber(pDC, &m_PngTimeNumber, TEXT("0123456789"), (LONGLONG)m_dwElapseTime, m_ptClock[ME_VIEW_CHAIR].x+36, m_ptClock[ME_VIEW_CHAIR].y+18, DT_CENTER);
	}
}

//���Ƹ���ʱ��
VOID CGameClientView::DrawFuZhuClock( CDC* pDC)
{
	for(BYTE i=0;i<GAME_PLAYER;i++)
	{
		if(ME_VIEW_CHAIR == i) continue;

		if (m_bShowFuZhuClock[i] && m_dwFuZhuTime > 0)
		{
			//����ʱ��
			m_PngTimerFlag.DrawImage(pDC, m_ptClock[i].x, m_ptClock[i].y);
			DrawNumber(pDC, &m_PngTimeNumber, TEXT("0123456789"), (LONGLONG)m_dwFuZhuTime,
				m_ptClock[i].x+36, m_ptClock[i].y+18, DT_CENTER);
		}
	}
	return;
}

//�����
BOOL CGameClientView::OnCommand(WPARAM wParam, LPARAM lParam)
{
	UINT nCtrlID = LOWORD(wParam);
	switch (nCtrlID)
	{
		case IDC_LOOK_SCORE:
			{
				//�ı�״̬
				m_bShowHistory = !m_bShowHistory;
				m_btLookScore.SetButtonImage((m_bShowHistory==true)?IDB_BT_SCORE_CLOSE:IDB_BT_SCORE_SHOW, AfxGetInstanceHandle(), false, false);

				//���½���
				RefreshGameView();

				return TRUE;
			}
		case IDC_START:
			{
				SendEngineMessage(IDM_START,0,0);
				
				//����ʱ��
				ZeroMemory(m_bShowFuZhuClock, sizeof(m_bShowFuZhuClock));
				m_dwFuZhuTime=0;
				m_cbTipInfoState=0;

				//���½���
				RefreshGameView();
				return TRUE;
			}
		case IDC_CALL_BANKER:
			{
				SendEngineMessage(IDM_CALL_BANKER, 0, 0);
				return TRUE;
			}
		case IDC_NO_CALL_BANKER:
			{
				SendEngineMessage(IDM_NO_CALL_BANKER, 0, 0);
				return TRUE;
			}
		case IDC_SPLIT_CARD:
			{
				SendEngineMessage(IDM_SPLIT_CARD, 0, 0) ;
				return TRUE;
			}
		case IDC_FRONT_CARD:
			{
				SendEngineMessage(IDM_FRONTCARD,0,0);
				return TRUE;
			}
		case IDC_MID_CARD:
			{
				SendEngineMessage(IDM_MIDCARD,0,0);
				return TRUE;
			}
		case IDC_BACK_CARD:
			{
				SendEngineMessage(IDM_BACKCARD,0,0);
				return TRUE;
			}
		case IDC_SHOW_CARD:
			{
				SendEngineMessage(IDM_SHOWCARD,0,0);
				return TRUE;
			}
		case IDC_SORT_CARD:
			{
				SendEngineMessage(IDM_SORT_CARD, 0, 0);
				return TRUE;
			}
		case IDC_TRUSTEE:
			{
				SendEngineMessage(IDM_TRUSTEE, 0, 0);
				return TRUE;
			}
		case IDC_SET_CHIP_0:
		case IDC_SET_CHIP_1:
		case IDC_SET_CHIP_2:
		case IDC_SET_CHIP_3:
			{
				UINT nPressedID = (nCtrlID - IDC_SET_CHIP_0);
				SendEngineMessage(IDM_SETCHIP , (WPARAM)nPressedID, 0) ;
				return TRUE;
			}
		case IDC_PROMPT_1:
		case IDC_PROMPT_2:
		case IDC_PROMPT_3:
		case IDC_PROMPT_4:
		case IDC_PROMPT_5:
		case IDC_PROMPT_6:
		case IDC_PROMPT_7:
		case IDC_PROMPT_8:
			{
				UINT nPressedID = (nCtrlID - IDC_PROMPT_1);
				SendEngineMessage(IDM_PROMPT, (WPARAM)nPressedID, 0);
				return TRUE;
			}
		case IDC_RESORE_0:
		case IDC_RESORE_1:
		case IDC_RESORE_2:
		case IDC_RESORE_3:
			{
				UINT nPressedID = (nCtrlID - IDC_RESORE_0);
				SendEngineMessage(IDM_RESTORE, nPressedID, 0);
				return TRUE;
			}
		case IDC_CHOSE_YUYIN0:
		case IDC_CHOSE_YUYIN1:
		case IDC_CHOSE_YUYIN2:
		case IDC_CHOSE_YUYIN3:
			{
				UINT nPressedID = (nCtrlID - IDC_CHOSE_YUYIN0);
				SendEngineMessage(IDM_VOICE_PHRASE, (BYTE)nPressedID, 0);
				return TRUE;
			}
		case IDC_YU_YIN_OPEN:
			{
				//SetVoicePanelStatus();
				return TRUE;
			}

	}

	return __super::OnCommand(wParam, lParam);
}

//���˫��
VOID CGameClientView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	__super::OnLButtonDblClk(nFlags, point);

	if (m_UserCardControl[ME_VIEW_CHAIR].GetDirection()==false ||
		m_UserCardControl[ME_VIEW_CHAIR].GetPositively()==false) return;

	//��ȡ����
	m_wMouseDownItem=m_UserCardControl[ME_VIEW_CHAIR].SwitchCardPoint(point);

	BYTE tmpCardData[HAND_CARD_COUNT];
	if (m_wMouseDownItem == INVALID_ITEM && 
		0 != m_UserCardControl[ME_VIEW_CHAIR].GetShootCard(tmpCardData, HAND_CARD_COUNT))
	{
		m_UserCardControl[ME_VIEW_CHAIR].ShootAllCard(false);
		m_wMouseDownItem = INVALID_ITEM;
		m_wHoverCardItem = INVALID_ITEM;

		m_btFrontCard.EnableWindow(FALSE);
		m_btMidCard.EnableWindow(FALSE);
		m_btBackCard.EnableWindow(FALSE);	
	}
	
	
	return;
}

//�����Ϣ
VOID CGameClientView::OnLButtonUp(UINT nFlags, CPoint Point)
{
	__super::OnLButtonUp(nFlags, Point);

	if (m_wMouseDownItem==INVALID_ITEM) return;
	if (m_UserCardControl[ME_VIEW_CHAIR].GetDirection()==false ||
		m_UserCardControl[ME_VIEW_CHAIR].GetPositively()==false) return;

	//��ȡ����
	DWORD dwCurrentIndex=m_UserCardControl[ME_VIEW_CHAIR].SwitchCardPoint(Point);
	//if (dwCurrentIndex!=m_wMouseDownItem) return;//�������A���������ƶ���B�ٷſ��������Ч

	//������֤
	//ASSERT(dwCurrentIndex>=0 && dwCurrentIndex<=HAND_CARD_COUNT);
	if (dwCurrentIndex == INVALID_ITEM)
	{
		//�����˿�
		if (m_wHoverCardItem>m_wMouseDownItem)
		{
			m_UserCardControl[ME_VIEW_CHAIR].SetShootIndex(m_wMouseDownItem,m_wHoverCardItem);
		}
		else
		{
			m_UserCardControl[ME_VIEW_CHAIR].SetShootIndex(m_wHoverCardItem,m_wMouseDownItem);
		}
	}
	else
	{
		if (m_bMoveMouse)
		{
			//�����˿�
			if (m_wHoverCardItem>m_wMouseDownItem)
			{
				m_UserCardControl[ME_VIEW_CHAIR].SetShootIndex(m_wMouseDownItem,m_wHoverCardItem);
			}
			else
			{
				m_UserCardControl[ME_VIEW_CHAIR].SetShootIndex(m_wHoverCardItem,m_wMouseDownItem);
			}
		}
		else
		{
			//���ÿؼ�
			tagCardItem * pCardItem=m_UserCardControl[ME_VIEW_CHAIR].GetCardFromIndex((WORD)dwCurrentIndex);
			pCardItem->bShoot=!pCardItem->bShoot;
		}

	}


	//������Ϣ
	SendEngineMessage(IDM_LEFT_HIT_CARD, 0, 0);

	//�ͷ����
	ReleaseCapture();

	//ˢ�½���
	RefreshGameView();

	//��������
	m_wHoverCardItem=INVALID_ITEM;
	m_wMouseDownItem=INVALID_ITEM;
	m_bMoveMouse = false;
	
	m_UserCardControl[ME_VIEW_CHAIR].SetShootIndex(INVALID_ITEM, INVALID_ITEM);


	return;
}

//�����Ϣ
VOID CGameClientView::OnLButtonDown(UINT nFlags, CPoint Point)
{
	__super::OnLButtonDown(nFlags, Point);

	//״̬�ж�
	if ((m_UserCardControl[ME_VIEW_CHAIR].GetDirection()==false) ||
		(m_UserCardControl[ME_VIEW_CHAIR].GetPositively()==false)) return;

	//��ȡ����
	m_wMouseDownItem=m_UserCardControl[ME_VIEW_CHAIR].SwitchCardPoint(Point);

	//��ʼ��������
	m_wHoverCardItem = m_wMouseDownItem;
	m_bMoveMouse = false;
	return;
}

//����ƶ�
VOID CGameClientView::OnMouseMove( UINT nFlags, CPoint Point )
{
	__super::OnMouseMove(nFlags, Point);

	//��������
	WORD wHoverCardItem=INVALID_ITEM;
	
	//�ƶ��ж�
	if (m_wHoverCardItem != INVALID_ITEM)
	{

		//�˿˴�С
		CSize ControlSize;
		m_UserCardControl[ME_VIEW_CHAIR].GetControlSize(ControlSize);

		//�˿�λ��
		CPoint OriginPoint;
		m_UserCardControl[ME_VIEW_CHAIR].GetOriginPoint(OriginPoint);

		//���е���
		if (Point.x<OriginPoint.x) Point.x=OriginPoint.x;
		if (Point.x>(OriginPoint.x+ControlSize.cx)) Point.x=(OriginPoint.x+ControlSize.cx );

		//��ȡ����
		Point.y=OriginPoint.y+DEF_SHOOT_DISTANCE;
		
		wHoverCardItem=m_UserCardControl[ME_VIEW_CHAIR].SwitchCardPoint(Point);

		//�ƶ�����
		if (wHoverCardItem!=m_wHoverCardItem) m_bMoveMouse=true;

	}

	//ѡ���˿�
	if (wHoverCardItem != m_wHoverCardItem)
	{
		//�����˿�
		if (wHoverCardItem>m_wMouseDownItem)
		{
			m_UserCardControl[ME_VIEW_CHAIR].SetSelectIndex(m_wMouseDownItem, m_wHoverCardItem + 1);
		}
		else
		{
			m_UserCardControl[ME_VIEW_CHAIR].SetSelectIndex(m_wHoverCardItem  - 1, m_wMouseDownItem);
		}
		
		RefreshGameView();
	}
	//���ñ���
	m_wHoverCardItem=wHoverCardItem;
}


//�����Ϣ
BOOL CGameClientView::OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage)
{
	//��ȡ���
	CPoint MousePoint;
	GetCursorPos(&MousePoint);
	ScreenToClient(&MousePoint);

	//�˿˲���
	if (m_UserCardControl[ME_VIEW_CHAIR].OnEventSetCursor(MousePoint)==true) return TRUE;

	return __super::OnSetCursor(pWnd,nHitTest,uMessage);
}

//��������
VOID CGameClientView::PlayCardTypeSound(WORD wChairID, BYTE cbCardType, BYTE cbMaxCard)
{
	if (false == CGlobalUnits::GetInstance()->m_bAllowSound)
		return;
	
	//��ȡ�Ա�
	ASSERT((wChairID >= 0) && (wChairID < GAME_PLAYER));
	BYTE cbGender = GetClientUserItem(wChairID)->GetGender();

	//��������
	CString szDirName;
	TCHAR szFullPath[MAX_PATH] = TEXT("");
	TCHAR szFilePath[MAX_PATH] = TEXT("");
	TCHAR szCommonPath[MAX_PATH] = TEXT("");
	_sntprintf(szCommonPath, CountArray(szCommonPath), TEXT("%s\\%s"), m_szSoundPath, (GENDER_MANKIND==cbGender)?TEXT("Men"):TEXT("Women"));
	
	//���������
	srand((unsigned int)time(NULL));
	INT nRand = rand()%RAND_INDEX;

	switch(cbCardType)
	{
	case CT_ONE_DOUBLE:
		{
			if (nRand > RAND_INDEX/RAND_FACTOR) 
			{
				switch (cbMaxCard)
				{
				case 11: szDirName = TEXT("duizi\\J"); break;
				case 12: szDirName = TEXT("duizi\\Q"); break;
				case 13: szDirName = TEXT("duizi\\K"); break;
				case 14: szDirName = TEXT("duizi\\A"); break;
				default: szDirName = TEXT("duizi\\Other"); break;
				}
			}
			else szDirName = TEXT("duizi\\Next");

			break;
		}
	case CT_TWO_DOUBLE:
		{
			if (nRand > RAND_INDEX/RAND_FACTOR)
			{
				switch (cbMaxCard)
				{
				case 11: szDirName = TEXT("liangdui\\J"); break;
				case 12: szDirName = TEXT("liangdui\\Q"); break;
				case 13: szDirName = TEXT("liangdui\\K"); break;
				case 14: szDirName = TEXT("liangdui\\A"); break;
				default: szDirName = TEXT("liangdui\\Other"); break;
				}
			}
			else szDirName = TEXT("liangdui\\Next");

			break;
		}
	case CT_THREE:
		{
			if (nRand > RAND_INDEX/RAND_FACTOR)
			{
				switch (cbMaxCard)
				{
				case 11: szDirName = TEXT("santiao\\J"); break;
				case 12: szDirName = TEXT("santiao\\Q"); break;
				case 13: szDirName = TEXT("santiao\\K"); break;
				case 14: szDirName = TEXT("santiao\\A"); break;
				default: szDirName = TEXT("santiao\\Other"); break;
				}
			}
			else szDirName = TEXT("santiao\\Next");

			break;
		}
	case CT_FIVE_MIXED_FLUSH_FIRST_A:
	case CT_FIVE_MIXED_FLUSH_NO_A:
	case CT_FIVE_MIXED_FLUSH_BACK_A:
		{
			if (nRand > RAND_INDEX/RAND_FACTOR) 
			{
				switch (cbMaxCard)
				{
				case 11: szDirName = TEXT("shunzi\\J"); break;
				case 12: szDirName = TEXT("shunzi\\Q"); break;
				case 13: szDirName = TEXT("shunzi\\K"); break;
				case 14: szDirName = TEXT("shunzi\\A"); break;
				default: szDirName = TEXT("shunzi\\Other"); break;
				}
			}
			else szDirName = TEXT("shunzi\\Next");

			break;
		}
	case CT_FIVE_FLUSH:
		{
			if (nRand > RAND_INDEX/RAND_FACTOR) 
			{
				switch (cbMaxCard)
				{
				case 11: szDirName = TEXT("tonghua\\J"); break;
				case 12: szDirName = TEXT("tonghua\\Q"); break;
				case 13: szDirName = TEXT("tonghua\\K"); break;
				case 14: szDirName = TEXT("tonghua\\A"); break;
				default: szDirName = TEXT("tonghua\\Other"); break;
				}
			}
			else szDirName = TEXT("tonghua\\Next");

			break;
		}
	case CT_FIVE_THREE_DEOUBLE:
		{
			if (nRand > RAND_INDEX/RAND_FACTOR) 
			{
				switch (cbMaxCard)
				{
				case 11: szDirName = TEXT("hulu\\J"); break;
				case 12: szDirName = TEXT("hulu\\Q"); break;
				case 13: szDirName = TEXT("hulu\\K"); break;
				case 14: szDirName = TEXT("hulu\\A"); break;
				default: szDirName = TEXT("hulu\\Other"); break;
				}
			}
			else szDirName = TEXT("hulu\\Next");

			break;
		}
	case CT_FIVE_FOUR_ONE:
		{
			if (nRand > RAND_INDEX/RAND_FACTOR)
			{
				switch (cbMaxCard)
				{
				case 2: szDirName = TEXT("tiezhi\\2"); break;
				case 3: szDirName = TEXT("tiezhi\\3"); break;
				case 4: szDirName = TEXT("tiezhi\\4"); break;
				case 5: szDirName = TEXT("tiezhi\\5"); break;
				case 6: szDirName = TEXT("tiezhi\\6"); break;
				case 7: szDirName = TEXT("tiezhi\\7"); break;
				case 8: szDirName = TEXT("tiezhi\\8"); break;
				case 9: szDirName = TEXT("tiezhi\\9"); break;
				case 10: szDirName = TEXT("tiezhi\\10");break;
				case 11: szDirName = TEXT("tiezhi\\J"); break;
				case 12: szDirName = TEXT("tiezhi\\Q"); break;
				case 13: szDirName = TEXT("tiezhi\\K"); break;
				case 14: szDirName = TEXT("tiezhi\\A"); break;
				}
			}
			else szDirName = TEXT("tiezhi\\Other");

			break;
		}
	case CT_FIVE_STRAIGHT_FLUSH_FIRST_A:
	case CT_FIVE_STRAIGHT_FLUSH:
		{
			switch (cbMaxCard)
			{
			case 5:  szDirName = TEXT("tonghuashun\\5"); break;
			case 6:  szDirName = TEXT("tonghuashun\\6"); break;
			case 7:  szDirName = TEXT("tonghuashun\\7"); break;
			case 8:  szDirName = TEXT("tonghuashun\\8"); break;
			case 9:	 szDirName = TEXT("tonghuashun\\9"); break;
			case 10: szDirName = TEXT("tonghuashun\\10");break;
			case 11: szDirName = TEXT("tonghuashun\\J"); break;
			case 12: szDirName = TEXT("tonghuashun\\Q"); break;
			case 13: szDirName = TEXT("tonghuashun\\K"); break;
			case 14: szDirName = TEXT("tonghuashun\\A"); break;
			default: szDirName = TEXT("tonghuashun\\Other");break;
			}

			break;
		}
	default:
		{
			if (nRand > RAND_INDEX/RAND_FACTOR)
			{
				switch (cbMaxCard)
				{
				case 11: szDirName = TEXT("sanpai\\J"); break;
				case 12: szDirName = TEXT("sanpai\\Q"); break;
				case 13: szDirName = TEXT("sanpai\\K"); break;
				case 14: szDirName = TEXT("sanpai\\A"); break;
				default: szDirName = TEXT("sanpai\\Other");break;
				}
			}
			else szDirName = TEXT("sanpai\\Next");
			
			break;
		}
	}	

	//GetRandomFile��Ҫһ��Ŀ¼����
	_sntprintf(szFilePath, CountArray(szFilePath), TEXT("%s\\%s"), szCommonPath, szDirName);
	_sntprintf(szFullPath, CountArray(szFullPath), TEXT("%s\\%s"), szFilePath, GetRandomFile(szFilePath));

	//��������
	PlayGameSound(szFullPath);
	return;
}

//��������
VOID CGameClientView::PlaySpecCardTypeSound(WORD wChairID, BYTE cbSpecialType)
{
	if (false == CGlobalUnits::GetInstance()->m_bAllowSound)
		return;

	//��֤����
	ASSERT((cbSpecialType > CT_EX_INVALID) && (cbSpecialType <= CT_EX_ZHIZUNQINGLONG));
	if ((cbSpecialType <= CT_EX_INVALID) || (cbSpecialType > CT_EX_ZHIZUNQINGLONG)) return;

	//��֤����
	ASSERT((wChairID >=0) && (wChairID < GAME_PLAYER));
	BYTE cbGender = GetClientUserItem(wChairID)->GetGender();

	//��������
	CString szDirName;
	TCHAR szFullPath[MAX_PATH] = TEXT("");
	TCHAR szFilePath[MAX_PATH] = TEXT("");
	TCHAR szCommonPath[MAX_PATH] = TEXT("");
	_sntprintf(szCommonPath, CountArray(szCommonPath), TEXT("%s\\%s"), m_szSoundPath, (GENDER_MANKIND==cbGender)?TEXT("Men"):TEXT("Women"));

	switch(cbSpecialType)
	{
	case CT_EX_SANTONGHUA:			szDirName = TEXT("santonghua");			break;
	case CT_EX_SANSHUNZI:			szDirName = TEXT("sanshunzi");			break;
	case CT_EX_LIUDUIBAN:			szDirName = TEXT("liuduiban");			break;
	case CT_EX_WUDUISANTIAO:		szDirName = TEXT("5dui3tiao");			break;
	case CT_EX_SITAOSANTIAO:		szDirName = TEXT("4tao3tiao");			break;
	case CT_EX_SHUANGGUAICHONGSAN:	szDirName = TEXT("shuangguaichongsan");	break;
	case CT_EX_COUYISE:				szDirName = TEXT("chouyise");			break;
	case CT_EX_QUANXIAO:			szDirName = TEXT("quanxiao");			break;
	case CT_EX_QUANDA:				szDirName = TEXT("quanda");				break;
	case CT_EX_SANFENGTIANXIA:		szDirName = TEXT("sanfentianxia");		break;
	case CT_EX_SANTONGHUASHUN:		szDirName = TEXT("santonghuashun");		break;
	case CT_EX_SHIERHUANGZU:		szDirName = TEXT("12huangzhu");			break;
	case CT_EX_YITIAOLONG	:		szDirName = TEXT("yitiaolong");			break;
	case CT_EX_ZHIZUNQINGLONG:		szDirName = TEXT("qinglong");			break;
	}

	//GetRandomFile��Ҫһ��Ŀ¼����
	_sntprintf(szFilePath, CountArray(szFilePath), TEXT("%s\\%s"), szCommonPath, szDirName);
	_sntprintf(szFullPath, CountArray(szFullPath), TEXT("%s\\%s"), szFilePath, GetRandomFile(szFilePath));
	PlayGameSound(szFullPath);
	return;
}

//��������ļ���
LPCTSTR CGameClientView::GetRandomFile(LPCTSTR pszPath)
{
	INT nFileCount = 0;
	HANDLE hFind = INVALID_HANDLE_VALUE;
	WIN32_FIND_DATA wfd;
	TCHAR szFullPath[MAX_PATH] = TEXT("");
	_sntprintf(szFullPath, CountArray(szFullPath), TEXT("%s\\*.wav"), pszPath);

	//ͳ���ļ�����
	hFind = FindFirstFile(szFullPath, &wfd);

	ASSERT(hFind != INVALID_HANDLE_VALUE);
	
	BOOL bRet = ((hFind != INVALID_HANDLE_VALUE) ? TRUE : FALSE);
	while(bRet)
	{
		if(!(wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))//�ļ�
		{
			nFileCount++;
		}
		bRet = FindNextFile(hFind, &wfd);
	}

	//�����ļ�����һ�����ѡ
	if (nFileCount > 1)
	{	
		//��ʼ��ʱ������
		srand((unsigned int)time(NULL));

		//�������
		INT nCount = 0;
		INT nRandIndex = rand()%RAND_INDEX;
		nRandIndex = nRandIndex%nFileCount;

		//�����ļ�
		hFind = FindFirstFile(szFullPath, &wfd);
		bRet = ((hFind != INVALID_HANDLE_VALUE) ? TRUE : FALSE);
		while(bRet && (nCount <= nRandIndex))
		{
			if(!(wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))//�ļ�
			{
				nCount++;
			}
			bRet = FindNextFile(hFind, &wfd);
		}
	}

	return wfd.cFileName;
}

//���ƶ���, 0: ��ע����, 1: ��ע����, 2: ��ע�ϲ�����ע, 3: Ӯ���ճ���
VOID CGameClientView::DrawMoveAnte( WORD wViewChairID, LONGLONG lTableScore)
{
	//������֤
	ASSERT ((wViewChairID>=0) && (wViewChairID<GAME_PLAYER));

	m_JettonStatus[wViewChairID].wMoveIndex = 0;
	m_JettonStatus[wViewChairID].ptFrom = m_ptAvatar[wViewChairID]; 
	m_JettonStatus[wViewChairID].ptDest = m_ptChip[wViewChairID];
	m_JettonStatus[wViewChairID].ptCourse = m_ptChip[wViewChairID];
	m_JettonStatus[wViewChairID].lGold =lTableScore;

	//��������
	int nAnimeStep = 60;

	//λ�Ƽ���
	INT nXCount=abs(m_JettonStatus[wViewChairID].ptDest.x-m_JettonStatus[wViewChairID].ptFrom.x)/nAnimeStep+rand()%8;
	INT nYCount=abs(m_JettonStatus[wViewChairID].ptDest.y-m_JettonStatus[wViewChairID].ptFrom.y)/nAnimeStep+rand()%8;
	m_JettonStatus[wViewChairID].wMoveCount = __max(1,__max(nXCount,nYCount));

	//����ʱ��
	if(!m_bJettonAction)
	{
		m_bJettonAction = true;
		SetTimer(IDI_JETTON_ACTION,30,NULL);	//--------��ǰ��50
	}

	return;
}

//�ƶ�����
bool CGameClientView::MoveJetton()
{
	bool bAllClean = true;

	//���ñ���
	for(BYTE i=0;i<GAME_PLAYER;i++)
	{
		//�ƶ�����
		if(m_JettonStatus[i].wMoveIndex<m_JettonStatus[i].wMoveCount)
		{
			bAllClean = false;
			m_JettonStatus[i].wMoveIndex++;
			WORD wMoveIndex = m_JettonStatus[i].wMoveIndex;
			WORD wMoveCount = m_JettonStatus[i].wMoveCount;
			m_JettonStatus[i].ptCourse.x = m_JettonStatus[i].ptFrom.x + (m_JettonStatus[i].ptDest.x-m_JettonStatus[i].ptFrom.x)*wMoveIndex/wMoveCount;
			m_JettonStatus[i].ptCourse.y = m_JettonStatus[i].ptFrom.y + (m_JettonStatus[i].ptDest.y-m_JettonStatus[i].ptFrom.y)*wMoveIndex/wMoveCount;
		}

		if(m_JettonStatus[i].wMoveCount>0)
		{
			//���봦��
			m_lUserChip[i] = m_JettonStatus[i].lGold;
		}

		//��������
		if (m_JettonStatus[i].wMoveIndex==m_JettonStatus[i].wMoveCount)
		{
			//״̬�ָ�
			m_JettonStatus[i].wMoveIndex = 0;
			m_JettonStatus[i].wMoveCount = 0;
			m_JettonStatus[i].ptFrom = m_ptAvatar[i]; 
			m_JettonStatus[i].ptDest = m_ptChip[i];
			m_JettonStatus[i].ptCourse = m_ptChip[i];
		}
	}

	if(bAllClean) m_bJettonAction = false;

	//���½���
	InvalidGameView(0,0,0,0);

	return !bAllClean;
}

//��������
VOID CGameClientView::AnalyseChip(WORD wChairID, LONGLONG lChip)
{
	ASSERT((wChairID>=0 && wChairID<GAME_PLAYER) && (lChip >= 0));

	LONGLONG lChipArray[] = {1L,5L,10L,50L,100L,500L,1000L,5000L,10000L,50000L,100000L,500000L,1000000L, 5000000L};
	LONGLONG lTemp = lChip;

	for (UINT i=JETTON_COUNT-1; i>=0 && lTemp>0; i--)
	{
		if (lTemp < lChipArray[i]) continue;
		else
		{
			m_JettonData[wChairID][i] = (BYTE)(lTemp/lChipArray[i]);
			lTemp -= (m_JettonData[wChairID][i]*lChipArray[i]);
		}
	}

	return;
}

//����ʱ��
void CGameClientView::SetGlobalClock( bool isClock )
{
	m_bGlobalClock = isClock;
}

//����ȫ��ʱ��
void CGameClientView::SetElapseTime( DWORD dwElapseTimer )
{
	m_dwElapseTime = dwElapseTimer;
}

//��ȡ��ʾ
bool CGameClientView::GetShowCardTip()
{
	return m_bShowTips;
}

//����ʱ��
void CGameClientView::ShowFuZhuClock(WORD wChairID,bool bShow)
{
	if(INVALID_CHAIR == wChairID)
	{
		ZeroMemory(m_bShowFuZhuClock, sizeof(m_bShowFuZhuClock));
		m_dwFuZhuTime=0;
	}
	else
	{
		m_bShowFuZhuClock[wChairID]=bShow;
	}
	return;
}

//���ø���ʱ��
void CGameClientView::SetFuZhuTime(DWORD dwFuZhuTime)
{
	m_dwFuZhuTime = dwFuZhuTime;
	return;
}

//������ʾ��Ϣ
void CGameClientView::SetTipInfoState(BYTE cbTipInfoState)
{
	m_cbTipInfoState=cbTipInfoState;
	return;
}


