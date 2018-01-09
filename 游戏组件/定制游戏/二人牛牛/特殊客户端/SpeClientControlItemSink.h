#pragma once
#include "../��Ϸ�ͻ���/SpeClientControl.h"
#include "..\��Ϸ������\GameLogic.h"

// CSpeClientControlItemSinkDlg �Ի���

class AFX_EXT_CLASS CSpeClientControlItemSinkDlg : public ISpeClientControlDlg
{
  DECLARE_DYNAMIC(CSpeClientControlItemSinkDlg)


public:
  CSpeClientControlItemSinkDlg(CWnd* pParent = NULL);   // ��׼���캯��
  virtual ~CSpeClientControlItemSinkDlg();

  CGameLogic            m_GameLogic;      //��Ϸ�߼�

// �Ի�������
  enum { IDD = IDD_CLIENT_CONTROL };

protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

  DECLARE_MESSAGE_MAP()

public:
  //���¿���
  virtual void __cdecl OnAllowControl(bool bEnable);
  //������Ϣ
  virtual bool __cdecl ReqResult(const void * pBuffer);

  virtual BOOL OnInitDialog();
private:
  //ת����Ϣ
  void ChangeUserInfo(BYTE bCardData[],BYTE bCardCount,CString &CardInfo);
};
