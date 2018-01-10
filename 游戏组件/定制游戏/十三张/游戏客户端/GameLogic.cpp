#include "StdAfx.h"
#include "GameLogic.h"

//////////////////////////////////////////////////////////////////////////
//索引变量
const BYTE cbIndexCount=4;


//扑克数据
const BYTE	CGameLogic::m_bCardListData[52]=
{
	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,	//方块 A - K
	0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,0x1B,0x1C,0x1D,	//梅花 A - K
	0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2A,0x2B,0x2C,0x2D,	//红桃 A - K
	0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3A,0x3B,0x3C,0x3D	//黑桃 A - K
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

//获取类型
BYTE CGameLogic::GetCardType(BYTE bCardData[], BYTE bCardCount, BYTE & bMaxCardData)
{
	//数据校验
	ASSERT(bCardCount==3 || bCardCount==5 ) ;
	if(bCardCount!=3 && bCardCount!=5) return CT_INVALID ;

	bMaxCardData = 0;

	//分析扑克
	tagAnalyseData AnalyseData ;
	ZeroMemory(&AnalyseData, sizeof(AnalyseData)) ;
	AnalyseCard(bCardData , bCardCount , AnalyseData);
	SortCardList(bCardData, bCardCount, enDescend);	
	
	//分析扑克
	tagAnalyseResult AnalyseResult;
	ZeroMemory(&AnalyseResult, sizeof(AnalyseResult));
	AnalysebCardData(bCardData, bCardCount, AnalyseResult);

	//开始分析
	switch (bCardCount)
	{
	case 3:	//三条类型
		{
			//单牌类型
			if(3==AnalyseData.bOneCount)
			{
				bMaxCardData = GetCardLogicValue(AnalyseResult.cbCardData[0][0]);
				return CT_SINGLE;
			}

			//对带一张
			if(1==AnalyseData.bTwoCount && 1==AnalyseData.bOneCount)
			{
				bMaxCardData = GetCardLogicValue(AnalyseResult.cbCardData[1][0]);
				return CT_ONE_DOUBLE;
			}

			//三张牌型
			if(1==AnalyseData.bThreeCount)
			{
				bMaxCardData = GetCardLogicValue(AnalyseResult.cbCardData[2][0]);
				return CT_THREE ;
			}
			//错误类型
			return CT_INVALID ;
		}
	case 5:	//五张牌型
		{
			bool bFlushNoA	    = false; 
			bool bFlushFirstA	= false;
			bool bFlushBackA	= false;

			//A连在后
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

			//A连在前
			if(false==bFlushBackA && false==bFlushNoA && 14==GetCardLogicValue(bCardData[0]))
			{
				bFlushFirstA = true;
				for(BYTE i=1 ; i<4 ; ++i) 
					if(1!=GetCardLogicValue(bCardData[i])-GetCardLogicValue(bCardData[i+1])) 
						bFlushFirstA = false ;
				if(2!=GetCardLogicValue(bCardData[4]))
					bFlushFirstA = false ;
			}

			//同花五牌
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
				//杂顺类型
				if(false==AnalyseData.bSameColor) return CT_FIVE_MIXED_FLUSH_NO_A;
				//同花顺牌
				else return CT_FIVE_STRAIGHT_FLUSH;
			}
			else if(true==bFlushFirstA)
			{
				bMaxCardData = GetCardLogicValue(AnalyseResult.cbCardData[0][0]);
				//杂顺类型
				if(false==AnalyseData.bSameColor) return CT_FIVE_MIXED_FLUSH_FIRST_A;
				//同花顺牌
				else							  return CT_FIVE_STRAIGHT_FLUSH_FIRST_A;
			}
			else if(true==bFlushBackA)
			{
				bMaxCardData = GetCardLogicValue(AnalyseResult.cbCardData[0][0]);
				//杂顺类型
				if(false==AnalyseData.bSameColor) return CT_FIVE_MIXED_FLUSH_NO_A;
				//同花顺牌
				else							  return CT_FIVE_STRAIGHT_FLUSH;
			}

			//四带单张
			if(1==AnalyseData.bFourCount && 1==AnalyseData.bOneCount)
			{
				bMaxCardData = GetCardLogicValue(AnalyseResult.cbCardData[3][0]);
				return	 CT_FIVE_FOUR_ONE;
			}

			//三条一对
			if(1==AnalyseData.bThreeCount && 1==AnalyseData.bTwoCount) 
			{
				bMaxCardData = GetCardLogicValue(AnalyseResult.cbCardData[2][0]);
				return	 CT_FIVE_THREE_DEOUBLE;
			}

			//三条带单
			if(1==AnalyseData.bThreeCount && 2==AnalyseData.bOneCount)
			{
				bMaxCardData = GetCardLogicValue(AnalyseResult.cbCardData[2][0]);
				return	 CT_THREE;
			}

			//两对牌型
			if(2==AnalyseData.bTwoCount && 1==AnalyseData.bOneCount)
			{
				bMaxCardData = GetCardLogicValue(AnalyseResult.cbCardData[1][0]);
				return	 CT_TWO_DOUBLE;
			}

			//只有一对
			if(1==AnalyseData.bTwoCount && 3==AnalyseData.bOneCount)
			{
				bMaxCardData = GetCardLogicValue(AnalyseResult.cbCardData[1][0]);
				return	 CT_ONE_DOUBLE;
			}

			//单牌类型
			if(5==AnalyseData.bOneCount && false==AnalyseData.bSameColor)
			{
				bMaxCardData = GetCardLogicValue(AnalyseResult.cbCardData[0][0]);
				return  CT_SINGLE;
			}

			//错误类型
			return CT_INVALID;
		}
	}

	return CT_INVALID;
}

//判断特殊牌型
//必须是13张
BYTE CGameLogic::GetSpecialType(BYTE bHandCardData[], BYTE bCardCount)
{
	ASSERT(bCardCount == HAND_CARD_COUNT);
	if (bCardCount != HAND_CARD_COUNT) return CT_EX_INVALID;

	//排列扑克
	BYTE bCardData[13];
	CopyMemory(bCardData , bHandCardData , sizeof(BYTE)*13);
	//排序扑克
	SortCardList(bCardData, bCardCount, enDescend);

	//设置结果
	tagAnalyseData AnalyseData;
	ZeroMemory(&AnalyseData, sizeof(AnalyseData));

	//变量定义
	BYTE bSameCount = 1,
		bCardValueTemp=0,
		bFirstCardIndex = 0;	//记录下标

	BYTE bLogicValue = GetCardLogicValue(bCardData[0]);
	BYTE bCardColor = GetCardColor(bCardData[0]) ;
	//////////////////////////////////////////////////////////////////////////
	//扑克分析
	for (BYTE i=1;i<bCardCount;i++)
	{
		//获取扑克
		bCardValueTemp=GetCardLogicValue(bCardData[i]);
		if (bCardValueTemp==bLogicValue) bSameCount++;

		//保存结果
		if ((bCardValueTemp!=bLogicValue)||(i==(bCardCount-1)))
		{
			switch (bSameCount)
			{
			case 1:		//一张
				break; 
			case 2:		//两张
				{
					AnalyseData.bTwoFirst[AnalyseData.bTwoCount] = bFirstCardIndex ;
					AnalyseData.bTwoCount++ ;
					break;
				}
			case 3:		//三张
				{
					AnalyseData.bThreeFirst[AnalyseData.bThreeCount] = bFirstCardIndex ;
					AnalyseData.bThreeCount++ ;
					break;
				}
			case 4:		//四张
				{
					AnalyseData.bFourFirst[AnalyseData.bFourCount] = bFirstCardIndex ;
					AnalyseData.bFourCount++ ;
					break;
				}
				break;
			}
		}

		//设置变量
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
	//至尊青龙
	if (IsStraightDragon(bCardData, bCardCount)==true)
		return CT_EX_ZHIZUNQINGLONG;

	//一条龙
	if ((IsLinkCard(bCardData, bCardCount)==true))
		return CT_EX_YITIAOLONG;
	
	//六对半
	if (AnalyseData.bTwoCount==6 && AnalyseData.bOneCount==1)
		return CT_EX_LIUDUIBAN;

	//三顺子
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

	//三同花
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

			//后墩扑克
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

					//中墩扑克
					RemoveCard(SameColorResult.cbResultCard[0], SameColorResult.cbCardCount[0], cbTempData1, cbTempCount);
					cbTempCount -= SameColorResult.cbCardCount[0];

					if (IsSameColorCard(cbTempData1, cbTempCount)) 
						return CT_EX_SANTONGHUA;
				}
			}
		}
	}

	//非特殊牌
	return CT_EX_INVALID;
}

