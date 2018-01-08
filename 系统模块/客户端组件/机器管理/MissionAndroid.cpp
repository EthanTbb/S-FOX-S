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

//��������
#define IDI_AVTIVE_MISSION				100								//��������
#define TIME_AVTIVE_MISSION				600000L							//��������

//////////////////////////////////////////////////////////////////////////////////
//��̬����

CMissionAndroid * CMissionAndroid::m_pMissionAndroid=NULL;						//����ָ��

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CMissionAndroid, CWnd)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CMissionAndroid::CMissionAndroid()
{
	//�洢����
	m_wDataSize=0;
	ZeroMemory(m_cbDataBuffer,sizeof(m_cbDataBuffer));

	//���ñ���
	m_AndroidAction = AAction_NULL;
	m_pIAndroidOperateCenter=NULL;	

	//���ö���
	ASSERT(m_pMissionAndroid==NULL);
	if (m_pMissionAndroid==NULL) m_pMissionAndroid=this;

	return;
}

//��������
CMissionAndroid::~CMissionAndroid()
{
	//�ͷŶ���
	ASSERT(m_pMissionAndroid==this);
	if (m_pMissionAndroid==this) m_pMissionAndroid=NULL;

	return;
}

//�����¼�
bool CMissionAndroid::OnEventMissionLink(INT nErrorCode)
{
	//�����ж�
	if (nErrorCode!=0L)
	{
		//��������
		if (m_hWnd==NULL)
		{
			CRect rcCreate(0,0,0,0);
			Create(NULL,NULL,WS_CHILD,rcCreate,GetDesktopWindow(),100);
		}

		//���ÿؼ�
		if(m_pIAndroidOperateCenter!=NULL)
		{
			m_pIAndroidOperateCenter->OnEventEnableControls(TRUE);
		}

		//��ʾ��ʾ
		CInformation Information(CPlatformFrame::GetInstance());
		Information.ShowMessageBox(TEXT("��������������Ѿ��Ͽ�,�����µ�¼��"),MB_ICONERROR);
	}
	else
	{
		//��ȡ����
		if(m_AndroidAction==AAction_GetParameter)
		{
			//��������
			ASSERT(GetMissionManager()!=NULL);
			GetMissionManager()->SendData(MDM_GP_ANDROID_SERVICE,SUB_GP_GET_PARAMETER,m_cbDataBuffer,m_wDataSize);
		}

		//��Ӳ���
		if(m_AndroidAction==AAction_AddParameter)
		{
			//��������
			ASSERT(GetMissionManager()!=NULL);
			GetMissionManager()->SendData(MDM_GP_ANDROID_SERVICE,SUB_GP_ADD_PARAMETER,m_cbDataBuffer,m_wDataSize);
		}

		//�޸Ĳ���
		if(m_AndroidAction==AAction_ModifyParameter)
		{
			//��������
			ASSERT(GetMissionManager()!=NULL);
			GetMissionManager()->SendData(MDM_GP_ANDROID_SERVICE,SUB_GP_MODIFY_PARAMETER,m_cbDataBuffer,m_wDataSize);
		}

		//ɾ������
		if(m_AndroidAction==AAction_DeleteParameter)
		{
			//��������
			ASSERT(GetMissionManager()!=NULL);
			GetMissionManager()->SendData(MDM_GP_ANDROID_SERVICE,SUB_GP_DELETE_PARAMETER,m_cbDataBuffer,m_wDataSize);
		}

		//���ñ���
		m_wDataSize=0;
		m_AndroidAction=AAction_NULL;
		ZeroMemory(m_cbDataBuffer,sizeof(m_cbDataBuffer));

		return true;
	}

	return true;
}

//�ر��¼�
bool CMissionAndroid::OnEventMissionShut(BYTE cbShutReason)
{
	//���ñ���
	m_wDataSize=0;
	m_AndroidAction=AAction_NULL;
	ZeroMemory(m_cbDataBuffer,sizeof(m_cbDataBuffer));

	//���ÿؼ�
	if(m_pIAndroidOperateCenter!=NULL)
	{
		m_pIAndroidOperateCenter->OnEventEnableControls(TRUE);
	}

	return true;
}

