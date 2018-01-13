#ifndef GAME_CLIENT_VIEW_HEAD_FILE
#define GAME_CLIENT_VIEW_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "CardControl.h"
#include "RecordGameList.h"
#include "ApplyUserList.h"
#include "../游戏服务器/GameLogic.h"
#include "ClientControl.h"
#include "flashpaly\ShowLotteryFlash.h"
//////////////////////////////////////////////////////////////////////////
//动画
enum enCartoonIds
{
	CTN_CHANGE_BANKER = 0,												//轮换庄家
	CTN_BANKER_MAX,
	CTN_RESULT_FLASH,													//输赢选择框闪烁
	CTN_OPEN_CARD,														//搓牌
	CTN_POST_CARD,														//发牌
	CTN_DISPATCH_CARD,													//发牌标识
	CTN_DISPATCH_TIP,													//发牌提示
	CTN_STAR_SELECT,													//明星选择	
	CTN_FLASH_WAIT,														//FLASH动画等待  
	CTN_FLASH_EFFECT,													//FLASH动画  
	CTN_COUNT,
};
struct stuCartoonInfo
{
	bool							bShow;							//是否动画
	int								nShowFrame;						//动画桢数
	DWORD							dwShowTime;						//上桢动画时间
	int								nShowTimer;						//刷新间隔（毫秒）
};
const conCartoonTimer[CTN_COUNT] = {30,30,500,50,500,30,2000,500,4000,40};

//筹码定义
#define JETTON_COUNT				6									//筹码数目
#define JETTON_RADII				68									//筹码半径

//消息定义
#define IDM_PLACE_JETTON			WM_USER+200							//加住信息
#define IDM_APPLY_BANKER			WM_USER+201							//申请信息
#define IDM_CONTINUE_CARD			WM_USER+202							//继续发牌
#define IDM_AUTO_OPEN_CARD			WM_USER+203							//按钮标识
#define IDM_OPEN_CARD				WM_USER+204		
#define IDM_MANAGE_CONTROL			WM_USER+205							//申请信息
#define IDM_OPEN_SOUND			    WM_USER+206							//按钮标识
#define IDM_CLOSE_SOUND				WM_USER+207		
#define IDM_POSTCARD_SOUND          WM_USER+208	                        //发牌完成
#define IDM_START				    WM_USER+209				        	//开始消息


//索引定义
#define INDEX_PLAYER				0									//闲家索引
#define INDEX_BANKER				1									//庄家索引

//////////////////////////////////////////////////////////////////////////
//结构定义

//历史记录
//#ifdef _DEBUG
//#define MAX_SCORE_HISTORY			18									//历史个数
//#else
//#define MAX_SCORE_HISTORY			65									//历史个数
//#endif

#define MAX_SCORE_HISTORY			65									//历史个数

#define MAX_FALG_COUNT				19									//标识个数

//筹码信息
struct tagJettonInfo
{
	int								nXPos;								//筹码位置
	int								nYPos;								//筹码位置
	BYTE							cbJettonIndex;						//筹码索引
};

//操作结果
enum enOperateResult
{
	enOperateResult_NULL,
	enOperateResult_Win,
	enOperateResult_Lost
};

//记录信息
struct tagClientGameRecord
{
	enOperateResult					enOperateShunMen;					//操作标识
	enOperateResult					enOperateDuiMen;					//操作标识
	enOperateResult					enOperateDaoMen;					//操作标识
	BYTE							bWinShunMen;						//顺门胜利
	BYTE							bWinDuiMen;							//对门胜利
	BYTE							bWinDaoMen;							//倒门胜利
	BYTE							bWinShunMenSame;						//顺门胜利
	BYTE							bWinDuiMenSame;							//对门胜利
	BYTE							bWinDaoMenSame;							//倒门胜利
};

//发牌提示
enum enDispatchCardTip
{
	enDispatchCardTip_NULL,
	enDispatchCardTip_Continue,											//继续发牌
	enDispatchCardTip_Dispatch											//重新洗牌
};

