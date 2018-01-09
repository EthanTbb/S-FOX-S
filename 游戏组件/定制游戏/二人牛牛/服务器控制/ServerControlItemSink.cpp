#include "StdAfx.h"
#include "servercontrolitemsink.h"

//////////////////////////////////////////////////////////////////////////



//
CServerControlItemSink::CServerControlItemSink(void)
{
}

CServerControlItemSink::~CServerControlItemSink(void)
{

}

//返回控制区域
bool __cdecl CServerControlItemSink::ControlResult(BYTE cbControlCardData[GAME_PLAYER][MAX_COUNT], USERROSTER userroster[GAME_PLAYER])
{
  BYTE bCardData[MAX_COUNT];
  BYTE bHandCardData[GAME_PLAYER][MAX_COUNT];
  CopyMemory(bCardData,cbControlCardData[0],sizeof(bCardData));

  for(BYTE i=0; i<GAME_PLAYER; i++)
  {
    CopyMemory(bHandCardData[i],cbControlCardData[i],sizeof(BYTE)*MAX_COUNT);
  }

  //最大牌型
  BYTE bMaxUser=0;
  for(BYTE i=1; i<GAME_PLAYER; i++)
  {
    if(cbControlCardData[i][0] == 0)
    {
      continue;
    }

    bool bFistOx=m_GameLogic.GetOxCard(bCardData,MAX_COUNT);
    bool bNextOx=m_GameLogic.GetOxCard(cbControlCardData[i],MAX_COUNT);


    if(m_GameLogic.CompareCard(bCardData,cbControlCardData[i],MAX_COUNT,bFistOx,bNextOx) == false)
    {
      CopyMemory(bCardData,cbControlCardData[i],sizeof(bCardData));
      bMaxUser=i;
    }
  }

  BYTE bMinUser = (bMaxUser + 1) % GAME_PLAYER;

  if(userroster[0] == INVALID_ROSTER)
  {
    ASSERT(userroster[1] != INVALID_ROSTER);

    if(userroster[1] == WHITE_ROSTER)
    {
      CopyMemory(cbControlCardData[1],bHandCardData[bMaxUser],sizeof(bHandCardData[bMaxUser]));
      CopyMemory(cbControlCardData[0],bHandCardData[bMinUser],sizeof(bHandCardData[bMinUser]));
    }
    else if(userroster[1] == BLACK_ROSTER)
    {
      CopyMemory(cbControlCardData[1],bHandCardData[bMinUser],sizeof(bHandCardData[bMinUser]));
      CopyMemory(cbControlCardData[0],bHandCardData[bMaxUser],sizeof(bHandCardData[bMaxUser]));
    }
  }
  else if(userroster[0] == WHITE_ROSTER)
  {
    ASSERT(userroster[1] != WHITE_ROSTER);

    if(userroster[1] == INVALID_ROSTER)
    {
      CopyMemory(cbControlCardData[1],bHandCardData[bMinUser],sizeof(bHandCardData[bMinUser]));
      CopyMemory(cbControlCardData[0],bHandCardData[bMaxUser],sizeof(bHandCardData[bMaxUser]));
    }
    else if(userroster[1] == BLACK_ROSTER)
    {
      CopyMemory(cbControlCardData[1],bHandCardData[bMinUser],sizeof(bHandCardData[bMinUser]));
      CopyMemory(cbControlCardData[0],bHandCardData[bMaxUser],sizeof(bHandCardData[bMaxUser]));
    }
  }
  else if(userroster[0] == BLACK_ROSTER)
  {
    ASSERT(userroster[1] != BLACK_ROSTER);

    if(userroster[1] == INVALID_ROSTER)
    {
      CopyMemory(cbControlCardData[1],bHandCardData[bMaxUser],sizeof(bHandCardData[bMaxUser]));
      CopyMemory(cbControlCardData[0],bHandCardData[bMinUser],sizeof(bHandCardData[bMinUser]));
    }
    else if(userroster[1] == WHITE_ROSTER)
    {
      CopyMemory(cbControlCardData[1],bHandCardData[bMaxUser],sizeof(bHandCardData[bMaxUser]));
      CopyMemory(cbControlCardData[0],bHandCardData[bMinUser],sizeof(bHandCardData[bMinUser]));
    }
  }

  return true;
}