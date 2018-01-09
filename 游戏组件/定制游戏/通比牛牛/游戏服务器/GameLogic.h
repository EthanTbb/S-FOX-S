#ifndef GAME_LOGIC_HEAD_FILE
#define GAME_LOGIC_HEAD_FILE

#pragma once

#include "Stdafx.h"

//////////////////////////////////////////////////////////////////////////
//�궨��

//��ֵ����
#define LOGIC_MASK_COLOR      0xF0                //��ɫ����
#define LOGIC_MASK_VALUE      0x0F                //��ֵ����

//�˿�����
#define OX_VALUE_0          0                 //��ţ����
#define OX_VALUE_BIG_0        1                 //��ţ����
#define OX_VALUE_1          2                 //ţһ����
#define OX_VALUE_BIG_1        3                 //ţһ����
#define OX_VALUE_2          4                 //ţ������
#define OX_VALUE_BIG_2        5                 //ţ������
#define OX_VALUE_3          6                 //ţ������
#define OX_VALUE_BIG_3        7                 //ţ������
#define OX_VALUE_4          8                 //ţ������
#define OX_VALUE_BIG_4        9                 //ţ������
#define OX_VALUE_5          10                  //ţ������
#define OX_VALUE_BIG_5        11                  //ţ������
#define OX_VALUE_6          12                  //ţ������
#define OX_VALUE_BIG_6        13                  //ţ������
#define OX_VALUE_7          14                  //ţ������
#define OX_VALUE_BIG_7        15                  //ţ������
#define OX_VALUE_8          16                  //ţ������
#define OX_VALUE_BIG_8        17                  //ţ������
#define OX_VALUE_9          18                  //ţ������
#define OX_VALUE_BIG_9        19                  //ţ������
#define OX_VALUE_10         20                  //ţţ����
#define OX_VALUE_BIG_10       21                  //ţţ����
#define OX_FOUR_KING        22                  //�Ļ�ţ��
#define OX_FIVE_KING        23                  //�廨ţ��

//////////////////////////////////////////////////////////////////////////

//�����ṹ
struct tagAnalyseResult
{
  BYTE              cbFourCount;            //������Ŀ
  BYTE              cbThreeCount;           //������Ŀ
  BYTE              cbDoubleCount;            //������Ŀ
  BYTE              cbSignedCount;            //������Ŀ
  BYTE              cbFourLogicVolue[1];        //�����б�
  BYTE              cbThreeLogicVolue[1];       //�����б�
  BYTE              cbDoubleLogicVolue[2];        //�����б�
  BYTE              cbSignedLogicVolue[5];        //�����б�
  BYTE              cbFourCardData[MAX_COUNT];      //�����б�
  BYTE              cbThreeCardData[MAX_COUNT];     //�����б�
  BYTE              cbDoubleCardData[MAX_COUNT];    //�����б�
  BYTE              cbSignedCardData[MAX_COUNT];    //������Ŀ
};



static CString GetFileDialogPath()
{
  CString strFileDlgPath;
  TCHAR szModuleDirectory[MAX_PATH];  //ģ��Ŀ¼
  GetModuleFileName(AfxGetInstanceHandle(),szModuleDirectory,sizeof(szModuleDirectory));
  int nModuleLen=lstrlen(szModuleDirectory);
  int nProcessLen=lstrlen(AfxGetApp()->m_pszExeName)+lstrlen(TEXT(".EXE")) + 1;
  if(nModuleLen<=nProcessLen)
  {
    return TEXT("");
  }
  szModuleDirectory[nModuleLen-nProcessLen]=0;
  strFileDlgPath = szModuleDirectory;
  return strFileDlgPath;
}

static void NcaTextOut(CString strInfo)
{
  CString strName = GetFileDialogPath()+  "\\ͨ��ţţ.log";

  CTime time = CTime::GetCurrentTime() ;
  CString strTime ;
  strTime.Format(TEXT("%d-%d-%d %d:%d:%d") ,
                 time.GetYear() ,
                 time.GetMonth() ,
                 time.GetDay() ,
                 time.GetHour() ,
                 time.GetMinute() ,
                 time.GetSecond()
                );
  CString strMsg;
  strMsg.Format(TEXT("%s %s\r\n"),strTime, strInfo);

  CFile fLog;
  if(fLog.Open(strName, CFile::modeCreate|CFile::modeNoTruncate|CFile::modeReadWrite))
  {

    fLog.SeekToEnd();
    int strLength=strMsg.GetLength();
#ifdef _UNICODE
    BYTE bom[2] = {0xFF, 0xfe};
    fLog.Write(bom,sizeof(BYTE)*2);
    strLength*=2;
#endif
    fLog.Write((LPCTSTR)strMsg,strLength);
    fLog.Close();
  }

}

//////////////////////////////////////////////////////////////////////////

//��Ϸ�߼���
class CGameLogic
{
  //��������
private:
  static BYTE           m_cbCardListData[54];       //�˿˶���

  //��������
public:
  //���캯��
  CGameLogic();
  //��������
  virtual ~CGameLogic();

  //���ͺ���
public:
  //��ȡ����
  BYTE GetCardType(BYTE cbCardData[], BYTE cbCardCount);
  //��ȡ��ֵ
  BYTE GetCardValue(BYTE cbCardData)
  {
    return cbCardData&LOGIC_MASK_VALUE;
  }
  //��ȡ��ɫ
  BYTE GetCardColor(BYTE cbCardData)
  {
    return cbCardData&LOGIC_MASK_COLOR;
  }
  //��ȡ����
  BYTE GetTimes(BYTE cbCardData[], BYTE cbCardCount);
  //��ȡţţ
  bool GetOxCard(BYTE cbCardData[], BYTE cbCardCount);
  //��ȡ����
  bool IsIntValue(BYTE cbCardData[], BYTE cbCardCount);

  //���ƺ���
public:
  //�����˿�
  void SortCardList(BYTE cbCardData[], BYTE cbCardCount);
  //�����˿�
  void RandCardList(BYTE cbCardBuffer[], BYTE cbBufferCount);

  //���ܺ���
public:
  //�߼���ֵ
  BYTE GetCardLogicValue(BYTE cbCardData);
  //�Ա��˿�
  bool CompareCard(BYTE cbFirstData[], BYTE cbNextData[], BYTE cbCardCount,BOOL FirstOX,BOOL NextOX);
};

//////////////////////////////////////////////////////////////////////////

#endif
