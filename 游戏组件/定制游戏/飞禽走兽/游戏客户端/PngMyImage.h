#ifndef __H_CPNGMYIMAGE
#define __H_CPNGMYIMAGE
#include "StdAfx.h"
#include <GdiPlus.h>
using namespace Gdiplus;



//////////////////�ҵ�CPngMyImage��ť��//////////////////////////////////////

class CPngMyImage
{
  //��������
public:
  //���캯��
  CPngMyImage(void);
  //��������
  ~CPngMyImage(void);

  //��������
public:
  CDC* m_pDC;                   //��ͼDC
  CPngImageEx  m_png;                 //��ťͼƬ
  int m_x;                    //��ʾ��x����
  int m_y;                    //��ʾ��y����
  int m_w;                    //��ť���
  int m_h;                    //��ť�߶�

  //״̬����
private:
  bool m_bEnable;                 //�Ƿ���Ч
  bool m_bVisible;                //�Ƿ�ɼ�
  bool m_bMouseOn;                //����Ƿ���������
  bool m_bMouseDown;                //����Ƿ���
  bool m_bMouseUp;                //����Ƿ�����
  bool m_bActive;                 //�Ƿ��ǻ״̬���Ա����ι���

  //���ܺ���
public:
  void LoadPng(CWnd *pWnd,HINSTANCE hinstanc,LPCTSTR pszFileName);  //������Դ
  void SetBtnVisible(bool visible);       //�趨�Ƿ�ɼ�
  void SetBtnEnable(bool enable);         //�趨�Ƿ���Ч
  void Draw(CDC *pDC, int x, int y);        //��ʾ��ť
  BOOL ImageFromIDResource(UINT nID,       //��ԴID
                           LPCTSTR sTR,                //��Դ����
                           Image * &pImg);               //IMG����

  int GetWidth();                 //��ť���
  int GetHeight();                //��ť�߶�
  BOOL GetWindowRect(LPRECT lpRect);        //��ȡ��������
  //����¼�
public:
  void MouseMove(CPoint point);         //�����OnMouseMove���������
  void MouseDown(CPoint point);         //�����OnLButtonDown��������
  void MouseUp(CPoint point);           //�����OnLButtonUp��������

  //���״̬
public:
  bool GetMouseDown();              //����״̬
  bool GetMouseMove();              //�ƶ�״̬
  bool GetMouseUp();                //����״̬
};

//////////////////�ҵ�CPngMyImage��ť��//////////////////////////////////////
#endif //__H_CPNGMYIMAGE
