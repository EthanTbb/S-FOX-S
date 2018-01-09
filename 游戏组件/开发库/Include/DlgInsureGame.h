#ifndef DLG_INSURE_GAME_HEAD_FILE
#define DLG_INSURE_GAME_HEAD_FILE

#pragma once

#include "GameFrameHead.h"


class CNumberConvert
{
public:
	CNumberConvert();
	~CNumberConvert();

public:
	VOID SwitchScoreFormat(SCORE lScore, UINT uSpace, LPTSTR pszBuffer, WORD wBufferSize);
	SCORE GetUserControlScore(LPCTSTR pszScore);
	//转换字符
	VOID SwitchScoreString(SCORE lScore, LPTSTR pszBuffer, WORD wBufferSize);
	//转换字符
	VOID NumberConvert(TCHAR* szNum, TCHAR szResult[], int nLen);

};

///////////////////////////////////////////////////////////////////////////////////////////////////
//游戏银行
class CDlgInsureGame : public CFGuiDialog
{
	//标识变量
public:
	bool							m_bSwitchIng;					//转换标识	
	bool							m_blCanStore;                   //存款标识
	bool                            m_blCanGetOnlyFree;             //取款标识

	//银行信息
public:
	WORD							m_wRevenueTake;					//税收比例
	WORD							m_wRevenueTransfer;				//税收比例
	SCORE							m_lUserScore;					//用户游戏币
	SCORE							m_lUserInsure;					//银行游戏币
	
	//界面控件
protected:
	CEditUI *						m_pEditAmount;					//数量编辑	
	CEditUI *						m_pEditPassword;				//密码编辑
	CTextUI *						m_pTextForget;					//忘记密码

	//界面控件
protected:
	CButtonUI *						m_pButtonSave;					//存款按钮
	CButtonUI *						m_pButtonTake;					//取款按钮
	CButtonUI *						m_pButtonRefresh;				//刷新按钮
	CEditUI*						m_pEditInOut;
	CEditUI*						m_pEditPassWord;
	CLabelUI*						m_LabelInOut;

	SCORE			m_InOutScore;		//输入的分数
	//接口变量
protected:	
	IClientUserItem	*				m_pIMeUserData;					//用户信息
	IClientKernel *					m_pIClientKernel;				//内核接口

	//函数定义
public:
	//构造函数
	CDlgInsureGame();
	//析构函数
	virtual ~CDlgInsureGame();
	//重载函数
public:		
	//确认消息
	virtual VOID OnOK();
	//取消消息
	virtual VOID OnCancel();	
	//初始化函数
	virtual BOOL OnInitDialog();
    //接口函数
public:
	//初始控件
	virtual void InitControlUI();
	//消息提醒
	virtual void Notify(TNotifyUI &  msg);	
	//皮肤名称
	virtual LPCTSTR GetSkinFile();

	//辅助函数
public:

	//加载数据
	VOID LoadDataFromUI();

	//界面操控
	VOID UpdateUI();

	//显示大写
	VOID ShowCapital();
	VOID SetAccessEditNum( SCORE  dwNum);
	VOID OnBnClickAccess(TNotifyUI& msg);
	//辅助函数
public:

	//转换字符
	VOID SwitchScoreFormat(SCORE lScore, UINT uSpace, LPTSTR pszBuffer, WORD wBufferSize);

	//控件函数
protected:

	//存款按钮
	VOID OnBnClickedSaveScore();
	//取款按钮
	VOID OnBnClickedTakeScore();
	//刷新按钮
	VOID OnBnClickedRefreshScore();
	//忘记密码
	VOID OnBnClickedForgetInsure();

	//接口函数
public:
	//显示银行
	void ShowItem();
	//设置位置
	void SetPostPoint(CPoint Point);

	//功能函数
public:
	//设置组件
	void SetSendInfo(IClientKernel *pClientKernel,IClientUserItem const*pMeUserDatas);
	//发送信息
	void SendSocketData(WORD wMainCmdID, WORD wSubCmdID, void * pBuffer, WORD wDataSize);

};

///////////////////////////////////////////////////////////////////////////////////////////////////

#endif