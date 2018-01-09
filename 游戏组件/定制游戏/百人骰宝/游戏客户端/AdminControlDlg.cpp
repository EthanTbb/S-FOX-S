// AdminControlDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "AdminControlDlg.h"
#include ".\admincontroldlg.h"


// CAdminControlDlg 对话框

IMPLEMENT_DYNAMIC(CAdminControlDlg, CDialog)
CAdminControlDlg::CAdminControlDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAdminControlDlg::IDD, pParent)
{
}

CAdminControlDlg::~CAdminControlDlg()
{
}

void CAdminControlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

void CAdminControlDlg::ReSetAdminWnd()
{

	if(m_cbControlStyle!=CS_BANKER_WIN&&m_cbControlStyle!=CS_BANKER_LOSE)
	{
		((CButton*)GetDlgItem(IDC_RADIO_WIN))->SetCheck(0);
		((CButton*)GetDlgItem(IDC_RADIO_LOSE))->SetCheck(0);
	}
	else
	{
		((CButton*)GetDlgItem(IDC_RADIO_CT_BANKER))->SetCheck(1);
		((CButton*)GetDlgItem(IDC_RADIO_CT_AREA))->SetCheck(0);
		((CButton*)GetDlgItem(IDC_RADIO_WIN))->SetCheck(m_cbControlStyle==CS_BANKER_WIN?1:0);
		((CButton*)GetDlgItem(IDC_RADIO_LOSE))->SetCheck(m_cbControlStyle==CS_BANKER_LOSE?1:0);
	}

	if(m_cbControlStyle==CS_BET_AREA)
	{
		((CButton*)GetDlgItem(IDC_RADIO_CT_BANKER))->SetCheck(0);
		((CButton*)GetDlgItem(IDC_RADIO_CT_AREA))->SetCheck(1);

		BYTE cbIndex=0;
		for(;cbIndex<CONTROL_AREA;cbIndex++)
			((CButton*)GetDlgItem(IDC_CHECK_TIAN+cbIndex))->SetCheck(m_bWinArea[cbIndex]?1:0);
	}
	((CComboBox*)GetDlgItem(IDC_COMBO_TIMES))->SetCurSel(m_cbExcuteTimes>0?(m_cbExcuteTimes-1):-1);
	OnRadioClick();
}

//申请结果
bool CAdminControlDlg::ReqResult(const void * pBuffer)
{
	const CMD_S_CommandResult*pResult=(CMD_S_CommandResult*)pBuffer;
	CString str;
	switch(pResult->cbAckType)
	{
	case ACK_SET_WIN_AREA:
		{
			if(pResult->cbResult==CR_ACCEPT)
			{  
				str=TEXT("输赢控制命令已经接受！");
				SetTimer(1,1000,0);
			}
			else
			{
				str.Format(TEXT("请求失败!"));
				m_cbExcuteTimes=0;
				m_cbControlStyle=0;
				ZeroMemory(m_bWinArea,sizeof(m_bWinArea));
				SetTimer(1,1000,0);
			}
			break;
		}
	case ACK_RESET_CONTROL:
		{
			if(pResult->cbResult==CR_ACCEPT)
			{  
				m_cbControlStyle=0;
				m_cbExcuteTimes=0;
				ZeroMemory(m_bWinArea,sizeof(m_bWinArea));
				ReSetAdminWnd();
				str=TEXT("重置请求已接受!");
				SetTimer(1,1000,0);
			}
			break;
		}
	case ACK_PRINT_SYN:
		{
			if(pResult->cbResult==CR_ACCEPT)
			{  
				str=TEXT("服务器同步请求已接受!");
				
				tagAdminReq*pAdminReq=(tagAdminReq*)pResult->cbExtendData;
				m_cbExcuteTimes=pAdminReq->m_cbExcuteTimes;
				m_cbControlStyle=pAdminReq->m_cbControlStyle;
				memcpy(m_bWinArea,pAdminReq->m_bWinArea,sizeof(m_bWinArea));
				ReSetAdminWnd();
				SetTimer(1,1000,0);
			}
			else
			{
				str.Format(TEXT("请求失败!"));
			}
			break;
		}

	default: break;
	}

	SetWindowText(str);
	return true;
}


