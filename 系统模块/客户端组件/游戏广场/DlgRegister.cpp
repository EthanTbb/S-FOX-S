#include "StdAfx.h"
#include "GamePlaza.h"
#include "DlgRegister.h"
#include "MissionLogon.h"
#include "DlgInformation.h"
//////////////////////////////////////////////////////////////////////////////////

//屏幕位置
#define LAYERED_SIZE				5									//分层大小

//////////////////////////////////////////////////////////////////////////////////
#define	LAST_VERIFY_NONE			0
#define	LAST_VERIFY_LOCAL			1
#define	LAST_VERIFY_RESULT_FAIL		2
#define	LAST_VERIFY_RESULT_SUCESS	3

#define PERFORM_VERIFY_ACCOUNTS		0
#define PERFORM_VERIFY_NICKNAME		1
#define PERFORM_VERIFY_PASSWORD		2
#define PERFORM_VERIFY_TWICE		3


//////////////////////////////////////////////////////////////////////////////////
//响应控件
const TCHAR * szButtonCloseControlName = TEXT("ButtonClose");
const TCHAR * szButtonRegisterControlName = TEXT("ButtonRegister");
const TCHAR * szButtonRefreshControlName = TEXT("ButtonRefresh");
const TCHAR * szButtonArticleControlName = TEXT("ButtonArticle");
const TCHAR * szCheckButtonAgreeControlName = TEXT("CheckButtonAgree");
const TCHAR * szVerifyCodeControlName = TEXT("Verifycode");
const TCHAR * szPowerScaleControlName = TEXT("PowerScaleUIPass");
const TCHAR * szEditAccountsControlName = TEXT("EditAccounts");
const TCHAR * szEditNickNameControlName = TEXT("EditNickName");
const TCHAR * szEditLogonPassControlName = TEXT("EditLogonPass");
const TCHAR * szEditLogonPass1ControlName = TEXT("EditLogonPass1");
const TCHAR * szEditCompellationControlName = TEXT("EditCompellation");
const TCHAR * szEditSpreaderControlName = TEXT("EditSpreader");
const TCHAR * szEditValidateCodeControlName = TEXT("EditValidateCode");
const TCHAR * szLabelAccountsValidateControlName = TEXT("LabelAccountsValidate");
const TCHAR * szLabelAccountsInvalidControlName = TEXT("LabelAccountsInvalid");
const TCHAR * szLabelAccountsTipControlName = TEXT("LabelAccountsTip");
const TCHAR * szLabelNickNameValidateControlName = TEXT("LabelNickNameValidate");
const TCHAR * szLabelNickNameInvalidControlName = TEXT("LabelNickNameInvalid");
const TCHAR * szLabelNickNameTipControlName = TEXT("LabelNickNameTip");
const TCHAR * szLabelLogonPassValidateControlName = TEXT("LabelLogonPassValidate");
const TCHAR * szLabelLogonPassInvalidControlName = TEXT("LabelLogonPassInvalid");
const TCHAR * szLabelLogonPassTipControlName = TEXT("LabelLogonPassTip");
const TCHAR * szLabelLogonPassValidate1ControlName = TEXT("LabelLogonPassValidate1");
const TCHAR * szLabelLogonPassInvalid1ControlName = TEXT("LabelLogonPassInvalid1");
const TCHAR * szLabelLogonPassTip1ControlName = TEXT("LabelLogonPassTip1");
//////////////////////////////////////////////////////////////////////////////////

//构造函数
CDlgRegister::CDlgRegister() : CFGuiDialog(IDD_DLG_POPUP)
{
	//验证变量
	m_wVerifyMask=0;
	m_bVerifyIndividual=false;	

	//设置变量
	m_wFaceID=0;
	m_cbGender=0;
	m_szAccounts[0]=0;
	m_szNickName[0]=0;
	m_szLogonPass[0]=0;
	m_szInsurePass[0]=0;
	m_szPassPortID[0]=0;
	m_szCompellation[0]=0;
	m_dwAgentID = 0;
	m_dwSpreaderGameID=0;

	//设置变量
	m_cbRemPassword=FALSE;
	m_cbLogonPassLevel=PASSWORD_LEVEL_0;

	m_wLastVerifyAccountsResult = LAST_VERIFY_NONE;
	m_wLastVerifyNickNameResult = LAST_VERIFY_NONE;



	return;
}

//析构函数
CDlgRegister::~CDlgRegister()
{
	DestroyWindow();
}

