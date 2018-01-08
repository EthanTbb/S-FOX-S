#include "StdAfx.h"
#include "Resource.h"
#include "DlgBuyProperty.h"
#include "GamePropertyItem.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////////////
//�ؼ�����

//��ť�ؼ�
const TCHAR * szButtonBuyPropertyControlName = TEXT("ButtonBuyProperty");
const TCHAR * szButtonClosePropertyControlName = TEXT("ButtonCloseProperty");

//�ı��ؼ�
const TCHAR * szTextPropertyNameControlName = TEXT("TextPropertyName");
const TCHAR * szTextPropertyPriceControlName = TEXT("TextPropertyPrice");

//��ǩ�ؼ�
const TCHAR * szLabelAmountControlName = TEXT("LabelAmount");
const TCHAR * szLabelDescribeControlName = TEXT("LabelDescribe");
const TCHAR * szLabelPropertyNameControlName = TEXT("LabelPropertyName");
const TCHAR * szLabelPropertyPriceControlName = TEXT("LabelPropertyPrice");

//�༭�ؼ�
const TCHAR * szEditAmountControlName = TEXT("EditAmount"); 
const TCHAR * szEditNickNameControlName = TEXT("EditNickName"); 

//�ؼ��ؼ�
const TCHAR * szContainerRoundControlName = TEXT("ContainerRound"); 
const TCHAR * szRadioButtonInsureControlName = TEXT("RadioButtonInsure"); 

//////////////////////////////////////////////////////////////////////////////////

//��̬����
CDlgBuyProperty * CDlgBuyProperty::m_pDlgBuyProperty=NULL;				//����ָ��

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CDlgBuyProperty, CSkinDialog)
	ON_EN_CHANGE(IDC_PROPERTY_COUNT, OnEnChangePropertyCount)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CDlgBuyProperty::CDlgBuyProperty() : CSkinDialog(IDD_DLG_PROPERTY)
{
	//���ñ���
	m_lCanBuyCount=0;

	//���ñ���
	m_pGamePropertyItem=NULL;
	m_pIGamePropertySink=NULL;

	//���ö���
	ASSERT(m_pDlgBuyProperty==NULL);
	if (m_pDlgBuyProperty==NULL) m_pDlgBuyProperty=this;

	return;
}

//��������
CDlgBuyProperty::~CDlgBuyProperty()
{
	//���ö���
	ASSERT(m_pDlgBuyProperty==this);
	if (m_pDlgBuyProperty==this) m_pDlgBuyProperty=NULL;

	return;
}

//��Ϣ����
BOOL CDlgBuyProperty::PreTranslateMessage(MSG * pMsg)
{
	//��������
	if ((pMsg->message==WM_KEYDOWN)&&(pMsg->wParam==VK_ESCAPE))
	{
		return TRUE;
	}

	return __super::PreTranslateMessage(pMsg);
}

//��������
BOOL CDlgBuyProperty::OnInitDialog()
{
	__super::OnInitDialog();

	//����״̬
	UpdateCtrlStatus();

	//���ô���
	SetWindowPos(NULL,0,0,490,370,SWP_NOZORDER|SWP_NOMOVE);

	return FALSE;
}

//ȷ������
VOID CDlgBuyProperty::OnOK()
{
	//���ҿؼ�
	CControlUI * pEditNickName=GetControlByName(szEditNickNameControlName);
	if(pEditNickName==NULL) return;

	//���ҿؼ�
	CControlUI * pEditAmount=GetControlByName(szEditAmountControlName);
	if(pEditAmount==NULL) return;

	//��ȡ��Ϣ
	CString strNickName=pEditNickName->GetText();
	strNickName.TrimLeft();strNickName.TrimRight();

	//��ȡ��Ŀ
	UINT wItemCount=_tcstol(pEditAmount->GetText(),NULL,10);

	//�ǳ��ж�
	if (strNickName.IsEmpty()==true)
	{
		//��ʾ��Ϣ
		CInformation Information(this);
		Information.ShowMessageBox(TEXT("�û��ǳƲ���ȷ������������"),MB_ICONERROR);

		//���ý���
		pEditNickName->SetFocus();

		return;
	}

	//��Ŀ�ж�
	if (wItemCount==0 || wItemCount > m_lCanBuyCount)
	{
		//��ʾ��Ϣ
		CInformation Information(this);
		Information.ShowMessageBox(TEXT("������Ŀ����ȷ������������"),MB_ICONERROR);

		//���ý���
		pEditAmount->SetFocus();

		return;
	}

	//��ȡ��Ϣ
	ASSERT(m_pGamePropertyItem->GetPropertyAttrib()!=NULL);
	tagPropertyAttrib * pPropertyAttrib=m_pGamePropertyItem->GetPropertyAttrib();

	//��ǰ���
	ASSERT(m_pIGamePropertySink!=NULL);
	TCHAR szMessage[128]=TEXT("");
	if(m_pIGamePropertySink->OnEventBuyPropertyPrep(strNickName,pPropertyAttrib->wIndex,szMessage)==false)
	{
		//��ʾ��Ϣ
		CInformation Information(this);
		Information.ShowMessageBox(szMessage,MB_ICONERROR);

		return;
	}

	//�¼�֪ͨ	
	if (m_pIGamePropertySink->OnEventBuyProperty(strNickName,wItemCount,pPropertyAttrib->wIndex)==true)
	{
		//�رմ���
		DestroyWindow();
	}

	return;
}

