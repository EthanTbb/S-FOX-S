#ifndef TABLE_VIEW_FRAME_HEAD_FILE
#define TABLE_VIEW_FRAME_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "Resource.h"
#include "GamePlazaHead.h"
#include "PlatformPublicize.h"

//////////////////////////////////////////////////////////////////////////////////

//进入索引
#define INDEX_ENTER_CHAIR			MAX_CHAIR							//进入区域

//按钮标识
#define BT_JOIN_MATCH				1									//加入比赛
#define BT_GROUP_MATCH				2									//分组比赛
#define BT_QUIT_MATCH				3									//退出比赛
#define BT_REVIVE_MATH				4									//复活比赛	
#define BT_JOIN_GAME				5									//加入游戏

//////////////////////////////////////////////////////////////////////////////////

//桌子属性
struct tagTableAttribute
{
	//桌子标志
	bool							bLocker;							//密码标志
	bool							bPlaying;							//游戏标志
	bool							bFocusFrame;						//框架标志

	//桌子状态
	WORD							wWatchCount;						//旁观数目
	DWORD							dwTableOwnerID;						//桌主索引

	//属性变量
	WORD							wTableID;							//桌子号码
	WORD							wChairCount;						//椅子数目
	IClientUserItem *				pIClientUserItem[MAX_CHAIR];		//用户信息
};

//比赛位置
struct tagMatchPosition
{	
	//模式信息
	BYTE							cbPostionMode;						//位置模式

	//比赛位置
	CPoint							ptMatchShow;						//展示位置
	CPoint							ptMatchText;						//文本位置
	CPoint							ptMatchWaitPlay;					//等待开始

	//奖励位置
	CPoint							ptMatchRank;						//名称位置	
	CPoint							ptMatchReward;						//奖励位置
	CPoint							ptRewardSize;						//奖励大小
	CPoint							ptRewardOffset;						//偏移位置	

	//信息位置
	CRect							rcMatchMode;						//比赛模式
	CRect							rcMatchCondition;					//开始条件
	CRect							rcMatchFee;							//报名费用
	CRect							rcMatchStartTime;					//开赛时间	
	CRect							rcMatchEndTime;						//结束时间
	CRect							rcMatchSingupEnd;					//报名截止
	CRect							rcStartCount;						//开始人数
	CRect							rcPalyCount;						//游戏局数
	CRect							rcMatchPlayCount;					//最少局数
	CRect							rcRankingMode;						//排名方式

	//数字位置
	CPoint							ptNumber1;							//数字位置
	CPoint							ptNumber2;							//数字位置
	CPoint							ptNumber3;							//数字位置

	//按钮位置
	CPoint							ptMatchJoin;						//加入比赛
	CPoint							ptMatchDetails;						//比赛详情
};

//////////////////////////////////////////////////////////////////////////////////

//桌子资源
class CTableResource
{
	//桌子属性
public:
	CSize							m_SizeChair;						//椅子尺寸
	CSize							m_SizeTable;						//桌子尺寸
	CSize							m_SizeEnter;						//进入尺寸
	CSize                           m_SizeNumberBK;                     //桌号尺寸
	CSize                           m_SizeNumberNB;                     //桌号尺寸

	//属性变量
public:
	bool							m_bShowUser;						//显示用户
	bool							m_bShowEnter;						//显示进入
	bool							m_bShowChair;						//显示椅子
	bool							m_bShowTable;						//显示桌子
	bool							m_bShowFocus;						//显示焦点
	bool							m_bCustomUser;						//自定用户
	bool							m_bShowTableID;						//显示桌号
	bool							m_bRenderImage;						//渲染标志
	bool							m_bShowCellScore;					//显示底注

	//数目变量
public:
	WORD							m_wTableItemCount;					//子项数目
	WORD							m_wChairItemCount;					//子项数目

	//类型数目
public:
	WORD							m_wUserGenreCount;					//类型数目
	WORD							m_wTableGenreCount;					//类型数目
	WORD							m_wChairGenreCount;					//类型数目

	//动画变量
public:
	CPoint							m_ptAnimation;						//动画位置
	INT								m_nAnimationFrame;					//分组帧数		

	//位置属性
public:
	CPoint							m_ptLock;							//桌锁位置
	CPoint							m_ptEnter;							//进入位置
	CPoint							m_ptTableID;						//桌号位置
	CPoint							m_ptReadyArray[MAX_CHAIR];			//准备位置	

	//框架位置
public:
	CRect							m_rcNameArray[MAX_CHAIR];			//名字位置
	CRect							m_rcChairArray[MAX_CHAIR];			//椅子位置

