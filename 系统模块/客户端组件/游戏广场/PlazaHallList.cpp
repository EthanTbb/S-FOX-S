#include "StdAfx.h"
#include "PlatformFrame.h"
#include "PlazaHallList.h"
#include "PlatformEvent.h"

//////////////////////////////////////////////////////////////////////////////////
//�ؼ�����
const TCHAR* const szButtonCollapseAllControlName = TEXT("ButtonCollapseAll");
const TCHAR* const szButtonExpandAllControlName = TEXT("ButtonExpandAll");
const TCHAR* const szButtonAllGameNumControlName = TEXT("ButtonAllGameNum");

//��ť�ؼ�
const TCHAR* const szHallGameListControlName = TEXT("HallGameList");
//////////////////////////////////////////////////////////////////////////////////
//���캯��
CPlazaHallList::CPlazaHallList(CPaintManagerUI*	pPaintManager):m_pPaintManager(pPaintManager)
{
}

//��������
CPlazaHallList::~CPlazaHallList()
{
}

//��ʼ�ؼ�
bool CPlazaHallList::InitControlUI()
{
	return true;
}

//��Ϣ����
bool CPlazaHallList::Notify(TNotifyUI &  msg)
{
	//��ȡ����
	CControlUI * pControlUI = msg.pSender;

	//����¼�
	if (lstrcmp(msg.sType, TEXT("click")) == 0)
	{
		//�����Ϸ
		if(lstrcmp(pControlUI->GetName(), szButtonAllGameNumControlName)==0) 
		{
			OnNotifyClickAddGame(msg);
		}
		//չ������
		if(lstrcmp(pControlUI->GetName(), szButtonExpandAllControlName)==0) 
		{
			OnNotifyClickExpandAll(msg);
		}
		if(lstrcmp(pControlUI->GetName(), szButtonCollapseAllControlName)==0) 
		{
			OnNotifyClickCollapseAll(msg);
		}
	}
	else if (lstrcmp(msg.sType, TEXT("itemExpand")) == 0)
	{
		if (_tcsicmp(msg.pSender->GetClass(), _T("NodeElementUI")) == 0)
		{
			OnNotifyItemExpand(msg);
		}

	}
	else if (lstrcmp(msg.sType, TEXT("itemCollapse")) == 0)
	{
		if (_tcsicmp(msg.pSender->GetClass(), _T("NodeElementUI")) == 0)
		{
			OnNotifyItemCollapse(msg);
		}

	}
	else if ( (lstrcmp(msg.sType, TEXT("itemclick")) == 0))
	{
		if (_tcsicmp(msg.pSender->GetClass(), _T("NodeElementUI")) == 0)
		{
			OnNotifyListClickNode(msg);
		}
	}
	else if ((lstrcmp(msg.sType, TEXT("itemactivate")) == 0))
	{
		if (_tcsicmp(msg.pSender->GetClass(), _T("NodeElementUI")) == 0)
		{
			OnNotifyListActivateNode(msg);
		}
	}
	return false;
}

CControlUI * CPlazaHallList::GetControlByName( LPCTSTR pszControlName )
{
	return m_pPaintManager->FindControl(pszControlName);
}

void CPlazaHallList::OnNotifyClickAddGame(TNotifyUI & msg)
{
	SendMessage(WM_ITEM_ADD_GAME,0,0);
}

void CPlazaHallList::OnNotifyClickExpandAll(TNotifyUI & msg)
{
	CListExUI * pGameList = (CListExUI * )(GetControlByName(szHallGameListControlName));
	if(pGameList != NULL)
	{
		CNodeElementUI	*pNodeElement= (CNodeElementUI	*)pGameList->GetRoot();	
		if (pNodeElement !=NULL)
		{
			for( int i = 0; i < pNodeElement->num_children(); ++i )
			{
				CControlUI* control = pNodeElement->child(i);
				if( _tcscmp(control->GetClass(), _T("NodeElementUI")) == 0 )
				{
					CNodeElementUI	*pElement = (CNodeElementUI	*)control;
					if (pElement->data()._level == 0 )
					{
						pGameList->ExpandItem(pElement);
					}		
				}			
			}
		}
	}
}

