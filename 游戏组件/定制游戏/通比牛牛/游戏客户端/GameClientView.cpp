#include "StdAfx.h"
#include "Resource.h"
#include "GameClient.h"
#include "GameClientView.h"
#include "GameClientEngine.h"

//////////////////////////////////////////////////////////////////////////
//�궨��

//��ť��ʶ
#define IDC_START           100               //��ʼ��ť
#define IDC_HINT_OX           101               //��ʾ��ť
#define IDC_READY           102               //̯�ư�ť
#define IDC_RESORT            103               //���Ű�ť
#define IDC_OX              104               //ţţ��ť
#define IDC_SCORE           105               //�鿴����
#define IDC_HAND_OPEN         106               //�ֶ�����
#define IDC_AUTO_OPEN         107               //�Զ�����
#define IDC_ADMIN_WND         108               //���ư�ť

//��ʱ����ʶ
#define IDI_GOLD            97                //���ƶ�ʱ��
#define IDI_SEND_CARD         98                //���ƶ�ʱ��
#define IDI_USER_ACTION         99                //������ʱ��
#define IDI_UPDATA_GAME         100               //���¶�ʱ��
#define IDI_JETTON_ACTION       101               //������ʱ��
#define IDI_CARTOON_LIGHT       102               //������ʱ��
#define IDI_BONUS           104               //�ʽ�ʱ��

#define BONUS_FPS           26
#define BONUS_PERPOSX         320
#define BONUS_PERPOSY         240
//��ʱ��ʱ��
#define TIME_USER_ACTION        4000              //������ʱ��

//�ƶ��ٶ�
#define SEND_PELS           160               //�����ٶ�
#define TIME_SENDSPEED          5               //����ٶ�


//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameClientView, CGameFrameViewGDI)
  ON_WM_CREATE()
  ON_BN_CLICKED(IDC_SCORE, OnBnClickedScore)
  ON_BN_CLICKED(IDC_AUTO_OPEN, OnBnClickedAutoOpen)
  ON_BN_CLICKED(IDC_HAND_OPEN, OnBnClickedHandOpen)
  ON_BN_CLICKED(IDC_ADMIN_WND, OnBnClickedAdminWnd)
  ON_WM_LBUTTONDOWN()
  ON_WM_TIMER()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameClientView::CGameClientView()
{
  //��������
  m_cbIndex=0;
  m_lCellScore=0;
  m_bJettonAction=false;
  m_cbDynamicJoin=FALSE;

  m_wBonusCartoonIndex = 0;
  m_bShowBonusCartoon = false;
  ZeroMemory(m_bBonus, sizeof(m_bBonus));
  m_lBonus = 0;

  for(WORD i=0; i<GAME_PLAYER; i++)
  {
    m_cbOxValue[i]=0xFF;
    m_ptSendEndingPos[i].SetPoint(0,0);
  }
  m_wSendCount=0;
  m_wSendIndex=0;
  m_ptSendCardPos.SetPoint(0,0);
  m_SendCardItemArray.RemoveAll();
  ZeroMemory(m_JettonStatus,sizeof(m_JettonStatus));
  ZeroMemory(m_lMoveGold,sizeof(m_lMoveGold));

  m_bShowCartoon = false;
  m_wCartoonLightIndex = 0;

  //���ݱ���
  ZeroMemory(m_lTableScore,sizeof(m_lTableScore));
  ZeroMemory(m_cbOpenCardUser,sizeof(m_cbOpenCardUser));
  ZeroMemory(m_bShowSocre,sizeof(m_bShowSocre));
  m_wBankerUser=INVALID_CHAIR;
  m_bShow=false;
  m_bHandOpen=false;
  m_bInvest=false;
  m_bOpenCard=false;
  m_bUserAction=false;
  m_bLookOnUser=false;
  m_bShowScore=false;
  m_bShowCellScore=false;
  ZeroMemory(m_pHistoryScore,sizeof(m_pHistoryScore));

  m_bCheck=false;
  m_pClientControlDlg = NULL;
  m_hInst = NULL;

  //����λͼ
  HINSTANCE hInstance=AfxGetInstanceHandle();
  m_ImageJeton.LoadFromResource(hInstance, IDB__MONEY);
  m_ImageCard.LoadFromResource(hInstance, IDB_CARD);
  m_ImageViewBack.LoadFromResource(hInstance, IDB_VIEW_BACK);
  m_ImageViewCenter.LoadFromResource(hInstance, IDB_VIEW_FILL);

  //������Դ
  m_PngReady.LoadImage(hInstance,TEXT("READY"));
  m_PngBonus.LoadImage(hInstance,TEXT("BONUS"));
  m_PngBonusNum.LoadImage(this, hInstance, TEXT("BONUSNUM"));
  m_PngUserBackL.LoadImage(hInstance,TEXT("USER_HEAD_BACK_L"));
  m_PngUserBackR.LoadImage(hInstance,TEXT("USER_HEAD_BACK_R"));
  m_PngUserBackMe.LoadImage(hInstance,TEXT("USER_HEAD_BACK_ME"));
  m_ImageOxValueZero.LoadImage(hInstance,TEXT("COW_VALUE_ZERO"));
  m_ImageDoulbeOx.LoadImage(hInstance,TEXT("DOUBLE_COW"));
  m_ImageNumber.LoadImage(hInstance,TEXT("NUMBER"));
  m_ImageOxValue.LoadImage(hInstance,TEXT("COW_VALUE"));
  m_ImageTimeBack.LoadImage(hInstance,TEXT("TIME_BACK"));
  m_ImageTimeNumber.LoadImage(hInstance,TEXT("TIME_NUMBER"));
  m_ImageBanker.LoadImage(hInstance,TEXT("BANKER_FLAG"));
  m_PngAutoOpen.LoadImage(hInstance,TEXT("AUTO_OPEN"));
  m_PngWaitOpen.LoadImage(hInstance,TEXT("WAIT_OPEN"));
  m_PngOpenTag.LoadImage(hInstance,TEXT("OPEN_TAG"));
  m_PngCartoonLight.LoadImage(hInstance,TEXT("CARTOON_LIGHT"));
  m_PngHistoryScore.LoadImage(hInstance,TEXT("HISTORY_SCORE"));
  m_PngScoreBack.LoadImage(hInstance,TEXT("SCORE_BACK"));

  //��ȡ��С
  m_sizeHistory.SetSize(m_PngHistoryScore.GetWidth(),m_PngHistoryScore.GetHeight());

  return;
}

//��������
CGameClientView::~CGameClientView()
{
  if(m_pClientControlDlg != NULL)
  {
    delete m_pClientControlDlg;
    m_pClientControlDlg = NULL;
  }

  if(m_hInst != NULL)
  {
    FreeLibrary(m_hInst);
    m_hInst = NULL;
  }
}

//��Ϣ����
BOOL CGameClientView::PreTranslateMessage(MSG * pMsg)
{
  return __super::PreTranslateMessage(pMsg);
}

