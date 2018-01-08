#include "StdAfx.h"
#include "Resource.h"
#include "PlatformEvent.h"
#include "DlgExchange.h"

////////////////////////////////////////////////////////////////////////////////////////////
//�ؼ�����

//��ť�ؼ�
const TCHAR* const szButtonConfirmExchangeControlName=TEXT("ButtonConfirmExchange");
const TCHAR* const szButtonConfirmPurchaseControlName=TEXT("ButtonConfirmPurchase");

//�༭�ؼ�
const TCHAR* const szEditIngotCountControlName=TEXT("EditIngotCount");
const TCHAR* const szEditPurchaseTimeControlName=TEXT("EditPurchaseTime");

//��Ͽؼ�
const TCHAR* const szComboMemberTypeControlName=TEXT("ComboMemberType");

//�ļ��ؼ�
const TCHAR* const szTextMoreExchangeControlName=TEXT("TextMoreExchange");
const TCHAR* const szTextExchangeRateControlName=TEXT("TextExchangeRate");

////////////////////////////////////////////////////////////////////////////////////////////
//���캯��
CDlgExchange::CDlgExchange(): CSkinDialog(IDD_DLG_POPUP)
{
	//��ʶ����
	m_bLableRound=false;

	//��ʾ����
	m_lExchangeScore=0;
	m_lConsumeBeans=0;
	m_lPresentScore=0;

	//��������
	m_dwExchangeRate=0L;
	m_wMemberCount=0;
	ZeroMemory(&m_MemberParameter,sizeof(m_MemberParameter));
}

//��������
CDlgExchange::~CDlgExchange()
{
}


//��ʼ�ؼ�
void CDlgExchange::InitControlUI()
{
	__super::InitControlUI();

	//��������
	CEditUI * pEditControl = NULL;

	//���ÿؼ�
	pEditControl = static_cast<CEditUI *>(GetControlByName(szEditIngotCountControlName));
	if(pEditControl)
	{
		pEditControl->SetNumberOnly();
		pEditControl->SetIntegerNumber();
		pEditControl->SetMaxChar(12);
	}

	//���ÿؼ�
	CEditUI * pEditPurchaseTime =  static_cast<CEditUI *>(GetControlByName(szEditPurchaseTimeControlName));
	if(pEditPurchaseTime!=NULL)
	{
		pEditPurchaseTime->SetNumberOnly();
		pEditPurchaseTime->SetMaxChar(3);
	}
}

//��Ϣ����
void CDlgExchange::Notify(TNotifyUI &  msg)
{
	__super::Notify(msg);

	//��ȡ����
	CControlUI * pControlUI = msg.pSender;

	if (lstrcmp(msg.sType, TEXT("click")) == 0)
	{
		if (lstrcmp(pControlUI->GetName(),szButtonConfirmExchangeControlName) == 0)
		{
			return OnClickedConfirmExchange();
		}
		else if (lstrcmp(pControlUI->GetName(),szButtonConfirmPurchaseControlName) == 0)
		{
			return OnClickedConfirmPurchase();
		}
	}
	else if(lstrcmp(msg.sType,TEXT("link")) ==0 )
	{
		if(lstrcmp(pControlUI->GetName(),szTextMoreExchangeControlName)==0)
		{
			return OnTextLinkMoreExchange();
		}
	}
	else if(lstrcmp(msg.sType,TEXT("textchanged")) ==0 )
	{	
		if(lstrcmp(pControlUI->GetName(),szEditIngotCountControlName)==0)
		{
			return OnEditIngotTextChange();
		} 
		else if(lstrcmp(pControlUI->GetName(),szEditPurchaseTimeControlName)==0)
		{
			return OnComboControlItemChange();
		}		
	}
	else if(lstrcmp(msg.sType,TEXT("itemselect")) ==0 )
	{
		if(lstrcmp(pControlUI->GetName(), szComboMemberTypeControlName) == 0)
		{
			return OnComboControlItemChange();
		}
	}
}


//��������
BOOL CDlgExchange::OnInitDialog()
{
	__super::OnInitDialog();

	//���д���
	CenterWindow(this);

	//���ز���
	PerformLoadExchangeParam();

	return FALSE;
}

//��Ϣ����
BOOL CDlgExchange::PreTranslateMessage(MSG * pMsg)
{
	//��������
	if (pMsg->message==WM_KEYDOWN)
	{
		if(pMsg->wParam==VK_ESCAPE) return TRUE;
		if(pMsg->wParam==VK_RETURN) return TRUE;
	}

	return __super::PreTranslateMessage(pMsg);
}

