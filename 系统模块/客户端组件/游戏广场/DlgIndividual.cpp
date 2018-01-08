#include "Stdafx.h"
#include "Resource.h"
#include "DlgIndividual.h"
#include "PlatformFrame.h"
#include "PlatformEvent.h"
#include "GlobalUnits.h"
#include "DlgCustomFace.h"
#include "DlgInformation.h"

//////////////////////////////////////////////////////////////////////////////////
//控件名字
const TCHAR* const szButtonCloseControlName = TEXT("ButtonClose");
const TCHAR* const szEditUserIDControlName = TEXT("EditUserID");
const TCHAR* const szEditNickNameControlName = TEXT("EditNickName");
const TCHAR* const szEditAccountsControlName = TEXT("EditAccounts");
const TCHAR* const szEditStandardLogonPWControlName = TEXT("EditStandardLogonPW");
const TCHAR* const szNumberUserLevelControlName = TEXT("NumberUserLevel");
const TCHAR* const szRadioMankindControlName = TEXT("RadioButtonMale");
const TCHAR* const szRadioFemaleControlName = TEXT("RadioButtonFemale");
const TCHAR* const szLabelVipControlName = TEXT("LabelVip");
const TCHAR* const szLableInsureTittleControlName = TEXT("LableInsureTittle");
const TCHAR* const szButtonStandardComfirmControlName = TEXT("ButtonStandardComfirm");
const TCHAR* const szRichSignControlName = TEXT("RichSign");
const TCHAR* const szEditSignControlName = TEXT("EditSign");
const TCHAR* const szLabelSignLimitTipControlName = TEXT("LabelSignLimitTip");
const TCHAR* const szButtonVipControlName = TEXT("ButtonVip");
const TCHAR* const szLabelVipLevelControlName = TEXT("LabelVipLevel");
//详细资料
const TCHAR* const szEditMobilePhoneControlName = TEXT("EditMobilePhone");
const TCHAR* const szEditQQControlName = TEXT("EditQQ");
const TCHAR* const szEditEMailControlName = TEXT("EditEMail");
const TCHAR* const szEditSpreaderControlName = TEXT("EditSpreader");
const TCHAR* const szszEditAddressControlName = TEXT("szEditAddress");
const TCHAR* const szEditRealInfoPWControlName = TEXT("EditRealInfoPW");
const TCHAR* const szButtonRealInfoComfirmControlName = TEXT("ButtonRealInfoComfirm");
//实名认证
const TCHAR* const szButtonRealAuthComfirmControlName = TEXT("ButtonRealAuthComfirm");
const TCHAR* const szEditRealAuthPWControlName = TEXT("EditRealAuthPW");
const TCHAR* const szEditRealNameControlName = TEXT("EditRealName");
const TCHAR* const szEditIDNumberControlName = TEXT("EditIDNumber");
const TCHAR* const szContainerRealAuthControlName = TEXT("ContainerRealAuth");
const TCHAR* const szContainerRealAuthTipPassControlName = TEXT("ContainerRealAuthTipPass");
const TCHAR* const szContainerRealAuthTipPassFlagControlName = TEXT("ContainerRealAuthTipPassFlag");
//修改密码
const TCHAR* const szEditLogonPWOrgControlName = TEXT("EditLogonPWOrg");
const TCHAR* const szEditLogonPWNewControlName = TEXT("EditLogonPWNew");
const TCHAR* const szEditLogonPWComfirmControlName = TEXT("EditLogonPWComfirm");
const TCHAR* const szEditInsurePWOrgControlName = TEXT("EditInsurePWOrg");
const TCHAR* const szEditInsurePWNewControlName = TEXT("EditInsurePWNew");
const TCHAR* const szEditInsurePWComfirmControlName = TEXT("EditInsurePWComfirm");
const TCHAR* const szEditInsurePassControlName = TEXT("EditInsurePwd");
const TCHAR* const szEditInsurePass1ControlName = TEXT("EditInsurePwd2");
const TCHAR* const szEditInsureLogonControlName = TEXT("EditLogonPwd");
const TCHAR* const szPScaleLogonPWNewControlName = TEXT("PScaleLogonPWNew");
const TCHAR* const szPScaleInsurePWNewControlName = TEXT("PScaleInsurePWNew");
const TCHAR* const szPScaleInsurePWSetControlName = TEXT("PScaleInsurePWSet");
//开通银行
const TCHAR* const szButtonLogonPWModifyControlName = TEXT("ButtonLogonPWModify");
const TCHAR* const szButtonInsurePWModifyControlName = TEXT("ButtonInsurePWModify");
const TCHAR* const szButtonOpenControlName = TEXT("ButtonInsureOpen");
const TCHAR* const szButtonModifyAvatarControlName = TEXT("ButtonModifyAvatar");
//标签控件
const TCHAR* const szOptionIndividualStandardControlName = TEXT("OptionIndividualStandard");
const TCHAR* const szOptionIndividualRealInfoControlName = TEXT("OptionIndividualRealInfo");
const TCHAR* const szOptionIndividualModifyControlName = TEXT("OptionIndividualModify");
const TCHAR* const szOptionIndividualRealAuthControlName = TEXT("OptionIndividualRealAuth");
const TCHAR* const szTabIndividualControlName = TEXT("TabLayoutIndividual");
const TCHAR* const szTabInsurePassControlName = TEXT("TabLayoutInsurePass");
const TCHAR* const szUserAvatarControlName = TEXT("UserAvatar");

//////////////////////////////////////////////////////////////////////////////////
//网络事件
const BYTE byMissionNone					=		0	;			//没有任务
const BYTE byMissionIndividualLoad			=		1	;			//查询资料
const BYTE byMissionIndividualModify		=		2	;			//修改资料
const BYTE byMissionIndividualAuth			=		3	;			//实名认证
const BYTE byMissionLogonModify				=		4	;			//登录密码
const BYTE byMissionInsureModify			=		5	;			//银行密码
const BYTE byMissionInsureSet				=		6	;			//银行密码

//////////////////////////////////////////////////////////////////////////////////


//构造函数
CDlgIndividual::CDlgIndividual(): CFGuiDialog(IDD_DLG_POPUP)
{
	//设置任务
	m_byMissionType = byMissionNone;
	m_MissionManager.InsertMissionItem(this);

	m_cbSelectedIndex = 0;
}

//析构函数
CDlgIndividual::~CDlgIndividual()
{

}


//创建函数
BOOL CDlgIndividual::OnInitDialog()
{
	__super::OnInitDialog();

	//修改样式
	ModifyStyle(0, WS_MINIMIZEBOX|WS_MAXIMIZEBOX);

	//设置标题
	SetWindowText(TEXT("个人中心"));

	//居中窗口
	CenterWindow();

	//更新界面
	UpdateUI();

	AvtiveMission(byMissionIndividualLoad);


	return TRUE;
}

//确定函数
VOID CDlgIndividual::OnOK()
{

	//return __super::OnOK();
}

//取消消息
VOID CDlgIndividual::OnCancel()
{
	return __super::OnCancel();
}

void CDlgIndividual::InitControlUI()
{
	//获取控件
	CTabLayoutUI *pTabLayoutUI = static_cast<CTabLayoutUI *>(GetControlByName(szTabIndividualControlName));
	COptionUI* pOptionUI[4];
	pOptionUI[0] = static_cast<COptionUI *>(GetControlByName(szOptionIndividualStandardControlName));
	pOptionUI[1] = static_cast<COptionUI *>(GetControlByName(szOptionIndividualRealInfoControlName));
	pOptionUI[2] = static_cast<COptionUI *>(GetControlByName(szOptionIndividualModifyControlName));
	pOptionUI[3] = static_cast<COptionUI *>(GetControlByName(szOptionIndividualRealAuthControlName));
	

	//设置页面
	if(pTabLayoutUI != NULL && pOptionUI[m_cbSelectedIndex] != NULL)
	{
		pTabLayoutUI->SelectItem(m_cbSelectedIndex);
		pOptionUI[m_cbSelectedIndex]->Selected(true);

	}

	if(m_cbSelectedIndex == 2)
	{
		//设置银行密码
		if(CGlobalUserInfo::GetInstance()->GetGlobalUserData()->cbInsureEnabled==FALSE)
		{
			pTabLayoutUI = static_cast<CTabLayoutUI *>(GetControlByName(szTabInsurePassControlName));
			if(pTabLayoutUI != NULL) pTabLayoutUI->SelectItem(1);
		}
	}

	//用户信息
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	if(pGlobalUserData->CustomFaceInfo.dwDataSize != 0)
	{
		CCustomFaceManager * pCustomFaceManager=CCustomFaceManager::GetInstance();
		pCustomFaceManager->LoadUserCustomFace(pGlobalUserData->dwUserID,pGlobalUserData->dwCustomID,m_CustomFaceInfo96);
	}

	UpdateUserFace();

	if (CGlobalUnits::GetInstance()->GetAuthRealParameter()->dwAuthentDisable)
	{
		CControlUI *pControl = GetControlByName(szOptionIndividualRealAuthControlName);
		if (pControl!=NULL)
		{
			pControl->SetVisible(false);
		}
	}
}

