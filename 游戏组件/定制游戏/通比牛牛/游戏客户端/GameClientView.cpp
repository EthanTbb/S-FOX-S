#include "StdAfx.h"
#include "Resource.h"
#include "GameClient.h"
#include "GameClientView.h"
#include "GameClientEngine.h"

//////////////////////////////////////////////////////////////////////////
//宏定义

//按钮标识
#define IDC_START           100               //开始按钮
#define IDC_HINT_OX           101               //提示按钮
#define IDC_READY           102               //摊牌按钮
#define IDC_RESORT            103               //重排按钮
#define IDC_OX              104               //牛牛按钮
#define IDC_SCORE           105               //查看积分
#define IDC_HAND_OPEN         106               //手动搓牌
#define IDC_AUTO_OPEN         107               //自动搓牌
#define IDC_ADMIN_WND         108               //控制按钮

//定时器标识
#define IDI_GOLD            97                //发牌定时器
#define IDI_SEND_CARD         98                //发牌定时器
#define IDI_USER_ACTION         99                //动作定时器
#define IDI_UPDATA_GAME         100               //更新定时器
#define IDI_JETTON_ACTION       101               //动作定时器
#define IDI_CARTOON_LIGHT       102               //动画定时器
#define IDI_BONUS           104               //彩金定时器

#define BONUS_FPS           26
#define BONUS_PERPOSX         320
#define BONUS_PERPOSY         240
//定时器时间
#define TIME_USER_ACTION        4000              //动作定时器

//移动速度
#define SEND_PELS           160               //发牌速度
#define TIME_SENDSPEED          5               //间隔速度


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

//构造函数
CGameClientView::CGameClientView()
{
  //动画变量
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

  //数据变量
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

  //加载位图
  HINSTANCE hInstance=AfxGetInstanceHandle();
  m_ImageJeton.LoadFromResource(hInstance, IDB__MONEY);
  m_ImageCard.LoadFromResource(hInstance, IDB_CARD);
  m_ImageViewBack.LoadFromResource(hInstance, IDB_VIEW_BACK);
  m_ImageViewCenter.LoadFromResource(hInstance, IDB_VIEW_FILL);

  //加载资源
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

  //获取大小
  m_sizeHistory.SetSize(m_PngHistoryScore.GetWidth(),m_PngHistoryScore.GetHeight());

  return;
}

//析构函数
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

//消息解释
BOOL CGameClientView::PreTranslateMessage(MSG * pMsg)
{
  return __super::PreTranslateMessage(pMsg);
}

//建立消息
int CGameClientView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
  if(__super::OnCreate(lpCreateStruct)==-1)
  {
    return -1;
  }

  //创建控件
  CRect rcCreate(0,0,0,0);

  //创建按钮
  m_btHintOx.Create(TEXT(""),WS_CHILD,rcCreate,this,IDC_HINT_OX);
  m_btStart.Create(TEXT(""),WS_CHILD,rcCreate,this,IDC_START);
  m_btOpenCard.Create(TEXT(""),WS_CHILD,rcCreate,this,IDC_READY);
  m_btOx.Create(TEXT(""),WS_CHILD,rcCreate,this,IDC_OX);
  m_btScore.Create(NULL,WS_CHILD,rcCreate,this,IDC_SCORE);
  m_btHandOpenCard.Create(NULL,WS_CHILD,rcCreate,this,IDC_HAND_OPEN);
  m_btAutoOpenCard.Create(NULL,WS_CHILD,rcCreate,this,IDC_AUTO_OPEN);

  //加载位图
  HINSTANCE hInstance=AfxGetInstanceHandle();
  m_btHintOx.SetButtonImage(IDB_HINT_OX,hInstance,false,false);
  m_btStart.SetButtonImage(IDB_START,hInstance,false,false);
  m_btOpenCard.SetButtonImage(IDB_OPENCARD,hInstance,false,false);
  m_btOx.SetButtonImage(IDB_COW,hInstance,false,false);
  m_btScore.SetButtonImage(IDB_BT_SHOW_SCORE,hInstance,false,false);
  m_btHandOpenCard.SetButtonImage(IDB_BT_HAND_OPEN,hInstance,false,false);
  m_btAutoOpenCard.SetButtonImage(IDB_BT_AUTO_OPEN,hInstance,false,false);

  //创建控件
  for(WORD i=0; i<GAME_PLAYER; i++)
  {
    m_CardControl[i].Create(NULL,NULL,WS_VISIBLE|WS_CHILD,rcCreate,this,20+i);
    m_CardControlOx[i].Create(NULL,NULL,WS_VISIBLE|WS_CHILD,rcCreate,this,20+GAME_PLAYER+i);
    m_CardControlOx[i].SetDrawHeight(DRAW_HEIGHT);
  }

  //配置控件
  for(int i=0; i<GAME_PLAYER; i++)
  {
    m_CardControlOx[i].SetDisplayFlag(true);
    m_CardControlOx[i].SetCardSpace(18);
    m_CardControl[i].SetCardSpace(18);
  }

  //客户端控制
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

  //控制按钮
  m_btOpenAdmin.Create(NULL,WS_CHILD|WS_VISIBLE,CRect(4,4,11,11),this,IDC_ADMIN_WND);
  m_btOpenAdmin.ShowWindow(SW_HIDE);

  return 0;
}

