#include "StdAfx.h"
#include <algorithm>
#include <shellapi.h>
#include "PlatformEvent.h"
#include "PlatformFrame.h"
#include "PlazaHallGame.h"
#include "GlobalUnits.h"
#include "DlgAddGame.h"
#include "DlgInformation.h"
#include "DlgPhoneScanCode.h"

//////////////////////////////////////////////////////////////////////////////////
//�ؼ�����



//�м���Ϸ
const TCHAR* const szButtonEditControlName = TEXT("ButtonEdit");
const TCHAR* const szButtonAddGameControlName = TEXT("ButtonAddGame");
const TCHAR* const szHallGameMainControlName = TEXT("HallGameMain");
const TCHAR* const szItemGameIOCControlName = TEXT("ItemGameIOC");
const TCHAR* const szItemGameNameControlName = TEXT("ItemGameName");
const TCHAR* const szItemGameSelectControlName = TEXT("ItemGameSelect");
const TCHAR* const szItemGameDeleteControlName = TEXT("ItemGameDelete");
const TCHAR* const szItemPlazaSelectControlName = TEXT("ItemPlazaSelect");

const TCHAR* const szItemGameDownControlName = TEXT("ItemGameDown");
const TCHAR* const szItemGameInstallingControlName = TEXT("ItemGameInstalling");
const TCHAR* const szItemGameDownloadingControlName = TEXT("ItemGameDownloading");
const TCHAR* const szItemGameQueuingControlName = TEXT("ItemGameQueuing");
const TCHAR* const szItemGameProgressControlName = TEXT("ItemGameProgress");

//���¹��
const TCHAR* const szHallGameAdControlName = TEXT("HallGameAd");
const TCHAR* const szItemGameAdIOCControlName = TEXT("ItemGameAdIOC");
const TCHAR* const szItemGameAdNameControlName = TEXT("ItemGameAdName");
const TCHAR* const szItemGameAdSelectControlName = TEXT("ItemGameAdSelect");

//�м䷿��
const TCHAR* const szHallRoomMainControlName = TEXT("HallRoomMain");
const TCHAR* const szLabelGameNameControlName = TEXT("LabelGameName");
const TCHAR* const szLabelGameTileControlName = TEXT("LabelGameTile");
const TCHAR* const szButtonReturnHallControlName = TEXT("ButtonReturnHall");
const TCHAR* const szButtonGameRuleControlName = TEXT("ButtonGameRule");

//��Ϸ����
const TCHAR* const szItemRoomSelectControlName = TEXT("ItemRoomSelect");
const TCHAR* const szItemRoomNameControlName = TEXT("ItemRoomName");
const TCHAR* const szItemRoomTypeControlName = TEXT("ItemRoomType");
const TCHAR* const szItemRoomTypeCheatControlName = TEXT("ItemRoomTypeCheat");
const TCHAR* const szItemRoomTypePassWordControlName = TEXT("ItemRoomTypePassWord");
const TCHAR* const szItemRoomBaseScoreControlName = TEXT("ItemRoomBaseScore");
const TCHAR* const szItemRoomLimitFreeControlName = TEXT("ItemRoomLimitFree");
const TCHAR* const szItemRoomLimitScoreControlName = TEXT("ItemRoomLimitScore");
const TCHAR* const szItemRoomStateFreeControlName = TEXT("ItemRoomStateFree");
const TCHAR* const szItemRoomStateHotControlName = TEXT("ItemRoomStateHot");
const TCHAR* const szItemRoomStateBusyControlName = TEXT("ItemRoomStateBusy");
const TCHAR* const szItemRoomUserAllControlName = TEXT("ItemRoomUserAll");
const TCHAR* const szItemRoomUserVipControlName = TEXT("ItemRoomUserVip");
const TCHAR* const szItemRoomVipLevelControlName = TEXT("ItemRoomVipLevel");
const TCHAR* const szItemRoomVipSuffixControlName = TEXT("ItemRoomVipSuffix");

//��������
const TCHAR* const szItemRoomMatchSelectControlName = TEXT("ItemRoomMatchSelect");
const TCHAR* const szItemRoomMatchNameControlName = TEXT("ItemRoomMatchName");
const TCHAR* const szItemRoomMatchTypeControlName = TEXT("ItemRoomMatchType");
const TCHAR* const szItemRoomMatchPassWordControlName = TEXT("ItemRoomMatchPassWord");
const TCHAR* const szItemRoomMatchSignFreeControlName = TEXT("ItemRoomMatchSignFree");
const TCHAR* const szItemRoomMatchSignScoreControlName = TEXT("ItemRoomMatchSignScore");
const TCHAR* const szItemRoomMatchRuleNameControlName = TEXT("ItemRoomMatchRuleName");
const TCHAR* const szItemRoomMatchRuleContextControlName = TEXT("ItemRoomMatchRuleContext");
const TCHAR* const szItemRoomMatchTimeControlName = TEXT("ItemRoomMatchTime");
const TCHAR* const szItemRoomMatchConditionControlName = TEXT("ItemRoomMatchCondition");
//////////////////////////////////////////////////////////////////////////////////
CStdString	m_ImageUnSure=TEXT("file=\'GAME_KIND\\KIND_UNSURE.png\' restype=\'FILE\'");
//////////////////////////////////////////////////////////////////////////////////

//���캯��
CPlazaHallGame::CPlazaHallGame(CPaintManagerUI*	pPaintManager):m_pPaintManager(pPaintManager)
{
	//�ؼ�����
	m_pUpdateStatusView=NULL;
}

//��������
CPlazaHallGame::~CPlazaHallGame()
{
	//�ͷŶ���
	if(m_pUpdateStatusView != NULL) SafeDelete(m_pUpdateStatusView);
}



