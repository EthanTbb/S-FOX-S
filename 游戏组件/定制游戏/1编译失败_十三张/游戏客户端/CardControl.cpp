#include "StdAfx.h"
#include "GameClient.h"
#include "CardControl.h"

//////////////////////////////////////////////////////////////////////////
bool					CCardControl::m_bLoad=false;					//���ر�ʶ
CSize					CCardControl::m_CardSize = CSize(0, 0);			//�˿˴�С
CPngImage				CCardControl::m_ImageCard;						//ͼƬ��Դ
CPngImage				CCardControl::m_ImageHorCard;					//������ͼ
//////////////////////////////////////////////////////////////////////////

//���캯��
CCardControl::CCardControl()
{
	//״̬����
	m_bSmallMode=false;
	m_bHorizontal=true;
	m_bPositively=false;
	m_bDisplayItem=false;
	m_bShowControl=true;

	//ѡ�����
	m_wEndIndex=INVALID_ITEM;
	m_wStartIndex=INVALID_ITEM;

	//�˿�����
	m_wMaxSize = 52;
	m_wCardCount=0;
	ZeroMemory(m_CardItemArray,sizeof(m_CardItemArray));

	//λ�ñ���
	m_YCollocateMode=enYTop;
	m_XCollocateMode=enXLeft;
	m_BenchmarkPos.SetPoint(0,0);

	//�������
	m_nXDistance=(m_bSmallMode==false)?DEF_X_DISTANCE:DEF_X_DISTANCE_SMALL;
	m_nYDistance=(m_bSmallMode==false)?DEF_Y_DISTANCE:DEF_Y_DISTANCE_SMALL;
	m_nShootDistance=(m_bSmallMode==false)?DEF_SHOOT_DISTANCE:DEF_SHOOT_DISTANCE_SMALL;

	return;
}

//��������
CCardControl::~CCardControl()
{
}

BYTE CCardControl::GetCardLogicValue(BYTE cbCardData)
{
	//�˿�����
	BYTE cbReturn=0;
	BYTE cbCardColor=CARD_MASK_COLOR&(cbCardData);
	BYTE cbCardValue=CARD_MASK_VALUE&(cbCardData);

	if(cbCardColor==0x40)
		cbReturn=cbCardValue+2; 
	else
		cbReturn=(cbCardValue<=2)?(cbCardValue+13):cbCardValue;
	//ת����ֵ
	return cbReturn;
}


//�����˿�
bool CCardControl::SetCardData(WORD wCardCount)
{
	//Ч�����
	ASSERT(wCardCount<=m_wMaxSize);
	if (wCardCount>m_wMaxSize) return false;

	//���ñ���
	m_wCardCount=wCardCount;
	ZeroMemory(m_CardItemArray,sizeof(m_CardItemArray));

	return true;
}



//����ģʽ
bool CCardControl::SetCardMode(bool bSmallMode)
{
	//����ģʽ
	if (m_bSmallMode!=bSmallMode)
	{
		//���ñ���
		m_bSmallMode=bSmallMode;

		//���ü��
		m_nXDistance=(m_bSmallMode==false)?DEF_X_DISTANCE:DEF_X_DISTANCE_SMALL;
		m_nYDistance=(m_bSmallMode==false)?DEF_Y_DISTANCE:DEF_Y_DISTANCE_SMALL;
		m_nShootDistance=(m_bSmallMode==false)?DEF_SHOOT_DISTANCE:DEF_SHOOT_DISTANCE_SMALL;
	}

	return true;
}

//�����˿�
bool CCardControl::SetCardData(const BYTE cbCardData[], WORD wCardCount)
{
	//Ч�����
	ASSERT(wCardCount<=m_wMaxSize);
	if (wCardCount>m_wMaxSize) return false;

	//���ñ���
	m_wEndIndex=INVALID_ITEM;
	m_wStartIndex=INVALID_ITEM;

	//�˿���Ŀ
	m_wCardCount=wCardCount;

	//�����˿�
	for (WORD i=0;i<wCardCount;i++)
	{
		m_CardItemArray[i].bShoot=false;
		m_CardItemArray[i].cbCardData=cbCardData[i];
	}

	return true;
}

