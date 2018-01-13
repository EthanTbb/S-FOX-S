// ClientControlItemSink.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Resource.h"
#include "ClientControlItemSink.h"
#include ".\clientcontrolitemsink.h"


WNDPROC OldHeadProc;		//ϵͳ���ڹ���
int		g_nSortKey=0;		//���йؼ���
bool	g_nSortWay=false;	//false��ʾ�ӵ͵���
//�б�ͷ�����϶�
static LRESULT CALLBACK NewHeadProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HDHITTESTINFO hti;
	switch(uMsg)
	{
	case WM_SETCURSOR:
		{
			return 0;
		}
	case WM_LBUTTONDOWN:
		{
			hti.pt.x=LOWORD(lParam);
			hti.pt.y=HIWORD(lParam);
			SendMessage(hWnd,HDM_HITTEST,0,(LPARAM) &hti);
			if((hti.flags & HHT_ONDIVIDER) || (hti.flags & HHT_ONDIVOPEN))		//��ֹ�϶�
			{
				return 0;
			}
			if(hti.iItem<=2 || hti.iItem>13)
			{
				return 0;
			}
			break;
		}

	}
	return CallWindowProc((WNDPROC)GetProp(hWnd,_T("SystemHeadProc")),hWnd, uMsg, wParam, lParam);
}
/*�ú�������-1�������һ����Ӧ�ڵڶ���ǰ�棻 
����1�����һ����Ӧ�ڵڶ�����棻 
����0����������ȡ�*/
static int CALLBACK MyCompareProc(LPARAM lParam11, LPARAM lParam12, LPARAM lParam1sort)
{
	int row1=(int )lParam11;
	int row2=(int )lParam12;
	CListCtrl *lc =(CListCtrl *)lParam1sort;
	//�����ж��Ƿ��ǹ�ע���
	CString lp1=lc->GetItemText(row1,1);
	CString lp2=lc->GetItemText(row2,1);
	int n1= _ttoi(lp1);
	int n2= _ttoi(lp2);
	if(n1!=n2)
	{
		return n1>n2?-1:1;
	}

	//�ȽϹؼ���
	lp1=lc->GetItemText(row1,g_nSortKey);
	lp2=lc->GetItemText(row2,g_nSortKey);
	LONGLONG l1 = _ttoi64(lp1);
	LONGLONG l2 = _ttoi64(lp2);

	return g_nSortWay?l1-l2:l2-l1;
}
// CClientControlItemSinkDlg �Ի���

IMPLEMENT_DYNAMIC(CClientControlItemSinkDlg, IClientControlDlg)

CClientControlItemSinkDlg::CClientControlItemSinkDlg(CWnd* pParent /*=NULL*/)
	: IClientControlDlg(CClientControlItemSinkDlg::IDD, pParent)
{
	m_lStorageStart = 0;
	m_lStorageDeduct = 0;
	m_lStorageCurrent = 0;
	m_lStorageMax1 = 0;
	m_lStorageMul1 = 0;
	m_lStorageMax2 = 0;
	m_lStorageMul2 = 0;
	m_UserBetArray.RemoveAll();
	m_bShowRobot=true;
	m_lQueryGameID = -1;
}

CClientControlItemSinkDlg::~CClientControlItemSinkDlg()
{
}

void CClientControlItemSinkDlg::DoDataExchange(CDataExchange* pDX)
{
	IClientControlDlg::DoDataExchange(pDX);
	DDX_Text(pDX,IDC_EDIT_STORAGE_START,m_lStorageStart);
	DDX_Text(pDX,IDC_EDIT_STORAGE_DEDUCT,m_lStorageDeduct);
	DDX_Text(pDX,IDC_EDIT_STORAGE_CURRENT,m_lStorageCurrent);
	DDX_Text(pDX,IDC_EDIT_STORAGE_MAX1,m_lStorageMax1);
	DDX_Text(pDX,IDC_EDIT_STORAGE_MUL1,m_lStorageMul1);
	DDX_Text(pDX,IDC_EDIT_STORAGE_MAX2,m_lStorageMax2);
	DDX_Text(pDX,IDC_EDIT_STORAGE_MUL2,m_lStorageMul2);

	CheckDataMinMax(m_lStorageStart);
	CheckDataMinMax(m_lStorageDeduct);
	CheckDataMinMax(m_lStorageMax1);
	CheckDataMinMax(m_lStorageMul1);
	CheckDataMinMax(m_lStorageMax2);
	CheckDataMinMax(m_lStorageMul2);

	DDX_Control(pDX, IDC_LIST_USER_BET, m_listUserBet);
	DDX_Control(pDX, IDC_LIST_USER_BET_ALL, m_listUserBetAll);
	DDX_Control(pDX, IDC_LIST_ATTENTION_USER, m_ListAttention);
}


BEGIN_MESSAGE_MAP(CClientControlItemSinkDlg, IClientControlDlg)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON_RESET, OnBnClickedButtonReset)
	ON_BN_CLICKED(IDC_BUTTON_SYN, OnBnClickedButtonSyn)
	ON_BN_CLICKED(IDC_BUTTON_OK, OnBnClickedButtonOk)
	ON_BN_CLICKED(IDC_BUTTON_CANCEL, OnBnClickedButtonCancel)
	ON_BN_CLICKED(IDC_BTN_REFRESH_KUCUN,OnRefreshKuCun)
	ON_BN_CLICKED(IDC_RADIO_CT_BANKER, OnBnClickedRadioCtBanker)
	ON_BN_CLICKED(IDC_RADIO_CT_AREA, OnBnClickedRadioCtArea)

	ON_BN_CLICKED(IDC_BTN_UPDATE_STORAGE, OnBnClickedBtnUpdateStorage)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_USER_BET, OnNMDblclkListUserBet)
	ON_BN_CLICKED(IDC_BUTTON_ADDUSER, OnBnClickedButtonAdduser)
	ON_BN_CLICKED(IDC_BUTTON_DEL, OnBnClickedButtonDel)
	ON_CBN_SELCHANGE(IDC_COMBO_CHOOSE, OnCbnSelchangeComboChoose)
	ON_BN_CLICKED(IDC_CHECK1, OnBnClickedCheck1)
	ON_COMMAND_RANGE(IDC_BUTTON_CAR_1,IDC_BUTTON_CAR_8,OnBnClickedButtonCar)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST_USER_BET, OnLvnColumnclickListUserBet)
END_MESSAGE_MAP()

// CClientControlItemSinkDlg ��Ϣ�������

