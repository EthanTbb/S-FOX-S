// ClientControlItemSink.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Resource.h"
#include "ClientControlItemSink.h"
#include ".\clientcontrolitemsink.h"


// CClientControlItemSinkDlg �Ի���

IMPLEMENT_DYNAMIC(CClientControlItemSinkDlg, IClientControlDlg)

CClientControlItemSinkDlg::CClientControlItemSinkDlg(CWnd* pParent /*=NULL*/)
	: IClientControlDlg(CClientControlItemSinkDlg::IDD, pParent)
{
	ZeroMemory(m_lUserJettonScore,sizeof(m_lUserJettonScore));
}

CClientControlItemSinkDlg::~CClientControlItemSinkDlg()
{
}

void CClientControlItemSinkDlg::DoDataExchange(CDataExchange* pDX)
{
	IClientControlDlg::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CClientControlItemSinkDlg, IClientControlDlg)
	ON_BN_CLICKED(IDC_C_SM, OnBnClickedCSm)
	ON_BN_CLICKED(IDC_C_TM, OnBnClickedCTm)
	ON_BN_CLICKED(IDC_C_XM, OnBnClickedCXm)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDC_C_QXKZ, OnBnClickedCQxkz)
END_MESSAGE_MAP()

// CClientControlItemSinkDlg ��Ϣ�������

//��ע��Ϣ
bool CClientControlItemSinkDlg::PlayersBet(BYTE cbViewIndex, LONGLONG lScoreCount)
{
	//Ч�����
	ASSERT(cbViewIndex<=ID_JIAO_R);
	if (cbViewIndex>ID_JIAO_R) return false;

	m_lUserJettonScore[cbViewIndex] += lScoreCount;
	return true;
}


//ȡ������
void CClientControlItemSinkDlg::OnBnClickedCQxkz()
{
	((CButton*)GetDlgItem(IDC_C_SM))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_C_TM))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_C_XM))->SetCheck(FALSE);
}

//����
void CClientControlItemSinkDlg::OnBnClickedCSm()
{
	((CButton*)GetDlgItem(IDC_C_QXKZ))->SetCheck(FALSE);
}

//����
void CClientControlItemSinkDlg::OnBnClickedCTm()
{
	((CButton*)GetDlgItem(IDC_C_QXKZ))->SetCheck(FALSE);
}

//����
void CClientControlItemSinkDlg::OnBnClickedCXm()
{
	((CButton*)GetDlgItem(IDC_C_QXKZ))->SetCheck(FALSE);
}

//ȷ����Ϣ
void CClientControlItemSinkDlg::OnBnClickedOk()
{
	CMD_C_AdminControl AdminControl;
	// �ϣ� �죬 �� ����
	AdminControl.cbControlArea[SHUN_MEN_INDEX] = ((CButton*)GetDlgItem(IDC_C_SM))->GetCheck();
	AdminControl.cbControlArea[DUI_MEN_INDEX] = ((CButton*)GetDlgItem(IDC_C_TM))->GetCheck();
	AdminControl.cbControlArea[DAO_MEN_INDEX] = ((CButton*)GetDlgItem(IDC_C_XM))->GetCheck();

	// ȡ������
	AdminControl.cbControlArea[BANKER_INDEX] = ((CButton*)GetDlgItem(IDC_C_QXKZ))->GetCheck();

	if ( AdminControl.cbControlArea[BANKER_INDEX] 
		 && AdminControl.cbControlArea[SHUN_MEN_INDEX]
		 && AdminControl.cbControlArea[DUI_MEN_INDEX]
		 && AdminControl.cbControlArea[DAO_MEN_INDEX] )
	{
		AfxMessageBox(TEXT("���ô���"));
		return;
	}

	//���ж���ѡ��Ϊȡ������
	CGameFrameEngine::GetInstance()->SendMessage(IDM_ADMIN_COMMDN, ((WPARAM)(&AdminControl)), 0);
}

//ȷ����Ϣ
void CClientControlItemSinkDlg::OnOK()
{
	OnBnClickedOk();
}

//���¿���
void CClientControlItemSinkDlg::OnAllowControl( bool bEnable )
{
	ZeroMemory(m_lUserJettonScore,sizeof(m_lUserJettonScore));
}