//�滭����
VOID CDlgExchange::OnDrawSystemTitle(CDC * pDC, INT nWdith, INT nHeight)
{
	//��Դ���
	HINSTANCE hInstance = GetModuleHandle(PLATFORM_RESOURCE_DLL_NAME);

	//���ر���
	CPngImage ImageTitle;
	ImageTitle.LoadImage(hInstance,TEXT("IMAGE_EXCHANGE_TITLE"));

	//���Ʊ���
	ImageTitle.DrawImage(pDC,10,10);
}

//�滭��Ϣ
VOID CDlgExchange::OnDrawClientAreaEnd(CDC * pDC, INT nWidth, INT nHeight)
{
	//��ȡ����
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	//��ǰԪ��
	TCHAR szCurrIngot[16]=TEXT("");
	CRect rcCurrIngot(93,108,158,121);
	SwitchScoreFormat(pGlobalUserData->lUserIngot,3,szCurrIngot,CountArray(szCurrIngot));
	pDC->DrawText(szCurrIngot,lstrlen(szCurrIngot),rcCurrIngot,DT_LEFT|DT_SINGLELINE|DT_END_ELLIPSIS);

	//�һ���Ϸ��
	TCHAR szExchangeScore[32]=TEXT("");
	CRect rcExchangeScore(93,172,210,185);
	SwitchScoreFormat(m_lExchangeScore,3,szExchangeScore,CountArray(szExchangeScore));
	pDC->DrawText(szExchangeScore,lstrlen(szExchangeScore),rcExchangeScore,DT_LEFT|DT_SINGLELINE|DT_END_ELLIPSIS);

	//������Ϸ��
	TCHAR szConsumeBeans[32]=TEXT("");
	CRect rcConsumeBeans(324,171,404,185);
	SwitchScoreFormat(m_lConsumeBeans,3,szConsumeBeans,CountArray(szConsumeBeans));
	pDC->DrawText(szConsumeBeans,lstrlen(szConsumeBeans),rcConsumeBeans,DT_CENTER|DT_SINGLELINE|DT_END_ELLIPSIS);

	//������Ϸ��
	TCHAR szPresentScore[32]=TEXT("");
	CRect rcPresentScore(324,203,404,217);
	SwitchScoreFormat(m_lPresentScore,3,szPresentScore,CountArray(szPresentScore));
	pDC->DrawText(szPresentScore,lstrlen(szPresentScore),rcPresentScore,DT_CENTER|DT_SINGLELINE|DT_END_ELLIPSIS);
}

//��ʼ����
VOID CDlgExchange::OnMissionStart()
{
	//��������
	CControlUI * pControlUI = NULL; 
	
	//ȷ�϶һ�
	pControlUI=GetControlByName(szButtonConfirmExchangeControlName);
	if(pControlUI!=NULL) pControlUI->SetEnabled(false); 

	//ȷ�Ϲ���
	pControlUI=GetControlByName(szButtonConfirmPurchaseControlName);
	if(pControlUI!=NULL) pControlUI->SetEnabled(false);
}

//��ֹ����
VOID CDlgExchange::OnMissionConclude()
{
	//��������
	CControlUI * pControlUI = NULL; 
	
	//ȷ�϶һ�
	pControlUI=GetControlByName(szButtonConfirmExchangeControlName);
	if(pControlUI!=NULL) pControlUI->SetEnabled(true); 

	//ȷ�Ϲ���
	pControlUI=GetControlByName(szButtonConfirmPurchaseControlName);
	if(pControlUI!=NULL) pControlUI->SetEnabled(true);
}

//ȷ�϶һ�
VOID CDlgExchange::OnClickedConfirmExchange()
{
	//���ҿؼ�
	CEditUI * pControlUI = static_cast<CEditUI *>(GetControlByName(szEditIngotCountControlName));
	if(pControlUI==NULL) return;
	
	//��������
	CString strInputIngot(pControlUI->GetText());	
	SCORE lExchangeIngot = GetUserControlNumber(pControlUI->GetText());

	//�����ж�
	if(lExchangeIngot==0)
	{
		CInformation Information;
		Information.ShowMessageBox(TEXT("������һ�Ԫ��������"));

		//���ý���
		pControlUI->SetFocus();

		return;
	}

	//ִ�жһ�
	return PerformExchangeScore();
}