//消息提醒
void CDlgIndividual::Notify(TNotifyUI &  msg)
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
		//vip链接
		else if(lstrcmp(pControlUI->GetName(), szButtonVipControlName)==0)
		{
			//获取对象
			ASSERT(CGlobalWebLink::GetInstance()!=NULL);
			CGlobalWebLink * pGlobalWebLink=CGlobalWebLink::GetInstance();

			//构造地址
			TCHAR szVipLinkInfo[256]=TEXT("");
			_sntprintf(szVipLinkInfo,CountArray(szVipLinkInfo),TEXT("%s/%sUserService/UserService5.aspx"),pGlobalWebLink->GetPlatformLink(),pGlobalWebLink->GetValidateLink());

			//打开页面
			ShellExecute(NULL,TEXT("OPEN"),szVipLinkInfo,NULL,NULL,SW_NORMAL);
		}
		//账号中心
		if(pControlUI->GetName() == szButtonStandardComfirmControlName)
		{
			if(EfficacyStandard())
			{
				AvtiveMission(byMissionIndividualModify);
			}

		}
		//实名资料
		if(pControlUI->GetName() == szButtonRealInfoComfirmControlName )
		{
			if(EfficacyExtendInfo())
			{
				AvtiveMission(byMissionIndividualModify);
			}
		}
		else if(pControlUI->GetName() == szButtonRealAuthComfirmControlName )
		{
			if(EfficacyReal())
			{
				AvtiveMission(byMissionIndividualAuth);
			}
		}
		//修改登录密码
		else if(pControlUI->GetName() == szButtonLogonPWModifyControlName)
		{
			if(EfficacyModifyLogonPassward())
			{
				AvtiveMission(byMissionLogonModify);
			}

		}
		//修改银行密码
		else if(pControlUI->GetName() == szButtonInsurePWModifyControlName)
		{
			if(EfficacyModifyInsurePassward())
			{
				AvtiveMission(byMissionInsureModify);
			}

		}
		//设置银行密码
		else if(pControlUI->GetName() == szButtonOpenControlName)
		{
			if(EfficacyOpenInsurePassward())
			{
				AvtiveMission(byMissionInsureSet);
			}
		}
		//更改头像
		else if(pControlUI->GetName() == szButtonModifyAvatarControlName)
		{
			CDlgCustomFace DlgCustomFace;
			DlgCustomFace.SetCustomFaceEvent(QUERY_OBJECT_PTR_INTERFACE(CPlatformFrame::GetInstance(),IUnknownEx));
			//显示窗口
			DlgCustomFace.DoModal();

			//用户信息
			CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
			tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();
			if(pGlobalUserData->CustomFaceInfo.dwDataSize != 0)
			{
				CCustomFaceManager * pCustomFaceManager=CCustomFaceManager::GetInstance();
				pCustomFaceManager->LoadUserCustomFace(pGlobalUserData->dwUserID,pGlobalUserData->dwCustomID,m_CustomFaceInfo96);
			}
			UpdateUserFace();
		}

	}
	else if(lstrcmp(msg.sType, TEXT("selectchanged")) == 0)
	{
		if(pControlUI->GetName() == szOptionIndividualStandardControlName)
		{
			m_cbSelectedIndex = 0;
			return;
		}
		else if(pControlUI->GetName() == szOptionIndividualRealInfoControlName)
		{
			m_cbSelectedIndex = 1;
			AvtiveMission(byMissionIndividualLoad);

			return;
		}
		else if(pControlUI->GetName() == szOptionIndividualModifyControlName)
		{
			m_cbSelectedIndex = 2;
			//设置银行密码
			if(CGlobalUserInfo::GetInstance()->GetGlobalUserData()->cbInsureEnabled==FALSE)
			{
				CTabLayoutUI *pTabLayoutUI = static_cast<CTabLayoutUI *>(GetControlByName(szTabInsurePassControlName));
				if(pTabLayoutUI != NULL) pTabLayoutUI->SelectItem(1);
			}

			return;
		}
		else if(pControlUI->GetName() == szOptionIndividualRealAuthControlName)
		{
			m_cbSelectedIndex = 3;
			return;
		}
	}
	else if(lstrcmp(msg.sType,TEXT("textchanged")) == 0)
	{	
		if(lstrcmp(pControlUI->GetName(), szEditLogonPWNewControlName) == 0)
		{
			//设置强度
			SetPassLevel(szPScaleLogonPWNewControlName, szEditLogonPWNewControlName);
			return;
		}
		else if(lstrcmp(pControlUI->GetName(), szEditInsurePWNewControlName) == 0)
		{
			//设置强度
			SetPassLevel(szPScaleInsurePWNewControlName, szEditInsurePWNewControlName);
			return;
		}
		else if(lstrcmp(pControlUI->GetName(),szEditInsurePass1ControlName) == 0)
		{
			//设置强度
			SetPassLevel(szPScaleInsurePWSetControlName, szEditInsurePass1ControlName);
			return;
		}
		else if(lstrcmp(pControlUI->GetName(),szEditSignControlName) == 0)
		{
			//用户签名
			CEditUI * pEdit = static_cast<CEditUI *>(GetControlByName(szEditSignControlName));
			if (pEdit)
			{
				int nMaxCharNum = 0;
				int nCurrCharNum = 0;
				nCurrCharNum = (int)(_tcslen(pEdit->GetText()));
				nMaxCharNum = pEdit->GetMaxChar();

				TCHAR szText[128]={0};	
				_sntprintf(szText, CountArray(szText), TEXT("(还可以输入%d字)"), nMaxCharNum - nCurrCharNum);

				CLabelUI *pLabelUI = static_cast<CLabelUI *>(GetControlByName(szLabelSignLimitTipControlName));
				if(pLabelUI)
				{
					pLabelUI->SetText(szText);

				}
			}

			return;
		}
	}
	
	return;
}

LPCTSTR CDlgIndividual::GetSkinFile()
{
	return TEXT("DlgIndividual.xml");
}

//连接事件
bool CDlgIndividual::OnEventMissionLink(INT nErrorCode)
{
	if (nErrorCode!=0L)
	{
		OnMissionLinkError(nErrorCode);
		return true;
	}

	switch(m_byMissionType)
	{
	case byMissionIndividualLoad:
		{
			OnMissionLinkIndividualLoad();
			break;
		}
	case byMissionIndividualModify:
		{
			OnMissionLinkIndividualModify();
			break;
		}
	case byMissionIndividualAuth:
		{
			OnMissionLinkIndividualAuth();
			break;
		}
	case byMissionLogonModify:
		{
			OnMissionLinkLogonModify();
			break;
		}
	case byMissionInsureSet:
		{
			OnMissionLinkInsureSet();
			break;
		}
	case byMissionInsureModify:
		{
			OnMissionLinkInsureModify();
			break;
		}
	default:
		return true;
	}
	return true;
}

//关闭事件
bool CDlgIndividual::OnEventMissionShut(BYTE cbShutReason)
{
	//提示消息
	if (cbShutReason!=SHUT_REASON_NORMAL)
	{
		//重试任务
		if (m_MissionManager.AvtiveMissionItem(this,true)==true)
		{
			return true;
		}

		//错误提示
		if (m_byMissionType == byMissionIndividualLoad)
		{
			//提示消息
			CDlgInformation Information(this);
			Information.ShowMessageBox(TEXT("连接服务器超时，用户资料获取失败！"),MB_OK|MB_ICONSTOP,30);
		}

		//错误提示
		if (m_byMissionType == byMissionIndividualModify)
		{
			//提示消息
			CDlgInformation Information(this);
			Information.ShowMessageBox(TEXT("连接服务器超时，用户资料修改失败！"),MB_OK|MB_ICONSTOP,30);
		}

		//错误提示
		if (m_byMissionType == byMissionInsureModify || m_byMissionType == byMissionLogonModify)
		{
			//提示消息
			CDlgInformation Information(this);
			Information.ShowMessageBox(TEXT("连接服务器超时，密码修改失败！"),MB_OK|MB_ICONSTOP,30);
		}
	}

	return true;
}

