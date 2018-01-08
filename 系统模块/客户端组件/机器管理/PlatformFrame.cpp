#include "Stdafx.h"
#include "AndroidManager.h"
#include "PlatformFrame.h"
#include ".\platformframe.h"

//////////////////////////////////////////////////////////////////////////////////

//���ư�ť
#define IDC_MIN						100									//��С��ť
#define IDC_MAX						101									//���ť
#define IDC_CLOSE					102									//�رհ�ť

//�ؼ���ʶ
#define IDC_SYSTEM_TRAY_ICON		303									//����ͼ��

//��������
#define MIN_INOUTINTERVAL			30*60								//��С���
#define MAX_INOUTINTERVAL			12*3600								//�����

//////////////////////////////////////////////////////////////////////////////////
//��̬����

CPlatformFrame * CPlatformFrame::m_pPlatformFrame=NULL;					//���ָ��

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CPlatformFrame,CSkinDialog)

	//ϵͳ��Ϣ
	ON_WM_SIZE()
	ON_WM_CLOSE()
	ON_WM_TIMER()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_GETMINMAXINFO()
	ON_WM_SETTINGCHANGE()

	//�Զ���Ϣ
	ON_MESSAGE(WM_PLATFORM_EVENT, OnMessagePlatformEvent)

	ON_CBN_SELCHANGE(IDC_COMBO_GAME, OnCbnComboGameSelchange)
	ON_BN_CLICKED(IDC_BT_ADD, OnBnClickedBtAdd)
	ON_BN_CLICKED(IDC_BT_MODIFY, OnBnClickedBtModify)
	ON_BN_CLICKED(IDC_BT_DELETE, OnBnClickedBtDelete)
	ON_BN_CLICKED(IDC_BT_QUERY, OnBnClickedBtQuery)	

	ON_NOTIFY(LVN_DELETEITEM, IDC_LIST_PARAMETER, OnLvnListParameterDeleteitem)
	ON_NOTIFY(NM_CLICK, IDC_LIST_PARAMETER, OnHdnListParameterItemclick)
	ON_NOTIFY(LVN_KEYDOWN, IDC_LIST_PARAMETER, OnLvnKeydownListParameter)

	ON_WM_DESTROY()	
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CPlatformFrame::CPlatformFrame() : CSkinDialog(IDD_PLATFORM_FRAME)
{
	//���ñ���
	m_bLogonSuccess=false;
	m_nSelectItem=-1;

	//�������
	m_MissionManager.InsertMissionItem(&m_MissionList);
	m_MissionManager.InsertMissionItem(&m_MissionLogon);
	m_MissionManager.InsertMissionItem(&m_MissionAndroid);

	//ƽ̨����
	ASSERT(m_pPlatformFrame==NULL);
	if (m_pPlatformFrame==NULL) m_pPlatformFrame=this;

	//������ˢ
	m_brBackGround.CreateSolidBrush(RGB(215,223,228));

	return;
}

//��������
CPlatformFrame::~CPlatformFrame()
{
	//�ͷŶ���
	m_brBackGround.DeleteObject();	

	//ƽ̨����
	ASSERT(m_pPlatformFrame==this);
	if (m_pPlatformFrame==this) m_pPlatformFrame=NULL;

	return;
}

//��Ϣ����
BOOL CPlatformFrame::PreTranslateMessage(MSG * pMsg)
{
	return __super::PreTranslateMessage(pMsg);
}

//�����
BOOL CPlatformFrame::OnCommand(WPARAM wParam, LPARAM lParam)
{
	//��������
	UINT nCommandID=LOWORD(wParam);

	//���ܰ�ť
	switch (nCommandID)
	{
	case IDC_MIN:				//��С��ť
		{
			ShowWindow(SW_MINIMIZE);
			return TRUE;
		}
	case IDC_MAX:				//���ť
		{
			//���½���
			RedrawWindow(NULL,NULL,RDW_ERASE|RDW_INVALIDATE|RDW_ERASENOW|RDW_UPDATENOW);

			return TRUE;
		}
	case IDC_CLOSE:				//�رհ�ť
		{
			PostMessage(WM_CLOSE,0,0);
			return TRUE;
		}
	}

	//�˵�����
	switch (nCommandID)
	{
	case IDM_USER_LOGON:		//�û���¼
		{
			m_MissionLogon.ShowLogon();

			return TRUE;
		}
	}
	
	return __super::OnCommand(wParam,lParam);	
}

//��������
VOID CPlatformFrame::DoDataExchange(CDataExchange* pDX)
{
	//�󶨿ؼ�
	DDX_Control(pDX, IDC_COMBO_GAME, m_cbGameList);
	DDX_Control(pDX, IDC_COMBO_SERVER, m_cbServerList);
	DDX_Control(pDX, IDC_COMBO_ENTER_HOUR, m_cbEnterHour);
	DDX_Control(pDX, IDC_COMBO_ENTER_MINUTE, m_cbEnterMinute);
	DDX_Control(pDX, IDC_COMBO_ENTER_SECOND, m_cbEnterSecond);
	DDX_Control(pDX, IDC_COMBO_LEAVE_HOUR, m_cbLeaveHour);
	DDX_Control(pDX, IDC_COMBO_LEAVE_MINUTE, m_cbLeaveMinute);
	DDX_Control(pDX, IDC_COMBO_LEAVE_SECOND, m_cbLeaveSecond);

	//�󶨿ؼ�
	DDX_Control(pDX, IDC_EDIT_ANDROID_COUNT, m_edtAndroidCount);
	DDX_Control(pDX, IDC_EDIT_TAKESCORE_MIN, m_edtTableScoreMin);
	DDX_Control(pDX, IDC_EDIT_TAKESCORE_MAX, m_edtTableScoreMax);
	DDX_Control(pDX, IDC_EDIT_ENTERINTERVAL_MIN, m_edtEnterIntervalMin);
	DDX_Control(pDX, IDC_EDIT_ENTERINTERVAL_MAX, m_edtEnterIntervalMax);
	DDX_Control(pDX, IDC_EDIT_LEAVEINTERVAL_MIN, m_edtLeaveIntervalMin);
	DDX_Control(pDX, IDC_EDIT_LEAVEINTERVAL_MAX, m_edtLeaveIntervalMax);
	DDX_Control(pDX, IDC_EDIT_SWITCHTABLE_MIN, m_edtSwitchTableMin);
	DDX_Control(pDX, IDC_EDIT_SWITCHTABLE_MAX, m_edtSwitchTableMax);
	DDX_Control(pDX, IDC_EDIT_COUNT_MEMBER0, m_edtCountMember0);
	DDX_Control(pDX, IDC_EDIT_COUNT_MEMBER1, m_edtCountMember1);
	DDX_Control(pDX, IDC_EDIT_COUNT_MEMBER2, m_edtCountMember2);
	DDX_Control(pDX, IDC_EDIT_COUNT_MEMBER3, m_edtCountMember3);
	DDX_Control(pDX, IDC_EDIT_COUNT_MEMBER4, m_edtCountMember4);
	DDX_Control(pDX, IDC_EDIT_COUNT_MEMBER5, m_edtCountMember5);

	//�󶨿ؼ�
	DDX_Control(pDX, IDC_BT_QUERY, m_btQuery);
	DDX_Control(pDX, IDC_BT_ADD, m_btAddItem);
	DDX_Control(pDX, IDC_BT_MODIFY, m_btModifyItem);
	DDX_Control(pDX, IDC_BT_DELETE, m_btDeleteItem);

	//�󶨿ؼ�
	DDX_Control(pDX, IDC_LIST_PARAMETER, m_ParemeterList);
    

	__super::DoDataExchange(pDX);
}

