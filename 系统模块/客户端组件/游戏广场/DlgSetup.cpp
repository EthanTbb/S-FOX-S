#include "Stdafx.h"
#include "Resource.h"
#include "DlgSetup.h"
#include "DlgInformation.h"
//////////////////////////////////////////////////////////////////////////////////
//控件名字
const TCHAR* const szButtonCloseControlName = TEXT("ButtonClose");
const TCHAR* const szButtonDefaultControlName = TEXT("ButtonDefault");
const TCHAR* const szButtonComfirmControlName = TEXT("ButtonComfirm");
const TCHAR* const szButtonCancleControlName = TEXT("ButtonCancle");
const TCHAR* const szHotKeyBossControlName = TEXT("HotKeyBoss");
const TCHAR* const szHotKeyChatControlName = TEXT("HotKeyChat");
const TCHAR* const szRadioButtonEnterControlName = TEXT("RadioButtonEnter");
const TCHAR* const szRadioButtonCtrlEnterControlName = TEXT("RadioButtonCtrlEnter");
const TCHAR* const szCheckButtonFullControlName = TEXT("CheckButtonFull");
const TCHAR* const szCheckButtonBaseControlName = TEXT("CheckButtonBase");
const TCHAR* const szCheckButtonInOutControlName = TEXT("CheckButtonInOut");
const TCHAR* const szCheckButtonDeskControlName = TEXT("CheckButtonDesk");
//////////////////////////////////////////////////////////////////////////////////

//构造函数
CDlgSetup::CDlgSetup() : CFGuiDialog(IDD_DLG_POPUP)
{
}

//析构函数
CDlgSetup::~CDlgSetup()
{
}

//创建函数
BOOL CDlgSetup::OnInitDialog()
{
	//设置标题
	SetWindowText(TEXT("系统设置"));

	//居中窗口
	CenterWindow(this);

	//初始数据
	InitDataLocation();

	LoadDataFromMemory();

	UpdateDataToUI();

	return TRUE;
}

//确定函数
VOID CDlgSetup::OnOK()
{
	DoApply();
}

//取消消息
VOID CDlgSetup::OnCancel()
{
	return __super::OnCancel();
}

//初始函数
void CDlgSetup::InitControlUI()
{
}

//皮肤函数
LPCTSTR CDlgSetup::GetSkinFile()
{
	return TEXT("DlgSetup.xml"); 
}

//消息函数
void CDlgSetup::Notify(TNotifyUI &  msg)
{
	//获取对象
	CControlUI * pControlUI = msg.pSender;

	//点击事件
	if (lstrcmp(msg.sType, TEXT("click")) == 0)
	{
		if(lstrcmp(pControlUI->GetName(), szButtonCloseControlName)==0)
		{
			return OnCancel();
		}
		else if(lstrcmp(pControlUI->GetName(), szButtonCancleControlName)==0)
		{
			return OnCancel();
		}
		else if(lstrcmp(pControlUI->GetName(), szButtonComfirmControlName)==0)
		{
			return OnOK();
		}
		else if(lstrcmp(pControlUI->GetName(), szButtonDefaultControlName)==0)
		{
			return OnDefault();
		}
	}
	return;
}


void CDlgSetup::InitDataLocation()
{
	m_wDefaultWhisperHotKey			 = MAKEWORD(VK_F11,HOTKEYF_ALT);
	m_wDefaultBossHotKey		     = MAKEWORD(VK_F12,HOTKEYF_CONTROL);
	m_bDefaultSendWhisperByEnter	 = true;
	m_bDefaultFullScreenBrowse       = false;
	m_bDefaultAutoReceiveBase        = false;
	m_bDefaultSalienceTable          = true;
	m_bDefaultNotifyUserInOut        = false;

	m_wWhisperHotKey		  = m_wDefaultWhisperHotKey	;	
	m_wBossHotKey		      = m_wDefaultBossHotKey;	
	m_bSendWhisperByEnter     = m_bDefaultSendWhisperByEnter;
	m_bFullScreenBrowse       = m_bDefaultFullScreenBrowse;
	m_bAutoReceiveBase        = m_bDefaultAutoReceiveBase ;
	m_bSalienceTable          = m_bDefaultSalienceTable   ;
	m_bNotifyUserInOut        = m_bDefaultNotifyUserInOut ;
}

