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
	m_lStorageStart = 0;
	m_lStorageDeduct = 0;
	m_lStorageCurrent = 0;
	m_lStorageMax1 = 0;
	m_lStorageMul1 = 0;
	m_lStorageMax2 = 0;
	m_lStorageMul2 = 0;
	m_cbTotalCount = 0;

	m_UserBetArray.RemoveAll();

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
	DDX_Text(pDX,IDC_TOTAL_COUNT,m_cbTotalCount);
	DDX_Control(pDX, IDC_LIST_USER_BET, m_listUserBet);
}

//������
bool  CClientControlItemSinkDlg::ReqResult(const void * pBuffer)
{
	const CMD_S_CommandResult*pResult=(CMD_S_CommandResult*)pBuffer;
	CString str;
	switch(pResult->cbAckType)
	{
	case ACK_SET_WIN_AREA:
		{
			if(pResult->cbResult==CR_ACCEPT)
			{  
				str=TEXT("������Ч!");
			}
			else if (pResult->cbResult==CR_INVALID)
			{
				str=TEXT("����ʧЧ!");
			}
			else
			{
				str=TEXT("����ʧ��!");
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
				((CButton*)GetDlgItem(IDC_SINGLE_CONTROL))->SetCheck(1);
				((CButton*)GetDlgItem(IDC_TOTAL_CONTROL))->SetCheck(0);
				m_cbTotalCount = pResult->cbTotalCount;
				((CComboBox*)GetDlgItem(IDC_COMBO_SICBO_ONE))->SetCurSel(0);
				((CComboBox*)GetDlgItem(IDC_COMBO_SICBO_TWO))->SetCurSel(0);
				((CComboBox*)GetDlgItem(IDC_COMBO_SICBO_THREE))->SetCurSel(0);
				//��ȡ������
				UpdateData(FALSE);
				OnBnClickedControl();
				str=TEXT("���������ѽ���!");
				SetTimer(1,1000,0);
			}
			break;
		}
	case ACK_PRINT_SYN:
		{
			if(pResult->cbResult==CR_ACCEPT)
			{
				if (pResult->cbLastType==ACK_SET_WIN_AREA)
				{
					if (pResult->cbTotalCount!=0)
					{
						((CButton*)GetDlgItem(IDC_TOTAL_CONTROL))->SetCheck(1);
						((CButton*)GetDlgItem(IDC_SINGLE_CONTROL))->SetCheck(0);
						GetDlgItem(IDC_COMBO_SICBO_ONE)->EnableWindow(FALSE);
						GetDlgItem(IDC_COMBO_SICBO_TWO)->EnableWindow(FALSE);
						GetDlgItem(IDC_COMBO_SICBO_THREE)->EnableWindow(FALSE);
						GetDlgItem(IDC_TOTAL_COUNT)->EnableWindow(TRUE);
						m_cbTotalCount = pResult->cbTotalCount;
						//��������ֵ���ؼ���ʾ
						UpdateData(FALSE);
						str.Format(TEXT("��ǰ�����ܵ���Ϊ��%d"),m_cbTotalCount);
					}
					else
					{
						((CButton*)GetDlgItem(IDC_SINGLE_CONTROL))->SetCheck(1);
						((CButton*)GetDlgItem(IDC_TOTAL_CONTROL))->SetCheck(0);
						GetDlgItem(IDC_COMBO_SICBO_ONE)->EnableWindow(TRUE);
						GetDlgItem(IDC_COMBO_SICBO_TWO)->EnableWindow(TRUE);
						GetDlgItem(IDC_COMBO_SICBO_THREE)->EnableWindow(TRUE);
						GetDlgItem(IDC_TOTAL_COUNT)->EnableWindow(FALSE);
						CString cs;
						cs.Format(TEXT("%d"),pResult->cbSicbo1);
						int nIndexID = ((CComboBox*)GetDlgItem(IDC_COMBO_SICBO_ONE))->FindStringExact(0,cs);
						if (nIndexID > CB_ERR)
						{
							((CComboBox*)GetDlgItem(IDC_COMBO_SICBO_ONE))->SetCurSel(nIndexID);
							nIndexID = CB_ERR;
						}

						cs.Format(TEXT("%d"),pResult->cbSicbo2);
						nIndexID = ((CComboBox*)GetDlgItem(IDC_COMBO_SICBO_TWO))->FindStringExact(0,cs);
						if (nIndexID > CB_ERR)
						{
							((CComboBox*)GetDlgItem(IDC_COMBO_SICBO_TWO))->SetCurSel(nIndexID);
							nIndexID = CB_ERR;
						}

						cs.Format(TEXT("%d"),pResult->cbSicbo3);
						nIndexID = ((CComboBox*)GetDlgItem(IDC_COMBO_SICBO_THREE))->FindStringExact(0,cs);
						if (nIndexID > CB_ERR)
						{
							((CComboBox*)GetDlgItem(IDC_COMBO_SICBO_THREE))->SetCurSel(nIndexID);
						}

						str.Format(TEXT("��ǰ����--����1��%d������2��%d������3��%d"),pResult->cbSicbo1,pResult->cbSicbo2,pResult->cbSicbo3);
					}	
				}
				else
				{
					str.Format(TEXT("��ǰ�޿��ƣ�"));
				}
				SetTimer(1,1000,0);
			}
			else
			{
				str.Format(TEXT("����ʧ��!"));
			}
			break;
		}

	default: break;
	}

	SetDlgItemText(IDC_STATIC_STATE,str);
	return true;
}

