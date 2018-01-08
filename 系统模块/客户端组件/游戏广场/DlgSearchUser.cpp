#include "StdAfx.h"
#include "Resource.h"
#include "DlgSearchUser.h"
#include "PlatformEvent.h"
#include "ServerListData.h"

//////////////////////////////////////////////////////////////////////////////////

//�Զ�����
#define UD_USER_STATION				(UD_CUSTOM+0)						//�û�λ��

//ʱ�䶨��
#define IDI_SEARCH_TIME_OUT			100									//��ʱ��ʶ
#define TIME_SEARCH_TIME_OUT		15000L								//��ʱʱ��

//////////////////////////////////////////////////////////////////////////////////
//��̬����

CDlgSearchUser * CDlgSearchUser::m_pDlgSearchUser=NULL;					//����ָ��

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CDlgSearchUser, CSkinDialog)

	//ϵͳ��Ϣ
	ON_WM_TIMER()

END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////
//��ť�ؼ�
const TCHAR* const szButtonOKControlName = TEXT("ButtonOK");
const TCHAR* const szButtonEnterControlName = TEXT("ButtonEnter");

//�༭�ؼ�
const TCHAR* const szEditUINickNameControlName = TEXT("EditUINickName");
const TCHAR* const szEditUIGameIDControlName = TEXT("EditUIGameID");

//��ǩ�ؼ�
const TCHAR* const szLabelAccountsControlName = TEXT("LabelAccounts");
const TCHAR* const szLabelGameIDControlName = TEXT("LabelGameID");
const TCHAR* const szLabelPositionControlName = TEXT("LabelPosition");

//��չ��ť
const TCHAR* const szComboGameIDControlName = TEXT("ComboGameID");
const TCHAR* const szComboNickNameControlName = TEXT("ComboNickName");

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CDlgSearchUser::CDlgSearchUser() : CSkinDialog(IDD_DLG_POPUP)
{
	//��������
	m_MissionManager.InsertMissionItem(this);

	//���ñ���
	m_dwGameID=0L;
	ZeroMemory(m_szNickName,sizeof(m_szNickName));

	//��Ϣ����
	ZeroMemory(&m_UserRemoteInfo,sizeof(m_UserRemoteInfo));

	//���ñ���
	ASSERT(m_pDlgSearchUser==NULL);
	if (m_pDlgSearchUser==NULL) m_pDlgSearchUser=this;

	return;
}

//��������
CDlgSearchUser::~CDlgSearchUser()
{
	//���ñ���
	ASSERT(m_pDlgSearchUser==this);
	if (m_pDlgSearchUser==this) m_pDlgSearchUser=NULL;

	return;
}

//��Ϣ����
BOOL CDlgSearchUser::PreTranslateMessage(MSG * pMsg)
{
	//��������
	if ((pMsg->message==WM_KEYDOWN)&&(pMsg->wParam==VK_ESCAPE))
	{
		//��������
		CEditUI * pEditControl = NULL;

		//���ý���
		pEditControl = static_cast<CEditUI *>(GetControlByName(szEditUINickNameControlName));
		if(pEditControl && pEditControl->IsVisible())
		{
			pEditControl->SetFocus();
			pEditControl->SetText(TEXT(""));
		}

		//���ý���
		pEditControl = static_cast<CEditUI *>(GetControlByName(szEditUIGameIDControlName));
		if(pEditControl && pEditControl->IsVisible())
		{
			pEditControl->SetFocus();
			pEditControl->SetText(TEXT(""));
		}

		return TRUE;
	}

	return __super::PreTranslateMessage(pMsg);
}

//��������
BOOL CDlgSearchUser::OnInitDialog()
{
	__super::OnInitDialog();

	//���ñ���
	SetWindowText(TEXT("�û�����"));

	//���ô���
	SetWindowPos(NULL,0,0,441,245,SWP_NOZORDER|SWP_NOMOVE);	

	//���غ���
	LoadFriendUserInfo();

	return FALSE;
}

