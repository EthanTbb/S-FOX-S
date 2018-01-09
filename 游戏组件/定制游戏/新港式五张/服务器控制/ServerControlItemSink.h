#pragma once
#include "../服务器组件/ServerControl.h"

//数值掩码
#define	LOGIC_MASK_COLOR			0xF0								//花色掩码
#define	LOGIC_MASK_VALUE			0x0F								//数值掩码


class CServerControlItemSink : public IServerControl
{
public:
	CServerControlItemSink(void);
	virtual ~CServerControlItemSink(void);

public:
	//服务器控制
	virtual bool __cdecl ServerControl(BYTE cbHandCardData[GAME_PLAYER][5], ITableFrame * pITableFrame);

	//获取牌信息
	CString GetCradInfo( BYTE cbCardData );
};
