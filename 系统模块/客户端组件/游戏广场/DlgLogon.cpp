#include "Stdafx.h"
#include "Resource.h"
#include "GamePlaza.h"
#include "MissionLogon.h"
#include "DlgLogon.h"
#include "DlgProxyConfig.h"
#include "DlgAddGame.h"
#include "DlgHelp.h"
#include "DLGMBCard.h"
#include "DlgInformation.h"
//////////////////////////////////////////////////////////////////////////////////
//响应控件
const TCHAR* const szButtonCloseControlName			 = TEXT("ButtonClose");
const TCHAR* const szButtonMinControlName			 = TEXT("ButtonMin");
const TCHAR* const szButtonLogonControlName			 = TEXT("ButtonLogon");
const TCHAR* const szButtonRegisterControlName		 = TEXT("ButtonRegister");
const TCHAR* const szButtonForgetPasswdControlName	 = TEXT("ButtonForgetPasswd");
const TCHAR* const szButtonWebLink1ControlName		 = TEXT("ButtonWebLink1");
const TCHAR* const szButtonWebLink2ControlName		 = TEXT("ButtonWebLink2");
const TCHAR* const szButtonWebLink3ControlName		 = TEXT("ButtonWebLink3");
const TCHAR* const szButtonWebLink4ControlName		 = TEXT("ButtonWebLink4");
const TCHAR* const szWebBrowserLogonAdControlName	 = TEXT("WebBrowserLogonAd");
const TCHAR* const szComboServerControlName			 = TEXT("ComboServer");
const TCHAR* const szComboAccountsControlName		 = TEXT("ComboAccounts");
const TCHAR* const szEditAccountsControlName		 = TEXT("EditAccounts");
const TCHAR* const szEditPasswordControlName		 = TEXT("EditPassword");
const TCHAR* const szLabelTitleControlName			 = TEXT("LabelTitle");
const TCHAR* const szLabelLogoControlName			 = TEXT("LabelLogo");
const TCHAR* const szCheckButtonPasswdControlName	 = TEXT("CheckButtonPasswd");
//////////////////////////////////////////////////////////////////////////////////

//构造函数
CDlgLogon::CDlgLogon() : CFGuiDialog(IDD_DLG_POPUP)
{
	//登录信息
	m_dwGameID=0;
	m_szAccounts[0]=0;
	m_szPassword[0]=0;
	m_szLogonServer[0]=0;
	m_szPassPortID[0]=0;

	//配置变量
	m_cbRemPassword=FALSE;
	m_cbLogonMode=LOGON_BY_ACCOUNTS;

	//控件变量
	m_pDlgAccedit=NULL;

	return;
}

//析构函数
CDlgLogon::~CDlgLogon()
{
	DestroyWindow();

	//删除对象
	for (INT_PTR i=0;i<m_AccountsInfoArray.GetCount();i++)
	{
		SafeDelete(m_AccountsInfoArray[i]);
	}

	if(m_pDlgAccedit) m_pDlgAccedit->EndDialog(IDOK);

	return;
}


//创建函数
BOOL CDlgLogon::OnInitDialog()
{
	__super::OnInitDialog();

	//设置窗口
	ModifyStyle(0, WS_MINIMIZEBOX);

	//设置标题
	SetWindowText(TEXT("用户登录"));

	//设置图标
	SetIcon(LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME)),FALSE);

	////居中窗口
	//CenterWindow(this);

	//配置信息
	LoadAccountsInfo();
	LoadLogonServerInfo();
	return TRUE;
}

//消息解释
BOOL CDlgLogon::PreTranslateMessage(MSG * pMsg)
{
	//按键过虑
	if ((pMsg->message==WM_KEYDOWN)&&(pMsg->wParam==VK_ESCAPE))
	{
		return TRUE;
	}

	return __super::PreTranslateMessage(pMsg);
}

//确定函数
VOID CDlgLogon::OnOK()
{
	//获取信息
	if (GetInformation()==false) return;

	//隐藏窗口
	ShowWindow(SW_HIDE);

	//执行登录
	CMissionLogon * pMissionLogon=CMissionLogon::GetInstance();
	if (pMissionLogon!=NULL) pMissionLogon->PerformLogonMission(m_cbRemPassword==TRUE);
	DestroyWindow();

	return;
}

