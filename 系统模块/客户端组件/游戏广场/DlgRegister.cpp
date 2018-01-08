#include "StdAfx.h"
#include "GamePlaza.h"
#include "DlgRegister.h"
#include "MissionLogon.h"
#include "DlgInformation.h"
//////////////////////////////////////////////////////////////////////////////////

//��Ļλ��
#define LAYERED_SIZE				5									//�ֲ��С

//////////////////////////////////////////////////////////////////////////////////
#define	LAST_VERIFY_NONE			0
#define	LAST_VERIFY_LOCAL			1
#define	LAST_VERIFY_RESULT_FAIL		2
#define	LAST_VERIFY_RESULT_SUCESS	3

#define PERFORM_VERIFY_ACCOUNTS		0
#define PERFORM_VERIFY_NICKNAME		1
#define PERFORM_VERIFY_PASSWORD		2
#define PERFORM_VERIFY_TWICE		3


//////////////////////////////////////////////////////////////////////////////////
//��Ӧ�ؼ�
const TCHAR * szButtonCloseControlName = TEXT("ButtonClose");
const TCHAR * szButtonRegisterControlName = TEXT("ButtonRegister");
const TCHAR * szButtonRefreshControlName = TEXT("ButtonRefresh");
const TCHAR * szButtonArticleControlName = TEXT("ButtonArticle");
const TCHAR * szCheckButtonAgreeControlName = TEXT("CheckButtonAgree");
const TCHAR * szVerifyCodeControlName = TEXT("Verifycode");
const TCHAR * szPowerScaleControlName = TEXT("PowerScaleUIPass");
const TCHAR * szEditAccountsControlName = TEXT("EditAccounts");
const TCHAR * szEditNickNameControlName = TEXT("EditNickName");
const TCHAR * szEditLogonPassControlName = TEXT("EditLogonPass");
const TCHAR * szEditLogonPass1ControlName = TEXT("EditLogonPass1");
const TCHAR * szEditCompellationControlName = TEXT("EditCompellation");
const TCHAR * szEditSpreaderControlName = TEXT("EditSpreader");
const TCHAR * szEditValidateCodeControlName = TEXT("EditValidateCode");
const TCHAR * szLabelAccountsValidateControlName = TEXT("LabelAccountsValidate");
const TCHAR * szLabelAccountsInvalidControlName = TEXT("LabelAccountsInvalid");
const TCHAR * szLabelAccountsTipControlName = TEXT("LabelAccountsTip");
const TCHAR * szLabelNickNameValidateControlName = TEXT("LabelNickNameValidate");
const TCHAR * szLabelNickNameInvalidControlName = TEXT("LabelNickNameInvalid");
const TCHAR * szLabelNickNameTipControlName = TEXT("LabelNickNameTip");
const TCHAR * szLabelLogonPassValidateControlName = TEXT("LabelLogonPassValidate");
const TCHAR * szLabelLogonPassInvalidControlName = TEXT("LabelLogonPassInvalid");
const TCHAR * szLabelLogonPassTipControlName = TEXT("LabelLogonPassTip");
const TCHAR * szLabelLogonPassValidate1ControlName = TEXT("LabelLogonPassValidate1");
const TCHAR * szLabelLogonPassInvalid1ControlName = TEXT("LabelLogonPassInvalid1");
const TCHAR * szLabelLogonPassTip1ControlName = TEXT("LabelLogonPassTip1");
//////////////////////////////////////////////////////////////////////////////////

//���캯��
CDlgRegister::CDlgRegister() : CFGuiDialog(IDD_DLG_POPUP)
{
	//��֤����
	m_wVerifyMask=0;
	m_bVerifyIndividual=false;	

	//���ñ���
	m_wFaceID=0;
	m_cbGender=0;
	m_szAccounts[0]=0;
	m_szNickName[0]=0;
	m_szLogonPass[0]=0;
	m_szInsurePass[0]=0;
	m_szPassPortID[0]=0;
	m_szCompellation[0]=0;
	m_dwAgentID = 0;
	m_dwSpreaderGameID=0;

	//���ñ���
	m_cbRemPassword=FALSE;
	m_cbLogonPassLevel=PASSWORD_LEVEL_0;

	m_wLastVerifyAccountsResult = LAST_VERIFY_NONE;
	m_wLastVerifyNickNameResult = LAST_VERIFY_NONE;



	return;
}

//��������
CDlgRegister::~CDlgRegister()
{
	DestroyWindow();
}

