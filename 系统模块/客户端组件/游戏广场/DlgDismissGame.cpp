#include "stdafx.h"
#include "Resource.h"
#include "DlgDismissGame.h"

///////////////////////////////////////////////////////////////////////////////////////////
//��Ϣӳ��

BEGIN_MESSAGE_MAP(CDlgDismissGame, CSkinDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////////////////////
//���캯��
CDlgDismissGame::CDlgDismissGame(CWnd* pParent)	: CSkinDialog(IDD_DLG_DISMISSGAME, pParent)
{
}

//��������
CDlgDismissGame::~CDlgDismissGame()
{
}

//���ݽ���
void CDlgDismissGame::DoDataExchange(CDataExchange* pDX)
{
	__super::DoDataExchange(pDX);
	DDX_Control( pDX,IDOK,m_btOK );
	DDX_Control( pDX,IDCANCEL,m_btCancel );
}

//ȷ����ť
void CDlgDismissGame::OnBnClickedOk()
{
	CString strTableNum;
	GetDlgItem(IDC_EDIT_TABLE_NUM)->GetWindowText(strTableNum);
	m_wDismissTable=(WORD)StrToInt(strTableNum);
	if(m_wDismissTable > 0) m_wDismissTable -= 1;

	OnOK();
}

//��ʼ������
BOOL CDlgDismissGame::OnInitDialog()
{
	__super::OnInitDialog();

	//���ñ���
	SetWindowText(TEXT("��ɢ����"));

	//��������
	((CEdit*)GetDlgItem(IDC_EDIT_TABLE_NUM))->SetLimitText(2);

	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////////////////