//读取事件
bool CDlgIndividual::OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize)
{
	//命令处理
	if (Command.wMainCmdID==MDM_GP_USER_SERVICE)
	{
		switch (Command.wSubCmdID)
		{
		case SUB_GP_OPERATE_FAILURE:		//操作失败
			{
				//效验参数
				CMD_GP_OperateFailure * pOperateFailure=(CMD_GP_OperateFailure *)pData;
				ASSERT(wDataSize>=(sizeof(CMD_GP_OperateFailure)-sizeof(pOperateFailure->szDescribeString)));
				if (wDataSize<(sizeof(CMD_GP_OperateFailure)-sizeof(pOperateFailure->szDescribeString))) return false;

				//失败处理
				//OnMissionConclude();

				//关闭连接
				m_MissionManager.ConcludeMissionItem(this,false);

				//显示消息
				if (pOperateFailure->szDescribeString[0]!=0)
				{
					CDlgInformation Information(this);
					Information.ShowMessageBox(pOperateFailure->szDescribeString,MB_ICONERROR,60);
				}

				return true;
			}
		case SUB_GP_INDIVIDUAL_RESULT:
			{
				//变量定义
				CMD_GP_IndividuaResult * pIndividualResult=(CMD_GP_IndividuaResult *)pData;

				//效验数据
				ASSERT(wDataSize>=(sizeof(CMD_GP_IndividuaResult)-sizeof(pIndividualResult->szNotifyContent)));
				if (wDataSize<(sizeof(CMD_GP_IndividuaResult)-sizeof(pIndividualResult->szNotifyContent))) return false;

				//关闭连接
				m_MissionManager.ConcludeMissionItem(this,false);

				//获取对象
				CGlobalUserInfo * pGlobalUserInfo = CGlobalUserInfo::GetInstance();
				tagGlobalUserData * pGlobalUserData = pGlobalUserInfo->GetGlobalUserData();

				//设置变量					
				pGlobalUserData->lUserScore = pIndividualResult->lCurrScore;

				//发送事件
				CPlatformEvent * pPlatformEvent=CPlatformEvent::GetInstance();
				if (pPlatformEvent!=NULL) 
				{
					pPlatformEvent->SendPlatformEvent(EVENT_USER_INFO_UPDATE,0L);
					pPlatformEvent->SendPlatformEvent(EVENT_USER_HIDE_REAL_TIP,0L);
				}

				//更新数据
				UpdateData();

				//更新界面
				UpdateUI();

				//显示消息
				if (pIndividualResult->szNotifyContent[0]!=0)
				{
					CDlgInformation Information(this);
					Information.ShowMessageBox(pIndividualResult->szNotifyContent,MB_ICONINFORMATION,60);
				}

				//更新大厅

				return true;
			}
		case SUB_GP_OPERATE_SUCCESS:	//操作成功
			{
				//变量定义
				CMD_GP_OperateSuccess * pOperateSuccess=(CMD_GP_OperateSuccess *)pData;

				//效验数据
				ASSERT(wDataSize>=(sizeof(CMD_GP_OperateSuccess)-sizeof(pOperateSuccess->szDescribeString)));
				if (wDataSize<(sizeof(CMD_GP_OperateSuccess)-sizeof(pOperateSuccess->szDescribeString))) return false;

				//更新数据
				UpdateData();

				//更新界面
				UpdateUI();

				//关闭连接
				m_MissionManager.ConcludeMissionItem(this,false);

				//显示消息
				if (pOperateSuccess->szDescribeString[0]!=0)
				{
					CDlgInformation Information(this);
					Information.ShowMessageBox(pOperateSuccess->szDescribeString,MB_ICONINFORMATION,60);
				}

				return true;
			}
		case SUB_GP_USER_INDIVIDUAL:	//个人信息
			{
				//效验参数
				ASSERT(wDataSize>=sizeof(CMD_GP_UserIndividual));
				if (wDataSize<sizeof(CMD_GP_UserIndividual)) return false;

				//变量定义
				CMD_GP_UserIndividual * pUserIndividual=(CMD_GP_UserIndividual *)pData;

				//变量定义
				CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
				tagIndividualUserData * pIndividualUserData=pGlobalUserInfo->GetIndividualUserData();

				//设置变量
				pIndividualUserData->dwUserID=pUserIndividual->dwUserID;

				
				//变量定义
				VOID * pDataBuffer=NULL;
				tagDataDescribe DataDescribe;
				CRecvPacketHelper RecvPacket(pUserIndividual+1,wDataSize-sizeof(CMD_GP_UserIndividual));

				//扩展信息
				while (true)
				{
					pDataBuffer=RecvPacket.GetData(DataDescribe);
					if (DataDescribe.wDataDescribe==DTP_NULL) break;
					switch (DataDescribe.wDataDescribe)
					{
					case DTP_GP_UI_USER_NOTE:		//用户备注
						{
							lstrcpyn(pIndividualUserData->szUserNote,(LPCTSTR)pDataBuffer,CountArray(pIndividualUserData->szUserNote));
							break;
						}
					case DTP_GP_UI_COMPELLATION:	//真实名字
						{
							lstrcpyn(pIndividualUserData->szCompellation,(LPCTSTR)pDataBuffer,CountArray(pIndividualUserData->szCompellation));
							break;
						}
					case DTP_GP_UI_PASSPORTID:	//身份证
						{
							lstrcpyn(pIndividualUserData->szPassPortID,(LPCTSTR)pDataBuffer,CountArray(pIndividualUserData->szPassPortID));
							break;
						}
					case DTP_GP_UI_SEAT_PHONE:		//固定电话
						{
							lstrcpyn(pIndividualUserData->szSeatPhone,(LPCTSTR)pDataBuffer,CountArray(pIndividualUserData->szSeatPhone));
							break;
						}
					case DTP_GP_UI_MOBILE_PHONE:	//移动电话
						{
							lstrcpyn(pIndividualUserData->szMobilePhone,(LPCTSTR)pDataBuffer,CountArray(pIndividualUserData->szMobilePhone));
							break;
						}
					case DTP_GP_UI_QQ:				//Q Q 号码
						{
							lstrcpyn(pIndividualUserData->szQQ,(LPCTSTR)pDataBuffer,CountArray(pIndividualUserData->szQQ));
							break;
						}
					case DTP_GP_UI_EMAIL:			//电子邮件
						{
							lstrcpyn(pIndividualUserData->szEMail,(LPCTSTR)pDataBuffer,CountArray(pIndividualUserData->szEMail));
							break;
						}
					case DTP_GP_UI_DWELLING_PLACE:	//联系地址
						{
							lstrcpyn(pIndividualUserData->szDwellingPlace,(LPCTSTR)pDataBuffer,CountArray(pIndividualUserData->szDwellingPlace));
							break;
						}
					}
				}

				//成功处理
				//OnMissionConclude();

				//更新资料
				//UpdateIndividualInfo();
				
				UpdateUI();
				//关闭连接
				m_MissionManager.ConcludeMissionItem(this,false);

				return true;
			}
		case SUB_GP_USER_INSURE_ENABLE_RESULT:	//开通结果
			{
				//效验参数
				ASSERT(wDataSize<=sizeof(CMD_GP_UserInsureEnableResult));
				if (wDataSize>sizeof(CMD_GP_UserInsureEnableResult)) return false;

				//关闭连接
				m_MissionManager.ConcludeMissionItem(this,false);

				//变量定义
				CMD_GP_UserInsureEnableResult * pUserInsureEnableResult=(CMD_GP_UserInsureEnableResult *)pData;

				//获取对象
				CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
				tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

				//设置变量
				pGlobalUserData->cbInsureEnabled = pUserInsureEnableResult->cbInsureEnabled;

				//设置银行密码
				if(pGlobalUserData->cbInsureEnabled==TRUE)
				{
					CTabLayoutUI *pTabLayoutUI = static_cast<CTabLayoutUI *>(GetControlByName(szTabInsurePassControlName));
					if(pTabLayoutUI != NULL) pTabLayoutUI->SelectItem(0);
				}

				//显示消息
				if (pUserInsureEnableResult->szDescribeString[0]!=0)
				{
					CDlgInformation Information(this);
					Information.ShowMessageBox(pUserInsureEnableResult->szDescribeString,MB_ICONINFORMATION,60);
				}
				else
				{
					CDlgInformation Information(this);
					Information.ShowMessageBox(TEXT("银行密码设置成功！"),MB_ICONINFORMATION,60);
				}

				return true;
			}

		}
	}

	return true;
}


//查询资料
void CDlgIndividual::OnMissionLinkIndividualLoad()
{
	//变量定义
	CMD_GP_QueryIndividual QueryIndividual;
	ZeroMemory(&QueryIndividual,sizeof(QueryIndividual));

	//变量定义
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	//设置变量
	QueryIndividual.dwUserID=pGlobalUserData->dwUserID;
	lstrcpyn(QueryIndividual.szPassword,pGlobalUserData->szPassword,CountArray(QueryIndividual.szPassword));

	//发送数据
	m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_QUERY_INDIVIDUAL,&QueryIndividual,sizeof(QueryIndividual));

	return;
}

