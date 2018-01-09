#include "StdAfx.h"
#include "GameLogic.h"

//////////////////////////////////////////////////////////////////////////

//扑克数据
BYTE CGameLogic::m_cbCardListData[54]=
{
  0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D, //方块 A - K
  0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,0x1B,0x1C,0x1D, //梅花 A - K
  0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2A,0x2B,0x2C,0x2D, //红桃 A - K
  0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3A,0x3B,0x3C,0x3D, //黑桃 A - K
  0x4E,0x4F
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
BYTE CGameLogic::GetCardType(BYTE cbCardData[], BYTE cbCardCount)
{
  //数据效验
  ASSERT(cbCardCount==MAX_COUNT);

  //王牌统计
  BYTE cbKingCount = 0;
  for(BYTE i = 0; i <MAX_COUNT; i++)
  {
    if(cbCardData[i] == 0x4E || cbCardData[i] == 0x4F)
    {
      cbKingCount++;
    }
  }
  ASSERT(cbKingCount < 3);

  //分类组合
  if(cbKingCount == 2)
  {
    //设置变量
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
    //设置变量
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
    //特殊牌型
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

    //设置变量
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

//获取倍数
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

//获取牛牛
bool CGameLogic::GetOxCard(BYTE cbCardData[], BYTE cbCardCount)
{
  //数据效验
  ASSERT(cbCardCount==MAX_COUNT);

  //王牌统计
  BYTE cbKingCount = 0;
  for(BYTE i = 0; i <MAX_COUNT; i++)
  {
    if(cbCardData[i] == 0x4E || cbCardData[i] == 0x4F)
    {
      cbKingCount++;
    }
  }
  ASSERT(cbKingCount < 3);

  //分类组合
  if(cbKingCount == 2)
  {
    //拷贝扑克
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
    //设置变量
    BYTE cbTempData[MAX_COUNT];
    CopyMemory(cbTempData, cbCardData, sizeof(cbTempData));
    SortCardList(cbTempData, MAX_COUNT);

    //牛牛牌型
    BYTE cbSum = 0;
    BYTE cbTemp[MAX_COUNT];
    ZeroMemory(cbTemp, sizeof(cbTemp));
    for(BYTE i = 1; i < MAX_COUNT; i++)
    {
      cbTemp[i] = GetCardLogicValue(cbTempData[i]);
      cbSum += cbTemp[i];
    }

    //三张成十
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

    //两张成十
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

    //四中取大
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
    //普通牌型
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

    //查找牛牛
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

//获取整数
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

//排列扑克
void CGameLogic::SortCardList(BYTE cbCardData[], BYTE cbCardCount)
{
  //转换数值
  BYTE cbLogicValue[MAX_COUNT];
  for(BYTE i=0; i<cbCardCount; i++)
  {
    cbLogicValue[i]=GetCardValue(cbCardData[i]);
  }

  //排序操作
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
        //交换位置
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

//混乱扑克
void CGameLogic::RandCardList(BYTE cbCardBuffer[], BYTE cbBufferCount)
{
  //混乱准备
  BYTE cbCardData[CountArray(m_cbCardListData)];
  CopyMemory(cbCardData,m_cbCardListData,sizeof(m_cbCardListData));

  //混乱扑克
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

//逻辑数值
BYTE CGameLogic::GetCardLogicValue(BYTE cbCardData)
{
  //扑克属性
  BYTE bCardColor=GetCardColor(cbCardData);
  BYTE bCardValue=GetCardValue(cbCardData);

  //转换数值
  return (bCardValue>10)?(10):bCardValue;
}

//对比扑克
bool CGameLogic::CompareCard(BYTE cbFirstData[], BYTE cbNextData[], BYTE cbCardCount,BOOL FirstOX,BOOL NextOX)
{
  if(FirstOX != NextOX)
  {
    return (FirstOX > NextOX);
  }

  //比较大小
  if(FirstOX == TRUE)
  {
    //获取点数
    BYTE cbNextType = GetCardType(cbNextData, cbCardCount);
    BYTE cbFirstType = GetCardType(cbFirstData, cbCardCount);

    //点数判断
    if(cbFirstType != cbNextType)
    {
      return (cbFirstType > cbNextType);
    }
  }

  //排序大小
  BYTE bNextTemp[MAX_COUNT];
  BYTE bFirstTemp[MAX_COUNT];
  CopyMemory(bNextTemp, cbNextData, cbCardCount);
  CopyMemory(bFirstTemp, cbFirstData, cbCardCount);
  SortCardList(bNextTemp, cbCardCount);
  SortCardList(bFirstTemp, cbCardCount);

  //比较数值
  BYTE cbNextMaxValue = GetCardValue(bNextTemp[0]);
  BYTE cbFirstMaxValue = GetCardValue(bFirstTemp[0]);
  if(cbNextMaxValue != cbFirstMaxValue)
  {
    return cbFirstMaxValue > cbNextMaxValue;
  }

  //比较颜色
  return GetCardColor(bFirstTemp[0]) > GetCardColor(bNextTemp[0]);

  return false;
}

//////////////////////////////////////////////////////////////////////////
