#include "Stdafx.h"
#include "Resource.h"
#include "DlgBagPresent.h"
#include "DlgInformation.h"

//////////////////////////////////////////////////////////////////////////////////
//��ť�ؼ�
const TCHAR* const szButtonCloseControlName = TEXT("ButtonClose");
const TCHAR* const szBagItemPropIOCControlName = TEXT("BagItemPropIOC");
const TCHAR* const szBagItemPropNumControlName = TEXT("BagItemPropNum");
const TCHAR* const szBagItemropSelectControlName = TEXT("BagItemropSelect");
const TCHAR* const szButtonGiveControlName = TEXT("ButtonGive");
const TCHAR* const szButtonDecreaseControlName = TEXT("ButtonDecrease");
const TCHAR* const szButtonIncreaseControlName = TEXT("ButtonIncrease");

//��ѡ�ؼ�
const TCHAR* const szRadioButtonGameIDControlName = TEXT("RadioButtonGameID");
const TCHAR* const szRadioButtonNickNameControlName = TEXT("RadioButtonNickName");

//��ǩ�ؼ�
const TCHAR* const szNumberUseCount = TEXT("NumberUseCount");

//�༭��
const TCHAR* const szEditReceiveNameControlName = TEXT("EditReceiveName");

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CDlgBagPresent::CDlgBagPresent() : CFGuiDialog(IDD_DLG_POPUP)
,m_pNumberUseCount(NULL)
,m_pButtonIncrease(NULL)
,m_pButtonDecrease(NULL)
,m_pLabelUseAfterCount(NULL)
{
	::ZeroMemory(&m_BackpackProperty, sizeof(m_BackpackProperty));
	m_nUseCount = 1;
	m_bIsGiveProperty = false;
	m_pEditReceiveName = NULL;
	m_ePresendMode = PRESEND_NICKNAME;
}

//��������
CDlgBagPresent::~CDlgBagPresent()
{
}

//���õ���
void CDlgBagPresent::SetProperty(const tagBackpackProperty* pBackpackProperty)
{
	memcpy(&m_BackpackProperty, pBackpackProperty, sizeof(tagBackpackProperty));
}

//ʹ������
int CDlgBagPresent::GetUseCount()
{
	return m_nUseCount;
}

//�Ƿ����͵���
bool CDlgBagPresent::IsGiveProperty()
{
	return m_bIsGiveProperty;
}

//���ͷ�ʽ
PRESEND_MODE CDlgBagPresent::GetPresendMode()
{
	return m_ePresendMode;
}

//�༭����
CStdString CDlgBagPresent::GetEditReceiveName()
{
	return m_pEditReceiveName->GetText();
}

//��������
BOOL CDlgBagPresent::OnInitDialog()
{
	__super::OnInitDialog();

	//�޸���ʽ
	ModifyStyle(0, WS_MINIMIZEBOX|WS_MAXIMIZEBOX);

	//���ñ���
	SetWindowText(TEXT("������Ʒ"));

	//���д���
	CenterWindow(this);

	return TRUE;
}

//ȷ������
VOID CDlgBagPresent::OnOK()
{
	return __super::OnOK();
}

//ȡ����Ϣ
VOID CDlgBagPresent::OnCancel()
{
	return __super::OnCancel();
}


//��Ϣ����
void CDlgBagPresent::Notify(TNotifyUI &  msg)
{
	//��ȡ����
	CControlUI * pControlUI = msg.pSender;

	//����¼�
	if (lstrcmp(msg.sType, TEXT("click")) == 0)
	{
		//����ر�
		if(lstrcmp(pControlUI->GetName(), szButtonCloseControlName)==0)
		{
			return OnCancel();
		}
		//I D����
		else if( pControlUI->GetName() == szRadioButtonGameIDControlName )
		{
			CLabelUI* pLabelUI = (CLabelUI*)GetControlByName(TEXT("LabelReceiveName"));
			if(pLabelUI != NULL) pLabelUI->SetText(TEXT("����I D��"));
			if(m_pEditReceiveName != NULL){
				m_pEditReceiveName->SetNumberOnly(true);
				m_pEditReceiveName->SetText(TEXT(""));
			}
			m_ePresendMode = PRESEND_GAMEID;
		}
		//�ǳ�����
		else if( pControlUI->GetName() == szRadioButtonNickNameControlName )
		{
			CLabelUI* pLabelUI = (CLabelUI*)GetControlByName(TEXT("LabelReceiveName"));
			if(pLabelUI != NULL) pLabelUI->SetText(TEXT("�����ǳƣ�"));
			if(m_pEditReceiveName != NULL){
				m_pEditReceiveName->SetNumberOnly(false);
				m_pEditReceiveName->SetText(TEXT(""));
			}
			m_ePresendMode = PRESEND_NICKNAME;
		}
		//ȷ������
		else if( pControlUI->GetName() == szButtonGiveControlName )
		{
			CStdString sReceiveName = m_pEditReceiveName->GetText();
			if(sReceiveName.IsEmpty() )
			{
				//��ʾ��Ϣ
				CDlgInformation Information(NULL);
				Information.ShowMessageBox(TEXT("����������ǳƻ���ID��"),MB_ICONERROR,30L);
				return;
			}
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
				if( m_pButtonDecrease ) m_pButtonDecrease->SetEnabled(m_nUseCount > 1);
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

LPCTSTR CDlgBagPresent::GetSkinFile()
{
	return TEXT("DlgBagPresent.xml"); 
}

VOID CDlgBagPresent::InitData()
{
	
}

VOID CDlgBagPresent::LoadDataFromMemory()
{
}

void CDlgBagPresent::InitControlUI()
{
	InitData();
	InitUI();
}

VOID CDlgBagPresent::LoadDataFromUI()
{

}

VOID CDlgBagPresent::UpdateData()
{

}

VOID CDlgBagPresent::InitUI()
{
	m_pEditReceiveName = static_cast<CEditUI*>(GetControlByName(szEditReceiveNameControlName));

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
	m_pNumberUseCount = static_cast<CNumberUI*>(GetControlByName(szNumberUseCount));
	if(m_pNumberUseCount != NULL)
	{
		m_pNumberUseCount->SetNumber(1);
		m_pNumberUseCount->SetMinNumber(1);
		m_pNumberUseCount->SetMaxNumber(m_BackpackProperty.nCount);
	}

	//ʣ������
	m_pLabelUseAfterCount = static_cast<CLabelUI*>(GetControlByName(TEXT("LabelUseAfterCount")));
	if(m_pLabelUseAfterCount != NULL)
	{
		CStdString	csUseAfterCount;
		csUseAfterCount.Format(TEXT("ʣ�ࣺ%d"), m_BackpackProperty.nCount-1);
		m_pLabelUseAfterCount->SetText(csUseAfterCount);
	}
}

VOID CDlgBagPresent::UpdateUI()
{

}

//////////////////////////////////////////////////////////////////////////////////