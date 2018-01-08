#include "Stdafx.h"
#include "Resource.h"
#include "GamePlaza.h"
#include "MissionLogon.h"
#include "DlgLogon.h"
#include "DlgProxyConfig.h"
#include "DlgAddGame.h"
#include "DlgHelp.h"
#include "DLGMBCard.h"
#include "DlgInformation.h"
//////////////////////////////////////////////////////////////////////////////////
//��Ӧ�ؼ�
const TCHAR* const szButtonCloseControlName			 = TEXT("ButtonClose");
const TCHAR* const szButtonMinControlName			 = TEXT("ButtonMin");
const TCHAR* const szButtonLogonControlName			 = TEXT("ButtonLogon");
const TCHAR* const szButtonRegisterControlName		 = TEXT("ButtonRegister");
const TCHAR* const szButtonForgetPasswdControlName	 = TEXT("ButtonForgetPasswd");
const TCHAR* const szButtonWebLink1ControlName		 = TEXT("ButtonWebLink1");
const TCHAR* const szButtonWebLink2ControlName		 = TEXT("ButtonWebLink2");
const TCHAR* const szButtonWebLink3ControlName		 = TEXT("ButtonWebLink3");
const TCHAR* const szButtonWebLink4ControlName		 = TEXT("ButtonWebLink4");
const TCHAR* const szWebBrowserLogonAdControlName	 = TEXT("WebBrowserLogonAd");
const TCHAR* const szComboServerControlName			 = TEXT("ComboServer");
const TCHAR* const szComboAccountsControlName		 = TEXT("ComboAccounts");
const TCHAR* const szEditAccountsControlName		 = TEXT("EditAccounts");
const TCHAR* const szEditPasswordControlName		 = TEXT("EditPassword");
const TCHAR* const szLabelTitleControlName			 = TEXT("LabelTitle");
const TCHAR* const szLabelLogoControlName			 = TEXT("LabelLogo");
const TCHAR* const szCheckButtonPasswdControlName	 = TEXT("CheckButtonPasswd");
//////////////////////////////////////////////////////////////////////////////////

//���캯��
CDlgLogon::CDlgLogon() : CFGuiDialog(IDD_DLG_POPUP)
{
	//��¼��Ϣ
	m_dwGameID=0;
	m_szAccounts[0]=0;
	m_szPassword[0]=0;
	m_szLogonServer[0]=0;
	m_szPassPortID[0]=0;

	//���ñ���
	m_cbRemPassword=FALSE;
	m_cbLogonMode=LOGON_BY_ACCOUNTS;

	//�ؼ�����
	m_pDlgAccedit=NULL;

	return;
}

//��������
CDlgLogon::~CDlgLogon()
{
	DestroyWindow();

	//ɾ������
	for (INT_PTR i=0;i<m_AccountsInfoArray.GetCount();i++)
	{
		SafeDelete(m_AccountsInfoArray[i]);
	}

	if(m_pDlgAccedit) m_pDlgAccedit->EndDialog(IDOK);

	return;
}


//��������
BOOL CDlgLogon::OnInitDialog()
{
	__super::OnInitDialog();

	//���ô���
	ModifyStyle(0, WS_MINIMIZEBOX);

	//���ñ���
	SetWindowText(TEXT("�û���¼"));

	//����ͼ��
	SetIcon(LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME)),FALSE);

	////���д���
	//CenterWindow(this);

	//������Ϣ
	LoadAccountsInfo();
	LoadLogonServerInfo();
	return TRUE;
}

//��Ϣ����
BOOL CDlgLogon::PreTranslateMessage(MSG * pMsg)
{
	//��������
	if ((pMsg->message==WM_KEYDOWN)&&(pMsg->wParam==VK_ESCAPE))
	{
		return TRUE;
	}

	return __super::PreTranslateMessage(pMsg);
}