BOOL CDlgRegister::PreTranslateMessage(MSG * pMsg)
{
	//��������
	if ((pMsg->message==WM_KEYDOWN)&&(pMsg->wParam==VK_ESCAPE))
	{
		return TRUE;
	}

	return __super::PreTranslateMessage(pMsg);
}

//���ú���
BOOL CDlgRegister::OnInitDialog()
{
	__super::OnInitDialog();

	//���ñ���
	SetWindowText(TEXT("�û�ע��"));

	return TRUE;
}

//ȷ������
VOID CDlgRegister::OnOK()
{
	//��ȡ��Ϣ
	if (GetInformation()==false) return;

	//���ñ���
	m_bVerifyIndividual=false;

	//���ش���
	ShowWindow(SW_HIDE);

	//ִ��ע��
	CMissionLogon * pMissionLogon=CMissionLogon::GetInstance();
	if (pMissionLogon!=NULL) pMissionLogon->PerformLogonMission(m_cbRemPassword==TRUE);

	DestroyWindow();

	return;
}

//ȡ������
VOID CDlgRegister::OnCancel()
{
	//�رմ���
	EndDialog(IDCANCEL);

	//��ʾ��¼
	ASSERT(CMissionLogon::GetInstance()!=NULL);
	if (CMissionLogon::GetInstance()!=NULL) CMissionLogon::GetInstance()->ShowLogon();

	DestroyWindow();
	
	return;
}

//���͵�¼
WORD CDlgRegister::ConstructRegisterPacket(BYTE cbBuffer[], WORD wBufferSize, BYTE cbValidateFlags)
{
	//ע������
	if(m_bVerifyIndividual==false)
	{
		//��������
		TCHAR szLogonPass[LEN_MD5];
		TCHAR szInsurePass[LEN_MD5];
		CWHEncrypt::MD5Encrypt(m_szLogonPass,szLogonPass);
		CWHEncrypt::MD5Encrypt(m_szInsurePass,szInsurePass);

		//��������
		CMD_GP_RegisterAccounts * pRegisterAccounts=(CMD_GP_RegisterAccounts *)cbBuffer;

		//ϵͳ��Ϣ
		pRegisterAccounts->dwPlazaVersion=VERSION_PLAZA;

		//������ʶ
		CWHService::GetMachineIDEx(pRegisterAccounts->szMachineID);

		//������Ϣ
		pRegisterAccounts->wFaceID=m_wFaceID;
		pRegisterAccounts->cbGender=m_cbGender;
		pRegisterAccounts->cbValidateFlags=cbValidateFlags;
		pRegisterAccounts->dwAgentID = m_dwAgentID;
		pRegisterAccounts->dwSpreaderGameID = m_dwSpreaderGameID;
		lstrcpyn(pRegisterAccounts->szAccounts,m_szAccounts,CountArray(pRegisterAccounts->szAccounts));
		lstrcpyn(pRegisterAccounts->szNickName,m_szNickName,CountArray(pRegisterAccounts->szNickName));
		lstrcpyn(pRegisterAccounts->szLogonPass,szLogonPass,CountArray(pRegisterAccounts->szLogonPass));
		lstrcpyn(pRegisterAccounts->szPassPortID,m_szPassPortID,CountArray(pRegisterAccounts->szPassPortID));
		lstrcpyn(pRegisterAccounts->szCompellation,m_szCompellation,CountArray(pRegisterAccounts->szCompellation));		

		//��������
		CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
		tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();
		lstrcpyn(pGlobalUserData->szPassword,pRegisterAccounts->szLogonPass,CountArray(pGlobalUserData->szPassword));

		//��������
		return sizeof(CMD_GP_RegisterAccounts);
	}
	else
	{
		//��������
		CMD_GP_VerifyIndividual * pVerifyIndividual=(CMD_GP_VerifyIndividual *)cbBuffer;
		WORD wVerifySize = (WORD)(sizeof(cbBuffer)-sizeof(CMD_GP_VerifyIndividual));
		CSendPacketHelper SendPacket(cbBuffer+sizeof(CMD_GP_VerifyIndividual),wVerifySize);

		//����ṹ
		pVerifyIndividual->dwPlazaVersion=VERSION_PLAZA;
		pVerifyIndividual->wVerifyMask=m_wVerifyMask;

		//�û��ʺ�
		if(pVerifyIndividual->wVerifyMask==VERIFY_ACCOUNTS)
		{
			SendPacket.AddPacket(m_szAccounts,DTP_GP_UI_ACCOUNTS);
		}

		//�û��ǳ�
		if(pVerifyIndividual->wVerifyMask==VERIFY_NICKNAME)
		{
			SendPacket.AddPacket(m_szNickName,DTP_GP_UI_NICKNAME);
		}

		//��������
		return sizeof(CMD_GP_ModifyIndividual)+SendPacket.GetDataSize();		 
	}
}

