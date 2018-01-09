// AnimationWnd.cpp : implementation file
//

#include "stdafx.h"
#include "AnimationWnd.h"
#include ".\animationwnd.h"


// CAnimationWnd

IMPLEMENT_DYNAMIC(CAnimationWnd, CWnd)
CAnimationWnd::CAnimationWnd(CWnd*pParent)
{
	m_nPlayTime=1;
    m_nAniamationID=0;      //动画序号
    m_nFrameCount=0;        //动画帧数
    m_nCurFrame=0;          //当前帧
    m_bISRuning=false;      //正在执行动画
	m_bSoundLoop=false;
	m_bPostEndEvent=false;
	m_bHaveBGImage=false;
	m_bHasCopy=false;

}

CAnimationWnd::~CAnimationWnd()
{	
	if(!m_bHaveBGImage)
	{
		m_MemDCBG.SelectObject(m_pMemBmpBG);
		m_MemDCBG.DeleteDC();
		m_MemBmpBG.DeleteObject();
	}
}


BEGIN_MESSAGE_MAP(CAnimationWnd, CWnd)
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_ENABLE()
	ON_WM_ERASEBKGND()
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()



// CAnimationWnd message handlers


void CAnimationWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CDC MemDC;
	CRect ClientRect;
	CBitmap MemBitMap,*pOldBitmap;
	GetClientRect(&ClientRect);
	MemDC.CreateCompatibleDC(&dc);
	MemBitMap.CreateCompatibleBitmap(&dc,ClientRect.Width(),ClientRect.Height());
	pOldBitmap=MemDC.SelectObject(&MemBitMap);

	if(!m_bHaveBGImage)
	    MemDC.BitBlt(0,0,ClientRect.Width(),ClientRect.Height(),&m_MemDCBG,0,0,SRCCOPY);
	else
	{
		m_ImageBackGround.BitBlt(MemDC.m_hDC,0,0,ClientRect.Width(),ClientRect.Height(),414,1);
	}
   
	if(m_bISRuning&&m_nCurFrame>0&&m_nCurFrame<=m_nFrameCount)
   { 
		m_pFrameInfo[m_nCurFrame-1].m_PngImage->DrawImage(&MemDC,(ClientRect.Width()-m_pFrameInfo[m_nCurFrame-1].m_nCX)/2,
		(ClientRect.Height()-m_pFrameInfo[m_nCurFrame-1].m_nCY)/2,
		m_pFrameInfo[m_nCurFrame-1].m_nCX,m_pFrameInfo[m_nCurFrame-1].m_nCY,
		m_pFrameInfo[m_nCurFrame-1].m_nXPos,m_pFrameInfo[m_nCurFrame-1].m_nYPos);
   }
	
  dc.BitBlt(0,0,ClientRect.Width(),ClientRect.Height(),&MemDC,0,0,SRCCOPY);
  MemDC.SelectObject(pOldBitmap);
  MemDC.DeleteDC();
  MemBitMap.DeleteObject();
}

bool CAnimationWnd::CloseAninamation()
{
   if(m_bISRuning)
   {
	   KillTimer(1);
	   KillTimer(3);
	   if(m_bPostEndEvent)
		   GetParent()->SendMessage(WM_ANIMATIONFINISH,m_nCurUser,m_nAniamationID);
	   m_bISRuning=false;
	   ShowWindow(SW_HIDE);
   }
   return true;
}

bool CAnimationWnd::PlayAnimation(WORD wCharID,CGameCartoon&CartoonInfo,int	 nPlayTimes,bool bloop,bool bPostEndEvent,bool bSoundLoop)
{

	if(m_bISRuning)
	{
		KillTimer(1);
	}
	
	m_nCurUser=wCharID;
	m_nCurFrame=0;

	m_bloop=bloop;
	m_bISRuning=true;
	m_nPlayTime=nPlayTimes;
	m_bSoundLoop=bSoundLoop;
	m_bPostEndEvent=bPostEndEvent;
	m_nFrameCount=CartoonInfo.m_nFrameCount;
	m_nAniamationID=CartoonInfo.m_nCartoonID;
	m_pFrameInfo=CartoonInfo.GetFrameInfo(0);
	
	//播放音效
	if(!m_bSoundLoop&&!CartoonInfo.m_pszSoundName.IsEmpty())
	{
		PlayAninamationSound(AfxGetInstanceHandle(),CartoonInfo.m_pszSoundName);
	}
	else
		m_szSoundName=CartoonInfo.m_pszSoundName;

	SetTimer(1,10,0);
	ShowWindow(SW_SHOW);
	return true;

}

void CAnimationWnd::SetPosInParent(CRect& Pos)
{
	m_rcInParent=Pos;
	UpdateWindow();
}

bool CAnimationWnd::PlayAninamationSound(HINSTANCE hInstance, LPCTSTR pszSoundName)
{		
	    //加载资源
	   HRSRC hResour=FindResource(hInstance,pszSoundName,TEXT("WAVE"));
		if (hResour==NULL) return false;
		HGLOBAL hGlobal=LoadResource(hInstance,hResour);
		if (hGlobal==NULL) return false;

		//播放声音
		LPCTSTR pszMemory=(LPCTSTR)LockResource(hGlobal);
		::PlaySound(pszMemory,hInstance,SND_ASYNC|SND_MEMORY);

		//清理资源
		UnlockResource(hGlobal);
		FreeResource(hGlobal);
	return true;
}

