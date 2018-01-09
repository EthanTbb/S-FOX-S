#pragma once
#include "../��Ϸ�ͻ���/ClientControl.h"
#include "stdafx.h"

#define CONTROL_AREA		3
// CClientControlItemSinkDlg �Ի���

#define IDM_ADMIN_COMMDN WM_USER+1000

class AFX_EXT_CLASS CClientControlItemSinkDlg : public IClientControlDlg
{
	DECLARE_DYNAMIC(CClientControlItemSinkDlg)

protected:
	BYTE m_cbControlStyle;
	BYTE m_cbExcuteTimes;

	bool m_bWinArea[CONTROL_AREA];
	CString m_strCaption;
	LONGLONG						m_lStorageStart;				//��ʼ���
	LONGLONG						m_lStorageDeduct;				//���˥��
	LONGLONG						m_lStorageCurrent;				//��ǰ���
	LONGLONG						m_lStorageMax1;					//�������1
	LONGLONG						m_lStorageMul1;					//ϵͳ��ָ���1
	LONGLONG						m_lStorageMax2;					//�������2
	LONGLONG						m_lStorageMul2;					//ϵͳ��ָ���2
	BYTE							m_cbTotalCount;					//�����ܵ���

	CListCtrl						m_listUserBet;					//�б�ؼ�
	LONGLONG						m_lQueryGameID;					//��ѯID

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
	virtual bool  ReqResult(const void * pBuffer);
	//������ע
	virtual void __cdecl UpdateUserBet(bool bReSet);
	//���¿��
	virtual bool __cdecl UpdateStorage(const void * pBuffer);	
	//������Ӯ
	virtual bool __cdecl UpdateControlWin(const void * pBuffer);
	//���¿ؼ�
	virtual void __cdecl UpdateControl();

public:
	//���ý���
	void ReSetAdminWnd();

public:
	//��ʾ״̬
	void PrintCurStatus();
	void RequestUpdateStorage();

public:
	afx_msg void  OnReSet();
	afx_msg void  OnRefresh();
	afx_msg void  OnExcute();
	afx_msg void  OnFreshStorage();

public:
	virtual BOOL OnInitDialog();
	
	afx_msg void OnBnClickedControl();
	afx_msg void OnEnSetfocusEditUserId();
	afx_msg void OnBnClickedBtnUserBetQuery();
	afx_msg void OnBnClickedBtnUserBetAll();
	afx_msg void OnBnClickedSureWinlose();
	afx_msg void OnNMClickListUserBet(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnEnUpdateEditUserId();
};
