#pragma once

#include "..\��Ϸ������\ServerControl.h"
#include "..\��Ϸ������\GameLogic.h"

//////////////////////////////////////////////////////////////////////////

class CServerControlItemSink : public IServerControl
{
  CGameLogic            m_GameLogic;            //��Ϸ�߼�

public:
  CServerControlItemSink();
  virtual ~CServerControlItemSink();

public:
  virtual bool __cdecl ControlResult(BYTE cbControlCardData[GAME_PLAYER][MAX_COUNT], USERROSTER userroster[GAME_PLAYER]);


};

//////////////////////////////////////////////////////////////////////////
