#pragma once
#include "../��Ϸ�ͻ���/ClientControl.h"
#include "afxcmn.h"


// CClientControlItemSinkDlg �Ի���
#ifndef _UNICODE
#define myprintf	_snprintf
#define mystrcpy	strcpy
#define mystrlen	strlen
#define myscanf		_snscanf
#define	myLPSTR		LPCSTR
#define myatoi      atoi
#define myatoi64    _atoi64
#else
#define myprintf	swprintf
#define mystrcpy	wcscpy
#define mystrlen	wcslen
#define myscanf		_snwscanf
#define	myLPSTR		LPWSTR
#define myatoi      _wtoi
#define myatoi64	_wtoi64
#endif


class AFX_EXT_CLASS CClientControlItemSinkDlg : public IClientControlDlg
{
	DECLARE_DYNAMIC(CClientControlItemSinkDlg)

protected:
	BYTE m_cbControlArea;
	BYTE m_cbControlStyle;
	BYTE m_cbExcuteTimes;

	LONGLONG						m_lStorageStart;				//��ʼ���
	LONGLONG						m_lStorageDeduct;				//���˥��
	LONGLONG						m_lStorageCurrent;				//��ǰ���
	LONGLONG						m_lStorageMax1;					//�������1
	LONGLONG						m_lStorageMul1;					//ϵͳ��ָ���1
	LONGLONG						m_lStorageMax2;					//�������2
	LONGLONG						m_lStorageMul2;					//ϵͳ��ָ���2

	CListCtrl						m_listUserBet;					//�б�ؼ�
	CListCtrl						m_listUserBetAll;				//�б�ؼ�
	CListCtrl						m_ListAttention;				//�б�ؼ�

	LONGLONG						m_lQueryGameID;					//��ѯID

	int								m_nShowWay;						//��ʾ��ʽ
	bool							m_bShowRobot;					//��������ע
	//��������
public:
	CClientControlItemSinkDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CClientControlItemSinkDlg();

// �Ի�������
	enum { IDD = IDD_CLIENT_CONTROL_EX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

	//����̳�
public:
	//���¿���
	virtual void  UpdateControl(CMD_S_ControlReturns* pControlReturns);
	//��ʼ�ؼ�
	virtual bool  InitControl();
	//��ʼ��
	virtual BOOL OnInitDialog();

	//��������
protected:
	//��Ϣ����
	void PrintingInfo(TCHAR* pText, WORD cbCount, BYTE cbArea, BYTE cbTimes);
	//���ý���
	void ReSetAdminWnd(bool bQuickControl=false);
	//��������
	bool CheckDataMinMax(LONGLONG valueMax);
	//ˢ����Ϣ
	void RequestUpdateStorage();
	//��������
	void ListSort();
	//�̳к���
public:
	//�趨��ǰ���
	virtual void  SetStorageCurrentValue(LONGLONG lvalue);
	//������ע
	virtual void __cdecl UpdateUserBet(bool bReSet);
	//�����Ϣ
	virtual void ClearText();
	//���¿���
	virtual void __cdecl UpdateControl();
	//���¿��
	virtual bool __cdecl UpdateStorage(const void * pBuffer);
	//�����б�
	virtual void ClearClist();
	//�����б�
	virtual void UpdateUserList(bool bDelUser=false);
	//��עͳ��
	virtual void UpdateAllUserBet(LONGLONG lAllUserBet[AREA_COUNT+1],bool bClear=false);
	//�¼���Ϣ
public:
	//������ɫ
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//ȡ������
	afx_msg void OnBnClickedButtonReset();
	//���ֿ���
	afx_msg void OnBnClickedButtonSyn();
	//��������
	afx_msg void OnBnClickedButtonOk();
	//ȡ���ر�
	afx_msg void OnBnClickedButtonCancel();
	//���ˢ��
	afx_msg void  OnRefreshKuCun();
	//ׯ�ҿ���
	afx_msg void OnBnClickedRadioCtBanker();
	//�������
	afx_msg void OnBnClickedRadioCtArea();
	//���¿��
	afx_msg void OnBnClickedBtnUpdateStorage();
	//��ӹ�ע
	afx_msg void OnNMDblclkListUserBet(NMHDR *pNMHDR, LRESULT *pResult);
	//��ӹ�ע
	afx_msg void OnBnClickedButtonAdduser();
	//ɾ����ע
	afx_msg void OnBnClickedButtonDel();
	//�ı���ע��ʾ
	afx_msg void OnCbnSelchangeComboChoose();
	//��������ע��ʾ
	afx_msg void OnBnClickedCheck1();
	//��ݿ���
	afx_msg void OnBnClickedButtonCar(UINT nID);
	//�����б�
	afx_msg void OnLvnColumnclickListUserBet(NMHDR *pNMHDR, LRESULT *pResult);
};