//修改资料
void CDlgIndividual::OnMissionLinkIndividualModify()
{
	//变量定义
	BYTE cbBuffer[SOCKET_TCP_BUFFER];
	ZeroMemory(cbBuffer,sizeof(cbBuffer));

	//变量定义
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();
	tagIndividualUserData * pIndividualUserData=pGlobalUserInfo->GetIndividualUserData();

	//变量定义
	CMD_GP_ModifyIndividual * pModifyIndividual=(CMD_GP_ModifyIndividual *)cbBuffer;
	CSendPacketHelper SendPacket(cbBuffer+sizeof(CMD_GP_ModifyIndividual),sizeof(cbBuffer)-sizeof(CMD_GP_ModifyIndividual));

	//设置变量
	pModifyIndividual->dwUserID=pGlobalUserData->dwUserID;

	
	//获取密码
	TCHAR szItemText[128] = {0};
	if(m_cbSelectedIndex == 0)
		GetControlItemText(szEditStandardLogonPWControlName,szItemText,CountArray(szItemText));
	else if(m_cbSelectedIndex == 1)
		GetControlItemText(szEditRealInfoPWControlName,szItemText,CountArray(szItemText));
	if (szItemText[0]!=0) 
	{
		//加密密码
		TCHAR szPassword[128] = {0};
		CWHEncrypt::MD5Encrypt(szItemText,szPassword);
		lstrcpyn(pModifyIndividual->szPassword,szPassword,CountArray(pModifyIndividual->szPassword));
		ZeroMemory(szItemText,sizeof(szItemText));
	}

	if (m_cbSelectedIndex ==0)
	{
		//用户昵称
		GetControlItemText(szEditNickNameControlName,szItemText,CountArray(szItemText));
		if (szItemText[0]!=0)
		{
			SendPacket.AddPacket(szItemText,DTP_GP_UI_NICKNAME);
			ZeroMemory(szItemText,sizeof(szItemText));
		}

		//用户性别
		CRadioButtonUI * pManButtonUI = static_cast<CRadioButtonUI *>(GetControlByName(szRadioMankindControlName));
		CRadioButtonUI * pFemaleButtonUI = static_cast<CRadioButtonUI *>(GetControlByName(szRadioFemaleControlName));
		if (pManButtonUI && pManButtonUI->GetCheck() ) pModifyIndividual->cbGender=GENDER_MANKIND;
		if (pFemaleButtonUI && pFemaleButtonUI->GetCheck()) pModifyIndividual->cbGender=GENDER_FEMALE;


		//用户签名
		GetControlItemText(szEditSignControlName,szItemText,CountArray(szItemText));
		if (szItemText[0]!=0)
		{
			SendPacket.AddPacket(szItemText,DTP_GP_UI_UNDER_WRITE);
			ZeroMemory(szItemText,sizeof(szItemText));
		}
	}
	else
	{
		//用户昵称
		SendPacket.AddPacket(pGlobalUserData->szNickName,DTP_GP_UI_NICKNAME);

		//用户性别
		pModifyIndividual->cbGender= pGlobalUserData->cbGender;

		//用户签名
		SendPacket.AddPacket(pGlobalUserData->szUnderWrite,DTP_GP_UI_UNDER_WRITE);
	}

	if (m_cbSelectedIndex ==1)
	{
		//推荐标识
		if (pGlobalUserData->szSpreader[0] ==0)
		{
			GetControlItemText(szEditSpreaderControlName,szItemText,CountArray(szItemText));
			//转换数值
			DWORD dwSpreaderID = _ttoi(szItemText);
			_sntprintf(szItemText,CountArray(szItemText),TEXT("%d"),dwSpreaderID);
			if (szItemText[0]!=0) 
			{
				SendPacket.AddPacket(szItemText,DTP_GP_UI_SPREADER);
				ZeroMemory(szItemText,sizeof(szItemText));
			}
		}

		//手机号码
		GetControlItemText(szEditMobilePhoneControlName,szItemText,CountArray(szItemText));
		SendPacket.AddPacket(szItemText,DTP_GP_UI_MOBILE_PHONE);
		ZeroMemory(szItemText,sizeof(szItemText));


		//Q Q 号码
		GetControlItemText(szEditQQControlName,szItemText,CountArray(szItemText));
		SendPacket.AddPacket(szItemText,DTP_GP_UI_QQ);
		ZeroMemory(szItemText,sizeof(szItemText));

		//电子邮件
		GetControlItemText(szEditEMailControlName,szItemText,CountArray(szItemText));
		SendPacket.AddPacket(szItemText,DTP_GP_UI_EMAIL);
		ZeroMemory(szItemText,sizeof(szItemText));

		//详细地址
		GetControlItemText(szszEditAddressControlName,szItemText,CountArray(szItemText));
		SendPacket.AddPacket(szItemText,DTP_GP_UI_DWELLING_PLACE);
		ZeroMemory(szItemText,sizeof(szItemText));
	}
	else
	{
		//固定号码
		SendPacket.AddPacket(pIndividualUserData->szSeatPhone,DTP_GP_UI_SEAT_PHONE);

		//手机号码
		SendPacket.AddPacket(pIndividualUserData->szMobilePhone,DTP_GP_UI_MOBILE_PHONE);

		//Q Q 号码
		SendPacket.AddPacket(pIndividualUserData->szQQ,DTP_GP_UI_QQ);

		//电子邮件
		SendPacket.AddPacket(pIndividualUserData->szEMail,DTP_GP_UI_EMAIL);

		//详细地址
		SendPacket.AddPacket(pIndividualUserData->szDwellingPlace,DTP_GP_UI_DWELLING_PLACE);
	}

	//发送数据
	WORD wSendSize=sizeof(CMD_GP_ModifyIndividual)+SendPacket.GetDataSize();
	m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_MODIFY_INDIVIDUAL,cbBuffer,wSendSize);
	return;
}

void CDlgIndividual::OnMissionLinkIndividualAuth()
{
	//变量定义
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();
	tagIndividualUserData * pIndividualUserData=pGlobalUserInfo->GetIndividualUserData();

	//获取用户
	CMD_GP_RealAuth CmdParameter;
	ZeroMemory(&CmdParameter,sizeof(CmdParameter));
	CmdParameter.dwUserID=pGlobalUserData->dwUserID;

	//获取密码
	TCHAR szItemText[128] = {0};
	GetControlItemText(szEditRealAuthPWControlName,szItemText,CountArray(szItemText));
	if (szItemText[0]!=0) 
	{
		TCHAR szPassword[128] = {0};
		CWHEncrypt::MD5Encrypt(szItemText,szPassword);
		lstrcpyn(CmdParameter.szPassword,szPassword,CountArray(CmdParameter.szPassword));
		ZeroMemory(szItemText,sizeof(szItemText));
	}

	//获取名字
	GetControlItemText(szEditRealNameControlName,szItemText,CountArray(szItemText));
	if (szItemText[0]!=0) 
	{
		lstrcpyn(CmdParameter.szCompellation,szItemText,CountArray(CmdParameter.szCompellation));
		
		ZeroMemory(szItemText,sizeof(szItemText));
	}

	//获取身份
	GetControlItemText(szEditIDNumberControlName,szItemText,CountArray(szItemText));
	if (szItemText[0]!=0) 
	{
		lstrcpyn(CmdParameter.szPassPortID,szItemText,CountArray(CmdParameter.szPassPortID));
		ZeroMemory(szItemText,sizeof(szItemText));
	}

	m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_REAL_AUTH_QUERY,&CmdParameter,sizeof(CmdParameter));
}

//登录密码
void CDlgIndividual::OnMissionLinkLogonModify()
{
	//变量定义
	TCHAR szSrcPassword[LEN_PASSWORD];
	TCHAR szDesPassword[2][LEN_PASSWORD];

	//变量定义
	TCHAR szDescribe[128]=TEXT("");
	CUserItemElement * pUserItemElement=CUserItemElement::GetInstance();

	//变量定义
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	//获取密码
	CEditUI * pEditPass1Control = (CEditUI *)GetControlByName(szEditLogonPWOrgControlName);
	if(pEditPass1Control==NULL) return; 
	lstrcpyn(szSrcPassword,pEditPass1Control->GetText(),CountArray(szSrcPassword));

	//获取密码
	CEditUI * pEditPass2Control = (CEditUI *)GetControlByName(szEditLogonPWNewControlName);
	if(pEditPass2Control==NULL) return; 
	lstrcpyn(szDesPassword[0],pEditPass2Control->GetText(),CountArray(szDesPassword[0]));

	//获取密码
	CEditUI * pEditPass3Control = (CEditUI *)GetControlByName(szEditLogonPWComfirmControlName);
	if(pEditPass3Control==NULL) return; 
	lstrcpyn(szDesPassword[1],pEditPass3Control->GetText(),CountArray(szDesPassword[1]));

	//加密密码
	TCHAR szEncryptPassword[LEN_PASSWORD]=TEXT("");
	CWHEncrypt::MD5Encrypt(szSrcPassword,szEncryptPassword);

	//变量定义
	CMD_GP_ModifyLogonPass ModifyLogonPass;
	ZeroMemory(&ModifyLogonPass,sizeof(ModifyLogonPass));

	//加密密码
	TCHAR szSrcPasswordMD5[LEN_PASSWORD]=TEXT("");
	TCHAR szDesPasswordMD5[LEN_PASSWORD]=TEXT("");
	CWHEncrypt::MD5Encrypt(szSrcPassword,szSrcPasswordMD5);
	CWHEncrypt::MD5Encrypt(szDesPassword[0],szDesPasswordMD5);

	//构造数据
	ModifyLogonPass.dwUserID=pGlobalUserData->dwUserID;
	lstrcpyn(ModifyLogonPass.szScrPassword,szSrcPasswordMD5,CountArray(ModifyLogonPass.szScrPassword));
	lstrcpyn(ModifyLogonPass.szDesPassword,szDesPasswordMD5,CountArray(ModifyLogonPass.szDesPassword));

	//发送数据
	m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_MODIFY_LOGON_PASS,&ModifyLogonPass,sizeof(ModifyLogonPass));

	return;
}