void CClientControlItemSinkDlg::ReSetAdminWnd()
{	
}

BEGIN_MESSAGE_MAP(CClientControlItemSinkDlg, IClientControlDlg)
	ON_BN_CLICKED(IDC_BT_RESET,OnReSet)
	ON_BN_CLICKED(IDC_BT_EXCUTE,OnExcute)
	ON_BN_CLICKED(IDC_BT_CURSET,OnRefresh)
	ON_BN_CLICKED(IDC_BTN_UPDATE_STORAGE,OnFreshStorage)
	ON_BN_CLICKED(IDC_SINGLE_CONTROL, OnBnClickedControl)
	ON_BN_CLICKED(IDC_TOTAL_CONTROL, OnBnClickedControl)
	ON_EN_SETFOCUS(IDC_EDIT_USER_ID, OnEnSetfocusEditUserId)
	ON_BN_CLICKED(IDC_BTN_USER_BET_QUERY, OnBnClickedBtnUserBetQuery)
	ON_BN_CLICKED(IDC_BTN_USER_BET_ALL, OnBnClickedBtnUserBetAll)
	ON_BN_CLICKED(IDC_SURE_WINLOSE, OnBnClickedSureWinlose)
	ON_NOTIFY(NM_CLICK, IDC_LIST_USER_BET, OnNMClickListUserBet)
	ON_WM_CTLCOLOR()
	ON_EN_UPDATE(IDC_EDIT_USER_ID, OnEnUpdateEditUserId)
END_MESSAGE_MAP()

// CClientControlItemSinkDlg ��Ϣ�������

void CClientControlItemSinkDlg::OnReSet()
{
	CMD_C_AdminReq adminReq;
	adminReq.cbReqType=RQ_RESET_CONTROL;

	CGameFrameEngine * pGameFrameEngine=CGameFrameEngine::GetInstance();
	if (pGameFrameEngine!=NULL) pGameFrameEngine->SendMessage(IDM_ADMIN_COMMDN,(WPARAM)&adminReq,0);
}

void CClientControlItemSinkDlg::OnRefresh()
{
	CMD_C_AdminReq adminReq;
	adminReq.cbReqType=RQ_PRINT_SYN;	

	CGameFrameEngine * pGameFrameEngine=CGameFrameEngine::GetInstance();
	if (pGameFrameEngine!=NULL) pGameFrameEngine->SendMessage(IDM_ADMIN_COMMDN,(WPARAM)&adminReq,0);
}

void CClientControlItemSinkDlg::RequestUpdateStorage()
{
	CMD_C_FreshStorage adminReq;
	ZeroMemory(&adminReq,sizeof(adminReq));
	adminReq.cbReqType=RQ_REFRESH_STORAGE;

	CGameFrameEngine * pGameFrameEngine=CGameFrameEngine::GetInstance();
	if (pGameFrameEngine!=NULL) pGameFrameEngine->SendMessage(IDM_ADMIN_STORAGE,(WPARAM)&adminReq,0);

}

