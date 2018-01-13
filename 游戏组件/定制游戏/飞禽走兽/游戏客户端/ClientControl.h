#pragma once

//��Ϸ���ƻ���
class IClientControlDlg : public CDialog
{
public:
  IClientControlDlg(UINT UID, CWnd* pParent) : CDialog(UID, pParent) {}
  virtual ~IClientControlDlg(void) {}
  CUserBetArray          m_UserBetArray;         //�û���ע
public:
  //���¿���
  virtual void  UpdateControl(CMD_S_ControlReturns* pControlReturns) = NULL;
  //��ʼ�ؼ�
  virtual bool  InitControl()=NULL;
  //���¿��
  virtual bool __cdecl UpdateStorage(const void * pBuffer) = NULL;
  //������ע
  virtual void __cdecl UpdateUserBet(bool bReSet) = NULL;
  virtual     void ClearText()=NULL;
  //���¿ؼ�
  virtual void __cdecl UpdateControl()=NULL;
  virtual void  SetStorageCurrentValue(LONGLONG lCurrentStorage, LONGLONG lCurrentBonus)=NULL;
  virtual  void ClearClist()=NULL;
};
