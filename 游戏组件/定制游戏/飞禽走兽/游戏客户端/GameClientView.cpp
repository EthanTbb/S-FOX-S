#include "StdAfx.h"
#include "Math.h"
#include "Resource.h"
#include "GameClientView.h"
#include "GameClientEngine.h"


//////////////////////////////////////////////////////////////////////////

//ʱ���ʶ
#define IDI_FLASH_WINNER       100                  //������ʶ
#define IDI_SHOW_CHANGE_BANKER     101                  //�ֻ�ׯ��
//#define IDI_DISPATCH_CARD      102                  //���Ʊ�ʶ
#define IDI_SHOWDISPATCH_CARD_TIP  103                  //������ʾ
#define IDI_MOVECARD_END       104                  //�Ƅ��ƽY��
#define IDI_HANDLELEAVE_ANIMATION  105
#define IDI_FLASH_CARD         106                  //������ʶ
#define IDI_FLASH_RAND_SIDE      107                  //������ʶ
#define IDI_END_FLASH_CARD       108
#define IDI_FLASH_ADD_MULTI          109                  //������ʶ
#define IDI_END_ADD_MULTI            110                  //��������
#define IDI_FLASH_ANOTHER_CARD       111
#define IDI_SHOW_ANOTHER_CARD_RESULT 112
#define IDI_SHOW_ANIMAL              113                  //��ʾ����



#define IDI_FLASH_NUMBER1              114                  //
#define IDI_END_FLASH_NUMBER1          115                  //

#define IDI_FLASH_NUMBER2              116                  //
#define IDI_END_FLASH_NUMBER2          117                  //

//��������ע,�������ж�ʱ����������(1000�Ժ�),ר�������ṩ
#define IDI_ANDROID_BET       1000

//��ť��ʶ
#define IDC_JETTON_BUTTON_10    200                 //��ť��ʶ
#define IDC_JETTON_BUTTON_100   201                 //��ť��ʶ
#define IDC_JETTON_BUTTON_1000    202                 //��ť��ʶ
#define IDC_JETTON_BUTTON_10000   203                 //��ť��ʶ
#define IDC_JETTON_BUTTON_100000  204                 //��ť��ʶ
#define IDC_JETTON_BUTTON_1000000 205                 //��ť��ʶ
#define IDC_JETTON_BUTTON_5000000 208                 //��ť��ʶ
#define IDC_APPY_BANKER       206                 //��ť��ʶ
#define IDC_CANCEL_BANKER     207                 //��ť��ʶ
#define IDC_SCORE_MOVE_L      209                 //��ť��ʶ
#define IDC_SCORE_MOVE_R      210                 //��ť��ʶ
#define IDC_VIEW_CHART        211                 //��ť��ʶ
#define IDC_AUTO_OPEN_CARD      213                 //��ť��ʶ
#define IDC_OPEN_CARD       214                 //��ť��ʶ
#define IDC_BANK          215                 //��ť��ʶ
#define IDC_BANK_STORAGE      217                 //��ť��ʶ
#define IDC_BANK_DRAW       218                 //��ť��ʶ

#define IDC_ADMIN         226                 //��ť��ʶ

#define IDC_BUTTON_ADD_0      227                 //��ť��ʶ
#define IDC_BUTTON_ADD_1      228                 //��ť��ʶ
#define IDC_BUTTON_ADD_2      229                 //��ť��ʶ
#define IDC_BUTTON_ADD_3      230                 //��ť��ʶ
#define IDC_BUTTON_ADD_4      231                 //��ť��ʶ
#define IDC_BUTTON_ADD_5      232                 //��ť��ʶ
#define IDC_BUTTON_ADD_6      233                 //��ť��ʶ
#define IDC_BUTTON_ADD_7      234                 //��ť��ʶ
#define IDC_BUTTON_ADD_8      235                 //��ť��ʶ
#define IDC_BUTTON_ADD_9      236                 //��ť��ʶ
#define IDC_BUTTON_ADD_10     237                 //��ť��ʶ
#define IDC_CON_JETTON        238                 //��ť��ʶ  

#define IDC_COMMIT_PLACE_JETTON     238                 //��ť��ʶ
//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameClientView, CGameFrameViewGDI)
  ON_WM_TIMER()
  ON_WM_CREATE()
  ON_WM_SETCURSOR()
  ON_WM_LBUTTONDOWN()
  ON_WM_LBUTTONUP()
  ON_WM_RBUTTONDOWN()
  ON_WM_MOUSEMOVE()
  ON_BN_CLICKED(IDC_ADMIN,OpenAdminWnd)
  ON_BN_CLICKED(IDC_APPY_BANKER, OnApplyBanker)
  ON_BN_CLICKED(IDC_CANCEL_BANKER, OnCancelBanker)
  ON_BN_CLICKED(IDC_SCORE_MOVE_L, OnScoreMoveL)
  ON_BN_CLICKED(IDC_SCORE_MOVE_R, OnScoreMoveR)
  ON_MESSAGE(WM_VIEWLBTUP,OnViLBtUp)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameClientView::CGameClientView()
{
  //��ע��Ϣ
  ZeroMemory(m_lUserJettonScore,sizeof(m_lUserJettonScore));

  //ȫ����ע
  ZeroMemory(m_lAllJettonScore,sizeof(m_lAllJettonScore));
  ZeroMemory(m_lUserLastJetton,sizeof(m_lUserLastJetton));
  ZeroMemory(m_lUserAddScore,sizeof(m_lUserAddScore));
  //ׯ����Ϣ
  m_wBankerUser=INVALID_CHAIR;
  m_wBankerTime=0;
  m_lBankerScore=0L;
  m_lBankerWinScore=0L;
  m_lTmpBankerWinScore=0;
  m_CarIndex = 0;
  m_lRobotMaxJetton = 5000000l;

  //���ֳɼ�
  m_lMeCurGameScore=0L;
  m_lMeCurGameReturnScore=0L;
  m_lBankerCurGameScore=0L;
  m_lGameRevenue=0L;

  //״̬��Ϣ
  m_lCurrentJetton=0L;
  m_cbAreaFlash=0xFF;
  m_wMeChairID=INVALID_CHAIR;
  m_bShowChangeBanker=FALSE;
  m_bNeedSetGameRecord=FALSE;

  m_bFlashResult=FALSE;
  m_blMoveFinish = FALSE;


  m_lMeCurGameScore=0L;
  m_lMeCurGameReturnScore=0L;
  m_lBankerCurGameScore=0L;
  m_blCanStore=false;

  m_lAreaLimitScore=0L;

  m_bAddScoreStatue=false;

  //λ����Ϣ
  m_nScoreHead = 0;
  m_nRecordFirst= 0;
  m_nRecordLast= 0;

  //��ʷ�ɼ�
  m_lMeStatisticScore=0;

  //�ؼ�����
  m_pGameClientDlg=CONTAINING_RECORD(this,CGameClientEngine,m_GameClientView);

  //����λͼ
  HINSTANCE hInstance=AfxGetInstanceHandle();

  m_imgGlodShaYu.LoadImage(hInstance,TEXT("GLODSHAYU"));
  m_pngTimeBack.LoadImage(hInstance,TEXT("TIME_BACK"));
  m_ImageViewFill.LoadFromResource(hInstance,IDB_VIEW_FILL);
  m_idb_selPng.LoadImage(hInstance,TEXT("IDB_IDB_SEL_PNG"));
  m_pngLuZiAnimal.LoadImage(hInstance,TEXT("LUZI_ANIMAL"));
  m_pngCoinNumber.LoadImage(hInstance,TEXT("COIN_NUMBER"));
  m_ImageTimeFlagPng.LoadImage(hInstance,TEXT("IDB_TIME_PNG"));
  m_ImageWinFlags.LoadImage(hInstance,TEXT("IDB_IDB_FLAG_PNG"));
  m_ViewBackPng.LoadImage(hInstance,TEXT("IDB_VIEW_BACK_PNG"));
  m_pngResultAnimal.LoadImage(hInstance,TEXT("RESULT_ANIMAL"));
  m_pngAnimationList.LoadImage(hInstance,TEXT("ANIMAL_LIST"));
  m_ImageScoreNumber.LoadFromResource(hInstance,IDB_SCORE_NUMBER);
  m_pngResultFShayu.LoadImage(hInstance,TEXT("RESULT_FRAME_SHAYU"));
  m_pngResultFOther.LoadImage(hInstance,TEXT("RESULT_FRAME_OTHER"));
  m_pngNumberASymbol.LoadImage(hInstance,TEXT("NUMBER_AND_SYMBOL"));
  m_ImageMeScoreNumber.LoadFromResource(hInstance,IDB_ME_SCORE_NUMBER);

  TCHAR szBuffer[128]=TEXT("");
  for(int i=0; i<3; i++)
  {
    myprintf(szBuffer,CountArray(szBuffer),TEXT("WinFrame%d"),i+1);
    m_ImgWinTexture[i].LoadImage(hInstance,szBuffer);
  }
  m_pngAnimalLight.LoadImage(hInstance,TEXT("ANIMAL_LIGHT"));
  m_pngTimeNumber.LoadImage(hInstance,TEXT("TIME_NUMBER"));
  m_ImgCurStorage.LoadImage(hInstance,TEXT("CURSTORAGE"));

  //-------------------------------------------------------------------
  memset(szBuffer,'/0',sizeof(szBuffer));
  for(int i = 0; i<3; i++)
  {
    myprintf(szBuffer,CountArray(szBuffer),TEXT("TEXTURE%d"),i+1);
    m_ImgTextture[i].LoadImage(hInstance,szBuffer);
  }

  m_ImgJettonNum.LoadImage(hInstance,TEXT("JETTON_NUM"));
  m_ImgAllJettonNum.LoadImage(hInstance,TEXT("ALLJETTON_NUM"));
  m_ImgJettonNum_B.LoadImage(hInstance,TEXT("JETTON_NUM_B"));
  m_ImgAllJettonNum_B.LoadImage(hInstance,TEXT("ALLJETTON_NUM_B"));

  memset(szBuffer,'/0',sizeof(szBuffer));
  for(int i = 0; i<AREA_COUNT - 1; i++)
  {
    myprintf(szBuffer,CountArray(szBuffer),TEXT("BT_ANIMAL_%d"),i+1);
    m_btAdd[i].LoadPng(this,hInstance,szBuffer);
    m_btAdd[i].SetBtnEnable(true);
    m_btAdd[i].SetBtnVisible(true);
  }
  //-------------------------------------------------------------------
  m_ImgPersonData_add.LoadImage(hInstance,TEXT("PersonData_ADD"));
  m_ImgPersonData_jian.LoadImage(hInstance,TEXT("PersonData_JIAN"));

  //��Ϸ��Ϣ
  for(int i = 0; i < CountArray(m_nAnimalPercent); ++i)
  {
    m_nAnimalPercent[i] = 0;
  }


  m_cbRandAddMulti=0;
  m_cbShaYuAddMulti=0;

  m_ShaYuIndex = 0;

  m_bShowShaYuResult = false;
  m_bShowAnotherResult = false;
  m_cbAnimalIndex=0;
  m_bShowCartoon=false;
  m_nPlayGameTimes=0;

  m_cbOneDigit = 0;
  m_cbTwoDigit = 0;
  m_cbRandOneDigit = 0;
  m_cbRandTwoDigit = 0;

  m_nAllRunTimes=1;
  m_lUserContinueAddJeffon = 0;

  m_lCurStorage = 0L;

  m_DrawGlodShaYu = false;

  m_bDrawYingZi =false;

  m_bIsContinueStatue = true;
  m_bGameStatus = 255;
  return;
}

//��������
CGameClientView::~CGameClientView(void)
{

  if(m_hInst)
  {
    FreeLibrary(m_hInst);
    m_hInst = NULL;
  }
}
//������Ϣ
int CGameClientView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
  if(__super::OnCreate(lpCreateStruct)==-1)
  {
    return -1;
  }


  AfxSetResourceHandle(AfxGetInstanceHandle());
  //�����ؼ�
  CRect rcCreate(0,0,0,0);
  m_GameRecord.Create(IDD_DLG_GAME_RECORD,this);

  m_GameRecord.ShowWindow(SW_HIDE);

  //��ע��ť
  m_btJetton100.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_JETTON_BUTTON_100);
  m_btJetton1000.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_JETTON_BUTTON_1000);
  m_btJetton10000.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_JETTON_BUTTON_10000);
  m_btJetton100000.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_JETTON_BUTTON_100000);
  m_btJetton1000000.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_JETTON_BUTTON_1000000);

  //������ɫ
  //m_btJetton100.SetButtonColor(RGB(0,255,0));
  //m_btJetton1000.SetButtonColor(RGB(0,255,0));
  //m_btJetton10000.SetButtonColor(RGB(0,255,0));
  //m_btJetton100000.SetButtonColor(RGB(0,255,0));
  //m_btJetton1000000.SetButtonColor(RGB(0,255,0));

  //��ť�ؼ�
  m_btScoreMoveL.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_SCORE_MOVE_L);
  m_btScoreMoveR.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_SCORE_MOVE_R);
  m_btBankStorage.Create(NULL,WS_CHILD|WS_DISABLED,rcCreate,this,IDC_BANK_STORAGE);
  m_btBankDraw.Create(NULL,WS_CHILD,rcCreate,this,IDC_BANK_DRAW);
  m_btBankDraw.EnableWindow(TRUE);
  m_btBankDraw.ShowWindow(TRUE);
  //���ð�ť
  HINSTANCE hResInstance=AfxGetInstanceHandle();
  m_btJetton100.SetButtonImage(IDB_BT_JETTON_100,hResInstance,false,false);
  m_btJetton1000.SetButtonImage(IDB_BT_JETTON_1000,hResInstance,false,false);
  m_btJetton10000.SetButtonImage(IDB_BT_JETTON_10000,hResInstance,false,false);
  m_btJetton100000.SetButtonImage(IDB_BT_JETTON_100000,hResInstance,false,true);
  m_btJetton1000000.SetButtonImage(IDB_BT_JETTON_1000000,hResInstance,false,true);

  m_btScoreMoveL.SetButtonImage(IDB_BT_SCORE_MOVE_L,hResInstance,false,false);
  m_btScoreMoveR.SetButtonImage(IDB_BT_SCORE_MOVE_R,hResInstance,false,false);
  m_btBankStorage.SetButtonImage(IDB_BT_STORAGE,hResInstance,false,false);
  m_btBankDraw.SetButtonImage(IDB_BT_DRAW,hResInstance,false,false);
  m_btOpenAdmin.Create(NULL,WS_CHILD|WS_VISIBLE,CRect(4,4,11,11),this,IDC_ADMIN);
  m_btOpenAdmin.ShowWindow(SW_HIDE);
  SwitchToCheck();

  //����
  m_hInst = NULL;
  m_pAdminControl = NULL;
  m_hInst = LoadLibrary(TEXT("AnimalBattleClientControl.dll"));
  if(m_hInst)
  {
    typedef void * (*CREATE)(CWnd* pParentWnd);
    CREATE ClientControl = (CREATE)GetProcAddress(m_hInst,"CreateClientControl");
    if(ClientControl)
    {
      m_pAdminControl = static_cast<IClientControlDlg*>(ClientControl(this));
      m_pAdminControl->ShowWindow(SW_HIDE);
    }
  }

  //m_btBankStorage.ShowWindow(SW_SHOW);
  //m_btBankDraw.ShowWindow(SW_SHOW);

  m_FontEx.CreateFont(this, TEXT("����"), 12, 400);
  //�����ť
  m_btCleanJeffon.LoadPng(this,hResInstance,TEXT("CLEAN_JEFFON"));
  m_btCleanJeffon.SetBtnEnable(true);
  m_btCleanJeffon.SetBtnVisible(true);

  //��ע��ť
  m_btContinuJeffon.LoadPng(this,hResInstance,TEXT("CONTINUE_JEFFON"));
  m_btContinuJeffon.SetBtnEnable(true);
  m_btContinuJeffon.SetBtnVisible(true);

  return 0;
}

//���ý���
VOID CGameClientView::ResetGameView()
{
  ////��ע��Ϣ
  //ZeroMemory(m_lUserJettonScore,sizeof(m_lUserJettonScore));
  //ZeroMemory(m_lUserLastJetton,sizeof(m_lUserLastJetton));
  ////ȫ����ע
  //ZeroMemory(m_lAllJettonScore,sizeof(m_lAllJettonScore));

  ////ׯ����Ϣ
  //m_wBankerUser=INVALID_CHAIR;
  //m_wBankerTime=0;
  //m_lBankerScore=0L;
  //m_lBankerWinScore=0L;
  //m_lTmpBankerWinScore=0;

  ////���ֳɼ�
  //m_lMeCurGameScore=0L;
  //m_lMeCurGameReturnScore=0L;
  //m_lBankerCurGameScore=0L;
  //m_lGameRevenue=0L;

  ////״̬��Ϣ
  //m_cbAreaFlash=0xFF;
  //m_wMeChairID=INVALID_CHAIR;
  //m_bShowChangeBanker=false;
  //m_bNeedSetGameRecord=false;
  //
  //m_bFlashResult=false;
  //m_bShowGameResult=false;
  //
  //m_lMeCurGameScore=0L;
  //m_lMeCurGameReturnScore=0L;
  //m_lBankerCurGameScore=0L;

  //m_lAreaLimitScore=0L;
  //
  //m_bAddScoreStatue=false;

  ////λ����Ϣ
  //m_nScoreHead = 0;
  //m_nRecordFirst= 0;
  //m_nRecordLast= 0;
  ////��ʷ�ɼ�
  //m_lMeStatisticScore=0;
  ////�������
  CleanUserJetton();
  m_pAdminControl->ClearClist();
  /*m_cbRandAddMulti=0;
  m_cbShaYuAddMulti=0;
  m_ShaYuIndex=0;

  m_bShowShaYuResult = false;
  m_DrawGlodShaYu = false;
  m_bShowAnotherResult = false;
  m_cbAnimalIndex=0;
  m_bShowCartoon=false;
  ZeroMemory(m_lUserAddScore,sizeof(m_lUserAddScore));
  m_cbOneDigit = 0;
  m_cbTwoDigit = 0;
  m_cbRandOneDigit = 0;
  m_cbRandTwoDigit = 0;
  m_lUserContinueAddJeffon = 0;

  m_lCurStorage = 0L;*/
  return;
}