//���֪ͨ
VOID CPlatformFrame::OnGameItemFinish()
{
}

//���֪ͨ
VOID CPlatformFrame::OnGameKindFinish(WORD wKindID)
{
}

//����֪ͨ
VOID CPlatformFrame::OnGameItemUpdateFinish()
{
}

//����֪ͨ
VOID CPlatformFrame::OnGameItemInsert(CGameListItem * pGameListItem)
{
	//Ч�����
	ASSERT(pGameListItem!=NULL);
	if (pGameListItem==NULL) return;

	//���봦��
	switch (pGameListItem->GetItemGenre())
	{
	case ItemGenre_Type:	//��Ϸ����
		{
			break;
		}
	case ItemGenre_Kind:	//��Ϸ����
		{
			//��������
			CGameKindItem * pGameKindItem=(CGameKindItem *)pGameListItem;
			CGameListItem * pParentListItem=pGameListItem->GetParentListItem();
			
			//��������
			m_cbGameList.SetItemData(m_cbGameList.AddString(pGameKindItem->m_GameKind.szKindName),pGameKindItem->m_GameKind.wKindID);

			break;
		}
	case ItemGenre_Node:	//��Ϸ�ڵ�
		{
			break;
		}
	case ItemGenre_Page:	//��������
		{
			break;
		}
	case ItemGenre_Server:	//��Ϸ����
		{
			////��������
			//CGameListItem * pParentListItem=pGameListItem->GetParentListItem();
			//CGameServerItem * pGameServerItem=(CGameServerItem *)pGameListItem;

			////��������
			//tagServerListItem *pServerListItem=NULL;
			//WORD wKindID=pGameServerItem->m_GameServer.wKindID;
			//POSITION pos=m_ServerList.GetHeadPosition();

			////���Ҹ���
			//while(pos!=NULL)
			//{
			//	pServerListItem=m_ServerList.GetNext(pos);
			//	if(pServerListItem && pServerListItem->pGameKindItem->m_GameKind.wKindID == wKindID)
			//	{
			//		break;
			//	}
			//}

			////��������
			//if (pServerListItem!=NULL)
			//{
			//	//���봦��
			//	TCHAR szTitle[64]=TEXT("");
			//	GetGameItemTitle(pGameServerItem,szTitle,CountArray(szTitle));

			//	//�������
			//	pServerListItem->ServerList.AddTail(pGameServerItem);
			//}

			break;
		}
	}
}

//����֪ͨ
VOID CPlatformFrame::OnGameItemUpdate(CGameListItem * pGameListItem)
{
}

//ɾ��֪ͨ
VOID CPlatformFrame::OnGameItemDelete(CGameListItem * pGameListItem)
{

}

//�ؼ�ʹ��
VOID CPlatformFrame::OnEventEnableControls(BOOL bEnabled)
{
	//���ÿؼ�
	m_btQuery.EnableWindow(bEnabled);
	m_btAddItem.EnableWindow(bEnabled);
	m_btModifyItem.EnableWindow(bEnabled);
	m_btDeleteItem.EnableWindow(bEnabled);
}

//��������
VOID CPlatformFrame::OnEventAndroidParenter(WORD wSubCommdID,WORD wParameterCount,tagAndroidParameter * pAndroidParameter)
{
	//��ȡ����
	if(wSubCommdID==SUB_GP_GET_PARAMETER)
	{
		//��������
		int nItemIndex=-1;
		tagAndroidParameter * pTempParameter=NULL;
		tagAndroidParameter * pItemData=NULL;

		//�������
		m_ParemeterList.DeleteAllItems();

		//��������
		LVITEM lVItem;
		lVItem.mask = LVIF_TEXT; 
		lVItem.iSubItem = 0; 
		lVItem.pszText = TEXT(""); 

		//ѭ������
		for(WORD wIndex=0;wIndex<wParameterCount;wIndex++)
		{
			//��ȡ����
			pTempParameter = &pAndroidParameter[wIndex];
			if(pTempParameter==NULL) continue;

			//��������.
			lVItem.iItem=wIndex;
			nItemIndex=m_ParemeterList.InsertItem(&lVItem);
			if(nItemIndex==-1) continue;

			//��������
			SetItemToParameterList(nItemIndex,pTempParameter);		

			//������Դ
			pItemData = new tagAndroidParameter;
			if(pItemData!=NULL)
			{
				//��������
				CopyMemory(pItemData,pTempParameter,sizeof(tagAndroidParameter));

				//��������
				m_ParemeterList.SetItemData(nItemIndex,(DWORD_PTR)pItemData);
			}
		}

		return;
	}

	//��Ӳ���
	if(wSubCommdID==SUB_GP_ADD_PARAMETER)
	{
		//��������
		int nItemIndex=-1;

		//��������
		LVITEM lVItem;
		lVItem.mask = LVIF_TEXT;   
		lVItem.iSubItem = 0; 
		lVItem.pszText = TEXT(""); 

		//��������
		lVItem.iItem=m_ParemeterList.GetItemCount();
		nItemIndex=m_ParemeterList.InsertItem(&lVItem);

		//��������
		SetItemToParameterList(nItemIndex,pAndroidParameter);	

		//������Դ
		tagAndroidParameter * pItemData = new tagAndroidParameter;
		if(pItemData!=NULL)
		{
			//��������
			CopyMemory(pItemData,pAndroidParameter,sizeof(tagAndroidParameter));

			//��������
			m_ParemeterList.SetItemData(nItemIndex,(DWORD_PTR)pItemData);
		}

		return;
	}

	//�޸Ĳ���
	if(wSubCommdID==SUB_GP_MODIFY_PARAMETER)
	{
		//��������
		int nItemIndex=-1;
		tagAndroidParameter * pItemData=NULL;

		//���Ҷ���
		for(int nIndex=0;nIndex<m_ParemeterList.GetItemCount();nIndex++)
		{
			//��ȡ����
			tagAndroidParameter * pItemData = (tagAndroidParameter *)m_ParemeterList.GetItemData(nIndex);
			if(pItemData==NULL) continue;
			if(pAndroidParameter->dwBatchID==pItemData->dwBatchID)
			{
				//��������
				SetItemToParameterList(nIndex,pAndroidParameter);

				//��������
				CopyMemory(pItemData,pAndroidParameter,sizeof(tagAndroidParameter));

				break;
			}
		}		

		return;
	}

	//ɾ������
	if(wSubCommdID==SUB_GP_DELETE_PARAMETER)
	{

		//���Ҷ���
		for(int nIndex=0;nIndex<m_ParemeterList.GetItemCount();nIndex++)
		{
			tagAndroidParameter * pItemData = (tagAndroidParameter *)m_ParemeterList.GetItemData(nIndex);
			if(pItemData==NULL) continue;
			if(pAndroidParameter->dwBatchID==pItemData->dwBatchID)
			{
				//��������
				m_ParemeterList.DeleteItem(nIndex);

				break;
			}
		}		

		return;
	}

	return;
}