//取消消息
VOID CDlgLogon::OnCancel()
{
	//关闭窗口
	EndDialog(IDCANCEL);
	DestroyWindow();
	AfxGetMainWnd()->PostMessage(WM_CLOSE,0,0);
	return;
}

LPCTSTR CDlgLogon::GetSkinFile()
{
	return TEXT("DlgLogon.xml");
}

//初始控件
void CDlgLogon::InitControlUI()
{
	//查找对象
	CEditUI * pEditPassword = (CEditUI * )GetControlByName(szEditPasswordControlName);
	if(pEditPassword!=NULL)
	{
		pEditPassword->SetPasswordMode(true);
		pEditPassword->SetMaxChar(LEN_PASSWORD-1);
	}
	
	CEditComboUI * pComboServer= (CEditComboUI * )GetControlByName(szComboServerControlName);
	if(pComboServer!=NULL)
	{
		pComboServer->SetBkColor(0x00000000);
		pComboServer->SetItemBkColor(0x00000000);
		pComboServer->SetHotItemBkColor(0x000000);
		pComboServer->SetSelectedItemBkColor(0x000000);
	}

	CEditComboUI * pComboAccounts= (CEditComboUI * )GetControlByName(szComboAccountsControlName);
	if(pComboAccounts != NULL)
	{
		pComboAccounts->SetBkColor(0x00000000);
		pComboAccounts->SetItemBkColor(0x00000000);
		pComboAccounts->SetHotItemBkColor(0x000000);
		pComboAccounts->SetSelectedItemBkColor(0x000000);

	}

	//获取对象
	CGlobalWebLink * pGlobalWebLink=CGlobalWebLink::GetInstance();

	//构造地址
	TCHAR szBillUrl[256]=TEXT("");
	_sntprintf(szBillUrl,CountArray(szBillUrl),TEXT("%s/Ads/LogonLogo.aspx"),pGlobalWebLink->GetPlatformLink());
	
	CWkeBrowserUI* pActiveXUI2 = static_cast<CWkeBrowserUI*>(GetControlByName(szWebBrowserLogonAdControlName));
	if( pActiveXUI2 ) 
	{
		pActiveXUI2->Navigate(szBillUrl);
	}
}

//消息提醒
void CDlgLogon::Notify(TNotifyUI &  msg)
{
	//获取对象
	CControlUI * pControlUI = msg.pSender;

	//点击事件
	if (lstrcmp(msg.sType, TEXT("click")) == 0)
	{
		if(lstrcmp(pControlUI->GetName(), szButtonCloseControlName)==0) 
		{
			return OnCancel(); 
		}
		else if(lstrcmp(pControlUI->GetName(), szButtonMinControlName)==0)
		{
			ShowWindow(SW_MINIMIZE);
			return;
		}
		else if(lstrcmp(pControlUI->GetName(), szButtonLogonControlName)==0)
		{
			return OnOK();
		}
		else if(lstrcmp(pControlUI->GetName(), szButtonRegisterControlName)==0)
		{
			return OnBnClickedRegister();
		}
		//else if(lstrcmp(pControlUI->GetName(), szButtonCleanAccountsControlName)==0)
		//{
		//	return OnBnClickedDeleteRecord();
		//}
		else if(lstrcmp(pControlUI->GetName(), szButtonForgetPasswdControlName)==0)
		{
			return OnBnForgetPassword();
		}
		else if(lstrcmp(pControlUI->GetName(), szButtonWebLink1ControlName)==0 ||
				lstrcmp(pControlUI->GetName(), szButtonWebLink2ControlName)==0 ||
				lstrcmp(pControlUI->GetName(), szButtonWebLink3ControlName)==0 ||
				lstrcmp(pControlUI->GetName(), szButtonWebLink4ControlName)==0  )
		{
			//获取对象
			CGlobalWebLink * pGlobalWebLink=CGlobalWebLink::GetInstance();

			//构造地址
			TCHAR szLogonLink[256]=TEXT("");
			_sntprintf(szLogonLink,CountArray(szLogonLink),TEXT("%s/LogonLink%d.aspx"),pGlobalWebLink->GetPlatformLink(),pControlUI->GetTag());

			//打开页面
			ShellExecute(NULL,TEXT("OPEN"),szLogonLink,NULL,NULL,SW_NORMAL);

			return;
		}
	}
	else if(lstrcmp(msg.sType,TEXT("itemselect")) ==0 )
	{
		if(lstrcmp(pControlUI->GetName(), szComboServerControlName)==0)
		{
			//查找对象
			//CEditComboUI* pEditComboUI = static_cast<CEditComboUI*>(pControlUI);
			//pEditComboUI->SetText(pEditComboUI->GetItemAt(msg.wParam)->GetText());

			return;
		}
		else if(lstrcmp(pControlUI->GetName(), szComboAccountsControlName)==0)
		{
			return OnSelchangeAccounts();
		}
	}
	else if(lstrcmp(msg.sType,TEXT("textchanged")) ==0 )
	{
		if(lstrcmp(pControlUI->GetName(),szComboAccountsControlName)==0)
		{
			CEditComboUI * pComboAccounts=(CEditComboUI *)pControlUI;
			INT nCurrentSel=pComboAccounts->GetCurSel();
			CControlUI* pCnt= pComboAccounts->GetItemAt(nCurrentSel);
			if(pCnt)
			{
				tagAccountsInfo * pAccountsInfo=(tagAccountsInfo *)(pCnt->GetUserData());
				if(pAccountsInfo != NULL )
				{
					CEditUI * pEditPassword = (CEditUI * )GetControlByName(szEditPasswordControlName);
					CCheckButtonUI * pCheckButtonPasswd=(CCheckButtonUI *)GetControlByName(szCheckButtonPasswdControlName);

					if( pControlUI->GetText() != pAccountsInfo->szAccounts )
					{
						if(pCheckButtonPasswd!=NULL) pCheckButtonPasswd->SetCheck(false);
						if(pEditPassword) pEditPassword->SetText(TEXT(""));
					}
					else
					{
						if(pCheckButtonPasswd!=NULL) pCheckButtonPasswd->SetCheck(true);
						if(pEditPassword) pEditPassword->SetText(pAccountsInfo->szPassword);
					}
				}
			}


			return ;
		}
	}
	else if (lstrcmp(msg.sType,TEXT("itemdelete")) ==0 )
	{
		if(lstrcmp(pControlUI->GetName(),szComboAccountsControlName)==0)
		{
			int nIndex = (int)msg.wParam;

			OnBnClickedDeleteRecord(nIndex);
		}
	}

	return;
}

