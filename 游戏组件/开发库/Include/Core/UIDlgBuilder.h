#ifndef UIDLGBUILDER_HEAD_FILE
#define UIDLGBUILDER_HEAD_FILE
#pragma once


/////////////////////////////////////////////////////////////////////////////////////

//�����ص�
class IDialogBuilderCallback
{
	//��������
public:
	//�����ؼ�
    virtual CControlUI* CreateControl(LPCTSTR pstrClass) = NULL;
	//��������
	virtual void SetCustomAttribute(LPCTSTR pszName,LPCTSTR pszValue)=NULL;
};

class IDialogHelper
{
public:
	virtual void IncludeControl(CControlUI* pControlUI, CControlUI* pParentUI, LPCTSTR pXml) = NULL;
};

/////////////////////////////////////////////////////////////////////////////////////

//XML�ֵ�
class CStdMarkupPtrMap : public CStdStringPtrMap
{
	//��������
public:
	//���캯��
	CStdMarkupPtrMap(int nSize=83);
	//��������
	virtual ~CStdMarkupPtrMap();
};

/////////////////////////////////////////////////////////////////////////////////////

class FGUI_LIB_CLASS CDialogBuilder
{
	//��������
protected:
	CMarkup *							m_pxml;							//XML����	
	LPCTSTR								m_pstrtype;						//��Դ����
    IDialogBuilderCallback*				m_pCallback;					//�ص��ӿ�
	IDialogHelper*						m_pHelper;						//�����ӿ�
	//�洢����
protected:
	static CStdMarkupPtrMap				m_MarkupMap;					//XML�ֵ�

	//��������
public:
	//���캯��
    CDialogBuilder();
	CDialogBuilder(IDialogHelper* pHelper);
	//��������
    virtual ~CDialogBuilder();

    //���ܺ���
public:
	//��������
    CControlUI* Create(STRINGorID xml, LPCTSTR type = NULL, IDialogBuilderCallback* pCallback = NULL,
        CPaintManagerUI* pManager = NULL, CControlUI* pParent = NULL, bool bInclude = false);
	//��������
    CControlUI* Create(IDialogBuilderCallback* pCallback = NULL, CPaintManagerUI* pManager = NULL,
        CControlUI* pParent = NULL, bool bInclude = false);
	//��ȡXML����
	CMarkup* GetMarkup() { return m_pxml; }
	//ɾ����Դ
    static bool Remove(STRINGorID xml);

	//��Ϣ����
public:
	//������Ϣ
    void GetLastErrorMessage(LPTSTR pstrMessage, SIZE_T cchMax) const;
	//������Ϣ
    void GetLastErrorLocation(LPTSTR pstrSource, SIZE_T cchMax) const;

	//��������
private:
	//��������
    CControlUI* _Parse(CMarkupNode* parent, CControlUI* pParent = NULL, CPaintManagerUI* pManager = NULL);    
};

/////////////////////////////////////////////////////////////////////////////////////
#endif
