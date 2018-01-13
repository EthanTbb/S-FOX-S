#include "StdAfx.h"
#include "GameLogic.h"

//////////////////////////////////////////////////////////////////////////

//构造函数
CGameLogic::CGameLogic()
{
}

//析构函数
CGameLogic::~CGameLogic()
{
}

//获取牌型
BYTE CGameLogic::GetCardType(const BYTE cbCardData, BYTE cbCardCount,BYTE *bcOutCadDataWin )
{
	ASSERT(1==cbCardCount);
	if (!(1==cbCardCount) )return 0;
	memset(bcOutCadDataWin,0,AREA_COUNT);

	BYTE bcData =cbCardData;
	if(0==bcData) return true;
	
	//玛莎拉蒂
	if(1==bcData)
	{
		bcOutCadDataWin[0]= 20;
	}
	//法拉利
	else if(5==bcData)
	{
		bcOutCadDataWin[1]= 20;
	}
	//兰博基尼
	else if(13==bcData)
	{
		bcOutCadDataWin[2]= 20;
	}
	//保时捷
	else if(9==bcData)
	{
		bcOutCadDataWin[3]= 20;
	}
	//路虎
	else if(8==bcData||bcData==11||bcData==14)
	{
		bcOutCadDataWin[4]= 5;
	}
	//宝马
	else if(2==bcData||bcData==12||bcData==16)
	{
		bcOutCadDataWin[5]= 5;
	}
	//捷豹
	else if(4==bcData||bcData==7||bcData==10)
	{
		bcOutCadDataWin[6]= 5;
	}
	//奔驰
	else if(3==bcData||bcData==6||bcData==15)
	{
		bcOutCadDataWin[7]= 5;
	}
		
	return true;	
}

//大小比较
/*
cbNextCardData>cbFirstCardData  返回1
cbNextCardData<cbFirstCardData  返回-1
cbNextCardData==cbFirstCardData 返回0
*/
//Multiple 比较出来的倍数
int CGameLogic::CompareCard(const BYTE cbFirstCardData[], BYTE cbFirstCardCount,const BYTE cbNextCardData[], BYTE cbNextCardCount,BYTE &Multiple)
{
	//合法判断
	ASSERT(1==cbFirstCardCount && 1==cbNextCardCount);
	if (!(1==cbFirstCardCount && 1==cbNextCardCount)) return 0;

	return true;
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
//////////////////////////////////////////////////////////////////////////

//混乱数组
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

//获取索引
BYTE CGameLogic::GetAnimalIndex(BYTE cbAllIndex)
{
	//玛莎拉蒂
	if(1==cbAllIndex)
	{
		return 1;
	}
	//法拉利
	else if(5==cbAllIndex)
	{
		return 2;
	}
	//兰博基尼
	else if(13==cbAllIndex)
	{
		return 3;
	}
	//保时捷
	else if(9==cbAllIndex)
	{
		return 4;
	}
	//路虎
	else if(8==cbAllIndex||cbAllIndex==11||cbAllIndex==14)
	{
		return 5;
	}
	//宝马
	else if(2==cbAllIndex||cbAllIndex==12||cbAllIndex==16)
	{
		return 6;

	}
	//捷豹
	else if(4==cbAllIndex||cbAllIndex==7||cbAllIndex==10)
	{
		return 7;
	}
	//奔驰
	else if(3==cbAllIndex||cbAllIndex==6||cbAllIndex==15)
	{
		return 8;
	}

	return 0;
}