//��Ϣ����
bool CPlazaHallGame::Notify(TNotifyUI &  msg)
{
	//��ȡ����
	CControlUI * pControlUI = msg.pSender;

	//����¼�
	if (lstrcmp(msg.sType, TEXT("click")) == 0)
	{
		if(lstrcmp(pControlUI->GetName(), szButtonGameRuleControlName)==0) 
		{
			//��ȡ����
			CGlobalWebLink * pGlobalWebLink=CGlobalWebLink::GetInstance();

			//�����ַ
			TCHAR szBrowser[MAX_PATH]=TEXT("");
			_sntprintf(szBrowser,CountArray(szBrowser),TEXT("%s/GameRule.aspx?GameID=%ld&KindID=%ld"),pGlobalWebLink->GetPlatformLink(),m_pEnterGame->m_GameKind.wGameID,m_pEnterGame->m_GameKind.wKindID);

			//��ҳ��
			ShellExecute(NULL,TEXT("OPEN"),szBrowser,NULL,NULL,SW_NORMAL);

		}
		if(lstrcmp(pControlUI->GetName(), szButtonEditControlName)==0) 
		{
			UpdateDeleteUI();
			return true;
		}
		else if(lstrcmp(pControlUI->GetName(), szItemGameDeleteControlName)==0) 
		{
			CGameKindItem * pGameKind = (CGameKindItem*)pControlUI->GetTag();
			DeleteGameKind(pGameKind);
			return true;
		}
		else if(lstrcmp(pControlUI->GetName(), szItemPlazaSelectControlName)==0) 
		{
			CPhoneScanCodePlaza dlg;
			dlg.DoModal();
			return true;
		}
		else if(lstrcmp(pControlUI->GetName(), szButtonAddGameControlName)==0) 
		{
			AddGameKind();
			return true;
		}
		else if(lstrcmp(pControlUI->GetName(), szItemGameSelectControlName)==0 )
		{
			
			CGameKindItem * pGameKind = (CGameKindItem*)pControlUI->GetTag();
			EnterGameKind(pGameKind);
			return true;
		}
		else if(lstrcmp(pControlUI->GetName(), szItemGameAdSelectControlName)==0 )
		{
			CGameKindItem * pGameKind = (CGameKindItem*)pControlUI->GetTag();
			EnterGameAdKind(pGameKind);
			return true;
		}
		else if(lstrcmp(pControlUI->GetName(), _T("ButtonReturnHall"))==0 )
		{
			ReturnHallMain();
			return true;
		}
		else if(lstrcmp(pControlUI->GetName(), szItemRoomSelectControlName)==0 || lstrcmp(pControlUI->GetName(), szItemRoomMatchSelectControlName)==0 )
		{
			CGameServerItem * pGameServer = (CGameServerItem *)pControlUI->GetTag();
			EnterGameServer(pGameServer);
			return true;
		}
	}

	return false;
}

//��ʼ�ؼ�
bool CPlazaHallGame::InitControlUI()
{
	return true;
}

bool CPlazaHallGame::IsNotify(TNotifyUI &  msg)
{
	return true;
}

CControlUI * CPlazaHallGame::GetControlByName( LPCTSTR pszControlName )
{
	return m_pPaintManager->FindControl(pszControlName);
}

VOID CPlazaHallGame::InitData()
{
	//��������

}

VOID CPlazaHallGame::ShowPlazaHallGame()
{
	LoadDataFromMemory();
	LoadDataFromFile();
	FilterGameKind();
	UpdateUI();
	UpdateAdUI();
}

VOID CPlazaHallGame::UpdateUI()
{
	//���Ҷ���
	CContainerUI * pCnt = (CContainerUI * )GetControlByName(_T("PlazaHallRoom"));
	if(pCnt!=NULL)
	{
		pCnt->SetVisible(false);
	}
	//���Ҷ���
	pCnt = (CContainerUI * )GetControlByName(_T("PlazaHallGame"));
	if(pCnt!=NULL)
	{
		pCnt->SetVisible();
	}

	//���Ҷ���
	CPageLayoutUI * pGame = (CPageLayoutUI * )(GetControlByName(szHallGameMainControlName));
	if (pGame != NULL)
	{
		pGame->RemoveAll();
		m_vMyGameDelete.clear();
		m_vMyGameDown.clear();

		//��ȡ����
		TCHAR szPath[MAX_PATH]=TEXT("");
		CWHService::GetWorkDirectory(szPath, sizeof(szPath));
		TCHAR szFileName[MAX_PATH]=TEXT("");
		_sntprintf(szFileName,CountArray(szFileName),TEXT("%s\\PlazaConfig.INI"),szPath);

		TCHAR	szCode[MAX_PATH];
		ZeroMemory(szCode,CountArray(szCode));
		GetPrivateProfileString(TEXT("Code"),TEXT("DisEnable"),TEXT(""),szCode,CountArray(szCode),szFileName);
		int nExpandAll = _ttoi(szCode);
		//������ά��
		if (nExpandAll==0)
		{
			CDialogBuilder builderPlaza;
			CContainerUI *pNewContainerPlaza = NULL;
			pNewContainerPlaza = static_cast<CContainerUI*>(builderPlaza.Create( _T("PlazaHallGamePlaza.xml"), (UINT)0, NULL, m_pPaintManager));
			pGame->Add(pNewContainerPlaza);
		}


		//���ر���Ԫ��
		for (vector<CGameKindItem*>::iterator iter1 = m_vMyGameKind.begin();iter1!=m_vMyGameKind.end();iter1++)
		{
			CGameKindItem * pGameKind = (CGameKindItem *)(*iter1);
			CDialogBuilder builder;
			CContainerUI *pNewContainerItem = NULL;
			pNewContainerItem = static_cast<CContainerUI*>(builder.Create( _T("PlazaHallGameItem.xml"), (UINT)0, NULL, m_pPaintManager));

			TCHAR szProcessName[LEN_PROCESS]=TEXT("");
			GetProcessName(pGameKind,szProcessName);

			CStdString	csImageKind;
			csImageKind.Format(TEXT("file=\'GAME_KIND\\GameKind_%s.png\' restype=\'FILE\'"),szProcessName);

			if(m_pPaintManager->GetImageEx(csImageKind)==NULL) csImageKind.Empty();

			if(csImageKind.IsEmpty()==true) csImageKind = m_ImageUnSure;

			CControlUI *pIco = pNewContainerItem->FindControl(szItemGameIOCControlName);
			if (pIco!=NULL)
			{
				pIco->SetBkImage(csImageKind);
			}
			CControlUI *pName = pNewContainerItem->FindControl(szItemGameNameControlName);
			if (pName!=NULL)
			{
				pName->SetText(pGameKind->m_GameKind.szKindName);
			}
			CControlUI *pSelect = pNewContainerItem->FindControl(szItemGameSelectControlName);
			if (pSelect!=NULL)
			{
				pSelect->SetTag((UINT_PTR)pGameKind);
			}
			CControlUI *pDelete = pNewContainerItem->FindControl(szItemGameDeleteControlName);
			if (pDelete!=NULL)
			{
				pDelete->SetTag((UINT_PTR)pGameKind);
				m_vMyGameDelete.push_back(pDelete);
			}
			CContainerUI *pDown = (CContainerUI *)(pNewContainerItem->FindControl(szItemGameDownControlName));
			if (pDown!=NULL)
			{
				pDown->SetTag((UINT_PTR)pGameKind);
				pDown->SetVisible(false);
				m_vMyGameDown.push_back(pDown);
			}

			pGame->Add(pNewContainerItem);
		}
		//���ؼӺ�
		CDialogBuilder builderAdd;
		CContainerUI *pContainerAdd = NULL;
		pContainerAdd = static_cast<CContainerUI*>(builderAdd.Create( _T("PlazaHallGameItemAdd.xml"), (UINT)0, NULL, m_pPaintManager));
		pGame->Add(pContainerAdd);

		UpdateDeleteUI();
	}


}