//��ʼ��
BOOL CClientControlItemSinkDlg::OnInitDialog()
{
	IClientControlDlg::OnInitDialog();

	//������ʾ
	((CComboBox*)GetDlgItem(IDC_COMBO_CHOOSE))->AddString(TEXT("�������"));
	((CComboBox*)GetDlgItem(IDC_COMBO_CHOOSE))->AddString(TEXT("��ע���"));
	((CComboBox*)GetDlgItem(IDC_COMBO_CHOOSE))->SetCurSel(0);
	m_nShowWay=0;
	//������Ϣ
	((CComboBox*)GetDlgItem(IDC_COMBO_TIMES))->AddString(TEXT("1"));
	((CComboBox*)GetDlgItem(IDC_COMBO_TIMES))->AddString(TEXT("2"));
	((CComboBox*)GetDlgItem(IDC_COMBO_TIMES))->AddString(TEXT("3"));
	((CComboBox*)GetDlgItem(IDC_COMBO_TIMES))->AddString(TEXT("4"));
	((CComboBox*)GetDlgItem(IDC_COMBO_TIMES))->AddString(TEXT("5"));
	SetDlgItemText(IDC_STATIC_TIMES,TEXT("���ƾ�����"));
	SetDlgItemText(IDC_STATIC_NOTIC,TEXT("����˵����"));
	

	SetDlgItemText(IDC_RADIO_1,TEXT("��ɯ����"));
	SetDlgItemText(IDC_RADIO_2,TEXT("������"));
	SetDlgItemText(IDC_RADIO_3,TEXT("��������"));
	SetDlgItemText(IDC_RADIO_4,TEXT("��ʱ��"));
	SetDlgItemText(IDC_RADIO_5,TEXT("·��"));
	SetDlgItemText(IDC_RADIO_6,TEXT("����"));
	SetDlgItemText(IDC_RADIO_7,TEXT("�ݱ�"));
	SetDlgItemText(IDC_RADIO_8,TEXT("����"));
	SetDlgItemText(IDC_BUTTON_RESET,TEXT("ȡ������"));
	SetDlgItemText(IDC_BUTTON_SYN,TEXT("������Ϣ"));
	SetDlgItemText(IDC_BUTTON_OK,TEXT("ִ��"));
	SetDlgItemText(IDC_BUTTON_CANCEL,TEXT("ȡ��"));

	OnBnClickedRadioCtArea();
	GetDlgItem(IDC_RADIO_LOSE)->EnableWindow(FALSE);
	GetDlgItem(IDC_RADIO_WIN)->EnableWindow(FALSE);

	//�����ע
	m_listUserBet.SetExtendedStyle(m_listUserBet.GetExtendedStyle()|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);

	INT nColumnCount=0;
	m_listUserBet.InsertColumn(nColumnCount++,TEXT(""),LVCFMT_CENTER,0);
	m_listUserBet.InsertColumn(nColumnCount++,TEXT("��ע"),LVCFMT_CENTER,40);
	m_listUserBet.InsertColumn(nColumnCount++,TEXT("����ǳ�"),LVCFMT_CENTER,82);
	m_listUserBet.InsertColumn(nColumnCount++,TEXT("������"),LVCFMT_CENTER,80);
	m_listUserBet.InsertColumn(nColumnCount++,TEXT("��ɯ����"),LVCFMT_CENTER,80);
	m_listUserBet.InsertColumn(nColumnCount++,TEXT("��������"),LVCFMT_CENTER,80);
	m_listUserBet.InsertColumn(nColumnCount++,TEXT("��ʱ��"),LVCFMT_CENTER,80);
	m_listUserBet.InsertColumn(nColumnCount++,TEXT("·��"),LVCFMT_CENTER,80);
	m_listUserBet.InsertColumn(nColumnCount++,TEXT("����"),LVCFMT_CENTER,80);
	m_listUserBet.InsertColumn(nColumnCount++,TEXT("�ݱ�"),LVCFMT_CENTER,80);
	m_listUserBet.InsertColumn(nColumnCount++,TEXT("����"),LVCFMT_CENTER,80);
	m_listUserBet.InsertColumn(nColumnCount++,TEXT("����ע"),LVCFMT_CENTER,80);
	m_listUserBet.InsertColumn(nColumnCount++,TEXT("���+����"),LVCFMT_CENTER,80);
	m_listUserBet.InsertColumn(nColumnCount++,TEXT("���д��"),LVCFMT_CENTER,80);
	m_listUserBet.InsertColumn(nColumnCount++,TEXT("����Ӯ"),LVCFMT_CENTER,80);
	m_listUserBet.InsertColumn(nColumnCount++,TEXT("���ID"),LVCFMT_CENTER,60);

	CHeaderCtrl *pHead = m_listUserBet.GetHeaderCtrl();
	OldHeadProc=(WNDPROC) SetWindowLong(pHead->m_hWnd, GWL_WNDPROC, (LONG)NewHeadProc);
	SetProp(pHead->m_hWnd,_T("SystemHeadProc"),(HANDLE)OldHeadProc);
	
	m_listUserBetAll.SetExtendedStyle(m_listUserBetAll.GetExtendedStyle()|LVS_EX_GRIDLINES);
	nColumnCount=0;
	m_listUserBetAll.InsertColumn(nColumnCount++,TEXT(""),LVCFMT_CENTER,122);
	for (int i=nColumnCount;i<=nColumnCount+8;i++)
	{
		m_listUserBetAll.InsertColumn(i,TEXT(""),LVCFMT_CENTER,80);
	}

	m_listUserBetAll.InsertItem(0, TEXT(""));
	m_listUserBetAll.SetItemText(0, 0, TEXT("��ע�����עͳ��"));
	for(BYTE cbSubItem=1; cbSubItem <=9; cbSubItem++)
	{
		m_listUserBetAll.SetItemText(0, cbSubItem, TEXT("0"));
	}
	
	m_ListAttention.SetExtendedStyle(m_listUserBet.GetExtendedStyle()|LVS_EX_FULLROWSELECT);
	m_ListAttention.InsertColumn(0,TEXT(""));
	return TRUE; 
}

//������ɫ
HBRUSH CClientControlItemSinkDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = IClientControlDlg::OnCtlColor(pDC, pWnd, nCtlColor);

	if (pWnd->GetDlgCtrlID()==IDC_STATIC_INFO) 
	{  
		pDC->SetTextColor(RGB(255,10,10)); 
	} 
	if (pWnd->GetDlgCtrlID()==IDC_STATIC_TEXT) 
	{  
		pDC->SetTextColor(RGB(255,10,10)); 
	} 
	if (pWnd->GetDlgCtrlID()==IDC_STORAGE_STATIC) 
	{  
		pDC->SetTextColor(RGB(255,10,10)); 
	}
	return hbr;
}