//ȡ����Ϣ
VOID CDlgBuyProperty::OnCancel()
{
	//�رմ���
	DestroyWindow();

	return;
}

//��ʼ�ؼ�
VOID CDlgBuyProperty::InitControlUI()
{
	__super::InitControlUI();

	//�û��ǳ�
	CEditUI * pEditNickName=static_cast<CEditUI*>(GetControlByName(szEditNickNameControlName));
	if(pEditNickName)
	{
		pEditNickName->SetMaxChar(LEN_NICKNAME-1);	
	}

	//��������
	CEditUI * pEditAmount=static_cast<CEditUI*>(GetControlByName(szEditAmountControlName));
	if(pEditAmount) 
	{
		pEditAmount->SetMaxChar(12);	
		pEditAmount->SetNumberOnly(true);
	}

	//��������
	CLabelUI * pLabelDescribe=static_cast<CLabelUI *>(GetControlByName(szLabelDescribeControlName));
	if(pLabelDescribe)
	{
		pLabelDescribe->SetTextStyle(DT_LEFT|DT_TOP|DT_WORDBREAK);
	}

	//֧����ʽ
	CRadioButtonUI * pRedioButtonInsure=static_cast<CRadioButtonUI *>(GetControlByName(szRadioButtonInsureControlName));
	if(pRedioButtonInsure)
	{
		pRedioButtonInsure->SetCheck(true);
	}
	
}

//��Ϣ����
VOID CDlgBuyProperty::Notify(TNotifyUI &  msg)
{
	__super::Notify(msg);

	//��ȡ����
	CControlUI * pControlUI = msg.pSender;

	if (lstrcmp(msg.sType, TEXT("click")) == 0)
	{
		if (lstrcmp(pControlUI->GetName(),szButtonBuyPropertyControlName) == 0)
		{
			return OnOK();
		}
		else if (lstrcmp(pControlUI->GetName(),szButtonClosePropertyControlName) == 0)
		{
			return OnCancel();
		}
	}
	else if(lstrcmp(msg.sType,TEXT("textchanged")) ==0 )
	{
		if(lstrcmp(pControlUI->GetName(),szEditAmountControlName)==0)
		{
			return OnEnChangePropertyCount();
		}
	}
}

//�滭��Ϣ
VOID CDlgBuyProperty::OnDrawClientAreaEnd(CDC * pDC, INT nWidth, INT nHeight)
{
	//���λ��
	INT nTBorder=m_SkinAttribute.m_EncircleInfoFrame.nTBorder;
	INT nBBorder=m_SkinAttribute.m_EncircleInfoFrame.nBBorder;
	INT nLBorder=m_SkinAttribute.m_EncircleInfoFrame.nLBorder;
	INT nRBorder=m_SkinAttribute.m_EncircleInfoFrame.nRBorder;

	//����λ��
	CSize SizeRound(0,0);
	CControlUI * pControlRound = GetControlByName(szContainerRoundControlName);
	if(pControlRound) SizeRound = pControlRound->GetFixedXY();

	//������Դ
	CPngImage ImagePropertyFrame;
	ImagePropertyFrame.LoadImage(GetModuleHandle(GAME_PROPERTY_DLL_NAME),TEXT("PROPERTY_FRAME"));

	//��ȡ��С
	CSize SizePropertyFrame;
	SizePropertyFrame.SetSize(ImagePropertyFrame.GetWidth(),ImagePropertyFrame.GetHeight());

	//�滭���
	INT nXItemPos=SizeRound.cx+15;
	INT nYItemPos=SizeRound.cy+15;
	ImagePropertyFrame.DrawImage(pDC,nXItemPos-(SizePropertyFrame.cx-PROPERTY_SYMBOL_CX)/2,nYItemPos-(SizePropertyFrame.cy-PROPERTY_SYMBOL_CY)/2);

	//�滭ͼ��
	if (m_pGamePropertyItem!=NULL)
	{
		//��ȡ��Դ
		tagPropertyImage PropertyImage;
		m_pGamePropertyItem->GetImageInfo(PropertyImage);

		//�滭ͼ��
		CPngImage ImageSymbol;
		ImageSymbol.LoadImage(PropertyImage.hResInstance,PropertyImage.pszSymbolID);
		ImageSymbol.DrawImage(pDC,nXItemPos,nYItemPos,PROPERTY_SYMBOL_CX,PROPERTY_SYMBOL_CY,0,0,ImageSymbol.GetWidth(),ImageSymbol.GetHeight());
	}

	return;
}

