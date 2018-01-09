#ifndef GAME_CLIENT_HEAD_FILE
#define GAME_CLIENT_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "CardControl.h"
#include "ApplyUserList.h"
#include "RecordGameList.h"
#include "ClientControl.h"
#include "GameLogic.h"
#include "SkinListCtrl.h"
#include "MyEdit.h"
#include "BankTipsDlg.h"
//////////////////////////////////////////////////////////////////////////////////

//筹码定义
#define JETTON_COUNT				7									//筹码数目
#define JETTON_RADII				68									//筹码半径

//消息定义
#define IDM_PLACE_JETTON			WM_USER+200							//加住信息
#define IDM_APPLY_BANKER			WM_USER+201							//申请信息
#define IDM_CONTINUE_CARD			WM_USER+202							//继续发牌
#define IDM_AUTO_OPEN_CARD			WM_USER+203							//按钮标识
#define IDM_OPEN_CARD				WM_USER+204		
#define IDM_GAME_OPTIONS			WM_USER+205							//游戏设置消息
#define IDM_CHAT_MESSAGE			WM_USER+206							//游戏聊天消息
#define IDM_GAME_SOUND				WM_USER+207							//游戏设置
#define IDM_FINISH_DISPATCHCARD		WM_USER+210							//完成发牌
#define WM_SET_CAPTION				WM_USER+208							//设置标题

//索引定义
#define INDEX_PLAYER				0									//闲家索引
#define INDEX_BANKER				1									//庄家索引

#define MAX_APPLY_DISPLAY			6									//最大申请显示人数
//////////////////////////////////////////////////////////////////////////
//结构定义

//历史记录
#define MAX_SCORE_HISTORY			65									//历史个数
#define NORMAL_MAX_FALG_COUNT				13							//正常模式标识个数
#define ZOOMED_MAX_FALG_COUNT				20							//最大化模式标识个数

//筹码信息
struct tagJettonInfo
{
	int								nXPos;								//筹码位置
	int								nYPos;								//筹码位置
	BYTE							cbJettonIndex;						//筹码索引
	BOOL							bShow;								//是否显示
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
	enOperateResult					enOperateTianMen;					//操作标识
	enOperateResult					enOperateDiMen;						//操作标识
	enOperateResult					enOperateXuanMen;					//操作标识
	enOperateResult					enOperateHuangMen;					//操作标识
	bool							bWinTianMen;						//顺门胜利
	bool							bWinDiMen;							//对门胜利
	bool							bWinXuanMen;						//倒门胜利
	bool							bWinHuangMen;						//倒门胜利
};

//发牌提示
enum enDispatchCardTip
{
	enDispatchCardTip_NULL,
	enDispatchCardTip_Continue,											//继续发牌
	enDispatchCardTip_Dispatch											//重新洗牌
};

//客户端机器人下分
struct tagAndroidBet
{
	BYTE							cbJettonArea;						//筹码区域
	LONGLONG						lJettonScore;						//加注数目
	WORD							wChairID;							//玩家位置
	int								nLeftTime;							//剩余时间 (100ms为单位)
};

//筹码数组
typedef CWHArray<tagJettonInfo,tagJettonInfo&> CJettonInfoArray;

//累声明
class CGameClientEngine;

//////////////////////////////////////////////////////////////////////////////////

//游戏视图
class CGameClientView : public CGameFrameViewGDI
{
	//限制信息
protected:
	LONGLONG						m_lMeMaxScore;						//最大下注
	LONGLONG						m_lAreaLimitScore;					//区域限制

	//下注信息
protected:
	LONGLONG						m_lUserJettonScore[AREA_COUNT+1];	//个人总注
	LONGLONG						m_lAllJettonScore[AREA_COUNT+1];	//全体总注
public:
	INT								m_nEndGameMul;						//提前开牌百分比