//������Ϣ
int CGameClientView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
  if(__super::OnCreate(lpCreateStruct)==-1)
  {
    return -1;
  }

  //�����ؼ�
  CRect rcCreate(0,0,0,0);

  //������ť
  m_btHintOx.Create(TEXT(""),WS_CHILD,rcCreate,this,IDC_HINT_OX);
  m_btStart.Create(TEXT(""),WS_CHILD,rcCreate,this,IDC_START);
  m_btOpenCard.Create(TEXT(""),WS_CHILD,rcCreate,this,IDC_READY);
  m_btOx.Create(TEXT(""),WS_CHILD,rcCreate,this,IDC_OX);
  m_btScore.Create(NULL,WS_CHILD,rcCreate,this,IDC_SCORE);
  m_btHandOpenCard.Create(NULL,WS_CHILD,rcCreate,this,IDC_HAND_OPEN);
  m_btAutoOpenCard.Create(NULL,WS_CHILD,rcCreate,this,IDC_AUTO_OPEN);

  //����λͼ
  HINSTANCE hInstance=AfxGetInstanceHandle();
  m_btHintOx.SetButtonImage(IDB_HINT_OX,hInstance,false,false);
  m_btStart.SetButtonImage(IDB_START,hInstance,false,false);
  m_btOpenCard.SetButtonImage(IDB_OPENCARD,hInstance,false,false);
  m_btOx.SetButtonImage(IDB_COW,hInstance,false,false);
  m_btScore.SetButtonImage(IDB_BT_SHOW_SCORE,hInstance,false,false);
  m_btHandOpenCard.SetButtonImage(IDB_BT_HAND_OPEN,hInstance,false,false);
  m_btAutoOpenCard.SetButtonImage(IDB_BT_AUTO_OPEN,hInstance,false,false);

  //�����ؼ�
  for(WORD i=0; i<GAME_PLAYER; i++)
  {
    m_CardControl[i].Create(NULL,NULL,WS_VISIBLE|WS_CHILD,rcCreate,this,20+i);
    m_CardControlOx[i].Create(NULL,NULL,WS_VISIBLE|WS_CHILD,rcCreate,this,20+GAME_PLAYER+i);
    m_CardControlOx[i].SetDrawHeight(DRAW_HEIGHT);
  }

  //���ÿؼ�
  for(int i=0; i<GAME_PLAYER; i++)
  {
    m_CardControlOx[i].SetDisplayFlag(true);
    m_CardControlOx[i].SetCardSpace(18);
    m_CardControl[i].SetCardSpace(18);
  }

  //�ͻ��˿���
  m_hInst = NULL;
  m_pClientControlDlg = NULL;
  m_hInst = LoadLibrary(TEXT("OxSixExClientControl.dll"));
  if(m_hInst != NULL)
  {
    typedef void * (*CREATE)(CWnd* pParentWnd);
    CREATE ClientControl = (CREATE)GetProcAddress(m_hInst,"CreateClientControl");
    if(ClientControl != NULL)
    {
      m_pClientControlDlg = static_cast<IClientControlDlg*>(ClientControl(this));
      m_pClientControlDlg->ShowWindow(SW_HIDE);
    }
  }

  //���ư�ť
  m_btOpenAdmin.Create(NULL,WS_CHILD|WS_VISIBLE,CRect(4,4,11,11),this,IDC_ADMIN_WND);
  m_btOpenAdmin.ShowWindow(SW_HIDE);

  return 0;
}

//�����
BOOL CGameClientView::OnCommand(WPARAM wParam, LPARAM lParam)
{
  switch(LOWORD(wParam))
  {
    case IDC_HINT_OX:     //��ʾ��ť
    {
      //������Ϣ
      SendEngineMessage(IDM_HINT_OX,0,0);
      return TRUE;
    }
    case IDC_START:       //��ʼ��ť
    {
      //������Ϣ
      SendEngineMessage(IDM_START,0,0);
      return TRUE;
    }
    case IDC_READY:       //̯�ư�ť
    {
      //������Ϣ
      SendEngineMessage(IDM_READY,0,0);
      return TRUE;
    }
    case IDC_OX:        //ţţ��ť
    {
      //������Ϣ
      SendEngineMessage(IDM_OX,0,0);
      return TRUE;
    }
    case IDC_RESORT:      //���Ű�ť
    {
      //������Ϣ
      SendEngineMessage(IDM_RESORT,0,0);
      return TRUE;
    }
  }

  return __super::OnCommand(wParam, lParam);
}

//���ý���
VOID CGameClientView::ResetGameView()
{
  __super::ResetGameView();

  //��������
  m_cbIndex=0;
  m_lCellScore=0;
  m_bJettonAction=false;
  for(WORD i=0; i<GAME_PLAYER; i++)
  {
    m_cbOxValue[i]=0xFF;
  }
  m_wSendCount=0;
  m_wSendIndex=0;
  m_cbDynamicJoin=FALSE;
  m_ptSendCardPos.SetPoint(0,0);
  m_SendCardItemArray.RemoveAll();
  ZeroMemory(m_JettonStatus,sizeof(m_JettonStatus));
  ZeroMemory(m_lMoveGold,sizeof(m_lMoveGold));

  m_bShowCartoon = false;
  m_wCartoonLightIndex = 0;

  m_wBonusCartoonIndex = 0;
  m_bShowBonusCartoon = false;
  ZeroMemory(m_bBonus, sizeof(m_bBonus));

  //���ݱ���
  ZeroMemory(m_lTableScore,sizeof(m_lTableScore));
  ZeroMemory(m_cbOpenCardUser,sizeof(m_cbOpenCardUser));
  ZeroMemory(m_bShowSocre,sizeof(m_bShowSocre));
  m_wBankerUser=INVALID_CHAIR;
  m_bShow=false;
  m_bHandOpen=false;
  m_bInvest=false;
  m_bOpenCard=false;
  m_bLookOnUser=false;
  m_bUserAction=false;
  m_bShowScore=false;
  m_bShowCellScore=false;
  ZeroMemory(m_pHistoryScore,sizeof(m_pHistoryScore));

  //ɾ����ʱ��
  KillTimer(IDI_SEND_CARD);

  //���ذ�ť
  m_btHintOx.ShowWindow(SW_HIDE);
  m_btStart.ShowWindow(SW_HIDE);
  m_btOpenCard.ShowWindow(SW_HIDE);
  m_btOx.ShowWindow(SW_HIDE);
  m_btScore.ShowWindow(SW_HIDE);
  m_btHandOpenCard.ShowWindow(SW_HIDE);
  m_btAutoOpenCard.ShowWindow(SW_HIDE);

  //��ԭ��ť
  HINSTANCE hInstance=AfxGetInstanceHandle();
  m_btScore.SetButtonImage(IDB_BT_SHOW_SCORE,hInstance,false,false);

  //���ؿؼ�
  for(WORD i=0; i<GAME_PLAYER; i++)
  {
    m_CardControl[i].SetDisplayFlag(false);
    m_CardControl[i].SetCardData(NULL,0);
    m_CardControlOx[i].SetCardData(NULL,0);
  }

  m_bCheck=false;
  m_pClientControlDlg = NULL;
  m_hInst = NULL;

  return;
}

