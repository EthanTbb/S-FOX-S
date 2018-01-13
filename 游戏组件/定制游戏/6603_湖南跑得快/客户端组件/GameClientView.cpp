#include "StdAfx.h"
#include "GameLogic.h"
#include "Resource.h"
#include "GameClient.h"
#include "GameClientView.h"

//////////////////////////////////////////////////////////////////////////
//��ť��ʶ

#define IDC_START						100								//��ʼ��ť
#define IDC_OUT_CARD					101								//���ư�ť
#define IDC_PASS_CARD					102								//PASS��ť
#define IDC_OUT_PROMPT					103								//��ʾ��ť

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameClientView, CGameFrameViewGDI)
	ON_WM_CREATE()
	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CGameClientView::CGameClientView()
{
	//��Ϸ����
	m_lCellScore=0;

	//���ñ���
	m_bDeasilOrder=true;

	//״̬����
	ZeroMemory(m_bPass,sizeof(m_bPass));
	ZeroMemory(m_bBombCount,sizeof(m_bBombCount));
	ZeroMemory(m_bCardCount,sizeof(m_bCardCount));

	//��ʷ����
	ZeroMemory(m_lAllTurnScore,sizeof(m_lAllTurnScore));
	ZeroMemory(m_lLastTurnScore,sizeof(m_lLastTurnScore));


}

//��������
CGameClientView::~CGameClientView()
{
}

//���ý���
VOID CGameClientView::ResetGameView()
{
	//��Ϸ����
	m_lCellScore=0L;

	//״̬����
	ZeroMemory(m_bPass,sizeof(m_bPass));
	ZeroMemory(m_bCardCount,sizeof(m_bCardCount));
	ZeroMemory(m_bBombCount,sizeof(m_bBombCount));

	//��ʷ����
	ZeroMemory(m_lAllTurnScore,sizeof(m_lAllTurnScore));
	ZeroMemory(m_lLastTurnScore,sizeof(m_lLastTurnScore));

	//���ؿؼ�
	m_ScoreView.ShowWindow(SW_HIDE);

	//���ذ�ť
	m_btStart.ShowWindow(SW_HIDE);
	m_btOutCard.ShowWindow(SW_HIDE);
	m_btPassCard.ShowWindow(SW_HIDE);
	m_btOutPrompt.ShowWindow(SW_HIDE);

	//���ÿؼ�
	m_btOutCard.EnableWindow(FALSE);
	m_btPassCard.EnableWindow(FALSE);

	//�˿˿ؼ�
	m_HandCardControl.SetCardData(NULL,0);
	m_HandCardControl.SetPositively(false);
	m_HandCardControl.SetDisplayFlag(false);
	m_LeaveCardControl[0].SetCardData(NULL,0);
	m_LeaveCardControl[1].SetCardData(NULL,0);
	for (WORD i=0;i<GAME_PLAYER;i++) m_UserCardControl[i].SetCardData(NULL,0);

}

