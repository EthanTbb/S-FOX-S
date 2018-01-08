#include "Stdafx.h"
#include "Resource.h"
#include "DlgShop.h"
#include "PlatformEvent.h"
#include "DlgShopDetails.h"
#include "PlatformFrame.h"
#include "PlatformEvent.h"
#include "DlgInformation.h"
#include "DlgRechargeEnquire.h"
//////////////////////////////////////////////////////////////////////////////////

//容器控件
const TCHAR* const szHLayoutOptionShopControlName = TEXT("HLayoutOptionShop");
const TCHAR* const szTabLayoutShopControlName = TEXT("TabLayoutShop");

//按钮控件
const TCHAR* const szButtonCloseControlName = TEXT("ButtonClose");
const TCHAR* const szButtonButtonHelpControlName = TEXT("ButtonButtonHelp");

//道具控件
const TCHAR* const szPageLayoutAllProperty = TEXT("PageLayoutAllProperty");
const TCHAR* const szPageLayoutCashProperty = TEXT("PageLayoutCashProperty");
const TCHAR* const szPageLayoutGoldProperty = TEXT("PageLayoutGoldProperty");
const TCHAR* const szPageLayoutLoveLinessProperty = TEXT("PageLayoutLoveLinessProperty");
const TCHAR* const szPageLayoutUserMedalProperty = TEXT("PageLayoutUserMedalProperty");

//道具属性控件
const TCHAR* const szLableItemShopIOC = TEXT("ItemShopIOC");
const TCHAR* const szLableItemShopName = TEXT("ItemShopName");
const TCHAR* const szLableItemShopPrice = TEXT("ItemShopPrice");

//VIP控件
const TCHAR* const szLableItemShopVip = TEXT("ItemShopVip");
const TCHAR* const szItemContainerVipControlName = TEXT("ItemContainerVip");
const TCHAR* const szLabelVipLevelControlName = TEXT("LabelVipLevel");
//按钮控件
const TCHAR* const szButtonMyBag = TEXT("ButtonMyBag");
const TCHAR* const szButtonRecharge = TEXT("ButtonRecharge");
const TCHAR* const szButtonMyInsure = TEXT("ButtonMyInsure");
//文本控件
const TCHAR* const szTextShopLink = TEXT("TextShopLink");

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CDlgShopMain::CDlgShopMain() : CFGuiDialog(IDD_DLG_POPUP)
{
	m_pDlgShopDetails = NULL;
	//设置任务
	m_bQueryProperty = false;
	m_bBuyProperty = false;
	m_bUseProperty = false;
	ZeroMemory(&m_PropertyUseInfo, sizeof(m_PropertyUseInfo));
	
}

//析构函数
CDlgShopMain::~CDlgShopMain()
{
}

void CDlgShopMain::EndDialog()
{
	if(m_pDlgShopDetails!=NULL) m_pDlgShopDetails->EndDialog(IDOK);
	__super::EndDialog(IDOK);
}

//创建函数
BOOL CDlgShopMain::OnInitDialog()
{
	//修改样式
	ModifyStyle(0, WS_MINIMIZEBOX|WS_MAXIMIZEBOX);

	//设置标题
	SetWindowText(TEXT("商城中心"));

	//居中窗口
	/*CenterWindow(this);*/

	LoadDataFromMemory();

	CreateUI();


	return TRUE;
}

//确定函数
VOID CDlgShopMain::OnOK()
{
	//return __super::OnOK();
}

//取消消息
VOID CDlgShopMain::OnCancel()
{
	return __super::OnCancel();
}


