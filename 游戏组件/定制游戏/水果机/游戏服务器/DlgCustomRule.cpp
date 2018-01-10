// DlgCustomRule.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Resource.h"
#include "DlgCustomRule.h"


// CDlgCustomRule �Ի���

BEGIN_MESSAGE_MAP(CDlgCustomRule, CDialog)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_CUSTOM, OnTcnSelchangeTab)
END_MESSAGE_MAP()

CDlgCustomRule::CDlgCustomRule() : CDialog(IDD_CUSTOM_RULE)
{
}

CDlgCustomRule::~CDlgCustomRule()
{
}

void CDlgCustomRule::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

//��ʼ������
BOOL CDlgCustomRule::OnInitDialog()
{
	__super::OnInitDialog();

	((CTabCtrl *)GetDlgItem(IDC_TAB_CUSTOM))->InsertItem(0,TEXT("ͨ�ù���"));
	((CTabCtrl *)GetDlgItem(IDC_TAB_CUSTOM))->InsertItem(1,TEXT("������"));

	m_DlgCustomGeneral.Create(IDD_CUSTOM_GENERAL,GetDlgItem(IDC_TAB_CUSTOM)); 
	m_DlgCustomAndroid.Create(IDD_CUSTOM_ANDROID,GetDlgItem(IDC_TAB_CUSTOM)); 

	CRect rcTabClient;
	GetDlgItem(IDC_TAB_CUSTOM)->GetClientRect(rcTabClient);
	rcTabClient.top+=20;
	rcTabClient.bottom-=4; 
	rcTabClient.left+=4; 
	rcTabClient.right-=4; 
	m_DlgCustomGeneral.MoveWindow(rcTabClient);
	m_DlgCustomAndroid.MoveWindow(rcTabClient);

	m_DlgCustomGeneral.ShowWindow(TRUE);
	m_DlgCustomAndroid.ShowWindow(FALSE);
	((CTabCtrl *)GetDlgItem(IDC_TAB_CUSTOM))->SetCurSel(0);

	m_wGeneralErrorCount=0;
	m_wAndroidErrorCount=0;
	m_strErrorMessage.Empty();
	return FALSE;
}
//ȷ������
VOID CDlgCustomRule::OnOK() 
{ 
	//Ͷ����Ϣ
	GetParent()->PostMessage(WM_COMMAND,MAKELONG(IDOK,0),0);

	return;
}

//ȡ����Ϣ
VOID CDlgCustomRule::OnCancel() 
{ 
	//Ͷ����Ϣ
	GetParent()->PostMessage(WM_COMMAND,MAKELONG(IDCANCEL,0),0);

	return;
}
//��������
BOOL CDlgCustomRule::UpdateConfigData(BOOL bSaveAndValidate)
{
	if(bSaveAndValidate)	//�ӿؼ���ȡ���ݵ�����
	{
		m_DlgCustomGeneral.UpdateData(TRUE);
		m_DlgCustomAndroid.UpdateData(TRUE);

		CopyMemory(&m_CustomConfig.CustomGeneral, &m_DlgCustomGeneral.m_CustomGeneral, sizeof(tagCustomGeneral));
		CopyMemory(&m_CustomConfig.CustomAndroid, &m_DlgCustomAndroid.m_CustomAndroid, sizeof(tagCustomAndroid));

	}
	else					//��������ֵ���ؼ���ʾ
	{
		CopyMemory(&m_DlgCustomGeneral.m_CustomGeneral, &m_CustomConfig.CustomGeneral, sizeof(tagCustomGeneral));
		CopyMemory(&m_DlgCustomAndroid.m_CustomAndroid, &m_CustomConfig.CustomAndroid, sizeof(tagCustomAndroid));

		m_DlgCustomGeneral.UpdateData(FALSE);
		m_DlgCustomAndroid.UpdateData(FALSE);
	}

	return TRUE;
}

