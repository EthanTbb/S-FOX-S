#ifndef MUL_SECTION_CARD_CONTROL_HEAD_FILE
#define MUL_SECTION_CARD_CONTROL_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "CardControl.h"

//////////////////////////////////////////////////////////////////////////
//�궨��
#define MSCC_SECTION_NUM			3
#define MSCC_SECTION_ONE			3
#define MSCC_SECTION_TWO			5
#define MSCC_SECTION_THREE			5

//�˿˿ؼ�
class CMulSecCardControlEx
{
	//�ڲ�����
public:
	CCardControl					m_PerCardData[MSCC_SECTION_NUM];	//�ֶοؼ�

	//��������
public:
	//���캯��
	CMulSecCardControlEx();
	//��������
	virtual ~CMulSecCardControlEx();

	//�滭�˿�
	VOID DrawCardControl(CDC * pDC, INT nLayer = -1);

	//�ؼ�����
public:
	//��ʾ����
	void ShowWindow(UINT nShowCmd, int nSecIndex = -1);
	//������ʾ
	void SetDisplayFlag(bool bDisplay, int nSecIndex = -1);
	//���ü��
	void SetCardSpace(DWORD dwCardHSpace, DWORD dwCardVSpace);
	//��׼λ��
	void SetBenchmarkPos(int nXPos, int nYPos, enXCollocateMode XCollocateMode, enYCollocateMode YCollocateMode);
	//��׼λ��
	void SetBenchmarkPos(const CPoint & BenchmarkPos, enXCollocateMode XCollocateMode, enYCollocateMode YCollocateMode);

	//�˿˿���
public:
	//��ȡ��Ŀ
	WORD GetCardCount(int nSecIndex);
	//�����˿�
	WORD SetCardData(BYTE cbCardData[], WORD wCardCount);
	//�����˿�
	WORD SetCardData(int nSecIndex, BYTE cbCardData[], WORD wCardCount);
	//��ȡ�˿�
	WORD GetCardData(int nSecIndex, BYTE cbCardData[], WORD wMaxCount);
	//��ȡԭ��
	CPoint GetOrignPoint(int nSecIndex);
	//��ȡ����
	CPoint GetCenterPoint(int nSecIndex);
};

//////////////////////////////////////////////////////////////////////////

#endif
