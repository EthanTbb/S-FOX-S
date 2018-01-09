#pragma once

#include "Stdafx.h"
#include "CardControl.h"
#include "HistoryRecord.h"
#include "ClientControl.h"

//////////////////////////////////////////////////////////////////////////

//消息定义
#define IDM_HINT_OX         WM_USER+99              //提示消息
#define IDM_START         WM_USER+100             //开始消息
#define IDM_READY         WM_USER+101             //摊牌消息
#define IDM_OX            WM_USER+102             //牛牛消息
#define IDM_MAX_SCORE       WM_USER+103             //最大加注
#define IDM_SEND_CARD_FINISH    WM_USER+104             //发牌完成
#define IDM_SHOW_VALUE        WM_USER+105             //点数信息
#define IDM_SHOW_INFO       WM_USER+106             //提示信息
#define IDM_OX_ENABLE       WM_USER+107             //牛牛按钮
#define IDM_SORT          WM_USER+108             //扑克分类
#define IDM_RESORT          WM_USER+109             //重排消息

#define MY_VIEW_CHAIRID       3                 //椅子位置

#define DRAW_HEIGHT         34                  //绘画高度
/////////////////////////////////////////////////////////////////////////
//结构定义

//筹码状态
struct tagJettonStatus
{
  //属性信息
  WORD              wMoveCount;             //移动次数
  WORD              wMoveIndex;             //移动索引

  //筹码信息
  CPoint              ptFrom;               //出发位置
  CPoint              ptDest;               //目的位置
  CPoint              ptCourse;             //过程位置
  LONGLONG            lGold;                //筹码数目

  //移动形式
  INT               iMoveType;              //移动形式
};

//发牌子项
struct tagSendCardItem
{
  WORD              wChairID;             //发牌用户
  BYTE              cbCardData;             //发牌数据
};

//数组说明
typedef CWHArray<tagSendCardItem,tagSendCardItem &> CSendCardItemArray;

//////////////////////////////////////////////////////////////////////////

//游戏视图
class CGameClientView : public CGameFrameViewGDI
{
  //数据变量
protected:
  BYTE              m_cbIndex;              //牛牛表情
  WORD              m_wBankerUser;            //庄家用户
  LONGLONG            m_lCellScore;           //游戏底分
  LONGLONG            m_lTableScore[GAME_PLAYER];     //下注数目
  BYTE              m_cbOxValue[GAME_PLAYER];     //牛牛数据
  BYTE              m_cbOpenCardUser[GAME_PLAYER];    //摊牌用户
  BYTE                            m_cbDynamicJoin;                    //动态加入
  bool              m_bInvest;              //等待下注
  bool              m_bShowSocre[GAME_PLAYER];      //显示积分
  bool              m_bUserAction;            //等待标志
  bool              m_bJettonAction;          //移动标示
  tagJettonStatus         m_JettonStatus[GAME_PLAYER];    //筹码信息
  LONGLONG            m_lMoveGold[GAME_PLAYER];     //筹码信息

  //牌型数据
public:
  bool              m_bShow;              //显示牌型
  bool              m_bHandOpen;            //自动搓牌
  bool              m_bLookOnUser;            //旁观标志
  bool              m_bOpenCard;            //等待摊牌
  bool              m_bShowScore;           //积分状态
  bool              m_bShowCellScore;         //积分状态
  tagHistoryScore *       m_pHistoryScore[GAME_PLAYER];   //积分信息

  //动画变量
protected:
  WORD              m_wSendCount;           //移动次数
  WORD              m_wSendIndex;           //移动次数
  CPoint              m_ptSendCardPos;          //发牌位置
  CPoint              m_ptSendEndingPos[GAME_PLAYER];   //桌牌位置
  CPoint              m_ptSendStartPos;         //始发位置
  CSendCardItemArray        m_SendCardItemArray;        //发牌数组

  //灯光动画
protected:
  bool              m_bShowCartoon;           //显示标识
  WORD              m_wCartoonLightIndex;       //动画索引

  //按钮控件
public:
  CSkinButton           m_btHintOx;             //提示按钮
  CSkinButton           m_btStart;              //开始按钮
  CSkinButton           m_btOpenCard;           //摊牌按钮
  CSkinButton           m_btOx;               //牛牛按钮
  CSkinButton           m_btHandOpenCard;         //手动搓牌
  CSkinButton           m_btAutoOpenCard;         //自动搓牌
  CSkinButton           m_btScore;              //看分按钮

  //控件变量
public:
  CCardControl          m_CardControl[GAME_PLAYER];     //用户扑克
  CCardControl          m_CardControlOx[GAME_PLAYER];   //用户扑克

  //客户端控制
public:
  bool              m_bCheck;             //控制标识
  CRect             m_rcCheck;              //响应区域
  HINSTANCE           m_hInst;              //实例句柄
  IClientControlDlg*        m_pClientControlDlg;        //接口指针
  CButton             m_btOpenAdmin;            //系统控制

  //位置信息
protected:
  CPoint              m_ptBanker[GAME_PLAYER];      //庄家位置
  CPoint              m_ptJeton[GAME_PLAYER];       //金币位置
  CPoint              m_ptValue[GAME_PLAYER];       //点数位置
  CPoint              m_ptOpenTag[GAME_PLAYER];     //摊牌位置
  CPoint              m_ptCard[GAME_PLAYER];        //金币位置
  CSize             m_sizeHistory;            //积分大小