//�����ؼ�
VOID CGameClientView::RectifyControl(int nWidth, int nHeight)
{
  SetTimer(IDI_UPDATA_GAME,1000,NULL);

  //����λ��
  m_ptSendStartPos.SetPoint(nWidth/2,nHeight/2-30);

  m_ptBonus[0].x = nWidth/2-118 + 265;
  m_ptBonus[0].y = 105;
  m_ptAvatar[0].x=nWidth/2-118;
  m_ptAvatar[0].y=105;
  m_ptNickName[0].x=m_ptAvatar[0].x + 38;
  m_ptNickName[0].y=m_ptAvatar[0].y + 90;
  m_ptJeton[0].x=nWidth/2-36;
  m_ptJeton[0].y=206;
  m_ptCard[0].x=m_ptJeton[0].x;
  m_ptCard[0].y=m_ptJeton[0].y-100;
  m_ptReady[0].x=m_ptJeton[0].x;
  m_ptReady[0].y=m_ptJeton[0].y+40;
  m_ptClock[0].x=m_ptAvatar[0].x+(FACE_CX-65)/2+32;
  m_ptClock[0].y=m_ptAvatar[0].y+FACE_CY+80;
  m_ptBanker[0].x = m_ptAvatar[0].x+5;
  m_ptBanker[0].y =m_ptAvatar[0].y-40;
  m_ptValue[0].x=nWidth/2-40;
  m_ptValue[0].y=200;

  m_ptBonus[1].x = 5;
  m_ptBonus[1].y = nHeight/2-FACE_CY/2+5-148 - 200;
  m_ptAvatar[1].x=5;
  m_ptAvatar[1].y=nHeight/2-FACE_CY/2+5-148;
  m_ptNickName[1].x=m_ptAvatar[1].x + 38;
  m_ptNickName[1].y=m_ptAvatar[1].y + 90;
  m_ptJeton[1].x=263-30;
  m_ptJeton[1].y=nHeight/2-90;
  m_ptCard[1].x=m_ptJeton[1].x-80;
  m_ptCard[1].y=m_ptJeton[1].y-60;
  m_ptReady[1].x=m_ptJeton[1].x+20;
  m_ptReady[1].y=m_ptJeton[1].y-80;
  m_ptClock[1].x=m_ptJeton[1].x+40;
  m_ptClock[1].y=m_ptJeton[1].y-100;
  m_ptBanker[1].x =m_ptAvatar[1].x+5;
  m_ptBanker[1].y =m_ptAvatar[1].y-40;
  m_ptValue[1].x=230;
  m_ptValue[1].y=nHeight/2-115;

  m_ptBonus[2].x = 5;
  m_ptBonus[2].y = nHeight/2-FACE_CY/2+5+62 + 140;
  m_ptAvatar[2].x=5;
  m_ptAvatar[2].y=nHeight/2-FACE_CY/2+5+62;
  m_ptNickName[2].x=m_ptAvatar[2].x + 38;
  m_ptNickName[2].y=m_ptAvatar[2].y + 90;
  m_ptJeton[2].x=233;
  m_ptJeton[2].y=nHeight/2+120;
  m_ptCard[2].x=m_ptJeton[2].x-80;
  m_ptCard[2].y=m_ptJeton[2].y+30;
  m_ptReady[2].x=m_ptJeton[2].x+20;
  m_ptReady[2].y=m_ptJeton[2].y-80;
  m_ptClock[2].x=m_ptJeton[2].x+40;
  m_ptClock[2].y=m_ptJeton[2].y;
  m_ptBanker[2].x =m_ptAvatar[2].x+5;
  m_ptBanker[2].y =m_ptAvatar[2].y-40;
  m_ptValue[2].x=230;
  m_ptValue[2].y=nHeight/2+90;

  m_ptBonus[3].x = nWidth/2-118-44 + 360;
  m_ptBonus[3].y = nHeight-115+2;
  m_ptAvatar[3].x=nWidth/2-118-44;
  m_ptAvatar[3].y=nHeight-115+2;
  m_ptNickName[3].x=m_ptAvatar[3].x + 38;
  m_ptNickName[3].y=m_ptAvatar[3].y + 90;
  m_ptJeton[3].x=nWidth/2-36;
  m_ptJeton[3].y=nHeight-285;
  m_ptCard[3].x=m_ptJeton[3].x;
  m_ptCard[3].y=m_ptJeton[3].y+90;
  m_ptReady[3].x=m_ptAvatar[3].x-69-10-4+230;
  m_ptReady[3].y=m_ptAvatar[3].y+(FACE_CY-48)/2+8;
  m_ptClock[3].x=m_ptAvatar[3].x+300;
  m_ptClock[3].y=m_ptAvatar[3].y+50;
  m_ptBanker[3].x = m_ptAvatar[3].x+5;
  m_ptBanker[3].y =m_ptAvatar[3].y-40;
  m_ptValue[3].x=nWidth/2-260;
  m_ptValue[3].y=nHeight-100;

  m_ptBonus[4].x = nWidth-200;
  m_ptBonus[4].y = nHeight/2-FACE_CY/2+5+62 + 240;
  m_ptAvatar[4].x=nWidth-230;
  m_ptAvatar[4].y=nHeight/2-FACE_CY/2+5+62;
  m_ptNickName[4].x=m_ptAvatar[4].x + 38;
  m_ptNickName[4].y=m_ptAvatar[4].y + 90;
  m_ptJeton[4].x=nWidth-303;
  m_ptJeton[4].y=nHeight/2+120;
  m_ptCard[4].x=m_ptJeton[4].x+80;
  m_ptCard[4].y=m_ptJeton[4].y-70;
  m_ptReady[4].x=m_ptAvatar[4].x-83;
  m_ptReady[4].y=m_ptAvatar[4].y-0;
  m_ptClock[4].x=m_ptJeton[4].x+30;
  m_ptClock[4].y=m_ptJeton[4].y;
  m_ptBanker[4].x = m_ptAvatar[4].x+175;
  m_ptBanker[4].y = m_ptAvatar[4].y-40;
  m_ptValue[4].x=nWidth-330;
  m_ptValue[4].y=nHeight/2+90;

  m_ptBonus[5].x = nWidth-200;
  m_ptBonus[5].y = nHeight/2-FACE_CY/2+5-148 - 50;
  m_ptAvatar[5].x=nWidth-230;
  m_ptAvatar[5].y=nHeight/2-FACE_CY/2+5-148;
  m_ptNickName[5].x=m_ptAvatar[5].x + 38;
  m_ptNickName[5].y=m_ptAvatar[5].y + 90;
  m_ptJeton[5].x=nWidth-303;
  m_ptJeton[5].y=nHeight/2-90;
  m_ptCard[5].x=m_ptJeton[5].x+80;
  m_ptCard[5].y=m_ptJeton[5].y-60;
  m_ptReady[5].x=m_ptAvatar[5].x-83;
  m_ptReady[5].y=m_ptAvatar[5].y-0;
  m_ptClock[5].x=m_ptJeton[5].x+30;
  m_ptClock[5].y=m_ptJeton[5].y;
  m_ptBanker[5].x = m_ptAvatar[5].x+175;
  m_ptBanker[5].y = m_ptAvatar[5].y-40;
  m_ptValue[5].x=nWidth-330;
  m_ptValue[5].y=nHeight/2-115;

  m_CardControl[0].SetBenchmarkPos(nWidth/2+30,20+20,enXCenter,enYTop);
  m_CardControlOx[0].SetBenchmarkPos(nWidth/2+30,20-DRAW_HEIGHT+20,enXCenter,enYTop);
  m_CardControl[1].SetBenchmarkPos(150,nHeight/2-150-20,enXCenter,enYCenter);
  m_CardControlOx[1].SetBenchmarkPos(150,nHeight/2-150-DRAW_HEIGHT-20,enXCenter,enYCenter);
  m_CardControl[2].SetBenchmarkPos(150,nHeight/2+60-20,enXCenter,enYCenter);
  m_CardControlOx[2].SetBenchmarkPos(150,nHeight/2+60-DRAW_HEIGHT-20,enXCenter,enYCenter);
  m_CardControl[3].SetBenchmarkPos(nWidth/2+30,nHeight-43,enXCenter,enYBottom);
  m_CardControlOx[3].SetBenchmarkPos(nWidth/2+30,nHeight-43-DRAW_HEIGHT,enXCenter,enYBottom);
  m_CardControl[4].SetBenchmarkPos(nWidth-145,nHeight/2+60-20,enXCenter,enYCenter);
  m_CardControlOx[4].SetBenchmarkPos(nWidth-145,nHeight/2+60-DRAW_HEIGHT-20,enXCenter,enYCenter);
  m_CardControl[5].SetBenchmarkPos(nWidth-145,nHeight/2-150-20,enXCenter,enYCenter);
  m_CardControlOx[5].SetBenchmarkPos(nWidth-145,nHeight/2-150-DRAW_HEIGHT-20,enXCenter,enYCenter);

  //����λ��
  m_ptSendEndingPos[0].x=m_CardControl[0].GetCardPos().x;
  m_ptSendEndingPos[0].y=m_CardControl[0].GetCardPos().y;
  m_ptSendEndingPos[1].x=m_CardControl[1].GetCardPos().x;
  m_ptSendEndingPos[1].y=m_CardControl[1].GetCardPos().y;
  m_ptSendEndingPos[2].x=m_CardControl[2].GetCardPos().x;
  m_ptSendEndingPos[2].y=m_CardControl[2].GetCardPos().y;
  m_ptSendEndingPos[3].x=m_CardControl[3].GetCardPos().x;
  m_ptSendEndingPos[3].y=m_CardControl[3].GetCardPos().y;
  m_ptSendEndingPos[4].x=m_CardControl[4].GetCardPos().x;
  m_ptSendEndingPos[4].y=m_CardControl[4].GetCardPos().y;
  m_ptSendEndingPos[5].x=m_CardControl[5].GetCardPos().x;
  m_ptSendEndingPos[5].y=m_CardControl[5].GetCardPos().y;

  //ţֵλ��
  for(WORD i = 0; i < GAME_PLAYER; i++)
  {
    m_ptOpenTag[i] = m_ptValue[i];
  }

  //��������
  m_rcCheck.SetRect(m_ptAvatar[3].x+8, m_ptAvatar[3].y+29 ,m_ptAvatar[3].x+56, m_ptAvatar[3].y+77);

  //��ť�ؼ�
  CRect rcButton;
  HDWP hDwp = BeginDeferWindowPos(32);
  const UINT uFlags = SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOCOPYBITS|SWP_NOSIZE;

  //��ʼ��ť
  m_btStart.GetWindowRect(&rcButton);
  DeferWindowPos(hDwp, m_btStart, NULL, nWidth/2-50, nHeight-245, 0, 0, uFlags);

  //���ư�ť
  m_btOpenCard.GetWindowRect(&rcButton);
  DeferWindowPos(hDwp,m_btOx, NULL, nWidth/2-118, nHeight-237, 0, 0, uFlags);
  DeferWindowPos(hDwp,m_btHintOx, NULL, nWidth/2+250, nHeight-55, 0, 0, uFlags);
  DeferWindowPos(hDwp,m_btOpenCard, NULL,nWidth/2+250, nHeight-100, 0, 0, uFlags);
  DeferWindowPos(hDwp,m_btAutoOpenCard, NULL, nWidth/2+135, nHeight-194, 0, 0, uFlags);
  DeferWindowPos(hDwp,m_btHandOpenCard, NULL, nWidth/2+135, nHeight-122, 0, 0, uFlags);

  //��ְ�ť
  CRect rcScore;
  m_btScore.GetWindowRect(&rcScore);
  DeferWindowPos(hDwp, m_btScore, NULL, nWidth-50, 5, 0, 0, uFlags);

  //�����ƶ�
  EndDeferWindowPos(hDwp);

  return;
}