void CClientControlItemSinkDlg::OnExcute()
{
	//��ȡ������
	UpdateData(TRUE); //�ӿؼ���ȡ���ݵ�����
	
	if(((CButton*)GetDlgItem(IDC_TOTAL_CONTROL))->GetCheck()==1)
	{
		if (m_cbTotalCount < 3 || m_cbTotalCount > 18)
		{
			AfxMessageBox(TEXT("�����ܵ������뷶ΧΪ3-18"));
			return;
		}
	}	
	
	CMD_C_AdminReq adminReq;
	adminReq.cbReqType = RQ_SET_COUNT;
	adminReq.cbSicbo1=(BYTE)((CComboBox*)GetDlgItem(IDC_COMBO_SICBO_ONE))->GetCurSel()+1;
	adminReq.cbSicbo2=(BYTE)((CComboBox*)GetDlgItem(IDC_COMBO_SICBO_TWO))->GetCurSel()+1;
	adminReq.cbSicbo3=(BYTE)((CComboBox*)GetDlgItem(IDC_COMBO_SICBO_THREE))->GetCurSel()+1;
	if (((CButton*)GetDlgItem(IDC_SINGLE_CONTROL))->GetCheck()==1)
	{		
		adminReq.cbTotalCount = 0;
	}
	else
	{
		if (((CButton*)GetDlgItem(IDC_TOTAL_CONTROL))->GetCheck()==1)
		{			
			adminReq.cbTotalCount = m_cbTotalCount;
		}
	}	

	CGameFrameEngine * pGameFrameEngine=CGameFrameEngine::GetInstance();
	if (pGameFrameEngine!=NULL) pGameFrameEngine->SendMessage(IDM_ADMIN_COMMDN,(WPARAM)&adminReq,0);
	
}

//��ʼ��
BOOL CClientControlItemSinkDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CDialog::OnInitDialog();	

	SetDlgItemText(IDC_STATIC_INFO,TEXT("˵�������Ƶ��������Ƶ���������÷������������棡"));

	SetDlgItemText(IDC_STATIC_STATE,TEXT("˵��:ֻ�ܿ���һ�Σ��´�����������"));

	//�����ע
	m_listUserBet.SetExtendedStyle(m_listUserBet.GetExtendedStyle()|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	m_listUserBet.GetHeaderCtrl()->EnableWindow(false);
	INT nColumnCount=0;
	m_listUserBet.InsertColumn(nColumnCount++,TEXT(""),LVCFMT_CENTER,0);
	m_listUserBet.InsertColumn(nColumnCount++,TEXT("���"),LVCFMT_CENTER,40);
	m_listUserBet.InsertColumn(nColumnCount++,TEXT("����ǳ�"),LVCFMT_CENTER,92);
	m_listUserBet.InsertColumn(nColumnCount++,TEXT("���ID"),LVCFMT_CENTER,70);
	m_listUserBet.InsertColumn(nColumnCount++,TEXT("������"),LVCFMT_CENTER,75);
	m_listUserBet.InsertColumn(nColumnCount++,TEXT("����Ӯ"),LVCFMT_CENTER,60);

	((CComboBox*)GetDlgItem(IDC_COMBO_SICBO_ONE))->AddString(TEXT("1"));
	((CComboBox*)GetDlgItem(IDC_COMBO_SICBO_ONE))->AddString(TEXT("2"));
	((CComboBox*)GetDlgItem(IDC_COMBO_SICBO_ONE))->AddString(TEXT("3"));
	((CComboBox*)GetDlgItem(IDC_COMBO_SICBO_ONE))->AddString(TEXT("4"));
	((CComboBox*)GetDlgItem(IDC_COMBO_SICBO_ONE))->AddString(TEXT("5"));
	((CComboBox*)GetDlgItem(IDC_COMBO_SICBO_ONE))->AddString(TEXT("6"));

	((CComboBox*)GetDlgItem(IDC_COMBO_SICBO_TWO))->AddString(TEXT("1"));
	((CComboBox*)GetDlgItem(IDC_COMBO_SICBO_TWO))->AddString(TEXT("2"));
	((CComboBox*)GetDlgItem(IDC_COMBO_SICBO_TWO))->AddString(TEXT("3"));
	((CComboBox*)GetDlgItem(IDC_COMBO_SICBO_TWO))->AddString(TEXT("4"));
	((CComboBox*)GetDlgItem(IDC_COMBO_SICBO_TWO))->AddString(TEXT("5"));
	((CComboBox*)GetDlgItem(IDC_COMBO_SICBO_TWO))->AddString(TEXT("6"));

	((CComboBox*)GetDlgItem(IDC_COMBO_SICBO_THREE))->AddString(TEXT("1"));
	((CComboBox*)GetDlgItem(IDC_COMBO_SICBO_THREE))->AddString(TEXT("2"));
	((CComboBox*)GetDlgItem(IDC_COMBO_SICBO_THREE))->AddString(TEXT("3"));
	((CComboBox*)GetDlgItem(IDC_COMBO_SICBO_THREE))->AddString(TEXT("4"));
	((CComboBox*)GetDlgItem(IDC_COMBO_SICBO_THREE))->AddString(TEXT("5"));
	((CComboBox*)GetDlgItem(IDC_COMBO_SICBO_THREE))->AddString(TEXT("6"));

	((CComboBox*)GetDlgItem(IDC_COMBO_SICBO_ONE))->SetCurSel(0);
	((CComboBox*)GetDlgItem(IDC_COMBO_SICBO_TWO))->SetCurSel(0);
	((CComboBox*)GetDlgItem(IDC_COMBO_SICBO_THREE))->SetCurSel(0);

	((CEdit*)GetDlgItem(IDC_TOTAL_COUNT))->SetLimitText(2);
	((CEdit*)GetDlgItem(IDC_EDIT_USER_ID))->SetLimitText(9);

	((CButton*)GetDlgItem(IDC_SINGLE_CONTROL))->SetCheck(1);
	((CButton*)GetDlgItem(IDC_RADIO_WIN))->SetCheck(1);
	GetDlgItem(IDC_SURE_WINLOSE)->EnableWindow(FALSE);
	OnBnClickedControl();

	m_lStorageStart=0;
	m_lStorageDeduct=0;
	m_lStorageCurrent = 0;
	m_lStorageMax1 = 0;
	m_lStorageMul1 = 0;
	m_lStorageMax2 = 0;
	m_lStorageMul2 = 0;
	m_cbTotalCount = 0;
	m_lQueryGameID = -1;

	m_strCaption=TEXT("���ƹ�����");
	SetWindowText(m_strCaption);
	return TRUE;
}

