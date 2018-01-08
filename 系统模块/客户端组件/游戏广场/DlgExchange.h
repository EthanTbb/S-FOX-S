#ifndef DLG_EXCHANGE_HEAD_FILE
#define DLG_EXCHANGE_HEAD_FILE
#pragma once


///////////////////////////////////////////////////////////////////////////////////////////////////
//兑换窗口
class CDlgExchange : public CSkinDialog
{
	//显示变量
protected:
	SCORE							m_lExchangeScore;					//兑换积分
	SCORE							m_lConsumeBeans;					//消耗游戏豆
	SCORE							m_lPresentScore;					//赠送积分

	//参数变量
protected:
	DWORD							m_dwExchangeRate;					//兑换比率
	WORD							m_wMemberCount;						//会员数目
	tagMemberParameter				m_MemberParameter[10];				//会员参数

	//函数定义
public:
	//构造函数
	CDlgExchange();
	//析构函数
	virtual ~CDlgExchange();

    //接口函数
public:
	//初始控件
	virtual void InitControlUI();
	//消息提醒
	virtual void Notify(TNotifyUI &  msg);	

	//重载函数
public:
	//皮肤名称
	virtual LPCTSTR GetSkinFile() { return TEXT("DlgExchange.xml"); }

	//重载函数
protected:
	//创建函数
	virtual BOOL OnInitDialog();
	//消息解释
	virtual BOOL PreTranslateMessage(MSG * pMsg);

	//绘画接口
public:
	//绘画标题
	virtual VOID OnDrawSystemTitle(CDC * pDC, INT nWdith, INT nHeight);	
	//绘画消息
	virtual VOID OnDrawClientAreaEnd(CDC * pDC, INT nWidth, INT nHeight);

	//事件处理
public:
	//开始任务
	virtual VOID OnMissionStart();
	//终止任务
	virtual VOID OnMissionConclude();

	//执行函数
protected:
	//兑换游戏币
	virtual VOID PerformExchangeScore();
	//购买会员
	virtual VOID PerformPurchaseMember();
	//加载参数
	virtual VOID PerformLoadExchangeParam();

	//控件事件
protected:
	//确认兑换
	VOID OnClickedConfirmExchange();
	//确认购买
	VOID OnClickedConfirmPurchase();
	//更多兑换
	VOID OnTextLinkMoreExchange();
	//文本变更
	VOID OnEditIngotTextChange();
	//子项变更
	VOID OnComboControlItemChange();

	//辅助函数
protected:
	//获取数字
	SCORE GetUserControlNumber(LPCTSTR pszNumber);
	//转换字符
	VOID SwitchScoreFormat(SCORE lScore, UINT uSpace, LPTSTR pszBuffer, WORD wBufferSize);
};

///////////////////////////////////////////////////////////////////////////////////////////////////
//大厅兑换
class CDlgExchangePlaza : public CDlgExchange,public CMissionItem
{
	//控制变量
protected:
	bool							m_bLoadExchangeParam;				//加载参数
	bool							m_bPurchaseMember;					//购买会员
	bool							m_bExchangeScore;					//兑换游戏币

	//组件变量
protected:
	CMissionManager					m_MissionManager;					//任务管理

	//函数定义
public:
	//构造函数
	CDlgExchangePlaza();
	//析构函数
	virtual ~CDlgExchangePlaza();

	//网络事件
public:
	//连接事件
	virtual bool OnEventMissionLink(INT nErrorCode);
	//关闭事件
	virtual bool OnEventMissionShut(BYTE cbShutReason);
	//读取事件
	virtual bool OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize);

	//执行函数
protected:
	//兑换游戏币
	virtual VOID PerformExchangeScore();
	//购买会员
	virtual VOID PerformPurchaseMember();
	//加载参数
	virtual VOID PerformLoadExchangeParam();
};

///////////////////////////////////////////////////////////////////////////////////////////////////

//房间兑换
class CDlgExchangeServer : public CDlgExchange
{
	//变量定义
protected:
	ITCPSocket *					m_pITCPSocket;						//网络接口

	//函数定义
public:
	//构造函数
	CDlgExchangeServer();
	//析构函数
	virtual ~CDlgExchangeServer();

	//执行函数
protected:
	//兑换游戏币
	virtual VOID PerformExchangeScore();
	//购买会员
	virtual VOID PerformPurchaseMember();
	//加载参数
	virtual VOID PerformLoadExchangeParam();

	//功能函数
public:
	//配置任务
	VOID Initialization(ITCPSocket * pITCPSocket);
	//任务消息
	bool OnExchangeMessage(WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize);
};

///////////////////////////////////////////////////////////////////////////////////////////////////

#endif
