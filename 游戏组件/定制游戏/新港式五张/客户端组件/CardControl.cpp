#include "StdAfx.h"
#include "GameClient.h"
#include "CardControl.h"

//////////////////////////////////////////////////////////////////////////
//�궨��

#define INVALID_ITEM				0xFFFF								//��Ч����

//////////////////////////////////////////////////////////////////////////

//���캯��
CCardControl::CCardControl()
{
	//״̬����
	m_bPositively=false;
	m_bDisplayHead=false;
	m_nXDistance=DEF_X_DISTANCE;

	//�˿�����
	m_wCardCount=0;
	ZeroMemory(m_cbCardData,sizeof(m_cbCardData));

	//λ�ñ���
	m_YCollocateMode=enYTop;
	m_XCollocateMode=enXLeft;
	m_BenchmarkPos.SetPoint(0,0);

	return;
}

//��������
CCardControl::~CCardControl()
{
}

//��ʼ��
VOID CCardControl::Initialize(bool bSmall)
{
	//������Դ
	if( bSmall )
	{
		m_ImageCard.LoadImage(AfxGetInstanceHandle(), TEXT("GAME_CARD_MID"));
		m_nXDistance = 17;
	}
	else
	{
		m_ImageCard.LoadImage(AfxGetInstanceHandle(), TEXT("GAME_CARD"));
	}

	//��ȡ��С
	m_CardSize.SetSize(m_ImageCard.GetWidth()/13,m_ImageCard.GetHeight()/5);
}
//��ȡ�˿�
WORD CCardControl::GetCardData(BYTE cbCardData[], WORD wBufferCount)
{
	//Ч�����
	ASSERT(wBufferCount>=m_wCardCount);
	if (wBufferCount<m_wCardCount) return 0;

	//�����˿�
	CopyMemory(cbCardData,m_cbCardData,sizeof(BYTE)*m_wCardCount);

	return m_wCardCount;
}

//�����˿�
bool CCardControl::SetCardData(const BYTE cbCardData[], WORD wCardCount)
{
	//Ч�����
	ASSERT(wCardCount<=CountArray(m_cbCardData));
	if (wCardCount>CountArray(m_cbCardData)) return false;

	//���ñ���
	m_wCardCount=wCardCount;
	CopyMemory(m_cbCardData,cbCardData,sizeof(BYTE)*m_wCardCount);

	return true;
}

//��׼λ��
VOID CCardControl::SetBenchmarkPos(INT nXPos, INT nYPos, enXCollocateMode XCollocateMode, enYCollocateMode YCollocateMode)
{
	//���ñ���
	m_BenchmarkPos.x=nXPos;
	m_BenchmarkPos.y=nYPos;
	m_XCollocateMode=XCollocateMode;
	m_YCollocateMode=YCollocateMode;

	return;
}

//��׼λ��
VOID CCardControl::SetBenchmarkPos(const CPoint & BenchmarkPos, enXCollocateMode XCollocateMode, enYCollocateMode YCollocateMode)
{
	//���ñ���
	m_BenchmarkPos=BenchmarkPos;
	m_XCollocateMode=XCollocateMode;
	m_YCollocateMode=YCollocateMode;

	return;
}

//�滭�˿�
VOID CCardControl::DrawCardControl(CDC * pDC)
{
	//��ȡλ��
	CPoint OriginPoint;
	GetOriginPoint(OriginPoint);

	//�滭�˿�
	for (WORD i=0;i<m_wCardCount;i++)
	{
		//��������
		INT nXImagePos=0,nYImagePos=0;
		BYTE cbCardData=m_cbCardData[i];

		//ͼƬλ��
		if ((cbCardData!=0)&&(cbCardData!=0xFF)&&((i>0)||(m_bDisplayHead==true)))
		{
			nXImagePos=((cbCardData&CARD_MASK_VALUE)-1)*m_CardSize.cx;
			nYImagePos=((cbCardData&CARD_MASK_COLOR)>>4)*m_CardSize.cy;
		}
		else
		{
			nXImagePos=m_CardSize.cx*2;
			nYImagePos=m_CardSize.cy*4;
		}

		//�滭�˿�
		m_ImageCard.DrawImage(pDC,OriginPoint.x+m_nXDistance*i,OriginPoint.y,m_CardSize.cx,m_CardSize.cy,nXImagePos,nYImagePos);
	}

	return;
}

//�����Ϣ
bool CCardControl::OnEventSetCursor(CPoint Point)
{
	//��괦��
	if (m_bPositively==true)
	{
		//��ȡ����
		WORD wHoverItem=SwitchCardPoint(Point);

		//�����ж�
		if (wHoverItem==0)
		{
			SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_CARD_CUR)));
			return true;
		}
	}

	return false;
}

