#include "StdAfx.h"
#include "PlazaViewServer.h"
#include "DlgSearchTable.h"
#include "DlgInformation.h"
//////////////////////////////////////////////////////////////////////////
//�ؼ�����

//��ť�ؼ�
const TCHAR * szButtonSearchControlName = TEXT("ButtonSearch");
const TCHAR * szButtonQuickJoinControlName = TEXT("ButtonQuickJoin");
const TCHAR * szButtonCloseControlName1=TEXT("ButtonClose");
const TCHAR * szButtonCancelControlName1=TEXT("ButtonCancel");
//��չ�ؼ�
const TCHAR * szCheckOption1ControlName = TEXT("CheckOption1");
const TCHAR * szCheckOption2ControlName = TEXT("CheckOption2");
const TCHAR * szCheckOption3ControlName = TEXT("CheckOption3");
const TCHAR * szCheckOption4ControlName = TEXT("CheckOption4");
const TCHAR * szCheckOption5ControlName = TEXT("CheckOption5");

//////////////////////////////////////////////////////////////////////////

//���캯��
CDlgSearchTable::CDlgSearchTable() : CFGuiDialog(IDD_DLG_POPUP)
{
	//�ӿڱ���
	m_pITableViewFrame=NULL;
	m_pServerViewItem=NULL;

	//���ñ���
	m_wResultTableID=INVALID_TABLE;

	return;
}

//��������
CDlgSearchTable::~CDlgSearchTable()
{
}

//���ú���
BOOL CDlgSearchTable::OnInitDialog()
{
	__super::OnInitDialog();

	////���ñ���
	//m_wResultTableID=INVALID_TABLE;

	//���ñ���
	SetWindowText(TEXT("������Ϸ��"));

	//���ô���
	CSize SizeWindow(m_PaintManager.GetInitSize());
	SetWindowPos(NULL, 0, 0, SizeWindow.cx, SizeWindow.cy, SWP_NOZORDER|SWP_NOMOVE);	

	return FALSE;
}

//��Ϣ����
BOOL CDlgSearchTable::PreTranslateMessage(MSG * pMsg)
{
	//��������
	if ((pMsg->message==WM_KEYDOWN)&&(pMsg->wParam==VK_ESCAPE))
	{
		return TRUE;
	}

	return __super::PreTranslateMessage(pMsg);
}

//ȷ������
VOID CDlgSearchTable::OnOK()
{
	//��������
	tagSearchTable SearchTable;
	ZeroMemory(&SearchTable,sizeof(SearchTable));

	//��������
	CCheckButtonUI * pCheckOption=NULL;

	//��ѡѡ��
	pCheckOption = static_cast<CCheckButtonUI*>(GetControlByName(szCheckOption1ControlName));
	if(pCheckOption!=NULL) SearchTable.bOneNull=pCheckOption->GetCheck();

	//��ѡѡ��
	pCheckOption = static_cast<CCheckButtonUI*>(GetControlByName(szCheckOption2ControlName));
	if(pCheckOption!=NULL) SearchTable.bTwoNull=pCheckOption->GetCheck();

	//��ѡѡ��
	pCheckOption = static_cast<CCheckButtonUI*>(GetControlByName(szCheckOption3ControlName));
	if(pCheckOption!=NULL) SearchTable.bNotFull=pCheckOption->GetCheck();

	//��ѡѡ��
	pCheckOption = static_cast<CCheckButtonUI*>(GetControlByName(szCheckOption4ControlName));
	if(pCheckOption!=NULL) SearchTable.bAllNull=pCheckOption->GetCheck();

	//��ѡѡ��
	pCheckOption = static_cast<CCheckButtonUI*>(GetControlByName(szCheckOption5ControlName));
	if(pCheckOption!=NULL) SearchTable.bFilterPass=pCheckOption->GetCheck();

	//��ѡѡ��
	CControlUI * pControlAutoJoin = GetControlByName(szButtonQuickJoinControlName);
	if(pControlAutoJoin==NULL) return;

	//��������
	SearchTable.wResultTableID=INVALID_TABLE;
	SearchTable.wResultChairID=INVALID_CHAIR;
	SearchTable.wStartTableID=m_wResultTableID+1;

	//��������
	ASSERT(m_pServerViewItem!=NULL);
	m_pServerViewItem->SearchGameTable(SearchTable);

	//���ý��
	m_wResultTableID=SearchTable.wResultTableID;

	//����ж�
	if (SearchTable.wResultTableID==INVALID_TABLE)
	{
		//��ѡѡ��
		pControlAutoJoin->SetEnabled(false);

		//��ʾ��Ϣ
		CDlgInformation Information(this);
		Information.ShowMessageBox(TEXT("��Ǹ����ʱ�Ҳ���������Ҫ�����Ϸ��������������������Ժ����ԣ�"),MB_ICONINFORMATION,30);

		return;
	}

	//���ý���
	pControlAutoJoin->SetEnabled(m_wResultTableID!=INVALID_TABLE);
	m_pITableViewFrame->VisibleTable(SearchTable.wResultTableID);
	m_pITableViewFrame->FlashGameChair(SearchTable.wResultTableID, SearchTable.wResultChairID);

	return; 
}

