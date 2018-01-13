#pragma once
#include "../��Ϸ�ͻ���/ClientControl.h"


// CClientControlItemSinkDlg �Ի���
#ifndef _UNICODE
#define myprintf  _snprintf
#define mystrcpy  strcpy
#define mystrlen  strlen
#define myscanf   _snscanf
#define myLPSTR   LPCSTR
#define myatoi      atoi
#define myatoi64    _atoi64
#else
#define myprintf  swprintf
#define mystrcpy  wcscpy
#define mystrlen  wcslen
#define myscanf   _snwscanf
#define myLPSTR   LPWSTR
#define myatoi      _wtoi
#define myatoi64  _wtoi64
#endif


class AFX_EXT_CLASS CClientControlItemSinkDlg : public IClientControlDlg
{
  DECLARE_DYNAMIC(CClientControlItemSinkDlg)

protected:
  BYTE m_cbControlArea;
  BYTE m_cbControlStyle;
  BYTE m_cbExcuteTimes;


  LONGLONG            m_lStorageStart;        //��ʼ���
  LONGLONG            m_lStorageDeduct;       //���˥��
  LONGLONG            m_lStorageCurrent;        //��ǰ���
  LONGLONG            m_lCurrentBonus;        //��ǰ�ʽ��
  LONGLONG            m_lStorageMax1;         //�������1
  LONGLONG            m_lStorageMul1;         //ϵͳ��ָ���1
  LONGLONG            m_lStorageMax2;         //�������2
  LONGLONG            m_lStorageMul2;         //ϵͳ��ָ���2

  CListCtrl           m_listUserBet;          //�б�ؼ�
  CListCtrl           m_listUserBetAll;       //�б�ؼ�
  LONGLONG            m_lQueryGameID;         //��ѯID
public:
  CClientControlItemSinkDlg(CWnd* pParent = NULL);   // ��׼���캯��
  virtual ~CClientControlItemSinkDlg();

// �Ի�������
  enum { IDD = IDD_CLIENT_CONTROL_EX };

protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

  DECLARE_MESSAGE_MAP()

public:
  //���¿���
  virtual void  UpdateControl(CMD_S_ControlReturns* pControlReturns);
  //��ʼ�ؼ�
  virtual bool  InitControl();

protected:
  //��Ϣ����
  void PrintingInfo(TCHAR* pText, WORD cbCount, BYTE cbArea, BYTE cbTimes);
  //���ý���
  void ReSetAdminWnd();
  //���¿ؼ�
  virtual void __cdecl UpdateControl();


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

  afx_msg void  OnRefreshKuCun();


  afx_msg void OnBnClickedRadioCtBanker();
  afx_msg void OnBnClickedRadioCtArea();
  afx_msg void OnBnClickedRadioWin();
  afx_msg void OnBnClickedRadioLose();
  bool CheckDataMinMax(LONGLONG valueMax);
  afx_msg void OnBnClickedBtnUpdateStorage();
  void RequestUpdateStorage();
  afx_msg void OnBnClickedBtnUserBetQuery();
  afx_msg void OnBnClickedBtnUserBetAll();
  afx_msg void OnEnSetfocusEditUserId();
  virtual void  SetStorageCurrentValue(LONGLONG lCurrentStorage, LONGLONG lCurrentBonus);
  //������ע
  virtual void __cdecl UpdateUserBet(bool bReSet);
  virtual     void ClearText();
  void ClearClist();
  //���¿��
  virtual bool __cdecl UpdateStorage(const void * pBuffer);

  afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
};