//У����
void CDlgRegister::OnVerifypIndividualResult(WORD wVerifyMask,bool bVerifyPassage,LPCTSTR pszErrorMsg)
{
	if(bVerifyPassage==true)
	{
		//��֤�ʺ�
		if(wVerifyMask==VERIFY_ACCOUNTS)
		{
			//��֤ͨ��
			CControlUI * pLabelAccountValidate = GetControlByName(szLabelAccountsValidateControlName);
			if(pLabelAccountValidate!=NULL) pLabelAccountValidate->SetVisible(true);

			m_wLastVerifyAccountsResult = LAST_VERIFY_RESULT_SUCESS;

			return;
		}

		//��֤�ǳ�
		if(wVerifyMask==VERIFY_NICKNAME)
		{
			//��֤ͨ��
			CControlUI * pLabelNickNameValidate = GetControlByName(szLabelNickNameValidateControlName);
			if(pLabelNickNameValidate!=NULL) pLabelNickNameValidate->SetVisible(true);

			m_wLastVerifyNickNameResult = LAST_VERIFY_RESULT_SUCESS;
			return;
		}
	}
	else
	{
		//��֤�ʺ�
		if(wVerifyMask==VERIFY_ACCOUNTS)
		{
			//�ʺ���Ч
			CControlUI * pLabelAccountInvalid = GetControlByName(szLabelAccountsInvalidControlName);
			if(pLabelAccountInvalid!=NULL)
			{
				pLabelAccountInvalid->SetVisible(true);
				pLabelAccountInvalid->SetText(pszErrorMsg);
			}	

			m_wLastVerifyAccountsResult = LAST_VERIFY_RESULT_FAIL;

			return;
		}

		//��֤�ʺ�
		if(wVerifyMask==VERIFY_NICKNAME)
		{
			//�ʺ���Ч
			CControlUI * pLabelNickNameInvalid = GetControlByName(szLabelNickNameInvalidControlName);
			if(pLabelNickNameInvalid!=NULL)
			{
				pLabelNickNameInvalid->SetVisible(true);
				pLabelNickNameInvalid->SetText(pszErrorMsg);
			}

			m_wLastVerifyNickNameResult = LAST_VERIFY_RESULT_FAIL;
		}
	}

	return;
}

//��Ϣ����
 void CDlgRegister::Notify(TNotifyUI &  msg)
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
		else if(lstrcmp(pControlUI->GetName(), szButtonRegisterControlName)==0)
		{
			return OnOK();
		}
		else if(lstrcmp(pControlUI->GetName(),szButtonRefreshControlName)==0)
		{
			return OnBnClickedChangeValidate();
		}
		else if(lstrcmp(pControlUI->GetName(),szButtonArticleControlName)==0)
		{
			return OnBnClickedAgrrArticle();
		}
	}
	else if(lstrcmp(msg.sType,TEXT("textchanged")) ==0 )
	{	
		if(lstrcmp(pControlUI->GetName(),szEditLogonPassControlName)==0)
		{
			return OnEnChangeLogonPass();
		}
	}
	else if(lstrcmp(msg.sType,TEXT("killfocus")) ==0 )
	{
		if(lstrcmp(pControlUI->GetName(),szEditAccountsControlName)==0)
		{
			return PerformVerifyIndividual(PERFORM_VERIFY_ACCOUNTS);
		}
		else if(lstrcmp(pControlUI->GetName(),szEditNickNameControlName)==0)
		{
			return PerformVerifyIndividual(PERFORM_VERIFY_NICKNAME);
		}
		else if(lstrcmp(pControlUI->GetName(),szEditLogonPassControlName)==0)
		{
			return PerformVerifyIndividual(PERFORM_VERIFY_PASSWORD);
		}
		else if(lstrcmp(pControlUI->GetName(),szEditLogonPass1ControlName)==0)
		{
			return PerformVerifyIndividual(PERFORM_VERIFY_TWICE);
		}
	}
	else if(lstrcmp(msg.sType,TEXT("setfocus")) ==0 )
	{
		if(lstrcmp(pControlUI->GetName(),szEditAccountsControlName)==0)
		{
			return SelectedTipInfo(PERFORM_VERIFY_ACCOUNTS);
		}
		else if(lstrcmp(pControlUI->GetName(),szEditNickNameControlName)==0)
		{
			return SelectedTipInfo(PERFORM_VERIFY_NICKNAME);
		}
		else if(lstrcmp(pControlUI->GetName(),szEditLogonPassControlName)==0)
		{
			return SelectedTipInfo(PERFORM_VERIFY_PASSWORD);
		}
		else if(lstrcmp(pControlUI->GetName(),szEditLogonPass1ControlName)==0)
		{
			return SelectedTipInfo(PERFORM_VERIFY_TWICE);
		}
	}
 }