//ȷ�Ϲ���
VOID CDlgExchange::OnClickedConfirmPurchase()
{
	//���ҿؼ�
	CEditUI * pControlUI = static_cast<CEditUI *>(GetControlByName(szEditPurchaseTimeControlName));
	if(pControlUI==NULL) return;
	
	//��������
	SCORE lPurchaseTime = GetUserControlNumber(pControlUI->GetText());

	//�����ж�
	if(lPurchaseTime==0)
	{
		CInformation Information;
		Information.ShowMessageBox(TEXT("�����빺���Ա����Чʱ�䣡"));

		//���ý���
		pControlUI->SetFocus();

		return;
	}

	//ִ�жһ�
	return PerformPurchaseMember();
}

//����һ�
VOID CDlgExchange::OnTextLinkMoreExchange()
{
	//��ȡ����
	CGlobalWebLink * pGlobalWebLink=CGlobalWebLink::GetInstance();

	//�����ַ
	TCHAR szNavigation[256]=TEXT("");
	_sntprintf(szNavigation,CountArray(szNavigation),TEXT("%s/%sShop/Index.aspx"),pGlobalWebLink->GetPlatformLink(),pGlobalWebLink->GetValidateLink());

	//��ҳ��
	ShellExecute(NULL,TEXT("OPEN"),szNavigation,NULL,NULL,SW_NORMAL);
}

//�ı����
VOID CDlgExchange::OnEditIngotTextChange()
{
	//��ȡ����
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	//���ҿؼ�
	CEditUI * pControlUI = static_cast<CEditUI *>(GetControlByName(szEditIngotCountControlName));
	if(pControlUI==NULL) return;
	
	//��������
	SCORE lExchangeIngot = GetUserControlNumber(pControlUI->GetText());
	if(lExchangeIngot>pGlobalUserData->lUserIngot)
	{
		//���ñ���
		lExchangeIngot=pGlobalUserData->lUserIngot;		

		//��������
		TCHAR szUserIngot[32]=TEXT("");
		_sntprintf(szUserIngot,CountArray(szUserIngot),TEXT("%I64d"),lExchangeIngot);
		pControlUI->SetText(szUserIngot);
	}	

	//�һ����
	m_lExchangeScore = lExchangeIngot*m_dwExchangeRate;

	//ˢ�½���
	RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW);
}

//������
VOID CDlgExchange::OnComboControlItemChange()
{
	//��������
	CComboUI * pComboMemberType=(CComboUI *)GetControlByName(szComboMemberTypeControlName);;
	if(pComboMemberType==NULL) return;

	//��ȡѡ��
	INT nCurrentSel=pComboMemberType->GetCurSel();
	if(nCurrentSel==LB_ERR) return;
	tagMemberParameter * pMemberParameter=(tagMemberParameter *)pComboMemberType->GetItemAt(nCurrentSel)->GetUserData();

	//��������
	CEditUI * pEditPurchaseTime=(CEditUI *)GetControlByName(szEditPurchaseTimeControlName);;
	if(pEditPurchaseTime==NULL) return;

	//��ȡѡ��
	SCORE lPurchaseTime = GetUserControlNumber(pEditPurchaseTime->GetText());

	//���ñ���
	m_lConsumeBeans = pMemberParameter->lMemberPrice*lPurchaseTime;
	m_lPresentScore = pMemberParameter->lPresentScore*lPurchaseTime;

	//ˢ�½���
	RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW);
}

//�һ���Ϸ��
VOID CDlgExchange::PerformExchangeScore()
{	
}

//�����Ա
VOID CDlgExchange::PerformPurchaseMember()
{	
}

//���صͱ�
VOID CDlgExchange::PerformLoadExchangeParam()
{	
}

//��ȡ����
SCORE CDlgExchange::GetUserControlNumber(LPCTSTR pszNumber)
{
	//��������
	WORD wIndex=0;
	SCORE lNumber=0L;

	//��ȡ��ֵ
	TCHAR szBuffer[32]=TEXT("");
	lstrcpyn(szBuffer,pszNumber,CountArray(szBuffer));

	//������ֵ
	while (szBuffer[wIndex]!=0)
	{
		//��������
		WORD wCurrentIndex=wIndex++;

		//������ֵ
		if ((szBuffer[wCurrentIndex]>=TEXT('0'))&&(szBuffer[wCurrentIndex]<=TEXT('9')))
		{
			lNumber=lNumber*10L+(szBuffer[wCurrentIndex]-TEXT('0'));
		}
	}

	return lNumber;
}

