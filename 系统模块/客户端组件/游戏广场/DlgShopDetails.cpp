#include "Stdafx.h"
#include "Resource.h"
#include "DlgShopDetails.h"
#include "PlatformEvent.h"
#include "DlgInformation.h"
//////////////////////////////////////////////////////////////////////////////////
//按钮控件
const TCHAR* const szButtonDecrease = TEXT("ButtonDecrease");
const TCHAR* const szButtonIncrease = TEXT("ButtonIncrease");
const TCHAR* const szButtonBuyUse = TEXT("ButtonBuyUse");
const TCHAR* const szButtonBuyIntoBag = TEXT("ButtonBuyIntoBag");
//const TCHAR* const sz = TEXT("ButtonBuyUse");
//const TCHAR* const sz = TEXT("ButtonBuyUse");

//道具信息
const TCHAR* const szLabelPrice = TEXT("LabelPrice");

const TCHAR* const szLabelVIPPrice = TEXT("LabelVIPPrice");
const TCHAR* const szLabelBuyScore = TEXT("LabelBuyScore");
const TCHAR* const szLabelInsureScore = TEXT("LabelInsureScore");
const TCHAR* const szLabelItemShopIOC = TEXT("LabelItemShopIOC");
const TCHAR* const szLableItemShopName = TEXT("LableItemShopName");
const TCHAR* const szNumberBuy = TEXT("NumberBuy");

//VIP控件
const TCHAR* const szContainerVipAwardControlName = TEXT("ContainerVip");
const TCHAR* const szLabelVipLevelControlName = TEXT("LabelVipLevel");
const TCHAR* const szLabelAwardMemberControlName = TEXT("LableShopVip");

const TCHAR* const szRadioButtonGold = TEXT("RadioButtonGold");
const TCHAR* const szRadioButtonUserMadel = TEXT("RadioButtonUserMadel");
const TCHAR* const szRadioButtonCash = TEXT("RadioButtonCash");
const TCHAR* const szRadioButtonLoveLiness = TEXT("RadioButtonLoveLiness");

const TCHAR* const szNumberOfTypeScore = TEXT("NumberOfTypeScore");
const TCHAR* const szLabelBuyTypeName = TEXT("LabelBuyTypeName");

//文字控件
const TCHAR* const szTextUseTipControlName = TEXT("TextlUseTip");

//容器控件
const TCHAR* const szHoriLayoutDiscountControlName = TEXT("HoriLayoutDiscount");
const TCHAR* const szHoriLayoutVIPPriceControlName = TEXT("HoriLayoutVIPPrice");
//////////////////////////////////////////////////////////////////////////////////

//构造函数
CDlgShopDetails::CDlgShopDetails() : CFGuiDialog(IDD_DLG_POPUP)
{
	//设置任务
	m_MissionManager.InsertMissionItem(this);


	m_dwBuyNumber = 1;
	m_cbConsumeType = INVALID_BYTE;

	m_pIGamePropertyAction = NULL;
	m_eBuyMode = BUY_INTO_BAG;

	m_pCurrGamePropertyTypeItem = NULL;
	m_pCurrGamePropertyItem =NULL;
	m_CurrBagPropertyNum = 0;
}

//析构函数
CDlgShopDetails::~CDlgShopDetails()
{
}

//创建函数
BOOL CDlgShopDetails::OnInitDialog()
{
	__super::OnInitDialog();

	//修改样式
	ModifyStyle(0, WS_MINIMIZEBOX|WS_MAXIMIZEBOX);

	//设置标题
	SetWindowText(TEXT("商城"));

	//居中窗口
	CenterWindow(this);

	InitUI();

	UpdateUI();

	//发起连接
	if (m_MissionManager.AvtiveMissionItem(this,false)==false)  
	{
		return TRUE;
	}

	return TRUE;
}

//确定函数
VOID CDlgShopDetails::OnOK()
{

	return __super::OnOK();
}

//取消消息
VOID CDlgShopDetails::OnCancel()
{
	return __super::OnCancel();
}