//����״̬
VOID CDlgBuyProperty::UpdateCtrlStatus()
{
	//Ч�����
	ASSERT(m_pGamePropertyItem!=NULL);
	ASSERT(m_pIGamePropertySink!=NULL);

	//��ȡ����
	tagPropertyInfo * pPropertyInfo=m_pGamePropertyItem->GetPropertyInfo();
	tagPropertyAttrib * pPropertyAttrib=m_pGamePropertyItem->GetPropertyAttrib();

	//��ȡ����
	CControlUI * pLabelDescribe=GetControlByName(szLabelDescribeControlName);
	if(pLabelDescribe==NULL) return;

	//��ȡ����
	CControlUI * pLabelAmount=GetControlByName(szLabelAmountControlName);
	if(pLabelAmount==NULL) return;

	//��ȡ����
	CControlUI * pTextPropertyName=GetControlByName(szTextPropertyNameControlName);
	if(pTextPropertyName==NULL) return;

	//��ȡ����
	CControlUI * pTextPropertyPrice=GetControlByName(szTextPropertyPriceControlName);
	if(pTextPropertyPrice==NULL) return;

	//��ȡ����
	CControlUI * pLabelPropertyName=GetControlByName(szLabelPropertyNameControlName);
	if(pLabelPropertyName==NULL) return;

	//��ȡ����
	CControlUI * pLabelPropertyPrice=GetControlByName(szLabelPropertyPriceControlName);
	if(pLabelPropertyPrice==NULL) return;

	//��ȡ����
	CEditUI * pEditAmount=static_cast<CEditUI*>(GetControlByName(szEditAmountControlName));
	if(pEditAmount==NULL)  return;

	//���ñ�ǩ
	if(pPropertyAttrib->wPropertyType==PT_TYPE_PROPERTY)
	{		
		pTextPropertyName->SetText(TEXT("�������ƣ�"));
		pTextPropertyPrice->SetText(TEXT("���߼۸�"));
	}
	else
	{
		pTextPropertyName->SetText(TEXT("�������ƣ�"));
		pTextPropertyPrice->SetText(TEXT("����۸�"));
	}

	
	
	//��Ա�ۿ�
	WORD wDiscount = 0;


	//���߼۸�
	TCHAR szPropertyGold[64]=TEXT("");
	_sntprintf(szPropertyGold,CountArray(szPropertyGold),TEXT("%I64d ��Ϸ��/%s ����Ա�ۿۣ�%ld%%��"),
		pPropertyInfo->lPropertyGold,pPropertyAttrib->szMeasuringunit,wDiscount);

	//���ñ���
	SetWindowText(pPropertyAttrib->szPropertyName);

	//���߼۸�
	pLabelPropertyPrice->SetText(szPropertyGold);

	//���ߵ�λ
	//SetDlgItemText(IDC_STATIC_COUNT,pPropertyAttrib->szMeasuringunit);

	//��������
	pLabelDescribe->SetText(pPropertyAttrib->szRegulationsInfo);
	pLabelPropertyName->SetText(pPropertyAttrib->szPropertyName);	

	//������Ŀ
	if(m_pIGamePropertySink!=NULL)	
	{
		IClientUserItem * pClientUserItem = m_pIGamePropertySink->OnQueryClientUserItem();
		if(pClientUserItem!=NULL)
		{
			//����۸�
			DOUBLE lPrice=(DOUBLE)pPropertyInfo->lPropertyGold;
			if(pClientUserItem->GetMemberOrder()>0) lPrice=lPrice*wDiscount/100;

			//������Ŀ
			TCHAR szBuyCountInfo[32]=TEXT("");
			if(pClientUserItem!=NULL) 
			{
				m_lCanBuyCount=__max((LONGLONG)(pClientUserItem->GetUserInsure()/lPrice),0);
				if(pPropertyInfo->wIndex==PROPERTY_ID_SCORE_CLEAR || pPropertyInfo->wIndex==PROPERTY_ID_ESCAPE_CLEAR)
				{
					m_lCanBuyCount=__min(m_lCanBuyCount,1);
				}
			}
			_sntprintf(szBuyCountInfo,CountArray(szBuyCountInfo),TEXT("��������ܹ��� %I64d %s��"),m_lCanBuyCount,pPropertyAttrib->szMeasuringunit);

			pLabelAmount->SetText(szBuyCountInfo);	

			//��������
			UINT  wItemCount = _tcstol(pEditAmount->GetText(),NULL,10);
			
			//��Ŀ�ж�
			if(wItemCount>m_lCanBuyCount) 
			{
				//��Ŀ����
				TCHAR szPropertyCount[16]=TEXT("");
				_sntprintf(szPropertyCount,CountArray(szPropertyCount),TEXT("%I64d"),m_lCanBuyCount);
				pEditAmount->SetText(szPropertyCount);
				pEditAmount->SetSel(-1,-1);
			}
		}
	}

	return;
}