//ת���ַ�
VOID CDlgExchange::SwitchScoreFormat(SCORE lScore, UINT uSpace, LPTSTR pszBuffer, WORD wBufferSize)
{
	//ת����ֵ
	TCHAR szSwitchScore[16]=TEXT("");
	_sntprintf(szSwitchScore,CountArray(szSwitchScore),TEXT("%I64d"),lScore);

	//��������
	WORD wTargetIndex=0;
	WORD wSourceIndex=0;
	UINT uSwitchLength=lstrlen(szSwitchScore);

	//ת���ַ�
	while (szSwitchScore[wSourceIndex]!=0)
	{
		//�����ַ�
		pszBuffer[wTargetIndex++]=szSwitchScore[wSourceIndex++];

		//���붺��
		if ((uSwitchLength!=wSourceIndex)&&(((uSwitchLength-wSourceIndex)%uSpace)==0L))
		{
			pszBuffer[wTargetIndex++]=TEXT(',');
		}
	}

	//�����ַ�
	pszBuffer[wTargetIndex++]=0;

	return;
}
////////////////////////////////////////////////////////////////////////////////////////////
//���캯��
CDlgExchangePlaza::CDlgExchangePlaza()
{
	//���Ʊ���
	m_bExchangeScore=false;
	m_bPurchaseMember=false;
	m_bLoadExchangeParam=false;


	//��������
	m_MissionManager.InsertMissionItem(this);
}

//��������
CDlgExchangePlaza::~CDlgExchangePlaza()
{
}

//�����¼�
bool CDlgExchangePlaza::OnEventMissionLink(INT nErrorCode)
{
	if (nErrorCode!=0L)
	{
		//��ʾ��ʾ
		CInformation Information;
		Information.ShowMessageBox(TEXT("�޷����ӵ���¼��������������Ϣ����ʧ�ܣ����Ժ����ԣ�"),MB_ICONERROR);
	}
	else
	{
		//�һ���Ϸ��
		if(m_bExchangeScore==true)
		{
			//��ȡ����
			CGlobalUserInfo * pGlobalUserInfo = CGlobalUserInfo::GetInstance();
			tagGlobalUserData * pGlobalUserData = pGlobalUserInfo->GetGlobalUserData();

			//���ҿؼ�
			CEditUI * pControlUI = static_cast<CEditUI *>(GetControlByName(szEditIngotCountControlName));
			if(pControlUI==NULL) return true;
			
			//��������
			CString strInputIngot(pControlUI->GetText());	
			SCORE lExchangeIngot = GetUserControlNumber(pControlUI->GetText());

			//����ṹ
			CMD_GP_ExchangeScoreByIngot ExchangeScore;
			ZeroMemory(&ExchangeScore,sizeof(ExchangeScore));

			//���ýṹ
			ExchangeScore.dwUserID=pGlobalUserData->dwUserID;
			ExchangeScore.lExchangeIngot=lExchangeIngot;
			CWHService::GetMachineIDEx(ExchangeScore.szMachineID);

			//��������
			m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_EXCHANGE_SCORE_BYINGOT,&ExchangeScore,sizeof(ExchangeScore));

			return true;
		}

		//�����Ա
		if(m_bPurchaseMember==true)
		{
			//��ȡ����
			CGlobalUserInfo * pGlobalUserInfo = CGlobalUserInfo::GetInstance();
			tagGlobalUserData * pGlobalUserData = pGlobalUserInfo->GetGlobalUserData();

			//��������
			CComboUI * pComboMemberType=(CComboUI *)GetControlByName(szComboMemberTypeControlName);;
			if(pComboMemberType==NULL) return true;

			//��ȡѡ��
			INT nCurrentSel=pComboMemberType->GetCurSel();
			if(nCurrentSel==LB_ERR) return true;
			tagMemberParameter * pMemberParameter=(tagMemberParameter *)pComboMemberType->GetItemAt(nCurrentSel)->GetUserData();

			//��������
			CEditUI * pEditPurchaseTime=(CEditUI *)GetControlByName(szEditPurchaseTimeControlName);;
			if(pEditPurchaseTime==NULL) return true;

			//��ȡѡ��
			WORD dwPurchaseTime = (WORD)GetUserControlNumber(pEditPurchaseTime->GetText());

			//����ṹ
			CMD_GP_PurchaseMember PurchaseMember;
			ZeroMemory(&PurchaseMember,sizeof(PurchaseMember));

			//���ñ���
			PurchaseMember.dwUserID=pGlobalUserData->dwUserID;
			PurchaseMember.cbMemberOrder=pMemberParameter->cbMemberOrder;
			PurchaseMember.wPurchaseTime=dwPurchaseTime;
			CWHService::GetMachineIDEx(PurchaseMember.szMachineID);

			//��������
			m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_PURCHASE_MEMBER,&PurchaseMember,sizeof(PurchaseMember));

			return true;
		}

		//���ز���
		if(m_bLoadExchangeParam==true)
		{
			//��������
			m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_EXCHANGE_QUERY,NULL,0);

			return true;
		}	
	}

	return true;
}