bool CPlazaHallGame::CheckUpdate( CGameKindItem *vMyGameKind)
{
	//����У��
	if(vMyGameKind==NULL) return false;

	//��������
	WORD wKindID = vMyGameKind->m_GameKind.wKindID;

	//���Ҷ���
	CServerListData * pServerListData=CServerListData::GetInstance();
	CGameKindItem * pGameKindItem=pServerListData->SearchGameKind(wKindID);
	if (pGameKindItem!=NULL)
	{
		//�����ж�
		if(pGameKindItem->m_GameKind.wGameID==0)
		{
			//��ʾ��Ϣ
			CInformation Information(NULL);
			Information.ShowMessageBox(TEXT("�������ߣ������ڴ���"),MB_ICONINFORMATION);
			return false;
		}
		//��ȡ�汾
        CWHService::GetModuleVersion(pGameKindItem->m_GameKind.szProcessName,pGameKindItem->m_dwProcessVersion);
		//��װ�ж�
		if (pGameKindItem->m_dwProcessVersion==0L)
		{
			DownLoadGame(pGameKindItem->m_GameKind.szKindName,wKindID,0);
			return false;
		}		
	}

	return true;
}

VOID CPlazaHallGame::UpdateAdUI()
{
	//���Ҷ���
	CPageLayoutUI * pGameAd = (CPageLayoutUI * )(GetControlByName(szHallGameAdControlName));
	if (pGameAd != NULL)
	{
		pGameAd->RemoveAll();
		//���ر���Ԫ��
		for (vector<CGameKindItem*>::iterator iter1 = m_vAdGameKind.begin();iter1!=m_vAdGameKind.end();iter1++)
		{
			CGameKindItem * pGameKind = (CGameKindItem *)(*iter1);
			CDialogBuilder builder;
			CContainerUI *pNewContainerItem = NULL;
			pNewContainerItem = static_cast<CContainerUI*>(builder.Create( _T("PlazaHallGameItemAd.xml"), (UINT)0, NULL, m_pPaintManager));


			TCHAR szProcessName[LEN_PROCESS]=TEXT("");
			GetProcessName(pGameKind,szProcessName);

			CStdString	csImageKind;
			csImageKind.Format(TEXT("file=\'GAME_KIND\\GameKind_%s.png\' restype=\'FILE\'"),szProcessName);

			if(m_pPaintManager->GetImageEx(csImageKind)==NULL) csImageKind.Empty();

			if(csImageKind.IsEmpty()==true) csImageKind = m_ImageUnSure;

			CControlUI *pIco = pNewContainerItem->FindControl(szItemGameAdIOCControlName);
			if (pIco!=NULL)
			{
				pIco->SetBkImage(csImageKind);
			}

			CControlUI *pName = pNewContainerItem->FindControl(szItemGameAdNameControlName);
			if (pName!=NULL)
			{
				pName->SetText(pGameKind->m_GameKind.szKindName);
			}
			CControlUI *pSelect = pNewContainerItem->FindControl(szItemGameAdSelectControlName);
			if (pSelect!=NULL)
			{
				pSelect->SetTag((UINT_PTR)pGameKind);
			}

			pGameAd->Add(pNewContainerItem);
		}

	}

}

void CPlazaHallGame::GetProcessName(CGameKindItem *vMyGameKind, TCHAR szProcessName[LEN_PROCESS] )
{
	//ָ��У��
	if(vMyGameKind==NULL) return;

	//��������
	lstrcpyn(szProcessName,vMyGameKind->m_GameKind.szProcessName,sizeof(TCHAR)*LEN_PROCESS);

	//��������
	LPTSTR pszProcess = (_tcsrchr(szProcessName,TEXT('.'))); 

	//�����ַ�
	pszProcess[0]=TEXT('\0');

	return;

}

VOID CPlazaHallGame::OnEventDownLoadTaskFinish()
{

	WORD wServerID = CGlobalUnits::GetInstance()->GetTrackServerID();
	CServerListData * pServerListData=CServerListData::GetInstance();
	CGameServerItem * pGameServerItem=pServerListData->SearchGameServer(wServerID);
	CPlatformFrame * pPlatformFrame=CPlatformFrame::GetInstance();

	if (pGameServerItem!=NULL) pPlatformFrame->EntranceServerItem(pGameServerItem);
}

VOID CPlazaHallGame::OnEventUpdateCheckFinish(bool bNeedUpdate)
{
	if(bNeedUpdate == false)
	{

		WORD wServerID = CGlobalUnits::GetInstance()->GetTrackServerID();
		CServerListData * pServerListData=CServerListData::GetInstance();
		CGameServerItem * pGameServerItem=pServerListData->SearchGameServer(wServerID);
		CPlatformFrame * pPlatformFrame=CPlatformFrame::GetInstance();

		if (pGameServerItem!=NULL) pPlatformFrame->EntranceServerItem(pGameServerItem);
	}
}

bool CPlazaHallGame::SendMessage( UINT message, WPARAM wParam , LPARAM lParam )
{
	//�����¼�
	CPlatformEvent * pPlatformEvent=CPlatformEvent::GetInstance();
	if (pPlatformEvent!=NULL) pPlatformEvent->SendPlatformEvent(message,(LPARAM)wParam);

	return true;
}

bool CPlazaHallGame::DownLoadGame( LPCTSTR pszClientName, WORD wKindID, WORD wServerID )
{
	////������Ϸ
	//for (INT_PTR i=0;i<m_DownLoadSinkArray.GetCount();i++)
	//{
	//	//��ȡ����
	//	ASSERT(m_DownLoadSinkArray[i]!=NULL);
	//	CPlazaHallGameDownLoadSink * pDownLoadSink=m_DownLoadSinkArray[i];

	//	//�����ж�
	//	if (pDownLoadSink->m_wKindID==wKindID)
	//	{

	//		return true;
	//	}
	//}

	for (vector<CPlazaHallGameDownLoadSink *>::iterator iter=m_vDwonLoadSink.begin();iter!=m_vDwonLoadSink.end();iter++)
	{
		CPlazaHallGameDownLoadSink *pDownLoadSink=*iter;
		if (pDownLoadSink->m_wKindID==wKindID)
		{
			return true;
		}

	}


	//��������
	CPlazaHallGameDownLoadSink * pDownLoadSink=NULL;
	try
	{
		//��������
		pDownLoadSink=new CPlazaHallGameDownLoadSink;

		//�����ж�
		if (pDownLoadSink==NULL)
		{
			ASSERT(FALSE);
			return false;
		}
	}
	catch (...)
	{
		ASSERT(FALSE);
		return false;
	}

	////��������
	//m_DownLoadSinkArray.Add(pDownLoadSink);

	m_vDwonLoadSink.push_back(pDownLoadSink);

	//��������
	pDownLoadSink->m_pDlgDownLoad=this;
	pDownLoadSink->PerformDownLoad(pszClientName,wKindID,wServerID);

	return true;
}