//消息提醒
void CDlgShopMain::Notify(TNotifyUI &  msg)
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

		//背包按钮
		if(lstrcmp(pControlUI->GetName(), szButtonMyBag)==0)
		{
			CPlatformEvent * pPlatformEvent=CPlatformEvent::GetInstance();
			if (pPlatformEvent!=NULL) pPlatformEvent->PostPlatformEvent(EVENT_USER_SHOW_BAG,0L);

			OnCancel();
		}

		//银行按钮
		if(lstrcmp(pControlUI->GetName(), szButtonMyInsure)==0)
		{
			//发送事件
			CPlatformEvent * pPlatformEvent=CPlatformEvent::GetInstance();
			if (pPlatformEvent!=NULL) pPlatformEvent->PostPlatformEvent(EVENT_USER_SHOW_MY_INSURE,0L);

			OnCancel();
		}

		//兑换按钮
		if(lstrcmp(pControlUI->GetName(), szButtonRecharge)==0)
		{
			CPlatformEvent * pPlatformEvent=CPlatformEvent::GetInstance();
			if (pPlatformEvent!=NULL) pPlatformEvent->PostPlatformEvent(EVENT_USER_SHOW_RECHARGE,0L);

			OnCancel();
		}

		//道具响应
		if(lstrcmp(pControlUI->GetName(), TEXT("ItemGameSelect"))==0)
		{
			CDlgShopDetails dlgShopDetails;
			m_pDlgShopDetails = &dlgShopDetails;
			/*dlgShopDetails.SetPropertyInfo((tagPropertyInfo *)pControlUI->GetTag());*/

			dlgShopDetails.LoadDataFromExternal(m_CurrGamePropertyType,(CGamePropertyItem*)pControlUI->GetTag());
			dlgShopDetails.SetShopMainSink((IGamePropertyAction*)this);
			CTabLayoutUI* pControl = static_cast<CTabLayoutUI*>(GetControlByName(TEXT("TabLayoutShop")));
			if(pControl)
			{
				dlgShopDetails.SetDefaultBuy(pControl->GetCurSel());
			}
			dlgShopDetails.DoModal();
			m_pDlgShopDetails = NULL;
			return;
		}
		
	}
	else if(msg.sType == TEXT("tabselect"))
	{
		if( pControlUI->GetName() == TEXT("TabLayoutShop") )
		{
			m_CurrGamePropertyType = (CGamePropertyTypeItem *)((CTabLayoutUI*)pControlUI)->GetItemAt(((CTabLayoutUI*)pControlUI)->GetCurSel())->GetTag();
			CTextUI* pTextShopLink = static_cast<CTextUI*>(GetControlByName(szTextShopLink));
			if(pTextShopLink != NULL)
			{
				if(msg.wParam >= 0 && msg.wParam < 5 )
				{
					TCHAR szLinkName[][16]={TEXT("游戏豆"), TEXT("游戏豆"), TEXT("游戏币"), TEXT("魅力值"), TEXT("元宝")};
					CStdString sLinkText;
					sLinkText.Format(TEXT("<a name=\'GetBeanLink\' linkcolor=\'#FF25A5FF\' linkhovercolor=\'#FF0585CF\'>如何获取%s？</a>"), szLinkName[msg.wParam]);
					pTextShopLink->SetText(sLinkText);
				}
			}
		}
	}
	else if( msg.sType == TEXT("link") )
	{
		CStdString sLinkName = (LPCTSTR)msg.lParam;
		if( sLinkName == TEXT("GetBeanLink") )
		{
			//获取对象
			ASSERT(CGlobalWebLink::GetInstance()!=NULL);
			CGlobalWebLink * pGlobalWebLink=CGlobalWebLink::GetInstance();

			//构造地址
			TCHAR szLinkInfo[256]=TEXT("");
			_sntprintf(szLinkInfo,CountArray(szLinkInfo),TEXT("%s/%sUserService/UserService4.aspx"),pGlobalWebLink->GetPlatformLink(),pGlobalWebLink->GetValidateLink());
			//打开页面
			ShellExecute(NULL,TEXT("OPEN"),szLinkInfo,NULL,NULL,SW_NORMAL);
		}
	}
	
	return;
}

LPCTSTR CDlgShopMain::GetSkinFile()
{
	return TEXT("DlgShop.xml"); 
}

VOID CDlgShopMain::InitData()
{

}

VOID CDlgShopMain::LoadDataFromMemory()
{
	//获取对象
	ASSERT(CGamePropertyListData::GetInstance()!=NULL);
	CGamePropertyListData * pGamePropertyListData=CGamePropertyListData::GetInstance();

	m_vGamePropertyTypeList.clear();
	m_vGamePropertyRelatList.clear();
	m_vGamePropertyList.clear();

	//变量定义
	POSITION Position=NULL;
	CGamePropertyTypeItem    * pGamePropertyTypeItem=NULL;
	CGamePropertyRelatItem   * pGamePropertyRelatItem=NULL;
	CGamePropertyItem		 * pGamePropertyItem=NULL;
	vector<CGamePropertyTypeItem *>::iterator itGamePropertyType;
	vector<CGamePropertyRelatItem *>::iterator itGamePropertyRelat;
	vector<CGamePropertyItem *>::iterator itGameProperty;

	//加载类型
	while (true)
	{
		pGamePropertyTypeItem=pGamePropertyListData->EmunGamePropertyTypeItem(Position);
		if (pGamePropertyTypeItem==NULL) break;
		for (itGamePropertyType = m_vGamePropertyTypeList.begin();itGamePropertyType!=m_vGamePropertyTypeList.end();itGamePropertyType++)
		{
			CGamePropertyTypeItem * pGamePropertyTypeTemp= (CGamePropertyTypeItem *)(*itGamePropertyType);
			if (pGamePropertyTypeItem->m_PropertyTypeItem.dwSortID <=pGamePropertyTypeTemp->m_PropertyTypeItem.dwSortID)
			{
				m_vGamePropertyTypeList.insert(itGamePropertyType,pGamePropertyTypeItem);
				break;
			}
		}
		if (itGamePropertyType==m_vGamePropertyTypeList.end()) m_vGamePropertyTypeList.push_back(pGamePropertyTypeItem);
		if (Position==NULL) break;
	}

	//加载游戏
	while (true)
	{
		pGamePropertyRelatItem=pGamePropertyListData->EmunGamePropertyRelatItem(Position);
		if (pGamePropertyRelatItem==NULL) break;
		m_vGamePropertyRelatList.push_back(pGamePropertyRelatItem);
		if (Position==NULL) break;
	}

	//加载游戏
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

}

