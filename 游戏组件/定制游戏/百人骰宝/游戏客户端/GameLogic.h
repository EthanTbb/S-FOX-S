#ifndef GAME_LOGIC_HEAD_FILE
#define GAME_LOGIC_HEAD_FILE

#pragma once

#include "Stdafx.h"

//////////////////////////////////////////////////////////////////////////
//����

enum emCardType
{
	CT_PAIR_TEN		=		10,				//��ʮ
	CT_PAIR_NIGHT	=		9,				//�Ծ�
	CT_PAIR_EIGHT	=		8,				//�԰�
	CT_PAIR_SEVEN	=		7,				//����
	CT_PAIR_SISEX	=		6,				//����
	CT_PAIR_FIVE	=		5,				//����
	CT_PAIR_FOUR	=		4,				//����
	CT_PAIR_THREE	=		3,				//����
	CT_PAIR_TWO		=		2,				//�Զ�
	CT_PAIR_ONE		=		1,				//��һ
	CT_POINT		=		0,				//��������
	CT_PAIR			=		11

};
		
//��ֵ����
#define	LOGIC_MASK_COLOR			0xF0								//��ɫ����
#define	LOGIC_MASK_VALUE			0x0F								//��ֵ����

//��������
#define	ST_VALUE					1									//��ֵ����
#define	ST_NEW					    3									//��ֵ����
#define	ST_LOGIC					2									//�߼�����

//////////////////////////////////////////////////////////////////////////

//��Ϸ�߼�
class CGameLogic
{
	//��������
private:
	static const BYTE				m_cbCardListData[ALL_CARD_COUNT];		//�˿˶���

	//��������
public:
	//���캯��
	CGameLogic();
	//��������
	virtual ~CGameLogic();

	//���ͺ���
public:
	//��ȡ��ֵ
	BYTE GetCardValue(BYTE cbCardData) 
	{ 
		return cbCardData&LOGIC_MASK_VALUE; 
	}
	//��ȡ��ɫ
	BYTE GetCardColor(BYTE cbCardData)
	{
		return (cbCardData&LOGIC_MASK_COLOR)>>4;
	}

	//���ƺ���
public:
	//�����˿�
	void RandCardList(BYTE cbCardBuffer[], BYTE cbBufferCount);
	//�����˿�
	void SortCardList(BYTE cbCardData[], BYTE cbCardCount, BYTE cbSortType);

	//�߼�����
public:
	//��ȡ�Ƶ�
	BYTE GetCardListPip(const BYTE cbCardData[], BYTE cbCardCount);
	//��ȡ����
	BYTE GetCardType(const BYTE cbCardData[], BYTE cbCardCount);
	//��С�Ƚ�
	int CompareCard(const BYTE cbFirstCardData[],const BYTE cbNextCardData[]);
	//�߼���С
	BYTE GetCardLogicValue(BYTE cbCardData);

	BYTE GetCardNewValue(BYTE cbCardData);
};

//////////////////////////////////////////////////////////////////////////

#endif