BEGIN_MESSAGE_MAP(CAdminControlDlg, CDialog)
	ON_BN_CLICKED(IDC_BT_RESET,OnReSet)
	ON_BN_CLICKED(IDC_BT_CANCEL,OnCancel)
	ON_BN_CLICKED(IDC_BT_EXCUTE,OnExcute)
	ON_BN_CLICKED(IDC_BT_CURSET,OnRefresh)
	ON_BN_CLICKED(IDC_RADIO_CT_AREA,OnRadioClick)
	ON_BN_CLICKED(IDC_RADIO_CT_BANKER,OnRadioClick)
	ON_WM_TIMER()
END_MESSAGE_MAP()



void CAdminControlDlg::OnRadioClick()
{
	if(((CButton*)GetDlgItem(IDC_RADIO_CT_BANKER))->GetCheck()==1)
	{
		GetDlgItem(IDC_RADIO_LOSE)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_WIN)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHECK_TIAN)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_DI)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_XUAN)->EnableWindow(FALSE);
	}
	else
	{
		if(((CButton*)GetDlgItem(IDC_RADIO_CT_AREA))->GetCheck())
		{
			GetDlgItem(IDC_RADIO_LOSE)->EnableWindow(FALSE);
			GetDlgItem(IDC_RADIO_WIN)->EnableWindow(FALSE);
			GetDlgItem(IDC_CHECK_TIAN)->EnableWindow(TRUE);
			GetDlgItem(IDC_CHECK_DI)->EnableWindow(TRUE);
			GetDlgItem(IDC_CHECK_XUAN)->EnableWindow(TRUE);
		}
	}
}

// CAdminControlDlg 消息处理程序
void CAdminControlDlg::OnReSet()
{
	CMD_C_AdminReq adminReq;
	adminReq.cbReqType=RQ_RESET_CONTROL;
	AfxGetMainWnd()->SendMessage(IDM_ADMIN_COMMDN,(WPARAM)&adminReq,0);
}
void CAdminControlDlg::OnRefresh()
{
	CMD_C_AdminReq adminReq;
	adminReq.cbReqType=RQ_PRINT_SYN;
	AfxGetMainWnd()->SendMessage(IDM_ADMIN_COMMDN,(WPARAM)&adminReq,0);
}
void CAdminControlDlg::OnExcute()
{
	BYTE cbIndex=0;
	bool bFlags=false;
	m_cbControlStyle=0;
	m_cbExcuteTimes=0;
	ZeroMemory(m_bWinArea,sizeof(m_bWinArea));

	//控制庄家
	if(((CButton*)GetDlgItem(IDC_RADIO_CT_BANKER))->GetCheck())
	{
		if(((CButton*)GetDlgItem(IDC_RADIO_WIN))->GetCheck())
		{
			m_cbControlStyle=CS_BANKER_WIN;
			bFlags=true;
		}
		else
		{
			if(((CButton*)GetDlgItem(IDC_RADIO_LOSE))->GetCheck())
			{
				m_cbControlStyle=CS_BANKER_LOSE;
				bFlags=true;
			}
		}
	}
	else //控制区域
	{
		if(((CButton*)GetDlgItem(IDC_RADIO_CT_AREA))->GetCheck())
		{
			m_cbControlStyle=CS_BET_AREA;
			for(cbIndex=0;cbIndex<CONTROL_AREA;cbIndex++)
				m_bWinArea[cbIndex]=((CButton*)GetDlgItem(IDC_CHECK_TIAN+cbIndex))->GetCheck()?true:false;
			bFlags=true;
		}
	}

	m_cbExcuteTimes=(BYTE)((CComboBox*)GetDlgItem(IDC_COMBO_TIMES))->GetCurSel()+1;
	//获取执行次数
	if(m_cbExcuteTimes<=0)
	{
		bFlags=false;
	}
	

	if(bFlags) //参数有效
	{
		CMD_C_AdminReq adminReq;
		adminReq.cbReqType=RQ_SET_WIN_AREA;
		tagAdminReq*pAdminReq=(tagAdminReq*)adminReq.cbExtendData;
		pAdminReq->m_cbExcuteTimes=m_cbExcuteTimes;
		pAdminReq->m_cbControlStyle=m_cbControlStyle;
		memcpy(pAdminReq->m_bWinArea,m_bWinArea,sizeof(m_bWinArea));
		AfxGetMainWnd()->SendMessage(IDM_ADMIN_COMMDN,(WPARAM)&adminReq,0);
	}
	else
	{
		AfxMessageBox(TEXT("请选择受控次数以及受控方式!"));
		OnRefresh();
	}
}
void CAdminControlDlg::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类

	CDialog::OnCancel();
}