//��ʼ���ؼ�
VOID CPlatformFrame::InitControls()
{
	//��������
	m_edtAndroidCount.LimitText(3);
	m_edtTableScoreMin.LimitText(16);
	m_edtTableScoreMax.LimitText(16);
	m_edtSwitchTableMin.LimitText(4);
	m_edtSwitchTableMax.LimitText(4);
	m_edtEnterIntervalMin.LimitText(4);
	m_edtEnterIntervalMax.LimitText(4);
	m_edtLeaveIntervalMin.LimitText(4);
	m_edtLeaveIntervalMax.LimitText(4);
	m_edtCountMember0.LimitText(3);
	m_edtCountMember1.LimitText(3);
	m_edtCountMember2.LimitText(3);
	m_edtCountMember3.LimitText(3);
	m_edtCountMember4.LimitText(3);
	m_edtCountMember5.LimitText(3);

	//��������
	CString strItemText;

	//�����ؼ�
	for(BYTE cbIndex=0;cbIndex<24;cbIndex++)
	{
		strItemText.Format(TEXT("%d"),cbIndex);
		m_cbEnterHour.SetItemData(m_cbEnterHour.InsertString(cbIndex,strItemText),cbIndex);
		m_cbLeaveHour.SetItemData(m_cbLeaveHour.InsertString(cbIndex,strItemText),cbIndex);
	}

	//�����ؼ�
	for(BYTE cbIndex=0;cbIndex<60;cbIndex++)
	{
		strItemText.Format(TEXT("%d"),cbIndex);
		m_cbEnterMinute.SetItemData(m_cbEnterMinute.InsertString(cbIndex,strItemText),cbIndex);
		m_cbEnterSecond.SetItemData(m_cbEnterSecond.InsertString(cbIndex,strItemText),cbIndex);
		m_cbLeaveMinute.SetItemData(m_cbLeaveMinute.InsertString(cbIndex,strItemText),cbIndex);
		m_cbLeaveSecond.SetItemData(m_cbLeaveSecond.InsertString(cbIndex,strItemText),cbIndex);
	}

	//���ÿؼ�
	DWORD dwExstyle = m_ParemeterList.GetExtendedStyle();
	m_ParemeterList.SetExtendedStyle( dwExstyle|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|WS_EX_STATICEDGE );

	//�б�ؼ�
	m_ParemeterList.InsertColumn(0,TEXT("���α�ʶ"),LVCFMT_CENTER,60);
	m_ParemeterList.InsertColumn(1,TEXT("������Ŀ"),LVCFMT_CENTER,60);
	m_ParemeterList.InsertColumn(2,TEXT("����ģʽ"),LVCFMT_CENTER,60);
	m_ParemeterList.InsertColumn(3,TEXT("����ʱ��"),LVCFMT_CENTER,60);
	m_ParemeterList.InsertColumn(4,TEXT("�뿪ʱ��"),LVCFMT_CENTER,60);
	m_ParemeterList.InsertColumn(5,TEXT("Я�����ٷ���"),LVCFMT_CENTER,80);
	m_ParemeterList.InsertColumn(6,TEXT("Я��������"),LVCFMT_CENTER,80);
	m_ParemeterList.InsertColumn(7,TEXT("������С���"),LVCFMT_CENTER,80);
	m_ParemeterList.InsertColumn(8,TEXT("���������"),LVCFMT_CENTER,80);
	m_ParemeterList.InsertColumn(9,TEXT("�뿪��С���"),LVCFMT_CENTER,80);
	m_ParemeterList.InsertColumn(10,TEXT("�뿪�����"),LVCFMT_CENTER,80);
	m_ParemeterList.InsertColumn(11,TEXT("������С����"),LVCFMT_CENTER,80);
	m_ParemeterList.InsertColumn(12,TEXT("����������"),LVCFMT_CENTER,80);
	m_ParemeterList.InsertColumn(13,TEXT("��ͨ��Ա"),LVCFMT_CENTER,60);
	m_ParemeterList.InsertColumn(14,TEXT("һ����Ա"),LVCFMT_CENTER,60);
	m_ParemeterList.InsertColumn(15,TEXT("������Ա"),LVCFMT_CENTER,60);
	m_ParemeterList.InsertColumn(16,TEXT("������Ա"),LVCFMT_CENTER,60);
	m_ParemeterList.InsertColumn(17,TEXT("�ļ���Ա"),LVCFMT_CENTER,60);
	m_ParemeterList.InsertColumn(18,TEXT("�弶��Ա"),LVCFMT_CENTER,60);
}