//ȷ������
VOID CDlgLogon::OnOK()
{
	//��ȡ��Ϣ
	if (GetInformation()==false) return;

	//���ش���
	ShowWindow(SW_HIDE);

	//ִ�е�¼
	CMissionLogon * pMissionLogon=CMissionLogon::GetInstance();
	if (pMissionLogon!=NULL) pMissionLogon->PerformLogonMission(m_cbRemPassword==TRUE);
	DestroyWindow();

	return;
}

//ȡ����Ϣ
VOID CDlgLogon::OnCancel()
{
	//�رմ���
	EndDialog(IDCANCEL);
	DestroyWindow();
	AfxGetMainWnd()->PostMessage(WM_CLOSE,0,0);
	return;
}

LPCTSTR CDlgLogon::GetSkinFile()
{
	return TEXT("DlgLogon.xml");
}

//��ʼ�ؼ�
void CDlgLogon::InitControlUI()
{
	//���Ҷ���
	CEditUI * pEditPassword = (CEditUI * )GetControlByName(szEditPasswordControlName);
	if(pEditPassword!=NULL)
	{
		pEditPassword->SetPasswordMode(true);
		pEditPassword->SetMaxChar(LEN_PASSWORD-1);
	}
	
	CEditComboUI * pComboServer= (CEditComboUI * )GetControlByName(szComboServerControlName);
	if(pComboServer!=NULL)
	{
		pComboServer->SetBkColor(0x00000000);
		pComboServer->SetItemBkColor(0x00000000);
		pComboServer->SetHotItemBkColor(0x000000);
		pComboServer->SetSelectedItemBkColor(0x000000);
	}

	CEditComboUI * pComboAccounts= (CEditComboUI * )GetControlByName(szComboAccountsControlName);
	if(pComboAccounts != NULL)
	{
		pComboAccounts->SetBkColor(0x00000000);
		pComboAccounts->SetItemBkColor(0x00000000);
		pComboAccounts->SetHotItemBkColor(0x000000);
		pComboAccounts->SetSelectedItemBkColor(0x000000);

	}

	//��ȡ����
	CGlobalWebLink * pGlobalWebLink=CGlobalWebLink::GetInstance();

	//�����ַ
	TCHAR szBillUrl[256]=TEXT("");
	_sntprintf(szBillUrl,CountArray(szBillUrl),TEXT("%s/Ads/LogonLogo.aspx"),pGlobalWebLink->GetPlatformLink());
	
	CWkeBrowserUI* pActiveXUI2 = static_cast<CWkeBrowserUI*>(GetControlByName(szWebBrowserLogonAdControlName));
	if( pActiveXUI2 ) 
	{
		pActiveXUI2->Navigate(szBillUrl);
	}
}

