#include "Stdafx.h"
#include "resource.h"
#include "DlgTrumpetItem.h"

//////////////////////////////////////////////////////////////////////
//�ؼ�����
//��ť�ؼ�
const TCHAR * szButtonColorControlName = TEXT("ButtonColor");
const TCHAR * szButtonCancelControlName = TEXT("ButtonClose");
const TCHAR * szButtonConfirmControlName = TEXT("ButtonConfirm");
const TCHAR * szButtonExpressionControlName = TEXT("ButtonExpression");
const TCHAR * szButtonBagControlName = TEXT("ButtonBag");
const TCHAR * szButtonShopControlName = TEXT("ButtonShop");

//��ǩ�ؼ�
const TCHAR * szLabelBuyDescribeControlName = TEXT("LabelDescribe");

//��չ�ؼ�
const TCHAR * szRadioButtonTyphonControlName = TEXT("RadioButtonTyphon");
const TCHAR * szRadioButtonTrumpetControlName = TEXT("RadioButtonTrumpet");

//�༭��
const TCHAR* szEditTextInputControlName = TEXT("EditTextInput");

//////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CDlgTrumpet, CFGuiDialog)
	ON_WM_TIMER()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////

//���캯��
CDlgTrumpet::CDlgTrumpet() : CFGuiDialog(IDD_DLG_TRUMPET)
{
	//���ñ���
	m_crChatTX = RGB(255,255,255);
	m_wRadioSelected=0;
	m_dwTrumpetCount=0;
	m_dwTyphonCount=0;
   
	//�ӿڱ���
	m_pGameTrumpetSink=NULL;
	m_pIClientUserItem=NULL;

	//�ؼ�����
	m_pExpressionControl=NULL;
}

//��������
CDlgTrumpet::~CDlgTrumpet()
{
	//���ٴ���
	if(m_pExpressionControl && IsWindow(m_pExpressionControl->m_hWnd)) m_pExpressionControl->DestroyWindow();

	//ɾ������
	SafeDelete(m_pExpressionControl);
}

//�ؼ���
void CDlgTrumpet::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_InputChat, m_InputChat);
	DDX_Text(pDX, IDC_InputChat, m_strInuptChat);
	DDV_MaxChars(pDX, m_strInuptChat, TRUMPET_MAX_CHAR);

	return;
}

//��������
void  CDlgTrumpet::UpdateControlSurface()
{
	//�����ж�
	if(m_pIClientUserItem==NULL) return;

	//���߹���
	CGamePropertyManager * pGamePropertyManager = CGamePropertyManager::GetInstance();

	//���Ҷ���
	CRadioButtonUI * pButtonTrumpet = static_cast<CRadioButtonUI *>(GetControlByName(szRadioButtonTrumpetControlName));
	if(pButtonTrumpet==NULL) return;

	//��Ա�ۿ�
	WORD wDiscount = 0;
	//��ʽ�ַ�
	CString strTitle ;
	if(pButtonTrumpet->GetCheck()==true)
	{
		//��ȡ�ۿ�


		//��ȡ����
		CGamePropertyBase * pPropertyItem = pGamePropertyManager->GetPropertyItem(PROPERTY_ID_TRUMPET); 
		if(pPropertyItem==NULL) return;
		tagPropertyItem * pPropertyInfo = pPropertyItem->GetPropertyItem();

		//����۸�
		DOUBLE lPrice=(DOUBLE)pPropertyInfo->lPropertyGold;
		if(m_pIClientUserItem->GetMemberOrder()>0) lPrice=lPrice*wDiscount/100;

		//������Ŀ
		TCHAR szBuyCountInfo[32]=TEXT("");
		DWORD dwCanBuyCount= __max((DWORD)(m_pIClientUserItem->GetUserInsure()/lPrice),0);	

		//��������
		strTitle.Format(TEXT("���ܹ����С������ĿΪ[ %d ] ��"), dwCanBuyCount);       
	}
	else
	{
		//��ȡ����
		CGamePropertyBase * pPropertyItem = pGamePropertyManager->GetPropertyItem(PROPERTY_ID_TYPHON); 
		if(pPropertyItem==NULL) return;
		tagPropertyItem * pPropertyInfo = pPropertyItem->GetPropertyItem();

		//����۸�
		DOUBLE lPrice=(DOUBLE)pPropertyInfo->lPropertyGold;
		if(m_pIClientUserItem->GetMemberOrder()>0) lPrice=lPrice*wDiscount/100;

		//������Ŀ
		TCHAR szBuyCountInfo[32]=TEXT("");
		DWORD dwCanBuyCount= __max((DWORD)(m_pIClientUserItem->GetUserInsure()/lPrice),0);	

		//��������
		strTitle.Format(TEXT("���ܹ���Ĵ�������ĿΪ[ %d ] ��"), dwCanBuyCount);		
	}

	//���ñ���
	//SetWindowText(strTitle);
}

