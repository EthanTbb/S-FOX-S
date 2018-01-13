#include "StdAfx.h"
#include "GameLogic.h"

//////////////////////////////////////////////////////////////////////////

//�˿�����
 BYTE CGameLogic::m_cbCardListData[172-4]=
{
	    2,4,6,8,10,12,14,16,18,20,22,24,26,28,30,32,\
		2,4,6,8,10,12,14,16, 18,20,22,24,26,28,30,32,\
		1,9,17,25,3,11,19,27,3,11,19,27, 5,13,21,29,5,\
		13,21,29,5,13,21,29,7,15,23,31,7,15,23,31,7,15,23,31,7,15,23,31,\
		2,4,6,8,10,12,14,16,18,20,22,24,26,28,30,32,\
		2,4,6,8,10,12,14,16,18,20,22,24,26,28,30,32,\
		2,4,6,8,10,12,14,16,18,20,22,24,26,28,30,32,\
		2,4,6,8,10,12,14,16,18,20,22,24,26,28,30,32,\
		2,4,6,8,10,12,14,16,18,20,22,24,26,28,30,32,\
		2,4,6,8,10,12,14,16,18,20,22,24,26,28,30,32\
	
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
void CGameLogic::RandCardList(BYTE cbCardBuffer[], BYTE cbBufferCount)
{

	//����׼��
	BYTE cbCardData[CountArray(m_cbCardListData)];
	CopyMemory(cbCardData,m_cbCardListData,sizeof(m_cbCardListData));

	//�����˿�
	BYTE cbRandCount=0,cbPosition=0;
	do
	{
		cbPosition=(BYTE)(rand()+GetTickCount()*2)%(CountArray(cbCardData)-cbRandCount);
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
	memset(bcOutCadDataWin,0,AREA_COUNT+1);
	BYTE bcData =cbCardData;	
//1,13�����ӡ�2,14,24��������3С����4������5,11,17,23��ƻ����6Сƻ����7,19�����ʡ�8�����ϡ�9С���ϡ�10��LUCK��12С���ӡ�15С˫7��16��˫7��18С���ʡ�20�����ǡ�21С���ǡ�22��LUCK
	
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
		if(bcData==24)
			bcOutCadDataWin[3]= 2;
		else
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
	else if(bcData==10)
		bcOutCadDataWin[8]=99;
	else if(bcData==22)
		bcOutCadDataWin[8]=100;

	return true;	
}

//��ȡ����
BYTE CGameLogic::GetCardTypeEx(const BYTE cbCardData, BYTE cbCardCount,BYTE *bcOutCadDataWin )
{
	ASSERT(1==cbCardCount);
	if (!(1==cbCardCount) )return 0;
	memset(bcOutCadDataWin,0,AREA_COUNT+1);
	BYTE bcData =cbCardData;	
	//1,13�����ӡ�2,14,24��������3С����4������5,11,17,23��ƻ����6Сƻ����7,19�����ʡ�8�����ϡ�9С���ϡ�10��LUCK��12С���ӡ�15С˫7��16��˫7��18С���ʡ�20�����ǡ�21С���ǡ�22��LUCK

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
		if(bcData==24)
			bcOutCadDataWin[3]= 2;
		else
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
	else if(bcData==10)
		bcOutCadDataWin[8]=99;
	else if(bcData==22)
		bcOutCadDataWin[8]=100;

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
void CGameLogic::SortCardList(BYTE cbCardData[], BYTE cbCardCount, BYTE cbSortType)
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

//��������
void CGameLogic::ChaosArray( INT nArrayOne[], INT nCountOne, INT nArrayTwo[], INT nCountTwo )
{
	ASSERT( nCountOne == nCountTwo );
	if( nCountTwo != nCountOne )
		return;

	DWORD wTick = GetTickCount();
	for (int i = 1; i < nCountOne; ++i)
	{
		int nTempIndex = (rand()+wTick)%nCountOne;

		int nTempValueOne = nArrayOne[i];
		nArrayOne[i] = nArrayOne[nTempIndex];
		nArrayOne[nTempIndex] = nTempValueOne;

		int nTempValueTwo = nArrayTwo[i];
		nArrayTwo[i] = nArrayTwo[nTempIndex];
		nArrayTwo[nTempIndex] = nTempValueTwo;
	}
}
//////////////////////////////////////////////////////////////////////////

