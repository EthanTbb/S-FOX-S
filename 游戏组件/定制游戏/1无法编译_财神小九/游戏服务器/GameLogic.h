#ifndef GAME_LOGIC_HEAD_FILE
#define GAME_LOGIC_HEAD_FILE

#pragma once

#include "Stdafx.h"

//////////////////////////////////////////////////////////////////////////
//����
#define	CT_ERROR					0									//��������
#define	CT_POINT					1									//��������
#define	CT_SPECIAL				    2									//�������� ����


//��ֵ����
#define	LOGIC_MASK_COLOR			0xF0								//��ɫ����
#define	LOGIC_MASK_VALUE			0x0F								//��ֵ����

//��������
#define	ST_VALUE					1									//��ֵ����
#define	ST_LOGIC					2									//�߼�����

//�˿���Ŀ
#define CARD_COUNT					40									//�˿���Ŀ
//////////////////////////////////////////////////////////////////////////


#ifndef _UNICODE
#define myprintf	_snprintf
#define mystrcpy	strcpy
#define mystrlen	strlen
#define myscanf		_snscanf
#define	myLPSTR		LPCSTR
#else
#define myprintf	swprintf
#define mystrcpy	wcscpy
#define mystrlen	wcslen
#define myscanf		_snwscanf
#define	myLPSTR		LPWSTR
#endif

//��Ϸ�߼�
class CGameLogic
{
	//��������
private:
	static const BYTE				m_cbCardListData[CARD_COUNT];		//�˿˶���

	//��������
public:
	//���캯��
	CGameLogic();
	//��������
	virtual ~CGameLogic();

	//���ͺ���
public:
	//��ȡ��ֵ
	BYTE GetCardValue(BYTE cbCardData) { return cbCardData&LOGIC_MASK_VALUE; }
	//��ȡ��ɫ
	BYTE GetCardColor(BYTE cbCardData) { return (cbCardData&LOGIC_MASK_COLOR)>>4; }

	//���ƺ���
public:
	//�����˿�
	VOID RandCardList(BYTE cbCardBuffer[], BYTE cbBufferCount);
	//�����˿�
	VOID SortCardList(BYTE cbCardData[], BYTE cbCardCount, BYTE cbSortType);

	//�߼�����
public:
	//��ȡ�Ƶ�
	BYTE GetCardListPip(const BYTE cbCardData[], BYTE cbCardCount);
	//��ȡ����
	BYTE GetCardType(const BYTE cbCardData[], BYTE cbCardCount);
	//��С�Ƚ�
	int CompareCard(const BYTE cbFirstCardData[], BYTE cbFirstCardCount,const BYTE cbNextCardData[], BYTE cbNextCardCount);
	//�߼���С
	BYTE GetCardLogicValue(BYTE cbCardData);
};

//////////////////////////////////////////////////////////////////////////

#endif
