#ifndef DLG_INSURE_COUNTER_HEAD_FILE
#define DLG_INSURE_COUNTER_HEAD_FILE

#pragma once

#include "MissionManager.h"
#include "PasswordControl.h"
#include "WebPublicize.h"
#include "ShareControlHead.h"

//////////////////////////////////////////////////////////////////////////////////
//消息命令
#define WM_INSUREPLAZA_EVENT      (WM_USER+1680)             //银行事件

//命令定义
#define INSURE_COMMAND_UPDATE	  1							//更新命令
#define INSURE_COMMAND_EXCHANGE	  2							//兑换命令			

//更新结构
struct tagInsurePlazaEvent
{
	bool   bHeapData;
	WORD   wCommandID;			
	SCORE  lUserScore;			
	SCORE  lUserInsure;		
};

//////////////////////////////////////////////////////////////////////////////////

//类说明
class CDlgInsureItem;
class CDlgInsureMain;
class CDlgInsureSave;
class CDlgInsureOperater;

//////////////////////////////////////////////////////////////////////////////////

//操作方式
#define INSURE_SAVE					0									//存取方式
#define INSURE_TRANSFER				1									//转帐方式
#define INSURE_RECORD				2									//操作记录

//银行掩码
#define INSURE_MASK_SAVE			0x01								//存取掩码
#define INSURE_MASK_TRANSFER		0x02								//转帐掩码
#define INSURE_MASK_RECORD			0x04								//记录掩码

//////////////////////////////////////////////////////////////////////////////////

//银行信息
struct tagUserInsureInfo
{
	BYTE							cbEnjoinTransfer;					//转帐开关
	WORD							wRevenueTake;						//税收比例
	WORD							wRevenueTransfer;					//税收比例
	WORD							wRevenueTransferMember;				//税收比例
	WORD							wServerID;							//房间标识
	SCORE							lUserScore;							//用户游戏币
	SCORE							lUserInsure;						//银行游戏币
	SCORE							lTransferPrerequisite;				//转帐条件
};

//////////////////////////////////////////////////////////////////////////////////

//银行接口
interface IInsureCounterAction
{
	//查询事件
	virtual VOID PerformQueryInfo()=NULL;	
	//存入事件
	virtual VOID PerformSaveScore(SCORE lSaveScore)=NULL;
	//取出事件
	virtual VOID PerformTakeScore(SCORE lTakeScore, LPCTSTR pszInsurePass)=NULL;
	//开通事件
	virtual VOID PerformEnableInsure(LPCTSTR pszInsurePass,LPCTSTR pszLogonPass)=NULL;
	//转帐事件
	virtual VOID PerformTransferScore(LPCTSTR pszAccounts, SCORE lTransferScore, LPCTSTR pszInsurePass,LPCTSTR pszTransRemark,BYTE cbByNickName)=NULL;
};

//////////////////////////////////////////////////////////////////////////////////

//页面子类
class CDlgInsureItem
{
	//功能函数
public:
	//获取游戏币
	static SCORE GetUserControlScore(LPCTSTR pszScore);

	//功能函数
public:
	//绘画数字
	static VOID DrawNumberString(CDC * pDC, SCORE lScore, INT nXPos, INT nYPos);
	//转换字符
	static VOID SwitchScoreString(SCORE lScore, LPTSTR pszBuffer, WORD wBufferSize);
	//转换字符
	static VOID SwitchScoreFormat(SCORE lScore, UINT uSpace, LPTSTR pszBuffer, WORD wBufferSize);
};

//////////////////////////////////////////////////////////////////////////////////

//存取游戏币
class CDlgInsureSave : public CFGuiWnd
{
	//友元定义
	friend class CDlgInsureOperater;

	//辅助变量
protected:
	bool							m_bSwitchIng;						//转换标志

	//对象指针
protected:
	CDlgInsureMain *				m_pDlgInsureMain;					//窗口指针
	CDlgInsureOperater *			m_pDlgInsureOperater;				//窗口指针
	tagUserInsureInfo *				m_pUserInsureInfo;					//用户信息

	//界面控件
public:
    CEditUI *						m_pEditAmount;						//数量编辑	
	CEditUI *						m_pEditInsurePass;					//密码编辑
	CTextUI *						m_pTextForget;						//忘记密码
	CButtonUI *						m_pButtonSave;						//存款按钮
	CButtonUI *						m_pButtonTake;						//取款按钮
	CButtonUI *						m_pButtonExchange;					//兑换按钮
	//CButtonUI *						m_pButtonRefresh;					//刷新按钮