BOOL CDlgRegister::PreTranslateMessage(MSG * pMsg)
{
	//按键过虑
	if ((pMsg->message==WM_KEYDOWN)&&(pMsg->wParam==VK_ESCAPE))
	{
		return TRUE;
	}

	return __super::PreTranslateMessage(pMsg);
}

//配置函数
BOOL CDlgRegister::OnInitDialog()
{
	__super::OnInitDialog();

	//设置标题
	SetWindowText(TEXT("用户注册"));

	return TRUE;
}

//确定函数
VOID CDlgRegister::OnOK()
{
	//获取信息
	if (GetInformation()==false) return;

	//设置变量
	m_bVerifyIndividual=false;

	//隐藏窗口
	ShowWindow(SW_HIDE);

	//执行注册
	CMissionLogon * pMissionLogon=CMissionLogon::GetInstance();
	if (pMissionLogon!=NULL) pMissionLogon->PerformLogonMission(m_cbRemPassword==TRUE);

	DestroyWindow();

	return;
}

//取消函数
VOID CDlgRegister::OnCancel()
{
	//关闭窗口
	EndDialog(IDCANCEL);

	//显示登录
	ASSERT(CMissionLogon::GetInstance()!=NULL);
	if (CMissionLogon::GetInstance()!=NULL) CMissionLogon::GetInstance()->ShowLogon();

	DestroyWindow();
	
	return;
}

//发送登录
WORD CDlgRegister::ConstructRegisterPacket(BYTE cbBuffer[], WORD wBufferSize, BYTE cbValidateFlags)
{
	//注册数据
	if(m_bVerifyIndividual==false)
	{
		//变量定义
		TCHAR szLogonPass[LEN_MD5];
		TCHAR szInsurePass[LEN_MD5];
		CWHEncrypt::MD5Encrypt(m_szLogonPass,szLogonPass);
		CWHEncrypt::MD5Encrypt(m_szInsurePass,szInsurePass);

		//变量定义
		CMD_GP_RegisterAccounts * pRegisterAccounts=(CMD_GP_RegisterAccounts *)cbBuffer;

		//系统信息
		pRegisterAccounts->dwPlazaVersion=VERSION_PLAZA;

		//机器标识
		CWHService::GetMachineIDEx(pRegisterAccounts->szMachineID);

		//基本信息
		pRegisterAccounts->wFaceID=m_wFaceID;
		pRegisterAccounts->cbGender=m_cbGender;
		pRegisterAccounts->cbValidateFlags=cbValidateFlags;
		pRegisterAccounts->dwAgentID = m_dwAgentID;
		pRegisterAccounts->dwSpreaderGameID = m_dwSpreaderGameID;
		lstrcpyn(pRegisterAccounts->szAccounts,m_szAccounts,CountArray(pRegisterAccounts->szAccounts));
		lstrcpyn(pRegisterAccounts->szNickName,m_szNickName,CountArray(pRegisterAccounts->szNickName));
		lstrcpyn(pRegisterAccounts->szLogonPass,szLogonPass,CountArray(pRegisterAccounts->szLogonPass));
		lstrcpyn(pRegisterAccounts->szPassPortID,m_szPassPortID,CountArray(pRegisterAccounts->szPassPortID));
		lstrcpyn(pRegisterAccounts->szCompellation,m_szCompellation,CountArray(pRegisterAccounts->szCompellation));		

		//保存密码
		CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
		tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();
		lstrcpyn(pGlobalUserData->szPassword,pRegisterAccounts->szLogonPass,CountArray(pGlobalUserData->szPassword));

		//发送数据
		return sizeof(CMD_GP_RegisterAccounts);
	}
	else
	{
		//变量定义
		CMD_GP_VerifyIndividual * pVerifyIndividual=(CMD_GP_VerifyIndividual *)cbBuffer;
		WORD wVerifySize = (WORD)(sizeof(cbBuffer)-sizeof(CMD_GP_VerifyIndividual));
		CSendPacketHelper SendPacket(cbBuffer+sizeof(CMD_GP_VerifyIndividual),wVerifySize);

		//构造结构
		pVerifyIndividual->dwPlazaVersion=VERSION_PLAZA;
		pVerifyIndividual->wVerifyMask=m_wVerifyMask;

		//用户帐号
		if(pVerifyIndividual->wVerifyMask==VERIFY_ACCOUNTS)
		{
			SendPacket.AddPacket(m_szAccounts,DTP_GP_UI_ACCOUNTS);
		}

		//用户昵称
		if(pVerifyIndividual->wVerifyMask==VERIFY_NICKNAME)
		{
			SendPacket.AddPacket(m_szNickName,DTP_GP_UI_NICKNAME);
		}

		//发送数据
		return sizeof(CMD_GP_ModifyIndividual)+SendPacket.GetDataSize();		 
	}
}

