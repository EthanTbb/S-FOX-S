#include "Stdafx.h"
#include "DlgAccredit.h"
#include "MissionLogon.h"
#include "DlgInformation.h"

//////////////////////////////////////////////////////////////////////////////////
//���붨��
LPCTSTR g_pszPassPortID = TEXT("0123456789X");

//////////////////////////////////////////////////////////////////////////////////
//��ť�ؼ�
const TCHAR* const szButtonCloseControlName = TEXT("ButtonClose");
const TCHAR* const szButtonResetControlName = TEXT("ButtonReset");
const TCHAR* const szButtonNumberControlName= TEXT("ButtonNumber");
const TCHAR* const szButtonAccreditControlName = TEXT("ButtonAccredit");
const TCHAR* const szButtonBackSpaceControlName = TEXT("ButtonBackSpace");

//��ǩ�ؼ�
const TCHAR* const szLabelPassPortIDControlName = TEXT("LabelPassPortID");

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CDlgAccredit::CDlgAccredit() : CFGuiDialog(IDD_DLG_SIMPLE)
{
}

//��������
CDlgAccredit::~CDlgAccredit()
{
}

//��������
BOOL CDlgAccredit::OnInitDialog()
{
	__super::OnInitDialog();

	//�޸���ʽ
	ModifyStyle(0, WS_MINIMIZEBOX|WS_MAXIMIZEBOX);

	//���ñ���
	SetWindowText(TEXT("��ȫ��֤"));

	//���д���
	CenterWindow(this);

	//�������
	RandPassportNumber();

	return TRUE;
}

//ȷ������
VOID CDlgAccredit::OnOK()
{
	//��������
	CString strPassPortID;
	TCHAR szDescribe[128]=TEXT("");
	CUserItemElement * pUserItemElement=CUserItemElement::GetInstance();

	//���ҿؼ�
	CControlUI * pLabelUI=GetControlByName(szLabelPassPortIDControlName);
	if(pLabelUI!=NULL) strPassPortID=pLabelUI->GetText();
	if(strPassPortID.IsEmpty()==true) return;

	//���֤��
	if (pUserItemElement->EfficacyPassPortID(strPassPortID,szDescribe,CountArray(szDescribe))==false)
	{
		//��ʾ��Ϣ
		CDlgInformation Information(this);
		Information.ShowMessageBox(szDescribe,MB_ICONERROR,0);

		return;
	}
	
	return __super::OnOK();
}

//ȡ����Ϣ
VOID CDlgAccredit::OnCancel()
{
	return __super::OnCancel();
}

//��Ϣ����
void CDlgAccredit::Notify(TNotifyUI &  msg)
{
	//��ȡ����
	CControlUI * pControlUI = msg.pSender;

	//����¼�
	if (lstrcmp(msg.sType, TEXT("click")) == 0)
	{
		if(lstrcmp(pControlUI->GetName(), szButtonCloseControlName)==0)
		{
			return OnCancel();
		}
		if(lstrcmp(pControlUI->GetName(), szButtonAccreditControlName)==0)
		{
			return OnOK();
		}
		else if(lstrcmp(pControlUI->GetName(), szButtonBackSpaceControlName)==0) 
		{
			return OnClickedButtonBackSpace(pControlUI); 
		}
		else if(lstrcmp(pControlUI->GetName(), szButtonResetControlName)==0)
		{
			return OnClickedButtonReset(pControlUI);
		}
		else
		{
			if(lstrcmp(pControlUI->GetClass(), TEXT("ButtonUI"))==0) 
			{
				return OnClickedButtonNumber(pControlUI); 
			}
		}
	}
	
	return;
}

//��ȡ����
LPCTSTR CDlgAccredit::GetPassPortNumber()
{
	//���ҿؼ�
	CControlUI * pLabelUI=GetControlByName(szLabelPassPortIDControlName);
	if(pLabelUI!=NULL) return pLabelUI->GetText();

	return TEXT("");
}

//�������
VOID CDlgAccredit::RandPassportNumber()
{
	//��������
	TCHAR szPassPortID[12];
	lstrcpyn(szPassPortID,g_pszPassPortID,CountArray(szPassPortID));

	//��������
	TCHAR chaTrans;
	BYTE cbRandIndex=0;
	BYTE cbNumberCount=lstrlen(szPassPortID);

	//�������
	for(BYTE i=0;i<cbNumberCount;i++)
	{
		cbRandIndex=rand()%cbNumberCount;
		if(cbRandIndex!=i)
		{
			chaTrans=szPassPortID[i];
			szPassPortID[i]=szPassPortID[cbRandIndex];
			szPassPortID[cbRandIndex]=chaTrans;
		}
	}

	//��������
	TCHAR szNumberControlName[64]=TEXT("");
	TCHAR szControlText[2]=TEXT("");
	CControlUI * pControlUI=NULL;

	//�����ı�
	for(BYTE i=0;i<cbNumberCount;i++)
	{
		_sntprintf(szNumberControlName,CountArray(szNumberControlName),TEXT("%s%d"),szButtonNumberControlName,i+1);
		pControlUI=GetControlByName(szNumberControlName);
        if(pControlUI!=NULL) 
		{
			szControlText[0]=szPassPortID[i];
			pControlUI->SetText(szControlText);
		}
	}
	 
	return;
}

//���ð�ť
VOID CDlgAccredit::OnClickedButtonReset(CControlUI * pControlUI)
{
	CControlUI * pLabelUI=GetControlByName(szLabelPassPortIDControlName);
	if(pLabelUI!=NULL) pLabelUI->SetText(TEXT("")); 
}

//���鰴ť
VOID CDlgAccredit::OnClickedButtonNumber(CControlUI * pControlUI)
{
	//����У��
	ASSERT(pControlUI!=NULL);
	if(pControlUI==NULL) return;

	//��������
	CString strPassPortID;

	//���ҿؼ�
	CControlUI * pLabelUI = GetControlByName(szLabelPassPortIDControlName);
	if(pLabelUI!=NULL) strPassPortID = pLabelUI->GetText();

	//�����ж�
	if(strPassPortID.GetLength() >= LEN_PASS_PORT_ID-1) return;

	//���ú���
	strPassPortID += pControlUI->GetText();
	pLabelUI->SetText(strPassPortID);

	return;
}

//�˸�ť
VOID CDlgAccredit::OnClickedButtonBackSpace(CControlUI * pControlUI)
{
	//��������
	CString strPassPortID;

	//���ҿؼ�
	CControlUI * pLabelUI=GetControlByName(szLabelPassPortIDControlName);
	if(pLabelUI!=NULL) strPassPortID = pLabelUI->GetText();

	//���ú���
	strPassPortID.Delete(strPassPortID.GetLength()-1);
	pLabelUI->SetText(strPassPortID);

	return;
}
//////////////////////////////////////////////////////////////////////////////////