#pragma once
#include "../��Ϸ������/GameLogic.h"
void MakeString(CString &strNum,SCORE lNumber);
void MakeStringToNum(CString str,SCORE &Num);
// CDlgBank dialog
#define  WM_CREADED						WM_USER+1
class CDlgBank : public CSkinDialog
{
	//��������
public:
	bool							m_bBankStorage;					//�洢��ʶ,true=��
	IClientUserItem						*m_pMeUserData;					//�û���Ϣ
	SCORE						m_lGameGold;					//������Ŀ
	SCORE						m_lStorageGold;					//�洢��Ŀ
	SCORE						m_lInCount;						//������Ϸ��
	SCORE						m_OrInCount;
	bool							m_bCanDraw;
	bool							m_blCanStore;
	//bool							m_bStorageAllGold;				//�������н��
	//bool							m_bDrawAllGold;					//ȡ�����н��
	//�ؼ�����
protected:
	CSkinButton						m_btOK;							//ȷ����ť
	CSkinButton						m_btCancel;						//ȡ����ť
	CSkinButton						m_btFresh;						//ˢ�°�ť
	CEdit							m_Edit;
	CStatic							m_Static;

	bool							m_blUsingPassWord;
	CPngImage						m_ImageFrame;					//	
	CPngImage						m_ImageNumber;

	//�ӿڱ���
protected:	
	IClientKernel					* m_pIClientKernel;				//�ں˽ӿ�


public:
	CDlgBank(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgBank();

	//��������
public:
	//���½���
	void UpdateView();

	//���غ���
protected:
	//�ؼ���
	virtual void DoDataExchange(CDataExchange* pDX);
	//��ʼ������
	virtual BOOL OnInitDialog();
	//ȷ����ť
	afx_msg void OnBnClickedOk();
	//�����Ϣ
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//�����Ϣ
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//������Ϣ
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//ʱ����Ϣ
	afx_msg void OnTimer(UINT nIDEvent);
	//������Ϣ
	afx_msg void OnEnChangeInCount();

	afx_msg void OnEnRadioStore();

	afx_msg void OnEnRadioGet();
	//�滭��Ϣ
	virtual VOID OnDrawClientArea(CDC * pDC, INT nWidth, INT nHeight);

    int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//�ӿں���
public:
	//���ÿؼ�
	void ShowItem();
	//����ȡ��
	void AllowDraw(bool bAllow);
	//������
	void AllowStorage(bool bAllow);
	//����λ��
	void SetPostPoint(CPoint Point);
	//��ȡ����
	bool GetBankActionType(){return m_bBankStorage;}
	//��������
	void SetBankActionType(bool bStorage);

	//���ܺ���
public:
	//�Ƿ�ѡ��
	bool IsButtonSelected(UINT uButtonID);
	//ѡ�а�ť
	void SetButtonSelected(UINT uButtonID, bool bSelected);
	//�������
	void SetSendInfo(IClientKernel *pClientKernel,IClientUserItem const*pMeUserDatas);
	//������Ϣ
	void SendSocketData(WORD wMainCmdID, WORD wSubCmdID, void * pBuffer, WORD wDataSize);
	void OnCancelEvent();
	//�滭����
	VOID DrawNumberString(CDC * pDC, SCORE lScore, INT nXPos, INT nYPos);

	DECLARE_MESSAGE_MAP()
	DECLARE_DYNAMIC(CDlgBank)
protected:
	virtual void OnCancel();
};