//�����ؼ�
VOID CGameClientView::RectifyControl(INT nWidth, INT nHeight)
{
  //λ����Ϣ
  CSize Size;
  Size.cy = Size.cy/2;
  int iWidth =756;
  int iHeight =705;

  int LifeWidth = (nWidth-iWidth)/2;
  int TopHeight = (nHeight-iHeight)/2;

  m_TopHeight = TopHeight;
  m_LifeWidth = LifeWidth;

  int ix = nWidth/2+20;
  int iy = nHeight/2+6;
  int iAready = 52;
  int iAreadx =77;

  int iStartX = nWidth/2 - 480;
  int iStartY = nHeight/2 -142;

  int xLength=25;
  int yLength=71;
  int xOffset = 28;
  int yOffset = 17;

  //��һ��
  int Offset1[7][2] = {{212,181},{284,199},{359,208},{434,215},{510,208},{585,200},{656,183}};
  //�ڶ���
  int Offset2[7][2] = {{728,159},{793,124},{845,69},{867,-1},{844,-72},{792,-125},{727,-160}};
  //������
  int Offset3[7][2] = {{656,-184},{584,-201},{509,-210},{434,-216},{358,-210},{284,-201},{213,-184}};
  //���Ľ�
  int Offset4[7][2] = {{141,-160},{76,-124},{24,-71},{0,0},{25,71},{77,124},{141,160}};
  for(int i = 0; i<28; i++)
  {
    if(i < 7)
    {
      m_CarRect[i].left = iStartX + Offset1[i][0];
      m_CarRect[i].top  = iStartY - Offset1[i][1];
    }
    else if(i >6 && i< 14)
    {
      m_CarRect[i].left = iStartX + Offset2[i - 7][0];
      m_CarRect[i].top  = iStartY - Offset2[i - 7][1];
    }
    else if(i>13 && i<21)
    {
      m_CarRect[i].left = iStartX + Offset3[i - 14][0];
      m_CarRect[i].top  = iStartY - Offset3[i - 14][1];
    }
    else
    {
      m_CarRect[i].left = iStartX + Offset4[i - 21][0];
      m_CarRect[i].top  = iStartY - Offset4[i - 21][1];
    }
  }


  m_RectArea[0].SetRect(ix-279,iy-138,ix-151,iy);
  m_RectArea[1].SetRect(ix-151,iy-138,ix-21 ,iy);
  m_RectArea[2].SetRect(ix-21 ,iy-138,ix+109,iy);
  m_RectArea[3].SetRect(ix+109,iy-138,ix+239,iy);

  m_RectArea[4].SetRect(ix-279,iy,ix-151,iy+138);
  m_RectArea[5].SetRect(ix-151,iy,ix-21 ,iy+138);
  m_RectArea[6].SetRect(ix-21 ,iy,ix+109,iy+138);
  m_RectArea[7].SetRect(ix+109,iy,ix+239,iy+138);


  int JettonBeginX=m_LifeWidth+100;
  int JettonBeginY=TopHeight+197;
  int xJettonLength = 58;
  for(int i = 0; i<AREA_COUNT; i++)
  {
    m_PointAnimalRand[i].SetPoint(JettonBeginX+i*xJettonLength, JettonBeginY);
    m_PointJetton[i].SetPoint(m_RectArea[i].left, m_RectArea[i].top);
  }

  int nYAddPos =TopHeight+613-24+13;
  int nXAddPos =LifeWidth+74;

  int addSpaceAdd=m_btAdd[0].GetWidth() + 5;
  int addScoreWidth=30;


  int offset = m_btAdd[0].GetHeight() -m_ImgTextture[0].GetHeight() - 16;

  for(int i = 0; i < AREA_COUNT; ++i)
  {
    if(i<4)
    {
      m_PointAddButton[i].SetPoint(nXAddPos + (addSpaceAdd * i) -172,nYAddPos-58);
      m_PointJettonScore[i].SetPoint(m_PointAddButton[i].x+16,m_PointAddButton[i].y+offset +22);
      m_TextTurePoint[i].SetPoint(m_PointAddButton[i].x+6,m_PointAddButton[i].y+offset);
      m_WinPosition[i].SetPoint(nXAddPos + (addSpaceAdd * i) -182,nYAddPos -68);
    }
    else if(i == 4)
    {
      m_PointAddButton[i].SetPoint(nXAddPos + (addSpaceAdd * i) -149,nYAddPos-58);
      m_PointJettonScore[i].SetPoint(m_PointAddButton[i].x+33,m_PointAddButton[i].y+offset +22);
      m_TextTurePoint[i].SetPoint(m_PointAddButton[i].x+6,m_PointAddButton[i].y+offset);
      m_WinPosition[i].SetPoint(nXAddPos + (addSpaceAdd * i) -159,nYAddPos-68);

    }
    else if(i >4 && i<9)
    {
      m_PointAddButton[i].SetPoint(nXAddPos + (addSpaceAdd * i)-25 ,nYAddPos-58);
      m_PointJettonScore[i].SetPoint(m_PointAddButton[i].x+16,m_PointAddButton[i].y+offset+22);
      m_TextTurePoint[i].SetPoint(m_PointAddButton[i].x+6,m_PointAddButton[i].y+offset);
      m_WinPosition[i].SetPoint(nXAddPos + (addSpaceAdd * i)-35 ,nYAddPos-68);

    }
    else if(i == 9)
    {
      m_PointAddButton[i].SetPoint(nXAddPos -170,nYAddPos - 126);
      m_PointJettonScore[i].SetPoint(m_PointAddButton[i].x+36,m_PointAddButton[i].y+offset - 26);
      m_TextTurePoint[i].SetPoint(m_PointAddButton[i].x+9,m_PointAddButton[i].y+offset - 61);
      m_WinPosition[i].SetPoint(nXAddPos -180,nYAddPos - 136);

    }
    else if(i == 10)
    {
      m_PointAddButton[i].SetPoint(nXAddPos + (addSpaceAdd * i) - 366 ,nYAddPos - 126);
      m_PointJettonScore[i].SetPoint(m_PointAddButton[i].x+ 36,m_PointAddButton[i].y+offset -26);
      m_TextTurePoint[i].SetPoint(m_PointAddButton[i].x+9,m_PointAddButton[i].y+offset - 61);
      m_WinPosition[i].SetPoint(nXAddPos + (addSpaceAdd * i) - 376 ,nYAddPos - 136);
    }

  }

  nXAddPos+=274;
  nYAddPos-=66;

  m_UserScoreTall.SetPoint(m_PointAddButton[0].x +100,m_PointAddButton[0].y + 125);

  //�ƶ��ؼ�
  HDWP hDwp=BeginDeferWindowPos(33);
  const UINT uFlags=SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOCOPYBITS;

  m_MeInfoRect.top = TopHeight+22;
  m_MeInfoRect.left = LifeWidth+265+389;

  m_btBankStorage.EnableWindow(TRUE);

  //���밴ť
  CRect rcJetton;
  m_btJetton100.GetWindowRect(&rcJetton);
  int nYPos =TopHeight+613-41;
  int nXPos =LifeWidth+197+103;
  int nSpace=8;
  m_nWinFlagsExcursionX = LifeWidth+220;
  m_nWinFlagsExcursionY = TopHeight+593;
  nYPos -=46;
  DeferWindowPos(hDwp,m_btJetton100,NULL,nXPos -113,nYPos +130,0,0,uFlags|SWP_NOSIZE);
  DeferWindowPos(hDwp,m_btJetton1000,NULL,nXPos    + rcJetton.Width() * 1 -113, nYPos + 130, 0,0,uFlags|SWP_NOSIZE);
  DeferWindowPos(hDwp,m_btJetton10000,NULL,nXPos   + rcJetton.Width() * 2 -113, nYPos + 130, 0,0,uFlags|SWP_NOSIZE);
  DeferWindowPos(hDwp,m_btJetton100000,NULL,nXPos  + rcJetton.Width() * 3 -113, nYPos + 130, 0,0,uFlags|SWP_NOSIZE);
  DeferWindowPos(hDwp,m_btJetton1000000,NULL,nXPos + rcJetton.Width() * 4 -113, nYPos + 130, 0,0,uFlags|SWP_NOSIZE);

  m_LuZiStart.SetPoint(m_LifeWidth+130,TopHeight+223);
  DeferWindowPos(hDwp,m_btScoreMoveL,NULL,m_LuZiStart.x - 53,m_LuZiStart.y+108,0,0,uFlags|SWP_NOSIZE);
  DeferWindowPos(hDwp,m_btScoreMoveR,NULL,m_LuZiStart.x+539,m_LuZiStart.y+108,0,0,uFlags|SWP_NOSIZE);
  //������ť
  DeferWindowPos(hDwp,m_btBankStorage,NULL,nWidth/2+399 ,nHeight/2+93-5,0,0,uFlags|SWP_NOSIZE);
  DeferWindowPos(hDwp,m_btBankDraw,NULL,nWidth/2+399,nHeight/2 +63-5,0,0,uFlags|SWP_NOSIZE);
  //�����ƶ�
  EndDeferWindowPos(hDwp);
  return;
}
void CGameClientView::SwitchToCheck()
{
  SwithToNormalView();
  return;

}
void CGameClientView::SwithToNormalView()
{
  m_btScoreMoveL.ShowWindow(SW_SHOW);
  m_btScoreMoveR.ShowWindow(SW_SHOW);
  m_btJetton100.ShowWindow(SW_SHOW);
  m_btJetton1000.ShowWindow(SW_SHOW);
  m_btJetton10000.ShowWindow(SW_SHOW);
  m_btJetton100000.ShowWindow(SW_SHOW);
  m_btJetton1000000.ShowWindow(SW_SHOW);

  m_DrawBack = true;
}