	//函数定义
public:
	//构造函数
	CDlgInsureSave();
	//析构函数
	virtual ~CDlgInsureSave();

	//绘画接口
public:
	//开始绘画
	virtual VOID OnBeginPaintWindow(HDC hDC);

	//重载函数
public:
	//初始控件
	virtual VOID InitControlUI();
	//消息提醒
	virtual VOID Notify(TNotifyUI &  msg);	

	//控件函数
protected:
	//分数变更
	VOID OnEnChangeScore();
	//存款按钮
	VOID OnBnClickedSaveScore();
	//取款按钮
	VOID OnBnClickedTakeScore();
	//忘记密码
	VOID OnBnClickedForgetInsure();
	//兑换游戏币
	VOID OnBnClickedExChangeScore();	
};

//////////////////////////////////////////////////////////////////////////////////

//转帐游戏币
class CDlgInsureTransfer : public CFGuiWnd
{
	//友元定义
	friend class CDlgInsureOperater;

	//辅助变量
protected:
	bool							m_bSwitchIng;						//转换标志

	//对象指针
protected:
	CDlgInsureMain *				m_pDlgInsureMain;					//窗口指针
	CDlgInsureOperater *			m_pDlgInsureOperater;				//窗口指针
	tagUserInsureInfo *				m_pUserInsureInfo;					//用户信息

	//界面控件
public:
	CEditUI *						m_pEditAccount;						//接收帐号
    CEditUI *						m_pEditAmount;						//赠送金额	
	CEditUI *						m_pEditInsurePass;					//银行密码
	CEditUI *						m_pEditRemarks;						//备注说明

	//界面控件
public:
	CLabelUI *						m_pLabelTarget;						//目标标签
	CButtonUI *						m_pButtonConfirm;					//确认按钮
	CRadioButtonUI *				m_pRadioByUserID;					//单选按钮
	CRadioButtonUI *				m_pRadioByNickName;					//单选按钮

	//函数定义
public:
	//构造函数
	CDlgInsureTransfer();
	//析构函数
	virtual ~CDlgInsureTransfer();

	//绘画接口
public:
	//开始绘画
	virtual VOID OnBeginPaintWindow(HDC hDC);

	//重载函数
public:
	//初始控件
	virtual VOID InitControlUI();
	//消息提醒
	virtual VOID Notify(TNotifyUI &  msg);	

	//控件函数
protected:
	//分数变更
	VOID OnEnChangeScore();
	//转帐按钮
	VOID OnBnClickedTransferScore();
	//依据ID
	VOID OnBnCheckedTransferByUserID();
	//依据昵称
	VOID OnBnCheckedTransferByNickName();
};

//////////////////////////////////////////////////////////////////////////////////
//银行记录
class CDlgInsureRecord : public CFGuiWnd
{
	//友元定义
	friend class CDlgInsureOperater;

	//对象指针
protected:
	CDlgInsureMain *				m_pDlgInsureMain;					//窗口指针
	CDlgInsureOperater *			m_pDlgInsureOperater;				//窗口指针

	//其他控件
public:
	CWebPublicize			        m_PublicizeRecord;					//操作记录

	//函数定义
public:
	//构造函数
	CDlgInsureRecord();
	//析构函数
	virtual ~CDlgInsureRecord();

	//绘画接口
public:
	//开始绘画
	virtual VOID OnBeginPaintWindow(HDC hDC);

	//功能函数
public:
	//刷新纪录
	VOID RefreshInsureRecord();

	//消息函数
public:
	//创建消息
	int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//调整大小
	VOID OnSize(UINT nType, int cx, int cy);

	DECLARE_MESSAGE_MAP()		
};

//////////////////////////////////////////////////////////////////////////////////

//银行操作
class SHARE_CONTROL_CLASS CDlgInsureOperater : public CFGuiWnd
{
	//查询变量
public:
	bool							m_bInitInfo;						//初始标识
	tagUserInsureInfo *				m_pUserInsureInfo;					//银行信息

	//选择控件
public:
	BYTE							m_cbInsureMode;						//银行模式
	BYTE							m_cbInsureMask;						//银行掩码
	CTabButtonUI *					m_TabButton[3];						//选择按钮