//消息提醒
void CDlgShopDetails::Notify(TNotifyUI &  msg)
{
	//获取对象
	CControlUI * pControlUI = msg.pSender;

	//点击事件
	if (lstrcmp(msg.sType, TEXT("click")) == 0)
	{
		//关闭按钮
		if(lstrcmp(pControlUI->GetName(), TEXT("ButtonClose"))==0)
		{
			return OnCancel();
		}

		//
		if(lstrcmp(pControlUI->GetName(), szRadioButtonGold)==0 
		  ||lstrcmp(pControlUI->GetName(), szRadioButtonUserMadel)==0
		  ||lstrcmp(pControlUI->GetName(), szRadioButtonCash)==0
		  ||lstrcmp(pControlUI->GetName(), szRadioButtonLoveLiness)==0)
		{
			UpdateUI();
		//	return OnCancel();
		}

		if(pControlUI->GetName() == szButtonDecrease)
		{
			CNumberUI* pNumberBuy = static_cast<CNumberUI *>(GetControlByName(szNumberBuy));
			if(pNumberBuy != NULL) m_dwBuyNumber = (WORD)(pNumberBuy->GetNumber());
			
			if(m_dwBuyNumber == 1)
			{
				CButtonUI * pButtonUI = static_cast<CButtonUI *>(GetControlByName(szButtonDecrease));
				if(pButtonUI != NULL) pButtonUI->SetEnabled(false);
			}	
			UpdateUI();
			return;
		}
		
		if(pControlUI->GetName() == szButtonIncrease)
		{
			CNumberUI* pNumberBuy = static_cast<CNumberUI *>(GetControlByName(szNumberBuy));
			if(pNumberBuy != NULL) m_dwBuyNumber = (WORD)(pNumberBuy->GetNumber());

			CButtonUI * pButtonUI = static_cast<CButtonUI *>(GetControlByName(szButtonDecrease));
			if(pButtonUI != NULL) pButtonUI->SetEnabled(true);

			UpdateUI();
			return;
		}

		if(pControlUI->GetName() == szButtonBuyUse || pControlUI->GetName() == szButtonBuyIntoBag)
		{
			m_eBuyMode = BUY_INTO_BAG;
			if( pControlUI->GetName() == szButtonBuyUse )
			{
				m_eBuyMode = BUY_IMMEDIATELY_USE;
			}
			tagBuyGamePropertyInfo BuyGamePropertyInfo;
			BuyGamePropertyInfo.dwBuyNumber = m_dwBuyNumber;
			BuyGamePropertyInfo.cbScoreType = m_cbConsumeType;
			BuyGamePropertyInfo.dwPropertyID = m_pCurrGamePropertyItem->m_PropertyItem.dwPropertyID;
			BuyGamePropertyInfo.cbUse = m_eBuyMode;

			m_pIGamePropertyAction->PerformBuyGameProperty(&BuyGamePropertyInfo);
			return;
		}
	}
	
	return;
}

LPCTSTR CDlgShopDetails::GetSkinFile()
{
	return TEXT("DlgShopDetails.xml"); 
}

void CDlgShopDetails::LoadDataFromExternal(CGamePropertyTypeItem *pGamePropertyTypeItem,CGamePropertyItem *pGamePropertyItem)
{
	m_pCurrGamePropertyItem = pGamePropertyItem;
	m_pCurrGamePropertyTypeItem = pGamePropertyTypeItem;
}

VOID CDlgShopDetails::InitData()
{

}

VOID CDlgShopDetails::LoadDataFromMemory()
{

}

VOID CDlgShopDetails::LoadDataFromUI()
{
	return;
}

VOID CDlgShopDetails::UpdateData()
{

}

