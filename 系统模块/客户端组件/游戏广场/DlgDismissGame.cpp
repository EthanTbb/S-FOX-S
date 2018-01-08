#include "stdafx.h"
#include "Resource.h"
#include "DlgDismissGame.h"

///////////////////////////////////////////////////////////////////////////////////////////
//消息映射

BEGIN_MESSAGE_MAP(CDlgDismissGame, CSkinDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////////////////////
//构造函数
CDlgDismissGame::CDlgDismissGame(CWnd* pParent)	: CSkinDialog(IDD_DLG_DISMISSGAME, pParent)
{
}

//析构函数
CDlgDismissGame::~CDlgDismissGame()
{
}

//数据交换
void CDlgDismissGame::DoDataExchange(CDataExchange* pDX)
{
	__super::DoDataExchange(pDX);
	DDX_Control( pDX,IDOK,m_btOK );
	DDX_Control( pDX,IDCANCEL,m_btCancel );
}

//确定按钮
void CDlgDismissGame::OnBnClickedOk()
{
	CString strTableNum;
	GetDlgItem(IDC_EDIT_TABLE_NUM)->GetWindowText(strTableNum);
	m_wDismissTable=(WORD)StrToInt(strTableNum);
	if(m_wDismissTable > 0) m_wDismissTable -= 1;

	OnOK();
}

//初始化窗口
BOOL CDlgDismissGame::OnInitDialog()
{
	__super::OnInitDialog();

	//设置标题
	SetWindowText(TEXT("解散桌子"));

	//输入限制
	((CEdit*)GetDlgItem(IDC_EDIT_TABLE_NUM))->SetLimitText(2);

	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////////////////
