#include "Stdafx.h"
#include "Resource.h"
#include "DlgBagUse.h"

//////////////////////////////////////////////////////////////////////////////////
//��ť�ؼ�
const TCHAR* const szButtonCloseControlName = TEXT("ButtonClose");
const TCHAR* const szBagItemPropIOCControlName = TEXT("BagItemPropIOC");
const TCHAR* const szBagItemPropNumControlName = TEXT("BagItemPropNum");
const TCHAR* const szBagItemropSelectControlName = TEXT("BagItemropSelect");
const TCHAR* const szButtonUseControlName = TEXT("ButtonUse");
const TCHAR* const szButtonGiveControlName = TEXT("ButtonGive");
const TCHAR* const szButtonDecreaseControlName = TEXT("ButtonDecrease");
const TCHAR* const szButtonIncreaseControlName = TEXT("ButtonIncrease");

//���߿ؼ�
const TCHAR* const szPageLayoutAllProperty = TEXT("PageLayoutAllProperty");
const TCHAR* const szPageLayoutGemProperty = TEXT("PageLayoutGemProperty");
const TCHAR* const szPageLayoutCardProperty = TEXT("PageLayoutCardProperty");
const TCHAR* const szPageLayoutProperty = TEXT("PageLayoutProperty");
const TCHAR* const szPageLayoutGiftProperty = TEXT("PageLayoutGiftProperty");


//��ǩ�ؼ�
const TCHAR* const szNumberUseCountControlName = TEXT("NumberUseCount");
const TCHAR* const szLabelUseAreaControlName = TEXT("LabelUseArea");

//���ֿؼ�
const TCHAR* const szTextUseTipControlName = TEXT("TextlUseTip");

//////////////////////////////////////////////////////////////////////////////////

//���캯��
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

//��������
CDlgBagUse::~CDlgBagUse()
{
}

//���õ���
void CDlgBagUse::SetProperty(const tagBackpackProperty* pBackpackProperty)
{
	memcpy(&m_BackpackProperty, pBackpackProperty, sizeof(tagBackpackProperty));
}

int CDlgBagUse::GetUseCount()
{
	return m_nUseCount;
}

//�Ƿ�ʹ�õ���
bool CDlgBagUse::IsUseProperty()
{
	return m_bIsUseProperty;
}

//�Ƿ����͵���
bool CDlgBagUse::IsGiveProperty()
{
	return m_bIsGiveProperty;
}

//��������
BOOL CDlgBagUse::OnInitDialog()
{
	__super::OnInitDialog();

	//�޸���ʽ
	ModifyStyle(0, WS_MINIMIZEBOX|WS_MAXIMIZEBOX);

	//���ñ���
	SetWindowText(TEXT("ʹ����Ʒ"));

	//���д���
	CenterWindow(this);

	return TRUE;
}

//ȷ������
VOID CDlgBagUse::OnOK()
{
 return __super::OnOK();
}

//ȡ����Ϣ
VOID CDlgBagUse::OnCancel()
{
	return __super::OnCancel();
}


