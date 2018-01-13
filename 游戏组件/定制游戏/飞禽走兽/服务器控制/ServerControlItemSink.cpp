#include "StdAfx.h"
#include "servercontrolitemsink.h"
#include <locale>

CServerControlItemSink::CServerControlItemSink(void)
{
  m_cbControlArea = 0xff;
  m_cbControlTimes = 0;
  m_lCurStorage=0;
  ZeroMemory(m_lAllJettonScore,sizeof(m_lAllJettonScore));
  m_cbControlStyle=0;

  srand((unsigned)time(NULL));
}

CServerControlItemSink::~CServerControlItemSink(void)
{

}

//����������
bool  CServerControlItemSink::ServerControl(WORD wSubCmdID, const void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem, ITableFrame * pITableFrame)
{
  ASSERT(wDataSize==sizeof(CMD_C_ControlApplication));
  if(wDataSize!=sizeof(CMD_C_ControlApplication))
  {
    return false;
  }
  //��������й���ԱȨ�� �򷵻ش���
  if(!CUserRight::IsGameCheatUser(pIServerUserItem->GetUserRight()))
  {
    return false;
  }

  CMD_C_ControlApplication* pControlApplication = (CMD_C_ControlApplication*)pDataBuffer;

  switch(pControlApplication->cbControlAppType)
  {
    case C_CA_UPDATE: //����
    {
      CMD_S_ControlReturns ControlReturns;
      ZeroMemory(&ControlReturns,sizeof(ControlReturns));
      ControlReturns.cbReturnsType = S_CR_UPDATE_SUCCES;
      ControlReturns.cbControlArea = m_cbControlArea;
      ControlReturns.cbControlTimes = m_cbControlTimes;
      ControlReturns.cbControlStyle=m_cbControlStyle;
      UpdateControl(&ControlReturns,pIServerUserItem);
      pITableFrame->SendUserItemData(pIServerUserItem,SUB_S_ADMIN_COMMDN,&ControlReturns,sizeof(ControlReturns));
    }
    return true;
    case C_CA_SET:    //����
    {

      switch(pControlApplication->cbControlStyle)
      {
        case CS_BET_AREA: //��������
        {
          m_cbControlStyle=pControlApplication->cbControlStyle;

          m_cbControlArea = pControlApplication->cbControlArea;
          m_cbControlTimes = pControlApplication->cbControlTimes;

          CMD_S_ControlReturns ControlReturns;
          ZeroMemory(&ControlReturns,sizeof(ControlReturns));
          ControlReturns.cbReturnsType = S_CR_SET_SUCCESS;
          ControlReturns.cbControlArea = m_cbControlArea;
          ControlReturns.cbControlTimes = m_cbControlTimes;
          ControlReturns.cbControlStyle=m_cbControlStyle;
          UpdateControl(&ControlReturns,pIServerUserItem);
          pITableFrame->SendUserItemData(pIServerUserItem,SUB_S_ADMIN_COMMDN,&ControlReturns,sizeof(ControlReturns));

          break;
        }
        case CS_BANKER_LOSE:  //ׯ������
        {
          m_cbControlStyle=pControlApplication->cbControlStyle;
          m_cbControlTimes=pControlApplication->cbControlTimes;
          CMD_S_ControlReturns ControlReturns;
          ZeroMemory(&ControlReturns,sizeof(ControlReturns));
          ControlReturns.cbReturnsType = S_CR_SET_SUCCESS;
          ControlReturns.cbControlArea = m_cbControlArea;
          ControlReturns.cbControlTimes = m_cbControlTimes;
          ControlReturns.cbControlStyle=m_cbControlStyle;
          UpdateControl(&ControlReturns,pIServerUserItem);
          pITableFrame->SendUserItemData(pIServerUserItem,SUB_S_ADMIN_COMMDN,&ControlReturns,sizeof(ControlReturns));

          break;
        }
        case CS_BANKER_WIN:   //ׯ��Ӯ��
        {
          m_cbControlStyle=pControlApplication->cbControlStyle;
          m_cbControlTimes=pControlApplication->cbControlTimes;
          CMD_S_ControlReturns ControlReturns;
          ZeroMemory(&ControlReturns,sizeof(ControlReturns));
          ControlReturns.cbReturnsType = S_CR_SET_SUCCESS;
          ControlReturns.cbControlArea = m_cbControlArea;
          ControlReturns.cbControlTimes = m_cbControlTimes;
          ControlReturns.cbControlStyle=m_cbControlStyle;
          UpdateControl(&ControlReturns,pIServerUserItem);
          pITableFrame->SendUserItemData(pIServerUserItem,SUB_S_ADMIN_COMMDN,&ControlReturns,sizeof(ControlReturns));
          break;
        }
        default:  //�ܾ�����
        {
          //����ʧ��
          CMD_S_ControlReturns ControlReturns;
          ZeroMemory(&ControlReturns,sizeof(ControlReturns));
          ControlReturns.cbReturnsType = S_CR_FAILURE;
          ControlReturns.cbControlArea = 0xff;
          UpdateControl(&ControlReturns,pIServerUserItem);
          pITableFrame->SendUserItemData(pIServerUserItem,SUB_S_ADMIN_COMMDN,&ControlReturns,sizeof(ControlReturns));
          break;
        }

      }
      return true;
    }
    case C_CA_CANCELS:  //ȡ��
    {
      m_cbControlArea = 0xff;
      m_cbControlTimes = 0;
      CMD_S_ControlReturns ControlReturns;
      ZeroMemory(&ControlReturns,sizeof(ControlReturns));
      ControlReturns.cbReturnsType = S_CR_CANCEL_SUCCESS;
      ControlReturns.cbControlArea = 0xff;
      UpdateControl(&ControlReturns,pIServerUserItem);
      pITableFrame->SendUserItemData(pIServerUserItem,SUB_S_ADMIN_COMMDN,&ControlReturns,sizeof(ControlReturns));
    }
    return true;

    case C_CA_PRINT_STORAGE:  //��ӡ���
    {
      CMD_S_ControlReturns ControlReturns;
      ZeroMemory(&ControlReturns,sizeof(ControlReturns));
      ControlReturns.cbReturnsType = S_CR_ACK_PRINT_STORAGE;
      ControlReturns.lCurStorage = m_lCurStorage;
      UpdateControl(&ControlReturns,pIServerUserItem);
      pITableFrame->SendUserItemData(pIServerUserItem,SUB_S_ADMIN_COMMDN,&ControlReturns,sizeof(ControlReturns));
    }
    return true;
  }
  //����ʧ��
  CMD_S_ControlReturns ControlReturns;
  ZeroMemory(&ControlReturns,sizeof(ControlReturns));
  ControlReturns.cbReturnsType = S_CR_FAILURE;
  ControlReturns.cbControlArea = 0xff;
  UpdateControl(&ControlReturns,pIServerUserItem);
  pITableFrame->SendUserItemData(pIServerUserItem,SUB_S_ADMIN_COMMDN,&ControlReturns,sizeof(ControlReturns));
  return true;
}

