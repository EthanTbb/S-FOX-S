// ClientControlItemSink.cpp : 实现文件
//

#include "stdafx.h"
#include "Resource.h"
#include "ClientControlItemSink.h"
#include ".\clientcontrolitemsink.h"


// CClientControlItemSinkDlg 对话框

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

// CClientControlItemSinkDlg 消息处理程序

//下注信息
bool CClientControlItemSinkDlg::PlayersBet(BYTE cbViewIndex, LONGLONG lScoreCount)
{
	//效验参数
	ASSERT(cbViewIndex<=ID_JIAO_R);
	if (cbViewIndex>ID_JIAO_R) return false;

	m_lUserJettonScore[cbViewIndex] += lScoreCount;
	return true;
}


//取消控制
void CClientControlItemSinkDlg::OnBnClickedCQxkz()
{
	((CButton*)GetDlgItem(IDC_C_SM))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_C_TM))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_C_XM))->SetCheck(FALSE);
}

//上门
void CClientControlItemSinkDlg::OnBnClickedCSm()
{
	((CButton*)GetDlgItem(IDC_C_QXKZ))->SetCheck(FALSE);
}

//天门
void CClientControlItemSinkDlg::OnBnClickedCTm()
{
	((CButton*)GetDlgItem(IDC_C_QXKZ))->SetCheck(FALSE);
}

//下门
void CClientControlItemSinkDlg::OnBnClickedCXm()
{
	((CButton*)GetDlgItem(IDC_C_QXKZ))->SetCheck(FALSE);
}

//确定信息
void CClientControlItemSinkDlg::OnBnClickedOk()
{
	CMD_C_AdminControl AdminControl;
	// 上， 天， 下 控制
	AdminControl.cbControlArea[SHUN_MEN_INDEX] = ((CButton*)GetDlgItem(IDC_C_SM))->GetCheck();
	AdminControl.cbControlArea[DUI_MEN_INDEX] = ((CButton*)GetDlgItem(IDC_C_TM))->GetCheck();
	AdminControl.cbControlArea[DAO_MEN_INDEX] = ((CButton*)GetDlgItem(IDC_C_XM))->GetCheck();

	// 取消控制
	AdminControl.cbControlArea[BANKER_INDEX] = ((CButton*)GetDlgItem(IDC_C_QXKZ))->GetCheck();

	if ( AdminControl.cbControlArea[BANKER_INDEX] 
		 && AdminControl.cbControlArea[SHUN_MEN_INDEX]
		 && AdminControl.cbControlArea[DUI_MEN_INDEX]
		 && AdminControl.cbControlArea[DAO_MEN_INDEX] )
	{
		AfxMessageBox(TEXT("设置错误！"));
		return;
	}

	//所有都不选择为取消控制
	CGameFrameEngine::GetInstance()->SendMessage(IDM_ADMIN_COMMDN, ((WPARAM)(&AdminControl)), 0);
}

//确定信息
void CClientControlItemSinkDlg::OnOK()
{
	OnBnClickedOk();
}

//更新控制
void CClientControlItemSinkDlg::OnAllowControl( bool bEnable )
{
	ZeroMemory(m_lUserJettonScore,sizeof(m_lUserJettonScore));
}