//ȷ����Ϣ
VOID CDlgSearchUser::OnOK()
{	
	//��������
	CControlUI * pNickNameControl=NULL;
	CControlUI * pGameIDControl=NULL;

	//���Ҷ���
	pNickNameControl=GetControlByName(szEditUINickNameControlName);
	pGameIDControl=GetControlByName(szEditUIGameIDControlName);
	if (pNickNameControl==NULL || pGameIDControl==NULL) return;

	//��������
	CString strSearchKey;
	bool bNickNameMode=true;

	//�ǳƲ���
	if (lstrlen(pNickNameControl->GetText())>0)
	{
		bNickNameMode=true;
		strSearchKey = pNickNameControl->GetText();
	}

	//I D����
	if (strSearchKey.IsEmpty()) bNickNameMode=false; 
	if (bNickNameMode==false) strSearchKey = pGameIDControl->GetText();

	//���봦��
	strSearchKey.TrimLeft();
	strSearchKey.TrimRight();
	strSearchKey.MakeLower();

	//Զ���ж�
	if (GetActiveStatus()==true)
	{
		//��ʾ��Ϣ
		CInformation Information(this);
		Information.ShowMessageBox(TEXT("���Ҳ������ڽ����У����Ժ��ٽ����µĲ����ύ��"),MB_ICONQUESTION,30L);

		return;
	}

	//Ч������
	if ((strSearchKey.IsEmpty()==TRUE)&&(bNickNameMode==true))
	{
		//��ʾ��Ϣ
		CInformation Information(this);
		Information.ShowMessageBox(TEXT("�����������ġ��û��ǳơ��������еĲ����ַ���"),MB_ICONSTOP);

		//���ý���
		pNickNameControl->SetFocus();

		return;
	}

	//Ч������
	if ((_tstol(strSearchKey)==0L)&&(bNickNameMode==false))
	{
		//��ʾ��Ϣ
		CInformation Information(this);
		Information.ShowMessageBox(TEXT("��������Ҫ���ҵġ���Ϸ I D�����룡"),MB_ICONSTOP);

		//���ý���
		pGameIDControl->SetFocus();

		return;
	}	

	//��������
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	//���ñ���
	if (bNickNameMode==true)
	{		
		m_dwGameID=0L;
		lstrcpyn(m_szNickName,strSearchKey,CountArray(m_szNickName));

		//�����Լ����˳�
		if (lstrcmp(pGlobalUserData->szNickName,m_szNickName)==0) return;
	}
	else
	{
		m_dwGameID=_tstol(strSearchKey);
		ZeroMemory(m_szNickName,sizeof(m_szNickName));

		//�����Լ����˳�
		if (pGlobalUserData->dwGameID==m_dwGameID) return;
	}

	//��������
	if (m_MissionManager.AvtiveMissionItem(this,false)==true) 
	{
		//����ʱ��
		SetTimer(IDI_SEARCH_TIME_OUT,TIME_SEARCH_TIME_OUT,NULL);
	}

	return;
}

//��ʼ�ؼ�
void CDlgSearchUser::InitControlUI()
{
	__super::InitControlUI();

	//��������
	CEditUI * pEditControl=NULL;

	//�û��ǳ�
	pEditControl = static_cast<CEditUI *>(GetControlByName(szEditUINickNameControlName));
	if(pEditControl!=NULL) pEditControl->SetMaxChar(LEN_NICKNAME-1);

	//��ϷID
	pEditControl = static_cast<CEditUI *>(GetControlByName(szEditUIGameIDControlName));
	if(pEditControl!=NULL) 
	{
		pEditControl->SetMaxChar(9);
		pEditControl->SetNumberOnly();
	}

	//���밴ť
	CButtonUI * pButtonControl = static_cast<CButtonUI *>(GetControlByName(szButtonEnterControlName));
	if(pButtonControl!=NULL) pButtonControl->SetVisible(false);
}