//����޸�
VOID CPlatformFrame::AddModifyParameter(bool bModify)
{
	//��������
	TCHAR szMessage[128]=TEXT("");
	tagAndroidParameter AndroidParameter;
	ZeroMemory(&AndroidParameter,sizeof(AndroidParameter));

	//ѡ���ж�
	if(bModify==true && m_nSelectItem==-1) return;

	AndroidParameter.AndroidCountMember0 = GetDlgItemInt(IDC_EDIT_COUNT_MEMBER0);
	AndroidParameter.AndroidCountMember1 = GetDlgItemInt(IDC_EDIT_COUNT_MEMBER1);
	AndroidParameter.AndroidCountMember2 = GetDlgItemInt(IDC_EDIT_COUNT_MEMBER2);
	AndroidParameter.AndroidCountMember3 = GetDlgItemInt(IDC_EDIT_COUNT_MEMBER3);
	AndroidParameter.AndroidCountMember4 = GetDlgItemInt(IDC_EDIT_COUNT_MEMBER4);
	AndroidParameter.AndroidCountMember5 = GetDlgItemInt(IDC_EDIT_COUNT_MEMBER5);

	//������Ŀ
    AndroidParameter.dwAndroidCount = 
		AndroidParameter.AndroidCountMember0 + 
		AndroidParameter.AndroidCountMember1 +
		AndroidParameter.AndroidCountMember2 + 
		AndroidParameter.AndroidCountMember3 +
		AndroidParameter.AndroidCountMember4 +
		AndroidParameter.AndroidCountMember5;

	//ͳ�ƻ�����
	DWORD dwAndroidCount=0;
	tagAndroidParameter * pItemData=NULL;
	for(int nIndex=0;nIndex<m_ParemeterList.GetItemCount();nIndex++)
	{
		pItemData = (tagAndroidParameter *)m_ParemeterList.GetItemData(nIndex);
		if(pItemData!=NULL)
		{
			if(bModify==false || nIndex!=m_nSelectItem)
			{
				dwAndroidCount += pItemData->dwAndroidCount;
			}
		}
	}

	if(AndroidParameter.dwAndroidCount<=0 || AndroidParameter.dwAndroidCount+dwAndroidCount>MAX_ANDROID)
	{		
		_sntprintf(szMessage,CountArray(szMessage),TEXT("��Ǹ,ÿ������Ļ�������Ŀ���Ϊ%d��,���������룡"),MAX_ANDROID);

		if (AndroidParameter.AndroidCountMember0<=0)
		{
			_sntprintf(szMessage,CountArray(szMessage),TEXT("��Ǹ,ÿ������Ļ��������������ÿգ�"));
		}
		//��Ϣ��ʾ
		CInformation Information;
		Information.ShowMessageBox(TEXT("��ʾ"),szMessage);

		//���ý���
		m_edtCountMember0.SetFocus();

		return;
	}

	if(AndroidParameter.AndroidCountMember0<0 || AndroidParameter.AndroidCountMember0+dwAndroidCount>MAX_ANDROID)
	{	
		_sntprintf(szMessage,CountArray(szMessage),TEXT("��Ǹ,ÿ�����䷿�����ͨ��Ա��������Ŀ���Ϊ%d��,���������룡"),MAX_ANDROID);		
		CInformation Information;
		Information.ShowMessageBox(TEXT("��ʾ"),szMessage);
		m_edtCountMember0.SetFocus();
		return;
	}   
	if(AndroidParameter.AndroidCountMember1<0 || AndroidParameter.AndroidCountMember1+dwAndroidCount>MAX_ANDROID)
	{		
		_sntprintf(szMessage,CountArray(szMessage),TEXT("��Ǹ,ÿ�������һ����Ա��������Ŀ���Ϊ%d��,���������룡"),MAX_ANDROID);
		CInformation Information;
		Information.ShowMessageBox(TEXT("��ʾ"),szMessage);
		m_edtCountMember1.SetFocus();
		return;
	}  
	if(AndroidParameter.AndroidCountMember2<0 || AndroidParameter.AndroidCountMember2+dwAndroidCount>MAX_ANDROID)
	{		
		_sntprintf(szMessage,CountArray(szMessage),TEXT("��Ǹ,ÿ������Ķ�����Ա��������Ŀ���Ϊ%d��,���������룡"),MAX_ANDROID);
		CInformation Information;
		Information.ShowMessageBox(TEXT("��ʾ"),szMessage);
		m_edtCountMember2.SetFocus();
		return;
	}  
	if(AndroidParameter.AndroidCountMember3<0 || AndroidParameter.AndroidCountMember3+dwAndroidCount>MAX_ANDROID)
	{		
		_sntprintf(szMessage,CountArray(szMessage),TEXT("��Ǹ,ÿ�������������Ա��������Ŀ���Ϊ%d��,���������룡"),MAX_ANDROID);
		CInformation Information;
		Information.ShowMessageBox(TEXT("��ʾ"),szMessage);
		m_edtCountMember3.SetFocus();
		return;
	}  
	if(AndroidParameter.AndroidCountMember4<0 || AndroidParameter.AndroidCountMember4+dwAndroidCount>MAX_ANDROID)
	{		
		_sntprintf(szMessage,CountArray(szMessage),TEXT("��Ǹ,ÿ��������ļ���Ա��������Ŀ���Ϊ%d��,���������룡"),MAX_ANDROID);
		CInformation Information;
		Information.ShowMessageBox(TEXT("��ʾ"),szMessage);
		m_edtCountMember4.SetFocus();
		return;
	}  
	if(AndroidParameter.AndroidCountMember5<0 || AndroidParameter.AndroidCountMember5+dwAndroidCount>MAX_ANDROID)
	{		
		_sntprintf(szMessage,CountArray(szMessage),TEXT("��Ǹ,ÿ��������弶��Ա��������Ŀ���Ϊ%d��,���������룡"),MAX_ANDROID);
		CInformation Information;
		Information.ShowMessageBox(TEXT("��ʾ"),szMessage);
		m_edtCountMember5.SetFocus();
		return;
	}  


	//����ģʽ
	AndroidParameter.dwServiceMode |= ((CButton *)GetDlgItem(IDC_CHECK_SERVICE_MODE1))->GetCheck()==BST_CHECKED?ANDROID_SIMULATE:0;
	AndroidParameter.dwServiceMode |= ((CButton *)GetDlgItem(IDC_CHECK_SERVICE_MODE2))->GetCheck()==BST_CHECKED?ANDROID_INITIATIVE:0;
	AndroidParameter.dwServiceMode |= ((CButton *)GetDlgItem(IDC_CHECK_SERVICE_MODE3))->GetCheck()==BST_CHECKED?ANDROID_PASSIVITY:0;
	if(AndroidParameter.dwServiceMode==0)
	{		
		//��Ϣ��ʾ
		CInformation Information;
		Information.ShowMessageBox(TEXT("��ʾ"),TEXT("�������ٹ�ѡһ�ַ���ģʽ��"));

		return;
	}

	//����ʱ��
	AndroidParameter.dwEnterTime = GetDlgItemInt(IDC_COMBO_ENTER_HOUR)*3600+GetDlgItemInt(IDC_COMBO_ENTER_MINUTE)*60+GetDlgItemInt(IDC_COMBO_ENTER_SECOND);
	AndroidParameter.dwLeaveTime = GetDlgItemInt(IDC_COMBO_LEAVE_HOUR)*3600+GetDlgItemInt(IDC_COMBO_LEAVE_MINUTE)*60+GetDlgItemInt(IDC_COMBO_LEAVE_SECOND);

	//������
    DWORD dwIntervalTime=0;
	if(AndroidParameter.dwLeaveTime>=AndroidParameter.dwEnterTime)
	{
		dwIntervalTime=AndroidParameter.dwLeaveTime-AndroidParameter.dwEnterTime;
	}
	else
	{
		dwIntervalTime=AndroidParameter.dwLeaveTime+24*3600-AndroidParameter.dwEnterTime;
	}

	//���̫��
	if(dwIntervalTime<MIN_INOUTINTERVAL)
	{	
		_sntprintf(szMessage,CountArray(szMessage),TEXT("��Ǹ,�������뿪��ʱ������Ҫ�Ƚ����ʱ����%d���ӣ�"),MIN_INOUTINTERVAL/60);

		//��Ϣ��ʾ
		CInformation Information;
		Information.ShowMessageBox(TEXT("��ʾ"),szMessage);

		return;
	}	

	//���̫��
	if(dwIntervalTime>MAX_INOUTINTERVAL)
	{	
		_sntprintf(szMessage,CountArray(szMessage),TEXT("��Ǹ,�������뿪�ͽ����ʱ�������%dСʱ֮�ڣ�"),MAX_INOUTINTERVAL/3600);

		//��Ϣ��ʾ
		CInformation Information;
		Information.ShowMessageBox(TEXT("��ʾ"),szMessage);

		return;
	}		

	//��������
	TCHAR szValue[32]=TEXT("");

	//��С����
	GetDlgItemText(IDC_EDIT_TAKESCORE_MIN,szValue,CountArray(szValue));
	AndroidParameter.lTakeMinScore = _tcstoi64(szValue,NULL,10);

	//������
	GetDlgItemText(IDC_EDIT_TAKESCORE_MAX,szValue,CountArray(szValue));
	AndroidParameter.lTakeMaxScore = _tcstoi64(szValue,NULL,10);

	//����У��
	if(AndroidParameter.lTakeMaxScore-AndroidParameter.lTakeMinScore<0)
	{
		//��Ϣ��ʾ
		CInformation Information;
		Information.ShowMessageBox(TEXT("��ʾ"),TEXT("��Ǹ,������Я�������������ܵ�����С������"));

		return;
	}

	//������
	AndroidParameter.dwEnterMinInterval=GetDlgItemInt(IDC_EDIT_ENTERINTERVAL_MIN);
	AndroidParameter.dwEnterMaxInterval=GetDlgItemInt(IDC_EDIT_ENTERINTERVAL_MAX);

	//���У��
	if(AndroidParameter.dwEnterMaxInterval<AndroidParameter.dwEnterMinInterval)
	{
		//��Ϣ��ʾ
		CInformation Information;
		Information.ShowMessageBox(TEXT("��ʾ"),TEXT("��Ǹ,�����˽��뷿�����������ܵ�����С�����"));

		return;
	}

	//�뿪���
	AndroidParameter.dwLeaveMinInterval=GetDlgItemInt(IDC_EDIT_LEAVEINTERVAL_MIN);
	AndroidParameter.dwLeaveMaxInterval=GetDlgItemInt(IDC_EDIT_LEAVEINTERVAL_MAX);

	//���У��
	if(AndroidParameter.dwLeaveMaxInterval<AndroidParameter.dwLeaveMinInterval)
	{
		//��Ϣ��ʾ
		CInformation Information;
		Information.ShowMessageBox(TEXT("��ʾ"),TEXT("��Ǹ,�������뿪�������������ܵ�����С�����"));

		return;
	}

	//�뿪���
	AndroidParameter.dwSwitchMinInnings=GetDlgItemInt(IDC_EDIT_SWITCHTABLE_MIN);
	AndroidParameter.dwSwitchMaxInnings=GetDlgItemInt(IDC_EDIT_SWITCHTABLE_MAX);

	//����У��
	if(AndroidParameter.dwSwitchMaxInnings<AndroidParameter.dwSwitchMinInnings)
	{
		//��Ϣ��ʾ
		CInformation Information;
		Information.ShowMessageBox(TEXT("��ʾ"),TEXT("��Ǹ,�����˻���������������С����С������"));

		return;
	}

	if(bModify==false)
	{
		//��Ŀ�ж�
		if(m_ParemeterList.GetItemCount()>=MAX_BATCH)
		{
			_sntprintf(szMessage,CountArray(szMessage),TEXT("��Ǹ,ÿ���������ֻ�����%d�������˲�����"),MAX_BATCH);

			CInformation Information;
			Information.ShowMessageBox(TEXT("��ʾ"),szMessage);

			return;
		}

		//��������
		int nCurSel = m_cbServerList.GetCurSel();
		if(nCurSel==-1) return;		

		//��ȡ����
		WORD wServerID = (WORD)m_cbServerList.GetItemData(nCurSel);
		m_MissionAndroid.AddAndroidParameter(wServerID,&AndroidParameter);
	}
	else
	{
		if(m_nSelectItem==-1) return;
		tagAndroidParameter * pItemData = (tagAndroidParameter *)m_ParemeterList.GetItemData(m_nSelectItem);
		if(pItemData==NULL) return;

		//��������
		int nCurSel = m_cbServerList.GetCurSel();
		if(nCurSel==-1) return;		

		//��ȡ����
		WORD wServerID = (WORD)m_cbServerList.GetItemData(nCurSel);
		AndroidParameter.dwBatchID=pItemData->dwBatchID;
		m_MissionAndroid.ModifyAndroidParameter(wServerID,&AndroidParameter);
	}

	return;
}

