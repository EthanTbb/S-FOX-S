#include "StdAfx.h"
#include "GameClient.h"
#include "GameClientView.h"
#include ".\gameclientview.h"

//////////////////////////////////////////////////////////////////////////
//宏定义
//////////////////////////////////////////////////////////////////////////
//按钮标识 
#define IDC_MIN_BUTTON					1104								//最小按钮
#define IDC_SOUND						1105								//声音按钮
#define IDC_CLOSE_BUTTON				1106								//关闭按钮
#define IDC_OPTION						1107								//设置按钮

//动画标识
#define IDI_HIDE_CARD					100								//隐藏扑克
#define IDI_SEND_CARD					101								//发牌标识
#define IDI_USER_ACTION					102								//动作标识
#define IDI_MOVE_CHIP					103								//筹码移动
#define IDI_MOVE_EYE					104								//闪瞎眼睛

//动画数目
#define SEND_CARD_COUNT					5								//发牌数目

//位置定义
#define CARD_EXCURSION_X				283								//扑克偏移
#define CARD_EXCURSION_Y				177								//扑克偏移

//发牌定义
#define SEND_STEP_COUNT					8								//步数
#define SPEED_SEND_CARD					10								//发牌速度

//筹码定义
#define CHIP_COUNT						14								//筹码数量
#define PILING_HEIGHT					6								//堆积高度

//聊天控件
#define IDC_CHAT_INPUT					209								//控件标识
#define IDC_CHAT_DISPLAY				210								//控件标识
#define IDC_SEND_MESSAGE				211								//发送消息
#define IDC_HISTORY_CHAT				212								//查看聊天
#define IDC_MUTE						104								//静音按钮

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameClientView, CGameFrameViewGDI)

	//系统消息
	ON_WM_TIMER()
	ON_WM_CREATE()
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONDOWN()
	ON_WM_CTLCOLOR()


	//按钮消息
	ON_BN_CLICKED(IDC_START, OnBnClickedStart)
	ON_BN_CLICKED(IDC_CONTROL_FOLLOW, OnBnClickedFollow)
	ON_BN_CLICKED(IDC_CONTROL_GIVEUP, OnBnClickedGiveUp)
	ON_BN_CLICKED(IDC_CONTROL_SHOWHAND, OnBnClickedShowHand)

	ON_BN_CLICKED(IDC_ADD_SCORE, OnBnClickedAddScore)
	ON_BN_CLICKED(IDC_ADD_SCORE_ONE, OnBnClickedAddScoreOne)
	ON_BN_CLICKED(IDC_ADD_SCORE_TWO, OnBnClickedAddScoreTwo)
	ON_BN_CLICKED(IDC_ADD_SCORE_THREE, OnBnClickedAddScoreThree)
	ON_BN_CLICKED(IDC_ADD_SCORE_FOUR, OnBnClickedAddScoreFour)

	ON_BN_CLICKED(IDC_CONTROL_OK, OnBnClickedOK)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//构造函数
CGameClientView::CGameClientView()
{
	//状态变量
	m_dwChatTime=0;
	m_bShowChatMessage=true;
	m_bFirstChange = TRUE;
	m_lCellScore=0L;
	m_lDrawMaxScore=0L;
	ZeroMemory(m_lUserScore,sizeof(m_lUserScore));
	ZeroMemory(m_lUserAddScore,sizeof(m_lUserAddScore));
	ZeroMemory(m_lUserAddAllScore,sizeof(m_lUserAddAllScore));
	ZeroMemory(m_lUserSaveScore,sizeof(m_lUserSaveScore));
	ZeroMemory(m_lTableScore,sizeof(m_lTableScore));
	ZeroMemory(m_lOverScore,sizeof(m_lOverScore));
	ZeroMemory(m_lOverSaveScore,sizeof(m_lOverSaveScore));
	ZeroMemory(m_cbUserEye,sizeof(m_cbUserEye));
	

	//动画变量
	m_SendCardPos.SetPoint(0,0);
	m_SendCardCurPos = m_SendCardPos;
	m_nStepCount = SEND_STEP_COUNT;
	m_nXStep = 0;
	m_nYStep = 0;

	//动作变量
	m_cbUserAction=0;
	m_wActionUser=INVALID_CHAIR;

	//状态变量
	m_bShowScore=false;
	ZeroMemory(m_pHistoryScore,sizeof(m_pHistoryScore));

	//扑克筹码
	ZeroMemory(m_lPalyBetChip,sizeof(m_lPalyBetChip));
	m_lALLBetChip = 0;

	for ( int i = 0; i < GAME_PLAYER; ++i )
	{
		m_CardControl[i].Initialize( i != 2 );
		m_wTableUser[i] = INVALID_CHAIR;
	}

	//加载资源
	HINSTANCE hResInstance=AfxGetInstanceHandle();
	m_ImageReady.LoadImage(this, hResInstance, TEXT("READY"));

	m_ImageGameCard.LoadImage(this, hResInstance, TEXT("GAME_CARD"));
	m_ImageViewBack.LoadImage(this, hResInstance,TEXT("VIEW_BACK"));
	m_ImageTable.LoadImage(this, hResInstance,TEXT("TABLE"));
	m_ImgaeScoreTotalNumber.LoadImage(this, hResInstance,TEXT("SCORE_NUMBER"));
	m_ImageActionBack.LoadImage(this, hResInstance,TEXT("ACTION_BACK"));
	m_ImageActionFont.LoadImage(this, hResInstance,TEXT("ACTION_FONT"));
	m_ImageShowHand.LoadImage(this, hResInstance,TEXT("SHOW_HAND"));

	m_ImageUserInfo.LoadImage(this, hResInstance,TEXT("USER_INFO"));
	m_ImageUserPaly.LoadImage(this, hResInstance,TEXT("USERPLAYING_INFO"));
	m_ImageChip.LoadImage(this, hResInstance,TEXT("JETTON_VIEW"));

	m_ImageCellNumber.LoadImage(this, hResInstance,TEXT("CELL_NUMBER"));
	m_ImageUserScore.LoadImage(this, hResInstance,TEXT("USER_SCORE"));

	m_ImgaeScoreBackR.LoadImage(this, hResInstance,TEXT("GAME_SCORE_R"));
	m_ImgaeScoreBackL.LoadImage(this, hResInstance,TEXT("GAME_SCORE_L"));
	m_ImgaeScoreBackM.LoadImage(this, hResInstance,TEXT("GAME_SCORE_M"));

	m_ImgaeMyScoreBackR.LoadImage(this, hResInstance,TEXT("MY_GAME_SCORE_R"));
	m_ImgaeMyScoreBackL.LoadImage(this, hResInstance,TEXT("MY_GAME_SCORE_L"));
	m_ImgaeMyScoreBackM.LoadImage(this, hResInstance,TEXT("MY_GAME_SCORE_M"));

	m_ImgaeScoreNumber.LoadImage(this, hResInstance,TEXT("GAME_OVER_SCORE_NUMBER"));

	m_ImageTimeNumber.LoadImage(this, hResInstance,TEXT("TIME_NUMBER"));
	m_ImageTimeBack.LoadImage(this, hResInstance,TEXT("TIME_BACK"));

	m_ImageCradType.LoadImage(this, hResInstance,TEXT("CARD_TYPE"));

	m_PngChatBack.LoadImage(this, hResInstance,TEXT("CHAT_BACK"));

	m_ImageCellScoreBack.LoadImage(this,hResInstance,TEXT("CELL_SCORE"));

	//获取大小
	m_SizeGameCard.SetSize(m_ImageGameCard.GetWidth()/13,m_ImageGameCard.GetHeight()/5);
	return;
}

//析构函数
CGameClientView::~CGameClientView()
{
	//删除筹码
	for ( int i = 0 ; i < m_ArrayPlayChip.GetCount(); ++i )
		SafeDelete(m_ArrayPlayChip[i]);
	m_ArrayPlayChip.RemoveAll();

	for ( int i = 0 ; i < m_ArrayBetChip.GetCount(); ++i )
		SafeDelete(m_ArrayBetChip[i]);
	m_ArrayBetChip.RemoveAll();

	for ( int i = 0 ; i < m_ArrayOverChip.GetCount(); ++i )
		SafeDelete(m_ArrayOverChip[i]);
	m_ArrayOverChip.RemoveAll();
}

//消息解释
BOOL CGameClientView::PreTranslateMessage(MSG * pMsg)
{
	//提示消息
	if (m_ToolTipCtrl.m_hWnd!=NULL) 
	{
		m_ToolTipCtrl.RelayEvent(pMsg);
	}

	if (pMsg->message == WM_KEYUP)
	{
		if (pMsg-> wParam == VK_RETURN)
		{
			CString str;
			GetDlgItem(IDC_CHAT_INPUT)->GetWindowText(str);
			if (str.GetLength() > 0)
			{
				OnSendMessage();
			}
			else
			{
				GetDlgItem(IDC_CHAT_INPUT)->SetFocus();
			}
		}
	}

// 	if ( pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN 
// 		&& m_PlayOperate.IsShow() && m_PlayOperate.IsAddShow() )
// 	{
// 		//发送消息
// 		SendEngineMessage(IDM_FOLLOW,(WPARAM)m_PlayOperate.GetBetCount(),0);
// 	}

	if ( m_PlayOperate.PreTranslateMessage(pMsg) )
	{
		CRect rect = m_PlayOperate.GetOperateRect();
		InvalidGameView(rect.left, rect.top, rect.Width(), rect.Height());
		return TRUE;
	}

	bool bUpdateGameView = false;
	if ( pMsg->message == WM_CHAR )
	{
		bUpdateGameView = true;

	}

	BOOL bReturn = __super::PreTranslateMessage(pMsg);

	if( bUpdateGameView )
	{
		CRect rect = m_PlayOperate.GetOperateRect();
		InvalidGameView(rect.left, rect.top, rect.Width(), rect.Height());
	}

	return bReturn;
}

