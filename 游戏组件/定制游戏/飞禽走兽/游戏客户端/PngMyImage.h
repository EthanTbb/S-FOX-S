#ifndef __H_CPNGMYIMAGE
#define __H_CPNGMYIMAGE
#include "StdAfx.h"
#include <GdiPlus.h>
using namespace Gdiplus;



//////////////////我的CPngMyImage按钮类//////////////////////////////////////

class CPngMyImage
{
  //函数定义
public:
  //构造函数
  CPngMyImage(void);
  //析构函数
  ~CPngMyImage(void);

  //变量定义
public:
  CDC* m_pDC;                   //绘图DC
  CPngImageEx  m_png;                 //按钮图片
  int m_x;                    //显示的x坐标
  int m_y;                    //显示的y坐标
  int m_w;                    //按钮宽度
  int m_h;                    //按钮高度

  //状态变量
private:
  bool m_bEnable;                 //是否有效
  bool m_bVisible;                //是否可见
  bool m_bMouseOn;                //鼠标是否在其上面
  bool m_bMouseDown;                //鼠标是否按下
  bool m_bMouseUp;                //鼠标是否起来
  bool m_bActive;                 //是否是活动状态，以便屏蔽功能

  //功能函数
public:
  void LoadPng(CWnd *pWnd,HINSTANCE hinstanc,LPCTSTR pszFileName);  //加载资源
  void SetBtnVisible(bool visible);       //设定是否可见
  void SetBtnEnable(bool enable);         //设定是否有效
  void Draw(CDC *pDC, int x, int y);        //显示按钮
  BOOL ImageFromIDResource(UINT nID,       //资源ID
                           LPCTSTR sTR,                //资源类型
                           Image * &pImg);               //IMG对象

  int GetWidth();                 //按钮宽度
  int GetHeight();                //按钮高度
  BOOL GetWindowRect(LPRECT lpRect);        //获取窗口区域
  //鼠标事件
public:
  void MouseMove(CPoint point);         //添加在OnMouseMove函数里面的
  void MouseDown(CPoint point);         //添加在OnLButtonDown函数里面
  void MouseUp(CPoint point);           //添加在OnLButtonUp函数里面

  //鼠标状态
public:
  bool GetMouseDown();              //按下状态
  bool GetMouseMove();              //移动状态
  bool GetMouseUp();                //起来状态
};

//////////////////我的CPngMyImage按钮类//////////////////////////////////////
#endif //__H_CPNGMYIMAGE