//�ر��¼�
bool CDlgExchangePlaza::OnEventMissionShut(BYTE cbShutReason)
{
	return true;
}

//��ȡ�¼�
bool CDlgExchangePlaza::OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize)
{
	//�������
	if(Command.wMainCmdID!=MDM_GP_USER_SERVICE) return true;

	//�һ�����
	if(Command.wSubCmdID==SUB_GP_EXCHANGE_PARAMETER)
	{
		//����У��
		ASSERT(wDataSize<=sizeof(CMD_GP_ExchangeParameter));
		if(wDataSize>sizeof(CMD_GP_ExchangeParameter)) return false;

		//��ȡ����
		CMD_GP_ExchangeParameter * pExchangeParameter= (CMD_GP_ExchangeParameter *)pData;

		//���ñ���
		m_dwExchangeRate = pExchangeParameter->dwExchangeRate;
		m_wMemberCount = pExchangeParameter->wMemberCount;
		CopyMemory(&m_MemberParameter,pExchangeParameter->MemberParameter,sizeof(tagMemberParameter)*m_wMemberCount);
		
		//��ֹ����
		m_MissionManager.ConcludeMissionItem(this,false);

		//������ֹ
		OnMissionConclude();

		//��������
		CListLabelElementUI * pListLabelElement=NULL;

		//���ҿؼ�	
		CComboUI * pComboMemberType =  static_cast<CComboUI *>(GetControlByName(szComboMemberTypeControlName));
		if(pComboMemberType!=NULL)
		{
			//��ͨʱ��
			for(INT nIndex=0;nIndex<m_wMemberCount;nIndex++)
			{
				//��������
				pListLabelElement = new CListLabelElementUI;
				if(pListLabelElement==NULL) throw TEXT("�ڴ治�㣡");

				//��������
				pListLabelElement->SetAttribute(TEXT("text"),m_MemberParameter[nIndex].szMemberName);
				pListLabelElement->SetAttribute(TEXT("height"),TEXT("23"));
				pListLabelElement->SetManager(&m_PaintManager,pComboMemberType,true);
				if(pComboMemberType->Add(pListLabelElement)==true)
				{
					pListLabelElement->SetUserData(&m_MemberParameter[nIndex]);
				}
			}

			//Ĭ��ѡ��
			if ((pComboMemberType->GetCount()>0)&&(pComboMemberType->GetCurSel()==LB_ERR))
			{
				pComboMemberType->SelectItem(0);
			}		
		}	

		//���ҿؼ�	
		CControlUI * pControlUI=  GetControlByName(szTextExchangeRateControlName);
		if(pControlUI!=NULL)
		{
			//��������
			TCHAR szExchangeRate[128]=TEXT("");
			_sntprintf(szExchangeRate,CountArray(szExchangeRate),TEXT("{a}{f 2}<u><c #00FF0000>1Ԫ��=%d��Ϸ��{/c}</u>{/f}{/a}"),m_dwExchangeRate);
			pControlUI->SetText(szExchangeRate);
		}

		//���½���
		RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW);

		return true;
	}

	//������
	if(Command.wSubCmdID==SUB_GP_PURCHASE_RESULT)
	{
		//����У��
		ASSERT(wDataSize<=sizeof(CMD_GP_PurchaseResult));
		if(wDataSize>sizeof(CMD_GP_PurchaseResult)) return false;

		//��ȡ����
		CMD_GP_PurchaseResult * pPurchaseResult= (CMD_GP_PurchaseResult *)pData;

		//����ɹ�
		if(pPurchaseResult->bSuccessed==true)
		{
			//��ȡ����
			CGlobalUserInfo * pGlobalUserInfo = CGlobalUserInfo::GetInstance();
			tagGlobalUserData * pGlobalUserData = pGlobalUserInfo->GetGlobalUserData();

			//���ñ���
			pGlobalUserData->lUserScore=pPurchaseResult->lCurrScore;
			pGlobalUserData->dUserBeans=pPurchaseResult->dCurrBeans;
			pGlobalUserData->cbMemberOrder=pPurchaseResult->cbMemberOrder;			

			//�����¼�
			CPlatformEvent * pPlatformEvent=CPlatformEvent::GetInstance();
			if (pPlatformEvent!=NULL) pPlatformEvent->SendPlatformEvent(EVENT_USER_INFO_UPDATE,0L);
		}

		//��Ϣ��ʾ
		if(pPurchaseResult->szNotifyContent[0]!=0)
		{
			CInformation Information;
			Information.ShowMessageBox(pPurchaseResult->szNotifyContent);
		}

		//��ֹ����
		m_MissionManager.ConcludeMissionItem(this,false);

		//������ֹ
		OnMissionConclude();

		//���½���
		RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW);

		return true;
	}

	//�һ����
	if(Command.wSubCmdID==SUB_GP_EXCHANGE_RESULT)
	{
		//����У��
		ASSERT(wDataSize<=sizeof(CMD_GP_ExchangeResult));
		if(wDataSize>sizeof(CMD_GP_ExchangeResult)) return false;

		//��ȡ����
		CMD_GP_ExchangeResult * pExchangeResult= (CMD_GP_ExchangeResult *)pData;

		//�һ��ɹ�
		if(pExchangeResult->bSuccessed==true)
		{
			//��ȡ����
			CGlobalUserInfo * pGlobalUserInfo = CGlobalUserInfo::GetInstance();
			tagGlobalUserData * pGlobalUserData = pGlobalUserInfo->GetGlobalUserData();

			//���ñ���
			pGlobalUserData->lUserScore=pExchangeResult->lCurrScore;
			pGlobalUserData->lUserIngot=pExchangeResult->lCurrIngot;

			//�����¼�
			CPlatformEvent * pPlatformEvent=CPlatformEvent::GetInstance();
			if (pPlatformEvent!=NULL) pPlatformEvent->SendPlatformEvent(EVENT_USER_INFO_UPDATE,0L);
		}

		//��Ϣ��ʾ
		if(pExchangeResult->szNotifyContent[0]!=0)
		{
			CInformation Information;
			Information.ShowMessageBox(pExchangeResult->szNotifyContent);
		}
		
		//��ֹ����
		m_MissionManager.ConcludeMissionItem(this,false);

		//������ֹ
		OnMissionConclude();

		//���½���
		RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW);

		return true;
	}

	return true;
}