//筹码数组
typedef CWHArray<tagJettonInfo,tagJettonInfo&> CJettonInfoArray;

//累声明
class CGameClientEngine;
//////////////////////////////////////////////////////////////////////////

//游戏视图
class CGameClientView : public CGameFrameViewD3D//CGameFrameViewGDI
{
	//限制信息
protected:
	LONGLONG						m_lMeMaxScore;						//最大下注
    LONGLONG						m_lAreaLimitScore;					//区域限制
	LONGLONG						m_lUserLimitScore;					//限制下注

	//下注信息
protected:
	LONGLONG						m_lUserJettonScore[AREA_ARRY_COUNT];	//个人总注
	LONGLONG						m_lAllJettonScore[AREA_ARRY_COUNT];	//全体总注
	LONGLONG						m_lAllPlayBet[GAME_PLAYER][AREA_ARRY_COUNT];//所有玩家下注

	//位置信息
protected:
	int								m_nWinFlagsExcursionX;				//偏移位置
	int								m_nWinFlagsExcursionY;				//偏移位置
	int								m_nScoreHead;						//成绩位置
	CRect							m_rcShunMen;						//闲家区域
	CRect							m_rcJiaoL;							//闲天王区域
	CRect							m_rcQiao;							//对子区域
	CRect							m_rcDuiMen;							//平区域
	CRect							m_rcDaoMen;							//同点平区域
	CRect							m_rcJiaoR;							//庄家区域
	int								m_OpenCardIndex;					//開牌順序
	int								m_PostCardIndex;					//發牌順序
	int								m_PostStartIndex;					//发牌起始位置
	

	//扑克信息
public:	
    BYTE							m_cbTableCardArray[4][2];			//桌面扑克
	bool							m_blMoveFinish;
	BYTE							m_bcfirstShowCard;
	BYTE							m_bcShowCount;
	bool							m_blAutoOpenCard;					//手自动
	bool							m_bBankerMax;						//是否达到庄家上限
	bool                            m_bShowHalf;
	int								m_nRecordLast;						//最后记录
	bool                            m_bToBankerMax;

	//历史信息
protected:
	LONGLONG						m_lMeStatisticScore;				//游戏成绩
	tagClientGameRecord				m_GameRecordArrary[MAX_SCORE_HISTORY];//游戏记录
	int								m_nRecordFirst;						//开始记录

	//状态变量
protected:
	WORD							m_wMeChairID;						//我的位置
	BYTE							m_cbAreaFlash;						//胜利玩家
	LONGLONG						m_lCurrentJetton;					//当前筹码
	bool							m_bShowChangeBanker;				//轮换庄家
	bool							m_bNeedSetGameRecord;				//完成设置
	BYTE							m_bWinShunMen;						//胜利标识
	BYTE							m_bWinDuiMen;						//胜利标识
	BYTE							m_bWinDaoMen;						//胜利标识	
	bool							m_bShowGameResult;					//显示结果
	enDispatchCardTip				m_enDispatchCardTip;				//发牌提示
public:
	//动画
	stuCartoonInfo					m_stuCartoonInfo[CTN_COUNT];

	//庄家信息
protected:	
	WORD							m_wBankerUser;						//当前庄家
	WORD							m_wBankerTime;						//做庄次数
	LONGLONG						m_lBankerScore;						//庄家积分
	LONGLONG						m_lBankerWinScore;					//庄家成绩	
	LONGLONG						m_lTmpBankerWinScore;				//庄家成绩	
	bool							m_bEnableSysBanker;					//系统做庄

	//当局成绩
public:
	LONGLONG						m_lMeCurGameScore;					//我的成绩
	LONGLONG						m_lMeCurGameReturnScore;			//我的成绩
	LONGLONG						m_lBankerCurGameScore;				//庄家成绩
	LONGLONG						m_lGameRevenue;						//游戏税收

