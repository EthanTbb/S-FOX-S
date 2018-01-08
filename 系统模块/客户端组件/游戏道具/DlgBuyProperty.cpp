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
//控件定义

//按钮控件
const TCHAR * szButtonBuyPropertyControlName = TEXT("ButtonBuyProperty");
const TCHAR * szButtonClosePropertyControlName = TEXT("ButtonCloseProperty");

//文本控件
const TCHAR * szTextPropertyNameControlName = TEXT("TextPropertyName");
const TCHAR * szTextPropertyPriceControlName = TEXT("TextPropertyPrice");

//标签控件
const TCHAR * szLabelAmountControlName = TEXT("LabelAmount");
const TCHAR * szLabelDescribeControlName = TEXT("LabelDescribe");
const TCHAR * szLabelPropertyNameControlName = TEXT("LabelPropertyName");
const TCHAR * szLabelPropertyPriceControlName = TEXT("LabelPropertyPrice");

//编辑控件
const TCHAR * szEditAmountControlName = TEXT("EditAmount"); 
const TCHAR * szEditNickNameControlName = TEXT("EditNickName"); 

//控件控件
const TCHAR * szContainerRoundControlName = TEXT("ContainerRound"); 
const TCHAR * szRadioButtonInsureControlName = TEXT("RadioButtonInsure"); 

//////////////////////////////////////////////////////////////////////////////////

//静态变量
CDlgBuyProperty * CDlgBuyProperty::m_pDlgBuyProperty=NULL;				//对象指针

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CDlgBuyProperty, CSkinDialog)
	ON_EN_CHANGE(IDC_PROPERTY_COUNT, OnEnChangePropertyCount)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CDlgBuyProperty::CDlgBuyProperty() : CSkinDialog(IDD_DLG_PROPERTY)
{
	//设置变量
	m_lCanBuyCount=0;

	//设置变量
	m_pGamePropertyItem=NULL;
	m_pIGamePropertySink=NULL;

	//设置对象
	ASSERT(m_pDlgBuyProperty==NULL);
	if (m_pDlgBuyProperty==NULL) m_pDlgBuyProperty=this;

	return;
}

//析构函数
CDlgBuyProperty::~CDlgBuyProperty()
{
	//设置对象
	ASSERT(m_pDlgBuyProperty==this);
	if (m_pDlgBuyProperty==this) m_pDlgBuyProperty=NULL;

	return;
}

//消息解释
BOOL CDlgBuyProperty::PreTranslateMessage(MSG * pMsg)
{
	//按键过虑
	if ((pMsg->message==WM_KEYDOWN)&&(pMsg->wParam==VK_ESCAPE))
	{
		return TRUE;
	}

	return __super::PreTranslateMessage(pMsg);
}

//创建函数
BOOL CDlgBuyProperty::OnInitDialog()
{
	__super::OnInitDialog();

	//更新状态
	UpdateCtrlStatus();

	//设置窗口
	SetWindowPos(NULL,0,0,490,370,SWP_NOZORDER|SWP_NOMOVE);

	return FALSE;
}

//确定函数
VOID CDlgBuyProperty::OnOK()
{
	//查找控件
	CControlUI * pEditNickName=GetControlByName(szEditNickNameControlName);
	if(pEditNickName==NULL) return;

	//查找控件
	CControlUI * pEditAmount=GetControlByName(szEditAmountControlName);
	if(pEditAmount==NULL) return;

	//获取信息
	CString strNickName=pEditNickName->GetText();
	strNickName.TrimLeft();strNickName.TrimRight();

	//获取数目
	UINT wItemCount=_tcstol(pEditAmount->GetText(),NULL,10);

	//昵称判断
	if (strNickName.IsEmpty()==true)
	{
		//提示信息
		CInformation Information(this);
		Information.ShowMessageBox(TEXT("用户昵称不正确，请重新输入"),MB_ICONERROR);

		//设置焦点
		pEditNickName->SetFocus();

		return;
	}

	//数目判断
	if (wItemCount==0 || wItemCount > m_lCanBuyCount)
	{
		//提示信息
		CInformation Information(this);
		Information.ShowMessageBox(TEXT("购买数目不正确，请重新输入"),MB_ICONERROR);

		//设置焦点
		pEditAmount->SetFocus();

		return;
	}

	//获取信息
	ASSERT(m_pGamePropertyItem->GetPropertyAttrib()!=NULL);
	tagPropertyAttrib * pPropertyAttrib=m_pGamePropertyItem->GetPropertyAttrib();

	//购前检查
	ASSERT(m_pIGamePropertySink!=NULL);
	TCHAR szMessage[128]=TEXT("");
	if(m_pIGamePropertySink->OnEventBuyPropertyPrep(strNickName,pPropertyAttrib->wIndex,szMessage)==false)
	{
		//提示信息
		CInformation Information(this);
		Information.ShowMessageBox(szMessage,MB_ICONERROR);

		return;
	}

	//事件通知	
	if (m_pIGamePropertySink->OnEventBuyProperty(strNickName,wItemCount,pPropertyAttrib->wIndex)==true)
	{
		//关闭窗口
		DestroyWindow();
	}

	return;
}

