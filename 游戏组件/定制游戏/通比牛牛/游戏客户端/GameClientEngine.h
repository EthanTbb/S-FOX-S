#pragma once

#include "Stdafx.h"
#include "GameLogic.h"
#include "GameClientView.h"
#include "HistoryRecord.h"

//////////////////////////////////////////////////////////////////////////

//����˵��
typedef CWHArray<UDP_Buffer,UDP_Buffer &> CSocketPacketArray;

//////////////////////////////////////////////////////////////////////////

//��Ϸ�Ի���
class CGameClientEngine : public CGameFrameEngine
{
  //���ñ���
protected:
  DWORD             m_dwCardHSpace;             //�˿�����

  //��ע��Ϣ
protected:
  LONGLONG            m_lCellScore;             //��Ϸ�׷�
  LONGLONG            m_lTableScore[GAME_PLAYER];       //��ע��Ŀ

  //״̬����
protected:
  bool              m_bTimeOut;               //��ʱ��ʶ
  BYTE                            m_cbDynamicJoin;                        //��̬����
  WORD              m_wBankerUser;              //ׯ���û�
  BYTE              m_cbPlayStatus[GAME_PLAYER];      //��Ϸ״̬
  TCHAR             m_szAccounts[GAME_PLAYER][LEN_ACCOUNTS];//�������

  //�û��˿�
protected:
  BYTE              m_cbUserOxCard[GAME_PLAYER];      //ţţ����
  BYTE              m_cbHandCardData[GAME_PLAYER][MAX_COUNT];//�û�����
  BYTE              m_cbOxCardData[MAX_COUNT];        //ţţ�˿�

  //�������
protected:
  CSocketPacketArray        m_SocketPacketArray;          //���ݻ���

  //�������
protected:
  CHistoryScore         m_HistoryScore;             //��ʷ����

  //�ؼ�����
public:
  CGameLogic            m_GameLogic;              //��Ϸ�߼�
  CGameClientView         m_GameClientView;           //��Ϸ��ͼ

  //��������
public:
  //���캯��
  CGameClientEngine();
  //��������
  virtual ~CGameClientEngine();

  //���ƽӿ�
public:
  //��ʼ����
  virtual bool OnInitGameEngine();
  //���ÿ��
  virtual bool OnResetGameEngine();

  //ʱ���¼�
public:
  //ʱ��ɾ��
  virtual bool OnEventGameClockKill(WORD wChairID);
  //ʱ����Ϣ
  virtual bool OnEventGameClockInfo(WORD wChairID, UINT nElapse, WORD wClockID);

  //�¼��ӿ�
public:
  //�Թ�״̬
  virtual bool OnEventLookonMode(VOID * pData, WORD wDataSize);
  //������Ϣ
  virtual bool OnEventGameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize);
  //��Ϸ����
  virtual bool OnEventSceneMessage(BYTE cbGameStatus, bool bLookonOther, VOID * pData, WORD wDataSize);

  //�û��¼�
public:
  //�û�����
  virtual VOID OnEventUserEnter(IClientUserItem * pIClientUserItem, bool bLookonUser);
  //�û��뿪
  virtual VOID OnEventUserLeave(IClientUserItem * pIClientUserItem, bool bLookonUser);
  //�û�״̬
  virtual VOID OnEventUserStatus(IClientUserItem * pIClientUserItem, bool bLookonUser);

  //��Ϣ����
protected:
  //��Ϸ��ʼ
  bool OnSubGameStart(const void * pBuffer, WORD wDataSize);
  //�û�̯��
  bool OnSubOpenCard(const void * pBuffer, WORD wDataSize);
  //�û�ǿ��
  bool OnSubPlayerExit(const void * pBuffer, WORD wDataSize);
  //��Ϸ����
  bool OnSubGameEnd(const void * pBuffer, WORD wDataSize);

  //��������
protected:
  //�Ƿ�ǰ�����û�
  bool IsCurrentUser(WORD wCurrentUser)
  {
    return (!IsLookonMode() && wCurrentUser==GetMeChairID());
  }

  //��Ϣӳ��
protected:
  //��ʼ��ť
  LRESULT OnStart(WPARAM wParam, LPARAM lParam);
  //��ʾ��ť
  LRESULT OnHintOx(WPARAM wParam, LPARAM lParam);
  //ţţ��ť
  LRESULT OnOx(WPARAM wParam, LPARAM lParam);
  //̯�ư�ť
  LRESULT OnOpenCard(WPARAM wParam, LPARAM lParam);
  //�������
  LRESULT OnSendCardFinish(WPARAM wParam, LPARAM lParam);
  //��ʾ����
  LRESULT OnSetUserOxValue(WPARAM wParam, LPARAM lParam);
  //��ʾ��Ϣ
  LRESULT OnShowInfo(WPARAM wParam, LPARAM lParam);
  //ţBTΪ��
  LRESULT OnOxEnable(WPARAM wParam, LPARAM lParam);
  //�˿˷���
  LRESULT OnSortCard(WPARAM wParam, LPARAM lParam);
  //�˿˷���
  LRESULT OnReSortCard(WPARAM wParam, LPARAM lParam);
  //�����Ϣ
  LRESULT OnStorageInfo(WPARAM wParam,LPARAM lParam);
  //��������
  LRESULT OnAddUserRoster(WPARAM wParam,LPARAM lParam);
  //ɾ������
  LRESULT OnDeleteUserRoster(WPARAM wParam,LPARAM lParam);
  //�����������
  LRESULT OnRequestUpdateUserRoster(WPARAM wParam,LPARAM lParam);

public:
  afx_msg void OnTimer(UINT nIDEvent);

  DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////