VOID CDlgShopDetails::InitUI()
{
	//获取对象
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	//定义变量
	CRadioButtonUI *pRadioButtonUIGold, *pRadioButtonUIUserMedal, *pRadioButtonUICash, *pRadioButtonUILoveLiness;

	//获取控件
	pRadioButtonUIGold = static_cast<CRadioButtonUI *>(GetControlByName(szRadioButtonGold));
	pRadioButtonUIUserMedal = static_cast<CRadioButtonUI *>(GetControlByName(szRadioButtonUserMadel));
	pRadioButtonUICash = static_cast<CRadioButtonUI *>(GetControlByName(szRadioButtonCash));
	pRadioButtonUILoveLiness = static_cast<CRadioButtonUI *>(GetControlByName(szRadioButtonLoveLiness));

	//设置按钮
	if(m_pCurrGamePropertyItem->m_PropertyItem.lPropertyGold == 0 && pRadioButtonUIGold != NULL) pRadioButtonUIGold->SetVisible(false);
	if(m_pCurrGamePropertyItem->m_PropertyItem.lPropertyUserMedal == 0 && pRadioButtonUIUserMedal != NULL) pRadioButtonUIUserMedal->SetVisible(false);
	if(m_pCurrGamePropertyItem->m_PropertyItem.dPropertyCash == 0 && pRadioButtonUICash != NULL) pRadioButtonUICash->SetVisible(false);
	if(m_pCurrGamePropertyItem->m_PropertyItem.lPropertyLoveLiness == 0 && pRadioButtonUILoveLiness != NULL) pRadioButtonUILoveLiness->SetVisible(false);
	
	if(m_cbConsumeType==0)
	{
		if(m_pCurrGamePropertyItem->m_PropertyItem.dPropertyCash != 0 && pRadioButtonUICash != NULL && pRadioButtonUICash->IsEnabled() == true)
			pRadioButtonUICash->SetCheck(true);
		else if(m_pCurrGamePropertyItem->m_PropertyItem.lPropertyGold != 0 && pRadioButtonUIGold != NULL && pRadioButtonUIGold->IsEnabled() == true)
			pRadioButtonUIGold->SetCheck(true);
		else if(m_pCurrGamePropertyItem->m_PropertyItem.lPropertyUserMedal != 0 && pRadioButtonUIUserMedal != NULL && pRadioButtonUIUserMedal->IsEnabled() == true)
			pRadioButtonUIUserMedal->SetCheck(true);
		else if(m_pCurrGamePropertyItem->m_PropertyItem.lPropertyLoveLiness != 0 && pRadioButtonUILoveLiness != NULL && pRadioButtonUILoveLiness->IsEnabled() == true)
			pRadioButtonUILoveLiness->SetCheck(true);

	}
	else
	{
		if(pRadioButtonUIGold)
			pRadioButtonUIGold->SetCheck(m_cbConsumeType == CONSUME_TYPE_GOLD);
		if(pRadioButtonUIUserMedal)
			pRadioButtonUIUserMedal->SetCheck(m_cbConsumeType == CONSUME_TYPE_USEER_MADEL);
		if(pRadioButtonUICash)
			pRadioButtonUICash->SetCheck(m_cbConsumeType == CONSUME_TYPE_CASH);
		if(pRadioButtonUILoveLiness)
			pRadioButtonUILoveLiness->SetCheck(m_cbConsumeType == CONSUME_TYPE_LOVELINESS);
	}


	//折扣显示
	CHorizontalLayoutUI* pHoriLayoutDiscount = static_cast<CHorizontalLayoutUI*>(GetControlByName(szHoriLayoutDiscountControlName));
	if(pHoriLayoutDiscount) pHoriLayoutDiscount->SetVisible(pGlobalUserData->cbMemberOrder>0);
	
	//折后价格
	CHorizontalLayoutUI* HoriLayoutVIPPrice = static_cast<CHorizontalLayoutUI*>(GetControlByName(szHoriLayoutVIPPriceControlName));
	if(HoriLayoutVIPPrice) HoriLayoutVIPPrice->SetVisible(pGlobalUserData->cbMemberOrder>0);

	//设置文本
	TCHAR szText[128];
	_sntprintf(szText, CountArray(szText), TEXT("%I64d游戏币"), m_pCurrGamePropertyItem->m_PropertyItem.lPropertyGold);
	pRadioButtonUIGold->SetText(szText);
	_sntprintf(szText, CountArray(szText), TEXT("%I64d元宝"), m_pCurrGamePropertyItem->m_PropertyItem.lPropertyUserMedal);
	pRadioButtonUIUserMedal->SetText(szText);
	_sntprintf(szText, CountArray(szText), TEXT("%0.2f游戏豆"), m_pCurrGamePropertyItem->m_PropertyItem.dPropertyCash);
	pRadioButtonUICash->SetText(szText);
	_sntprintf(szText, CountArray(szText), TEXT("%I64d魅力值"), m_pCurrGamePropertyItem->m_PropertyItem.lPropertyLoveLiness);
	pRadioButtonUILoveLiness->SetText(szText);
	
	//道具图标
	CLabelUI *pLabel = static_cast<CLabelUI *>(GetControlByName(szLabelItemShopIOC));
	if(pLabel != NULL)
	{
		CStdString	csImageKind;
		csImageKind.Format(TEXT("file=\'GMAE_PROPERTY\\GMAE_PROPERTY_%d.png\' restype=\'FILE\'"),m_pCurrGamePropertyItem->m_PropertyItem.dwPropertyID);
		if(m_PaintManager.GetImageEx(csImageKind)==NULL) csImageKind.Empty();
		if(csImageKind.IsEmpty()==true) 
		{
			csImageKind=TEXT("file=\'GMAE_PROPERTY\\GMAE_PROPERTY_NULL.png\' restype=\'FILE\'");
		}

        pLabel->SetBkImage(csImageKind);
	}

	//道具名称
	pLabel = static_cast<CLabelUI *>(GetControlByName(szLableItemShopName));
	if(pLabel != NULL)
	{
		pLabel->SetText(m_pCurrGamePropertyItem->m_PropertyItem.szName);
	}


	TCHAR szTaskRewardTotal[64]=TEXT("");
	if(pGlobalUserData->cbMemberOrder > 0)
	{

		//会员任务
		const tagMemberParameterNew* pMemberParameter = CUserItemElement::GetInstance()->GetUserMemberParameter(pGlobalUserData->cbMemberOrder);
		if(pMemberParameter != NULL)
		{
			_sntprintf(szTaskRewardTotal,CountArray(szTaskRewardTotal),TEXT("%d%%折扣"),pMemberParameter->dwMemberShop);
		}

		CControlUI * pContainerVip = GetControlByName(szContainerVipAwardControlName);
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

			CControlUI * pControl4 = pContainerVip->FindControl(szLabelAwardMemberControlName);
			if(pControl4 !=NULL) 
			{
				pControl4->SetVisible(true);
				pControl4->SetText(szTaskRewardTotal);
			}
		}

	}
	else
	{
		CControlUI * pContainerVip = GetControlByName(szContainerVipAwardControlName);
		if(pContainerVip !=NULL)  pContainerVip->SetVisible(false);
	}

	////VIP折扣
	//pLabel = static_cast<CLabelUI *>(GetControlByName(szLableShopVip));
	//if(pGlobalUserData->cbMemberOrder > 0)
	//{
	//	_sntprintf(szText, CountArray(szText), TEXT("VIP %0.2f折"), (double)m_pCurrGamePropertyItem->m_PropertyItem.wDiscount/10.00);
	//	if(pLabel != NULL) pLabel->SetText(szText);
	//}
	//else
	//{
	//	if(pLabel != NULL) pLabel->SetText(TEXT(""));
	//}

	//设置按钮
	CButtonUI * pButtonUI = static_cast<CButtonUI *>(GetControlByName(szButtonDecrease));
	if(pButtonUI != NULL) pButtonUI->SetEnabled(false);
	
	//按钮可用
	bool bEnbale = IsImmediatelyUseProp();
	CButtonUI* pUseImmediately = static_cast<CButtonUI*>(GetControlByName(szButtonBuyUse));
	if(pUseImmediately != NULL)
	{
		pUseImmediately->SetEnabled(bEnbale);
	}

	//功能说明
	CTextUI* pLabelUseTip = static_cast<CTextUI*>(GetControlByName(szTextUseTipControlName));
	if(pLabelUseTip != NULL)
	{
		if( _tcslen(m_pCurrGamePropertyItem->m_PropertyItem.szRegulationsInfo) == 0)
		{
			pLabelUseTip->SetText(TEXT("暂无说明"));
		}
		else
		{
			pLabelUseTip->SetText(m_pCurrGamePropertyItem->m_PropertyItem.szRegulationsInfo);
		}
	}
	return;
}