bool CPlazaHallGame::OnMessagePlatformEvent( WPARAM wParam, LPARAM lParam )
{
	if (wParam == WM_ITEM_STATUS_UPDATE)
	{
		//��ȡ����
		ASSERT((CPlazaHallGameDownLoadSink *)lParam!=NULL);
		CPlazaHallGameDownLoadSink * pDownLoadSink=(CPlazaHallGameDownLoadSink *)lParam;
	
		//ˢ��UI
		UpdateDownUI(pDownLoadSink);

		//��������
		if (pDownLoadSink->m_cbDownLoadStatus == DOWN_LOAD_FINISH)
		{
			UpdateDeleteUI();
		}

		for (vector<CPlazaHallGameDownLoadSink *>::iterator iter=m_vDwonLoadSink.begin();iter!=m_vDwonLoadSink.end();iter++)
		{
			CPlazaHallGameDownLoadSink *pDownLoadSinkItem=*iter;
			if (pDownLoadSinkItem->m_wKindID==pDownLoadSink->m_wKindID)
			{
				m_vDwonLoadSink.erase(iter);
				break;
			}

		}

	}
	else if (wParam == WM_ITEM_QUERY_UPDATE)
	{
		//��ȡ����
		ASSERT((CGameKindItem *)lParam!=NULL);
		CGameKindItem * pGameKind=(CGameKindItem *)lParam;
		CheckUpdate(pGameKind);

	}
	else if(wParam == WM_ITEM_ADD_GAME)
	{
		
		AddGameKind();
	}
	else if (wParam == WM_ITEM_ENTER_SERVER)
	{
		EnterGameServer((CGameServerItem*)lParam);
	}

	return 0L;
}

VOID CPlazaHallGame::UpdateDownUI( CPlazaHallGameDownLoadSink * pDownLoadSink )
{
	//����״̬
	for (vector<CContainerUI*>::iterator iter1 = m_vMyGameDown.begin();iter1!=m_vMyGameDown.end();iter1++)
	{
		CContainerUI * pDwon = (CContainerUI *)(*iter1);
		if (pDwon!=NULL)
		{
			
			CGameKindItem * pGameKind = (CGameKindItem *)(pDwon->GetTag());
			if (pGameKind->m_GameKind.wKindID == pDownLoadSink->m_wKindID)
			{
				//��������
				UpdateDownStateUI(pDwon,pDownLoadSink->m_cbDownLoadStatus);

				//���ý���

				//�������
				DWORD dwDownScale=0L;
				if (pDownLoadSink->m_lTotalFileSize>0L)
				{
					dwDownScale=DWORD(pDownLoadSink->m_lDownLoadSize*100L/pDownLoadSink->m_lTotalFileSize);
				}
				CProgressUI *pProgress = (CProgressUI *)pDwon->FindControl(szItemGameProgressControlName);
				if (pProgress!=NULL)
				{
					pProgress->SetValue(dwDownScale);
				}
				//��������
				bool bShow = false;
				switch (pDownLoadSink->m_cbDownLoadStatus)
				{
				case DOWN_LOAD_LINK:	
				case DOWN_LOAD_READ:			
				case DOWN_LOAD_SETUP:			
					{
						bShow = true;
						break;
					}
				case DOWN_LOAD_FINISH:	
					{
						//�������ˣ��Զ�����
						bShow = false;
						break;
					}
				}
				pDwon->SetVisible(bShow);
			}
		}
	}

	UpdateDeleteUI();
}

VOID CPlazaHallGame::UpdateDownStateUI(CContainerUI * pDwon, int nState )
{

	//switch (nState)
	//{
	//case DOWN_LOAD_IDLE:
	//case DOWN_LOAD_LINK: { return TEXT("��������"); }
	//case DOWN_LOAD_READ: { return TEXT("��������"); }
	//case DOWN_LOAD_SETUP: { return TEXT("���ڰ�װ"); }
	//case DOWN_LOAD_ERROR: { return TEXT("����ʧ��"); }
	//case DOWN_LOAD_FINISH: { return TEXT("��װ�ɹ�"); }
	//}

	CControlUI *pState = pDwon->FindControl(szItemGameInstallingControlName);
	if (pState!=NULL)
	{
		pState->SetVisible(nState == DOWN_LOAD_SETUP);
	}
	pState = pDwon->FindControl(szItemGameDownloadingControlName);
	if (pState!=NULL)
	{
		pState->SetVisible(nState == DOWN_LOAD_READ);
	}
	pState = pDwon->FindControl(szItemGameQueuingControlName);
	if (pState!=NULL)
	{
		pState->SetVisible(nState == DOWN_LOAD_LINK);
	}
}

bool CPlazaHallGame::SaveGameKind()
{
	//�û���Ϣ
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	//��ȡĿ¼
	TCHAR szWorkDirectory[MAX_PATH]=TEXT("");
	CWHService::GetWorkDirectory(szWorkDirectory,CountArray(szWorkDirectory));

	//����·��
	TCHAR szUserGameForder[MAX_PATH]=TEXT("");
	_sntprintf(szUserGameForder,CountArray(szUserGameForder),TEXT("%s\\FavoriteGame"),szWorkDirectory);
	if (!PathIsDirectory(szUserGameForder))::CreateDirectory(szUserGameForder, NULL);
	
	//�����ļ�
	TCHAR szUserGameName[MAX_PATH]=TEXT("");
	_sntprintf(szUserGameName,CountArray(szUserGameName),TEXT("%s\\Favorite_%d.INI"),szUserGameForder,pGlobalUserData->dwGameID);

	//д������
	TCHAR szUserSectionName[MAX_PATH]=TEXT("FavoriteGame");
	TCHAR szUserKeyName[MAX_PATH]=TEXT("");
	TCHAR szUserVauleName[MAX_PATH]=TEXT("");
	int nIndex = 0;
	int nKindID = 0;
	for(vector<CGameKindItem *>::const_iterator citer = m_vMyGameKind.begin();citer != m_vMyGameKind.end();citer++,nIndex++)
	{
		CGameKindItem * pGameKindItem = (CGameKindItem *)(*citer);
		nKindID = pGameKindItem->m_GameKind.wKindID;
		_sntprintf(szUserKeyName,CountArray(szUserKeyName),TEXT("Favorite%d"),nIndex);
		_sntprintf(szUserVauleName,CountArray(szUserVauleName),TEXT("%d"),nKindID);
		WritePrivateProfileString(szUserSectionName,szUserKeyName,szUserVauleName,szUserGameName);
	}

	_sntprintf(szUserKeyName,CountArray(szUserKeyName),TEXT("FavoriteCount"));
	_sntprintf(szUserVauleName,CountArray(szUserVauleName),TEXT("%d"),nIndex);
	WritePrivateProfileString(szUserSectionName,szUserKeyName,szUserVauleName,szUserGameName);

	return true;

}

