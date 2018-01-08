#include "Stdafx.h"
#include "Resource.h"
#include "GlobalUnits.h"
#include "DlgGameOption.h"

//��ť�ؼ�
const TCHAR* const szButtonCloseControlName = TEXT("ButtonClose");
const TCHAR* const szButtonDefaultControlName = TEXT("ButtonDefault");
const TCHAR* const szButtonComfirmControlName = TEXT("ButtonComfirm");
const TCHAR* const szButtonCancleControlName = TEXT("ButtonCancle");

//��ѡ�ؼ�
const TCHAR* const szCheckAllowLookonControlName = TEXT("CheckAllowLookon");
const TCHAR* const szCheckFullGameViewControlName = TEXT("CheckFullGameView");
const TCHAR* const szCheckAllowSoundControlName = TEXT("CheckAllowSound");
const TCHAR* const szCheckAllowBackGroundSoundControlName = TEXT("CheckAllowBackGroundSound");
const TCHAR* const szCheckNotifyUserInOutControlName = TEXT("CheckNotifyUserInOut");
const TCHAR* const szCheckDebarDetestChatControlName = TEXT("CheckDebarDetestChat");


//////////////////////////////////////////////////////////////////////////

//���캯��
CDlgGameOption::CDlgGameOption() : CFGuiDialog(IDD_GAME_RULE)
{


}

//��������
CDlgGameOption::~CDlgGameOption()
{

}


//��ʼ������
BOOL CDlgGameOption::OnInitDialog()
{
	__super::OnInitDialog();

	//���д���
	CenterWindow(AfxGetMainWnd());

	//���ñ���
	SetWindowText(TEXT("��Ϸ����"));

	LoadDataFromMemory();
	UpdateUI();

	return FALSE;
}

//ȷ����Ϣ
VOID CDlgGameOption::OnOK() 
{
	DoApply();

	//��ѯ�ӿ�
	ASSERT(g_GlobalUnits.QueryGlobalModule(MODULE_GAME_FRAME_WND,IID_IGameFrameWnd,VER_IGameFrameWnd));
	IGameFrameWnd * pIGameFrameWnd=(IGameFrameWnd *)g_GlobalUnits.QueryGlobalModule(MODULE_GAME_FRAME_WND,IID_IGameFrameWnd,VER_IGameFrameWnd);

	//Ӧ��ѡ��
	if (pIGameFrameWnd!=NULL) 
	{
		pIGameFrameWnd->OnGameOptionChange();
	}

	__super::OnOK();
}

VOID CDlgGameOption::OnCancel()
{
	return __super::OnCancel();
}

//��ʼ�ؼ�
void CDlgGameOption::InitControlUI()
{
	__super::InitControlUI();
}

//��Ϣ����
void CDlgGameOption::Notify(TNotifyUI &  msg)
{
	//��ȡ����
	CControlUI * pControlUI = msg.pSender;

	//����¼�
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
}


LPCTSTR CDlgGameOption::GetSkinFile()
{
    return TEXT("GameDlgOption.xml"); 
}

VOID CDlgGameOption::InitData()
{
	//��������
	m_bDefaultMuteStatuts=true;
	m_bDefaultAllowSound=true;
	m_bDefaultAllowBackGroundSound=true;
	m_bDefaultFullGameView=false;
	m_bDefaultNotifyUserInOut=true;
	m_bDefaultDebarDetestChat=true;
	m_bDefaultAllowLookon=true;
	m_bDefaultAllowFriendLookon=true;

	m_bMuteStatuts			=	m_bDefaultMuteStatuts;
	m_bAllowSound			=	m_bDefaultAllowSound;
	m_bAllowBackGroundSound	=	m_bDefaultAllowBackGroundSound;
	m_bFullGameView			=	m_bDefaultFullGameView;
	m_bNotifyUserInOut		=	m_bDefaultNotifyUserInOut;
	m_bDebarDetestChat		=	m_bDefaultDebarDetestChat;
	m_bAllowLookon			=	m_bDefaultAllowLookon;
	m_bAllowFriendLookon	=	m_bDefaultAllowFriendLookon;
}

VOID CDlgGameOption::LoadDataFromMemory()
{
	CGlobalUnits *pGlobalUnits=CGlobalUnits::GetInstance();

	m_bAllowLookon			=pGlobalUnits->m_bAllowLookon;
	m_bAllowFriendLookon	=pGlobalUnits->m_bAllowFriendLookon;
	m_bAllowSound			=pGlobalUnits->m_bAllowSound;
	m_bAllowBackGroundSound	=pGlobalUnits->m_bAllowBackGroundSound;
	m_bFullGameView			=pGlobalUnits->m_bFullGameView;
	m_bDebarDetestChat		=pGlobalUnits->m_bDebarDetestChat;
	m_bNotifyUserInOut		=pGlobalUnits->m_bNotifyUserInOut;
}