	//其他控件
public:
	CLabelUI *						m_pLabelRound;						//环绕标签
	CDlgInsureMain *				m_pDlgInsureMain;					//银行类

	//子项窗口
public:	
	CDlgInsureSave					m_DlgInsureSave;					//存取子项
	CDlgInsureRecord				m_DlgInsureRecord;					//银行记录
	CDlgInsureTransfer				m_DlgInsureTransfer;				//转帐子项	

	//函数定义
public:
	//构造函数
	CDlgInsureOperater();
	//析构函数
	virtual ~CDlgInsureOperater();

	//绘画接口
public:
	//开始绘画
	virtual VOID OnBeginPaintWindow(HDC hDC);

	//重载函数
public:
	//消息提醒
	virtual VOID Notify(TNotifyUI &  msg);
	//初始控件
	virtual VOID InitControlUI();

	//控制函数
public:
	//调整子项
	VOID RectifyTabItem();
	//插入子项
	VOID InsertTabItem(BYTE cbItemMask);
	//删除子项
	VOID DeleteTabItem(BYTE cbItemMask);

	//功能函数
public:
	//设置模式
	VOID SwitchInsureModeView(BYTE cbInsureMode);

	//辅助函数
public:
	//获取颜色
	COLORREF GetItemBackColor() { return RGB(189,173,139); }
	//设置对象
	VOID SetDlgInsureMain(CDlgInsureMain * pDlgInsureMain);
	//设置信息
	VOID SetUserInsureInfo(tagUserInsureInfo * pUserInsureInfo);

	//消息函数
protected:
	//查询按钮
	VOID OnBnClickedQueryInfo();

	//消息函数
public:
	//创建消息
	int OnCreate(LPCREATESTRUCT lpCreateStruct);

	DECLARE_MESSAGE_MAP()	
};

//////////////////////////////////////////////////////////////////////////////////
//

//银行设置
class CDlgInsureSetup : public CFGuiWnd
{
	//对象指针
protected:
	CDlgInsureMain *				m_pDlgInsureMain;					//窗口指针

	//控件指针
public:
	CLabelUI *						m_pLabelRound;						//环绕标签
	CEditUI *						m_pEditLogonPass;					//登录密码
	CEditUI *						m_pEditInsurePass;					//银行密码
    CEditUI *						m_pEditInsurePass1;					//确认密码
	CButtonUI *						m_pButtonEnable;					//开通按钮	

	//函数定义
public:
	//构造函数
	CDlgInsureSetup();
	//析构函数
	virtual ~CDlgInsureSetup();

	//绘画接口
public:
	//开始绘画
	virtual VOID OnBeginPaintWindow(HDC hDC);

	//重载函数
public:
	//初始控件
	virtual VOID InitControlUI();
	//消息提醒
	virtual VOID Notify(TNotifyUI &  msg);	

	//功能函数
public:
	//设置对象
	VOID SetDlgInsureMain(CDlgInsureMain * pDlgInsureMain) { m_pDlgInsureMain=pDlgInsureMain; }

	//控件函数
protected:
	//开通银行
	VOID OnBnClickedEnableInsure();
};

//////////////////////////////////////////////////////////////////////////////////

//银行类
class SHARE_CONTROL_CLASS CDlgInsureMain : public CSkinDialog, public IInsureCounterAction
{
	//变量定义
protected:
	bool							m_bEnableMission;					//开通任务
	bool							m_bSaveMission;						//存款任务
	bool							m_bTakeMission;						//取款任务
	bool							m_bQueryMission;					//查询任务
	bool							m_bTransferMission;					//转帐任务

	//查询变量
public:
	bool							m_bInitInfo;						//初始标识
	tagUserInsureInfo 				m_UserInsureInfo;					//银行信息

	//控件变量
protected:
	CDlgInsureSetup					m_DlgInsureSetup;					//银行设置
	CDlgInsureOperater				m_DlgInsureOperater;				//银行操作

	//函数定义
public:
	//构造函数
	CDlgInsureMain();
	//析构函数
	virtual ~CDlgInsureMain();

	//重载函数
protected:
	//控件绑定
	virtual VOID DoDataExchange(CDataExchange * pDX);
	//消息解释
	virtual BOOL PreTranslateMessage(MSG * pMsg);
	//创建函数
	virtual BOOL OnInitDialog();

	//重载函数
public:
	//确定按钮
	virtual VOID OnOK();