bool CPlazaHallGame::LoadDataFromMemory()
{
	CServerListData * pServerListData=CServerListData::GetInstance();
	m_vGameKind.clear();
	m_vAdGameKind.clear();
	m_vGameServer.clear();

	//��������
	POSITION Position=NULL;
	CGameKindItem * pGameKindItem=NULL;
	CGameServerItem * pGameServerItem=NULL;
	vector<CGameKindItem *> vGameKind;
	vector<CGameKindItem *>::iterator itGameKind;
	vector<CGameServerItem *>::iterator itGameServer;
	//��������
	while (true)
	{
		pGameKindItem=pServerListData->EmunGameKindItem(Position);
		if (pGameKindItem==NULL) break;
		for (itGameKind = m_vGameKind.begin();itGameKind!=m_vGameKind.end();itGameKind++)
		{
			CGameKindItem * pGameKindTemp= (CGameKindItem *)(*itGameKind);
			if (pGameKindItem->m_GameKind.wSortID<=pGameKindTemp->m_GameKind.wSortID)
			{
				m_vGameKind.insert(itGameKind,pGameKindItem);
				break;
			}
		}
		if (itGameKind==m_vGameKind.end()) m_vGameKind.push_back(pGameKindItem);
		if (Position==NULL) break;
	}

	//���ع��
	while (true)
	{
		pGameKindItem=pServerListData->EmunGameKindItem(Position);
		if (pGameKindItem==NULL) break;
		if(pGameKindItem->m_GameKind.wRecommend == 1)
		{
			for (itGameKind = m_vAdGameKind.begin();itGameKind!=m_vAdGameKind.end();itGameKind++)
			{
				CGameKindItem * pGameKindTemp= (CGameKindItem *)(*itGameKind);
				if (pGameKindItem->m_GameKind.wSortID<=pGameKindTemp->m_GameKind.wSortID)
				{
					m_vAdGameKind.insert(itGameKind,pGameKindItem);
					break;
				}
			}
			if (itGameKind==m_vAdGameKind.end()) m_vAdGameKind.push_back(pGameKindItem);
		}
		if (Position==NULL) break;
	}

	//���ط���
	while (true)
	{
		pGameServerItem=pServerListData->EmunGameServerItem(Position);
		if (pGameServerItem==NULL) break;
		for (itGameServer = m_vGameServer.begin();itGameServer!=m_vGameServer.end();itGameServer++)
		{
			CGameServerItem * pGameServerTemp= (CGameServerItem *)(*itGameServer);
			if (pGameServerItem->m_GameServer.wSortID<=pGameServerTemp->m_GameServer.wSortID)
			{
				m_vGameServer.insert(itGameServer,pGameServerItem);
				break;
			}
		}
		if (itGameServer==m_vGameServer.end()) m_vGameServer.push_back(pGameServerItem);
		if (Position==NULL) break;
	}

	return true;
}

bool CPlazaHallGame::LoadDataFromFile()
{
	m_vGameKindID.clear();
	//�û���Ϣ
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	//��ȡĿ¼
	TCHAR szWorkDirectory[MAX_PATH]=TEXT("");
	CWHService::GetWorkDirectory(szWorkDirectory,CountArray(szWorkDirectory));

	//����·��
	TCHAR szUserGameForder[MAX_PATH]=TEXT("");
	_sntprintf(szUserGameForder,CountArray(szUserGameForder),TEXT("%s\\FavoriteGame"),szWorkDirectory);

	//�����ļ�
	TCHAR szUserGameName[MAX_PATH]=TEXT("");
	_sntprintf(szUserGameName,CountArray(szUserGameName),TEXT("%s\\Favorite_%d.INI"),szUserGameForder,pGlobalUserData->dwGameID);

	//·������
	if (!PathIsDirectory(szUserGameForder)) return false;

	//�ļ�����
	CFileFind ff;
	if (!ff.FindFile(szUserGameName)) return false;

	//��ȡ����
	TCHAR szUserSectionName[MAX_PATH]=TEXT("FavoriteGame");
	TCHAR szUserKeyName[MAX_PATH]=TEXT("FavoriteCount");
	TCHAR szUserVauleName[MAX_PATH]=TEXT("");
	UINT nFavoriteCount=GetPrivateProfileInt(szUserSectionName,szUserKeyName,0,szUserGameName);

	//�ղ�����
	if (nFavoriteCount == 0)return false;

	int nTempGameKindID = 0;
	for (UINT i = 0;i<nFavoriteCount;i++)
	{
		_sntprintf(szUserKeyName,CountArray(szUserKeyName),TEXT("Favorite%d"),i);
		nTempGameKindID = GetPrivateProfileInt(szUserSectionName,szUserKeyName,0,szUserGameName);
		m_vGameKindID.push_back(nTempGameKindID);
	}
	return true;
}

bool CPlazaHallGame::FilterGameKind()
{
	m_vMyGameKind.clear();
	vector<CGameKindItem *>::iterator itGameKind;
	vector<int>::iterator itGameKindID;
	for (itGameKind = m_vGameKind.begin();itGameKind!=m_vGameKind.end();itGameKind++)
	{
		CGameKindItem * pGameKindTemp= (CGameKindItem *)(*itGameKind);
		for (itGameKindID = m_vGameKindID.begin();itGameKindID!=m_vGameKindID.end();itGameKindID++)
		{
			int nGameKindID =(int)(*itGameKindID);
			if (nGameKindID == pGameKindTemp->m_GameKind.wKindID)
			{
				m_vMyGameKind.push_back(pGameKindTemp);
			}
		}
	}
	return true;
}

bool CPlazaHallGame::EnterGameKind( CGameKindItem * pGameKindItem )
{
	//�������
	if(!CheckUpdate(pGameKindItem))
	{
		return false;
	}
	
	//���Ҷ���
	CContainerUI * pCnt = (CContainerUI * )GetControlByName(_T("PlazaHallRoom"));
	if(pCnt!=NULL)
	{
		pCnt->SetVisible();
	}
	//���Ҷ���
	pCnt = (CContainerUI * )GetControlByName(_T("PlazaHallGame"));
	if(pCnt!=NULL)
	{
		pCnt->SetVisible(false);
	}

	UpdateRoomUI(pGameKindItem);
	m_pEnterGame = pGameKindItem;

    return true;
}

bool CPlazaHallGame::EnterGameAdKind(CGameKindItem * pGameKindItem)
{
	CPhoneScanCode dlg;
	dlg.LoadDataFromExternal(pGameKindItem);
	dlg.DoModal();
	return true;
}

