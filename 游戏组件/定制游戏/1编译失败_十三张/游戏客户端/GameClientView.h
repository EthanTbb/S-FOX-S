#ifndef GAME_CLIENT_VIEW_HEAD_FILE
#define GAME_CLIENT_VIEW_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "CardControl.h"
#include "MulsecCardControlEx.h"
#include "HistoryScore.h"
#include "GameLogic.h"
#include "MessageBox.h"

/////////////////////////////////////////////////////////////////////////////////////////////////
//消息定义
#define IDM_START					(WM_USER+100)						//开始消息
#define IDM_CALL_BANKER				(WM_USER+101)                       //叫庄消息
#define IDM_NO_CALL_BANKER			(WM_USER+102)                       //不叫消息
#define IDM_SETCHIP					(WM_USER+103)						//压注消息
#define IDM_SHUFFLE_FINISH			(WM_USER+104)						//洗牌完毕
#define IDM_SPLIT_CARD				(WM_USER+105)						//切牌消息
#define IDM_SPLIT_FINISH			(WM_USER+106)						//切牌完毕
#define IDM_REVERSAL_FINISH			(WM_USER+107)						//翻牌完毕
#define IDM_DISPATCH_CARD_FINISH	(WM_USER+108)						//发牌完毕
#define IDM_FRONTCARD				(WM_USER+109)						//设置前墩  
#define IDM_MIDCARD					(WM_USER+110)						//设置中墩
#define IDM_BACKCARD				(WM_USER+111)						//设置后墩
#define IDM_SHOWCARD				(WM_USER+112)						//确定消息
#define IDM_SORT_CARD				(WM_USER+113)						//排序消息
#define IDM_TRUSTEE			        (WM_USER+114)                       //托管消息
#define IDM_RESTORE_CARD			(WM_USER+115)                       //恢复扑克
#define IDM_PROMPT					(WM_USER+116)						//提示消息
#define IDM_RESTORE					(WM_USER+117)						//回收扑克
#define IDM_REVERSAL_TURN_FINISH	(WM_USER+118)						//比牌消息
#define IDM_VOICE_PHRASE			(WM_USER+200)						//语音消息

#define JETTON_COUNT				14									//筹码种类数
////////////////////////////////////////////////////////////////////////////////////////////////
//筹码状态
struct tagJettonStatus
{
	//属性信息
	WORD							wMoveCount;							//移动次数
	WORD							wMoveIndex;							//移动索引

	//筹码信息
	CPoint							ptFrom;								//出发位置
	CPoint							ptDest;								//目的位置
	CPoint							ptCourse;							//过程位置
	LONGLONG						lGold;								//筹码数目

	//移动形式
	INT								iMoveType;							//移动形式
};

//打枪动画
struct tagGunStatus
{
	WORD							wSrcUser;							//打枪玩家
	WORD							wDestUser;							//目标玩家
};

//发牌子项
struct tagSendCardItem
{
	WORD							wChairID;							//发牌用户
	BYTE							cbCardData;							//发牌数据
};

//数组说明
typedef CWHArray<tagSendCardItem,tagSendCardItem &> CSendCardItemArray;

//扑克类型
enum enCardControlType
{
	enCCTHandCard,						//手上扑克
	enCCTSegmentCard,					//用户分段
	enCCTAllSegmentCard					//所有分段
};

//游戏视图
class CGameClientView : public CGameFrameViewGDI
{
	//鼠标变量
protected:
	bool							m_bMoveMouse;						//移动标志
	WORD							m_wMouseDownItem;					//点击索引
	WORD							m_wHoverCardItem;					//盘旋索引
	//界面变量
protected:
	WORD							m_wBanker;							//庄家玩家
	enSegmentType					m_enSegmentType ;					//比较类型
	bool							m_bCallBankerStatus[GAME_PLAYER] ;	//不叫标识
	bool							m_bGameStatus[GAME_PLAYER];			//玩家状态
	bool							m_bTrustee[GAME_PLAYER];			//托管标识
	bool							m_bShowHistory;						//显示成绩
	bool							m_bShowCardFrame;					//显示牌框
	bool							m_bVoicePanelStatus;				//语音面板
	bool							m_bGlobalClock;						//全局时钟
	DWORD							m_dwElapseTime;						//时间计数
	TCHAR							m_szSoundPath[MAX_PATH];			//文件路径
	
	bool							m_bShowFuZhuClock[GAME_PLAYER];		//辅助时钟
	DWORD							m_dwFuZhuTime;						//辅助时间
	