//重置界面
VOID CGameClientView::ResetGameView()
{
	__super::ResetGameView();

	//删除时间
	KillTimer(IDI_HIDE_CARD);
	KillTimer(IDI_SEND_CARD);
	KillTimer(IDI_USER_ACTION);

	//状态变量
	m_lCellScore=0L;
	m_lDrawMaxScore=0L;
	ZeroMemory(m_lUserScore,sizeof(m_lUserScore));
	ZeroMemory(m_lUserAddScore,sizeof(m_lUserAddScore));
	ZeroMemory(m_lUserAddAllScore,sizeof(m_lUserAddAllScore));
	ZeroMemory(m_lUserSaveScore,sizeof(m_lUserSaveScore));
	ZeroMemory(m_lTableScore,sizeof(m_lTableScore));
	ZeroMemory(m_lOverScore,sizeof(m_lOverScore));
	ZeroMemory(m_lOverSaveScore,sizeof(m_lOverSaveScore));
	ZeroMemory(m_cbUserEye,sizeof(m_cbUserEye));

	//动画变量
	m_SendCardCurPos = m_SendCardPos;
	m_nStepCount = 0;
	m_nXStep = 0;
	m_nYStep = 0;
	m_SendCardItemArray.RemoveAll();

	//动作变量
	m_cbUserAction=0;
	m_wActionUser=INVALID_CHAIR;

	//状态变量
	m_bShowScore=false;
	ZeroMemory(m_pHistoryScore,sizeof(m_pHistoryScore));

	//控制按钮
	m_btStart.ShowWindow(SW_HIDE);
	m_PlayOperate.ShowOperate(false);

	//禁止按钮
	m_PlayOperate.m_btFollow.EnableWindow(FALSE);
	m_PlayOperate.m_btAddScore.EnableWindow(FALSE);
	m_PlayOperate.m_btShowHand.EnableWindow(FALSE);

	//扑克筹码
	ZeroMemory(m_lPalyBetChip,sizeof(m_lPalyBetChip));
	m_lALLBetChip = 0;

	//界面控件
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		m_CardControl[i].SetCardData(NULL,0);
		m_wTableUser[i] = INVALID_CHAIR;
	}

	//用户扑克
	m_CardControl[MYSELF_VIEW_ID].SetPositively(false);
	m_CardControl[MYSELF_VIEW_ID].SetDisplayHead(false);

	//删除筹码
	for ( int i = 0 ; i < m_ArrayPlayChip.GetCount(); ++i )
		SafeDelete(m_ArrayPlayChip[i]);
	m_ArrayPlayChip.RemoveAll();

	for ( int i = 0 ; i < m_ArrayBetChip.GetCount(); ++i )
		SafeDelete(m_ArrayBetChip[i]);
	m_ArrayBetChip.RemoveAll();

	for ( int i = 0 ; i < m_ArrayOverChip.GetCount(); ++i )
		SafeDelete(m_ArrayOverChip[i]);
	m_ArrayOverChip.RemoveAll();

	return;
}

//调整控件
VOID CGameClientView::RectifyControl(INT nWidth, INT nHeight)
{
	//准备位置
	m_ptReady[0].SetPoint(nWidth/2 - 192, nHeight-369 - 20-15);
	m_ptReady[1].SetPoint(nWidth/2 - 347, nHeight-369 + 130-15);
	m_ptReady[2].SetPoint(nWidth/2 - 310, nHeight-369 + 330-15);
	m_ptReady[3].SetPoint(nWidth/2 + 350, nHeight-369 + 160-15);
	m_ptReady[4].SetPoint(nWidth/2 + 255, nHeight-369 - 7-15);

	// 头像
	m_ptUserInfo[0].SetPoint(320, nHeight - 458);
	m_ptUserInfo[1].SetPoint(80, 509-30);
	m_ptUserInfo[2].SetPoint(90, nHeight-170);
	m_ptUserInfo[3].SetPoint(nWidth/2 + 435, nHeight-369 + 154-15);
	m_ptUserInfo[4].SetPoint(nWidth/2 + 256, nHeight-369 - 60-15);

	//时间位置
	m_ptClock[0].SetPoint(nWidth/2 - 355, nHeight-369 - 252-15);
	m_ptClock[1].SetPoint(nWidth/2 - 410, nHeight-369-15);
	m_ptClock[2].SetPoint(nWidth/2 - 165, nHeight-369 + 180-15);
	m_ptClock[3].SetPoint(nWidth/2 + 390, nHeight-369 + 60-15);
	m_ptClock[4].SetPoint(nWidth/2 + 295, nHeight-369 - 237-15);

	//动作位置
	m_ptUserAction[0].SetPoint(nWidth/2 - 160, nHeight-369 - 230-15);
	m_ptUserAction[1].SetPoint(nWidth/2 - 380, nHeight-369 - 100-15);
	m_ptUserAction[2].SetPoint(nWidth/2 - 360, nHeight-369 + 220-15);
	m_ptUserAction[3].SetPoint(nWidth/2 + 375, nHeight-369 - 45-15);
	m_ptUserAction[4].SetPoint(nWidth/2 + 170, nHeight-369 - 235-15);


	//扑克控件
	m_CardControl[0].SetBenchmarkPos(nWidth/2 - 190, nHeight-369 - 43-15,	enXCenter,enYTop);
	m_CardControl[1].SetBenchmarkPos(nWidth/2 - 383, nHeight-369 + 110-15,	enXLeft,enYCenter);
	m_CardControl[2].SetBenchmarkPos(nWidth/2,		 nHeight-369 + 288 + 8-15,	enXCenter,enYBottom);
	m_CardControl[3].SetBenchmarkPos(nWidth/2 + 385, nHeight-369 + 140-15,	enXRight,enYCenter);
	m_CardControl[4].SetBenchmarkPos(nWidth/2 + 220, nHeight-369 - 30-15,	enXCenter,enYTop);

	//玩家筹码
	m_ptPalyBetChip[0].SetPoint(nWidth/2 - 122, nHeight-369 - 125-15);
	m_ptPalyBetChip[1].SetPoint(nWidth/2 - 120,	nHeight-369 + 30-15);
	m_ptPalyBetChip[2].SetPoint(nWidth/2,		nHeight-369 + 45 + 18-15);
	m_ptPalyBetChip[3].SetPoint(nWidth/2 + 120,	nHeight-369 + 30-15);
	m_ptPalyBetChip[4].SetPoint(nWidth/2 + 122, nHeight-369 - 125-15);

	// 结束积分
	m_ptOverScore[0].SetPoint(nWidth/2 - 188, nHeight-369 + 53-15);
	m_ptOverScore[1].SetPoint(nWidth/2 - 318, nHeight-369 + 165-15);
	m_ptOverScore[2].SetPoint(nWidth/2 - 210, nHeight-369 + 270-15);
	m_ptOverScore[3].SetPoint(nWidth/2 + 322, nHeight-369 + 195-15);
	m_ptOverScore[4].SetPoint(nWidth/2 + 220, nHeight-369 + 66-15);

	// 人物位置
	m_ptUser[0].SetPoint(150, nHeight-658);
	m_ptUser[1].SetPoint(-28, nHeight-442);
	m_ptUser[2].SetPoint(90, nHeight-183);
	m_ptUser[3].SetPoint(nWidth/2 + 364, nHeight-369 - 48-15);
	m_ptUser[4].SetPoint(nWidth/2 + 185, nHeight-369 - 275-15);

	m_ptAvatar[0].SetPoint(250, nHeight-518);
	m_ptAvatar[1].SetPoint(70, nHeight-282);
	m_ptAvatar[2].SetPoint(200, nHeight-43);
	m_ptAvatar[3].SetPoint(nWidth/2 + 464, nHeight-369 + 108-15);
	m_ptAvatar[4].SetPoint(nWidth/2 + 285, nHeight-369 - 135-15);

	//发牌起始位置
	m_SendCardPos.SetPoint(nWidth / 2 + 20,nHeight-369 - 200-15);
	
	//开始按钮
	m_btStart.SetWindowPos( NULL,nWidth/2 - 52,nHeight-369 + 119-15,0,0,SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOCOPYBITS|SWP_NOSIZE);

	//移动调整
	m_btMinButton.SetWindowPos( NULL, nWidth-50-46-43-46, 4, 0, 0, SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOCOPYBITS|SWP_NOSIZE);
	m_btSound.SetWindowPos( NULL, nWidth-50 - 46, 4, 0, 0, SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOCOPYBITS|SWP_NOSIZE);
	m_btCloseButton.SetWindowPos( NULL, nWidth-50, 4, 0, 0, SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOCOPYBITS|SWP_NOSIZE);
	m_btOption.SetWindowPos( NULL, nWidth-50-46-46, 4, 0, 0, SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOCOPYBITS|SWP_NOSIZE);

	//控制窗口
	m_PlayOperate.SetOperatePos( nWidth/2 + 100, nHeight-369 + 78-15);

	// 桌面筹码
	m_rectBetChip.SetRect( nWidth / 2 - 75,nHeight-369, nWidth / 2 + 75,nHeight-369 + 130 + 18-15);

	// 变换筹码位置
	for ( int i = 0 ; i < m_ArrayBetChip.GetCount(); ++i )
	{
		CPoint ptOffset( (nWidth - m_sizeWin.cx)/2,(nHeight - m_sizeWin.cy)/2 -15); 
		m_ArrayBetChip[i]->ptBeginPos += ptOffset;
		m_ArrayBetChip[i]->ptEndPos += ptOffset;
		m_ArrayBetChip[i]->ptCurrentPos += ptOffset;
	}
	
	// 窗口大小
	m_sizeWin.SetSize(nWidth, nHeight);

	HDWP hDwp=BeginDeferWindowPos(32);
	DeferWindowPos(hDwp, m_ChatInput, NULL, nWidth - 229, nHeight-27, 176, 22, SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOCOPYBITS);
	//DeferWindowPos(hDwp, m_ChatDisplay, NULL, nWidth - 260, nHeight-164, 236, 120, SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOCOPYBITS);
	DeferWindowPos(hDwp, m_ChatMessage, NULL, nWidth - 260, nHeight-164, 236+14, 120, SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOCOPYBITS);
	DeferWindowPos(hDwp, m_btSendMessage, NULL, nWidth - 53, nHeight-27, 0, 0, SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOCOPYBITS|SWP_NOSIZE);
	DeferWindowPos(hDwp, m_btHistoryChat, NULL, nWidth - 267, nHeight-27, 0, 0, SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOCOPYBITS|SWP_NOSIZE);
	//结束移动
	EndDeferWindowPos(hDwp);

	//m_ChatDisplay.RectifyControl(nWidth, nHeight);

	return;
}