VOID CDlgShopDetails::UpdateUI()
{

	//定义变量
	CLabelUI *pLablePrice, *pLabelVIPPrice, *pLabelBuyScore;
	CRadioButtonUI *pRadioButtonUIGold, *pRadioButtonUIUserMedal, *pRadioButtonUICash, *pRadioButtonUILoveLiness;
	
	TCHAR szText[128] = {0};

	//获取控件
	pLablePrice = static_cast<CLabelUI *>(GetControlByName(szLabelPrice));
	pLabelVIPPrice = static_cast<CLabelUI *>(GetControlByName(szLabelVIPPrice));
	pLabelBuyScore = static_cast<CLabelUI *>(GetControlByName(szLabelBuyScore));
	CNumberUI* pNumberBuy = static_cast<CNumberUI *>(GetControlByName(szNumberBuy));

	pRadioButtonUIGold = static_cast<CRadioButtonUI *>(GetControlByName(szRadioButtonGold));
	pRadioButtonUIUserMedal = static_cast<CRadioButtonUI *>(GetControlByName(szRadioButtonUserMadel));
	pRadioButtonUICash = static_cast<CRadioButtonUI *>(GetControlByName(szRadioButtonCash));
	pRadioButtonUILoveLiness = static_cast<CRadioButtonUI *>(GetControlByName(szRadioButtonLoveLiness));

	ASSERT(pLablePrice != NULL && pLabelVIPPrice != NULL && pLabelBuyScore != NULL && pNumberBuy != NULL);
	if(pLablePrice == NULL || pLabelVIPPrice == NULL || pLabelBuyScore == NULL || pNumberBuy == NULL) return;

	//购买数量
	//_sntprintf(szText, CountArray(szText), TEXT("%d"), m_dwBuyNumber);
	//pLableNum->SetText(szText);
	if(pNumberBuy != NULL) pNumberBuy->SetMinNumber(1);
	
	//获取对象
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	//会员折扣
	WORD wDiscount = 0;
	const tagMemberParameterNew* pMemberParameter = CUserItemElement::GetInstance()->GetUserMemberParameter(pGlobalUserData->cbMemberOrder);
	if(pMemberParameter != NULL)
	{
		wDiscount = (WORD)pMemberParameter->dwMemberShop;
	}
	//金币购买
	if(pRadioButtonUIGold != NULL && pRadioButtonUIGold->GetCheck() == true)
	{
		//道具原价
		_sntprintf(szText, CountArray(szText), TEXT("%I64d游戏币/个"), m_pCurrGamePropertyItem->m_PropertyItem.lPropertyGold);
		pLablePrice->SetText(szText);

		//会员价格
		_sntprintf(szText, CountArray(szText), TEXT("%I64d游戏币/个"), m_pCurrGamePropertyItem->m_PropertyItem.lPropertyGold * wDiscount / 100);
		pLabelVIPPrice->SetText(szText);
		
		//购买金额
		LONGLONG lBuyScore;
		LONG BuyOneScore = 0;
		if(pGlobalUserData->cbMemberOrder == 0)
		{
			BuyOneScore =  (LONG)(m_pCurrGamePropertyItem->m_PropertyItem.lPropertyGold);
		}
		else
		{
			BuyOneScore = (LONG)(m_pCurrGamePropertyItem->m_PropertyItem.lPropertyGold * ((double)wDiscount / 100.00));
		}
		lBuyScore = BuyOneScore * m_dwBuyNumber;
		_sntprintf(szText, CountArray(szText), TEXT("%I64d游戏币"), lBuyScore);
		pLabelBuyScore->SetText(szText);

		m_cbConsumeType = CONSUME_TYPE_GOLD;

		CLabelUI* LabelBuyTypeName = static_cast<CLabelUI*>(GetControlByName(szLabelBuyTypeName));
		if(LabelBuyTypeName != NULL)
		{
			LabelBuyTypeName->SetText(TEXT("银行存款："));
		}

		//银行金币
		CNumberUI *pNumberUI = static_cast<CNumberUI *>(GetControlByName(szNumberOfTypeScore));
		if(pNumberUI != NULL)
		{
			//获取对象
			CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
			tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

			float fUserInsure = (float)(pGlobalUserData->lUserInsure);
			pNumberUI->SetNumberDecimal(false);
			pNumberUI->SetNumber(fUserInsure);
		}
		int maxNumber;
		if (BuyOneScore==0)
		{
			maxNumber= 1;
		}
		else
		{
			maxNumber = pGlobalUserData->lUserInsure/BuyOneScore;
		}

		if(pNumberBuy != NULL) pNumberBuy->SetMaxNumber(maxNumber);

		CButtonUI* pButtonIncrease = static_cast<CButtonUI*>(GetControlByName(szButtonIncrease));
		if(pButtonIncrease != NULL) pButtonIncrease->SetEnabled( maxNumber > 1 );
	}

	//元宝购买
	if(pRadioButtonUIUserMedal != NULL && pRadioButtonUIUserMedal->GetCheck() == true)
	{
		//道具原价
		_sntprintf(szText, CountArray(szText), TEXT("%I64d元宝/个"), m_pCurrGamePropertyItem->m_PropertyItem.lPropertyUserMedal);
		pLablePrice->SetText(szText);

		//会员价格
		_sntprintf(szText, CountArray(szText), TEXT("%I64d元宝/个"), m_pCurrGamePropertyItem->m_PropertyItem.lPropertyUserMedal * wDiscount / 100);
		pLabelVIPPrice->SetText(szText);
		
		//购买金额
		LONGLONG lBuyScore;
		LONG BuyOneScore = 0;
		if(pGlobalUserData->cbMemberOrder == 0)
		{
			BuyOneScore =  (LONG)(m_pCurrGamePropertyItem->m_PropertyItem.lPropertyUserMedal);
		}
		else
		{
			BuyOneScore = (LONG)(m_pCurrGamePropertyItem->m_PropertyItem.lPropertyUserMedal * ((double)wDiscount / 100.00));
		}
		lBuyScore = BuyOneScore * m_dwBuyNumber;
		_sntprintf(szText, CountArray(szText), TEXT("%I64d元宝"), lBuyScore);
		pLabelBuyScore->SetText(szText);

		m_cbConsumeType = CONSUME_TYPE_USEER_MADEL;


		CLabelUI* LabelBuyTypeName = static_cast<CLabelUI*>(GetControlByName(szLabelBuyTypeName));
		if(LabelBuyTypeName != NULL)
		{
			LabelBuyTypeName->SetText(TEXT("元    宝："));
		}

		CNumberUI *pNumberUI = static_cast<CNumberUI *>(GetControlByName(szNumberOfTypeScore));
		if(pNumberUI != NULL)
		{
			float fUserIngot = (float)(pGlobalUserData->lUserIngot);
			pNumberUI->SetNumberDecimal(false);
			pNumberUI->SetNumber(fUserIngot);
		}

		int maxNumber;
		if (BuyOneScore==0)
		{
			maxNumber= 1;
		}
		else
		{
			maxNumber = pGlobalUserData->lUserIngot/BuyOneScore;
		}

		if(pNumberBuy != NULL) pNumberBuy->SetMaxNumber(maxNumber);

		CButtonUI* pButtonIncrease = static_cast<CButtonUI*>(GetControlByName(szButtonIncrease));
		if(pButtonIncrease != NULL) pButtonIncrease->SetEnabled( maxNumber > 1 );
	}

	//游戏豆购买
	if(pRadioButtonUICash != NULL && pRadioButtonUICash->GetCheck() == true)
	{
		//道具原价
		_sntprintf(szText, CountArray(szText), TEXT("%0.2f游戏豆/个"), m_pCurrGamePropertyItem->m_PropertyItem.dPropertyCash);
		pLablePrice->SetText(szText);

		//会员价格
		_sntprintf(szText, CountArray(szText), TEXT("%0.2f游戏豆/个"), m_pCurrGamePropertyItem->m_PropertyItem.dPropertyCash * ((double)wDiscount / 100.00));
		pLabelVIPPrice->SetText(szText);
		
		//购买金额
		double lBuyScore;
		LONG BuyOneScore = 0;
		if(pGlobalUserData->cbMemberOrder == 0)
		{
			BuyOneScore =  (LONG)(m_pCurrGamePropertyItem->m_PropertyItem.dPropertyCash);
		}
		else
		{
			BuyOneScore = (LONG)(m_pCurrGamePropertyItem->m_PropertyItem.dPropertyCash * ((double)wDiscount / 100.00));
		}
		lBuyScore = BuyOneScore * m_dwBuyNumber;
		_sntprintf(szText, CountArray(szText), TEXT("%0.2f游戏豆"), lBuyScore);
		pLabelBuyScore->SetText(szText);

		m_cbConsumeType = CONSUME_TYPE_CASH;

		CLabelUI* LabelBuyTypeName = static_cast<CLabelUI*>(GetControlByName(szLabelBuyTypeName));
		if(LabelBuyTypeName != NULL)
		{
			LabelBuyTypeName->SetText(TEXT("  游戏豆："));
		}

		CNumberUI *pNumberUI = static_cast<CNumberUI *>(GetControlByName(szNumberOfTypeScore));
		if(pNumberUI != NULL)
		{
			float fUserBeans = (float)(pGlobalUserData->dUserBeans);
			pNumberUI->SetNumberDecimal(true);
			pNumberUI->SetNumber(fUserBeans);
			
		}

		int maxNumber;
		if (BuyOneScore==0)
		{
			maxNumber= 1;
		}
		else
		{
			maxNumber = pGlobalUserData->dUserBeans/BuyOneScore;
		}

		if(pNumberBuy != NULL) pNumberBuy->SetMaxNumber(maxNumber);

		CButtonUI* pButtonIncrease = static_cast<CButtonUI*>(GetControlByName(szButtonIncrease));
		if(pButtonIncrease != NULL) pButtonIncrease->SetEnabled( maxNumber > 1 );
	}

	//魅力值购买
	if(pRadioButtonUILoveLiness != NULL && pRadioButtonUILoveLiness->GetCheck() == true)
	{
		//道具原价
		_sntprintf(szText, CountArray(szText), TEXT("%I64d魅力值/个"), m_pCurrGamePropertyItem->m_PropertyItem.lPropertyLoveLiness);
		pLablePrice->SetText(szText);

		//会员价格
		_sntprintf(szText, CountArray(szText), TEXT("%I64d魅力值/个"), m_pCurrGamePropertyItem->m_PropertyItem.lPropertyLoveLiness * wDiscount / 100);
		pLabelVIPPrice->SetText(szText);
		
		//购买金额
		LONGLONG lBuyScore;
		LONG BuyOneScore = 0;
		if(pGlobalUserData->cbMemberOrder == 0)
		{
			BuyOneScore =  (LONG)(m_pCurrGamePropertyItem->m_PropertyItem.lPropertyLoveLiness);
		}
		else
		{
			BuyOneScore = (LONG)(m_pCurrGamePropertyItem->m_PropertyItem.lPropertyLoveLiness * ((double)wDiscount / 100.00));
		}
		lBuyScore = BuyOneScore * m_dwBuyNumber;

		_sntprintf(szText, CountArray(szText), TEXT("%I64d魅力值"), lBuyScore);
		pLabelBuyScore->SetText(szText);

		m_cbConsumeType = CONSUME_TYPE_LOVELINESS;

		CLabelUI* LabelBuyTypeName = static_cast<CLabelUI*>(GetControlByName(szLabelBuyTypeName));
		if(LabelBuyTypeName != NULL)
		{
			LabelBuyTypeName->SetText(TEXT("  魅力值："));
		}

		CNumberUI *pNumberUI = static_cast<CNumberUI *>(GetControlByName(szNumberOfTypeScore));
		if(pNumberUI != NULL)
		{
			float fLoveLiness = (float)(pGlobalUserData->lLoveLiness);
			pNumberUI->SetNumberDecimal(false);
			pNumberUI->SetNumber(fLoveLiness);
		}

		int maxNumber;
		if (BuyOneScore==0)
		{
			maxNumber= 1;
		}
		else
		{
			maxNumber = pGlobalUserData->lLoveLiness/BuyOneScore;
		}
		if(pNumberBuy != NULL) pNumberBuy->SetMaxNumber(maxNumber);

		CButtonUI* pButtonIncrease = static_cast<CButtonUI*>(GetControlByName(szButtonIncrease));
		if(pButtonIncrease != NULL) pButtonIncrease->SetEnabled( maxNumber > 1 );
	}

	//背包数量

	CLabelUI* pBagNum = (CLabelUI *)GetControlByName(_T("TextlBagNum"));
	if(pBagNum)
	{
		_sntprintf(szText, CountArray(szText), TEXT("%d"), m_CurrBagPropertyNum);
		pBagNum->SetText(szText);
	}

	return;
}


