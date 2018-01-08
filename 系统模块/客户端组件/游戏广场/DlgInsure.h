#ifndef DLG_DLGINSURE_HEAD_FILE
#define DLG_DLGINSURE_HEAD_FILE

#pragma once
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

//前置声明
class CInsureItemWealth;
class CInsureItemAccess;
class CInsureItemPresent;
//////////////////////////////////////////////////////////////////////////////////
//大厅银行
class CDlgInsure : public CFGuiDialog, public CMissionItem
{
public:
	//构造函数
	CDlgInsure();
	//析构函数
	virtual ~CDlgInsure();

	//MFC重载
public:
	//创建函数
	virtual BOOL OnInitDialog();
	//确定函数
	virtual VOID OnOK();
	//取消消息
	virtual VOID OnCancel();

	//FGUI重载
public:
	//初始控件
	virtual VOID InitControlUI();
	//消息提醒
    virtual void Notify(TNotifyUI &  msg);
	//皮肤名称
	virtual LPCTSTR GetSkinFile();

	//网络事件
public:
	//连接事件
	virtual bool OnEventMissionLink(INT nErrorCode);
	//关闭事件
	virtual bool OnEventMissionShut(BYTE cbShutReason);
	//读取事件
	virtual bool OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize);

	//事件处理
public:
	//开始任务
	VOID OnMissionStart();
	//终止任务
	VOID OnMissionConclude();

public:
	//查询事件
	virtual VOID PerformQueryRebate();
	//查询事件
	virtual VOID PerformQueryInfo();
	//开通事件
	virtual VOID PerformEnableInsure(LPCTSTR pszInsurePass,LPCTSTR pszLogonPass);
	//查询事件
	virtual VOID QueryUserInsureInfo();
	//存入事件
	virtual VOID PerformSaveScore(SCORE lSaveScore);	
	//取出事件
	virtual VOID PerformTakeScore(SCORE lTakeScore, LPCTSTR pszInsurePass);
	//转帐事件
	virtual VOID PerformTransferScore(LPCTSTR pszAccounts, SCORE lTransferScore, LPCTSTR pszInsurePass,LPCTSTR pszTransRemark,BYTE cbByNickName);

	//用户函数
public:
	tagUserInsureInfo* GetUserInsureInfo();

	//缓冲数据
	VOID InitData();
	VOID LoadDataFromMemory();
	VOID LoadDataFromUI();
	VOID UpdateData();
	//界面操控
	VOID InitUI();
	VOID UpdateUI();
	//界面逻辑
	VOID OnDefault();
	VOID DoApply();
	//隐藏存取
	void ShowPresent();

public:
	CContainerUI* GetWealthContainer();
	CContainerUI* GetAccessContainer();
	CContainerUI* GetPresentContainer();
	CContainerUI* GetRecordContainer();

	//子项变量
protected:
	CInsureItemWealth*		m_pItemWealth;						//我的资产
	CInsureItemAccess*		m_pItemAccess;						//游戏币存取
	CInsureItemPresent*		m_pItemPresent;						//游戏币赠送

	//控件变量
protected:
	CContainerUI*			m_pContainerWealth;					//我的资产
	CContainerUI*			m_pContainerAccess;					//游戏币存取
	CContainerUI*			m_pContainerPresent;				//游戏币赠送
	CContainerUI*			m_pContainerRecord;					//操作记录

	//组件变量
protected:
	CMissionManager			m_MissionManager;					//任务管理

	//变量定义
protected:
	//查询变量
public:
	bool					m_bInitInfo;						//初始标识
	tagUserInsureInfo 		m_UserInsureInfo;					//银行信息

	bool					m_bEnableMission;					//开通任务
	bool					m_bSaveMission;						//存款任务
	bool					m_bTakeMission;						//取款任务
	bool					m_bQueryMission;					//查询任务
	bool					m_bTransferMission;					//转帐任务
	bool					m_bQueryRebateMission;				//查询返利

	SCORE					m_lScore;							//游戏币数目
	BYTE					m_cbByNickName;						//依据昵称
	DWORD					m_dwTargetUserID;					//目标ID
	TCHAR					m_szAccounts[LEN_NICKNAME];			//用户帐号	
	TCHAR					m_szLogonPass[LEN_PASSWORD];		//银行密码
	TCHAR					m_szInsurePass[LEN_PASSWORD];		//银行密码
	TCHAR					m_szTransRemark[LEN_TRANS_REMARK];	//转帐备注

	bool					m_bIsTransfer;

	BYTE					cbRebateEnabled;					//使能标识	
	SCORE					lRebateIngot;						//返利元宝
	SCORE					lRebateLoveLiness;					//返利魅力值
};


