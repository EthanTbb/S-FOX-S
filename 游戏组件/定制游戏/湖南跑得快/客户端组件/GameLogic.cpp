#include "StdAfx.h"
#include "GameLogic.h"

//////////////////////////////////////////////////////////////////////////
//��̬����

//�˿�����
const BYTE	CGameLogic::m_bCardListData[48]=
{
	          0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,	//���� A - K
	0x11,     0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,0x1B,0x1C,0x1D,	//÷�� A - K
	0x21,     0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2A,0x2B,0x2C,0x2D,	//���� A - K
	0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3A,0x3B,0x3C,0x3D,	//���� A - K
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
BYTE CGameLogic::GetCardType(BYTE bCardData[], BYTE bCardCount)
{
	//������
	switch (bCardCount)
	{
	case 1: //����
		{
			return CT_SINGLE;
		}
	case 2:	//����
		{
			return (GetCardLogicValue(bCardData[0])==GetCardLogicValue(bCardData[1]))?CT_DOUBLE_LINE:CT_ERROR;
		}
	}

	//�����˿�
	tagAnalyseResult AnalyseResult;
	AnalysebCardData(bCardData,bCardCount,AnalyseResult);

	//ը���ж�
	if ((AnalyseResult.bFourCount==1)&&(bCardCount==4)) return CT_BOMB;

	//�����ж�
	if (AnalyseResult.bThreeCount>0)
	{
		//�����ж�
		bool bSeriesCard=false;
		if ((AnalyseResult.bThreeCount==1)||(AnalyseResult.bThreeLogicVolue[0]!=15))
		{
			BYTE i=0;
			for (i=1;i<AnalyseResult.bThreeCount;i++)
			{
				if (AnalyseResult.bThreeLogicVolue[i]!=(AnalyseResult.bThreeLogicVolue[0]-i)) break;
			}
			if (i==AnalyseResult.bThreeCount) bSeriesCard=true;
		}

		//�����ж�
		if (bSeriesCard==true)
		{
			//���ݶ���
			BYTE bSignedCount=bCardCount-AnalyseResult.bThreeCount*3;
			BYTE bDoubleCount=AnalyseResult.bDoubleCount+AnalyseResult.bFourCount*2;

			//���ͷ���
			if ((AnalyseResult.bThreeCount*3)==bCardCount) return CT_THREE_LINE;
			if ((AnalyseResult.bThreeCount==bSignedCount)&&((AnalyseResult.bThreeCount*3+bSignedCount)==bCardCount)) return CT_THREE_LINE_TAKE_SINGLE;
			if ((AnalyseResult.bThreeCount==bDoubleCount)&&((AnalyseResult.bThreeCount*3+bDoubleCount*2)==bCardCount)) return CT_THREE_LINE_TAKE_DOUBLE;
		}
	}

	//�����ж�
	if (AnalyseResult.bDoubleCount>0)
	{
		//�����ж�
		bool bSeriesCard=false;
		if ((AnalyseResult.bDoubleCount==1)||(AnalyseResult.bDoubleLogicVolue[0]!=15))
		{
			BYTE i=0;
			for (BYTE i=1;i<AnalyseResult.bDoubleCount;i++)
			{
				if (AnalyseResult.bDoubleLogicVolue[i]!=(AnalyseResult.bDoubleLogicVolue[0]-i)) break;
			}
			if (i==AnalyseResult.bDoubleCount) bSeriesCard=true;
		}

		//�����ж�
		if ((bSeriesCard==true)&&(AnalyseResult.bDoubleCount*2==bCardCount)) return CT_DOUBLE_LINE;
	}

	//�����ж�
	if ((AnalyseResult.bSignedCount>=5)&&(AnalyseResult.bSignedCount==bCardCount))
	{
		//��������
		bool bSeriesCard=false;
		BYTE bLogicValue=GetCardLogicValue(bCardData[0]);

		//�����ж�
		if (bLogicValue!=15)
		{
			BYTE i=0;
			for (BYTE i=1;i<AnalyseResult.bSignedCount;i++)
			{
				if (GetCardLogicValue(bCardData[i])!=(bLogicValue-i)) break;
			}
			if (i==AnalyseResult.bSignedCount) bSeriesCard=true;
		}

		//�����ж�
		if (bSeriesCard==true) return CT_SINGLE_LINE;
	}

	return CT_ERROR;
}

//�����˿�
void CGameLogic::SortCardList(BYTE bCardData[], BYTE bCardCount)
{
	//ת����ֵ
	BYTE bLogicVolue[MAX_COUNT];
	for (BYTE i=0;i<bCardCount;i++)	bLogicVolue[i]=GetCardLogicValue(bCardData[i]);	

	//�������
	bool bSorted=true;
	BYTE bTempData,bLast=bCardCount-1;
	do
	{
		bSorted=true;
		for (BYTE i=0;i<bLast;i++)
		{
			if ((bLogicVolue[i]<bLogicVolue[i+1])||
				((bLogicVolue[i]==bLogicVolue[i+1])&&(bCardData[i]<bCardData[i+1])))
			{
				//����λ��
				bTempData=bCardData[i];
				bCardData[i]=bCardData[i+1];
				bCardData[i+1]=bTempData;
				bTempData=bLogicVolue[i];
				bLogicVolue[i]=bLogicVolue[i+1];
				bLogicVolue[i+1]=bTempData;
				bSorted=false;
			}	
		}
		bLast--;
	} while(bSorted==false);

	return;
}

//�����˿�
void CGameLogic::RandCardList(BYTE bCardBuffer[], BYTE bBufferCount)
{
	//����׼��
	BYTE bCardData[sizeof(m_bCardListData)];
	CopyMemory(bCardData,m_bCardListData,sizeof(m_bCardListData));

	//�����˿�
	BYTE bRandCount=0,bPosition=0;
	do
	{
		bPosition=rand()%(bBufferCount-bRandCount);
		bCardBuffer[bRandCount++]=bCardData[bPosition];
		bCardData[bPosition]=bCardData[bBufferCount-bRandCount];
	} while (bRandCount<bBufferCount);

	return;
}

//ɾ���˿�
bool CGameLogic::RemoveCard(const BYTE bRemoveCard[], BYTE bRemoveCount, BYTE bCardData[], BYTE bCardCount)
{
	//��������
	ASSERT(bRemoveCount<=bCardCount);

	//�������
	BYTE bDeleteCount=0,bTempCardData[20];
	if (bCardCount>CountArray(bTempCardData)) return false;
	CopyMemory(bTempCardData,bCardData,bCardCount*sizeof(bCardData[0]));

	//�����˿�
	for (BYTE i=0;i<bRemoveCount;i++)
	{
		for (BYTE j=0;j<bCardCount;j++)
		{
			if (bRemoveCard[i]==bTempCardData[j])
			{
				bDeleteCount++;
				bTempCardData[j]=0;
				break;
			}
		}
	}
	if (bDeleteCount!=bRemoveCount) return false;

	//�����˿�
	BYTE bCardPos=0;
	for (BYTE i=0;i<bCardCount;i++)
	{
		if (bTempCardData[i]!=0) bCardData[bCardPos++]=bTempCardData[i];
	}

	return true;
}

//��Ч�ж�
bool CGameLogic::IsValidCard(BYTE cbCardData)
{
	//��������
	BYTE cbColor=GetCardColor(cbCardData);
	BYTE cbValue=GetCardValue(cbCardData);

	//��Ч�ж�
	switch (cbColor)
	{
	case 0x00:
		{
			return ((cbValue>=0x03)&&(cbValue<=0x0D));
		}
	case 0x10:
		{
			return ((cbValue==0x01)||((cbValue>=0x03)&&(cbValue<=0x0D)));
		}
	case 0x20:
		{
			return ((cbValue==0x01)||((cbValue>=0x03)&&(cbValue<=0x0D)));
		}
	case 0x30:
		{
			return ((cbValue>=0x01)&&(cbValue<=0x0D));
		}
	}

	return false;
}

//�߼���ֵ
BYTE CGameLogic::GetCardLogicValue(BYTE bCardData)
{
	//�˿�����
	BYTE bCardColor=GetCardColor(bCardData);
	BYTE bCardValue=GetCardValue(bCardData);

	//ת����ֵ
	return (bCardValue<=2)?(bCardValue+13):bCardValue;
}

//�Ա��˿�
bool CGameLogic::CompareCard(BYTE bFirstList[], BYTE bNextList[], BYTE bFirstCount, BYTE bNextCount)
{
	//��ȡ����
	BYTE bNextType=GetCardType(bNextList,bNextCount);
	BYTE bFirstType=GetCardType(bFirstList,bFirstCount);

	//�����ж�
	if (bFirstType==CT_ERROR) return false;

	//ը���ж�
	if ((bFirstType==CT_BOMB)&&(bNextType!=CT_BOMB)) return true;
	if ((bFirstType!=CT_BOMB)&&(bNextType==CT_BOMB)) return false;

	//�����ж�
	if ((bFirstType!=bNextType)||(bFirstCount!=bNextCount)) return false;

	//��ʼ�Ա�
	switch (bNextType)
	{
	case CT_BOMB:
	case CT_SINGLE:
	case CT_SINGLE_LINE:
	case CT_DOUBLE_LINE:
		{
			BYTE bNextLogicValue=GetCardLogicValue(bNextList[0]);
			BYTE bFirstLogicValue=GetCardLogicValue(bFirstList[0]);
			return bFirstLogicValue>bNextLogicValue;
		}
	case CT_THREE_LINE:
	case CT_THREE_LINE_TAKE_SINGLE:
	case CT_THREE_LINE_TAKE_DOUBLE:
		{
			tagAnalyseResult NextResult;
			tagAnalyseResult FirstResult;
			AnalysebCardData(bNextList,bNextCount,NextResult);
			AnalysebCardData(bFirstList,bFirstCount,FirstResult);
			return FirstResult.bThreeLogicVolue[0]>NextResult.bThreeLogicVolue[0];
		}
	}
	
	return false;
}

//�����˿�
void CGameLogic::AnalysebCardData(const BYTE bCardData[], BYTE bCardCount, tagAnalyseResult & AnalyseResult)
{
	//���ý��
	ZeroMemory(&AnalyseResult,sizeof(AnalyseResult));

	//�˿˷���
	for (BYTE i=0;i<bCardCount;i++)
	{
		//��������
		BYTE bSameCount=1;
		BYTE bSameCardData[4]={bCardData[i],0,0,0};
		BYTE bLogicValue=GetCardLogicValue(bCardData[i]);

		//��ȡͬ��
		for (int j=i+1;j<bCardCount;j++)
		{
			//�߼��Ա�
			if (GetCardLogicValue(bCardData[j])!=bLogicValue) break;

			//�����˿�
			bSameCardData[bSameCount++]=bCardData[j];
		}

		//������
		switch (bSameCount)
		{
		case 1:		//����
			{
				AnalyseResult.bSignedLogicVolue[AnalyseResult.bSignedCount]=bLogicValue;
				CopyMemory(&AnalyseResult.bSignedCardData[(AnalyseResult.bSignedCount++)*bSameCount],bSameCardData,bSameCount);
				break;
			}
		case 2:		//����
			{
				AnalyseResult.bDoubleLogicVolue[AnalyseResult.bDoubleCount]=bLogicValue;
				CopyMemory(&AnalyseResult.bDoubleCardData[(AnalyseResult.bDoubleCount++)*bSameCount],bSameCardData,bSameCount);
				break;
			}
		case 3:		//����
			{
				AnalyseResult.bThreeLogicVolue[AnalyseResult.bThreeCount]=bLogicValue;
				CopyMemory(&AnalyseResult.bThreeCardData[(AnalyseResult.bThreeCount++)*bSameCount],bSameCardData,bSameCount);
				break;
			}
		case 4:		//����
			{
				AnalyseResult.bFourLogicVolue[AnalyseResult.bFourCount]=bLogicValue;
				CopyMemory(&AnalyseResult.bFourCardData[(AnalyseResult.bFourCount++)*bSameCount],bSameCardData,bSameCount);
				break;
			}
		}

		//���õ���
		i+=bSameCount-1;
	}

	return;
}

//�����ж�
bool CGameLogic::SearchOutCard(BYTE bCardData[], BYTE bCardCount, BYTE bTurnCardData[], BYTE bTurnCardCount, BYTE bTurnOutType, tagOutCardResult & OutCardResult)
{
	//���ý��
	OutCardResult.cbCardCount=0;

	//��������
	switch (bTurnOutType)
	{
	case CT_ERROR:					//��������
		{
			//��ȡ��ֵ
			BYTE bLogicValue=GetCardLogicValue(bCardData[bCardCount-1]);

			//�����ж�
			BYTE cbSameCount=1;
			for (BYTE i=1;i<bCardCount;i++)
			{
				if (GetCardLogicValue(bCardData[bCardCount-i-1])==bLogicValue) cbSameCount++;
				else break;
			}

			//��ɴ���
			if (cbSameCount>1)
			{
				OutCardResult.cbCardCount=cbSameCount;
				for (BYTE j=0;j<cbSameCount;j++) OutCardResult.cbResultCard[j]=bCardData[bCardCount-1-j];
				return true;
			}

			//���ƴ���
			OutCardResult.cbCardCount=1;
			OutCardResult.cbResultCard[0]=bCardData[bCardCount-1];

			return true;
		}
	case CT_SINGLE:					//��������
		{
			//��ȡ��ֵ
			BYTE bLogicValue=GetCardLogicValue(bTurnCardData[0]);

			//�Աȴ�С
			for (BYTE i=0;i<bCardCount;i++)
			{
				if (GetCardLogicValue(bCardData[bCardCount-i-1])>bLogicValue)
				{
					OutCardResult.cbCardCount=1;
					OutCardResult.cbResultCard[0]=bCardData[bCardCount-i-1];
					return true;
				}
			}

			break;
		}
	case CT_SINGLE_LINE:			//��������
		{
			//�����ж�
			if (bCardCount<bTurnCardCount) break;

			//��ȡ��ֵ
			BYTE bLogicValue=GetCardLogicValue(bTurnCardData[0]);

			//��������
			for (BYTE i=(bTurnCardCount-1);i<bCardCount;i++)
			{
				//��ȡ��ֵ
				BYTE bHandLogicValue=GetCardLogicValue(bCardData[bCardCount-i-1]);

				//�����ж�
				if (bHandLogicValue>=15) break;
				if (bHandLogicValue<=bLogicValue) continue;

				//��������
				BYTE bLineCount=0;
				for (BYTE j=(bCardCount-i-1);j<bCardCount;j++)
				{
					if ((GetCardLogicValue(bCardData[j])+bLineCount)==bHandLogicValue) 
					{
						//��������
						OutCardResult.cbResultCard[bLineCount++]=bCardData[j];

						//����ж�
						if (bLineCount==bTurnCardCount)
						{
							OutCardResult.cbCardCount=bTurnCardCount;
							return true;
						}
					}
				}
			}

			break;
		}
	case CT_DOUBLE_LINE:	//��������
		{
			//�����ж�
			if (bCardCount<bTurnCardCount) break;

			//��ȡ��ֵ
			BYTE bLogicValue=GetCardLogicValue(bTurnCardData[0]);

			//��������
			for (BYTE i=(bTurnCardCount-1);i<bCardCount;i++)
			{
				//��ȡ��ֵ
				BYTE bHandLogicValue=GetCardLogicValue(bCardData[bCardCount-i-1]);

				//�����ж�
				if (bHandLogicValue<=bLogicValue) continue;
				if ((bTurnCardCount>2)&&(bHandLogicValue>=15)) break;

				//��������
				BYTE bLineCount=0;
				for (BYTE j=(bCardCount-i-1);j<(bCardCount-1);j++)
				{
					if (((GetCardLogicValue(bCardData[j])+bLineCount)==bHandLogicValue)
						&&((GetCardLogicValue(bCardData[j+1])+bLineCount)==bHandLogicValue))
					{
						//��������
						OutCardResult.cbResultCard[bLineCount*2]=bCardData[j];
						OutCardResult.cbResultCard[(bLineCount++)*2+1]=bCardData[j+1];

						//����ж�
						if (bLineCount*2==bTurnCardCount)
						{
							OutCardResult.cbCardCount=bTurnCardCount;
							return true;
						}
					}
				}
			}

			break;
		}
	case CT_THREE_LINE:				//��������
	case CT_THREE_LINE_TAKE_SINGLE:	//����һ��
	case CT_THREE_LINE_TAKE_DOUBLE:	//����һ��
		{
			//�����ж�
			if (bCardCount<bTurnCardCount) break;

			//��ȡ��ֵ
			BYTE bLogicValue=0;
			for (BYTE i=0;i<bTurnCardCount-2;i++)
			{
				bLogicValue=GetCardLogicValue(bTurnCardData[i]);
				if (GetCardLogicValue(bTurnCardData[i+1])!=bLogicValue) continue;
				if (GetCardLogicValue(bTurnCardData[i+2])!=bLogicValue) continue;
				break;
			}

			//������ֵ
			BYTE bTurnLineCount=0;
			if (bTurnOutType==CT_THREE_LINE_TAKE_SINGLE) bTurnLineCount=bTurnCardCount/4;
			else if (bTurnOutType==CT_THREE_LINE_TAKE_DOUBLE) bTurnLineCount=bTurnCardCount/5;
			else bTurnLineCount=bTurnCardCount/3;

			//��������
			for (BYTE i=bTurnLineCount*3-1;i<bCardCount;i++)
			{
				//��ȡ��ֵ
				BYTE bHandLogicValue=GetCardLogicValue(bCardData[bCardCount-i-1]);

				//�����ж�
				if (bHandLogicValue<=bLogicValue) continue;
				if ((bTurnLineCount>1)&&(bHandLogicValue>=15)) break;

				//��������
				BYTE bLineCount=0;
				for (BYTE j=(bCardCount-i-1);j<(bCardCount-2);j++)
				{
					//�����ж�
					if ((GetCardLogicValue(bCardData[j])+bLineCount)!=bHandLogicValue) continue;
					if ((GetCardLogicValue(bCardData[j+1])+bLineCount)!=bHandLogicValue) continue;
					if ((GetCardLogicValue(bCardData[j+2])+bLineCount)!=bHandLogicValue) continue;

					//��������
					OutCardResult.cbResultCard[bLineCount*3]=bCardData[j];
					OutCardResult.cbResultCard[bLineCount*3+1]=bCardData[j+1];
					OutCardResult.cbResultCard[(bLineCount++)*3+2]=bCardData[j+2];

					//����ж�
					if (bLineCount==bTurnLineCount)
					{
						//��������
						OutCardResult.cbCardCount=bLineCount*3;

						//�����˿�
						BYTE bLeftCardData[MAX_COUNT];
						BYTE bLeftCount=bCardCount-OutCardResult.cbCardCount;
						CopyMemory(bLeftCardData,bCardData,sizeof(BYTE)*bCardCount);
						RemoveCard(OutCardResult.cbResultCard,OutCardResult.cbCardCount,bLeftCardData,bCardCount);

						//�����˿�
						tagAnalyseResult AnalyseResultLeft;
						AnalysebCardData(bLeftCardData,bLeftCount,AnalyseResultLeft);

						//���ƴ���
						if (bTurnOutType==CT_THREE_LINE_TAKE_SINGLE)
						{
							//��ȡ����
							for (BYTE k=0;k<AnalyseResultLeft.bSignedCount;k++)
							{
								//��ֹ�ж�
								if (OutCardResult.cbCardCount==bTurnCardCount) break;

								//�����˿�
								BYTE bIndex=AnalyseResultLeft.bSignedCount-k-1;
								BYTE bSignedCard=AnalyseResultLeft.bSignedCardData[bIndex];
								OutCardResult.cbResultCard[OutCardResult.cbCardCount++]=bSignedCard;
							}

							//��ȡ����
							for (BYTE k=0;k<AnalyseResultLeft.bDoubleCount*2;k++)
							{
								//��ֹ�ж�
								if (OutCardResult.cbCardCount==bTurnCardCount) break;

								//�����˿�
								BYTE bIndex=(AnalyseResultLeft.bDoubleCount*2-k-1);
								BYTE bSignedCard=AnalyseResultLeft.bDoubleCardData[bIndex];
								OutCardResult.cbResultCard[OutCardResult.cbCardCount++]=bSignedCard;
							}

							//��ȡ����
							for (BYTE k=0;k<AnalyseResultLeft.bThreeCount*3;k++)
							{
								//��ֹ�ж�
								if (OutCardResult.cbCardCount==bTurnCardCount) break;

								//�����˿�
								BYTE bIndex=(AnalyseResultLeft.bThreeCount*3-k-1);
								BYTE bSignedCard=AnalyseResultLeft.bThreeCardData[bIndex];
								OutCardResult.cbResultCard[OutCardResult.cbCardCount++]=bSignedCard;
							}

							//��ȡ����
							for (BYTE k=0;k<AnalyseResultLeft.bFourCount*4;k++)
							{
								//��ֹ�ж�
								if (OutCardResult.cbCardCount==bTurnCardCount) break;

								//�����˿�
								BYTE bIndex=(AnalyseResultLeft.bFourCount*4-k-1);
								BYTE bSignedCard=AnalyseResultLeft.bFourCardData[bIndex];
								OutCardResult.cbResultCard[OutCardResult.cbCardCount++]=bSignedCard;
							}
						}

						//���ƴ���
						if (bTurnOutType==CT_THREE_LINE_TAKE_DOUBLE)
						{
							//��ȡ����
							for (BYTE k=0;k<AnalyseResultLeft.bDoubleCount;k++)
							{
								//��ֹ�ж�
								if (OutCardResult.cbCardCount==bTurnCardCount) break;

								//�����˿�
								BYTE bIndex=(AnalyseResultLeft.bDoubleCount-k-1)*2;
								BYTE bCardData1=AnalyseResultLeft.bDoubleCardData[bIndex];
								BYTE bCardData2=AnalyseResultLeft.bDoubleCardData[bIndex+1];
								OutCardResult.cbResultCard[OutCardResult.cbCardCount++]=bCardData1;
								OutCardResult.cbResultCard[OutCardResult.cbCardCount++]=bCardData2;
							}

							//��ȡ����
							for (BYTE k=0;k<AnalyseResultLeft.bThreeCount;k++)
							{
								//��ֹ�ж�
								if (OutCardResult.cbCardCount==bTurnCardCount) break;

								//�����˿�
								BYTE bIndex=(AnalyseResultLeft.bThreeCount-k-1)*3;
								BYTE bCardData1=AnalyseResultLeft.bThreeCardData[bIndex];
								BYTE bCardData2=AnalyseResultLeft.bThreeCardData[bIndex+1];
								OutCardResult.cbResultCard[OutCardResult.cbCardCount++]=bCardData1;
								OutCardResult.cbResultCard[OutCardResult.cbCardCount++]=bCardData2;
							}

							//��ȡ����
							for (BYTE k=0;k<AnalyseResultLeft.bFourCount;k++)
							{
								//��ֹ�ж�
								if (OutCardResult.cbCardCount==bTurnCardCount) break;

								//�����˿�
								BYTE bIndex=(AnalyseResultLeft.bFourCount-k-1)*4;
								BYTE bCardData1=AnalyseResultLeft.bFourCardData[bIndex];
								BYTE bCardData2=AnalyseResultLeft.bFourCardData[bIndex+1];
								OutCardResult.cbResultCard[OutCardResult.cbCardCount++]=bCardData1;
								OutCardResult.cbResultCard[OutCardResult.cbCardCount++]=bCardData2;
							}
						}

						//����ж�
						if (OutCardResult.cbCardCount!=bTurnCardCount)
						{
							OutCardResult.cbCardCount=0;
							return false;
						}

						return true;
					}
				}
			}

			break;
		}
	case CT_BOMB:						//ը������
		{
			//�����ж�
			if (bCardCount<bTurnCardCount) break;

			//��ȡ��ֵ
			BYTE bLogicValue=GetCardLogicValue(bTurnCardData[0]);

			//����ը��
			for (BYTE i=3;i<bCardCount;i++)
			{
				//��ȡ��ֵ
				BYTE bHandLogicValue=GetCardLogicValue(bCardData[bCardCount-i-1]);

				//�����ж�
				if (bHandLogicValue<=bLogicValue) continue;

				//ը���ж�
				BYTE bTempLogicValue=GetCardLogicValue(bCardData[bCardCount-i-1]);
				BYTE j=0;
				for (BYTE j=1;j<4;j++)
				{
					if (GetCardLogicValue(bCardData[bCardCount+j-i-1])!=bTempLogicValue) break;
				}
				if (j!=4) continue;

				//��ɴ���
				OutCardResult.cbCardCount=bTurnCardCount;
				OutCardResult.cbResultCard[0]=bCardData[bCardCount-i-1];
				OutCardResult.cbResultCard[1]=bCardData[bCardCount-i];
				OutCardResult.cbResultCard[2]=bCardData[bCardCount-i+1];
				OutCardResult.cbResultCard[3]=bCardData[bCardCount-i+2];

				return true;
			}

			return false;
		}
	}

	//ը������
	for (BYTE i=3;i<bCardCount;i++)
	{
		//��ȡ��ֵ
		BYTE bHandLogicValue=GetCardLogicValue(bCardData[bCardCount-i-1]);

		//ը���ж�
		BYTE bTempLogicValue=GetCardLogicValue(bCardData[bCardCount-i-1]);
		BYTE j=0;
		for (BYTE j=1;j<4;j++)
		{
			if (GetCardLogicValue(bCardData[bCardCount+j-i-1])!=bTempLogicValue) break;
		}
		if (j!=4) continue;

		//��ɴ���
		OutCardResult.cbCardCount=4;
		OutCardResult.cbResultCard[0]=bCardData[bCardCount-i-1];
		OutCardResult.cbResultCard[1]=bCardData[bCardCount-i];
		OutCardResult.cbResultCard[2]=bCardData[bCardCount-i+1];
		OutCardResult.cbResultCard[3]=bCardData[bCardCount-i+2];

		return true;
	}

	return false;
}

//////////////////////////////////////////////////////////////////////////