//校验结果
void CDlgRegister::OnVerifypIndividualResult(WORD wVerifyMask,bool bVerifyPassage,LPCTSTR pszErrorMsg)
{
	if(bVerifyPassage==true)
	{
		//验证帐号
		if(wVerifyMask==VERIFY_ACCOUNTS)
		{
			//验证通过
			CControlUI * pLabelAccountValidate = GetControlByName(szLabelAccountsValidateControlName);
			if(pLabelAccountValidate!=NULL) pLabelAccountValidate->SetVisible(true);

			m_wLastVerifyAccountsResult = LAST_VERIFY_RESULT_SUCESS;

			return;
		}

		//验证昵称
		if(wVerifyMask==VERIFY_NICKNAME)
		{
			//验证通过
			CControlUI * pLabelNickNameValidate = GetControlByName(szLabelNickNameValidateControlName);
			if(pLabelNickNameValidate!=NULL) pLabelNickNameValidate->SetVisible(true);

			m_wLastVerifyNickNameResult = LAST_VERIFY_RESULT_SUCESS;
			return;
		}
	}
	else
	{
		//验证帐号
		if(wVerifyMask==VERIFY_ACCOUNTS)
		{
			//帐号无效
			CControlUI * pLabelAccountInvalid = GetControlByName(szLabelAccountsInvalidControlName);
			if(pLabelAccountInvalid!=NULL)
			{
				pLabelAccountInvalid->SetVisible(true);
				pLabelAccountInvalid->SetText(pszErrorMsg);
			}	

			m_wLastVerifyAccountsResult = LAST_VERIFY_RESULT_FAIL;

			return;
		}

		//验证帐号
		if(wVerifyMask==VERIFY_NICKNAME)
		{
			//帐号无效
			CControlUI * pLabelNickNameInvalid = GetControlByName(szLabelNickNameInvalidControlName);
			if(pLabelNickNameInvalid!=NULL)
			{
				pLabelNickNameInvalid->SetVisible(true);
				pLabelNickNameInvalid->SetText(pszErrorMsg);
			}

			m_wLastVerifyNickNameResult = LAST_VERIFY_RESULT_FAIL;
		}
	}

	return;
}

//消息提醒
 void CDlgRegister::Notify(TNotifyUI &  msg)
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
		else if(lstrcmp(pControlUI->GetName(), szButtonRegisterControlName)==0)
		{
			return OnOK();
		}
		else if(lstrcmp(pControlUI->GetName(),szButtonRefreshControlName)==0)
		{
			return OnBnClickedChangeValidate();
		}
		else if(lstrcmp(pControlUI->GetName(),szButtonArticleControlName)==0)
		{
			return OnBnClickedAgrrArticle();
		}
	}
	else if(lstrcmp(msg.sType,TEXT("textchanged")) ==0 )
	{	
		if(lstrcmp(pControlUI->GetName(),szEditLogonPassControlName)==0)
		{
			return OnEnChangeLogonPass();
		}
	}
	else if(lstrcmp(msg.sType,TEXT("killfocus")) ==0 )
	{
		if(lstrcmp(pControlUI->GetName(),szEditAccountsControlName)==0)
		{
			return PerformVerifyIndividual(PERFORM_VERIFY_ACCOUNTS);
		}
		else if(lstrcmp(pControlUI->GetName(),szEditNickNameControlName)==0)
		{
			return PerformVerifyIndividual(PERFORM_VERIFY_NICKNAME);
		}
		else if(lstrcmp(pControlUI->GetName(),szEditLogonPassControlName)==0)
		{
			return PerformVerifyIndividual(PERFORM_VERIFY_PASSWORD);
		}
		else if(lstrcmp(pControlUI->GetName(),szEditLogonPass1ControlName)==0)
		{
			return PerformVerifyIndividual(PERFORM_VERIFY_TWICE);
		}
	}
	else if(lstrcmp(msg.sType,TEXT("setfocus")) ==0 )
	{
		if(lstrcmp(pControlUI->GetName(),szEditAccountsControlName)==0)
		{
			return SelectedTipInfo(PERFORM_VERIFY_ACCOUNTS);
		}
		else if(lstrcmp(pControlUI->GetName(),szEditNickNameControlName)==0)
		{
			return SelectedTipInfo(PERFORM_VERIFY_NICKNAME);
		}
		else if(lstrcmp(pControlUI->GetName(),szEditLogonPassControlName)==0)
		{
			return SelectedTipInfo(PERFORM_VERIFY_PASSWORD);
		}
		else if(lstrcmp(pControlUI->GetName(),szEditLogonPass1ControlName)==0)
		{
			return SelectedTipInfo(PERFORM_VERIFY_TWICE);
		}
	}
 }

