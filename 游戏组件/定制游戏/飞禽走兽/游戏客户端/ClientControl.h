#pragma once

//游戏控制基类
class IClientControlDlg : public CDialog
{
public:
  IClientControlDlg(UINT UID, CWnd* pParent) : CDialog(UID, pParent) {}
  virtual ~IClientControlDlg(void) {}
  CUserBetArray          m_UserBetArray;         //用户下注
public:
  //更新控制
  virtual void  UpdateControl(CMD_S_ControlReturns* pControlReturns) = NULL;
  //初始控件
  virtual bool  InitControl()=NULL;
  //更新库存
  virtual bool __cdecl UpdateStorage(const void * pBuffer) = NULL;
  //更新下注
  virtual void __cdecl UpdateUserBet(bool bReSet) = NULL;
  virtual     void ClearText()=NULL;
  //更新控件
  virtual void __cdecl UpdateControl()=NULL;
  virtual void  SetStorageCurrentValue(LONGLONG lCurrentStorage, LONGLONG lCurrentBonus)=NULL;
  virtual  void ClearClist()=NULL;
};