	//数据变量
protected:
	CPoint							m_PointJetton[JETTON_COUNT];			//筹码位置
	CPoint							m_PointJettonNumber[JETTON_COUNT];	//数字位置
	CJettonInfoArray				m_JettonInfoArray[JETTON_COUNT];		//筹码数组
	COLORREF                        m_CurSelectColor;					//当前颜色
	//控件变量
public:
	CVirtualButton					m_btJetton100;						//筹码按钮
	CVirtualButton					m_btJetton1000;						//筹码按钮
	CVirtualButton					m_btJetton10000;					//筹码按钮
	CVirtualButton					m_btJetton100000;					//筹码按钮
	CVirtualButton					m_btJetton500000;					//筹码按钮
	CVirtualButton					m_btJetton1000000;					//筹码按钮
	CVirtualButton					m_btJetton5000000;					//筹码按钮
	CVirtualButton					m_btJetton10000000;					//筹码按钮
	CVirtualButton					m_btApplyBanker;					//申请庄家
	CVirtualButton					m_btCancelBanker;					//取消庄家
	CVirtualButton					m_btScoreMoveL;						//移动成绩
	CVirtualButton					m_btScoreMoveR;						//移动成绩
	CVirtualButton					m_btAutoOpenCard;					//自动开牌
	CVirtualButton					m_btOpenCard;						//手动开牌
	CVirtualButton					m_btBank;							//银行按钮
	CVirtualButton					m_btContinueCard;					//继续发牌
	CVirtualButton				    m_btBankerStorage;					//存款按钮
	CVirtualButton					m_btBankerDraw;						//取款按钮
	//CVirtualButton                  m_btStart;                          //开始按钮
	CVirtualButton					m_btUp;					            //存款按钮
	CVirtualButton					m_btDown;
	CButton							m_btOpenAdmin;                      //管理员控制

	CD3DTexture						m_btTexJetton1000;					//筹码按钮
	CD3DTexture						m_btTexJetton10000;					//筹码按钮
	CD3DTexture						m_btTexJetton100000;					//筹码按钮
	CD3DTexture						m_btTexJetton500000;					//筹码按钮
	CD3DTexture						m_btTexJetton1000000;					//筹码按钮
	CD3DTexture						m_btTexJetton5000000;					//筹码按钮
	CD3DTexture						m_btTexJetton10000000;				//筹码按钮

	//控件变量
public:
	CApplyUser						m_ApplyUser;						//申请列表
	CGameRecord						m_GameRecord;						//记录列表
	CCardControl					m_CardControl[5];					//扑克控件	
	CGameClientEngine				*m_pGameClientEngine;				//父类指针
	CGameLogic						m_GameLogic;						//游戏逻辑
	bool							m_blCanStore;						//是否能保存
	
public:
	//顺对倒底图区域
	CRect							m_rcShunMenBack[3];					//顺对倒区域
	//
	CRect							m_rcBack;
	CD3DTexture						m_TexApplyBlockBack;				//上庄列表滑块底
	CD3DTexture						m_TexApplyBlock;					//上庄列表滑块图
	CRect							m_rcApplyBlock;						//上庄列表滑块
	int								m_nApplyFocusIndex;					//
	bool							m_bApplyLBDown;						//是否左键在滑块上按下
	CD3DTexture						m_TexScoreBack;						//下注底图
	//选美女
	CD3DTexture						m_TexGod;							//财神
	CD3DTexture						m_TexProgress;						//进度条
	CD3DTexture						m_TexProgressBack;					//进度条底
	CD3DTexture						m_TexProgressNum;					//进度条数字
	CD3DTexture						m_TexStar;							//明星全图
	CD3DTexture						m_TexStarShow[3];					//明星选择
	BYTE							m_cbStarSelect[3];
	float							m_fStarWinRate[STAR_COUNT];
	LONGLONG						m_llStarAllCount[STAR_COUNT];
	LONGLONG						m_llStarWinCount[STAR_COUNT];
	LONGLONG						m_llAllStarCount;
	bool							m_bSelectStar;
	//Falsh变量
	CShowLotteryFlash2				m_ShowLotteryFlash;
	CShowLotteryFlash2				m_ShowLotteryFlashArray[2];
	bool							m_bFlashShow;						//flash动画
	WORD							m_wFlashId;
	CPoint							m_ptGodBack;						//财神位置
	CPoint                          m_ptFlash[2];						//0财神到1筛元宝