LPCTSTR CDlgRegister::GetSkinFile()
{
	return TEXT("DlgRegister.xml");
}

 //初始控件
void CDlgRegister::InitControlUI()
{

}



//获取信息
bool CDlgRegister::GetInformation()
{
	//获取头像
	IFaceItemControl * pIFaceItemControl=CFaceItemControl::GetInstance();
	m_wFaceID=rand()%pIFaceItemControl->GetFaceCount();

	//用户帐号
	CControlUI * pEditAccountsUI = GetControlByName(szEditAccountsControlName);
	if(pEditAccountsUI!=NULL) GetControlString(pEditAccountsUI,m_szAccounts,CountArray(m_szAccounts));

	//用户昵称
	//CControlUI * pEditNickNameUI = GetControlByName(szEditNickNameControlName);
	//if(pEditNickNameUI!=NULL) GetControlString(pEditNickNameUI,m_szNickName,CountArray(m_szNickName));
	lstrcpyn(m_szNickName,m_szAccounts,CountArray(m_szNickName));

	//推荐人
	//获取目录
	TCHAR szPath[MAX_PATH]=TEXT("");
	CWHService::GetWorkDirectory(szPath, sizeof(szPath));

	//读取配置
	TCHAR szFileName[MAX_PATH]=TEXT("");
	_sntprintf(szFileName,CountArray(szFileName),TEXT("%s\\Spreader.ini"),szPath);

	//读取推荐人
	TCHAR	szSpreaderID[MAX_PATH];
	ZeroMemory(szSpreaderID,CountArray(szSpreaderID));
	GetPrivateProfileString(TEXT("SpreaderInfo"),TEXT("SpreaderID"),TEXT(""),szSpreaderID,CountArray(szSpreaderID),szFileName);
	m_dwSpreaderGameID = _ttoi(szSpreaderID);

	//登录密码
	CControlUI *  pEditLogonPassUI = GetControlByName(szEditLogonPassControlName);
	if(pEditLogonPassUI!=NULL) GetControlString(pEditLogonPassUI,m_szLogonPass,CountArray(m_szLogonPass));

	//确认密码
	TCHAR szLogonPass2[LEN_PASSWORD]=TEXT("");
	//CControlUI * pEditLogonPass1UI = GetControlByName(szEditLogonPass1ControlName);
	//if(pEditLogonPass1UI!=NULL) GetControlString(pEditLogonPass1UI,szLogonPass2,CountArray(szLogonPass2));
	lstrcpyn(szLogonPass2,m_szLogonPass,CountArray(szLogonPass2));


	//真实
	CControlUI * pEditCompellationUI = GetControlByName(szEditCompellationControlName);
	if(pEditCompellationUI!=NULL) GetControlString(pEditCompellationUI,m_szCompellation,CountArray(m_szCompellation));

	//验证码
	TCHAR szValidateCode[5]=TEXT("");
	CControlUI * pEditValidateCodeUI = GetControlByName(szEditValidateCodeControlName);
	if(pEditValidateCodeUI!=NULL) GetControlString(pEditValidateCodeUI,szValidateCode,CountArray(szValidateCode));

	//变量定义
	TCHAR szDescribe[128]=TEXT("");
	CUserItemElement * pUserItemElement=CUserItemElement::GetInstance();

	//帐号判断
	if (pUserItemElement->EfficacyAccounts(m_szAccounts,szDescribe,CountArray(szDescribe))==false)
	{
		//提示信息
		CDlgInformation Information(this);
		Information.ShowMessageBox(szDescribe,MB_ICONERROR,0);

		//设置焦点
		pEditAccountsUI->SetFocus();

		return false;
	}

	////昵称判断
	//if (pUserItemElement->EfficacyNickName(m_szNickName,szDescribe,CountArray(szDescribe))==false)
	//{
	//	//提示信息
	//	CDlgInformation Information(this);
	//	Information.ShowMessageBox(szDescribe,MB_ICONERROR,0);

	//	//设置焦点
	//	pEditNickNameUI->SetFocus();

	//	return false;
	//}

	//密码判断
	if (pUserItemElement->EfficacyPassword(m_szLogonPass,szDescribe,CountArray(szDescribe))==false)
	{
		//提示信息
		CDlgInformation Information(this);
		Information.ShowMessageBox(szDescribe,MB_ICONERROR,0);

		//设置焦点
		pEditLogonPassUI->SetFocus();

		return false;
	}

	////密码判断
	//if (lstrcmp(m_szLogonPass,szLogonPass2)!=0)
	//{
	//	//提示信息
	//	CDlgInformation Information(this);
	//	Information.ShowMessageBox(TEXT("确认密码与密码不一致，请重新输入！"),MB_ICONERROR,0);

	//	//设置焦点
	//	pEditLogonPass1UI->SetFocus();

	//	return false;
	//}

	//效验判断
	CVerifyCodeUI * pVerifyCodeUI = (CVerifyCodeUI *)GetControlByName(szVerifyCodeControlName);
	CString strVerifyCodeString(pVerifyCodeUI->GetCurrCode());
	if (strVerifyCodeString.CompareNoCase(szValidateCode)!=0)
	{
		//提示信息
		CDlgInformation Information(this);
		Information.ShowMessageBox(TEXT("效验码验证失败，请重新输入！"),MB_ICONERROR,0);

		//设置焦点
		pEditValidateCodeUI->SetFocus();
		pVerifyCodeUI->Refresh();

		return false;
	}

	//同意条款
	CCheckButtonUI * pCheckButtonAgree = static_cast<CCheckButtonUI*>(GetControlByName(szCheckButtonAgreeControlName));
	if(pCheckButtonAgree!=NULL && pCheckButtonAgree->GetCheck()==false)
	{
		//提示信息
		CDlgInformation Information(this);
		Information.ShowMessageBox(TEXT("请勾选《游戏中心服务条款》！"),MB_ICONERROR,0);

		return false;
	}

	//性别选择
	m_cbGender = GENDER_FEMALE;

	//获取代理
	CWHRegKey RegAgentID;
	if (RegAgentID.OpenRegKey(TEXT(""), false)==true)
	{
		TCHAR szAgentID[MAX_PATH]={0};
		RegAgentID.GetString(TEXT("InstallName"),szAgentID,CountArray(szAgentID));
		m_dwAgentID = _ttoi(szAgentID);
	}


	return true;
}


