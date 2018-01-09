#include "StdAfx.h"
#include "resource.h"
#include ".\playoperate.h"

//////////////////////////////////////////////////////////////////////////
// 玩家操作
CPlayOperate::CPlayOperate(void)
{
	m_bShow = false;
	m_bMouseDown = false;
	m_lCellScore = 0;
	m_ptPos.SetPoint( 0, 0 );
	m_sizeBakc.SetSize( 0, 0 );
	m_fontScoreInput.CreateFont(18,0,0,0,700,0,0,0,0,0,0,0,0,TEXT("宋体"));

	m_lMaxScore = 0;
	m_lMinScore = 0;
	m_lScore = 0;
	m_lDragSite = 0;
	m_lDownSite = 0;

	m_bShowAdd = false;
}

CPlayOperate::~CPlayOperate(void)
{
	m_fontScoreInput.DeleteObject();
}

// 初始化
void CPlayOperate::Initialization( CWnd* pParent )
{
	HINSTANCE hResInstance=AfxGetInstanceHandle();
	m_ImageBack.LoadImage( pParent, hResInstance, TEXT("USER_CONTROL"));
	m_ImageBTBack.LoadImage( pParent, hResInstance, TEXT("USER_CONTROL_BACK"));
	m_ImageScoreNumber.LoadImage( pParent, hResInstance, TEXT("CONTROL_NUMBER"));

	m_ImageDrag.LoadImage( pParent, hResInstance, TEXT("ARROW_CONTROL"));
	m_ImageDragBack.LoadImage( pParent, hResInstance, TEXT("ARROW_CONTROL_BACK"));
	m_ImageDragSelect.LoadImage( pParent, hResInstance, TEXT("ARROW_CONTROL_SELECT"));
	
	m_sizeBakc.SetSize( m_ImageBack.GetWidth(), m_ImageBack.GetHeight() );
	m_sizeDrag.SetSize( m_ImageDrag.GetWidth(), m_ImageDrag.GetHeight() );

	CRect rcCreate(0,0,0,0);
	m_btOneScore.Create(NULL,WS_CHILD,rcCreate,pParent,IDC_ADD_SCORE_ONE);
	m_btTwoScore.Create(NULL,WS_CHILD,rcCreate,pParent,IDC_ADD_SCORE_TWO);
	m_btThreeScore.Create(NULL,WS_CHILD,rcCreate,pParent,IDC_ADD_SCORE_THREE);
	m_btFourScore.Create(NULL,WS_CHILD,rcCreate,pParent,IDC_ADD_SCORE_FOUR);
	m_btAddScore.Create(NULL,WS_CHILD,rcCreate,pParent,IDC_ADD_SCORE);
	m_btFollow.Create(NULL,WS_CHILD,rcCreate,pParent,IDC_CONTROL_FOLLOW);
	m_btGiveUp.Create(NULL,WS_CHILD,rcCreate,pParent,IDC_CONTROL_GIVEUP);
	m_btShowHand.Create(NULL,WS_CHILD,rcCreate,pParent,IDC_CONTROL_SHOWHAND);
	m_btOK.Create(NULL,WS_CHILD,rcCreate,pParent,IDC_CONTROL_OK);

	m_btOneScore.SetButtonImage(IDB_BT_SCORE,hResInstance,false,true);
	m_btTwoScore.SetButtonImage(IDB_BT_SCORE,hResInstance,false,true);
	m_btThreeScore.SetButtonImage(IDB_BT_SCORE,hResInstance,false,true);
	m_btFourScore.SetButtonImage(IDB_BT_SCORE,hResInstance,false,true);
	m_btAddScore.SetButtonImage(IDB_BT_ADD_SCORE,hResInstance,false,true);
	m_btFollow.SetButtonImage(IDB_BT_CONTROL_FOLLOW,hResInstance,false,true);
	m_btGiveUp.SetButtonImage(IDB_BT_CONTROL_GIVEUP,hResInstance,false,true);
	m_btShowHand.SetButtonImage(IDB_BT_CONTROL_SHOWHAND,hResInstance,false,true);
	m_btOK.SetButtonImage(IDB_BT_CONTROL_OK,hResInstance,false,true);

	m_btOneScore.SetFont(&m_fontScoreInput);
	m_btTwoScore.SetFont(&m_fontScoreInput);
	m_btThreeScore.SetFont(&m_fontScoreInput);
	m_btFourScore.SetFont(&m_fontScoreInput);

	m_btOneScore.SetButtonColor(RGB(255, 212, 146));
	m_btTwoScore.SetButtonColor(RGB(255, 212, 146));
	m_btThreeScore.SetButtonColor(RGB(255, 212, 146));
	m_btFourScore.SetButtonColor(RGB(255, 212, 146));
	
}