	//控制
public:
	HINSTANCE						m_hInst;
	IClientControlDlg*				m_pClientControlDlg;		

	//界面变量
public:
	CD3DDevice *					m_pD3DDevice;
protected:	
	CD3DTexture						m_TexViewFill;						//背景位图
	CD3DTexture						m_TexViewBack;						//背景位图
	CD3DTexture						m_TexWinFlags;					//标志位图
	CD3DTexture						m_TexJettonView;					//筹码视图
	CD3DTexture						m_TexScoreNumber;					//数字视图
	CD3DTexture						m_TexMeScoreNumber;				//数字视图
	CD3DTexture						m_TexTimeFlag;					//时间标识
	CD3DTexture						m_TexDispatchCardTip;				//发牌提示
	CD3DTexture						m_TexPastHalf;
	CD3DTexture						m_TexBankerBomb;
	CD3DTexture						m_TextureDispatchCard;				//资源变量

	struct TotoalPress
	{
		CD3DTexture						Tex_tatoalflag;
		CD3DTexture						Tex_tipC;
		CD3DTexture						Tex_tipN;
		
	};


	CD3DTexture						m_TexPngScoreNumber;					//数字视图
	CD3DTexture						m_TexPngScoreNumber1;					//数字视图

	TotoalPress                     m_TotoalPress;               

	//边框资源
protected:
	CD3DTexture						m_TexFrameShunMen;				//边框图片
	CD3DTexture						m_TexFrameDuiMen;					//边框图片
	CD3DTexture						m_TexFrameDaoMen;					//边框图片
	CD3DTexture						m_TexFocusShunMen;				//边框图片
	CD3DTexture						m_TexFocusDuiMen;					//边框图片
	CD3DTexture						m_TexFocusDaoMen;					//边框图片
	CD3DTexture						m_TexMeBanker;					//切换庄家
	CD3DTexture						m_TexChangeBanker;				//切换庄家
	CD3DTexture						m_TexNoBanker;					//切换庄家
	CD3DTexture						m_TexTimerNumber;

	CD3DTexture						m_TexTimer;						//切换庄家	
	CD3DTexture						m_TexBankerMax;					//

	//结束资源
protected:
	CD3DTexture						m_TexGameEnd;						//成绩图片

	//结束资源
public:
	CListCtrl						m_SkinListCtrl[3];

	//函数定义
public:
	//构造函数
	CGameClientView();
	//析构函数
	virtual ~CGameClientView();


	//继承函数
private:
	//重置界面
	virtual VOID ResetGameView();
	//调整控件
	virtual VOID RectifyControl(INT nWidth, INT nHeight);
	//绘画界面
	//virtual VOID DrawGameView(CDC * pDC, INT nWidth, INT nHeight);
	//WIN7支持
	virtual bool RealizeWIN7() { return false; }
	//关闭窗口权限
	virtual bool GetCloseFrame(TCHAR szInfo[]);

	//CGameFrameViewD3D界面函数
private:
	//动画驱动
	virtual VOID CartoonMovie();
	//配置界面
	virtual VOID InitGameView(CD3DDevice * pD3DDevice, INT nWidth, INT nHeight);
	//绘画界面
	virtual VOID DrawGameView(CD3DDevice * pD3DDevice, INT nWidth, INT nHeight);

