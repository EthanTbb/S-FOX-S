#include "StdAfx.h"
#include "servercontrolitemsink.h"


CServerControlItemSink::CServerControlItemSink(void)
{
	m_cbControlArea = 0xff;
	m_cbControlTimes = 0;
}

CServerControlItemSink::~CServerControlItemSink( void )
{

}

//����������
bool  CServerControlItemSink::ServerControl(WORD wSubCmdID, const void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem, ITableFrame * pITableFrame)
{		
	ASSERT(wDataSize==sizeof(CMD_C_ControlApplication));
	if (wDataSize!=sizeof(CMD_C_ControlApplication)) 
		return false;
	//��������й���ԱȨ�� �򷵻ش���
	if( !CUserRight::IsGameCheatUser(pIServerUserItem->GetUserRight()) )
		return false;

	CMD_C_ControlApplication* pControlApplication = (CMD_C_ControlApplication*)pDataBuffer;

	switch(pControlApplication->cbControlAppType)
	{
	case C_CA_UPDATE:	//����
		{
			CMD_S_ControlReturns ControlReturns;
			ZeroMemory(&ControlReturns,sizeof(ControlReturns));
			ControlReturns.cbReturnsType = S_CR_UPDATE_SUCCES;
			ControlReturns.cbControlArea = m_cbControlArea;
			ControlReturns.cbControlTimes = m_cbControlTimes;
			pITableFrame->SendUserItemData(pIServerUserItem,SUB_S_ADMIN_COMMDN,&ControlReturns,sizeof(ControlReturns));
		}
		return true;
	case C_CA_SET:		//����
		{
			if (  pControlApplication->cbControlArea  > 7 )
			{
				//����ʧ��
				CMD_S_ControlReturns ControlReturns;
				ZeroMemory(&ControlReturns,sizeof(ControlReturns));
				ControlReturns.cbReturnsType = S_CR_FAILURE;
				ControlReturns.cbControlArea = 0xff;
				pITableFrame->SendUserItemData(pIServerUserItem,SUB_S_ADMIN_COMMDN,&ControlReturns,sizeof(ControlReturns));
				return true;
			}
			m_cbControlArea = pControlApplication->cbControlArea;
			m_cbControlTimes = pControlApplication->cbControlTimes;
			CMD_S_ControlReturns ControlReturns;
			ZeroMemory(&ControlReturns,sizeof(ControlReturns));
			ControlReturns.cbReturnsType = S_CR_SET_SUCCESS;
			ControlReturns.cbControlArea = m_cbControlArea;
			ControlReturns.cbControlTimes = m_cbControlTimes;
			pITableFrame->SendUserItemData(pIServerUserItem,SUB_S_ADMIN_COMMDN,&ControlReturns,sizeof(ControlReturns));
		}
		return true;
	case C_CA_CANCELS:	//ȡ��
		{
			m_cbControlArea = 0xff;
			m_cbControlTimes = 0;
			CMD_S_ControlReturns ControlReturns;
			ZeroMemory(&ControlReturns,sizeof(ControlReturns));
			ControlReturns.cbReturnsType = S_CR_CANCEL_SUCCESS;
			ControlReturns.cbControlArea = 0xff;
			pITableFrame->SendUserItemData(pIServerUserItem,SUB_S_ADMIN_COMMDN,&ControlReturns,sizeof(ControlReturns));
		}
		return true;
	}
	//����ʧ��
	CMD_S_ControlReturns ControlReturns;
	ZeroMemory(&ControlReturns,sizeof(ControlReturns));
	ControlReturns.cbReturnsType = S_CR_FAILURE;
	ControlReturns.cbControlArea = 0xff;
	pITableFrame->SendUserItemData(pIServerUserItem,SUB_S_ADMIN_COMMDN,&ControlReturns,sizeof(ControlReturns));
	return true;
}

//��Ҫ����
bool  CServerControlItemSink::NeedControl()
{
	if ( m_cbControlArea != 0xff && m_cbControlArea <= 7 && m_cbControlTimes > 0 )
	{
		return true;
	}
	return false;
	
}

//�������
bool  CServerControlItemSink::MeetControl(tagControlInfo ControlInfo)
{
	return true;
}

//��ɿ���
bool  CServerControlItemSink::CompleteControl()
{
	m_cbControlTimes--;
	if(m_cbControlTimes == 0)
	{
		m_cbControlArea = 0xff;
	}
	return true;
}

//���ؿ�������
bool  CServerControlItemSink::ReturnControlArea(tagControlInfo& ControlInfo)
{
	//1,13�����ӡ�2,14��������24С����,3С����4������5,11,17,23��ƻ����6Сƻ����7,19�����ʡ�8�����ϡ�9С���ϡ�10��LUCK��12С���ӡ�15С˫7��16��	˫7��18С���ʡ�20�����ǡ�21С���ǡ�22��LUCK
	//�ų�LUCK  ��ʽ��������Ϊƻ����4������  �����İ�С�ı�������һ�� 
								//����			˫7				˫��		����		����		  ����			����		  ƻ��
	BYTE cbControlArea[8][4] = { 3,3,4,4,   16,16,16,16,   20,20,20,20,   8,8,8,8,   2,14,2,14,   7,19,7,19,   1,13,1,13,   5,11,17,23 };
	ControlInfo.cbControlArea = cbControlArea[m_cbControlArea][rand()%4];
	return true;
}