//���¿��
void CClientControlItemSinkDlg::OnFreshStorage()
{
	INT nID = IDC_EDIT_STORAGE_DEDUCT;
	while(nID <= IDC_EDIT_STORAGE_MUL2)
	{
		TCHAR szData[256] = {0};
		(CButton*)GetDlgItemText(nID, szData, sizeof(szData));

		if(0 == szData[0])
		{
			if( IDOK  == AfxMessageBox(TEXT("������һ���Ϸ����ݣ�")))
			{
				return;
			}
		}

		nID ++;
	}

	//��ȡ������
	UpdateData(TRUE); //�ӿؼ���ȡ���ݵ�����	

	if (m_lStorageDeduct < 0|| m_lStorageDeduct > 1000)
	{
		AfxMessageBox(TEXT("[���˥��ֵ]���뷶ΧΪ0-1000"));
		return;
	}
	if (m_lStorageCurrent < 0|| m_lStorageCurrent > LLONG_MAX)
	{
		CString str;
		str.Format(TEXT("[��ǰ���ֵ]���뷶ΧΪ0-%I64d"),LLONG_MAX);
		AfxMessageBox(str);
		return;
	}
	if (m_lStorageMax1 < 0|| m_lStorageMax1 > LLONG_MAX)
	{
		CString str;
		str.Format(TEXT("[�������1]���뷶ΧΪ0-%I64d"),LLONG_MAX);
		AfxMessageBox(str);
		return;
	}
	if (m_lStorageMul1 < 0|| m_lStorageMul1 > 100)
	{
		AfxMessageBox(TEXT("[���Ӯ�ָ���1]���뷶ΧΪ0-100"));
		return;
	}
	if (m_lStorageMax2 < 0|| m_lStorageMax2 > LLONG_MAX)
	{
		CString str;
		str.Format(TEXT("[�������2]���뷶ΧΪ0-%I64d"),LLONG_MAX);
		AfxMessageBox(str);
		return;
	}
	if (m_lStorageMul2 < 0|| m_lStorageMul2 > 100)
	{
		AfxMessageBox(TEXT("[���Ӯ�ָ���2]���뷶ΧΪ0-100"));
		return;
	}

	CMD_C_FreshStorage adminReq;
	ZeroMemory(&adminReq,sizeof(adminReq));
	adminReq.cbReqType=RQ_SET_STORAGE;

	adminReq.lStorageDeduct = m_lStorageDeduct;
	adminReq.lStorageCurrent = m_lStorageCurrent;
	adminReq.lStorageMax1 = m_lStorageMax1;
	adminReq.lStorageMul1 = m_lStorageMul1;
	adminReq.lStorageMax2 = m_lStorageMax2;
	adminReq.lStorageMul2 = m_lStorageMul2;

	CGameFrameEngine * pGameFrameEngine=CGameFrameEngine::GetInstance();
	if (pGameFrameEngine!=NULL) pGameFrameEngine->SendMessage(IDM_ADMIN_STORAGE,(WPARAM)&adminReq,0);
}