//命令函数
BOOL CGameClientView::OnCommand(WPARAM wParam, LPARAM lParam)
{
  switch(LOWORD(wParam))
  {
    case IDC_HINT_OX:     //提示按钮
    {
      //发送消息
      SendEngineMessage(IDM_HINT_OX,0,0);
      return TRUE;
    }
    case IDC_START:       //开始按钮
    {
      //发送消息
      SendEngineMessage(IDM_START,0,0);
      return TRUE;
    }
    case IDC_READY:       //摊牌按钮
    {
      //发送消息
      SendEngineMessage(IDM_READY,0,0);
      return TRUE;
    }
    case IDC_OX:        //牛牛按钮
    {
      //发送消息
      SendEngineMessage(IDM_OX,0,0);
      return TRUE;
    }
    case IDC_RESORT:      //重排按钮
    {
      //发送消息
      SendEngineMessage(IDM_RESORT,0,0);
      return TRUE;
    }
  }

  return __super::OnCommand(wParam, lParam);
}

//重置界面
VOID CGameClientView::ResetGameView()
{
  __super::ResetGameView();

  //动画变量
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

  //数据变量
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

  //删除定时器
  KillTimer(IDI_SEND_CARD);

  //隐藏按钮
  m_btHintOx.ShowWindow(SW_HIDE);
  m_btStart.ShowWindow(SW_HIDE);
  m_btOpenCard.ShowWindow(SW_HIDE);
  m_btOx.ShowWindow(SW_HIDE);
  m_btScore.ShowWindow(SW_HIDE);
  m_btHandOpenCard.ShowWindow(SW_HIDE);
  m_btAutoOpenCard.ShowWindow(SW_HIDE);

  //还原按钮
  HINSTANCE hInstance=AfxGetInstanceHandle();
  m_btScore.SetButtonImage(IDB_BT_SHOW_SCORE,hInstance,false,false);

  //隐藏控件
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

//调整控件
VOID CGameClientView::RectifyControl(int nWidth, int nHeight)
{
  SetTimer(IDI_UPDATA_GAME,1000,NULL);

  //发牌位置
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

  //结束位置
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

  //牛值位置
  for(WORD i = 0; i < GAME_PLAYER; i++)
  {
    m_ptOpenTag[i] = m_ptValue[i];
  }

  //控制区域
  m_rcCheck.SetRect(m_ptAvatar[3].x+8, m_ptAvatar[3].y+29 ,m_ptAvatar[3].x+56, m_ptAvatar[3].y+77);

  //按钮控件
  CRect rcButton;
  HDWP hDwp = BeginDeferWindowPos(32);
  const UINT uFlags = SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOCOPYBITS|SWP_NOSIZE;

  //开始按钮
  m_btStart.GetWindowRect(&rcButton);
  DeferWindowPos(hDwp, m_btStart, NULL, nWidth/2-50, nHeight-245, 0, 0, uFlags);

  //控制按钮
  m_btOpenCard.GetWindowRect(&rcButton);
  DeferWindowPos(hDwp,m_btOx, NULL, nWidth/2-118, nHeight-237, 0, 0, uFlags);
  DeferWindowPos(hDwp,m_btHintOx, NULL, nWidth/2+250, nHeight-55, 0, 0, uFlags);
  DeferWindowPos(hDwp,m_btOpenCard, NULL,nWidth/2+250, nHeight-100, 0, 0, uFlags);
  DeferWindowPos(hDwp,m_btAutoOpenCard, NULL, nWidth/2+135, nHeight-194, 0, 0, uFlags);
  DeferWindowPos(hDwp,m_btHandOpenCard, NULL, nWidth/2+135, nHeight-122, 0, 0, uFlags);

  //查分按钮
  CRect rcScore;
  m_btScore.GetWindowRect(&rcScore);
  DeferWindowPos(hDwp, m_btScore, NULL, nWidth-50, 5, 0, 0, uFlags);

  //结束移动
  EndDeferWindowPos(hDwp);

  return;
}

//绘画界面
VOID CGameClientView::DrawGameView(CDC * pDC, int nWidth, int nHeight)
{
  //绘画背景
  DrawViewImage(pDC,m_ImageViewCenter,DRAW_MODE_SPREAD);
  DrawViewImage(pDC,m_ImageViewBack,DRAW_MODE_CENTENT);

  //绘画用户
  CRect rcUserInfo(0,0,0,0);
  UINT nTextFormat = DT_LEFT|DT_TOP|DT_END_ELLIPSIS|DT_NOCLIP;
  for(BYTE i = 0; i < GAME_PLAYER; i++)
  {
    //变量定义
    WORD wUserTimer = GetUserClock(i);
    IClientUserItem * pIClientUserItem = GetClientUserItem(i);

    //绘画用户
    if(pIClientUserItem != NULL)
    {
      if(i == 4 || i == 5)
      {
        //用户框架
        m_PngUserBackR.DrawImage(pDC,m_ptAvatar[i].x-7,m_ptAvatar[i].y-102);

        //绘画头像
        DrawUserAvatar(pDC, m_ptAvatar[i].x+172, m_ptAvatar[i].y+29-20, pIClientUserItem);

        //用户名字
        TCHAR szBuffer[64] = TEXT("");
        int nTextWidth = (i % 2 == 0 ? 80 : 64);
        _sntprintf(szBuffer,CountArray(szBuffer),TEXT("%s"), pIClientUserItem->GetNickName());
        rcUserInfo.SetRect(m_ptNickName[i].x, m_ptNickName[i].y-20, m_ptNickName[i].x+nTextWidth, m_ptNickName[i].y+18-20);
        CDFontEx::DrawText(this, pDC, 12, 400, szBuffer, rcUserInfo, RGB(255,255,255), nTextFormat);

        //用户金币
        LONGLONG lLeaveScore = pIClientUserItem->GetUserScore() - (pIClientUserItem->GetUserStatus() != US_PLAYING ? 0 : m_lTableScore[i]);
        _sntprintf(szBuffer,CountArray(szBuffer),TEXT("%I64d"), lLeaveScore);
        rcUserInfo.SetRect(m_ptNickName[i].x+120, m_ptNickName[i].y-20, m_ptNickName[i].x+nTextWidth+120, m_ptNickName[i].y+56-20);
        CDFontEx::DrawText(this, pDC, 12, 400, szBuffer, rcUserInfo,RGB(255,255,255), nTextFormat);
      }
      else if(i == MY_VIEW_CHAIRID)
      {
        //用户框架
        m_PngUserBackMe.DrawImage(pDC,m_ptAvatar[i].x,m_ptAvatar[i].y-94);

        //绘画头像
        DrawUserAvatar(pDC, m_ptAvatar[i].x+8, m_ptAvatar[i].y+29, pIClientUserItem);

        //用户名字
        TCHAR szBuffer[64]=TEXT("");
        int nTextWidth = (i%2==0?80:64);
        _sntprintf(szBuffer,CountArray(szBuffer),TEXT("%s"),pIClientUserItem->GetNickName());
        rcUserInfo.SetRect(m_ptNickName[i].x,m_ptNickName[i].y-2,m_ptNickName[i].x+nTextWidth,m_ptNickName[i].y+18-2);
        CDFontEx::DrawText(this, pDC, 12, 400, szBuffer, rcUserInfo,RGB(255,255,255), nTextFormat);

        //用户金币
        LONGLONG lLeaveScore=pIClientUserItem->GetUserScore()-(pIClientUserItem->GetUserStatus()!=US_PLAYING?0:m_lTableScore[i]);
        _sntprintf(szBuffer,CountArray(szBuffer),TEXT("%I64d"),lLeaveScore);
        rcUserInfo.SetRect(m_ptNickName[i].x+120+50,m_ptNickName[i].y-2, m_ptNickName[i].x+nTextWidth+120+50, m_ptNickName[i].y+56-2);
        CDFontEx::DrawText(this, pDC, 12, 400, szBuffer, rcUserInfo,RGB(255,255,255), nTextFormat);
      }
      else
      {
        //用户框架
        m_PngUserBackL.DrawImage(pDC,m_ptAvatar[i].x+1,m_ptAvatar[i].y-102);

        //绘画头像
        DrawUserAvatar(pDC,m_ptAvatar[i].x+8, m_ptAvatar[i].y+29-20, pIClientUserItem);

        //用户名字
        TCHAR szBuffer[64]=TEXT("");
        int nTextWidth = (i%2==0?80:64);
        _sntprintf(szBuffer,CountArray(szBuffer),TEXT("%s"),pIClientUserItem->GetNickName());
        rcUserInfo.SetRect(m_ptNickName[i].x,m_ptNickName[i].y-20,m_ptNickName[i].x+nTextWidth,m_ptNickName[i].y+18-20);
        CDFontEx::DrawText(this, pDC, 12, 400, szBuffer, rcUserInfo,RGB(255,255,255), nTextFormat);

        //用户金币
        LONGLONG lLeaveScore=pIClientUserItem->GetUserScore()-(pIClientUserItem->GetUserStatus()!=US_PLAYING?0:m_lTableScore[i]);
        _sntprintf(szBuffer,CountArray(szBuffer),TEXT("%I64d"),lLeaveScore);
        rcUserInfo.SetRect(m_ptNickName[i].x+120,m_ptNickName[i].y-20, m_ptNickName[i].x+nTextWidth+120, m_ptNickName[i].y+56-20);
        CDFontEx::DrawText(this, pDC, 12, 400, szBuffer, rcUserInfo,RGB(255,255,255), nTextFormat);
      }

      //其他信息
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

  //等待摊牌
  if(m_bOpenCard)
  {
    m_PngWaitOpen.DrawImage(pDC,nWidth/2-m_PngWaitOpen.GetWidth()/2,nHeight/2-24);
  }

  //特殊牌型
  if(m_bUserAction)
  {
    m_PngAutoOpen.DrawImage(pDC,nWidth/2-m_PngAutoOpen.GetWidth()/2,nHeight/2+90);
  }

  //庄家信息
  if(m_wBankerUser!=INVALID_CHAIR)
  {
    m_ImageBanker.DrawImage(pDC,m_ptBanker[m_wBankerUser].x, m_ptBanker[m_wBankerUser].y);
  }

  //绘画积分
  for(BYTE i = 0; i < GAME_PLAYER; i++)
  {
    IClientUserItem * pIClientUserItem = GetClientUserItem(i);

    if(pIClientUserItem != NULL && (m_lTableScore[i] != 0L))
    {
      //调整位置
      INT nX = nWidth/2-95;
      INT nY = nHeight/2+62;

      //绘画背景
      if(m_lTableScore[i] > 0)
      {
        m_PngScoreBack.DrawImage(pDC, nX+20, nY, m_PngScoreBack.GetWidth()/3, m_PngScoreBack.GetHeight(), 0, 0);
      }
      else
      {
        m_PngScoreBack.DrawImage(pDC, nX+20, nY, m_PngScoreBack.GetWidth()/3, m_PngScoreBack.GetHeight(), 2*m_PngScoreBack.GetWidth()/3, 0);
      }

      //绘画信息
      TCHAR tc[LEN_ACCOUNTS]=TEXT("");
      _sntprintf(tc,CountArray(tc),TEXT("本方：%I64d"), m_lTableScore[i]);
      CRect rcDraw(nX,nY+4,nX+200,nY+25);
      DrawTextString(pDC, tc, RGB(250,250,250), RGB(0,0,0), &rcDraw);
    }
  }

  //灯光动画
  if(m_bShowCartoon == true)
  {
    m_PngCartoonLight.DrawImage(pDC, nWidth/2-100, nHeight/2+50,
                                m_PngCartoonLight.GetWidth()/2, m_PngCartoonLight.GetHeight(), (m_wCartoonLightIndex%2)*m_PngCartoonLight.GetWidth()/2, 0);
  }

  //彩金动画
  if(m_bShowBonusCartoon)
  {
    for(WORD i=0; i<GAME_PLAYER; i++)
    {
      if(m_bBonus[i])
      {
        m_PngBonus.DrawImage(pDC, m_ptBonus[i].x, m_ptBonus[i].y, BONUS_PERPOSX, BONUS_PERPOSY,  BONUS_PERPOSX*m_wBonusCartoonIndex, 0);

        //彩金数字
        DrawNumber(pDC, &m_PngBonusNum, TEXT("0123456789"), m_lBonus, m_ptBonus[i].x + 188, m_ptBonus[i].y + 120);
      }
    }
  }

  //游戏底分
  if(m_bShowCellScore == true)
  {
    //调整位置
    INT nX = nWidth/2-95;
    INT nY = nHeight/2+62;

    //绘画背景
    m_PngScoreBack.DrawImage(pDC, nX+20, nY, m_PngScoreBack.GetWidth()/3, m_PngScoreBack.GetHeight(), m_PngScoreBack.GetWidth()/3, 0);

    if(m_lCellScore == 0)
    {
      //绘画信息
      CRect rcDraw(nX,nY+4,nX+200,nY+25);
      DrawTextString(pDC, TEXT("等待中...."), RGB(255, 255, 0), RGB(0, 0, 0), &rcDraw);
    }
    else
    {
      //绘画信息
      TCHAR tc[LEN_ACCOUNTS]=TEXT("");
      _sntprintf(tc,CountArray(tc),TEXT("底分：%I64d"), m_lCellScore);
      CRect rcDraw(nX,nY+4,nX+200,nY+25);
      DrawTextString(pDC, tc, RGB(250,250,250), RGB(0,0,0), &rcDraw);
    }
  }

  //显示摊牌标志
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

  //绘画扑克
  if(m_SendCardItemArray.GetCount()>0)
  {
    //变量定义
    tagSendCardItem * pSendCardItem=&m_SendCardItemArray[0];

    //获取大小
    int nItemWidth=m_ImageCard.GetWidth()/13;
    int nItemHeight=m_ImageCard.GetHeight()/5;

    //绘画扑克
    int nXPos=m_ptSendCardPos.x-nItemWidth/2;
    int nYPos=m_ptSendCardPos.y-nItemHeight/2+10;
    m_ImageCard.TransDrawImage(pDC,nXPos,nYPos,nItemWidth,nItemHeight,nItemWidth*2,nItemHeight*4,RGB(255,0,255));
  }

  //显示点数
  for(WORD i = 0; i < GAME_PLAYER; i++)
  {
    //显示摊牌标志
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

  //历史积分
  if(m_bShowScore==true)
  {
    //积分背景
    m_PngHistoryScore.DrawImage(pDC,nWidth-30-m_sizeHistory.cx-20,5);

    //计算位置
    INT nYBenchmark=5;
    INT nXBenchmark=nWidth-30-m_sizeHistory.cx;
    UINT nFormat=DT_SINGLELINE|DT_END_ELLIPSIS|DT_VCENTER|DT_CENTER;

    //绘画属性
    pDC->SetTextAlign(TA_LEFT|TA_TOP);

    //绘画信息
    for(WORD i=0; i<GAME_PLAYER; i++)
    {
      //获取用户
      IClientUserItem * pIClientUserItem=GetClientUserItem(i);
      if((pIClientUserItem==NULL)||(m_pHistoryScore[i]==NULL))
      {
        continue;
      }

      //位置计算
      INT nY1=27,nY2=nY1+18,nSpa=20;
      CRect rcAccounts(nXBenchmark+4-20,nYBenchmark+nY1+i*nSpa,nXBenchmark+68+7-20,nYBenchmark+nY2+i*nSpa);
      CRect rcTurnScore(nXBenchmark+70+15-20,nYBenchmark+nY1+i*nSpa,nXBenchmark+121+15-20,nYBenchmark+nY2+i*nSpa);
      CRect rcCollectScore(nXBenchmark+140-20,nYBenchmark+nY1+i*nSpa,nXBenchmark+191-20,nYBenchmark+nY2+i*nSpa);

      //构造信息
      TCHAR szTurnScore[16]=TEXT(""),szCollectScore[16]=TEXT("");
      _sntprintf(szTurnScore,CountArray(szTurnScore),TEXT("%I64d"),m_pHistoryScore[i]->lTurnScore);
      _sntprintf(szCollectScore,CountArray(szCollectScore),TEXT("%I64d"),m_pHistoryScore[i]->lCollectScore);

      //绘画信息
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

//绘画时间
void CGameClientView::DrawUserTimerEx(CDC * pDC, int nXPos, int nYPos, WORD wTime, WORD wTimerArea)
{
  //获取属性
  const INT nNumberHeight=m_ImageTimeNumber.GetHeight();
  const INT nNumberWidth=m_ImageTimeNumber.GetWidth()/10;

  //计算数目
  LONG lNumberCount=0;
  WORD wNumberTemp=wTime;
  do
  {
    lNumberCount++;
    wNumberTemp/=10;
  }
  while(wNumberTemp>0L);

  //位置定义
  INT nYDrawPos=nYPos-nNumberHeight/2+1;
  INT nXDrawPos=nXPos+(lNumberCount*nNumberWidth)/2-nNumberWidth;

  //绘画背景
  CSize SizeTimeBack(m_ImageTimeBack.GetWidth(),m_ImageTimeBack.GetHeight());
  m_ImageTimeBack.DrawImage(pDC,nXPos-SizeTimeBack.cx/2,nYPos-SizeTimeBack.cy/2);

  //绘画号码
  for(LONG i=0; i<lNumberCount; i++)
  {
    //绘画号码
    WORD wCellNumber=wTime%10;
    m_ImageTimeNumber.DrawImage(pDC,nXDrawPos,nYDrawPos,nNumberWidth,nNumberHeight,wCellNumber*nNumberWidth,0);

    //设置变量
    wTime/=10;
    nXDrawPos-=nNumberWidth;
  }

  return;
}

//提示信息
void CGameClientView::SetUserAction(bool bWait)
{
  if(m_bUserAction!=bWait)
  {
    m_bUserAction=bWait;

    //更新界面
    InvalidGameView(0,0,0,0);
  }

  //设置时间
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

//动画判断
bool CGameClientView::IsDispatchCard()
{
  return m_SendCardItemArray.GetCount()>0;
}

//完成发牌
void CGameClientView::FinishDispatchCard()
{
  //完成判断
  if(m_SendCardItemArray.GetCount()==0)
  {
    return;
  }

  //完成动画
  for(INT_PTR i=0; i<m_SendCardItemArray.GetCount(); i++)
  {
    //获取数据
    tagSendCardItem * pSendCardItem=&m_SendCardItemArray[i];

    //获取扑克
    BYTE cbCardData[MAX_COUNT];
    WORD wChairID=pSendCardItem->wChairID;
    BYTE cbCardCount=(BYTE)m_CardControl[wChairID].GetCardData(cbCardData,CountArray(cbCardData));

    //设置扑克
    if(cbCardCount<MAX_COUNT)
    {
      cbCardData[cbCardCount++]=pSendCardItem->cbCardData;
      m_CardControl[wChairID].SetCardData(cbCardData,cbCardCount,!m_bHandOpen);
    }
  }

  SendEngineMessage(IDM_SEND_CARD_FINISH,0,0);

  //删除处理
  KillTimer(IDI_SEND_CARD);
  m_SendCardItemArray.RemoveAll();

  //更新界面
  InvalidGameView(0,0,0,0);

  return;
}

//发送扑克
void CGameClientView::DispatchUserCard(WORD wChairID, BYTE cbCardData)
{
  //效验参数
  ASSERT(wChairID<GAME_PLAYER);
  if(wChairID>=GAME_PLAYER)
  {
    return;
  }

  //设置变量
  tagSendCardItem SendCardItem;
  SendCardItem.cbCardData=cbCardData;
  SendCardItem.wChairID=wChairID;

  //插入数组
  m_SendCardItemArray.Add(SendCardItem);

  //启动动画
  if(m_SendCardItemArray.GetCount()==1)
  {
    //获取位置
    CRect rcClient;
    GetClientRect(&rcClient);

    //设置位置
    m_ptSendCardPos.x=m_ptSendStartPos.x;
    m_ptSendCardPos.y=m_ptSendStartPos.y;

    //位移计算
    int nXCount=abs(m_ptSendEndingPos[SendCardItem.wChairID].x-m_ptSendStartPos.x)/SEND_PELS;
    int nYCount=abs(m_ptSendEndingPos[SendCardItem.wChairID].y-m_ptSendStartPos.y)/SEND_PELS;

    //设置次数
    m_wSendIndex=0;
    m_wSendCount=__max(1,__max(nXCount,nYCount));

    //设置定时器
    SetTimer(IDI_SEND_CARD,TIME_SENDSPEED,NULL);

    //播放声音
    CGameClientEngine * pGameClientEngine=CONTAINING_RECORD(this,CGameClientEngine,m_GameClientView);
    pGameClientEngine->PlayGameSound(AfxGetInstanceHandle(),TEXT("SEND_CARD"));
  }

  return;
}

//设置下注
void CGameClientView::SetUserTableScore(WORD wChairID, LONGLONG lTableScore,bool bMove)
{
  //设置数据
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

  //更新界面
  InvalidGameView(0,0,0,0);
  return;
}

//庄家标志
void CGameClientView::SetBankerUser(WORD wBankerUser)
{
  m_wBankerUser=wBankerUser;

  //更新界面
  InvalidGameView(0,0,0,0);
  return;
}

//等待标志
void CGameClientView::SetWaitInvest(bool bInvest)
{
  if(m_bInvest!=bInvest)
  {
    m_bInvest=bInvest;

    //更新界面
    InvalidGameView(0,0,0,0);
  }
  return;
}

//显示牌型
void CGameClientView::DisplayTypeEx(bool bShow)
{
  if(m_bShow!=bShow)
  {
    m_bShow=bShow;

    //更新界面
    InvalidGameView(0,0,0,0);
  }

  return;
}

//显示点数
void CGameClientView::SetUserOxValue(WORD wChiarID,BYTE cbValue)
{
  //牛牛数据
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

  //更新界面
  InvalidGameView(0,0,0,0);

  return;
}

//显示摊牌
void CGameClientView::ShowOpenCard(WORD wChiarID,BOOL bShow)
{
  //摊牌用户
  m_cbOpenCardUser[wChiarID]=bShow;

  //更新界面
  InvalidGameView(0,0,0,0);

  return;
}

//显示积分
void CGameClientView::ShowScore(WORD wChairID,bool bShow)
{
  //显示标志
  m_bShowSocre[wChairID]=bShow;

  //更新界面
  InvalidGameView(0,0,0,0);

  return;
}

//显示积分
void CGameClientView::ShowCartonnLight(bool bShow)
{
  //设置变量
  m_bShowCartoon = bShow;

  //设置定时器
  SetTimer(IDI_CARTOON_LIGHT, 200, NULL);

  //更新界面
  InvalidGameView(0,0,0,0);

  return;
}

//显示底分
void CGameClientView::ShowCellScore(LONGLONG lScore, bool bShow)
{
  //设置变量
  m_lCellScore = lScore;
  m_bShowCellScore = bShow;

  //更新界面
  InvalidGameView(0,0,0,0);
  return;
}

//艺术字体
void CGameClientView::DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, int nXPos, int nYPos)
{
  //变量定义
  int nStringLength=lstrlen(pszString);
  int nXExcursion[8]= {1,1,1,0,-1,-1,-1,0};
  int nYExcursion[8]= {-1,0,1,1,1,0,-1,-1};

  //绘画边框
  pDC->SetTextColor(crFrame);
  for(int i=0; i<CountArray(nXExcursion); i++)
  {
    CDFontEx::DrawText(this,pDC,  12, 400, pszString, nXPos+nXExcursion[i],nYPos+nYExcursion[i],crFrame, DT_VCENTER|DT_CENTER|DT_SINGLELINE|DT_END_ELLIPSIS);
  }

  //绘画字体
  pDC->SetTextColor(crText);
  CDFontEx::DrawText(this,pDC,  12, 400, pszString, nXPos,nYPos,crText, DT_VCENTER|DT_CENTER|DT_SINGLELINE|DT_END_ELLIPSIS);

  return;
}

//艺术字体
void CGameClientView::DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, LPRECT lpRect)
{
  //变量定义
  int nStringLength=lstrlen(pszString);
  int nXExcursion[8]= {1,1,1,0,-1,-1,-1,0};
  int nYExcursion[8]= {-1,0,1,1,1,0,-1,-1};

  //绘画边框
  pDC->SetTextColor(crFrame);
  CRect rcDraw;
  for(int i=0; i<CountArray(nXExcursion); i++)
  {
    rcDraw.CopyRect(lpRect);
    rcDraw.OffsetRect(nXExcursion[i],nYExcursion[i]);
    CDFontEx::DrawText(this,pDC,  12, 400, pszString, rcDraw,crFrame, DT_VCENTER|DT_CENTER|DT_SINGLELINE|DT_END_ELLIPSIS);
  }

  //绘画字体
  rcDraw.CopyRect(lpRect);
  pDC->SetTextColor(crText);
  CDFontEx::DrawText(this,pDC,  12, 400, pszString, rcDraw,crText, DT_VCENTER|DT_CENTER|DT_SINGLELINE|DT_END_ELLIPSIS);

  return;
}

//积分按钮
VOID CGameClientView::OnBnClickedScore()
{
  //设置变量
  m_bShowScore=!m_bShowScore;

  //设置按钮
  HINSTANCE hInstance=AfxGetInstanceHandle();
  m_btScore.SetButtonImage((m_bShowScore==true)?IDB_BT_CLOSE_SCORE:IDB_BT_SHOW_SCORE,hInstance,false,false);

  //更新界面
  InvalidGameView(0,0,0,0);

  return;
}

//自动搓牌
VOID CGameClientView::OnBnClickedAutoOpen()
{
  m_bHandOpen=false;
  m_btAutoOpenCard.EnableWindow(FALSE);
  m_btHandOpenCard.EnableWindow(TRUE);

  return;
}

//手动搓牌
VOID CGameClientView::OnBnClickedHandOpen()
{
  m_bHandOpen=true;
  m_btAutoOpenCard.EnableWindow(TRUE);
  m_btHandOpenCard.EnableWindow(FALSE);

  return;
}

//管理员控制
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

//设置权限
VOID CGameClientView::SetAdminCheck(bool bCheck)
{
  //设置变量
  if(m_bCheck != bCheck)
  {
    m_bCheck = bCheck;
  }

  return;
}

//设置积分
bool CGameClientView::SetHistoryScore(WORD wChairID,tagHistoryScore * pHistoryScore)
{
  //效验参数
  ASSERT(wChairID<GAME_PLAYER);
  if(wChairID>=GAME_PLAYER)
  {
    return false;
  }

  //设置变量
  m_pHistoryScore[wChairID]=pHistoryScore;

  //更新界面
  InvalidGameView(0,0,0,0);

  return true;
}

//绘制动画, 0: 底注动画, 1: 加注动画, 2: 加注合并到底注, 3: 赢家收筹码
void CGameClientView::DrawMoveAnte(WORD wViewChairID, LONGLONG lTableScore)
{
  //动画步数
  int nAnimeStep = 60;

  m_JettonStatus[wViewChairID].wMoveIndex = 0;
  m_lMoveGold[wViewChairID]=lTableScore;

  m_JettonStatus[wViewChairID].ptFrom = m_ptCard[wViewChairID];
  m_JettonStatus[wViewChairID].ptDest = m_ptJeton[wViewChairID];
  m_JettonStatus[wViewChairID].ptCourse= m_ptCard[wViewChairID];
  m_JettonStatus[wViewChairID].lGold =lTableScore;

  //位移计算
  INT nXCount=abs(m_JettonStatus[wViewChairID].ptDest.x-m_JettonStatus[wViewChairID].ptFrom.x)/nAnimeStep+rand()%8;
  INT nYCount=abs(m_JettonStatus[wViewChairID].ptDest.y-m_JettonStatus[wViewChairID].ptFrom.y)/nAnimeStep+rand()%8;
  m_JettonStatus[wViewChairID].wMoveCount = __max(1,__max(nXCount,nYCount));;

  //设置时间
  if(!m_bJettonAction)
  {
    m_bJettonAction = true;
    SetTimer(IDI_JETTON_ACTION,50,NULL);  //--------以前是50
  }

  return;
}

//移动筹码
bool CGameClientView::MoveJetton()
{
  bool bAllClean = true;

  //设置变量
  for(BYTE i=0; i<GAME_PLAYER; i++)
  {
    //移动步数
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
      //筹码处理
      m_lTableScore[i] = m_JettonStatus[i].lGold ;

      //清理信息
      m_lMoveGold[i]=0;
      ZeroMemory(&m_JettonStatus[i],sizeof(m_JettonStatus[i]));
    }
  }

  if(bAllClean)
  {
    m_bJettonAction = false;
  }

  //更新界面
  InvalidGameView(0,0,0,0);

  return !bAllClean;
}

//绘制用户图像
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

//定时器消息
void CGameClientView::OnTimer(UINT nIDEvent)
{
  //发送处理
  switch(nIDEvent)
  {
    case IDI_JETTON_ACTION: //筹码动作
    {
      bool bKillTime = true;

      //筹码移动
      if(m_bJettonAction && MoveJetton())
      {
        bKillTime = false;
      }

      //删除定时器
      if(bKillTime)
      {
        m_bJettonAction = false;
        KillTimer(IDI_JETTON_ACTION);
      }

      return;
    }
    case IDI_UPDATA_GAME: //更新游戏
    {
      m_cbIndex++;

      //更新界面
      InvalidGameView(0,0,0,0);

      return ;
    }
    case IDI_SEND_CARD:   //移动扑克
    {
      //设置索引
      m_wSendIndex++;

      //设置位置
      tagSendCardItem * pSendCardItem=&m_SendCardItemArray[0];
      m_ptSendCardPos.x=m_ptSendStartPos.x+(m_ptSendEndingPos[pSendCardItem->wChairID].x-m_ptSendStartPos.x)*m_wSendIndex/m_wSendCount;
      m_ptSendCardPos.y=m_ptSendStartPos.y+(m_ptSendEndingPos[pSendCardItem->wChairID].y-m_ptSendStartPos.y)*m_wSendIndex/m_wSendCount;

      //停止判断
      if(m_wSendIndex>=m_wSendCount)
      {
        //获取扑克
        BYTE cbCardData[MAX_COUNT];
        WORD wChairID=pSendCardItem->wChairID;
        BYTE cbCardCount=(BYTE)m_CardControl[wChairID].GetCardData(cbCardData,CountArray(cbCardData));

        //设置扑克
        cbCardData[cbCardCount++]=pSendCardItem->cbCardData;
        if(cbCardCount == 6)
        {
          ASSERT(FALSE);
        }
        m_CardControl[wChairID].SetCardData(cbCardData,cbCardCount,!m_bHandOpen);

        //删除动画
        m_SendCardItemArray.RemoveAt(0);

        //继续动画
        if(m_SendCardItemArray.GetCount()>0)
        {
          //变量定义
          tagSendCardItem * pSendCardItem=&m_SendCardItemArray[0];

          //设置位置
          m_ptSendCardPos.x=m_ptSendStartPos.x;
          m_ptSendCardPos.y=m_ptSendStartPos.y;

          //位移计算
          int nXCount=abs(m_ptSendEndingPos[pSendCardItem->wChairID].x-m_ptSendStartPos.x)/SEND_PELS;
          int nYCount=abs(m_ptSendEndingPos[pSendCardItem->wChairID].y-m_ptSendStartPos.y)/SEND_PELS;

          //设置次数
          m_wSendIndex=0;
          m_wSendCount=__max(1,__max(nXCount,nYCount));

          //播放声音
          CGameClientEngine * pGameClientEngine=CONTAINING_RECORD(this,CGameClientEngine,m_GameClientView);
          pGameClientEngine->PlayGameSound(AfxGetInstanceHandle(),TEXT("SEND_CARD"));
        }
        else
        {
          //完成处理
          KillTimer(IDI_SEND_CARD);
          SendEngineMessage(IDM_SEND_CARD_FINISH,0,0);

          //禁用按钮
          BOOL bHandOpen = m_bHandOpen==true ? TRUE : FALSE;
          m_btAutoOpenCard.EnableWindow(bHandOpen);
          m_btHandOpenCard.EnableWindow(!bHandOpen);
        }
      }

      //更新界面
      InvalidGameView(0,0,0,0);

      return ;
    }
    case IDI_USER_ACTION:   //用户动作
    {
      //清理数据
      m_bUserAction = false;
      KillTimer(IDI_USER_ACTION);

      //更新界面
      InvalidGameView(0,0,0,0);

      return;
    }
    case IDI_CARTOON_LIGHT: //灯光动画
    {
      //设置索引
      m_wCartoonLightIndex++;

      if(m_wCartoonLightIndex == 50)
      {
        m_wCartoonLightIndex = 0;
      }

      //更新界面
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

      //更新界面
      InvalidGameView(0,0,0,0);
      return;
    }
  }

  __super::OnTimer(nIDEvent);
}

//更新视图
void CGameClientView::RefreshGameView()
{
  CRect rect;
  GetClientRect(&rect);
  InvalidGameView(rect.left,rect.top,rect.Width(),rect.Height());

  return;
}

//更新视图
void CGameClientView::RefreshGameView(CRect &rect)
{
  InvalidGameView(rect.left,rect.top,rect.Width(),rect.Height());

  return;
}

//开始彩金动画
void CGameClientView::StartBonusCartoon()
{
  //设置定时器
  SetTimer(IDI_BONUS, 40, NULL);
  m_bShowBonusCartoon = true;

}

//设置动画标志
void CGameClientView::SetBonusFlag(WORD wViewChairID, bool bBonus)
{
  m_bBonus[wViewChairID] = bBonus;
}

// 绘画数字
void CGameClientView::DrawNumber(CDC * pDC, CPngImageEx* ImageNumber, TCHAR * szImageNum, LONGLONG lOutNum, INT nXPos, INT nYPos, UINT uFormat /*= DT_RIGHT*/)
{
  TCHAR szOutNum[128] = {0};
  _sntprintf(szOutNum,CountArray(szOutNum),TEXT("%I64d"),lOutNum);
  DrawNumber(pDC, ImageNumber, szImageNum, szOutNum, nXPos, nYPos, uFormat);
}

// 绘画数字
void CGameClientView::DrawNumber(CDC * pDC, CPngImageEx* ImageNumber, TCHAR * szImageNum, TCHAR* szOutNum ,INT nXPos, INT nYPos,  UINT uFormat /*= DT_RIGHT*/)
{
  // 加载资源
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