//绘画界面
VOID CGameClientView::DrawGameView(CDC * pDC, INT nWidth, INT nHeight)
{
	//绘画背景
	m_ImageViewBack.DrawImage( pDC, 0, nHeight - 738);
	CRect rectInfo( 10, 8, 250, 30);

	//绘画用户
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		if(i != MYSELF_VIEW_ID)
		{
// 			IClientUserItem * pIClientUserItem=GetClientUserItem(i);
// 			if (pIClientUserItem == NULL) continue;

			//画人物
			DrawUser(pDC, i);
		}
	}

	m_ImageTable.DrawImage(pDC, 86, nHeight - 479  - 31);

	//画人物
	DrawUser(pDC, MYSELF_VIEW_ID);
	
	m_ImageCellScoreBack.DrawImage(pDC,5,0);
	//信息变量
	if( m_lCellScore != 0 )
		DrawNumber(pDC, &m_ImageCellNumber, TEXT("0123456789"), m_lCellScore,   nWidth/2 - 437, 59-30);
	if( m_lDrawMaxScore != 0 )
		DrawNumber(pDC, &m_ImageCellNumber, TEXT("0123456789"), m_lDrawMaxScore,nWidth/2 - 437, 82-30);

	//计算总注
	LONGLONG lTableScore = m_lALLBetChip;
	for( INT i = 0; i < GAME_PLAYER; i++ )
		lTableScore += m_lPalyBetChip[i];

	//总注信息
	if ( lTableScore > 0L )
	{
		DrawNumber(pDC, &m_ImgaeScoreTotalNumber, TEXT("0123456789"), lTableScore, nWidth/2 - 60, nHeight - 498);
	}

	//用户扑克
	for (BYTE i=0;i<GAME_PLAYER;i++) 
	{
		m_CardControl[i].DrawCardControl(pDC);
	}

	//绘画用户
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		IClientUserItem * pIClientUserItem=GetClientUserItem(i);
		if (pIClientUserItem == NULL) continue;
		
		//绘画时间
		WORD wTime = GetUserClock(i);
		if ( wTime > 0 && wTime <= 99 )
		{
			TCHAR szTime[128] = {0};
			_sntprintf(szTime,CountArray(szTime),TEXT("%02d"), wTime);

			m_ImageTimeBack.DrawImage(pDC, m_ptClock[i].x, m_ptClock[i].y);
			DrawNumber(pDC , &m_ImageTimeNumber, TEXT("0123456789"), szTime, m_ptClock[i].x + m_ImageTimeBack.GetWidth() / 2, m_ptClock[i].y + m_ImageTimeBack.GetHeight() / 2, DT_CENTER, DT_VCENTER);
		}		
			
		if(i != MYSELF_VIEW_ID)
		{
			CPoint ptShowPos;		
			ptShowPos.SetPoint(m_ptUserInfo[i].x - m_ImageUserInfo.GetWidth()/2, m_ptUserInfo[i].y - m_ImageUserInfo.GetHeight()/2);

			// 背景
			m_ImageUserInfo.DrawImage( pDC, ptShowPos.x, ptShowPos.y);

// 			// 外框
// 			if ( pIClientUserItem->GetUserStatus() == US_PLAYING && GetUserClock(i) )
// 			{
// 				m_ImageUserPaly.DrawImage( pDC, m_ptUserInfo[i].x - m_ImageUserPaly.GetWidth()/2, m_ptUserInfo[i].y - m_ImageUserPaly.GetHeight()/2);
// 			}

			// 名字
			CRect rectInfo( ptShowPos.x + 8, ptShowPos.y + 9, ptShowPos.x + 82, ptShowPos.y + 23);
			DrawTextStringEx(pDC,pIClientUserItem->GetNickName(),RGB(255,255,255),RGB(0,0,0),rectInfo, DT_WORDBREAK|DT_EDITCONTROL|DT_END_ELLIPSIS|DT_SINGLELINE|DT_CENTER);


			//用户积分
			rectInfo.OffsetRect( 0, 22 );
			DrawTextStringEx(pDC,AddComma(m_lUserScore[i]),RGB(255,255,0),RGB(0,0,0),rectInfo, DT_WORDBREAK|DT_EDITCONTROL|DT_END_ELLIPSIS|DT_SINGLELINE|DT_CENTER);

		}
		else
		{
			// 名字
			CRect rectInfo(250, nHeight - 22, 240 + 145, nHeight - 6);
			DrawTextStringEx(pDC,pIClientUserItem->GetNickName(),RGB(255,255,255),RGB(0,0,0),rectInfo, DT_WORDBREAK|DT_EDITCONTROL|DT_END_ELLIPSIS|DT_SINGLELINE|DT_CENTER);

// 			TCHAR szOutNum[128] = {0};
// 			_sntprintf(szOutNum,CountArray(szOutNum),TEXT("%I64d"),m_lUserScore[i]);
// 			DrawNumber(pDC, &m_ImageUserScore, TEXT("0123456789"), szOutNum, 503, nHeight - 26, DT_LEFT);

			DrawNumber(pDC, &m_ImageUserScore, TEXT("0123456789"), m_lUserScore[i], 723, nHeight - 26, DT_RIGHT);

//			DrawNumber(pDC, &m_ImageUserScore, TEXT("0123456789"), m_lUserScore[i], 503, nHeight - 26, DT_LEFT);
		}

		// 用户下注
		if( m_lTableScore[i] > 0L )
		{
			DrawOverScore(pDC, m_lTableScore[i], m_ptOverScore[i].x, m_ptOverScore[i].y, i == MYSELF_VIEW_ID, false);
		}
	}

	// 移动筹码
	for ( int i = 0 ; i < m_ArrayBetChip.GetCount(); ++i )
	{
		// 获取属性
		INT nChipHeight = m_ImageChip.GetHeight();
		INT nChipWidth  = m_ImageChip.GetWidth() / CHIP_COUNT;

		// 绘画筹码
		m_ImageChip.DrawImage( pDC, (INT)m_ArrayBetChip[i]->ptCurrentPos.x - nChipWidth/2, (INT)m_ArrayBetChip[i]->ptCurrentPos.y - nChipHeight / 2,
			nChipWidth, nChipHeight, 
			nChipWidth * m_ArrayBetChip[i]->wChipIndex, 0 );
	}

	// 回收筹码
	for ( int i = 0 ; i < m_ArrayOverChip.GetCount(); ++i )
	{
		// 获取属性
		INT nChipHeight = m_ImageChip.GetHeight();
		INT nChipWidth  = m_ImageChip.GetWidth() / CHIP_COUNT;

		// 绘画筹码
		if ( !m_ArrayOverChip[i]->bShow )
			continue;

		m_ImageChip.DrawImage( pDC, (INT)m_ArrayOverChip[i]->ptCurrentPos.x - nChipWidth/2, (INT)m_ArrayOverChip[i]->ptCurrentPos.y - nChipHeight / 2,
			nChipWidth, nChipHeight, 
			nChipWidth * m_ArrayOverChip[i]->wChipIndex, 0 );
	}

	//用户当前筹码
	int nChipCount[GAME_PLAYER] = {0};
	for ( int i = 0 ; i < m_ArrayPlayChip.GetCount(); ++i )
	{
		// 获取属性
		INT nChipHeight = m_ImageChip.GetHeight();
		INT nChipWidth  = m_ImageChip.GetWidth() / CHIP_COUNT;

		if( m_ArrayPlayChip[i]->wChipChairID < GAME_PLAYER && nChipCount[m_ArrayPlayChip[i]->wChipChairID] <= PILING_HEIGHT )
			nChipCount[m_ArrayPlayChip[i]->wChipChairID]++;

		if( nChipCount[m_ArrayPlayChip[i]->wChipChairID] >= PILING_HEIGHT )
			continue;

		// 绘画筹码
		m_ImageChip.DrawImage( pDC, (INT)m_ArrayPlayChip[i]->ptCurrentPos.x - nChipWidth/2, (INT)m_ArrayPlayChip[i]->ptCurrentPos.y - nChipHeight / 2,
			nChipWidth, nChipHeight, 
			nChipWidth * m_ArrayPlayChip[i]->wChipIndex, 0 );
	}

	////用户当前停止筹码
	//for (WORD i=0;i<GAME_PLAYER;i++)
	//{
	//	//获取用户
	//	IClientUserItem * pIClientUserItem=GetClientUserItem(i);
	//	if (pIClientUserItem == NULL) continue;

	//	LONGLONG lPalyBetChip = m_lPalyBetChip[i];
	//	LONGLONG lScoreArray[] = { 1L, 5L, 10L, 50L, 100L, 500L, 1000L, 5000L, 10000L, 50000L, 100000L, 500000L, 1000000L, 5000000L };
	//	UINT	 nChipCount    = 0;
	//	while ( lPalyBetChip > 0  && nChipCount < PILING_HEIGHT )
	//	{
	//		for( int j = CountArray(lScoreArray) - 1; j >=0; j-- )
	//		{
	//			if ( lPalyBetChip >= lScoreArray[j] )
	//			{
	//				// 获取属性
	//				INT nChipHeight = m_ImageChip.GetHeight();
	//				INT nChipWidth  = m_ImageChip.GetWidth() / CHIP_COUNT;
	//				INT nXPos = m_ptPalyBetChip[i].x - nChipWidth/2;
	//				INT nYPos = m_ptPalyBetChip[i].y - nChipHeight/2 - nChipCount * 5;
	//				m_ImageChip.DrawImage(pDC, nXPos, nYPos, nChipWidth, nChipHeight, nChipWidth * j, 0);
	//				lPalyBetChip -= lScoreArray[j];
	//				nChipCount++;
	//				break;
	//			}
	//		}
	//	}


	//	//筹码数字
	//	if( m_lPalyBetChip[i] > 0L )
	//	{
	//		UINT oldAlign = pDC->SetTextAlign(TA_CENTER);
	//		DrawTextStringEx(pDC, AddComma(m_lPalyBetChip[i]), RGB(200,249,248), RGB(0,0,0), m_ptPalyBetChip[i].x,m_ptPalyBetChip[i].y + 15);
	//		pDC->SetTextAlign(oldAlign);
	//	}
	//}

	//绘画发牌扑克
	if ( m_SendCardItemArray.GetCount() > 0 )
	{
		//变量定义
		tagSendCardItem * pSendCardItem=&m_SendCardItemArray[0];

		//获取大小
		int nItemWidth=m_SizeGameCard.cx;
		int nItemHeight=m_SizeGameCard.cy;

		//绘画扑克
		m_ImageGameCard.DrawImage(pDC,m_SendCardCurPos.x,m_SendCardCurPos.y,nItemWidth,nItemHeight,nItemWidth*2,nItemHeight*4);
	}

	//用户动作
	if ( m_wActionUser != INVALID_CHAIR && m_lOverScore[m_wActionUser] == 0 )
	{
		if ( m_cbUserAction != AC_SHOW_HAND )
		{
			//计算位置
			INT nXPos=m_ptUserAction[m_wActionUser].x-m_ImageActionBack.GetWidth()/10;
			INT nYPos=m_ptUserAction[m_wActionUser].y-m_ImageActionBack.GetHeight()/2;

			//绘画背景
			m_ImageActionBack.DrawImage(pDC,nXPos ,nYPos, 
				m_ImageActionBack.GetWidth() / 5,	m_ImageActionBack.GetHeight(),
				m_wActionUser * m_ImageActionBack.GetWidth() / 5, 0);

			//绘画动作
			m_ImageActionFont.DrawImage(pDC,nXPos + 12, nYPos + 7,
				m_ImageActionFont.GetWidth() / 5,	m_ImageActionFont.GetHeight(),
				m_cbUserAction * m_ImageActionFont.GetWidth() / 5, 0);
		}
		else
		{
			//梭哈动作
			m_ImageShowHand.DrawImage(pDC,(nWidth-m_ImageShowHand.GetWidth())/2,(nHeight-m_ImageShowHand.GetHeight())/2 -15);
		}
	}
	
	// 结束信息
	for ( int i = 0; i < GAME_PLAYER; ++i )
	{
		DrawOverScore(pDC, m_lOverScore[i], m_ptOverScore[i].x, m_ptOverScore[i].y, i == MYSELF_VIEW_ID, true);

		//获取用户
		IClientUserItem * pIClientUserItem = GetClientUserItem(i);

		if ( m_lOverScore[i] != 0 && m_CardControl[i].GetCardCount() == MAX_CARD_COUNT && m_CardControl[i].GetDisplayHead()
			&& ( pIClientUserItem == NULL || pIClientUserItem->GetUserStatus() != US_READY))
		{
			BYTE cbCrad[MAX_CARD_COUNT] = {0};
			m_CardControl[i].GetCardData( cbCrad, MAX_CARD_COUNT );

			//计算位置
			CPoint ptCenterPoint;
			m_CardControl[i].GetCenterPoint(ptCenterPoint);
			INT nXPos = ptCenterPoint.x - m_ImageCradType.GetWidth()/18;
			INT nYPos = ptCenterPoint.y - m_ImageCradType.GetHeight()/2;

			//绘画动作
			m_GameLogic.SortCardList( cbCrad, MAX_CARD_COUNT);
			m_ImageCradType.DrawImage(pDC,nXPos, nYPos ,
				m_ImageCradType.GetWidth() / 9,	m_ImageCradType.GetHeight(),
				(m_GameLogic.GetCardGenre( cbCrad, (BYTE)m_CardControl[i].GetCardCount() ) - 1) * m_ImageCradType.GetWidth() / 9, 0);
		}
	}
	
	//准备标志
	for( int i = 0; i < GAME_PLAYER; ++i )
	{
		//获取用户
		IClientUserItem * pIClientUserItem=GetClientUserItem(i);
		if (pIClientUserItem == NULL) continue;

		if (pIClientUserItem->GetUserStatus() == US_READY)
			m_ImageReady.DrawImage(pDC, m_ptReady[i].x - m_ImageReady.GetWidth()/2, m_ptReady[i].y - m_ImageReady.GetHeight()/2);
	}

	// 操作界面
	m_PlayOperate.DrawPlayOperate(pDC,nWidth,nHeight);

	//聊天背景
	if (m_bShowChatMessage == true)
	{
		m_PngChatBack.DrawImage(pDC, nWidth - m_PngChatBack.GetWidth() - 5, nHeight-m_PngChatBack.GetHeight() - 35);
	}


	//聊天输入
	CDC dcBuffer;
	CBitmap bmpBuffer;
	m_brush.DeleteObject();
	dcBuffer.CreateCompatibleDC(pDC);
	bmpBuffer.CreateCompatibleBitmap(pDC, 176, 23);
	CBitmap *pBmpBuffer = dcBuffer.SelectObject(&bmpBuffer);	
	dcBuffer.BitBlt(0, 0, 176, 23, pDC, nWidth - 230, nHeight-28, SRCCOPY);
	dcBuffer.SelectObject(pBmpBuffer);
	m_brush.CreatePatternBrush(&bmpBuffer);
	//m_ChatDisplay.Invalidate();

	return;
}

