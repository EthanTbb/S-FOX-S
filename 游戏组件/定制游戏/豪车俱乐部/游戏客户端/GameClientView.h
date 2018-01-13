#ifndef GAME_CLIENT_VIEW_HEAD_FILE
#define GAME_CLIENT_VIEW_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "CardControl.h"
#include "RecordGameList.h"
#include "ApplyUserList.h"
#include "..\游戏服务器\GameLogic.h"
#include "ClientControl.h"
#include "SkinListCtrl.h"
#include "MessageTipDlg.h"
#include "BankTipsDlg.h"
#include "ChatMessage.h"
#include "StringMessage.h"
#include "EditEx.h"
//////////////////////////////////////////////////////////////////////////

//筹码定义
#define JETTON_COUNT				6									//筹码数目
#define JETTON_RADII				68									//筹码半径

//消息定义
#define IDM_PLACE_JETTON			WM_USER+200							//加住信息
#define IDM_APPLY_BANKER			WM_USER+201							//申请信息		
#define IDM_SOUND					WM_USER+206		
//#define IDM_ADMIN_COMMDN			WM_USER+207
#define IDM_CHAT_MESSAGE			WM_USER+208
#define IDM_GAME_OPTIONS			WM_USER+209		
#define IDM_PLACE_XUYA				WM_USER+210							//续压消息


//索引定义
#define INDEX_PLAYER				0									//闲家索引
#define INDEX_BANKER				1									//庄家索引


#define MAX_MOVE_ANIMAL_INDEX       20									//最大索引
#define MAX_DIAL_INDEX				6									//最大索引
#define MAX_END_ANIMAL_INDEX		6									//最大索引

#define BUTTON_COUNT                19
#define SELECT_ANIMAL_COUNT			12
//////////////////////////////////////////////////////////////////////////
//结构定义

//历史记录
#define MAX_SCORE_HISTORY			65									//历史个数
#define MAX_FALG_COUNT				8									//标识个数
#define COUNT_DRAWINFO              20

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


//按钮状态
enum enButtonState 
{ 					
	enNormal,
	enDown,
	enMouseOver,
	enDisabled

};
//所有按钮
enum enButtonName 
{ 					
	enButtonClose=0,
	enButtonStore,
	enButtonMax,
	enButtonMin,
	enButtonOption,
	enButtonApplyBanker,
	enButtonGiveupBanker,
	enButtonDisListUp,
	enButtonDisListDown,
	enButtonListUp,
	enButtonListDown,
	enButtonLuziUp,
	enButtonLuziDown,
	enButtonBanker,
	enButton1w,
	enButton10w,
	enButton50w,
	enButton100w,
	enButton500w,
	enButton1000w
};

//提示状态
enum enTipState 
{ 					
	enNull,									//无状态
	enWaitBanker,							//等待上庄
	enStartBet,								//开始下注
	enEndBet,								//结束下注
	enChangeBanker,							//交换庄家
	enAreaOverFlow,							//超过区域下注额
	enNoMoneyForBanker,						//金币不足，无法上庄
	enBankerNoMoneyForBet,					//庄家金币不足，无法下注
	enUserNoMoneyForBet,					//玩家金币不足，无法下注
	enOverMaxLimitMoney,					//超过玩家最大下注额，无法下注
	enOverThreeBetArea,						//超过三个下注区域
	enOverThisJuMoney,						//超过单局最大下注额
	enBetWillEnd							//下注时间马上结束

};

//记录信息
struct tagClientGameRecord
{
	BYTE							cbAnimal;						   //开中动物

};

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

//游戏视图
class CGameClientView : public CGameFrameViewGDI,public IGamePropertyUseSink,public IExpressionSink
{
	//限制信息,
protected:
	LONGLONG						m_lMeMaxScore;						//最大下注
	LONGLONG						m_lAreaLimitScore;					//区域限制
	LONGLONG						m_lRobotMaxJetton;					//最大筹码