VOID CDlgGameOption::LoadDataFromUI()
{
	CCheckButtonUI *pCheckAllowLookon =  (CCheckButtonUI * )GetControlByName(szCheckAllowLookonControlName);
	if(pCheckAllowLookon!=NULL)
	{
		m_bAllowLookon = pCheckAllowLookon->GetCheck();
	}

	CCheckButtonUI *pCheckFullGameView =  (CCheckButtonUI * )GetControlByName(szCheckFullGameViewControlName);
	if(pCheckFullGameView!=NULL)
	{
		m_bFullGameView = pCheckFullGameView->GetCheck();
	}

	CCheckButtonUI *pCheckAllowSound =  (CCheckButtonUI * )GetControlByName(szCheckAllowSoundControlName);
	if(pCheckAllowSound!=NULL)
	{
		m_bAllowSound = pCheckAllowSound->GetCheck();
	}

	CCheckButtonUI *pAllowBackGroundSound =  (CCheckButtonUI * )GetControlByName(szCheckAllowBackGroundSoundControlName);
	if(pAllowBackGroundSound!=NULL)
	{
		m_bAllowBackGroundSound = pAllowBackGroundSound->GetCheck();
	}

	CCheckButtonUI *pNotifyUserInOut =  (CCheckButtonUI * )GetControlByName(szCheckNotifyUserInOutControlName);
	if(pNotifyUserInOut!=NULL)
	{
		m_bNotifyUserInOut = pNotifyUserInOut->GetCheck();
	}

	CCheckButtonUI *pDebarDetestChat =  (CCheckButtonUI * )GetControlByName(szCheckDebarDetestChatControlName);
	if(pDebarDetestChat!=NULL)
	{
		m_bDebarDetestChat = pDebarDetestChat->GetCheck();
	}
}

VOID CDlgGameOption::UpdateData()
{
	CGlobalUnits *pGlobalUnits=CGlobalUnits::GetInstance();

	pGlobalUnits->m_bAllowLookon			=m_bAllowLookon;
	pGlobalUnits->m_bAllowFriendLookon		=m_bAllowFriendLookon;
	pGlobalUnits->m_bAllowSound				=m_bAllowSound;
	pGlobalUnits->m_bAllowBackGroundSound	=m_bAllowBackGroundSound;
	pGlobalUnits->m_bFullGameView			=m_bFullGameView;
	pGlobalUnits->m_bDebarDetestChat		=m_bDebarDetestChat;
	pGlobalUnits->m_bNotifyUserInOut		=m_bNotifyUserInOut;
}

VOID CDlgGameOption::InitUI()
{

}

VOID CDlgGameOption::UpdateUI()
{
	CCheckButtonUI *pCheckAllowLookon =  (CCheckButtonUI * )GetControlByName(szCheckAllowLookonControlName);
	if(pCheckAllowLookon!=NULL)
	{
		 pCheckAllowLookon->SetCheck(m_bAllowLookon);
	}

	CCheckButtonUI *pCheckFullGameView =  (CCheckButtonUI * )GetControlByName(szCheckFullGameViewControlName);
	if(pCheckFullGameView!=NULL)
	{
		 pCheckFullGameView->SetCheck(m_bFullGameView);
	}

	CCheckButtonUI *pCheckAllowSound =  (CCheckButtonUI * )GetControlByName(szCheckAllowSoundControlName);
	if(pCheckAllowSound!=NULL)
	{
		 pCheckAllowSound->SetCheck(m_bAllowSound);
	}

	CCheckButtonUI *pAllowBackGroundSound =  (CCheckButtonUI * )GetControlByName(szCheckAllowBackGroundSoundControlName);
	if(pAllowBackGroundSound!=NULL)
	{
		 pAllowBackGroundSound->SetCheck(m_bAllowBackGroundSound);
	}

	CCheckButtonUI *pNotifyUserInOut =  (CCheckButtonUI * )GetControlByName(szCheckNotifyUserInOutControlName);
	if(pNotifyUserInOut!=NULL)
	{
		 pNotifyUserInOut->SetCheck(m_bNotifyUserInOut);
	}

	CCheckButtonUI *pDebarDetestChat =  (CCheckButtonUI * )GetControlByName(szCheckDebarDetestChatControlName);
	if(pDebarDetestChat!=NULL)
	{
		 pDebarDetestChat->SetCheck(m_bDebarDetestChat);
	}
}

VOID CDlgGameOption::OnDefault()
{
	InitData();
	UpdateUI();
}

VOID CDlgGameOption::DoApply()
{
	LoadDataFromUI();
	UpdateData();
}

bool CDlgGameOption::CreateGameOption( IUnknownEx * pIUnknownEx[], WORD wItemCount )
{
	//��ʾ����
	if (DoModal()==IDOK)
	{
		return true;
	}

	return false;

}

//////////////////////////////////////////////////////////////////////////
