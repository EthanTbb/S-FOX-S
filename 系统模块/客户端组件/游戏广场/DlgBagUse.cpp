#include "Stdafx.h"
#include "Resource.h"
#include "DlgBagUse.h"

//////////////////////////////////////////////////////////////////////////////////
//按钮控件
const TCHAR* const szButtonCloseControlName = TEXT("ButtonClose");
const TCHAR* const szBagItemPropIOCControlName = TEXT("BagItemPropIOC");
const TCHAR* const szBagItemPropNumControlName = TEXT("BagItemPropNum");
const TCHAR* const szBagItemropSelectControlName = TEXT("BagItemropSelect");
const TCHAR* const szButtonUseControlName = TEXT("ButtonUse");
const TCHAR* const szButtonGiveControlName = TEXT("ButtonGive");
const TCHAR* const szButtonDecreaseControlName = TEXT("ButtonDecrease");
const TCHAR* const szButtonIncreaseControlName = TEXT("ButtonIncrease");

//道具控件
const TCHAR* const szPageLayoutAllProperty = TEXT("PageLayoutAllProperty");
const TCHAR* const szPageLayoutGemProperty = TEXT("PageLayoutGemProperty");
const TCHAR* const szPageLayoutCardProperty = TEXT("PageLayoutCardProperty");
const TCHAR* const szPageLayoutProperty = TEXT("PageLayoutProperty");
const TCHAR* const szPageLayoutGiftProperty = TEXT("PageLayoutGiftProperty");


//标签控件
const TCHAR* const szNumberUseCountControlName = TEXT("NumberUseCount");
const TCHAR* const szLabelUseAreaControlName = TEXT("LabelUseArea");

//文字控件
const TCHAR* const szTextUseTipControlName = TEXT("TextlUseTip");

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CDlgBagUse::CDlgBagUse() : CFGuiDialog(IDD_DLG_POPUP)
,m_pNumberUseCount(NULL)
,m_pButtonIncrease(NULL)
,m_pButtonDecrease(NULL)
,m_pLabelUseAfterCount(NULL)
{
	m_bIsUseProperty = false;
	m_bIsGiveProperty = false;
	::ZeroMemory(&m_BackpackProperty, sizeof(m_BackpackProperty));
	m_nUseCount = 1;
}

//析构函数
CDlgBagUse::~CDlgBagUse()
{
}

//设置道具
void CDlgBagUse::SetProperty(const tagBackpackProperty* pBackpackProperty)
{
	memcpy(&m_BackpackProperty, pBackpackProperty, sizeof(tagBackpackProperty));
}

int CDlgBagUse::GetUseCount()
{
	return m_nUseCount;
}

//是否使用道具
bool CDlgBagUse::IsUseProperty()
{
	return m_bIsUseProperty;
}

//是否赠送道具
bool CDlgBagUse::IsGiveProperty()
{
	return m_bIsGiveProperty;
}

//创建函数
BOOL CDlgBagUse::OnInitDialog()
{
	__super::OnInitDialog();

	//修改样式
	ModifyStyle(0, WS_MINIMIZEBOX|WS_MAXIMIZEBOX);

	//设置标题
	SetWindowText(TEXT("使用物品"));

	//居中窗口
	CenterWindow(this);

	return TRUE;
}

//确定函数
VOID CDlgBagUse::OnOK()
{
 return __super::OnOK();
}

//取消消息
VOID CDlgBagUse::OnCancel()
{
	return __super::OnCancel();
}


//消息提醒
void CDlgBagUse::Notify(TNotifyUI &  msg)
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
		//确定使用
		else if( pControlUI->GetName() == szButtonUseControlName )
		{
			m_bIsUseProperty = true;
			m_bIsGiveProperty = false;
			OnOK();
		}
		//确定赠送
		else if( pControlUI->GetName() == szButtonGiveControlName )
		{
			m_bIsUseProperty = false;
			m_bIsGiveProperty = true;
			OnOK();
		}
		//减少数量
		else if( pControlUI == m_pButtonDecrease )
		{
			if(m_pNumberUseCount != NULL)
			{
				m_nUseCount = (int)m_pNumberUseCount->GetNumber();
				if( m_pButtonIncrease ) m_pButtonIncrease->SetEnabled(m_nUseCount < m_BackpackProperty.nCount);
				m_pButtonDecrease->SetEnabled(m_nUseCount > 1);

				//剩余数量
				if(m_pLabelUseAfterCount != NULL){
					CStdString	csUseAfterCount;
					csUseAfterCount.Format(TEXT("剩余：%d"), m_BackpackProperty.nCount-m_nUseCount);
					m_pLabelUseAfterCount->SetText(csUseAfterCount);
				}
			}
			
		}
		//增加数量
		else if( pControlUI == m_pButtonIncrease )
		{
			if(m_pNumberUseCount != NULL)
			{
				m_nUseCount = (int)m_pNumberUseCount->GetNumber();

				if( m_pButtonDecrease ) m_pButtonDecrease->SetEnabled(m_nUseCount >= 1);
				m_pButtonIncrease ->SetEnabled(m_nUseCount < m_BackpackProperty.nCount);

				//剩余数量
				if(m_pLabelUseAfterCount != NULL){
					CStdString	csUseAfterCount;
					csUseAfterCount.Format(TEXT("剩余：%d"), m_BackpackProperty.nCount-m_nUseCount);
					m_pLabelUseAfterCount->SetText(csUseAfterCount);
				}
			}
		}
	}
	
	return;
}