//显示验证
bool CDlgLogon::ShowAccreditWindow()
{
	//变量定义
	CDlgAccredit DlgAccredit;
	m_pDlgAccedit = &DlgAccredit;
	INT_PTR nEndCode = DlgAccredit.DoModal();
	m_pDlgAccedit=NULL;

	//拷贝号码
	if(nEndCode==IDOK)
	{		
		lstrcpyn(m_szPassPortID,DlgAccredit.GetPassPortNumber(),CountArray(m_szPassPortID));
	}

	return nEndCode==IDOK;
}

//构造数据
WORD CDlgLogon::ConstructLogonPacket(BYTE cbBuffer[], WORD wBufferSize, BYTE cbValidateFlags)
{
	//变量定义
	TCHAR szPassword[LEN_MD5];
	CWHEncrypt::MD5Encrypt(m_szPassword,szPassword);

	//发送数据
	switch (m_cbLogonMode)
	{
	case LOGON_BY_VISITOR:			//游客登录
		{
			//变量定义
			CMD_GP_LogonVisitor * pLogonVisitor=(CMD_GP_LogonVisitor *)cbBuffer;

			//系统信息
			pLogonVisitor->dwPlazaVersion=VERSION_PLAZA;
			pLogonVisitor->cbValidateFlags=cbValidateFlags;

			return sizeof(CMD_GP_LogonVisitor);
		}
	case LOGON_BY_GAME_ID:			//I D 登录
		{
			//变量定义
			CMD_GP_LogonGameID * pLogonGameID=(CMD_GP_LogonGameID *)cbBuffer;

			//系统信息
			pLogonGameID->dwPlazaVersion=VERSION_PLAZA;

			//机器标识
			CWHService::GetMachineIDEx(pLogonGameID->szMachineID);

			//登录信息
			pLogonGameID->dwGameID=m_dwGameID;
			lstrcpyn(pLogonGameID->szPassword,szPassword,CountArray(pLogonGameID->szPassword));
			pLogonGameID->cbValidateFlags=cbValidateFlags;

			//保存密码
			CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
			tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();
			lstrcpyn(pGlobalUserData->szPassword,pLogonGameID->szPassword,CountArray(pGlobalUserData->szPassword));

			return sizeof(CMD_GP_LogonGameID);
		}
	case LOGON_BY_ACCOUNTS:			//帐号登录
		{
			//变量定义
			CMD_GP_LogonAccounts * pLogonAccounts=(CMD_GP_LogonAccounts *)cbBuffer;

			//系统信息
			pLogonAccounts->dwPlazaVersion=VERSION_PLAZA;

			//机器标识
			CWHService::GetMachineIDEx(pLogonAccounts->szMachineID);

			//登录信息
			pLogonAccounts->cbValidateFlags=cbValidateFlags;
			lstrcpyn(pLogonAccounts->szPassword,szPassword,CountArray(pLogonAccounts->szPassword));
			lstrcpyn(pLogonAccounts->szAccounts,m_szAccounts,CountArray(pLogonAccounts->szAccounts));
			lstrcpyn(pLogonAccounts->szPassPortID,m_szPassPortID,CountArray(pLogonAccounts->szPassPortID));	

			//保存密码
			CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
			tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();
			lstrcpyn(pGlobalUserData->szPassword,pLogonAccounts->szPassword,CountArray(pGlobalUserData->szPassword));

			//计算大小
			WORD wDataSize = sizeof(CMD_GP_LogonAccounts)-sizeof(pLogonAccounts->szPassPortID);
			wDataSize += CountStringBuffer(pLogonAccounts->szPassPortID);

			return wDataSize;
		}
	}

	return 0;
}

