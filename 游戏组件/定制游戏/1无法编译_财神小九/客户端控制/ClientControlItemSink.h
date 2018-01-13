#pragma once
#include "../��Ϸ�ͻ���/ClientControl.h"

#define CONTROL_AREA		3
// CClientControlItemSinkDlg �Ի���

class AFX_EXT_CLASS CClientControlItemSinkDlg : public IClientControlDlg
{
	DECLARE_DYNAMIC(CClientControlItemSinkDlg)

protected:
	BYTE m_bWinArea;
	BYTE m_cbControlStyle;
	BYTE m_cbExcuteTimes;
	LONGLONG m_lUserJettonScore[AREA_ARRY_COUNT];

public:
	CClientControlItemSinkDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CClientControlItemSinkDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_ADMIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:
	//���¿���
	virtual void  OnAllowControl(bool bEnable);
	//������
	virtual bool  ReqResult(const void * pBuffer) { return true; };
	//��ע��Ϣ
	virtual bool  PlayersBet(BYTE cbViewIndex, LONGLONG lScoreCount);

public:
	//���ý���
	void ReSetAdminWnd();

public:
	//��ʾ״̬
	void PrintCurStatus();
public:
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnBnClickedCSm();
	afx_msg void OnBnClickedCTm();
	afx_msg void OnBnClickedCXm();
	afx_msg void OnBnClickedOk();
protected:
	virtual void OnOK();
public:
	afx_msg void OnBnClickedCQxkz();
};