// 按钮消息
BOOL CPlayOperate::OnCommand( WPARAM wParam, LPARAM lParam )
{
	return FALSE;
}

// 绘画函数
void CPlayOperate::DrawPlayOperate( CDC * pDC, INT nWidth, INT nHeight )
{
	if ( !m_bShow )
		return;

	m_ImageBTBack.DrawImage( pDC, nWidth / 2 - m_ImageBTBack.GetWidth() / 2, m_ptPos.y + m_ImageBack.GetHeight());

	if(m_bShowAdd)
	{
		m_ImageBack.DrawImage( pDC, m_ptPos.x, m_ptPos.y );

		DrawNumber(pDC, &m_ImageScoreNumber, TEXT(",0123456789"), AddComma(m_lScore), m_ptPos.x + 67, m_ptPos.y + 17, DT_CENTER, DT_TOP);

		int nYPos = m_lDragSite + m_sizeDrag.cy / 2;
		int nXPos = m_rectDrag.left + (m_rectDrag.Width() - m_ImageDragSelect.GetWidth()) / 2;
		int nHeightSelect = m_rectDrag.bottom - nYPos - m_sizeDrag.cy / 8;

		//选中
		m_ImageDragSelect.DrawImage(pDC, nXPos, nYPos - 1, m_ImageDragSelect.GetWidth(), nHeightSelect, 0, m_ImageDragSelect.GetHeight() - nHeightSelect, m_ImageDragSelect.GetWidth(), nHeightSelect);

		nHeightSelect = nYPos - m_rectDrag.top;
		m_ImageDragBack.DrawImage(pDC, nXPos, m_rectDrag.top + 2, m_ImageDragBack.GetWidth(), nHeightSelect, 0, 0, m_ImageDragBack.GetWidth(), nHeightSelect);

		m_ImageDrag.DrawImage( pDC, m_rectDrag.left - 4, m_lDragSite);
	}
}

// 添加逗号
CString CPlayOperate::AddComma( LONGLONG lScore )
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

// 绘画数字
void CPlayOperate::DrawNumber( CDC * pDC, CPngImageEx* ImageNumber, TCHAR * szImageNum, LONGLONG lOutNum,INT nXPos, INT nYPos, UINT uFormatX /*= DT_LEFT*/, UINT uFormatY /*= DT_TOP*/)
{
	TCHAR szOutNum[128] = {0};
	_sntprintf(szOutNum,CountArray(szOutNum),TEXT("%I64d"),lOutNum);
	DrawNumber(pDC, ImageNumber, szImageNum, szOutNum, nXPos, nYPos, uFormatX, uFormatY);
}

// 绘画数字
void CPlayOperate::DrawNumber(CDC * pDC , CPngImageEx* ImageNumber, TCHAR * szImageNum, LPCTSTR szOutNum ,INT nXPos, INT nYPos,  UINT uFormatX /*= DT_LEFT*/, UINT uFormatY /*= DT_TOP*/)
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

