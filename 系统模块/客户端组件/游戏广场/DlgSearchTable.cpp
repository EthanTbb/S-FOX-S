#include "StdAfx.h"
#include "PlazaViewServer.h"
#include "DlgSearchTable.h"
#include "DlgInformation.h"
//////////////////////////////////////////////////////////////////////////
//控件定义

//按钮控件
const TCHAR * szButtonSearchControlName = TEXT("ButtonSearch");
const TCHAR * szButtonQuickJoinControlName = TEXT("ButtonQuickJoin");
const TCHAR * szButtonCloseControlName1=TEXT("ButtonClose");
const TCHAR * szButtonCancelControlName1=TEXT("ButtonCancel");
//扩展控件
const TCHAR * szCheckOption1ControlName = TEXT("CheckOption1");
const TCHAR * szCheckOption2ControlName = TEXT("CheckOption2");
const TCHAR * szCheckOption3ControlName = TEXT("CheckOption3");
const TCHAR * szCheckOption4ControlName = TEXT("CheckOption4");
const TCHAR * szCheckOption5ControlName = TEXT("CheckOption5");

//////////////////////////////////////////////////////////////////////////

//构造函数
CDlgSearchTable::CDlgSearchTable() : CFGuiDialog(IDD_DLG_POPUP)
{
	//接口变量
	m_pITableViewFrame=NULL;
	m_pServerViewItem=NULL;

	//设置变量
	m_wResultTableID=INVALID_TABLE;

	return;
}

//析构函数
CDlgSearchTable::~CDlgSearchTable()
{
}

//配置函数
BOOL CDlgSearchTable::OnInitDialog()
{
	__super::OnInitDialog();

	////设置变量
	//m_wResultTableID=INVALID_TABLE;

	//设置标题
	SetWindowText(TEXT("查找游戏桌"));

	//设置窗口
	CSize SizeWindow(m_PaintManager.GetInitSize());
	SetWindowPos(NULL, 0, 0, SizeWindow.cx, SizeWindow.cy, SWP_NOZORDER|SWP_NOMOVE);	

	return FALSE;
}

//消息解释
BOOL CDlgSearchTable::PreTranslateMessage(MSG * pMsg)
{
	//按键过滤
	if ((pMsg->message==WM_KEYDOWN)&&(pMsg->wParam==VK_ESCAPE))
	{
		return TRUE;
	}

	return __super::PreTranslateMessage(pMsg);
}

//确定函数
VOID CDlgSearchTable::OnOK()
{
	//变量定义
	tagSearchTable SearchTable;
	ZeroMemory(&SearchTable,sizeof(SearchTable));

	//变量定义
	CCheckButtonUI * pCheckOption=NULL;

	//复选选项
	pCheckOption = static_cast<CCheckButtonUI*>(GetControlByName(szCheckOption1ControlName));
	if(pCheckOption!=NULL) SearchTable.bOneNull=pCheckOption->GetCheck();

	//复选选项
	pCheckOption = static_cast<CCheckButtonUI*>(GetControlByName(szCheckOption2ControlName));
	if(pCheckOption!=NULL) SearchTable.bTwoNull=pCheckOption->GetCheck();

	//复选选项
	pCheckOption = static_cast<CCheckButtonUI*>(GetControlByName(szCheckOption3ControlName));
	if(pCheckOption!=NULL) SearchTable.bNotFull=pCheckOption->GetCheck();

	//复选选项
	pCheckOption = static_cast<CCheckButtonUI*>(GetControlByName(szCheckOption4ControlName));
	if(pCheckOption!=NULL) SearchTable.bAllNull=pCheckOption->GetCheck();

	//复选选项
	pCheckOption = static_cast<CCheckButtonUI*>(GetControlByName(szCheckOption5ControlName));
	if(pCheckOption!=NULL) SearchTable.bFilterPass=pCheckOption->GetCheck();

	//复选选项
	CControlUI * pControlAutoJoin = GetControlByName(szButtonQuickJoinControlName);
	if(pControlAutoJoin==NULL) return;

	//搜索变量
	SearchTable.wResultTableID=INVALID_TABLE;
	SearchTable.wResultChairID=INVALID_CHAIR;
	SearchTable.wStartTableID=m_wResultTableID+1;

	//查找桌子
	ASSERT(m_pServerViewItem!=NULL);
	m_pServerViewItem->SearchGameTable(SearchTable);

	//设置结果
	m_wResultTableID=SearchTable.wResultTableID;

	//结果判断
	if (SearchTable.wResultTableID==INVALID_TABLE)
	{
		//复选选项
		pControlAutoJoin->SetEnabled(false);

		//提示消息
		CDlgInformation Information(this);
		Information.ShowMessageBox(TEXT("抱歉，暂时找不到符合您要求的游戏桌，请调整查找条件或稍后再试！"),MB_ICONINFORMATION,30);

		return;
	}

	//设置界面
	pControlAutoJoin->SetEnabled(m_wResultTableID!=INVALID_TABLE);
	m_pITableViewFrame->VisibleTable(SearchTable.wResultTableID);
	m_pITableViewFrame->FlashGameChair(SearchTable.wResultTableID, SearchTable.wResultChairID);

	return; 
}

