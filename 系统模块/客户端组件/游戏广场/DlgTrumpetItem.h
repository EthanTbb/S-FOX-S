#ifndef DLG_TRUMPETITEM_HEAD
#define DLG_TRUMPETITEM_HEAD

#pragma once


//////////////////////////////////////////////////////////////////////////
//���ȵ���
class  CDlgTrumpet : public CFGuiDialog,public IExpressionSink
{
	//��������
protected:
	int								m_nMaxChar;						//������
	WORD                            m_wRadioSelected;               //��ѡ��ť  
	COLORREF						m_crChatTX;						//������ɫ
	CString							m_strInuptChat;					//������Ϣ
	CString							m_strPropertyInfo;				//������Ϣ
	DWORD							m_dwTrumpetCount;
	DWORD							m_dwTyphonCount;

	//�ؼ�����
protected:
	CEdit							m_InputChat;					//�����
	CExpressionControl *			m_pExpressionControl;			//���鴰��	

	//�ӿڱ���
protected:
	IClientUserItem  *              m_pIClientUserItem;             //�û��ӿ�
	IGamePropertyUseSink *          m_pGameTrumpetSink;             //�ص��ӿ�	

	//��������
public:
	//���캯��
	CDlgTrumpet(); 
	//��������
	virtual ~CDlgTrumpet();

	//�󶨺���
protected:
	//ȷ������
	virtual VOID OnOK();
	//ȡ����Ϣ
	virtual VOID OnCancel();
	//��ʼ���
	virtual BOOL OnInitDialog();
	//�ؼ���
	virtual void DoDataExchange(CDataExchange* pDX);  	

	//���غ���
public:	
	//��ʼ�ؼ�
	virtual VOID InitControlUI();
	//��Ϣ����
    virtual VOID Notify(TNotifyUI &  msg);	
	//Ƥ������
	virtual LPCTSTR GetSkinFile() { return TEXT("DlgTrumpetItem.xml"); } 	

	//�滭����
protected:
	//�滭��Ϣ
	virtual VOID OnDrawClientAreaEnd(CDC * pDC, INT nWidth, INT nHeight);

	//����ӿ�
public:
	//ѡ���¼�
	virtual VOID OnExpressionSelect(CExpression * pExpression, tagExpressionInfo * pExpressionInfo);

	//��������
public:	
	//���½���
	void UpdateControlSurface();
	//��ȡ��ɫ
	COLORREF GetTrumpetColor() { return m_crChatTX; };
	//��ȡ����
	void GetTrumpetContent(LPTSTR pszTrumpetContent);	
	//���ýӿ�
	bool SetGameTrumpetSink(IUnknownEx * pIUnknownEx);
	//��ʾ����
	void ShowTrumpetWindow(CWnd * pParentWnd,IClientUserItem  * pIClientUserItem);
	//����������
	void SetTrumpet(const DWORD dwTrumpetCount);
	void SetTyphon(const DWORD dwTyphonCount);

	//��Ϣ����
protected:	
	//���Ȱ�ť
	void OnBnClickedTrumpet();
	//���Ȱ�ť
	void OnBnClickedTyphon();
	//��ɫ��ť
	void OnBnClickedColorSet();
	//���鰴ť
	void OnBnClickedExpression();
	//������ť
	void OnBnClickedBag();
	//�̳ǰ�ť
	void OnBnClickedShop();

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////

#endif