//ȡ������
VOID CDlgSearchTable::OnCancel() 
{
	//�رմ���
	DestroyWindow();

	return; 
}

//Ƥ������
LPCTSTR CDlgSearchTable::GetSkinFile()
{
	return TEXT("DlgSearchTable.xml");
}

//��ʼ�ؼ�
VOID CDlgSearchTable::InitControlUI()
{
	__super::InitControlUI();

	//��ѡѡ��
	CCheckButtonUI * pCheckOption = static_cast<CCheckButtonUI*>(GetControlByName(szCheckOption1ControlName));
	if(pCheckOption!=NULL) pCheckOption->SetCheck(true);

	//��ѡѡ��
	pCheckOption = static_cast<CCheckButtonUI*>(GetControlByName(szCheckOption2ControlName));
	if(pCheckOption!=NULL) pCheckOption->SetCheck(true);

	//��ѡѡ��
	pCheckOption = static_cast<CCheckButtonUI*>(GetControlByName(szCheckOption3ControlName));
	if(pCheckOption!=NULL) pCheckOption->SetCheck(true);

	//��ѡѡ��
	pCheckOption = static_cast<CCheckButtonUI*>(GetControlByName(szCheckOption4ControlName));
	if(pCheckOption!=NULL) pCheckOption->SetCheck(false);

	//��ѡѡ��
	pCheckOption = static_cast<CCheckButtonUI*>(GetControlByName(szCheckOption5ControlName));
	if(pCheckOption!=NULL) pCheckOption->SetCheck(true);

	CControlUI * pControlAutoJoin = GetControlByName(szButtonQuickJoinControlName);
	if(pControlAutoJoin!=NULL) pControlAutoJoin->SetEnabled(false);
}

//��Ϣ����
VOID CDlgSearchTable::Notify(TNotifyUI &  msg)
{
	__super::Notify(msg);

	//��ȡ����
	CControlUI * pControlUI = msg.pSender;

	if (lstrcmp(msg.sType, TEXT("click")) == 0)
	{
		 if((lstrcmp(pControlUI->GetName(), szButtonCancelControlName1)==0)
			||(lstrcmp(pControlUI->GetName(), szButtonCloseControlName1)==0))
		{
			return OnCancel();
		}
		else if (lstrcmp(pControlUI->GetName(), szButtonSearchControlName) == 0)
		{
			return OnOK();
		}
		else if (lstrcmp(pControlUI->GetName(), szButtonQuickJoinControlName) == 0)
		{
			return OnBnClickedAutoJoin();
		}
	}
}


//���÷���
VOID CDlgSearchTable::SetServerViewItem(CPlazaViewServer * pServerViewItem)
{
	//���ñ���
	m_pServerViewItem=pServerViewItem;

	return;
}

//��������
VOID CDlgSearchTable::SetTableViewFrame(ITableViewFrame * pITableViewFrame)
{
	//���þ���
	ASSERT(pITableViewFrame!=NULL);
	m_pITableViewFrame=pITableViewFrame;

	return;
}

//���ټ���
VOID CDlgSearchTable::OnBnClickedAutoJoin()
{
	//Ч��״̬
	ASSERT(m_wResultTableID!=INVALID_TABLE);
	if (m_wResultTableID==INVALID_TABLE) return;

	//��������
	WORD wChairID=INVALID_CHAIR;
	m_pITableViewFrame->GetNullChairCount(m_wResultTableID,wChairID);

	//������Ϸ
	if(m_pITableViewFrame->GetChairCount() >= INDEX_ENTER_CHAIR)
		wChairID = INDEX_ENTER_CHAIR;

	//�����ж�
	if (wChairID==INVALID_CHAIR)
	{
		//��ʾ��Ϣ
		CDlgInformation Information(this);
		Information.ShowMessageBox(TEXT("����Ϸ���Ѿ������ˣ��޷��������Ϸ�����������������ٴγ��ԣ�"),MB_ICONINFORMATION,30);

		return;
	}

	//������Ϸ
	ASSERT(m_pServerViewItem!=NULL);
	m_pServerViewItem->OnLButtonHitTable(m_wResultTableID,wChairID);

	return;
}

//////////////////////////////////////////////////////////////////////////
