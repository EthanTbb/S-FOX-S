#include "Stdafx.h"
#include "Resource.h"
#include "DlgCustomRule.h"

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CDlgCustomRule, CDialog)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CDlgCustomRule::CDlgCustomRule() : CDialog(IDD_CUSTOM_RULE)
{
	//���ñ���
	m_CustomRule.cbTimeStartGame=30;
	m_CustomRule.cbTimeCallBanker=20;
	m_CustomRule.cbTimeSetChip=20;
	m_CustomRule.cbTimeRangeCard=60;
	m_CustomRule.cbTimeShowCard=10;

	return;
}

//��������
CDlgCustomRule::~CDlgCustomRule()
{
}

//���ú���
BOOL CDlgCustomRule::OnInitDialog()
{
	__super::OnInitDialog();

	//���ÿؼ�
	((CEdit *)GetDlgItem(IDC_TIME_START_GAME))->LimitText(2);
	((CEdit *)GetDlgItem(IDC_TIME_CALL_BANKER))->LimitText(2);
	((CEdit *)GetDlgItem(IDC_TIME_SET_CHIP))->LimitText(2);
	((CEdit *)GetDlgItem(IDC_TIME_RANGE_CARD))->LimitText(2);
	((CEdit *)GetDlgItem(IDC_TIME_SHOW_CARD))->LimitText(2);

	//���²���
	FillDataToControl();

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

//���¿ؼ�
bool CDlgCustomRule::FillDataToControl()
{
	//��������
	
	SetDlgItemInt(IDC_TIME_START_GAME,m_CustomRule.cbTimeStartGame);
	SetDlgItemInt(IDC_TIME_CALL_BANKER,m_CustomRule.cbTimeCallBanker);
	SetDlgItemInt(IDC_TIME_SET_CHIP,m_CustomRule.cbTimeSetChip);
	SetDlgItemInt(IDC_TIME_RANGE_CARD,m_CustomRule.cbTimeRangeCard);
	SetDlgItemInt(IDC_TIME_SHOW_CARD,m_CustomRule.cbTimeShowCard);

	return true;
}

//��������
bool CDlgCustomRule::FillControlToData()
{
	//��������
	m_CustomRule.cbTimeStartGame=(BYTE)GetDlgItemInt(IDC_TIME_START_GAME);
	m_CustomRule.cbTimeCallBanker=(BYTE)GetDlgItemInt(IDC_TIME_CALL_BANKER);
	m_CustomRule.cbTimeSetChip=(BYTE)GetDlgItemInt(IDC_TIME_SET_CHIP);
	m_CustomRule.cbTimeRangeCard=(BYTE)GetDlgItemInt(IDC_TIME_RANGE_CARD);
	m_CustomRule.cbTimeShowCard=(BYTE)GetDlgItemInt(IDC_TIME_SHOW_CARD);

	//��ʼʱ��
	if ((m_CustomRule.cbTimeStartGame<5)||(m_CustomRule.cbTimeStartGame>60))
	{
		AfxMessageBox(TEXT("��ʼʱ�����÷�Χ�������������ã�"),MB_ICONSTOP);
		return false;
	}

	//��ׯʱ��
	if ((m_CustomRule.cbTimeCallBanker<5)||(m_CustomRule.cbTimeCallBanker>60))
	{
		AfxMessageBox(TEXT("��ׯʱ�����÷�Χ�������������ã�"),MB_ICONSTOP);
		return false;
	}

	//��עʱ��
	if ((m_CustomRule.cbTimeSetChip<5)||(m_CustomRule.cbTimeSetChip>60))
	{
		AfxMessageBox(TEXT("��עʱ�����÷�Χ�������������ã�"),MB_ICONSTOP);
		return false;
	}

	//����ʱ��
	if ((m_CustomRule.cbTimeRangeCard<20)||(m_CustomRule.cbTimeRangeCard>99))
	{
		AfxMessageBox(TEXT("����ʱ�����÷�Χ�������������ã�"),MB_ICONSTOP);
		return false;
	}

	//����ʱ��
	if ((m_CustomRule.cbTimeShowCard<10)||(m_CustomRule.cbTimeShowCard>20))
	{
		AfxMessageBox(TEXT("����ʱ�����÷�Χ�������������ã�"),MB_ICONSTOP);
		return false;
	}

	return true;
}

//��ȡ����
bool CDlgCustomRule::GetCustomRule(tagCustomRule & CustomRule)
{
	//��ȡ����
	if (FillControlToData()==true)
	{
		CustomRule=m_CustomRule;
		return true;
	}

	return false;
}

//��������
bool CDlgCustomRule::SetCustomRule(tagCustomRule & CustomRule)
{
	//���ñ���
	m_CustomRule=CustomRule;

	//���²���
	if (m_hWnd!=NULL) FillDataToControl();

	return true;
}

//////////////////////////////////////////////////////////////////////////////////