//ѡ������
VOID CPlatformFrame::SelectItem(int nIndex)
{
	//���ñ���
	m_nSelectItem =nIndex;
	if(m_nSelectItem==-1) return;

	//��ȡ����
	tagAndroidParameter * pItemData = (tagAndroidParameter * )m_ParemeterList.GetItemData(m_nSelectItem); 
	if(pItemData==NULL) return;

	//������Ŀ
	SetDlgItemInt(IDC_EDIT_COUNT_MEMBER0,pItemData->AndroidCountMember0);
	SetDlgItemInt(IDC_EDIT_COUNT_MEMBER1,pItemData->AndroidCountMember1);
	SetDlgItemInt(IDC_EDIT_COUNT_MEMBER2,pItemData->AndroidCountMember2);
	SetDlgItemInt(IDC_EDIT_COUNT_MEMBER3,pItemData->AndroidCountMember3);
	SetDlgItemInt(IDC_EDIT_COUNT_MEMBER4,pItemData->AndroidCountMember4);
	SetDlgItemInt(IDC_EDIT_COUNT_MEMBER5,pItemData->AndroidCountMember5);

	//����ģʽ
	((CButton *)GetDlgItem(IDC_CHECK_SERVICE_MODE1))->SetCheck((pItemData->dwServiceMode&ANDROID_SIMULATE)?BST_CHECKED:BST_UNCHECKED);
	((CButton *)GetDlgItem(IDC_CHECK_SERVICE_MODE2))->SetCheck((pItemData->dwServiceMode&ANDROID_INITIATIVE)?BST_CHECKED:BST_UNCHECKED);
	((CButton *)GetDlgItem(IDC_CHECK_SERVICE_MODE3))->SetCheck((pItemData->dwServiceMode&ANDROID_PASSIVITY)?BST_CHECKED:BST_UNCHECKED);

	//����Сʱ
	int nHourIndex = pItemData->dwEnterTime/3600;
	m_cbEnterHour.SetCurSel(nHourIndex);

	//�������
	int nMinuteIndex = (pItemData->dwEnterTime-nHourIndex*3600)/60;
	m_cbEnterMinute.SetCurSel(nMinuteIndex);

	//��������
	int nSecondIndex = pItemData->dwEnterTime-nHourIndex*3600-nMinuteIndex*60;
	m_cbEnterSecond.SetCurSel(nSecondIndex);

	//�뿪Сʱ
	nHourIndex = pItemData->dwLeaveTime/3600;
	m_cbLeaveHour.SetCurSel(nHourIndex);

	//�뿪����
	nMinuteIndex = (pItemData->dwLeaveTime-nHourIndex*3600)/60;
	m_cbLeaveMinute.SetCurSel(nMinuteIndex);

	//�뿪����
	nSecondIndex = pItemData->dwLeaveTime-nHourIndex*3600-nMinuteIndex*60;
	m_cbLeaveSecond.SetCurSel(nSecondIndex);

	//Я������
	TCHAR szTakeScore[32]=TEXT("");
	_sntprintf(szTakeScore,CountArray(szTakeScore),TEXT("%I64d"),pItemData->lTakeMinScore);
	m_edtTableScoreMin.SetWindowText(szTakeScore);
	_sntprintf(szTakeScore,CountArray(szTakeScore),TEXT("%I64d"),pItemData->lTakeMaxScore);
	m_edtTableScoreMax.SetWindowText(szTakeScore);

	//������
	SetDlgItemInt(IDC_EDIT_ENTERINTERVAL_MIN,pItemData->dwEnterMinInterval);
	SetDlgItemInt(IDC_EDIT_ENTERINTERVAL_MAX,pItemData->dwEnterMaxInterval);
	
	//�뿪���
	SetDlgItemInt(IDC_EDIT_LEAVEINTERVAL_MIN,pItemData->dwLeaveMinInterval);
	SetDlgItemInt(IDC_EDIT_LEAVEINTERVAL_MAX,pItemData->dwLeaveMaxInterval);

	//��������
	SetDlgItemInt(IDC_EDIT_SWITCHTABLE_MIN,pItemData->dwSwitchMinInnings);
	SetDlgItemInt(IDC_EDIT_SWITCHTABLE_MAX,pItemData->dwSwitchMaxInnings);
}

