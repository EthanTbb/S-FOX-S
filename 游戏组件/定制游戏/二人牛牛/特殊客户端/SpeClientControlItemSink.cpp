// ClientControlItemSink.cpp : 实现文件
//

#include "stdafx.h"
#include "Resource.h"
#include "SpeClientControlItemSink.h"
#include ".\SpeClientcontrolitemsink.h"

// CSpeClientControlItemSinkDlg 对话框

IMPLEMENT_DYNAMIC(CSpeClientControlItemSinkDlg, ISpeClientControlDlg)

CSpeClientControlItemSinkDlg::CSpeClientControlItemSinkDlg(CWnd* pParent /*=NULL*/)
  : ISpeClientControlDlg(CSpeClientControlItemSinkDlg::IDD, pParent)
{
}

CSpeClientControlItemSinkDlg::~CSpeClientControlItemSinkDlg()
{
}

void CSpeClientControlItemSinkDlg::DoDataExchange(CDataExchange* pDX)
{
  ISpeClientControlDlg::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CSpeClientControlItemSinkDlg, ISpeClientControlDlg)
END_MESSAGE_MAP()

// CSpeClientControlItemSinkDlg 消息处理程序

//初始化
BOOL CSpeClientControlItemSinkDlg::OnInitDialog()
{
  ISpeClientControlDlg::OnInitDialog();
  return TRUE;
}



bool CSpeClientControlItemSinkDlg::ReqResult(const void * pBuffer)
{

  //消息处理
  CMD_S_AllCard * pSendCard=(CMD_S_AllCard *)pBuffer;

  CString strInfo=TEXT("");

  CGameFrameEngine *pGameFrameEngine=CGameFrameEngine::GetInstance();

  //扑克信息
  //m_pIStringMessage->InsertNormalString(TEXT("\n[游戏开始]:"));

  strInfo+=TEXT("\n[游戏开始]:\r\n");

  for(WORD i=0; i<GAME_PLAYER; i++)
  {
    //获取用户
    IClientUserItem * pUserData=pGameFrameEngine->GetTableUserItem(i);
    if(pUserData==NULL)
    {
      continue;
    }

    //用户名字
    //CW2CT strAccounts(pUserData->GetNickName());
    TCHAR szAccounts[LEN_ACCOUNTS]= {0};
    lstrcpyn(szAccounts,pUserData->GetNickName(),CountArray(szAccounts));

    //输出名字
    TCHAR szBuffer[255];
    if(!pSendCard->bAICount[i])
    {
      _sntprintf(szBuffer,CountArray(szBuffer),TEXT("\n[%s]:\r\n"),szAccounts);
    }
    else
    {
      _sntprintf(szBuffer,CountArray(szBuffer),TEXT("\n[%s](机器人):\r\n"),szAccounts);
    }
    //m_pIStringMessage->InsertNormalString(szBuffer);

    strInfo+=szBuffer;

    //扑克信息
    CString sBuffer;
    ChangeUserInfo(pSendCard->cbCardData[i],MAX_COUNT,sBuffer);
    //m_pIStringMessage->InsertNormalString(sBuffer);
    strInfo+=sBuffer;
    strInfo+=TEXT("\r\n");

    //牌型信息
    BYTE cbValue=m_GameLogic.GetCardType(pSendCard->cbCardData[i],MAX_COUNT);
    if(cbValue== OX_FIVEKING)
    {
      sBuffer=TEXT("牌型：五金牛牛");
    }
    else if(cbValue== OX_FOURKING)
    {
      sBuffer=TEXT("牌型：四金牛牛");
    }
    else if(cbValue== 10)
    {
      sBuffer=TEXT("牌型：牛牛");
    }
    else if(cbValue>0)
    {
      sBuffer.Format(TEXT("牌型：牛%ld"),cbValue);
    }
    else
    {
      sBuffer=TEXT("牌型：无牛");
    }
    //m_pIStringMessage->InsertNormalString(sBuffer);

    strInfo+=sBuffer;

  }
  CGameFrameEngine::GetInstance()->SendMessage(IDM_SPE_COMMDN,(WPARAM)&strInfo,0);

  return true;
}

void __cdecl CSpeClientControlItemSinkDlg::OnAllowControl(bool bEnable)
{

}

//转换信息
void CSpeClientControlItemSinkDlg::ChangeUserInfo(BYTE bCardData[],BYTE bCardCount,CString &CardInfo)
{
  //转换信息
  for(BYTE i=0; i<bCardCount; i++)
  {
    //扑克花色
    BYTE bCardColor = bCardData[i]&LOGIC_MASK_COLOR;
    switch(bCardColor)
    {
      case 0x00:
      {
        CardInfo+=TEXT("方块");
      }
      break;
      case 0x10:
      {
        CardInfo+=TEXT("梅花");
      }
      break;
      case 0x20:
      {
        CardInfo+=TEXT("红心");
      }
      break;
      case 0x30:
      {
        CardInfo+=TEXT("黑桃");
      }
      break;
    }

    //扑克数据
    BYTE bTempCardData = bCardData[i]&LOGIC_MASK_VALUE;
    switch(bTempCardData)
    {
      case 0x01:
        CardInfo+=TEXT("［A］.");
        break;
      case 0x02:
        CardInfo+=TEXT("［2］.");
        break;
      case 0x03:
        CardInfo+=TEXT("［3］.");
        break;
      case 0x04:
        CardInfo+=TEXT("［4］.");
        break;
      case 0x05:
        CardInfo+=TEXT("［5］.");
        break;
      case 0x06:
        CardInfo+=TEXT("［6］.");
        break;
      case 0x07:
        CardInfo+=TEXT("［7］.");
        break;
      case 0x08:
        CardInfo+=TEXT("［8］.");
        break;
      case 0x09:
        CardInfo+=TEXT("［9］.");
        break;
      case 0x0a:
        CardInfo+=TEXT("［10］.");
        break;
      case 0x0b:
        CardInfo+=TEXT("［J］.");
        break;
      case 0x0c:
        CardInfo+=TEXT("［Q］.");
        break;
      case 0x0d:
        CardInfo+=TEXT("［K］.");
        break;
    }
  }

  return ;
}