	BYTE							m_cbTipInfoState;					//提示信息	1:叫庄 2:下注

	//特殊提示
	bool							m_bShowSpecType[GAME_PLAYER];		//显示牌型
	BYTE							m_cbSpecType[GAME_PLAYER];			//特殊牌型
	
	SCORE							m_wMaxChip;							//最高下注
	SCORE							m_lUserChip[GAME_PLAYER];			//玩家押注
	SCORE							m_lChipArray[SET_CHIP_COUNT];		//押注按钮
	SCORE							m_lGameScore[GAME_PLAYER];			//玩家得分
	SCORE							m_lScoreTimes[GAME_PLAYER];			//分数倍数
	
	//积分信息
protected:
	tagHistoryScore*				m_pHistoryScore[GAME_PLAYER];		//积分信息

	//提示动画
	bool							m_bShowTips;						//动画标识
	
	//筹码动画
	tagJettonStatus					m_JettonStatus[GAME_PLAYER];		//筹码信息
	bool							m_bJettonAction;					//动画标识
	BYTE							m_JettonData[GAME_PLAYER][JETTON_COUNT];//筹码数据

	//洗牌动画
protected:
	bool							m_bShuffleCard;						//洗牌标识
	WORD							m_wShuffleIndex;					//动画索引

	//切牌动画
protected:
	bool							m_bSplitCard;						//切牌标识
	WORD							m_wSplitIndex;						//动画索引

	//翻牌动画
protected:
	bool							m_bReversalCard;					//翻牌标识
	BYTE							m_cbReversalCount;					//翻牌数量
	WORD							m_wReversalIndex;					//翻牌索引
	WORD							m_wReversalUser;					//当前玩家
	CPoint							m_ptReversal;						//翻牌位置
	BYTE							m_cbCardType;						//当前牌型
	bool							m_bShowType;						//显示标识
    
	//发牌动画
protected:
	WORD							m_wStartDispatcher;					//首得牌者
	bool							m_bDispatchCard;					//动画标志
	WORD							m_wDispatchCardIndex;				//帧数索引
	BYTE							m_cbHandCardData[HAND_CARD_COUNT];	//发牌数据

	//全垒打动画
protected:
	WORD							m_wSwatUser;						//赢家ID
	bool							m_bSwatCartoon;						//动画标志
	WORD							m_wSwatIndex;						//帧数索引
	CPoint							m_ptSwat;							//播放位置

	//打枪动画
protected:
	tagGunStatus					m_GunStatus[GAME_PLAYER];			//打枪结构
	WORD							m_wGroupCount;						//动画组数
	WORD							m_wCurrentIndex;					//当前索引
	bool							m_bGunCartoon;						//动画标记
	bool							m_bBullet;							//
	CPoint							m_ptSrc;							//打枪位置
	CPoint							m_ptDest;							//目标位置



	//位置变量
protected:
	CPoint							m_ptUserFrame[GAME_PLAYER];			//用户位置
	CPoint							m_ptBankerFlag[GAME_PLAYER];		//庄家位置		
	CPoint							m_ptCardControl[GAME_PLAYER] ;		//扑克位置
	CPoint							m_ptChip[GAME_PLAYER];				//筹码位置
	CPoint							m_ptCallScore[GAME_PLAYER];			//叫分位置
	CPoint							m_ptTrusteeFlag[GAME_PLAYER];		//托管位置
	CPoint							m_ptClockCenter;					//时钟位置
	CPoint							m_ptClockDir[GAME_PLAYER];			//时钟方向
	CPoint							m_ptGameScore[GAME_PLAYER];			//玩家得分
	CPoint							m_ptScoreTimes[GAME_PLAYER];		//倍数位置
	CPoint							m_ptVoicePanel;						//语音面板

	//控件变量
public:
	CSkinButton						m_btLookScore;						//成绩按钮
	CSkinButton						m_btStart;							//开始按钮
	CSkinButton						m_btShowCard;						//确定按钮
	CSkinButton						m_btFrontCard;						//第一注按钮
	CSkinButton						m_btMidCard;						//第二注按钮
	CSkinButton						m_btBackCard;						//第三注按钮
	CSkinButton						m_btCallBanker ;					//叫庄按钮
	CSkinButton						m_btNoCallBanker ;					//不叫按钮
	CSkinButton						m_btSetChip[4];						//压注按钮
	CSkinButton						m_btPrompt[8];						//提示按钮
	CSkinButton						m_btSplitCard;						//切牌按钮
	CSkinButton						m_btSortCard;						//排序按钮
	CSkinButton						m_btTrustee;						//托管按钮
	CSkinButton						m_btVoice;							//语音按钮
	CSkinButton						m_btRestore[4];						//收回按钮