	//位置信息
protected:
	int								m_nWinFlagsExcursionX;				//偏移位置
	int								m_nWinFlagsExcursionY;				//偏移位置
	int								m_nScoreHead;						//成绩位置
	CRect							m_rcTianMen;						//闲家区域
	CRect							m_rcDimen;							//闲天王区域
	CRect							m_rcQiao;							//对子区域
	CRect							m_rcHuangMen;						//平区域
	CRect							m_rcXuanMen;						//同点平区域
	CRect							m_rcHuang;							//同点平区域
	CRect							m_rcJiaoR;							//庄家区域
	int								m_OpenCardIndex;					//開牌順序
	int								m_PostCardIndex;					//發牌順序
	int								m_PostStartIndex;					//发牌起始位置
	CPoint							m_CardTypePoint[AREA_COUNT+1];
	CRect						    m_RectUserList;						//列表位置
	CRect						    m_RectChartDisplay;					//聊天位置

	//扑克信息
public:	
	BYTE							m_cbTableCardArray[5][5];			//桌面扑克
	BYTE							m_cbTableSortCardArray[5][5];		//桌面扑克
	bool							m_blMoveFinish;
	BYTE							m_bcfirstShowCard;
	BYTE							m_bcShowCount;
	bool							m_blAutoOpenCard;					//手自动
	int								m_lUserCardType[AREA_COUNT+1];		//个人总注

	//历史信息
protected:
	LONGLONG						m_lMeStatisticScore;				//游戏成绩
	tagClientGameRecord				m_GameRecordArrary[MAX_SCORE_HISTORY];//游戏记录
	int								m_nRecordFirst;						//开始记录
	int								m_nRecordLast;						//最后记录

	//状态变量
public:
	WORD							m_wMeChairID;						//我的位置
	BYTE							m_cbAreaFlash;						//胜利玩家
	LONGLONG						m_lCurrentJetton;					//当前筹码
	bool							m_bShowChangeBanker;				//轮换庄家
	bool							m_bAdvancedOpenCard;				//提前开牌
	bool							m_bNeedSetGameRecord;				//完成设置
	bool							m_bWinTianMen;						//胜利标识
	bool							m_bWinHuangMen;						//胜利标识
	bool                            m_bWinDiMen;
	bool							m_bWinXuanMen;						//胜利标识
	bool							m_bFlashResult;						//显示结果
	bool							m_bShowGameResult;					//显示结果
	enDispatchCardTip				m_enDispatchCardTip;				//发牌提示
	CString							m_strRoomName;						//房间名

	//庄家信息
protected:	
	WORD							m_wBankerUser;						//当前庄家
	WORD							m_wBankerTime;						//做庄次数
	LONGLONG						m_lBankerScore;						//庄家积分
	LONGLONG						m_lBankerWinScore;					//庄家成绩	
	bool							m_bEnableSysBanker;					//系统做庄

	//当局成绩
public:
	LONGLONG						m_lMeCurGameScore;					//我的成绩
	LONGLONG						m_lMeCurGameReturnScore;			//我的成绩
	LONGLONG						m_lBankerCurGameScore;				//庄家成绩
	LONGLONG						m_lGameRevenue;						//游戏税收

	//数据变量
protected:
	CPoint							m_PointJetton[AREA_COUNT];			//筹码位置
	CPoint							m_PointJettonNumber[AREA_COUNT];	//数字位置
	CJettonInfoArray				m_JettonInfoArray[JETTON_COUNT];	//筹码数组

	//控件变量
public:
	CSkinButton						m_btJetton100;						//筹码按钮
	CSkinButton						m_btJetton1000;						//筹码按钮
	CSkinButton						m_btJetton10000;					//筹码按钮
	CSkinButton						m_btJetton50000;					//筹码按钮
	CSkinButton						m_btJetton100000;					//筹码按钮
	CSkinButton						m_btJetton500000;					//筹码按钮
	CSkinButton						m_btJetton1000000;					//筹码按钮
	CSkinButton						m_btJetton5000000;					//筹码按钮
	CSkinButton						m_btApplyBanker;					//申请庄家
	CSkinButton						m_btCancelBanker;					//取消庄家
	CSkinButton						m_btScoreMoveL;						//移动成绩
	CSkinButton						m_btScoreMoveR;						//移动成绩
	//CSkinButton						m_btAutoOpenCard;					//自动开牌
	CSkinButton						m_btOpenCard;						//手动开牌
	CSkinButton						m_btBank;							//银行按钮
	CSkinButton						m_btContinueCard;					//继续发牌
	CSkinButton						m_btUp;								//存款按钮
	CSkinButton						m_btDown;							//取款按钮
	CSkinButton						m_btOption;							//设置按钮
	CSkinButton						m_btMin;							//最小化按钮
	CSkinButton						m_btClose;							//关闭按钮
	CSkinButton						m_btMax;
	CSkinButton						m_btRestore;
	CSkinButton						m_btSound;							//

