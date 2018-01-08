#include "Stdafx.h"
#include "Resource.h"
#include "DlgCustomFace.h"
#include "DlgInformation.h"

//////////////////////////////////////////////////////////////////////////////////

//�༭ģʽ
#define MM_SYSTEM					0									//ϵͳģʽ
#define MM_CUSTOM					1									//�Զ�ģʽ

//���Ͷ���
#define FACE_SELECT_ITEM			0									//ѡ��ͷ��
#define FACE_CUSTOM_EDIT			1									//�Զ�ͷ��
#define FACE_CUSTOM_VIDEO			2									//����ͷ��

//�ؼ���ʶ
#define IDC_FACE_CUSTOM_WND			100									//�Զ�ͷ��
#define IDC_FACE_SELECT_WND			101									//ѡ��ͷ��

//////////////////////////////////////////////////////////////////////////////////
//�����ؼ�
const TCHAR* const szAvatarPreviewControlName = TEXT("AvatarPreview");

//��ť
const TCHAR* const szButtonOkControlName = TEXT("ButtonOk");
const TCHAR* const szButtonCancelControlName = TEXT("ButtonCancel");
const TCHAR* const szButtonCloseControlName = TEXT("ButtonClose");
const TCHAR* const szButtonLocalControlName = TEXT("ButtonLocal");
const TCHAR* const szButtonSelfieControlName = TEXT("ButtonSelfie");

//����
const TCHAR* const szTileLayoutSymAvatarControlName = TEXT("TileLayoutSymAvatar");
const TCHAR* const szTabLayoutAvatarControlName = TEXT("TabLayoutAvatar");


BEGIN_MESSAGE_MAP(CDlgCustomFace, CFGuiDialog)
	//ON_NOTIFY(TCN_SELCHANGE,IDC_TAB_FACE,OnTcnSelAlterFaceMode)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////////

//���캯��
CDlgCustomFace::CDlgCustomFace() : CFGuiDialog(IDD_DLG_COSTOM_FACE)
,m_pTileLayoutSysAvatar(NULL)
,m_pAvatarPreview(NULL)
{
	//���ñ���
	m_pICustomFaceEvent=NULL;

	//���ܰ�ť
	m_pbtOk=NULL;
	m_pbtCancel=NULL;

	//���ñ���
	m_cbMode=0;
	m_wFaceID=INVALID_WORD;
	ZeroMemory(&m_CustomFaceInfo,sizeof(m_CustomFaceInfo));
	ZeroMemory(&m_CustomFaceInfo96,sizeof(m_CustomFaceInfo96));

	//�������
	m_MissionManager.InsertMissionItem(this);

	//���ýӿ�
	m_FaceItemCustomWnd.SetImageEditorEvent(this);
	//m_FaceItemSelectWnd.SetFaceItemSelectEvent(this);

	return;
}

//��������
CDlgCustomFace::~CDlgCustomFace()
{
}

//ͼ���¼�
VOID CDlgCustomFace::OnEventFaceSelect(WORD wFaceID)
{
	//���ñ���
	m_wFaceID=wFaceID;

	//����ģʽ
	m_cbMode=MM_SYSTEM;

	//���ÿؼ�
	//m_FaceItemCustomWnd.DestoryImage();

	//���½���
	//RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_ERASENOW);

	return;
}

//ͼ���¼�
VOID CDlgCustomFace::OnEventImageSelect(bool bLoadImage, bool bOperateing)
{
	//���½���
	RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_ERASENOW);

	return;
}

//�ؼ���
VOID CDlgCustomFace::DoDataExchange(CDataExchange * pDX)
{
	__super::DoDataExchange(pDX);
}

//��������
BOOL CDlgCustomFace::OnInitDialog()
{
	__super::OnInitDialog();

	//��ȡ��С
	CRect rcClient;
	GetClientRect(&rcClient);

	//���ñ���
	SetWindowText(TEXT("����ͷ��"));

	//��������
	CRect rcCreate(0,0,0,0);
	m_FaceItemCustomWnd.Create(NULL,NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_FACE_CUSTOM_WND);

	CTabLayoutUI* pTabLayoutAvatar = (CTabLayoutUI*)GetControlByName(szTabLayoutAvatarControlName);
	if(pTabLayoutAvatar != NULL){
		CRect rcCtrl = pTabLayoutAvatar->GetPos();
		//����λ��
		m_FaceItemCustomWnd.SetWindowPos(NULL,rcCtrl.left,rcCtrl.top,rcCtrl.Width(),rcCtrl.Height(),SWP_NOZORDER);
		m_FaceItemCustomWnd.ShowWindow(SW_HIDE);
	}

	return TRUE;
}