  //界面变量
protected:
  CBitImage           m_ImageCard;            //扑克资源
  CBitImage           m_ImageViewBack;          //背景资源
  CBitImage           m_ImageViewCenter;          //背景资源
  CBitImage           m_ImageJeton;           //筹码资源
  CBitImage           m_ImageOpenCard;          //摊牌资源
  CPngImage           m_ImageOxValueZero;         //牛值资源
  CPngImage           m_ImageDoulbeOx;          //牛牛资源
  CPngImage           m_ImageNumber;            //数字资源
  CPngImage           m_ImageOxValue;           //牛值资源
  CPngImage             m_ImageTimeBack;          //时间背景
  CPngImage             m_ImageTimeNumber;          //时间数字
  CPngImage           m_ImageBanker;            //庄家资源
  CPngImage           m_PngUserBackL;           //玩家底框
  CPngImage           m_PngUserBackR;           //玩家底框
  CPngImage           m_PngUserBackMe;          //玩家底框
  CPngImage           m_PngAutoOpen;            //自动开牌
  CPngImage           m_PngWaitOpen;            //等待开牌
  CPngImage           m_PngOpenTag;           //开牌标示
  CPngImage                       m_PngReady;                         //准备标识
  CPngImage           m_PngCartoonLight;          //闪动动画
  CPngImage           m_PngHistoryScore;          //历史积分
  CPngImage           m_PngScoreBack;           //底分背景

  //彩金动画
public:
  WORD              m_wBonusCartoonIndex;
  CPngImage                       m_PngBonus;                         //彩金动画
  CPngImageEx                     m_PngBonusNum;                      //彩金动画
  bool              m_bShowBonusCartoon;
  bool              m_bBonus[GAME_PLAYER];
  CPoint              m_ptBonus[GAME_PLAYER];
  LONGLONG            m_lBonus;

  //函数定义
public:
  //构造函数
  CGameClientView();
  //析构函数
  virtual ~CGameClientView();
  //消息解释
  virtual BOOL PreTranslateMessage(MSG * pMsg);

  //重载函数
protected:
  //命令函数
  virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);

  //继承函数
private:
  //重置界面
  virtual VOID ResetGameView();
  //调整控件
  virtual VOID RectifyControl(int nWidth, int nHeight);
  //绘画界面
  virtual VOID DrawGameView(CDC * pDC, int nWidth, int nHeight);

  //WIN7支持
public:
  virtual bool RealizeWIN7()
  {
    return true;
  }

  //功能函数
public:
  //绘画时间
  void DrawUserTimerEx(CDC * pDC, int nXPos, int nYPos, WORD wTime, WORD wTimerArea=99);
  // 绘画数字
  void DrawNumber(CDC * pDC, CPngImageEx* ImageNumber, TCHAR * szImageNum, LONGLONG lOutNum, INT nXPos, INT nYPos, UINT uFormat = DT_RIGHT);
  // 绘画数字
  void DrawNumber(CDC * pDC, CPngImageEx* ImageNumber, TCHAR * szImageNum, TCHAR* szOutNum ,INT nXPos, INT nYPos,  UINT uFormat = DT_RIGHT);
  //提示信息
  void SetUserAction(bool bWait);
  //动画判断
  bool IsDispatchCard();
  //完成发牌
  void FinishDispatchCard();
  //发送扑克
  void DispatchUserCard(WORD wChairID, BYTE cbCardData);
  //设置下注
  void SetUserTableScore(WORD wChairID, LONGLONG lTableScore,bool bMove=false);
  //庄家标志
  void SetBankerUser(WORD wBankerUser);
  //等待标志
  void SetWaitInvest(bool Invest);
  //设置动态加入标志
  void SetUserDynamicJoin(BYTE cbDynamicJoin)
  {
    m_cbDynamicJoin=cbDynamicJoin;
  };
  //显示牌型
  void DisplayTypeEx(bool bShow);
  //设置点数
  void SetUserOxValue(WORD wChiarID,BYTE cbValue);
  //获取点数
  BYTE GetValue(WORD wChiarID)
  {
    return m_cbOxValue[wChiarID];
  }
  //显示摊牌
  void ShowOpenCard(WORD wChiarID,BOOL bShow=TRUE);
  //显示积分
  void ShowScore(WORD wChiarID,bool bShow);
  //显示积分
  void ShowCartonnLight(bool bShow);
  //显示底分
  void ShowCellScore(LONGLONG lScore, bool bShow);
  //艺术字体
  void DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, int nXPos, int nYPos);
  //艺术字体
  void DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, LPRECT lpRect);
  //积分按钮
  VOID OnBnClickedScore();
  //自动搓牌
  VOID OnBnClickedAutoOpen();
  //手动搓牌
  VOID OnBnClickedHandOpen();
  //管理员控制
  VOID OnBnClickedAdminWnd();
  //设置权限
  VOID SetAdminCheck(bool bCheck);
  //设置积分
  bool SetHistoryScore(WORD wChairID,tagHistoryScore * pHistoryScore);
  //绘制动画
  void DrawMoveAnte(WORD wViewChairID, LONGLONG lTableScore);
  //移动筹码
  bool MoveJetton();
  //绘制用户图像
  void DrawUserReadyEx(CDC *pDC,int nXPos,int nYPos);
  //更新视图
  void RefreshGameView();
  //更新视图
  void RefreshGameView(CRect &rect);
  //开始彩金动画
  void StartBonusCartoon();
  //设置动画标志
  void SetBonusFlag(WORD wViewChairID, bool bBonus);
  //设置彩金
  void SetBonus(LONGLONG lBonus)
  {
    if(lBonus != m_lBonus)
    {
      m_lBonus = lBonus;
    }
  }
  //消息映射
protected:
  //建立函数
  afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
  //左键消息
  afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
  //定时器消息
  afx_msg void OnTimer(UINT nIDEvent);

  DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////