//��Ϣ����
void CDlgLogon::Notify(TNotifyUI &  msg)
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
		else if(lstrcmp(pControlUI->GetName(), szButtonMinControlName)==0)
		{
			ShowWindow(SW_MINIMIZE);
			return;
		}
		else if(lstrcmp(pControlUI->GetName(), szButtonLogonControlName)==0)
		{
			return OnOK();
		}
		else if(lstrcmp(pControlUI->GetName(), szButtonRegisterControlName)==0)
		{
			return OnBnClickedRegister();
		}
		//else if(lstrcmp(pControlUI->GetName(), szButtonCleanAccountsControlName)==0)
		//{
		//	return OnBnClickedDeleteRecord();
		//}
		else if(lstrcmp(pControlUI->GetName(), szButtonForgetPasswdControlName)==0)
		{
			return OnBnForgetPassword();
		}
		else if(lstrcmp(pControlUI->GetName(), szButtonWebLink1ControlName)==0 ||
				lstrcmp(pControlUI->GetName(), szButtonWebLink2ControlName)==0 ||
				lstrcmp(pControlUI->GetName(), szButtonWebLink3ControlName)==0 ||
				lstrcmp(pControlUI->GetName(), szButtonWebLink4ControlName)==0  )
		{
			//��ȡ����
			CGlobalWebLink * pGlobalWebLink=CGlobalWebLink::GetInstance();

			//�����ַ
			TCHAR szLogonLink[256]=TEXT("");
			_sntprintf(szLogonLink,CountArray(szLogonLink),TEXT("%s/LogonLink%d.aspx"),pGlobalWebLink->GetPlatformLink(),pControlUI->GetTag());

			//��ҳ��
			ShellExecute(NULL,TEXT("OPEN"),szLogonLink,NULL,NULL,SW_NORMAL);

			return;
		}
	}
	else if(lstrcmp(msg.sType,TEXT("itemselect")) ==0 )
	{
		if(lstrcmp(pControlUI->GetName(), szComboServerControlName)==0)
		{
			//���Ҷ���
			//CEditComboUI* pEditComboUI = static_cast<CEditComboUI*>(pControlUI);
			//pEditComboUI->SetText(pEditComboUI->GetItemAt(msg.wParam)->GetText());

			return;
		}
		else if(lstrcmp(pControlUI->GetName(), szComboAccountsControlName)==0)
		{
			return OnSelchangeAccounts();
		}
	}
	else if(lstrcmp(msg.sType,TEXT("textchanged")) ==0 )
	{
		if(lstrcmp(pControlUI->GetName(),szComboAccountsControlName)==0)
		{
			CEditComboUI * pComboAccounts=(CEditComboUI *)pControlUI;
			INT nCurrentSel=pComboAccounts->GetCurSel();
			CControlUI* pCnt= pComboAccounts->GetItemAt(nCurrentSel);
			if(pCnt)
			{
				tagAccountsInfo * pAccountsInfo=(tagAccountsInfo *)(pCnt->GetUserData());
				if(pAccountsInfo != NULL )
				{
					CEditUI * pEditPassword = (CEditUI * )GetControlByName(szEditPasswordControlName);
					CCheckButtonUI * pCheckButtonPasswd=(CCheckButtonUI *)GetControlByName(szCheckButtonPasswdControlName);

					if( pControlUI->GetText() != pAccountsInfo->szAccounts )
					{
						if(pCheckButtonPasswd!=NULL) pCheckButtonPasswd->SetCheck(false);
						if(pEditPassword) pEditPassword->SetText(TEXT(""));
					}
					else
					{
						if(pCheckButtonPasswd!=NULL) pCheckButtonPasswd->SetCheck(true);
						if(pEditPassword) pEditPassword->SetText(pAccountsInfo->szPassword);
					}
				}
			}


			return ;
		}
	}
	else if (lstrcmp(msg.sType,TEXT("itemdelete")) ==0 )
	{
		if(lstrcmp(pControlUI->GetName(),szComboAccountsControlName)==0)
		{
			int nIndex = (int)msg.wParam;

			OnBnClickedDeleteRecord(nIndex);
		}
	}

	return;
}

//��ʾ��֤
bool CDlgLogon::ShowAccreditWindow()
{
	//��������
	CDlgAccredit DlgAccredit;
	m_pDlgAccedit = &DlgAccredit;
	INT_PTR nEndCode = DlgAccredit.DoModal();
	m_pDlgAccedit=NULL;

	//��������
	if(nEndCode==IDOK)
	{		
		lstrcpyn(m_szPassPortID,DlgAccredit.GetPassPortNumber(),CountArray(m_szPassPortID));
	}

	return nEndCode==IDOK;
}