//�滭����
VOID CGameClientView::DrawGameView(CDC * pDC, int nWidth, int nHeight)
{
  //�滭����
  DrawViewImage(pDC,m_ImageViewCenter,DRAW_MODE_SPREAD);
  DrawViewImage(pDC,m_ImageViewBack,DRAW_MODE_CENTENT);

  //�滭�û�
  CRect rcUserInfo(0,0,0,0);
  UINT nTextFormat = DT_LEFT|DT_TOP|DT_END_ELLIPSIS|DT_NOCLIP;
  for(BYTE i = 0; i < GAME_PLAYER; i++)
  {
    //��������
    WORD wUserTimer = GetUserClock(i);
    IClientUserItem * pIClientUserItem = GetClientUserItem(i);

    //�滭�û�
    if(pIClientUserItem != NULL)
    {
      if(i == 4 || i == 5)
      {
        //�û����
        m_PngUserBackR.DrawImage(pDC,m_ptAvatar[i].x-7,m_ptAvatar[i].y-102);

        //�滭ͷ��
        DrawUserAvatar(pDC, m_ptAvatar[i].x+172, m_ptAvatar[i].y+29-20, pIClientUserItem);

        //�û�����
        TCHAR szBuffer[64] = TEXT("");
        int nTextWidth = (i % 2 == 0 ? 80 : 64);
        _sntprintf(szBuffer,CountArray(szBuffer),TEXT("%s"), pIClientUserItem->GetNickName());
        rcUserInfo.SetRect(m_ptNickName[i].x, m_ptNickName[i].y-20, m_ptNickName[i].x+nTextWidth, m_ptNickName[i].y+18-20);
        CDFontEx::DrawText(this, pDC, 12, 400, szBuffer, rcUserInfo, RGB(255,255,255), nTextFormat);

        //�û����
        LONGLONG lLeaveScore = pIClientUserItem->GetUserScore() - (pIClientUserItem->GetUserStatus() != US_PLAYING ? 0 : m_lTableScore[i]);
        _sntprintf(szBuffer,CountArray(szBuffer),TEXT("%I64d"), lLeaveScore);
        rcUserInfo.SetRect(m_ptNickName[i].x+120, m_ptNickName[i].y-20, m_ptNickName[i].x+nTextWidth+120, m_ptNickName[i].y+56-20);
        CDFontEx::DrawText(this, pDC, 12, 400, szBuffer, rcUserInfo,RGB(255,255,255), nTextFormat);
      }
      else if(i == MY_VIEW_CHAIRID)
      {
        //�û����
        m_PngUserBackMe.DrawImage(pDC,m_ptAvatar[i].x,m_ptAvatar[i].y-94);

        //�滭ͷ��
        DrawUserAvatar(pDC, m_ptAvatar[i].x+8, m_ptAvatar[i].y+29, pIClientUserItem);

        //�û�����
        TCHAR szBuffer[64]=TEXT("");
        int nTextWidth = (i%2==0?80:64);
        _sntprintf(szBuffer,CountArray(szBuffer),TEXT("%s"),pIClientUserItem->GetNickName());
        rcUserInfo.SetRect(m_ptNickName[i].x,m_ptNickName[i].y-2,m_ptNickName[i].x+nTextWidth,m_ptNickName[i].y+18-2);
        CDFontEx::DrawText(this, pDC, 12, 400, szBuffer, rcUserInfo,RGB(255,255,255), nTextFormat);

        //�û����
        LONGLONG lLeaveScore=pIClientUserItem->GetUserScore()-(pIClientUserItem->GetUserStatus()!=US_PLAYING?0:m_lTableScore[i]);
        _sntprintf(szBuffer,CountArray(szBuffer),TEXT("%I64d"),lLeaveScore);
        rcUserInfo.SetRect(m_ptNickName[i].x+120+50,m_ptNickName[i].y-2, m_ptNickName[i].x+nTextWidth+120+50, m_ptNickName[i].y+56-2);
        CDFontEx::DrawText(this, pDC, 12, 400, szBuffer, rcUserInfo,RGB(255,255,255), nTextFormat);
      }
      else
      {
        //�û����
        m_PngUserBackL.DrawImage(pDC,m_ptAvatar[i].x+1,m_ptAvatar[i].y-102);

        //�滭ͷ��
        DrawUserAvatar(pDC,m_ptAvatar[i].x+8, m_ptAvatar[i].y+29-20, pIClientUserItem);

        //�û�����
        TCHAR szBuffer[64]=TEXT("");
        int nTextWidth = (i%2==0?80:64);
        _sntprintf(szBuffer,CountArray(szBuffer),TEXT("%s"),pIClientUserItem->GetNickName());
        rcUserInfo.SetRect(m_ptNickName[i].x,m_ptNickName[i].y-20,m_ptNickName[i].x+nTextWidth,m_ptNickName[i].y+18-20);
        CDFontEx::DrawText(this, pDC, 12, 400, szBuffer, rcUserInfo,RGB(255,255,255), nTextFormat);

        //�û����
        LONGLONG lLeaveScore=pIClientUserItem->GetUserScore()-(pIClientUserItem->GetUserStatus()!=US_PLAYING?0:m_lTableScore[i]);
        _sntprintf(szBuffer,CountArray(szBuffer),TEXT("%I64d"),lLeaveScore);
        rcUserInfo.SetRect(m_ptNickName[i].x+120,m_ptNickName[i].y-20, m_ptNickName[i].x+nTextWidth+120, m_ptNickName[i].y+56-20);
        CDFontEx::DrawText(this, pDC, 12, 400, szBuffer, rcUserInfo,RGB(255,255,255), nTextFormat);
      }

      //������Ϣ
      if(wUserTimer != 0)
      {
        if(pIClientUserItem->GetUserStatus()==US_PLAYING || m_cbDynamicJoin)
        {
          DrawUserTimerEx(pDC,nWidth/2,nHeight/2-60,wUserTimer,99L);
        }
        else
        {
          if(i == MY_VIEW_CHAIRID && pIClientUserItem->GetUserStatus() != US_PLAYING)
          {
            DrawUserTimerEx(pDC,m_ptAvatar[i].x+300-270,m_ptAvatar[i].y+50-80,wUserTimer,99L);
          }
          else
          {
            DrawUserTimerEx(pDC,m_ptClock[i].x,m_ptClock[i].y,wUserTimer,99L);
          }
        }
      }
      if(pIClientUserItem->GetUserStatus()==US_READY)
      {
        DrawUserReadyEx(pDC, m_ptReady[i].x, m_ptReady[i].y);
      }
    }
    else
    {
      m_CardControl[i].SetOX(false);
      m_CardControl[i].SetCardData(NULL,0);
      m_CardControlOx[i].SetCardData(NULL,0);
    }
  }

  //�ȴ�̯��
  if(m_bOpenCard)
  {
    m_PngWaitOpen.DrawImage(pDC,nWidth/2-m_PngWaitOpen.GetWidth()/2,nHeight/2-24);
  }

  //��������
  if(m_bUserAction)
  {
    m_PngAutoOpen.DrawImage(pDC,nWidth/2-m_PngAutoOpen.GetWidth()/2,nHeight/2+90);
  }

  //ׯ����Ϣ
  if(m_wBankerUser!=INVALID_CHAIR)
  {
    m_ImageBanker.DrawImage(pDC,m_ptBanker[m_wBankerUser].x, m_ptBanker[m_wBankerUser].y);
  }

  //�滭����
  for(BYTE i = 0; i < GAME_PLAYER; i++)
  {
    IClientUserItem * pIClientUserItem = GetClientUserItem(i);

    if(pIClientUserItem != NULL && (m_lTableScore[i] != 0L))
    {
      //����λ��
      INT nX = nWidth/2-95;
      INT nY = nHeight/2+62;

      //�滭����
      if(m_lTableScore[i] > 0)
      {
        m_PngScoreBack.DrawImage(pDC, nX+20, nY, m_PngScoreBack.GetWidth()/3, m_PngScoreBack.GetHeight(), 0, 0);
      }
      else
      {
        m_PngScoreBack.DrawImage(pDC, nX+20, nY, m_PngScoreBack.GetWidth()/3, m_PngScoreBack.GetHeight(), 2*m_PngScoreBack.GetWidth()/3, 0);
      }

      //�滭��Ϣ
      TCHAR tc[LEN_ACCOUNTS]=TEXT("");
      _sntprintf(tc,CountArray(tc),TEXT("������%I64d"), m_lTableScore[i]);
      CRect rcDraw(nX,nY+4,nX+200,nY+25);
      DrawTextString(pDC, tc, RGB(250,250,250), RGB(0,0,0), &rcDraw);
    }
  }

  //�ƹ⶯��
  if(m_bShowCartoon == true)
  {
    m_PngCartoonLight.DrawImage(pDC, nWidth/2-100, nHeight/2+50,
                                m_PngCartoonLight.GetWidth()/2, m_PngCartoonLight.GetHeight(), (m_wCartoonLightIndex%2)*m_PngCartoonLight.GetWidth()/2, 0);
  }

  //�ʽ𶯻�
  if(m_bShowBonusCartoon)
  {
    for(WORD i=0; i<GAME_PLAYER; i++)
    {
      if(m_bBonus[i])
      {
        m_PngBonus.DrawImage(pDC, m_ptBonus[i].x, m_ptBonus[i].y, BONUS_PERPOSX, BONUS_PERPOSY,  BONUS_PERPOSX*m_wBonusCartoonIndex, 0);

        //�ʽ�����
        DrawNumber(pDC, &m_PngBonusNum, TEXT("0123456789"), m_lBonus, m_ptBonus[i].x + 188, m_ptBonus[i].y + 120);
      }
    }
  }

  //��Ϸ�׷�
  if(m_bShowCellScore == true)
  {
    //����λ��
    INT nX = nWidth/2-95;
    INT nY = nHeight/2+62;

    //�滭����
    m_PngScoreBack.DrawImage(pDC, nX+20, nY, m_PngScoreBack.GetWidth()/3, m_PngScoreBack.GetHeight(), m_PngScoreBack.GetWidth()/3, 0);

    if(m_lCellScore == 0)
    {
      //�滭��Ϣ
      CRect rcDraw(nX,nY+4,nX+200,nY+25);
      DrawTextString(pDC, TEXT("�ȴ���...."), RGB(255, 255, 0), RGB(0, 0, 0), &rcDraw);
    }
    else
    {
      //�滭��Ϣ
      TCHAR tc[LEN_ACCOUNTS]=TEXT("");
      _sntprintf(tc,CountArray(tc),TEXT("�׷֣�%I64d"), m_lCellScore);
      CRect rcDraw(nX,nY+4,nX+200,nY+25);
      DrawTextString(pDC, tc, RGB(250,250,250), RGB(0,0,0), &rcDraw);
    }
  }

  //��ʾ̯�Ʊ�־
  for(BYTE i = 0; i < GAME_PLAYER; i++)
  {
    IClientUserItem * pIClientUserItem=GetClientUserItem(i);

    if(pIClientUserItem!=NULL)
    {
      if(m_cbOpenCardUser[i]==TRUE && i!=MY_VIEW_CHAIRID)
      {
        m_PngOpenTag.DrawImage(pDC,m_ptOpenTag[i].x,m_ptOpenTag[i].y);
      }
    }
  }

  //�滭�˿�
  if(m_SendCardItemArray.GetCount()>0)
  {
    //��������
    tagSendCardItem * pSendCardItem=&m_SendCardItemArray[0];

    //��ȡ��С
    int nItemWidth=m_ImageCard.GetWidth()/13;
    int nItemHeight=m_ImageCard.GetHeight()/5;

    //�滭�˿�
    int nXPos=m_ptSendCardPos.x-nItemWidth/2;
    int nYPos=m_ptSendCardPos.y-nItemHeight/2+10;
    m_ImageCard.TransDrawImage(pDC,nXPos,nYPos,nItemWidth,nItemHeight,nItemWidth*2,nItemHeight*4,RGB(255,0,255));
  }

  //��ʾ����
  for(WORD i = 0; i < GAME_PLAYER; i++)
  {
    //��ʾ̯�Ʊ�־
    for(BYTE i = 0; i < GAME_PLAYER; i++)
    {
      IClientUserItem * pIClientUserItem=GetClientUserItem(i);

      if(pIClientUserItem!=NULL)
      {
        if(m_cbOxValue[i] != 0xFF)
        {
          if(m_bShow || i == MY_VIEW_CHAIRID)
          {
            INT nXpt = m_ptValue[i].x;
            INT nypt = m_ptValue[i].y - 20;
            if(m_cbOxValue[i] != 0)
            {
              if(m_cbOxValue[i] == 10)
              {
                m_ImageDoulbeOx.DrawImage(pDC, nXpt, nypt);
              }
              else if(m_cbOxValue[i] < 10)
              {
                m_ImageOxValue.DrawImage(pDC, nXpt, nypt+15);
                m_ImageNumber.DrawImage(pDC, nXpt+45, nypt+5,
                                        m_ImageNumber.GetWidth()/9,m_ImageNumber.GetHeight(),
                                        m_ImageNumber.GetWidth()/9*((int)m_cbOxValue[i]-1),0);
              }
            }
            else if(m_cbOxValue[i]==0 && (m_CardControl[i].GetCardCount()!=0))
            {
              m_ImageOxValueZero.DrawImage(pDC,nXpt,nypt);
            }
          }
        }
      }
    }
  }

  //��ʷ����
  if(m_bShowScore==true)
  {
    //���ֱ���
    m_PngHistoryScore.DrawImage(pDC,nWidth-30-m_sizeHistory.cx-20,5);

    //����λ��
    INT nYBenchmark=5;
    INT nXBenchmark=nWidth-30-m_sizeHistory.cx;
    UINT nFormat=DT_SINGLELINE|DT_END_ELLIPSIS|DT_VCENTER|DT_CENTER;

    //�滭����
    pDC->SetTextAlign(TA_LEFT|TA_TOP);

    //�滭��Ϣ
    for(WORD i=0; i<GAME_PLAYER; i++)
    {
      //��ȡ�û�
      IClientUserItem * pIClientUserItem=GetClientUserItem(i);
      if((pIClientUserItem==NULL)||(m_pHistoryScore[i]==NULL))
      {
        continue;
      }

      //λ�ü���
      INT nY1=27,nY2=nY1+18,nSpa=20;
      CRect rcAccounts(nXBenchmark+4-20,nYBenchmark+nY1+i*nSpa,nXBenchmark+68+7-20,nYBenchmark+nY2+i*nSpa);
      CRect rcTurnScore(nXBenchmark+70+15-20,nYBenchmark+nY1+i*nSpa,nXBenchmark+121+15-20,nYBenchmark+nY2+i*nSpa);
      CRect rcCollectScore(nXBenchmark+140-20,nYBenchmark+nY1+i*nSpa,nXBenchmark+191-20,nYBenchmark+nY2+i*nSpa);

      //������Ϣ
      TCHAR szTurnScore[16]=TEXT(""),szCollectScore[16]=TEXT("");
      _sntprintf(szTurnScore,CountArray(szTurnScore),TEXT("%I64d"),m_pHistoryScore[i]->lTurnScore);
      _sntprintf(szCollectScore,CountArray(szCollectScore),TEXT("%I64d"),m_pHistoryScore[i]->lCollectScore);

      //�滭��Ϣ
      if(i == MY_VIEW_CHAIRID)
      {
        pDC->SetTextColor(RGB(255,255,0));
      }
      else
      {
        pDC->SetTextColor(RGB(255,255,255));
      }
      CDFontEx::DrawText(this,pDC, 12, 400, pIClientUserItem->GetNickName(), rcAccounts,RGB(255,255,255), nFormat);

      if(i == MY_VIEW_CHAIRID)
      {
        pDC->SetTextColor(RGB(255,153,0));
      }
      else
      {
        pDC->SetTextColor(RGB(0,255,153));
      }
      CDFontEx::DrawText(this,pDC,  12, 400, szTurnScore, rcTurnScore,RGB(255,255,255), nFormat);
      CDFontEx::DrawText(this,pDC,  12, 400, szCollectScore, rcCollectScore,RGB(255,255,255), nFormat);
    }
  }

  return;
}

