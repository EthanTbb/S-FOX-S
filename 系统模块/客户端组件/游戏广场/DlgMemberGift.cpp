#include "Stdafx.h"
#include "Resource.h"
#include "DlgMemberGift.h"
#include "GlobalUnits.h"

//////////////////////////////////////////////////////////////////////////////////
//��ť�ؼ�
const TCHAR* const szButtonCloseControlName = TEXT("ButtonClose");
const TCHAR* const szButtonTakeControlName = TEXT("ButtonTake");
const TCHAR* const szPageLayoutGiftControlName = TEXT("PageLayoutGift");

const TCHAR* const szLableItemPropIOC = TEXT("ItemPropIOC");
const TCHAR* const szLableItemPropName = TEXT("ItemPropName");
const TCHAR* const szLableItemPropNum = TEXT("ItemPropNum");
//////////////////////////////////////////////////////////////////////////////////

//���캯��
CDlgMemberGift::CDlgMemberGift() : CFGuiDialog(IDD_DLG_POPUP)
{
	m_bEnableReceive=false;
}

//��������
CDlgMemberGift::~CDlgMemberGift()
{
}

//��������
BOOL CDlgMemberGift::OnInitDialog()
{
	__super::OnInitDialog();

	//�޸���ʽ
	ModifyStyle(0, WS_MINIMIZEBOX|WS_MAXIMIZEBOX);

	//���ñ���
	SetWindowText(TEXT("��Ա���"));

	//���д���
	CenterWindow(this);

	LoadDataFromMemory();

	CreateUI();

	UpdateUI();


	return TRUE;
}

//ȷ������
VOID CDlgMemberGift::OnOK()
{

	return __super::OnOK();
}

//ȡ����Ϣ
VOID CDlgMemberGift::OnCancel()
{
	return __super::OnCancel();
}

void CDlgMemberGift::InitControlUI()
{
}

//��Ϣ����
void CDlgMemberGift::Notify(TNotifyUI &  msg)
{
	//��ȡ����
	CControlUI * pControlUI = msg.pSender;

	if (lstrcmp(msg.sType, TEXT("click")) == 0)
	{
		if(lstrcmp(pControlUI->GetName(), szButtonCloseControlName)==0)
		{
			return OnCancel();
		}
		else if(lstrcmp(pControlUI->GetName(), szButtonTakeControlName)==0 	)
		{
			return OnOK();
		}
	}
	
	return;
}

LPCTSTR CDlgMemberGift::GetSkinFile()
{
	return TEXT("DlgMemberGift.xml"); 
}

void CDlgMemberGift::LoadDataFromMemory()
{
	//��ȡ����
	ASSERT(CGamePropertyListData::GetInstance()!=NULL);
	CGamePropertyListData * pGamePropertyListData=CGamePropertyListData::GetInstance();

	//��������
	POSITION Position=NULL;
	CGamePropertyItem		 * pGamePropertyItem = NULL;
	CGamePropertySubItem     * pGamePropertySubItem = NULL;
	vector<CGamePropertyItem *>::iterator itGameProperty;
	vector<CGamePropertySubItem *>::iterator itGamePropertySub;
	m_vGamePropertyList.clear();
	m_vGamePropertySubList.clear();

	//���ص���
	Position=NULL;
	while (true)
	{
		pGamePropertyItem=pGamePropertyListData->EmunGamePropertyItem(Position);
		if (pGamePropertyItem==NULL) break;
		for (itGameProperty = m_vGamePropertyList.begin();itGameProperty!=m_vGamePropertyList.end();itGameProperty++)
		{
			CGamePropertyItem * pGamePropertyTemp= (CGamePropertyItem *)(*itGameProperty);
			if (pGamePropertyItem->m_PropertyItem.dwSortID <=pGamePropertyTemp->m_PropertyItem.dwSortID)
			{
				m_vGamePropertyList.insert(itGameProperty,pGamePropertyItem);
				break;
			}
		}
		if (itGameProperty==m_vGamePropertyList.end()) m_vGamePropertyList.push_back(pGamePropertyItem);
		if (Position==NULL) break;
	}

	//�����ӵ���
	Position=NULL;
	while (true)
	{
		pGamePropertySubItem=pGamePropertyListData->EmunGamePropertySubItem(Position);
		if (pGamePropertySubItem==NULL) break;
		for (itGamePropertySub = m_vGamePropertySubList.begin();itGamePropertySub!=m_vGamePropertySubList.end();itGamePropertySub++)
		{
			CGamePropertySubItem * pGamePropertySubTemp= (CGamePropertySubItem *)(*itGamePropertySub);
			if (pGamePropertySubItem->m_PropertySubItem.dwSortID <=pGamePropertySubTemp->m_PropertySubItem.dwSortID)
			{
				m_vGamePropertySubList.insert(itGamePropertySub,pGamePropertySubItem);
				break;
			}
		}
		if (itGamePropertySub==m_vGamePropertySubList.end()) m_vGamePropertySubList.push_back(pGamePropertySubItem);
		if (Position==NULL) break;
	}

}