//��������
bool CDlgCustomRule::SetCustomRule(LPBYTE pcbCustomRule, WORD wCustomSize)
{
	//���ñ���
	m_wCustomSize=wCustomSize;
	m_pcbCustomRule=pcbCustomRule;

	//���ñ���
	ASSERT(m_pcbCustomRule);
	if( !m_pcbCustomRule ) return false;
	tagCustomConfig *pCustomConfig = (tagCustomConfig *)m_pcbCustomRule;
	//if( pCustomConfig->lApplyBankerCondition != 0 )
	memcpy(&m_CustomConfig, pCustomConfig, sizeof(tagCustomConfig));

	//���½���
	if( m_hWnd )
		UpdateConfigData(FALSE);  //��������ֵ���ؼ���ʾ

	return true;
}

//��������
bool CDlgCustomRule::SaveCustomRule(LPBYTE pcbCustomRule, WORD wCustomSize)
{
	//���½���
	if( m_hWnd )
	{
		//����ǰ�ȼ�¼
		tagCustomConfig tempCustomConfig;
		CopyMemory(&tempCustomConfig,&m_CustomConfig,sizeof(m_CustomConfig));
		UpdateConfigData(TRUE); //�ӿؼ���ȡ���ݵ�����
		//������ݴ���,��ԭ������ǰ������
		if(CheckData() == false)
		{
			CopyMemory(&m_CustomConfig,&tempCustomConfig,sizeof(m_CustomConfig));
		}
	}

	//���ñ���
	m_wCustomSize=wCustomSize;
	m_pcbCustomRule=pcbCustomRule;

	//���ñ���
	ASSERT(m_pcbCustomRule);
	if( !m_pcbCustomRule ) return true;
	tagCustomConfig *pCustomConfig = (tagCustomConfig *)m_pcbCustomRule;
	memcpy(pCustomConfig, &m_CustomConfig, sizeof(tagCustomConfig));

	return true;
}

//Ĭ������
bool CDlgCustomRule::DefaultCustomRule(LPBYTE pcbCustomRule, WORD wCustomSize)
{
	//���ñ���
	m_wCustomSize=wCustomSize;
	m_pcbCustomRule=pcbCustomRule;

	//���ñ���
	ASSERT(m_pcbCustomRule);
	if( !m_pcbCustomRule ) return true;
	tagCustomConfig *pCustomConfig = (tagCustomConfig *)m_pcbCustomRule;

	m_CustomConfig.DefaultCustomRule();
	memcpy(pCustomConfig, &m_CustomConfig, sizeof(tagCustomConfig));

	//���½���
	if( m_hWnd )
		UpdateConfigData(FALSE); // ��������ֵ���ؼ���ʾ

	return true;
}
//���������������ʱ ɾ������������ѡ��
void CDlgCustomRule::DeleteAndroid()
{
	((CTabCtrl *)GetDlgItem(IDC_TAB_CUSTOM))->DeleteItem(1);	
}
void CDlgCustomRule::OnTcnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult)
{   
	INT CurSel =((CTabCtrl *)GetDlgItem(IDC_TAB_CUSTOM))->GetCurSel();

	switch(CurSel)
	{
	case 0:
		m_DlgCustomGeneral.ShowWindow(true);
		m_DlgCustomAndroid.ShowWindow(false);
		break;
	case 1:
		m_DlgCustomGeneral.ShowWindow(false);
		m_DlgCustomAndroid.ShowWindow(true);
		break;
	}
	*pResult = 0;

	return;
}