bool CPlazaHallGame::EnterGameServer( CGameServerItem * pGameServerItem )
{
#ifdef  _DEBUG
	if (pGameServerItem!=NULL) 
	{
		CPlatformFrame * pPlatformFrame=CPlatformFrame::GetInstance();
		return  pPlatformFrame->EntranceServerItem(pGameServerItem);
	}	
#endif	

	CServerListData * pServerListData=CServerListData::GetInstance();
	CGameKindItem * pGameKindItem=pServerListData->SearchGameKind(pGameServerItem->m_GameServer.wKindID);
	if(!CheckUpdate(pGameKindItem))
	{
		return false;
	}


	if(m_pUpdateStatusView == NULL)
	{
		m_pUpdateStatusView = new CUpdateStatusView();
		if(m_pUpdateStatusView==NULL) throw TEXT("�ڴ治��,���󴴽�ʧ��!");			
	}
	if(m_pUpdateStatusView->m_hWnd !=NULL) return false;

	//��������
	TCHAR szGameDirectory[LEN_PROCESS]=TEXT("");
	lstrcpyn(szGameDirectory,pGameServerItem->m_pGameKindItem->m_GameKind.szProcessName,CountArray(szGameDirectory));
	GetGameResDirectory(szGameDirectory,CountArray(szGameDirectory));

	//��ʼ����
	CGlobalUnits::GetInstance()->SetTrackServerID(pGameServerItem->m_GameServer.wServerID);
	if(m_pUpdateStatusView->m_hWnd==NULL)
        m_pUpdateStatusView->Create(IDD_DLG_POPUP,NULL);
	m_pUpdateStatusView->ShowWindow(SW_SHOW);
	m_pUpdateStatusView->InitUpdateStatus(this,szGameDirectory);

	//CPlatformFrame * pPlatformFrame=CPlatformFrame::GetInstance();

	//if (pGameServerItem!=NULL) pPlatformFrame->EntranceServerItem(pGameServerItem);

    return true;
}

