#include "Stdafx.h"
#include "Resource.h"
#include "DlgBagPresent.h"
#include "DlgInformation.h"

//////////////////////////////////////////////////////////////////////////////////
//按钮控件
const TCHAR* const szButtonCloseControlName = TEXT("ButtonClose");
const TCHAR* const szBagItemPropIOCControlName = TEXT("BagItemPropIOC");
const TCHAR* const szBagItemPropNumControlName = TEXT("BagItemPropNum");
const TCHAR* const szBagItemropSelectControlName = TEXT("BagItemropSelect");
const TCHAR* const szButtonGiveControlName = TEXT("ButtonGive");
const TCHAR* const szButtonDecreaseControlName = TEXT("ButtonDecrease");
const TCHAR* const szButtonIncreaseControlName = TEXT("ButtonIncrease");

//单选控件
const TCHAR* const szRadioButtonGameIDControlName = TEXT("RadioButtonGameID");
const TCHAR* const szRadioButtonNickNameControlName = TEXT("RadioButtonNickName");

//标签控件
const TCHAR* const szNumberUseCount = TEXT("NumberUseCount");

//编辑框
const TCHAR* const szEditReceiveNameControlName = TEXT("EditReceiveName");

//////////////////////////////////////////////////////////////////////////////////

//构造函数
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

//析构函数
CDlgBagPresent::~CDlgBagPresent()
{
}

//设置道具
void CDlgBagPresent::SetProperty(const tagBackpackProperty* pBackpackProperty)
{
	memcpy(&m_BackpackProperty, pBackpackProperty, sizeof(tagBackpackProperty));
}

//使用数量
int CDlgBagPresent::GetUseCount()
{
	return m_nUseCount;
}

//是否赠送道具
bool CDlgBagPresent::IsGiveProperty()
{
	return m_bIsGiveProperty;
}

//赠送方式
PRESEND_MODE CDlgBagPresent::GetPresendMode()
{
	return m_ePresendMode;
}

//编辑名字
CStdString CDlgBagPresent::GetEditReceiveName()
{
	return m_pEditReceiveName->GetText();
}

//创建函数
BOOL CDlgBagPresent::OnInitDialog()
{
	__super::OnInitDialog();

	//修改样式
	ModifyStyle(0, WS_MINIMIZEBOX|WS_MAXIMIZEBOX);

	//设置标题
	SetWindowText(TEXT("赠送物品"));

	//居中窗口
	CenterWindow(this);

	return TRUE;
}

//确定函数
VOID CDlgBagPresent::OnOK()
{
	return __super::OnOK();
}

//取消消息
VOID CDlgBagPresent::OnCancel()
{
	return __super::OnCancel();
}


//消息提醒
void CDlgBagPresent::Notify(TNotifyUI &  msg)
{
	//获取对象
	CControlUI * pControlUI = msg.pSender;

	//点击事件
	if (lstrcmp(msg.sType, TEXT("click")) == 0)
	{
		//点击关闭
		if(lstrcmp(pControlUI->GetName(), szButtonCloseControlName)==0)
		{
			return OnCancel();
		}
		//I D赠送
		else if( pControlUI->GetName() == szRadioButtonGameIDControlName )
		{
			CLabelUI* pLabelUI = (CLabelUI*)GetControlByName(TEXT("LabelReceiveName"));
			if(pLabelUI != NULL) pLabelUI->SetText(TEXT("接收I D："));
			if(m_pEditReceiveName != NULL){
				m_pEditReceiveName->SetNumberOnly(true);
				m_pEditReceiveName->SetText(TEXT(""));
			}
			m_ePresendMode = PRESEND_GAMEID;
		}
		//昵称赠送
		else if( pControlUI->GetName() == szRadioButtonNickNameControlName )
		{
			CLabelUI* pLabelUI = (CLabelUI*)GetControlByName(TEXT("LabelReceiveName"));
			if(pLabelUI != NULL) pLabelUI->SetText(TEXT("接收昵称："));
			if(m_pEditReceiveName != NULL){
				m_pEditReceiveName->SetNumberOnly(false);
				m_pEditReceiveName->SetText(TEXT(""));
			}
			m_ePresendMode = PRESEND_NICKNAME;
		}
		//确定赠送
		else if( pControlUI->GetName() == szButtonGiveControlName )
		{
			CStdString sReceiveName = m_pEditReceiveName->GetText();
			if(sReceiveName.IsEmpty() )
			{
				//提示消息
				CDlgInformation Information(NULL);
				Information.ShowMessageBox(TEXT("请输入玩家昵称或者ID！"),MB_ICONERROR,30L);
				return;
			}
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
				if( m_pButtonDecrease ) m_pButtonDecrease->SetEnabled(m_nUseCount > 1);
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
	m_pNumberUseCount = static_cast<CNumberUI*>(GetControlByName(szNumberUseCount));
	if(m_pNumberUseCount != NULL)
	{
		m_pNumberUseCount->SetNumber(1);
		m_pNumberUseCount->SetMinNumber(1);
		m_pNumberUseCount->SetMaxNumber(m_BackpackProperty.nCount);
	}

	//剩余数量
	m_pLabelUseAfterCount = static_cast<CLabelUI*>(GetControlByName(TEXT("LabelUseAfterCount")));
	if(m_pLabelUseAfterCount != NULL)
	{
		CStdString	csUseAfterCount;
		csUseAfterCount.Format(TEXT("剩余：%d"), m_BackpackProperty.nCount-1);
		m_pLabelUseAfterCount->SetText(csUseAfterCount);
	}
}

VOID CDlgBagPresent::UpdateUI()
{

}

//////////////////////////////////////////////////////////////////////////////////