	//位图变量
protected:
	CBitImage						m_ImageBack;						//背景资源
	CBitImage						m_ImageCenter;						//背景资源
	CPngImage						m_PngShowUserChip;					//筹码图片
	CPngImage						m_PngShuffleCard;					//洗牌动画
	CPngImage						m_PngSplitCard;						//切牌动画
	CPngImage						m_PngSwatCartoon;					//全垒打动画
	CPngImage						m_PngMovingCard;					//旋转动画
	CPngImage						m_PngUserHeadBackH;					//用户背景
	CPngImage						m_PngUserHeadBackV;					//用户背景
	CPngImage						m_PngUserInfo;						//用户背景
	CPngImage						m_PngHistoryScore;					//成绩背景
	CPngImage						m_PngChipNumer;						//数字图片
	CPngImage						m_PngResultNumber;					//数字图片
	CPngImage						m_PngReadyFlag;						//准备图片
	CPngImage						m_PngTrusteeFlag;					//托管图片
	CPngImage						m_PngNoCallBanker;					//不叫图片
	CPngImage						m_PngBankerFlag;					//庄家图片	
	CPngImage						m_PngReversalCard;					//翻牌图片
	CPngImage						m_PngCardType;						//牌型提示
	CPngImage						m_PngCardTypeSpec[GAME_PLAYER];		//牌型提示
	CPngImage						m_PngCardTip;						//相公提示
	CPngImage						m_PngCardFrame;						//扑克边框
	CPngImage						m_PngVoicePanelBack;				//语音面板	

	CPngImage						m_PngGun;							//图片资源
	CPngImage						m_PngGunFire;						//图片资源
	CPngImage						m_PngGunHole[2];					//图片资源

	CPngImage						m_PngTimeNumber;					//数字图片
	CPngImage						m_PngTimerFlag;						//时钟图片
	CPngImage						m_PngDirectionOp;					//箭头图片

	CPngImage						m_PngTipCallBanker;					//提示信息
	CPngImage						m_PngTipCallScore;					//提示信息

	//扑克控件
public:
	CCardControl					m_UserCardControl[GAME_PLAYER];		//手上扑克
	CCardControl					m_HeapCardControl;					//牌堆扑克
	CMulSecCardControlEx			m_AllSegCardControl[GAME_PLAYER];	//三个分段	
	CMulSecCardControlEx			m_CenterCardControl;				//分段扑克

	//组件变量
public:
	CGameLogic						m_GameLogic;						//逻辑组件
	CMessageBox						m_MsgBox;							//消息框
	CDFontEx						m_FontChip;							//押注字体
	CDFontEx						m_FontComp;							//比较字体

	//函数定义
public:
	//构造函数
	CGameClientView();
	//析构函数
	virtual ~CGameClientView();

	//重载函数
protected:
	//命令函数
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);

	//功能函数