//����У��(�ؼ�������)
bool CDlgCustomRule::CheckData()
{
	m_wGeneralErrorCount=0;
	m_wAndroidErrorCount=0;
	m_strErrorMessage.Empty();
//m_DlgCustomGeneralͨ�ù��ܴ���У��//////////////////////////////////////////////////////////////////////////////////////////////////////
	m_strErrorMessage+=TEXT("ͨ�ù����������ó���:");
	
	//��ׯ����
	FormatDataCheck(m_CustomConfig.CustomGeneral.lApplyBankerCondition,	1000,	LLONG_MAX,	TEXT("��ׯ����"),	true);
	FormatDataCheck(m_CustomConfig.CustomGeneral.lBankerTime,			1,		100,		TEXT("��ׯ����"),	true);
	FormatDataCheck(m_CustomConfig.CustomGeneral.lBankerTimeAdd,		1,		100,		TEXT("������ׯ"),	true);
	FormatDataCheck(m_CustomConfig.CustomGeneral.lBankerScoreMAX,		1000,	LLONG_MAX,	TEXT("��������"),	true);
	FormatDataCheck(m_CustomConfig.CustomGeneral.lBankerTimeExtra,		1,		100,		TEXT("������ׯ"),	true);
	//ʱ�����
	FormatDataCheck(m_CustomConfig.CustomGeneral.cbFreeTime,			5,		99,			TEXT("����ʱ��"),	true);
	FormatDataCheck(m_CustomConfig.CustomGeneral.cbBetTime,				10,		99,			TEXT("��עʱ��"),	true);
	FormatDataCheck(m_CustomConfig.CustomGeneral.cbEndTime,				20,		99,			TEXT("����ʱ��"),	true);
	FormatDataCheck(m_CustomConfig.CustomGeneral.cbGuessTime,			15,		99,			TEXT("�´�Сʱ��"),	true);
	//��ע����
	FormatDataCheck(m_CustomConfig.CustomGeneral.lAreaLimitScore,		10000,	LLONG_MAX,	TEXT("��������"),	true);
	FormatDataCheck(m_CustomConfig.CustomGeneral.lUserLimitScore,		10000,	LLONG_MAX,	TEXT("��������"),	true);
	FormatDataCheck(m_CustomConfig.CustomGeneral.lEndGameMul,			1,		10000000,	TEXT("�һ�����"),true);

	//������
	FormatDataCheck(m_CustomConfig.CustomGeneral.StorageStart,			0,		LLONG_MAX,	TEXT("�����ʼֵ"),	true);
	FormatDataCheck(m_CustomConfig.CustomGeneral.StorageDeduct,			0,		100,		TEXT("���˥��ֵ"),	true);
	FormatDataCheck(m_CustomConfig.CustomGeneral.StorageMax1,			10,		LLONG_MAX,	TEXT("�������ֵ1"),true);
	FormatDataCheck(m_CustomConfig.CustomGeneral.StorageMul1,			0,		100,		TEXT("Ӯ�ָ���1"),	true);
	FormatDataCheck(m_CustomConfig.CustomGeneral.StorageMax2,			20,		LLONG_MAX,	TEXT("�������ֵ2"),true);
	FormatDataCheck(m_CustomConfig.CustomGeneral.StorageMul2,			0,		100,		TEXT("��������"),	true);

	if(m_CustomConfig.CustomGeneral.StorageMax2 < m_CustomConfig.CustomGeneral.StorageMax1)
	{
		m_strErrorMessage+=TEXT("\n  �������ֵ2 �� �������ֵ1 С������������");
		m_wGeneralErrorCount++;
	}

	if(m_wGeneralErrorCount == 0)  m_strErrorMessage.Empty();

//�������������ü��//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	CString strBuf = m_strErrorMessage;
	if(m_strErrorMessage.IsEmpty() == false)
		m_strErrorMessage+=TEXT("\n�������������ó���:");
	else
		m_strErrorMessage+=TEXT("�������������ó���:");

	FormatDataCheck(m_CustomConfig.CustomAndroid.lRobotBankerCountMin,		1,		99,			TEXT("��ׯ������С"),		false);
	FormatDataCheck(m_CustomConfig.CustomAndroid.lRobotBankerCountMax,		1,		99,			TEXT("��ׯ�������"),		false);
	FormatDataCheck(m_CustomConfig.CustomAndroid.lRobotListMinCount,		1,		10,			TEXT("�б��������"),		false);
	FormatDataCheck(m_CustomConfig.CustomAndroid.lRobotListMaxCount,		1,		10,			TEXT("�б��������"),		false);
	FormatDataCheck(m_CustomConfig.CustomAndroid.lRobotApplyBanker,			1,		10,			TEXT("����������"),		false);
	FormatDataCheck(m_CustomConfig.CustomAndroid.lRobotWaitBanker,			1,		10,			TEXT("�����������"),		false);

	FormatDataCheck(m_CustomConfig.CustomAndroid.lRobotMinBetTime,			1,		50,			TEXT("�����������"),		false);
	FormatDataCheck(m_CustomConfig.CustomAndroid.lRobotMaxBetTime,			1,		50,			TEXT("����������"),		false);
	if(m_CustomConfig.CustomAndroid.lRobotMaxBetTime<m_CustomConfig.CustomAndroid.lRobotMinBetTime)
	{
		m_strErrorMessage+=TEXT("\n  ���������� ���õ�ֵ�� ����������� С������������");
		m_wAndroidErrorCount++;
	}
	FormatDataCheck(m_CustomConfig.CustomAndroid.lRobotMinJetton,			1,		100000000,	TEXT("��������С"),		false);
	FormatDataCheck(m_CustomConfig.CustomAndroid.lRobotMaxJetton,			1,		100000000,	TEXT("���������"),		false);
	if(m_CustomConfig.CustomAndroid.lRobotMaxJetton<m_CustomConfig.CustomAndroid.lRobotMinJetton)
	{
		m_strErrorMessage+=TEXT("\n  ��������� ���õ�ֵ�� ��������С С������������");
		m_wAndroidErrorCount++;
	}
	FormatDataCheck(m_CustomConfig.CustomAndroid.lRobotBetMinCount,			1,		255,		TEXT("��ע��������"),		false);
	FormatDataCheck(m_CustomConfig.CustomAndroid.lRobotBetMaxCount,			1,		255,		TEXT("��ע�������"),		false);
	if(m_CustomConfig.CustomAndroid.lRobotBetMaxCount<m_CustomConfig.CustomAndroid.lRobotBetMinCount)
	{
		m_strErrorMessage+=TEXT("\n  ��ע������� ���õ�ֵ�� ��ע�������� С������������");
		m_wAndroidErrorCount++;
	}
	FormatDataCheck(m_CustomConfig.CustomAndroid.lRobotAreaLimit,			100000,	LLONG_MAX,	TEXT("��ע��������"),		false);

	FormatDataCheck(m_CustomConfig.CustomAndroid.lRobotScoreMin,			1000,	LLONG_MAX,	TEXT("�����˷�����Сֵ"),	false);
	FormatDataCheck(m_CustomConfig.CustomAndroid.lRobotScoreMax,			1000,	LLONG_MAX,	TEXT("�����˷������ֵ"),	false);
	if(m_CustomConfig.CustomAndroid.lRobotScoreMax<m_CustomConfig.CustomAndroid.lRobotScoreMin)
	{
		m_strErrorMessage+=TEXT("\n  �����˷������ֵ ���õ�ֵ�� �����˷�����Сֵ С������������");
		m_wAndroidErrorCount++;
	}
	FormatDataCheck(m_CustomConfig.CustomAndroid.lRobotBankGetMin,			1000,	LLONG_MAX,	TEXT("������ȡ����Сֵ"),	false);
	FormatDataCheck(m_CustomConfig.CustomAndroid.lRobotBankGetMax,			1000,	LLONG_MAX,	TEXT("������ȡ�����ֵ"),	false);
	if(m_CustomConfig.CustomAndroid.lRobotBankGetMax<m_CustomConfig.CustomAndroid.lRobotBankGetMin)
	{
		m_strErrorMessage+=TEXT("\n  ������ȡ�����ֵ ���õ�ֵ�� ������ȡ����Сֵ С������������");
		m_wAndroidErrorCount++;
	}
	FormatDataCheck(m_CustomConfig.CustomAndroid.lRobotBankGetBankerMin,	1000,	LLONG_MAX,	TEXT("ׯ��ȡ����Сֵ"),		false);
	FormatDataCheck(m_CustomConfig.CustomAndroid.lRobotBankGetBankerMax,	1000,	LLONG_MAX,	TEXT("ׯ��ȡ�����ֵ"),		false);
	if(m_CustomConfig.CustomAndroid.lRobotBankGetBankerMax<m_CustomConfig.CustomAndroid.lRobotBankGetBankerMin)
	{
		m_strErrorMessage+=TEXT("\n  ׯ��ȡ�����ֵ ���õ�ֵ�� ׯ��ȡ����Сֵ С������������");
		m_wAndroidErrorCount++;
	}
	FormatDataCheck(m_CustomConfig.CustomAndroid.lRobotBankStoMul,			1,		99,			TEXT("���ٷֱ�"),			false);


	if(m_wAndroidErrorCount == 0) m_strErrorMessage = strBuf;
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	if(m_wAndroidErrorCount+m_wGeneralErrorCount>8)
		m_strErrorMessage.Format(TEXT("���ǹ���İɣ����õ����ݾ�Ȼ�г���8���Ǵ���ġ��������ϸ�������ã��ⲻ���棬��ģ�"));
	if(m_strErrorMessage.IsEmpty() == false)
	{
		MessageBox(m_strErrorMessage);
		return false;
	}
		
	return true;		
}

