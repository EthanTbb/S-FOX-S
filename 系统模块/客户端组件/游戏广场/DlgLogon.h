#ifndef DLG_LOGON_HEAD_FILE
#define DLG_LOGON_HEAD_FILE

#pragma once

#include "DlgAccredit.h"
#include "Stdafx.h"
#include "PlatformPublicize.h"

//////////////////////////////////////////////////////////////////////////////////

//登录方式
#define LOGON_BY_VISITOR			2									//游客登录
#define LOGON_BY_GAME_ID			1									//I D 登录
#define LOGON_BY_ACCOUNTS			0									//帐号登录

//////////////////////////////////////////////////////////////////////////////////

//帐号信息
struct tagAccountsInfo
{
	DWORD							dwUserID;							//用户标识
	TCHAR							szGameID[16];						//游戏标识
	TCHAR							szAccounts[LEN_ACCOUNTS];			//登录帐号
	TCHAR							szPassword[LEN_PASSWORD];			//登录密码
	time_t							tAcitveTime;						//登录时间
};

//数组定义
typedef CWHArray<tagAccountsInfo *>	CAccountsInfoArray;					//帐号数组

//////////////////////////////////////////////////////////////////////////////////

//登录窗口
class CDlgLogon : public CFGuiDialog
{

	//函数定义
public:
	//构造函数
	CDlgLogon();
	//析构函数
	virtual ~CDlgLogon();

	//重载函数
protected:
	//消息解释
	virtual BOOL PreTranslateMessage(MSG * pMsg);
	//创建函数
	virtual BOOL OnInitDialog();
	//确定函数
	virtual VOID OnOK();
	//取消消息
	virtual VOID OnCancel();

	//接口函数
public:
	//皮肤名称
	virtual LPCTSTR GetSkinFile(); 
	//初始控件
	virtual void InitControlUI();
	//消息提醒
    virtual void Notify(TNotifyUI &  msg);

	//功能函数
public:
	//显示验证
	bool ShowAccreditWindow();
	//构造数据
	WORD ConstructLogonPacket(BYTE cbBuffer[], WORD wBufferSize, BYTE cbValidateFlags);

	//辅助函数
private:
	//获取信息
	bool GetInformation();
	//加载帐号
	void LoadAccountsInfo();
	//地址信息
	void LoadLogonServerInfo();

	//按钮消息
protected:
	//注册帐号
	void OnBnClickedRegister();
	//忘记密码
	void OnBnForgetPassword();
	//删除记录
	void OnBnClickedDeleteRecord(int index);

	//控件函数
protected:
	//选择改变
	void OnSelchangeAccounts();
	//编辑改变
	void OnCbnEditchangeAccounts();

	//配置变量
public:
	BYTE							m_cbLogonMode;						//登录模式
	BYTE							m_cbRemPassword;					//记住密码

	//登录信息
public:
	DWORD							m_dwGameID;							//游戏 I D
	TCHAR							m_szAccounts[LEN_ACCOUNTS];			//游戏帐号
	TCHAR							m_szPassword[LEN_PASSWORD];			//用户密码
	TCHAR							m_szLogonServer[LEN_SERVER];		//登录地址
	TCHAR							m_szPassPortID[LEN_PASS_PORT_ID];	//身份证号
	//界面控件
protected:
	CDlgAccredit *					m_pDlgAccedit;						//验证窗口
	//变量定义
protected:
	CAccountsInfoArray				m_AccountsInfoArray;				//帐号信息
};

//////////////////////////////////////////////////////////////////////////////////

#endif