#include "Stdafx.h"
#include "Resource.h"
#include "DlgPropertyUse.h"
#include "PlatformEvent.h"
//////////////////////////////////////////////////////////////////////////////////
const TCHAR* const szTextPropertyInfoControlName = TEXT("TextPropertyInfo");

//按钮控件
const TCHAR* const szButtonCloseControlName = TEXT("ButtonClose");
const TCHAR* const szItemPropIOCControlName = TEXT("ItemPropIOC");
const TCHAR* const szItemPropNumControlName = TEXT("ItemPropNum");
const TCHAR* const szItemPropSelectControlName = TEXT("ItemPropSelect");
const TCHAR* const szButtonUseControlName = TEXT("ButtonPropUse");
const TCHAR* const szButtonBuyControlName = TEXT("ButtonPropBuy");

//道具控件
const TCHAR* const szPageLayoutGiftProperty = TEXT("PageLayoutGiftProperty");

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CDlgPropertyUseServer::CDlgPropertyUseServer() : CFGuiDialog(IDD_DLG_POPUP)
,m_pPageLayoutUI(NULL)
{
	::ZeroMemory(&m_curSelectProp, sizeof(m_curSelectProp));
}

//析构函数
CDlgPropertyUseServer::~CDlgPropertyUseServer()
{
}

//创建函数
BOOL CDlgPropertyUseServer::OnInitDialog()
{
	__super::OnInitDialog();

	//修改样式
	ModifyStyle(0, WS_MINIMIZEBOX|WS_MAXIMIZEBOX);

	//设置标题
	SetWindowText(TEXT("道具使用"));

	//居中窗口
	CenterWindow(this);

	return TRUE;
}

//确定函数
VOID CDlgPropertyUseServer::OnOK()
{
	return __super::OnOK();
}

//取消消息
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
	//加载UI
	CVerticalLayoutUI* pVerticalLayoutUI =NULL;
	int nBackPackCount = (int)m_vBackPack.size();
	for(int i = 0; i < nBackPackCount; i++)
	{
		//创建UI
		pVerticalLayoutUI = CreateItem(m_vBackPack[i]);
		m_pPageLayoutUI->Add(pVerticalLayoutUI);
		//更新UI
		UpdateItemUI(pVerticalLayoutUI, m_vBackPack[i]);
	}
}

void CDlgPropertyUseServer::InitControlUI()
{
	//初始数据
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
	//变量赋值
	m_pPageLayoutUI = static_cast<CPageLayoutUI *>(GetControlByName(szPageLayoutGiftProperty));
	if(m_pPageLayoutUI) m_pPageLayoutUI->RemoveAll();
	
	//变量赋值
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
	//创建UI
	CDialogBuilder builder;
	CVerticalLayoutUI* pVerticalLayoutUI =NULL;

	pVerticalLayoutUI = static_cast<CVerticalLayoutUI *>(builder.Create(_T("DlgPropUseItem.xml"), (UINT)0, NULL, &m_PaintManager));
	
	//设置Tag
	CControlUI* pControlUI = pVerticalLayoutUI->FindControl( szItemPropSelectControlName );
	if(pControlUI != NULL) pControlUI->SetTag( (UINT_PTR)&BackpackProperty );

	return pVerticalLayoutUI;
}