//ͨ��У��
void CDlgCustomRule::FormatDataCheck(LONGLONG lCheckData,LONGLONG lMindata,LONGLONG lMaxdata,LPCTSTR strDataName,bool IsGeneralData)
{
	if(lCheckData<lMindata || lCheckData>lMaxdata)
	{
		CString strBuf;
		strBuf.Format(TEXT("\n  %s�������ô���������%I64d-%I64d��Χ�ڵ�����"),strDataName,lMindata,lMaxdata);
		m_strErrorMessage+=strBuf;

		if(IsGeneralData)
			m_wGeneralErrorCount++;
		else
			m_wAndroidErrorCount++;
	}
}

//����У��
bool CDlgCustomRule::CheckCustomRule()
{
	if(m_CustomConfig.CustomGeneral.cbBetTime == 0 ||
	   m_CustomConfig.CustomGeneral.cbEndTime == 0 ||
	   m_CustomConfig.CustomGeneral.cbFreeTime ==0 ||
	   m_CustomConfig.CustomAndroid.lRobotAreaLimit==0)
	{
		return false;
	}

	return true;
}
//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CDlgCustomGeneral, CDialog)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CDlgCustomGeneral::CDlgCustomGeneral() : CDialog(IDD_CUSTOM_GENERAL)
{
}