void CGameClientView::DrawPngNumberKind(int nPngKindID,LONGLONG lCurStorage,CDC *pDC,INT nWidth, INT nHeight)
{
  switch(nPngKindID)
  {
      //
    case IDB_IMG_CURSTORAGE:
    {
      if(m_ImgCurStorage.IsNull())
      {
        return;
      }
      if(lCurStorage<0)
      {
        lCurStorage = 0;
      }
      //������Դ
      CSize SizeScoreNumber(m_ImgCurStorage.GetWidth()/11,m_ImgCurStorage.GetHeight());


      //λ�ö���
      INT nOffset=8;
      INT nYDrawPos=nHeight-SizeScoreNumber.cy/2;
      INT nXDrawPos=(INT)(nWidth) ;

      //������Ŀ
      LONGLONG lNumberCount=0;
      LONGLONG lNumberTemp=lCurStorage;
      do
      {
        lNumberCount++;
        lNumberTemp/=10;
      }
      while(lNumberTemp>0);

      int nNumspace = 0;

      //�滭����
      for(LONGLONG i=0; i<lNumberCount; i++)
      {
        LONG lCellNumber=(LONG)(lCurStorage%10);
        nNumspace = 77 + ((SizeScoreNumber.cx  + 5)* (lNumberCount - i)) + nXDrawPos -170;
        m_ImgCurStorage.DrawImage(pDC,
                                  nNumspace,
                                  nYDrawPos + 3,
                                  SizeScoreNumber.cx,
                                  SizeScoreNumber.cy ,
                                  (lCellNumber)*SizeScoreNumber.cx,
                                  0);

        //���ñ���
        lCurStorage/=10;
      };

      return;
    }

    //������
    case INVALID_CHAIR:
    {
      return;
    }
  }

  return ;
}
//�滭����
VOID CGameClientView::DrawGameView(CDC * pDC, INT nWidth, INT nHeight)
{
  //��ȡ���
  IClientUserItem * pClientUserItem=m_wBankerUser==INVALID_CHAIR ? NULL :GetClientUserItem(m_wBankerUser);

  //�滭����
  DrawViewImage(pDC,m_ImageViewFill,DRAW_MODE_SPREAD);
  m_ViewBackPng.DrawImage(pDC,(nWidth-m_ViewBackPng.GetWidth())/2,nHeight/2-m_ViewBackPng.GetHeight()/2,\
                          m_ViewBackPng.GetWidth() ,m_ViewBackPng.GetHeight(),0,0,m_ViewBackPng.GetWidth() ,m_ViewBackPng.GetHeight());

  m_pngAnimationList.DrawImage(pDC,(nWidth-m_ViewBackPng.GetWidth())/2,nHeight/2-m_ViewBackPng.GetHeight()/2);
  if(m_blRungingCar)
  {
    //����ת��
    //m_idb_selPng.DrawImage(pDC ,m_CarRect[m_CarIndex].left,m_CarRect[m_CarIndex].top);

    //��ʾ����
    //int AnimalIndex=GetAnimalInfo(m_CarIndex+1);

    int selPngWidth = m_idb_selPng.GetWidth()/5;
    int setPngHeight = m_idb_selPng.GetHeight();

    m_idb_selPng.DrawImage(pDC ,m_CarRect[m_CarIndex].left,
                           m_CarRect[m_CarIndex].top,
                           selPngWidth ,
                           setPngHeight,
                           0*selPngWidth,
                           0,
                           selPngWidth ,
                           setPngHeight);

    if(m_bDrawYingZi == true)
    {
      int TempCardIndex1 = (((m_CarIndex -1)+ANIMAL_COUNT)%ANIMAL_COUNT);
      m_idb_selPng.DrawImage(pDC ,m_CarRect[TempCardIndex1].left,
                             m_CarRect[TempCardIndex1].top,
                             selPngWidth ,
                             setPngHeight,
                             1*selPngWidth,
                             0,
                             selPngWidth ,
                             setPngHeight);


      int TempCardIndex2 = (((m_CarIndex -2)+ANIMAL_COUNT)%ANIMAL_COUNT);
      m_idb_selPng.DrawImage(pDC ,m_CarRect[TempCardIndex2].left,
                             m_CarRect[TempCardIndex2].top,
                             selPngWidth ,
                             setPngHeight,
                             2*selPngWidth,
                             0,
                             selPngWidth ,
                             setPngHeight);

      int TempCardIndex3 = (((m_CarIndex -3)+ANIMAL_COUNT)%ANIMAL_COUNT);
      m_idb_selPng.DrawImage(pDC ,m_CarRect[TempCardIndex3].left,
                             m_CarRect[TempCardIndex3].top,
                             selPngWidth ,
                             setPngHeight,
                             3*selPngWidth,
                             0,
                             selPngWidth ,
                             setPngHeight);

      int TempCardIndex4 = (((m_CarIndex -4)+ANIMAL_COUNT)%ANIMAL_COUNT);
      m_idb_selPng.DrawImage(pDC ,m_CarRect[TempCardIndex4].left,
                             m_CarRect[TempCardIndex4].top,
                             selPngWidth ,
                             setPngHeight,
                             4*selPngWidth,
                             0,
                             selPngWidth ,
                             setPngHeight);
    }

  }


  if(m_bFlashrandShow)
  {
  }

  //��ʾ��ͨ
  if(m_bShowCartoon)
  {
    int nAnimalIndex=GetCartoonIndex(m_CarIndex +1);
    int XLength = m_pngAnimalLight.GetWidth()/12;
    int YHeight = m_pngAnimalLight.GetHeight();
    m_pngAnimalLight.DrawImage(pDC, nWidth/2-106,nHeight/2-180,XLength,YHeight,(nAnimalIndex%12)*XLength,0,XLength,YHeight);
  }

  int nOffset = 0;



  if(m_bGameStatus != 255)
  {
    //��ȡ״̬
    BYTE cbGameStatus=m_pGameClientDlg->GetGameStatus();
    //ʱ����ʾ
    if(m_DrawBack)
    {
      int nTimeFlagWidth = m_ImageTimeFlagPng.GetWidth()/3;
      int nFlagIndex=0;

      //����״̬
      if(cbGameStatus==GAME_STATUS_FREE)
      {
        for(int i = 0; i< AREA_COUNT -1; i++)
        {
          m_btAdd[i].SetBtnEnable(true);
        }

        m_btCleanJeffon.SetBtnEnable(false);
        m_btContinuJeffon.SetBtnEnable(false);

        nOffset = 0;
        nFlagIndex=0;
        m_lCurrentJetton = 0L;
      }
      //��ע״̬
      else if(cbGameStatus==GS_PLACE_JETTON)
      {
        for(int i = 0; i< AREA_COUNT -1; i++)
        {
          m_btAdd[i].SetBtnEnable(true);
        }

        m_btCleanJeffon.SetBtnEnable(true);
        m_btContinuJeffon.SetBtnEnable(true);
        nOffset = 0;
        nFlagIndex=1;

        //�Ͼ���ע
      }
      //����״̬
      else if(cbGameStatus==GS_GAME_END)
      {
        nOffset = 2;
        m_lCurrentJetton = 0L;
        nFlagIndex=2;

        for(int i = 0; i< AREA_COUNT -1; i++)
        {
          m_btAdd[i].SetBtnEnable(false);
        }

        m_btCleanJeffon.SetBtnEnable(false);
        m_btContinuJeffon.SetBtnEnable(false);
      }

      int xBegin = nWidth/2-70-81;
      int yBegin = (nHeight-705)/2+160+397;
      m_ImageTimeFlagPng.DrawImage(pDC,xBegin + 85, yBegin-36, nTimeFlagWidth, m_ImageTimeFlagPng.GetHeight(),
                                   nFlagIndex*nTimeFlagWidth,0);
      WORD wUserTimer =0;
      if(m_wMeChairID!=INVALID_CHAIR)
      {
        wUserTimer = GetUserClock(m_wMeChairID);
      }
      if(wUserTimer>=0)
      {
        //m_pngTimeBack.DrawImage(pDC,xBegin+7,yBegin-55);
        DrawNumberString(pDC,wUserTimer,xBegin+175,yBegin-26,true);
      }
    }



  }
  CRect rcDispatchCardTips(m_LifeWidth+612, m_TopHeight+297,m_LifeWidth+812, m_TopHeight+412);

  ////ʤ����־
  DrawWinFlags(pDC);

  ////ʤ���߿�
  FlashJettonAreaFrame(nWidth,nHeight,pDC);

  //�滭ׯ��
  if(m_DrawBack)
  {
    //�滭�û�
    DrawMeInfo(pDC,nWidth,nHeight);
  }

  if(CD2DEngine::GetD2DEngine()!=NULL)
  {
    //����ť
    for(int i = 0; i < AREA_COUNT -1; ++i)
    {
      if(i<4)
      {
        m_btAdd[i].Draw(pDC,m_PointAddButton[i].x,m_PointAddButton[i].y);
        m_ImgTextture[0].DrawImage(pDC,m_TextTurePoint[i].x,m_TextTurePoint[i].y + nOffset);
      }
      else if(i == 4)
      {
        m_btAdd[i].Draw(pDC,m_PointAddButton[i].x,m_PointAddButton[i].y);
        m_ImgTextture[1].DrawImage(pDC,m_TextTurePoint[i].x,m_TextTurePoint[i].y + nOffset);
      }
      else if(i >4 && i<9)
      {
        m_btAdd[i].Draw(pDC,m_PointAddButton[i].x,m_PointAddButton[i].y);
        m_ImgTextture[0].DrawImage(pDC,m_TextTurePoint[i].x,m_TextTurePoint[i].y + nOffset);
      }
      else if(i == 9)
      {
        m_btAdd[i].Draw(pDC,m_PointAddButton[i].x,m_PointAddButton[i].y);
        m_ImgTextture[2].DrawImage(pDC,m_TextTurePoint[i].x,m_TextTurePoint[i].y + nOffset);
      }
      else if(i == 10)
      {
        m_btAdd[i].Draw(pDC,m_PointAddButton[i].x,m_PointAddButton[i].y);
        m_ImgTextture[2].DrawImage(pDC,m_TextTurePoint[i].x,m_TextTurePoint[i].y + nOffset);
      }
    }
  }
  else
  {
    //����ť
    for(int i = 0; i < AREA_COUNT -1; ++i)
    {
      if(i<4)
      {
        m_btAdd[i].Draw(pDC,m_PointAddButton[i].x,m_PointAddButton[i].y);
        m_ImgTextture[0].DrawImage(pDC,m_TextTurePoint[i].x,m_TextTurePoint[i].y + nOffset);
      }
      else if(i == 4)
      {
        m_btAdd[i].Draw(pDC,m_PointAddButton[i].x,m_PointAddButton[i].y);
        m_ImgTextture[1].DrawImage(pDC,m_TextTurePoint[i].x,m_TextTurePoint[i].y + nOffset);
      }
      else if(i >4 && i<9)
      {
        m_btAdd[i].Draw(pDC,m_PointAddButton[i].x,m_PointAddButton[i].y);
        m_ImgTextture[0].DrawImage(pDC,m_TextTurePoint[i].x,m_TextTurePoint[i].y + nOffset);
      }
      else if(i == 9)
      {
        m_btAdd[i].Draw(pDC,m_PointAddButton[i].x,m_PointAddButton[i].y);
        m_ImgTextture[2].DrawImage(pDC,m_TextTurePoint[i].x,m_TextTurePoint[i].y + nOffset);
      }
      else if(i == 10)
      {
        m_btAdd[i].Draw(pDC,m_PointAddButton[i].x,m_PointAddButton[i].y);
        m_ImgTextture[2].DrawImage(pDC,m_TextTurePoint[i].x,m_TextTurePoint[i].y + nOffset);
      }
    }
  }


  //�滭��ע
  for(int i = 0; i < AREA_COUNT ; ++i)
  {
    int j=i;

    if(i==ID_YAN_ZI-1)
    {
      j=0;
    }
    else if(i==ID_GE_ZI-1)
    {
      j=1;
    }
    else if(i==ID_KONG_QUE-1)
    {
      j=2;
    }
    else if(i==ID_LAO_YING-1)
    {
      j=3;
    }

    else if(i==ID_SHA_YU-1)
    {
      j=4;
    }

    else if(i==ID_SHI_ZI-1)
    {
      j=5;
    }
    else if(i==ID_XIONG_MAO-1)
    {
      j=6;
    }
    else if(i==ID_HOU_ZI-1)
    {
      j=7;
    }
    else if(i==ID_TU_ZI-1)
    {
      j=8;
    }

    else if(i==ID_FEI_QIN-1)
    {
      j=9;
    }

    else if(i==ID_ZOU_SHOU-1)
    {
      j=10;
    }

    if(m_lUserJettonScore[i+1]!=0)
    {
      if(j != 9 && j!= 10)
      {
        DrawJettonData(pDC,m_lUserJettonScore[i+1],m_PointJettonScore[j].x ,m_PointJettonScore[j].y-5 - 12);
      }
      else
      {
        DrawJettonData_B(pDC,m_lUserJettonScore[i+1],m_PointJettonScore[j].x ,m_PointJettonScore[j].y-5 - 12);
      }
    }

    if(m_lAllJettonScore[i+1]!=0)
    {
      if(j != 9 && j!= 10)
      {
        DrawAllJettonData(pDC,m_lAllJettonScore[i+1],m_PointJettonScore[j].x,m_PointJettonScore[j].y+7 - 12);
      }
      else
      {
        DrawAllJettonData_B(pDC,m_lAllJettonScore[i+1],m_PointJettonScore[j].x,m_PointJettonScore[j].y+7 - 12);
      }
    }
  }
  LONGLONG curRand=rand()%9999999;
  DrawPngNumberKind(IDB_IMG_CURSTORAGE,m_lCurStorage,pDC,nWidth/2,nHeight/2 - 211);

  //�滭���
  if(m_bShowShaYuResult)
  {
    DrawShaYuResult(pDC, nWidth /2 -480, nHeight/2 -245);
    if(m_bShowAnotherResult)
    {
      DrawAnotherResult(pDC,nWidth,nHeight);
    }
  }
  else
  {
    if(m_DrawGlodShaYu == true)
    {
      m_imgGlodShaYu.DrawImage(pDC,nWidth /2 -250, nHeight/2 -255);
      DrawAnotherResult(pDC, nWidth, nHeight);
    }
    else
    {
      DrawAnotherResult(pDC, nWidth, nHeight);
    }
  }

  if(CD2DEngine::GetD2DEngine()!=NULL)
  {
    m_btCleanJeffon.Draw(pDC,nWidth/2 - 480,nHeight/2 + 90-5);
    m_btContinuJeffon.Draw(pDC,nWidth/2 - 480,nHeight/2 + 90 - m_btCleanJeffon.GetHeight()-5);
  }
  else
  {
    m_btCleanJeffon.Draw(pDC,nWidth/2 - 480,nHeight/2 + 90-5);
    m_btContinuJeffon.Draw(pDC,nWidth/2 - 480,nHeight/2 + 90 - m_btCleanJeffon.GetHeight()-5);
  }


  return;
}

//��ȡ����
BYTE CGameClientView::GetAnimalInfo(BYTE cbAllIndex)
{
  //static BYTE AnimalInfo[ANIMAL_COUNT]={4,4,4,1,8,8,8,7,7,7,1,6,6,6,5,5,5,1,3,3,3,2,2,2,1,0,0,0};
  static BYTE AnimalInfo[ANIMAL_COUNT]= {7,7,7,11,0,0,0, 1,1,1,10,2,2,2, 3,3,3,8,4,4,4,5,5,5,9,6,6,6};

  if(cbAllIndex>=1&&cbAllIndex<=28)
  {
    return AnimalInfo[cbAllIndex-1];
  }

  return 0;
}


//��ȡ����
BYTE CGameClientView::GetCartoonIndex(BYTE cbAllIndex)
{
  static BYTE CartoonIndex[ANIMAL_COUNT]= {7,7,7,11,0,0,0, 1,1,1,10,2,2,2, 3,3,3,8,4,4,4,5,5,5,9,6,6,6};
  //static BYTE CartoonIndex[ANIMAL_COUNT]={0,0,0,8,1,1,1,2,2,2,8,3,3,3,4,4,4,8,5,5,5,6,6,6,8,7,7,7};

  if(cbAllIndex>=1&&cbAllIndex<=28)
  {
    return CartoonIndex[cbAllIndex-1];
  }

  return 0;
}

//��ȡ����
BYTE CGameClientView::GetAnimalMulti(BYTE cbAllIndex)
{
  //1 ��ͨ��  ; 2 ��ͨɱ
  static BYTE AnimalMulti[ANIMAL_COUNT]= {6,6,6,100,6,6,6,8,8,8,0,8,8,8,12,12,12,24,12,12,12,8,8,8,0,8,8,8};

  if(cbAllIndex>=1&&cbAllIndex<=28)
  {
    return AnimalMulti[cbAllIndex-1];
  }

  return 0;
}
//������Ϣ
void CGameClientView::SetMeMaxScore(LONGLONG lMeMaxScore)
{
  if(m_lMeMaxScore!=lMeMaxScore)
  {
    //���ñ���
    m_lMeMaxScore=lMeMaxScore;
  }

//#ifdef _DEBUG
//  m_lMeMaxScore = 333330;
//#endif

  return;
}

//�����ע
void CGameClientView::SetAreaLimitScore(LONGLONG lAreaLimitScore)
{
  if(m_lAreaLimitScore!= lAreaLimitScore)
  {
    //���ñ���
    m_lAreaLimitScore=lAreaLimitScore;
  }
}
//��ʷ��¼
void CGameClientView::SetGameHistory(BYTE *bcResulte)
{
  //��������
  BYTE bcResulteTmp[AREA_COUNT];
  memcpy(bcResulteTmp,bcResulte,AREA_COUNT);
  tagClientGameRecord &GameRecord = m_GameRecordArrary[m_nRecordLast];


  if(bcResulteTmp[11] == 100)
  {
    for(int i = 1; i<=AREA_COUNT; i++)
    {
      if(bcResulteTmp[i-1] == 100)
      {
        GameRecord.enOperateMen[i]=enOperateResult_Win;

      }
      else
      {
        GameRecord.enOperateMen[i]=enOperateResult_Lost;

      }
    }
  }
  else if(bcResulteTmp[10] == 255)
  {
    for(int i = 1; i<=AREA_COUNT; i++)
    {
      if(bcResulteTmp[i-1] == 255)
      {
        GameRecord.enOperateMen[i]=enOperateResult_Win;

      }
      else
      {
        GameRecord.enOperateMen[i]=enOperateResult_Lost;

      }
    }
  }
  else if(bcResulteTmp[9] == 1)
  {
    for(int i = 1; i<=AREA_COUNT; i++)
    {
      if(bcResulteTmp[i-1] == 1)
      {
        GameRecord.enOperateMen[i]=enOperateResult_Win;

      }
      else
      {
        GameRecord.enOperateMen[i]=enOperateResult_Lost;

      }
    }
  }
  else
  {
    for(int i = 1; i<=AREA_COUNT; i++)
    {
      if(bcResulteTmp[i-1]>2)
      {
        GameRecord.enOperateMen[i]=enOperateResult_Win;

      }
      else
      {
        GameRecord.enOperateMen[i]=enOperateResult_Lost;

      }
    }
  }
  //�ƶ��±�
  m_nRecordLast = (m_nRecordLast+1) % MAX_SCORE_HISTORY;
  if(m_nRecordLast == m_nRecordFirst)
  {
    m_nRecordFirst = (m_nRecordFirst+1) % MAX_SCORE_HISTORY;
    if(m_nScoreHead < m_nRecordFirst)
    {
      m_nScoreHead = m_nRecordFirst;
    }
  }

  int nHistoryCount = (m_nRecordLast - m_nRecordFirst + MAX_SCORE_HISTORY) % MAX_SCORE_HISTORY;
  if(MAX_FALG_COUNT < nHistoryCount)
  {
    m_btScoreMoveR.EnableWindow(TRUE);
  }

  //�Ƶ����¼�¼
  if(MAX_FALG_COUNT < nHistoryCount)
  {
    m_nScoreHead = (m_nRecordLast - MAX_FALG_COUNT + MAX_SCORE_HISTORY) % MAX_SCORE_HISTORY;
    m_btScoreMoveL.EnableWindow(TRUE);
    m_btScoreMoveR.EnableWindow(FALSE);
  }

  return;
}

//�������
void CGameClientView::CleanUserJetton()
{

  //��ע��Ϣ
  ZeroMemory(m_lUserJettonScore,sizeof(m_lUserJettonScore));

  //ȫ����ע
  ZeroMemory(m_lAllJettonScore,sizeof(m_lAllJettonScore));

  m_bShowShaYuResult = false;
  m_DrawGlodShaYu = false;
  m_bShowAnotherResult = false;
  m_bShowCartoon = false;

  m_cbShaYuAddMulti = 0;

  KillTimer(IDI_SHOW_ANIMAL);

  KillTimer(IDI_FLASH_NUMBER1);
  KillTimer(IDI_END_FLASH_NUMBER1);

  KillTimer(IDI_FLASH_NUMBER2);
  KillTimer(IDI_END_FLASH_NUMBER2);

  m_ShaYuIndex=0;

  ZeroMemory(m_lUserAddScore,sizeof(m_lUserAddScore));

  ZeroMemory(m_cbTableCardArray,sizeof(m_cbTableCardArray));

  m_cbOneDigit = 0;
  m_cbTwoDigit = 0;
  m_cbRandOneDigit = 0;
  m_cbRandTwoDigit = 0;

  if(NULL != m_pAdminControl && NULL != m_pAdminControl->GetSafeHwnd())
  {
    m_pAdminControl->m_UserBetArray.RemoveAll();
    m_pAdminControl->UpdateUserBet(true);
  }
  //���½���
  RefreshGameView();

  return;
}

//������ע
void CGameClientView::SetMePlaceJetton(BYTE cbViewIndex, LONGLONG lJettonCount)
{
  //Ч�����
  ASSERT(cbViewIndex<=AREA_COUNT);
  if(cbViewIndex>AREA_COUNT)
  {
    return;
  }

  m_lUserJettonScore[cbViewIndex]+=lJettonCount;

  //���½���
  RefreshGameView();
}

//�����˿�
void CGameClientView::SetCardInfo(BYTE cbTableCardArray[2])
{
  if(cbTableCardArray!=NULL)
  {
    CopyMemory(m_cbTableCardArray,cbTableCardArray,sizeof(m_cbTableCardArray));
    // m_cbTableCardArray[0]=25;
    //��ʼ����
    DispatchCard();
  }
  else
  {
    ZeroMemory(m_cbTableCardArray,sizeof(m_cbTableCardArray));
  }
}

//���ó���
void CGameClientView::PlaceUserJetton(BYTE cbViewIndex, LONGLONG lScoreCount)
{
  //Ч�����
  ASSERT(cbViewIndex<=AREA_COUNT);
  if(cbViewIndex>AREA_COUNT)
  {
    return;
  }

  m_lAllJettonScore[cbViewIndex] += lScoreCount;

  RefreshGameView();

  return;
}

//��������ע
void CGameClientView::AndroidBet(BYTE cbViewIndex, LONGLONG lScoreCount)
{
  //Ч�����
  ASSERT(cbViewIndex<=AREA_COUNT);
  if(cbViewIndex>AREA_COUNT)
  {
    return;
  }

  if(lScoreCount <= 0L)
  {
    return;
  }

  tagAndroidBet Androi;
  Androi.cbJettonArea = cbViewIndex;
  Androi.lJettonScore = lScoreCount;
  m_ArrayAndroid.Add(Androi);
  SetTimer(IDI_ANDROID_BET,100,NULL);
  int nHaveCount = 0;
  for(int i = 0 ; i < m_ArrayAndroid.GetCount(); ++i)
  {
    if(m_ArrayAndroid[i].lJettonScore > 0)
    {
      nHaveCount++;
    }
  }
  UINT nElapse = 0;
  if(nHaveCount <= 1)
  {
    nElapse = 260;
  }
  else if(nHaveCount <= 2)
  {
    nElapse = 160;
  }
  else
  {
    nElapse = 100;
  }

  SetTimer(IDI_ANDROID_BET+m_ArrayAndroid.GetCount(),nElapse,NULL);
}

//���ֳɼ�
void CGameClientView::SetCurGameScore(LONGLONG lMeCurGameScore, LONGLONG lMeCurGameReturnScore, LONGLONG lBankerCurGameScore, LONGLONG lGameRevenue)
{
  m_lMeCurGameScore=lMeCurGameScore;
  m_lMeCurGameReturnScore=lMeCurGameReturnScore;
  m_lBankerCurGameScore=lBankerCurGameScore;
  m_lGameRevenue=lGameRevenue;
}