//�����ؼ�
VOID CGameClientView::RectifyControl(INT nWidth, INT nHeight)
{
	int m_nYBorder=0,m_nXBorder=0;
	int m_nXFace=50,m_nYFace=50;
	int m_nXTimer=55,m_nYTimer=55;
	int nViewHeight=nHeight-m_nYBorder;
	//��������
	m_ptAvatar[2].x=nWidth-m_nXBorder-m_nXFace-5;
	m_ptAvatar[2].y=nViewHeight/2-m_nYFace-25;
	m_ptNickName[2].x=nWidth-m_nXBorder-m_nXFace-10;
	m_ptNickName[2].y=nViewHeight/2-m_nYFace-23;
	m_ptClock[2].x=nWidth-m_nXBorder-m_nXTimer/2;
	m_ptClock[2].y=nHeight/2-m_nYFace-m_nYTimer-5;
	m_ptReady[2].x=nWidth-m_nXBorder-m_nXTimer-5;
	m_ptReady[2].y=nViewHeight/2-m_nYFace-m_nYTimer/2-30;
	m_PointUserPass[2].x=nWidth-m_nXBorder-m_nXFace-150;
	m_PointUserPass[2].y=nHeight/2-m_nYFace+5;
	m_PointUserBomb[2].x=nWidth-m_nXBorder-m_nXFace-5;
	m_PointUserBomb[2].y=nHeight/2+5;

	m_ptAvatar[0].x=m_nXBorder+5;
	m_ptAvatar[0].y=nViewHeight/2-m_nYFace-25;
	m_ptNickName[0].x=m_nXBorder+m_nXFace+10;
	m_ptNickName[0].y=nViewHeight/2-m_nYFace-23;
	m_ptClock[0].x=m_nXBorder+m_nXTimer/2;
	m_ptClock[0].y=nHeight/2-m_nYFace-m_nYTimer;
	m_ptReady[0].x=m_nXBorder+5+m_nXTimer;
	m_ptReady[0].y=nViewHeight/2-m_nYFace-m_nYTimer/2-30;
	m_PointUserPass[0].x=m_nXBorder+m_nXFace+107;
	m_PointUserPass[0].y=nHeight/2-m_nYFace+5;
	m_PointUserBomb[0].x=m_nXBorder+5;
	m_PointUserBomb[0].y=nHeight/2+5;

	m_ptAvatar[1].x=nWidth/2-m_nXFace/2;
	m_ptAvatar[1].y=nHeight-m_nYBorder-m_nYFace-5;
	m_ptNickName[1].x=nWidth/2+5+m_nXFace/2;
	m_ptNickName[1].y=nHeight-m_nYBorder-m_nYFace-2;
	m_ptClock[1].x=nWidth/2-m_nXFace/2-m_nXTimer/2-5;
	m_ptClock[1].y=nHeight-m_nYBorder-m_nYTimer+20;
	m_ptReady[1].x=nWidth/2;
	m_ptReady[1].y=nHeight-140;//-m_nYBorder-m_nYTimer/2-10;
	m_PointUserPass[1].x=nWidth/2-21;
	m_PointUserPass[1].y=nHeight-m_nYBorder-m_nYFace-210;
	m_PointUserBomb[1].x=nWidth/2-m_nXFace/2-150;
	m_PointUserBomb[1].y=nHeight-m_nYBorder-m_nYFace-3;

	//�ƶ���ť
	CRect rcButton;
	HDWP hDwp=BeginDeferWindowPos(11);
	const UINT uFlags=SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOCOPYBITS|SWP_NOSIZE;

	m_btStart.GetWindowRect(&rcButton);
	DeferWindowPos(hDwp,m_btStart,NULL,(nWidth-rcButton.Width())/2,nHeight-193-10,0,0,uFlags);

	m_btOutCard.GetWindowRect(&rcButton);
	DeferWindowPos(hDwp,m_btOutCard,NULL,nWidth/2-rcButton.Width()*3/2-10,nHeight-203-10,0,0,uFlags);
	DeferWindowPos(hDwp,m_btOutPrompt,NULL,nWidth/2-rcButton.Width()/2,nHeight-203-10,0,0,uFlags);
	DeferWindowPos(hDwp,m_btPassCard,NULL,nWidth/2+rcButton.Width()/2+10,nHeight-203-10,0,0,uFlags);

	EndDeferWindowPos(hDwp);

	//�ƶ��˿�
	m_HandCardControl.SetBenchmarkPos(nWidth/2,nHeight-m_nYFace-m_nYBorder-20,enXCenter,enYBottom);
	m_UserCardControl[1].SetBenchmarkPos(nWidth/2,nHeight-208-m_nYBorder,enXCenter,enYBottom);

	//�û��˿�
	m_UserCardControl[2].SetBenchmarkPos(nWidth-m_nXFace-10-130,nHeight/2-30,enXRight,enYCenter);
	m_UserCardControl[0].SetBenchmarkPos(10+m_nXFace+130,nHeight/2-30,enXLeft,enYCenter);
	m_LeaveCardControl[1].SetBenchmarkPos(nWidth-m_nXBorder-m_nXFace-70,nHeight/2-50,enXRight,enYCenter);
	m_LeaveCardControl[0].SetBenchmarkPos(m_nXBorder+m_nXFace+70,nHeight/2-50,enXLeft,enYCenter);


	//������ͼ
	CRect rcControl;
	m_ScoreView.GetWindowRect(&rcControl);
	m_ScoreView.SetWindowPos(NULL,(nWidth-rcControl.Width())/2,nHeight/2-rcControl.Height()/2,0,0,SWP_NOZORDER|SWP_NOSIZE);

	return;
}
//������Ϣ
int CGameClientView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct)==-1) return -1;


	//������Դ
	HINSTANCE hInstance=AfxGetInstanceHandle();
	m_ImageBomb.LoadFromResource(this,hInstance,IDB_BOMB);
	m_ImageBack.LoadFromResource(this,hInstance,IDB_VIEW_BACK);
	m_ImageFill.LoadFromResource(this,hInstance,IDB_VIEW_FILL);
	m_ImageGameInfo.LoadFromResource(this,hInstance,IDB_GAME_INFO);
	m_ImageUserPass.LoadFromResource(this,hInstance,IDB_USER_PASS);

	//�����ؼ�
	CRect CreateRect(0,0,0,0);
	m_ScoreView.Create(NULL,NULL,WS_CHILD|WS_CLIPSIBLINGS|WS_CLIPCHILDREN,CreateRect,this,9);

	//�����˿�
	for (WORD i=0;i<3;i++)
	{
		m_UserCardControl[i].SetDirection(true);
		m_UserCardControl[i].SetDisplayFlag(true);
		m_UserCardControl[i].Create(NULL,NULL,WS_VISIBLE|WS_CHILD,CreateRect,this,20+i);
	}
	for (WORD i=0;i<2;i++) 
	{
		m_LeaveCardControl[i].SetDirection(false);
		m_LeaveCardControl[i].SetDisplayFlag(true);
		m_LeaveCardControl[i].SetCardSpace(0,18,0);
		m_LeaveCardControl[i].Create(NULL,NULL,WS_VISIBLE|WS_CHILD,CreateRect,this,30+i);
	}
	m_HandCardControl.SetSinkWindow(this);
	m_HandCardControl.Create(NULL,NULL,WS_VISIBLE|WS_CHILD,CreateRect,this,40);

	//������ť
	m_btStart.Create(TEXT(""),WS_CHILD,CreateRect,this,IDC_START);
	m_btOutCard.Create(TEXT(""),WS_CHILD,CreateRect,this,IDC_OUT_CARD);
	m_btPassCard.Create(TEXT(""),WS_CHILD,CreateRect,this,IDC_PASS_CARD);
	m_btOutPrompt.Create(TEXT(""),WS_CHILD,CreateRect,this,IDC_OUT_PROMPT);

	//���ð�ť

	m_btStart.SetButtonImage(IDB_START,hInstance,false,false);
	m_btOutCard.SetButtonImage(IDB_OUT_CARD,hInstance,false,false);
	m_btPassCard.SetButtonImage(IDB_PASS,hInstance,false,false);
	m_btOutPrompt.SetButtonImage(IDB_OUT_PROMPT,hInstance,false,false);

	//��ȡ����
	//m_bDeasilOrder=AfxGetApp()->GetProfileInt(TEXT("GameOption"),TEXT("DeasilOrder"),FALSE)?true:false;
	return 0;
}
//�����
BOOL CGameClientView::OnCommand(WPARAM wParam, LPARAM lParam)
{
	switch(LOWORD(wParam))
	{
	case IDC_START:
		{
			SendEngineMessage(IDM_START,0,0);
			return TRUE;
		}
	case IDC_OUT_CARD:
		{
			SendEngineMessage(IDM_OUT_CARD,1,1);
			return TRUE;
		}
	case IDC_PASS_CARD:
		{
			SendEngineMessage(IDM_PASS_CARD,1,1);
			return TRUE;
		}
	case IDC_OUT_PROMPT:
		{
			SendEngineMessage(IDM_OUT_PROMPT,0,0);
			return TRUE;
		}
	}
	return __super::OnCommand(wParam, lParam);
}
//�滭����
VOID CGameClientView::DrawGameView(CDC * pDC, int nWidth, int nHeight)
{
	//�滭����
	DrawViewImage(pDC,m_ImageFill,DRAW_MODE_SPREAD);
	DrawViewImage(pDC,m_ImageBack,DRAW_MODE_CENTENT);

	//DrawUserReady(pDC,m_ptReady[1].x,m_ptReady[1].y);

	//�滭�û�
	TCHAR szBuffer[32];
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//��������
		WORD wUserTimer=GetUserClock(i);
		IClientUserItem *pUserItem=GetClientUserItem(i);
		const tagUserInfo * pUserData=(pUserItem==NULL?NULL:pUserItem->GetUserInfo());

		//�滭�û�
		if (pUserData!=NULL)
		{
			//�û�����
			CDFontEx::DrawText(this, pDC, 14, 400,pUserData->szNickName,m_ptNickName[i].x,m_ptNickName[i].y, (wUserTimer>0)?RGB(250,250,250):RGB(220,220,220), i==2?DT_RIGHT:DT_LEFT);
			//ʣ���˿�
			_sntprintf(szBuffer,sizeof(szBuffer),TEXT("��%d �ţ�"),m_bCardCount[i]);

			CDFontEx::DrawText(this, pDC, 14, 400,szBuffer,m_ptNickName[i].x,m_ptNickName[i].y+18, (wUserTimer>0)?RGB(250,250,250):RGB(220,220,220), i==2?DT_RIGHT:DT_LEFT);
			//ը����Ϣ
			if (m_bBombCount[i]!=0)
			{
				//�滭λͼ
				m_ImageBomb.TransDrawImage(pDC,m_PointUserBomb[i].x,m_PointUserBomb[i].y,RGB(255,0,255));

				//�����Ϣ
				pDC->SetTextColor(RGB(255,255,0));
				_sntprintf(szBuffer,sizeof(szBuffer),TEXT("ը %d ��"),m_bBombCount[i]);
				if (((i==0)&&(m_bDeasilOrder==true))||((i==2)&&(m_bDeasilOrder==false)))
				{
					CDFontEx::DrawText(this, pDC, 14, 400,szBuffer,m_PointUserBomb[i].x-5,m_PointUserBomb[i].y+18, RGB(255,255,0), i==2?DT_RIGHT:DT_LEFT);

				}
				else
				{
					CDFontEx::DrawText(this, pDC, 14, 400,szBuffer,m_PointUserBomb[i].x+32,m_PointUserBomb[i].y+18, RGB(255,255,0), i==2?DT_RIGHT:DT_LEFT);


				}
			}

			//������Ϣ
			if (wUserTimer!=0) DrawUserClock(pDC,m_ptClock[i].x,m_ptClock[i].y,wUserTimer);
			if (pUserData->cbUserStatus==US_READY) DrawUserReady(pDC,m_ptReady[i].x,m_ptReady[i].y);
			DrawUserAvatar(pDC,m_ptAvatar[i].x,m_ptAvatar[i].y,pUserItem);
		}
	}

	//������־
	for (WORD i=0;i<3;i++)
	{
		if (m_bPass[i]==true) 
		{
			m_ImageUserPass.TransDrawImage(pDC,m_PointUserPass[i].x,m_PointUserPass[i].y,RGB(255,0,255));
		}
	}

	//��Ϸ��Ϣ
	m_ImageGameInfo.TransDrawImage(pDC,0,0,RGB(255,0,255));

	//������Ϣ
	TCHAR szCellScore[16];
	pDC->SetTextColor(RGB(255,255,255));
	pDC->SetTextAlign(TA_CENTER|TA_TOP);
	_sntprintf(szCellScore,CountArray(szCellScore),TEXT("%I64d"),m_lCellScore);

	CDFontEx::DrawText(this, pDC, 14, 400,szCellScore,62,9, RGB(255,255,255), DT_CENTER);



}