//��������
CDlgCustomGeneral::~CDlgCustomGeneral()
{
}

//�ؼ���
VOID CDlgCustomGeneral::DoDataExchange(CDataExchange* pDX)
{
	__super::DoDataExchange(pDX);

	DDX_Text(pDX,IDC_EDIT_GENERAL_1,m_CustomGeneral.lApplyBankerCondition);
	DDX_Text(pDX,IDC_EDIT_GENERAL_2,m_CustomGeneral.lBankerTime);
	DDX_Text(pDX,IDC_EDIT_GENERAL_3,m_CustomGeneral.lBankerTimeAdd);
	DDX_Text(pDX,IDC_EDIT_GENERAL_4,m_CustomGeneral.lBankerScoreMAX);
	DDX_Text(pDX,IDC_EDIT_GENERAL_5,m_CustomGeneral.lBankerTimeExtra);
	DDX_Text(pDX,IDC_EDIT_GENERAL_6,m_CustomGeneral.cbFreeTime);
	DDX_Text(pDX,IDC_EDIT_GENERAL_7,m_CustomGeneral.cbBetTime);
	DDX_Text(pDX,IDC_EDIT_GENERAL_8,m_CustomGeneral.cbEndTime);
	DDX_Text(pDX,IDC_EDIT_GENERAL_9,m_CustomGeneral.cbGuessTime);
	
	DDX_Text(pDX,IDC_EDIT_GENERAL_10,m_CustomGeneral.lAreaLimitScore);
	DDX_Text(pDX,IDC_EDIT_GENERAL_11,m_CustomGeneral.lUserLimitScore);
	DDX_Text(pDX,IDC_EDIT_GENERAL_12,m_CustomGeneral.lEndGameMul);

	DDX_Text(pDX,IDC_EDIT_GENERAL_13,m_CustomGeneral.szMessageItem1,64);
	DDX_Text(pDX,IDC_EDIT_GENERAL_14,m_CustomGeneral.szMessageItem2,64);
	DDX_Text(pDX,IDC_EDIT_GENERAL_15,m_CustomGeneral.szMessageItem3,64);

	DDX_Text(pDX,IDC_EDIT_GENERAL_16,m_CustomGeneral.StorageStart);
	DDX_Text(pDX,IDC_EDIT_GENERAL_17,m_CustomGeneral.StorageDeduct);
	DDX_Text(pDX,IDC_EDIT_GENERAL_18,m_CustomGeneral.StorageMax1);
	DDX_Text(pDX,IDC_EDIT_GENERAL_19,m_CustomGeneral.StorageMul1);
	DDX_Text(pDX,IDC_EDIT_GENERAL_20,m_CustomGeneral.StorageMax2);
	DDX_Text(pDX,IDC_EDIT_GENERAL_21,m_CustomGeneral.StorageMul2);

	DDX_Check(pDX,IDC_CHECK_GENERAL_1,m_CustomGeneral.nEnableSysBanker);

	DDV_MaxChars(pDX,m_CustomGeneral.szMessageItem1,64);
	DDV_MaxChars(pDX,m_CustomGeneral.szMessageItem2,64);
	DDV_MaxChars(pDX,m_CustomGeneral.szMessageItem3,64);


}