//��������
VOID CPlatformFrame::SetItemToParameterList(int nItemIndex,tagAndroidParameter * pAndroidParameter)
{
	//����У��
	ASSERT(nItemIndex!=-1 && pAndroidParameter!=NULL);
	if(nItemIndex==-1 || pAndroidParameter==NULL) return;

	//��������
	TCHAR szItemValue[32]=TEXT("");

	//��������
	_sntprintf(szItemValue,CountArray(szItemValue),TEXT("%d"),pAndroidParameter->dwBatchID);
	m_ParemeterList.SetItemText(nItemIndex,0,szItemValue);

	//��������
	_sntprintf(szItemValue,CountArray(szItemValue),TEXT("%d"),pAndroidParameter->dwAndroidCount);
	m_ParemeterList.SetItemText(nItemIndex,1,szItemValue);

	//��������
	_sntprintf(szItemValue,CountArray(szItemValue),TEXT("%d"),pAndroidParameter->dwServiceMode);
	m_ParemeterList.SetItemText(nItemIndex,2,szItemValue);

	//��������
	_sntprintf(szItemValue,CountArray(szItemValue),TEXT("%d"),pAndroidParameter->dwEnterTime);
	m_ParemeterList.SetItemText(nItemIndex,3,szItemValue);

	//��������
	_sntprintf(szItemValue,CountArray(szItemValue),TEXT("%d"),pAndroidParameter->dwLeaveTime);
	m_ParemeterList.SetItemText(nItemIndex,4,szItemValue);

	//��������
	_sntprintf(szItemValue,CountArray(szItemValue),TEXT("%I64d"),pAndroidParameter->lTakeMinScore);
	m_ParemeterList.SetItemText(nItemIndex,5,szItemValue);

	//��������
	_sntprintf(szItemValue,CountArray(szItemValue),TEXT("%I64d"),pAndroidParameter->lTakeMaxScore);
	m_ParemeterList.SetItemText(nItemIndex,6,szItemValue);

	//��������
	_sntprintf(szItemValue,CountArray(szItemValue),TEXT("%d"),pAndroidParameter->dwEnterMinInterval);
	m_ParemeterList.SetItemText(nItemIndex,7,szItemValue);

	//��������
	_sntprintf(szItemValue,CountArray(szItemValue),TEXT("%d"),pAndroidParameter->dwEnterMaxInterval);
	m_ParemeterList.SetItemText(nItemIndex,8,szItemValue);

	//��������
	_sntprintf(szItemValue,CountArray(szItemValue),TEXT("%d"),pAndroidParameter->dwLeaveMinInterval);
	m_ParemeterList.SetItemText(nItemIndex,9,szItemValue);

	//��������
	_sntprintf(szItemValue,CountArray(szItemValue),TEXT("%d"),pAndroidParameter->dwLeaveMaxInterval);
	m_ParemeterList.SetItemText(nItemIndex,10,szItemValue);

	//��������
	_sntprintf(szItemValue,CountArray(szItemValue),TEXT("%d"),pAndroidParameter->dwSwitchMinInnings);
	m_ParemeterList.SetItemText(nItemIndex,11,szItemValue);

	//��������
	_sntprintf(szItemValue,CountArray(szItemValue),TEXT("%d"),pAndroidParameter->dwSwitchMaxInnings);
	m_ParemeterList.SetItemText(nItemIndex,12,szItemValue);

	//��������
	_sntprintf(szItemValue,CountArray(szItemValue),TEXT("%d"),pAndroidParameter->AndroidCountMember0);
	m_ParemeterList.SetItemText(nItemIndex,13,szItemValue);

	//��������
	_sntprintf(szItemValue,CountArray(szItemValue),TEXT("%d"),pAndroidParameter->AndroidCountMember1);
	m_ParemeterList.SetItemText(nItemIndex,14,szItemValue);

	//��������
	_sntprintf(szItemValue,CountArray(szItemValue),TEXT("%d"),pAndroidParameter->AndroidCountMember2);
	m_ParemeterList.SetItemText(nItemIndex,15,szItemValue);

	//��������
	_sntprintf(szItemValue,CountArray(szItemValue),TEXT("%d"),pAndroidParameter->AndroidCountMember3);
	m_ParemeterList.SetItemText(nItemIndex,16,szItemValue);

	//��������
	_sntprintf(szItemValue,CountArray(szItemValue),TEXT("%d"),pAndroidParameter->AndroidCountMember4);
	m_ParemeterList.SetItemText(nItemIndex,17,szItemValue);

	//��������
	_sntprintf(szItemValue,CountArray(szItemValue),TEXT("%d"),pAndroidParameter->AndroidCountMember5);
	m_ParemeterList.SetItemText(nItemIndex,18,szItemValue);

}