//��ʼ�ؼ�
void CDlgCustomFace::InitControlUI()
{
	__super::InitControlUI();

	//��������
	CRect rcClient;
	GetClientRect(&rcClient);

	//��������
	CLabelUI * pLabelControl=NULL;

	//��ȡ����
	CContainerUI * pParent = static_cast<CContainerUI *>(m_PaintManager.GetRoot());

	//ȷ����ť
	m_pbtOk = (CButtonUI*)GetControlByName(szButtonOkControlName);

	//ȡ����ť
	m_pbtCancel = (CButtonUI*)GetControlByName(szButtonCancelControlName);

	m_pTileLayoutSysAvatar = (CTileLayoutUI*)GetControlByName(szTileLayoutSymAvatarControlName);
	m_pAvatarPreview = (CImageUI*)GetControlByName(szAvatarPreviewControlName);

	//��������
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	//���ñ���
	m_wFaceID=pGlobalUserData->wFaceID;
	CopyMemory(&m_CustomFaceInfo,&pGlobalUserData->CustomFaceInfo,sizeof(m_CustomFaceInfo));

	LoadSysAvatar();

	//����ͷ��
	if(pGlobalUserData->CustomFaceInfo.dwDataSize != 0)
	{
		CCustomFaceManager * pCustomFaceManager=CCustomFaceManager::GetInstance();
		pCustomFaceManager->LoadUserCustomFace(pGlobalUserData->dwUserID,pGlobalUserData->dwCustomID,m_CustomFaceInfo96);
	}

	if(pGlobalUserData->CustomFaceInfo.dwDataSize != NULL)
	{
		UpdateAvatarPreview(m_CustomFaceInfo96.dwCustomFace, BIGFACE_CX*BIGFACE_CY);
	}
	else
	{
		UpdateAvatarPreview(pGlobalUserData->wFaceID);
	}
}

//��Ϣ����
void CDlgCustomFace::Notify(TNotifyUI &  msg)
{
	__super::Notify(msg);

	//��ȡ����
	CControlUI * pControlUI = msg.pSender;

	if (lstrcmp(msg.sType, TEXT("click")) == 0)
	{
		//ȷ��
		if (pControlUI==m_pbtOk)
		{
			return OnOK();
		}
		//ȡ��
		else if (pControlUI==m_pbtCancel)
		{
			return OnCancel();
		}
		//�ر�
		else if( pControlUI->GetName() == szButtonCloseControlName)
		{
			return OnCancel();
		}
	}
	//ѡ��ͷ��
	else if( msg.sType == TEXT("selectchanged") )
	{
		if(pControlUI->GetParent() != NULL && m_pTileLayoutSysAvatar != NULL)
		{	
			int iIndex = m_pTileLayoutSysAvatar->GetItemIndex(pControlUI->GetParent());
			if(iIndex != -1)
			{
				m_wFaceID = iIndex ;
				UpdateAvatarPreview(m_wFaceID);
			}
		}
	}
	//ģʽ�л�
	else if( msg.sType == TEXT("tabselect") )
	{
		if( pControlUI->GetName() == szTabLayoutAvatarControlName )
		{
			m_cbMode = (BYTE)msg.wParam;
			m_FaceItemCustomWnd.ShowWindow( (m_cbMode==MM_CUSTOM)?SW_SHOW:SW_HIDE );
			if(m_cbMode == MM_CUSTOM)
			{
				//���ý���
				m_FaceItemCustomWnd.SetFocus();
				m_FaceItemCustomWnd.SetActiveWindow();
			}
		}
	}
}

//�滭����
VOID CDlgCustomFace::OnDrawSystemTitle(CDC * pDC, INT nWdith, INT nHeight)
{
	//��Դ���
	HINSTANCE hInstance = GetModuleHandle(PLATFORM_RESOURCE_DLL_NAME);

	//���ر���
	CPngImage ImageTitle;
	ImageTitle.LoadImage(hInstance,TEXT("CHANGE_FACE_TITLE"));

	//���Ʊ���
	ImageTitle.DrawImage(pDC,10,10);
}

