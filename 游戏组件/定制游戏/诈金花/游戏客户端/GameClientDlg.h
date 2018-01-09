#pragma once

#include "Stdafx.h"
#include "GameLogic.h"
#include "GameClientView.h"

//////////////////////////////////////////////////////////////////////////

//����˵��
//typedef CWHArray<CMD_Buffer,CMD_Buffer &> CSocketPacketArray;

//////////////////////////////////////////////////////////////////////////

//��Ϸ�Ի���
class CGameClientEngine : public CGameFrameEngine
{
  //��Ϸ����
protected:
  WORD              m_wCurrentUser;             //��ǰ�û�
  WORD              m_wBankerUser;              //ׯ���û�
  bool              m_bCheatUser;             //�������
  BYTE              m_cbGameCount;            //

  //�û�״̬
protected:
  LPCTSTR             m_strAccounts[GAME_PLAYER];
  TCHAR             m_szAccounts[GAME_PLAYER][LEN_ACCOUNTS];  //�������
  BYTE              m_cbPlayStatus[GAME_PLAYER];      //��Ϸ״̬
  LONGLONG            m_lTableScore[GAME_PLAYER];       //��ע��Ŀ
  bool              m_bPlaySound;

  //�˿˱���
protected:
  BYTE              m_cbHandCardData[GAME_PLAYER][MAX_COUNT];//�����˿�

  //��ע��Ϣ
protected:
  LONGLONG            m_lMaxScore;              //�ⶥ��Ŀ
  LONGLONG            m_lMaxCellScore;            //��Ԫ����
  LONGLONG            m_lCellScore;             //��Ԫ��ע
  LONGLONG            m_lCurrentTimes;            //��ǰ����
  LONGLONG            m_lUserMaxScore;            //������
  bool              m_bMingZhu[GAME_PLAYER];        //���ƶ���


  //�����û�
protected:
  WORD              m_wLostUser;              //����ʧ��
  WORD              m_wWinnerUser;              //ʤ���û�

  //������Ϣ
protected:
  WORD              m_wLostUserID[GAME_PLAYER];       //�Ȱ��û�

  //������Ϣ
protected:
  WORD              m_wViewChairID[GAME_PLAYER];      //�������

  //���ñ���
protected:
  DWORD             m_dwCardHSpace;             //�˿�����

  //�������
//protected:
  //CSocketPacketArray        m_SocketPacketArray;          //���ݻ���

  //�ؼ�����
public:
  CGameLogic            m_GameLogic;              //��Ϸ�߼�
  CGameClientView         m_GameClientView;           //��Ϸ��ͼ
  CGlobalUnits *                  m_pGlobalUnits;                         //ȫ�ֵ�Ԫ

  //��������
public:
  //���캯��
  CGameClientEngine();
  //��������
  virtual ~CGameClientEngine();

  //����̳�
private:
  //��ʼ����
  virtual bool OnInitGameEngine();
  //���ÿ��
  virtual bool OnResetGameEngine();
  //��Ϸ����
  virtual void OnGameOptionSet();

  //ʱ���¼�
public:
  //ʱ��ɾ��
  virtual bool OnEventGameClockKill(WORD wChairID);
  //ʱ����Ϣ
  virtual bool OnEventGameClockInfo(WORD wChairID, UINT nElapse, WORD wClockID);
  //��������
  virtual bool AllowBackGroundSound(bool bAllowSound);

  //��Ϸ�¼�
public:
  //�Թ���Ϣ
  virtual bool OnEventLookonMode(VOID * pData, WORD wDataSize);
  //��Ϸ��Ϣ
  virtual bool OnEventGameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize);
  //������Ϣ
  virtual bool OnEventSceneMessage(BYTE cbGameStatus, bool bLookonUser, VOID * pData, WORD wDataSize);

  //�û�ʱ��
public:
  //�û�����
  virtual VOID OnEventUserEnter(IClientUserItem * pIClientUserItem, bool bLookonUser);
  //�û��뿪
  virtual VOID OnEventUserLeave(IClientUserItem * pIClientUserItem, bool bLookonUser);

  //��Ϣ����
protected:
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

  //��������
protected:
  //ת����Ϣ
  void ChangeUserInfo(BYTE bCardData[],BYTE bCardCount,CString &CardInfo);
  //�������
  void ScoreControl(BOOL bShow);
  //��ע����
  void AddScoreControl(BOOL bShow);
  //�������
  void UpdataControl();

  //��Ϣӳ��
protected:
  //��ʼ��ť
  LRESULT OnStart(WPARAM wParam, LPARAM lParam);
  //��ע��ť
  LRESULT OnAddScore(WPARAM wParam, LPARAM lParam);
  //���ټ�ע
  LRESULT OnMinScore(WPARAM wParam, LPARAM lParam);
  //����ע
  LRESULT OnMaxScore(WPARAM wParam, LPARAM lParam);
  //ȷ����Ϣ
  LRESULT OnConfirmScore(WPARAM wParam, LPARAM lParam);
  //ȡ����Ϣ
  LRESULT OnCancelAdd(WPARAM wParam, LPARAM lParam);
  //������Ϣ
  LRESULT OnLookCard(WPARAM wParam, LPARAM lParam);
  //������Ϣ
  LRESULT OnCompareCard(WPARAM wParam, LPARAM lParam);
  //��ѡ�����û�
  LRESULT OnChooseCUser(WPARAM wParam, LPARAM lParam);
  //������Ϣ
  LRESULT OnOpenCard(WPARAM wParam, LPARAM lParam);
  //������Ϣ
  LRESULT OnGiveUp(WPARAM wParam, LPARAM lParam);
  //�������
  LRESULT OnSendCardFinish(WPARAM wParam, LPARAM lParam);
  //�������
  LRESULT OnFlashCardFinish(WPARAM wParam, LPARAM lParam);
  //��Ϸ����
  LRESULT OnMessageGameSound(WPARAM wParam, LPARAM lParam);
public:
  //������Ϣ
  LRESULT OnChatMessage(WPARAM wParam, LPARAM lParam);
  //�����Ϣ
  LRESULT OnStorageInfo(WPARAM wParam,LPARAM lParam);
  //��������
  LRESULT OnAddUserRoster(WPARAM wParam,LPARAM lParam);
  //ɾ������
  LRESULT OnDeleteUserRoster(WPARAM wParam,LPARAM lParam);
  //�����������
  LRESULT OnRequestUpdateUserRoster(WPARAM wParam,LPARAM lParam);

  DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////
