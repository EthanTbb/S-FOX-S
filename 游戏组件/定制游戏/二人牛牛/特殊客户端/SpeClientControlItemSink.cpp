// ClientControlItemSink.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Resource.h"
#include "SpeClientControlItemSink.h"
#include ".\SpeClientcontrolitemsink.h"

// CSpeClientControlItemSinkDlg �Ի���

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

// CSpeClientControlItemSinkDlg ��Ϣ�������

//��ʼ��
BOOL CSpeClientControlItemSinkDlg::OnInitDialog()
{
  ISpeClientControlDlg::OnInitDialog();
  return TRUE;
}



bool CSpeClientControlItemSinkDlg::ReqResult(const void * pBuffer)
{

  //��Ϣ����
  CMD_S_AllCard * pSendCard=(CMD_S_AllCard *)pBuffer;

  CString strInfo=TEXT("");

  CGameFrameEngine *pGameFrameEngine=CGameFrameEngine::GetInstance();

  //�˿���Ϣ
  //m_pIStringMessage->InsertNormalString(TEXT("\n[��Ϸ��ʼ]:"));

  strInfo+=TEXT("\n[��Ϸ��ʼ]:\r\n");

  for(WORD i=0; i<GAME_PLAYER; i++)
  {
    //��ȡ�û�
    IClientUserItem * pUserData=pGameFrameEngine->GetTableUserItem(i);
    if(pUserData==NULL)
    {
      continue;
    }

    //�û�����
    //CW2CT strAccounts(pUserData->GetNickName());
    TCHAR szAccounts[LEN_ACCOUNTS]= {0};
    lstrcpyn(szAccounts,pUserData->GetNickName(),CountArray(szAccounts));

    //�������
    TCHAR szBuffer[255];
    if(!pSendCard->bAICount[i])
    {
      _sntprintf(szBuffer,CountArray(szBuffer),TEXT("\n[%s]:\r\n"),szAccounts);
    }
    else
    {
      _sntprintf(szBuffer,CountArray(szBuffer),TEXT("\n[%s](������):\r\n"),szAccounts);
    }
    //m_pIStringMessage->InsertNormalString(szBuffer);

    strInfo+=szBuffer;

    //�˿���Ϣ
    CString sBuffer;
    ChangeUserInfo(pSendCard->cbCardData[i],MAX_COUNT,sBuffer);
    //m_pIStringMessage->InsertNormalString(sBuffer);
    strInfo+=sBuffer;
    strInfo+=TEXT("\r\n");

    //������Ϣ
    BYTE cbValue=m_GameLogic.GetCardType(pSendCard->cbCardData[i],MAX_COUNT);
    if(cbValue== OX_FIVEKING)
    {
      sBuffer=TEXT("���ͣ����ţţ");
    }
    else if(cbValue== OX_FOURKING)
    {
      sBuffer=TEXT("���ͣ��Ľ�ţţ");
    }
    else if(cbValue== 10)
    {
      sBuffer=TEXT("���ͣ�ţţ");
    }
    else if(cbValue>0)
    {
      sBuffer.Format(TEXT("���ͣ�ţ%ld"),cbValue);
    }
    else
    {
      sBuffer=TEXT("���ͣ���ţ");
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

//ת����Ϣ
void CSpeClientControlItemSinkDlg::ChangeUserInfo(BYTE bCardData[],BYTE bCardCount,CString &CardInfo)
{
  //ת����Ϣ
  for(BYTE i=0; i<bCardCount; i++)
  {
    //�˿˻�ɫ
    BYTE bCardColor = bCardData[i]&LOGIC_MASK_COLOR;
    switch(bCardColor)
    {
      case 0x00:
      {
        CardInfo+=TEXT("����");
      }
      break;
      case 0x10:
      {
        CardInfo+=TEXT("÷��");
      }
      break;
      case 0x20:
      {
        CardInfo+=TEXT("����");
      }
      break;
      case 0x30:
      {
        CardInfo+=TEXT("����");
      }
      break;
    }

    //�˿�����
    BYTE bTempCardData = bCardData[i]&LOGIC_MASK_VALUE;
    switch(bTempCardData)
    {
      case 0x01:
        CardInfo+=TEXT("��A��.");
        break;
      case 0x02:
        CardInfo+=TEXT("��2��.");
        break;
      case 0x03:
        CardInfo+=TEXT("��3��.");
        break;
      case 0x04:
        CardInfo+=TEXT("��4��.");
        break;
      case 0x05:
        CardInfo+=TEXT("��5��.");
        break;
      case 0x06:
        CardInfo+=TEXT("��6��.");
        break;
      case 0x07:
        CardInfo+=TEXT("��7��.");
        break;
      case 0x08:
        CardInfo+=TEXT("��8��.");
        break;
      case 0x09:
        CardInfo+=TEXT("��9��.");
        break;
      case 0x0a:
        CardInfo+=TEXT("��10��.");
        break;
      case 0x0b:
        CardInfo+=TEXT("��J��.");
        break;
      case 0x0c:
        CardInfo+=TEXT("��Q��.");
        break;
      case 0x0d:
        CardInfo+=TEXT("��K��.");
        break;
    }
  }

  return ;
}