//取消函数
VOID CDlgSearchTable::OnCancel() 
{
	//关闭窗口
	DestroyWindow();

	return; 
}

//皮肤函数
LPCTSTR CDlgSearchTable::GetSkinFile()
{
	return TEXT("DlgSearchTable.xml");
}

//初始控件
VOID CDlgSearchTable::InitControlUI()
{
	__super::InitControlUI();

	//复选选项
	CCheckButtonUI * pCheckOption = static_cast<CCheckButtonUI*>(GetControlByName(szCheckOption1ControlName));
	if(pCheckOption!=NULL) pCheckOption->SetCheck(true);

	//复选选项
	pCheckOption = static_cast<CCheckButtonUI*>(GetControlByName(szCheckOption2ControlName));
	if(pCheckOption!=NULL) pCheckOption->SetCheck(true);

	//复选选项
	pCheckOption = static_cast<CCheckButtonUI*>(GetControlByName(szCheckOption3ControlName));
	if(pCheckOption!=NULL) pCheckOption->SetCheck(true);

	//复选选项
	pCheckOption = static_cast<CCheckButtonUI*>(GetControlByName(szCheckOption4ControlName));
	if(pCheckOption!=NULL) pCheckOption->SetCheck(false);

	//复选选项
	pCheckOption = static_cast<CCheckButtonUI*>(GetControlByName(szCheckOption5ControlName));
	if(pCheckOption!=NULL) pCheckOption->SetCheck(true);

	CControlUI * pControlAutoJoin = GetControlByName(szButtonQuickJoinControlName);
	if(pControlAutoJoin!=NULL) pControlAutoJoin->SetEnabled(false);
}

//消息提醒
VOID CDlgSearchTable::Notify(TNotifyUI &  msg)
{
	__super::Notify(msg);

	//获取对象
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


//设置房间
VOID CDlgSearchTable::SetServerViewItem(CPlazaViewServer * pServerViewItem)
{
	//设置变量
	m_pServerViewItem=pServerViewItem;

	return;
}

//设置桌子
VOID CDlgSearchTable::SetTableViewFrame(ITableViewFrame * pITableViewFrame)
{
	//设置镜像
	ASSERT(pITableViewFrame!=NULL);
	m_pITableViewFrame=pITableViewFrame;

	return;
}

//快速加入
VOID CDlgSearchTable::OnBnClickedAutoJoin()
{
	//效验状态
	ASSERT(m_wResultTableID!=INVALID_TABLE);
	if (m_wResultTableID==INVALID_TABLE) return;

	//加入桌子
	WORD wChairID=INVALID_CHAIR;
	m_pITableViewFrame->GetNullChairCount(m_wResultTableID,wChairID);

	//百人游戏
	if(m_pITableViewFrame->GetChairCount() >= INDEX_ENTER_CHAIR)
		wChairID = INDEX_ENTER_CHAIR;

	//加入判断
	if (wChairID==INVALID_CHAIR)
	{
		//提示消息
		CDlgInformation Information(this);
		Information.ShowMessageBox(TEXT("此游戏桌已经满人了，无法加入此游戏桌，请重新搜索后再次尝试！"),MB_ICONINFORMATION,30);

		return;
	}

	//加入游戏
	ASSERT(m_pServerViewItem!=NULL);
	m_pServerViewItem->OnLButtonHitTable(m_wResultTableID,wChairID);

	return;
}

//////////////////////////////////////////////////////////////////////////
