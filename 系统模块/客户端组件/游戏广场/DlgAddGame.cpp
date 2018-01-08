#include "Stdafx.h"
#include "Resource.h"
#include "DlgAddGame.h"
#include <algorithm>
//////////////////////////////////////////////////////////////////////////////////
//按钮控件
const TCHAR* const szButtonCloseControlName = TEXT("ButtonClose");
const TCHAR* const szButtonComfirmControlName = TEXT("ButtonComfirm");
const TCHAR* const szButtonAllSelectControlName = TEXT("ButtonAllSelect");
const TCHAR* const szButtonCancleControlName = TEXT("ButtonCancle");

//容器控件
const TCHAR* const szHLayoutOptionAddGameControlName = TEXT("HLayoutOptionAddGame");
const TCHAR* const szTabLayoutAddGameControlName = TEXT("TabLayoutAddGame");

//子项控件
const TCHAR* const szItemGameIOCControlName = TEXT("ItemGameIOC");
const TCHAR* const szItemGameNameControlName = TEXT("ItemGameName");
const TCHAR* const szItemGameCheckControlName = TEXT("ItemGameCheck");

//标签控件
const TCHAR* const szLabelAllGameNumControlName = TEXT("LabelAllGameNum");
const TCHAR* const szLabelMyGameNumControlName = TEXT("LabelMyGameNum");

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CDlgAddGame::CDlgAddGame() : CFGuiDialog(IDD_DLG_POPUP)
{
}

//析构函数
CDlgAddGame::~CDlgAddGame()
{
}

//创建函数
BOOL CDlgAddGame::OnInitDialog()
{
	__super::OnInitDialog();

	//设置标题
	SetWindowText(TEXT("添加游戏"));

	//居中窗口
	CenterWindow(this);

	LoadDataFromMemory();

	CreateUI();

	UpdateUI();

	return TRUE;
}

//确定函数
VOID CDlgAddGame::OnOK()
{
	LoadDataFromUI();
	return __super::OnOK();
}

//取消消息
VOID CDlgAddGame::OnCancel()
{
	return __super::OnCancel();
}

//初始控件
VOID CDlgAddGame::InitControlUI()
{

}

//消息提醒
void CDlgAddGame::Notify(TNotifyUI &  msg)
{
	//获取对象
	CControlUI * pControlUI = msg.pSender;
	//点击事件
	if (lstrcmp(msg.sType, TEXT("click")) == 0)
	{
		if (lstrcmp(pControlUI->GetName(), szButtonCloseControlName) == 0 || lstrcmp(pControlUI->GetName(), szButtonCancleControlName) == 0)
		{
			return OnCancel();
		}
		if (lstrcmp(pControlUI->GetName(), szButtonComfirmControlName) == 0)
		{
			return OnOK();
		}
		else if (lstrcmp(pControlUI->GetName(), szButtonAllSelectControlName) == 0)
		{
			return OnNotifyAllSelect(msg);
		}
		else if (lstrcmp(pControlUI->GetName(), szItemGameCheckControlName) == 0)
		{
			return OnNotifySelect(msg);
		}
	}
	
	return;
}

LPCTSTR CDlgAddGame::GetSkinFile() 
{ 
	return TEXT("DlgAddGame.xml"); 
} 


void CDlgAddGame::OnNotifyAllSelect(TNotifyUI & msg)
{
	CTabLayoutUI * pTabLayout = (CTabLayoutUI * )(GetControlByName(szTabLayoutAddGameControlName));
	if (pTabLayout != NULL)
	{
		int nCurSel = pTabLayout->GetCurSel();
		if (nCurSel ==0)
		{
			for (vector<VECheckButtonList>::iterator it1 = m_vecCheckbutton.begin();it1!=m_vecCheckbutton.end();it1++)
			{
				VECheckButtonList vecCheckList = *it1;
				for (VECheckButtonList::iterator it2 = vecCheckList.begin();it2!=vecCheckList.end();it2++)
				{
					CCheckButtonUI* pCheckButton      = (*it2);
					pCheckButton->SetCheck(true);
				}
			}

			for (vector<CCheckButtonUI*>::iterator it3 = m_vecAllGameCheckList.begin();it3!=m_vecAllGameCheckList.end();it3++)
			{
				CCheckButtonUI* pCheckButton = (*it3);
				pCheckButton->SetCheck(true);
			}
		}
		else
		{
			CContainerUI *pNewContainer = (CContainerUI *)(pTabLayout->GetItemAt(nCurSel));
			if (pNewContainer !=NULL)
			{
				CGameTypeItem * pGameTypeItem=(CGameTypeItem *)(pNewContainer->GetTag());
				for (vector<VECheckButtonList>::iterator it1 = m_vecCheckbutton.begin();it1!=m_vecCheckbutton.end();it1++)
				{
					VECheckButtonList vecCheckList = *it1;
					for (VECheckButtonList::iterator it2 = vecCheckList.begin();it2!=vecCheckList.end();it2++)
					{
						CCheckButtonUI* pCheckButton      = (*it2);
						CGameKindItem * pGameKindItem     = (CGameKindItem*)pCheckButton->GetTag();
						if (pGameKindItem->m_GameKind.wTypeID == pGameTypeItem->m_GameType.wTypeID)
						{
							pCheckButton->SetCheck(true);
						}

					}
				}
				for (vector<CCheckButtonUI*>::iterator it3 = m_vecAllGameCheckList.begin();it3!=m_vecAllGameCheckList.end();it3++)
				{
					CCheckButtonUI* pCheckButton = (*it3);
					CGameKindItem *pGameKindItem = (CGameKindItem*)pCheckButton->GetTag();
					if(pGameKindItem->m_GameKind.wTypeID == pGameTypeItem->m_GameType.wTypeID)
					{
						pCheckButton->SetCheck(true);
					}
				}
			}
		}
	}
}