void CDlgShopMain::CreateUI()
{
	//创建选项
	CContainerUI * pHLayoutOption = (CContainerUI * )(GetControlByName(szHLayoutOptionShopControlName));
	if (pHLayoutOption != NULL)
	{
		//创建推荐
		CDialogBuilder Allbuilder;
		COptionUI * pAllOption = NULL;
		pAllOption = static_cast<COptionUI*>(Allbuilder.Create( _T("DlgShopItemOption.xml"), (UINT)0));
		pHLayoutOption->Add(pAllOption);

		//创建类别
		int nTabIndex = 1;
		for (vector<CGamePropertyTypeItem *>::iterator iter=m_vGamePropertyTypeList.begin();iter!=m_vGamePropertyTypeList.end();iter++)
		{
			CGamePropertyTypeItem * pGamePropertyTypeItem=(*iter);
			CDialogBuilder builder;
			COptionUI * pNewOption = NULL;
			pNewOption = static_cast<COptionUI*>(builder.Create( _T("DlgShopItemOption.xml"), (UINT)0));
			pNewOption->SetText(pGamePropertyTypeItem->m_PropertyTypeItem.szTypeName);
			pNewOption->Selected(false);
			pNewOption->BindTabIndex(nTabIndex++);
			pHLayoutOption->Add(pNewOption);
		}
	}
	//创建页面
	CTabLayoutUI * pTabLayout = (CTabLayoutUI * )(GetControlByName(szTabLayoutShopControlName));
	if (pTabLayout != NULL)
	{
		//创建推荐
		CDialogBuilder Allbuilder;
		CContainerUI *pAllContainer = NULL;
		pAllContainer = static_cast<CContainerUI*>(Allbuilder.Create( _T("DlgShopItemPage.xml"), (UINT)0, NULL, &m_PaintManager));
		pTabLayout->Add(pAllContainer);

		//创建类别
		vector<CContainerUI *> vTabLayoutContainer;
		for (vector<CGamePropertyTypeItem *>::iterator iter=m_vGamePropertyTypeList.begin();iter!=m_vGamePropertyTypeList.end();iter++)
		{
			CGamePropertyTypeItem * pGamePropertyTypeItem=(*iter);
			CDialogBuilder builder;
			CContainerUI *pNewContainer = NULL;
			pNewContainer = static_cast<CContainerUI*>(builder.Create( _T("DlgShopItemPage.xml"), (UINT)0, NULL, &m_PaintManager));
			pNewContainer->SetTag((UINT_PTR)pGamePropertyTypeItem);
			pTabLayout->Add(pNewContainer);
			vTabLayoutContainer.push_back(pNewContainer);
		}

		//创建图标
		for (vector<CContainerUI *>::iterator iter=vTabLayoutContainer.begin();iter!=vTabLayoutContainer.end();iter++)
		{
			CContainerUI *pContainerUI = (*iter);
			CGamePropertyTypeItem * pGamePropertyTypeItem=(CGamePropertyTypeItem *)(pContainerUI->GetTag());
			for (vector<CGamePropertyRelatItem*>::iterator iter2 = m_vGamePropertyRelatList.begin();iter2!=m_vGamePropertyRelatList.end();iter2++)
			{
				vector<CGamePropertyItem*> vCurrGamePropertyList;
				vCurrGamePropertyList.clear();
				CGamePropertyRelatItem * pGamePropertyRelatItem = (CGamePropertyRelatItem *)(*iter2);
				if(pGamePropertyRelatItem->m_PropertyRelatItem.dwTypeID == pGamePropertyTypeItem->m_PropertyTypeItem.dwTypeID)
				{
					//排序道具
					for (vector<CGamePropertyItem*>::iterator iter3 = m_vGamePropertyList.begin();iter3!=m_vGamePropertyList.end();iter3++)
					{
						CGamePropertyItem * pGamePropertyItem = (CGamePropertyItem *)(*iter3);
						if (pGamePropertyItem->m_PropertyItem.dwPropertyID == pGamePropertyRelatItem->m_PropertyRelatItem.dwPropertyID)
						{
							vector<CGamePropertyItem *>::iterator itGameProperty;
							for (itGameProperty = vCurrGamePropertyList.begin();itGameProperty!=vCurrGamePropertyList.end();itGameProperty++)
							{
								CGamePropertyItem * pGamePropertyTemp= (CGamePropertyItem *)(*itGameProperty);
								if (pGamePropertyItem->m_PropertyItem.dwSortID <=pGamePropertyTemp->m_PropertyItem.dwSortID)
								{
									vCurrGamePropertyList.insert(itGameProperty,pGamePropertyItem);
									break;
								}
							}
							if (itGameProperty==vCurrGamePropertyList.end()) vCurrGamePropertyList.push_back(pGamePropertyItem);
						}
					}

					//创建图标
					for (vector<CGamePropertyItem*>::iterator iter3 = vCurrGamePropertyList.begin();iter3!=vCurrGamePropertyList.end();iter3++)
					{
						CGamePropertyItem * pGamePropertyItem = (CGamePropertyItem *)(*iter3);
						CDialogBuilder builder2;
						CContainerUI *pNewGameICOItem = NULL;
						pNewGameICOItem = static_cast<CContainerUI*>(builder2.Create( _T("DlgShopItemPageItem.xml"), (UINT)0, NULL, &m_PaintManager));

						//设置图标
						CStdString	csImageKind;
						csImageKind.Format(TEXT("file=\'GMAE_PROPERTY\\GMAE_PROPERTY_%d.png\' restype=\'FILE\'"),pGamePropertyItem->m_PropertyItem.dwPropertyID);
						if(m_PaintManager.GetImageEx(csImageKind)==NULL) csImageKind.Empty();
						if(csImageKind.IsEmpty()==true) 
						{
							csImageKind=TEXT("file=\'GMAE_PROPERTY\\GMAE_PROPERTY_NULL.png\' restype=\'FILE\'");
						}
						CControlUI *pIco = pNewGameICOItem->FindControl(szLableItemShopIOC);
						if (pIco!=NULL)
						{
							pIco->SetBkImage(csImageKind);
						}

						//设置名字
						CControlUI *pName = pNewGameICOItem->FindControl(szLableItemShopName);
						if (pName!=NULL)
						{
							pName->SetText(pGamePropertyItem->m_PropertyItem.szName);
						}

						//设置价格
						TCHAR szPrice[64]={0};	
						if (pGamePropertyItem->m_PropertyItem.dPropertyCash>0 && pGamePropertyTypeItem->m_PropertyTypeItem.dwTypeID ==1)
						{
							_sntprintf(szPrice,CountArray(szPrice), TEXT("%0.2f游戏豆"), pGamePropertyItem->m_PropertyItem.dPropertyCash);
						}
						else if (pGamePropertyItem->m_PropertyItem.lPropertyGold && pGamePropertyTypeItem->m_PropertyTypeItem.dwTypeID ==2)
						{
							_sntprintf(szPrice,CountArray(szPrice), TEXT("%I64d游戏币"), pGamePropertyItem->m_PropertyItem.lPropertyGold);
						}
						else if (pGamePropertyItem->m_PropertyItem.lPropertyLoveLiness && pGamePropertyTypeItem->m_PropertyTypeItem.dwTypeID ==3)
						{
							_sntprintf(szPrice,CountArray(szPrice), TEXT("%I64d魅力值"), pGamePropertyItem->m_PropertyItem.lPropertyLoveLiness);
						}
						else if (pGamePropertyItem->m_PropertyItem.lPropertyUserMedal && pGamePropertyTypeItem->m_PropertyTypeItem.dwTypeID ==4)
						{
							_sntprintf(szPrice,CountArray(szPrice), TEXT("%I64d元宝"), pGamePropertyItem->m_PropertyItem.lPropertyUserMedal);

						}
						CControlUI *Price = pNewGameICOItem->FindControl(szLableItemShopPrice);
						if(Price != NULL)
						{
							Price->SetText(szPrice);
						}
						//设置折扣
						CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
						tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();
						TCHAR szTaskRewardTotal[64]=TEXT("");
						if(pGlobalUserData->cbMemberOrder > 0)
						{

							//会员任务
							const tagMemberParameterNew* pMemberParameter = CUserItemElement::GetInstance()->GetUserMemberParameter(pGlobalUserData->cbMemberOrder);
							if(pMemberParameter != NULL)
							{
								_sntprintf(szTaskRewardTotal,CountArray(szTaskRewardTotal),TEXT("%d%%折扣"),pMemberParameter->dwMemberShop);
							}

							CControlUI * pContainerVip = pNewGameICOItem->FindControl(szItemContainerVipControlName);
							if(pContainerVip!=NULL)
							{
								//会员等级
								for (int i=0;i<6;i++)
								{
									CStdString strLevel;
									strLevel.SmallFormat(TEXT("%s%d"),szLabelVipLevelControlName,i);
									CControlUI * pLabelVipUI = pContainerVip->FindControl(strLevel);
									if(pLabelVipUI)
									{
										if (i==pGlobalUserData->cbMemberOrder)
										{
											pLabelVipUI->SetVisible(true);
										}
										else
										{
											pLabelVipUI->SetVisible(false);
										}
									}
								}

								CControlUI * pControl4 = pContainerVip->FindControl(szLableItemShopVip);
								if(pControl4 !=NULL) 
								{
									pControl4->SetVisible(true);
									pControl4->SetText(szTaskRewardTotal);
								}
							}

						}
						else
						{
							CControlUI * pContainerVip = pNewGameICOItem->FindControl(szItemContainerVipControlName);
							if(pContainerVip !=NULL)  pContainerVip->SetVisible(false);
						}

						//绑定数据
						CButtonUI *pButtonUI = (CButtonUI *)pNewGameICOItem->FindControl(	TEXT("ItemGameSelect"));
						if(pButtonUI != NULL)
						{
							pButtonUI->SetTag((UINT_PTR)pGamePropertyItem);
						}

						pContainerUI->Add(pNewGameICOItem);
					}
				
				}

			}
		}

		//创建推荐
		for (vector<CGamePropertyItem*>::iterator iter3 = m_vGamePropertyList.begin();iter3!=m_vGamePropertyList.end();iter3++)
		{
			CGamePropertyItem * pGamePropertyItem = (CGamePropertyItem *)(*iter3);
			if (pGamePropertyItem->m_PropertyItem.cbRecommend == 1)
			{
				CDialogBuilder builder2;
				CContainerUI *pNewGameICOItem = NULL;
				pNewGameICOItem = static_cast<CContainerUI*>(builder2.Create( _T("DlgShopItemPageItem.xml"), (UINT)0, NULL, &m_PaintManager));

				//设置图标
				CStdString	csImageKind;
				csImageKind.Format(TEXT("file=\'GMAE_PROPERTY\\GMAE_PROPERTY_%d.png\' restype=\'FILE\'"),pGamePropertyItem->m_PropertyItem.dwPropertyID);
				if(m_PaintManager.GetImageEx(csImageKind)==NULL) csImageKind.Empty();
				if(csImageKind.IsEmpty()==true) 
				{
					csImageKind=TEXT("file=\'GMAE_PROPERTY\\GMAE_PROPERTY_NULL.png\' restype=\'FILE\'");
				}
				CControlUI *pIco = pNewGameICOItem->FindControl(szLableItemShopIOC);
				if (pIco!=NULL)
				{
					pIco->SetBkImage(csImageKind);
				}

				//设置名字
				CControlUI *pName = pNewGameICOItem->FindControl(szLableItemShopName);
				if (pName!=NULL)
				{
					pName->SetText(pGamePropertyItem->m_PropertyItem.szName);
				}

				//设置价格
				TCHAR szPrice[64];	
				if (pGamePropertyItem->m_PropertyItem.dPropertyCash>0)
				{
					_sntprintf(szPrice,CountArray(szPrice), TEXT("%0.2f游戏豆"), pGamePropertyItem->m_PropertyItem.dPropertyCash);
				}
				else if (pGamePropertyItem->m_PropertyItem.lPropertyGold>0)
				{
					_sntprintf(szPrice,CountArray(szPrice), TEXT("%I64d游戏币"), pGamePropertyItem->m_PropertyItem.lPropertyGold);

				}
				else if (pGamePropertyItem->m_PropertyItem.lPropertyLoveLiness>0)
				{
					_sntprintf(szPrice,CountArray(szPrice), TEXT("%I64d魅力值"), pGamePropertyItem->m_PropertyItem.lPropertyLoveLiness);

				}
				else if (pGamePropertyItem->m_PropertyItem.lPropertyUserMedal =4)
				{
					_sntprintf(szPrice,CountArray(szPrice), TEXT("%I64d元宝"), pGamePropertyItem->m_PropertyItem.lPropertyUserMedal);
				}
				CControlUI *Price = pNewGameICOItem->FindControl(szLableItemShopPrice);
				if(Price != NULL)
				{
					Price->SetText(szPrice);
				}
				//设置折扣
				CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
				tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();
				TCHAR szTaskRewardTotal[64]=TEXT("");
				if(pGlobalUserData->cbMemberOrder > 0)
				{

					//会员任务
					const tagMemberParameterNew* pMemberParameter = CUserItemElement::GetInstance()->GetUserMemberParameter(pGlobalUserData->cbMemberOrder);
					if(pMemberParameter != NULL)
					{
						_sntprintf(szTaskRewardTotal,CountArray(szTaskRewardTotal),TEXT("%d%%折扣"),pMemberParameter->dwMemberShop);
					}

					CControlUI * pContainerVip = pNewGameICOItem->FindControl(szItemContainerVipControlName);
					if(pContainerVip!=NULL)
					{
						//会员等级
						for (int i=0;i<6;i++)
						{
							CStdString strLevel;
							strLevel.SmallFormat(TEXT("%s%d"),szLabelVipLevelControlName,i);
							CControlUI * pLabelVipUI = pContainerVip->FindControl(strLevel);
							if(pLabelVipUI)
							{
								if (i==pGlobalUserData->cbMemberOrder)
								{
									pLabelVipUI->SetVisible(true);
								}
								else
								{
									pLabelVipUI->SetVisible(false);
								}
							}
						}

						CControlUI * pControl4 = pContainerVip->FindControl(szLableItemShopVip);
						if(pControl4 !=NULL) 
						{
							pControl4->SetVisible(true);
							pControl4->SetText(szTaskRewardTotal);
						}
					}

				}
				else
				{
					CControlUI * pContainerVip = pNewGameICOItem->FindControl(szItemContainerVipControlName);
					if(pContainerVip !=NULL)  pContainerVip->SetVisible(false);
				}

				//绑定数据
				CButtonUI *pButtonUI = (CButtonUI *)pNewGameICOItem->FindControl(	TEXT("ItemGameSelect"));
				if(pButtonUI != NULL)
				{
					pButtonUI->SetTag((UINT_PTR)pGamePropertyItem);
				}

				pAllContainer->Add(pNewGameICOItem);
			}

		}


	}

}

