#ifndef UIDLGBUILDER_HEAD_FILE
#define UIDLGBUILDER_HEAD_FILE
#pragma once


/////////////////////////////////////////////////////////////////////////////////////

//构建回调
class IDialogBuilderCallback
{
	//函数定义
public:
	//创建控件
    virtual CControlUI* CreateControl(LPCTSTR pstrClass) = NULL;
	//设置属性
	virtual void SetCustomAttribute(LPCTSTR pszName,LPCTSTR pszValue)=NULL;
};

class IDialogHelper
{
public:
	virtual void IncludeControl(CControlUI* pControlUI, CControlUI* pParentUI, LPCTSTR pXml) = NULL;
};

/////////////////////////////////////////////////////////////////////////////////////

//XML字典
class CStdMarkupPtrMap : public CStdStringPtrMap
{
	//函数定义
public:
	//构造函数
	CStdMarkupPtrMap(int nSize=83);
	//析构函数
	virtual ~CStdMarkupPtrMap();
};

/////////////////////////////////////////////////////////////////////////////////////

class FGUI_LIB_CLASS CDialogBuilder
{
	//变量定义
protected:
	CMarkup *							m_pxml;							//XML对象	
	LPCTSTR								m_pstrtype;						//资源类型
    IDialogBuilderCallback*				m_pCallback;					//回调接口
	IDialogHelper*						m_pHelper;						//帮助接口
	//存储变量
protected:
	static CStdMarkupPtrMap				m_MarkupMap;					//XML字典

	//函数定义
public:
	//构造函数
    CDialogBuilder();
	CDialogBuilder(IDialogHelper* pHelper);
	//析构函数
    virtual ~CDialogBuilder();

    //功能函数
public:
	//创建函数
    CControlUI* Create(STRINGorID xml, LPCTSTR type = NULL, IDialogBuilderCallback* pCallback = NULL,
        CPaintManagerUI* pManager = NULL, CControlUI* pParent = NULL, bool bInclude = false);
	//创建函数
    CControlUI* Create(IDialogBuilderCallback* pCallback = NULL, CPaintManagerUI* pManager = NULL,
        CControlUI* pParent = NULL, bool bInclude = false);
	//获取XML对象
	CMarkup* GetMarkup() { return m_pxml; }
	//删除资源
    static bool Remove(STRINGorID xml);

	//信息函数
public:
	//错误信息
    void GetLastErrorMessage(LPTSTR pstrMessage, SIZE_T cchMax) const;
	//错误信息
    void GetLastErrorLocation(LPTSTR pstrSource, SIZE_T cchMax) const;

	//辅助函数
private:
	//解析函数
    CControlUI* _Parse(CMarkupNode* parent, CControlUI* pParent = NULL, CPaintManagerUI* pManager = NULL);    
};

/////////////////////////////////////////////////////////////////////////////////////
#endif
