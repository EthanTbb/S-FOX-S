#ifndef INFORMATION_NEW_HEAD_FILE
#define INFORMATION_NEW_HEAD_FILE

#pragma once



//////////////////////////////////////////////////////////////////////////////////

//��Ϣ����
class  CDlgInformation : public CFGuiDialog
{
	//���Ա���
protected:
	UINT							m_uType;							//��Ϣ����
	CString							m_strString;						//��ʾ��Ϣ
	CString							m_strCaption;						//��Ϣ����

	//��������
protected:
	UINT							m_nElapse;							//ʣ��ʱ��
	UINT							m_nBtCount;							//��ť��Ŀ
	CRect							m_rcString;							//�ַ�λ��
	HINSTANCE						m_hResInstance;						//��Դ���

	//�ؼ�����
protected:
	UINT							m_uButtonID[3];						//��ť��ʶ
	CButtonUI *						m_pbtButton[3];						//��ť�ؼ�

	//��������
public:
	//���캯��
	CDlgInformation(CWnd * pParentWnd=NULL);
	//��������
	virtual ~CDlgInformation();

	//���غ���
protected:
	//��ʼ����Ϣ
	virtual BOOL OnInitDialog();

    //�ӿں���
public:
	//��ʼ�ؼ�
	virtual void InitControlUI();
	//��Ϣ����
	virtual void Notify(TNotifyUI &  msg);	
	//Ƥ������
	virtual LPCTSTR GetSkinFile(); 
	//���ܺ���
public:
	//��ʾ��Ϣ
	INT ShowMessageBox(LPCTSTR pszString, UINT uType=MB_OK, UINT nElapse=0);
	//��ʾ��Ϣ
	INT ShowMessageBox(LPCTSTR pszCaption, LPCTSTR pszString, UINT uType=MB_OK, UINT nElapse=0);

	//�ڲ�����
private:
	//������ť
	VOID CreateBoxButton();

	VOID UpdateUI();

	//��Ϣӳ��
protected:
	//ʱ����Ϣ
	VOID OnTimer(UINT_PTR nIDEvent);

	DECLARE_MESSAGE_MAP()
};


#endif