public:
	//设置状态
	void SetGameStatus(WORD wChairID,bool bStatus) {m_bGameStatus[wChairID] = bStatus;};
	//设置扑克
	void SetCardData(WORD wChairID , BYTE bCardData[] , BYTE bCardCount , enCardControlType bCardType , BYTE bSeg=-1) ;
	//设置庄家
	void SetBanker(WORD wBanker) ;
	//设置托管
	void SetTrustee(WORD wChairID, bool bTrustee);
	//设置压注
	void SetChip(WORD wChairID, SCORE lChip, bool bMove=false) ;
	//设置分数
	void SetGameScore(WORD wChairID, SCORE lScore);
	//设置倍数
	void SetScoreTimes(WORD wChairID, SCORE lScoreTimes);
	//显示比较
	void SetCompareScene(enSegmentType SegmentType=enSegErr) ;
	//显示牌框
	void ShowCenterControl(bool bShow);
	//牌型提示
	void SetShowCardTip(bool bShow);
	//显示牌型
	void SetCardType(BYTE cbCardType);
	//显示牌型
	void SetCardTypeSpec(bool bShow, WORD wChairID, BYTE cbCardType);
	//最高下注
	void SetMaxChip(SCORE wMaxChip) ;
	//叫庄信息
	void SetCallBankerStatus(WORD wChairID, bool bCallBankerStatus) ;
	//设置下注
	void SetChipArray(SCORE lChipsArray[SET_CHIP_COUNT]) ;
	//获取注码
	SCORE GetChipArrayItem(int nIndex) { return m_lChipArray[nIndex]; }
	//设置积分
	VOID SetHistoryScore(WORD wChairID, tagHistoryScore * pHistoryScore);	
	//随机文件
	LPCTSTR GetRandomFile(LPCTSTR pszPath);
	//播放声音
	VOID PlayCardTypeSound(WORD wChairID, BYTE cbCardType, BYTE cbMaxCard);
	//播放声音
	VOID PlaySpecCardTypeSound(WORD wChairID, BYTE cbSpecialType);
	//开启时钟
	void SetGlobalClock(bool isClock);
	//设置时间
	void SetElapseTime(DWORD dwElapseTimer);
	//获取提示
	bool GetShowCardTip();
	
	//辅助时钟
	void ShowFuZhuClock(WORD wChairID,bool bShow);
	//设置辅助时间
	void SetFuZhuTime(DWORD dwFuZhuTime);
	
	//设置提示信息
	void SetTipInfoState(BYTE cbTipInfoState);

	//更新视图
	VOID RefreshGameView();
	//洗牌动画
	VOID StartShuffleCard();
	//停止洗牌
	VOID StopShuffleCard();
	//发牌动画
	VOID StartDispatchCard(BYTE cbCardData[HAND_CARD_COUNT], WORD wStartUser);
	//全垒打动画
	VOID StartSwatCartoon(WORD wChairID);
	//停止动画
	VOID StopSwatCartoon();
	//设置动画
	VOID SetGunCartoonGroup(tagGunStatus GunStatus[], WORD wGroupCount);
	//打枪动画
	VOID StartGunCartoon(WORD wSrcUser, WORD wDestUser);
	//打枪动画
	VOID StopGunCartoon();
	//停止发牌
	VOID StopDispatchCard();
	//切牌动画
	VOID StartSplitCard();
	//停止切牌
	VOID StopSplitCard();
	//翻牌动画
	VOID ReversalCard(WORD wStartUserID, BYTE cbCardCount);
	// 绘画数字
	VOID DrawNumber(CDC * pDC, CPngImage* ImageNumber, TCHAR * szImageNum, SCORE lOutNum, INT nXPos, INT nYPos, UINT uFormat = DT_LEFT);
	//绘制数字
	VOID DrawSignNumber(CDC * pDC, CPngImage* ImageNumber, TCHAR * szImageNum, SCORE lOutNum, INT nXPos, INT nYPos, UINT uFormat = DT_LEFT);
	//绘画时钟
	VOID DrawUserCustomClock(CDC * pDC, WORD wChairID, WORD wElapseTime);
	//绘制全局时钟
	VOID DrawGlobalClock(CDC* pDc);
	//绘制辅助时钟
	VOID DrawFuZhuClock(CDC* pDc);
	//绘制动画
	VOID DrawMoveAnte(WORD wViewChairID, SCORE lTableScore);
	//移动筹码
	bool MoveJetton();
	//设置语音
	//VOID SetVoicePanelStatus();
	//分析筹码
	VOID AnalyseChip(WORD wChairID, LONGLONG lChip);

	
	//继承函数
private:
	//重置界面
	virtual VOID ResetGameView();
	//调整控件
	virtual VOID RectifyControl(INT nWidth, INT nHeight);
	//绘画界面
	virtual VOID DrawGameView(CDC * pDC, INT nWidth, INT nHeight);
	
	
	//是否特殊绘制
	virtual bool RealizeWIN7() { return true; }

	//消息映射
protected:
	//建立函数
	INT OnCreate(LPCREATESTRUCT lpCreateStruct);
	//定时器
	VOID OnTimer(UINT nIDEvent);
	//鼠标双击
	VOID OnLButtonDblClk(UINT nFlags, CPoint point);
	//鼠标消息
	VOID OnLButtonUp(UINT nFlags, CPoint Point);
	//鼠标消息
	VOID OnLButtonDown(UINT nFlags, CPoint Point);
	//鼠标移动
	VOID OnMouseMove(UINT nFlags, CPoint Point);
	//光标消息
	BOOL OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage);
	

	DECLARE_MESSAGE_MAP()
};

#endif
//////////////////////////////////////////////////////////////////////////