//��ʼ������
BOOL CDlgCustomGeneral::OnInitDialog()
{
	__super::OnInitDialog();

	return TRUE;
}


//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CDlgCustomAndroid, CDialog)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CDlgCustomAndroid::CDlgCustomAndroid() : CDialog(IDD_CUSTOM_ANDROID)
{
}

//��������
CDlgCustomAndroid::~CDlgCustomAndroid()
{
}

//�ؼ���
VOID CDlgCustomAndroid::DoDataExchange(CDataExchange* pDX)
{
	__super::DoDataExchange(pDX);

	DDX_Text(pDX,IDC_EDIT_ANDROID_1,m_CustomAndroid.lRobotBankerCountMin);
	DDX_Text(pDX,IDC_EDIT_ANDROID_2,m_CustomAndroid.lRobotBankerCountMax);
	DDX_Text(pDX,IDC_EDIT_ANDROID_3,m_CustomAndroid.lRobotListMinCount);
	DDX_Text(pDX,IDC_EDIT_ANDROID_4,m_CustomAndroid.lRobotListMaxCount);
	DDX_Text(pDX,IDC_EDIT_ANDROID_5,m_CustomAndroid.lRobotApplyBanker);
	DDX_Text(pDX,IDC_EDIT_ANDROID_6,m_CustomAndroid.lRobotWaitBanker);

	DDX_Text(pDX,IDC_EDIT_ANDROID_7,m_CustomAndroid.lRobotMinBetTime);
	DDX_Text(pDX,IDC_EDIT_ANDROID_8,m_CustomAndroid.lRobotMaxBetTime);
	DDX_Text(pDX,IDC_EDIT_ANDROID_9,m_CustomAndroid.lRobotMinJetton);
	DDX_Text(pDX,IDC_EDIT_ANDROID_10,m_CustomAndroid.lRobotMaxJetton);
	DDX_Text(pDX,IDC_EDIT_ANDROID_11,m_CustomAndroid.lRobotBetMinCount);
	DDX_Text(pDX,IDC_EDIT_ANDROID_12,m_CustomAndroid.lRobotBetMaxCount);
	DDX_Text(pDX,IDC_EDIT_ANDROID_13,m_CustomAndroid.lRobotAreaLimit);

	DDX_Text(pDX,IDC_EDIT_ANDROID_14,m_CustomAndroid.lRobotScoreMin);
	DDX_Text(pDX,IDC_EDIT_ANDROID_15,m_CustomAndroid.lRobotScoreMax);
	DDX_Text(pDX,IDC_EDIT_ANDROID_16,m_CustomAndroid.lRobotBankGetMin);
	DDX_Text(pDX,IDC_EDIT_ANDROID_17,m_CustomAndroid.lRobotBankGetMax);
	DDX_Text(pDX,IDC_EDIT_ANDROID_18,m_CustomAndroid.lRobotBankGetBankerMin);
	DDX_Text(pDX,IDC_EDIT_ANDROID_19,m_CustomAndroid.lRobotBankGetBankerMax);
	DDX_Text(pDX,IDC_EDIT_ANDROID_20,m_CustomAndroid.lRobotBankStoMul);
	DDX_Check(pDX,IDC_CHECK_ANDROID_1,m_CustomAndroid.nEnableRobotBanker);

}

//��ʼ������
BOOL CDlgCustomAndroid::OnInitDialog()
{
	__super::OnInitDialog();

	return TRUE;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////
