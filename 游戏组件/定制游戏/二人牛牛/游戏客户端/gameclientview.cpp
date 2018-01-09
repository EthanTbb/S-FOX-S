#include "StdAfx.h"
#include "Resource.h"
#include "GameClient.h"
#include "GameClientView.h"
#include "GameClientEngine.h"


//////////////////////////////////////////////////////////////////////////
//�궨��

//��ť��ʶ
#define IDC_HINT_OX           130               //��ʾ��ť
#define IDC_START           100               //��ʼ��ť
#define IDC_MAX_SCORE         104               //���ť
#define IDC_MIN_SCORE         105               //���ٰ�ť
#define IDC_ADD_SCORE         110               //��ע��ť

#define IDC_READY           111               //̯�ư�ť
#define IDC_RESORT            112               //���Ű�ť
#define IDC_OX              113               //ţţ��ť
#define IDC_ONE_SCORE         117               //��ע��ť
#define IDC_TWO_SCORE         114               //��ע��ť
#define IDC_THREE_SCORE         115               //��ע��ť
#define IDC_FOUR_SCORE          116               //��ע��ť
#define IDC_SHORTCUT          118               //��ݼ���ť
#define IDC_IDLER           119               //�мҰ�ť
#define IDC_BANKER            120               //ׯ�Ұ�ť
#define IDC_SCORE           122               //��ְ�ť
#define IDC_YU_YIN            123               //������ť
#define IDC_ADMIN           124                 //��ť��ʶ

//��ʱ����ʶ
#define IDI_SEND_CARD         98                //���ƶ�ʱ��
#define IDI_GOLD            97                //���ƶ�ʱ��
#define IDI_USER_ACTION         101               //������ʱ��
#define IDI_UPDATA_GAME         102               //���¶�ʱ��
#define IDI_JETTON_ACTION       103               //������ʱ��
#define IDI_BONUS           104               //�ʽ�ʱ��

#define BONUS_FPS           26
#define BONUS_PERPOSX         320
#define BONUS_PERPOSY         240

//��ʱ��ʱ��
#define TIME_USER_ACTION        4000              //������ʱ��

//�ƶ��ٶ�
#define SEND_PELS           80                //�����ٶ�
#define TIME_SENDSPEED          50                //����ٶ�


//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameClientView, CGameFrameViewGDI)
  ON_WM_CREATE()
  ON_BN_CLICKED(IDC_SCORE, OnBnClickedScore)
  ON_BN_CLICKED(IDC_ADMIN,OpenAdminWnd)
  ON_WM_TIMER()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameClientView::CGameClientView()
{
  //��������
  m_cbIndex=0;
  m_bJettonAction=false;
  m_cbDynamicJoin=FALSE;

  m_wBonusCartoonIndex = 0;
  m_bShowBonusCartoon = false;
  ZeroMemory(m_bBonus, sizeof(m_bBonus));
  m_lBonus = 0;

  for(WORD i=0; i<GAME_PLAYER; i++)
  {
    m_bOxValue[i]=0xff;
    m_SendEndingPos[i].SetPoint(0,0);
  }
  m_wSendCount=0;
  m_wSendIndex=0;
  m_SendCardPos.SetPoint(0,0);
  m_SendCardItemArray.RemoveAll();
  ZeroMemory(m_JettonStatus,sizeof(m_JettonStatus));
  ZeroMemory(m_lMoveGold,sizeof(m_lMoveGold));

  //���ݱ���
  ZeroMemory(m_lTableScore,sizeof(m_lTableScore));
  ZeroMemory(m_bOpenCardUser,sizeof(m_bOpenCardUser));
  ZeroMemory(m_bShowSocre,sizeof(m_bShowSocre));
  m_wBankerUser=INVALID_CHAIR;
  m_bCallUser=0xff;
  m_bShow=false;
  m_bInvest=false;
  m_bOpenCard=false;
  m_bUserAction=false;
  m_bLookOnUser=false;
  m_bShowScore=false;
  ZeroMemory(m_pHistoryScore,sizeof(m_pHistoryScore));

  //����λͼ
  HINSTANCE hInstance=AfxGetInstanceHandle();
  m_ImageScore.LoadFromResource(hInstance, IDB_SCORE);
  m_ImageJeton.LoadFromResource(hInstance, IDB__MONEY);
  m_ImageCard.LoadFromResource(hInstance, IDB_CARD);
  m_ImageViewBack.LoadFromResource(hInstance, IDB_VIEW_BACK);
  m_ImageViewCenter.LoadFromResource(hInstance, IDB_VIEW_FILL);

  //������Դ
  m_PngReady.LoadImage(hInstance,TEXT("READY"));
  m_PngBonus.LoadImage(hInstance,TEXT("BONUS"));
  m_PngBonusNum.LoadImage(this, hInstance, TEXT("BONUSNUM"));
  m_PngUserBack.LoadImage(hInstance,TEXT("USER_HEAD_BACK"));
  m_ImageOxValueZero.LoadImage(hInstance,TEXT("COW_VALUE_ZERO"));
  m_ImageDoulbeOx.LoadImage(hInstance,TEXT("DOUBLE_COW"));
  m_ImageNumber.LoadImage(hInstance,TEXT("NUMBER"));
  m_ImageOxValue.LoadImage(hInstance,TEXT("COW_VALUE"));
  m_ImageWaitCall.LoadImage(hInstance,TEXT("CALLBANKER"));
  m_ImageTimeBack.LoadImage(hInstance,TEXT("TIME_BACK"));
  m_ImageTimeNumber.LoadImage(hInstance,TEXT("TIME_NUMBER"));
  m_ImageBanker.LoadImage(hInstance,TEXT("BANKER_FLAG"));
  m_PngAutoOpen.LoadImage(hInstance,TEXT("AUTO_OPEN"));
  m_PngWaitBet.LoadImage(hInstance,TEXT("WAIT_BET"));
  m_PngWaitOpen.LoadImage(hInstance,TEXT("WAIT_OPEN"));
  m_PngOpenTag.LoadImage(hInstance,TEXT("OPEN_TAG"));
  m_PngHistoryScore.LoadImage(hInstance,TEXT("HISTORY_SCORE"));


  //��ȡ��С
  m_sizeHistory.SetSize(m_PngHistoryScore.GetWidth(),m_PngHistoryScore.GetHeight());

  m_pClientControlDlg = NULL;
  m_pSpeClientControlDlg = NULL;
  m_hInst = NULL;
  m_hSpeInst = NULL;
  return;
}