//����ʤ��
void CGameClientView::SetWinnerSide(bool blWin[], bool bSet)
{
  //����ʱ��
  for(int i= 0; i<AREA_COUNT; i++)
  {

    m_bWinFlag[i]=blWin[i];
  }
  if(true==bSet)
  {
    //���ö�ʱ��
    SetTimer(IDI_FLASH_WINNER,500,NULL);

    //ȫʤ�ж�
    bool blWinAll = true;

    for(int i= 0; i<AREA_COUNT; i++)
    {

      if(m_bWinFlag[i]==true)
      {
        blWinAll = false;
      }
    }

  }
  else
  {
    //�����ʱ��
    KillTimer(IDI_FLASH_WINNER);

    //ȫʤ�ж�
    bool blWinAll = true;

    for(int i= 0; i<AREA_COUNT; i++)
    {

      if(m_bWinFlag[i]==true)
      {
        blWinAll = false;
      }
    }
    if(blWinAll)
    {
    }
  }

  //���ñ���
  m_bFlashResult=bSet;
  m_bShowGameResult=bSet;
  m_cbAreaFlash=0xFF;

  //���½���
  RefreshGameView();

  return;
}

//��ȡ����
BYTE CGameClientView::GetJettonArea(CPoint MousePoint)
{
  for(int i = 0; i<AREA_COUNT; i++)
  {
    if(m_RectArea[i].PtInRect(MousePoint))
    {
      return i+1;
    }
  }
  return 0xFF;
}
//�滭����
void CGameClientView::DrawNumberString(CDC * pDC, LONGLONG lNumber, INT nXPos, INT nYPos, bool blTimer,bool bMeScore)
{
  //������Դ
  CSize SizeScoreNumber(m_ImageScoreNumber.GetWidth()/11,m_ImageScoreNumber.GetHeight());

  if(bMeScore)
  {
    SizeScoreNumber.SetSize(m_ImageMeScoreNumber.GetWidth()/11,m_ImageMeScoreNumber.GetHeight());
  }

  if(blTimer)
  {
    SizeScoreNumber.SetSize(m_pngTimeNumber.GetWidth()/10, m_pngTimeNumber.GetHeight());

  }

  //������Ŀ
  LONGLONG lNumberCount=0;
  LONGLONG lNumberTemp=lNumber;
  do
  {
    lNumberCount++;
    lNumberTemp/=10;
  }
  while(lNumberTemp>0);

  //λ�ö���
  INT nOffset=8;
  INT nYDrawPos=nYPos-SizeScoreNumber.cy/2;
  INT nXDrawPos=(INT)(nXPos+lNumberCount*SizeScoreNumber.cx/2+lNumberCount/4*(SizeScoreNumber.cx-nOffset)/2-SizeScoreNumber.cx);


  //�滭����
  for(LONGLONG i=0; i<lNumberCount; i++)
  {
    //�滭����
    if(i!=0&&i%3==0)
    {
      if(!blTimer)
      {
        if(bMeScore)
        {
          m_ImageMeScoreNumber.TransDrawImage(pDC,nXDrawPos+nOffset,nYDrawPos,SizeScoreNumber.cx-nOffset,SizeScoreNumber.cy,
                                              10*SizeScoreNumber.cx+nOffset/2,0,RGB(255,0,255));
        }
        else
        {
          m_ImageScoreNumber.TransDrawImage(pDC,nXDrawPos+nOffset,nYDrawPos,SizeScoreNumber.cx-nOffset,SizeScoreNumber.cy,
                                            10*SizeScoreNumber.cx+nOffset/2,0,RGB(255,0,255));
        }
        nXDrawPos-=(SizeScoreNumber.cx-nOffset);
      }
    }
    LONG lCellNumber=(LONG)(lNumber%10);
    if(blTimer)
    {
      m_pngTimeNumber.DrawImage(pDC,nXDrawPos,nYDrawPos-2,SizeScoreNumber.cx,SizeScoreNumber.cy,
                                lCellNumber*SizeScoreNumber.cx,0);
    }
    else
    {
      if(bMeScore)
      {
        m_ImageMeScoreNumber.TransDrawImage(pDC,nXDrawPos,nYDrawPos,SizeScoreNumber.cx,SizeScoreNumber.cy,
                                            lCellNumber*SizeScoreNumber.cx,0,RGB(255,0,255));
      }
      else
      {
        m_ImageScoreNumber.TransDrawImage(pDC,nXDrawPos,nYDrawPos,SizeScoreNumber.cx,SizeScoreNumber.cy,
                                          lCellNumber*SizeScoreNumber.cx,0,RGB(255,0,255));
      }

    }
    //���ñ���
    lNumber/=10;
    nXDrawPos-=SizeScoreNumber.cx;
  };
  return;
}

//�滭����
void CGameClientView::DrawNumStrWithSpace(CDC * pDC, LONGLONG lNumber,CRect&rcPrint,INT nFormat)
{
  LONGLONG lTmpNumber = lNumber;
  CString strNumber;
  CString strTmpNumber1;
  CString strTmpNumber2;
  bool blfirst = true;
  bool bLongNum = false;
  int nNumberCount = 0;

  strTmpNumber1.Empty();
  strTmpNumber2.Empty();
  strNumber.Empty();

  if(lNumber == 0)
  {
    strNumber=TEXT("0");
  }

  if(lNumber < 0)
  {
    lNumber =- lNumber;
  }

  if(lNumber >= 100)
  {
    bLongNum = true;
  }

  while(lNumber > 0)
  {
    strTmpNumber1.Format(TEXT("%I64d"),lNumber%10);
    nNumberCount++;
    strTmpNumber2 = strTmpNumber1+strTmpNumber2;

    if(nNumberCount == 3)
    {
      if(blfirst)
      {
        strTmpNumber2 += (TEXT("") +strNumber);
        blfirst = false;
      }
      else
      {
        strTmpNumber2 += (TEXT(",") +strNumber);
      }

      strNumber = strTmpNumber2;
      nNumberCount = 0;
      strTmpNumber2 = TEXT("");
    }
    lNumber /= 10;
  }

  if(strTmpNumber2.IsEmpty() == FALSE)
  {
    if(bLongNum)
    {
      strTmpNumber2 += (TEXT(",") +strNumber);
    }
    else
    {
      strTmpNumber2 += strNumber;
    }

    strNumber = strTmpNumber2;
  }

  if(lTmpNumber < 0)
  {
    strNumber = TEXT("-") + strNumber;
  }
  //�������
  if(nFormat==-1)
  {
    m_FontEx.DrawText(pDC, strNumber, rcPrint, RGB(118,224,255), DT_END_ELLIPSIS|DT_LEFT|DT_TOP|DT_SINGLELINE);
  }
  else
  {
    m_FontEx.DrawText(pDC, strNumber, rcPrint, pDC->GetTextColor(), nFormat);
  }
}

//�滭����
void CGameClientView::DrawSymbolNumberString(CDC * pDC, LONGLONG lNumber, INT nXPos, INT nYPos,int nHalf)
{
  //������Դ
  CSize SizeScoreNumber(m_pngNumberASymbol.GetWidth()/15,m_pngNumberASymbol.GetHeight());

  //������Ŀ
  LONGLONG lNumberCount=0;
  LONGLONG lNumberTemp=lNumber;
  do
  {
    lNumberCount++;
    lNumberTemp/=10;
  }
  while(lNumberTemp>0);

  //λ�ö���
  INT nOffset=8;
  INT nYDrawPos=nYPos-SizeScoreNumber.cy/2;
  INT nXDrawPos=(INT)(nXPos)+ lNumberCount*(SizeScoreNumber.cx-7) ;

  //�滭����
  for(LONGLONG i=0; i<lNumberCount; i++)
  {

    LONG lCellNumber=(LONG)(lNumber%10);
    {
      int yIndex=0,yLength=SizeScoreNumber.cy;
      if(nHalf==1)
      {
        yIndex = m_pngNumberASymbol.GetHeight()/2;
        yLength=yLength/2;
      }
      else if(nHalf==2)
      {
        yLength=yLength/2;
      }
      m_pngNumberASymbol.DrawImage(pDC,nXDrawPos,nYDrawPos,SizeScoreNumber.cx,yLength,
                                   lCellNumber*SizeScoreNumber.cx,yIndex,SizeScoreNumber.cx,yLength);
    }

    //���ñ���
    lNumber/=10;
    nXDrawPos-=(SizeScoreNumber.cx-7);
  };
  return;
}


//�滭����
void CGameClientView::DrawPercentNumberString(CDC * pDC, LONGLONG lNumber, INT nXPos, INT nYPos)
{
  //������Դ
  CSize SizeScoreNumber(m_pngNumberASymbol.GetWidth()/15,m_pngNumberASymbol.GetHeight());

  //������Ŀ
  LONGLONG lNumberCount=0;
  LONGLONG lNumberTemp=lNumber;
  do
  {
    lNumberCount++;
    lNumberTemp/=10;
  }
  while(lNumberTemp>0);

  //λ�ö���
  INT nOffset=8;
  INT nYDrawPos=nYPos-SizeScoreNumber.cy/2;
  INT nXDrawPos=(INT)(nXPos)+ lNumberCount*(SizeScoreNumber.cx-9) ;

  if(lNumber<10)
  {
    nXDrawPos+=15;
  }

  INT nPercentPos = nXDrawPos+SizeScoreNumber.cx;


  //�滭����
  for(LONGLONG i=0; i<lNumberCount; i++)
  {

    LONG lCellNumber=(LONG)(lNumber%10);
    {
      int yIndex=0;
      m_pngNumberASymbol.DrawImage(pDC,nXDrawPos,nYDrawPos,SizeScoreNumber.cx,SizeScoreNumber.cy-yIndex,
                                   lCellNumber*SizeScoreNumber.cx,yIndex,SizeScoreNumber.cx,SizeScoreNumber.cy-yIndex);
    }

    //���ñ���
    lNumber/=10;
    nXDrawPos-=(SizeScoreNumber.cx-9);
  };

  int SystomIndex=13;
  int nSybomWidth=m_pngNumberASymbol.GetWidth()/15;
  m_pngNumberASymbol.DrawImage(pDC,nPercentPos-5,nYDrawPos,nSybomWidth,m_pngNumberASymbol.GetHeight(),
                               SystomIndex*nSybomWidth,0,nSybomWidth,m_pngNumberASymbol.GetHeight());


  return;
}


//ͼƬ����
int CGameClientView::DrawPicNumberString(CDC * pDC, LONGLONG lNumber, INT nXPos, INT nYPos,enXCollocateMode xMode)
{
  //������Դ
  CSize SizeScoreNumber(m_pngNumberASymbol.GetWidth()/15,m_pngNumberASymbol.GetHeight());

  //������Ŀ
  LONGLONG lNumberCount=0;
  LONGLONG lNumberTemp=lNumber<0?-lNumber:lNumber;
  do
  {
    lNumberCount++;
    lNumberTemp/=10;
  }
  while(lNumberTemp>0);

  //λ�ö���
  INT Distance=-7;
  INT nYDrawPos=nYPos-SizeScoreNumber.cy/2;
  INT nXDrawPos=nXPos;
  INT nAllLenth=lNumberCount*SizeScoreNumber.cx+(lNumberCount-1)*Distance;

  if(lNumber<0)
  {
    nAllLenth+=(SizeScoreNumber.cx+Distance);
  }

  //����λ��
  switch(xMode)
  {
    case enXLeft:
    {
      nXDrawPos=(INT)(nXPos)+ nAllLenth ;
      break;
    }
    case enXCenter:
    {
      nXDrawPos=(INT)(nXPos)+ nAllLenth/2 ;
      break;
    }
    case enXRight:
    {
      nXDrawPos=(INT)(nXPos) ;
      break;
    }
  }

  lNumberTemp=lNumber<0?-lNumber:lNumber;

  for(LONGLONG i=0; i<lNumberCount; i++)
  {

    LONG lCellNumber=(LONG)(lNumberTemp%10);
    {
      int yIndex=0,yLength=SizeScoreNumber.cy;

      m_pngNumberASymbol.DrawImage(pDC,nXDrawPos,nYDrawPos,SizeScoreNumber.cx,yLength,
                                   lCellNumber*SizeScoreNumber.cx,yIndex,SizeScoreNumber.cx,yLength);
    }

    //���ñ���
    lNumberTemp/=10;
    nXDrawPos-=(SizeScoreNumber.cx+Distance);
  };

  if(lNumber<0)
  {
    //����
    int SystomIndex=14;

    m_pngNumberASymbol.DrawImage(pDC,nXDrawPos,nYDrawPos,SizeScoreNumber.cx,SizeScoreNumber.cy,
                                 SystomIndex*SizeScoreNumber.cx,0,SizeScoreNumber.cx,SizeScoreNumber.cy);
  }



  return nXDrawPos;
}


void CGameClientView::KillCardTime()
{
  KillTimer(IDI_FLASH_WINNER);

}
void CGameClientView::StartRunCar(int iTimer)
{
  m_bFlashrandShow = false;
  KillTimer(IDI_FLASH_RAND_SIDE);
  KillTimer(IDI_FLASH_CARD);
  KillTimer(IDI_END_FLASH_CARD);
  m_nTimerStep = 400;
  SetTimer(IDI_FLASH_CARD,iTimer,NULL);
  m_nCarOpenSide = m_cbTableCardArray[0];
  m_nTotoalRun = m_nCarOpenSide+ANIMAL_COUNT*3-2;
  m_nCurRunIndex = 0;
  m_CarIndex = 1;
  m_blRungingCar = true;
  m_bDrawYingZi = true;
  SetTimer(IDI_END_FLASH_CARD,12*1000,NULL);

}

void CGameClientView::StartRunAnotherCar(int iTimer)
{
  m_bFlashrandShow = false;
  KillTimer(IDI_FLASH_RAND_SIDE);
  KillTimer(IDI_FLASH_ANOTHER_CARD);
  KillTimer(IDI_SHOW_ANOTHER_CARD_RESULT);
  m_nTimerStep = 400;
  SetTimer(IDI_FLASH_ANOTHER_CARD,iTimer,NULL);
  m_nCarOpenSide = m_cbTableCardArray[1];
  m_nTotoalRun = (ANIMAL_COUNT-m_cbTableCardArray[0]+2)+m_nCarOpenSide+ANIMAL_COUNT*3-2;
  m_nCurRunIndex = 0;
  //m_CarIndex = m_cbTableCardArray[0];
  m_blRungingCar = true;
  m_bDrawYingZi = true;

  SetTimer(IDI_SHOW_ANOTHER_CARD_RESULT,12*1000,NULL);

}

void CGameClientView::StartRunNumber1(int iTimer)
{
  m_nTimerStep = 300;
  SetTimer(IDI_FLASH_NUMBER1,iTimer,NULL);
  m_nNumberOpenSide = m_cbTwoDigit;
  m_nTotoalRun = m_nNumberOpenSide+10*2-2;
  m_nCurRunIndex1 = 0;
  m_NumberIndex1 = 1;
  m_blRungingNumber1 = true;
  SetTimer(IDI_END_FLASH_NUMBER1,5*1000,NULL);
}

void CGameClientView::StartRunNumber2(int iTimer)
{
  m_nTimerStep = 300;
  SetTimer(IDI_FLASH_NUMBER2,iTimer,NULL);
  m_nNumberOpenSide = m_cbOneDigit;
  m_nTotoalRun = m_nNumberOpenSide+10*2-2;
  m_nCurRunIndex2 = 0;
  m_NumberIndex2 = 1;
  m_blRungingNumber2 = true;
  SetTimer(IDI_END_FLASH_NUMBER2,5*1000,NULL);
}