//��ȡ����
void CDlgTrumpet::GetTrumpetContent(LPTSTR pszTrumpetContent)
{
	//�����ж�
	if(pszTrumpetContent==NULL) return;

	//��������
	CopyMemory(pszTrumpetContent,m_strInuptChat,sizeof(TCHAR)*TRUMPET_MAX_CHAR);
}

//ȷ������
VOID CDlgTrumpet::OnOK()
{
	//�ӿ��ж�
	if(m_pGameTrumpetSink==NULL) return; 

	//���Ҷ���
	CRadioButtonUI * pButtonTrumpet = static_cast<CRadioButtonUI *>(GetControlByName(szRadioButtonTrumpetControlName));
	if(pButtonTrumpet==NULL) return;

	//��ȡ����
	WORD wPropertyIndex = pButtonTrumpet->GetCheck()?PROPERTY_ID_TRUMPET:PROPERTY_ID_TYPHON;

	if(wPropertyIndex == PROPERTY_ID_TRUMPET && m_dwTrumpetCount <= 0)
	{
		CInformation Information2(this);
		Information2.ShowMessageBox(TEXT("��û��С���ȣ�����ͨ���̳ǹ���Ŷ��"),MB_ICONWARNING);
		return;
	}

	if(wPropertyIndex == PROPERTY_ID_TYPHON && m_dwTyphonCount <= 0)
	{
		CInformation Information2(this);
		Information2.ShowMessageBox(TEXT("��û�д����ȣ�����ͨ���̳ǹ���Ŷ��"),MB_ICONWARNING);
		return;
	}

	//��ȡ��Ϣ
	m_InputChat.GetWindowText(m_strInuptChat);
	if ( m_strInuptChat.IsEmpty() )
	{
		CInformation Information2(this);
		Information2.ShowMessageBox(TEXT("�������ݲ���Ϊ�գ�"),MB_ICONWARNING);
		return ;
	}

	if ( TRUMPET_MAX_CHAR < m_strInuptChat.GetLength() )
	{ 
		CString strMsg;
		strMsg.Format(TEXT("�����������Ϊ%d���ַ���"), TRUMPET_MAX_CHAR);

		CInformation Information2(this);
		Information2.ShowMessageBox(TEXT("�����������Ϊ%d���ַ���"),TRUMPET_MAX_CHAR,MB_ICONWARNING);
		return ;
	}	

	//�ص��ӿ�
	if(m_pGameTrumpetSink->OnEventUseProperty(wPropertyIndex))
	{
		//�رմ���
		DestroyWindow();
	}
}

//ȡ����Ϣ
VOID CDlgTrumpet::OnCancel()
{
	//�رմ���
	DestroyWindow();

	return;
}

//��ʼ���
BOOL CDlgTrumpet::OnInitDialog()
{
	__super::OnInitDialog();	

	//��������
	m_InputChat.LimitText(TRUMPET_MAX_CHAR);

	return TRUE;
}