void CDlgMemberGift::CreateUI()
{
	CPageLayoutUI * pPageLayoutUI = static_cast<CPageLayoutUI *>(GetControlByName(szPageLayoutGiftControlName));	
	if(pPageLayoutUI)
	{
		CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
		tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();
		CUserItemElement * pUserItemElement=CUserItemElement::GetInstance();
		int nMemberOrder = pGlobalUserData->cbMemberOrder;
		if (nMemberOrder == 0)
		{
			nMemberOrder = 1;
		}
		const tagMemberParameterNew* pMemberParameter = CUserItemElement::GetInstance()->GetUserMemberParameter(nMemberOrder);

		//ȡ���ӵ���
		vector<CGamePropertySubItem *> vCurrPropertySubItemList;
		vector<CGamePropertySubItem *>::iterator itGamePropertySub;
		for (itGamePropertySub = m_vGamePropertySubList.begin();itGamePropertySub!=m_vGamePropertySubList.end();itGamePropertySub++)
		{
			CGamePropertySubItem * pGamePropertySubTemp= (CGamePropertySubItem *)(*itGamePropertySub);
			if (pGamePropertySubTemp->m_PropertySubItem.dwOwnerPropertyID == pMemberParameter->dwMemberDayGiftID)
			{
				vCurrPropertySubItemList.push_back(pGamePropertySubTemp);
			}
		}

		//�������
		vector<CGamePropertyItem *> vCurrPropertyItemList;
		vector<CGamePropertyItem *>::iterator itGameProperty;
		for (itGamePropertySub = vCurrPropertySubItemList.begin();itGamePropertySub!=vCurrPropertySubItemList.end();itGamePropertySub++)
		{
			CGamePropertySubItem * pGamePropertySubTemp= (CGamePropertySubItem *)(*itGamePropertySub);
			for (itGameProperty = m_vGamePropertyList.begin();itGameProperty!=m_vGamePropertyList.end();itGameProperty++)
			{
				CGamePropertyItem * pGamePropertyItem= (CGamePropertyItem *)(*itGameProperty);

				if (pGamePropertySubTemp->m_PropertySubItem.dwPropertyID == pGamePropertyItem->m_PropertyItem.dwPropertyID)
				{
					//����UI
					CDialogBuilder builder;
					CContainerUI* pNewPropertyItem =NULL;
					pNewPropertyItem = static_cast<CContainerUI *>(builder.Create(_T("DlgMemberGiftItem.xml"), (UINT)0, NULL, &m_PaintManager));

					//����ͼ��
					CStdString	csImageKind;
					csImageKind.Format(TEXT("file=\'GMAE_PROPERTY\\GMAE_PROPERTY_%d.png\' restype=\'FILE\'"),pGamePropertyItem->m_PropertyItem.dwPropertyID);
					if(m_PaintManager.GetImageEx(csImageKind)==NULL) csImageKind.Empty();
					if(csImageKind.IsEmpty()==true) 
					{
						csImageKind=TEXT("file=\'GMAE_PROPERTY\\GMAE_PROPERTY_NULL.png\' restype=\'FILE\'");
					}
					CControlUI *pIco = pNewPropertyItem->FindControl(szLableItemPropIOC);
					if (pIco!=NULL)
					{
						pIco->SetBkImage(csImageKind);
					}

					//��������
					CNumberUI* pNum = (CNumberUI*)pNewPropertyItem->FindControl( szLableItemPropNum );
					if(pNum != NULL)
					{
						float fNum = (float)pGamePropertySubTemp->m_PropertySubItem.dwPropertyCount;
						pNum->SetNumber(fNum);
					}

					//��������
					CControlUI *pName = pNewPropertyItem->FindControl(szLableItemPropName);
					if (pName!=NULL)
					{
						pName->SetText(pGamePropertyItem->m_PropertyItem.szName);
					}

					pPageLayoutUI->Add(pNewPropertyItem);

				}
			}
		}
		//�������
		if (pMemberParameter->dwMemberDayPresent > 0)
		{
			//����UI
			CDialogBuilder builder;
			CContainerUI* pNewPropertyItem =NULL;
			pNewPropertyItem = static_cast<CContainerUI *>(builder.Create(_T("DlgMemberGiftItem.xml"), (UINT)0, NULL, &m_PaintManager));

			//����ͼ��
			CStdString	csImageKind;
			csImageKind.Format(TEXT("file=\'GMAE_PROPERTY\\GMAE_PROPERTY_MONEY.png\' restype=\'FILE\'"));
			if(m_PaintManager.GetImageEx(csImageKind)==NULL) csImageKind.Empty();
			if(csImageKind.IsEmpty()==true) 
			{
				csImageKind=TEXT("file=\'GMAE_PROPERTY\\GMAE_PROPERTY_NULL.png\' restype=\'FILE\'");
			}
			CControlUI *pIco = pNewPropertyItem->FindControl(szLableItemPropIOC);
			if (pIco!=NULL)
			{
				pIco->SetBkImage(csImageKind);
			}

			//��������
			CNumberUI* pNum = (CNumberUI*)pNewPropertyItem->FindControl( szLableItemPropNum );
			if(pNum != NULL)
			{
				pNum->SetVisible(false);;
			}


			//��������
			TCHAR szText[128]={0};	
			_sntprintf(szText, CountArray(szText), TEXT("%d"), pMemberParameter->dwMemberDayPresent);
			CControlUI *pName = pNewPropertyItem->FindControl(szLableItemPropName);
			if (pName!=NULL)
			{
				pName->SetText(szText);
			}

			pPageLayoutUI->Add(pNewPropertyItem);
		}

	}
}

void CDlgMemberGift::UpdateUI()
{
	CControlUI * pControlUI = GetControlByName(szButtonTakeControlName);
	pControlUI->SetEnabled(m_bEnableReceive);
}

void CDlgMemberGift::SetEnableReceive(bool EnableReceive)
{
	m_bEnableReceive = EnableReceive;
}

//////////////////////////////////////////////////////////////////////////////////