void CPlazaHallList::OnNotifyClickCollapseAll(TNotifyUI & msg)
{
	CListExUI * pGameList = (CListExUI * )(GetControlByName(szHallGameListControlName));
	if(pGameList != NULL)
	{
		CNodeElementUI	*pNodeElement= (CNodeElementUI	*)pGameList->GetRoot();
		if (pNodeElement !=NULL)
		{
			for( int i = 0; i < pNodeElement->num_children(); ++i )
			{
				CControlUI* control = pNodeElement->child(i);
				if( _tcscmp(control->GetClass(), _T("NodeElementUI")) == 0 )
				{
					CNodeElementUI	*pElement = (CNodeElementUI	*)control;
					if (pElement->data()._level == 0 )
					{
						pGameList->CollapseItem(pElement);
					}		
				}			
			}
		}

	}
}

void CPlazaHallList::OnNotifyItemExpand(TNotifyUI & msg)
{
	CControlUI * pControlUI = msg.pSender;
	CNodeElementUI	*pElementMe= (CNodeElementUI*)pControlUI;

	bool bShow = true;
	CListExUI * pGameList = (CListExUI * )(GetControlByName(szHallGameListControlName));
	if(pGameList != NULL)
	{
		CNodeElementUI	*pNodeElement= (CNodeElementUI	*)pGameList->GetRoot();
		if (pNodeElement !=NULL)
		{
			for( int i = 0; i < pNodeElement->num_children(); ++i )
			{
				CControlUI* control = pNodeElement->child(i);
				if( _tcscmp(control->GetClass(), _T("NodeElementUI")) == 0 )
				{
					CNodeElementUI	*pElement = (CNodeElementUI	*)control;
					if (pElement != pElementMe)
					{
						if (pElement->data()._level == 0 )
						{
							if(!pElement->IsExpanded())
							{
								bShow = false;
								break;
							}
						}	
					}
	
				}
			}
		}
	}
	if(bShow)
	{
		CControlUI * pCollapse = (GetControlByName(szButtonCollapseAllControlName));
		if(pCollapse != NULL)
		{
			pCollapse->SetVisible();
		}
		CControlUI * pExpand = (GetControlByName(szButtonExpandAllControlName));
		if(pExpand != NULL)
		{
			pExpand->SetVisible(false);
		}
	}
}

void CPlazaHallList::OnNotifyItemCollapse(TNotifyUI & msg)
{
	CControlUI * pControlUI = msg.pSender;
	CNodeElementUI	*pElementMe= (CNodeElementUI*)pControlUI;

	bool bShow = true;
	CListExUI * pGameList = (CListExUI * )(GetControlByName(szHallGameListControlName));
	if(pGameList != NULL)
	{
		CNodeElementUI	*pNodeElement= (CNodeElementUI	*)pGameList->GetRoot();
		if (pNodeElement !=NULL)
		{
			for( int i = 0; i < pNodeElement->num_children(); ++i )
			{
				CControlUI* control = pNodeElement->child(i);
				if( _tcscmp(control->GetClass(), _T("NodeElementUI")) == 0 )
				{
					CNodeElementUI	*pElement = (CNodeElementUI	*)control;
					if (pElement != pElementMe)
					{
						if (pElement->data()._level == 0 )
						{
							if(pElement->IsExpanded())
							{
								bShow = false;
								break;
							}
						}	
					}
				}
			}
		}
	}
	if(bShow)
	{
		CControlUI * pCollapse = (GetControlByName(szButtonCollapseAllControlName));
		if(pCollapse != NULL)
		{
			pCollapse->SetVisible(false);
		}
		CControlUI * pExpand = (GetControlByName(szButtonExpandAllControlName));
		if(pExpand != NULL)
		{
			pExpand->SetVisible();
		}
	}
}

void CPlazaHallList::OnNotifyListClickNode(TNotifyUI & msg)
{
	CControlUI * pControlUI = msg.pSender;
	CNodeElementUI	*pElement= (CNodeElementUI	*)pControlUI;
	if(pElement->data()._level == 0)
	{
		pElement->Expand(!pElement->IsExpanded());
	}
	else if(pElement->data()._level == 1)
	{
		pElement->Expand(!pElement->IsExpanded());
	}
	else if(pElement->data()._level == 2)
	{
		CGameServerItem * pGameServerItem = (CGameServerItem *)pControlUI->GetTag();
		SendMessage(WM_ITEM_ENTER_SERVER,(WPARAM)pGameServerItem,0);
	}
}

