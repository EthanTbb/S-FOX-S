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

//�����ؼ�
const TCHAR* const szHLayoutOptionShopControlName = TEXT("HLayoutOptionShop");
const TCHAR* const szTabLayoutShopControlName = TEXT("TabLayoutShop");

//��ť�ؼ�
const TCHAR* const szButtonCloseControlName = TEXT("ButtonClose");
const TCHAR* const szButtonButtonHelpControlName = TEXT("ButtonButtonHelp");

//���߿ؼ�
const TCHAR* const szPageLayoutAllProperty = TEXT("PageLayoutAllProperty");
const TCHAR* const szPageLayoutCashProperty = TEXT("PageLayoutCashProperty");
const TCHAR* const szPageLayoutGoldProperty = TEXT("PageLayoutGoldProperty");
const TCHAR* const szPageLayoutLoveLinessProperty = TEXT("PageLayoutLoveLinessProperty");
const TCHAR* const szPageLayoutUserMedalProperty = TEXT("PageLayoutUserMedalProperty");

//�������Կؼ�
const TCHAR* const szLableItemShopIOC = TEXT("ItemShopIOC");
const TCHAR* const szLableItemShopName = TEXT("ItemShopName");
const TCHAR* const szLableItemShopPrice = TEXT("ItemShopPrice");

//VIP�ؼ�
const TCHAR* const szLableItemShopVip = TEXT("ItemShopVip");
const TCHAR* const szItemContainerVipControlName = TEXT("ItemContainerVip");
const TCHAR* const szLabelVipLevelControlName = TEXT("LabelVipLevel");
//��ť�ؼ�
const TCHAR* const szButtonMyBag = TEXT("ButtonMyBag");
const TCHAR* const szButtonRecharge = TEXT("ButtonRecharge");
const TCHAR* const szButtonMyInsure = TEXT("ButtonMyInsure");
//�ı��ؼ�
const TCHAR* const szTextShopLink = TEXT("TextShopLink");

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CDlgShopMain::CDlgShopMain() : CFGuiDialog(IDD_DLG_POPUP)
{
	m_pDlgShopDetails = NULL;
	//��������
	m_bQueryProperty = false;
	m_bBuyProperty = false;
	m_bUseProperty = false;
	ZeroMemory(&m_PropertyUseInfo, sizeof(m_PropertyUseInfo));
	
}

//��������
CDlgShopMain::~CDlgShopMain()
{
}

void CDlgShopMain::EndDialog()
{
	if(m_pDlgShopDetails!=NULL) m_pDlgShopDetails->EndDialog(IDOK);
	__super::EndDialog(IDOK);
}

//��������
BOOL CDlgShopMain::OnInitDialog()
{
	//�޸���ʽ
	ModifyStyle(0, WS_MINIMIZEBOX|WS_MAXIMIZEBOX);

	//���ñ���
	SetWindowText(TEXT("�̳�����"));

	//���д���
	/*CenterWindow(this);*/

	LoadDataFromMemory();

	CreateUI();


	return TRUE;
}

//ȷ������
VOID CDlgShopMain::OnOK()
{
	//return __super::OnOK();
}

//ȡ����Ϣ
VOID CDlgShopMain::OnCancel()
{
	return __super::OnCancel();
}