	//下注信息
protected:
	LONGLONG						m_lUserJettonScore[AREA_COUNT+1];	//个人总注
	LONGLONG						m_lAllJettonScore[AREA_COUNT+1];	//全体总注
	LONGLONG						m_lUserAddScore[AREA_COUNT];		//加注信息       
	LONGLONG						m_lAllPlayBet[AREA_COUNT+1];//所有真实玩家下注
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
	WORD							m_wMeChairID;						//我的位置
	LONGLONG						m_lMeCurGameScore;					//我的成绩
	LONGLONG						m_lMeCurGameReturnScore;			//我的成绩
	LONGLONG						m_lBankerCurGameScore;				//庄家成绩
	LONGLONG						m_lGameRevenue;						//游戏税收

	//状态变量
public:
	LONGLONG						m_lCurrentJetton;					//当前筹码
	BYTE							m_cbAreaFlash;						//选中区域
	bool							m_bFlashResult;						//显示结果
	bool							m_bFlashAllArea;					//全部闪烁
	bool							m_bShowGameResult;					//显示结果
	bool							m_bNeedSetGameRecord;				//完成设置	
	int								m_nMultule;							//倍数
	//结束控制
protected:
	bool							m_blRungingCar;						//是否移动
	int								m_nTimerStep;						//移动步值
	int								m_nCarOpenSide;						//开中动物
	int								m_nCurRunIndex;						//当前索引
	int								m_nTotoalRun;						//移动圈数
	int								m_CarIndex;							//当前index

	//位置信息
protected:
	CRect							m_rectBigRect;						//大的矩形
	CRect							m_rectSmallRect;					//小的矩形
// 	CPoint							m_PointJettonLight[AREA_COUNT];		//变亮位置
	CRect						    m_RectJettonScore[AREA_COUNT];		//分数位置
	CPoint							m_BaseAnimalPt[ANIMAL_COUNT];		//位置信息	
	CPoint                          m_LuZiStart;						//路子信息

	//扑克信息
public:	
	BYTE							m_cbTableCard;						//桌面扑克
	tagClientGameRecord				m_GameRecordArrary[MAX_SCORE_HISTORY];	//路单
	int								m_nRecordFirst;						//开始记录
	int								m_nRecordLast;						//最后记录
	int								m_nScoreHead;						//成绩位置
	LONGLONG						m_lMeStatisticScore;				//游戏成绩

	//界面变量
protected:
	CPngImage						m_ViewBackPng;						//背景图片
	CPngImage						m_BackMutiple;						//背景图片
	CPngImage						m_ChangeBanker;						//切换庄家
	CPngImage						m_CurrentBankerTime;				//当前庄数
	CPngImage						m_UserInfo;							//用户信息	
	CPngImage						m_BetFlagMe;						//下注标志
	CPngImage						m_BetFlagTotal;						//下注标志
	CPngImage						m_EndCarInfo;						//结束车型
	CPngImage						m_EndMaskBack;						//结束背景
	CPngImage						m_HistroyCar;						//历史记录
	CPngImage						m_RecordLast;						//历史记录
	CPngImage						m_SelPng;							//转动区域
	CPngImage						m_TimeFree;							//时间标志
	CPngImage						m_TimeBet;							//时间标志
	CPngImage						m_TimeDrawe;						//时间标志

	CPngImage						m_EndImage;							//结束图片
	CPngImage						m_BetBack;							//下注背景
	CPngImage						m_AearMaskLeft;						//选中区域左
	CPngImage						m_AearMaskRight;					//选中区域右

	CPngImage						m_MeBetNum;							//下注字体
	CPngImage						m_ScoreNum;							//下注字体
	CPngImage						m_EndWinScore;						//结束字体
	CPngImage						m_EndLostScore;						//结束字体
	CPngImage						m_TimeNum;							//时钟字体
		
	//按钮图片
	CPngImage						m_pngCloseButton;					//关闭按钮0
	CPngImage						m_pngMinButton;						//最小化1
	CPngImage						m_pngGameOption;					//游戏设置2