void CDlgSetup::LoadDataFromMemory()
{
	//变量定义
	ASSERT(CParameterGlobal::GetInstance()!=NULL);
	CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();

	m_wBossHotKey         = pParameterGlobal->m_wBossHotKey;
	m_wWhisperHotKey      = pParameterGlobal->m_wWhisperHotKey;
	m_bSendWhisperByEnter = pParameterGlobal->m_bSendWhisperByEnter;
	m_bFullScreenBrowse   = pParameterGlobal->m_bFullScreenBrowse;
	m_bAutoReceiveBase    = pParameterGlobal->m_bAutoReceiveBase;
	m_bSalienceTable      = pParameterGlobal->m_bSalienceTable;
	m_bNotifyUserInOut    = pParameterGlobal->m_bNotifyUserInOut;

}

void CDlgSetup::UpdateDataToMemory()
{
	//变量定义
	ASSERT(CParameterGlobal::GetInstance()!=NULL);
	CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();

	pParameterGlobal->m_wBossHotKey				=  m_wBossHotKey; 
	pParameterGlobal->m_wWhisperHotKey			=  m_wWhisperHotKey;
	pParameterGlobal->m_bSendWhisperByEnter		=  m_bSendWhisperByEnter;
	pParameterGlobal->m_bFullScreenBrowse		=  m_bFullScreenBrowse;
	pParameterGlobal->m_bAutoReceiveBase		=  m_bAutoReceiveBase; 
	pParameterGlobal->m_bSalienceTable			=  m_bSalienceTable;
	pParameterGlobal->m_bNotifyUserInOut		=  m_bNotifyUserInOut; 

}

void CDlgSetup::LoadDataFromUI()
{
	CHotKeyUI * pHotKeyBoss = (CHotKeyUI * )GetControlByName(szHotKeyBossControlName);
	if(pHotKeyBoss!=NULL)
	{
		m_wBossHotKey = (WORD)(pHotKeyBoss->GetHotKey());
	}

	CHotKeyUI * pHotKeyChat = (CHotKeyUI * )GetControlByName(szHotKeyChatControlName);
	if(pHotKeyChat!=NULL)
	{
		m_wWhisperHotKey = (WORD)(pHotKeyChat->GetHotKey());
	}

	CRadioButtonUI *pRadioButtonEnter =  (CRadioButtonUI * )GetControlByName(szRadioButtonEnterControlName);
	if(pRadioButtonEnter!=NULL)
	{
		m_bSendWhisperByEnter = pRadioButtonEnter->GetCheck();
	}


	CCheckButtonUI *pCheckButtonFull =  (CCheckButtonUI * )GetControlByName(szCheckButtonFullControlName);
	if(pCheckButtonFull!=NULL)
	{
		m_bFullScreenBrowse = pCheckButtonFull->GetCheck();
	}

	CCheckButtonUI *pCheckButtonBase =  (CCheckButtonUI * )GetControlByName(szCheckButtonBaseControlName);
	if(pCheckButtonBase!=NULL)
	{
		m_bAutoReceiveBase = pCheckButtonBase->GetCheck();
	}

	CCheckButtonUI *pCheckButtonInOut =  (CCheckButtonUI * )GetControlByName(szCheckButtonInOutControlName);
	if(pCheckButtonInOut!=NULL)
	{
		m_bNotifyUserInOut = pCheckButtonInOut->GetCheck();
	}

	CCheckButtonUI *pCheckButtonDesk =  (CCheckButtonUI * )GetControlByName(szCheckButtonDeskControlName);
	if(pCheckButtonDesk!=NULL)
	{
		m_bSalienceTable = pCheckButtonDesk->GetCheck();
	}
}