void CDlgRegister::SelectedTipInfo( WORD wPreVerifyMask )
{
	//用户帐号
	if(wPreVerifyMask==PERFORM_VERIFY_ACCOUNTS)
	{
		CControlUI * pLabelAccountValidate = GetControlByName(szLabelAccountsValidateControlName);
		if(pLabelAccountValidate!=NULL) pLabelAccountValidate->SetVisible(false);

		CControlUI * pLabelAccountInvalid = GetControlByName(szLabelAccountsInvalidControlName);
		if(pLabelAccountInvalid!=NULL)	pLabelAccountInvalid->SetVisible(false);

		CControlUI * pLabelAccountTip = GetControlByName(szLabelAccountsTipControlName);
		if(pLabelAccountTip!=NULL)
		{
			pLabelAccountTip->SetVisible(true);
		}

	}
	//用户昵称
	else if(wPreVerifyMask==PERFORM_VERIFY_NICKNAME)
	{
		CControlUI * pLabelNickNameValidate = GetControlByName(szLabelNickNameValidateControlName);
		if(pLabelNickNameValidate!=NULL) pLabelNickNameValidate->SetVisible(false);

		CControlUI * pLabelNickNameInvalid = GetControlByName(szLabelNickNameInvalidControlName);
		if(pLabelNickNameInvalid!=NULL)	pLabelNickNameInvalid->SetVisible(false);

		CControlUI * LabelNickNameTip = GetControlByName(szLabelNickNameTipControlName);
		if(LabelNickNameTip!=NULL)
		{
			LabelNickNameTip->SetVisible(true);
		}
	}
	//用户密码
	else if(wPreVerifyMask==PERFORM_VERIFY_PASSWORD)
	{
		CControlUI * pLabelLogonPassValidate = GetControlByName(szLabelLogonPassValidateControlName);
		if(pLabelLogonPassValidate!=NULL) pLabelLogonPassValidate->SetVisible(false);

		CControlUI * pLabelLogonPassInvalid = GetControlByName(szLabelLogonPassInvalidControlName);
		if(pLabelLogonPassInvalid!=NULL)	pLabelLogonPassInvalid->SetVisible(false);

		CControlUI * LabelLogonPassTip = GetControlByName(szLabelLogonPassTipControlName);
		if(LabelLogonPassTip!=NULL)
		{
			LabelLogonPassTip->SetVisible(true);
		}
	}
	//确认密码
	else if(wPreVerifyMask==PERFORM_VERIFY_TWICE)
	{
		CControlUI * pLabelLogonPassValidate1 = GetControlByName(szLabelLogonPassValidate1ControlName);
		if(pLabelLogonPassValidate1!=NULL) pLabelLogonPassValidate1->SetVisible(false);

		CControlUI * pLabelLogonPassInvalid1 = GetControlByName(szLabelLogonPassInvalid1ControlName);
		if(pLabelLogonPassInvalid1!=NULL)	pLabelLogonPassInvalid1->SetVisible(false);

		CControlUI * LabelLogonPassTip1 = GetControlByName(szLabelLogonPassTip1ControlName);
		if(LabelLogonPassTip1!=NULL)
		{
			LabelLogonPassTip1->SetVisible(true);
		}
	}
}