	CSkinButton						m_btSendChat;						//发送按钮

public:
	CButton							m_btOpenAdmin;						//开启控制
	//控件变量
public:

	INT								m_nShowValleyIndex;					//显示上庄列表
	CWHArray<WORD>					m_ValleysList;						//上庄列表
	CGameRecord						m_GameRecord;						//记录列表
	CCardControl					m_CardControl[6];					//扑克控件	
	CGameClientEngine					*m_pGameClientDlg;					//父类指针
	CGameLogic						m_GameLogic;						//游戏逻辑
	CRect							m_MeInfoRect;

	//控制
public:
	HINSTANCE						m_hInst;
	IClientControlDlg*				m_pClientControlDlg;		
	CSkinListCtrlEx                 m_UserList;							//列表控件
	CMyEdit							m_ChatDisplay;						//显示控件
	CEdit							m_ChatInput;						//输入控件
	CBrush							m_brush;							//背景画刷

	//界面变量
protected:
	CBitImage						m_ImageViewFill;					//背景填充
	CPngImage						m_ImageViewBack;					//背景位图
	CPngImage						m_ImageGameFrame[8];				//游戏框架	
	CPngImage						m_ImageWinFlags;					//标志位图
	CPngImage						m_ImageJettonView;					//筹码视图
	CPngImage						m_ImageScoreNumber;					//数字视图
	CPngImage						m_ImageMeScoreNumber;				//数字视图
	CPngImage						m_ImageTimeFlag;					//时间标识
	CPngImage						m_ImageDispatchCardTip;				//发牌提示
	CPngImage						m_ImageCardType[AREA_COUNT+1];		//数字视图
	CPngImage						m_ImageWaitValleys;					//等待上庄
	CPngImage						m_ImageTimeNumber;					//时间数字
	CPngImage						m_ImageTimeBack;					//时间背景
	CPngImage						m_ImageUserBack;					//游戏玩家列表
	CPngImage						m_ImageChatBack;					//游戏消息框

	CPngImage						m_ImageBankerInfo;
	CPngImage						m_ImagePlayerInfo;
	CPngImage						m_ImageChipInfo;

	CPngImage						m_ImageHistoryMid;               
	CPngImage						m_ImageHistoryFront;
	CPngImage						m_ImageHistoryBack;

	//边框资源
protected:
	CPngImage						m_ImageFrameTianMen;				//边框图片
	CPngImage						m_ImageFrameDiMen;					//边框图片
	CPngImage						m_ImageFrameHuangMen;				//边框图片
	CPngImage						m_ImageFrameXuanMen;				//边框图片
	CBitImage						m_ImageFrameQiao;					//边框图片
	CBitImage						m_ImageFrameJiaoR;					//边框图片

	CPngImage						m_ImageMeBanker;					//切换庄家
	CPngImage						m_ImageChangeBanker;				//切换庄家
	CPngImage						m_ImageNoBanker;					//切换庄家

	CPngImage						m_ImageAdvanceOpenCard;				//提前开牌

	//结束资源
protected:
	CPngImage						m_ImageGameEnd;						//成绩图片
	CDFontEx						m_DFontExT;
	CDFontEx						m_DBigFont;							//大号字体
	CRgn							m_JettonRegion[AREA_COUNT];			//显示区域
	
	DWORD							m_dwChatTime;						//聊天时间
	
	CString							m_strCaption;