//�һ���Ϸ��
VOID CDlgExchangePlaza::PerformExchangeScore()
{
	//���ñ���
	m_bExchangeScore=true;
	m_bPurchaseMember=false;
	m_bLoadExchangeParam=false;

	//��������
	if (m_MissionManager.AvtiveMissionItem(this,false)==false)  
	{
		return;
	}

	//��ʼ����
	OnMissionStart();
}

//�����Ա
VOID CDlgExchangePlaza::PerformPurchaseMember()
{
	//���ñ���
	m_bExchangeScore=false;
	m_bPurchaseMember=true;
	m_bLoadExchangeParam=false;

	//��������
	if (m_MissionManager.AvtiveMissionItem(this,false)==false)  
	{
		return;
	}

	//��ʼ����
	OnMissionStart();
}

//���ز���
VOID CDlgExchangePlaza::PerformLoadExchangeParam()
{
	//���ñ���
	m_bExchangeScore=false;
	m_bPurchaseMember=false;
	m_bLoadExchangeParam=true;

	//��������
	if (m_MissionManager.AvtiveMissionItem(this,false)==false)  
	{
		return;
	}

	//��ʼ����
	OnMissionStart();
}
////////////////////////////////////////////////////////////////////////////////////////////

//���캯��
CDlgExchangeServer::CDlgExchangeServer()
{
	//���ñ���
	m_pITCPSocket=NULL;
}

//��������
CDlgExchangeServer::~CDlgExchangeServer()
{
}