	CPngImage						m_pngBtJetton_100;					//筹码信息3
	CPngImage						m_pngBtJetton_1000;					//筹码信息4
	CPngImage						m_pngBtJetton_1W;					//筹码信息5
	CPngImage						m_pngBtJetton_10W;					//筹码信息6
	CPngImage						m_pngBtJetton_100W;					//筹码信息7
	CPngImage						m_pngBtJetton_500W;					//筹码信息8
	CPngImage						m_pngBtJetton_1000W;				//筹码信息9
	CPngImage						m_pngBtJetton_XUYA;					//筹码信息10
	//CPngImage						m_pngBtJetton_5W;					//筹码信息6
	//CPngImage						m_pngBtJetton_5000W;				//筹码信息8
	//CPngImage						m_pngBtJetton_1Y;					//筹码信息9

	CPngImage						m_pngBtBank;						//银行按钮11
	CPngImage						m_pngBtUpList;						//列表按钮12
	CPngImage						m_pngApplyBanker;					//申请庄家13
	CPngImage						m_pngCancerBanker;					//取消申请14
	CPngImage						m_pngBtDownList;					//列表按钮15

	CPngImage						m_pngBtSwitchChat;					//切换按钮16
	CPngImage						m_pngBtSwitchUserList;				//切换按钮17

	CPngImage						m_pngBtSendChat;					//发送消息18
// 	CPngImage						m_pngBtExpress;						//表情按钮19
// 	CPngImage						m_pngBtColor;						//颜色按钮20
// 	CPngImage						m_pngBtTrumpet;						//喇叭按钮21
	
	CPngImage						m_pngCarMaserati;					//玛莎拉蒂
	CPngImage						m_pngCarBMW;						//宝马
	CPngImage						m_pngCarPorsche;					//保时捷
	CPngImage						m_pngCarFerrari;					//法拉利
	CPngImage						m_pngCarJaguar;						//捷豹
	CPngImage						m_pngCarLamborghini;				//兰博基尼
	CPngImage						m_pngCarLandrover;					//路虎
	CPngImage						m_pngCarBenz;						//奔驰
	//筹码区域
	CPngImage						m_pngBetMaserati;					//玛莎拉蒂
	CPngImage						m_pngBetBMW;						//宝马
	CPngImage						m_pngBetPorsche;					//保时捷
	CPngImage						m_pngBetFerrari;					//法拉利
	CPngImage						m_pngBetJaguar;						//捷豹
	CPngImage						m_pngBetLamborghini;				//兰博基尼
	CPngImage						m_pngBetLandrover;					//路虎
	CPngImage						m_pngBetBenz;						//奔驰	
	//聊天资源
	CPngImage						m_ListBack;							//申请列表

	//状态变量
public:
	INT								m_nSwitchIndex;						//切换按钮
	INT								m_nShowValleyIndex;					//显示上庄列表
	CWHArray<WORD>					m_ValleysList;						//上庄列表
	COLORREF						m_texColor;							//文本颜色
	//按钮相关
	bool							m_bTrackmouse[BUTTON_COUNT];		//按钮状态
	enButtonState					m_btState[BUTTON_COUNT];			//按钮状态
	bool							m_bBtShow[BUTTON_COUNT];			//是否显示
	CRect                           m_RectBt[BUTTON_COUNT];				//按钮位置
	CRect							m_RectCurSelect;					//当前选中
	BYTE                            m_cbSelectIndex;					//选中状态
	CString							m_strTotalRoomName;					//房间全名

	//控件变量
public:
	CButton							m_btOpenAdmin;						//系统控制
	HINSTANCE						m_hInst;
	IClientControlDlg			    *m_pAdminControl;					//控制控件
	CEditEx							m_ChatInput;						//输入控件
	CChatMessage					m_ChatMessage;						//聊天窗口
	CSkinListCtrlEx                 m_UserList;							//列表控件
	CGameClientEngine				*m_pGameClientDlg;					//父类指针
	CGameLogic						m_GameLogic;						//游戏逻辑
	CDlgTrumpetItem *               m_pTrumpetItem;                     //喇叭窗口 
	CExpressionControl *			m_pExpressionControl;				//表情窗口

