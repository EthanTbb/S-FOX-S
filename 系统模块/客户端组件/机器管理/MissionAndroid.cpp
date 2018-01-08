#include "StdAfx.h"
#include "MissionAndroid.h"
#include "ServerListData.h"
#include "PlatformFrame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////////////

//激活任务
#define IDI_AVTIVE_MISSION				100								//激活任务
#define TIME_AVTIVE_MISSION				600000L							//激活任务

//////////////////////////////////////////////////////////////////////////////////
//静态变量

CMissionAndroid * CMissionAndroid::m_pMissionAndroid=NULL;						//对象指针

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CMissionAndroid, CWnd)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CMissionAndroid::CMissionAndroid()
{
	//存储变量
	m_wDataSize=0;
	ZeroMemory(m_cbDataBuffer,sizeof(m_cbDataBuffer));

	//设置变量
	m_AndroidAction = AAction_NULL;
	m_pIAndroidOperateCenter=NULL;	

	//设置对象
	ASSERT(m_pMissionAndroid==NULL);
	if (m_pMissionAndroid==NULL) m_pMissionAndroid=this;

	return;
}

//析构函数
CMissionAndroid::~CMissionAndroid()
{
	//释放对象
	ASSERT(m_pMissionAndroid==this);
	if (m_pMissionAndroid==this) m_pMissionAndroid=NULL;

	return;
}

//连接事件
bool CMissionAndroid::OnEventMissionLink(INT nErrorCode)
{
	//错误判断
	if (nErrorCode!=0L)
	{
		//创建窗口
		if (m_hWnd==NULL)
		{
			CRect rcCreate(0,0,0,0);
			Create(NULL,NULL,WS_CHILD,rcCreate,GetDesktopWindow(),100);
		}

		//设置控件
		if(m_pIAndroidOperateCenter!=NULL)
		{
			m_pIAndroidOperateCenter->OnEventEnableControls(TRUE);
		}

		//显示提示
		CInformation Information(CPlatformFrame::GetInstance());
		Information.ShowMessageBox(TEXT("与服务器的连接已经断开,请重新登录！"),MB_ICONERROR);
	}
	else
	{
		//获取参数
		if(m_AndroidAction==AAction_GetParameter)
		{
			//发送数据
			ASSERT(GetMissionManager()!=NULL);
			GetMissionManager()->SendData(MDM_GP_ANDROID_SERVICE,SUB_GP_GET_PARAMETER,m_cbDataBuffer,m_wDataSize);
		}

		//添加参数
		if(m_AndroidAction==AAction_AddParameter)
		{
			//发送数据
			ASSERT(GetMissionManager()!=NULL);
			GetMissionManager()->SendData(MDM_GP_ANDROID_SERVICE,SUB_GP_ADD_PARAMETER,m_cbDataBuffer,m_wDataSize);
		}

		//修改参数
		if(m_AndroidAction==AAction_ModifyParameter)
		{
			//发送数据
			ASSERT(GetMissionManager()!=NULL);
			GetMissionManager()->SendData(MDM_GP_ANDROID_SERVICE,SUB_GP_MODIFY_PARAMETER,m_cbDataBuffer,m_wDataSize);
		}

		//删除参数
		if(m_AndroidAction==AAction_DeleteParameter)
		{
			//发送数据
			ASSERT(GetMissionManager()!=NULL);
			GetMissionManager()->SendData(MDM_GP_ANDROID_SERVICE,SUB_GP_DELETE_PARAMETER,m_cbDataBuffer,m_wDataSize);
		}

		//设置变量
		m_wDataSize=0;
		m_AndroidAction=AAction_NULL;
		ZeroMemory(m_cbDataBuffer,sizeof(m_cbDataBuffer));

		return true;
	}

	return true;
}

//关闭事件
bool CMissionAndroid::OnEventMissionShut(BYTE cbShutReason)
{
	//设置变量
	m_wDataSize=0;
	m_AndroidAction=AAction_NULL;
	ZeroMemory(m_cbDataBuffer,sizeof(m_cbDataBuffer));

	//设置控件
	if(m_pIAndroidOperateCenter!=NULL)
	{
		m_pIAndroidOperateCenter->OnEventEnableControls(TRUE);
	}

	return true;
}

