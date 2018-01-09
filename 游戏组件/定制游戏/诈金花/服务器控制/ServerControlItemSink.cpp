#include "StdAfx.h"
#include "servercontrolitemsink.h"

//
CServerControlItemSink::CServerControlItemSink(void)
{
}

CServerControlItemSink::~CServerControlItemSink(void)
{

}

//服务器控制
bool __cdecl CServerControlItemSink::ServerControl(BYTE cbHandCardData[GAME_PLAYER][MAX_COUNT], ITableFrame * pITableFrame)
{
  CString strInfo = TEXT("\n");
  for(int i = 0; i < GAME_PLAYER; ++i)
  {
    IServerUserItem* pTableUserItem = pITableFrame->GetTableUserItem(i);
    if(pTableUserItem == NULL)
    {
      continue;
    }

    strInfo += pTableUserItem->GetNickName();
    strInfo += TEXT("\n");

    for(int j = 0; j < MAX_COUNT; ++j)
    {
      strInfo += GetCradInfo(cbHandCardData[i][j]);
    }
    strInfo += TEXT("\n");
  }

  for(int i = 0; i < GAME_PLAYER; ++i)
  {
    IServerUserItem*  pTableUserItem = pITableFrame->GetTableUserItem(i);
    if(pTableUserItem == NULL)
    {
      continue;
    }

    if(CUserRight::IsGameCheatUser(pTableUserItem->GetUserRight()))
    {
      ////聊天框显示
      //pITableFrame->SendGameMessage(pTableUserItem, strInfo, SMT_CHAT);

      //明牌显示
      CMD_S_CheatCardInfo cheatCard;
      CopyMemory(cheatCard.cbAllHandCardData,cbHandCardData,sizeof(cheatCard));

      pITableFrame->SendTableData(i,SUB_S_CHEAT_CARD,&cheatCard,sizeof(cheatCard));
    }
  }

  int nLookonCount = 0;
  IServerUserItem* pLookonUserItem = pITableFrame->EnumLookonUserItem(nLookonCount);
  while(pLookonUserItem)
  {
    //if( CUserRight::IsGameCheatUser(pLookonUserItem->GetUserRight()) )
    //  pITableFrame->SendGameMessage(pLookonUserItem, strInfo, SMT_CHAT);

    nLookonCount++;
    pLookonUserItem = pITableFrame->EnumLookonUserItem(nLookonCount);
  }

  return true;
}

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

    if(m_GameLogic.CompareCard(bCardData,cbControlCardData[i],MAX_COUNT) == false)
    {
      CopyMemory(bCardData,cbControlCardData[i],sizeof(bCardData));
      bMaxUser=i;
    }
  }

  //最小牌型
  BYTE bMinUser = 0;
  CopyMemory(bCardData,cbControlCardData[0],sizeof(bCardData));
  for(BYTE i=1; i<GAME_PLAYER; i++)
  {
    if(cbControlCardData[i][0] == 0)
    {
      continue;
    }

    if(m_GameLogic.CompareCard(bCardData,cbControlCardData[i],MAX_COUNT) == true)
    {
      CopyMemory(bCardData,cbControlCardData[i],sizeof(bCardData));
      bMinUser=i;
    }
  }

  bool bExistWhite = false;
  bool bExistBlack = false;
  bool bExistInvalid = false;

  WORD wWhiteChairID = INVALID_CHAIR;
  WORD wBlackChairID = INVALID_CHAIR;
  WORD wInvalidChairID = INVALID_CHAIR;

  for(WORD i=0; i<GAME_PLAYER; i++)
  {
    if(userroster[i] == WHITE_ROSTER)
    {
      bExistWhite = true;
      wWhiteChairID = i;
    }
    else if(userroster[i] == BLACK_ROSTER)
    {
      bExistBlack = true;
      wBlackChairID = i;
    }
    else if(userroster[i] == INVALID_ROSTER)
    {
      bExistInvalid = true;
      wInvalidChairID = i;
    }
  }

  BYTE cbSwapCard[MAX_COUNT];
  ZeroMemory(cbSwapCard, sizeof(cbSwapCard));

  //存在白名单
  if(bExistWhite == true)
  {
    //最大的牌不为白名单
    if(bMaxUser != wWhiteChairID)
    {
      CopyMemory(cbSwapCard, cbControlCardData[wWhiteChairID], sizeof(BYTE) * MAX_COUNT);
      CopyMemory(cbControlCardData[wWhiteChairID], cbControlCardData[bMaxUser], sizeof(BYTE) * MAX_COUNT);
      CopyMemory(cbControlCardData[bMaxUser], cbSwapCard, sizeof(BYTE) * MAX_COUNT);
    }

  }
  else  //不存在白名单
  {
    //存在黑名单
    if(bExistBlack == true)
    {
      //最小的牌不为黑名单
      if(bMinUser != wBlackChairID)
      {
        CopyMemory(cbSwapCard, cbControlCardData[wBlackChairID], sizeof(BYTE) * MAX_COUNT);
        CopyMemory(cbControlCardData[wBlackChairID], cbControlCardData[bMinUser], sizeof(BYTE) * MAX_COUNT);
        CopyMemory(cbControlCardData[bMinUser], cbSwapCard, sizeof(BYTE) * MAX_COUNT);
      }
    }

  }

  return true;
}