	//函数定义
public:
	//构造函数
	CGameClientView();
	//析构函数
	virtual ~CGameClientView();

	//继承函数
	//重置界面
	virtual VOID ResetGameView();
private:
	
	//调整控件
	virtual VOID RectifyControl(INT nWidth, INT nHeight);
	//绘画界面
	virtual VOID DrawGameView(CDC * pDC, INT nWidth, INT nHeight);

	//WIN7支持
	virtual bool RealizeWIN7() { return true; }
	//释放对象
	virtual VOID Release() { }
	//接口查询
	virtual VOID * QueryInterface(REFGUID Guid, DWORD dwQueryVer);
	//使用道具
	virtual bool OnEventUseProperty(WORD wPropertyIndex);	
	//表情事件
	virtual VOID OnExpressionSelect(CExpression * pUserExpression, tagExpressionInfo * pExpressionInfo);

	//设置函数
public:
	//设置信息
	void SetMeMaxScore(LONGLONG lMeMaxScore);
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
	void SetCardInfo(BYTE cbTableCard);
	//设置筹码
	void SetCurrentJetton(LONGLONG lCurrentJetton);
	//历史记录
	void SetGameHistory(BYTE bcResulte);
	//轮换庄家
	void ShowChangeBanker( bool bChangeBanker );
	//成绩设置
	void SetGameScore(LONGLONG lMeCurGameScore, LONGLONG lMeCurGameReturnScore, LONGLONG lBankerCurGameScore);
	//允许系统做庄
	void EnableSysBanker(bool bEnableSysBanker) {m_bEnableSysBanker=bEnableSysBanker;}
	//设置倍数
	void SetMultule(int nMul){m_nMultule = nMul;}
	//执行剩余所有的缓冲动画
	void PerformAllBetAnimation();
	//设置机器人最大筹码
	//void SetRobotMaxJetton(LONGLONG lRobotMaxJetton){ m_lRobotMaxJetton = lRobotMaxJetton; };
	//更新视图
	void RefreshGameView();
	//刷新按钮
	void ReflashButton(BYTE cbButtonIndex,bool bAll=false);
	//设置移动
	void StartRunCar(int m_nTimerStep);
	//进行移动
	void RuningCar(int m_nTimerStep);
	//聊天消息
	void InsertAllChatMessage(ChatMsg *pMsg,int nMessageType) ;
	//设置状态提示
	void SetStateTips(enTipState newState,bool bTimer=true);
	//设置全名
	void SetTotalRoomName(CString strName);
	//全部闪烁
	void FlashAllArea();
	//控制下注检测
	void ControlBetDetection(BYTE cbViewIndex, LONGLONG lScoreCount, WORD wChair );
			
	//动画函数
public:	
	//开始发牌
	void DispatchCard();
	//结束发牌
	void FinishDispatchCard( bool bRecord = true );
	
	//计算限制
public:
	//最大下注
	LONGLONG GetUserMaxJetton(BYTE cbJettonArea = 0);
	//区域倍率
	void  GetAllWinArea(BYTE bcWinArea[],BYTE bcAreaCount,BYTE InArea);
	//降低筹码
	void ReduceJettonNumber();
	//绘画函数
protected:
	//闪烁动物
	void FlashLightAnimal(CDC *pDC);
	//全部闪烁
	void FlashAllAnimal(CDC *pDC);
	//区域变亮
	void JettonAreaLight(CDC * pDC);

	//界面函数
public:
	//清理筹码
	void CleanUserJetton();
	//设置胜方
	void SetWinnerSide(bool blWin[],bool bSet);
	//艺术字体
	void DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, int nXPos, int nYPos);
	//基本场景
	void DrawBaseScene(CDC *pDC,int nWidth, int nHeight);
	//绘画庄家
	void DrawBankerInfo(CDC *pDC,int nWidth,int nHeight);
	//绘画玩家
	void DrawMeInfo(CDC *pDC,int nWidth,int nHeight);
	//绘画按钮
	void DrawPngButton(CDC *pDC,int buttonIndex);
	//取消闪动
	void ClearAreaFlash() { m_cbAreaFlash = 0xFF; }
	//更新按钮
	VOID UpdateButtonContron();

	//内联函数
