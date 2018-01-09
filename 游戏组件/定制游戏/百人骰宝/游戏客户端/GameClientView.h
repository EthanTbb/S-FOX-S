#ifndef GAME_CLIENT_VIEW_HEAD_FILE
#define GAME_CLIENT_VIEW_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "CardControl.h"
#include "RecordGameList.h"
#include "ApplyUserList.h"
#include "..\游戏服务器\GameLogic.h"
#include "AnimationWnd.h"
#include "ClientControl.h"
#include "ChatEdit.h"
#include "SkinListCtrl.h"
//////////////////////////////////////////////////////////////////////////

//筹码定义
#define JETTON_RADII				68									//筹码半径

//消息定义
#define IDM_PLACE_JETTON			WM_USER+200							//加住信息
#define IDM_APPLY_BANKER			WM_USER+201							//申请信息
#define IDM_CONTINUE_CARD			WM_USER+202							//继续发牌
#define IDM_AUTO_OPEN_CARD			WM_USER+203							//按钮标识
#define IDM_OPEN_CARD				WM_USER+204		
#define IDM_CHAT_MESSAGE			WM_USER+205							//游戏聊天消息
#define IDM_GAME_CLOSE				(WM_USER+206)						//游戏关闭

#define IDM_ADMIN_COMMDN			WM_USER+1000

//索引定义
#define INDEX_PLAYER				0									//闲家索引
#define INDEX_BANKER				1									//庄家索引

//////////////////////////////////////////////////////////////////////////
//结构定义

//历史记录
#define MAX_SCORE_HISTORY			64									//历史个数
#define MAX_FALG_COUNT				63									//标识个数

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


struct tagUserBetInfo
{
	BYTE cbAreaID;
	INT	 nChipsIndex;
};

//客户端机器人下分
struct tagAndroidBet
{
	BYTE							cbJettonArea;						//筹码区域
	SCORE							lJettonScore;						//加注数目
	WORD							wChairID;							//玩家位置
	int								nLeftTime;							//剩余时间 (100ms为单位)
};

//筹码数组
typedef CWHArray<tagJettonInfo,tagJettonInfo&> CJettonInfoArray;

//累声明
class CGameClientEngine;
//////////////////////////////////////////////////////////////////////////

//游戏视图
class CGameClientView : public CGameFrameViewGDI
{
	//限制信息
protected:
	SCORE							m_lMeMaxScore;						//最大下注
	SCORE							m_lAreaLimitScore;					//区域限制

	//下注信息
protected:
	SCORE							m_lUserBet[AREA_COUNT];				//个人总注
	SCORE							m_lAllUserBet[AREA_COUNT];			//全体总注
	
	//位置信息
protected:
	int								m_nWinFlagsExcursionX;				//偏移位置
	int								m_nWinFlagsExcursionY;				//偏移位置
	CPoint							m_ptPlayerInfo;						//玩家信息
	CRect							m_rcDiceValue;						//骰子总计
	CRect							m_rcBetArea[AREA_COUNT];			//下注区域
	CRect						    m_RectUserList;						//列表位置
	CRect						    m_RectChartDisplay;					//聊天位置
	CRect							m_rcNumArea[AREA_COUNT];			//下注区域
	CRect							m_rcMeNum[AREA_COUNT];				//下注区域

	//扑克信息
public:
	BYTE							m_cbPrintCount;						//显示数量
	BYTE							m_cbDiceValue[DICE_COUNT];
	BYTE							m_cbCurDirect;						//当前方位
	BYTE							m_cbCurRound;						//当前回合
	BYTE							m_bcShowCount;
	bool							m_blMoveFinish;
	BYTE							m_bcfirstShowCard;
	BYTE							m_cbTableCardArray[DIRECT_COUNT][CARD_COUNT];			//桌面扑克
	BYTE							m_cbTableSortCardArray[DIRECT_COUNT][CARD_COUNT];
	bool							m_bDrawResult;
	bool							m_bAnimatFinish;
	bool							m_bRecordWinner;					//是否记录输赢
	bool							m_bShowList;						//是否显示玩家列表
	bool							m_bShowChat;						//是否显示聊天框
	//历史信息
protected:

	int								m_nRecordFirst;							//开始记录
	int								m_nRecordLast;							//最后记录
	int								m_nRecordCount;
	SCORE							m_lMeStatisticScore;					//游戏成绩
	tagGameRecord					m_GameRecordArrary[MAX_SCORE_HISTORY];  //游戏记录
	//状态变量
protected:
	int								m_nWinMutiple[AREA_COUNT];
	WORD							m_wMeChairID;						//我的位置
	BYTE							m_cbAreaFlash;						//胜利玩家
	int								m_nJettonIndex;						//筹码索引
	LONG							m_lCurrentJetton;					//当前筹码
	bool							m_bShowRecord;
	bool							m_bShowChangeBanker;				//轮换庄家
	bool							m_bNeedSetGameRecord;				//完成设置
	bool							m_bWinFlag[AREA_COUNT];
	bool							m_bFlashResult;						//显示结果
	bool							m_bShowGameResult;					//显示结果
	BYTE                            m_cbGameStatus;						//状态信息
	//庄家信息
protected:	
	WORD							m_wBankerUser;						//当前庄家
	WORD							m_wBankerTime;						//做庄次数
	bool							m_bEnableSysBanker;					//系统做庄
	SCORE							m_lBankerScore;						//庄家积分
	SCORE							m_lBankerWinScore;					//庄家成绩	
	SCORE							m_lTmpBankerWinScore;				//庄家成绩	

	//当局成绩
public:
	LONG							m_lGameRevenue;						//游戏税收
	SCORE							m_lMeCurGameScore;					//我的成绩
	SCORE							m_lMeCurGameReturnScore;			//我的成绩
	SCORE							m_lBankerCurGameScore;				//庄家成绩


	//数据变量
protected:
	CPoint							m_ptRecordPos;						//记录起始位置
	CPoint							m_ptRecordBkPos;					//记录背景位置
	CPoint							m_ptRecordPosEx;
	CPoint							m_ptDicePos[DICE_COUNT];			//骰子位置
	CPoint							m_ptRtDicePos[DICE_COUNT];			//骰子位置
	CPoint							m_PointJetton[AREA_COUNT];			//筹码位置
	CPoint							m_PointJettonNumber[AREA_COUNT];	//数字位置
	CPoint							m_MeJettonNumber[AREA_COUNT];	//数字位置
	CJettonInfoArray				m_JettonInfoArray[AREA_COUNT];		//筹码数组
	//控件变量
public:
	CSkinButton						m_btCloseRecord;					//关闭记录
	CSkinButton						m_btScrollUp;						//上翻按钮
	CSkinButton						m_btScrollDown;						//下翻按钮
	CSkinButton						m_btLookRecord;						//查看记录
	CSkinButton						m_btApplyBanker;					//申请庄家
	CSkinButton						m_btCancelBanker;					//取消庄家
	CSkinButton						m_btJetton[JETTON_COUNT];			//筹码按钮
	CSkinButton						m_btChat;							//聊天按钮
	CSkinButton						m_btUserList;						//玩家列表按钮
	CSkinButton						m_btSendMessage;					//发送消息
public:
	CButton							m_btOpenAdmin;						//开启控制
	//动画控件
public:
	CGameCartoon					m_FrameDice;						//复位动画
	CAnimationWnd					m_CartoonDice;						//动画窗体

	CRect							m_rcAnimatWnd;						//窗口位置
	CPngImage						m_PngAnimatDice;					//动画图片
	//银行模块
public:
	//CDlgBank						m_DlgBank;							//银行控件
	//CSkinButton						m_btBankStorage;					//存款按钮
	CSkinButton						m_btBankDraw;						//取款按钮
	bool							m_blCanStore;                       //是否能保存
	//控件变量
public:
	INT								m_nShowValleyIndex;					//显示上庄列表
	CWHArray<WORD>					m_ValleysList;						//上庄列表
	CGameLogic						m_GameLogic;						//游戏逻辑
	CGameRecord						m_GameRecord;						//记录列表
	CGameClientEngine				*m_pGameClientEngine;				//父类指针
	CSkinListCtrlEx                 m_UserList;							//列表控件
	CChatEdit						m_ChatDisplay;						//显示控件
	CEdit							m_ChatInput;						//输入控件
	CBrush							m_brush;							//背景画刷
	//界面变量
protected:
	CPngImage						m_PngDice3D;
	CPngImage						m_PngDice2D;
	CPngImage						m_PngRecord;
	CPngImage						m_PngFlagsTip;						//下注提醒
	CPngImage						m_PngTimeNumber;					//时间数字
	CPngImage						m_PngAreaFrame[52];					//边框图片
	CPngImage						m_PngTimerBackGround;				//时钟背景
	CPngImage						m_PngScoreNumber;					//数字资源
	CPngImage						m_PngMeScoreNumber;					//数字资源
	CPngImage						m_ImageUserBack;					//游戏玩家列表
	CPngImage						m_ImageChatBack;					//游戏消息框
	CPngImage						m_ImageJettonBack;					//下注数字底框
	CPngImage						m_ImageMeJetton;					//自己下注底框

