#ifndef DLG_TRUMPETITEM_HEAD
#define DLG_TRUMPETITEM_HEAD

#pragma once


//////////////////////////////////////////////////////////////////////////
//喇叭道具
class  CDlgTrumpet : public CFGuiDialog,public IExpressionSink
{
	//变量定义
protected:
	int								m_nMaxChar;						//最大个数
	WORD                            m_wRadioSelected;               //单选按钮  
	COLORREF						m_crChatTX;						//字体颜色
	CString							m_strInuptChat;					//输入信息
	CString							m_strPropertyInfo;				//道具信息
	DWORD							m_dwTrumpetCount;
	DWORD							m_dwTyphonCount;

	//控件变量
protected:
	CEdit							m_InputChat;					//聊天框
	CExpressionControl *			m_pExpressionControl;			//表情窗口	

	//接口变量
protected:
	IClientUserItem  *              m_pIClientUserItem;             //用户接口
	IGamePropertyUseSink *          m_pGameTrumpetSink;             //回调接口	

	//函数定义
public:
	//构造函数
	CDlgTrumpet(); 
	//析构函数
	virtual ~CDlgTrumpet();

	//绑定函数
protected:
	//确定函数
	virtual VOID OnOK();
	//取消消息
	virtual VOID OnCancel();
	//初始框架
	virtual BOOL OnInitDialog();
	//控件绑定
	virtual void DoDataExchange(CDataExchange* pDX);  	

	//重载函数
public:	
	//初始控件
	virtual VOID InitControlUI();
	//消息提醒
    virtual VOID Notify(TNotifyUI &  msg);	
	//皮肤名称
	virtual LPCTSTR GetSkinFile() { return TEXT("DlgTrumpetItem.xml"); } 	

	//绘画函数
protected:
	//绘画消息
	virtual VOID OnDrawClientAreaEnd(CDC * pDC, INT nWidth, INT nHeight);

	//表情接口
public:
	//选择事件
	virtual VOID OnExpressionSelect(CExpression * pExpression, tagExpressionInfo * pExpressionInfo);

	//辅助函数
public:	
	//更新界面
	void UpdateControlSurface();
	//获取颜色
	COLORREF GetTrumpetColor() { return m_crChatTX; };
	//获取内容
	void GetTrumpetContent(LPTSTR pszTrumpetContent);	
	//设置接口
	bool SetGameTrumpetSink(IUnknownEx * pIUnknownEx);
	//显示窗口
	void ShowTrumpetWindow(CWnd * pParentWnd,IClientUserItem  * pIClientUserItem);
	//设置喇叭数
	void SetTrumpet(const DWORD dwTrumpetCount);
	void SetTyphon(const DWORD dwTyphonCount);

	//消息处理
protected:	
	//喇叭按钮
	void OnBnClickedTrumpet();
	//喇叭按钮
	void OnBnClickedTyphon();
	//颜色按钮
	void OnBnClickedColorSet();
	//表情按钮
	void OnBnClickedExpression();
	//背包按钮
	void OnBnClickedBag();
	//商城按钮
	void OnBnClickedShop();

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////

#endif