//��ɫ��ť
void CDlgTrumpet::OnBnClickedColorSet()
{
	//������ɫ
	CColorDialog ColorDialog(m_crChatTX,CC_FULLOPEN);
	
	if (ColorDialog.DoModal()==IDOK)
	{
		m_crChatTX=ColorDialog.GetColor();

		//���ý���
		m_InputChat.SetFocus();
	}

	return;
}

//���鰴ť
void CDlgTrumpet::OnBnClickedExpression()
{
	//�����ؼ�
	if (m_pExpressionControl==NULL) m_pExpressionControl = new CExpressionControl;

	//���Ҷ���
	CControlUI * pButtonExpression = GetControlByName(szButtonExpressionControlName);
	if(pButtonExpression==NULL) return;

	//�ƶ�����
	CRect rcButton = pButtonExpression->GetPos();
	ClientToScreen(&rcButton);

	//��ʾ����
	m_pExpressionControl->ShowExpression(this,rcButton.left,rcButton.top,this);
	m_pExpressionControl->SetFocus();

	return;
}

//���Ȱ�ť
void CDlgTrumpet::OnBnClickedTrumpet()
{
	//���߹���
	CGamePropertyManager * pGamePropertyManager = CGamePropertyManager::GetInstance();

	//��ȡ����
	CGamePropertyBase * pPropertyItem = pGamePropertyManager->GetPropertyItem(PROPERTY_ID_TRUMPET); 
	if(pPropertyItem==NULL) return;
	tagPropertyItem * pPropertyInfo = pPropertyItem->GetPropertyItem();

	//���Ҷ���
	CTextUI * pTextDescribe = (CTextUI*)GetControlByName(szLabelBuyDescribeControlName);
	if(pTextDescribe==NULL) return;

	//��������
	TCHAR szDescribe[128]=TEXT("");
	_sntprintf(szDescribe,CountArray(szDescribe),TEXT("ʣ��С����<c #FF6600>%d</c>����������<c #FF6600>%d</c>����"), m_dwTrumpetCount, m_dwTyphonCount,TRUMPET_MAX_CHAR);	

	//�����ı�
	pTextDescribe->SetText(szDescribe);

	//���¿ؼ�
	UpdateControlSurface();
}

//���Ȱ�ť
void CDlgTrumpet::OnBnClickedTyphon()
{
	//���߹���
	CGamePropertyManager * pGamePropertyManager = CGamePropertyManager::GetInstance();

	//��ȡ����
	CGamePropertyBase * pPropertyItem = pGamePropertyManager->GetPropertyItem(PROPERTY_ID_TYPHON); 
	if(pPropertyItem==NULL) return;
	tagPropertyItem * pPropertyInfo = pPropertyItem->GetPropertyItem();

	//���Ҷ���
	CTextUI * pTextDescribe = (CTextUI*)GetControlByName(szLabelBuyDescribeControlName);
	if(pTextDescribe==NULL) return;

	//��������
	TCHAR szDescribe[128]=TEXT("");
	_sntprintf(szDescribe,CountArray(szDescribe),TEXT("ʣ��С����<c #FF6600>%d</c>����������<c #FF6600>%d</c>����"), m_dwTrumpetCount, m_dwTyphonCount,TRUMPET_MAX_CHAR);	

	//���ÿؼ�
	pTextDescribe->SetText(szDescribe);

	//���¿ؼ�
	UpdateControlSurface();
}

//������ť
void CDlgTrumpet::OnBnClickedBag()
{
	OnCancel();
	if(m_pGameTrumpetSink) m_pGameTrumpetSink->PerformShowBag();
}

//�̳ǰ�ť
void CDlgTrumpet::OnBnClickedShop()
{
	OnCancel();
	if(m_pGameTrumpetSink) m_pGameTrumpetSink->PerformShowShop();
}

//��ʼ�ؼ�
VOID CDlgTrumpet::InitControlUI()
{
	__super::InitControlUI();

	//���Ҷ���
	CButtonUI * pButtonConfirm = static_cast<CButtonUI *>(GetControlByName(szButtonConfirmControlName));
	if(pButtonConfirm)
	{
		pButtonConfirm->SetWantReturn();
	}
}

