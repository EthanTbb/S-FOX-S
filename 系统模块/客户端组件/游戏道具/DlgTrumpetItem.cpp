#include "Stdafx.h"
#include "resource.h"
#include "DlgTrumpetItem.h"
#include "GamePropertyManager.h"

//////////////////////////////////////////////////////////////////////
//控件定义
//按钮控件
const TCHAR * szButtonColorControlName = TEXT("ButtonColor");
const TCHAR * szButtonCancelControlName = TEXT("ButtonClose");
const TCHAR * szButtonConfirmControlName = TEXT("ButtonConfirm");
const TCHAR * szButtonExpressionControlName = TEXT("ButtonExpression");
const TCHAR * szButtonBagControlName = TEXT("ButtonBag");
const TCHAR * szButtonShopControlName = TEXT("ButtonShop");

//标签控件
const TCHAR * szLabelBuyDescribeControlName = TEXT("LabelDescribe");

//扩展控件
const TCHAR * szRadioButtonTyphonControlName = TEXT("RadioButtonTyphon");
const TCHAR * szRadioButtonTrumpetControlName = TEXT("RadioButtonTrumpet");

//编辑框
const TCHAR* szEditTextInputControlName = TEXT("EditTextInput");

//////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CDlgTrumpetItem, CFGuiDialog)
	ON_WM_TIMER()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////

//构造函数
CDlgTrumpetItem::CDlgTrumpetItem() : CFGuiDialog(IDD_DLG_TRUMPET)
{
	//设置变量
	m_crChatTX = RGB(255,255,255);
	m_wRadioSelected=0;
	m_dwTrumpetCount=0;
	m_dwTyphonCount=0;
   
	//接口变量
	m_pGameTrumpetSink=NULL;
	m_pIClientUserItem=NULL;

	//控件变量
	m_pExpressionControl=NULL;
}

//析构函数
CDlgTrumpetItem::~CDlgTrumpetItem()
{
	//销毁窗口
	if(m_pExpressionControl && IsWindow(m_pExpressionControl->m_hWnd)) m_pExpressionControl->DestroyWindow();

	//删除对象
	SafeDelete(m_pExpressionControl);
}

//控件绑定
void CDlgTrumpetItem::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_InputChat, m_InputChat);
	DDX_Text(pDX, IDC_InputChat, m_strInuptChat);
	DDV_MaxChars(pDX, m_strInuptChat, TRUMPET_MAX_CHAR);

	return;
}

//更新数据
void  CDlgTrumpetItem::UpdateControlSurface()
{
	//变量判断
	if(m_pIClientUserItem==NULL) return;

	//道具管理
	CGamePropertyManager * pGamePropertyManager = CGamePropertyManager::GetInstance();

	//查找对象
	CRadioButtonUI * pButtonTrumpet = static_cast<CRadioButtonUI *>(GetControlByName(szRadioButtonTrumpetControlName));
	if(pButtonTrumpet==NULL) return;

	//会员折扣
	WORD wDiscount = 0;
	//格式字符
	CString strTitle ;
	if(pButtonTrumpet->GetCheck()==true)
	{
		//获取折扣


		//获取道具
		CGamePropertyBase * pPropertyItem = pGamePropertyManager->GetPropertyItem(PROPERTY_ID_TRUMPET); 
		if(pPropertyItem==NULL) return;
		tagPropertyItem * pPropertyInfo = pPropertyItem->GetPropertyItem();

		//计算价格
		DOUBLE lPrice=(DOUBLE)pPropertyInfo->lPropertyGold;
		if(m_pIClientUserItem->GetMemberOrder()>0) lPrice=lPrice*wDiscount/100;

		//购买数目
		TCHAR szBuyCountInfo[32]=TEXT("");
		DWORD dwCanBuyCount= __max((DWORD)(m_pIClientUserItem->GetUserInsure()/lPrice),0);	

		//设置文字
		strTitle.Format(TEXT("您能购买的小喇叭数目为[ %d ] 条"), dwCanBuyCount);       
	}
	else
	{
		//获取道具
		CGamePropertyBase * pPropertyItem = pGamePropertyManager->GetPropertyItem(PROPERTY_ID_TYPHON); 
		if(pPropertyItem==NULL) return;
		tagPropertyItem * pPropertyInfo = pPropertyItem->GetPropertyItem();

		//计算价格
		DOUBLE lPrice=(DOUBLE)pPropertyInfo->lPropertyGold;
		if(m_pIClientUserItem->GetMemberOrder()>0) lPrice=lPrice*wDiscount/100;

		//购买数目
		TCHAR szBuyCountInfo[32]=TEXT("");
		DWORD dwCanBuyCount= __max((DWORD)(m_pIClientUserItem->GetUserInsure()/lPrice),0);	

		//设置文字
		strTitle.Format(TEXT("您能购买的大喇叭数目为[ %d ] 条"), dwCanBuyCount);		
	}

	//设置标题
	//SetWindowText(strTitle);
}

