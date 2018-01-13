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

//服务器控制
bool  CServerControlItemSink::ServerControl(WORD wSubCmdID, const void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem, ITableFrame * pITableFrame)
{		
	ASSERT(wDataSize==sizeof(CMD_C_ControlApplication));
	if (wDataSize!=sizeof(CMD_C_ControlApplication)) 
		return false;
	//如果不具有管理员权限 则返回错误
	if( !CUserRight::IsGameCheatUser(pIServerUserItem->GetUserRight()) )
		return false;

	CMD_C_ControlApplication* pControlApplication = (CMD_C_ControlApplication*)pDataBuffer;

	switch(pControlApplication->cbControlAppType)
	{
	case C_CA_UPDATE:	//更新
		{
			CMD_S_ControlReturns ControlReturns;
			ZeroMemory(&ControlReturns,sizeof(ControlReturns));
			ControlReturns.cbReturnsType = S_CR_UPDATE_SUCCES;
			ControlReturns.cbControlArea = m_cbControlArea;
			ControlReturns.cbControlTimes = m_cbControlTimes;
			pITableFrame->SendUserItemData(pIServerUserItem,SUB_S_ADMIN_COMMDN,&ControlReturns,sizeof(ControlReturns));
		}
		return true;
	case C_CA_SET:		//设置
		{
			if (  pControlApplication->cbControlArea  > 7 )
			{
				//设置失败
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
	case C_CA_CANCELS:	//取消
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
	//设置失败
	CMD_S_ControlReturns ControlReturns;
	ZeroMemory(&ControlReturns,sizeof(ControlReturns));
	ControlReturns.cbReturnsType = S_CR_FAILURE;
	ControlReturns.cbControlArea = 0xff;
	pITableFrame->SendUserItemData(pIServerUserItem,SUB_S_ADMIN_COMMDN,&ControlReturns,sizeof(ControlReturns));
	return true;
}

//需要控制
bool  CServerControlItemSink::NeedControl()
{
	if ( m_cbControlArea != 0xff && m_cbControlArea <= 7 && m_cbControlTimes > 0 )
	{
		return true;
	}
	return false;
	
}

//满足控制
bool  CServerControlItemSink::MeetControl(tagControlInfo ControlInfo)
{
	return true;
}

//完成控制
bool  CServerControlItemSink::CompleteControl()
{
	m_cbControlTimes--;
	if(m_cbControlTimes == 0)
	{
		m_cbControlArea = 0xff;
	}
	return true;
}

//返回控制区域
bool  CServerControlItemSink::ReturnControlArea(tagControlInfo& ControlInfo)
{
	//1,13大橘子、2,14大铃铛、24小铃铛,3小王，4大王、5,11,17,23大苹果、6小苹果、7,19大柠檬、8大西瓜、9小西瓜、10绿LUCK、12小橘子、15小双7、16大	双7、18小柠檬、20大星星、21小星星、22红LUCK
	//排出LUCK  格式化区域因为苹果有4个区域  三个的把小的倍数增加一次 
								//大码			双7				双星		西瓜		金钟		  柠檬			橘子		  苹果
	BYTE cbControlArea[8][4] = { 3,3,4,4,   16,16,16,16,   20,20,20,20,   8,8,8,8,   2,14,2,14,   7,19,7,19,   1,13,1,13,   5,11,17,23 };
	ControlInfo.cbControlArea = cbControlArea[m_cbControlArea][rand()%4];
	return true;
}