//ȡ������
void CClientControlItemSinkDlg::OnBnClickedButtonReset()
{
	CMD_C_ControlApplication ControlApplication;
	ZeroMemory(&ControlApplication, sizeof(ControlApplication));
	ControlApplication.cbControlAppType = C_CA_CANCELS;
	//AfxGetMainWnd()->SendMessage(IDM_ADMIN_COMMDN,(WPARAM)&ControlApplication,0);
	ReSetAdminWnd();
	CGameFrameEngine * pGameFrameEngine=CGameFrameEngine::GetInstance();
	if (pGameFrameEngine!=NULL) pGameFrameEngine->SendMessage(IDM_ADMIN_COMMDN,(WPARAM)&ControlApplication,0);

}

//���ֿ���
void CClientControlItemSinkDlg::OnBnClickedButtonSyn()
{
	CMD_C_ControlApplication ControlApplication;
	ZeroMemory(&ControlApplication, sizeof(ControlApplication));
	ControlApplication.cbControlAppType = C_CA_UPDATE;
	//AfxGetMainWnd()->SendMessage(IDM_ADMIN_COMMDN,(WPARAM)&ControlApplication,0);

	CGameFrameEngine * pGameFrameEngine=CGameFrameEngine::GetInstance();
	if (pGameFrameEngine!=NULL) pGameFrameEngine->SendMessage(IDM_ADMIN_COMMDN,(WPARAM)&ControlApplication,0);
}

//��������
void CClientControlItemSinkDlg::OnBnClickedButtonOk()
{
	
	BYTE cbIndex=0;
	bool bFlags=false;
	m_cbControlStyle=0;
	m_cbExcuteTimes=0;
	m_cbControlArea = 0xff;

	//����ׯ��
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
	else //��������
	{
		if(((CButton*)GetDlgItem(IDC_RADIO_CT_AREA))->GetCheck())
		{
			m_cbControlStyle=CS_BET_AREA;
			
			if(((CButton*)GetDlgItem(IDC_RADIO_1))->GetCheck())
				m_cbControlArea = 0;
			else if(((CButton*)GetDlgItem(IDC_RADIO_2))->GetCheck())
				m_cbControlArea = 1;
			else if(((CButton*)GetDlgItem(IDC_RADIO_3))->GetCheck())
				m_cbControlArea = 2;
			else if(((CButton*)GetDlgItem(IDC_RADIO_4))->GetCheck())
				m_cbControlArea = 3;
			else if(((CButton*)GetDlgItem(IDC_RADIO_5))->GetCheck())
				m_cbControlArea = 4;
			else if(((CButton*)GetDlgItem(IDC_RADIO_6))->GetCheck())
				m_cbControlArea = 5;
			else if(((CButton*)GetDlgItem(IDC_RADIO_7))->GetCheck())
				m_cbControlArea = 6;
			else if(((CButton*)GetDlgItem(IDC_RADIO_8))->GetCheck())
				m_cbControlArea = 7;
		}

		if(m_cbControlArea==0xff) 
			bFlags=false;
		else
		{
			bFlags=true;
		}
	}

	m_cbExcuteTimes=(BYTE)((CComboBox*)GetDlgItem(IDC_COMBO_TIMES))->GetCurSel()+1;
	//��ȡִ�д���
	if(m_cbExcuteTimes<=0)
	{
		bFlags=false;
	}

	if(bFlags) //������Ч
	{
		CMD_C_ControlApplication ControlApplication;
		ZeroMemory(&ControlApplication, sizeof(ControlApplication));

		ControlApplication.cbControlAppType = C_CA_SET;
		ControlApplication.cbControlArea = m_cbControlArea;
		ControlApplication.cbControlStyle = m_cbControlStyle;
		ControlApplication.cbControlTimes = static_cast<BYTE>(m_cbExcuteTimes);
		//AfxGetMainWnd()->SendMessage(IDM_ADMIN_COMMDN,(WPARAM)&ControlApplication,0);
		CGameFrameEngine * pGameFrameEngine=CGameFrameEngine::GetInstance();
		if (pGameFrameEngine!=NULL) pGameFrameEngine->SendMessage(IDM_ADMIN_COMMDN,(WPARAM)&ControlApplication,0);


	}
	else
	{
		AfxMessageBox(TEXT("��ѡ���ܿش����Լ��ܿط�ʽ!"));
		return;
	}

}

//ȡ���ر�
void CClientControlItemSinkDlg::OnBnClickedButtonCancel()
{
	ShowWindow(SW_HIDE);
}

void CClientControlItemSinkDlg::OnRefreshKuCun()
{

	CString strStorage=TEXT("");
	GetDlgItemText(IDC_EDIT_KUCUN,strStorage);

	LONGLONG lNewStorage=myatoi64(strStorage);

	CMD_C_ControlApplication adminReq;
	adminReq.cbControlAppType=S_CR_ACK_PRINT_STORAGE;
	adminReq.lSetStorage=lNewStorage;
	CGameFrameEngine::GetInstance()->SendMessage(IDM_ADMIN_COMMDN,(WPARAM)&adminReq,0);
}