LPCTSTR CDlgRegister::GetSkinFile()
{
	return TEXT("DlgRegister.xml");
}

 //��ʼ�ؼ�
void CDlgRegister::InitControlUI()
{

}



//��ȡ��Ϣ
bool CDlgRegister::GetInformation()
{
	//��ȡͷ��
	IFaceItemControl * pIFaceItemControl=CFaceItemControl::GetInstance();
	m_wFaceID=rand()%pIFaceItemControl->GetFaceCount();

	//�û��ʺ�
	CControlUI * pEditAccountsUI = GetControlByName(szEditAccountsControlName);
	if(pEditAccountsUI!=NULL) GetControlString(pEditAccountsUI,m_szAccounts,CountArray(m_szAccounts));

	//�û��ǳ�
	//CControlUI * pEditNickNameUI = GetControlByName(szEditNickNameControlName);
	//if(pEditNickNameUI!=NULL) GetControlString(pEditNickNameUI,m_szNickName,CountArray(m_szNickName));
	lstrcpyn(m_szNickName,m_szAccounts,CountArray(m_szNickName));

	//�Ƽ���
	//��ȡĿ¼
	TCHAR szPath[MAX_PATH]=TEXT("");
	CWHService::GetWorkDirectory(szPath, sizeof(szPath));

	//��ȡ����
	TCHAR szFileName[MAX_PATH]=TEXT("");
	_sntprintf(szFileName,CountArray(szFileName),TEXT("%s\\Spreader.ini"),szPath);

	//��ȡ�Ƽ���
	TCHAR	szSpreaderID[MAX_PATH];
	ZeroMemory(szSpreaderID,CountArray(szSpreaderID));
	GetPrivateProfileString(TEXT("SpreaderInfo"),TEXT("SpreaderID"),TEXT(""),szSpreaderID,CountArray(szSpreaderID),szFileName);
	m_dwSpreaderGameID = _ttoi(szSpreaderID);

	//��¼����
	CControlUI *  pEditLogonPassUI = GetControlByName(szEditLogonPassControlName);
	if(pEditLogonPassUI!=NULL) GetControlString(pEditLogonPassUI,m_szLogonPass,CountArray(m_szLogonPass));

	//ȷ������
	TCHAR szLogonPass2[LEN_PASSWORD]=TEXT("");
	//CControlUI * pEditLogonPass1UI = GetControlByName(szEditLogonPass1ControlName);
	//if(pEditLogonPass1UI!=NULL) GetControlString(pEditLogonPass1UI,szLogonPass2,CountArray(szLogonPass2));
	lstrcpyn(szLogonPass2,m_szLogonPass,CountArray(szLogonPass2));


	//��ʵ
	CControlUI * pEditCompellationUI = GetControlByName(szEditCompellationControlName);
	if(pEditCompellationUI!=NULL) GetControlString(pEditCompellationUI,m_szCompellation,CountArray(m_szCompellation));

	//��֤��
	TCHAR szValidateCode[5]=TEXT("");
	CControlUI * pEditValidateCodeUI = GetControlByName(szEditValidateCodeControlName);
	if(pEditValidateCodeUI!=NULL) GetControlString(pEditValidateCodeUI,szValidateCode,CountArray(szValidateCode));

	//��������
	TCHAR szDescribe[128]=TEXT("");
	CUserItemElement * pUserItemElement=CUserItemElement::GetInstance();

	//�ʺ��ж�
	if (pUserItemElement->EfficacyAccounts(m_szAccounts,szDescribe,CountArray(szDescribe))==false)
	{
		//��ʾ��Ϣ
		CDlgInformation Information(this);
		Information.ShowMessageBox(szDescribe,MB_ICONERROR,0);

		//���ý���
		pEditAccountsUI->SetFocus();

		return false;
	}

	////�ǳ��ж�
	//if (pUserItemElement->EfficacyNickName(m_szNickName,szDescribe,CountArray(szDescribe))==false)
	//{
	//	//��ʾ��Ϣ
	//	CDlgInformation Information(this);
	//	Information.ShowMessageBox(szDescribe,MB_ICONERROR,0);

	//	//���ý���
	//	pEditNickNameUI->SetFocus();

	//	return false;
	//}

	//�����ж�
	if (pUserItemElement->EfficacyPassword(m_szLogonPass,szDescribe,CountArray(szDescribe))==false)
	{
		//��ʾ��Ϣ
		CDlgInformation Information(this);
		Information.ShowMessageBox(szDescribe,MB_ICONERROR,0);

		//���ý���
		pEditLogonPassUI->SetFocus();

		return false;
	}

	////�����ж�
	//if (lstrcmp(m_szLogonPass,szLogonPass2)!=0)
	//{
	//	//��ʾ��Ϣ
	//	CDlgInformation Information(this);
	//	Information.ShowMessageBox(TEXT("ȷ�����������벻һ�£����������룡"),MB_ICONERROR,0);

	//	//���ý���
	//	pEditLogonPass1UI->SetFocus();

	//	return false;
	//}

	//Ч���ж�
	CVerifyCodeUI * pVerifyCodeUI = (CVerifyCodeUI *)GetControlByName(szVerifyCodeControlName);
	CString strVerifyCodeString(pVerifyCodeUI->GetCurrCode());
	if (strVerifyCodeString.CompareNoCase(szValidateCode)!=0)
	{
		//��ʾ��Ϣ
		CDlgInformation Information(this);
		Information.ShowMessageBox(TEXT("Ч������֤ʧ�ܣ����������룡"),MB_ICONERROR,0);

		//���ý���
		pEditValidateCodeUI->SetFocus();
		pVerifyCodeUI->Refresh();

		return false;
	}

	//ͬ������
	CCheckButtonUI * pCheckButtonAgree = static_cast<CCheckButtonUI*>(GetControlByName(szCheckButtonAgreeControlName));
	if(pCheckButtonAgree!=NULL && pCheckButtonAgree->GetCheck()==false)
	{
		//��ʾ��Ϣ
		CDlgInformation Information(this);
		Information.ShowMessageBox(TEXT("�빴ѡ����Ϸ���ķ��������"),MB_ICONERROR,0);

		return false;
	}

	//�Ա�ѡ��
	m_cbGender = GENDER_FEMALE;

	//��ȡ����
	CWHRegKey RegAgentID;
	if (RegAgentID.OpenRegKey(TEXT(""), false)==true)
	{
		TCHAR szAgentID[MAX_PATH]={0};
		RegAgentID.GetString(TEXT("InstallName"),szAgentID,CountArray(szAgentID));
		m_dwAgentID = _ttoi(szAgentID);
	}


	return true;
}


