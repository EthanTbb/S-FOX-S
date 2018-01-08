#ifndef DLG_DLGPHONESCANCODE_HEAD_FILE
#define DLG_DLGPHONESCANCODE_HEAD_FILE
#pragma once
#include "ServerListData.h"
//////////////////////////////////////////////////////////////////////////////////

//二维码窗口
class CPhoneScanCode : public CFGuiDialog
{
	//函数定义
public:
	//构造函数
	CPhoneScanCode();
	//析构函数
	virtual ~CPhoneScanCode();

	//重载函数
protected:
	//创建函数
	virtual BOOL OnInitDialog();
	//确定函数
	virtual VOID OnOK();
	//取消消息
	virtual VOID OnCancel();

	//FGui重载
public:
	//初始函数
	virtual void InitControlUI();
	//皮肤函数
	virtual LPCTSTR GetSkinFile();
	//消息函数
	virtual void Notify(TNotifyUI &  msg);	

	//用户函数
public:
	//加载数据
	void LoadDataFromExternal(CGameKindItem * pGameKind);
private:
	CGameKindItem * m_pGameKind;
};

//////////////////////////////////////////////////////////////////////////////////

//二维码窗口
class CPhoneScanCodePlaza : public CFGuiDialog
{
	//函数定义
public:
	//构造函数
	CPhoneScanCodePlaza();
	//析构函数
	virtual ~CPhoneScanCodePlaza();

	//重载函数
protected:
	//创建函数
	virtual BOOL OnInitDialog();
	//确定函数
	virtual VOID OnOK();
	//取消消息
	virtual VOID OnCancel();

	//FGui重载
public:
	//初始函数
	virtual void InitControlUI();
	//皮肤函数
	virtual LPCTSTR GetSkinFile();
	//消息函数
	virtual void Notify(TNotifyUI &  msg);	
};

//////////////////////////////////////////////////////////////////////////////////
#endif