	//颜色属性
public:
	COLORREF						m_crName;							//名字颜色
	COLORREF						m_crMember;							//会员颜色
	COLORREF						m_crMaster;							//管理颜色

	//辅助变量
public:
	INT								m_nDrawStyle[MAX_CHAIR];			//输出格式
	TCHAR							m_szDirectory[MAX_PATH];			//资源路径

	//外部资源
public:
	CPngImage						m_ImageEnter;						//进入位图
	CBitImage						m_ImageChair;						//椅子位图
	CBitImage						m_ImageTable;						//桌子位图	
	CBitImage						m_ImageGround;						//背景位图
	CBitImage						m_ImageWaitDistribute;				//桌子位图
	CPngImage						m_ImageAnimation;					//分组动画
	CPngImage						m_ImageCustomUser;					//自定位图
	CPngImage						m_ImageCellNumber;					//底注位图

	//内部资源
public:
	CPngImage						m_ImageReady;						//准备位图
	CPngImage						m_ImageLocker;						//桌锁位图
	CPngImage						m_ImageNumberNB;					//号码位图
	CPngImage						m_ImageNumberBK;					//号码背景
	//CBitImage						m_ImageUserFace;					//用户头像
	CPngImage						m_ImgGroupUserFace[255];			//用户头像

	//静态变量
protected:
	static CTableResource *			m_pTableResource;					//桌子资源

	//函数定义
public:
	//构造函数
	CTableResource();
	//析构函数
	virtual ~CTableResource();

	//功能函数
public:
	//释放资源
	VOID DestroyResource();
	//加载资源
	bool LoadResource(LPCTSTR pszResDirectory, WORD wChairCount, bool bAvertCheatMode);	
	//更新资源
	bool UpdateResource(CSkinRenderManager * pSkinRenderManager);

	//辅助函数
public:
	//读取数值
	LONG ReadStringValue(LPCTSTR & pszSring);
	//读取矩形
	bool ReadRect(RECT & ValueRect, LPCTSTR pszItem, LPCTSTR pszSubItem, LPCTSTR pszIniFile);
	//读取坐标
	bool ReadPoint(POINT & ValuePoint, LPCTSTR pszItem, LPCTSTR pszSubItem, LPCTSTR pszIniFile);
	//读取颜色
	bool ReadColor(COLORREF & ValueColor, LPCTSTR pszItem, LPCTSTR pszSubItem, LPCTSTR pszIniFile);

	//静态函数
public:
	static CTableResource * GetInstance() { return m_pTableResource; }
};

//////////////////////////////////////////////////////////////////////////////////

//桌子视图
class CTableView : public ITableView
{
	//状态变量
protected:
	bool							m_bMouseDown;						//是否按下
	WORD							m_wHoverChairID;					//盘旋椅子	
	LONG							m_lCellScore;						//桌子底分
	tagTableAttribute				m_TableAttribute;					//桌子属性

	//组件接口
protected:
	ITableViewFrame *				m_pITableViewFrame;					//桌子接口

	//函数定义
public:
	//构造函数
	CTableView();
	//析构函数
	virtual ~CTableView();

	//基础接口
public:
	//释放对象
	virtual VOID Release() { delete this;}
	//接口查询
	virtual VOID * QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	//功能接口
public:
	//空椅子数
	virtual WORD GetNullChairCount(WORD & wNullChairID);
	//配置函数
	virtual VOID InitTableView(WORD wTableID, WORD wChairCount, ITableViewFrame * pITableViewFrame);

	//用户接口
public:
	//获取用户
	virtual IClientUserItem * GetClientUserItem(WORD wChairID);
	//设置信息
	virtual bool SetClientUserItem(WORD wChairID, IClientUserItem * pIClientUserItem);

	//状态接口
public:
	//鼠标按下
	virtual VOID SetMouseDown(bool bMouseDown);
	//焦点框架
	virtual VOID SetFocusFrame(bool bFocusFrame);
	//盘旋位置
	virtual VOID SetHoverStatus(WORD wHoverChairID);
	//桌子状态 
	virtual VOID SetTableStatus(bool bPlaying, bool bLocker,LONG lCellScore);

	//查询接口
public:
	//游戏标志
	virtual bool GetPlayFlag() { return m_TableAttribute.bPlaying; }
	//密码标志
	virtual bool GetLockerFlag() { return m_TableAttribute.bLocker; }

