#include "StdAfx.h"
#include ".\pngmyimage.h"

//���캯��
CPngMyImage::CPngMyImage(void)
{
  m_x = 0;
  m_y = 0;
  //m_w = 92;
  // m_h = 29;

  m_bEnable = false;
  m_bVisible = true;//Ĭ�Ͽɼ�
  m_bMouseDown = false;
  m_bMouseUp = false;
  m_bMouseOn = false;
  m_bActive = true;//Ĭ���ǻ״̬
}

//��������
CPngMyImage::~CPngMyImage(void)
{
}
//�趨�Ƿ�ɼ�
void CPngMyImage::SetBtnVisible(bool visible)
{
  m_bVisible = visible;
  return;
}
//�趨�Ƿ���Ч
void CPngMyImage::SetBtnEnable(bool enable)
{
  m_bEnable = enable;
  return;
}
//����Դ�м���ͼ��
BOOL CPngMyImage::ImageFromIDResource(UINT nID, LPCTSTR sTR, Image *&pImg)
{
  /*
  //HINSTANCE hInst = GetModuleHandle(CLIENT_DLL_NAME);//��dll��ʹ��
  HINSTANCE hInst = AfxGetResourceHandle();
  HRSRC hRsrc = ::FindResource (hInst,MAKEINTRESOURCE(nID),sTR); // type
  if (!hRsrc)
   return FALSE;

  // load resource into memory
  DWORD len = SizeofResource(hInst, hRsrc);
  BYTE* lpRsrc = (BYTE*)LoadResource(hInst, hRsrc);
  if (!lpRsrc)
   return FALSE;

  // Allocate global memory on which to create stream
  HGLOBAL m_hMem = GlobalAlloc(GMEM_FIXED, len);
  BYTE* pmem = (BYTE*)GlobalLock(m_hMem);
  memcpy(pmem,lpRsrc,len);
  GlobalUnlock(m_hMem);
  IStream* pstm;
  CreateStreamOnHGlobal(m_hMem,FALSE,&pstm);

  // load from stream
  pImg=Gdiplus::Image::FromStream(pstm);

  // free/release stuff
  pstm->Release();
  FreeResource(lpRsrc);
  GlobalFree(m_hMem);
  */
  return TRUE;
}


//������ԴID
void CPngMyImage::LoadPng(CWnd *pWnd,HINSTANCE hinstanc,LPCTSTR pszFileName)
{
  m_png.LoadImage(pWnd,hinstanc,pszFileName);


  //��ȡͼƬ��С
  m_w = m_png.GetWidth() / 5;
  m_h = m_png.GetHeight();

  return;
}

//��ʾ��ť
void CPngMyImage::Draw(CDC *pDC, int x, int y)
{
  m_pDC = pDC;
  m_x = x;
  m_y = y;
  //������ɼ�
  if(!m_bVisible)
  {
    return;
  }
  else
  {
    //�������Ч״̬
    if(!m_bEnable)
    {
      m_png.DrawImage(pDC,x,y,m_w,m_h,m_w*1,0,m_w,m_h);
      return;
    }
    //����״̬
    else if(m_bEnable && m_bMouseDown && m_bMouseOn)
    {
      m_png.DrawImage(pDC,x,y,m_w,m_h,m_w*4,0,m_w,m_h);

    }
    //����״̬
    else if(m_bEnable && m_bMouseOn)
    {
      m_png.DrawImage(pDC,x,y,m_w,m_h,m_w*3,0,m_w,m_h);
      return;
    }
    else
    {
      m_png.DrawImage(pDC,x,y,m_w,m_h,m_w*0,0,m_w,m_h);
      return;
    }
  }
}
//�����OnMouseMove���������
void CPngMyImage::MouseMove(CPoint point)
{
  //ĿǰΪ�˾���ֻ�Ǽ򵥵��ж��Ƿ��ھ��η�Χ��
  //�Ժ���Լ����͸������Ȼ�����������󽻼�
  if((point.x - m_x) < m_w
     && (point.x - m_x) > 0
     && (point.y - m_y) < m_h
     && (point.y - m_y) > 0
     && m_bVisible
     && m_bEnable
     /*&& m_bActive*/)
  {
    m_bMouseOn = true;
  }
  else
  {
    m_bMouseOn = false;
    m_bMouseDown = false;
    m_bMouseUp = false;
  }

  return;
}

//�����OnLButtonDown��������
void CPngMyImage::MouseDown(CPoint point)
{
  if(m_bMouseOn && m_bActive)
  {
    m_bMouseDown = true;
    return;
  }
  else
  {
    m_bMouseDown = false;
    return;
  }
}

//�����OnLButtonUp��������
void CPngMyImage::MouseUp(CPoint point)
{
  if(m_bMouseOn && m_bActive)
  {
    m_bMouseUp = true;
    m_bMouseDown = false;
    return;
  }
  else
  {
    m_bMouseUp = false;
    return;
  }
}
//����״̬
bool CPngMyImage::GetMouseDown()
{
  return m_bMouseDown;
}
//�ƶ�״̬
bool CPngMyImage::GetMouseMove()
{
  return m_bMouseOn;;
}
//����״̬
bool CPngMyImage::GetMouseUp()
{
  return m_bMouseUp;;
}
//��ť���
int CPngMyImage::GetWidth()
{
  return m_w;
}
//��ť�߶�
int CPngMyImage::GetHeight()
{
  return m_h;
}

BOOL CPngMyImage::GetWindowRect(LPRECT lpRect)
{
  lpRect->left=m_x;
  lpRect->top=m_y;
  lpRect->right=m_x+m_w;
  lpRect->bottom=m_y+m_h;
  return true;
}