void CPlazaHallList::OnNotifyListActivateNode(TNotifyUI & msg)
{
	CControlUI * pControlUI = msg.pSender;
	CNodeElementUI	*pElement= (CNodeElementUI	*)pControlUI;
	if(pElement->data()._level == 0)
	{
		/*pElement->Expand(!pElement->IsExpanded());*/
	}
	else if(pElement->data()._level == 1)
	{
		CGameKindItem * pGameKind = (CGameKindItem *)(pElement->GetTag());
		SendMessage(WM_ITEM_QUERY_UPDATE,(WPARAM)pGameKind,0);
	}
	else if(pElement->data()._level == 2)
	{
		CGameServerItem * pGameServerItem = (CGameServerItem *)pControlUI->GetTag();
		SendMessage(WM_ITEM_ENTER_SERVER,(WPARAM)pGameServerItem,0);
	}
}

void CPlazaHallList::ShowPlazaHallList()
{
	LoadDataFromMemory();
	InitUI();
	CreateUI();
	UpdateUI();

	//��ȡ����
	TCHAR szPath[MAX_PATH]=TEXT("");
	CWHService::GetWorkDirectory(szPath, sizeof(szPath));
	TCHAR szFileName[MAX_PATH]=TEXT("");
	_sntprintf(szFileName,CountArray(szFileName),TEXT("%s\\PlazaConfig.INI"),szPath);

	TCHAR	szExpandAll[MAX_PATH];
	ZeroMemory(szExpandAll,CountArray(szExpandAll));
	GetPrivateProfileString(TEXT("ExpandAll"),TEXT("DisEnable"),TEXT(""),szExpandAll,CountArray(szExpandAll),szFileName);
	int nExpandAll = _ttoi(szExpandAll);
	if (nExpandAll==0)
	{
		TNotifyUI msg;
		OnNotifyClickExpandAll(msg);
	}
}

void CPlazaHallList::LoadDataFromMemory()
{
	CServerListData * pServerListData=CServerListData::GetInstance();
	m_vGameType.clear();
	m_vGameKind.clear();
	m_vGameServer.clear();

	//��������
	POSITION Position=NULL;
	CGameTypeItem   * pGameTypeItem=NULL;
	CGameKindItem   * pGameKindItem=NULL;
	CGameServerItem * pGameServerItem=NULL;
	vector<CGameTypeItem *>::iterator itGameType;
	vector<CGameKindItem *>::iterator itGameKind;
	vector<CGameServerItem *>::iterator itGameServer;
	//��������
	while (true)
	{
		pGameTypeItem=pServerListData->EmunGameTypeItem(Position);
		if (pGameTypeItem==NULL) break;
		for (itGameType = m_vGameType.begin();itGameType!=m_vGameType.end();itGameType++)
		{
			CGameTypeItem * pGameTypeTemp= (CGameTypeItem *)(*itGameType);
			if (pGameTypeItem->m_GameType.wSortID<=pGameTypeTemp->m_GameType.wSortID)
			{
				m_vGameType.insert(itGameType,pGameTypeItem);
				break;
			}
		}
		if (itGameType==m_vGameType.end()) m_vGameType.push_back(pGameTypeItem);
		if (Position==NULL) break;
	}

	//������Ϸ
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


}

void CPlazaHallList::InitUI()
{
	//��ʼչ��
	CControlUI * pExpand = (GetControlByName(szButtonExpandAllControlName));
	if(pExpand != NULL)
	{
		pExpand->SetVisible();
	}
	//��ʼ����
	CControlUI * pCollapse = (GetControlByName(szButtonCollapseAllControlName));
	if(pCollapse != NULL)
	{
		pCollapse->SetVisible(false);
	}
	//��ʼ��Ϸ�б�
	CListExUI * pGameList = (CListExUI * )(GetControlByName(szHallGameListControlName));
	if(pGameList != NULL)
	{
		pGameList->RemoveAll();
	}
	m_vGameDown.clear();
}