//�滭ʱ��
void CGameClientView::DrawUserTimerEx(CDC * pDC, int nXPos, int nYPos, WORD wTime, WORD wTimerArea)
{
  //��ȡ����
  const INT nNumberHeight=m_ImageTimeNumber.GetHeight();
  const INT nNumberWidth=m_ImageTimeNumber.GetWidth()/10;

  //������Ŀ
  LONG lNumberCount=0;
  WORD wNumberTemp=wTime;
  do
  {
    lNumberCount++;
    wNumberTemp/=10;
  }
  while(wNumberTemp>0L);

  //λ�ö���
  INT nYDrawPos=nYPos-nNumberHeight/2+1;
  INT nXDrawPos=nXPos+(lNumberCount*nNumberWidth)/2-nNumberWidth;

  //�滭����
  CSize SizeTimeBack(m_ImageTimeBack.GetWidth(),m_ImageTimeBack.GetHeight());
  m_ImageTimeBack.DrawImage(pDC,nXPos-SizeTimeBack.cx/2,nYPos-SizeTimeBack.cy/2);

  //�滭����
  for(LONG i=0; i<lNumberCount; i++)
  {
    //�滭����
    WORD wCellNumber=wTime%10;
    m_ImageTimeNumber.DrawImage(pDC,nXDrawPos,nYDrawPos,nNumberWidth,nNumberHeight,wCellNumber*nNumberWidth,0);

    //���ñ���
    wTime/=10;
    nXDrawPos-=nNumberWidth;
  }

  return;
}