//验证资料
void CDlgRegister::PerformVerifyIndividual(WORD wPreVerifyMask)
{
	//变量定义
	TCHAR szDescribe[128]=TEXT("");

	//验证帐号
	if(wPreVerifyMask==PERFORM_VERIFY_ACCOUNTS)
	{
		//隐藏标签
		CControlUI * pLabelAccountInvalid = GetControlByName(szLabelAccountsInvalidControlName);
		if(pLabelAccountInvalid!=NULL) pLabelAccountInvalid->SetVisible(false);

		CControlUI * pLabelAccountTip = GetControlByName(szLabelAccountsTipControlName);
		if(pLabelAccountTip!=NULL)	pLabelAccountTip->SetVisible(false);

		CControlUI * pLabelAccountValidate = GetControlByName(szLabelAccountsValidateControlName);
		if(pLabelAccountValidate!=NULL) pLabelAccountValidate->SetVisible(false);

		//变量定义
		TCHAR szAccounts[LEN_ACCOUNTS]=TEXT("");	

		//用户帐号
		CControlUI * pEditAccountsUI = GetControlByName(szEditAccountsControlName);
		if(pEditAccountsUI!=NULL) GetControlString(pEditAccountsUI,szAccounts,CountArray(szAccounts));
		if (lstrlen(szAccounts) ==0) return;
		if(m_szAccounts[0]!=0 && lstrcmp(m_szAccounts,szAccounts)==0) 
		{
			if(m_wLastVerifyAccountsResult == LAST_VERIFY_LOCAL || m_wLastVerifyAccountsResult == LAST_VERIFY_RESULT_FAIL)
			{
				if(pLabelAccountInvalid!=NULL) pLabelAccountInvalid->SetVisible(true);
			}
			else if (m_wLastVerifyAccountsResult == LAST_VERIFY_RESULT_SUCESS)
			{
				if(pLabelAccountValidate!=NULL) pLabelAccountValidate->SetVisible(true);
			}

			return;
		}

		//帐号判断
		CUserItemElement * pUserItemElement=CUserItemElement::GetInstance();
		if (pUserItemElement->EfficacyAccounts(szAccounts,szDescribe,CountArray(szDescribe))==false)
		{
			//帐号无效
			if(pLabelAccountInvalid!=NULL)
			{
				pLabelAccountInvalid->SetVisible(true);
				pLabelAccountInvalid->SetText(szDescribe);
			}

			m_wLastVerifyAccountsResult = LAST_VERIFY_LOCAL;

			//设置焦点
			pEditAccountsUI->SetFocus();
			lstrcpyn(m_szAccounts,szAccounts,CountArray(m_szAccounts));

			return;
		}

		//设置变量
		m_bVerifyIndividual=true;
		m_wVerifyMask=VERIFY_ACCOUNTS;
		lstrcpyn(m_szAccounts,szAccounts,CountArray(m_szAccounts));

		//执行注册
		CMissionLogon * pMissionLogon=CMissionLogon::GetInstance();
		if (pMissionLogon!=NULL) pMissionLogon->PerformLogonMission(false);

		return;
	}

	//用户昵称
	if(wPreVerifyMask==PERFORM_VERIFY_NICKNAME)
	{
		//隐藏标签
		CControlUI * pLabelNickNameInvalid = GetControlByName(szLabelNickNameInvalidControlName);
		if(pLabelNickNameInvalid!=NULL) pLabelNickNameInvalid->SetVisible(false);

		CControlUI * LabelNickNameTip = GetControlByName(szLabelNickNameTipControlName);
		if(LabelNickNameTip!=NULL) LabelNickNameTip->SetVisible(false);

		CControlUI * pLabelNickNameValidate = GetControlByName(szLabelNickNameValidateControlName);
		if(pLabelNickNameValidate!=NULL) pLabelNickNameValidate->SetVisible(false);

		//变量定义
		TCHAR szNickName[LEN_ACCOUNTS]=TEXT("");	

		//用户帐号
		CControlUI * pEditNickNameUI = GetControlByName(szEditNickNameControlName);
		if(pEditNickNameUI!=NULL) GetControlString(pEditNickNameUI,szNickName,CountArray(szNickName));
		if (lstrlen(szNickName) ==0) return;
		if(m_szNickName[0]!=0 && lstrcmp(m_szNickName,szNickName)==0) 
		{
			if(m_wLastVerifyNickNameResult == LAST_VERIFY_LOCAL || m_wLastVerifyNickNameResult == LAST_VERIFY_RESULT_FAIL)
			{
				if(pLabelNickNameInvalid!=NULL) pLabelNickNameInvalid->SetVisible(true);
			}
			else if (m_wLastVerifyNickNameResult == LAST_VERIFY_RESULT_SUCESS)
			{
				if(pLabelNickNameValidate!=NULL) pLabelNickNameValidate->SetVisible(true);
			}
			return;
		}

		//帐号判断
		CUserItemElement * pUserItemElement=CUserItemElement::GetInstance();
		if (pUserItemElement->EfficacyNickName(szNickName,szDescribe,CountArray(szDescribe))==false)
		{
			//帐号无效
			if(pLabelNickNameInvalid!=NULL)
			{
				pLabelNickNameInvalid->SetVisible(true);
				pLabelNickNameInvalid->SetText(szDescribe);
			}

			m_wLastVerifyNickNameResult = LAST_VERIFY_LOCAL;

			//设置焦点
			pEditNickNameUI->SetFocus();

			return;
		}

		//设置变量
		m_bVerifyIndividual=true;
		m_wVerifyMask=VERIFY_NICKNAME;
		lstrcpyn(m_szNickName,szNickName,CountArray(m_szNickName));

		//执行注册
		CMissionLogon * pMissionLogon=CMissionLogon::GetInstance();
		if (pMissionLogon!=NULL) pMissionLogon->PerformLogonMission(false);

		return;
	}

	//用户密码
	if(wPreVerifyMask==PERFORM_VERIFY_PASSWORD)
	{
		CControlUI * pLabelLogonPassValidate = GetControlByName(szLabelLogonPassValidateControlName);
		if(pLabelLogonPassValidate!=NULL) pLabelLogonPassValidate->SetVisible(false);

		CControlUI * pLabelLogonPassInvalid = GetControlByName(szLabelLogonPassInvalidControlName);
		if(pLabelLogonPassInvalid!=NULL)	pLabelLogonPassInvalid->SetVisible(false);

		CControlUI * LabelLogonPassTip = GetControlByName(szLabelLogonPassTipControlName);
		if(LabelLogonPassTip!=NULL)	LabelLogonPassTip->SetVisible(false);

		//变量定义
		TCHAR szLogonPass[LEN_ACCOUNTS]=TEXT("");	

		CControlUI * pEditLogonPassUI = GetControlByName(szEditLogonPassControlName);
		if(pEditLogonPassUI!=NULL) GetControlString(pEditLogonPassUI,szLogonPass,CountArray(szLogonPass));

		if (lstrlen(szLogonPass) ==0) return;

		//密码判断
		CUserItemElement * pUserItemElement=CUserItemElement::GetInstance();
		if (pUserItemElement->EfficacyPassword(szLogonPass,szDescribe,CountArray(szDescribe))==false)
		{
			//提示信息
			if(pLabelLogonPassInvalid!=NULL)
			{
				pLabelLogonPassInvalid->SetVisible(true);
				pLabelLogonPassInvalid->SetText(szDescribe);
			}

			return;
		}

		if(pLabelLogonPassValidate!=NULL)	pLabelLogonPassValidate->SetVisible(true);

	}

	//2次密码
	if(wPreVerifyMask==PERFORM_VERIFY_TWICE)
	{
		CControlUI * pLabelLogonPassValidate1 = GetControlByName(szLabelLogonPassValidate1ControlName);
		if(pLabelLogonPassValidate1!=NULL) pLabelLogonPassValidate1->SetVisible(false);

		CControlUI * pLabelLogonPassInvalid1 = GetControlByName(szLabelLogonPassInvalid1ControlName);
		if(pLabelLogonPassInvalid1!=NULL)	pLabelLogonPassInvalid1->SetVisible(false);

		CControlUI * LabelLogonPassTip1 = GetControlByName(szLabelLogonPassTip1ControlName);
		if(LabelLogonPassTip1!=NULL) LabelLogonPassTip1->SetVisible(false);

		//变量定义
		TCHAR szLogonPass[LEN_ACCOUNTS]=TEXT("");
		TCHAR szLogonPass1[LEN_ACCOUNTS]=TEXT("");	

		CControlUI * pEditLogonPassUI = GetControlByName(szEditLogonPassControlName);
		if(pEditLogonPassUI!=NULL) GetControlString(pEditLogonPassUI,szLogonPass,CountArray(szLogonPass));

		CControlUI * pEditLogonPass1UI = GetControlByName(szEditLogonPass1ControlName);
		if(pEditLogonPass1UI!=NULL) GetControlString(pEditLogonPass1UI,szLogonPass1,CountArray(szLogonPass1));

		if (lstrlen(szLogonPass) ==0 && lstrlen(szLogonPass1) ==0) return;
		//密码判断
		if (lstrcmp(szLogonPass,szLogonPass1)!=0)
		{
			//提示信息
			if(pLabelLogonPassInvalid1!=NULL)
			{
				pLabelLogonPassInvalid1->SetVisible(true);
				pLabelLogonPassInvalid1->SetText(TEXT("确认密码与密码不一致"));
			}

			return;
		}

		if(pLabelLogonPassValidate1!=NULL)	pLabelLogonPassValidate1->SetVisible(true);
	}
}