//��������
WORD CDlgLogon::ConstructLogonPacket(BYTE cbBuffer[], WORD wBufferSize, BYTE cbValidateFlags)
{
	//��������
	TCHAR szPassword[LEN_MD5];
	CWHEncrypt::MD5Encrypt(m_szPassword,szPassword);

	//��������
	switch (m_cbLogonMode)
	{
	case LOGON_BY_VISITOR:			//�ο͵�¼
		{
			//��������
			CMD_GP_LogonVisitor * pLogonVisitor=(CMD_GP_LogonVisitor *)cbBuffer;

			//ϵͳ��Ϣ
			pLogonVisitor->dwPlazaVersion=VERSION_PLAZA;
			pLogonVisitor->cbValidateFlags=cbValidateFlags;

			return sizeof(CMD_GP_LogonVisitor);
		}
	case LOGON_BY_GAME_ID:			//I D ��¼
		{
			//��������
			CMD_GP_LogonGameID * pLogonGameID=(CMD_GP_LogonGameID *)cbBuffer;

			//ϵͳ��Ϣ
			pLogonGameID->dwPlazaVersion=VERSION_PLAZA;

			//������ʶ
			CWHService::GetMachineIDEx(pLogonGameID->szMachineID);

			//��¼��Ϣ
			pLogonGameID->dwGameID=m_dwGameID;
			lstrcpyn(pLogonGameID->szPassword,szPassword,CountArray(pLogonGameID->szPassword));
			pLogonGameID->cbValidateFlags=cbValidateFlags;

			//��������
			CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
			tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();
			lstrcpyn(pGlobalUserData->szPassword,pLogonGameID->szPassword,CountArray(pGlobalUserData->szPassword));

			return sizeof(CMD_GP_LogonGameID);
		}
	case LOGON_BY_ACCOUNTS:			//�ʺŵ�¼
		{
			//��������
			CMD_GP_LogonAccounts * pLogonAccounts=(CMD_GP_LogonAccounts *)cbBuffer;

			//ϵͳ��Ϣ
			pLogonAccounts->dwPlazaVersion=VERSION_PLAZA;

			//������ʶ
			CWHService::GetMachineIDEx(pLogonAccounts->szMachineID);

			//��¼��Ϣ
			pLogonAccounts->cbValidateFlags=cbValidateFlags;
			lstrcpyn(pLogonAccounts->szPassword,szPassword,CountArray(pLogonAccounts->szPassword));
			lstrcpyn(pLogonAccounts->szAccounts,m_szAccounts,CountArray(pLogonAccounts->szAccounts));
			lstrcpyn(pLogonAccounts->szPassPortID,m_szPassPortID,CountArray(pLogonAccounts->szPassPortID));	

			//��������
			CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
			tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();
			lstrcpyn(pGlobalUserData->szPassword,pLogonAccounts->szPassword,CountArray(pGlobalUserData->szPassword));

			//�����С
			WORD wDataSize = sizeof(CMD_GP_LogonAccounts)-sizeof(pLogonAccounts->szPassPortID);
			wDataSize += CountStringBuffer(pLogonAccounts->szPassPortID);

			return wDataSize;
		}
	}

	return 0;
}