//��Ϣ����
void CDlgSearchUser::Notify(TNotifyUI &  msg)
{
	__super::Notify(msg);

	//��ȡ����
	CControlUI * pControlUI = msg.pSender;

	if (lstrcmp(msg.sType, TEXT("click")) == 0)
	{
		if(lstrcmp(pControlUI->GetName(), szButtonOKControlName) == 0) return OnOK();
		if(lstrcmp(pControlUI->GetName(), szButtonEnterControlName) == 0)
		{
			//��������
            tagTrackAccountsInfo TrackAccountsInfo;		
			TrackAccountsInfo.wKindID=m_UserRemoteInfo.wKindID;
			TrackAccountsInfo.wServerID=m_UserRemoteInfo.wServerID;
			TrackAccountsInfo.dwTargetID=m_UserRemoteInfo.dwUserID;

			//�����¼�
			CPlatformEvent * pPlatformEvent=CPlatformEvent::GetInstance();
			if (pPlatformEvent!=NULL) pPlatformEvent->SendPlatformEvent(EVENT_USER_TRACK_ACCOUNTS,(LPARAM)&TrackAccountsInfo);

			return EndDialog(IDOK);
		}
	}
	else if(lstrcmp(msg.sType,TEXT("itemselect")) ==0 )
	{
		if(lstrcmp(pControlUI->GetName(), szComboGameIDControlName)==0)
		{
			//���Ҷ���
			CControlUI * pEditGameID = GetControlByName(szEditUIGameIDControlName);
			if(pEditGameID!=NULL) pEditGameID->SetText(pControlUI->GetText());

			return;
		}
		else if(lstrcmp(pControlUI->GetName(), szComboNickNameControlName)==0)
		{
			//���Ҷ���
			CControlUI * pEditNickName = GetControlByName(szEditUINickNameControlName);
			if(pEditNickName!=NULL) pEditNickName->SetText(pControlUI->GetText());

			return;
		}
	}
}

//����ر�
VOID CDlgSearchUser::OnViewItemDelete(CPlazaViewServer * pServerViewItem)
{
	return;
}

//�û�ɾ��
VOID CDlgSearchUser::OnUserItemDelete(IClientUserItem * pIClientUserItem, CPlazaViewServer * pServerViewItem)
{
	return;
}

//�û�����
VOID CDlgSearchUser::OnUserItemUpdate(IClientUserItem * pIClientUserItem, CPlazaViewServer * pServerViewItem)
{
	return;
}

//�滭����
VOID CDlgSearchUser::OnDrawSystemTitle(CDC * pDC, INT nWdith, INT nHeight)
{
	//��Դ���
	HINSTANCE hInstance = GetModuleHandle(PLATFORM_RESOURCE_DLL_NAME);

	//���ر���
	CPngImage ImageLogo;
	ImageLogo.LoadImage(hInstance,TEXT("IMAGE_SEARCH_TITLE"));

	//���Ʊ���
	ImageLogo.DrawImage(pDC,10,10);
}

//�滭��Ϣ
VOID CDlgSearchUser::OnDrawClientAreaEnd(CDC * pDC, INT nWidth, INT nHeight)
{
	////���λ��
	//INT nTBorder=m_SkinAttribute.m_EncircleInfoFrame.nTBorder;
	//INT nBBorder=m_SkinAttribute.m_EncircleInfoFrame.nBBorder;
	//INT nLBorder=m_SkinAttribute.m_EncircleInfoFrame.nLBorder;
	//INT nRBorder=m_SkinAttribute.m_EncircleInfoFrame.nRBorder;

	////�滭ͼƬ
	//CPngImage ImageSearchLogo;
	//ImageSearchLogo.LoadImage(AfxGetInstanceHandle(),TEXT("SEARCH_LOGO"));
	//ImageSearchLogo.DrawImage(pDC,rcListControl.left+6,rcListControl.top-ImageSearchLogo.GetHeight()-5);

	return;
}

