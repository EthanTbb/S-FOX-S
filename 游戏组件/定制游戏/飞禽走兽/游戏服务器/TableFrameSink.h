#ifndef TABLE_FRAME_SINK_HEAD_FILE
#define TABLE_FRAME_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "GameLogic.h"
#include "ServerControl.h"

//////////////////////////////////////////////////////////////////////////
//��ʷ��¼
#define MAX_SCORE_HISTORY     65                  //��ʷ����
//////////////////////////////////////////////////////////////////////////

//��Ϸ������
class CTableFrameSink : public ITableFrameSink, public ITableUserAction
{
  //����ע��
protected:
  LONGLONG            m_lAllJettonScore[AREA_COUNT+1];    //ȫ����ע

  //������ע
protected:
  LONGLONG            m_lUserJettonScore[AREA_COUNT+1][GAME_PLAYER];  //������ע

  //�Ͼ���Ϣ
public:
  LONGLONG            m_lLastAllJettonPlace[AREA_COUNT +1][GAME_PLAYER]; //�Ͼ�����ע
  LONGLONG            m_lLastAllJettonArea[AREA_COUNT +1][GAME_PLAYER];  //�������һ�β���



  //���Ʊ���
protected:
  LONGLONG            m_lAreaLimitScore;            //��������
  LONGLONG            m_lUserLimitScore;            //��������
  LONGLONG            m_lApplyBankerCondition;        //��������
  LONGLONG            m_StorageStart;
  //��ҳɼ�
protected:
  LONGLONG            m_lUserWinScore[GAME_PLAYER];     //��ҳɼ�
  LONGLONG            m_lUserReturnScore[GAME_PLAYER];    //������ע
  LONGLONG            m_lUserRevenue[GAME_PLAYER];      //���˰��


  //���Ʋ���
protected:
  BYTE              m_cbControlArea;            //��������
  BYTE              m_cbControlTimes;           //���ƴ���
  bool              m_bControl;               //�Ƿ����
  LONGLONG            m_lControlStorage;            //���ƺ�����д��
  TCHAR             m_szControlName[LEN_NICKNAME];      //��������

  //�˿���Ϣ
protected:

  BYTE              m_cbTableCardArray[2];          //�����˿�
  BYTE                            m_cbShaYuAddMulti;                      //��������


  INT               m_nMultiple[AREA_ALL];          //������
  INT               m_nAnimalPercent[AREA_ALL];       //���б���
  INT               m_nAnimalTimes[AREA_ALL];       //���ִ���


  //״̬����
protected:
  DWORD             m_dwJettonTime;             //��ʼʱ��
  bool              m_bJettonState;             //��ע״̬

  //ׯ����Ϣ
protected:
  CWHArray<WORD>          m_ApplyUserArray;           //�������
  WORD              m_wCurrentBanker;           //��ǰׯ��
  WORD              m_wBankerTime;              //��ׯ����
  LONGLONG            m_lBankerWinScore;            //�ۼƳɼ�
  LONGLONG            m_lBankerCurGameScore;          //��ǰ�ɼ�
  bool              m_bEnableSysBanker;           //ϵͳ��ׯ

  //��¼����
protected:
  tagServerGameRecord       m_GameRecordArrary[MAX_SCORE_HISTORY];  //��Ϸ��¼
  int               m_nRecordFirst;             //��ʼ��¼
  int               m_nRecordLast;              //����¼


  //���Ʊ���
protected:
  LONGLONG            m_StorageControl;           //������
  LONGLONG            m_lStorageCurrent;            //�����ֵ
  LONGLONG            m_lStorageStart;              //�������
  LONGLONG            m_lBonus;               //�����ʽ��
  LONGLONG            m_StorageDeduct;            //�۳�����
  LONGLONG            m_lCompensateRadio;           //������⸶����
  TCHAR             m_szConfigFileName[MAX_PATH];     //�����ļ�
  TCHAR             m_szGameRoomName[SERVER_LEN];     //��������

