#include "Stdafx.h"
#include "Resource.h"
#include "DlgPropertyUse.h"
#include "PlatformEvent.h"
//////////////////////////////////////////////////////////////////////////////////
const TCHAR* const szTextPropertyInfoControlName = TEXT("TextPropertyInfo");

//��ť�ؼ�
const TCHAR* const szButtonCloseControlName = TEXT("ButtonClose");
const TCHAR* const szItemPropIOCControlName = TEXT("ItemPropIOC");
const TCHAR* const szItemPropNumControlName = TEXT("ItemPropNum");
const TCHAR* const szItemPropSelectControlName = TEXT("ItemPropSelect");
const TCHAR* const szButtonUseControlName = TEXT("ButtonPropUse");
const TCHAR* const szButtonBuyControlName = TEXT("ButtonPropBuy");

//���߿ؼ�
const TCHAR* const szPageLayoutGiftProperty = TEXT("PageLayoutGiftProperty");

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CDlgPropertyUseServer::CDlgPropertyUseServer() : CFGuiDialog(IDD_DLG_POPUP)
,m_pPageLayoutUI(NULL)
{
	::ZeroMemory(&m_curSelectProp, sizeof(m_curSelectProp));
}

//��������
CDlgPropertyUseServer::~CDlgPropertyUseServer()
{
}

//��������
BOOL CDlgPropertyUseServer::OnInitDialog()
{
	__super::OnInitDialog();

	//�޸���ʽ
	ModifyStyle(0, WS_MINIMIZEBOX|WS_MAXIMIZEBOX);

	//���ñ���
	SetWindowText(TEXT("����ʹ��"));

	//���д���
	CenterWindow(this);

	return TRUE;
}

//ȷ������
VOID CDlgPropertyUseServer::OnOK()
{
	return __super::OnOK();
}

//ȡ����Ϣ
VOID CDlgPropertyUseServer::OnCancel()
{
	return __super::OnCancel();
}


LPCTSTR CDlgPropertyUseServer::GetSkinFile()
{
	return TEXT("DlgPropertyUse.xml"); 
}

VOID CDlgPropertyUseServer::InitData()
{
	
}

VOID CDlgPropertyUseServer::LoadDataFromMemory()
{
	//����UI
	CVerticalLayoutUI* pVerticalLayoutUI =NULL;
	int nBackPackCount = (int)m_vBackPack.size();
	for(int i = 0; i < nBackPackCount; i++)
	{
		//����UI
		pVerticalLayoutUI = CreateItem(m_vBackPack[i]);
		m_pPageLayoutUI->Add(pVerticalLayoutUI);
		//����UI
		UpdateItemUI(pVerticalLayoutUI, m_vBackPack[i]);
	}
}

void CDlgPropertyUseServer::InitControlUI()
{
	//��ʼ����
	InitData();
	InitUI();
}

VOID CDlgPropertyUseServer::LoadDataFromUI()
{

}

VOID CDlgPropertyUseServer::UpdateData()
{

}

VOID CDlgPropertyUseServer::InitUI()
{
	//������ֵ
	m_pPageLayoutUI = static_cast<CPageLayoutUI *>(GetControlByName(szPageLayoutGiftProperty));
	if(m_pPageLayoutUI) m_pPageLayoutUI->RemoveAll();
	
	//������ֵ
	CTextUI* pPropertyInfoUI = static_cast<CTextUI *>(GetControlByName(szTextPropertyInfoControlName));
	if(pPropertyInfoUI != NULL)
	{
		pPropertyInfoUI->SetText(TEXT(""));
	}
}

VOID CDlgPropertyUseServer::UpdateUI()
{

}

CVerticalLayoutUI* CDlgPropertyUseServer::CreateItem(tagBackpackProperty& BackpackProperty)
{
	//����UI
	CDialogBuilder builder;
	CVerticalLayoutUI* pVerticalLayoutUI =NULL;

	pVerticalLayoutUI = static_cast<CVerticalLayoutUI *>(builder.Create(_T("DlgPropUseItem.xml"), (UINT)0, NULL, &m_PaintManager));
	
	//����Tag
	CControlUI* pControlUI = pVerticalLayoutUI->FindControl( szItemPropSelectControlName );
	if(pControlUI != NULL) pControlUI->SetTag( (UINT_PTR)&BackpackProperty );

	return pVerticalLayoutUI;
}