	//函数定义
public:
	//构造函数
	CGameClientView();
	//析构函数
	virtual ~CGameClientView();

public:
	//消息解释
	virtual BOOL PreTranslateMessage(MSG * pMsg);

protected:
	//命令函数
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//重载函数
private:
	//重置界面
	virtual VOID ResetGameView();
	//调整控件
	virtual VOID RectifyControl(INT nWidth, INT nHeight);
	//绘画界面
	virtual VOID DrawGameView(CDC * pDC, INT nWidth, INT nHeight);
	//WIN7支持
	virtual bool RealizeWIN7() { return true; }

public:
	//更新按钮
	VOID UpdateButtonContron();

	//设置函数
public:
	//设置房名
	void SetRoomName(CString strRoomName) { m_strRoomName = strRoomName;}
	//设置信息
	void SetMeMaxScore(LONGLONG lMeMaxScore);
	//我的位置
	void SetMeChairID(WORD dwMeUserID);
	//个人下注
	void SetMePlaceJetton(BYTE cbViewIndex, LONGLONG lJettonCount);
	//庄家信息
	void SetBankerInfo(DWORD dwBankerUserID, LONGLONG lBankerScore);
	//庄家成绩
	void SetBankerScore(WORD wBankerTime, LONGLONG lWinScore) {m_wBankerTime=wBankerTime; m_lBankerWinScore=lWinScore;}
	//当局成绩
	void SetCurGameScore(LONGLONG lMeCurGameScore, LONGLONG lMeCurGameReturnScore, LONGLONG lBankerCurGameScore, LONGLONG lGameRevenue);
	//设置筹码
	void PlaceUserJetton(BYTE cbViewIndex, LONGLONG lScoreCount);
	//区域限制
	void SetAreaLimitScore(LONGLONG lAreaLimitScore);	
	//设置扑克
	void SetCardInfo(BYTE cbTableCardArray[5][5]);
	//设置筹码
	void SetCurrentJetton(LONGLONG lCurrentJetton);
	//历史记录
	void SetGameHistory(bool bWinTian, bool bWinDi, bool bWinXuan,bool bWinHuang);
	//轮换庄家
	void ShowChangeBanker( bool bChangeBanker );
	//提前开牌
	void ShowAdvancedOpenCard(bool bAdvancedOpenCard);
	//成绩设置
	void SetGameScore(LONGLONG lMeCurGameScore, LONGLONG lMeCurGameReturnScore, LONGLONG lBankerCurGameScore);
	//允许系统做庄
	void EnableSysBanker(bool bEnableSysBanker) {m_bEnableSysBanker=bEnableSysBanker;}
	//设置提示
	void SetDispatchCardTip(enDispatchCardTip DispatchCardTip);

	void SetFirstShowCard(BYTE bcCard);
	//取消闪动
	void ClearAreaFlash() { m_cbAreaFlash = 0xFF; }
	//聊天消息
	void InsertAllChatMessage(LPCTSTR pszString,int nMessageType) ;
	//系统消息
	void InsertSystemMessage(LPCTSTR pszString);
	//聊天消息
	void InsertChatMessage(LPCTSTR pszString);
	//常规消息
	void InsertNormalMessage(LPCTSTR pszString);
	//消息函数
	void InsertMessage(LPCTSTR pszString,COLORREF color=RGB(193,167,108));
	//结算消息
	void InsertGameEndInfo();
	//银行按钮
	void OnButtonGoBanker();

	//动画函数
public:	
	//开始发牌
	void DispatchCard();
	//结束发牌
	void FinishDispatchCard(bool bNotScene);
	//设置爆炸
	bool SetBombEffect(bool bBombEffect, WORD wAreaIndex);
	//终止动画
	void StopAnimal();

	//计算限制
public:
	//最大下注
	LONGLONG GetUserMaxJetton();
	// 绘画上庄列表
	void DrawBankerList(CDC *pDC, int nWidth, int nHeight);

	//绘画函数
protected:
	//闪烁边框
	void FlashJettonAreaFrame(int nWidth, int nHeight, CDC *pDC);