//�����ؼ�
VOID CPlatformFrame::RectifyControl(INT nWidth, INT nHeight)
{
	//״̬�ж�
	if ((nWidth==0)||(nHeight==0)) return;

	//�ƶ�׼��
	HDWP hDwp=BeginDeferWindowPos(64);
	UINT uFlags=SWP_NOACTIVATE|SWP_NOCOPYBITS|SWP_NOZORDER;

	//���ư�ť
	/*DeferWindowPos(hDwp,m_btMin,NULL,nWidth-88,1,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btClose,NULL,nWidth-32,1,0,0,uFlags|SWP_NOSIZE);*/

	//��������
	LockWindowUpdate();
	EndDeferWindowPos(hDwp);
	UnlockWindowUpdate();

	//���½���
	RedrawWindow(NULL,NULL,RDW_ERASE|RDW_INVALIDATE);

	return;
}

//�ر���Ϣ
VOID CPlatformFrame::OnClose()
{
	//��¼��ʶ
	if(m_bLogonSuccess==true)
	{
		//ѡ����ʾ
		CInformation Information;
		INT nRetCode = Information.ShowMessageBox(TEXT("ϵͳ��ʾ"),TEXT("��ȷ��Ҫ�˳���������"),MB_YESNO);

		//��ʱ���˳�
		if(nRetCode==IDNO) return;
	}

	//���ٴ���
	DestroyWindow();

	__super::OnClose();
}

//������Ϣ
VOID CPlatformFrame::OnDestroy()
{
	__super::OnDestroy();

	//ɾ������
	m_ParemeterList.DeleteAllItems();

    //delete this;
}

//�滭����
BOOL CPlatformFrame::OnEraseBkgnd(CDC * pDC)
{
	//��ȡλ��
	CRect rcClient;
	GetClientRect(&rcClient);

	//�������
	pDC->SetBkMode(TRANSPARENT);
	pDC->FillSolidRect(rcClient,RGB(215,223,228));

	return TRUE;
}

//ʱ����Ϣ
VOID CPlatformFrame::OnTimer(UINT_PTR nIDEvent)
{
	__super::OnTimer(nIDEvent);
}

//λ����Ϣ
VOID CPlatformFrame::OnSize(UINT nType, INT cx, INT cy) 
{
	__super::OnSize(nType, cx, cy);

	//�����ؼ�
	RectifyControl(cx,cy);

	return;
}


//λ����Ϣ
VOID CPlatformFrame::OnGetMinMaxInfo(MINMAXINFO * lpMMI)
{
	__super::OnGetMinMaxInfo(lpMMI);

	return;
}

//��ʼ����
BOOL CPlatformFrame::OnInitDialog()
{
	__super::OnInitDialog();

	//���ô���
	ModifyStyle(WS_CAPTION|WS_MAXIMIZE|WS_MINIMIZE, WS_MINIMIZEBOX|WS_MAXIMIZEBOX);

	//����ͼ��
	HINSTANCE hInstance=AfxGetInstanceHandle();
	SetIcon(LoadIcon(hInstance,MAKEINTRESOURCE(IDR_MAINFRAME)),TRUE);
	SetIcon(LoadIcon(hInstance,MAKEINTRESOURCE(IDR_MAINFRAME)),FALSE);

	//���ñ���
	SetWindowText(TEXT("�����˹�����"));

	//���ýӿ�
	m_MissionAndroid.SetAndroidOperateCenter(this);

	//���ûص�
	CServerListData::GetInstance()->SetServerListDataSink(this);

	//��ʼ���ؼ�
	InitControls();

	//ע���¼�
	CPlatformEvent * pPlatformEvent=CPlatformEvent::GetInstance();
	if (pPlatformEvent!=NULL) pPlatformEvent->RegisterEventWnd(m_hWnd);

	//��¼ϵͳ
	PostMessage(WM_COMMAND,IDM_USER_LOGON,0);

	return TRUE;  
}

