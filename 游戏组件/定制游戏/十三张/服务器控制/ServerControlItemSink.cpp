#include "StdAfx.h"
#include "servercontrolitemsink.h"

//
CServerControlItemSink::CServerControlItemSink(void)
{
}

CServerControlItemSink::~CServerControlItemSink( void )
{

}

//服务器控制
bool __cdecl CServerControlItemSink::ServerControl(CMD_S_CheatCard *pCheatCard, ITableFrame * pITableFrame, bool m_bIsReady[])
{
	for(WORD i = 0; i < GAME_PLAYER ;i++)
	{
		if(m_bIsReady[i])
		{
			IServerUserItem * pIServerUserItem = pITableFrame->GetTableUserItem(i);
			ASSERT(pIServerUserItem);
			if(pIServerUserItem)
			{
				//作弊用户
				if(CUserRight::IsGameCheatUser(pIServerUserItem->GetUserRight()))
				{
					ServerControl(pCheatCard, pITableFrame, pIServerUserItem);

				}
			}
		}
	}

	//WB改
	WORD wEnumIndex=0;
	do
	{
		IServerUserItem * pIServerUserItem = pITableFrame->EnumLookonUserItem(wEnumIndex++);
		if(pIServerUserItem == NULL) break;
		//作弊用户
		if(CUserRight::IsGameCheatUser(pIServerUserItem->GetUserRight()))
		{
			ServerControl(pCheatCard, pITableFrame, pIServerUserItem);
		}
	}while(TRUE);

	return true;
}
//服务器控制
bool __cdecl CServerControlItemSink::ServerControl(CMD_S_CheatCard *pCheatCard, ITableFrame * pITableFrame, IServerUserItem * pIServerUserItem)
{
	pITableFrame->SendUserItemData(pIServerUserItem,SUB_S_CHEAT_CARD,pCheatCard,sizeof(CMD_S_CheatCard));
	return true;
}