// 设置位置
void CPlayOperate::SetOperatePos( int nPosX, int nPosY)
{
	m_ptPos.x = nPosX;
	m_ptPos.y = nPosY;

	//按钮控件
	CRect rcButton;
	m_btOneScore.GetWindowRect(&rcButton);

	HDWP hDwp=BeginDeferWindowPos(32);
	const UINT uFlags=SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOCOPYBITS|SWP_NOSIZE;

	
	m_btOK.GetWindowRect(&rcButton);
	int nXPos = nPosX - rcButton.Width() + m_sizeBakc.cx;
	int nYPos = nPosY + m_sizeBakc.cy + 8;
	DeferWindowPos( hDwp, m_btOK, NULL, nXPos, nYPos , 0, 0, uFlags );	

	m_btOneScore.GetWindowRect(&rcButton);

	nXPos -= (rcButton.Width() + 5);
	DeferWindowPos( hDwp,m_btOneScore , NULL, nXPos, nYPos, 0, 0, uFlags );
	
	nXPos -= (rcButton.Width() + 5);
	DeferWindowPos( hDwp, m_btTwoScore,NULL, nXPos, nYPos, 0, 0, uFlags );
	
	nXPos -= (rcButton.Width() + 5);
	DeferWindowPos( hDwp, m_btThreeScore,  NULL, nXPos, nYPos, 0, 0, uFlags );
	
	nXPos -= (rcButton.Width() + 5);	
	DeferWindowPos( hDwp, m_btFourScore,  NULL, nXPos, nYPos, 0, 0, uFlags );	

	nXPos = nPosX; 
	m_btGiveUp.GetWindowRect(&rcButton);
	DeferWindowPos( hDwp, m_btGiveUp,   NULL, nXPos, nYPos , 0, 0, uFlags );
	
	nXPos -= (rcButton.Width() + 5);
	DeferWindowPos( hDwp, m_btShowHand, NULL, nXPos, nYPos , 0, 0, uFlags );
	
	nXPos -= (rcButton.Width() + 5);
	DeferWindowPos( hDwp, m_btFollow,   NULL, nXPos, nYPos , 0, 0, uFlags );	
	
	nXPos -= (rcButton.Width() + 5);
	DeferWindowPos( hDwp, m_btAddScore, NULL, nXPos, nYPos, 0, 0, uFlags );
	
	//结束移动
	EndDeferWindowPos(hDwp);


	CRect rcButtonOver;
	m_btOneScore.GetWindowRect(&rcButtonOver);
	m_lDragSite += rcButtonOver.top - rcButton.top;

	m_rectDrag.SetRect( nPosX + 68, nPosY + 51, nPosX + 96, nPosY + 213 + m_sizeDrag.cy / 8);
}

// 设置信息
void CPlayOperate::SetCellScore( LONG lCellScore )
{
	m_lCellScore = lCellScore;

	CString str;
	str.Format(TEXT("%d"), lCellScore);
	m_btOneScore.SetWindowText(str);

	str.Format(TEXT("%d"), lCellScore * 2);
	m_btTwoScore.SetWindowText(str);

	str.Format(TEXT("%d"), lCellScore * 3);
	m_btThreeScore.SetWindowText(str);

	str.Format(TEXT("%d"), lCellScore * 4);
	m_btFourScore.SetWindowText(str);
}

// 设置最大数
void CPlayOperate::SetBetCount(  LONGLONG lMaxBetCount, LONGLONG lMinBetCount  )
{
	m_lMaxScore = lMaxBetCount;
	m_lMinScore = lMinBetCount;

	if ( m_lMaxScore < 0 )
		m_lMaxScore = 0;

	if ( m_lMinScore < 0 )
		m_lMinScore = 0;

	if ( m_lMinScore > m_lMaxScore )
		m_lMinScore = m_lMaxScore;

	if ( m_lCellScore > lMaxBetCount )
	{
		m_btOneScore.EnableWindow(FALSE);
	}
	else
	{
		CString str;
		str.Format(TEXT("%d"), m_lCellScore);
		m_btOneScore.SetWindowText(str);
		m_btOneScore.EnableWindow(TRUE);
	}
	

	if ( m_lCellScore * 2 > lMaxBetCount )
	{
		m_btTwoScore.EnableWindow(FALSE);
	}
	else
	{
		CString str;
		str.Format(TEXT("%d"), m_lCellScore * 2);
		m_btTwoScore.SetWindowText(str);
		m_btTwoScore.EnableWindow(TRUE);
	}

	if ( m_lCellScore * 3 > lMaxBetCount )
	{
		m_btThreeScore.EnableWindow(FALSE);
	}
	else
	{
		CString str;
		str.Format(TEXT("%d"), m_lCellScore * 3);
		m_btThreeScore.SetWindowText(str);
		m_btThreeScore.EnableWindow(TRUE);
	}

	if ( m_lCellScore * 4 > lMaxBetCount )
	{
		m_btFourScore.EnableWindow(FALSE);
	}
	else
	{
		CString str;
		str.Format(TEXT("%d"), m_lCellScore * 4);
		m_btFourScore.SetWindowText(str);
		m_btFourScore.EnableWindow(TRUE);
	}

	if ( m_lMinScore != 0 )
	{
		m_lScore = m_lMinScore;
	}
}

