#include "Stdafx.h"
#include "Resource.h"
#include "DlgBag.h"
#include "DlgBagUse.h"
//#include "DlgBagPresent.h"
#include "PlatformFrame.h"
#include "PlatformEvent.h"
#include "DlgInformation.h"
//////////////////////////////////////////////////////////////////////////////////
//按钮控件
const TCHAR* const szButtonCloseControlName = TEXT("ButtonClose");
const TCHAR* const szBagItemPropIOCControlName = TEXT("BagItemPropIOC");
const TCHAR* const szBagItemPropNumControlName = TEXT("BagItemPropNum");
const TCHAR* const szBagItemropSelectControlName = TEXT("BagItemropSelect");
const TCHAR* const szButtonMyBagRecordControlName = TEXT("ButtonMyBagRecord");
const TCHAR* const szButtonMyShopControlName = TEXT("ButtonMyShop");
const TCHAR* const szButtonMyInsureControlName = TEXT("ButtonMyInsure");

//道具控件
const TCHAR* const szPageLayoutAllProperty = TEXT("PageLayoutAllProperty");
const TCHAR* const szPageLayoutGemProperty = TEXT("PageLayoutGemProperty");
const TCHAR* const szPageLayoutCardProperty = TEXT("PageLayoutCardProperty");
const TCHAR* const szPageLayoutProperty = TEXT("PageLayoutProperty");
const TCHAR* const szPageLayoutGiftProperty = TEXT("PageLayoutGiftProperty");

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CDlgBagMain::CDlgBagMain() : CFGuiDialog(IDD_DLG_POPUP)
{
	m_pDlgBagUse = NULL;
	for(int  i = 0; i <5; i++)
	{
		m_pPageLayoutUI[i] = NULL;
	}
	ZeroMemory(&m_curSelectProp, sizeof(m_curSelectProp));
	m_bIsUseProperty = false;
	m_bIsGiveProperty = false;
	m_dwGiveReceiveID = 0;
	m_ePresendMode = PRESEND_NICKNAME;
}

//析构函数
CDlgBagMain::~CDlgBagMain()
{
}

//创建函数
BOOL CDlgBagMain::OnInitDialog()
{
	__super::OnInitDialog();

	//修改样式
	ModifyStyle(0, WS_MINIMIZEBOX|WS_MAXIMIZEBOX);

	//设置标题
	SetWindowText(TEXT("我的背包"));

	//居中窗口
	CenterWindow(this);

	//下移距离
	CRect rc;
	GetWindowRect(&rc);
	rc.top += 30;
	rc.bottom += 30;
	//移动窗口
	MoveWindow(rc);


	return TRUE;
}

//确定函数
VOID CDlgBagMain::OnOK()
{

	//return __super::OnOK();
}

//取消消息
VOID CDlgBagMain::OnCancel()
{
	return __super::OnCancel();
}

void CDlgBagMain::EndDialog()
{
	if(m_pDlgBagUse!=NULL) m_pDlgBagUse->EndDialog(IDOK);
	__super::EndDialog(IDOK);
}

//是否使用道具
bool CDlgBagMain::IsUseProperty()
{
	return m_bIsUseProperty;
}

//是否赠送道具
bool CDlgBagMain::IsGiveProperty()
{
	return m_bIsGiveProperty;
}


//消息提醒
void CDlgBagMain::Notify(TNotifyUI &  msg)
{
	//获取对象
	CControlUI * pControlUI = msg.pSender;

	//点击事件
	if (lstrcmp(msg.sType, TEXT("click")) == 0)
	{
		if(lstrcmp(pControlUI->GetName(), szButtonCloseControlName)==0)
		{
			return OnCancel();
		}
		else if( pControlUI->GetName() == szBagItemropSelectControlName )
		{
			tagBackpackProperty* pBackpackProperty = (tagBackpackProperty*)pControlUI->GetTag();
			ShowUseProperty(pBackpackProperty);
		}
		else if( pControlUI->GetName() == szButtonMyShopControlName )
		{
			CPlatformEvent * pPlatformEvent=CPlatformEvent::GetInstance();
			if (pPlatformEvent!=NULL) pPlatformEvent->PostPlatformEvent(EVENT_USER_SHOW_SHOP,0L);

			OnCancel();
		}
		else if( pControlUI->GetName() == szButtonMyInsureControlName )
		{
			CPlatformEvent * pPlatformEvent=CPlatformEvent::GetInstance();
			if (pPlatformEvent!=NULL) pPlatformEvent->PostPlatformEvent(EVENT_USER_SHOW_MY_INSURE,0L);

			OnCancel();
		}
		else if( pControlUI->GetName() == szButtonMyBagRecordControlName )
		{
			CPlatformEvent * pPlatformEvent=CPlatformEvent::GetInstance();
			if (pPlatformEvent!=NULL) pPlatformEvent->PostPlatformEvent(EVENT_USER_SHOW_BAG_RECORD,0L);

			OnCancel();
		}
	}
	
	return;
}