void CPlazaHallList::CreateUI()
{
	//���Ҷ���
	CListExUI * pGameList = (CListExUI * )(GetControlByName(szHallGameListControlName));
	if(pGameList != NULL)
	{
		CDialogBuilder builder;
		//����һ��Ԫ��
		vector<CNodeElementUI*> vItme0;
		for (vector<CGameTypeItem *>::iterator iter=m_vGameType.begin();iter!=m_vGameType.end();iter++)
		{
			CGameTypeItem * pGameTypeItem=(*iter);
			CNodeElementUI* itme0 =NULL;
			itme0 = static_cast<CNodeElementUI*>(builder.Create( _T("PlazaHallListItemType.xml"), (UINT)0, (UINT)0, m_pPaintManager));
			if (itme0 !=NULL)
			{
				pGameList->AddNode(itme0,NULL);
				itme0->SetTag((UINT_PTR)pGameTypeItem);
				vItme0.push_back(itme0);
			}
		}

		//��������Ԫ��
		vector<CNodeElementUI*> vItme1;
		for (vector<CNodeElementUI *>::iterator iter=vItme0.begin();iter!=vItme0.end();iter++)
		{
			CNodeElementUI * pNodeElement=(*iter);
			CGameTypeItem * pGameTypeItem = (CGameTypeItem *)(pNodeElement->GetTag());
			for (vector<CGameKindItem*>::iterator iter2 = m_vGameKind.begin();iter2!=m_vGameKind.end();iter2++)
			{
				CGameKindItem * pGameKind = (CGameKindItem *)(*iter2);
				if (pGameTypeItem->m_GameType.wTypeID == pGameKind->m_GameKind.wTypeID)
				{
					CNodeElementUI* itme1 =NULL;
					itme1 = static_cast<CNodeElementUI*>(builder.Create( _T("PlazaHallListItemKind.xml"), (UINT)0, (UINT)0, m_pPaintManager));
					if (itme1 !=NULL)
					{
						pGameList->AddNode(itme1,pNodeElement);
						itme1->SetTag((UINT_PTR)pGameKind);
						vItme1.push_back(itme1);
					}
				}
			}

		}

		//��������Ԫ��
		vector<CNodeElementUI*> vItme2;
		for (vector<CNodeElementUI *>::iterator iter=vItme1.begin();iter!=vItme1.end();iter++)
		{
			CNodeElementUI * pNodeElement=(*iter);
			CGameKindItem * pGameKind = (CGameKindItem *)(pNodeElement->GetTag());
			for (vector<CGameServerItem*>::iterator iter2 = m_vGameServer.begin();iter2!=m_vGameServer.end();iter2++)
			{
				CGameServerItem * pGameServer = (CGameServerItem *)(*iter2);
				if ( pGameKind->m_GameKind.wKindID == pGameServer->m_GameServer.wKindID)
				{
					CNodeElementUI* itme2 =NULL;
					itme2 = static_cast<CNodeElementUI*>(builder.Create( _T("PlazaHallListItemServer.xml"), (UINT)0, (UINT)0, m_pPaintManager));
					if (itme2 !=NULL)
					{
						pGameList->AddNode(itme2,pNodeElement);
						itme2->SetTag((UINT_PTR)pGameServer);
						vItme2.push_back(itme2);
					}
				}
			}
		}

		//��ʼһ��Ԫ��
		for (vector<CNodeElementUI *>::iterator iter=vItme0.begin();iter!=vItme0.end();iter++)
		{
			CNodeElementUI * itme0=(*iter);
			CGameTypeItem * pGameTypeItem = (CGameTypeItem *)(itme0->GetTag());

			//��������
			CControlUI * pControl=NULL;
			pControl =	itme0->FindControl(_TEXT("ExpNormalGameTypeName"));
			if(pControl != NULL) pControl->SetText(pGameTypeItem->m_GameType.szTypeName);
			pControl =	itme0->FindControl(_TEXT("ExpHotGameTypeName"));
			if(pControl != NULL) pControl->SetText(pGameTypeItem->m_GameType.szTypeName);
			pControl =	itme0->FindControl(_TEXT("ColNormalGameTypeName"));
			if(pControl != NULL) pControl->SetText(pGameTypeItem->m_GameType.szTypeName);
			pControl =	itme0->FindControl(_TEXT("ColHotGameTypeName"));
			if(pControl != NULL) pControl->SetText(pGameTypeItem->m_GameType.szTypeName);

			//��������
			int nGameTypeNum = 0 ;
			for( int i = 0; i < itme0->num_children(); ++i )
			{
				CControlUI* control = itme0->child(i);
				if( _tcscmp(control->GetClass(), _T("NodeElementUI")) == 0 )
				{
					CNodeElementUI	*pElement = (CNodeElementUI	*)control;
					if (pElement->data()._level == 1 )
					{
						nGameTypeNum++;
					}		
				}			
			}
			TCHAR szTypeGameNum[64]=TEXT("");
			_sntprintf(szTypeGameNum,CountArray(szTypeGameNum),TEXT("��%d��"),nGameTypeNum);
			pControl =	itme0->FindControl(_TEXT("ExpNormalGameTypeNum"));
			if(pControl != NULL) pControl->SetText(szTypeGameNum);
			pControl =	itme0->FindControl(_TEXT("ExpHotGameTypeNum"));
			if(pControl != NULL) pControl->SetText(szTypeGameNum);
			pControl =	itme0->FindControl(_TEXT("ColNormalGameTypeNum"));
			if(pControl != NULL) pControl->SetText(szTypeGameNum);
			pControl =	itme0->FindControl(_TEXT("ColHotGameTypeNum"));
			if(pControl != NULL) pControl->SetText(szTypeGameNum);

		}

		//��ʼ����Ԫ��
		for (vector<CNodeElementUI *>::iterator iter=vItme1.begin();iter!=vItme1.end();iter++)
		{
			CNodeElementUI * itme1=(*iter);
			CGameKindItem * pGameKind = (CGameKindItem *)(itme1->GetTag());

			//����ͼ��
			UpdateItemIco(itme1, pGameKind);

			//��������
			CControlUI * pControl=NULL;
			pControl =	itme1->FindControl(_TEXT("GameKindName"));
			if(pControl != NULL) pControl->SetText(pGameKind->m_GameKind.szKindName);

			//���ñ�־
			pControl =	itme1->FindControl(_TEXT("GameKindFlagNew"));
			if(pControl != NULL) pControl->SetVisible((pGameKind->m_GameKind.wGameFlag & 0x1) == 0x1);
			pControl =	itme1->FindControl(_TEXT("GameKindFlagRecommed"));
			if(pControl != NULL) pControl->SetVisible((pGameKind->m_GameKind.wGameFlag & 0x2) == 0x2);
			pControl =	itme1->FindControl(_TEXT("GameKindFlagHot"));
			if(pControl != NULL) pControl->SetVisible((pGameKind->m_GameKind.wGameFlag & 0x4) == 0x4);
			pControl =	itme1->FindControl(_TEXT("GameKindFlagMatch"));
			if(pControl != NULL) pControl->SetVisible((pGameKind->m_GameKind.wGameFlag & 0x8) == 0x8);

			//��������
			CContainerUI *pDown = (CContainerUI *)(itme1->FindControl(_TEXT("GameKindDown")));
			if (pDown!=NULL)
			{
				pDown->SetTag((UINT_PTR) pGameKind);
				pDown->SetVisible(false);
				m_vGameDown.push_back(pDown);
			}
		}

		//��ʼ����Ԫ��
		for (vector<CNodeElementUI *>::iterator iter=vItme2.begin();iter!=vItme2.end();iter++)
		{
			CNodeElementUI * itme2=(*iter);
			CGameServerItem * pGameServer = (CGameServerItem *)(itme2->GetTag());

			//��������
			CControlUI * pControl=NULL;
			pControl =	itme2->FindControl(_TEXT("GameServerName"));
			if(pControl != NULL) pControl->SetText(pGameServer->m_GameServer.szServerName);

			//��������
			TCHAR szServerGameLimit[64]=TEXT("");
			_sntprintf(szServerGameLimit,CountArray(szServerGameLimit),TEXT("%d��������"),pGameServer->m_GameServer.lEnterScore);
			pControl =	itme2->FindControl(_TEXT("GameServerLimit"));
			if(pControl != NULL) pControl->SetText(szServerGameLimit);

		}
	}
}

