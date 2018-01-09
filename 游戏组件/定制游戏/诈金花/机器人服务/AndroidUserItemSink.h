#ifndef ANDROID_USER_ITEM_SINK_HEAD_FILE
#define ANDROID_USER_ITEM_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "..\��Ϸ������\GameLogic.h"

//////////////////////////////////////////////////////////////////////////



//��Ϸ�Ի���
class CAndroidUserItemSink : public IAndroidUserItemSink
{
  //��Ϸ����
protected:
  LONGLONG            m_lStockScore;              //��ǰ���
  WORD              m_wBankerUser;              //ׯ���û�
  WORD              m_wCurrentUser;             //��ǰ�û�
  BYTE              m_cbCardType;             //�û�����

  //��ע��Ϣ
protected:
  bool              m_bMingZhu;               //���ƶ���
  LONGLONG            m_lCellScore;             //��Ԫ��ע
  LONGLONG            m_lMaxCellScore;            //�����ע
  LONGLONG            m_lUserMaxScore;            //������
  LONGLONG            m_lCurrentTimes;            //��ǰ����
  LONGLONG            m_lTableScore[GAME_PLAYER];       //��ע��Ŀ

  //�û�״̬
protected:
  BYTE              m_cbPlayStatus[GAME_PLAYER];      //��Ϸ״̬
  BYTE              m_cbRealPlayer[GAME_PLAYER];      //�������
  BYTE              m_cbAndroidStatus[GAME_PLAYER];     //�������

  //�û��˿�
protected:
  BYTE              m_cbHandCardData[MAX_COUNT];      //�û�����
  BYTE              m_cbAllHandCardData[GAME_PLAYER][MAX_COUNT];//�����˿�

  //�ؼ�����
public:
  CGameLogic            m_GameLogic;              //��Ϸ�߼�
  IAndroidUserItem *        m_pIAndroidUserItem;          //�û��ӿ�
  TCHAR             m_szRoomName[32];           //���÷���

  //�����˴�ȡ��
  LONGLONG            m_lRobotScoreRange[2];          //���Χ
  LONGLONG            m_lRobotBankGetScore;         //�������
  LONGLONG            m_lRobotBankGetScoreBanker;       //������� (ׯ��)
  int               m_nRobotBankStorageMul;         //����

  //��������
public:
  //���캯��
  CAndroidUserItemSink();
  //��������
  virtual ~CAndroidUserItemSink();

  //�����ӿ�
public:
  //�ͷŶ���
  virtual VOID Release()
  {
    delete this;
  }
  //�ӿڲ�ѯ
  virtual void * QueryInterface(const IID & Guid, DWORD dwQueryVer);

  //���ƽӿ�
public:
  //��ʼ�ӿ�
  virtual bool Initialization(IUnknownEx * pIUnknownEx);
  //���ýӿ�
  virtual bool RepositionSink();

  //��Ϸ�¼�
public:
  //ʱ����Ϣ
  virtual bool OnEventTimer(UINT nTimerID);
  //��Ϸ��Ϣ
  virtual bool OnEventGameMessage(WORD wSubCmdID, void * pData, WORD wDataSize);
  //��Ϸ��Ϣ
  virtual bool OnEventFrameMessage(WORD wSubCmdID, void * pData, WORD wDataSize);
  //������Ϣ
  virtual bool OnEventSceneMessage(BYTE cbGameStatus, bool bLookonOther, void * pData, WORD wDataSize);

  //�û��¼�
public:
  //�û�����
  virtual void OnEventUserEnter(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser);
  //�û��뿪
  virtual void OnEventUserLeave(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser);
  //�û�����
  virtual void OnEventUserScore(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser);
  //�û�״̬
  virtual void OnEventUserStatus(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser);
  //�û���λ
  virtual void OnEventUserSegment(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser);

  //��Ϣ����
protected:
  //��������Ϣ
  bool OnSubAndroidCard(const void * pBuffer, WORD wDataSize);
  //��Ϸ��ʼ
  bool OnSubGameStart(const void * pBuffer, WORD wDataSize);
  //�û�����
  bool OnSubGiveUp(const void * pBuffer, WORD wDataSize);
  //�û���ע
  bool OnSubAddScore(const void * pBuffer, WORD wDataSize);
  //�û�����
  bool OnSubLookCard(const void * pBuffer, WORD wDataSize);
  //�û�����
  bool OnSubCompareCard(const void * pBuffer, WORD wDataSize);
  //�û�����
  bool OnSubOpenCard(const void * pBuffer, WORD wDataSize);
  //�û�ǿ��
  bool OnSubPlayerExit(const void * pBuffer, WORD wDataSize);
  //��Ϸ����
  bool OnSubGameEnd(const void * pBuffer, WORD wDataSize);

  //������
private:
  //��ȡ����
  void ReadConfigInformation(tagRobotConfig *pConfig);
  //���в���
  void BankOperate(BYTE cbType);

  //�߼�����
protected:
  //�ƶ�ʤ��
  WORD EstimateWinner();
  //��ע����
  LONGLONG GetAddScoreTimes();
};

//////////////////////////////////////////////////////////////////////////

#endif
