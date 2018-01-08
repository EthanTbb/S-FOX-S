#include "stdafx.h"
#include "Resource.h"
#include "DlgCollocateRoom.h"
#include "DlgInformation.h"

//////////////////////////////////////////////////////////////////////////////////
//�ؼ�����

//��ť�ؼ�
const TCHAR * szButtonOKControlName=TEXT("ButtonOK");
const TCHAR * szButtonCloseControlName2=TEXT("ButtonClose");
const TCHAR * szButtonCancelControlName2=TEXT("ButtonCancel");

//�༭�ؼ�
const TCHAR * szEditWinRateControlName=TEXT("EditWinRate");
const TCHAR * szEditFleeRateControlName=TEXT("EditFleeRate");
const TCHAR * szEditScoreMinControlName=TEXT("EditScoreMin");
const TCHAR * szEditScoreMaxControlName=TEXT("EditScoreMax");

//��ѡ�ؼ�
const TCHAR * szCheckOptionWinRateControlName=TEXT("CheckOptionWinRate");
const TCHAR * szCheckOptionFleeRateControlName=TEXT("CheckOptionFleeRate");
const TCHAR * szCheckOptionScoreRangeControlName=TEXT("CheckOptionScoreRange");
const TCHAR * szCheckOptionLimitDetestControlName=TEXT("CheckOptionLimitDetest");
const TCHAR * szCheckOptionLimitSameIPControlName=TEXT("CheckOptionLimitSameIP");

//����ؼ�
const TCHAR * szCheckOptionPwdControlName=TEXT("CheckOptionPwd");
const TCHAR * szEditPwdControlName=TEXT("EditPwd");

//////////////////////////////////////////////////////////////////////////////////

//��������
CDlgCollocateRoom::CDlgCollocateRoom() : CFGuiDialog(IDD_DLG_POPUP)
{
	//���ñ���
	m_pParameterGame=NULL;
	m_pParameterServer=NULL;
}

CDlgCollocateRoom::~CDlgCollocateRoom()
{
}

//���ò���
bool CDlgCollocateRoom::InitCollocate(CParameterGame * pParameterGame, CParameterServer * pParameterServer)
{
	//���ñ���
	m_pParameterGame=pParameterGame;
	m_pParameterServer=pParameterServer;

	//Ĭ�ϲ���
	DefaultParameter();

	return true;
}

//��ʼ�ؼ�
VOID CDlgCollocateRoom::InitControlUI()
{
	__super::InitControlUI();

	//��������
	CEditUI * pEditUIControl=NULL;

	//��������
	pEditUIControl = static_cast<CEditUI *>(GetControlByName(szEditWinRateControlName));
	if(pEditUIControl!=NULL) 
	{
		pEditUIControl->SetMaxChar(5);
		pEditUIControl->SetNumberOnly();
	}

	//��������
	pEditUIControl = static_cast<CEditUI *>(GetControlByName(szEditFleeRateControlName));
	if(pEditUIControl!=NULL) 
	{
		pEditUIControl->SetMaxChar(5);
		pEditUIControl->SetNumberOnly();
	}

	//��������
	pEditUIControl = static_cast<CEditUI *>(GetControlByName(szEditScoreMinControlName));
	if(pEditUIControl!=NULL) 
	{
		pEditUIControl->SetMaxChar(11);
		pEditUIControl->SetNumberOnly();
	}

	//��������
	pEditUIControl = static_cast<CEditUI *>(GetControlByName(szEditScoreMaxControlName));
	if(pEditUIControl!=NULL) 
	{
		pEditUIControl->SetMaxChar(10);
		pEditUIControl->SetNumberOnly();
	}
}

//��Ϣ����
VOID CDlgCollocateRoom::Notify(TNotifyUI &  msg)
{
	__super::Notify(msg);

	//��ȡ����
	CControlUI * pControlUI = msg.pSender;

	//����¼�
	if (lstrcmp(msg.sType, TEXT("click")) == 0)
	{
		if(lstrcmp(pControlUI->GetName(), szButtonOKControlName)==0)
		{
			return OnOK();
		}
		else if((lstrcmp(pControlUI->GetName(), szButtonCancelControlName2)==0)
			   ||(lstrcmp(pControlUI->GetName(), szButtonCloseControlName2)==0))
		{
			return OnCancel();
		}
	}
}

