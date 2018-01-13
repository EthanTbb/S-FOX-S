#include "../Stdafx.h"
#include ".\flashshowwnd.h"




#ifdef _DEBUG
#define  new DEBUG_NEW
#endif



BEGIN_MESSAGE_MAP(CFlashShowWnd2, CWnd)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_WM_CLOSE()
END_MESSAGE_MAP()


CFlashShowWnd2::CFlashShowWnd2(void)
{
	m_cx = m_cy = 100;
	m_returnparam = NULL;
	m_pPlayer = NULL;
}

CFlashShowWnd2::~CFlashShowWnd2(void)
{
	SafeDelete(m_pPlayer);
}

bool CFlashShowWnd2::StartAnimation(PlayFlashAnimation *pplayparam,bool bShow)
{
	RECT	rcParent;
	LPRECT	prcNow;

	if (!PathFileExists(pplayparam->theFileName))
	{
		ASSERT(FALSE);
	    MessageBox(0,TEXT( "对不起文件损害,建议您重新下载更新!"),  0);
		return false;
	}

	//copy and save variable
	m_ppWnd			= pplayparam->ppWnd;
	m_returnparam	= pplayparam->return_param;
	m_msgID			= pplayparam->msgID;
	m_nCurrentFrame = 0;

	//
	if (pplayparam->lprcshow) prcNow = pplayparam->lprcshow;
	else
	{
		::GetWindowRect(pplayparam->ppWnd, &rcParent);
		prcNow = &rcParent;
	}
	m_cx = prcNow->right-prcNow->left;
	m_cy = prcNow->bottom-prcNow->top;
	if (!GetSafeHwnd())
	{
		if(bShow)
		{
			if (!CreateEx(
				WS_EX_LAYERED, NULL, NULL,
				WS_POPUP|WS_VISIBLE|WS_CLIPCHILDREN|WS_CLIPSIBLINGS,
				prcNow->left, prcNow->top, m_cx, m_cy,
				pplayparam->ppWnd, 0, 0))
				return false;
			//this->SetParent(FromHandle(m_ppWnd));
		}
		else
		{
			if (!CreateEx(
				WS_EX_LAYERED, NULL, NULL,
				WS_POPUP|WS_DISABLED|WS_CLIPCHILDREN|WS_CLIPSIBLINGS,
				prcNow->left, prcNow->top, m_cx, m_cy,
				pplayparam->ppWnd, 0, 0))
				return false;
			//this->SetParent(FromHandle(m_ppWnd));
		}
		//::SetWindowLong(m_hWnd, GWL_EXSTYLE,::GetWindowLong(m_hWnd,GWL_EXSTYLE)|WS_EX_LAYERED);
		//::SetLayeredWindowAttributes();
	}
	else
	{
		SetWindowPos(NULL, prcNow->left, prcNow->top, m_cx, m_cy, SWP_NOREDRAW);
	}

	if (m_pPlayer==NULL) m_pPlayer = new FlashPlayer2;

	//char szfile[260];
	//memset(szfile,0,sizeof(szfile));
	//CopyMemory(szfile,pplayparam->theFileName,sizeof(pplayparam->theFileName));
	//std::string str(szfile);

    
   
	//这个地方的转换可能会出错
	if (!m_pPlayer->StartAnimation(pplayparam->theFileName, m_cx, m_cy, m_hWnd))
	{
		return false;
	}
	//if (!m_pPlayer->StartAnimation(str, m_cx, m_cy, m_hWnd))
	//{
	//	return false;
	//}
   
	m_pPlayer->SetQuality(FlashPlayer2::QUALITY_HIGH);

	if(bShow)
	{
		DrawFlashToClient();
		ShowWindow(SW_SHOW);
	}	
	KillTimer(TIMER_ID_FLASH_PLAY);
	//SetTimer(TIMER_ID_FLASH_PLAY, 1000/25, NULL);

	return true;
}


void CFlashShowWnd2::StopAnimation()
{
	KillTimer(TIMER_ID_FLASH_PLAY);
	ShowWindow(SW_HIDE);
}

void CFlashShowWnd2::DrawFlashToClient()
{
	static BLENDFUNCTION bl={AC_SRC_OVER, 0, 255, 0x01};

	CClientDC	clientdc(this);
	CDC			buffdc;
	LONG		w, h;

	if (m_buffer.IsNull()) return ;

	buffdc.Attach(m_buffer.GetDC());
	//
	m_pPlayer->GetBitmapSize(&w, &h);
	buffdc.FillSolidRect(0, 0, w, h, 0x0);
	if (m_pPlayer->Render(buffdc.m_hDC, 0, 0, FALSE))
	{
		//
		RECT	re;
		POINT	pt={0, 0};
		POINT	pt2;
		SIZE	sz;

		GetWindowRect(&re);
		sz.cx = w, sz.cy=h;
		pt2.x = re.left, pt2.y = re.top;
		//clientdc.BitBlt(0, 0, m_cx, m_cy, &buffdc, 0, 0, SRCCOPY);
		::UpdateLayeredWindow(m_hWnd, clientdc.m_hDC, &pt2, &sz, buffdc.m_hDC, &pt, 0x0, &bl, ULW_ALPHA);
	}	
	buffdc.Detach();
	m_buffer.ReleaseDC();
}