	CPngImage						m_PngCommer;						//逗号

	CBitImage						m_ImageTimeFlag;					//时间标识
	CBitImage						m_ImageViewFill;					//背景位图
	CBitImage						m_ImageViewBack;					//背景位图
	CBitImage						m_ImageWinFlags;					//标志位图
	CPngImage						m_ImageJettonView;					//筹码视图
	CBitImage						m_ImageScoreNumber;					//数字视图
	CBitImage						m_ImageMeScoreNumber;				//数字视图
	CBitImage						m_ImageDispatchCardTip;				//发牌提示

	//边框资源
protected:
	CBitImage						m_ImageFrame;						//边框图片
	CPngImage						m_ImageNoBanker;					//切换庄家
	CPngImage						m_ImageMeBanker;					//切换庄家
	CPngImage						m_ImageChangeBanker;				//切换庄家

	//结束资源
protected:
	CPngImage						m_ImageGameEnd;						//成绩图片

	//控制
public:
	HINSTANCE						m_hInst;
	IClientControlDlg*				m_pClientControlDlg;

	//函数定义
public:
	//构造函数
	CGameClientView();
	//析构函数
	virtual ~CGameClientView();

	//继承函数
private:
	//重置界面
	virtual void ResetGameView();
	//调整控件
	virtual void RectifyControl(INT nWidth, INT nHeight);
	//绘画界面
	virtual void DrawGameView(CDC * pDC, INT nWidth, INT nHeight);
	//WIN7支持
	virtual bool RealizeWIN7() { return true; }

	//绘制游戏记录
	void DrawGameRecord(CDC*pDC);
	//绘画时间
	VOID DrawUserTimer(CDC * pDC, int nXPos, int nYPos, WORD wTime);

	//设置函数
public:
	void SetFlashArea(BYTE cbFlashArea){m_cbAreaFlash=cbFlashArea;}
	//我的位置
	void SetMeChairID(WORD dwMeUserID);
	//设置信息
	void SetMeMaxScore(SCORE lMeMaxScore);
	//个人下注
	void SetMePlaceJetton(BYTE cbViewIndex, SCORE lJettonCount);
	//庄家信息
	void SetBankerInfo(DWORD dwBankerUserID, SCORE lBankerScore);
	//庄家成绩
	void SetBankerScore(WORD wBankerTime, SCORE lWinScore) {m_wBankerTime=wBankerTime; m_lTmpBankerWinScore=lWinScore;}
	//当局成绩
	void SetCurGameScore(SCORE lMeCurGameScore, SCORE lMeCurGameReturnScore, SCORE lBankerCurGameScore, LONG lGameRevenue);
	//历史记录
	void SetGameHistory(BYTE *cbDiceValue);
	//设置筹码
	void SetCurrentJetton(LONG lCurrentJetton,int nJettonIndex);
	//轮换庄家
	void ShowChangeBanker( bool bChangeBanker );
	//区域限制
	void SetAreaLimitScore(SCORE lAreaLimitScore);
	//设置筹码
	void PlaceUserJetton(BYTE cbViewIndex, SCORE lScoreCount,bool bReDraw=true);
	//设置扑克
	void SetDiceValue(BYTE cbDiceValue[]);	
	//成绩设置
	void SetGameScore(SCORE lMeCurGameScore, SCORE lMeCurGameReturnScore, SCORE lBankerCurGameScore);
	//允许系统做庄
	void EnableSysBanker(bool bEnableSysBanker) {m_bEnableSysBanker=bEnableSysBanker;}
	//首次显示
	void SetFirstShowCard(BYTE bcCard);
	//游戏结束
	void OnGameEnd(const CMD_S_GameEnd * pGameEnd);
	//取款操作
	void OnBankDraw();
	//存款操作
	void OnBankStorage();
	//设置状态
	void SetGameStatus(BYTE cbStatus){m_cbGameStatus=cbStatus;}
	BYTE GetClientViewStatus(){ return m_cbGameStatus;}
	// 添加逗号
	CString AddComma( LONGLONG lScore , bool bPlus = false);
	//动画函数
public:	