//��Ҫ����
bool  CServerControlItemSink::NeedControl()
{
  /*if ( m_cbControlArea != 0xff && m_cbControlArea <= 7 && m_cbControlTimes > 0 )
  {
    return true;
  }*/

  if(m_cbControlStyle > 0 && m_cbControlTimes > 0)
  {
    return true;
  }

  return false;

}

//�������
bool  CServerControlItemSink::MeetControl(tagControlInfo ControlInfo)
{
  return true;
}

//��ɿ���
bool  CServerControlItemSink::CompleteControl()
{
  /*m_cbControlTimes--;
  if(m_cbControlTimes == 0)
  {
    m_cbControlArea = 0xff;
  }*/

  if(m_cbControlTimes > 0)
  {
    m_cbControlTimes--;
  }

  if(m_cbControlTimes == 0)
  {
    m_cbControlTimes = 0;
    m_cbControlStyle = 0;
    m_cbControlArea = 0xff;
    ZeroMemory(m_lAllJettonScore,sizeof(m_lAllJettonScore));
  }

  return true;
}

//���ؿ�������
bool  CServerControlItemSink::ReturnControlArea(tagControlInfo& ControlInfo,LONGLONG lAllJettonScore[])
{
  //��������
  memcpy(m_lAllJettonScore, lAllJettonScore, sizeof(m_lAllJettonScore));

  ControlInfo.cbControlArea2=0;

  switch(m_cbControlStyle)
  {
    case CS_BET_AREA:
    {
      ControlInfo.cbControlArea1=GetRealArea(m_cbControlArea);
      //���� �������
      if(ControlInfo.cbControlArea1%7==4)
      {
        ControlInfo.cbControlArea2=GetAnotherArea();
      }
      break;
    }
    case CS_BANKER_WIN:
    {
      ControlInfo.cbControlArea1=BankerWinCard(true);
      break;
    }
    case CS_BANKER_LOSE:
    {
      ControlInfo.cbControlArea1=BankerWinCard(false);
      break;
    }

  }

  return true;
}