void CPlazaHallList::UpdateUI()
{
	//��ȡ����
	CPlatformFrame * pPlatrformFrame = CPlatformFrame::GetInstance();

	//��Ϸ����
	CControlUI * pGameNum = (GetControlByName(szButtonAllGameNumControlName));
	if (pGameNum!=NULL)
	{
		CStdString	csGameNum;
		csGameNum.Format(TEXT("%d"),m_vGameKind.size());
		pGameNum->SetText(csGameNum);
	}
}

bool CPlazaHallList::OnMessagePlatformEvent( WPARAM wParam, LPARAM lParam )
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
		}
	}
	return 0L;
}

void CPlazaHallList::UpdateDownUI( CPlazaHallGameDownLoadSink * pDownLoadSink )
{
	//����״̬
	for (vector<CContainerUI*>::iterator iter1 = m_vGameDown.begin();iter1!=m_vGameDown.end();iter1++)
	{
		CContainerUI * pDwon = (CContainerUI *)(*iter1);
		if (pDwon!=NULL)
		{
			CGameKindItem * pGameKind = (CGameKindItem *)(pDwon->GetTag());
			if (pGameKind->m_GameKind.wKindID == pDownLoadSink->m_wKindID)
			{
				//�������
				DWORD dwDownScale=0L;
				if (pDownLoadSink->m_lTotalFileSize>0L)
				{
					dwDownScale=DWORD(pDownLoadSink->m_lDownLoadSize*100L/pDownLoadSink->m_lTotalFileSize);
				}
				CProgressUI *pProgress = (CProgressUI *)pDwon->FindControl(_T("GameKindProgress"));
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

						CListExUI * pGameList = (CListExUI * )(GetControlByName(szHallGameListControlName));
						if(pGameList != NULL)
						{
							CNodeElementUI	*pNodeElement= (CNodeElementUI	*)pGameList->GetRoot();
							if (pNodeElement !=NULL)
							{
								for( int i = 0; i < pNodeElement->num_children(); ++i )
								{
									CControlUI* control = pNodeElement->child(i);
									if( _tcscmp(control->GetClass(), _T("NodeElementUI")) == 0 )
									{
										CNodeElementUI	*pElement = (CNodeElementUI	*)control;
										if (pElement->data()._level == 0 )
										{
											for (int j = 0; j< pElement->num_children();++j)
											{
												CControlUI* control2 = pElement->child(j);
												if( _tcscmp(control2->GetClass(), _T("NodeElementUI")) == 0 )
												{
													CNodeElementUI	*pElement2 = (CNodeElementUI	*)control2;
													if (pElement2->data()._level == 1 )
													{
														CGameKindItem * pNodeGameKind = (CGameKindItem *)(pElement2->GetTag());
														if (pNodeGameKind->m_GameKind.wKindID == pGameKind->m_GameKind.wKindID)
														{
															UpdateItemIco(pElement2, pGameKind);
														}
													}
												}
											}

										}		
									}			
								}
							}

						}
						break;
					}
				}
				pDwon->SetVisible(bShow);
			}
		}
	}
}

