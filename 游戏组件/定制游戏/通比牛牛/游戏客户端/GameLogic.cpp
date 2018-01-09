#include "StdAfx.h"
#include "GameLogic.h"

//////////////////////////////////////////////////////////////////////////

//�˿�����
BYTE CGameLogic::m_cbCardListData[54]=
{
  0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D, //���� A - K
  0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,0x1B,0x1C,0x1D, //÷�� A - K
  0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2A,0x2B,0x2C,0x2D, //���� A - K
  0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3A,0x3B,0x3C,0x3D, //���� A - K
  0x4E,0x4F
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
BYTE CGameLogic::GetCardType(BYTE cbCardData[], BYTE cbCardCount)
{
  //����Ч��
  ASSERT(cbCardCount==MAX_COUNT);

  //����ͳ��
  BYTE cbKingCount = 0;
  for(BYTE i = 0; i <MAX_COUNT; i++)
  {
    if(cbCardData[i] == 0x4E || cbCardData[i] == 0x4F)
    {
      cbKingCount++;
    }
  }
  ASSERT(cbKingCount < 3);

  //�������
  if(cbKingCount == 2)
  {
    //���ñ���
    BYTE cbTempData[MAX_COUNT];
    CopyMemory(cbTempData, cbCardData, sizeof(cbTempData));

    if(cbTempData[0] != 0x4E && cbTempData[0] != 0x4F &&
       cbTempData[1] != 0x4E && cbTempData[1] != 0x4F &&
       cbTempData[2] != 0x4E && cbTempData[2] != 0x4F)
    {
      BYTE cbValue0 = GetCardLogicValue(cbTempData[0]);
      BYTE cbValue1 = GetCardLogicValue(cbTempData[1]);
      BYTE cbValue2 = GetCardLogicValue(cbTempData[2]);

      if((cbValue0 + cbValue1 + cbValue2) % 10 == 0)
      {
        return OX_VALUE_10;
      }
    }
    else if(cbTempData[3] != 0x4E && cbTempData[3] != 0x4F &&
            cbTempData[4] != 0x4E && cbTempData[4] != 0x4F)
    {
      BYTE cbCount = (GetCardLogicValue(cbTempData[3]) + GetCardLogicValue(cbTempData[4])) % 10;
      switch(cbCount)
      {
        case 1:
          return OX_VALUE_1;
        case 2:
          return OX_VALUE_2;
        case 3:
          return OX_VALUE_3;
        case 4:
          return OX_VALUE_4;
        case 5:
          return OX_VALUE_5;
        case 6:
          return OX_VALUE_6;
        case 7:
          return OX_VALUE_7;
        case 8:
          return OX_VALUE_8;
        case 9:
          return OX_VALUE_9;
        case 0:
          return OX_VALUE_10;
        default:
          ASSERT(FALSE);
      }
    }
    else
    {
      return OX_VALUE_10;
    }
  }
  else if(cbKingCount == 1)
  {
    //���ñ���
    BYTE cbTempData[MAX_COUNT];
    CopyMemory(cbTempData, cbCardData, sizeof(cbTempData));

    if(cbTempData[0] != 0x4E && cbTempData[0] != 0x4F &&
       cbTempData[1] != 0x4E && cbTempData[1] != 0x4F &&
       cbTempData[2] != 0x4E && cbTempData[2] != 0x4F)
    {
      BYTE cbValue0 = GetCardLogicValue(cbTempData[0]);
      BYTE cbValue1 = GetCardLogicValue(cbTempData[1]);
      BYTE cbValue2 = GetCardLogicValue(cbTempData[2]);

      if((cbValue0 + cbValue1 + cbValue2) % 10 == 0)
      {
        return OX_VALUE_10;
      }
    }
    else
    {
      BYTE cbCount = (GetCardLogicValue(cbTempData[3]) + GetCardLogicValue(cbTempData[4])) % 10;
      switch(cbCount)
      {
        case 1:
          return OX_VALUE_1;
        case 2:
          return OX_VALUE_2;
        case 3:
          return OX_VALUE_3;
        case 4:
          return OX_VALUE_4;
        case 5:
          return OX_VALUE_5;
        case 6:
          return OX_VALUE_6;
        case 7:
          return OX_VALUE_7;
        case 8:
          return OX_VALUE_8;
        case 9:
          return OX_VALUE_9;
        case 0:
          return OX_VALUE_10;
        default:
          ASSERT(FALSE);
      }
    }
  }
  else if(cbKingCount == 0)
  {
    //��������
    BYTE cbTenCount = 0;
    BYTE cbJQKCount = 0;
    for(BYTE i=0; i<cbCardCount; i++)
    {
      if(GetCardValue(cbCardData[i]) > 10)
      {
        cbJQKCount++;
      }
      else if(GetCardValue(cbCardData[i]) == 10)
      {
        cbTenCount++;
      }
    }
    if(cbJQKCount==MAX_COUNT)
    {
      return OX_FIVE_KING;
    }
    else if(cbJQKCount==MAX_COUNT-1 && cbTenCount==1)
    {
      return OX_FOUR_KING;
    }

    //���ñ���
    BYTE cbTempData[MAX_COUNT];
    CopyMemory(cbTempData, cbCardData, sizeof(cbTempData));

    BYTE cbValue0 = GetCardLogicValue(cbTempData[0]);
    BYTE cbValue1 = GetCardLogicValue(cbTempData[1]);
    BYTE cbValue2 = GetCardLogicValue(cbTempData[2]);

    if((cbValue0 + cbValue1 + cbValue2) % 10 == 0)
    {
      BYTE cbCount = (GetCardLogicValue(cbTempData[3]) + GetCardLogicValue(cbTempData[4])) % 10;
      switch(cbCount)
      {
        case 1:
          return OX_VALUE_BIG_1;
        case 2:
          return OX_VALUE_BIG_2;
        case 3:
          return OX_VALUE_BIG_3;
        case 4:
          return OX_VALUE_BIG_4;
        case 5:
          return OX_VALUE_BIG_5;
        case 6:
          return OX_VALUE_BIG_6;
        case 7:
          return OX_VALUE_BIG_7;
        case 8:
          return OX_VALUE_BIG_8;
        case 9:
          return OX_VALUE_BIG_9;
        case 0:
          return OX_VALUE_BIG_10;
        default:
          ASSERT(FALSE);
      }
    }
  }

  return OX_VALUE_0;
}

//��ȡ����
BYTE CGameLogic::GetTimes(BYTE cbCardData[], BYTE cbCardCount)
{
  if(cbCardCount != MAX_COUNT)
  {
    return 0;
  }

  BYTE bTimes = GetCardType(cbCardData, MAX_COUNT);
  if(bTimes < OX_VALUE_10)
  {
    return 1;
  }
  else if(bTimes == OX_VALUE_10)
  {
    return 2;
  }
  else if(bTimes == OX_VALUE_BIG_10)
  {
    return 2;
  }
  else if(bTimes == OX_FOUR_KING)
  {
    return 2;
  }
  else if(bTimes == OX_FIVE_KING)
  {
    return 2;
  }

  return 0;
}

//��ȡţţ
bool CGameLogic::GetOxCard(BYTE cbCardData[], BYTE cbCardCount)
{
  //����Ч��
  ASSERT(cbCardCount==MAX_COUNT);

  //����ͳ��
  BYTE cbKingCount = 0;
  for(BYTE i = 0; i <MAX_COUNT; i++)
  {
    if(cbCardData[i] == 0x4E || cbCardData[i] == 0x4F)
    {
      cbKingCount++;
    }
  }
  ASSERT(cbKingCount < 3);

  //�������
  if(cbKingCount == 2)
  {
    //�����˿�
    BYTE cbTempData[MAX_COUNT];
    CopyMemory(cbTempData, cbCardData, sizeof(cbTempData));
    SortCardList(cbTempData, MAX_COUNT);

    cbCardData[0] = cbTempData[2];
    cbCardData[1] = cbTempData[3];
    cbCardData[2] = cbTempData[0];
    cbCardData[3] = cbTempData[1];
    cbCardData[4] = cbTempData[4];

    return true;
  }
  else if(cbKingCount == 1)
  {
    //���ñ���
    BYTE cbTempData[MAX_COUNT];
    CopyMemory(cbTempData, cbCardData, sizeof(cbTempData));
    SortCardList(cbTempData, MAX_COUNT);

    //ţţ����
    BYTE cbSum = 0;
    BYTE cbTemp[MAX_COUNT];
    ZeroMemory(cbTemp, sizeof(cbTemp));
    for(BYTE i = 1; i < MAX_COUNT; i++)
    {
      cbTemp[i] = GetCardLogicValue(cbTempData[i]);
      cbSum += cbTemp[i];
    }

    //���ų�ʮ
    for(BYTE i = 1; i < MAX_COUNT; i++)
    {
      if((cbSum - cbTemp[i])%10 == 0)
      {
        BYTE cbCount = 0;
        for(BYTE j = 1; j < MAX_COUNT; j++)
        {
          if(i != j)
          {
            cbCardData[cbCount++] = cbTempData[j];
          }
        }
        ASSERT(cbCount == 3);

        cbCardData[cbCount++] = cbTempData[i];
        cbCardData[cbCount++] = cbTempData[0];

        return true;
      }
    }

    //���ų�ʮ
    for(BYTE i = 1; i < MAX_COUNT - 1; i++)
    {
      for(BYTE j = i + 1; j < MAX_COUNT; j++)
      {
        if((cbSum - cbTemp[i] - cbTemp[j]) % 10 == 0)
        {
          BYTE cbCount = 0;
          cbCardData[cbCount++] = cbTempData[0];
          cbCardData[cbCount++] = cbTempData[i];
          cbCardData[cbCount++] = cbTempData[j];

          for(BYTE k = 1; k < MAX_COUNT; k++)
          {
            if(i != k && j != k)
            {
              cbCardData[cbCount++] = cbTempData[k];
            }
          }
          ASSERT(cbCount == 5);

          return true;
        }
      }
    }

    //����ȡ��
    BYTE cbBigCount = 0;
    BYTE cbBigIndex1 = 0;
    BYTE cbBigIndex2 = 0;
    for(BYTE i = 1; i < MAX_COUNT; i++)
    {
      for(BYTE j = i + 1; j < MAX_COUNT; j++)
      {
        BYTE cbSumCount = (GetCardLogicValue(cbTempData[i]) + GetCardLogicValue(cbTempData[j])) % 10;
        if(cbSumCount > cbBigCount)
        {
          cbBigIndex1 = i;
          cbBigIndex2 = j;
          cbBigCount = cbSumCount;
        }
      }
    }

    BYTE cbCount = 0;
    for(BYTE i = 1; i < MAX_COUNT; i++)
    {
      if(i != cbBigIndex1 && i != cbBigIndex2)
      {
        cbCardData[cbCount++] = cbTempData[i];
      }
    }
    ASSERT(cbCount == 2);

    cbCardData[cbCount++] = cbTempData[0];
    cbCardData[cbCount++] = cbTempData[cbBigIndex1];
    cbCardData[cbCount++] = cbTempData[cbBigIndex2];

    return true;
  }
  else if(cbKingCount == 0)
  {
    //��ͨ����
    BYTE cbSum = 0;
    BYTE cbTemp[MAX_COUNT];
    BYTE cbTempData[MAX_COUNT];
    ZeroMemory(cbTemp, sizeof(cbTemp));
    CopyMemory(cbTempData, cbCardData, sizeof(cbTempData));
    for(BYTE i = 0; i < cbCardCount; i++)
    {
      cbTemp[i] = GetCardLogicValue(cbCardData[i]);
      cbSum += cbTemp[i];
    }

    //����ţţ
    for(BYTE i = 0; i < cbCardCount - 1; i++)
    {
      for(BYTE j = i + 1; j < cbCardCount; j++)
      {
        if((cbSum - cbTemp[i] - cbTemp[j]) % 10 == 0)
        {
          BYTE cbCount = 0;
          for(BYTE k = 0; k < cbCardCount; k++)
          {
            if(k != i && k != j)
            {
              cbCardData[cbCount++] = cbTempData[k];
            }
          }
          ASSERT(cbCount == 3);

          cbCardData[cbCount++] = cbTempData[i];
          cbCardData[cbCount++] = cbTempData[j];

          return true;
        }
      }
    }
  }

  return false;
}

//��ȡ����
bool CGameLogic::IsIntValue(BYTE cbCardData[], BYTE cbCardCount)
{
  BYTE sum=0;
  for(BYTE i=0; i<cbCardCount; i++)
  {
    if(GetCardColor(cbCardData[i]) == 0x40)
    {
      return true;
    }
    sum+=GetCardLogicValue(cbCardData[i]);
  }
  ASSERT(sum>0);
  return (sum%10==0);
}

//�����˿�
void CGameLogic::SortCardList(BYTE cbCardData[], BYTE cbCardCount)
{
  //ת����ֵ
  BYTE cbLogicValue[MAX_COUNT];
  for(BYTE i=0; i<cbCardCount; i++)
  {
    cbLogicValue[i]=GetCardValue(cbCardData[i]);
  }

  //�������
  bool bSorted=true;
  BYTE cbTempData,bLast=cbCardCount-1;
  do
  {
    bSorted=true;
    for(BYTE i=0; i<bLast; i++)
    {
      if((cbLogicValue[i]<cbLogicValue[i+1])||
         ((cbLogicValue[i]==cbLogicValue[i+1])&&(cbCardData[i]<cbCardData[i+1])))
      {
        //����λ��
        cbTempData=cbCardData[i];
        cbCardData[i]=cbCardData[i+1];
        cbCardData[i+1]=cbTempData;
        cbTempData=cbLogicValue[i];
        cbLogicValue[i]=cbLogicValue[i+1];
        cbLogicValue[i+1]=cbTempData;
        bSorted=false;
      }
    }
    bLast--;
  }
  while(bSorted==false);

  return;
}

//�����˿�
void CGameLogic::RandCardList(BYTE cbCardBuffer[], BYTE cbBufferCount)
{
  //����׼��
  BYTE cbCardData[CountArray(m_cbCardListData)];
  CopyMemory(cbCardData,m_cbCardListData,sizeof(m_cbCardListData));

  //�����˿�
  BYTE bRandCount=0,bPosition=0;
  do
  {
    bPosition=rand()%(CountArray(m_cbCardListData)-bRandCount);
    cbCardBuffer[bRandCount++]=cbCardData[bPosition];
    cbCardData[bPosition]=cbCardData[CountArray(m_cbCardListData)-bRandCount];
  }
  while(bRandCount<cbBufferCount);

  return;
}

//�߼���ֵ
BYTE CGameLogic::GetCardLogicValue(BYTE cbCardData)
{
  //�˿�����
  BYTE bCardColor=GetCardColor(cbCardData);
  BYTE bCardValue=GetCardValue(cbCardData);

  //ת����ֵ
  return (bCardValue>10)?(10):bCardValue;
}

//�Ա��˿�
bool CGameLogic::CompareCard(BYTE cbFirstData[], BYTE cbNextData[], BYTE cbCardCount,BOOL FirstOX,BOOL NextOX)
{
  if(FirstOX != NextOX)
  {
    return (FirstOX > NextOX);
  }

  //�Ƚϴ�С
  if(FirstOX == TRUE)
  {
    //��ȡ����
    BYTE cbNextType = GetCardType(cbNextData, cbCardCount);
    BYTE cbFirstType = GetCardType(cbFirstData, cbCardCount);

    //�����ж�
    if(cbFirstType != cbNextType)
    {
      return (cbFirstType > cbNextType);
    }
  }

  //�����С
  BYTE bNextTemp[MAX_COUNT];
  BYTE bFirstTemp[MAX_COUNT];
  CopyMemory(bNextTemp, cbNextData, cbCardCount);
  CopyMemory(bFirstTemp, cbFirstData, cbCardCount);
  SortCardList(bNextTemp, cbCardCount);
  SortCardList(bFirstTemp, cbCardCount);

  //�Ƚ���ֵ
  BYTE cbNextMaxValue = GetCardValue(bNextTemp[0]);
  BYTE cbFirstMaxValue = GetCardValue(bFirstTemp[0]);
  if(cbNextMaxValue != cbFirstMaxValue)
  {
    return cbFirstMaxValue > cbNextMaxValue;
  }

  //�Ƚ���ɫ
  return GetCardColor(bFirstTemp[0]) > GetCardColor(bNextTemp[0]);

  return false;
}

//////////////////////////////////////////////////////////////////////////
