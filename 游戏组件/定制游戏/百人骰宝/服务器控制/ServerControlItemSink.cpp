#include "StdAfx.h"
#include "servercontrolitemsink.h"


CServerControlItemSink::CServerControlItemSink(void)
{
	m_cbExcuteTimes = 0;								
	m_cbControlStyle = 0;	
	m_cbEnemyChairID=0xFF;
}

CServerControlItemSink::~CServerControlItemSink( void )
{

}

//����������
bool  CServerControlItemSink::ServerControl(WORD wSubCmdID, const void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem, ITableFrame * pITableFrame)
{
	//��������й���ԱȨ�� �򷵻ش���
//#ifndef _DEBUG
//	if( !CUserRight::IsGameCheatUser(pIServerUserItem->GetUserRight()) )
//	{
//		return false;
//	}
//#endif
//
//	m_cbEnemyChairID = (pIServerUserItem->GetChairID()+1)%GAME_PLAYER;
//
//	const CMD_C_AdminReq* AdminReq=static_cast<const CMD_C_AdminReq*>(pDataBuffer);
//	switch(AdminReq->cbReqType)
//	{
//	case RQ_RESET_CONTROL:
//		{	
//			m_cbControlStyle=0;
//			m_cbExcuteTimes=0;
//			
//			CMD_S_CommandResult cResult;
//			cResult.cbResult=CR_ACCEPT;
//			cResult.cbAckType=ACK_RESET_CONTROL;
//			pITableFrame->SendUserItemData(pIServerUserItem,SUB_S_AMDIN_COMMAND,&cResult,sizeof(CMD_S_CommandResult));
//			break;
//		}
//	case RQ_SET_WIN_AREA:
//		{
//
//			const tagAdminReq*pAdminReq=reinterpret_cast<const tagAdminReq*>(AdminReq->cbExtendData);
//			switch(pAdminReq->m_cbControlStyle)
//			{
//			
//			case CS_ENEMY_LOSE:	//�Լ�����
//				{
//					m_cbControlStyle=pAdminReq->m_cbControlStyle;
//					m_cbExcuteTimes=pAdminReq->m_cbExcuteTimes;
//					CMD_S_CommandResult cResult;
//					cResult.cbResult=CR_ACCEPT;
//					cResult.cbAckType=ACK_SET_WIN_AREA;
//					pITableFrame->SendUserItemData(pIServerUserItem,SUB_S_AMDIN_COMMAND,&cResult,sizeof(CMD_S_CommandResult));
//
//					break;
//				}
//			case CS_ENEMY_WIN:	//�Լ�Ӯ��
//				{
//					m_cbControlStyle=pAdminReq->m_cbControlStyle;
//					m_cbExcuteTimes=pAdminReq->m_cbExcuteTimes;
//					CMD_S_CommandResult cResult;
//					cResult.cbResult=CR_ACCEPT;
//					cResult.cbAckType=ACK_SET_WIN_AREA;
//					pITableFrame->SendUserItemData(pIServerUserItem,SUB_S_AMDIN_COMMAND,&cResult,sizeof(CMD_S_CommandResult));
//					break;
//				}
//			default:	//�ܾ�����
//				{
//					CMD_S_CommandResult cResult;
//					cResult.cbResult=CR_REFUSAL;
//					cResult.cbAckType=ACK_SET_WIN_AREA;
//					pITableFrame->SendUserItemData(pIServerUserItem,SUB_S_AMDIN_COMMAND,&cResult,sizeof(CMD_S_CommandResult));
//					break;
//				}
//			}
//
//			break;
//		}
//	case RQ_PRINT_SYN:
//		{
//			CMD_S_CommandResult cResult;
//			cResult.cbResult=CR_ACCEPT;
//			cResult.cbAckType=ACK_PRINT_SYN;
//			tagAdminReq*pAdminReq=reinterpret_cast<tagAdminReq*>(cResult.cbExtendData);
//			pAdminReq->m_cbControlStyle=m_cbControlStyle;
//			pAdminReq->m_cbExcuteTimes=m_cbExcuteTimes;
//			pITableFrame->SendUserItemData(pIServerUserItem,SUB_S_AMDIN_COMMAND,&cResult,sizeof(CMD_S_CommandResult));
//			break;
//		}
//	default:
//		{
//			break;
//		}
//	}
	return true;
}

//��Ҫ����
bool  CServerControlItemSink::NeedControl()
{
	
	if(m_cbControlStyle > 0 && m_cbExcuteTimes > 0)
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
	if (m_cbExcuteTimes > 0)
		m_cbExcuteTimes--;	

	if ( m_cbExcuteTimes == 0)
	{
		m_cbExcuteTimes = 0;								
		m_cbControlStyle = 0;	
	}

	return true;
}
//���ؿ�������
bool  CServerControlItemSink::ReturnControlArea( tagControlInfo& ControlInfo )
{
	return true;
}

//��������
void  CServerControlItemSink::GetSuitResult( BYTE cbTableCardArray[][2] )
{
		
}
