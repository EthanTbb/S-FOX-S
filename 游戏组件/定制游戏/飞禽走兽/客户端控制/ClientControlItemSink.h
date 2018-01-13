#pragma once
#include "../游戏客户端/ClientControl.h"


// CClientControlItemSinkDlg 对话框
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


  LONGLONG            m_lStorageStart;        //起始库存
  LONGLONG            m_lStorageDeduct;       //库存衰减
  LONGLONG            m_lStorageCurrent;        //当前库存
  LONGLONG            m_lCurrentBonus;        //当前彩金池
  LONGLONG            m_lStorageMax1;         //库存上限1
  LONGLONG            m_lStorageMul1;         //系统输分概率1
  LONGLONG            m_lStorageMax2;         //库存上限2
  LONGLONG            m_lStorageMul2;         //系统输分概率2

  CListCtrl           m_listUserBet;          //列表控件
  CListCtrl           m_listUserBetAll;       //列表控件
  LONGLONG            m_lQueryGameID;         //查询ID
public:
  CClientControlItemSinkDlg(CWnd* pParent = NULL);   // 标准构造函数
  virtual ~CClientControlItemSinkDlg();

// 对话框数据
  enum { IDD = IDD_CLIENT_CONTROL_EX };

protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

  DECLARE_MESSAGE_MAP()

public:
  //更新控制
  virtual void  UpdateControl(CMD_S_ControlReturns* pControlReturns);
  //初始控件
  virtual bool  InitControl();

protected:
  //信息解析
  void PrintingInfo(TCHAR* pText, WORD cbCount, BYTE cbArea, BYTE cbTimes);
  //重置界面
  void ReSetAdminWnd();
  //更新控件
  virtual void __cdecl UpdateControl();


public:
  //初始化
  virtual BOOL OnInitDialog();
  //设置颜色
  afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
  //取消控制
  afx_msg void OnBnClickedButtonReset();
  //本局控制
  afx_msg void OnBnClickedButtonSyn();
  //开启控制
  afx_msg void OnBnClickedButtonOk();
  //取消关闭
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
  //更新下注
  virtual void __cdecl UpdateUserBet(bool bReSet);
  virtual     void ClearText();
  void ClearClist();
  //更新库存
  virtual bool __cdecl UpdateStorage(const void * pBuffer);

  afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
};


