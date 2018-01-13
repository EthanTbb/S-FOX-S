#ifndef GAME_CLIENT_ENGINE_HEAD_FILE
#define GAME_CLIENT_ENGINE_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "GameLogic.h"
#include "GameClientView.h"
#include "HistoryScore.h"
#include "MessageBox.h"

//////////////////////////////////////////////////////////////////////////

//游戏对话框
class CGameClientEngine : public CGameFrameEngine
{
	//时间定义
protected:
	BYTE							m_cbTimeStartGame;					//开始时间
	BYTE							m_cbTimeCallBanker;					//叫庄时间
	BYTE							m_cbTimeSetChip;					//下注时间
	BYTE							m_cbTimeRangeCard;					//理牌时间
	BYTE							m_cbTimeShowCard;					//开牌时间
	
	BYTE							m_cbFuZhuTime;						//辅助时间

	//控件变量
protected:
	CGameLogic						m_GameLogic;						//游戏逻辑
	CGameClientView					m_GameClientView;					//游戏视图
	CHistoryScore					m_HistroyScore;						//历史成绩

	//配置变量
protected:
	UINT_PTR						m_nTimer ;							//定时器ID
	UINT							m_nCompareTime;						//比较时间
	bool							m_bCanPlayVoice;					//是否语音

	//游戏变量
protected:
	BYTE							m_bHandCardData[GAME_PLAYER][HAND_CARD_COUNT];	//手上扑克
	BYTE							m_bHandCardCount ;					//扑克数目
	BYTE							m_bTmpHandCardData[HAND_CARD_COUNT];//备份扑克
	BYTE							m_bSegmentCard[GAME_PLAYER][3][5];	//分段扑克
	tagSegInfo						m_tagSegInfo;						//临时分段

	WORD							m_wBanker;							//庄家玩家
	SCORE							m_lScoreTimes[GAME_PLAYER][3];		//分数倍数
	SCORE							m_lUserChip[GAME_PLAYER];			//压注大小
	SCORE							m_wMaxChip;							//最大下注
	bool							m_bFinishSegment;					//完成分段
	bool							m_bGameStatus[GAME_PLAYER];			//游戏状态
	bool							m_bRequestSplit;					//是否切牌
	bool							m_bIsShowCard;						//是否确定
	bool							m_bTrustee;							//是否托管
	BYTE							m_bComparedCard;					//特殊牌型

	tagSearchCardResult				m_SearchCardResult;					//搜索结果
	BYTE							m_cbSearchTypeIndex;				//结果索引
	BYTE							m_cbSortTypeIndex;					//排序索引
	DWORD							m_TimerCount;						//时间计数

	bool							m_bCheatRight;						//作弊用户
	bool							m_bUserCheat[GAME_PLAYER];			//作弊标志

	//函数定义
public:
	//构造函数
	CGameClientEngine();
	//析构函数
	virtual ~CGameClientEngine();

	//消息处理
protected:
	//开始叫庄
	bool OnSubBeginCall(const void * pBuffer, WORD wDataSize);
	//玩家叫庄
	bool OnSubUserCall(const void * pBuffer, WORD wDataSize);
	//都不叫庄
	bool OnSubAllNotCall(const void * pBuffer, WORD wDataSize);
	//发送扑克
	bool OnSubSendCardEx(const void * pBuffer, WORD wDataSize);
	//游戏结束
	bool OnSubGameEnd(const void * pBuffer, WORD wDataSize) ;
	//设置下注
	bool OnSubSetChip(const void * pBuffer , WORD wDataSize) ;	
	//开始洗牌
	bool OnSubStartShuffle(const void * pBuffer , WORD wDataSize) ;
	//设置分段
    bool OnSubSetSegment(const void * pBuffer , WORD wDataSize) ;
    //玩家摊牌
	bool OnSubShowCard(const void * pBuffer , WORD wDataSize) ;
	//比较扑克
	bool OnSubCompareCard(const void * pBuffer , WORD wDataSize) ;
	//用户强退
	bool OnSubPlayerExit(const void * pBuffer, WORD wDataSize);
	//用户切牌
	bool OnSubSplitCard(const void * pBuffer, WORD wDataSize);
	//开始发牌
	bool OnSubDispatchStart(const void * pBuffer, WORD wDataSize);
	//用户托管
	bool OnSubTrustee(const void * pBuffer, WORD wDataSize);
	//语音请求
	bool OnSubVoicePhrase(const void * pBuffer, WORD wDataSize);
	//作弊数据
	bool OnSubCheatCard(VOID * pData, WORD wDataSize);