BOOL CAdminControlDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	SetDlgItemText(IDC_BT_RESET,TEXT("取消控制"));	
	SetDlgItemText(IDC_BT_CURSET,TEXT("当前设置"));
	SetDlgItemText(IDC_BT_EXCUTE,TEXT("执行"));	
	SetDlgItemText(IDC_BT_CANCEL,TEXT("取消"));	
	SetDlgItemText(IDC_RADIO_WIN,TEXT("庄家赢"));	
	SetDlgItemText(IDC_RADIO_LOSE,TEXT("庄家输"));			 
	SetDlgItemText(IDC_RADIO_CT_BANKER,TEXT("庄家控制"));	
	SetDlgItemText(IDC_RADIO_CT_AREA,TEXT("区域控制"));	
	SetDlgItemText(IDC_CHECK_TIAN,TEXT("龙"));	
	SetDlgItemText(IDC_CHECK_DI,TEXT("虎"));	
	SetDlgItemText(IDC_CHECK_XUAN,TEXT("豹"));
	SetDlgItemText(IDC_STATIC_TIMES	,TEXT("控制次数"));	
	SetDlgItemText(IDC_STATIC_CHOICE,TEXT("控制选项"));	
	SetDlgItemText(IDC_STATIC_NOTICE,TEXT("说明"));
	SetDlgItemText(IDC_STATIC_NOTICE1,TEXT("1.区域输赢控制比游戏库存控制策略优先。"));	
	SetDlgItemText(IDC_STATIC_NOTICE2,TEXT("2.区域控制，选中为胜，未选中为输。"));	

	((CComboBox*)GetDlgItem(IDC_COMBO_TIMES))->AddString(TEXT("1"));
	((CComboBox*)GetDlgItem(IDC_COMBO_TIMES))->AddString(TEXT("2"));
	((CComboBox*)GetDlgItem(IDC_COMBO_TIMES))->AddString(TEXT("3"));
	((CComboBox*)GetDlgItem(IDC_COMBO_TIMES))->AddString(TEXT("4"));
	((CComboBox*)GetDlgItem(IDC_COMBO_TIMES))->AddString(TEXT("5"));

	((CButton*)GetDlgItem(IDC_RADIO_CT_AREA))->SetCheck(1);
	OnRadioClick();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CAdminControlDlg::PrintCurStatus()
{
	CString str;
	switch(m_cbControlStyle)
	{
	case CS_BET_AREA:
		{
			str=TEXT("胜利区域:");
			BYTE cbIndex=0;
			bool bFlags=false;
			for(cbIndex=0;cbIndex<AREA_COUNT;cbIndex++)
			{
				if(m_bWinArea[cbIndex])
				{
					bFlags=true;
					switch(cbIndex)
					{
						case 0:str+=TEXT("龙 ");break;
						case 1:str+=TEXT("虎 ");break;
						case 2:str+=TEXT("豹 ");break;
						default: break;
					}
				}
			}
			if(!bFlags)
			{
				str+=TEXT("庄家通杀");
			}
			str.AppendFormat(TEXT(",执行次数:%d"),m_cbExcuteTimes);
			break;
		}
	case CS_BANKER_WIN:
		{
			str.Format(TEXT("庄家受控,受控方式:赢,执行次数:%d"),m_cbExcuteTimes);break;
		}
	case CS_BANKER_LOSE:
		{
			str.Format(TEXT("庄家受控,受控方式:输,执行次数:%d"),m_cbExcuteTimes);break;
		}
	default: str=TEXT("非控制状态");break;
	}	

	SetWindowText(str);
}

VOID CAdminControlDlg::OnAllowControl(bool bEnable)
{
	GetDlgItem(IDC_BT_EXCUTE)->EnableWindow(bEnable);
}
void CAdminControlDlg::OnTimer(UINT nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	KillTimer(1);
	PrintCurStatus();
	CDialog::OnTimer(nIDEvent);
}