LPCTSTR CDlgBagMain::GetSkinFile()
{
	return TEXT("DlgBag.xml"); 
}

VOID CDlgBagMain::InitData()
{
	
}

VOID CDlgBagMain::LoadDataFromMemory()
{
	CVerticalLayoutUI* pVerticalLayoutUI =NULL;
	int nCount = (int) (m_vBackPack.size());
	for(int i = 0; i < nCount; i++)
	{
		pVerticalLayoutUI = CreateItem(m_vBackPack[i]);
		const int nType = m_vBackPack[i].Property.wIndex/100;
		AddByType(pVerticalLayoutUI, nType);
		UpdateItemUI(pVerticalLayoutUI, m_vBackPack[i]);

		pVerticalLayoutUI = CreateItem(m_vBackPack[i]);
		AddByType(pVerticalLayoutUI);
		UpdateItemUI(pVerticalLayoutUI, m_vBackPack[i]);
	}
}

void CDlgBagMain::InitControlUI()
{
	InitData();
	InitUI();
}

VOID CDlgBagMain::LoadDataFromUI()
{

}

VOID CDlgBagMain::UpdateData()
{

}

VOID CDlgBagMain::InitUI()
{
	m_pPageLayoutUI[0] = static_cast<CPageLayoutUI *>(GetControlByName(szPageLayoutAllProperty));
	m_pPageLayoutUI[1] = static_cast<CPageLayoutUI *>(GetControlByName(szPageLayoutGemProperty));
	m_pPageLayoutUI[2] = static_cast<CPageLayoutUI *>(GetControlByName(szPageLayoutCardProperty));
	m_pPageLayoutUI[3] = static_cast<CPageLayoutUI *>(GetControlByName(szPageLayoutProperty));
	m_pPageLayoutUI[4] = static_cast<CPageLayoutUI *>(GetControlByName(szPageLayoutGiftProperty));

	Clear();
}

VOID CDlgBagMain::UpdateUI()
{

}

VOID CDlgBagMain::Clear()
{
	for(int i = 0; i < 5; ++i)
	{
		if(m_pPageLayoutUI[i] != NULL)
		{
			m_pPageLayoutUI[i]->RemoveAll();
		}
	}
	m_vBackPack.clear();
}

CVerticalLayoutUI* CDlgBagMain::CreateItem(tagBackpackProperty& BackpackProperty)
{
	CDialogBuilder builder;
	CVerticalLayoutUI* pVerticalLayoutUI =NULL;
	
	pVerticalLayoutUI = static_cast<CVerticalLayoutUI *>(builder.Create(_T("DlgBagItem.xml"), (UINT)0, NULL, &m_PaintManager));

	CControlUI* pControlUI = pVerticalLayoutUI->FindControl( szBagItemropSelectControlName );
	if(pControlUI != NULL) pControlUI->SetTag( (UINT_PTR)&BackpackProperty );

	return pVerticalLayoutUI;
}

VOID CDlgBagMain::UpdateItemUI(CContainerUI* pVerticalLayoutUI, const tagBackpackProperty& BackpackProperty)
{
	CLabelUI* pPropertyImage = (CLabelUI*)pVerticalLayoutUI->FindControl( TEXT("BagItemPropIOC") );
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

	CNumberUI* pPropertyNum = (CNumberUI*)pVerticalLayoutUI->FindControl( TEXT("BagItemPropNum") );
	if(pPropertyNum != NULL)
	{
		float fCount = (float)BackpackProperty.nCount;
		pPropertyNum->SetNumber(fCount);
	}
}