CContainerUI* CDlgPropertyUseServer::FindUIItem(CPageLayoutUI* pPageLayoutUI, const DWORD dwPropID)
{
	//查找UI
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

//更新UI
VOID CDlgPropertyUseServer::UpdateItemUI(CContainerUI* pVerticalLayoutUI, const tagBackpackProperty& BackpackProperty)
{
	//道具图标
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
	//道具数量
	CNumberUI* pPropertyNum = (CNumberUI*)pVerticalLayoutUI->FindControl( TEXT("ItemPropNum") );
	if(pPropertyNum != NULL)
	{
		pPropertyNum->SetNumber(BackpackProperty.nCount);
	}
	//道具名字
	CLabelUI* pPropertyName = (CLabelUI*)pVerticalLayoutUI->FindControl( TEXT("ItemPropName") );
	if(pPropertyName != NULL)
	{
		pPropertyName->SetText(BackpackProperty.Property.szName);
	}
}

bool CDlgPropertyUseServer::UpdateProperty(const tagBackpackProperty& BackpackProperty)
{
	//更新删除
	if(BackpackProperty.Property.wIndex <= 0) return false;
	CContainerUI* pContainerUI =  FindUIItem(m_pPageLayoutUI, BackpackProperty.Property.wIndex);
	if( pContainerUI == NULL ) return false;
	if(BackpackProperty.nCount <= 0) m_pPageLayoutUI->Remove(pContainerUI, true);
	else UpdateItemUI(pContainerUI, BackpackProperty);
	return true;
}

//配置
VOID CDlgPropertyUseServer::Initialization(ITCPSocket * pITCPSocket, IClientUserItem * pIMySelfUserItem, IClientUserItem* pTargetUserItem)
{
	//变量赋值
	m_pITCPSocket = pITCPSocket;
	m_pIMySelfUserItem = pIMySelfUserItem;
	m_pTargetUserItem = pTargetUserItem;
	
	//发送消息
	CMD_GR_C_BackpackProperty  BackpackProperty={0};
	BackpackProperty.dwUserID = m_pIMySelfUserItem->GetUserID();
	BackpackProperty.dwKindID = PROP_KIND_GIFT;
	m_pITCPSocket->SendData(MDM_GR_PROPERTY, SUB_GR_PROPERTY_BACKPACK, &BackpackProperty, sizeof(BackpackProperty));
}

//消息
bool CDlgPropertyUseServer::OnServerPropertyUseMessage(WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	if(wSubCmdID == SUB_GR_PROPERTY_BACKPACK_RESULT)
	{
		CMD_GR_S_BackpackProperty* pBackpackProperty = (CMD_GR_S_BackpackProperty*)pData;
		//拷贝数据
		vector<tagBackpackProperty> vBackPack;
		int nCount = (int) pBackpackProperty->dwCount;
		for(int i = 0; i < nCount; i++)
		{
			m_vBackPack.push_back(pBackpackProperty->PropertyInfo[i]);
		}
		//结束标志
		if(pBackpackProperty->dwStatus == 1)
		{
			//加载UI
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

		//更新删掉
		tagBackpackProperty tmpBackpackProperty;
		ZeroMemory(&tmpBackpackProperty, sizeof(tmpBackpackProperty));
		vector<tagBackpackProperty>::iterator itr = m_vBackPack.begin();
		for(; itr != m_vBackPack.end(); ++itr)
		{
			if(itr->Property.wIndex == pPropertyUse->dwPropID){
				itr->nCount -= pPropertyUse->wPropCount;
				//拷贝更新
				memcpy(&tmpBackpackProperty, &(*itr), sizeof(tagBackpackProperty));
				UpdateProperty(*itr);
				if(itr->nCount <= 0) m_vBackPack.erase(itr);
				break;
			}
		}
		
		CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
		tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();
		//设置变量
		pGlobalUserData->lUserScore=pPropertyUse->Score;
		pGlobalUserData->cbMemberOrder=pPropertyUse->cbMemberOrder;			

		//发送事件
		CPlatformEvent * pPlatformEvent=CPlatformEvent::GetInstance();
		if (pPlatformEvent!=NULL) pPlatformEvent->SendPlatformEvent(EVENT_USER_INFO_UPDATE,0L);
		//CInformation Information(NULL);
		//Information.ShowMessageBox(TEXT("道具使用成功！"),MB_OK|MB_ICONSTOP,30);
		return true;
	}
	return false;
}

//消息提醒
void CDlgPropertyUseServer::Notify(TNotifyUI &  msg)
{
	//获取对象
	CControlUI * pControlUI = msg.pSender;

	//点击事件
	if (lstrcmp(msg.sType, TEXT("click")) == 0)
	{
		//关闭按钮
		if(lstrcmp(pControlUI->GetName(), szButtonCloseControlName)==0)
		{
			return OnCancel();
		}
		//选择道具
		else if( pControlUI->GetName() == szItemPropSelectControlName )
		{	
			//获取数据
			tagBackpackProperty* pBackpackProperty = (tagBackpackProperty*)pControlUI->GetTag();
			//拷贝数据
			memcpy(&m_curSelectProp, pBackpackProperty, sizeof(m_curSelectProp));
			//更新UI
			CTextUI* pPropertyInfoUI = static_cast<CTextUI *>(GetControlByName(szTextPropertyInfoControlName));
			if(pPropertyInfoUI != NULL)
			{
				pPropertyInfoUI->SetText(m_curSelectProp.Property.szRegulationsInfo);
			}
		}
		//点击使用
		else if( pControlUI->GetName() == szButtonUseControlName )
		{
			if(m_curSelectProp.Property.wKind == PROP_KIND_GIFT)
			{
				//获取对象
				CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
				tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();
				//定义变量
				CMD_GR_C_PropertyUse PropertyUse;
				PropertyUse.dwUserID = m_pIMySelfUserItem->GetUserID();
				PropertyUse.dwRecvUserID = m_pTargetUserItem->GetUserID();
				PropertyUse.dwPropID = m_curSelectProp.Property.wIndex;
				PropertyUse.wPropCount = 1;
				//发送消息
				m_pITCPSocket->SendData(MDM_GR_PROPERTY, SUB_GR_PROPERTY_USE, &PropertyUse, sizeof(PropertyUse));
			}
		}
		//点击购买
		else if( pControlUI->GetName() == szButtonBuyControlName )
		{
			OnOK();
		}
	}
	return;
}
//////////////////////////////////////////////////////////////////////////////////