//是否发牌
bool CGameClientView::IsDispatchStatus()
{
	return m_SendCardItemArray.GetCount()>0;
}

//停止发牌
VOID CGameClientView::ConcludeDispatch()
{
	//完成判断
	if (m_SendCardItemArray.GetCount()==0) return;

	//完成动画
	for (INT_PTR i=0;i<m_SendCardItemArray.GetCount();i++)
	{
		//获取数据
		tagSendCardItem * pSendCardItem=&m_SendCardItemArray[i];

		//获取扑克
		BYTE cbCardData[MAX_COUNT];
		WORD wChairID=pSendCardItem->wChairID;
		BYTE cbCardCount=(BYTE)m_CardControl[wChairID].GetCardData(cbCardData,CountArray(cbCardData));

		//设置扑克
		if (cbCardCount<MAX_COUNT)
		{
			cbCardData[cbCardCount++]=pSendCardItem->cbCardData;
			m_CardControl[wChairID].SetCardData(cbCardData,cbCardCount);
		}
	}

	//删除处理
	KillTimer(IDI_SEND_CARD);
	m_SendCardItemArray.RemoveAll();

	//更新界面
	InvalidGameView(0,0,0,0);

	return;
}

//发送扑克
VOID CGameClientView::DispatchUserCard(WORD wChairID, BYTE cbCardData)
{
	//效验参数
	ASSERT(wChairID<GAME_PLAYER);
	if (wChairID>=GAME_PLAYER) return;

	//设置变量
	tagSendCardItem SendCardItem;
	SendCardItem.wChairID=wChairID;
	SendCardItem.cbCardData=cbCardData;
	m_SendCardItemArray.Add(SendCardItem);

	//启动动画
	if (m_SendCardItemArray.GetCount()==1) 
	{
		//设置定时器
		SetTimer(IDI_SEND_CARD,SPEED_SEND_CARD,NULL);

		//
		m_nStepCount = SEND_STEP_COUNT;
		m_SendCardCurPos = m_SendCardPos;
		m_nXStep = (m_CardControl[wChairID].GetTailPos().x-m_SendCardPos.x)/m_nStepCount;
		m_nYStep = (m_CardControl[wChairID].GetTailPos().y-m_SendCardPos.y)/m_nStepCount;

		InvalidGameView(0,0,0,0);

		//播放声音
		CGlobalUnits * pGlobalUnits=CGlobalUnits::GetInstance();
		pGlobalUnits->PlayGameSound(AfxGetInstanceHandle(),TEXT("SEND_CARD"));
	}

	return;
}

//游戏底注
bool CGameClientView::SetCellScore(LONG lCellScore)
{
	m_PlayOperate.SetCellScore(lCellScore);
	if (m_lCellScore!=lCellScore)
	{
		//设置变量
		m_lCellScore=lCellScore;

		//更新界面
		InvalidGameView(0,0,0,0);
	}

	return true;
}

//最大下注
bool CGameClientView::SetDrawMaxScore(LONGLONG lDrawMaxScore)
{
	if (m_lDrawMaxScore!=lDrawMaxScore)
	{
		//设置变量
		m_lDrawMaxScore=lDrawMaxScore;

		//更新界面
		InvalidGameView(0,0,0,0);
	}

	return true;
}

//设置积分
bool CGameClientView::SetUserScore(WORD wChairID, LONGLONG lUserScore)
{
	//效验参数
	ASSERT(wChairID<GAME_PLAYER);
	if (wChairID>=GAME_PLAYER) return false;

	//设置积分
	if (m_lUserScore[wChairID]!=lUserScore)
	{
		m_lUserScore[wChairID] = lUserScore;
		m_lUserSaveScore[wChairID] = lUserScore;

		//更新界面
		InvalidGameView(0,0,0,0);
	}

	return true;
}

//设置积分
bool CGameClientView::SetUserSaveScore(WORD wChairID, LONGLONG lUserScore)
{
	//效验参数
	ASSERT(wChairID<GAME_PLAYER);
	if (wChairID>=GAME_PLAYER) return false;

	//设置积分
	if (m_lUserSaveScore[wChairID] != lUserScore)
	{
		m_lUserAddScore[wChairID] = 0;
		m_lUserAddAllScore[wChairID] = lUserScore - m_lUserScore[wChairID];
		m_lUserSaveScore[wChairID] = lUserScore;

		//更新界面
		InvalidGameView(0,0,0,0);
	}

	return true;
}

//设置结束积分
bool CGameClientView::SetOverScore(WORD wChairID, LONGLONG lUserScore)
{
	//效验参数
	ASSERT(wChairID<GAME_PLAYER);
	if (wChairID>=GAME_PLAYER) return false;

	//设置积分
	if (m_lOverScore[wChairID] != lUserScore)
	{
		m_lOverScore[wChairID] = 0;
		m_lOverSaveScore[wChairID] = lUserScore;
	}

	return true;
}

//设置下注
void CGameClientView::SetUserTableScore(WORD wChairID, LONGLONG lTableScore)
{
	//设置数据
	if (wChairID!=INVALID_CHAIR) m_lTableScore[wChairID]=lTableScore;
	else ZeroMemory(m_lTableScore,sizeof(m_lTableScore));

	//更新界面
	InvalidGameView(0,0,0,0);

	return;
}

//设置动作
bool CGameClientView::SetUserAction(WORD wActionUser, BYTE cbUserAction)
{
	//设置动作
	if ((m_wActionUser!=wActionUser)||(m_cbUserAction!=cbUserAction))
	{
		//设置变量
		m_wActionUser=wActionUser;
		m_cbUserAction=cbUserAction;

		//更新界面
		InvalidGameView(0,0,0,0);

		//设置时间
		if (m_wActionUser!=INVALID_CHAIR) SetTimer(IDI_USER_ACTION,3000,NULL);
	}

	return true;
}

//设置积分
bool CGameClientView::SetHistoryScore(WORD wChairID,tagHistoryScore * pHistoryScore)
{
	//效验参数
	ASSERT(wChairID<GAME_PLAYER);
	if (wChairID>=GAME_PLAYER) return false;

	//设置变量
	m_pHistoryScore[wChairID]=pHistoryScore;

	//更新界面
	InvalidGameView(0,0,0,0);

	return true;
}

// 绘画数字
void CGameClientView::DrawNumber( CDC * pDC, CPngImageEx* ImageNumber, TCHAR * szImageNum, LONGLONG lOutNum,INT nXPos, INT nYPos, UINT uFormatX /*= DT_LEFT*/, UINT uFormatY /*= DT_TOP*/)
{
	TCHAR szOutNum[128] = {0};
	_sntprintf(szOutNum,CountArray(szOutNum),TEXT("%I64d"),lOutNum);
	DrawNumber(pDC, ImageNumber, szImageNum, szOutNum, nXPos, nYPos, uFormatX, uFormatY);
}

