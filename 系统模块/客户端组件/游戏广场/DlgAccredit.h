#ifndef DLG_ACCREDIT_HEAD_FILE
#define DLG_ACCREDIT_HEAD_FILE

#pragma once

//////////////////////////////////////////////////////////////////////////////////

//验证窗口
class CDlgAccredit : public CFGuiDialog
{
	//函数定义
public:
	//构造函数
	CDlgAccredit();
	//析构函数
	virtual ~CDlgAccredit();

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
	//消息提醒
    virtual void Notify(TNotifyUI &  msg);
	
	//重载函数
public:
	//皮肤名称
	virtual LPCTSTR GetSkinFile() { return TEXT("DlgAccredit.xml"); } 

	//信息函数
public:
	//获取号码
	LPCTSTR GetPassPortNumber();

	//辅助函数
private:
	//随机数字
	VOID RandPassportNumber();

	//按钮事件
protected:
	//重置按钮
	VOID OnClickedButtonReset(CControlUI * pControlUI);
	//数字按钮
	VOID OnClickedButtonNumber(CControlUI * pControlUI);
	//退格按钮
	VOID OnClickedButtonBackSpace(CControlUI * pControlUI);
};

//////////////////////////////////////////////////////////////////////////////////

#endif