//��Ϣ����
VOID CDlgTrumpet::Notify(TNotifyUI &  msg)
{
	__super::Notify(msg);
	
	//��ȡ����
	CControlUI * pControlUI = msg.pSender;

	//����¼�
	if (lstrcmp(msg.sType, TEXT("click")) == 0)
	{
		
		if(lstrcmp(pControlUI->GetName(), szButtonCancelControlName)==0) 
		{
			return OnCancel();
		}
		else if(lstrcmp(pControlUI->GetName(), szButtonConfirmControlName)==0) 
		{
			return OnOK();
		}
		else if(lstrcmp(pControlUI->GetName(), szButtonColorControlName)==0) 
		{
			return OnBnClickedColorSet();
		}
		else if(lstrcmp(pControlUI->GetName(), szButtonExpressionControlName)==0) 
		{
			return OnBnClickedExpression();
		}
		else if(lstrcmp(pControlUI->GetName(), szRadioButtonTyphonControlName)==0) 
		{
			return OnBnClickedTyphon();
		}
		else if(lstrcmp(pControlUI->GetName(), szRadioButtonTrumpetControlName)==0) 
		{
			return OnBnClickedTrumpet();
		}
		else if(pControlUI->GetName() == szButtonBagControlName)
		{
			return OnBnClickedBag();
		}
		else if(pControlUI->GetName() == szButtonShopControlName)
		{
			return OnBnClickedShop();
		}
	}
}

//�滭��Ϣ
VOID CDlgTrumpet::OnDrawClientAreaEnd(CDC * pDC, INT nWidth, INT nHeight)
{
	//�滭�߿�
	CRect rcWindowRect(0,0,0,0);
	m_InputChat.GetWindowRect(&rcWindowRect);

	//��������
	ScreenToClient(&rcWindowRect);
	rcWindowRect.InflateRect(1,1,1,1);

	//���Ʊ߿�
	pDC->MoveTo(rcWindowRect.left,rcWindowRect.top);
	pDC->LineTo(rcWindowRect.right,rcWindowRect.top);
	pDC->MoveTo(rcWindowRect.right,rcWindowRect.top);
	pDC->LineTo(rcWindowRect.right,rcWindowRect.bottom);
	pDC->MoveTo(rcWindowRect.right,rcWindowRect.bottom);
	pDC->LineTo(rcWindowRect.left,rcWindowRect.bottom);
	pDC->MoveTo(rcWindowRect.left,rcWindowRect.bottom);
	pDC->LineTo(rcWindowRect.left,rcWindowRect.top);
}

//ѡ���¼�
VOID CDlgTrumpet::OnExpressionSelect(CExpression * pExpression, tagExpressionInfo * pExpressionInfo)
{
	if (pExpressionInfo!=NULL)
	{
		CString strChatMsg;
		m_InputChat.GetWindowText(strChatMsg);
		strChatMsg+="/";
		strChatMsg+=pExpressionInfo->szExpression[1];
		m_InputChat.SetWindowText(strChatMsg);
		m_InputChat.SetFocus();
		m_InputChat.SetSel(strChatMsg.GetLength(),strChatMsg.GetLength());
	}
}

//���ýӿ�
bool CDlgTrumpet::SetGameTrumpetSink(IUnknownEx * pIUnknownEx)
{
	//���ýӿ�
	if (pIUnknownEx!=NULL)
	{
		//��ѯ�ӿ�
		ASSERT(QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,IGamePropertyUseSink)!=NULL);
		m_pGameTrumpetSink=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,IGamePropertyUseSink);

		//�ɹ��ж�
		if (m_pGameTrumpetSink==NULL) return false;
	}
	else m_pGameTrumpetSink=NULL;

	return true;
}