BOOL CFlashShowWnd2::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

void CFlashShowWnd2::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	m_buffer.BitBlt(dc.m_hDC, 0, 0);
}

bool CFlashShowWnd2::DealFlashCommand(LPCSTR cmd, LPCSTR lparam)
{
	return false;
}

//绘制
bool CFlashShowWnd2::DrawAnimation(int nFrame)
{
	if (m_pPlayer == NULL) 
		return true;
	//最后一桢绘制后返回结束消息
	int nTotalF = m_pPlayer->GetTotalFrames();
	int nCurF = m_nCurrentFrame+1;//m_pPlayer->GetCurrentFrame();	
	if(nCurF>=nTotalF-1)//nCurF < m_nCurrentFrame||nCurF>=nTotalF-1)
	{	
		nCurF = nTotalF-1;
		m_pPlayer->GotoFrame(nCurF);
		m_pPlayer->Pause(true);
		DrawFlashToClient();		
		m_nCurrentFrame = 0;
		//不停止，再显示4秒后隐藏
  		//KillTimer(TIMER_ID_FLASH_PLAY);
  		//PostMessage(WM_CLOSE);
  		//m_pPlayer->Rewind();
  		//if (m_ppWnd) ::SendMessage(m_ppWnd, m_msgID, (WPARAM)m_returnparam, m_msgID);
  		//SafeDelete(m_pPlayer);
		return true;
	}
	m_pPlayer->GotoFrame(nCurF);
	DrawFlashToClient();
	m_nCurrentFrame = nCurF;
	return false;
}

void CFlashShowWnd2::OnTimer(UINT nIDEvent)
{
	switch(nIDEvent)
	{
	case TIMER_ID_FLASH_PLAY:
		if (m_pPlayer == NULL) break;
		//最后一桢绘制后返回结束消息
		int nTotalF = m_pPlayer->GetTotalFrames();
		int nCurF = m_pPlayer->GetCurrentFrame();

		//DrawFlashToClient();
		//
 	//	if(nCurF < m_nCurrentFrame||nCurF>=nTotalF-1)
 	//	{
 	//		//m_pPlayer->Pause(true);
 	//		KillTimer(TIMER_ID_FLASH_PLAY);
  //			PostMessage(WM_CLOSE);
  //			m_pPlayer->Rewind();
  //			if (m_ppWnd) ::SendMessage(m_ppWnd, m_msgID, (WPARAM)m_returnparam, m_msgID);
  //			SafeDelete(m_pPlayer);
 	//		break;
 	//	}
		//m_nCurrentFrame = nCurF;

		std::string command, cmdparam;
		if (m_pPlayer->GetFlashCommand(command, cmdparam))
		{
			if (DealFlashCommand(command.c_str(), cmdparam.c_str()))
			{
				break;
			}
			else if (command.compare("close") == 0)
			{
				KillTimer(TIMER_ID_FLASH_PLAY);
				PostMessage(WM_CLOSE);
				m_pPlayer->Rewind();
				if (m_ppWnd) ::SendMessage(m_ppWnd, m_msgID, (WPARAM)m_returnparam, 0);
				SafeDelete(m_pPlayer);
			}
		}
		break;
	}
	return ;
}

void CFlashShowWnd2::OnSize(UINT nType, int cx, int cy)
{
	m_cx = cx;
	m_cy = cy;
	if (m_cx<1||m_cy<1) return ;

	m_buffer.Destroy();
	m_buffer.Create(m_cx, m_cy, 32);
}


void CFlashShowWnd2::OnLButtonDown(UINT nFlags, CPoint point)
{
    SendMessage(WM_SYSCOMMAND, 0xf012);
	CWnd::OnLButtonDown(nFlags, point);
}

void CFlashShowWnd2::OnClose()
{
	DestroyWindow();
}
void CFlashShowWnd2::OnCloseFalsh()
{
	KillTimer(TIMER_ID_FLASH_PLAY);
	PostMessage(WM_CLOSE);
	if(m_pPlayer)
	{
		m_pPlayer->Rewind();
		//if (m_ppWnd) ::SendMessage(m_ppWnd, m_msgID, (WPARAM)m_returnparam, 0);
		SafeDelete(m_pPlayer);
	}	
	DestroyWindow();
	return;
}

BOOL CFlashShowWnd2::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if(pMsg->message!=WM_TIMER)
		return TRUE;

	return CWnd::PreTranslateMessage(pMsg);
}