CContainerUI* CDlgPropertyUseServer::FindUIItem(CPageLayoutUI* pPageLayoutUI, const DWORD dwPropID)
{
	//����UI
	if(pPageLayoutUI == NULL) return NULL;

	for(int i = 0; i < pPageLayoutUI->GetCount(); i++)
	{
		CContainerUI* pContainerUI = static_cast<CContainerUI*>(pPageLayoutUI->GetItemAt(i));
		if(pContainerUI != NULL)
		{
			CControlUI* pControlUI = pContainerUI->FindControl(szItemPropSelectControlName);
			if(pControlUI != NULL)
			{
				tagBackpackProperty* pBackpackProperty = (tagBackpackProperty*)pControlUI->GetTag();
				if(pBackpackProperty->Property.wIndex == dwPropID) return pContainerUI;
			}
		}
	}
	return NULL;
}

//����UI
VOID CDlgPropertyUseServer::UpdateItemUI(CContainerUI* pVerticalLayoutUI, const tagBackpackProperty& BackpackProperty)
{
	//����ͼ��
	CLabelUI* pPropertyImage = (CLabelUI*)pVerticalLayoutUI->FindControl( TEXT("ItemPropIOC") );
	if(pPropertyImage != NULL)
	{
		CStdString	csImageKind;
		csImageKind.Format(TEXT("file=\'GMAE_PROPERTY\\GMAE_PROPERTY_%d.png\' restype=\'FILE\'"),BackpackProperty.Property.wIndex);
		if(m_PaintManager.GetImageEx(csImageKind)==NULL) csImageKind.Empty();
		if(csImageKind.IsEmpty()==true) 
		{
			csImageKind=TEXT("file=\'GMAE_PROPERTY\\GMAE_PROPERTY_NULL.png\' restype=\'FILE\'");
		}
		pPropertyImage->SetBkImage(csImageKind);
	}
	//��������
	CNumberUI* pPropertyNum = (CNumberUI*)pVerticalLayoutUI->FindControl( TEXT("ItemPropNum") );
	if(pPropertyNum != NULL)
	{
		pPropertyNum->SetNumber(BackpackProperty.nCount);
	}
	//��������
	CLabelUI* pPropertyName = (CLabelUI*)pVerticalLayoutUI->FindControl( TEXT("ItemPropName") );
	if(pPropertyName != NULL)
	{
		pPropertyName->SetText(BackpackProperty.Property.szName);
	}
}

bool CDlgPropertyUseServer::UpdateProperty(const tagBackpackProperty& BackpackProperty)
{
	//����ɾ��
	if(BackpackProperty.Property.wIndex <= 0) return false;
	CContainerUI* pContainerUI =  FindUIItem(m_pPageLayoutUI, BackpackProperty.Property.wIndex);
	if( pContainerUI == NULL ) return false;
	if(BackpackProperty.nCount <= 0) m_pPageLayoutUI->Remove(pContainerUI, true);
	else UpdateItemUI(pContainerUI, BackpackProperty);
	return true;
}

//����
VOID CDlgPropertyUseServer::Initialization(ITCPSocket * pITCPSocket, IClientUserItem * pIMySelfUserItem, IClientUserItem* pTargetUserItem)
{
	//������ֵ
	m_pITCPSocket = pITCPSocket;
	m_pIMySelfUserItem = pIMySelfUserItem;
	m_pTargetUserItem = pTargetUserItem;
	
	//������Ϣ
	CMD_GR_C_BackpackProperty  BackpackProperty={0};
	BackpackProperty.dwUserID = m_pIMySelfUserItem->GetUserID();
	BackpackProperty.dwKindID = PROP_KIND_GIFT;
	m_pITCPSocket->SendData(MDM_GR_PROPERTY, SUB_GR_PROPERTY_BACKPACK, &BackpackProperty, sizeof(BackpackProperty));
}