//��ʾ��Ϣ
void CGameClientView::SetUserAction(bool bWait)
{
  if(m_bUserAction!=bWait)
  {
    m_bUserAction=bWait;

    //���½���
    InvalidGameView(0,0,0,0);
  }

  //����ʱ��
  if(bWait)
  {
    SetTimer(IDI_USER_ACTION,TIME_USER_ACTION,NULL);
  }
  else
  {
    KillTimer(IDI_USER_ACTION);
  }

  return;
}

//�����ж�
bool CGameClientView::IsDispatchCard()
{
  return m_SendCardItemArray.GetCount()>0;
}

//��ɷ���
void CGameClientView::FinishDispatchCard()
{
  //����ж�
  if(m_SendCardItemArray.GetCount()==0)
  {
    return;
  }

  //��ɶ���
  for(INT_PTR i=0; i<m_SendCardItemArray.GetCount(); i++)
  {
    //��ȡ����
    tagSendCardItem * pSendCardItem=&m_SendCardItemArray[i];

    //��ȡ�˿�
    BYTE cbCardData[MAX_COUNT];
    WORD wChairID=pSendCardItem->wChairID;
    BYTE cbCardCount=(BYTE)m_CardControl[wChairID].GetCardData(cbCardData,CountArray(cbCardData));

    //�����˿�
    if(cbCardCount<MAX_COUNT)
    {
      cbCardData[cbCardCount++]=pSendCardItem->cbCardData;
      m_CardControl[wChairID].SetCardData(cbCardData,cbCardCount,!m_bHandOpen);
    }
  }

  SendEngineMessage(IDM_SEND_CARD_FINISH,0,0);

  //ɾ������
  KillTimer(IDI_SEND_CARD);
  m_SendCardItemArray.RemoveAll();

  //���½���
  InvalidGameView(0,0,0,0);

  return;
}

//�����˿�
void CGameClientView::DispatchUserCard(WORD wChairID, BYTE cbCardData)
{
  //Ч�����
  ASSERT(wChairID<GAME_PLAYER);
  if(wChairID>=GAME_PLAYER)
  {
    return;
  }

  //���ñ���
  tagSendCardItem SendCardItem;
  SendCardItem.cbCardData=cbCardData;
  SendCardItem.wChairID=wChairID;

  //��������
  m_SendCardItemArray.Add(SendCardItem);

  //��������
  if(m_SendCardItemArray.GetCount()==1)
  {
    //��ȡλ��
    CRect rcClient;
    GetClientRect(&rcClient);

    //����λ��
    m_ptSendCardPos.x=m_ptSendStartPos.x;
    m_ptSendCardPos.y=m_ptSendStartPos.y;

    //λ�Ƽ���
    int nXCount=abs(m_ptSendEndingPos[SendCardItem.wChairID].x-m_ptSendStartPos.x)/SEND_PELS;
    int nYCount=abs(m_ptSendEndingPos[SendCardItem.wChairID].y-m_ptSendStartPos.y)/SEND_PELS;

    //���ô���
    m_wSendIndex=0;
    m_wSendCount=__max(1,__max(nXCount,nYCount));

    //���ö�ʱ��
    SetTimer(IDI_SEND_CARD,TIME_SENDSPEED,NULL);

    //��������
    CGameClientEngine * pGameClientEngine=CONTAINING_RECORD(this,CGameClientEngine,m_GameClientView);
    pGameClientEngine->PlayGameSound(AfxGetInstanceHandle(),TEXT("SEND_CARD"));
  }

  return;
}

//������ע
void CGameClientView::SetUserTableScore(WORD wChairID, LONGLONG lTableScore,bool bMove)
{
  //��������
  if(wChairID<GAME_PLAYER)
  {
    if(bMove)
    {
      DrawMoveAnte(wChairID,lTableScore);
    }
    else
    {
      m_lTableScore[wChairID]=lTableScore;
    }
  }
  else
  {
    ZeroMemory(m_lTableScore,sizeof(m_lTableScore));
  }

  //���½���
  InvalidGameView(0,0,0,0);
  return;
}

//ׯ�ұ�־
void CGameClientView::SetBankerUser(WORD wBankerUser)
{
  m_wBankerUser=wBankerUser;

  //���½���
  InvalidGameView(0,0,0,0);
  return;
}

//�ȴ���־
void CGameClientView::SetWaitInvest(bool bInvest)
{
  if(m_bInvest!=bInvest)
  {
    m_bInvest=bInvest;

    //���½���
    InvalidGameView(0,0,0,0);
  }
  return;
}

//��ʾ����
void CGameClientView::DisplayTypeEx(bool bShow)
{
  if(m_bShow!=bShow)
  {
    m_bShow=bShow;

    //���½���
    InvalidGameView(0,0,0,0);
  }

  return;
}

//��ʾ����
void CGameClientView::SetUserOxValue(WORD wChiarID,BYTE cbValue)
{
  //ţţ����
  switch(cbValue)
  {
    case OX_VALUE_0:
    {
      m_cbOxValue[wChiarID] = 0;
    }
    break;
    case OX_VALUE_1:
    case OX_VALUE_BIG_1:
    {
      m_cbOxValue[wChiarID] = 1;
    }
    break;
    case OX_VALUE_2:
    case OX_VALUE_BIG_2:
    {
      m_cbOxValue[wChiarID] = 2;
    }
    break;
    case OX_VALUE_3:
    case OX_VALUE_BIG_3:
    {
      m_cbOxValue[wChiarID] = 3;
    }
    break;
    case OX_VALUE_4:
    case OX_VALUE_BIG_4:
    {
      m_cbOxValue[wChiarID] = 4;
    }
    break;
    case OX_VALUE_5:
    case OX_VALUE_BIG_5:
    {
      m_cbOxValue[wChiarID] = 5;
    }
    break;
    case OX_VALUE_6:
    case OX_VALUE_BIG_6:
    {
      m_cbOxValue[wChiarID] = 6;
    }
    break;
    case OX_VALUE_7:
    case OX_VALUE_BIG_7:
    {
      m_cbOxValue[wChiarID] = 7;
    }
    break;
    case OX_VALUE_8:
    case OX_VALUE_BIG_8:
    {
      m_cbOxValue[wChiarID] = 8;
    }
    break;
    case OX_VALUE_9:
    case OX_VALUE_BIG_9:
    {
      m_cbOxValue[wChiarID] = 9;
    }
    break;
    case OX_VALUE_10:
    case OX_VALUE_BIG_10:
    case OX_FOUR_KING:
    case OX_FIVE_KING:
    {
      m_cbOxValue[wChiarID] = 10;
    }
    break;
    case 0xFF:
    {
      m_cbOxValue[wChiarID] = 0xFF;
    }
  }

  //���½���
  InvalidGameView(0,0,0,0);

  return;
}