// 绘画数字
void CGameClientView::DrawNumber(CDC * pDC , CPngImageEx* ImageNumber, TCHAR * szImageNum, TCHAR* szOutNum ,INT nXPos, INT nYPos,  UINT uFormatX /*= DT_LEFT*/, UINT uFormatY /*= DT_TOP*/)
{
	// 加载资源
	INT nNumberHeight=ImageNumber->GetHeight();
	INT nNumberWidth=ImageNumber->GetWidth()/lstrlen(szImageNum);

	if ( uFormatX == DT_CENTER )
	{
		nXPos -= (INT)(((DOUBLE)(lstrlen(szOutNum)) / 2.0) * nNumberWidth);
	}
	else if ( uFormatX == DT_RIGHT )
	{
		nXPos -= lstrlen(szOutNum) * nNumberWidth;
	}

	if ( uFormatY == DT_VCENTER )
	{
		nYPos -= nNumberHeight / 2;
	}
	else if ( uFormatY == DT_BOTTOM )
	{
		nYPos -= nNumberHeight;
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


// 绘画结束分数
void CGameClientView::DrawOverScore( CDC * pDC, LONGLONG lOutNum, INT nXPos, INT nYPos, bool bMy, bool bAdd )
{
	if( lOutNum == 0 )
		return;

	TCHAR szOutNum[128] = {0};
	TCHAR szImageNum[128] = { TEXT("+-0123456789") };
	if(bAdd)
	{
		_sntprintf(szOutNum,CountArray(szOutNum),TEXT("%s%I64d"),lOutNum > 0 ? TEXT("+") : TEXT(""), lOutNum);
	}
	else
	{
		_sntprintf(szOutNum,CountArray(szOutNum),TEXT("%I64d"),lOutNum);
	}

	INT nNumberHeight = m_ImgaeScoreNumber.GetHeight();
	INT nNumberWidth  = m_ImgaeScoreNumber.GetWidth() / 12;
	int nMaxWidth = (INT)(((DOUBLE)(lstrlen(szOutNum))) * nNumberWidth) + 40;

	nXPos -= nMaxWidth / 2;
	nYPos -= m_ImgaeScoreNumber.GetHeight() / 2;

	if(bMy)
	{
		int nBackX = nXPos + m_ImgaeMyScoreBackL.GetWidth();
		int nBackY = nYPos + m_ImgaeScoreNumber.GetHeight() / 2 - m_ImgaeMyScoreBackL.GetHeight() / 2;
		int nBackWidthX = nBackX + (nMaxWidth - m_ImgaeMyScoreBackL.GetWidth() - m_ImgaeMyScoreBackR.GetWidth());

		m_ImgaeMyScoreBackL.DrawImage( pDC, nXPos, nBackY);
		while(nBackX < nBackWidthX)
		{
			int nBackWidthTemp = __min(m_ImgaeMyScoreBackM.GetWidth(), nBackWidthX - nBackX);
			m_ImgaeMyScoreBackM.DrawImage( pDC, nBackX, nBackY, nBackWidthTemp, m_ImgaeMyScoreBackM.GetHeight(), 0, 0, nBackWidthTemp, m_ImgaeMyScoreBackM.GetHeight());
			nBackX += nBackWidthTemp;
		}
		m_ImgaeMyScoreBackR.DrawImage( pDC, nBackX, nBackY);
		nXPos = nXPos + m_ImgaeMyScoreBackL.GetWidth();
	}
	else
	{
		int nBackX = nXPos + m_ImgaeScoreBackL.GetWidth();
		int nBackY = nYPos + m_ImgaeScoreNumber.GetHeight() / 2 - m_ImgaeScoreBackL.GetHeight() / 2;
		int nBackWidthX = nBackX + (nMaxWidth - m_ImgaeScoreBackL.GetWidth() - m_ImgaeScoreBackR.GetWidth());

		m_ImgaeScoreBackL.DrawImage( pDC, nXPos, nBackY);
		while(nBackX < nBackWidthX)
		{
			int nBackWidthTemp = __min(m_ImgaeScoreBackM.GetWidth(), nBackWidthX - nBackX);
			m_ImgaeScoreBackM.DrawImage( pDC, nBackX, nBackY, nBackWidthTemp, m_ImgaeScoreBackM.GetHeight(), 0, 0, nBackWidthTemp, m_ImgaeScoreBackM.GetHeight());
			nBackX += nBackWidthTemp;
		}
		m_ImgaeScoreBackR.DrawImage( pDC, nBackX, nBackY);
		nXPos = nXPos + m_ImgaeScoreBackL.GetWidth();
	}

	for ( INT i = 0; i < lstrlen(szOutNum); ++i )
	{
		for ( INT j = 0; j < lstrlen(szImageNum); ++j )
		{
			if ( szOutNum[i] == szImageNum[j] && szOutNum[i] != '\0' )
			{
				m_ImgaeScoreNumber.DrawImage(pDC, nXPos, nYPos, nNumberWidth, nNumberHeight, j * nNumberWidth, 0, nNumberWidth, nNumberHeight);
				nXPos += nNumberWidth;
				break;
			}
		}
	}
}

// 绘画人物
void CGameClientView::DrawUser(CDC * pDC, WORD wChiarID)
{
	if(m_wTableUser[wChiarID] == INVALID_CHAIR) return;
	CPngImageEx *pImageUser = NULL;
	CPngImageEx *pImageUserEye = NULL;
	switch(wChiarID)
	{
	case 0:
		{
			pImageUser = &m_ImageUser0;
			pImageUserEye = &m_ImageUserEye0;
			break;
		}
	case 1:
		{
			pImageUser = &m_ImageUser1;
			pImageUserEye = &m_ImageUserEye1;
			break;
		}
	case 2:
		{
			pImageUser = &m_ImageUser2;
			pImageUserEye = &m_ImageUserEye2;
			break;
		}
	case 3:
		{
			pImageUser = &m_ImageUser3;
			pImageUserEye = &m_ImageUserEye3;
			break;
		}
	case 4:
		{
			pImageUser = &m_ImageUser4;
			pImageUserEye = &m_ImageUserEye4;
			break;
		}
	}

	if(pImageUser != NULL)
	{
		if(!pImageUser->IsNull())
		{
			pImageUser->DrawImage( pDC, m_ptUser[wChiarID].x, m_ptUser[wChiarID].y, pImageUser->GetWidth() / 6, pImageUser->GetHeight(), m_wTableUser[wChiarID] * pImageUser->GetWidth() / 6, 0);
			if(m_cbUserEye[wChiarID] == 0 && pImageUserEye != NULL)
			{
				if(!pImageUserEye->IsNull())
				{
					pImageUserEye->DrawImage( pDC, m_ptUser[wChiarID].x, m_ptUser[wChiarID].y, pImageUserEye->GetWidth() / 6, pImageUserEye->GetHeight(), m_wTableUser[wChiarID] * pImageUserEye->GetWidth() / 6, 0);
				}
			}
		}
	}
}

//开始按钮
VOID CGameClientView::OnBnClickedStart()
{
	//发送消息
	SendEngineMessage(IDM_START,0,0);

	return;
}

//放弃按钮
VOID CGameClientView::OnBnClickedGiveUp()
{
	//发送消息
	SendEngineMessage(IDM_GIVE_UP,0,0);

	return;
}

//跟注按钮
VOID CGameClientView::OnBnClickedFollow()
{
	//发送消息
	SendEngineMessage(IDM_FOLLOW,0,0);

	return;
}

//加注按钮
VOID CGameClientView::OnBnClickedAddScore()
{
	//发送消息
	m_PlayOperate.ShowOperateAdd(true);
	InvalidGameView(0, 0, 0, 0);

	return;
}

//加注按钮
VOID CGameClientView::OnBnClickedAddScoreOne()
{
	//发送消息
	SendEngineMessage(IDM_FOLLOW,m_lCellScore,0);
}

//加注按钮
VOID CGameClientView::OnBnClickedAddScoreTwo()
{
	//发送消息
	SendEngineMessage(IDM_FOLLOW,m_lCellScore*2,0);
}

//加注按钮
VOID CGameClientView::OnBnClickedAddScoreThree()
{
	//发送消息
	SendEngineMessage(IDM_FOLLOW,m_lCellScore*3,0);
}

//加注按钮
VOID CGameClientView::OnBnClickedAddScoreFour()
{
	//发送消息
	SendEngineMessage(IDM_FOLLOW,m_lCellScore*4,0);
}

//梭哈按钮
VOID CGameClientView::OnBnClickedShowHand()
{
	//发送消息
	SendEngineMessage(IDM_SHOW_HAND,0,0);

	return;
}

//积分按钮
VOID CGameClientView::OnBnClickedOK()
{
	//发送消息
	SendEngineMessage(IDM_FOLLOW,(WPARAM)m_PlayOperate.GetBetCount(),0);
}


void CGameClientView::OnSendMessage() 
{
	CString str, mes;
	GetDlgItem(IDC_CHAT_INPUT)->GetWindowText(str);
	if (str.GetLength() < 1)
	{
		//提示信息
		CInformation Information(this);
		Information.ShowMessageBox(TEXT("发送消息不能为空！"),MB_ICONWARNING|MB_OK);
		return;
	}

	//查询接口
	IClientKernel * pIClientKernel;
	CGlobalUnits * pGlobalUnits = CGlobalUnits::GetInstance();
	ASSERT(pGlobalUnits->QueryGlobalModule(MODULE_CLIENT_KERNEL,IID_IClientKernel,VER_IClientKernel)!=NULL);
	pIClientKernel=(IClientKernel *)pGlobalUnits->QueryGlobalModule(MODULE_CLIENT_KERNEL,IID_IClientKernel,VER_IClientKernel);

	//长度比较
	if(str.GetLength() >= (LEN_USER_CHAT/2))
	{
		m_ChatMessage.InsertSystemString(TEXT("抱歉，您输入的聊天内容过长，请缩短后再发送！"));
		return;
	}

	//变量定义
	BYTE cbMemberOrder=pIClientKernel->GetMeUserItem()->GetMemberOrder();
	BYTE cbMasterOrder=pIClientKernel->GetMeUserItem()->GetMasterOrder();

	//属性定义
	tagUserAttribute * pUserAttribute=pIClientKernel->GetUserAttribute();
	tagServerAttribute * pServerAttribute=pIClientKernel->GetServerAttribute();

	//房间配置
	if (CServerRule::IsForfendGameChat(pServerAttribute->dwServerRule)&&(cbMasterOrder==0))
	{
		//插入消息
		m_ChatMessage.InsertSystemString(TEXT("抱歉，当前此游戏房间禁止用户房间聊天！"));

		return;
	}

	//权限判断
	if (CUserRight::CanGameChat(pUserAttribute->dwUserRight)==false)
	{
		//插入消息
		m_ChatMessage.InsertSystemString(TEXT("抱歉，您没有游戏发言的权限，若需要帮助，请联系游戏客服咨询！"));

		return;
	}

	//速度判断
	DWORD dwCurrentTime=(DWORD)time(NULL);
	if ((cbMasterOrder==0)&&((dwCurrentTime-m_dwChatTime)<=TIME_USER_CHAT))
	{
		//插入消息
		m_ChatMessage.InsertSystemString(TEXT("您的说话速度太快了，请坐下来喝杯茶休息下吧！"));

		return;
	}

	//设置变量
	m_dwChatTime=dwCurrentTime;

	
	//原始消息
	if (!pIClientKernel->SendUserChatMessage(0, str, RGB(193,167,108)))
	{
		CString strDescribe;
		strDescribe.Format(TEXT("“%s”发送失败"),str);
		m_ChatMessage.InsertString(strDescribe,COLOR_EVENT);
		return;
	}

	GetDlgItem(IDC_CHAT_INPUT)->SetWindowText(TEXT(""));
	GetDlgItem(IDC_CHAT_INPUT)->SetFocus();

	if(m_bShowChatMessage == false)
	{
		OnHideChatMessage();
	}
	return;
}

//时间消息
VOID CGameClientView::OnTimer(UINT nIDEvent)
{
	//消息处理
	switch (nIDEvent)
	{
	case IDI_HIDE_CARD:		//隐藏扑克
		{
			//删除时间
			KillTimer(IDI_HIDE_CARD);

			//隐藏判断
			IClientUserItem * pIClientUserItem=GetClientUserItem(MYSELF_VIEW_ID);
			if( pIClientUserItem == NULL )
				return;

			if ( m_lOverScore[MYSELF_VIEW_ID] == 0 && m_CardControl[MYSELF_VIEW_ID].GetDisplayHead() && m_CardControl[MYSELF_VIEW_ID].GetPositively() && pIClientUserItem->GetUserStatus() == US_PLAYING )
			{
				//设置控件
				m_CardControl[MYSELF_VIEW_ID].SetDisplayHead(false);

				//更新界面
				InvalidGameView(0,0,0,0);
			}

			return;
		}
	case IDI_SEND_CARD:		//发牌动画
		{
			//设置位置
			m_SendCardCurPos.x += m_nXStep;
			m_SendCardCurPos.y += m_nYStep;

			//更新界面
			InvalidGameView(0,0,0,0);

			//停止判断
			if ( --m_nStepCount == 0 )
			{
				//获取扑克
				BYTE cbCardData[MAX_COUNT];
				WORD wChairID=m_SendCardItemArray[0].wChairID;
				BYTE cbCardCount=(BYTE)m_CardControl[wChairID].GetCardData(cbCardData,CountArray(cbCardData));

				//设置扑克
				if (cbCardCount<MAX_COUNT)
				{
					cbCardData[cbCardCount++]=m_SendCardItemArray[0].cbCardData;
					m_CardControl[wChairID].SetCardData(cbCardData,cbCardCount);
				}

				//删除动画
				m_SendCardItemArray.RemoveAt(0);

				//继续动画
				if (m_SendCardItemArray.GetCount()>0)
				{
					//
					wChairID = m_SendCardItemArray[0].wChairID;
					m_nStepCount = SEND_STEP_COUNT;
					m_SendCardCurPos = m_SendCardPos;
					m_nXStep = (m_CardControl[wChairID].GetTailPos().x-m_SendCardPos.x)/m_nStepCount;
					m_nYStep = (m_CardControl[wChairID].GetTailPos().y-m_SendCardPos.y)/m_nStepCount;

					//播放声音
					CGlobalUnits * pGlobalUnits=CGlobalUnits::GetInstance();
					pGlobalUnits->PlayGameSound(AfxGetInstanceHandle(),TEXT("SEND_CARD"));

					return;
				}
				else 
				{
					//完成处理
					KillTimer(IDI_SEND_CARD);
					//发送消息
					SendEngineMessage(IDM_SEND_CARD_FINISH,0,0);

					return;
				}
			}

			return;
		}
	case IDI_USER_ACTION:	//用户动作
		{
			//设置变量
			m_cbUserAction=0;
			m_wActionUser=INVALID_CHAIR;

			//更新界面
			InvalidGameView(0,0,0,0);

			return;
		}
	case IDI_MOVE_EYE:	//闪瞎眼睛
		{
			CRect rcEye;
			rcEye.SetRectEmpty();

			for(int i = 0; i < GAME_PLAYER; i++)
			{
				if(m_cbUserEye[i] == 0 && m_wTableUser[i] != INVALID_CHAIR)
				{
					rcEye |= CRect(m_ptUser[i].x, m_ptUser[i].y, m_ptUser[i].x + 242, m_ptUser[i].y + 287);
					m_cbUserEye[i] = 3 + rand() % 4;
				}
			}

			static int nTime = 0;
			if(++nTime == 3)
			{
				nTime = 0;
				for(int i = 0; i < GAME_PLAYER; i++)	
				{
					if(--m_cbUserEye[i] == 0 && m_wTableUser[i] != INVALID_CHAIR)
					{
						rcEye |= CRect(m_ptUser[i].x, m_ptUser[i].y, m_ptUser[i].x + 242, m_ptUser[i].y + 287);
					}
				}
			}

			//更新界面
			if(!rcEye.IsRectEmpty())
			{
				InvalidGameView(rcEye.left, rcEye.top, rcEye.right, rcEye.bottom);
			}

			return;
		}	
	case IDI_MOVE_CHIP:
		{
			BOOL bUpdateGameView = FALSE;
			for ( int i = 0 ; i < m_ArrayBetChip.GetCount(); ++i )
			{
				if( m_ArrayBetChip[i]->Move() )
				{
					bUpdateGameView = TRUE;
				}
			}

			int nIndex = 0;
			while ( nIndex < m_ArrayPlayChip.GetCount())
			{
				if( m_ArrayPlayChip[nIndex]->Move() )
				{
					bUpdateGameView = TRUE;
					nIndex++;
				}
				else
				{
					bUpdateGameView = TRUE;

					// 变量定义
					LONGLONG lScoreArray[] = { 1L, 5L, 10L, 50L, 100L, 500L, 1000L, 5000L, 10000L, 50000L, 100000L, 500000L, 1000000L, 5000000L };

					// 添加分数
					m_lPalyBetChip[m_ArrayPlayChip[nIndex]->wChipChairID] += lScoreArray[m_ArrayPlayChip[nIndex]->wChipIndex];

					// 删除筹码
					SafeDelete(m_ArrayPlayChip[nIndex]);
					m_ArrayPlayChip.RemoveAt(nIndex);

				}
			}

			// 添加回收
			if ( m_ArrayOverChip.GetCount() )
			{
				// 回收位置
				WORD   wChairID = m_ArrayOverChip[0]->wChipChairID;
				CPoint ptBenchmarkPos = m_ptAvatar[wChairID];
				ptBenchmarkPos.Offset( 25, 25 );

				// 添加回收
				if ( m_ArrayBetChip.GetCount() )
				{
					INT_PTR nMaxIndex = m_ArrayBetChip.GetCount()/5;
					if ( nMaxIndex <= 0 )
						nMaxIndex = 1;

					while ( nMaxIndex > 0 )
					{
						stuMoveChip* pMoveChip = new stuMoveChip(m_ArrayBetChip[0]->wChipIndex, wChairID, 10, m_ArrayBetChip[0]->ptCurrentPos, ptBenchmarkPos, FALSE);
						m_ArrayOverChip.Add(pMoveChip);
						SafeDelete(m_ArrayBetChip[0]);
						m_ArrayBetChip.RemoveAt(0);
						nMaxIndex--;
					}

				}

				// 添加回收
				if ( m_ArrayPlayChip.GetCount() )
				{
					INT_PTR nMaxIndex = m_ArrayPlayChip.GetCount()/5;
					if ( nMaxIndex <= 0 )
						nMaxIndex = 1;

					while ( nMaxIndex > 0 )
					{
						stuMoveChip* pMoveChip = new stuMoveChip(m_ArrayPlayChip[0]->wChipIndex, wChairID, 10, m_ArrayPlayChip[0]->ptCurrentPos, ptBenchmarkPos, FALSE);
						m_ArrayOverChip.Add(pMoveChip);
						SafeDelete(m_ArrayPlayChip[0]);
						m_ArrayPlayChip.RemoveAt(0);
						nMaxIndex--;
					}
				}
				

				// 添加回收
				LONGLONG lScoreArray[] = { 1L, 5L, 10L, 50L, 100L, 500L, 1000L, 5000L, 10000L, 50000L, 100000L, 500000L, 1000000L, 5000000L };
			
				for ( int nSite = 0; nSite < GAME_PLAYER; ++nSite )
				{
					int nChipCount = 0;
					while ( m_lPalyBetChip[nSite] > 0 )
					{
						for( int j = CountArray(lScoreArray) - 1; j >=0; j-- )
						{
							if ( m_lPalyBetChip[nSite] >= lScoreArray[j] )
							{	
								CPoint ptBegin(m_ptPalyBetChip[nSite]);
								CPoint ptEnd(ptBenchmarkPos);
								ptEnd.y -= nChipCount * 5;
								ptBegin.y -= nChipCount * 5;
								stuMoveChip* pMoveChip = new stuMoveChip(j, wChairID, 10, ptBegin, ptEnd, FALSE);
								if ( nChipCount >= PILING_HEIGHT )
									pMoveChip->SetShow(FALSE);
								
								m_ArrayOverChip.Add(pMoveChip);
								m_lPalyBetChip[nSite] -= lScoreArray[j];
								nChipCount++;
								break;
							}
						}
					}
				}
			}

			// 移动回收动画
			nIndex = 0;
			while ( nIndex < m_ArrayOverChip.GetCount())
			{
				if( m_ArrayOverChip[nIndex]->Move() )
				{
					bUpdateGameView = TRUE;
					nIndex++;
				}
				else
				{
					// 变量定义
					WORD wChipChairID = m_ArrayOverChip[nIndex]->wChipChairID;
					LONGLONG lScoreArray[] = { 1L, 5L, 10L, 50L, 100L, 500L, 1000L, 5000L, 10000L, 50000L, 100000L, 500000L, 1000000L, 5000000L };

					// 添加分数
					m_lUserScore[wChipChairID] += lScoreArray[m_ArrayOverChip[nIndex]->wChipIndex];
					m_lUserAddScore[wChipChairID] += lScoreArray[m_ArrayOverChip[nIndex]->wChipIndex];

					if ( m_lUserAddScore[wChipChairID] > m_lUserAddAllScore[wChipChairID] )
						m_lUserAddScore[wChipChairID] = m_lUserAddAllScore[wChipChairID];


					for ( int i = 0; i < GAME_PLAYER; ++i )
					{
						if ( m_lUserAddAllScore[i] == 0 || m_lOverSaveScore[i] == 0 || m_lUserAddScore[wChipChairID] == 0 )
							continue;

						m_lOverScore[i] = (LONGLONG)( m_lOverSaveScore[i] * m_lUserAddScore[wChipChairID] / m_lUserAddAllScore[wChipChairID]);
					}

					// 删除筹码
					SafeDelete(m_ArrayOverChip[nIndex]);
					m_ArrayOverChip.RemoveAt(nIndex);

					bUpdateGameView = TRUE;

					if ( m_ArrayOverChip.GetCount() == 0 )
					{
						memcpy(m_lUserScore, m_lUserSaveScore, sizeof(m_lUserScore));
						memcpy(m_lOverScore, m_lOverSaveScore, sizeof(m_lOverScore));
					}
				}
			}


			//更新界面
			if ( bUpdateGameView )
			{
				InvalidGameView( 0,0,0,0 );
			}
			
			return;
		}
	}

	__super::OnTimer(nIDEvent);
}

//创建函数
INT CGameClientView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct)==-1) return -1;

	//创建控件
	CRect rcCreate(0,0,0,0);
	m_PlayOperate.Initialization(this);

	//资源变量
	HINSTANCE hResInstance=AfxGetInstanceHandle();

	//控制按钮
	m_btStart.Create(NULL,WS_CHILD,rcCreate,this,IDC_START);
	m_btStart.SetButtonImage(IDB_BT_START,hResInstance,false,false);

	//最小按钮
	m_btMinButton.Create(NULL, WS_CHILD, rcCreate, this, IDC_MIN_BUTTON);
	m_btMinButton.SetButtonImage(IDB_BT_MIN_BUTTON, AfxGetInstanceHandle(), false, false);

	m_btSound.Create(NULL, WS_CHILD, rcCreate, this, IDC_SOUND);
	m_btSound.SetButtonImage(IDB_BT_SOUND_OPEN, AfxGetInstanceHandle(), false, false);

	//关闭按钮
	m_btCloseButton.Create(NULL, WS_CHILD, rcCreate, this, IDC_CLOSE_BUTTON);
	m_btCloseButton.SetButtonImage(IDB_BT_CLOSE_BUTTON, AfxGetInstanceHandle(), false, false);

	//设置按钮
	m_btOption.Create(NULL, WS_CHILD, rcCreate, this, IDC_OPTION);
	m_btOption.SetButtonImage(IDB_BT_OPTION, AfxGetInstanceHandle(), false, false);

	//建立提示
	m_ToolTipCtrl.Create(this);
	m_ToolTipCtrl.Activate(TRUE);
	m_ToolTipCtrl.AddTool(&m_btStart,TEXT("开始游戏"));
	m_ToolTipCtrl.AddTool(&m_PlayOperate.m_btFollow,TEXT("跟注"));
	m_ToolTipCtrl.AddTool(&m_PlayOperate.m_btGiveUp,TEXT("放弃"));
	m_ToolTipCtrl.AddTool(&m_PlayOperate.m_btAddScore,TEXT("加注"));
	m_ToolTipCtrl.AddTool(&m_PlayOperate.m_btShowHand,TEXT("梭哈"));

	m_DFontEx.CreateFont(this, TEXT("宋体"), 12, 400 );

	//发送按钮
	m_btSendMessage.Create(NULL,WS_CHILD|WS_VISIBLE, rcCreate, this, IDC_SEND_MESSAGE);
	m_btSendMessage.SetButtonImage(IDB_BT_SEND_MESSAGE, AfxGetInstanceHandle(), false, false);

	//查看聊天
	m_btHistoryChat.Create(NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_HISTORY_CHAT);
	m_btHistoryChat.SetButtonImage(IDB_BT_HIDE_CHAT, AfxGetInstanceHandle(), false, true);

	//聊天控件
	m_ChatInput.Create(ES_AUTOHSCROLL|WS_CHILD|WS_VISIBLE|WS_TABSTOP|WS_BORDER, rcCreate, this, IDC_CHAT_INPUT);
	//m_ChatDisplay.Create(ES_AUTOVSCROLL|ES_WANTRETURN|ES_MULTILINE|WS_CHILD|WS_VISIBLE|WS_TABSTOP|WS_VSCROLL, rcCreate, this, IDC_CHAT_DISPLAY);
	m_ChatMessage.Create(ES_AUTOVSCROLL|ES_WANTRETURN|ES_MULTILINE|WS_CHILD|WS_VISIBLE|WS_TABSTOP|WS_VSCROLL, rcCreate, this, IDC_CHAT_DISPLAY);
	m_ChatMessage.SetBackgroundColor(false,RGB(17,9,8));
	m_ChatMessage.SetExpressionManager(CExpressionManager::GetInstance(),RGB(17,9,8));
	m_ChatMessage.SetReadOnly(TRUE);

	IClientKernel * m_pIClientKernel;
	CGlobalUnits * pGlobalUnits=CGlobalUnits::GetInstance();
	//查询接口
	ASSERT(pGlobalUnits->QueryGlobalModule(MODULE_CLIENT_KERNEL,IID_IClientKernel,VER_IClientKernel)!=NULL);
	m_pIClientKernel=(IClientKernel *)pGlobalUnits->QueryGlobalModule(MODULE_CLIENT_KERNEL,IID_IClientKernel,VER_IClientKernel);
	m_pIClientKernel->SetStringMessage(QUERY_OBJECT_INTERFACE(m_ChatMessage,IStringMessage));

	//控件属性
	LOGFONT lf;
	memset(&lf, 0, sizeof(LOGFONT));      
	lf.lfHeight = 15;
	wcscpy(lf.lfFaceName, TEXT("Arial"));
	m_ChatInput.SetLimitText(30);