void CDlgRegister::SelectedTipInfo( WORD wPreVerifyMask )
{
	//�û��ʺ�
	if(wPreVerifyMask==PERFORM_VERIFY_ACCOUNTS)
	{
		CControlUI * pLabelAccountValidate = GetControlByName(szLabelAccountsValidateControlName);
		if(pLabelAccountValidate!=NULL) pLabelAccountValidate->SetVisible(false);

		CControlUI * pLabelAccountInvalid = GetControlByName(szLabelAccountsInvalidControlName);
		if(pLabelAccountInvalid!=NULL)	pLabelAccountInvalid->SetVisible(false);

		CControlUI * pLabelAccountTip = GetControlByName(szLabelAccountsTipControlName);
		if(pLabelAccountTip!=NULL)
		{
			pLabelAccountTip->SetVisible(true);
		}

	}
	//�û��ǳ�
	else if(wPreVerifyMask==PERFORM_VERIFY_NICKNAME)
	{
		CControlUI * pLabelNickNameValidate = GetControlByName(szLabelNickNameValidateControlName);
		if(pLabelNickNameValidate!=NULL) pLabelNickNameValidate->SetVisible(false);

		CControlUI * pLabelNickNameInvalid = GetControlByName(szLabelNickNameInvalidControlName);
		if(pLabelNickNameInvalid!=NULL)	pLabelNickNameInvalid->SetVisible(false);

		CControlUI * LabelNickNameTip = GetControlByName(szLabelNickNameTipControlName);
		if(LabelNickNameTip!=NULL)
		{
			LabelNickNameTip->SetVisible(true);
		}
	}
	//�û�����
	else if(wPreVerifyMask==PERFORM_VERIFY_PASSWORD)
	{
		CControlUI * pLabelLogonPassValidate = GetControlByName(szLabelLogonPassValidateControlName);
		if(pLabelLogonPassValidate!=NULL) pLabelLogonPassValidate->SetVisible(false);

		CControlUI * pLabelLogonPassInvalid = GetControlByName(szLabelLogonPassInvalidControlName);
		if(pLabelLogonPassInvalid!=NULL)	pLabelLogonPassInvalid->SetVisible(false);

		CControlUI * LabelLogonPassTip = GetControlByName(szLabelLogonPassTipControlName);
		if(LabelLogonPassTip!=NULL)
		{
			LabelLogonPassTip->SetVisible(true);
		}
	}
	//ȷ������
	else if(wPreVerifyMask==PERFORM_VERIFY_TWICE)
	{
		CControlUI * pLabelLogonPassValidate1 = GetControlByName(szLabelLogonPassValidate1ControlName);
		if(pLabelLogonPassValidate1!=NULL) pLabelLogonPassValidate1->SetVisible(false);

		CControlUI * pLabelLogonPassInvalid1 = GetControlByName(szLabelLogonPassInvalid1ControlName);
		if(pLabelLogonPassInvalid1!=NULL)	pLabelLogonPassInvalid1->SetVisible(false);

		CControlUI * LabelLogonPassTip1 = GetControlByName(szLabelLogonPassTip1ControlName);
		if(LabelLogonPassTip1!=NULL)
		{
			LabelLogonPassTip1->SetVisible(true);
		}
	}
}