//取消消息
VOID CDlgBuyProperty::OnCancel()
{
	//关闭窗口
	DestroyWindow();

	return;
}

//初始控件
VOID CDlgBuyProperty::InitControlUI()
{
	__super::InitControlUI();

	//用户昵称
	CEditUI * pEditNickName=static_cast<CEditUI*>(GetControlByName(szEditNickNameControlName));
	if(pEditNickName)
	{
		pEditNickName->SetMaxChar(LEN_NICKNAME-1);	
	}

	//购买数量
	CEditUI * pEditAmount=static_cast<CEditUI*>(GetControlByName(szEditAmountControlName));
	if(pEditAmount) 
	{
		pEditAmount->SetMaxChar(12);	
		pEditAmount->SetNumberOnly(true);
	}

	//功能描述
	CLabelUI * pLabelDescribe=static_cast<CLabelUI *>(GetControlByName(szLabelDescribeControlName));
	if(pLabelDescribe)
	{
		pLabelDescribe->SetTextStyle(DT_LEFT|DT_TOP|DT_WORDBREAK);
	}

	//支付方式
	CRadioButtonUI * pRedioButtonInsure=static_cast<CRadioButtonUI *>(GetControlByName(szRadioButtonInsureControlName));
	if(pRedioButtonInsure)
	{
		pRedioButtonInsure->SetCheck(true);
	}
	
}

