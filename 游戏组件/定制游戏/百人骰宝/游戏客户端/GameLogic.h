#ifndef GAME_LOGIC_HEAD_FILE
#define GAME_LOGIC_HEAD_FILE

#pragma once

#include "Stdafx.h"

//////////////////////////////////////////////////////////////////////////
//牌型

enum emCardType
{
	CT_PAIR_TEN		=		10,				//对十
	CT_PAIR_NIGHT	=		9,				//对九
	CT_PAIR_EIGHT	=		8,				//对八
	CT_PAIR_SEVEN	=		7,				//对七
	CT_PAIR_SISEX	=		6,				//对六
	CT_PAIR_FIVE	=		5,				//对五
	CT_PAIR_FOUR	=		4,				//对四
	CT_PAIR_THREE	=		3,				//对三
	CT_PAIR_TWO		=		2,				//对二
	CT_PAIR_ONE		=		1,				//对一
	CT_POINT		=		0,				//点数类型
	CT_PAIR			=		11

};
		
//数值掩码
#define	LOGIC_MASK_COLOR			0xF0								//花色掩码
#define	LOGIC_MASK_VALUE			0x0F								//数值掩码

//排序类型
#define	ST_VALUE					1									//数值排序
#define	ST_NEW					    3									//数值排序
#define	ST_LOGIC					2									//逻辑排序

//////////////////////////////////////////////////////////////////////////

//游戏逻辑
class CGameLogic
{
	//变量定义
private:
	static const BYTE				m_cbCardListData[ALL_CARD_COUNT];		//扑克定义

	//函数定义
public:
	//构造函数
	CGameLogic();
	//析构函数
	virtual ~CGameLogic();

	//类型函数
public:
	//获取数值
	BYTE GetCardValue(BYTE cbCardData) 
	{ 
		return cbCardData&LOGIC_MASK_VALUE; 
	}
	//获取花色
	BYTE GetCardColor(BYTE cbCardData)
	{
		return (cbCardData&LOGIC_MASK_COLOR)>>4;
	}

	//控制函数
public:
	//混乱扑克
	void RandCardList(BYTE cbCardBuffer[], BYTE cbBufferCount);
	//排列扑克
	void SortCardList(BYTE cbCardData[], BYTE cbCardCount, BYTE cbSortType);

	//逻辑函数
public:
	//获取牌点
	BYTE GetCardListPip(const BYTE cbCardData[], BYTE cbCardCount);
	//获取牌型
	BYTE GetCardType(const BYTE cbCardData[], BYTE cbCardCount);
	//大小比较
	int CompareCard(const BYTE cbFirstCardData[],const BYTE cbNextCardData[]);
	//逻辑大小
	BYTE GetCardLogicValue(BYTE cbCardData);

	BYTE GetCardNewValue(BYTE cbCardData);
};

//////////////////////////////////////////////////////////////////////////

#endif
