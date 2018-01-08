#ifndef INFORMATION_NEW_HEAD_FILE
#define INFORMATION_NEW_HEAD_FILE

#pragma once



//////////////////////////////////////////////////////////////////////////////////

//消息窗口
class  CDlgInformation : public CFGuiDialog
{
	//属性变量
protected:
	UINT							m_uType;							//消息类型
	CString							m_strString;						//提示消息
	CString							m_strCaption;						//信息标题

	//辅助变量
protected:
	UINT							m_nElapse;							//剩余时间
	UINT							m_nBtCount;							//按钮数目
	CRect							m_rcString;							//字符位置
	HINSTANCE						m_hResInstance;						//资源句柄

	//控件变量
protected:
	UINT							m_uButtonID[3];						//按钮标识
	CButtonUI *						m_pbtButton[3];						//按钮控件

	//函数定义
public:
	//构造函数
	CDlgInformation(CWnd * pParentWnd=NULL);
	//析构函数
	virtual ~CDlgInformation();

	//重载函数
protected:
	//初始化消息
	virtual BOOL OnInitDialog();

    //接口函数
public:
	//初始控件
	virtual void InitControlUI();
	//消息提醒
	virtual void Notify(TNotifyUI &  msg);	
	//皮肤名称
	virtual LPCTSTR GetSkinFile(); 
	//功能函数
public:
	//显示消息
	INT ShowMessageBox(LPCTSTR pszString, UINT uType=MB_OK, UINT nElapse=0);
	//显示消息
	INT ShowMessageBox(LPCTSTR pszCaption, LPCTSTR pszString, UINT uType=MB_OK, UINT nElapse=0);

	//内部函数
private:
	//创建按钮
	VOID CreateBoxButton();

	VOID UpdateUI();

	//消息映射
protected:
	//时间消息
	VOID OnTimer(UINT_PTR nIDEvent);

	DECLARE_MESSAGE_MAP()
};


#endif