//��Ϣ����
void CDlgShopMain::Notify(TNotifyUI &  msg)
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

		//������ť
		if(lstrcmp(pControlUI->GetName(), szButtonMyBag)==0)
		{
			CPlatformEvent * pPlatformEvent=CPlatformEvent::GetInstance();
			if (pPlatformEvent!=NULL) pPlatformEvent->PostPlatformEvent(EVENT_USER_SHOW_BAG,0L);

			OnCancel();
		}

		//���а�ť
		if(lstrcmp(pControlUI->GetName(), szButtonMyInsure)==0)
		{
			//�����¼�
			CPlatformEvent * pPlatformEvent=CPlatformEvent::GetInstance();
			if (pPlatformEvent!=NULL) pPlatformEvent->PostPlatformEvent(EVENT_USER_SHOW_MY_INSURE,0L);

			OnCancel();
		}

		//�һ���ť
		if(lstrcmp(pControlUI->GetName(), szButtonRecharge)==0)
		{
			CPlatformEvent * pPlatformEvent=CPlatformEvent::GetInstance();
			if (pPlatformEvent!=NULL) pPlatformEvent->PostPlatformEvent(EVENT_USER_SHOW_RECHARGE,0L);

			OnCancel();
		}

		//������Ӧ
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
					TCHAR szLinkName[][16]={TEXT("��Ϸ��"), TEXT("��Ϸ��"), TEXT("��Ϸ��"), TEXT("����ֵ"), TEXT("Ԫ��")};
					CStdString sLinkText;
					sLinkText.Format(TEXT("<a name=\'GetBeanLink\' linkcolor=\'#FF25A5FF\' linkhovercolor=\'#FF0585CF\'>��λ�ȡ%s��</a>"), szLinkName[msg.wParam]);
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
			//��ȡ����
			ASSERT(CGlobalWebLink::GetInstance()!=NULL);
			CGlobalWebLink * pGlobalWebLink=CGlobalWebLink::GetInstance();

			//�����ַ
			TCHAR szLinkInfo[256]=TEXT("");
			_sntprintf(szLinkInfo,CountArray(szLinkInfo),TEXT("%s/%sUserService/UserService4.aspx"),pGlobalWebLink->GetPlatformLink(),pGlobalWebLink->GetValidateLink());
			//��ҳ��
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
	//��ȡ����
	ASSERT(CGamePropertyListData::GetInstance()!=NULL);
	CGamePropertyListData * pGamePropertyListData=CGamePropertyListData::GetInstance();

	m_vGamePropertyTypeList.clear();
	m_vGamePropertyRelatList.clear();
	m_vGamePropertyList.clear();

	//��������
	POSITION Position=NULL;
	CGamePropertyTypeItem    * pGamePropertyTypeItem=NULL;
	CGamePropertyRelatItem   * pGamePropertyRelatItem=NULL;
	CGamePropertyItem		 * pGamePropertyItem=NULL;
	vector<CGamePropertyTypeItem *>::iterator itGamePropertyType;
	vector<CGamePropertyRelatItem *>::iterator itGamePropertyRelat;
	vector<CGamePropertyItem *>::iterator itGameProperty;

	//��������
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

	//������Ϸ
	while (true)
	{
		pGamePropertyRelatItem=pGamePropertyListData->EmunGamePropertyRelatItem(Position);
		if (pGamePropertyRelatItem==NULL) break;
		m_vGamePropertyRelatList.push_back(pGamePropertyRelatItem);
		if (Position==NULL) break;
	}

	//������Ϸ
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
	//����ѡ��
	CContainerUI * pHLayoutOption = (CContainerUI * )(GetControlByName(szHLayoutOptionShopControlName));
	if (pHLayoutOption != NULL)
	{
		//�����Ƽ�
		CDialogBuilder Allbuilder;
		COptionUI * pAllOption = NULL;
		pAllOption = static_cast<COptionUI*>(Allbuilder.Create( _T("DlgShopItemOption.xml"), (UINT)0));
		pHLayoutOption->Add(pAllOption);

		//�������
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
	//����ҳ��
	CTabLayoutUI * pTabLayout = (CTabLayoutUI * )(GetControlByName(szTabLayoutShopControlName));
	if (pTabLayout != NULL)
	{
		//�����Ƽ�
		CDialogBuilder Allbuilder;
		CContainerUI *pAllContainer = NULL;
		pAllContainer = static_cast<CContainerUI*>(Allbuilder.Create( _T("DlgShopItemPage.xml"), (UINT)0, NULL, &m_PaintManager));
		pTabLayout->Add(pAllContainer);

		//�������
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

		//����ͼ��
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
					//�������
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

					//����ͼ��
					for (vector<CGamePropertyItem*>::iterator iter3 = vCurrGamePropertyList.begin();iter3!=vCurrGamePropertyList.end();iter3++)
					{
						CGamePropertyItem * pGamePropertyItem = (CGamePropertyItem *)(*iter3);
						CDialogBuilder builder2;
						CContainerUI *pNewGameICOItem = NULL;
						pNewGameICOItem = static_cast<CContainerUI*>(builder2.Create( _T("DlgShopItemPageItem.xml"), (UINT)0, NULL, &m_PaintManager));

						//����ͼ��
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

						//��������
						CControlUI *pName = pNewGameICOItem->FindControl(szLableItemShopName);
						if (pName!=NULL)
						{
							pName->SetText(pGamePropertyItem->m_PropertyItem.szName);
						}

						//���ü۸�
						TCHAR szPrice[64]={0};	
						if (pGamePropertyItem->m_PropertyItem.dPropertyCash>0 && pGamePropertyTypeItem->m_PropertyTypeItem.dwTypeID ==1)
						{
							_sntprintf(szPrice,CountArray(szPrice), TEXT("%0.2f��Ϸ��"), pGamePropertyItem->m_PropertyItem.dPropertyCash);
						}
						else if (pGamePropertyItem->m_PropertyItem.lPropertyGold && pGamePropertyTypeItem->m_PropertyTypeItem.dwTypeID ==2)
						{
							_sntprintf(szPrice,CountArray(szPrice), TEXT("%I64d��Ϸ��"), pGamePropertyItem->m_PropertyItem.lPropertyGold);
						}
						else if (pGamePropertyItem->m_PropertyItem.lPropertyLoveLiness && pGamePropertyTypeItem->m_PropertyTypeItem.dwTypeID ==3)
						{
							_sntprintf(szPrice,CountArray(szPrice), TEXT("%I64d����ֵ"), pGamePropertyItem->m_PropertyItem.lPropertyLoveLiness);
						}
						else if (pGamePropertyItem->m_PropertyItem.lPropertyUserMedal && pGamePropertyTypeItem->m_PropertyTypeItem.dwTypeID ==4)
						{
							_sntprintf(szPrice,CountArray(szPrice), TEXT("%I64dԪ��"), pGamePropertyItem->m_PropertyItem.lPropertyUserMedal);

						}
						CControlUI *Price = pNewGameICOItem->FindControl(szLableItemShopPrice);
						if(Price != NULL)
						{
							Price->SetText(szPrice);
						}
						//�����ۿ�
						CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
						tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();
						TCHAR szTaskRewardTotal[64]=TEXT("");
						if(pGlobalUserData->cbMemberOrder > 0)
						{

							//��Ա����
							const tagMemberParameterNew* pMemberParameter = CUserItemElement::GetInstance()->GetUserMemberParameter(pGlobalUserData->cbMemberOrder);
							if(pMemberParameter != NULL)
							{
								_sntprintf(szTaskRewardTotal,CountArray(szTaskRewardTotal),TEXT("%d%%�ۿ�"),pMemberParameter->dwMemberShop);
							}

							CControlUI * pContainerVip = pNewGameICOItem->FindControl(szItemContainerVipControlName);
							if(pContainerVip!=NULL)
							{
								//��Ա�ȼ�
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

						//������
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

		//�����Ƽ�
		for (vector<CGamePropertyItem*>::iterator iter3 = m_vGamePropertyList.begin();iter3!=m_vGamePropertyList.end();iter3++)
		{
			CGamePropertyItem * pGamePropertyItem = (CGamePropertyItem *)(*iter3);
			if (pGamePropertyItem->m_PropertyItem.cbRecommend == 1)
			{
				CDialogBuilder builder2;
				CContainerUI *pNewGameICOItem = NULL;
				pNewGameICOItem = static_cast<CContainerUI*>(builder2.Create( _T("DlgShopItemPageItem.xml"), (UINT)0, NULL, &m_PaintManager));

				//����ͼ��
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

				//��������
				CControlUI *pName = pNewGameICOItem->FindControl(szLableItemShopName);
				if (pName!=NULL)
				{
					pName->SetText(pGamePropertyItem->m_PropertyItem.szName);
				}

				//���ü۸�
				TCHAR szPrice[64];	
				if (pGamePropertyItem->m_PropertyItem.dPropertyCash>0)
				{
					_sntprintf(szPrice,CountArray(szPrice), TEXT("%0.2f��Ϸ��"), pGamePropertyItem->m_PropertyItem.dPropertyCash);
				}
				else if (pGamePropertyItem->m_PropertyItem.lPropertyGold>0)
				{
					_sntprintf(szPrice,CountArray(szPrice), TEXT("%I64d��Ϸ��"), pGamePropertyItem->m_PropertyItem.lPropertyGold);

				}
				else if (pGamePropertyItem->m_PropertyItem.lPropertyLoveLiness>0)
				{
					_sntprintf(szPrice,CountArray(szPrice), TEXT("%I64d����ֵ"), pGamePropertyItem->m_PropertyItem.lPropertyLoveLiness);

				}
				else if (pGamePropertyItem->m_PropertyItem.lPropertyUserMedal =4)
				{
					_sntprintf(szPrice,CountArray(szPrice), TEXT("%I64dԪ��"), pGamePropertyItem->m_PropertyItem.lPropertyUserMedal);
				}
				CControlUI *Price = pNewGameICOItem->FindControl(szLableItemShopPrice);
				if(Price != NULL)
				{
					Price->SetText(szPrice);
				}
				//�����ۿ�
				CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
				tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();
				TCHAR szTaskRewardTotal[64]=TEXT("");
				if(pGlobalUserData->cbMemberOrder > 0)
				{

					//��Ա����
					const tagMemberParameterNew* pMemberParameter = CUserItemElement::GetInstance()->GetUserMemberParameter(pGlobalUserData->cbMemberOrder);
					if(pMemberParameter != NULL)
					{
						_sntprintf(szTaskRewardTotal,CountArray(szTaskRewardTotal),TEXT("%d%%�ۿ�"),pMemberParameter->dwMemberShop);
					}

					CControlUI * pContainerVip = pNewGameICOItem->FindControl(szItemContainerVipControlName);
					if(pContainerVip!=NULL)
					{
						//��Ա�ȼ�
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

				//������
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

//��ѯ�¼�
VOID CDlgShopMain::PerformQueryInfo()
{
	return;
}

//��ѯ�¼�
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

	CStdString tMessage = TEXT("����ʹ�óɹ���");
	if( pPropertyUse->dwPropKind == PROP_KIND_GEM )
	{
		tMessage.Format(TEXT("%s����ʹ�óɹ�����ϲ�����%d��Ϸ�ң�"), pPropertyUse->szName, pPropertyUse->lUseResultsGold);
	}

	CDlgInformation Information(NULL);
	Information.ShowMessageBox(tMessage.GetData(), MB_ICONINFORMATION);
}

//////////////////////////////////////////////////////////////////////

//���캯��
CDlgShopPlaza::CDlgShopPlaza()
{
	//��������
	m_MissionManager.InsertMissionItem(this);
	m_bQueryProperty = false;
	m_bBuyProperty = false;
}

//��������
CDlgShopPlaza::~CDlgShopPlaza()
{
}

//��������
BOOL CDlgShopPlaza::OnInitDialog()
{
	__super::OnInitDialog();

	//�޸���ʽ
	ModifyStyle(0, WS_MINIMIZEBOX|WS_MAXIMIZEBOX);

	//���ñ���
	SetWindowText(TEXT("�̳�"));

	//���д���
	CenterWindow(this);

	//���ƾ���
	CRect rc;
	GetWindowRect(&rc);
	rc.top += 30;
	rc.bottom += 30;
	//�ƶ�����
	MoveWindow(rc);


	m_bQueryProperty = true;

	LoadDataFromUI();

	return TRUE;
}

//��ѯ�¼�
VOID CDlgShopPlaza::PerformQueryInfo()
{
	m_bBuyProperty = false;
	m_bQueryProperty = true;

	//��������
	if (m_MissionManager.AvtiveMissionItem(this,false)==false)  
	{
		MessageBox(TEXT("����ʧ��"), TEXT(""), 0);
		return ;
	}

	return;
}

//��ѯ�¼�
VOID CDlgShopPlaza::PerformBuyGameProperty(tagBuyGamePropertyInfo *ptagBuyGamePropertyInfo)
{
	m_bBuyProperty = true;
	m_bQueryProperty = false;

	CopyMemory(&m_tagBuyGamePropertyInfo,ptagBuyGamePropertyInfo,sizeof(tagBuyGamePropertyInfo));

	//��������
	if (m_MissionManager.AvtiveMissionItem(this,false)==false)  
	{
		MessageBox(TEXT("����ʧ��"), TEXT(""), 0);
		return ;
	}
	return;
}

//ʹ���¼�
VOID CDlgShopPlaza::PerformUseGameProperty(const tagUseGamePropertyInfo *pPropertyUseInfo)
{
	m_bBuyProperty = false;
	m_bQueryProperty = false;
	m_bUseProperty = true;
	
	CopyMemory(&m_PropertyUseInfo,pPropertyUseInfo,sizeof(tagUseGamePropertyInfo));

	//��������
	if (m_MissionManager.AvtiveMissionItem(this,false)==false)  
	{
		MessageBox(TEXT("����ʧ��"), TEXT(""), 0);
		return ;
	}
	return;
}

//�����¼�
bool CDlgShopPlaza::OnEventMissionLink(INT nErrorCode)
{
	//���ӽ��
	if (nErrorCode!=0)
	{
		//��������
		if (m_MissionManager.AvtiveMissionItem(this,true)==true)
		{
			return true;
		}

		//������ʾ
		CDlgInformation Information(NULL);
		Information.ShowMessageBox(TEXT("���ӷ�������ʱ��������Ϣ��ȡʧ�ܣ�"),MB_OK|MB_ICONSTOP,30);

		return true;
	}
	//��������
	if(m_bQueryProperty == true)
	{
		//��ȡ����
		CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
		tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

		m_MissionManager.SendData(MDM_GP_PROPERTY, SUB_GP_QUERY_PROPERTY);
	}
	else if(m_bBuyProperty == true)
	{
		//��ȡ����
		CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
		tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

		//�������
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
		//��ȡ����
		CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
		tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

		//ʹ�ñ�����Ʒ
		CMD_GP_C_PropertyUse PropertyUse;
		PropertyUse.dwUserID = m_PropertyUseInfo.dwUserID;
		PropertyUse.dwRecvUserID = m_PropertyUseInfo.dwRecvUserID;
		PropertyUse.dwPropID = m_PropertyUseInfo.dwPropID;
		PropertyUse.wPropCount = m_PropertyUseInfo.wPropCount;
		m_MissionManager.SendData(MDM_GP_PROPERTY, SUB_GP_PROPERTY_USE, &PropertyUse, sizeof(PropertyUse));
	}

	return true;
}

//�ر��¼�
bool CDlgShopPlaza::OnEventMissionShut(BYTE cbShutReason)
{
	//��ʾ��Ϣ
	if (cbShutReason!=SHUT_REASON_NORMAL)
	{
		//��������
		if (m_MissionManager.AvtiveMissionItem(this,true)==true)
		{
			return true;
		}

		//������ʾ
		CDlgInformation Information(NULL);
		Information.ShowMessageBox(TEXT("���ӷ�������ʱ��������Ϣ��ȡʧ�ܣ�"),MB_OK|MB_ICONSTOP);
	}

	return true;
}

//��ȡ�¼�
bool CDlgShopPlaza::OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize)
{
	//�����
	if (Command.wMainCmdID==MDM_GP_PROPERTY)
	{
		if(Command.wSubCmdID == SUB_GP_PROPERTY_FAILURE)
		{
			//Ч�����
			ASSERT(wDataSize == sizeof(CMD_GP_PropertyFailure));
			if(wDataSize != sizeof(CMD_GP_PropertyFailure)) return false;

			//�������
			CMD_GP_PropertyFailure * pPropertyFailure=(CMD_GP_PropertyFailure *)pData;

			//��ֹ����
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
			//Ч�����
			ASSERT(wDataSize == sizeof(CMD_GP_PropertyBuyResult));
			if(wDataSize != sizeof(CMD_GP_PropertyBuyResult)) return false;

			//�������
			CMD_GP_PropertyBuyResult * pPropertySuccess=(CMD_GP_PropertyBuyResult *)pData;

			//��ȡ����
			CGlobalUserInfo * pGlobalUserInfo = CGlobalUserInfo::GetInstance();
			tagGlobalUserData * pGlobalUserData = pGlobalUserInfo->GetGlobalUserData();
			pGlobalUserData->lUserInsure = pPropertySuccess->lInsureScore;
			pGlobalUserData->lUserIngot = pPropertySuccess->lUserMedal;
			pGlobalUserData->lLoveLiness = pPropertySuccess->lLoveLiness;
			pGlobalUserData->dUserBeans = pPropertySuccess->dCash;
			pGlobalUserData->cbMemberOrder = pPropertySuccess->cbCurrMemberOrder;


			//��������
			if(m_pDlgShopDetails)
			{
				m_pDlgShopDetails->UpdateUI();
			}

			//�����¼�
			CPlatformEvent * pPlatformEvent=CPlatformEvent::GetInstance();
			if (pPlatformEvent!=NULL) pPlatformEvent->SendPlatformEvent(EVENT_USER_INFO_UPDATE, 0L);

			//��ֹ����
			m_MissionManager.ConcludeMissionItem(this,false);

			//������Ϣ
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
				//��ʾ��Ϣ
				CDlgInformation Information(NULL);
				Information.ShowMessageBox(pPropertyUse->szNotifyContent,MB_OK|MB_ICONSTOP,30);
				return false;
			}

			//���ñ���
			pGlobalUserData->lUserScore=pPropertyUse->Score;
			pGlobalUserData->cbMemberOrder=pPropertyUse->cbMemberOrder;			

			//�����¼�
			CPlatformEvent * pPlatformEvent=CPlatformEvent::GetInstance();
			if (pPlatformEvent!=NULL) pPlatformEvent->SendPlatformEvent(EVENT_USER_INFO_UPDATE,0L);
			//����BUFF
			tagPropertyBuff Buff={0};
			Buff.dwKind = pPropertyUse->dwPropKind;
			Buff.tUseTime = pPropertyUse->tUseTime;
			Buff.UseResultsValidTime = pPropertyUse->UseResultsValidTime;
			Buff.dwScoreMultiple = pPropertyUse->dwScoreMultiple;
			//�����¼�
			if (pPlatformEvent!=NULL) pPlatformEvent->SendPlatformEvent(EVENT_USER_SHOW_UPDATE_BUFF,(LPARAM)&Buff);

			//��ʾ��Ϣ
			ShowSuccessMessage(pPropertyUse);
		}
		return true;
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CDlgShopServer::CDlgShopServer()
{
	//��������
	m_bQueryProperty = false;
	m_bBuyProperty = false;
}

//��������
CDlgShopServer::~CDlgShopServer()
{
}

//��������
BOOL CDlgShopServer::OnInitDialog()
{
	__super::OnInitDialog();

	//�޸���ʽ
	ModifyStyle(0, WS_MINIMIZEBOX|WS_MAXIMIZEBOX);

	//���ñ���
	SetWindowText(TEXT("�̳�"));

	//���д���
	CenterWindow(this);

	//���ƾ���
	CRect rc;
	GetWindowRect(&rc);
	rc.top += 30;
	rc.bottom += 30;
	//�ƶ�����
	MoveWindow(rc);


	LoadDataFromUI();

	return TRUE;
}

//��ѯ�¼�
VOID CDlgShopServer::PerformQueryInfo()
{
	m_pITCPSocket->SendData(MDM_GR_PROPERTY, SUB_GR_QUERY_PROPERTY);
	return;
}

//��ѯ�¼�
VOID CDlgShopServer::PerformBuyGameProperty(tagBuyGamePropertyInfo *ptagBuyGamePropertyInfo)
{
	CopyMemory(&m_tagBuyGamePropertyInfo,ptagBuyGamePropertyInfo,sizeof(tagBuyGamePropertyInfo));
	//��ȡ����
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	//�������
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

//��������
VOID CDlgShopServer::Initialization(ITCPSocket * pITCPSocket, IClientUserItem * pIMySelfUserItem)
{
	//���ñ���
	m_pITCPSocket=pITCPSocket;
	m_pIMySelfUserItem=pIMySelfUserItem;

	return;
}
//������Ϣ
bool CDlgShopServer::OnServerShopMessage(WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	if(wMainCmdID == MDM_GR_PROPERTY)
	{
		if(wSubCmdID == SUB_GR_GAME_PROPERTY_FAILURE)
		{
			//Ч�����
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
			//Ч�����
			ASSERT(wDataSize <= sizeof(CMD_GR_PropertyBuyResult));
			if(wDataSize > sizeof(CMD_GR_PropertyBuyResult)) return false;

			//�������
			CMD_GR_PropertyBuyResult * pPropertySuccess=(CMD_GR_PropertyBuyResult *)pData;

			//��ȡ����
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

			//�����¼�
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
				//��������
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
			//���ñ���
			pGlobalUserData->lUserScore=pPropertyUse->Score;
			pGlobalUserData->cbMemberOrder=pPropertyUse->cbMemberOrder;			

			//�����¼�
			CPlatformEvent * pPlatformEvent=CPlatformEvent::GetInstance();
			if (pPlatformEvent!=NULL) pPlatformEvent->SendPlatformEvent(EVENT_USER_INFO_UPDATE,0L);

		}
	}
	return true;
}


//////////////////////////////////////////////////////////////////////////////////