//�����˿�
bool CCardControl::SetCardItem(const tagCardItem CardItemArray[], WORD wCardCount)
{
	//Ч�����
	ASSERT(wCardCount<=m_wMaxSize);
	if (wCardCount>m_wMaxSize) return false;

	//���ñ���
	m_wEndIndex=INVALID_ITEM;
	m_wStartIndex=INVALID_ITEM;

	//�����˿�
	m_wCardCount=wCardCount;
	CopyMemory(m_CardItemArray,CardItemArray,wCardCount*sizeof(tagCardItem));

	return true;
}

//�����˿�
bool CCardControl::SetCardItem(const BYTE cbCardData, WORD wIndex)
{
	ASSERT((wIndex >= 0) && (wIndex < m_wCardCount));
	if ((wIndex < 0) || (wIndex >= m_wCardCount)) return false;

	m_CardItemArray[wIndex].cbCardData = cbCardData;
	return true;
}

//�����˿�
bool CCardControl::SetShootIndex(WORD wStartIndex, WORD wEndIndex)
{
	//���ñ���
	m_wEndIndex=wEndIndex;
	m_wStartIndex=wStartIndex;

	//�����˿�
	for (WORD i=0;i<m_wCardCount;i++)
	{
		if ((i>=wStartIndex)&&(i<=wEndIndex))
		{
			m_CardItemArray[i].bShoot=!m_CardItemArray[i].bShoot;
		}
	}

	return true;
}

//�����˿�
bool CCardControl::SetShootByIndex(const BYTE bCardDataIndex[], WORD wCardCount)
{
	//������֤
	ASSERT((wCardCount > 0) && (wCardCount<= m_wMaxSize));
	if ((wCardCount <= 0) || (wCardCount > m_wMaxSize)) return false;

	for(WORD i=0;i<wCardCount;i++)
	{
		tagCardItem * pCardItem=&m_CardItemArray[bCardDataIndex[i]];
		pCardItem->bShoot=true;
	}
	return true;
}

//�����˿�
bool CCardControl::SetSelectIndex(WORD wStartIndex, WORD wEndIndex)
{
	//���ñ���
	m_wEndIndex=wEndIndex;
	m_wStartIndex=wStartIndex;

	return true;
}

//�����˿�
bool CCardControl::SetShootCard(const BYTE cbCardData[], WORD wCardCount)
{
	//��������
	bool bChangeStatus=false;

	//�����˿�
	for (WORD i=0;i<m_wCardCount;i++) 
	{
		if (m_CardItemArray[i].bShoot==true)
		{
			bChangeStatus=true;
			m_CardItemArray[i].bShoot=false;
		}
	}

	//�����˿�
	for (WORD i=0;i<wCardCount;i++)
	{
		for (WORD j=0;j<m_wCardCount;j++)
		{
			if ((m_CardItemArray[j].bShoot==false)&&(m_CardItemArray[j].cbCardData==cbCardData[i])) 
			{
				bChangeStatus=true;
				m_CardItemArray[j].bShoot=true;
				break;
			}
		}
	}

	return bChangeStatus;
}

//ɾ���˿�
bool CCardControl::RemoveShootItem()
{
	//���ñ���
	m_wEndIndex=INVALID_ITEM;
	m_wStartIndex=INVALID_ITEM;

	//�����˿�
	tagCardItem	CardItemArray[52];
	CopyMemory(CardItemArray,m_CardItemArray,sizeof(tagCardItem)*m_wCardCount);

	//ɾ���˿�
	WORD wRemoveCount=0;
	for (WORD i=0;i<m_wCardCount;i++)
	{
		if (CardItemArray[i].bShoot==true)
		{
			wRemoveCount++;
			CardItemArray[i].cbCardData=0x00;
		}
	}

	//�����˿�
	if (wRemoveCount>0)
	{
		//�����˿�
		WORD wInsertCount=0;
		for (WORD i=0;i<m_wCardCount;i++)
		{
			if (CardItemArray[i].cbCardData!=0x00)
			{
				m_CardItemArray[wInsertCount++]=CardItemArray[i];
			}
		}

		//���ñ���
		m_wCardCount=m_wCardCount-wRemoveCount;
	}

	return true;
}