BYTE CServerControlItemSink::BankerWinCard(bool bIsWin)
{

  BYTE cbRealArea=1;
  //�Ƚ���֮���ϵ
  for(BYTE cbIndex=0; cbIndex<11; cbIndex++)
  {
    if(cbIndex==8)
    {
      continue;
    }

    cbRealArea=GetRealArea(cbIndex);

    LONGLONG lBankerWinScore=0;

    lBankerWinScore= GetBankerWinScore(cbRealArea);

    if(lBankerWinScore==0)
    {
      return cbRealArea;
    }

    if(bIsWin&&lBankerWinScore>0)
    {
      return cbRealArea;
    }

    if((!bIsWin)&&lBankerWinScore<0)
    {
      return cbRealArea;
    }

  }

  return cbRealArea;

}

LONGLONG CServerControlItemSink::GetBankerWinScore(BYTE cbRealArea)
{
  BYTE  bcResulteOut[AREA_COUNT];
  memset(bcResulteOut,0,AREA_COUNT);
  m_GameLogic.GetCardType(cbRealArea,1,bcResulteOut);

  LONGLONG lBankerWinScore=0;

  for(WORD wAreaIndex = 1; wAreaIndex <= AREA_COUNT; ++wAreaIndex)
  {

    if(bcResulteOut[wAreaIndex-1]>1)
    {
      lBankerWinScore -= (m_lAllJettonScore[wAreaIndex]* (bcResulteOut[wAreaIndex-1]-1)) ;
    }
    else
    {
      if(bcResulteOut[wAreaIndex-1]==0)
      {
        lBankerWinScore += m_lAllJettonScore[wAreaIndex];
      }

    }
  }

  return lBankerWinScore;
}

//��������
BYTE CServerControlItemSink::GetAnotherArea()
{
  BYTE cbControlArea[24] = {1,2,3,5, 6, 7,8,9,10,12,13,14,15,16,17,19, 20, 21,22,23,24,26,27,28};
  BYTE cbIndex=rand()%24;
  ASSERT(cbControlArea[cbIndex] != 4 && cbControlArea[cbIndex] != 11 && cbControlArea[cbIndex] != 18 && cbControlArea[cbIndex] != 25);
  return cbControlArea[cbIndex];
}
//ʵ������
BYTE CServerControlItemSink::GetRealArea(BYTE cbNeedControlArea)
{
  BYTE cbRealArea=0;

  //����
  if(0==cbNeedControlArea)
  {
    BYTE cbControlArea[3] = {5, 6, 7};
    cbRealArea=cbControlArea[rand()%3];
  }
  else if(1==cbNeedControlArea)//����
  {
    BYTE cbControlArea[3] = {8, 9, 10};
    cbRealArea=cbControlArea[rand()%3];
  }

  else if(2==cbNeedControlArea)//��ȸ
  {
    BYTE cbControlArea[3] = {12, 13, 14};
    cbRealArea=cbControlArea[rand()%3];
  }
  else if(3==cbNeedControlArea)//��ӥ
  {
    BYTE cbControlArea[3] = {15, 16, 17};
    cbRealArea=cbControlArea[rand()%3];
  }
  else if(4==cbNeedControlArea)//ʨ��
  {
    BYTE cbControlArea[3] = {19, 20, 21};
    cbRealArea=cbControlArea[rand()%3];
  }
  else if(5==cbNeedControlArea)//��è
  {
    BYTE cbControlArea[3] = {22, 23, 24};
    cbRealArea=cbControlArea[rand()%3];
  }
  else if(6==cbNeedControlArea)//����
  {
    BYTE cbControlArea[3] = {26, 27, 28};
    cbRealArea=cbControlArea[rand()%3];
  }
  else if(7==cbNeedControlArea)//����
  {
    BYTE cbControlArea[3] = {1, 2, 3};
    cbRealArea=cbControlArea[rand()%3];
  }
  else if(8==cbNeedControlArea)//����
  {
    //4 �ƽ������ ; 11 ͨɱ;  18 ������;  25 ͨ��;
    BYTE cbControlArea[4] = {4, 11, 18,25};
    cbRealArea=cbControlArea[rand()%4];
  }
  else if(9==cbNeedControlArea)//����
  {
    BYTE cbControlArea[12] = {5, 6, 7,8,9,10,12,13,14,15,16,17};
    cbRealArea=cbControlArea[rand()%12];
  }
  else if(10==cbNeedControlArea)//����
  {
    BYTE cbControlArea[12] = {1,2,3,19, 20, 21,22,23,24,26,27,28};
    cbRealArea=cbControlArea[rand()%12];
  }


  return cbRealArea;
}