//获取特殊牌
//bSpecCardData[][]
//顺序为前中后
BYTE CGameLogic::GetSpecialCardData(BYTE bHandCardData[HAND_CARD_COUNT], BYTE bHandCardCount, BYTE bSpecCardData[3][5])
{
	ASSERT(bHandCardCount == HAND_CARD_COUNT);
	if (bHandCardCount != HAND_CARD_COUNT) return 0;

	//清空结果
	ZeroMemory(bSpecCardData, sizeof(bSpecCardData));

	//变量定义
	BYTE bCardData[HAND_CARD_COUNT];
	BYTE bCardCount = bHandCardCount;

	//复制扑克
	CopyMemory(bCardData, bHandCardData, sizeof(BYTE)*bHandCardCount);
	//排序扑克
	SortCardList(bCardData, bCardCount, enDescend);

	//设置结果
	tagAnalyseData AnalyseData;
	ZeroMemory(&AnalyseData, sizeof(AnalyseData));

	//变量定义
	BYTE bSameCount = 1,
		bCardValueTemp=0,
		bFirstCardIndex = 0;	//记录下标

	BYTE bLogicValue = GetCardLogicValue(bCardData[0]);
	BYTE bCardColor = GetCardColor(bCardData[0]) ;	
	//扑克分析
	//////////////////////////////////////////////////////////////////////////
	for (BYTE i=1;i<bCardCount;i++)
	{
		//获取扑克
		bCardValueTemp=GetCardLogicValue(bCardData[i]);
		if (bCardValueTemp==bLogicValue) bSameCount++;

		//保存结果
		if ((bCardValueTemp!=bLogicValue)||(i==(bCardCount-1)))
		{
			switch (bSameCount)
			{
			case 1:		//一张
				break; 
			case 2:		//两张
				{
					AnalyseData.bTwoFirst[AnalyseData.bTwoCount] = bFirstCardIndex ;
					AnalyseData.bTwoCount++ ;
					break;
				}
			case 3:		//三张
				{
					AnalyseData.bThreeFirst[AnalyseData.bThreeCount] = bFirstCardIndex ;
					AnalyseData.bThreeCount++ ;
					break;
				}
			case 4:		//四张
				{
					AnalyseData.bFourFirst[AnalyseData.bFourCount] = bFirstCardIndex ;
					AnalyseData.bFourCount++ ;
					break;
				}
				break;
			}
		}

		//设置变量
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
	//至尊青龙
	if (IsStraightDragon(bCardData, bCardCount) == true)
	{	
		CopyMemory(bSpecCardData[2], &bCardData[0], sizeof(BYTE)*5);
		CopyMemory(bSpecCardData[1], &bCardData[5], sizeof(BYTE)*5);
		CopyMemory(bSpecCardData  , &bCardData[10], sizeof(BYTE)*3);
		
		return 1;
	}

	//一条龙
	if (IsLinkCard(bCardData, bCardCount) == true)
	{
		CopyMemory(bSpecCardData[2], &bCardData[0], sizeof(BYTE)*5);
		CopyMemory(bSpecCardData[1], &bCardData[5], sizeof(BYTE)*5);
		CopyMemory(bSpecCardData  , &bCardData[10], sizeof(BYTE)*3);
	
		return 1;
	}

	//六对半 （e.g. AA KK QQ JJ 99 77 2）
	if (AnalyseData.bTwoCount==6 && AnalyseData.bOneCount==1)
	{
		BYTE bTempCardData[13] = {0};
		CopyMemory(bTempCardData, bCardData, sizeof(BYTE)*13);

		BYTE bTempSegCard[5];

		//填充后墩 (AA KK 2)
		ZeroMemory(bTempSegCard, sizeof(bTempSegCard));
		CopyMemory(&bTempSegCard,	 &bTempCardData[AnalyseData.bTwoFirst[0]], sizeof(BYTE)*2);
		CopyMemory(&bTempSegCard[2], &bTempCardData[AnalyseData.bTwoFirst[1]], sizeof(BYTE)*2);
		CopyMemory(&bTempSegCard[4], &bTempCardData[AnalyseData.bOneFirst[0]], sizeof(BYTE)*1);
		CopyMemory(bSpecCardData[2], bTempSegCard, sizeof(BYTE)*5);

		//填充中墩 (QQ JJ 7)
		ZeroMemory(bTempSegCard, sizeof(bTempSegCard));
		CopyMemory(&bTempSegCard,	 &bTempCardData[AnalyseData.bTwoFirst[2]], sizeof(BYTE)*2);
		CopyMemory(&bTempSegCard[2], &bTempCardData[AnalyseData.bTwoFirst[3]], sizeof(BYTE)*2);
		CopyMemory(&bTempSegCard[4], &bTempCardData[AnalyseData.bTwoFirst[5]], sizeof(BYTE)*1);
		CopyMemory(bSpecCardData[1], bTempSegCard, sizeof(BYTE)*5);

		//填充前墩 (99 2)
		ZeroMemory(bTempSegCard, sizeof(bTempSegCard));
		CopyMemory(&bTempSegCard,	 &bTempCardData[AnalyseData.bTwoFirst[4]], sizeof(BYTE)*2);
		CopyMemory(&bTempSegCard[2], &bTempCardData[AnalyseData.bTwoFirst[5]+1], sizeof(BYTE)*1);
		CopyMemory(bSpecCardData[0], bTempSegCard, sizeof(BYTE)*3);

		return 1;
	}

	//三顺子
	tagSearchCardResult LineCardResult;
	BYTE cbLineCardResult5 = SearchLineCardType(bCardData, bCardCount, 5, &LineCardResult);
	if (cbLineCardResult5 >= 2)
	{	
		if (IsLinkCard(LineCardResult.cbResultCard[0], LineCardResult.cbCardCount[0]))
		{		
			BYTE cbTempData[13] = {0};
			BYTE cbTempCount = 13;
			CopyMemory(cbTempData, bCardData, sizeof(bCardData));

			//后墩扑克
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

					//中墩扑克
					CopyMemory(bSpecCardData[1], LineCardResult.cbResultCard[0], sizeof(BYTE)*LineCardResult.cbCardCount[0]);
					RemoveCard(LineCardResult.cbResultCard[0], LineCardResult.cbCardCount[0], cbTempData1, cbTempCount);
					cbTempCount -= LineCardResult.cbCardCount[0];

					if (IsLinkCard(cbTempData1, cbTempCount))
					{
						//前墩扑克
						CopyMemory(bSpecCardData[0], cbTempData1, sizeof(BYTE)*3);

						//防止乌龙
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

	//三同花
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

			//后墩扑克
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

					//中墩扑克
					CopyMemory(bSpecCardData[1], SameColorResult.cbResultCard[0], sizeof(BYTE)*SameColorResult.cbCardCount[0]);
					RemoveCard(SameColorResult.cbResultCard[0], SameColorResult.cbCardCount[0], cbTempData1, cbTempCount);
					cbTempCount -= SameColorResult.cbCardCount[0];

					if (IsSameColorCard(cbTempData1, cbTempCount))
					{
						//前墩扑克
						CopyMemory(bSpecCardData[0], cbTempData1, sizeof(BYTE)*3);
						return 1;
					}
				}
			}
		}
	}

	return 0;
}

