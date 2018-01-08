#include "stdafx.h"
#include "DLGMBCard.h"
#include "resource.h"

//////////////////////////////////////////////////////////////////////////////////
//��ť�ؼ�
const TCHAR* const szButtonCloseControlName = TEXT("ButtonClose");
const TCHAR* const szButtonComfirmControlName = TEXT("ButtonComfirm");
const TCHAR* const szButtonCancleControlName = TEXT("ButtonCancle");

//��ǩ�ؼ�
const TCHAR* const szLabelCoordinateNumberControlName = TEXT("LabelCoordinateNumber");

//�༭�ؼ�
const TCHAR* const szEditInputControlName = TEXT("EditInput");

//////////////////////////////////////////////////////////////////////////////////

CDLGMBCard::CDLGMBCard():CFGuiDialog(IDD_DLG_SIMPLE)
{
	m_uMBCardID=0;
	ZeroMemory(m_szRandLetter,sizeof(m_szRandLetter));
	ZeroMemory(m_szRandNumber,sizeof(m_szRandNumber));
	m_cbErrorCount=0;
}

CDLGMBCard::~CDLGMBCard()
{
}



//������Ϣ
BOOL CDLGMBCard::OnInitDialog()
{

	SetWindowText(TEXT("�ܱ���Ϣ"));

	srand((unsigned)time(NULL));

	RandCoordinate();

	//������Ϣ
	CString strCoordinate,strTmp;
	for (int i=0; i<3; ++i)
	{
		strTmp.Format(TEXT("%c%d"),m_szRandLetter[i],m_szRandNumber[i]);
		strCoordinate+=strTmp;
		if(i+1 < 3)
			strCoordinate+=" ";
	}
	CControlUI * pLabelUI=GetControlByName(szLabelCoordinateNumberControlName);
	pLabelUI->SetText(strCoordinate);
	return FALSE; 
}

//��Ϣ����
void CDLGMBCard::Notify(TNotifyUI &  msg)
{
	//��ȡ����
	CControlUI * pControlUI = msg.pSender;

	//����¼�
	if (lstrcmp(msg.sType, TEXT("click")) == 0)
	{
		if(lstrcmp(pControlUI->GetName(), szButtonCloseControlName)==0 || lstrcmp(pControlUI->GetName(), szButtonCancleControlName)==0)
		{
			return OnCancel();
		}
		if(lstrcmp(pControlUI->GetName(), szButtonComfirmControlName)==0)
		{
			return OnOK();
		}
	}
	else if(lstrcmp(msg.sType,TEXT("link")) ==0 )
	{
		if (lstrcmp(pControlUI->GetName(), _T("ClickHere"))==0)
		{
			//��ȡ����
			ASSERT(CGlobalWebLink::GetInstance()!=NULL);
			CGlobalWebLink * pGlobalWebLink=CGlobalWebLink::GetInstance();
			//�����ַ
			TCHAR szNavigation[256]=TEXT("");
			_sntprintf(szNavigation,CountArray(szNavigation),TEXT("%s/Member/ExitPasswordCard.aspx.aspx"),pGlobalWebLink->GetPlatformLink());
			//��ҳ��
			ShellExecute(NULL,TEXT("OPEN"),szNavigation,NULL,NULL,SW_NORMAL);
			return;
		}
	}	
	return;
}

//�ؼ��¼�
void CDLGMBCard::OnOK()
{
	//��ȡ��Ϣ
	CString strInput;
	CControlUI * pLabelUI=GetControlByName(szEditInputControlName);
	if(pLabelUI!=NULL) strInput=pLabelUI->GetText();

	CString strCoorNumber=TEXT(""),strTmp;
	for (int i=0; i<3; ++i)
	{
		int nIndex1=int(m_szRandLetter[i]-'A');
		int nIndex2=int(m_szRandNumber[i]-1);
		if(m_uCoorNumber[nIndex1][nIndex2]<10)
			strTmp.Format(TEXT("%d00"),m_uCoorNumber[nIndex1][nIndex2]);
		else if(m_uCoorNumber[nIndex1][nIndex2]<100)
			strTmp.Format(TEXT("%d0"),m_uCoorNumber[nIndex1][nIndex2]);
		else
			strTmp.Format(TEXT("%d"),m_uCoorNumber[nIndex1][nIndex2]);
		strCoorNumber+=strTmp;
	}

	//Ч����Ϣ
	if (strCoorNumber!=strInput) 
	{		
		++m_cbErrorCount;
		if (m_cbErrorCount>=3)
		{
			ShowInformationEx(TEXT("��ʾ��Ϣ"),TEXT("�ܱ����������ޣ������µ�¼��"),MB_ICONINFORMATION,0);
			EndDialog(IDCANCEL);
			return;
		}		
		ShowInformationEx(TEXT("��ʾ��Ϣ"),TEXT("�ܱ������Ӧ������������"),MB_ICONINFORMATION,0);
		RandCoordinate();
		Invalidate();
		return;
	}

	EndDialog(IDOK);
}

//ȡ����Ϣ
VOID CDLGMBCard::OnCancel()
{
	return __super::OnCancel();
}

//�������
void CDLGMBCard::RandCoordinate()
{
	for (int i=0; i<CountArray(m_szRandLetter); ++i)
		m_szRandLetter[i]='A'+rand()%4;
	for (int i=0; i<CountArray(m_szRandNumber); ++i) 
		m_szRandNumber[i]=1+rand()%4;
}

//�����ܱ�
void CDLGMBCard::SetMBCard(UINT uMBCardID)
{
	m_uMBCardID=uMBCardID;

	m_uCoorNumber[0][0]=(m_uMBCardID/9)%1000;
	m_uCoorNumber[0][1]=(m_uMBCardID/15)%1000;
	m_uCoorNumber[0][2]=(m_uMBCardID/24)%1000;
	m_uCoorNumber[0][3]=(m_uMBCardID/268)%1000;

	m_uCoorNumber[1][0]=(m_uMBCardID/1)%1000;
	m_uCoorNumber[1][1]=(m_uMBCardID/88)%1000;
	m_uCoorNumber[1][2]=(m_uMBCardID/25)%1000;
	m_uCoorNumber[1][3]=(m_uMBCardID/76)%1000;

	m_uCoorNumber[2][0]=(m_uMBCardID/185)%1000;
	m_uCoorNumber[2][1]=(m_uMBCardID/28)%1000;
	m_uCoorNumber[2][2]=(m_uMBCardID/65)%1000;
	m_uCoorNumber[2][3]=(m_uMBCardID/205)%1000;

	m_uCoorNumber[3][0]=(m_uMBCardID/72)%1000;
	m_uCoorNumber[3][1]=(m_uMBCardID/39)%1000;
	m_uCoorNumber[3][2]=(m_uMBCardID/11)%1000;
	m_uCoorNumber[3][3]=(m_uMBCardID/31)%1000;

	m_uCoorNumber[4][0]=(m_uMBCardID/52)%1000;
	m_uCoorNumber[4][1]=(m_uMBCardID/220)%1000;
	m_uCoorNumber[4][2]=(m_uMBCardID/7)%1000;
	m_uCoorNumber[4][3]=(m_uMBCardID/105)%1000;

	m_uCoorNumber[5][0]=(m_uMBCardID/99)%1000;
	m_uCoorNumber[5][1]=(m_uMBCardID/117)%1000;
	m_uCoorNumber[5][2]=(m_uMBCardID/93)%1000;
	m_uCoorNumber[5][3]=(m_uMBCardID/48)%1000;

}