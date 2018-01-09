#ifndef JETTON_CONTROL_HEAD_FILE
#define JETTON_CONTROL_HEAD_FILE

#pragma once

#include "Stdafx.h"

//////////////////////////////////////////////////////////////////////////

//���Զ���
#define MAX_JETTON_INDEX        14                  //������Ŀ

//��������
struct tagJetPending
{
  LONGLONG  lScore;                         //�����
  CPoint    ptJetton;                       //����λ��
};

//////////////////////////////////////////////////////////////////////////

//����ؼ�
class CJettonControl
{
  //��������
protected:
  static LONGLONG         m_lJetonIndex[MAX_JETTON_INDEX];  //��������

  LONGLONG            m_lScore;             //������Ŀ
  CPoint              m_BenchmarkPos;           //��׼λ��

  //λͼ����
protected:
  CPngImage           m_PngJetton;            //����λͼ
  INT               m_nJettonWidth;           //�����
  INT               m_nJettonHeight;          //�����

  //λ�ñ���
protected:
  CWHArray<WORD>      m_arJetIndex;           //�滭����
  CWHArray<CPoint>      m_arJetExcusions;         //�滭λ��


  //��������
protected:
  CWHArray<tagJetPending> m_arJetPending;           //��������

  CWHArray<WORD>      m_arIndexPending;         //������������
  CWHArray<CPoint>      m_arExcusionsPend;          //ƫ��
  CWHArray<INT>       m_arStepCount;            //����
  CWHArray<INT>       m_arXStep;              //X����
  CWHArray<INT>       m_arYStep;              //Y����
  LONGLONG            m_lScorePending;          //���������
  CRect             m_rcMove;             //�ƶ���������
  CRect             m_rcDraw;             //�ƶ��滭����

  //��������
protected:
  //�����ƶ�����
  VOID    ComputeMoveRect();
  //���춯��
  BOOL    ConstructCartoon();
  //��ȡ��������
  BOOL    DistillJetIndex(LONGLONG lScore, INT_PTR &nJetIndex);
  //ѹ������,��Сֵ������ϳɴ�ֵ����
  VOID    CompactJetIndex();

public:
  //���캯��
  CJettonControl();
  //��������
  virtual ~CJettonControl();

  //���ܺ���
public:
  //��ȡ����
  LONGLONG  GetScore()
  {
    return m_lScore;
  }
  //���ÿؼ�
  VOID    ResetControl();
  //�ӳ���
  VOID    AddScore(LONGLONG lScore, CPoint ptFrom = CPoint(0,0));
  //�Ƴ�����
  VOID    RemoveScore(LONGLONG lScore, CPoint ptTo = CPoint(0,0));
  //�Ƴ����г���
  VOID    RemoveAllScore(CPoint ptTo = CPoint(0,0));

  //���ܺ���
public:
  //��ʼ�ƶ�����
  BOOL    BeginMoveJettons();
  //�ƶ�����
  BOOL    PlayMoveJettons();
  //ֹͣ�ƶ�����
  BOOL    FinishMoveJettons();
  //�Ƿ����ƶ�
  BOOL    IsPlayMoving();

  //λ�ú���
public:
  //��׼λ��
  VOID    SetBenchmarkPos(INT nXPos, INT nYPos);
  //��ȡλ��
  CPoint    GetBenchmarkPos()
  {
    return m_BenchmarkPos;
  }
  //��ȡ��������
  VOID    GetDrawRect(CRect &rc);

  //���ƺ���
public:
  //�滭�ؼ�
  VOID    DrawJettonControl(CDC * pDC);
};

//////////////////////////////////////////////////////////////////////////

#endif