//�����ʺ�
void CDlgLogon::LoadAccountsInfo()
{
	//�����¼
	m_AccountsInfoArray.RemoveAll();

	//��ȡ��Ϣ
	CWHRegKey RegUserInfo;
	if (RegUserInfo.OpenRegKey(REG_USER_INFO, false)==false) return;

	//��������
	DWORD dwRegIndex=0L;
	DWORD dwLastUserID=RegUserInfo.GetValue(TEXT("LastUserID"),0L);

	//��ȡ��Ϣ ���MAX_ACCOUNTS��
	for(int i = 0; i < MAX_ACCOUNTS; i++)
	{
		//��������
		tagAccountsInfo AccountsInfo;
		ZeroMemory(&AccountsInfo,sizeof(AccountsInfo));

		//��ȡ����
		TCHAR szKeyItemName[16]=TEXT("");
		if (RegUserInfo.EnumItemString(dwRegIndex++,szKeyItemName,CountArray(szKeyItemName))==false) break;

		//���Ӽ�
		CWHRegKey RegUserItem;
		if (RegUserItem.Attach(RegUserInfo.OpenItemKey(szKeyItemName))==NULL) continue;

		//�ʻ���Ϣ
		AccountsInfo.dwUserID=_tstol(szKeyItemName);
		RegUserItem.GetString(TEXT("GameID"),AccountsInfo.szGameID,CountArray(AccountsInfo.szGameID));
		RegUserItem.GetString(TEXT("UserAccount"),AccountsInfo.szAccounts,CountArray(AccountsInfo.szAccounts));

		//��ȡ����
		TCHAR szPassword[MAX_ENCRYPT_LEN]=TEXT("");
		RegUserItem.GetString(TEXT("UserPassword"),szPassword,CountArray(szPassword));

		//�⿪����
		if (szPassword[0]!=0)
		{
			CWHEncrypt::XorCrevasse(szPassword,AccountsInfo.szPassword,CountArray(AccountsInfo.szPassword));
		}

		//��������
		tagAccountsInfo * pAccountsInfo=new tagAccountsInfo;
		CopyMemory(pAccountsInfo,&AccountsInfo,sizeof(AccountsInfo));

		//��������
		m_AccountsInfoArray.InsertAt(m_AccountsInfoArray.GetCount(),pAccountsInfo);
	}

	//���Ҷ���
	CEditComboUI * pComboAccounts= (CEditComboUI * )GetControlByName(szComboAccountsControlName);
	if(pComboAccounts==NULL) return;

	//������Ϣ
	for (INT_PTR i=0;i<m_AccountsInfoArray.GetCount();i++)
	{
		//������Ϣ
		INT nGameIDItem=CB_ERR;
		INT nAccountsItem=CB_ERR;
		tagAccountsInfo * pAccountsInfo=m_AccountsInfoArray[i];
		
		//������Ϣ
		if (pAccountsInfo->szAccounts[0]!=0)
		{
			try
			{
				CDialogBuilder builder2;
				//��������
				CListComboElementUI * pListLabelElement=NULL;

				pListLabelElement = static_cast<CListComboElementUI *>(builder2.Create( _T("DlgLogonAcountItem.xml"), (UINT)0, NULL, &m_PaintManager));

				if (pListLabelElement)
				{
					//��������
					pListLabelElement->SetText(pAccountsInfo->szAccounts);
					if(pComboAccounts->Add(pListLabelElement)==true) pListLabelElement->SetUserData(pAccountsInfo);

					//Ĭ���ʻ�
					if(dwLastUserID==pAccountsInfo->dwUserID) ((IListItemUI*)pListLabelElement)->Select(true);
				}


			}
			catch(...)
			{
				ASSERT(FALSE);
				break;
			}
		}
	}

	//����ѡ��
	if ((pComboAccounts->GetCount()>0)&&(pComboAccounts->GetCurSel()==LB_ERR))
	{
		pComboAccounts->SelectItem(0);
	}

	return;
}