	//功能函数
public:
	//绘画函数
	VOID DrawTableView(CDC * pDC, DWORD dwServerRule, CTableResource * pTableResource);
	//绘画名字
	VOID DrawTableName(CDC * pDC, WORD wChairID, LPCTSTR pszNickName, CTableResource * pTableResource);
	//绘画头像
	VOID DrawUserAvatar(CDC * pDC, INT nXPos, INT nYPos, IClientUserItem * pIClientUserItem, CTableResource * pTableResource);
	//绘画头像
	VOID DrawUserAvatar(CDC * pDC, INT nXPos, INT nYPos, INT nWidth, INT nHeight, IClientUserItem * pIClientUserItem, CTableResource * pTableResource);
	//绘画数字
	VOID DrawNumberString(CDC * pDC, CPngImage &PngNumber, LONG lNumber, INT nXPos, INT nYPos,BYTE cbAlign=TA_LEFT);
};

//////////////////////////////////////////////////////////////////////////////////

//数组定义
typedef CWHArray<CTableView *>		CTableViewArray;					//桌子数组


//////////////////////////////////////////////////////////////////////////////////

//桌子框架
class CTableViewFrame : public CWnd, public ITableViewFrame
{
	//友元定义
	friend class CTableView;

	//状态信息
protected:
	bool							m_bHovering;						//盘旋标志
	bool							m_bLMouseDown;						//右键标志
	bool							m_bRMouseDown;						//左键标志
	bool                            m_bWaitDistribute;                  //等待分配
	BYTE							m_cbButtonDown;						//按钮掩码
	BYTE							m_cbButtonHover;					//按钮掩码
	INT								m_nAnimalFrameIndex;				//动画索引

	//索引变量
protected:
	WORD							m_wDownTableID;						//桌子号码
	WORD							m_wDownChairID;						//椅子号码

	//属性变量
protected:
	WORD							m_wTableCount;						//游戏桌数
	WORD							m_wChairCount;						//椅子数目
	DWORD							m_dwServerRule;						//房间规则
	WORD							m_wServerType;						//房间类型

	//滚动信息
protected:
	INT								m_nXExcursion;						//横行偏移
	INT								m_nYExcursion;						//竖向偏移
	INT								m_nXTableCount;						//桌子列数
	INT								m_nYTableCount;						//桌子行数

	//滚动信息
protected:
	INT								m_nScrollPos;						//滚动位置
	INT								m_nScrollPosMax;					//最大位置
	INT								m_nVisibleLineCount;				//可见行数

	//控件变量
public:
	CSkinScrollBar					m_SkinScrollBar;					//滚动条类
	CTableResource					m_TableResource;					//桌子资源
	CTableViewArray					m_TableViewArray;					//桌子数组

	//组件接口
protected:
	ITableViewFrameSink *			m_pITableViewFrameSink;				//通知接口

	//函数定义
public:
	//构造函数
	CTableViewFrame();
	//析构函数
	virtual ~CTableViewFrame();

	//基础接口
public:
	//释放对象
	virtual VOID Release() { delete this; }
	//接口查询
	virtual VOID * QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	//配置接口
public:
	//创建桌子
	virtual bool CreateTableFrame(CWnd * pParentWnd, UINT uWndID, IUnknownEx * pIUnknownEx);
	//配置桌子
	virtual bool ConfigTableFrame(WORD wTableCount, WORD wChairCount, DWORD dwServerRule, WORD wServerType, WORD wServerID,LPCTSTR pszResDirectory);

	//信息接口
public:
	//桌子数目
	virtual WORD GetTableCount() { return m_wTableCount; }
	//椅子数目
	virtual WORD GetChairCount() { return m_wChairCount; }
	//获取资源
	virtual CTableResource * GetTableResource() { return &m_TableResource; }

	//视图接口
public:
	//视图窗口
	virtual CWnd * GetViewWnd() { return this; }
	//视图数据
	virtual bool OnEventViewData(WORD wCmdID, VOID * pData, WORD wDataSize) { return true; } 

	//用户接口
public:
	//获取用户
	virtual IClientUserItem * GetClientUserItem(WORD wTableID, WORD wChairID);
	//设置信息
	virtual bool SetClientUserItem(WORD wTableID, WORD wChairID, IClientUserItem * pIClientUserItem);

	//状态接口
public:
	//游戏标志
	virtual bool GetPlayFlag(WORD wTableID);
	//密码标志
	virtual bool GetLockerFlag(WORD wTableID);
	//焦点框架
	virtual VOID SetFocusFrame(WORD wTableID, bool bFocusFrame);
	//桌子状态 
	virtual VOID SetTableStatus(bool bWaitDistribute);
	//桌子状态 
	virtual VOID SetTableStatus(WORD wTableID, bool bPlaying, bool bLocker,LONG lCellScore);	