//排列扑克
void CGameLogic::SortCardList(BYTE bCardData[], BYTE bCardCount, enSortCardType SortCardType)
{
	ASSERT(bCardCount>=1 && bCardCount<=13) ;
	if(bCardCount<1 || bCardCount>13) return ;

	//转换数值
	BYTE bLogicVolue[13];
	for (BYTE i=0;i<bCardCount;i++)	bLogicVolue[i]=GetCardLogicValue(bCardData[i]);	

	if(enDescend==SortCardType)
	{
		//排序操作
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
					//交换位置
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
		//排序操作
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
					//交换位置
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
		//排序操作
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
					//交换位置
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
	//	MyMsgBox(_T("错误排列类型！")) ;

	return;
}

//混乱扑克
void CGameLogic::RandCardList(BYTE bCardBuffer[], BYTE bBufferCount)
{
	//混乱准备
	BYTE bTotalCount = sizeof(m_bCardListData);
	BYTE bCardData[sizeof(m_bCardListData)];
	CopyMemory(bCardData,m_bCardListData,sizeof(m_bCardListData));

	srand((unsigned)time(NULL));

	//混乱扑克
	BYTE bRandCount=0,bPosition=0;
	do
	{
		bPosition=rand()%(bTotalCount-bRandCount);
		bCardBuffer[bRandCount++]=bCardData[bPosition];
		bCardData[bPosition]=bCardData[bTotalCount-bRandCount];
	} while (bRandCount<bBufferCount);

	return;
}

//删除扑克
bool CGameLogic::RemoveCard(const BYTE bRemoveCard[], BYTE bRemoveCount, BYTE bCardData[], BYTE bCardCount)
{
	//检验数据
	ASSERT(bRemoveCount<=bCardCount);

	//定义变量
	BYTE bDeleteCount=0,bTempCardData[13];
	if (bCardCount>CountArray(bTempCardData)) return false;
	CopyMemory(bTempCardData,bCardData,bCardCount*sizeof(bCardData[0]));

	//置零扑克
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

	//清理扑克
	BYTE bCardPos=0;
	for (BYTE i=0;i<bCardCount;i++)
	{
		if (bTempCardData[i]!=0) bCardData[bCardPos++]=bTempCardData[i];
	}

	return true;
}

/****************************************************
*函数名：AutoPutCard 
*功能：  对一手牌进行自动摆牌，不摆乌龙牌，牌数目暂定一定要13张，另
抽牌逻辑已分开，此函数可随意扩展为对N张牌进行N次自动摆牌 JJ
*参数：	 bCardData			牌数组(13)			IN
bPutCardData		摆好的牌(前中后13)	OUT
*返回值	 无
****************************************************/

void CGameLogic::AutoPutCard(BYTE bCardData[], BYTE bPutCardData[], bool bIsAndroidUser, bool bTest)
{
	BYTE bTempCard[6][3][5]; //保存权值最大的六副牌型
	int  iTempWeights[6][3];  //保存权值最大的六副牌型的权值
	BYTE bCardList[13];      //保存手牌
	BYTE bCard[3][5];        //临时存放牌
	BYTE bSubscript[13];     //下标用来保存每一注使用的哪些牌
	bool blBiaoJi[13];       //标记那些牌没有使用
	int iFWeights,iSWeights,iTWeights;
	BYTE bWCard;

	CopyMemory(bCardList, bCardData, sizeof(BYTE)*13);
	ZeroMemory(bTempCard, sizeof(bTempCard));
	ZeroMemory(iTempWeights,sizeof(iTempWeights));
	ZeroMemory(bCard,sizeof(bCard));
	ZeroMemory(blBiaoJi,sizeof(blBiaoJi));

	//进行全排列寻找最优牌型
	SortCardList(bCardList, 13, enAscend);

	//第一注使用的三张牌
	for(bSubscript[0] = 0;                 bSubscript[0] < 13; bSubscript[0] ++)
	for(bSubscript[1] = bSubscript[0] + 1; bSubscript[1] < 13; bSubscript[1] ++)
	for(bSubscript[2] = bSubscript[1] + 1; bSubscript[2] < 13; bSubscript[2] ++)
	{
		//从大到小将牌保存到bCard[2]数组中，将已经使用的牌进行标记
		for(int i = 0; i < 3; i ++)
		{
			blBiaoJi[bSubscript[i]] = true;
			bCard[2][2 - i] = bCardList[bSubscript[i]];
		}
		//第二注使用的五张牌
		for(bSubscript[3] = 0;                 bSubscript[3] < 13; bSubscript[3] ++)
		if(!blBiaoJi[bSubscript[3]]) for(bSubscript[4] = bSubscript[3] + 1; bSubscript[4] < 13; bSubscript[4] ++)
		if(!blBiaoJi[bSubscript[4]]) for(bSubscript[5] = bSubscript[4] + 1; bSubscript[5] < 13; bSubscript[5] ++)
		if(!blBiaoJi[bSubscript[5]]) for(bSubscript[6] = bSubscript[5] + 1; bSubscript[6] < 13; bSubscript[6] ++)
		if(!blBiaoJi[bSubscript[6]]) for(bSubscript[7] = bSubscript[6] + 1; bSubscript[7] < 13; bSubscript[7] ++)
		if(!blBiaoJi[bSubscript[7]]){
		do{

			//从大到小将牌保存到bCard[1]数组中，将已经使用的牌进行标记
			for(int i = 0 ; i < 5 ; i ++)
			{
				blBiaoJi[bSubscript[3 + i]] = true;
				bCard[1][4 - i] = bCardList[bSubscript[3 + i]];
			}

			iFWeights = CardTypeToWeights(GetCardType(bCard[2],3,bWCard), 1);
			iSWeights = CardTypeToWeights(GetCardType(bCard[1],5,bWCard), 2);

			//比较第一注和第二注牌型权值的大小
			if(iFWeights > iSWeights)
			{
				break;
			}

			//将剩下元素放入bCard[0]数组当中
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
				//已经存在三注牌型都不小于它的组合，就放弃
				if(iFWeights <= iTempWeights[i][2] && iSWeights <= iTempWeights[i][1] && iTWeights <= iTempWeights[i][0])
				{
					bHave = true;
					break;
				}
			}

			if(false == bHave)
			{

				//不存在就保存
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

		//清除标记
		for(i = 0; i < 3; i ++)
		{
			blBiaoJi[bSubscript[i]] = false;
		}	
	}
	//将权值没法比上一种牌型组合优的去掉
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

	//将中间出现的空隙删除
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

	//按权值排序
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

	//随机选择一副权值大的牌打出
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
	
	////随机选择一副牌
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
	
	//牌组填充
	CopyMemory(bPutCardData, bTempCard[iCnt][2], sizeof(BYTE)*3);
	CopyMemory(bPutCardData+3, bTempCard[iCnt][1], sizeof(BYTE)*5);
	CopyMemory(bPutCardData+8, bTempCard[iCnt][0], sizeof(BYTE)*5);

	//牌组优化
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
//	//定义变量
//	BYTE bCardList[13] = {0};
//	CopyMemory(bCardList, bCardData, sizeof(BYTE)*13);
//	ZeroMemory(bPutCardData, sizeof(BYTE)*13);
//	BYTE bLeftCCount = 13;
//	BYTE bTempCard[3][5] = {0};
//	BYTE bTempCCount[3] = {0};
//	BYTE bSigCCount = 0;		//用于散牌填充时的计数
//	BYTE bCardType[3] = {0};	
//
//
//	//摆牌开始
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
//		//牌组填充
//		CopyMemory(bPutCardData, bTempCard[2], sizeof(BYTE)*3);
//		CopyMemory(bPutCardData+3, bTempCard[1], sizeof(BYTE)*5);
//		CopyMemory(bPutCardData+8, bTempCard[0], sizeof(BYTE)*5);
//
//		return;
//	}
//
//	SortCardList(bCardList, bLeftCCount, enDescend);
//
//	//散牌填充
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
//	//牌组填充
//	CopyMemory(bPutCardData, bTempCard[2], sizeof(BYTE)*3);
//	CopyMemory(bPutCardData+3, bTempCard[1], sizeof(BYTE)*5);
//	CopyMemory(bPutCardData+8, bTempCard[0], sizeof(BYTE)*5);
//
//	return;
//}

//逻辑数值
BYTE CGameLogic::GetCardLogicValue(BYTE bCardData)
{
	//扑克属性
	BYTE bCardValue=GetCardValue(bCardData); 

	//转换数值
	return (bCardValue==1)?(bCardValue+13):bCardValue; 
}

//比较扑克
int CGameLogic::CompareCard(BYTE bInFirstList[], BYTE bInNextList[], BYTE bFirstCount, BYTE bNextCount , bool bComperWithOther)
{
	//定义变量
	BYTE bFirstList[13], bNextList[13] ;
	tagAnalyseData FirstAnalyseData , NextAnalyseData;
	ZeroMemory(&FirstAnalyseData, sizeof(FirstAnalyseData)) ;
	ZeroMemory(&NextAnalyseData, sizeof(NextAnalyseData)) ;

	//检查转化
	tagTransData ttdFst;
	tagTransData ttdNxt;
	AnalyseMaxTransform(bInFirstList, bFirstCount, bFirstList, ttdFst);
	AnalyseMaxTransform(bInNextList, bNextCount, bNextList, ttdNxt);

	//排序牌组
	SortCardList(bFirstList , bFirstCount , enDescend) ;
	SortCardList(bNextList , bNextCount , enDescend) ;

	//分析牌组
	AnalyseCard(bFirstList , bFirstCount , FirstAnalyseData) ;	
	AnalyseCard(bNextList  , bNextCount  , NextAnalyseData) ;	

	//数据验证
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

	//获取类型
	BYTE cbMaxCard = 0;
	BYTE bNextType=GetCardType(bNextList,bNextCount,cbMaxCard);
	BYTE bFirstType=GetCardType(bFirstList,bFirstCount,cbMaxCard);

	ASSERT(CT_INVALID!=bNextType && CT_INVALID!=bFirstType) ;
	if(CT_INVALID==bFirstType || CT_INVALID==bNextType) return enCRError ;

	//同段比较
	if(true==bComperWithOther)
	{
		//开始对比
		if(bNextType==bFirstType) 
		{
			switch(bFirstType) 
			{
			case CT_SINGLE:				//单牌类型
				{
					//数据验证
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
			case CT_ONE_DOUBLE:			//对带一张
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
			case CT_TWO_DOUBLE:	//两对牌型
				{
					//数据验证
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
			case CT_THREE:						//三张牌型
				{
					if (GetCardLogicValue(bNextList[NextAnalyseData.bThreeFirst[0]]) > GetCardLogicValue(bFirstList[FirstAnalyseData.bThreeFirst[0]]))
						return enCRGreater;
					else if (GetCardLogicValue(bNextList[NextAnalyseData.bThreeFirst[0]]) < GetCardLogicValue(bFirstList[FirstAnalyseData.bThreeFirst[0]]))
						return enCRLess;
					else
						return enCREqual;
				}
			case CT_FIVE_THREE_DEOUBLE:			//三条一对
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
			case CT_FIVE_MIXED_FLUSH_FIRST_A:	//A在前顺子
			case CT_FIVE_MIXED_FLUSH_BACK_A:	//A在后顺子
				{
					//比较最大的一张牌
					return enCREqual;
				}
			case CT_FIVE_MIXED_FLUSH_NO_A:		//没A杂顺
			case CT_FIVE_FLUSH:					//同花五牌
				{
					//比较数值
					for(BYTE i=0; i<5; ++i)
					{
						if (GetCardLogicValue(bNextList[i]) > GetCardLogicValue(bFirstList[i]))
							return enCRGreater;
						else if (GetCardLogicValue(bNextList[i]) < GetCardLogicValue(bFirstList[i]))
							return enCRLess;
					}

					//比较花色
					for(BYTE i = 0 ; i < 5; i++)
					{
						if (GetCardColor(bNextList[i]) > GetCardColor(bFirstList[i]))
							return enCRGreater;
						else if (GetCardColor(bNextList[i]) < GetCardColor(bFirstList[i]))
							return enCRLess;
					}
				}
			case CT_FIVE_FOUR_ONE:				//四带一张
				{
					if (GetCardLogicValue(bNextList[NextAnalyseData.bFourFirst[0]]) > GetCardLogicValue(bFirstList[FirstAnalyseData.bFourFirst[0]]))
						return enCRGreater;
					else
						return enCRLess;
				}
			case CT_FIVE_STRAIGHT_FLUSH_FIRST_A://A在前同花顺
				{
					//数据验证
					ASSERT(bNextList[0]!=bFirstList[0]) ;
					if(bNextList[0]==bFirstList[0]) return enCRError ;

					//比较数值
					for(BYTE i=0; i<5; ++i)
					{
						if (GetCardLogicValue(bNextList[i]) > GetCardLogicValue(bFirstList[i]))
							return enCRGreater;
						else if (GetCardLogicValue(bNextList[i]) < GetCardLogicValue(bFirstList[i]))
							return enCRLess;
					}

					//比较花色
					for(BYTE i = 0 ; i < 5; i++)
					{
						if (GetCardColor(bNextList[i]) > GetCardColor(bFirstList[i]))
							return enCRGreater;
						else if (GetCardColor(bNextList[i]) < GetCardColor(bFirstList[i]))
							return enCRLess;
					}

				}
			case CT_FIVE_STRAIGHT_FLUSH:		//同花顺牌
				{
					//比较数值
					for(BYTE i=0; i<5; ++i)
					{
						if (GetCardLogicValue(bNextList[i]) > GetCardLogicValue(bFirstList[i]))
							return enCRGreater;
						else if (GetCardLogicValue(bNextList[i]) < GetCardLogicValue(bFirstList[i]))
							return enCRLess;
					}

					//比较花色
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
		//开始对比
		if(bNextType==bFirstType) 
		{
			switch(bFirstType)
			{
			case CT_SINGLE:				//单牌类型
				{
					//数据验证
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
			case CT_ONE_DOUBLE:			//对带一张
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
			case CT_THREE:				//三张牌型
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

//是否顺子
bool CGameLogic::IsLinkCard(const BYTE cbCardData[], BYTE cbCardCount)
{
	ASSERT(cbCardCount > 0);
	if (cbCardCount <= 0) return false;

	bool bRet = true;
	BYTE cbCardBuffer[13] = {0};
	CopyMemory(cbCardBuffer, cbCardData, sizeof(BYTE)*cbCardCount);
	
	//降序排列
	SortCardList(cbCardBuffer, cbCardCount, enDescend);

	BYTE cbFirstCard=GetCardLogicValue(cbCardBuffer[0]);
	for (WORD i=1; i<cbCardCount; i++)
	{
		BYTE cbNextCard=GetCardLogicValue(cbCardBuffer[i]);
		if (cbFirstCard != cbNextCard+i) bRet=false;
	}

	return bRet;
}

//是否同花
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

//是否同花顺
bool CGameLogic::IsStraightDragon(const BYTE cbCardData[], BYTE bCardCount)
{
	//校验数据
	ASSERT(bCardCount>0 && bCardCount<=13);
	if (bCardCount<=0 || bCardCount>13)	return false;

	bool b1 = IsLinkCard(cbCardData, bCardCount);
	bool b2 = IsSameColorCard(cbCardData, bCardCount);
	if (b1==false || b2==false)
		return false;

	return true;
}

//获取对数
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

//分析扑克
void CGameLogic::AnalyseCard(const BYTE bCardDataList[] , const BYTE bCardCount , tagAnalyseData& AnalyseData) 
{
	ASSERT(3==bCardCount || 5==bCardCount) ;

	//排列扑克
	BYTE bCardData[13] ;
	CopyMemory(bCardData , bCardDataList , bCardCount) ;
	SortCardList(bCardData , bCardCount , enDescend) ;

	//变量定义
	BYTE bSameCount = 1 ,
		bCardValueTemp=0,
		bSameColorCount = 1 ,
		bFirstCardIndex = 0 ;	//记录下标

	BYTE bLogicValue = GetCardLogicValue(bCardData[0]);
	BYTE bCardColor = GetCardColor(bCardData[0]) ;

	//设置结果
	ZeroMemory(&AnalyseData, sizeof(AnalyseData));

	//扑克分析
	for (BYTE i=1;i<bCardCount;i++)
	{
		//获取扑克
		bCardValueTemp=GetCardLogicValue(bCardData[i]);
		if (bCardValueTemp==bLogicValue) bSameCount++;

		//保存结果
		if ((bCardValueTemp!=bLogicValue)||(i==(bCardCount-1))||bCardData[i]==0)
		{
			switch (bSameCount)
			{
			case 1:		//一张
				break; 
			case 2:		//两张
				{
					AnalyseData.bTwoFirst[AnalyseData.bTwoCount]	 = bFirstCardIndex ;
					AnalyseData.bTwoCount++ ;
					break;
				}
			case 3:		//三张
				{
					AnalyseData.bThreeFirst[AnalyseData.bThreeCount] = bFirstCardIndex ;
					AnalyseData.bThreeCount++ ;
					break;
				}
			case 4:		//四张
				{
					AnalyseData.bFourFirst[AnalyseData.bFourCount]   = bFirstCardIndex ;
					AnalyseData.bFourCount++ ;
					break;
				}
			default:
				//MyMsgBox(_T("AnalyseCard：错误扑克！: %d") , bSameCount) ;
				break;
			}
		}

		//王牌自动转化同花
		if (bCardData[i] == 0)	
		{
			bSameColorCount += bCardCount-i;
			break;
		}

		//设置变量
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

	//是否同花
	AnalyseData.bSameColor = (5==bSameColorCount) ? true : false ;

	return;
}

//分析扑克
void CGameLogic::AnalysebCardData(const BYTE cbCardData[], BYTE cbCardCount, tagAnalyseResult & AnalyseResult)
{
	//设置结果
	ZeroMemory(&AnalyseResult,sizeof(AnalyseResult));

	//扑克分析
	for (BYTE i=0;i<cbCardCount;i++)
	{
		//变量定义
		BYTE cbSameCount=1;
		BYTE cbLogicValue=GetCardLogicValue(cbCardData[i]);

		//搜索同牌
		for (BYTE j=i+1;j<cbCardCount;j++)
		{
			//获取扑克
			if (GetCardLogicValue(cbCardData[j])!=cbLogicValue) break;

			//设置变量
			cbSameCount++;
		}

		//设置结果
		BYTE cbIndex = AnalyseResult.cbBlockCount[cbSameCount-1]++;
		for (BYTE j = 0; j < cbSameCount; j++) 
			AnalyseResult.cbCardData[cbSameCount-1][cbIndex*cbSameCount+j]=cbCardData[i+j];

		//设置索引
		i += (cbSameCount-1);
	}

	return;
}

//分析分布
VOID CGameLogic::AnalysebDistributing(const BYTE cbCardData[], BYTE cbCardCount, tagDistributing & Distributing)
{
	//设置变量
	ZeroMemory(&Distributing,sizeof(Distributing));

	//设置变量
	for (BYTE i=0;i<cbCardCount;i++)
	{
		if (cbCardData[i]==0) continue;

		//获取属性
		BYTE cbCardColor=GetCardColor(cbCardData[i]);
		BYTE cbCardValue=GetCardValue(cbCardData[i]);

		//分布信息
		Distributing.cbCardCount++;
		Distributing.cbDistributing[cbCardValue-1][cbIndexCount]++;
		Distributing.cbDistributing[cbCardValue-1][cbCardColor]++;
	}

	return;
}


/****************************************************
*函数名：CompareOneCardEx 
*功能：  单张比较，先比牌值，牌值相同则比较花色 (方<梅<红<黑)
*参数：	 
bFirstCard			牌一			IN
bNextCard			牌二			IN
*返回值：
true				后大于前
false				前大于后
****************************************************/
bool CGameLogic::CompareOneCardEx(BYTE bFirstCard, BYTE bNextCard, tagTransData & ttdFst, tagTransData & ttdNxt)
{
	//牌值比较
	if (GetCardLogicValue(bFirstCard) != GetCardLogicValue(bNextCard))
		return GetCardLogicValue(bFirstCard) < GetCardLogicValue(bNextCard);

	//转化比较	(由王转化来的牌比普通同值牌大，大王转化来的大于小王转化来的)
	bool bFromTransFst = (ttdFst.transList.Find(bFirstCard)!=NULL);
	bool bFromTransNxt = (ttdNxt.transList.Find(bNextCard)!=NULL);
	if (bFromTransFst != bFromTransNxt)
	{
		return !bFromTransFst;	
	}
	else
	{
		if (bFromTransFst)		//比较大小王
		{
			ASSERT((ttdFst.bHaveKing==0) == (ttdNxt.bHaveKing==0));
			return (ttdFst.bHaveKing<ttdNxt.bHaveKing);
		}
		else					//比较花色
		{
			return ( GetCardColor(bFirstCard)<GetCardColor(bNextCard) );
		}
	}

	return false;
}

/****************************************************
*函数名：GetMaxCardData 
*功能：  从得定牌组中抽取出最大牌型的牌	JJ
*参数：	 
cbCardData			原牌(3< <=13)		IN
cbCardCount			原牌数目				IN
cbMaxCardData		取出的最大牌(<=5)	OUT
bMaxCardCount		取出牌数目(1<= <=5)  OUT
*返回值：最大类型 (用于单元测试做校验,实际无用,不能做为可靠数据)
****************************************************/
BYTE CGameLogic::GetMaxCardData(const BYTE cbCardData[], const BYTE cbCardCount, BYTE bMaxCardData[], BYTE & bMaxCardCount, BYTE bNeedCCount)
{
	//校验数据
	ASSERT(cbCardCount <= 13 || cbCardCount > 3);

	//定义变量
	BYTE bKCount = 0;
	CList<BYTE> evCardList[15];	//0位存王牌,1位保留,其他位按逻辑值存放
	CList<BYTE> evColorList[4];	//方梅红黑
	BYTE bCardArray[13] = {0};
	CopyMemory(bCardArray, cbCardData, sizeof(BYTE)*cbCardCount);

	SortCardList(bCardArray, cbCardCount, enDescend);

	//分析扑克
	for (int i = 0; i < cbCardCount; i++)
	{
		//保存王牌
		if (bCardArray[i] == 0x41 || bCardArray[i] == 0x42)
		{
			evCardList[0].AddTail(bCardArray[i]);
			continue;
		}

		//保存其他
		BYTE bLogicNum = GetCardLogicValue(bCardArray[i]);
		BYTE bColor = GetCardColor(bCardArray[i]);

		ASSERT(bLogicNum>1 && bLogicNum<15 && bColor>=0 && bColor<=3);
		ASSERT(evCardList[bLogicNum].Find(bCardArray[i]) == NULL);

		evCardList[bLogicNum].AddTail(bCardArray[i]);
		evColorList[bColor].AddTail(bCardArray[i]);
	}

	ASSERT(evCardList[0].GetCount() <= 2);

	//寻找同花顺
	if (bNeedCCount == 5)
	{
		for (int i = 0; i < 4; i++)
		{
			if (evColorList[i].GetCount()+evCardList[0].GetCount() >= 5)	//同花+王牌数大于等于5
			{
				BYTE bCount = 0;
				if (evCardList[0].GetCount() >= 0 && evColorList[i].GetCount() >= 5)		//不带王
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
					if (GetCardValue(evColorList[i].GetHead()) == 1 &&						//检查A2345顺
						GetCardValue(evColorList[i].GetAt(evColorList[i].FindIndex(evColorList[i].GetCount()-4))) == 5)	
					{
						bMaxCardData[0] = evColorList[i].GetHead();
						for (int k = 1; k < 5; k++)
							bMaxCardData[k] = evColorList[i].GetAt(evColorList[i].FindIndex(evColorList[i].GetCount()-k));
						bMaxCardCount = 5;
						return CT_FIVE_STRAIGHT_FLUSH_FIRST_A;
					}
				}
				if (evCardList[0].GetCount() >= 1 && evColorList[i].GetCount() >= 4)		//带单王
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
					if (GetCardValue(evColorList[i].GetHead()) == 1 &&						//检查A2345顺
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
				if (evCardList[0].GetCount() == 2 && evColorList[i].GetCount() >= 3)		//带双王
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
					if (GetCardValue(evColorList[i].GetHead()) == 1 &&						//检查A2345顺
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

	//寻找铁支
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

	//寻找葫芦
	if (bNeedCCount == 5)
	{
		for (int i = 14; i > 1; i--)
		{
			if (evCardList[i].GetCount() + evCardList[0].GetCount() == 3)
			{
				ASSERT(evCardList[i].GetCount()<=3&&evCardList[i].GetCount()>=1);
				//寻找一对
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

	//寻找同花
	if (bNeedCCount == 5)
	{
		BYTE bPossibleCard[4][5] = {0};	//各个能组成同花的牌组
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

	//寻找顺子
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

	//寻找三条
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

	//寻找两对
	if (bNeedCCount == 5)
	{
		for (int i = 14; i > 1; i--)
		{
			if (evCardList[i].GetCount() + evCardList[0].GetCount() == 2)
			{
				ASSERT(evCardList[i].GetCount()<=2&&evCardList[i].GetCount()>=0);

				//寻找一对
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

	//寻找对子
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

	//寻找散牌
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
*函数名：AnalyseMaxTransform 
*功能：  根据牌型和王牌数目对牌进行最大转换 JJ
*参数：	 cbCardData			牌数组			IN
		 cbCardCount		牌数目			IN
		 bTransCardData		转化后牌数组	OUT
		 TransData			转化信息		OUT
*返回值：0					不转化
		 other				转化后的牌型
****************************************************/
BYTE CGameLogic::AnalyseMaxTransform(const BYTE cbCardData[], const BYTE cbCardCount, BYTE bTransCardData[], tagTransData & TransData)
{
	ASSERT(cbCardCount==3 || cbCardCount==5);

	//变量声明
	BYTE bTempCardData[5] = {0};		//保存清空王后的数组
	BYTE bKcount = 0;					//王数目

	//初始化
	memcpy(bTransCardData, cbCardData, sizeof(BYTE)*cbCardCount);
	memcpy(bTempCardData, cbCardData, sizeof(BYTE)*cbCardCount);

	//将王牌置0并统计	
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

	//无王返回
	if (bKcount == 0)
		return 0;

	//有王则分析牌数组
	SortCardList(bTempCardData , cbCardCount , enDescend) ;
	tagAnalyseData	tad = {0};
	AnalyseCard(bTempCardData, cbCardCount, tad);

	//炸弹直接返回
	/*if (tad.bFourCount == 1)
	return 0;*/

	//潜质判断	(从大到小依次计算)

	//同花顺 (5张牌 且 同花 且 全单牌)
	if ( 5 == cbCardCount && tad.bSameColor &&	0 == (tad.bTwoCount+tad.bThreeCount+tad.bFourCount) )				 	
	{
		//A当1用，有A 且 第二大数字小于等于5, 最小顺子A2345
		if (14 == GetCardLogicValue(bTempCardData[0]) && 5 >= GetCardLogicValue(bTempCardData[1]))		
		{
			TransformCard(bTempCardData, cbCardCount, bKcount, CT_FIVE_STRAIGHT_FLUSH_FIRST_A, tad, bTransCardData, TransData);
			return CT_FIVE_STRAIGHT_FLUSH_FIRST_A;
		}

		//最大牌减去最小牌小于等于4
		if ( 4 >= GetCardLogicValue(bTempCardData[0]) - GetCardLogicValue(bTempCardData[5-bKcount-1]) )
		{
			TransformCard(bTempCardData, cbCardCount, bKcount, CT_FIVE_STRAIGHT_FLUSH, tad, bTransCardData, TransData);
			return CT_FIVE_STRAIGHT_FLUSH;
		}
	}

	//炸弹	(5张牌 且 单王3张数目等于一或者双王2张数目或者3张数目等于一)
	if ( 5 == cbCardCount &&  ((1==bKcount&&1==tad.bThreeCount) || (2==bKcount&&(1==tad.bThreeCount||1==tad.bTwoCount))) )
	{
		TransformCard(bTempCardData, cbCardCount, bKcount, CT_FIVE_FOUR_ONE, tad, bTransCardData, TransData);
		return CT_FIVE_FOUR_ONE;
	}

	//葫芦  (5张牌 且 单王2张数目等于2）
	if ( 5 == cbCardCount && 1 == bKcount &&2 == tad.bTwoCount )
	{
		TransformCard(bTempCardData, cbCardCount, bKcount, CT_FIVE_THREE_DEOUBLE, tad, bTransCardData, TransData);
		return CT_FIVE_THREE_DEOUBLE;
	}

	//同花  (5张牌 且 全部同花）
	if (5 == cbCardCount && tad.bSameColor)
	{	
		TransformCard(bTempCardData, cbCardCount, bKcount, CT_FIVE_FLUSH, tad, bTransCardData, TransData);
		return CT_FIVE_FLUSH;
	}

	//顺子	(5张牌 且 非同花 且 全单牌)
	if ( 5 == cbCardCount && !tad.bSameColor &&	0 == (tad.bTwoCount+tad.bThreeCount+tad.bFourCount) )				 	
	{	
		//A当1用，有A 且 第二大数字小于等于5, 最小顺子A2345
		if (14 == GetCardLogicValue(bTempCardData[0]) && 5 >= GetCardLogicValue(bTempCardData[1]))		
		{
			TransformCard(bTempCardData, cbCardCount, bKcount, CT_FIVE_MIXED_FLUSH_FIRST_A, tad, bTransCardData, TransData);
			return CT_FIVE_MIXED_FLUSH_FIRST_A;
		}

		//最大牌减去最小牌小于等于4
		if ( 4 >= GetCardLogicValue(bTempCardData[0]) - GetCardLogicValue(bTempCardData[5-bKcount-1]) )
		{
			TransformCard(bTempCardData, cbCardCount, bKcount, CT_FIVE_MIXED_FLUSH_NO_A, tad, bTransCardData, TransData);
			return CT_FIVE_MIXED_FLUSH_NO_A;
		}
	}

	//三条  (3或5张牌 且 单王2张数目等于1或双王全单牌)
	if ( (1 == bKcount && 1 == tad.bTwoCount) || (2 == bKcount && 0 == (tad.bTwoCount+tad.bThreeCount+tad.bFourCount)) )
	{
		TransformCard(bTempCardData, cbCardCount, bKcount, CT_THREE, tad, bTransCardData, TransData);
		return CT_THREE;
	}

	//两对  (不存在)

	//一对  (3或5张牌 且单王全单牌)
	if (1 == bKcount && 0 == (tad.bTwoCount+tad.bThreeCount+tad.bFourCount))
	{
		TransformCard(bTempCardData, cbCardCount, bKcount, CT_ONE_DOUBLE, tad, bTransCardData, TransData);
		return CT_ONE_DOUBLE;
	}

	//这里正常来说是不可能到的，到了只能说有Bug。。。
	//MyMsgBox(_T("%d %d [%s %s %s %s %s]"), cbCardCount, bKcount, bTempCardData[0]
	//, bTempCardData[1], bTempCardData[2], bTempCardData[3], bTempCardData[4]);
	return CT_SINGLE;
}

/****************************************************
*函数名：TransformCard 
*功能：  用于对已确定潜质牌型的牌进行转换,只能由AnalyseMaxTransform函数使用 JJ
*参数：	 cbNkCardData		除去王的牌组	IN
		 cbCardCount		牌数目(3or5)			IN
		 bKCount			王数目(1or2)			IN
		 bCardType			牌潜质类型(即转化方向)  IN
		 tad				牌型信息				IN
		 bTransCardData		转化后牌数组(升序排列)	OUT
		 TransData			转化信息				INOUT
*返回值：无(本函数不检查数据，数据检查由本函数唯一调用
			者AnalyseMaxTransform进行)
****************************************************/
void CGameLogic::TransformCard(const BYTE cbNkCardData[], BYTE bCardCount, BYTE bKCount, BYTE bCardType, const tagAnalyseData & tad,
							   BYTE bTransCardData[], tagTransData & TransData)
{
	ASSERT( (3==bCardCount||5==bCardCount) && (1==bKCount||2==bKCount) );

	//变量定义
	CList<BYTE>	cardList;			//记录转化后牌数组		
	BYTE bTempCardData[5] = {0};

	//初始化
	memcpy(bTempCardData, cbNkCardData, sizeof(BYTE)*(bCardCount-bKCount));
	for (WORD i = 0; i < bCardCount - bKCount; i++)
	{
		cardList.AddTail(bTempCardData[i]);
	}

	//转化开始
	switch (bCardType)
	{
	//顺子的转化算法是从非王最小牌开始,往上增直到非王最大牌发现有空缺先填空缺，没有空缺则根据是否到终点填充两头
	case CT_FIVE_STRAIGHT_FLUSH_FIRST_A:
	case CT_FIVE_STRAIGHT_FLUSH:
	case CT_FIVE_MIXED_FLUSH_FIRST_A:
	case CT_FIVE_MIXED_FLUSH_NO_A:
		{	
			//数据校验
			ASSERT(5==bCardCount);

			//升序排列			(仅顺子使用升序排列)
			SortCardList(bTempCardData, 5-bKCount, enAscend);

			//清空链表
			cardList.RemoveAll();

			//定义变量
			BYTE bLogicHeadCard = 0;		//最小牌的逻辑值
			BYTE bTempCount = 0;			//转化进行到的位置
			BYTE bCardColor = GetCardColor(bTempCardData[0]);	

			//填充首部
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

			//填充剩余
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

			//剩余王牌
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
	//炸弹的转化算法是直接找到3张的或者2张的，王变成同值的黑桃牌
	case CT_FIVE_FOUR_ONE:
		{
			//数据校验
			ASSERT(5==bCardCount);

			//王牌转化
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
	//葫芦的转化算法是直接找到2对2张中大的，王变成同值的黑桃牌
	case CT_FIVE_THREE_DEOUBLE:
		{
			//数据校验
			ASSERT(5==bCardCount&&bKCount==1);

			//王牌转化
			BYTE transCard = 0x30 + GetCardValue(bTempCardData[tad.bTwoFirst[0]]);
			cardList.AddTail(transCard);
			TransData.transList.AddTail(transCard);
			break;
		} 
	//同花的转化算法是王变成同花
	case CT_FIVE_FLUSH:
		{
			//数据校验
			ASSERT(5==bCardCount);

			BYTE bCardColor = GetCardColor(bTempCardData[0]);

			//王牌转化
			while (cardList.GetCount() < 5)
			{
				BYTE transCard = (bCardColor<<4) + 0x01;
				cardList.AddTail(transCard);
				TransData.transList.AddTail(transCard);
			}
			break;
		} 
	//三条的转化算法是直接找到2张的或者单牌最大的，王变成同值的黑桃牌
	case CT_THREE:
		{
			//王牌转化
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
	//一对的转化算法是直接找到单牌最大的，王变成同值的黑桃牌
	case CT_ONE_DOUBLE:
		{
			//数据校验
			ASSERT(1==bKCount);

			//王牌转化
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
	//填充信息
	ASSERT(cardList.GetCount() == 5 || cardList.GetCount() == 3);
	for (int i = 0; i < bCardCount; i++)
	{
		bTransCardData[i] = cardList.GetAt(cardList.FindIndex(i));
	}

	return;	
}

//同牌搜索
BYTE CGameLogic::SearchSameCard( const BYTE cbHandCardData[], BYTE cbHandCardCount, BYTE cbSameCardCount, tagSearchCardResult *pSearchCardResult )
{
	//设置结果
	if( pSearchCardResult )
		ZeroMemory(pSearchCardResult,sizeof(tagSearchCardResult));
	BYTE cbResultCount = 0;

	//构造扑克
	BYTE cbCardData[13];
	BYTE cbCardCount=cbHandCardCount;
	CopyMemory(cbCardData,cbHandCardData,sizeof(BYTE)*cbHandCardCount);

	//排列扑克
	SortCardList(cbCardData, cbCardCount, enAscend);

	//分析扑克
	tagAnalyseResult AnalyseResult = {};
	AnalysebCardData( cbCardData,cbCardCount,AnalyseResult);

	BYTE cbBlockIndex = cbSameCardCount-1;
	do
	{
		for( BYTE i = 0; i < AnalyseResult.cbBlockCount[cbBlockIndex]; i++ )
		{
			BYTE cbIndex = (AnalyseResult.cbBlockCount[cbBlockIndex]-i-1)*(cbBlockIndex+1);

			//复制扑克
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
	//设置结果
	if( pSearchCardResult )
		ZeroMemory(pSearchCardResult,sizeof(tagSearchCardResult));
	BYTE cbResultCount = 0;

	//效验
	ASSERT( cbSameCount == 3 || cbSameCount == 4 );
	ASSERT( cbTakeCardCount == 2 || cbTakeCardCount == 1 );
	if( cbSameCount != 3 && cbSameCount != 4 )
		return cbResultCount;
	if( cbTakeCardCount != 2 && cbTakeCardCount != 1 )
		return cbResultCount;
	if(cbHandCardCount<cbSameCount+cbTakeCardCount)
		return cbResultCount;

	//搜索同牌
	tagSearchCardResult SameCardResult;
	ZeroMemory(&SameCardResult, sizeof(SameCardResult));
	BYTE cbSameCardResultCount = SearchSameCard( cbHandCardData,cbHandCardCount,cbSameCount,&SameCardResult);

	if( cbSameCardResultCount > 0)
	{
		//搜索带牌
		tagSearchCardResult TakeCardResult;
		ZeroMemory(&TakeCardResult, sizeof(TakeCardResult));
		BYTE cbTakeCardResultCount = SearchSameCard(cbHandCardData,cbHandCardCount,cbTakeCardCount,&TakeCardResult);

		//可以组成带牌
		if (cbTakeCardResultCount > 0)
		{
			for (BYTE i=0; i<cbSameCardResultCount; i++)
			{
				for (BYTE j=0; j<cbTakeCardResultCount; j++)
				{
					//搜索三条： AAA
					//搜索一对：AA 33 66 99
					//忽略组合：AAAAA、
					//保留组合：AAA33，AAA66，AAA99
					if (GetCardLogicValue(SameCardResult.cbResultCard[i][0]) == GetCardLogicValue(TakeCardResult.cbResultCard[j][0]))
						continue;

					//复制扑克
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


//搜索同花
BYTE CGameLogic::SearchSameColorType(const BYTE cbHandCardData[], BYTE cbHandCardCount, BYTE cbSameCount,  tagSearchCardResult *pSearchCardResult)
{
	//设置结果
	if( pSearchCardResult )
		ZeroMemory(pSearchCardResult,sizeof(tagSearchCardResult));
	BYTE cbResultCount = 0;
	
	//复制扑克
	BYTE cbCardData[13] = {0};
	CopyMemory(cbCardData, cbHandCardData, sizeof(BYTE)*cbHandCardCount);
	SortCardList(cbCardData, cbHandCardCount, enDescend);

	//同花变量
	BYTE cbSameCardCount[4] = {0};
	BYTE cbSameCardData[4][13] = {0};

	//统计同花
	for (BYTE i = 0; i < cbHandCardCount; i++)
	{
		//获取牌色
		BYTE cbCardColor = GetCardColor(cbCardData[i]);

		//原牌数目
		BYTE cbCount = cbSameCardCount[cbCardColor];

		//追加扑克
		cbSameCardData[cbCardColor][cbCount] = cbCardData[i];
		cbSameCardCount[cbCardColor]++;
	}

	//判断是否满cbSameCount
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
	//设置结果
	if( pSearchCardResult )
		ZeroMemory(pSearchCardResult,sizeof(tagSearchCardResult));

	//定义变量
	BYTE cbResultCount = 0;
	BYTE cbBlockCount = 1;

	//超过A
	if (cbLineCount > 14) return cbResultCount;

	//长度判断
	if (cbHandCardCount < cbLineCount) return cbResultCount;

	//构造扑克
	BYTE cbCardData[HAND_CARD_COUNT];
	BYTE cbCardCount=cbHandCardCount;
	CopyMemory(cbCardData,cbHandCardData,sizeof(BYTE)*cbHandCardCount);

	//排列扑克
	SortCardList(cbCardData,cbCardCount, enDescend);

	//分析扑克
	tagDistributing Distributing;
	ZeroMemory(&Distributing, sizeof(Distributing));
	AnalysebDistributing(cbCardData,cbCardCount,Distributing);

	//搜索顺子
	BYTE cbTmpLinkCount = 0;
	for (BYTE cbValueIndex=0;cbValueIndex<13;cbValueIndex++)
	{
		//继续判断
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

			//寻找最长连
			if (cbLineCount == 0) continue;
		}

		if( cbTmpLinkCount >= cbLineCount )
		{
			if( pSearchCardResult == NULL ) return 1;

			ASSERT( cbResultCount < CountArray(pSearchCardResult->cbCardCount) );

			//复制扑克
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

			//设置变量
			pSearchCardResult->cbCardCount[cbResultCount] = cbCount;
			cbResultCount++;
			
			cbTmpLinkCount--;
		}
	}

	//特殊顺子
	if( cbTmpLinkCount >= cbLineCount-1 && cbValueIndex == 13 )
	{
		if( Distributing.cbDistributing[0][cbIndexCount] >= cbBlockCount || cbTmpLinkCount >= cbLineCount )
		{
			if( pSearchCardResult == NULL ) return 1;

			ASSERT(cbResultCount < CountArray(pSearchCardResult->cbCardCount));
			
			//复制扑克
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

			//复制A
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

			//设置变量
			pSearchCardResult->cbCardCount[cbResultCount] = cbCount;
			cbResultCount++;
		}
	}
	
	if( pSearchCardResult )
			pSearchCardResult->cbSearchCount = cbResultCount;
	return cbResultCount;
}

//搜索同花顺
BYTE CGameLogic::SearchSameColorLineType(const BYTE cbHandCardData[], BYTE cbHandCardCount, BYTE cbLineCount, tagSearchCardResult *pSearchCardResult)
{
	//设置结果
	if (pSearchCardResult)
		ZeroMemory(pSearchCardResult,sizeof(tagSearchCardResult));
	BYTE cbResultCount = 0;

	//长度判断
	if (cbHandCardCount < cbLineCount) return cbResultCount;

	//变量定义
	tagSearchCardResult tagTempResult, tagTempResult2;
	ZeroMemory(&tagTempResult, sizeof(tagTempResult));
	ZeroMemory(&tagTempResult2, sizeof(tagTempResult2));

	//搜索同花
	SearchSameColorType(cbHandCardData, cbHandCardCount, 5, &tagTempResult);

	//同花中搜索顺子
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

//构造扑克
BYTE CGameLogic::MakeCardData(BYTE cbValueIndex, BYTE cbColorIndex)
{
	return (cbColorIndex<<4)|(cbValueIndex+1);
}
/****************************************************
*函数名：CardTypeToWeights 
*功能：  用于将牌的类型转换成权值输出
*参数：	 bType		牌类型	IN
*返回值：int(权值，本函数只对已有的牌型做出相应权值的换算)
****************************************************/
int CGameLogic::CardTypeToWeights(BYTE bType, BYTE cbIndex)
{
	switch(bType)
	{
	case  CT_INVALID:												//错误类型
		{
			return -1;
		}
	case  CT_SINGLE:											    //单牌类型
		{
			return 0;
		}
	case CT_ONE_DOUBLE:										        //只有一对
		{
			return 2;
		}
	case CT_TWO_DOUBLE:										        //两对牌型
		{
			return 3;
		}
	case CT_THREE:													//三张牌型
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
	case CT_FIVE_MIXED_FLUSH_FIRST_A:							    //A前顺子
		{
			return 5;
		}
	case CT_FIVE_MIXED_FLUSH_NO_A:								    //普通顺子
		{
			return 6;
		}
	case CT_FIVE_MIXED_FLUSH_BACK_A:								//A后顺子
		{
			return 7;
		}
	case CT_FIVE_FLUSH:								                //同花
		{
			return 8;
		}
	case CT_FIVE_THREE_DEOUBLE:								        //三条一对
		{
			return 9;
		}
	case CT_FIVE_FOUR_ONE:								            //四带一张
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
	case CT_FIVE_STRAIGHT_FLUSH_FIRST_A:							//A同花顺
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
	case CT_FIVE_STRAIGHT_FLUSH:									//同花顺牌
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
	//case  CT_INVALID:												//错误类型
	//	{
	//		return -1;
	//	}
	//case  CT_SINGLE:											    //单牌类型
	//	{
	//		return 0;
	//	}
	//case CT_ONE_DOUBLE:										        //只有一对
	//	{
	//		return 2;
	//	}
	//case CT_TWO_DOUBLE:										        //两对牌型
	//	{
	//		return 4;
	//	}
	//case CT_THREE:													//三张牌型
	//	{
	//		return 8;
	//	}
	//case CT_FIVE_MIXED_FLUSH_FIRST_A:							    //A前顺子
	//	{
	//		return 15;
	//	}
	//case CT_FIVE_MIXED_FLUSH_NO_A:								    //普通顺子
	//case CT_FIVE_MIXED_FLUSH_BACK_A:								//A后顺子
	//	{
	//		return 16;
	//	}
	//case CT_FIVE_FLUSH:								                //同花
	//	{
	//		return 32;
	//	}
	//case CT_FIVE_THREE_DEOUBLE:								        //三条一对
	//	{
	//		return 64;
	//	}
	//case CT_FIVE_FOUR_ONE:								            //四带一张
	//	{
	//		return 128;
	//	}
	//case CT_FIVE_STRAIGHT_FLUSH_FIRST_A:							//A同花顺
	//	{
	//		return 255;
	//	}
	//case CT_FIVE_STRAIGHT_FLUSH:									//同花顺牌
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
*函数名：OptimizationCombo 
*功能：  对已经摆好的牌序列进行优化，让摆出的牌显得更合理
*参数：	 bInFirstList		第一组牌数据	IN/OUT
*参数：	 bInNextList		第二组牌数据	IN/OUT
*参数：	 bFirstCount		第一组牌张数	IN
*参数：	 bNextCount 		第二组牌张数	IN
*返回值：无(本函数要保证第一组牌型大小不大于第二组牌型)
****************************************************/
void CGameLogic::OptimizationCombo(BYTE bInFirstList[], BYTE bInNextList[], BYTE bFirstCount, BYTE bNextCount)
{

	if(bFirstCount <= bNextCount && (3 == bFirstCount || 5==bFirstCount) && 5 == bNextCount)
	{

		//获取类型
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


		//牌型相同不进行优化
		if(bFirstType == bNextType)
		{
			return;
		}
		switch(bNextType)
		{
		case CT_INVALID:												//错误类型
		case CT_SINGLE:											        //单牌类型
		case CT_FIVE_MIXED_FLUSH_FIRST_A:							    //A前顺子
		case CT_FIVE_MIXED_FLUSH_NO_A:								    //普通顺子
		case CT_FIVE_MIXED_FLUSH_BACK_A:								//A后顺子
		case CT_FIVE_FLUSH:								                //同花
		case CT_FIVE_STRAIGHT_FLUSH_FIRST_A:							//A同花顺
		case CT_FIVE_STRAIGHT_FLUSH:									//同花顺牌
			{
				//这些牌型无需优化直接返回
				return ;
			}
		case CT_ONE_DOUBLE:										        //只有一对
		case CT_THREE:													//三张牌型
		case CT_FIVE_FOUR_ONE:								            //四带一张
			{
				//对原有的对子(三张、四张)进行保存不允许修改
				for(int i = 0;i < (bNextCount - 1);i ++)
				{
					if(GetCardLogicValue(bInNextList[i]) == GetCardLogicValue(bInNextList[i + 1]))
					{
						iTemp1 = GetCardLogicValue(bInNextList[i]);
						break;
					}
				}

				//对剩下的单牌进行置换，在不破坏牌型的情况下将大的单牌放在前面一注
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
		case CT_TWO_DOUBLE:										        //两对牌型
			{
				//对原有的两个对子进行保存不允许修改
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

				//对剩下的单牌进行置换，在不破坏牌型的情况下将大的单牌放在前面一注
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
		case CT_FIVE_THREE_DEOUBLE:								        //三条一对
			{
				//找出对子
				if(GetCardLogicValue(bInNextList[1]) == GetCardLogicValue(bInNextList[2]))
				{
					iTemp1 = GetCardLogicValue(bInNextList[3]);
				}
				else
				{
					iTemp1 = GetCardLogicValue(bInNextList[1]);
				}
				//对对子进行置换，在不破坏牌型的情况下将大的对子放在前面一注
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