//��֤����
void CDlgRegister::PerformVerifyIndividual(WORD wPreVerifyMask)
{
	//��������
	TCHAR szDescribe[128]=TEXT("");

	//��֤�ʺ�
	if(wPreVerifyMask==PERFORM_VERIFY_ACCOUNTS)
	{
		//���ر�ǩ
		CControlUI * pLabelAccountInvalid = GetControlByName(szLabelAccountsInvalidControlName);
		if(pLabelAccountInvalid!=NULL) pLabelAccountInvalid->SetVisible(false);

		CControlUI * pLabelAccountTip = GetControlByName(szLabelAccountsTipControlName);
		if(pLabelAccountTip!=NULL)	pLabelAccountTip->SetVisible(false);

		CControlUI * pLabelAccountValidate = GetControlByName(szLabelAccountsValidateControlName);
		if(pLabelAccountValidate!=NULL) pLabelAccountValidate->SetVisible(false);

		//��������
		TCHAR szAccounts[LEN_ACCOUNTS]=TEXT("");	

		//�û��ʺ�
		CControlUI * pEditAccountsUI = GetControlByName(szEditAccountsControlName);
		if(pEditAccountsUI!=NULL) GetControlString(pEditAccountsUI,szAccounts,CountArray(szAccounts));
		if (lstrlen(szAccounts) ==0) return;
		if(m_szAccounts[0]!=0 && lstrcmp(m_szAccounts,szAccounts)==0) 
		{
			if(m_wLastVerifyAccountsResult == LAST_VERIFY_LOCAL || m_wLastVerifyAccountsResult == LAST_VERIFY_RESULT_FAIL)
			{
				if(pLabelAccountInvalid!=NULL) pLabelAccountInvalid->SetVisible(true);
			}
			else if (m_wLastVerifyAccountsResult == LAST_VERIFY_RESULT_SUCESS)
			{
				if(pLabelAccountValidate!=NULL) pLabelAccountValidate->SetVisible(true);
			}

			return;
		}

		//�ʺ��ж�
		CUserItemElement * pUserItemElement=CUserItemElement::GetInstance();
		if (pUserItemElement->EfficacyAccounts(szAccounts,szDescribe,CountArray(szDescribe))==false)
		{
			//�ʺ���Ч
			if(pLabelAccountInvalid!=NULL)
			{
				pLabelAccountInvalid->SetVisible(true);
				pLabelAccountInvalid->SetText(szDescribe);
			}

			m_wLastVerifyAccountsResult = LAST_VERIFY_LOCAL;

			//���ý���
			pEditAccountsUI->SetFocus();
			lstrcpyn(m_szAccounts,szAccounts,CountArray(m_szAccounts));

			return;
		}

		//���ñ���
		m_bVerifyIndividual=true;
		m_wVerifyMask=VERIFY_ACCOUNTS;
		lstrcpyn(m_szAccounts,szAccounts,CountArray(m_szAccounts));

		//ִ��ע��
		CMissionLogon * pMissionLogon=CMissionLogon::GetInstance();
		if (pMissionLogon!=NULL) pMissionLogon->PerformLogonMission(false);

		return;
	}

	//�û��ǳ�
	if(wPreVerifyMask==PERFORM_VERIFY_NICKNAME)
	{
		//���ر�ǩ
		CControlUI * pLabelNickNameInvalid = GetControlByName(szLabelNickNameInvalidControlName);
		if(pLabelNickNameInvalid!=NULL) pLabelNickNameInvalid->SetVisible(false);

		CControlUI * LabelNickNameTip = GetControlByName(szLabelNickNameTipControlName);
		if(LabelNickNameTip!=NULL) LabelNickNameTip->SetVisible(false);

		CControlUI * pLabelNickNameValidate = GetControlByName(szLabelNickNameValidateControlName);
		if(pLabelNickNameValidate!=NULL) pLabelNickNameValidate->SetVisible(false);

		//��������
		TCHAR szNickName[LEN_ACCOUNTS]=TEXT("");	

		//�û��ʺ�
		CControlUI * pEditNickNameUI = GetControlByName(szEditNickNameControlName);
		if(pEditNickNameUI!=NULL) GetControlString(pEditNickNameUI,szNickName,CountArray(szNickName));
		if (lstrlen(szNickName) ==0) return;
		if(m_szNickName[0]!=0 && lstrcmp(m_szNickName,szNickName)==0) 
		{
			if(m_wLastVerifyNickNameResult == LAST_VERIFY_LOCAL || m_wLastVerifyNickNameResult == LAST_VERIFY_RESULT_FAIL)
			{
				if(pLabelNickNameInvalid!=NULL) pLabelNickNameInvalid->SetVisible(true);
			}
			else if (m_wLastVerifyNickNameResult == LAST_VERIFY_RESULT_SUCESS)
			{
				if(pLabelNickNameValidate!=NULL) pLabelNickNameValidate->SetVisible(true);
			}
			return;
		}

		//�ʺ��ж�
		CUserItemElement * pUserItemElement=CUserItemElement::GetInstance();
		if (pUserItemElement->EfficacyNickName(szNickName,szDescribe,CountArray(szDescribe))==false)
		{
			//�ʺ���Ч
			if(pLabelNickNameInvalid!=NULL)
			{
				pLabelNickNameInvalid->SetVisible(true);
				pLabelNickNameInvalid->SetText(szDescribe);
			}

			m_wLastVerifyNickNameResult = LAST_VERIFY_LOCAL;

			//���ý���
			pEditNickNameUI->SetFocus();

			return;
		}

		//���ñ���
		m_bVerifyIndividual=true;
		m_wVerifyMask=VERIFY_NICKNAME;
		lstrcpyn(m_szNickName,szNickName,CountArray(m_szNickName));

		//ִ��ע��
		CMissionLogon * pMissionLogon=CMissionLogon::GetInstance();
		if (pMissionLogon!=NULL) pMissionLogon->PerformLogonMission(false);

		return;
	}

	//�û�����
	if(wPreVerifyMask==PERFORM_VERIFY_PASSWORD)
	{
		CControlUI * pLabelLogonPassValidate = GetControlByName(szLabelLogonPassValidateControlName);
		if(pLabelLogonPassValidate!=NULL) pLabelLogonPassValidate->SetVisible(false);

		CControlUI * pLabelLogonPassInvalid = GetControlByName(szLabelLogonPassInvalidControlName);
		if(pLabelLogonPassInvalid!=NULL)	pLabelLogonPassInvalid->SetVisible(false);

		CControlUI * LabelLogonPassTip = GetControlByName(szLabelLogonPassTipControlName);
		if(LabelLogonPassTip!=NULL)	LabelLogonPassTip->SetVisible(false);

		//��������
		TCHAR szLogonPass[LEN_ACCOUNTS]=TEXT("");	

		CControlUI * pEditLogonPassUI = GetControlByName(szEditLogonPassControlName);
		if(pEditLogonPassUI!=NULL) GetControlString(pEditLogonPassUI,szLogonPass,CountArray(szLogonPass));

		if (lstrlen(szLogonPass) ==0) return;

		//�����ж�
		CUserItemElement * pUserItemElement=CUserItemElement::GetInstance();
		if (pUserItemElement->EfficacyPassword(szLogonPass,szDescribe,CountArray(szDescribe))==false)
		{
			//��ʾ��Ϣ
			if(pLabelLogonPassInvalid!=NULL)
			{
				pLabelLogonPassInvalid->SetVisible(true);
				pLabelLogonPassInvalid->SetText(szDescribe);
			}

			return;
		}

		if(pLabelLogonPassValidate!=NULL)	pLabelLogonPassValidate->SetVisible(true);

	}

	//2������
	if(wPreVerifyMask==PERFORM_VERIFY_TWICE)
	{
		CControlUI * pLabelLogonPassValidate1 = GetControlByName(szLabelLogonPassValidate1ControlName);
		if(pLabelLogonPassValidate1!=NULL) pLabelLogonPassValidate1->SetVisible(false);

		CControlUI * pLabelLogonPassInvalid1 = GetControlByName(szLabelLogonPassInvalid1ControlName);
		if(pLabelLogonPassInvalid1!=NULL)	pLabelLogonPassInvalid1->SetVisible(false);

		CControlUI * LabelLogonPassTip1 = GetControlByName(szLabelLogonPassTip1ControlName);
		if(LabelLogonPassTip1!=NULL) LabelLogonPassTip1->SetVisible(false);

		//��������
		TCHAR szLogonPass[LEN_ACCOUNTS]=TEXT("");
		TCHAR szLogonPass1[LEN_ACCOUNTS]=TEXT("");	

		CControlUI * pEditLogonPassUI = GetControlByName(szEditLogonPassControlName);
		if(pEditLogonPassUI!=NULL) GetControlString(pEditLogonPassUI,szLogonPass,CountArray(szLogonPass));

		CControlUI * pEditLogonPass1UI = GetControlByName(szEditLogonPass1ControlName);
		if(pEditLogonPass1UI!=NULL) GetControlString(pEditLogonPass1UI,szLogonPass1,CountArray(szLogonPass1));

		if (lstrlen(szLogonPass) ==0 && lstrlen(szLogonPass1) ==0) return;
		//�����ж�
		if (lstrcmp(szLogonPass,szLogonPass1)!=0)
		{
			//��ʾ��Ϣ
			if(pLabelLogonPassInvalid1!=NULL)
			{
				pLabelLogonPassInvalid1->SetVisible(true);
				pLabelLogonPassInvalid1->SetText(TEXT("ȷ�����������벻һ��"));
			}

			return;
		}

		if(pLabelLogonPassValidate1!=NULL)	pLabelLogonPassValidate1->SetVisible(true);
	}
}