//��¼���
bool CServerControlItemSink::SetStorage(LONGLONG lCurStorage)
{
  m_lCurStorage=lCurStorage;
  return true;
}

//���¿���
void  CServerControlItemSink::UpdateControl(CMD_S_ControlReturns* pControlReturns ,IServerUserItem * pIServerUserItem)
{
  CString str=TEXT("");
  switch(pControlReturns->cbReturnsType)
  {
    case S_CR_FAILURE:
    {
      //SetDlgItemText(IDC_STATIC_INFO,TEXT("����ʧ�ܣ�"));
      str+="����ʧ��";
      break;
    }
    case S_CR_UPDATE_SUCCES:
    {
      TCHAR zsText[256] = TEXT("");
      TCHAR zsTextTemp[256] = TEXT("");


      //PrintingInfo(zsTextTemp,256,pControlReturns->cbControlArea,pControlReturns->cbControlTimes);
      myprintf(zsText,CountArray(zsText),TEXT("�������ݳɹ���\r\n %s"),zsTextTemp);
      //SetDlgItemText(IDC_STATIC_INFO,zsText);

      str+=zsText;

      break;
    }
    case S_CR_SET_SUCCESS:
    {
      TCHAR zsText[256] = TEXT("");
      TCHAR zsTextTemp[256] = TEXT("");
      PrintingInfo(zsTextTemp,256,pControlReturns->cbControlArea,pControlReturns->cbControlTimes);
      myprintf(zsText,CountArray(zsText),TEXT("�������ݳɹ���\r\n %s"),zsTextTemp);
      str+=zsText;


      switch(m_cbControlStyle)
      {
        case CS_BANKER_WIN:
        {
          str.Format(TEXT("�����ѽ��ܣ����Ʒ�ʽ��ׯ��Ӯ��ִ�д�����%d"),m_cbControlTimes);
          break;
        }
        case CS_BANKER_LOSE:
        {
          str.Format(TEXT("�����ѽ��ܣ����Ʒ�ʽ��ׯ���䣬ִ�д�����%d"),m_cbControlTimes);
          break;
        }
        case CS_BET_AREA:
        {

          str.Format(TEXT("�����ѽ��ܣ����Ʒ�ʽ��������ƣ�ִ�д�����%d"),m_cbControlTimes);

          break;
        }
        default:
          break;
      }


      break;
    }
    case S_CR_CANCEL_SUCCESS:
    {
      //SetDlgItemText(IDC_STATIC_INFO,TEXT("ȡ�����óɹ���"));
      str+="ȡ�����óɹ���";
      break;
    }

    case S_CR_ACK_PRINT_STORAGE:
    {

      {
        CString strStorage=TEXT("");
        strStorage.Format(TEXT("%I64d"),pControlReturns->lCurStorage);
        //SetDlgItemText(IDC_EDIT_KUCUN,strStorage);
        str+=strStorage;


      }
      break;
    }
  }

  TCHAR szClientAddr[16]=TEXT("");
  DWORD dwAddr=pIServerUserItem->GetClientAddr();
  BYTE * pClientAddr=(BYTE *)&dwAddr;
  _sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

  CString strRecord;
  CTime Time(CTime::GetCurrentTime());
  strRecord.Format(TEXT(" %s :| ʱ��: %d-%d-%d %d:%d:%d |������:%s|IP��ַ:%d\n"),pIServerUserItem->GetNickName(), Time.GetYear(), Time.GetMonth(), Time.GetDay(), Time.GetHour(), Time.GetMinute(), Time.GetSecond(),pIServerUserItem->GetMachineID(),szClientAddr);
  strRecord=strRecord+str;
  WriteInfo(strRecord);
}

