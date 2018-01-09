#ifndef GOLD_CONTROL_HEAD_FILE
#define GOLD_CONTROL_HEAD_FILE

#pragma once

#include "Stdafx.h"

//////////////////////////////////////////////////////////////////////////

//��ҿ���
class CGoldControl : public CWnd
{
public:
  bool                m_bMingTag;           //���Ʊ�
  //��������
protected:
  LONGLONG              m_lAllGold;           //�ܱ���Ŀ
  LONGLONG              m_lGoldCount[3];        //�����Ŀ
  LONGLONG              m_lMaxGold;           //�����
  LONGLONG              m_lGoldCell[7];         //��Ԫ���
  LONGLONG              m_lMinGold;           //���ٽ��
  CPoint                m_BasicPoint;         //����λ��

  //λ�ñ���
protected:
  int                 m_nWidth;           //�ؼ����
  int                 m_nCellCount;         //��Ԫ��Ŀ
  int                 m_AppendWidth;          //���ӿ��

  //��Դ����
protected:
  CBitImage             m_ImageBack;          //������Դ
  CBitImage             m_ImageMoney;         //�����Դ
  CBitImage             m_ImageNumber;          //������Դ

public:
  CSkinButton           m_btMaxScore;           //���ť
  CSkinButton           m_btMinScore;           //����ť
  CSkinButton           m_btCancel;             //����ť
  CSkinButton           m_btConfirm;            //ȡ����ť

  //��������
public:
  //���캯��
  CGoldControl();
  //��������
  virtual ~CGoldControl();

  //���ܺ���
public:
  //��ȡ���
  LONGLONG GetGold()
  {
    return m_lAllGold;
  };
  //���ó���
  void SetGoldCount(LONGLONG lCellSocre);
  //���ý��
  void SetGold(LONGLONG lGold);
  //������ע
  void SetMaxGold(LONGLONG lMaxGold);
  //���Ƶ�Ԫ
  void SetMinGold(LONGLONG lMinGold);
  //����λ��
  void SetBasicPoint(int nXPos, int nYPos);

  //�ڲ�����
private:
  //�����ؼ�
  void RectifyControl();

  //��Ϣӳ��
protected:
  //�ػ�����
  afx_msg void OnPaint();
  //���������Ϣ
  afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
  //���ù��
  afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);

  DECLARE_MESSAGE_MAP()
public:
  afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
protected:
  virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
};


//////////////////////////////////////////////////////////////////////////

#endif
