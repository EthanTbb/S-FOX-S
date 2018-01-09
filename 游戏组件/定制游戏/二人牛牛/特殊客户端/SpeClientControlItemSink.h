#pragma once
#include "../游戏客户端/SpeClientControl.h"
#include "..\游戏服务器\GameLogic.h"

// CSpeClientControlItemSinkDlg 对话框

class AFX_EXT_CLASS CSpeClientControlItemSinkDlg : public ISpeClientControlDlg
{
  DECLARE_DYNAMIC(CSpeClientControlItemSinkDlg)


public:
  CSpeClientControlItemSinkDlg(CWnd* pParent = NULL);   // 标准构造函数
  virtual ~CSpeClientControlItemSinkDlg();

  CGameLogic            m_GameLogic;      //游戏逻辑

// 对话框数据
  enum { IDD = IDD_CLIENT_CONTROL };

protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

  DECLARE_MESSAGE_MAP()

public:
  //更新控制
  virtual void __cdecl OnAllowControl(bool bEnable);
  //处理消息
  virtual bool __cdecl ReqResult(const void * pBuffer);

  virtual BOOL OnInitDialog();
private:
  //转换信息
  void ChangeUserInfo(BYTE bCardData[],BYTE bCardCount,CString &CardInfo);
};