    //辅助函数
protected:
	//自动代打
	void AutomaticOutCard() ;
	//显示牌型
	void ShowAutomaticOutCard(WORD wChairID) ;
	//比牌场景
	void CompareCard(BYTE bSceneName);
	//更新按钮
	void UpdateButtonState();
	//更新按钮
	void UpdateChipButtonState(int nShowCmd, BOOL bEnable);
	//更新按钮
	void UpdateBlockButton(BYTE cbBtnIndex, int nShowCmd, BOOL bEnable);
	//读取短语
	LPCTSTR GetPhraseString(BYTE bGender,BYTE bType,BYTE bIndex);
	//随机短语
	BYTE RandYuYin(BYTE bType,BYTE bGender);

	//常规继承
private:
	//初始函数
	virtual bool OnInitGameEngine();
	//重置框架
	virtual bool OnResetGameEngine();

	//时钟事件
public:
	//时钟删除
	virtual bool OnEventGameClockKill(WORD wChairID);
	//时钟信息
	virtual bool OnEventGameClockInfo(WORD wChairID, UINT nElapse, WORD wClockID);

	//游戏事件
public:
	//旁观消息
	virtual bool OnEventLookonMode(VOID * pData, WORD wDataSize);
	//游戏消息
	virtual bool OnEventGameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//场景消息
	virtual bool OnEventSceneMessage(BYTE cbGameStatus, bool bLookonUser, VOID * pData, WORD wDataSize);

	//用户进入
	virtual VOID OnEventUserEnter(IClientUserItem * pIClientUserItem, bool bLookonUser);
	//用户离开
	virtual VOID OnEventUserLeave(IClientUserItem * pIClientUserItem, bool bLookonUser);

public:
	//声音控制
	virtual bool AllowBackGroundSound(bool bAllowSound){return true;}

	//消息函数
protected:
	//开始消息
	LRESULT OnStart(WPARAM wParam, LPARAM lParam);
	//叫庄消息
	LRESULT OnCallBanker(WPARAM wParam, LPARAM lParam);
	//不叫消息
	LRESULT OnNoCallBanker(WPARAM wParam, LPARAM lParam);
	//压注消息
	LRESULT OnSetChip(WPARAM wParam, LPARAM lParam);
	//切牌消息
	LRESULT OnSplitCard(WPARAM wParam, LPARAM lParam);
	//洗牌完毕
	LRESULT OnSplitFinish(WPARAM wParam, LPARAM lParam);
	//洗牌完毕
	LRESULT OnShuffleFinish(WPARAM wParam, LPARAM lParam);
	//发牌完毕
	LRESULT OnDispatchCardFinish(WPARAM wParam, LPARAM lParam);
	//提示消息
	LRESULT OnPrompt(WPARAM wParam, LPARAM lParam);
	//语音消息
	LRESULT OnVoicePhrase(WPARAM wParam, LPARAM lParam);
	//排序消息
	LRESULT	OnSortCard(WPARAM wParam, LPARAM lParam);	
	//托管消息
	LRESULT OnTrustee(WPARAM wParam, LPARAM lParam);
	//第一注消息
	LRESULT OnFrontCard(WPARAM wParam, LPARAM lParam) ;
	//第二注消息
	LRESULT OnMidCard(WPARAM wParam, LPARAM lParam) ;
	//第三注消息
	LRESULT OnBackCard(WPARAM wParam, LPARAM lParam) ;
	//玩家确定
	LRESULT OnShowCard(WPARAM wParam, LPARAM lParam) ;	
	//左键扑克
	LRESULT OnLeftHitCard(WPARAM wParam, LPARAM lParam) ;
	//语言切换
	LRESULT OnSwitchLang(WPARAM wParam, LPARAM lParam);
	//回收扑克
	LRESULT OnRestoreCard(WPARAM wParam, LPARAM lParam);
	//请求比牌
	LRESULT OnRevesalTurnFinish(WPARAM wParam, LPARAM lParam);
	//特殊理牌
	LRESULT	OnMsgInfo(WPARAM wParam, LPARAM lParam);


	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

	DECLARE_MESSAGE_MAP()
};

#endif
//////////////////////////////////////////////////////////////////////////
