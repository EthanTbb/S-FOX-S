#pragma once
#include "../��Ϸ������/ServerControl.h"
#include "GameLogic.h"


class CServerControlItemSink : public IServerControl
{
  CGameLogic            m_GameLogic;              //��Ϸ�߼�

public:
  CServerControlItemSink(void);
  virtual ~CServerControlItemSink(void);

public:


  //���ؿ�������
  virtual bool __cdecl ControlResult(BYTE cbControlCardData[GAME_PLAYER][MAX_COUNT], USERROSTER userroster[GAME_PLAYER]);

};