//���¿���
void  CClientControlItemSinkDlg::UpdateControl( CMD_S_ControlReturns* pControlReturns )
{
	CString str=TEXT("");
	switch(pControlReturns->cbReturnsType)
	{
	case S_CR_FAILURE:
		{
			SetDlgItemText(IDC_STATIC_INFO,TEXT("����ʧ�ܣ�"));
			break;
		}
	case S_CR_UPDATE_SUCCES:
		{
			TCHAR zsText[256] = TEXT("");
			TCHAR zsTextTemp[256] = TEXT("");

			m_cbExcuteTimes=pControlReturns->cbControlTimes;
			m_cbControlStyle=pControlReturns->cbControlStyle;
			m_cbControlArea=pControlReturns->cbControlArea;
			//PrintingInfo(zsTextTemp,256,pControlReturns->cbControlArea,pControlReturns->cbControlTimes);
			myprintf(zsText,CountArray(zsText),TEXT("�������ݳɹ���\r\n %s"),zsTextTemp);
			SetDlgItemText(IDC_STATIC_INFO,zsText);
			
			ReSetAdminWnd();
			
			break;
		}
	case S_CR_SET_SUCCESS:
		{
			TCHAR zsText[256] = TEXT("");
			TCHAR zsTextTemp[256] = TEXT("");
			PrintingInfo(zsTextTemp,256,pControlReturns->cbControlArea,pControlReturns->cbControlTimes);
			myprintf(zsText,CountArray(zsText),TEXT("�������ݳɹ���\r\n %s"),zsTextTemp);
			SetDlgItemText(IDC_STATIC_INFO,zsText);


			switch(m_cbControlStyle)
			{
			case CS_BANKER_WIN:
				{
					str.Format(TEXT("�����ѽ��ܣ����Ʒ�ʽ��ׯ��Ӯ��ִ�д�����%d"),m_cbExcuteTimes);break;
				}
			case CS_BANKER_LOSE:
				{
					str.Format(TEXT("�����ѽ��ܣ����Ʒ�ʽ��ׯ���䣬ִ�д�����%d"),m_cbExcuteTimes);break;
				}
			case CS_BET_AREA:
				{
					
					str.Format(TEXT("�����ѽ��ܣ����Ʒ�ʽ��������ƣ�ִ�д�����%d"),m_cbExcuteTimes);						

					break;
				}
			default:break;
			}


			break;
		}
	case S_CR_CANCEL_SUCCESS:
		{
			SetDlgItemText(IDC_STATIC_TEXT,TEXT("ȡ�����óɹ���"));			
			break;
		}

	case S_CR_ACK_PRINT_STORAGE:
		{
	
			{  
				CString strStorage=TEXT("");
				strStorage.Format(TEXT("%I64d"),pControlReturns->lCurStorage);
				SetDlgItemText(IDC_STATIC_TEXT,strStorage);

			}
			break;
		}
	}

	SetDlgItemText(IDC_STATIC_TEXT,str);	
}



//��Ϣ
void CClientControlItemSinkDlg::PrintingInfo( TCHAR* pText, WORD cbCount, BYTE cbArea, BYTE cbTimes )
{
	if (cbArea == 0xff )
	{
		myprintf(pText,cbCount,TEXT("��ʱ�޿��ơ�"));
		return;
	}

	TCHAR szDesc[32] = TEXT("");
	myprintf(pText,cbCount,TEXT("ʤ������"));
	if ( cbArea == 0 )
	{
		_sntprintf( szDesc,CountArray(szDesc),TEXT("��ɯ���١�20��") );
		lstrcat( pText,szDesc );
	}
	else if ( cbArea == 1 )
	{
		_sntprintf( szDesc,CountArray(szDesc),TEXT("��������20��") );
		lstrcat( pText,szDesc );
	}
	else if ( cbArea == 2 )
	{
		_sntprintf( szDesc,CountArray(szDesc),TEXT("���������20��") );
		lstrcat( pText,szDesc );
	}
	else if ( cbArea == 3 )
	{
		_sntprintf( szDesc,CountArray(szDesc),TEXT("��ʱ�ݡ�20��") );
		lstrcat( pText,szDesc );
	}
	else if ( cbArea == 4 )
	{
		_sntprintf( szDesc,CountArray(szDesc),TEXT("·����5��") );
		lstrcat( pText,szDesc );
	}
	else if ( cbArea == 5 )
	{
		_sntprintf( szDesc,CountArray(szDesc),TEXT("�����5��") );
		lstrcat( pText,szDesc );
	}
	else if ( cbArea == 6 )
	{
		_sntprintf( szDesc,CountArray(szDesc),TEXT("�ݱ���5��") );
		lstrcat( pText,szDesc );
	}
	else if ( cbArea == 7 )
	{
		_sntprintf( szDesc,CountArray(szDesc),TEXT("���ۡ�5��") );
		lstrcat( pText,szDesc );
	}
	myprintf(szDesc,CountArray(szDesc),TEXT("ִ�д�����%d��"), cbTimes);
	lstrcat( pText,szDesc );
}

void CClientControlItemSinkDlg::ReSetAdminWnd(bool bQuickControl)
{	
	if(bQuickControl)
	{
		((CButton*)GetDlgItem(IDC_RADIO_WIN))->SetCheck(0);
		((CButton*)GetDlgItem(IDC_RADIO_LOSE))->SetCheck(0);
		((CButton*)GetDlgItem(IDC_RADIO_CT_BANKER))->SetCheck(0);
	}
	else
	{
		if(m_cbControlStyle!=CS_BANKER_WIN&&m_cbControlStyle!=CS_BANKER_LOSE)
		{
			((CButton*)GetDlgItem(IDC_RADIO_WIN))->SetCheck(0);
			((CButton*)GetDlgItem(IDC_RADIO_LOSE))->SetCheck(0);
		}
		else
		{
			((CButton*)GetDlgItem(IDC_RADIO_CT_BANKER))->SetCheck(1);
			((CButton*)GetDlgItem(IDC_RADIO_WIN))->SetCheck(m_cbControlStyle==CS_BANKER_WIN?1:0);
			((CButton*)GetDlgItem(IDC_RADIO_LOSE))->SetCheck(m_cbControlStyle==CS_BANKER_LOSE?1:0);
		}
	}


	if(m_cbControlStyle==CS_BET_AREA)
	{
		((CButton*)GetDlgItem(IDC_RADIO_CT_AREA))->SetCheck(1);

		((CButton*)GetDlgItem(IDC_RADIO_1))->SetCheck(m_cbControlArea==0?1:0);
		((CButton*)GetDlgItem(IDC_RADIO_2))->SetCheck(m_cbControlArea==1?1:0);
		((CButton*)GetDlgItem(IDC_RADIO_3))->SetCheck(m_cbControlArea==2?1:0);
		((CButton*)GetDlgItem(IDC_RADIO_4))->SetCheck(m_cbControlArea==3?1:0);
		((CButton*)GetDlgItem(IDC_RADIO_5))->SetCheck(m_cbControlArea==4?1:0);
		((CButton*)GetDlgItem(IDC_RADIO_6))->SetCheck(m_cbControlArea==5?1:0);
		((CButton*)GetDlgItem(IDC_RADIO_7))->SetCheck(m_cbControlArea==6?1:0);
		((CButton*)GetDlgItem(IDC_RADIO_8))->SetCheck(m_cbControlArea==7?1:0);
	}
	
	((CComboBox*)GetDlgItem(IDC_COMBO_TIMES))->SetCurSel(m_cbExcuteTimes>0?(bQuickControl?0:m_cbExcuteTimes):-1);

	//OnRadioClick();
	

}

bool CClientControlItemSinkDlg:: InitControl()
{
      ClearText();
	  return true;
}

