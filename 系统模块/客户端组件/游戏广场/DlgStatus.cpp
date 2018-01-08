#include "StdAfx.h"
#include "DlgStatus.h"
#include "PlatformFrame.h"
#include "PlazaViewServer.h"

//////////////////////////////////////////////////////////////////////////////////

//Բ�Ǵ�С
#define ROUND_CX					5									//Բ�ǿ��
#define ROUND_CY					5									//Բ�Ǹ߶�

//ʱ���ʶ
#define IDT_STATUS					10									//״̬ʱ��
#define TIMER_STATUS				20									//״̬ʱ��


const TCHAR* const szStatusProgressControlName = TEXT("StatusProgress");
const TCHAR* const szStatusLableControlName = TEXT("LableStatus");

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CDlgStatus, CFGuiDialog)

	//ϵͳ��Ϣ
	ON_WM_TIMER()

END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CDlgStatus::CDlgStatus() : CFGuiDialog(IDD_DLG_POPUP)
{
	//״̬����
	m_uImageIndex=0;
	m_bFinished = false;

	//�ؼ�����
	m_pbtGiveUp=NULL;

	//�ӿڱ���
	m_pIStatusViewSink=NULL;

	return;
}

//��������
CDlgStatus::~CDlgStatus()
{
}

//��Ϣ����
BOOL CDlgStatus::PreTranslateMessage(MSG * pMsg)
{
	//��������
	if ((pMsg->message==WM_KEYDOWN)&&((pMsg->wParam==VK_ESCAPE)||(pMsg->wParam==VK_RETURN)))
	{
		return TRUE;
	}

	return __super::PreTranslateMessage(pMsg);
}

//���ú���
BOOL CDlgStatus::OnInitDialog()
{
	__super::OnInitDialog();

	//���ñ���
	SetWindowText(szProduct);

	
	//���д���
	CenterWindow(CPlatformFrame::GetInstance());
	//���ÿؼ�
	SetTimer(IDT_STATUS,TIMER_STATUS,NULL);

	return TRUE;
}

//��ʼ�ؼ�
void CDlgStatus::InitControlUI()
{
	__super::InitControlUI();
	return;
}

//��Ϣ����
void CDlgStatus::Notify(TNotifyUI &  msg)
{
	//��ȡ����
	CControlUI * pControlUI = msg.pSender;

	//����¼�
	if (lstrcmp(msg.sType, TEXT("click")) == 0)
	{
		if(m_pbtGiveUp==pControlUI) 
		{
			return OnBnClickedGiveUp(); 
		}
	}
}

//Ƥ������
LPCTSTR CDlgStatus::GetSkinFile()
{
	return TEXT("DlgStatus.xml");
}

//��ʼ�滭
void CDlgStatus::OnBeginPaintWindow(HDC hDC)
{
	return;
}

//�رմ���
VOID CDlgStatus::HideStatusWindow()
{
	//�رմ���
	CProgressUI *pCProgressUI =  (CProgressUI * )GetControlByName(szStatusProgressControlName);
	if(NULL != pCProgressUI)
	{
		pCProgressUI->SetValue(pCProgressUI->GetMaxValue());
	}

	//�ö�����
	//SetActiveWindow();
	//BringWindowToTop();
	SetForegroundWindow();

	m_bFinished = true;
	return;
}

//������Ϣ
VOID CDlgStatus::ShowStatusWindow(LPCTSTR pszDescribe)
{
	//��������
	if (m_hWnd==NULL)
	{
		if (CPlatformFrame::GetInstance()->IsWindowVisible()==FALSE)
		{
			Create(IDD_DLG_POPUP,GetDesktopWindow());
		}
		else
		{
			Create(IDD_DLG_POPUP,CPlatformFrame::GetInstance());
		}
	}

	//��������
	m_strDescribe=pszDescribe;

	CLabelUI *pCProgressUI =  (CLabelUI *)GetControlByName(szStatusLableControlName);
	if(NULL != pCProgressUI)
	{
		pCProgressUI->SetText(m_strDescribe);
	}

	//��ʾ����
	ShowWindow(SW_SHOW);

	//��Ļ����
	CRect rtDesk;  
	CRect rtDlg;  
	::GetWindowRect(::GetDesktopWindow(), &rtDesk);  
	GetWindowRect(&rtDlg);  
	int iXpos = rtDesk.Width() / 2 - rtDlg.Width() / 2;  
	int iYpos = rtDesk.Height() / 2 - rtDlg.Height() / 2;  
	SetWindowPos(NULL, iXpos, iYpos, 0, 0, SWP_NOOWNERZORDER|SWP_NOSIZE|SWP_NOZORDER);

	return;
}

//���ýӿ�
VOID CDlgStatus::SetStatusViewSink(IStatusViewSink * pIStatusViewSink)
{
	//���ñ���
	m_pIStatusViewSink=pIStatusViewSink;

	return;
}

//ʱ����Ϣ
VOID CDlgStatus::OnTimer(UINT nIDEvent)
{
	//���ý���
	if (nIDEvent==IDT_STATUS)
	{	
		if(true == m_bFinished)
		{
			m_bFinished = false;
			if (m_hWnd!=NULL) ShowWindow(SW_HIDE);
			return;
		}

		CProgressUI *pCProgressUI =  (CProgressUI * )GetControlByName(szStatusProgressControlName);
		if(NULL != pCProgressUI)
		{
			pCProgressUI->SetValue(m_uImageIndex);
		}

		//��������
		m_uImageIndex=(m_uImageIndex>=100)?0:(m_uImageIndex+1L);

		return;
	}

	__super::OnTimer(nIDEvent);
}

//ȡ����ť
VOID CDlgStatus::OnBnClickedGiveUp()
{
	//���ؽ���
	HideStatusWindow();

	//Ͷ����Ϣ
	ASSERT(m_pIStatusViewSink!=NULL);
	if (m_pIStatusViewSink!=NULL) m_pIStatusViewSink->OnStatusCancel();

	return;
}



//////////////////////////////////////////////////////////////////////////////////
