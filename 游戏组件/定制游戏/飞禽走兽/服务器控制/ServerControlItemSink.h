#pragma once
#include "../��Ϸ������/ServerControl.h"
#include "../��Ϸ������/GameLogic.h"

class CServerControlItemSink : public IServerControl
{
  //���Ʋ���
protected:
  LONGLONG            m_lAllJettonScore[AREA_COUNT+1];    //ȫ����ע
  BYTE              m_cbControlArea;            //��������
  BYTE              m_cbControlTimes;           //���ƴ���
  LONGLONG                        m_lCurStorage;              //��ǰ���
  BYTE              m_cbControlStyle;           //���Ʒ�ʽ
  CGameLogic            m_GameLogic;              //��Ϸ�߼�

public:
  CServerControlItemSink(void);
  virtual ~CServerControlItemSink(void);

  //ʵ������
  BYTE    GetRealArea(BYTE cbNeedControlArea);
  //��������
  BYTE    BankerWinCard(bool bIsWin);
  //ׯ��Ӯ��
  LONGLONG  GetBankerWinScore(BYTE cbRealArea);
  //��������
  BYTE    GetAnotherArea();


public:
  //����������
  virtual bool  ServerControl(WORD wSubCmdID, const void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem, ITableFrame * pITableFrame);

  //��Ҫ����
  virtual bool  NeedControl();

  //�������
  virtual bool  MeetControl(tagControlInfo ControlInfo);

  //��ɿ���
  virtual bool  CompleteControl();

  //���ؿ�������
  virtual bool  ReturnControlArea(tagControlInfo& ControlInfo,LONGLONG lAllJettonScore[]) ;
  //��¼���
  virtual bool  SetStorage(LONGLONG lCurStorage);

  void  UpdateControl(CMD_S_ControlReturns* pControlReturns,IServerUserItem * pIServerUserItem);
  void WriteInfo(LPCTSTR pszString);

  //��Ϣ����
  void PrintingInfo(TCHAR* pText, WORD cbCount, BYTE cbArea, BYTE cbTimes);
};
