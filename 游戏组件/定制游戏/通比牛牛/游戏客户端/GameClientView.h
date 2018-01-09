#pragma once

#include "Stdafx.h"
#include "CardControl.h"
#include "HistoryRecord.h"
#include "ClientControl.h"

//////////////////////////////////////////////////////////////////////////

//��Ϣ����
#define IDM_HINT_OX         WM_USER+99              //��ʾ��Ϣ
#define IDM_START         WM_USER+100             //��ʼ��Ϣ
#define IDM_READY         WM_USER+101             //̯����Ϣ
#define IDM_OX            WM_USER+102             //ţţ��Ϣ
#define IDM_MAX_SCORE       WM_USER+103             //����ע
#define IDM_SEND_CARD_FINISH    WM_USER+104             //�������
#define IDM_SHOW_VALUE        WM_USER+105             //������Ϣ
#define IDM_SHOW_INFO       WM_USER+106             //��ʾ��Ϣ
#define IDM_OX_ENABLE       WM_USER+107             //ţţ��ť
#define IDM_SORT          WM_USER+108             //�˿˷���
#define IDM_RESORT          WM_USER+109             //������Ϣ

#define MY_VIEW_CHAIRID       3                 //����λ��

#define DRAW_HEIGHT         34                  //�滭�߶�
/////////////////////////////////////////////////////////////////////////
//�ṹ����

//����״̬
struct tagJettonStatus
{
  //������Ϣ
  WORD              wMoveCount;             //�ƶ�����
  WORD              wMoveIndex;             //�ƶ�����

  //������Ϣ
  CPoint              ptFrom;               //����λ��
  CPoint              ptDest;               //Ŀ��λ��
  CPoint              ptCourse;             //����λ��
  LONGLONG            lGold;                //������Ŀ

  //�ƶ���ʽ
  INT               iMoveType;              //�ƶ���ʽ
};

//��������
struct tagSendCardItem
{
  WORD              wChairID;             //�����û�
  BYTE              cbCardData;             //��������
};

//����˵��
typedef CWHArray<tagSendCardItem,tagSendCardItem &> CSendCardItemArray;

//////////////////////////////////////////////////////////////////////////

//��Ϸ��ͼ
class CGameClientView : public CGameFrameViewGDI
{
  //���ݱ���
protected:
  BYTE              m_cbIndex;              //ţţ����
  WORD              m_wBankerUser;            //ׯ���û�
  LONGLONG            m_lCellScore;           //��Ϸ�׷�
  LONGLONG            m_lTableScore[GAME_PLAYER];     //��ע��Ŀ
  BYTE              m_cbOxValue[GAME_PLAYER];     //ţţ����
  BYTE              m_cbOpenCardUser[GAME_PLAYER];    //̯���û�
  BYTE                            m_cbDynamicJoin;                    //��̬����
  bool              m_bInvest;              //�ȴ���ע
  bool              m_bShowSocre[GAME_PLAYER];      //��ʾ����
  bool              m_bUserAction;            //�ȴ���־
  bool              m_bJettonAction;          //�ƶ���ʾ
  tagJettonStatus         m_JettonStatus[GAME_PLAYER];    //������Ϣ
  LONGLONG            m_lMoveGold[GAME_PLAYER];     //������Ϣ

  //��������
public:
  bool              m_bShow;              //��ʾ����
  bool              m_bHandOpen;            //�Զ�����
  bool              m_bLookOnUser;            //�Թ۱�־
  bool              m_bOpenCard;            //�ȴ�̯��
  bool              m_bShowScore;           //����״̬
  bool              m_bShowCellScore;         //����״̬
  tagHistoryScore *       m_pHistoryScore[GAME_PLAYER];   //������Ϣ

  //��������
protected:
  WORD              m_wSendCount;           //�ƶ�����
  WORD              m_wSendIndex;           //�ƶ�����
  CPoint              m_ptSendCardPos;          //����λ��
  CPoint              m_ptSendEndingPos[GAME_PLAYER];   //����λ��
  CPoint              m_ptSendStartPos;         //ʼ��λ��
  CSendCardItemArray        m_SendCardItemArray;        //��������

  //�ƹ⶯��
protected:
  bool              m_bShowCartoon;           //��ʾ��ʶ
  WORD              m_wCartoonLightIndex;       //��������

  //��ť�ؼ�
public:
  CSkinButton           m_btHintOx;             //��ʾ��ť
  CSkinButton           m_btStart;              //��ʼ��ť
  CSkinButton           m_btOpenCard;           //̯�ư�ť
  CSkinButton           m_btOx;               //ţţ��ť
  CSkinButton           m_btHandOpenCard;         //�ֶ�����
  CSkinButton           m_btAutoOpenCard;         //�Զ�����
  CSkinButton           m_btScore;              //���ְ�ť

  //�ؼ�����
public:
  CCardControl          m_CardControl[GAME_PLAYER];     //�û��˿�
  CCardControl          m_CardControlOx[GAME_PLAYER];   //�û��˿�

  //�ͻ��˿���
public:
  bool              m_bCheck;             //���Ʊ�ʶ
  CRect             m_rcCheck;              //��Ӧ����
  HINSTANCE           m_hInst;              //ʵ�����
  IClientControlDlg*        m_pClientControlDlg;        //�ӿ�ָ��
  CButton             m_btOpenAdmin;            //ϵͳ����

  //λ����Ϣ
protected:
  CPoint              m_ptBanker[GAME_PLAYER];      //ׯ��λ��
  CPoint              m_ptJeton[GAME_PLAYER];       //���λ��
  CPoint              m_ptValue[GAME_PLAYER];       //����λ��
  CPoint              m_ptOpenTag[GAME_PLAYER];     //̯��λ��
  CPoint              m_ptCard[GAME_PLAYER];        //���λ��
  CSize             m_sizeHistory;            //���ִ�С

  //�������
protected:
  CBitImage           m_ImageCard;            //�˿���Դ
  CBitImage           m_ImageViewBack;          //������Դ
  CBitImage           m_ImageViewCenter;          //������Դ
  CBitImage           m_ImageJeton;           //������Դ
  CBitImage           m_ImageOpenCard;          //̯����Դ
  CPngImage           m_ImageOxValueZero;         //ţֵ��Դ
  CPngImage           m_ImageDoulbeOx;          //ţţ��Դ
  CPngImage           m_ImageNumber;            //������Դ
  CPngImage           m_ImageOxValue;           //ţֵ��Դ
  CPngImage             m_ImageTimeBack;          //ʱ�䱳��
  CPngImage             m_ImageTimeNumber;          //ʱ������
  CPngImage           m_ImageBanker;            //ׯ����Դ
  CPngImage           m_PngUserBackL;           //��ҵ׿�
  CPngImage           m_PngUserBackR;           //��ҵ׿�
  CPngImage           m_PngUserBackMe;          //��ҵ׿�
  CPngImage           m_PngAutoOpen;            //�Զ�����
  CPngImage           m_PngWaitOpen;            //�ȴ�����
  CPngImage           m_PngOpenTag;           //���Ʊ�ʾ
  CPngImage                       m_PngReady;                         //׼����ʶ
  CPngImage           m_PngCartoonLight;          //��������
  CPngImage           m_PngHistoryScore;          //��ʷ����
  CPngImage           m_PngScoreBack;           //�׷ֱ���

  //�ʽ𶯻�
public:
  WORD              m_wBonusCartoonIndex;
  CPngImage                       m_PngBonus;                         //�ʽ𶯻�
  CPngImageEx                     m_PngBonusNum;                      //�ʽ𶯻�
  bool              m_bShowBonusCartoon;
  bool              m_bBonus[GAME_PLAYER];
  CPoint              m_ptBonus[GAME_PLAYER];
  LONGLONG            m_lBonus;

  //��������
public:
  //���캯��
  CGameClientView();
  //��������
  virtual ~CGameClientView();
  //��Ϣ����
  virtual BOOL PreTranslateMessage(MSG * pMsg);