void CDlgAddGame::OnNotifySelect(TNotifyUI & msg)
{
	//获取对象
	CControlUI * pControlUI = msg.pSender;

	//同时选中
	for (vector<VECheckButtonList>::iterator it1 = m_vecCheckbutton.begin();it1!=m_vecCheckbutton.end();it1++)
	{
		VECheckButtonList vecCheckList = *it1;
		for (VECheckButtonList::iterator it2 = vecCheckList.begin();it2!=vecCheckList.end();it2++)
		{
			CCheckButtonUI* pCheckButton = (*it2);
			CGameKindItem *pGameKindItem = (CGameKindItem*)pCheckButton->GetTag();

			CCheckButtonUI* pCurrCheckButton = (CCheckButtonUI*)pControlUI;
			CGameKindItem *pCurrGameKindItem = (CGameKindItem*)pCurrCheckButton->GetTag();
			if(pCurrGameKindItem->m_GameKind.wKindID == pGameKindItem->m_GameKind.wKindID)
			{
				pCheckButton->SetCheck(pCurrCheckButton->GetCheck());
			}
		}
	}

	for (vector<CCheckButtonUI*>::iterator it3 = m_vecAllGameCheckList.begin();it3!=m_vecAllGameCheckList.end();it3++)
	{
		CCheckButtonUI* pCheckButton = (*it3);
		CGameKindItem *pGameKindItem = (CGameKindItem*)pCheckButton->GetTag();

		CCheckButtonUI* pCurrCheckButton = (CCheckButtonUI*)pControlUI;
		CGameKindItem *pCurrGameKindItem = (CGameKindItem*)pCurrCheckButton->GetTag();
		if(pCurrGameKindItem->m_GameKind.wKindID == pGameKindItem->m_GameKind.wKindID)
		{
			pCheckButton->SetCheck(pCurrCheckButton->GetCheck());
		}
	}
}

VOID CDlgAddGame::LoadDataFromExternal(vector<CGameKindItem *> & vMyGameKind)
{
	m_vMyGameKind.clear();
	m_vMyGameKind.insert(m_vMyGameKind.begin(),vMyGameKind.begin(),vMyGameKind.end());
}

VOID CDlgAddGame::UpdateDataToExternal(vector<CGameKindItem *> & vMyGameKind)
{
	vMyGameKind.clear();
	vMyGameKind.insert(vMyGameKind.begin(),m_vMyGameKind.begin(),m_vMyGameKind.end());
}


VOID CDlgAddGame::LoadDataFromMemory()
{
	//获取对象
	ASSERT(CServerListData::GetInstance()!=NULL);
	CServerListData * pServerListData=CServerListData::GetInstance();

	m_vGameType.clear();
	m_vGameKind.clear();

	//变量定义
	POSITION Position=NULL;
	CGameTypeItem   * pGameTypeItem=NULL;
	CGameKindItem   * pGameKindItem=NULL;
	CGameServerItem * pGameServerItem=NULL;
	vector<CGameTypeItem *>::iterator itGameType;
	vector<CGameKindItem *>::iterator itGameKind;

	//加载类型
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

	//加载游戏
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

}