VOID CDlgShopMain::LoadDataFromUI()
{
	return;
}

VOID CDlgShopMain::UpdateData()
{

}

VOID CDlgShopMain::InitUI()
{
	
}

VOID CDlgShopMain::UpdateUI()
{

}

//查询事件
VOID CDlgShopMain::PerformQueryInfo()
{
	return;
}

//查询事件
VOID CDlgShopMain::PerformBuyGameProperty(tagBuyGamePropertyInfo *ptagBuyGamePropertyInfo)
{
	return;
}

VOID CDlgShopMain::PerformUseGameProperty(const tagUseGamePropertyInfo *pPropertyUseInfo)
{
	return;
}

void CDlgShopMain::ShowSuccessMessage(const CMD_GP_S_PropertyUse* pPropertyUse)
{
	if(pPropertyUse == NULL) return;
	CPlatformFrame * pPlatformFrame =  CPlatformFrame::GetInstance();
	if( pPlatformFrame->IsEnteredServerItem() ) return;

	CStdString tMessage = TEXT("道具使用成功！");
	if( pPropertyUse->dwPropKind == PROP_KIND_GEM )
	{
		tMessage.Format(TEXT("%s道具使用成功！恭喜您获得%d游戏币！"), pPropertyUse->szName, pPropertyUse->lUseResultsGold);
	}

	CDlgInformation Information(NULL);
	Information.ShowMessageBox(tMessage.GetData(), MB_ICONINFORMATION);
}