	//设置函数
public:
	//设置信息
	void SetMeMaxScore(LONGLONG lMeMaxScore);
	//设置信息
	void SetUserLimitScore(LONGLONG lUserLimitScore);
	//我的位置
	void SetMeChairID(DWORD dwMeUserID);
	//个人下注
	void SetMePlaceJetton(BYTE cbViewIndex, LONGLONG lJettonCount);
	//庄家信息
	void SetBankerInfo(DWORD dwBankerUserID, LONGLONG lBankerScore);
	//庄家成绩
	void SetBankerScore(WORD wBankerTime, LONGLONG lWinScore) {m_wBankerTime=wBankerTime; m_lTmpBankerWinScore=lWinScore;}
	//当局成绩
	void SetCurGameScore(LONGLONG lMeCurGameScore, LONGLONG lMeCurGameReturnScore, LONGLONG lBankerCurGameScore, LONGLONG lGameRevenue);
	//设置筹码
	void PlaceUserJetton(BYTE cbViewIndex, LONGLONG lScoreCount);
	//区域限制
	void SetAreaLimitScore(LONGLONG lAreaLimitScore);	
	//设置扑克
	void SetCardInfo(BYTE cbTableCardArray[4][2]);
	//设置筹码
	void SetCurrentJetton(LONGLONG lCurrentJetton);
	//历史记录
	void SetGameHistory(BYTE bWinShunMen, BYTE bWinDaoMen, BYTE bWinDuiMen);
	//轮换庄家
	void ShowChangeBanker( bool bChangeBanker );
	//成绩设置
	void SetGameScore(LONGLONG lMeCurGameScore, LONGLONG lMeCurGameReturnScore, LONGLONG lBankerCurGameScore);
	//允许系统做庄
	void EnableSysBanker(bool bEnableSysBanker) {m_bEnableSysBanker=bEnableSysBanker;}
	//设置提示
	void SetDispatchCardTip(enDispatchCardTip DispatchCardTip);

	void SetFirstShowCard(BYTE bcCard);

	void ClearBank();
	//
	void SetBankerAllResult(LONGLONG lAllWinScore){m_lBankerWinScore = lAllWinScore;}
	//切换底图
	void TransportBackRes(int nRedBackRes);

	//动画函数
public:	
	//开始发牌
	void DispatchCard();
	//结束发牌
	void FinishDispatchCard();
	//设置爆炸
	bool SetBombEffect(bool bBombEffect, WORD wAreaIndex);

	//计算限制
public:
	//最大下注
	LONGLONG GetUserMaxJetton();

	//绘画函数
protected:
	//闪烁边框
	void FlashJettonAreaFrame(int nWidth, int nHeight, CD3DDevice * pD3DDevice);

	//界面函数
public:
	//清理筹码
	void CleanUserJetton();
	//设置胜方
	void SetWinnerSide(BYTE bWinShunMen, BYTE bWinDuiMen, BYTE bWinDaoMen, bool bSet);
	//艺术字体
	//void DrawTextString(CD3DDevice * pD3DDevice, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, int nXPos, int nYPos);
	//绘画数字
	void DrawMeJettonNumber(CD3DDevice * pD3DDevice);
	//绘画庄家
	void DrawBankerInfo(CD3DDevice * pD3DDevice,int nWidth,int nHeight);
	//绘画玩家
	void DrawMeInfo(CD3DDevice * pD3DDevice,int nWidth,int nHeight);

	//内联函数
public:
	//当前筹码
	inline LONGLONG GetCurrentJetton() { return m_lCurrentJetton; }	
	
	void SetMoveCardTimer();
	//接收场景时结束发牌
	void SetFinishDiscard();

