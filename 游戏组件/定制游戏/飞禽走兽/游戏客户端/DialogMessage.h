#pragma once


// CDialogMessage �Ի���
#include "../��Ϸ������/GameLogic.h"
class CDialogMessage : public CDialog
{
  DECLARE_DYNAMIC(CDialogMessage)

public:
  CDialogMessage(CWnd* pParent = NULL);   // ��׼���캯��
  virtual ~CDialogMessage();

// �Ի�������
  enum { IDD = IDD_DIALOG_MES };

  //����
private:
  CFont       m_InfoFont;     //��Ϣ����
  TCHAR       m_szMessage[128]; //��Ϣ
  CBitImage     m_ImageBackdrop;  //����
  CSkinButton     m_btDetermine;    //ȷ��
  CSkinButton     m_btClosee;     //�ر�

  //���ú���
public:
  //������Ϣ
  void SetMessage(LPCTSTR lpszString);

protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

  DECLARE_MESSAGE_MAP()
public:
  virtual BOOL OnInitDialog();
  afx_msg BOOL OnEraseBkgnd(CDC* pDC);
  afx_msg void OnPaint();
  afx_msg void OnBnClickedButtonClose();
};
