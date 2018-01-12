#include "StdAfx.h"
#include "GameLogic.h"

//////////////////////////////////////////////////////////////////////////

//�˿�����
const BYTE CGameLogic::m_cbCardListData[ALL_CARD_COUNT]=
{
		0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,	//���� A - 10
		0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,	//÷�� A - 10
		0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2A,	//���� A - 10
		0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3A	//���� A - 10
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
BYTE CGameLogic::GetCardType(const BYTE cbCardData[], BYTE cbCardCount)
{
	//�Ϸ��ж�
	if((cbCardData[0]&0x0F)==(cbCardData[1]&0x0F))
		return CT_PAIR;
	return CT_POINT;
}

//��С�Ƚ�
/*
cbNextCardData>cbFirstCardData  ����1
cbNextCardData<cbFirstCardData  ����-1
cbNextCardData==cbFirstCardData ����0
*/
//Multiple �Ƚϳ����ı���

bool CGameLogic::EqualCard(const BYTE cbFirstCardData[],const BYTE cbNextCardData[])
{
	//�Ϸ��ж�
	bool bReturn=false;
	//��ȡ����
	BYTE cbFirstCardType=GetCardType(cbFirstCardData,CARD_COUNT);
	BYTE cbNextCardType=GetCardType(cbNextCardData,CARD_COUNT);

	if(cbFirstCardType==cbNextCardType)
	{
		if(cbFirstCardType==CT_PAIR)
		{
			if((cbFirstCardData[0]&0x0F)==(cbNextCardData[0]&0x0F))
				bReturn=true;
		}
		else
		{
			BYTE cbFirstCardPoint=(cbFirstCardData[0]&0x0F)+(cbFirstCardData[1]&0x0F);
			BYTE cbNextCardPoint=(cbNextCardData[0]&0x0F)+(cbNextCardData[1]&0x0F);
			cbFirstCardPoint=cbFirstCardPoint%10;
			cbNextCardPoint=cbNextCardPoint%10;
			if(cbFirstCardPoint==cbNextCardPoint)
				bReturn=true;
		}	
	}
	return bReturn;
}

int CGameLogic::CompareCard(const BYTE cbFirstCardData[],const BYTE cbNextCardData[])
{
	//�Ϸ��ж�
	int nReturn=0;
	//��ȡ����
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



VOID CGameLogic::DeduceWinner(int *nWinMultiple,BYTE *cbDiceValue)
{
	//��������
	tagAnalyseResult analyseResult;
	memset(nWinMultiple,-1,sizeof(int)*AREA_COUNT);
	memset(&analyseResult,0,sizeof(analyseResult));
	for(BYTE cbIndex=0;cbIndex<DICE_COUNT;cbIndex++)
	{
		for(BYTE cbRecord=0;cbRecord<analyseResult.cbUnEqualDice;cbRecord++)
		{
			if(analyseResult.m_DiceType[cbRecord].cbDate==cbDiceValue[cbIndex])
			{
				analyseResult.m_DiceType[cbRecord].cbCount++;
				break;
			}

			if (analyseResult.cbUnEqualDice == cbRecord)
			{
				analyseResult.m_DiceType[analyseResult.cbUnEqualDice].cbDate = cbDiceValue[cbIndex];
				analyseResult.m_DiceType[analyseResult.cbUnEqualDice++].cbCount++;
			}
		}

		analyseResult.cbAllDiceValue+=cbDiceValue[cbIndex];
	}

	//�ж϶��Ӻ����
	for(BYTE cx=0;cx<3;cx++)
	{
		for(BYTE cy=cx+1;cy<3;cy++)
		{
			BYTE cbAreaIndex=DiceMutiple[cbDiceValue[cx]-1][cbDiceValue[cy]-1].cbAreaIndex;
			nWinMultiple[cbAreaIndex]=DiceMutiple[cbDiceValue[cx]-1][cbDiceValue[cy]-1].nMutiple;
		}
	}

	//�ж�������ֵ
	if(analyseResult.cbUnEqualDice!=1) //�������ͬ��Ϊͨɱ
	{
		if(analyseResult.cbAllDiceValue>=11&&analyseResult.cbAllDiceValue<=17) nWinMultiple[0]=1;
		if(analyseResult.cbAllDiceValue>=4&&analyseResult.cbAllDiceValue<=10)  nWinMultiple[1]=1;
	}

	//��˫
	if(analyseResult.cbUnEqualDice!=1)
	{
		if(analyseResult.cbAllDiceValue%2==0) nWinMultiple[51]=1;
		if(analyseResult.cbAllDiceValue%2==1)  nWinMultiple[50]=1;
	}

	if(analyseResult.cbAllDiceValue>=4&&analyseResult.cbAllDiceValue<=17)
	{
		BYTE cbAreaIndex=DiceMutipleEx[analyseResult.cbAllDiceValue-4].cbAreaIndex;
		nWinMultiple[cbAreaIndex]=DiceMutipleEx[analyseResult.cbAllDiceValue-4].nMutiple;
	}

	//�ж�ͬ����ͬ�����͵���
	for(BYTE cbIndex=0;cbIndex<analyseResult.cbUnEqualDice;cbIndex++)
	{
		//���·�6������
		nWinMultiple[43+analyseResult.m_DiceType[cbIndex].cbDate]=analyseResult.m_DiceType[cbIndex].cbCount;
		if(analyseResult.m_DiceType[cbIndex].cbCount==3)
		{
			nWinMultiple[7+analyseResult.m_DiceType[cbIndex].cbDate]=150;
			nWinMultiple[14]=24;
		}
	}
}
//////////////////////////////////////////////////////////////////////////

