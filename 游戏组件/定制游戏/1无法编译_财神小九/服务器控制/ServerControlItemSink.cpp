#include "StdAfx.h"
#include "servercontrolitemsink.h"


CServerControlItemSink::CServerControlItemSink(void)
{
	m_cbExcuteTimes = 0;								
	ZeroMemory(m_cbControlArea, sizeof(m_cbControlArea));
}

CServerControlItemSink::~CServerControlItemSink( void )
{

}

//服务器控制
bool  CServerControlItemSink::ServerControl(WORD wSubCmdID, const void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem, ITableFrame * pITableFrame)
{
	//如果不具有管理员权限 则返回错误
	if( !CUserRight::IsGameCheatUser(pIServerUserItem->GetUserRight()) )
	{
		return false;
	}

	// 消息装换
	const CMD_C_AdminControl* pAdminControl = static_cast<const CMD_C_AdminControl*>(pDataBuffer);

	// 返回消息
	CMD_S_AdminControl SAdminControl;

	if ( pAdminControl->cbControlArea[BANKER_INDEX] 
		&& (pAdminControl->cbControlArea[SHUN_MEN_INDEX] || pAdminControl->cbControlArea[DUI_MEN_INDEX] || pAdminControl->cbControlArea[DAO_MEN_INDEX]) )
	{
		//"设置错误！";
		SAdminControl.cbControlType = 0;
		pITableFrame->SendUserItemData(pIServerUserItem, SUB_S_AMDIN_COMMAND_RESULT, &SAdminControl, sizeof(SAdminControl) );
		return false;
	}

	if ( pAdminControl->cbControlArea[BANKER_INDEX] )
	{
		//取消控制
		m_cbExcuteTimes = 0;

		//发送取消消息
		SAdminControl.cbControlType = 1;
		pITableFrame->SendUserItemData(pIServerUserItem, SUB_S_AMDIN_COMMAND_RESULT, &SAdminControl, sizeof(SAdminControl) );
		return true;
	}

	//设置控制
	m_cbExcuteTimes = 1;
	CopyMemory( m_cbControlArea, pAdminControl->cbControlArea, sizeof(m_cbControlArea) );

	//发送成功
	SAdminControl.cbControlType = 2;
	pITableFrame->SendUserItemData(pIServerUserItem, SUB_S_AMDIN_COMMAND_RESULT, &SAdminControl, sizeof(SAdminControl) );

	return true;
}

//需要控制
bool  CServerControlItemSink::NeedControl()
{
	if( m_cbExcuteTimes > 0 )
	{
		return true;
	}

	return false;
}

//完成控制
bool  CServerControlItemSink::CompleteControl()
{
	if (m_cbExcuteTimes > 0)
		m_cbExcuteTimes--;	

	if ( m_cbExcuteTimes == 0)
	{
		ZeroMemory(m_cbControlArea, sizeof(m_cbControlArea));
	}

	return true;
}


//控制牌型
void  CServerControlItemSink::GetSuitResult( BYTE cbTableCardArray[][MAX_CARD], BYTE cbTableCard[], LONGLONG lAllJettonScore[] )
{
	BYTE cbCardArray[MAX_INDEX][MAX_CARD];
	CopyMemory(cbCardArray, cbTableCardArray, sizeof(cbCardArray));

	for ( int i = 0; i < MAX_INDEX; ++i )
	{
		for ( int j = 0; j < MAX_INDEX; ++j )
		{
			if ( m_GameLogic.CompareCard( cbCardArray[j], 2, cbCardArray[i], 2) == 1 )
			{
				BYTE cbCardTemp[MAX_CARD];
				CopyMemory(cbCardTemp, cbCardArray[i], sizeof(BYTE) * MAX_CARD);
				CopyMemory(cbCardArray[i], cbCardArray[j], sizeof(BYTE) * MAX_CARD);
				CopyMemory(cbCardArray[j], cbCardTemp, sizeof(BYTE) * MAX_CARD);
			}
		}
	}

	int nArrayCount = 0;

	//赢取区域
	for ( int i = 0; i < MAX_INDEX; ++i )
	{
		if ( m_cbControlArea[i] )
		{
			CopyMemory(cbTableCardArray[i], cbCardArray[nArrayCount], sizeof(BYTE) * MAX_CARD);
			nArrayCount++;
		}
	}

	//输区域
	for ( int i = 0; i < MAX_INDEX; ++i )
	{
		if ( !m_cbControlArea[i] )
		{
			CopyMemory(cbTableCardArray[i], cbCardArray[nArrayCount], sizeof(BYTE) * MAX_CARD);
			nArrayCount++;
		}
	}
}