//��������
CGameClientView::~CGameClientView()
{
  if(m_pClientControlDlg)
  {
    delete m_pClientControlDlg;
    m_pClientControlDlg = NULL;
  }

  if(m_hInst)
  {
    FreeLibrary(m_hInst);
    m_hInst = NULL;
  }

  if(m_pSpeClientControlDlg)
  {
    delete m_pSpeClientControlDlg;
    m_pSpeClientControlDlg = NULL;
  }

  if(m_hSpeInst)
  {
    FreeLibrary(m_hSpeInst);
    m_hSpeInst = NULL;
  }
}

//��Ϣ����
BOOL CGameClientView::PreTranslateMessage(MSG * pMsg)
{
  m_ToolTipCtrl.RelayEvent(pMsg);
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
  //m_GoldControl.Create(NULL,NULL,WS_CHILD|WS_CLIPSIBLINGS,CRect(50,50,66,66),this,8);
//  m_ScoreView.Create(NULL,NULL,WS_CHILD|WS_CLIPSIBLINGS|WS_CLIPCHILDREN,rcCreate,this,10);

  //������ť
  m_btYuYin.Create(NULL,WS_CHILD,rcCreate,this,IDC_YU_YIN);
  m_btScore.Create(NULL,WS_CHILD,rcCreate,this,IDC_SCORE);
  m_btHintOx.Create(TEXT(""),WS_CHILD,rcCreate,this,IDC_HINT_OX);
  m_btStart.Create(TEXT(""),WS_CHILD,rcCreate,this,IDC_START);
//  m_btReSort.Create(TEXT(""),WS_CHILD,rcCreate,this,IDC_RESORT);
  m_btOpenCard.Create(TEXT(""),WS_CHILD,rcCreate,this,IDC_READY);
  m_btOx.Create(TEXT(""),WS_CHILD,rcCreate,this,IDC_OX);
//  m_btShortcut.Create(TEXT(""),WS_CHILD,rcCreate,this,IDC_SHORTCUT);
  m_btOneScore.Create(TEXT(""),WS_CHILD,rcCreate,this,IDC_ONE_SCORE);
  m_btTwoScore.Create(TEXT(""),WS_CHILD,rcCreate,this,IDC_TWO_SCORE);
  m_btThreeScore.Create(TEXT(""),WS_CHILD,rcCreate,this,IDC_THREE_SCORE);
  m_btFourScore.Create(TEXT(""),WS_CHILD,rcCreate,this,IDC_FOUR_SCORE);
  m_btBanker.Create(TEXT(""),WS_CHILD,rcCreate,this,IDC_BANKER);
  m_btIdler.Create(TEXT(""),WS_CHILD,rcCreate,this,IDC_IDLER);

  //������ɫ
  m_btOneScore.SetButtonColor(RGB(0,0,0));
  m_btTwoScore.SetButtonColor(RGB(0,0,0));
  m_btThreeScore.SetButtonColor(RGB(0,0,0));
  m_btFourScore.SetButtonColor(RGB(0,0,0));

  //����λͼ
  HINSTANCE hInstance=AfxGetInstanceHandle();
  m_btYuYin.SetButtonImage(IDB_BT_YU_YIN,hInstance,false,false);
  m_btScore.SetButtonImage(IDB_BT_SHOW_SCORE,hInstance,false,false);
  m_btHintOx.SetButtonImage(IDB_HINT_OX,hInstance,false,false);
  m_btStart.SetButtonImage(IDB_START,hInstance,false,false);
//  m_btReSort.SetButtonImage(IDB_RESOCT,hInstance,false,true);
  m_btOpenCard.SetButtonImage(IDB_OPENCARD,hInstance,false,false);
  m_btOx.SetButtonImage(IDB_COW,hInstance,false,false);
//  m_btShortcut.SetButtonImage(IDB_ShortCut,hInstance,false,true);
  m_btOneScore.SetButtonImage(IDB__MONEY,hInstance,false,false);
  m_btTwoScore.SetButtonImage(IDB__MONEY,hInstance,false,false);
  m_btThreeScore.SetButtonImage(IDB__MONEY,hInstance,false,false);
  m_btFourScore.SetButtonImage(IDB__MONEY,hInstance,false,false);
  m_btBanker.SetButtonImage(IDB_BANKER,hInstance,false,false);
  m_btIdler.SetButtonImage(IDB_IDLER,hInstance,false,false);

  //�����ؼ�
  for(WORD i=0; i<GAME_PLAYER; i++)
  {
    m_CardControl[i].Create(NULL,NULL,WS_VISIBLE|WS_CHILD,rcCreate,this,20+i);
    m_CardControlOx[i].Create(NULL,NULL,WS_VISIBLE|WS_CHILD,rcCreate,this,20+GAME_PLAYER+i);
    m_CardControlOx[i].SetDrawHeight(DRAW_HEIGHT);
  }

  //��ť��ʾ
  m_ToolTipCtrl.Create(this);
  m_ToolTipCtrl.Activate(TRUE);
//  m_ToolTipCtrl.AddTool(&m_btShortcut,TEXT("ţ��ʾ��ݼ�"));

#ifdef VIDEO_GAME
  //������Ƶ
  for(WORD i=0; i<GAME_PLAYER; i++)
  {
    //������Ƶ
    m_DlgVideoService[i].Create(NULL,NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,200+i);
    m_DlgVideoService[i].InitVideoService(i==1,i!=1);

    //������Ƶ
    g_VideoServiceManager.SetVideoServiceControl(i,&m_DlgVideoService[i]);
  }
#endif

  //���ÿؼ�
  for(int i=0; i<GAME_PLAYER; i++)
  {
    m_CardControlOx[i].SetDisplayFlag(true);
    m_CardControlOx[i].SetCardSpace(18);
    m_CardControl[i].SetCardSpace(18);
  }

  //m_btShortcut.ShowWindow(SW_SHOW);
  //BYTE bTemp[]={1,2,3,4,5};
  //for (BYTE i=0;i<GAME_PLAYER;i++)
  //{
  //  m_CardControl[i].SetDisplayFlag(true);
  //  m_CardControl[i].SetCardData(bTemp,5);
  //  m_CardControlOx[i].SetDisplayFlag(true);
  //  if(i!=0)m_CardControlOx[i].SetCardData(bTemp,2);
  //}

  //m_btHintOx.ShowWindow(SW_SHOW);
  //m_btStart.ShowWindow(SW_SHOW);
  //m_btReSort.ShowWindow(SW_SHOW);
  //m_btOpenCard.ShowWindow(SW_SHOW);
  //m_btOx.ShowWindow(SW_SHOW);
  //m_btOneScore.ShowWindow(SW_SHOW);
  //m_btTwoScore.ShowWindow(SW_SHOW);
  //m_btThreeScore.ShowWindow(SW_SHOW);
  //m_btFourScore.ShowWindow(SW_SHOW);
  //m_btShortcut.ShowWindow(SW_SHOW);
  //m_btBanker.ShowWindow(SW_SHOW);
  //m_btIdler.ShowWindow(SW_SHOW);

  //����
  m_hInst = NULL;
  m_pClientControlDlg = NULL;
  m_hInst = LoadLibrary(TEXT("OxExClientControl.dll"));
  if(m_hInst)
  {
    typedef void * (*CREATE)(CWnd* pParentWnd);
    CREATE ClientControl = (CREATE)GetProcAddress(m_hInst,"CreateClientControl");
    if(ClientControl)
    {
      m_pClientControlDlg = static_cast<IClientControlDlg*>(ClientControl(this));
      m_pClientControlDlg->ShowWindow(SW_HIDE);
    }
  }


  //�������

  m_hSpeInst = NULL;
  m_pSpeClientControlDlg = NULL;
  m_hSpeInst = LoadLibrary(TEXT("OxExSpeClientControl.dll"));
  if(m_hSpeInst)
  {
    typedef void * (*CREATE)(CWnd* pParentWnd);
    CREATE ClientControl = (CREATE)GetProcAddress(m_hSpeInst,"CreateClientControl");
    if(ClientControl)
    {
      m_pSpeClientControlDlg = static_cast<ISpeClientControlDlg*>(ClientControl(this));
      m_pSpeClientControlDlg->ShowWindow(SW_HIDE);
    }
  }

  //���ư�ť
  m_btOpenAdmin.Create(NULL,WS_CHILD|WS_VISIBLE,CRect(4,4,11,11),this,IDC_ADMIN);
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
    case IDC_ONE_SCORE:     //��ע��ť
    {
      //������Ϣ
      SendEngineMessage(IDM_ADD_SCORE,4,1);
      return TRUE;
    }
    case IDC_TWO_SCORE:     //��ע��ť
    {
      //������Ϣ
      SendEngineMessage(IDM_ADD_SCORE,3,2);
      return TRUE;
    }
    case IDC_THREE_SCORE:   //��ע��ť
    {
      //������Ϣ
      SendEngineMessage(IDM_ADD_SCORE,2,3);
      return TRUE;
    }
    case IDC_FOUR_SCORE:    //��ע��ť
    {
      //������Ϣ
      SendEngineMessage(IDM_ADD_SCORE,1,4);
      return TRUE;
    }
    case IDC_SHORTCUT:      //��ݼ���ť
    {
      //������Ϣ
      SendEngineMessage(IDM_SHORTCUT_KEY,0,0);
      return TRUE;
    }
    case IDC_BANKER:      //ׯ�Ұ�ť
    {
      //������Ϣ
      SendEngineMessage(IDM_BANKER,1,1);
      return TRUE;
    }
    case IDC_IDLER:       //�мҰ�ť
    {
      //������Ϣ
      SendEngineMessage(IDM_BANKER,0,0);
      return TRUE;
    }
    case IDC_YU_YIN:
    {
      //������Ϣ
      SendEngineMessage(IDM_YU_YIN,0,0);
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
  m_bJettonAction=false;
  for(WORD i=0; i<GAME_PLAYER; i++)
  {
    m_bOxValue[i]=0xff;
  }
  m_wSendCount=0;
  m_wSendIndex=0;
  m_cbDynamicJoin=FALSE;
  m_SendCardPos.SetPoint(0,0);
  m_SendCardItemArray.RemoveAll();
  ZeroMemory(m_JettonStatus,sizeof(m_JettonStatus));
  ZeroMemory(m_lMoveGold,sizeof(m_lMoveGold));

  //���ݱ���
  ZeroMemory(m_lTableScore,sizeof(m_lTableScore));
  ZeroMemory(m_bOpenCardUser,sizeof(m_bOpenCardUser));
  ZeroMemory(m_bShowSocre,sizeof(m_bShowSocre));
  m_wBankerUser=INVALID_CHAIR;
  m_bCallUser=0xff;
  m_bShow=false;
  m_bInvest=false;
  m_bOpenCard=false;
  m_bLookOnUser=false;
  m_bUserAction=false;
  m_bShowScore=false;
  ZeroMemory(m_pHistoryScore,sizeof(m_pHistoryScore));

  m_wBonusCartoonIndex = 0;
  m_bShowBonusCartoon = false;
  ZeroMemory(m_bBonus, sizeof(m_bBonus));

  //ɾ����ʱ��
  KillTimer(IDI_SEND_CARD);

  //���ذ�ť
  m_btScore.ShowWindow(SW_HIDE);
  m_btHintOx.ShowWindow(SW_HIDE);
  m_btStart.ShowWindow(SW_HIDE);
//  m_btReSort.ShowWindow(SW_HIDE);
  m_btOpenCard.ShowWindow(SW_HIDE);
  m_btOx.ShowWindow(SW_HIDE);
  m_btOneScore.ShowWindow(SW_HIDE);
  m_btTwoScore.ShowWindow(SW_HIDE);
  m_btThreeScore.ShowWindow(SW_HIDE);
  m_btFourScore.ShowWindow(SW_HIDE);
//  m_btShortcut.ShowWindow(SW_HIDE);
  m_btBanker.ShowWindow(SW_HIDE);
  m_btIdler.ShowWindow(SW_HIDE);
  m_btYuYin.EnableWindow(FALSE);

  //��ԭ��ť
  HINSTANCE hInstance=AfxGetInstanceHandle();
  m_btScore.SetButtonImage(IDB_BT_SHOW_SCORE,hInstance,false,false);

  //���ؿؼ�
//  m_ScoreView.ShowWindow(SW_HIDE);
  for(WORD i=0; i<GAME_PLAYER; i++)
  {
    m_CardControl[i].SetDisplayFlag(false);
    //m_CardControlOx[i].SetDisplayFlag(false);
    m_CardControl[i].SetCardData(NULL,0);
    m_CardControlOx[i].SetCardData(NULL,0);
  }

  m_pClientControlDlg = NULL;
  m_hInst = NULL;

  return;
}

//�����ؼ�
VOID CGameClientView::RectifyControl(int nWidth, int nHeight)
{
  SetTimer(IDI_UPDATA_GAME,1000,NULL);

  //����λ��
  m_SendStartPos.SetPoint(nWidth/2,nHeight/2-30);

  m_ptBonus[0].x = nWidth/2-FACE_CX/2 + 106;
  m_ptBonus[0].y = 70;
  m_ptAvatar[0].x=nWidth/2-FACE_CX/2;
  m_ptAvatar[0].y=9;
  m_ptNickName[0].x=nWidth/2+32;
  m_ptNickName[0].y=9;
  m_ptJeton[0].x=nWidth/2-36;
  m_ptJeton[0].y=206;
  m_ptCard[0].x=m_ptJeton[0].x;
  m_ptCard[0].y=m_ptJeton[0].y-100;
  m_ptReady[0].x=m_ptAvatar[0].x-10-69-4;
  m_ptReady[0].y=m_ptAvatar[0].y+(FACE_CY-48)/2+8;
  m_ptClock[0].x=m_ptAvatar[0].x+(FACE_CX-65)/2+32;
  m_ptClock[0].y=m_ptAvatar[0].y+FACE_CY+80;
  m_PointBanker[0].x = m_ptAvatar[0].x - 55;
  m_PointBanker[0].y =m_ptAvatar[0].y + 8;
  m_ptValue[0].x=nWidth/2+85;
  m_ptValue[0].y=129;

  m_ptBonus[1].x = nWidth/2-FACE_CX/2 + 106;
  m_ptBonus[1].y = nHeight-62 - 170;
  m_ptAvatar[1].x=nWidth/2-FACE_CX/2;
  m_ptAvatar[1].y=nHeight-62;
  m_ptNickName[1].x=nWidth/2+32;
  m_ptNickName[1].y=nHeight-62;
  m_ptJeton[1].x=nWidth/2-36;
  m_ptJeton[1].y=nHeight-285;
  m_ptCard[1].x=m_ptJeton[1].x;
  m_ptCard[1].y=m_ptJeton[1].y+90;
  m_ptReady[1].x=m_ptAvatar[1].x-69-10-4;
  m_ptReady[1].y=m_ptAvatar[1].y+(FACE_CY-48)/2+8;
  m_ptClock[1].x=m_ptAvatar[1].x+(FACE_CX-65)/2+32;
  m_ptClock[1].y=m_ptAvatar[1].y+-57-140;
  m_PointBanker[1].x = m_ptAvatar[1].x - 54;
  m_PointBanker[1].y =m_ptAvatar[1].y +FACE_CY-40;
  m_ptValue[1].x=nWidth/2+85;
  m_ptValue[1].y=nHeight-161;

  /*m_ptAvatar[1].x=nWidth-FACE_CX-16;
  m_ptAvatar[1].y=nHeight/2-FACE_CY/2+5;
  m_ptNickName[1].x=m_ptAvatar[1].x;
  m_ptNickName[1].y=m_ptAvatar[1].y+FACE_CY+6;
  m_ptJeton[1].x=nWidth-328+25;
  m_ptJeton[1].y=nHeight/2-49;
  m_ptCard[1].x=m_ptJeton[1].x+100;
  m_ptCard[1].y=m_ptJeton[1].y;
  m_ptReady[1].x=m_ptAvatar[1].x-10-69-4;
  m_ptReady[1].y=m_ptAvatar[1].y+(FACE_CY-48)/2+8;
  m_ptClock[1].x=m_ptAvatar[1].x-80;
  m_ptClock[1].y=m_ptAvatar[1].y+(FACE_CY-57)/2+29;
  m_PointBanker[1].x = m_ptAvatar[1].x+5;
  m_PointBanker[1].y = m_ptAvatar[1].y-40;
  m_ptValue[1].x=nWidth-180;
  m_ptValue[1].y=nHeight/2+40;

  m_ptAvatar[3].x=16;
  m_ptAvatar[3].y=nHeight/2-FACE_CY/2+5;
  m_ptNickName[3].x=m_ptAvatar[3].x;
  m_ptNickName[3].y=m_ptAvatar[3].y+FACE_CY+6;
  m_ptJeton[3].x=263-30;
  m_ptJeton[3].y=nHeight/2-49;
  m_ptCard[3].x=m_ptJeton[3].x-100;
  m_ptCard[3].y=m_ptJeton[3].y;
  m_ptReady[3].x=m_ptAvatar[3].x+FACE_CX+10+4;
  m_ptReady[3].y=m_ptAvatar[3].y+(FACE_CY-48)/2+8;
  m_ptClock[3].x=m_ptAvatar[3].x+FACE_CX+80;
  m_ptClock[3].y=m_ptAvatar[3].y+(FACE_CY-57)/2+29;
  m_PointBanker[3].x =m_ptAvatar[3].x+5;
  m_PointBanker[3].y =m_ptAvatar[3].y-40;
  m_ptValue[3].x=99;
  m_ptValue[3].y=nHeight/2+40;*/

  m_CardControl[0].SetBenchmarkPos(nWidth/2,80,enXCenter,enYTop);
  m_CardControlOx[0].SetBenchmarkPos(nWidth/2,80-DRAW_HEIGHT,enXCenter,enYTop);
  m_CardControl[1].SetBenchmarkPos(nWidth/2,nHeight-68,enXCenter,enYBottom);
  m_CardControlOx[1].SetBenchmarkPos(nWidth/2,nHeight-68-DRAW_HEIGHT,enXCenter,enYBottom);
  /*m_CardControl[1].SetBenchmarkPos(nWidth-70,nHeight/2-30,enXRight,enYCenter);
  m_CardControlOx[1].SetBenchmarkPos(nWidth-80,nHeight/2-30-DRAW_HEIGHT,enXRight,enYCenter);
  m_CardControl[3].SetBenchmarkPos(74,nHeight/2-30,enXLeft,enYCenter);
  m_CardControlOx[3].SetBenchmarkPos(84,nHeight/2-30-DRAW_HEIGHT,enXLeft,enYCenter);*/


  //����λ��
  m_SendEndingPos[0].x=m_CardControl[0].GetCardPos().x;
  m_SendEndingPos[0].y=m_CardControl[0].GetCardPos().y;
  /*m_SendEndingPos[1].x=m_CardControl[1].GetCardPos().x;
  m_SendEndingPos[1].y=m_CardControl[1].GetCardPos().y;*/
  m_SendEndingPos[1].x=m_CardControl[1].GetCardPos().x;
  m_SendEndingPos[1].y=m_CardControl[1].GetCardPos().y;
  //m_SendEndingPos[3].x=m_CardControl[3].GetCardPos().x;
  //m_SendEndingPos[3].y=m_CardControl[3].GetCardPos().y;


  //ţֵλ��
  for(WORD i=0; i<GAME_PLAYER; i++)
  {
    m_ptOpenTag[i]=m_ptValue[i];
  }

  //��ť�ؼ�
  CRect rcButton;
  HDWP hDwp=BeginDeferWindowPos(32);
  const UINT uFlags=SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOCOPYBITS|SWP_NOSIZE;

  //��ʼ��ť
  m_btStart.GetWindowRect(&rcButton);
  DeferWindowPos(hDwp,m_btStart,NULL,nWidth/2-44,nHeight-170-76,0,0,uFlags);

  //���ư�ť
  m_btOpenCard.GetWindowRect(&rcButton);
  //DeferWindowPos(hDwp,m_btReSort,NULL,nWidth/2-32,nHeight-237,0,0,uFlags);
  DeferWindowPos(hDwp,m_btOpenCard,NULL,nWidth/2+15,nHeight-237,0,0,uFlags);
  DeferWindowPos(hDwp,m_btOx,NULL,nWidth/2-118,nHeight-237,0,0,uFlags);
  DeferWindowPos(hDwp,m_btHintOx,NULL,nWidth/2-102,nHeight-237,0,0,uFlags);

  //������ť
//  DeferWindowPos(hDwp,m_btShortcut,NULL,nWidth/2+330,nHeight/2+320,0,0,uFlags);

  //���밴ť
  DeferWindowPos(hDwp,m_btOneScore,NULL,nWidth/2-151,nHeight-186,0,0,uFlags);
  DeferWindowPos(hDwp,m_btTwoScore,NULL,nWidth/2-67,nHeight-186,0,0,uFlags);
  DeferWindowPos(hDwp,m_btThreeScore,NULL,nWidth/2+15,nHeight-186,0,0,uFlags);
  DeferWindowPos(hDwp,m_btFourScore,NULL,nWidth/2+98,nHeight-186,0,0,uFlags);

  //ѡׯ��ť
  DeferWindowPos(hDwp,m_btBanker,NULL,nWidth/2-99,nHeight-198+10,0,0,uFlags);
  DeferWindowPos(hDwp,m_btIdler,NULL,nWidth/2+10,nHeight-198+10,0,0,uFlags);

  m_btYuYin.GetWindowRect(&rcButton);
  DeferWindowPos(hDwp,m_btYuYin,NULL,nWidth/2,nHeight-rcButton.Height()-5,0,0,uFlags);
#ifdef VIDEO_GAME

  //��Ƶ����
  DeferWindowPos(hDwp,m_DlgVideoService[0],NULL,nWidth/2-162,nHeight/2-350,0,0,uFlags|SWP_NOSIZE);
  //DeferWindowPos(hDwp,m_DlgVideoService[1],NULL,nWidth/2+287,nHeight/2-172,0,0,uFlags|SWP_NOSIZE);
  DeferWindowPos(hDwp,m_DlgVideoService[1],NULL,nWidth/2-290,nHeight/2+195,0,0,uFlags|SWP_NOSIZE);
  //DeferWindowPos(hDwp,m_DlgVideoService[3],NULL,nWidth/2-375,nHeight/2-172,0,0,uFlags|SWP_NOSIZE);
#endif

  //��ְ�ť
  CRect rcScore;
  m_btScore.GetWindowRect(&rcScore);
  DeferWindowPos(hDwp,m_btScore,NULL,nWidth-35,5,0,0,uFlags);

  //�����ƶ�
  EndDeferWindowPos(hDwp);

  //������ͼ
  //CRect rcControl;
  //m_ScoreView.GetWindowRect(&rcControl);
  //m_ScoreView.SetWindowPos(NULL,nWidth-66-m_sizeHistory.cx,5,0,0,SWP_NOZORDER|SWP_NOSIZE);

  return;
}

//�滭����
VOID CGameClientView::DrawGameView(CDC * pDC, int nWidth, int nHeight)
{
  //�滭����
  DrawViewImage(pDC,m_ImageViewCenter,DRAW_MODE_SPREAD);
  DrawViewImage(pDC,m_ImageViewBack,DRAW_MODE_CENTENT);

  //�滭�û�
  pDC->SetTextAlign(TA_LEFT);
  pDC->SetTextColor(RGB(255,255,255));
  CRect rcUserInfo(0,0,0,0);
  UINT nTextFormat = DT_LEFT|DT_TOP|DT_END_ELLIPSIS|DT_NOCLIP;
  for(BYTE i=0; i<GAME_PLAYER; i++)
  {
    //��������
    WORD wUserTimer=GetUserClock(i);
    IClientUserItem * pIClientUserItem=GetClientUserItem(i);

    //�滭�û�
    if(pIClientUserItem!=NULL)
    {
      TCHAR szBuffer[64]=TEXT("");

      //�û�����
      int nTextWidth = (i==1?100:64);
      _sntprintf(szBuffer,sizeof(szBuffer),TEXT("%s"),pIClientUserItem->GetNickName());
      rcUserInfo.SetRect(m_ptNickName[i].x,m_ptNickName[i].y,m_ptNickName[i].x+nTextWidth,m_ptNickName[i].y+18);
      pDC->DrawText(szBuffer,lstrlen(szBuffer),&rcUserInfo,nTextFormat);

      //�û��ȼ�
      LPCTSTR pszUserLevel=GetLevelDescribe(pIClientUserItem);
      _sntprintf(szBuffer,sizeof(szBuffer),TEXT("%s"),pszUserLevel);
      rcUserInfo.SetRect(m_ptNickName[i].x,m_ptNickName[i].y+19,m_ptNickName[i].x+nTextWidth,m_ptNickName[i].y+37);
      pDC->DrawText(szBuffer,lstrlen(szBuffer),&rcUserInfo,nTextFormat);

      //�û����
      LONGLONG lLeaveScore=pIClientUserItem->GetUserScore()-(pIClientUserItem->GetUserStatus()!=US_PLAYING?0:m_lTableScore[i]);
      _sntprintf(szBuffer,sizeof(szBuffer),TEXT("%I64d"),lLeaveScore);
      rcUserInfo.SetRect(m_ptNickName[i].x,m_ptNickName[i].y+38,m_ptNickName[i].x+nTextWidth,m_ptNickName[i].y+56);
      pDC->DrawText(szBuffer,lstrlen(szBuffer),&rcUserInfo,nTextFormat);

      //������Ϣ
      if(wUserTimer!=0)
      {
        if(m_bCallUser==0xff && (pIClientUserItem->GetUserStatus()==US_PLAYING||m_cbDynamicJoin))
        {
          DrawUserTimerEx(pDC,nWidth/2,nHeight/2-60,wUserTimer,99L);
        }
        else
        {
          if(i==1&&pIClientUserItem->GetUserStatus()!=US_PLAYING)
          {
            DrawUserTimerEx(pDC,m_ptAvatar[i].x-65-30-4,m_ptAvatar[1].y+(FACE_CY-57)/2+29,wUserTimer,99L);
          }
          else
          {
            DrawUserTimerEx(pDC,m_ptClock[i].x,m_ptClock[i].y,wUserTimer,99L);
          }
        }
      }
      if(pIClientUserItem->GetUserStatus()==US_READY)
      {
        DrawUserReadyEx(pDC,m_ptReady[i].x,m_ptReady[i].y);
      }

      //�滭����
      pDC->FillSolidRect(m_ptAvatar[i].x-2,m_ptAvatar[i].y-2,52,52,RGB(255,255,255));
      DrawUserAvatar(pDC,m_ptAvatar[i].x,m_ptAvatar[i].y,pIClientUserItem);
      m_PngUserBack.DrawImage(pDC,m_ptAvatar[i].x-4,m_ptAvatar[i].y-4);
    }
  }

  //�ȴ���ע
  if(m_bInvest)
  {
    m_PngWaitBet.DrawImage(pDC,nWidth/2-m_PngWaitBet.GetWidth()/2,nHeight/2-24);
  }

  //�ȴ�̯��
  if(m_bOpenCard)
  {
    m_PngWaitOpen.DrawImage(pDC,nWidth/2-m_PngWaitOpen.GetWidth()/2,nHeight/2-24);
  }

  //��������
  if(m_bUserAction)
  {
    m_PngAutoOpen.DrawImage(pDC,nWidth/2-m_PngAutoOpen.GetWidth()/2,nHeight/2+90);//��������,ϵͳ�Զ�Ϊ������
  }

  //�ȴ���ׯ
  if(m_bCallUser!=0xff/* && (m_bCallUser!=2 || m_bLookOnUser)*/)
  {
    m_ImageWaitCall.DrawImage(pDC,nWidth/2-m_ImageWaitCall.GetWidth()/2,nHeight/2-24);
  }

  //ׯ����Ϣ
  if(m_wBankerUser!=INVALID_CHAIR)
  {
    IClientUserItem * pIClientUserItem=GetClientUserItem(m_wBankerUser);
    if(pIClientUserItem!=NULL)
    {
      m_ImageBanker.DrawImage(pDC,m_PointBanker[m_wBankerUser].x,m_PointBanker[m_wBankerUser].y);
    }
  }

  //�滭����
  for(BYTE i=0; i<GAME_PLAYER; i++)
  {
    IClientUserItem * pIClientUserItem=GetClientUserItem(i);
    //m_lTableScore[i]=1000;
    if(pIClientUserItem!=NULL && m_lTableScore[i]!=0L)
    {
      //����λ��
      int iPos=1;
      if(m_bShowSocre[i])
      {
        if(m_lTableScore[i]>0L)
        {
          iPos=0;
        }
        else if(m_lTableScore[i]<=0L)
        {
          iPos=2;
        }
      }

      INT nX = m_ptJeton[i].x;
      INT nY =m_ptJeton[i].y;

      //�滭����
      m_ImageScore.TransDrawImage(pDC,nX,nY,m_ImageScore.GetWidth()/3,
                                  m_ImageScore.GetHeight(),m_ImageScore.GetWidth()/3*iPos,0,RGB(255,0,255));

      //�滭��Ϣ
      TCHAR tc[LEN_ACCOUNTS]=TEXT("");
      _sntprintf(tc,sizeof(tc),TEXT("%I64d"),m_lTableScore[i]);
      CRect rcDraw(nX,nY+8,nX+73,nY+31);
      DrawTextString(pDC,tc,RGB(250,250,250),RGB(0,0,0),&rcDraw);
    }
  }

  //��ʾ̯�Ʊ�־
  for(BYTE i=0; i<GAME_PLAYER; i++)
  {
    if(m_bOpenCardUser[i]==TRUE && i!=1)
    {
      m_PngOpenTag.DrawImage(pDC,m_ptOpenTag[i].x,m_ptOpenTag[i].y);
    }
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

  //�滭�˿�
  if(m_SendCardItemArray.GetCount()>0)
  {
    //��������
    tagSendCardItem * pSendCardItem=&m_SendCardItemArray[0];

    //��ȡ��С
    int nItemWidth=m_ImageCard.GetWidth()/13;
    int nItemHeight=m_ImageCard.GetHeight()/5;

    //�滭�˿�
    int nXPos=m_SendCardPos.x-nItemWidth/2;
    int nYPos=m_SendCardPos.y-nItemHeight/2+10;
    m_ImageCard.TransDrawImage(pDC,nXPos,nYPos,nItemWidth,nItemHeight,nItemWidth*2,nItemHeight*4,RGB(255,0,255));
  }

  //��ʾ����
  for(WORD i=0; i<GAME_PLAYER; i++)
  {
//#ifdef _DEBUG
//    m_bShow=true;
//    if((m_bOxValue[i]=i) || 1)
//#else
    if(m_bOxValue[i]!=0xff)
//#endif
    {
      if(m_bShow || i==MY_VIEW_CHAIRID)
      {
        INT nXpt = m_ptValue[i].x;
        INT nypt = m_ptValue[i].y;
        if(m_bOxValue[i]!=0)
        {
          m_ImageOxValue.DrawImage(pDC,nXpt,nypt);
          if(m_bOxValue[i]==10 || m_bOxValue[i]>=OX_THREE_SAME)
          {
            m_ImageDoulbeOx.DrawImage(pDC,nXpt,nypt);
          }
          else if(m_bOxValue[i]<10)
          {
            m_ImageNumber.DrawImage(pDC,nXpt+40,nypt+7,
                                    m_ImageNumber.GetWidth()/9,m_ImageNumber.GetHeight(),
                                    m_ImageNumber.GetWidth()/9*((int)m_bOxValue[i]-1),0);
          }
        }
        else if(m_bOxValue[i]==0)
        {
          m_ImageOxValueZero.DrawImage(pDC,nXpt,nypt);
        }
      }
    }
  }

  //��ʷ����
  if(m_bShowScore==true)
  {
    //���ֱ���
    m_PngHistoryScore.DrawImage(pDC,nWidth-35-m_sizeHistory.cx,5);

    //����λ��
    INT nYBenchmark=5;
    INT nXBenchmark=nWidth-35-m_sizeHistory.cx;
    UINT nFormat=DT_SINGLELINE|DT_END_ELLIPSIS|DT_VCENTER|DT_CENTER;

    //�滭����
    pDC->SetTextAlign(TA_LEFT|TA_TOP);

    //�滭��Ϣ
    for(WORD i=0; i<GAME_PLAYER; i++)
    {
      //��ȡ�û�
      IClientUserItem * pIClientUserItem=GetClientUserItem(i);
      //test
#ifdef _DEBUG
      //pIClientUserItem = GetClientUserItem(MYSELF_VIEW_ID);
#endif
      //end test
      if((pIClientUserItem==NULL)||(m_pHistoryScore[i]==NULL))
      {
        continue;
      }

      //λ�ü���
      INT nY1=27,nY2=nY1+18,nSpa=20;
      CRect rcAccounts(nXBenchmark+4,nYBenchmark+nY1+i*nSpa,nXBenchmark+68+7,nYBenchmark+nY2+i*nSpa);
      CRect rcTurnScore(nXBenchmark+70+15,nYBenchmark+nY1+i*nSpa,nXBenchmark+121+15,nYBenchmark+nY2+i*nSpa);
      CRect rcCollectScore(nXBenchmark+140,nYBenchmark+nY1+i*nSpa,nXBenchmark+191,nYBenchmark+nY2+i*nSpa);

      //������Ϣ
      TCHAR szTurnScore[16]=TEXT(""),szCollectScore[16]=TEXT("");
      _sntprintf(szTurnScore,CountArray(szTurnScore),TEXT("%I64d"),m_pHistoryScore[i]->lTurnScore);
      _sntprintf(szCollectScore,CountArray(szCollectScore),TEXT("%I64d"),m_pHistoryScore[i]->lCollectScore);

      //�滭��Ϣ
      if(i==1)
      {
        pDC->SetTextColor(RGB(255,255,0));
      }
      else
      {
        pDC->SetTextColor(RGB(255,255,255));
      }
      pDC->DrawText(pIClientUserItem->GetNickName(),lstrlen(pIClientUserItem->GetNickName()),&rcAccounts,nFormat);

      if(i==1)
      {
        pDC->SetTextColor(RGB(255,153,0));
      }
      else
      {
        pDC->SetTextColor(RGB(0,255,153));
      }
      pDC->DrawText(szTurnScore,lstrlen(szTurnScore),&rcTurnScore,nFormat);
      pDC->DrawText(szCollectScore,lstrlen(szCollectScore),&rcCollectScore,nFormat);
    }
  }

  return;
}

//�滭ʱ��
void CGameClientView::DrawUserTimerEx(CDC * pDC, int nXPos, int nYPos, WORD wTime, WORD wTimerArea)
{
//#ifdef _DEBUG
//  return;
//#endif
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
      m_CardControl[wChairID].SetCardData(cbCardData,cbCardCount);
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
    m_SendCardPos.x=m_SendStartPos.x;
    m_SendCardPos.y=m_SendStartPos.y;

    //λ�Ƽ���
    int nXCount=abs(m_SendEndingPos[SendCardItem.wChairID].x-m_SendStartPos.x)/SEND_PELS;
    int nYCount=abs(m_SendEndingPos[SendCardItem.wChairID].y-m_SendStartPos.y)/SEND_PELS;

    //���ô���
    m_wSendIndex=0;
    //m_wSendCount=__max(1,__max(nXCount,nYCount));
    m_wSendCount=3;

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

//�ȴ���ׯ
void CGameClientView::SetWaitCall(BYTE bCallUser)
{
  m_bCallUser=bCallUser;

  //���½���
  InvalidGameView(0,0,0,0);

  return ;
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

//������Ϣ
//void CGameClientView::SetScoreInfo(LONGLONG lTurnMaxScore,LONGLONG lTurnLessScore)
//{
//  m_lTurnMaxScore=lTurnMaxScore;
//  //m_lTurnLessScore=lTurnLessScore;
//
//  //���½���
//  InvalidGameView(0,0,0,0);
//  return;
//}

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
void CGameClientView::SetUserOxValue(WORD wChiarID,BYTE bValue)
{
  //ţţ����
  m_bOxValue[wChiarID]=bValue;

  //���½���
  InvalidGameView(0,0,0,0);

  return;
}

//��ʾ̯��
void CGameClientView::ShowOpenCard(WORD wChiarID,BOOL bShow)
{
  //̯���û�
  m_bOpenCardUser[wChiarID]=bShow;

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
    pDC->TextOut(nXPos+nXExcursion[i],nYPos+nYExcursion[i],pszString,nStringLength);
  }

  //�滭����
  pDC->SetTextColor(crText);
  pDC->TextOut(nXPos,nYPos,pszString,nStringLength);

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
    pDC->DrawText(pszString,nStringLength,&rcDraw,DT_VCENTER|DT_CENTER|DT_SINGLELINE|DT_END_ELLIPSIS);
  }

  //�滭����
  rcDraw.CopyRect(lpRect);
  pDC->SetTextColor(crText);
  pDC->DrawText(pszString,nStringLength,&rcDraw,DT_VCENTER|DT_CENTER|DT_SINGLELINE|DT_END_ELLIPSIS);

  return;
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

      //�����ƶ�
      //if(m_bCardAction && MoveCard()) bKillTime = false;

      //ɾ����ʱ��
      if(bKillTime)
      {
        m_bJettonAction = false;
        //m_bCardAction = false;
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
    case IDI_SEND_CARD:   //�ƶ��˿�
    {
      //��������
      m_wSendIndex++;

      //����λ��
      tagSendCardItem * pSendCardItem=&m_SendCardItemArray[0];
      if(pSendCardItem->wChairID%2!=0)
      {
        m_SendCardPos.x=m_SendStartPos.x+(m_SendEndingPos[pSendCardItem->wChairID].x-m_SendStartPos.x)*m_wSendIndex/m_wSendCount;
      }
      else
      {
        m_SendCardPos.y=m_SendStartPos.y+(m_SendEndingPos[pSendCardItem->wChairID].y-m_SendStartPos.y)*m_wSendIndex/m_wSendCount;
      }

      //ֹͣ�ж�
      if(m_wSendIndex>=m_wSendCount)
      {
        //��ȡ�˿�
        BYTE cbCardData[MAX_COUNT];
        WORD wChairID=pSendCardItem->wChairID;
        BYTE cbCardCount=(BYTE)m_CardControl[wChairID].GetCardData(cbCardData,CountArray(cbCardData));

        //�����˿�
        cbCardData[cbCardCount++]=pSendCardItem->cbCardData;
        m_CardControl[wChairID].SetCardData(cbCardData,cbCardCount);

        //ɾ������
        m_SendCardItemArray.RemoveAt(0);

        //��������
        if(m_SendCardItemArray.GetCount()>0)
        {
          //��������
          tagSendCardItem * pSendCardItem=&m_SendCardItemArray[0];

          //����λ��
          m_SendCardPos.x=m_SendStartPos.x;
          m_SendCardPos.y=m_SendStartPos.y;

          //λ�Ƽ���
          int nXCount=abs(m_SendEndingPos[pSendCardItem->wChairID].x-m_SendStartPos.x)/SEND_PELS;
          int nYCount=abs(m_SendEndingPos[pSendCardItem->wChairID].y-m_SendStartPos.y)/SEND_PELS;

          //���ô���
          m_wSendIndex=0;
          //m_wSendCount=__max(1,__max(nXCount,nYCount));
          m_wSendCount=3;

          //��������
          CGameClientEngine * pGameClientEngine=CONTAINING_RECORD(this,CGameClientEngine,m_GameClientView);
          pGameClientEngine->PlayGameSound(AfxGetInstanceHandle(),TEXT("SEND_CARD"));
        }
        else
        {
          //��ɴ���
          KillTimer(IDI_SEND_CARD);
          SendEngineMessage(IDM_SEND_CARD_FINISH,0,0);
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
  }

  __super::OnTimer(nIDEvent);
}
//����Ա����
void CGameClientView::OpenAdminWnd()
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
}

//////////////////////////////////////////////////////////////////////////