//�����¼�
bool CDlgSearchUser::OnEventMissionLink(INT nErrorCode)
{
	//���ӽ��
	if (nErrorCode!=0)
	{
		//��������
		if (m_MissionManager.AvtiveMissionItem(this,true)==true)
		{
			return true;
		}

		//ɾ��ʱ��
		KillTimer(IDI_SEARCH_TIME_OUT);

		//��ʾ��Ϣ
		CInformation Information(this);
		Information.ShowMessageBox(TEXT("���ӷ�������ʱ��Զ�̲��Ҳ���ʧ�ܣ�"),MB_OK|MB_ICONSTOP,30);

		return true;
	}

	//��������
	CMD_GP_C_SearchCorrespond SearchCorrespond;
	ZeroMemory(&SearchCorrespond,sizeof(SearchCorrespond));

	//��������
	SearchCorrespond.dwGameID=m_dwGameID;
	lstrcpyn(SearchCorrespond.szNickName,m_szNickName,CountArray(SearchCorrespond.szNickName));

	//��������
	m_MissionManager.SendData(MDM_GP_REMOTE_SERVICE,SUB_GP_C_SEARCH_CORRESPOND,&SearchCorrespond,sizeof(SearchCorrespond));

	return true;
}

//�ر��¼�
bool CDlgSearchUser::OnEventMissionShut(BYTE cbShutReason)
{
	//��ʾ��Ϣ
	if (cbShutReason!=SHUT_REASON_NORMAL)
	{
		//��������
		if (m_MissionManager.AvtiveMissionItem(this,true)==true)
		{
			return true;
		}

		//ɾ��ʱ��
		KillTimer(IDI_SEARCH_TIME_OUT);

		//��ʾ��Ϣ
		CInformation Information(this);
		Information.ShowMessageBox(TEXT("��������������쳣�Ͽ��ˣ�Զ�̲��Ҳ���ʧ�ܣ�"),MB_ICONERROR,30L);
	}
	else
	{
		//ɾ��ʱ��
		KillTimer(IDI_SEARCH_TIME_OUT);
	}
	
	return true;
}