LPCTSTR CDlgBagUse::GetSkinFile()
{
	return TEXT("DlgBagUse.xml"); 
}

VOID CDlgBagUse::InitData()
{
	
}

VOID CDlgBagUse::LoadDataFromMemory()
{
}

void CDlgBagUse::InitControlUI()
{
	InitData();
	InitUI();
}

VOID CDlgBagUse::LoadDataFromUI()
{

}

VOID CDlgBagUse::UpdateData()
{

}

VOID CDlgBagUse::InitUI()
{
	//物品图标
	CControlUI* pItemPropIOC = static_cast<CControlUI*>(GetControlByName(TEXT("ItemPropIOC")));
	if(pItemPropIOC != NULL)
	{
		CStdString	csImageKind;
		csImageKind.Format(TEXT("file=\'GMAE_PROPERTY\\GMAE_PROPERTY_%d.png\' restype=\'FILE\'"),m_BackpackProperty.Property.wIndex);
		if(m_PaintManager.GetImageEx(csImageKind)==NULL) csImageKind.Empty();
		if(csImageKind.IsEmpty()==true) 
		{
			csImageKind=TEXT("file=\'GMAE_PROPERTY\\GMAE_PROPERTY_NULL.png\' restype=\'FILE\'");
		}
		pItemPropIOC->SetBkImage(csImageKind);
	}

	//物品名称
	CLabelUI* pItemPropName = static_cast<CLabelUI*>(GetControlByName(TEXT("ItemPropName")));
	if(pItemPropName != NULL)
	{
		pItemPropName->SetText(m_BackpackProperty.Property.szName);
	}

	//物品数量
	CLabelUI* pItemPropCount = static_cast<CLabelUI*>(GetControlByName(TEXT("ItemPropCount")));
	if(pItemPropCount != NULL)
	{
		CStdString	csImageCount;
		csImageCount.Format(TEXT("数量：%d"),m_BackpackProperty.nCount);
		pItemPropCount->SetText(csImageCount);
	}
	
	//减少按钮
	m_pButtonDecrease =static_cast<CButtonUI*>(GetControlByName(szButtonDecreaseControlName));
	if(m_pButtonDecrease != NULL)
	{
		m_pButtonDecrease->SetEnabled(false);
	}

	//增加按钮
	m_pButtonIncrease =static_cast<CButtonUI*>(GetControlByName(szButtonIncreaseControlName));
	if(m_pButtonIncrease != NULL)
	{
		m_pButtonIncrease->SetEnabled(m_BackpackProperty.nCount > 1);
	}

	//使用数量
	m_pNumberUseCount = static_cast<CNumberUI*>(GetControlByName(szNumberUseCountControlName));
	if(m_pNumberUseCount != NULL)
	{
		m_pNumberUseCount->SetNumber(1.0f);
		m_pNumberUseCount->SetMinNumber(1);
		m_pNumberUseCount->SetMaxNumber(m_BackpackProperty.nCount);
	}

	//使用后剩余数量
	m_pLabelUseAfterCount = static_cast<CLabelUI*>(GetControlByName(TEXT("LabelUseAfterCount")));
	if(m_pLabelUseAfterCount != NULL)
	{
		CStdString	csUseAfterCount;
		csUseAfterCount.Format(TEXT("剩余：%d"), m_BackpackProperty.nCount-1);
		m_pLabelUseAfterCount->SetText(csUseAfterCount);
	}

	//使用范围
	CLabelUI* pLabelUseArea = static_cast<CLabelUI*>(GetControlByName(szLabelUseAreaControlName));
	if(pLabelUseArea != NULL)
	{
		CStdString csImageCount;
		WORD wUseArea = m_BackpackProperty.Property.wUseArea;

		bool bIssuePlatform = ( (wUseArea&PT_ISSUE_AREA_PLATFORM) != 0);
		bool bIssueServer = ( (wUseArea&PT_ISSUE_AREA_SERVER) != 0);
		bool bIssueGame = ( (wUseArea&PT_ISSUE_AREA_GAME) != 0);

		if(m_BackpackProperty.Property.wKind == PROP_KIND_GIFT)
		{
			csImageCount = TEXT("选定玩家");
		}
		else if(bIssuePlatform && bIssueServer && bIssueGame)
		{
			csImageCount = TEXT("所有");
		}
		else if( !bIssuePlatform && (bIssueServer || bIssueGame) )
		{
			csImageCount = TEXT("房间");
		}
		pLabelUseArea->SetText(csImageCount);
	}
	
	//功能说明
	CTextUI* pLabelUseTip = static_cast<CTextUI*>(GetControlByName(szTextUseTipControlName));
	if(pLabelUseTip != NULL)
	{
		if( _tcslen(m_BackpackProperty.Property.szRegulationsInfo) == 0)
		{
			pLabelUseTip->SetText(TEXT("暂无说明"));
		}
		else
		{
			pLabelUseTip->SetText(m_BackpackProperty.Property.szRegulationsInfo);
		}
	}
	
	//使用道具按钮
	CButtonUI* pButtonUse =static_cast<CButtonUI*>(GetControlByName(szButtonUseControlName));
	if(pButtonUse != NULL)
	{
		bool bEnable = (m_BackpackProperty.Property.wKind != PROP_KIND_GIFT) && (m_BackpackProperty.Property.wKind != PROP_KIND_TRUMPET) && (m_BackpackProperty.Property.wKind != PROP_KIND_TYPHON);
		pButtonUse->SetEnabled(bEnable);
	}
}

VOID CDlgBagUse::UpdateUI()
{

}

//////////////////////////////////////////////////////////////////////////////////