//加载帐号
void CDlgLogon::LoadAccountsInfo()
{
	//清理记录
	m_AccountsInfoArray.RemoveAll();

	//读取信息
	CWHRegKey RegUserInfo;
	if (RegUserInfo.OpenRegKey(REG_USER_INFO, false)==false) return;

	//变量定义
	DWORD dwRegIndex=0L;
	DWORD dwLastUserID=RegUserInfo.GetValue(TEXT("LastUserID"),0L);

	//读取信息 最多MAX_ACCOUNTS个
	for(int i = 0; i < MAX_ACCOUNTS; i++)
	{
		//变量定义
		tagAccountsInfo AccountsInfo;
		ZeroMemory(&AccountsInfo,sizeof(AccountsInfo));

		//读取键项
		TCHAR szKeyItemName[16]=TEXT("");
		if (RegUserInfo.EnumItemString(dwRegIndex++,szKeyItemName,CountArray(szKeyItemName))==false) break;

		//打开子键
		CWHRegKey RegUserItem;
		if (RegUserItem.Attach(RegUserInfo.OpenItemKey(szKeyItemName))==NULL) continue;

		//帐户信息
		AccountsInfo.dwUserID=_tstol(szKeyItemName);
		RegUserItem.GetString(TEXT("GameID"),AccountsInfo.szGameID,CountArray(AccountsInfo.szGameID));
		RegUserItem.GetString(TEXT("UserAccount"),AccountsInfo.szAccounts,CountArray(AccountsInfo.szAccounts));

		//读取密码
		TCHAR szPassword[MAX_ENCRYPT_LEN]=TEXT("");
		RegUserItem.GetString(TEXT("UserPassword"),szPassword,CountArray(szPassword));

		//解开密码
		if (szPassword[0]!=0)
		{
			CWHEncrypt::XorCrevasse(szPassword,AccountsInfo.szPassword,CountArray(AccountsInfo.szPassword));
		}

		//构造数据
		tagAccountsInfo * pAccountsInfo=new tagAccountsInfo;
		CopyMemory(pAccountsInfo,&AccountsInfo,sizeof(AccountsInfo));

		//插入数据
		m_AccountsInfoArray.InsertAt(m_AccountsInfoArray.GetCount(),pAccountsInfo);
	}

	//查找对象
	CEditComboUI * pComboAccounts= (CEditComboUI * )GetControlByName(szComboAccountsControlName);
	if(pComboAccounts==NULL) return;

	//插入信息
	for (INT_PTR i=0;i<m_AccountsInfoArray.GetCount();i++)
	{
		//插入信息
		INT nGameIDItem=CB_ERR;
		INT nAccountsItem=CB_ERR;
		tagAccountsInfo * pAccountsInfo=m_AccountsInfoArray[i];
		
		//插入信息
		if (pAccountsInfo->szAccounts[0]!=0)
		{
			try
			{
				CDialogBuilder builder2;
				//变量定义
				CListComboElementUI * pListLabelElement=NULL;

				pListLabelElement = static_cast<CListComboElementUI *>(builder2.Create( _T("DlgLogonAcountItem.xml"), (UINT)0, NULL, &m_PaintManager));

				if (pListLabelElement)
				{
					//设置子项
					pListLabelElement->SetText(pAccountsInfo->szAccounts);
					if(pComboAccounts->Add(pListLabelElement)==true) pListLabelElement->SetUserData(pAccountsInfo);

					//默认帐户
					if(dwLastUserID==pAccountsInfo->dwUserID) ((IListItemUI*)pListLabelElement)->Select(true);
				}


			}
			catch(...)
			{
				ASSERT(FALSE);
				break;
			}
		}
	}

	//设置选择
	if ((pComboAccounts->GetCount()>0)&&(pComboAccounts->GetCurSel()==LB_ERR))
	{
		pComboAccounts->SelectItem(0);
	}

	return;
}

