#ifndef FGUIMFC_WINDOW_HEAD_FILE
#define FGUIMFC_WINDOW_HEAD_FILE
#pragma once

//引入文件
#include "FGuimfcFactoryHead.h"

///////////////////////////////////////////////////////////////////////////////////////////
//DUI窗口
class FGUIMFC_FACTORY_CLASS CFGuiWnd : public CWnd, public INotifyUI, public IPaintNotifyUI, public IMessageFilterUI, public IDialogBuilderCallback
{
	//变量定义
protected:
	CPaintManagerUI						m_PaintManager;						//绘画管理

	//函数定义
public:
	//构造函数
	CFGuiWnd(void);
	//析构函数
	virtual ~CFGuiWnd(void);

    //接口函数
public:
	//消息提醒
	virtual void Notify(TNotifyUI &  msg) { return;}
	//创建控件
	virtual CControlUI* CreateControl(LPCTSTR pstrClass) { return NULL; }
	//设置属性
	virtual void SetCustomAttribute(LPCTSTR pszName,LPCTSTR pszValue) { return; }
	//消息处理
    virtual LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
	//处理消息
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

	//绘画接口
public:
	//开始绘画
	virtual void OnBeginPaintWindow(HDC hDC) { return; }
	//结束绘画
	virtual void OnEndPaintWindow(HDC hDC) { return; }

	//继承函数
public:
	//皮肤名称
	virtual LPCTSTR GetSkinFile() { return TEXT(""); }
	//初始控件
	virtual void InitControlUI() { return; }

	//响应函数
public:
	//按钮测试
	virtual LRESULT HandleNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	//左键按下
	virtual LRESULT HandleLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	//左键双击
	virtual LRESULT HandleLButtonDbClick(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	//自定义消息
	virtual LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	//改变大小
	virtual LRESULT HandleOnSizeMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	//辅助函数
protected:
	//点击测试
	bool DoCaptionHitTest(POINT Point);

	//辅助函数
protected:
	//获取对象
	CControlUI * GetControlByName(LPCTSTR pszControlName);
	//功能函数
public:
	//标题区域
	CRect GetCaptionRect() { return CRect(m_PaintManager.GetCaptionRect()); }
	//尺寸边框
	CRect GetSizeBox() { return CRect(m_PaintManager.GetSizeBox()); }
	//圆角尺寸
	CSize GetRoundCornor() { return CSize(m_PaintManager.GetRoundCorner()); }	
	
	//继承函数
protected:
	//消息转换
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//窗口过程
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);	 

	//消息函数
public:
	//创建消息
    int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//销毁消息
	void OnNcDestroy();


	DECLARE_MESSAGE_MAP()
};

///////////////////////////////////////////////////////////////////////////////////////////
//DUI窗口
class FGUIMFC_FACTORY_CLASS CFGuiFrameWnd : public CFrameWnd,public INotifyUI,public IPaintNotifyUI,public IMessageFilterUI,public IDialogBuilderCallback
{
	//变量定义
protected:
	CPaintManagerUI						m_PaintManager;						//绘画管理

	//函数定义
public:
	//构造函数
	CFGuiFrameWnd(void);
	//析构函数
	virtual ~CFGuiFrameWnd(void);

    //接口函数
public:
	//消息提醒
	virtual void Notify(TNotifyUI &  msg) { return;}
	//创建控件
	virtual CControlUI* CreateControl(LPCTSTR pstrClass) { return NULL; }
	//设置属性
	virtual void SetCustomAttribute(LPCTSTR pszName,LPCTSTR pszValue) { return; }
	//消息处理
    virtual LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
	//处理消息
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

	//绘画接口
public:
	//开始绘画
	virtual void OnBeginPaintWindow(HDC hDC) { return; }
	//结束绘画
	virtual void OnEndPaintWindow(HDC hDC) { return; }

	//继承函数
public:
	//皮肤名称
	virtual LPCTSTR GetSkinFile() { return TEXT(""); }
	//初始控件
	virtual void InitControlUI() { return; }