//获取牌信息
CString CServerControlItemSink::GetCradInfo(BYTE cbCardData)
{
  CString strInfo;
  if((cbCardData&LOGIC_MASK_COLOR) == 0x00)
  {
    strInfo += TEXT("[方块 ");
  }
  else if((cbCardData&LOGIC_MASK_COLOR) == 0x10)
  {
    strInfo += TEXT("[梅花 ");
  }
  else if((cbCardData&LOGIC_MASK_COLOR) == 0x20)
  {
    strInfo += TEXT("[红桃 ");
  }
  else if((cbCardData&LOGIC_MASK_COLOR) == 0x30)
  {
    strInfo += TEXT("[黑桃 ");
  }

  if((cbCardData&LOGIC_MASK_VALUE) == 0x01)
  {
    strInfo += TEXT("A] ");
  }
  else if((cbCardData&LOGIC_MASK_VALUE) == 0x02)
  {
    strInfo += TEXT("2] ");
  }
  else if((cbCardData&LOGIC_MASK_VALUE) == 0x03)
  {
    strInfo += TEXT("3] ");
  }
  else if((cbCardData&LOGIC_MASK_VALUE) == 0x04)
  {
    strInfo += TEXT("4] ");
  }
  else if((cbCardData&LOGIC_MASK_VALUE) == 0x05)
  {
    strInfo += TEXT("5] ");
  }
  else if((cbCardData&LOGIC_MASK_VALUE) == 0x06)
  {
    strInfo += TEXT("6] ");
  }
  else if((cbCardData&LOGIC_MASK_VALUE) == 0x07)
  {
    strInfo += TEXT("7] ");
  }
  else if((cbCardData&LOGIC_MASK_VALUE) == 0x08)
  {
    strInfo += TEXT("8] ");
  }
  else if((cbCardData&LOGIC_MASK_VALUE) == 0x09)
  {
    strInfo += TEXT("9] ");
  }
  else if((cbCardData&LOGIC_MASK_VALUE) == 0x0A)
  {
    strInfo += TEXT("10] ");
  }
  else if((cbCardData&LOGIC_MASK_VALUE) == 0x0B)
  {
    strInfo += TEXT("J] ");
  }
  else if((cbCardData&LOGIC_MASK_VALUE) == 0x0C)
  {
    strInfo += TEXT("Q] ");
  }
  else if((cbCardData&LOGIC_MASK_VALUE) == 0x0D)
  {
    strInfo += TEXT("K] ");
  }

  return strInfo;
}