//�û�˳��
void CGameClientView::SetUserOrder(bool bDeasilOrder)
{
	//״̬�ж�
	if (m_bDeasilOrder==bDeasilOrder) return;

	//���ñ���
	m_bDeasilOrder=bDeasilOrder;
	//AfxGetApp()->WriteProfileInt(TEXT("GameOption"),TEXT("DeasilOrder"),m_bDeasilOrder?TRUE:FALSE);

	//���ý���
	CRect rcClient;
	GetClientRect(&rcClient);
	RectifyControl(rcClient.Width(),rcClient.Height());

	//ˢ�½���
	InvalidGameView(0,0,0,0);

	return;
}

//��������
void CGameClientView::SetCellScore(LONGLONG lCellScore)
{
	//���ñ���
	m_lCellScore=lCellScore;

	//ˢ�½���
	InvalidGameView(0,0,0,0);

	return;
}

//���÷���
void CGameClientView::SetPassFlag(WORD wChairID, bool bPass)
{
	//���ñ���
	if (wChairID==INVALID_CHAIR)
	{
		for (WORD i=0;i<GAME_PLAYER;i++) m_bPass[i]=bPass;
	}
	else m_bPass[wChairID]=bPass;

	//���½���
	InvalidGameView(0,0,0,0);

	return;
}