//��ַ��Ϣ
void CDlgLogon::LoadLogonServerInfo()
{
	//��ȡ��Ŀ
	CGlobalServer GlobalServer;
	WORD wItemCount=GlobalServer.GetItemCount();

	//�ϴε�¼
	TCHAR szLastServer[LEN_SERVER]=TEXT("");
	GlobalServer.GetLastServerName(szLastServer);

	//���Ҷ���
	CEditComboUI * pComboServer= (CEditComboUI * )GetControlByName(szComboServerControlName);
	if(pComboServer==NULL) return;



	//��ȡ��Ϣ
	for (WORD i=0;i<wItemCount;i++)
	{
		//��ȡ��Ϣ
		TCHAR szServerItem[LEN_SERVER]=TEXT("");
		GlobalServer.GetServerItemName(i,szServerItem);

		//������Ϣ
		if (szServerItem[0]!=0)
		{
			try
			{
				CDialogBuilder builder2;
				//��������
				CListLabelElementUI * pListLabelElement=NULL;

				pListLabelElement = static_cast<CListLabelElementUI *>(builder2.Create( _T("DlgLogonServerItem.xml"), (UINT)0, NULL, &m_PaintManager));

				if (pListLabelElement)
				{
					//��������
					pListLabelElement->SetText(szServerItem);
					pComboServer->Add(pListLabelElement);
				}
			}
			catch(...)
			{
				ASSERT(FALSE);
				break;
			}

			//�����¼
			if ((szLastServer[0]!=0)&&(lstrcmp(szLastServer,szServerItem)==0))
			{
				szLastServer[0]=0;
				pComboServer->SelectItem(i);
			}
		}
	}

	//�����¼
	if (szLastServer[0]!=0)
	{
		try
		{
			CDialogBuilder builder2;
			//��������
			CListLabelElementUI * pListLabelElement=NULL;

			pListLabelElement = static_cast<CListLabelElementUI *>(builder2.Create( _T("DlgLogonServerItem.xml"), (UINT)0, NULL, &m_PaintManager));

			if (pListLabelElement)
			{
				//��������
				pListLabelElement->SetText(szLastServer);
				pComboServer->Add(pListLabelElement);
			}
			pComboServer->SelectItem(pListLabelElement->GetIndex());
		}
		catch(...)
		{
			ASSERT(FALSE);
		}
	}

	//����ѡ��
	if (pComboServer->GetCurSel()==LB_ERR)
	{
		if (pComboServer->GetCount()==0L)
		{
			try
			{
				CDialogBuilder builder2;
				//��������
				CListLabelElementUI * pListLabelElement=NULL;

				pListLabelElement = static_cast<CListLabelElementUI *>(builder2.Create( _T("DlgLogonServerItem.xml"), (UINT)0, NULL, &m_PaintManager));

				if (pListLabelElement)
				{
					//��������
					pListLabelElement->SetText(szLogonServer);
					pComboServer->Add(pListLabelElement);
				}
				pComboServer->SelectItem(pListLabelElement->GetIndex());
			}
			catch(...)
			{
				ASSERT(FALSE);
			}
		}
		else
		{
			pComboServer->SelectItem(0);
		}
	}

	return;
}

//��ȡ��Ϣ
bool CDlgLogon::GetInformation()
{
	//��ȡ��Ϣ
	m_dwGameID=0;
	m_szPassPortID[0]=0;

	//���Ҷ���
	CEditComboUI * pEditServer = (CEditComboUI * )GetControlByName(szComboServerControlName);
	if(pEditServer!=NULL) lstrcpyn(m_szLogonServer,pEditServer->GetText(),CountArray(m_szLogonServer));

	//���Ҷ���
	CEditComboUI * pEditAccounts = (CEditComboUI * )GetControlByName(szComboAccountsControlName);
	if(pEditAccounts!=NULL) lstrcpyn(m_szAccounts,pEditAccounts->GetText(),CountArray(m_szAccounts));

	//���Ҷ���
	CEditUI * pEditPassword = (CEditUI * )GetControlByName(szEditPasswordControlName);
	if(pEditPassword!=NULL) lstrcpyn(m_szPassword,pEditPassword->GetText(),CountArray(m_szPassword));

	//��ַ�ж�
	if (m_szLogonServer[0]==0L)
	{
		//��ʾ��Ϣ
		CDlgInformation Information(this);
		Information.ShowMessageBox(TEXT("��ѡ�񡰵�¼�����������ٵ�¼������������������Ϸָ�ϡ���ȡ������"),MB_ICONERROR,0);

		//��ʾ����
		ShowWindow(SW_SHOW);

		//���ý���
		if(pEditServer) pEditServer->SetFocus();

		return false;
	}

	//�ʺ��ж�
	if ((m_cbLogonMode==LOGON_BY_ACCOUNTS)&&(m_szAccounts[0]==0L))
	{
		//��ʾ��Ϣ
		CDlgInformation Information(this);
		Information.ShowMessageBox(TEXT("���������ʺź��ٵ�¼��"),MB_ICONERROR,0);

		//��ʾ����
		ShowWindow(SW_SHOW);

		//���ý���
		pEditAccounts->SetFocus();

		return false;
	}

	//�����ж�
	if (m_szPassword[0]==0)
	{
		//��ʾ��Ϣ
		CDlgInformation Information(this);
		Information.ShowMessageBox(TEXT("��������������ٵ�¼��"),MB_ICONERROR,0);

		//��ʾ����
		ShowWindow(SW_SHOW);

		//���Ҷ���
		CEditUI * pEditPassword = (CEditUI * )GetControlByName(szEditPasswordControlName);
		if(pEditPassword!=NULL) pEditPassword->SetFocus();

		return false;
	}

	//��ס����
	CCheckButtonUI * pCheckButtonPasswd=(CCheckButtonUI *)GetControlByName(szCheckButtonPasswdControlName);;
	if(pCheckButtonPasswd!=NULL)
	{
		m_cbRemPassword=pCheckButtonPasswd->GetCheck();
	}

	return true;
}