//获取内容
void CDlgTrumpetItem::GetTrumpetContent(LPTSTR pszTrumpetContent)
{
	//参数判断
	if(pszTrumpetContent==NULL) return;

	//拷贝数据
	CopyMemory(pszTrumpetContent,m_strInuptChat,sizeof(TCHAR)*TRUMPET_MAX_CHAR);
}

//确定函数
VOID CDlgTrumpetItem::OnOK()
{
	//接口判断
	if(m_pGameTrumpetSink==NULL) return; 

	//查找对象
	CRadioButtonUI * pButtonTrumpet = static_cast<CRadioButtonUI *>(GetControlByName(szRadioButtonTrumpetControlName));
	if(pButtonTrumpet==NULL) return;

	//获取道具
	WORD wPropertyIndex = pButtonTrumpet->GetCheck()?PROPERTY_ID_TRUMPET:PROPERTY_ID_TYPHON;

	if(wPropertyIndex == PROPERTY_ID_TRUMPET && m_dwTrumpetCount <= 0)
	{
		CInformation Information2(this);
		Information2.ShowMessageBox(TEXT("您没有小喇叭，可以通过商城购买哦！"),MB_ICONWARNING);
		return;
	}

	if(wPropertyIndex == PROPERTY_ID_TYPHON && m_dwTyphonCount <= 0)
	{
		CInformation Information2(this);
		Information2.ShowMessageBox(TEXT("您没有大喇叭，可以通过商城购买哦！"),MB_ICONWARNING);
		return;
	}

	//获取信息
	m_InputChat.GetWindowText(m_strInuptChat);
	if ( m_strInuptChat.IsEmpty() )
	{
		CInformation Information2(this);
		Information2.ShowMessageBox(TEXT("输入内容不能为空！"),MB_ICONWARNING);
		return ;
	}

	if ( TRUMPET_MAX_CHAR < m_strInuptChat.GetLength() )
	{ 
		CString strMsg;
		strMsg.Format(TEXT("输入内容最多为%d个字符！"), TRUMPET_MAX_CHAR);

		CInformation Information2(this);
		Information2.ShowMessageBox(TEXT("输入内容最多为%d个字符！"),TRUMPET_MAX_CHAR,MB_ICONWARNING);
		return ;
	}	

	//回调接口
	if(m_pGameTrumpetSink->OnEventUseProperty(wPropertyIndex))
	{
		//关闭窗口
		DestroyWindow();
	}
}

//取消消息
VOID CDlgTrumpetItem::OnCancel()
{
	//关闭窗口
	DestroyWindow();

	return;
}

//初始框架
BOOL CDlgTrumpetItem::OnInitDialog()
{
	__super::OnInitDialog();	

	//输入限制
	m_InputChat.LimitText(TRUMPET_MAX_CHAR);

	return TRUE;
}


//颜色按钮
void CDlgTrumpetItem::OnBnClickedColorSet()
{
	//设置颜色
	CColorDialog ColorDialog(m_crChatTX,CC_FULLOPEN);
	
	if (ColorDialog.DoModal()==IDOK)
	{
		m_crChatTX=ColorDialog.GetColor();

		//设置界面
		m_InputChat.SetFocus();
	}

	return;
}

//表情按钮
void CDlgTrumpetItem::OnBnClickedExpression()
{
	//创建控件
	if (m_pExpressionControl==NULL) m_pExpressionControl = new CExpressionControl;

	//查找对象
	CControlUI * pButtonExpression = GetControlByName(szButtonExpressionControlName);
	if(pButtonExpression==NULL) return;

	//移动窗口
	CRect rcButton = pButtonExpression->GetPos();
	ClientToScreen(&rcButton);

	//显示窗口
	m_pExpressionControl->ShowExpression(this,rcButton.left,rcButton.top,this);
	m_pExpressionControl->SetFocus();

	return;
}