//银行密码
void CDlgIndividual::OnMissionLinkInsureModify()
{
	//变量定义
	TCHAR szSrcPassword[LEN_PASSWORD];
	TCHAR szDesPassword[2][LEN_PASSWORD];

	//变量定义
	TCHAR szDescribe[128]=TEXT("");
	CUserItemElement * pUserItemElement=CUserItemElement::GetInstance();

	//变量定义
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	//获取密码
	CEditUI * pEditPass1Control = (CEditUI *)GetControlByName(szEditInsurePWOrgControlName);
	if(pEditPass1Control==NULL) return; 
	lstrcpyn(szSrcPassword,pEditPass1Control->GetText(),CountArray(szSrcPassword));

	//获取密码
	CEditUI * pEditPass2Control = (CEditUI *)GetControlByName(szEditInsurePWNewControlName);
	if(pEditPass2Control==NULL) return; 
	lstrcpyn(szDesPassword[0],pEditPass2Control->GetText(),CountArray(szDesPassword[0]));

	//获取密码
	CEditUI * pEditPass3Control = (CEditUI *)GetControlByName(szEditInsurePWComfirmControlName);
	if(pEditPass3Control==NULL) return; 
	lstrcpyn(szDesPassword[1],pEditPass3Control->GetText(),CountArray(szDesPassword[1]));

	//加密密码
	TCHAR szEncryptPassword[LEN_PASSWORD]=TEXT("");
	CWHEncrypt::MD5Encrypt(szSrcPassword,szEncryptPassword);

	//变量定义
	CMD_GP_ModifyInsurePass ModifyInsurePass;
	ZeroMemory(&ModifyInsurePass,sizeof(ModifyInsurePass));

	//加密密码
	TCHAR szSrcPasswordMD5[LEN_PASSWORD]=TEXT("");
	TCHAR szDesPasswordMD5[LEN_PASSWORD]=TEXT("");
	CWHEncrypt::MD5Encrypt(szSrcPassword,szSrcPasswordMD5);
	CWHEncrypt::MD5Encrypt(szDesPassword[0],szDesPasswordMD5);

	//构造数据
	ModifyInsurePass.dwUserID=pGlobalUserData->dwUserID;
	lstrcpyn(ModifyInsurePass.szScrPassword,szSrcPasswordMD5,CountArray(ModifyInsurePass.szScrPassword));
	lstrcpyn(ModifyInsurePass.szDesPassword,szDesPasswordMD5,CountArray(ModifyInsurePass.szDesPassword));

	//发送数据
	m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_MODIFY_INSURE_PASS,&ModifyInsurePass,sizeof(ModifyInsurePass));
	return;
}

//银行密码
void CDlgIndividual::OnMissionLinkInsureSet()
{
	//密码信息
	TCHAR szLogonPass[LEN_PASSWORD]=TEXT("");
	TCHAR szInsurePass1[LEN_PASSWORD]=TEXT("");
	TCHAR szInsurePass2[LEN_PASSWORD]=TEXT("");

	//获取密码
	GetControlItemText(szEditInsurePassControlName,szInsurePass1,CountArray(szInsurePass1));
	GetControlItemText(szEditInsurePass1ControlName,szInsurePass2,CountArray(szInsurePass2));
	GetControlItemText(szEditInsureLogonControlName,szLogonPass,CountArray(szLogonPass));

	//变量定义
	TCHAR szDescribe[128]=TEXT("");
	CUserItemElement * pUserItemElement=CUserItemElement::GetInstance();

	//密码加密
	TCHAR szEnciryptInsurePass[LEN_PASSWORD]=TEXT("");
	CWHEncrypt::MD5Encrypt(szInsurePass1,szEnciryptInsurePass);

	//密码加密
	TCHAR szEnciryptLogonPass[LEN_PASSWORD]=TEXT("");
	CWHEncrypt::MD5Encrypt(szLogonPass,szEnciryptLogonPass);

	//变量定义
	CMD_GP_UserEnableInsure UserEnableInsure;
	ZeroMemory(&UserEnableInsure,sizeof(UserEnableInsure));

	//变量定义
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	//设置变量
	UserEnableInsure.dwUserID=pGlobalUserData->dwUserID;
	lstrcpyn(UserEnableInsure.szLogonPass,szEnciryptLogonPass,CountArray(UserEnableInsure.szLogonPass));
	lstrcpyn(UserEnableInsure.szInsurePass,szEnciryptInsurePass,CountArray(UserEnableInsure.szInsurePass));		
	CWHService::GetMachineIDEx(UserEnableInsure.szMachineID);

	//发送数据
	m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_USER_ENABLE_INSURE,&UserEnableInsure,sizeof(UserEnableInsure));

	return;
}

bool CDlgIndividual::ConcludeMission()
{
	m_byMissionType = byMissionNone;
	m_MissionManager.ConcludeMissionItem(this,false);
	return true;
}

//开始任务
bool CDlgIndividual::AvtiveMission( BYTE byMissionType )
{
	m_byMissionType = byMissionType;
	//发起连接
	if (m_MissionManager.AvtiveMissionItem(this,false)==false)
	{
		//错误提示
		CDlgInformation Information(this);
		Information.ShowMessageBox(TEXT("服务器连接失败，请检查网络配置以及网络连接状况！"),MB_ICONERROR);
		return false;
	}
	return true;
}

void CDlgIndividual::OnMissionLinkError(INT nErrorCode)
{
	switch(m_byMissionType)
	{
	case byMissionIndividualLoad:
		{
			CDlgInformation Information(this); 
			Information.ShowMessageBox(TEXT("连接服务器超时，用户资料获取失败！"),MB_OK|MB_ICONSTOP,30);
			break;
		}
	case byMissionIndividualModify:
		{
			CDlgInformation Information(this);
			Information.ShowMessageBox(TEXT("连接服务器超时，用户资料修改失败！"),MB_OK|MB_ICONSTOP,30);
			break;
		}
	case byMissionInsureModify:
	case byMissionLogonModify:
		{
			CDlgInformation Information(this);
			Information.ShowMessageBox(TEXT("连接服务器超时，密码修改失败！"),MB_OK|MB_ICONSTOP,30);
			break;
		}
	default:
		{
			CDlgInformation Information(this);
			Information.ShowMessageBox(TEXT("尝试了所有的服务器都无法成功连接服务器，请留意网站维护公告！"),MB_ICONERROR);
		}
	}
}

void CDlgIndividual::OnMissionShutError(BYTE cbShutReason)
{
	CDlgInformation Information(this);
	Information.ShowMessageBox(TEXT("由于当前服务器处理繁忙，操作处理失败，请稍后再重试！"),MB_ICONERROR);
}

//更新界面
void CDlgIndividual::UpdateUI()
{
	//获取对象
	CUserItemElement * pUserItemElement = CUserItemElement::GetInstance();
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();
	tagIndividualUserData * pIndividualUserData=pGlobalUserInfo->GetIndividualUserData();

	//用户昵称
	SetControlItemText(szEditNickNameControlName,pGlobalUserData->szNickName);

	//用户帐号
	SetControlItemText(szEditAccountsControlName,pGlobalUserData->szAccounts);
	
	//用户资料	
	TCHAR szText[128]={0};	
	_sntprintf(szText, CountArray(szText), TEXT("%d"), pGlobalUserData->dwGameID);
	SetControlItemText(szEditUserIDControlName,szText);

	//用户等级
	CNumberUI* pNumberUI = static_cast<CNumberUI *>(GetControlByName(szNumberUserLevelControlName));
	if(pNumberUI != NULL)
	{
		pNumberUI->SetNumber(pUserItemElement->GetExperienceLevel(pGlobalUserData->dwExperience));
	}

	//会员等级
	for (int i=0;i<6;i++)
	{
		CStdString strLevel;
		strLevel.SmallFormat(TEXT("%s%d"),szLabelVipLevelControlName,i);
		CControlUI* pLabelUI = GetControlByName(strLevel);
		if(pLabelUI)
		{
			if (i==pGlobalUserData->cbMemberOrder)
			{
				pLabelUI->SetVisible(true);
			}
			else
			{
				pLabelUI->SetVisible(false);
			}
		}
	}

	//用户性别
	CRadioButtonUI* pRadioFemale = static_cast<CRadioButtonUI *>(GetControlByName(szRadioFemaleControlName));
	if (pRadioFemale != NULL)
	{
		pRadioFemale->SetCheck(pGlobalUserData->cbGender == GENDER_FEMALE);
	}
	CRadioButtonUI* pRadioMale = static_cast<CRadioButtonUI *>(GetControlByName(szRadioMankindControlName));
	if(pRadioMale != NULL)
	{
        pRadioMale->SetCheck(pGlobalUserData->cbGender == GENDER_MANKIND);
	}

	//用户签名
	SetControlItemText(szEditSignControlName,pGlobalUserData->szUnderWrite);

	//签名提示
	CEditUI * pEdit = static_cast<CEditUI *>(GetControlByName(szEditSignControlName));
	if (pEdit)
	{
		int nMaxCharNum = 0;
		int nCurrCharNum = 0;
		nCurrCharNum = (int)(_tcslen(pEdit->GetText()));
		nMaxCharNum = pEdit->GetMaxChar();

		TCHAR szText[128]={0};	
		_sntprintf(szText, CountArray(szText), TEXT("(还可以输入%d字)"), nMaxCharNum - nCurrCharNum);

		CLabelUI *pLabelUI = static_cast<CLabelUI *>(GetControlByName(szLabelSignLimitTipControlName));
		if(pLabelUI)
		{
			pLabelUI->SetText(szText);

		}
	}

	//手机号码
	TCHAR szMobilePhone[LEN_MOBILE_PHONE]=TEXT("");
	pUserItemElement->ProtectMobilePhone(pIndividualUserData->szMobilePhone,szMobilePhone,CountArray(szMobilePhone),'*');
	SetControlItemText(szEditMobilePhoneControlName,szMobilePhone);

	//QQ号码
	SetControlItemText(szEditQQControlName,pIndividualUserData->szQQ);

	//电子邮件
	SetControlItemText(szEditEMailControlName,pIndividualUserData->szEMail);

	//详细地址
	SetControlItemText(szszEditAddressControlName,pIndividualUserData->szDwellingPlace);


	//真实姓名
	if (pIndividualUserData->szCompellation[0] != 0)
	{
		TCHAR szCompellation[LEN_COMPELLATION]=TEXT("");
		pUserItemElement->ProtectCompellation(pIndividualUserData->szCompellation,szCompellation,CountArray(szCompellation),'*');
		SetControlItemText(szEditRealNameControlName,szCompellation);
		CControlUI * pControlUI = GetControlByName(szEditRealNameControlName);
		if (pControlUI)
		{
			pControlUI->SetEnabled(false);
		}
	}

	//身份标识
	if(pIndividualUserData->szPassPortID[0] != 0)
	{
		TCHAR szPassPortID[LEN_PASS_PORT_ID]=TEXT("");
		pUserItemElement->ProtectPassPortID(pIndividualUserData->szPassPortID,szPassPortID,CountArray(szPassPortID),'*');
		SetControlItemText(szEditIDNumberControlName,szPassPortID);
		CControlUI *pControlUI = GetControlByName(szEditIDNumberControlName);
		if (pControlUI)
		{
			pControlUI->SetEnabled(false);
		}

		pControlUI = GetControlByName(szContainerRealAuthTipPassControlName);
		if (pControlUI)
		{
			pControlUI->SetVisible(true);
		}
		pControlUI = GetControlByName(szContainerRealAuthTipPassFlagControlName);
		if (pControlUI)
		{
			pControlUI->SetVisible(true);
		}
		pControlUI = GetControlByName(szContainerRealAuthControlName);
		if (pControlUI)
		{
			pControlUI->SetVisible(false);
		}
	}

	//推广标识
	if (pGlobalUserData->szSpreader[0]!= 0)
	{
		SetControlItemText(szEditSpreaderControlName,pGlobalUserData->szSpreader);
		CControlUI * pControlUI = GetControlByName(szEditSpreaderControlName);
		if (pControlUI)
		{
			pControlUI->SetEnabled(false);
		}
	}
}