void CAnimationWnd::SetBGImage(UINT nResourceId,bool IsWndSize)
{
    HINSTANCE hInstance=AfxGetInstanceHandle();
	m_ImageBackGround.LoadFromResource(hInstance,nResourceId);
	if(IsWndSize)
	{
		int nWidth=m_ImageBackGround.GetWidth();
		int nHeight=m_ImageBackGround.GetHeight();
		SetWindowPos(NULL,0,0,nWidth,nHeight,SWP_NOMOVE);
		
	}
	m_bHaveBGImage=true;

}


void CAnimationWnd::CreateMemDC()
{
	m_bUserFramList=true;
	CDC*pDC=GetDC();
	CRect rect;
	GetClientRect(&rect);
	m_MemDCBG.CreateCompatibleDC(pDC);
	m_MemBmpBG.CreateCompatibleBitmap(pDC,rect.Width(),rect.Height());
	m_pMemBmpBG=m_MemDCBG.SelectObject(&m_MemBmpBG);
	ReleaseDC(pDC);
	m_bHaveBGImage=false;
}


void CAnimationWnd::CopyBackGroud(CDC*pDC)
{
	if(!m_bHaveBGImage)
	{
		if(!m_bHasCopy)
		{
			m_MemDCBG.BitBlt(0,0,m_rcInParent.Width(),m_rcInParent.Height(),pDC,m_rcInParent.left,m_rcInParent.top,SRCCOPY);
			m_bHasCopy=true;
		}
	}
}


void CAnimationWnd::OnTimer(UINT nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
   switch(nIDEvent) 
   {
		case 1:
			{
				KillTimer(1);
				if((++m_nCurFrame)>=m_nFrameCount)
				{
					if(!m_bloop)
					{
						if((m_nPlayTime--)>0)
						{
							m_nCurFrame=1;
							SetTimer(1,m_pFrameInfo[m_nCurFrame-1].m_nTimer,0);
						}
						else
						{
							if(m_pFrameInfo[m_nFrameCount-1].m_nTimer==0)
							{
								m_nFrameCount=0;
								m_nCurFrame=0;
								m_bISRuning=false;
								if(m_bPostEndEvent)
									GetParent()->SendMessage(WM_ANIMATIONFINISH,m_nCurUser,m_nAniamationID);
								ShowWindow(SW_HIDE);
							}
							else
								SetTimer(2,m_pFrameInfo[m_nFrameCount-1].m_nTimer,0);
						}
					}
					else
					{
						m_nCurFrame=1;
						SetTimer(1,m_pFrameInfo[m_nCurFrame-1].m_nTimer,0);
					}
				}
				else
				{
					if(m_bSoundLoop)
						PlayAninamationSound(AfxGetInstanceHandle(),m_szSoundName);
					if(m_pFrameInfo[m_nCurFrame-1].m_bSendMsg)
						GetParent()->SendMessage(WM_ANIMATIONFINISH,m_nCurUser,m_nAniamationID);
					SetTimer(1,m_pFrameInfo[m_nCurFrame-1].m_nTimer,0);
				}
				Invalidate(true);
				return;
			}
		case 2:
			{
				KillTimer(2);
				if(m_bPostEndEvent)
					GetParent()->SendMessage(WM_ANIMATIONFINISH,m_nCurUser,m_nAniamationID);
				//ShowWindow(SW_HIDE);
				return;
			}
   }
	CWnd::OnTimer(nIDEvent);
}

void CAnimationWnd::OnEnable(BOOL bEnable)
{
	CWnd::OnEnable(bEnable);
   // TODO: Add your message handler code here
}

BOOL CAnimationWnd::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default
	return TRUE;
}

void CAnimationWnd::OnShowWindow(BOOL bShow, UINT nStatus)
{
	if(bShow==SW_HIDE&&m_bISRuning)
	{
			KillTimer(1);
			KillTimer(3);
	}
	CWnd::OnShowWindow(bShow, nStatus);
	// TODO: Add your message handler code here
}





CGameCartoon::CGameCartoon()
{

}

CGameCartoon::~CGameCartoon()
{
	delete m_pFramList;
}

bool CGameCartoon::UpdataFrame(int cbIndex,int nXPos,int nYPos)
{
   m_pFramList[cbIndex].m_nXPos=nXPos;
   m_pFramList[cbIndex].m_nYPos=nYPos;
   return true;
}

bool CGameCartoon::SetCartoonInfo(int nCartoonID,int nFrameCount,LPCTSTR pszSoundName)
{
	m_nCartoonID=nCartoonID;
	m_nFrameCount=nFrameCount;
	if(pszSoundName!=NULL)
		m_pszSoundName.Format(_T("%s"),pszSoundName);
	else
		m_pszSoundName=_T("");
	if(nFrameCount>0)
		m_pFramList=new tagFrameInfo[nFrameCount];
	return true;
}

CGameCartoon::tagFrameInfo* CGameCartoon::GetFrameInfo(int nIndex)
{
	if(nIndex<m_nFrameCount&&nIndex>=0)
	{
		return &m_pFramList[nIndex];
	}
	return NULL;
}

bool CGameCartoon::SetFrameInfo(int cbIndex,int cbTimer,int nXPos,int nYPos,int nCX,int nCY,CPngImage *pPngImage,bool bSendMsg)
{
	if(cbIndex<m_nFrameCount&&cbIndex>=0&&m_pFramList!=NULL)
	{
		m_pFramList[cbIndex].SetFrameInfo(cbIndex,cbTimer,nXPos,nYPos,nCX,nCY,pPngImage,bSendMsg);
		return true;
	}
	return false;
}
