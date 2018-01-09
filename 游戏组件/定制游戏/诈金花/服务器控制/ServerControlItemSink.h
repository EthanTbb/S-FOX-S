#pragma once
#include "../游戏服务器/ServerControl.h"
#include "GameLogic.h"

//数值掩码
#define LOGIC_MASK_COLOR      0xF0                //花色掩码
#define LOGIC_MASK_VALUE      0x0F                //数值掩码


class CServerControlItemSink : public IServerControl
{
public:
  CServerControlItemSink(void);
  virtual ~CServerControlItemSink(void);

  CGameLogic            m_GameLogic;            //游戏逻辑

public:
  //服务器控制
  virtual bool __cdecl ServerControl(BYTE cbHandCardData[GAME_PLAYER][MAX_COUNT], ITableFrame * pITableFrame);

  virtual bool __cdecl ControlResult(BYTE cbControlCardData[GAME_PLAYER][MAX_COUNT], USERROSTER userroster[GAME_PLAYER]);

  //获取牌信息
  CString GetCradInfo(BYTE cbCardData);
};