// 获取下载数量
LONGLONG CPlayOperate::GetBetCount()
{
	if ( m_lScore < m_lMinScore )
	{
		m_lScore = m_lMinScore;
	}	
	return m_lScore;
}

// 获取位置
CRect CPlayOperate::GetOperateRect()
{
	return CRect( m_ptPos.x, m_ptPos.y, m_ptPos.x + m_sizeBakc.cx, m_ptPos.y + m_sizeBakc.cy);
}


// 显示函数
void CPlayOperate::ShowOperate( bool bShow )
{
	m_lScore = 0;
	m_bShow = bShow;

	if ( !m_bShow )
	{
		m_bShowAdd = false;
		m_btOneScore.ShowWindow(SW_HIDE);
		m_btTwoScore.ShowWindow(SW_HIDE);
		m_btThreeScore.ShowWindow(SW_HIDE);
		m_btFourScore.ShowWindow(SW_HIDE);
		m_btAddScore.ShowWindow(SW_HIDE);
		m_btFollow.ShowWindow(SW_HIDE);
		m_btGiveUp.ShowWindow(SW_HIDE);
		m_btShowHand.ShowWindow(SW_HIDE);
		m_btOK.ShowWindow(SW_HIDE);
	}
	else
	{
// 		m_btOK.ShowWindow(SW_SHOW);
// 		m_btOneScore.ShowWindow(SW_SHOW);
// 		m_btTwoScore.ShowWindow(SW_SHOW);
// 		m_btThreeScore.ShowWindow(SW_SHOW);
// 		m_btFourScore.ShowWindow(SW_SHOW);
		m_lDragSite = m_rectDrag.bottom - m_sizeDrag.cy + m_sizeDrag.cy / 8 ;
		m_lDownSite = -m_sizeDrag.cy / 8;
	}
}

// 显示函数
void CPlayOperate::ShowOperateAdd( bool bShow )
{
	if(bShow)
	{
		m_bShowAdd = true;
		m_btOK.ShowWindow(SW_SHOW);
		m_btOneScore.ShowWindow(SW_SHOW);
		m_btTwoScore.ShowWindow(SW_SHOW);
		m_btThreeScore.ShowWindow(SW_SHOW);
		m_btFourScore.ShowWindow(SW_SHOW);
		m_btAddScore.ShowWindow(SW_HIDE);
		m_btFollow.ShowWindow(SW_HIDE);
		m_btGiveUp.ShowWindow(SW_HIDE);
		m_btShowHand.ShowWindow(SW_HIDE);
		m_lDragSite = m_rectDrag.bottom - m_sizeDrag.cy + m_sizeDrag.cy / 8 ;
		m_lDownSite = -m_sizeDrag.cy / 8;
	}
}

