#ifndef DLG_REGISTER_HEAD_FILE
#define DLG_REGISTER_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "Resource.h"

//////////////////////////////////////////////////////////////////////////////////

//注册窗口
class CDlgRegister : public CFGuiDialog
{
	//函数定义
public:
	//构造函数
	CDlgRegister();
	//析构函数
	virtual ~CDlgRegister();
	//重载函数
protected:
	//消息解释
	virtual BOOL PreTranslateMessage(MSG * pMsg);
	//配置函数
	virtual BOOL OnInitDialog();
	//确定函数
	virtual VOID OnOK();
	//取消函数
	virtual VOID OnCancel();
	//功能函数
public:
	//构造数据
	WORD ConstructRegisterPacket(BYTE cbBuffer[], WORD wBufferSize, BYTE cbValidateFlags);
	//校验结果
	void OnVerifypIndividualResult(WORD wVerifyMask,bool bVerifyPassage,LPCTSTR pszErrorMsg=NULL);
	//重载函数
public:
	//皮肤名称
	virtual LPCTSTR GetSkinFile(); 
	//初始控件
	virtual void InitControlUI();
	//消息提醒
    virtual void Notify(TNotifyUI &  msg);

	//辅助函数
private:
	//获取信息
	bool GetInformation();
	//即时验证
	void PerformVerifyIndividual(WORD wPreVerifyMask);
	//提示信息
	void SelectedTipInfo(WORD wPreVerifyMask);
	//获取字符
	void GetControlString(CControlUI * pControlUI,TCHAR szString[], WORD wMaxCount);
	//消息处理
public:
	//密码输入
	void OnEnChangeLogonPass();
	//更换验证
	void OnBnClickedChangeValidate();
	//同意条款
	void OnBnClickedAgrrArticle();
public:
	bool							m_bVerifyIndividual;				//验证资料
	WORD							m_wVerifyMask;						//校验掩码							
	//变量定义
public:
	WORD							m_wFaceID;							//头像标识
	BYTE							m_cbGender;							//用户性别
	TCHAR							m_szAccounts[LEN_ACCOUNTS];			//注册帐号
	TCHAR							m_szNickName[LEN_NICKNAME];			//注册昵称
	TCHAR							m_szLogonPass[LEN_PASSWORD];		//登录密码
	TCHAR							m_szInsurePass[LEN_PASSWORD];		//银行密码
	TCHAR							m_szPassPortID[LEN_PASS_PORT_ID];	//证件号码
	TCHAR							m_szCompellation[LEN_COMPELLATION];	//真实名字
	WORD                            m_wLastVerifyAccountsResult;        //上次验证
	WORD                            m_wLastVerifyNickNameResult;        //上次验证
	WORD							m_dwAgentID;							//代理标识
	WORD							m_dwSpreaderGameID;					//推广标识

	//配置变量
protected:
	BYTE							m_cbRemPassword;					//记住密码
	BYTE							m_cbLogonPassLevel;					//密码强度

};

//////////////////////////////////////////////////////////////////////////////////

#endif