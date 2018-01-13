#include "StdAfx.h"
#include "GameLogic.h"

//////////////////////////////////////////////////////////////////////////

//�˿�����
BYTE CGameLogic::m_cbCardListData[172-4]=
{
  2,4,6,8,10,12,14,16,18,20,22,24,26,28,30,32,\
  2,4,6,8,10,12,14,16, 18,20,22,24,26,28,30,32,\
  1,9,17,25,3,11,19,27,3,11,19,27, 5,13,21,29,5,\
  13,21,29,5,13,21,29,7,15,23,31,7,15,23,31,7,15,23,31,7,15,23,31,\
  2,4,6,8,10,12,14,16,18,20,22,24,26,28,30,32,\
  2,4,6,8,10,12,14,16,18,20,22,24,26,28,30,32,\
  2,4,6,8,10,12,14,16,18,20,22,24,26,28,30,32,\
  2,4,6,8,10,12,14,16,18,20,22,24,26,28,30,32,\
  2,4,6,8,10,12,14,16,18,20,22,24,26,28,30,32,\
  2,4,6,8,10,12,14,16,18,20,22,24,26,28,30,32\

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

//�����˿�
void CGameLogic::RandCardList(BYTE cbCardBuffer[], BYTE cbBufferCount)
{

  //����׼��
  BYTE cbCardData[CountArray(m_cbCardListData)];
  CopyMemory(cbCardData,m_cbCardListData,sizeof(m_cbCardListData));

  //�����˿�
  BYTE cbRandCount=0,cbPosition=0;
  do
  {
    cbPosition=rand()%(CountArray(cbCardData)-cbRandCount);
    cbCardBuffer[cbRandCount++]=cbCardData[cbPosition];
    cbCardData[cbPosition]=cbCardData[CountArray(cbCardData)-cbRandCount];
  }
  while(cbRandCount<cbBufferCount);


  return;
}
//��ȡ����
BYTE CGameLogic::GetCardType(const BYTE cbCardData, BYTE cbCardCount,BYTE *bcOutCadDataWin)
{
  ASSERT(1==cbCardCount);
  if(!(1==cbCardCount))
  {
    return 0;
  }
  memset(bcOutCadDataWin,0,AREA_COUNT);


  BYTE bcData =cbCardData;
  if(0==bcData)
  {
    return true;
  }
  //����X6  ����X8  ��ȸX8  ��ӥX12 ʨ��X12 ��èX8  ����X8  ����X6 ����24 ����2 ����2
  //���� 4     3       3       2       2       3       3       4       1


  //����3 ���� ����3 ����3 ���� ��ȸ3 ��ӥ3 ���� ʨ��3 ��è3 ���� ����3

  //����
  if(1==bcData||bcData==2||bcData==3)
  {
    bcOutCadDataWin[1-1]= 6;
    bcOutCadDataWin[11-1]= 2;

  }
  //����
  else if(5==bcData||bcData==6||bcData==7)
  {
    bcOutCadDataWin[2-1]= 6;
    bcOutCadDataWin[10-1]= 2;

  }
  //����
  else if(8==bcData||bcData==9||bcData==10)
  {
    bcOutCadDataWin[3-1]= 8;
    bcOutCadDataWin[10-1]= 2;
  }
  //��ȸ
  else if(12==bcData||bcData==13||bcData==14)
  {
    bcOutCadDataWin[4-1]= 8;
    bcOutCadDataWin[10-1]= 2;
  }
  //��ӥ
  else if(15==bcData||bcData==16||bcData==17)
  {
    bcOutCadDataWin[5-1]= 12;
    bcOutCadDataWin[10-1]= 2;
  }
  //ʨ��
  else if(19==bcData||bcData==20||bcData==21)
  {
    bcOutCadDataWin[6-1]= 12;
    bcOutCadDataWin[11-1]= 2;
  }
  //��è
  else if(22==bcData||bcData==23||bcData==24)
  {
    bcOutCadDataWin[7-1]= 8;
    bcOutCadDataWin[11-1]= 2;
  }
  //����
  else if(26==bcData||bcData==27||bcData==28)
  {
    bcOutCadDataWin[8-1]= 8;
    bcOutCadDataWin[11-1]= 2;
  }
  //����
  else
  {
    //memset(bcOutCadDataWin,0,AREA_COUNT);

    //�ƽ�����
    if(bcData == 4)
    {
      bcOutCadDataWin[12 -1] =100;
    }
    //����
    else if(cbCardData == 18)
    {
      bcOutCadDataWin[9 -1] =24;
    }
    //ͨɱ
    else if(bcData == 11)
    {
      bcOutCadDataWin[11-1] = 255;
    }
    //ͨ��
    else if(cbCardData == 25)
    {
      bcOutCadDataWin[10-1] = 1;
    }
    else
    {
      AfxMessageBox(TEXT("�쳣"));
    }

  }

  return true;
}

//��С�Ƚ�
/*
cbNextCardData>cbFirstCardData  ����1
cbNextCardData<cbFirstCardData  ����-1
cbNextCardData==cbFirstCardData ����0
*/
//Multiple �Ƚϳ����ı���
int CGameLogic::CompareCard(const BYTE cbFirstCardData[], BYTE cbFirstCardCount,const BYTE cbNextCardData[], BYTE cbNextCardCount,BYTE &Multiple)
{
  //�Ϸ��ж�
  ASSERT(1==cbFirstCardCount && 1==cbNextCardCount);
  if(!(1==cbFirstCardCount && 1==cbNextCardCount))
  {
    return 0;
  }

  return true;
}

//��ȡ�Ƶ�
BYTE CGameLogic::GetCardListPip(const BYTE cbCardData[], BYTE cbCardCount)
{
  //��������
  BYTE cbPipCount=0;

  //��ȡ�Ƶ�
  BYTE cbCardValue=0;
  for(BYTE i=0; i<cbCardCount; i++)
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
  //�˿�����
  BYTE cbCardColor=GetCardColor(cbCardData);
  BYTE cbCardValue=GetCardValue(cbCardData);

  //ת����ֵ
  if(cbCardColor==0x04)
  {
    return cbCardValue+13+2;
  }
  return cbCardValue;

}
//�߼���С
BYTE CGameLogic::GetCardLogicValue(BYTE cbCardData)
{
  BYTE cbValue=GetCardValue(cbCardData);

  //��ȡ��ɫ
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

//�����˿�
void CGameLogic::SortCardList(BYTE cbCardData[], BYTE cbCardCount, BYTE cbSortType)
{
  //��Ŀ����
  if(cbCardCount==0)
  {
    return;
  }

  //ת����ֵ
  BYTE cbSortValue[CARD_COUNT];
  if(ST_VALUE==cbSortType)
  {
    for(BYTE i=0; i<cbCardCount; i++)
    {
      cbSortValue[i]=GetCardValue(cbCardData[i]);
    }
  }
  else
  {
    if(cbSortType==ST_NEW)
    {
      for(BYTE i=0; i<cbCardCount; i++)
      {
        cbSortValue[i]=GetCardNewValue(cbCardData[i]);
      }

    }
    else
    {
      for(BYTE i=0; i<cbCardCount; i++)
      {
        cbSortValue[i]=GetCardLogicValue(cbCardData[i]);
      }

    }

  }


  //�������
  bool bSorted=true;
  BYTE cbThreeCount,cbLast=cbCardCount-1;
  do
  {
    bSorted=true;
    for(BYTE i=0; i<cbLast; i++)
    {
      if((cbSortValue[i]<cbSortValue[i+1])||
         ((cbSortValue[i]==cbSortValue[i+1])&&(cbCardData[i]<cbCardData[i+1])))
      {
        //����λ��
        cbThreeCount=cbCardData[i];
        cbCardData[i]=cbCardData[i+1];
        cbCardData[i+1]=cbThreeCount;
        cbThreeCount=cbSortValue[i];
        cbSortValue[i]=cbSortValue[i+1];
        cbSortValue[i+1]=cbThreeCount;
        bSorted=false;
      }
    }
    cbLast--;
  }
  while(bSorted==false);

  return;
}

//////////////////////////////////////////////////////////////////////////

//��������
void CGameLogic::RandArray(INT nArrayOne[], INT nCountOne, INT nArrayTwo[], INT nCountTwo)
{
  ASSERT(nCountOne == nCountTwo);
  if(nCountTwo != nCountOne)
  {
    return;
  }

  DWORD wTick = GetTickCount();
  for(int i = 1; i < nCountOne; ++i)
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

void CGameLogic::RandListArray(BYTE cbBuffer[], BYTE cbBufferCount, bool conflict)
{
  //�ǶԳ�
  if(conflict == false)
  {
    BYTE cbControlArea[28] = { 1, 2, 3, 4,  5, 6, 7, 8, 9, 10,  11,  12,13,14, 15,16,17, 18,  19,20, 21, 22, 23, 24,  25, 26, 27, 28 };

    //����׼��
    BYTE cbRandArray[CountArray(cbControlArea)];
    CopyMemory(cbRandArray, cbControlArea, sizeof(cbControlArea));

    //�����˿�
    BYTE cbRandCount = 0;
    BYTE cbPosition = 0;
    do
    {
      cbPosition = (BYTE)(rand() + GetTickCount() * 2) % (CountArray(cbRandArray) - cbRandCount);
      cbBuffer[cbRandCount++] = cbRandArray[cbPosition];
      cbRandArray[cbPosition] = cbRandArray[CountArray(cbRandArray) - cbRandCount];
    }
    while(cbRandCount < cbBufferCount);
  }
  else // �Գ�
  {
    BYTE cbControlArea[24] = { 1, 2, 3, 5, 6, 7, 8, 9, 10,  12,13,14, 15,16,17,  19,20, 21, 22, 23, 24, 26, 27, 28 };

    //����׼��
    BYTE cbRandArray[CountArray(cbControlArea)];
    CopyMemory(cbRandArray, cbControlArea, sizeof(cbControlArea));

    //�����˿�
    BYTE cbRandCount = 0;
    BYTE cbPosition = 0;
    do
    {
      cbPosition = (BYTE)(rand() + GetTickCount() * 2) % (CountArray(cbRandArray) - cbRandCount);
      cbBuffer[cbRandCount++] = cbRandArray[cbPosition];
      cbRandArray[cbPosition] = cbRandArray[CountArray(cbRandArray) - cbRandCount];
    }
    while(cbRandCount < cbBufferCount);
  }


  return;
}