//更新数据
void CDlgIndividual::UpdateData()
{

	//变量定义
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();
	tagIndividualUserData * pIndividualUserData=pGlobalUserInfo->GetIndividualUserData();
	CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();


	if(m_byMissionType == byMissionLogonModify)
	{
		//密码资料
		TCHAR szDesPassword[LEN_PASSWORD]=TEXT("");
		TCHAR szDesPasswordMD5[LEN_PASSWORD]=TEXT("");
		CEditUI* pEditUI = static_cast<CEditUI *>(GetControlByName(szEditLogonPWComfirmControlName));
		if(pEditUI != NULL)
		{
			lstrcpyn(szDesPassword,pEditUI->GetText(),CountArray(szDesPassword));
		}
		else
		{
			return;
		}
		CWHEncrypt::MD5Encrypt(szDesPassword,szDesPasswordMD5);
		lstrcpyn(pGlobalUserData->szPassword,szDesPasswordMD5,CountArray(pGlobalUserData->szPassword));

		//记录密码
		if (pParameterGlobal->m_bRemberPassword==true)
		{
			//构造信息
			TCHAR szKeyUserID[16];
			_sntprintf(szKeyUserID,CountArray(szKeyUserID),TEXT("%s\\%ld"),REG_USER_INFO,pGlobalUserData->dwUserID);

			//上次用户
			CWHRegKey RegUserInfo;
			RegUserInfo.OpenRegKey(REG_USER_INFO,true);

			//用户表项
			CWHRegKey RegUserItem;
			RegUserItem.OpenRegKey(szKeyUserID,true);

			//保存密码
			TCHAR szPassBuffer[MAX_ENCRYPT_LEN]=TEXT("");
			CWHEncrypt::XorEncrypt(szDesPassword,szPassBuffer,CountArray(szPassBuffer));

			//帐号信息
			RegUserItem.WriteString(TEXT("UserPassword"),szPassBuffer);
			RegUserItem.WriteString(TEXT("UserPassToken"),pGlobalUserData->szPassword);
		}

		return;
	}

	TCHAR szItemText[128] = {0};

	if(m_cbSelectedIndex == 0)
	{
		//用户昵称
		GetControlItemText(szEditNickNameControlName,szItemText,CountArray(szItemText));
		lstrcpyn(pGlobalUserData->szNickName,szItemText,CountArray(pGlobalUserData->szNickName));
		ZeroMemory(szItemText,sizeof(szItemText));

		//性别选择
		CRadioButtonUI * pManButtonUI = static_cast<CRadioButtonUI *>(GetControlByName(szRadioMankindControlName));
		CRadioButtonUI * pFemaleButtonUI = static_cast<CRadioButtonUI *>(GetControlByName(szRadioFemaleControlName));
		if (pManButtonUI && pManButtonUI->GetCheck() ) pGlobalUserData->cbGender=GENDER_MANKIND;
		if (pFemaleButtonUI && pFemaleButtonUI->GetCheck()) pGlobalUserData->cbGender=GENDER_FEMALE;
		
		//用户签名
		GetControlItemText(szEditSignControlName,szItemText,CountArray(szItemText));
		lstrcpyn(pGlobalUserData->szUnderWrite,szItemText,CountArray(pGlobalUserData->szUnderWrite));
		ZeroMemory(szItemText,sizeof(szItemText));

	}
	if(m_cbSelectedIndex == 1)
	{
		//推荐人
		GetControlItemText(szEditSpreaderControlName,szItemText,CountArray(szItemText));
		if (szItemText[0]!=0) 
		{
			lstrcpyn(pGlobalUserData->szSpreader,szItemText,CountArray(pGlobalUserData->szSpreader));
			ZeroMemory(szItemText,sizeof(szItemText));
		}

		//手机号码
		GetControlItemText(szEditMobilePhoneControlName,szItemText,CountArray(szItemText));
		lstrcpyn(pIndividualUserData->szMobilePhone,szItemText,CountArray(pIndividualUserData->szMobilePhone));
		ZeroMemory(szItemText,sizeof(szItemText));

		//Q Q 号码
		GetControlItemText(szEditQQControlName,szItemText,CountArray(szItemText));
		lstrcpyn(pIndividualUserData->szQQ,szItemText,CountArray(pIndividualUserData->szQQ));
		ZeroMemory(szItemText,sizeof(szItemText));

		//电子邮件
		GetControlItemText(szEditEMailControlName,szItemText,CountArray(szItemText));		
		lstrcpyn(pIndividualUserData->szEMail,szItemText,CountArray(pIndividualUserData->szEMail));
		ZeroMemory(szItemText,sizeof(szItemText));

		//详细地址
		GetControlItemText(szszEditAddressControlName,szItemText,CountArray(szItemText));
		lstrcpyn(pIndividualUserData->szDwellingPlace,szItemText,CountArray(pIndividualUserData->szDwellingPlace));
		ZeroMemory(szItemText,sizeof(szItemText));
	}
	if(m_cbSelectedIndex == 3)
	{
		//真实名字
		GetControlItemText(szEditRealNameControlName,szItemText,CountArray(szItemText));
		if (szItemText[0]!=0) 
		{
			lstrcpyn(pIndividualUserData->szCompellation,szItemText,CountArray(pIndividualUserData->szCompellation));
			ZeroMemory(szItemText,sizeof(szItemText));
		}

		//身份证号
		GetControlItemText(szEditIDNumberControlName,szItemText,CountArray(szItemText));
		if (szItemText[0]!=0) 
		{
			lstrcpyn(pIndividualUserData->szPassPortID,szItemText,CountArray(pIndividualUserData->szPassPortID));
			ZeroMemory(szItemText,sizeof(szItemText));
		}
	}

	//发送事件
	CPlatformEvent * pPlatformEvent=CPlatformEvent::GetInstance();
	if (pPlatformEvent!=NULL) pPlatformEvent->SendPlatformEvent(EVENT_USER_INFO_UPDATE,0L);

	return;
}
//设置字符
VOID CDlgIndividual::SetControlItemText(LPCTSTR pszControlName,LPCTSTR pszText)
{
	//玩家帐号
	CControlUI * pControlUI = GetControlByName(pszControlName);
	if(pControlUI) pControlUI->SetText(pszText);
}

//获取字符
VOID CDlgIndividual::GetControlItemText(LPCTSTR pszControlName, TCHAR szString[], WORD wMaxCount)
{
	//玩家帐号
	CControlUI * pControlUI = GetControlByName(pszControlName);
	if(pControlUI==NULL) return;

	//获取字符
	CString strString(pControlUI->GetText());	

	////调整字符
	//strString.TrimLeft();
	//strString.TrimRight();

	//设置结果
	lstrcpyn(szString,strString,wMaxCount);

	return;
}

//设置焦点

VOID CDlgIndividual::SetControlFocus(LPCTSTR pszControlName)
{
	//玩家帐号
	CControlUI * pControlUI = GetControlByName(pszControlName);
	if(pControlUI==NULL) return;
	pControlUI->SetFocus();
	return;
}