//读取事件
bool CMissionAndroid::OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize)
{
	if (Command.wMainCmdID==MDM_GP_ANDROID_SERVICE)
	{
		switch (Command.wSubCmdID)
		{
		case SUB_GP_ANDROID_PARAMETER:		//机器参数
			{
				//参数校验
				ASSERT(wDataSize<=sizeof(CMD_GP_AndroidParameter));
				if(wDataSize>sizeof(CMD_GP_AndroidParameter)) return false;

				//提取变量
				CMD_GP_AndroidParameter * pAndroidParameter = (CMD_GP_AndroidParameter *)pData;
				ASSERT(pAndroidParameter!=NULL);

				//接口回调
				if(m_pIAndroidOperateCenter!=NULL)
				{
					m_pIAndroidOperateCenter->OnEventEnableControls(TRUE);
					m_pIAndroidOperateCenter->OnEventAndroidParenter(pAndroidParameter->wSubCommdID,pAndroidParameter->wParameterCount,pAndroidParameter->AndroidParameter);
				}

				return true;
			}
		case SUB_GP_OPERATE_FAILURE:	//操作失败
			{
				//参数校验
				ASSERT(sizeof(CMD_GP_OperateFailure)>=wDataSize);
				if(sizeof(CMD_GP_OperateFailure)<wDataSize) return false;

				//提取数据
				CMD_GP_OperateFailure * pOperateFailure = (CMD_GP_OperateFailure *)pData;

				//控件设置
				if(m_pIAndroidOperateCenter!=NULL)
				{
					m_pIAndroidOperateCenter->OnEventEnableControls(TRUE);
				}

				//弹出提示
				CInformation Information;
				Information.ShowMessageBox(pOperateFailure->szDescribeString,MB_ICONERROR);

				return true;
			}
		}
	}

	return true;
}

//设置接口
VOID CMissionAndroid::SetAndroidOperateCenter(IAndroidOperateCenter * pIAndroidOperateCenter)
{
	//参数校验
	ASSERT(pIAndroidOperateCenter!=NULL);

	//设置接口
	m_pIAndroidOperateCenter=pIAndroidOperateCenter;
}

//获取参数
VOID CMissionAndroid::GetAndroidParameter(WORD wServerID)
{
	//获取对象
	CMD_GP_GetParameter * pGetParameter = (CMD_GP_GetParameter *)m_cbDataBuffer;

	//设置变量
	pGetParameter->wServerID=wServerID;
	m_AndroidAction=AAction_GetParameter;
	m_wDataSize=sizeof(CMD_GP_GetParameter);

	//激活任务
	CMissionManager * pMissionManager=GetMissionManager();
	if (pMissionManager!=NULL) pMissionManager->AvtiveMissionItem(this,false);

	//设置控件
	if(m_pIAndroidOperateCenter!=NULL)
	{
		m_pIAndroidOperateCenter->OnEventEnableControls(FALSE);
	}

	return;
}

//删除参数
VOID CMissionAndroid::DeleteAndroidParameter(WORD wServerID,DWORD dwBatchID)
{
	//获取对象
	CMD_GP_DeleteParameter * pDeleteParameter = (CMD_GP_DeleteParameter *)m_cbDataBuffer;

	//设置变量
	pDeleteParameter->wServerID=wServerID;
	pDeleteParameter->dwBatchID=dwBatchID;
	m_AndroidAction=AAction_DeleteParameter;
	m_wDataSize=sizeof(CMD_GP_DeleteParameter);

	//激活任务
	CMissionManager * pMissionManager=GetMissionManager();
	if (pMissionManager!=NULL) pMissionManager->AvtiveMissionItem(this,false);

	//设置控件
	if(m_pIAndroidOperateCenter!=NULL)
	{
		m_pIAndroidOperateCenter->OnEventEnableControls(FALSE);
	}

	return;
}

//修改参数
VOID CMissionAndroid::ModifyAndroidParameter(WORD wServerID,tagAndroidParameter * pAndroidParameter)
{
	//获取对象
	CMD_GP_ModifyParameter * pModifyParameter = (CMD_GP_ModifyParameter *)m_cbDataBuffer;

	//设置变量
	m_AndroidAction=AAction_ModifyParameter;
	m_wDataSize=sizeof(CMD_GP_ModifyParameter);
	pModifyParameter->wServerID=wServerID;
	CopyMemory(&pModifyParameter->AndroidParameter,pAndroidParameter,sizeof(tagAndroidParameter));	

	//激活任务
	CMissionManager * pMissionManager=GetMissionManager();
	if (pMissionManager!=NULL) pMissionManager->AvtiveMissionItem(this,false);

	//设置控件
	if(m_pIAndroidOperateCenter!=NULL)
	{
		m_pIAndroidOperateCenter->OnEventEnableControls(FALSE);
	}

	return;
}

//添加参数
VOID CMissionAndroid::AddAndroidParameter(WORD wServerID,tagAndroidParameter * pAndroidParameter)
{
	//获取对象
	CMD_GP_AddParameter * pAddParameter = (CMD_GP_AddParameter *)m_cbDataBuffer;

	//设置变量
	pAddParameter->wServerID=wServerID;
	m_AndroidAction=AAction_AddParameter;
	m_wDataSize=sizeof(CMD_GP_AddParameter);
	CopyMemory(&pAddParameter->AndroidParameter,pAndroidParameter,sizeof(tagAndroidParameter));	

	//激活任务
	CMissionManager * pMissionManager=GetMissionManager();
	if (pMissionManager!=NULL) pMissionManager->AvtiveMissionItem(this,false);

	//设置控件
	if(m_pIAndroidOperateCenter!=NULL)
	{
		m_pIAndroidOperateCenter->OnEventEnableControls(FALSE);
	}

	return;
}

//////////////////////////////////////////////////////////////////////////////////