//���¿ؼ�
void __cdecl CClientControlItemSinkDlg::UpdateControl()
{
	RequestUpdateStorage();
	OnRefresh();
}


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

	return true;
}

//������Ӯ
bool __cdecl CClientControlItemSinkDlg::UpdateControlWin(const void * pBuffer)
{
	const CMD_S_ControlWinLose *pControl=(CMD_S_ControlWinLose*)pBuffer;

	CString cs;
	if(pControl->cbWinLose==1)
	{
		cs.Format(TEXT("���ID��%d ����Ӯ��Ч��"),pControl->lQueryGameID);
		SetDlgItemText(IDC_STATIC_STATE,cs);		
	}
	else if(pControl->cbWinLose==2)
	{
		cs.Format(TEXT("���ID��%d ��������Ч��"),pControl->lQueryGameID);
		SetDlgItemText(IDC_STATIC_STATE,cs);
	}
	else
	{
		SetDlgItemText(IDC_STATIC_STATE,TEXT("������Ч��"));
	}
	
	return true;
}

//������ע
void __cdecl CClientControlItemSinkDlg::UpdateUserBet(bool bReSet)
{
	//����б�
	m_listUserBet.DeleteAllItems();	

	if(bReSet)
	{
		m_lQueryGameID = -1;

		SetDlgItemText(IDC_EDIT_USER_ID, TEXT("���������ID"));
		GetDlgItem(IDC_EDIT_STORAGE_START)->SetFocus();		

		GetDlgItem(IDC_BTN_USER_BET_QUERY)->EnableWindow(true);
		GetDlgItem(IDC_BTN_USER_BET_ALL)->EnableWindow(false);
		GetDlgItem(IDC_SURE_WINLOSE)->EnableWindow(FALSE);

		return;
	}

	if(0 == m_UserBetArray.GetCount()) return;

	LONGLONG lAllBetScore[AREA_COUNT+1];
	ZeroMemory(lAllBetScore, sizeof(lAllBetScore));
	GetDlgItem(IDC_SURE_WINLOSE)->EnableWindow(FALSE);

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
	}

	return;
}

void  CClientControlItemSinkDlg::OnAllowControl(bool bEnable)
{
	GetDlgItem(IDC_BT_EXCUTE)->EnableWindow(bEnable);
}


void CClientControlItemSinkDlg::OnBnClickedControl()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(((CButton*)GetDlgItem(IDC_SINGLE_CONTROL))->GetCheck()==1)
	{
		GetDlgItem(IDC_COMBO_SICBO_ONE)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO_SICBO_TWO)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO_SICBO_THREE)->EnableWindow(TRUE);
		GetDlgItem(IDC_TOTAL_COUNT)->EnableWindow(FALSE);
	}
	else
	{
		if(((CButton*)GetDlgItem(IDC_TOTAL_CONTROL))->GetCheck()==1)
		{
			GetDlgItem(IDC_COMBO_SICBO_ONE)->EnableWindow(FALSE);
			GetDlgItem(IDC_COMBO_SICBO_TWO)->EnableWindow(FALSE);
			GetDlgItem(IDC_COMBO_SICBO_THREE)->EnableWindow(FALSE);
			GetDlgItem(IDC_TOTAL_COUNT)->EnableWindow(TRUE);
		}
	}
}

void CClientControlItemSinkDlg::OnEnSetfocusEditUserId()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	SetDlgItemText(IDC_EDIT_USER_ID, TEXT(""));
	//GetDlgItem(IDC_SURE_WINLOSE)->EnableWindow(TRUE);

	return;
}

void CClientControlItemSinkDlg::OnBnClickedBtnUserBetQuery()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//��ȡ�û�ID
	CString strUserID = TEXT("");
	GetDlgItemText(IDC_EDIT_USER_ID,strUserID);

	//ȥ���ո�
	strUserID.TrimLeft();
	strUserID.TrimRight();

	m_lQueryGameID = StrToInt(strUserID);

	GetDlgItem(IDC_BTN_USER_BET_ALL)->EnableWindow(true);

	//���¿ؼ�
	UpdateUserBet(false);
}

