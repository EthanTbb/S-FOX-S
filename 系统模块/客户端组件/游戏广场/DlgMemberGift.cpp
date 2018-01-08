#include "Stdafx.h"
#include "Resource.h"
#include "DlgMemberGift.h"
#include "GlobalUnits.h"

//////////////////////////////////////////////////////////////////////////////////
//按钮控件
const TCHAR* const szButtonCloseControlName = TEXT("ButtonClose");
const TCHAR* const szButtonTakeControlName = TEXT("ButtonTake");
const TCHAR* const szPageLayoutGiftControlName = TEXT("PageLayoutGift");

const TCHAR* const szLableItemPropIOC = TEXT("ItemPropIOC");
const TCHAR* const szLableItemPropName = TEXT("ItemPropName");
const TCHAR* const szLableItemPropNum = TEXT("ItemPropNum");
//////////////////////////////////////////////////////////////////////////////////

//构造函数
CDlgMemberGift::CDlgMemberGift() : CFGuiDialog(IDD_DLG_POPUP)
{
	m_bEnableReceive=false;
}

//析构函数
CDlgMemberGift::~CDlgMemberGift()
{
}

//创建函数
BOOL CDlgMemberGift::OnInitDialog()
{
	__super::OnInitDialog();

	//修改样式
	ModifyStyle(0, WS_MINIMIZEBOX|WS_MAXIMIZEBOX);

	//设置标题
	SetWindowText(TEXT("会员礼包"));

	//居中窗口
	CenterWindow(this);

	LoadDataFromMemory();

	CreateUI();

	UpdateUI();


	return TRUE;
}

//确定函数
VOID CDlgMemberGift::OnOK()
{

	return __super::OnOK();
}

//取消消息
VOID CDlgMemberGift::OnCancel()
{
	return __super::OnCancel();
}

void CDlgMemberGift::InitControlUI()
{
}

//消息提醒
void CDlgMemberGift::Notify(TNotifyUI &  msg)
{
	//获取对象
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
	//获取对象
	ASSERT(CGamePropertyListData::GetInstance()!=NULL);
	CGamePropertyListData * pGamePropertyListData=CGamePropertyListData::GetInstance();

	//变量定义
	POSITION Position=NULL;
	CGamePropertyItem		 * pGamePropertyItem = NULL;
	CGamePropertySubItem     * pGamePropertySubItem = NULL;
	vector<CGamePropertyItem *>::iterator itGameProperty;
	vector<CGamePropertySubItem *>::iterator itGamePropertySub;
	m_vGamePropertyList.clear();
	m_vGamePropertySubList.clear();

	//加载道具
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

	//加载子道具
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

		//取出子道具
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

		//创建礼包
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
					//创建UI
					CDialogBuilder builder;
					CContainerUI* pNewPropertyItem =NULL;
					pNewPropertyItem = static_cast<CContainerUI *>(builder.Create(_T("DlgMemberGiftItem.xml"), (UINT)0, NULL, &m_PaintManager));

					//设置图标
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

					//设置数量
					CNumberUI* pNum = (CNumberUI*)pNewPropertyItem->FindControl( szLableItemPropNum );
					if(pNum != NULL)
					{
						float fNum = (float)pGamePropertySubTemp->m_PropertySubItem.dwPropertyCount;
						pNum->SetNumber(fNum);
					}

					//设置名字
					CControlUI *pName = pNewPropertyItem->FindControl(szLableItemPropName);
					if (pName!=NULL)
					{
						pName->SetText(pGamePropertyItem->m_PropertyItem.szName);
					}

					pPageLayoutUI->Add(pNewPropertyItem);

				}
			}
		}
		//创建金币
		if (pMemberParameter->dwMemberDayPresent > 0)
		{
			//创建UI
			CDialogBuilder builder;
			CContainerUI* pNewPropertyItem =NULL;
			pNewPropertyItem = static_cast<CContainerUI *>(builder.Create(_T("DlgMemberGiftItem.xml"), (UINT)0, NULL, &m_PaintManager));

			//设置图标
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

			//设置数量
			CNumberUI* pNum = (CNumberUI*)pNewPropertyItem->FindControl( szLableItemPropNum );
			if(pNum != NULL)
			{
				pNum->SetVisible(false);;
			}


			//设置名字
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