//��ʾ����
void CDlgTrumpet::ShowTrumpetWindow(CWnd * pParentWnd,IClientUserItem  * pIClientUserItem)
{
	//Ч��״̬
	ASSERT(pIClientUserItem!=NULL);
	if (pIClientUserItem==NULL) return;

	//״̬�ж�
	if(m_hWnd!=NULL && IsWindowVisible()==TRUE) return;

	//��鴰��
	if(m_hWnd==NULL) 
	{
		//���ýӿ�
		m_pIClientUserItem = pIClientUserItem;

		//��������
		AfxSetResourceHandle(GetModuleHandle(GAME_PROPERTY_DLL_NAME));
		Create(IDD_DLG_TRUMPET,pParentWnd);
		AfxSetResourceHandle(GetModuleHandle(NULL));		
	}

	//��ȡλ��
	CRect rcParentRect(0,0,0,0);
	pParentWnd->GetWindowRect(&rcParentRect);
	
	//��ȡ����
	CRect rcWindowRect(0,0,0,0);
	GetWindowRect(&rcWindowRect);

	//����λ��
	INT nXPos = (rcParentRect.Width()-rcWindowRect.Width())/2;
	INT nYPos = (rcParentRect.Height()-rcWindowRect.Height())/2;
	//SetWindowPos(&CWnd::wndTopMost,nXPos,nYPos,rcWindowRect.Width(),rcWindowRect.Height(),SWP_NOZORDER);
	CenterWindow(pParentWnd);
	//��ʾ����
	ShowWindow(SW_SHOW);

	//��������
	CGamePropertyManager * pGamePropertyManager = CGamePropertyManager::GetInstance();
	CGamePropertyBase * pGamePropertyTrumpet = pGamePropertyManager->GetPropertyItem(PROPERTY_ID_TRUMPET);
	CGamePropertyBase * pGamePropertyTyphon  = pGamePropertyManager->GetPropertyItem(PROPERTY_ID_TYPHON);

	//���Ҷ���
	CRadioButtonUI * pButtonTrumpet = static_cast<CRadioButtonUI *>(GetControlByName(szRadioButtonTrumpetControlName));
	if(pButtonTrumpet==NULL) return;

	//���Ҷ���
	CRadioButtonUI * pButtonTyphon = static_cast<CRadioButtonUI *>(GetControlByName(szRadioButtonTyphonControlName));
	if(pButtonTyphon==NULL) return;

	//Ĭ������
	if(pGamePropertyTrumpet!=NULL && pGamePropertyTyphon==NULL)
	{
		pButtonTrumpet->SetCheck();
		pButtonTrumpet->SetEnabled(true);
		pButtonTyphon->SetCheck(false);
		pButtonTyphon->SetEnabled(false);
		OnBnClickedTrumpet();
	}

	//Ĭ������
	if(pGamePropertyTrumpet==NULL && pGamePropertyTyphon!=NULL)
	{
		pButtonTrumpet->SetCheck(false);
		pButtonTrumpet->SetEnabled(false);
		pButtonTyphon->SetCheck(true);
		pButtonTyphon->SetEnabled(true);
		OnBnClickedTyphon();
	}

	//Ĭ������
	if(pGamePropertyTrumpet!=NULL && pGamePropertyTyphon!=NULL)
	{
		pButtonTrumpet->SetCheck(true);
		pButtonTrumpet->SetEnabled(true);
		pButtonTyphon->SetCheck(false);
		pButtonTyphon->SetEnabled(true);
		OnBnClickedTrumpet();
	}

	//���ñ���
	m_strInuptChat=TEXT("");
	UpdateData(FALSE);

	//���½���
	UpdateControlSurface();
}

void CDlgTrumpet::SetTrumpet(const DWORD dwTrumpetCount)
{
	m_dwTrumpetCount=dwTrumpetCount;
}

void CDlgTrumpet::SetTyphon(const DWORD dwTyphonCount)
{
	m_dwTyphonCount=dwTyphonCount;
}
//////////////////////////////////////////////////////////////////////