//ׯ�ҿ���
void CClientControlItemSinkDlg::OnBnClickedRadioCtBanker()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	if(((CButton*)GetDlgItem(IDC_RADIO_CT_BANKER))->GetCheck()==1)
	{
		
		((CButton*)GetDlgItem(IDC_RADIO_CT_AREA))->SetCheck(0);
		
		GetDlgItem(IDC_RADIO_LOSE)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_WIN)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_1)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_2)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_3)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_4)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_5)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_6)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_7)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_8)->EnableWindow(FALSE);
	}
	
}
//�������
void CClientControlItemSinkDlg::OnBnClickedRadioCtArea()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	if(((CButton*)GetDlgItem(IDC_RADIO_CT_AREA))->GetCheck()==1)
	{
		((CButton*)GetDlgItem(IDC_RADIO_CT_BANKER))->SetCheck(0);
		
		GetDlgItem(IDC_RADIO_LOSE)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_WIN)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_1)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_2)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_3)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_4)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_5)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_6)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_7)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_8)->EnableWindow(TRUE);
	}
	((CButton*)GetDlgItem(IDC_RADIO_CT_AREA))->SetCheck(1);
}
//�趨��ǰ���
void CClientControlItemSinkDlg::SetStorageCurrentValue(LONGLONG lvalue)
{
	m_lStorageCurrent=lvalue;
	UpdateData(FALSE);
}