	//设置爆炸
	bool SetBombEffect(bool bBombEffect, WORD wAreaIndex);

	//计算限制
public:
	//最大下注
	SCORE GetUserMaxBet(BYTE cbAreaID);
	//更新视图
	void RefreshGameView();

	//绘画函数
protected:
	void MakeString(CString &strNum,SCORE lNumber);
	//闪烁边框
	void FlashAreaFrame(int nWidth, int nHeight, CDC *pDC);

	//界面函数
public:
	//清理筹码
	void CleanUserJetton();
	//设置胜方
	void SetWinnerSide(bool *bWinArea, bool bSet);
	//艺术字体
	void DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame,CRect*rect,UINT nFormat=DT_LEFT);
	//艺术字体
	void DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, int nXPos, int nYPos);
	//绘画数字
	void DrawMeJettonNumber(CDC *pDC);
	//绘画庄家
	void DrawBankerInfo(CDC *pDC,int nWidth,int nHeight);
	//绘画玩家
	void DrawMeInfo(CDC *pDC,int nWidth,int nHeight);
	// 绘画上庄列表
	void DrawBankerList(CDC *pDC, int nWidth, int nHeight);

	//内联函数
public:
	//当前筹码
	inline LONG GetCurrentJetton() { return m_lCurrentJetton; }	
	//删除定时器
	void KillCardTime();

	//内部函数
private:
	CString NumberStringWithSpace(SCORE lNumber);

	BYTE GetFramePngIndex(BYTE cbAreaIndex);
	//获取区域
	BYTE GetJettonArea(CPoint MousePoint);
	//绘画数字
	void DrawNumberString(CDC * pDC, LONG lNumber, INT nXPos, INT nYPos, bool bMeScore = false);
	//绘画数字
	void DrawNumberStringWithSpace(CDC * pDC, LONGLONG lNumber, INT nXPos, INT nYPos);
	//绘画数字
	void DrawNumberStringWithSpace(CDC * pDC, LONGLONG lNumber, CRect rcRect, INT nFormat=-1);
	// 绘画数字
	void DrawNumber(CDC * pDC, CPngImage* ImageNumber, TCHAR * szImageNum, LONGLONG lOutNum, INT nXPos, INT nYPos, UINT uFormat = DT_LEFT);
	// 绘画数字
	void DrawNumber(CDC * pDC, CPngImage* ImageNumber, TCHAR * szImageNum, CString szOutNum, INT nXPos, INT nYPos, UINT uFormat = DT_LEFT);
	// 绘画数字
	void DrawNumber(CDC * pDC, CPngImage* ImageNumber, TCHAR * szImageNum, TCHAR* szOutNum ,INT nXPos, INT nYPos,  UINT uFormat = DT_LEFT);
	//显示结果
	void ShowGameResult(CDC *pDC, int nWidth, int nHeight);
	//透明绘画
	bool DrawAlphaRect(CDC* pDC, LPRECT lpRect, COLORREF clr, FLOAT fAlpha);
	//庄家最小得分
	SCORE GetBankerWinMinScore(BYTE cbAreaID);	

	//辅助函数
public:
	void OnTimeGameEnd();
	//系统消息
	void InsertSystemMessage(LPCTSTR pszString);
	//聊天消息
	void InsertChatMessage(LPCTSTR pszString);
	//常规消息
	void InsertNormalMessage(LPCTSTR pszString);
	//聊天消息
	void InsertAllChatMessage(LPCTSTR pszString, int nMessageType);
	//消息函数
	void InsertMessage(LPCTSTR pszString,COLORREF color=RGB(193,167,108));
	//按钮消息
protected:
	//上翻按钮
	afx_msg void OnUp();
	//下翻按钮
	afx_msg void OnDown();
	//管理员控制
	afx_msg void OpenAdminWnd();
	//上庄按钮
	afx_msg void OnApplyBanker();
	//下庄按钮
	afx_msg void OnCancelBanker();
	//发送消息
	afx_msg void OnSendMessage();

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
	//光标消息
	afx_msg BOOL OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage);
	//控件命令
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//其他控件鼠标UP消息
	LRESULT OnViLBtUp(WPARAM wParam, LPARAM lParam);
	//动画结束
	LRESULT OnAnimatEnd(WPARAM wParam,LPARAM lParam);

	DECLARE_MESSAGE_MAP()	
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};

//////////////////////////////////////////////////////////////////////////

#endif
