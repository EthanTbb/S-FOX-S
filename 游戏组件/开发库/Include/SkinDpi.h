
#ifndef SKIN_SKINDPI_HEAD_FILE
#define SKIN_SKINDPI_HEAD_FILE

#pragma once

#include "SkinControlHead.h"

//////////////////////////////////////////////////////////////////////////////////
class SKIN_CONTROL_CLASS CSkintDPI
{
	//变量定义
protected:
    HINSTANCE                            m_Inst;
    HWND                                 m_Hwnd;
    HFONT                                m_Font;
	HFONT                                m_Oldfont;
	LPCTSTR                              m_lpszTemplateName;

	//函数定义
public:
	//构造函数
    CSkintDPI();
	//析构函数
	virtual ~CSkintDPI();

	//功能函数
public:
	//附加函数
    void Attach(HINSTANCE inst,HWND dlg,LPCTSTR lpszTemplateName,double dpi);
	//附加函数
	void Attach(HINSTANCE inst,HWND dlg,UINT IDD,double dpi);
	//分离函数
    void Detach();
};

#endif
