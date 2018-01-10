#include "StdAfx.h"
#include "GameLogic.h"

//////////////////////////////////////////////////////////////////////////
//��������
const BYTE cbIndexCount=4;


//�˿�����
const BYTE	CGameLogic::m_bCardListData[52]=
{
	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,	//���� A - K
	0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,0x1B,0x1C,0x1D,	//÷�� A - K
	0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2A,0x2B,0x2C,0x2D,	//���� A - K
	0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3A,0x3B,0x3C,0x3D	//���� A - K
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
BYTE CGameLogic::GetCardType(BYTE bCardData[], BYTE bCardCount, BYTE & bMaxCardData)
{
	//����У��
	ASSERT(bCardCount==3 || bCardCount==5 ) ;
	if(bCardCount!=3 && bCardCount!=5) return CT_INVALID ;

	bMaxCardData = 0;

	//�����˿�
	tagAnalyseData AnalyseData ;
	ZeroMemory(&AnalyseData, sizeof(AnalyseData)) ;
	AnalyseCard(bCardData , bCardCount , AnalyseData);
	SortCardList(bCardData, bCardCount, enDescend);	
	
	//�����˿�
	tagAnalyseResult AnalyseResult;
	ZeroMemory(&AnalyseResult, sizeof(AnalyseResult));
	AnalysebCardData(bCardData, bCardCount, AnalyseResult);

	//��ʼ����
	switch (bCardCount)
	{
	case 3:	//��������
		{
			//��������
			if(3==AnalyseData.bOneCount)
			{
				bMaxCardData = GetCardLogicValue(AnalyseResult.cbCardData[0][0]);
				return CT_SINGLE;
			}

			//�Դ�һ��
			if(1==AnalyseData.bTwoCount && 1==AnalyseData.bOneCount)
			{
				bMaxCardData = GetCardLogicValue(AnalyseResult.cbCardData[1][0]);
				return CT_ONE_DOUBLE;
			}

			//��������
			if(1==AnalyseData.bThreeCount)
			{
				bMaxCardData = GetCardLogicValue(AnalyseResult.cbCardData[2][0]);
				return CT_THREE ;
			}
			//��������
			return CT_INVALID ;
		}
	case 5:	//��������
		{
			bool bFlushNoA	    = false; 
			bool bFlushFirstA	= false;
			bool bFlushBackA	= false;

			//A���ں�
			if(14==GetCardLogicValue(bCardData[0]) && 10==GetCardLogicValue(bCardData[4]))
				bFlushBackA = true ;
			else
				bFlushNoA = true ;
			for(BYTE i=0 ; i<4 ; ++i)
			{
				if(1!=GetCardLogicValue(bCardData[i])-GetCardLogicValue(bCardData[i+1])) 
				{
					bFlushBackA = false;
					bFlushNoA   = false;
				}
			}

			//A����ǰ
			if(false==bFlushBackA && false==bFlushNoA && 14==GetCardLogicValue(bCardData[0]))
			{
				bFlushFirstA = true;
				for(BYTE i=1 ; i<4 ; ++i) 
					if(1!=GetCardLogicValue(bCardData[i])-GetCardLogicValue(bCardData[i+1])) 
						bFlushFirstA = false ;
				if(2!=GetCardLogicValue(bCardData[4]))
					bFlushFirstA = false ;
			}

			//ͬ������
			if(false==bFlushBackA && false==bFlushNoA && false==bFlushFirstA)
			{
				if(true==AnalyseData.bSameColor)
				{
					bMaxCardData = GetCardLogicValue(AnalyseResult.cbCardData[0][0]);
					return CT_FIVE_FLUSH;
				}
			}
			else if(true==bFlushNoA)
			{
				bMaxCardData = GetCardLogicValue(AnalyseResult.cbCardData[0][0]);
				//��˳����
				if(false==AnalyseData.bSameColor) return CT_FIVE_MIXED_FLUSH_NO_A;
				//ͬ��˳��
				else return CT_FIVE_STRAIGHT_FLUSH;
			}
			else if(true==bFlushFirstA)
			{
				bMaxCardData = GetCardLogicValue(AnalyseResult.cbCardData[0][0]);
				//��˳����
				if(false==AnalyseData.bSameColor) return CT_FIVE_MIXED_FLUSH_FIRST_A;
				//ͬ��˳��
				else							  return CT_FIVE_STRAIGHT_FLUSH_FIRST_A;
			}
			else if(true==bFlushBackA)
			{
				bMaxCardData = GetCardLogicValue(AnalyseResult.cbCardData[0][0]);
				//��˳����
				if(false==AnalyseData.bSameColor) return CT_FIVE_MIXED_FLUSH_NO_A;
				//ͬ��˳��
				else							  return CT_FIVE_STRAIGHT_FLUSH;
			}

			//�Ĵ�����
			if(1==AnalyseData.bFourCount && 1==AnalyseData.bOneCount)
			{
				bMaxCardData = GetCardLogicValue(AnalyseResult.cbCardData[3][0]);
				return	 CT_FIVE_FOUR_ONE;
			}

			//����һ��
			if(1==AnalyseData.bThreeCount && 1==AnalyseData.bTwoCount) 
			{
				bMaxCardData = GetCardLogicValue(AnalyseResult.cbCardData[2][0]);
				return	 CT_FIVE_THREE_DEOUBLE;
			}

			//��������
			if(1==AnalyseData.bThreeCount && 2==AnalyseData.bOneCount)
			{
				bMaxCardData = GetCardLogicValue(AnalyseResult.cbCardData[2][0]);
				return	 CT_THREE;
			}

			//��������
			if(2==AnalyseData.bTwoCount && 1==AnalyseData.bOneCount)
			{
				bMaxCardData = GetCardLogicValue(AnalyseResult.cbCardData[1][0]);
				return	 CT_TWO_DOUBLE;
			}

			//ֻ��һ��
			if(1==AnalyseData.bTwoCount && 3==AnalyseData.bOneCount)
			{
				bMaxCardData = GetCardLogicValue(AnalyseResult.cbCardData[1][0]);
				return	 CT_ONE_DOUBLE;
			}

			//��������
			if(5==AnalyseData.bOneCount && false==AnalyseData.bSameColor)
			{
				bMaxCardData = GetCardLogicValue(AnalyseResult.cbCardData[0][0]);
				return  CT_SINGLE;
			}

			//��������
			return CT_INVALID;
		}
	}

	return CT_INVALID;
}

//�ж���������
//������13��
BYTE CGameLogic::GetSpecialType(BYTE bHandCardData[], BYTE bCardCount)
{
	ASSERT(bCardCount == HAND_CARD_COUNT);
	if (bCardCount != HAND_CARD_COUNT) return CT_EX_INVALID;

	//�����˿�
	BYTE bCardData[13];
	CopyMemory(bCardData , bHandCardData , sizeof(BYTE)*13);
	//�����˿�
	SortCardList(bCardData, bCardCount, enDescend);

	//���ý��
	tagAnalyseData AnalyseData;
	ZeroMemory(&AnalyseData, sizeof(AnalyseData));

	//��������
	BYTE bSameCount = 1,
		bCardValueTemp=0,
		bFirstCardIndex = 0;	//��¼�±�

	BYTE bLogicValue = GetCardLogicValue(bCardData[0]);
	BYTE bCardColor = GetCardColor(bCardData[0]) ;
	//////////////////////////////////////////////////////////////////////////
	//�˿˷���
	for (BYTE i=1;i<bCardCount;i++)
	{
		//��ȡ�˿�
		bCardValueTemp=GetCardLogicValue(bCardData[i]);
		if (bCardValueTemp==bLogicValue) bSameCount++;

		//������
		if ((bCardValueTemp!=bLogicValue)||(i==(bCardCount-1)))
		{
			switch (bSameCount)
			{
			case 1:		//һ��
				break; 
			case 2:		//����
				{
					AnalyseData.bTwoFirst[AnalyseData.bTwoCount] = bFirstCardIndex ;
					AnalyseData.bTwoCount++ ;
					break;
				}
			case 3:		//����
				{
					AnalyseData.bThreeFirst[AnalyseData.bThreeCount] = bFirstCardIndex ;
					AnalyseData.bThreeCount++ ;
					break;
				}
			case 4:		//����
				{
					AnalyseData.bFourFirst[AnalyseData.bFourCount] = bFirstCardIndex ;
					AnalyseData.bFourCount++ ;
					break;
				}
				break;
			}
		}

		//���ñ���
		if (bCardValueTemp!=bLogicValue)
		{
			if(bSameCount==1)
			{
				if(i!=bCardCount-1)
				{
					AnalyseData.bOneFirst[AnalyseData.bOneCount] = bFirstCardIndex ;
					AnalyseData.bOneCount++ ;
				}
				else
				{
					AnalyseData.bOneFirst[AnalyseData.bOneCount] = bFirstCardIndex ;
					AnalyseData.bOneCount++ ;
					AnalyseData.bOneFirst[AnalyseData.bOneCount] = i ;
					AnalyseData.bOneCount++ ;				
				}
			}
			else
			{
				if(i==bCardCount-1)
				{
					AnalyseData.bOneFirst[AnalyseData.bOneCount] = i ;
					AnalyseData.bOneCount++ ;
				}
			}
			bSameCount=1;
			bLogicValue=bCardValueTemp;
			bFirstCardIndex = i ;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	//��������
	if (IsStraightDragon(bCardData, bCardCount)==true)
		return CT_EX_ZHIZUNQINGLONG;

	//һ����
	if ((IsLinkCard(bCardData, bCardCount)==true))
		return CT_EX_YITIAOLONG;
	
	//���԰�
	if (AnalyseData.bTwoCount==6 && AnalyseData.bOneCount==1)
		return CT_EX_LIUDUIBAN;

	//��˳��
	tagSearchCardResult LineCardResult;
	BYTE cbLineCardResult5 = SearchLineCardType(bCardData, bCardCount, 5, &LineCardResult);
	if (cbLineCardResult5 >= 2)
	{	
		if (IsLinkCard(LineCardResult.cbResultCard[0], LineCardResult.cbCardCount[0]))
		{		
			BYTE cbTempData[13] = {0};
			BYTE cbTempCount = 13;
			CopyMemory(cbTempData, bCardData, sizeof(bCardData));

			RemoveCard(LineCardResult.cbResultCard[0], LineCardResult.cbCardCount[0],cbTempData,cbTempCount);
			cbTempCount -= LineCardResult.cbCardCount[0];

			cbLineCardResult5 = SearchLineCardType(cbTempData, cbTempCount, 5, &LineCardResult);
			if (cbLineCardResult5 >= 1)
			{
				if (IsLinkCard(LineCardResult.cbResultCard[0], LineCardResult.cbCardCount[0]))
				{
					BYTE cbTempData1[13] = {0};
					CopyMemory(cbTempData1, cbTempData, sizeof(BYTE)*cbTempCount);

					RemoveCard(LineCardResult.cbResultCard[0], LineCardResult.cbCardCount[0], cbTempData1, cbTempCount);
					cbTempCount -= LineCardResult.cbCardCount[0];

					if (IsLinkCard(cbTempData1, cbTempCount))
						return CT_EX_SANSHUNZI;
				}
			}
		}
	}

	//��ͬ��
	tagSearchCardResult SameColorResult;
	ZeroMemory(&SameColorResult, sizeof(SameColorResult));
	BYTE cbSameColorResult5 = SearchSameColorType(bCardData, bCardCount, 5, &SameColorResult);
	if (cbSameColorResult5 >= 2)
	{	
		if (IsSameColorCard(SameColorResult.cbResultCard[0], SameColorResult.cbCardCount[0]))
		{		
			BYTE cbTempData[13] = {0};
			BYTE cbTempCount = 13;
			CopyMemory(cbTempData, bCardData, sizeof(bCardData));

			//����˿�
			RemoveCard(SameColorResult.cbResultCard[0], SameColorResult.cbCardCount[0],cbTempData,cbTempCount);
			cbTempCount -= SameColorResult.cbCardCount[0];

			ZeroMemory(&SameColorResult, sizeof(SameColorResult));
			cbSameColorResult5 = SearchSameColorType(cbTempData, cbTempCount, 5, &SameColorResult);
			if (cbSameColorResult5 >= 1)
			{
				if (IsSameColorCard(SameColorResult.cbResultCard[0], SameColorResult.cbCardCount[0]))
				{
					BYTE cbTempData1[13] = {0};
					CopyMemory(cbTempData1, cbTempData, sizeof(BYTE)*cbTempCount);

					//�ж��˿�
					RemoveCard(SameColorResult.cbResultCard[0], SameColorResult.cbCardCount[0], cbTempData1, cbTempCount);
					cbTempCount -= SameColorResult.cbCardCount[0];

					if (IsSameColorCard(cbTempData1, cbTempCount)) 
						return CT_EX_SANTONGHUA;
				}
			}
		}
	}

	//��������
	return CT_EX_INVALID;
}

//��ȡ������
//bSpecCardData[][]
//˳��Ϊǰ�к�
BYTE CGameLogic::GetSpecialCardData(BYTE bHandCardData[HAND_CARD_COUNT], BYTE bHandCardCount, BYTE bSpecCardData[3][5])
{
	ASSERT(bHandCardCount == HAND_CARD_COUNT);
	if (bHandCardCount != HAND_CARD_COUNT) return 0;

	//��ս��
	ZeroMemory(bSpecCardData, sizeof(bSpecCardData));

	//��������
	BYTE bCardData[HAND_CARD_COUNT];
	BYTE bCardCount = bHandCardCount;

	//�����˿�
	CopyMemory(bCardData, bHandCardData, sizeof(BYTE)*bHandCardCount);
	//�����˿�
	SortCardList(bCardData, bCardCount, enDescend);

	//���ý��
	tagAnalyseData AnalyseData;
	ZeroMemory(&AnalyseData, sizeof(AnalyseData));

	//��������
	BYTE bSameCount = 1,
		bCardValueTemp=0,
		bFirstCardIndex = 0;	//��¼�±�

	BYTE bLogicValue = GetCardLogicValue(bCardData[0]);
	BYTE bCardColor = GetCardColor(bCardData[0]) ;	
	//�˿˷���
	//////////////////////////////////////////////////////////////////////////
	for (BYTE i=1;i<bCardCount;i++)
	{
		//��ȡ�˿�
		bCardValueTemp=GetCardLogicValue(bCardData[i]);
		if (bCardValueTemp==bLogicValue) bSameCount++;

		//������
		if ((bCardValueTemp!=bLogicValue)||(i==(bCardCount-1)))
		{
			switch (bSameCount)
			{
			case 1:		//һ��
				break; 
			case 2:		//����
				{
					AnalyseData.bTwoFirst[AnalyseData.bTwoCount] = bFirstCardIndex ;
					AnalyseData.bTwoCount++ ;
					break;
				}
			case 3:		//����
				{
					AnalyseData.bThreeFirst[AnalyseData.bThreeCount] = bFirstCardIndex ;
					AnalyseData.bThreeCount++ ;
					break;
				}
			case 4:		//����
				{
					AnalyseData.bFourFirst[AnalyseData.bFourCount] = bFirstCardIndex ;
					AnalyseData.bFourCount++ ;
					break;
				}
				break;
			}
		}

		//���ñ���
		if (bCardValueTemp!=bLogicValue)
		{
			if(bSameCount==1)
			{
				if(i!=bCardCount-1)
				{
					AnalyseData.bOneFirst[AnalyseData.bOneCount] = bFirstCardIndex ;
					AnalyseData.bOneCount++ ;
				}
				else
				{
					AnalyseData.bOneFirst[AnalyseData.bOneCount] = bFirstCardIndex ;
					AnalyseData.bOneCount++ ;
					AnalyseData.bOneFirst[AnalyseData.bOneCount] = i ;
					AnalyseData.bOneCount++ ;				
				}
			}
			else
			{
				if(i==bCardCount-1)
				{
					AnalyseData.bOneFirst[AnalyseData.bOneCount] = i ;
					AnalyseData.bOneCount++ ;
				}
			}
			bSameCount=1;
			bLogicValue=bCardValueTemp;
			bFirstCardIndex = i ;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	//��������
	if (IsStraightDragon(bCardData, bCardCount) == true)
	{	
		CopyMemory(bSpecCardData[2], &bCardData[0], sizeof(BYTE)*5);
		CopyMemory(bSpecCardData[1], &bCardData[5], sizeof(BYTE)*5);
		CopyMemory(bSpecCardData  , &bCardData[10], sizeof(BYTE)*3);
		
		return 1;
	}

	//һ����
	if (IsLinkCard(bCardData, bCardCount) == true)
	{
		CopyMemory(bSpecCardData[2], &bCardData[0], sizeof(BYTE)*5);
		CopyMemory(bSpecCardData[1], &bCardData[5], sizeof(BYTE)*5);
		CopyMemory(bSpecCardData  , &bCardData[10], sizeof(BYTE)*3);
	
		return 1;
	}

	//���԰� ��e.g. AA KK QQ JJ 99 77 2��
	if (AnalyseData.bTwoCount==6 && AnalyseData.bOneCount==1)
	{
		BYTE bTempCardData[13] = {0};
		CopyMemory(bTempCardData, bCardData, sizeof(BYTE)*13);

		BYTE bTempSegCard[5];

		//����� (AA KK 2)
		ZeroMemory(bTempSegCard, sizeof(bTempSegCard));
		CopyMemory(&bTempSegCard,	 &bTempCardData[AnalyseData.bTwoFirst[0]], sizeof(BYTE)*2);
		CopyMemory(&bTempSegCard[2], &bTempCardData[AnalyseData.bTwoFirst[1]], sizeof(BYTE)*2);
		CopyMemory(&bTempSegCard[4], &bTempCardData[AnalyseData.bOneFirst[0]], sizeof(BYTE)*1);
		CopyMemory(bSpecCardData[2], bTempSegCard, sizeof(BYTE)*5);

		//����ж� (QQ JJ 7)
		ZeroMemory(bTempSegCard, sizeof(bTempSegCard));
		CopyMemory(&bTempSegCard,	 &bTempCardData[AnalyseData.bTwoFirst[2]], sizeof(BYTE)*2);
		CopyMemory(&bTempSegCard[2], &bTempCardData[AnalyseData.bTwoFirst[3]], sizeof(BYTE)*2);
		CopyMemory(&bTempSegCard[4], &bTempCardData[AnalyseData.bTwoFirst[5]], sizeof(BYTE)*1);
		CopyMemory(bSpecCardData[1], bTempSegCard, sizeof(BYTE)*5);

		//���ǰ�� (99 2)
		ZeroMemory(bTempSegCard, sizeof(bTempSegCard));
		CopyMemory(&bTempSegCard,	 &bTempCardData[AnalyseData.bTwoFirst[4]], sizeof(BYTE)*2);
		CopyMemory(&bTempSegCard[2], &bTempCardData[AnalyseData.bTwoFirst[5]+1], sizeof(BYTE)*1);
		CopyMemory(bSpecCardData[0], bTempSegCard, sizeof(BYTE)*3);

		return 1;
	}

	//��˳��
	tagSearchCardResult LineCardResult;
	BYTE cbLineCardResult5 = SearchLineCardType(bCardData, bCardCount, 5, &LineCardResult);
	if (cbLineCardResult5 >= 2)
	{	
		if (IsLinkCard(LineCardResult.cbResultCard[0], LineCardResult.cbCardCount[0]))
		{		
			BYTE cbTempData[13] = {0};
			BYTE cbTempCount = 13;
			CopyMemory(cbTempData, bCardData, sizeof(bCardData));

			//����˿�
			CopyMemory(bSpecCardData[2], LineCardResult.cbResultCard[0], sizeof(BYTE)*LineCardResult.cbCardCount[0]);
			RemoveCard(LineCardResult.cbResultCard[0], LineCardResult.cbCardCount[0],cbTempData,cbTempCount);
			cbTempCount -= LineCardResult.cbCardCount[0];

			cbLineCardResult5 = SearchLineCardType(cbTempData, cbTempCount, 5, &LineCardResult);
			if (cbLineCardResult5 >= 1)
			{
				if (IsLinkCard(LineCardResult.cbResultCard[0], LineCardResult.cbCardCount[0]))
				{
					BYTE cbTempData1[13] = {0};
					CopyMemory(cbTempData1, cbTempData, sizeof(BYTE)*cbTempCount);

					//�ж��˿�
					CopyMemory(bSpecCardData[1], LineCardResult.cbResultCard[0], sizeof(BYTE)*LineCardResult.cbCardCount[0]);
					RemoveCard(LineCardResult.cbResultCard[0], LineCardResult.cbCardCount[0], cbTempData1, cbTempCount);
					cbTempCount -= LineCardResult.cbCardCount[0];

					if (IsLinkCard(cbTempData1, cbTempCount))
					{
						//ǰ���˿�
						CopyMemory(bSpecCardData[0], cbTempData1, sizeof(BYTE)*3);

						//��ֹ����
						if (enCRGreater != CompareCard(bSpecCardData[1], bSpecCardData[2], 5, 5, true))
						{
							BYTE cbCardTemp[5] = {0};
							CopyMemory(cbCardTemp, bSpecCardData[1], sizeof(cbCardTemp));
							CopyMemory(bSpecCardData[1], bSpecCardData[2], sizeof(bSpecCardData[1]));
							CopyMemory(bSpecCardData[2], cbCardTemp, sizeof(bSpecCardData[2]));
						}

						return 1;
					}
				}
			}
		}
	}

	//��ͬ��
	tagSearchCardResult SameColorResult;
	ZeroMemory(&SameColorResult, sizeof(SameColorResult));
	BYTE cbSameColorResult5 = SearchSameColorType(bCardData, bCardCount, 5, &SameColorResult);
	if (cbSameColorResult5 >= 2)
	{	
		if (IsSameColorCard(SameColorResult.cbResultCard[0], SameColorResult.cbCardCount[0]))
		{		
			BYTE cbTempData[13] = {0};
			BYTE cbTempCount = 13;
			CopyMemory(cbTempData, bCardData, sizeof(bCardData));

			//����˿�
			CopyMemory(bSpecCardData[2], SameColorResult.cbResultCard[0], sizeof(BYTE)*SameColorResult.cbCardCount[0]);
			RemoveCard(SameColorResult.cbResultCard[0], SameColorResult.cbCardCount[0],cbTempData,cbTempCount);
			cbTempCount -= SameColorResult.cbCardCount[0];

			ZeroMemory(&SameColorResult, sizeof(SameColorResult));
			cbSameColorResult5 = SearchSameColorType(cbTempData, cbTempCount, 5, &SameColorResult);
			if (cbSameColorResult5 >= 1)
			{
				if (IsSameColorCard(SameColorResult.cbResultCard[0], SameColorResult.cbCardCount[0]))
				{
					BYTE cbTempData1[13] = {0};
					CopyMemory(cbTempData1, cbTempData, sizeof(BYTE)*cbTempCount);

					//�ж��˿�
					CopyMemory(bSpecCardData[1], SameColorResult.cbResultCard[0], sizeof(BYTE)*SameColorResult.cbCardCount[0]);
					RemoveCard(SameColorResult.cbResultCard[0], SameColorResult.cbCardCount[0], cbTempData1, cbTempCount);
					cbTempCount -= SameColorResult.cbCardCount[0];

					if (IsSameColorCard(cbTempData1, cbTempCount))
					{
						//ǰ���˿�
						CopyMemory(bSpecCardData[0], cbTempData1, sizeof(BYTE)*3);
						return 1;
					}
				}
			}
		}
	}

	return 0;
}

//�����˿�
void CGameLogic::SortCardList(BYTE bCardData[], BYTE bCardCount, enSortCardType SortCardType)
{
	ASSERT(bCardCount>=1 && bCardCount<=13) ;
	if(bCardCount<1 || bCardCount>13) return ;

	//ת����ֵ
	BYTE bLogicVolue[13];
	for (BYTE i=0;i<bCardCount;i++)	bLogicVolue[i]=GetCardLogicValue(bCardData[i]);	

	if(enDescend==SortCardType)
	{
		//�������
		bool bSorted=true;
		BYTE bTempData,bLast=bCardCount-1;
		BYTE m_bCardCount=1;
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
	}
	else if(enAscend==SortCardType)
	{
		//�������
		bool bSorted=true;
		BYTE bTempData,bLast=bCardCount-1;
		BYTE m_bCardCount=1;
		do
		{
			bSorted=true;
			for (BYTE i=0;i<bLast;i++)
			{
				if ((bLogicVolue[i]>bLogicVolue[i+1])||
					((bLogicVolue[i]==bLogicVolue[i+1])&&(bCardData[i]>bCardData[i+1])))
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
	}
	else if(enColor==SortCardType)
	{
		//�������
		bool bSorted=true;
		BYTE bTempData,bLast=bCardCount-1;
		BYTE m_bCardCount=1;
		BYTE bColor[13];
		for (BYTE i=0;i<bCardCount;i++)	bColor[i]=GetCardColor(bCardData[i]);	
		do
		{
			bSorted=true;
			for (BYTE i=0;i<bLast;i++)
			{
				if ((bColor[i]<bColor[i+1])||
					((bColor[i]==bColor[i+1])&&(GetCardLogicValue(bCardData[i])<GetCardLogicValue(bCardData[i+1]))))
				{
					//����λ��
					bTempData=bCardData[i];
					bCardData[i]=bCardData[i+1];
					bCardData[i+1]=bTempData;
					bTempData=bColor[i];
					bColor[i]=bColor[i+1];
					bColor[i+1]=bTempData;
					bSorted=false;
				}	
			}
			bLast--;
		} while(bSorted==false);
	}
	//else 
	//	MyMsgBox(_T("�����������ͣ�")) ;

	return;
}

//�����˿�
void CGameLogic::RandCardList(BYTE bCardBuffer[], BYTE bBufferCount)
{
	//����׼��
	BYTE bTotalCount = sizeof(m_bCardListData);
	BYTE bCardData[sizeof(m_bCardListData)];
	CopyMemory(bCardData,m_bCardListData,sizeof(m_bCardListData));

	srand((unsigned)time(NULL));

	//�����˿�
	BYTE bRandCount=0,bPosition=0;
	do
	{
		bPosition=rand()%(bTotalCount-bRandCount);
		bCardBuffer[bRandCount++]=bCardData[bPosition];
		bCardData[bPosition]=bCardData[bTotalCount-bRandCount];
	} while (bRandCount<bBufferCount);

	return;
}

//ɾ���˿�
bool CGameLogic::RemoveCard(const BYTE bRemoveCard[], BYTE bRemoveCount, BYTE bCardData[], BYTE bCardCount)
{
	//��������
	ASSERT(bRemoveCount<=bCardCount);

	//�������
	BYTE bDeleteCount=0,bTempCardData[13];
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

/****************************************************
*��������AutoPutCard 
*���ܣ�  ��һ���ƽ����Զ����ƣ����������ƣ�����Ŀ�ݶ�һ��Ҫ13�ţ���
�����߼��ѷֿ����˺�����������չΪ��N���ƽ���N���Զ����� JJ
*������	 bCardData			������(13)			IN
bPutCardData		�ںõ���(ǰ�к�13)	OUT
*����ֵ	 ��
****************************************************/

void CGameLogic::AutoPutCard(BYTE bCardData[], BYTE bPutCardData[], bool bIsAndroidUser, bool bTest)
{
	BYTE bTempCard[6][3][5]; //����Ȩֵ������������
	int  iTempWeights[6][3];  //����Ȩֵ�����������͵�Ȩֵ
	BYTE bCardList[13];      //��������
	BYTE bCard[3][5];        //��ʱ�����
	BYTE bSubscript[13];     //�±���������ÿһעʹ�õ���Щ��
	bool blBiaoJi[13];       //�����Щ��û��ʹ��
	int iFWeights,iSWeights,iTWeights;
	BYTE bWCard;

	CopyMemory(bCardList, bCardData, sizeof(BYTE)*13);
	ZeroMemory(bTempCard, sizeof(bTempCard));
	ZeroMemory(iTempWeights,sizeof(iTempWeights));
	ZeroMemory(bCard,sizeof(bCard));
	ZeroMemory(blBiaoJi,sizeof(blBiaoJi));

	//����ȫ����Ѱ����������
	SortCardList(bCardList, 13, enAscend);

	//��һעʹ�õ�������
	for(bSubscript[0] = 0;                 bSubscript[0] < 13; bSubscript[0] ++)
	for(bSubscript[1] = bSubscript[0] + 1; bSubscript[1] < 13; bSubscript[1] ++)
	for(bSubscript[2] = bSubscript[1] + 1; bSubscript[2] < 13; bSubscript[2] ++)
	{
		//�Ӵ�С���Ʊ��浽bCard[2]�����У����Ѿ�ʹ�õ��ƽ��б��
		for(int i = 0; i < 3; i ++)
		{
			blBiaoJi[bSubscript[i]] = true;
			bCard[2][2 - i] = bCardList[bSubscript[i]];
		}
		//�ڶ�עʹ�õ�������
		for(bSubscript[3] = 0;                 bSubscript[3] < 13; bSubscript[3] ++)
		if(!blBiaoJi[bSubscript[3]]) for(bSubscript[4] = bSubscript[3] + 1; bSubscript[4] < 13; bSubscript[4] ++)
		if(!blBiaoJi[bSubscript[4]]) for(bSubscript[5] = bSubscript[4] + 1; bSubscript[5] < 13; bSubscript[5] ++)
		if(!blBiaoJi[bSubscript[5]]) for(bSubscript[6] = bSubscript[5] + 1; bSubscript[6] < 13; bSubscript[6] ++)
		if(!blBiaoJi[bSubscript[6]]) for(bSubscript[7] = bSubscript[6] + 1; bSubscript[7] < 13; bSubscript[7] ++)
		if(!blBiaoJi[bSubscript[7]]){
		do{

			//�Ӵ�С���Ʊ��浽bCard[1]�����У����Ѿ�ʹ�õ��ƽ��б��
			for(int i = 0 ; i < 5 ; i ++)
			{
				blBiaoJi[bSubscript[3 + i]] = true;
				bCard[1][4 - i] = bCardList[bSubscript[3 + i]];
			}

			iFWeights = CardTypeToWeights(GetCardType(bCard[2],3,bWCard), 1);
			iSWeights = CardTypeToWeights(GetCardType(bCard[1],5,bWCard), 2);

			//�Ƚϵ�һע�͵ڶ�ע����Ȩֵ�Ĵ�С
			if(iFWeights > iSWeights)
			{
				break;
			}

			//��ʣ��Ԫ�ط���bCard[0]���鵱��
			int iCount = 0;
			for(int i = 0; i < 13; i ++)
			if(!blBiaoJi[i]){
				bCard[0][iCount ++ ] = bCardList[i];
			}

			iTWeights = CardTypeToWeights(GetCardType(bCard[0],5,bWCard), 3);
			if(iSWeights > iTWeights)
			{
				break;
			}

			bool bHave = false;
			for(int i = 0; i < 6; i ++)
			{
				//�Ѿ�������ע���Ͷ���С��������ϣ��ͷ���
				if(iFWeights <= iTempWeights[i][2] && iSWeights <= iTempWeights[i][1] && iTWeights <= iTempWeights[i][0])
				{
					bHave = true;
					break;
				}
			}

			if(false == bHave)
			{

				//�����ھͱ���
				for(int i = 0; i < 6; i ++)
				if(iFWeights >= iTempWeights[i][2] && iSWeights >= iTempWeights[i][1] && iTWeights >= iTempWeights[i][0])
				{
					iTempWeights[i][2] = iFWeights;
					iTempWeights[i][1] = iSWeights;
					iTempWeights[i][0] = iTWeights;

					for(int j = 0; j < 3; j ++)
					{
						CopyMemory(bTempCard[i][j],bCard[j],sizeof(BYTE)*5);
					}
					for(int j = i + 1; j < 6; j ++)
					{
						if(iFWeights >= iTempWeights[j][2] && iSWeights >= iTempWeights[j][1] && iTWeights >= iTempWeights[j][0])
						{
							iTempWeights[j][2] = 0;
							iTempWeights[j][1] = 0;
							iTempWeights[j][0] = 0;
						}
					}
					break;
				}
			}
		  }while(0);

		  for(int i = 0 ; i < 5 ; i ++)
		  {
			blBiaoJi[bSubscript[3 + i]] = false;
		  }	
		}

		//������
		for(i = 0; i < 3; i ++)
		{
			blBiaoJi[bSubscript[i]] = false;
		}	
	}
	//��Ȩֵû������һ����������ŵ�ȥ��
	for(int i = 0; i < 6; i ++)
	{
		for(int j = i + 1; j < 6; j ++)
		{
			if(iTempWeights[j][0] <= iTempWeights[i][0] && iTempWeights[j][1] <= iTempWeights[i][1] && iTempWeights[j][2] <= iTempWeights[i][2])
			{
				iTempWeights[j][0] = 0;
				iTempWeights[j][1] = 0;
				iTempWeights[j][2] = 0;
			}
		}
	}

	//���м���ֵĿ�϶ɾ��
	int iCount = 0;
	for(int i = 0; i < 6; i ++)
	{
		if(iTempWeights[i][0] != 0)
		{
			for(int j = 0; j < 3; j ++)
			{
				CopyMemory(bTempCard[iCount][j],bTempCard[i][j],sizeof(BYTE)*5);
				iTempWeights[iCount][j] = iTempWeights[i][j];
			}
			iCount ++;
		}

	}

	//��Ȩֵ����
	for(int i = 0; i < 6; i ++)
	{
		for(int j = i + 1; j < 6; j ++)
		{
			if((iTempWeights[j][0] + iTempWeights[j][1] + iTempWeights[j][2]) > (iTempWeights[i][0] + iTempWeights[i][1] + iTempWeights[i][2])||
				((iTempWeights[j][0] + iTempWeights[j][1] + iTempWeights[j][2]) == (iTempWeights[i][0] + iTempWeights[i][1] + iTempWeights[i][2])&&
				(iTempWeights[j][0] > iTempWeights[i][0] || (iTempWeights[j][0] == iTempWeights[i][0] && iTempWeights[j][1] > iTempWeights[i][1]))))
			{
				for(int k = 0; k < 3; k ++)
				{
					int iTemp;

					iTemp = iTempWeights[i][k];
					iTempWeights[i][k] = iTempWeights[j][k];
					iTempWeights[j][k] = iTemp;

					CopyMemory(bCard[k],bTempCard[i][k],sizeof(BYTE)*5);
					CopyMemory(bTempCard[i][k],bTempCard[j][k],sizeof(BYTE)*5);
					CopyMemory(bTempCard[j][k],bCard[k],sizeof(BYTE)*5);
				}	
			}
		}
	}

	//���ѡ��һ��Ȩֵ����ƴ��
	int iRandom = rand() % 100;
	int iCnt = 0; 

	/*if(iRandom < 5 && iTempWeights[2][0] > 0 )
	{
		iCnt = 2;
	}
	else if(iRandom < 10 && iTempWeights[1][0] > 0)
	{
		iCnt = 1;
	}*/
	
	////���ѡ��һ����
	//if(iCount > 1)
	//{
	//	if(bIsAndroidUser)
	//	{
	//		if(bTest)
	//		{
	//			iCnt = iCount-1;
	//		}
	//		else
	//		{
	//			iCnt = 0;
	//		}
	//		/*rand()%iCount*/;
	//	}
	//	else
	//	{
	//		if(bTest)
	//		{
	//			iCnt = 0;
	//		}
	//		else
	//		{
	//			iCnt = iCount-1;
	//		}
	//		
	//	}
	//}
	
	//�������
	CopyMemory(bPutCardData, bTempCard[iCnt][2], sizeof(BYTE)*3);
	CopyMemory(bPutCardData+3, bTempCard[iCnt][1], sizeof(BYTE)*5);
	CopyMemory(bPutCardData+8, bTempCard[iCnt][0], sizeof(BYTE)*5);

	//�����Ż�
	//if((bIsAndroidUser && !bTest) || (!bIsAndroidUser && bTest))
	{
		OptimizationCombo(bPutCardData + 3,bPutCardData + 8,5,5);
		OptimizationCombo(bPutCardData,bPutCardData + 8,3,5);
		OptimizationCombo(bPutCardData,bPutCardData + 3,3,5);
	}
	return;
}
//void CGameLogic::AutoPutCard(BYTE bCardData[], BYTE bPutCardData[])
//{
//	//�������
//	BYTE bCardList[13] = {0};
//	CopyMemory(bCardList, bCardData, sizeof(BYTE)*13);
//	ZeroMemory(bPutCardData, sizeof(BYTE)*13);
//	BYTE bLeftCCount = 13;
//	BYTE bTempCard[3][5] = {0};
//	BYTE bTempCCount[3] = {0};
//	BYTE bSigCCount = 0;		//����ɢ�����ʱ�ļ���
//	BYTE bCardType[3] = {0};	
//
//
//	//���ƿ�ʼ
//	for (int i = 0; i < 3; i++)
//	{
//		bCardType[i] = GetMaxCardData(bCardList, bLeftCCount, bTempCard[i], bTempCCount[i], (i==2?3:5));
//		RemoveCard(bTempCard[i], bTempCCount[i], bCardList, bLeftCCount);
//		bLeftCCount -= bTempCCount[i];
//	}
//
//	ASSERT(bLeftCCount+bTempCCount[0]+bTempCCount[1]+bTempCCount[2] == 13);
//	ASSERT(bTempCCount[0]<=5&&bTempCCount[1]<=5&&bTempCCount[2]<=3);
//
//	if (bLeftCCount == 0)
//	{
//		//�������
//		CopyMemory(bPutCardData, bTempCard[2], sizeof(BYTE)*3);
//		CopyMemory(bPutCardData+3, bTempCard[1], sizeof(BYTE)*5);
//		CopyMemory(bPutCardData+8, bTempCard[0], sizeof(BYTE)*5);
//
//		return;
//	}
//
//	SortCardList(bCardList, bLeftCCount, enDescend);
//
//	//ɢ�����
//	while (bTempCCount[2] < 3)
//	{
//		bTempCard[2][bTempCCount[2]++] = bCardList[bSigCCount++];
//	}
//	while (bTempCCount[0] < 5)
//	{
//		bTempCard[0][bTempCCount[0]++] = bCardList[bSigCCount++];
//	}
//	while (bTempCCount[1] < 5)
//	{
//		bTempCard[1][bTempCCount[1]++] = bCardList[bSigCCount++];
//	}
//	ASSERT(bSigCCount == bLeftCCount);
//
//	//�������
//	CopyMemory(bPutCardData, bTempCard[2], sizeof(BYTE)*3);
//	CopyMemory(bPutCardData+3, bTempCard[1], sizeof(BYTE)*5);
//	CopyMemory(bPutCardData+8, bTempCard[0], sizeof(BYTE)*5);
//
//	return;
//}

//�߼���ֵ
BYTE CGameLogic::GetCardLogicValue(BYTE bCardData)
{
	//�˿�����
	BYTE bCardValue=GetCardValue(bCardData); 

	//ת����ֵ
	return (bCardValue==1)?(bCardValue+13):bCardValue; 
}

//�Ƚ��˿�
int CGameLogic::CompareCard(BYTE bInFirstList[], BYTE bInNextList[], BYTE bFirstCount, BYTE bNextCount , bool bComperWithOther)
{
	//�������
	BYTE bFirstList[13], bNextList[13] ;
	tagAnalyseData FirstAnalyseData , NextAnalyseData;
	ZeroMemory(&FirstAnalyseData, sizeof(FirstAnalyseData)) ;
	ZeroMemory(&NextAnalyseData, sizeof(NextAnalyseData)) ;

	//���ת��
	tagTransData ttdFst;
	tagTransData ttdNxt;
	AnalyseMaxTransform(bInFirstList, bFirstCount, bFirstList, ttdFst);
	AnalyseMaxTransform(bInNextList, bNextCount, bNextList, ttdNxt);

	//��������
	SortCardList(bFirstList , bFirstCount , enDescend) ;
	SortCardList(bNextList , bNextCount , enDescend) ;

	//��������
	AnalyseCard(bFirstList , bFirstCount , FirstAnalyseData) ;	
	AnalyseCard(bNextList  , bNextCount  , NextAnalyseData) ;	

	//������֤
	ASSERT(bFirstCount==(FirstAnalyseData.bOneCount+FirstAnalyseData.bTwoCount*2+FirstAnalyseData.bThreeCount*3+FirstAnalyseData.bFourCount*4)) ;
	ASSERT(bNextCount=(NextAnalyseData.bOneCount+NextAnalyseData.bTwoCount*2+NextAnalyseData.bThreeCount*3+NextAnalyseData.bFourCount*4)) ;
	if(bFirstCount!=(FirstAnalyseData.bOneCount+FirstAnalyseData.bTwoCount*2+FirstAnalyseData.bThreeCount*3+FirstAnalyseData.bFourCount*4))
	{
		return enCRError ;
	}
	if(bNextCount != (NextAnalyseData.bOneCount + NextAnalyseData.bTwoCount*2 + NextAnalyseData.bThreeCount*3+NextAnalyseData.bFourCount*4))
	{
		return enCRError ;
	}

	ASSERT((bFirstCount==bNextCount) || (bFirstCount!=bNextCount && (3==bFirstCount && 5==bNextCount || 5==bFirstCount && 3==bNextCount))) ;
	if(!((bFirstCount==bNextCount) || (bFirstCount!=bNextCount && (3==bFirstCount && 5==bNextCount || 5==bFirstCount && 3==bNextCount)))) return enCRError ;

	ASSERT(3==bNextCount || 5==bNextCount) ;
	ASSERT(3==bFirstCount || 5==bFirstCount) ;

	//��ȡ����
	BYTE cbMaxCard = 0;
	BYTE bNextType=GetCardType(bNextList,bNextCount,cbMaxCard);
	BYTE bFirstType=GetCardType(bFirstList,bFirstCount,cbMaxCard);

	ASSERT(CT_INVALID!=bNextType && CT_INVALID!=bFirstType) ;
	if(CT_INVALID==bFirstType || CT_INVALID==bNextType) return enCRError ;

	//ͬ�αȽ�
	if(true==bComperWithOther)
	{
		//��ʼ�Ա�
		if(bNextType==bFirstType) 
		{
			switch(bFirstType) 
			{
			case CT_SINGLE:				//��������
				{
					//������֤
					ASSERT(bNextList[0]!=bFirstList[0]) ;
					if(bNextList[0]==bFirstList[0]) return enCRError ;

					for(BYTE i=0 ; i<bFirstCount ; ++i)
					{
						if(GetCardLogicValue(bNextList[i]) > GetCardLogicValue(bFirstList[i]))
							return enCRGreater;
						else if (GetCardLogicValue(bNextList[i]) < GetCardLogicValue(bFirstList[i]))
							return enCRLess;
					}
					return enCREqual;
				}
			case CT_ONE_DOUBLE:			//�Դ�һ��
				{
					if(GetCardLogicValue(bNextList[NextAnalyseData.bTwoFirst[0]])==GetCardLogicValue(bFirstList[FirstAnalyseData.bTwoFirst[0]]))
					{
						if (GetCardLogicValue(bNextList[NextAnalyseData.bOneFirst[0]]) > GetCardLogicValue(bFirstList[FirstAnalyseData.bOneFirst[0]]))
							return enCRGreater;
						else if (GetCardLogicValue(bNextList[NextAnalyseData.bOneFirst[0]]) < GetCardLogicValue(bFirstList[FirstAnalyseData.bOneFirst[0]]))
							return enCRLess;
						else 
							return enCREqual;
					}
					else 
					{
						if (GetCardLogicValue(bNextList[NextAnalyseData.bTwoFirst[0]]) > GetCardLogicValue(bFirstList[FirstAnalyseData.bTwoFirst[0]]))
							return enCRGreater;
						else
							return enCRLess;
					}
				}
			case CT_TWO_DOUBLE:	//��������
				{
					//������֤
					ASSERT(bNextList[NextAnalyseData.bTwoFirst[0]]!=bFirstList[FirstAnalyseData.bTwoFirst[0]]);
					if(bNextList[NextAnalyseData.bTwoFirst[0]]==bFirstList[FirstAnalyseData.bTwoFirst[0]]) return enCRError ;

					if(GetCardLogicValue(bNextList[NextAnalyseData.bTwoFirst[0]])==GetCardLogicValue(bFirstList[FirstAnalyseData.bTwoFirst[0]]))
					{
						if(GetCardLogicValue(bNextList[NextAnalyseData.bTwoFirst[1]])==GetCardLogicValue(bFirstList[FirstAnalyseData.bTwoFirst[1]]))
						{
							if (GetCardLogicValue(bNextList[NextAnalyseData.bOneFirst[0]]) > GetCardLogicValue(bFirstList[FirstAnalyseData.bOneFirst[0]]))
								return enCRGreater;
							else if (GetCardLogicValue(bNextList[NextAnalyseData.bOneFirst[0]]) < GetCardLogicValue(bFirstList[FirstAnalyseData.bOneFirst[0]]))
								return enCRLess;
							else
								return enCREqual;
						}
						else
						{
							if (GetCardLogicValue(bNextList[NextAnalyseData.bTwoFirst[1]]) > GetCardLogicValue(bFirstList[FirstAnalyseData.bTwoFirst[1]]))
								return enCRGreater;
							else
								return enCRLess;
						}
					}							
					else
					{
						if (GetCardLogicValue(bNextList[NextAnalyseData.bTwoFirst[0]]) > GetCardLogicValue(bFirstList[FirstAnalyseData.bTwoFirst[0]]))
							return enCRGreater;
						else 
							return enCRLess;
					}
				}
			case CT_THREE:						//��������
				{
					if (GetCardLogicValue(bNextList[NextAnalyseData.bThreeFirst[0]]) > GetCardLogicValue(bFirstList[FirstAnalyseData.bThreeFirst[0]]))
						return enCRGreater;
					else if (GetCardLogicValue(bNextList[NextAnalyseData.bThreeFirst[0]]) < GetCardLogicValue(bFirstList[FirstAnalyseData.bThreeFirst[0]]))
						return enCRLess;
					else
						return enCREqual;
				}
			case CT_FIVE_THREE_DEOUBLE:			//����һ��
				{
					if (GetCardLogicValue(bNextList[NextAnalyseData.bThreeFirst[0]]) == GetCardLogicValue(bFirstList[FirstAnalyseData.bThreeFirst[0]]))
					{
						if (GetCardLogicValue(bNextList[NextAnalyseData.bTwoFirst[0]]) > GetCardLogicValue(bFirstList[FirstAnalyseData.bTwoFirst[0]]))
							return enCRGreater;
						else if (GetCardLogicValue(bNextList[NextAnalyseData.bTwoFirst[0]]) < GetCardLogicValue(bFirstList[FirstAnalyseData.bTwoFirst[0]]))
							return enCRLess;
						else
							return enCREqual;
					}
					else
					{
						if (GetCardLogicValue(bNextList[NextAnalyseData.bThreeFirst[0]]) > GetCardLogicValue(bFirstList[FirstAnalyseData.bThreeFirst[0]]))
							return enCRGreater;
						else
							return enCRLess;
					}
				}
			case CT_FIVE_MIXED_FLUSH_FIRST_A:	//A��ǰ˳��
			case CT_FIVE_MIXED_FLUSH_BACK_A:	//A�ں�˳��
				{
					//�Ƚ�����һ����
					return enCREqual;
				}
			case CT_FIVE_MIXED_FLUSH_NO_A:		//ûA��˳
			case CT_FIVE_FLUSH:					//ͬ������
				{
					//�Ƚ���ֵ
					for(BYTE i=0; i<5; ++i)
					{
						if (GetCardLogicValue(bNextList[i]) > GetCardLogicValue(bFirstList[i]))
							return enCRGreater;
						else if (GetCardLogicValue(bNextList[i]) < GetCardLogicValue(bFirstList[i]))
							return enCRLess;
					}

					//�Ƚϻ�ɫ
					for(BYTE i = 0 ; i < 5; i++)
					{
						if (GetCardColor(bNextList[i]) > GetCardColor(bFirstList[i]))
							return enCRGreater;
						else if (GetCardColor(bNextList[i]) < GetCardColor(bFirstList[i]))
							return enCRLess;
					}
				}
			case CT_FIVE_FOUR_ONE:				//�Ĵ�һ��
				{
					if (GetCardLogicValue(bNextList[NextAnalyseData.bFourFirst[0]]) > GetCardLogicValue(bFirstList[FirstAnalyseData.bFourFirst[0]]))
						return enCRGreater;
					else
						return enCRLess;
				}
			case CT_FIVE_STRAIGHT_FLUSH_FIRST_A://A��ǰͬ��˳
				{
					//������֤
					ASSERT(bNextList[0]!=bFirstList[0]) ;
					if(bNextList[0]==bFirstList[0]) return enCRError ;

					//�Ƚ���ֵ
					for(BYTE i=0; i<5; ++i)
					{
						if (GetCardLogicValue(bNextList[i]) > GetCardLogicValue(bFirstList[i]))
							return enCRGreater;
						else if (GetCardLogicValue(bNextList[i]) < GetCardLogicValue(bFirstList[i]))
							return enCRLess;
					}

					//�Ƚϻ�ɫ
					for(BYTE i = 0 ; i < 5; i++)
					{
						if (GetCardColor(bNextList[i]) > GetCardColor(bFirstList[i]))
							return enCRGreater;
						else if (GetCardColor(bNextList[i]) < GetCardColor(bFirstList[i]))
							return enCRLess;
					}

				}
			case CT_FIVE_STRAIGHT_FLUSH:		//ͬ��˳��
				{
					//�Ƚ���ֵ
					for(BYTE i=0; i<5; ++i)
					{
						if (GetCardLogicValue(bNextList[i]) > GetCardLogicValue(bFirstList[i]))
							return enCRGreater;
						else if (GetCardLogicValue(bNextList[i]) < GetCardLogicValue(bFirstList[i]))
							return enCRLess;
					}

					//�Ƚϻ�ɫ
					for(BYTE i = 0 ; i < 5; i++)
					{
						if (GetCardColor(bNextList[i]) > GetCardColor(bFirstList[i]))
							return enCRGreater;
						else if (GetCardColor(bNextList[i]) < GetCardColor(bFirstList[i]))
							return enCRLess;
					}
				}
			default:
				return enCRError ;
			}
		}
		else
		{
			if (bNextType > bFirstType)
				return enCRGreater;
			else 
				return enCRLess;
		}
	}
	else
	{
		ASSERT(bFirstType==CT_SINGLE||bFirstType==CT_ONE_DOUBLE||bFirstType==CT_THREE);
		if (bFirstType!=CT_SINGLE && bFirstType!=CT_ONE_DOUBLE && bFirstType!=CT_THREE)
			return enCRError;
		//��ʼ�Ա�
		if(bNextType==bFirstType) 
		{
			switch(bFirstType)
			{
			case CT_SINGLE:				//��������
				{
					//������֤
					ASSERT(bNextList[0]!=bFirstList[0]) ;
					if(bNextList[0]==bFirstList[0]) return enCRError ;

					for(BYTE i=0 ; i<bFirstCount ; ++i)
					{
						if(GetCardLogicValue(bNextList[i]) > GetCardLogicValue(bFirstList[i]))
							return enCRGreater;
						else if (GetCardLogicValue(bNextList[i]) < GetCardLogicValue(bFirstList[i]))
							return enCRLess;
					}

					if (bNextCount > bFirstCount)
						return enCRGreater;
					else 
						return enCRLess;
				}
			case CT_ONE_DOUBLE:			//�Դ�һ��
				{
					if(GetCardLogicValue(bNextList[NextAnalyseData.bTwoFirst[0]])==GetCardLogicValue(bFirstList[FirstAnalyseData.bTwoFirst[0]]))
					{
						if (GetCardLogicValue(bNextList[NextAnalyseData.bOneFirst[0]]) > GetCardLogicValue(bFirstList[FirstAnalyseData.bOneFirst[0]]))
							return enCRGreater;
						else if (GetCardLogicValue(bNextList[NextAnalyseData.bOneFirst[0]]) < GetCardLogicValue(bFirstList[FirstAnalyseData.bOneFirst[0]]))
							return enCRLess;
						else
						{
							if (bNextCount > bFirstCount)
								return enCRGreater;
							else 
								return enCRLess;
						}
					}
					else 
					{
						if (GetCardLogicValue(bNextList[NextAnalyseData.bTwoFirst[0]]) > GetCardLogicValue(bFirstList[FirstAnalyseData.bTwoFirst[0]]))
							return enCRGreater;
						else
							return enCRLess;
					}
				}
			case CT_THREE:				//��������
				{
					if (GetCardLogicValue(bNextList[NextAnalyseData.bThreeFirst[0]]) > GetCardLogicValue(bFirstList[FirstAnalyseData.bThreeFirst[0]]))
						return enCRGreater;
					else if (GetCardLogicValue(bNextList[NextAnalyseData.bThreeFirst[0]]) < GetCardLogicValue(bFirstList[FirstAnalyseData.bThreeFirst[0]]))
						return enCRLess;
					else 
					{
						if (bNextCount > bFirstCount)
							return enCRGreater;
						else 
							return enCRLess;
					}
				}
			default:
				return enCRError ;
			}
		}
		else
		{
			if (bNextType > bFirstType)
				return enCRGreater;
			else 
				return enCRLess;
		}
	}

	return enCRError;
}

//�Ƿ�˳��
bool CGameLogic::IsLinkCard(const BYTE cbCardData[], BYTE cbCardCount)
{
	ASSERT(cbCardCount > 0);
	if (cbCardCount <= 0) return false;

	bool bRet = true;
	BYTE cbCardBuffer[13] = {0};
	CopyMemory(cbCardBuffer, cbCardData, sizeof(BYTE)*cbCardCount);
	
	//��������
	SortCardList(cbCardBuffer, cbCardCount, enDescend);

	BYTE cbFirstCard=GetCardLogicValue(cbCardBuffer[0]);
	for (WORD i=1; i<cbCardCount; i++)
	{
		BYTE cbNextCard=GetCardLogicValue(cbCardBuffer[i]);
		if (cbFirstCard != cbNextCard+i) bRet=false;
	}

	return bRet;
}

//�Ƿ�ͬ��
bool CGameLogic::IsSameColorCard(const BYTE cbCardData[], BYTE cbCardCount)
{
	ASSERT(cbCardCount > 0);
	if (cbCardCount <= 0) return false;

	bool bRet=true;
	
	BYTE cbFirstCard=GetCardColor(cbCardData[0]);
	for (WORD i=1; i<cbCardCount; i++)
	{
		BYTE cbNextCard=GetCardColor(cbCardData[i]);
		if (cbNextCard != cbFirstCard) bRet=false;
	}

	return bRet;
}

//�Ƿ�ͬ��˳
bool CGameLogic::IsStraightDragon(const BYTE cbCardData[], BYTE bCardCount)
{
	//У������
	ASSERT(bCardCount>0 && bCardCount<=13);
	if (bCardCount<=0 || bCardCount>13)	return false;

	bool b1 = IsLinkCard(cbCardData, bCardCount);
	bool b2 = IsSameColorCard(cbCardData, bCardCount);
	if (b1==false || b2==false)
		return false;

	return true;
}

//��ȡ����
BYTE CGameLogic::GetDoubleCount(BYTE cbFrontCard[], BYTE cbMidCard[], BYTE cbBackCard[])
{
	tagAnalyseData AanlyseFront, AnalyseMid, AnalyeBack;
	ZeroMemory(&AanlyseFront, sizeof(AanlyseFront));
	ZeroMemory(&AnalyseMid, sizeof(AnalyseMid));
	ZeroMemory(&AnalyeBack, sizeof(AnalyeBack));

	AnalyseCard(cbFrontCard, 3, AanlyseFront);
	AnalyseCard(cbMidCard, 5, AnalyseMid);
	AnalyseCard(cbBackCard, 5, AnalyeBack);

	if ((AanlyseFront.bTwoCount == 1) && (AnalyseMid.bTwoCount == 2) && (AnalyeBack.bTwoCount == 2))
	{
		if ( (GetCardLogicValue(cbFrontCard[AanlyseFront.bOneFirst[0]]) == GetCardLogicValue(cbMidCard[AnalyseMid.bOneFirst[0]])) ||
		     (GetCardLogicValue(cbMidCard[AnalyseMid.bOneFirst[0]]) == GetCardLogicValue(cbBackCard[AnalyeBack.bOneFirst[0]])) ||
			 (GetCardLogicValue(cbFrontCard[AanlyseFront.bOneFirst[0]]) == GetCardLogicValue(cbBackCard[AnalyeBack.bOneFirst[0]])))
			 return 6;
	}
	
	return AanlyseFront.bTwoCount + AnalyseMid.bTwoCount + AnalyeBack.bTwoCount;
}

//�����˿�
void CGameLogic::AnalyseCard(const BYTE bCardDataList[] , const BYTE bCardCount , tagAnalyseData& AnalyseData) 
{
	ASSERT(3==bCardCount || 5==bCardCount) ;

	//�����˿�
	BYTE bCardData[13] ;
	CopyMemory(bCardData , bCardDataList , bCardCount) ;
	SortCardList(bCardData , bCardCount , enDescend) ;

	//��������
	BYTE bSameCount = 1 ,
		bCardValueTemp=0,
		bSameColorCount = 1 ,
		bFirstCardIndex = 0 ;	//��¼�±�

	BYTE bLogicValue = GetCardLogicValue(bCardData[0]);
	BYTE bCardColor = GetCardColor(bCardData[0]) ;

	//���ý��
	ZeroMemory(&AnalyseData, sizeof(AnalyseData));

	//�˿˷���
	for (BYTE i=1;i<bCardCount;i++)
	{
		//��ȡ�˿�
		bCardValueTemp=GetCardLogicValue(bCardData[i]);
		if (bCardValueTemp==bLogicValue) bSameCount++;

		//������
		if ((bCardValueTemp!=bLogicValue)||(i==(bCardCount-1))||bCardData[i]==0)
		{
			switch (bSameCount)
			{
			case 1:		//һ��
				break; 
			case 2:		//����
				{
					AnalyseData.bTwoFirst[AnalyseData.bTwoCount]	 = bFirstCardIndex ;
					AnalyseData.bTwoCount++ ;
					break;
				}
			case 3:		//����
				{
					AnalyseData.bThreeFirst[AnalyseData.bThreeCount] = bFirstCardIndex ;
					AnalyseData.bThreeCount++ ;
					break;
				}
			case 4:		//����
				{
					AnalyseData.bFourFirst[AnalyseData.bFourCount]   = bFirstCardIndex ;
					AnalyseData.bFourCount++ ;
					break;
				}
			default:
				//MyMsgBox(_T("AnalyseCard�������˿ˣ�: %d") , bSameCount) ;
				break;
			}
		}

		//�����Զ�ת��ͬ��
		if (bCardData[i] == 0)	
		{
			bSameColorCount += bCardCount-i;
			break;
		}

		//���ñ���
		if (bCardValueTemp!=bLogicValue)
		{
			if(bSameCount==1)
			{
				if(i!=bCardCount-1)
				{
					AnalyseData.bOneFirst[AnalyseData.bOneCount]	= bFirstCardIndex ;
					AnalyseData.bOneCount++ ;
				}
				else
				{
					AnalyseData.bOneFirst[AnalyseData.bOneCount]	= bFirstCardIndex ;
					AnalyseData.bOneCount++ ;
					AnalyseData.bOneFirst[AnalyseData.bOneCount]	= i ;
					AnalyseData.bOneCount++ ;				
				}
			}
			else
			{
				if(i==bCardCount-1)
				{
					AnalyseData.bOneFirst[AnalyseData.bOneCount]	= i ;
					AnalyseData.bOneCount++ ;
				}
			}
			bSameCount=1;
			bLogicValue=bCardValueTemp;
			bFirstCardIndex = i ;

		}
		if(GetCardColor(bCardData[i])!=bCardColor) bSameColorCount = 1 ;
		else									   ++bSameColorCount ;
	}

	//�Ƿ�ͬ��
	AnalyseData.bSameColor = (5==bSameColorCount) ? true : false ;

	return;
}

//�����˿�
void CGameLogic::AnalysebCardData(const BYTE cbCardData[], BYTE cbCardCount, tagAnalyseResult & AnalyseResult)
{
	//���ý��
	ZeroMemory(&AnalyseResult,sizeof(AnalyseResult));

	//�˿˷���
	for (BYTE i=0;i<cbCardCount;i++)
	{
		//��������
		BYTE cbSameCount=1;
		BYTE cbLogicValue=GetCardLogicValue(cbCardData[i]);

		//����ͬ��
		for (BYTE j=i+1;j<cbCardCount;j++)
		{
			//��ȡ�˿�
			if (GetCardLogicValue(cbCardData[j])!=cbLogicValue) break;

			//���ñ���
			cbSameCount++;
		}

		//���ý��
		BYTE cbIndex = AnalyseResult.cbBlockCount[cbSameCount-1]++;
		for (BYTE j = 0; j < cbSameCount; j++) 
			AnalyseResult.cbCardData[cbSameCount-1][cbIndex*cbSameCount+j]=cbCardData[i+j];

		//��������
		i += (cbSameCount-1);
	}

	return;
}

//�����ֲ�
VOID CGameLogic::AnalysebDistributing(const BYTE cbCardData[], BYTE cbCardCount, tagDistributing & Distributing)
{
	//���ñ���
	ZeroMemory(&Distributing,sizeof(Distributing));

	//���ñ���
	for (BYTE i=0;i<cbCardCount;i++)
	{
		if (cbCardData[i]==0) continue;

		//��ȡ����
		BYTE cbCardColor=GetCardColor(cbCardData[i]);
		BYTE cbCardValue=GetCardValue(cbCardData[i]);

		//�ֲ���Ϣ
		Distributing.cbCardCount++;
		Distributing.cbDistributing[cbCardValue-1][cbIndexCount]++;
		Distributing.cbDistributing[cbCardValue-1][cbCardColor]++;
	}

	return;
}


/****************************************************
*��������CompareOneCardEx 
*���ܣ�  ���űȽϣ��ȱ���ֵ����ֵ��ͬ��Ƚϻ�ɫ (��<÷<��<��)
*������	 
bFirstCard			��һ			IN
bNextCard			�ƶ�			IN
*����ֵ��
true				�����ǰ
false				ǰ���ں�
****************************************************/
bool CGameLogic::CompareOneCardEx(BYTE bFirstCard, BYTE bNextCard, tagTransData & ttdFst, tagTransData & ttdNxt)
{
	//��ֵ�Ƚ�
	if (GetCardLogicValue(bFirstCard) != GetCardLogicValue(bNextCard))
		return GetCardLogicValue(bFirstCard) < GetCardLogicValue(bNextCard);

	//ת���Ƚ�	(����ת�������Ʊ���ֵͨͬ�ƴ󣬴���ת�����Ĵ���С��ת������)
	bool bFromTransFst = (ttdFst.transList.Find(bFirstCard)!=NULL);
	bool bFromTransNxt = (ttdNxt.transList.Find(bNextCard)!=NULL);
	if (bFromTransFst != bFromTransNxt)
	{
		return !bFromTransFst;	
	}
	else
	{
		if (bFromTransFst)		//�Ƚϴ�С��
		{
			ASSERT((ttdFst.bHaveKing==0) == (ttdNxt.bHaveKing==0));
			return (ttdFst.bHaveKing<ttdNxt.bHaveKing);
		}
		else					//�Ƚϻ�ɫ
		{
			return ( GetCardColor(bFirstCard)<GetCardColor(bNextCard) );
		}
	}

	return false;
}

/****************************************************
*��������GetMaxCardData 
*���ܣ�  �ӵö������г�ȡ��������͵���	JJ
*������	 
cbCardData			ԭ��(3< <=13)		IN
cbCardCount			ԭ����Ŀ				IN
cbMaxCardData		ȡ���������(<=5)	OUT
bMaxCardCount		ȡ������Ŀ(1<= <=5)  OUT
*����ֵ��������� (���ڵ�Ԫ������У��,ʵ������,������Ϊ�ɿ�����)
****************************************************/
BYTE CGameLogic::GetMaxCardData(const BYTE cbCardData[], const BYTE cbCardCount, BYTE bMaxCardData[], BYTE & bMaxCardCount, BYTE bNeedCCount)
{
	//У������
	ASSERT(cbCardCount <= 13 || cbCardCount > 3);

	//�������
	BYTE bKCount = 0;
	CList<BYTE> evCardList[15];	//0λ������,1λ����,����λ���߼�ֵ���
	CList<BYTE> evColorList[4];	//��÷���
	BYTE bCardArray[13] = {0};
	CopyMemory(bCardArray, cbCardData, sizeof(BYTE)*cbCardCount);

	SortCardList(bCardArray, cbCardCount, enDescend);

	//�����˿�
	for (int i = 0; i < cbCardCount; i++)
	{
		//��������
		if (bCardArray[i] == 0x41 || bCardArray[i] == 0x42)
		{
			evCardList[0].AddTail(bCardArray[i]);
			continue;
		}

		//��������
		BYTE bLogicNum = GetCardLogicValue(bCardArray[i]);
		BYTE bColor = GetCardColor(bCardArray[i]);

		ASSERT(bLogicNum>1 && bLogicNum<15 && bColor>=0 && bColor<=3);
		ASSERT(evCardList[bLogicNum].Find(bCardArray[i]) == NULL);

		evCardList[bLogicNum].AddTail(bCardArray[i]);
		evColorList[bColor].AddTail(bCardArray[i]);
	}

	ASSERT(evCardList[0].GetCount() <= 2);

	//Ѱ��ͬ��˳
	if (bNeedCCount == 5)
	{
		for (int i = 0; i < 4; i++)
		{
			if (evColorList[i].GetCount()+evCardList[0].GetCount() >= 5)	//ͬ��+���������ڵ���5
			{
				BYTE bCount = 0;
				if (evCardList[0].GetCount() >= 0 && evColorList[i].GetCount() >= 5)		//������
				{
					for (int j = 0; j < evColorList[i].GetCount()-4; j++)
					{
						BYTE bFstCard = evColorList[i].GetAt(evColorList[i].FindIndex(j));
						BYTE bLstCard = evColorList[i].GetAt(evColorList[i].FindIndex(j+4));

						if (GetCardLogicValue(bFstCard) -  GetCardLogicValue(bLstCard) == 4)
						{
							for (int k = 0; k < 5; k++)
								bMaxCardData[k] = evColorList[i].GetAt(evColorList[i].FindIndex(j+k));
							bMaxCardCount = 5;
							return CT_FIVE_STRAIGHT_FLUSH;
						}
					}
					if (GetCardValue(evColorList[i].GetHead()) == 1 &&						//���A2345˳
						GetCardValue(evColorList[i].GetAt(evColorList[i].FindIndex(evColorList[i].GetCount()-4))) == 5)	
					{
						bMaxCardData[0] = evColorList[i].GetHead();
						for (int k = 1; k < 5; k++)
							bMaxCardData[k] = evColorList[i].GetAt(evColorList[i].FindIndex(evColorList[i].GetCount()-k));
						bMaxCardCount = 5;
						return CT_FIVE_STRAIGHT_FLUSH_FIRST_A;
					}
				}
				if (evCardList[0].GetCount() >= 1 && evColorList[i].GetCount() >= 4)		//������
				{
					for (int j = 0; j < evColorList[i].GetCount()-3; j++)
					{
						BYTE bFstCard = evColorList[i].GetAt(evColorList[i].FindIndex(j));
						BYTE bLstCard = evColorList[i].GetAt(evColorList[i].FindIndex(j+3));

						if ( (GetCardLogicValue(bFstCard) -  GetCardLogicValue(bLstCard) == 3) || 
							(GetCardLogicValue(bFstCard) -  GetCardLogicValue(bLstCard) == 4) )
						{
							bMaxCardData[0] = evCardList[0].GetHead();
							for (int k = 0; k < 4; k++)
								bMaxCardData[k+1] = evColorList[i].GetAt(evColorList[i].FindIndex(j+k));
							bMaxCardCount = 5;
							return CT_FIVE_STRAIGHT_FLUSH;
						}
					}
					if (GetCardValue(evColorList[i].GetHead()) == 1 &&						//���A2345˳
						GetCardValue(evColorList[i].GetAt(evColorList[i].FindIndex(evColorList[i].GetCount()-3))) <= 5)	
					{
						bMaxCardData[0] = evCardList[0].GetHead();
						bMaxCardData[1] = evColorList[i].GetHead();
						for (int k = 1; k < 4; k++)
							bMaxCardData[k+1] = evColorList[i].GetAt(evColorList[i].FindIndex(evColorList[i].GetCount()-k));
						bMaxCardCount = 5;
						return CT_FIVE_STRAIGHT_FLUSH_FIRST_A;
					}
				}
				if (evCardList[0].GetCount() == 2 && evColorList[i].GetCount() >= 3)		//��˫��
				{
					for (int j = 0; j < evColorList[i].GetCount()-2; j++)
					{
						BYTE bFstCard = evColorList[i].GetAt(evColorList[i].FindIndex(j));
						BYTE bLstCard = evColorList[i].GetAt(evColorList[i].FindIndex(j+2));

						if ( (GetCardLogicValue(bFstCard) -  GetCardLogicValue(bLstCard) == 2) ||
							(GetCardLogicValue(bFstCard) -  GetCardLogicValue(bLstCard) == 3) || 
							(GetCardLogicValue(bFstCard) -  GetCardLogicValue(bLstCard) == 4))
						{
							bMaxCardData[0] = evCardList[0].GetHead();
							bMaxCardData[1] = evCardList[0].GetTail();
							for (int k = 0; k < 3; k++)
								bMaxCardData[k+2] = evColorList[i].GetAt(evColorList[i].FindIndex(j+k));
							bMaxCardCount = 5;
							return CT_FIVE_STRAIGHT_FLUSH;
						}
					}
					if (GetCardValue(evColorList[i].GetHead()) == 1 &&						//���A2345˳
						GetCardValue(evColorList[i].GetAt(evColorList[i].FindIndex(evColorList[i].GetCount()-2))) <= 5)	
					{
						bMaxCardData[0] = evCardList[0].GetHead();
						bMaxCardData[1] = evCardList[0].GetTail();
						bMaxCardData[2] = evColorList[i].GetHead();
						for (int k = 1; k < 3; k++)
							bMaxCardData[k+2] = evColorList[i].GetAt(evColorList[i].FindIndex(evColorList[i].GetCount()-k));
						bMaxCardCount = 5;
						return CT_FIVE_STRAIGHT_FLUSH_FIRST_A;
					}
				}
			}
		}
	}

	//Ѱ����֧
	if (bNeedCCount == 5)
	{
		for (int i = 14; i > 1; i--)
		{
			if (evCardList[i].GetCount() + evCardList[0].GetCount() >= 4)
			{
				ASSERT(evCardList[i].GetCount()<=4&&evCardList[i].GetCount()>=2);
				int j = 0;
				for (; j < 4 - evCardList[i].GetCount(); j++)
				{
					bMaxCardData[j] = evCardList[0].GetAt(evCardList[0].FindIndex(j));
				}
				for (int k = 0; k < evCardList[i].GetCount(); k++)
					bMaxCardData[j+k] = evCardList[i].GetAt(evCardList[i].FindIndex(k));
				bMaxCardCount = 4;
				return CT_FIVE_FOUR_ONE;
			}
		}
	}

	//Ѱ�Һ�«
	if (bNeedCCount == 5)
	{
		for (int i = 14; i > 1; i--)
		{
			if (evCardList[i].GetCount() + evCardList[0].GetCount() == 3)
			{
				ASSERT(evCardList[i].GetCount()<=3&&evCardList[i].GetCount()>=1);
				//Ѱ��һ��
				BYTE bDoubleLogicCard = 0;
				for (int k = 2; k < 15; k++)
				{
					if (k == i) continue;
					if (evCardList[k].GetCount() >= 2)
					{
						bDoubleLogicCard = k;
						break;
					}
				}
				if (bDoubleLogicCard == 0)	break;		

				int j = 0;
				for (; j < 3 - evCardList[i].GetCount(); j++)
				{
					bMaxCardData[j] = evCardList[0].GetAt(evCardList[0].FindIndex(j));
				}
				for (int k = 0; k < evCardList[i].GetCount(); k++)
					bMaxCardData[j+k] = evCardList[i].GetAt(evCardList[i].FindIndex(k));
				bMaxCardData[3] = evCardList[bDoubleLogicCard].GetAt(evCardList[bDoubleLogicCard].FindIndex(0));
				bMaxCardData[4] = evCardList[bDoubleLogicCard].GetAt(evCardList[bDoubleLogicCard].FindIndex(1));
				bMaxCardCount = 5;
				return CT_FIVE_THREE_DEOUBLE;
			}
		}
	}

	//Ѱ��ͬ��
	if (bNeedCCount == 5)
	{
		BYTE bPossibleCard[4][5] = {0};	//���������ͬ��������
		CList<BYTE> maxCardColorList;
		for (int i = 0; i < 4; i++)
		{
			if (evColorList[i].GetCount() + evCardList[0].GetCount() >= 5)
			{
				if (evColorList[i].GetCount() >= 5)
				{
					for (int k = 0; k < 5; k++)
						bPossibleCard[i][k] = evColorList[i].GetAt(evColorList[i].FindIndex(k));
				}
				else
				{
					int j = 0;
					for (; j < 5 - evColorList[i].GetCount(); j++)
					{
						bPossibleCard[i][j] = evCardList[0].GetAt(evCardList[0].FindIndex(j));
					}
					for (int k = 0; k < evColorList[i].GetCount(); k++)
						bPossibleCard[i][j+k] = evColorList[i].GetAt(evColorList[i].FindIndex(k));
				}
				maxCardColorList.AddTail(i);
			}
		}
		if (maxCardColorList.GetCount() != 0)		
		{
			BYTE bMax = maxCardColorList.GetAt(maxCardColorList.FindIndex(0));
			for (int i = 1; i < maxCardColorList.GetCount(); i++)
			{
				BYTE bColor = maxCardColorList.GetAt(maxCardColorList.FindIndex(i));
				if (CompareCard(bPossibleCard[bMax], bPossibleCard[bColor], 5, 5, true) == enCRGreater)
					bMax = bColor;
			}
			CopyMemory(bMaxCardData, bPossibleCard[bMax], sizeof(BYTE)*5);
			bMaxCardCount = 5;
			return CT_FIVE_FLUSH;
		}
	}

	//Ѱ��˳��
	if (bNeedCCount == 5)
	{
		for (int i = 14; i > 4; i--)
		{
			BYTE bHaveCard[5] = {0};
			for (int k = 0; k < 4; k++)
				bHaveCard[k] = (evCardList[i-k].GetCount()>0);
			bHaveCard[4] = ( ((i == 5)?evCardList[14].GetCount():evCardList[i-4].GetCount()) > 0 );
			BYTE bCount = (bHaveCard[0]?1:0) + (bHaveCard[1]?1:0) + (bHaveCard[2]?1:0) + (bHaveCard[3]?1:0) + (bHaveCard[4]?1:0);
			if (bCount + evCardList[0].GetCount() >= 5)
			{
				ASSERT(bCount>=3&&bCount<=5);
				int j = 0;
				for (; j < 5 - bCount; j++)
				{
					bMaxCardData[j] = evCardList[0].GetAt(evCardList[0].FindIndex(j));
				}
				for (int k = 0; k < 4; k++)
				{
					if (bHaveCard[k])
						bMaxCardData[j++] = evCardList[i-k].GetHead();
				}

				BYTE bFirstCardNum = ( (i == 5)?14:i-4 );
				if (bHaveCard[4])
					bMaxCardData[4] = evCardList[bFirstCardNum].GetHead();

				bMaxCardCount = 5;
				return ( (i == 5)?CT_FIVE_MIXED_FLUSH_FIRST_A:CT_FIVE_MIXED_FLUSH_NO_A );
			}
		}
	}

	//Ѱ������
	for (int i = 14; i > 1; i--)
	{
		if (evCardList[i].GetCount() + evCardList[0].GetCount() == 3)
		{
			ASSERT(evCardList[i].GetCount()<=3&&evCardList[i].GetCount()>=1);

			int j = 0;
			for (; j < 3 - evCardList[i].GetCount(); j++)
			{
				bMaxCardData[j] = evCardList[0].GetAt(evCardList[0].FindIndex(j));
			}
			for (int k = 0; k < evCardList[i].GetCount(); k++)
				bMaxCardData[j+k] = evCardList[i].GetAt(evCardList[i].FindIndex(k));
			bMaxCardCount = 3;
			return CT_THREE;
		}
	}

	//Ѱ������
	if (bNeedCCount == 5)
	{
		for (int i = 14; i > 1; i--)
		{
			if (evCardList[i].GetCount() + evCardList[0].GetCount() == 2)
			{
				ASSERT(evCardList[i].GetCount()<=2&&evCardList[i].GetCount()>=0);

				//Ѱ��һ��
				BYTE bDoubleLogicCard = 0;
				for (int k = 2; k < 15; k++)
				{
					if (k == i) continue;
					if (evCardList[k].GetCount() >= 2)
					{
						bDoubleLogicCard = k;
						break;
					}
				}
				if (bDoubleLogicCard == 0)	break;	

				int j = 0;
				for (; j < 2 - evCardList[i].GetCount(); j++)
				{
					bMaxCardData[j] = evCardList[0].GetAt(evCardList[0].FindIndex(j));
				}
				for (int k = 0; k < evCardList[i].GetCount(); k++)
					bMaxCardData[j+k] = evCardList[i].GetAt(evCardList[i].FindIndex(k));

				bMaxCardData[2] = evCardList[bDoubleLogicCard].GetAt(evCardList[bDoubleLogicCard].FindIndex(0));
				bMaxCardData[3] = evCardList[bDoubleLogicCard].GetAt(evCardList[bDoubleLogicCard].FindIndex(1));

				bMaxCardCount = 4;
				return CT_TWO_DOUBLE;
			}
		}
	}

	//Ѱ�Ҷ���
	for (int i = 14; i > 1; i--)
	{
		if (evCardList[i].GetCount() + evCardList[0].GetCount() == 2)
		{
			ASSERT(evCardList[i].GetCount()<=2&&evCardList[i].GetCount()>=0);

			int j = 0;
			for (; j < 2 - evCardList[i].GetCount(); j++)
			{
				bMaxCardData[j] = evCardList[0].GetAt(evCardList[0].FindIndex(j));
			}
			for (int k = 0; k < evCardList[i].GetCount(); k++)
				bMaxCardData[j+k] = evCardList[i].GetAt(evCardList[i].FindIndex(k));

			bMaxCardCount = 2;
			return CT_ONE_DOUBLE;
		}
	}

	//Ѱ��ɢ��
	for (int i = 14; i > 1; i--)
	{
		if (evCardList[i].GetCount() == 1)
		{
			bMaxCardCount = 1;
			bMaxCardData[0] = evCardList[i].GetHead();
			return CT_SINGLE;
		}
	}


	return CT_INVALID;
}
/****************************************************
*��������AnalyseMaxTransform 
*���ܣ�  �������ͺ�������Ŀ���ƽ������ת�� JJ
*������	 cbCardData			������			IN
		 cbCardCount		����Ŀ			IN
		 bTransCardData		ת����������	OUT
		 TransData			ת����Ϣ		OUT
*����ֵ��0					��ת��
		 other				ת���������
****************************************************/
BYTE CGameLogic::AnalyseMaxTransform(const BYTE cbCardData[], const BYTE cbCardCount, BYTE bTransCardData[], tagTransData & TransData)
{
	ASSERT(cbCardCount==3 || cbCardCount==5);

	//��������
	BYTE bTempCardData[5] = {0};		//����������������
	BYTE bKcount = 0;					//����Ŀ

	//��ʼ��
	memcpy(bTransCardData, cbCardData, sizeof(BYTE)*cbCardCount);
	memcpy(bTempCardData, cbCardData, sizeof(BYTE)*cbCardCount);

	//��������0��ͳ��	
	for (int i = 0; i < cbCardCount; i++)
	{
		if (bTempCardData[i] == 0x41 || bTempCardData[i] == 0x42)
		{
			TransData.bHaveKing += ((bTempCardData[i]==0x41)?1:2);
			bTempCardData[i] = 0;
			bKcount++;
		}
	}
	TransData.bKingCount = bKcount;

	//��������
	if (bKcount == 0)
		return 0;

	//���������������
	SortCardList(bTempCardData , cbCardCount , enDescend) ;
	tagAnalyseData	tad = {0};
	AnalyseCard(bTempCardData, cbCardCount, tad);

	//ը��ֱ�ӷ���
	/*if (tad.bFourCount == 1)
	return 0;*/

	//Ǳ���ж�	(�Ӵ�С���μ���)

	//ͬ��˳ (5���� �� ͬ�� �� ȫ����)
	if ( 5 == cbCardCount && tad.bSameColor &&	0 == (tad.bTwoCount+tad.bThreeCount+tad.bFourCount) )				 	
	{
		//A��1�ã���A �� �ڶ�������С�ڵ���5, ��С˳��A2345
		if (14 == GetCardLogicValue(bTempCardData[0]) && 5 >= GetCardLogicValue(bTempCardData[1]))		
		{
			TransformCard(bTempCardData, cbCardCount, bKcount, CT_FIVE_STRAIGHT_FLUSH_FIRST_A, tad, bTransCardData, TransData);
			return CT_FIVE_STRAIGHT_FLUSH_FIRST_A;
		}

		//����Ƽ�ȥ��С��С�ڵ���4
		if ( 4 >= GetCardLogicValue(bTempCardData[0]) - GetCardLogicValue(bTempCardData[5-bKcount-1]) )
		{
			TransformCard(bTempCardData, cbCardCount, bKcount, CT_FIVE_STRAIGHT_FLUSH, tad, bTransCardData, TransData);
			return CT_FIVE_STRAIGHT_FLUSH;
		}
	}

	//ը��	(5���� �� ����3����Ŀ����һ����˫��2����Ŀ����3����Ŀ����һ)
	if ( 5 == cbCardCount &&  ((1==bKcount&&1==tad.bThreeCount) || (2==bKcount&&(1==tad.bThreeCount||1==tad.bTwoCount))) )
	{
		TransformCard(bTempCardData, cbCardCount, bKcount, CT_FIVE_FOUR_ONE, tad, bTransCardData, TransData);
		return CT_FIVE_FOUR_ONE;
	}

	//��«  (5���� �� ����2����Ŀ����2��
	if ( 5 == cbCardCount && 1 == bKcount &&2 == tad.bTwoCount )
	{
		TransformCard(bTempCardData, cbCardCount, bKcount, CT_FIVE_THREE_DEOUBLE, tad, bTransCardData, TransData);
		return CT_FIVE_THREE_DEOUBLE;
	}

	//ͬ��  (5���� �� ȫ��ͬ����
	if (5 == cbCardCount && tad.bSameColor)
	{	
		TransformCard(bTempCardData, cbCardCount, bKcount, CT_FIVE_FLUSH, tad, bTransCardData, TransData);
		return CT_FIVE_FLUSH;
	}

	//˳��	(5���� �� ��ͬ�� �� ȫ����)
	if ( 5 == cbCardCount && !tad.bSameColor &&	0 == (tad.bTwoCount+tad.bThreeCount+tad.bFourCount) )				 	
	{	
		//A��1�ã���A �� �ڶ�������С�ڵ���5, ��С˳��A2345
		if (14 == GetCardLogicValue(bTempCardData[0]) && 5 >= GetCardLogicValue(bTempCardData[1]))		
		{
			TransformCard(bTempCardData, cbCardCount, bKcount, CT_FIVE_MIXED_FLUSH_FIRST_A, tad, bTransCardData, TransData);
			return CT_FIVE_MIXED_FLUSH_FIRST_A;
		}

		//����Ƽ�ȥ��С��С�ڵ���4
		if ( 4 >= GetCardLogicValue(bTempCardData[0]) - GetCardLogicValue(bTempCardData[5-bKcount-1]) )
		{
			TransformCard(bTempCardData, cbCardCount, bKcount, CT_FIVE_MIXED_FLUSH_NO_A, tad, bTransCardData, TransData);
			return CT_FIVE_MIXED_FLUSH_NO_A;
		}
	}

	//����  (3��5���� �� ����2����Ŀ����1��˫��ȫ����)
	if ( (1 == bKcount && 1 == tad.bTwoCount) || (2 == bKcount && 0 == (tad.bTwoCount+tad.bThreeCount+tad.bFourCount)) )
	{
		TransformCard(bTempCardData, cbCardCount, bKcount, CT_THREE, tad, bTransCardData, TransData);
		return CT_THREE;
	}

	//����  (������)

	//һ��  (3��5���� �ҵ���ȫ����)
	if (1 == bKcount && 0 == (tad.bTwoCount+tad.bThreeCount+tad.bFourCount))
	{
		TransformCard(bTempCardData, cbCardCount, bKcount, CT_ONE_DOUBLE, tad, bTransCardData, TransData);
		return CT_ONE_DOUBLE;
	}

	//����������˵�ǲ����ܵ��ģ�����ֻ��˵��Bug������
	//MyMsgBox(_T("%d %d [%s %s %s %s %s]"), cbCardCount, bKcount, bTempCardData[0]
	//, bTempCardData[1], bTempCardData[2], bTempCardData[3], bTempCardData[4]);
	return CT_SINGLE;
}

/****************************************************
*��������TransformCard 
*���ܣ�  ���ڶ���ȷ��Ǳ�����͵��ƽ���ת��,ֻ����AnalyseMaxTransform����ʹ�� JJ
*������	 cbNkCardData		��ȥ��������	IN
		 cbCardCount		����Ŀ(3or5)			IN
		 bKCount			����Ŀ(1or2)			IN
		 bCardType			��Ǳ������(��ת������)  IN
		 tad				������Ϣ				IN
		 bTransCardData		ת����������(��������)	OUT
		 TransData			ת����Ϣ				INOUT
*����ֵ����(��������������ݣ����ݼ���ɱ�����Ψһ����
			��AnalyseMaxTransform����)
****************************************************/
void CGameLogic::TransformCard(const BYTE cbNkCardData[], BYTE bCardCount, BYTE bKCount, BYTE bCardType, const tagAnalyseData & tad,
							   BYTE bTransCardData[], tagTransData & TransData)
{
	ASSERT( (3==bCardCount||5==bCardCount) && (1==bKCount||2==bKCount) );

	//��������
	CList<BYTE>	cardList;			//��¼ת����������		
	BYTE bTempCardData[5] = {0};

	//��ʼ��
	memcpy(bTempCardData, cbNkCardData, sizeof(BYTE)*(bCardCount-bKCount));
	for (WORD i = 0; i < bCardCount - bKCount; i++)
	{
		cardList.AddTail(bTempCardData[i]);
	}

	//ת����ʼ
	switch (bCardType)
	{
	//˳�ӵ�ת���㷨�Ǵӷ�����С�ƿ�ʼ,������ֱ����������Ʒ����п�ȱ�����ȱ��û�п�ȱ������Ƿ��յ������ͷ
	case CT_FIVE_STRAIGHT_FLUSH_FIRST_A:
	case CT_FIVE_STRAIGHT_FLUSH:
	case CT_FIVE_MIXED_FLUSH_FIRST_A:
	case CT_FIVE_MIXED_FLUSH_NO_A:
		{	
			//����У��
			ASSERT(5==bCardCount);

			//��������			(��˳��ʹ����������)
			SortCardList(bTempCardData, 5-bKCount, enAscend);

			//�������
			cardList.RemoveAll();

			//�������
			BYTE bLogicHeadCard = 0;		//��С�Ƶ��߼�ֵ
			BYTE bTempCount = 0;			//ת�����е���λ��
			BYTE bCardColor = GetCardColor(bTempCardData[0]);	

			//����ײ�
			if (bCardType == CT_FIVE_STRAIGHT_FLUSH_FIRST_A || bCardType == CT_FIVE_MIXED_FLUSH_FIRST_A)
			{
				bLogicHeadCard = 1;
				cardList.AddTail(bTempCardData[5-bKCount-1]);
				bTempCount = 0;
			}
			else 
			{
				bLogicHeadCard = GetCardLogicValue(bTempCardData[0]);
				cardList.AddTail(bTempCardData[0]);
				bTempCount = 1;
			}

			//���ʣ��
			for (int i = 1; i < 5; i++)
			{
				if (GetCardLogicValue(bTempCardData[bTempCount]) != bLogicHeadCard+i)	
				{
					BYTE transCard = 0;
					if (bCardType == CT_FIVE_STRAIGHT_FLUSH_FIRST_A || bCardType == CT_FIVE_STRAIGHT_FLUSH)
						transCard = (bCardColor<<4) + bLogicHeadCard + i;
					else
						transCard = 0x30 + bLogicHeadCard + i;
					cardList.AddTail(transCard);
					TransData.transList.AddTail(transCard);
				}
				else
				{
					cardList.AddTail(bTempCardData[bTempCount]);
					bTempCount++;
				}
				if (bTempCount == 5 - bKCount)
					break;
			}

			//ʣ������
			if (cardList.GetCount() != 5)
			{
				while (cardList.GetCount() < 5)
				{
					BYTE bLastCard = cardList.GetTail();
					BYTE bFirstCard = cardList.GetHead();
					BYTE transCard = 0;
					bool bMaxEnd = (GetCardLogicValue(bLastCard)==14);
					BYTE bExValue = GetCardLogicValue(bMaxEnd?bFirstCard:bLastCard) + (bMaxEnd?-1:1);
					if (bExValue == 14)		bExValue = 1;

					if (bCardType == CT_FIVE_STRAIGHT_FLUSH_FIRST_A || bCardType == CT_FIVE_STRAIGHT_FLUSH)
						transCard = (bCardColor<<4) + bExValue;
					else
						transCard = 0x30 + bExValue;		

					if (bMaxEnd)
						cardList.AddHead(transCard);
					else
						cardList.AddTail(transCard);
					TransData.transList.AddTail(transCard);
				}
			}

			break;
		}
	//ը����ת���㷨��ֱ���ҵ�3�ŵĻ���2�ŵģ������ֵͬ�ĺ�����
	case CT_FIVE_FOUR_ONE:
		{
			//����У��
			ASSERT(5==bCardCount);

			//����ת��
			if (bKCount == 1)
			{
				BYTE transCard = 0x30 + GetCardValue(bTempCardData[tad.bThreeFirst[0]]);
				cardList.AddTail(transCard);
				TransData.transList.AddTail(transCard);
			}
			else 
			{
				if (tad.bThreeCount == 1)
				{
					BYTE transCard = 0x30 + GetCardValue(bTempCardData[tad.bThreeFirst[0]]);
					cardList.AddTail(transCard);
					TransData.transList.AddTail(transCard);

					if ( GetCardLogicValue(bTempCardData[tad.bThreeFirst[0]]) == 14)
						transCard = 0x3D;
					else
						transCard = 0x31;
					cardList.AddTail(transCard);
					TransData.transList.AddTail(transCard);
				}
				else 
				{
					BYTE transCard = 0x30 + GetCardValue(bTempCardData[tad.bTwoFirst[0]]);
					cardList.AddTail(transCard);
					cardList.AddTail(transCard);
					TransData.transList.AddTail(transCard);
					TransData.transList.AddTail(transCard);
				}
			}

			break;
		} 
	//��«��ת���㷨��ֱ���ҵ�2��2���д�ģ������ֵͬ�ĺ�����
	case CT_FIVE_THREE_DEOUBLE:
		{
			//����У��
			ASSERT(5==bCardCount&&bKCount==1);

			//����ת��
			BYTE transCard = 0x30 + GetCardValue(bTempCardData[tad.bTwoFirst[0]]);
			cardList.AddTail(transCard);
			TransData.transList.AddTail(transCard);
			break;
		} 
	//ͬ����ת���㷨�������ͬ��
	case CT_FIVE_FLUSH:
		{
			//����У��
			ASSERT(5==bCardCount);

			BYTE bCardColor = GetCardColor(bTempCardData[0]);

			//����ת��
			while (cardList.GetCount() < 5)
			{
				BYTE transCard = (bCardColor<<4) + 0x01;
				cardList.AddTail(transCard);
				TransData.transList.AddTail(transCard);
			}
			break;
		} 
	//������ת���㷨��ֱ���ҵ�2�ŵĻ��ߵ������ģ������ֵͬ�ĺ�����
	case CT_THREE:
		{
			//����ת��
			if (tad.bTwoCount == 1)
			{
				BYTE transCard = 0x30 + GetCardValue(bTempCardData[tad.bTwoFirst[0]]);
				cardList.AddTail(transCard);
				TransData.transList.AddTail(transCard);
			}
			else
			{
				while (cardList.GetCount() < bCardCount)
				{
					BYTE transCard = 0x30 + GetCardValue(bTempCardData[0]);
					cardList.AddTail(transCard);
					TransData.transList.AddTail(transCard);
				}
			}
			break;
		}
	//һ�Ե�ת���㷨��ֱ���ҵ��������ģ������ֵͬ�ĺ�����
	case CT_ONE_DOUBLE:
		{
			//����У��
			ASSERT(1==bKCount);

			//����ת��
			BYTE transCard = 0x30 + GetCardValue(bTempCardData[0]);
			cardList.AddTail(transCard);
			TransData.transList.AddTail(transCard);

			break;
		}
	default:
		{
			//MyMsgBox(_T("CGameLogic::TransFormCard [%d]"), bCardType);
			break;
		}
	}
	//�����Ϣ
	ASSERT(cardList.GetCount() == 5 || cardList.GetCount() == 3);
	for (int i = 0; i < bCardCount; i++)
	{
		bTransCardData[i] = cardList.GetAt(cardList.FindIndex(i));
	}

	return;	
}

//ͬ������
BYTE CGameLogic::SearchSameCard( const BYTE cbHandCardData[], BYTE cbHandCardCount, BYTE cbSameCardCount, tagSearchCardResult *pSearchCardResult )
{
	//���ý��
	if( pSearchCardResult )
		ZeroMemory(pSearchCardResult,sizeof(tagSearchCardResult));
	BYTE cbResultCount = 0;

	//�����˿�
	BYTE cbCardData[13];
	BYTE cbCardCount=cbHandCardCount;
	CopyMemory(cbCardData,cbHandCardData,sizeof(BYTE)*cbHandCardCount);

	//�����˿�
	SortCardList(cbCardData, cbCardCount, enAscend);

	//�����˿�
	tagAnalyseResult AnalyseResult = {};
	AnalysebCardData( cbCardData,cbCardCount,AnalyseResult);

	BYTE cbBlockIndex = cbSameCardCount-1;
	do
	{
		for( BYTE i = 0; i < AnalyseResult.cbBlockCount[cbBlockIndex]; i++ )
		{
			BYTE cbIndex = (AnalyseResult.cbBlockCount[cbBlockIndex]-i-1)*(cbBlockIndex+1);

			//�����˿�
			CopyMemory(pSearchCardResult->cbResultCard[cbResultCount], &AnalyseResult.cbCardData[cbBlockIndex][cbIndex], cbSameCardCount*sizeof(BYTE));
			pSearchCardResult->cbCardCount[cbResultCount] = cbSameCardCount;
			cbResultCount++;
		}
		cbBlockIndex++;
	}while( cbBlockIndex < CountArray(AnalyseResult.cbBlockCount));

	if( pSearchCardResult )
		pSearchCardResult->cbSearchCount = cbResultCount;
	return cbResultCount;
}

BYTE CGameLogic::SearchTakeCardType( const BYTE cbHandCardData[], BYTE cbHandCardCount, BYTE cbSameCount, BYTE cbTakeCardCount, tagSearchCardResult *pSearchCardResult )
{
	//���ý��
	if( pSearchCardResult )
		ZeroMemory(pSearchCardResult,sizeof(tagSearchCardResult));
	BYTE cbResultCount = 0;

	//Ч��
	ASSERT( cbSameCount == 3 || cbSameCount == 4 );
	ASSERT( cbTakeCardCount == 2 || cbTakeCardCount == 1 );
	if( cbSameCount != 3 && cbSameCount != 4 )
		return cbResultCount;
	if( cbTakeCardCount != 2 && cbTakeCardCount != 1 )
		return cbResultCount;
	if(cbHandCardCount<cbSameCount+cbTakeCardCount)
		return cbResultCount;

	//����ͬ��
	tagSearchCardResult SameCardResult;
	ZeroMemory(&SameCardResult, sizeof(SameCardResult));
	BYTE cbSameCardResultCount = SearchSameCard( cbHandCardData,cbHandCardCount,cbSameCount,&SameCardResult);

	if( cbSameCardResultCount > 0)
	{
		//��������
		tagSearchCardResult TakeCardResult;
		ZeroMemory(&TakeCardResult, sizeof(TakeCardResult));
		BYTE cbTakeCardResultCount = SearchSameCard(cbHandCardData,cbHandCardCount,cbTakeCardCount,&TakeCardResult);

		//������ɴ���
		if (cbTakeCardResultCount > 0)
		{
			for (BYTE i=0; i<cbSameCardResultCount; i++)
			{
				for (BYTE j=0; j<cbTakeCardResultCount; j++)
				{
					//���������� AAA
					//����һ�ԣ�AA 33 66 99
					//������ϣ�AAAAA��
					//������ϣ�AAA33��AAA66��AAA99
					if (GetCardLogicValue(SameCardResult.cbResultCard[i][0]) == GetCardLogicValue(TakeCardResult.cbResultCard[j][0]))
						continue;

					//�����˿�
					pSearchCardResult->cbCardCount[cbResultCount] = cbSameCount+cbTakeCardCount;
					CopyMemory(pSearchCardResult->cbResultCard[cbResultCount], SameCardResult.cbResultCard[i], sizeof(BYTE)*cbSameCount);
					CopyMemory(&pSearchCardResult->cbResultCard[cbResultCount][cbSameCount], TakeCardResult.cbResultCard[j], sizeof(BYTE)*cbTakeCardCount);
					cbResultCount++;
				}
			}
		}
	}

	if( pSearchCardResult )
		pSearchCardResult->cbSearchCount = cbResultCount;
	return cbResultCount;
}


//����ͬ��
BYTE CGameLogic::SearchSameColorType(const BYTE cbHandCardData[], BYTE cbHandCardCount, BYTE cbSameCount,  tagSearchCardResult *pSearchCardResult)
{
	//���ý��
	if( pSearchCardResult )
		ZeroMemory(pSearchCardResult,sizeof(tagSearchCardResult));
	BYTE cbResultCount = 0;
	
	//�����˿�
	BYTE cbCardData[13] = {0};
	CopyMemory(cbCardData, cbHandCardData, sizeof(BYTE)*cbHandCardCount);
	SortCardList(cbCardData, cbHandCardCount, enDescend);

	//ͬ������
	BYTE cbSameCardCount[4] = {0};
	BYTE cbSameCardData[4][13] = {0};

	//ͳ��ͬ��
	for (BYTE i = 0; i < cbHandCardCount; i++)
	{
		//��ȡ��ɫ
		BYTE cbCardColor = GetCardColor(cbCardData[i]);

		//ԭ����Ŀ
		BYTE cbCount = cbSameCardCount[cbCardColor];

		//׷���˿�
		cbSameCardData[cbCardColor][cbCount] = cbCardData[i];
		cbSameCardCount[cbCardColor]++;
	}

	//�ж��Ƿ���cbSameCount
	for (int i = 0; i < 4; i++)
	{
		if (cbSameCardCount[i] >= cbSameCount)
		{
			for (int j = 0; j<=(cbSameCardCount[i]%cbSameCount); j++)
			{
				pSearchCardResult->cbCardCount[cbResultCount] = cbSameCount;
				CopyMemory(pSearchCardResult->cbResultCard[cbResultCount], &cbSameCardData[i][j], sizeof(BYTE)*cbSameCount);
					
				cbResultCount++;
			}
		}
	}

	if( pSearchCardResult )
		pSearchCardResult->cbSearchCount = cbResultCount;

	return pSearchCardResult->cbSearchCount;
}

BYTE CGameLogic::SearchLineCardType( const BYTE cbHandCardData[], BYTE cbHandCardCount, BYTE cbLineCount, tagSearchCardResult *pSearchCardResult )
{
	//���ý��
	if( pSearchCardResult )
		ZeroMemory(pSearchCardResult,sizeof(tagSearchCardResult));

	//�������
	BYTE cbResultCount = 0;
	BYTE cbBlockCount = 1;

	//����A
	if (cbLineCount > 14) return cbResultCount;

	//�����ж�
	if (cbHandCardCount < cbLineCount) return cbResultCount;

	//�����˿�
	BYTE cbCardData[HAND_CARD_COUNT];
	BYTE cbCardCount=cbHandCardCount;
	CopyMemory(cbCardData,cbHandCardData,sizeof(BYTE)*cbHandCardCount);

	//�����˿�
	SortCardList(cbCardData,cbCardCount, enDescend);

	//�����˿�
	tagDistributing Distributing;
	ZeroMemory(&Distributing, sizeof(Distributing));
	AnalysebDistributing(cbCardData,cbCardCount,Distributing);

	//����˳��
	BYTE cbTmpLinkCount = 0;
	for (BYTE cbValueIndex=0;cbValueIndex<13;cbValueIndex++)
	{
		//�����ж�
		if (Distributing.cbDistributing[cbValueIndex][cbIndexCount]< cbBlockCount)
		{
			if (cbTmpLinkCount < cbLineCount)
			{
				cbTmpLinkCount=0;
				continue;
			}
			else cbValueIndex--;
		}
		else 
		{
			cbTmpLinkCount++;

			//Ѱ�����
			if (cbLineCount == 0) continue;
		}

		if( cbTmpLinkCount >= cbLineCount )
		{
			if( pSearchCardResult == NULL ) return 1;

			ASSERT( cbResultCount < CountArray(pSearchCardResult->cbCardCount) );

			//�����˿�
			BYTE cbCount = 0;
			for( BYTE cbIndex = cbValueIndex+1-cbTmpLinkCount; cbIndex <= cbValueIndex; cbIndex++ )
			{
				BYTE cbTmpCount = 0;
				for (BYTE cbColorIndex=0;cbColorIndex<4;cbColorIndex++)
				{
					for( BYTE cbColorCount = 0; cbColorCount < Distributing.cbDistributing[cbIndex][3-cbColorIndex]; cbColorCount++ )
					{
						pSearchCardResult->cbResultCard[cbResultCount][cbCount++]=MakeCardData(cbIndex,3-cbColorIndex);

						if (++cbTmpCount == cbBlockCount) break;
					}
					if (cbTmpCount == cbBlockCount) break;
				}
			}

			//���ñ���
			pSearchCardResult->cbCardCount[cbResultCount] = cbCount;
			cbResultCount++;
			
			cbTmpLinkCount--;
		}
	}

	//����˳��
	if( cbTmpLinkCount >= cbLineCount-1 && cbValueIndex == 13 )
	{
		if( Distributing.cbDistributing[0][cbIndexCount] >= cbBlockCount || cbTmpLinkCount >= cbLineCount )
		{
			if( pSearchCardResult == NULL ) return 1;

			ASSERT(cbResultCount < CountArray(pSearchCardResult->cbCardCount));
			
			//�����˿�
			BYTE cbCount = 0;
			BYTE cbTmpCount = 0;
			for( BYTE cbIndex = cbValueIndex-cbTmpLinkCount; cbIndex < 13; cbIndex++ )
			{
				cbTmpCount = 0;
				for (BYTE cbColorIndex=0;cbColorIndex<4;cbColorIndex++)
				{
					for( BYTE cbColorCount = 0; cbColorCount < Distributing.cbDistributing[cbIndex][3-cbColorIndex]; cbColorCount++ )
					{
						pSearchCardResult->cbResultCard[cbResultCount][cbCount++]=MakeCardData(cbIndex,3-cbColorIndex);
						
						if (++cbTmpCount == cbBlockCount) break;
					}
					if (cbTmpCount == cbBlockCount) break;
				}
			}

			//����A
			if( Distributing.cbDistributing[0][cbIndexCount] >= cbBlockCount )
			{
				cbTmpCount = 0;
				for (BYTE cbColorIndex=0;cbColorIndex<4;cbColorIndex++)
				{
					for( BYTE cbColorCount = 0; cbColorCount < Distributing.cbDistributing[0][3-cbColorIndex]; cbColorCount++ )
					{
						pSearchCardResult->cbResultCard[cbResultCount][cbCount++]=MakeCardData(0,3-cbColorIndex);
						
						if (++cbTmpCount == cbBlockCount) break;
					}
					if (cbTmpCount == cbBlockCount) break;
				}
			}

			//���ñ���
			pSearchCardResult->cbCardCount[cbResultCount] = cbCount;
			cbResultCount++;
		}
	}
	
	if( pSearchCardResult )
			pSearchCardResult->cbSearchCount = cbResultCount;
	return cbResultCount;
}

//����ͬ��˳
BYTE CGameLogic::SearchSameColorLineType(const BYTE cbHandCardData[], BYTE cbHandCardCount, BYTE cbLineCount, tagSearchCardResult *pSearchCardResult)
{
	//���ý��
	if (pSearchCardResult)
		ZeroMemory(pSearchCardResult,sizeof(tagSearchCardResult));
	BYTE cbResultCount = 0;

	//�����ж�
	if (cbHandCardCount < cbLineCount) return cbResultCount;

	//��������
	tagSearchCardResult tagTempResult, tagTempResult2;
	ZeroMemory(&tagTempResult, sizeof(tagTempResult));
	ZeroMemory(&tagTempResult2, sizeof(tagTempResult2));

	//����ͬ��
	SearchSameColorType(cbHandCardData, cbHandCardCount, 5, &tagTempResult);

	//ͬ��������˳��
	for (int i = 0; i < tagTempResult.cbSearchCount; i++)
	{
		BYTE cbLineResultCount = SearchLineCardType(tagTempResult.cbResultCard[i], tagTempResult.cbCardCount[i], 5, &tagTempResult2);
		if (cbLineResultCount > 0)
		{
			pSearchCardResult->cbCardCount[cbResultCount] = cbLineCount;
			CopyMemory(pSearchCardResult->cbResultCard[cbResultCount], tagTempResult2.cbResultCard[0], sizeof(BYTE)*cbLineCount);
			cbResultCount++;
		}
	}

	if( pSearchCardResult )
		pSearchCardResult->cbSearchCount = cbResultCount;
	return cbResultCount;
}

//�����˿�
BYTE CGameLogic::MakeCardData(BYTE cbValueIndex, BYTE cbColorIndex)
{
	return (cbColorIndex<<4)|(cbValueIndex+1);
}
/****************************************************
*��������CardTypeToWeights 
*���ܣ�  ���ڽ��Ƶ�����ת����Ȩֵ���
*������	 bType		������	IN
*����ֵ��int(Ȩֵ��������ֻ�����е�����������ӦȨֵ�Ļ���)
****************************************************/
int CGameLogic::CardTypeToWeights(BYTE bType, BYTE cbIndex)
{
	switch(bType)
	{
	case  CT_INVALID:												//��������
		{
			return -1;
		}
	case  CT_SINGLE:											    //��������
		{
			return 0;
		}
	case CT_ONE_DOUBLE:										        //ֻ��һ��
		{
			return 2;
		}
	case CT_TWO_DOUBLE:										        //��������
		{
			return 3;
		}
	case CT_THREE:													//��������
		{
			if(1 == cbIndex)
			{
				return (4+3);
			}
			else
			{
				return 4;
			}
			
		}
	case CT_FIVE_MIXED_FLUSH_FIRST_A:							    //Aǰ˳��
		{
			return 5;
		}
	case CT_FIVE_MIXED_FLUSH_NO_A:								    //��ͨ˳��
		{
			return 6;
		}
	case CT_FIVE_MIXED_FLUSH_BACK_A:								//A��˳��
		{
			return 7;
		}
	case CT_FIVE_FLUSH:								                //ͬ��
		{
			return 8;
		}
	case CT_FIVE_THREE_DEOUBLE:								        //����һ��
		{
			return 9;
		}
	case CT_FIVE_FOUR_ONE:								            //�Ĵ�һ��
		{
			if(2 == cbIndex)
			{
				return (10+8);
			}
			if(3 == cbIndex)
			{
				return (10+4);
			}
			else
			{
				return 10;
			}
			
		}
	case CT_FIVE_STRAIGHT_FLUSH_FIRST_A:							//Aͬ��˳
		{
			if(2 == cbIndex)
			{
				return (11+10);
			}
			if(3 == cbIndex)
			{
				return (11+5);
			}
			else
			{
				return 11;
			}
		}
	case CT_FIVE_STRAIGHT_FLUSH:									//ͬ��˳��
		{
			if(2 == cbIndex)
			{
				return (12+10);
			}
			if(3 == cbIndex)
			{
				return (12+5);
			}
			else
			{
				return 12;
			}
		}
	default:
		{
			return -1;
		}
	}

	//switch(bType)
	//{
	//case  CT_INVALID:												//��������
	//	{
	//		return -1;
	//	}
	//case  CT_SINGLE:											    //��������
	//	{
	//		return 0;
	//	}
	//case CT_ONE_DOUBLE:										        //ֻ��һ��
	//	{
	//		return 2;
	//	}
	//case CT_TWO_DOUBLE:										        //��������
	//	{
	//		return 4;
	//	}
	//case CT_THREE:													//��������
	//	{
	//		return 8;
	//	}
	//case CT_FIVE_MIXED_FLUSH_FIRST_A:							    //Aǰ˳��
	//	{
	//		return 15;
	//	}
	//case CT_FIVE_MIXED_FLUSH_NO_A:								    //��ͨ˳��
	//case CT_FIVE_MIXED_FLUSH_BACK_A:								//A��˳��
	//	{
	//		return 16;
	//	}
	//case CT_FIVE_FLUSH:								                //ͬ��
	//	{
	//		return 32;
	//	}
	//case CT_FIVE_THREE_DEOUBLE:								        //����һ��
	//	{
	//		return 64;
	//	}
	//case CT_FIVE_FOUR_ONE:								            //�Ĵ�һ��
	//	{
	//		return 128;
	//	}
	//case CT_FIVE_STRAIGHT_FLUSH_FIRST_A:							//Aͬ��˳
	//	{
	//		return 255;
	//	}
	//case CT_FIVE_STRAIGHT_FLUSH:									//ͬ��˳��
	//	{
	//		return 255;
	//	}
	//default:
	//	{
	//		return -1;
	//	}
	//}
}

/****************************************************
*��������OptimizationCombo 
*���ܣ�  ���Ѿ��ںõ������н����Ż����ðڳ������Եø�����
*������	 bInFirstList		��һ��������	IN/OUT
*������	 bInNextList		�ڶ���������	IN/OUT
*������	 bFirstCount		��һ��������	IN
*������	 bNextCount 		�ڶ���������	IN
*����ֵ����(������Ҫ��֤��һ�����ʹ�С�����ڵڶ�������)
****************************************************/
void CGameLogic::OptimizationCombo(BYTE bInFirstList[], BYTE bInNextList[], BYTE bFirstCount, BYTE bNextCount)
{

	if(bFirstCount <= bNextCount && (3 == bFirstCount || 5==bFirstCount) && 5 == bNextCount)
	{

		//��ȡ����
		BYTE cbMaxCard = 0;
		BYTE bNextType=GetCardType(bInNextList,bNextCount,cbMaxCard);
		BYTE bFirstType=GetCardType(bInFirstList,bFirstCount,cbMaxCard);
		BYTE bFirstCard[5],bNextCard[5];
		int iTemp1,iTemp2;

        if(bFirstCount == bNextCount && enCRLess == CompareCard(bInFirstList,bInNextList, 5, 5, true))
		{
			CopyMemory(bFirstCard,bInFirstList,sizeof(BYTE) * 5);
			CopyMemory(bNextCard,bInNextList,sizeof(BYTE) * 5);
			CopyMemory(bInFirstList,bNextCard,sizeof(BYTE) * 5);
			CopyMemory(bInNextList,bFirstCard,sizeof(BYTE) * 5);
		}


		//������ͬ�������Ż�
		if(bFirstType == bNextType)
		{
			return;
		}
		switch(bNextType)
		{
		case CT_INVALID:												//��������
		case CT_SINGLE:											        //��������
		case CT_FIVE_MIXED_FLUSH_FIRST_A:							    //Aǰ˳��
		case CT_FIVE_MIXED_FLUSH_NO_A:								    //��ͨ˳��
		case CT_FIVE_MIXED_FLUSH_BACK_A:								//A��˳��
		case CT_FIVE_FLUSH:								                //ͬ��
		case CT_FIVE_STRAIGHT_FLUSH_FIRST_A:							//Aͬ��˳
		case CT_FIVE_STRAIGHT_FLUSH:									//ͬ��˳��
			{
				//��Щ���������Ż�ֱ�ӷ���
				return ;
			}
		case CT_ONE_DOUBLE:										        //ֻ��һ��
		case CT_THREE:													//��������
		case CT_FIVE_FOUR_ONE:								            //�Ĵ�һ��
			{
				//��ԭ�еĶ���(���š�����)���б��治�����޸�
				for(int i = 0;i < (bNextCount - 1);i ++)
				{
					if(GetCardLogicValue(bInNextList[i]) == GetCardLogicValue(bInNextList[i + 1]))
					{
						iTemp1 = GetCardLogicValue(bInNextList[i]);
						break;
					}
				}

				//��ʣ�µĵ��ƽ����û����ڲ��ƻ����͵�����½���ĵ��Ʒ���ǰ��һע
				for(int i = 0;i < bNextCount; i ++)
				{
					if(GetCardLogicValue(bInNextList[i]) != iTemp1)
					{
						for(int j = bFirstCount - 1;j >= 0; j --)
						{
							if(GetCardLogicValue(bInNextList[i]) > GetCardLogicValue(bInFirstList[j]))
							{
								CopyMemory(bFirstCard,bInFirstList,sizeof(BYTE) * bFirstCount);
								CopyMemory(bNextCard,bInNextList,sizeof(BYTE) * bNextCount);
								bFirstCard[j] = bInNextList[i];
								bNextCard[i] = bInFirstList[j];
								SortCardList(bFirstCard,bFirstCount,enDescend);
								SortCardList(bNextCard,bNextCount,enDescend);
								if(bFirstType == GetCardType(bFirstCard,bFirstCount,cbMaxCard)&&
									bNextType == GetCardType(bNextCard,bNextCount,cbMaxCard))
								{
									CopyMemory(bInFirstList,bFirstCard,sizeof(BYTE) * bFirstCount);
									CopyMemory(bInNextList,bNextCard,sizeof(BYTE) * bNextCount);
									i = -1;
									break;
								}
							}
						}
					}
				}
				return;
			}
		case CT_TWO_DOUBLE:										        //��������
			{
				//��ԭ�е��������ӽ��б��治�����޸�
				for(int i = 0;i < (bNextCount - 1);i ++)
				{
					if(GetCardLogicValue(bInNextList[i]) == GetCardLogicValue(bInNextList[i + 1]))
					{
						iTemp1 = GetCardLogicValue(bInNextList[i]);
						break;
					}
				}
				for(int i = bNextCount - 2;i >= 0;i --)
				{
					if(GetCardLogicValue(bInNextList[i]) == GetCardLogicValue(bInNextList[i + 1]))
					{
						iTemp2 = GetCardLogicValue(bInNextList[i]);
						break;
					}
				}

				//��ʣ�µĵ��ƽ����û����ڲ��ƻ����͵�����½���ĵ��Ʒ���ǰ��һע
				for(int i = 0;i < bNextCount; i ++)
				{
					if(GetCardLogicValue(bInNextList[i]) != iTemp1  && GetCardLogicValue(bInNextList[i]) != iTemp2) 
					{
						for(int j = bFirstCount - 1;j >= 0; j --)
						{
							if(GetCardLogicValue(bInNextList[i]) > GetCardLogicValue(bInFirstList[j]))
							{
								CopyMemory(bFirstCard,bInFirstList,sizeof(BYTE) * bFirstCount);
								CopyMemory(bNextCard,bInNextList,sizeof(BYTE) * bNextCount);
								bFirstCard[j] = bInNextList[i];
								bNextCard[i] = bInFirstList[j];
								SortCardList(bFirstCard,bFirstCount,enDescend);
								SortCardList(bNextCard,bNextCount,enDescend);
								if(bFirstType == GetCardType(bFirstCard,bFirstCount,cbMaxCard)&&
									bNextType == GetCardType(bNextCard,bNextCount,cbMaxCard))
								{
									CopyMemory(bInFirstList,bFirstCard,sizeof(BYTE) * bFirstCount);
									CopyMemory(bInNextList,bNextCard,sizeof(BYTE) * bNextCount);
									i = -1;
									break;
								}
							}
						}
					}
				}
				return;
			}
		case CT_FIVE_THREE_DEOUBLE:								        //����һ��
			{
				//�ҳ�����
				if(GetCardLogicValue(bInNextList[1]) == GetCardLogicValue(bInNextList[2]))
				{
					iTemp1 = GetCardLogicValue(bInNextList[3]);
				}
				else
				{
					iTemp1 = GetCardLogicValue(bInNextList[1]);
				}
				//�Զ��ӽ����û����ڲ��ƻ����͵�����½���Ķ��ӷ���ǰ��һע
				for(int i = 0;i < bNextCount - 1; i ++)
				{
					if(GetCardLogicValue(bInNextList[i]) == iTemp1 ) 
					{
						for(int j = bFirstCount - 2;j >= 0; j --)
						{
							if(GetCardLogicValue(bInNextList[i]) > GetCardLogicValue(bInFirstList[j]))
							{
								CopyMemory(bFirstCard,bInFirstList,sizeof(BYTE) * bFirstCount);
								CopyMemory(bNextCard,bInNextList,sizeof(BYTE) * bNextCount);

								bFirstCard[j] = bInNextList[i];
								bNextCard[i] = bInFirstList[j];
								bFirstCard[j + 1] = bInNextList[i + 1];
								bNextCard[i + 1] = bInFirstList[j + 1];

								SortCardList(bFirstCard,bFirstCount,enDescend);
								SortCardList(bNextCard,bNextCount,enDescend);
								if(bFirstType == GetCardType(bFirstCard,bFirstCount,cbMaxCard)&&
									bNextType == GetCardType(bNextCard,bNextCount,cbMaxCard))
								{
									CopyMemory(bInFirstList,bFirstCard,sizeof(BYTE) * bFirstCount);
									CopyMemory(bInNextList,bNextCard,sizeof(BYTE) * bNextCount);
									i = -1;
									break;
								}
							}
						}
					}
				}

				return;
			}
		default:
			{
				return ;
			}
		}
	}
	return;
}
/////////////////////////////////////////////////////////////////////////
