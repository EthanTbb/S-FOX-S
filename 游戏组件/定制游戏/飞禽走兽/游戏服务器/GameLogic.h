#ifndef GAME_LOGIC_HEAD_FILE
#define GAME_LOGIC_HEAD_FILE

#pragma once

#include "Stdafx.h"

//////////////////////////////////////////////////////////////////////////
//����

//��ֵ����
#define LOGIC_MASK_COLOR      0xF0                //��ɫ����
#define LOGIC_MASK_VALUE      0x0F                //��ֵ����

//��������
#define ST_VALUE          1                 //��ֵ����
#define ST_NEW              3                 //��ֵ����
#define ST_LOGIC          2                 //�߼�����

//�˿���Ŀ
#define CARD_COUNT          32                  //�˿���Ŀ
//////////////////////////////////////////////////////////////////////////

//��Ϸ�߼�
class CGameLogic
{
  //��������
private:
  static  BYTE        m_cbCardListData[172-4];    //�˿˶���

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
  //��������
  void RandArray(INT nArrayOne[], INT nCountOne, INT nArrayTwo[], INT nCountTwo);
  //�����˿�
  void RandCardList(BYTE cbCardBuffer[], BYTE cbBufferCount);
  //�����˿�
  void SortCardList(BYTE cbCardData[], BYTE cbCardCount, BYTE cbSortType);
  //��������
  void RandListArray(BYTE cbBuffer[], BYTE cbBufferCount, bool conflict);

  //�߼�����
public:
  //��ȡ�Ƶ�
  BYTE GetCardListPip(const BYTE cbCardData[], BYTE cbCardCount);
  //��ȡ����
  BYTE GetCardType(const BYTE cbCardData, BYTE cbCardCount,BYTE *bcOutCadData = NULL);
  //��С�Ƚ�
  int CompareCard(const BYTE cbFirstCardData[], BYTE cbFirstCardCount,const BYTE cbNextCardData[], BYTE cbNextCardCount,BYTE &Multiple);
  //�߼���С
  BYTE GetCardLogicValue(BYTE cbCardData);

  BYTE GetCardNewValue(BYTE cbCardData);
};

//////////////////////////////////////////////////////////////////////////

#endif