VOID CPlazaHallGame::UpdateRoomUI(CGameKindItem * pGameKindItem)
{
	//��Ϸ����
	CControlUI * pGameName = (GetControlByName(szLabelGameNameControlName));
	if (pGameName!=NULL)
	{
		pGameName->SetText(pGameKindItem->m_GameKind.szKindName);
	}

	//��������
	TCHAR szProcessName[LEN_PROCESS]=TEXT("");
	GetProcessName(pGameKindItem,szProcessName);

	CStdString	csImageKindName;
	csImageKindName.Format(TEXT("file=\'GAME_KIND_TITLE\\GameName_%s.png\' restype=\'FILE\'"),szProcessName);

	//��Ϸ����
	CControlUI * pGameTitle = (GetControlByName(szLabelGameTileControlName));
	if (pGameTitle!=NULL)
	{
		pGameTitle->SetBkImage(csImageKindName);
	}

	//���Ҷ���
	CPageLayoutUI * pRoom = (CPageLayoutUI * )(GetControlByName(szHallRoomMainControlName));
	if (pRoom != NULL)
	{
		pRoom->RemoveAll();
		pRoom->SetCurrPage(0);

		//���ر���Ԫ��
		for (vector<CGameServerItem *>::iterator iter1 = m_vGameServer.begin();iter1!=m_vGameServer.end();iter1++)
		{
			CGameServerItem * pGameServer = (CGameServerItem *)(*iter1);
			if (pGameServer->m_GameServer.wKindID == pGameKindItem->m_GameKind.wKindID)
			{
				CDialogBuilder builder;
				CContainerUI *pNewContainerItem = NULL;
				//�Ƿ����
				if (!pGameServer->IsMatchRoom())
				{
					pNewContainerItem = static_cast<CContainerUI*>(builder.Create( _T("PlazaHallRoomItem.xml"), (UINT)0, NULL, m_pPaintManager));
					CStdString	csImageBkSelect;
					CStdString	csImageBkType;
					
					//���ַ���
					if(pGameServer->m_GameServer.wServerType == GAME_GENRE_SCORE)
					{
						csImageBkSelect.Format(TEXT("PLAZA_HALL_ROOM\\BUTTON_ROOM_SELECT1.png"));
						csImageBkType.Format(TEXT("PLAZA_HALL_ROOM\\TYPE_SCORE.png"));
					}
					//��ҷ���
					else if(pGameServer->m_GameServer.wServerType == GAME_GENRE_GOLD)
					{
						csImageBkSelect.Format(TEXT("PLAZA_HALL_ROOM\\BUTTON_ROOM_SELECT2.png"));
						csImageBkType.Format(TEXT("PLAZA_HALL_ROOM\\TYPE_MONEY.png"));
					}
					//ѵ������
					else if(pGameServer->m_GameServer.wServerType == GAME_GENRE_EDUCATE)
					{
						csImageBkSelect.Format(TEXT("PLAZA_HALL_ROOM\\BUTTON_ROOM_SELECT7.png"));
						csImageBkType.Format(TEXT("PLAZA_HALL_ROOM\\TYPE_EDUCATE.png"));
					}
					//�����׷���
					CControlUI *pCheat = pNewContainerItem->FindControl(szItemRoomTypeCheatControlName);
					if (pCheat!=NULL)
					{
						if(CServerRule::IsAllowAvertCheatMode(pGameServer->m_GameServer.dwServerRule))
						{
							csImageBkSelect.Format(TEXT("PLAZA_HALL_ROOM\\BUTTON_ROOM_SELECT5.png"));
							pCheat->SetVisible();
						}
						else
						{
							pCheat->SetVisible(false);
						}

					}					
					CButtonUI *pSelect = (CButtonUI *)(pNewContainerItem->FindControl(szItemRoomSelectControlName));
					if (pSelect!=NULL)
					{
						pSelect->SetStatusImage(csImageBkSelect);
						pSelect->SetTag((UINT_PTR)pGameServer);
					}
					CControlUI *pName = pNewContainerItem->FindControl(szItemRoomNameControlName);
					if (pName!=NULL)
					{
						pName->SetText(pGameServer->m_GameServer.szServerName);
					}
					//���뷿��
					CControlUI *pPassWord = pNewContainerItem->FindControl(szItemRoomTypePassWordControlName);
					if (pPassWord!=NULL)
					{
						if((pGameServer->m_GameServer.wServerKind&SERVER_GENRE_PASSWD)!=0)
						{
							pPassWord->SetVisible();
						}
						else
						{
							pPassWord->SetVisible(false);
						}
					}
					

					//��������
					CControlUI *pType = pNewContainerItem->FindControl(szItemRoomTypeControlName);
					if (pType!=NULL)
					{
						pType->SetBkImage(csImageBkType);
					}
					//����׷�
					CControlUI *pBase = pNewContainerItem->FindControl(szItemRoomBaseScoreControlName);
					if (pBase!=NULL)
					{
						CStdString	csRoomBaseScore;
						csRoomBaseScore.Format(TEXT("%d"),pGameServer->m_GameServer.lCellScore);
						pBase->SetText(csRoomBaseScore);
					}
					//��������
					CControlUI *pLimitFree = pNewContainerItem->FindControl(szItemRoomLimitFreeControlName);
					if (pLimitFree!=NULL)
					{
						pLimitFree->SetVisible(pGameServer->m_GameServer.lEnterScore == 0);
					}
					CControlUI *pLimitScore = pNewContainerItem->FindControl(szItemRoomLimitScoreControlName);
					if (pLimitScore!=NULL)
					{
						CStdString	csRoomEnterScore;
						csRoomEnterScore.Format(TEXT("%d"),pGameServer->m_GameServer.lEnterScore);
						pLimitScore->SetText(csRoomEnterScore);
						pLimitScore->SetVisible(pGameServer->m_GameServer.lEnterScore > 0);
					}
					//����״̬
					CControlUI *pStateFree = pNewContainerItem->FindControl(szItemRoomStateFreeControlName);
					if (pStateFree!=NULL)
					{
						pStateFree->SetVisible(pGameServer->m_GameServer.dwOnLineCount < pGameServer->m_GameServer.dwFullCount * 30/100);
					}
					CControlUI *pStateBusy = pNewContainerItem->FindControl(szItemRoomStateBusyControlName);
					if (pStateBusy!=NULL)
					{
						pStateBusy->SetVisible(pGameServer->m_GameServer.dwOnLineCount >= pGameServer->m_GameServer.dwFullCount* 70/100);
					}
					CControlUI *pStateHot = pNewContainerItem->FindControl(szItemRoomStateHotControlName);
					if (pStateHot!=NULL)
					{
						pStateHot->SetVisible(pGameServer->m_GameServer.dwOnLineCount >= pGameServer->m_GameServer.dwFullCount * 30/100);
					}
					
					//�����û�
					CControlUI *pUserAll = pNewContainerItem->FindControl(szItemRoomUserAllControlName);
					if (pUserAll!=NULL)
					{
						if(pGameServer->m_GameServer.cbEnterMember ==0)
						{
							pUserAll->SetVisible(true);
						}
						else
						{
							pUserAll->SetVisible(false);
						}
					}
					CControlUI *pUserVip = pNewContainerItem->FindControl(szItemRoomUserVipControlName);
					if (pUserVip!=NULL)
					{
						bool bVip = false;
						if(pGameServer->m_GameServer.cbEnterMember >0)
						{
							bVip = true;
						}
						pUserVip->SetVisible(bVip);
						if (bVip)
						{
							CControlUI *pUserVip = pNewContainerItem->FindControl(szItemRoomUserVipControlName);
							if (pUserVip != NULL)
							{
								for (int i=1;i<6;i++)
								{
									CStdString strLevel;
									strLevel.SmallFormat(TEXT("%s%d"),szItemRoomVipLevelControlName,i);
									CControlUI * pLabelUI = pNewContainerItem->FindControl(strLevel);
									if(pLabelUI)
									{
										if (i==pGameServer->m_GameServer.cbEnterMember)
										{
											pLabelUI->SetVisible(true);
										}
										else
										{
											pLabelUI->SetVisible(false);
										}
									}
								}

							}
						}
						
					}

				}
				else
				{

					pNewContainerItem = static_cast<CContainerUI*>(builder.Create( _T("PlazaHallRoomItemMatch.xml"), (UINT)0, NULL, m_pPaintManager));
					CControlUI *pName = pNewContainerItem->FindControl(szItemRoomMatchNameControlName);
					if (pName!=NULL)
					{
						pName->SetText(pGameServer->m_GameMatch.szMatchName);
					}
					
					CButtonUI *pSelect = (CButtonUI *)(pNewContainerItem->FindControl(szItemRoomMatchSelectControlName));
					if (pSelect!=NULL)
					{
						pSelect->SetTag((UINT_PTR)pGameServer);
					}

					//���뷿��
					CControlUI *pPassWord = pNewContainerItem->FindControl(szItemRoomTypePassWordControlName);
					if (pPassWord!=NULL)
					{
						if((pGameServer->m_GameServer.wServerKind&SERVER_GENRE_PASSWD)!=0)
						{
							pPassWord->SetVisible();
						}
						else
						{
							pPassWord->SetVisible(false);
						}
					}

					//��������
					CControlUI *pSignFree = pNewContainerItem->FindControl(szItemRoomMatchSignFreeControlName);
					if (pSignFree!=NULL)
					{
						pSignFree->SetVisible(pGameServer->m_GameMatch.lSignupFee == 0);

					}
					CControlUI *pSignScore = pNewContainerItem->FindControl(szItemRoomMatchSignScoreControlName);
					if (pSignScore!=NULL)
					{
						pSignScore->SetVisible(pGameServer->m_GameMatch.lSignupFee>0);
						TCHAR szMatchFee[32]=TEXT("");
						_sntprintf(szMatchFee,CountArray(szMatchFee),SCORE_STRING TEXT("%s"),pGameServer->m_GameMatch.lSignupFee,pGameServer->m_GameMatch.cbFeeType==FEE_TYPE_GOLD?TEXT("��Ϸ��"):TEXT("Ԫ��"));

						CStdString	csMatchScore;
						csMatchScore.Format(TEXT("%s"),szMatchFee);
						pSignScore->SetText(csMatchScore);
					}

					//�����û�
					CControlUI *pUserAll = pNewContainerItem->FindControl(szItemRoomUserAllControlName);
					if (pUserAll!=NULL)
					{
						if(pGameServer->m_GameServer.cbEnterMember ==0)
						{
							pUserAll->SetVisible(true);
						}
						else
						{
							pUserAll->SetVisible(false);
						}
					}
					//��Ա�ȼ�
					CControlUI *pUserVip = pNewContainerItem->FindControl(szItemRoomUserVipControlName);
					if (pUserVip!=NULL)
					{
						bool bVip = false;
						if(pGameServer->m_GameServer.cbEnterMember >0)
						{
							bVip = true;
						}
						pUserVip->SetVisible(bVip);
						if (bVip)
						{
							CControlUI *pUserVip = pNewContainerItem->FindControl(szItemRoomUserVipControlName);
							if (pUserVip != NULL)
							{
								for (int i=1;i<6;i++)
								{
									CStdString strLevel;
									strLevel.SmallFormat(TEXT("%s%d"),szItemRoomVipLevelControlName,i);
									CControlUI * pLabelUI = pNewContainerItem->FindControl(strLevel);
									if(pLabelUI)
									{
										if (i==pGameServer->m_GameServer.cbEnterMember)
										{
											pLabelUI->SetVisible(true);
										}
										else
										{
											pLabelUI->SetVisible(false);
										}
									}
								}

							}
						}

					}				

					//��ʱ����
					if (pGameServer->m_GameMatch.cbMatchType==MATCH_TYPE_IMMEDIATE)
					{
						//��ȡ��ʱ��
						tagImmediateMatch * pImmediateMatch=(tagImmediateMatch *)pGameServer->m_GameMatch.cbMatchRule;

						//��������
						CStdString	csImageBkType;
						csImageBkType.Format(TEXT("PLAZA_HALL_ROOM\\TYPE_MATCH_IMMEDIATE.png"));

						CControlUI *pType = pNewContainerItem->FindControl(szItemRoomMatchTypeControlName);
						if (pType!=NULL)
						{
							pType->SetBkImage(csImageBkType);
						}
						//��������
						CControlUI *pMatchRuleName = pNewContainerItem->FindControl(szItemRoomMatchRuleNameControlName);
						if (pMatchRuleName!=NULL)
						{
							CStdString	csMatchRule;
							csMatchRule.Format(TEXT("��ʱ����"));
							pMatchRuleName->SetText(csMatchRule);
						}
						CControlUI *RuleContext = pNewContainerItem->FindControl(szItemRoomMatchRuleContextControlName);
						if (RuleContext!=NULL)
						{
							TCHAR szStartUserCount[32]=TEXT("");
							_sntprintf(szStartUserCount,CountArray(szStartUserCount),TEXT("����%d��"),pImmediateMatch->wStartUserCount);
							CStdString	csMatchRule;
							csMatchRule.Format(TEXT("%s"),szStartUserCount);
							RuleContext->SetText(csMatchRule);
						}
						//��������
						CControlUI *pTime = pNewContainerItem->FindControl(szItemRoomMatchTimeControlName);
						if (pTime!=NULL)
						{
							pTime->SetVisible(false);
						}
						CControlUI *pCondtion = pNewContainerItem->FindControl(szItemRoomMatchConditionControlName);
						if (pCondtion!=NULL)
						{
							pCondtion->SetVisible();
						}

					}
					//��ʱ����
					else if(pGameServer->m_GameMatch.cbMatchType==MATCH_TYPE_LOCKTIME)
					{
						//��ȡ��ʱ��
						tagLockTimeMatch * pLockTimeMatch=(tagLockTimeMatch *)pGameServer->m_GameMatch.cbMatchRule;

						//��������
						CStdString	csImageBkType;
						csImageBkType.Format(TEXT("PLAZA_HALL_ROOM\\TYPE_MATCH_TIME.png"));
						
						CControlUI *pType = pNewContainerItem->FindControl(szItemRoomMatchTypeControlName);
						if (pType!=NULL)
						{
							pType->SetBkImage(csImageBkType);
						}

						//��������
						CControlUI *pMatchRuleName = pNewContainerItem->FindControl(szItemRoomMatchRuleNameControlName);
						if (pMatchRuleName!=NULL)
						{
							CStdString	csMatchRule;
							csMatchRule.Format(TEXT("��ʱ����"));
							pMatchRuleName->SetText(csMatchRule);
						}
						CControlUI *RuleContext = pNewContainerItem->FindControl(szItemRoomMatchRuleContextControlName);
						if (RuleContext!=NULL)
						{
							//��������
							CTime CurrTime=CTime::GetCurrentTime();
							DWORD dwCurrInterval = CurrTime.GetHour()*3600+CurrTime.GetMinute()*60+CurrTime.GetSecond();
							DWORD dwEndInterval = pLockTimeMatch->MatchEndTime.wHour*3600+pLockTimeMatch->MatchEndTime.wMinute*60+pLockTimeMatch->MatchEndTime.wSecond;

							//����ʱ��
							TCHAR szStartTime[32]=TEXT("");
							_sntprintf(szStartTime,CountArray(szStartTime),TEXT("%02d:%02d:%02d"),pLockTimeMatch->MatchStartTime.wHour,pLockTimeMatch->MatchStartTime.wMinute,pLockTimeMatch->MatchStartTime.wSecond);

							//����ʱ��
							TCHAR szEndTime[32]=TEXT("");
							_sntprintf(szEndTime,CountArray(szEndTime),TEXT("%02d:%02d:%02d"),pLockTimeMatch->MatchEndTime.wHour,pLockTimeMatch->MatchEndTime.wMinute,pLockTimeMatch->MatchEndTime.wSecond);

							CStdString	csMatchRule;
							csMatchRule.Format(TEXT("%s-%s"),szStartTime,szEndTime);
							RuleContext->SetText(csMatchRule);

							//����ʱ��
							CControlUI *pTime = pNewContainerItem->FindControl(szItemRoomMatchTimeControlName);
							if (pTime!=NULL)
							{
								pTime->SetVisible();
							}
							CControlUI *pCondtion = pNewContainerItem->FindControl(szItemRoomMatchConditionControlName);
							if (pCondtion!=NULL)
							{
								pCondtion->SetVisible(false);
							}
						}
					}
				}

				pRoom->Add(pNewContainerItem);
			}


			
		}

	}

}

