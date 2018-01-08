#ifndef DLG_DLGPRESENT_PROOF_HEAD_FILE
#define DLG_DLGPRESENT_PROOF_HEAD_FILE


#pragma once

//#include "InsureCommon.h"

struct tagTransferScoreInfo
{
	DWORD							dwTargetID;							//用户 I D
	SCORE							lTransferScore;						//转账金币
	TCHAR							szTargetAccounts[LEN_NICKNAME];		//目标用户
	TCHAR							szTransRemark[LEN_TRANS_REMARK];	//转账备注
};

class CDlgPresentProof: public CFGuiDialog
{
public:
	//函数定义
public:
	//构造函数
	CDlgPresentProof();
	//析构函数
	virtual ~CDlgPresentProof();

	//重载函数
protected:
	//创建函数
	virtual BOOL OnInitDialog();
	//确定函数
	virtual VOID OnOK();
	//取消消息
	virtual VOID OnCancel();

	//接口函数
public:
	//初始控件
	virtual void InitControlUI();
	//消息提醒
	virtual void Notify(TNotifyUI &  msg);

	//重载函数
public:
	//皮肤名称
	virtual LPCTSTR GetSkinFile();

	//辅助函数
public:

	//功能函数
public:
	//转账信息
	void SetTransferScoreInfo(tagTransferScoreInfo TransferScoreInfo);
	BOOL SaveBmp(HBITMAP hBitmap, CString FileName);
	//转账信息
protected:
	tagTransferScoreInfo						m_tagTransferScoreInfo;
};

#endif