#pragma once
#include "../游戏客户端/ClientControl.h"
#include "afxcmn.h"
#include "skinlistctrl.h"


// CClientControlItemSinkDlg 对话框
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
	CClientControlItemSinkDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CClientControlItemSinkDlg();
	LONGLONG						m_lStorageStart;						//库存数值
	LONGLONG						m_lStorageDeduct;						//库存衰减
	CSkinListCtrlEx					m_SkinListCtrl;
	LONGLONG						m_AreaTotalBet[AREA_COUNT];				//区域总注	
	LONGLONG						m_lUserTotalScore[GAME_PLAYER];			//玩家总输赢
	//LONGLONG                        m_lPlayerInfo[GAME_PLAYER];				//玩家信息
// 对话框数据
	enum { IDD = IDD_CLIENT_CONTROL_EX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	//更新控制
	virtual void  UpdateControl(CMD_S_ControlReturns* pControlReturns);
	//更新列表
	virtual void UpdateClistCtrl(LONGLONG lAllPlayBet[GAME_PLAYER][AREA_COUNT],BYTE cbViewIndex, LONGLONG lScoreCount, WORD wChair, WORD wViewChairID,IClientUserItem* pUserData);
	//清除列表
	virtual void ClearClist();
	//设置库存信息
	virtual void SetStorageInfo(CMD_S_ADMIN_STORAGE_INFO *pStorageInof);

protected:
	//信息解析
	void PrintingInfo(TCHAR* pText, WORD cbCount, BYTE cbArea, BYTE cbTimes);
	//重置界面
	void ReSetAdminWnd();

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
	//更新库存
	afx_msg void  OnFreshStorage();
	//更新衰减
	afx_msg void  OnFreshDeuct();
	//设置起始库存值
	afx_msg LRESULT OnSetStartStorage(WPARAM wParam,LPARAM lParam);
	//控件消息
	afx_msg void OnBnClickedBtDeduct2();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	

	CListCtrl m_ListTotalBet;
};