	//视图控制
public:
	//桌子可视
	virtual bool VisibleTable(WORD wTableID);
	//闪动桌子
	virtual bool FlashGameTable(WORD wTableID);
	//闪动椅子
	virtual bool FlashGameChair(WORD wTableID, WORD wChairID);

	//功能接口
public:
	//更新桌子
	virtual bool UpdateTableView(WORD wTableID);
	//获取桌子
	virtual ITableView * GetTableViewItem(WORD wTableID);
	//空椅子数
	virtual WORD GetNullChairCount(WORD wTableID, WORD & wNullChairID);

	//重载函数
protected:
	//消息解释
	virtual BOOL PreTranslateMessage(MSG * pMsg);

	//辅助函数
protected:
	//桌子测试
	WORD SwitchTableID(POINT MousePoint);
	//椅子测试
	WORD SwitchChairID(POINT MousePoint);
	//调整控件
	VOID RectifyControl(INT nWidth, INT nHeight);
	//绘画数字
	VOID DrawNumberString(CDC * pDC, CPngImage &PngNumber, LONG lNumber, INT nXPos, INT nYPos,BYTE cbAlign=TA_LEFT);

	//消息函数
protected:
	//重画消息
	VOID OnPaint();
	//销毁消息
	VOID OnNcDestroy();
	//时钟消息
	VOID OnTimer(UINT nIDEvent);
	//位置消息
	VOID OnSize(UINT nType, INT cx, INT cy);	
	//鼠标消息
	VOID OnRButtonUp(UINT nFlags, CPoint MousePoint);
	//鼠标消息
	VOID OnLButtonUp(UINT nFlags, CPoint MousePoint);
	//鼠标离开
	LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
	//鼠标消息
	VOID OnRButtonDown(UINT nFlags, CPoint MousePoint);
	//鼠标消息
	VOID OnLButtonDown(UINT nFlags, CPoint MousePoint);
	//鼠标消息
	VOID OnLButtonDblClk(UINT nFlags, CPoint MousePoint);
	//光标消息
	BOOL OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage);
	//鼠标消息
	BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint MousePoint);
	//滚动消息
	VOID OnVScroll(UINT nSBCode, UINT nPos, CScrollBar * pScrollBar);

	//自定消息