//��ʼ������
BOOL CDlgCollocateRoom::OnInitDialog()
{
	__super::OnInitDialog();

	//��ȡλ��
	CRect rcClient;
	GetClientRect(&rcClient);

	//���ñ���
	SetWindowText(TEXT("��������"));

	//���ô���
	CSize SizeWindow(m_PaintManager.GetInitSize());
	SetWindowPos(NULL, 0, 0, SizeWindow.cx, SizeWindow.cy, SWP_NOZORDER|SWP_NOMOVE);	

	//��������
	ASSERT(CParameterGlobal::GetInstance()!=NULL);
	CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();

	if ((m_pParameterServer!=NULL)&&(m_pParameterGame!=NULL))
	{
		//��Ϸ����
		m_wMinWinRate=m_pParameterGame->m_wMinWinRate;
		m_wMaxFleeRate=m_pParameterGame->m_wMaxFleeRate;
		m_bLimitWinRate=m_pParameterGame->m_bLimitWinRate;
		m_lMaxGameScore=m_pParameterGame->m_lMaxGameScore;
		m_lMinGameScore=m_pParameterGame->m_lMinGameScore;
		m_bLimitFleeRate=m_pParameterGame->m_bLimitFleeRate;
		m_bLimitGameScore=m_pParameterGame->m_bLimitGameScore;

		//��������
		m_bLimitDetest=pParameterGlobal->m_bLimitDetest;
		m_bLimitSameIP=pParameterGlobal->m_bLimitSameIP;

		m_bTakePassword = m_pParameterServer->m_bTakePassword;
		lstrcpyn(m_szTablePassword,m_pParameterServer->m_szPassword,CountArray(m_szTablePassword));
		
	}

	//���¿ؼ�
	UpdateControlStatus();

	return TRUE; 
}

//ȷ������
VOID CDlgCollocateRoom::OnOK()
{
	//�������
	if(SaveParameter()==false)return;

	//��������
	ASSERT(CParameterGlobal::GetInstance()!=NULL);
	CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();

	//��������
	if ((m_pParameterServer!=NULL)&&(m_pParameterGame!=NULL))
	{
		//��������
		pParameterGlobal->m_bLimitDetest=m_bLimitDetest;
		pParameterGlobal->m_bLimitSameIP=m_bLimitSameIP;

		//��Ϸ����
		m_pParameterGame->m_wMinWinRate=m_wMinWinRate;
		m_pParameterGame->m_wMaxFleeRate=m_wMaxFleeRate;
		m_pParameterGame->m_lMaxGameScore=m_lMaxGameScore;
		m_pParameterGame->m_lMinGameScore=m_lMinGameScore;
		m_pParameterGame->m_bLimitWinRate=m_bLimitWinRate;
		m_pParameterGame->m_bLimitFleeRate=m_bLimitFleeRate;
		m_pParameterGame->m_bLimitGameScore=m_bLimitGameScore;

		m_pParameterServer->m_bTakePassword = m_bTakePassword;
		lstrcpyn(m_pParameterServer->m_szPassword,m_szTablePassword,CountArray(m_pParameterServer->m_szPassword));
	}

	//���ٴ���
	__super::OnOK();
}

//��ȡ״̬
bool CDlgCollocateRoom::GetButtonCheckStatus(LPCTSTR pszButtonName)
{
	//��ȡ�ؼ�
	CCheckButtonUI * pControlUI= static_cast<CCheckButtonUI *>(GetControlByName(pszButtonName));
	if(pControlUI==NULL) return false;

	return pControlUI->GetCheck();
}

//��ȡ�ַ�
VOID CDlgCollocateRoom::GetControlText(LPCTSTR pszControlName,TCHAR szString[], WORD wMaxCount)
{
	//Ĭ�����
	szString[0]=0;

	//��ȡ�ؼ�
	CControlUI * pControlUI=GetControlByName(pszControlName);
	if(pControlUI==NULL) return;

	//��ȡ�ַ�
	CString strString(pControlUI->GetText());

	//ȥ���ո�
	strString.TrimLeft();
	strString.TrimRight();

	//�����ַ�
	lstrcpyn(szString,strString,wMaxCount);

	return;
}

//�������
bool CDlgCollocateRoom::SaveParameter()
{
	//��ȡ����
	m_bLimitWinRate=GetButtonCheckStatus(szCheckOptionWinRateControlName);
	m_bLimitFleeRate=GetButtonCheckStatus(szCheckOptionFleeRateControlName);
	m_bLimitGameScore=GetButtonCheckStatus(szCheckOptionScoreRangeControlName);
	m_bLimitDetest=GetButtonCheckStatus(szCheckOptionLimitDetestControlName);
	m_bLimitSameIP=GetButtonCheckStatus(szCheckOptionLimitSameIPControlName);



	//��������
	TCHAR szBuffer[128]=TEXT("");
	CControlUI * pControlUI=NULL;

	//���ʤ��	
	GetControlText(szEditWinRateControlName,szBuffer,CountArray(szBuffer));
	m_wMinWinRate=(WORD)(_tstof(szBuffer)*100.0)%10000;

	//�������
	GetControlText(szEditFleeRateControlName,szBuffer,CountArray(szBuffer));
	m_wMaxFleeRate=(WORD)(_tstof(szBuffer)*100.0)%10000;

	//��������
	GetControlText(szEditScoreMaxControlName,szBuffer,CountArray(szBuffer));
	m_lMaxGameScore=(DWORD)_tstoi64(szBuffer);

	//��������
	GetControlText(szEditScoreMinControlName,szBuffer,CountArray(szBuffer));
	m_lMinGameScore=(DWORD)_tstoi64(szBuffer);

	if ((m_bLimitGameScore)&&(m_lMinGameScore>=m_lMaxGameScore))
	{
		//��ʾ��Ϣ
		CDlgInformation Information(this);
		Information.ShowMessageBox(TEXT("�������Ʒ�Χ��������ȷ������ȷ���û������Ʒ�Χ��"),MB_ICONINFORMATION);

		//���ý���
		pControlUI=GetControlByName(szEditScoreMinControlName);
		pControlUI->SetFocus();

		return false;
	}

	//��ȡ����
	CEditUI * pEditPass1Control = (CEditUI *)GetControlByName(szEditPwdControlName);
	if(pEditPass1Control!=NULL) lstrcpyn(m_szTablePassword,pEditPass1Control->GetText(),CountArray(m_szTablePassword));

	//��������
	if(m_szTablePassword[0]!=0) m_bTakePassword = true;

	


	return true;
}