//////////////////////////////////////////////////////////////////////////////////
//我的资产（银行子页面）
class CInsureItemWealth
{
public:
	CInsureItemWealth(CDlgInsure* pDlgInsureMain);
	~CInsureItemWealth();
	
	//缓冲数据
	VOID InitData();
	//VOID LoadDataFromUI();
	//VOID UpdateData();
	
	//界面操控
	VOID InitUI();
	VOID UpdateUI();

	//界面逻辑
	//VOID OnDefault();
	//VOID DoApply();

	//应用函数
	void Notify(TNotifyUI &  msg);

private:
	//我的资产容器
	CDlgInsure* m_pDlgInsureMain;
	CContainerUI* m_pContainerUI;
};


//////////////////////////////////////////////////////////////////////////////////
//游戏币存取（银行子页面）
class CInsureItemAccess
{
public:
	CInsureItemAccess(CDlgInsure* pDlgInsureMain);
	~CInsureItemAccess();

	//缓冲数据
	VOID InitData();
	VOID LoadDataFromUI();
	//VOID UpdateData();

	//界面操控
	VOID InitUI();
	VOID UpdateUI();

	//界面逻辑
	//VOID OnDefault();
	//VOID DoApply();

	//显示大写
	VOID ShowCapital();

	//应用函数
	void Notify(TNotifyUI &  msg);
	void OnBnClickAccess(TNotifyUI& msg);
	VOID OnBnClickedSaveScore();
	VOID OnBnClickedTakeScore();

	void SetEditFocus();

private:
	//我的资产容器
	CDlgInsure*		m_pDlgInsureMain;
	CContainerUI*	m_pContainerUI;
	CEditUI*		m_pEditInOut;
	CEditUI*		m_pEditPassWord;
	CLabelUI*		m_LabelInOut;

	SCORE			m_InOutScore;		//输入的分数
};


//////////////////////////////////////////////////////////////////////////////////
//金币赠送（银行子页面）
class CInsureItemPresent
{
public:
	CInsureItemPresent(CDlgInsure* pDlgInsureMain);
	~CInsureItemPresent();

	//缓冲数据
	VOID InitData();
	//VOID LoadDataFromUI();
	//VOID UpdateData();

	//界面操控
	VOID InitUI();
	VOID UpdateUI();

	//界面逻辑
	//VOID OnDefault();
	//VOID DoApply();

	//应用函数
	void Notify(TNotifyUI &  msg);

	//确定赠送
	VOID OnBnClickedTransferScore();
	//依据ID
	VOID OnBnCheckedTransferByUserID();
	//依据昵称
	VOID OnBnCheckedTransferByNickName();

private:
	//我的资产容器
	CDlgInsure* m_pDlgInsureMain;
	CContainerUI* m_pContainerUI;

	CRadioButtonUI* m_pRadioByNickName;		//昵称单选
	CRadioButtonUI* m_pRadioByUserID;		//ID单选
	CEditUI*		m_pEditAccount;			//昵称
	CEditUI*		m_pEditAmount;			//数量
	CEditUI*		m_pEditInsurePass;		//密码
	CEditUI*		m_pEditRemarks;			//备注
	CLabelUI*		m_pLabelTarget;			//接收ID or 昵称
	CLabelUI*		m_pLabelChineseNumeral;	//大写
};



//房间银行
class CDlgInsureServerNew : public CDlgInsure
{
	//函数定义
public:
	//构造函数
	CDlgInsureServerNew();
	//析构函数
	virtual ~CDlgInsureServerNew();




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

	VOID QueryUserInsureInfo();

	//变量定义
protected:
	ITCPSocket *					m_pITCPSocket;						//网络接口
	IClientUserItem *				m_pIMySelfUserItem;					//自己指针



};


#endif