BOOL CPlayOperate::PreTranslateMessage( MSG * pMsg )
{
	if ( pMsg->message == WM_LBUTTONDOWN )
	{
		CPoint ptMouse(LOWORD(pMsg->lParam) , HIWORD(pMsg->lParam));
		CRect rc(m_rectDrag.left, m_lDragSite, m_rectDrag.right, m_lDragSite + m_sizeDrag.cy);
		//rc.OffsetRect(0, m_lDownSite);
		if( rc.PtInRect(ptMouse) )
		{			
			m_bMouseDown = true;
			m_lDownSite = ptMouse.y - m_lDragSite;
			SetEditScore();
			m_ptMouse = ptMouse;
			return TRUE;
		}
	}
	else if ( pMsg->message == WM_MOUSEMOVE )
	{
		if ( m_bMouseDown )
		{
			CPoint ptMouse(LOWORD(pMsg->lParam) , HIWORD(pMsg->lParam));

			bool bUp = false;
			//向上
			if(m_ptMouse.y < ptMouse.y)
			{
				bUp = true;
				if(ptMouse.y - m_lDownSite <= m_rectDrag.top)
				{
					m_lDownSite = __max(-m_sizeDrag.cy / 8, ptMouse.y - m_rectDrag.top);
				}
			}
			else
			{
				if(ptMouse.y - m_lDownSite >= m_rectDrag.bottom)
				{
					m_lDownSite = __min(m_sizeDrag.cy - m_sizeDrag.cy / 8, ptMouse.y - m_rectDrag.bottom);
				}
			}

			CRect rc = m_rectDrag;
			rc.OffsetRect(0, m_lDownSite);
			
			if (rc.PtInRect(ptMouse) )
			{
				m_lDragSite = (ptMouse.y - m_lDownSite);

				if(m_lDragSite < m_rectDrag.top - m_sizeDrag.cy / 8)
				{
					m_lDragSite = m_rectDrag.top - m_sizeDrag.cy / 8;
				}

 				if(m_lDragSite >= m_rectDrag.bottom - m_sizeDrag.cy + m_sizeDrag.cy / 8)
 				{
 					m_lDragSite = m_rectDrag.bottom - m_sizeDrag.cy + m_sizeDrag.cy / 8;
 				}
				SetEditScore();
				m_ptMouse = ptMouse;
				return TRUE;
			}
			rc = m_rectDrag;
			rc.OffsetRect(0, -m_sizeDrag.cy / 8);
			if(rc.PtInRect(ptMouse))
			{
				m_lDragSite = (ptMouse.y - m_lDownSite);

				if(m_lDragSite < m_rectDrag.top - m_sizeDrag.cy / 8)
				{
					m_lDragSite = m_rectDrag.top - m_sizeDrag.cy / 8;
					m_lDownSite = max(-m_sizeDrag.cy / 8, ptMouse.y - m_lDragSite);
				}
				SetEditScore();
				m_ptMouse = ptMouse;
				return TRUE;
			}
		}
	}
	else if ( pMsg->message == WM_LBUTTONUP || pMsg->message == WM_KILLFOCUS )
	{
		m_bMouseDown = false;
	}
	
	return FALSE;
}

// 设置积分
void CPlayOperate::SetEditScore()
{
	int nHeight = m_rectDrag.Height() - m_sizeDrag.cy * 3 / 4;
	int nOffSet = m_sizeDrag.cy / 8;
	int nDragY = m_lDragSite;
	if ( nDragY < m_rectDrag.top - nOffSet)
		nDragY = m_rectDrag.top - nOffSet;
	else if ( nDragY > m_rectDrag.bottom - m_sizeDrag.cy + nOffSet)
		nDragY = m_rectDrag.bottom - m_sizeDrag.cy + nOffSet;

	nDragY -= (m_rectDrag.top - nOffSet);

	m_lScore = m_lMaxScore - (SCORE)((m_lMaxScore - m_lMinScore) * ( (DOUBLE)nDragY/(DOUBLE)nHeight ));
}

// 设置积分
void CPlayOperate::SetDragScore()
{
	if ( m_lScore < m_lMinScore )
	{
		m_lScore = m_lMinScore;
	}	

	if ( m_lScore > m_lMaxScore )
	{
		m_lScore = m_lMaxScore;
	}	

	int nHeight = m_rectDrag.Height() - m_sizeDrag.cy * 3 / 4;
	int nDragY = (int)( ((DOUBLE)m_lScore / (DOUBLE)(m_lMaxScore - m_lMinScore)) * nHeight);
	
	nDragY += m_rectDrag.top + m_sizeDrag.cy / 2;
	m_lDragSite = nDragY;
}

// 设置积分
void CPlayOperate::ScoreADD()
{
	if( m_lScore < m_lMaxScore )
		m_lScore++;
	
	SetDragScore();
}


// 设置积分
void CPlayOperate::ScoreSUB()
{
	if( m_lScore > 0 )
		m_lScore--;

	SetDragScore();
}
// 添加逗号
CString CPlayOperate::AddComma( CString strScore )
{
	CString strReturn;
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

// 删除逗号
LONGLONG CPlayOperate::DeleteComma( CString strScore )
{
	LONGLONG lScore = 0l;
	strScore.Remove(',');
	if ( !strScore.IsEmpty() )
		lScore = _ttoi64(strScore);

	return lScore;
}