//连接事件
bool CDlgShopDetails::OnEventMissionLink(INT nErrorCode)
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

	//获取对象
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	CMD_GP_PropertyQuerySingle PropertyQuerySingle;
	ZeroMemory(&PropertyQuerySingle,sizeof(PropertyQuerySingle));
	PropertyQuerySingle.dwUserID = pGlobalUserData->dwUserID;
	PropertyQuerySingle.dwPropertyID = m_pCurrGamePropertyItem->m_PropertyItem.dwPropertyID;
	lstrcpyn(PropertyQuerySingle.szPassword,pGlobalUserData->szPassword,CountArray(PropertyQuerySingle.szPassword));
	m_MissionManager.SendData(MDM_GP_PROPERTY, SUB_GP_QUERY_SINGLE, &PropertyQuerySingle, sizeof(PropertyQuerySingle));
	
	
	return true;
}

//关闭事件
bool CDlgShopDetails::OnEventMissionShut(BYTE cbShutReason)
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
		Information.ShowMessageBox(TEXT("连接服务器超时，道具信息获取失败！"),MB_OK|MB_ICONSTOP,30);
	}


	return true;
}

//读取事件
bool CDlgShopDetails::OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize)
{

	if(Command.wMainCmdID == MDM_GP_PROPERTY)
	{
		if(Command.wSubCmdID == SUB_GP_QUERY_SINGLE_RESULT)
		{
			//效验参数
			ASSERT(wDataSize == sizeof(CMD_GP_PropertyQuerySingleResult));
			if(wDataSize != sizeof(CMD_GP_PropertyQuerySingleResult)) return false;

			//提取数据
			CMD_GP_PropertyQuerySingleResult * pParamter= (CMD_GP_PropertyQuerySingleResult *)pData;

			m_CurrBagPropertyNum = pParamter->dwItemCount;
			//终止任务
			m_MissionManager.ConcludeMissionItem(this,false);

			UpdateUI();

		}								  
	}									  
	return true;						  
}										  