//喇叭按钮
void CDlgTrumpetItem::OnBnClickedTrumpet()
{
	//道具管理
	CGamePropertyManager * pGamePropertyManager = CGamePropertyManager::GetInstance();

	//获取道具
	CGamePropertyBase * pPropertyItem = pGamePropertyManager->GetPropertyItem(PROPERTY_ID_TRUMPET); 
	if(pPropertyItem==NULL) return;
	tagPropertyItem * pPropertyInfo = pPropertyItem->GetPropertyItem();

	//查找对象
	CTextUI * pTextDescribe = (CTextUI*)GetControlByName(szLabelBuyDescribeControlName);
	if(pTextDescribe==NULL) return;

	//构造描述
	TCHAR szDescribe[128]=TEXT("");
	_sntprintf(szDescribe,CountArray(szDescribe),TEXT("剩余小喇叭<c #FF6600>%d</c>个，大喇叭<c #FF6600>%d</c>个。"), m_dwTrumpetCount, m_dwTyphonCount,TRUMPET_MAX_CHAR);	

	//设置文本
	pTextDescribe->SetText(szDescribe);

	//更新控件
	UpdateControlSurface();
}

//喇叭按钮
void CDlgTrumpetItem::OnBnClickedTyphon()
{
	//道具管理
	CGamePropertyManager * pGamePropertyManager = CGamePropertyManager::GetInstance();

	//获取道具
	CGamePropertyBase * pPropertyItem = pGamePropertyManager->GetPropertyItem(PROPERTY_ID_TYPHON); 
	if(pPropertyItem==NULL) return;
	tagPropertyItem * pPropertyInfo = pPropertyItem->GetPropertyItem();

	//查找对象
	CTextUI * pTextDescribe = (CTextUI*)GetControlByName(szLabelBuyDescribeControlName);
	if(pTextDescribe==NULL) return;

	//构造描述
	TCHAR szDescribe[128]=TEXT("");
	_sntprintf(szDescribe,CountArray(szDescribe),TEXT("剩余小喇叭<c #FF6600>%d</c>个，大喇叭<c #FF6600>%d</c>个。"), m_dwTrumpetCount, m_dwTyphonCount,TRUMPET_MAX_CHAR);	

	//设置控件
	pTextDescribe->SetText(szDescribe);

	//更新控件
	UpdateControlSurface();
}

//背包按钮
void CDlgTrumpetItem::OnBnClickedBag()
{
	OnCancel();
	if(m_pGameTrumpetSink) m_pGameTrumpetSink->PerformShowBag();
}

//商城按钮
void CDlgTrumpetItem::OnBnClickedShop()
{
	OnCancel();
	if(m_pGameTrumpetSink) m_pGameTrumpetSink->PerformShowShop();
}

//初始控件
VOID CDlgTrumpetItem::InitControlUI()
{
	__super::InitControlUI();

	//查找对象
	CButtonUI * pButtonConfirm = static_cast<CButtonUI *>(GetControlByName(szButtonConfirmControlName));
	if(pButtonConfirm)
	{
		pButtonConfirm->SetWantReturn();
	}

	//查找对象
	CControlUI * pControl = GetControlByName(szButtonBagControlName);
	if(pControl) pControl->SetVisible(false);

	//查找对象
	pControl = GetControlByName(szButtonShopControlName);
	if(pControl) pControl->SetVisible(false);
}

//消息提醒
VOID CDlgTrumpetItem::Notify(TNotifyUI &  msg)
{
	__super::Notify(msg);
	
	//获取对象
	CControlUI * pControlUI = msg.pSender;

	//点击事件
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

//绘画消息
VOID CDlgTrumpetItem::OnDrawClientAreaEnd(CDC * pDC, INT nWidth, INT nHeight)
{
	//绘画边框
	CRect rcWindowRect(0,0,0,0);
	m_InputChat.GetWindowRect(&rcWindowRect);

	//区域膨胀
	ScreenToClient(&rcWindowRect);
	rcWindowRect.InflateRect(1,1,1,1);

	//绘制边框
	pDC->MoveTo(rcWindowRect.left,rcWindowRect.top);
	pDC->LineTo(rcWindowRect.right,rcWindowRect.top);
	pDC->MoveTo(rcWindowRect.right,rcWindowRect.top);
	pDC->LineTo(rcWindowRect.right,rcWindowRect.bottom);
	pDC->MoveTo(rcWindowRect.right,rcWindowRect.bottom);
	pDC->LineTo(rcWindowRect.left,rcWindowRect.bottom);
	pDC->MoveTo(rcWindowRect.left,rcWindowRect.bottom);
	pDC->LineTo(rcWindowRect.left,rcWindowRect.top);
}

//选择事件
VOID CDlgTrumpetItem::OnExpressionSelect(CExpression * pExpression, tagExpressionInfo * pExpressionInfo)
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

//设置接口
bool CDlgTrumpetItem::SetGameTrumpetSink(IUnknownEx * pIUnknownEx)
{
	//设置接口
	if (pIUnknownEx!=NULL)
	{
		//查询接口
		ASSERT(QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,IGamePropertyUseSink)!=NULL);
		m_pGameTrumpetSink=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,IGamePropertyUseSink);

		//成功判断
		if (m_pGameTrumpetSink==NULL) return false;
	}
	else m_pGameTrumpetSink=NULL;

	return true;
}