bool CDlgBagMain::AddByType(CContainerUI* pVerticalLayoutUI, const int nType)
{
	if(pVerticalLayoutUI == NULL) return false;

	if(nType == 0) //礼物
	{
		if(m_pPageLayoutUI[4]) m_pPageLayoutUI[4]->Add(pVerticalLayoutUI);
	}
	else if(nType == 1) //宝石
	{
		if(m_pPageLayoutUI[1]) m_pPageLayoutUI[1]->Add(pVerticalLayoutUI);
	}
	else if(nType == 2) //卡片
	{
		if(m_pPageLayoutUI[2]) m_pPageLayoutUI[2]->Add(pVerticalLayoutUI);
	}
	else if(nType == 3) //道具
	{
		if(m_pPageLayoutUI[3]) m_pPageLayoutUI[3]->Add(pVerticalLayoutUI);
	}
	else
	{
		if(m_pPageLayoutUI[0]) m_pPageLayoutUI[0]->Add(pVerticalLayoutUI);
	}
	return true;
}


CContainerUI* CDlgBagMain::FindUIItem(CPageLayoutUI* pPageLayoutUI, const DWORD dwPropID)
{
	if(pPageLayoutUI == NULL) return NULL;

	for(int i = 0; i < pPageLayoutUI->GetCount(); i++)
	{
		CContainerUI* pContainerUI = static_cast<CContainerUI*>(pPageLayoutUI->GetItemAt(i));
		if(pContainerUI != NULL)
		{
			CControlUI* pControlUI = pContainerUI->FindControl(szBagItemropSelectControlName);
			if(pControlUI != NULL)
			{
				tagBackpackProperty* pBackpackProperty = (tagBackpackProperty*)pControlUI->GetTag();
				if(pBackpackProperty->Property.wIndex == dwPropID) return pContainerUI;
			}
		}
	}
	return NULL;
}

bool CDlgBagMain::UpdateProperty(const tagBackpackProperty& BackpackProperty)
{
	bool bUpdate = false;
	for(int i = 0; i < CountArray(m_pPageLayoutUI); i++)
	{
		if(BackpackProperty.Property.wIndex == 0) continue;
		CContainerUI* pContainerUI =  FindUIItem(m_pPageLayoutUI[i], BackpackProperty.Property.wIndex);
		if( pContainerUI == NULL ) continue;
		if(BackpackProperty.nCount <= 0) m_pPageLayoutUI[i]->Remove(pContainerUI, false);
		else UpdateItemUI(pContainerUI, BackpackProperty);
		bUpdate = true;
	}
	return bUpdate;
}

INT_PTR CDlgBagMain::ShowUseProperty(const tagBackpackProperty* pBackpackProperty)
{
	if(pBackpackProperty == NULL) return IDCANCEL;
	m_bIsGiveProperty = false;
	m_bIsUseProperty = false;

	memcpy(&m_curSelectProp, pBackpackProperty, sizeof(m_curSelectProp));

	CDlgBagUse bagUse;
	m_pDlgBagUse = &bagUse;
	bagUse.SetProperty(pBackpackProperty);
	bagUse.DoModal();
	m_pDlgBagUse = NULL;
	m_bIsUseProperty = bagUse.IsUseProperty();

	if( bagUse.IsUseProperty() )
	{
		m_curSelectProp.nCount = bagUse.GetUseCount();
		return IDOK;
	}
	else if( bagUse.IsGiveProperty() ) 
	{
		CDlgBagPresent BagPresent;
		BagPresent.SetProperty(pBackpackProperty);
		BagPresent.DoModal();
		m_bIsGiveProperty = BagPresent.IsGiveProperty();
		m_ePresendMode = BagPresent.GetPresendMode();
		if( m_ePresendMode == PRESEND_NICKNAME )
		{
			m_sGiveReceiveName = BagPresent.GetEditReceiveName();
			m_dwGiveReceiveID = 0;
		}
		else if( m_ePresendMode == PRESEND_GAMEID )
		{
			CStdString EditReceiveName = BagPresent.GetEditReceiveName();
			m_dwGiveReceiveID = _ttoi(EditReceiveName.GetData());
			m_sGiveReceiveName = TEXT("");
		}
		m_curSelectProp.nCount = BagPresent.GetUseCount();
		return IDOK;
	}
	return IDCANCEL;
}