//��ȡ�¼�
bool CDlgSearchUser::OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize)
{
	//�����
	if (Command.wMainCmdID==MDM_GP_REMOTE_SERVICE)
	{
		switch (Command.wSubCmdID)
		{
		case SUB_GP_S_SEARCH_CORRESPOND:	//���ҽ��
			{
				//��������
				CMD_GP_S_SearchCorrespond * pSearchCorrespond=(CMD_GP_S_SearchCorrespond *)pData;

				//Ч�����
				ASSERT(wDataSize<=sizeof(CMD_GP_S_SearchCorrespond));
				ASSERT(wDataSize>=(sizeof(CMD_GP_S_SearchCorrespond)-sizeof(pSearchCorrespond->UserRemoteInfo)));
				ASSERT(wDataSize==(sizeof(CMD_GP_S_SearchCorrespond)-sizeof(pSearchCorrespond->UserRemoteInfo)+pSearchCorrespond->wUserCount*sizeof(pSearchCorrespond->UserRemoteInfo[0])));

				//Ч�����
				if (wDataSize>sizeof(CMD_GP_S_SearchCorrespond)) return false;
				if (wDataSize<(sizeof(CMD_GP_S_SearchCorrespond)-sizeof(pSearchCorrespond->UserRemoteInfo))) return false;
				if (wDataSize!=(sizeof(CMD_GP_S_SearchCorrespond)-sizeof(pSearchCorrespond->UserRemoteInfo)+pSearchCorrespond->wUserCount*sizeof(pSearchCorrespond->UserRemoteInfo[0]))) return false;

				//ʧ�ܴ���
				KillTimer(IDI_SEARCH_TIME_OUT);

				//�ر�����
				m_MissionManager.ConcludeMissionItem(this,false);

				//��ȡ����
				CControlUI * pControlAccounts=GetControlByName(szLabelAccountsControlName);
				if(pControlAccounts==NULL) return true;

				CControlUI * pControlGameID=GetControlByName(szLabelGameIDControlName);
				if(pControlGameID==NULL) return true;

				CControlUI * pControlPosition=GetControlByName(szLabelPositionControlName);
				if(pControlPosition==NULL) return true;

				CControlUI * pControlBTEnter = GetControlByName(szButtonEnterControlName);
				if(pControlBTEnter==NULL) return true;

				//���ÿؼ�
				pControlAccounts->SetText(TEXT(""));
				pControlGameID->SetText(TEXT(""));
				pControlPosition->SetText(TEXT(""));
				pControlBTEnter->SetVisible(false);

				//�������
				ZeroMemory(&m_UserRemoteInfo,sizeof(m_UserRemoteInfo));

				//�����б�
				for (WORD i=0;i<pSearchCorrespond->wUserCount;i++)
				{
					//��������
					CServerListData * pServerListData=CServerListData::GetInstance();
					CGameKindItem * pGameKindItem=pServerListData->SearchGameKind(pSearchCorrespond->UserRemoteInfo[i].wKindID);

					//�����б�
					if (pGameKindItem!=NULL)
					{
						tagUserRemoteInfo * pUserRemoteInfo = &pSearchCorrespond->UserRemoteInfo[i];
						if(pUserRemoteInfo!=NULL)
						{
							//��������
							CopyMemory(&m_UserRemoteInfo,pUserRemoteInfo,sizeof(m_UserRemoteInfo));

							//��ϷID
							TCHAR szGameID[16]=TEXT("");
							_sntprintf(szGameID,CountArray(szGameID),TEXT("%d"),pUserRemoteInfo->dwGameID);

							//��Ϸ����
							TCHAR szGameServer[36]=TEXT("");
							_sntprintf(szGameServer,CountArray(szGameServer),TEXT("%s����"),pUserRemoteInfo->szGameServer);

							//���ÿؼ�
							pControlAccounts->SetText(pUserRemoteInfo->szNickName);
							pControlGameID->SetText(szGameID);
							pControlPosition->SetText(szGameServer);
							pControlBTEnter->SetVisible(true);

							break;
						}
					}
				}

				//����ж�
				if (pSearchCorrespond->wUserCount==0L)
				{
					CInformation Information(this);
					Information.ShowMessageBox(TEXT("������ѯ���û������ڻ���û�н��뷿�䣬����Ĳ�ѯ�������ԣ�"),MB_ICONQUESTION,30L);
				}

				return true;
			}
		}
	}

	return true;
}

//ʱ����Ϣ
VOID CDlgSearchUser::OnTimer(UINT_PTR nIDEvent)
{
	switch (nIDEvent)
	{
	case IDI_SEARCH_TIME_OUT:	//��ʱʱ��
		{
			//ɾ��ʱ��
			KillTimer(IDI_SEARCH_TIME_OUT);

			//�ر�����
			m_MissionManager.ConcludeMissionItem(this,false);

			//��ʾ��Ϣ
			CInformation Information(this);
			Information.ShowMessageBox(TEXT("��������ʱû����Ӧ�����Ҳ���ʧ�ܣ����Ժ����ԣ�"),MB_OK|MB_ICONSTOP,30L);

			return;
		}
	}

	__super::OnTimer(nIDEvent);
}

