#pragma once

//游戏控制基类
class IServerControl
{
public:
  IServerControl(void) {};
  virtual ~IServerControl(void) {};

public:
  //服务器控制
  virtual bool __cdecl ServerControl(BYTE cbHandCardData[GAME_PLAYER][MAX_COUNT], ITableFrame * pITableFrame) = NULL;

  virtual bool __cdecl ControlResult(BYTE cbControlCardData[GAME_PLAYER][MAX_COUNT], USERROSTER userroster[GAME_PLAYER]) = NULL;
};