//��ʾ̯��
void CGameClientView::ShowOpenCard(WORD wChiarID,BOOL bShow)
{
  //̯���û�
  m_cbOpenCardUser[wChiarID]=bShow;

  //���½���
  InvalidGameView(0,0,0,0);

  return;
}

//��ʾ����
void CGameClientView::ShowScore(WORD wChairID,bool bShow)
{
  //��ʾ��־
  m_bShowSocre[wChairID]=bShow;

  //���½���
  InvalidGameView(0,0,0,0);

  return;
}

//��ʾ����
void CGameClientView::ShowCartonnLight(bool bShow)
{
  //���ñ���
  m_bShowCartoon = bShow;

  //���ö�ʱ��
  SetTimer(IDI_CARTOON_LIGHT, 200, NULL);

  //���½���
  InvalidGameView(0,0,0,0);

  return;
}

//��ʾ�׷�
void CGameClientView::ShowCellScore(LONGLONG lScore, bool bShow)
{
  //���ñ���
  m_lCellScore = lScore;
  m_bShowCellScore = bShow;

  //���½���
  InvalidGameView(0,0,0,0);
  return;
}

//��������
void CGameClientView::DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, int nXPos, int nYPos)
{
  //��������
  int nStringLength=lstrlen(pszString);
  int nXExcursion[8]= {1,1,1,0,-1,-1,-1,0};
  int nYExcursion[8]= {-1,0,1,1,1,0,-1,-1};

  //�滭�߿�
  pDC->SetTextColor(crFrame);
  for(int i=0; i<CountArray(nXExcursion); i++)
  {
    CDFontEx::DrawText(this,pDC,  12, 400, pszString, nXPos+nXExcursion[i],nYPos+nYExcursion[i],crFrame, DT_VCENTER|DT_CENTER|DT_SINGLELINE|DT_END_ELLIPSIS);
  }

  //�滭����
  pDC->SetTextColor(crText);
  CDFontEx::DrawText(this,pDC,  12, 400, pszString, nXPos,nYPos,crText, DT_VCENTER|DT_CENTER|DT_SINGLELINE|DT_END_ELLIPSIS);

  return;
}

//��������
void CGameClientView::DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, LPRECT lpRect)
{
  //��������
  int nStringLength=lstrlen(pszString);
  int nXExcursion[8]= {1,1,1,0,-1,-1,-1,0};
  int nYExcursion[8]= {-1,0,1,1,1,0,-1,-1};

  //�滭�߿�
  pDC->SetTextColor(crFrame);
  CRect rcDraw;
  for(int i=0; i<CountArray(nXExcursion); i++)
  {
    rcDraw.CopyRect(lpRect);
    rcDraw.OffsetRect(nXExcursion[i],nYExcursion[i]);
    CDFontEx::DrawText(this,pDC,  12, 400, pszString, rcDraw,crFrame, DT_VCENTER|DT_CENTER|DT_SINGLELINE|DT_END_ELLIPSIS);
  }

  //�滭����
  rcDraw.CopyRect(lpRect);
  pDC->SetTextColor(crText);
  CDFontEx::DrawText(this,pDC,  12, 400, pszString, rcDraw,crText, DT_VCENTER|DT_CENTER|DT_SINGLELINE|DT_END_ELLIPSIS);

  return;
}

//���ְ�ť
VOID CGameClientView::OnBnClickedScore()
{
  //���ñ���
  m_bShowScore=!m_bShowScore;

  //���ð�ť
  HINSTANCE hInstance=AfxGetInstanceHandle();
  m_btScore.SetButtonImage((m_bShowScore==true)?IDB_BT_CLOSE_SCORE:IDB_BT_SHOW_SCORE,hInstance,false,false);

  //���½���
  InvalidGameView(0,0,0,0);

  return;
}

//�Զ�����
VOID CGameClientView::OnBnClickedAutoOpen()
{
  m_bHandOpen=false;
  m_btAutoOpenCard.EnableWindow(FALSE);
  m_btHandOpenCard.EnableWindow(TRUE);

  return;
}

//�ֶ�����
VOID CGameClientView::OnBnClickedHandOpen()
{
  m_bHandOpen=true;
  m_btAutoOpenCard.EnableWindow(TRUE);
  m_btHandOpenCard.EnableWindow(FALSE);

  return;
}

//����Ա����
VOID CGameClientView::OnBnClickedAdminWnd()
{
  if(m_pClientControlDlg != NULL)
  {
    if(!m_pClientControlDlg->IsWindowVisible())
    {
      m_pClientControlDlg->ShowWindow(SW_SHOW);
    }
    else
    {
      m_pClientControlDlg->ShowWindow(SW_HIDE);
    }
  }

  return;
}

//����Ȩ��
VOID CGameClientView::SetAdminCheck(bool bCheck)
{
  //���ñ���
  if(m_bCheck != bCheck)
  {
    m_bCheck = bCheck;
  }

  return;
}

//���û���
bool CGameClientView::SetHistoryScore(WORD wChairID,tagHistoryScore * pHistoryScore)
{
  //Ч�����
  ASSERT(wChairID<GAME_PLAYER);
  if(wChairID>=GAME_PLAYER)
  {
    return false;
  }

  //���ñ���
  m_pHistoryScore[wChairID]=pHistoryScore;

  //���½���
  InvalidGameView(0,0,0,0);

  return true;
}

//���ƶ���, 0: ��ע����, 1: ��ע����, 2: ��ע�ϲ�����ע, 3: Ӯ���ճ���
void CGameClientView::DrawMoveAnte(WORD wViewChairID, LONGLONG lTableScore)
{
  //��������
  int nAnimeStep = 60;

  m_JettonStatus[wViewChairID].wMoveIndex = 0;
  m_lMoveGold[wViewChairID]=lTableScore;

  m_JettonStatus[wViewChairID].ptFrom = m_ptCard[wViewChairID];
  m_JettonStatus[wViewChairID].ptDest = m_ptJeton[wViewChairID];
  m_JettonStatus[wViewChairID].ptCourse= m_ptCard[wViewChairID];
  m_JettonStatus[wViewChairID].lGold =lTableScore;

  //λ�Ƽ���
  INT nXCount=abs(m_JettonStatus[wViewChairID].ptDest.x-m_JettonStatus[wViewChairID].ptFrom.x)/nAnimeStep+rand()%8;
  INT nYCount=abs(m_JettonStatus[wViewChairID].ptDest.y-m_JettonStatus[wViewChairID].ptFrom.y)/nAnimeStep+rand()%8;
  m_JettonStatus[wViewChairID].wMoveCount = __max(1,__max(nXCount,nYCount));;

  //����ʱ��
  if(!m_bJettonAction)
  {
    m_bJettonAction = true;
    SetTimer(IDI_JETTON_ACTION,50,NULL);  //--------��ǰ��50
  }

  return;
}

//�ƶ�����
bool CGameClientView::MoveJetton()
{
  bool bAllClean = true;

  //���ñ���
  for(BYTE i=0; i<GAME_PLAYER; i++)
  {
    //�ƶ�����
    if(m_JettonStatus[i].wMoveIndex<m_JettonStatus[i].wMoveCount)
    {
      bAllClean = false;
      m_JettonStatus[i].wMoveIndex++;
      WORD wMoveIndex = m_JettonStatus[i].wMoveIndex;
      WORD wMoveCount = m_JettonStatus[i].wMoveCount;
      m_JettonStatus[i].ptCourse.x = m_JettonStatus[i].ptFrom.x + (m_JettonStatus[i].ptDest.x-m_JettonStatus[i].ptFrom.x)*wMoveIndex/wMoveCount;
      m_JettonStatus[i].ptCourse.y = m_JettonStatus[i].ptFrom.y + (m_JettonStatus[i].ptDest.y-m_JettonStatus[i].ptFrom.y)*wMoveIndex/wMoveCount;
    }
    else if(m_JettonStatus[i].wMoveCount>0)
    {
      //���봦��
      m_lTableScore[i] = m_JettonStatus[i].lGold ;

      //������Ϣ
      m_lMoveGold[i]=0;
      ZeroMemory(&m_JettonStatus[i],sizeof(m_JettonStatus[i]));
    }
  }

  if(bAllClean)
  {
    m_bJettonAction = false;
  }

  //���½���
  InvalidGameView(0,0,0,0);

  return !bAllClean;
}