// 	m_ChatDisplay.SetTextFont(lf);
// 	m_ChatDisplay.SetReadOnly(TRUE);
// 	m_ChatDisplay.SetTextColor(RGB(193,167,108));
	m_brush.CreateSolidBrush(RGB(255,0,255));
	CFont m_font;
	m_font.CreateFontIndirect(&lf);
	//绑定控件
	m_ChatMessage.SetFont(&m_font);

	//播放声音
	//CGlobalUnits *pGlobalUnits=CGlobalUnits::GetInstance();
	pGlobalUnits->m_bAllowSound = true;
	pGlobalUnits->m_bAllowBackGroundSound = true;
	
	UpdateSound();

	// 开启筹码移动
	SetTimer(IDI_MOVE_CHIP, 40, NULL);
	SetTimer(IDI_MOVE_EYE, 333, NULL);

	return 0;
}

//鼠标消息
VOID CGameClientView::OnLButtonDown(UINT nFlags, CPoint Point)
{
	__super::OnLButtonDown(nFlags, Point);

	//for ( int i = 0 ; i < GAME_PLAYER; ++i )
	//{
	//	BYTE cbCrad[5] = { 0x09,0x19,0x29,0x0C,0x0A };
	//	m_CardControl[i].SetCardData( cbCrad, 5);
	//}
	//
	//InvalidGameView(0,0,0,0);

	//获取光标
	CPoint MousePoint;
	GetCursorPos(&MousePoint);
	ScreenToClient(&MousePoint);

	//扑克点击
	if (m_CardControl[MYSELF_VIEW_ID].OnEventLeftMouseDown(MousePoint)==true)
	{
		//更新界面
		InvalidGameView(0,0,0,0);

		//设置时间
		if (m_CardControl[MYSELF_VIEW_ID].GetDisplayHead()==true)
		{
			SetTimer(IDI_HIDE_CARD,2000,NULL);
		}
		else KillTimer(IDI_HIDE_CARD);
		return;
	}

	LPARAM lParam = MAKEWPARAM(Point.x,Point.y);
	AfxGetMainWnd()->PostMessage(WM_LBUTTONDOWN,0,lParam);

	return;
}

