#pragma once

#include "..\游戏服务器\ServerControl.h"
#include "..\游戏服务器\GameLogic.h"

//////////////////////////////////////////////////////////////////////////

class CServerControlItemSink : public IServerControl
{
  CGameLogic            m_GameLogic;            //游戏逻辑

public:
  CServerControlItemSink();
  virtual ~CServerControlItemSink();

public:
  virtual bool __cdecl ControlResult(BYTE cbControlCardData[GAME_PLAYER][MAX_COUNT], USERROSTER userroster[GAME_PLAYER]);


};

//////////////////////////////////////////////////////////////////////////
