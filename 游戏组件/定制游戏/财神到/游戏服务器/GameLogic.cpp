#include "StdAfx.h"
#include "GameLogic.h"

//////////////////////////////////////////////////////////////////////////

//�˿�����
const BYTE CGameLogic::m_cbCardListData[CARD_COUNT]=
{
	   0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,	//���� A - 10
	   0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,	//÷�� A - 10
	   0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2A,	//���� A - 10
	   0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3A,	//���� A - 10
		
};

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameLogic::CGameLogic()
{
}

//��������
CGameLogic::~CGameLogic()
{
}

//�����˿�
VOID CGameLogic::RandCardList(BYTE cbCardBuffer[], BYTE cbBufferCount)
{
	//����׼��
	BYTE cbCardData[CountArray(m_cbCardListData)];
	CopyMemory(cbCardData,m_cbCardListData,sizeof(m_cbCardListData));

	//�����˿�
	BYTE cbRandCount=0,cbPosition=0;
	do
	{
		cbPosition=rand()%(CountArray(cbCardData)-cbRandCount);
		cbCardBuffer[cbRandCount++]=cbCardData[cbPosition];
		cbCardData[cbPosition]=cbCardData[CountArray(cbCardData)-cbRandCount];
	} while (cbRandCount<cbBufferCount);

	/*BYTE cbTempCardBuffer[] = {
      0x21,0x11,0x04,0x22,0x05,0x35,0x14,0x36,
	  0x21,0x11,0x04,0x22,0x05,0x35,0x14,0x36,
      0x21,0x11,0x04,0x22,0x05,0x35,0x14,0x36,
	  0x21,0x11,0x04,0x22,0x05,0x35,0x14,0x36,
	  0x21,0x11,0x04,0x22,0x05,0x35,0x14,0x36
	};
	CopyMemory(cbCardBuffer,cbTempCardBuffer,sizeof(cbTempCardBuffer));*/

	return;
}

//��ȡ����
BYTE CGameLogic::GetCardType(const BYTE cbCardData[], BYTE cbCardCount)
{
	//�Ϸ��ж�
	ASSERT(2==cbCardCount);
	if (2!=cbCardCount) return CT_ERROR;

	//�����˿�
	BYTE cbCardDataSort[CARD_COUNT];
	CopyMemory(cbCardDataSort,cbCardData,sizeof(BYTE)*cbCardCount);
	SortCardList(cbCardDataSort,cbCardCount,ST_LOGIC);

	//��ȡ����
	BYTE cbFirstCardValue=GetCardValue(cbCardDataSort[0]);
	BYTE cbSecondCardValue=GetCardValue(cbCardDataSort[1]);

	//��ȡ��ɫ
	BYTE cbFistCardColor=GetCardColor(cbCardDataSort[0]);
	BYTE cbSecondCardColor=GetCardColor(cbCardDataSort[1]);

	if( cbFirstCardValue == cbSecondCardValue)
		return CT_SPECIAL;
	//��������
	return CT_POINT;
}

//��С�Ƚ�
/*
cbNextCardData>cbFirstCardData  ����1
cbNextCardData<cbFirstCardData  ����-1
cbNextCardData==cbFirstCardData ����0
*/
int CGameLogic::CompareCard(const BYTE cbFirstCardData[], BYTE cbFirstCardCount,const BYTE cbNextCardData[], BYTE cbNextCardCount)
{
	//�Ϸ��ж�
	ASSERT(2==cbFirstCardCount && 2==cbNextCardCount);
	if (!(2==cbFirstCardCount && 2==cbNextCardCount)) return 0;

	//��ȡ����
	BYTE cbFirstCardType=GetCardType(cbFirstCardData, cbFirstCardCount);
	BYTE cbNextCardType=GetCardType(cbNextCardData, cbNextCardCount);

	//���ͱȽ�
	if (cbFirstCardType != cbNextCardType) 
	{
		if (cbNextCardType > cbFirstCardType) return 1;
		else return -1;
	}
	else
	{
		if(CT_SPECIAL == cbFirstCardType )
		{
			BYTE bcCardTmp[2];
			BYTE bcCardTmp2[2];
			CopyMemory(bcCardTmp,cbFirstCardData,2);
			CopyMemory(bcCardTmp2,cbNextCardData,2);

			SortCardList(bcCardTmp,cbFirstCardCount,2);
			SortCardList(bcCardTmp2,cbNextCardCount,2);

			if(GetCardLogicValue(bcCardTmp[0])>=GetCardLogicValue(bcCardTmp2[0]))
			{
				return -1;
			}
			else
			{
				return 1;
			}
		}
	}

	//��ȡ����
	BYTE cbFirstPip=GetCardListPip(cbFirstCardData, cbFirstCardCount);
	BYTE cbNextPip=GetCardListPip(cbNextCardData, cbNextCardCount);

	//�����Ƚ�

	if (cbFirstPip != cbNextPip)
	{
		if (cbNextPip >cbFirstPip)
		{	
			return 1;
		}
		else return -1;
	}
	if(cbNextPip==cbFirstPip)
	{
		//BYTE bcCardTmp[2];
		//BYTE bcCardTmp2[2];
		//CopyMemory(bcCardTmp,cbFirstCardData,2);
		//CopyMemory(bcCardTmp2,cbNextCardData,2);

		//SortCardList(bcCardTmp,cbFirstCardCount,2);
		//SortCardList(bcCardTmp2,cbNextCardCount,2);

		//if(GetCardLogicValue(bcCardTmp[0])>=GetCardLogicValue(bcCardTmp2[0]))
		//{
		//	return -1;
		//}else
		//{
		//	return 1;
		//}

		//�Ƶ�һ����Ļ�����ׯ��ʤ����
		return -1;
	}

	return 0;
}

//��ȡ�Ƶ�
BYTE CGameLogic::GetCardListPip(const BYTE cbCardData[], BYTE cbCardCount)
{
	//��������
	BYTE cbPipCount=0;

	//��ȡ�Ƶ�
	BYTE cbCardValue=0;
	for (BYTE i=0;i<cbCardCount;i++)
	{
		cbCardValue=GetCardValue(cbCardData[i]);
		if(cbCardValue>10)
		{
			cbCardValue = 10;
		}
		cbPipCount+=(1==cbCardValue ? 1 : cbCardValue);
	}

	return (cbPipCount%10);
}

//�߼���С
BYTE CGameLogic::GetCardLogicValue(BYTE cbCardData)
{

	BYTE bCardValue=GetCardValue(cbCardData);

	/*if(bCardValue>10)
	{
		bCardValue = 10;
	}*/

	//ת����ֵ
	return (bCardValue<1)?(bCardValue+13):bCardValue;
	
	return 0;
}

//�����˿�
VOID CGameLogic::SortCardList(BYTE cbCardData[], BYTE cbCardCount, BYTE cbSortType)
{
	//��Ŀ����
	if (cbCardCount==0) return;

	//ת����ֵ
	BYTE cbSortValue[CARD_COUNT];
	if (ST_VALUE==cbSortType)
	{
		for (BYTE i=0;i<cbCardCount;i++) cbSortValue[i]=GetCardValue(cbCardData[i]);	
	}
	else 
	{
		for (BYTE i=0;i<cbCardCount;i++) cbSortValue[i]=GetCardLogicValue(cbCardData[i]);	
	}

	//�������
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
				//����λ��
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