//地址信息
void CDlgLogon::LoadLogonServerInfo()
{
	//读取数目
	CGlobalServer GlobalServer;
	WORD wItemCount=GlobalServer.GetItemCount();

	//上次登录
	TCHAR szLastServer[LEN_SERVER]=TEXT("");
	GlobalServer.GetLastServerName(szLastServer);

	//查找对象
	CEditComboUI * pComboServer= (CEditComboUI * )GetControlByName(szComboServerControlName);
	if(pComboServer==NULL) return;



	//读取信息
	for (WORD i=0;i<wItemCount;i++)
	{
		//读取信息
		TCHAR szServerItem[LEN_SERVER]=TEXT("");
		GlobalServer.GetServerItemName(i,szServerItem);

		//插入信息
		if (szServerItem[0]!=0)
		{
			try
			{
				CDialogBuilder builder2;
				//变量定义
				CListLabelElementUI * pListLabelElement=NULL;

				pListLabelElement = static_cast<CListLabelElementUI *>(builder2.Create( _T("DlgLogonServerItem.xml"), (UINT)0, NULL, &m_PaintManager));

				if (pListLabelElement)
				{
					//设置子项
					pListLabelElement->SetText(szServerItem);
					pComboServer->Add(pListLabelElement);
				}
			}
			catch(...)
			{
				ASSERT(FALSE);
				break;
			}

			//最近登录
			if ((szLastServer[0]!=0)&&(lstrcmp(szLastServer,szServerItem)==0))
			{
				szLastServer[0]=0;
				pComboServer->SelectItem(i);
			}
		}
	}

	//最近登录
	if (szLastServer[0]!=0)
	{
		try
		{
			CDialogBuilder builder2;
			//变量定义
			CListLabelElementUI * pListLabelElement=NULL;

			pListLabelElement = static_cast<CListLabelElementUI *>(builder2.Create( _T("DlgLogonServerItem.xml"), (UINT)0, NULL, &m_PaintManager));

			if (pListLabelElement)
			{
				//设置子项
				pListLabelElement->SetText(szLastServer);
				pComboServer->Add(pListLabelElement);
			}
			pComboServer->SelectItem(pListLabelElement->GetIndex());
		}
		catch(...)
		{
			ASSERT(FALSE);
		}
	}

	//设置选择
	if (pComboServer->GetCurSel()==LB_ERR)
	{
		if (pComboServer->GetCount()==0L)
		{
			try
			{
				CDialogBuilder builder2;
				//变量定义
				CListLabelElementUI * pListLabelElement=NULL;

				pListLabelElement = static_cast<CListLabelElementUI *>(builder2.Create( _T("DlgLogonServerItem.xml"), (UINT)0, NULL, &m_PaintManager));

				if (pListLabelElement)
				{
					//设置子项
					pListLabelElement->SetText(szLogonServer);
					pComboServer->Add(pListLabelElement);
				}
				pComboServer->SelectItem(pListLabelElement->GetIndex());
			}
			catch(...)
			{
				ASSERT(FALSE);
			}
		}
		else
		{
			pComboServer->SelectItem(0);
		}
	}

	return;
}

