#include "StdAfx.h"
#include "GameLogic.h"

//////////////////////////////////////////////////////////////////////////

//扑克数据
const BYTE CGameLogic::m_cbCardListData[ALL_CARD_COUNT]=
{
		0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,	//方块 A - 10
		0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,	//梅花 A - 10
		0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2A,	//红桃 A - 10
		0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3A	//黑桃 A - 10
};

//////////////////////////////////////////////////////////////////////////

//构造函数
CGameLogic::CGameLogic()
{
}

//析构函数
CGameLogic::~CGameLogic()
{
}

//混乱扑克
void CGameLogic::RandCardList(BYTE cbCardBuffer[], BYTE cbBufferCount)
{
	//混乱准备
	BYTE cbCardData[CountArray(m_cbCardListData)];
	CopyMemory(cbCardData,m_cbCardListData,sizeof(m_cbCardListData));

	//混乱扑克
	BYTE cbRandCount=0,cbPosition=0;
	do
	{
		cbPosition=rand()%(CountArray(cbCardData)-cbRandCount);
		cbCardBuffer[cbRandCount++]=cbCardData[cbPosition];
		cbCardData[cbPosition]=cbCardData[CountArray(cbCardData)-cbRandCount];
	} while (cbRandCount<cbBufferCount);

	return;
}

//获取牌型
BYTE CGameLogic::GetCardType(const BYTE cbCardData[], BYTE cbCardCount)
{
	//合法判断
	if((cbCardData[0]&0x0F)==(cbCardData[1]&0x0F))
		return CT_PAIR;
	return CT_POINT;
}

//大小比较
/*
cbNextCardData>cbFirstCardData  返回1
cbNextCardData<cbFirstCardData  返回-1
cbNextCardData==cbFirstCardData 返回0
*/
//Multiple 比较出来的倍数
int CGameLogic::CompareCard(const BYTE cbFirstCardData[],const BYTE cbNextCardData[])
{
	//合法判断
	int nReturn=0;
	//获取牌型
	BYTE cbFirstCardType=GetCardType(cbFirstCardData,CARD_COUNT);
	BYTE cbNextCardType=GetCardType(cbNextCardData,CARD_COUNT);
	if(cbFirstCardType>cbNextCardType)
	{
		nReturn=-1;
	}
	else
	{
		if(cbFirstCardType==cbNextCardType)
		{
			if(cbFirstCardType==CT_PAIR)
			{
				if((cbFirstCardData[0]&0x0F)>=(cbNextCardData[0]&0x0F))
					nReturn=-1;
				else
					nReturn=1;
			}
			else
			{
				BYTE cbFirstCardPoint=(cbFirstCardData[0]&0x0F)+(cbFirstCardData[1]&0x0F);
				BYTE cbNextCardPoint=(cbNextCardData[0]&0x0F)+(cbNextCardData[1]&0x0F);
				cbFirstCardPoint=cbFirstCardPoint%10;
				cbNextCardPoint=cbNextCardPoint%10;
				
				if(cbFirstCardPoint>=cbNextCardPoint)	nReturn=-1;
				else nReturn=1;
			}
		}
		else	
		{
			nReturn=1;
		}
	}
	return nReturn;
}

//获取牌点
BYTE CGameLogic::GetCardListPip(const BYTE cbCardData[], BYTE cbCardCount)
{
	//变量定义
	BYTE cbPipCount=0;

	//获取牌点
	BYTE cbCardValue=0;
	for (BYTE i=0;i<cbCardCount;i++)
	{
		cbCardValue=GetCardValue(cbCardData[i]);
		if(cbCardValue>10)
		{
			cbCardValue = 10;

		}
		cbPipCount+=cbCardValue;
	}
	return (cbPipCount%10);
}
BYTE CGameLogic::GetCardNewValue(BYTE cbCardData)
{
	//扑克属性
	BYTE cbCardColor=GetCardColor(cbCardData);
	BYTE cbCardValue=GetCardValue(cbCardData);

	//转换数值
	if (cbCardColor==0x04) return cbCardValue+13+2;
	return cbCardValue;

}
//逻辑大小
BYTE CGameLogic::GetCardLogicValue(BYTE cbCardData)
{
	BYTE cbValue=GetCardValue(cbCardData);

	//获取花色
	BYTE cbColor=GetCardColor(cbCardData);

	if(cbValue>10)
	{
		cbValue = 10;

	}
	if(cbColor==0x4)
	{
		return 11;
	}
	return cbValue;
}

//排列扑克
void CGameLogic::SortCardList(BYTE cbCardData[], BYTE cbCardCount, BYTE cbSortType)
{
	//数目过虑
	if (cbCardCount==0) return;

	//转换数值
	BYTE cbSortValue[ALL_CARD_COUNT];
	if (ST_VALUE==cbSortType)
	{
		for (BYTE i=0;i<cbCardCount;i++) cbSortValue[i]=GetCardValue(cbCardData[i]);	
	}
	else 
	{
		if(cbSortType==ST_NEW)
		{
			for (BYTE i=0;i<cbCardCount;i++) cbSortValue[i]=GetCardNewValue(cbCardData[i]);	

		}else
		{
			for (BYTE i=0;i<cbCardCount;i++) cbSortValue[i]=GetCardLogicValue(cbCardData[i]);	

		}

	}


	//排序操作
	bool bSorted=true;
	BYTE cbThreeCount,cbLast=cbCardCount-1;
	do
	{
		bSorted=true;
		for (BYTE i=0;i<cbLast;i++)
		{
			if ((cbSortValue[i]<cbSortValue[i+1])||
				((cbSortValue[i]==cbSortValue[i+1])&&(cbCardData[i]<cbCardData[i+1])))
			{
				//交换位置
				cbThreeCount=cbCardData[i];
				cbCardData[i]=cbCardData[i+1];
				cbCardData[i+1]=cbThreeCount;
				cbThreeCount=cbSortValue[i];
				cbSortValue[i]=cbSortValue[i+1];
				cbSortValue[i+1]=cbThreeCount;
				bSorted=false;
			}	
		}
		cbLast--;
	} while(bSorted==false);

	return;
}

//////////////////////////////////////////////////////////////////////////