  LONGLONG            m_lStorageMax1;             //���ⶥ1
  LONGLONG            m_lStorageMul1;             //ϵͳ��Ǯ����
  LONGLONG            m_lStorageMax2;             //���ⶥ2
  LONGLONG            m_lStorageMul2;             //ϵͳ��Ǯ����
  //�����˿���
protected:
  int               m_nMaxChipRobot;            //�����Ŀ
  int               m_nChipRobotCount;            //����ͳ��
  LONGLONG            m_lRobotAreaLimit;            //��������
  LONGLONG            m_lRobotBetCount;           //��ע����
  LONGLONG            m_lRobotAreaScore[AREA_COUNT+1];    //������ע
  int               m_nRobotListMaxCount;         //�������
  //ׯ������
protected:

  LONGLONG            m_lBankerMAX;             //���ׯ��
  LONGLONG            m_lBankerAdd;             //ׯ������

  LONGLONG            m_lBankerScoreMAX;            //ׯ��Ǯ
  LONGLONG            m_lBankerScoreAdd;            //��ׯ����

  //���ׯ����
  LONGLONG            m_lPlayerBankerMAX;           //���ׯ��
  int               m_nBankerTimeLimit;           //��������
  int               m_nBankerTimeAdd;           //���Ӵ��� (��Ҵ����������ʱ)
  LONGLONG            m_lExtraBankerScore;          //�������� (���ڴ�ֵʱ������������������ׯ)
  int               m_nExtraBankerTime;           //�������
  //��ׯ
  bool              m_bExchangeBanker;            //����ׯ��

  //���
  bool              m_bIsCleanCurJetton;

  //ʱ������
protected:
  BYTE              m_cbFreeTime;             //����ʱ��
  BYTE              m_cbBetTime;              //��עʱ��
  BYTE              m_cbEndTime;              //����ʱ��
  BYTE                            m_cbAddTime;              //����ʱ��
  LONGLONG            m_lUserStartScore[GAME_PLAYER];   //��ʼ����
  //�������
protected:
  //�������
protected:
  HINSTANCE           m_hInst;
  IServerControl*         m_pServerControl;

  CGameLogic            m_GameLogic;              //��Ϸ�߼�

  //����ӿ�
protected:
  ITableFrame *         m_pITableFrame;             //��ܽӿ�
  tagGameServiceOption *      m_pGameServiceOption;         //��������
  tagGameServiceAttrib *      m_pGameServiceAttrib;         //��Ϸ����

  //���Ա���
protected:
  static const WORD       m_wPlayerCount;             //��Ϸ����

  //��������
public:
  //���캯��
  CTableFrameSink();
  //��������
  virtual ~CTableFrameSink();

  //�����ӿ�
public:
  //�ͷŶ���
  virtual VOID  Release()
  {
    delete this;
  }

  //�ӿڲ�ѯ
  virtual void *  QueryInterface(const IID & Guid, DWORD dwQueryVer);

  //����ӿ�
public:
  //��ʼ��
  virtual bool Initialization(IUnknownEx * pIUnknownEx);
  //��λ����
  virtual VOID RepositionSink();

  //��ѯ�ӿ�
public:
  //��ѯ�޶�
  virtual SCORE QueryConsumeQuota(IServerUserItem * pIServerUserItem);
  //���ٻ���
  virtual SCORE QueryLessEnterScore(WORD wChairID, IServerUserItem * pIServerUserItem)
  {
    return 0;
  }
  //�����¼�
  virtual bool OnDataBaseMessage(WORD wRequestID, VOID * pData, WORD wDataSize)
  {
    return false;
  }
  //�����¼�
  virtual bool OnUserScroeNotify(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)
  {
    return false;
  }
  //��Ϣ�ӿ�
public:
  //��Ϸ״̬
  virtual bool IsUserPlaying(WORD wChairID);