//密码强度
VOID CDlgIndividual::SetPassLevel(LPCTSTR szPowerScale, LPCTSTR szPassControl)
{
	//获取密码
	TCHAR szPassword[LEN_PASSWORD]=TEXT("");

	//获取密码
	CControlUI * pEditLogonPass=GetControlByName(szPassControl);
	if(pEditLogonPass!=NULL) lstrcpyn(szPassword, pEditLogonPass->GetText(), CountArray(szPassword));

	//等级判断
	CUserItemElement * pUserItemElement=CUserItemElement::GetInstance();
	BYTE cbPasswordLevel=pUserItemElement->GetPasswordLevel(szPassword);

	//更新窗口
	CPowerScaleUI * pPowerScaleUI = (CPowerScaleUI *)GetControlByName(szPowerScale);
	pPowerScaleUI->SetLevelState(cbPasswordLevel);

	return;
}

//设置页面
void CDlgIndividual::SetTabSelectedIndex(BYTE cbIndex/* = 0*/)
{
	m_cbSelectedIndex = cbIndex;
	return;
}

void CDlgIndividual::UpdateUserFace()
{
	//用户信息
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	CImageUI* pUserAvatar = static_cast<CImageUI*>(GetControlByName(szUserAvatarControlName));
	if(pUserAvatar != NULL)
	{
		//更新头像
		if (pGlobalUserData->CustomFaceInfo.dwDataSize!=0)
		{
			pUserAvatar->SetCustomImage(m_CustomFaceInfo96.dwCustomFace, BIGFACE_CX*BIGFACE_CY);
		}
		else
		{
			TCHAR szBuffer[64]={0};
			WORD wFaceID = pGlobalUserData->wFaceID;
			wsprintf(szBuffer, TEXT("file=\'AVATAR_96_96\\Avatar%d.png\' restype=\'FILE\'"), wFaceID);
			pUserAvatar->SetImage(szBuffer);
		}
	}
}

bool CDlgIndividual::EfficacyStandard()
{
	//用户信息
	CUserItemElement * pUserItemElement=CUserItemElement::GetInstance();
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	TCHAR szItemText[128] = {0};
	TCHAR szDescribe[128]=TEXT("");
	//昵称判断
	GetControlItemText(szEditNickNameControlName,szItemText,CountArray(szItemText));
	if (pUserItemElement->EfficacyNickName(szItemText,szDescribe,CountArray(szDescribe))==false)
	{
		//提示信息
		CDlgInformation Information(this);
		Information.ShowMessageBox(szDescribe,MB_ICONERROR,0);

		//设置焦点
		SetControlFocus(szEditNickNameControlName);
		return false;
	}

	//签名
	ZeroMemory(szItemText,sizeof(szItemText));
	GetControlItemText(szEditSignControlName,szItemText,CountArray(szItemText));
	if (pUserItemElement->EfficacyIllegal(szItemText)==false)
	{
		//提示信息
		CDlgInformation Information(this);
		Information.ShowMessageBox(TEXT("签名包含非法字符串，请重新输入！"),MB_ICONERROR,0);

		//设置焦点
		SetControlFocus(szEditSignControlName);
		return false;
	}


	//密码判断
	ZeroMemory(szItemText,sizeof(szItemText));
	GetControlItemText(szEditStandardLogonPWControlName,szItemText,CountArray(szItemText));
	if (pUserItemElement->EfficacyPassword(szItemText,szDescribe,CountArray(szDescribe))==false)
	{
		//提示信息
		CDlgInformation Information(this);
		Information.ShowMessageBox(szDescribe,MB_ICONERROR,0);

		//设置焦点
		SetControlFocus(szEditStandardLogonPWControlName);

		return false;
	}

	//加密密码
	TCHAR szEncryptPassword[LEN_PASSWORD]=TEXT("");
	CWHEncrypt::MD5Encrypt(szItemText,szEncryptPassword);
	//密码判断
	if (lstrcmp(szEncryptPassword,pGlobalUserData->szPassword)!=0)
	{
		//提示信息
		CDlgInformation Information(this);
		Information.ShowMessageBox(TEXT("登录密码错误，请重新输入！"),MB_ICONERROR,0);

		//设置焦点
		SetControlFocus(szEditStandardLogonPWControlName);

		return false;
	}

	return true;

}

bool CDlgIndividual::EfficacyExtendInfo()
{
	//用户信息
	CUserItemElement * pUserItemElement=CUserItemElement::GetInstance();
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	//变量定义
	TCHAR szItemText[128] = {0};
	TCHAR szDescribe[128]=TEXT("");

	//手机号码
	ZeroMemory(szItemText,sizeof(szItemText));
	GetControlItemText(szEditMobilePhoneControlName,szItemText,CountArray(szItemText));
	if (szItemText[0] !=0)
	{
		if (pUserItemElement->EfficacyMobilePhone(szItemText,szDescribe,CountArray(szDescribe))==false)
		{
			//提示信息
			CDlgInformation Information(this);
			Information.ShowMessageBox(szDescribe,MB_ICONERROR,0);

			//设置焦点
			SetControlFocus(szEditRealNameControlName);
			return false;
		}
	}


	//QQ号码
	ZeroMemory(szItemText,sizeof(szItemText));
	GetControlItemText(szEditQQControlName,szItemText,CountArray(szItemText));
	if (szItemText[0] !=0)
	{
		if (pUserItemElement->EfficacyQQ(szItemText,szDescribe,CountArray(szDescribe))==false)
		{
			//提示信息
			CDlgInformation Information(this);
			Information.ShowMessageBox(szDescribe,MB_ICONERROR,0);

			//设置焦点
			SetControlFocus(szEditQQControlName);
			return false;
		}
	}

	//电子邮箱
	ZeroMemory(szItemText,sizeof(szItemText));
	GetControlItemText(szEditEMailControlName,szItemText,CountArray(szItemText));
	if (szItemText[0] !=0)
	{
		if (pUserItemElement->EfficacyEmail(szItemText,szDescribe,CountArray(szDescribe))==false)
		{
			//提示信息
			CDlgInformation Information(this);
			Information.ShowMessageBox(szDescribe,MB_ICONERROR,0);

			//设置焦点
			SetControlFocus(szEditEMailControlName);
			return false;
		}
	}

	//联系地址
	ZeroMemory(szItemText,sizeof(szItemText));
	GetControlItemText(szszEditAddressControlName,szItemText,CountArray(szItemText));
	if (szItemText[0] !=0)
	{
		if (pUserItemElement->EfficacyIllegal(szItemText)==false)
		{
			//提示信息
			CDlgInformation Information(this);
			Information.ShowMessageBox(TEXT("联系包含非法字符串，请重新输入！"),MB_ICONERROR,0);

			//设置焦点
			SetControlFocus(szszEditAddressControlName);
			return false;
		}
	}

	//密码判断
	ZeroMemory(szItemText,sizeof(szItemText));
	GetControlItemText(szEditRealInfoPWControlName,szItemText,CountArray(szItemText));
	if (pUserItemElement->EfficacyPassword(szItemText,szDescribe,CountArray(szDescribe))==false)
	{
		//提示信息
		CDlgInformation Information(this);
		Information.ShowMessageBox(szDescribe,MB_ICONERROR,0);

		//设置焦点
		SetControlFocus(szEditRealInfoPWControlName);

		return false;
	}

	//加密密码
	TCHAR szEncryptPassword[LEN_PASSWORD]=TEXT("");
	CWHEncrypt::MD5Encrypt(szItemText,szEncryptPassword);
	//密码判断
	if (lstrcmp(szEncryptPassword,pGlobalUserData->szPassword)!=0)
	{
		//提示信息
		CDlgInformation Information(this);
		Information.ShowMessageBox(TEXT("登录密码错误，请重新输入！"),MB_ICONERROR,0);

		//设置焦点
		SetControlFocus(szEditRealInfoPWControlName);

		return false;
	}

	return true;
}

bool CDlgIndividual::EfficacyReal()
{
	//用户信息
	CUserItemElement * pUserItemElement=CUserItemElement::GetInstance();
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();
	//变量定义
	TCHAR szItemText[128] = {0};
	TCHAR szDescribe[128]=TEXT("");

	//真实名字
	GetControlItemText(szEditRealNameControlName,szItemText,CountArray(szItemText));
	if (pUserItemElement->EfficacyCompellation(szItemText,szDescribe,CountArray(szDescribe))==false)
	{
		//提示信息
		CDlgInformation Information(this);
		Information.ShowMessageBox(szDescribe,MB_ICONERROR,0);

		//设置焦点
		SetControlFocus(szEditRealNameControlName);
		return false;
	}

	//身份证号
	ZeroMemory(szItemText,sizeof(szItemText));
	GetControlItemText(szEditIDNumberControlName,szItemText,CountArray(szItemText));
	//身份证号
	if (pUserItemElement->EfficacyPassPortID(szItemText,szDescribe,CountArray(szDescribe))==false)
	{
		//提示信息
		CDlgInformation Information(this);
		Information.ShowMessageBox(szDescribe,MB_ICONERROR,0);

		//设置焦点
		SetControlFocus(szEditIDNumberControlName);
		return false;
	}

	//密码判断
	ZeroMemory(szItemText,sizeof(szItemText));
	GetControlItemText(szEditRealAuthPWControlName,szItemText,CountArray(szItemText));
	if (pUserItemElement->EfficacyPassword(szItemText,szDescribe,CountArray(szDescribe))==false)
	{
		//提示信息
		CDlgInformation Information(this);
		Information.ShowMessageBox(szDescribe,MB_ICONERROR,0);

		//设置焦点
		SetControlFocus(szEditRealAuthPWControlName);

		return false;
	}

	//加密密码
	TCHAR szEncryptPassword[LEN_PASSWORD]=TEXT("");
	CWHEncrypt::MD5Encrypt(szItemText,szEncryptPassword);
	//密码判断
	if (lstrcmp(szEncryptPassword,pGlobalUserData->szPassword)!=0)
	{
		//提示信息
		CDlgInformation Information(this);
		Information.ShowMessageBox(TEXT("登录密码错误，请重新输入！"),MB_ICONERROR,0);

		//设置焦点
		SetControlFocus(szEditRealInfoPWControlName);

		return false;
	}

	return true;
}