void CClientControlItemSinkDlg::OnBnClickedBtnUserBetAll()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//��ʾȫ��
	m_lQueryGameID = -1;

	SetDlgItemText(IDC_EDIT_USER_ID, TEXT("���������ID"));

	GetDlgItem(IDC_BTN_USER_BET_QUERY)->EnableWindow(true);
	GetDlgItem(IDC_BTN_USER_BET_ALL)->EnableWindow(false);

	//���¿ؼ�
	UpdateUserBet(false);

	return;
}

void CClientControlItemSinkDlg::OnBnClickedSureWinlose()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//��ȡ�û�ID
	CString strUserID = TEXT("");
	GetDlgItemText(IDC_EDIT_USER_ID,strUserID);

	//ȥ���ո�
	strUserID.TrimLeft();
	strUserID.TrimRight();

	LONGLONG lQueryGameID = StrToInt(strUserID);

	//���¿ؼ�
	UpdateUserBet(false);

	if (m_UserBetArray.GetCount()==0)
	{
		AfxMessageBox(TEXT("��ע�б�Ϊ�գ�"));
		return;
	}
	
	if (lQueryGameID==0)
	{
		AfxMessageBox(TEXT("����Ҳ����ڣ�"));
		return;
	}
	if (lQueryGameID!=0)
	{
		bool bNull = false;
		for(INT i=0; i<m_UserBetArray.GetCount(); i++)
		{
			//У���û�ID
			if(m_UserBetArray[i].dwUserGameID != lQueryGameID)
			{
				bNull = true;
			}
		}

		if (bNull)
		{
			AfxMessageBox(TEXT("�����û����ע��"));
			return;
		}
	}

	CMD_C_ControlWinLose ControlWinLose;
	ZeroMemory(&ControlWinLose,sizeof(ControlWinLose));
	ControlWinLose.lQueryGameID = lQueryGameID;

	if(((CButton*)GetDlgItem(IDC_RADIO_WIN))->GetCheck()==1)
	{
		ControlWinLose.cbWinLose = 1;
		CGameFrameEngine * pGameFrameEngine=CGameFrameEngine::GetInstance();
		if (pGameFrameEngine!=NULL) pGameFrameEngine->SendMessage(IDM_CONTROL_WIN,(WPARAM)&ControlWinLose,0);
	}
	else
	{
		if (((CButton*)GetDlgItem(IDC_RADIO_LOSE))->GetCheck()==1)
		{
			ControlWinLose.cbWinLose = 2;
			CGameFrameEngine * pGameFrameEngine=CGameFrameEngine::GetInstance();
			if (pGameFrameEngine!=NULL) pGameFrameEngine->SendMessage(IDM_CONTROL_WIN,(WPARAM)&ControlWinLose,0);
		}		
	}
	
}


void CClientControlItemSinkDlg::OnNMClickListUserBet(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;

	int nItem = -1;

	LPNMITEMACTIVATE lpNMItemActivate = (LPNMITEMACTIVATE)pNMHDR;
	if(lpNMItemActivate != NULL)
	{
		nItem = lpNMItemActivate->iItem;
	}

	if (nItem!=-1)
	{
		CString cs;
		cs.Format(TEXT("%d"),m_UserBetArray[nItem].dwUserGameID);	
		SetDlgItemText(IDC_EDIT_USER_ID, cs);
		GetDlgItem(IDC_SURE_WINLOSE)->EnableWindow(TRUE);
	}	
}

HBRUSH CClientControlItemSinkDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = IClientControlDlg::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����
	if( pWnd->GetDlgCtrlID() == IDC_STATIC_STATE )
	{
		pDC->SetTextColor(RGB(255,0,0)); //������ɫ
		//pDC->SetBkColor(RGB(0, 0, 255)); //���屳��ɫ
	}


	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}

void CClientControlItemSinkDlg::OnEnUpdateEditUserId()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ�������������
	// ���͸�֪ͨ��������д IClientControlDlg::OnInitDialog()
	// �������� EM_SETEVENTMASK ��Ϣ���͵��ؼ���
	// ͬʱ�� ENM_UPDATE ��־�������㵽 lParam �����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	//��ȡ�û�ID
	CString strUserID = TEXT("");
	GetDlgItemText(IDC_EDIT_USER_ID,strUserID);

	//ȥ���ո�
	strUserID.TrimLeft();
	strUserID.TrimRight();

	LONGLONG lQueryGameID = StrToInt(strUserID);

	if (lQueryGameID!=0)
	{
		GetDlgItem(IDC_SURE_WINLOSE)->EnableWindow(TRUE);
	}
	else
		GetDlgItem(IDC_SURE_WINLOSE)->EnableWindow(FALSE);
}