void CGameClientView::RuningCar(int iTimer)
{
  if(m_nCurRunIndex<10)
  {
    m_nTimerStep-=43;

  }
  if(m_nCurRunIndex >= m_nTotoalRun-15)
  {
    m_nTimerStep+=47 /*8*(m_nCurRunIndex-m_nTotoalRun+15)*/;
  }
  if(m_nCurRunIndex==m_nTotoalRun)
  {
    KillTimer(IDI_FLASH_CARD);
    KillTimer(IDI_END_FLASH_CARD);

    //��ʾ����
    if(m_cbShaYuAddMulti!=0)
    {

      m_cbAnimalIndex=0;
      m_bShowCartoon=true;
      SetTimer(IDI_SHOW_ANIMAL,100,NULL);

      m_cbRandAddMulti=0;

      BYTE cbAnimalIndex = GetAnimalInfo(m_cbTableCardArray[0]);

      int cbIndex = 0;
      if(cbAnimalIndex == 8)
      {
        cbIndex = 1;
      }
      if(cbAnimalIndex == 4)
      {
        cbIndex = 3;  //ʨ��
      }
      if(cbAnimalIndex == 5)
      {
        cbIndex = 2;  //��è
      }
      if(cbAnimalIndex == 6)
      {
        cbIndex = 0;  //����
      }
      if(cbAnimalIndex == 7)
      {
        cbIndex = 4;  //����
      }
      if(cbAnimalIndex == 0)
      {
        cbIndex = 8;  //����
      }
      if(cbAnimalIndex == 1)
      {
        cbIndex = 7;  //����
      }
      if(cbAnimalIndex == 2)
      {
        cbIndex = 6;  //��ȸ
      }
      if(cbAnimalIndex == 3)
      {
        cbIndex = 5;  //��ӥ
      }

      //ͨɱ
      if(cbAnimalIndex == 10)
      {
        PlayGameSound(AfxGetInstanceHandle(),TEXT("TONGSHA_SOUND"));
        m_bShowShaYuResult = true;
        m_bShowAnotherResult = false;
        m_DrawGlodShaYu = false;
        RefreshGameView();
        return;
      }
      //������
      if(cbAnimalIndex == 11)
      {
        SendEngineMessage(IDM_ANIMAL_SOUND,1,0);
        m_bShowShaYuResult = true;
        m_bShowAnotherResult = false;
        m_DrawGlodShaYu = true;
        RefreshGameView();
        return;
      }
      //ͨ��
      if(cbAnimalIndex == 9)
      {
        PlayGameSound(AfxGetInstanceHandle(),TEXT("TONGPEI_SOUND"));
        m_DrawGlodShaYu = false;
        m_bShowShaYuResult = true;
        m_bShowAnotherResult = false;
        RefreshGameView();
        return;
      }

      StartRunNumber1(5);
      m_bShowShaYuResult = true;
      m_bShowAnotherResult = false;
      SendEngineMessage(IDM_ANIMAL_SOUND,cbIndex,0);
      m_DrawGlodShaYu = false;
      return;

    }
    else
    {

      m_cbAnimalIndex=0;
      m_bShowCartoon=true;
      SetTimer(IDI_SHOW_ANIMAL,100,NULL);

      BYTE cbAnimalIndex = GetAnimalInfo(m_cbTableCardArray[0]);
      int cbIndex = 0;
      if(cbAnimalIndex == 4)
      {
        cbIndex = 3;  //ʨ��
      }
      if(cbAnimalIndex == 5)
      {
        cbIndex = 2;  //��è
      }
      if(cbAnimalIndex == 6)
      {
        cbIndex = 0;  //����
      }
      if(cbAnimalIndex == 7)
      {
        cbIndex = 4;  //����
      }
      if(cbAnimalIndex == 0)
      {
        cbIndex = 8;  //����
      }
      if(cbAnimalIndex == 1)
      {
        cbIndex = 7;  //����
      }
      if(cbAnimalIndex == 2)
      {
        cbIndex = 6;  //��ȸ
      }
      if(cbAnimalIndex == 3)
      {
        cbIndex = 5;  //��ӥ
      }
      //ͨɱ
      if(cbAnimalIndex == 10)
      {
        PlayGameSound(AfxGetInstanceHandle(),TEXT("TONGSHA_SOUND"));
        return;
      }
      //������
      if(cbAnimalIndex == 11)
      {
        SendEngineMessage(IDM_ANIMAL_SOUND,1,0);
        m_DrawGlodShaYu = true;
        return;
      }
      //ͨ��
      if(cbAnimalIndex == 9)
      {
        PlayGameSound(AfxGetInstanceHandle(),TEXT("TONGPEI_SOUND"));
        m_DrawGlodShaYu = false;
        return;
      }
      m_DrawGlodShaYu = false;
      SendEngineMessage(IDM_ANIMAL_SOUND,cbIndex,0);
      return;
    }
  }
  if(m_nTimerStep<0)
  {
    return ;
  }
  KillTimer(IDI_FLASH_CARD);
  SetTimer(IDI_FLASH_CARD,iTimer,NULL);

}

void CGameClientView::RuningAnotherCar(int iTimer)
{
  if(m_nCurRunIndex<10)
  {
    m_nTimerStep-=43;

  }
  if(m_nCurRunIndex >= m_nTotoalRun-15)
  {
    m_nTimerStep+=47 /*8*(m_nCurRunIndex-m_nTotoalRun+15)*/;
  }
  if(m_nCurRunIndex==m_nTotoalRun)
  {
    KillTimer(IDI_FLASH_ANOTHER_CARD);
    KillTimer(IDI_SHOW_ANOTHER_CARD_RESULT);

    {

      m_blRungingCar = false;
      m_bDrawYingZi = false;

      //���ö�ʱ��
      m_blMoveFinish = true;

      BYTE cbAnimalIndex = GetAnimalInfo(m_cbTableCardArray[1]);
      int cbIndex = 0;
      if(cbAnimalIndex == 8)
      {
        cbIndex = 1;
      }
      if(cbAnimalIndex == 4)
      {
        cbIndex = 3;  //ʨ��
      }
      if(cbAnimalIndex == 5)
      {
        cbIndex = 2;  //��è
      }
      if(cbAnimalIndex == 6)
      {
        cbIndex = 0;  //����
      }
      if(cbAnimalIndex == 7)
      {
        cbIndex = 4;  //����
      }
      if(cbAnimalIndex == 0)
      {
        cbIndex = 8;  //����
      }
      if(cbAnimalIndex == 1)
      {
        cbIndex = 7;  //����
      }
      if(cbAnimalIndex == 2)
      {
        cbIndex = 6;  //��ȸ
      }
      if(cbAnimalIndex == 3)
      {
        cbIndex = 5;  //��ӥ
      }

      //ͨɱ
      if(cbAnimalIndex == 10)
      {
        m_DrawGlodShaYu = false;
        PlayGameSound(AfxGetInstanceHandle(),TEXT("TONGSHA_SOUND"));
        return;
      }
      //������
      if(cbAnimalIndex == 11)
      {
        m_DrawGlodShaYu = true;
        SendEngineMessage(IDM_ANIMAL_SOUND,1,0);
        return;
      }
      //ͨ��
      if(cbAnimalIndex == 9)
      {
        m_DrawGlodShaYu = false;
        PlayGameSound(AfxGetInstanceHandle(),TEXT("TONGPEI_SOUND"));
        return;
      }

      SendEngineMessage(IDM_ANIMAL_SOUND,cbIndex,0);

      this->DispatchCard();
      FinishDispatchCard();
      m_bShowShaYuResult = true;
      m_bShowAnotherResult = true;
    }

    return ;

  }
  if(m_nTimerStep<0)
  {
    return ;
  }
  KillTimer(IDI_FLASH_ANOTHER_CARD);
  SetTimer(IDI_FLASH_ANOTHER_CARD,iTimer,NULL);

}




void CGameClientView::RuningNumber1(int iTimer)
{
  if(m_nCurRunIndex1<10)
  {
    m_nTimerStep-=30;

  }
  if(m_nCurRunIndex1 >= m_nTotoalRun-6)
  {
    m_nTimerStep+=37;//47 /*8*(m_nCurRunIndex-m_nTotoalRun+15)*/;
  }
  if(m_nCurRunIndex1==m_nTotoalRun+2)
  {
    KillTimer(IDI_FLASH_NUMBER1);
    KillTimer(IDI_END_FLASH_NUMBER1);
    {

      StartRunNumber2(5);
      return;


    }

    return ;

  }
  if(m_nTimerStep<0)
  {
    return ;
  }
  KillTimer(IDI_FLASH_NUMBER1);
  SetTimer(IDI_FLASH_NUMBER1,iTimer,NULL);

}

void CGameClientView::RuningNumber2(int iTimer)
{
  if(m_nCurRunIndex2<10)
  {
    m_nTimerStep-=30;

  }
  if(m_nCurRunIndex2 >= m_nTotoalRun-6)
  {
    m_nTimerStep+=37 /*8*(m_nCurRunIndex-m_nTotoalRun+15)*/;
  }
  if(m_nCurRunIndex2==m_nTotoalRun+2)
  {
    KillTimer(IDI_FLASH_NUMBER2);
    KillTimer(IDI_END_FLASH_NUMBER2);
    {
      m_cbRandAddMulti=m_cbShaYuAddMulti;
      StartRunAnotherCar(20);
      return;


    }

    return ;

  }
  if(m_nTimerStep<0)
  {
    return ;
  }
  KillTimer(IDI_FLASH_NUMBER2);
  SetTimer(IDI_FLASH_NUMBER2,iTimer,NULL);

}

void CGameClientView::StartRandShowSide()
{
  KillTimer(IDI_FLASH_RAND_SIDE);
  m_nTimerStep = 100;
  m_bFlashrandShow = true;
  SetTimer(IDI_FLASH_RAND_SIDE,m_nTimerStep,NULL);

  if(m_pGameClientDlg->GetGameStatus()==GS_PLACE_JETTON)
  {
    m_nPlayGameTimes++;
  }



}
//��ʱ����Ϣ
void CGameClientView::OnTimer(UINT nIDEvent)
{
  //��ʾ����
  if(IDI_SHOW_ANIMAL == nIDEvent)
  {
    m_cbAnimalIndex++;// = (m_cbAnimalIndex+1)%15;
    if(m_cbAnimalIndex==14)
    {
      //���ö�ʱ��
      if(!m_cbShaYuAddMulti)
      {
        m_blRungingCar = false;
        m_bDrawYingZi = false;
        m_blMoveFinish = true;
        this->DispatchCard();
        FinishDispatchCard();
        m_bShowAnotherResult = true;

      }

    }
    //���½���
    RefreshGameView();
    return;
  }


  //�ƶ�����1
  if(IDI_FLASH_NUMBER1 == nIDEvent)
  {

    m_cbRandTwoDigit = (m_cbRandTwoDigit+1)%10;
    m_nCurRunIndex1++;
    RuningNumber1(m_nTimerStep);
    //SendEngineMessage(IDM_SOUND,3,3);
    //���½���
    RefreshGameView();
  }
  //�����ƶ�1
  if(IDI_END_FLASH_NUMBER1==nIDEvent)
  {
    StartRunNumber2(5);
    KillTimer(IDI_FLASH_NUMBER1);
    KillTimer(IDI_END_FLASH_NUMBER1);
    return;
  }

  //�ƶ�����2
  if(IDI_FLASH_NUMBER2 == nIDEvent)
  {
    m_cbRandOneDigit = (m_cbRandOneDigit+1)%10;
    m_nCurRunIndex2++;
    RuningNumber2(m_nTimerStep);
    //SendEngineMessage(IDM_SOUND,3,3);
    //���½���
    RefreshGameView();
  }
  //�����ƶ�2
  if(IDI_END_FLASH_NUMBER2==nIDEvent)
  {
    RuningAnotherCar(10);
    KillTimer(IDI_FLASH_NUMBER2);
    KillTimer(IDI_END_FLASH_NUMBER2);
    return;
  }

  //�ƶ�
  if(IDI_FLASH_CARD == nIDEvent)
  {
    m_CarIndex = (m_CarIndex+1)%ANIMAL_COUNT;
    m_nCurRunIndex++;
    RuningCar(m_nTimerStep);
    SendEngineMessage(IDM_SOUND,3,3);

    //���½���
    RefreshGameView();
  }
  //�����ƶ�
  if(IDI_END_FLASH_CARD==nIDEvent)
  {
    for(int i = m_nCurRunIndex; i<m_nTotoalRun; i++)
    {
      m_CarIndex = (m_CarIndex+1)%ANIMAL_COUNT;
      m_nCurRunIndex++;
      if(m_nCurRunIndex==m_nTotoalRun)
      {
        KillTimer(IDI_FLASH_CARD);
        KillTimer(IDI_END_FLASH_CARD);


        //��ʾ����
        if(m_cbShaYuAddMulti!=0)
        {
          //m_cbRandAddMulti=0;
          //SetTimer(IDI_FLASH_ADD_MULTI,100,NULL);
          //SetTimer(IDI_END_ADD_MULTI,1500,NULL);
          StartRunNumber1(5);
          m_bShowShaYuResult = true;
          m_bShowAnotherResult = false;

        }
        else
        {
          //m_cbAnimalIndex=0;
          //m_bShowCartoon=true;
          //SetTimer(IDI_SHOW_ANIMAL,100,NULL);
        }

        return ;
      }
    }
  }
  //�������(��������ʱ��)
  if(IDI_FLASH_ADD_MULTI==nIDEvent)
  {
    m_cbRandAddMulti++;
    if(m_cbRandAddMulti == m_cbShaYuAddMulti)
    {
      KillTimer(IDI_FLASH_ADD_MULTI);
      m_ShaYuIndex = m_CarIndex;
      StartRunAnotherCar(20);
    }
    //���½���
    RefreshGameView();
    return;

  }
  //�����������(��������ʱ��)
  if(IDI_END_ADD_MULTI==nIDEvent)
  {
    m_cbRandAddMulti = m_cbShaYuAddMulti;
    KillTimer(IDI_FLASH_ADD_MULTI);
    KillTimer(IDI_END_ADD_MULTI);
    m_ShaYuIndex = m_CarIndex;
    StartRunAnotherCar(20);
    //���½���
    RefreshGameView();
    return;

  }

  //�����ƶ�һ��(��������ʱ��)
  if(IDI_FLASH_ANOTHER_CARD == nIDEvent)
  {
    m_CarIndex = (m_CarIndex+1)%ANIMAL_COUNT;
    m_nCurRunIndex++;

    RuningAnotherCar(m_nTimerStep);
    SendEngineMessage(IDM_SOUND,3,3);

    //���½���
    RefreshGameView();
  }
  //�����ƶ�һ�ν���(��������ʱ��)
  if(IDI_SHOW_ANOTHER_CARD_RESULT==nIDEvent)
  {
    for(int i = m_nCurRunIndex; i<m_nTotoalRun; i++)
    {
      m_CarIndex = (m_CarIndex+1)%ANIMAL_COUNT;
      m_nCurRunIndex++;
      if(m_nCurRunIndex==m_nTotoalRun)
      {
        KillTimer(IDI_FLASH_ANOTHER_CARD);
        KillTimer(IDI_SHOW_ANOTHER_CARD_RESULT);

        m_blRungingCar = false;
        m_bDrawYingZi = false;

        {
          m_blMoveFinish = true;
          //���ö�ʱ��
          this->DispatchCard();
          FinishDispatchCard();
          //m_cbRandAddMulti = 0;
        }

        return ;
      }
    }
  }

  if(IDI_FLASH_RAND_SIDE ==nIDEvent)
  {
    m_bFlashrandShow=!m_bFlashrandShow;
    m_CarIndex = rand()%ANIMAL_COUNT;
    //���½���
    RefreshGameView();
  }
  //����ʤ��
  if(nIDEvent==IDI_FLASH_WINNER)
  {
    //���ñ���
    m_bFlashResult=!m_bFlashResult;

    //���½���
    RefreshGameView();
    return;
  }
  //�ֻ�ׯ��
  else if(nIDEvent == IDI_SHOW_CHANGE_BANKER)
  {
    ShowChangeBanker(false);
    return;
  }

  else if(nIDEvent == IDI_ANDROID_BET)
  {
    //���½���
    RefreshGameView();
    return;
  }
  else if(nIDEvent >= (UINT)(IDI_ANDROID_BET + 1) && nIDEvent < (UINT)(IDI_ANDROID_BET + m_ArrayAndroid.GetCount() + 1))
  {
    INT_PTR Index = nIDEvent - IDI_ANDROID_BET - 1;
    if(Index < 0 || Index >= m_ArrayAndroid.GetCount())
    {
      ASSERT(FALSE);
      KillTimer(nIDEvent);
      return;
    }

    if(m_ArrayAndroid[Index].lJettonScore > 0)
    {
      LONGLONG lScoreIndex[] = {1000000L,100000L,10000L,1000L,100L};
      BYTE cbViewIndex = m_ArrayAndroid[Index].cbJettonArea;

      //���ӳ���
      for(BYTE i=0; i<CountArray(lScoreIndex); i++)
      {
        if(lScoreIndex[i] > m_lRobotMaxJetton)
        {
          continue;
        }

        if(m_ArrayAndroid[Index].lJettonScore >= lScoreIndex[i])
        {
          m_ArrayAndroid[Index].lJettonScore -= lScoreIndex[i];
          m_lAllJettonScore[cbViewIndex] += lScoreIndex[i];

          tagJettonInfo JettonInfo;
          int iWSize = m_RectArea[cbViewIndex-1].right - m_RectArea[cbViewIndex-1].left - 60;
          int iHSize = m_RectArea[cbViewIndex-1].bottom - m_RectArea[cbViewIndex-1].top - 95;
          JettonInfo.nXPos=rand()%(iWSize);
          JettonInfo.nYPos=rand()%(iHSize);
          JettonInfo.cbJettonIndex = JETTON_COUNT - i - 1;


          //��������
          AfxGetMainWnd()->PostMessage(IDM_SOUND,7,7);
          break;
        }
      }
    }
    return;
  }

  __super::OnTimer(nIDEvent);
}