//�滭����
void CDlgCustomFace::OnEndPaintWindow(HDC hDC)
{
	if( m_cbMode == MM_CUSTOM && ::IsWindow(m_FaceItemCustomWnd.GetSafeHwnd()) && !m_FaceItemCustomWnd.IsNull() )
	{
		CRect rcCtrl(0,0,BIGFACE_CX,BIGFACE_CY);
		if(m_pAvatarPreview != NULL)
		{
			rcCtrl = m_pAvatarPreview->GetPos();
		}
		CDC* pDC = CDC::FromHandle(hDC);
		m_FaceItemCustomWnd.DrawEditImage(pDC,rcCtrl.left,rcCtrl.top,rcCtrl.Width(),rcCtrl.Height());
	}
	return;
}

//�����¼�
bool CDlgCustomFace::OnEventMissionLink(INT nErrorCode)
{
	//�����ж�
	if (nErrorCode==0L)
	{
		//��������
		CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
		tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

		//������Ϣ 
		switch (m_cbMode)
		{
		case MM_SYSTEM:		//ϵͳģʽ
			{
				//��������
				CMD_GP_SystemFaceInfo SystemFaceInfo;
				ZeroMemory(&SystemFaceInfo,sizeof(SystemFaceInfo));

				//���ñ���
				SystemFaceInfo.wFaceID=m_wFaceID;
				SystemFaceInfo.dwUserID=pGlobalUserData->dwUserID;
				CWHService::GetMachineIDEx(SystemFaceInfo.szMachineID);
				CopyMemory(SystemFaceInfo.szPassword,pGlobalUserData->szPassword,sizeof(SystemFaceInfo.szPassword));

				//��������
				CMissionManager * pMissionManager=GetMissionManager();
				pMissionManager->SendData(MDM_GP_USER_SERVICE,SUB_GP_SYSTEM_FACE_INFO,&SystemFaceInfo,sizeof(SystemFaceInfo));

				break;
			}
		case MM_CUSTOM:		//�Զ�ģʽ
			{
				//��������
				CMD_GP_CustomFaceInfo CustomFaceInfo;
				ZeroMemory(&CustomFaceInfo,sizeof(CustomFaceInfo));

				//���ñ���
				CustomFaceInfo.dwUserID=pGlobalUserData->dwUserID;
				CWHService::GetMachineIDEx(CustomFaceInfo.szMachineID);
				CopyMemory(CustomFaceInfo.szPassword,pGlobalUserData->szPassword,sizeof(CustomFaceInfo.szPassword));
				CopyMemory(CustomFaceInfo.dwCustomFace,m_CustomFaceInfo.dwCustomFace,sizeof(CustomFaceInfo.dwCustomFace));

				//��������
				CMissionManager * pMissionManager=GetMissionManager();
				pMissionManager->SendData(MDM_GP_USER_SERVICE,SUB_GP_CUSTOM_FACE_INFO,&CustomFaceInfo,sizeof(CustomFaceInfo));

				break;
			}
		}

		return true;
	}
	else
	{
		//��ʾ�ؼ�
		EnableControl(true);

		//��ʾ��ʾ
		CDlgInformation Information(this);
		Information.ShowMessageBox(TEXT("�޷����ӷ�������ͷ���޸�ʧ�ܣ����������Ƿ�ͨ��"),MB_ICONERROR);
	}

	return true;
}

//�ر��¼�
bool CDlgCustomFace::OnEventMissionShut(BYTE cbShutReason)
{
	//�رմ���
	if (cbShutReason!=SHUT_REASON_NORMAL)
	{
		//��������
		CMissionManager * pMissionManager=GetMissionManager();
		if (pMissionManager->AvtiveMissionItem(this,true)==true) return true;

		//��ʾ�ؼ�
		EnableControl(true);

		//��ʾ��ʾ
		CDlgInformation Information(this);
		Information.ShowMessageBox(TEXT("���ڵ�ǰ����������æ���ϴ��Զ���ͷ��ʧ�ܣ����Ժ������ԣ�"),MB_ICONERROR);
	}

	return true;
}