  //��Ϸ�¼�
public:
  //��Ϸ��ʼ
  virtual bool OnEventGameStart();
  //��Ϸ����
  virtual bool OnEventGameConclude(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason);
  //���ͳ���
  virtual bool OnEventSendGameScene(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbGameStatus, bool bSendSecret);

  //�¼��ӿ�
public:
  //��ʱ���¼�
  virtual bool OnTimerMessage(DWORD wTimerID, WPARAM wBindParam);
  //��Ϸ��Ϣ����
  virtual bool OnGameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem);
  //�����Ϣ����
  virtual bool OnFrameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem);
  //�����¼�
  virtual bool OnGameDataBase(WORD wRequestID, VOID * pData, WORD wDataSize);
  //�۳������
  virtual bool QueryBuckleServiceCharge(WORD wChairID);
  //�����ӿ�
public:
  //���û���
  virtual void SetGameBaseScore(LONG lBaseScore) {};

  //�����¼�
public:
  //�û�����
  virtual bool  OnActionUserOffLine(WORD wChairID, IServerUserItem * pIServerUserItem) ;

  //�û�����
  virtual bool OnActionUserConnect(WORD wChairID,IServerUserItem * pIServerUserItem)
  {
    return true;
  }

  //�û�����
  virtual bool  OnActionUserSitDown(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser);
  //�û�����
  virtual bool  OnActionUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser);
  //�û�ͬ��
  virtual bool  OnActionUserOnReady(WORD wChairID, IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize)
  {
    return true;
  }

#ifdef __SPECIAL___
  //���в���
public:
  virtual bool  OnActionUserBank(WORD wChairID, IServerUserItem * pIServerUserItem);////
#endif

  //����
protected:
  bool OnAdminControl(CMD_C_ControlApplication* pData, IServerUserItem * pIServerUserItem);

  //��Ϸ�¼�
protected:
  //��ע�¼�
  bool OnUserPlaceJetton(WORD wChairID, BYTE cbJettonArea, LONGLONG lJettonScore);
  //����ׯ��
  bool OnUserApplyBanker(IServerUserItem *pIApplyServerUserItem);
  //ȡ������
  bool OnUserCancelBanker(IServerUserItem *pICancelServerUserItem);

  //��������
private:
  bool FindSuitBanker();
  //�����˿�
  void DispatchTableCard();
  //����ׯ��
  void SendApplyUser(IServerUserItem *pServerUserItem);
  //����ׯ��
  bool ChangeBanker(bool bCancelCurrentBanker);
  //���ͼ�¼
  void SendGameRecord(IServerUserItem *pIServerUserItem);
  //������Ϣ
  void SendGameMessage(WORD wChairID, LPCTSTR pszTipMsg);
  //��ע����
private:
  void   GetAllWinArea(BYTE bcWinArea[],BYTE bcAreaCount,BYTE InArea);
  //�����ע
  LONGLONG GetUserMaxJetton(WORD wChairID, BYTE Area);

  void RandList(BYTE cbCardBuffer[], BYTE cbBufferCount);
  //��ȡ��ֵ
  BYTE GetRandCardValue();
  //��ȡ��ֵ
  BYTE GetAnotherRandCardValue();
  //�������
  void CalculateAnimalPercent();
  //��ȡ����
  BYTE GetAnimalIndex(BYTE cbAllIndex);
  //�Ƿ�˥��
  bool NeedDeductStorage();

  //��Ϸͳ��
private:
  //����÷�
  LONGLONG CalculateScore();
  //��̽���ж�
  bool ProbeJudge();
  // �ж���Ӯ
  void JudgeSystemScore(LONGLONG& lSystemScore, LONGLONG& lAllScore);
  //��ȡ����
  void ReadConfigInformation();

  // ��¼����
  void WriteStorageInfo(SCORE lStorage,bool bControl);

};

//////////////////////////////////////////////////////////////////////////

#endif