//////////////////////////////////////////////////////////////////////

//构造函数
CDlgShopPlaza::CDlgShopPlaza()
{
	//设置任务
	m_MissionManager.InsertMissionItem(this);
	m_bQueryProperty = false;
	m_bBuyProperty = false;
}

//析构函数
CDlgShopPlaza::~CDlgShopPlaza()
{
}

//创建函数
BOOL CDlgShopPlaza::OnInitDialog()
{
	__super::OnInitDialog();

	//修改样式
	ModifyStyle(0, WS_MINIMIZEBOX|WS_MAXIMIZEBOX);

	//设置标题
	SetWindowText(TEXT("商城"));

	//居中窗口
	CenterWindow(this);

	//下移距离
	CRect rc;
	GetWindowRect(&rc);
	rc.top += 30;
	rc.bottom += 30;
	//移动窗口
	MoveWindow(rc);


	m_bQueryProperty = true;

	LoadDataFromUI();

	return TRUE;
}

//查询事件
VOID CDlgShopPlaza::PerformQueryInfo()
{
	m_bBuyProperty = false;
	m_bQueryProperty = true;

	//发起连接
	if (m_MissionManager.AvtiveMissionItem(this,false)==false)  
	{
		MessageBox(TEXT("连接失败"), TEXT(""), 0);
		return ;
	}

	return;
}