//�����Ϣ
VOID CPlatformFrame::OnLButtonDown(UINT nFlags, CPoint Point)
{
	__super::OnLButtonDown(nFlags,Point);

	return;
}

//�����Ϣ
VOID CPlatformFrame::OnLButtonDblClk(UINT nFlags, CPoint Point)
{
	__super::OnLButtonDblClk(nFlags,Point);
	
	return;
}

//���øı�
VOID CPlatformFrame::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	__super::OnSettingChange(uFlags,lpszSection);

	return;
}


//�¼���Ϣ
LRESULT CPlatformFrame::OnMessagePlatformEvent(WPARAM wParam, LPARAM lParam)
{
	//�¼�����
	switch (wParam)
	{
	case EVENT_USER_LOGON:			//��¼���
		{
			//��ʾ����
			ShowWindow(SW_SHOW);
			SetForegroundWindow();

			//���ñ���
			m_bLogonSuccess=true;

			return 0L;
		}	
	}

	return 0L;
}

//ѡ��ı�
VOID CPlatformFrame::OnCbnComboGameSelchange()
{
	//��ȡ����
	int nSelectIndex = m_cbGameList.GetCurSel();
	if(nSelectIndex==-1) return;

	//���ͱ�ʶ
	WORD wKindID = (WORD)m_cbGameList.GetItemData(nSelectIndex);

	//�Ƴ�����
	while(m_cbServerList.GetCount()>0) m_cbServerList.DeleteString(0);

	//��������
	POSITION Position=NULL;
	CGameServerItem * pGameServerItem=NULL;
    do
	{
		pGameServerItem=m_ServerListData.EmunGameServerItem(Position);
		if(pGameServerItem!=NULL && pGameServerItem->m_GameServer.wKindID==wKindID)
		{
			m_cbServerList.SetItemData(m_cbServerList.AddString(pGameServerItem->m_GameServer.szServerName),pGameServerItem->m_GameServer.wServerID);
		}
	}
	while(Position!=NULL);

	//Ĭ��ѡ��
	if(m_cbServerList.GetCount()>0) m_cbServerList.SetCurSel(0);

	//���¿ؼ�
	m_cbServerList.Invalidate();
}

//��������
VOID CPlatformFrame::OnHdnListParameterItemclick(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);

	//ѡ������
	SelectItem(phdr->iItem);

	*pResult = 0;
}

//ɾ������
VOID CPlatformFrame::OnLvnListParameterDeleteitem(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	//��ȡ����
	tagAndroidParameter * pItemData = (tagAndroidParameter * )m_ParemeterList.GetItemData(pNMLV->iItem); 
	if(pItemData!=NULL)
	{
		SafeDelete(pItemData);
	}

	*pResult = 0;
}

//���̰���
VOID CPlatformFrame::OnLvnKeydownListParameter(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLVKEYDOWN pLVKeyDow = reinterpret_cast<LPNMLVKEYDOWN>(pNMHDR);

	//��������
	int nItem=-1;
	POSITION pos;

	switch(pLVKeyDow->wVKey) 
	{
	case VK_DOWN:
		{
			pos=m_ParemeterList.GetFirstSelectedItemPosition();
			while (pos) 
			{
				//��ȡ����
				nItem=m_ParemeterList.GetNextSelectedItem(pos);
				if(nItem==-1) break;

				//��������
				nItem = __min(nItem+1,m_ParemeterList.GetItemCount()-1);
				SelectItem(nItem);
				break;
			}

			break;
		}
	case VK_UP:
		{
			pos=m_ParemeterList.GetFirstSelectedItemPosition();
			while (pos) 
			{
				//��ȡ����
				nItem=m_ParemeterList.GetNextSelectedItem(pos);
				if(nItem==-1) break;

				//��������
				nItem = __max(nItem-1,0);
				SelectItem(nItem);
				break;
			}
			break;
		}
	}

	*pResult = 0;
}

//�������
VOID CPlatformFrame::OnBnClickedBtAdd()
{
	//�������
	AddModifyParameter(false);
}

//�༭����
VOID CPlatformFrame::OnBnClickedBtModify()
{
	//�������
	AddModifyParameter(true);
}

//ɾ������
VOID CPlatformFrame::OnBnClickedBtDelete()
{
	//��ȡ����
	if(m_nSelectItem==-1) return;
	if(m_ParemeterList.GetItemCount()<=m_nSelectItem)
	{
		m_nSelectItem=-1;
		return;
	}

	//��ȡ����
	int nIndex = m_cbServerList.GetCurSel();
	if(nIndex==-1) return;

	//��ȡ����
	tagAndroidParameter * pItemData = (tagAndroidParameter * )m_ParemeterList.GetItemData(m_nSelectItem); 
	if(pItemData==NULL) return;

	//��ȡʱ��
	SYSTEMTIME SystemTime;
	GetLocalTime(&SystemTime);
	DWORD dwTodayTickCount= SystemTime.wHour*3600+SystemTime.wMinute*60+SystemTime.wSecond;

	//ʱ���ж�
	if(dwTodayTickCount>=pItemData->dwEnterTime && dwTodayTickCount<=pItemData->dwLeaveTime)
	{
		//ѡ����ʾ
		CInformation Information;
		INT nRetCode = Information.ShowMessageBox(TEXT("����"),TEXT("���������������ڷ���״̬,��ȷ��Ҫɾ����"),MB_YESNO);

		//��ʱ���˳�
		if(nRetCode==IDNO) return;
	}

	//��ȡ����
	WORD wServerID = (WORD)m_cbServerList.GetItemData(nIndex);

	//ɾ������
	m_MissionAndroid.DeleteAndroidParameter(wServerID,pItemData->dwBatchID);

	//���ñ���
	m_nSelectItem=-1;

	return;
}

//��ѯ����
VOID CPlatformFrame::OnBnClickedBtQuery()
{
	//��ȡ����
	int nIndex = m_cbServerList.GetCurSel();
	if(nIndex==-1) return;

	//��ȡ����
	WORD wServerID = (WORD)m_cbServerList.GetItemData(nIndex);

	//��ȡ����
	m_MissionAndroid.GetAndroidParameter(wServerID);

	return;
}

//////////////////////////////////////////////////////////////////////////////////