void CPlazaHallList::UpdateItemIco(CContainerUI* pContainerUI, CGameKindItem* pGameKindItem)
{
	if(pContainerUI == NULL) return;
	if(pGameKindItem == NULL) return;
	
	CControlUI* pControl =	pContainerUI->FindControl(_TEXT("GameKindIco"));
	if(pControl==NULL) return;

	CStdString sProcessName;
	sProcessName.Format(TEXT("..\\%s"), pGameKindItem->m_GameKind.szProcessName);
	CStdString sFile = CPaintManagerUI::GetResourcePath() + sProcessName;
	//�жϴ���
	if( GetFileAttributes(sFile.GetData())!=0xFFFFFFFF )
	{
		CStdString strKindIco;
		strKindIco.Format(TEXT("file='..\\%s' mask='#FFFF00FF' restype='FILE'"), pGameKindItem->m_GameKind.szProcessName);
		pControl->SetBkImage(strKindIco);
	}
	else
	{
		pControl->SetBkImage(TEXT("file='PLAZA_HALL_LIST\\unDownload.png' "));
	}
}

bool CPlazaHallList::SendMessage( UINT message, WPARAM wParam , LPARAM lParam )
{
	//�����¼�
	CPlatformEvent * pPlatformEvent=CPlatformEvent::GetInstance();
	if (pPlatformEvent!=NULL) pPlatformEvent->SendPlatformEvent(message,(LPARAM)wParam);
	return true;
}
//////////////////////////////////////////////////////////////////////////////////