//查询事件
VOID CDlgShopPlaza::PerformBuyGameProperty(tagBuyGamePropertyInfo *ptagBuyGamePropertyInfo)
{
	m_bBuyProperty = true;
	m_bQueryProperty = false;

	CopyMemory(&m_tagBuyGamePropertyInfo,ptagBuyGamePropertyInfo,sizeof(tagBuyGamePropertyInfo));

	//发起连接
	if (m_MissionManager.AvtiveMissionItem(this,false)==false)  
	{
		MessageBox(TEXT("连接失败"), TEXT(""), 0);
		return ;
	}
	return;
}

//使用事件
VOID CDlgShopPlaza::PerformUseGameProperty(const tagUseGamePropertyInfo *pPropertyUseInfo)
{
	m_bBuyProperty = false;
	m_bQueryProperty = false;
	m_bUseProperty = true;
	
	CopyMemory(&m_PropertyUseInfo,pPropertyUseInfo,sizeof(tagUseGamePropertyInfo));

	//发起连接
	if (m_MissionManager.AvtiveMissionItem(this,false)==false)  
	{
		MessageBox(TEXT("连接失败"), TEXT(""), 0);
		return ;
	}
	return;
}

//连接事件
bool CDlgShopPlaza::OnEventMissionLink(INT nErrorCode)
{
	//连接结果
	if (nErrorCode!=0)
	{
		//重试任务
		if (m_MissionManager.AvtiveMissionItem(this,true)==true)
		{
			return true;
		}

		//错误提示
		CDlgInformation Information(NULL);
		Information.ShowMessageBox(TEXT("连接服务器超时，道具信息获取失败！"),MB_OK|MB_ICONSTOP,30);

		return true;
	}
	//发送数据
	if(m_bQueryProperty == true)
	{
		//获取对象
		CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
		tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

		m_MissionManager.SendData(MDM_GP_PROPERTY, SUB_GP_QUERY_PROPERTY);
	}
	else if(m_bBuyProperty == true)
	{
		//获取对象
		CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
		tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

		//定义变量
		CMD_GP_PropertyBuy PropertyBuy;
		PropertyBuy.dwUserID = pGlobalUserData->dwUserID;
		PropertyBuy.dwPropertyID = m_tagBuyGamePropertyInfo.dwPropertyID;
		PropertyBuy.dwItemCount = m_tagBuyGamePropertyInfo.dwBuyNumber;
		PropertyBuy.cbConsumeType = m_tagBuyGamePropertyInfo.cbScoreType;
		CWHService::GetMachineIDEx(PropertyBuy.szMachineID);
		lstrcpyn(PropertyBuy.szPassword,pGlobalUserData->szPassword,CountArray(PropertyBuy.szPassword));	
		
		m_MissionManager.SendData(MDM_GP_PROPERTY, SUB_GP_PROPERTY_BUY, &PropertyBuy, sizeof(PropertyBuy));

	}
	else if( m_bUseProperty == true )
	{
		//获取对象
		CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
		tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

		//使用背包物品
		CMD_GP_C_PropertyUse PropertyUse;
		PropertyUse.dwUserID = m_PropertyUseInfo.dwUserID;
		PropertyUse.dwRecvUserID = m_PropertyUseInfo.dwRecvUserID;
		PropertyUse.dwPropID = m_PropertyUseInfo.dwPropID;
		PropertyUse.wPropCount = m_PropertyUseInfo.wPropCount;
		m_MissionManager.SendData(MDM_GP_PROPERTY, SUB_GP_PROPERTY_USE, &PropertyUse, sizeof(PropertyUse));
	}

	return true;
}

//关闭事件
bool CDlgShopPlaza::OnEventMissionShut(BYTE cbShutReason)
{
	//提示消息
	if (cbShutReason!=SHUT_REASON_NORMAL)
	{
		//重试任务
		if (m_MissionManager.AvtiveMissionItem(this,true)==true)
		{
			return true;
		}

		//错误提示
		CDlgInformation Information(NULL);
		Information.ShowMessageBox(TEXT("连接服务器超时，道具信息获取失败！"),MB_OK|MB_ICONSTOP);
	}

	return true;
}