//�����Ϣ
void CGameClientView::OnLButtonDown(UINT nFlags, CPoint Point)
{
  if(m_bAddScoreStatue)
  {
    return;
  }

  for(int i = 0; i< AREA_COUNT; i++)
  {
    m_btAdd[i].MouseDown(Point);
  }
  //��ȡ���
  IClientUserItem * pClientUserItem=GetClientUserItem(m_wMeChairID);

  for(int i = 0; i < AREA_COUNT; ++i)
  {
    if(m_btAdd[i].GetMouseDown())
    {
      m_bIsContinueStatue = true;
      if(i <4)
      {
        ButtonAddScore(ID_YAN_ZI +i);
      }
      else if(i == 4)
      {
        ButtonAddScore(ID_SHA_YU);
      }
      else if(i >4 && i<9)
      {
        if(i == 5)
        {
          ButtonAddScore(ID_SHI_ZI);
        }
        else if(i == 6)
        {
          ButtonAddScore(ID_XIONG_MAO);
        }
        else if(i == 7)
        {
          ButtonAddScore(ID_HOU_ZI);
        }
        else if(i == 8)
        {
          ButtonAddScore(ID_TU_ZI);
        }
      }
      else if(i == 9)
      {
        ButtonAddScore(ID_FEI_QIN);
      }
      else if(i == 10)
      {
        ButtonAddScore(ID_ZOU_SHOU);
      }


    }
  }

  m_btCleanJeffon.MouseDown(Point);
  m_btContinuJeffon.MouseDown(Point);

  if(m_btCleanJeffon.GetMouseDown())
  {
    ZeroMemory(&m_lAllJettonScore,sizeof(m_lAllJettonScore));
    ZeroMemory(&m_lUserJettonScore,sizeof(m_lUserJettonScore));
    m_lCurrentJetton = 0L;
    SendEngineMessage(IDM_CLEAN_JETTON,0,0);
  }


  if(m_btContinuJeffon.GetMouseDown())
  {
    SendEngineMessage(IDM_LAST_JETTON,0,0);
  }
  /*
  if (m_btContinuJeffon.GetMouseDown())
  {
    bool bCurGameStatue = false;
    if(bCurGameStatue == false) return;
    if(bCurGameStatue == true)
    {

    }

    for (int nAreaIndex=1; nAreaIndex<AREA_COUNT+1; ++nAreaIndex)
    {
      if (m_lUserLastJetton[nAreaIndex] >1)
      {
        m_lCurrentJetton = m_lUserLastJetton[nAreaIndex];
        int i = nAreaIndex -1;

        int nFlagIndex = 0;

        if(i == 1)
        {
          nFlagIndex = 0;
          ButtonAddScore(ID_YAN_ZI+ nFlagIndex);
        }
        else if(i == 2)
        {
          nFlagIndex = 1;
          ButtonAddScore(ID_YAN_ZI+ nFlagIndex);
        }
        else if(i == 3)
        {
          nFlagIndex = 2;
          ButtonAddScore(ID_YAN_ZI+ nFlagIndex);
        }
        else if(i == 4)
        {
          nFlagIndex = 3;
          ButtonAddScore(ID_YAN_ZI+ nFlagIndex);
        }
        else if(i == 8)
        {
          ButtonAddScore(ID_SHA_YU);
        }
        else if(i == 5)
        {
          nFlagIndex = 4;
          ButtonAddScore(ID_YAN_ZI+ nFlagIndex);
        }
        else if(i == 6)
        {
          nFlagIndex = 5;
          ButtonAddScore(ID_YAN_ZI+ nFlagIndex);
        }
        else if(i == 7)
        {
          nFlagIndex = 6;
          ButtonAddScore(ID_YAN_ZI+ nFlagIndex);
        }
        else if(i ==9)
        {
          nFlagIndex = 8;
          ButtonAddScore(ID_YAN_ZI + nFlagIndex);
        }
        else if(i ==0)
        {
          nFlagIndex =-1;
          ButtonAddScore(ID_YAN_ZI+ nFlagIndex);
        }
        else if(i ==10)
        {
          nFlagIndex =9;
          ButtonAddScore(ID_YAN_ZI+ nFlagIndex);
        }
        else continue;
      }
      else continue;
    }
  }*/


  Invalidate(FALSE);
  return __super::OnLButtonDown(nFlags,Point);

  return;
}
void CGameClientView::OnLButtonUp(UINT nFlags, CPoint Point)
{
  for(int i = 0; i< AREA_COUNT; i++)
  {
    m_btAdd[i].MouseUp(Point);
  }

  m_bIsContinueStatue = true;
  m_btCleanJeffon.MouseUp(Point);
  m_btContinuJeffon.MouseUp(Point);
  Invalidate(TRUE);
  return __super::OnLButtonUp(nFlags,Point);;
}
//���������ؼ���������Ϣ
LRESULT CGameClientView::OnViLBtUp(WPARAM wParam, LPARAM lParam)
{
  CPoint *pPoint = (CPoint*)lParam;

  ScreenToClient(pPoint);
  OnLButtonUp(1,*pPoint);
  return 1;
}

//����ƶ���Ϣ
void CGameClientView::OnMouseMove(UINT nFlags, CPoint Point)
{
  for(int i = 0; i< AREA_COUNT; i++)
  {
    m_btAdd[i].MouseMove(Point);
  }

  m_btCleanJeffon.MouseMove(Point);
  m_btContinuJeffon.MouseMove(Point);

  if(m_bGameStatus != 255)
  {
    if(m_btContinuJeffon.GetMouseMove())
    {
      m_bIsContinueStatue = false;

    }
    else
    {
      CRect SelectJetton;
      CRect WinRect;
      GetWindowRect(WinRect);
      SelectJetton.SetRect(0,530,WinRect.Width(),WinRect.Height()/3 + 530);
      CRect rcFeiQin,rcZouSou;
      m_btAdd[9].GetWindowRect(rcFeiQin);
      m_btAdd[10].GetWindowRect(rcZouSou);
      if(SelectJetton.PtInRect(Point) || rcFeiQin.PtInRect(Point) || rcZouSou.PtInRect(Point))
      {
        m_bIsContinueStatue = true;
      }
      else
      {
        m_bIsContinueStatue = false;
      }

    }
  }

  Invalidate(TRUE);
  return __super::OnMouseMove(nFlags,Point);

}

//�����Ϣ
void CGameClientView::OnRButtonDown(UINT nFlags, CPoint Point)
{
  //���ñ���
  //m_lCurrentJetton=0L;
  if(m_pGameClientDlg->GetGameStatus()!=GS_GAME_END && m_cbAreaFlash!=0xFF)
  {
    m_cbAreaFlash=0xFF;
    RefreshGameView();
  }

  __super::OnLButtonDown(nFlags,Point);
}

//�����Ϣ
BOOL CGameClientView::OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage)
{
  if(FALSE == m_bIsContinueStatue)
  {
    SetCursor(LoadCursor(NULL,IDC_ARROW));
    return TRUE;
  };
  //���ù��
  switch(m_lCurrentJetton)
  {
    case 100:
    {
      SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_SCORE_100)));
      return TRUE;
    }
    case 1000:
    {
      SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_SCORE_1000)));
      return TRUE;
    }
    case 10000:
    {
      SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_SCORE_10000)));
      return TRUE;
    }
    case 100000:
    {
      SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_SCORE_100000)));
      return TRUE;
    }
    case 1000000:
    {
      SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_SCORE_1000000)));
      return TRUE;
    }
    default:
    {
      SetCursor(LoadCursor(NULL,IDC_ARROW));
    }
  }

  Invalidate(FALSE);
  return __super::OnSetCursor(pWnd, nHitTest, uMessage);
}

//�ֻ�ׯ��
void CGameClientView::ShowChangeBanker(bool bChangeBanker)
{
  //�ֻ�ׯ��
  if(bChangeBanker)
  {
    SetTimer(IDI_SHOW_CHANGE_BANKER, 3000, NULL);
    m_bShowChangeBanker = true;
  }
  else
  {
    KillTimer(IDI_SHOW_CHANGE_BANKER);
    m_bShowChangeBanker = false ;
  }

  //���½���
  RefreshGameView();
}

//��ׯ��ť
void CGameClientView::OnApplyBanker()
{
  SendEngineMessage(IDM_APPLY_BANKER,1,0);

}

//��ׯ��ť
void CGameClientView::OnCancelBanker()
{
  SendEngineMessage(IDM_APPLY_BANKER,0,0);
}

//ׯ����Ϣ
void CGameClientView::SetBankerInfo(DWORD dwBankerUserID, LONGLONG lBankerScore)
{
  //ׯ�����Ӻ�
  WORD wBankerUser=INVALID_CHAIR;

  //�������Ӻ�
  if(0!=dwBankerUserID)
  {
    for(WORD wChairID=0; wChairID<MAX_CHAIR; ++wChairID)
    {
      //tagUserData const *pUserData=GetUserInfo(wChairID);
      IClientUserItem * pClientUserItem=GetClientUserItem(wChairID);
      if(NULL!=pClientUserItem && dwBankerUserID==pClientUserItem->GetUserID())
      {
        wBankerUser=wChairID;
        break;
      }
    }
  }

  //�л��ж�
  if(m_wBankerUser!=wBankerUser)
  {
    m_wBankerUser=wBankerUser;
    m_wBankerTime=0L;
    m_lBankerWinScore=0L;
    m_lTmpBankerWinScore=0L;
  }
  m_lBankerScore=lBankerScore;
}

//�滭��ʶ
void CGameClientView::DrawWinFlags(CDC * pDC)
{
  ////�ǿ��ж�
  if(m_nRecordLast==m_nRecordFirst)
  {
    return;
  }
  int nIndex = m_nScoreHead;
  COLORREF static clrOld ;
  clrOld = pDC->SetTextColor(RGB(52,116,23));
  int nDrawCount=0;
  while(nIndex != m_nRecordLast && (m_nRecordLast!=m_nRecordFirst) && nDrawCount < MAX_FALG_COUNT)
  {
    //ʤ����ʶ
    tagClientGameRecord &ClientGameRecord = m_GameRecordArrary[nIndex];
    //λ�ñ���
    int static nYPos=0,nXPos=0;
    nYPos=m_LuZiStart.y + 110;
    nXPos=m_LuZiStart.x+((nIndex - m_nScoreHead + MAX_SCORE_HISTORY) % MAX_SCORE_HISTORY)*46 - 30;

    //ʤ����ʶ
    int static nFlagsIndex=0;

    for(int i = 1; i<AREA_COUNT+1; i++)
    {
      if(ClientGameRecord.enOperateMen[i]==enOperateResult_Win)
      {
        switch(i)
        {
          case ID_TU_ZI:
            nFlagsIndex=7;
            break;

          case ID_YAN_ZI:
            nFlagsIndex=0;
            break;
          case ID_GE_ZI:
            nFlagsIndex=1;
            break;
          case ID_KONG_QUE:
            nFlagsIndex=2;
            break;
          case ID_LAO_YING:
            nFlagsIndex=3;
            break;

          case ID_SHI_ZI:
            nFlagsIndex=4;
            break;
          case ID_XIONG_MAO:
            nFlagsIndex=5;
            break;
          case ID_HOU_ZI:
            nFlagsIndex=6;
            break;
          case ID_SHA_YU:
            nFlagsIndex=8;
            break;
            //ͨɱ
          case ID_FEI_QIN:
            nFlagsIndex = 9;
            break;
            //ͨ��
          case ID_ZOU_SHOU:
            nFlagsIndex = 10;
            break;
            //����
          case ID_GLOD_SHA:
            nFlagsIndex = 11;
            break;
        }
      }
    }

    //�滭��ʶ
    m_pngLuZiAnimal.DrawImage(pDC, nXPos, nYPos, m_pngLuZiAnimal.GetWidth()/12, \
                              m_pngLuZiAnimal.GetHeight(),m_pngLuZiAnimal.GetWidth()/12 * nFlagsIndex, 0);

    //�ƶ��±�
    nIndex = (nIndex+1) % MAX_SCORE_HISTORY;
    nDrawCount++;
  }

  pDC->SetTextColor(clrOld);
}
//�ƶ���ť
void CGameClientView::OnScoreMoveL()
{
  if(m_nRecordFirst == m_nScoreHead)
  {
    return;
  }

  m_nScoreHead = (m_nScoreHead - 1 + MAX_SCORE_HISTORY) % MAX_SCORE_HISTORY;
  if(m_nScoreHead == m_nRecordFirst)
  {
    m_btScoreMoveL.EnableWindow(FALSE);
  }

  m_btScoreMoveR.EnableWindow(TRUE);

  //���½���
  RefreshGameView();
}

//�ƶ���ť
void CGameClientView::OnScoreMoveR()
{
  int nHistoryCount = (m_nRecordLast - m_nScoreHead + MAX_SCORE_HISTORY) % MAX_SCORE_HISTORY;
  if(nHistoryCount == MAX_FALG_COUNT)
  {
    return;
  }

  m_nScoreHead = (m_nScoreHead + 1) % MAX_SCORE_HISTORY;
  if(nHistoryCount-1 == MAX_FALG_COUNT)
  {
    m_btScoreMoveR.EnableWindow(FALSE);
  }

  m_btScoreMoveL.EnableWindow(TRUE);

  //���½���
  RefreshGameView();
}
//��ʾ���
void CGameClientView::DrawShaYuResult(CDC *pDC, int nWidth, int nHeight)
{

  if(false==m_bShowShaYuResult)
  {
    return;
  }

  int nXPos = nWidth +5;
  int nYPos = nHeight + 80;
  int nSybomWidth=m_pngNumberASymbol.GetWidth()/15;
  int indexAnimal = 8;
  //����
  m_pngResultFShayu.DrawImage(pDC,nWidth,nHeight);
  ////����
  //m_pngResultAnimal.DrawImage(pDC,nXPos+8,nYPos+55,m_pngResultAnimal.GetWidth()/12,m_pngResultAnimal.GetHeight(),
  //  8*m_pngResultAnimal.GetWidth()/12,0,m_pngResultAnimal.GetWidth()/12,m_pngResultAnimal.GetHeight());

  int yDis = nYPos+60;
  //�˺�
  int SystomIndex=11;
  int xBegin = nXPos+70;
  //m_pngNumberASymbol.DrawImage(pDC,xBegin+5,yDis+2,nSybomWidth,m_pngNumberASymbol.GetHeight(),
  //  SystomIndex*nSybomWidth,0,nSybomWidth,m_pngNumberASymbol.GetHeight());

  //24
  //xBegin+=(nSybomWidth+5-55);
  //DrawSymbolNumberString(pDC,24,xBegin+33,yDis+16);

  //+
  SystomIndex=10;
  xBegin+=(nSybomWidth*2+51);
  //m_pngNumberASymbol.DrawImage(pDC,xBegin-2,yDis,nSybomWidth,m_pngNumberASymbol.GetHeight(),
  //  SystomIndex*nSybomWidth,0,nSybomWidth,m_pngNumberASymbol.GetHeight());

  xBegin+=(nSybomWidth+5) +293;
  int xLen = 0;
  if(m_cbRandAddMulti<10)
  {
    xLen+=7;
  }
  //�����
  //DrawSymbolNumberString(pDC,m_cbRandAddMulti+1,xBegin-18+xLen,yDis+8,true);

  //�����
  //DrawSymbolNumberString(pDC,m_cbShaYuAddMulti,xBegin-18+xLen,yDis-23);

  //�����
  //DrawSymbolNumberString(pDC,m_cbRandAddMulti-1,xBegin-18+xLen,yDis+46+8,true);


  //�����(��)
  //DrawSymbolNumberString(pDC,m_cbRandOneDigit==0?9:(m_cbRandOneDigit-1)%10,xBegin-14+xLen,yDis+9,1);

  //�����(��)
  //DrawSymbolNumberString(pDC,m_cbRandTwoDigit==0?9:(m_cbRandTwoDigit-1)%10,xBegin+2+xLen,yDis+9,1);


  //�����(��)
  DrawSymbolNumberString(pDC,m_cbRandOneDigit,xBegin-14+xLen,yDis+22);

  //�����(��)
  DrawSymbolNumberString(pDC,m_cbRandTwoDigit,xBegin+2+xLen,yDis+22);

  //�����(��)
  //DrawSymbolNumberString(pDC,(m_cbRandOneDigit+1)%10,xBegin-14+xLen,yDis+53,2);

  //�����(��)
  //DrawSymbolNumberString(pDC,(m_cbRandTwoDigit+1)%10,xBegin+2+xLen,yDis+53,2);

  //=
  xBegin+=(nSybomWidth*2+5);
  SystomIndex=12;
  //m_pngNumberASymbol.DrawImage(pDC,xBegin-12,yDis,nSybomWidth,m_pngNumberASymbol.GetHeight(),
  //SystomIndex*nSybomWidth,0,nSybomWidth,m_pngNumberASymbol.GetHeight());


  if(m_cbRandAddMulti==m_cbShaYuAddMulti)
  {
    //���
    DrawSymbolNumberString(pDC,m_cbRandAddMulti+24,xBegin + 20,yDis+24);
  }



}

//��ʾ���
void CGameClientView::DrawAnotherResult(CDC *pDC, int nWidth, int nHeight)
{

  if(false==m_bShowAnotherResult)
  {
    return;
  }

  int CurAnimal = 0;
  if(m_cbShaYuAddMulti!=0)
  {
    CurAnimal = m_cbTableCardArray[1];

  }
  else
  {
    CurAnimal = m_cbTableCardArray[0];
  }


  //����
  int nMulti=GetAnimalMulti(CurAnimal);

  if((int)nMulti == -1 && CurAnimal== 11)
  {

  }
  m_pngResultFOther.DrawImage(pDC,nWidth /2 - 140,nHeight/2 + 32);

  LONGLONG lRestuleScore =  m_lMeCurGameScore;

  DrawPersonData(pDC,lRestuleScore,nWidth /2 - 30,nHeight/2 + 46);
//  DrawPersonData(pDC,nMulti,nWidth /2 - 30,nHeight/2 + 26);
}