//获取信息
bool CDlgLogon::GetInformation()
{
	//获取信息
	m_dwGameID=0;
	m_szPassPortID[0]=0;

	//查找对象
	CEditComboUI * pEditServer = (CEditComboUI * )GetControlByName(szComboServerControlName);
	if(pEditServer!=NULL) lstrcpyn(m_szLogonServer,pEditServer->GetText(),CountArray(m_szLogonServer));

	//查找对象
	CEditComboUI * pEditAccounts = (CEditComboUI * )GetControlByName(szComboAccountsControlName);
	if(pEditAccounts!=NULL) lstrcpyn(m_szAccounts,pEditAccounts->GetText(),CountArray(m_szAccounts));

	//查找对象
	CEditUI * pEditPassword = (CEditUI * )GetControlByName(szEditPasswordControlName);
	if(pEditPassword!=NULL) lstrcpyn(m_szPassword,pEditPassword->GetText(),CountArray(m_szPassword));

	//地址判断
	if (m_szLogonServer[0]==0L)
	{
		//提示信息
		CDlgInformation Information(this);
		Information.ShowMessageBox(TEXT("请选择“登录服务器”后再登录，若有困难请点击“游戏指南”获取帮助！"),MB_ICONERROR,0);

		//显示窗口
		ShowWindow(SW_SHOW);

		//设置焦点
		if(pEditServer) pEditServer->SetFocus();

		return false;
	}

	//帐号判断
	if ((m_cbLogonMode==LOGON_BY_ACCOUNTS)&&(m_szAccounts[0]==0L))
	{
		//提示信息
		CDlgInformation Information(this);
		Information.ShowMessageBox(TEXT("请您输入帐号后再登录！"),MB_ICONERROR,0);

		//显示窗口
		ShowWindow(SW_SHOW);

		//设置焦点
		pEditAccounts->SetFocus();

		return false;
	}

	//密码判断
	if (m_szPassword[0]==0)
	{
		//提示信息
		CDlgInformation Information(this);
		Information.ShowMessageBox(TEXT("请您输入密码后再登录！"),MB_ICONERROR,0);

		//显示窗口
		ShowWindow(SW_SHOW);

		//查找对象
		CEditUI * pEditPassword = (CEditUI * )GetControlByName(szEditPasswordControlName);
		if(pEditPassword!=NULL) pEditPassword->SetFocus();

		return false;
	}

	//记住密码
	CCheckButtonUI * pCheckButtonPasswd=(CCheckButtonUI *)GetControlByName(szCheckButtonPasswdControlName);;
	if(pCheckButtonPasswd!=NULL)
	{
		m_cbRemPassword=pCheckButtonPasswd->GetCheck();
	}

	return true;
}

//选择改变
void CDlgLogon::OnSelchangeAccounts()
{
	//变量定义
	CEditComboUI * pComboAccounts=(CEditComboUI *)GetControlByName(szComboAccountsControlName);
	if(pComboAccounts==NULL) return;

	//查找对象
	//CEditUI * pEditAccounts = (CEditUI * )GetControlByName(szEditAccountsControlName);
	//if(pEditAccounts!=NULL) pEditAccounts->SetText(pComboAccounts->GetText());	

	//查找对象
	CEditUI * pEditPassword = (CEditUI * )GetControlByName(szEditPasswordControlName);
	if(pEditPassword==NULL) return;

	//查找对象
	CCheckButtonUI * pCheckButtonPasswd=(CCheckButtonUI *)GetControlByName(szCheckButtonPasswdControlName);;
	if(pCheckButtonPasswd==NULL) return;

	//获取选择
	INT nCurrentSel=pComboAccounts->GetCurSel();
	if(nCurrentSel==LB_ERR) return;
	tagAccountsInfo * pAccountsInfo=(tagAccountsInfo *)pComboAccounts->GetItemAt(nCurrentSel)->GetUserData();	

	//设置控件
	if ((nCurrentSel!=LB_ERR)&&(pAccountsInfo!=NULL))
	{
		//设置密码
		pEditPassword->SetText(pAccountsInfo->szPassword);

		//记住密码
		bool bRemPassword=(pAccountsInfo->szPassword[0]!=0);
		pCheckButtonPasswd->SetCheck(bRemPassword);
	}
	else
	{
		//设置密码
		pEditPassword->SetText(TEXT(""));

		//记住密码
		pCheckButtonPasswd->SetCheck(false);
	}

	return;
}