//消息提醒
VOID CDlgBuyProperty::Notify(TNotifyUI &  msg)
{
	__super::Notify(msg);

	//获取对象
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

//绘画消息
VOID CDlgBuyProperty::OnDrawClientAreaEnd(CDC * pDC, INT nWidth, INT nHeight)
{
	//框架位置
	INT nTBorder=m_SkinAttribute.m_EncircleInfoFrame.nTBorder;
	INT nBBorder=m_SkinAttribute.m_EncircleInfoFrame.nBBorder;
	INT nLBorder=m_SkinAttribute.m_EncircleInfoFrame.nLBorder;
	INT nRBorder=m_SkinAttribute.m_EncircleInfoFrame.nRBorder;

	//环绕位置
	CSize SizeRound(0,0);
	CControlUI * pControlRound = GetControlByName(szContainerRoundControlName);
	if(pControlRound) SizeRound = pControlRound->GetFixedXY();

	//加载资源
	CPngImage ImagePropertyFrame;
	ImagePropertyFrame.LoadImage(GetModuleHandle(GAME_PROPERTY_DLL_NAME),TEXT("PROPERTY_FRAME"));

	//获取大小
	CSize SizePropertyFrame;
	SizePropertyFrame.SetSize(ImagePropertyFrame.GetWidth(),ImagePropertyFrame.GetHeight());

	//绘画框架
	INT nXItemPos=SizeRound.cx+15;
	INT nYItemPos=SizeRound.cy+15;
	ImagePropertyFrame.DrawImage(pDC,nXItemPos-(SizePropertyFrame.cx-PROPERTY_SYMBOL_CX)/2,nYItemPos-(SizePropertyFrame.cy-PROPERTY_SYMBOL_CY)/2);

	//绘画图标
	if (m_pGamePropertyItem!=NULL)
	{
		//获取资源
		tagPropertyImage PropertyImage;
		m_pGamePropertyItem->GetImageInfo(PropertyImage);

		//绘画图标
		CPngImage ImageSymbol;
		ImageSymbol.LoadImage(PropertyImage.hResInstance,PropertyImage.pszSymbolID);
		ImageSymbol.DrawImage(pDC,nXItemPos,nYItemPos,PROPERTY_SYMBOL_CX,PROPERTY_SYMBOL_CY,0,0,ImageSymbol.GetWidth(),ImageSymbol.GetHeight());
	}

	return;
}

//更新状态
VOID CDlgBuyProperty::UpdateCtrlStatus()
{
	//效验参数
	ASSERT(m_pGamePropertyItem!=NULL);
	ASSERT(m_pIGamePropertySink!=NULL);

	//获取属性
	tagPropertyInfo * pPropertyInfo=m_pGamePropertyItem->GetPropertyInfo();
	tagPropertyAttrib * pPropertyAttrib=m_pGamePropertyItem->GetPropertyAttrib();

	//获取对象
	CControlUI * pLabelDescribe=GetControlByName(szLabelDescribeControlName);
	if(pLabelDescribe==NULL) return;

	//获取对象
	CControlUI * pLabelAmount=GetControlByName(szLabelAmountControlName);
	if(pLabelAmount==NULL) return;

	//获取对象
	CControlUI * pTextPropertyName=GetControlByName(szTextPropertyNameControlName);
	if(pTextPropertyName==NULL) return;

	//获取对象
	CControlUI * pTextPropertyPrice=GetControlByName(szTextPropertyPriceControlName);
	if(pTextPropertyPrice==NULL) return;

	//获取对象
	CControlUI * pLabelPropertyName=GetControlByName(szLabelPropertyNameControlName);
	if(pLabelPropertyName==NULL) return;

	//获取对象
	CControlUI * pLabelPropertyPrice=GetControlByName(szLabelPropertyPriceControlName);
	if(pLabelPropertyPrice==NULL) return;

	//获取对象
	CEditUI * pEditAmount=static_cast<CEditUI*>(GetControlByName(szEditAmountControlName));
	if(pEditAmount==NULL)  return;

	//设置标签
	if(pPropertyAttrib->wPropertyType==PT_TYPE_PROPERTY)
	{		
		pTextPropertyName->SetText(TEXT("道具名称："));
		pTextPropertyPrice->SetText(TEXT("道具价格："));
	}
	else
	{
		pTextPropertyName->SetText(TEXT("礼物名称："));
		pTextPropertyPrice->SetText(TEXT("礼物价格："));
	}

	
	
	//会员折扣
	WORD wDiscount = 0;


	//道具价格
	TCHAR szPropertyGold[64]=TEXT("");
	_sntprintf(szPropertyGold,CountArray(szPropertyGold),TEXT("%I64d 游戏币/%s （会员折扣：%ld%%）"),
		pPropertyInfo->lPropertyGold,pPropertyAttrib->szMeasuringunit,wDiscount);

	//设置标题
	SetWindowText(pPropertyAttrib->szPropertyName);

	//道具价格
	pLabelPropertyPrice->SetText(szPropertyGold);

	//道具单位
	//SetDlgItemText(IDC_STATIC_COUNT,pPropertyAttrib->szMeasuringunit);

	//道具属性
	pLabelDescribe->SetText(pPropertyAttrib->szRegulationsInfo);
	pLabelPropertyName->SetText(pPropertyAttrib->szPropertyName);	

	//购买数目
	if(m_pIGamePropertySink!=NULL)	
	{
		IClientUserItem * pClientUserItem = m_pIGamePropertySink->OnQueryClientUserItem();
		if(pClientUserItem!=NULL)
		{
			//计算价格
			DOUBLE lPrice=(DOUBLE)pPropertyInfo->lPropertyGold;
			if(pClientUserItem->GetMemberOrder()>0) lPrice=lPrice*wDiscount/100;

			//购买数目
			TCHAR szBuyCountInfo[32]=TEXT("");
			if(pClientUserItem!=NULL) 
			{
				m_lCanBuyCount=__max((LONGLONG)(pClientUserItem->GetUserInsure()/lPrice),0);
				if(pPropertyInfo->wIndex==PROPERTY_ID_SCORE_CLEAR || pPropertyInfo->wIndex==PROPERTY_ID_ESCAPE_CLEAR)
				{
					m_lCanBuyCount=__min(m_lCanBuyCount,1);
				}
			}
			_sntprintf(szBuyCountInfo,CountArray(szBuyCountInfo),TEXT("（您最多能购买 %I64d %s）"),m_lCanBuyCount,pPropertyAttrib->szMeasuringunit);

			pLabelAmount->SetText(szBuyCountInfo);	

			//变量定义
			UINT  wItemCount = _tcstol(pEditAmount->GetText(),NULL,10);
			
			//数目判断
			if(wItemCount>m_lCanBuyCount) 
			{
				//数目设置
				TCHAR szPropertyCount[16]=TEXT("");
				_sntprintf(szPropertyCount,CountArray(szPropertyCount),TEXT("%I64d"),m_lCanBuyCount);
				pEditAmount->SetText(szPropertyCount);
				pEditAmount->SetSel(-1,-1);
			}
		}
	}

	return;
}

//显示窗口
VOID CDlgBuyProperty::ShowBuyPropertyWnd(CGamePropertyBase * pGamePropertyItem, IGamePropertySink * pIGamePropertySink)
{
	//效验参数
	ASSERT(pGamePropertyItem!=NULL);
	ASSERT(pIGamePropertySink!=NULL);

	//变量定义
	CDlgBuyProperty * pDlgBuyProperty=m_pDlgBuyProperty;

	//创建对象
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

	//设置变量
	pDlgBuyProperty->m_pGamePropertyItem=pGamePropertyItem;
	pDlgBuyProperty->m_pIGamePropertySink=pIGamePropertySink;

	//创建窗口
	if (pDlgBuyProperty->m_hWnd==NULL)
	{
		AfxSetResourceHandle(GetModuleHandle(GAME_PROPERTY_DLL_NAME));
		pDlgBuyProperty->Create(IDD_DLG_PROPERTY,AfxGetMainWnd());
		AfxSetResourceHandle(GetModuleHandle(NULL));
	}
	else
	{
		//更新状态
		pDlgBuyProperty->UpdateCtrlStatus();
	}

	//显示窗口
	pDlgBuyProperty->ShowWindow(SW_SHOW);

	//激活窗口
	pDlgBuyProperty->SetActiveWindow();
	pDlgBuyProperty->SetForegroundWindow();

	return;
}

//改变事件
VOID CDlgBuyProperty::OnEventPerformCommand(WORD wCommandID,VOID * pCmdData)
{
	//存在判断
	if(m_pDlgBuyProperty==NULL) return;

	//分别处理
	if(wCommandID==CMD_CLOSE_WND)
	{
		//关闭窗口
		if(m_pDlgBuyProperty->m_hWnd!=NULL) m_pDlgBuyProperty->DestroyWindow();
		SafeDelete(m_pDlgBuyProperty);
	}
	else if(wCommandID==CMD_USER_CHANGE)
	{
		//提取数据
		IClientUserItem * pIClintUserItem=(IClientUserItem *)pCmdData;

		//参数判断
		if(pIClintUserItem==NULL) return;

		//修改昵称
		CControlUI * pEditNickName = m_pDlgBuyProperty->GetControlByName(szEditNickNameControlName);
		if(pEditNickName!=NULL) pEditNickName->SetText(pIClintUserItem->GetNickName());
	}
}

//改变事件
void CDlgBuyProperty::OnEnChangePropertyCount()
{
	//获取对象
	CEditUI * pEditAmount=static_cast<CEditUI*>(GetControlByName(szEditAmountControlName));
	if(pEditAmount==NULL)  return;

	//变量定义	
	CString strCount=pEditAmount->GetText();
	
	//去掉前面的0
	CString strLeft = strCount.Left(1);
	if(strLeft.Find('0') != -1)
	{
		strCount = strCount.Right(strCount.GetLength() - 1);
		pEditAmount->SetText(strCount);
	}

	UINT wItemCount=_tcstol(pEditAmount->GetText(),NULL,10);

	//数目判断
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