void   CGameClientView::GetAllWinArea(BYTE bcWinArea[],BYTE bcAreaCount,BYTE InArea)
{
  if(InArea==0xFF)
  {
    return ;
  }
  ZeroMemory(bcWinArea,bcAreaCount);


  LONGLONG lMaxSocre = 0;

  for(int i = 0; i<ANIMAL_COUNT; i++)
  {
    BYTE bcOutCadDataWin[AREA_COUNT];
    BYTE bcData[1];
    bcData[0]=i+1;
    m_GameLogic.GetCardType(bcData[0],1,bcOutCadDataWin);
    for(int j= 0; j<AREA_COUNT; j++)
    {

      if(bcOutCadDataWin[j]>1&&j==InArea-1)
      {
        LONGLONG Score = 0;
        for(int nAreaIndex=1; nAreaIndex<=AREA_COUNT; ++nAreaIndex)
        {
          if(bcOutCadDataWin[nAreaIndex-1]>1)
          {
            Score += m_lAllJettonScore[nAreaIndex]*(bcOutCadDataWin[nAreaIndex-1]);
          }
        }
        if(Score>=lMaxSocre)
        {
          lMaxSocre = Score;
          CopyMemory(bcWinArea,bcOutCadDataWin,bcAreaCount);

        }
        break;
      }
    }
  }
}
//�����ע
LONGLONG CGameClientView::GetUserMaxJetton(BYTE cbJettonArea)
{
  if(cbJettonArea==0xFF)
  {
    return 0;
  }

  //����ע��
  LONGLONG lNowJetton = 0;
  ASSERT(AREA_COUNT<=CountArray(m_lUserJettonScore));
  for(int nAreaIndex=1; nAreaIndex<=AREA_COUNT; ++nAreaIndex)
  {
    lNowJetton += m_lUserJettonScore[nAreaIndex];
  }
  //ׯ�ҽ��
  LONGLONG lBankerScore = 0x7fffffffffffffff;
  if(m_wBankerUser!=INVALID_CHAIR)
  {
    lBankerScore = m_lBankerScore;
  }

  BYTE bcWinArea[AREA_COUNT];
  LONGLONG LosScore = 0;
  LONGLONG WinScore = 0;

  GetAllWinArea(bcWinArea,AREA_COUNT,cbJettonArea);

  for(int nAreaIndex=1; nAreaIndex<=AREA_COUNT; ++nAreaIndex)
  {
    if(bcWinArea[nAreaIndex-1]>1)
    {
      LosScore+=m_lAllJettonScore[nAreaIndex]*(bcWinArea[nAreaIndex-1]);
    }
    else
    {
      if(bcWinArea[nAreaIndex-1]==0)
      {
        WinScore+=m_lAllJettonScore[nAreaIndex];
      }
    }
  }

  LONGLONG lTemp = lBankerScore;
  lBankerScore = lBankerScore + WinScore - LosScore;

  if(lBankerScore < 0)
  {
    if(m_wBankerUser!=INVALID_CHAIR)
    {
      lBankerScore = m_lBankerScore;
    }
    else
    {
      lBankerScore = 0x7fffffffffffffff;
    }
  }

  //��������
  LONGLONG lMeMaxScore;

  if((m_lMeMaxScore - lNowJetton)>m_lAreaLimitScore)
  {
    lMeMaxScore= m_lAreaLimitScore;
  }
  else
  {
    lMeMaxScore = m_lMeMaxScore-lNowJetton;
    lMeMaxScore = lMeMaxScore;
  }

  //ׯ������
  lMeMaxScore=min(lMeMaxScore,(lBankerScore)/(bcWinArea[cbJettonArea-1]));

  //��������
  lMeMaxScore = max(lMeMaxScore, 0);

  return lMeMaxScore;
}

//�ɼ�����
void CGameClientView::SetGameScore(LONGLONG lMeCurGameScore, LONGLONG lMeCurGameReturnScore, LONGLONG lBankerCurGameScore)
{
  m_lMeCurGameScore=lMeCurGameScore;
  m_lMeCurGameReturnScore=lMeCurGameReturnScore;
  m_lBankerCurGameScore=lBankerCurGameScore;
}

void CGameClientView::DrawMeJettonNumber(CDC *pDC)
{
  //�滭����
  for(int nAreaIndex=1; nAreaIndex<=AREA_COUNT; ++nAreaIndex)
  {
    if(m_lUserJettonScore[nAreaIndex] > 0)
    {
      //�滭����
      pDC->SetTextColor(RGB(255,255,255));


      CString strJettonNumber;
      strJettonNumber.Format(TEXT("%I64d"), m_lUserJettonScore[nAreaIndex]);

      //pDC->TextOut(m_PointAnimalRand[nAreaIndex-1].x,m_PointAnimalRand[nAreaIndex-1].y+25,strJettonNumber);


    }
    //DrawNumberString(pDC,m_lUserJettonScore[nAreaIndex],m_PointAnimalRand[nAreaIndex-1].x,m_PointAnimalRand[nAreaIndex-1].y+25, false,true);
  }
}

//��ʼ����
void CGameClientView::DispatchCard()
{

  //���ñ�ʶ
  m_bNeedSetGameRecord=true;
}

//��������
void CGameClientView::FinishDispatchCard(bool bRecord /*= true*/)
{

  //����ж�
  if(m_bNeedSetGameRecord==false)
  {
    return;
  }

  //���ñ�ʶ
  m_bNeedSetGameRecord=false;

  //ɾ����ʱ��
  //KillTimer(IDI_DISPATCH_CARD);

  BYTE  bcResulteOut[AREA_COUNT];
  memset(bcResulteOut,0,AREA_COUNT);

  m_GameLogic.GetCardType(m_cbTableCardArray[0],1,bcResulteOut);

  //�����¼
  if(bRecord)
  {
    SetGameHistory(bcResulteOut);
  }

  //�ۼƻ���
  m_lMeStatisticScore+=m_lMeCurGameScore;
  m_lBankerWinScore=m_lTmpBankerWinScore;


  bool blWin[AREA_COUNT];
  memset(blWin,0,sizeof(bool)*AREA_COUNT);
  if(bcResulteOut[9] == 1)
  {
    for(int i = 0; i<AREA_COUNT; i++)
    {

      blWin[i]=true;

    }

    //����Ӯ��
    SetWinnerSide(blWin, true);
  }
  else if(bcResulteOut[10] == 255)
  {
    for(int i = 0; i<AREA_COUNT; i++)
    {
      if(blWin[i])
      {
        blWin[i]=false;
      }
    }

    //����Ӯ��
    SetWinnerSide(blWin, true);
  }
  else
  {
    for(int i = 0; i<AREA_COUNT; i++)
    {

      if(bcResulteOut[i]>0)
      {
        blWin[i]=true;
      }
      else
      {
        blWin[i]=false;
      }
    }

    //����Ӯ��
    SetWinnerSide(blWin, true);
  }

  //��������
  if(m_lMeCurGameScore>0)
  {
    SendEngineMessage(IDM_SOUND,5,5);

  }
  else if(m_lMeCurGameScore<0)
  {
    SendEngineMessage(IDM_SOUND,4,4);

  }
}

//ʤ���߿�
void CGameClientView::FlashJettonAreaFrame(int nWidth, int nHeight, CDC *pDC)
{
  //�Ϸ��ж�
  if(m_bFlashResult&&!m_blRungingCar)
  {

    //����ת��
    int selPngWidth = m_idb_selPng.GetWidth()/5;
    int setPngHeight = m_idb_selPng.GetHeight();

    m_idb_selPng.DrawImage(pDC ,m_CarRect[m_CarIndex].left,
                           m_CarRect[m_CarIndex].top,
                           selPngWidth ,
                           setPngHeight,
                           1*selPngWidth,
                           0,
                           selPngWidth ,
                           setPngHeight);

    //��ʾ����
    int AnimalIndex=GetAnimalInfo(m_CarIndex+1);
    /*m_pngAnimalLight.DrawImage(pDC ,m_CarRect[m_CarIndex].left,
      m_CarRect[m_CarIndex].top,
      m_pngAnimalLight.GetWidth()/12 ,
      m_pngAnimalLight.GetHeight(),
      (AnimalIndex)*(m_pngAnimalLight.GetWidth()/12),
      0,
      m_pngAnimalLight.GetWidth()/12 ,
      m_pngAnimalLight.GetHeight());*/

    int nFlagAnimal = 0;
    for(int i = 0; i < AREA_COUNT -1; ++i)
    {
      if(m_bWinFlag[i])
      {
        if(i<4)
        {
          if(i == 0)
          {
            nFlagAnimal = 8;
          }
          if(i == 1)
          {
            nFlagAnimal = 0;
          }
          if(i == 2)
          {
            nFlagAnimal = 1;
          }
          if(i == 3)
          {
            nFlagAnimal = 2;
          }
          m_ImgWinTexture[0].DrawImage(pDC,m_WinPosition[nFlagAnimal].x,m_WinPosition[nFlagAnimal].y);
        }
        else if(i == 4)
        {
          if(i == 4)
          {
            nFlagAnimal =3;
          }
          m_ImgWinTexture[0].DrawImage(pDC,m_WinPosition[nFlagAnimal].x,m_WinPosition[nFlagAnimal].y);
        }
        else if(i >4 && i<9)
        {
          if(i == 5)
          {
            nFlagAnimal =5;
          }
          if(i == 6)
          {
            nFlagAnimal =6;
          }
          if(i == 7)
          {
            nFlagAnimal =7;
          }
          if(i == 8)
          {
            nFlagAnimal =4;
            m_ImgWinTexture[1].DrawImage(pDC,m_WinPosition[nFlagAnimal].x,m_WinPosition[nFlagAnimal].y);
          }
          else
          {
            m_ImgWinTexture[0].DrawImage(pDC,m_WinPosition[nFlagAnimal].x,m_WinPosition[nFlagAnimal].y);
          }

        }
        else if(i == 9)
        {
          m_ImgWinTexture[2].DrawImage(pDC,m_WinPosition[i].x,m_WinPosition[i].y);
        }
        else if(i == 10)
        {
          m_ImgWinTexture[2].DrawImage(pDC,m_WinPosition[i].x,m_WinPosition[i].y);
        }
        else
        {
          continue;
        }
      }
    }


    if(m_ShaYuIndex!=0)
    {

      //����ת��
      int selPngWidth = m_idb_selPng.GetWidth()/5;
      int setPngHeight = m_idb_selPng.GetHeight();

      m_idb_selPng.DrawImage(pDC ,m_CarRect[m_CarIndex].left,
                             m_CarRect[m_CarIndex].top,
                             selPngWidth ,
                             setPngHeight,
                             1*selPngWidth,
                             0,
                             selPngWidth ,
                             setPngHeight);

      //��ʾ����
      int AnimalIndex=GetAnimalInfo(m_CarIndex+1);
      m_pngAnimalLight.DrawImage(pDC ,m_CarRect[m_CarIndex].left,
                                 m_CarRect[m_CarIndex].top,
                                 m_pngAnimalLight.GetWidth()/12 ,
                                 m_pngAnimalLight.GetHeight(),
                                 (AnimalIndex)*(m_pngAnimalLight.GetWidth()/12),
                                 0,
                                 m_pngAnimalLight.GetWidth()/12 ,
                                 m_pngAnimalLight.GetHeight());

    }

  }
}

//�ƶ�Ӯ��
void CGameClientView::DeduceWinner(bool bWinMen[])
{
  BYTE bcData = m_cbTableCardArray[0];
  if(1==bcData||bcData==2||bcData==9||bcData==10||bcData==17||bcData==18||bcData==25||bcData==26)
  {
    if(bcData%2==1)
    {
      bWinMen[0]= true;
    }
    else
    {
      bWinMen[1]= true;
    }

  }
  else if(3==bcData||bcData==4||bcData==3+8||bcData==4+8||bcData==3+2*8||bcData==4+2*8||bcData==3+3*8||bcData==4+3*8)
  {
    if(bcData%2==1)
    {
      bWinMen[2]= true;
    }
    else
    {
      bWinMen[3]= true;
    }



  }
  else if(5==bcData||bcData==6||bcData==5+8||bcData==6+8||bcData==5+2*8||bcData==6+2*8||bcData==5+3*8||bcData==6+3*8)
  {
    if(bcData%2==1)
    {
      bWinMen[4]= true;
    }
    else
    {
      bWinMen[5]= true;
    }

  }
  else if(7==bcData||bcData==8||bcData==7+8||bcData==8+8||bcData==7+2*8||bcData==8+2*8||bcData==7+3*8||bcData==8+3*8)
  {
    if(bcData%2==1)
    {
      bWinMen[6]= true;
    }
    else
    {
      bWinMen[7]= true;
    }

  }
}

//�ؼ�����
BOOL CGameClientView::OnCommand(WPARAM wParam, LPARAM lParam)
{
  //��ȡID
  WORD wControlID=LOWORD(wParam);

  //�ؼ��ж�
  switch(wControlID)
  {
      //case IDC_JETTON_BUTTON_10:
      //  {
      //    //���ñ���
      //    m_lCurrentJetton=10L;

      //    ResetButtonState();
      //    m_btJetton10.SetWindowText(TEXT("��"));
      //    break;
      //  }
    case IDC_JETTON_BUTTON_100:
    {
      //���ñ���
      m_lCurrentJetton=100L;
      ResetButtonState();
      m_bIsContinueStatue = true;
      break;
    }
    case IDC_JETTON_BUTTON_1000:
    {
      //���ñ���
      m_lCurrentJetton=1000L;
      ResetButtonState();
      m_bIsContinueStatue = true;
      break;
    }
    case IDC_JETTON_BUTTON_10000:
    {
      //���ñ���
      m_lCurrentJetton=10000L;
      ResetButtonState();
      m_bIsContinueStatue = true;
      break;
    }
    case IDC_JETTON_BUTTON_100000:
    {
      //���ñ���
      m_lCurrentJetton=100000L;
      ResetButtonState();
      m_bIsContinueStatue = true;
      break;
    }
    case IDC_JETTON_BUTTON_1000000:
    {
      //���ñ���
      m_lCurrentJetton=1000000L;
      ResetButtonState();
      m_bIsContinueStatue = true;
      break;
    }
    //case IDC_JETTON_BUTTON_5000000:
    //  {
    //    //���ñ���
    //    m_lCurrentJetton=5000000L;
    //    ResetButtonState();
    //    m_btJetton5000000.SetWindowText(TEXT("��"));
    //    break;
    //  }
    case IDC_AUTO_OPEN_CARD:
    {
      break;
    }
    case IDC_OPEN_CARD:
    {
      break;
    }
    case IDC_BANK:
    {
      break;
    }
    case IDC_BANK_DRAW:
    {
      OnBankDraw();
      break;
    }
    case IDC_BANK_STORAGE:
    {
      OnBankStorage();
      break;
    }
    case IDC_COMMIT_PLACE_JETTON:
    {
      OnPlaceJetton();
    }

  }

  return CGameFrameView::OnCommand(wParam, lParam);
}

//����״̬
void CGameClientView::ResetButtonState()
{
  //m_btJetton10.SetWindowText(TEXT(""));
  m_btJetton100.SetWindowText(TEXT(""));
  m_btJetton1000.SetWindowText(TEXT(""));
  m_btJetton10000.SetWindowText(TEXT(""));
  m_btJetton100000.SetWindowText(TEXT(""));
  m_btJetton1000000.SetWindowText(TEXT(""));
  //m_btJetton5000000.SetWindowText(TEXT(""));
}

//�ҵ�λ��
void CGameClientView::SetMeChairID(DWORD dwMeUserID)
{
  //�������Ӻ�
  for(WORD wChairID=0; wChairID<MAX_CHAIR; ++wChairID)
  {
    //tagUserData const *pUserData=GetUserInfo(wChairID);
    IClientUserItem * pClientUserItem=GetClientUserItem(wChairID);
    if(NULL!=pClientUserItem && dwMeUserID==pClientUserItem->GetUserID())
    {
      m_wMeChairID=wChairID;
      break;
    }
  }
}

//�滭ׯ��
void CGameClientView::DrawBankerInfo(CDC *pDC,int nWidth,int nHeight)
{
  CBrush brush;
  brush.CreateSolidBrush(RGB(255,0,0));
  //ׯ����Ϣ
  pDC->SetTextColor(RGB(255,255,0));
  //��ȡ���


  IClientUserItem * pClientUserItem=m_wBankerUser==INVALID_CHAIR ? NULL :GetClientUserItem(m_wBankerUser);

  CRect  StrRect;
  StrRect.left = m_LifeWidth+89;
  StrRect.top  = m_TopHeight+55;
  StrRect.right = StrRect.left + 90;
  StrRect.bottom = StrRect.top + 15;

  tagUserInfo* pUserInfo=NULL;

  if(pClientUserItem!=NULL)
  {
    pUserInfo =  pClientUserItem->GetUserInfo();
  }
  //ׯ������
  TCHAR Text[256];
  if(m_bEnableSysBanker)
  {
    myprintf(Text,256,_TEXT("%s"),pClientUserItem==NULL?(m_bEnableSysBanker?TEXT("ϵͳ��ׯ"):TEXT("ϵͳ��ׯ")):pUserInfo->szNickName);
    m_FontEx.DrawText(pDC, Text, StrRect, pDC->GetTextColor(), DT_END_ELLIPSIS | DT_CENTER | DT_TOP|DT_SINGLELINE);
  }
  else
  {
    myprintf(Text,256,_TEXT("%s"),pClientUserItem==NULL?(m_bEnableSysBanker==false?TEXT("������ׯ"):TEXT("������ׯ")):pUserInfo->szNickName);
    m_FontEx.DrawText(pDC, Text, StrRect, pDC->GetTextColor(), DT_END_ELLIPSIS | DT_CENTER | DT_TOP|DT_SINGLELINE);
  }

  StrRect.left = StrRect.left+150;
  StrRect.right = StrRect.left +30;
  myprintf(Text,256,_TEXT("%d"),m_wBankerTime);
  m_FontEx.DrawText(pDC, Text, StrRect, pDC->GetTextColor(), DT_END_ELLIPSIS | DT_CENTER | DT_TOP|DT_SINGLELINE);

  StrRect.left = StrRect.left+90;
  StrRect.right = StrRect.left +80;
  DrawNumStrWithSpace(pDC,pClientUserItem==NULL?0:pClientUserItem->GetUserScore(),StrRect);

  StrRect.left = StrRect.left+96+45;
  StrRect.right = StrRect.left +85;
  DrawNumStrWithSpace(pDC,m_lBankerWinScore,StrRect);
}

