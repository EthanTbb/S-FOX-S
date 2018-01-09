#pragma once

#include "Stdafx.h"
#include "ScoreView.h"
#include "CardControl.h"
#include "GoldControl.h"
#include "JettonControl.h"
#include "NumberControl.h"
#include "ChatEdit.h"
#include "SkinButtonEx.h"
#include "ChatMessage.h"
#include "ClientControl.h"

//////////////////////////////////////////////////////////////////////////
//��Ϣ����

//��Ϣ����
#define IDM_START         WM_USER+100             //��ʼ��Ϣ
#define IDM_ADD_SCORE       WM_USER+101             //��ע��Ϣ
#define IDM_CONFIRM         WM_USER+109             //ȷ����Ϣ  
#define IDM_CANCEL_ADD        WM_USER+119             //ȡ����Ϣ  
#define IDM_MIN_SCORE       WM_USER+102             //���ټ�ע
#define IDM_MAX_SCORE       WM_USER+103             //����ע
#define IDM_LOOK_CARD       WM_USER+104             //������Ϣ
#define IDM_COMPARE_CARD      WM_USER+105             //������Ϣ
#define IDM_OPEN_CARD       WM_USER+106             //������Ϣ
#define IDM_GIVE_UP         WM_USER+107             //������Ϣ
#define IDM_SEND_CARD_FINISH    WM_USER+108             //�������
#define IDM_COMPARE_USER      WM_USER+110             //��ѡ�û�
#define IDM_FALSH_CARD_FINISH   WM_USER+114             //�������
#define IDM_GAME_SOUND        WM_USER+115             //��Ϸ����
#define IDM_GAME_CLOSE        WM_USER+116             //��Ϸ�ر�
#define WM_SET_CAPTION        WM_USER+117             //���ı���

//////////////////////////////////////////////////////////////////////////
//�ṹ����

//��������
struct tagSendCardItem
{
  WORD              wChairID;             //�����û�
  BYTE              cbCardData;             //��������
};

//������Ϣ
struct tagGoldMes
{
  LONGLONG            lGoldType;              //��������
  LONGLONG            lDrawCount;             //�滭����
};

//����˵��
typedef CWHArray<tagSendCardItem,tagSendCardItem &> CSendCardItemArray;

//////////////////////////////////////////////////////////////////////////

//��Ϸ��ͼ
class CGameClientView : public CGameFrameViewGDI
{
  //���ݱ���
protected:
  WORD              m_wBankerUser;            //ׯ���û�
  LONGLONG            m_lMaxCellScore;          //�����ע
  LONGLONG            m_lCellScore;           //������ע
  LONGLONG            m_lTableScore[GAME_PLAYER];     //��ע��Ŀ
  LONGLONG            m_lStopUpdataScore[GAME_PLAYER];  //������Ŀ
  bool              m_bInvest;              //�ȴ���ע
  bool              m_bCompareCard;           //ѡ�ȱ�־
  WORD              m_wWaitUserChoice;          //�ȴ���־
  bool              m_bShowControlBack;         //�ؼ�����
  bool              m_bShowChatMessage;         //
  INT               m_nHeight;              //���ڸ߶�
  INT               m_nWidth;             //���ڿ��
  bool              m_bChangSence;            //�����л�
  bool              m_bCloseEye[GAME_PLAYER];     //գ��״̬
  BYTE              m_cbWink[GAME_PLAYER];        //��գ���
  BYTE              m_cbUserIndex[GAME_PLAYER];     //��������

  //��������
public:
  BYTE              m_cbPlayStatus[GAME_PLAYER];    //��Ϸ״̬
  bool              m_bShow;              //��ʾ����
  TCHAR             m_tcBuffer[GAME_PLAYER][64];    //�Ƶ�����
  bool              m_bCheatUser;           //�����û�

  //���Ʊ���
protected:
  WORD              m_wConmareIndex;          //��������
  WORD              m_wConmareCount;          //������Ŀ
  WORD              m_wCompareChairID[2];       //�����û�

  //���Ʊ���
protected:
  bool              m_bFalsh;             //���Ʋ���
  WORD              m_wFlashUser[GAME_PLAYER];      //�����û�
  WORD              m_wFalshCount;            //���ƴ���

  //������־
protected:
  bool              m_bCartoon;             //������־
  WORD              m_wLoserUser;           //���ưܻ�
  bool              m_bStopDraw;            //���Ʊ�־
  CSendCardItemArray        m_SendCardItemArray;        //��������

  //λ�ñ���
protected:
  CPoint              m_ptUserCompare;          //����λ��
  CPoint              m_SendCardPos;            //����λ��
  CPoint              m_ptUserCard[GAME_PLAYER];      //����λ��
  CPoint              m_KeepPos;              //����λ��
  CPoint              m_ptPlayerFrame[GAME_PLAYER];   //��ҿ��
  CPoint              m_ptUser[GAME_PLAYER];        //����λ��

  INT               m_nXFace;
  INT               m_nYFace;
  INT               m_nXTimer;
  INT               m_nYTimer;
  //�û�״̬
protected:
  BOOL              m_bCompareUser[GAME_PLAYER];    //��ѡ�����û�

  //��ť�ؼ�
public:
  CSkinButton           m_btStart;              //��ʼ��ť
  CSkinButton           m_btFollow;             //��ע��ť
  CSkinButton           m_btGiveUp;             //������ť
  CSkinButton           m_btLookCard;           //���ư�ť
  CSkinButton           m_btCompareCard;          //���ư�ť
  CSkinButton           m_btClose;              //�رհ�ť
  CSkinButton           m_btMin;              //��С��ť
  CSkinButton           m_btJetton_1;           //��ע��ť
  CSkinButton           m_btJetton_2;           //��ע��ť
  CSkinButton           m_btJetton_5;           //��ע��ť
  CSkinButton           m_btSound;              //������ť
  CSkinButton           m_btSend;             //���Ͱ�ť
  CSkinButton           m_btOption;             //���ð�ť
  CSkinButtonEx         m_btHistoryChat;          //�鿴����

  //�ؼ�����
public:
  CScoreView            m_ScoreView;            //�ɼ�����
//  CGoldControl          m_GoldControl;            //��ע��ͼ
  CCardControl          m_CardControl[GAME_PLAYER];     //�û��˿�
  CJettonControl          m_JettonControl;          //����ؼ�
  CNumberControl          m_NumberControl[GAME_PLAYER];   //���ֿؼ�
  CDFontEx            m_DFontEx;              // ����
  CDFontEx            m_CaptionFontEx;          // ����
  CEdit             m_ChatInput;            //����ؼ�
  CChatEdit           m_ChatDisplay;            //��ʾ�ؼ�
  CBrush              m_brush;              //������ˢ
  CString             m_strCaption;
  DWORD             m_dwChatTime;
  CChatMessage          m_ChatMessage;

  //λ����Ϣ
protected:
  CPoint              m_PointBanker[GAME_PLAYER];     //ׯ��λ��
  CPoint              m_ptJettons[GAME_PLAYER];     //����λ��
  CWHArray<tagGoldMes>      m_lDrawGold[GAME_PLAYER];     //�滭��Ŀ
  CWHArray<CPoint>        m_ptKeepJeton[GAME_PLAYER];     //���λ��

  //�������
protected:
  CBitImage           m_ImageBanker;            //ׯ����Դ
  CPngImage           m_ImageCard;            //�˿���Դ
  CPngImage           m_ImageComPareCard;         //�˿���Դ
  CBitImage           m_ImageTitle;           //������Դ
  CBitImage           m_ImageViewBack;          //������Դ
  CBitImage           m_ImageViewCenter;          //������Դ
  CBitImage           m_ImageJeton;           //������Դ
  CBitImage           m_ImageArrowhead;         //��ͷ��Դ
  CBitImage           m_ImageReady;           //׼����Դ
  CBitImage           m_ImagePocket;            //�����Դ
  CBitImage           m_ImageNumber;            //������Դ
  CBitImage           m_ImageBalance;           //������Դ

  CPngImage           m_PngImageViewBack_1;       //������Դ
  CPngImage           m_PngImageViewBack_2;       //������Դ
  CPngImage           m_PngImageTable;          //������Դ
  CPngImage           m_PngImagePlayerFrame;        //��ҿ��
  CPngImage           m_PngImageJetonBack;        //��ť����
  CPngImage           m_PngImageFrameBack;        //��ܱ���
  CPngImage           m_PngFengding;            //�ⶥ��
  CPngImage           m_PngImageFrameBackBottom;      //

  CPngImage           m_PngImageTimeBack;         //ʱ�䱳��
  CPngImage           m_PngImageTimeNumber;       //ʱ������
  CPngImage           m_PngaImageXZNumber;        //��ע����
  CPngImage           m_PngImageFDNumber;         //�ⶥ����
  CPngImage           m_PngImageChatBack;         //���챳��
  CPngImage           m_PngImageMyself;         //�������
  CPngImage           m_PngImageArrow;          //��ͷ��Դ
  CPngImage           m_PngSelectPlayer;          //ѡ�����
  CPngImage           m_PngWaitSelect;          //�ȴ�����

  CPngImageEx           m_ImageUser0;           // ����ͼƬ
  CPngImageEx           m_ImageUser1;           // ����ͼƬ
  CPngImageEx           m_ImageUser2;           // ����ͼƬ
  CPngImageEx           m_ImageUser3;           // ����ͼƬ
  CPngImageEx           m_ImageUser4;           // ����ͼƬ
  CPngImageEx           m_ImageUser[GAME_PLAYER];     // ����ͼƬ
  CPngImageEx           m_ImageEye[GAME_PLAYER];      // ����ͼƬ

  //�ͻ��˿���
public:
  HINSTANCE           m_hInst;              //ʵ�����
  IClientControlDlg*        m_pClientControlDlg;        //�ӿ�ָ��
  CButton             m_btOpenAdmin;            //ϵͳ����

  //��������
public:
  //���캯��
  CGameClientView();
  //��������
  virtual ~CGameClientView();

public:
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
  virtual VOID RectifyControl(INT nWidth, INT nHeight);
  //�滭����
  virtual VOID DrawGameView(CDC * pDC, INT nWidth, INT nHeight);
  //�Ƿ��������
  virtual bool RealizeWIN7()
  {
    return true;
  }
  // �滭����
  void DrawUser(CDC * pDC,INT nWidth, INT nHeight);

  //���ܺ���
public:
  //��ʼ���ֹ���
  void BeginMoveNumber();
  //ֹͣ���ֹ���
  void StopMoveNumber();
  //������ע
  void SetUserTableScore(WORD wChairID, LONGLONG lTableScore,LONGLONG lCurrentScore);
  //��ʼ���붯��
  void BeginMoveJettons();
  //ֹͣ���붯��
  void StopMoveJettons();
  //ʤ�����
  void SetGameEndInfo(WORD wWinner);
  //�ȴ�ѡ��
  void SetWaitUserChoice(WORD wChoice);
  //ׯ�ұ�־
  void SetBankerUser(WORD wBankerUse);
  //������Ϣ
  void SetScoreInfo(LONGLONG lMaxCellScore,LONGLONG lCellScore);
  //ѡ�ȱ�־
  void SetCompareCard(bool bCompareCard,BOOL bCompareUser[]);
  //ֹͣ����
  void StopUpdataScore(bool bStop);
  //�滭�˿�
  void DrawCompareCard(CDC * pDC, INT nXPos, INT nYPos, bool bChapped);
  //��������
  void DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, int nXPos, int nYPos,UINT nFormat);
  //��������
  void DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, LPRECT lpRect,UINT nFormat=DT_VCENTER|DT_CENTER|DT_SINGLELINE|DT_END_ELLIPSIS);
  //�����ж�
  bool IsDispatchCard()
  {
    return m_bCartoon;
  }
  //��ȡ��Ϣ
  bool GetCompareInfo()
  {
    return m_bCompareCard;
  };
  //������ͼ
  void RefreshGameView();
  //�滭ʱ��
  void DrawUserTimerEx(CDC * pDC, INT nXPos, INT nYPos, WORD wUserClock);
  //����Ч��
  bool EfficacyUserChat(LPCTSTR pszChatString, WORD wExpressionIndex);
  //�ؼ�����
  void SetControlBack(bool bShow)
  {
    m_bShowControlBack = bShow;
  }
  //�����л�
  void SwitchSence()
  {
    m_bChangSence = !m_bChangSence;
  }
  //�����û�
  void LoadUserImage(WORD wChairID,INT nGender);
  //ж���û�
  void DestroyUserImage(WORD wChairID);
  //����Ա����
  VOID OnBnClickedAdminWnd();

  //��������
public:
  //���ƶ���
  void PerformCompareCard(WORD wCompareUser[2], WORD wLoserUser);
  //���ƶ���
  void DispatchUserCard(WORD wChairID, BYTE cbCardData);
  //���ƶ���
  bool bFalshCard(WORD wFalshUser[]);
  //���ƴ���
  bool SendCard();
  //���ƴ���
  bool FlashCard();
  //���ƴ���
  bool CompareCard();
  //ֹͣ����
  void StopFlashCard();
  //ֹͣ����
  void StopCompareCard();
  //��ɷ���
  void FinishDispatchCard();

  //��Ϣӳ��
protected:
  //��������
  afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
  //��ʱ����Ϣ
  afx_msg void OnTimer(UINT nIDEvent);
  //�����Ϣ
  afx_msg void OnLButtonDown(UINT nFlags, CPoint Point);
  //�ؼ���ɫ
  afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
  //������Ϣ
  afx_msg void OnSendMessage();
  //�鿴����
  afx_msg void OnHideChatMessage();
  //��������
  afx_msg LRESULT OnSetCaption(WPARAM wParam, LPARAM lParam);

  DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////