//��ȡ�¼�
bool CDlgCustomFace::OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize)
{
	//�����ɹ�
	if ((Command.wMainCmdID==MDM_GP_USER_SERVICE)&&(Command.wSubCmdID==SUB_GP_USER_FACE_INFO))
	{
		//Ч�����
		ASSERT(wDataSize==sizeof(CMD_GP_UserFaceInfo));
		if (wDataSize!=sizeof(CMD_GP_UserFaceInfo)) return false;

		//��Ϣ����
		CMD_GP_UserFaceInfo * pUserFaceInfo=(CMD_GP_UserFaceInfo *)pData;

		//��������
		CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
		tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

		//��������
		if (pUserFaceInfo->dwCustomID==0)
		{
			//�¼�֪ͨ
			ASSERT(m_pICustomFaceEvent!=NULL);
			if (m_pICustomFaceEvent!=NULL) m_pICustomFaceEvent->OnEventSystemFace(pGlobalUserData->dwUserID,pUserFaceInfo->wFaceID);
		}
		else
		{
			//��������
			CCustomFaceManager * pCustomFaceManager=CCustomFaceManager::GetInstance();
			pCustomFaceManager->SaveUserCustomFace(pGlobalUserData->dwUserID,pUserFaceInfo->dwCustomID,m_CustomFaceInfo.dwCustomFace);
			pCustomFaceManager->SaveUserCustomFace(pGlobalUserData->dwUserID,pUserFaceInfo->dwCustomID,m_CustomFaceInfo96.dwCustomFace, BIGFACE_CX*BIGFACE_CY);

			//�¼�֪ͨ
			ASSERT(m_pICustomFaceEvent!=NULL);
			if (m_pICustomFaceEvent!=NULL) m_pICustomFaceEvent->OnEventCustomFace(pGlobalUserData->dwUserID,pUserFaceInfo->dwCustomID,m_CustomFaceInfo);
		}

		//�رմ���
		EndDialog(IDOK);

		return true;
	}

	return true;
}

//ȷ������
VOID CDlgCustomFace::OnOK()
{
	//�����ж�
	bool bConnect=false;

	//�Զ�ģʽ
	if (m_cbMode==MM_CUSTOM && m_FaceItemCustomWnd.m_bLoadCustom)
	{		
		//��������
		CImage ImageCustomFace;
		ImageCustomFace.Create(FACE_CX,FACE_CY,32);

		//���� DC
		CImageDC BufferDC(ImageCustomFace);
		m_FaceItemCustomWnd.DrawEditImage(CDC::FromHandle(BufferDC),0,0,FACE_CX,FACE_CY);

		//��ȡ����
		INT nImagePitch=ImageCustomFace.GetPitch();
		LPBYTE cbBitCustomFace=(LPBYTE)ImageCustomFace.GetBits();

		//��������
		for (INT nYImagePos=0;nYImagePos<FACE_CY;nYImagePos++)
		{
			for (INT nXImagePos=0;nXImagePos<FACE_CX;nXImagePos++)
			{
				//������ɫ
				DWORD dwImageTarget=nYImagePos*nImagePitch+nXImagePos*4L;
				m_CustomFaceInfo.dwCustomFace[nYImagePos*FACE_CX+nXImagePos]=*(COLORREF *)(cbBitCustomFace+dwImageTarget);
			}
		}

		//���ñ���
		bConnect=true;
		m_CustomFaceInfo.dwDataSize=sizeof(m_CustomFaceInfo);


		//96*96
		//��������
		CImage ImageCustomFace96;
		ImageCustomFace96.Create(BIGFACE_CX,BIGFACE_CY,32);
		//���� DC
		CImageDC BufferDC2(ImageCustomFace96);
		m_FaceItemCustomWnd.DrawEditImage(CDC::FromHandle(BufferDC2),0,0,BIGFACE_CX,BIGFACE_CY);

		//��ȡ����
		INT nImagePitch2=ImageCustomFace96.GetPitch();
		LPBYTE cbBitCustomFace2=(LPBYTE)ImageCustomFace96.GetBits();

		//��������
		for (INT nYImagePos=0;nYImagePos<BIGFACE_CY;nYImagePos++)
		{
			for (INT nXImagePos=0;nXImagePos<BIGFACE_CX;nXImagePos++)
			{
				//������ɫ
				DWORD dwImageTarget=nYImagePos*nImagePitch2+nXImagePos*4L;
				m_CustomFaceInfo96.dwCustomFace[nYImagePos*BIGFACE_CX+nXImagePos]=*(COLORREF *)(cbBitCustomFace2+dwImageTarget);
			}
		}

		m_CustomFaceInfo96.dwDataSize=sizeof(m_CustomFaceInfo96);

	}

	//ϵͳģʽ
	if (m_cbMode==MM_SYSTEM)
	{
		CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
		bConnect=(pGlobalUserInfo->GetGlobalUserData()->wFaceID!=m_wFaceID);
	}

	//�Ƿ����
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	if (!m_FaceItemCustomWnd.m_bLoadCustom && pGlobalUserInfo->GetGlobalUserData()->wFaceID!=m_wFaceID)
	{
		m_cbMode=MM_SYSTEM;
		bConnect=(pGlobalUserInfo->GetGlobalUserData()->wFaceID!=m_wFaceID);
	}

	//��������
	if (bConnect==true)
	{
		//�ؼ�����
		EnableControl(false);

		//��������
		m_MissionManager.AvtiveMissionItem(this,false);

		return;
	}

	__super::OnOK();
}

