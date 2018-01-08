#include "StdAfx.h"
#include "Resource.h"
#include "DlgIssueMessage.h"
#include "DlgInformation.h"

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CDlgIssueMessage, CSkinDialog)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CDlgIssueMessage::CDlgIssueMessage() : CSkinDialog(IDD_DLG_ISSUE_MESSAGE)
{
	m_bSendALLRoom = FALSE;
	m_bSendGame = TRUE;
	m_bSendRoom = TRUE;
	m_bLoop = FALSE;
	m_dwTimeRate=0;
	tConcludeTime=0;
}

//��������
CDlgIssueMessage::~CDlgIssueMessage()
{
}

//�ؼ���
VOID CDlgIssueMessage::DoDataExchange(CDataExchange * pDX)
{
	__super::DoDataExchange(pDX);
	DDX_Control( pDX,IDOK,m_btOK );
	DDX_Control( pDX,IDCANCEL,m_btCancel );
	DDX_Check( pDX,IDC_CHECK_ALLROOM,m_bSendALLRoom);
	DDX_Check( pDX,IDC_CHEK_ROOM,m_bSendRoom );
	DDX_Check( pDX,IDC_CHEK_GAME,m_bSendGame );
	DDX_Check( pDX,IDC_CHEK_LOOP,m_bLoop );
	DDX_Text(pDX,IDC_EDT_MESSAGE,m_strMessage);
	DDX_Text(pDX,IDC_ED_TIMERATE,m_dwTimeRate);
}

//��ʼ������
BOOL CDlgIssueMessage::OnInitDialog()
{
	__super::OnInitDialog();

	//���ñ���
	SetWindowText(TEXT("��Ϣ����"));

	UpdateData(FALSE);

	return TRUE;
}

//ȷ������
VOID CDlgIssueMessage::OnOK()
{
	UpdateData();
	
	//Ч����Ϣ
	if( m_strMessage.IsEmpty() )
	{
		//��ʾ��Ϣ
		//��ʾ��Ϣ
		CDlgInformation Information(this);
		Information.ShowMessageBox(TEXT("��Ǹ,��ʾ��Ϣ����Ϊ��,����������!"),MB_ICONWARNING|MB_OK);
		return;
	}

	if( !m_bSendGame && !m_bSendRoom )
	{
		//��ʾ��Ϣ
		CDlgInformation Information(this);
		Information.ShowMessageBox(TEXT("������Ϣ����Ϸ��Ϣ��������ѡ������һ����"),MB_ICONWARNING|MB_OK);
		return;
	}

	if(m_bLoop)
	{
		CDateTimeCtrl* pCtrlDate = (CDateTimeCtrl*) GetDlgItem(IDC_CONCLUDEDATE);
		CDateTimeCtrl* pCtrlTime = (CDateTimeCtrl*) GetDlgItem(IDC_CONCLUDETIME);
		ASSERT(pCtrlDate!=NULL && pCtrlTime != NULL);

		CTime timeTime;
		DWORD dwResult = pCtrlDate->GetTime(timeTime);
		if (dwResult == GDT_VALID)
		{
			tConcludeTime=timeTime.GetTime()-timeTime.GetHour()*3600-timeTime.GetMinute()*60-timeTime.GetSecond();
		}
		else
		{
			//��ʾ��Ϣ
			CDlgInformation Information(this);
			Information.ShowMessageBox(TEXT("��������Ϣѭ���������ڣ�"),MB_ICONWARNING|MB_OK);
			return;
		}

		dwResult = pCtrlTime->GetTime(timeTime);
		if (dwResult == GDT_VALID)
		{
			tConcludeTime +=timeTime.GetHour()*3600+timeTime.GetMinute()*60+timeTime.GetSecond();
		}
		else
		{
			//��ʾ��Ϣ
			CDlgInformation Information(this);
			Information.ShowMessageBox(TEXT("��������Ϣѭ������ʱ�䣡"),MB_ICONWARNING|MB_OK);
			return;
		}
	}

	return __super::OnOK();
}

//////////////////////////////////////////////////////////////////////////