//获取字符
void CDlgRegister::GetControlString(CControlUI * pControlUI, TCHAR szString[], WORD wMaxCount)
{
	if(pControlUI==NULL) return;
	CString strString(pControlUI->GetText());
	//strString.TrimLeft();
	//strString.TrimRight();
	lstrcpyn(szString,strString,wMaxCount);
	return;
}


//密码输入
void CDlgRegister::OnEnChangeLogonPass()
{
	//获取密码
	TCHAR szPassword[LEN_PASSWORD]=TEXT("");

	//获取密码
	CControlUI * pEditLogonPass=GetControlByName(szEditLogonPassControlName);
	if(pEditLogonPass!=NULL) lstrcpyn(szPassword,pEditLogonPass->GetText(),CountArray(szPassword));

	//等级判断
	CUserItemElement * pUserItemElement=CUserItemElement::GetInstance();
	BYTE cbPasswordLevel=pUserItemElement->GetPasswordLevel(szPassword);

	//更新窗口
	if (m_cbLogonPassLevel!=cbPasswordLevel)
	{
		//设置变量
		m_cbLogonPassLevel=cbPasswordLevel;

		CPowerScaleUI * pPowerScaleUI = (CPowerScaleUI *)GetControlByName(szPowerScaleControlName);
		pPowerScaleUI->SetLevelState(m_cbLogonPassLevel);
	}

	return;
}

//更换验证
void CDlgRegister::OnBnClickedChangeValidate()
{
	CVerifyCodeUI * pVerifyCodeUI = (CVerifyCodeUI *)GetControlByName(szVerifyCodeControlName);
	pVerifyCodeUI->Refresh();
	return;
}

//同意条款
void CDlgRegister::OnBnClickedAgrrArticle()
{
	//获取对象
	CGlobalWebLink * pGlobalWebLink=CGlobalWebLink::GetInstance();

	//构造地址
	TCHAR szNavigation[256]=TEXT("");
	_sntprintf(szNavigation,CountArray(szNavigation),TEXT("%s/Agreement.aspx"),pGlobalWebLink->GetPlatformLink());

	//打开页面
	ShellExecute(NULL,TEXT("OPEN"),szNavigation,NULL,NULL,SW_NORMAL);
}
//////////////////////////////////////////////////////////////////////////////////
