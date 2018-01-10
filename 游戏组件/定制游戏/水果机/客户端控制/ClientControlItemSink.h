#pragma once
#include "../��Ϸ�ͻ���/ClientControl.h"
#include "afxcmn.h"
#include "skinlistctrl.h"


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

public:
	CClientControlItemSinkDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CClientControlItemSinkDlg();
	LONGLONG						m_lStorageStart;						//�����ֵ
	LONGLONG						m_lStorageDeduct;						//���˥��
	CSkinListCtrlEx					m_SkinListCtrl;
	LONGLONG						m_AreaTotalBet[AREA_COUNT];				//������ע	
	LONGLONG						m_lUserTotalScore[GAME_PLAYER];			//�������Ӯ
	//LONGLONG                        m_lPlayerInfo[GAME_PLAYER];				//�����Ϣ
// �Ի�������
	enum { IDD = IDD_CLIENT_CONTROL_EX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:
	//���¿���
	virtual void  UpdateControl(CMD_S_ControlReturns* pControlReturns);
	//�����б�
	virtual void UpdateClistCtrl(LONGLONG lAllPlayBet[GAME_PLAYER][AREA_COUNT],BYTE cbViewIndex, LONGLONG lScoreCount, WORD wChair, WORD wViewChairID,IClientUserItem* pUserData);
	//����б�
	virtual void ClearClist();
	//���ÿ����Ϣ
	virtual void SetStorageInfo(CMD_S_ADMIN_STORAGE_INFO *pStorageInof);

protected:
	//��Ϣ����
	void PrintingInfo(TCHAR* pText, WORD cbCount, BYTE cbArea, BYTE cbTimes);
	//���ý���
	void ReSetAdminWnd();

public:
	//��ʼ��
	virtual BOOL OnInitDialog();
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
	//���¿��
	afx_msg void  OnFreshStorage();
	//����˥��
	afx_msg void  OnFreshDeuct();
	//������ʼ���ֵ
	afx_msg LRESULT OnSetStartStorage(WPARAM wParam,LPARAM lParam);
	//�ؼ���Ϣ
	afx_msg void OnBnClickedBtDeduct2();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	

	CListCtrl m_ListTotalBet;
};