//�����û�ͼ��
void CGameClientView::DrawUserReadyEx(CDC *pDC,int nXPos,int nYPos)
{
  m_PngReady.DrawImage(pDC,nXPos,nYPos);
}

void CGameClientView::OnLButtonDown(UINT nFlags, CPoint point)
{
  if(m_bCheck)
  {
    if(m_rcCheck.PtInRect(point))
    {
      OnBnClickedAdminWnd();
    }
  }

  CGameFrameViewGDI::OnLButtonDown(nFlags, point);
}

//��ʱ����Ϣ
void CGameClientView::OnTimer(UINT nIDEvent)
{
  //���ʹ���
  switch(nIDEvent)
  {
    case IDI_JETTON_ACTION: //���붯��
    {
      bool bKillTime = true;

      //�����ƶ�
      if(m_bJettonAction && MoveJetton())
      {
        bKillTime = false;
      }

      //ɾ����ʱ��
      if(bKillTime)
      {
        m_bJettonAction = false;
        KillTimer(IDI_JETTON_ACTION);
      }

      return;
    }
    case IDI_UPDATA_GAME: //������Ϸ
    {
      m_cbIndex++;

      //���½���
      InvalidGameView(0,0,0,0);

      return ;
    }
    case IDI_SEND_CARD:   //�ƶ��˿�
    {
      //��������
      m_wSendIndex++;

      //����λ��
      tagSendCardItem * pSendCardItem=&m_SendCardItemArray[0];
      m_ptSendCardPos.x=m_ptSendStartPos.x+(m_ptSendEndingPos[pSendCardItem->wChairID].x-m_ptSendStartPos.x)*m_wSendIndex/m_wSendCount;
      m_ptSendCardPos.y=m_ptSendStartPos.y+(m_ptSendEndingPos[pSendCardItem->wChairID].y-m_ptSendStartPos.y)*m_wSendIndex/m_wSendCount;

      //ֹͣ�ж�
      if(m_wSendIndex>=m_wSendCount)
      {
        //��ȡ�˿�
        BYTE cbCardData[MAX_COUNT];
        WORD wChairID=pSendCardItem->wChairID;
        BYTE cbCardCount=(BYTE)m_CardControl[wChairID].GetCardData(cbCardData,CountArray(cbCardData));

        //�����˿�
        cbCardData[cbCardCount++]=pSendCardItem->cbCardData;
        if(cbCardCount == 6)
        {
          ASSERT(FALSE);
        }
        m_CardControl[wChairID].SetCardData(cbCardData,cbCardCount,!m_bHandOpen);

        //ɾ������
        m_SendCardItemArray.RemoveAt(0);

        //��������
        if(m_SendCardItemArray.GetCount()>0)
        {
          //��������
          tagSendCardItem * pSendCardItem=&m_SendCardItemArray[0];

          //����λ��
          m_ptSendCardPos.x=m_ptSendStartPos.x;
          m_ptSendCardPos.y=m_ptSendStartPos.y;

          //λ�Ƽ���
          int nXCount=abs(m_ptSendEndingPos[pSendCardItem->wChairID].x-m_ptSendStartPos.x)/SEND_PELS;
          int nYCount=abs(m_ptSendEndingPos[pSendCardItem->wChairID].y-m_ptSendStartPos.y)/SEND_PELS;

          //���ô���
          m_wSendIndex=0;
          m_wSendCount=__max(1,__max(nXCount,nYCount));

          //��������
          CGameClientEngine * pGameClientEngine=CONTAINING_RECORD(this,CGameClientEngine,m_GameClientView);
          pGameClientEngine->PlayGameSound(AfxGetInstanceHandle(),TEXT("SEND_CARD"));
        }
        else
        {
          //��ɴ���
          KillTimer(IDI_SEND_CARD);
          SendEngineMessage(IDM_SEND_CARD_FINISH,0,0);

          //���ð�ť
          BOOL bHandOpen = m_bHandOpen==true ? TRUE : FALSE;
          m_btAutoOpenCard.EnableWindow(bHandOpen);
          m_btHandOpenCard.EnableWindow(!bHandOpen);
        }
      }

      //���½���
      InvalidGameView(0,0,0,0);

      return ;
    }
    case IDI_USER_ACTION:   //�û�����
    {
      //��������
      m_bUserAction = false;
      KillTimer(IDI_USER_ACTION);

      //���½���
      InvalidGameView(0,0,0,0);

      return;
    }
    case IDI_CARTOON_LIGHT: //�ƹ⶯��
    {
      //��������
      m_wCartoonLightIndex++;

      if(m_wCartoonLightIndex == 50)
      {
        m_wCartoonLightIndex = 0;
      }

      //���½���
      InvalidGameView(0,0,0,0);

      return;
    }
    case IDI_BONUS:
    {
      m_wBonusCartoonIndex++;

      if(m_wBonusCartoonIndex >= BONUS_FPS)
      {
        KillTimer(IDI_BONUS);
        m_wBonusCartoonIndex = 0;
        ZeroMemory(m_bBonus, sizeof(m_bBonus));
        m_bShowBonusCartoon = false;
      }

      //���½���
      InvalidGameView(0,0,0,0);
      return;
    }
  }

  __super::OnTimer(nIDEvent);
}

//������ͼ
void CGameClientView::RefreshGameView()
{
  CRect rect;
  GetClientRect(&rect);
  InvalidGameView(rect.left,rect.top,rect.Width(),rect.Height());

  return;
}

//������ͼ
void CGameClientView::RefreshGameView(CRect &rect)
{
  InvalidGameView(rect.left,rect.top,rect.Width(),rect.Height());

  return;
}

//��ʼ�ʽ𶯻�
void CGameClientView::StartBonusCartoon()
{
  //���ö�ʱ��
  SetTimer(IDI_BONUS, 40, NULL);
  m_bShowBonusCartoon = true;

}

//���ö�����־
void CGameClientView::SetBonusFlag(WORD wViewChairID, bool bBonus)
{
  m_bBonus[wViewChairID] = bBonus;
}

// �滭����
void CGameClientView::DrawNumber(CDC * pDC, CPngImageEx* ImageNumber, TCHAR * szImageNum, LONGLONG lOutNum, INT nXPos, INT nYPos, UINT uFormat /*= DT_RIGHT*/)
{
  TCHAR szOutNum[128] = {0};
  _sntprintf(szOutNum,CountArray(szOutNum),TEXT("%I64d"),lOutNum);
  DrawNumber(pDC, ImageNumber, szImageNum, szOutNum, nXPos, nYPos, uFormat);
}

// �滭����
void CGameClientView::DrawNumber(CDC * pDC, CPngImageEx* ImageNumber, TCHAR * szImageNum, TCHAR* szOutNum ,INT nXPos, INT nYPos,  UINT uFormat /*= DT_RIGHT*/)
{
  // ������Դ
  INT nNumberHeight=ImageNumber->GetHeight();
  INT nNumberWidth=ImageNumber->GetWidth()/lstrlen(szImageNum);

  if(uFormat == DT_CENTER)
  {
    nXPos -= (INT)(((DOUBLE)(lstrlen(szOutNum)) / 2.0) * nNumberWidth);
  }
  else if(uFormat == DT_RIGHT)
  {
    nXPos -= lstrlen(szOutNum) * nNumberWidth;
  }

  for(INT i = 0; i < lstrlen(szOutNum); ++i)
  {
    for(INT j = 0; j < lstrlen(szImageNum); ++j)
    {
      if(szOutNum[i] == szImageNum[j] && szOutNum[i] != '\0')
      {
        ImageNumber->DrawImage(pDC, nXPos, nYPos, nNumberWidth, nNumberHeight, j * nNumberWidth, 0, nNumberWidth, nNumberHeight);
        nXPos += nNumberWidth;
        break;
      }
    }
  }
}
//////////////////////////////////////////////////////////////////////////