//��ʾ����
VOID CDlgBuyProperty::ShowBuyPropertyWnd(CGamePropertyBase * pGamePropertyItem, IGamePropertySink * pIGamePropertySink)
{
	//Ч�����
	ASSERT(pGamePropertyItem!=NULL);
	ASSERT(pIGamePropertySink!=NULL);

	//��������
	CDlgBuyProperty * pDlgBuyProperty=m_pDlgBuyProperty;

	//��������
	if (pDlgBuyProperty==NULL)
	{
		try
		{
			pDlgBuyProperty=new CDlgBuyProperty;
		}
		catch (...)
		{
			ASSERT(FALSE);
		}
	}

	//���ñ���
	pDlgBuyProperty->m_pGamePropertyItem=pGamePropertyItem;
	pDlgBuyProperty->m_pIGamePropertySink=pIGamePropertySink;

	//��������
	if (pDlgBuyProperty->m_hWnd==NULL)
	{
		AfxSetResourceHandle(GetModuleHandle(GAME_PROPERTY_DLL_NAME));
		pDlgBuyProperty->Create(IDD_DLG_PROPERTY,AfxGetMainWnd());
		AfxSetResourceHandle(GetModuleHandle(NULL));
	}
	else
	{
		//����״̬
		pDlgBuyProperty->UpdateCtrlStatus();
	}

	//��ʾ����
	pDlgBuyProperty->ShowWindow(SW_SHOW);

	//�����
	pDlgBuyProperty->SetActiveWindow();
	pDlgBuyProperty->SetForegroundWindow();

	return;
}

//�ı��¼�
VOID CDlgBuyProperty::OnEventPerformCommand(WORD wCommandID,VOID * pCmdData)
{
	//�����ж�
	if(m_pDlgBuyProperty==NULL) return;

	//�ֱ���
	if(wCommandID==CMD_CLOSE_WND)
	{
		//�رմ���
		if(m_pDlgBuyProperty->m_hWnd!=NULL) m_pDlgBuyProperty->DestroyWindow();
		SafeDelete(m_pDlgBuyProperty);
	}
	else if(wCommandID==CMD_USER_CHANGE)
	{
		//��ȡ����
		IClientUserItem * pIClintUserItem=(IClientUserItem *)pCmdData;

		//�����ж�
		if(pIClintUserItem==NULL) return;

		//�޸��ǳ�
		CControlUI * pEditNickName = m_pDlgBuyProperty->GetControlByName(szEditNickNameControlName);
		if(pEditNickName!=NULL) pEditNickName->SetText(pIClintUserItem->GetNickName());
	}
}

//�ı��¼�
void CDlgBuyProperty::OnEnChangePropertyCount()
{
	//��ȡ����
	CEditUI * pEditAmount=static_cast<CEditUI*>(GetControlByName(szEditAmountControlName));
	if(pEditAmount==NULL)  return;

	//��������	
	CString strCount=pEditAmount->GetText();
	
	//ȥ��ǰ���0
	CString strLeft = strCount.Left(1);
	if(strLeft.Find('0') != -1)
	{
		strCount = strCount.Right(strCount.GetLength() - 1);
		pEditAmount->SetText(strCount);
	}

	UINT wItemCount=_tcstol(pEditAmount->GetText(),NULL,10);

	//��Ŀ�ж�
	if(wItemCount>m_lCanBuyCount) 
	{
		TCHAR szPropertyCount[16]=TEXT("");
		_sntprintf(szPropertyCount,CountArray(szPropertyCount),TEXT("%I64d"),m_lCanBuyCount);
		pEditAmount->SetText(szPropertyCount);
		pEditAmount->SetSel(-1,0);
	}
	
	return;
}

//////////////////////////////////////////////////////////////////////////////////
