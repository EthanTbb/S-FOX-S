#ifndef SKIN_DIALOG_HEAD_FILE
#define SKIN_DIALOG_HEAD_FILE

#pragma once

#include "SkinEncircle.h"
#include "SkinControlHead.h"
#include "SkinRenderManager.h"
#include "SkinResourceManager.h"
#include "SkinDpi.h"

//////////////////////////////////////////////////////////////////////////////////
//类型声明
class CSkinDialogAttribute;
class CSkinLayered;
class CSkinDialog;

//类型声明
typedef CMap<HWND,HWND,CSkinLayered*,CSkinLayered*>		CSkinLayeredMap;

//////////////////////////////////////////////////////////////////////////////////

//窗口资源
class SKIN_CONTROL_CLASS CSkinDialogAttribute
{
	//颜色定义
public:
	COLORREF						m_crTitleText;						//标题颜色
	COLORREF						m_crTitleFrame;						//标题颜色
	COLORREF						m_crBackGround;						//背景颜色
	COLORREF						m_crControlText;					//控制颜色
	COLORREF						m_crLabelRound;						//标签环绕

	//资源变量
public:
	CFont                           m_Font;                             //控件字体
	CSize							m_SizeButton;						//按钮大小
	CBrush							m_brBackGround;						//背景画刷
	CBrush							m_brLabelRound;						//环绕画刷
	CEncirclePNG					m_PNGEncircleFrame;					//框架资源

	//位置变量
public:
	tagEncircleInfo					m_EncircleInfoView;					//视图环绕
	tagEncircleInfo					m_EncircleInfoFrame;				//框架环绕

	//函数定义
public:
	//构造函数
	CSkinDialogAttribute();
	//析构函数
	virtual ~CSkinDialogAttribute();

	//管理函数
public:
	//配置资源
	bool Initialization(CSkinRenderManager * pSkinRenderManager);
};

//////////////////////////////////////////////////////////////////////////////////

//分层窗口
class SKIN_CONTROL_CLASS CSkinLayered : public CWnd
{
	//变量定义
protected:	
	HWND							m_hwndControl;						//窗口句柄
	LONG							m_OriParentProc;					//窗口过程

	//静态变量
protected:
	static CSkinLayeredMap			m_SkinLayeredMap;					//分层列表

	//函数定义
public:
	//构造函数
	CSkinLayered();
	//析构函数
	virtual ~CSkinLayered();

	//功能函数
public:
	//创建窗口
	bool CreateLayered(HWND hWnd);
	//设置区域
	VOID InitLayeredArea(CDC * pDCImage, BYTE cbAlpha, CRect & rcUnLayered, CPoint & PointRound, bool bUnLayeredChild);
	//设置区域
	VOID InitLayeredArea(CPngImage & Image, BYTE cbAlpha, CRect & rcUnLayered, CPoint & PointRound, bool bUnLayeredChild);

	//消息函数
public:
	//关闭消息
	VOID OnClose();
	//设置焦点
	VOID OnSetFocus(CWnd* pOldWnd);

	//静态函数
protected:
	//枚举函数
	static BOOL CALLBACK EnumChildProc(HWND hWndChild, LPARAM lParam);
	//窗口过程
	static LRESULT CALLBACK ParentProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

//对话框类
class SKIN_CONTROL_CLASS CSkinDialog : public CFGuiDialog
{
	//标识变量
protected:
	bool							m_bInitDialog;						//初始标识
	bool							m_bLableRound;						//环绕标识

	//变量定义
protected:
	BYTE							m_cbButtonState[3];					//按钮状态
	CRect							m_rcRoundSpace;						//环绕间距

	//界面控件
protected:
	CLabelUI *						m_pLabelRound;						//环绕标签
	CButtonUI *						m_pButton[3];						//按钮位置

	//窗口变量
private:
	UINT                            m_nIDTemplate;                      //资源对象	
	CSkintDPI                       m_SkinDPI;                          //设置DPI
	CSkinLayered					m_SkinLayered;						//分层窗口

	//资源变量
public:
	static CSkinDialogAttribute		m_SkinAttribute;					//资源变量

	//函数定义
public:
	//构造函数
	CSkinDialog(UINT nIDTemplate, CWnd * pParentWnd=NULL);
	//析构函数
	virtual ~CSkinDialog();

    //接口函数
public:
	//初始控件
	virtual void InitControlUI();
	//消息提醒
	virtual void Notify(TNotifyUI &  msg);	

	//绘画接口
public:
	//结束绘画
	virtual void OnEndPaintWindow(HDC hDC);
	//开始绘画
	virtual void OnBeginPaintWindow(HDC hDC);

	//重载函数
public:
	//配置消息
	virtual BOOL OnInitDialog();
	//绘画标题
	virtual VOID OnDrawSystemTitle(CDC * pDC, INT nWdith, INT nHeight);
	//绘画消息
	virtual VOID OnDrawClientAreaBegin(CDC * pDC, INT nWidth, INT nHeight);
	//绘画消息
	virtual VOID OnDrawClientAreaEnd(CDC * pDC, INT nWidth, INT nHeight);

	//辅助函数
private:
	//调整资源
	VOID RectifyResource(INT nWidth, INT nHeight);
	//调整按钮
	VOID CaleTitleButton(INT nWidth, INT nHeight);

	//绘画函数
private:
	//绘画控件
	VOID DrawControlView(CDC * pDC, INT nWdith, INT nHeight);

	//消息映射
protected:
	//位置消息
	VOID OnSize(UINT nType, INT cx, INT cy);
	//控件颜色
	HBRUSH OnCtlColor(CDC * pDC, CWnd * pWnd, UINT nCtlColor);

	//自定消息
protected:
	//标题消息
	LRESULT	OnSetTextMesage(WPARAM wParam, LPARAM lParam);	

	//辅助函数
public:
	//枚举函数
	static BOOL CALLBACK EnumChildProc(HWND hWndChild, LPARAM lParam);
	//设置字体
	static VOID CALLBACK SetWndFont(CWnd *pWnd, CFont *pFont);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

#endif