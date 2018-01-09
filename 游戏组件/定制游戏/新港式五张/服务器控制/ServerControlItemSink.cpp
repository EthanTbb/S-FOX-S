#include "StdAfx.h"
#include "servercontrolitemsink.h"

//
CServerControlItemSink::CServerControlItemSink(void)
{
}

CServerControlItemSink::~CServerControlItemSink( void )
{

}

//����������
bool __cdecl CServerControlItemSink::ServerControl( BYTE cbHandCardData[GAME_PLAYER][5], ITableFrame * pITableFrame )
{
	CString strInfo = TEXT("\n");
	for( int i = 0; i < GAME_PLAYER; ++i )
	{
		IServerUserItem* pTableUserItem = pITableFrame->GetTableUserItem(i);
		if( pTableUserItem == NULL )
			continue;

		strInfo += pTableUserItem->GetNickName();
		strInfo += TEXT("\n");

		for( int j = 0; j < 5; ++j )
		{
			strInfo += GetCradInfo(cbHandCardData[i][j]);
		}
		strInfo += TEXT("\n");
	}

	for( int i = 0; i < GAME_PLAYER; ++i )
	{
		IServerUserItem*  pTableUserItem = pITableFrame->GetTableUserItem(i);
		if( pTableUserItem == NULL )
			continue;

		if( CUserRight::IsGameCheatUser(pTableUserItem->GetUserRight()) )
			pITableFrame->SendGameMessage(pTableUserItem, strInfo, SMT_CHAT);
	}

	int nLookonCount = 0;
	IServerUserItem* pLookonUserItem = pITableFrame->EnumLookonUserItem(nLookonCount);
	while( pLookonUserItem )
	{
		if( CUserRight::IsGameCheatUser(pLookonUserItem->GetUserRight()) )
			pITableFrame->SendGameMessage(pLookonUserItem, strInfo, SMT_CHAT);

		nLookonCount++;
		pLookonUserItem = pITableFrame->EnumLookonUserItem(nLookonCount);
	}

	return true;
}

//��ȡ����Ϣ
CString CServerControlItemSink::GetCradInfo( BYTE cbCardData )
{
	CString strInfo;
	if( (cbCardData&LOGIC_MASK_COLOR) == 0x00 )
		strInfo += TEXT("[���� ");
	else if( (cbCardData&LOGIC_MASK_COLOR) == 0x10 )
		strInfo += TEXT("[÷�� ");
	else if( (cbCardData&LOGIC_MASK_COLOR) == 0x20 )
		strInfo += TEXT("[���� ");
	else if( (cbCardData&LOGIC_MASK_COLOR) == 0x30 )
		strInfo += TEXT("[���� ");

	if( (cbCardData&LOGIC_MASK_VALUE) == 0x01 )
		strInfo += TEXT("A] ");
	else if( (cbCardData&LOGIC_MASK_VALUE) == 0x08 )
		strInfo += TEXT("8] ");
	else if( (cbCardData&LOGIC_MASK_VALUE) == 0x09 )
		strInfo += TEXT("9] ");
	else if( (cbCardData&LOGIC_MASK_VALUE) == 0x0A )
		strInfo += TEXT("10] ");
	else if( (cbCardData&LOGIC_MASK_VALUE) == 0x0B )
		strInfo += TEXT("J] ");
	else if( (cbCardData&LOGIC_MASK_VALUE) == 0x0C )
		strInfo += TEXT("Q] ");
	else if( (cbCardData&LOGIC_MASK_VALUE) == 0x0D )
		strInfo += TEXT("K] ");

	return strInfo;
}
