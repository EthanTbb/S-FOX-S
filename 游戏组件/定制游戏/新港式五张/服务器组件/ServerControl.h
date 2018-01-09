#pragma once

//游戏控制基类
class IServerControl
{
public:
	IServerControl(void){};
	virtual ~IServerControl(void){};

public:
	//服务器控制
	virtual bool __cdecl ServerControl(BYTE cbHandCardData[GAME_PLAYER][5], ITableFrame * pITableFrame) = NULL;
};