//显示窗口
void CDlgTrumpetItem::ShowTrumpetWindow(CWnd * pParentWnd,IClientUserItem  * pIClientUserItem)
{
	//效验状态
	ASSERT(pIClientUserItem!=NULL);
	if (pIClientUserItem==NULL) return;

	//状态判断
	if(m_hWnd!=NULL && IsWindowVisible()==TRUE) return;

	//检查窗口
	if(m_hWnd==NULL) 
	{
		//设置接口
		m_pIClientUserItem = pIClientUserItem;

		//创建窗口
		AfxSetResourceHandle(GetModuleHandle(GAME_PROPERTY_DLL_NAME));
		Create(IDD_DLG_TRUMPET,pParentWnd);
		AfxSetResourceHandle(GetModuleHandle(NULL));		
	}

	//获取位置
	CRect rcParentRect(0,0,0,0);
	pParentWnd->GetWindowRect(&rcParentRect);
	
	//获取区域
	CRect rcWindowRect(0,0,0,0);
	GetWindowRect(&rcWindowRect);

	//设置位置
	INT nXPos = (rcParentRect.Width()-rcWindowRect.Width())/2;
	INT nYPos = (rcParentRect.Height()-rcWindowRect.Height())/2;
	//SetWindowPos(&CWnd::wndTopMost,nXPos,nYPos,rcWindowRect.Width(),rcWindowRect.Height(),SWP_NOZORDER);
	CenterWindow(pParentWnd);
	//显示窗口
	ShowWindow(SW_SHOW);

	//变量定义
	CGamePropertyManager * pGamePropertyManager=CGamePropertyManager::GetInstance();
	CGamePropertyBase * pGamePropertyTrumpet = pGamePropertyManager->GetPropertyItem(PROPERTY_ID_TRUMPET);
	CGamePropertyBase * pGamePropertyTyphon  = pGamePropertyManager->GetPropertyItem(PROPERTY_ID_TYPHON);

	//查找对象
	CRadioButtonUI * pButtonTrumpet = static_cast<CRadioButtonUI *>(GetControlByName(szRadioButtonTrumpetControlName));
	if(pButtonTrumpet==NULL) return;

	//查找对象
	CRadioButtonUI * pButtonTyphon = static_cast<CRadioButtonUI *>(GetControlByName(szRadioButtonTyphonControlName));
	if(pButtonTyphon==NULL) return;

	//默认喇叭
	if(pGamePropertyTrumpet!=NULL && pGamePropertyTyphon==NULL)
	{
		pButtonTrumpet->SetCheck();
		pButtonTrumpet->SetEnabled(true);
		pButtonTyphon->SetCheck(false);
		pButtonTyphon->SetEnabled(false);
		OnBnClickedTrumpet();
	}

	//默认喇叭
	if(pGamePropertyTrumpet==NULL && pGamePropertyTyphon!=NULL)
	{
		pButtonTrumpet->SetCheck(false);
		pButtonTrumpet->SetEnabled(false);
		pButtonTyphon->SetCheck(true);
		pButtonTyphon->SetEnabled(true);
		OnBnClickedTyphon();
	}

	//默认喇叭
	if(pGamePropertyTrumpet!=NULL && pGamePropertyTyphon!=NULL)
	{
		pButtonTrumpet->SetCheck(true);
		pButtonTrumpet->SetEnabled(true);
		pButtonTyphon->SetCheck(false);
		pButtonTyphon->SetEnabled(true);
		OnBnClickedTrumpet();
	}

	//设置变量
	m_strInuptChat=TEXT("");
	UpdateData(FALSE);

	//更新界面
	UpdateControlSurface();
}

void CDlgTrumpetItem::SetTrumpet(const DWORD dwTrumpetCount)
{
	m_dwTrumpetCount=dwTrumpetCount;
}

void CDlgTrumpetItem::SetTyphon(const DWORD dwTyphonCount)
{
	m_dwTyphonCount=dwTyphonCount;
}
//////////////////////////////////////////////////////////////////////