bool CDlgIndividual::EfficacyModifyLogonPassward()
{
	//用户信息
	CUserItemElement * pUserItemElement=CUserItemElement::GetInstance();
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();
	//变量定义
	TCHAR szDesPassword[3][LEN_PASSWORD];
	TCHAR szDescribe[128]=TEXT("");

	//获取密码
	GetControlItemText(szEditLogonPWOrgControlName,szDesPassword[0],CountArray(szDesPassword[0]));
	GetControlItemText(szEditLogonPWNewControlName,szDesPassword[1],CountArray(szDesPassword[0]));
	GetControlItemText(szEditLogonPWComfirmControlName,szDesPassword[2],CountArray(szDesPassword[0]));

	////旧密码验证1
	//if (pUserItemElement->EfficacyPassword(szDesPassword[0],szDescribe,CountArray(szDescribe))==false)
	//{
	//	//提示信息
	//	CDlgInformation Information(this);
	//	Information.ShowMessageBox(szDescribe,MB_ICONERROR,0);

	//	//设置焦点
	//	SetControlFocus(szEditLogonPWOrgControlName);

	//	return false;
	//}

	//旧密码验证2
	TCHAR szEncryptPassword[LEN_PASSWORD]=TEXT("");
	CWHEncrypt::MD5Encrypt(szDesPassword[0],szEncryptPassword);
	if (lstrcmp(szEncryptPassword,pGlobalUserData->szPassword)!=0)
	{
		//提示信息
		CDlgInformation Information(this);
		Information.ShowMessageBox(TEXT("旧密码错误，请重新输入！"),MB_ICONERROR,0);

		//设置焦点
		SetControlFocus(szEditLogonPWOrgControlName);

		return false;
	}

	//新密码验证
	if (pUserItemElement->EfficacyPassword(szDesPassword[1],szDescribe,CountArray(szDescribe))==false)
	{
		//提示信息
		CDlgInformation Information(this);
		Information.ShowMessageBox(szDescribe,MB_ICONERROR,0);

		//设置焦点
		SetControlFocus(szEditLogonPWNewControlName);

		return false;
	}

	//确认密码验证
	if (lstrcmp(szDesPassword[1],szDesPassword[2])!=0)
	{
		//提示信息
		CDlgInformation Information(this);
		Information.ShowMessageBox(TEXT("确认密码与新密码不一致，请重新输入！"),MB_ICONERROR,0);

		//设置焦点
		SetControlFocus(szEditLogonPWComfirmControlName);

		return false;
	}

	//一致判断
	if (lstrcmp(szDesPassword[1],szDesPassword[0])==0)
	{
		//提示信息
		CDlgInformation Information(this);
		Information.ShowMessageBox(TEXT("新密码与旧密码一样，无需修改！"),MB_ICONINFORMATION,0);

		return false;
	}

	return true;
}

bool CDlgIndividual::EfficacyModifyInsurePassward()
{
	//用户信息
	CUserItemElement * pUserItemElement=CUserItemElement::GetInstance();
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();
	//变量定义
	TCHAR szDesPassword[3][LEN_PASSWORD];
	TCHAR szDescribe[128]=TEXT("");

	//获取密码
	GetControlItemText(szEditInsurePWOrgControlName,szDesPassword[0],CountArray(szDesPassword[0]));
	GetControlItemText(szEditInsurePWNewControlName,szDesPassword[1],CountArray(szDesPassword[0]));
	GetControlItemText(szEditInsurePWComfirmControlName,szDesPassword[2],CountArray(szDesPassword[0]));

	////旧密码验证2
	//TCHAR szEncryptPassword[LEN_PASSWORD]=TEXT("");
	//CWHEncrypt::MD5Encrypt(szDesPassword[0],szEncryptPassword);
	//if (lstrcmp(szEncryptPassword,pGlobalUserData->szPassword)!=0)
	//{
	//	//提示信息
	//	CDlgInformation Information(this);
	//	Information.ShowMessageBox(TEXT("旧密码错误，请重新输入！"),MB_ICONERROR,0);

	//	//设置焦点
	//	SetControlFocus(szEditInsurePWOrgControlName);

	//	return false;
	//}

	//新密码验证
	if (pUserItemElement->EfficacyPassword(szDesPassword[1],szDescribe,CountArray(szDescribe))==false)
	{
		//提示信息
		CDlgInformation Information(this);
		Information.ShowMessageBox(szDescribe,MB_ICONERROR,0);

		//设置焦点
		SetControlFocus(szEditInsurePWNewControlName);

		return false;
	}

	//确认密码验证
	if (lstrcmp(szDesPassword[1],szDesPassword[2])!=0)
	{
		//提示信息
		CDlgInformation Information(this);
		Information.ShowMessageBox(TEXT("确认密码与新密码不一致，请重新输入！"),MB_ICONERROR,0);

		//设置焦点
		SetControlFocus(szEditInsurePWComfirmControlName);

		return false;
	}

	//一致判断
	if (lstrcmp(szDesPassword[1],szDesPassword[0])==0)
	{
		//提示信息
		CDlgInformation Information(this);
		Information.ShowMessageBox(TEXT("新密码与旧密码一样，无需修改！"),MB_ICONINFORMATION,0);

		return false;
	}

	//验证相同
	TCHAR szEncryptPassword[LEN_PASSWORD]=TEXT("");
	CWHEncrypt::MD5Encrypt(szDesPassword[1],szEncryptPassword);
	if (lstrcmp(szEncryptPassword,pGlobalUserData->szPassword)==0)
	{
		//提示信息
		CDlgInformation Information(this);
		Information.ShowMessageBox(TEXT("新密码不能与登录密码相同！"),MB_ICONINFORMATION,0);

		return false;
	}

	return true;
}

bool CDlgIndividual::EfficacyOpenInsurePassward()
{
	//用户信息
	CUserItemElement * pUserItemElement=CUserItemElement::GetInstance();
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();
	//变量定义
	TCHAR szDesPassword[3][LEN_PASSWORD];
	TCHAR szDescribe[128]=TEXT("");

	//获取密码
	GetControlItemText(szEditInsureLogonControlName,szDesPassword[0],CountArray(szDesPassword[0]));
	GetControlItemText(szEditInsurePassControlName,szDesPassword[1],CountArray(szDesPassword[0]));
	GetControlItemText(szEditInsurePass1ControlName,szDesPassword[2],CountArray(szDesPassword[0]));



	//新密码验证
	if (pUserItemElement->EfficacyPassword(szDesPassword[1],szDescribe,CountArray(szDescribe))==false)
	{
		//提示信息
		CDlgInformation Information(this);
		Information.ShowMessageBox(szDescribe,MB_ICONERROR,0);

		//设置焦点
		SetControlFocus(szEditInsurePassControlName);

		return false;
	}

	//确认密码验证
	if (lstrcmp(szDesPassword[1],szDesPassword[2])!=0)
	{
		//提示信息
		CDlgInformation Information(this);
		Information.ShowMessageBox(TEXT("确认密码与新密码不一致，请重新输入！"),MB_ICONERROR,0);

		//设置焦点
		SetControlFocus(szEditInsurePass1ControlName);

		return false;
	}

	//登录密码验证
	if (pUserItemElement->EfficacyPassword(szDesPassword[0],szDescribe,CountArray(szDescribe))==false)
	{
		//提示信息
		CDlgInformation Information(this);
		Information.ShowMessageBox(szDescribe,MB_ICONERROR,0);

		//设置焦点
		SetControlFocus(szEditInsureLogonControlName);

		return false;
	}

	//登录密码验证
	TCHAR szEncryptPassword[LEN_PASSWORD]=TEXT("");
	CWHEncrypt::MD5Encrypt(szDesPassword[0],szEncryptPassword);
	if (lstrcmp(szEncryptPassword,pGlobalUserData->szPassword)!=0)
	{
		//提示信息
		CDlgInformation Information(this);
		Information.ShowMessageBox(TEXT("登录密码错误，请重新输入！"),MB_ICONERROR,0);

		//设置焦点
		SetControlFocus(szEditInsureLogonControlName);

		return false;
	}

	//验证相同
	if (lstrcmp(szDesPassword[1],szDesPassword[0])==0)
	{
		//提示信息
		CDlgInformation Information(this);
		Information.ShowMessageBox(TEXT("开通银行密码不能与登录密码相同！"),MB_ICONINFORMATION,0);

		return false;
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////////////