void CDlgBagMain::ShowSuccessMessage(const CMD_GP_S_PropertyUse* pPropertyUse)
{
	if(pPropertyUse == NULL) return;
	CPlatformFrame * pPlatformFrame =  CPlatformFrame::GetInstance();
	if( pPlatformFrame->IsEnteredServerItem() ) return;
	
	CStdString tMessage = pPropertyUse->szNotifyContent;
	if( pPropertyUse->dwPropKind == PROP_KIND_GEM )
	{
		tMessage.Format(TEXT("%s道具使用成功！恭喜您获得%d游戏币！"), pPropertyUse->szName, pPropertyUse->lUseResultsGold*pPropertyUse->wPropCount);
	}
	
	CDlgInformation Information(NULL);
	Information.ShowMessageBox(tMessage.GetData(), MB_ICONINFORMATION);
}

//减少道具
bool CDlgBagMain::ReduceProperty(const DWORD dwPropID, const DWORD dwPropCount)
{
	vector<tagBackpackProperty>::iterator itr = m_vBackPack.begin();
	for(; itr != m_vBackPack.end(); ++itr)
	{
		if(itr->Property.wIndex == dwPropID){
			itr->nCount -= dwPropCount;
			UpdateProperty(*itr);
			/*if(itr->nCount <= 0) m_vBackPack.erase(itr);*/
			return true;
		}
	}
	return false;
}


//////////////////////////////////////////////////////////////////////////////////
CDlgBagPlaza::CDlgBagPlaza():m_bQueryBackpack(false), m_bUseBackpackProp(false), m_bGiveBackpackProp(false),m_bGetSendPresent(false)
{
	m_MissionManager.InsertMissionItem(this);
}

CDlgBagPlaza::~CDlgBagPlaza()
{

}

BOOL CDlgBagPlaza::OnInitDialog()
{
	CDlgBagMain::OnInitDialog();
	//发起连接

	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();
	
	if(pGlobalUserData->dwPresentCount > 0)
	{
		m_bGetSendPresent = true;
		m_bQueryBackpack = false;
		m_bUseBackpackProp = false;
	}
	else
	{
		m_bQueryBackpack = true;
		m_bGetSendPresent = false;
		m_bUseBackpackProp = false;
	}
	AvtiveMission();
	return TRUE;
}

