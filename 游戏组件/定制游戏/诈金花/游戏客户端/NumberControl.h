#ifndef NUMBER_CONTROL_HEAD_FILE
#define NUMBER_CONTROL_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "CardControl.h"

//////////////////////////////////////////////////////////////////////////

//���ֹ����ؼ�
class CNumberControl
{
  //�ؼ�����
protected:
  CPoint          m_BenchmarkPos;             //��׼λ��
  enXCollocateMode    m_XCollocateMode;           //��ʾģʽ
  CRect         m_rcDraw;               //���»滭����

  //���ֱ���
  LONGLONG        m_lScore;               //��������

  //��������
  INT           m_nYExcusion;             //ƫ��
  INT_PTR         m_nScrollIndex;             //��������
  INT           m_nScrollCount;             //��������
  CWHArray<BYTE>      m_arScoreNum;             //��������
  BOOL          m_bMoving;                //������ʶ

  //λͼ����
protected:
  CPngImage       m_PngScoreNum;              //��������ͼ
  CPngImage       m_PngScoreBack;             //��������ͼ

public:
  //���캯��
  CNumberControl();
  //��������
  virtual ~CNumberControl();

  //���ܺ���
public:
  //���û�׼λ��
  VOID    SetBencbmarkPos(INT nXPos, INT nYPos, enXCollocateMode XCollocateMode);
  //��������
  VOID    SetScore(LONGLONG lScore);
  //��ʼ����
  BOOL    BeginScrollNumber();
  //��������
  BOOL    PlayScrollNumber();
  //ֹͣ����
  BOOL    FinishScrollNumber();
  //���ÿؼ�
  VOID    ResetControl();
  //��ȡλ��
  CPoint    GetBenchMarkPos()
  {
    return m_BenchmarkPos;
  }
  //���뷽ʽ
  BYTE    GetXCollocateMode()
  {
    return m_XCollocateMode;
  };

  //�滭����
public:
  //�滭�ؼ�
  VOID    DrawNumberControl(CDC *pDC);
  //��ȡ���»滭����
  VOID    GetDrawRect(CRect &rc);
  //��ȡ����
  LONGLONG  GetScore()
  {
    return m_lScore;
  };

  //��������
protected:
  //��ȡԭ��
  VOID GetOriginPoint(CPoint & OriginPoint);
};

#endif

//////////////////////////////////////////////////////////////////////////