	//响应函数
public:
	//按钮测试
	virtual LRESULT HandleNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	//左键按下
	virtual LRESULT HandleLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	//左键双击
	virtual LRESULT HandleLButtonDbClick(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	//自定义消息
	virtual LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	//改变大小
	virtual LRESULT HandleOnSizeMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	//辅助函数
protected:
	//点击测试
	bool DoCaptionHitTest(POINT Point);

	//辅助函数
protected:
	//获取对象
	CControlUI * GetControlByName(LPCTSTR pszControlName);

	//功能函数
public:
	//标题区域
	CRect GetCaptionRect() { return CRect(m_PaintManager.GetCaptionRect()); }
	//尺寸边框
	CRect GetSizeBox() { return CRect(m_PaintManager.GetSizeBox()); }
	//圆角尺寸
	CSize GetRoundCornor() { return CSize(m_PaintManager.GetRoundCorner()); }
	
	//继承函数
protected:
	//消息转换
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//窗口过程
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);	 

	//消息函数
public:
	//创建消息
    int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//销毁消息
	void OnNcDestroy();

	DECLARE_MESSAGE_MAP()
};

///////////////////////////////////////////////////////////////////////////////////////////
//DUI对话框
class FGUIMFC_FACTORY_CLASS CFGuiDialog : public CDialog,public INotifyUI,public IPaintNotifyUI, public IMessageFilterUI,public IDialogBuilderCallback,public IWindowCloseCallBack
{
	//变量定义
protected:
	CPaintManagerUI						m_PaintManager;						//绘画管理

	//函数定义
public:
	//构造函数
	CFGuiDialog(void);
	//构造函数
	explicit CFGuiDialog(LPCTSTR lpszTemplateName, CWnd* pParentWnd = NULL);
	//构造函数
	explicit CFGuiDialog(UINT nIDTemplate, CWnd* pParentWnd = NULL);
	//析构函数
	virtual ~CFGuiDialog(void);
	//模态对话
	virtual INT_PTR DoModal();

    //接口函数
public:
	//消息提醒
	virtual void Notify(TNotifyUI &  msg) { return;}
	//创建控件
	virtual CControlUI* CreateControl(LPCTSTR pstrClass) { return NULL; }
	//设置属性
	virtual void SetCustomAttribute(LPCTSTR pszName,LPCTSTR pszValue) { return; }
	//消息处理
    virtual LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
	//处理消息
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	//关闭窗口
	virtual LRESULT CloseWindow(); 
	//绘画接口
public:
	//开始绘画
	virtual void OnBeginPaintWindow(HDC hDC) { return; }
	//结束绘画
	virtual void OnEndPaintWindow(HDC hDC) { return; }

	//响应函数
public:
	//按钮测试
	virtual LRESULT HandleNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	//左键按下
	virtual LRESULT HandleLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	//左键双击
	virtual LRESULT HandleLButtonDbClick(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	//自定义消息
	virtual LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	//改变大小
	virtual LRESULT HandleOnSizeMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	//辅助函数
protected:
	//点击测试
	bool DoCaptionHitTest(POINT Point);

	//辅助函数
protected:
	//获取对象
	CControlUI * GetControlByName(LPCTSTR pszControlName);

	//功能函数
public:
	//标题区域
	CRect GetCaptionRect() { return CRect(m_PaintManager.GetCaptionRect()); }
	//尺寸边框
	CRect GetSizeBox() { return CRect(m_PaintManager.GetSizeBox()); }
	//圆角尺寸
	CSize GetRoundCornor() { return CSize(m_PaintManager.GetRoundCorner()); }

	//继承函数
public:
	//皮肤名称
	virtual LPCTSTR GetSkinFile() { return TEXT(""); }
	//初始控件
	virtual void InitControlUI() { return; }

	//继承函数
protected:
	//消息转换
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//窗口过程
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);	 

	//消息函数
public:
	//创建消息
    int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//销毁消息
	void OnNcDestroy();

	DECLARE_MESSAGE_MAP()	
};
///////////////////////////////////////////////////////////////////////////////////////////
#endif