//连接事件
bool CDlgBagPlaza::OnEventMissionLink(INT nErrorCode)
{
	//连接结果
	if (nErrorCode!=0)
	{
		//重试任务
		if (m_MissionManager.AvtiveMissionItem(this,true)==true)
		{
			return true;
		}

		//提示消息
		CDlgInformation Information(NULL);
		Information.ShowMessageBox(TEXT("连接服务器超时，远程查找操作失败！"),MB_OK|MB_ICONSTOP,30);

		return true;
	}


	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	//获取赠送
	if(m_bGetSendPresent)
	{
		CMD_GP_C_GetSendPresent GetSendPresent;
		GetSendPresent.dwUserID = pGlobalUserData->dwUserID;
		wcscpy(GetSendPresent.szPassword, pGlobalUserData->szPassword);
		m_MissionManager.SendData(MDM_GP_PROPERTY, SUB_GP_GET_SEND_PRESENT, &GetSendPresent, sizeof(GetSendPresent));
		m_bGetSendPresent = false;

	}
	else if(m_bQueryBackpack)
	{
		//获取背包信息
		CMD_GP_C_BackpackProperty BackpackProperty;
		BackpackProperty.dwUserID = pGlobalUserData->dwUserID;
		BackpackProperty.dwKind = PROP_KIND_ALL;
		m_MissionManager.SendData(MDM_GP_PROPERTY,SUB_GP_QUERY_BACKPACKET, &BackpackProperty, sizeof(BackpackProperty));
		m_bQueryBackpack = false;
	}
	else if(m_bUseBackpackProp)
	{
		//使用背包物品
		CMD_GP_C_PropertyUse PropertyUse;
		PropertyUse.dwUserID = pGlobalUserData->dwUserID;
		PropertyUse.dwRecvUserID = pGlobalUserData->dwUserID;
		PropertyUse.dwPropID = m_curSelectProp.Property.wIndex;
		PropertyUse.wPropCount = m_curSelectProp.nCount;
		m_MissionManager.SendData(MDM_GP_PROPERTY,SUB_GP_PROPERTY_USE, &PropertyUse, sizeof(PropertyUse));
		m_bUseBackpackProp = false;
	}
	else if( m_bGiveBackpackProp )
	{
		//赠送物品
		CMD_GP_C_PropertyPresent PropertyPresent={0};
		PropertyPresent.dwUserID = pGlobalUserData->dwUserID;
		if(m_ePresendMode == PRESEND_NICKNAME)
		{
			wcscpy(PropertyPresent.szRecvNickName, m_sGiveReceiveName.GetData());
			PropertyPresent.wType = 0;
		}
		else if(m_ePresendMode == PRESEND_GAMEID)
		{
			PropertyPresent.dwRecvGameID = m_dwGiveReceiveID;
			PropertyPresent.wType = 1;
		}
		PropertyPresent.dwPropID = m_curSelectProp.Property.wIndex;
		PropertyPresent.wPropCount = m_curSelectProp.nCount;

		m_MissionManager.SendData(MDM_GP_PROPERTY, SUB_GP_PROPERTY_PRESENT, &PropertyPresent, sizeof(PropertyPresent));
		m_bGiveBackpackProp = false;
	}
	return true;
}

//关闭事件
bool CDlgBagPlaza::OnEventMissionShut(BYTE cbShutReason)
{
	//提示消息
	if (cbShutReason!=SHUT_REASON_NORMAL)
	{
		//重试任务
		if (m_MissionManager.AvtiveMissionItem(this,true)==true)
		{
			return true;
		}

		//提示消息
		CDlgInformation Information(NULL);
		Information.ShowMessageBox(TEXT("与服务器的连接异常断开了，远程查找操作失败！"),MB_ICONERROR,30L);
	}
	return true;
}