//光标消息
BOOL CGameClientView::OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage)
{
	//获取光标
	CPoint MousePoint;
	GetCursorPos(&MousePoint);
	ScreenToClient(&MousePoint);

	//扑克测试
	if (m_CardControl[MYSELF_VIEW_ID].OnEventSetCursor(MousePoint)==true) return TRUE;

	return __super::OnSetCursor(pWnd,nHitTest,uMessage);
}

//绘画字符
bool CGameClientView::DrawTextStringEx(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, CRect rcRect, INT nDrawFormat)
{
	//变量定义
	INT nXExcursion[8]={1,1,1,0,-1,-1,-1,0};
	INT nYExcursion[8]={-1,0,1,1,1,0,-1,-1};

	//保存格式
	UINT nTextAlign=pDC->SetTextAlign(TA_TOP|TA_LEFT);

	//绘画边框
	for (INT i=0;i<CountArray(nXExcursion);i++)
	{
		//计算位置
		CRect rcFrame;
		rcFrame.top=rcRect.top+nYExcursion[i];
		rcFrame.left=rcRect.left+nXExcursion[i];
		rcFrame.right=rcRect.right+nXExcursion[i];
		rcFrame.bottom=rcRect.bottom+nYExcursion[i];

		//绘画字符
		m_DFontEx.DrawText(pDC,pszString,&rcFrame,crFrame,nDrawFormat);
	}

	//绘画字符
	m_DFontEx.DrawText(pDC,pszString,&rcRect,crText,nDrawFormat);

	//还原格式
	pDC->SetTextAlign(nTextAlign);

	return true;
}

// 绘画字符
bool CGameClientView::DrawTextStringEx( CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, INT nXPos, INT nYPos )
{
	//变量定义
	INT nXExcursion[8]={1,1,1,0,-1,-1,-1,0};
	INT nYExcursion[8]={-1,0,1,1,1,0,-1,-1};

	//保存格式
	UINT nTextAlign=pDC->GetTextAlign();
	INT nDrawFormat=0;

	if ( (nTextAlign&TA_CENTER) == TA_CENTER)
		nDrawFormat |= DT_CENTER;
	else if( (nTextAlign&TA_RIGHT) == TA_RIGHT )
		nDrawFormat |= DT_RIGHT;
	else
		nDrawFormat |= DT_LEFT;

	if( (nTextAlign&TA_BOTTOM) == TA_BOTTOM )
		nDrawFormat |= DT_BOTTOM;
	else 
		nDrawFormat |= DT_TOP;

	//绘画边框
	for (INT i=0;i<CountArray(nXExcursion);i++)
	{
		//计算位置
		int nYPosT=nYPos+nYExcursion[i];
		int nXPosT=nXPos+nXExcursion[i];

		//绘画字符
		m_DFontEx.DrawText(pDC,pszString,nXPosT,nYPosT,crFrame,nDrawFormat);
	}

	//绘画字符
	m_DFontEx.DrawText(pDC,pszString,nXPos,nYPos,crText,nDrawFormat);

	return true;
}

// 添加桌面下注
void CGameClientView::AddDeskChip( LONGLONG lScore, WORD wChairID )
{

	// 变量定义
	LONGLONG lScoreArray[] = { 1L, 5L, 10L, 50L, 100L, 500L, 1000L, 5000L, 10000L, 50000L, 100000L, 500000L, 1000000L, 5000000L };
	UINT	 nChipCount    = 0;

	// 删除下注
	if( wChairID <= GAME_PLAYER )
	{
		int nIndex = 0;
		while( nIndex < m_ArrayPlayChip.GetCount() )
		{	
			if( m_ArrayPlayChip[nIndex]->wChipChairID == wChairID )
			{
				WORD	wChipIndex; 
				UINT	nAllTime; 
				DPOINT	ptBeginPos; 
				DPOINT	ptEndPos;

				wChipIndex = m_ArrayPlayChip[nIndex]->wChipIndex;
				nAllTime = 8;
				CPoint ptBenchmarkPos = m_ptPalyBetChip[wChairID];
				ptBeginPos.SetPoint( m_ArrayPlayChip[nIndex]->ptCurrentPos.x, m_ArrayPlayChip[nIndex]->ptCurrentPos.y);
				ptEndPos.SetPoint( m_rectBetChip.left + rand()%m_rectBetChip.Width(), m_rectBetChip.top + rand()%m_rectBetChip.Height());
				stuMoveChip* pMoveChip = new stuMoveChip(wChipIndex, wChairID, nAllTime, ptBeginPos, ptEndPos, TRUE);
				m_ArrayBetChip.Add(pMoveChip);
				lScore -= lScoreArray[m_ArrayPlayChip[nIndex]->wChipIndex];
				SafeDelete(m_ArrayPlayChip[nIndex]);
				m_ArrayPlayChip.RemoveAt(nIndex);
				continue;
			}
			nIndex++;
		}
	}

	// 获取筹码
	nChipCount    = 0;
	while ( lScore > 0 )
	{
		for( int i = CountArray(lScoreArray) - 1; i >=0; i-- )
		{
			if ( lScore >= lScoreArray[i] )
			{
				WORD	wChipIndex; 
				UINT	nAllTime; 
				DPOINT	ptBeginPos; 
				DPOINT	ptEndPos;

				if ( wChairID >= GAME_PLAYER )
				{
					wChipIndex = i;
					nAllTime = 0;
					ptBeginPos.SetPoint( m_rectBetChip.left + rand()%m_rectBetChip.Width(), m_rectBetChip.top + rand()%m_rectBetChip.Height());
					ptEndPos = ptBeginPos;
				}
				else
				{
					wChipIndex = i;
					nAllTime = 8;
					CPoint ptBenchmarkPos = m_ptPalyBetChip[wChairID];
					ptBeginPos.SetPoint( ptBenchmarkPos.x, ptBenchmarkPos.y - nChipCount * 5);
					ptEndPos.SetPoint( m_rectBetChip.left + rand()%m_rectBetChip.Width(), m_rectBetChip.top + rand()%m_rectBetChip.Height());
				}
				stuMoveChip* pMoveChip = new stuMoveChip(wChipIndex, wChairID, nAllTime, ptBeginPos, ptEndPos, TRUE);
				m_ArrayBetChip.Add(pMoveChip);
				lScore -= lScoreArray[i];
				if ( nChipCount < PILING_HEIGHT )
					nChipCount++;

				break;
			}
		}
	}
}


// 添加玩家下注
void CGameClientView::AddPlayChip( LONGLONG lScore, WORD wChairID )
{
	if( wChairID >= GAME_PLAYER )
		return;

	// 变量定义
	LONGLONG lScoreArray[] = { 1L, 5L, 10L, 50L, 100L, 500L, 1000L, 5000L, 10000L, 50000L, 100000L, 500000L, 1000000L, 5000000L };

	// 获取筹码
	while ( lScore > 0 )
	{
		for( int i = CountArray(lScoreArray) - 1; i >=0; i-- )
		{
			if ( lScore >= lScoreArray[i] )
			{
				WORD	wChipIndex; 
				UINT	nAllTime; 
				DPOINT	ptBeginPos; 
				DPOINT	ptEndPos;

				wChipIndex = i;
				nAllTime = 10;
				CPoint ptBenchmarkPos = m_ptAvatar[wChairID];
				ptBenchmarkPos.Offset( 25, 25 );
				ptBeginPos.SetPoint( ptBenchmarkPos.x, ptBenchmarkPos.y );
				ptEndPos.SetPoint( m_rectBetChip.left + rand()%m_rectBetChip.Width(), m_rectBetChip.top + rand()%m_rectBetChip.Height());

				stuMoveChip* pMoveChip = new stuMoveChip(wChipIndex, wChairID, nAllTime, ptBeginPos, ptEndPos, TRUE);
				m_ArrayBetChip.Add(pMoveChip);
				lScore -= lScoreArray[i];
				
				break;
			}
		}
	}
	
}

