#pragma once
#include "../游戏客户端/ClientControl.h"
#include "afxcmn.h"


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

protected:
	BYTE m_cbControlArea;
	BYTE m_cbControlStyle;
	BYTE m_cbExcuteTimes;

	LONGLONG						m_lStorageStart;				//起始库存
	LONGLONG						m_lStorageDeduct;				//库存衰减
	LONGLONG						m_lStorageCurrent;				//当前库存
	LONGLONG						m_lStorageMax1;					//库存上限1
	LONGLONG						m_lStorageMul1;					//系统输分概率1
	LONGLONG						m_lStorageMax2;					//库存上限2
	LONGLONG						m_lStorageMul2;					//系统输分概率2

	CListCtrl						m_listUserBet;					//列表控件
	CListCtrl						m_listUserBetAll;				//列表控件
	CListCtrl						m_ListAttention;				//列表控件

	LONGLONG						m_lQueryGameID;					//查询ID

	int								m_nShowWay;						//显示方式
	bool							m_bShowRobot;					//机器人下注
	//函数定义
public:
	CClientControlItemSinkDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CClientControlItemSinkDlg();

// 对话框数据
	enum { IDD = IDD_CLIENT_CONTROL_EX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

	//常规继承
public:
	//更新控制
	virtual void  UpdateControl(CMD_S_ControlReturns* pControlReturns);
	//初始控件
	virtual bool  InitControl();
	//初始化
	virtual BOOL OnInitDialog();

	//辅助函数
protected:
	//信息解析
	void PrintingInfo(TCHAR* pText, WORD cbCount, BYTE cbArea, BYTE cbTimes);
	//重置界面
	void ReSetAdminWnd(bool bQuickControl=false);
	//检验数据
	bool CheckDataMinMax(LONGLONG valueMax);
	//刷新信息
	void RequestUpdateStorage();
	//数列排序
	void ListSort();
	//继承函数
public:
	//设定当前库存
	virtual void  SetStorageCurrentValue(LONGLONG lvalue);
	//更新下注
	virtual void __cdecl UpdateUserBet(bool bReSet);
	//清除信息
	virtual void ClearText();
	//更新控制
	virtual void __cdecl UpdateControl();
	//更新库存
	virtual bool __cdecl UpdateStorage(const void * pBuffer);
	//清理列表
	virtual void ClearClist();
	//更新列表
	virtual void UpdateUserList(bool bDelUser=false);
	//下注统计
	virtual void UpdateAllUserBet(LONGLONG lAllUserBet[AREA_COUNT+1],bool bClear=false);
	//事件消息
public:
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
	//库存刷新
	afx_msg void  OnRefreshKuCun();
	//庄家控制
	afx_msg void OnBnClickedRadioCtBanker();
	//区域控制
	afx_msg void OnBnClickedRadioCtArea();
	//更新库存
	afx_msg void OnBnClickedBtnUpdateStorage();
	//添加关注
	afx_msg void OnNMDblclkListUserBet(NMHDR *pNMHDR, LRESULT *pResult);
	//添加关注
	afx_msg void OnBnClickedButtonAdduser();
	//删除关注
	afx_msg void OnBnClickedButtonDel();
	//改变下注显示
	afx_msg void OnCbnSelchangeComboChoose();
	//机器人下注显示
	afx_msg void OnBnClickedCheck1();
	//快捷控制
	afx_msg void OnBnClickedButtonCar(UINT nID);
	//排列列表
	afx_msg void OnLvnColumnclickListUserBet(NMHDR *pNMHDR, LRESULT *pResult);
};