//�滭���
void CGameClientView::DrawMeInfo(CDC *pDC,int nWidth,int nHeight)
{
  if(INVALID_CHAIR==m_wMeChairID)
  {
    return;
  }
  //ׯ����Ϣ
  pDC->SetTextColor(RGB(255,255,0));

  //��ȡ���
  IClientUserItem * pClientUserItem=GetClientUserItem(m_wMeChairID);

  CRect static StrRect;
  StrRect.left = m_LifeWidth+690;
  StrRect.top = m_TopHeight+649;
  StrRect.right =StrRect.left+87;
  StrRect.bottom=StrRect.top+20;

  //�ܽ��
  StrRect.top = StrRect.bottom;
  StrRect.right = StrRect.left + 87;
  StrRect.bottom = StrRect.top + 19;

  LONGLONG lMeJetton=0L;
  for(int nAreaIndex=1; nAreaIndex<AREA_COUNT+1; ++nAreaIndex)
  {
    lMeJetton += m_lUserJettonScore[nAreaIndex];
  }
  //this->DrawPersonData(pDC,pClientUserItem->GetUserScore() - lMeJetton,StrRect.left+70,StrRect.top);
  DrawNumberString(pDC,pClientUserItem->GetUserScore() - lMeJetton,StrRect.left+70,StrRect.top+4);

  //���ý��
  StrRect.bottom-=500;
  StrRect.left = StrRect.bottom;
  StrRect.right = StrRect.left + 87;
  StrRect.bottom = StrRect.top + 19;

  DrawNumberString(pDC,m_lMeStatisticScore,m_UserScoreTall.x+60,m_UserScoreTall.y+4);
  //this->DrawPersonData(pDC,m_lMeStatisticScore,m_UserScoreTall.x,m_UserScoreTall.y);
}

//���Ƴ���
void CGameClientView::DrawJettonData(CDC *pDC,LONGLONG lNumber, INT nXPos,INT nYPos)
{
  if(m_ImgJettonNum.IsNull())
  {
    return;
  }
  //������Դ
  CSize SizeScoreNumber(m_ImgJettonNum.GetWidth()/11,m_ImgJettonNum.GetHeight());

  //������Ŀ
  LONGLONG lNumberCount=0;
  LONGLONG lNumberTemp=lNumber;
  do
  {
    lNumberCount++;
    lNumberTemp/=10;
  }
  while(lNumberTemp>0);

  //λ�ö���
  INT nOffset=8;
  INT nYDrawPos=nYPos-SizeScoreNumber.cy/2;
  INT nXDrawPos=nXPos-lNumberCount*SizeScoreNumber.cx/2+17;



  int nNumspace = 0;
  //�滭����
  for(LONGLONG i=0; i<lNumberCount; i++)
  {
    LONG lCellNumber=(LONG)(lNumber%10);
    nNumspace =((SizeScoreNumber.cx+1) * (lNumberCount - i)) + nXDrawPos;

    m_ImgJettonNum.DrawImage(pDC,
                             nNumspace,
                             nYDrawPos + 3,
                             SizeScoreNumber.cx,
                             SizeScoreNumber.cy ,
                             (lCellNumber)*SizeScoreNumber.cx,
                             0);

    //���ñ���
    lNumber/=10;
    //nXDrawPos-=SizeScoreNumber.cx;
  };
}

//���Ƴ���
void CGameClientView::DrawAllJettonData(CDC *pDC,LONGLONG lNumber, INT nXPos,INT nYPos)
{
  if(m_ImgAllJettonNum.IsNull())
  {
    return;
  }
  //������Դ
  CSize SizeScoreNumber(m_ImgAllJettonNum.GetWidth()/11,m_ImgAllJettonNum.GetHeight());

  //������Ŀ
  LONGLONG lNumberCount=0;
  LONGLONG lNumberTemp=lNumber;
  do
  {
    lNumberCount++;
    lNumberTemp/=10;
  }
  while(lNumberTemp>0);

  //λ�ö���
  INT nOffset=8;
  INT nYDrawPos=nYPos-SizeScoreNumber.cy/2;
  INT nXDrawPos=nXPos-lNumberCount*SizeScoreNumber.cx/2+17;

  int nNumspace = 0;
  //�滭����
  for(LONGLONG i=0; i<lNumberCount; i++)
  {
    LONG lCellNumber=(LONG)(lNumber%10);
    nNumspace =((SizeScoreNumber.cx+1) * (lNumberCount - i)) + nXDrawPos;

    m_ImgAllJettonNum.DrawImage(pDC,
                                nNumspace,
                                nYDrawPos + 3,
                                SizeScoreNumber.cx,
                                SizeScoreNumber.cy ,
                                (lCellNumber)*SizeScoreNumber.cx,
                                0);

    //���ñ���
    lNumber/=10;
    //nXDrawPos-=SizeScoreNumber.cx;
  };
}

//���Ƴ���
void CGameClientView::DrawJettonData_B(CDC *pDC,LONGLONG lNumber, INT nXPos,INT nYPos)
{
  if(m_ImgJettonNum_B.IsNull())
  {
    return;
  }
  //������Դ
  CSize SizeScoreNumber(m_ImgJettonNum_B.GetWidth()/10,m_ImgJettonNum_B.GetHeight());

  //������Ŀ
  LONGLONG lNumberCount=0;
  LONGLONG lNumberTemp=lNumber;
  do
  {
    lNumberCount++;
    lNumberTemp/=10;
  }
  while(lNumberTemp>0);

  //λ�ö���
  INT nOffset=8;
  INT nYDrawPos=nYPos-SizeScoreNumber.cy/2-3;
  INT nXDrawPos=nXPos-lNumberCount*SizeScoreNumber.cx/2+25;

  int nNumspace = 0;
  //�滭����
  for(LONGLONG i=0; i<lNumberCount; i++)
  {
    LONG lCellNumber=(LONG)(lNumber%10);
    nNumspace =((SizeScoreNumber.cx) * (lNumberCount - i)) + nXDrawPos;

    m_ImgJettonNum_B.DrawImage(pDC,
                               nNumspace,
                               nYDrawPos,
                               SizeScoreNumber.cx,
                               SizeScoreNumber.cy ,
                               (lCellNumber)*SizeScoreNumber.cx,
                               0);

    //���ñ���
    lNumber/=10;
    //nXDrawPos-=SizeScoreNumber.cx;
  };
}

//���Ƴ���
void CGameClientView::DrawAllJettonData_B(CDC *pDC,LONGLONG lNumber, INT nXPos,INT nYPos)
{
  if(m_ImgAllJettonNum_B.IsNull())
  {
    return;
  }
  //������Դ
  CSize SizeScoreNumber(m_ImgAllJettonNum_B.GetWidth()/10,m_ImgAllJettonNum_B.GetHeight());

  //������Ŀ
  LONGLONG lNumberCount=0;
  LONGLONG lNumberTemp=lNumber;
  do
  {
    lNumberCount++;
    lNumberTemp/=10;
  }
  while(lNumberTemp>0);

  //λ�ö���
  INT nYDrawPos=nYPos-SizeScoreNumber.cy/2+2;
  INT nXDrawPos=nXPos-lNumberCount*SizeScoreNumber.cx/2+25;

  int nNumspace = 0;
  //�滭����
  for(LONGLONG i=0; i<lNumberCount; i++)
  {
    LONG lCellNumber=(LONG)(lNumber%10);
    nNumspace =((SizeScoreNumber.cx) * (lNumberCount - i)) + nXDrawPos;

    m_ImgAllJettonNum_B.DrawImage(pDC,
                                  nNumspace,
                                  nYDrawPos + 3,
                                  SizeScoreNumber.cx,
                                  SizeScoreNumber.cy ,
                                  (lCellNumber)*SizeScoreNumber.cx,
                                  0);

    //���ñ���
    lNumber/=10;
    //nXDrawPos-=SizeScoreNumber.cx;
  };
}

//��������
void CGameClientView::DrawPersonData(CDC *pDC,LONGLONG lNumber, INT nXPos,INT nYPos)
{

  //������Դ
  CSize SizeScoreNumber(m_ImgPersonData_add.GetWidth()/11,m_ImgPersonData_add.GetHeight());

  if(lNumber < 0&& lNumber != 0)
  {
    SizeScoreNumber.SetSize(m_ImgPersonData_jian.GetWidth()/11,m_ImgPersonData_jian.GetHeight());
  }

  //λ�ö���
  INT nOffset=8;
  INT nYDrawPos=nYPos-SizeScoreNumber.cy/2;
  INT nXDrawPos=(INT)(nXPos) ;

  if(lNumber <0 && lNumber != 0)
  {
    //������Ŀ
    LONGLONG lNumberCount=0;
    LONGLONG lNumberTemp= (-lNumber);
    LONGLONG lNumberTemps = (-lNumber);
    do
    {
      lNumberCount++;
      lNumberTemp/=10;
    }
    while(lNumberTemp>0);
    //����һ��-��
    m_ImgPersonData_jian.DrawImage(pDC,
                                   nXDrawPos,
                                   nYDrawPos + 3,
                                   SizeScoreNumber.cx,
                                   SizeScoreNumber.cy,
                                   (0)*SizeScoreNumber.cx,
                                   0);

    int nNumspace = 0;
    //�滭����
    for(int i=0; i<lNumberCount; i++)
    {
      LONG lCellNumber=(LONG)(lNumberTemps%10);
      nNumspace = 77 + (SizeScoreNumber.cx * (lNumberCount - i)) + nXDrawPos -80+ 2;

      m_ImgPersonData_jian.DrawImage(pDC,
                                     nNumspace,
                                     nYDrawPos + 3,
                                     SizeScoreNumber.cx,
                                     SizeScoreNumber.cy ,
                                     (lCellNumber+1)*SizeScoreNumber.cx,
                                     0);

      //���ñ���
      lNumberTemps/=10;
      //nXDrawPos-=SizeScoreNumber.cx;
    };
  }
  else
  {
    //������Ŀ
    LONGLONG lNumberCount=0;
    LONGLONG lNumberTemp=lNumber;
    do
    {
      lNumberCount++;
      lNumberTemp/=10;
    }
    while(lNumberTemp>0);
    // ����һ��+��
    m_ImgPersonData_add.DrawImage(pDC,
                                  nXDrawPos,
                                  nYDrawPos + 3,
                                  SizeScoreNumber.cx,
                                  SizeScoreNumber.cy,
                                  (0)*SizeScoreNumber.cx,
                                  0);

    int nNumspace = 0;
    //�滭����
    for(LONGLONG i=0; i<lNumberCount; i++)
    {
      LONG lCellNumber=(LONG)(lNumber%10);
      nNumspace = 77 + (SizeScoreNumber.cx * (lNumberCount - i)) + nXDrawPos -80 + 2;

      m_ImgPersonData_add.DrawImage(pDC,
                                    nNumspace,
                                    nYDrawPos + 3,
                                    SizeScoreNumber.cx,
                                    SizeScoreNumber.cy ,
                                    (lCellNumber+1)*SizeScoreNumber.cx,
                                    0);

      //���ñ���
      lNumber/=10;
      //nXDrawPos-=SizeScoreNumber.cx;
    };
  }


  return;
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//���д��
void CGameClientView::OnBankStorage()
{
  //��ȡ�ӿ�
  CGameClientEngine *pGameClientEngine=CONTAINING_RECORD(this,CGameClientEngine,m_GameClientView);
  IClientKernel *pIClientKernel=(IClientKernel *)pGameClientEngine->m_pIClientKernel;

  if(NULL!=pIClientKernel)
  {
    CRect ClientRect;
    GetWindowRect(&ClientRect);
    CRect btRect;
    m_btBankStorage.GetWindowRect(&btRect);
    ShowInsureSave(pIClientKernel,CPoint((ClientRect.Width() - btRect.Width())/2 - 200,ClientRect.Height()/2 + 100));
  }
}

//����ȡ��
void CGameClientView::OnBankDraw()
{

  //��ȡ�ӿ�
  CGameClientEngine *pGameClientEngine=CONTAINING_RECORD(this,CGameClientEngine,m_GameClientView);
  IClientKernel *pIClientKernel=(IClientKernel *)pGameClientEngine->m_pIClientKernel;

  if(NULL!=pIClientKernel)
  {
    CRect ClientRect;
    GetWindowRect(&ClientRect);

    CRect btRect;
    m_btBankDraw.GetWindowRect(&btRect);
    ShowInsureGet(pIClientKernel,CPoint((ClientRect.Width() - btRect.Width())/2- 60,ClientRect.Height()/2 + 230));
  }


}


void CGameClientView::OnUp()
{


}
void CGameClientView::OnDown()
{

}


void CGameClientView::ButtonAddScore(BYTE cbJettonArea)
{
  if(cbJettonArea > AREA_COUNT)
  {
    ASSERT(FALSE);
    return;
  }

  if(m_lCurrentJetton==0)
  {
    return;
  }
  //�����ע
  LONGLONG lMaxJettonScore=GetUserMaxJetton(cbJettonArea);

  if((m_lAllJettonScore[cbJettonArea]+m_lCurrentJetton)>m_lAreaLimitScore)
  {
    return ;
  }


  //�Ϸ��ж�
  if(lMaxJettonScore < m_lCurrentJetton)
  {
    //SetJettonHide(0);
    ReduceJettonNumber();
    return ;
  }

  //��ע״̬
  m_bAddScoreStatue=true;

  SendEngineMessage(IDM_PLACE_JETTON,cbJettonArea,(LPARAM)(&m_lCurrentJetton));
}

//���ͳ���
void CGameClientView::ReduceJettonNumber()
{
  if(m_lCurrentJetton==5000000L)
  {
    m_lCurrentJetton=1000000;
    ResetButtonState();
  }
  else if(m_lCurrentJetton==1000000L)
  {
    m_lCurrentJetton=100000;
    ResetButtonState();
  }
  else if(m_lCurrentJetton==100000L)
  {
    m_lCurrentJetton=10000;
    ResetButtonState();
  }
  else if(m_lCurrentJetton==10000L)
  {
    m_lCurrentJetton=1000;
    ResetButtonState();
  }
  else if(m_lCurrentJetton==1000L)
  {
    m_lCurrentJetton=100;
    ResetButtonState();;
  }
  /*else
  {
    m_lCurrentJetton=0;
  }*/
  RefreshGameView();

}

//�ύ��ע
void CGameClientView::OnPlaceJetton()
{
  return;
}

void CGameClientView::StartHandle_Leave()
{
  SetTimer(IDI_HANDLELEAVE_ANIMATION,400,NULL);
  SendEngineMessage(IDM_SOUND,0,1);
}

//����Ա����
void CGameClientView::OpenAdminWnd()
{
  //��Ȩ��
  //if(m_pAdminControl!=NULL&&(GetUserInfo(m_wMeChairID)-> &UR_GAME_CONTROL)!=0)
  {
    if(!m_pAdminControl->IsWindowVisible())
    {
      m_pAdminControl->CenterWindow();
      CRect rect;
      m_pAdminControl->GetWindowRect(&rect);
      rect.left-=110;
      rect.right-=110;
      m_pAdminControl->MoveWindow(rect);
      m_pAdminControl->ClearText();
      m_pAdminControl->ShowWindow(SW_SHOW);
    }
    else
    {
      m_pAdminControl->ShowWindow(SW_HIDE);
    }
  }
}

//ִ��ʣ�����еĻ��嶯��
void CGameClientView::PerformAllBetAnimation()
{
  KillTimer(IDI_ANDROID_BET);
  for(int i = 0 ; i < m_ArrayAndroid.GetCount(); ++i)
  {
    KillTimer(IDI_ANDROID_BET + i + 1);
    PlaceUserJetton(m_ArrayAndroid[i].cbJettonArea, m_ArrayAndroid[i].lJettonScore);
  }
  m_ArrayAndroid.RemoveAll();
}

//������ͼ
void CGameClientView::RefreshGameView()
{
  CRect rect;
  GetClientRect(&rect);
  InvalidGameView(rect.left,rect.top,rect.Width(),rect.Height());

  return;
}

//���ü���
void CGameClientView::SetAnimalPercent(INT nAnimalPercent[AREA_ALL])
{
  memcpy(m_nAnimalPercent, nAnimalPercent, sizeof(m_nAnimalPercent));
  int nFirstValue=m_nAnimalPercent[0];
  for(int i=0; i<AREA_ALL-1; i++)
  {
    m_nAnimalPercent[i]=m_nAnimalPercent[i+1];
  }

  m_nAnimalPercent[AREA_ALL-2]=nFirstValue;

}
//�������㽱��
void CGameClientView::SetShaYuAddMulti(BYTE cbShaYuAddMulti)
{
  m_cbShaYuAddMulti = cbShaYuAddMulti;

  m_cbOneDigit = m_cbShaYuAddMulti/10;
  m_cbTwoDigit = m_cbShaYuAddMulti%10;
  m_cbRandOneDigit =  0;
  m_cbRandTwoDigit =  0;

}


//��ȡ����
CString CGameClientView::GetAnimalStrInfo(BYTE cbAllIndex)
{
  //����
  if(1==cbAllIndex||cbAllIndex==2||cbAllIndex==3)
  {
    return TEXT("����");
  }
  //����
  else if(5==cbAllIndex||cbAllIndex==6||cbAllIndex==7)
  {
    return TEXT("��");

  }
  //����
  else if(8==cbAllIndex||cbAllIndex==9||cbAllIndex==10)
  {
    return TEXT("����");
  }
  //��ȸ
  else if(12==cbAllIndex||cbAllIndex==13||cbAllIndex==14)
  {
    return TEXT("��ȸ");
  }
  //��ӥ
  else if(15==cbAllIndex||cbAllIndex==16||cbAllIndex==17)
  {
    return TEXT("��ӥ");
  }
  //ʨ��
  else if(19==cbAllIndex||cbAllIndex==20||cbAllIndex==21)
  {
    return TEXT("ʨ��");
  }
  //��è
  else if(22==cbAllIndex||cbAllIndex==23||cbAllIndex==24)
  {
    return TEXT("��è");
  }
  //����
  else if(26==cbAllIndex||cbAllIndex==27||cbAllIndex==28)
  {
    return TEXT("����");
  }
  //����
  else
  {
    return TEXT("����");
  }
  return TEXT("");
}