VOID CPlazaHallGame::UpdateDeleteUI()
{
	CCheckButtonUI* pCheck = (CCheckButtonUI*)GetControlByName(szButtonEditControlName);
	for (vector<CControlUI *>::iterator it1=m_vMyGameDelete.begin();it1!=m_vMyGameDelete.end();it1++)
	{
		CControlUI *pDelete= (*it1);
		if (pDelete!=NULL)
		{
			pDelete->SetVisible(pCheck->GetCheck());

			CContainerUI *pNewContainerItem = (CContainerUI *)pDelete->GetParent();
			if (pNewContainerItem !=NULL)
			{
				CContainerUI *pDown = (CContainerUI *)(pNewContainerItem->FindControl(szItemGameDownControlName));
				if (pDown!=NULL)
				{
					if(pDown->IsVisible())
					{
						pDelete->SetVisible(false);
					}
				}
			}
		}
	}
}

bool CPlazaHallGame::AddGameKind()
{
	CDlgAddGame dlg;
	dlg.LoadDataFromExternal(m_vMyGameKind);
	if (IDOK == dlg.DoModal())
	{
		dlg.UpdateDataToExternal(m_vMyGameKind);
		SaveGameKind();
		UpdateUI();
	}
	return true;
}

bool CPlazaHallGame::DeleteGameKind(CGameKindItem * pGameKindItem)
{
	vector<CGameKindItem *>::iterator itemFinder=find(m_vMyGameKind.begin(),m_vMyGameKind.end(),pGameKindItem);
	if (itemFinder!= m_vMyGameKind.end())
	{
		m_vMyGameKind.erase(itemFinder);
	}
	//ˢ��UI
	UpdateUI();

	SaveGameKind();

	return true;
}

VOID CPlazaHallGame::CloseDeleteState()
{
	CCheckButtonUI* pCheck = (CCheckButtonUI*)GetControlByName(szButtonEditControlName);
	if (pCheck !=NULL)
	{
		pCheck->SetCheck(false);
	}
	
	UpdateDeleteUI();
}

VOID CPlazaHallGame::ReturnHallMain()
{
	//���Ҷ���
	CContainerUI * pCnt = (CContainerUI * )GetControlByName(_T("PlazaHallGame"));
	if(pCnt!=NULL)
	{
		pCnt->SetVisible();
	}
	//���Ҷ���
	pCnt = (CContainerUI * )GetControlByName(_T("PlazaHallRoom"));
	if(pCnt!=NULL)
	{
		pCnt->SetVisible(false);
	}
}

//��ԴĿ¼
VOID CPlazaHallGame::GetGameResDirectory(TCHAR szResDirectory[], WORD wBufferCount)
{
	//��������
	WORD wStringIndex=0;

	//����Ŀ¼
	while ((szResDirectory[wStringIndex]!=0)&&(szResDirectory[wStringIndex]!=TEXT('.'))) wStringIndex++;

	//�ַ���ֹ
	szResDirectory[wStringIndex]=0;

	return;
}

//////////////////////////////////////////////////////////////////////////////////