//ѡ��ı�
void CDlgLogon::OnSelchangeAccounts()
{
	//��������
	CEditComboUI * pComboAccounts=(CEditComboUI *)GetControlByName(szComboAccountsControlName);
	if(pComboAccounts==NULL) return;

	//���Ҷ���
	//CEditUI * pEditAccounts = (CEditUI * )GetControlByName(szEditAccountsControlName);
	//if(pEditAccounts!=NULL) pEditAccounts->SetText(pComboAccounts->GetText());	

	//���Ҷ���
	CEditUI * pEditPassword = (CEditUI * )GetControlByName(szEditPasswordControlName);
	if(pEditPassword==NULL) return;

	//���Ҷ���
	CCheckButtonUI * pCheckButtonPasswd=(CCheckButtonUI *)GetControlByName(szCheckButtonPasswdControlName);;
	if(pCheckButtonPasswd==NULL) return;

	//��ȡѡ��
	INT nCurrentSel=pComboAccounts->GetCurSel();
	if(nCurrentSel==LB_ERR) return;
	tagAccountsInfo * pAccountsInfo=(tagAccountsInfo *)pComboAccounts->GetItemAt(nCurrentSel)->GetUserData();	

	//���ÿؼ�
	if ((nCurrentSel!=LB_ERR)&&(pAccountsInfo!=NULL))
	{
		//��������
		pEditPassword->SetText(pAccountsInfo->szPassword);

		//��ס����
		bool bRemPassword=(pAccountsInfo->szPassword[0]!=0);
		pCheckButtonPasswd->SetCheck(bRemPassword);
	}
	else
	{
		//��������
		pEditPassword->SetText(TEXT(""));

		//��ס����
		pCheckButtonPasswd->SetCheck(false);
	}

	return;
}

//�༭�ı�
void CDlgLogon::OnCbnEditchangeAccounts()
{
	//��ȫ����
	CCheckButtonUI * pCheckButtonPasswd=(CCheckButtonUI *)GetControlByName(szCheckButtonPasswdControlName);;
	if(pCheckButtonPasswd!=NULL) pCheckButtonPasswd->SetCheck(false);

	//��������
	CEditUI * pEditPassword = (CEditUI * )GetControlByName(szEditPasswordControlName);
	if(pEditPassword) pEditPassword->SetText(TEXT(""));

	return;
}