//编辑改变
void CDlgLogon::OnCbnEditchangeAccounts()
{
	//安全设置
	CCheckButtonUI * pCheckButtonPasswd=(CCheckButtonUI *)GetControlByName(szCheckButtonPasswdControlName);;
	if(pCheckButtonPasswd!=NULL) pCheckButtonPasswd->SetCheck(false);

	//密码重置
	CEditUI * pEditPassword = (CEditUI * )GetControlByName(szEditPasswordControlName);
	if(pEditPassword) pEditPassword->SetText(TEXT(""));

	return;
}


//忘记密码
void CDlgLogon::OnBnForgetPassword()
{
	//获取对象
	CGlobalWebLink * pGlobalWebLink=CGlobalWebLink::GetInstance();

	//构造地址
	TCHAR szLogonLink[256]=TEXT("");
	_sntprintf(szLogonLink,CountArray(szLogonLink),TEXT("%s/LogonLink5.aspx"),pGlobalWebLink->GetPlatformLink());

	//打开页面
	ShellExecute(NULL,TEXT("OPEN"),szLogonLink,NULL,NULL,SW_NORMAL);
}

//删除记录
void CDlgLogon::OnBnClickedDeleteRecord(int index)
{
	//查找对象
	CEditComboUI * pComboAccounts= (CEditComboUI * )GetControlByName(szComboAccountsControlName);
	if(pComboAccounts==NULL) return;


	//查找对象
	CEditUI * pEditPassword = (CEditUI * )GetControlByName(szEditPasswordControlName);
	if(pEditPassword==NULL) return;	

	//获取信息
	tagAccountsInfo * pAccountsInfo=NULL;
	INT nCurrentSel=index;

	//删除处理
	if (nCurrentSel!=LB_ERR)
	{
		//获取变量
		ASSERT(pComboAccounts->GetItemAt(nCurrentSel)->GetUserData()!=NULL);
		pAccountsInfo=(tagAccountsInfo *)pComboAccounts->GetItemAt(nCurrentSel)->GetUserData();

		//构造标识
		CString strUserID;
		strUserID.Format(TEXT("%ld"),pAccountsInfo->dwUserID);

		//用户表项
		CWHRegKey RegUserInfo;
		RegUserInfo.OpenRegKey(REG_USER_INFO,false);

		//删除表项
		RegUserInfo.RecurseDeleteKey(strUserID);

		//删除列表
		pComboAccounts->RemoveAt(nCurrentSel);				

		//更新选择
		if ((pComboAccounts->GetCurSel()==LB_ERR)&&(pComboAccounts->GetCount()>0))
		{
			pComboAccounts->SelectItem(0);
		}
	}

	//设置焦点
	pComboAccounts->SetFocus();

	//获取变量
	nCurrentSel=pComboAccounts->GetCurSel();
	pAccountsInfo=(nCurrentSel!=LB_ERR)?pAccountsInfo=(tagAccountsInfo *)pComboAccounts->GetItemAt(nCurrentSel)->GetUserData():NULL;

	//设置密码
	pEditPassword->SetText((pAccountsInfo!=NULL)?pAccountsInfo->szPassword:TEXT(""));

	//查找对象
	CCheckButtonUI * pCheckButtonPasswd=(CCheckButtonUI *)GetControlByName(szCheckButtonPasswdControlName);;
	if(pCheckButtonPasswd==NULL) return;

	//记住密码
	bool bRemPassword=(pAccountsInfo!=NULL)?(pAccountsInfo->szPassword[0]!=0):false;
	pCheckButtonPasswd->SetCheck(bRemPassword);

	return;
}

//注册帐号
void CDlgLogon::OnBnClickedRegister()
{
	//隐藏窗口
	ShowWindow(SW_HIDE);

	//获取地址
	CString strLogonServer;
	GetDlgItemText(IDC_LOGON_SERVER,strLogonServer);

	//注册帐号
	CControlUI * pControl = GetControlByName(szComboServerControlName);
	if (pControl != NULL) strLogonServer=pControl->GetText();

	//构造地址
	strLogonServer.TrimLeft();
	strLogonServer.TrimRight();
	lstrcpyn(m_szLogonServer,(strLogonServer.IsEmpty()==true)?szLogonServer:strLogonServer,CountArray(m_szLogonServer));

	//显示注册
	ASSERT(CMissionLogon::GetInstance()!=NULL);
	if (CMissionLogon::GetInstance()!=NULL) CMissionLogon::GetInstance()->ShowRegister();

	return;
}



//////////////////////////////////////////////////////////////////////////////////