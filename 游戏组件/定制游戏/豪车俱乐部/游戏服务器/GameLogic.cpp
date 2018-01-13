#include "StdAfx.h"
#include "GameLogic.h"

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameLogic::CGameLogic()
{
}

//��������
CGameLogic::~CGameLogic()
{
}

//��ȡ����
BYTE CGameLogic::GetCardType(const BYTE cbCardData, BYTE cbCardCount,BYTE *bcOutCadDataWin )
{
	ASSERT(1==cbCardCount);
	if (!(1==cbCardCount) )return 0;
	memset(bcOutCadDataWin,0,AREA_COUNT);

	BYTE bcData =cbCardData;
	if(0==bcData) return true;
	
	//��ɯ����
	if(1==bcData)
	{
		bcOutCadDataWin[0]= 20;
	}
	//������
	else if(5==bcData)
	{
		bcOutCadDataWin[1]= 20;
	}
	//��������
	else if(13==bcData)
	{
		bcOutCadDataWin[2]= 20;
	}
	//��ʱ��
	else if(9==bcData)
	{
		bcOutCadDataWin[3]= 20;
	}
	//·��
	else if(8==bcData||bcData==11||bcData==14)
	{
		bcOutCadDataWin[4]= 5;
	}
	//����
	else if(2==bcData||bcData==12||bcData==16)
	{
		bcOutCadDataWin[5]= 5;
	}
	//�ݱ�
	else if(4==bcData||bcData==7||bcData==10)
	{
		bcOutCadDataWin[6]= 5;
	}
	//����
	else if(3==bcData||bcData==6||bcData==15)
	{
		bcOutCadDataWin[7]= 5;
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
//////////////////////////////////////////////////////////////////////////

//��������
void CGameLogic::RandArray( INT nArrayOne[], INT nCountOne, INT nArrayTwo[], INT nCountTwo )
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

//��ȡ����
BYTE CGameLogic::GetAnimalIndex(BYTE cbAllIndex)
{
	//��ɯ����
	if(1==cbAllIndex)
	{
		return 1;
	}
	//������
	else if(5==cbAllIndex)
	{
		return 2;
	}
	//��������
	else if(13==cbAllIndex)
	{
		return 3;
	}
	//��ʱ��
	else if(9==cbAllIndex)
	{
		return 4;
	}
	//·��
	else if(8==cbAllIndex||cbAllIndex==11||cbAllIndex==14)
	{
		return 5;
	}
	//����
	else if(2==cbAllIndex||cbAllIndex==12||cbAllIndex==16)
	{
		return 6;

	}
	//�ݱ�
	else if(4==cbAllIndex||cbAllIndex==7||cbAllIndex==10)
	{
		return 7;
	}
	//����
	else if(3==cbAllIndex||cbAllIndex==6||cbAllIndex==15)
	{
		return 8;
	}

	return 0;
}