	//界面函数
public:
	//清理筹码
	void CleanUserJetton();
	//设置胜方
	void SetWinnerSide(bool bWinTian, bool bWinDi, bool bWinXuan,bool bWinHuang, bool bSet);
	//艺术字体
	void DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, int nXPos, int nYPos);
	//艺术字体
	void DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, CRect rcRect, UINT nDrawFormat);
	//绘画数字
	void DrawMeJettonNumber(CDC *pDC);
	//绘画庄家
	void DrawBankerInfo(CDC *pDC,int nWidth,int nHeight);
	//绘画玩家
	void DrawMeInfo(CDC *pDC,int nWidth,int nHeight);
	//绘画框架
	void DrawGameFrame(CDC *pDC, int nWidth, int nHeight);
	// 绘画时钟
	void DrawTime(CDC * pDC, WORD wUserTime,INT nXPos, INT nYPos);
	// 绘画数字
	void DrawNumber(CDC * pDC, CPngImage* ImageNumber, TCHAR * szImageNum, LONGLONG lOutNum, INT nXPos, INT nYPos, UINT uFormat = DT_LEFT);
	// 绘画数字
	void DrawNumber(CDC * pDC, CPngImage* ImageNumber, TCHAR * szImageNum, CString szOutNum, INT nXPos, INT nYPos, UINT uFormat = DT_LEFT);
	// 绘画数字
	void DrawNumber(CDC * pDC, CPngImage* ImageNumber, TCHAR * szImageNum, TCHAR* szOutNum ,INT nXPos, INT nYPos,  UINT uFormat = DT_LEFT);

	//内联函数
public:
	//当前筹码
	LONGLONG GetCurrentJetton() { return m_lCurrentJetton; }

	void SetMoveCardTimer();
	void KillCardTime();

	// 是否处于最大化模式
	bool IsZoomedMode();

	// 计算全屏绘画的缝隙，向右平移的像素
	INT CalFrameGap();

	//内部函数
private:
	//获取区域
	BYTE GetJettonArea(CPoint MousePoint);
	//绘画数字
	void DrawNumberString(CDC * pDC, LONGLONG lNumber, INT nXPos, INT nYPos, bool bMeScore = false);
	//绘画数字
	void DrawNumberStringWithSpace(CDC * pDC, LONGLONG lNumber, INT nXPos, INT nYPos);
	//绘画数字
	void DrawNumberStringWithSpace(CDC * pDC, LONGLONG lNumber, CRect rcRect, INT nFormat=-1,bool bBig = false);
	//绘画标识
	void DrawWinFlags(CDC * pDC);
	//显示结果
	void ShowGameResult(CDC *pDC, int nWidth, int nHeight);
	//透明绘画
	bool DrawAlphaRect(CDC* pDC, LPRECT lpRect, COLORREF clr, FLOAT fAlpha);

	//
	void DrawType(CDC* pDC,WORD wChairID);
	//推断赢家
	void DeduceWinner(bool &bWinTian, bool &bWinDi, bool &bWinXuan,bool &bWinHuan,BYTE &TianMultiple,BYTE &diMultiple,BYTE &TianXuanltiple,BYTE &HuangMultiple );


	//按钮消息
protected:
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
	//银行取款
	afx_msg void OnBankDraw();
	//翻页按钮
	afx_msg void OnUp();
	//翻页按钮
	afx_msg void OnDown();
	//关闭游戏
	afx_msg void OnButtonClose();
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
	//鼠标消息
	afx_msg void OnLButtonUp(UINT nFlags, CPoint Point);
	//光标消息
	afx_msg BOOL OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage);
	//标题
	afx_msg LRESULT OnSetCaption(WPARAM wParam, LPARAM lParam);
	//控件颜色
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//鼠标移动
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);	
	//管理员控制
	afx_msg void OpenAdminWnd();
	
	afx_msg void OnLButtonDblClk( UINT nFlags, CPoint point );
	
	//聊天效验
	bool EfficacyUserChat(LPCTSTR pszChatString, WORD wExpressionIndex);

	DECLARE_MESSAGE_MAP()	
};

//////////////////////////////////////////////////////////////////////////////////

#endif