VOID CDlgAddGame::CreateUI()
{
	//创建选项Option
	CContainerUI * pHLayoutOption = (CContainerUI * )(GetControlByName(szHLayoutOptionAddGameControlName));
	if (pHLayoutOption != NULL)
	{
		//创建全部游戏
		CDialogBuilder Allbuilder;
		COptionUI * pAllOption = NULL;
		pAllOption = static_cast<COptionUI*>(Allbuilder.Create( _T("DlgAddGameItemOption.xml"), (UINT)0));
		pHLayoutOption->Add(pAllOption);

		//创建类别游戏
		int nTabIndex = 1;
		for (vector<CGameTypeItem *>::iterator iter=m_vGameType.begin();iter!=m_vGameType.end();iter++)
		{
			CGameTypeItem * pGameTypeItem=(*iter);
			CDialogBuilder builder;
			COptionUI * pNewOption = NULL;
			pNewOption = static_cast<COptionUI*>(builder.Create( _T("DlgAddGameItemOption.xml"), (UINT)0));
			pNewOption->SetText(pGameTypeItem->m_GameType.szTypeName);
			pNewOption->Selected(false);
			pNewOption->BindTabIndex(nTabIndex++);
			pHLayoutOption->Add(pNewOption);
		}
	}

	//创建页面Tab
	CTabLayoutUI * pTabLayout = (CTabLayoutUI * )(GetControlByName(szTabLayoutAddGameControlName));
	if (pTabLayout != NULL)
	{
		//创建全部游戏
		CDialogBuilder Allbuilder;
		CContainerUI *pAllContainer = NULL;
		pAllContainer = static_cast<CContainerUI*>(Allbuilder.Create( _T("DlgAddGameItemPage.xml"), (UINT)0, NULL, &m_PaintManager));
		pTabLayout->Add(pAllContainer);

		//创建类别游戏
		vector<CContainerUI *> vTabLayoutContainer;
		for (vector<CGameTypeItem *>::iterator iter=m_vGameType.begin();iter!=m_vGameType.end();iter++)
		{
			CGameTypeItem * pGameTypeItem=(*iter);
			CDialogBuilder builder;
			CContainerUI *pNewContainer = NULL;
			pNewContainer = static_cast<CContainerUI*>(builder.Create( _T("DlgAddGameItemPage.xml"), (UINT)0, NULL, &m_PaintManager));
			pNewContainer->SetTag((UINT_PTR)pGameTypeItem);
			pTabLayout->Add(pNewContainer);
			vTabLayoutContainer.push_back(pNewContainer);
		}

		//创建类别游戏图标
		for (vector<CContainerUI *>::iterator iter=vTabLayoutContainer.begin();iter!=vTabLayoutContainer.end();iter++)
		{
			CContainerUI *pNewContainer = (*iter);
			CGameTypeItem * pGameTypeItem=(CGameTypeItem *)(pNewContainer->GetTag());
			//保存单类别选择
			vector<CCheckButtonUI*> vecCheckList;
			for (vector<CGameKindItem*>::iterator iter2 = m_vGameKind.begin();iter2!=m_vGameKind.end();iter2++)
			{
				CGameKindItem * pGameKind = (CGameKindItem *)(*iter2);
				if (pGameKind->m_GameKind.wTypeID == pGameTypeItem->m_GameType.wTypeID)
				{
					CDialogBuilder builder2;
					CContainerUI *pNewGameICOItem = NULL;
					pNewGameICOItem = static_cast<CContainerUI*>(builder2.Create( _T("DlgAddGameItemPageItem.xml"), (UINT)0, NULL, &m_PaintManager));

					//设置图标
					TCHAR szProcessName[LEN_PROCESS]=TEXT("");
					GetProcessName(pGameKind,szProcessName);
					CStdString	csImageKind;
					csImageKind.Format(TEXT("file=\'GAME_KIND\\GameKind_%s.png\' restype=\'FILE\'"),szProcessName);
					if(m_PaintManager.GetImageEx(csImageKind)==NULL) csImageKind.Empty();
					CStdString	m_ImageUnSure=TEXT("file=\'GAME_KIND\\KIND_UNSURE.png\' restype=\'FILE\'");
					if(csImageKind.IsEmpty()==true) csImageKind = m_ImageUnSure;
					CControlUI *pIco = pNewGameICOItem->FindControl(szItemGameIOCControlName);
					if (pIco!=NULL)
					{
						pIco->SetBkImage(csImageKind);
					}

					//设置名字
					CControlUI *pName = pNewGameICOItem->FindControl(szItemGameNameControlName);
					if (pName!=NULL)
					{
						pName->SetText(pGameKind->m_GameKind.szKindName);
					}

					//设置勾选
					CCheckButtonUI *pCheck = (CCheckButtonUI *)pNewGameICOItem->FindControl(szItemGameCheckControlName);
					if (pCheck!=NULL)
					{
						pCheck->SetTag((UINT_PTR)pGameKind);
						vector<CGameKindItem *>::iterator itemFinder=find(m_vMyGameKind.begin(),m_vMyGameKind.end(),pGameKind);
						if (itemFinder!= m_vMyGameKind.end())
						{
							pCheck->SetCheck(true);
						}
						vecCheckList.push_back(pCheck);
					}
					pNewContainer->Add(pNewGameICOItem);
				}

			}

			m_vecCheckbutton.push_back(vecCheckList);
		}

		//创建全部游戏图标
		for (vector<CGameKindItem*>::iterator iter2 = m_vGameKind.begin();iter2!=m_vGameKind.end();iter2++)
		{
			CGameKindItem * pGameKind = (CGameKindItem *)(*iter2);

			CDialogBuilder builder2;
			CContainerUI *pNewContainerItem = NULL;
			pNewContainerItem = static_cast<CContainerUI*>(builder2.Create( _T("DlgAddGameItemPageItem.xml"), (UINT)0, NULL, &m_PaintManager));

			//设置图标
			TCHAR szProcessName[LEN_PROCESS]=TEXT("");
			GetProcessName(pGameKind,szProcessName);
			CStdString	csImageKind;
			csImageKind.Format(TEXT("GAME_KIND\\GameKind_%s.png"),szProcessName);
			if(m_PaintManager.GetImageEx(csImageKind,NULL)==NULL) csImageKind.Empty();
			CStdString	m_ImageUnSure=TEXT("file=\'GAME_KIND\\KIND_UNSURE.png\' restype=\'FILE\'");
			if(csImageKind.IsEmpty()==true) csImageKind = m_ImageUnSure;
			CControlUI *pIco = pNewContainerItem->FindControl(szItemGameIOCControlName);
			if (pIco!=NULL)
			{
				pIco->SetBkImage(csImageKind);
			}

			//设置名字
			CControlUI *pName = pNewContainerItem->FindControl(szItemGameNameControlName);
			if (pName!=NULL)
			{
				pName->SetText(pGameKind->m_GameKind.szKindName);
			}

			//设置勾选
			CCheckButtonUI *pCheck = (CCheckButtonUI *)pNewContainerItem->FindControl(szItemGameCheckControlName);
			if (pCheck!=NULL)
			{
				pCheck->SetTag((UINT_PTR)pGameKind);
				vector<CGameKindItem *>::iterator itemFinder=find(m_vMyGameKind.begin(),m_vMyGameKind.end(),pGameKind);
				if (itemFinder!= m_vMyGameKind.end())
				{
					pCheck->SetCheck(true);
				}
				m_vecAllGameCheckList.push_back(pCheck);
			}

			pAllContainer->Add(pNewContainerItem);

		}


	}
}