//��ȡ�˿�
tagCardItem * CCardControl::GetCardFromIndex(WORD wIndex)
{
	return (wIndex<m_wCardCount)?&m_CardItemArray[wIndex]:NULL;
}

//��ȡ�˿�
tagCardItem * CCardControl::GetCardFromPoint(CPoint & MousePoint)
{
	WORD wIndex=SwitchCardPoint(MousePoint);
	return (wIndex!=INVALID_ITEM)?&m_CardItemArray[wIndex]:NULL;
}

//��ȡ�˿�
WORD CCardControl::GetCardData(BYTE cbCardData[], WORD wBufferCount)
{
	//Ч�����
	ASSERT(wBufferCount>=m_wCardCount);
	if (wBufferCount<m_wCardCount) return 0;

	//�����˿�
	for (WORD i=0;i<m_wCardCount;i++) cbCardData[i]=m_CardItemArray[i].cbCardData;

	return m_wCardCount;
}

//��ȡ�˿�
BYTE CCardControl::GetCardData(WORD wIndex)
{
	ASSERT((wIndex>=0) && (wIndex < m_wCardCount));
	if ((wIndex<0) || (wIndex >= m_wCardCount)) return 0;

	return m_CardItemArray[wIndex].cbCardData;
}

//��ȡ�˿�
WORD CCardControl::GetShootCard(BYTE cbCardData[], WORD wBufferCount)
{
	//��������
	WORD wShootCount=0;

	//�����˿�
	for (WORD i=0;i<m_wCardCount;i++) 
	{
		//Ч�����
		ASSERT(wBufferCount>wShootCount);
		if (wBufferCount<=wShootCount) break;

		//�����˿�
		if (m_CardItemArray[i].bShoot==true) cbCardData[wShootCount++]=m_CardItemArray[i].cbCardData;
	}

	return wShootCount;
}

//���þ���
VOID CCardControl::SetCardSpace(UINT nXDistance, UINT nYDistance, UINT nShootDistance)
{
	//���ñ���
	m_nXDistance=nXDistance;
	m_nYDistance=nYDistance;
	m_nShootDistance=nShootDistance;

	return;
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
	//��ʾ�ж�
	if (m_bShowControl==false) return;
	if (m_wCardCount <= 0) return;

	//��ȡλ��
	CPoint OriginPoint;
	GetOriginPoint(OriginPoint);

	//��������
	INT nXDrawPos=0,nYDrawPos=0;
	INT nXImagePos=0,nYImagePos=0;

	//�滭�˿�
	for (WORD i=0;i<m_wCardCount;i++)
	{
		//��ȡ�˿�
		bool bShoot=m_CardItemArray[i].bShoot;
		BYTE cbCardData=m_CardItemArray[i].cbCardData;

		//��϶����
		if (cbCardData==SPACE_CARD_DATA) continue;

		//ͼƬλ��
		if ((m_bDisplayItem==true)&&(cbCardData!=0))
		{
			if ((cbCardData==0x4E)||(cbCardData==0x4F))
			{
				nXImagePos=((cbCardData&CARD_MASK_VALUE)%14)*m_CardSize.cx;
				nYImagePos=((cbCardData&CARD_MASK_COLOR)>>4)*m_CardSize.cy;
			}
			else
			{
				nXImagePos=((cbCardData&CARD_MASK_VALUE)-1)*m_CardSize.cx;
				nYImagePos=((cbCardData&CARD_MASK_COLOR)>>4)*m_CardSize.cy;
			}
		}
		else
		{
			nXImagePos=m_CardSize.cx*2;
			nYImagePos=m_CardSize.cy*4;
		}

		//��Ļλ��
		if (m_bHorizontal==true)
		{
			nXDrawPos=m_nXDistance*i;
			nYDrawPos=(bShoot==false)?m_nShootDistance:0;
		}
		else
		{
			nXDrawPos=0;
			nYDrawPos=m_nYDistance*i;
		}

		//�滭�˿�
		if ((m_bHorizontal == false) && (m_bDisplayItem == false))
		{
			m_ImageHorCard.DrawImage(pDC, OriginPoint.x+nXDrawPos, OriginPoint.y+nYDrawPos);
		}
		else
		{
			m_ImageCard.DrawImage(pDC,OriginPoint.x+nXDrawPos,OriginPoint.y+nYDrawPos,m_CardSize.cx,m_CardSize.cy,nXImagePos,nYImagePos);
		}

		//�滭ѡ��
		if ((m_wStartIndex<=i)&&(i<=m_wEndIndex))
		{
			m_ImageCard.DrawImage(pDC,OriginPoint.x+nXDrawPos,OriginPoint.y+nYDrawPos,m_CardSize.cx,m_CardSize.cy, m_CardSize.cx*12, m_CardSize.cy*4, m_CardSize.cx,m_CardSize.cy);
		}
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
		if (wHoverItem!=INVALID_ITEM)
		{
			SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_CARD_CUR)));
			return true;
		}
	}

	return false;
}