//读取事件
bool CDlgShopPlaza::OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize)
{
	//命令处理
	if (Command.wMainCmdID==MDM_GP_PROPERTY)
	{
		if(Command.wSubCmdID == SUB_GP_PROPERTY_FAILURE)
		{
			//效验参数
			ASSERT(wDataSize == sizeof(CMD_GP_PropertyFailure));
			if(wDataSize != sizeof(CMD_GP_PropertyFailure)) return false;

			//定义变量
			CMD_GP_PropertyFailure * pPropertyFailure=(CMD_GP_PropertyFailure *)pData;

			//终止任务
			m_MissionManager.ConcludeMissionItem(this,false);

			if (pPropertyFailure->lErrorCode == 6)
			{
				CDlgRechargeEnquire EnquireDlg;
				INT intRet = EnquireDlg.ShowMessageBox(pPropertyFailure->szDescribeString);
				if (intRet == IDOK)
				{
					EndDialog();
				}
				
			}
			else
			{
				CDlgInformation Information(this);
				Information.ShowMessageBox(pPropertyFailure->szDescribeString,MB_OK|MB_ICONERROR,30);
			}
			return 0;
		}
		else if(Command.wSubCmdID == SUB_GP_PROPERTY_BUY_RESULT)
		{
			//效验参数
			ASSERT(wDataSize == sizeof(CMD_GP_PropertyBuyResult));
			if(wDataSize != sizeof(CMD_GP_PropertyBuyResult)) return false;

			//定义变量
			CMD_GP_PropertyBuyResult * pPropertySuccess=(CMD_GP_PropertyBuyResult *)pData;

			//获取对象
			CGlobalUserInfo * pGlobalUserInfo = CGlobalUserInfo::GetInstance();
			tagGlobalUserData * pGlobalUserData = pGlobalUserInfo->GetGlobalUserData();
			pGlobalUserData->lUserInsure = pPropertySuccess->lInsureScore;
			pGlobalUserData->lUserIngot = pPropertySuccess->lUserMedal;
			pGlobalUserData->lLoveLiness = pPropertySuccess->lLoveLiness;
			pGlobalUserData->dUserBeans = pPropertySuccess->dCash;
			pGlobalUserData->cbMemberOrder = pPropertySuccess->cbCurrMemberOrder;


			//更新内容
			if(m_pDlgShopDetails)
			{
				m_pDlgShopDetails->UpdateUI();
			}

			//发送事件
			CPlatformEvent * pPlatformEvent=CPlatformEvent::GetInstance();
			if (pPlatformEvent!=NULL) pPlatformEvent->SendPlatformEvent(EVENT_USER_INFO_UPDATE, 0L);

			//终止任务
			m_MissionManager.ConcludeMissionItem(this,false);

			//弹出信息
			if( static_cast<BUY_MODE>(m_tagBuyGamePropertyInfo.cbUse) == BUY_IMMEDIATELY_USE )
			{
				tagUseGamePropertyInfo  UsePropertyInfo;
				UsePropertyInfo.dwUserID = pGlobalUserData->dwUserID;
				UsePropertyInfo.dwRecvUserID = pGlobalUserData->dwUserID;
				UsePropertyInfo.dwPropID = m_tagBuyGamePropertyInfo.dwPropertyID;
				UsePropertyInfo.wPropCount = m_tagBuyGamePropertyInfo.dwBuyNumber;
				PerformUseGameProperty(&UsePropertyInfo);
			}
			else
			{
				if(pPropertySuccess->szNotifyContent[0] != 0)
				{
					CDlgInformation Information(this);
					Information.ShowMessageBox(pPropertySuccess->szNotifyContent,MB_OK|MB_ICONINFORMATION,30);
					return true;
				}
			}

		}
		else if(Command.wSubCmdID == SUB_GP_PROPERTY_USE_RESULT)
		{
			CMD_GP_S_PropertyUse* pPropertyUse = (CMD_GP_S_PropertyUse*)pData;
			CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
			tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();
			if(pPropertyUse->dwHandleCode != 0)
			{
				//提示消息
				CDlgInformation Information(NULL);
				Information.ShowMessageBox(pPropertyUse->szNotifyContent,MB_OK|MB_ICONSTOP,30);
				return false;
			}

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
		}
		return true;
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CDlgShopServer::CDlgShopServer()
{
	//设置任务
	m_bQueryProperty = false;
	m_bBuyProperty = false;
}

//析构函数
CDlgShopServer::~CDlgShopServer()
{
}

//创建函数
BOOL CDlgShopServer::OnInitDialog()
{
	__super::OnInitDialog();

	//修改样式
	ModifyStyle(0, WS_MINIMIZEBOX|WS_MAXIMIZEBOX);

	//设置标题
	SetWindowText(TEXT("商城"));

	//居中窗口
	CenterWindow(this);

	//下移距离
	CRect rc;
	GetWindowRect(&rc);
	rc.top += 30;
	rc.bottom += 30;
	//移动窗口
	MoveWindow(rc);


	LoadDataFromUI();

	return TRUE;
}

//查询事件
VOID CDlgShopServer::PerformQueryInfo()
{
	m_pITCPSocket->SendData(MDM_GR_PROPERTY, SUB_GR_QUERY_PROPERTY);
	return;
}

//查询事件
VOID CDlgShopServer::PerformBuyGameProperty(tagBuyGamePropertyInfo *ptagBuyGamePropertyInfo)
{
	CopyMemory(&m_tagBuyGamePropertyInfo,ptagBuyGamePropertyInfo,sizeof(tagBuyGamePropertyInfo));
	//获取对象
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	//定义变量
	CMD_GR_PropertyBuy PropertyBuy;
	PropertyBuy.dwUserID = pGlobalUserData->dwUserID;
	PropertyBuy.dwPropertyID = m_tagBuyGamePropertyInfo.dwPropertyID;
	PropertyBuy.dwItemCount = m_tagBuyGamePropertyInfo.dwBuyNumber;
	PropertyBuy.cbConsumeType = m_tagBuyGamePropertyInfo.cbScoreType;
	CWHService::GetMachineIDEx(PropertyBuy.szMachineID);
	lstrcpyn(PropertyBuy.szPassword,pGlobalUserData->szPassword,CountArray(PropertyBuy.szPassword));	
	m_pITCPSocket->SendData(MDM_GR_PROPERTY, SUB_GR_GAME_PROPERTY_BUY, &PropertyBuy, sizeof(PropertyBuy));

	return;
}

VOID CDlgShopServer::PerformUseGameProperty(const tagUseGamePropertyInfo *pPropertyUseInfo)
{
	CopyMemory(&m_PropertyUseInfo,pPropertyUseInfo,sizeof(tagUseGamePropertyInfo));

	CMD_GR_C_PropertyUse PropertyUse;
	PropertyUse.dwUserID = m_PropertyUseInfo.dwUserID;
	PropertyUse.dwRecvUserID = m_PropertyUseInfo.dwRecvUserID;
	PropertyUse.dwPropID = m_PropertyUseInfo.dwPropID;
	PropertyUse.wPropCount = m_PropertyUseInfo.wPropCount;
	m_pITCPSocket->SendData(MDM_GR_PROPERTY, SUB_GR_PROPERTY_USE, &PropertyUse, sizeof(PropertyUse));
}

//配置银行
VOID CDlgShopServer::Initialization(ITCPSocket * pITCPSocket, IClientUserItem * pIMySelfUserItem)
{
	//设置变量
	m_pITCPSocket=pITCPSocket;
	m_pIMySelfUserItem=pIMySelfUserItem;

	return;
}
//银行消息
bool CDlgShopServer::OnServerShopMessage(WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	if(wMainCmdID == MDM_GR_PROPERTY)
	{
		if(wSubCmdID == SUB_GR_GAME_PROPERTY_FAILURE)
		{
			//效验参数
			ASSERT(wDataSize == sizeof(CMD_GR_GamePropertyFailure));
			if(wDataSize != sizeof(CMD_GR_GamePropertyFailure)) return false;
			CMD_GR_GamePropertyFailure * pPropertyFailure=(CMD_GR_GamePropertyFailure *)pData;

			if (pPropertyFailure->lErrorCode == 6)
			{
				CDlgRechargeEnquire EnquireDlg;
				INT intRet = EnquireDlg.ShowMessageBox(pPropertyFailure->szDescribeString);
				if (intRet == IDOK)
				{
					EndDialog();
				}
			}
			else
			{
				CDlgInformation Information(this);
				Information.ShowMessageBox(pPropertyFailure->szDescribeString,MB_OK|MB_ICONERROR,30);
			}
			return true;
		}
		else if(wSubCmdID == SUB_GR_GAME_PROPERTY_BUY_RESULT)
		{
			//效验参数
			ASSERT(wDataSize <= sizeof(CMD_GR_PropertyBuyResult));
			if(wDataSize > sizeof(CMD_GR_PropertyBuyResult)) return false;

			//定义变量
			CMD_GR_PropertyBuyResult * pPropertySuccess=(CMD_GR_PropertyBuyResult *)pData;

			//获取对象
			CGlobalUserInfo * pGlobalUserInfo = CGlobalUserInfo::GetInstance();
			tagGlobalUserData * pGlobalUserData = pGlobalUserInfo->GetGlobalUserData();

			pGlobalUserData->lUserInsure = pPropertySuccess->lInsureScore;
			pGlobalUserData->lUserIngot = pPropertySuccess->lUserMedal;
			pGlobalUserData->lLoveLiness = pPropertySuccess->lLoveLiness;
			pGlobalUserData->dUserBeans = pPropertySuccess->dCash;
			pGlobalUserData->cbMemberOrder = pPropertySuccess->cbCurrMemberOrder;

			if(m_pDlgShopDetails)
			{
				m_pDlgShopDetails->UpdateUI();
			}

			//发送事件
			CPlatformEvent * pPlatformEvent=CPlatformEvent::GetInstance();
			if (pPlatformEvent!=NULL) pPlatformEvent->SendPlatformEvent(EVENT_USER_INFO_UPDATE, 0L);

			if( static_cast<BUY_MODE>(m_tagBuyGamePropertyInfo.cbUse) == BUY_IMMEDIATELY_USE )
			{
				tagUseGamePropertyInfo  UsePropertyInfo;
				UsePropertyInfo.dwUserID = pGlobalUserData->dwUserID;
				UsePropertyInfo.dwRecvUserID = pGlobalUserData->dwUserID;
				UsePropertyInfo.dwPropID = m_tagBuyGamePropertyInfo.dwPropertyID;
				UsePropertyInfo.wPropCount = m_tagBuyGamePropertyInfo.dwBuyNumber;
				PerformUseGameProperty(&UsePropertyInfo);
			}
			else
			{
				//房间喇叭
				if(pPropertySuccess->dwPropertyID == PROPERTY_ID_TRUMPET)
				{
					int nTrumpetCount = CPlatformFrame::GetInstance()->GetPlazaViewServerCtrl()->GetTrumpetCount();
					nTrumpetCount += pPropertySuccess->dwItemCount;
					CPlatformFrame::GetInstance()->GetPlazaViewServerCtrl()->SetTrumpetCount(nTrumpetCount);
				}
				else if(pPropertySuccess->dwPropertyID == PROPERTY_ID_TYPHON)
				{
					int nTyphonCount = CPlatformFrame::GetInstance()->GetPlazaViewServerCtrl()->GetTyphonCount();
					nTyphonCount += pPropertySuccess->dwItemCount;
					CPlatformFrame::GetInstance()->GetPlazaViewServerCtrl()->SetTyphonCount(nTyphonCount);
				}

				if(pPropertySuccess->szNotifyContent[0] != 0)
				{
					CDlgInformation Information(this);
					Information.ShowMessageBox(pPropertySuccess->szNotifyContent,MB_OK|MB_ICONINFORMATION,30);
					return true;
				}
			}

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

			CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
			tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();
			//设置变量
			pGlobalUserData->lUserScore=pPropertyUse->Score;
			pGlobalUserData->cbMemberOrder=pPropertyUse->cbMemberOrder;			

			//发送事件
			CPlatformEvent * pPlatformEvent=CPlatformEvent::GetInstance();
			if (pPlatformEvent!=NULL) pPlatformEvent->SendPlatformEvent(EVENT_USER_INFO_UPDATE,0L);

		}
	}
	return true;
}


//////////////////////////////////////////////////////////////////////////////////