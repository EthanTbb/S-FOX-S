#ifndef MUL_SECTION_CARD_CONTROL_HEAD_FILE
#define MUL_SECTION_CARD_CONTROL_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "CardControl.h"

//////////////////////////////////////////////////////////////////////////
//宏定义
#define MSCC_SECTION_NUM			3
#define MSCC_SECTION_ONE			3
#define MSCC_SECTION_TWO			5
#define MSCC_SECTION_THREE			5

//扑克控件
class CMulSecCardControlEx
{
	//内部数据
public:
	CCardControl					m_PerCardData[MSCC_SECTION_NUM];	//分段控件

	//函数定义
public:
	//构造函数
	CMulSecCardControlEx();
	//析构函数
	virtual ~CMulSecCardControlEx();

	//绘画扑克
	VOID DrawCardControl(CDC * pDC, INT nLayer = -1);

	//控件控制
public:
	//显示窗体
	void ShowWindow(UINT nShowCmd, int nSecIndex = -1);
	//设置显示
	void SetDisplayFlag(bool bDisplay, int nSecIndex = -1);
	//设置间距
	void SetCardSpace(DWORD dwCardHSpace, DWORD dwCardVSpace);
	//基准位置
	void SetBenchmarkPos(int nXPos, int nYPos, enXCollocateMode XCollocateMode, enYCollocateMode YCollocateMode);
	//基准位置
	void SetBenchmarkPos(const CPoint & BenchmarkPos, enXCollocateMode XCollocateMode, enYCollocateMode YCollocateMode);

	//扑克控制
public:
	//获取数目
	WORD GetCardCount(int nSecIndex);
	//设置扑克
	WORD SetCardData(BYTE cbCardData[], WORD wCardCount);
	//设置扑克
	WORD SetCardData(int nSecIndex, BYTE cbCardData[], WORD wCardCount);
	//获取扑克
	WORD GetCardData(int nSecIndex, BYTE cbCardData[], WORD wMaxCount);
	//获取原点
	CPoint GetOrignPoint(int nSecIndex);
	//获取中心
	CPoint GetCenterPoint(int nSecIndex);
};

//////////////////////////////////////////////////////////////////////////

#endif