//��������
void CDlgLogon::OnBnForgetPassword()
{
	//��ȡ����
	CGlobalWebLink * pGlobalWebLink=CGlobalWebLink::GetInstance();

	//�����ַ
	TCHAR szLogonLink[256]=TEXT("");
	_sntprintf(szLogonLink,CountArray(szLogonLink),TEXT("%s/LogonLink5.aspx"),pGlobalWebLink->GetPlatformLink());

	//��ҳ��
	ShellExecute(NULL,TEXT("OPEN"),szLogonLink,NULL,NULL,SW_NORMAL);
}

//ɾ����¼
void CDlgLogon::OnBnClickedDeleteRecord(int index)
{
	//���Ҷ���
	CEditComboUI * pComboAccounts= (CEditComboUI * )GetControlByName(szComboAccountsControlName);
	if(pComboAccounts==NULL) return;


	//���Ҷ���
	CEditUI * pEditPassword = (CEditUI * )GetControlByName(szEditPasswordControlName);
	if(pEditPassword==NULL) return;	

	//��ȡ��Ϣ
	tagAccountsInfo * pAccountsInfo=NULL;
	INT nCurrentSel=index;

	//ɾ������
	if (nCurrentSel!=LB_ERR)
	{
		//��ȡ����
		ASSERT(pComboAccounts->GetItemAt(nCurrentSel)->GetUserData()!=NULL);
		pAccountsInfo=(tagAccountsInfo *)pComboAccounts->GetItemAt(nCurrentSel)->GetUserData();

		//�����ʶ
		CString strUserID;
		strUserID.Format(TEXT("%ld"),pAccountsInfo->dwUserID);

		//�û�����
		CWHRegKey RegUserInfo;
		RegUserInfo.OpenRegKey(REG_USER_INFO,false);

		//ɾ������
		RegUserInfo.RecurseDeleteKey(strUserID);

		//ɾ���б�
		pComboAccounts->RemoveAt(nCurrentSel);				

		//����ѡ��
		if ((pComboAccounts->GetCurSel()==LB_ERR)&&(pComboAccounts->GetCount()>0))
		{
			pComboAccounts->SelectItem(0);
		}
	}

	//���ý���
	pComboAccounts->SetFocus();

	//��ȡ����
	nCurrentSel=pComboAccounts->GetCurSel();
	pAccountsInfo=(nCurrentSel!=LB_ERR)?pAccountsInfo=(tagAccountsInfo *)pComboAccounts->GetItemAt(nCurrentSel)->GetUserData():NULL;

	//��������
	pEditPassword->SetText((pAccountsInfo!=NULL)?pAccountsInfo->szPassword:TEXT(""));

	//���Ҷ���
	CCheckButtonUI * pCheckButtonPasswd=(CCheckButtonUI *)GetControlByName(szCheckButtonPasswdControlName);;
	if(pCheckButtonPasswd==NULL) return;

	//��ס����
	bool bRemPassword=(pAccountsInfo!=NULL)?(pAccountsInfo->szPassword[0]!=0):false;
	pCheckButtonPasswd->SetCheck(bRemPassword);

	return;
}

//ע���ʺ�
void CDlgLogon::OnBnClickedRegister()
{
	//���ش���
	ShowWindow(SW_HIDE);

	//��ȡ��ַ
	CString strLogonServer;
	GetDlgItemText(IDC_LOGON_SERVER,strLogonServer);

	//ע���ʺ�
	CControlUI * pControl = GetControlByName(szComboServerControlName);
	if (pControl != NULL) strLogonServer=pControl->GetText();

	//�����ַ
	strLogonServer.TrimLeft();
	strLogonServer.TrimRight();
	lstrcpyn(m_szLogonServer,(strLogonServer.IsEmpty()==true)?szLogonServer:strLogonServer,CountArray(m_szLogonServer));

	//��ʾע��
	ASSERT(CMissionLogon::GetInstance()!=NULL);
	if (CMissionLogon::GetInstance()!=NULL) CMissionLogon::GetInstance()->ShowRegister();

	return;
}



//////////////////////////////////////////////////////////////////////////////////