//��ȡ�ַ�
void CDlgRegister::GetControlString(CControlUI * pControlUI, TCHAR szString[], WORD wMaxCount)
{
	if(pControlUI==NULL) return;
	CString strString(pControlUI->GetText());
	//strString.TrimLeft();
	//strString.TrimRight();
	lstrcpyn(szString,strString,wMaxCount);
	return;
}


//��������
void CDlgRegister::OnEnChangeLogonPass()
{
	//��ȡ����
	TCHAR szPassword[LEN_PASSWORD]=TEXT("");

	//��ȡ����
	CControlUI * pEditLogonPass=GetControlByName(szEditLogonPassControlName);
	if(pEditLogonPass!=NULL) lstrcpyn(szPassword,pEditLogonPass->GetText(),CountArray(szPassword));

	//�ȼ��ж�
	CUserItemElement * pUserItemElement=CUserItemElement::GetInstance();
	BYTE cbPasswordLevel=pUserItemElement->GetPasswordLevel(szPassword);

	//���´���
	if (m_cbLogonPassLevel!=cbPasswordLevel)
	{
		//���ñ���
		m_cbLogonPassLevel=cbPasswordLevel;

		CPowerScaleUI * pPowerScaleUI = (CPowerScaleUI *)GetControlByName(szPowerScaleControlName);
		pPowerScaleUI->SetLevelState(m_cbLogonPassLevel);
	}

	return;
}

//������֤
void CDlgRegister::OnBnClickedChangeValidate()
{
	CVerifyCodeUI * pVerifyCodeUI = (CVerifyCodeUI *)GetControlByName(szVerifyCodeControlName);
	pVerifyCodeUI->Refresh();
	return;
}

//ͬ������
void CDlgRegister::OnBnClickedAgrrArticle()
{
	//��ȡ����
	CGlobalWebLink * pGlobalWebLink=CGlobalWebLink::GetInstance();

	//�����ַ
	TCHAR szNavigation[256]=TEXT("");
	_sntprintf(szNavigation,CountArray(szNavigation),TEXT("%s/Agreement.aspx"),pGlobalWebLink->GetPlatformLink());

	//��ҳ��
	ShellExecute(NULL,TEXT("OPEN"),szNavigation,NULL,NULL,SW_NORMAL);
}
//////////////////////////////////////////////////////////////////////////////////