//�һ���Ϸ��
VOID CDlgExchangeServer::PerformExchangeScore()
{
	//��ȡ����
	CGlobalUserInfo * pGlobalUserInfo = CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData = pGlobalUserInfo->GetGlobalUserData();

	//���ҿؼ�
	CEditUI * pControlUI = static_cast<CEditUI *>(GetControlByName(szEditIngotCountControlName));
	if(pControlUI==NULL) return;
	
	//��������
	CString strInputIngot(pControlUI->GetText());	
	SCORE lExchangeIngot = GetUserControlNumber(pControlUI->GetText());

	//����ṹ
	CMD_GR_ExchangeScoreByIngot ExchangeScore;
	ZeroMemory(&ExchangeScore,sizeof(ExchangeScore));

	//���ýṹ
	ExchangeScore.dwUserID=pGlobalUserData->dwUserID;
	ExchangeScore.lExchangeIngot=lExchangeIngot;
	CWHService::GetMachineIDEx(ExchangeScore.szMachineID);

	//��������
	m_pITCPSocket->SendData(MDM_GR_EXCHANGE,SUB_GR_EXCHANGE_SCORE_BYINGOT,&ExchangeScore,sizeof(ExchangeScore));
}

//�����Ա
VOID CDlgExchangeServer::PerformPurchaseMember()
{
	//��ȡ����
	CGlobalUserInfo * pGlobalUserInfo = CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData = pGlobalUserInfo->GetGlobalUserData();

	//��������
	CComboUI * pComboMemberType=(CComboUI *)GetControlByName(szComboMemberTypeControlName);;
	if(pComboMemberType==NULL) return;

	//��ȡѡ��
	INT nCurrentSel=pComboMemberType->GetCurSel();
	if(nCurrentSel==LB_ERR) return;
	tagMemberParameter * pMemberParameter=(tagMemberParameter *)pComboMemberType->GetItemAt(nCurrentSel)->GetUserData();

	//��������
	CEditUI * pEditPurchaseTime=(CEditUI *)GetControlByName(szEditPurchaseTimeControlName);;
	if(pEditPurchaseTime==NULL) return;

	//��ȡѡ��
	WORD dwPurchaseTime = (WORD)GetUserControlNumber(pEditPurchaseTime->GetText());

	//����ṹ
	CMD_GR_PurchaseMember PurchaseMember;
	ZeroMemory(&PurchaseMember,sizeof(PurchaseMember));

	//���ñ���
	PurchaseMember.dwUserID=pGlobalUserData->dwUserID;
	PurchaseMember.cbMemberOrder=pMemberParameter->cbMemberOrder;
	PurchaseMember.wPurchaseTime=dwPurchaseTime;
	CWHService::GetMachineIDEx(PurchaseMember.szMachineID);

	//��������
	m_pITCPSocket->SendData(MDM_GR_EXCHANGE,SUB_GR_PURCHASE_MEMBER,&PurchaseMember,sizeof(PurchaseMember));
}

//���ز���
VOID CDlgExchangeServer::PerformLoadExchangeParam()
{
	//��������
	m_pITCPSocket->SendData(MDM_GR_EXCHANGE,SUB_GR_EXCHANGE_LOAD_INFO,NULL,0);
}

//��������
VOID CDlgExchangeServer::Initialization(ITCPSocket * pITCPSocket)
{
	//���ͱ���
	m_pITCPSocket=pITCPSocket;
}