//
void CDlgShopDetails::SetShopMainSink(IGamePropertyAction* pIGamePropertyAction)
{
	m_pIGamePropertyAction = pIGamePropertyAction;
	return;
}

bool CDlgShopDetails::IsImmediatelyUseProp()
{
	if(m_pCurrGamePropertyItem == NULL) return true;
	//礼物 和 大小喇叭 不可以购买立即使用
	if( m_pCurrGamePropertyItem->m_PropertyItem.dwPropertyKind == PROP_KIND_GIFT || 
		m_pCurrGamePropertyItem->m_PropertyItem.dwPropertyKind == PROP_KIND_TRUMPET || 
		m_pCurrGamePropertyItem->m_PropertyItem.dwPropertyKind == PROP_KIND_TYPHON )
	{
		return false;
	}
	return true;
}

void CDlgShopDetails::SetDefaultBuy( int nValue )
{
	switch(nValue)
	{
	case 1:
		{
			m_cbConsumeType= CONSUME_TYPE_CASH;
			break;
		}
	case 2:
		{
			m_cbConsumeType= CONSUME_TYPE_GOLD;
			break;
		}
	case 3:
		{
			m_cbConsumeType= CONSUME_TYPE_LOVELINESS;
			break;
		}
	case 4:
		{
			m_cbConsumeType= CONSUME_TYPE_USEER_MADEL;
			break;
		}
	default:
		m_cbConsumeType = 0;
	}

}

//////////////////////////////////////////////////////////////////////////////////