void CDlgSetup::UpdateDataToUI()
{
	CHotKeyUI * pHotKeyBoss = (CHotKeyUI * )GetControlByName(szHotKeyBossControlName);
	if(pHotKeyBoss!=NULL)
	{
		pHotKeyBoss->SetHotKey(LOBYTE(m_wBossHotKey),HIBYTE(m_wBossHotKey));
	}

	CHotKeyUI * pHotKeyChat = (CHotKeyUI * )GetControlByName(szHotKeyChatControlName);
	if(pHotKeyChat!=NULL)
	{
		pHotKeyChat->SetHotKey(LOBYTE(m_wWhisperHotKey),HIBYTE(m_wWhisperHotKey));
	}

	CRadioButtonUI *pRadioButtonEnter =  (CRadioButtonUI * )GetControlByName(szRadioButtonEnterControlName);
	if(pRadioButtonEnter!=NULL)
	{
		pRadioButtonEnter->SetCheck(m_bSendWhisperByEnter);
	}

	CRadioButtonUI *pRadioButtonCtrlEnter =  (CRadioButtonUI * )GetControlByName(szRadioButtonCtrlEnterControlName);
	if(pRadioButtonCtrlEnter!=NULL)
	{
		pRadioButtonCtrlEnter->SetCheck(!m_bSendWhisperByEnter);
	}

	CCheckButtonUI *pCheckButtonFull =  (CCheckButtonUI * )GetControlByName(szCheckButtonFullControlName);
	if(pCheckButtonFull!=NULL)
	{
		pCheckButtonFull->SetCheck(m_bFullScreenBrowse);
	}

	CCheckButtonUI *pCheckButtonBase =  (CCheckButtonUI * )GetControlByName(szCheckButtonBaseControlName);
	if(pCheckButtonBase!=NULL)
	{
		pCheckButtonBase->SetCheck(m_bAutoReceiveBase);
	}

	CCheckButtonUI *pCheckButtonInOut =  (CCheckButtonUI * )GetControlByName(szCheckButtonInOutControlName);
	if(pCheckButtonInOut!=NULL)
	{
		pCheckButtonInOut->SetCheck(m_bNotifyUserInOut);
	}

	CCheckButtonUI *pCheckButtonDesk =  (CCheckButtonUI * )GetControlByName(szCheckButtonDeskControlName);
	if(pCheckButtonDesk!=NULL)
	{
		pCheckButtonDesk->SetCheck(m_bSalienceTable);
	}
}

void CDlgSetup::OnDefault()
{
	InitDataLocation();
	UpdateDataToUI();
	RegeditHotkey();
}

void CDlgSetup::DoApply()
{
	LoadDataFromUI();
	UpdateDataToMemory();
	RegeditHotkey();
}

void CDlgSetup::RegeditHotkey()
{
	//老板热键
	UnregisterHotKey(AfxGetMainWnd()->GetSafeHwnd(),IDI_HOT_KEY_BOSS);
	if (CWHService::RegisterHotKey(AfxGetMainWnd()->GetSafeHwnd(),IDI_HOT_KEY_BOSS,m_wBossHotKey)==false)
	{
		CDlgInformation Information(this);
		Information.ShowMessageBox(TEXT("系统热键冲突，老板热键注册失败！"));
	}

	//私聊热键
	UnregisterHotKey(AfxGetMainWnd()->GetSafeHwnd(),IDI_HOT_KEY_WHISPER);
	if (CWHService::RegisterHotKey(AfxGetMainWnd()->GetSafeHwnd(),IDI_HOT_KEY_WHISPER,m_wWhisperHotKey)==false)
	{
		CDlgInformation Information(this);
		Information.ShowMessageBox(TEXT("系统热键冲突，提取消息热键注册失败！"));
	}
}
//////////////////////////////////////////////////////////////////////////////////