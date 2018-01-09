#ifndef DLG_TIP_CONTROL_HEAD_FILE
#define DLG_TIP_CONTROL_HEAD_FILE

#pragma once

#include "ShareControlHead.h"

//////////////////////////////////////////////////////////////////////////////////
//类型声明
class CWndTipControl;

//////////////////////////////////////////////////////////////////////////////////
//类型定义
typedef CMap<HWND,HWND,CWndTipControl *,CWndTipControl *>				CTipControlMap;

//类型定义
typedef BOOL (WINAPI * LPUpdateLayeredWindow)(HWND,HDC,POINT*,SIZE*,HDC,POINT*,COLORREF,BLENDFUNCTION*,DWORD);          

//////////////////////////////////////////////////////////////////////////////////

//对话框类
class SHARE_CONTROL_CLASS CWndTipControl
{
	//核心变量
protected:
	HWND							m_hWnd;								//窗口句柄

	//资源变量
protected:
	HINSTANCE						m_hInstance;						//资源句柄
	TCHAR							m_szImageGround[64];				//背景图片

	//变量定义
protected:
	bool							m_bTrackMouse;						//跟踪标识
	INT								m_nRelayTime;						//延时时长
	CRect							m_rcInvalidArea;					//有效区域
	
	//变量定义
protected:
	BLENDFUNCTION					m_Blend;							//混合结构	
	HINSTANCE						m_hFuncInst;						//函数句柄							

	//变量定义
protected:
	LPUpdateLayeredWindow			m_pUpdateLayeredWindow;				//更新函数

	//资源变量
public:	
	static CTipControlMap			m_TipControlMap;					//控件映射

	//函数定义
public:
	//构造函数
	CWndTipControl();
	//析构函数
	virtual ~CWndTipControl();

	//功能函数
public:
	//隐藏窗口
	VOID QuickHidwWindow();
	//设置延时
	VOID SetRelayTime(INT nRelayTime);	
	//创建控件
	VOID CreateTipControl(INT nWidth,INT nHeight,INT nAlpha);	
	//显示窗口
	VOID RelayShowWindow(INT nXPos,INT YPos,CRect rcInvalidArea);
	//设置资源
	VOID SetControlResource(HINSTANCE hInstanceRes,LPCTSTR pszImageGround);

	//重载函数
public:
	//显示探测
	virtual BOOL DetectIsShowWindow();
	//隐藏探测
	virtual BOOL DetectIsHideWindow();
	//绘画消息
	virtual VOID OnDrawClientArea(CDC * pDC, INT nWidth, INT nHeight);

	//绘画函数
private:
	//更新控件
	BOOL UpdateTipControl();
	//绘画控件
	VOID DrawControlView(HDC hDC, INT nWdith, INT nHeight);
	
	//消息映射
protected:	
	//时间消息
	VOID OnTimer(UINT_PTR nIDEvent);
	//鼠标消息
	VOID OnMouseMove(UINT nFlags, CPoint Point);
	//鼠标离开
	LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);

	//静态函数
protected:
	//窗口过程
	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};

//////////////////////////////////////////////////////////////////////////////////

#endif