//���¿��
void CClientControlItemSinkDlg::OnBnClickedBtnUpdateStorage()
{
	//��ȡ������
	UpdateData(TRUE); //�ӿؼ���ȡ���ݵ�����

	CMD_C_UpdateStorage adminReq;
	ZeroMemory(&adminReq,sizeof(adminReq));
	adminReq.cbReqType=RQ_SET_STORAGE;

	adminReq.lStorageDeduct = m_lStorageDeduct;
	adminReq.lStorageCurrent = m_lStorageCurrent;
	adminReq.lStorageMax1 = m_lStorageMax1;
	adminReq.lStorageMul1 = m_lStorageMul1;
	adminReq.lStorageMax2 = m_lStorageMax2;
	adminReq.lStorageMul2 = m_lStorageMul2;

	if (m_lStorageDeduct<0||m_lStorageDeduct>1000)
	{
		AfxMessageBox(L"[���˥��ֵ]���뷶ΧΪ0-1000");
		return ;
	}
	if (m_lStorageMul1<0||m_lStorageMul1>100)
	{
		AfxMessageBox(L"[���Ӯ�ָ���1]���뷶ΧΪ0-100");
		return ;
	}
	if (m_lStorageMul2<0||m_lStorageMul2>100)
	{
		AfxMessageBox(L"[���Ӯ�ָ���2]���뷶ΧΪ0-100");
		return ;
	}
	CString str;
	if ((m_lStorageCurrent<0)||(m_lStorageDeduct<0)||
		(m_lStorageMax1<0)||(m_lStorageMax2<0)||(m_lStorageMul1<0)||(m_lStorageMul2<0))
	{
		str=TEXT("��ֵ����Ϊ����������Ч��");
		return;
	}
	else
	{
		CGameFrameEngine * pGameFrameEngine=CGameFrameEngine::GetInstance();
		if (pGameFrameEngine!=NULL) pGameFrameEngine->SendMessage(IDM_UPDATE_STORAGE,(WPARAM)&adminReq,0);
		str=TEXT("����Ѹ��£�");
	}
	SetDlgItemText(IDC_STORAGE_STATIC,str);
}
//ˢ����Ϣ
void CClientControlItemSinkDlg::RequestUpdateStorage()
{
	CMD_C_UpdateStorage adminReq;
	ZeroMemory(&adminReq,sizeof(adminReq));
	adminReq.cbReqType=RQ_REFRESH_STORAGE;

	CGameFrameEngine * pGameFrameEngine=CGameFrameEngine::GetInstance();
	if (pGameFrameEngine!=NULL) pGameFrameEngine->SendMessage(IDM_UPDATE_STORAGE,(WPARAM)&adminReq,0);

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

//���¿��
bool __cdecl CClientControlItemSinkDlg::UpdateStorage(const void * pBuffer)
{

	const CMD_S_UpdateStorage*pResult=(CMD_S_UpdateStorage*)pBuffer;

	//���¿��
	m_lStorageStart = pResult->lStorageStart;
	m_lStorageDeduct = pResult->lStorageDeduct;
	m_lStorageCurrent = pResult->lStorageCurrent;
	m_lStorageMax1 = pResult->lStorageMax1;
	m_lStorageMul1 = pResult->lStorageMul1;
	m_lStorageMax2 = pResult->lStorageMax2;
	m_lStorageMul2 = pResult->lStorageMul2;

	//��������ֵ���ؼ���ʾ
	UpdateData(FALSE);

	SetDlgItemText(IDC_STORAGE_STATIC,TEXT("����޸ĳɹ���"));
	return true;
}
//�����Ϣ
void CClientControlItemSinkDlg::ClearText()
{

	SetDlgItemText(IDC_STORAGE_STATIC,TEXT(""));
	SetDlgItemText(IDC_STATIC_TEXT,TEXT(""));
}


//������ע
void __cdecl CClientControlItemSinkDlg::UpdateUserBet(bool bReSet)
{
	//����б�
	m_listUserBet.DeleteAllItems();

	if(bReSet)
	{
		m_lQueryGameID = -1;

		GetDlgItem(IDC_EDIT_STORAGE_START)->SetFocus();

		for(BYTE cbSubItem=1; cbSubItem <=9; cbSubItem++)
		{
			m_listUserBetAll.SetItemText(0, cbSubItem, TEXT("0"));
		}

		GetDlgItem(IDC_BTN_USER_BET_QUERY)->EnableWindow(true);
		GetDlgItem(IDC_BTN_USER_BET_ALL)->EnableWindow(false);

		return;
	}

	if(0 == m_UserBetArray.GetCount()) return;

	LONGLONG lAllBetScore[AREA_COUNT+1];
	ZeroMemory(lAllBetScore, sizeof(lAllBetScore));

	INT nItemIndex = 0;
	for(INT i=0; i<m_UserBetArray.GetCount(); i++)
	{
		//У���û�ID
		if(-1 != m_lQueryGameID && m_UserBetArray[i].dwUserGameID != m_lQueryGameID) continue;

		//��������
		m_listUserBet.InsertItem(nItemIndex, TEXT(""));

		CString strInfo;
		strInfo.Format(TEXT("%d"), nItemIndex+1);
		m_listUserBet.SetItemText(nItemIndex, 1, strInfo);

		m_listUserBet.SetItemText(nItemIndex, 2, m_UserBetArray[i].szNickName);

		strInfo.Format(TEXT("%u"), m_UserBetArray[i].dwUserGameID);
		m_listUserBet.SetItemText(nItemIndex, 3, strInfo);

		strInfo.Format(TEXT("%I64d"), m_UserBetArray[i].lUserStartScore);
		m_listUserBet.SetItemText(nItemIndex, 4, strInfo);

		strInfo.Format(TEXT("%I64d"), m_UserBetArray[i].lUserWinLost);
		m_listUserBet.SetItemText(nItemIndex, 5, strInfo);


		INT nSubItem = -1;
		for(BYTE j=0; j<AREA_COUNT; j++)
		{
			switch(j)
			{
			case AREA_MASERATI:
				{
					nSubItem = 5;
					break;
				}
			case AREA_FERRARI:
				{
					nSubItem = 6;
					break;
				}
			case AREA_LAMBORGHINI:
				{
					nSubItem = 7;   
					break;		     
				}				  
			case AREA_PORSCHE:		  
				{				    
					nSubItem = 8;
					break;		    
				}				     
			case AREA_LANDROVER:		    
				{				   
					nSubItem = 9 ;
					break;		 
				}
			case AREA_BMW:
				{
					nSubItem = 10;
					break;
				}
			case AREA_JAGUAR:
				{
					nSubItem = 11;
					break;
				}
			case AREA_BENZ:
				{
					nSubItem = 12;
					break;
				}
		 default:
				break;
			}
			nSubItem+=1;
			strInfo.Format(TEXT("%I64d"), m_UserBetArray[i].lUserBet[j]);
			m_listUserBet.SetItemText(nItemIndex, nSubItem, strInfo);

			lAllBetScore[j] += m_UserBetArray[i].lUserBet[j];
		}

		nItemIndex++;
	}

	INT nSubItemAll = -1;
	for(BYTE k=0; k<AREA_COUNT; k++)
	{
		switch(k)
		{
		case AREA_MASERATI:
			{
				nSubItemAll = 3;
				break;
			}
		case AREA_FERRARI:
			{
				nSubItemAll = 4;
				break;
			}
		case AREA_LAMBORGHINI:
			{
				nSubItemAll = 5;
				break;
			}
		case AREA_PORSCHE:
			{
				nSubItemAll = 6;     
				break;			      
			}					   
		case AREA_LANDROVER:			   
			{					     
				nSubItemAll = 7;  
				break;			     
			}					      
		case AREA_BMW:			     
			{					    
				nSubItemAll = 8;   
				break;
			}
		case AREA_JAGUAR:
			{
				nSubItemAll = 9;     
				break;			      
			}
		case AREA_BENZ:
			{
				nSubItemAll = 10;     
				break;			      
			}
	
		default:
			break;
		}
		nSubItemAll-=2;
		CString strInfoAll;
		strInfoAll.Format(TEXT("%I64d"), lAllBetScore[k]);
		m_listUserBetAll.SetItemText(0, nSubItemAll, strInfoAll);
	}

	return;
}

//���¿ؼ�
void __cdecl CClientControlItemSinkDlg::UpdateControl()
{

	RequestUpdateStorage();

}
//��������
bool CClientControlItemSinkDlg::CheckDataMinMax(LONGLONG valueMax)
{


	if (valueMax<0||valueMax>LLONG_MAX)
	{  

		CString str;
		str.Format(L"���뷶ΧΪ0-%I64d",LLONG_MAX);
		AfxMessageBox(str);
	}
	return true;
}
//�����б�
void CClientControlItemSinkDlg::ClearClist()
{
	for(int i=0;i<m_UserBetArray.GetCount();i++)
	{
		ZeroMemory(m_UserBetArray.GetAt(i).lUserBet,sizeof(m_UserBetArray.GetAt(i).lUserBet));
	}
	UpdateUserList();	
}

//�����б�
void CClientControlItemSinkDlg::UpdateUserList(bool bDelUser)
{
	//У��
	if(0 == m_UserBetArray.GetCount()) return;
	m_listUserBet.DeleteAllItems();

	//��������
	INT nItemIndex = 0;
	LONGLONG lAttentionTotalBet[AREA_COUNT+1] = {0};
	int nUserCount = 0;
	int nBetUserCount = 0;
	CString strInfo;

	for (int i=0;i<m_UserBetArray.GetCount();i++)
	{
		if( m_UserBetArray[i].bAndroid && !m_bShowRobot) continue;
		if(!m_UserBetArray[i].bAndroid) nUserCount++;
		if(!m_UserBetArray[i].bAttention && m_nShowWay==1) continue;
		
		//������
		m_listUserBet.InsertItem(nItemIndex, TEXT("0"));
		//��������
		LONGLONG lUserTotalBet = 0L;
		//��ע
		strInfo.Format(_T("%d"), m_UserBetArray[i].bAttention);
		m_listUserBet.SetItemText(nItemIndex,1,strInfo);
		//����ǳ�
		strInfo.Format(_T("%s"), m_UserBetArray[i].szNickName);
		m_listUserBet.SetItemText(nItemIndex,2,strInfo);
		//��ע

		strInfo.Format(_T("%I64d"), m_UserBetArray[i].lUserBet[AREA_FERRARI]);
		m_listUserBet.SetItemText(nItemIndex,3,strInfo);
		lUserTotalBet+=m_UserBetArray[i].lUserBet[AREA_FERRARI];
		if(m_UserBetArray[i].bAttention) lAttentionTotalBet[0]+=m_UserBetArray[i].lUserBet[AREA_FERRARI];

		strInfo.Format(_T("%I64d"), m_UserBetArray[i].lUserBet[AREA_MASERATI]);
		m_listUserBet.SetItemText(nItemIndex,4,strInfo);
		lUserTotalBet+=m_UserBetArray[i].lUserBet[AREA_MASERATI];
		if(m_UserBetArray[i].bAttention) lAttentionTotalBet[1]+=m_UserBetArray[i].lUserBet[AREA_MASERATI];

		for(int j=AREA_LAMBORGHINI;j<AREA_COUNT;j++)
		{
			strInfo.Format(_T("%I64d"), m_UserBetArray[i].lUserBet[j]);
			m_listUserBet.SetItemText(nItemIndex,3+j,strInfo);
			lUserTotalBet+=m_UserBetArray[i].lUserBet[j];
			if(m_UserBetArray[i].bAttention) lAttentionTotalBet[j]+=m_UserBetArray[i].lUserBet[j];

		}

		//����ע
		strInfo.Format(_T("%I64d"), lUserTotalBet);
		m_listUserBet.SetItemText(nItemIndex,11,strInfo);
		if(m_UserBetArray[i].bAttention) lAttentionTotalBet[8]+=lUserTotalBet;
		//���+����
		strInfo.Format(_T("%I64d"), m_UserBetArray[i].lUserInsure+m_UserBetArray[i].lUserStartScore);
		m_listUserBet.SetItemText(nItemIndex,12,strInfo);
		//���д��
		strInfo.Format(_T("%I64d"), m_UserBetArray[i].lUserInsure);
		m_listUserBet.SetItemText(nItemIndex,13,strInfo);
		//����Ӯ
		strInfo.Format(_T("%I64d"), m_UserBetArray[i].lUserWinLost);
		m_listUserBet.SetItemText(nItemIndex,14,strInfo);
		//���ID
		strInfo.Format(_T("%d"), m_UserBetArray[i].dwUserGameID);
		m_listUserBet.SetItemText(nItemIndex,15,strInfo);
		if(lUserTotalBet>0 && !m_UserBetArray[i].bAndroid) nBetUserCount++;
	}

	//��ע���ͳ��
	for (int i=0;i<AREA_COUNT+1;i++)
	{
		strInfo.Format(_T("%I64d"),lAttentionTotalBet[i]);
		m_listUserBetAll.SetItemText(0,i+1,strInfo);
	}
	//���ͳ��
	strInfo.Format(_T("%d"),nUserCount);
	((CStatic*)GetDlgItem(IDC_STATIC_USERCOUNT))->SetWindowText(strInfo);
	strInfo.Format(_T("%d"),nBetUserCount);
	((CStatic*)GetDlgItem(IDC_STATIC_BETCOUNT))->SetWindowText(strInfo);

	if(g_nSortKey!=0)
	{
		//����׼�� �������ò�Ȼ�޷�����
		for(int i=0;i<m_listUserBet.GetItemCount();i++)  m_listUserBet.SetItemData(i,i);
		m_listUserBet.SortItems(MyCompareProc,(DWORD)&m_listUserBet);
	}

	//ɾ�����
	if(bDelUser)	
	{		
		for(int i=0;i<m_ListAttention.GetItemCount();i++)
		{
			strInfo =m_ListAttention.GetItemText(i,0);
			bool bDel = true;
			for(int j=0;j<m_UserBetArray.GetCount();j++)
			{
				if(m_UserBetArray[j].bAndroid) continue;
				if(strInfo.Compare(m_UserBetArray[j].szNickName)==0)
				{
					bDel = false;
					break;
				}
			}
			if(bDel)
			{
				m_ListAttention.DeleteItem(i);
				return;
			}
		}
	}
}
//��ӹ�ע
void CClientControlItemSinkDlg::OnNMDblclkListUserBet(NMHDR *pNMHDR, LRESULT *pResult)
{
	NM_LISTVIEW *pNMListView=(NM_LISTVIEW *)pNMHDR;
	int nItem=pNMListView->iItem;
	*pResult = 0;
	if(nItem>=0 && nItem<m_listUserBet.GetItemCount()) //�ж�˫��λ���Ƿ��������ݵ��б�������
	{
		CString str = m_listUserBet.GetItemText(nItem,2);

		for(int i=0;i<m_UserBetArray.GetCount();i++)
		{
			if(0==str.Compare(m_UserBetArray[i].szNickName))
			{
				//�����˲��ܱ���ע
				if(m_UserBetArray[i].bAndroid) return;
				//�Ѿ�����ע
				if(m_UserBetArray[i].bAttention)
				{
					ASSERT(m_ListAttention.GetItemCount()>0);
					for(int j=0;j<m_ListAttention.GetItemCount();j++)
					{
						CString strAttention = m_ListAttention.GetItemText(j,0);
						if(strAttention.Compare(str)==0)
						{
							m_ListAttention.SetItemState(i, LVIS_SELECTED, LVIS_SELECTED);
						}						
					}
					return;
				}
				
				//��ӹ�ע
				m_UserBetArray[i].bAttention = true;
				//������
				int nAddItem = m_ListAttention.InsertItem(m_ListAttention.GetItemCount(), TEXT("0"));

				m_ListAttention.SetItemText(nAddItem,0,str);
				m_ListAttention.SetItemState(nAddItem, LVIS_SELECTED, LVIS_SELECTED);

				SetDlgItemText(IDC_EDIT_ADDUSER,_T(""));
				//�����б�
				UpdateUserList();
				return;
			}
		}

	}

}
//��ӹ�ע
void CClientControlItemSinkDlg::OnBnClickedButtonAdduser()
{
	CString str;
	((CEdit*)GetDlgItem(IDC_EDIT_ADDUSER))->GetWindowText(str);
	if(str.IsEmpty()==false)
	{
		for(int i=0;i<m_UserBetArray.GetCount();i++)
		{
			if(0==str.Compare(m_UserBetArray[i].szNickName))
			{
				if(m_UserBetArray[i].bAndroid) 	
				{
					SetDlgItemText(IDC_STATIC_TEXT,_T("�������޷�����ע!"));
					return;
				}

				if(m_UserBetArray[i].bAttention)
				{
					ASSERT(m_ListAttention.GetItemCount()>0);
					for(int j=0;j<m_ListAttention.GetItemCount();j++)
					{
						CString strAttention = m_ListAttention.GetItemText(j,0);
						if(strAttention.Compare(str)==0)
						{
							SetDlgItemText(IDC_STATIC_TEXT,_T("������Ѿ�����ע!"));
							m_ListAttention.SetItemState(i, LVIS_SELECTED, LVIS_SELECTED);
						}						
					}
					return;
				}

				m_UserBetArray[i].bAttention = true;
				//������
				int nAddItem = m_ListAttention.InsertItem(m_ListAttention.GetItemCount(), TEXT("0"));

				m_ListAttention.SetItemText(nAddItem,0,str);
				m_ListAttention.SetItemState(nAddItem, LVIS_SELECTED, LVIS_SELECTED);

				SetDlgItemText(IDC_EDIT_ADDUSER,_T(""));
				//�����б�
				UpdateUserList();
				return;
			}	
		}

		SetDlgItemText(IDC_STATIC_TEXT,_T("û�и����!"));
		return;

	}
}
//ɾ����ע
void CClientControlItemSinkDlg::OnBnClickedButtonDel()
{
	CString str;
	POSITION pos=m_ListAttention.GetFirstSelectedItemPosition(); //posѡ�е�����λ��
	if(pos==NULL) return;
	else
	{
		while(pos) //���ѡ�����
		{
			int nIdx=-1;
			nIdx= m_ListAttention.GetNextSelectedItem(pos);

			if(nIdx >=0&&nIdx<m_ListAttention.GetItemCount())
			{
				str =m_ListAttention.GetItemText(nIdx,0);
				for(int i=0;i<m_UserBetArray.GetCount();i++)
				{
					if(str.Compare(m_UserBetArray[i].szNickName)==0)
					{
						ASSERT(m_UserBetArray[i].bAttention);
						m_UserBetArray[i].bAttention=false;
						m_ListAttention.DeleteItem(nIdx);
					}
				}
			}
		}
		//�����б�
		UpdateUserList();

	}
}

//�ı���ע��ʾ
void CClientControlItemSinkDlg::OnCbnSelchangeComboChoose()
{
	if(m_nShowWay!=((CComboBox*)GetDlgItem(IDC_COMBO_CHOOSE))->GetCurSel())
	{
		m_nShowWay=((CComboBox*)GetDlgItem(IDC_COMBO_CHOOSE))->GetCurSel();
		UpdateUserList();	
	}
}
//��������ע��ʾ
void CClientControlItemSinkDlg::OnBnClickedCheck1()
{
	if(BST_CHECKED == IsDlgButtonChecked(IDC_CHECK1))	m_bShowRobot=false;
	else	m_bShowRobot = true;

	UpdateUserList();
}
//��ݿ���
void CClientControlItemSinkDlg::OnBnClickedButtonCar(UINT nID)
{
	m_cbControlStyle=CS_BET_AREA;
	m_cbExcuteTimes=1;
	switch(nID)
	{
	case IDC_BUTTON_CAR_1:{m_cbControlArea = AREA_LANDROVER;	break;}
	case IDC_BUTTON_CAR_2:{m_cbControlArea = AREA_BMW;			break;}
	case IDC_BUTTON_CAR_3:{m_cbControlArea = AREA_FERRARI;		break;}
	case IDC_BUTTON_CAR_4:{m_cbControlArea = AREA_MASERATI;		break;}
	case IDC_BUTTON_CAR_5:{m_cbControlArea = AREA_LAMBORGHINI;	break;}
	case IDC_BUTTON_CAR_6:{m_cbControlArea = AREA_PORSCHE;		break;}
	case IDC_BUTTON_CAR_7:{m_cbControlArea = AREA_JAGUAR;		break;}
	case IDC_BUTTON_CAR_8:{m_cbControlArea = AREA_BENZ;			break;}
	}
	//������Ϣ
	CMD_C_ControlApplication ControlApplication;
	ZeroMemory(&ControlApplication, sizeof(ControlApplication));

	ControlApplication.cbControlAppType = C_CA_SET;
	ControlApplication.cbControlArea = m_cbControlArea;
	ControlApplication.cbControlStyle = m_cbControlStyle;
	ControlApplication.cbControlTimes = static_cast<BYTE>(m_cbExcuteTimes);
	CGameFrameEngine * pGameFrameEngine=CGameFrameEngine::GetInstance();
	if (pGameFrameEngine!=NULL) pGameFrameEngine->SendMessage(IDM_ADMIN_COMMDN,(WPARAM)&ControlApplication,0);

	//ˢ�¿���
	ReSetAdminWnd(true);
}
//����ð������
// void Bubble_2 ( int r[], int n){  
// 	int low = 0;   
// 	int high= n -1; //���ñ����ĳ�ʼֵ  
// 	int tmp,j;  
// 	while (low < high) {  
// 		for (j= low; j< high; ++j) //����ð��,�ҵ������  
// 			if (r[j]> r[j+1]) {  
// 				tmp = r[j]; r[j]=r[j+1];r[j+1]=tmp;  
// 			}   
// 			--high;                 //�޸�highֵ, ǰ��һλ  
// 			for ( j=high; j>low; --j) //����ð��,�ҵ���С��  
// 				if (r[j]<r[j-1]) {  
// 					tmp = r[j]; r[j]=r[j-1];r[j-1]=tmp;  
// 				}  
// 				++low;                  //�޸�lowֵ,����һλ  
// 	}   
// }  

//�����б�
void CClientControlItemSinkDlg::OnLvnColumnclickListUserBet(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	g_nSortKey = pNMLV->iSubItem;
	g_nSortWay = !g_nSortWay;
	//����׼�� �������ò�Ȼ�޷�����
	for(int i=0;i<m_listUserBet.GetItemCount();i++)  m_listUserBet.SetItemData(i,i);

	m_listUserBet.SortItems(MyCompareProc,(DWORD)&m_listUserBet);
	*pResult = 0;
}

//��עͳ��
void CClientControlItemSinkDlg::UpdateAllUserBet(LONGLONG lAllUserBet[AREA_COUNT+1],bool bClear)
{
	if(bClear)
	{
		for(int i=0;i<AREA_COUNT;i++)
		{
			SetDlgItemText(IDC_EDIT_CAR_1+i,_T("0"));

		}
		return;
	}
	else
	{
		CString strInfo;
		strInfo.Format(_T("%I64d"),lAllUserBet[AREA_LANDROVER+1]);
		SetDlgItemText(IDC_EDIT_CAR_1,strInfo);
		strInfo.Format(_T("%I64d"),lAllUserBet[AREA_BMW+1]);
		SetDlgItemText(IDC_EDIT_CAR_2,strInfo);
		strInfo.Format(_T("%I64d"),lAllUserBet[AREA_FERRARI+1]);
		SetDlgItemText(IDC_EDIT_CAR_3,strInfo);
		strInfo.Format(_T("%I64d"),lAllUserBet[AREA_MASERATI+1]);
		SetDlgItemText(IDC_EDIT_CAR_4,strInfo);
		strInfo.Format(_T("%I64d"),lAllUserBet[AREA_LAMBORGHINI+1]);
		SetDlgItemText(IDC_EDIT_CAR_5,strInfo);
		strInfo.Format(_T("%I64d"),lAllUserBet[AREA_PORSCHE+1]);
		SetDlgItemText(IDC_EDIT_CAR_6,strInfo);
		strInfo.Format(_T("%I64d"),lAllUserBet[AREA_JAGUAR+1]);
		SetDlgItemText(IDC_EDIT_CAR_7,strInfo);
		strInfo.Format(_T("%I64d"),lAllUserBet[AREA_BENZ+1]);
		SetDlgItemText(IDC_EDIT_CAR_8,strInfo);

	}
}