//����д��Ϣ
void CServerControlItemSink::WriteInfo(LPCTSTR pszString)
{
  //������������
  char* old_locale = _strdup(setlocale(LC_CTYPE,NULL));
  setlocale(LC_CTYPE, "chs");

  CStdioFile myFile;
  CString strFileName;
  strFileName.Format(TEXT("�������޿��Ƽ�¼.txt"));
  BOOL bOpen = myFile.Open(strFileName, CFile::modeReadWrite|CFile::modeCreate|CFile::modeNoTruncate);
  if(bOpen)
  {
    myFile.SeekToEnd();
    myFile.WriteString(pszString);
    myFile.Flush();
    myFile.Close();
  }

  //��ԭ�����趨
  setlocale(LC_CTYPE, old_locale);
  free(old_locale);
}

//��Ϣ
void CServerControlItemSink::PrintingInfo(TCHAR* pText, WORD cbCount, BYTE cbArea, BYTE cbTimes)
{
  if(cbArea == 0xff)
  {
    myprintf(pText,cbCount,TEXT("��ʱ�޿��ơ�"));
    return;
  }
  TCHAR szDesc[32] = TEXT("");
  myprintf(pText,cbCount,TEXT("ʤ������"));
  if(cbArea == 0)
  {
    _sntprintf(szDesc,CountArray(szDesc),TEXT("���ӡ�8��"));
    lstrcat(pText,szDesc);
  }
  else if(cbArea == 1)
  {
    _sntprintf(szDesc,CountArray(szDesc),TEXT("���ӡ�6��"));
    lstrcat(pText,szDesc);
  }
  else if(cbArea == 2)
  {
    _sntprintf(szDesc,CountArray(szDesc),TEXT("��ȸ��6��"));
    lstrcat(pText,szDesc);
  }
  else if(cbArea == 3)
  {
    _sntprintf(szDesc,CountArray(szDesc),TEXT("��ӥ��12��"));
    lstrcat(pText,szDesc);
  }
  else if(cbArea == 4)
  {
    _sntprintf(szDesc,CountArray(szDesc),TEXT("ʨ�ӡ�8��"));
    lstrcat(pText,szDesc);
  }
  else if(cbArea == 5)
  {
    _sntprintf(szDesc,CountArray(szDesc),TEXT("��è��6��"));
    lstrcat(pText,szDesc);
  }
  else if(cbArea == 6)
  {
    _sntprintf(szDesc,CountArray(szDesc),TEXT("���ӡ�6��"));
    lstrcat(pText,szDesc);
  }
  else if(cbArea == 7)
  {
    _sntprintf(szDesc,CountArray(szDesc),TEXT("���ӡ�12��"));
    lstrcat(pText,szDesc);
  }
  else if(cbArea == 8)
  {
    _sntprintf(szDesc,CountArray(szDesc),TEXT("�����24��"));
    lstrcat(pText,szDesc);
  }
  else if(cbArea == 9)
  {
    _sntprintf(szDesc,CountArray(szDesc),TEXT("���ݡ�2��"));
    lstrcat(pText,szDesc);
  }
  else if(cbArea == 10)
  {
    _sntprintf(szDesc,CountArray(szDesc),TEXT("���ޡ�2��"));
    lstrcat(pText,szDesc);
  }
  myprintf(szDesc,CountArray(szDesc),TEXT("ִ�д�����%d��"), cbTimes);
  lstrcat(pText,szDesc);
}

