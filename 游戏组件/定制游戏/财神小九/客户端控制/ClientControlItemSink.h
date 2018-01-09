#pragma once
#include "../游戏客户端/ClientControl.h"

#define CONTROL_AREA		3
// CClientControlItemSinkDlg 对话框

class AFX_EXT_CLASS CClientControlItemSinkDlg : public IClientControlDlg
{
	DECLARE_DYNAMIC(CClientControlItemSinkDlg)

protected:
	BYTE m_bWinArea;
	BYTE m_cbControlStyle;
	BYTE m_cbExcuteTimes;
	LONGLONG m_lUserJettonScore[AREA_ARRY_COUNT];

public:
	CClientControlItemSinkDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CClientControlItemSinkDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_ADMIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	//更新控制
	virtual void  OnAllowControl(bool bEnable);
	//申请结果
	virtual bool  ReqResult(const void * pBuffer) { return true; };
	//下注信息
	virtual bool  PlayersBet(BYTE cbViewIndex, LONGLONG lScoreCount);

public:
	//重置界面
	void ReSetAdminWnd();

public:
	//显示状态
	void PrintCurStatus();
public:
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnBnClickedCSm();
	afx_msg void OnBnClickedCTm();
	afx_msg void OnBnClickedCXm();
	afx_msg void OnBnClickedOk();
protected:
	virtual void OnOK();
public:
	afx_msg void OnBnClickedCQxkz();
};