//���ýӿ�
bool CDlgCustomFace::SetCustomFaceEvent(IUnknownEx * pIUnknownEx)
{
	//���ýӿ�
	if (pIUnknownEx!=NULL)
	{
		//��ѯ�ӿ�
		ASSERT(QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,ICustomFaceEvent)!=NULL);
		m_pICustomFaceEvent=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,ICustomFaceEvent);

		//�ɹ��ж�
		if (m_pICustomFaceEvent==NULL) return false;
	}
	else m_pICustomFaceEvent=NULL;

	return true;
}

//�ؼ�����
VOID CDlgCustomFace::EnableControl(bool bEnableControl)
{
	//���ÿؼ�
	m_pbtOk->SetEnabled((bEnableControl==true)?TRUE:FALSE);
	//m_TabControl.EnableWindow((bEnableControl==true)?TRUE:FALSE);
	//m_FaceItemCustomWnd.EnableWindow((bEnableControl==true)?TRUE:FALSE);
	//m_FaceItemSelectWnd.EnableWindow((bEnableControl==true)?TRUE:FALSE);

	return;
}


bool CDlgCustomFace::UpdateAvatarPreview(const WORD wFaceID)
{
	if( wFaceID == INVALID_WORD ) return false;
	if( m_pAvatarPreview == NULL ) return false;
	TCHAR szBuffer[64]={0};
	//wsprintf(szBuffer, TEXT("AVATAR_96_96\\Avatar%d.png"), wFaceID);
	wsprintf(szBuffer, TEXT("file=\'AVATAR_96_96\\Avatar%d.png\' restype=\'FILE\'"), wFaceID);
	m_pAvatarPreview->SetImage(szBuffer);
	return true;
}

bool CDlgCustomFace::UpdateAvatarPreview(DWORD* dwCustomFace, DWORD dwSize)
{
	if( m_pAvatarPreview == NULL ) return false;
	m_pAvatarPreview->SetCustomImage(dwCustomFace, dwSize);
	return true;
}

bool CDlgCustomFace::LoadSysAvatar()
{
	if(m_pTileLayoutSysAvatar == NULL) return false;

	TCHAR szBuffer[64]={0};
	for(int i = 0; i < m_pTileLayoutSysAvatar->GetCount(); i++)
	{
		CContainerUI* pItemAvatar = static_cast<CContainerUI*>(m_pTileLayoutSysAvatar->GetItemAt(i));
		CControlUI* pAvatar = pItemAvatar->GetItemAt(0);
		WORD wFaceID = i;
		wsprintf(szBuffer, TEXT("file=\'AVATAR_48_48\\Avatar%d.png\' restype=\'FILE\'"), wFaceID);
		pAvatar->SetBkImage(szBuffer);
		if (m_wFaceID == i)
		{
			COptionUI* pSelect = (COptionUI*)pItemAvatar->GetItemAt(1);
			pSelect->Selected(true);
		}
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////////////
