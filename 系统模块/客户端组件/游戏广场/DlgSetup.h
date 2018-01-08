#ifndef DLG_DLGSETUP_HEAD_FILE
#define DLG_DLGSETUP_HEAD_FILE
#pragma once
//////////////////////////////////////////////////////////////////////////////////

//设置窗口
class CDlgSetup : public CFGuiDialog
{
public:
	//构造函数
	CDlgSetup();
	//析构函数
	virtual ~CDlgSetup();

	//MFC重载
protected:
	//创建函数
	virtual BOOL OnInitDialog();
	//确定函数
	virtual VOID OnOK();
	//取消消息
	virtual VOID OnCancel();

	//FGUI重载
public:
	//初始函数
	virtual void InitControlUI();
	//皮肤函数
	virtual LPCTSTR GetSkinFile();
	//消息函数
	virtual void Notify(TNotifyUI &  msg);

	//用户函数
public:
	//初始数据
	VOID InitDataLocation();
	//加载数据
	VOID LoadDataFromMemory();
	//加载数据
	VOID LoadDataFromUI();
	//刷新数据
	VOID UpdateDataToMemory();
	//刷新数据
	VOID UpdateDataToUI();
	//默认函数
	VOID OnDefault();
	//应用函数
	VOID DoApply();
	//注册函数
	void RegeditHotkey();

public:
	//缓冲数据
	WORD							m_wBossHotKey;						//老板热键
	WORD							m_wWhisperHotKey;					//私聊热键
	bool							m_bSendWhisperByEnter;				//回车发送
	bool							m_bFullScreenBrowse;				//全屏浏览
	bool							m_bAutoReceiveBase;					//领取低保
	bool							m_bNotifyUserInOut;					//进出消息
	bool							m_bSalienceTable;					//突出桌子

	//默认数据
	WORD							m_wDefaultBossHotKey;				
	WORD							m_wDefaultWhisperHotKey;	
	bool							m_bDefaultSendWhisperByEnter;	
	bool							m_bDefaultFullScreenBrowse;			
	bool							m_bDefaultAutoReceiveBase;			
	bool							m_bDefaultNotifyUserInOut;			
	bool							m_bDefaultSalienceTable;			
};

//////////////////////////////////////////////////////////////////////////////////

#endif