protected:
	//事件消息
	LRESULT OnMessagePlatformEvent(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

//比赛框架
class CMatchViewFrame : public CFGuiWnd, public ITableViewFrame
{
	//友元定义
	friend class CTableView;

	//状态信息
protected:
	bool							m_bHovering;						//盘旋标志
	BYTE							m_cbButtonDown;						//按钮掩码
	BYTE							m_cbButtonHover;					//按钮掩码

	//属性变量
protected:
	WORD							m_wServerID;						//房间标识
	WORD							m_wTableCount;						//游戏桌数
	WORD							m_wChairCount;						//椅子数目

	//属性变量
protected:
	tagGameMatch 					m_GameMatch;						//比赛信息
	tagMatchPosition				m_MatchPosition;					//比赛位置

	//控件变量
protected:	
	CButtonUI *						m_pBtMatchJoin;						//参加比赛
	CButtonUI *						m_pBtMatchGroup;					//比赛分组
	CButtonUI *						m_pBtMatchRevive;					//比赛分组	
	CButtonUI *						m_pBtMatchQuit;						//退出比赛
	CButtonUI *						m_pBtMatchDetails;					//比赛详情

	//控件变量
public:
	CTableViewArray					m_TableViewArray;					//桌子数组

//	//比赛变量
//public:
//	DWORD							m_dwWaitting;						//等待人数
//	DWORD							m_dwTotalUser;						//参赛人数
//	DWORD							m_dwBestRank;						//最好成绩
//	DWORD							m_dwMatchCount;						//参数次数

	//比赛变量
public:
	CTime							m_tStartTime;						//开始时间
	DWORD							m_dwWaitting;						//等待人数
	DWORD							m_dwTotalUser;						//参赛人数				
	WORD							m_wLackUserCount;					//缺少人数
	BYTE							m_cbMatchStatus;					//比赛状态
	BYTE							m_cbUserMatchStatus;				//比赛状态

	//资源变量
public:
	CBitImage						m_ImageGround;						//背景环绕
	CPngImage						m_ImageStatus;						//状态文字
	CPngImage						m_ImageNumber1;						//数字图片
	CPngImage						m_ImageNumber2;						//时间数字	
	CPngImage						m_ImageMatchRank;					//比赛名次
	CPngImage						m_ImageMatchShow;					//比赛展示
	CPngImage						m_ImageMatchText;					//比赛文字
	CPngImage						m_ImageMatchBack;					//比赛背景	
	CPngImage						m_ImageMatchWaitPlay;				//状态文字

	//组件接口
protected:
	ITableViewFrameSink *			m_pITableViewFrameSink;				//通知接口

	//函数定义
public:
	//构造函数
	CMatchViewFrame();
	//析构函数
	virtual ~CMatchViewFrame();

	//基础接口
public:
	//释放对象
	virtual VOID Release() { delete this; }
	//接口查询
	virtual VOID * QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	//接口函数
public:
	//消息提醒
	virtual void Notify(TNotifyUI &  msg);	

	//绘画接口
public:
	//开始绘画
	virtual void OnBeginPaintWindow(HDC hDC);

	//配置接口
public:
	//创建桌子
	virtual bool CreateTableFrame(CWnd * pParentWnd, UINT uWndID, IUnknownEx * pIUnknownEx);
	//配置桌子
	virtual bool ConfigTableFrame(WORD wTableCount, WORD wChairCount, DWORD dwServerRule, WORD wServerType, WORD wServerID,LPCTSTR pszResDirectory);

	//信息接口
public:
	//桌子数目
	virtual WORD GetTableCount() { return m_wTableCount; }
	//椅子数目
	virtual WORD GetChairCount() { return m_wChairCount; }

	//视图接口
public:
	//视图窗口
	virtual CWnd * GetViewWnd() { return this; }
	//视图数据
	virtual bool OnEventViewData(WORD wCmdID, VOID * pData, WORD wDataSize);

	//用户接口
public:
	//获取用户
	virtual IClientUserItem * GetClientUserItem(WORD wTableID, WORD wChairID);
	//设置信息
	virtual bool SetClientUserItem(WORD wTableID, WORD wChairID, IClientUserItem * pIClientUserItem);

	//状态接口
public:
	//游戏标志
	virtual bool GetPlayFlag(WORD wTableID);
	//密码标志
	virtual bool GetLockerFlag(WORD wTableID);
	//焦点框架
	virtual VOID SetFocusFrame(WORD wTableID, bool bFocusFrame);
	//桌子状态 
	virtual VOID SetTableStatus(WORD wTableID, bool bPlaying, bool bLocker,LONG lCellScore);
	//桌子状态 
	virtual VOID SetTableStatus(bool bWaitDistribute);

	//视图控制
public:
	//桌子可视
	virtual bool VisibleTable(WORD wTableID);
	//闪动桌子
	virtual bool FlashGameTable(WORD wTableID);
	//闪动椅子
	virtual bool FlashGameChair(WORD wTableID, WORD wChairID);

	//功能接口
public:
	//更新桌子
	virtual bool UpdateTableView(WORD wTableID);
	//获取桌子
	virtual ITableView * GetTableViewItem(WORD wTableID);
	//空椅子数
	virtual WORD GetNullChairCount(WORD wTableID, WORD & wNullChairID);

	//辅助函数
protected:			
	//比赛状态
	VOID SetMatchStatus(BYTE cbMatchStatus);
	//玩家参赛状态
	VOID SetUserMatchStatus(BYTE cbUserMatchStatus);
	//调整控件
	VOID RectifyControl(INT nWidth, INT nHeight);
	//加载资源
	bool LoadResource(LPCTSTR pszResDirectory);
	//创建控件
	bool CreateControlUI(LPCTSTR pszMatchDirectory);
	//判断目录
	bool IsDirectoryExist(LPCTSTR pszDirectofy);

	//绘画函数
protected:
	//绘制文字
	VOID DrawWords(CDC * pDC,INT nXPos,INT nYPos,CPngImage * pWordsImage,LPCTSTR pszWords,LPCTSTR pszText,UINT nFormat=DT_LEFT);
	//绘制图片
	VOID DrawNumber(CDC * pDC,INT nXPos,INT nYPos,CPngImage * pNumberImage,LPCTSTR pszNumber,INT nNumber,LPCTSTR pszFormat,UINT nFormat=DT_LEFT);

	//消息函数
protected:
	//销毁消息
	VOID OnNcDestroy();
	//时钟消息
	VOID OnTimer(UINT nIDEvent);
	//位置消息
	VOID OnSize(UINT nType, INT cx, INT cy);

	DECLARE_MESSAGE_MAP()
};

#endif