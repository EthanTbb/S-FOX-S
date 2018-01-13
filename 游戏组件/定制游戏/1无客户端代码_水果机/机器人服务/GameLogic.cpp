#include "StdAfx.h"
#include "GameLogic.h"

//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////

//�˿�����
const BYTE CGameLogic::m_cbCardListData[CARD_COUNT*2]=
{
	2,4,6,8,10,12,14,16,18,20,22,24,26,28,30,32,1,3,5,7,9,11,13,15,17,19,21,23,25,27,29,31,\
		2,4,6,8,10,12,14,16,18,20,22,24,26,28,30,32,\
		2,4,6,8,10,12,14,16,18,20,22,24,26,28,30,32
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

	return;
}
//��ȡ����
BYTE CGameLogic::GetCardType(const BYTE cbCardData, BYTE cbCardCount,BYTE *bcOutCadDataWin )
{
	ASSERT(1==cbCardCount);
	if (!(1==cbCardCount) )return 0;
	memset(bcOutCadDataWin,0,AREA_COUNT);
	BYTE bcData =cbCardData;

	if(5==bcData||bcData==11||bcData==17||bcData==23||bcData==6)//ƻ��
	{
		if(bcData==6)
			bcOutCadDataWin[0]= 2;
		else
			bcOutCadDataWin[0]= 5;
	}
	else if(1==bcData||bcData==13||bcData==12)//����
	{
		if(bcData==12)
			bcOutCadDataWin[1]= 2;
		else
			bcOutCadDataWin[1]= 10;

	}
	else if(7==bcData||bcData==19||bcData==18)//����
	{
		if(bcData==18)
			bcOutCadDataWin[2]= 2;
		else
			bcOutCadDataWin[2]= 10;

	}
	else if(2==bcData||bcData==14||bcData==24)//����
	{
		bcOutCadDataWin[3]= 10;
	}
	else if(8==bcData||bcData==9)//����
	{
		if(bcData==9)
			bcOutCadDataWin[4]= 2;
		else
			bcOutCadDataWin[4]= 20;
	}
	else if(20==bcData||bcData==21)//����
	{
		if(bcData==21)
			bcOutCadDataWin[5]= 2;
		else
			bcOutCadDataWin[5]= 20;
	}
	else if(15==bcData||bcData==16)//˫7
	{
		if(bcData==15)
			bcOutCadDataWin[6]= 2;
		else
			bcOutCadDataWin[6]= 20;
	}
	else if(3==bcData||bcData==4)//��С��
	{
		if(bcData==3)
			bcOutCadDataWin[7]= 25;
		else
			bcOutCadDataWin[7]= 50;
	}

	return true;

}

//��С�Ƚ�
/*
cbNextCardData>cbFirstCardData  ����1
cbNextCardData<cbFirstCardData  ����-1
cbNextCardData==cbFirstCardData ����0
*/
//Multiple �Ƚϳ����ı���
int CGameLogic::CompareCard(const BYTE cbFirstCardData[], BYTE cbFirstCardCount,const BYTE cbNextCardData[], BYTE cbNextCardCount,BYTE &Multiple)
{
	//�Ϸ��ж�
	ASSERT(1==cbFirstCardCount && 1==cbNextCardCount);
	if (!(1==cbFirstCardCount && 1==cbNextCardCount)) return 0;

	return true;
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
		cbPipCount+=cbCardValue;
	}
	return (cbPipCount%10);
}
BYTE CGameLogic::GetCardNewValue(BYTE cbCardData)
{
	//�˿�����
	BYTE cbCardColor=GetCardColor(cbCardData);
	BYTE cbCardValue=GetCardValue(cbCardData);

	//ת����ֵ
	if (cbCardColor==0x04) return cbCardValue+13+2;
	return cbCardValue;

}
//�߼���С
BYTE CGameLogic::GetCardLogicValue(BYTE cbCardData)
{
	BYTE cbValue=GetCardValue(cbCardData);

	//��ȡ��ɫ
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
		if(cbSortType==ST_NEW)
		{
			for (BYTE i=0;i<cbCardCount;i++) cbSortValue[i]=GetCardNewValue(cbCardData[i]);	

		}else
		{
			for (BYTE i=0;i<cbCardCount;i++) cbSortValue[i]=GetCardLogicValue(cbCardData[i]);	

		}

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
