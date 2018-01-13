#include "StdAfx.h"
#include ".\pngmyimage.h"

//构造函数
CPngMyImage::CPngMyImage(void)
{
  m_x = 0;
  m_y = 0;
  //m_w = 92;
  // m_h = 29;

  m_bEnable = false;
  m_bVisible = true;//默认可见
  m_bMouseDown = false;
  m_bMouseUp = false;
  m_bMouseOn = false;
  m_bActive = true;//默认是活动状态
}

//析构函数
CPngMyImage::~CPngMyImage(void)
{
}
//设定是否可见
void CPngMyImage::SetBtnVisible(bool visible)
{
  m_bVisible = visible;
  return;
}
//设定是否有效
void CPngMyImage::SetBtnEnable(bool enable)
{
  m_bEnable = enable;
  return;
}
//从资源中加载图像
BOOL CPngMyImage::ImageFromIDResource(UINT nID, LPCTSTR sTR, Image *&pImg)
{
  /*
  //HINSTANCE hInst = GetModuleHandle(CLIENT_DLL_NAME);//在dll中使用
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


//加载资源ID
void CPngMyImage::LoadPng(CWnd *pWnd,HINSTANCE hinstanc,LPCTSTR pszFileName)
{
  m_png.LoadImage(pWnd,hinstanc,pszFileName);


  //获取图片大小
  m_w = m_png.GetWidth() / 5;
  m_h = m_png.GetHeight();

  return;
}

//显示按钮
void CPngMyImage::Draw(CDC *pDC, int x, int y)
{
  m_pDC = pDC;
  m_x = x;
  m_y = y;
  //如果不可见
  if(!m_bVisible)
  {
    return;
  }
  else
  {
    //如果是无效状态
    if(!m_bEnable)
    {
      m_png.DrawImage(pDC,x,y,m_w,m_h,m_w*1,0,m_w,m_h);
      return;
    }
    //按下状态
    else if(m_bEnable && m_bMouseDown && m_bMouseOn)
    {
      m_png.DrawImage(pDC,x,y,m_w,m_h,m_w*4,0,m_w,m_h);

    }
    //高亮状态
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
//添加在OnMouseMove函数里面的
void CPngMyImage::MouseMove(CPoint point)
{
  //目前为了精简，只是简单的判断是否在矩形范围内
  //以后可以计算非透明区域，然后和这个矩形求交集
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

//添加在OnLButtonDown函数里面
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

//添加在OnLButtonUp函数里面
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
//按下状态
bool CPngMyImage::GetMouseDown()
{
  return m_bMouseDown;
}
//移动状态
bool CPngMyImage::GetMouseMove()
{
  return m_bMouseOn;;
}
//起来状态
bool CPngMyImage::GetMouseUp()
{
  return m_bMouseUp;;
}
//按钮宽度
int CPngMyImage::GetWidth()
{
  return m_w;
}
//按钮高度
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