	//内部函数
private:
	//获取区域
	BYTE GetJettonArea(CPoint MousePoint);
	//绘画数字
	void DrawNumberString(CD3DDevice * pD3DDevice, LONGLONG lNumber, INT nXPos, INT nYPos, bool bMeScore = false,bool blTimer=false);
	//绘画数字(nLeft=0左1中2右,nTop=0上1中2下)
	void DrawNumberStringWithPng(CD3DDevice * pD3DDevice,CD3DTexture* pTexNumBack, LONGLONG lNumber, INT nXPos, INT nYPos, bool bMeScore = false,int nLeft=0,int nTop=0);
	//绘画数字(nLeft=0左1中2右,nTop=0上1中2下)
	void DrawNumberStringWithSign(CD3DDevice * pD3DDevice,CD3DTexture* pTexNumBack, LONGLONG lNumber, INT nXPos, INT nYPos,
		bool bSign=true,int nFrame = 11,int nLeft=0,int nTop=0);
	//绘画数字
	void DrawNumberStringWithSpace(CD3DDevice * pD3DDevice, LONGLONG lNumber, INT nXPos, INT nYPos);
	//绘画数字
	void DrawNumberStringWithSpace(CD3DDevice * pD3DDevice, LONGLONG lNumber, CRect rcRect, DWORD dwClr,INT nFormat=-1);
	//绘画标识
	void DrawWinFlags(CD3DDevice * pD3DDevice);
	//显示结果
	void ShowGameResult(CD3DDevice * pD3DDevice, int nWidth, int nHeight);
	//透明绘画
	bool DrawAlphaRect(CDC* pDC, LPRECT lpRect, COLORREF clr, FLOAT fAlpha);
	//推断赢家
	void DeduceWinner(BYTE &bWinShunMen, BYTE &bWinDuiMen, BYTE &bWinDaoMen);

	void DrawJettonPress(CD3DDevice * pD3DDevice,int iWidth,int iHeight,LONGLONG lTotal,LONGLONG lHaveJetton);

	void DrawGameTimer(CD3DDevice * pD3DDevice,int iWidth,int iHeight,int iTimer);

	LONGLONG GetBankMaxJetton();

public:
	//控制下注检测
	void ControlBetDetection(BYTE cbViewIndex, LONGLONG lScoreCount, WORD wChair );

public:
	//播放Falsh
	void StartShowCard(WORD wFalshID,int nXPos,int nYPos);
	//移动Flash
	void MoveFlash(WORD wFalshID);
	//移动Flash
	void MoveFlashEx(WORD wFalshID);
	//播放Falsh
	void StartShowCardEx(WORD wFalshID,int nXPos,int nYPos,bool bShow);

	//按钮消息
protected:
	//开始按钮
	//afx_msg void OnStart();
	//上庄按钮
	afx_msg void OnApplyBanker();
	//下庄按钮
	afx_msg void OnCancelBanker();
	//移动按钮
	afx_msg void OnScoreMoveL();
	//移动按钮
	afx_msg void OnScoreMoveR();
	//手工搓牌
	afx_msg void OnOpenCard();
	//自动搓牌
	afx_msg void OnAutoOpenCard();

	afx_msg void OnOpenSound();

	afx_msg void OnCloseSound();
	//银行存款
	afx_msg void OnBankStorage();
	//银行取款
	afx_msg void OnBankDraw();
	//上翻
	afx_msg void OnUp();
	//下翻
	afx_msg void OnDown();
	//管理员控制
	afx_msg void OpenAdminWnd();
	//输赢控制
	afx_msg void OnControl();

	//消息映射
protected:
	//定时器消息
	afx_msg void OnTimer(UINT nIDEvent);
	//建立函数
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//鼠标消息
	afx_msg void OnLButtonDown(UINT nFlags, CPoint Point);	
	//鼠标消息
	afx_msg void OnRButtonDown(UINT nFlags, CPoint Point);	
    //鼠标消息
	afx_msg void OnLButtonUp(UINT nFlags, CPoint Point);
	//光标消息
	afx_msg BOOL OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage);
	//控件命令
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
    //鼠标移动
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//其他控件鼠标UP消息
	LRESULT OnViLBtUp(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnPlayFlashEnd(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnPlayFlashEndEx(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()	
};

//////////////////////////////////////////////////////////////////////////

#endif