//��ȡ������С
VOID CCardControl::GetCardSize(CSize & CardSize)
{
	CardSize = m_CardSize;
	return;
}

//��ȡ��С
VOID CCardControl::GetControlSize(CSize & ControlSize)
{
	//��ȡ��С
	if (m_bHorizontal==true)
	{
		ControlSize.cy=m_CardSize.cy+m_nShootDistance;
		ControlSize.cx=(m_wCardCount>0)?(m_CardSize.cx+(m_wCardCount-1)*m_nXDistance):0;
	}
	else
	{
		if (m_bDisplayItem)
		{
			ControlSize.cx=m_CardSize.cx;
			ControlSize.cy=(m_wCardCount>0)?(m_CardSize.cy+(m_wCardCount-1)*m_nYDistance):0;
		}
		else
		{
			ControlSize.cx=m_ImageHorCard.GetWidth();
			ControlSize.cy=(m_wCardCount>0)?(m_ImageHorCard.GetHeight()+(m_wCardCount-1)*m_nYDistance):0;
		}
	}

	return;
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

//��ȡβ��
VOID CCardControl::GetTailPoint(CPoint  & TailPoint)
{
	//��ȡ�ߴ�
	CSize ControlSize;
	GetControlSize(ControlSize);

	//����λ��
	switch (m_XCollocateMode)
	{
	case enXLeft:	{ TailPoint.x=m_BenchmarkPos.x+ControlSize.cx; break; }
	case enXCenter: { TailPoint.x=m_BenchmarkPos.x+ControlSize.cx/2; break; }
	case enXRight:	{ TailPoint.x=m_BenchmarkPos.x; break; }
	}

	//����λ��
	switch (m_YCollocateMode)
	{
	case enYTop:	{ TailPoint.y=m_BenchmarkPos.y+ControlSize.cy/2; break; }
	case enYCenter: { TailPoint.y=m_BenchmarkPos.y; break; }
	case enYBottom: { TailPoint.y=m_BenchmarkPos.y-ControlSize.cy; break; }
	}

	return;
}

//��ȡ����
VOID CCardControl::GetCenterPoint(CPoint & CenterPoint)
{
	//��ȡԭ��
	CPoint OriginPoint;
	GetOriginPoint(OriginPoint);

	//��ȡλ��
	CSize ControlSize;
	GetControlSize(ControlSize);

	//��������
	CenterPoint.x=OriginPoint.x+ControlSize.cx/2;
	CenterPoint.y=OriginPoint.y+ControlSize.cy/2;

	return;
}

//�����л�
WORD CCardControl::SwitchCardPoint(CPoint & MousePoint)
{
	//����ģʽ
	if ((m_bHorizontal==true)&&(m_wCardCount>0))
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
		if ((nXPos<0)||(nXPos>ControlSize.cx)) return INVALID_ITEM;
		if ((nYPos<0)||(nYPos>ControlSize.cy)) return INVALID_ITEM;

		//��������
		WORD wCardIndex=nXPos/m_nXDistance;
		if (wCardIndex>=m_wCardCount) wCardIndex=(m_wCardCount-1);

		//�˿�����
		for (WORD i=0;i<=wCardIndex;i++)
		{
			//��������
			WORD wCurrentIndex=wCardIndex-i;

			//�������
			if (nXPos>(INT)(wCurrentIndex*m_nXDistance+m_CardSize.cx)) break;

			//�������
			bool bShoot=m_CardItemArray[wCurrentIndex].bShoot;
			if ((bShoot==true)&&(nYPos<=m_CardSize.cy)) return wCurrentIndex;
			if ((bShoot==false)&&(nYPos>=(INT)m_nShootDistance)) return wCurrentIndex;
		}
	}

	return INVALID_ITEM;
}