//������Ϣ
bool CDlgExchangeServer::OnExchangeMessage(WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	//�������
	if(wMainCmdID!=MDM_GR_EXCHANGE) return true;

	//�һ�����
	if(wSubCmdID==SUB_GR_EXCHANGE_PARAM_INFO)
	{
		//����У��
		ASSERT(wDataSize<=sizeof(CMD_GR_ExchangeParameter));
		if(wDataSize>sizeof(CMD_GR_ExchangeParameter)) return false;

		//��ȡ����
		CMD_GR_ExchangeParameter * pExchangeParameter= (CMD_GR_ExchangeParameter *)pData;

		//���ñ���
		m_dwExchangeRate = pExchangeParameter->dwExchangeRate;
		m_wMemberCount = pExchangeParameter->wMemberCount;
		CopyMemory(&m_MemberParameter,pExchangeParameter->MemberParameter,sizeof(tagMemberParameter)*m_wMemberCount);
		
		//������ֹ
		OnMissionConclude();

		//��������
		CListLabelElementUI * pListLabelElement=NULL;

		//���ҿؼ�	
		CComboUI * pComboMemberType =  static_cast<CComboUI *>(GetControlByName(szComboMemberTypeControlName));
		if(pComboMemberType!=NULL)
		{
			//��ͨʱ��
			for(INT nIndex=0;nIndex<m_wMemberCount;nIndex++)
			{
				//��������
				try
				{
					pListLabelElement = new CListLabelElementUI;
					if(pListLabelElement==NULL) throw TEXT("�ڴ治�㣡");

					//��������
					pListLabelElement->SetAttribute(TEXT("text"),m_MemberParameter[nIndex].szMemberName);
					pListLabelElement->SetAttribute(TEXT("height"),TEXT("23"));
					pListLabelElement->SetManager(&m_PaintManager,pComboMemberType,true);
					if(pComboMemberType->Add(pListLabelElement)==true)
					{
						pListLabelElement->SetUserData(&m_MemberParameter[nIndex]);
					}
				}
				catch(...)
				{
					ASSERT(FALSE);
					break;
				}
			}

			//Ĭ��ѡ��
			if ((pComboMemberType->GetCount()>0)&&(pComboMemberType->GetCurSel()==LB_ERR))
			{
				pComboMemberType->SelectItem(0);
			}		
		}	

		//���ҿؼ�	
		CControlUI * pControlUI=  GetControlByName(szTextExchangeRateControlName);
		if(pControlUI!=NULL)
		{
			//��������
			TCHAR szExchangeRate[128]=TEXT("");
			_sntprintf(szExchangeRate,CountArray(szExchangeRate),TEXT("{a}{f 2}<u><c #00FF0000>1Ԫ��=%d��Ϸ��{/c}</u>{/f}{/a}"),m_dwExchangeRate);
			pControlUI->SetText(szExchangeRate);
		}

		//���½���
		RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW);

		return true;
	}

	//������
	if(wSubCmdID==SUB_GR_PURCHASE_RESULT)
	{
		//����У��
		ASSERT(wDataSize<=sizeof(CMD_GR_PurchaseResult));
		if(wDataSize>sizeof(CMD_GR_PurchaseResult)) return false;

		//��ȡ����
		CMD_GR_PurchaseResult * pPurchaseResult= (CMD_GR_PurchaseResult *)pData;

		//����ɹ�
		if(pPurchaseResult->bSuccessed==true)
		{
			//��ȡ����
			CGlobalUserInfo * pGlobalUserInfo = CGlobalUserInfo::GetInstance();
			tagGlobalUserData * pGlobalUserData = pGlobalUserInfo->GetGlobalUserData();

			//���ñ���
			pGlobalUserData->lUserScore=pPurchaseResult->lCurrScore;
			pGlobalUserData->cbMemberOrder=pPurchaseResult->cbMemberOrder;			

			//�����¼�
			CPlatformEvent * pPlatformEvent=CPlatformEvent::GetInstance();
			if (pPlatformEvent!=NULL) pPlatformEvent->SendPlatformEvent(EVENT_USER_INFO_UPDATE,0L);
		}

		//��Ϣ��ʾ
		if(pPurchaseResult->szNotifyContent[0]!=0)
		{
			CInformation Information;
			Information.ShowMessageBox(pPurchaseResult->szNotifyContent);
		}

		//������ֹ
		OnMissionConclude();

		//���½���
		RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW);

		return true;
	}

	//�һ����
	if(wSubCmdID==SUB_GR_EXCHANGE_RESULT)
	{
		//����У��
		ASSERT(wDataSize<=sizeof(CMD_GR_ExchangeResult));
		if(wDataSize>sizeof(CMD_GR_ExchangeResult)) return false;

		//��ȡ����
		CMD_GR_ExchangeResult * pExchangeResult= (CMD_GR_ExchangeResult *)pData;

		//�һ��ɹ�
		if(pExchangeResult->bSuccessed==true)
		{
			//��ȡ����
			CGlobalUserInfo * pGlobalUserInfo = CGlobalUserInfo::GetInstance();
			tagGlobalUserData * pGlobalUserData = pGlobalUserInfo->GetGlobalUserData();

			//���ñ���
			pGlobalUserData->lUserScore=pExchangeResult->lCurrScore;
			pGlobalUserData->lUserIngot=pExchangeResult->lCurrIngot;

			//�����¼�
			CPlatformEvent * pPlatformEvent=CPlatformEvent::GetInstance();
			if (pPlatformEvent!=NULL) pPlatformEvent->SendPlatformEvent(EVENT_USER_INFO_UPDATE,0L);
		}

		//��Ϣ��ʾ
		if(pExchangeResult->szNotifyContent[0]!=0)
		{
			CInformation Information;
			Information.ShowMessageBox(pExchangeResult->szNotifyContent);
		}
		
		//������ֹ
		OnMissionConclude();

		//���½���
		RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW);

		return true;
	}

	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////