  //���غ���
protected:
  //�����
  virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);

  //�̳к���
private:
  //���ý���
  virtual VOID ResetGameView();
  //�����ؼ�
  virtual VOID RectifyControl(int nWidth, int nHeight);
  //�滭����
  virtual VOID DrawGameView(CDC * pDC, int nWidth, int nHeight);

  //WIN7֧��
public:
  virtual bool RealizeWIN7()
  {
    return true;
  }

  //���ܺ���
public:
  //�滭ʱ��
  void DrawUserTimerEx(CDC * pDC, int nXPos, int nYPos, WORD wTime, WORD wTimerArea=99);
  // �滭����
  void DrawNumber(CDC * pDC, CPngImageEx* ImageNumber, TCHAR * szImageNum, LONGLONG lOutNum, INT nXPos, INT nYPos, UINT uFormat = DT_RIGHT);
  // �滭����
  void DrawNumber(CDC * pDC, CPngImageEx* ImageNumber, TCHAR * szImageNum, TCHAR* szOutNum ,INT nXPos, INT nYPos,  UINT uFormat = DT_RIGHT);
  //��ʾ��Ϣ
  void SetUserAction(bool bWait);
  //�����ж�
  bool IsDispatchCard();
  //��ɷ���
  void FinishDispatchCard();
  //�����˿�
  void DispatchUserCard(WORD wChairID, BYTE cbCardData);
  //������ע
  void SetUserTableScore(WORD wChairID, LONGLONG lTableScore,bool bMove=false);
  //ׯ�ұ�־
  void SetBankerUser(WORD wBankerUser);
  //�ȴ���־
  void SetWaitInvest(bool Invest);
  //���ö�̬�����־
  void SetUserDynamicJoin(BYTE cbDynamicJoin)
  {
    m_cbDynamicJoin=cbDynamicJoin;
  };
  //��ʾ����
  void DisplayTypeEx(bool bShow);
  //���õ���
  void SetUserOxValue(WORD wChiarID,BYTE cbValue);
  //��ȡ����
  BYTE GetValue(WORD wChiarID)
  {
    return m_cbOxValue[wChiarID];
  }
  //��ʾ̯��
  void ShowOpenCard(WORD wChiarID,BOOL bShow=TRUE);
  //��ʾ����
  void ShowScore(WORD wChiarID,bool bShow);
  //��ʾ����
  void ShowCartonnLight(bool bShow);
  //��ʾ�׷�
  void ShowCellScore(LONGLONG lScore, bool bShow);
  //��������
  void DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, int nXPos, int nYPos);
  //��������
  void DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, LPRECT lpRect);
  //���ְ�ť
  VOID OnBnClickedScore();
  //�Զ�����
  VOID OnBnClickedAutoOpen();
  //�ֶ�����
  VOID OnBnClickedHandOpen();
  //����Ա����
  VOID OnBnClickedAdminWnd();
  //����Ȩ��
  VOID SetAdminCheck(bool bCheck);
  //���û���
  bool SetHistoryScore(WORD wChairID,tagHistoryScore * pHistoryScore);
  //���ƶ���
  void DrawMoveAnte(WORD wViewChairID, LONGLONG lTableScore);
  //�ƶ�����
  bool MoveJetton();
  //�����û�ͼ��
  void DrawUserReadyEx(CDC *pDC,int nXPos,int nYPos);
  //������ͼ
  void RefreshGameView();
  //������ͼ
  void RefreshGameView(CRect &rect);
  //��ʼ�ʽ𶯻�
  void StartBonusCartoon();
  //���ö�����־
  void SetBonusFlag(WORD wViewChairID, bool bBonus);
  //���òʽ�
  void SetBonus(LONGLONG lBonus)
  {
    if(lBonus != m_lBonus)
    {
      m_lBonus = lBonus;
    }
  }
  //��Ϣӳ��
protected:
  //��������
  afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
  //�����Ϣ
  afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
  //��ʱ����Ϣ
  afx_msg void OnTimer(UINT nIDEvent);

  DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////