//读取事件
bool CDlgBagPlaza::OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize)
{
	if( Command.wMainCmdID==MDM_GP_PROPERTY )
	{
		switch (Command.wSubCmdID)
		{
		case SUB_GP_GET_SEND_PRESENT_RESULT:	//获取赠送
			{
				CMD_GP_S_GetSendPresent* pGetSendPresentResult = (CMD_GP_S_GetSendPresent*)pData;
				
				vector<SendPresent> vecSendPresent;
				for(int i = 0; i < pGetSendPresentResult->wPresentCount; i++)
				{
					vecSendPresent.push_back(pGetSendPresentResult->Present[i]);
				}

				CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
				tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();
				pGlobalUserData->dwPresentCount = 0;

				m_bQueryBackpack = true;
				AvtiveMission();
				
				return true;
			}
		case SUB_GP_QUERY_BACKPACKET_RESULT:	//背包道具
			{
				CMD_GP_S_BackpackProperty* pBackpackProperty = (CMD_GP_S_BackpackProperty*)pData;
				vector<tagBackpackProperty> vBackPack;
				int nCount = (int) pBackpackProperty->dwCount;
				for(int i = 0; i < nCount; i++)
				{
					vBackPack.push_back(pBackpackProperty->PropertyInfo[i]);
				}
				//发送结束标志
				if(pBackpackProperty->dwStatus == 1)
				{
					Clear();
					std::copy(vBackPack.begin(), vBackPack.end(), std::back_inserter(m_vBackPack)); 
					LoadDataFromMemory();
				}
				return true;
			}
		case SUB_GP_PROPERTY_USE_RESULT:		//使用背包道具
			{
				CMD_GP_S_PropertyUse* pPropertyUse = (CMD_GP_S_PropertyUse*)pData;
				CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
				tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();
				if(pPropertyUse->dwUserID != pGlobalUserData->dwUserID)
				{
					//提示消息
					CDlgInformation Information(NULL);
					Information.ShowMessageBox(TEXT("道具使用者信息错误！"),MB_OK|MB_ICONSTOP,30);
					return false;
				}
				if(pPropertyUse->dwHandleCode != 0)
				{
					//提示消息
					CDlgInformation Information(NULL);
					Information.ShowMessageBox(pPropertyUse->szNotifyContent,MB_OK|MB_ICONSTOP,30);
					return false;
				}
				
				//减少道具
				ReduceProperty(pPropertyUse->dwPropID, pPropertyUse->wPropCount);

				//设置变量
				pGlobalUserData->lUserScore=pPropertyUse->Score;
				pGlobalUserData->cbMemberOrder=pPropertyUse->cbMemberOrder;			

				//发送事件
				CPlatformEvent * pPlatformEvent=CPlatformEvent::GetInstance();
				if (pPlatformEvent!=NULL) pPlatformEvent->SendPlatformEvent(EVENT_USER_INFO_UPDATE,0L);
				
				//更新BUFF
				tagPropertyBuff Buff={0};
				Buff.dwKind = pPropertyUse->dwPropKind;
				Buff.tUseTime = pPropertyUse->tUseTime;
				Buff.UseResultsValidTime = pPropertyUse->UseResultsValidTime;
				Buff.dwScoreMultiple = pPropertyUse->dwScoreMultiple;
				//发送事件
				if (pPlatformEvent!=NULL) pPlatformEvent->SendPlatformEvent(EVENT_USER_SHOW_UPDATE_BUFF,(LPARAM)&Buff);
				
				//显示消息
				ShowSuccessMessage(pPropertyUse);
				return true;
			}
		case SUB_GP_PROPERTY_PRESENT_RESULT:
			{
				ASSERT(wDataSize == sizeof(CMD_GP_S_PropertyPresent));
				CMD_GP_S_PropertyPresent* pPropertyPresentRes = (CMD_GP_S_PropertyPresent*)pData;
				CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
				tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();
				
				//成功
				if(pPropertyPresentRes->nHandleCode == 0)
				{
					//减少道具
					ReduceProperty(pPropertyPresentRes->dwPropID, pPropertyPresentRes->wPropCount);
				}

				CDlgInformation Information(NULL);
				Information.ShowMessageBox(pPropertyPresentRes->szNotifyContent, (UINT)MB_OK,30);
				
				return true;
			}
		}
	}
	return false;
}

//显示使用道具
INT_PTR CDlgBagPlaza::ShowUseProperty(const tagBackpackProperty* pBackpackProperty)
{
	CDlgBagMain::ShowUseProperty(pBackpackProperty);

	//使用道具
	if( IsUseProperty() )
	{
		if( m_curSelectProp.Property.wKind == PROP_KIND_GIFT )
		{
			//提示消息
			CDlgInformation Information(NULL);
			Information.ShowMessageBox(TEXT("背包内不可以使用礼物道具，请到房间玩家列表菜单中选中使用！"),MB_ICONERROR,30L);
			return IDCANCEL;
		}
		
		//激活任务
		m_bQueryBackpack = false;
		m_bGiveBackpackProp = false;
		m_bUseBackpackProp = true;
		m_MissionManager.AvtiveMissionItem(this,false);
		return IDOK;
	}
	//赠送道具
	else if( IsGiveProperty() )
	{
		//获取变量
		CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
		tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();
		
		//赠送判断
		if(pGlobalUserData->dwGameID == m_dwGiveReceiveID || m_sGiveReceiveName == pGlobalUserData->szNickName)
		{
			CDlgInformation Information(NULL);
			Information.ShowMessageBox(TEXT("不能把物品赠送给自己！"),MB_ICONERROR,30L);
			return IDCANCEL;
		}
		//激活任务
		m_bQueryBackpack = false;
		m_bUseBackpackProp = false;
		m_bGiveBackpackProp = true;
		m_MissionManager.AvtiveMissionItem(this,false);
		return IDOK;
	}
	return IDCANCEL;
}


bool CDlgBagPlaza::AvtiveMission()
{
	if (m_MissionManager.AvtiveMissionItem(this,false)==true) 
	{
		//设置时间
	}
	return true;
}