//ը����Ŀ
void CGameClientView::SetBombCount(WORD wChairID, BYTE bBombCount)
{
	//���ñ���
	if (wChairID==INVALID_CHAIR)
	{
		for (WORD i=0;i<GAME_PLAYER;i++) m_bBombCount[i]=bBombCount;
	}
	else m_bBombCount[wChairID]=bBombCount;

	//���½���
	InvalidGameView(0,0,0,0);

	return;
}

//�˿���Ŀ
void CGameClientView::SetCardCount(WORD wChairID, BYTE bCardCount)
{
	//���ñ���
	if (wChairID==INVALID_CHAIR)
	{
		for (WORD i=0;i<GAME_PLAYER;i++) m_bCardCount[i]=bCardCount;
	}
	else m_bCardCount[wChairID]=bCardCount;

	//���½���
	InvalidGameView(0,0,0,0);

	return;
}
//�����Ϣ
void CGameClientView::OnRButtonUp(UINT nFlags, CPoint Point)
{
	__super::OnRButtonUp(nFlags, Point);

	//�����˿�
	m_btOutCard.EnableWindow(FALSE);
	m_HandCardControl.ShootAllCard(false);

	return;
}

//���˫��
void CGameClientView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	__super::OnLButtonDblClk(nFlags, point);

	//�����˿�
	m_btOutCard.EnableWindow(FALSE);
	m_HandCardControl.ShootAllCard(false);

	return;
}

//////////////////////////////////////////////////////////////////////////////////