//�����Ϣ
bool CCardControl::OnEventLeftMouseDown(CPoint Point)
{
	//��괦��
	if ((m_bPositively==true)&&(SwitchCardPoint(Point)==0))
	{
		//���ñ���
		m_bDisplayHead=!m_bDisplayHead;

		return true;
	}

	return false;
}

//��ȡ��С
VOID CCardControl::GetControlSize(CSize & ControlSize)
{
	//��ȡ��С
	ControlSize.cy=m_CardSize.cy;
	ControlSize.cx=(m_wCardCount>0)?(m_CardSize.cx+(m_wCardCount-1)*m_nXDistance):0;

	return;
}

//�����л�
WORD CCardControl::SwitchCardPoint(CPoint & MousePoint)
{
	//��ȡλ��
	CSize ControlSize;
	CPoint OriginPoint;
	GetControlSize(ControlSize);
	GetOriginPoint(OriginPoint);

	//��׼λ��
	INT nXPos=MousePoint.x-OriginPoint.x;
	INT nYPos=MousePoint.y-OriginPoint.y;

	//Խ���ж�
	if ((nXPos<=0)||(nXPos>=ControlSize.cx)) return INVALID_ITEM;
	if ((nYPos<=0)||(nYPos>=ControlSize.cy)) return INVALID_ITEM;

	//��������
	WORD wCardIndex=nXPos/m_nXDistance;
	if (wCardIndex>=m_wCardCount) wCardIndex=(m_wCardCount-1);

	return wCardIndex;
}

//��ȡԭ��
VOID CCardControl::GetOriginPoint(CPoint & OriginPoint)
{
	//��ȡλ��
	CSize ControlSize;
	GetControlSize(ControlSize);

	//����λ��
	switch (m_XCollocateMode)
	{
	case enXLeft:	{ OriginPoint.x=m_BenchmarkPos.x; break; }
	case enXCenter: { OriginPoint.x=m_BenchmarkPos.x-ControlSize.cx/2; break; }
	case enXRight:	{ OriginPoint.x=m_BenchmarkPos.x-ControlSize.cx; break; }
	}

	//����λ��
	switch (m_YCollocateMode)
	{
	case enYTop:	{ OriginPoint.y=m_BenchmarkPos.y; break; }
	case enYCenter: { OriginPoint.y=m_BenchmarkPos.y-ControlSize.cy/2; break; }
	case enYBottom: { OriginPoint.y=m_BenchmarkPos.y-ControlSize.cy; break; }
	}

	return;
}

//��ȡ��βλ��
CPoint CCardControl::GetTailPos()
{
	CPoint ptTail;

	//�����С
	INT nSpaceWidth = m_wCardCount*m_nXDistance;
	CSize ControlSize;
	GetControlSize(ControlSize);

	//����λ��
	switch (m_XCollocateMode)
	{
	case enXLeft:	{ ptTail.x=m_BenchmarkPos.x+nSpaceWidth; break; }
	case enXCenter: { ptTail.x=m_BenchmarkPos.x-ControlSize.cx/2+nSpaceWidth; break; }
	case enXRight:	{ ptTail.x=m_BenchmarkPos.x-ControlSize.cx+nSpaceWidth; break; }
	}

	//����λ��
	switch (m_YCollocateMode)
	{
	case enYTop:	{ ptTail.y=m_BenchmarkPos.y; break; }
	case enYCenter: { ptTail.y=m_BenchmarkPos.y-ControlSize.cy/2; break; }
	case enYBottom: { ptTail.y=m_BenchmarkPos.y-ControlSize.cy; break; }
	}

	return ptTail;
}

//��ȡ���ĵ�
VOID CCardControl::GetCenterPoint( CPoint & CenterPoint )
{
	//��ȡλ��
	CSize ControlSize;
	GetControlSize(ControlSize);

	//����λ��
	switch (m_XCollocateMode)
	{
	case enXLeft:	{ CenterPoint.x=m_BenchmarkPos.x + ControlSize.cx/2; break; }
	case enXCenter: { CenterPoint.x=m_BenchmarkPos.x; break; }
	case enXRight:	{ CenterPoint.x=m_BenchmarkPos.x - ControlSize.cx/2; break; }
	}

	//����λ��
	switch (m_YCollocateMode)
	{
	case enYTop:	{ CenterPoint.y=m_BenchmarkPos.y + ControlSize.cy/2; break; }
	case enYCenter: { CenterPoint.y=m_BenchmarkPos.y ; break; }
	case enYBottom: { CenterPoint.y=m_BenchmarkPos.y - ControlSize.cy/2; break; }
	}

	return;
}

//////////////////////////////////////////////////////////////////////////