//查询赠送
bool CDlgBagPlaza::QuerySendPresentMisson()
{
	//变量赋值
	m_bGetSendPresent = true;
	m_bQueryBackpack = false;
	m_bUseBackpackProp = false;

	//执行激活
	AvtiveMission();
	return true;
}

////////////////////////////////////////////////////////////////////////////////////////

CDlgBagServer::CDlgBagServer()
{

}

CDlgBagServer::~CDlgBagServer()
{

}

BOOL CDlgBagServer::OnInitDialog()
{
	CDlgBagMain::OnInitDialog();

	//修改样式
	ModifyStyle(0, WS_MINIMIZEBOX|WS_MAXIMIZEBOX);

	//设置标题
	SetWindowText(TEXT("我的背包"));

	//居中窗口
	CenterWindow(this);

	//下移距离
	CRect rc;
	GetWindowRect(&rc);
	rc.top += 30;
	rc.bottom += 30;
	//移动窗口
	MoveWindow(rc);

	//获取变量
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	//查询赠送
	if(pGlobalUserData->dwPresentCount > 0)
	{
		PerformQuerySendPresent();
	}
	//查询背包
	else
	{
		PerformQueryInfo();
	}

	return TRUE;
}

//查询背包
VOID CDlgBagServer::PerformQueryInfo()
{
	ASSERT( m_pIMySelfUserItem != NULL );
	//变量定义
	CMD_GR_C_BackpackProperty  BackpackProperty={0};
	if(m_pIMySelfUserItem != NULL)
	{
		BackpackProperty.dwUserID = m_pIMySelfUserItem->GetUserID();
	}
	//发送消息
	m_pITCPSocket->SendData(MDM_GR_PROPERTY, SUB_GR_PROPERTY_BACKPACK, &BackpackProperty, sizeof(BackpackProperty));
}

VOID CDlgBagServer::PerformUseProperty()
{
	
}

VOID CDlgBagServer::PerformQuerySendPresent()
{
	//变量定义
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	CMD_GR_C_GetSendPresent GetSendPresent;
	GetSendPresent.dwUserID = pGlobalUserData->dwUserID;
	wcscpy(GetSendPresent.szPassword, pGlobalUserData->szPassword);

	//发送消息
	m_pITCPSocket->SendData(MDM_GR_PROPERTY, SUB_GR_GET_SEND_PRESENT, &GetSendPresent, sizeof(GetSendPresent));
}

VOID CDlgBagServer::Initialization(ITCPSocket * pITCPSocket, IClientUserItem * pIMySelfUserItem)
{
	//设置变量
	m_pITCPSocket=pITCPSocket;
	m_pIMySelfUserItem=pIMySelfUserItem;
}