//Ĭ�ϲ���
bool CDlgCollocateRoom::DefaultParameter()
{
	//ʤ������
	m_wMinWinRate=0;
	m_bLimitWinRate=false;

	//��������
	m_wMaxFleeRate=5000;
	m_bLimitFleeRate=false;

	//��������
	m_bLimitGameScore=false;
	m_lMaxGameScore=2147483647L;
	m_lMinGameScore=-2147483646L;

	//��������
	m_bLimitDetest=false;
	m_bLimitSameIP=false;

	m_bTakePassword = false;
	ZeroMemory(m_szTablePassword,sizeof(m_szTablePassword));

	return true;
}

//���¿���
bool CDlgCollocateRoom::UpdateControlStatus()
{
	//��������
	TCHAR szBuffer[128]=TEXT("");
	CControlUI * pControlUI=NULL;

	//���ÿؼ�
	CCheckButtonUI * pCheckOption = static_cast<CCheckButtonUI *>(GetControlByName(szCheckOptionWinRateControlName));
	if(pCheckOption) pCheckOption->SetCheck(m_bLimitWinRate);

	//���ÿؼ�
	pCheckOption = static_cast<CCheckButtonUI *>(GetControlByName(szCheckOptionFleeRateControlName));
	if(pCheckOption) pCheckOption->SetCheck(m_bLimitFleeRate);

	//���ÿؼ�
	pCheckOption = static_cast<CCheckButtonUI *>(GetControlByName(szCheckOptionScoreRangeControlName));
	if(pCheckOption) pCheckOption->SetCheck(m_bLimitGameScore);

	//���ÿؼ�
	pCheckOption = static_cast<CCheckButtonUI *>(GetControlByName(szCheckOptionLimitDetestControlName));
	if(pCheckOption) pCheckOption->SetCheck(m_bLimitDetest);

	//���ÿؼ�
	pCheckOption = static_cast<CCheckButtonUI *>(GetControlByName(szCheckOptionLimitSameIPControlName));
	if(pCheckOption) pCheckOption->SetCheck(m_bLimitSameIP);

	//���ʤ��	
	_sntprintf(szBuffer,CountArray(szBuffer),TEXT("%.2f"),((DOUBLE)(m_wMinWinRate))/100.0);
	pControlUI = GetControlByName(szEditWinRateControlName);
	if(pControlUI) pControlUI->SetText(szBuffer);

	//�������
	_sntprintf(szBuffer,CountArray(szBuffer),TEXT("%.2f"),((DOUBLE)(m_wMaxFleeRate))/100.0);
	pControlUI = GetControlByName(szEditFleeRateControlName);
	if(pControlUI) pControlUI->SetText(szBuffer);

	//��������
	_sntprintf(szBuffer,CountArray(szBuffer),TEXT("%ld"),m_lMinGameScore);
	pControlUI = GetControlByName(szEditScoreMinControlName);
	if(pControlUI) pControlUI->SetText(szBuffer);

	//��������
	_sntprintf(szBuffer,CountArray(szBuffer),TEXT("%ld"),m_lMaxGameScore);
	pControlUI = GetControlByName(szEditScoreMaxControlName);
	if(pControlUI) pControlUI->SetText(szBuffer);

	//����
	//pCheckOption = static_cast<CCheckButtonUI *>(GetControlByName(szCheckOptionPwdControlName));
	//if(pCheckOption) pCheckOption->SetCheck(m_bTakePassword);


	pControlUI = GetControlByName(szEditPwdControlName);
	if(pControlUI) 
	{
		pControlUI->SetText(m_szTablePassword);
		/*pControlUI->SetEnabled(m_bTakePassword);*/
	}



	return true;
}
