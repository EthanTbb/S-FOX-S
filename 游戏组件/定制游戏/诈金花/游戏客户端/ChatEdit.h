#pragma once
#include "Stdafx.h"
#include "SkinVerticleScrollbar.h"

//////////////////////////////////////////////////////////////////////////////////

class CChatEdit : public CRichEditCtrl
{
  //DECLARE_DYNAMIC(CChatEdit)

  //成员变量
private:
  CFont             m_font;               //文本字体
  COLORREF            m_crTextColor;            //文本颜色

  //组件变量
protected:
  CSkinVerticleScrollbar      m_SkinVerticleScrollbar;      //滚动条类

  //函数定义
public:
  //构造函数
  CChatEdit();
  //析构函数
  ~CChatEdit();

  //功能函数
public:
  //初始化函数
  VOID Initialization();
  //调整控件
  VOID RectifyControl(INT nWidth, INT nHeigth);
  //设置字体颜色
  VOID SetTextColor(COLORREF rcTextColor);
  //设置字体
  VOID SetTextFont(const LOGFONT &lfTextFont);
  //设置隐藏
  VOID SetHideWindow(bool bHide);
  //获取字体颜色
  COLORREF GetTextColor(void)
  {
    return m_crTextColor;
  }
  //获取当前字体
  BOOL GetTextFont(LOGFONT &lfTextFont);

protected:
  virtual void PreSubclassWindow();

public:
  afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
  afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
  afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp);
  afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
  afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
  DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////