//背包消息
bool CDlgBagServer::OnServerBagMessage(WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	//背包道具
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
	//使用道具
	else if(wSubCmdID == SUB_GR_PROPERTY_USE_RESULT)
	{
		CMD_GR_S_PropertyUse* pPropertyUse = (CMD_GR_S_PropertyUse*)pData;
		if(pPropertyUse->dwUserID != m_pIMySelfUserItem->GetUserID())
		{
			return false;
		}
		if(pPropertyUse->dwHandleCode != 0 )
		{
			return false;
		}

		CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
		tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();
		//设置变量
		pGlobalUserData->lUserScore=pPropertyUse->Score;
		pGlobalUserData->cbMemberOrder=pPropertyUse->cbMemberOrder;			

		//发送事件
		CPlatformEvent * pPlatformEvent=CPlatformEvent::GetInstance();
		if (pPlatformEvent!=NULL) pPlatformEvent->SendPlatformEvent(EVENT_USER_INFO_UPDATE,0L);


		//更新删除
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
				/*if(itr->nCount <= 0) m_vBackPack.erase(itr);*/
				break;
			}
		}
		return true;
	}
	//道具赠送
	else if(wSubCmdID == SUB_GR_PROPERTY_PRESENT_RESULT)
	{
		ASSERT(wDataSize == sizeof(CMD_GR_S_PropertyPresent));
		//变量定义
		CMD_GR_S_PropertyPresent* pPropertyPresentRes = (CMD_GR_S_PropertyPresent*)pData;
		CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
		tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

		//赠送成功
		if(pPropertyPresentRes->nHandleCode == 0)
		{
			//减少道具
			ReduceProperty(pPropertyPresentRes->dwPropID, pPropertyPresentRes->wPropCount);
		}
		
		//提示消息
		CDlgInformation Information(NULL);
		Information.ShowMessageBox(pPropertyPresentRes->szNotifyContent, (UINT)MB_OK,30);
		return true;
	}
	//获取赠送
	else if(wSubCmdID == SUB_GR_GET_SEND_PRESENT_RESULT)
	{
		CMD_GR_S_GetSendPresent* pGetSendPresentResult = (CMD_GR_S_GetSendPresent*)pData;

		//拷贝数据
		vector<SendPresent> vecSendPresent;
		for(int i = 0; i < pGetSendPresentResult->wPresentCount; i++)
		{
			vecSendPresent.push_back(pGetSendPresentResult->Present[i]);
		}
		
		//赋值变量
		CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
		tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();
		pGlobalUserData->dwPresentCount = 0;
		
		//查询背包
		PerformQueryInfo();
		return true;
	}
	return false;
}
INT_PTR CDlgBagServer::ShowUseProperty(const tagBackpackProperty* pBackpackProperty)
{
	CDlgBagMain::ShowUseProperty(pBackpackProperty);
	
	//使用道具
	if( IsUseProperty() )
	{
		//判断类型
		if( m_curSelectProp.Property.wKind == PROP_KIND_GIFT )
		{
			//提示消息
			CDlgInformation Information(NULL);
			Information.ShowMessageBox(TEXT("背包内不可以使用礼物道具，请到房间玩家列表菜单中选中使用！"),MB_ICONERROR,30L);
			return IDCANCEL;
		}
		
		//变量定义
		m_bQueryBackpack = false;
		m_bUseBackpackProp = true;
		CMD_GR_C_PropertyUse PropertyUse;
		PropertyUse.dwUserID = m_pIMySelfUserItem->GetUserID();
		PropertyUse.dwRecvUserID = m_pIMySelfUserItem->GetUserID();
		PropertyUse.dwPropID = m_curSelectProp.Property.wIndex;
		PropertyUse.wPropCount = m_curSelectProp.nCount;

		//发送消息
		m_pITCPSocket->SendData(MDM_GR_PROPERTY, SUB_GR_PROPERTY_USE, &PropertyUse, sizeof(PropertyUse));
		m_bUseBackpackProp = false;
		return IDOK;
	}
	//赠送道具
	else if( IsGiveProperty() )
	{
		//变量定义
		CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
		tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

		//判读赠送
		if(pGlobalUserData->dwGameID == m_dwGiveReceiveID || m_sGiveReceiveName == pGlobalUserData->szNickName)
		{
			CDlgInformation Information(NULL);
			Information.ShowMessageBox(TEXT("不能把物品赠送给自己！"),MB_ICONERROR,30L);
			return IDCANCEL;
		}

		//赠送物品
		CMD_GR_C_PropertyPresent PropertyPresent={0};
		PropertyPresent.dwUserID = pGlobalUserData->dwUserID;

		//类型判读
		if(m_ePresendMode == PRESEND_NICKNAME)
		{
			wcscpy(PropertyPresent.szRecvNickName, m_sGiveReceiveName.GetData());
			PropertyPresent.wType = 0;
		}
		else if(m_ePresendMode == PRESEND_GAMEID)
		{
			PropertyPresent.dwRecvGameID = m_dwGiveReceiveID;
			PropertyPresent.wType = 1;
		}
		//道具赋值
		PropertyPresent.dwPropID = m_curSelectProp.Property.wIndex;
		PropertyPresent.wPropCount = m_curSelectProp.nCount;
		
		//发送消息
		m_pITCPSocket->SendData(MDM_GR_PROPERTY, SUB_GR_PROPERTY_PRESENT, &PropertyPresent, sizeof(PropertyPresent));

		return IDOK;
	}
	return IDCANCEL;
}

bool CDlgBagServer::QuerySendPresentMisson()
{

	return true;
}