	//窗口函数
protected:
	//绘画标题
	virtual VOID OnDrawSystemTitle(CDC * pDC, INT nWdith, INT nHeight);
	//绘画消息
	virtual VOID OnDrawClientAreaBegin(CDC * pDC, INT nWidth, INT nHeight);

	//事件处理
public:
	//开始任务
	VOID OnMissionStart();
	//终止任务
	VOID OnMissionConclude();

	//辅助函数
protected:
	//调整步骤
	VOID RectifyInsureStep();

	//功能函数
public:
	//查询资料
	VOID QueryUserInsureInfo();	
};

//////////////////////////////////////////////////////////////////////////////////

//广场银行
class SHARE_CONTROL_CLASS CDlgInsurePlaza : public CDlgInsureMain, public CMissionItem
{
	//变量定义
protected:
	SCORE							m_lScore;							//游戏币数目
	BYTE							m_cbByNickName;						//依据昵称
	TCHAR							m_szAccounts[LEN_NICKNAME];			//用户帐号	
	TCHAR							m_szLogonPass[LEN_PASSWORD];		//银行密码
	TCHAR							m_szInsurePass[LEN_PASSWORD];		//银行密码
	TCHAR							m_szTransRemark[LEN_TRANS_REMARK];	//转帐备注

	//组件变量
protected:
	CMissionManager					m_MissionManager;					//任务管理

	//函数定义
public:
	//构造函数
	CDlgInsurePlaza();
	//析构函数
	virtual ~CDlgInsurePlaza();

	//执行函数
protected:
	//查询事件
	virtual VOID PerformQueryInfo();
	//存入事件
	virtual VOID PerformSaveScore(SCORE lSaveScore);	
	//取出事件
	virtual VOID PerformTakeScore(SCORE lTakeScore, LPCTSTR pszInsurePass);
	//开通事件
	virtual VOID PerformEnableInsure(LPCTSTR pszInsurePass,LPCTSTR pszLogonPass);
	//转帐事件
	virtual VOID PerformTransferScore(LPCTSTR pszAccounts, SCORE lTransferScore, LPCTSTR pszInsurePass,LPCTSTR pszTransRemark,BYTE cbByNickName);

	//网络事件
private:
	//连接事件
	virtual bool OnEventMissionLink(INT nErrorCode);
	//关闭事件
	virtual bool OnEventMissionShut(BYTE cbShutReason);
	//读取事件
	virtual bool OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize);
};

//////////////////////////////////////////////////////////////////////////////////

//房间银行
class SHARE_CONTROL_CLASS CDlgInsureServer : public CDlgInsureMain
{
	//变量定义
protected:
	SCORE							m_lScore;							//游戏币数目
	BYTE							m_cbByNickName;						//依据昵称
	TCHAR							m_szAccounts[LEN_NICKNAME];			//用户帐号
	TCHAR							m_szLogonPass[LEN_PASSWORD];		//登录密码
	TCHAR							m_szInsurePass[LEN_PASSWORD];		//银行密码
	TCHAR							m_szTransRemark[LEN_TRANS_REMARK];	//转帐备注

	//变量定义
protected:
	ITCPSocket *					m_pITCPSocket;						//网络接口
	IClientUserItem *				m_pIMySelfUserItem;					//自己指针

	//函数定义
public:
	//构造函数
	CDlgInsureServer();
	//析构函数
	virtual ~CDlgInsureServer();

	//执行函数
protected:
	//查询事件
	virtual VOID PerformQueryInfo();	
	//存入事件
	virtual VOID PerformSaveScore(SCORE lSaveScore);	
	//取出事件
	virtual VOID PerformTakeScore(SCORE lTakeScore, LPCTSTR pszInsurePass);
	//开通事件
	virtual VOID PerformEnableInsure(LPCTSTR pszInsurePass,LPCTSTR pszLogonPass);
	//转帐事件
	virtual VOID PerformTransferScore(LPCTSTR pszAccounts, SCORE lTransferScore, LPCTSTR pszInsurePass,LPCTSTR pszTransRemark,BYTE cbByNickName);

	//功能函数
public:
	//配置银行
	VOID Initialization(ITCPSocket * pITCPSocket, IClientUserItem * pIMySelfUserItem);
	//银行消息
	bool OnServerInsureMessage(WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize);
};

//////////////////////////////////////////////////////////////////////////////////

#endif