//��Ϣ
bool CDlgPropertyUseServer::OnServerPropertyUseMessage(WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	if(wSubCmdID == SUB_GR_PROPERTY_BACKPACK_RESULT)
	{
		CMD_GR_S_BackpackProperty* pBackpackProperty = (CMD_GR_S_BackpackProperty*)pData;
		//��������
		vector<tagBackpackProperty> vBackPack;
		int nCount = (int) pBackpackProperty->dwCount;
		for(int i = 0; i < nCount; i++)
		{
			m_vBackPack.push_back(pBackpackProperty->PropertyInfo[i]);
		}
		//������־
		if(pBackpackProperty->dwStatus == 1)
		{
			//����UI
			LoadDataFromMemory();
		}
		return true;
	}
	else if(wSubCmdID == SUB_GR_PROPERTY_USE_RESULT)
	{
		CMD_GR_S_PropertyUse* pPropertyUse = (CMD_GR_S_PropertyUse*)pData;
		if(pPropertyUse->dwUserID != m_pIMySelfUserItem->GetUserID())
		{
			return false;
		}
		if(pPropertyUse->dwHandleCode != 0)
		{
			return false;
		}

		//����ɾ��
		tagBackpackProperty tmpBackpackProperty;
		ZeroMemory(&tmpBackpackProperty, sizeof(tmpBackpackProperty));
		vector<tagBackpackProperty>::iterator itr = m_vBackPack.begin();
		for(; itr != m_vBackPack.end(); ++itr)
		{
			if(itr->Property.wIndex == pPropertyUse->dwPropID){
				itr->nCount -= pPropertyUse->wPropCount;
				//��������
				memcpy(&tmpBackpackProperty, &(*itr), sizeof(tagBackpackProperty));
				UpdateProperty(*itr);
				if(itr->nCount <= 0) m_vBackPack.erase(itr);
				break;
			}
		}
		
		CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
		tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();
		//���ñ���
		pGlobalUserData->lUserScore=pPropertyUse->Score;
		pGlobalUserData->cbMemberOrder=pPropertyUse->cbMemberOrder;			

		//�����¼�
		CPlatformEvent * pPlatformEvent=CPlatformEvent::GetInstance();
		if (pPlatformEvent!=NULL) pPlatformEvent->SendPlatformEvent(EVENT_USER_INFO_UPDATE,0L);
		//CInformation Information(NULL);
		//Information.ShowMessageBox(TEXT("����ʹ�óɹ���"),MB_OK|MB_ICONSTOP,30);
		return true;
	}
	return false;
}

//��Ϣ����
void CDlgPropertyUseServer::Notify(TNotifyUI &  msg)
{
	//��ȡ����
	CControlUI * pControlUI = msg.pSender;

	//����¼�
	if (lstrcmp(msg.sType, TEXT("click")) == 0)
	{
		//�رհ�ť
		if(lstrcmp(pControlUI->GetName(), szButtonCloseControlName)==0)
		{
			return OnCancel();
		}
		//ѡ�����
		else if( pControlUI->GetName() == szItemPropSelectControlName )
		{	
			//��ȡ����
			tagBackpackProperty* pBackpackProperty = (tagBackpackProperty*)pControlUI->GetTag();
			//��������
			memcpy(&m_curSelectProp, pBackpackProperty, sizeof(m_curSelectProp));
			//����UI
			CTextUI* pPropertyInfoUI = static_cast<CTextUI *>(GetControlByName(szTextPropertyInfoControlName));
			if(pPropertyInfoUI != NULL)
			{
				pPropertyInfoUI->SetText(m_curSelectProp.Property.szRegulationsInfo);
			}
		}
		//���ʹ��
		else if( pControlUI->GetName() == szButtonUseControlName )
		{
			if(m_curSelectProp.Property.wKind == PROP_KIND_GIFT)
			{
				//��ȡ����
				CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
				tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();
				//�������
				CMD_GR_C_PropertyUse PropertyUse;
				PropertyUse.dwUserID = m_pIMySelfUserItem->GetUserID();
				PropertyUse.dwRecvUserID = m_pTargetUserItem->GetUserID();
				PropertyUse.dwPropID = m_curSelectProp.Property.wIndex;
				PropertyUse.wPropCount = 1;
				//������Ϣ
				m_pITCPSocket->SendData(MDM_GR_PROPERTY, SUB_GR_PROPERTY_USE, &PropertyUse, sizeof(PropertyUse));
			}
		}
		//�������
		else if( pControlUI->GetName() == szButtonBuyControlName )
		{
			OnOK();
		}
	}
	return;
}
//////////////////////////////////////////////////////////////////////////////////