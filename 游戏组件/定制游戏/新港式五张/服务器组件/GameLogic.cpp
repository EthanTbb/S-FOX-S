#include "StdAfx.h"
#include "GameLogic.h"

//////////////////////////////////////////////////////////////////////////

//�˿�����
BYTE CGameLogic::m_cbCardListData[28]=
{
	0x01,0x08,0x09,0x0A,0x0B,0x0C,0x0D,									//���� A - K
	0x11,0x18,0x19,0x1A,0x1B,0x1C,0x1D,									//÷�� A - K
	0x21,0x28,0x29,0x2A,0x2B,0x2C,0x2D,									//���� A - K
	0x31,0x38,0x39,0x3A,0x3B,0x3C,0x3D,									//���� A - K
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

//��ȡ����
BYTE CGameLogic::GetCardGenre(BYTE cbCardData[], BYTE cbCardCount)
{
	BYTE cbCardTemp[MAX_COUNT];
	ZeroMemory(cbCardTemp, sizeof(cbCardTemp));

	CopyMemory(cbCardTemp,cbCardData, cbCardCount * sizeof(BYTE));
	SortCardList(cbCardTemp,cbCardCount);

	//������
	switch (cbCardCount)
	{
	case 1: //����
		{
			return CT_SINGLE;
		}
	case 2:	//����
		{
			return (GetCardLogicValue(cbCardData[0])==GetCardLogicValue(cbCardData[1]))?CT_ONE_DOUBLE:CT_SINGLE;
		}
	}

	//��������
	if (cbCardCount>=MAX_COUNT)
	{
		//��������
		bool cbSameColor=true,bLineCard=true;
		BYTE cbFirstColor=GetCardColor(cbCardTemp[0]);
		BYTE cbFirstValue=GetCardLogicValue(cbCardTemp[0]);

		//���η���
		for (BYTE i=1;i<cbCardCount;i++)
		{
			//���ݷ���
			if (GetCardColor(cbCardTemp[i])!=cbFirstColor) cbSameColor=false;
			if (cbFirstValue!=(GetCardLogicValue(cbCardTemp[i])+i)) bLineCard=false;

			//�����ж�
			if ((cbSameColor==false)&&(bLineCard==false)) break;
		}

		//˳������
		if ((cbSameColor==false)&&(bLineCard==true)) return CT_SHUN_ZI;

		//ͬ������
		if ((cbSameColor==true)&&(bLineCard==false)) return CT_TONG_HUA;

		//ͬ��˳����
		if ((cbSameColor==true)&&(bLineCard==true)) return CT_TONG_HUA_SHUN;
	}

	//�˿˷���
	tagAnalyseResult AnalyseResult;
	AnalysebCardData(cbCardTemp,cbCardCount,AnalyseResult);

	//��������
	if (AnalyseResult.cbFourCount==1) return CT_TIE_ZHI;

	//��������
	if (AnalyseResult.cbDoubleCount==2) return CT_TWO_DOUBLE;

	//��������
	if ((AnalyseResult.cbDoubleCount==1)&&(AnalyseResult.cbThreeCount==0)) return CT_ONE_DOUBLE;

	//��«����
	if (AnalyseResult.cbThreeCount==1) return (AnalyseResult.cbDoubleCount==1)?CT_HU_LU:CT_THREE_TIAO;

	return CT_SINGLE;
}

//�����˿�
VOID CGameLogic::SortCardList(BYTE cbCardData[], BYTE cbCardCount)
{
	//ת����ֵ
	BYTE cbLogicValue[MAX_COUNT];
	for (BYTE i=0;i<cbCardCount;i++) cbLogicValue[i]=GetCardLogicValue(cbCardData[i]);	

	//�������
	bool bSorted=true;
	BYTE cbTempData,bLast=cbCardCount-1;
	do
	{
		bSorted=true;
		for (BYTE i=0;i<bLast;i++)
		{
			if ((cbLogicValue[i]<cbLogicValue[i+1])||
				((cbLogicValue[i]==cbLogicValue[i+1])&&(cbCardData[i]<cbCardData[i+1])))
			{
				//����λ��
				cbTempData=cbCardData[i];
				cbCardData[i]=cbCardData[i+1];
				cbCardData[i+1]=cbTempData;
				cbTempData=cbLogicValue[i];
				cbLogicValue[i]=cbLogicValue[i+1];
				cbLogicValue[i+1]=cbTempData;
				bSorted=false;
			}	
		}
		bLast--;
	} while(bSorted==false);

	return;
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

//�߼���ֵ
BYTE CGameLogic::GetCardLogicValue(BYTE cbCardData)
{
	//ת����ֵ
	BYTE cbCardValue=GetCardValue(cbCardData);
	BYTE cbLogicValue=(cbCardValue<=2)?(cbCardValue+13):cbCardValue;

	return cbLogicValue;
}

//�Ա��˿�
bool CGameLogic::CompareCard(BYTE cbFirstCardData[], BYTE cbNextCardData[], BYTE cbCardCount)
{
	//��ȡ����
	BYTE cbNextGenre=GetCardGenre(cbNextCardData,cbCardCount);
	BYTE cbFirstGenre=GetCardGenre(cbFirstCardData,cbCardCount);

	//�����ж�
	if (cbFirstGenre!=cbNextGenre) return (cbFirstGenre>cbNextGenre);

	//���ͶԱ�
	switch(cbFirstGenre)
	{
	case CT_SINGLE:			//����
		{
			SortCardList(cbNextCardData,cbCardCount);
			SortCardList(cbFirstCardData,cbCardCount);

			BYTE cbNextValue=GetCardLogicValue(cbNextCardData[0]);
			BYTE cbFirstValue=GetCardLogicValue(cbFirstCardData[0]);
			BYTE cbNextColor=GetCardColor(cbNextCardData[0]);
			BYTE cbFirstColor=GetCardColor(cbFirstCardData[0]);

			if (cbNextValue != cbFirstValue)
			{
				return cbFirstValue>cbNextValue;
			}

			return cbFirstColor > cbNextColor;
		}
	case CT_HU_LU:			//��«
	case CT_TIE_ZHI:		//��֧
	case CT_ONE_DOUBLE:		//����
	case CT_TWO_DOUBLE:		//����
	case CT_THREE_TIAO:		//����
		{
			//�����˿�
			tagAnalyseResult AnalyseResultNext;
			tagAnalyseResult AnalyseResultFirst;
			AnalysebCardData(cbNextCardData,cbCardCount,AnalyseResultNext);
			AnalysebCardData(cbFirstCardData,cbCardCount,AnalyseResultFirst);

			//������ֵ
			if (AnalyseResultFirst.cbFourCount>0)
			{
				BYTE cbNextValue=AnalyseResultNext.cbFourLogicVolue[0];
				BYTE cbFirstValue=AnalyseResultFirst.cbFourLogicVolue[0];
				return cbFirstValue>cbNextValue;
			}

			//������ֵ
			if (AnalyseResultFirst.cbThreeCount>0)
			{
				BYTE cbNextValue=AnalyseResultNext.cbThreeLogicVolue[0];
				BYTE cbFirstValue=AnalyseResultFirst.cbThreeLogicVolue[0];
				return cbFirstValue>cbNextValue;
			}

			//����
			if (cbFirstGenre == CT_TWO_DOUBLE)
			{
				ASSERT(AnalyseResultFirst.cbDoubleCount == 2);

				//�ȱȽϴ���Ӻ�С���ӵ��߼�ֵ
				for (BYTE i=0; i<AnalyseResultFirst.cbDoubleCount; i++)
				{
					SortCardList(AnalyseResultNext.cbDoubleLogicVolue, AnalyseResultNext.cbDoubleCount);
					SortCardList(AnalyseResultFirst.cbDoubleLogicVolue, AnalyseResultFirst.cbDoubleCount);
					BYTE cbNextValue = AnalyseResultNext.cbDoubleLogicVolue[i];
					BYTE cbFirstValue = AnalyseResultFirst.cbDoubleLogicVolue[i];
					if (cbFirstValue != cbNextValue) 
					{
						return cbFirstValue > cbNextValue;
					}
				}

				//���ӻ�ɫ
				if (AnalyseResultFirst.cbDoubleCount > 0)
				{
					SortCardList(AnalyseResultNext.cbDoubleCardData, AnalyseResultNext.cbDoubleCount);
					SortCardList(AnalyseResultFirst.cbDoubleCardData, AnalyseResultFirst.cbDoubleCount);
					BYTE cbNextColor = GetCardColor(AnalyseResultNext.cbDoubleCardData[0]);
					BYTE cbFirstColor = GetCardColor(AnalyseResultFirst.cbDoubleCardData[0]);
					return cbFirstColor>cbNextColor;
				}
			}

			//һ��
			if (cbFirstGenre == CT_ONE_DOUBLE)
			{
				ASSERT(AnalyseResultFirst.cbDoubleCount == 1);

				//�ȱȽ϶��ӵ��߼�ֵ
				for (BYTE i=0; i<AnalyseResultFirst.cbDoubleCount; i++)
				{
					SortCardList(AnalyseResultNext.cbDoubleLogicVolue, AnalyseResultNext.cbDoubleCount);
					SortCardList(AnalyseResultFirst.cbDoubleLogicVolue, AnalyseResultFirst.cbDoubleCount);
					BYTE cbNextValue = AnalyseResultNext.cbDoubleLogicVolue[i];
					BYTE cbFirstValue = AnalyseResultFirst.cbDoubleLogicVolue[i];
					if (cbFirstValue != cbNextValue) 
					{
						return cbFirstValue > cbNextValue;
					}
				}

				//���ӻ�ɫ
				if (AnalyseResultFirst.cbDoubleCount > 0)
				{
					SortCardList(AnalyseResultNext.cbDoubleCardData, AnalyseResultNext.cbDoubleCount);
					SortCardList(AnalyseResultFirst.cbDoubleCardData, AnalyseResultFirst.cbDoubleCount);
					BYTE cbNextColor = GetCardColor(AnalyseResultNext.cbDoubleCardData[0]);
					BYTE cbFirstColor = GetCardColor(AnalyseResultFirst.cbDoubleCardData[0]);
					return cbFirstColor > cbNextColor;
				}
			}

			break;
		}
	case CT_SHUN_ZI:		//˳��
	case CT_TONG_HUA:		//ͬ��
	case CT_TONG_HUA_SHUN:	//ͬ��˳
		{
			//��ֵ�ж�
			BYTE cbNextValue=GetCardLogicValue(cbNextCardData[0]);
			BYTE cbFirstValue=GetCardLogicValue(cbFirstCardData[0]);
			if (cbFirstValue!=cbNextValue) return (cbFirstValue>cbNextValue);

			//��ɫ�ж�
			BYTE cbNextColor=GetCardColor(cbNextCardData[0]);
			BYTE cbFirstColor=GetCardColor(cbFirstCardData[0]);

			return (cbFirstColor>cbNextColor);
		}
	}

	//�������
	ASSERT(FALSE);

	return false;
}

//�����˿�
VOID CGameLogic::AnalysebCardData(const BYTE cbCardData[], BYTE cbCardCount, tagAnalyseResult & AnalyseResult)
{
	//���ý��
	ZeroMemory(&AnalyseResult,sizeof(AnalyseResult));

	//�˿˷���
	for (BYTE i=0;i<cbCardCount;i++)
	{
		//��������
		BYTE cbSameCount=1;
		BYTE cbSameCardData[4]={cbCardData[i],0,0,0};
		BYTE cbLogicValue=GetCardLogicValue(cbCardData[i]);

		//��ȡͬ��
		for (int j=i+1;j<cbCardCount;j++)
		{
			//�߼��Ա�
			if (GetCardLogicValue(cbCardData[j])!=cbLogicValue) break;

			//�����˿�
			cbSameCardData[cbSameCount++]=cbCardData[j];
		}

		//������
		switch (cbSameCount)
		{
		case 1:		//����
			{
				AnalyseResult.cbSignedLogicVolue[AnalyseResult.cbSignedCount]=cbLogicValue;
				CopyMemory(&AnalyseResult.cbSignedCardData[(AnalyseResult.cbSignedCount++)*cbSameCount],cbSameCardData,cbSameCount);
				break;
			}
		case 2:		//����
			{
				AnalyseResult.cbDoubleLogicVolue[AnalyseResult.cbDoubleCount]=cbLogicValue;
				CopyMemory(&AnalyseResult.cbDoubleCardData[(AnalyseResult.cbDoubleCount++)*cbSameCount],cbSameCardData,cbSameCount);
				break;
			}
		case 3:		//����
			{
				AnalyseResult.cbThreeLogicVolue[AnalyseResult.cbThreeCount]=cbLogicValue;
				CopyMemory(&AnalyseResult.cbThreeCardData[(AnalyseResult.cbThreeCount++)*cbSameCount],cbSameCardData,cbSameCount);
				break;
			}
		case 4:		//����
			{
				AnalyseResult.cbFourLogicVolue[AnalyseResult.cbFourCount]=cbLogicValue;
				CopyMemory(&AnalyseResult.cbFourCardData[(AnalyseResult.cbFourCount++)*cbSameCount],cbSameCardData,cbSameCount);
				break;
			}
		}

		//���õ���
		i+=cbSameCount-1;
	}

	return;
}

//////////////////////////////////////////////////////////////////////////