//�ƶ��˿�
bool CCardControl::MoveCardItem(WORD wTargerItem)
{
	//Ч�����
	ASSERT(wTargerItem<m_wCardCount);
	if (wTargerItem>=m_wCardCount) return false;

	//ͳ���˿�
	WORD wShootCount=0;
	for (WORD i=0;i<m_wCardCount;i++)
	{
		if (m_CardItemArray[i].bShoot==true)
		{
			wShootCount++;
		}
	}

	//�ƶ��˿�
	if ((wShootCount>0)&&(wShootCount<m_wCardCount))
	{
		//��������
		wTargerItem=__min(m_wCardCount-wShootCount,wTargerItem);

		//��������
		WORD wShootIndex=wTargerItem;
		WORD wNormalIndex=(wTargerItem==0)?(wTargerItem+wShootCount):0;

		//��ȡ�˿�
		tagCardItem CardItemTemp[52];
		CopyMemory(CardItemTemp,m_CardItemArray,sizeof(tagCardItem)*m_wCardCount);

		//�����˿�
		for (WORD i=0;i<m_wCardCount;i++)
		{
			if (CardItemTemp[i].bShoot==false)
			{
				//�����˿�
				m_CardItemArray[wNormalIndex++]=CardItemTemp[i];

				//��������
				if (wNormalIndex==wTargerItem) wNormalIndex=wTargerItem+wShootCount;
			}
			else
			{
				//�����˿�
				m_CardItemArray[wShootIndex++]=CardItemTemp[i];
			}
		}

		return true;
	}

	return false;
}

//�����˿�
bool CCardControl::SwitchCardItem(WORD wSourceItem, WORD wTargerItem)
{
	//Ч�����
	if (wSourceItem==wTargerItem) return false;
	if ((wSourceItem>=m_wCardCount)||(wTargerItem>=m_wCardCount)) return false;

	//�����˿�
	tagCardItem CardItem=m_CardItemArray[wSourceItem];

	//�ƶ��˿�
	if (wSourceItem>wTargerItem)
	{
		WORD wMoveCount=wSourceItem-wTargerItem;
		MoveMemory(&m_CardItemArray[wTargerItem+1],&m_CardItemArray[wTargerItem],sizeof(tagCardItem)*wMoveCount);
	}
	else
	{
		WORD wMoveCount=wTargerItem-wSourceItem;
		MoveMemory(&m_CardItemArray[wSourceItem],&m_CardItemArray[wSourceItem+1],sizeof(tagCardItem)*wMoveCount);
	}

	//����Ŀ��
	m_CardItemArray[wTargerItem]=CardItem;

	return true;
}

//�����˿�
void CCardControl::ShootAllCard(bool bShoot)
{
	if (bShoot==true)
	{
		SetShootIndex(0,m_wCardCount-1);
	}
	else
	{
		for (WORD i=0;i<m_wCardCount;i++)
		{
			m_CardItemArray[i].bShoot=false;
		}
	}

	
}

//������Դ
void CCardControl::Load()
{
	if (m_bLoad == true) return;

	//���ı�ʶ
	m_bLoad = true;	
	
	//������Դ
	HINSTANCE hResInstance=AfxGetInstanceHandle();
	m_ImageCard.LoadImage(hResInstance,TEXT("GAME_CARD"));
	m_ImageHorCard.LoadImage(hResInstance, TEXT("HOR_CARD"));

	//��ȡ��С
	m_CardSize.SetSize(m_ImageCard.GetWidth()/13,m_ImageCard.GetHeight()/5);
}
//////////////////////////////////////////////////////////////////////////