//��ȡ�¼�
bool CMissionAndroid::OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize)
{
	if (Command.wMainCmdID==MDM_GP_ANDROID_SERVICE)
	{
		switch (Command.wSubCmdID)
		{
		case SUB_GP_ANDROID_PARAMETER:		//��������
			{
				//����У��
				ASSERT(wDataSize<=sizeof(CMD_GP_AndroidParameter));
				if(wDataSize>sizeof(CMD_GP_AndroidParameter)) return false;

				//��ȡ����
				CMD_GP_AndroidParameter * pAndroidParameter = (CMD_GP_AndroidParameter *)pData;
				ASSERT(pAndroidParameter!=NULL);

				//�ӿڻص�
				if(m_pIAndroidOperateCenter!=NULL)
				{
					m_pIAndroidOperateCenter->OnEventEnableControls(TRUE);
					m_pIAndroidOperateCenter->OnEventAndroidParenter(pAndroidParameter->wSubCommdID,pAndroidParameter->wParameterCount,pAndroidParameter->AndroidParameter);
				}

				return true;
			}
		case SUB_GP_OPERATE_FAILURE:	//����ʧ��
			{
				//����У��
				ASSERT(sizeof(CMD_GP_OperateFailure)>=wDataSize);
				if(sizeof(CMD_GP_OperateFailure)<wDataSize) return false;

				//��ȡ����
				CMD_GP_OperateFailure * pOperateFailure = (CMD_GP_OperateFailure *)pData;

				//�ؼ�����
				if(m_pIAndroidOperateCenter!=NULL)
				{
					m_pIAndroidOperateCenter->OnEventEnableControls(TRUE);
				}

				//������ʾ
				CInformation Information;
				Information.ShowMessageBox(pOperateFailure->szDescribeString,MB_ICONERROR);

				return true;
			}
		}
	}

	return true;
}

//���ýӿ�
VOID CMissionAndroid::SetAndroidOperateCenter(IAndroidOperateCenter * pIAndroidOperateCenter)
{
	//����У��
	ASSERT(pIAndroidOperateCenter!=NULL);

	//���ýӿ�
	m_pIAndroidOperateCenter=pIAndroidOperateCenter;
}

//��ȡ����
VOID CMissionAndroid::GetAndroidParameter(WORD wServerID)
{
	//��ȡ����
	CMD_GP_GetParameter * pGetParameter = (CMD_GP_GetParameter *)m_cbDataBuffer;

	//���ñ���
	pGetParameter->wServerID=wServerID;
	m_AndroidAction=AAction_GetParameter;
	m_wDataSize=sizeof(CMD_GP_GetParameter);

	//��������
	CMissionManager * pMissionManager=GetMissionManager();
	if (pMissionManager!=NULL) pMissionManager->AvtiveMissionItem(this,false);

	//���ÿؼ�
	if(m_pIAndroidOperateCenter!=NULL)
	{
		m_pIAndroidOperateCenter->OnEventEnableControls(FALSE);
	}

	return;
}

//ɾ������
VOID CMissionAndroid::DeleteAndroidParameter(WORD wServerID,DWORD dwBatchID)
{
	//��ȡ����
	CMD_GP_DeleteParameter * pDeleteParameter = (CMD_GP_DeleteParameter *)m_cbDataBuffer;

	//���ñ���
	pDeleteParameter->wServerID=wServerID;
	pDeleteParameter->dwBatchID=dwBatchID;
	m_AndroidAction=AAction_DeleteParameter;
	m_wDataSize=sizeof(CMD_GP_DeleteParameter);

	//��������
	CMissionManager * pMissionManager=GetMissionManager();
	if (pMissionManager!=NULL) pMissionManager->AvtiveMissionItem(this,false);

	//���ÿؼ�
	if(m_pIAndroidOperateCenter!=NULL)
	{
		m_pIAndroidOperateCenter->OnEventEnableControls(FALSE);
	}

	return;
}

//�޸Ĳ���
VOID CMissionAndroid::ModifyAndroidParameter(WORD wServerID,tagAndroidParameter * pAndroidParameter)
{
	//��ȡ����
	CMD_GP_ModifyParameter * pModifyParameter = (CMD_GP_ModifyParameter *)m_cbDataBuffer;

	//���ñ���
	m_AndroidAction=AAction_ModifyParameter;
	m_wDataSize=sizeof(CMD_GP_ModifyParameter);
	pModifyParameter->wServerID=wServerID;
	CopyMemory(&pModifyParameter->AndroidParameter,pAndroidParameter,sizeof(tagAndroidParameter));	

	//��������
	CMissionManager * pMissionManager=GetMissionManager();
	if (pMissionManager!=NULL) pMissionManager->AvtiveMissionItem(this,false);

	//���ÿؼ�
	if(m_pIAndroidOperateCenter!=NULL)
	{
		m_pIAndroidOperateCenter->OnEventEnableControls(FALSE);
	}

	return;
}

//��Ӳ���
VOID CMissionAndroid::AddAndroidParameter(WORD wServerID,tagAndroidParameter * pAndroidParameter)
{
	//��ȡ����
	CMD_GP_AddParameter * pAddParameter = (CMD_GP_AddParameter *)m_cbDataBuffer;

	//���ñ���
	pAddParameter->wServerID=wServerID;
	m_AndroidAction=AAction_AddParameter;
	m_wDataSize=sizeof(CMD_GP_AddParameter);
	CopyMemory(&pAddParameter->AndroidParameter,pAndroidParameter,sizeof(tagAndroidParameter));	

	//��������
	CMissionManager * pMissionManager=GetMissionManager();
	if (pMissionManager!=NULL) pMissionManager->AvtiveMissionItem(this,false);

	//���ÿؼ�
	if(m_pIAndroidOperateCenter!=NULL)
	{
		m_pIAndroidOperateCenter->OnEventEnableControls(FALSE);
	}

	return;
}

//////////////////////////////////////////////////////////////////////////////////