public:
	//当前筹码
	inline LONGLONG GetCurrentJetton() { return m_lCurrentJetton; }	
	//取消定时
	void KillCardTime();
	
	//内部函数
private:
	//获取区域
	BYTE GetJettonArea(CPoint MousePoint);
	//绘画数字
	int DrawNumberString(CDC * pDC, LONGLONG lNumber,  CRect rcRect,bool bMeScore=false);
	//绘画时间
	void DrawTimeString(CDC * pDC, LONGLONG lNumber, INT nXPos, INT nYPos);
	//绘画数字
	void DrawNumStrWithSpace(CDC * pDC, LONGLONG lNumber,CRect&rcPrint,INT nFormat=-1);
	//绘画数字
	void DrawEndNum(CDC * pDC, LONGLONG lNumber, INT nXPos, INT nYPos,bool bWin=true);
	//绘画标识
	void DrawWinFlags(CDC * pDC);
	//转换格式
	CString NumberStringWithSpace(CString strNumber);
	//获取索引
	BYTE GetAnimalIndex(BYTE cbAllIndex);
	//结算消息
	void InsertGameEndInfo();
	//重置状态
	void ResetButtonState();
	//按钮状态
	BYTE GetButtonState(int buttonIndex);
	//鼠标消息
	 bool PngButtonOnLButtonDown(UINT nFlags, CPoint Point);	
	//鼠标消息
	 void PngButtonOnLButtonUp(UINT nFlags, CPoint Point);	
	//鼠标移动
	 void PngButtonOnMouseMove(UINT nFlags, CPoint point);
	 //功能按钮
	 void ButtonAction( int buttonIndex);
	 
	
	 //按钮消息
protected: 
	 //关闭按钮	
	 void OnButtonClose();
	 //最大按钮		
	 void OnButtonMax();
	 //恢复按钮	
	 void OnButtonStandard();
	 //最小按钮	
	 void OnButtonMin();
	 //设置按钮	
	 void OnButtonGameOption();
	 //申请按钮	
	 void OnButtonAskForBanker();
	 //取消按钮	
	 void OnButtonGiveupBanker();
	 //切换用户列表	
	 void OnButtonSwitchUserList();
	 //切换聊天	
	 void OnButtonSwitchChat();
	 //上翻按钮	
	 void OnButtonListUp();
	 //下翻按钮	
	 void OnButtonListDown();
	 //银行按钮	
	 void OnButtonGoBanker();
	 //下注按钮	
	 void OnButton100();
	 //下注按钮	
	 void OnButton1000();
	 //下注按钮	
	 void OnButton1w();
	 //下注按钮	
	 void OnButton10w();
	 //下注按钮	
	 void OnButton100w();
	 //下注按钮	
	 void OnButton500w();
	 //下注按钮	
	 void OnButton1000W();
	 //下注按钮	
	 void OnButtonXUYA();
	//上庄按钮
	afx_msg void OnApplyBanker();
	//下庄按钮
	afx_msg void OnCancelBanker();
	//移动按钮
	afx_msg void OnScoreMoveL();
	//移动按钮
	afx_msg void OnScoreMoveR();
	//发送消息
	LRESULT OnSendMessage(WPARAM wParam,LPARAM lParam) ;
	//颜色选择
	void SelectColor();
	//喇叭按钮
	void OnProperty();
	//选择指定颜色
	void OnSelectChatColor(UINT nCommandID);
	//表情按钮
	void OnExpression();
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
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	
	//管理员控制
	afx_msg void OpenAdminWnd();
	DECLARE_MESSAGE_MAP()	
public:
	afx_msg void OnMoreColor();
};

//////////////////////////////////////////////////////////////////////////

#endif
