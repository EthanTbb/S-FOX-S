#include "StdAfx.h"
#include "Resource.h"
#include "MulsecCardControlEx.h"

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
CMulSecCardControlEx::CMulSecCardControlEx(void)
{
}

CMulSecCardControlEx::~CMulSecCardControlEx(void)
{
}

//绘画扑克
VOID CMulSecCardControlEx::DrawCardControl(CDC * pDC, INT nLayer/* = -1*/)
{
	if (nLayer == 0)
	{
		m_PerCardData[1].DrawCardControl(pDC);
		m_PerCardData[2].DrawCardControl(pDC);
		m_PerCardData[0].DrawCardControl(pDC);
	}
	else if (nLayer == 1)
	{
		m_PerCardData[0].DrawCardControl(pDC);
		m_PerCardData[2].DrawCardControl(pDC);
		m_PerCardData[1].DrawCardControl(pDC);
	}
	else
	{
		m_PerCardData[0].DrawCardControl(pDC);
		m_PerCardData[1].DrawCardControl(pDC);
		m_PerCardData[2].DrawCardControl(pDC);
	}
}

void CMulSecCardControlEx::ShowWindow(UINT nShowCmd, int nSecIndex)
{
	if (nSecIndex != -1)
		m_PerCardData[nSecIndex].ShowWindow(nShowCmd);
	else
	{
		for (int i = 0; i < MSCC_SECTION_NUM; i++)
		{
			m_PerCardData[i].ShowWindow(nShowCmd);
		}
	}
}

//设置显示
void CMulSecCardControlEx::SetDisplayFlag(bool bDisplay, int nSecIndex)
{
	if (nSecIndex != -1)
		m_PerCardData[nSecIndex].SetDisplayFlag(bDisplay);
	else
	{
		for (int i = 0; i < MSCC_SECTION_NUM; i++)
		{
			m_PerCardData[i].SetDisplayFlag(bDisplay);
		}
	}

	return;
}

//设置间距
void CMulSecCardControlEx::SetCardSpace(DWORD dwCardHSpace, DWORD dwCardVSpace)
{
	for (int i = 0; i < MSCC_SECTION_NUM; i++)
	{
		m_PerCardData[i].SetCardSpace(dwCardHSpace, dwCardVSpace, 0);
	}

	return;
}

//基准位置
void CMulSecCardControlEx::SetBenchmarkPos(int nXPos, int nYPos, enXCollocateMode XCollocateMode, enYCollocateMode YCollocateMode)
{
	m_PerCardData[0].SetBenchmarkPos(nXPos, nYPos-30, XCollocateMode, YCollocateMode);
	m_PerCardData[1].SetBenchmarkPos(nXPos, nYPos, XCollocateMode, YCollocateMode);
	m_PerCardData[2].SetBenchmarkPos(nXPos, nYPos+30, XCollocateMode, YCollocateMode);

	return;
}

//基准位置
void CMulSecCardControlEx::SetBenchmarkPos(const CPoint & BenchmarkPos, enXCollocateMode XCollocateMode, enYCollocateMode YCollocateMode)
{
	SetBenchmarkPos(BenchmarkPos.x, BenchmarkPos.y, XCollocateMode, YCollocateMode);
	return;
}

//获取数目
WORD CMulSecCardControlEx::GetCardCount(int nSecIndex)
{
	return m_PerCardData[nSecIndex].GetCardCount();
}

//设置扑克
WORD CMulSecCardControlEx::SetCardData(BYTE cbCardData[], WORD wCardCount)
{
	//数据验证
	ASSERT(wCardCount>=0 && wCardCount<=HAND_CARD_COUNT);

	//设置扑克
	if (wCardCount == 0)
	{
		for (int j = 0; j < MSCC_SECTION_NUM; j++)
			m_PerCardData[j].SetCardData(NULL, 0);
	}
	else
	{
		BYTE cbData[5];
		ZeroMemory(cbData, sizeof(cbData));

		//第一注
		CopyMemory(cbData, cbCardData, sizeof(BYTE)*3);
		m_PerCardData[0].SetCardData(cbData, 3);

		//第二注
		CopyMemory(cbData, &cbCardData[3], sizeof(BYTE)*5);
		m_PerCardData[1].SetCardData(cbData, 5);

		//第三注
		CopyMemory(cbData, &cbCardData[8], sizeof(BYTE)*5);
		m_PerCardData[2].SetCardData(cbData, 5);
	}

	return wCardCount;
}

//设置扑克
WORD CMulSecCardControlEx::SetCardData(int nSecIndex, BYTE cbCardData[], WORD wCardCount)
{
	//数据验证
	ASSERT(wCardCount>=0 && wCardCount<=HAND_CARD_COUNT) ;
	ASSERT((nSecIndex >= 0 && nSecIndex < MSCC_SECTION_NUM) || (nSecIndex == -1));

	if (nSecIndex == -1)
	{
		for(BYTE i=0; i<MSCC_SECTION_NUM; i++)
		{
			m_PerCardData[i].SetCardData(cbCardData, wCardCount);
		}
	}
	else m_PerCardData[nSecIndex].SetCardData(cbCardData, wCardCount);

	return wCardCount;
}

//获取扑克
WORD CMulSecCardControlEx::GetCardData(int nSecIndex, BYTE cbCardData[], WORD wMaxCount)
{
	//效验参数
	ASSERT(wMaxCount>=m_PerCardData[nSecIndex].GetCardCount());
	//数据验证
	ASSERT(wMaxCount>0 && wMaxCount<=HAND_CARD_COUNT) ;

	//拷贝扑克
	WORD wCardCount=m_PerCardData[nSecIndex].GetCardCount();
	ASSERT(wCardCount>=0 && wCardCount<=HAND_CARD_COUNT) ;

	for(WORD i=0 ; i<wCardCount ; ++i)
		cbCardData[i] = m_PerCardData[nSecIndex].GetCardData(i) ;

	return wCardCount;
}

//获取原点
CPoint CMulSecCardControlEx::GetOrignPoint(int nSecIndex)
{
	ASSERT((nSecIndex >= 0 && nSecIndex < MSCC_SECTION_NUM));
		
	//变量定义
	CPoint	ptOrign;
	m_PerCardData[nSecIndex].GetOriginPoint(ptOrign);

	return ptOrign;
}

//获取中心
CPoint CMulSecCardControlEx::GetCenterPoint(int nSecIndex)
{
	ASSERT((nSecIndex >= 0 && nSecIndex < MSCC_SECTION_NUM) || (nSecIndex == -1));

	//变量定义
	CPoint	ptOrign;
	if (nSecIndex == -1) m_PerCardData[1].GetCenterPoint(ptOrign);
	else m_PerCardData[nSecIndex].GetCenterPoint(ptOrign);

	return ptOrign;
}