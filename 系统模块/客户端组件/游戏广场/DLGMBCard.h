#ifndef DLG_DLGMBCARD_HEAD_FILE
#define DLG_DLGMBCARD_HEAD_FILE

#pragma once

//////////////////////////////////////////////////////////////////////////////////

//密保窗口
class CDLGMBCard : public CFGuiDialog
{
public:
	UINT								m_uMBCardID;						//机器序列
	CHAR								m_szRandLetter[3];
	BYTE								m_szRandNumber[3];
	BYTE								m_cbErrorCount;
	UINT								m_uCoorNumber[6][4];

	//函数定义
public:
	CDLGMBCard(); 
	virtual ~CDLGMBCard();

	//重载函数
public:
	//配置消息
	virtual BOOL OnInitDialog();
	//按钮消息
	void OnOK();
	//取消消息
	void OnCancel();
	//接口函数
public:
	//消息提醒
    virtual void Notify(TNotifyUI &  msg);
	//重载函数
public:
	//皮肤名称
	virtual LPCTSTR GetSkinFile() { return TEXT("DlgMBCard.xml"); } 
	//辅助函数
public:
	void RandCoordinate();
	//设置密保
	void SetMBCard(UINT uMBCardID);
};

//////////////////////////////////////////////////////////////////////////////////

#endif