VOID CDlgAddGame::LoadDataFromUI()
{
	m_vMyGameKind.clear();

	for (vector<CCheckButtonUI*>::iterator it1 = m_vecAllGameCheckList.begin();it1!=m_vecAllGameCheckList.end();it1++)
	{
		CCheckButtonUI* pCheckButton = (*it1);
		if (pCheckButton->GetCheck())
		{
			CGameKindItem* pGameKindItem = (CGameKindItem*)(pCheckButton->GetTag());
			m_vMyGameKind.push_back(pGameKindItem);
		}
	}
}


VOID CDlgAddGame::UpdateUI()
{


	CControlUI *pAllGameNum = GetControlByName(szLabelAllGameNumControlName);
	if (pAllGameNum!=NULL)
	{
		CStdString	csNum;
		csNum.Format(TEXT("%d款"),m_vGameKind.size());
		pAllGameNum->SetText(csNum);
	}

	CControlUI *pMyGameNum = GetControlByName(szLabelMyGameNumControlName);
	if (pMyGameNum!=NULL)
	{
		CStdString	csNum;
		csNum.Format(TEXT("%d款"),m_vMyGameKind.size());
		pMyGameNum->SetText(csNum);
	}

}

void CDlgAddGame::GetProcessName(CGameKindItem *vMyGameKind, TCHAR szProcessName[LEN_PROCESS] )
{
	//指针校验
	if(vMyGameKind==NULL) return;

	//变量定义
	lstrcpyn(szProcessName,vMyGameKind->m_GameKind.szProcessName,sizeof(TCHAR)*LEN_PROCESS);

	//变量定义
	LPTSTR pszProcess = (_tcsrchr(szProcessName,TEXT('.'))); 

	//设置字符
	pszProcess[0]=TEXT('\0');

	return;

}
//////////////////////////////////////////////////////////////////////////////////