//��Ϣ����
void CDlgBagUse::Notify(TNotifyUI &  msg)
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
		//ȷ��ʹ��
		else if( pControlUI->GetName() == szButtonUseControlName )
		{
			m_bIsUseProperty = true;
			m_bIsGiveProperty = false;
			OnOK();
		}
		//ȷ������
		else if( pControlUI->GetName() == szButtonGiveControlName )
		{
			m_bIsUseProperty = false;
			m_bIsGiveProperty = true;
			OnOK();
		}
		//��������
		else if( pControlUI == m_pButtonDecrease )
		{
			if(m_pNumberUseCount != NULL)
			{
				m_nUseCount = (int)m_pNumberUseCount->GetNumber();
				if( m_pButtonIncrease ) m_pButtonIncrease->SetEnabled(m_nUseCount < m_BackpackProperty.nCount);
				m_pButtonDecrease->SetEnabled(m_nUseCount > 1);

				//ʣ������
				if(m_pLabelUseAfterCount != NULL){
					CStdString	csUseAfterCount;
					csUseAfterCount.Format(TEXT("ʣ�ࣺ%d"), m_BackpackProperty.nCount-m_nUseCount);
					m_pLabelUseAfterCount->SetText(csUseAfterCount);
				}
			}
			
		}
		//��������
		else if( pControlUI == m_pButtonIncrease )
		{
			if(m_pNumberUseCount != NULL)
			{
				m_nUseCount = (int)m_pNumberUseCount->GetNumber();

				if( m_pButtonDecrease ) m_pButtonDecrease->SetEnabled(m_nUseCount >= 1);
				m_pButtonIncrease ->SetEnabled(m_nUseCount < m_BackpackProperty.nCount);

				//ʣ������
				if(m_pLabelUseAfterCount != NULL){
					CStdString	csUseAfterCount;
					csUseAfterCount.Format(TEXT("ʣ�ࣺ%d"), m_BackpackProperty.nCount-m_nUseCount);
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
	//��Ʒͼ��
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

	//��Ʒ����
	CLabelUI* pItemPropName = static_cast<CLabelUI*>(GetControlByName(TEXT("ItemPropName")));
	if(pItemPropName != NULL)
	{
		pItemPropName->SetText(m_BackpackProperty.Property.szName);
	}

	//��Ʒ����
	CLabelUI* pItemPropCount = static_cast<CLabelUI*>(GetControlByName(TEXT("ItemPropCount")));
	if(pItemPropCount != NULL)
	{
		CStdString	csImageCount;
		csImageCount.Format(TEXT("������%d"),m_BackpackProperty.nCount);
		pItemPropCount->SetText(csImageCount);
	}
	
	//���ٰ�ť
	m_pButtonDecrease =static_cast<CButtonUI*>(GetControlByName(szButtonDecreaseControlName));
	if(m_pButtonDecrease != NULL)
	{
		m_pButtonDecrease->SetEnabled(false);
	}

	//���Ӱ�ť
	m_pButtonIncrease =static_cast<CButtonUI*>(GetControlByName(szButtonIncreaseControlName));
	if(m_pButtonIncrease != NULL)
	{
		m_pButtonIncrease->SetEnabled(m_BackpackProperty.nCount > 1);
	}

	//ʹ������
	m_pNumberUseCount = static_cast<CNumberUI*>(GetControlByName(szNumberUseCountControlName));
	if(m_pNumberUseCount != NULL)
	{
		m_pNumberUseCount->SetNumber(1.0f);
		m_pNumberUseCount->SetMinNumber(1);
		m_pNumberUseCount->SetMaxNumber(m_BackpackProperty.nCount);
	}

	//ʹ�ú�ʣ������
	m_pLabelUseAfterCount = static_cast<CLabelUI*>(GetControlByName(TEXT("LabelUseAfterCount")));
	if(m_pLabelUseAfterCount != NULL)
	{
		CStdString	csUseAfterCount;
		csUseAfterCount.Format(TEXT("ʣ�ࣺ%d"), m_BackpackProperty.nCount-1);
		m_pLabelUseAfterCount->SetText(csUseAfterCount);
	}

	//ʹ�÷�Χ
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
			csImageCount = TEXT("ѡ�����");
		}
		else if(bIssuePlatform && bIssueServer && bIssueGame)
		{
			csImageCount = TEXT("����");
		}
		else if( !bIssuePlatform && (bIssueServer || bIssueGame) )
		{
			csImageCount = TEXT("����");
		}
		pLabelUseArea->SetText(csImageCount);
	}
	
	//����˵��
	CTextUI* pLabelUseTip = static_cast<CTextUI*>(GetControlByName(szTextUseTipControlName));
	if(pLabelUseTip != NULL)
	{
		if( _tcslen(m_BackpackProperty.Property.szRegulationsInfo) == 0)
		{
			pLabelUseTip->SetText(TEXT("����˵��"));
		}
		else
		{
			pLabelUseTip->SetText(m_BackpackProperty.Property.szRegulationsInfo);
		}
	}
	
	//ʹ�õ��߰�ť
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