//
////Զ�̲���
//VOID CDlgSearchUser::OnBnClickedSearchRemote()
//{
//	//��ȡ״̬
//	UINT nCurrentCheck=m_SearchRemote.GetCheck();
//
//	//��Ա�ж�
//	if (nCurrentCheck==BST_UNCHECKED)
//	{
//		//��������
//		CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
//		tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();
//
//		//��Ա�ж�
//		if (pGlobalUserData->cbMemberOrder==0)
//		{
//			//��ʾ��Ϣ
//			CInformation Information(this);
//			INT_PTR nResult=Information.ShowMessageBox(TEXT("�����ǻ�Ա���޷�ʹ��Զ�̲��ҹ��ܣ���Ҫ�����ɹ���Ա��"),MB_ICONQUESTION|MB_YESNO);
//
//			//��Ա����
//			if (nResult==IDYES)
//			{
//				//���´���
//				UpdateWindow();
//
//				//�����ַ
//				TCHAR szLinkUrl[128]=TEXT("");
//				_sntprintf(szLinkUrl,CountArray(szLinkUrl),TEXT("%s/PayIndex.aspx"),szPlatformLink);
//
//				//���ӵ�ַ
//				ShellExecute(NULL,TEXT("OPEN"),szLinkUrl,NULL,NULL,SW_NORMAL);
//			}
//
//			return;
//		}
//	}
//
//	//���ý���
//	if (m_edGameID.IsWindowVisible()==TRUE) m_edGameID.SetFocus();
//	if (m_edNickName.IsWindowVisible()==TRUE) m_edNickName.SetFocus();
//
//	//���ÿؼ�
//	m_ComSearchArea.EnableWindow((nCurrentCheck==BST_CHECKED)?TRUE:FALSE);
//	m_SearchRemote.SetCheck((nCurrentCheck==BST_UNCHECKED)?BST_CHECKED:BST_UNCHECKED);
//
//	return;

//���غ���
VOID CDlgSearchUser::LoadFriendUserInfo()
{
	//���Ҷ���
	CComboUI * pComboGameID = static_cast<CComboUI *>(GetControlByName(szComboGameIDControlName));
	if(pComboGameID==NULL) return;

	//���Ҷ���
	CComboUI * pComboNickName = static_cast<CComboUI *>(GetControlByName(szComboNickNameControlName));
	if(pComboNickName==NULL) return;

	//��ȡ����
	ASSERT(CUserInformation::GetInstance());
	CUserInformation * pUserInformation = CUserInformation::GetInstance();

	//��������
	TCHAR szGameID[16]=TEXT("");
	CListLabelElementUI * pListLabelElement=NULL;			

    //ö�ٺ���
	WORD wIndex=0;
	tagCompanionInfo * pCompanionInfo=NULL;
	while(pCompanionInfo=pUserInformation->EmunCompanionInfo(wIndex++))
	{
		//�жϹ�ϵ
		if(pCompanionInfo->cbCompanion!=CP_FRIEND) continue;

		try
		{
			//����ǳ�
			pListLabelElement = new CListLabelElementUI;
			if(pListLabelElement==NULL) throw TEXT("�ڴ治�㣡");

			//��������
			pListLabelElement->SetAttribute(TEXT("text"),pCompanionInfo->szNickName);
			pListLabelElement->SetAttribute(TEXT("height"),TEXT("23"));
			pListLabelElement->SetManager(&m_PaintManager,pComboNickName,true);
			pComboNickName->Add(pListLabelElement);

			//��ʽ�ַ�
			_sntprintf(szGameID,CountArray(szGameID),TEXT("%d"),pCompanionInfo->dwGameID);

			//��ӱ�ʶ
			pListLabelElement = new CListLabelElementUI;
			if(pListLabelElement==NULL) throw TEXT("�ڴ治�㣡");

			//��������
			pListLabelElement->SetAttribute(TEXT("text"),szGameID);
			pListLabelElement->SetAttribute(TEXT("height"),TEXT("23"));
			pListLabelElement->SetManager(&m_PaintManager,pComboGameID,true);
			pComboGameID->Add(pListLabelElement);
		}
		catch(...)
		{
			ASSERT(FALSE);
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////