// 添加玩家加注
void CGameClientView::AddPlayBetChip( LONGLONG lScore, WORD wChairID )
{
	if( wChairID >= GAME_PLAYER )
		return;

	if ( lScore == 0 )
	{
		m_lPalyBetChip[wChairID] = lScore;
		return;
	}

	lScore -= m_lPalyBetChip[wChairID];

	// 变量定义
	LONGLONG lScoreArray[] = { 1L, 5L, 10L, 50L, 100L, 500L, 1000L, 5000L, 10000L, 50000L, 100000L, 500000L, 1000000L, 5000000L };
	UINT	 nChipCount    = 0;

	// 获取筹码
	while ( lScore > 0 )
	{
		for( int i = CountArray(lScoreArray) - 1; i >=0; i-- )
		{
			if ( lScore >= lScoreArray[i] )
			{
				WORD	wChipIndex; 
				UINT	nAllTime; 
				DPOINT	ptBeginPos; 
				DPOINT	ptEndPos;

				wChipIndex = i;
				nAllTime = 15;
				CPoint ptBenchmarkPos = m_ptAvatar[wChairID];
				ptBenchmarkPos.Offset( 25, 25 );
				ptBeginPos.SetPoint( ptBenchmarkPos.x, ptBenchmarkPos.y - nChipCount * 5);
				ptEndPos.SetPoint( m_ptPalyBetChip[wChairID].x, m_ptPalyBetChip[wChairID].y - nChipCount * 5 +100);

				stuMoveChip* pMoveChip = new stuMoveChip(wChipIndex, wChairID, nAllTime, ptBeginPos, ptEndPos, FALSE);
				m_ArrayPlayChip.Add(pMoveChip);
				lScore -= lScoreArray[i];

				if ( nChipCount < PILING_HEIGHT )
					nChipCount++;
				break;
			}
		}
	}
}

// 筹码回收
void CGameClientView::ChioRecovery( WORD wChairID )
{
	// 回收位置
	CPoint ptBenchmarkPos = m_ptAvatar[wChairID];
	ptBenchmarkPos.Offset( 25, 25 );

	// 添加回收
	if ( m_ArrayBetChip.GetCount() )
	{
		stuMoveChip* pMoveChip = new stuMoveChip(m_ArrayBetChip[0]->wChipIndex, wChairID, 10, m_ArrayBetChip[0]->ptCurrentPos, ptBenchmarkPos, FALSE);
		m_ArrayOverChip.Add(pMoveChip);
		SafeDelete(m_ArrayBetChip[0]);
		m_ArrayBetChip.RemoveAt(0);
	}
}


//系统消息
void CGameClientView::InsertSystemMessage(LPCTSTR pszString)
{
	CString strMessage=TEXT("[系统消息]:");
	InsertMessage(strMessage,RGB(255,0,0));
	InsertMessage(pszString);
	InsertMessage(TEXT("\r\n"));

	return;
}

//聊天消息
void CGameClientView::InsertChatMessage(LPCTSTR pszString)
{
	CString  strInit=pszString;
	CString  FileLeft,FileRight;
	int index; 
	index		= strInit.Find(':');
	if (index<0)
	{
		return;
	}

	FileLeft	= strInit.Left(index+1);
	FileRight   = strInit.Right(strInit.GetLength()-index-1);

	InsertMessage(FileLeft,RGB(0,255,0));
	InsertMessage(FileRight);
	InsertMessage(TEXT("\r\n"));

	return;
}

//常规消息
void CGameClientView::InsertNormalMessage(LPCTSTR pszString)
{
	InsertMessage(pszString);
	InsertMessage(TEXT("\r\n"));

	return;
}

//聊天消息
void CGameClientView::InsertAllChatMessage(LPCTSTR pszString, int nMessageType) 
{
	if(nMessageType==1)
	{
		InsertSystemMessage(pszString);
	}
	else if(nMessageType==2)
	{
		InsertChatMessage(pszString);
	}
	else
	{
		InsertNormalMessage(pszString);
	}

	//m_ChatDisplay.PostMessage(WM_VSCROLL, SB_BOTTOM, 0);

	return;
}

//消息函数
void CGameClientView::InsertMessage(LPCTSTR pszString, COLORREF color)
{
	CString mes;
	CEdit *pEditCtrl = (CEdit *)GetDlgItem(IDC_CHAT_DISPLAY);
	pEditCtrl->GetWindowText(mes);

	CHARFORMAT cf;
	cf.cbSize = sizeof(cf); 
	//m_ChatDisplay.GetSelectionCharFormat(cf); 
	cf.dwMask = CFM_SIZE|CFM_COLOR; 
	cf.dwEffects = CFE_PROTECTED;
	cf.crTextColor = color; 
	//m_ChatDisplay.SetSelectionCharFormat(cf); 

	int nlength = mes.GetLength();
	pEditCtrl->SetSel(nlength,nlength);
	pEditCtrl->ReplaceSel(pszString);

	//int nLine = pEditCtrl->GetLineCount();
	//pEditCtrl->LineScroll(pEditCtrl->GetLineCount()); 

	return;
}

//设置人物
void CGameClientView::SetUserImage(WORD wChairID, WORD wUserID, bool bBoy)
{
	CPngImageEx *pImageUser = NULL;
	CPngImageEx *pImageUserEye = NULL;
	switch(wChairID)
	{
	case 0:
		{
			pImageUser = &m_ImageUser0;
			pImageUserEye = &m_ImageUserEye0;
			break;
		}
	case 1:
		{
			pImageUser = &m_ImageUser1;
			pImageUserEye = &m_ImageUserEye1;
			break;
		}
	case 2:
		{
			pImageUser = &m_ImageUser2;
			pImageUserEye = &m_ImageUserEye2;
			break;
		}
	case 3:
		{
			pImageUser = &m_ImageUser3;
			pImageUserEye = &m_ImageUserEye3;
			break;
		}
	case 4:
		{
			pImageUser = &m_ImageUser4;
			pImageUserEye = &m_ImageUserEye4;
			break;
		}
	}


	if(pImageUser != NULL)
	{		
		if(!pImageUser->IsNull())
		{
			pImageUser->DestroyImage();			
		}

		if(!pImageUserEye->IsNull())
		{
			pImageUserEye->DestroyImage();		
		}
		if(wUserID != INVALID_CHAIR)
		{
			CString strImage;
			strImage.Format(TEXT("USER_%d_%s"), wChairID, bBoy ? TEXT("BOY") : TEXT("GIRL"));
			pImageUser->LoadImage(this, AfxGetInstanceHandle(),strImage);

			strImage.Format(TEXT("USER_%d_%s_EYE"), wChairID, bBoy ? TEXT("BOY") : TEXT("GIRL"));
			pImageUserEye->LoadImage(this, AfxGetInstanceHandle(),strImage);
		}
		m_wTableUser[wChairID] = wUserID;
		m_cbUserEye[wChairID] = 2 + rand() % 4;
	}	
}

// 消息函数
BOOL CGameClientView::OnCommand(WPARAM wParam, LPARAM lParam)
{
	switch (LOWORD(wParam))
	{
	case IDC_MIN_BUTTON:			//最小按钮
		{
			AfxGetMainWnd()->PostMessage(WM_COMMAND, 100, 0);

			return TRUE;
		}
	case IDC_CLOSE_BUTTON:			//关闭按钮
		{
			SendEngineMessage(IDM_GAME_CLOSE,0,0);
			//AfxGetMainWnd()->PostMessage(WM_COMMAND, 101, 0);
			return TRUE;
		}
	case IDC_SEND_MESSAGE:
		{
			OnSendMessage();

			//更新界面
			InvalidGameView(0,0,0,0);

			return TRUE;
		}
	case IDC_HISTORY_CHAT:
		{
			OnHideChatMessage();

			//更新界面
			InvalidGameView(0,0,0,0);

			return TRUE;
		}
	case IDC_SOUND:
		{
			AfxGetMainWnd()->SendMessage(IDC_MUTE);
			//播放声音
			CGlobalUnits *pGlobalUnits=CGlobalUnits::GetInstance();
			pGlobalUnits->m_bMuteStatuts = !pGlobalUnits->m_bMuteStatuts;
			//状态处理
			if(pGlobalUnits->m_bMuteStatuts)
			{
				pGlobalUnits->StopSound();
			}
			UpdateSound();

			return TRUE;
		}
	case IDC_OPTION:
		{
// 			CDlgGameOption DlgGameOption;
// 			if (DlgGameOption.CreateGameOption(NULL,0)==true)
// 			{
// 				return TRUE;
// 			}
// 			return TRUE;
		}
	}

	return CGameFrameView::OnCommand(wParam, lParam);
}

//查看聊天
void CGameClientView::OnHideChatMessage()
{
	if (m_bShowChatMessage == true)
	{
		m_bShowChatMessage = false;
		//m_ChatDisplay.ShowWindow(SW_HIDE);
		m_ChatMessage.ShowWindow(SW_HIDE);
		//m_ChatDisplay.SetHideWindow(true);
		m_btHistoryChat.SetButtonImage(IDB_BT_SHOW_CHAT, AfxGetInstanceHandle(), false, true);
	}
	else
	{
		m_bShowChatMessage = true;
		//m_ChatDisplay.ShowWindow(SW_SHOW);
		m_ChatMessage.ShowWindow(SW_SHOW);
		//m_ChatDisplay.SetHideWindow(false);
		m_btHistoryChat.SetButtonImage(IDB_BT_HIDE_CHAT, AfxGetInstanceHandle(), false, true);
	}

	//更新界面
	InvalidGameView(0,0,0,0);

	return;
}

// 添加逗号
CString CGameClientView::AddComma( LONGLONG lScore )
{
	CString strScore;
	CString strReturn;
	strScore.Format(TEXT("%I64d"), lScore);

	int nStrCount = 0;
	for( int i = strScore.GetLength() - 1; i >= 0; )
	{
		if( (nStrCount%3) == 0 && nStrCount != 0 )
		{
			strReturn.Insert(0, ',');
			nStrCount = 0;
		}
		else
		{
			strReturn.Insert(0, strScore.GetAt(i));
			nStrCount++;
			i--;
		}
	}
	return strReturn;
}

// 更新按钮
VOID CGameClientView::UpdateSound()
{
	CGlobalUnits *pGlobalUnits=CGlobalUnits::GetInstance();
	m_btSound.SetButtonImage(!pGlobalUnits->m_bMuteStatuts ? IDB_BT_SOUND_OPEN : IDB_BT_SOUND_CLOSE, AfxGetInstanceHandle(), false, false);
}

HBRUSH CGameClientView::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = __super::OnCtlColor(pDC, pWnd, nCtlColor);

	if(pWnd->GetSafeHwnd() == GetDlgItem(IDC_CHAT_INPUT)->GetSafeHwnd())
	{
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(RGB(193,167,108));
		return m_brush; 
	}
	return hbr;
}